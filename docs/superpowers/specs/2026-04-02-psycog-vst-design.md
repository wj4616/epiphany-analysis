# Psycog VST - Plugin Specification

**Version:** 1.5  
**Date:** 2026-04-02  
**Status:** Approved for Implementation (post-audit revision)

---

## Overview

**Name:** Psycog VST  
**Type:** Multi-effect module (psychedelic audio transformation)  
**Tagline:** Interdimensional sound transformation — constantly morphing, never harsh

Psycog VST is a creative audio transformation module that combines time manipulation with wavefolding to create vivid, psychedelic sounds. The effect is so prominent that it functions more as a module than a traditional effect — transforming any input into something entirely new.

---

## Sound Identity

**Character:** Constantly morphing, never settling, never harsh  
**Reference Universe:** Ambient/psybient (Shpongle, Ott, Carbon Based Lifeforms, Bluetech) + Glitch/experimental (Aphex Twin, Autechre)  
**Core Transformation:** Time-stretch/freeze feeding into wavefolding with bidirectional interaction

The plugin creates evolving soundscapes that transform continuously. When the wavefolder's output amplitude crosses a threshold, it triggers new freeze points — creating cascading layers of transformation that never repeat exactly the same way twice.

---

## Signal Flow

```
Audio Input
    ├──── (dry path: 2048-sample delay) ──────────────┐
    ↓                                                 │
┌─────────────────────────────────────┐               │
│     Time-Stretch / Freeze Stage     │               │
│  - Stretch: 0.1x to 10x            │               │
│  - Position: Buffer position        │               │
│  - Freeze: Manual / Auto / Off      │               │
│  - Double-buffered: 3 sec each      │               │
│  ◄── freeze trigger (from below)    │               │
└─────────────────────────────────────┘               │
    ↓                                                 │
┌─────────────────────────────────────┐               │
│        Wavefolding Stage            │               │
│  - Fold Amount (drive 1x–10x)       │               │
│  - Fold Offset (DC offset)          │               │
│  - Triangle fold: reflects at ±1    │               │
│  - Different offset L/R for stereo  │               │
│  ──► threshold detect ──────────────── (to above)   │
└─────────────────────────────────────┘               │
    ↓                                                 │
┌─────────────────────────────────────┐               │
│      Wet Output Processing          │               │
│  - Soft-clip: tanh(signal)          │               │
│  - Auto-normalize: RMS to -6dBFS   │               │
│  - Noise gate: bypass below -60dBFS │               │
└─────────────────────────────────────┘               │
    ↓                                                 │
┌─────────────────────────────────────┐               │
│           Mix Stage                 │◄──────────────┘
│  - Wet/Dry: 0-100%                  │
│  - 0% = dry only (fully transparent)│
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│        Final Output Protection      │
│  - Soft-clip: tanh(output)          │
│  - Catches hot input passthrough    │
└─────────────────────────────────────┘
    ↓
Audio Output
```

### Interaction Cascade

The wavefolder's **output amplitude** (measured directly at wavefolder output, before wet processing and mix) can trigger new freeze points:
- When Freeze mode = Auto, crossing the threshold captures a new 3-second buffer
- This creates cascading transformation layers
- Never repeats exactly the same way twice
- At high fold + low threshold, triggers may occur every 100ms (cooldown floor) — this rapid buffer-switching is intended behavior, producing rhythmic cascading artifacts

---

## Parameters

### Time Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Stretch** | 0.1x – 10x (log scale, centre = 1.0x) | Time stretch amount. 0.1x = extreme compression, 10x = extreme stretch. Logarithmic knob: 50% = 1.0x, equal travel for speedup and slowdown. When Freeze = Off, uses real-time granular stretch on incoming audio (requires grain buffer, see Latency). When frozen, controls playback speed of frozen buffer. |
| **Position** | 0 – 1 | Buffer position (where playhead reads). Active only when frozen (Manual/Auto modes). Ignored in Off mode. |
| **Freeze Mode** | Manual / Auto / Off | Manual = toggle button (press to freeze, press again to unfreeze). Auto = threshold triggers freeze. Off = no freeze |
| **Threshold** | 0 – 1 | Sensitivity for auto-trigger. Only active in Auto mode. Visible but dimmed in other modes. 100ms cooldown between triggers prevents rapid re-triggering. Note: at 0, any non-silent input triggers freeze continuously (chaos mode). At 1, only full-scale peaks trigger — effectively disabled for most material. |

**Freeze Buffer:** 3 seconds of audio using **double-buffering** — two buffers, but only one records at a time. The recording buffer is always active regardless of freeze mode (Off, Manual, or Auto) so that switching modes via automation always has valid buffer content. Buffer A records incoming audio while Buffer B holds frozen content (read-only). On freeze trigger: A stops recording and becomes the playback buffer; B starts recording fresh audio. On the next trigger, they swap again. This ensures recording never overwrites audio being played back.

**Stretch During Freeze:** When frozen, Stretch controls playback speed of the frozen buffer (creates pitch-shift effect on frozen audio).

### Wavefolding Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Fold Amount** | 0 – 1 | Intensity of wavefolding. Maps to drive multiplier [1.0, 10.0]. At 0: drive=1.0, signal passes through unchanged (offset is also zeroed — see below). At 1: drive=10.0, aggressive folding with rich harmonics. |
| **Fold Offset** | -1 – 1 | DC offset for asymmetric folding. Scaled by fold_amount: `effective_offset = offset * fold_amount`. At fold_amount=0, offset has no effect (true passthrough). Zero = symmetric (odd harmonics). Non-zero = asymmetric (odd + even harmonics). Different values for L/R create stereo width. |

### Output Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Soft-clip** | (internal, wet path) | `tanh(signal)` — applied to wet signal before auto-normalize. Tames peaks for stable RMS measurement. Auto-normalize may boost above ±1 after this; final soft-clip catches that. |
| **Auto-normalize** | (internal, wet path) | RMS-based, target -6dBFS, 300ms attack / 1000ms release. Applied to wet signal after soft-clip, before mix. Bypassed when wet RMS is below -60dBFS (noise gate). |
| **Mix** | 0 – 100% | Wet/dry blend. 0% = dry only (fully transparent, no processing on dry path), 100% = fully processed |
| **Final soft-clip** | (internal, post-mix) | `tanh(output)` — applied after mix. Catches hot input signals passing through the dry path. Nearly transparent below ±0.5 (< 0.04dB deviation). |

**Signal path:** Wet → Soft-clip → Auto-normalize → Mix with Dry (delay-compensated) → Final soft-clip → Output. Dry path includes a 2048-sample delay matching the wet path's granular latency, preventing comb filtering at partial mix settings. At 0% mix, dry signal passes through delay + final soft-clip only (transparent at normal levels).

### LFO Stage

| Parameter | Range | Description |
|-----------|-------|-------------|
| **LFO Rate** | 0.01 – 20 Hz (log scale, centre = ~0.5 Hz) | How fast the modulated parameter cycles. Logarithmic knob: slow rates get more knob travel. |
| **LFO Waveform** | Sine, Triangle, Square, Random S&H | Shape of modulation |
| **LFO Depth** | 0 – 1 | Modulation intensity (0 = none, 1 = full range of target parameter) |
| **LFO Target** | Stretch, Position, Fold Amount, Fold Offset (any combination) | Which parameter(s) are modulated. Single shared LFO — all selected targets receive the same rate, waveform, and depth. |

Each preset defines default LFO assignments. Users can modify rate, waveform, depth, and targets. Note: this is intentionally a single LFO for simplicity. All selected targets share the same modulation shape.

### LFO Modulation Model

LFO modulation operates on each parameter's **normalized internal scale** (0.0–1.0). For parameters with logarithmic mapping (Stretch, LFO Rate), this means modulation is perceptually even above and below the base value.

```
// Modulation formula:
normalized_base = to_normalized(base_value)        // e.g., Stretch 1.0x → 0.5
lfo_output = lfo_waveform(phase)                   // bipolar: -1.0 to +1.0
modulated = normalized_base + lfo_output * depth    // e.g., 0.5 + (-0.3) = 0.2
modulated = clamp(modulated, 0.0, 1.0)
final_value = from_normalized(modulated)            // e.g., 0.2 → ~0.25x Stretch
```

Example — "Golden Memories" preset: Stretch = 0.8x (normalized ~0.45 on log scale), depth = 0.6:
- LFO sweeps normalized from 0.45 ± 0.6 = clamped to [0.0, 1.0]
- Maps back to Stretch range: roughly 0.1x to 6.5x, centred on 0.8x
- Log scale ensures equal perceptual modulation above and below base

### Parameter Smoothing

All continuously-variable parameters use `SmoothedValue` (20ms ramp) to prevent zipper noise from knob changes and LFO modulation:
- **Stretch:** 20ms smoothing (prevents granular engine glitches from rapid changes)
- **Position:** 20ms smoothing (prevents playhead jumps/clicks in frozen buffer)
- **Fold Amount / Fold Offset:** 20ms smoothing (prevents wavefolding discontinuities)
- **Mix:** 20ms smoothing (prevents blend clicks)
- **Freeze Mode:** Not smoothed (discrete state), but transitions use 10ms crossfade (see Freeze Trigger Mechanism)

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
| **Sample Rates** | 44.1 kHz, 48 kHz (primary). 88.2 kHz, 96 kHz supported but not optimized — CPU budget may exceed 15% at high rates. |
| **Buffer Size** | 128 samples minimum (live use possible) |
| **Latency** | One grain window (~46ms @ 48kHz, 2048-sample grains). Granular time-stretch requires a full grain window of look-ahead for overlap-add. Report latency to host via `setLatencySamples()`. |
| **CPU Budget** | < 15% single core |
| **Automation** | All parameters DAW-automatable |

### CPU Breakdown (Estimated)

| Component | CPU Cost |
|-----------|----------|
| Buffer recording | ~1% |
| Time-stretch (granular) | ~3-5% |
| Threshold detection | <1% |
| Wavefolding | ~1-2% |
| LFO (single, multi-target) | <1% |
| Auto-normalize (RMS) | ~1-2% |
| Soft-clip (wet + final) | <1% |
| Dry path delay (2048 samples) | <1% |
| **Total** | **~8-13%** |

---

## GUI Layout

**Style:** Moderate — all parameters visible, organized sections

```
┌─────────────────────────────────────────────────────────┐
│  PSYCOG VST                          [Preset Name]      │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌───────────────────┐    ┌───────────────────┐        │
│  │   TIME-STRETCH    │    │   WAVEFOLDING    │        │
│  │                   │    │                  │        │
│  │  Stretch  [0.5x]  │    │  Fold Amt [0.4]  │        │
│  │  Position [────]  │    │  Fold Off [──0.2]│        │
│  │  Mode    [Auto]   │    │                  │        │
│  │  Threshold [0.3] │    │                  │        │
│  │  [■ FREEZE]       │    │                  │        │
│  └───────────────────┘    └───────────────────┘        │
│                                                         │
│  ┌───────────────────┐    ┌───────────────────┐        │
│  │      LFO          │    │     OUTPUT        │        │
│  │                   │    │                   │        │
│  │  Rate   [0.5 Hz]  │    │  Mix    [80%]     │        │
│  │  Wave  [Sine]     │    │                   │        │
│  │  Depth  [0.5]     │    │                   │        │
│  │  [☐Str][☑Pos]     │    │                   │        │
│  │  [☐Amt][☐Off]     │    │                   │        │
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
**Character:** Warm keys that transform to psychedelic when held — sustained amplitude triggers freeze cascade (amplitude-based, not MIDI-aware)

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
| Depth | 0.6 |
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

The freeze system uses **double-buffering** — two 3-second circular buffers, only one recording at a time. See Freeze Buffer implementation for swap mechanics.

**All freeze/unfreeze transitions use a 10ms crossfade** to prevent clicks — this applies to Manual toggle, Auto triggers, and mode switches. This preserves "never harsh" across all state changes.

**Manual Mode:**
- Toggle button: press once to freeze (10ms crossfade to frozen buffer), press again to unfreeze (10ms crossfade back to live audio)
- Position parameter scrubs through frozen buffer

**Auto Mode:**
1. Wavefolder output amplitude is monitored (measured at wavefolder output, before wet processing)
2. When amplitude crosses threshold, freeze triggers (10ms crossfade to frozen buffer)
3. **100ms cooldown** prevents rapid re-triggering from same peak
4. Playback loops within the frozen 3-second buffer indefinitely until the next trigger
5. Position parameter scrubs through frozen playback buffer
6. Other buffer records fresh audio (only one buffer records at a time)
7. On next trigger: buffers swap (10ms crossfade), new frozen content plays, old playback buffer starts recording

**Off Mode:**
- No freeze, position parameter ignored
- Audio passes through time-stretch only

### LFO Assignment

Each preset assigns LFO to specific parameters:
- **Rate:** How fast the parameter cycles
- **Depth:** How much the parameter varies (0 = no modulation, 1 = full range)
- **Target:** Which parameter(s) are modulated

Users can modify LFO assignments beyond presets.

### Mono → Stereo Processing

For mono input:
1. Mono signal is split to L and R channels
2. Fold Offset is applied with opposite signs per channel (`+offset` for L, `-offset` for R)
3. Asymmetric folding with opposite offsets inherently creates stereo width — no additional mid/side processing needed
4. Result: Wide stereo image from mono source

---

## What Not To Do

- **Do not** add harsh distortion or aggressive clipping — maintain "never harsh" character
- **Do not** allow runaway feedback without limiting — soft-clip protects output
- **Do not** create interface complexity — keep all parameters visible and accessible
- **Do not** exceed CPU budget — optimize threshold detection and buffer operations
- **Do not** introduce latency beyond one grain window (~46ms) — report actual latency to host via `setLatencySamples()`

---

## Implementation Notes

### Time-Stretch Algorithm
Use granular time-stretch with windowed overlap-add (2048-sample Hann window) for efficiency. Behavior varies by stretch value:
- **Stretch > 1x (expansion):** Grains overlap more, audio slows down. Smooth, lush character.
- **Stretch = 1x:** Normal speed. Grains reconstruct original timing. ~46ms latency still applies.
- **Stretch < 1x (compression):** Grains skip ahead through input, discarding intermediate audio. At 0.1x, ~90% of input is skipped. Produces glitchy, fragmented character — this is intentional and fits the psychedelic aesthetic.

At extreme settings (0.1x, 10x), quality may vary but character is preserved.

### Freeze Buffer
**Double-buffered:** two 3-second circular buffers (A and B), only one recording at a time. Initial state: A records, B is idle. On first freeze trigger: A stops recording and becomes playback buffer (read-only); B starts recording fresh audio. On next trigger: B stops recording and becomes playback; A starts recording. This swap pattern continues. Only one buffer ever writes at a time, preventing recording from corrupting playback audio. Position parameter scrubs through the frozen playback buffer.

### Wavefolding Implementation
Use **triangle wavefolding** — the signal reflects back at ±1 boundaries, producing rich harmonics that increase with drive. This is distinct from tanh saturation (which compresses but never folds).

```
// Triangle fold function: reflects signal at ±1 boundaries
// Input can be any value; output is always in [-1, 1]
triangle_fold(x):
    x = fmod(x + 1, 4)          // wrap to period [0, 4)
    if x < 0: x += 4
    return (x < 2) ? (x - 1) : (3 - x)

// Drive mapping: fold_amount [0, 1] → drive [1.0, 10.0]
// At fold_amount=0: drive=1.0, no folding (signal passes through)
// At fold_amount=1: drive=10.0, aggressive folding
drive = 1.0 + fold_amount * 9.0

// Offset scales with fold_amount so fold_amount=0 is true passthrough
effective_offset = offset * fold_amount

// Stereo input: offset modulates each channel independently
L: folded = triangle_fold((input_L + effective_offset) * drive)
R: folded = triangle_fold((input_R - effective_offset) * drive)

// Mono input: split to L/R with opposite offset for stereo width
L: folded = triangle_fold((input + effective_offset) * drive)
R: folded = triangle_fold((input - effective_offset) * drive)
```

Fold Offset shifts the zero-crossing before folding: at zero offset, folding is symmetric (odd harmonics). Non-zero offset creates asymmetric folding (odd + even harmonics). Opposite offset signs for L/R channels create stereo width.

### Dry Path Delay
The dry path includes a 2048-sample delay line matching the wet path's granular latency. Without this, mixing dry and wet at partial settings (e.g., 70% mix) creates a ~46ms time offset between them, causing comb filtering with destructive nulls at ~11Hz intervals. The delay is a simple circular buffer read — negligible CPU.

### Soft-Clip
Two stages:
1. **Wet soft-clip** — applied to wet signal before auto-normalize. Tames peaks for stable RMS measurement. Auto-normalize may boost above ±1 after this; final soft-clip catches that.
```
wet_clipped = tanh(wet_signal)
```
2. **Final soft-clip** — applied after mix, catches both auto-normalized wet peaks and hot dry signals:
```
output = tanh(mixed_signal)
```
The final soft-clip ensures output never exceeds ±1 regardless of input level. At normal levels (below ±0.5), tanh introduces < 0.04dB deviation — effectively transparent. At 0% mix with normal input levels, audio passes through delay + final soft-clip with negligible coloration.

### Auto-Normalize
RMS-based leveling applied to wet signal after soft-clip, before mix:
- **Target:** -6dBFS RMS
- **Attack:** 300ms (slow enough to avoid pumping)
- **Release:** 1000ms (smooth decay)
- **Noise gate:** Bypassed when wet RMS is below -60dBFS. This prevents amplifying silence/noise floor between notes and avoids breathing artifacts.
- Measures RMS over a sliding window matched to attack time
- Computes gain adjustment: `gain = target_rms / current_rms` (clamped to 0.1–3.0, i.e., max ~10dB boost)
- Smooths gain changes with the attack/release envelope

---

## Files

- **Spec File:** `docs/superpowers/specs/2026-04-02-psycog-vst-design.md`
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