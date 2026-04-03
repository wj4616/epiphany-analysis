/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - LFO
*/

#include "LFO.h"
#include "../utils/MathUtils.h"
#include "../Parameters.h"
#include <chrono>

LFO::LFO()
{
}

void LFO::prepare(double sr)
{
    sampleRate = sr;
    depthSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);

    // Pre-allocate buffer for max block size (no allocations in processBlock)
    blockSize = 4096;
    lfoValues.resize(blockSize);

    // Initialize random number generator for S&H
    randomEngine.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
    heldValue = 0.0f;

    // Start at phase 0
    phase = 0.0f;
}

void LFO::process(int numSamples)
{
    // CRITICAL: Per-sample processing
    // Phase increments per-sample, not per-block
    depthSmoother.setTargetValue(depth);

    for (int i = 0; i < numSamples; ++i)
    {
        // Smooth depth per-sample (CM-05 prevention)
        float smoothedDepth = depthSmoother.getNextValue();

        // Generate waveform value
        float value = 0.0f;

        switch (waveform)
        {
            case PsycogConstants::LfoWaveform::Sine:
                value = std::sin(2.0f * juce::MathConstants<float>::pi * phase);
                break;

            case PsycogConstants::LfoWaveform::Triangle:
                // Triangle: bipolar from -1 to +1
                // Simpler formula: 1 - 4 * |phase - 0.5|
                value = 1.0f - 4.0f * std::abs(phase - 0.5f);
                break;

            case PsycogConstants::LfoWaveform::Square:
                value = (phase < 0.5f) ? 1.0f : -1.0f;
                break;

            case PsycogConstants::LfoWaveform::SampleAndHold:
                // S&H: Generate new random value at start of each cycle
                // Detect cycle start when phase wraps around
                {
                    float nextPhase = phase + phaseIncrement;
                    if (nextPhase >= 1.0f || phase < phaseIncrement)
                    {
                        // Just started a new cycle
                        heldValue = randomDist(randomEngine);
                    }
                }
                value = heldValue;
                break;
        }

        // Store bipolar value scaled by depth
        lfoValues[i] = value * smoothedDepth;

        // CRITICAL: Increment phase per-sample
        float prevPhase = phase;
        phase += phaseIncrement;
        if (phase >= 1.0f)
            phase -= 1.0f;
    }
}

void LFO::setRate(float normalizedRate)
{
    // Convert normalized [0, 1] to actual rate using log scale
    // Rate range: 0.01 Hz to 20 Hz
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
}

float LFO::getValue(int sampleIndex) const
{
    return lfoValues[sampleIndex];
}