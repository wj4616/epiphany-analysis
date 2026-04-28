# Phase 0: Plugin Concept & Specification

> Define exactly what plugin to build, for whom, and under what constraints.

## Overview

| Attribute | Value |
|-----------|-------|
| **Phase** | 0 |
| **Name** | Plugin Concept & Specification |
| **Role Mindset** | Stakeholder |
| **Duration** | 1-2 sessions |
| **Criticality** | Foundation for all subsequent phases |

## Purpose

Lock purpose, scope, constraints, and success criteria so every subsequent phase has a fixed target. This phase is about **decisions**, not research or building.

## Pre-Check

Before starting this phase, verify:
- [ ] User has provided plugin concept or description
- [ ] Target user profile understood
- [ ] Budget constraints known

## Tasks

### Task 0.1: Create Project Folder Structure

**Owner**: Coordinator

**Description**: Create the project directory with tracking files.

**Output**: Project directory, decisions-ledger.md, artifact-manifest.md, metrics-tracker.md

**Steps**:
1. Create project folder
2. Initialize decisions-ledger.md
3. Initialize artifact-manifest.md
4. Initialize metrics-tracker.md

---

### Task 0.2: Define Plugin Purpose

**Owner**: Stakeholder

**Description**: Answer precisely:
- What plugin does this create?
- What problem does it solve?
- What goes wrong without it?
- Is this new, an upgrade, or a variant?

**Output**: README.md with purpose statement

**KB Lookups**:
- sound-design-kb/bridges (for sonic descriptions)

---

### Task 0.3: Define Sound Identity

**Owner**: Stakeholder

**Description**: Document the sonic character of the plugin using sonic descriptors.

**Output**: Sound identity document

**Sonic Descriptors** (use KB bridges):
- Warm / Bright / Lush / Punchy
- Psychedelic / Ambient / Evolving
- Any custom sonic terms

**KB Lookups**:
- sound-design-kb/bridges (translate descriptions to parameters)

---

### Task 0.4: Define Technical Constraints

**Owner**: Stakeholder

**Description**: Document:
- JUCE version
- C++ standard
- CMake version
- Target formats (VST3, AU, AAX)
- Target platforms (Linux, Windows, macOS)
- Target DAWs for testing

**Output**: constraints.md

---

### Task 0.5: Define Target User

**Owner**: Stakeholder

**Description**: Document:
- Skill level (beginner, intermediate, expert)
- Primary DAW
- Development environment
- Budget constraints

**Output**: User profile in README.md

---

### Task 0.6: Define Platform Targets

**Owner**: Stakeholder

**Description**: Document:
- Primary platform
- Secondary platforms
- Format priorities
- Testing requirements

**Output**: Platform targets in constraints.md

---

### Task 0.7: Phase Gate

**Owner**: Coordinator

**Description**: Verify all gate conditions before proceeding to Phase 1.

**Gate Conditions**:
- [ ] Purpose is one unambiguous paragraph
- [ ] Sound identity documented with sonic descriptors
- [ ] Technical constraints documented (JUCE, C++, formats)
- [ ] Target user profile defined
- [ ] Platform targets defined
- [ ] Decisions-ledger initialized with all Phase 0 decisions

**Blocker Examples**:
- "Purpose says 'improve the process' without specifying which process" — too vague, rewrite
- "Sound identity has no sonic descriptors" — use KB bridges to translate
- "No target user defined" — define at minimum skill level

---

## KB Lookups

| KB | Entry | Purpose |
|----|-------|---------|
| sound-design-kb/bridges | bridge-warm, bridge-bright, etc. | Translate sonic descriptions |
| market-kb | positioning | Market positioning guidance |

## Skills

- `juce-plugin-spec`: Skill for defining plugin specifications

## Output Artifacts

| Artifact | Purpose |
|----------|---------|
| README.md | Project overview, purpose statement |
| constraints.md | Technical and budget constraints |
| decisions-ledger.md | Decision tracking |
| artifact-manifest.md | Output tracking |
| metrics-tracker.md | Progress tracking |

## Common Failure Modes

| FM | Risk | Prevention |
|----|------|------------|
| FM-15 | Scope covers all variants | Define primary scope, mark variants as conditional |

## Next Phase

Phase 1: Performance Audit (if upgrading) or Phase 2: Architecture & Planning (if new plugin)