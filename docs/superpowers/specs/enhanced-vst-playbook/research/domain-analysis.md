# Domain Analysis: VST Audio Plugin Development

> Comprehensive domain research for the enhanced VST playbook. Covers standard processes, failure modes, terminology, KPIs, and regulatory requirements.

---

## 1. Standard Processes

### The 13-Phase VST Development Workflow

VST plugin development follows a gated workflow with quality gates between each phase:

| Phase | Name | Purpose | Key Deliverables |
|-------|------|---------|------------------|
| 0 | Plugin Concept & Specification | Define WHAT to build | SPEC.md with signal flow, parameters, sound identity |
| 1 | Performance Audit | Audit existing code (if upgrading) | Thread safety audit, allocation audit |
| 2 | Architecture & Task Breakdown | Turn spec into buildable plan | Class diagram, DSP algorithms, task list |
| 3 | Project Setup & Test Framework | Establish build infrastructure | CMake config, test framework, CI pipeline |
| 4 | Core DSP Implementation | Build audio engine | DSP modules with two-stage review |
| 5 | Preset Data & State Management | Implement presets | PresetData, PresetManager, state serialization |
| 6 | Processor Integration | Wire modules together | processBlock, parameter layout, SmoothedValue audit |
| 7 | GUI/Editor Implementation | Build visual interface | LookAndFeel, components, visual polish |
| 8 | Build Verification & Cross-Platform | Validate builds | Multi-platform, sample rates, buffer sizes |
| 9 | DAW Testing & Sound Approval | Human perceptual testing | DAW testing checklist, preset validation |
| 10 | Release & Distribution | Package and deploy | Binary distribution, documentation |
| 11 | Optimization & Hardening | Performance tuning | CPU profiling, memory audit |
| 12 | Retrospective & Reusable Components | Learn and improve | Process analysis, template extraction |

### Critical Process Rules

1. **Two-Stage Code Review**: Stage 1 checks spec compliance; Stage 2 checks code quality
2. **Human-Only Creative Decisions**: AI never proposes defaults for sound/visuals
3. **Gated Progression**: Each phase must pass before the next begins
4. **Audio Thread Safety**: Mandatory audit before any phase gate passes

### Development Workflow Patterns

| Pattern | Description | When Used |
|---------|-------------|-----------|
| **Greenfield** | New plugin from scratch | Phase 0-12 complete |
| **Upgrade Existing** | Improve existing plugin | Phase 1 audit, then Phase 2-12 |
| **Variant** | New plugin from template | Skip Phase 0, use template spec |

---

## 2. Failure Modes

### Critical Failure Modes (Crashes, Data Loss)

| ID | Symptom | Cause | Prevention |
|----|---------|-------|-------------|
| FM-01 | Audio dropouts, crashes | Heap allocation on audio thread | Pre-allocate buffers, no new/delete in processBlock |
| FM-02 | Crash at high sample rates | Buffer overflow at 96kHz/192kHz | Scale buffers by sample rate |
| FM-03 | Parameters reset on DAW reopen | Empty state serialization | Implement APVTS state save/restore |
| FM-04 | CPU spikes on silence | Denormal floating-point values | ScopedNoDenormals in processBlock |
| FM-05 | Audio grows to infinity | Feedback gain >= 1.0 | Clamp feedback to 0.95 max |
| FM-06 | Crash on editor re-open | Dangling listener | Match addListener with removeListener |
| FM-07 | Intermittent crashes | Thread safety violation | Use atomic flags, Timer for UI updates |

### Serious Failure Modes (Incorrect Audio)

| ID | Symptom | Cause | Prevention |
|----|---------|-------|-------------|
| FM-08 | Silent right channel | Not handling mono input | Copy channel 0 to channel 1 |
| FM-09 | Zipper noise | SmoothedValue not advanced | Call getNextValue() per sample |
| FM-10 | Wrong sound after rate change | Coefficients not recalculated | Update in prepareToPlay |
| FM-11 | Click on bypass toggle | Discontinuity at switch | Smooth bypass with crossfade |
| FM-12 | Buffer overrun/underrun | Fixed block size assumption | Use buffer.getNumSamples() |
| FM-13 | Metallic artifacts | Aliasing in nonlinear processing | Use oversampling |
| FM-14 | Volume dip at 50% wet/dry | Linear crossfade | Use equal-power cosine mixing |

### Moderate Failure Modes (Compatibility)

| ID | Symptom | Cause | Prevention |
|----|---------|-------|-------------|
| FM-15 | AU not discovered | Wrong location or stale cache | Copy to correct path, reset cache |
| FM-16 | Automation breaks | Parameter ID hash changed | Never change parameter IDs |
| FM-17 | Crash on close | LookAndFeel destruction order | Declare LookAndFeel before components |
| FM-18 | Instance interference | Static/singleton state | All state as instance members |
| FM-19 | Flickering UI | Excessive repaint() | setOpaque(true), setBufferedToImage(true) |
| FM-20 | Windows plugin fails | MSVC runtime linking | Statically link runtime |

### Session-Specific Bugs (Epiphany Machine v3)

16 real bugs found during production session:

| ID | Bug | Key Lesson |
|----|-----|------------|
| EM-13 | SmoothedValue never advanced | Silent bugs—no crash, just dead parameters |
| EM-14 | Linear mix blend | Use equal-power cosine mixing |
| EM-15 | Default decay too long | Default decay <= 2.0s for responsiveness |

**Critical insight**: EM-13, EM-14, EM-15 were caught by USER in DAW testing, not automated review. Human DAW testing is irreplaceable.

---

## 3. Terminology Glossary

### Audio Thread Concepts

| Term | Definition |
|------|------------|
| **Audio Thread** | Real-time thread where processBlock() runs. Must never block, allocate, or do I/O. |
| **processBlock()** | JUCE callback that processes audio. Must complete within bufferTime (typically 1-10ms). |
| **prepareToPlay()** | JUCE callback for initialization. Called on sample rate or buffer size change. |
| **Real-Time Safe** | Code guaranteed to complete within deadline. No allocations, locks, or I/O. |
| **Denormals** | Floating-point values near zero causing 10-100x CPU stalls. Fixed with ScopedNoDenormals. |

### DSP Terminology

| Term | Definition |
|------|------------|
| **APVTS** | AudioProcessorValueTreeState - JUCE's parameter management system. |
| **SmoothedValue** | JUCE class for click-free parameter ramping. Must call getNextValue() per sample. |
| **FDN** | Feedback Delay Network - algorithmic reverb using N delay lines + mixing matrix. |
| **TPT SVF** | Topology-Preserving State Variable Filter - stable at all frequencies, modulatable. |
| **Waveshaper** | Transfer function for distortion/saturation. Often needs oversampling. |
| **PolyBLEP** | Polynomial Band-Limited Step - anti-aliasing technique for hard-edged waveforms. |
| **Equal-Power Panning** | Cosine/sine mixing preserving loudness at center. |

### JUCE Architecture Terms

| Term | Definition |
|------|------------|
| **AudioProcessor** | Base class for all plugins. Contains DSP logic and parameter management. |
| **AudioProcessorEditor** | GUI base class. Owned by AudioProcessor, created/destroyed by DAW. |
| **AudioBuffer<float>** | JUCE's main audio buffer type. getWritePointer() for raw access. |
| **Component** | Base class for all GUI elements. paint() for drawing, resized() for layout. |
| **LookAndFeel_V4** | JUCE's theming system. Override draw* methods for custom rendering. |

### Plugin Format Terms

| Term | Definition |
|------|------------|
| **VST3** | Steinberg's plugin format. Cross-platform, current standard. |
| **AU** | Audio Units - Apple's plugin format. Required for macOS DAWs. |
| **AAX** | Avid's plugin format. Required for Pro Tools. |
| **pluginval** | Cross-platform validation tool. Strictness 5+ for release. |

---

## 4. Key Performance Indicators (KPIs)

### Quality Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| **pluginval strictness** | Level 5+ pass | Run pluginval binary |
| **State round-trip** | 100% preserved | Save, close, reopen, verify |
| **Sample rate range** | 44.1k, 48k, 96k, 192k | Test at each rate |
| **Buffer size range** | 64 to 2048 samples | Test all standard sizes |
| **Multi-instance isolation** | 4+ instances, no cross-talk | Static audit, load test |
| **Editor cycles** | 10 open/close cycles | No crashes |

### Performance Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| **processBlock CPU** | < bufferTime | Perfetto profiling |
| **Denormal handling** | CPU stays low on silence | 60s silence test |
| **Memory allocations** | Zero in processBlock | Code audit |
| **UI repaint** | < 60fps | JUCE_ENABLE_REPAINT_DEBUGGING |

### Development Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| **Phase gate pass rate** | 90%+ first pass | Track gate rejections |
| **Time to first plugin** | < 40 hours | Phase 0 to Phase 8 |
| **Time to first sound** | < 10 hours | Phase 4 audible output |
| **Bug rate in DAW testing** | < 5 critical bugs | Phase 9 human testing |

---

## 5. Regulatory and Compliance Requirements

### Audio Standards

| Standard | Requirement |
|----------|-------------|
| **VST3 SDK** | Comply with Steinberg guidelines |
| **Audio safety** | No clipping above 0dB without warning |
| **CPU efficiency** | Must not exceed reasonable limits |
| **Plugin validation** | Must pass VST3 validator |

### Platform Paths

| Platform | VST3 Path | Notes |
|----------|-----------|-------|
| Linux | ~/.vst3/ | No code signing |
| macOS | /Library/Audio/Plug-Ins/VST3/ | Code signing + notarization |
| Windows | C:\Program Files\Common Files\VST3\ | Statically link MSVC runtime |

### Code Signing

| Platform | Method |
|----------|--------|
| macOS | codesign + notarytool (required 10.15+) |
| Windows | Azure Trusted Signing |
| Linux | None required |

### Pre-Release Validation Checklist

```
[ ] pluginval at strictness 5+ passes
[ ] State save/restore round-trips correctly
[ ] Tested at 44.1k, 48k, 96k sample rates
[ ] Tested at buffer sizes: 64, 128, 256, 512, 1024, 2048
[ ] Process 60s silence - CPU stays low
[ ] Process full-scale noise - no NaN/Inf
[ ] Multiple instances don't interfere
[ ] Editor open/close 10 times rapidly
[ ] Automation record/playback works
[ ] DAW bypass produces clean dry signal
[ ] No DBG() calls in Release
[ ] No static mutable state
[ ] All addListener calls have matching removeListener
[ ] Output never exceeds 0dBFS
```

---

## 6. Sources

- JUCE Official Documentation (juce.com/learn/)
- JUCE Coding Standards
- Will Pirkle Audio Technology (willpirkle.com)
- The Art of VA Filter Design (Vadim Zavalishin)
- Steinberg VST3 SDK Documentation
- pluginval (github.com/Tracktion/pluginval)
- melatonin.dev blog
- Epiphany Machine v3 build session (24,292 lines)