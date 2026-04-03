/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - FreezeBuffer
*/

#include "FreezeBuffer.h"

FreezeBuffer::FreezeBuffer()
{
}

void FreezeBuffer::prepare(double sr)
{
    sampleRate = sr;

    // Allocate 3 seconds * 2 channels per spec
    bufferLength = static_cast<int>(PsycogConstants::freezeBufferSeconds * sampleRate);
    bufferA.setSize(2, bufferLength);
    bufferB.setSize(2, bufferLength);
    bufferA.clear();
    bufferB.clear();

    // Crossfade length (10ms per spec)
    crossfadeLength = static_cast<int>(PsycogConstants::freezeCrossfadeMs * sampleRate / 1000.0);
    crossfadeWindow.resize(crossfadeLength);

    // Generate Hann window for crossfade
    for (int i = 0; i < crossfadeLength; ++i)
    {
        crossfadeWindow[i] = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * i / (crossfadeLength - 1)));
    }

    // Cooldown (100ms per spec)
    cooldownLength = static_cast<int>(PsycogConstants::autoTriggerCooldownMs * sampleRate / 1000.0);

    reset();
}

void FreezeBuffer::reset()
{
    bufferA.clear();
    bufferB.clear();
    frozen = false;
    bufferAIsPlayback = false;
    writePosA = 0;
    writePosB = 0;
    readPos = 0;
    crossfadeProgress = 1.0f;
}

void FreezeBuffer::write(const float* leftIn, const float* rightIn, int numSamples)
{
    // Always write to the recording buffer (the one NOT playing)
    auto& recordBuffer = bufferAIsPlayback ? bufferB : bufferA;
    int& wPos = bufferAIsPlayback ? writePosB : writePosA;

    for (int i = 0; i < numSamples; ++i)
    {
        recordBuffer.setSample(0, wPos, leftIn[i]);
        recordBuffer.setSample(1, wPos, rightIn[i]);

        wPos = (wPos + 1) % bufferLength;
    }
}

void FreezeBuffer::read(float* leftOut, float* rightOut, int numSamples, float position)
{
    if (!frozen)
    {
        // Not frozen - output silence
        std::fill(leftOut, leftOut + numSamples, 0.0f);
        std::fill(rightOut, rightOut + numSamples, 0.0f);
        return;
    }

    // Read from the playback buffer at the given position
    auto& playBuffer = bufferAIsPlayback ? bufferA : bufferB;

    // Convert position (0-1) to sample position
    int startPos = static_cast<int>(position * (bufferLength - 1));
    startPos = juce::jlimit(0, bufferLength - 1, startPos);

    for (int i = 0; i < numSamples; ++i)
    {
        // Handle crossfade between old and new buffer
        float fadeInGain = 1.0f;
        float fadeOutGain = 0.0f;

        if (crossfadeProgress < 1.0f)
        {
            // Crossfading between buffers
            int crossfadeIndex = static_cast<int>(crossfadeProgress * crossfadeLength);
            crossfadeIndex = juce::jlimit(0, crossfadeLength - 1, crossfadeIndex);

            fadeInGain = crossfadeWindow[crossfadeIndex];
            fadeOutGain = 1.0f - fadeInGain;

            crossfadeProgress += 1.0f / static_cast<float>(crossfadeLength);
            if (crossfadeProgress > 1.0f)
                crossfadeProgress = 1.0f;
        }

        // Calculate read position (loop within frozen buffer)
        int readSample = (startPos + i) % bufferLength;
        if (readSample < 0) readSample += bufferLength;

        // Read from playback buffer
        float leftSample = playBuffer.getSample(0, readSample);
        float rightSample = playBuffer.getSample(1, readSample);

        // If crossfading, also read from old buffer
        if (fadeOutGain > 0.0f)
        {
            auto& oldBuffer = bufferAIsPlayback ? bufferB : bufferA;
            float oldLeft = oldBuffer.getSample(0, readSample);
            float oldRight = oldBuffer.getSample(1, readSample);

            leftSample = leftSample * fadeInGain + oldLeft * fadeOutGain;
            rightSample = rightSample * fadeInGain + oldRight * fadeOutGain;
        }

        leftOut[i] = leftSample;
        rightOut[i] = rightSample;
    }

    // Advance cooldown
    if (cooldownRemaining > 0)
    {
        cooldownRemaining -= numSamples;
        if (cooldownRemaining < 0)
            cooldownRemaining = 0;
    }
}

void FreezeBuffer::toggleFreeze()
{
    if (!frozen)
    {
        frozen = true;
        bufferAIsPlayback = !bufferAIsPlayback;
        crossfadeProgress = 0.0f;
        readPos = 0;

        // Reset new recording buffer's writePos
        if (bufferAIsPlayback)
            writePosB = 0;
        else
            writePosA = 0;
    }
    else
    {
        frozen = false;
        crossfadeProgress = 1.0f;
    }
}

void FreezeBuffer::triggerFreeze()
{
    // For Auto mode: trigger freeze if not in cooldown
    if (cooldownRemaining <= 0)
    {
        // Swap playback buffer
        bufferAIsPlayback = !bufferAIsPlayback;
        frozen = true;
        crossfadeProgress = 0.0f;
        cooldownRemaining = cooldownLength;

        // Reset the new recording buffer's write position
        if (bufferAIsPlayback)
            writePosB = 0;  // B is now recording
        else
            writePosA = 0;  // A is now recording
    }
}

void FreezeBuffer::readSampleAt(int sampleIndex, float& outL, float& outR) const
{
    // Direct read from playback buffer — no crossfade or cooldown side effects
    // Used by granular engine grains for random-access position reads
    const auto& playBuffer = bufferAIsPlayback ? bufferA : bufferB;

    int idx = sampleIndex % bufferLength;
    if (idx < 0) idx += bufferLength;

    outL = playBuffer.getSample(0, idx);
    outR = playBuffer.getSample(1, idx);
}