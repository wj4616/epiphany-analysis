/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - OutputProtection
*/

#include "OutputProtection.h"

void OutputProtection::processInPlace(float* left, float* right, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = std::tanh(left[i]);
        right[i] = std::tanh(right[i]);
    }
}

void OutputProtection::process(float* leftOut, float* rightOut,
                                const float* leftIn, const float* rightIn,
                                int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        leftOut[i] = std::tanh(leftIn[i]);
        rightOut[i] = std::tanh(rightIn[i]);
    }
}