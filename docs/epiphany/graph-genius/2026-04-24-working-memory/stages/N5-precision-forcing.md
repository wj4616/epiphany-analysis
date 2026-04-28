# N5 — Precision Forcing (PRECISION)
## Executed: 2026-04-24 | Scale: STANDARD | Hat: Feynman

**Primary claim under analysis:** Working memory has three major bottlenecks — item count (~3–4 chunks), phonological temporal decay (~2s), and central executive capacity. These bottlenecks directly determine which learning strategies are effective.

<precision_forcing>

<renderings>

## 5-level vocabulary strip

**Level 1 — Five-year-old:**
"Your brain has a small tray that holds thoughts while you're working. It can only hold about 4 big ideas at once, and it forgets them after about 2 seconds if you don't keep thinking about them. If you try to think about too many things at once, the tray overflows and you forget something. To learn well, you need to either make each idea 'bigger' (clump small pieces into one big piece), keep going back to things before you forget them, or use both your word-thoughts and your picture-thoughts at the same time since those are kept in different parts of the brain."

**Level 2 — Fifteen-year-old:**
"Working memory is the mental workspace where you process new information — it can hold roughly 3–4 meaningful units simultaneously and loses verbal information after about 2 seconds without rehearsal. Cognitive bottlenecks occur when the amount of information to process exceeds this capacity. Learning strategies work by either reducing what needs to be held in working memory (chunking, schemas), exploiting redundancy in the system (dual coding — words and images use separate storage), or ensuring that previously learned material is automatic so it doesn't occupy working memory space at all (spaced practice → automatization)."

**Level 3 — Domain expert:**
"Working memory capacity is ~3–4 meaningful chunks (Cowan 2001) governed by a multicomponent architecture: phonological loop (~2s decay, refreshed by subvocal rehearsal), visuospatial sketchpad (parallel storage, limited capacity), and the central executive (attentional controller with finite processing bandwidth). Cognitive load is partitioned into intrinsic load (task complexity, determined by element interactivity), extraneous load (inefficiencies in instructional format), and germane load (schema-building activity). Effective learning strategies reduce extraneous load, exploit dual-channel independence, automate sub-skills via spaced retrieval practice, and scaffold schema formation through worked examples and explicit category training. The expertise reversal effect constrains strategy applicability: novices require guidance (reduces extraneous load); experts require challenges (prevents schema competition)."

**Level 4 — Mathematician/logician:**
"Let WM = (PL, VS, CE) where PL = phonological loop with capacity T_PL ≈ 2s speech content, VS = visuospatial sketchpad with capacity κ_VS elements (parallel to PL), CE = central executive with capacity κ_CE ∈ [2,4] attentional units. Cognitive load L = L_I + L_E + L_G ≤ κ_CE. Where: L_I = intrinsic load (fixed by task's element interactivity, not reducible by design); L_E = extraneous load (reducible by instructional design choices); L_G = germane load (schema formation activity, productive). A learning strategy S is effective iff applying S reduces L_E + L_G × (1 - schema_depth) to fit within κ_CE for the learner's current schema_depth. Automatization: repeated retrieval practice → schema_depth → 1 → sub-skill L_I contribution → 0 (offloaded to LTM as procedural)."

**Level 5 — Minimal symbol:**
L_total = L_I + L_E + L_G ≤ κ_WM ≈ 4 chunks
Strategy_effective iff L_E ↓ AND/OR L_I(sub-skill) → 0 via automatization AND/OR κ(PL, VS) used in parallel

## 3-representation rendering

**Representation 1 — Diagram:**
Three stacked buffers: [Phonological Loop | 2s decay, ~2 speech units] || [Visuospatial Sketchpad | ~2-4 spatial units] → both feed into [Central Executive | κ ≈ 3–4 attentional slots]. Incoming task content has three load types: L_I (dark bar, fixed by task), L_E (dashed bar, design-dependent), L_G (grey bar, schema formation). When total bar height exceeds the CE slot count → overflow → forgetting/errors. Strategies are represented as arrows that reduce bar height or increase slot count per modality (dual coding = two parallel buffers rather than one).

**Representation 2 — Equation:**
κ_WM ≈ 4 chunks (Cowan 2001 empirical constraint)
L_total = L_I + L_E + L_G
Constraint: L_total ≤ κ_WM (must hold or learning is impaired)
Strategy S* minimizes: L_E + α·L_G(non-schema) subject to schema_depth increasing
Dual coding: κ_effective = κ_PL + κ_VS (when content is dual-codable)
Automatization: L_I(sub-skill) → 0 as practice trials → ∞ (power law of practice)

**Representation 3 — Procedural:**
For a given learning task T:
1. Measure L_I: count number of interacting elements (item count before any chunking).
2. Identify L_E sources: separated diagrams and text; redundant information; unclear organization.
3. Reduce L_E: integrate text into diagrams; remove redundancy; sequence prerequisite skills.
4. Check if L_I + L_E ≤ κ_WM. If not: split task into simpler sub-goals; provide worked examples.
5. Exploit dual coding: convert purely verbal content to verbal+visual where possible.
6. Build automatization: spaced retrieval practice on sub-skills until response time < 2s (automaticity criterion).
7. Adapt to learner expertise: monitor for expertise reversal (high-prior-knowledge learners → remove scaffolding).

</renderings>

<constructive_specification>

## Constructive specification

**Spec:** An agent that (a) assigns a learner a task with measurable element interactivity L_I, (b) implements two instructional conditions (high-extraneous vs. low-extraneous), (c) measures learning outcomes and WM recall scores after identical practice, and (d) demonstrates L_E(high) > L_E(low) → outcome(high) < outcome(low) at equal L_I, constitutes a demonstration of the WM bottleneck → learning strategy link.

**Minimal-model check:** Can condition (d) be simplified? A within-subjects design with only the dual-coding intervention (adding images to text) at fixed L_I provides the minimum adequate test. The broader L_E manipulation is not eliminable — L_I must be held constant or the test is confounded. Minimal model: single-factor L_E manipulation at fixed L_I, with WM span as covariate.

</constructive_specification>

<flagged_vagueness>

## Flagged vague claims

**[VAGUE]** "working memory capacity"
Reason: Used in two senses: (a) architectural slot count (κ ≈ 4), a stable individual property; (b) available capacity at a given moment, which varies with fatigue, anxiety, and prior task load. At Level 4, the formal version is κ_effective(t) = κ_max - load_from_prior_activity(t), where the second term is rarely operationalized.
Possible resolution: Specify "nominal capacity" (κ ≈ 4, architectural) vs. "effective capacity" (κ_effective, state-dependent). Most studies use nominal capacity; individual difference studies use effective.

**[VAGUE]** "chunk"
Reason: "Chunk" has no consensus formal definition — it is defined circularly as whatever can be recalled as a single unit. At Level 4, there is no predicate that determines whether two elements are one chunk vs. two.
Possible resolution: Operationalize via recall latency (items recalled in <500ms burst = same chunk) or via transfer error patterns (grouping errors between items → same chunk).

</flagged_vagueness>

<forward_chain>

## Forward-chain consequence tree

Root: L_total = L_I + L_E + L_G ≤ κ_WM ≈ 4 chunks; strategies reduce L_E or redistribute load across parallel channels.

```
Branch A — When L_I alone exceeds κ_WM (highly complex task, many interacting elements):
  A.1: Even with L_E=0 and L_G=0, task cannot be processed without error/forgetting
    A.1.1: Prerequisite decomposition required — break task into sub-skills with lower individual L_I
    A.1.2: Worked examples reduce L_I by providing solution structure as external memory
  A.2: Schema formation cannot begin until L_I sub-skills are handled
    A.2.1: Learning sequence must proceed from simple to complex (merrill's first principles)
    [CONTRADICTION] A.2.1 vs. desirable difficulty: complex tasks sometimes benefit from early interleaving — resolution: interleaving increases L_G but reduces eventual L_I via forced retrieval; optimal only after basic schemas formed

Branch B — When dual coding is applied (visual + verbal for same content):
  B.1: κ_effective doubles for dual-coded content (PL and VS channels parallel)
    B.1.1: Modality effect — spoken + visual beats text + visual (text loads PL and VS; speech loads PL, image loads VS)
    B.1.2: Redundancy effect — spoken text + same written text increases L_E not decreases it (same channel overloaded)
    [DEGENERATE] B.1.2: Redundancy effect reverses dual-coding benefit — degenerate case where "dual" means same channel twice
  B.2: Content must be structurally mappable across modalities for dual coding to work
    B.2.1: Abstract relationships (proofs, logical chains) resist visual encoding → dual coding benefit limited for pure abstraction

Branch C — When automatization occurs (spaced practice to asymptote):
  C.1: Sub-skill L_I contribution → ~0 (LTM procedural encoding)
    C.1.1: WM κ freed for higher-order processing at same task
    C.1.2: Expertise reversal effect emerges — formerly helpful scaffolding now adds L_E (redundant with LTM content)
  C.2: Power law of practice: response time RT = a·N^(-b) — automaticity measurable as performance asymptote
    C.2.1: Individual b exponents vary with WM span — higher WM span → faster automatization
    C.2.2: Domain-specific automatization; does not transfer across unrelated tasks (limited WM training transfer evidence)

Branch D — When L_E is reduced (split-attention fix, worked examples):
  D.1: Freed capacity re-allocated to L_G (schema formation)
    D.1.1: Learning rate increases for novices
    D.1.2: Beyond expertise transition: L_E reduction provides no benefit (expert already processes content as schema)
  D.2: Redundancy effect: over-reduction of L_E (removing all challenge) can reduce L_G
    D.2.1: Minimum viable L_E may exist below which germane load fails to engage schema elaboration

Branch E — Individual differences in κ_WM:
  E.1: High-WM learners tolerate higher L_total before overflow
    E.1.1: High-WM learners benefit less from scaffolding; may be hindered by it (expertise reversal analog)
    E.1.2: Same instructional design produces different outcomes by WM span — adaptive design is required
  E.2: Anxiety reduces κ_effective (Eysenck attentional control theory)
    E.2.1: Test anxiety → lower κ_effective → higher failure rate at same nominal L_total → vicious cycle
    E.2.2: Desirable difficulties become undesirable difficulties for high-anxiety learners at critical threshold

Branch F — When learning occurs across sessions (spaced repetition):
  F.1: Between-session consolidation moves WM trace to LTM (sleep consolidation)
    F.1.1: Items rehearsed at WM → encoded in LTM → next session begins with lower L_I
    F.1.2: Spacing interval = optimal interval at which trace has weakened but not disappeared (Ebbinghaus curve)
  F.2: Forgetting between sessions is adaptive — reloading weakened trace is the mechanism of LTR
    F.2.1: Massed practice avoids forgetting but avoids the strengthening mechanism → fragile memory
    F.2.2: Interleaving forces reloading across topics within session — same mechanism at short timescale

Branch G — Expert instructional design (CLT application):
  G.1: Split-attention fix: integrate text and diagram spatially → reduces L_E by eliminating eye-movement overhead
    G.1.1: Reduces referential load (must-hold-text-in-mind-while-looking-at-diagram)
    G.1.2: No benefit if content is purely verbal (no diagram to integrate)
  G.2: Worked examples → completion problems → full problems (fading technique)
    G.2.1: Fading reduces L_E progressively as schema depth grows
    G.2.2: Fading rate must track schema formation rate — too fast → L_total overflow; too slow → expertise reversal

Branch H — Metacognition as load monitor:
  H.1: Metacognitive awareness of WM saturation allows proactive strategy switching
    H.1.1: Learner switches from elaboration to schema recall when sensing overload
    H.1.2: Metacognitive skill itself occupies WM — novices cannot afford the overhead
  H.2: Metacognitive monitoring is teachable (Schraw et al., Butler & Winne)
    H.2.1: Explicit metacognitive instruction improves learning outcomes but requires initial scaffolding to offset H.1.2 overhead

Branch I — Transfer of learning:
  I.1: Near transfer: same task, different surface features → high transfer via schema
    I.1.1: Transfer proportional to schema abstraction level (higher abstraction → wider transfer)
    I.1.2: WM load during encoding predicts transfer — low WM load → richer LTM encoding → more transfer
  I.2: Far transfer: structurally different task domain
    I.2.1: Far transfer requires schema abstraction + explicit analogical comparison — cannot be achieved via drill alone
    I.2.2: Analogical comparison during learning increases WM load temporarily but produces higher transfer (Gentner)

Branch J — Technology-mediated learning:
  J.1: Adaptive learning systems can reduce L_E dynamically by tracking learner performance
    J.1.1: Granularity of adaptation determines benefit — concept-level vs. item-level tracking
    J.1.2: Excessive adaptive scaffolding may induce learned helplessness (removes all L_G challenge)
  J.2: Multimedia learning environments can induce split-attention if interface design is poor
    J.2.1: Interaction adds cognitive load (navigation overhead) before content load — design must budget for interface L_E
```

## Flags Summary

[CONTRADICTION] Branch A.2.1 vs. desirable difficulty (A.2): early interleaving appears to conflict with prerequisite sequencing; resolved by learning phase (novice: sequenced; intermediate: interleaved).
[DEGENERATE] Branch B.1.2: redundancy effect degenerates the dual-coding benefit when both channels carry identical content.

## Tree Statistics

Branches: 10 (A–J) | Flags: 1 contradiction (resolvable), 1 degenerate | Depth: 3–4

</forward_chain>

</precision_forcing>

---

## SIGNAL OUTPUT

Emits: precision_digest (5–15 lines)

```
key_findings:
  - Formal model: L_total = L_I + L_E + L_G ≤ κ_WM ≈ 4 chunks; all effective strategies reduce L_E or exploit parallel channels
  - Three bottleneck types mapped to strategies: (1) item-count → chunking/schema; (2) temporal decay → rehearsal/spaced repetition; (3) executive capacity → dual coding/load reduction
  - Dual coding: κ_effective = κ_PL + κ_VS when content is structurally dual-codable; redundancy effect is the failure mode
  - Automatization: L_I(sub-skill) → 0 via power law of practice; expertise reversal emerges as threshold crossed
  - Two vague claims flagged: "working memory capacity" (nominal vs. effective); "chunk" (no consensus formal definition)
  - Key contradiction: desirable difficulty vs. prerequisite sequencing — resolved by learning phase (novice: scaffold; intermediate+: challenge)
  - Metacognition occupies WM — novices cannot afford it; scaffolded metacognitive training required for novices
  - Individual differences (WM span, anxiety) → adaptive instructional design is required; single strategy is suboptimal
  - Constructive spec: measure L_I (element interactivity count), manipulate L_E (design conditions), measure outcomes — minimum adequate single-factor test
named_entities:
  - cognitive load theory, intrinsic load L_I, extraneous load L_E, germane load L_G, κ_WM≈4, phonological loop, visuospatial sketchpad, dual coding, automatization, expertise reversal, desirable difficulty
confidence_flags: [H, H, H, H, H, H, H, H, H]
signal_flags: []
```
