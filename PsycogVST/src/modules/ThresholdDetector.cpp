/*
  PsycogVST - Interdimensional sound transformation plugin
  ThresholdDetector — 10ms EMA with 100ms cooldown (Issue #8)
*/

#include "ThresholdDetector.h"

ThresholdDetector::ThresholdDetector()
{
}

void ThresholdDetector::prepare(double sr)
{
    sampleRate = sr;

    // 10ms EMA time constant
    // alpha = 1 - exp(-1 / (timeConstant * sampleRate))
    alpha = 1.0f - std::exp(-1.0f / (0.01f * static_cast<float>(sampleRate)));

    // 100ms cooldown per spec
    cooldownLength = static_cast<int>(PsycogConstants::autoTriggerCooldownMs * sampleRate / 1000.0);

    reset();
}

void ThresholdDetector::reset()
{
    rmsPower = 0.0f;
    cooldownRemaining = 0;
}

bool ThresholdDetector::processSample(float ampL, float ampR, float threshold)
{
    // Track peak of L/R
    float peak = std::max(std::abs(ampL), std::abs(ampR));

    // Update 10ms EMA of squared amplitude
    rmsPower = alpha * (peak * peak) + (1.0f - alpha) * rmsPower;
    float rms = std::sqrt(rmsPower);

    // Decrement cooldown
    if (cooldownRemaining > 0)
    {
        --cooldownRemaining;
        return false;
    }

    // Check threshold
    if (rms >= threshold)
    {
        cooldownRemaining = cooldownLength;
        return true;
    }

    return false;
}
