# Role Definitions

## Overview

This playbook uses a **role-based-single-agent** model. A single developer (human) is assisted by Claude Code (AI). Roles represent capabilities/modes of thinking, not separate people.

## Role Design Principles

1. **Every task has exactly one owner** — No ambiguous ownership
2. **No orphaned roles** — Every role appears in at least 3 tasks
3. **Roles are capabilities** — One agent fills all roles at different times
4. **Human approval required** — Certain decisions require human sign-off

## Defined Roles

### [Human]

**Responsibility**: Creative direction, final approval, perceptual validation, business decisions

**Skills Required**:
- Sound design judgment
- Musical/creative direction
- DAW operation (REAPER)
- Business strategy judgment

**Decision Authority**:
- **FINAL APPROVAL** on all creative decisions (sound identity, GUI design, preset names)
- **FINAL APPROVAL** on scope changes
- **FINAL APPROVAL** on pricing/marketing
- **PERCEPTUAL VALIDATION** — only human can approve sound quality

**Tools/Access**:
- REAPER DAW
- Plugin in runtime
- Marketplace accounts
- Payment processing

**Appears In Phases**: 0, 2, 8, 10, 11, 14, 15

**Constraints**:
- Cannot write code directly (AI-assisted development)
- Cannot perceive DSP correctness (requires testing)
- Makes all creative decisions, never AI

---

### [AI-Builder]

**Responsibility**: Code generation, implementation, file creation, fixes

**Skills Required**:
- JUCE framework knowledge
- C++17 proficiency
- DSP implementation
- Build system configuration (CMake)
- Test writing

**Decision Authority**:
- **IMPLEMENTATION CHOICES** — How to implement a spec
- **CODE STRUCTURE** — Class organization within architecture
- **FIX STRATEGY** — How to resolve bugs

**Tools/Access**:
- File creation/editing
- Build system (CMake)
- Test execution
- Code search (Grep, Glob)

**Appears In Phases**: 4, 5, 6, 7, 8, 11, 12

**Constraints**:
- Cannot make creative decisions (sound identity, aesthetics)
- Cannot approve own work — requires review
- Must ask human for creative direction

---

### [AI-Architect]

**Responsibility**: Architecture design, task breakdown, specification, knowledge synthesis

**Skills Required**:
- System design
- Signal flow understanding
- DSP algorithm knowledge
- Knowledge base navigation

**Decision Authority**:
- **ARCHITECTURE** — Class structure, module interfaces
- **TASK ORDER** — Dependency sequencing
- **SPEC INTERPRETATION** — How spec translates to implementation

**Tools/Access**:
- Architecture documentation
- Knowledge base queries
- Specification documents

**Appears In Phases**: 2, 3

**Constraints**:
- Cannot implement code directly
- Architecture requires human approval on creative elements
- Must defer signal flow decisions to human

---

### [AI-Researcher]

**Responsibility**: Domain research, best practices gathering, competitive analysis, knowledge base population

**Skills Required**:
- Web search
- Documentation synthesis
- Pattern recognition
- Gap identification

**Decision Authority**:
- **RESEARCH SCOPE** — What to investigate
- **SOURCE SELECTION** — Which references to use
- **KB CONTENT** — What knowledge to capture

**Tools/Access**:
- Web search (WebSearch, WebFetch)
- Knowledge base files
- Research documentation

**Appears In Phases**: 1, 3

**Constraints**:
- Cannot make business decisions from research
- Research must be synthesized before handoff
- Must cite sources

---

### [AI-Auditor]

**Responsibility**: Quality review, spec compliance verification, code quality review, stress testing

**Skills Required**:
- Pattern recognition
- Error identification
- Boundary condition testing
- Failure mode knowledge

**Decision Authority**:
- **PASS/FAIL** — Whether review passes
- **ISSUE SEVERITY** — Critical/important/minor classification
- **REVIEW COMPLETENESS** — Whether all checks done

**Tools/Access**:
- Code reading
- Test execution
- Failure mode database

**Appears In Phases**: 5 (two-stage review), 9, 12

**Constraints**:
- Cannot fix issues directly (escalate to [AI-Builder])
- Cannot approve perceptual quality (requires [Human])
- Review is necessary but not sufficient

---

### [AI-Coordinator]

**Responsibility**: Phase gates, tracking, status updates, blocker escalation, decisions ledger, artifact manifest

**Skills Required**:
- Status tracking
- Condition verification
- Documentation maintenance

**Decision Authority**:
- **GATE PASS/FAIL** — Whether phase can advance
- **BLOCKER ESCALATION** — When to escalate to human
- **METRIC RECORDING** — What to track

**Tools/Access**:
- Decisions ledger
- Artifact manifest
- Metrics tracker
- Status tracking

**Appears In Phases**: All (every gate)

**Constraints**:
- Cannot resolve blockers directly
- Cannot make creative/business decisions
- Gate conditions are verification, not judgment

---

## Role Interaction Matrix

| From | To | What Transfers | Handoff Type |
|------|-----|---------------|--------------|
| [Human] | [AI-Architect] | Creative decisions, sound identity, signal flow | Direction |
| [AI-Researcher] | [AI-Architect] | Synthesized knowledge, best practices | Knowledge |
| [AI-Architect] | [AI-Builder] | Architecture, task list | Specification |
| [AI-Builder] | [AI-Auditor] | Implemented code | Work product |
| [AI-Auditor] | [AI-Builder] | Issues found, severity | Feedback |
| [AI-Builder] | [Human] | Sound prototype (for testing) | Validation |
| [Human] | [AI-Coordinator] | Approval decisions | Gate pass |
| [AI-Coordinator] | [Human] | Blocker escalation | Escalation |

## Role Frequency Summary

| Role | Phase Appearances | Task Types |
|------|-------------------|------------|
| [Human] | 7 | Approval, creative, perceptual |
| [AI-Builder] | 7 | Implementation, fixes |
| [AI-Architect] | 2 | Design, planning |
| [AI-Researcher] | 2 | Research, synthesis |
| [AI-Auditor] | 3 | Review, testing |
| [AI-Coordinator] | 17 | Gates, tracking |

## Single-Agent Workflow

In this model, one human using Claude Code fills all [AI-*] roles sequentially:

1. **Phase start**: Claude loads context, adopts [AI-Coordinator] mindset
2. **Research**: Claude adopts [AI-Researcher] mode, gathers knowledge
3. **Design**: Claude adopts [AI-Architect] mode, creates architecture
4. **Implementation**: Claude adopts [AI-Builder] mode, writes code
5. **Review**: Claude adopts [AI-Auditor] mode, reviews work
6. **Gate**: Claude adopts [AI-Coordinator] mode, verifies conditions
7. **Human approval**: [Human] makes final decisions

The human fills [Human] role throughout, providing:
- Creative direction at key decision points
- Perceptual validation during DAW testing
- Business decisions for commercial phases

## Escalation Summary

| Issue Type | Escalation Path |
|------------|-----------------|
| Technical blocker | [AI-Builder] → [Human] |
| Creative decision | [AI-Architect] → [Human] |
| Quality dispute | [AI-Auditor] → [Human] |
| Scope question | [AI-Coordinator] → [Human] |
| Resource conflict | [AI-Coordinator] → [Human] |