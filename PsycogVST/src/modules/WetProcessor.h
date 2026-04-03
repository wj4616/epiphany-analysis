/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — Soft-clip + auto-normalize for wet signal

  Processing per sample: tanh(signal) -> auto-normalize -> output
  Auto-normalize: RMS to -6dBFS, 300ms attack / 1000ms release
  Freeze-aware: 100ms fast release during freeze transitions (200ms window)
  Noise gate: bypass below -60dBFS
  Gain clamp: 0.1 to 3.0 (~ -20dB to +10dB)
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include "../utils/Constants.h"

class WetProcessor
{
public:
    WetProcessor();
    ~WetProcessor() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for wet path loop
    void processSample(float& outL, float& outR, float inL, float inR);

    // Block-level processing (calls processSample internally)
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples);

    // Call when freeze buffer swaps — enables fast release for 200ms
    void notifyFreezeTransition();

private:
    double sampleRate = 48000.0;

    // Auto-normalize
    float targetRmsLinear = 0.0f;   // -6dBFS in linear
    float attackCoeff = 0.0f;       // 300ms attack
    float normalReleaseCoeff = 0.0f; // 1000ms release (normal)
    float fastReleaseCoeff = 0.0f;   // 100ms release (freeze transition)
    float currentGain = 1.0f;        // Current gain adjustment

    // EMA RMS tracking (replaces flawed running sum)
    float rmsPower = 0.0f;           // EMA of squared amplitude
    float rmsCoeff = 0.0f;           // EMA coefficient (~300ms window)

    // Noise gate
    float noiseGateThreshold = 0.0f;

    // Freeze transition state
    int freezeTransitionRemaining = 0;
    int freezeTransitionLength = 0;   // 200ms in samples
};
