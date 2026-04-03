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

    // Initialize smoothers (20ms per spec)
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    foldAmountSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    foldOffsetSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    mixSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);

    // Allocate temp buffers
    wetBuffer.setSize(2, samplesPerBlock * 2);  // Double for safety
    dryBuffer.setSize(2, samplesPerBlock * 2);
}

void PsycogAudioProcessor::releaseResources()
{
    dryDelay.reset();
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

void PsycogAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // Handle mono input by copying to both channels
    bool isMonoInput = (totalNumInputChannels == 1);
    if (isMonoInput && totalNumOutputChannels >= 2)
    {
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
    }

    // Get input pointers
    const float* leftIn = buffer.getReadPointer(0);
    const float* rightIn = buffer.getReadPointer(1);

    // Get parameter values (raw, not normalized)
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

    freezeMode = static_cast<PsycogConstants::FreezeMode>(freezeModeInt);
    auto lfoWaveform = static_cast<PsycogConstants::LfoWaveform>(lfoWaveformInt);

    // Set LFO parameters and process
    lfo.setRate(lfoRate);
    lfo.setWaveform(lfoWaveform);
    lfo.setDepth(lfoDepth);
    lfo.targetStretch = targetStretch;
    lfo.targetPosition = targetPosition;
    lfo.targetFoldAmount = targetFoldAmount;
    lfo.targetFoldOffset = targetFoldOffset;
    lfo.process(numSamples);

    // Set smoother targets (per-sample getNextValue in loop below)
    stretchSmoother.setTargetValue(stretchNorm);
    positionSmoother.setTargetValue(positionNorm);
    foldAmountSmoother.setTargetValue(foldAmount);
    foldOffsetSmoother.setTargetValue(foldOffset);
    mixSmoother.setTargetValue(mix);

    // Get buffer pointers for wet path
    float* wetLeft = wetBuffer.getWritePointer(0);
    float* wetRight = wetBuffer.getWritePointer(1);
    float* dryLeft = dryBuffer.getWritePointer(0);
    float* dryRight = dryBuffer.getWritePointer(1);

    // Dry path: delay 2048 samples to match wet path latency
    dryDelay.process(dryLeft, dryRight, leftIn, rightIn, numSamples);

    // ========================================================================
    // Process wet path sample-by-sample with per-sample LFO modulation
    // This ensures LFO affects parameters correctly per the spec
    // ========================================================================
    for (int i = 0; i < numSamples; ++i)
    {
        // CRITICAL: Per-sample smoothing (CM-05 prevention)
        float smoothedStretch = stretchSmoother.getNextValue();
        float smoothedPosition = positionSmoother.getNextValue();
        float smoothedFoldAmount = foldAmountSmoother.getNextValue();
        float smoothedFoldOffset = foldOffsetSmoother.getNextValue();
        float smoothedMix = mixSmoother.getNextValue();

        // Apply LFO modulation to normalized parameter values
        float lfoValue = lfo.getValue(i);

        float finalStretch = smoothedStretch;
        float finalPosition = smoothedPosition;
        float finalFoldAmount = smoothedFoldAmount;
        float finalFoldOffset = smoothedFoldOffset;

        if (targetStretch)
            finalStretch = juce::jlimit(0.0f, 1.0f, smoothedStretch + lfoValue);
        if (targetPosition)
            finalPosition = juce::jlimit(0.0f, 1.0f, smoothedPosition + lfoValue);
        if (targetFoldAmount)
            finalFoldAmount = juce::jlimit(0.0f, 1.0f, smoothedFoldAmount + lfoValue);
        if (targetFoldOffset)
            finalFoldOffset = juce::jlimit(-1.0f, 1.0f, smoothedFoldOffset + lfoValue);

        // Note: LFO does NOT modulate mix (per spec)

        // ----------------------------------------------------------------
        // Wet path processing for this sample
        // ----------------------------------------------------------------

        // Input sample
        float wetL = leftIn[i];
        float wetR = rightIn[i];

        // TimeStretch / Freeze (simplified - uses current position)
        // For now, pass through if not frozen (granular engine is a stub)
        if (freezeMode == PsycogConstants::FreezeMode::Off)
        {
            // TODO: Implement granular time-stretch
            // Currently passes through - stretch parameter has no effect
            wetL = leftIn[i];
            wetR = rightIn[i];
        }
        // Note: For freeze modes, we'd need sample-by-sample freeze buffer access
        // This is a known limitation - freeze works at block level, not sample level

        // Wavefolder with LFO-modulated parameters
        float wfL, wfR;
        wavefolder.process(&wfL, &wfR, &wetL, &wetR, 1,
                          finalFoldAmount, finalFoldOffset, isMonoInput);

        // WetProcessor (soft-clip + auto-normalize)
        wetProcessor.process(&wfL, &wfR, &wfL, &wfR, 1);

        // Store for mixing
        wetLeft[i] = wfL;
        wetRight[i] = wfR;
    }

    // For freeze modes, process through TimeStretch at block level
    // This is a known limitation - freeze and LFO position work at different rates
    if (freezeMode != PsycogConstants::FreezeMode::Off)
    {
        timeStretch.process(wetLeft, wetRight, leftIn, rightIn, numSamples,
                           stretchNorm, positionNorm, freezeMode, threshold);
    }

    // Check threshold for auto-freeze (amplitude measured at Wavefolder output)
    if (freezeMode == PsycogConstants::FreezeMode::Auto)
    {
        float amplitude = wavefolder.getCurrentAmplitude();
        if (thresholdDetector.checkThreshold(amplitude, threshold))
        {
            timeStretch.triggerFreeze();
        }
    }

    // CRITICAL: Advance cooldown for threshold detector
    thresholdDetector.advanceCooldown(numSamples);

    // Mix wet and dry — mix is NOT modulated by LFO (per spec)
    // Use smoothed mix value (smoother was advanced per-sample in loop above)
    float smoothedMixForBlock = mixSmoother.getCurrentValue();
    mixer.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                  wetLeft, wetRight, dryLeft, dryRight,
                  numSamples, smoothedMixForBlock);

    // Final output protection (soft-clip)
    outputProtection.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getReadPointer(0), buffer.getReadPointer(1),
                            numSamples);
}

void PsycogAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    // Convert to float and process
    juce::AudioBuffer<float> floatBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            floatBuffer.setSample(channel, sample, static_cast<float>(buffer.getSample(channel, sample)));
        }
    }
    processBlock(floatBuffer, midiBuffer);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            buffer.setSample(channel, sample, static_cast<double>(floatBuffer.getSample(channel, sample)));
        }
    }
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