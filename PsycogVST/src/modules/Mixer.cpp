/*
  PsycogVST - Interdimensional sound transformation plugin
  Mixer — Equal-power crossfade with internal smoothing (EM-14, Issue #3)
*/

#include "Mixer.h"
#include "../utils/Constants.h"

Mixer::Mixer()
{
}

void Mixer::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;
    mixSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    mixSmoother.setCurrentAndTargetValue(1.0f);  // Default 100% wet
}

void Mixer::reset()
{
    mixSmoother.setCurrentAndTargetValue(1.0f);
}

void Mixer::process(float* leftOut, float* rightOut,
                    const float* wetLeft, const float* wetRight,
                    const float* dryLeft, const float* dryRight,
                    int numSamples, float mix)
{
    mixSmoother.setTargetValue(mix);

    const float halfPi = juce::MathConstants<float>::halfPi;

    for (int i = 0; i < numSamples; ++i)
    {
        // Per-sample smoothing (CM-05 prevention)
        float smoothedMix = mixSmoother.getNextValue();

        // Equal-power (cosine) crossfade (EM-14)
        float wetGain = std::sin(smoothedMix * halfPi);
        float dryGain = std::cos(smoothedMix * halfPi);

        leftOut[i]  = wetLeft[i] * wetGain + dryLeft[i] * dryGain;
        rightOut[i] = wetRight[i] * wetGain + dryRight[i] * dryGain;
    }
}
