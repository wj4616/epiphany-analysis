/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — Passthrough (OutputProtection handles safety clipping)
*/

#include "WetProcessor.h"

void WetProcessor::prepare(double /*sampleRate*/, int /*maxSamples*/)
{
}

void WetProcessor::reset()
{
}

void WetProcessor::processSample(float& outL, float& outR, float inL, float inR)
{
    outL = inL;
    outR = inR;
}

void WetProcessor::process(float* leftOut, float* rightOut,
                           const float* leftIn, const float* rightIn,
                           int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        leftOut[i] = leftIn[i];
        rightOut[i] = rightIn[i];
    }
}

void WetProcessor::notifyFreezeTransition()
{
}
