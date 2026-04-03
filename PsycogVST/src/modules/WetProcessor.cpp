/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — EMA RMS, freeze-aware release (Issue #13b)
*/

#include "WetProcessor.h"
#include "../utils/MathUtils.h"

WetProcessor::WetProcessor()
{
}

void WetProcessor::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;

    // Target -6dBFS per spec
    targetRmsLinear = MathUtils::dbToGain(static_cast<float>(PsycogConstants::autoNormalizeTargetDb));
    noiseGateThreshold = MathUtils::dbToGain(static_cast<float>(PsycogConstants::noiseGateThresholdDb));

    // Attack coefficient (300ms)
    float attackTime = static_cast<float>(PsycogConstants::autoNormalizeAttackMs) / 1000.0f;
    attackCoeff = std::exp(-1.0f / (attackTime * static_cast<float>(sampleRate)));

    // Release coefficients
    normalReleaseCoeff = std::exp(-1.0f / (1.0f * static_cast<float>(sampleRate)));    // 1000ms
    fastReleaseCoeff = std::exp(-1.0f / (0.1f * static_cast<float>(sampleRate)));       // 100ms

    // RMS EMA coefficient (~300ms window)
    rmsCoeff = std::exp(-1.0f / (attackTime * static_cast<float>(sampleRate)));

    // Freeze transition window (200ms)
    freezeTransitionLength = static_cast<int>(0.2 * sampleRate);

    reset();
}

void WetProcessor::reset()
{
    rmsPower = 0.0f;
    currentGain = 1.0f;
    freezeTransitionRemaining = 0;
}

void WetProcessor::processSample(float& outL, float& outR, float inL, float inR)
{
    // 1. Soft-clip (tanh)
    float leftClipped = std::tanh(inL);
    float rightClipped = std::tanh(inR);

    // 2. Update EMA RMS (replaces flawed running sum)
    float samplePower = (leftClipped * leftClipped + rightClipped * rightClipped) * 0.5f;
    rmsPower = rmsCoeff * rmsPower + (1.0f - rmsCoeff) * samplePower;
    float currentRms = std::sqrt(rmsPower);

    // 3. Noise gate check
    if (currentRms < noiseGateThreshold)
    {
        // Below noise gate — bypass gain adjustment
        outL = leftClipped;
        outR = rightClipped;
        return;
    }

    // 4. Compute target gain
    float targetGain = targetRmsLinear / std::max(currentRms, 1e-6f);

    // Clamp gain (0.1 to 3.0 per spec)
    targetGain = juce::jlimit(static_cast<float>(PsycogConstants::gainMin),
                              static_cast<float>(PsycogConstants::gainMax),
                              targetGain);

    // 5. Apply attack/release to gain changes
    float releaseCoeff = normalReleaseCoeff;
    if (freezeTransitionRemaining > 0)
    {
        releaseCoeff = fastReleaseCoeff;
        --freezeTransitionRemaining;
    }

    if (targetGain > currentGain)
    {
        // Increasing gain (signal got quieter) — use attack
        currentGain = currentGain + (1.0f - attackCoeff) * (targetGain - currentGain);
    }
    else
    {
        // Decreasing gain (signal got louder) — use release
        currentGain = currentGain + (1.0f - releaseCoeff) * (targetGain - currentGain);
    }

    // 6. Apply gain
    outL = leftClipped * currentGain;
    outR = rightClipped * currentGain;
}

void WetProcessor::process(float* leftOut, float* rightOut,
                           const float* leftIn, const float* rightIn,
                           int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        processSample(leftOut[i], rightOut[i], leftIn[i], rightIn[i]);
    }
}

void WetProcessor::notifyFreezeTransition()
{
    freezeTransitionRemaining = freezeTransitionLength;
}
