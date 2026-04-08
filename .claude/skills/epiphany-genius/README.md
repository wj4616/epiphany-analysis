# Epiphany Genius

**A reasoning amplifier that operationalizes research-backed genius-level cognitive techniques into a 4-phase AI-native pipeline.**

[![Skill Type](https://img.shields.io/badge/skill-reasoning-purple)](https://claude.ai/code)
[![Version](https://img.shields.io/badge/version-1.0.0-blue)](https://github.com)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

---

## Overview

Epiphany Genius frames a problem, explores solutions across parallel cognitive modes, stress-tests them against contradictions, and synthesizes a verified answer — mirroring how expert problem-solvers think, but running the steps in parallel where humans run them sequentially.

**Based on research from cognitive science and genius methodology:**
- **Knowledge Organization / Chunking** (§1.1) — Surface features → deep principle mapping
- **DMN↔ECN Network Switching** (§1.2) — Associative and analytical modes separated by phase
- **Incubation** (§1.3) — Iterative retry with explicit failure context
- **Cross-Domain Pattern Transfer** (§1.4) — Analogical reasoning across domains
- **Combinatorial Play + Janusian Process** (§1.5, §2) — Holding contradictions, exploring combinations
- **Feynman's Algorithm, Einstein Thought Experiments, da Vinci's Process** (§2) — Naive questions, limit cases, observation vs. recognition

---

## Installation

The skill is installed at:
```
~/.claude/skills/epiphany-genius/SKILL.md
```

No additional dependencies required.

---

## Usage

### Invocation

| Trigger | Behavior |
|---------|----------|
| `/epiphany-genius` | Activate immediately. If no problem statement, ask for one. |
| User says "epiphany-genius" or "epiphany genius" | Activate. Ask for problem if not provided. |
| User says "think harder" / "brainstorm" / "be creative" without naming this skill | **Do NOT activate.** |
| Pure retrieval task | **Do NOT activate.** |
| Already-solved problem | **Do NOT activate.** |
| Problem blocked on missing information | **Do NOT activate — gather facts first.** |
| Time-critical incident response | **Do NOT activate — verification gates add latency.** |

### Composition Model

**Standalone (primary):** Run `/epiphany-genius` with a problem statement. Output is structured XML: frame → candidates → stress-tested finalist → rationale.

**Pre-step pattern (secondary):** Any skill can document: *"For novel or high-stakes problems, run `/epiphany-genius` first, then pass its output as context."*

---

## Pipeline Architecture

```
[1] FRAME ──▶ [2] DIVERGE ──▶ [3] STRESS-TEST ──▶ [4] SYNTHESIZE ──▶ output
                    ▲                    │
                    │                    ▼
                    └────────── INCUBATE LOOP ──────────┐
                              (backward edges)          │
                                                       │
                    ◀───────────────────────────────────┘
```

### Phase 1: FRAME

**What it does:** Generates N candidate framings in parallel, selects the strongest.

Each framing contains:
- **Deep-structure principle** — What kind of problem is this?
- **Success criterion** — What does "solved" look like?
- **Known / Unknown / Assumed** — Explicit lists
- **Naive questions** — What would an outsider ask?
- **Observation vs. recognition** — What's stated vs. what I'm recognizing

**Selection criteria:** Explanatory fit (hard filter) → Assumption surfacing → Observation/recognition split → Naive-question productivity

### Phase 2: DIVERGE

**What it does:** Runs three generative branches in parallel.

| Branch | Operation | Source |
|--------|-----------|--------|
| Associative | Chain associations from deep-structure principle | DMN-mode (§1.2) |
| Combinatorial | Enumerate element combinations, including overinclusive | Combinatorial Play (§1.5) |
| Analogical | Map to structurally similar problems in other domains | Cross-Domain Transfer (§1.4) |

**Output:** 6–12 candidates after deduplication, each with branch lineage and initial plausibility.

### Phase 3: STRESS-TEST

**What it does:** Applies three attacks in parallel to every candidate.

| Attack | Operation |
|--------|-----------|
| Contradiction-holding (Janusian) | In what respect are both the candidate and its negation true? |
| Limit-case (Einstein) | What happens at boundary conditions? |
| Bayesian update (Feynman) | What's the updated confidence under strongest counterevidence? |

**Classification:** Survived (objections absorbed) vs. Killed (objections cannot be absorbed).

**Loop decision:**
- ≥1 survivor → forward to Synthesize
- All killed, common frame root cause → LOOP to Frame
- All killed, candidate-specific failures → LOOP to Diverge
- Budget exhausted → forward highest residual plausibility with escape-hatch

### Phase 4: SYNTHESIZE

**What it does:** Combines surviving candidates, runs gap scan, emits XML output.

**Answer cases:**
- One survivor → emit as answer
- Multiple with dominance → pick dominant, record runners-up
- Multiple without dominance → emit ranked set (top 2–3)
- Zero survivors, budget exhausted → emit with escape-hatch block

**Gap scan:** Success criterion coverage → Unknown position → Naive-question answering → Unexplored dimensions → Unattacked boundaries

---

## Stakes-Based Calibration

Depth is chosen automatically based on problem assessment, not manual flags.

| Stakes | Parameters |
|--------|-------------|
| **low** | `N_framings=2`, `N_candidates_per_branch=2`, `loop_budget=1`, `bayesian_bar=low`, `gap_scan_depth=core` |
| **medium** (default) | `N_framings=3`, `N_candidates_per_branch=3`, `loop_budget=2`, `bayesian_bar=medium`, `gap_scan_depth=full` |
| **high** | `N_framings=4`, `N_candidates_per_branch=4`, `loop_budget=3`, `bayesian_bar=high`, `gap_scan_depth=full` |

**Signals:** Reversibility, impact, explicit "this is important", compliance/legal implications.

---

## Output Schema

```xml
<epiphany_genius_output>
  <frame>
    <deep_structure_principle>...</deep_structure_principle>
    <success_criterion>...</success_criterion>
    <known>...</known>
    <unknown>...</unknown>
    <assumed>...</assumed>
    <naive_questions>
      <what>...</what>
      <why>...</why>
      <how>...</how>
    </naive_questions>
    <observation_vs_recognition>
      <observed>...</observed>
      <recognized>...</recognized>
      <divergence>...</divergence>
    </observation_vs_recognition>
  </frame>

  <answer>
    <primary>...</primary>
    <!-- optional: runners_up OR ranked_set -->
  </answer>

  <tradeoffs>
    <tradeoff attack="contradiction|limit_case|bayesian">...</tradeoff>
  </tradeoffs>

  <!-- optional: confidence_notes, loop_history, escape_hatch -->

  <provenance>
    <stakes>low|medium|high</stakes>
    <parameters>...</parameters>
  </provenance>
</epiphany_genius_output>
```

---

## Corporate Use

The pipeline handles corporate enterprise brainstorming and strategic planning through stakes assessment, not a separate mode.

| Scenario | Stakes | Parameters |
|----------|--------|------------|
| Strategic decision ("should we acquire X?") | high | Full depth |
| Cross-functional planning | medium | Defaults |
| Executive ideation | medium | Defaults |
| Crisis response / post-incident | high | Full depth |
| Quick retro, low-stakes workshop | low | Core depth |

**Phase mapping for corporate vocabulary:**
- FRAME → Problem framing & success-criterion alignment
- DIVERGE → Structured ideation
- STRESS-TEST → Risk surfacing / critical review
- SYNTHESIZE → Decision framing / recommendation packaging

---

## Confidence Tiers

| Research Finding | Tier | Output Annotation |
|------------------|------|-------------------|
| Chunking (§1.1) | High | None |
| DMN↔ECN switching (§1.2) | High | None (architectural) |
| Incubation (§1.3) | Medium-High | `<confidence_note>` when loops fire |
| Cross-Domain Transfer (§1.4) | High | None |
| Combinatorial Play (§1.5) | Medium | `<confidence_note>` if primary answer |
| Janusian Process (§1.5/§2) | Medium | `<confidence_note>` if classification changed |

---

## Why This Works

### The AI-Native Advantage

| Phase | Human (sequential) | AI (parallel) |
|-------|-------------------|----------------|
| Frame | Try framings one at a time | Generate N candidates at once, select strongest |
| Diverge | Sample one associative chain | Run all three branches in one pass |
| Stress-Test | Attack candidates one at a time | Apply all three attacks to all candidates simultaneously |
| Synthesize | Write, then recheck | Draft and gap-scan in the same pass |

### Incubation Translated

Human incubation is stepping away so the unconscious can work. AI-native incubation is **iterative retry with explicit failure context** — the retry is informed, not stochastic. Each backward edge carries a structured failure report: what was tried, why it failed, what to do differently.

---

## Output Gate

Seven checks run before emission:

| Check | Behavior |
|-------|----------|
| G1: Frame completeness | Hard fail → escape-hatch |
| G2: Answer traces to pipeline candidate | Hard fail → escape-hatch |
| G3: Tradeoffs populated | Fix inline |
| G4: Gap scan reflected in escape-hatch | Fix inline |
| G5: No fabricated source attributions | Remove + hard fail if primary |
| G6: Same success criterion | Fix inline + hard fail if drift too large |
| G7: Escape hatch populated | Fix inline |

---

## Limitations

- **No auto-activation** — must be explicitly invoked
- **No self-apply mode** — cannot recursively call itself
- **No runtime hooks** — composition is by convention, not injection
- **Hard gate blocks** — insufficient problems don't enter the pipeline
- **Escape-hatch for budget exhaustion** — when loops run out, best-available answer is emitted with unresolved gap documented

---

## References

The skill operationalizes research from cognitive science and genius methodology:

1. **Knowledge Organization / Chunking** — Surface features → deep principle (§1.1)
2. **DMN↔ECN Network Switching** — Associative and analytical mode separation (§1.2)
3. **Incubation** — Iterative retry with failure context (§1.3)
4. **Cross-Domain Pattern Transfer** — Analogical reasoning (§1.4)
5. **Combinatorial Play** — Exploring combinations (§1.5)
6. **Janusian Process** — Holding contradictions (§1.5, §2)
7. **Feynman's Algorithm** — Naive questions, Bayesian update, gap detection (§2.1)
8. **Einstein Thought Experiments** — Limit-case testing (§2.2)
9. **da Vinci's Process** — Observation vs. recognition, nine-stage process (§2.3)

---

## License

MIT License

---

## Changelog

### v1.0.0 (2026-04-08)
- Initial release
- 4-phase pipeline: Frame → Diverge → Stress-Test → Synthesize
- Stakes-based calibration (low/medium/high)
- 5 backward edges for incubation loop
- 7-gate output verification
- XML output schema with provenance