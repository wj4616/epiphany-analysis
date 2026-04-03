/*
  PsycogVST - Interdimensional sound transformation plugin
  LFO — Inline per-sample computation
*/

#include "LFO.h"
#include "../Parameters.h"

LFO::LFO()
{
}

void LFO::prepare(double sr)
{
    sampleRate = sr;
    depthSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    depthSmoother.setCurrentAndTargetValue(0.0f);

    phase = 0.0f;
    phaseIncrement = 0.0f;

    // Seed LCG from address of this object (deterministic per instance, unique per run)
    lcgState = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)) | 1u;

    // Initialize S&H with a random value so first cycle isn't silent
    heldValue = nextRandom();
}

void LFO::reset()
{
    phase = 0.0f;
    depthSmoother.setCurrentAndTargetValue(0.0f);
    heldValue = nextRandom();
}

void LFO::setRate(float normalizedRate)
{
    float rate = ParamConversions::lfoRateFromNormalized(normalizedRate);
    phaseIncrement = static_cast<float>(rate / sampleRate);
}

void LFO::setWaveform(PsycogConstants::LfoWaveform wf)
{
    waveform = wf;
}

void LFO::setDepth(float d)
{
    depth = d;
    depthSmoother.setTargetValue(d);
}

float LFO::advance()
{
    float smoothedDepth = depthSmoother.getNextValue();

    // Generate waveform value (bipolar -1 to +1)
    float value = 0.0f;

    switch (waveform)
    {
        case PsycogConstants::LfoWaveform::Sine:
            value = std::sin(2.0f * juce::MathConstants<float>::pi * phase);
            break;

        case PsycogConstants::LfoWaveform::Triangle:
            value = 1.0f - 4.0f * std::abs(phase - 0.5f);
            break;

        case PsycogConstants::LfoWaveform::Square:
            value = (phase < 0.5f) ? 1.0f : -1.0f;
            break;

        case PsycogConstants::LfoWaveform::SampleAndHold:
            value = heldValue;
            break;
    }

    // Advance phase
    float prevPhase = phase;
    phase += phaseIncrement;
    if (phase >= 1.0f)
        phase -= 1.0f;

    // S&H: new random value when phase wraps
    // Fix for Issue #6: compare prevPhase vs phase, not phase vs phaseIncrement
    if (waveform == PsycogConstants::LfoWaveform::SampleAndHold && phase < prevPhase)
        heldValue = nextRandom();

    return value * smoothedDepth;
}

float LFO::nextRandom()
{
    // Numerical Recipes LCG — lightweight, no allocation, deterministic
    lcgState = lcgState * 1664525u + 1013904223u;
    // Convert to float in [-1, 1]
    return static_cast<float>(static_cast<int32_t>(lcgState)) / 2147483648.0f;
}
