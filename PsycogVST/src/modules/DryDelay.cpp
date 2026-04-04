/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - DryDelay
*/

#include "DryDelay.h"

DryDelay::DryDelay()
{
}

void DryDelay::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;
    bufferLength = delaySamples + 1;  // +1 for safety
    delayBuffer.setSize(2, bufferLength);
    delayBuffer.clear();
    writePos = 0;
}

void DryDelay::process(float* leftOut, float* rightOut,
                        const float* leftIn, const float* rightIn,
                        int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        // Write input to delay buffer
        delayBuffer.setSample(0, writePos, leftIn[i]);
        delayBuffer.setSample(1, writePos, rightIn[i]);

        // Read from delay position (2048 samples ago)
        int readPos = (writePos - delaySamples + bufferLength) % bufferLength;

        leftOut[i] = delayBuffer.getSample(0, readPos);
        rightOut[i] = delayBuffer.getSample(1, readPos);

        // Advance write position
        writePos = (writePos + 1) % bufferLength;
    }
}

void DryDelay::reset()
{
    delayBuffer.clear();
    writePos = 0;
}