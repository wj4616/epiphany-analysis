/*
  PsycogVST - Interdimensional sound transformation plugin
  Wavefolder — Fix stereo offset: L=+offset, R=-offset (Issue #4)
*/

#include "Wavefolder.h"
#include "../utils/Constants.h"
#include "../utils/MathUtils.h"

Wavefolder::Wavefolder()
{
}

void Wavefolder::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;
    foldAmountSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    offsetSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    foldAmountSmoother.setCurrentAndTargetValue(0.0f);
    offsetSmoother.setCurrentAndTargetValue(0.0f);
}

void Wavefolder::reset()
{
    foldAmountSmoother.setCurrentAndTargetValue(0.0f);
    offsetSmoother.setCurrentAndTargetValue(0.0f);
}

void Wavefolder::processSample(float& outL, float& outR,
                                float inL, float inR,
                                float foldAmount, float foldOffset, bool isMono)
{
    // Set targets (called every sample — SmoothedValue handles this efficiently)
    foldAmountSmoother.setTargetValue(foldAmount);
    offsetSmoother.setTargetValue(foldOffset);

    // Per-sample smoothing (CM-05 prevention)
    float smoothedFold = foldAmountSmoother.getNextValue();
    float smoothedOffset = offsetSmoother.getNextValue();

    // CRITICAL FIX (Issue #4): L gets +offset, R gets -offset
    // This creates stereo width for BOTH mono and stereo input
    if (isMono)
    {
        outL = MathUtils::applyWavefold(inL, smoothedFold, +smoothedOffset);
        outR = MathUtils::applyWavefold(inL, smoothedFold, -smoothedOffset);
    }
    else
    {
        // Stereo: L channel gets +offset, R channel gets -offset
        outL = MathUtils::applyWavefold(inL, smoothedFold, +smoothedOffset);
        outR = MathUtils::applyWavefold(inR, smoothedFold, -smoothedOffset);
    }

}

void Wavefolder::process(float* leftOut, float* rightOut,
                          const float* leftIn, const float* rightIn,
                          int numSamples, float foldAmount, float foldOffset, bool isMono)
{
    for (int i = 0; i < numSamples; ++i)
    {
        processSample(leftOut[i], rightOut[i], leftIn[i], rightIn[i],
                      foldAmount, foldOffset, isMono);
    }
}
