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

    // Set LFO parameters (advance() is called per-sample in the loop below)
    lfo.setRate(lfoRate);
    lfo.setWaveform(lfoWaveform);
    lfo.setDepth(lfoDepth);

    // Get buffer pointers for wet path
    float* wetLeft = wetBuffer.getWritePointer(0);
    float* wetRight = wetBuffer.getWritePointer(1);
    float* dryLeft = dryBuffer.getWritePointer(0);
    float* dryRight = dryBuffer.getWritePointer(1);

    // Dry path: delay 2048 samples to match wet path latency
    dryDelay.process(dryLeft, dryRight, leftIn, rightIn, numSamples);

    for (int i = 0; i < numSamples; ++i)
    {
        // LFO advances per-sample
        float lfoValue = lfo.advance();

        // Apply LFO modulation to raw parameter values (modules smooth internally)
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

        // Wet path processing for this sample (same modules, raw+LFO values)
        float wetL = leftIn[i];
        float wetR = rightIn[i];

        if (freezeMode == PsycogConstants::FreezeMode::Off)
        {
            wetL = leftIn[i];
            wetR = rightIn[i];
        }

        float wfL, wfR;
        wavefolder.process(&wfL, &wfR, &wetL, &wetR, 1,
                          finalFoldAmount, finalFoldOffset, isMonoInput);

        wetProcessor.process(&wfL, &wfR, &wfL, &wfR, 1);

        wetLeft[i] = wfL;
        wetRight[i] = wfR;
    }

    // Block-level freeze path (still uses old TimeStretch.process until Task 10)
    if (freezeMode != PsycogConstants::FreezeMode::Off)
    {
        timeStretch.process(wetLeft, wetRight, leftIn, rightIn, numSamples,
                           stretchNorm, positionNorm, freezeMode, threshold);
    }

    // === Mixer (pass raw mix — Mixer will get internal smoother in Task 8) ===
    mixer.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                  wetLeft, wetRight, dryLeft, dryRight,
                  numSamples, mix);

    // === Output protection ===
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