/*
  PsycogVST - Interdimensional sound transformation plugin
  TimeStretch — 4-grain variable overlap-add engine

  Unfrozen: grains read from circular input buffer, hop size controls time-stretch
  Frozen: grains read from FreezeBuffer, stretch controls pitch, position controls read head

  Grain lifecycle: launch -> advance through grain window -> expire
  4 grain slots, pre-allocated. Hann window lookup table.
  ±64 sample jitter prevents metallic periodicity at extreme stretch.
  10ms crossfade on freeze/unfreeze mode transitions.
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include "FreezeBuffer.h"
#include "../utils/Constants.h"
#include "../Parameters.h"
#include <array>

class TimeStretch
{
public:
    TimeStretch();
    ~TimeStretch() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for wet path loop
    void processSample(float& outL, float& outR,
                       float inL, float inR,
                       float normalizedStretch, float normalizedPosition,
                       PsycogConstants::FreezeMode freezeMode);

    // Write input to freeze buffer (call once per block, ALWAYS)
    void writeFreezeBuffer(const float* leftIn, const float* rightIn, int numSamples);

    void triggerFreeze();
    void toggleFreeze();
    bool isFrozen() const { return freezeBuffer.isFrozen(); }

private:
    // Grain structure
    struct Grain
    {
        float sourcePosition = 0.0f;     // Where in source buffer to start reading
        int playbackPosition = 0;        // How far through the grain (0 to grainSize)
        bool active = false;
    };

    static constexpr int numGrains = 4;
    static constexpr int grainSize = PsycogConstants::grainSize;  // 2048
    static constexpr int inputBufferSize = grainSize * 8;          // 16384 samples headroom
    static constexpr int jitterRange = 64;                         // ±64 samples

    double sampleRate = 48000.0;

    // Grain state
    std::array<Grain, numGrains> grains;
    int nextGrainSlot = 0;
    int samplesSinceLastGrain = 0;

    // Hann window lookup table
    std::array<float, grainSize> hannWindow;

    // Circular input buffer
    std::vector<float> inputBufferL;
    std::vector<float> inputBufferR;
    int inputWritePos = 0;
    float inputReadHead = 0.0f;

    // Smoothed parameters (module owns smoothing)
    juce::SmoothedValue<float> stretchSmoother;
    juce::SmoothedValue<float> positionSmoother;

    // Freeze system
    FreezeBuffer freezeBuffer;
    PsycogConstants::FreezeMode lastFreezeMode = PsycogConstants::FreezeMode::Off;

    // Mode transition crossfade (10ms)
    float crossfadeProgress = 1.0f;
    int crossfadeLength = 0;
    float prevOutputL = 0.0f;
    float prevOutputR = 0.0f;

    // Lightweight LCG for grain jitter
    uint32_t jitterLcgState = 1;
    int nextJitter();

    // Internal processing
    void launchGrain(float stretch, float position, PsycogConstants::FreezeMode mode);
    void processGrainSample(Grain& grain, float& outL, float& outR,
                            PsycogConstants::FreezeMode mode, float stretchRate);
};
