# Psycog - Plugin Specification

**Version:** 1.0  
**Date:** 2026-04-02  
**Status:** Approved for Implementation

---

## Overview

**Name:** Psycog  
**Type:** Multi-effect module (psychedelic audio transformation)  
**Tagline:** Interdimensional sound transformation — constantly morphing, never harsh

Psycog is a creative audio transformation module that combines time manipulation with wavefolding to create vivid, psychedelic sounds. The effect is so prominent that it functions more as a module than a traditional effect — transforming any input into something entirely new.

---

## Sound Identity

**Character:** Constantly morphing, never settling, never harsh  
**Reference Universe:** Ambient/psybient (Shpongle, Ott, Carbon Based Lifeforms, Bluetech) + Glitch/experimental (Aphex Twin, Autechre)  
**Core Transformation:** Time-stretch/freeze feeding into wavefolding with bidirectional interaction

The plugin creates evolving soundscapes that transform continuously. When the wavefolder's harmonic intensity crosses a threshold, it triggers new freeze points — creating cascading layers of transformation that never repeat exactly the same way twice.

---

## Signal Flow

```
Audio Input
    ↓
┌─────────────────────────────────────┐
│     Time-Stretch / Freeze Stage     │
│  - Stretch: 0.1x to 10x             │
│  - Position: Buffer position        │
│  - Freeze: Manual / Auto / Off      │
│  - Buffer: 3 seconds                │
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│        Wavefolding Stage            │
│  - Fold Amount                      │
│  - Fold Offset (DC offset)          │
│  - Creates harmonic complexity      │
│  - Different offset L/R for stereo  │
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│           Mix Stage                 │
│  - Wet/Dry: 0-100%                  │
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│         Output Stage                │
│  - Auto-normalize                   │
│  - Soft-clip (never harsh)          │
└─────────────────────────────────────┘
    ↓
Audio Output
```

### Interaction Cascade

The wavefolder's **output amplitude** (fold intensity) can trigger new freeze points:
- When Freeze mode = Auto, crossing the threshold captures a new 3-second buffer
- This creates cascading transformation layers
- Never repeats exactly the same way twice

---

## Parameters

### Time Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Stretch** | 0.1x – 10x | Time stretch amount. 0.1x = extreme compression, 10x = extreme stretch |
| **Position** | 0 – 1 | Buffer position (where playhead reads). When frozen: position in frozen buffer. When not frozen: position in stretched audio |
| **Freeze Mode** | Manual / Auto / Off | Manual = button triggers freeze. Auto = threshold triggers freeze. Off = no freeze |
| **Threshold** | 0 – 1 | Sensitivity for auto-trigger. Only active in Auto mode. Visible but dimmed in other modes |

**Freeze Buffer:** 3 seconds of audio, **continuously recording** (circular buffer, oldest audio replaced by newest). When freeze is triggered, playback loops within the current 3-second buffer.

### Wavefolding Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Fold Amount** | 0 – 1 | Intensity of wavefolding. Higher = more harmonic complexity |
| **Fold Offset** | -1 – 1 | DC offset for asymmetric folding. Zero = symmetric (odd harmonics). Non-zero = asymmetric (odd + even harmonics). Different values for L/R create stereo width |

### Output Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Mix** | 0 – 100% | Wet/dry blend. 0% = dry only, 100% = fully processed |
| **Auto-normalize** | (internal) | Output always at healthy level |
| **Soft-clip** | (internal) | Prevents harshness on final output |

### LFO Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **LFO Rate** | 0.01 – 20 Hz | How fast the modulated parameter cycles |
| **LFO Waveform** | Sine, Triangle, Square, Random S&H | Shape of modulation |
| **LFO Depth** | 0 – 1 | Modulation intensity (0 = none, 1 = full range of target parameter) |
| **LFO Target** | Stretch, Position, Fold Amount, Fold Offset (any combination) | Which parameter(s) are modulated |

Each preset defines default LFO assignments. Users can modify rate, waveform, depth, and targets.

---

## Channel Configuration

| Input | Output | Processing |
|-------|--------|------------|
| Stereo | Stereo | Full stereo processing |
| Mono | Stereo | Mono split to L/R → different Fold Offset per channel → stereo widening |

---

## Technical Specifications

| Specification | Value |
|---------------|-------|
| **Formats** | VST3 |
| **Platforms** | Linux, Windows, macOS |
| **Sample Rates** | 44.1 kHz, 48 kHz |
| **Buffer Size** | 128 samples minimum (live use possible) |
| **Latency** | One buffer (~2.7ms @ 48kHz, 128 samples) |
| **CPU Budget** | < 15% single core |
| **Automation** | All parameters DAW-automatable |

### CPU Breakdown (Estimated)

| Component | CPU Cost |
|-----------|----------|
| Buffer recording | ~1% |
| Time-stretch (granular) | ~3-5% |
| Threshold detection | <1% |
| Wavefolding | ~1-2% |
| Auto-normalize | ~1-2% |
| Soft-clip | <1% |
| **Total** | **~7-12%** |

---

## GUI Layout

**Style:** Moderate — all parameters visible, organized sections

```
┌─────────────────────────────────────────────────────────┐
│  PSYCOG                              [Preset Name]      │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌───────────────────┐    ┌───────────────────┐        │
│  │   TIME-STRETCH    │    │   WAVEFOLDING    │        │
│  │                   │    │                  │        │
│  │  Stretch  [0.5x]  │    │  Fold Amt [0.4]  │        │
│  │  Position [────]  │    │  Fold Off [──0.2]│        │
│  │  Mode    [Auto]   │    │                  │        │
│  │  Threshold [0.3] │    │                  │        │
│  └───────────────────┘    └───────────────────┘        │
│                                                         │
│  ┌───────────────────┐    ┌───────────────────┐        │
│  │      LFO          │    │     OUTPUT        │        │
│  │                   │    │                   │        │
│  │  Rate   [0.5 Hz]  │    │  Mix    [80%]     │        │
│  │  Wave  [Sine]     │    │                   │        │
│  │  Depth  [0.5]     │    │                   │        │
│  │  Target[Position] │    │                   │        │
│  └───────────────────┘    └───────────────────┘        │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

**Note:** Final layout may vary. All parameters visible, no hidden menus.

---

## Core Presets

### 1. Turning Through Time
**Character:** Evolving morphing psychedelic pad with strong ambient presence

| Parameter | Value | Notes |
|-----------|-------|-------|
| Stretch | 0.5x | Slow evolution |
| Position | LFO modulated | Continuous movement |
| Freeze Mode | Auto | Threshold triggers cascade |
| Threshold | 0.3 | Moderate sensitivity |
| Fold Amount | 0.4 | Moderate harmonics |
| Fold Offset | 0.2 | Slight asymmetry |
| Mix | 80% | Primarily wet |

| LFO Setting | Value |
|-------------|-------|
| Rate | 0.1 Hz |
| Waveform | Sine |
| Depth | 0.3 |
| Target | Position |

### 2. Golden Memories
**Character:** Warm keys that transform to psychedelic when held, responds to note duration

| Parameter | Value | Notes |
|-----------|-------|-------|
| Stretch | 0.8x base, LFO modulated | Starts subtle, gets extreme |
| Position | Static | No position movement |
| Freeze Mode | Auto | Catches sustained notes |
| Threshold | 0.4 | Medium-high sensitivity |
| Fold Amount | 0.3 base, LFO modulated | Evolves over time |
| Fold Offset | 0.1 | Minimal asymmetry |
| Mix | 70% | Blend with original |

| LFO Setting | Value |
|-------------|-------|
| Rate | 0.05 Hz |
| Waveform | Triangle |
| Depth | Stretch: 0.7, Fold: 0.5 |
| Target | Stretch, Fold Amount |

### 3. Omnipotent Observers
**Character:** Thick fat psy pad with overlapping modulation zones, epiphany of sound

| Parameter | Value | Notes |
|-----------|-------|-------|
| Stretch | 0.3x | Strong stretch for density |
| Position | LFO modulated | Layered zones |
| Freeze Mode | Auto | Frequent captures |
| Threshold | 0.5 | Higher sensitivity |
| Fold Amount | 0.7 | High harmonic content |
| Fold Offset | 0.4 | Noticeable asymmetry |
| Mix | 90% | Almost fully processed |

| LFO Setting | Value |
|-------------|-------|
| Rate | 0.15 Hz |
| Waveform | Sine |
| Depth | 0.4 |
| Target | Position |

### 4. Infinite Cogs
**Character:** Interlocking evolution — mechanical motion dissolving into organic drift

| Parameter | Value | Notes |
|-----------|-------|-------|
| Stretch | 0.6x | Moderate stretch |
| Position | LFO modulated | Continuous mechanical motion |
| Freeze Mode | Auto | Catches transformation moments |
| Threshold | 0.35 | Balanced sensitivity |
| Fold Amount | 0.5 | Medium harmonics |
| Fold Offset | 0.3 | Moderate asymmetry |
| Mix | 85% | Strong processing |

| LFO Setting | Value |
|-------------|-------|
| Rate | 0.2 Hz |
| Waveform | Triangle |
| Depth | 0.35 |
| Target | Position |

---

## Interaction Details

### Freeze Trigger Mechanism

The freeze buffer is **always recording** — a circular buffer continuously captures the last 3 seconds of input audio (oldest replaced by newest).

When Freeze Mode = Auto:
1. Wavefolder output amplitude is continuously monitored
2. When amplitude crosses the threshold, a new freeze point is set at the current buffer position
3. Playback loops within the captured 3-second buffer
4. The buffer continues recording underneath (fresh audio available when next freeze triggers)
5. This creates cascading transformation layers — each freeze captures a different moment

### LFO Assignment

Each preset assigns LFO to specific parameters:
- **Rate:** How fast the parameter cycles
- **Depth:** How much the parameter varies (0 = no modulation, 1 = full range)
- **Target:** Which parameter(s) are modulated

Users can modify LFO assignments beyond presets.

### Mono → Stereo Processing

For mono input:
1. Mono signal is split to L and R channels
2. Fold Offset is applied differently per channel (creates width from asymmetric harmonics)
3. Post-fold stereo widening effect is applied
4. Result: Wide stereo image from mono source

---

## What Not To Do

- **Do not** add harsh distortion or aggressive clipping — maintain "never harsh" character
- **Do not** allow runaway feedback without limiting — soft-clip protects output
- **Do not** create interface complexity — keep all parameters visible and accessible
- **Do not** exceed CPU budget — optimize threshold detection and buffer operations
- **Do not** introduce latency beyond one buffer — live use must remain possible

---

## Implementation Notes

### Time-Stretch Algorithm
Use granular time-stretch with windowed overlap-add for efficiency. At extreme settings (0.1x, 10x), quality may vary but character is preserved.

### Freeze Buffer
Circular buffer of 3 seconds. When freeze is triggered, playback loops within the captured buffer. Position parameter scrubs through frozen audio.

### Wavefolding Implementation
Use tanh-based folding for musical harmonics. Fold Offset shifts the zero-crossing point before folding:
```
// Mono input: apply different offset per channel for stereo width
L: folded = tanh((input + (offset * stereo_spread)) * amount)
R: folded = tanh((input + (offset * -stereo_spread)) * amount)

// Stereo input: offset modulates each channel independently
L: folded = tanh((input_L + offset) * amount)
R: folded = tanh((input_R - offset) * amount)  // opposite sign creates width
```
This creates stereo width from mono input and enhances width from stereo input.

### Soft-Clip
Apply soft-clip after mix:
```
output = tanh(mixed_signal)
```
This ensures output never exceeds ±1 while remaining musical.

---

## Files

- **Spec File:** `docs/superpowers/specs/2026-04-02-psycog-design.md`
- **Playbook:** `/home/myuser/agents/juce-agent/playbooks/v2/vst-plugin-playbook.json`

---

## Approval

- [x] Plugin type and signal flow defined
- [x] Sound identity documented
- [x] Gain staging philosophy explicit
- [x] Technical constraints locked
- [x] Core presets named with character descriptions
- [x] All parameter gaps resolved
- [x] Interaction cascade defined

**Status:** Ready for Phase 2 — Architecture & Implementation Planning