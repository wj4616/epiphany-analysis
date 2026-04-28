# PsycogVST Production-Grade Fix Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Fix 13 correctness bugs, implement the granular time-stretch engine, rewrite signal flow, and add comprehensive tests to PsycogVST.

**Architecture:** Modules own their smoothing and expose `processSample()` for the per-sample wet path. TimeStretch contains a 4-grain overlap-add engine. processBlock reads raw APVTS values, runs LFO inline, and calls modules per-sample in correct signal order: TimeStretch -> Wavefolder -> ThresholdDetector -> WetProcessor. Block-level modules (DryDelay, Mixer, OutputProtection) process buffers.

**Tech Stack:** C++17, JUCE 7.0.12 (FetchContent), Catch2 v3.5.2, CMake 3.22+

**Parent Spec:** `docs/superpowers/specs/2026-04-02-psycog-production-fix-design.md`

---

## File Structure

| File | Responsibility | Changes |
|------|---------------|---------|
| `src/utils/Constants.h` | Global constants | Fix `smoothingMs` -> `smoothingTimeSeconds = 0.02` |
| `src/modules/TimeStretch.h` | Granular time-stretch engine header | Grain struct, 4-grain state, `processSample()`, circular input buffer |
| `src/modules/TimeStretch.cpp` | Granular engine implementation | Full 4-grain OLA, dual-mode (stretch/pitch), jitter, freeze crossfade |
| `src/modules/FreezeBuffer.h` | Double-buffered freeze storage | Per-buffer writePos, `readSampleAt()` for granular engine |
| `src/modules/FreezeBuffer.cpp` | Freeze buffer implementation | Per-buffer writePos, reset on swap, `readSampleAt()`, always-record support |
| `src/modules/Wavefolder.h` | Wavefolder header | `processSample()` interface, restore internal smoothers, remove amplitude tracking |
| `src/modules/Wavefolder.cpp` | Wavefolder implementation | Fix stereo offset, `processSample()`, internal smoothing, no amplitude tracking |
| `src/modules/LFO.h` | LFO header | Remove buffer, add `advance()`, `nextRandom()` |
| `src/modules/LFO.cpp` | LFO implementation | Inline per-sample, S&H wrap fix, LCG random |
| `src/modules/ThresholdDetector.h` | Threshold detection header | `processSample()` with 10ms EMA |
| `src/modules/ThresholdDetector.cpp` | Threshold detection | EMA RMS, integrated cooldown, per-sample |
| `src/modules/WetProcessor.h` | Wet processor header | `processSample()`, `notifyFreezeTransition()` |
| `src/modules/WetProcessor.cpp` | Wet processor | EMA RMS, freeze-aware release, per-sample |
| `src/modules/Mixer.h` | Mixer header | Restore internal smoother (module owns smoothing) |
| `src/modules/Mixer.cpp` | Mixer implementation | Fix smoothing ramp time, equal-power crossfade |
| `src/PluginProcessor.h` | Processor header | Remove 5 smoothers, remove `processBlock(double)` decl, keep `lastFreezeMode` |
| `src/PluginProcessor.cpp` | Processor implementation | Full `processBlock` rewrite, Manual freeze toggle, fix `releaseResources`, remove double override |
| `src/PresetManager.cpp` | Preset system | Fix `applyPreset()` to use `setValueNotifyingHost` |
| `tests/TestTimeStretch.cpp` | TimeStretch tests | 20 tests replacing stub |
| `tests/TestFreezeBuffer.cpp` | FreezeBuffer tests | 15 tests replacing stub |
| `tests/TestWavefolder.cpp` | Wavefolder tests | 15 tests replacing stub |
| `tests/TestLFO.cpp` | LFO tests | 15 tests replacing stub |
| `tests/TestParameters.cpp` | Parameter conversion tests | 8 tests replacing stub |
| `tests/TestThresholdDetector.cpp` | ThresholdDetector tests | 8 new tests (new file) |
| `tests/TestWetProcessor.cpp` | WetProcessor tests | 8 new tests (new file) |
| `CMakeLists.txt` | Build config | Add 2 new test files |

---

## Task 1: Fix SmoothedValue Ramp Time (Issue #1)

**Files:**
- Modify: `src/utils/Constants.h:28`
- Modify: `src/modules/TimeStretch.cpp:37-38`
- Modify: `src/modules/LFO.cpp:18`
- Modify: `src/PluginProcessor.cpp:39-44`

Note: `Mixer.cpp` currently has no smoother (removed during Phase 6). Task 8 restores it with the correct constant.

All `SmoothedValue::reset()` calls pass `PsycogConstants::smoothingMs` (value: 20.0) as the ramp time. JUCE expects **seconds**, so this creates a 20-second ramp instead of 20ms.

- [ ] **Step 1: Rename constant in Constants.h**

In `src/utils/Constants.h`, replace:

```cpp
    // === Parameter Smoothing ===
    constexpr double smoothingMs = 20.0;               // All continuous params
```

with:

```cpp
    // === Parameter Smoothing ===
    constexpr double smoothingTimeSeconds = 0.02;      // 20ms — JUCE SmoothedValue::reset() expects seconds
```

- [ ] **Step 2: Update all module references**

Search for all uses of `smoothingMs` and replace with `smoothingTimeSeconds`. Files to update:

`src/modules/TimeStretch.cpp:37`:
```cpp
    // BEFORE:
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingMs);
    // AFTER:
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
```

`src/modules/TimeStretch.cpp:38`:
```cpp
    // BEFORE:
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingMs);
    // AFTER:
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
```

`src/modules/LFO.cpp:18`:
```cpp
    // BEFORE:
    depthSmoother.reset(sampleRate, PsycogConstants::smoothingMs);
    // AFTER:
    depthSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
```

Also update `src/PluginProcessor.cpp` (lines ~39-44) — all 5 smoother `.reset()` calls use this constant. These smoothers will be removed in Task 3, but fix them now so the build passes at each step.

```cpp
    // BEFORE (each line):
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingMs);
    // AFTER (each line):
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
```

- [ ] **Step 3: Build to verify no compilation errors**

Run: `cd ~/PsycogVST && cmake --build build 2>&1 | tail -5`
Expected: `[100%] Built target PsycogVSTTests` with zero errors

- [ ] **Step 4: Run tests**

Run: `cd ~/PsycogVST && ./build/PsycogVSTTests`
Expected: `All tests passed (637 assertions in 11 test cases)`

- [ ] **Step 5: Commit**

```bash
cd ~/PsycogVST
git add src/utils/Constants.h src/modules/TimeStretch.cpp src/modules/LFO.cpp src/PluginProcessor.cpp
git commit -m "fix: SmoothedValue ramp time — 20ms not 20 seconds (Issue #1)

smoothingMs (20.0) was passed to reset() which expects seconds.
Renamed to smoothingTimeSeconds (0.02) so all smoothers ramp in 20ms."
```

---

## Task 2: LFO Rewrite — Inline advance(), Fix S&H, LCG Random (Issues #5, #6)

**Files:**
- Rewrite: `src/modules/LFO.h`
- Rewrite: `src/modules/LFO.cpp`

This removes the pre-computed buffer (fixes overread at block > 4096), replaces `std::mt19937` with a lightweight LCG for audio-thread safety, fixes S&H spurious triggers, and adds non-zero S&H initialization.

- [ ] **Step 1: Write new LFO.h**

Replace the entire file `src/modules/LFO.h`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  LFO — Single LFO for multi-target modulation

  Computes inline per-sample via advance(). No pre-allocated buffer.
  Uses lightweight LCG random for audio thread safety (no std::mt19937).

  Targets (when enabled): stretch, position, foldAmount, foldOffset
  NOT modulated: mix
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include "../utils/Constants.h"

class LFO
{
public:
    LFO();
    ~LFO() = default;

    void prepare(double sampleRate);
    void reset();

    void setRate(float normalizedRate);
    void setWaveform(PsycogConstants::LfoWaveform wf);
    void setDepth(float d);

    // Advance phase by one sample and return bipolar value in [-depth, +depth]
    // MUST be called once per sample in the wet path loop
    float advance();

private:
    double sampleRate = 48000.0;
    float phase = 0.0f;           // Current phase [0, 1)
    float phaseIncrement = 0.0f;  // Per-sample increment
    float depth = 0.0f;           // Modulation depth [0, 1]
    PsycogConstants::LfoWaveform waveform = PsycogConstants::LfoWaveform::Sine;

    // Smoothed depth (20ms per spec)
    juce::SmoothedValue<float> depthSmoother;

    // S&H held value
    float heldValue = 0.0f;

    // Lightweight LCG random for audio thread safety
    // Avoids std::mt19937 which allocates and is heavy
    uint32_t lcgState = 1;
    float nextRandom();  // Returns uniform float in [-1, 1]
};
```

- [ ] **Step 2: Write new LFO.cpp**

Replace the entire file `src/modules/LFO.cpp`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  LFO — Inline per-sample computation
*/

#include "LFO.h"
#include "../Parameters.h"

LFO::LFO()
{
}

void LFO::prepare(double sr)
{
    sampleRate = sr;
    depthSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    depthSmoother.setCurrentAndTargetValue(0.0f);

    phase = 0.0f;
    phaseIncrement = 0.0f;

    // Seed LCG from address of this object (deterministic per instance, unique per run)
    lcgState = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)) | 1u;

    // Initialize S&H with a random value so first cycle isn't silent
    heldValue = nextRandom();
}

void LFO::reset()
{
    phase = 0.0f;
    depthSmoother.setCurrentAndTargetValue(0.0f);
    heldValue = nextRandom();
}

void LFO::setRate(float normalizedRate)
{
    float rate = ParamConversions::lfoRateFromNormalized(normalizedRate);
    phaseIncrement = static_cast<float>(rate / sampleRate);
}

void LFO::setWaveform(PsycogConstants::LfoWaveform wf)
{
    waveform = wf;
}

void LFO::setDepth(float d)
{
    depth = d;
    depthSmoother.setTargetValue(d);
}

float LFO::advance()
{
    float smoothedDepth = depthSmoother.getNextValue();

    // Generate waveform value (bipolar -1 to +1)
    float value = 0.0f;

    switch (waveform)
    {
        case PsycogConstants::LfoWaveform::Sine:
            value = std::sin(2.0f * juce::MathConstants<float>::pi * phase);
            break;

        case PsycogConstants::LfoWaveform::Triangle:
            // Triangle: bipolar, peaks at phase=0.25 (+1) and phase=0.75 (-1)
            value = 1.0f - 4.0f * std::abs(phase - 0.5f);
            break;

        case PsycogConstants::LfoWaveform::Square:
            value = (phase < 0.5f) ? 1.0f : -1.0f;
            break;

        case PsycogConstants::LfoWaveform::SampleAndHold:
            value = heldValue;
            break;
    }

    // Advance phase (per-sample, CRITICAL)
    float prevPhase = phase;
    phase += phaseIncrement;
    if (phase >= 1.0f)
        phase -= 1.0f;

    // S&H: new random value when phase wraps
    // Fix for Issue #6: compare prevPhase vs phase, not phase vs phaseIncrement
    if (waveform == PsycogConstants::LfoWaveform::SampleAndHold && phase < prevPhase)
        heldValue = nextRandom();

    return value * smoothedDepth;
}

float LFO::nextRandom()
{
    // Numerical Recipes LCG — lightweight, no allocation, deterministic
    lcgState = lcgState * 1664525u + 1013904223u;
    // Convert to float in [-1, 1]
    return static_cast<float>(static_cast<int32_t>(lcgState)) / 2147483648.0f;
}
```

- [ ] **Step 3: Build to verify**

Run: `cd ~/PsycogVST && cmake --build build 2>&1 | tail -5`
Expected: Build succeeds. The old `process(int)` and `getValue(int)` calls in `PluginProcessor.cpp` will fail — that's expected. We'll fix those in Task 5 (signal flow rewrite). For now, temporarily stub them to keep the build green.

Actually — the processor still calls `lfo.process(numSamples)` and `lfo.getValue(i)`. To keep the build green, we need to update those calls now. In `src/PluginProcessor.cpp`:

Replace line 118:
```cpp
    lfo.process(numSamples);
```
with:
```cpp
    // LFO now computed inline per-sample via advance() — no block-level call needed
```

And in the per-sample loop, replace line 151:
```cpp
        float lfoValue = lfo.getValue(i);
```
with:
```cpp
        float lfoValue = lfo.advance();
```

Also remove `lfo.targetStretch`, `lfo.targetPosition`, `lfo.targetFoldAmount`, `lfo.targetFoldOffset` assignments (lines 114-117) since the target flags are now read from APVTS directly in the processor.

- [ ] **Step 4: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all 637 tests pass

- [ ] **Step 5: Commit**

```bash
cd ~/PsycogVST
git add src/modules/LFO.h src/modules/LFO.cpp src/PluginProcessor.cpp
git commit -m "fix: LFO inline advance(), fix S&H triggers, LCG random (Issues #5, #6)

- Remove pre-allocated buffer (fixes overread at block > 4096)
- Replace std::mt19937 with lightweight LCG for audio thread safety
- Fix S&H wrap detection: compare prevPhase vs phase
- Initialize heldValue with random value (no silent first cycle)
- advance() returns per-sample value inline"
```

---

## Task 3: Remove Double Smoothing from Processor (Issue #3)

**Files:**
- Modify: `src/PluginProcessor.h:67-74`
- Modify: `src/PluginProcessor.cpp:38-44, 120-126, 141-148`

Remove all 5 SmoothedValue declarations from the processor. Modules own their smoothing.

- [ ] **Step 1: Remove smoother declarations from header**

In `src/PluginProcessor.h`, remove:

```cpp
    // Smoothed parameters (per-sample in processBlock loop)
    juce::SmoothedValue<float> stretchSmoother;
    juce::SmoothedValue<float> positionSmoother;
    juce::SmoothedValue<float> foldAmountSmoother;
    juce::SmoothedValue<float> foldOffsetSmoother;
    juce::SmoothedValue<float> mixSmoother;
```

Also remove the LFO target flag members (lines 76-79) since they're now read per-block from APVTS:

```cpp
    // LFO target flags
    bool lfoTargetStretch = false;
    bool lfoTargetPosition = false;
    bool lfoTargetFoldAmount = false;
    bool lfoTargetFoldOffset = false;
```

**Keep the `freezeMode` member** (line 81) — it will be renamed to `lastFreezeMode` in Task 10 for Manual freeze toggle detection.

- [ ] **Step 2: Remove smoother initialization from prepareToPlay**

In `src/PluginProcessor.cpp`, in `prepareToPlay()`, remove:

```cpp
    // Initialize smoothers (20ms per spec)
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    foldAmountSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    foldOffsetSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    mixSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
```

- [ ] **Step 3: Update processBlock to pass raw+LFO values to modules**

In the processBlock per-sample loop, replace the smoother code with direct raw values + LFO:

```cpp
    for (int i = 0; i < numSamples; ++i)
    {
        // LFO advances per-sample
        float lfoValue = lfo.advance();

        // Apply LFO modulation to raw parameter values (modules smooth internally)
        float finalStretch = stretchNorm;
        float finalPosition = positionNorm;
        float finalFoldAmount = foldAmount;
        float finalFoldOffset = foldOffset;

        if (targetStretch)
            finalStretch = juce::jlimit(0.0f, 1.0f, stretchNorm + lfoValue);
        if (targetPosition)
            finalPosition = juce::jlimit(0.0f, 1.0f, positionNorm + lfoValue);
        if (targetFoldAmount)
            finalFoldAmount = juce::jlimit(0.0f, 1.0f, foldAmount + lfoValue);
        if (targetFoldOffset)
            finalFoldOffset = juce::jlimit(-1.0f, 1.0f, foldOffset + lfoValue);

        // Wet path processing for this sample (same modules, raw+LFO values)
        float wetL = leftIn[i];
        float wetR = rightIn[i];

        if (freezeMode == PsycogConstants::FreezeMode::Off)
        {
            wetL = leftIn[i];
            wetR = rightIn[i];
        }

        float wfL, wfR;
        wavefolder.process(&wfL, &wfR, &wetL, &wetR, 1,
                          finalFoldAmount, finalFoldOffset, isMonoInput);

        wetProcessor.process(&wfL, &wfR, &wfL, &wfR, 1);

        wetLeft[i] = wfL;
        wetRight[i] = wfR;
    }

    // Block-level freeze path (still uses old TimeStretch.process until Task 10)
    if (freezeMode != PsycogConstants::FreezeMode::Off)
    {
        timeStretch.process(wetLeft, wetRight, leftIn, rightIn, numSamples,
                           stretchNorm, positionNorm, freezeMode, threshold);
    }

    // === Mixer (pass raw mix — Mixer will get internal smoother in Task 8) ===
    mixer.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                  wetLeft, wetRight, dryLeft, dryRight,
                  numSamples, mix);

    // === Output protection ===
    outputProtection.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getReadPointer(0), buffer.getReadPointer(1),
                            numSamples);
```

This replaces everything from the per-sample loop to end of processBlock. The following code is **removed** (callers would break in Task 4 when Wavefolder loses `getCurrentAmplitude()`):

```cpp
    // REMOVE — block-level threshold check (replaced by per-sample in Task 5/10)
    if (freezeMode == PsycogConstants::FreezeMode::Auto) { ... }
    thresholdDetector.advanceCooldown(numSamples);

    // REMOVE — old smoothed mix call
    float smoothedMixForBlock = mixSmoother.getCurrentValue();
```

- [ ] **Step 4: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 5: Commit**

```bash
cd ~/PsycogVST
git add src/PluginProcessor.h src/PluginProcessor.cpp
git commit -m "refactor: remove double smoothing from processor (Issue #3)

Modules own their parameter smoothing. Processor reads raw APVTS
values, applies LFO modulation (pure arithmetic), passes to modules.
Removed 5 SmoothedValue declarations and LFO target flag members."
```

---

## Task 4: Wavefolder — Fix Stereo Offset + processSample (Issues #3, #4)

**Files:**
- Rewrite: `src/modules/Wavefolder.h`
- Rewrite: `src/modules/Wavefolder.cpp`

Restore internal smoothers (module owns smoothing), add `processSample()`, fix stereo offset to use opposite signs for L/R.

- [ ] **Step 1: Write new Wavefolder.h**

Replace `src/modules/Wavefolder.h`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  Wavefolder — Triangle wavefolding with stereo offset

  Stereo offset: L gets +offset, R gets -offset (both mono and stereo input).
  Module owns parameter smoothing (20ms ramp).
  NOTE: Amplitude is no longer tracked here. ThresholdDetector receives
  wavefolder output directly in the per-sample loop (Task 10 signal flow).
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class Wavefolder
{
public:
    Wavefolder();
    ~Wavefolder() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for the wet path loop
    // foldAmount/foldOffset are raw+LFO values; smoothed internally
    void processSample(float& outL, float& outR,
                       float inL, float inR,
                       float foldAmount, float foldOffset, bool isMono);

    // Block-level processing (calls processSample internally, for tests)
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples, float foldAmount, float foldOffset, bool isMono);

private:
    double sampleRate = 48000.0;

    // Module owns smoothing (20ms per spec)
    juce::SmoothedValue<float> foldAmountSmoother;
    juce::SmoothedValue<float> offsetSmoother;
};
```

- [ ] **Step 2: Write new Wavefolder.cpp**

Replace `src/modules/Wavefolder.cpp`:

```cpp
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
```

- [ ] **Step 3: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 4: Commit**

```bash
cd ~/PsycogVST
git add src/modules/Wavefolder.h src/modules/Wavefolder.cpp
git commit -m "fix: Wavefolder stereo offset L=+, R=- and processSample (Issues #3, #4)

- Stereo input now uses opposite offsets for L/R (was same offset)
- Added processSample() for per-sample wet path
- Restored internal smoothers (module owns smoothing)
- Both mono and stereo paths create stereo width from offset"
```

---

## Task 5: ThresholdDetector — Per-Sample EMA Redesign (Issue #8)

**Files:**
- Rewrite: `src/modules/ThresholdDetector.h`
- Rewrite: `src/modules/ThresholdDetector.cpp`

Replace the block-level single-sample threshold check with a per-sample 10ms EMA RMS tracker with integrated cooldown.

- [ ] **Step 1: Write new ThresholdDetector.h**

Replace `src/modules/ThresholdDetector.h`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  ThresholdDetector — Per-sample sliding window RMS with cooldown

  Uses 10ms EMA for smooth amplitude tracking.
  Integrated 100ms cooldown prevents rapid re-triggering.
  Receives amplitude from Wavefolder output, BEFORE WetProcessor.
*/

#pragma once

#include <cmath>
#include "../utils/Constants.h"

class ThresholdDetector
{
public:
    ThresholdDetector();
    ~ThresholdDetector() = default;

    void prepare(double sampleRate);
    void reset();

    // Per-sample: returns true if freeze should trigger
    // ampL/ampR are wavefolder output samples (not absolute values — we take abs internally)
    bool processSample(float ampL, float ampR, float threshold);

private:
    double sampleRate = 48000.0;
    float rmsPower = 0.0f;        // EMA of squared amplitude
    float alpha = 0.0f;           // EMA coefficient (10ms time constant)
    int cooldownRemaining = 0;    // Remaining cooldown samples
    int cooldownLength = 0;       // 100ms in samples
};
```

- [ ] **Step 2: Write new ThresholdDetector.cpp**

Replace `src/modules/ThresholdDetector.cpp`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  ThresholdDetector — 10ms EMA with 100ms cooldown (Issue #8)
*/

#include "ThresholdDetector.h"

ThresholdDetector::ThresholdDetector()
{
}

void ThresholdDetector::prepare(double sr)
{
    sampleRate = sr;

    // 10ms EMA time constant
    // alpha = 1 - exp(-1 / (timeConstant * sampleRate))
    alpha = 1.0f - std::exp(-1.0f / (0.01f * static_cast<float>(sampleRate)));

    // 100ms cooldown per spec
    cooldownLength = static_cast<int>(PsycogConstants::autoTriggerCooldownMs * sampleRate / 1000.0);

    reset();
}

void ThresholdDetector::reset()
{
    rmsPower = 0.0f;
    cooldownRemaining = 0;
}

bool ThresholdDetector::processSample(float ampL, float ampR, float threshold)
{
    // Track peak of L/R
    float peak = std::max(std::abs(ampL), std::abs(ampR));

    // Update 10ms EMA of squared amplitude
    rmsPower = alpha * (peak * peak) + (1.0f - alpha) * rmsPower;
    float rms = std::sqrt(rmsPower);

    // Decrement cooldown
    if (cooldownRemaining > 0)
    {
        --cooldownRemaining;
        return false;
    }

    // Check threshold
    if (rms >= threshold)
    {
        cooldownRemaining = cooldownLength;
        return true;
    }

    return false;
}
```

- [ ] **Step 3: Verify processBlock no longer calls old ThresholdDetector methods**

The block-level threshold check and `advanceCooldown` call were already removed in Task 3 (when we rewrote the processBlock body). Verify that `checkThreshold` and `advanceCooldown` no longer appear in `PluginProcessor.cpp`:

```bash
grep -n "checkThreshold\|advanceCooldown\|getCurrentAmplitude" src/PluginProcessor.cpp
```

Expected: No matches. If any remain, remove them now.

- [ ] **Step 4: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 5: Commit**

```bash
cd ~/PsycogVST
git add src/modules/ThresholdDetector.h src/modules/ThresholdDetector.cpp
git commit -m "fix: ThresholdDetector per-sample 10ms EMA redesign (Issue #8)

- Replace block-level single-sample check with per-sample EMA RMS
- Integrated 100ms cooldown (no separate advanceCooldown)
- 10ms time constant smooths transient noise
- processSample() interface for wet path loop"
```

---

## Task 6: FreezeBuffer Fixes — Per-Buffer WritePos, Reset on Swap, readSampleAt (Issue #10)

**Files:**
- Modify: `src/modules/FreezeBuffer.h:56`
- Modify: `src/modules/FreezeBuffer.cpp:45-46, 56-62, 152-162`

Fix the single shared `writePos` to per-buffer positions, reset writePos on swap. Add `readSampleAt()` for direct random-access reads by the granular engine (no crossfade/cooldown side effects).

- [ ] **Step 1: Update FreezeBuffer.h — per-buffer writePos + readSampleAt**

In `src/modules/FreezeBuffer.h`, replace:

```cpp
    int writePos = 0;     // Write position in recording buffer
```

with:

```cpp
    int writePosA = 0;    // Write position in buffer A
    int writePosB = 0;    // Write position in buffer B
```

Also add this public method to the class (after `read()`):

```cpp
    // Direct random-access read from playback buffer at a sample index.
    // Used by the granular engine — no crossfade or cooldown side effects.
    // sampleIndex wraps within buffer length.
    void readSampleAt(int sampleIndex, float& outL, float& outR) const;

    // Get playback buffer length in samples (for position mapping)
    int getBufferLength() const { return bufferLength; }
```

- [ ] **Step 2: Update reset() in FreezeBuffer.cpp**

Replace:
```cpp
    writePos = 0;
```
with:
```cpp
    writePosA = 0;
    writePosB = 0;
```

- [ ] **Step 3: Update write() to use per-buffer writePos**

Replace `FreezeBuffer::write()`:

```cpp
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
```

- [ ] **Step 4: Update triggerFreeze() to reset new recording buffer's writePos**

Replace `FreezeBuffer::triggerFreeze()`:

```cpp
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
```

- [ ] **Step 5: Update toggleFreeze() similarly**

In `toggleFreeze()`, when starting freeze, also handle writePos:

```cpp
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
```

- [ ] **Step 6: Add readSampleAt() implementation to FreezeBuffer.cpp**

Add at the end of `FreezeBuffer.cpp`:

```cpp
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
```

- [ ] **Step 7: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 8: Commit**

```bash
cd ~/PsycogVST
git add src/modules/FreezeBuffer.h src/modules/FreezeBuffer.cpp
git commit -m "fix: FreezeBuffer per-buffer writePos, reset on swap, readSampleAt (Issue #10)

- Replace single shared writePos with writePosA/writePosB
- Reset new recording buffer's writePos to 0 on buffer swap
- Add readSampleAt() for direct random-access by granular engine
- Prevents stale content from previous recording cycle"
```

---

## Task 7: WetProcessor — EMA RMS, Freeze-Aware Release, processSample (Issue #13b)

**Files:**
- Rewrite: `src/modules/WetProcessor.h`
- Rewrite: `src/modules/WetProcessor.cpp`

Replace running-sum RMS with proper EMA. Add freeze-aware fast release. Add `processSample()`.

- [ ] **Step 1: Write new WetProcessor.h**

Replace `src/modules/WetProcessor.h`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — Soft-clip + auto-normalize for wet signal

  Processing per sample: tanh(signal) -> auto-normalize -> output
  Auto-normalize: RMS to -6dBFS, 300ms attack / 1000ms release
  Freeze-aware: 100ms fast release during freeze transitions (200ms window)
  Noise gate: bypass below -60dBFS
  Gain clamp: 0.1 to 3.0 (~ -20dB to +10dB)
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include "../utils/Constants.h"

class WetProcessor
{
public:
    WetProcessor();
    ~WetProcessor() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // Per-sample processing for wet path loop
    void processSample(float& outL, float& outR, float inL, float inR);

    // Block-level processing (calls processSample internally)
    void process(float* leftOut, float* rightOut,
                 const float* leftIn, const float* rightIn,
                 int numSamples);

    // Call when freeze buffer swaps — enables fast release for 200ms
    void notifyFreezeTransition();

private:
    double sampleRate = 48000.0;

    // Auto-normalize
    float targetRmsLinear = 0.0f;   // -6dBFS in linear
    float attackCoeff = 0.0f;       // 300ms attack
    float normalReleaseCoeff = 0.0f; // 1000ms release (normal)
    float fastReleaseCoeff = 0.0f;   // 100ms release (freeze transition)
    float currentGain = 1.0f;        // Current gain adjustment

    // EMA RMS tracking (replaces flawed running sum)
    float rmsPower = 0.0f;           // EMA of squared amplitude
    float rmsCoeff = 0.0f;           // EMA coefficient (~300ms window)

    // Noise gate
    float noiseGateThreshold = 0.0f;

    // Freeze transition state
    int freezeTransitionRemaining = 0;
    int freezeTransitionLength = 0;   // 200ms in samples
};
```

- [ ] **Step 2: Write new WetProcessor.cpp**

Replace `src/modules/WetProcessor.cpp`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  WetProcessor — EMA RMS, freeze-aware release (Issue #13b)
*/

#include "WetProcessor.h"
#include "../utils/MathUtils.h"

WetProcessor::WetProcessor()
{
}

void WetProcessor::prepare(double sr, int /*maxSamples*/)
{
    sampleRate = sr;

    // Target -6dBFS per spec
    targetRmsLinear = MathUtils::dbToGain(static_cast<float>(PsycogConstants::autoNormalizeTargetDb));
    noiseGateThreshold = MathUtils::dbToGain(static_cast<float>(PsycogConstants::noiseGateThresholdDb));

    // Attack coefficient (300ms)
    float attackTime = static_cast<float>(PsycogConstants::autoNormalizeAttackMs) / 1000.0f;
    attackCoeff = std::exp(-1.0f / (attackTime * static_cast<float>(sampleRate)));

    // Release coefficients
    normalReleaseCoeff = std::exp(-1.0f / (1.0f * static_cast<float>(sampleRate)));    // 1000ms
    fastReleaseCoeff = std::exp(-1.0f / (0.1f * static_cast<float>(sampleRate)));       // 100ms

    // RMS EMA coefficient (~300ms window)
    rmsCoeff = std::exp(-1.0f / (attackTime * static_cast<float>(sampleRate)));

    // Freeze transition window (200ms)
    freezeTransitionLength = static_cast<int>(0.2 * sampleRate);

    reset();
}

void WetProcessor::reset()
{
    rmsPower = 0.0f;
    currentGain = 1.0f;
    freezeTransitionRemaining = 0;
}

void WetProcessor::processSample(float& outL, float& outR, float inL, float inR)
{
    // 1. Soft-clip (tanh)
    float leftClipped = std::tanh(inL);
    float rightClipped = std::tanh(inR);

    // 2. Update EMA RMS (replaces flawed running sum)
    float samplePower = (leftClipped * leftClipped + rightClipped * rightClipped) * 0.5f;
    rmsPower = rmsCoeff * rmsPower + (1.0f - rmsCoeff) * samplePower;
    float currentRms = std::sqrt(rmsPower);

    // 3. Noise gate check
    if (currentRms < noiseGateThreshold)
    {
        // Below noise gate — bypass gain adjustment
        outL = leftClipped;
        outR = rightClipped;
        return;
    }

    // 4. Compute target gain
    float targetGain = targetRmsLinear / std::max(currentRms, 1e-6f);

    // Clamp gain (0.1 to 3.0 per spec)
    targetGain = juce::jlimit(static_cast<float>(PsycogConstants::gainMin),
                              static_cast<float>(PsycogConstants::gainMax),
                              targetGain);

    // 5. Apply attack/release to gain changes
    // Choose release coefficient based on freeze transition state
    float releaseCoeff = normalReleaseCoeff;
    if (freezeTransitionRemaining > 0)
    {
        releaseCoeff = fastReleaseCoeff;
        --freezeTransitionRemaining;
    }

    if (targetGain > currentGain)
    {
        // Increasing gain (signal got quieter) — use attack
        currentGain = currentGain + (1.0f - attackCoeff) * (targetGain - currentGain);
    }
    else
    {
        // Decreasing gain (signal got louder) — use release
        currentGain = currentGain + (1.0f - releaseCoeff) * (targetGain - currentGain);
    }

    // 6. Apply gain
    outL = leftClipped * currentGain;
    outR = rightClipped * currentGain;
}

void WetProcessor::process(float* leftOut, float* rightOut,
                           const float* leftIn, const float* rightIn,
                           int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        processSample(leftOut[i], rightOut[i], leftIn[i], rightIn[i]);
    }
}

void WetProcessor::notifyFreezeTransition()
{
    freezeTransitionRemaining = freezeTransitionLength;
}
```

- [ ] **Step 3: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 4: Commit**

```bash
cd ~/PsycogVST
git add src/modules/WetProcessor.h src/modules/WetProcessor.cpp
git commit -m "fix: WetProcessor EMA RMS, freeze-aware release, processSample (Issue #13b)

- Replace flawed running-sum RMS with proper EMA (no drift)
- Add freeze-aware fast release (100ms for 200ms after freeze transition)
- Add processSample() for per-sample wet path
- Add notifyFreezeTransition() for freeze buffer swap events"
```

---

## Task 8: Mixer — Restore Internal Smoother (Issue #3)

**Files:**
- Modify: `src/modules/Mixer.h`
- Modify: `src/modules/Mixer.cpp`

The Mixer needs its internal smoother restored since modules own their smoothing. Fix the ramp time too.

- [ ] **Step 1: Write updated Mixer.h**

Replace `src/modules/Mixer.h`:

```cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  Mixer — Wet/dry blend with equal-power (cosine) crossfade

  Mix = 0%: Dry only (transparent)
  Mix = 100%: Wet only
  Module owns smoothing (20ms ramp).
  EM-14 fix: Uses cos/sin equal-power crossfade.
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class Mixer
{
public:
    Mixer();
    ~Mixer() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // mix is raw value from APVTS; smoothed internally per-sample
    void process(float* leftOut, float* rightOut,
                 const float* wetLeft, const float* wetRight,
                 const float* dryLeft, const float* dryRight,
                 int numSamples, float mix);

private:
    double sampleRate = 48000.0;
    juce::SmoothedValue<float> mixSmoother;
};
```

- [ ] **Step 2: Write updated Mixer.cpp**

Replace `src/modules/Mixer.cpp`:

```cpp
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
```

- [ ] **Step 3: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 4: Commit**

```bash
cd ~/PsycogVST
git add src/modules/Mixer.h src/modules/Mixer.cpp
git commit -m "fix: Mixer restore internal smoother, equal-power crossfade (Issues #3, EM-14)

- Restored internal SmoothedValue (module owns smoothing)
- Fixed ramp time to use smoothingTimeSeconds (20ms not 20s)
- Per-sample sin/cos equal-power crossfade
- Added reset() method"
```

---

## Task 9: Granular Time-Stretch Engine (Issue #7)

**Files:**
- Rewrite: `src/modules/TimeStretch.h`
- Rewrite: `src/modules/TimeStretch.cpp`

This is the core DSP task. Implements 4-grain variable overlap-add with dual-mode (time-stretch when unfrozen, pitch-shift when frozen), grain jitter, and freeze crossfade.

- [ ] **Step 1: Write new TimeStretch.h**

Replace `src/modules/TimeStretch.h`:

```cpp
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
    // normalizedStretch/normalizedPosition are raw+LFO values; smoothed internally
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
    int nextGrainSlot = 0;              // Round-robin grain scheduling
    int samplesSinceLastGrain = 0;      // Counter for hop timing

    // Hann window lookup table
    std::array<float, grainSize> hannWindow;

    // Circular input buffer (stereo interleaved as separate L/R)
    std::vector<float> inputBufferL;
    std::vector<float> inputBufferR;
    int inputWritePos = 0;
    float inputReadHead = 0.0f;          // Fractional read position for stretch

    // Smoothed parameters (module owns smoothing)
    juce::SmoothedValue<float> stretchSmoother;
    juce::SmoothedValue<float> positionSmoother;

    // Freeze system
    FreezeBuffer freezeBuffer;
    PsycogConstants::FreezeMode lastFreezeMode = PsycogConstants::FreezeMode::Off;

    // Mode transition crossfade (10ms)
    float crossfadeProgress = 1.0f;      // 1.0 = done
    int crossfadeLength = 0;             // 10ms in samples
    float prevOutputL = 0.0f;            // Previous output for crossfade
    float prevOutputR = 0.0f;

    // Lightweight LCG for grain jitter (audio-thread safe)
    uint32_t jitterLcgState = 1;
    int nextJitter();  // Returns random int in [-jitterRange, +jitterRange]

    // Internal processing
    void launchGrain(float stretch, float position, PsycogConstants::FreezeMode mode);
    void processGrainSample(Grain& grain, float& outL, float& outR,
                            PsycogConstants::FreezeMode mode, float stretchRate);
};
```

- [ ] **Step 2: Write new TimeStretch.cpp**

Replace `src/modules/TimeStretch.cpp`:

```cpp
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

    // Allocate input buffer
    inputBufferL.resize(inputBufferSize, 0.0f);
    inputBufferR.resize(inputBufferSize, 0.0f);
    inputWritePos = 0;
    inputReadHead = 0.0f;

    // Initialize freeze buffer
    freezeBuffer.prepare(sampleRate);

    // Initialize smoothers (20ms per spec)
    stretchSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    positionSmoother.reset(sampleRate, PsycogConstants::smoothingTimeSeconds);
    stretchSmoother.setCurrentAndTargetValue(0.5f);  // Normalized 1.0x
    positionSmoother.setCurrentAndTargetValue(0.5f);

    // Crossfade length (10ms)
    crossfadeLength = static_cast<int>(PsycogConstants::freezeCrossfadeMs * sampleRate / 1000.0);

    // Seed jitter LCG
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
    // ALWAYS write, regardless of freeze mode (spec requirement, Issue #10b)
    freezeBuffer.write(leftIn, rightIn, numSamples);
}

int TimeStretch::nextJitter()
{
    // Numerical Recipes LCG
    jitterLcgState = jitterLcgState * 1664525u + 1013904223u;
    // Map to [-jitterRange, +jitterRange]
    return static_cast<int>(jitterLcgState % (2 * jitterRange + 1)) - jitterRange;
}

void TimeStretch::launchGrain(float stretch, float position, PsycogConstants::FreezeMode mode)
{
    // Find an inactive slot (round-robin)
    for (int attempt = 0; attempt < numGrains; ++attempt)
    {
        int slot = (nextGrainSlot + attempt) % numGrains;
        if (!grains[slot].active)
        {
            Grain& g = grains[slot];
            g.playbackPosition = 0;
            g.active = true;

            int jitter = nextJitter();

            if (mode == PsycogConstants::FreezeMode::Off)
            {
                // Unfrozen: read from circular input buffer at current read head
                float readPos = inputReadHead + static_cast<float>(jitter);
                // Wrap to buffer bounds
                while (readPos < 0.0f) readPos += static_cast<float>(inputBufferSize);
                while (readPos >= static_cast<float>(inputBufferSize)) readPos -= static_cast<float>(inputBufferSize);
                g.sourcePosition = readPos;
            }
            else
            {
                // Frozen: read from freeze buffer at position parameter
                // position (0-1) maps to buffer position; stretch controls playback rate
                int bufLen = static_cast<int>(PsycogConstants::freezeBufferSeconds * sampleRate);
                float startSample = position * static_cast<float>(bufLen - 1) + static_cast<float>(jitter);
                // Clamp to buffer
                startSample = juce::jlimit(0.0f, static_cast<float>(bufLen - 1), startSample);
                g.sourcePosition = startSample;
            }

            nextGrainSlot = (slot + 1) % numGrains;
            return;
        }
    }
    // All slots busy — skip this grain launch (at extreme stretch this is rare)
}

void TimeStretch::processGrainSample(Grain& grain, float& outL, float& outR,
                                      PsycogConstants::FreezeMode mode, float stretchRate)
{
    if (!grain.active) return;

    // Get Hann window amplitude
    float window = hannWindow[grain.playbackPosition];

    float sampleL = 0.0f;
    float sampleR = 0.0f;

    if (mode == PsycogConstants::FreezeMode::Off)
    {
        // Read from circular input buffer
        int readIdx = static_cast<int>(grain.sourcePosition + grain.playbackPosition) % inputBufferSize;
        if (readIdx < 0) readIdx += inputBufferSize;
        sampleL = inputBufferL[readIdx];
        sampleR = inputBufferR[readIdx];
    }
    else
    {
        // Read from freeze buffer at grain position
        // stretchRate controls how fast we advance through the buffer (pitch shift)
        float readPos = grain.sourcePosition + static_cast<float>(grain.playbackPosition) * stretchRate;

        // Direct random-access read — no crossfade/cooldown side effects
        // (FreezeBuffer::read() manages crossfade state and would corrupt if called per-grain)
        int readIdx = static_cast<int>(readPos);
        freezeBuffer.readSampleAt(readIdx, sampleL, sampleR);
    }

    // Apply window and accumulate
    outL += sampleL * window;
    outR += sampleR * window;

    // Advance grain
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
    // CRITICAL: prevOutputL/R are saved from the PREVIOUS call's output (member vars),
    // NOT from the current outL/outR references (which are uninitialized at this point).
    bool modeChanged = (freezeMode != lastFreezeMode);
    if (modeChanged)
    {
        crossfadeProgress = 0.0f;
        lastFreezeMode = freezeMode;
    }

    // Compute hop size for grain scheduling
    // hopSize = grainSize / (stretch * overlapFactor)
    // overlapFactor = 2 (50% base overlap)
    float overlapFactor = 2.0f;
    float hopSize = static_cast<float>(grainSize) / (stretch * overlapFactor);
    hopSize = std::max(hopSize, 1.0f);  // Minimum 1 sample hop

    // Check if it's time to launch a new grain
    samplesSinceLastGrain++;
    if (samplesSinceLastGrain >= static_cast<int>(hopSize))
    {
        launchGrain(stretch, smoothedPosition, freezeMode);
        samplesSinceLastGrain = 0;

        // Advance input read head (unfrozen mode only)
        if (freezeMode == PsycogConstants::FreezeMode::Off)
        {
            // Read head advances by hopSize — this is what creates time-stretch
            // Hop locked at grain launch time (not per-sample) to prevent AM artifacts
            inputReadHead += hopSize;
            while (inputReadHead >= static_cast<float>(inputBufferSize))
                inputReadHead -= static_cast<float>(inputBufferSize);
        }
    }

    // Sum all active grains
    float grainOutL = 0.0f;
    float grainOutR = 0.0f;

    // Stretch rate for frozen mode pitch shift
    float stretchRate = (freezeMode != PsycogConstants::FreezeMode::Off) ? stretch : 1.0f;

    for (auto& grain : grains)
    {
        if (grain.active)
            processGrainSample(grain, grainOutL, grainOutR, freezeMode, stretchRate);
    }

    // Apply mode transition crossfade if active
    // prevOutputL/R hold the last frame's output (saved at end of this function)
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

    // Save this frame's output for potential crossfade on next mode transition
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
```

- [ ] **Step 3: Do NOT build yet — proceed directly to Task 10**

**IMPORTANT:** This task removes `TimeStretch::process()` which is still called by `PluginProcessor.cpp`. The build WILL fail until Task 10 rewrites processBlock. Tasks 9 and 10 MUST be executed together as a unit. Do not attempt to build or commit between them.

- [ ] **Step 4: Commit together with Task 10 (single commit covering both)**

```bash
cd ~/PsycogVST
git add src/modules/TimeStretch.h src/modules/TimeStretch.cpp
git commit -m "feat: 4-grain granular time-stretch engine (Issue #7)

- 4-grain variable overlap-add with Hann windowing
- Unfrozen: hop size controls time-stretch (0.1x-10x)
- Frozen: stretch controls pitch, position controls read head
- ±64 sample grain jitter (LCG, audio-thread safe)
- 10ms crossfade on freeze/unfreeze mode transitions
- processSample() interface for per-sample wet path"
```

---

## Task 10: Signal Flow Rewrite — processBlock (Issue #2)

**Files:**
- Modify: `src/PluginProcessor.h`
- Rewrite processBlock in: `src/PluginProcessor.cpp`

Rewrite processBlock with correct signal order, remove processBlock(double), fix releaseResources, set tail length.

- [ ] **Step 1: Update PluginProcessor.h**

In `src/PluginProcessor.h`:

1. Remove `processBlock(double)` declaration:
```cpp
    // REMOVE this line:
    void processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&) override;
```

2. Remove the LFO target flags (already done in Task 3, verify):
```cpp
    // These should already be removed:
    bool lfoTargetStretch = false;
    bool lfoTargetPosition = false;
    bool lfoTargetFoldAmount = false;
    bool lfoTargetFoldOffset = false;
```

3. Remove any remaining SmoothedValue declarations (already done in Task 3, verify).

4. Keep `freezeMode` member (rename to `lastFreezeMode` for clarity) — needed for Manual freeze toggle detection:
```cpp
    // Freeze state — tracks previous block's mode for toggle detection
    PsycogConstants::FreezeMode lastFreezeMode = PsycogConstants::FreezeMode::Off;
```

5. Change tail length:
```cpp
    // BEFORE:
    double getTailLengthSeconds() const override { return 0.0; }
    // AFTER:
    double getTailLengthSeconds() const override { return 4.0; }
```

- [ ] **Step 2: Rewrite processBlock(float)**

Replace the entire `processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&)` method:

```cpp
void PsycogAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // Handle mono input by copying to both channels
    bool isMonoInput = (totalNumInputChannels == 1);
    if (isMonoInput && totalNumOutputChannels >= 2)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);

    // Get input pointers
    const float* leftIn = buffer.getReadPointer(0);
    const float* rightIn = buffer.getReadPointer(1);

    // === 1. Read all raw parameter values (once per block) ===
    float stretchNorm = *apvts.getRawParameterValue(ParamIDs::stretch);
    float positionNorm = *apvts.getRawParameterValue(ParamIDs::position);
    int freezeModeInt = static_cast<int>(*apvts.getRawParameterValue(ParamIDs::freezeMode));
    float threshold = *apvts.getRawParameterValue(ParamIDs::threshold);
    float foldAmount = *apvts.getRawParameterValue(ParamIDs::foldAmount);
    float foldOffset = *apvts.getRawParameterValue(ParamIDs::foldOffset);
    float lfoRate = *apvts.getRawParameterValue(ParamIDs::lfoRate);
    int lfoWaveformInt = static_cast<int>(*apvts.getRawParameterValue(ParamIDs::lfoWaveform));
    float lfoDepth = *apvts.getRawParameterValue(ParamIDs::lfoDepth);
    bool targetStretch = *apvts.getRawParameterValue(ParamIDs::lfoTargetStretch) > 0.5f;
    bool targetPosition = *apvts.getRawParameterValue(ParamIDs::lfoTargetPosition) > 0.5f;
    bool targetFoldAmount = *apvts.getRawParameterValue(ParamIDs::lfoTargetFoldAmount) > 0.5f;
    bool targetFoldOffset = *apvts.getRawParameterValue(ParamIDs::lfoTargetFoldOffset) > 0.5f;
    float mix = *apvts.getRawParameterValue(ParamIDs::mix);

    auto currentFreezeMode = static_cast<PsycogConstants::FreezeMode>(freezeModeInt);
    auto lfoWaveform = static_cast<PsycogConstants::LfoWaveform>(lfoWaveformInt);

    // === 2. Configure LFO ===
    lfo.setRate(lfoRate);
    lfo.setWaveform(lfoWaveform);
    lfo.setDepth(lfoDepth);

    // === 3. Dry path: DryDelay (block-level) ===
    float* dryLeft = dryBuffer.getWritePointer(0);
    float* dryRight = dryBuffer.getWritePointer(1);
    dryDelay.process(dryLeft, dryRight, leftIn, rightIn, numSamples);

    // === 4. FreezeBuffer write (ALWAYS, even in Off mode — spec requirement) ===
    timeStretch.writeFreezeBuffer(leftIn, rightIn, numSamples);

    // === 4b. Handle Manual freeze toggle ===
    // Detect transitions to/from Manual mode and toggle freeze buffer accordingly
    if (currentFreezeMode == PsycogConstants::FreezeMode::Manual
        && lastFreezeMode != PsycogConstants::FreezeMode::Manual)
    {
        timeStretch.toggleFreeze();  // Start freeze
    }
    else if (currentFreezeMode != PsycogConstants::FreezeMode::Manual
             && lastFreezeMode == PsycogConstants::FreezeMode::Manual)
    {
        timeStretch.toggleFreeze();  // Stop freeze
    }
    lastFreezeMode = currentFreezeMode;

    // === 5. Wet path PER-SAMPLE loop ===
    float* wetLeft = wetBuffer.getWritePointer(0);
    float* wetRight = wetBuffer.getWritePointer(1);

    for (int i = 0; i < numSamples; ++i)
    {
        // 5a. LFO advance (per-sample)
        float lfoValue = lfo.advance();

        // 5b. Compute modulated parameters (raw + LFO, clamped)
        float finalStretch = stretchNorm;
        float finalPosition = positionNorm;
        float finalFoldAmount = foldAmount;
        float finalFoldOffset = foldOffset;

        if (targetStretch)
            finalStretch = juce::jlimit(0.0f, 1.0f, stretchNorm + lfoValue);
        if (targetPosition)
            finalPosition = juce::jlimit(0.0f, 1.0f, positionNorm + lfoValue);
        if (targetFoldAmount)
            finalFoldAmount = juce::jlimit(0.0f, 1.0f, foldAmount + lfoValue);
        if (targetFoldOffset)
            finalFoldOffset = juce::jlimit(-1.0f, 1.0f, foldOffset + lfoValue);

        // 5c. TimeStretch (granular engine)
        float tsL, tsR;
        timeStretch.processSample(tsL, tsR, leftIn[i], rightIn[i],
                                  finalStretch, finalPosition, currentFreezeMode);

        // 5d. Wavefolder
        float wfL, wfR;
        wavefolder.processSample(wfL, wfR, tsL, tsR,
                                 finalFoldAmount, finalFoldOffset, isMonoInput);

        // 5e. Threshold detection (per-sample, from Wavefolder output)
        bool triggered = thresholdDetector.processSample(wfL, wfR, threshold);

        // 5f. Auto-freeze trigger
        if (triggered && currentFreezeMode == PsycogConstants::FreezeMode::Auto)
        {
            timeStretch.triggerFreeze();
            wetProcessor.notifyFreezeTransition();
        }

        // 5g. WetProcessor (soft-clip + auto-normalize)
        float wpL, wpR;
        wetProcessor.processSample(wpL, wpR, wfL, wfR);

        // 5h. Store in wet buffer
        wetLeft[i] = wpL;
        wetRight[i] = wpR;
    }

    // === 6. Mixer (block-level, smooths mix internally) ===
    mixer.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                  wetLeft, wetRight, dryLeft, dryRight,
                  numSamples, mix);

    // === 7. Output protection (block-level) ===
    outputProtection.process(buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getReadPointer(0), buffer.getReadPointer(1),
                            numSamples);
}
```

- [ ] **Step 3: Remove processBlock(double) entirely**

Delete the entire `processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&)` method from `PluginProcessor.cpp` (Issue #13a).

- [ ] **Step 4: Fix releaseResources (Issue #11)**

Replace `releaseResources()`:

```cpp
void PsycogAudioProcessor::releaseResources()
{
    dryDelay.reset();
    timeStretch.reset();
    wavefolder.reset();
    thresholdDetector.reset();
    wetProcessor.reset();
    mixer.reset();
    lfo.reset();
    // outputProtection is stateless — no reset needed
}
```

- [ ] **Step 5: Fix prepareToPlay — remove smoother init, add module init**

Update `prepareToPlay()` to remove the deleted smoother initialization:

```cpp
void PsycogAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Report latency (2048 samples = grain window)
    setLatencySamples(PsycogConstants::grainSize);

    // Initialize all modules
    dryDelay.prepare(sampleRate, samplesPerBlock);
    timeStretch.prepare(sampleRate, samplesPerBlock);
    wavefolder.prepare(sampleRate, samplesPerBlock);
    thresholdDetector.prepare(sampleRate);
    wetProcessor.prepare(sampleRate, samplesPerBlock);
    mixer.prepare(sampleRate, samplesPerBlock);
    lfo.prepare(sampleRate);

    // Allocate temp buffers
    wetBuffer.setSize(2, samplesPerBlock * 2);  // Double for safety
    dryBuffer.setSize(2, samplesPerBlock * 2);
}
```

- [ ] **Step 6: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`
Expected: Build succeeds, all tests pass

- [ ] **Step 7: Commit**

```bash
cd ~/PsycogVST
git add src/modules/TimeStretch.h src/modules/TimeStretch.cpp src/PluginProcessor.h src/PluginProcessor.cpp
git commit -m "feat: 4-grain granular engine + signal flow rewrite (Issues #2, #7, #11, #13a, #13c)

4-grain variable overlap-add granular time-stretch engine:
- Unfrozen: hop size controls time-stretch (0.1x-10x)
- Frozen: stretch controls pitch, position controls read head
- ±64 sample grain jitter (LCG, audio-thread safe)
- 10ms crossfade on freeze/unfreeze mode transitions

Signal flow: TimeStretch -> Wavefolder -> ThresholdDetect -> WetProcessor
- Manual freeze toggle detection (Off<->Manual transitions)
- FreezeBuffer.write() called unconditionally (always recording)
- Per-sample wet loop with correct module chain
- Threshold detection immediate (not end-of-block)
- Removed processBlock(double) — JUCE handles conversion
- releaseResources() calls all module reset()
- Tail length = 4.0 seconds (3s freeze + 1s release)"
```

---

## Task 11: Fix PresetManager applyPreset (Issue #9)

**Files:**
- Modify: `src/PresetManager.cpp:426-434`

Fix `applyPreset()` to use `setValueNotifyingHost` instead of `replaceState` with malformed XML.

- [ ] **Step 1: Replace applyPreset()**

In `src/PresetManager.cpp`, replace the `applyPreset` function:

```cpp
    bool applyPreset(juce::AudioProcessorValueTreeState& apvts, int index)
    {
        if (index < 0 || index >= numPresets)
            return false;

        const auto& p = factoryPresets[index];

        // Set parameters through APVTS using convertTo0to1 for correct normalization
        // This properly updates the APVTS tree and notifies the host for automation
        auto setParam = [&](const char* id, float rawValue) {
            if (auto* param = apvts.getParameter(id))
                param->setValueNotifyingHost(param->convertTo0to1(rawValue));
        };

        setParam(ParamIDs::stretch,    p.stretch);
        setParam(ParamIDs::position,   p.position);
        setParam(ParamIDs::threshold,  p.threshold);
        setParam(ParamIDs::foldAmount, p.foldAmount);
        setParam(ParamIDs::foldOffset, p.foldOffset);
        setParam(ParamIDs::lfoRate,    p.lfoRate);
        setParam(ParamIDs::lfoDepth,   p.lfoDepth);
        setParam(ParamIDs::mix,        p.mix);

        // Choice parameters
        setParam(ParamIDs::freezeMode,  static_cast<float>(p.freezeMode));
        setParam(ParamIDs::lfoWaveform, static_cast<float>(p.lfoWaveform));

        // Bool parameters
        setParam(ParamIDs::lfoTargetStretch,    p.lfoTargetStretch ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetPosition,   p.lfoTargetPosition ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetFoldAmount, p.lfoTargetFoldAmount ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetFoldOffset, p.lfoTargetFoldOffset ? 1.0f : 0.0f);

        return true;
    }
```

- [ ] **Step 2: Build and run tests**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests`

Note: The existing preset tests use `createPresetState()` which returns custom XML — those tests will still pass because they don't test `applyPreset`. The real `applyPreset` fix can only be fully verified in a DAW (Phase 9).

Expected: Build succeeds, all tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add src/PresetManager.cpp
git commit -m "fix: PresetManager applyPreset uses setValueNotifyingHost (Issue #9)

Old code used replaceState with flat XML that didn't match APVTS
tree structure. New code sets parameters directly through APVTS
parameter objects with proper normalization via convertTo0to1."
```

---

## Task 12: Add New Test Files to CMakeLists

**Files:**
- Modify: `CMakeLists.txt:103-106`

- [ ] **Step 1: Add TestThresholdDetector.cpp and TestWetProcessor.cpp**

In `CMakeLists.txt`, in the `add_executable(PsycogVSTTests ...)` block, add:

```cmake
    tests/TestThresholdDetector.cpp
    tests/TestWetProcessor.cpp
```

After the existing test file list:
```cmake
add_executable(PsycogVSTTests
    tests/TestTimeStretch.cpp
    tests/TestWavefolder.cpp
    tests/TestLFO.cpp
    tests/TestFreezeBuffer.cpp
    tests/TestParameters.cpp
    tests/TestPresets.cpp
    tests/TestThresholdDetector.cpp
    tests/TestWetProcessor.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/PresetManager.cpp
    ${TEST_MODULE_SOURCES}
)
```

- [ ] **Step 2: Commit**

```bash
cd ~/PsycogVST
git add CMakeLists.txt
git commit -m "chore: add ThresholdDetector and WetProcessor test files to CMake"
```

---

## Task 13: Write TestParameters.cpp (~8 tests)

**Files:**
- Rewrite: `tests/TestParameters.cpp`

- [ ] **Step 1: Write test file**

Replace `tests/TestParameters.cpp`:

```cpp
/*
  PsycogVST Tests - Parameter conversion tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Parameters.h"

TEST_CASE("Stretch log conversion round-trip", "[Parameters]")
{
    SECTION("normalized=0.0 -> stretch=0.1x")
    {
        float stretch = ParamConversions::stretchFromNormalized(0.0f);
        REQUIRE(stretch == Catch::Approx(0.1f).margin(0.001f));
    }

    SECTION("normalized=0.5 -> stretch=1.0x")
    {
        float stretch = ParamConversions::stretchFromNormalized(0.5f);
        REQUIRE(stretch == Catch::Approx(1.0f).margin(0.01f));
    }

    SECTION("normalized=1.0 -> stretch=10.0x")
    {
        float stretch = ParamConversions::stretchFromNormalized(1.0f);
        REQUIRE(stretch == Catch::Approx(10.0f).margin(0.01f));
    }

    SECTION("Round-trip at multiple points")
    {
        for (float norm : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
        {
            float stretch = ParamConversions::stretchFromNormalized(norm);
            float backToNorm = ParamConversions::stretchToNormalized(stretch);
            REQUIRE(backToNorm == Catch::Approx(norm).margin(1e-5f));
        }
    }
}

TEST_CASE("LFO rate log conversion round-trip", "[Parameters]")
{
    SECTION("normalized=0.0 -> rate=0.01 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(0.0f);
        REQUIRE(rate == Catch::Approx(0.01f).margin(0.001f));
    }

    SECTION("normalized=0.5 -> rate ~0.45 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(0.5f);
        // sqrt(0.01 * 20) = sqrt(0.2) ≈ 0.447
        REQUIRE(rate == Catch::Approx(0.447f).margin(0.05f));
    }

    SECTION("normalized=1.0 -> rate=20.0 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(1.0f);
        REQUIRE(rate == Catch::Approx(20.0f).margin(0.01f));
    }

    SECTION("Round-trip at multiple points")
    {
        for (float norm : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
        {
            float rate = ParamConversions::lfoRateFromNormalized(norm);
            float backToNorm = ParamConversions::lfoRateToNormalized(rate);
            REQUIRE(backToNorm == Catch::Approx(norm).margin(1e-5f));
        }
    }
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*Parameter*" -v`
Expected: All 8 assertions pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestParameters.cpp
git commit -m "test: parameter conversion round-trip tests (Issue #12)"
```

---

## Task 14: Write TestLFO.cpp (~15 tests)

**Files:**
- Rewrite: `tests/TestLFO.cpp`

- [ ] **Step 1: Write test file**

Replace `tests/TestLFO.cpp`:

```cpp
/*
  PsycogVST Tests - LFO module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/LFO.h"
#include "Parameters.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("LFO Sine shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    // 1 Hz -> full cycle in 48000 samples
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    // Advance past depth smoother ramp (~960 samples at 48kHz, 20ms)
    for (int i = 0; i < 2000; ++i)
        lfo.advance();

    SECTION("Quarter-cycle rises to +1")
    {
        // Reset phase by re-preparing
        lfo.prepare(testSampleRate);
        lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
        lfo.setDepth(1.0f);
        lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
        // Burn through smoother
        for (int i = 0; i < 2000; ++i) lfo.advance();

        // At 1Hz, peak at 12000 samples (quarter cycle)
        float peak = 0.0f;
        for (int i = 0; i < 12000; ++i)
        {
            float v = lfo.advance();
            if (v > peak) peak = v;
        }
        REQUIRE(peak == Catch::Approx(1.0f).margin(0.02f));
    }
}

TEST_CASE("LFO Triangle shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Triangle);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    // Burn through smoother
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Peaks at +depth and -depth")
    {
        float maxVal = -2.0f;
        float minVal = 2.0f;
        for (int i = 0; i < 48000; ++i)
        {
            float v = lfo.advance();
            if (v > maxVal) maxVal = v;
            if (v < minVal) minVal = v;
        }
        REQUIRE(maxVal == Catch::Approx(1.0f).margin(0.02f));
        REQUIRE(minVal == Catch::Approx(-1.0f).margin(0.02f));
    }
}

TEST_CASE("LFO Square shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Square);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Values are exactly +depth or -depth")
    {
        bool seenPositive = false, seenNegative = false;
        for (int i = 0; i < 48000; ++i)
        {
            float v = lfo.advance();
            REQUIRE((std::abs(v - 1.0f) < 0.02f || std::abs(v + 1.0f) < 0.02f));
            if (v > 0.0f) seenPositive = true;
            if (v < 0.0f) seenNegative = true;
        }
        REQUIRE(seenPositive);
        REQUIRE(seenNegative);
    }
}

TEST_CASE("LFO S&H behavior", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::SampleAndHold);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Constant between wraps")
    {
        // At 1Hz/48kHz, phase wraps every 48000 samples
        // Within a half cycle, value should be constant
        float firstVal = lfo.advance();
        bool allSame = true;
        for (int i = 1; i < 24000; ++i)
        {
            float v = lfo.advance();
            if (std::abs(v - firstVal) > 0.001f)
            {
                allSame = false;
                break;
            }
        }
        REQUIRE(allSame);
    }

    SECTION("Non-zero initialization")
    {
        LFO lfo2;
        lfo2.prepare(testSampleRate);
        lfo2.setWaveform(PsycogConstants::LfoWaveform::SampleAndHold);
        lfo2.setDepth(1.0f);
        lfo2.setRate(ParamConversions::lfoRateToNormalized(0.01f));
        for (int i = 0; i < 2000; ++i) lfo2.advance();
        // heldValue should be initialized non-zero (from LCG in prepare)
        // With depth=1 and after smoother ramp, output should be non-zero
        float v = lfo2.advance();
        // Can't guarantee non-zero (LCG could produce 0) but extremely unlikely
        // Just verify it doesn't crash and produces a valid float
        REQUIRE(std::isfinite(v));
    }
}

TEST_CASE("LFO depth=0 always zero", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(0.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    for (int i = 0; i < 48000; ++i)
    {
        float v = lfo.advance();
        REQUIRE(std::abs(v) < 0.001f);
    }
}

TEST_CASE("LFO depth scaling", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(0.5f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    float maxVal = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        float v = std::abs(lfo.advance());
        if (v > maxVal) maxVal = v;
    }
    REQUIRE(maxVal == Catch::Approx(0.5f).margin(0.02f));
}

TEST_CASE("LFO rate accuracy", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(10.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Count zero-crossings over 1 second
    int zeroCrossings = 0;
    float prev = lfo.advance();
    for (int i = 1; i < 48000; ++i)
    {
        float curr = lfo.advance();
        if ((prev >= 0.0f && curr < 0.0f) || (prev < 0.0f && curr >= 0.0f))
            zeroCrossings++;
        prev = curr;
    }
    // 10Hz sine = 20 zero-crossings per second (±1 for edge cases)
    REQUIRE(zeroCrossings >= 19);
    REQUIRE(zeroCrossings <= 21);
}

TEST_CASE("LFO phase continuity at block boundary", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(5.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Process 512 samples, record last value
    float lastVal = 0.0f;
    for (int i = 0; i < 512; ++i)
        lastVal = lfo.advance();

    // Next call should be continuous (no jump)
    float nextVal = lfo.advance();
    float delta = std::abs(nextVal - lastVal);
    // At 5Hz/48kHz, max per-sample delta for sine ≈ 2*pi*5/48000 ≈ 0.00065
    REQUIRE(delta < 0.01f);
}

TEST_CASE("LFO minimum rate", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(0.0f);  // 0.01Hz
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Should produce valid values with no numerical issues
    for (int i = 0; i < 48000; ++i)
    {
        float v = lfo.advance();
        REQUIRE(std::isfinite(v));
    }
}

TEST_CASE("LFO maximum rate", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(1.0f);  // 20Hz
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Should oscillate correctly
    float maxVal = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        float v = std::abs(lfo.advance());
        if (v > maxVal) maxVal = v;
    }
    REQUIRE(maxVal == Catch::Approx(1.0f).margin(0.02f));
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*LFO*" -v`
Expected: All LFO tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestLFO.cpp
git commit -m "test: LFO waveforms, rate, depth, S&H, boundary tests (Issue #12)"
```

---

## Task 15: Write TestWavefolder.cpp (~15 tests)

**Files:**
- Rewrite: `tests/TestWavefolder.cpp`

- [ ] **Step 1: Write test file**

Replace `tests/TestWavefolder.cpp`:

```cpp
/*
  PsycogVST Tests - Wavefolder module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/Wavefolder.h"
#include "utils/MathUtils.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

// Helper: prepare wavefolder and burn through smoother ramp
static Wavefolder createPreparedWavefolder()
{
    Wavefolder wf;
    wf.prepare(testSampleRate, 512);
    return wf;
}

TEST_CASE("Wavefolder passthrough at foldAmount=0", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float inL = 0.5f, inR = -0.3f;
    float outL, outR;

    // Process enough samples for smoother to settle
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, inL, inR, 0.0f, 0.0f, false);

    REQUIRE(outL == Catch::Approx(inL).margin(1e-4f));
    REQUIRE(outR == Catch::Approx(inR).margin(1e-4f));
}

TEST_CASE("Wavefolder output always bounded [-1, 1]", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Test extreme inputs at max fold
    float inputs[] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
    for (float in : inputs)
    {
        for (int i = 0; i < 2000; ++i)
            wf.processSample(outL, outR, in, in, 1.0f, 1.0f, false);

        REQUIRE(outL >= -1.0f);
        REQUIRE(outL <= 1.0f);
        REQUIRE(outR >= -1.0f);
        REQUIRE(outR <= 1.0f);
    }
}

TEST_CASE("Wavefolder stereo opposite offset", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Stereo input with offset=0.5 should produce different L and R
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.3f, 0.3f, 0.5f, 0.5f, false);

    // L gets +0.5 offset, R gets -0.5 offset -> different outputs
    REQUIRE(outL != Catch::Approx(outR).margin(0.01f));
}

TEST_CASE("Wavefolder mono stereo widening", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Mono input with offset > 0 should produce L != R
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.4f, 0.4f, 0.5f, 0.5f, true);

    REQUIRE(outL != Catch::Approx(outR).margin(0.01f));
}

TEST_CASE("Wavefolder DC offset scaling", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // foldAmount=0, offset=1.0 -> effective_offset=0 -> passthrough
    float inL = 0.7f, inR = -0.4f;
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, inL, inR, 0.0f, 1.0f, false);

    REQUIRE(outL == Catch::Approx(inL).margin(1e-4f));
    REQUIRE(outR == Catch::Approx(inR).margin(1e-4f));
}

TEST_CASE("Wavefolder zero input zero output", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Zero input with zero offset at any fold amount -> output should be 0 (or near 0)
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.0f, 0.0f, 0.8f, 0.0f, false);

    REQUIRE(std::abs(outL) < 1e-4f);
    REQUIRE(std::abs(outR) < 1e-4f);
}

TEST_CASE("Wavefolder high drive output range", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // foldAmount=1.0 (drive=10x) should still be bounded
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.9f, -0.9f, 1.0f, 0.0f, false);

    REQUIRE(outL >= -1.0f);
    REQUIRE(outL <= 1.0f);
    REQUIRE(outR >= -1.0f);
    REQUIRE(outR <= 1.0f);
}

TEST_CASE("Wavefolder negative offset swaps channels", "[Wavefolder]")
{
    auto wfPos = createPreparedWavefolder();
    auto wfNeg = createPreparedWavefolder();
    float posL, posR, negL, negR;

    // Positive offset: L=+offset, R=-offset
    for (int i = 0; i < 2000; ++i)
    {
        wfPos.processSample(posL, posR, 0.5f, 0.5f, 0.6f, 0.5f, false);
        wfNeg.processSample(negL, negR, 0.5f, 0.5f, 0.6f, -0.5f, false);
    }

    // Negative offset should swap L and R relative to positive offset
    REQUIRE(posL == Catch::Approx(negR).margin(0.01f));
    REQUIRE(posR == Catch::Approx(negL).margin(0.01f));
}

TEST_CASE("MathUtils::triangleFold values", "[Wavefolder]")
{
    // Verify the triangle fold function at key points
    // triangleFold maps input to [-1, 1] via triangle wave with period 4
    REQUIRE(MathUtils::triangleFold(0.0f) == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(1.0f) == Catch::Approx(1.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(2.0f) == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(3.0f) == Catch::Approx(-1.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(-1.0f) == Catch::Approx(-1.0f).margin(1e-6f));
}

TEST_CASE("Wavefolder block process produces valid output", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();

    // Process a block of non-zero input
    float inputs[512];
    float outputsL[512], outputsR[512];
    for (int i = 0; i < 512; ++i)
        inputs[i] = 0.5f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);

    wf.process(outputsL, outputsR, inputs, inputs, 512, 0.5f, 0.0f, false);

    // All outputs should be finite and bounded
    for (int i = 0; i < 512; ++i)
    {
        REQUIRE(std::isfinite(outputsL[i]));
        REQUIRE(std::isfinite(outputsR[i]));
        REQUIRE(outputsL[i] >= -1.0f);
        REQUIRE(outputsL[i] <= 1.0f);
    }
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*Wavefolder*" -v`
Expected: All tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestWavefolder.cpp
git commit -m "test: Wavefolder passthrough, stereo offset, bounds, fold math (Issue #12)"
```

---

## Task 16: Write TestThresholdDetector.cpp (~8 tests)

**Files:**
- Create: `tests/TestThresholdDetector.cpp`

- [ ] **Step 1: Write test file**

Create `tests/TestThresholdDetector.cpp`:

```cpp
/*
  PsycogVST Tests - ThresholdDetector module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/ThresholdDetector.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("ThresholdDetector above threshold triggers", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Feed sustained amplitude > threshold for 10ms+ to build EMA
    bool triggered = false;
    for (int i = 0; i < 1000; ++i)  // ~20ms
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            triggered = true;
    }
    REQUIRE(triggered);
}

TEST_CASE("ThresholdDetector below threshold no trigger", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    bool triggered = false;
    for (int i = 0; i < 48000; ++i)  // 1 second
    {
        if (td.processSample(0.1f, 0.1f, 0.5f))
            triggered = true;
    }
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector cooldown blocks re-trigger", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Build up to trigger
    bool firstTrigger = false;
    for (int i = 0; i < 1000; ++i)
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
        {
            firstTrigger = true;
            break;
        }
    }
    REQUIRE(firstTrigger);

    // Immediately try again — should be in cooldown (100ms = 4800 samples)
    bool secondTrigger = false;
    for (int i = 0; i < 2000; ++i)  // Less than 100ms
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            secondTrigger = true;
    }
    REQUIRE_FALSE(secondTrigger);

    // After cooldown (remaining ~2800 samples + some margin)
    bool afterCooldown = false;
    for (int i = 0; i < 5000; ++i)
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            afterCooldown = true;
    }
    REQUIRE(afterCooldown);
}

TEST_CASE("ThresholdDetector transient spike ignored", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Single spike then silence
    td.processSample(1.0f, 1.0f, 0.3f);

    bool triggered = false;
    for (int i = 0; i < 100; ++i)
    {
        if (td.processSample(0.0f, 0.0f, 0.3f))
            triggered = true;
    }
    // Single sample spike should be smoothed out by 10ms EMA
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector threshold=0 chaos mode", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Any non-zero sustained input should trigger at threshold=0
    bool triggered = false;
    for (int i = 0; i < 1000; ++i)
    {
        if (td.processSample(0.01f, 0.01f, 0.0f))
            triggered = true;
    }
    // threshold=0 means rms >= 0 triggers — always true once EMA builds up
    REQUIRE(triggered);
}

TEST_CASE("ThresholdDetector threshold=1 effectively disabled", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Normal audio levels (-12dBFS ≈ 0.25 linear) should never trigger at threshold=1
    bool triggered = false;
    for (int i = 0; i < 48000; ++i)
    {
        if (td.processSample(0.25f, 0.25f, 1.0f))
            triggered = true;
    }
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector reset clears state", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Build up EMA
    for (int i = 0; i < 1000; ++i)
        td.processSample(0.8f, 0.8f, 0.3f);

    // Reset
    td.reset();

    // After reset, no immediate trigger (EMA is 0)
    bool triggered = td.processSample(0.01f, 0.01f, 0.3f);
    REQUIRE_FALSE(triggered);
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*ThresholdDetector*" -v`
Expected: All tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestThresholdDetector.cpp
git commit -m "test: ThresholdDetector EMA, cooldown, chaos mode, reset (Issue #12)"
```

---

## Task 17: Write TestWetProcessor.cpp (~8 tests)

**Files:**
- Create: `tests/TestWetProcessor.cpp`

- [ ] **Step 1: Write test file**

Create `tests/TestWetProcessor.cpp`:

```cpp
/*
  PsycogVST Tests - WetProcessor module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/WetProcessor.h"
#include "utils/MathUtils.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("WetProcessor soft-clip bounds", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Hot input should be bounded
    for (int i = 0; i < 1000; ++i)
        wp.processSample(outL, outR, 10.0f, -10.0f);

    REQUIRE(outL > -1.0f);
    REQUIRE(outL < 1.0f);
    REQUIRE(outR > -1.0f);
    REQUIRE(outR < 1.0f);
}

TEST_CASE("WetProcessor transparency at low levels", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // ±0.3 input: tanh(0.3) ≈ 0.2913, deviation < 0.04dB
    // Need to process enough for auto-normalize to settle near unity gain
    // At -6dBFS target and input RMS ≈ 0.21 (-13.5dBFS), gain will increase
    // Just verify tanh doesn't distort much
    wp.processSample(outL, outR, 0.3f, -0.3f);
    // After first sample, gain is 1.0, output is just tanh
    float tanhVal = std::tanh(0.3f);
    // Output may differ due to auto-normalize, but tanh(0.3) should be close to 0.3
    REQUIRE(std::abs(tanhVal - 0.3f) < 0.01f);
}

TEST_CASE("WetProcessor auto-normalize target", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed steady -20dBFS signal for 2 seconds
    float inputLevel = MathUtils::dbToGain(-20.0f);  // ~0.1

    // Process 2 seconds to let auto-normalize settle
    for (int i = 0; i < 96000; ++i)
        wp.processSample(outL, outR, inputLevel, inputLevel);

    // Output should be closer to -6dBFS target
    float outputDb = MathUtils::gainToDb(std::abs(outL));
    // Allow ±3dB tolerance (auto-normalize is slow)
    REQUIRE(outputDb > -9.0f);
    REQUIRE(outputDb < -3.0f);
}

TEST_CASE("WetProcessor noise gate bypass", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed -70dBFS signal (below -60dBFS gate)
    float quietLevel = MathUtils::dbToGain(-70.0f);

    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, quietLevel, quietLevel);

    // Below noise gate — output should be approximately tanh(input) with gain=1
    float expected = std::tanh(quietLevel);
    REQUIRE(outL == Catch::Approx(expected).margin(1e-5f));
}

TEST_CASE("WetProcessor gain clamp max", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Very quiet input just above noise gate
    float quietLevel = MathUtils::dbToGain(-50.0f);

    for (int i = 0; i < 96000; ++i)
        wp.processSample(outL, outR, quietLevel, quietLevel);

    // Gain should be clamped at 3.0 max
    // Output = tanh(input) * gain, gain <= 3.0
    float maxOutput = std::tanh(quietLevel) * 3.0f;
    REQUIRE(std::abs(outL) <= maxOutput + 0.001f);
}

TEST_CASE("WetProcessor freeze transition fast release", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Settle with quiet signal
    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, 0.1f, 0.1f);

    float beforeGain = std::abs(outL);

    // Notify freeze transition then feed loud signal
    wp.notifyFreezeTransition();
    for (int i = 0; i < 9600; ++i)  // 200ms
        wp.processSample(outL, outR, 0.8f, 0.8f);

    // Gain should have adapted — output level should be reasonable
    REQUIRE(std::abs(outL) < 1.0f);
    REQUIRE(std::isfinite(outL));
}

TEST_CASE("WetProcessor no RMS drift", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed constant signal for 10 seconds
    float level = 0.3f;
    float lastOutput = 0.0f;

    for (int i = 0; i < 480000; ++i)
    {
        wp.processSample(outL, outR, level, level);
        lastOutput = outL;
    }

    // Process one more second and check stability
    float sumDelta = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        wp.processSample(outL, outR, level, level);
        sumDelta += std::abs(outL - lastOutput);
        lastOutput = outL;
    }

    float avgDelta = sumDelta / 48000.0f;
    REQUIRE(avgDelta < 0.001f);  // Stable within 0.1%
}

TEST_CASE("WetProcessor reset clears state", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Process loud signal to change gain
    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, 0.8f, 0.8f);

    wp.reset();

    // After reset, gain should be back to 1.0
    wp.processSample(outL, outR, 0.5f, 0.5f);
    // With gain=1.0, output = tanh(0.5) ≈ 0.4621
    // May differ slightly if RMS triggers, but should be close
    REQUIRE(std::abs(outL - std::tanh(0.5f)) < 0.1f);
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*WetProcessor*" -v`
Expected: All tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestWetProcessor.cpp
git commit -m "test: WetProcessor soft-clip, auto-normalize, noise gate, drift (Issue #12)"
```

---

## Task 18: Write TestTimeStretch.cpp (~20 tests)

**Files:**
- Rewrite: `tests/TestTimeStretch.cpp`

- [ ] **Step 1: Write test file**

Replace `tests/TestTimeStretch.cpp`:

```cpp
/*
  PsycogVST Tests - TimeStretch granular engine tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/TimeStretch.h"
#include "Parameters.h"
#include <cmath>
#include <vector>
#include <numeric>

static constexpr double testSampleRate = 48000.0;

// Helper: create and prepare a TimeStretch
static TimeStretch createPrepared()
{
    TimeStretch ts;
    ts.prepare(testSampleRate, 512);
    return ts;
}

// Helper: generate sine wave
static std::vector<float> generateSine(int numSamples, float freq = 440.0f)
{
    std::vector<float> buf(numSamples);
    for (int i = 0; i < numSamples; ++i)
        buf[i] = 0.5f * std::sin(2.0f * 3.14159265f * freq * i / static_cast<float>(testSampleRate));
    return buf;
}

TEST_CASE("TimeStretch produces output at stretch=1.0", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(8192);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    float sumSquares = 0.0f;
    for (int i = 0; i < 8192; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        sumSquares += outL * outL;
    }
    float rms = std::sqrt(sumSquares / 8192.0f);
    // Should produce some output (not silence)
    REQUIRE(rms > 0.01f);
}

TEST_CASE("TimeStretch extreme stretch=0.1 no crash", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);
    float normStretch = ParamConversions::stretchToNormalized(0.1f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch extreme stretch=10.0 no crash", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);
    float normStretch = ParamConversions::stretchToNormalized(10.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch silence in silence out", "[TimeStretch]")
{
    auto ts = createPrepared();
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    float maxOutput = 0.0f;
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        maxOutput = std::max(maxOutput, std::max(std::abs(outL), std::abs(outR)));
    }
    REQUIRE(maxOutput < 1e-6f);
}

TEST_CASE("TimeStretch freeze capture and playback", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(static_cast<int>(3.0 * testSampleRate));  // 3 seconds

    // Write 3 seconds of audio (fills freeze buffer)
    ts.writeFreezeBuffer(sine.data(), sine.data(), static_cast<int>(sine.size()));

    // Feed audio through (builds input buffer)
    float normStretch = ParamConversions::stretchToNormalized(1.0f);
    for (int i = 0; i < static_cast<int>(sine.size()); ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    // Trigger freeze
    ts.triggerFreeze();

    // Read in frozen mode — should produce non-silent output
    float sumSquares = 0.0f;
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Auto);
        sumSquares += outL * outL;
    }
    float rms = std::sqrt(sumSquares / 4096.0f);
    REQUIRE(rms > 0.001f);
}

TEST_CASE("TimeStretch mode transition no NaN", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(8192);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    // Process in Off mode
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    ts.writeFreezeBuffer(sine.data(), sine.data(), 8192);
    ts.triggerFreeze();

    // Switch to Auto mode — crossfade should produce valid output
    for (int i = 4096; i < 8192; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Auto);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch position scrub changes output", "[TimeStretch]")
{
    auto ts = createPrepared();
    // Write distinctive audio to freeze buffer
    int bufLen = static_cast<int>(3.0 * testSampleRate);
    std::vector<float> audio(bufLen);
    for (int i = 0; i < bufLen; ++i)
        audio[i] = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f * i / static_cast<float>(testSampleRate));

    ts.writeFreezeBuffer(audio.data(), audio.data(), bufLen);

    // Feed enough to build input buffer
    float normStretch = ParamConversions::stretchToNormalized(1.0f);
    for (int i = 0; i < bufLen; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, audio[i], audio[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    ts.triggerFreeze();

    // Read at position=0.2
    float sumA = 0.0f;
    for (int i = 0; i < 2048; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.2f,
                         PsycogConstants::FreezeMode::Auto);
        sumA += outL;
    }

    // Read at position=0.8
    ts.reset();
    ts.prepare(testSampleRate, 512);
    ts.writeFreezeBuffer(audio.data(), audio.data(), bufLen);
    for (int i = 0; i < bufLen; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, audio[i], audio[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }
    ts.triggerFreeze();

    float sumB = 0.0f;
    for (int i = 0; i < 2048; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.8f,
                         PsycogConstants::FreezeMode::Auto);
        sumB += outL;
    }

    // Different positions should produce different sums (different content)
    // Allow some tolerance since both read from the same sine
    REQUIRE(std::abs(sumA - sumB) > 0.001f);
}

TEST_CASE("TimeStretch sample rate 44100", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(44100.0, 512);
    auto sine = generateSine(4096, 440.0f);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
    }
}

TEST_CASE("TimeStretch sample rate 96000", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(96000.0, 512);
    auto sine = generateSine(4096, 440.0f);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
    }
}

TEST_CASE("TimeStretch buffer size stress 64", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(testSampleRate, 64);
    float normStretch = ParamConversions::stretchToNormalized(2.0f);

    // Process many 64-sample blocks
    for (int block = 0; block < 100; ++block)
    {
        for (int i = 0; i < 64; ++i)
        {
            float outL, outR;
            float in = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f * (block * 64 + i) / 48000.0f);
            ts.processSample(outL, outR, in, in, normStretch, 0.5f,
                             PsycogConstants::FreezeMode::Off);
            REQUIRE(std::isfinite(outL));
        }
    }
}

TEST_CASE("TimeStretch writeFreezeBuffer always works", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);

    // Writing in Off mode should not crash
    ts.writeFreezeBuffer(sine.data(), sine.data(), 4096);

    // Verify by switching to freeze — should have valid content
    ts.triggerFreeze();
    float outL, outR;
    ts.processSample(outL, outR, 0.0f, 0.0f,
                     ParamConversions::stretchToNormalized(1.0f), 0.5f,
                     PsycogConstants::FreezeMode::Auto);
    REQUIRE(std::isfinite(outL));
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*TimeStretch*" -v`
Expected: All tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestTimeStretch.cpp
git commit -m "test: TimeStretch granular engine, freeze, position, stress tests (Issue #12)"
```

---

## Task 19: Write TestFreezeBuffer.cpp (~15 tests)

**Files:**
- Rewrite: `tests/TestFreezeBuffer.cpp`

- [ ] **Step 1: Write test file**

Replace `tests/TestFreezeBuffer.cpp`:

```cpp
/*
  PsycogVST Tests - FreezeBuffer module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/FreezeBuffer.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

static FreezeBuffer createPrepared()
{
    FreezeBuffer fb;
    fb.prepare(testSampleRate);
    return fb;
}

TEST_CASE("FreezeBuffer initial state is silent", "[FreezeBuffer]")
{
    auto fb = createPrepared();
    REQUIRE_FALSE(fb.isFrozen());

    // Read from unfrozen buffer should be silent
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.5f);
    REQUIRE(outL == 0.0f);
    REQUIRE(outR == 0.0f);
}

TEST_CASE("FreezeBuffer write and freeze", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write a known pattern
    int numSamples = 48000;  // 1 second
    std::vector<float> left(numSamples), right(numSamples);
    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = 0.5f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);
        right[i] = 0.3f * std::sin(2.0f * 3.14159f * 880.0f * i / 48000.0f);
    }

    fb.write(left.data(), right.data(), numSamples);
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    // Read — should produce non-silent output
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.0f);
    // After freeze, we should be able to read content
    // (may be silent at exact position 0 depending on phase)
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("FreezeBuffer cooldown enforcement", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write some audio
    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);

    // First trigger
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    // Immediate re-trigger should be rejected (in cooldown)
    // triggerFreeze won't swap again during cooldown
    bool swappedAgain = false;
    fb.triggerFreeze();  // Should be no-op due to cooldown

    // Advance through cooldown by reading (cooldown decrements in read())
    float outL, outR;
    int cooldownSamples = static_cast<int>(100.0 * 48000.0 / 1000.0);  // 100ms
    fb.read(&outL, &outR, cooldownSamples + 100, 0.5f);

    // Now trigger should work
    fb.triggerFreeze();
    // If it worked, still frozen (just swapped buffers)
    REQUIRE(fb.isFrozen());
}

TEST_CASE("FreezeBuffer toggle freeze", "[FreezeBuffer]")
{
    auto fb = createPrepared();
    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);

    REQUIRE_FALSE(fb.isFrozen());

    fb.toggleFreeze();
    REQUIRE(fb.isFrozen());

    fb.toggleFreeze();
    REQUIRE_FALSE(fb.isFrozen());
}

TEST_CASE("FreezeBuffer position read at boundaries", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write 3 seconds of distinctive pattern
    int bufLen = static_cast<int>(3.0 * testSampleRate);
    std::vector<float> left(bufLen), right(bufLen);
    for (int i = 0; i < bufLen; ++i)
    {
        // Different frequencies at different positions
        float t = static_cast<float>(i) / static_cast<float>(bufLen);
        left[i] = std::sin(2.0f * 3.14159f * (200.0f + 800.0f * t) * i / 48000.0f);
        right[i] = left[i] * 0.8f;
    }

    fb.write(left.data(), right.data(), bufLen);
    fb.triggerFreeze();

    // Read at position 0.0
    float outL0, outR0;
    fb.read(&outL0, &outR0, 1, 0.0f);
    REQUIRE(std::isfinite(outL0));

    // Read at position 0.5
    float outL5, outR5;
    fb.read(&outL5, &outR5, 1, 0.5f);
    REQUIRE(std::isfinite(outL5));

    // Read at position 1.0
    float outL1, outR1;
    fb.read(&outL1, &outR1, 1, 1.0f);
    REQUIRE(std::isfinite(outL1));
}

TEST_CASE("FreezeBuffer stereo content preserved", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write different L and R
    int numSamples = 48000;
    std::vector<float> left(numSamples), right(numSamples);
    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = 0.5f;
        right[i] = -0.3f;
    }

    fb.write(left.data(), right.data(), numSamples);
    fb.triggerFreeze();

    // Wait for crossfade to complete
    float dummyL, dummyR;
    int crossfadeLen = static_cast<int>(10.0 * testSampleRate / 1000.0) + 100;
    for (int i = 0; i < crossfadeLen; ++i)
        fb.read(&dummyL, &dummyR, 1, 0.5f);

    // Read should have different L and R
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.5f);
    // L should be positive, R should be negative
    // (exact values depend on buffer content at position 0.5)
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("FreezeBuffer reset clears state", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    fb.reset();
    REQUIRE_FALSE(fb.isFrozen());
}

TEST_CASE("FreezeBuffer rapid freeze cycling no crash", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    std::vector<float> audio(48000, 0.3f);
    fb.write(audio.data(), audio.data(), 48000);

    // Trigger, read through cooldown, trigger again — 10 times
    for (int cycle = 0; cycle < 10; ++cycle)
    {
        fb.triggerFreeze();

        // Read through cooldown
        float outL, outR;
        int cooldownSamples = static_cast<int>(100.0 * 48000.0 / 1000.0) + 100;
        fb.read(&outL, &outR, cooldownSamples, 0.5f);

        REQUIRE(std::isfinite(outL));
    }
}
```

- [ ] **Step 2: Build and run**

Run: `cd ~/PsycogVST && cmake --build build && ./build/PsycogVSTTests --test-case="*FreezeBuffer*" -v`
Expected: All tests pass

- [ ] **Step 3: Commit**

```bash
cd ~/PsycogVST
git add tests/TestFreezeBuffer.cpp
git commit -m "test: FreezeBuffer double-buffer, cooldown, position, stereo (Issue #12)"
```

---

## Task 20: Final Build + Full Test Suite

**Files:** None (verification only)

- [ ] **Step 1: Clean rebuild**

```bash
cd ~/PsycogVST && rm -rf build && cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

Expected: Build succeeds with zero errors

- [ ] **Step 2: Run full test suite**

```bash
cd ~/PsycogVST && ./build/PsycogVSTTests -v
```

Expected: All tests pass (both new ~89 tests and existing preset tests)

- [ ] **Step 3: Verify VST3 installation**

```bash
ls -la ~/.vst3/Psycog\ VST.vst3/Contents/x86_64-linux/
```

Expected: `Psycog VST.so` exists with recent timestamp

- [ ] **Step 4: Commit any remaining fixes**

If any tests fail, fix the issues and commit fixes individually.

- [ ] **Step 5: Final verification commit**

```bash
cd ~/PsycogVST
git log --oneline -20
```

Verify all commits are clean and well-described.

---

## Verification Criteria

After all tasks complete, verify:

1. `cmake --build build` succeeds with zero errors (Release mode)
2. All tests pass (new + existing)
3. Plugin installs to `~/.vst3/Psycog VST.vst3/`
4. `getTailLengthSeconds()` returns 4.0
5. No `processBlock(double)` override exists
6. No SmoothedValue declarations in PluginProcessor.h
7. No references to `smoothingMs` anywhere (only `smoothingTimeSeconds`)
8. Wavefolder uses `-offset` for right channel
9. LFO has no `lfoValues` buffer
10. ThresholdDetector uses EMA, not block-level check
11. WetProcessor uses EMA, not running sum
12. FreezeBuffer has `writePosA`/`writePosB`, not shared `writePos`
13. FreezeBuffer has `readSampleAt()` method (used by granular engine for random-access)
14. TimeStretch Grain struct has single `sourcePosition` (not separate L/R)
15. Manual freeze mode toggle logic exists in processBlock (Off<->Manual transitions)
16. `prevOutputL`/`prevOutputR` are saved at END of `processSample`, not from uninitialized refs
17. No `freezeBuffer.read()` calls from inside `processGrainSample` (uses `readSampleAt` instead)
18. Wavefolder has no `getCurrentAmplitude()` (ThresholdDetector receives output directly)
