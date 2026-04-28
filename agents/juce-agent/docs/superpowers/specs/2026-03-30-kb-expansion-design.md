# Knowledge Base Expansion Specification

**Date:** 2026-03-30
**Status:** Draft
**Credits Budget:** ~15,000 of 20,500 available (reserve 5,500)

## Executive Summary

Expand the knowledge base system with professional-grade content focused on JUCE development, DSP algorithms, MIDI handling, and sound design. Add 63 new topics across 8 knowledge bases with harvest-ready search terms and sources.

## Current State

| KB | Files | Status | Primary Gaps |
|----|-------|--------|---------------|
| juce-kb | 169 | Comprehensive | Advanced patterns, real-time, performance |
| dsp-kb | 27 | Light | Effects algorithms, dynamics, spatial |
| sound-design-kb | 13 | Light | Preset design, modulation, layering |
| ui-kb | 11 | Adequate | Minimal expansion needed |
| cpp-kb | 28 | Adequate | No expansion planned |
| cmake-kb | 8 | Adequate | No expansion planned |
| testing-kb | 0 | NEW | Complete new KB needed |
| midi-kb | 0 | NEW | Complete new KB needed |

---

## 1. dsp-kb Expansion (15 topics, ~3,500 credits)

### New Folder Structure

```
dsp-kb/
├── algorithms/          # EXISTING
├── filters/             # EXISTING
├── theory/               # EXISTING
├── synthesis/            # EXISTING
├── optimization/         # EXISTING
├── stanford-ccrma/      # EXISTING
│
├── reverb/               # NEW
│   ├── algorithmic-reverb.json
│   ├── convolution-reverb.json
│   ├── impulse-response.json
│   └── reverb-design.json
│
├── dynamics/             # NEW
│   ├── compressor.json
│   ├── limiter-design.json
│   ├── noise-gate.json
│   ├── expander.json
│   └── multiband-compression.json
│
├── modulation/           # NEW
│   ├── chorus.json
│   ├── flanger.json
│   ├── phaser.json
│   ├── tremolo.json
│   └── vibrato.json
│
├── delay/                # NEW
│   ├── delay-lines.json
│   ├── echo.json
│   └── ping-pong-delay.json
│
├── distortion/           # NEW
│   ├── waveshaping.json
│   ├── saturation.json
│   └── soft-clipping.json
│
├── pitch/                # NEW
│   ├── pitch-shifting.json
│   ├── harmonizer.json
│   └── formant-shifting.json
│
└── spatial/              # NEW
    ├── stereo-width.json
    ├── mid-side-processing.json
    └── binaural-audio.json
```

### Search Terms and Sources

#### reverb/
**Primary concepts:** algorithmic reverb, convolution, impulse response, reverb design, room simulation, early reflections, late diffusion

| File | Search Terms | Sources |
|------|--------------|---------|
| algorithmic-reverb.json | "algorithmic reverb design", "Freeverb", "Schroeder reverb", "feedback delay network", "FDN reverb" | CCRMA DSP papers, EarLevel blog, musicdsp.org |
| convolution-reverb.json | "convolution reverb", "FFT convolution", "partitioned convolution", "zero latency convolution" | CCRMA, Signal Processing Stack Exchange |
| impulse-response.json | "impulse response measurement", "IRS creation", "sine sweep", "maximum length sequence" | CCRMA papers, OpenAir IR library docs |
| reverb-design.json | "reverb parameter design", "reverb EQ", "damping", "pre-delay", "room size" | Sound on Sound, Journal of AES |

#### dynamics/
**Primary concepts:** compression, limiting, gating, expansion, multiband, attack, release, ratio, knee

| File | Search Terms | Sources |
|------|--------------|---------|
| compressor.json | "audio compressor design", "feedforward compression", "feedback compression", "RMS compression", "peak compression" | EarLevel blog, CCRMA DSP papers |
| limiter-design.json | "brickwall limiter", "lookahead limiter", "true peak limiter", "limiter algorithms" | CCRMA, musicdsp.org |
| noise-gate.json | "noise gate algorithm", "hysteresis", "attack release gate", "sidechain gate" | musicdsp.org, DSP Stack Exchange |
| expander.json | "audio expander", "downward expansion", "upward expansion", "dynamic range" | AES papers, DSP textbooks |
| multiband-compression.json | "multiband compressor", "crossover filter", "band splitting", "compression bands" | Sound on Sound, Plugin Doctor |

#### modulation/
**Primary concepts:** chorus, flanger, phaser, tremolo, vibrato, LFO, modulation depth, rate

| File | Search Terms | Sources |
|------|--------------|---------|
| chorus.json | "chorus effect algorithm", "delay modulation", "multiple voices chorus", "stereo chorus" | musicdsp.org, CCRMA |
| flanger.json | "flanger algorithm", "comb filter flanger", "through-zero flanger", "feedback flanger" | DSP Stack Exchange, musicdsp.org |
| phaser.json | "phaser algorithm", "allpass filter phaser", "LFO phaser", "stages phaser" | CCRMA papers, musicdsp.org |
| tremolo.json | "tremolo effect", "amplitude modulation", "LFO tremolo", "stereo tremolo" | musicdsp.org |
| vibrato.json | "vibrato effect", "pitch modulation", "delay vibrato", "depth vibrato" | musicdsp.org |

#### delay/
**Primary concepts:** delay lines, echo, ping-pong, feedback, modulation

| File | Search Terms | Sources |
|------|--------------|---------|
| delay-lines.json | "digital delay line", "fractional delay", "interpolation delay", "circular buffer delay" | CCRMA DSP papers, DSP textbooks |
| echo.json | "echo effect", "feedback delay", "multi-tap delay", "tape echo emulation" | musicdsp.org |
| ping-pong-delay.json | "ping pong delay", "stereo delay", "cross feedback", "delay panning" | Plugin development forums |

#### distortion/
**Primary concepts:** waveshaping, saturation, soft clipping, hard clipping, tube emulation

| File | Search Terms | Sources |
|------|--------------|---------|
| waveshaping.json | "waveshaper design", "waveshaping function", "transfer function", "polynomial waveshaper" | CCRMA, DSP textbooks |
| saturation.json | "analog saturation", "tape saturation", "transformer saturation", "soft saturation" | Sound on Sound, musicdsp.org |
| soft-clipping.json | "soft clipper", "tanh clipping", "cubic soft clipper", "anti-aliased clipping" | DSP Stack Exchange |

#### pitch/
**Primary concepts:** pitch shifting, harmonizer, formant preservation, time stretching

| File | Search Terms | Sources |
|------|--------------|---------|
| pitch-shifting.json | "pitch shifting algorithm", "phase vocoder pitch", "granular pitch shift", "formant preservation" | CCRMA papers, DSP Stack Exchange |
| harmonizer.json | "harmonizer effect", "intelligent harmonizer", "chord harmonizer", "pitch corrected harmonizer" | Plugin development blogs |
| formant-shifting.json | "formant shifting", "formant preservation", "vocoder formant", "spectral envelope" | CCRMA papers |

#### spatial/
**Primary concepts:** stereo imaging, mid-side processing, binaural, HRTF, spatial audio

| File | Search Terms | Sources |
|------|--------------|---------|
| stereo-width.json | "stereo width processing", "mid-side encoding", "stereo widening", "mono compatibility" | Sound on Sound |
| mid-side-processing.json | "M/S processing", "mid side encoding", "mid side EQ", "side chain processing" | AES papers, Sound on Sound |
| binaural-audio.json | "binaural rendering", "HRTF", "head related transfer function", "3D audio headphones" | CCRMA, AES Spatial Audio papers |

---

## 2. sound-design-kb Expansion (10 topics, ~2,500 credits)

### New Folder Structure

```
sound-design-kb/
├── techniques/          # EXISTING
├── presets/              # EXISTING
│
├── preset-design/        # NEW
│   ├── preset-methodology.json
│   ├── macro-controls.json
│   ├── preset-morphing.json
│   └── sound-design-workflow.json
│
├── modulation/           # NEW
│   ├── modulation-routing.json
│   ├── lfo-shapes.json
│   ├── envelope-curves.json
│   └── mod-matrix.json
│
├── layering/             # NEW
│   ├── parallel-layering.json
│   └── serial-layering.json
│
└── effects-chain/        # NEW
    ├── effects-ordering.json
    └── parallel-effects.json
```

### Search Terms and Sources

#### preset-design/
**Primary concepts:** preset design methodology, macro controls, morphing, workflow, organization

| File | Search Terms | Sources |
|------|--------------|---------|
| preset-methodology.json | "preset design methodology", "sound design workflow", "preset organization", "library design" | Sound on Sound, Attack Magazine |
| macro-controls.json | "macro control design", "parameter morphing", "macro assignment", "performance controls" | Plugin development forums |
| preset-morphing.json | "preset morphing", "interpolation presets", "XY pad presets", "continuous preset control" | Music production blogs |
| sound-design-workflow.json | "sound design process", "synthesis workflow", "patch design steps" | Sound on Sound, YouTube tutorials |

#### modulation/
**Primary concepts:** modulation routing, LFO shapes, envelope curves, modulation matrix

| File | Search Terms | Sources |
|------|--------------|---------|
| modulation-routing.json | "modulation routing", "mod sources destinations", "modulation matrix design", "multiple modulations" | Synth tutorials, Sound on Sound |
| lfo-shapes.json | "LFO shapes", "custom LFO waveform", "LFO rate sync", "LFO phase" | Synth documentation, musicdsp.org |
| envelope-curves.json | "envelope curves", "ADSR shapes", "exponential envelope", "linear envelope" | Synth design papers |
| mod-matrix.json | "modulation matrix", "mod routing", "source destination matrix", "polyphonic modulation" | Plugin development forums |

#### layering/
**Primary concepts:** parallel layering, serial layering, splitting, blending

| File | Search Terms | Sources |
|------|--------------|---------|
| parallel-layering.json | "parallel synthesis", "layer blending", "stack sounds", "parallel processing" | Sound on Sound, Attack Magazine |
| serial-layering.json | "serial processing", "effect chaining", "cascade effects", "feedback layering" | Sound design tutorials |

#### effects-chain/
**Primary concepts:** effects ordering, parallel effects, feedback, routing

| File | Search Terms | Sources |
|------|--------------|---------|
| effects-ordering.json | "effects chain order", "EQ before compression", "reverb delay order", "signal flow effects" | Sound on Sound, mixing tutorials |
| parallel-effects.json | "parallel compression", "parallel reverb", "dry wet mixing", "parallel processing" | Sound on Sound, mixing blogs |

---

## 3. juce-kb Expansion (12 topics, ~3,000 credits)

### New Folder Structure

```
juce-kb/
├── juce-official/        # EXISTING
├── melatonin-dev/        # EXISTING
├── github-awesome-juce/  # EXISTING
├── patterns/             # EXISTING
├── failure-modes/        # EXISTING
├── api/                  # EXISTING
├── tooling/              # EXISTING
├── agent/                # EXISTING
│
├── realtime/             # NEW
│   ├── lock-free-queues.json
│   ├── allocation-free-patterns.json
│   ├── audio-thread-safety.json
│   └── realtime-profiling.json
│
├── formats/              # NEW
│   ├── vst3-specifics.json
│   ├── au-specifics.json
│   ├── aax-considerations.json
│   └── plugin-sandboxing.json
│
├── performance/          # NEW
│   ├── simd-optimization.json
│   ├── opengl-rendering.json
│   └── memory-pooling.json
│
└── state-advanced/       # NEW
    ├── apvts-patterns.json
    ├── automation-optimization.json
    └── preset-performance.json
```

### Search Terms and Sources

#### realtime/
**Primary concepts:** lock-free, allocation-free, audio thread, real-time safety, SPSC queues

| File | Search Terms | Sources |
|------|--------------|---------|
| lock-free-queues.json | "lock free queue C++", "SPSC queue audio", "moodycamel queue", "real time safe queue" | JUCE forum, CppCon talks, CCRMA |
| allocation-free-patterns.json | "allocation free audio", "preallocated memory", "audio thread allocation", "real time safe" | JUCE forum, melatonin.dev blog |
| audio-thread-safety.json | "audio thread safety", "JUCE thread safety", "MessageManagerLock audio", "real time thread" | JUCE forum, melatonin.dev blog |
| realtime-profiling.json | "audio profiler", "real time profiler", "CPU profiler audio", "plugin profiler" | JUCE forum, Plugin Doctor |

#### formats/
**Primary concepts:** VST3 specifics, AU specifics, AAX, sandboxing, code signing

| File | Search Terms | Sources |
|------|--------------|---------|
| vst3-specifics.json | "VST3 plugin development", "VST3 parameters", "VST3 preset", "Steinberg VST3 SDK" | Steinberg docs, JUCE forum |
| au-specifics.json | "Audio Unit development", "AU plugin", "AU parameters", "macOS AU", "AudioUnit framework" | Apple docs, JUCE forum |
| aax-considerations.json | "AAX plugin", "Avid AAX SDK", "Pro Tools plugin", "AAX development" | Avid docs, JUCE forum |
| plugin-sandboxing.json | "macOS sandboxing", "plugin notarization", "code signing audio plugin", "entitlements plugin" | JUCE forum, Apple docs |

#### performance/
**Primary concepts:** SIMD, OpenGL, memory pooling, profiling

| File | Search Terms | Sources |
|------|--------------|---------|
| simd-optimization.json | "SIMD audio processing", "JUCE dsp SIMD", "vectorized DSP", "SSE AVX audio" | JUCE forum, DSP blog posts |
| opengl-rendering.json | "JUCE OpenGL", "OpenGL audio plugin", "GPU rendering UI", "JUCE OpenGL context" | JUCE forum, melatonin.dev |
| memory-pooling.json | "memory pool audio", "object pool C++", "audio memory management", "preallocated buffers" | JUCE forum, CCRMA |

#### state-advanced/
**Primary concepts:** APVTS patterns, automation, preset performance

| File | Search Terms | Sources |
|------|--------------|---------|
| apvts-patterns.json | "JUCE APVTS", "AudioProcessorValueTreeState", "parameter attachment", "state management JUCE" | JUCE forum, melatonin.dev |
| automation-optimization.json | "plugin automation", "parameter automation", "DAW automation", "smooth parameters" | JUCE forum |
| preset-performance.json | "preset loading performance", "preset browser optimization", "async preset loading" | JUCE forum |

---

## 4. testing-kb (NEW KB, 10 topics, ~2,500 credits)

### Folder Structure

```
testing-kb/
├── unit-testing/
│   ├── catch2-juce.json
│   ├── mocking-audio.json
│   └── test-driven-audio.json
│
├── daw-testing/
│   ├── reaper-testing.json
│   ├── ableton-testing.json
│   └── logic-testing.json
│
├── validation/
│   ├── pluginval.json
│   └── automated-validation.json
│
├── regression/
│   ├── audio-comparison.json
│   └── snapshot-testing.json
│
└── performance-testing/
    ├── cpu-profiling.json
    └── latency-measurement.json
```

### Search Terms and Sources

#### unit-testing/
**Primary concepts:** unit testing, mocking, TDD, Catch2, GoogleTest

| File | Search Terms | Sources |
|------|--------------|---------|
| catch2-juce.json | "Catch2 JUCE", "unit test audio plugin", "JUCE testing", "Catch2 C++" | Catch2 docs, JUCE forum, melatonin.dev |
| mocking-audio.json | "mocking audio", "mock AudioProcessor", "fake audio buffer", "test double audio" | JUCE forum, testing blogs |
| test-driven-audio.json | "TDD audio plugin", "test driven development DSP", "testing audio code" | Software testing blogs, JUCE forum |

#### daw-testing/
**Primary concepts:** DAW-specific testing, REAPER, Ableton, Logic

| File | Search Terms | Sources |
|------|--------------|---------|
| reaper-testing.json | "REAPER plugin testing", "REAPER development", "ReaScript plugin test" | REAPER forums, Cockos docs |
| ableton-testing.json | "Ableton plugin testing", "Live plugin validation", "Ableton development" | Ableton docs, JUCE forum |
| logic-testing.json | "Logic Pro plugin testing", "AU testing Logic", "Logic plugin validation" | Apple docs, JUCE forum |

#### validation/
**Primary concepts:** pluginval, automated validation, CI testing

| File | Search Terms | Sources |
|------|--------------|---------|
| pluginval.json | "pluginval JUCE", "JUCE validation", "plugin validation tool", "Tracktion pluginval" | pluginval docs, JUCE forum |
| automated-validation.json | "CI plugin testing", "GitHub Actions audio plugin", "automated plugin validation" | JUCE forum, CI docs |

#### regression/
**Primary concepts:** audio comparison, snapshot testing, regression

| File | Search Terms | Sources |
|------|--------------|---------|
| audio-comparison.json | "audio output comparison", "regression testing audio", "audio snapshot", "WAV comparison" | Testing blogs, JUCE forum |
| snapshot-testing.json | "snapshot testing plugin", "parameter state snapshot", "UI snapshot testing" | Testing blogs |

#### performance-testing/
**Primary concepts:** CPU profiling, latency measurement, performance

| File | Search Terms | Sources |
|------|--------------|---------|
| cpu-profiling.json | "CPU profiling plugin", "audio CPU usage", "plugin performance testing" | Plugin Doctor, JUCE forum |
| latency-measurement.json | "plugin latency measurement", "round trip latency", "PDC testing" | JUCE forum, DAW docs |

---

## 5. midi-kb (NEW KB, 16 topics, ~3,500 credits)

### Folder Structure

```
midi-kb/
├── protocol/
│   ├── midi-basics.json
│   ├── midi-messages.json
│   ├── channel-messages.json
│   └── system-messages.json
│
├── realtime/
│   ├── midi-timing.json
│   ├── midi-buffering.json
│   ├── jitter-compensation.json
│   └── midi-latency.json
│
├── mpe/
│   ├── mpe-basics.json
│   ├── mpe-implementation.json
│   └── mpe-mapping.json
│
├── controllers/
│   ├── controller-mapping.json
│   ├── midi-learn.json
│   ├── parameter-automation.json
│   └── hardware-integration.json
│
└── advanced/
    ├── sysex.json
    ├── midi-clock.json
    └── midi-2.json
```

### Search Terms and Sources

#### protocol/
**Primary concepts:** MIDI protocol, status bytes, channel messages, system messages, MIDI specification

| File | Search Terms | Sources |
|------|--------------|---------|
| midi-basics.json | "MIDI protocol basics", "MIDI specification", "MIDI data format", "MIDI message structure" | MIDI Association, JUCE forum |
| midi-messages.json | "MIDI message types", "MIDI status byte", "MIDI data bytes", "MIDI message format" | MIDI Association, musicdsp.org |
| channel-messages.json | "MIDI note on off", "MIDI control change", "MIDI pitch bend", "MIDI program change" | MIDI Association, JUCE forum |
| system-messages.json | "MIDI system exclusive", "MIDI sysEx", "MIDI system common", "MIDI real-time messages" | MIDI Association, JUCE forum |

#### realtime/
**Primary concepts:** MIDI timing, buffering, jitter, latency, real-time MIDI handling

| File | Search Terms | Sources |
|------|--------------|---------|
| midi-timing.json | "MIDI timing", "MIDI clock", "MIDI timestamp", "sample accurate MIDI" | JUCE forum, CCRMA |
| midi-buffering.json | "MIDI buffer", "MIDI queue", "MidiBuffer JUCE", "MIDI buffering audio" | JUCE forum, melatonin.dev |
| jitter-compensation.json | "MIDI jitter", "MIDI timing jitter", "MIDI delay compensation", "MIDI timestamp" | DSP forums, JUCE forum |
| midi-latency.json | "MIDI latency", "MIDI round trip", "MIDI input latency", "MIDI output latency" | JUCE forum, DAW docs |

#### mpe/
**Primary concepts:** MPE (MIDI Polyphonic Expression), multidimensional polyphonic expression, MPE zones

| File | Search Terms | Sources |
|------|--------------|---------|
| mpe-basics.json | "MIDI Polyphonic Expression", "MPE specification", "MPE zones", "multidimensional polyphonic" | MPE specification, MIDI Association |
| mpe-implementation.json | "MPE implementation", "MPE JUCE", "MPE zone setup", "MPE channel assignment" | JUCE forum, MPE docs |
| mpe-mapping.json | "MPE mapping", "MPE to parameters", "MPE dimension mapping", "MPE timbre" | JUCE forum, synth docs |

#### controllers/
**Primary concepts:** controller mapping, MIDI learn, parameter automation, hardware integration

| File | Search Terms | Sources |
|------|--------------|---------|
| controller-mapping.json | "MIDI controller mapping", "CC to parameter", "MIDI CC assignment", "controller assignment" | JUCE forum, synth tutorials |
| midi-learn.json | "MIDI learn", "MIDI teach", "plugin MIDI learn", "automatic MIDI mapping" | JUCE forum, plugin docs |
| parameter-automation.json | "MIDI automation", "MIDI CC automation", "parameter mapping MIDI", "MIDI recording" | JUCE forum, DAW docs |
| hardware-integration.json | "MIDI hardware integration", "MIDI controller plugin", "USB MIDI", "MIDI interface" | JUCE forum, MIDI docs |

#### advanced/
**Primary concepts:** SysEx, MIDI clock, MIDI 2.0, advanced MIDI features

| File | Search Terms | Sources |
|------|--------------|---------|
| sysex.json | "MIDI system exclusive", "SysEx protocol", "SysEx JUCE", "manufacturer SysEx" | MIDI Association, JUCE forum |
| midi-clock.json | "MIDI clock", "MIDI sync", "MIDI tempo", "MIDI beat clock" | JUCE forum, MIDI docs |
| midi-2.json | "MIDI 2.0", "MIDI 2 specification", "MIDI CI", "MIDI 2.0 JUCE" | MIDI Association, MIDI 2.0 spec |

---

## 6. Index and Cross-Reference Updates

### master-index.json Additions

```json
{
  "knowledge_bases": {
    "testing-kb": {
      "path": "testing-kb",
      "description": "Audio plugin testing and validation",
      "source": "curated",
      "status": "ready",
      "file_count": 10,
      "topics": ["unit-testing", "daw-testing", "validation", "regression", "performance-testing"]
    },
    "midi-kb": {
      "path": "midi-kb",
      "description": "MIDI protocol and implementation for audio plugins",
      "source": "curated",
      "status": "ready",
      "file_count": 16,
      "topics": ["protocol", "realtime", "mpe", "controllers", "advanced"]
    }
  },
  "cross_references": {
    "reverb": { "description": "Reverb algorithms and design", "kbs": ["dsp-kb"] },
    "compression": { "description": "Dynamic range compression", "kbs": ["dsp-kb"] },
    "chorus": { "description": "Chorus and modulation effects", "kbs": ["dsp-kb"] },
    "pitch_shift": { "description": "Pitch shifting algorithms", "kbs": ["dsp-kb"] },
    "preset_design": { "description": "Preset design methodology", "kbs": ["sound-design-kb"] },
    "modulation": { "description": "Modulation routing and matrices", "kbs": ["sound-design-kb", "juce-kb"] },
    "layering": { "description": "Sound layering techniques", "kbs": ["sound-design-kb"] },
    "realtime": { "description": "Real-time audio programming", "kbs": ["juce-kb", "cpp-kb"] },
    "simd": { "description": "SIMD optimization", "kbs": ["juce-kb", "dsp-kb"] },
    "testing": { "description": "Plugin testing", "kbs": ["testing-kb", "juce-kb"] },
    "validation": { "description": "Plugin validation", "kbs": ["testing-kb"] },
    "daw_testing": { "description": "DAW-specific testing", "kbs": ["testing-kb"] },
    "midi": { "description": "MIDI protocol and implementation", "kbs": ["midi-kb", "juce-kb"] },
    "mpe": { "description": "MIDI Polyphonic Expression", "kbs": ["midi-kb"] },
    "midi_learn": { "description": "MIDI learn and mapping", "kbs": ["midi-kb", "juce-kb"] },
    "midi_timing": { "description": "MIDI timing and jitter", "kbs": ["midi-kb"] },
    "controllers": { "description": "MIDI controller integration", "kbs": ["midi-kb"] }
  }
}
```

### Individual KB Index Updates

Each KB's `index.json` will be updated with new topics and file references following the existing pattern.

---

## 6. Harvest Strategy

### Source Priority

1. **CCRMA Stanford** (already in dsp-kb) - Continue harvesting DSP papers
2. **EarLevel Blog** - DSP algorithms, filter design
3. **musicdsp.org** - Classic DSP code
4. **Sound on Sound** - Sound design, mixing
5. **Attack Magazine** - Sound design techniques
6. **JUCE Forum** - Real-world solutions
7. **melatonin.dev** - JUCE-specific patterns
8. **DSP Stack Exchange** - Q&A format solutions
9. **AES Papers** - Academic DSP research

### Credit Allocation

| KB | Topics | Estimated Credits |
|----|--------|-------------------|
| dsp-kb | 15 topics | 3,500 |
| sound-design-kb | 10 topics | 2,500 |
| juce-kb | 12 topics | 3,000 |
| testing-kb | 10 topics | 2,500 |
| midi-kb | 16 topics | 3,500 |
| Index/Cross-ref updates | - | 500 |
| **Buffer/Retry** | - | 1,000 |
| **Reserved** | - | 3,500 |
| **Total** | 63 topics | ~16,500 |

---

## 7. Implementation Order

1. **Create folder structure** - All new directories
2. **Update master-index.json** - Add testing-kb, new cross-references
3. **Create placeholder files** - Empty JSON with structure
4. **Update individual KB indexes** - Add new topics
5. **Harvest content** - By priority source
6. **Validate cross-references** - Ensure all references resolve

---

## 8. Success Criteria

- All 63 topics have populated JSON files
- Cross-references resolve correctly
- Search terms produce relevant results
- File counts match index metadata
- No orphan files in KB directories
- Harvest stays within credit budget
- midi-kb and testing-kb created with proper structure

---

## Appendix A: Search Term Template

Each topic folder gets a `search-terms.json` file for harvest reference:

```json
{
  "topic": "reverb",
  "category": "dsp-kb",
  "primary_concepts": ["reverb", "reverberation", "room simulation"],
  "search_terms": {
    "algorithmic": ["Freeverb", "Schroeder reverb", "feedback delay network", "FDN"],
    "convolution": ["convolution reverb", "FFT convolution", "zero latency convolution"],
    "design": ["reverb parameters", "early reflections", "late field", "damping"]
  },
  "sources": {
    "academic": ["CCRMA papers", "AES Journal"],
    "blogs": ["EarLevel", "musicdsp.org"],
    "documentation": ["JUCE DSP module"]
  },
  "harvest_priority": "high"
}
```

This file is optional for harvesting - systems should work without it, but it improves search quality.

---

## Appendix B: File Structure Validation

After implementation, validate:

```bash
# Check all KBs have valid structure
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb testing-kb midi-kb; do
  echo "=== $kb ==="
  find $kb -name "*.json" -not -name "index.json" -not -name "manifest.json" | wc -l
done

# Check cross-references resolve
python3 scripts/validate-cross-refs.py

# Check no orphan files
python3 scripts/check-orphans.py
```