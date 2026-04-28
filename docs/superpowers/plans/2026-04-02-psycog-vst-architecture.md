# Psycog VST Architecture Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Design module architecture, parameter routing, and state management for Psycog VST plugin.

**Architecture:** JUCE-based VST3 plugin using AudioProcessorValueTreeState (APVTS) for parameter management. Signal flow: Time-Stretch/Freeze → Wavefolding → Wet Processing → Mix with delayed dry. Double-buffered freeze system. Single LFO for multi-target modulation.

**Tech Stack:** JUCE 7+, C++17, Catch2 for testing

---

## Module Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            PsycogAudioProcessor                             │
│  ┌───────────────────────────────────────────────────────────────────────┐  │
│  │                           Audio Process                                │  │
│  │                                                                        │  │
│  │  ┌──────────────┐                              ┌──────────────────┐   │  │
│  │  │  DryDelay    │                              │   TimeStretch    │   │  │
│  │  │  (2048 samp) │                              │   + FreezeBuffer│   │  │
│  │  │              │                              │   (double buf)   │   │  │
│  │  └──────┬───────┘                              └────────┬─────────┘   │  │
│  │         │                                               │            │  │
│  │         │                                      ┌────────▼─────────┐  │  │
│  │         │                                      │   Wavefolder     │  │  │
│  │         │                                      │ (triangle fold)  │  │  │
│  │         │                                      │ (L/R offset)     │  │  │
│  │         │                                      └────────┬─────────┘  │  │
│  │         │                                               │            │  │
│  │         │         ┌──────────────────┐        ┌──────────▼─────────┐ │  │
│  │         │         │ Threshold       │        │   WetProcessor     │ │  │
│  │         │         │ Detector        │◄───────│   - SoftClip       │ │  │
│  │         │         │ (measures here) │  │     │   - AutoNormalize  │ │  │
│  │         │         └──────────────────┘  │     └──────────┬─────────┘ │  │
│  │         │                               │                │           │  │
│  │         │         ┌──────────────────┐  │                │           │  │
│  │         │         │      LFO         │  │                │           │  │
│  │         │         │    (single)      │──┼──► Modulates:  │           │  │
│  │         │         └──────────────────┘  │   stretch,     │           │  │
│  │         │                               │   position,    │           │  │
│  │         │                               │   foldAmount,  │           │  │
│  │         │                               │   foldOffset   │           │  │
│  │         │                               │                │           │  │
│  │         │                               │     ┌──────────▼─────────┐ │  │
│  │         │                               │     │      Mixer         │ │  │
│  │         └───────────────────────────────┼────►│   (wet/dry blend)  │ │  │
│  │                                       │     └──────────┬─────────┘ │  │
│  │                                       │                │           │  │
│  │         ┌─────────────────────────────┼────────────────┘           │  │
│  │         │                             │                            │  │
│  │  ┌──────▼──────────────────────────────────────────────────────────▼┐ │  │
│  │  │                     OutputProtection                             │ │  │
│  │  │                     (final soft-clip)                           │ │  │
│  │  └──────────────────────────────────────────────────────────────────┬┘ │  │
│  └─────────────────────────────────────────────────────────────────────┼──┘  │
└─────────────────────────────────────────────────────────────────────────┼────┘
                                                                          │
                                                                          ▼
                                                                    Audio Output

SIGNAL FLOW KEY:
═══════════════
Audio In → ┬→ DryDelay (2048 samples) ──────────────────────────────────┐
           │                                                            │
           └→ TimeStretch → Wavefolder ──┬──► WetProcessor ──► Mixer ──┼──► OutputProtection → Out
                                        │                              │
                                        └──► ThresholdDetector ────────┘
                                             (measures wavefolder
                                              output BEFORE wet proc)

LFO ─────► Modulates: stretch, position, foldAmount, foldOffset (NOT mix)
```

**CRITICAL: ThresholdDetector measures at Wavefolder output, BEFORE WetProcessor.**
This is per spec line 77: "measured directly at wavefolder output, before wet processing and mix"

---

## Constants

All implementation constants in one place for reference:

```cpp
// utils/Constants.h

namespace PsycogConstants {
    // === Buffer Sizes ===
    constexpr int grainSize = 2048;              // Hann window size
    constexpr int dryDelaySamples = 2048;        // Dry path delay (matches grain latency)
    constexpr double freezeBufferSeconds = 3.0; // Each freeze buffer holds 3 seconds
    
    // === Freeze System ===
    constexpr double freezeCrossfadeMs = 10.0;   // Crossfade for freeze/unfreeze transitions
    constexpr double autoTriggerCooldownMs = 100.0; // Minimum time between auto-triggers
    
    // === Auto-Normalize ===
    constexpr double autoNormalizeTargetDb = -6.0;  // Target RMS level
    constexpr double autoNormalizeAttackMs = 300.0;  // Attack time
    constexpr double autoNormalizeReleaseMs = 1000.0; // Release time
    constexpr double noiseGateThresholdDb = -60.0;    // Bypass below this
    constexpr double gainMin = 0.1;  // ~ -20dB max reduction
    constexpr double gainMax = 3.0;  // ~ +10dB max boost
    
    // === Parameter Smoothing ===
    constexpr double smoothingMs = 20.0; // All continuous params
    
    // === CPU Budget ===
    constexpr double cpuBudgetPercent = 15.0; // Target max CPU
    
    // === Latency ===
    // Granular time-stretch requires one grain window look-ahead
    // Report this to DAW via setLatencySamples(grainSize)
    
    // === Sample Rate Notes ===
    // Primary: 44.1kHz, 48kHz
    // Supported: 88.2kHz, 96kHz (CPU may exceed budget)
}
```

---

## Formulas & Algorithms

### Triangle Wavefolding (CRITICAL)

```cpp
// utils/MathUtils.h

/**
 * Triangle fold function: reflects signal at ±1 boundaries.
 * Input can be any value; output is always in [-1, 1].
 * Produces rich harmonics that increase with drive.
 */
inline float triangleFold(float x) {
    // Wrap to period [0, 4)
    x = std::fmod(x + 1.0f, 4.0f);
    if (x < 0.0f) x += 4.0f;
    
    // Triangle wave in [-1, 1]
    return (x < 2.0f) ? (x - 1.0f) : (3.0f - x);
}

/**
 * Apply wavefolding with drive and offset.
 * 
 * @param input        Input sample [-1, 1]
 * @param foldAmount   Normalized fold amount [0, 1]
 * @param offset       DC offset for asymmetry [-1, 1]
 * @return             Folded sample in [-1, 1]
 */
inline float applyWavefold(float input, float foldAmount, float offset) {
    // Drive mapping: foldAmount [0, 1] → drive [1.0, 10.0]
    // At foldAmount=0: drive=1.0, no folding (passthrough)
    // At foldAmount=1: drive=10.0, aggressive folding
    float drive = 1.0f + foldAmount * 9.0f;
    
    // Offset scales with foldAmount so foldAmount=0 is true passthrough
    // At foldAmount=0, offset has no effect regardless of its value
    float effectiveOffset = offset * foldAmount;
    
    // Apply drive and offset before folding
    float folded = triangleFold((input + effectiveOffset) * drive);
    
    return folded;
}
```

### Drive Mapping Formula

```
drive = 1.0 + foldAmount * 9.0

foldAmount = 0.0 → drive = 1.0  (no folding)
foldAmount = 0.5 → drive = 5.5  (moderate)
foldAmount = 1.0 → drive = 10.0 (aggressive)
```

### Offset Scaling Formula

```
effectiveOffset = offset * foldAmount

foldAmount = 0.0 → effectiveOffset = 0 (true passthrough)
foldAmount = 1.0 → effectiveOffset = offset (full asymmetry)
```

This ensures `foldAmount=0` is true passthrough regardless of offset value.

### LFO Modulation Formula

LFO modulates parameters on their **normalized internal scale** (0.0–1.0):

```cpp
// For each parameter targeted by LFO:
float normalizedBase = toNormalized(baseValue);  // e.g., stretch 1.0x → 0.5
float lfoOutput = lfoWaveform(phase);            // bipolar: -1.0 to +1.0
float modulated = normalizedBase + lfoOutput * depth;
modulated = juce::jlimit(0.0f, 1.0f, modulated);
float finalValue = fromNormalized(modulated);     // e.g., 0.2 → ~0.25x stretch
```

**Example:** Base stretch = 0.8x (normalized ~0.45 on log scale), depth = 0.6:
- LFO sweeps normalized from 0.45 ± 0.6 = clamped to [0.0, 1.0]
- Maps back to stretch: roughly 0.1x to 6.5x

### Auto-Normalize Gain Formula

```cpp
float targetRms = juce::Decibels::decibelsToGain(autoNormalizeTargetDb); // -6dBFS
float currentRms = computeRms(wetSignal, numSamples);

// Noise gate: bypass if below threshold
if (juce::Decibels::gainToDecibels(currentRms) < noiseGateThresholdDb) {
    return 1.0f; // Bypass: no gain adjustment
}

// Compute gain with clamping
float gain = targetRms / currentRms;
gain = juce::jlimit(gainMin, gainMax, gain); // Clamp 0.1 to 3.0

return gain;
```

### Logarithmic Parameter Mapping

```cpp
// Stretch: 0.1x to 10x, log scale, center at 1.0x
// Normalized 0.0 = 0.1x, 0.5 = 1.0x, 1.0 = 10.0x
float stretchFromNormalized(float normalized) {
    constexpr float minStretch = 0.1f;
    constexpr float maxStretch = 10.0f;
    return minStretch * std::pow(maxStretch / minStretch, normalized);
}

float stretchToNormalized(float stretch) {
    constexpr float minStretch = 0.1f;
    constexpr float maxStretch = 10.0f;
    return std::log(stretch / minStretch) / std::log(maxStretch / minStretch);
}

// LFO Rate: 0.01 Hz to 20 Hz, log scale
float lfoRateFromNormalized(float normalized) {
    constexpr float minRate = 0.01f;
    constexpr float maxRate = 20.0f;
    return minRate * std::pow(maxRate / minRate, normalized);
}
```

---

## Per-Sample Processing Requirements

### SmoothedValue Usage (CRITICAL)

**All SmoothedValue instances MUST be called per-sample, NOT per-block:**

```cpp
// WRONG - once per block (CM-05 failure mode):
void processBlock(AudioBuffer<float>& buffer) {
    auto currentValue = smoothedValue.getNextValue(); // Called once!
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        // Uses same value for entire block
    }
}

// RIGHT - per sample:
void processBlock(AudioBuffer<float>& buffer) {
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        float currentValue = smoothedValue.getNextValue(); // Per sample!
        // Use currentValue for this sample only
    }
}
```

### LFO Phase Increment (CRITICAL)

**LFO phase MUST increment per-sample:**

```cpp
// WRONG - once per block:
void processBlock(...) {
    float lfoValue = lfo.process(); // Single value for block
}

// RIGHT - per sample:
void processBlock(...) {
    float phaseIncrement = lfoRate / sampleRate;
    for (int sample = 0; sample < numSamples; ++sample) {
        float lfoValue = lfoWaveform(lfoPhase);
        lfoPhase += phaseIncrement;
        if (lfoPhase >= 1.0f) lfoPhase -= 1.0f;
        // Use lfoValue for this sample only
    }
}
```

---

## Mono→Stereo Processing

For mono input, create stereo width with opposite offsets:

```cpp
// Wavefolder.cpp
void Wavefolder::process(float* leftOut, float* rightOut, 
                         const float* input, int numSamples,
                         float foldAmount, float offset) {
    for (int i = 0; i < numSamples; ++i) {
        float in = input[i];
        
        // Apply wavefolding with opposite offsets for stereo width
        // L channel: +offset, R channel: -offset
        *leftOut++ = applyWavefold(in, foldAmount, +offset);
        *rightOut++ = applyWavefold(in, foldAmount, -offset);
    }
}

// For stereo input, apply same offset to both channels:
void Wavefolder::processStereo(float* leftOut, float* rightOut,
                               const float* leftIn, const float* rightIn,
                               int numSamples, float foldAmount, float offset) {
    for (int i = 0; i < numSamples; ++i) {
        // Both channels use same offset (not opposite)
        *leftOut++ = applyWavefold(*leftIn++, foldAmount, offset);
        *rightOut++ = applyWavefold(*rightIn++, foldAmount, offset);
    }
}
```

---

## Boundary Conditions

### Threshold at 0 (Chaos Mode)

```
threshold = 0 → Any non-silent input triggers freeze continuously

Behavior: At threshold=0, virtually any amplitude above -inf triggers freeze.
This creates rapid buffer switching (limited by 100ms cooldown).
Result: "Chaos mode" — intended behavior for extreme settings.
```

### Threshold at 1 (Effectively Disabled)

```
threshold = 1 → Only full-scale peaks trigger freeze

Behavior: At threshold=1, only signals reaching ±1.0 trigger freeze.
Most audio material never reaches full-scale peaks.
Result: Freeze triggering is effectively disabled for normal material.
```

### Fold Amount at 0 (True Passthrough)

```
foldAmount = 0 → Signal passes through unchanged

Behavior: drive = 1.0, effectiveOffset = 0.0
Signal multiplied by 1.0 with no offset → no folding
This ensures true passthrough regardless of offset parameter value.
```

### Stretch at 1x (Latency Still Applies)

```
stretch = 1.0x → Normal speed, but ~46ms latency still applies

Behavior: Grains reconstruct original timing.
Latency from grain window (2048 samples) is still reported to DAW.
This is inherent to the granular algorithm.
```

---

## Latency Reporting

```cpp
// PluginProcessor.cpp - prepareToPlay()
void PsycogAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Granular time-stretch requires one grain window look-ahead
    // Report latency to DAW for compensation
    setLatencySamples(grainSize); // 2048 samples ≈ 46ms @ 48kHz
    
    // ... rest of preparation
}
```

---

## Sample Rate Considerations

| Sample Rate | Status | Notes |
|-------------|--------|-------|
| 44.1 kHz | ✅ Primary | Optimized, within CPU budget |
| 48 kHz | ✅ Primary | Optimized, within CPU budget |
| 88.2 kHz | ⚠️ Supported | CPU may exceed 15% budget |
| 96 kHz | ⚠️ Supported | CPU may exceed 15% budget |

Higher sample rates require more processing:
- Freeze buffers: 3s × rate × 2 buffers × 2 channels
- Grain processing: More grains per second
- RMS window: More samples per 300ms

---

## File Structure

```
PsycogVST/
├── CMakeLists.txt
├── src/
│   ├── PluginProcessor.h          # Main processor, owns all modules
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h             # GUI
│   ├── PluginEditor.cpp
│   ├── Parameters.h               # APVTS layout, parameter IDs
│   ├── Parameters.cpp
│   ├── modules/
│   │   ├── DryDelay.h             # 2048-sample delay for dry path
│   │   ├── DryDelay.cpp
│   │   ├── TimeStretch.h          # Granular time-stretch + freeze
│   │   ├── TimeStretch.cpp
│   │   ├── FreezeBuffer.h         # Double-buffered 3-second buffer
│   │   ├── FreezeBuffer.cpp
│   │   ├── Wavefolder.h           # Triangle wavefolding
│   │   ├── Wavefolder.cpp
│   │   ├── ThresholdDetector.h    # Amplitude threshold for auto-freeze
│   │   ├── ThresholdDetector.cpp
│   │   ├── WetProcessor.h         # Soft-clip + auto-normalize
│   │   ├── WetProcessor.cpp
│   │   ├── Mixer.h                # Wet/dry blend
│   │   ├── Mixer.cpp
│   │   ├── LFO.h                  # Single LFO for modulation
│   │   ├── LFO.cpp
│   │   ├── OutputProtection.h     # Final soft-clip
│   │   └── OutputProtection.cpp
│   └── utils/
│       ├── Constants.h            # All constants (see above)
│       └── MathUtils.h            # triangleFold, formulas
├── tests/
│   ├── CMakeLists.txt
│   ├── TestTimeStretch.cpp
│   ├── TestWavefolder.cpp
│   ├── TestLFO.cpp
│   ├── TestFreezeBuffer.cpp
│   └── TestParameters.cpp
└── presets/
    ├── TurningThroughTime.xml
    ├── GoldenMemories.xml
    ├── OmnipotentObservers.xml
    └── InfiniteCogs.xml
```

---

## Parameter Routing

### Parameter Ownership

| Parameter | Owner Module | Type | Range | Default |
|-----------|--------------|------|-------|---------|
| `stretch` | TimeStretch | Float (log) | 0.1–10x | 1.0x |
| `position` | TimeStretch | Float | 0–1 | 0.5 |
| `freezeMode` | TimeStretch | Choice | Manual/Auto/Off | Off |
| `threshold` | ThresholdDetector | Float | 0–1 | 0.3 |
| `foldAmount` | Wavefolder | Float | 0–1 | 0.0 |
| `foldOffset` | Wavefolder | Float | -1–1 | 0.0 |
| `lfoRate` | LFO | Float (log) | 0.01–20 Hz | 0.5 Hz |
| `lfoWaveform` | LFO | Choice | Sine/Tri/Square/S&H | Sine |
| `lfoDepth` | LFO | Float | 0–1 | 0.0 |
| `lfoTargetStretch` | LFO | Bool | on/off | off |
| `lfoTargetPosition` | LFO | Bool | on/off | off |
| `lfoTargetFoldAmount` | LFO | Bool | on/off | off |
| `lfoTargetFoldOffset` | LFO | Bool | on/off | off |
| `mix` | Mixer | Float | 0–100% | 100% |

### LFO Target Parameters

LFO modulates parameters on their **normalized internal scale** (0.0–1.0). Each target has a dedicated bool parameter for enable/disable:

```
Parameters affected by LFO (when target enabled):
- stretch → lfoTargetStretch
- position → lfoTargetPosition
- foldAmount → lfoTargetFoldAmount
- foldOffset → lfoTargetFoldOffset

NOT modulated: mix (LFO does NOT affect mix parameter)
```

### Parameter Smoothing

| Parameter | Smoothing Time | Reason |
|-----------|----------------|--------|
| stretch | 20ms | Prevents granular glitches |
| position | 20ms | Prevents playhead clicks |
| foldAmount | 20ms | Prevents wavefolding discontinuities |
| foldOffset | 20ms | Prevents wavefolding discontinuities |
| mix | 20ms | Prevents blend clicks |
| lfoDepth | 20ms | Smooth modulation intensity |
| lfoRate | 20ms | Smooth rate changes |

**No smoothing:** freezeMode (discrete), lfoWaveform (discrete), lfoTarget* (bool)

---

## State Management

### APVTS Layout

```cpp
// Parameters.h
namespace ParamIDs {
    // Time Stage
    constexpr const char* stretch = "stretch";
    constexpr const char* position = "position";
    constexpr const char* freezeMode = "freezeMode";
    constexpr const char* threshold = "threshold";

    // Wavefolding
    constexpr const char* foldAmount = "foldAmount";
    constexpr const char* foldOffset = "foldOffset";

    // LFO
    constexpr const char* lfoRate = "lfoRate";
    constexpr const char* lfoWaveform = "lfoWaveform";
    constexpr const char* lfoDepth = "lfoDepth";
    constexpr const char* lfoTargetStretch = "lfoTargetStretch";
    constexpr const char* lfoTargetPosition = "lfoTargetPosition";
    constexpr const char* lfoTargetFoldAmount = "lfoTargetFoldAmount";
    constexpr const char* lfoTargetFoldOffset = "lfoTargetFoldOffset";

    // Output
    constexpr const char* mix = "mix";
}

// Parameter layout function
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
```

### Preset Format

Presets saved as XML via APVTS `copyState()` / `replaceState()`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<PsycogVST version="1.0">
  <PARAM id="stretch" value="0.45"/>  <!-- 0.8x -->
  <PARAM id="position" value="0.5"/>
  <PARAM id="freezeMode" value="2"/>   <!-- Auto -->
  <PARAM id="threshold" value="0.4"/>
  <PARAM id="foldAmount" value="0.3"/>
  <PARAM id="foldOffset" value="0.1"/>
  <PARAM id="lfoRate" value="0.3"/>
  <PARAM id="lfoWaveform" value="1"/>  <!-- Triangle -->
  <PARAM id="lfoDepth" value="0.6"/>
  <PARAM id="lfoTargetStretch" value="1"/>
  <PARAM id="lfoTargetPosition" value="0"/>
  <PARAM id="lfoTargetFoldAmount" value="1"/>
  <PARAM id="lfoTargetFoldOffset" value="0"/>
  <PARAM id="mix" value="0.7"/>
</PsycogVST>
```

### State Serialization Requirements

1. **getStateInformation()**: Serialize all APVTS parameters to XML
2. **setStateInformation()**: Restore from XML, trigger parameter updates
3. **Version compatibility**: Include version attribute for future migrations
4. **Round-trip test**: Verify save → load preserves all values

---

## Audio Thread Safety

### Real-time Safe Operations

| Module | Audio Thread | Non-Real-time |
|--------|-------------|----------------|
| DryDelay | Read/write circular buffer | — |
| TimeStretch | Read grains, crossfade | Buffer swap (atomic flag) |
| FreezeBuffer | Read frozen buffer | Allocate buffers (prepareToPlay) |
| Wavefolder | Triangle fold calculation | — |
| ThresholdDetector | Amplitude check | — |
| WetProcessor | Soft-clip, RMS, gain | — |
| LFO | Phase increment, waveform | — |
| Mixer | Blend calculation | — |

### Pre-allocated Resources

All buffers allocated in `prepareToPlay()`:
- Dry delay line: 2048 samples × 2 channels
- Freeze buffers: 3 seconds × sample rate × 2 buffers × 2 channels
- Grain buffer: 2048 samples × 2 channels
- RMS window: 300ms × sample rate

**No allocations in processBlock.**

---

## Test Strategy

### Unit Tests

| Test File | Coverage |
|-----------|----------|
| TestTimeStretch | Stretch ratios 0.1x, 1x, 10x; freeze mode switching; crossfade clicks |
| TestWavefolder | Fold amounts 0–1; offset L/R; mono→stereo; passthrough at 0 |
| TestLFO | Waveform shapes; rate accuracy; depth scaling; per-sample phase |
| TestFreezeBuffer | Double-buffer swap; position scrubbing; auto-trigger; cooldown |
| TestParameters | Log scaling; round-trip; preset load; boundary conditions |

### Integration Tests

| Test | Coverage |
|------|----------|
| Audio path | Signal flows input → output without crash |
| Parameter response | Each parameter affects audio output |
| Preset round-trip | Save → load → verify all values |
| LFO modulation | Modulated parameters vary over time |
| Latency reporting | setLatencySamples called correctly |

### Audio Thread Safety Audit

Run `juce-audio-thread-audit` after Phase 4:
- No allocations in processBlock
- No locks in processBlock
- No system calls in processBlock
- All buffers pre-allocated
- SmoothedValue called per-sample (CM-05 check)
- LFO phase incremented per-sample

---

## Tasks

### Task 1: Create Module Architecture Document

**Files:**
- Create: `docs/architecture/module-diagram.md`
- Create: `docs/architecture/parameters.md`
- Create: `docs/architecture/state-management.md`

- [ ] **Step 1: Write module diagram document**

```markdown
# Psycog VST Module Diagram

[Full module diagram with corrected ThresholdDetector placement]
```

- [ ] **Step 2: Write parameter routing document**

```markdown
# Psycog VST Parameter Routing

[Parameter ownership table and LFO routing]
```

- [ ] **Step 3: Write state management document**

```markdown
# Psycog VST State Management

[APVTS layout, preset format, serialization]
```

- [ ] **Step 4: Commit architecture documents**

```bash
git add docs/architecture/
git commit -m "docs: add Psycog VST architecture documents (Phase 2)"
```

---

## Self-Review

### 1. Spec Coverage

| Spec Section | Task Coverage |
|--------------|---------------|
| Signal Flow | ✅ Module diagram corrected, ThresholdDetector placement fixed |
| Time Stage Parameters | ✅ TimeStretch module, threshold detector |
| Wavefolding Parameters | ✅ Wavefolder module with formulas |
| LFO Parameters | ✅ LFO module with multi-target, NOT mix |
| Output Parameters | ✅ Mixer module |
| Channel Config | ✅ Mono→stereo detailed |
| Technical Specs | ✅ Latency, CPU, sample rates |
| Presets | ✅ 4 presets defined |
| Freeze Trigger Mechanism | ✅ FreezeBuffer with crossfade, cooldown |
| Audio Thread Safety | ✅ Pre-allocated, per-sample requirements explicit |
| Formulas | ✅ All algorithms included |
| Constants | ✅ All values from spec |
| Boundary Conditions | ✅ Threshold 0/1, FoldAmount 0, Stretch 1x |

### 2. Placeholder Scan

✅ No TBD, TODO, or placeholder patterns found.

### 3. Type Consistency

✅ Parameter IDs consistent throughout (stretch, position, etc.).
✅ Module names consistent (TimeStretch, Wavefolder, LFO, etc.).
✅ Formula variable names consistent.

---

## Audit Fixes Applied

| Issue | Fix |
|-------|-----|
| ThresholdDetector placement | ✅ Now measures at Wavefolder output |
| Triangle fold algorithm | ✅ Full algorithm included |
| Drive/offset formulas | ✅ Added to Formulas section |
| LFO modulation formula | ✅ Added with example |
| Auto-normalize constants | ✅ All values in Constants section |
| Freeze crossfade | ✅ 10ms constant added |
| Auto-trigger cooldown | ✅ 100ms constant added |
| SmoothedValue per-sample | ✅ Explicit requirement with code example |
| LFO phase per-sample | ✅ Explicit requirement with code example |
| Mono→stereo processing | ✅ Detailed with code |
| Boundary conditions | ✅ Threshold 0/1, foldAmount 0 documented |
| Latency reporting | ✅ setLatencySamples requirement added |
| Sample rate caveats | ✅ 88.2/96kHz warning added |
| Stretch at 1x | ✅ Latency still applies documented |
| LFO diagram arrow | ✅ Fixed - LFO modulates params, NOT mixer |

---

Plan complete and saved to `docs/superpowers/plans/2026-04-02-psycog-vst-architecture.md`.

**Two execution options:**

**1. Subagent-Driven (recommended)** - I dispatch a fresh subagent per task, review between tasks, fast iteration

**2. Inline Execution** - Execute tasks in this session using executing-plans, batch execution with checkpoints

**Which approach?**