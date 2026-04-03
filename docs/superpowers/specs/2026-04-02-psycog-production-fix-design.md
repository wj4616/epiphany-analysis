# PsycogVST Production-Grade Fix Specification

**Version:** 1.0  
**Date:** 2026-04-02  
**Status:** Approved for Implementation  
**Scope:** 13 issues from code audit — correctness bugs, granular engine, architecture fixes, comprehensive tests  
**Parent Spec:** `docs/superpowers/specs/2026-04-02-psycog-vst-design.md`

---

## Overview

This spec addresses all 13 issues identified in the PsycogVST code audit. The implementation order is: granular engine first (#7), then signal flow rewrite (#2), then module fixes outward, then infrastructure, then tests last (written against final code).

### Design Decisions (from brainstorming)

| Decision | Choice | Rationale |
|----------|--------|-----------|
| Implementation order | Granular engine first, fix outward | Core engine defines the architecture; fixes build on the real signal flow |
| Granular engine type | 4-grain variable overlap-add | Quality across full 0.1x–10x range; psychedelic character preserved at extremes |
| Smoothing ownership | Modules own their smoothing | Self-contained, independently testable modules; processor stays lean |
| Threshold detection | Sliding window RMS (10ms EMA) | Per-sample resolution, smooths transient noise, catches sustained levels |
| Test coverage | Comprehensive with signal integrity | ~80-100 tests covering correctness, boundary values, crossfade clicks, state transitions |

### Spec Amendments to Parent Design

| Parameter | Original Spec | Amendment | Reason |
|-----------|--------------|-----------|--------|
| Auto-normalize release | 1000ms always | 100ms during freeze transitions, 1000ms otherwise | Prevents 1-second hot signal burst when freeze content changes dramatically |
| Granular engine | "2048-sample Hann window" (unspecified grain count) | 4-grain variable overlap-add with ±64 sample jitter | Smoother quality at extreme stretch; jitter prevents metallic periodicity artifacts |

---

## Issue #7: Granular Time-Stretch Engine

### Architecture

4-grain variable overlap-add engine inside `TimeStretch`. This is the core DSP — all other fixes build around it.

### Grain Structure

```cpp
struct Grain
{
    float sourcePosition;     // Where in source buffer this grain reads from (float for sub-sample)
    int playbackPosition;     // How far through the grain we've played (0 to grainSize)
    bool active;              // Is this grain currently producing output
    float amplitude;          // Current Hann window amplitude at playbackPosition
};
```

- 4 grain slots, pre-allocated
- Hann window lookup table: pre-computed at `prepare()`, size = `grainSize` (2048)
- Circular input buffer: `grainSize * 8` samples (16384) — headroom for 4 overlapping grains at extreme stretch

### Grain Lifecycle

1. **Launch:** Grain scheduler determines it's time for a new grain. Assigns `sourcePosition` = current input read head position + random jitter (±64 samples). Sets `playbackPosition = 0`, `active = true`.
2. **Advance:** Each sample, grain increments `playbackPosition` by 1. Reads source buffer at `sourcePosition + playbackPosition`. Applies Hann window from lookup table at `playbackPosition`. Outputs windowed sample.
3. **Overlap:** All active grains sum into output. Hann windowing at variable overlap gives constant-power crossfade.
4. **Expire:** When `playbackPosition >= grainSize`, grain sets `active = false`. Slot is free for reuse.

### Stretch Mechanism (Unfrozen Mode)

Time-stretch by controlling grain spacing:

```
hopSize = grainSize / (stretch * overlapFactor)
overlapFactor = 2  (50% base overlap, up to 75% with 4 grains at high stretch)
```

- **stretch > 1x (expansion):** hop < grainSize → grains overlap more → same audio repeated → slows down
- **stretch = 1x:** hop = grainSize/overlapFactor → normal reconstruction with ~46ms latency
- **stretch < 1x (compression):** hop > grainSize → input read head skips forward → audio discarded → glitchy character (per parent spec: "intentional psychedelic aesthetic")

Input read head advances by `hopSize` each time a new grain launches. Hop size is locked at grain launch time — not updated per-sample — to prevent amplitude modulation artifacts from LFO-modulated stretch.

### Frozen Mode (Pitch Shift)

When frozen, grains read from the `FreezeBuffer` playback buffer instead of the input circular buffer:

- `position` parameter (0–1) sets the base read position within the 3-second frozen buffer
- `stretch` controls grain playback rate: each grain advances through the frozen buffer at `stretch` speed
  - stretch=2x → grains read 2x faster → pitch up one octave
  - stretch=0.5x → grains read at half speed → pitch down one octave
  - stretch=1x → normal pitch, looping at position
- Hop size stays constant in frozen mode (time is frozen; only pitch changes)

### Freeze Transitions

- 10ms crossfade at TimeStretch output level (not inside FreezeBuffer)
- When freezing: crossfade from live granular output to frozen granular output
- When unfreezing: crossfade from frozen to live
- TimeStretch manages the crossfade; FreezeBuffer is pure storage

### Grain Jitter

Each grain launch adds random jitter of ±64 samples to `sourcePosition`. Prevents periodic artifacts at extreme expansion (10x) where 4 grains repeat the same audio. Uses a simple linear congruential generator (no `std::mt19937` on audio thread — too heavy).

### Interface

Replace the block-level `process()` with per-sample `processSample()`:

```cpp
void prepare(double sampleRate, int maxSamples);

// Per-sample processing for the wet path loop
void processSample(float& outL, float& outR,
                   float inL, float inR,
                   float normalizedStretch, float normalizedPosition,
                   PsycogConstants::FreezeMode freezeMode);

void triggerFreeze();
void reset();
```

The old `process(float* leftOut, ...)` block-level method is removed. The processor calls `processSample()` once per sample in the wet loop. TimeStretch smooths `stretch` and `position` internally (module owns smoothing).

---

## Issue #2: Signal Flow Rewrite

### New processBlock Flow

```
1. Read all raw parameter values from APVTS (once per block, no smoothing)
2. Configure LFO (rate, waveform, depth, targets)
3. Dry path: DryDelay.process(input → dryBuffer)  [block-level]
4. FreezeBuffer.write(input)  [block-level, ALWAYS — even in Off mode]
5. Wet path PER-SAMPLE loop (i = 0 to numSamples):
   a. lfoValue = lfo.advance()
   b. Compute modulated params: base + lfoValue (clamped)
   c. [L, R] = timeStretch.processSample(inputL, inputR, stretch, position, freezeMode)
   d. [L, R] = wavefolder.processSample(L, R, foldAmount, foldOffset, isMono)
   e. triggered = thresholdDetector.processSample(L, R, threshold)
   f. if (triggered && freezeMode == Auto): timeStretch.triggerFreeze()
                                            wetProcessor.notifyFreezeTransition()
   g. [L, R] = wetProcessor.processSample(L, R)
   h. wetBuffer[i] = [L, R]
6. Mixer.process(wetBuffer, dryBuffer, mix)  [block-level, smooths mix internally]
7. OutputProtection.process(output)  [block-level]
```

### Key Differences from Current Code

| Aspect | Current (Broken) | New (Correct) |
|--------|-----------------|---------------|
| Signal order | Wavefolder(raw input) → overwrite with TimeStretch | TimeStretch → Wavefolder → WetProcessor |
| Freeze path | Separate block-level path overwrites per-sample wet output | Unified per-sample path handles all modes |
| Threshold detection | Once per block, single-sample amplitude | Per-sample, 10ms sliding window RMS |
| Freeze trigger | End of block, delayed reaction | Immediate per-sample, takes effect next sample |
| FreezeBuffer recording | Only in Manual/Auto modes | Always recording, all modes (spec requirement) |

### Per-Sample Module Interfaces

Modules in the per-sample loop get `processSample()` methods:

```cpp
// TimeStretch
void processSample(float& outL, float& outR,
                   float inL, float inR,
                   float normalizedStretch, float normalizedPosition,
                   PsycogConstants::FreezeMode freezeMode);

// Wavefolder
void processSample(float& outL, float& outR,
                   float inL, float inR,
                   float foldAmount, float foldOffset, bool isMono);

// ThresholdDetector
bool processSample(float ampL, float ampR, float threshold);

// WetProcessor
void processSample(float& outL, float& outR, float inL, float inR);

// LFO
float advance();  // Returns bipolar value * depth
```

Block-level modules (DryDelay, Mixer, OutputProtection) keep their existing `process()` signatures.

---

## Issue #1: SmoothedValue Ramp Time Fix

### The Bug

`SmoothedValue::reset(double sampleRate, double rampLengthInSeconds)` expects **seconds**.  
`PsycogConstants::smoothingMs = 20.0` is passed directly → 20-second ramp time.

### The Fix

Rename and correct the constant in `Constants.h`:

```cpp
// BEFORE (broken):
constexpr double smoothingMs = 20.0;

// AFTER (correct):
constexpr double smoothingTimeSeconds = 0.02;  // 20ms
```

All module `reset()` calls use `smoothingTimeSeconds`.

`WetProcessor` gain smoother uses `0.01` (10ms) — it's an internal envelope follower, not a UI parameter smoother.

---

## Issue #3: Remove Double Smoothing

### Ownership Rule

**Modules own all parameter smoothing.** The processor reads raw APVTS values, applies LFO modulation (pure arithmetic, no smoothing), and passes values to modules. Modules smooth internally per-sample.

### Changes

**Remove from `PsycogAudioProcessor`:**
- `stretchSmoother` — owned by TimeStretch
- `positionSmoother` — owned by TimeStretch
- `thresholdSmoother` — threshold is discrete, no smoothing needed (it's a comparator input)
- `foldAmountSmoother` — owned by Wavefolder
- `foldOffsetSmoother` — owned by Wavefolder
- `mixSmoother` — owned by Mixer

**Module smoothing (already exists, just fix ramp time):**

| Module | Smoothed Parameters | Ramp Time |
|--------|-------------------|-----------|
| TimeStretch | stretch, position | 20ms |
| Wavefolder | foldAmount, foldOffset | 20ms |
| Mixer | mix | 20ms |
| LFO | depth | 20ms |
| WetProcessor | gain (internal) | 10ms |

---

## Issue #4: Stereo Wavefolder Offset

### The Bug

`Wavefolder.cpp` applies same `+offset` to both channels for stereo input.  
Spec requires `+offset` for L, `-offset` for R.

### The Fix

In `Wavefolder::processSample()` for stereo input:

```cpp
// BEFORE (broken):
leftOut  = applyWavefold(leftIn,  foldAmount, +offset);
rightOut = applyWavefold(rightIn, foldAmount, +offset);  // same offset!

// AFTER (correct):
leftOut  = applyWavefold(leftIn,  foldAmount, +offset);
rightOut = applyWavefold(rightIn, foldAmount, -offset);  // opposite offset
```

Both stereo and mono paths now create stereo width from offset.

---

## Issue #5: LFO Buffer Overread

### The Bug

`lfoValues` vector is hardcoded to 4096. If host sends block > 4096, `getValue(sampleIndex)` reads out of bounds.

### The Fix

Remove `lfoValues` buffer entirely. LFO computes inline per-sample via `advance()`:

```cpp
float LFO::advance()
{
    float smoothedDepth = depthSmoother.getNextValue();
    float value = 0.0f;
    
    switch (waveform)
    {
        case Sine:     value = std::sin(2.0f * pi * phase); break;
        case Triangle: value = 1.0f - 4.0f * std::abs(phase - 0.5f); break;
        case Square:   value = (phase < 0.5f) ? 1.0f : -1.0f; break;
        case SampleAndHold: value = heldValue; break;
    }
    
    float prevPhase = phase;
    phase += phaseIncrement;
    if (phase >= 1.0f) phase -= 1.0f;
    
    // S&H: new random value on cycle wrap
    if (waveform == SampleAndHold && phase < prevPhase)
        heldValue = nextRandom();  // lightweight LCG, not std::mt19937
    
    return value * smoothedDepth;
}
```

No buffer, no bounds issue, less memory, simpler code.

### Remove from LFO class

- `std::vector<float> lfoValues`
- `int blockSize`
- `void process(int numSamples)`
- `float getValue(int sampleIndex) const`

### Add to LFO class

- `float advance()` — step phase, return value
- `float nextRandom()` — lightweight LCG for audio thread safety (replaces `std::mt19937`)

---

## Issue #6: LFO S&H Spurious Triggers

### The Bug

Condition `phase < phaseIncrement` fires at block start when phase is near zero.

### The Fix

Detect wrap by comparing phase before and after increment:

```cpp
float prevPhase = phase;
phase += phaseIncrement;
if (phase >= 1.0f) phase -= 1.0f;

if (waveform == SampleAndHold && phase < prevPhase)
    heldValue = nextRandom();
```

`phase < prevPhase` is true only when phase actually wraps past 1.0.

### Additional Fix: S&H Initialization

Initialize `heldValue` with a random value in `prepare()`, not zero. Prevents 0-100 seconds of zero output before first cycle wrap.

---

## Issue #8: Threshold Detection Redesign

### The Bug

Wavefolder called with `numSamples=1` in per-sample loop → `currentAmplitude = abs(lastSample)`. Single-sample value used for block-level threshold check. Extremely noisy.

### The Fix

`ThresholdDetector` becomes a per-sample sliding window RMS tracker with integrated cooldown:

```cpp
class ThresholdDetector
{
public:
    void prepare(double sampleRate);
    void reset();
    
    // Returns true if freeze should trigger
    // Call once per sample with wavefolder output amplitude
    bool processSample(float ampL, float ampR, float threshold);

private:
    float rmsPower = 0.0f;    // EMA of squared amplitude
    float alpha = 0.0f;       // EMA coefficient (10ms time constant)
    int cooldownRemaining = 0;
    int cooldownLength = 0;   // 100ms in samples
};
```

Implementation:

```cpp
bool ThresholdDetector::processSample(float ampL, float ampR, float threshold)
{
    // Track peak of L/R
    float peak = std::max(std::abs(ampL), std::abs(ampR));
    
    // Update 10ms EMA
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

Alpha computed in `prepare()`: `alpha = 1.0f - std::exp(-1.0f / (0.01f * sampleRate))`

Removes: `advanceCooldown()`, `startCooldown()`, `isInCooldown()`, `checkThreshold()`.  
Replaces with single `processSample()`.

---

## Issue #10: FreezeBuffer Fixes (4 sub-bugs)

### Bug 10a: Single writePos shared between buffers

**Fix:** Per-buffer write positions.

```cpp
// BEFORE:
int writePos = 0;

// AFTER:
int writePosA = 0;
int writePosB = 0;
```

`write()` uses the active recording buffer's own writePos.

### Bug 10b: Recording only in Manual/Auto mode

**Fix:** `FreezeBuffer::write()` is called from `processBlock` ALWAYS, regardless of freeze mode. Move the call outside any mode conditional. In the new signal flow (Section: Issue #2), step 4 calls `FreezeBuffer.write(input)` unconditionally before the wet path loop.

### Bug 10c: WritePos reset on swap

**Fix:** When `triggerFreeze()` swaps buffers, reset the new recording buffer's writePos to 0.

```cpp
void FreezeBuffer::triggerFreeze()
{
    if (cooldownRemaining <= 0)
    {
        bufferAIsPlayback = !bufferAIsPlayback;
        frozen = true;
        crossfadeProgress = 0.0f;
        cooldownRemaining = cooldownLength;
        
        // Reset new recording buffer's write position
        if (bufferAIsPlayback)
            writePosB = 0;  // B is now recording
        else
            writePosA = 0;  // A is now recording
    }
}
```

No buffer clear needed — circular write overwrites stale content within 3 seconds.

### Crossfade ownership clarification

FreezeBuffer retains crossfade logic for buffer-swap transitions (it knows the old and new buffer content). TimeStretch handles the freeze/unfreeze mode transition crossfade (live↔frozen). These are different events:
- **Buffer swap** (during Auto mode): FreezeBuffer crossfades between old and new frozen content
- **Mode transition** (Off↔Manual, Off↔Auto): TimeStretch crossfades between live granular and frozen granular output

---

## Issue #9: PresetManager Fix

### The Bug

`applyPreset()` creates flat XML attributes, converts to ValueTree, calls `replaceState()`. APVTS expects a specific tree structure with `<PARAM>` child elements. Presets silently don't apply.

### The Fix

Set parameters directly through APVTS parameter objects:

```cpp
bool applyPreset(juce::AudioProcessorValueTreeState& apvts, int index)
{
    if (index < 0 || index >= numPresets) return false;
    const auto& p = factoryPresets[index];
    
    // Use convertTo0to1() for ALL parameters to handle range normalization correctly
    auto setNorm = [&](const char* id, float rawValue) {
        if (auto* param = apvts.getParameter(id))
            param->setValueNotifyingHost(param->convertTo0to1(rawValue));
    };
    
    setNorm(ParamIDs::stretch,    p.stretch);
    setNorm(ParamIDs::position,   p.position);
    setNorm(ParamIDs::threshold,  p.threshold);
    setNorm(ParamIDs::foldAmount, p.foldAmount);
    setNorm(ParamIDs::foldOffset, p.foldOffset);   // convertTo0to1 handles [-1,1] → [0,1]
    setNorm(ParamIDs::lfoRate,    p.lfoRate);
    setNorm(ParamIDs::lfoDepth,   p.lfoDepth);
    setNorm(ParamIDs::mix,        p.mix);
    setNorm(ParamIDs::freezeMode, static_cast<float>(p.freezeMode));
    setNorm(ParamIDs::lfoWaveform, static_cast<float>(p.lfoWaveform));
    
    // Bool parameters: convertTo0to1 handles bool→float correctly
    setNorm(ParamIDs::lfoTargetStretch,    p.lfoTargetStretch ? 1.0f : 0.0f);
    setNorm(ParamIDs::lfoTargetPosition,   p.lfoTargetPosition ? 1.0f : 0.0f);
    setNorm(ParamIDs::lfoTargetFoldAmount, p.lfoTargetFoldAmount ? 1.0f : 0.0f);
    setNorm(ParamIDs::lfoTargetFoldOffset, p.lfoTargetFoldOffset ? 1.0f : 0.0f);
    
    return true;
}
```

`setValueNotifyingHost()` takes normalized 0–1 values. `convertTo0to1()` handles all range mapping (including foldOffset's [-1, 1] range). This updates APVTS state correctly and notifies the DAW for automation recording.

### Also Fix: getStateInformation / setStateInformation

Current `setStateInformation` checks `xml->hasTagName(apvts.state.getType())`. The APVTS state type is typically `"Parameters"` (the identifier passed to the APVTS constructor). Verify this matches during implementation. The version and currentProgram attributes are stored as extra attributes on the root element — confirm this works with the actual APVTS XML structure.

### Keep createPresetState() As-Is

`createPresetState()` is used only in tests for XML comparison. It doesn't need to match APVTS format — it's a test utility.

---

## Issue #11: Complete releaseResources

### The Fix

Every module gets a `reset()` method. `releaseResources()` calls all of them:

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

Each module's `reset()`:
- Clears internal buffers to zero
- Resets smoothers to default values (`setCurrentAndTargetValue`)
- Resets phase/position counters to initial state
- Does NOT deallocate memory

---

## Issue #13: Infrastructure Cleanup

### 13a: Remove processBlock(double)

Delete the `processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&)` override entirely. JUCE's VST3 wrapper handles float↔double conversion internally. Also remove the declaration from `PluginProcessor.h`.

### 13b: Fix WetProcessor RMS Drift

Replace the flawed running-sum approach with a proper EMA:

```cpp
// Per-sample in processSample():
float samplePower = (leftClipped * leftClipped + rightClipped * rightClipped) * 0.5f;
rmsPower = rmsCoeff * rmsPower + (1.0f - rmsCoeff) * samplePower;
currentRms = std::sqrt(rmsPower);
```

Remove: `rmsSum`, `rmsCount`, `rmsWindowLength`.  
Add: `rmsPower` (single float state variable).

Asymmetric attack/release for gain changes (already exists, keep):
- Gain increasing (signal got quieter): use `attackCoeff` (300ms)
- Gain decreasing (signal got louder): use `releaseCoeff` (1000ms normal, 100ms during freeze transition)

### Freeze-Aware Release

```cpp
void WetProcessor::notifyFreezeTransition()
{
    // Temporarily use fast release (100ms)
    freezeTransitionRemaining = freezeTransitionLength;  // 200ms in samples
}
```

In `processSample()`: if `freezeTransitionRemaining > 0`, use fast release coefficient (100ms). Decrement counter each sample. When counter reaches 0, revert to normal release (1000ms).

Coefficients computed in `prepare()`:
```cpp
normalReleaseCoeff = std::exp(-1.0f / (1.0f * sampleRate));     // 1000ms
fastReleaseCoeff   = std::exp(-1.0f / (0.1f * sampleRate));     // 100ms
freezeTransitionLength = static_cast<int>(0.2 * sampleRate);     // 200ms window
```

### 13c: Tail Length

```cpp
double getTailLengthSeconds() const override { return 4.0; }
```

3-second freeze buffer + 1-second auto-normalize release = 4 seconds maximum tail.

### 13d: SIMD Optimization — Deferred

Not included in this spec. The per-sample wet path loop (granular engine, threshold detector, auto-normalize) has per-sample state dependencies that prevent vectorization. Block-level modules (DryDelay, Mixer, OutputProtection) could use `FloatVectorOperations` but the gain is marginal. Defer to Phase 10 profiling if CPU budget becomes a concern.

---

## Issue #12: Comprehensive Tests

### Test Strategy

- ~80-100 tests across 7 test files
- Each module tested in isolation with direct instantiation (no plugin host)
- Signal integrity verified: no clicks, no NaN, no drift, correct frequency content
- Boundary conditions: param=0, param=1, extreme values
- State transitions: freeze/unfreeze, mode switches, preset changes

### TestTimeStretch.cpp (~20 tests)

| Test | Verification |
|------|-------------|
| Passthrough at stretch=1.0 | Output matches input within -60dB tolerance (accounting for grain windowing) |
| Expansion at stretch=2.0 | Output contains repeated source content (grain overlap) |
| Compression at stretch=0.5 | Output sources from 2x further ahead in input |
| Extreme stretch=0.1 | Produces output without NaN/inf/silence |
| Extreme stretch=10.0 | Produces output without NaN/inf/silence |
| Freeze capture | Write known signal → freeze → read at position=0.5 → content matches expected buffer position |
| Freeze crossfade no clicks | Trigger freeze → max sample-to-sample delta < 0.1 during 10ms transition |
| Pitch shift during freeze | Frozen buffer + stretch=2.0 → output frequency ≈ 2x input frequency |
| Grain jitter active | 100 grain launches → position variance > 0 |
| Position scrub | Sweep position 0→1 while frozen → output content changes smoothly |
| Dual-mode stretch | Unfrozen: stretch changes time. Frozen: stretch changes pitch. Verify both. |
| Mode transition Off→Auto | Crossfade from live to frozen output, no click |
| Mode transition Auto→Off | Crossfade from frozen to live, no click |
| 4 grains active simultaneously | At high stretch, verify 4 grains produce summed output |
| Hop size locked at grain launch | LFO-modulated stretch → no amplitude modulation artifacts |
| Empty input | Silence in → silence out (no noise, no NaN) |
| Sample rate 44100 | Prepare at 44.1k, process → correct behavior |
| Sample rate 96000 | Prepare at 96k, process → correct behavior |
| Buffer size stress 64 | Process 64-sample blocks → no crashes |
| Buffer size stress 8192 | Process 8192-sample blocks → no crashes |

### TestFreezeBuffer.cpp (~15 tests)

| Test | Verification |
|------|-------------|
| Double-buffer swap | Write to A → freeze → A is playback, B is recording |
| Per-buffer writePos | After swap, new recording buffer writePos == 0 |
| Always-recording in Off mode | Off mode → write 1 sec → switch to Auto → freeze → frozen content is valid |
| Cooldown enforcement | Trigger → immediate re-trigger rejected → wait 100ms → trigger succeeds |
| Crossfade on swap | 10ms transition → max sample delta < 0.1 |
| Position read at 0.0 | Reads from buffer start |
| Position read at 0.5 | Reads from buffer midpoint |
| Position read at 1.0 | Reads from buffer end |
| Circular wrap | Write 4 seconds → freeze → buffer has most recent 3 seconds |
| Rapid freeze cycling | Trigger every 100ms for 1 second → no crashes, valid buffers |
| Toggle freeze (Manual) | Freeze on → verify frozen → freeze off → verify live |
| Write during freeze | While frozen, recording buffer still accepts writes |
| Initial state | Both buffers are silent after prepare() |
| Buffer content preserved | Write known pattern → freeze → read back → matches |
| Stereo content | Write different L/R → read back → L and R preserved independently |

### TestWavefolder.cpp (~15 tests)

| Test | Verification |
|------|-------------|
| Passthrough at foldAmount=0 | Output == input (within 1e-6 epsilon) |
| Identity fold drive=1 offset=0 | Signal in [-1,1] → output matches input |
| Fold boundary ±1.0 | Input=±1.0 at drive=1 → output=±1.0 |
| Output always bounded | foldAmount=1.0, offset=1.0, input=1.0 → output in [-1, 1] |
| Stereo opposite offset | Stereo input, offset=0.5 → L and R outputs differ |
| Mono stereo widening | Mono input, offset > 0 → L ≠ R |
| Harmonic content increase | foldAmount=0.5, sine input → zero-crossing count increases |
| Smoothing no discontinuity | Abrupt foldAmount 0→1 → max sample delta < 0.1 during transition |
| DC offset scaling | foldAmount=0, offset=1.0 → effective_offset=0 → passthrough |
| Triangle fold math | Verify triangleFold(0)=−1, triangleFold(1)=0, triangleFold(2)=1, triangleFold(3)=0 |
| High drive output range | drive=10.0 → all outputs still in [-1, 1] |
| Asymmetric harmonics | offset≠0 → output has even harmonics (asymmetry test) |
| Symmetric harmonics | offset=0 → output has only odd harmonics |
| Negative offset | offset=-0.5 → L gets -offset, R gets +offset (swapped from positive) |
| Zero input | Input=0 at any foldAmount → output=0 (only if offset=0 too) |

### TestLFO.cpp (~15 tests)

| Test | Verification |
|------|-------------|
| Sine shape quarter-cycle | depth=1, rate=1Hz, 48kHz → first 12000 samples rise from 0 to +1 |
| Triangle peaks | Peaks at exactly +depth and -depth |
| Triangle linear ramps | Linear interpolation between peaks |
| Square values | Exactly +depth first half, -depth second half |
| S&H constant between wraps | Output doesn't change within a cycle |
| S&H changes at wrap | Output changes when phase wraps |
| S&H non-zero initialization | First value after prepare() is non-zero |
| Rate accuracy | rate=10Hz → ~20 zero-crossings per second (±1) |
| Depth=0 always zero | Any waveform, depth=0 → output always 0.0 |
| Depth scaling | depth=0.5 → output in [-0.5, 0.5] |
| Phase continuity at block boundary | Two consecutive blocks → no jump at boundary |
| Depth smoothing | depth 0→1 → ramps over ~20ms |
| Minimum rate | 0.01Hz → phase advances very slowly, no numerical issues |
| Maximum rate | 20Hz → correct oscillation |
| S&H random distribution | 1000 S&H values → mean ≈ 0 (±0.1), no bias |

### TestThresholdDetector.cpp (~8 tests)

| Test | Verification |
|------|-------------|
| Above threshold triggers | Sustained amplitude > threshold → returns true |
| Below threshold no trigger | Amplitude < threshold → returns false |
| Cooldown blocks re-trigger | Trigger → immediate re-check → false. After 100ms → true. |
| Transient spike ignored | Single spike then silence → no trigger (10ms EMA smooths it) |
| Sustained level triggers | 10ms of amplitude > threshold → triggers |
| Threshold=0 chaos mode | Any non-zero sustained input → triggers (after cooldown) |
| Threshold=1 effectively disabled | Normal audio (-12dBFS) never triggers |
| Reset clears state | reset() → cooldown is 0, rmsPower is 0 |

### TestWetProcessor.cpp (~8 tests)

| Test | Verification |
|------|-------------|
| Soft-clip bounds | Input ±10.0 → output in (-1, 1) |
| Transparency at low levels | Input ±0.3 → output within 0.04dB |
| Auto-normalize target | Steady -20dBFS for 2 seconds → output RMS ≈ -6dBFS (±1dB) |
| Noise gate bypass | Input -70dBFS → gain stays at 1.0 |
| Gain clamp max | Very quiet input (-50dBFS) → gain ≤ 3.0 |
| Freeze transition fast release | notifyFreezeTransition() → gain settles within 200ms |
| No RMS drift | 10 seconds constant signal → gain stable (variance < 0.01) |
| Reset clears state | reset() → gain=1.0, rmsPower=0 |

### TestParameters.cpp (~8 tests)

| Test | Verification |
|------|-------------|
| Stretch log round-trip | normalized→stretch→normalized matches (±1e-5) for 0.0, 0.25, 0.5, 0.75, 1.0 |
| Stretch center | normalized=0.5 → stretch=1.0x |
| Stretch min | normalized=0.0 → stretch=0.1x |
| Stretch max | normalized=1.0 → stretch=10.0x |
| LFO rate log round-trip | Same pattern for 0.01Hz–20Hz |
| LFO rate center | normalized=0.5 → ~0.45Hz |
| LFO rate min | normalized=0.0 → 0.01Hz |
| LFO rate max | normalized=1.0 → 20Hz |

### TestPresets.cpp — Existing (637 assertions, no changes)

---

## Implementation Order

```
1. TimeStretch granular engine (Issue #7)
   - Grain struct, scheduler, Hann window, input buffer
   - Unfrozen mode: variable overlap-add with hop control
   - Frozen mode: pitch-shift with position scrub
   - Grain jitter ±64 samples
   - processSample() interface
   
2. Signal flow rewrite (Issue #2)
   - Rewrite processBlock to correct order
   - Add FreezeBuffer.write() unconditional call
   - Per-sample wet loop with correct chain
   
3. SmoothedValue fix + remove double smoothing (Issues #1, #3)
   - Rename constant to smoothingTimeSeconds = 0.02
   - Fix all module reset() calls
   - Remove 6 smoothers from PsycogAudioProcessor
   
4. Module fixes (Issues #4, #5, #6, #8, #10)
   - Wavefolder stereo offset
   - LFO inline advance(), remove buffer, fix S&H, random init
   - ThresholdDetector per-sample EMA redesign
   - FreezeBuffer per-buffer writePos, always-record, reset on swap
   
5. Infrastructure (Issues #9, #11, #13a-c)
   - PresetManager setValueNotifyingHost
   - Complete releaseResources
   - Remove processBlock(double)
   - Fix WetProcessor RMS to EMA + freeze-aware release
   - Set tail length to 4.0 seconds
   
6. Tests (Issue #12)
   - Write all ~89 tests across 7 files
   - Verify all pass
   - Run existing preset tests (confirm still passing)
```

---

## Files Modified

| File | Changes |
|------|---------|
| `src/utils/Constants.h` | Rename `smoothingMs` → `smoothingTimeSeconds = 0.02` |
| `src/modules/TimeStretch.h` | Grain struct, 4-grain state, processSample(), input buffer redesign |
| `src/modules/TimeStretch.cpp` | Full granular engine implementation, dual-mode (stretch/pitch), jitter |
| `src/modules/FreezeBuffer.h` | Per-buffer writePos, remove single writePos |
| `src/modules/FreezeBuffer.cpp` | Per-buffer writePos, reset on swap, always-record support |
| `src/modules/Wavefolder.h` | processSample() interface |
| `src/modules/Wavefolder.cpp` | Fix stereo offset, processSample(), remove block-level amplitude tracking |
| `src/modules/LFO.h` | Remove buffer/blockSize/process/getValue, add advance()/nextRandom() |
| `src/modules/LFO.cpp` | Inline computation, S&H wrap fix, random init, LCG |
| `src/modules/ThresholdDetector.h` | processSample() with EMA, remove block-level methods |
| `src/modules/ThresholdDetector.cpp` | 10ms EMA, integrated cooldown, per-sample interface |
| `src/modules/WetProcessor.h` | processSample(), notifyFreezeTransition(), remove rmsSum/rmsCount |
| `src/modules/WetProcessor.cpp` | EMA RMS, freeze-aware release, processSample() |
| `src/modules/Mixer.cpp` | Fix smoothing ramp time |
| `src/PluginProcessor.h` | Remove 6 smoothers, remove processBlock(double) declaration |
| `src/PluginProcessor.cpp` | Full processBlock rewrite, remove double override, fix releaseResources |
| `src/PresetManager.cpp` | Fix applyPreset() to use setValueNotifyingHost |
| `tests/TestTimeStretch.cpp` | 20 real tests replacing stub |
| `tests/TestFreezeBuffer.cpp` | 15 real tests replacing stub |
| `tests/TestWavefolder.cpp` | 15 real tests replacing stub |
| `tests/TestLFO.cpp` | 15 real tests replacing stub |
| `tests/TestParameters.cpp` | 8 real tests replacing stub |
| `tests/TestThresholdDetector.cpp` | 8 new tests (new file) |
| `tests/TestWetProcessor.cpp` | 8 new tests (new file) |

---

## Verification Criteria

After implementation, the following must be true:

1. `cmake --build build` succeeds with zero warnings (Release mode)
2. All ~89 new tests + 637 existing preset tests pass
3. Plugin loads in REAPER without crash
4. "Init" preset: audio passes through transparently (foldAmount=0, stretch=1x passthrough with grain windowing ≤ -60dB)
5. "Turning Through Time" preset: audible morphing, freeze cascade active
6. Parameter changes respond within ~20ms (not 20 seconds)
7. Freeze mode switching produces no clicks
8. Stereo input with offset > 0 produces stereo width (L ≠ R at wavefolder output)
9. Auto-freeze triggers on sustained loud wavefolder output, respects 100ms cooldown
10. DAW reports 4-second tail length
11. State save/load round-trips correctly (preset survives DAW project save/reload)
