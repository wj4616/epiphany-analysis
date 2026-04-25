/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — Passthrough stage in the wet signal path.

  OutputProtection (tanh) handles safety clipping at the final output.
  This module exists as a hook point for future wet-path processing
  (e.g., saturation, filtering) without disrupting the signal chain.
*/

#pragma once

#include <juce_core/juce_core.h>

class WetProcessor
{
public:
    WetProcessor() = default;
    ~WetProcessor() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for wet path loop
    void processSample(float& outL, float& outR, float inL, float inR);

    // Block-level processing (calls processSample internally)
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples);

    // Called on freeze transitions — currently a no-op, kept for API stability
    void notifyFreezeTransition();
};
