/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - PluginProcessor integration
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "utils/Constants.h"
#include "Version.h"

PsycogAudioProcessor::PsycogAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

PsycogAudioProcessor::~PsycogAudioProcessor()
{
}

void PsycogAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Report latency (2048 samples = grain window)
    setLatencySamples(PsycogConstants::grainSize);

    // Initialize all modules
    dryDelay.prepare(sampleRate, samplesPerBlock);
    timeStretch.prepare(sampleRate, samplesPerBlock);
    wavefolder.prepare(sampleRate, samplesPerBlock);
    thresholdDetector.prepare(sampleRate);
    wetProcessor.prepare(sampleRate, samplesPerBlock);
    mixer.prepare(sampleRate, samplesPerBlock);
    lfo.prepare(sampleRate);

    // Allocate temp buffers
    wetBuffer.setSize(2, samplesPerBlock * 2);  // Double for safety
    dryBuffer.setSize(2, samplesPerBlock * 2);
}

void PsycogAudioProcessor::releaseResources()
{
    dryDelay.reset();
    timeStretch.reset();
    wavefolder.reset();
    thresholdDetector.reset();
    wetProcessor.reset();
    mixer.reset();
    lfo.reset();
    // outputProtection is stateless — no reset needed
}

bool PsycogAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support stereo input -> stereo output
    // Also mono input -> stereo output (width from wavefolder offset)
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
        return layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() ||
               layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono();
    }
    return false;
}

juce::AudioProcessorParameter* PsycogAudioProcessor::getBypassParameter() const
{
    return apvts.getParameter(ParamIDs::bypass);
}

void PsycogAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // Bypass: pass audio through unprocessed
    if (*apvts.getRawParameterValue(ParamIDs::bypass) > 0.5f)
        return;

    // Reset gain-sensitive modules on preset change to prevent screeching
    // from stale auto-normalize gain or threshold detector state
    if (presetChangePending.exchange(false))
    {
        wetProcessor.reset();
        thresholdDetector.reset();
        lastFreezeMode = PsycogConstants::FreezeMode::Off;
    }

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // Handle mono input by copying to both channels
    bool isMonoInput = (totalNumInputChannels == 1);
    if (isMonoInput && totalNumOutputChannels >= 2)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);

    // Get input pointers
    const float* leftIn = buffer.getReadPointer(0);
    const float* rightIn = buffer.getReadPointer(1);

    // === 1. Read all raw parameter values (once per block) ===
    float stretchNorm = *apvts.getRawParameterValue(ParamIDs::stretch);
    float positionNorm = *apvts.getRawParameterValue(ParamIDs::position);
    int freezeModeInt = static_cast<int>(*apvts.getRawParameterValue(ParamIDs::freezeMode));
    float threshold = *apvts.getRawParameterValue(ParamIDs::threshold);
    float foldAmount = *apvts.getRawParameterValue(ParamIDs::foldAmount);
    float foldOffset = *apvts.getRawParameterValue(ParamIDs::foldOffset);
    float lfoRate = *apvts.getRawParameterValue(ParamIDs::lfoRate);
    int lfoWaveformInt = static_cast<int>(*apvts.getRawParameterValue(ParamIDs::lfoWaveform));
    float lfoDepth = *apvts.getRawParameterValue(ParamIDs::lfoDepth);
    bool targetStretch = *apvts.getRawParameterValue(ParamIDs::lfoTargetStretch) > 0.5f;
    bool targetPosition = *apvts.getRawParameterValue(ParamIDs::lfoTargetPosition) > 0.5f;
    bool targetFoldAmount = *apvts.getRawParameterValue(ParamIDs::lfoTargetFoldAmount) > 0.5f;
    bool targetFoldOffset = *apvts.getRawParameterValue(ParamIDs::lfoTargetFoldOffset) > 0.5f;
    float mix = *apvts.getRawParameterValue(ParamIDs::mix);

    auto currentFreezeMode = static_cast<PsycogConstants::FreezeMode>(freezeModeInt);
    auto lfoWaveform = static_cast<PsycogConstants::LfoWaveform>(lfoWaveformInt);

    // === 2. Configure LFO ===
    lfo.setRate(lfoRate);
    lfo.setWaveform(lfoWaveform);
    lfo.setDepth(lfoDepth);

    // === 3. Dry path: DryDelay (block-level) ===
    float* dryLeft = dryBuffer.getWritePointer(0);
    float* dryRight = dryBuffer.getWritePointer(1);
    dryDelay.process(dryLeft, dryRight, leftIn, rightIn, numSamples);

    // === 4. FreezeBuffer write (ALWAYS, even in Off mode — spec requirement) ===
    timeStretch.writeFreezeBuffer(leftIn, rightIn, numSamples);

    // === 4b. Handle Manual freeze toggle ===
    if (currentFreezeMode == PsycogConstants::FreezeMode::Manual
        && lastFreezeMode != PsycogConstants::FreezeMode::Manual)
    {
        timeStretch.toggleFreeze();  // Start freeze
    }
    else if (currentFreezeMode != PsycogConstants::FreezeMode::Manual
             && lastFreezeMode == PsycogConstants::FreezeMode::Manual)
    {
        timeStretch.toggleFreeze();  // Stop freeze
    }
    lastFreezeMode = currentFreezeMode;

    // === 5. Wet path PER-SAMPLE loop ===
    float* wetLeft = wetBuffer.getWritePointer(0);
    float* wetRight = wetBuffer.getWritePointer(1);

    for (int i = 0; i < numSamples; ++i)
    {
        // 5a. LFO advance (per-sample)
        float lfoValue = lfo.advance();

        // 5b. Compute modulated parameters (raw + LFO, clamped)
        float finalStretch = stretchNorm;
        float finalPosition = positionNorm;
        float finalFoldAmount = foldAmount;
        float finalFoldOffset = foldOffset;

        if (targetStretch)
            finalStretch = juce::jlimit(0.0f, 1.0f, stretchNorm + lfoValue);
        if (targetPosition)
            finalPosition = juce::jlimit(0.0f, 1.0f, positionNorm + lfoValue);
        if (targetFoldAmount)
            finalFoldAmount = juce::jlimit(0.0f, 1.0f, foldAmount + lfoValue);
        if (targetFoldOffset)
            finalFoldOffset = juce::jlimit(-1.0f, 1.0f, foldOffset + lfoValue);

        // 5c. TimeStretch (granular engine)
        float tsL, tsR;
        timeStretch.processSample(tsL, tsR, leftIn[i], rightIn[i],
                                  finalStretch, finalPosition, currentFreezeMode);

        // 5d. Wavefolder
        float wfL, wfR;
        wavefolder.processSample(wfL, wfR, tsL, tsR,
                                 finalFoldAmount, finalFoldOffset, isMonoInput);

        // 5e. Threshold detection (per-sample, from Wavefolder output)
        bool triggered = thresholdDetector.processSample(wfL, wfR, threshold);

        // 5f. Auto-freeze trigger
        if (triggered && currentFreezeMode == PsycogConstants::FreezeMode::Auto)
        {
            timeStretch.triggerFreeze();
            wetProcessor.notifyFreezeTransition();
        }

        // 5g. WetProcessor (soft-clip + auto-normalize)
        float wpL, wpR;
        wetProcessor.processSample(wpL, wpR, wfL, wfR);

        // 5h. Store in wet buffer
        wetLeft[i] = wpL;
        wetRight[i] = wpR;
    }

    // === 6. Mixer (block-level, smooths mix internally) ===
    mixer.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                  wetLeft, wetRight, dryLeft, dryRight,
                  numSamples, mix);

    // === 7. Output protection (block-level, in-place) ===
    outputProtection.processInPlace(buffer.getWritePointer(0), buffer.getWritePointer(1),
                                    numSamples);

}

juce::AudioProcessorEditor* PsycogAudioProcessor::createEditor()
{
    return new PsycogAudioProcessorEditor(*this);
}

void PsycogAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->setAttribute("version", PsycogVersion::current);
    xml->setAttribute("currentProgram", currentProgramIndex);
    copyXmlToBinary(*xml, destData);
}

void PsycogAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType()))
    {
        // Version migration (future versions can add logic here)
        juce::String version = xml->getStringAttribute("version", PsycogVersion::current);

        // Restore state
        apvts.replaceState(juce::ValueTree::fromXml(*xml));

        // Restore current program index if present
        if (xml->hasAttribute("currentProgram"))
        {
            int savedProgram = xml->getIntAttribute("currentProgram", 0);
            if (savedProgram >= 0 && savedProgram < Presets::numPresets)
                currentProgramIndex = savedProgram;
        }
    }
}

void PsycogAudioProcessor::setCurrentProgram(int index)
{
    if (index >= 0 && index < Presets::numPresets)
    {
        currentProgramIndex = index;
        Presets::applyPreset(apvts, index);
        presetChangePending.store(true);  // Audio thread will reset modules
    }
}

const juce::String PsycogAudioProcessor::getProgramName(int index)
{
    return Presets::getPresetName(index);
}

void PsycogAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    // Factory preset names cannot be changed
    // This is a no-op as per JUCE pattern for plugins with fixed presets
    juce::ignoreUnused(index, newName);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PsycogAudioProcessor();
}
