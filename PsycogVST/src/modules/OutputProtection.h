/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - OutputProtection

  Final soft-clip after mix. Catches hot signals from both wet and dry paths.
  Uses tanh for soft limiting. Nearly transparent below ±0.5 (< 0.04dB deviation).
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

class OutputProtection
{
public:
    OutputProtection() = default;
    ~OutputProtection() = default;

    // In-place: applies tanh soft-clip directly to the buffer.
    void processInPlace(float* left, float* right, int numSamples);

    // Out-of-place: reads from input, writes to output.
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples);
};