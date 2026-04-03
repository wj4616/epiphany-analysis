/*
  PsycogVST - Interdimensional sound transformation plugin
  Wavefolder — Triangle wavefolding with stereo offset

  Stereo offset: L gets +offset, R gets -offset (both mono and stereo input).
  Module owns parameter smoothing (20ms ramp).
  NOTE: Amplitude is no longer tracked here. ThresholdDetector receives
  wavefolder output directly in the per-sample loop (Task 10 signal flow).
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class Wavefolder
{
public:
    Wavefolder();
    ~Wavefolder() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for the wet path loop
    // foldAmount/foldOffset are raw+LFO values; smoothed internally
    void processSample(float& outL, float& outR,
                       float inL, float inR,
                       float foldAmount, float foldOffset, bool isMono);

    // Block-level processing (calls processSample internally, for tests)
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples, float foldAmount, float foldOffset, bool isMono);

private:
    double sampleRate = 48000.0;

    // Module owns smoothing (20ms per spec)
    juce::SmoothedValue<float> foldAmountSmoother;
    juce::SmoothedValue<float> offsetSmoother;
};
