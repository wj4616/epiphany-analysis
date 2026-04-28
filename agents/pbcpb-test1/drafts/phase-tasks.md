# Phase Task Engineering

This document defines all tasks, gate conditions, and prevention rules for each of the 17 phases.

---

## Phase 0: Plugin Concept & Specification

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T0-01 | [Coordinator] Create project structure | Coordinator | Create project folder with README, research/, architecture/, drafts/, audits/, testing/, final/ | Project structure |
| T0-02 | [Human] Define plugin type | Human | Specify: synth, effect, module, or tool. Define signal flow. Human draws/describes, AI documents. | Plugin type decision |
| T0-03 | [Human] Define sound identity | Human | Describe sound character in exact words. AI records verbatim, never paraphrases. | Sound identity doc |
| T0-04 | [Human] Define technical constraints | Human | JUCE version, formats (VST3/AU/AAX), target OS, DAW, sample rates, buffer sizes, CPU budget. | constraints.md |
| T0-05 | [Human] Define gain staging philosophy | Human | When hot signals OK, conservative staging rules, headroom targets. | Gain staging doc |
| T0-06 | [Human] Define preset theme | Human | Creative direction for presets. Human approves every name. | Preset theme doc |
| T0-07 | [AI] Write one-page spec | AI | Structured: architecture, DSP, GUI, parameters, presets, build instructions, what-not-to-do. | SPEC.md |
| T0-08 | [AI] Spec self-review | AI | Check: placeholder values, contradictions, ambiguity, scope gaps, formula correctness at boundaries. | Review notes |
| T0-09 | [Coordinator] Phase 0 gate | Coordinator | Verify all gate conditions met before proceeding. | Gate pass |

### Gate Conditions

- [ ] Plugin type defined (synth/effect/module/tool)
- [ ] Signal flow documented (diagram or description)
- [ ] Sound identity in human's exact words
- [ ] Technical constraints locked
- [ ] Gain staging philosophy explicit
- [ ] One-page spec written and self-reviewed
- [ ] decisions-ledger.md updated
- [ ] artifact-manifest.md updated

### Prevention Rules

| ID | Lesson | Prevention |
|----|--------|------------|
| PR-001 | AI proposed generic preset names | ALWAYS use human's exact creative language |
| PR-002 | Excessive conservative gain staging | Audit existing gain staging early |
| PR-003 | Formula errors at boundaries | Test formulas at param=0, param=1, param=max |

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-015 | Purpose too vague | Enforce one-paragraph purpose with specifics |
| FM-016 | Scope creep later | Lock scope with explicit in/out/adjacent |

---

## Phase 1: Performance Audit (if upgrading)

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T1-01 | [AI] Benchmark existing code | AI | Profile existing implementation if upgrading. | Performance baseline |
| T1-02 | [AI] Identify hotspots | AI | Find CPU-critical paths. | Hotspot report |
| T1-03 | [AI] Document baseline | AI | Record metrics for comparison. | baseline.md |
| T1-04 | [Coordinator] Phase 1 gate | Coordinator | Verify baseline or mark N/A for new projects. | Gate pass |

### Gate Conditions

- [ ] Performance baseline documented OR
- [ ] N/A for new project (skip phase)

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-017 | No baseline for upgrades | Require baseline before optimization |

---

## Phase 2: Architecture & Task Breakdown

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T2-01 | [AI] Design module structure | AI | Define DSP modules, their interfaces, and dependencies. | Module diagram |
| T2-02 | [AI] Design DSP chain | AI | Define signal routing, order of processing, feedback paths. | DSP chain doc |
| T2-03 | [AI] Design KB architecture | AI | Define 5-layer KB structure, bridge layer schema. | KB architecture |
| T2-04 | [AI] Create implementation plan | AI | Task breakdown with dependencies, estimates, owners. | Implementation plan |
| T2-05 | [Human] Approve architecture | Human | Review and approve module structure and plan. | Approval |
| T2-06 | [Coordinator] Phase 2 gate | Coordinator | Verify architecture complete and approved. | Gate pass |

### Gate Conditions

- [ ] Module structure defined
- [ ] DSP chain documented
- [ ] KB architecture defined
- [ ] Implementation plan written
- [ ] Human approval recorded
- [ ] decisions-ledger.md updated

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-018 | Architecture too complex | Keep modules minimal, justify each boundary |
| FM-019 | Missing dependencies | Explicit dependency mapping required |

---

## Phase 3: Project Setup & Test Framework

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T3-01 | [AI] Create CMakeLists.txt | AI | FetchContent for JUCE, C++17, target definitions. | CMakeLists.txt |
| T3-02 | [AI] Set up Catch2 | AI | Add Catch2 via FetchContent, create test runner. | Test framework |
| T3-03 | [AI] Create directory structure | AI | Source directories matching architecture. | Project structure |
| T3-04 | [AI] Create PluginProcessor stub | AI | Minimal AudioProcessor implementation. | Processor stub |
| T3-05 | [AI] Create PluginEditor stub | AI | Minimal AudioProcessorEditor. | Editor stub |
| T3-06 | [AI] Seed KB directories | AI | Create kb/ structure with placeholders. | KB structure |
| T3-07 | [AI] Verify build | AI | Compile project, verify plugin loads. | Build verification |
| T3-08 | [Coordinator] Phase 3 gate | Coordinator | Build works, tests run, plugin loads. | Gate pass |

### Gate Conditions

- [ ] CMake project builds successfully
- [ ] Test framework runs (empty tests pass)
- [ ] Plugin stub loads in REAPER
- [ ] KB directories created with placeholders
- [ ] artifact-manifest.md updated

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-020 | CMake configuration errors | Use proven templates from research |
| FM-021 | Plugin won't load | Check VST3 path, rebuild clean |

---

## Phase 4: Core DSP Implementation

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T4-01 | [AI] Implement oscillators | AI | Oscillator modules per spec (if synth). | Oscillator classes |
| T4-02 | [AI] Implement filters | AI | Filter modules (LP, HP, BP, notch). | Filter classes |
| T4-03 | [AI] Implement effects | AI | Effect modules per spec. | Effect classes |
| T4-04 | [AI] Implement modulation | AI | LFOs, envelopes, modulation routing. | Modulation classes |
| T4-05 | [AI] Audio thread audit | AI | Run audio thread safety audit on all DSP. | Audit report |
| T4-06 | [AI] Add SmoothedValue | AI | Add smoothing to all parameters. | Smoothed parameters |
| T4-07 | [AI] Unit tests | AI | Catch2 tests for each module. | Unit tests |
| T4-08 | [Coordinator] Phase 4 gate | Coordinator | All DSP implemented, audits pass, tests pass. | Gate pass |

### Gate Conditions

- [ ] All DSP modules implemented per spec
- [ ] Audio thread safety audit passed (FM-01 to FM-07)
- [ ] SmoothedValue audit passed
- [ ] Unit tests pass
- [ ] No allocations in processBlock (verified)

### Prevention Rules

| ID | Lesson | Prevention |
|----|--------|------------|
| PR-004 | Allocation in audio thread | Pre-allocate all buffers in prepareToPlay |
| PR-005 | Lock in audio thread | Use lock-free queues for GUI communication |
| PR-006 | Once-per-block smoothing | Call getNextValue() per sample |

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-01 | Memory allocation | Audit with juce-audio-thread-audit |
| FM-02 | Lock acquisition | Use std::atomic, lock-free queues |
| FM-03 | Blocking I/O | Async message passing |
| FM-04 | Unbounded iteration | Worst-case bounds |
| FM-05 | Virtual dispatch overhead | Use final methods |
| FM-06 | Dynamic casting | Static type design |
| FM-07 | Exception throwing | No exceptions in audio code |
| EM-13 | Dead parameters | Audit parameter→DSP connections |
| CM-05 | Once-per-block smoothing | Per-sample smoothing required |

---

## Phase 5: Preset Data & State Management

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T5-01 | [AI] Configure APVTS | AI | AudioProcessorValueTreeState with parameter layout. | APVTS setup |
| T5-02 | [AI] Define parameters | AI | All parameters with ranges, defaults, names. | Parameter layout |
| T5-03 | [AI] Implement state save | AI | getStateInformation for preset saving. | State save |
| T5-04 | [AI] Implement state load | AI | setStateInformation for preset loading. | State load |
| T5-05 | [AI] Create factory presets | AI | Initial preset bank based on sound identity. | Factory presets |
| T5-06 | [AI] Implement preset browser | AI | Optional: preset selection UI. | Preset browser |
| T5-07 | [Coordinator] Phase 5 gate | Coordinator | Preset system works, factory presets validated. | Gate pass |

### Gate Conditions

- [ ] All parameters connected to APVTS
- [ ] Preset save works (XML/chunk format)
- [ ] Preset load works
- [ ] Factory presets created and validated
- [ ] State survives DAW project save/load

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-022 | Parameter not in APVTS | All audio parameters must use APVTS |
| FM-023 | Preset format incompatible | Use standard XML or chunk format |

---

## Phase 6: Processor Integration

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T6-01 | [AI] Connect DSP modules | AI | Wire DSP modules in Processor. | Integration |
| T6-02 | [AI] Connect parameters | AI | Link APVTS parameters to DSP via SmoothedValue. | Parameter routing |
| T6-03 | [AI] Implement processBlock | AI | Complete audio processing callback. | processBlock |
| T6-04 | [AI] Add gain staging | AI | Implement gain stages between modules. | Gain staging |
| T6-05 | [AI] Integration tests | AI | End-to-end audio processing tests. | Integration tests |
| T6-06 | [Coordinator] Phase 6 gate | Coordinator | Integration tests pass, signal flow matches spec. | Gate pass |

### Gate Conditions

- [ ] Signal flow matches spec diagram
- [ ] All parameters functional (each connects to DSP)
- [ ] Integration tests pass
- [ ] Gain staging implemented per philosophy

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-024 | Disconnected parameter | Verify each parameter affects audio |
| FM-025 | Gain too hot/cold | Measure levels at each stage |

---

## Phase 7: GUI / Editor Implementation

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T7-01 | [Human] Design layout | Human | Describe desired layout, AI implements. | Layout sketch |
| T7-02 | [AI] Create LookAndFeel | AI | Custom LookAndFeel class for styling. | LookAndFeel |
| T7-03 | [AI] Implement controls | AI | Knobs, sliders, buttons per design. | Control components |
| T7-04 | [AI] Add attachments | AI | Connect controls to APVTS. | Parameter attachments |
| T7-05 | [AI] Add visualizations | AI | Meters, scopes (if in spec). | Visualizations |
| T7-06 | [AI] Test scaling | AI | Verify at different sizes. | Scaling test |
| T7-07 | [Human] Approve GUI | Human | Final visual approval. | Approval |
| T7-08 | [Coordinator] Phase 7 gate | Coordinator | GUI complete, human approved. | Gate pass |

### Gate Conditions

- [ ] Layout approved by human
- [ ] All controls functional and connected
- [ ] Responsive scaling works (if applicable)
- [ ] Visual design matches human's direction

### Prevention Rules

| ID | Lesson | Prevention |
|----|--------|------------|
| PR-007 | AI proposed colors human didn't request | Human approves all visual decisions |
| PR-008 | Inconsistent control styling | Use single LookAndFeel throughout |

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-026 | UI thread blocks audio | Use AsyncUpdater for GUI updates |
| FM-027 | Scaling breaks layout | Test at multiple sizes |

---

## Phase 8: Build Verification & Cross-Platform

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T8-01 | [AI] Create Release build | AI | CMake Release configuration. | Release build |
| T8-02 | [AI] Test in REAPER | AI | Verify plugin loads and functions. | REAPER test |
| T8-03 | [AI] Test sample rates | AI | Verify at 44.1k, 48k, 96k. | Sample rate test |
| T8-04 | [AI] Test buffer sizes | AI | Verify at 64, 128, 256, 512, 1024. | Buffer size test |
| T8-05 | [AI] Create installer | AI | Optional: package for distribution. | Installer |
| T8-06 | [Coordinator] Phase 8 gate | Coordinator | Build verified, all tests pass. | Gate pass |

### Gate Conditions

- [ ] Release build succeeds
- [ ] Plugin loads in REAPER
- [ ] Works at all target sample rates
- [ ] Works at all target buffer sizes

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-028 | Release build fails | Check compiler warnings, link errors |
| FM-029 | Sample rate issues | Use prepareToPlay sample rate |

---

## Phase 9: DAW Testing & Sound Approval

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T9-01 | [Human] Load in REAPER | Human | Load plugin in DAW. | Plugin loaded |
| T9-02 | [Human] Test presets | Human | Listen to all presets. | Preset review |
| T9-03 | [Human] Test parameters | Human | Test all parameter ranges. | Parameter review |
| T9-04 | [Human] Listen for artifacts | Human | Check for clicks, pops, noise. | Artifact review |
| T9-05 | [Human] Approve sound | Human | Does it match the sound identity? | Sound approval |
| T9-06 | [AI] Log issues | AI | Record any issues for fixing. | Issue log |
| T9-07 | [Coordinator] Phase 9 gate | Coordinator | Human approved, issues logged. | Gate pass |

### Gate Conditions

- [ ] Human approves sound identity match
- [ ] No clicks/pops at any parameter setting
- [ ] Presets sound correct
- [ ] Issue log updated (or empty)

### Prevention Rules

| ID | Lesson | Prevention |
|----|--------|------------|
| PR-009 | AI tested instead of human | This is HUMAN-ONLY phase |
| PR-010 | Skipped listening tests | Human must listen, not just verify function |

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-030 | Sound doesn't match identity | Return to Phase 0 or 4 for adjustment |
| FM-031 | Clicks/pops at extreme settings | Check smoothing, gain staging |

---

## Phase 10: Quality Audits

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T10-01 | [AI] Audio thread audit | AI | Run juce-audio-thread-audit skill. | Audit report |
| T10-02 | [AI] SmoothedValue audit | AI | Run juce-smoothedvalue-audit skill. | Audit report |
| T10-03 | [AI] Memory leak check | AI | Run Valgrind or sanitizers. | Memory report |
| T10-04 | [AI] CPU profile | AI | Profile with realistic load. | CPU report |
| T10-05 | [AI] Document findings | AI | Compile all audit results. | Audit summary |
| T10-06 | [Coordinator] Phase 10 gate | Coordinator | All audits pass. | Gate pass |

### Gate Conditions

- [ ] Audio thread safety: PASS
- [ ] SmoothedValue: PASS
- [ ] No memory leaks
- [ ] CPU usage within budget (<5% typical)

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-32 | Audit failures | Fix before proceeding |
| FM-33 | CPU too high | Profile and optimize |

---

## Phase 11: Profiling & Optimization

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T11-01 | [AI] Profile DSP | AI | Detailed profiling of processBlock. | Profile data |
| T11-02 | [AI] Identify bottlenecks | AI | Find CPU hotspots. | Bottleneck report |
| T11-03 | [AI] Optimize critical paths | AI | Optimize without breaking correctness. | Optimized code |
| T11-04 | [AI] Re-verify | AI | Run tests again after optimization. | Verification |
| T11-05 | [Coordinator] Phase 11 gate | Coordinator | Performance within budget. | Gate pass |

### Gate Conditions

- [ ] CPU usage within budget
- [ ] No performance regressions
- [ ] All tests still pass

---

## Phase 12: Release Preparation

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T12-01 | [AI] Version bump | AI | Update version number. | Version update |
| T12-02 | [AI] Update changelog | AI | Document changes since last release. | Changelog |
| T12-03 | [AI] Create release notes | AI | User-facing release notes. | Release notes |
| T12-04 | [AI] Final build | AI | Clean release build. | Final build |
| T12-05 | [AI] Package for distribution | AI | Create distributable package. | Distribution package |
| T12-06 | [Coordinator] Phase 12 gate | Coordinator | Release package ready. | Gate pass |

### Gate Conditions

- [ ] Version number updated
- [ ] Changelog updated
- [ ] Release notes written
- [ ] Final build tested
- [ ] Package ready for distribution

---

## Phase 13: Licensing Integration

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T13-01 | [AI] Research options | AI | Review licensing systems from Commercial KB. | Options summary |
| T13-02 | [Human] Select approach | Human | Choose: iLok, MuseHub, custom, or none. | Selection |
| T13-03 | [AI] Implement validation | AI | Integrate license checking. | License validation |
| T13-04 | [AI] Create license generation | AI | Workflow for generating licenses. | License generation |
| T13-05 | [AI] Test enforcement | AI | Verify license checking works. | Test results |
| T13-06 | [AI] Create demo version | AI | Optional: limited demo build. | Demo version |
| T13-07 | [Coordinator] Phase 13 gate | Coordinator | Licensing functional. | Gate pass |

### Gate Conditions

- [ ] Licensing system selected by human
- [ ] License validation implemented
- [ ] License generation workflow works
- [ ] Demo restrictions work (if applicable)

### Failure Modes

| FM | Risk | Mitigation |
|----|------|------------|
| FM-034 | License bypass | Accept basic protection is sufficient |
| FM-035 | Demo doesn't expire | Test demo limitations |

---

## Phase 14: Marketplace Preparation

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T14-01 | [AI] Write product description | AI | Marketing copy for listing. | Product description |
| T14-02 | [AI] Create screenshots | AI | Professional screenshots. | Screenshots |
| T14-03 | [AI] Create demo audio | AI | Audio demos of presets. | Demo audio |
| T14-04 | [AI] Create demo video | AI | Optional: walkthrough video. | Demo video |
| T14-05 | [AI] Prepare KVR listing | AI | KVR database entry + marketplace. | KVR listing |
| T14-06 | [AI] Prepare Plugin Boutique | AI | Submission for Plugin Boutique. | PB submission |
| T14-07 | [Human] Set pricing | Human | Price and launch discount. | Pricing decision |
| T14-08 | [Coordinator] Phase 14 gate | Coordinator | Listings ready. | Gate pass |

### Gate Conditions

- [ ] Product description written
- [ ] Screenshots created
- [ ] Demo audio available
- [ ] KVR listing prepared
- [ ] Pricing set by human

---

## Phase 15: Marketing Strategy

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T15-01 | [AI] Pre-launch plan | AI | Activities before launch. | Pre-launch plan |
| T15-02 | [AI] Launch week plan | AI | Activities during launch. | Launch plan |
| T15-03 | [AI] Post-launch plan | AI | Ongoing marketing activities. | Post-launch plan |
| T15-04 | [AI] Identify reviewers | AI | YouTube creators, bloggers. | Reviewer list |
| T15-05 | [AI] Social schedule | AI | Content calendar. | Social calendar |
| T15-06 | [AI] Content templates | AI | Templates for announcements. | Content templates |
| T15-07 | [Human] Approve plan | Human | Review and approve marketing. | Approval |
| T15-08 | [Coordinator] Phase 15 gate | Coordinator | Marketing plan approved. | Gate pass |

### Gate Conditions

- [ ] Marketing plan documented
- [ ] Content templates ready
- [ ] Reviewer list prepared
- [ ] Human approval recorded

---

## Phase 16: Launch Execution

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T16-01 | [Human] Submit listings | Human | Submit to marketplaces. | Live listings |
| T16-02 | [Human] Send announcements | Human | Launch announcement. | Announcement |
| T16-03 | [Human] Send review copies | Human | Distribute to reviewers. | Copies sent |
| T16-04 | [Human] Post on forums | Human | KVR, Reddit, etc. | Forum posts |
| T16-05 | [AI] Monitor feedback | AI | Track initial reactions. | Feedback log |
| T16-06 | [AI] Address issues | AI | Fix urgent problems. | Issue fixes |
| T16-07 | [Coordinator] Phase 16 gate | Coordinator | Launch complete. | Gate pass |

### Gate Conditions

- [ ] Product live on marketplace(s)
- [ ] Launch announcement sent
- [ ] Review copies distributed
- [ ] Initial feedback addressed

---

## Phase 17: Updates & Analytics

### Tasks

| ID | Title | Owner | Description | Output |
|----|-------|-------|-------------|--------|
| T17-01 | [AI] Track analytics | AI | Monitor sales, downloads. | Analytics |
| T17-02 | [AI] Collect feedback | AI | User reviews, support requests. | Feedback log |
| T17-03 | [AI] Plan updates | AI | Prioritize fixes and features. | Update plan |
| T17-04 | [AI] Implement updates | AI | Code changes. | Updates |
| T17-05 | [AI] Release updates | AI | Version releases. | Released updates |

### Gate Conditions

- [ ] Analytics tracked
- [ ] Update process defined

---

## Cross-Cutting Task Summary

| Phase | Total Tasks | Human Tasks | AI Tasks | Coordinator Tasks |
|-------|-------------|-------------|----------|-------------------|
| 0 | 9 | 5 | 3 | 1 |
| 1 | 4 | 0 | 3 | 1 |
| 2 | 6 | 1 | 4 | 1 |
| 3 | 8 | 0 | 7 | 1 |
| 4 | 8 | 0 | 7 | 1 |
| 5 | 7 | 0 | 6 | 1 |
| 6 | 6 | 0 | 5 | 1 |
| 7 | 8 | 2 | 5 | 1 |
| 8 | 6 | 0 | 5 | 1 |
| 9 | 7 | 5 | 1 | 1 |
| 10 | 6 | 0 | 5 | 1 |
| 11 | 5 | 0 | 4 | 1 |
| 12 | 6 | 0 | 5 | 1 |
| 13 | 7 | 1 | 5 | 1 |
| 14 | 8 | 1 | 6 | 1 |
| 15 | 8 | 1 | 6 | 1 |
| 16 | 7 | 4 | 2 | 1 |
| 17 | 5 | 0 | 5 | 0 |
| **Total** | **117** | **20** | **88** | **17** |