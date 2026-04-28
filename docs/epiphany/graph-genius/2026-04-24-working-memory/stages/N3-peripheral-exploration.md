# N3 — Peripheral Exploration (SPREADING-ACTIVATION)
## Executed: 2026-04-24 | Scale: STANDARD | Hat: spreading-activation+elegance-rubric

<peripheral_exploration>

<activation_map>

## Activation map

**Seeds (a=1.0 each):** working_memory, cognitive_bottlenecks, capacity_limits, learning_strategies, human_learners

**Step 1 — Direct neighbors (decay=0.7, w=1.0–1.2 for direct components):**

| Node | a(t=1) | Activated by |
|---|---|---|
| phonological_loop | 0.85 | working_memory (w=1.2 direct component) |
| central_executive | 0.85 | working_memory (w=1.2), cognitive_bottlenecks (w=1.0) |
| visuospatial_sketchpad | 0.70 | working_memory |
| attention | 0.70 | working_memory, cognitive_bottlenecks (max) |
| cognitive_load | 0.70 | cognitive_bottlenecks, learning_strategies (max) |
| long_term_memory | 0.70 | working_memory |
| bandwidth_limitation | 0.70 | cognitive_bottlenecks |
| chunking | 0.80 | capacity_limits (w=1.0), working_memory (w=0.9 via chunking strategy) |
| slot_model | 0.70 | capacity_limits |
| cowan_4_limit | 0.80 | capacity_limits (direct quantification) |
| spaced_repetition | 0.80 | learning_strategies |
| dual_coding | 0.80 | learning_strategies |
| worked_examples | 0.70 | learning_strategies |
| interleaving | 0.70 | learning_strategies |
| metacognition | 0.70 | learning_strategies, human_learners |
| individual_differences | 0.70 | human_learners |
| novice_vs_expert | 0.80 | human_learners (w=1.1 central tension) |
| rehearsal | 0.70 | working_memory |
| expertise | 0.70 | human_learners, novice_vs_expert (max) |

*19 distinct nodes activated at step 1.*

**Step 2 — Propagation (decay=0.49 from seeds):**

| Node | a(t=2) | Activated by |
|---|---|---|
| schema | 0.56 | chunking (w=1.0), novice_vs_expert (w=1.0) — DUAL SOURCE |
| pattern_recognition | 0.56 | chunking, expertise |
| instructional_design | 0.56 | cognitive_load, worked_examples |
| ebbinghaus_forgetting_curve | 0.56 | spaced_repetition, long_term_memory — DUAL SOURCE |
| automatization | 0.56 | spaced_repetition, chunking, expertise — TRIPLE SOURCE |
| paivio_dual_coding_theory | 0.56 | dual_coding |
| multimedia_learning | 0.56 | dual_coding, instructional_design — DUAL SOURCE |
| sleep_consolidation | 0.49 | spaced_repetition, long_term_memory |
| expertise_reversal_effect | 0.56 | novice_vs_expert, worked_examples — DUAL SOURCE |
| desirable_difficulty | 0.49 | interleaving, spaced_repetition, cognitive_load — TRIPLE SOURCE |
| retrieval_practice | 0.56 | spaced_repetition, learning_strategies |
| WM_span_individual_diff | 0.49 | individual_differences, central_executive — DUAL SOURCE |
| extraneous_load | 0.56 | cognitive_load, instructional_design |
| germane_load | 0.49 | cognitive_load |
| anxiety_WM_interaction | 0.49 | individual_differences, central_executive — DUAL SOURCE |

*15 new nodes activated at step 2 (34 total).*

**Step 3 — Propagation (decay=0.343 from seeds):**

| Node | a(t=3) | Activated by |
|---|---|---|
| deliberate_practice | 0.39 | automatization, expertise |
| flow_state | 0.39 | automatization, desirable_difficulty — DUAL SOURCE |
| CTML_cognitive_theory_multimedia | 0.39 | multimedia_learning, paivio_dual_coding_theory |
| transfer_appropriate_processing | 0.39 | retrieval_practice, interleaving — DUAL SOURCE |
| worked_examples_to_problem_solving | 0.39 | expertise_reversal_effect, worked_examples |
| distributed_practice | 0.39 | spaced_repetition, ebbinghaus_forgetting_curve |
| cognitive_interleaving_benefit | 0.39 | interleaving, desirable_difficulty |

*7 additional nodes. Total activation cloud: 41 nodes.*

**Lateral inhibition applied:**
- schema and pattern_recognition: share >70% source paths (chunking + expertise chains) → suppress pattern_recognition: a = 0.56 - 0.3×0.56 = **0.39** (below convergent threshold)
- spaced_repetition and retrieval_practice: share source paths → suppress retrieval_practice: a = 0.56 - 0.3×0.56 = **0.39**

</activation_map>

<convergent_nodes>

## Convergent nodes

**Qualifying nodes (a ≥ 0.4 after inhibition, ≥2 independent source chains, not directly seeded):**

1. **schema** — a=0.56 — [multi-source-convergent: capacity_limits→chunking + learning_strategies→novice_vs_expert]
   - Independent roots: capacity limit (architectural) + learning experience (behavioral)
   
2. **automatization** — a=0.56 — [multi-source-convergent: spaced_repetition + chunking + expertise]
   - Independent roots: learning strategies path + capacity limits path + human expertise path (3 independent chains)

3. **ebbinghaus_forgetting_curve** — a=0.56 — [multi-source-convergent: spaced_repetition + long_term_memory]
   - Independent roots: strategy path + memory architecture path

4. **multimedia_learning** — a=0.56 — [multi-source-convergent: dual_coding + instructional_design]
   - Independent roots: capacity exploitation strategy + instructional design intervention

5. **expertise_reversal_effect** — a=0.56 — [multi-source-convergent: novice_vs_expert + worked_examples]
   - Independent roots: human learner variation + specific strategy interaction

6. **desirable_difficulty** — a=0.49 — [multi-source-convergent: interleaving + spaced_repetition + cognitive_load]
   - Independent roots: scheduling strategy + load theory (3 chains)

7. **metacognition** — a=0.70 (seeded indirectly) — [multi-source-convergent: learning_strategies + human_learners + cognitive_load]
   
8. **anxiety_WM_interaction** — a=0.49 — [multi-source-convergent: individual_differences + central_executive]

**8 convergent nodes found** — well above the S3_thin_or_empty threshold of <2.

</convergent_nodes>

<selected_illuminations>

## Selected illuminations

Elegance scoring and combined ranking:

| Node | a | Simplicity | Symmetry | Depth | Elegance | Combined |
|---|---|---|---|---|---|---|
| automatization | 0.56 | 1.0 | 0.7 | 1.0 | 2.7 | 1.51 |
| metacognition | 0.70 | 0.7 | 0.7 | 1.0 | 2.4 | 1.68 |
| schema | 0.56 | 1.0 | 0.7 | 0.7 | 2.4 | 1.34 |
| desirable_difficulty | 0.49 | 0.7 | 0.7 | 1.0 | 2.4 | 1.18 |
| expertise_reversal_effect | 0.56 | 0.7 | 0.4 | 0.7 | 1.8 | 1.01 |

**Top 5 selected (by combined score):**

**Illumination 1 — Metacognition as the executive bottleneck regulator** (combined=1.68)
The central executive bottleneck is not a fixed wall but a dynamically managed resource. Metacognition is the control loop that detects when WM load is approaching saturation and adjusts strategy (switch to chunking, apply a schema, reduce extraneous load). The insight: teaching metacognitive monitoring skills is equivalent to teaching load management — the learner becomes the instructional designer of their own WM. Cross-domain analog: error detection/correction loops in engineering control systems.

**Illumination 2 — Automatization as WM recycler** (combined=1.51)
The capacity bottleneck (~4 chunks) appears fixed, but automatization continuously relocates completed sub-skills from WM to LTM as procedural schemas, freeing the same slots for higher-order processing. This is not a strategy to work around the bottleneck — it IS the bottleneck's natural adaptation mechanism. Implication: learning is not filling a bucket but a continuous recycling of the bucket's contents. Cross-domain analog: CPU cache line replacement policy (LRU eviction frees cache for new work).

**Illumination 3 — Schema formation as single-slot compression** (combined=1.34)
A schema is the mechanism by which what previously occupied 4 slots (e.g., individual chess pieces in a formation) becomes 1 slot (a named attack pattern). Schema formation is how experts transcend the ~4-chunk limit without violating it — the architectural constraint is unchanged; the information density per slot increases. Strategy implication: instructional design should prioritize schema formation (recognition + naming) over content coverage.

**Illumination 4 — Desirable difficulty as productive load** (combined=1.18)
Not all extraneous cognitive load is wasteful. Desirable difficulties (interleaving, spacing, testing) impose load that produces superior long-term retention despite worse immediate performance. The tension: WM-load-minimizing instructional designs (worked examples, blocked practice) optimize for short-term fluency but produce fragile learning. Desirable difficulties impose controlled load that forces retrieval + recomputation, directly training the LTM transfer pathways. Implication: the optimal strategy set changes across learning phases (novice → guided low-load; intermediate → desirable difficulty).

**Illumination 5 — Expertise reversal as the evidence that scaffolding has cost** (combined=1.01)
The expertise reversal effect shows that the same scaffold that helps novices (worked examples, detailed guidance) actively harms experts — it forces WM allocation to information that the expert's schema makes redundant. This confirms that the bottleneck is real, not metaphorical: adding content always has cost, and the cost changes sign as the learner's schema density increases. Strategy implication: adaptive learning systems must track learner schema formation, not just knowledge coverage.

</selected_illuminations>

</peripheral_exploration>

## Verification

**Darwin independence check:**
- Illumination 1 (metacognition) and Illumination 2 (automatization): metacognition chain roots in learning_strategies + human_learners; automatization roots in capacity_limits + spaced_repetition. The chains share no ancestor within 3 propagation steps. ✓ Independent.
- Illumination 3 (schema) and Illumination 2 (automatization): schema from chunking+novice_vs_expert; automatization from spaced_repetition+chunking. Chunking is a shared ancestor — NOTE: partially non-independent. Schema and automatization share the "chunking" ancestor at depth 1. Flagged as partially correlated; however, their downstream implications are structurally distinct (schema = static compression; automatization = dynamic offloading), so they are retained as two distinct illuminations with this dependency noted.

**Ramanujan pattern verification:**
- "Automatization as WM recycler" — verified in: (1) motor learning (driving — sub-skills become automatic over sessions); (2) arithmetic fluency (single-digit addition becomes procedural, freeing WM for algebra); (3) reading comprehension (decoding automatizes, freeing WM for meaning). ≥3 instances. ✓
- "Schema as single-slot compression" — verified in: (1) chess masters (de Groot 1946); (2) radiologist pattern recognition (disease patterns as chunks); (3) basketball plays. ≥3 instances. ✓

**Activation-map completeness:** 41 distinct nodes activated across 3 propagation steps. ≥15 threshold met. ✓

**Signal status:** 8 convergent nodes found. S3_thin_or_empty NOT raised.

---

## SIGNAL OUTPUT

Emits: activation_digest (5–15 lines)

```
key_findings:
  - activation map found: working_memory,cognitive_bottlenecks,capacity_limits,learning_strategies,human_learners,phonological_loop,central_executive,chunking,spaced_repetition,dual_coding,novice_vs_expert,schema,automatization,desirable_difficulty,metacognition,expertise_reversal_effect,multimedia_learning; top convergent: metacognition,automatization,schema; activation strength: H
  - Illumination 1: metacognition is the control loop for executive bottleneck management — teaching metacognition = teaching WM load regulation
  - Illumination 2: automatization continuously recycles WM capacity by offloading sub-skills to LTM — the bottleneck adapts via practice
  - Illumination 3: schema formation compresses multiple primitives to single WM slot — experts transcend item-count limit without violating it
  - Illumination 4: desirable difficulties impose productive load — spacing/interleaving trade short-term fluency for LTR via forced retrieval
  - Illumination 5: expertise reversal proves scaffolding has real cost — optimal strategy inverts as learner schema density increases
  - Cross-domain convergences: automatization ↔ CPU cache replacement policy; schema ↔ compression codec; metacognition ↔ control system error-detection loop
  - Darwin dependency note: schema and automatization share "chunking" ancestor; their downstream implications are structurally distinct (static vs. dynamic)
  - Individual differences: WM span + anxiety interact with central executive bottleneck; one-size-fits-all strategies are suboptimal
named_entities:
  - metacognition, automatization, schema formation, desirable difficulty, expertise reversal effect
  - Cowan 4-chunk limit, central executive, Bjork (desirable difficulty), de Groot (chess schema), Sweller CLT
confidence_flags: [H, H, H, H, H, M, M, M]
signal_flags: []
```
