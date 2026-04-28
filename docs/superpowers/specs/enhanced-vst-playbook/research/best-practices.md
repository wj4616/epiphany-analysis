# Best Practices and Standards

> Published standards, expert recommendations, common process frameworks, and pitfalls for VST audio plugin development.

---

## 1. Published Standards

### Audio Plugin Format Standards

| Format | Version | Governing Body | Required For |
|--------|---------|----------------|--------------|
| **VST3** | 3.8.x | Steinberg | Cross-platform standard |
| **AU** | Current | Apple | macOS DAWs |
| **AAX** | Current | Avid | Pro Tools |
| **CLAP** | 1.x | CLAP Foundation | Emerging open standard |

### VST3 SDK Standards (Steinberg)

- Separate processor/controller architecture
- Parameter normalization (0.0 to 1.0 range)
- State persistence via XML streams
- Automation and preset support requirements
- Reference: https://steinbergmedia.github.io/vst3_doc/

### JUCE Framework Standards

- **C++17 minimum** - Modern features required
- **Naming conventions** - camelCase variables/methods, PascalCase classes
- **Memory management** - Never use `delete`; use smart pointers
- **Thread safety** - No allocations or locks on audio thread
- **Ownership** - Prefer composition over inheritance

---

## 2. Expert Recommendations

### Essential Books

| Book | Author | Coverage | Priority |
|------|--------|----------|----------|
| **Designing Audio Effect Plugins in C++** (2nd Ed) | Will Pirkle | VST3/AU/AAX, DSP theory, 50+ objects | **Essential** |
| **Designing Software Synthesizer Plugins in C++** | Will Pirkle | Synth architecture, VA filters, wavetables | **Essential** |
| **The Art of VA Filter Design** | Vadim Zavalishin | TPT filters, Cytomic SVF, analog modeling | **Essential** |
| **The Audio Programming Book** | Richard Boulanger | DSP fundamentals, audio coding | Recommended |
| **Physical Audio Signal Processing** | Julius O. Smith III | FDN reverbs, delay networks | Reference |

### Academic Resources

| Resource | Author | Coverage |
|----------|--------|----------|
| CCRMA Courses | Stanford (Julius O. Smith) | FDN reverbs, filters, granular synthesis |
| Cytomic DSP Papers | Andy Simper | TPT SVF (industry standard filter) |
| DSPRelated.com | Community | DSP theory and implementation |

### Community Libraries (Production-Ready)

| Library | Modules | Source |
|---------|---------|--------|
| **chowdsp_utils** | 30+ modules: filters, waveshapers (ADAA), FDN reverb, oscillators | GitHub: Chowdhury-DSP |
| **sst-filters** | Surge synthesizer filters | GitHub: surge-synthesizer |
| **sst-waveshapers** | Surge waveshapers | GitHub: surge-synthesizer |
| **signalsmith-stretch** | Polyphonic pitch/time stretching | GitHub: Signalsmith-Audio |
| **Gin** | Large utility collection, StackBlur, websockets | GitHub: FigBug |
| **ff_meters** | LED-style level meters | GitHub: ffAudio |

### Professional Courses

| Course | Provider | URL |
|--------|----------|-----|
| **JUCE Official Course** | JUCE | juce.com/learn/course |
| **JUCE Tutorials** | JUCE | juce.com/learn/tutorials (62 tutorials) |

---

## 3. Common Process Frameworks

### The 13-Phase Framework

| Phase | Purpose | Key Outputs |
|-------|---------|-------------|
| **0** | Concept & Spec | SPEC.md with signal flow, parameters, sound identity |
| **1** | Performance Audit | Thread safety, memory audit (if upgrading) |
| **2** | Architecture | Class diagram, DSP algorithms, ordered task list |
| **3** | Project Setup | CMake config, test framework, CI pipeline |
| **4** | DSP Implementation | DSP modules with two-stage review |
| **5** | Preset Data | PresetData, PresetManager, state serialization |
| **6** | Processor Integration | processBlock, parameter layout, SmoothedValue audit |
| **7** | GUI Implementation | LookAndFeel, components, visual polish |
| **8** | Build Verification | Multi-platform, sample rates, buffer sizes |
| **9** | DAW Testing | Human listening tests, preset validation |
| **10** | Release | Binary distribution, documentation |
| **11** | Optimization | CPU profiling, memory audit |
| **12** | Retrospective | Process analysis, template extraction |

### Two-Stage Review Process

**Stage 1 - Spec Compliance**: Does implementation match spec? Correct algorithm, formula constants, parameter ranges, signal flow.

**Stage 2 - Code Quality**: Independent of spec. Buffer overflows, off-by-one errors, thread safety, edge cases, performance.

**Key Insight**: Two-stage review catches implementation bugs but CANNOT catch perceptual/UX issues. Human DAW testing is irreplaceable.

### Validation Framework (pluginval)

| Strictness | What It Tests |
|-----------|---------------|
| 1-4 | Basic call coverage, crash testing |
| **5** (minimum) | Host compatibility, state round-trip |
| 6-9 | Extended parameter fuzzing, multiple state restoration |
| 10 | Real-time safety checks, full stress testing |

---

## 4. Common Pitfalls

### Critical Pitfalls (Crashes)

| Pitfall | Symptom | Prevention |
|---------|---------|------------|
| Heap allocation in processBlock | Dropouts, crashes | Pre-allocate in prepareToPlay |
| Buffer overflow at high rates | Crash at 96kHz/192kHz | Scale buffers by sample rate |
| Empty state serialization | Parameters reset | Implement APVTS state save |
| Denormal CPU explosion | CPU spikes on silence | ScopedNoDenormals |
| Feedback instability | Audio grows to infinity | Clamp feedback to 0.95 |
| Dangling listener | Crash on editor re-open | Match addListener/removeListener |

### Serious Pitfalls (Incorrect Audio)

| Pitfall | Symptom | Prevention |
|---------|---------|------------|
| Missing mono handling | Silent right channel | Copy channel 0 to 1 |
| SmoothedValue not advanced | Zipper noise | Call getNextValue() every sample |
| Stale coefficients | Wrong sound at different rates | Update in prepareToPlay |
| Click on bypass | Discontinuity | Smooth bypass with crossfade |
| Aliasing in distortion | Metallic artifacts | Use oversampling |
| Linear dry/wet mix | Volume dip at 50% | Use equal-power cosine mixing |

### Moderate Pitfalls (Compatibility)

| Pitfall | Symptom | Prevention |
|---------|---------|------------|
| AU not discovered | Plugin not in DAW list | Copy to correct path, reset cache |
| Parameter ID change | Automation breaks | Never change IDs after release |
| LookAndFeel destruction | Crash on close | Declare before components |
| Static mutable state | Instance interference | All state as instance members |
| Excessive repaint() | Flickering UI | setOpaque(true), buffered images |
| MSVC runtime linking | Fails on some Windows | Statically link runtime |

---

## 5. Mandatory Patterns

### SmoothedValue Pattern

```cpp
// prepareToPlay
smoothedValue.reset(sampleRate, 0.02);  // 20ms for continuous params
smoothedValue.setCurrentAndTargetValue(initialValue);

// processBlock (once per block)
smoothedValue.setTargetValue(newValue);

// per-sample loop
for (int i = 0; i < numSamples; ++i) {
    float value = smoothedValue.getNextValue();  // MUST call every sample
    // use value...
}
```

### Equal-Power Mixing Pattern

```cpp
// Dry/wet mixing - ALWAYS use equal-power
float dryGain = std::cos(mix * juce::MathConstants<float>::halfPi);
float wetGain = std::sin(mix * juce::MathConstants<float>::halfPi);
```

### Audio Thread Safety Rules

```
NEVER in processBlock:
  - new, delete, malloc, free
  - std::vector::push_back, std::string operations
  - DBG() or any console output
  - File I/O
  - Network I/O
  - Locks, mutexes, condition variables
  - Any blocking operation

ALWAYS in processBlock:
  - Use pre-allocated buffers
  - Use std::atomic<bool> for cross-thread flags
  - Check atomic flags in Timer callback for UI
  - ScopedNoDenormals at start
```

---

## 6. Pre-Release Checklist

```
[ ] pluginval at strictness 5+ passes on all target formats
[ ] State save/restore round-trips correctly
[ ] Tested at 44.1k, 48k, 96k sample rates (192k for safety)
[ ] Tested at buffer sizes: 64, 128, 256, 512, 1024, 2048
[ ] Process 60 seconds of silence - CPU stays low
[ ] Process full-scale noise - no NaN/Inf in output
[ ] Multiple instances simultaneously without interference
[ ] Editor open/close 10 times rapidly without crash
[ ] Automation record/playback works for all parameters
[ ] DAW bypass produces clean dry signal
[ ] No DBG() calls in Release build
[ ] No static mutable state
[ ] All addListener calls have matching removeListener in destructors
[ ] Output never exceeds 0dBFS (limiter/tanh present)
```

---

## 7. Sources

- JUCE Official Documentation (juce.com/learn/)
- JUCE Coding Standards
- Will Pirkle Audio Technology (willpirkle.com)
- Designing Audio Effect Plugins in C++ (Routledge)
- The Art of VA Filter Design (DSPRelated)
- pluginval (github.com/Tracktion/pluginval)
- melatonin.dev blog
- awesome-juce (github.com/sudara/awesome-juce)
- Epiphany Machine v3 session analysis