# Example Plugin: WarmPadSynth

> A complete example demonstrating the JUCE plugin development workflow from Phase 0 through Phase 12.

## Overview

**WarmPadSynth** is a simple synthesizer plugin that creates warm, evolving pad sounds. It demonstrates:
- Complete Phase 0 specification
- DSP implementation patterns
- GUI implementation with custom LookAndFeel
- DAW testing workflow
- Session state tracking

## Plugin Specification

### Type
Synthesizer (VST3/AU/Standalone)

### Sound Identity
"Warm analog pad with subtle movement and stereo width"

### Character Words
- Warm
- Evolving
- Analog
- Wide

### Reference Plugins
- Juno-106
- OB-8

### Technical Constraints
- JUCE Version: 7.0.12+
- Formats: VST3, AU, Standalone
- Target OS: Linux, macOS, Windows
- Target DAW: REAPER, Ableton Live, Logic Pro
- Sample Rates: 44.1kHz - 192kHz
- Buffer Sizes: 64 - 2048 samples
- CPU Budget: < 5% per instance

### Signal Flow
```
Oscillator 1 (Saw) ─┐
                     ├─► Filter (Lowpass) ─► Amp ─► Chorus ─► Output
Oscillator 2 (Saw) ─┘      │
                            │
                     Filter Envelope ─►

                     LFO ─► (Filter Mod)
```

### Parameters

| Parameter | Range | Default | Skew | Automation |
|-----------|-------|---------|------|------------|
| Osc1 Level | 0-1 | 0.8 | 1.0 | Yes |
| Osc2 Level | 0-1 | 0.8 | 1.0 | Yes |
| Osc2 Detune | 0-0.5 | 0.08 | 0.5 | Yes |
| Filter Cutoff | 20-20000Hz | 800Hz | 0.5 | Yes |
| Filter Resonance | 0-1 | 0.15 | 1.0 | Yes |
| Filter Env Amount | -1 to 1 | 0.3 | 1.0 | Yes |
| Filter Attack | 0.001-10s | 0.01s | 0.5 | Yes |
| Filter Decay | 0.001-10s | 0.5s | 0.5 | Yes |
| Filter Sustain | 0-1 | 0.7 | 1.0 | Yes |
| Filter Release | 0.001-10s | 1.0s | 0.5 | Yes |
| Amp Attack | 0.001-10s | 0.5s | 0.5 | Yes |
| Amp Decay | 0.001-10s | 0.3s | 0.5 | Yes |
| Amp Sustain | 0-1 | 0.8 | 1.0 | Yes |
| Amp Release | 0.001-10s | 2.0s | 0.5 | Yes |
| LFO Rate | 0.1-20Hz | 0.3Hz | 0.5 | Yes |
| LFO Depth | 0-1 | 0.2 | 1.0 | Yes |
| Chorus Rate | 0.1-5Hz | 0.3Hz | 1.0 | Yes |
| Chorus Depth | 0-1 | 0.3 | 1.0 | Yes |
| Chorus Mix | 0-1 | 0.4 | 1.0 | Yes |
| Master Volume | 0-1 | 0.7 | 1.0 | Yes |

### Presets
1. **Warm Pad** - Default warm pad
2. **Evolving Pad** - Slow LFO, more movement
3. **Bright Pad** - Higher cutoff, less warmth
4. **Deep Bass** - Low cutoff, mono

### Gain Staging Philosophy
- Oscillators at 0.8 each (combined peaks around -3dB)
- Filter at unity gain
- Chorus at 40% mix (wet at -8dB)
- Master at 0.7 (output peaks around -6dB)

### What Not To Do
- Don't use multiple filters (single filter is sufficient)
- Don't add reverb (chorus provides width)
- Don't use distortion (warmth comes from filter, not drive)
- Don't exceed 8 voices (CPU budget)

---

## Implementation

### Directory Structure
```
WarmPadSynth/
├── CMakeLists.txt
├── src/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   ├── PluginEditor.cpp
│   ├── DSP/
│   │   ├── Oscillator.h
│   │   ├── Oscillator.cpp
│   │   ├── Filter.h
│   │   ├── Filter.cpp
│   │   ├── Envelope.h
│   │   ├── Envelope.cpp
│   │   ├── LFO.h
│   │   ├── LFO.cpp
│   │   └── Chorus.h
│   └── GUI/
│       ├── LookAndFeel.h
│       ├── LookAndFeel.cpp
│       ├── KnobComponent.h
│       └── KnobComponent.cpp
├── tests/
│   ├── test_oscillator.cpp
│   ├── test_filter.cpp
│   └── test_state.cpp
└── presets/
    ├── WarmPad.preset
    ├── EvolvingPad.preset
    ├── BrightPad.preset
    └── DeepBass.preset
```

### Key Code Patterns

#### Oscillator (TPT-style for alias reduction)
```cpp
class Oscillator {
public:
    void setFrequency(float frequency) {
        phaseIncrement = frequency / sampleRate;
    }

    float process(float detune = 0.0f) {
        auto output = processSaw(phase + detune);
        phase += phaseIncrement;
        if (phase >= 1.0f) phase -= 1.0f;
        return output;
    }

private:
    float processSaw(float p) {
        // PolyBLEP for alias reduction
        float saw = 2.0f * p - 1.0f;
        if (p < phaseIncrement) {
            saw -= polyBlep(p);
        }
        return saw;
    }

    float polyBlep(float t) {
        float dt = phaseIncrement;
        t = t / dt - 0.5f;
        return t * t * (2.0f - 4.0f * std::abs(t));
    }

    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float sampleRate = 44100.0f;
};
```

#### Filter (TPT SVF)
```cpp
class TPTFilter {
public:
    void setCutoff(float cutoff) {
        g = std::tan(juce::MathConstants<float>::pi * cutoff / sampleRate);
        h = 1.0f / (1.0f + g);
    }

    void setResonance(float resonance) {
        g2 = 2.0f * resonance;
    }

    float process(float input) {
        float v = (input - z1 * g2 - z2) * h;
        float lp = v + z1;
        float hp = input - lp - g2 * z1;
        float bp = hp * g + z1;

        z1 = v + bp * g;
        z2 = hp * g + z2 + bp * g2 * h;

        return lp;
    }

private:
    float g = 0.0f, g2 = 0.0f, h = 1.0f;
    float z1 = 0.0f, z2 = 0.0f;
    float sampleRate = 44100.0f;
};
```

---

## Session State Example

```json
{
  "session_state": {
    "project_name": "WarmPadSynth",
    "created": "2026-03-29T10:00:00Z",
    "last_updated": "2026-03-29T18:30:00Z",
    "current_phase": 9,
    "phase_status": {
      "phase_0": {
        "status": "completed",
        "completed_tasks": [
          "Define plugin type and signal flow",
          "Define sound identity",
          "Define technical constraints",
          "Write one-page spec"
        ]
      },
      "phase_4": {
        "status": "completed",
        "completed_tasks": [
          "Oscillator module",
          "Filter module",
          "Envelope module",
          "LFO module",
          "Chorus module"
        ]
      },
      "phase_9": {
        "status": "in_progress",
        "completed_tasks": [
          "Load in REAPER",
          "Test oscillator",
          "Test filter"
        ],
        "pending_tasks": [
          "Test envelope",
          "Test LFO modulation",
          "Test chorus",
          "Verify preset: WarmPad",
          "Verify preset: EvolvingPad"
        ]
      }
    },
    "key_decisions": [
      "Using TPT filter for better frequency response",
      "Using PolyBLEP for alias reduction",
      "Chorus at 40% mix for warmth without phasiness"
    ],
    "pending_issues": [
      "Filter resonance click at values > 0.8 - may need soft limiting"
    ],
    "creative_language": {
      "sound_identity": "Warm analog pad with subtle movement",
      "character_words": ["warm", "evolving", "analog", "wide"]
    }
  }
}
```

---

## DAW Testing Results

### Preset: Warm Pad
- **Filter Cutoff:** 0.30 (800Hz) ✓ Verified
- **Filter Resonance:** 0.15 ✓ Verified
- **Chorus Depth:** 0.40 ✓ Verified
- **Sound:** Warm, wide, subtle movement. Matches expected description.
- **Confidence:** Ground Truth

### Preset: Evolving Pad
- **LFO Rate:** 0.3 Hz ✓ Verified
- **LFO Depth:** 0.4 ✓ Verified
- **Sound:** Evolving movement, slightly faster than default.
- **Confidence:** High

---

## Build Instructions

### Linux
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./WarmPadSynth_artefacts/Release/VST3/WarmPadSynth.vst3
```

### macOS
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
# AU: ./WarmPadSynth_artefacts/Release/AU/WarmPadSynth.component
# VST3: ./WarmPadSynth_artefacts/Release/VST3/WarmPadSynth.vst3
```

### Windows
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
# VST3: ./WarmPadSynth_artefacts/Release/VST3/WarmPadSynth.vst3
```

---

## Lessons Learned

### What Worked Well
- TPT filter provides smooth frequency response
- PolyBLEP reduces aliasing effectively
- Chorus at 40% mix provides width without phase issues
- Session state helps resume work across days

### Issues Found
- Filter resonance > 0.8 causes click (fixed with soft limiting)
- LFO needs to be mono before filter to avoid stereo modulation issues
- CPU spikes when polyphony > 8 (added voice limiting)

### Future Improvements
- Add reverb as optional post-effect
- Add velocity sensitivity for filter envelope
- Add aftertouch modulation

---

## Related Files

- **Spec:** `docs/superpowers/specs/2026-03-29-warmpadsynth-design.md`
- **Plan:** `docs/superpowers/plans/2026-03-29-warmpadsynth-implementation.md`
- **Session:** `sessions/warmpadsynth-session.json`
- **Playbook:** `playbooks/vst-plugin-playbook-v7-unified.json`