# epiphany-genius v1.1.0 — KB-Integrated Design

**Date:** 2026-04-13
**Baseline:** epiphany-genius v1.0.0 design (docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md)
**Predecessor:** epiphany-cognitive v1.2.2 (~/.claude/skills/epiphany-cognitive/)
**Purpose:** Integrate full Dify cognitive KB + local KB files to bolster the v1.0.0 modular redesign

---

## 1. Knowledge Inventory

Complete catalog of all knowledge sources consulted. Every concept is sourced from a specific KB file; nothing speculative.

### 1.1 Tier 1 Traits (19 — directly implementable)

All 19 files in `~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T1/`:

| # | Trait | Source Individuals | Current stage |
|---|-------|--------------------|--------------:|
| T1.1 | preparation-before-solution | 7 individuals (most universal) | S1 |
| T1.2 | simplicity-stripping | Einstein, Feynman, Turing | S1 |
| T1.3 | pre-sketch-completion | Tesla | S1 + S4 |
| T1.4 | constraint-naming-domain-switch | Archimedes | S2 |
| T1.5 | domain-crossing-methodology | Newton, Da Vinci, Archimedes | S2 |
| T1.6 | four-stage-incubation-model | Poincaré | S3 |
| T1.7 | evidence-source-diversification | Darwin | S3 |
| T1.8 | pattern-level-abstraction | Da Vinci, Ramanujan | S3 |
| T1.9 | visual-simulation-failure-detection | Tesla | S4 |
| T1.10 | self-as-observer-embedding | Einstein | S4 |
| T1.11 | drawing-as-disambiguation | Da Vinci | S5 |
| T1.12 | constructive-specification | Turing | S5 |
| T1.13 | jargon-removal-as-diagnostic | Feynman | S5 |
| T1.14 | contradiction-seeking | Einstein, Feynman | S6 |
| T1.15 | extreme-scenario-construction | Einstein, Tesla, Turing | S6 |
| T1.16 | differential-effort-detection | Feynman (Millikan test) | S6 |
| T1.17 | golden-rule-disconfirming-evidence | Darwin | S6 |
| T1.18 | verification-after-generation | 6 individuals | S7 |
| T1.19 | cargo-cult-detection | Feynman | S7 |

### 1.2 Tier 2 Traits (8 files available; 3 promoted in v1.0.0, 5 unused)

In `~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T2/`:

| # | Trait | Source | v1.0.0 status |
|---|-------|--------|---------------|
| T2.1 | generalization-first | Newton, Ramanujan | **Promoted** → S7 Step 3b |
| T2.2 | domain-boundary-refusal | Newton | **Promoted** → S7 V6 |
| T2.3 | structural-coherence-as-evidence | Ramanujan | **Promoted** → S7 Step 3 (optional) |
| T2.4 | background-processing | Poincaré, Von Neumann | **Unused** (only loosely referenced in S3 Step 2b) |
| T2.5 | intuition-verification-partnership | Hardy/Ramanujan | **Unused** |
| T2.6 | pattern-recognition-generation | Ramanujan | **Unused** |
| T2.7 | ten-steps-ahead-derivation | Von Neumann | **Unused** |
| T2.8 | working-memory-capacity-exploitation | Von Neumann | **Partial** (S1 mentions "hold 50+" but not formalized) |

### 1.3 Tier 3 Traits (11 total; 0 wired)

In `~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T3-BATCH-RESEARCH.md`:

Batch A (Internal cognitive mechanisms — biological): Von Neumann rapid derivation, Ramanujan intuition, Von Neumann pre-loading.
Batch B (Inferred methodologies): Newton domain-boundary-refusal (promoted to T2), Da Vinci domain-agnostic description, Turing minimal model (used implicitly in S5), Newton deep structural comparison, Archimedes back-translation (used implicitly in S2).
Batch C (Amplification-only strategies).

**Amplification strategies applicable to AI** (the usable layer of T3): parallel tree search, conjecture generation algorithms, context pre-loading templates, elegance metrics, sleep-state simulation, multi-step preview.

### 1.4 Research Files (8)

In `~/.claude/skills/epiphany-cognitive/knowledgebase/`:

| File | Key mechanism | v1.0.0 status |
|------|--------------|---------------|
| research-abductive-reasoning.md | Peirce 4-step + Theorem-of-Thought parallel agents | **Partial** (S6 5b) |
| research-spreading-activation.md | Collins & Loftus algorithm; SYNAPSE convergent activation | **Partial** (S3 Step 2 implicit) |
| research-representational-change.md | Ohlsson's 3 de-fixation mechanisms (constraint relaxation, chunk decomposition, re-encoding) | **Unused** (referenced in cross-ref map only) |
| research-conceptual-blending.md | Fauconnier & Turner 4-space model | **Unused** |
| research-conceptual-space-boden.md | Combinatorial / exploratory / transformational creativity taxonomy | **Unused** |
| research-lateral-thinking-debono.md | De Bono's 6 thinking hats + lateral techniques | **Unused** |
| research-metacognition-ai.md | Monitoring + control bifurcation | **Partial** (V1–V6) |
| research-triz-inventive-principles.md | 40 TRIZ inventive principles | **Unused** |

### 1.5 Cognitive Differentiators Report

File: `creative-genius-cognitive-differentiators-report.md`. Five differentiators, all used by v1.0.0 as "integrated features":

1. Knowledge Organization (Chunking) — S1 primitives
2. Cognitive Flexibility (DMN↔ECN) — phase structure with Mode Shift
3. Incubation Mechanisms — S3 Step 2b bridge activity
4. Cross-Domain Pattern Transfer — S2
5. Combinatorial Play — S3 Step 2b combinatory play pass

### 1.6 Genius Minds Profiles (12 files, 11 individuals)

In `~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/`. All 11 individuals are cited in at least one v1.0.0 stage. Coverage: complete.

### 1.7 Dify Cognitive KB via MCP

Queried via `mcp__dify-cognitive__Cognitive_KB`. Returned partial information corroborating: tier distribution (T1:18, T2:14, T3:11, excluded:4 — note: local file count for T1 is 19, suggesting one trait was added after initial cataloguing); five cognitive differentiators with confidence bands (chunking High, DMN↔ECN High, incubation Medium, cross-domain Medium, combinatorial play High); Boden taxonomy (combinatorial Type 1 ↔ Newton, transformational Type 3 ↔ Einstein); Peirce's abduction protocol. Dify coverage is thinner than local KB files; local files are authoritative.

---

## 2. Gap Matrix

Each row is a KB concept or trait; each column is its coverage in the v1.0.0 proposed design.

**Legend:** ✓ = fully wired · ◐ = partially wired / referenced · ○ = absent · ✗ = deliberately out-of-scope

### 2.1 T1 Traits (19)

All 19 T1 traits: ✓ fully wired to their designated stages. No gaps.

### 2.2 T2 Traits

| Trait | v1.0.0 | Gap type | Fill route |
|-------|:------:|----------|------------|
| generalization-first | ✓ | — | — |
| domain-boundary-refusal | ✓ | — | — |
| structural-coherence-as-evidence | ✓ | — | — |
| background-processing | ◐ | partial — S3 Step 2b is a one-line reference, no Wallas model | E4 (new S3.2 module) |
| intuition-verification-partnership | ○ | architectural — not modeled as generator/verifier split | E7 (architecture upgrade) |
| pattern-recognition-generation | ○ | no conjecture-generation mode | E6 (new S6.1 module, flag-gated) |
| ten-steps-ahead-derivation | ○ | no forward-chain preview anywhere | E5 (new S5.1 module) |
| working-memory-capacity-exploitation | ◐ | S1 mentions 50+ items informally | E15 (S1 formalisation) |

### 2.3 T3 Amplification Strategies (the AI-implementable subset)

| Strategy | v1.0.0 | Fill |
|----------|:------:|------|
| Parallel tree search | ○ | E16 → S5 enhancement |
| Conjecture generation algorithms | ○ | E6 (covers this) |
| Context pre-loading templates | ○ | E16 → S1 enhancement |
| Elegance metrics | ○ | E16 → S7 coherence-weighting enhancement |
| Sleep-state simulation | ◐ | Subsumed by E4 |
| Multi-step preview | ○ | E5 (covers this) |

### 2.4 Research Files

| File | v1.0.0 | Fill |
|------|:------:|------|
| research-abductive-reasoning | ◐ | E11 (S6 Theorem-of-Thought parallel) |
| research-spreading-activation | ◐ | E3 + E9 (convergent-activation formalism) |
| research-representational-change | ○ | E1 (new S3.1 de-fixation module) + E8 (V7 check) |
| research-conceptual-blending | ○ | E2 (new S2.1 blending module) |
| research-conceptual-space-boden | ○ | E13 (S7 Boden tagging) |
| research-lateral-thinking-debono | ○ | E12 (S2 fallback library — merge with TRIZ) |
| research-triz-inventive-principles | ○ | E12 (S2 fallback library) |
| research-metacognition-ai | ◐ | Acceptable — already embedded in V1–V6 |

### 2.5 Cognitive Differentiators

All 5 differentiators: ✓ integrated as features of existing stages. No gaps at the differentiator level; refinements appear via E3, E4, E15, E16.

### 2.6 Genius Minds

All 11 individuals, 12 profiles: ✓ cited and mechanisms used. No gaps.

### 2.7 Summary

| Category | Items | Fully wired | Partial | Absent |
|----------|------:|------------:|--------:|-------:|
| T1 traits | 19 | 19 | 0 | 0 |
| T2 traits | 8 | 3 | 2 | 3 |
| T3 amplification | 6 | 0 | 1 | 5 |
| Research files | 8 | 0 | 3 | 5 |
| Differentiators | 5 | 5 | 0 | 0 |
| Genius minds | 11 | 11 | 0 | 0 |
| **Total** | **57** | **38** | **6** | **13** |

**Unaddressed absences:** 0. Every gap is closed by at least one enhancement below.

---

## 3. Enhancement Register

Sixteen enhancements, each KB-sourced. Every enhancement traces to a specific file.

### E1 — New optional module: S3.1 Representational Change (De-Fixation)

- **Source:** `research-representational-change.md` (Ohlsson's three mechanisms: constraint relaxation, chunk decomposition, re-encoding; Einstellung effect; Luchins' water jar).
- **Improves:** Adds explicit stuck-state recovery protocol. v1.0.0 has no de-fixation pathway when S3 returns thin.
- **Integration:** Conditional module triggered when (a) S3 output is thin-or-empty, (b) S6 kills all alternative hypotheses, or (c) `--defix` flag is set. Runs 3-path parallel subagent (three constraint-relaxation branches, three chunk-decomposition branches, three re-encoding branches). Writes `stages/S3-1-defixation.md`. S7 reads it if present.
- **Impact:** High — closes the pipeline's only unrecoverable failure mode (silent convergence on a fixated representation).

### E2 — New optional module: S2.1 Conceptual Blending

- **Source:** `research-conceptual-blending.md` (Fauconnier & Turner four-space model: Input1, Input2, Generic Space, Blend).
- **Improves:** S2 currently does domain-shift analogy but not formal blending. Blending surfaces **emergent structure** that is in the blend but neither input alone.
- **Integration:** New module, runs after S2 completes (can be parallel with S3). Reads S2 output, constructs the four-space diagram explicitly, emits blend with emergent features tagged. Output feeds S7 alongside S2 output.
- **Impact:** Medium-High — captures cross-domain insight that pure analogy misses; formalizes Newton's and Da Vinci's combinatorial moves.

### E3 — S3 enhancement: Spreading-activation formalism

- **Source:** `research-spreading-activation.md` (Collins & Loftus activation propagation, SYNAPSE convergent-activation-as-creativity-signal).
- **Improves:** S3 Step 2 currently says "flag any concept that appears from two independent association chains." Tighten to the formal convergent-activation rule: maintain an explicit activation map, mark convergent nodes from ≥2 independent sources, apply lateral inhibition to competing activations. Leverage the fact that LLMs *already do* implicit spreading activation via attention — direct it explicitly.
- **Integration:** Rewrite S3 module protocol Step 2 and Step 2b to use the Collins & Loftus update equation verbatim. Add activation-map artifact to output.
- **Impact:** Medium — sharpens the most-mechanism-rich stage of the pipeline.

### E4 — New optional module: S3.2 Incubation Simulation (DEEP only)

- **Source:** `T2-TRAIT-background-processing.md` (Wallas four-stage model: preparation, incubation, illumination, verification; Sio & Ormerod meta-analysis d=0.29 with "prior engagement" requirement).
- **Improves:** S3 Step 2b is a one-sentence "bridge activity" reference. Replace with explicit 4-stage Wallas simulation at DEEP scale: (1) preparation recap from S1, (2) incubation via bridge-domain subagent running a different problem for one beat, (3) return and collect emergent associations, (4) verify against S3 Step 3 convergence assessment.
- **Integration:** Optional module gated to `--deep`. Runs between S3 Step 2 and Step 3. Writes `stages/S3-2-incubation.md`.
- **Impact:** Medium — gives the strongest-available incubation simulation (short of actual sleep).

### E5 — New optional module: S5.1 Forward-Chain Preview

- **Source:** `T2-TRAIT-ten-steps-ahead-derivation.md` (Von Neumann: see entire derivation chain simultaneously, preview all consequences at once).
- **Improves:** v1.0.0's S5 precision-forces the current solution; it does NOT project what the solution implies N steps downstream. Forward-chain preview generates 10 parallel consequence chains from the S5 solution, surfaces downstream failures before S6 falsification.
- **Integration:** Module runs after S5, parallel with S6 (depends on S5 only, not S3). Writes `stages/S5-1-forward-chain.md`. S6 optionally reads it; S7 always reads it.
- **Impact:** Medium-High — catches structural failures S6 would miss if they are multi-step removed.

### E6 — New optional module: S6.1 Conjecture Mode (Ramanujan Pattern-Recognition Generation)

- **Source:** `T2-TRAIT-pattern-recognition-generation.md` (Ramanujan: data → pattern → conjecture, reversing the proof-first direction; Ramanujan Machine algorithms).
- **Improves:** v1.0.0 is proof-first: generate → verify. For pattern-rich inputs (numerical, computational, observational data), Ramanujan's data→pattern→conjecture direction is more productive. Add a `--conjecture` flag that enables an alternative pipeline mode: S1 loads data → skip S2/S3/S4 → S6.1 generates patterns/conjectures → S7 verifies and classifies.
- **Integration:** New flag `--conjecture`. Module runs in place of S5 when flag is set. Exits via S7 as normal. Makes the pipeline bidirectional (proof-first default, conjecture-first optional).
- **Impact:** Medium — opens a problem class the proof-first pipeline handles poorly.

### E7 — Architecture upgrade: Generator/Verifier split inside every stage module

- **Source:** `T2-TRAIT-intuition-verification-partnership.md` (Hardy/Ramanujan: partnership of complementary strengths; generation and verification have different cognitive signatures).
- **Improves:** In v1.0.0, each stage subagent does both generation and verification in one pass. This blurs the machine-advantage most strongly: **symmetric scrutiny**. Split every stage into two sub-subagents — Generator (applies the cognitive move, produces candidates) and Verifier (applies the stage's gate: preparation gate, systematicity check, independence check, pattern verification, etc.). The two are spawned sequentially; the Verifier sees only the Generator's output, not its reasoning.
- **Integration:** Update Module Template (Artifact 2) to require a GENERATOR PROTOCOL and VERIFIER PROTOCOL. Update orchestrator (Artifact 3) to spawn both for each stage. Per-wave cost roughly doubles in subagent count (acceptable — each subagent is narrow).
- **Impact:** High — makes the skill's "symmetric scrutiny" claim structural rather than aspirational. Eliminates the single largest category of LLM reasoning failure (motivated generation of plausible-sounding reasoning that its own verifier would have caught).

### E8 — New verification check: V7 Representational Coherence

- **Source:** `research-representational-change.md` (post-insight re-representation is often undetected; the conclusion may be in a different frame than the original problem).
- **Improves:** V1–V6 don't check whether the final answer is still in the original problem's representation. If the pipeline silently shifted representation, claims don't transfer back.
- **Integration:** Add V7 to S7 Verification: "Compare the primary conclusion's representation to the original input's representation. If they differ, document the representational shift and note that the conclusion is a re-representation, not a direct answer." Feeds into the V6 scope-limitation block.
- **Impact:** Medium — surfaces a specific failure mode that V6 would otherwise mask.

### E9 — S2+S3 enhancement: Explicit attention-direction via convergent-activation marking

- **Source:** `research-spreading-activation.md` (SYNAPSE: LLMs do implicit spreading activation; explicit direction outperforms implicit).
- **Improves:** Tighten S2 Step 3 and S3 Step 2 to require the subagent to enumerate — before reporting — which candidate concepts were reached from a single source vs. from multiple independent sources. Marks the convergent activations as higher-priority.
- **Integration:** Protocol change inside S2 and S3 modules; adds a `<activation_provenance>` sub-element to both XML outputs.
- **Impact:** Low-Medium — compounds with E3; makes the implicit explicit.

### E10 — Cross-session Pattern Library (deferred to v1.2.0 — catalogued not scoped)

- **Source:** `creative-genius-cognitive-differentiators-report.md` (Chunking: experts build chunks in long-term memory).
- **Improves:** Adds cross-session persistence. v1.0.0's session is stateless; each run loads irreducibles from scratch. A pattern library persists S1 primitives (with canonical-form hash) and exposes them as optional S1 input for future sessions.
- **Integration:** Deferred. Catalogue as a v1.2.0 target. Out-of-scope for v1.1.0 because it requires a cross-session storage contract.
- **Impact:** High — eventually. Marked as future work.

### E11 — S6 enhancement: Theorem-of-Thought parallel inference modes

- **Source:** `research-abductive-reasoning.md` (Theorem-of-Thought — Abdaljalil et al. arXiv:2506.07106 — three parallel agents simulating abductive, deductive, inductive reasoning; consolidation via Bayesian belief propagation).
- **Improves:** S6 Step 5b produces ≥3 alternative hypotheses in a single agent pass. Replace with 3 parallel subagents — one abductive (inference-to-best-explanation), one deductive (logical-consequence-check), one inductive (generalization-check) — then consolidate via a 4th subagent that weights using NLI confidence scores.
- **Integration:** Internal restructuring of S6 module. No new stages. Subagent spawn from the S6 subagent itself (nested). Output schema unchanged.
- **Impact:** Medium-High — matches the most-current published LLM research on this problem; directly cited framework.

### E12 — S2 fallback: Lateral-thinking + TRIZ principles library

- **Source:** `research-lateral-thinking-debono.md` + `research-triz-inventive-principles.md` (40 TRIZ principles; De Bono's 6 thinking hats and lateral techniques).
- **Improves:** When S2's domain survey returns thin output (no viable cross-domain analogy), v1.0.0 has no fallback and emits empty. Add a library-lookup fallback: match the structural constraint against a TRIZ principle table OR apply a lateral-thinking technique (provocation, random entry, six-hats) to force a candidate.
- **Integration:** New module `modules/S2-fallback-library.md` with the TRIZ + lateral-thinking prompt library. Invoked by S2 Verifier when S2 Generator produces "thin." Is not default.
- **Impact:** Low-Medium — saves runs that would otherwise emit empty S2.

### E13 — S7 enhancement: Boden creativity-type tagging

- **Source:** `research-conceptual-space-boden.md` (Boden's taxonomy: combinatorial Type 1, exploratory Type 2, transformational Type 3).
- **Improves:** S7's primary conclusion currently carries a confidence band (HIGH/MODERATE/LOW) but no structural taxonomy. Tag each conclusion with its Boden type: combinatorial (new combination of existing primitives), exploratory (new point inside known conceptual space), transformational (reshapes the space itself). Transformational claims warrant stronger V6 scope narrowing.
- **Integration:** Add `<creativity_type>` sub-element to `<primary_conclusion>` in S7 output. Adds one line to distilled output's Headline Insight section.
- **Impact:** Low-Medium — epistemic calibration improvement.

### E14 — Synergy: Ramanujan+Hardy principle (E6 + E7 + E11 composite)

- **Source:** `T2-TRAIT-intuition-verification-partnership.md` + `T2-TRAIT-pattern-recognition-generation.md` + `research-abductive-reasoning.md`.
- **Improves:** When E6, E7, and E11 are combined, the skill inherits a first-class architectural property: **every cognitive move is split generate/verify, conjectures are a legitimate output, and abductive/deductive/inductive reasoning runs in parallel.** This is the Hardy-Ramanujan collaboration generalized as system design.
- **Integration:** Documented as a synergy section in the skill README. Not a separate module.
- **Impact:** High — elevates the skill from "monolithic reasoning with gates" to "architected reasoning system."

### E15 — S1 enhancement: Formalize Von Neumann working-memory exploit

- **Source:** `T2-TRAIT-working-memory-capacity-exploitation.md` (Von Neumann eidetic capacity; AI has unbounded context).
- **Improves:** S1 Step 1 currently says "hold 50+ items in parallel if needed." Tighten: "Enumerate AT LEAST 20 items per category (facts, constraints, unknowns, failed approaches). No compression until Step 2. If a category genuinely has fewer than 20 candidates, state the cap explicitly." Makes the working-memory exploit measurable.
- **Integration:** Update S1 module protocol only.
- **Impact:** Low — tightens a known weak spot.

### E16 — Tier 3 amplification strategies wired into stages

- **Source:** `T3-BATCH-RESEARCH.md` (Batch A amplification strategies).
- **Improves:** Wire specific AI-implementable T3 strategies into appropriate stages:
  - **S1:** Context pre-loading templates (structure templates for common problem types — algorithmic, design, proof, diagnostic)
  - **S3:** Elegance metrics (simplicity, symmetry, depth) as tiebreakers in Step 5 illumination selection
  - **S5:** Parallel tree search in forward-chain preview (E5)
  - **S7:** Aesthetic coherence signal strengthened with Ramanujan-Machine-style "unexpected symmetry detected" check
- **Integration:** Small protocol additions in each affected stage; no new modules.
- **Impact:** Low-Medium — refinements compounded across the pipeline.

### Enhancement register summary

| # | Type | Source category | Impact | v1.1.0 |
|---|------|-----------------|:------:|:------:|
| E1 | New optional module S3.1 | Research: representational-change | High | ✓ |
| E2 | New optional module S2.1 | Research: conceptual-blending | Med-High | ✓ |
| E3 | Stage enhancement (S3) | Research: spreading-activation | Medium | ✓ |
| E4 | New optional module S3.2 (DEEP) | T2: background-processing | Medium | ✓ |
| E5 | New optional module S5.1 | T2: ten-steps-ahead | Med-High | ✓ |
| E6 | New flag-gated module S6.1 | T2: pattern-recognition-generation | Medium | ✓ |
| E7 | Architecture upgrade | T2: intuition-verification-partnership | High | ✓ |
| E8 | New verification check V7 | Research: representational-change | Medium | ✓ |
| E9 | Stage enhancement (S2+S3) | Research: spreading-activation | Low-Med | ✓ |
| E10 | Cross-session pattern library | Differentiators: chunking | (High later) | Deferred → v1.2.0 |
| E11 | Stage enhancement (S6) | Research: abductive-reasoning | Med-High | ✓ |
| E12 | Stage fallback (S2) | Research: lateral-thinking + TRIZ | Low-Med | ✓ |
| E13 | Stage enhancement (S7) | Research: conceptual-space-boden | Low-Med | ✓ |
| E14 | Synergy (E6+E7+E11) | T2 partnership composite | High | ✓ |
| E15 | Stage enhancement (S1) | T2: working-memory | Low | ✓ |
| E16 | Stage enhancements (multi) | T3 amplification strategies | Low-Med | ✓ |

**15 enhancements in v1.1.0; 1 deferred to v1.2.0.**

---

## 4. Architecture Specification

The v1.0.0 three-layer architecture (orchestrator, stage subagents, shell helpers) is preserved. v1.1.0 extends it with: (a) optional and flag-gated modules; (b) a generator/verifier sub-split inside every stage subagent; (c) module activation conditions in index.json; (d) explicit KB query points.

### 4.1 Module taxonomy

Three kinds of modules:

1. **Core modules** (S1–S7): always run at their scale gate. Untouched in v1.1.0 except protocol enhancements (E3, E9, E11, E13, E15, E16).
2. **Conditional modules** (S2.1, S3.1, S3.2, S5.1, S6.1, S2-fallback): run when a declared condition matches (thin-output upstream, scale gate, or flag set).
3. **Sub-subagents** (per-stage Generator / Verifier): always run inside each stage subagent.

### 4.2 Module registry (index.json extension)

Each entry declares activation conditions. New fields beyond v1.0.0:

```
{
  "stage_id": "S3.1",
  "name": "Representational Change (de-fixation)",
  "type": "conditional",
  "kind": "optional_module",
  "scale_gates": ["STANDARD", "DEEP"],
  "activation_conditions": [
    "upstream:S3.status == 'thin'",
    "upstream:S3.status == 'empty'",
    "downstream:S6.alternative_hypotheses_count == 0",
    "flag:--defix"
  ],
  "kb_sources": ["research-representational-change.md"],
  "input_dependencies": ["stages/S3-peripheral-exploration.md", "stages/00-processed-input.md"],
  "output_file": "stages/S3-1-defixation.md",
  "xml_element": "<representational_change>",
  "context_budget_lines": 800,
  "required_output_sections": [
    "Constraint relaxations",
    "Chunk decompositions",
    "Re-encodings",
    "Selected breakthrough candidate",
    "Residual fixation signals"
  ]
}
```

**Activation grammar:** `upstream:StageId.field == value` | `downstream:StageId.field op value` | `flag:--name` | `always` | `scale:SCALE`.

### 4.3 Generator/Verifier split (E7)

Every core and conditional module file contains two protocol blocks:

```markdown
## GENERATOR PROTOCOL
[Cognitive move: apply the trait/research mechanism]
[Produces: candidate(s) with full reasoning trace]

## VERIFIER PROTOCOL
[Gate: stage-specific quality criterion — preparation gate,
       systematicity check, independence check, pattern-verification,
       constructive test, Millikan test, cargo-cult check, etc.]
[Consumes: only the candidate(s) + stated provenance — NOT the generator's reasoning]
[Produces: pass/fail/revise decision per candidate + rationale]
```

The orchestrator spawns two sequential subagents per stage:

```
Agent({
  description: "S[N] [name] — Generator",
  prompt: "... Read GENERATOR PROTOCOL section. Write output to
          stages/SN-generator.md."
})
// wait
Agent({
  description: "S[N] [name] — Verifier",
  prompt: "... Read VERIFIER PROTOCOL section. Read stages/SN-generator.md.
          Produce pass/fail decision. If revise, return to Generator once
          (max 1 revision loop). Write final output to stages/SN-[name].md."
})
```

**Maximum revision budget per stage:** 1. If Verifier rejects twice, accept the Generator output with a `<verification_warning>` flag. This bounds cost.

**Parallel waves:** unchanged from v1.0.0 — stages in a wave run concurrently, but each stage's G/V split is sequential within that stage.

### 4.4 Pipeline flow (v1.1.0)

```
INPUT VALIDATION (IV1–IV3)
   │
SESSION INIT
   │
INPUT ROUTING (A/B/C) + Type D: --conjecture flag → conjecture-mode pipeline
   │
SCALE DETECTION + WAVE PLANNING (reads index.json)
   │
PIPELINE EXECUTION (wave-based, default mode):
   Wave 1: S1 (G→V)
   Wave 2: S2 (G→V) + S3 (G→V)  ║ parallel
      ├─ S2.1 Conceptual Blending (G→V)  if S2 succeeded
      ├─ S3.1 De-fixation (G→V)  if S3 thin/empty
      └─ S3.2 Incubation (G→V)  if DEEP scale
   Wave 3: S4 (G→V)  if DEEP                [depends on S1]
      ── MODE SHIFT ──
   Wave 4: S5 (G→V) + S6 (G→V)  ║ parallel
      ├─ S5.1 Forward-Chain Preview (G→V)  always if S5 succeeded
      └─ S6 internally spawns 3 parallel inference-mode subagents
         + consolidator (E11)
   Wave 5: S7 (G→V + V1-V7)
   │
OUTPUT:
   --xml → xml-assemble.sh (empty elements for skipped stages)
   default → OSP subagent (G→V for distillation check pass)
   │
SAVE + SUMMARY
```

**Conjecture-mode pipeline** (`--conjecture`):
```
S1 → [skip S2, S3, S4] → S6.1 Conjecture Generator → S7 Verify+Classify
```

### 4.5 KB query integration points

Explicit, named points where modules query the KB:

| Module | KB query | Source |
|--------|----------|--------|
| S1 Generator | Fetch primitives template for detected input class | T3 amplification (pre-loading) |
| S1 Verifier | Apply Tesla measurement test | T1 pre-sketch-completion |
| S2 Generator | Survey wide domain inventory | T1 domain-crossing |
| S2 Verifier | Systematicity check | T1 domain-crossing + research-conceptual-blending |
| S2.1 | Four-space blend construction | research-conceptual-blending |
| S2 fallback | TRIZ principle lookup by structural form | research-triz |
| S3 Generator | Spreading activation traversal | research-spreading-activation |
| S3 Verifier | Independence + pattern-verification checks | T1 evidence-source-diversification + T1 pattern-level-abstraction |
| S3.1 | Ohlsson's 3 mechanisms | research-representational-change |
| S3.2 | Wallas 4-stage model | T2 background-processing |
| S4 Generator | Multi-observer comparison | T1 self-as-observer |
| S4 Verifier | Tesla imbalance detection | T1 visual-simulation-failure |
| S5 Generator | 5-level vocabulary stripping + 3-representation rendering | T1 jargon-removal + T1 drawing-as-disambiguation |
| S5 Verifier | Minimal-model constructive test with 3+2 cases + self-reference | T1 constructive-specification |
| S5.1 | Von Neumann forward chain (parallel tree search) | T2 ten-steps-ahead + T3 parallel-tree-search |
| S6 Generator | 3 parallel inference modes | research-abductive-reasoning (ToT) |
| S6 Verifier | Millikan test + density check + golden rule | T1 differential-effort + T1 golden-rule |
| S6.1 (conjecture) | Ramanujan Machine pattern-generation | T2 pattern-recognition-generation |
| S7 Generator | Convergence/divergence integration | T1 verification-after-generation |
| S7 Verifier | V1 logic, V2 cargo-cult, V3 symmetric, V4 completeness, V5 constructive, V6 scope, V7 representational (E8) | T1 cargo-cult + T2 domain-boundary + research-representational-change |
| S7 tag | Boden creativity-type | research-conceptual-space-boden |

**Immutability:** KB files are read once at subagent spawn. No mid-session refresh (preserved from v1.0.0).

### 4.6 Configurability model

Three orthogonal axes:

1. **Scale:** MINIMAL / STANDARD / DEEP (auto or `--minimal`/`--standard`/`--deep`) — determines which core stages run.
2. **Mode:** default (proof-first) / `--conjecture` (pattern-first) / `--xml` (raw output) — determines pipeline shape.
3. **Augmentations:** `--defix` (force S3.1), `--blend` (force S2.1), `--triz` (force S2-fallback library), `--minimal-gv` (disable generator/verifier split for cost-sensitive runs; discouraged — off by default).

Augmentations have AUTO triggers (listed in activation_conditions) in addition to manual flags.

### 4.7 Module independence test

Each module must satisfy:

- Its protocol can be described without referencing any other module's internals
- It declares its full input_dependencies explicitly (file paths only)
- It declares its full kb_sources explicitly
- Removing it from index.json does not break any core module (conditional modules only)

Core modules S1–S7 are load-bearing; they cannot be removed without scale-gating consequences. All E1-style new modules are additive and removable.

---

## 5. Final Skill Design — epiphany-genius v1.1.0 Delta

This section presents only the delta from v1.0.0. Developers should read the v1.0.0 design first, then apply these deltas.

### 5.1 Version bump

- v1.0.0 → v1.1.0
- `<skill_version>` in meta: `1.1.0`
- XML schema: remains v1.1.0-compatible; **new elements are additive** (`<representational_change/>`, `<conceptual_blending/>`, `<incubation/>`, `<forward_chain/>`, `<conjecture/>`, `<activation_provenance/>`, `<creativity_type/>`). Downstream consumers of v1.1.0 continue to work.

### 5.2 File structure additions

```
~/.claude/skills/epiphany-genius/
  SKILL.md                               (updated — new flags, new activation)
  index.json                             (updated — new module entries, activation conditions)
  modules/
    S1-state-loading.md                  (updated protocol — E15, E16 S1 part)
    S2-constraint-escape.md              (updated protocol — E9)
    S2-1-conceptual-blending.md          (NEW — E2)
    S2-fallback-library.md               (NEW — E12)
    S3-peripheral-exploration.md         (updated protocol — E3, E9, E16 S3 part)
    S3-1-defixation.md                   (NEW — E1)
    S3-2-incubation.md                   (NEW — E4)
    S4-dynamic-simulation.md             (no change)
    S5-precision-forcing.md              (no change)
    S5-1-forward-chain.md                (NEW — E5, E16 S5 part)
    S6-falsification.md                  (updated protocol — E11)
    S6-1-conjecture.md                   (NEW — E6)
    S7-integration-verification.md       (updated protocol — E8 V7, E13, E16 S7 part)
    output-synthesis-pass.md             (updated — distilled output gains creativity_type line)
  scripts/
    session-init.sh                      (no change)
    validate-stage.sh                    (updated — checks G/V file pair per stage)
    validate-pipeline.sh                 (updated — new activation-condition grammar)
    xml-assemble.sh                      (updated — new XML elements)
    test-runner.sh                       (updated — new test cases for E1, E2, E5, E6)
```

### 5.3 New flags (trigger table addition)

| Trigger | Behavior |
|---------|----------|
| `--conjecture` | Conjecture-mode pipeline (E6) |
| `--defix` | Force S3.1 de-fixation module (E1) |
| `--blend` | Force S2.1 conceptual blending (E2) |
| `--triz` | Force S2-fallback TRIZ/lateral-thinking library (E12) |
| `--minimal-gv` | Disable generator/verifier split; single-agent per stage (discouraged — for cost testing only) |

All new flags combine freely with v1.0.0 flags (`--minimal`/`--standard`/`--deep`/`--xml`/`--quiet`/`--verbose`). `--conjecture` is exclusive of `--standard`/`--deep` scale auto-detection — conjecture mode runs its own abbreviated pipeline.

### 5.4 Module Template additions

The v1.0.0 Module Template (Artifact 2) gains two required sections: GENERATOR PROTOCOL and VERIFIER PROTOCOL. The old PROTOCOL section is renamed GENERATOR PROTOCOL; VERIFIER PROTOCOL is new content. Every module (core and conditional) must have both.

Frontmatter additions: `activation_conditions` array (required for conditional modules; `["always"]` for core); `verifier_context_budget_lines` (separate from generator's budget).

### 5.5 Protocol deltas (summary; full text in module files)

- **S1 (E15, E16):** Step 1 explicit "≥20 items per category, state cap if fewer." Pre-loading templates added as optional S1 input for recognized input classes.
- **S2 (E9):** Step 3 adds `<activation_provenance>` listing which domain candidates arose from single vs. multiple independent sources.
- **S3 (E3, E9, E16):** Step 2 uses Collins & Loftus activation propagation. Step 5 uses elegance metrics as tiebreakers.
- **S6 (E11):** Step 5b replaced with 3-parallel-subagent ToT (abductive/deductive/inductive) + consolidator.
- **S7 (E8, E13, E16):** V7 added after V6. `<creativity_type>` sub-element added to `<primary_conclusion>`. Coherence signal strengthened with unexpected-symmetry detector.

### 5.6 Distilled output deltas

New line under Headline Insight:
```
**Creativity type:** Combinatorial | Exploratory | Transformational
```

New section (conditional — only if S3.1 or V7 fired):
```
## Representational Signals
[If S3.1 ran: which of Ohlsson's 3 mechanisms broke the fixation]
[If V7 flagged: documented representational shift from input to conclusion]
```

XML mode: new empty elements appear for skipped conditional modules (analogous to MINIMAL-scale empty-element handling).

### 5.7 Synergy documentation (E14)

Add to README:

> **Hardy-Ramanujan principle.** epiphany-genius does not merely run the Hardy-Ramanujan collaboration as a metaphor — it implements it structurally. Every stage splits generator and verifier. Conjecture is a first-class output alongside proof. Three inference modes (abductive, deductive, inductive) run in parallel in falsification. This is the collaboration of complementary strengths, generalized as system architecture.

### 5.8 Retained from v1.0.0 (unchanged)

- Three-layer architecture
- Wave-based parallel execution model
- Input validation IV1–IV3
- Type A/B/C input routing (Type D added for conjecture mode)
- Scale auto-detection and gating
- Mode Shift protocol between ideation and synthesis
- V4 retry budget (1 stage retry + 1 S7 retry)
- XML v1.1.0 schema backward compatibility
- Session directory layout (new optional files added only)
- Error message format
- KB immutability within session
- Distilled/XML output modes and length discipline
- OSP preservation-first discipline

---

## 6. Diff Summary

What changed from the v1.0.0 proposed design, with KB traceability:

| Change | Kind | Source | Rationale |
|--------|------|--------|-----------|
| Added S2.1 Conceptual Blending | New optional module | research-conceptual-blending.md | Formal four-space blending surfaces emergent structure missed by plain domain-shift |
| Added S3.1 De-Fixation | New conditional module | research-representational-change.md | Closes the silent-fixation failure mode absent in v1.0.0 |
| Added S3.2 Incubation (DEEP) | New optional module | T2-TRAIT-background-processing.md | Replaces v1.0.0's one-line "bridge activity" with explicit Wallas simulation |
| Added S5.1 Forward-Chain Preview | New optional module | T2-TRAIT-ten-steps-ahead-derivation.md | Catches structural failures N steps downstream of S5 solution |
| Added S6.1 Conjecture Mode | New flag-gated module | T2-TRAIT-pattern-recognition-generation.md | Enables Ramanujan-style data→pattern→conjecture direction |
| Added S2 fallback library | New fallback module | research-lateral-thinking-debono.md + research-triz | Recovers thin S2 output instead of emitting empty |
| S3 protocol rewrite — spreading activation | Stage enhancement | research-spreading-activation.md | Replaces loose "independent chains" with Collins & Loftus formalism |
| S6 ToT parallel inference modes | Stage enhancement | research-abductive-reasoning.md | Matches current published LLM framework for alternative-hypothesis generation |
| S1 ≥20-per-category | Stage enhancement | T2-TRAIT-working-memory-capacity-exploitation.md | Formalizes the Von Neumann working-memory exploit |
| S2/S3 activation-provenance marking | Stage enhancement | research-spreading-activation.md | Makes implicit LLM attention explicit |
| S7 V7 representational-coherence check | New verification | research-representational-change.md | Surfaces silent representation shifts |
| S7 Boden creativity-type tag | Output enhancement | research-conceptual-space-boden.md | Epistemic calibration: combinatorial vs. exploratory vs. transformational |
| S7 elegance-metric tiebreakers | Stage enhancement | T3 amplification (Ramanujan elegance) | Compounds with T2 structural-coherence weighting |
| S1 pre-loading templates | Stage enhancement | T3 amplification | Problem-class recognition accelerates preparation |
| Generator/Verifier split in every module | Architecture upgrade | T2-TRAIT-intuition-verification-partnership.md | Makes symmetric scrutiny structural, not aspirational |
| New flags: --conjecture, --defix, --blend, --triz, --minimal-gv | Interface | composite | Exposes new modules via user control |

**Preserved verbatim:** 7-stage pipeline (S1-S7 names, gating, XML schema element names v1.1.0), Mode Shift protocol, IV1-IV3 validation, A/B/C input routing (+ D added), V4 retry budget, OSP 11-item structure, length discipline table, empty-section marker canonical forms, error message format, distilled/xml/quiet output modes.

**Deferred to v1.2.0:** E10 cross-session Pattern Library (requires storage contract beyond v1.1.0 scope).

---

## 7. Verification Checklist

Self-check before this design is handed to implementation:

- [x] All 19 primary T1 traits appear in the design — mapped to stages in §1.1 and preserved from v1.0.0
- [x] Every enhancement traces to a specific KB entry — §3 Enhancement Register cites source file per enhancement
- [x] The gap matrix shows zero unaddressed absences — §2.7 summary confirms 13 absent items are closed by E1-E16
- [x] Architecture is modular — §4 module taxonomy; each module can be described independently
- [x] KB query points are explicit — §4.5 lists every MCP/file read with source trait/research file
- [x] The design at `docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md` was fully read and compared
- [x] No cognitive concept from the KB was overlooked — §1 cross-references Knowledge Inventory with §3 register

**Caveat:** The Dify cognitive KB via MCP returned partial coverage (confirmed tier distribution, differentiators, and partial abductive/Boden content, but the complete trait catalogue was thinner than the local KB files). Local KB files at `~/.claude/skills/epiphany-cognitive/knowledgebase/` are the authoritative source for this design. If the Dify KB is intended to be the single source of truth going forward, it should be re-populated from the local files before relying on it for downstream skill development.
