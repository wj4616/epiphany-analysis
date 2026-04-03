/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - TimeStretch

  NOTE: Full granular overlap-add implementation is pending.
  This version handles freeze correctly but uses pass-through for Off mode.
  The granular engine for time-stretching without freeze is a stub.
*/

#include "TimeStretch.h"
#include "../utils/Constants.h"
#include "../utils/MathUtils.h"

TimeStretch::TimeStretch()
{
    // Generate Hann window for granular processing
    grainWindow.resize(grainSize);
    for (int i = 0; i < grainSize; ++i)
    {
        grainWindow[i] = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * i / (grainSize - 1)));
    }

    // Allocate input buffers
    inputBufferL.resize(grainSize * 4);  // 4x grain size for overlap
    inputBufferR.resize(grainSize * 4);
}

void TimeStretch::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;
    currentStretch = 1.0f;

    // Initialize freeze buffer
    freezeBuffer.prepare(sampleRate);

    // Initialize smoothers (20ms per spec)
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    stretchSmoother.setCurrentAndTargetValue(0.5f);  // Normalized 1x
    positionSmoother.setCurrentAndTargetValue(0.5f);

    // Clear input buffers
    std::fill(inputBufferL.begin(), inputBufferL.end(), 0.0f);
    std::fill(inputBufferR.begin(), inputBufferR.end(), 0.0f);
    inputWritePos = 0;
    grainReadPos = 0;
}

void TimeStretch::process(float* leftOut, float* rightOut,
                          const float* leftIn, const float* rightIn,
                          int numSamples, float normalizedStretch, float normalizedPosition,
                          PsycogConstants::FreezeMode freezeMode, float threshold)
{
    // Set smoothing targets
    stretchSmoother.setTargetValue(normalizedStretch);
    positionSmoother.setTargetValue(normalizedPosition);

    // Track current stretch for reporting
    currentStretch = ParamConversions::stretchFromNormalized(normalizedStretch);

    for (int i = 0; i < numSamples; ++i)
    {
        // CRITICAL: Per-sample smoothing (CM-05 prevention)
        float smoothedStretchNorm = stretchSmoother.getNextValue();
        float smoothedPosition = positionSmoother.getNextValue();

        // Store input in buffer for potential granular processing
        inputBufferL[inputWritePos] = leftIn[i];
        inputBufferR[inputWritePos] = rightIn[i];
        inputWritePos = (inputWritePos + 1) % static_cast<int>(inputBufferL.size());

        switch (freezeMode)
        {
            case PsycogConstants::FreezeMode::Off:
                // Off mode: TODO - implement granular time-stretch
                // Currently passes through input
                // Granular overlap-add would go here
                leftOut[i] = leftIn[i];
                rightOut[i] = rightIn[i];
                break;

            case PsycogConstants::FreezeMode::Manual:
            case PsycogConstants::FreezeMode::Auto:
                // Manual/Auto mode: use freeze buffer
                if (freezeBuffer.isFrozen())
                {
                    // Read from frozen buffer at position
                    float outL, outR;
                    freezeBuffer.read(&outL, &outR, 1, smoothedPosition);
                    leftOut[i] = outL;
                    rightOut[i] = outR;
                }
                else
                {
                    // Not frozen: pass through
                    leftOut[i] = leftIn[i];
                    rightOut[i] = rightIn[i];
                }

                // Always write incoming audio for potential freeze
                freezeBuffer.write(&leftIn[i], &rightIn[i], 1);
                break;
        }
    }
}

void TimeStretch::triggerFreeze()
{
    freezeBuffer.triggerFreeze();
}