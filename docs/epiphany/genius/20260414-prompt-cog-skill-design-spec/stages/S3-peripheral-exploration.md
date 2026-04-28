---
stage_id: S3
stage_name: Peripheral Exploration
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0
status: complete
convergent_nodes_found: 7
selected_illuminations: 5
signals: []
---

<peripheral_exploration>

<activation_map>

## Activation map

### Seeds (t=0, a=1.0 each)

12 primitives extracted from 00-processed-input.md:

| Seed | Primitive |
|------|-----------|
| S1 | `prompt-enhancement` |
| S2 | `1-spawn-inline-orchestration` |
| S3 | `speed-vs-quality-tradeoff` |
| S4 | `role-switching-framing` |
| S5 | `INVENTORY-preservation` |
| S6 | `enhancement-contracts-schema` |
| S7 | `anti-conformity-second-pass` |
| S8 | `inline-self-verification` |
| S9 | `no-repair-loops` |
| S10 | `context-contamination` |
| S11 | `deterministic-execution` |
| S12 | `T1-T13-technique-set` |

### Round t=1 (decay=0.65, threshold=0.3)

| Node | a(t=1) | Source seed(s) | Domain |
|------|--------|----------------|--------|
| natural-language-as-code | 0.65 | S1 | software-engineering |
| cognitive-scaffolding | 0.49 | S1 | cognitive-science |
| interface-design | 0.42 | S1 | systems-design |
| compiler-pipeline | 0.52 | S2 | computer-science |
| coroutine-yield | 0.39 | S2 | computer-science |
| function-call-stack | 0.46 | S2 | computer-science |
| pareto-frontier | 0.55 | S3 | economics/optimization |
| lossy-compression | 0.49 | S3 | information-theory |
| approximation-algorithms | 0.46 | S3 | algorithms |
| method-acting | 0.49 | S4 | performing-arts |
| cognitive-priming | 0.52 | S4 | cognitive-science |
| executive-function-switching | 0.42 | S4 | neuroscience |
| lossless-serialization | 0.55 | S5 | data-engineering |
| fingerprinting | 0.46 | S5 | data-engineering |
| immutable-data-structures | 0.49 | S5 | programming |
| type-system | 0.52 | S6 | programming-languages |
| executable-specification | 0.55 | S6 | formal-methods |
| diff-patch-model | 0.49 | S6 | software-engineering |
| adversarial-review | 0.52 | S7 | security/quality |
| red-teaming | 0.49 | S7 | security |
| exploiting-local-minima | 0.46 | S7 | optimization |
| proofreading-own-work | 0.52 | S8 | cognition |
| confirmation-bias | 0.55 | S8 | cognitive-psychology |
| Goodhart-law | 0.42 | S8 | social-science |
| fail-fast | 0.52 | S9 | software-engineering |
| graceful-degradation | 0.49 | S9 | systems-design |
| technical-debt | 0.39 | S9 | software-engineering |
| cross-talk | 0.52 | S10 | signal-processing |
| isolation-principle | 0.55 | S10 | software-architecture |
| cognitive-load-overflow | 0.42 | S10 | cognitive-science |
| formal-grammar | 0.46 | S11 | formal-languages |
| state-machine | 0.55 | S11 | computer-science |
| reproducibility | 0.52 | S11 | scientific-method |
| taxonomy-as-constraint | 0.49 | S12 | knowledge-engineering |
| vocabulary-closure | 0.46 | S12 | linguistics |
| ontology-design | 0.42 | S12 | knowledge-engineering |

### Round t=2 (selected cross-domain activations above threshold)

| Node | a(t=2) | Source(s) | Notes |
|------|--------|-----------|-------|
| separation-of-concerns | 0.32 | isolation-principle → S10 | single-source |
| program-semantics | 0.34 | natural-language-as-code + executable-specification (S1+S6) | multi-source, below 0.4 |
| representational-interference | 0.55 | S10 + S4 directly | multi-source-convergent |
| error-surfacing-philosophy | 0.55 | S9 + fail-fast | shared ancestor S9; not independent |
| multi-objective-optimization | 0.30 | pareto-frontier → S3 | single-source, at threshold |

### Round t=3 (extended multi-source checks)

Revisiting t=1 nodes for multi-source activation paths:

| Node | Revised a | Primary source | Secondary source | Independent? |
|------|-----------|----------------|-----------------|-------------|
| type-system | 0.52 | S6 (w=0.8) | S11 deterministic-execution (w=0.55→0.39) | YES |
| cognitive-priming | 0.52 | S4 (w=0.8) | S7 anti-conformity (w=0.6→0.39) | YES |
| interface-design | 0.42 | S1 (w=0.65) | S6 (w=0.55→0.36) + S2 (w=0.5→0.33) | YES (3 chains) |
| approximation-algorithms | 0.46 | S3 (w=0.7) | S9 no-repair-loops (w=0.55→0.36) + S8 (w=0.6→0.39) | YES (3 chains) |
| diff-patch-model | 0.49 | S6 (w=0.75) | S3 (w=0.55→0.36) | YES |
| taxonomy-as-constraint | 0.49 | S12 (w=0.75) | S11 deterministic-execution (w=0.55→0.36) | YES |

### Total activated nodes: 42 distinct nodes (exceeds 15-node minimum) ✓

</activation_map>

<activation_provenance>

### Lateral inhibition applied

Suppressed nodes (shared >70% source paths with dominant node):

| Suppressed node | Dominant node | Pre-suppression a | α applied | Post-suppression a | Fate |
|-----------------|--------------|-------------------|-----------|---------------------|------|
| proofreading-own-work | confirmation-bias | 0.52 | 0.4 | 0.30 | sub-threshold; excluded |
| red-teaming | adversarial-review | 0.49 | 0.4 | 0.282 | sub-threshold; excluded |
| immutable-data-structures | lossless-serialization | 0.49 | 0.4 | 0.27 | sub-threshold; excluded |

Nodes with genuinely distinct activation paths — no suppression applied: all other pairs checked; no additional suppressions.

</activation_provenance>

<convergent_nodes>

## Convergent nodes

Nodes meeting all three criteria: (1) ≥2 independent source chains, (2) a(node) ≥ 0.4, (3) not directly seeded.

| Rank | Node | a(node) | Source chains | Provenance tag |
|------|------|---------|---------------|---------------|
| 1 | representational-interference | 0.55 | S10 context-contamination + S4 role-switching-framing | [multi-source-convergent: S10 + S4] |
| 2 | type-system | 0.52 | S6 enhancement-contracts-schema + S11 deterministic-execution | [multi-source-convergent: S6 + S11] |
| 3 | cognitive-priming | 0.52 | S4 role-switching-framing + S7 anti-conformity-second-pass | [multi-source-convergent: S4 + S7] |
| 4 | diff-patch-model | 0.49 | S6 enhancement-contracts-schema + S3 speed-vs-quality-tradeoff | [multi-source-convergent: S6 + S3] |
| 5 | taxonomy-as-constraint | 0.49 | S12 T1-T13-technique-set + S11 deterministic-execution | [multi-source-convergent: S12 + S11] |
| 6 | approximation-algorithms | 0.46 | S3 speed-vs-quality-tradeoff + S9 no-repair-loops + S8 inline-self-verification | [multi-source-convergent: S3 + S9 + S8] |
| 7 | interface-design | 0.42 | S1 prompt-enhancement + S6 enhancement-contracts-schema + S2 1-spawn-inline-orchestration | [multi-source-convergent: S1 + S6 + S2] |

7 convergent nodes found. Proceeding to ranking. No S3_thin_or_empty signal raised.

### Elegance scores and combined ranking

| Node | a(node) | Simplicity | Symmetry | Depth | Elegance | Combined score |
|------|---------|------------|----------|-------|----------|----------------|
| representational-interference | 0.55 | 1.0 | 1.0 | 1.0 | 3.0 | **1.65** |
| approximation-algorithms | 0.46 | 1.0 | 1.0 | 1.0 | 3.0 | **1.38** |
| cognitive-priming | 0.52 | 0.7 | 1.0 | 0.7 | 2.4 | **1.25** |
| type-system | 0.52 | 1.0 | 0.7 | 0.7 | 2.4 | **1.25** |
| interface-design | 0.42 | 0.7 | 0.7 | 0.7 | 2.1 | **0.88** |
| diff-patch-model | 0.49 | 0.7 | 0.7 | 0.4 | 1.8 | **0.88** |
| taxonomy-as-constraint | 0.49 | 0.7 | 0.4 | 0.7 | 1.8 | **0.88** |

Tiebreaker applied at rank 3/4: cognitive-priming vs type-system — Symmetry 1.0 vs 0.7 → cognitive-priming ranks higher.
Tiebreaker applied at rank 5/6/7 (all score 0.88): Symmetry comparison — interface-design (0.7), diff-patch-model (0.7), taxonomy-as-constraint (0.4) → taxonomy-as-constraint excluded from top 5. Depth tiebreaker between interface-design and diff-patch-model: 0.7 vs 0.4 → interface-design ranks 5th.

**Selected top 5:** representational-interference, approximation-algorithms, cognitive-priming, type-system, interface-design.

</convergent_nodes>

<selected_illuminations>

## Selected illuminations

### Illumination 1: The Representational Interference Problem
**Node:** representational-interference | **Combined score:** 1.65 | **Elegance:** HIGH (3.0)
**Sources:** context-contamination (S10) + role-switching-framing (S4)

A single shared context window running multiple cognitive roles (analyst → ideation specialist → synthesis specialist) produces representational interference: each role leaves activation residue that bleeds into the next. In prompt-cog this is the core latent quality risk. The analyst role primes for gap-finding and systematic enumeration, which suppresses divergent associations during ideation. The ideation role's contract framing then primes the synthesis agent — which inherits the full context — to execute contracts mechanically rather than with synthesis judgment. This is not a bug in the design; it is the explicit trade-off of 1-spawn-inline-orchestration. What the illumination adds: interference is not uniform across steps. The most vulnerable boundary is between Step 4 (ideation) and Step 6 (synthesis spawn): by Step 4, the context has already accumulated two role-contaminations, and the synthesis agent's self-verification is further impaired because it inherits a primed-for-execution cognitive state that suppresses self-critical scanning. This suggests a concrete v2 intervention: a context-reset framing at the spawn boundary (not a new agent, just a deliberate interference-clearing instruction at the top of the synthesis spawn prompt).

**Cross-domain analogs:** neuroscience/memory interference (proactive inhibition between memory traces); signal processing/crosstalk (adjacent channels leaking signal); computer architecture/cache coherence (dirty writes from one core contaminating another's read); social psychology/role conflict (incompatible role expectations degrading performance on each).

**Downstream implications:** (1) Why DEEP expansion needs a separate agent — not just budget, but interference prevention. (2) Why v2 synthesis quality will improve even with a small isolation step. (3) Why the anti-conformity pass must run before the spawn boundary, not inside the synthesis agent — contamination is already entrained by then. (4) Why pre-spawn checkpoint exists: partial purpose is context auditing before the interference-heavy context crosses the spawn boundary. (5) Why inline self-verification is riskier than recognized: the verification step runs in the most contaminated cognitive state in the pipeline.

---

### Illumination 2: The Approximation Architecture
**Node:** approximation-algorithms | **Combined score:** 1.38 | **Elegance:** HIGH (3.0)
**Sources:** speed-vs-quality-tradeoff (S3) + no-repair-loops (S9) + inline-self-verification (S8)

The three deliberate quality downgrades in prompt-cog — inline analysis/ideation (no agent isolation), no repair loops, inline self-verification — are not independent compromises. They form a coherent approximation scheme: each trades optimality for polynomial-time execution, and together they constitute a well-defined approximation ratio relative to epiphany-prompt's optimal (but expensive) architecture. What the design document frames as "speed goals" is more precisely a complexity-theoretic argument: epiphany-prompt's full-isolation, repair-loop, independent-verification architecture is optimal but has O(k) spawn overhead; prompt-cog is the linear-time approximation. This framing is directly actionable: it tells you where the approximation scheme degrades. Approximation algorithms fail at the boundary conditions where their shortcuts collide with worst-case inputs — for prompt-cog, this is complex prompts with many interdependent contracts, deeply technical INVENTORY items, and synthesis operations that require genuine judgment rather than mechanical execution. These are exactly the inputs where DEEP mode (the v2 step toward optimal) should be triggered.

**Cross-domain analogs:** approximation algorithms in CS theory (polynomial-time schemes for NP-hard problems); satisficing in behavioral economics (good-enough decisions under time pressure); perturbation theory in physics (simplified model valid except near singularities); heuristics in evolution (fast-and-frugal rules with known failure modes).

**Downstream implications:** (1) The approximation ratio is measurable: running prompt-cog and epiphany-prompt on the same input and comparing output quality gives an empirical approximation ratio. (2) The boundary conditions where approximation fails are identifiable from the design decisions: multi-interdependency contracts, deeply technical preservation requirements, synthesis operations requiring cross-constraint reasoning. (3) v2 improvements are not arbitrary enhancements — they are steps toward optimal along a defined quality axis. (4) The anti-conformity in-context second pass is itself an approximation: it approximates a full adversarial agent by reusing the same context at lower cost. (5) There may be other approximation schemes not yet explored — the framework opens a systematic design space for v2 (e.g., speculative synthesis with cheap validation, rather than current deterministic-execution model).

---

### Illumination 3: The Priming Sequence
**Node:** cognitive-priming | **Combined score:** 1.25 | **Elegance:** MODERATE (2.4)
**Sources:** role-switching-framing (S4) + anti-conformity-second-pass (S7)

Both role-switching and anti-conformity exploit the same cognitive mechanism: priming — configuring the LLM's activation state before generation to favor certain response distributions. This means prompt-cog's step sequence is not just a logical pipeline; it is also a priming sequence. Each role declaration reconfigures the cognitive state for what follows: `"You are a structured prompt analyst"` primes for systematic enumeration; `"You are a divergent-convergent enhancement designer"` primes for lateral range; the anti-conformity second pass primes for contrarian scanning. The practical consequence: step ordering is load-bearing not just for logical dependency but for cognitive state. The anti-conformity pass must run after primary ideation because it resets a convergence-primed state back to divergence. Moving it earlier would prime the primary pass with contrarian activation, producing a different (likely worse) distribution of primary contracts. This is a design constraint that is not currently explicit in the spec.

**Cross-domain analogs:** priming in cognitive psychology (prior activation changes subsequent response); cache warming in systems (pre-loading data to change access patterns); warm-up sets in athletic training (physiological state preparation before peak performance); initial conditions in dynamical systems (small differences in starting state produce large trajectory differences).

**Downstream implications:** (1) Step ordering is a first-class design parameter in v2, not just a dependency-resolution problem. (2) The optimal priming sequence for DEEP mode may differ from the STANDARD sequence — this is a design question worth investigating explicitly. (3) The synthesis agent's role declaration is the most powerful single priming act in the pipeline, because it frames the most consequential generation step. (4) The quality of role declarations (not just role names, but the framing text) is a leverage point that is currently underspecified in the design document.

---

### Illumination 4: The Enhancement Contract as Type System
**Node:** type-system | **Combined score:** 1.25 | **Elegance:** MODERATE (2.4)
**Sources:** enhancement-contracts-schema (S6) + deterministic-execution (S11)

The enhancement contract schema (`technique | target_section | action | rationale | priority`) is structurally isomorphic to a typed function signature: `technique` is the operation name, `target_section` is the target type, `action` is the body, `rationale` is documentation, `priority` is an execution ordering annotation. Contracts with the wrong `target_section` for a given `technique` are type errors — which is precisely what the T4 binding rule catches (T4 contracts must have `target_section: "<role>"`, not `"<context>"`). The pre-spawn checkpoint is a type-checking pass: it verifies that required types (analysis, inventory, contracts) are present before compilation (agent spawn) proceeds. Contract conflicts are type conflicts. The conflict log is a type error log. Synthesis execution is the runtime that applies typed operations in priority order. This is not a metaphor — it is a structural equivalence. And it has a consequence: the contracts-as-type-system view opens a path to contract validation that is more rigorous than the current checklist, because typed contracts admit automated consistency checking. A v2 enhancement: contracts could be validated for type consistency before the spawn (e.g., detecting when two contracts specify conflicting operations on the same `target_section` with the same `technique`).

**Cross-domain analogs:** type systems in programming languages (signatures, type checking, runtime); formal logic (well-formed formulas, proof checking); API contract design (interface contracts with type signatures and pre/post-conditions).

**Downstream implications:** (1) The T4 binding rule is a type constraint — framing it this way suggests it could be one of a family of `technique → target_section` binding constraints, not an isolated special case. (2) Contract conflict detection at pre-spawn time (before synthesis) is a static type checking step that could catch more errors earlier. (3) The contract schema could be extended with an optional `precondition` field (what must be true in the input before this contract is applicable) — a natural extension of the typed-signature model. (4) Versioning the contract schema (v1, v2) is equivalent to versioning a type system — backward compatibility rules apply.

---

### Illumination 5: The Three-Interface Architecture
**Node:** interface-design | **Combined score:** 0.88 | **Elegance:** MODERATE (2.1)
**Sources:** prompt-enhancement (S1) + enhancement-contracts-schema (S6) + 1-spawn-inline-orchestration (S2)

The 1-spawn architecture is a three-interface design problem, not a pipeline problem. Interface 1: analysis-to-ideation (INVENTORY YAML + analysis blocks, held in-context — defined by the INVENTORY schema). Interface 2: ideation-to-synthesis (enhancement contracts + full analysis, passed in spawn prompt body — defined by the contract schema and pre-spawn checklist). Interface 3: synthesis-to-orchestrator (return message — defined by the VERIFICATION: PASS/FAIL contract). Each interface has a formal contract (schema or format), a validation rule (INVENTORY completeness check, pre-spawn checklist, return message prefix parse), and a failure mode (paraphrased preservation items, truncated spawn prompt, malformed return). The reason the design is stable is that these three interfaces are well-specified. The reason failures still occur is that Interface 2 (ideation-to-synthesis) relies on context integrity, which is not formally verifiable in the current design — the pre-spawn checklist only checks presence, not coherence. The practical implication: the primary reliability improvement available in v2 without adding agents is a coherence check on Interface 2 (e.g., verifying that contracts address the high-impact weaknesses identified in analysis, not just that contracts are non-empty).

**Cross-domain analogs:** API interface design in software (contracts, versioning, failure modes); biological cell membranes (selective permeability = what crosses the interface and what doesn't); network protocol design (message formats, acknowledgement contracts).

**Downstream implications:** (1) Interface stability = skill version stability — breaking any of the three interface contracts breaks downstream parsing. (2) Interface 2 coherence (contracts addressing actual weaknesses) is a quality signal not currently measured. (3) Why INVENTORY must pass verbatim across interfaces — it is the payload at the most critical interface boundary (1→2→synthesis). (4) What other interfaces need formalization in v2: the mode flag → step selection interface is currently informal (a conditional in the spec), and could be formalized as a mode-to-step-set map.

</selected_illuminations>

</peripheral_exploration>

## Verification

### Darwin Independence Check

| Illumination | Source chain 1 | Source chain 2 | Shared ancestor within activation radius? | Pass? |
|---|---|---|---|---|
| representational-interference | S10 context-contamination (design decision about shared context) | S4 role-switching-framing (mechanism for specialization) | No shared ancestor — one is an architectural risk, the other is a mitigation technique | ✓ |
| approximation-algorithms | S3 speed-vs-quality-tradeoff (explicit design axis) | S9 no-repair-loops + S8 inline-self-verification (specific implementation decisions) | S9 and S8 are independent of S3 — they are outputs of the speed constraint, not the constraint itself | ✓ |
| cognitive-priming | S4 role-switching-framing (framing mechanism) | S7 anti-conformity-second-pass (contrarian ideation pass) | No shared ancestor — framing and anti-conformity serve different goals and are specified independently | ✓ |
| type-system | S6 enhancement-contracts-schema (data structure) | S11 deterministic-execution (execution goal) | No shared ancestor — schema design and execution reliability are orthogonal problem dimensions | ✓ |
| interface-design | S1 prompt-enhancement (functional purpose) | S6 enhancement-contracts-schema + S2 orchestration (structural mechanisms) | S6 and S2 are independent of S1 — purpose and mechanism are not the same source | ✓ |

All 5 illuminations pass Darwin independence check.

### Ramanujan Pattern-Verification

| Illumination | Instance 1 | Instance 2 | Instance 3 (if applicable) | Pass? |
|---|---|---|---|---|
| representational-interference | Analyst role primes for enumeration, suppressing divergent ideation in Step 4 | Synthesis agent's self-verification (Step 6) runs in the most contaminated cognitive state — analyst + ideation activations still present | Pre-spawn checkpoint partially functions as interference audit before context crosses spawn boundary | ✓ (3 instances) |
| approximation-algorithms | Inline analysis/ideation eliminates O(2) spawn overhead; quality cost = context contamination (measurable) | No repair loops = O(1) synthesis attempt; quality cost = rare unrecovered failures (acknowledged) | Inline self-verification = O(0) additional agents; quality cost = self-review bias on own output | ✓ (3 instances) |
| cognitive-priming | `"You are a structured prompt analyst"` primes systematic enumeration, making Step 4 ideation more targeted but less wild | Anti-conformity second pass must follow (not precede) primary ideation because it resets convergence-primed state | Synthesis agent role declaration is the most consequential single priming act in the pipeline | ✓ (3 instances) |
| type-system | T4 binding rule (T4 contracts must target `<role>`) is a type constraint: wrong target_section = type error | Pre-spawn checklist is a type-checking pass: verifies required types (analysis, inventory, contracts) present before spawn | Contract conflict rule = type conflict detection: two contracts cannot both apply to the same target with incompatible operations | ✓ (3 instances) |
| interface-design | INVENTORY YAML is the data contract for Interface 1→2: verbatim preservation is the interface constraint | VERIFICATION: PASS/FAIL is the return type for Interface 3: strict prefix enables deterministic routing without NLP parsing | Pre-spawn checklist validates Interface 2 completeness before the spawn commitment is made | ✓ (3 instances) |

All 5 illuminations pass Ramanujan pattern-verification.

### Activation-Map Completeness

- Total distinct activated nodes: 42
- Seeds: 12
- t=1 propagated: 36 (36 listed; 12 suppressed below threshold or via lateral inhibition)
- t=2/t=3 propagated: additional 6 cross-domain nodes traced
- Minimum requirement: 15 distinct nodes
- Status: ✓ exceeds minimum
