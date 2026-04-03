/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - PluginProcessor integration
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Parameters.h"
#include "PresetManager.h"
#include "modules/DryDelay.h"
#include "modules/TimeStretch.h"
#include "modules/Wavefolder.h"
#include "modules/ThresholdDetector.h"
#include "modules/WetProcessor.h"
#include "modules/Mixer.h"
#include "modules/LFO.h"
#include "modules/OutputProtection.h"

class PsycogAudioProcessor : public juce::AudioProcessor
{
public:
    PsycogAudioProcessor();
    ~PsycogAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "PsycogVST"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }

    int getNumPrograms() override { return Presets::numPresets; }
    int getCurrentProgram() override { return currentProgramIndex; }
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    // DSP Modules
    DryDelay dryDelay;
    TimeStretch timeStretch;
    Wavefolder wavefolder;
    ThresholdDetector thresholdDetector;
    WetProcessor wetProcessor;
    Mixer mixer;
    LFO lfo;
    OutputProtection outputProtection;

    // Freeze state
    PsycogConstants::FreezeMode lastFreezeMode = PsycogConstants::FreezeMode::Off;

    // Current preset index
    int currentProgramIndex = 0;

    // Temp buffers for signal routing
    juce::AudioBuffer<float> wetBuffer;
    juce::AudioBuffer<float> dryBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PsycogAudioProcessor)
};