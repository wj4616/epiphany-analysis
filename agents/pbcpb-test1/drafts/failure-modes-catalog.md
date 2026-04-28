# Failure Modes Catalog

This document catalogs all failure modes relevant to VST plugin development and commercial release, organized by category.

---

## Audio Thread Failures (FM-01 to FM-07)

These are critical failures that cause audio glitches, crashes, or unbounded latency.

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-01 | Memory allocation in processBlock | `new`, `malloc`, `std::vector` resize in audio callback | Sanitizers, audio thread audit | Pre-allocate in prepareToPlay |
| FM-02 | Lock acquisition in processBlock | `std::mutex`, `std::lock_guard` in audio callback | Audio thread audit | Use `std::atomic`, lock-free queues |
| FM-03 | Blocking I/O in processBlock | File read/write, network, printf in audio callback | Profiling | Async message passing |
| FM-04 | Unbounded iteration | Loop without worst-case bound in audio code | Code review | Hard limits, early exit |
| FM-05 | Virtual function overhead | Unbounded virtual dispatch in audio loop | Profiling | `final` keyword, static dispatch |
| FM-06 | Dynamic casting | `dynamic_cast` in audio code | Code review | Static type design |
| FM-07 | Exception throwing | Exceptions thrown in audio code | Sanitizers | No exceptions, use error codes |

---

## Parameter Failures (FM-08 to FM-15)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-08 | Parameter out of range | Value outside [0,1] or valid range | Unit tests, assertions | Clamp in setter |
| FM-09 | Parameter not smoothed | Abrupt changes cause clicks | Listening test, code review | SmoothedValue for all |
| FM-10 | Parameter connection broken | UI doesn't affect audio | Integration test | Verify each parameter |
| FM-11 | Wrong parameter type | Linear where dB needed | Code review, listening | Match parameter type to DSP |
| FM-12 | Missing parameter reset | Parameter not reset on preset load | Preset test | Clear all in setState |
| FM-13 | Dead parameter (EM-13) | Parameter exists but doesn't affect DSP | Parameter audit | Trace each to DSP |
| FM-14 | Duplicate parameter IDs | ID collision in APVTS | Build error | Unique IDs |
| FM-15 | Parameter name conflict | Same name, different function | Code review | Descriptive naming |

---

## DSP Failures (FM-16 to FM-23)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-16 | Denormals | Very small numbers cause CPU spikes | Profiling, CPU meter | Flush denormals to zero |
| FM-17 | NaN/Inf propagation | Invalid math creates NaN, spreads | Assertions, tests | Guard math, clamp |
| FM-18 | Gain staging error | Signal too hot or too cold | Metering | Measure at each stage |
| FM-19 | Sample rate assumption | Code assumes 44.1kHz | Multi-rate test | Use prepareToPlay rate |
| FM-20 | Block size assumption | Assumes 512 samples | Variable block test | Handle any size |
| FM-21 | Channel count mismatch | Assumes stereo | Mono test | Check channel count |
| FM-22 | Feedback instability | Feedback loop runs away | Listening, metering | Limit feedback gain |
| FM-23 | DC offset | Signal has DC component | High-pass test | DC blocking filter |

---

## Build Failures (FM-24 to FM-30)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-24 | CMake configuration error | Dependency not found, wrong version | Build fails | Use FetchContent, pin versions |
| FM-25 | Link error | Missing symbol | Build fails | Check all symbols exported |
| FM-26 | Plugin won't load | Invalid VST3 structure | DAW test | Use juce_add_plugin correctly |
| FM-27 | Wrong install path | Plugin in wrong directory | DAW can't find | Check VST3 path |
| FM-28 | Version mismatch | Header/lib version conflict | Runtime error | Consistent versions |
| FM-29 | Missing resources | Files not in bundle | Runtime error | Bundle resources correctly |
| FM-30 | Debug build shipped | Slow, contains assertions | Performance test | Use Release config |

---

## GUI Failures (FM-31 to FM-38)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-31 | UI thread blocks audio | GUI update in audio callback | Profiling | AsyncUpdater, timer |
| FM-32 | LookAndFeel leak | Not resetting LookAndFeel | Sanitizers | Reset in destructor |
| FM-33 | Scaling broken | Controls don't scale with window | Resize test | Use resizable editor |
| FM-34 | HiDPI blur | Not handling high DPI | HiDPI test | Set explicit scale |
| FM-35 | Control overlap | Controls cover each other | Visual test | Careful layout |
| FM-36 | Missing tooltip | User doesn't know what control does | UX review | Add tooltips |
| FM-37 | Accessibility missing | Screen reader can't read | Accessibility test | Set descriptions |
| FM-38 | Theme inconsistent | Controls look different | Visual review | Single LookAndFeel |

---

## State Management Failures (FM-39 to FM-45)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-39 | State not saved | get/setStateInformation not implemented | DAW project save test | Implement both |
| FM-40 | State corrupted | XML parsing fails | Malformed input test | Validate input |
| FM-41 | Preset path wrong | Can't find factory presets | Clean install test | Use correct path |
| FM-42 | Preset version mismatch | Old preset won't load | Version test | Version field in preset |
| FM-43 | Automation broken | DAW automation doesn't work | Automation test | Export automatable params |
| FM-44 | Undo/redo missing | No undo support | UX test | Implement undo if needed |
| FM-45 | State size too large | Preset file is huge | File size check | Compress if needed |

---

## Testing Failures (FM-46 to FM-52)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-46 | No unit tests | Code untested | CI failure | Write tests first |
| FM-47 | Test doesn't compile | Outdated tests | CI failure | Keep tests in sync |
| FM-48 | Flaky test | Test sometimes fails | CI intermittent | Deterministic tests |
| FM-49 | Missing edge case test | Boundary conditions untested | Bug in production | Test param=0, param=1 |
| FM-50 | No DAW testing | Only tested standalone | User reports bugs | Test in target DAW |
| FM-51 | No regression tests | Bug reintroduced | CI should catch | Regression suite |
| FM-52 | Coverage too low | Large code paths untested | Coverage report | Aim for >80% |

---

## Commercial Failures (FM-53 to FM-60)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-53 | License check fails | Legitimate users blocked | Support tickets | Test thoroughly |
| FM-54 | License easily bypassed | Cracked immediately | Piracy monitoring | Accept basic protection |
| FM-55 | Marketplace rejection | Listing not approved | Rejection notice | Follow guidelines |
| FM-56 | Wrong pricing | Too high or too low | Sales data | Market research |
| FM-57 | No marketing | Product invisible | No sales | Marketing plan |
| FM-58 | Poor product page | Low conversion | Analytics | A/B test |
| FM-59 | Demo too limited | Users can't evaluate | Support requests | Balance protection/evaluation |
| FM-60 | Payment processing fail | Can't purchase | User reports | Test purchase flow |

---

## Project Failures (FM-61 to FM-67)

| ID | Failure Mode | Description | Detection | Prevention |
|----|--------------|-------------|-----------|------------|
| FM-61 | Scope creep | Feature bloat | Timeline slip | Lock scope in Phase 0 |
| FM-62 | No spec | Building without plan | Confusion | Write spec first |
| FM-63 | Human didn't decide | AI made creative choice | User rejects | Human-only decisions |
| FM-64 | Session handoff failed | Context lost between sessions | Confusion | Update tracking files |
| FM-65 | KB not populated | Missing guidance | Poor implementations | Seed KB early |
| FM-66 | Session budget exceeded | Token limit hit mid-task | Truncated work | Define session boundaries |
| FM-67 | Documentation outdated | Docs don't match code | User confusion | Update with code |

---

## Failure Mode Summary by Phase

| Phase | Relevant FMs |
|-------|--------------|
| 0 | FM-61, FM-62, FM-63 |
| 1 | FM-17 (existing code) |
| 2 | FM-61, FM-64 |
| 3 | FM-24, FM-25, FM-26, FM-27 |
| 4 | FM-01 to FM-23 |
| 5 | FM-08 to FM-15, FM-39 to FM-45 |
| 6 | FM-10, FM-18, FM-22 |
| 7 | FM-31 to FM-38 |
| 8 | FM-28, FM-29, FM-30 |
| 9 | FM-18, FM-22, FM-50 |
| 10 | FM-01 to FM-07, FM-13, FM-16 |
| 11 | FM-16, FM-17 |
| 12 | FM-29, FM-30 |
| 13 | FM-53, FM-54, FM-59 |
| 14 | FM-55, FM-58 |
| 15 | FM-57 |
| 16 | FM-56, FM-60 |
| 17 | FM-67 |

---

## Total Failure Modes: 67

- Audio Thread: 7
- Parameters: 8
- DSP: 8
- Build: 7
- GUI: 8
- State: 7
- Testing: 7
- Commercial: 8
- Project: 7