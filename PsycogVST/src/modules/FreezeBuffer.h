/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 4: DSP Implementation - FreezeBuffer

  Double-buffered 3-second circular buffer for freeze effect.

  Uses two buffers (A and B), only one recording at a time:
  - Buffer A records incoming audio while Buffer B holds frozen content (read-only)
  - On freeze trigger: A stops recording and becomes playback buffer; B starts recording
  - On next trigger: they swap again

  This ensures recording never overwrites audio being played back.

  Cooldown for Auto mode re-triggering is handled by ThresholdDetector (100ms),
  NOT by FreezeBuffer. FreezeBuffer is pure storage + playback.
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include "../utils/Constants.h"

class FreezeBuffer
{
public:
    FreezeBuffer();
    ~FreezeBuffer() = default;

    void prepare(double sampleRate);
    void reset();

    // Write incoming audio to the recording buffer
    void write(const float* leftIn, const float* rightIn, int numSamples);

    // Read from the frozen playback buffer at position
    void read(float* leftOut, float* rightOut, int numSamples, float position);

    // Direct random-access read from playback buffer at a sample index.
    // Used by the granular engine — no crossfade or cooldown side effects.
    // sampleIndex wraps within buffer length.
    void readSampleAt(int sampleIndex, float& outL, float& outR) const;

    // Linear-interpolated read at a fractional sample position.
    // Used by the granular engine for smooth frozen playback at non-integer stretch rates.
    void readSampleAtFractional(float position, float& outL, float& outR) const;

    // Get playback buffer length in samples (for position mapping)
    int getBufferLength() const { return bufferLength; }

    // Freeze control
    void toggleFreeze();
    void triggerFreeze();  // For Auto mode
    bool isFrozen() const { return frozen; }

private:
    double sampleRate = 48000.0;
    int bufferLength = 0;  // 3 seconds * sampleRate

    // Double-buffer system
    juce::AudioBuffer<float> bufferA;  // Stereo
    juce::AudioBuffer<float> bufferB;

    bool frozen = false;
    bool bufferAIsPlayback = false;  // Which buffer is currently playing

    int writePosA = 0;    // Write position in buffer A
    int writePosB = 0;    // Write position in buffer B
    int readPos = 0;      // Read position in playback buffer

    // Crossfade state (10ms per spec)
    float crossfadeProgress = 1.0f;  // 1.0 = done
    int crossfadeLength = 0;

    // Hann window for crossfade
    std::vector<float> crossfadeWindow;
};