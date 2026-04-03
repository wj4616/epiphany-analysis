/*
  PsycogVST - Interdimensional sound transformation plugin
  LFO — Single LFO for multi-target modulation

  Computes inline per-sample via advance(). No pre-allocated buffer.
  Uses lightweight LCG random for audio thread safety (no std::mt19937).

  Targets (when enabled): stretch, position, foldAmount, foldOffset
  NOT modulated: mix
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include "../utils/Constants.h"

class LFO
{
public:
    LFO();
    ~LFO() = default;

    void prepare(double sampleRate);
    void reset();

    void setRate(float normalizedRate);
    void setWaveform(PsycogConstants::LfoWaveform wf);
    void setDepth(float d);

    // Advance phase by one sample and return bipolar value in [-depth, +depth]
    // MUST be called once per sample in the wet path loop
    float advance();

private:
    double sampleRate = 48000.0;
    float phase = 0.0f;           // Current phase [0, 1)
    float phaseIncrement = 0.0f;  // Per-sample increment
    float depth = 0.0f;           // Modulation depth [0, 1]
    PsycogConstants::LfoWaveform waveform = PsycogConstants::LfoWaveform::Sine;

    // Smoothed depth (20ms per spec)
    juce::SmoothedValue<float> depthSmoother;

    // S&H held value
    float heldValue = 0.0f;

    // Lightweight LCG random for audio thread safety
    uint32_t lcgState = 1;
    float nextRandom();  // Returns uniform float in [-1, 1]
};
