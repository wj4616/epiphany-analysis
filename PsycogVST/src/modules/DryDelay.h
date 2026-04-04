/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - DryDelay

  2048-sample delay for dry path to match wet path latency.
  The granular time-stretch requires ~46ms (2048 samples) of look-ahead.
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class DryDelay
{
public:
    DryDelay();
    ~DryDelay() = default;

    void prepare(double sampleRate, int maxSamples);
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples);
    void reset();

private:
    static constexpr int delaySamples = 2048;  // Fixed latency to match granular engine

    double sampleRate = 48000.0;
    juce::AudioBuffer<float> delayBuffer;
    int writePos = 0;
    int bufferLength = 0;
};