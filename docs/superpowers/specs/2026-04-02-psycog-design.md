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

**Freeze Buffer:** 3 seconds of audio

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

### LFO Modulation

Built-in LFO for preset parameter modulation:
- **Rate:** 0.01 – 20 Hz (how fast the parameter cycles)
- **Waveform:** Sine, Triangle, Square, Random S&H
- **Depth:** 0 – 1 per assigned parameter (0 = no modulation, 1 = full range)
- **Targets:** Any combination of: Stretch, Position, Fold Amount, Fold Offset

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
| **Latency** | One buffer (safety) |
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

### 2. Golden Memories
**Character:** Warm keys that transform to psychedelic when held, responds to note duration

| Parameter | Value | Notes |
|-----------|-------|-------|
| Stretch | 0.8x → 0.2x (LFO) | Starts subtle, gets extreme |
| Position | Static | No position movement |
| Freeze Mode | Auto | Catches sustained notes |
| Threshold | 0.4 | Medium-high sensitivity |
| Fold Amount | 0.3 → 0.6 (LFO) | Evolves over time |
| Fold Offset | 0.1 | Minimal asymmetry |
| Mix | 70% | Blend with original |

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

---

## Interaction Details

### Freeze Trigger Mechanism

When Freeze Mode = Auto:
1. Wavefolder output amplitude is continuously monitored
2. When amplitude crosses the threshold, a new 3-second buffer is captured
3. Playback continues from the new freeze point
4. This creates cascading transformation layers

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
folded = tanh((input + offset) * amount)
```
Apply different offset values for L and R channels to create stereo width.

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