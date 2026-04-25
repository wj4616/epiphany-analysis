/*
  PsycogVST - Interdimensional sound transformation plugin
  TimeStretch — 4-grain overlap-add engine (Issue #7)
*/

#include "TimeStretch.h"
#include "../utils/Constants.h"
#include "../utils/MathUtils.h"

TimeStretch::TimeStretch()
{
    // Pre-compute Hann window
    for (int i = 0; i < grainSize; ++i)
    {
        hannWindow[i] = 0.5f * (1.0f - std::cos(
            2.0f * juce::MathConstants<float>::pi * static_cast<float>(i)
            / static_cast<float>(grainSize - 1)));
    }
}

void TimeStretch::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;

    inputBufferL.resize(inputBufferSize, 0.0f);
    inputBufferR.resize(inputBufferSize, 0.0f);
    inputWritePos = 0;
    inputReadHead = 0.0f;

    freezeBuffer.prepare(sampleRate);

    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    stretchSmoother.setCurrentAndTargetValue(0.5f);
    positionSmoother.setCurrentAndTargetValue(0.5f);

    crossfadeLength = static_cast<int>(PsycogConstants::freezeCrossfadeMs * sampleRate / 1000.0);

    jitterLcgState = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)) | 1u;

    reset();
}

void TimeStretch::reset()
{
    for (auto& g : grains)
        g.active = false;

    nextGrainSlot = 0;
    samplesSinceLastGrain = grainSize;  // Launch a grain immediately
    inputWritePos = 0;
    inputReadHead = 0.0f;
    std::fill(inputBufferL.begin(), inputBufferL.end(), 0.0f);
    std::fill(inputBufferR.begin(), inputBufferR.end(), 0.0f);

    freezeBuffer.reset();
    lastFreezeMode = PsycogConstants::FreezeMode::Off;
    crossfadeProgress = 1.0f;
    prevOutputL = 0.0f;
    prevOutputR = 0.0f;
}

void TimeStretch::writeFreezeBuffer(const float* leftIn, const float* rightIn, int numSamples)
{
    freezeBuffer.write(leftIn, rightIn, numSamples);
}

int TimeStretch::nextJitter()
{
    jitterLcgState = jitterLcgState * 1664525u + 1013904223u;
    return static_cast<int>(jitterLcgState % (2 * jitterRange + 1)) - jitterRange;
}

void TimeStretch::launchGrain(float stretch, float position, PsycogConstants::FreezeMode mode)
{
    for (int attempt = 0; attempt < numGrains; ++attempt)
    {
        int slot = (nextGrainSlot + attempt) % numGrains;
        if (!grains[slot].active)
        {
            Grain& g = grains[slot];
            g.playbackPosition = 0;
            g.active = true;

            // Scale jitter with distance from unity stretch.
            // At stretch=1.0x, jitter=0 (perfect reconstruction).
            // At stretch>=2.0x, full ±64 sample jitter (breaks periodicity).
            float jitterScale = juce::jlimit(0.0f, 1.0f, std::abs(stretch - 1.0f));
            int rawJitter = nextJitter();
            int jitter = static_cast<int>(rawJitter * jitterScale);

            if (mode == PsycogConstants::FreezeMode::Off)
            {
                float readPos = inputReadHead + static_cast<float>(jitter);
                while (readPos < 0.0f) readPos += static_cast<float>(inputBufferSize);
                while (readPos >= static_cast<float>(inputBufferSize)) readPos -= static_cast<float>(inputBufferSize);
                g.sourcePosition = readPos;
            }
            else
            {
                int bufLen = static_cast<int>(PsycogConstants::freezeBufferSeconds * sampleRate);
                float startSample = position * static_cast<float>(bufLen - 1) + static_cast<float>(jitter);
                startSample = juce::jlimit(0.0f, static_cast<float>(bufLen - 1), startSample);
                g.sourcePosition = startSample;
            }

            nextGrainSlot = (slot + 1) % numGrains;
            return;
        }
    }
}

void TimeStretch::processGrainSample(Grain& grain, float& outL, float& outR,
                                      PsycogConstants::FreezeMode mode, float stretchRate)
{
    if (!grain.active) return;

    float window = hannWindow[grain.playbackPosition];

    float sampleL = 0.0f;
    float sampleR = 0.0f;

    if (mode == PsycogConstants::FreezeMode::Off)
    {
        int readIdx = static_cast<int>(grain.sourcePosition + grain.playbackPosition) % inputBufferSize;
        if (readIdx < 0) readIdx += inputBufferSize;
        sampleL = inputBufferL[readIdx];
        sampleR = inputBufferR[readIdx];
    }
    else
    {
        float readPos = grain.sourcePosition + static_cast<float>(grain.playbackPosition) * stretchRate;
        freezeBuffer.readSampleAtFractional(readPos, sampleL, sampleR);
    }

    outL += sampleL * window;
    outR += sampleR * window;

    grain.playbackPosition++;
    if (grain.playbackPosition >= grainSize)
        grain.active = false;
}

void TimeStretch::processSample(float& outL, float& outR,
                                 float inL, float inR,
                                 float normalizedStretch, float normalizedPosition,
                                 PsycogConstants::FreezeMode freezeMode)
{
    // Store input in circular buffer
    inputBufferL[inputWritePos] = inL;
    inputBufferR[inputWritePos] = inR;
    inputWritePos = (inputWritePos + 1) % inputBufferSize;

    // Smooth parameters
    stretchSmoother.setTargetValue(normalizedStretch);
    positionSmoother.setTargetValue(normalizedPosition);
    float smoothedStretchNorm = stretchSmoother.getNextValue();
    float smoothedPosition = positionSmoother.getNextValue();

    // Convert normalized stretch to actual value
    float stretch = ParamConversions::stretchFromNormalized(smoothedStretchNorm);

    // Detect mode transition for crossfade
    bool modeChanged = (freezeMode != lastFreezeMode);
    if (modeChanged)
    {
        crossfadeProgress = 0.0f;
        lastFreezeMode = freezeMode;
    }

    // Grain launch interval: constant for smooth OLA output.
    // grainSize / 2 = 1024 samples between launches → 50% Hann overlap = unity sum.
    float overlapFactor = 2.0f;
    float outputHop = static_cast<float>(grainSize) / overlapFactor;

    // Check if time to launch a new grain
    samplesSinceLastGrain++;
    if (samplesSinceLastGrain >= static_cast<int>(outputHop))
    {
        launchGrain(stretch, smoothedPosition, freezeMode);
        samplesSinceLastGrain = 0;

        if (freezeMode == PsycogConstants::FreezeMode::Off)
        {
            // Input advance is separate from grain launch interval.
            // Divide by stretch so the read head moves slower than real-time
            // (stretch>1 = expansion) or faster (stretch<1 = compression).
            float inputAdvance = outputHop / stretch;
            inputReadHead += inputAdvance;
            while (inputReadHead >= static_cast<float>(inputBufferSize))
                inputReadHead -= static_cast<float>(inputBufferSize);
        }
    }

    // Sum all active grains
    float grainOutL = 0.0f;
    float grainOutR = 0.0f;

    float stretchRate = (freezeMode != PsycogConstants::FreezeMode::Off) ? stretch : 1.0f;

    for (auto& grain : grains)
    {
        if (grain.active)
            processGrainSample(grain, grainOutL, grainOutR, freezeMode, stretchRate);
    }

    // Apply mode transition crossfade
    if (crossfadeProgress < 1.0f)
    {
        float fadeIn = crossfadeProgress;
        float fadeOut = 1.0f - fadeIn;
        outL = grainOutL * fadeIn + prevOutputL * fadeOut;
        outR = grainOutR * fadeIn + prevOutputR * fadeOut;
        crossfadeProgress += 1.0f / static_cast<float>(crossfadeLength);
        if (crossfadeProgress > 1.0f)
            crossfadeProgress = 1.0f;
    }
    else
    {
        outL = grainOutL;
        outR = grainOutR;
    }

    // Save output for potential crossfade on next mode transition
    prevOutputL = outL;
    prevOutputR = outR;
}

void TimeStretch::triggerFreeze()
{
    freezeBuffer.triggerFreeze();
}

void TimeStretch::toggleFreeze()
{
    freezeBuffer.toggleFreeze();
}
