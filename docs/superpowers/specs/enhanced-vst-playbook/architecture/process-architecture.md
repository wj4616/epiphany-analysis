# Process Architecture

> Defines the phase structure, task granularity, dependencies, gates, and document tree for the enhanced VST playbook.

---

## 1. Phase Overview

### Phase Count Rationale

| Dimension | Level | Target Phases | Actual |
|-----------|-------|---------------|--------|
| Process | Standard | 8-13 | 13 |
| Knowledge | Bridged | N/A | N/A |
| Roles | Complex | N/A | N/A |
| Overall | Complex | 12-18 | **13** |

The enhanced VST playbook uses **13 phases**, matching the v7-unified playbook structure. This is within the acceptable range for complex projects (12-18 phases).

### Phase Summary

| Phase | Name | Purpose | Key Outputs |
|-------|------|---------|--------------|
| **0** | Plugin Concept & Specification | Define WHAT to build | SPEC.md |
| **1** | Performance Audit | Audit existing code (if upgrading) | Audit report |
| **2** | Architecture & Planning | Turn spec into buildable plan | Architecture diagrams, task list |
| **3** | Project Setup | Establish build infrastructure | CMake, tests, CI |
| **4** | DSP Implementation | Build audio engine | DSP modules |
| **5** | State & Presets | Implement state management | APVTS, presets |
| **6** | Integration | Wire modules together | processBlock, parameter layout |
| **7** | GUI Implementation | Build visual interface | Editor, LookAndFeel |
| **8** | Build Verification | Validate builds | Multi-platform builds |
| **9** | DAW Testing | Human perceptual testing | Test results |
| **10** | Market Preparation | Prepare for release (NEW) | Marketing materials |
| **11** | Optimization | Performance tuning | Profiling results |
| **12** | Release & Retrospective | Deploy and learn | Release, retrospective |

---

## 2. Detailed Phase Structure

### Phase 0: Plugin Concept & Specification

**Purpose**: Define exactly what plugin to build, for whom, and under what constraints.

**Role Mindset**: Stakeholder — this phase is about decisions, not research.

**Pre-Check**:
- [ ] User has provided plugin concept or description
- [ ] Target user profile understood
- [ ] Budget constraints known

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 0.1 | Coordinator | Create project folder structure | Project directory, tracking files |
| 0.2 | Stakeholder | Define plugin purpose | README.md |
| 0.3 | Stakeholder | Define sound identity | Sound identity document |
| 0.4 | Stakeholder | Define technical constraints | constraints.md |
| 0.5 | Stakeholder | Define target user | User profile |
| 0.6 | Stakeholder | Define platform targets | Platform targets |
| 0.7 | Coordinator | Phase gate: Spec complete | Gate checklist |

**Gate Conditions**:
- [ ] Purpose is one unambiguous paragraph
- [ ] Sound identity documented with sonic descriptors
- [ ] Technical constraints documented (JUCE, C++, formats)
- [ ] Target user profile defined
- [ ] Platform targets defined
- [ ] Decisions-ledger initialized

**KB Lookups**:
- sound-design-kb/bridges (for sonic descriptions)
- market-kb (for market positioning)

---

### Phase 1: Performance Audit (Conditional)

**Purpose**: Audit existing code if upgrading an existing plugin.

**Role Mindset**: Auditor — this phase is about finding issues, not fixing.

**Pre-Check**:
- [ ] Existing code exists
- [ ] This is an upgrade, not new plugin

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 1.1 | Auditor | Audio thread safety audit | Thread safety report |
| 1.2 | Auditor | Instance isolation audit | Isolation report |
| 1.3 | Auditor | Memory audit | Memory report |
| 1.4 | Auditor | Parameter audit | Parameter report |
| 1.5 | Coordinator | Phase gate: Audit complete | Gate checklist |

**Gate Conditions**:
- [ ] All FM-01 through FM-07 violations identified
- [ ] Static variable audit complete
- [ ] Memory allocation audit complete
- [ ] Parameter smoothing audit complete

**KB Lookups**:
- failure-modes-kb/audio-thread
- failure-modes-kb/parameters
- cpp-kb/realtime-safety

**Note**: This phase is skipped for new plugins.

---

### Phase 2: Architecture & Planning

**Purpose**: Turn specification into buildable plan.

**Role Mindset**: Architect — this phase is about design, not implementation.

**Pre-Check**:
- [ ] Phase 0 gate passed
- [ ] Sound identity defined

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 2.1 | Architect | Design class architecture | Class diagram |
| 2.2 | Architect | Design DSP algorithms | Algorithm specification |
| 2.3 | Architect | Design parameter layout | Parameter table |
| 2.4 | Architect | Design signal flow | Signal flow diagram |
| 2.5 | Architect | Create ordered task list | Task breakdown |
| 2.6 | Coordinator | Phase gate: Architecture complete | Gate checklist |

**Gate Conditions**:
- [ ] Class architecture documented
- [ ] DSP algorithms specified with math
- [ ] All parameters documented with ranges
- [ ] Signal flow diagram complete
- [ ] Task list ordered with dependencies

**KB Lookups**:
- dsp-kb (for algorithms)
- sound-design-kb/bridges (for parameter ranges)

---

### Phase 3: Project Setup & Test Framework

**Purpose**: Establish build infrastructure.

**Role Mindset**: Builder — this phase is about infrastructure, not features.

**Pre-Check**:
- [ ] Phase 2 gate passed
- [ ] Architecture documented

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 3.1 | Builder | Create CMakeLists.txt | CMake configuration |
| 3.2 | Builder | Set up test framework | Test framework |
| 3.3 | Builder | Configure CI/CD | CI pipeline |
| 3.4 | Builder | Create project structure | Source files |
| 3.5 | Coordinator | Phase gate: Setup complete | Gate checklist |

**Gate Conditions**:
- [ ] CMake builds successfully
- [ ] Test framework runs
- [ ] CI pipeline configured
- [ ] Project structure follows conventions

**KB Lookups**:
- platform-kb (for platform-specific setup)
- juce-kb (for JUCE project setup)

---

### Phase 4: DSP Implementation

**Purpose**: Build audio engine.

**Role Mindset**: Implementer — this phase is about code.

**Pre-Check**:
- [ ] Phase 3 gate passed
- [ ] Project builds

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 4.1 | Implementer | Implement DSP module 1 | Module code + tests |
| 4.2 | Implementer | Review module 1 (spec) | Spec review |
| 4.3 | Implementer | Review module 1 (quality) | Quality review |
| 4.N | ... | (Repeat for each module) | Module code + tests |
| 4.N+1 | Coordinator | Phase gate: DSP complete | Gate checklist |

**Gate Conditions**:
- [ ] All DSP modules implemented
- [ ] All modules pass two-stage review
- [ ] Audio thread safety verified
- [ ] SmoothedValue audit passed

**KB Lookups**:
- dsp-kb (for algorithms)
- cpp-kb/realtime-safety (for thread safety)
- sound-design-kb (for parameter ranges)

**Critical Notes**:
- Two-stage review: Stage 1 = spec compliance, Stage 2 = code quality
- SmoothedValue: Must call getNextValue() every sample
- Equal-power: Must use cosine/sine for dry/wet mixing

---

### Phase 5: State & Presets

**Purpose**: Implement state management and presets.

**Role Mindset**: Implementer — this phase is about persistence.

**Pre-Check**:
- [ ] Phase 4 gate passed
- [ ] DSP complete

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 5.1 | Implementer | Implement APVTS state | getStateInformation, setStateInformation |
| 5.2 | Implementer | Create PresetManager | PresetManager class |
| 5.3 | Designer | Design presets | Preset definitions |
| 5.4 | Designer | Audition presets | Approved presets |
| 5.5 | Coordinator | Phase gate: State complete | Gate checklist |

**Gate Conditions**:
- [ ] State save/restore works
- [ ] Presets save/load correctly
- [ ] State round-trip test passes
- [ ] Presets auditioned and approved

**KB Lookups**:
- sound-design-kb/preset-templates
- sound-design-kb/bridges (for preset values)

---

### Phase 6: Integration

**Purpose**: Wire DSP modules together.

**Role Mindset**: Integrator — this phase is about connections.

**Pre-Check**:
- [ ] Phase 5 gate passed
- [ ] State management works

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 6.1 | Integrator | Implement processBlock | processBlock method |
| 6.2 | Integrator | Wire parameter layout | Parameter attachments |
| 6.3 | Integrator | SmoothedValue audit | Audit report |
| 6.4 | Integrator | Mono/stereo handling | Channel handling |
| 6.5 | Coordinator | Phase gate: Integration complete | Gate checklist |

**Gate Conditions**:
- [ ] processBlock implements signal flow correctly
- [ ] All parameters smooth correctly
- [ ] Mono input handled
- [ ] No allocations in processBlock
- [ ] Output never exceeds 0dBFS

**KB Lookups**:
- failure-modes-kb/audio-thread
- failure-modes-kb/parameters
- cpp-kb/patterns/smoothed-value

---

### Phase 7: GUI Implementation

**Purpose**: Build visual interface.

**Role Mindset**: Designer — this phase is about aesthetics and usability.

**Pre-Check**:
- [ ] Phase 6 gate passed
- [ ] Processor works

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 7.1 | Designer | Design component hierarchy | Component tree |
| 7.2 | Implementer | Implement LookAndFeel | Custom LookAndFeel |
| 7.3 | Implementer | Implement components | Component classes |
| 7.4 | Implementer | Wire parameter attachments | UI attachments |
| 7.5 | Designer | Polish visual design | Visual polish pass |
| 7.6 | Coordinator | Phase gate: GUI complete | Gate checklist |

**Gate Conditions**:
- [ ] All parameters have UI controls
- [ ] LookAndFeel implemented
- [ ] Components resize correctly
- [ ] No blocking calls from parameter callbacks
- [ ] UI thread safety verified

**KB Lookups**:
- ui-kb/layouts
- ui-kb/components
- ui-kb/aesthetics
- market-kb (for visual identity)

---

### Phase 8: Build Verification

**Purpose**: Validate builds across platforms.

**Role Mindset**: Validator — this phase is about compatibility.

**Pre-Check**:
- [ ] Phase 7 gate passed
- [ ] GUI complete

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 8.1 | Validator | Linux build | Linux VST3 |
| 8.2 | Validator | Windows build | Windows VST3 |
| 8.3 | Validator | macOS build | macOS VST3/AU |
| 8.4 | Validator | Sample rate tests | Test results |
| 8.5 | Validator | Buffer size tests | Test results |
| 8.6 | Validator | Multi-instance test | Test results |
| 8.7 | Coordinator | Phase gate: Build verified | Gate checklist |

**Gate Conditions**:
- [ ] All target platforms build
- [ ] Sample rate tests pass (44.1k, 48k, 96k, 192k)
- [ ] Buffer size tests pass (64, 128, 256, 512, 1024, 2048)
- [ ] Multi-instance test passes
- [ ] State round-trip passes

**KB Lookups**:
- platform-kb (for platform-specific issues)
- testing-kb/automated

---

### Phase 9: DAW Testing

**Purpose**: Human perceptual testing.

**Role Mindset**: Tester — this phase is about LISTENING.

**Pre-Check**:
- [ ] Phase 8 gate passed
- [ ] Builds available

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 9.1 | Tester | Load in DAW | DAW load test |
| 9.2 | Tester | Control responsiveness | Control test |
| 9.3 | Tester | Mix range test | Mix test |
| 9.4 | Tester | Preset audition | Preset test |
| 9.5 | Tester | Automation test | Automation test |
| 9.6 | Tester | Bypass test | Bypass test |
| 9.7 | Tester | Silence test | Denormal test |
| 9.8 | Tester | Full-scale test | NaN/Inf test |
| 9.9 | Coordinator | Phase gate: DAW tested | Gate checklist |

**Gate Conditions**:
- [ ] Plugin loads in target DAW(s)
- [ ] All controls respond correctly
- [ ] Mix knob effective across full range
- [ ] All presets sound correct
- [ ] Automation works
- [ ] Bypass produces clean dry signal
- [ ] 60s silence: CPU stays low
- [ ] Full-scale noise: no NaN/Inf

**KB Lookups**:
- testing-kb/manual
- sound-design-kb (for preset validation)

**Critical Notes**:
- This phase is HUMAN-ONLY
- Two-stage code review CANNOT catch perceptual issues
- Listen carefully to each preset

---

### Phase 10: Market Preparation (NEW)

**Purpose**: Prepare for commercial release.

**Role Mindset**: Marketer — this phase is about positioning.

**Pre-Check**:
- [ ] Phase 9 gate passed
- [ ] DAW testing complete

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 10.1 | Marketer | Define market positioning | Positioning document |
| 10.2 | Marketer | Set pricing | Pricing strategy |
| 10.3 | Marketer | Create demo version | Demo build |
| 10.4 | Marketer | Prepare distribution | Distribution plan |
| 10.5 | Marketer | Create marketing materials | Screenshots, demo video |
| 10.6 | Coordinator | Phase gate: Market ready | Gate checklist |

**Gate Conditions**:
- [ ] Market position defined
- [ ] Pricing strategy documented
- [ ] Demo limitations defined
- [ ] Distribution channels identified
- [ ] Marketing materials prepared

**KB Lookups**:
- market-kb/positioning
- market-kb/pricing
- market-kb/distribution
- market-kb/marketing

---

### Phase 11: Optimization

**Purpose**: Performance tuning.

**Role Mindset**: Optimizer — this phase is about efficiency.

**Pre-Check**:
- [ ] Phase 10 gate passed
- [ ] Market ready

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 11.1 | Optimizer | CPU profiling | Profiling results |
| 11.2 | Optimizer | Memory audit | Memory report |
| 11.3 | Optimizer | Denormal stress test | Test results |
| 11.4 | Optimizer | SIMD optimization (optional) | Optimization code |
| 11.5 | Coordinator | Phase gate: Optimized | Gate checklist |

**Gate Conditions**:
- [ ] CPU usage within budget
- [ ] Memory usage acceptable
- [ ] Denormal test passes (60s silence)
- [ ] pluginval strictness 5+ passes

**KB Lookups**:
- cpp-kb/realtime-safety
- testing-kb/stress

---

### Phase 12: Release & Retrospective

**Purpose**: Deploy and learn.

**Role Mindset**: Release Manager — this phase is about delivery.

**Pre-Check**:
- [ ] Phase 11 gate passed
- [ ] Optimization complete

**Tasks**:

| Task | Owner | Description | Output |
|------|-------|-------------|--------|
| 12.1 | Manager | Package release builds | Release binaries |
| 12.2 | Manager | Write release notes | Release notes |
| 12.3 | Manager | Publish to distribution | Published release |
| 12.4 | Manager | Conduct retrospective | Retrospective document |
| 12.5 | Manager | Extract reusable components | Template extraction |

**Gate Conditions**:
- [ ] Release builds packaged
- [ ] Release notes written
- [ ] Distribution complete
- [ ] Retrospective conducted
- [ ] Lessons learned documented

---

## 3. Task Granularity

### Task Structure

Every task follows this structure:

| Field | Description |
|-------|-------------|
| **ID** | Phase.Task format (e.g., 4.2) |
| **Title** | Brief action description |
| **Owner** | Role responsible (Coordinator, Implementer, Designer, etc.) |
| **Description** | What the task accomplishes |
| **Output** | Deliverable artifact |
| **Dependencies** | Tasks that must complete first |
| **KB Lookups** | Knowledge base entries needed |

### Owner Roles

| Role | Responsibility |
|------|----------------|
| **Coordinator** | Gate management, tracking, decisions |
| **Stakeholder** | Creative decisions, approval |
| **Architect** | Design, planning |
| **Builder** | Infrastructure, setup |
| **Implementer** | Code implementation |
| **Designer** | Visual/sound design |
| **Integrator** | Module wiring |
| **Validator** | Testing, verification |
| **Tester** | Human perceptual testing |
| **Marketer** | Market positioning |
| **Optimizer** | Performance tuning |
| **Manager** | Release management |

### Task Dependency Types

| Dependency | Meaning |
|------------|---------|
| **Sequential** | Must complete before next task |
| **Parallel** | Can run simultaneously |
| **Conditional** | Only if condition met |
| **Gate** | Must pass before next phase |

---

## 4. Phase Dependencies

### Dependency Graph

```
Phase 0 (Spec) ─────────────────────────────────────────┐
                                                          │
Phase 1 (Audit) ─────────────────────────────────────────┤ (Conditional)
                                                          │
Phase 2 (Architecture) ──────────────────────────────────┤
                                                          │
Phase 3 (Setup) ─────────────────────────────────────────┤
                                                          │
Phase 4 (DSP) ────────────────────────────────────────────┤
                                                          │
Phase 5 (State) ──────────────────────────────────────────┤
                                                          │
Phase 6 (Integration) ─────────────────────────────────────┤
                                                          │
Phase 7 (GUI) ────────────────────────────────────────────┤
                                                          │
Phase 8 (Build) ──────────────────────────────────────────┤
                                                          │
Phase 9 (DAW Test) ───────────────────────────────────────┤
                                                          │
Phase 10 (Market) ────────────────────────────────────────┤ (NEW)
                                                          │
Phase 11 (Optimization) ──────────────────────────────────┤
                                                          │
Phase 12 (Release) ───────────────────────────────────────┘
```

### Parallel Opportunities

| Phases | Parallel Tasks |
|--------|----------------|
| Phase 4 | Multiple DSP modules can be implemented in parallel |
| Phase 5 | Presets can be designed while state management is implemented |
| Phase 7 | Multiple UI components can be implemented in parallel |
| Phase 8 | Platform builds can run in parallel |
| Phase 10 | Marketing materials can be created in parallel with demo preparation |

---

## 5. Quality Gates

### Gate Structure

Each gate has:
1. **Pre-check**: Conditions before phase starts
2. **Gate conditions**: Conditions to pass before next phase
3. **Blocker examples**: Common issues that block progress

### Cross-Cutting Gate Conditions

These apply to ALL phases:

| Condition | Enforcement |
|-----------|-------------|
| Audio thread safety | No allocations/locks in processBlock |
| Decormal handling | ScopedNoDenormals present |
| Equal-power mixing | Cosine/sine for dry/wet |
| SmoothedValue usage | getNextValue() per sample |

### Gate Validation Methods

| Method | When Used |
|--------|-----------|
| Code review | Phases 4, 5, 6, 7 |
| Automated test | Phases 3, 8, 11 |
| Human test | Phase 9 |
| pluginval | Phases 8, 11 |
| DAW testing | Phase 9 |

---

## 6. Document Tree

### Output Playbook Documents

```
enhanced-vst-playbook/
├── README.md                    # Playbook overview
├── scope.md                     # Scope boundaries
├── constraints.md               # Strategic constraints
├── success-criteria.md          # Measurable criteria
├── decisions-ledger.md          # Decision tracking
├── artifact-manifest.md         # Output tracking
├── metrics-tracker.md           # Progress metrics
│
├── architecture/
│   ├── kb-architecture.md       # KB architecture
│   ├── entry-schema.json        # Entry schema
│   ├── bridge-schema.json       # Bridge schema
│   ├── population-strategy.md   # KB population
│   └── directory-structure.md    # KB directory tree
│
├── research/
│   ├── domain-analysis.md       # Domain research
│   ├── best-practices.md        # Best practices
│   ├── competitive-templates.md # Competitive audit
│   ├── existing-playbook-audit.md # v7 audit
│   ├── cross-cutting-concerns.md # Cross-cutting
│   ├── platform-concerns.md     # Platform specifics
│   └── requirements.md          # Requirements synthesis
│
├── phases/
│   ├── phase-0-spec.md          # Phase 0 details
│   ├── phase-1-audit.md         # Phase 1 details
│   ├── phase-2-architecture.md  # Phase 2 details
│   ├── phase-3-setup.md         # Phase 3 details
│   ├── phase-4-dsp.md           # Phase 4 details
│   ├── phase-5-state.md         # Phase 5 details
│   ├── phase-6-integration.md   # Phase 6 details
│   ├── phase-7-gui.md           # Phase 7 details
│   ├── phase-8-build.md         # Phase 8 details
│   ├── phase-9-daw-test.md      # Phase 9 details
│   ├── phase-10-market.md       # Phase 10 details (NEW)
│   ├── phase-11-optimization.md # Phase 11 details
│   └── phase-12-release.md     # Phase 12 details
│
├── templates/
│   ├── spec-template.md         # SPEC.md template
│   ├── task-template.md         # Task template
│   ├── gate-template.md         # Gate checklist
│   └── review-template.md       # Review template
│
├── validation/
│   ├── pluginval-checklist.md   # pluginval tests
│   ├── daw-test-checklist.md    # DAW tests
│   └── release-checklist.md     # Release checklist
│
└── kb/                           # Knowledge Base
    ├── master-index.json
    ├── dsp-kb/
    ├── sound-design-kb/
    ├── ui-kb/
    ├── failure-modes-kb/
    ├── platform-kb/
    ├── market-kb/
    ├── testing-kb/
    ├── cpp-kb/
    └── juce-kb/
```

### Knowledge Base Documents

See `/kb/` directory structure in `architecture/directory-structure.md`.

---

## 7. Integration with Existing Playbook

### What We Preserve

| From v7-unified | Enhancement |
|-----------------|-------------|
| 13-phase workflow | Preserved, added Phase 10 |
| Failure modes database | Preserved, enhanced |
| Two-stage review process | Preserved |
| Decisions-ledger pattern | Preserved |
| Artifact-manifest pattern | Preserved |
| Metrics-tracker pattern | Preserved |

### What We Enhance

| Area | Enhancement |
|------|-------------|
| Sound Design KB | Expand psychedelic/ambient techniques |
| UI KB | Add modern aesthetics guidance |
| Market KB | Create new (Phase 10) |
| Academic citations | Add to all DSP entries |
| Bridge entries | Create sonic translations |
| Cross-platform | Add explicit Windows/macOS testing |

### What We Add

| Addition | Purpose |
|----------|---------|
| Phase 10: Market Preparation | Commercial release guidance |
| Bridge schema | Sonic-to-parameter translations |
| Confidence scoring | Entry quality tracking |
| Source hierarchy | Academic/professional sources |

---

## 8. Next Steps

Phase 5 will implement this architecture:
- Create the output playbook JSON
- Populate phase definitions
- Create skill definitions for each phase
- Link KB lookups to phases
- Create templates and checklists