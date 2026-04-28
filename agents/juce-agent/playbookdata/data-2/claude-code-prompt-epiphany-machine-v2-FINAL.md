# Claude Code Prompt: "Epiphany Machine" v2 FINAL — Complete Specification

Copy everything below the line into Claude Code.

---

## Project: "Epiphany Machine" v2 — Ambient Glitch + Texture/Body Dual Filter VST3 Plugin

This is the FINAL v2 specification. If you have already built Epiphany Machine v1, use that as the starting point and add the new features. If starting fresh, use the Alien Machine codebase as the base and apply ALL modifications.

I'm on Linux Mint, using JUCE + CMake (no Projucer). I know Python/JS but am learning C++. Please add comments explaining non-obvious C++ and JUCE patterns.

### What this plugin does

Epiphany Machine creates deep, evolving stereo soundscapes from synthesizer input — blending lush ambient diffusion with rhythmic glitch artifacts. Designed for **live stereophonic synthesizer performance** creating ambient droning music. Primary input: long evolving pads, drones, textural synth patches.

### Complete feature set:
1. Ambient glitch engine with diffusion network, pitch-shifting feedback, stereo widening
2. Texture/Body signal split — separates evolving timbral movement from stable drone foundation
3. Dual filters (Cytomic TPT SVF) — independent filtering of texture and body streams
4. Glitch Focus routing — controls whether texture or body feeds the glitch engine
5. Freeze — captures and infinitely sustains the current reverb tail
6. Preset system — 6 factory presets + INIT, with smooth morphing between them
7. Master bypass
8. Cyberpunk visual theme — deep purple dominant, gold accents, signal-flow GUI layout

---

## ARCHITECTURE OVERVIEW

```
Input (mono or stereo → converted to stereo)
  │
  ├──→ Texture/Body Detector [INPUT] (dual envelope follower)
  │       ├── textureSignal
  │       └── bodySignal
  │
  ├──→ GLITCH FOCUS crossfade → Glitch Engine input
  ├──→ MORPH crossfade: blend [clean] and [Glitch Engine output]
  │
  → (+) summing junction ←── Feedback Return
  → [1] Diffusion Network (4-line FDN)
  │       └── FREEZE: when active, stops new input, sets feedback to ~0.999
  → [2] Stereo Widener
  → Wet signal
  │
  ├──→ Feedback Send → [3] Pitch Drifter → LPF → Soft clip
  │       → EXTRA soft clip during FREEZE → back to summing junction
  │
  ├──→ FILTER POSITION SWITCH (Off / Wet / Final)
  │       If Wet: run Texture/Body Detector [WET] on wet signal, then DualFilter
  │       If Final: run Texture/Body Detector [FINAL] on mixed signal, then DualFilter
  │
  → [4] Texture/Body Dual Filter (when active, using LOCAL detector output)
  → MIX crossfade with dry input → Final Output (always stereo)
```

### KEY CHANGE FROM PREVIOUS SPEC: The Dual Filter uses its OWN detection pass on whatever signal it actually processes, not the input detector's values. The input detector feeds GLITCH FOCUS routing. The filter detector feeds the Dual Filter. This ensures accurate texture/body separation at every point in the chain.

---

## TEXTURE/BODY DETECTOR

Separates signal into "texture" (evolving, moving content) and "body" (stable, sustained content). Tuned for synthesizer pads and drones. Used in TWO places — create TWO instances of this class.

### Implementation:

Detection operates on the mono sum of L+R for analysis, gains applied to stereo signal.

- **Fast envelope follower**: attack ~5ms, release ~50ms
- **Slow envelope follower**: attack ~100ms, release ~500ms
- **One-pole envelope follower formula**:
  ```cpp
  // In prepare():
  float attackCoeff = expf(-1.0f / (attackTimeSeconds * sampleRate));
  float releaseCoeff = expf(-1.0f / (releaseTimeSeconds * sampleRate));
  
  // Per sample:
  float inputLevel = fabsf(monoSample);
  if (inputLevel > envelope)
      envelope = attackCoeff * envelope + (1.0f - attackCoeff) * inputLevel;
  else
      envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * inputLevel;
  ```

- **Texture detection with SENSITIVITY**:
  ```cpp
  float rawTexture = fastEnvelope - slowEnvelope;
  float sensitivityGain = 1.0f + sensitivity * 7.0f;  // maps 0–1 to 1x–8x
  float textureGain = juce::jlimit(0.0f, 1.0f, rawTexture * sensitivityGain);
  // Smooth to avoid chattering (one-pole LPF, ~5ms)
  textureGain = smoothCoeff * prevTextureGain + (1.0f - smoothCoeff) * textureGain;
  ```

- **Signal separation** (per-sample, both channels):
  ```cpp
  textureL = inputL * textureGain;
  textureR = inputR * textureGain;
  bodyL = inputL * (1.0f - textureGain);
  bodyR = inputR * (1.0f - textureGain);
  ```

- Outputs: texture buffer (stereo), body buffer (stereo), per-sample textureGain array
- Parameter: **SENSITIVITY** ("sensitivity"), 0.0–1.0, default 0.5
- **TWO INSTANCES in PluginProcessor**: `inputDetector` (for Glitch Focus routing) and `filterDetector` (for Dual Filter). Both share the same SENSITIVITY parameter value but operate on different signals.

### Create as: `src/DSP/TextureBodyDetector.h/.cpp`

---

## DUAL FILTER — CYTOMIC TPT SVF (CRITICAL: DO NOT USE CHAMBERLIN SVF)

Two independent multimode filters using the **Cytomic Trapezoidal SVF** (Andy Simper's topology). This is the industry standard for modulation-safe filtering. The naive SVF (`f = 2*sin(...)`) is UNSTABLE at high frequencies and CLICKS during fast cutoff modulation — DO NOT USE IT.

### Cytomic TPT SVF implementation (per sample, per channel):

```cpp
// Coefficient calculation (can be done once per sample or once per block):
float g = tanf(juce::MathConstants<float>::pi * cutoffHz / sampleRate);
float k = 1.0f / Q;  // Q ranges 0.5 to 15.0
float a1 = 1.0f / (1.0f + g * (g + k));
float a2 = g * a1;
float a3 = g * a2;

// Per-sample processing (ic1eq and ic2eq are the filter state, init to 0):
float v3 = input - ic2eq;
float v1 = a1 * ic1eq + a2 * v3;
float v2 = ic2eq + a2 * ic1eq + a3 * v3;
ic1eq = 2.0f * v1 - ic1eq;
ic2eq = 2.0f * v2 - ic2eq;

// All outputs available simultaneously:
float lp = v2;
float bp = v1;
float hp = input - k * bp - lp;
// Select output based on filter type parameter
```

### Why this matters:
- The `tan()` coefficient (vs `sin()` in the Chamberlin SVF) provides correct frequency mapping at all frequencies
- The trapezoidal integration makes it inherently stable — no blowups near Nyquist
- Fast modulation of cutoff produces NO clicks or pops
- Same computational cost as the naive SVF

### Filter A (TEXTURE): type, cutoff, resonance, LFO
- LFO: sine wave, rate 0.01–10.0 Hz, depth 0.0–1.0
- LFO modulation:
  ```cpp
  float lfoValue = sinf(lfoPhase);  // -1 to +1
  float modulatedCutoff = baseCutoff * powf(2.0f, lfoValue * lfoDepth * 2.0f);
  modulatedCutoff = juce::jlimit(20.0f, 20000.0f, modulatedCutoff);
  ```
- Default: LP, 2000 Hz, resonance 0.3, LFO off

### Filter B (BODY): type, cutoff, resonance. NO LFO.
- Default: LP, 1200 Hz, resonance 0.2

### Internal structure: 4 SVF state sets (Texture L, Texture R, Body L, Body R), each stores ic1eq, ic2eq

### Recombination: `output = filteredTexture + filteredBody`

### IMPORTANT: The DualFilter receives its texture/body split from the `filterDetector` instance (not the `inputDetector`). In processBlock:
```cpp
// If filter is active:
filterDetector.process(signalToFilter);  // wet or final signal
dualFilter.process(filterDetector.getTextureBuffer(), 
                   filterDetector.getBodyBuffer(), outputBuffer);
```

### Create as: `src/DSP/DualFilter.h/.cpp`

---

## GLITCH ENGINE — GLITCH FOCUS ROUTING

Uses `inputDetector` output. The GlitchEngine's internal processing is unchanged from Alien Machine.

```cpp
float glitchInputL, glitchInputR, cleanPassL, cleanPassR;
if (glitchFocus <= 0.5f) {
    float t = glitchFocus * 2.0f;
    glitchInputL = bodyL * (1.0f - t) + inputL * t;
    glitchInputR = bodyR * (1.0f - t) + inputR * t;
    cleanPassL = textureL * (1.0f - t);
    cleanPassR = textureR * (1.0f - t);
} else {
    float t = (glitchFocus - 0.5f) * 2.0f;
    glitchInputL = inputL * (1.0f - t) + textureL * t;
    glitchInputR = inputR * (1.0f - t) + textureR * t;
    cleanPassL = bodyL * t;
    cleanPassR = bodyR * t;
}
```

---

## FREEZE

Modifies DiffusionNetwork behavior:

- ID: "freeze", bool, default false
- When ON:
  1. Input gain → 0.0 (stop feeding new audio)
  2. Internal feedback gain → 0.999 (near-infinite sustain, overrides DECAY)
  3. Damping coefficient → 0.0 (disable HF absorption so frozen tail doesn't darken)
  4. **SAFETY: Add extra soft-clip in the feedback path during freeze**: `x = x / (1.0f + fabsf(x))`. This prevents runaway energy buildup when DRIFT and FEEDBACK are both active on a frozen tail.
- All transitions use SmoothedValue over ~2048 samples (~42ms)
- When OFF: normal operation resumes, frozen tail naturally decays

### DiffusionNetwork changes:
```cpp
void setFreezeActive(bool frozen);
// Internally manages SmoothedValues for:
//   freezeInputGain (1.0 ↔ 0.0)
//   freezeFeedbackGain (normal DECAY ↔ 0.999)
//   freezeDampingOverride (normal ↔ 0.0)
//   freezeExtraClip (false ↔ true)
```

---

## MASTER BYPASS

- ID: "bypass", bool, default false
- SmoothedValue crossfade over ~512 samples
- When fully bypassed, skip ALL DSP to save CPU

---

## FILTER POSITION SWITCH

- ID: "filterPos", Choice: "Off"/"Wet"/"Final", default "Off"
- Off: skip DualFilter + filterDetector entirely
- Wet: filterDetector runs on wet signal, DualFilter processes result, THEN MIX
- Final: MIX first, THEN filterDetector + DualFilter on combined output
- Check ONCE per processBlock

---

## PRESET SYSTEM WITH SMOOTH MORPHING

7 slots: INIT + 6 factory presets.

### PresetData struct:
```cpp
struct PresetData {
    juce::String name;
    float morph, mix, density, chaos, decay, damping, drift, width, feedback;
    float glitchFocus, sensitivity;
    int filterPos, texFilterType;
    float texFilterCutoff, texFilterReso, texLfoRate, texLfoDepth;
    int bodyFilterType;
    float bodyFilterCutoff, bodyFilterReso;
    // freeze and bypass are NOT stored in presets — always false
};
```

### Parameters:
- **PRESET**: "preset", Choice with 7 options: "INIT", "Void Meditation", "Neural Storm", "Phantom Drift", "Crystal Lattice", "Deep Submersion", "Solar Flare"
- **MORPH TIME**: "morphTime", 0.1–10.0 seconds, default 2.0

### Smooth morphing:
- On preset change: capture current values as "start", target preset as "end"
- Per block: advance `morphProgress += blockSize / (morphTimeSec * sampleRate)`, clamp to 1.0
- **Float parameters**: interpolate linearly, EXCEPT frequency parameters which interpolate in log space: `currentFreq = expf(lerp(logf(startFreq), logf(endFreq), progress))`
- **MIX parameter**: use equal-power crossfade: `mix = startMix + (endMix - startMix) * (0.5f - 0.5f * cosf(progress * PI))` — prevents audible dip in the middle
- **Choice parameters** (filter type, filter position): snap at 50% morph progress
- **Bool parameters** (bypass, freeze): NOT morphed, snap instantly
- **User override**: if user moves a knob during morph, remove that parameter from interpolation
- Write interpolated values using `parameter->setValueNotifyingHost()` once per block

---

## THE 6 FACTORY PRESETS

### Preset 1: "Void Meditation"
*Vast, motionless dark ambient. An infinite abyss.*
morph=0.0, mix=0.7, density=0.1, chaos=0.1, decay=0.92, damping=0.8, drift=0, width=0.7, feedback=0.2, glitchFocus=0.5, sensitivity=0.5, filterPos=1(Wet), texType=LP, texCutoff=1500, texReso=0.15, texLfoRate=0.05, texLfoDepth=0.2, bodyType=LP, bodyCutoff=700, bodyReso=0.1

### Preset 2: "Neural Storm"
*Aggressive glitch chaos. Rhythmic stuttering into a dense, pulsing field.*
morph=0.85, mix=0.55, density=0.8, chaos=0.75, decay=0.5, damping=0.25, drift=5, width=0.85, feedback=0.55, glitchFocus=1.0, sensitivity=0.7, filterPos=1(Wet), texType=BP, texCutoff=2200, texReso=0.55, texLfoRate=2.5, texLfoDepth=0.5, bodyType=LP, bodyCutoff=3000, bodyReso=0.15

### Preset 3: "Phantom Drift"
*Slowly ascending shimmer. Sounds rise in pitch and dissolve into harmonics.*
morph=0.15, mix=0.6, density=0.2, chaos=0.15, decay=0.8, damping=0.15, drift=12, width=0.65, feedback=0.6, glitchFocus=0.5, sensitivity=0.5, filterPos=1(Wet), texType=HP, texCutoff=1800, texReso=0.2, texLfoRate=0.08, texLfoDepth=0.3, bodyType=LP, bodyCutoff=5000, bodyReso=0.1

### Preset 4: "Crystal Lattice"
*Delicate, mathematical. Precise micro-glitches in geometric patterns.*
morph=0.45, mix=0.45, density=0.6, chaos=0.1, decay=0.55, damping=0.2, drift=7, width=0.5, feedback=0.35, glitchFocus=0.0, sensitivity=0.4, filterPos=2(Final), texType=BP, texCutoff=3500, texReso=0.45, texLfoRate=0.2, texLfoDepth=0.15, bodyType=BP, bodyCutoff=800, bodyReso=0.35

### Preset 5: "Deep Submersion"
*Underwater cathedral. Dark, low, massive depth.*
morph=0.25, mix=0.75, density=0.25, chaos=0.35, decay=0.88, damping=0.85, drift=-5, width=0.55, feedback=0.5, glitchFocus=0.3, sensitivity=0.6, filterPos=2(Final), texType=LP, texCutoff=900, texReso=0.25, texLfoRate=0.03, texLfoDepth=0.4, bodyType=LP, bodyCutoff=500, bodyReso=0.3

### Preset 6: "Solar Flare"
*Explosive, bright, wide. Neon harmonics across the stereo field.*
morph=0.6, mix=0.5, density=0.45, chaos=0.5, decay=0.65, damping=0.05, drift=12, width=1.0, feedback=0.7, glitchFocus=0.8, sensitivity=0.8, filterPos=1(Wet), texType=HP, texCutoff=2500, texReso=0.4, texLfoRate=1.2, texLfoDepth=0.35, bodyType=HP, bodyCutoff=400, bodyReso=0.2

---

## COMPLETE PARAMETER LIST (25 parameters)

### Core DSP (9 — unchanged IDs from Alien Machine):
| Parameter | ID | Range | Default |
|-----------|----|-------|---------|
| MORPH | "morph" | 0.0–1.0 | 0.3 |
| MIX | "mix" | 0.0–1.0 | 0.5 |
| DENSITY | "density" | 0.0–1.0 | 0.3 |
| CHAOS | "chaos" | 0.0–1.0 | 0.2 |
| DECAY | "decay" | 0.0–1.0 | 0.6 |
| DAMPING | "damping" | 0.0–1.0 | 0.5 |
| DRIFT | "drift" | -12 to +12 | 0 |
| WIDTH | "width" | 0.0–1.0 | 0.6 |
| FEEDBACK | "feedback" | 0.0–0.85 | 0.3 |

### Epiphany Machine additions (16):
| Parameter | ID | Range | Default |
|-----------|----|-------|---------|
| BYPASS | "bypass" | bool | false |
| FREEZE | "freeze" | bool | false |
| GLITCH FOCUS | "glitchFocus" | 0.0–1.0 | 0.5 |
| SENSITIVITY | "sensitivity" | 0.0–1.0 | 0.5 |
| FILTER POS | "filterPos" | Choice: Off/Wet/Final | Off |
| TEXTURE TYPE | "texFilterType" | Choice: LP/HP/BP | LP |
| TEXTURE CUTOFF | "texFilterCutoff" | 20–20000 Hz | 2000 |
| TEXTURE RESO | "texFilterReso" | 0.0–1.0 | 0.3 |
| TEXTURE LFO RATE | "texLfoRate" | 0.01–10.0 Hz | 0.5 |
| TEXTURE LFO DEPTH | "texLfoDepth" | 0.0–1.0 | 0.0 |
| BODY TYPE | "bodyFilterType" | Choice: LP/HP/BP | LP |
| BODY CUTOFF | "bodyFilterCutoff" | 20–20000 Hz | 1200 |
| BODY RESO | "bodyFilterReso" | 0.0–1.0 | 0.2 |
| PRESET | "preset" | Choice: 7 options | INIT |
| MORPH TIME | "morphTime" | 0.1–10.0 sec | 2.0 |

### Parameter notes:
- Frequency: `NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f)` for log scaling
- Resonance 0–1 maps to Q 0.5–15.0: `Q = 0.5f + resonance * 14.5f`
- Original Alien Machine IDs are preserved exactly

---

## GUI DESIGN — CYBERPUNK THEME, SIGNAL-FLOW LAYOUT

Window size: **950w x 700h pixels**

### Color Palette:
```
BACKGROUND:         #0d0a1a
SURFACE:            #1a1430
SURFACE_RAISED:     #241e3d
NEON_PRIMARY:       #b44aff
NEON_SECONDARY:     #7b2ff2
NEON_GLOW:          #d88aff
GOLD:               #f0c040
GOLD_DIM:           #a08030
TEXT_PRIMARY:        #e8e0f0
TEXT_SECONDARY:      #8878a0
DANGER:             #ff3060
FILTER_ACCENT:      #6a3aff
```

### Design Principles:
- No pure white or pure black — everything has a purple tint
- Gold for interactive highlights: active preset, freeze glow, value readouts, knob indicators
- Neon purple for structure: knob rings, section dividers
- MORPH knob ring: transitions from NEON_SECONDARY (0.0) → NEON_PRIMARY (0.5) → GOLD (1.0)
- **Font**: JUCE's built-in sans-serif. Title "EPIPHANY MACHINE" in uppercase, bold, with wide letter-spacing (4px+) to approximate a cyberpunk/futuristic look. Do NOT load external fonts (Google Fonts won't work in a JUCE plugin).
- Parameter labels: bold weight (font-weight 600 equivalent), 11–12px
- Value readouts: bold, GOLD, 11–13px
- Section headers: bold, 11px, NEON_PRIMARY, uppercase with letter-spacing

### Signal-Flow Layout (LEFT TO RIGHT, 5 columns):

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ EPIPHANY MACHINE                              [FREEZE] [BYPASS]  v2.0      │
│ ┌─ PRESET: [◄] Void Meditation [►] ─────────── MORPH TIME: ◉ ───────────┐ │
├─┴──────────┬────────────────┬───────────────────┬──────────────┬─────────┴─┤
│ 1 DETECT   │  2 GLITCH      │  3 DIFFUSION +    │  4 DUAL      │ 5 OUTPUT  │
│  + SPLIT   │   ENGINE       │    FEEDBACK       │   FILTER     │           │
│            │                │                   │              │           │
│ SENSITIVITY│   ◉ MORPH      │ DECAY  DAMP WIDTH │ [OFF|WET|FIN]│  ◉ MIX   │
│            │  (large knob)  │                   │              │ (large)   │
│ GLITCH     │                │ ┌─FEEDBACK LOOP─┐ │ ┌TEXTURE───┐ │           │
│  FOCUS     │ DENSITY  CHAOS │ │ DRIFT FEEDBACK│ │ │TYPE CUT  │ │ DRY↔WET  │
│ BODY↔TEX   │                │ └───────────────┘ │ │RESO LFO× │ │           │
│            │                │                   │ ├BODY──────┤ │  → STEREO │
│            │    →           │       →           │ │TYPE CUT  │ │    OUT    │
│            │                │                   │ │RESO      │ │           │
│            │                │                   │ └──────────┘ │           │
├────────────┴────────────────┴───────────────────┴──────────────┴───────────┤
│                                              epiphanymachineart.com        │
└───────────────────────────────────────────────────────────────────────────-─┘
```

Column widths (approximate): 165px, 175px, 230px, 210px, 140px (with 8px gaps, totaling ~950px with padding)

### Specific GUI elements:

**Title bar**: "EPIPHANY MACHINE" in bold uppercase with 4px letter-spacing, TEXT_PRIMARY color. FREEZE and BYPASS buttons to the right.

**Preset bar**: Below title. [◄] [►] arrows, GOLD preset name in center, MORPH TIME small knob on right with value readout. When morphing, preset name gently pulses.

**Signal flow indicator**: Below preset bar, small text: "SIGNAL FLOW" on left, "INPUT → → → OUTPUT" on right, with a thin gradient line between. NEON_PRIMARY color, 10–11px.

**5 columns**: Each is a SURFACE-colored panel with rounded corners and subtle SURFACE_RAISED border. Columns are visually connected by → arrow indicators between them.

**Column 1 — DETECT + SPLIT**: SENSITIVITY knob, GLITCH FOCUS knob with "BODY ← → TEXTURE" label beneath.

**Column 2 — GLITCH ENGINE**: MORPH (large, ~60px diameter, with color ring), DENSITY and CHAOS below side-by-side.

**Column 3 — DIFFUSION + FEEDBACK**: DECAY, DAMPING, WIDTH across the top. Below: a dashed-border sub-panel labeled "↻ FEEDBACK LOOP" containing DRIFT and FEEDBACK knobs.

**Column 4 — DUAL FILTER**: Filter position toggle at top [OFF|WET|FINAL]. Below: TEXTURE sub-panel (type selector, CUTOFF, RESO, LFO RATE, LFO DEPTH in 2×2 grid). Below that: BODY sub-panel (type selector, CUTOFF, RESO). Sub-panels use FILTER_ACCENT color.

**Column 5 — OUTPUT**: MIX knob (prominent, gold-bordered, ~52px diameter), "DRY ← → WET" label, "→ STEREO OUT" indicator at bottom.

**FREEZE button**: GOLD background when frozen, GOLD_DIM when off. Large and clearly hittable.

**BYPASS button**: DANGER color when bypassed (all processing off), TEXT_SECONDARY when active.

**Knobs**: Outer ring NEON_PRIMARY (or FILTER_ACCENT in filter section), filled arc in GOLD for current value. Pointer/indicator GOLD. Value readout GOLD below knob. Label TEXT_PRIMARY above knob (bold, 11px).

**Segmented buttons**: Inactive = SURFACE_RAISED bg + TEXT_SECONDARY text. Active = NEON_PRIMARY bg (or FILTER_ACCENT) + TEXT_PRIMARY text.

**Website link**: Bottom-right corner. `juce::HyperlinkButton` with URL `https://epiphanymachineart.com`, display text "epiphanymachineart.com". Styled TEXT_SECONDARY, ~9pt. Hover → GOLD color.

---

## UPDATED PROJECT STRUCTURE

```
EpiphanyMachine/
├── CMakeLists.txt
├── src/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   ├── PluginEditor.cpp
│   ├── DSP/
│   │   ├── GlitchEngine.h
│   │   ├── GlitchEngine.cpp
│   │   ├── DiffusionNetwork.h       (modified: freeze support)
│   │   ├── DiffusionNetwork.cpp
│   │   ├── PitchDrifter.h
│   │   ├── PitchDrifter.cpp
│   │   ├── StereoWidener.h
│   │   ├── StereoWidener.cpp
│   │   ├── TextureBodyDetector.h    (TWO instances used in processor)
│   │   ├── TextureBodyDetector.cpp
│   │   ├── DualFilter.h             (Cytomic TPT SVF)
│   │   └── DualFilter.cpp
│   ├── Presets/
│   │   ├── PresetData.h
│   │   └── PresetManager.h/.cpp
│   └── GUI/
│       ├── CustomLookAndFeel.h       (cyberpunk theme)
│       ├── CustomLookAndFeel.cpp
│       ├── MorphKnob.h
│       ├── MorphKnob.cpp
│       ├── PresetBar.h
│       └── PresetBar.cpp
```

---

## CMAKE REQUIREMENTS

- FetchContent for JUCE from GitHub
- Plugin name: "Epiphany Machine", code: "EpMa", manufacturer: "Custom"
- `COPY_PLUGIN_AFTER_BUILD TRUE`, `VST3_COPY_DIR` → `~/.vst3`
- C++17, VST3 only

---

## DSP IMPLEMENTATION GUIDELINES

All Alien Machine guidelines still apply. Critical additions:

### Filter implementation:
- **USE CYTOMIC TPT SVF. DO NOT USE CHAMBERLIN SVF.** The Chamberlin SVF (`f = 2*sin(...)`) is unstable at high frequencies and clicks during modulation. The Cytomic TPT SVF (`g = tan(...)`) is stable across the full frequency range and handles LFO modulation cleanly. The exact implementation is specified in the DUAL FILTER section above.
- The `tan()` call per sample is acceptable — it's one transcendental function per filter update. If optimization is needed later, a fast approximation of tan can be used, but correctness first.

### Denormal protection:
- Long reverb tails and frozen signals produce denormalized floats that cause CPU spikes. Add a tiny constant to the FDN feedback path:
  ```cpp
  // At the end of each delay line's feedback processing:
  sample += 1.0e-18f;  // flush denormals without audible effect
  ```
- Alternatively, enable flush-to-zero at the top of processBlock:
  ```cpp
  juce::ScopedNoDenormals noDenormals;  // JUCE helper — add this as the first line of processBlock
  ```
  **Use BOTH approaches** for maximum safety.

### Dual detector instances:
- `inputDetector` runs at the START of processBlock on the raw input → feeds GLITCH FOCUS routing
- `filterDetector` runs ONLY when filter position != Off, on the signal the Dual Filter will process → feeds the Dual Filter's texture/body split
- Both instances share the same SENSITIVITY parameter value

### Freeze safety:
- During freeze with DRIFT != 0 and FEEDBACK > 0, energy can build in the feedback loop. Apply an ADDITIONAL soft-clip `x / (1.0f + fabsf(x))` in the feedback return path when freeze is active. This is in addition to the normal feedback soft-clip.

### Preset morphing:
- Float parameters: linear interpolation
- Frequency parameters (texFilterCutoff, bodyFilterCutoff): interpolate in LOG space: `exp(lerp(log(start), log(end), progress))`
- MIX parameter: cosine interpolation to prevent audible dip: `mix = start + (end - start) * (0.5f - 0.5f * cosf(progress * PI))`
- Choice parameters: snap at 50% progress
- Bool parameters (bypass, freeze): NOT morphed, snap instantly
- User override: if any parameter's listener fires during morph (not from the morph system), exclude it from interpolation

### Performance:
- `juce::ScopedNoDenormals noDenormals;` as first line of processBlock
- `juce::SmoothedValue` for ALL continuously-read parameters
- Skip filterDetector + DualFilter entirely when filterPos == Off
- Skip ALL DSP when bypass is fully engaged (SmoothedValue reached 1.0)

---

## WHAT NOT TO DO

- Don't use Chamberlin SVF — use Cytomic TPT SVF (critical stability fix)
- Don't use a single detector instance for both Glitch Focus and Dual Filter — use two instances
- Don't load external fonts in the JUCE plugin GUI — use built-in fonts with letter-spacing
- Don't use the Projucer, VST2, or JUCE's built-in Reverb class
- Don't use FFT for texture/body detection or pitch shifting
- Don't save presets to files — hardcode factory presets
- Don't morph bool parameters
- Don't reset FDN buffers when freeze toggles
- Don't allow morphTime below 0.1s
- Don't skip denormal protection — use both ScopedNoDenormals AND the 1e-18 offset
- Don't rename any Alien Machine parameter IDs

---

## IMPLEMENTATION ORDER

1. **Freeze**: Add freeze parameter. Modify DiffusionNetwork with freeze mode + extra safety clip. Add FREEZE button to GUI. Test infinite sustain + smooth unfreeze.

2. **Second detector instance + filter detector wiring**: Add filterDetector. Wire it to run on the appropriate signal based on filter position. Verify DualFilter now uses local detection, not input detection.

3. **Replace SVF with Cytomic TPT**: If DualFilter was already built with Chamberlin SVF, replace the per-sample processing with the Cytomic implementation. Test: sweep the texture filter LFO rate to 10 Hz at high cutoff — should be smooth with no clicks.

4. **Denormal protection**: Add `ScopedNoDenormals` + 1e-18 offset. Test: let a frozen reverb tail sustain for 60+ seconds — CPU should remain stable.

5. **Preset system**: Create PresetData.h + PresetManager. Add preset/morphTime parameters. Wire morph logic with log-space frequency interpolation and cosine MIX interpolation.

6. **Signal-flow GUI + cyberpunk theme**: Expand window to 950x700. Build the 5-column signal-flow layout. Apply the cyberpunk color palette. Add preset bar, website link. Use built-in JUCE fonts with bold weight and letter-spacing.

Each step produces a working plugin. Test after each step.

---

## BUILD & TEST

```bash
cd ~/EpiphanyMachine
cmake -B build
cmake --build build
# VST3 auto-copies to ~/.vst3/
# In REAPER: Options → Preferences → Plug-ins → VST → Rescan
```

### Final test checklist:
1. **Bypass**: Clean click-free toggle
2. **Freeze**: Infinite sustain, smooth unfreeze
3. **Freeze + Drift + Feedback**: Frozen tail evolves harmonically without exploding (safety clip working)
4. **Freeze 60 seconds**: CPU stays stable (denormal protection working)
5. **Dual Filter LFO at 10 Hz, cutoff at 15kHz**: No clicks or instability (TPT SVF working)
6. **Filter in Wet mode**: Dry signal unaffected, wet signal filtered with accurate texture/body split
7. **Filter in Final mode**: Everything filtered with accurate texture/body split on the mixed signal
8. **GLITCH FOCUS sweep**: Smooth transition from body-glitch to texture-glitch
9. **Preset morph to "Deep Submersion" (8s morph time)**: Watch cutoff knobs sweep smoothly in log space, MIX transitions without dip
10. **User override during morph**: Grab DECAY mid-morph, it stays where you put it
11. **Preset cycling**: All 7 presets sound distinctly different
12. **Freeze + preset morph**: Frozen tail holds while parameters change around it
13. **Visual**: Deep purple background, gold accents, signal-flow left-to-right layout, bold readable text
14. **Website link**: Click "epiphanymachineart.com" → opens browser
