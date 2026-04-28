# Handoff Points

## Definition

A **handoff point** is where one role's output becomes another role's input. Clear handoffs prevent work from falling through cracks and ensure continuity across sessions.

## Handoff Format

```markdown
| From | To | Phase | What | Format | Complete When |
|------|-----|-------|------|--------|----------------|
```

---

## Phase 0 → Phase 1 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [Human] | [AI-Researcher] | Commission brief, scope, constraints | README.md, scope.md, constraints.md | All files exist, purpose clear |

**Handoff Block**:
- `output_artifacts`: README.md, scope.md, constraints.md, success-criteria.md
- `next_phase_context`: README.md, scope.md, constraints.md
- `excluded_context`: Conversations about scope decisions

---

## Phase 1 → Phase 2 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Researcher] | [AI-Architect] | Synthesized knowledge, KB populated | KB entries, research/*.md | All topics have entries, KB usable |

**Handoff Block**:
- `output_artifacts`: research/domain-analysis.md, research/best-practices.md, KB entries
- `next_phase_context`: KB entries needed for spec
- `excluded_context`: Raw research notes

---

## Phase 2 → Phase 3 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [Human] | [AI-Architect] | Locked specification | docs/SPEC.md | All creative decisions locked, human approved |
| [AI-Architect] | [AI-Architect] | Architecture ready | architecture/*.md | Class diagram, interfaces defined |

**Handoff Block**:
- `output_artifacts`: SPEC.md, architecture document
- `next_phase_context`: SPEC.md, architecture
- `skill_validation`: `juce-plugin-spec` skill for spec compliance

**Critical Handoff**: [Human] must approve all creative decisions in SPEC.md before [AI-Builder] can implement.

---

## Phase 3 → Phase 4 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Architect] | [AI-Builder] | Architecture, task list | architecture/*.md, task-list.md | Architecture documented, tasks ordered |

**Handoff Block**:
- `output_artifacts`: Architecture document, task list
- `next_phase_context`: Architecture, task list
- `excluded_context`: Architecture alternatives considered

---

## Phase 4 → Phase 5 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Builder] | Project setup | CMakeLists.txt, tests/ | Build succeeds, tests pass, plugin loads |

**Handoff Block**:
- `output_artifacts`: CMakeLists.txt, test files, build output
- `next_phase_context`: Project structure
- `excluded_context`: IDE configuration details

---

## Phase 5 → Phase 6 Handoff (DSP Implementation)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Auditor] | Implemented DSP modules | Source files | Module implementations exist |
| [AI-Auditor] | [AI-Builder] | Review results | Review log | Two-stage review complete |

**Two-Stage Review Handoff**:

**Stage 1: Spec Compliance Review**
- What: Does implementation match SPEC?
- From: [AI-Auditor]
- To: [AI-Builder]
- Format: Review log with pass/fail per criterion
- Complete When: All spec compliance checks pass

**Stage 2: Code Quality Review**
- What: Buffer overflows, off-by-one, thread safety, edge cases?
- From: [AI-Auditor]
- To: [AI-Builder]
- Format: Review log with issues and severity
- Complete When: All critical/important issues resolved

**Fix Cycle Handoff**:
- [AI-Builder] fixes issues
- [AI-Auditor] re-reviews BOTH stages
- Repeat until pass

---

## Phase 6 → Phase 7 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Builder] | State management, presets | PresetData, PresetManager, state code | State round-trip test passes |

**Handoff Block**:
- `output_artifacts`: PresetData.h, PresetManager.cpp, state code
- `next_phase_context`: Preset structures
- `excluded_context`: Preset variations tried

---

## Phase 7 → Phase 8 Handoff

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Builder] | Processor integration | processBlock code | All parameters wired, SmoothedValue audit passed |

**Handoff Block**:
- `output_artifacts`: PluginProcessor.cpp
- `next_phase_context`: Parameter layout, processBlock
- `excluded_context`: Debug output

---

## Phase 8 → Phase 9 Handoff (GUI)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [Human] | GUI prototype | PluginEditor files | All controls implemented |
| [Human] | [AI-Builder] | GUI approval | Approval comment | Human approves visual design |

**Critical Handoff**: [Human] must approve GUI before build verification.

---

## Phase 9 → Phase 10 Handoff (Build Verification)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Auditor] | Built plugin | .vst3 file | Build succeeds |

**Handoff Block**:
- `output_artifacts`: Build output
- `next_phase_context`: Plugin binary
- `excluded_context`: Build logs

---

## Phase 10 → Phase 11 Handoff (DAW Testing)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [Human] | Plugin for testing | .vst3 in REAPER | Plugin loads, produces audio |
| [Human] | [AI-Builder] | Bug reports, sound approval | Testing log | Every control audible, sound approved |

**Critical Handoff**: [Human] MUST test in DAW. AI cannot perceive sound quality.

**Return Path**: If [Human] finds issues, return to Phase 5, 7, or 8.

---

## Phase 11 → Phase 12 Handoff (Licensing)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Builder] | Licensed plugin | Licensed binary | Licensing system works |

**Handoff Block**:
- `output_artifacts`: Licensed plugin
- `next_phase_context`: Licensing code
- `excluded_context`: Licensing SDK details

---

## Phase 12 → Phase 13 Handoff (Optimization)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [AI-Auditor] | Optimized plugin | Profiled binary | CPU acceptable, no denormals |
| [Human] | [AI-Builder] | Sound verification | Approval | Sound unchanged after optimization |

**Critical Handoff**: [Human] must verify sound unchanged after optimization.

---

## Phase 13 → Phase 14 Handoff (Marketplace)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [Human] | Marketplace content | Product page draft | Screenshots, description, audio demos |
| [Human] | [Human] | Pricing decision | pricing.md | Pricing documented |

---

## Phase 14 → Phase 15 Handoff (Marketing)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [Human] | [Human] | Marketing plan | marketing/strategy.md | Plan documented |

---

## Phase 15 → Phase 16 Handoff (Release)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [AI-Builder] | [Human] | Released plugin | Binary, docs | Released to marketplaces |

---

## Phase 16 Handoff (Retrospective)

| From | To | What | Format | Complete When |
|------|-----|------|--------|----------------|
| [Human] | [AI-Coordinator] | Lessons learned | retrospective.md | Documented |

**Handoff to Next Project**: Playbook updates extracted for future projects.

---

## Handoff Verification Checklist

Every handoff must pass:

- [ ] Output artifacts exist
- [ ] All files listed in artifact manifest
- [ ] Next phase can proceed with context_load only
- [ ] No orphaned context (everything needed is included)
- [ ] Human approval obtained where required