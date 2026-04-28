# Decisions Ledger — append at every phase gate

| Phase | Key Decisions | Constraints Discovered | Rejected Alternatives |
|-------|---------------|----------------------|----------------------|
| 0 | Purpose: Creative VST plugin development playbook for solo dev + Claude Code | Budget max $200/mo Claude; technical stack fixed; human makes all creative decisions | N/A |
| 1 | Research complete: 67 requirements defined; existing playbook audited; cross-cutting concerns identified | VST Playbook v7 needs commercial phases added; platform Linux-primary; KB 5-layer architecture confirmed | N/A |
| 2 | KB Architecture: Structured multi-layer with bridge layer; 5 layers defined; entry/bridge schemas defined | Sound Design → Technical bridge required for subjective→parameter translation; population strategy: placeholder→harvested→curated→synced | N/A |
| 3 | KB Bootstrapped: 10 harvested entries, 7 bridge entries across 6 layers | Bridge entries for warm/bright/lush/punchy/psychedelic/ambient/dark; harvested from domain-analysis, best-practices, failure modes | N/A |
| 4 | Process Architecture: 17 phases defined; early assessment at Phases 1 and 10; commercial phases 11-15 added to VST Playbook v7 structure | VST Playbook v7 phases 0-12 extended with 11-15 for licensing, marketplace, marketing; return paths from DAW testing to implementation phases | N/A |
| 5 | Role Engineering: 6 roles for role-based-single-agent workflow; Human + 5 AI roles (Builder, Architect, Researcher, Auditor, Coordinator) | Human makes all creative/approval decisions; AI roles are capability modes, not separate agents; escalation always to human | N/A |

## Complexity Profile (Phase 0)

| Dimension | Classification | Rationale |
|-----------|----------------|-----------|
| Process | **Complex** | Full lifecycle: development + testing + commercial + marketing = 11+ phases |
| Knowledge | **Bridged** | 5 layers (Technical, Sound Design, UI/UX, Commercial, Reference) with Sound Design ↔ Technical bridges |
| Roles | **Full** | 6 roles needed: Coordinator, Researcher, Architect, Builder, Auditor, Stakeholder |
| **Overall** | **Complex** | Highest of three dimensions; sets advisory guardrails for creation process |

## Key Decisions

1. **KB Architecture:** Structured (not flat) with 5 layers and bridge schema
2. **Workflow Model:** role-based-single-agent (solo dev + Claude Code)
3. **Scope:** Full product lifecycle including commercial/marketing (not just development)
4. **Target Platform:** Linux + REAPER only (v1.0)
5. **Extend VST Playbook v7:** KEEP phase structure, ADD commercial/licensing/marketing phases
6. **Bridge layer:** Sound Design → Technical translation required for creative DSP