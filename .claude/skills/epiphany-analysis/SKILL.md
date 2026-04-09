---
name: epiphany-analysis
version: 0.1.0-placeholder
last_modified: 2026-04-09
description: "Downstream analysis skill for epiphany-omnipotent XML output. Consumes <omnipotent_output_v1> XML files saved by epiphany-omnipotent v1.5.0+ and produces structured analysis reports. NOT IMPLEMENTED — placeholder only. Input contract defined here for implementers."
---

# Epiphany Analysis

> **Status: NOT IMPLEMENTED — placeholder only.**
>
> This skill is a forward dependency of `epiphany-omnipotent` v1.5.0+. It is stubbed here to make the handoff coherent. Implementation is a separate project. See the implementation spec for full design details.

When invoked in placeholder state, inform the user:

> "epiphany-analysis is not yet implemented. This placeholder defines the input contract only. See `docs/superpowers/specs/2026-04-09-epiphany-omnipotent-output-design.md` §6 for implementation details."

---

## Trigger Conditions

| Trigger | Behavior |
|---------|----------|
| `/epiphany-analysis <filepath>` | Activate with the given file path as input |
| `/epiphany-analysis` (no filepath) | Activate, ask: "Path to the epiphany-omnipotent XML file?" |
| User explicitly says "epiphany-analysis" followed by a file path | Activate with that file path |

---

## Input Contract

- **Input:** file path to a readable `.xml` file produced by `epiphany-omnipotent` v1.5.0+
- **Location:** `~/epiphany/omnipotent/`
- **Schema:** `<omnipotent_output_v1>` as defined in `~/.claude/skills/epiphany-omnipotent/SKILL.md` v1.4.3+
- **Degraded inputs are valid:** `status=degraded`, `pg2_status=contaminated`, `reasoning_status=shallow` — handle gracefully, do not error on these states

### Fields Carrying Previously-Lost Mid-Pipeline Reasoning

These fields were dropped by the cancelled distillation design. They are now preserved in the XML and should be the **primary analysis targets**:

| Field | Content |
|-------|---------|
| `<lens_outputs>` — all 5 lenses | Raw SCAMPER ideas, Lateral moves, Morphological box, Six Hats perspectives, TRIZ contradictions, Reverse Brainstorming failure modes |
| `<synthesis>.<disagreement>` | Lens conflicts, value tensions, steel-manned positions per lens |
| `<synthesis>.<dialectical_options>` | Synthesis-generated alternatives integrating competing value positions |
| `<synthesis>.<gaps_from_frame>` | Unresolved gaps from frame coverage check |
| `<process_notes>.<reasoning_self_assessment>` | Runner's honest self-critique against 8 reasoning criteria |
| `<decision>.<matrix>` | Per-cell Pugh Matrix scores and rationale |
| `<framing_context>` | Deep structure principle, known/unknown/assumed, naive questions, observation vs recognition |

### Additional Fields Available

| Field | Content |
|-------|---------|
| `<meta>` | Scale, stakes, parameters used, injections applied |
| `<synthesis>.<agreement>` | Cross-lens agreement points |
| `<synthesis>.<survivors_summary>` | Alternatives that passed the Evidence-Share Filter |
| `<synthesis>.<risk_carryover>` | Inverted lessons and safeguards from S5 |
| `<decision>` | Full Pugh Matrix: criteria, baseline, scores, recommendation, runners-up |
| `<inventory>.<input_inventory>` | Original input preserved verbatim |
| `<verification_report>` | V1–V4 check results |
| `<downstream_handoff>` | Recommended next skill and matched rule |

---

## Implementation Reference

Full input contract, field annotations, and design rationale:
`docs/superpowers/specs/2026-04-09-epiphany-omnipotent-output-design.md`

Canonical XML schema:
`~/.claude/skills/epiphany-omnipotent/SKILL.md` v1.4.3+
