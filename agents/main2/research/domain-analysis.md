# Domain Analysis: VST Audio Plugin Development

## Standard Processes

### Development Lifecycle
1. **Concept & Specification** — Define plugin type, signal flow, sound identity, parameters
2. **Architecture** — Class design, DSP algorithm specification, parameter layout
3. **Project Setup** — CMake configuration, test framework, CI pipeline
4. **DSP Implementation** — Module-by-module development with two-stage review
5. **State Management** — APVTS integration, preset system, state save/restore
6. **Processor Integration** — Wire DSP modules, parameter smoothing, processBlock
7. **GUI Implementation** — LookAndFeel, components, APVTS attachments
8. **Build Verification** — Cross-platform, sample rate/buffer stress tests
9. **DAW Testing** — Human perceptual testing, preset audition
10. **Release** — Binary packaging, documentation, distribution
11. **Optimization** — CPU profiling, denormal handling, memory audit

### Plugin Types
| Type | Description | Key Considerations |
|------|-------------|-------------------|
| Synthesizer | Generates audio from scratch | Voice management, modulation routing, polyphony |
| Effect | Processes incoming audio | Latency compensation, mono handling, wet/dry mix |
| Analyzer | Visualizes audio data | GUI performance, thread safety, real-time display |
| Hybrid | Combines synthesis + effects | Signal flow complexity, CPU budget |
| Tool | Utility function | Simplicity, reliability, DAW integration |

### Key Performance Indicators (KPIs)
| KPI | Target | Measurement |
|-----|--------|-------------|
| CPU Usage | < 5% at 48kHz/512 buffer | DAW CPU meter, perf profiling |
| Latency | As low as algorithm allows | getLatencySamples() |
| Memory | < 50MB typical | Process Explorer, valgrind |
| Instability | 0 crashes in 1000hr | Crash reports, user feedback |
| State Integrity | 100% round-trip | Automated state tests |

## Common Failure Modes

### Audio Thread Safety (Critical)
| ID | Symptom | Cause | Prevention |
|----|---------|-------|------------|
| FM-01 | Dropouts, crashes | Heap allocation on audio thread | Pre-allocate in prepareToPlay() |
| FM-02 | Crash at 96kHz+ | Fixed-size buffers | Scale by sample rate |
| FM-04 | CPU spikes on silence | Denormal floats | ScopedNoDenormals + DC offset |
| FM-05 | Audio explosion | Feedback gain ≥ 1.0 | Clamp to 0.95, soft-clip |
| FM-07 | Intermittent crashes | Non-atomic cross-thread ops | Use std::atomic<bool> flags |

### Parameter & State (Serious)
| ID | Symptom | Cause | Prevention |
|----|---------|-------|------------|
| FM-03 | Params reset on reload | Missing state save/restore | Implement APVTS serialization |
| FM-09 | Zipper noise | Wrong SmoothedValue API | Call getNextValue() per-sample |
| FM-16 | Automation breaks | Parameter ID changes | Lock IDs before release |

### Signal Processing (Serious)
| ID | Symptom | Cause | Prevention |
|----|---------|-------|------------|
| FM-08 | Silent right channel | Mono input handling | Copy ch0 to ch1 |
| FM-10 | Wrong filter at diff rates | Hardcoded coefficients | Recalculate in prepareToPlay() |
| FM-13 | Metallic distortion | No oversampling | Use 2x/4x oversampling |
| FM-14 | Volume dip at 50% mix | Linear crossfade | Equal-power cosine mixing |

### Build & Distribution (Moderate)
| ID | Symptom | Cause | Prevention |
|----|---------|-------|------------|
| FM-18 | Instance interference | Static mutable state | All state per-instance |
| FM-21 | Build failures | Stale CMake cache | Clean build in CI |
| FM-23 | pluginval failures | Various quality issues | Run locally before push |

## Regulatory & Compliance

### Plugin Format Requirements
| Format | Platform | Requirements |
|--------|----------|--------------|
| VST3 | Cross-platform | Steinberg SDK license (free), VST3 SDK |
| AU | macOS only | Apple Developer account, code signing |
| AAX | Pro Tools | Avid SDK license, iLok Pace protection |
| CLAP | Cross-platform | MIT license, open standard |

### Code Signing
- **macOS:** Apple Developer certificate required for AU distribution
- **Windows:** Code signing recommended, not required
- **Linux:** No signing requirement

## Terminology Glossary

| Term | Definition |
|------|------------|
| APVTS | AudioProcessorValueTreeState — JUCE's parameter management class |
| processBlock | JUCE callback that processes audio buffers |
| prepareToPlay | JUCE callback that prepares the plugin for playback |
| SmoothedValue | JUCE class for parameter smoothing (ramping) |
| Latency compensation | DAW feature to align delayed signals |
| Denormal | Subnormal floating-point numbers causing CPU spikes |
| Equal-power mix | Crossfade preserving constant power (cosine/sine) |
| Oversampling | Processing at higher rate to avoid aliasing |
| FDN | Feedback Delay Network reverb topology |

## Sources
- [Real-Time Safe Multi-Threaded DAW Audio](https://edwloef.github.io/posts/real-time-safe-multi-threaded-daw-audio/)
- [Developing Audio Plugins (Nathan Blair)](https://nthnblair.com/thesis/)
- JUCE Forum: Best practices for VST3 releases
- VST Plugin Playbook v7 (existing playbook)