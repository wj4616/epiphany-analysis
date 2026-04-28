---
name: epiphany-graph-analysis-enhanced-prompt
source: prompt-graph (normal mode)
date: 2026-04-24
original_input: recreate epiphany-analysis as graph-of-thought skill
complexity: high (>12 INVENTORY items)
audit_rev: 2 (all 16 audit findings applied)
---

<prompt>
<meta source="prompt-graph" mode="normal" version="1.0"/>

<role>
You are a skill architect designing the "epiphany-graph-analysis" skill — a Graph-of-Thought analysis tool that takes an original text (Node A) and an analysis of that text (Node B), then produces multiple analysis artifacts through an explicit, signal-driven graph topology. You have deep expertise in:
- Graph-of-Thought architectures (node/edge/signal topology, ready-set scheduling, scale-gated activation)
- The existing epiphany-analysis skill (6-stage inline pipeline with dual-input model, genius detection, section-tailoring, two-drafts-pick-best)
- The existing epiphany-graph-genius skill (11-node graph.json topology with inline/spawn hybrid, back-edges, SIGNAL_STATE data flow)
- Cognitive science research on genius-mind traits and their computational exploitation
- Claude Code skill system mechanics (Agent tool spawns, role-switched inline blocks, module files, graph.json schema)

Your design must be implementable as a concrete skill with files (SKILL.md, graph.json, modules/N*.md, kb/, scripts/, tests/). Every node, edge, signal, and artifact must be explicitly specified — no ambiguity.
</role>

<context>
## Design Precedents

### epiphany-analysis v2.0.0 (6-stage inline pipeline)
The current skill that epiphany-graph-analysis replaces. Key features to PRESERVE:
1. **Dual-input model** — Node A (original text) + Node B (analysis of text)
2. **Genius Detection** — classifies Node B as genius-current, genius-drift, or generic-fallback using canonical-or-synonym header counting
3. **Section-Tailoring Map** — 10 sections for genius-or-drift Node B (Headline Insight first, then Theory Collisions, Discovery vs. Proof, Independence-Verified Bridges, Alternative Hypotheses, Density-Checked Falsification, Scope Limits, Coherence Signals, Generalization Checks, Open Questions & Next Probes)
4. **Two-drafts-pick-best (HG-3)** — every solution entry must come from comparing two independently drafted solutions
5. **Bounded iteration (HG-5)** — every loop has a cap; no unbounded constructs
6. **Self-audit (R1-R6)** — runtime verification: every D_i has ≥1 solution, every solution targets a verifiable section, no solution contradictions, no dropped constraints, well-formed output, current timestamps
7. **Non-overwrite guarantee (HG-1, HG-4)** — never write to Node A's directory; all outputs in session directory
8. **Self-contained runtime (HG-2)** — no web search, no grep/glob, no user questions during execution

Key problems to SOLVE:
- Fully sequential execution (no parallelism) — root cause of 30+ minute runtime
- "Loosely defined nodes" — A, B, A1, B1, C, D, E are named but have no explicit edges, signals, or routing
- No adaptive depth — all 6 stages always execute regardless of input complexity
- No scale modes — no way to run lighter analysis for simpler inputs
- Single artifact dependency chain — enhanced.md depends on all prior stages completing

### epiphany-graph-genius v1.1.0 (11-node graph topology)
The architectural model to adopt. Key features:
1. **graph.json** — single source of truth for topology: node registry, edge/channel table, scale gates, signal fields, activation conditions
2. **Scale-gated activation** — MINIMAL (4-5 nodes), STANDARD (8-9 nodes), DEEP (all 11 nodes); inactive nodes/edges removed from working topology
3. **Signal-driven routing** — nodes raise signals (S3_thin_or_empty, S6_no_alternatives) that alter downstream execution via gate-condition edges
4. **Inline/spawn hybrid** — cheap operations (decomposition, precision, routing) run inline in orchestrator context; expensive operations (lateral, adversarial, aggregation, synthesis) run as Agent tool spawns
5. **Ready-set scheduling** — inline-to-fixpoint then parallel spawn fire; SIGNAL_STATE append-only dict for data flow between nodes
6. **Back-edges** — N3→N3.1 (defixation on thin analysis) and N8→N3.1 (late defixation on no alternatives)
7. **PRC1 validation** — DAG check (excluding back-edges), edge resolution, signal field validity, connectivity verification before execution
8. **Context budgets** — per-node line limits (50-2000 lines by type)
9. **Spawn failure handling** — log warning, skip node, cascade-block dependents, continue pipeline degraded
10. **graph.schema.json** — validates graph.json structure programmatically

### Knowledge Base Research (design-time only — NOT integrated into skill runtime)
Design-time queries to Dify cognitive/thought KBs inform these design decisions:
- **GoT O(log_k N) latency advantage** — parallel branches with aggregation nodes dominate sequential CoT (N,N) for analysis tasks
- **Signal-driven control flow** — data-dependent routing enables adaptive depth without human judgment
- **Spreading activation + elegance rubric** — peripheral exploration that converges on high-signal nodes (adopted from graph-genius N3)
- **Ohlsson defixation** — back-edge mechanism when primary analysis is thin or stuck
- **Feynman precision + vocabulary rubric** — forcing specificity in analysis outputs
- **Popper falsification** — adversarial verification of analysis claims
- **de Bono lateral** — domain-crossing for constraint escape in analysis
- **Boden creativity types** — H-creativity vs P-creativity classification for solution categorization
- **Genius-mind cognitive traits (Tier 1)** — 19 traits with verified AI implementation feasibility

## Computer/AI Agent Advantages to Exploit

These are concrete mechanisms where graph-of-thought topology gives the skill capabilities that a sequential pipeline cannot achieve:

1. **Integrated dual analysis with sub-subagent parallelism** — N2 (DualAnalyze) is a single orchestrator-level spawn that internally fans out to two sub-subagents for A1 and B1 analysis, running them in parallel within its Agent context. This counts as 1 spawn at orchestrator level but achieves parallel A1/B1 execution internally. In epiphany-analysis, these run sequentially (M-ANALYZE does A1 then B1). N2's internal parallelism halves the analysis phase wall-clock time while keeping spawn budget lean.

2. **Signal-driven adaptive depth** — If Node B is generic-fallback (thin analysis), skip the Section-Tailoring Map entirely and use a flat extraction pass instead. If Node B is genius-current with rich findings, enable the full 10-section processing. This is controlled by signals raised at the Detection node, not by human judgment.

3. **Deterministic ready-set scheduling** — When independent nodes are all eligible (all required edges satisfied), they fire in parallel as Agent spawns. The orchestrator doesn't need to decide execution order — graph topology determines it. This eliminates the human-in-the-loop scheduling bottleneck.

4. **Inline-vs-spawn optimization** — Cheap operations (type detection, section routing, cross-referencing, conflict resolution, solution engineering, formatting) run inline in ≤1500 lines. Expensive operations (dual analysis, ideation, adversarial verification, synthesis) run as Agent spawns with 1200-2000 line budgets. This keeps orchestrator context lean while giving heavy nodes full context.

5. **Typed signal-state data flow** — SIGNAL_STATE keyed by (node_id, signal_field) with append-only invariant (PRC2). No ambiguity about what data passes between nodes. No string-parsing of "stage file contents" — structured digests flow through declared channels.

6. **Multi-stage artifact production** — Artifacts are produced at different pipeline stages: solution-catalog.md from N8 (after cross-reference and solution engineering), enhanced.md from N10 (after synthesis), self-audit.md from N11 (after verification). Each stage builds on prior outputs, providing richer deliverables than epiphany-analysis's single enhanced.md output. The orchestrator assembles analysis-report.md and graph-trace.json from node outputs at pipeline end.

7. **Adaptive breadth over depth** — When spawn budget is tight, prefer breadth (more parallel nodes with lighter context) over depth (fewer sequential nodes with richer context). This exploits the computer's ability to run Agent calls in parallel, which a sequential pipeline cannot do.
</context>

<task>

## 1. Skill Identity

Create a skill named "epiphany-graph-analysis" with:
- **trigger**: `/epiphany-graph-analysis`
- **description**: Takes an original text (Node A) and an analysis of that text (Node B), producing multiple analysis artifacts through a 12-node graph-of-thought topology with signal-driven routing, scale-gated activation, and ready-set scheduling.
- **skill_path**: `~/.claude/skills/epiphany-graph-analysis/`
- **save_path**: `~/docs/epiphany/graph-analysis/`
- **graph_file**: `~/.claude/skills/epiphany-graph-analysis/graph.json`

## 2. Graph Topology Specification

Design a 12-node graph topology with the following structure. This is the MANDATORY deliverable — the skill's graph.json must implement exactly this topology.

### 2.1 Node Registry

| Node ID | Node Name | Type | Exec Type | Hat (Cognitive Persona) | Context Budget | Scale Gates | Raises Signals | KB Files | Output File |
|---------|-----------|------|-----------|------------------------|----------------|-------------|----------------|----------|-------------|
| N1 | IntakeDecompose | DECOMPOSITION | inline | Einstein/Feynman | 200 | MINIMAL, STANDARD, DEEP | — | input-preloading-templates.md | N1-intake-decompose.md |
| N2 | DualAnalyze | ANALYZE | spawn | Tesla+Darwin | 1500 | MINIMAL, STANDARD, DEEP | S2_thin_B | analysis-methodology.md | N2-dual-analyze.md |
| N3 | SectionTailor | TAILOR | inline | Feynman | 600 | STANDARD, DEEP | — | section-tailoring-map.md | N3-section-tailor.md |
| N4 | CrossReference | XREF | inline | Tesla (bridge-builder) | 800 | MINIMAL, STANDARD, DEEP | — | — | N4-cross-reference.md |
| N5 | LateralIdeate | LATERAL | spawn | de Bono | 1200 | STANDARD, DEEP | — | domain-catalog.md, debono-techniques.md | N5-lateral-ideate.md |
| N6 | Defixation | DEFIXATION | inline | Ohlsson | 700 | STANDARD, DEEP | — | ohlsson-defixation.md | N6-defixation.md |
| N7 | AdversarialVerify | ADVERSARIAL | spawn | Popper+Millikan | 1500 | STANDARD, DEEP | S7_no_alternatives | falsification-checklists.md | N7-adversarial-verify.md |
| N8 | SolutionEngineer | ENGINEER | inline | Feynman+Boden | 1500 | MINIMAL, STANDARD, DEEP | — | verification-gates.md | N8-solution-engineer.md |
| N9 | Router | ROUTER | inline | null | 50 | STANDARD, DEEP | — | — | N9-router-record.md |
| N10 | Synthesize | SYNTHESIS | spawn | Feynman+Boden(synthesizer) | 2000 | MINIMAL, STANDARD, DEEP | — | — | N10-synthesize.md |
| N11 | Verify | VERIFICATION | inline | Popper | 600 | MINIMAL, STANDARD, DEEP | S11_artifact_gap | — | N11-verify.md |
| N12 | Expand | EXPANSION | inline | Feynman | 800 | DEEP | — | — | N12-expand.md |

**Node type taxonomy:**
- DECOMPOSITION: intake parsing, genius detection, input classification
- ANALYZE: dual analysis of A and B (internally fans out to sub-subagents for A1 and B1)
- TAILOR: section-specific extraction from Node B (Section-Tailoring Map)
- XREF: cross-reference mapping between B-findings and A-sections
- LATERAL: divergent ideation across domains
- DEFIXATION: constraint relaxation and re-encoding when analysis is thin
- ADVERSARIAL: falsification of analysis claims and solution candidates
- ENGINEER: two-drafts-pick-best solution construction (preserves HG-3; inline role-switched block)
- ROUTER: control-flow decision (pure signal consumer, no hat)
- SYNTHESIS: final integration into enhanced.md
- VERIFICATION: self-audit (R1-R6) plus artifact completeness check
- EXPANSION: second-pass deepening for DEEP mode

**N8 inline execution note:** SolutionEngineer runs as an inline role-switched block in the orchestrator context, not as a separate Agent spawn. The two-drafts-pick-best protocol (HG-3) executes within this block: for each solution candidate, the block independently drafts two solutions, compares them, and keeps the best. This saves 1 spawn per run compared to a spawn-based design, keeping MINIMAL at ≤2 spawns and STANDARD at ≤4 spawns.

**N2 internal fan-out:** DualAnalyze is 1 orchestrator-level spawn but internally dispatches two sub-subagent calls (one for A1 analysis, one for B1 analysis) within its Agent context, running them in parallel. This achieves the wall-clock benefit of parallel A1/B1 execution while counting as 1 spawn at the orchestrator level (matching graph-genius N6's internal fan-out pattern).

### 2.2 Edge/Channel Table

| Edge ID | Source → Target | Type | Signal Field | Scale Gates | Gate Condition |
|---------|----------------|------|-------------|-------------|---------------|
| E01 | input → N1 | required | — | MINIMAL, STANDARD, DEEP | — |
| E02 | N1 → N2 | required | intake_digest | MINIMAL, STANDARD, DEEP | — |
| E03 | N1 → N4 | optional | intake_digest | MINIMAL, STANDARD, DEEP | — |
| E04 | N2 → N3 | forward-conditional | analysis_digest | STANDARD, DEEP | SIGNAL_STATE[(N1, intake_digest)].node_b_type in ['genius-current', 'genius-drift'] |
| E05 | N2 → N4 | required | analysis_digest | MINIMAL, STANDARD, DEEP | — |
| E06 | N2 → N6 | back-edge | gate:S2_thin_B | STANDARD, DEEP | SIGNAL_STATE[(N2, analysis_digest)].signal_flags includes 'S2_thin_B' |
| E07 | N3 → N4 | required | tailored_B1 | STANDARD, DEEP | — |
| E08 | N4 → N5 | required | xref_map | STANDARD, DEEP | — |
| E09 | N4 → N8 | required | xref_map | MINIMAL, STANDARD, DEEP | — |
| E10 | N5 → N7 | required | ideas_digest | STANDARD, DEEP | — |
| E11 | N5 → N8 | required | ideas_digest | STANDARD, DEEP | — |
| E13 | N6 → N8 | optional | breakthrough_digest | STANDARD, DEEP | — |
| E14 | N7 → N9 | required | falsification_result | STANDARD, DEEP | — |
| E15 | N8 → N10 | required | solutions_digest | MINIMAL, STANDARD, DEEP | — |
| E16 | N9 → N6 | back-edge | gate:S7_no_alternatives AND NOT N6_ran | STANDARD, DEEP | SIGNAL_STATE[(N7, falsification_result)].signal_flags includes 'S7_no_alternatives' AND 'N6' not in executed_nodes |
| E17 | N9 → N8 | gate-open | falsification_digest | STANDARD, DEEP | N9 executed (unconditionally opens) |
| E18 | N10 → N11 | required | enhanced_draft | MINIMAL, STANDARD, DEEP | — |
| E19 | N11 → N12 | forward-conditional | first_pass_verified | DEEP | SIGNAL_STATE[(N11, verification_report)].signal_flags includes 'S11_artifact_gap' AND mode=DEEP |
| E20 | N12 → N10 | back-edge | expansion_digest | DEEP | N12 completed with expansions |
| E21 | N10 → output | terminal | — | MINIMAL, STANDARD, DEEP | — |
| E22 | N7 → N8 | optional | adversarial_digest | STANDARD, DEEP | — |
| E23 | N1 → N3 | optional | intake_digest | STANDARD, DEEP | — |
| E24 | N6 → N7 | optional | breakthrough_digest | STANDARD, DEEP | — |

**Edge type taxonomy:**
- required: must be present in SIGNAL_STATE for target to activate
- optional: never blocks activation; consumed if present
- back-edge: requires explicit enqueue, never activated by standard ready-set rule
- gate-open: unconditionally resolves once source executes
- forward-conditional: activates only when specific gate condition evaluates true (checked at ready-set compute time)
- terminal: pipeline exit

**Cardinality:**
- E02: 1:1 from N1 to N2
- E03, E23: 1:N fan-out from N1 (N4 + N3, both optional/forward-conditional, activated when targets become eligible from other required edges)
- E04, E05: 1:N fan-out from N2 (tailor path + xref path)
- E10, E11: 1:N fan-out from N5 (adversarial + engineer)
- E13, E24: 1:N fan-out from N6 (engineer + adversarial, both optional)
- E06, E16: back-edges for defixation loop (each fires ≤1 per run)
- All others: 1:1

**Key edge decisions:**
- E03 (N1→N4) is optional: N4's activation is gated by E05 (N2→N4, required). E03 provides N1's intake_digest directly to N4 for Node A text reference, but doesn't affect scheduling.
- E04 (N2→N3) is forward-conditional: N3 only activates when Node B is genius-current or genius-drift. Generic-fallback Node B skips N3 entirely.
- E11 (N5→N8) is required in STANDARD/DEEP: N8 must wait for ideation output before engineering solutions. In MINIMAL, E11 is inactive and N8 works from E09 (xref_map) alone.
- E23 (N1→N3) is optional: gives N3 direct access to N1's intake_digest (including Node A text and genius type tag) without depending on N2's analysis_digest passing it through.
- E24 (N6→N7) is optional: when defixation runs early (via E06), breakthrough_digest is available to N7 for adversarial context. When defixation runs late (via E16), N7 has already completed and the edge is harmless (optional, never blocks).

### 2.3 Signal System

| Signal ID | Raised By | Condition | Consumed By | Effect |
|-----------|-----------|-----------|-------------|--------|
| S2_thin_B | N2 (DualAnalyze) | Node B is generic-fallback OR has <3 canonical genius sections | E06 gate condition → N6 (Defixation) | Triggers defixation back-edge; E04 (N2→N3) gate condition fails, skipping N3 SectionTailor entirely |
| S7_no_alternatives | N7 (AdversarialVerify) | All solution candidates survive falsification with no counter-examples found | E14 carries falsification_result to N9 (Router); N9 evaluates E16 gate condition → N6 (Defixation) if N6 hasn't run | Triggers late defixation if N6 hasn't run yet; otherwise N9 opens gate to N8 via E17 |
| S11_artifact_gap | N11 (Verify) | Self-audit R1-R5 pass but R6 reveals artifact incompleteness or thin spots in DEEP mode | E19 gate condition → N12 (Expand) | Triggers expansion pass in DEEP mode only |

**Signal propagation note:** Signals are embedded in the node's output digest written to SIGNAL_STATE. Downstream edges with gate conditions read the relevant signal_flags from SIGNAL_STATE at ready-set compute time. The signal doesn't travel along a specific edge — it's visible to any edge whose gate condition references it. E14 carries falsification_result (the data payload) to N9; the S7_no_alternatives signal is in that payload's signal_flags, which N9 reads to decide whether to enqueue N6 via E16.

**SIGNAL_STATE invariant (PRC2):** Append-only. Never overwrite an existing (node_id, signal_field) key. Each node writes its output digest(s) upon completion.

### 2.4 Genius Detection (integrated into N1, not separate pre-stage)

N1 IntakeDecompose performs Genius Detection on Node B as part of decomposition:
- Count canonical-or-synonym headers (Headline Insight, Theory Collisions, Discovery vs. Proof, Independence-Verified Bridges, Alternative Hypotheses, Density-Checked Falsification, Scope Limits, Coherence Signals, Generalization Checks, Open Questions & Next Probes)
- ≥8 canonical-or-synonym → `genius-current` tag
- 3-7 → `genius-drift` tag
- <3 → `generic-fallback` tag
- Tag is written to SIGNAL_STATE as part of intake_digest (field: `node_b_type`)
- Tag controls E04 (N2→N3): forward-conditional edge fires only for genius-current or genius-drift
- Generic-fallback → S2_thin_B signal raised by N2 → E06 back-edge fires → N6 defixation runs

### 2.5 Pipeline Diagram (STANDARD mode)

```
input → N1 (IntakeDecompose)
          │
          ├─────► N2 (DualAnalyze) [SPAWN — internally fans out A1 ‖ B1]
          │         │
          │         ├─[if genius-current/drift]──► N3 (SectionTailor) ◄── N1 intake_digest (E23)
          │         │                                 │
          │         │                                 ▼
          │         │              N4 (CrossReference) ◄── N1 intake_digest (E03, optional)
          │         │               ▲                   ▲
          │         │               │ analysis_digest   │ tailored_B1
          │         └───────────────┘                   │
          │                                           │
          │         ┌──[if S2_thin_B]──► N6 (Defixation)──► breakthrough_digest ──┐
          │         │                                           │                   │
          │         │                                           ├────► N7 (AdversarialVerify) [SPAWN]
          │         │                                           │       (E24, optional)
          │         │                                           │
          │         │                              xref_map ──┼────► N5 (LateralIdeate) [SPAWN]
          │         │                                           │        │
          │         │                                           │        ├──► N7 (AdversarialVerify) [SPAWN]
          │         │                                           │        │     (E10, required)
          │         │                                           │        │
          │         │                                           │        └──► N8 (SolutionEngineer) [INLINE]
          │         │                                           │              (E11, required)
          │         │                                           │                ▲
          │         │                              xref_map ───┼────────────────┘ (E09, required)
          │         │                                           │                │
          │         │                                           │  breakthrough_digest (E13, optional)
          │         │                              N6 ─────────┘                │
          │         │                                                           ▼
          │         │                                           N8 (SolutionEngineer) [INLINE]
          │         │                                                           │
          │         │                         N9 (Router) ◄── N7 falsification_result (E14)
          │         │                           ├──► N6 (Defixation) [if S7_no_alternatives + N6 not run] (E16)
          │         │                           └──► gate-open to N8 (E17)
          │         │                                                           │
          │         │                                                           ▼
          │         │                                                    N10 (Synthesize) [SPAWN]
          │         │                                                           │
          │         │                                                           ▼
          │         │                                                    N11 (Verify) → output
          │         │                                                     └──► [DEEP only] N12 → N10 (E20 back-edge)
```

### 2.6 Mode Activation Matrix

| Mode | Flag(s) | Active Nodes | Active Edges | Spawn Budget | Wall-clock Target |
|------|---------|--------------|-------------|-------------|-------------------|
| MINIMAL | `--minimal` | N1, N2, N4, N8, N10, N11 (6 nodes) | E01, E02, E03, E05, E09, E15, E18, E21 (8 edges) | ≤2 | ≤8 min |
| STANDARD | (no flag) | N1-N11 excl N12 (11 nodes) | E01-E11, E13-E18, E22-E24 (20 edges, E12 removed, E19/E20 inactive) | ≤4 | ≤18 min |
| DEEP | `--deep` | N1-N12 (all 12) | all 24 edges | ≤5 | ≤35 min |

**Spawn count verification:**
- MINIMAL: N2(spawn) + N10(spawn) = **2 spawns** ≤2 ✓
- STANDARD: N2(spawn) + N5(spawn) + N7(spawn) + N10(spawn) = **4 spawns** ≤4 ✓
- DEEP: N2(spawn) + N5(spawn) + N7(spawn) + N10(spawn) = **4 spawns** ≤5 ✓

**Per-node mode activation:**

| Node | MINIMAL | STANDARD | DEEP |
|------|:-------:|:--------:|:----:|
| N1 IntakeDecompose | ✓ | ✓ | ✓ |
| N2 DualAnalyze | ✓ | ✓ | ✓ |
| N3 SectionTailor | — | conditional* | conditional* |
| N4 CrossReference | ✓ | ✓ | ✓ |
| N5 LateralIdeate | — | ✓ | ✓ |
| N6 Defixation | — | cond | cond |
| N7 AdversarialVerify | — | ✓ | ✓ |
| N8 SolutionEngineer | ✓ | ✓ | ✓ |
| N9 Router | — | ✓ | ✓ |
| N10 Synthesize | ✓ | ✓ | ✓ |
| N11 Verify | ✓ | ✓ | ✓ |
| N12 Expand | — | — | ✓ |

*N3 is conditional in STANDARD/DEEP: activates only when E04 gate condition is met (Node B type = genius-current or genius-drift). Skipped for generic-fallback Node B.

**MINIMAL mode behavior:** Runs dual analysis (N2), cross-references findings (N4), engineers solutions directly from cross-reference without ideation or adversarial verification (N8), synthesizes (N10), and verifies (N11). No Section-Tailoring Map, no lateral ideation, no defixation, no adversarial testing. N8 works from xref_map alone (E09 from N4; E11 from N5 inactive). Fast but shallow. 2 spawns: N2 + N10.

**STANDARD mode behavior:** Full analysis pipeline with conditional Section-Tailoring Map for genius Node B (skipped for generic-fallback), lateral ideation, conditional defixation (triggered by S2_thin_B or S7_no_alternatives signals), adversarial verification, router-driven control flow, and inline two-drafts solution engineering. 4 spawns: N2 + N5 + N7 + N10.

**DEEP mode behavior:** STANDARD + expansion pass (N12) when verification reveals artifact gaps (S11_artifact_gap signal), with back-edge to N10 for re-synthesis of expanded output. Slowest but most thorough. 4 spawns (same as STANDARD; N12 is inline).

## 3. Dual-Input Model

### 3.1 Node A (Original Text)
- Any prose, code, structured document, or specification
- Read from file path or inline content
- Written verbatim to `{session_dir}/input-a.md`

### 3.2 Node B (Analysis of Text)
- File path to analysis document
- Three types detected via Genius Detection (integrated into N1):
  - **genius-current**: output of epiphany-genius or epiphany-graph-genius (≥8 canonical sections)
  - **genius-drift**: partial genius output with some canonical sections (3-7 matches)
  - **generic-fallback**: any other analysis document (<3 matches)
- Written verbatim to `{session_dir}/input-b.md`

### 3.3 Missing Node B Fallback
If no Node B is provided: N2 generates a lightweight inline analysis of Node A (structural decomposition, key claims, apparent gaps). This is always shallower than a dedicated analysis document but prevents the skill from halting. Emit advisory: "No analysis document provided. Generating inline structural analysis as Node B substitute. For richer results, provide a dedicated analysis document."

## 4. Section-Tailoring Map (N3)

Preserved from epiphany-analysis, activated only when Node B is genius-current or genius-drift (controlled by E04 forward-conditional edge):

| # | Section | Focus | Idea-extraction Strategy | Special Handling |
|---|---------|-------|--------------------------|------------------|
| 1 | Headline Insight | Primary conclusion + structural risks | Highest-leverage improvement first; anchor all later ideas | Processed FIRST |
| 2 | Theory Collisions | Claim A vs. Claim B + discriminating condition | One idea per unresolved collision | Compare claims against discriminating condition |
| 3 | Discovery vs. Proof | Gap between discovered claim and proof depth | One idea per partial-link | Flag unsupported claims as open questions |
| 4 | Independence-Verified Bridges | High-score bridges (≥1.0) | One idea per bridge exposing an A-level gap | Skip bridges whose disanalogy invalidates transfer |
| 5 | Alternative Hypotheses | Confidence-ranked hypotheses | Use only best-fit hypothesis | Discard all but best-fit |
| 6 | Density-Checked Falsification | Counter-examples + failure classes | One idea per counter-example mappable to A | Filter scope-exceeding ideas |
| 7 | Scope Limits | Applies-to / does-not-extend-to | One idea per "Breaks at" boundary | Guardrails only, no scope expansion |
| 8 | Coherence Signals | STRONG-strength convergent findings | One idea per unaddressed STRONG signal | Skip MODERATE/WEAK in STANDARD |
| 9 | Generalization Checks | Holds-at / breaks-at boundaries | One idea per in-scope break | Skip asymptotic breaks |
| 10 | Open Questions & Next Probes | HIGH-priority probes | One idea per testable change | Skip LOW-priority in STANDARD |

**Fallback (generic-fallback Node B):** E04 gate condition fails → N3 is skipped entirely. N2 produces a flat findings list. N4 cross-references from flat findings. S2_thin_B signal raised → E06 fires → N6 defixation runs.

**Drift handling (genius-drift):** N3 processes matched sections via the Map. Unmatched sections scanned generically and added as flat findings.

## 5. Output Artifacts

The skill produces 5 artifacts. Each has an explicit schema and a designated writer.

### 5.1 enhanced.md (written by N10)
The fresh enhanced Node A integrating all accepted solutions. Never overwrites original Node A.
```
Schema:
- YAML frontmatter: {session_id, node_a_source, node_b_source, node_b_type, scale, timestamp, solution_count, verification_status}
- Body: enhanced version of Node A's original content with all solutions integrated
- Trailing section: "## Enhancement Summary" listing each applied solution with its source (which B-finding, which section)
```

### 5.2 analysis-report.md (assembled by orchestrator from N2 + N4 stage outputs)
Full cross-cutting analysis combining A1 and B1 findings with cross-reference mapping.
```
Schema:
- YAML frontmatter: {session_id, node_a_source, node_b_source, node_b_type, a1_section_count, b1_section_count, xref_count}
- Section 1: Node A1 Analysis (from N2 stage output: structural decomposition, key claims, apparent gaps, strengths, weaknesses)
- Section 2: Node B1 Analysis (from N2/N3 stage output: section-tailored findings for genius B, or flat findings for generic B)
- Section 3: Cross-Reference Map (from N4 stage output: table of B-finding → A-section → relevance → potential action)
```

### 5.3 solution-catalog.md (written by N8)
All accepted solutions with pro/con/utility ratings and two-draft comparison.
```
Schema:
- YAML frontmatter: {session_id, total_ideas, accepted_count, rejected_count, conditional_count}
- Per-solution entry:
  - id: S01, S02, ...
  - source: (which B-finding or A-gap)
  - target_section: (which section of A)
  - draft_1_summary: (first independent draft)
  - draft_2_summary: (second independent draft)
  - comparison_rationale: (why draft X was selected)
  - pro: [list]
  - con: [list]
  - utility: high/medium/low
  - status: ACCEPT/ACCEPT-CONDITIONAL/REJECT
  - merged_into: (id of merged solution, or null)
```

### 5.4 self-audit.md (written by N11)
Verification report with R1-R6 checks.
```
Schema:
- YAML frontmatter: {session_id, overall_verdict: PASS|PARTIAL|FAIL, check_count, fail_count}
- Per-check entry:
  - R1: Every D_i has ≥1 solution → PASS/FAIL + detail
  - R2: Every solution targets a verifiable section → PASS/FAIL + detail
  - R3: No solution contradicts another → PASS/FAIL + detail
  - R4: No constraint from A dropped → PASS/FAIL + detail
  - R5: enhanced.md is well-formed → PASS/FAIL + detail
  - R6: All artifacts have current timestamps → PASS/FAIL + detail
```

### 5.5 graph-trace.json (assembled by orchestrator from SIGNAL_STATE + node timings)
Execution trace for replay, debugging, and post-hoc analysis.
```
Schema:
{
  "session_id": "...",
  "scale": "MINIMAL|STANDARD|DEEP",
  "node_b_type": "genius-current|genius-drift|generic-fallback|inline-substitute",
  "n3_activated": true|false,
  "executed_nodes": ["N1", "N2", ...],
  "signal_state": {(node_id, signal_field): value, ...},
  "spawn_count": 2,
  "wall_clock_seconds": 942,
  "back_edges_fired": ["E06"],
  "node_timings": {"N1": {"start": "...", "end": "...", "type": "inline"}, ...}
}
```

**Artifact assembly responsibility:**
- N8 writes `solution-catalog.md` at completion
- N10 writes `enhanced.md` at completion
- N11 writes `self-audit.md` at completion
- Orchestrator assembles `analysis-report.md` by extracting A1/B1 sections from N2's stage output and cross-reference table from N4's stage output
- Orchestrator assembles `graph-trace.json` from SIGNAL_STATE snapshot, node timings, and session metadata

## 6. Hard Gates

1. **HG-1 — Output ≠ Input.** N1 verifies enhanced.md target path ≠ Node A source path. Abort before any work.
2. **HG-2 — Self-contained runtime.** No web search, grep, glob, or user questions once N1 starts.
3. **HG-3 — Two-drafts-per-solution.** N8 never writes a solution from a single draft. Two independently-drafted solutions, compared, or the solution candidate is dropped.
4. **HG-4 — Fresh copy only.** All outputs in session directory. Never write to Node A's directory.
5. **HG-5 — Bounded iteration.** Every loop has a cap. Ideation: ≤3 passes, ≤50 ideas. Defixation: ≤1 back-edge firing per trigger. Expansion: ≤1 pass. No unbounded constructs.

## 7. Anti-Patterns

1. DO NOT remove, paraphrase, or summarize any directive from Node A or Node B — verbatim preservation
2. DO NOT introduce infinite loops — every iterate rule has a termination condition
3. DO NOT overengineer — must be realistic to implement without overcomplexity
4. DO NOT cause regression or loss of information from Node A during integration
5. DO NOT overwrite the original Node A
6. DO NOT assume a literal persistent database — stage files are markdown
7. DO NOT let the runtime ask the user questions or perform external research
8. DO NOT generate solutions outside the two-drafts-pick-best protocol
9. DO NOT treat Alternative Hypotheses as multiple competing ideas — only best-fit
10. DO NOT pick a section list at runtime by guessing — use Genius Detection + Section-Tailoring Map
11. DO NOT run all stages regardless of input complexity — use signal-driven routing to skip unnecessary work
12. DO NOT run ideation sequentially with analysis — use N2 internal parallelism and ready-set scheduling for independent nodes
13. DO NOT treat defixation as always-necessary — only trigger via back-edge signals (E06 or E16)

## 8. Ready-Set Scheduling Protocol

The orchestrator executes the graph using a ready-set scheduler:

1. **Inline-to-fixpoint:** All eligible inline nodes execute sequentially in orchestrator context (role-switched blocks). After each completes, recompute ready-set.

2. **Spawn fire:** After all inline work is exhausted, all spawn-type nodes in ready-set fire in parallel via Agent() tool calls in a single message.

3. **Ready-set recompute:** After spawns return, recalculate eligibility:
   - Required edges: all must have SIGNAL_STATE entries
   - Optional edges: never block activation
   - Forward-conditional edges: evaluate gate condition against current SIGNAL_STATE; block if condition fails
   - Gate-open edges: resolve unconditionally once source executes
   - Back-edge-only targets: NEVER activated by standard rule (explicit enqueue only)

4. **Terminal condition:** Pipeline ends when N10's output reaches the terminal edge (E21) AND N11 has completed verification.

**Early back-edge sync rule:** N7 is blocked if N2 raised S2_thin_B and N6 has not yet completed. This prevents N7 from missing the breakthrough_digest via E24.

**N8 activation in STANDARD/DEEP:** N8 requires both E09 (xref_map from N4) and E11 (ideas_digest from N5) before it can start. This ensures ideation output is incorporated into solution engineering. E22 (adversarial_digest from N7) and E13 (breakthrough_digest from N6) are optional — they enhance N8's work if available but don't block it.

**N8 activation in MINIMAL:** E11 (N5→N8) is inactive (N5 doesn't exist). N8 activates on E09 (xref_map from N4) alone. N8 engineers solutions directly from cross-reference findings without ideation input.

## 9. PRC1 Validation (pre-execution)

Before execution begins, validate the active topology:
1. **DAG check:** Excluding back-edges (E06, E16, E20), the active subgraph contains no cycles
2. **Edge resolution:** Every active edge's source and target resolve to declared active node IDs (or "input"/"output" sentinels)
3. **Signal field validity:** Every active edge's signal_field matches a declared digest name, gate literal (begins with "gate:"), or "—"
4. **Connectivity:** Active subgraph is connected with N1 as source and N10→output as sink

If any check fails: HALT with explicit error. Do not proceed.

## 10. Spawn Failure Handling

If an Agent() spawn fails:
1. Log warning with node ID and error
2. Do NOT write SIGNAL_STATE for the failed node
3. Do NOT add the node to executed_nodes
4. Let cascade naturally block dependents
5. If cascade empties ready-set before N10 completes: HALT with degraded output message
6. MINIMAL mode: if N2 spawn fails, attempt inline fallback (reduced quality, same output schema)

## 11. Wall-Clock Budgets

| Mode | Soft Target | Hard Halt |
|------|-------------|-----------|
| MINIMAL | 8 min | 15 min |
| STANDARD | 18 min | 30 min |
| DEEP | 35 min | 50 min |

Per-module hard halt: 10 minutes. Global hard halt: mode-specific hard halt.

If wall-clock target is exceeded but hard halt is not: emit advisory in graph-trace.json. Continue execution.

If hard halt is reached: write all current artifacts with PARTIAL status. Emit recovery guidance.

## 12. Development Methodology

The skill must be developed using this methodology:

### Phase 1: Design-Time Research (before skill implementation)
1. Read epiphany-analysis v2.0.0 SKILL.md in full — extract all features, hard gates, anti-patterns, Section-Tailoring Map
2. Read epiphany-graph-genius v1.1.0 SKILL.md + graph.json in full — extract graph topology patterns, signal system, ready-set scheduler, spawn budgets
3. Query dify-cognitive-kb for genius-mind traits relevant to analysis (decomposition, synthesis, verification, lateral thinking, defixation)
4. Query dify-thought-kb for GoT architectural patterns (parallel node design, signal-driven routing, aggregation topology)
5. Document all findings as design-time notes (NOT included in skill runtime)

### Phase 2: Graph Topology Design
1. Map epiphany-analysis's 6 stages to graph nodes (M-INTAKE→N1, M-ANALYZE→N2+N3+N4, M-IDEATE→N5, M-FILTER→N8 partial, M-ENGINEER→N8, M-INTEGRATE→N10+N11)
2. Identify parallelization opportunities (N2 internal A1‖B1, N5‖N7 via ready-set, artifact fan-out at pipeline end)
3. Define signal system (which nodes raise signals, which edges consume them)
4. Design back-edge topology (defixation triggers via E06/E16, expansion loop via E20)
5. Validate topology against PRC1 rules (DAG check, connectivity, edge resolution)
6. Write graph.json with complete node/edge/signal declarations
7. Write graph.schema.json for programmatic validation

### Phase 3: Module Protocol Design
1. Write N1-N12 module files (modules/N*.md) with PROTOCOL sections
2. Each module: role declaration, input schema, output schema, required output sections, step-by-step protocol, hard gate reminders
3. Inline modules: include role-switched block templates
4. Spawn modules: include Agent() spawn prompt templates (N2 includes internal A1/B1 sub-subagent fan-out template)

### Phase 4: Knowledge Base Assembly
1. Port relevant KB files from epiphany-graph-genius (section-tailoring-map.md, domain-catalog.md, debono-techniques.md, ohlsson-defixation.md, falsification-checklists.md, verification-gates.md)
2. Create new KB files specific to analysis (analysis-methodology.md for N2, input-preloading-templates.md adapted for dual-input)
3. Validate KB file references in graph.json

### Phase 5: Script Development
1. session-init.sh — session directory creation and ID generation
2. validate-graph.py — PRC1 validation script (DAG check, edge resolution, signal validity, connectivity, KB file existence, graph.schema.json validation)
3. test-runner.sh — smoke test battery

### Phase 6: Testing and Verification
1. Write smoke tests for MINIMAL, STANDARD, DEEP modes
2. Test signal-driven routing (S2_thin_B triggers E06, S7_no_alternatives triggers E16, S11_artifact_gap triggers E19)
3. Test back-edge firing and single-firing cap enforcement
4. Test E04 forward-conditional (N3 activates for genius-current/drift, skips for generic-fallback)
5. Test spawn failure handling
6. Test wall-clock budget enforcement
7. Test artifact completeness (all 5 artifacts produced, valid schemas, correct writers)
8. Test Genius Detection (genius-current, genius-drift, generic-fallback)
9. Test missing Node B fallback
10. Test N8 two-drafts-pick-best as inline role-switched block (HG-3 compliance)
11. Test N2 internal A1‖B1 sub-subagent fan-out

</task>

<constraints>
1. **Explicit graph topology required** — the skill MUST have graph.json with complete node registry, edge/channel table, and signal declarations. No "loosely defined" nodes. Every node has: id, type, exec_type, hat, context_budget, scale_gates, raises_signals, kb_files, output_file.

2. **Spawn budget is HARD** — MINIMAL ≤2, STANDARD ≤4, DEEP ≤5. The orchestrator MUST enforce this. N2 counts as 1 spawn at orchestrator level regardless of internal sub-subagent fan-out.

3. **Wall-clock targets are SOFT, hard halts are HARD** — exceeding the soft target triggers an advisory; exceeding the hard halt terminates the pipeline with PARTIAL status. Never ignore the hard halt.

4. **Power-first within budget** — when spawn budget is the bottleneck, reduce node depth (skip optional edges, use lighter context) rather than reducing artifact quality. The enhanced.md must always be the highest-quality output the budget allows.

5. **Breadth over depth when budget-tight** — prefer more parallel nodes with lighter context over fewer sequential nodes with richer context. Exploit the computer's ability to run Agent calls in parallel.

6. **No runtime KB queries** — all knowledge base content is baked into module files or KB files shipped with the skill. No MCP calls, no web fetches, no dify queries at runtime. Design-time KB queries inform the skill design but are NOT integrated into the skill.

7. **Self-contained runtime** — once N1 starts, the skill never asks the user questions and never performs external research. Deterministic with respect to (Node A, Node B).

8. **Two-drafts-pick-best is non-negotiable (HG-3)** — every solution in N8 must come from comparing two independently drafted solutions. N8 runs inline as a role-switched block; the two-draft protocol executes within that block. No exceptions.

9. **Non-overwrite guarantee (HG-1, HG-4)** — enhanced.md never overwrites Node A. All outputs in session directory.

10. **SIGNAL_STATE append-only (PRC2)** — never overwrite an existing (node_id, signal_field) key. Nodes write upon completion; subsequent nodes read but never modify.

11. **Back-edge single-firing cap (HG-5)** — each back-edge fires at most once per run. E06 (N2→N6) fires once if S2_thin_B is raised. E16 (N9→N6) fires once if S7_no_alternatives is raised and N6 hasn't run. E20 (N12→N10) fires once after expansion. Maximum back-edge firings per run: E06 + E20 = 2 in DEEP mode (E06 and E16 are mutually exclusive since E16 checks N6 not in executed_nodes, and if E06 fires N6 then E16 can't fire).

12. **Node A + Node B schemas are binding** — downstream nodes iterate content deterministically. Schemas are contracts, not hints.

13. **E04 forward-conditional enforcement** — N3 SectionTailor MUST NOT activate for generic-fallback Node B. The E04 gate condition is the sole mechanism for this; no fallback activation paths.
</constraints>

<output_format>
The skill MUST produce all 5 artifacts listed in Section 5:
1. enhanced.md — fresh enhanced Node A (PASS) or enhanced-partial.md + open-questions.md (PARTIAL/FAIL)
2. analysis-report.md — full cross-cutting analysis with A1, B1, and cross-reference map
3. solution-catalog.md — all solutions with two-draft comparisons, pro/con/utility, acceptance status
4. self-audit.md — R1-R6 verification results with PASS/PARTIAL/FAIL verdict
5. graph-trace.json — execution trace with node timings, signal state, spawn count, wall-clock, node_b_type, n3_activated

All artifacts are written to `{session_dir}/` (default: `~/docs/epiphany/graph-analysis/<session-id>/`).

Artifact schemas are defined in Section 5 and are binding — every field listed must be present.

The orchestrator emits a summary line at completion:
`epiphany-graph-analysis completed: [PASS|PARTIAL|FAIL] | scale=[MINIMAL|STANDARD|DEEP] | spawns=[N] | wall-clock=[X]s | artifacts=[enhanced.md, analysis-report.md, solution-catalog.md, self-audit.md, graph-trace.json]`
</output_format>

<verification>
## Self-Audit Checks (R1-R6, enforced by N11)

R1: Every accepted solution in solution-catalog.md has ≥1 draft comparison (HG-3 compliance)
R2: Every solution targets a verifiable section of Node A (not hallucinated targets)
R3: No two solutions contradict each other (cross-solution consistency)
R4: No explicit constraint from Node A is dropped in enhanced.md (zero information loss)
R5: enhanced.md is well-formed markdown with valid frontmatter
R6: All 5 artifacts exist with current timestamps in session directory

## Graph Topology Verification (PRC1, pre-execution)

1. DAG check (excluding back-edges E06, E16, E20)
2. Edge resolution (all sources/targets valid)
3. Signal field validity (all signal_fields match declared digests, gate literals, or "—")
4. Connectivity (N1 → N10→output connected)

## Signal Routing Tests (post-implementation)

1. S2_thin_B triggers defixation back-edge E06 (fires once)
2. E04 forward-conditional: activates N3 for genius-current/drift, deactivates for generic-fallback
3. S7_no_alternatives triggers router back-edge E16 (fires once, only if N6 not in executed_nodes)
4. S11_artifact_gap triggers expansion E19 in DEEP mode only
5. Back-edge single-firing cap enforced (max 2 firings per run in DEEP: E06 + E20, or E16 + E20)

## Mode Validation Tests

1. MINIMAL mode skips N3, N5, N6, N7, N9, N12
2. STANDARD mode skips N12
3. DEEP mode activates all 12 nodes (N3 conditional on Node B type)
4. Spawn budget per mode: ≤2/≤4/≤5
5. Wall-clock targets per mode: ≤8/≤18/≤35 min (soft)
6. N8 runs inline (not spawn) in all modes
7. N2 counts as 1 orchestrator-level spawn regardless of internal A1‖B1 fan-out
</verification>

<edge_cases>
a. **Node B missing** — Generate inline structural analysis as substitute. Emit advisory. Continue.
b. **Node B empty file** — Same as missing. Treat as generic-fallback with empty findings.
c. **Node B is not an analysis** — Genius Detection classifies as generic-fallback. E04 gate condition fails → N3 skipped. N4 works from flat findings.
d. **Node A and Node B are the same file** — HG-1 fires. Abort with error.
e. **All solutions rejected** — N8 produces empty solution catalog. N10 produces Node A unchanged with "No solutions accepted" note. Not a failure — accurate reporting.
f. **Spawn budget exhausted before N10** — HALT with degraded output. Write partial artifacts with FAIL status.
g. **S2_thin_B and S7_no_alternatives both raised** — E06 fires first (N2→N6, defixation runs). E16 checks N6 already in executed_nodes → does not fire. No double defixation.
h. **Two solutions target the same section of A** — N10 merges if compatible; logs conflict if contradictory. Contradictory solutions → keep higher-utility, log other.
i. **DEEP mode expansion produces no improvement** — N12 returns first-pass verified output unchanged with diagnostic note (O8 thin-spot gating).
j. **Node A is very large (>5000 lines)** — N1 emits complexity advisory. N2 token budget prioritizes structure over exhaustive detail. N8 limits solution count. Enhanced.md may be partial on very large inputs.
</edge_cases>

</prompt>