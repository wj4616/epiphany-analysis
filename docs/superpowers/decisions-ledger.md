# Decisions Ledger

**Project:** Psycog VST
**Session ID:** VST-LIFECYCLE-2026-04-02-001

---

## Phase 0: Specification

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | Use pre-approved spec at `docs/superpowers/specs/2026-04-02-psycog-vst-design.md` | Coordinator | Spec version 1.5 already approved for implementation |
| 2026-04-02 | Sound identity: "constantly morphing, never settling, never harsh" | Stakeholder | Human-authored creative language, not paraphrased |
| 2026-04-02 | Signal flow: Time-stretch → Wavefolding → Wet processing → Mix → Output | Stakeholder | Human-drawn signal flow diagram |
| 2026-04-02 | Target genre: Ambient/psybient + Glitch/experimental | Stakeholder | Reference universe defined |

**Gate Status:** ✅ PASSED

---

## Phase 1: Performance Baseline

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | CPU budget: <15% single core | Auditor | From spec technical specifications |
| 2026-04-02 | Latency target: ~46ms (2048 samples @ 48kHz) | Auditor | Granular time-stretch requires grain window look-ahead |
| 2026-04-02 | Primary sample rates: 44.1kHz, 48kHz | Auditor | Spec-defined; 88.2/96kHz supported but not optimized |

**Gate Status:** ✅ PASSED

---

## Phase 2: Architecture

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | Module structure: DryDelay, TimeStretch, FreezeBuffer, Wavefolder, ThresholdDetector, WetProcessor, Mixer, LFO, OutputProtection | Architect | Derived from signal flow in spec |
| 2026-04-02 | Parameter ownership: TimeStretch owns stretch/position/freezeMode; Wavefolder owns foldAmount/foldOffset; LFO owns rate/waveform/depth/targets | Architect | Clear module-parameter mapping |
| 2026-04-02 | State management: APVTS with XML presets, version attribute for compatibility | Architect | JUCE standard, supports DAW automation |
| 2026-04-02 | Logarithmic mapping for stretch and lfoRate parameters | Architect | Equal perceptual travel above/below center |
| 2026-04-02 | LFO modulates on normalized scale (0–1) | Architect | Perceptually even modulation for log params |
| 2026-04-02 | Pre-allocate all buffers in prepareToPlay | Architect | Audio thread safety (CCC-01) |
| 2026-04-02 | Architecture plan approved after deep audit | Stakeholder | All FAIL/WARN issues addressed |

---

## Phase 3: Project Setup

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | Use FetchContent for JUCE 7.0.12 | Builder | Self-contained, matches CogVST pattern |
| 2026-04-02 | Use Catch2 for testing | Builder | Matches CogVST pattern, BDD-style tests |
| 2026-04-02 | Create 9 module stub files | Builder | TimeStretch, FreezeBuffer, Wavefolder, etc. |
| 2026-04-02 | Use raw JUCE headers instead of JuceHeader.h | Builder | Allows tests to compile without plugin target |
| 2026-04-02 | VST3 format only | Builder | Spec defines VST3 as primary target |

**Gate Status:** ✅ PASSED

---

## Phase 4: DSP Implementation

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | Per-sample smoothing for all continuous params (CM-05) | Builder | Prevent zipper noise from parameter changes |
| 2026-04-02 | Pre-allocate all buffers in prepareToPlay (CM-03) | Builder | Audio thread safety |
| 2026-04-02 | Measure amplitude at Wavefolder output (not after WetProcessor) | Builder | Spec line 77 |
| 2026-04-02 | Triangle fold algorithm from MathUtils | Builder | Matches spec triangle_fold formula |
| 2026-04-02 | LFO phase increment per-sample (not per-block) | Builder | Prevent modulation artifacts |
| 2026-04-02 | Double-buffer for freeze system | Builder | Prevents recording overwriting playback |
| 2026-04-02 | 10ms crossfade for freeze transitions | Builder | Spec "never harsh" requirement |
| 2026-04-02 | Deep audit: Fixed WetProcessor coefficient formula | Builder | Was `-1000/(ms*sr)`, now `exp(-1/(time*sr))` |
| 2026-04-02 | Deep audit: Fixed WetProcessor O(n²) RMS | Builder | Now uses running sum, O(n) |
| 2026-04-02 | Deep audit: Fixed LFO S&H cycle detection | Builder | Detect cycle start correctly |
| 2026-04-02 | Deep audit: Removed amplitude from FreezeBuffer | Builder | Amplitude measured at Wavefolder only |
| 2026-04-02 | Deep audit: Simplified LFO triangle formula | Builder | `1 - 4*|phase - 0.5|` |
| 2026-04-02 | Deep audit: Fixed LFO modulation routing | Builder | Per-sample modulation applied to parameters before module calls |
| 2026-04-02 | Known limitation: TimeStretch granular engine stub | Builder | Off mode passes through without time-stretching; freeze modes work correctly |

**Gate Status:** ✅ PASSED (TimeStretch stub documented, freeze functional)

---

## Phase 5: State Management

| Date | Decision | Made By | Rationale |
|------|----------|---------|-----------|
| 2026-04-02 | Phase 5 started | Builder | Proceeding from Phase 4 completion |
| 2026-04-02 | Factory presets in code (PresetManager) | Builder | Embed 5 presets (Init + 4 from spec) directly in plugin binary |
| 2026-04-02 | getNumPrograms() returns 5 | Builder | DAW preset menu shows all factory presets |
| 2026-04-02 | All ParamDefaults constexpr | Builder | Required for constexpr PresetParams initialization |
| 2026-04-02 | Deep audit: Fixed stretch/LFO rate normalized values | Builder | Logarithmic scale requires correct conversion formulas |
| 2026-04-02 | Version constant in Version.h | Builder | Centralized version string for state serialization |
| 2026-04-02 | currentProgramIndex in state | Builder | Preserve selected preset across sessions |
| 2026-04-02 | Round-trip tests added | Builder | Verify save/load preserves all values |
| 2026-04-02 | Added 10 creative presets | Builder | Themed around "infinite golden cogs weaving the omniverse" |
| 2026-04-02 | Chaos mode presets (low threshold) | Builder | Observer's Gaze, Quantum Collapse, Omniscient Dawn for cascading transformation |
| 2026-04-02 | Multi-target LFO presets | Builder | Golden Helix (stretch+position), Cog Within Cog (stretch+position+fold), Omniscient Dawn (all targets) |
| 2026-04-02 | Waveform-specific presets | Builder | Triangle for smooth (Golden Helix, Weaver's Dance), Square for rhythmic (Infinite Recursion, Eternal Return), S&H for random (Observer's Gaze, Quantum Collapse, Omniscient Dawn) |
| 2026-04-02 | Audited and improved presets for heavy psybient | Builder | All presets now have fold ≥ 0.55, threshold ≤ 0.25, depth ≥ 0.45, mix ≥ 0.89, multi-target LFO |
| 2026-04-02 | Chaos mode presets with very low threshold | Builder | Observer's Gaze (0.1), Quantum Collapse (0.12), Omniscient Dawn (0.05) for cascading freeze layers |

**Gate Status:** ✅ PASSED (11/11 tests pass)

---

*Last updated: Phase 4*