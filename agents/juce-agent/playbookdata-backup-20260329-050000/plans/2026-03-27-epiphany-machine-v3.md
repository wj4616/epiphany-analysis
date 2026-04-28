# Epiphany Machine v3 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Upgrade the Epiphany Machine VST from v2 to v3 — 8-line FDN, wavefolder in feedback loop, expanded parameter ranges, gain staging overhaul, 18 quantum-physics presets, DRIVE knob, and version indicator.

**Architecture:** DSP modules (DiffusionNetwork, GlitchEngine, DualFilter) are rewritten first as self-contained units; PluginProcessor wires them together in Tasks 8–9; GUI is updated last in Task 10. Task 0 must run before everything else — it fixes audio-thread safety bugs that would cause silent failures or crashes during development.

**Tech Stack:** JUCE 7.0.12, C++17, CMake 3.22, VST3, Linux

---

## Task 0: Performance Audit Fixes

**MUST complete before all other tasks.**

Four bugs identified in the v2 codebase:

1. `static int lastPreset = 0` in `PluginProcessor.cpp:190` — static local is shared across plugin instances (all instances react to each other's preset changes).
2. `bool systemChanging` in `PresetManager.h:51` — plain `bool` written from the message thread, read from the audio thread (data race).
3. `GlitchEngine::process()` clears two 512KB `std::array` buffers (`bufL.fill(0)`, `bufR.fill(0)`) on the audio thread when silence is detected — causes dropouts.
4. `PresetManager::applyInterpolated()` calls `idx(id)` 17 times per block during morph; each call does an O(N) linear search with `getName()` string allocations — 425 string comparisons/block.

**Files:**
- Modify: `src/PluginProcessor.h`
- Modify: `src/PluginProcessor.cpp`
- Modify: `src/Presets/PresetManager.h`
- Modify: `src/Presets/PresetManager.cpp`
- Modify: `src/DSP/GlitchEngine.h`
- Modify: `src/DSP/GlitchEngine.cpp`

---

### Fix 1: `static int lastPreset` → instance member

- [ ] **Step 1: Add `lastPresetIdx` to `PluginProcessor.h`**

In `src/PluginProcessor.h`, add to the `private:` section after `bypassSmoothed`:

```cpp
    int lastPresetIdx = 0;
```

- [ ] **Step 2: Remove `static int lastPreset` from `PluginProcessor.cpp`**

In `src/PluginProcessor.cpp:190-194`, replace:

```cpp
    static int lastPreset = 0;
    if (presetIdx != lastPreset) {
        presetManager.selectPreset(presetIdx, morphTimeSec);
        lastPreset = presetIdx;
    }
```

with:

```cpp
    if (presetIdx != lastPresetIdx) {
        presetManager.selectPreset(presetIdx, morphTimeSec);
        lastPresetIdx = presetIdx;
    }
```

---

### Fix 2: `bool systemChanging` → `std::atomic<bool>`

- [ ] **Step 3: Update `PresetManager.h`**

At the top of `src/Presets/PresetManager.h`, after `#include <set>` add:

```cpp
#include <atomic>
#include <unordered_map>
```

Replace the `systemChanging` declaration (line 51):

```cpp
    bool systemChanging = false;
```

with:

```cpp
    std::atomic<bool> systemChanging{false};
    std::unordered_map<std::string, int> paramIndexCache;
```

- [ ] **Step 4: No changes needed to `PresetManager.cpp` for this fix** — `systemChanging = true` and `systemChanging = false` compile correctly with `std::atomic<bool>`.

---

### Fix 3: Remove silence-gate buffer clear from `GlitchEngine`

- [ ] **Step 5: Remove silence-gate members from `GlitchEngine.h`**

In `src/DSP/GlitchEngine.h`, remove the entire "Silence gate" block (lines 51–55):

```cpp
    // Silence gate — prevents stale-buffer grain playback when input is silent
    static constexpr float kSilenceThreshold = 0.0001f;  // ~-80dBFS
    float inputPeak    = 0.0f;   // slow-decay peak tracker
    int   silenceCount = 0;      // consecutive silent samples
    bool  isSilent     = false;
```

- [ ] **Step 6: Remove silence-gate logic from `GlitchEngine.cpp`**

In `src/DSP/GlitchEngine.cpp`, remove from `reset()` (lines 25–27):

```cpp
    inputPeak    = 0.0f;
    silenceCount = 0;
    isSilent     = false;
```

Remove the entire silence-gate block from `process()` (lines 56–76):

```cpp
        // ---- Silence gate ----
        // Track a slow-decay peak so brief inter-note gaps don't trigger the gate.
        // If the peak stays below -80dBFS for 50ms, mark as silent and clear the
        // buffer so grains can't play back stale audio from the previous note.
        const float absPeak = juce::jmax(std::abs(inL[i]), std::abs(inR[i]));
        inputPeak = juce::jmax(absPeak, inputPeak * 0.9999f);

        if (inputPeak < kSilenceThreshold)
        {
            const int silenceLimit = (int)(0.05 * sampleRate);  // 50ms
            if (++silenceCount >= silenceLimit && !isSilent)
            {
                isSilent = true;
                bufL.fill(0.0f);
                bufR.fill(0.0f);
            }
        }
        else
        {
            silenceCount = 0;
            isSilent     = false;
        }
```

Remove the silence gate output check (lines 126–130):

```cpp
        // Gate output: silent input must produce silent output — never play stale grains
        if (isSilent)
        {
            outL = outR = 0.0f;
            prevSampleL = prevSampleR = 0.0f;
        }
```

---

### Fix 4: Cache parameter indices in `PresetManager`

- [ ] **Step 7: Build index cache in `PresetManager::initialise()`**

In `src/Presets/PresetManager.cpp`, add this block at the end of `initialise()`, after `param->addListener(this)`:

```cpp
    // Build index cache — avoids O(N) string search in applyInterpolated()
    auto& params = apvts->processor.getParameters();
    for (int i = 0; i < (int)params.size(); ++i)
        paramIndexCache[params[i]->getName(256).toStdString()] = i;
```

- [ ] **Step 8: Replace `idx()` lambda in `applyInterpolated()`**

In `src/Presets/PresetManager.cpp`, inside `applyInterpolated()`, replace the two lines:

```cpp
    auto& params = apvts->processor.getParameters();

    // Helper to find parameter index by ID
    auto idx = [&](const char* id) -> int {
        for (int i = 0; i < params.size(); ++i)
            if (params[i]->getName(256) == apvts->getParameter(id)->getName(256))
                return i;
        return -1;
    };
```

with:

```cpp
    auto idx = [&](const char* id) -> int {
        auto* p = apvts->getParameter(id);
        if (!p) return -1;
        auto it = paramIndexCache.find(p->getName(256).toStdString());
        return it != paramIndexCache.end() ? it->second : -1;
    };
```

- [ ] **Step 9: Build and verify no regressions**

```bash
cd /home/myuser/EpiphanyMachine
cmake -B build -DCMAKE_BUILD_TYPE=Release 2>&1 | tail -5
cmake --build build -- -j$(nproc) 2>&1 | tail -10
```

Expected: `[100%] Linking CXX shared library ... EpiphanyMachine.vst3` with no errors.

- [ ] **Step 10: Commit**

```bash
git add src/PluginProcessor.h src/PluginProcessor.cpp \
        src/Presets/PresetManager.h src/Presets/PresetManager.cpp \
        src/DSP/GlitchEngine.h src/DSP/GlitchEngine.cpp
git commit -m "fix: audio thread safety — atomic systemChanging, static lastPreset, silence gate clear, param idx cache"
```

---

## Task 1: CMakeLists.txt Version + StereoWidener Removal

**Files:**
- Modify: `CMakeLists.txt`

- [ ] **Step 1: Update version and remove StereoWidener source**

In `CMakeLists.txt`, replace:

```cmake
project(EpiphanyMachine VERSION 2.0.0)
```

with:

```cmake
project(EpiphanyMachine VERSION 3.0.0)
```

Remove `src/DSP/StereoWidener.cpp` from the `target_sources` block:

```cmake
    src/DSP/StereoWidener.cpp
```

(Delete that line only; leave all others unchanged.)

- [ ] **Step 2: Commit**

```bash
git add CMakeLists.txt
git commit -m "build: bump version to 3.0.0, remove StereoWidener from sources"
```

---

## Task 2: DiffusionNetwork — 8-Line FDN Rewrite

**Changes:** 4 lines → 8 lines; 8×8 FHT; RT60 decay formula; per-line pan LFOs (replaces StereoWidener); remove 6dB output restriction; rename `setDecay` → `setDecaySeconds`.

**Files:**
- Modify: `src/DSP/DiffusionNetwork.h`
- Modify: `src/DSP/DiffusionNetwork.cpp`

- [ ] **Step 1: Rewrite `DiffusionNetwork.h`**

Replace the entire contents of `src/DSP/DiffusionNetwork.h`:

```cpp
#pragma once
#include <JuceHeader.h>

//==============================================================================
// DiffusionNetwork — 8-line FDN reverb.
// v3: 8 lines, 8x8 FHT, RT60 decay, per-line pan LFOs, setDecaySeconds API.
//==============================================================================
class DiffusionNetwork
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);  // in-place stereo
    void reset();

    void setDecaySeconds(float s)  { decaySeconds = std::max(0.01f, s); }
    void setDamping(float d)       { dampingParam = d; }
    void setWidth(float w)         { widthParam = juce::jlimit(0.0f, 1.0f, w); }
    void setFreezeActive(bool on);

private:
    static constexpr int kNumLines = 8;
    static constexpr int kBufSize  = 4096;
    static constexpr int kBufMask  = kBufSize - 1;
    static constexpr int kBaseLengths[kNumLines] = {1087,1283,1637,1933,2311,2719,3109,3511};
    static constexpr float kLfoRates[kNumLines]  = {0.07f,0.11f,0.15f,0.19f,0.23f,0.29f,0.31f,0.37f};
    static constexpr float kBasePan[kNumLines]   = {-0.8f,0.8f,-0.4f,0.4f,-0.6f,0.6f,-0.2f,0.2f};

    int delayLengths[kNumLines] = {};
    float bufL[kNumLines][kBufSize] = {};
    float bufR[kNumLines][kBufSize] = {};
    int   writeIndex[kNumLines] = {};
    float lpfStateL[kNumLines]  = {};
    float lpfStateR[kNumLines]  = {};
    float lfoPhase[kNumLines]   = {};

    float decaySeconds = 6.0f;
    float dampingParam = 0.5f;
    float widthParam   = 0.6f;
    double sampleRate  = 48000.0;

    juce::SmoothedValue<float> freezeInputGain;
    juce::SmoothedValue<float> freezeFeedbackBoost;
    juce::SmoothedValue<float> freezeDampingMute;
    bool isFrozen = false;

    void hadamardMix(float v[kNumLines]) const;
};
```

- [ ] **Step 2: Rewrite `DiffusionNetwork.cpp`**

Replace the entire contents of `src/DSP/DiffusionNetwork.cpp`:

```cpp
#include "DiffusionNetwork.h"

void DiffusionNetwork::setFreezeActive(bool on)
{
    isFrozen = on;
    freezeInputGain.setTargetValue(on ? 0.0f : 1.0f);
    freezeFeedbackBoost.setTargetValue(on ? 1.0f : 0.0f);
    freezeDampingMute.setTargetValue(on ? 1.0f : 0.0f);
}

void DiffusionNetwork::prepare(double sr, int)
{
    sampleRate = sr;
    for (int i = 0; i < kNumLines; ++i)
    {
        delayLengths[i] = (int)(kBaseLengths[i] * (sr / 48000.0));
        lfoPhase[i]     = (float)i / (float)kNumLines;
    }
    const double freezeRamp = 2048.0 / sr;
    freezeInputGain.reset(sr, freezeRamp);     freezeInputGain.setCurrentAndTargetValue(1.0f);
    freezeFeedbackBoost.reset(sr, freezeRamp); freezeFeedbackBoost.setCurrentAndTargetValue(0.0f);
    freezeDampingMute.reset(sr, freezeRamp);   freezeDampingMute.setCurrentAndTargetValue(0.0f);
    reset();
}

void DiffusionNetwork::reset()
{
    for (int i = 0; i < kNumLines; ++i)
    {
        std::fill(std::begin(bufL[i]), std::end(bufL[i]), 0.0f);
        std::fill(std::begin(bufR[i]), std::end(bufR[i]), 0.0f);
        writeIndex[i] = lpfStateL[i] = lpfStateR[i] = 0;
    }
}

// 8-element Fast Hadamard Transform — 3 butterfly stages, normalised by 1/sqrt(8).
void DiffusionNetwork::hadamardMix(float v[kNumLines]) const
{
    for (int stride = 1; stride < kNumLines; stride <<= 1)
        for (int i = 0; i < kNumLines; i += stride * 2)
            for (int j = 0; j < stride; ++j)
            {
                float a = v[i+j], b = v[i+j+stride];
                v[i+j] = a + b; v[i+j+stride] = a - b;
            }
    for (int i = 0; i < kNumLines; ++i) v[i] *= 0.3536f;  // 1/sqrt(8)
}

void DiffusionNetwork::process(juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    auto* inL = buffer.getWritePointer(0);
    auto* inR = buffer.getWritePointer(1);

    const float damping = dampingParam * 0.9f;
    const float twoPi   = juce::MathConstants<float>::twoPi;

    for (int i = 0; i < numSamples; ++i)
    {
        const float inputGain  = freezeInputGain.getNextValue();
        const float fbBoost    = freezeFeedbackBoost.getNextValue();
        const float dampMute   = freezeDampingMute.getNextValue();
        const float activeDamp = damping * (1.0f - dampMute);

        float readL[kNumLines], readR[kNumLines];

        for (int line = 0; line < kNumLines; ++line)
        {
            // RT60 feedback gain: amplitude decays to -60dB in decaySeconds
            const float normalGain = std::pow(10.0f,
                -3.0f * (float)delayLengths[line] / (decaySeconds * (float)sampleRate));
            const float fbGain = normalGain * (1.0f - fbBoost) + 0.999f * fbBoost;

            int readIdx = (writeIndex[line] - delayLengths[line] + kBufSize) & kBufMask;
            readL[line] = bufL[line][readIdx];
            readR[line] = bufR[line][readIdx];

            float fL = readL[line] * fbGain;
            float fR = readR[line] * fbGain;
            lpfStateL[line] = (1.0f - activeDamp) * fL + activeDamp * lpfStateL[line] + 1e-18f;
            lpfStateR[line] = (1.0f - activeDamp) * fR + activeDamp * lpfStateR[line] + 1e-18f;

            bufL[line][writeIndex[line]] = inL[i] * inputGain + lpfStateL[line];
            bufR[line][writeIndex[line]] = inR[i] * inputGain + lpfStateR[line];
            writeIndex[line] = (writeIndex[line] + 1) & kBufMask;

            lfoPhase[line] += kLfoRates[line] / (float)sampleRate;
            if (lfoPhase[line] >= 1.0f) lfoPhase[line] -= 1.0f;
        }

        hadamardMix(readL);
        hadamardMix(readR);

        // Pan each line according to base position + slow LFO, scaled by width
        float outL = 0.0f, outR = 0.0f;
        for (int line = 0; line < kNumLines; ++line)
        {
            float panLfo = 0.05f * std::sin(lfoPhase[line] * twoPi);
            float pan    = juce::jlimit(-1.0f, 1.0f, kBasePan[line] * widthParam + panLfo);
            float sig    = (readL[line] + readR[line]) * 0.5f;
            outL += sig * 0.5f * (1.0f - pan);
            outR += sig * 0.5f * (1.0f + pan);
        }
        inL[i] = outL * (2.0f / (float)kNumLines);
        inR[i] = outR * (2.0f / (float)kNumLines);
    }
}
```

- [ ] **Step 3: Build to verify compilation**

```bash
cd /home/myuser/EpiphanyMachine && cmake --build build -- -j$(nproc) 2>&1 | grep "error:" | head -10
```

Expected: only PluginProcessor.cpp errors (still calls old API — fixed in Task 8).

- [ ] **Step 4: Commit**

```bash
git add src/DSP/DiffusionNetwork.h src/DSP/DiffusionNetwork.cpp
git commit -m "feat: DiffusionNetwork v3 — 8-line FDN, FHT, RT60 decay, per-line pan LFOs"
```

---

## Task 3: GlitchEngine — Drift, Adaptive Envelopes, Chaos Expansion

**Changes:** Add `drift` param; replace crossfade counter with adaptive window (Hann/Tukey/Trapezoid); expand grain length range; expand chaos behavior (add double-speed); float playhead for fractional interpolation.

**Files:**
- Modify: `src/DSP/GlitchEngine.h`
- Modify: `src/DSP/GlitchEngine.cpp`

- [ ] **Step 1: Rewrite `GlitchEngine.h`**

Replace entire file:

```cpp
#pragma once
#include <JuceHeader.h>

class GlitchEngine
{
public:
    GlitchEngine();
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setDensity(float d) { density = d; }
    void setChaos(float c)   { chaos = c; }
    void setDrift(float d)   { drift = d; }

private:
    static constexpr int kBufSize = 131072;
    static constexpr int kBufMask = kBufSize - 1;

    std::array<float, kBufSize> bufL, bufR;

    int   writeHead        = 0;
    int   grainSamplesLeft = 0;
    int   grainStart       = 0;
    int   grainLength      = 0;
    float grainLengthMs    = 100.0f;
    bool  grainReverse     = false;
    float grainSpeed       = 1.0f;

    float  density    = 0.3f;
    float  chaos      = 0.2f;
    float  drift      = 0.0f;
    double sampleRate = 48000.0;

    juce::Random rng;

    void  triggerNewGrain();
    float getWindowValue(int samplePos, int grainLenSamples, float grainMs) const;
};
```

- [ ] **Step 2: Rewrite `GlitchEngine.cpp`**

Replace entire file:

```cpp
#include "GlitchEngine.h"

GlitchEngine::GlitchEngine() { bufL.fill(0.0f); bufR.fill(0.0f); }

void GlitchEngine::prepare(double sr, int) { sampleRate = sr; reset(); }

void GlitchEngine::reset()
{
    bufL.fill(0.0f); bufR.fill(0.0f);
    writeHead = grainSamplesLeft = grainStart = grainLength = 0;
    grainLengthMs = 100.0f; grainReverse = false; grainSpeed = 1.0f;
}

float GlitchEngine::getWindowValue(int samplePos, int grainLenSamples, float grainMs) const
{
    float t = juce::jlimit(0.0f, 1.0f,
        (float)samplePos / (float)std::max(grainLenSamples - 1, 1));

    if (grainMs > 50.0f)
    {
        float s = std::sin(t * juce::MathConstants<float>::pi);
        return s * s;  // Hann
    }
    else if (grainMs >= 10.0f)
    {
        const float half = 0.15f;  // Tukey alpha=0.3, half=0.15
        if (t < half)
            return 0.5f * (1.0f - std::cos(t / half * juce::MathConstants<float>::pi));
        else if (t > 1.0f - half)
            return 0.5f * (1.0f - std::cos((1.0f - t) / half * juce::MathConstants<float>::pi));
        return 1.0f;
    }
    else
    {
        // Trapezoid: 0.5ms ramps
        const float rampT = (0.5e-3f * (float)sampleRate) / (float)std::max(grainLenSamples, 1);
        if (t < rampT)            return t / rampT;
        else if (t > 1.0f - rampT) return (1.0f - t) / rampT;
        return 1.0f;
    }
}

void GlitchEngine::process(juce::AudioBuffer<float>& buffer)
{
    const int N  = buffer.getNumSamples();
    auto* inL = buffer.getWritePointer(0);
    auto* inR = buffer.getWritePointer(1);

    for (int i = 0; i < N; ++i)
    {
        bufL[writeHead & kBufMask] = inL[i];
        bufR[writeHead & kBufMask] = inR[i];
        ++writeHead;

        if (grainSamplesLeft <= 0) triggerNewGrain();

        const int consumed = grainLength - grainSamplesLeft;
        const float readPos = grainReverse
            ? (float)(grainStart + grainLength - consumed)
            : (float)grainStart + (float)consumed * grainSpeed;

        const int   idx0 = (int)readPos & kBufMask;
        const int   idx1 = (idx0 + 1) & kBufMask;
        const float frac = readPos - std::floor(readPos);
        float outL = bufL[idx0] * (1.0f - frac) + bufL[idx1] * frac;
        float outR = bufR[idx0] * (1.0f - frac) + bufR[idx1] * frac;

        const float win = getWindowValue(consumed, grainLength, grainLengthMs);
        inL[i] = outL * win;
        inR[i] = outR * win;
        --grainSamplesLeft;
    }
}

void GlitchEngine::triggerNewGrain()
{
    const float maxMs = juce::jmap(density, 0.0f, 1.0f, 1000.0f, 50.0f);
    const float minMs = juce::jmap(density, 0.0f, 1.0f,  500.0f,  1.0f);
    grainLengthMs    = minMs + rng.nextFloat() * (maxMs - minMs);
    grainLength      = juce::jmax(1, (int)(grainLengthMs * 0.001f * (float)sampleRate));
    grainSamplesLeft = grainLength;

    const int recentWindow = (int)(sampleRate * 0.2);
    if (chaos > 0.7f && rng.nextFloat() < (chaos - 0.7f) / 0.3f)
        grainStart = rng.nextInt(kBufSize);
    else
    {
        int offset = (int)(rng.nextFloat() * chaos * recentWindow);
        grainStart = (writeHead - grainLength - offset + kBufSize) & kBufMask;
    }

    grainReverse = rng.nextFloat() < chaos * 0.6f;
    grainSpeed   = 1.0f;
    if (!grainReverse)
    {
        float r = rng.nextFloat();
        if      (r < chaos * 0.4f) grainSpeed = 0.5f;
        else if (r < chaos * 0.6f) grainSpeed = 2.0f;
    }

    if (drift > 0.0f)
    {
        float randomSt = (rng.nextFloat() * 2.0f - 1.0f) * drift;
        grainSpeed *= std::pow(2.0f, randomSt / 12.0f);
    }
}
```

- [ ] **Step 3: Commit**

```bash
git add src/DSP/GlitchEngine.h src/DSP/GlitchEngine.cpp
git commit -m "feat: GlitchEngine v3 — drift, adaptive windowing, float playhead, expanded chaos"
```

---

## Task 4: DualFilter — Q Expansion + LFO Depth

**Two-line change only.**

**Files:**
- Modify: `src/DSP/DualFilter.cpp`

- [ ] **Step 1: Expand Q mapping from 2–30 to 0.4–50**

In `src/DSP/DualFilter.cpp` replace:

```cpp
        float texK  = 1.0f / (0.5f + texReso * 14.5f);  // Q from resonance param
```

with:

```cpp
        float texK  = 1.0f / (0.5f + texReso * 24.5f);  // Q 0.4–50, self-oscillating above ~40
```

Replace:

```cpp
        float bodK  = 1.0f / (0.5f + bodReso * 14.5f);
```

with:

```cpp
        float bodK  = 1.0f / (0.5f + bodReso * 24.5f);
```

- [ ] **Step 2: Expand LFO depth ±2 oct → ±3 oct**

Replace:

```cpp
        float modCut = texCutoff * std::pow(2.0f, lfoVal * lfoDepth * 2.0f);
```

with:

```cpp
        float modCut = texCutoff * std::pow(2.0f, lfoVal * lfoDepth * 3.0f);
```

- [ ] **Step 3: Commit**

```bash
git add src/DSP/DualFilter.cpp
git commit -m "feat: DualFilter — Q range 0.4-50 (self-oscillating), LFO depth ±3 oct"
```

---

## Task 5: Delete StereoWidener Files

**Files:**
- Delete: `src/DSP/StereoWidener.h`
- Delete: `src/DSP/StereoWidener.cpp`

- [ ] **Step 1: Delete files**

```bash
rm src/DSP/StereoWidener.h src/DSP/StereoWidener.cpp
```

- [ ] **Step 2: Commit**

```bash
git add -u src/DSP/StereoWidener.h src/DSP/StereoWidener.cpp
git commit -m "chore: delete StereoWidener (replaced by per-line pan in DiffusionNetwork)"
```

---

## Task 6: PresetData — 18 Quantum Physics Presets

**Changes:** Add `drive` field to struct; replace 34 presets with 18 quantum physics themed presets; update `kNumPresets`.

**Files:**
- Modify: `src/Presets/PresetData.h`

- [ ] **Step 1: Rewrite `PresetData.h`**

Replace the entire contents of `src/Presets/PresetData.h`:

```cpp
#pragma once
#include <JuceHeader.h>

//==============================================================================
// PresetData — v3 adds float drive field; decay is now seconds (0-20).
//==============================================================================
struct PresetData
{
    juce::String name;
    float morph, mix, density, chaos, decay, damping, drift, width, feedback, drive;
    float glitchFocus, sensitivity;
    int   filterPos;        // 0=Off 1=Wet 2=Final 3=Feedback
    int   texFilterType;    // 0=LP 1=HP 2=BP
    float texFilterCutoff, texFilterReso, texLfoRate, texLfoDepth;
    int   bodyFilterType;
    float bodyFilterCutoff, bodyFilterReso;
};

// Each row: name, morph,mix,density,chaos, decay(s),damping, drift(st),width,feedback, drive,
//           glitchFocus,sensitivity, filterPos,
//           texType,texCut,texReso,texLfoRate,texLfoDepth,
//           bodyType,bodyCut,bodyReso
inline const PresetData kFactoryPresets[] =
{
    { "INIT",
      0.3f,0.5f,0.3f,0.2f, 4.0f,0.5f, 0.0f,0.6f,0.3f, 0.0f,
      0.5f,0.5f, 0, 0,2000.0f,0.3f,0.5f,0.0f, 0,1200.0f,0.2f },

    { "Quantum Foam",
      0.3f,0.65f,0.6f,0.4f, 8.0f,0.6f, 4.0f,0.7f,0.4f, 0.1f,
      0.7f,0.6f, 1, 1,3000.0f,0.2f,0.3f,0.2f, 0,800.0f,0.1f },

    { "Superposition",
      0.5f,0.6f,0.3f,0.2f, 12.0f,0.3f, 12.0f,0.9f,0.6f, 0.3f,
      0.5f,0.5f, 1, 0,4000.0f,0.15f,0.05f,0.25f, 0,3000.0f,0.1f },

    { "Wave Collapse",
      0.8f,0.55f,0.7f,0.7f, 5.0f,0.4f, 8.0f,0.8f,0.55f, 0.5f,
      0.9f,0.7f, 2, 2,2500.0f,0.6f,2.0f,0.5f, 0,1500.0f,0.2f },

    { "Entanglement",
      0.4f,0.7f,0.2f,0.3f, 15.0f,0.5f, 7.0f,1.0f,0.7f, 0.2f,
      0.5f,0.5f, 1, 0,2000.0f,0.2f,0.1f,0.3f, 0,2000.0f,0.15f },

    { "Heisenberg Blur",
      0.6f,0.5f,0.5f,0.9f, 6.0f,0.3f, 16.0f,0.8f,0.5f, 0.6f,
      0.8f,0.7f, 1, 2,3000.0f,0.5f,3.0f,0.4f, 0,2000.0f,0.2f },

    { "Planck Wall",
      0.9f,0.45f,0.9f,0.5f, 2.0f,0.1f, 0.0f,0.5f,0.4f, 0.8f,
      1.0f,0.9f, 1, 1,5000.0f,0.4f,5.0f,0.3f, 1,800.0f,0.2f },

    { "Bose-Einstein",
      0.15f,0.75f,0.15f,0.1f, 18.0f,0.2f, 24.0f,0.85f,0.5f, 0.0f,
      0.3f,0.4f, 1, 1,2000.0f,0.1f,0.04f,0.2f, 0,4000.0f,0.05f },

    { "Hawking Radiation",
      0.2f,0.7f,0.2f,0.4f, 16.0f,0.7f, 12.0f,0.75f,0.8f, 0.4f,
      0.4f,0.5f, 1, 0,1000.0f,0.2f,0.03f,0.3f, 0,600.0f,0.2f },

    { "Dark Matter",
      0.1f,0.8f,0.15f,0.2f, 18.0f,0.9f, 0.0f,0.4f,0.6f, 0.0f,
      0.0f,0.3f, 2, 0,400.0f,0.3f,0.02f,0.2f, 0,250.0f,0.35f },

    { "Antimatter",
      0.7f,0.5f,0.6f,0.8f, 4.0f,0.4f, 0.0f,0.9f,0.6f, 0.7f,
      0.8f,0.7f, 3, 0,6000.0f,0.15f,0.1f,0.1f, 1,3000.0f,0.1f },

    { "String Theory",
      0.3f,0.6f,0.4f,0.15f, 10.0f,0.2f, 7.0f,0.7f,0.5f, 0.3f,
      0.5f,0.6f, 1, 2,3500.0f,0.8f,0.1f,0.3f, 2,1500.0f,0.5f },

    { "Quantum Tunneling",
      0.75f,0.55f,0.8f,0.6f, 3.0f,0.3f, 6.0f,0.7f,0.45f, 0.4f,
      1.0f,0.8f, 3, 1,4000.0f,0.3f,0.5f,0.3f, 0,2500.0f,0.1f },

    { "Decoherence",
      0.65f,0.5f,0.65f,0.75f, 5.0f,0.5f, 10.0f,0.8f,0.55f, 0.5f,
      0.7f,0.7f, 1, 2,2000.0f,0.4f,1.5f,0.4f, 0,1800.0f,0.15f },

    { "Zero Point Energy",
      0.45f,0.6f,0.55f,0.5f, 8.0f,0.4f, 5.0f,0.65f,0.5f, 0.2f,
      0.6f,0.6f, 1, 0,2500.0f,0.25f,0.2f,0.2f, 0,1500.0f,0.15f },

    { "Singularity",
      0.5f,0.7f,0.4f,0.6f, 20.0f,0.4f, 20.0f,0.6f,0.9f, 0.9f,
      0.5f,0.5f, 3, 0,8000.0f,0.15f,0.05f,0.1f, 0,3000.0f,0.1f },

    { "Multiverse",
      0.85f,0.6f,0.75f,0.95f, 6.0f,0.3f, 12.0f,1.0f,0.7f, 0.7f,
      1.0f,0.9f, 1, 2,3000.0f,0.6f,4.0f,0.5f, 1,1500.0f,0.3f },

    { "Schrodinger",
      0.5f,0.55f,0.5f,0.5f, 10.0f,0.5f, 12.0f,0.75f,0.5f, 0.5f,
      0.5f,0.5f, 1, 2,2500.0f,0.4f,0.25f,0.3f, 0,1600.0f,0.2f },
};

inline constexpr int kNumPresets = 18;
```

- [ ] **Step 2: Commit**

```bash
git add src/Presets/PresetData.h
git commit -m "feat: PresetData v3 — add drive field, 18 quantum physics presets, kNumPresets=18"
```

---

## Task 7: PresetManager — Drive Parameter + Atomic Already Done

**Task 0 already fixed `systemChanging` and `paramIndexCache`. This task adds `drive` to capture/apply and updates the kNumPresets bounds check.**

**Files:**
- Modify: `src/Presets/PresetManager.cpp`

- [ ] **Step 1: Add `drive` to `captureCurrentValues()`**

In `src/Presets/PresetManager.cpp`, in `captureCurrentValues()`, replace the return statement:

```cpp
    return {
        "",
        get("morph"),    get("mix"),      get("density"), get("chaos"),
        get("decay"),    get("damping"),  get("drift"),   get("width"),   get("feedback"),
        get("glitchFocus"), get("sensitivity"),
        getI("filterPos"),
        getI("texFilterType"), get("texFilterCutoff"), get("texFilterReso"),
        get("texLfoRate"), get("texLfoDepth"),
        getI("bodyFilterType"), get("bodyFilterCutoff"), get("bodyFilterReso")
    };
```

with:

```cpp
    return {
        "",
        get("morph"),    get("mix"),      get("density"), get("chaos"),
        get("decay"),    get("damping"),  get("drift"),   get("width"),   get("feedback"),
        get("drive"),
        get("glitchFocus"), get("sensitivity"),
        getI("filterPos"),
        getI("texFilterType"), get("texFilterCutoff"), get("texFilterReso"),
        get("texLfoRate"), get("texLfoDepth"),
        getI("bodyFilterType"), get("bodyFilterCutoff"), get("bodyFilterReso")
    };
```

- [ ] **Step 2: Add `drive` to `applyInterpolated()`**

In `src/Presets/PresetManager.cpp`, in `applyInterpolated()`, after the line:

```cpp
    WF("feedback", s.feedback, e.feedback);
```

add:

```cpp
    WF("drive",    s.drive,    e.drive);
```

- [ ] **Step 3: Commit**

```bash
git add src/Presets/PresetManager.cpp
git commit -m "feat: PresetManager — morph drive parameter through preset transitions"
```

---

## Task 8: PluginProcessor.h — Member Updates

**Remove StereoWidener; remove single-sample feedback scalars; add feedbackBuffer, driveSmoothed.**

**Files:**
- Modify: `src/PluginProcessor.h`

- [ ] **Step 1: Remove StereoWidener include + member**

Remove from `src/PluginProcessor.h`:

```cpp
#include "DSP/StereoWidener.h"
```

In the `private:` DSP members section, remove:

```cpp
    StereoWidener    stereoWidener;
```

- [ ] **Step 2: Remove single-sample feedback scalars and fbBuf**

Remove from `private:`:

```cpp
    // Feedback carry-over between blocks
    float feedbackL = 0.0f, feedbackR = 0.0f;
    float fbLpfStateL = 0.0f, fbLpfStateR = 0.0f, fbLpfCoeff = 0.0f;
```

Remove from working buffers section:

```cpp
    juce::AudioBuffer<float> fbBuf;  // feedback pitch-drift buffer — pre-allocated, never heap in processBlock
```

- [ ] **Step 3: Add feedbackBuffer and driveSmoothed**

In the "Working buffers" section after `processedBuffer`, add:

```cpp
    juce::AudioBuffer<float> feedbackBuffer;  // full-block feedback carry from previous block
```

In the "SmoothedValues" section after `bypassSmoothed`, add:

```cpp
    juce::SmoothedValue<float> driveSmoothed;
```

- [ ] **Step 4: Commit**

```bash
git add src/PluginProcessor.h
git commit -m "refactor: PluginProcessor.h — remove StereoWidener, add feedbackBuffer + driveSmoothed"
```

---

## Task 9: PluginProcessor.cpp — Parameters + processBlock Rewrite

**Changes:** New parameter ranges (drift 0–24, decay 0–20s, feedback 0–0.95), add drive 0–1, filterPos 4 options, morphTime 0.1–30, 18 preset names; full-buffer feedback path; wavefolder in feedback; tanh output limiter; filterPos=3 handling.**

**Files:**
- Modify: `src/PluginProcessor.cpp`

- [ ] **Step 1: Update `createParameterLayout()`**

In `src/PluginProcessor.cpp`, replace the parameter layout function body (lines 7–72) with:

```cpp
juce::AudioProcessorValueTreeState::ParameterLayout
EpiphanyMachineProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto addFloat = [&](const char* id, const char* name,
                        float mn, float mx, float def, float skew = 1.0f)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            id, name, juce::NormalisableRange<float>(mn, mx, 0.001f, skew), def));
    };
    auto addChoice = [&](const char* id, const char* name,
                         juce::StringArray choices, int def = 0)
    {
        params.push_back(std::make_unique<juce::AudioParameterChoice>(id, name, choices, def));
    };
    auto addBool = [&](const char* id, const char* name, bool def = false)
    {
        params.push_back(std::make_unique<juce::AudioParameterBool>(id, name, def));
    };

    addFloat("morph",    "Morph",    0.0f,  1.0f,   0.3f);
    addFloat("mix",      "Mix",      0.0f,  1.0f,   0.5f);
    addFloat("density",  "Density",  0.0f,  1.0f,   0.3f);
    addFloat("chaos",    "Chaos",    0.0f,  1.0f,   0.2f);
    addFloat("decay",    "Decay",    0.0f,  20.0f,  4.0f,  0.5f);  // seconds
    addFloat("damping",  "Damping",  0.0f,  1.0f,   0.5f);
    addFloat("drift",    "Drift",    0.0f,  24.0f,  0.0f);          // semitones up
    addFloat("width",    "Width",    0.0f,  1.0f,   0.6f);
    addFloat("feedback", "Feedback", 0.0f,  0.95f,  0.3f,  0.5f);
    addFloat("drive",    "Drive",    0.0f,  1.0f,   0.0f);

    addBool("bypass", "Bypass");
    addBool("freeze", "Freeze");
    addFloat("glitchFocus",  "Glitch Focus",  0.0f, 1.0f, 0.5f);
    addFloat("sensitivity",  "Sensitivity",   0.0f, 1.0f, 0.5f);

    addChoice("filterPos",    "Filter Position", {"Off","Wet","Final","Feedback"}, 0);
    addChoice("texFilterType","Tex Filter Type",  {"LP","HP","BP"}, 0);
    addFloat("texFilterCutoff", "Tex Cutoff",   20.0f, 20000.0f, 2000.0f, 0.3f);
    addFloat("texFilterReso",   "Tex Reso",     0.0f,  1.0f,     0.3f);
    addFloat("texLfoRate",      "Tex LFO Rate", 0.01f, 10.0f,    0.5f,  0.3f);
    addFloat("texLfoDepth",     "Tex LFO Depth",0.0f,  1.0f,     0.0f);
    addChoice("bodyFilterType","Body Filter Type", {"LP","HP","BP"}, 0);
    addFloat("bodyFilterCutoff","Body Cutoff",  20.0f, 20000.0f, 1200.0f, 0.3f);
    addFloat("bodyFilterReso",  "Body Reso",    0.0f,  1.0f,     0.2f);

    addChoice("preset", "Preset",
              {"INIT","Quantum Foam","Superposition","Wave Collapse","Entanglement",
               "Heisenberg Blur","Planck Wall","Bose-Einstein","Hawking Radiation",
               "Dark Matter","Antimatter","String Theory","Quantum Tunneling",
               "Decoherence","Zero Point Energy","Singularity","Multiverse","Schrodinger"}, 0);
    addFloat("morphTime", "Morph Time", 0.1f, 30.0f, 2.0f);

    return { params.begin(), params.end() };
}
```

- [ ] **Step 2: Update `prepareToPlay()`**

Replace the entire `prepareToPlay()` function in `src/PluginProcessor.cpp`:

```cpp
void EpiphanyMachineProcessor::prepareToPlay(double sr, int spb)
{
    glitchEngine.prepare(sr, spb);
    diffusionNetwork.prepare(sr, spb);
    pitchDrifter.prepare(sr, spb);
    inputDetector.prepare(sr, spb);
    filterDetector.prepare(sr, spb);
    dualFilter.prepare(sr, spb);

    dryBuffer.setSize(2, spb);
    glitchInputBuffer.setSize(2, spb);
    cleanPassBuffer.setSize(2, spb);
    wetBuffer.setSize(2, spb);
    filteredBuffer.setSize(2, spb);
    processedBuffer.setSize(2, spb);
    feedbackBuffer.setSize(2, spb);
    feedbackBuffer.clear();

    const double sm = 0.02;
    morphSmoothed.reset(sr, sm);
    mixSmoothed.reset(sr, sm);
    feedbackSmoothed.reset(sr, sm);
    glitchFocusSmoothed.reset(sr, sm);
    driveSmoothed.reset(sr, sm);
    bypassSmoothed.reset(sr, 512.0 / sr);

    auto getV = [&](const char* id) { return apvts.getRawParameterValue(id)->load(); };
    morphSmoothed.setCurrentAndTargetValue(getV("morph"));
    mixSmoothed.setCurrentAndTargetValue(getV("mix"));
    feedbackSmoothed.setCurrentAndTargetValue(getV("feedback"));
    glitchFocusSmoothed.setCurrentAndTargetValue(getV("glitchFocus"));
    driveSmoothed.setCurrentAndTargetValue(getV("drive"));
    bypassSmoothed.setCurrentAndTargetValue(getV("bypass") > 0.5f ? 1.0f : 0.0f);
}
```

- [ ] **Step 3: Rewrite `processBlock()`**

Replace the entire `processBlock()` function:

```cpp
void EpiphanyMachineProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int N = buffer.getNumSamples();

    if (buffer.getNumChannels() == 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, N);

    dryBuffer.copyFrom(0, 0, buffer, 0, 0, N);
    dryBuffer.copyFrom(1, 0, buffer, 1, 0, N);

    auto getV = [&](const char* id) { return apvts.getRawParameterValue(id)->load(); };

    const bool  isBypassed   = getV("bypass") > 0.5f;
    const bool  isFrozen     = getV("freeze") > 0.5f;
    const int   filterPos    = (int)getV("filterPos");
    const int   presetIdx    = (int)getV("preset");
    const float morphTimeSec = getV("morphTime");

    bypassSmoothed.setTargetValue(isBypassed ? 1.0f : 0.0f);
    morphSmoothed.setTargetValue(getV("morph"));
    mixSmoothed.setTargetValue(getV("mix"));
    feedbackSmoothed.setTargetValue(getV("feedback"));
    glitchFocusSmoothed.setTargetValue(getV("glitchFocus"));
    driveSmoothed.setTargetValue(getV("drive"));

    glitchEngine.setDensity(getV("density"));
    glitchEngine.setChaos(getV("chaos"));
    glitchEngine.setDrift(getV("drift"));
    diffusionNetwork.setDecaySeconds(getV("decay"));
    diffusionNetwork.setDamping(getV("damping"));
    diffusionNetwork.setWidth(getV("width"));
    diffusionNetwork.setFreezeActive(isFrozen);
    pitchDrifter.setDrift(getV("drift"));
    inputDetector.setSensitivity(getV("sensitivity"));
    filterDetector.setSensitivity(getV("sensitivity"));
    dualFilter.setTexFilterType((int)getV("texFilterType"));
    dualFilter.setTexCutoff(getV("texFilterCutoff"));
    dualFilter.setTexReso(getV("texFilterReso"));
    dualFilter.setTexLfoRate(getV("texLfoRate"));
    dualFilter.setTexLfoDepth(getV("texLfoDepth"));
    dualFilter.setBodyFilterType((int)getV("bodyFilterType"));
    dualFilter.setBodyCutoff(getV("bodyFilterCutoff"));
    dualFilter.setBodyReso(getV("bodyFilterReso"));

    if (presetIdx != lastPresetIdx) {
        presetManager.selectPreset(presetIdx, morphTimeSec);
        lastPresetIdx = presetIdx;
    }
    presetManager.advance(getSampleRate(), N);

    //--- Fast bypass ---
    if (bypassSmoothed.getCurrentValue() >= 0.999f && isBypassed)
    {
        for (int i = 0; i < N; ++i) {
            morphSmoothed.getNextValue(); mixSmoothed.getNextValue();
            feedbackSmoothed.getNextValue(); glitchFocusSmoothed.getNextValue();
            driveSmoothed.getNextValue(); bypassSmoothed.getNextValue();
        }
        buffer.copyFrom(0, 0, dryBuffer, 0, 0, N);
        buffer.copyFrom(1, 0, dryBuffer, 1, 0, N);
        return;
    }

    //--- Step 1: Input detect ---
    inputDetector.process(buffer, N);
    const auto& texBuf = inputDetector.getTextureBuffer();
    const auto& bodBuf = inputDetector.getBodyBuffer();

    //--- Step 2: Glitch focus routing ---
    {
        auto* giL = glitchInputBuffer.getWritePointer(0);
        auto* giR = glitchInputBuffer.getWritePointer(1);
        auto* cpL = cleanPassBuffer.getWritePointer(0);
        auto* cpR = cleanPassBuffer.getWritePointer(1);
        const auto* inL = buffer.getReadPointer(0);
        const auto* inR = buffer.getReadPointer(1);
        const auto* tL  = texBuf.getReadPointer(0);
        const auto* tR  = texBuf.getReadPointer(1);
        const auto* bL  = bodBuf.getReadPointer(0);
        const auto* bR  = bodBuf.getReadPointer(1);
        for (int i = 0; i < N; ++i)
        {
            const float gf = glitchFocusSmoothed.getNextValue();
            if (gf <= 0.5f) {
                float t = gf * 2.0f;
                giL[i] = bL[i]*(1.0f-t) + inL[i]*t; giR[i] = bR[i]*(1.0f-t) + inR[i]*t;
                cpL[i] = tL[i]*(1.0f-t);             cpR[i] = tR[i]*(1.0f-t);
            } else {
                float t = (gf-0.5f)*2.0f;
                giL[i] = inL[i]*(1.0f-t) + tL[i]*t; giR[i] = inR[i]*(1.0f-t) + tR[i]*t;
                cpL[i] = bL[i]*t;                     cpR[i] = bR[i]*t;
            }
        }
    }

    //--- Step 3: Glitch Engine ---
    glitchEngine.process(glitchInputBuffer);

    //--- Step 4: Morph blend + add full-block feedback from previous block ---
    {
        auto* outL = buffer.getWritePointer(0);
        auto* outR = buffer.getWritePointer(1);
        const auto* giL = glitchInputBuffer.getReadPointer(0);
        const auto* giR = glitchInputBuffer.getReadPointer(1);
        const auto* cpL = cleanPassBuffer.getReadPointer(0);
        const auto* cpR = cleanPassBuffer.getReadPointer(1);
        const auto* fbL = feedbackBuffer.getReadPointer(0);
        const auto* fbR = feedbackBuffer.getReadPointer(1);
        for (int i = 0; i < N; ++i)
        {
            const float morph = morphSmoothed.getNextValue();
            feedbackSmoothed.getNextValue();  // advance, read via getCurrentValue() later
            outL[i] = cpL[i] + giL[i] * morph + fbL[i];
            outR[i] = cpR[i] + giR[i] * morph + fbR[i];
        }
    }

    //--- Step 5: DiffusionNetwork ---
    diffusionNetwork.process(buffer);
    wetBuffer.copyFrom(0, 0, buffer, 0, 0, N);
    wetBuffer.copyFrom(1, 0, buffer, 1, 0, N);

    //--- Step 6: Filter in WET position ---
    if (filterPos == 1)
    {
        filterDetector.process(wetBuffer, N);
        dualFilter.process(filterDetector.getTextureBuffer(),
                           filterDetector.getBodyBuffer(), wetBuffer, N);
    }

    //--- Step 7: Dry/Wet MIX + Bypass blend ---
    {
        auto* outL = buffer.getWritePointer(0);
        auto* outR = buffer.getWritePointer(1);
        const auto* dryL = dryBuffer.getReadPointer(0);
        const auto* dryR = dryBuffer.getReadPointer(1);
        const auto* wetL = wetBuffer.getReadPointer(0);
        const auto* wetR = wetBuffer.getReadPointer(1);
        for (int i = 0; i < N; ++i)
        {
            const float mix      = mixSmoothed.getNextValue();
            const float bypassed = bypassSmoothed.getNextValue();
            float procL = dryL[i]*(1.0f-mix) + wetL[i]*mix;
            float procR = dryR[i]*(1.0f-mix) + wetR[i]*mix;
            outL[i] = dryL[i]*bypassed + procL*(1.0f-bypassed);
            outR[i] = dryR[i]*bypassed + procR*(1.0f-bypassed);
            processedBuffer.setSample(0, i, procL);
            processedBuffer.setSample(1, i, procR);
        }
    }

    //--- Step 8: Filter in FINAL position ---
    if (filterPos == 2)
    {
        filterDetector.process(processedBuffer, N);
        dualFilter.process(filterDetector.getTextureBuffer(),
                           filterDetector.getBodyBuffer(), filteredBuffer, N);
        auto* outL = buffer.getWritePointer(0);
        auto* outR = buffer.getWritePointer(1);
        const auto* dryL  = dryBuffer.getReadPointer(0);
        const auto* dryR  = dryBuffer.getReadPointer(1);
        const auto* filtL = filteredBuffer.getReadPointer(0);
        const auto* filtR = filteredBuffer.getReadPointer(1);
        const float bypassGain = bypassSmoothed.getCurrentValue();
        for (int i = 0; i < N; ++i)
        {
            outL[i] = dryL[i]*bypassGain + filtL[i]*(1.0f-bypassGain);
            outR[i] = dryR[i]*bypassGain + filtR[i]*(1.0f-bypassGain);
        }
    }

    //--- Step 9: tanh output limiter ---
    {
        auto* outL = buffer.getWritePointer(0);
        auto* outR = buffer.getWritePointer(1);
        for (int i = 0; i < N; ++i)
        {
            outL[i] = std::tanh(outL[i]);
            outR[i] = std::tanh(outR[i]);
        }
    }

    //--- Step 10: Compute new feedbackBuffer for next block ---
    {
        const float fbGain = feedbackSmoothed.getCurrentValue();
        const auto* wL = wetBuffer.getReadPointer(0);
        const auto* wR = wetBuffer.getReadPointer(1);
        auto* fbL = feedbackBuffer.getWritePointer(0);
        auto* fbR = feedbackBuffer.getWritePointer(1);
        for (int i = 0; i < N; ++i)
        {
            fbL[i] = wL[i] * fbGain;
            fbR[i] = wR[i] * fbGain;
        }

        // PitchDrifter operates in feedback loop
        pitchDrifter.process(feedbackBuffer);

        // Optional filter in feedback loop
        if (filterPos == 3)
        {
            filterDetector.process(feedbackBuffer, N);
            dualFilter.process(filterDetector.getTextureBuffer(),
                               filterDetector.getBodyBuffer(), feedbackBuffer, N);
        }

        // Wavefolder (true bypass at drive=0)
        const float drive = driveSmoothed.getCurrentValue();
        if (drive > 0.001f)
        {
            const float foldAmt = 1.0f + drive * 3.0f;
            const float piC     = juce::MathConstants<float>::pi;
            auto* fL = feedbackBuffer.getWritePointer(0);
            auto* fR = feedbackBuffer.getWritePointer(1);
            for (int i = 0; i < N; ++i)
            {
                float fx = fL[i] * foldAmt * piC;
                fL[i]    = fL[i] + drive * (std::sin(fx) / foldAmt - fL[i]);
                fx       = fR[i] * foldAmt * piC;
                fR[i]    = fR[i] + drive * (std::sin(fx) / foldAmt - fR[i]);
            }
        }
    }
}
```

- [ ] **Step 4: Build — expect clean compile**

```bash
cd /home/myuser/EpiphanyMachine && cmake --build build -- -j$(nproc) 2>&1 | tail -15
```

Expected: `[100%] Linking CXX shared library ... EpiphanyMachine.vst3` — no errors. (PluginEditor.cpp may warn about `filterPosBtns` array size — fixed in Task 10.)

- [ ] **Step 5: Commit**

```bash
git add src/PluginProcessor.cpp
git commit -m "feat: PluginProcessor v3 — new params, full-buffer feedback, wavefolder, tanh limiter, filterPos=Feedback"
```

---

## Task 10: PluginEditor — DRIVE Knob + 4 Filter Buttons + v3 Label

**Files:**
- Modify: `src/PluginEditor.h`
- Modify: `src/PluginEditor.cpp`

- [ ] **Step 1: Update `PluginEditor.h` — add drive, expand filterPosBtns**

In `src/PluginEditor.h`, replace:

```cpp
    juce::TextButton filterPosBtns[3];
```

with:

```cpp
    juce::TextButton filterPosBtns[4];
```

In the "Column 3: Diffusion + Feedback" section, after `KnobGroup drift, feedback;`, add:

```cpp
    KnobGroup drive;
```

- [ ] **Step 2: Update constructor in `PluginEditor.cpp`**

In `src/PluginEditor.cpp`, in the constructor, replace the filter position buttons setup:

```cpp
    juce::StringArray fpLabels{"OFF","WET","FINAL"};
    for (int i = 0; i < 3; ++i)
    {
        filterPosBtns[i].setButtonText(fpLabels[i]);
        filterPosBtns[i].setClickingTogglesState(false);
        filterPosBtns[i].setLookAndFeel(&laf);
        filterPosBtns[i].onClick = [this, i]() {
            if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(
                    processor.apvts.getParameter("filterPos")))
                param->setValueNotifyingHost(param->convertTo0to1(i));
            updateFilterButtons();
        };
        addAndMakeVisible(filterPosBtns[i]);
    }
```

with:

```cpp
    juce::StringArray fpLabels{"OFF","WET","FINAL","FB"};
    for (int i = 0; i < 4; ++i)
    {
        filterPosBtns[i].setButtonText(fpLabels[i]);
        filterPosBtns[i].setClickingTogglesState(false);
        filterPosBtns[i].setLookAndFeel(&laf);
        filterPosBtns[i].onClick = [this, i]() {
            if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(
                    processor.apvts.getParameter("filterPos")))
                param->setValueNotifyingHost(param->convertTo0to1(i));
            updateFilterButtons();
        };
        addAndMakeVisible(filterPosBtns[i]);
    }
```

After the filter buttons loop (before texture type buttons), add DRIVE knob setup:

```cpp
    // ---- Col 3: Drive (in feedback loop) ----
    drive.setup("drive", "DRIVE", p.apvts, laf, this);
```

- [ ] **Step 3: Update `updateFilterButtons()` — loop to 4**

Replace:

```cpp
    for (int i = 0; i < 3; ++i)
        filterPosBtns[i].setToggleState(i == fp, juce::dontSendNotification);
```

with:

```cpp
    for (int i = 0; i < 4; ++i)
        filterPosBtns[i].setToggleState(i == fp, juce::dontSendNotification);
```

- [ ] **Step 4: Add drive to `updateValueLabels()`**

After:

```cpp
    feedback.valueLabel.setText(juce::String(getV("feedback"), 2), juce::dontSendNotification);
```

add:

```cpp
    drive.valueLabel.setText(juce::String(getV("drive"), 2), juce::dontSendNotification);
```

- [ ] **Step 5: Update version label in `paint()`**

Replace:

```cpp
    g.drawText("v2.0", getWidth() - 44, 8, 36, 14, juce::Justification::centredRight);
```

with:

```cpp
    g.setColour(CustomLookAndFeel::gold().withAlpha(0.5f));
    g.drawText("v3", getWidth() - 36, 8, 28, 14, juce::Justification::centredRight);
```

- [ ] **Step 6: Update `resized()` — 4-way filter buttons + DRIVE knob placement**

In `resized()`, Col 4 section, replace:

```cpp
        const int btnW = (c.getWidth() - 8) / 3;
        for (int i = 0; i < 3; ++i)
            filterPosBtns[i].setBounds(c.getX() + 4 + i*btnW, c.getY() + 20, btnW - 2, btnH);
```

with:

```cpp
        const int btnW = (c.getWidth() - 8) / 4;
        for (int i = 0; i < 4; ++i)
            filterPosBtns[i].setBounds(c.getX() + 4 + i*btnW, c.getY() + 20, btnW - 2, btnH);
```

In `resized()`, Col 3 section, after `feedback.place(...)` add:

```cpp
        drive.place(cx - ks/2, fbTop + ks + 28, ks, this);
```

Also update the FEEDBACK LOOP sub-panel height in `paint()` to accommodate the extra knob. Replace:

```cpp
    const int fbPanelH = 100;
```

with:

```cpp
    const int fbPanelH = 160;
```

- [ ] **Step 7: Build — final clean compile**

```bash
cd /home/myuser/EpiphanyMachine && cmake --build build -- -j$(nproc) 2>&1 | tail -15
```

Expected: `[100%] Linking CXX shared library ... EpiphanyMachine.vst3` with zero errors.

- [ ] **Step 8: Commit**

```bash
git add src/PluginEditor.h src/PluginEditor.cpp
git commit -m "feat: PluginEditor v3 — DRIVE knob, 4-way filter position, v3 version label"
```

---

## Task 11: Integration Build + Smoke Test

**Files:** None (build + manual test only)

- [ ] **Step 1: Clean build**

```bash
cd /home/myuser/EpiphanyMachine
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(nproc) 2>&1 | tail -5
```

Expected: `[100%] Linking CXX shared library libEpiphanyMachine.so ... Built target EpiphanyMachine_VST3`

- [ ] **Step 2: Verify plugin installed**

```bash
ls -lh ~/.vst3/EpiphanyMachine.vst3/Contents/x86_64-linux/EpiphanyMachine.so
```

Expected: file exists, size ≥ 500KB.

- [ ] **Step 3: Manual smoke test in DAW**

Load the plugin and verify:
1. GUI shows `v3` label (gold, 50% opacity, top-right)
2. DRIVE knob appears in column 3 below DRIFT/FEEDBACK pair
3. Filter position buttons show 4 options: OFF / WET / FINAL / FB
4. Preset dropdown shows 18 quantum physics preset names
5. Select "Singularity" (index 15) — plugin morphs to that preset
6. Turn DRIVE to 1.0 with high FEEDBACK — harmonic fold content audible
7. Toggle FREEZE — frozen tail sustained cleanly
8. DRIFT at 24st with high FEEDBACK — shimmer pitch rises over time

- [ ] **Step 4: Final commit**

```bash
git tag v3.0.0
git commit --allow-empty -m "chore: v3.0.0 integration complete"
```

---

## Self-Review

### Spec coverage check

| Spec requirement | Task |
|---|---|
| 8-line FDN | Task 2 |
| FHT 8×8 | Task 2 |
| RT60 decay formula | Task 2 |
| Per-line pan LFOs (replace StereoWidener) | Task 2 |
| Delete StereoWidener | Task 5 |
| setDecaySeconds API (0–20s range) | Task 2 + Task 9 |
| GlitchEngine drift ±drift semitones | Task 3 |
| Adaptive grain envelopes (Hann/Tukey/Trap) | Task 3 |
| Grain length 1–1000ms (density-scaled) | Task 3 |
| Chaos expansion: double-speed at chaos=1 | Task 3 |
| Float playhead (fractional speed interp) | Task 3 |
| Remove silence-gate buffer clear | Task 0, Fix 3 |
| DualFilter Q 0.4–50 (self-oscillating) | Task 4 |
| LFO depth ±3 octaves | Task 4 |
| DRIVE knob (0–1, true bypass at 0) | Task 9 + Task 10 |
| Wavefolder in feedback loop | Task 9 |
| Full-buffer feedback path | Task 9 |
| filterPos=3 (Feedback) | Task 9 + Task 10 |
| tanh output limiter | Task 9 |
| Remove 8kHz feedback LPF | Task 8 (fbLpfCoeff removed) + Task 9 |
| Remove freeze double-clip | Task 9 (not in new processBlock) |
| Drift range 0–24st | Task 9 parameter layout |
| Decay 0–20s parameter | Task 9 parameter layout |
| Feedback 0–0.95 | Task 9 parameter layout |
| MorphTime 0.1–30s | Task 9 parameter layout |
| 18 quantum physics presets | Task 6 |
| `drive` field in PresetData | Task 6 |
| `drive` in preset morph | Task 7 |
| `static int lastPreset` fix | Task 0, Fix 1 |
| `atomic<bool> systemChanging` | Task 0, Fix 2 |
| paramIndexCache for O(1) lookup | Task 0, Fix 4 |
| CMakeLists version 3.0.0 | Task 1 |
| `v3` label (gold 50% opacity) | Task 10 |

### Placeholder scan

No TBD, TODO, "implement later", or "similar to Task N" patterns present.

### Type consistency

- `setDecaySeconds(float)` used in Task 2 (DiffusionNetwork.h) and Task 9 (processBlock): ✓ match
- `setDrift(float)` on GlitchEngine defined Task 3, called Task 9: ✓ match
- `setWidth(float)` on DiffusionNetwork defined Task 2, called Task 9: ✓ match
- `feedbackBuffer` declared Task 8, allocated Task 9 (`prepareToPlay`), used Task 9 (`processBlock`): ✓ consistent
- `lastPresetIdx` declared Task 0 (already added to .h), used Task 9: ✓ consistent
- `drive` in `PresetData` struct (Task 6) matches `captureCurrentValues` return (Task 7) and `applyInterpolated` write (Task 7): ✓ consistent
- `filterPosBtns[4]` declared Task 10 .h, iterated to 4 in Task 10 .cpp: ✓ consistent
- `KnobGroup drive` declared Task 10 .h, setup called Task 10 .cpp, placed in resized() Task 10: ✓ consistent



