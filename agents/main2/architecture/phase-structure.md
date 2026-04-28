# Phase Structure: Creative VST Plugin Development Playbook

## Design Principles

1. **Coherent Blocks**: Each phase has a single clear objective
2. **Sequential Outputs**: A phase's output is the next phase's input
3. **Gate Enforcement**: Every phase ends with verifiable conditions
4. **Flow Pattern**: Understanding → Building → Verifying → Releasing

## Phase Overview

| Phase | Name | Objective | Duration (est.) |
|-------|------|-----------|-----------------|
| 0 | Commission & Scoping | Define what plugin to build, for whom, under what constraints | 1 session |
| 1 | Domain Research | Gather DSP, sound design, commercial domain knowledge | 1-2 sessions |
| 2 | Specification | Lock all creative decisions in a complete SPEC.md | 1 session |
| 3 | Architecture | Design class structure, signal flow, module interfaces | 1 session |
| 4 | Project Setup | CMake, test framework, CI, project skeleton | 1 session |
| 5 | DSP Implementation | Build audio engine module by module with two-stage review | 3-5 sessions |
| 6 | State & Presets | APVTS, preset system, state save/restore | 1-2 sessions |
| 7 | Processor Integration | Wire DSP modules in processBlock, parameter smoothing | 1 session |
| 8 | GUI Implementation | Editor, LookAndFeel, component hierarchy | 2-3 sessions |
| 9 | Build Verification | Cross-platform, sample rates, buffer sizes, stress tests | 1 session |
| 10 | DAW Testing | Human perceptual testing in REAPER | 1 session |
| 11 | Licensing Integration | Implement commercial licensing system | 1-2 sessions |
| 12 | Optimization | CPU profiling, memory audit, denormal handling | 1 session |
| 13 | Marketplace Preparation | Product pages, pricing, distribution setup | 1 session |
| 14 | Marketing Strategy | Pre-launch, launch, post-launch activities | 1 session |
| 15 | Release | Binary packaging, documentation, distribution | 1 session |
| 16 | Retrospective | Lessons learned, playbook updates, reusable components | 1 session |

## Phase Details

### Phase 0: Commission & Scoping

**Objective**: Define exactly what plugin to build, for whom, and under what constraints.

**Input**: User's commission brief (can be verbal or written)

**Output**: 
- README.md with purpose statement
- scope.md with in/out/adjacent boundaries
- constraints.md with technical and budget constraints
- success-criteria.md with measurable outcomes
- decisions-ledger.md initialized

**Gate**: Purpose unambiguous, scope explicit, constraints documented, success criteria measurable

---

### Phase 1: Domain Research

**Objective**: Gather comprehensive knowledge for informed implementation.

**Input**: Phase 0 outputs

**Output**:
- Domain analysis (VST development, failure modes, terminology)
- Sound design research (synthesis types, creative techniques)
- Commercial research (marketplaces, licensing, pricing)
- Cross-cutting concerns documented

**Gate**: Research covers all KB layers needed for implementation

---

### Phase 2: Specification

**Objective**: Lock all creative and technical decisions in a complete specification.

**Input**: Phase 0 + Phase 1 outputs, KB entries

**Output**:
- SPEC.md with plugin type, signal flow, parameters
- Parameter list with ranges, defaults, skew
- Sound identity in user's exact words
- Gain staging philosophy
- Preset theme and naming

**Gate**: All creative decisions locked, signal flow documented, parameters defined, human approval

---

### Phase 3: Architecture

**Objective**: Design class structure, signal flow, module interfaces.

**Input**: SPEC.md, KB patterns

**Output**:
- Architecture document with class diagram
- Signal flow diagram
- Module interfaces defined
- Task list with dependency order

**Gate**: Architecture documented, all modules have interfaces, dependency order correct

---

### Phase 4: Project Setup

**Objective**: Establish build system, test framework, CI, project skeleton.

**Input**: Architecture document

**Output**:
- CMakeLists.txt configured
- Test framework (Catch2) integrated
- CI pipeline (optional)
- Project skeleton created

**Gate**: Build succeeds, tests pass, plugin loads in REAPER

---

### Phase 5: DSP Implementation

**Objective**: Build audio engine module by module with two-stage review.

**Input**: Architecture, SPEC.md, KB entries

**Output**:
- DSP module implementations
- Unit tests for each module
- Two-stage review completed (spec compliance + code quality)

**Gate**: All modules pass two-stage review, automated tests pass

---

### Phase 6: State & Presets

**Objective**: Implement preset system and state persistence.

**Input**: DSP modules, SPEC.md

**Output**:
- PresetData struct
- PresetManager
- State save/restore (APVTS)

**Gate**: State round-trip test passes, presets work

---

### Phase 7: Processor Integration

**Objective**: Wire DSP modules in processBlock, parameter smoothing.

**Input**: DSP modules, presets, SPEC.md

**Output**:
- processBlock implementation
- All parameters wired
- SmoothedValue audit passed

**Gate**: All parameters work, SmoothedValue audit passed

---

### Phase 8: GUI Implementation

**Objective**: Build visual interface with user direction.

**Input**: SPEC.md, parameter layout

**Output**:
- PluginEditor implementation
- LookAndFeel class
- All controls visible and functional

**Gate**: All parameters have controls, visual design approved by user

---

### Phase 9: Build Verification

**Objective**: Verify plugin works across platforms and configurations.

**Input**: Built plugin

**Output**:
- Linux build verification
- Sample rate tests (44.1k, 48k, 96k, 192k)
- Buffer size tests (64-2048)
- Extreme parameter tests
- Multi-instance tests
- State save/restore in DAW

**Gate**: All stress tests pass

---

### Phase 10: DAW Testing

**Objective**: Human perceptual testing — listen and fix.

**Input**: Built plugin, REAPER

**Output**:
- Bug reports from listening
- Approved presets
- Sound sign-off

**Gate**: Every control produces audible change, all presets approved, user signs off on sound

---

### Phase 11: Licensing Integration

**Objective**: Implement commercial licensing system.

**Input**: Built plugin, licensing decision (iLok, custom, etc.)

**Output**:
- Licensing system integrated
- Trial/full differentiation
- Anti-piracy measures

**Gate**: Licensing works, trial expires correctly, full version activates

---

### Phase 12: Optimization

**Objective**: Make it fast and stable.

**Input**: Licensed plugin

**Output**:
- CPU profile report
- Memory audit
- Denormal stress test passed
- Sound verification after optimization

**Gate**: CPU profile acceptable, no audio thread allocations, denormals handled

---

### Phase 13: Marketplace Preparation

**Objective**: Prepare product for distribution.

**Input**: Optimized plugin

**Output**:
- Product page content
- Screenshots/audio demos
- Marketplace listings prepared
- Pricing documented

**Gate**: Product page complete, marketplace accounts ready

---

### Phase 14: Marketing Strategy

**Objective**: Define marketing activities for solo developer.

**Input**: Product page, pricing

**Output**:
- Pre-launch strategy
- Launch day activities
- Post-launch marketing plan

**Gate**: Marketing plan documented with specific actions

---

### Phase 15: Release

**Objective**: Package and distribute.

**Input**: All artifacts

**Output**:
- Release binary
- Documentation updated
- Git tagged
- Distributed to marketplaces

**Gate**: Binary available, documentation current, release announced

---

### Phase 16: Retrospective

**Objective**: Learn and improve.

**Input**: Completed project

**Output**:
- Retrospective notes
- Playbook updates
- Reusable components extracted

**Gate**: Lessons documented, playbook updated

## Cross-Cutting Concerns Mapped to Phases

| Concern | Phases Applied |
|---------|----------------|
| Audio Thread Safety (CCC-01) | 3, 5, 7, 9, 12 |
| Gain Staging (CCC-02) | 0, 3, 5, 7, 10 |
| State Persistence (CCC-03) | 6, 7, 9 |
| Parameter Consistency (CCC-04) | 2, 6, 7, 9, 16 |
| Cross-Platform (CCC-05) | 3, 4, 8, 9 |
| Human Creative Authority (CCC-06) | 0, 2, 5, 8, 10 |
| Commercial Viability (CCC-07) | 11, 13, 14, 15 |
| KB Sufficiency (CCC-08) | All |

## Early Assessment Phases

### Phase 1: Domain Research
- **Purpose**: Variable starting conditions affect implementation
- **Detection**: Sound design expertise level affects DSP complexity
- **Workflow Impact**: More complex synthesis requires more KB support

### Phase 9: DAW Testing
- **Purpose**: Perceptual issues can only be detected by human ear
- **Detection**: Silent bugs (dead parameters, mix issues)
- **Workflow Impact**: May require return to Phase 5 or 7 for fixes

## Phase Flow Diagram

```
[0] Commission
     ↓
[1] Research ────→ KB Population
     ↓
[2] Specification ←─── KB Bridges (Sound Design → Technical)
     ↓
[3] Architecture ←─── KB Technical Patterns
     ↓
[4] Project Setup
     ↓
[5] DSP Implementation ←─── KB DSP + Failure Modes
     ↓        (two-stage review)
[6] State & Presets
     ↓
[7] Processor Integration
     ↓
[8] GUI Implementation ←─── KB UI/UX
     ↓
[9] Build Verification
     ↓
[10] DAW Testing ←─── Human perceptual validation
     ↓        (may return to 5, 7, 8)
[11] Licensing Integration ←─── KB Commercial
     ↓
[12] Optimization
     ↓
[13] Marketplace Preparation ←─── KB Commercial
     ↓
[14] Marketing Strategy ←─── KB Commercial
     ↓
[15] Release
     ↓
[16] Retrospective
```