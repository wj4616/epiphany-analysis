## Pipeline Status Header

> v1.1.0 · Scale: STANDARD · Pipeline: complete at 7 of 7 planned · Active conditionals: none · For raw state, re-invoke with --xml

---

## Headline Insight

Working memory imposes three structurally distinct, non-trainable constraints on learning: a fixed item-count ceiling (κ ≈ 3–4 chunks, Cowan 2001), a short temporal window (~2s phonological decay, Baddeley 1974), and a processing-bandwidth bottleneck (central executive, finite attentional capacity). These constraints are architectural — no learning strategy eliminates them. All effective learning strategies operate through one or more of exactly three mechanisms: **(1) compress or reduce load** to keep L_total ≤ κ_WM (chunking, schema pre-activation, worked examples, split-attention reduction); **(2) exploit parallel channels** to expand effective capacity without widening the bottleneck (dual coding, κ_effective = κ_PL + κ_VS when content is dual-codable); or **(3) progressively offload sub-skills** from WM to LTM via practice (automatization, spaced repetition). A fourth cross-cutting factor governs all three mechanisms: the learner's current schema state determines which strategies are beneficial and which are harmful — expertise reversal is not an anomaly but a structural implication. The framework is further constrained by a domain-regularity assumption: all regularity-exploiting strategies presuppose learnable structure in the domain and degenerate in structureless or random-stimulus contexts. Seven convergence signals, each confirmed by ≥4 independent reasoning nodes, underwrite this conclusion; overall confidence: **HIGH** (subject to three named scope qualifications).

**Creativity type:** Combinatorial — the insight is the convergence of constraint theory (CLT), Baddeley's multicomponent WM model, automatization/spaced-repetition research, and dual-coding theory under one formal constraint language, independently instantiated across five structurally distinct cross-domain analogs. No constitutive constraint of the conceptual space is negated.

---

## Theory Collisions

*(Stage N4 skipped at STANDARD scale — run DEEP for this section.)*

The following inter-theory tensions were identified across nodes and are documented here in lieu of a full N4 output:

**Collision T-1: Slot model vs. activated-LTM account**
- CLT/WM tradition (Baddeley, Cowan): WM is a separate architectural store with ~4 discrete slots.
- Ruchkin et al. (2003) / Jonides et al.: WM is simply activated portions of LTM — no separate storage system.
- Discriminating condition: if the activated-LTM account is correct, CLT's slot-count prescriptions are operationally useful approximations but theoretically misfounded. The "4 chunks" limit would be a threshold of simultaneous activation, not a storage count. Predictive difference: manipulations that increase LTM activation strength (e.g., very high familiarity) should raise the apparent slot count in the activated-LTM model but not in the strict slot model.

**Collision T-2: Germane load as mechanism vs. germane load as unfalsifiable label**
- CLT (original Sweller): germane load L_G is productive load that drives schema formation; the three-component model L_I + L_E + L_G ≤ κ_WM is the prescriptive engine.
- Sweller (2010) self-critique: germane load cannot be independently operationalized; anything that improves learning can be post-hoc labeled "germane." The three-component model loses falsifiability at the L_G term.
- Discriminating condition: if a neural proxy for germane load (hippocampal encoding activity, alpha suppression in schema-relevant regions) can be measured independently of the learning outcome, the original CLT survives; if no such proxy exists, the framework reduces to "reduce L_E, observe outcome," with the mechanism between load reduction and schema formation unspecified.

**Collision T-3: Chunk-compression (CA-1) vs. LT-WM retrieval structures (CA-4)**
- Chunk-compression account: expert capacity is explained by denser slot contents — the same 4 slots hold higher-order units.
- Ericsson & Kintsch (1995) LT-WM: experts build retrieval structures in LTM that allow rapid access via cue pointers held in WM. Experts effectively extend WM without increasing chunk count.
- Discriminating condition: degrade LTM access (retroactive interference task) during expert recall of structured domain material. If expert performance collapses to novice level, LT-WM is additive to chunk-compression; if the advantage persists, chunk-compression alone explains the residual.

---

## Discovery vs. Proof

| Discovery (N2 domain mappings + N3 illuminations) | Proof (N5 constructive spec + N7 verification) |
|---------------------------------------------------|-----------------------------------------------|
| **Resonant Cache blend (N2):** Schema pre-construction is a dual-function operation — simultaneously reduces WM slot demand (cache prefetch) AND amplifies encoding depth (resonant amplification at natural frequency). Neither source domain (acoustics, cache architecture) alone predicts both effects. | **Formal model (N5):** L_total = L_I + L_E + L_G ≤ κ_WM ≈ 4 chunks. Strategy S is effective iff it reduces L_E, reduces L_I(sub-skill) via automatization, or exploits κ_PL + κ_VS in parallel. Formally verified for 10 forward-chain branches (N–J). |
| **Emergent constraint from blend (N2):** Wrong-schema activation is WORSE than no schema — impedance mismatch causes reflection AND active slot consumption for conflict resolution. | **Constructive spec (N5):** Single-factor L_E manipulation at fixed L_I with WM span as covariate and secondary-task paradigm demonstrates the bottleneck → learning outcome link without requiring germane load operationalization. |
| **Turbulence / thrashing phase transition (N2):** Cognitive overload is not smooth degradation but a discontinuous phase transition — confirmed across fluids (turbulence), cache (thrashing), and geology (elastic rebound). | **V1 Logic (N7):** All stated inferential steps valid; one partial gap: induction over the known strategy space cannot exclude unknown mechanisms (bounded by N6 density review of 7+ counter-examples). |
| **Metacognition as load controller (N3 Illumination 1):** Teaching metacognition = teaching WM load regulation; the learner becomes the instructional designer of their own WM. | **V3 Symmetric Scrutiny (N7 PASS):** Primary conclusion faced 7+ named counter-examples, 8 boundary regimes, and 6 alternative hypotheses with Millikan correction applied to three detected asymmetries. |
| **Desirable difficulty as productive load (N3 Illumination 4):** Some cognitive load improves long-term retention by forcing retrieval and recomputation — WM-minimizing designs may produce fragile learning. | **V5 Constructive (N7 PASS):** Demonstrable in principle; germane load intermediate step acknowledged as not independently demonstrable (disclosed). |

---

## Independence-Verified Bridges

**Bridge B-1: Information Theory → WM bottleneck (N2 Mapping 1)**
- Source concept: Shannon channel capacity C — fixed bandwidth; signal above C is lost or corrupted; compression reduces effective bit-cost.
- Target insight: WM capacity κ_WM is a fixed-bandwidth channel. The instructional design solution is pre-compression (chunking/schema) and noise reduction (extraneous load suppression), not channel widening (WM training transfer is negligible — structurally equivalent to the impossibility of exceeding Shannon capacity by trying harder).
- Structural invariant: fixed-ceiling + compression-escape architecture.
- Disanalogy limit: Shannon capacity is fully precise; WM capacity is an empirical approximation with individual variation and state-dependence. The engineering metaphor does not import exact quantitative predictability.

**Bridge B-2: Cache Architecture / Memoization → Automatization (N2 Mapping 5)**
- Source concept: CPU register file (fewest, fastest); memoization moves frequently-needed sub-operations from "compute in registers" to "retrieve compiled result," shrinking the working set.
- Target insight: Automatization via spaced practice is the cognitive equivalent of memoization — sub-skill L_I contribution → 0, freeing WM slots for higher-order processing. The learning sequence "automate sub-skills first, then combine" is structurally identical to "memoize hot paths before optimizing the algorithm."
- Structural invariant: fixed-register constraint + memoization-as-offload escape.
- Disanalogy limit: CPU memoization is deterministic and complete; cognitive automatization follows a power law asymptote and is domain-specific (procedural > declarative).

**Bridge B-3: Acoustics / Resonance → Prior knowledge effect (N2 Mapping 4)**
- Source concept: resonant cavity with natural frequency f₀; input driven at f₀ amplifies with low energy cost; off-resonance input dissipates; impedance mismatch causes reflection and loss.
- Target insight: LTM schemas are resonant cavities — new content matching existing schema encodes deeply at low WM cost; schema mismatch causes both under-encoding and active interference (WM slots consumed for conflict resolution). Instructors can "design the cavity" — build a minimal scaffold schema before introducing new content.
- Structural invariant: pre-aligned structure amplifies incoming input; misaligned structure dissipates it.
- Disanalogy limit: resonance is a continuous physical phenomenon; schema matching is categorical and partially discrete. The "wrong-schema = worse than no schema" prediction is emergent from the blend and has not been directly tested in this specific form.

**Bridge B-4: Linguistics / Garden-Path Parsing → Instructional sequencing (N2 Mapping 3)**
- Source concept: parsing buffer overflow in garden-path sentences; right-branching (scaffolded) structures reduce buffer depth; predictive parsing (top-down frame before bottom-up detail) reduces constituent-holding cost.
- Target insight: Instructional sequences that leave many open conceptual threads ("partially integrated concepts awaiting closure") overload WM even with low individual item count. Optimal sequencing closes constituent chains early: conclusions before derivations where possible; schema frame before detail loading.
- Structural invariant: constituent-buffer management is equivalent to WM slot management during complex encoding.
- Disanalogy limit: syntactic parsing has a defined constituency grammar; instructional content does not always have an equivalent formal constituency structure — the analogy guides design heuristics but does not generate mechanical predictions.

**Bridge B-5: Resonant Cache blend — schema pre-construction as dual-function operation (N2 Step 7)**
- Source: combination of Mappings 4 and 5 (acoustics + cache); neither alone predicts the dual-function structure.
- Emergent target insight: Advance organizers and schema activation before learning are not merely helpful priming — they simultaneously (a) reduce WM slot demand by prefetching expected structures (cache warm-up) and (b) amplify encoding depth by tuning the "resonant cavity" of LTM to the incoming signal. A single pre-learning intervention has two mechanistically distinct beneficial effects.
- Structural invariant: pre-alignment of a fixed-capacity fast-access store with expected demand determines whether the system runs efficiently or wastes resources.
- Disanalogy limit: the dual-function prediction is emergent from the conceptual blend; empirical tests measuring both the WM-load reduction (secondary task) and the encoding-depth benefit (delayed cued recall) simultaneously have not been reported in this specific two-outcome form.

---

## Alternative Hypotheses

**CA-1 — Chunk-Compression as primary expertise mechanism**
- Confidence: 0.85
- Supporting modes: Abductive (H-1, highest combined score 2.35) + Deductive (D-A consequence valid) + Inductive (G2 robust across chess/arithmetic/reading/radiology/music/basketball)
- Claim: The expert–novice capacity gap is explained by hierarchical compression of domain primitives into higher-order chunks — same ~4 slots, denser contents. The slot ceiling holds; slot information density scales with experience.
- Falsification condition: Expert advantage should disappear on random stimuli that provide no domain-regular structure (de Groot random board replication; Chase & Simon 1973). Already tested; G2 ROBUST.

**CA-2 — CLT extraneous-load principle is learner-state-dependent**
- Confidence: 0.82
- Supporting modes: Abductive + Deductive (D-1 direct) + Inductive (G3 direct); Millikan flag (CLT presentations biased toward novice data)
- Claim: The prescription "reduce extraneous load → improve learning" holds for novice learners but inverts for experts (expertise reversal effect) — the same scaffold that reduces extraneous load for novices adds redundancy load for experts. CLT prescriptions must be conditioned on schema state.
- Falsification condition: If extraneous-load reduction helps experts as much as novices (no interaction effect), the state-dependence claim fails.

**CA-3 — Automatization WM benefit is domain-type-specific (procedural >> declarative)**
- Confidence: 0.72
- Supporting modes: Abductive + Deductive (D-2) + Inductive (G1 limitation + I-1); Millikan flag (primary account treats automatization as universally WM-freeing without declarative-domain evidence)
- Claim: The WM-freeing mechanism of spaced repetition is better evidenced in procedural and motor domains than in purely declarative learning. In declarative domains, the benefit may operate primarily through consolidation (sleep-dependent LTM encoding) rather than WM-freeing at time of learning.
- Falsification condition: If declarative spaced repetition produces equivalent dual-task cost reduction to motor learning, the domain-specificity claim is undermined.

**CA-4 — LT-WM Extension via LTM retrieval structures (Ericsson & Kintsch 1995)**
- Confidence: 0.65
- Supporting modes: Abductive (H-2, combined 2.30) + Deductive (indirect, D-A); Millikan flag — underweighted relative to CA-1
- Claim: Expert capacity is not only chunk-compression but also the construction of LTM retrieval structures that can be accessed as if in WM via cue pointers. Experts hold cues in their ~4 WM slots that point to large LTM structures, effectively extending capacity without increasing chunk count. This may be a fourth mechanism additive to the three-mechanism framework.
- Falsification condition: LTM interference task during expert recall of structured domain material should collapse expert advantage if LT-WM is the operative mechanism; if expert advantage persists, chunk-compression alone explains the residual.

**CA-5 — Attentional Filtering Sharpening**
- Confidence: 0.50
- Supporting modes: Abductive (H-5, combined 2.05) + partial Inductive (G2 random stimuli)
- Claim: Expertise sharpens early attentional filtering so irrelevant items never enter WM (inhibited before encoding), reducing effective load for experts on equivalent tasks — not because they have more capacity but because they load less.
- Falsification condition: Experts should show reduced early neural load markers (N2pc, MMN amplitude) on domain-relevant stimuli vs. novices; if markers are equivalent, filtering sharpening is undermined.

---

## Density-Checked Falsification

**CE-1 — WM training transfer controversy (N-back)**
7 vectors attempted — density HIGH — residual uncertainty HIGH
The "fixed architectural ceiling" claim is challenged by Jaeggi et al. (2008) reporting WM training transfer to fluid intelligence. Subsequent replications largely failed (Melby-Lervåg & Hulme 2013; Melby-Lervåg et al. 2016 meta-analyses). Counter-evidence is mixed; the ceiling is treated as real and non-trainable in this analysis. Residual: the degree of plasticity is contested; the claim's practical prescriptions are unchanged (strategies that route around the ceiling remain optimal whether the ceiling is absolutely fixed or merely very resistant to training).

**CE-2 — Desirable difficulty paradox**
7 vectors attempted — density HIGH — residual uncertainty MODERATE
CLT's load-minimization prescription conflicts with desirable difficulties (interleaving, spacing, testing) that increase short-term cognitive load yet produce superior long-term retention. Resolved by learning-phase qualification: load-minimization is optimal during initial encoding (novice phase); desirable difficulties are optimal during consolidation and intermediate-to-expert phases. The germane load construct invoked to reconcile this is itself criticized as unfalsifiable (Sweller 2010 self-critique) — this resolution is conceptual, not independently operationalizable.

**CE-3 — Beilock "choking under pressure"**
7 vectors attempted — density HIGH — residual uncertainty MODERATE
High-skilled performers (high WM span) show performance degradation under high-pressure conditions — directly counter to the simple WM-span → performance linearity claim. Mechanism: high-span individuals rely on WM-mediated executive control; under anxiety, this control system is disrupted, and automatized procedures are destabilized by explicit monitoring. Incorporated as CA-6 (confidence 0.48). Residual: generalization from procedural performance tasks to declarative learning tasks not yet confirmed.

**CE-4 — Activated LTM account (Ruchkin, Jonides)**
7 vectors attempted — density HIGH — residual uncertainty HIGH
The ontological assumption of WM as a separate architectural store (not merely activated LTM) is challenged. If WM is activation, not storage, the slot-count metaphor is an approximation. Residual: CLT's prescriptions remain operationally valid even if the underlying ontology is activation-based; the practical impact on strategy recommendations is low, but the theoretical foundations of the slot-count formalization are weakened.

**CE-5 — Einstellung effect (schema interference)**
7 vectors attempted — density HIGH — residual uncertainty LOW
Highly chunked schemas can suppress recognition of novel patterns not fitting the schema (Einstellung effect in chess). Expert chunks may impede flexible adaptation. Claim boundary: the bottleneck framework does not predict that "more chunking is always better" — schema formation is beneficial during acquisition; schema rigidity is a cost at the expert-adaptation boundary. This is a scope qualification, not a falsification of the acquisition-phase prescriptions.

**CE-6 — Individual variability in phonological loop decay**
7 vectors attempted — density HIGH — residual uncertainty LOW
The ~2s decay estimate is a population average; there is substantial individual variability. The claim is not falsified but requires qualification: the temporal bottleneck is real, but precise ~2s prescriptions (e.g., "all verbal content must be rehearsed within 2 seconds") are over-precise. The claim survives as a range estimate with individual variation acknowledged.

**CE-7 — Dual coding overload failure mode**
7 vectors attempted — density HIGH — residual uncertainty LOW
Dual coding harms learning when both channels are independently overloaded by complex material (modality-effect failure; split-attention with high-complexity visual + verbal simultaneously). This is the redundancy effect — not a falsification of dual coding but its known degenerate case. The failure mode is incorporated in N5 Branch B.1.2 and confirmed by multimedia learning research.

---

## Scope Limits

**Applies to:** Human learners acquiring structured knowledge or skills in domains with learnable regularities (schema-formable domains), during the acquisition phase (novice to intermediate to proficient), where instructional design choices can control extraneous cognitive load, and where at least one parallel processing channel (phonological or visuospatial) is available for dual-coding exploitation. Conclusion is expressed in Symbolic+Procedural frame — applies directly to instructional designers, adaptive learning system engineers, and cognitive researchers working within the CLT/WM constraint framework.
**Does not extend to:** (1) Structureless or random-stimulus domains where no learnable regularities exist — all regularity-exploiting strategies (chunking, schema, automatization) are inapplicable; (2) Asymptotic expert performance (Time → ∞ after full automatization) — the framework's prescriptions become redundant or harmful at this stage; (3) Clinical populations with WM capacity below functional threshold (severe ADHD, dementia onset) — CLT prescriptions designed for typical WM fail below a minimum effective-capacity floor; (4) Open-ended creative domains where deliberate WM-search-space maintenance is required rather than minimized; (5) Purely declarative automatization claims — the WM-freeing mechanism of spaced repetition is better evidenced in procedural/motor domains than in declarative ones; (6) Verbal-only declarative answers to the original question — the conclusion is a re-representation in Symbolic+Procedural frame and does not directly translate to narrative instructional guides without translation back to Declarative frame.
**Claims refused:** (1) "Germane load (L_G) can be independently measured in practice" — refused: Sweller (2010) retracted the falsifiable form of germane load; it is a conceptual construct without an independently operationalizable measure at the critical extraneous→germane→schema intermediate step; (2) "LT-WM retrieval structures (Ericsson & Kintsch) are subsumed by chunk-compression" — refused: CA-4 (confidence 0.65) is an open competing mechanism; subsuming it requires the LTM-interference falsification experiment, which has not been completed here; (3) "WM capacity can be trained to increase" — refused: WM training transfer meta-analyses (Melby-Lervåg et al. 2013, 2016) show negligible far-transfer; the ceiling is treated as fixed and non-trainable in this analysis.

---

## Coherence Signals

**CS-1 — Fixed architectural ceiling (κ_WM ≈ 4 chunks is structural and non-trainable)**
Confirmed by: N1 (empirical irreducible, Cowan 2001) + N2 (Information Theory: cannot exceed Shannon capacity by trying harder; Fluid Dynamics: pipe diameter fixed) + N3 (spreading activation: cowan_4_limit a=0.80) + N5 (formal model constraint: all strategy equations assume fixed κ_WM) + N6 (extreme scenario: claim degenerates at WM→∞ but holds throughout normal range; WM training transfer meta-analyses confirm ceiling).
Cross-domain recurrence: 5 independent nodes; 3+ independent source domains (Information Theory, Fluid Dynamics, Optics/Diffraction limit). Strongest convergence signal in the pipeline.

**CS-2 — Chunking / schema formation as item-count bypass**
Confirmed by: N1 (irreducible 4: chunking compresses without removing the limit) + N2 (IT Mapping: compression reduces bit-cost per unit; Cache Mapping: memoization = chunk-as-compiled-routine) + N3 (Illumination 3: schema = single-slot compressor; de Groot verification) + N5 (formal: L_I reduction via chunk hierarchy depth; Branch A) + N6 (CA-1 highest confidence 0.85; G2 robust).
Cross-domain recurrence: chess masters (de Groot 1946), radiologist pattern recognition, musical sight-reading, basketball play schemas — same mechanism instantiated across structurally independent domains.

**CS-3 — Automatization as WM recycler (spaced practice offloads sub-skills to LTM)**
Confirmed by: N1 (irreducible 7: automatization frees WM for higher-order processing) + N2 (Cache/Memoization: memoize hot paths; Fixed Action Patterns: motor sequences require zero executive input) + N3 (Illumination 2: automatization is the bottleneck's natural adaptation mechanism; 3 independent verification instances) + N5 (Branch C: power law of practice; expertise reversal threshold) + N6 (CA-3 confidence 0.72; D-C partially confirmed in motor learning).
Unexpected symmetry: the Cache/Memoization analog predicts the automatization sequence ("automate sub-skills first, then combine") from first principles of register-file management — independently of the cognitive literature.

**CS-4 — Learner-state dependence of optimal strategy (expertise reversal)**
Confirmed by: N1 (tension T17: expertise reversal) + N3 (Illumination 5: scaffolding has real cost; schema density inversion) + N5 (Branch C.1.2: expertise reversal threshold; Branch E.1.1: high-WM learners benefit less from scaffolding) + N6 (CA-2 confidence 0.82; D-1 direct; G3 direct; Millikan flag: CLT biased toward novice data).
Cross-domain recurrence: the inversion structure (scaffolding helps novices, harms experts) recurs in procedural learning (motor training), perceptual expertise (radiology), and language acquisition (L2 grammar automation).

**CS-5 — Dual-channel independence (dual coding exploits parallel channels)**
Confirmed by: N1 (irreducible 6: phonological loop and visuospatial sketchpad empirically independent) + N2 (CTML mapping: modality separation exploits parallel stores; Fluid Dynamics: parallel sub-flows) + N5 (equation: κ_effective = κ_PL + κ_VS when dual-codable; Branch B) + N6 (D-D confirmed by multimedia learning research; CE-7 documents the failure mode: overloading both channels independently degrades rather than benefits).
Cross-domain recurrence: confirmed by Paivio dual-coding theory, Mayer's CTML, and the modality effect in multimedia learning.

**CS-6 — Domain-regularity assumption (all regularity-exploiting strategies presuppose learnable structure)**
Confirmed by: N2 (bootstrapping dependency: chunking requires prior schema; random domains invalidate compression strategies) + N5 (Branch A.2: schema precondition for learning sequence; G2 scaffolding requires learnable structure) + N6 (extreme scenario: random/noisy input collapses chunking, schema, and automatization simultaneously; G3 limited to schema-formable domains).
Cross-domain recurrence: the Information Theory analog predicts this — compression algorithms produce no gain on already-random data (incompressible strings); the constraint is domain-agnostic.

**CS-7 — Individual differences + adaptive design required**
Confirmed by: N1 (irreducible 8: WM span stable predictor of fluid intelligence and learning rates) + N3 (activation: WM_span_individual_diff a=0.49; anxiety_WM_interaction a=0.49) + N5 (Branch E: high-WM learners benefit less from scaffolding; anxiety reduces κ_effective) + N6 (CA-6 confidence 0.48: high-span learners more vulnerable to anxiety degradation; extreme scenario: population→∞ reveals no single design is universally optimal).

---

## Generalization Checks

**Holds at:** Schema-formable domains with learnable regularities during the acquisition phase (novice to proficient), where at least one parallel processing channel is available and instructional design can control extraneous load. The three-mechanism framework (compress / parallelize / offload) generalizes to any bounded-capacity sequential-processing system meeting these conditions — instructional design for any structured domain, adaptive tutoring systems, expert training programs, procedural skill acquisition in motor, perceptual, and cognitive domains. (N7 generalization check: confirmed; structural signature of a generalizable constraint, not an artifact of a single experimental tradition — 12 viable structural analogs identified in N2 domain survey.)

**Breaks at — structureless domains:** When domain has no learnable regularities (random stimuli, incoherent content), chunking fails (no structure to compress), schema formation fails (no invariant to abstract), and automatization fails (no useful unit to compile). Framework degenerates to "keep load below 4 items and use dual coding." CS-6 violated. (N6 extreme scenario: random/noisy domain; N2 bootstrapping dependency.)

**Breaks at — asymptotic expertise (Time → ∞):** At full automatization, WM is largely bypassed; CLT's acquisition-phase prescriptions (reduce extraneous load, minimize split attention) become redundant or harmful (redundancy effect). Framework does not describe expert performance in the limit. (N6 extreme scenario: Time → ∞.)

**Breaks at — clinical populations below WM threshold (WM → 0):** CLT prescriptions designed for typical WM fail below a minimum effective-capacity floor. Environmental scaffolding becomes the only viable approach; the bottleneck framework predicts failure but prescribes nothing actionable without external support. (N6 extreme scenario: WM → 0.)

**Breaks at — open-ended creative domains:** In domains where deliberate WM-search-space maintenance is required (jazz improvisation, creative writing, novel problem generation), minimizing WM load may suppress generative diversity. The framework's prescription (minimize effective load) conflicts with the requirement to maintain a rich, simultaneously active search space. (N6 inductive G1 limitation; CA-3 scope qualification.)

**Holds with qualification — declarative automatization:** Automatization's WM-freeing mechanism holds but is weaker in purely declarative domains than in procedural/motor learning. The benefit in declarative domains may operate primarily through sleep-dependent consolidation rather than WM-freeing at time of learning. (N6 CA-3 confidence 0.72; D-2; N7 divergence D-2.)

**Holds with qualification — germane load as formal variable:** The formal model includes L_G as a term, but germane load is not independently operationalizable in practice (Sweller 2010 self-critique). The model's prescriptive force reduces to "reduce L_E, observe outcome" without a measurable intermediate step. (N6 density report weakest link; N7 divergence D-3; V1 PARTIAL.)

---

## Representational Signals

V7 detected a representational frame shift:

**Input frame (N1, V7 identification):** Declarative — the original question ("What are the cognitive bottlenecks of human working memory and how do they shape learning strategies?") requests factual description and causal relationship in plain declarative form.

**Conclusion frame (N7 V7):** Mixed Symbolic + Procedural — the conclusion is expressed as a formal constraint model (L_total = L_I + L_E + L_G ≤ κ_WM ≈ 4; κ_effective = κ_PL + κ_VS; CA-1 through CA-6 with confidence scores) and as a set of design prescriptions ("exploit parallel channels," "offload sub-skills via spaced practice," "adapt to learner schema state").

**Shift significance:** Moderate. The formalization (Declarative → Symbolic) is expected and adds precision without distorting meaning. The addition of a prescriptive layer (Declarative → Procedural) exceeds the original question's scope — the question asked "how do bottlenecks shape strategies?" (descriptive-causal), and the conclusion adds "how should an instructor respond to these constraints?" (normative-procedural). This re-representation is disclosed and is more informative than the original question scope, but it requires translation back to Declarative frame for plain-language instructional guides or verbal policy statements.

**Ohlsson fixation-break mechanism:** Not applicable (N3.1 not activated; no prior fixation detected in the input).

---

## Open Questions & Next Probes

**Probe 1 — LT-WM as fourth mechanism [HIGH PRIORITY]**
Does LT-WM (Ericsson & Kintsch 1995) constitute a fourth mechanism distinct from chunk-compression, or is it subsumable under a unified "information-density-per-slot" account?
- Why it matters: If LT-WM is genuinely separate, the three-mechanism framework is incomplete. A fourth mechanism — LTM retrieval structure scaffolding — must be added. If subsumable, the framework stands.
- Discriminating test: Degrade LTM access (retroactive interference task) during expert recall of structured domain material. If expert performance collapses to novice level, LT-WM is additive to chunk-compression. If the advantage persists, chunk-compression alone explains the residual.
- Source: N6 CA-4 (Millikan flag: underweighted); N7 divergence D-1.

**Probe 2 — Germane load neural proxy [HIGH PRIORITY]**
Is the extraneous→germane→schema causal chain measurable at intermediate steps, or does germane load remain permanently unfalsifiable?
- Why it matters: If germane load is definitionally circular (as Sweller 2010 conceded), the L_G term is a placeholder with no operational content, and CLT's prescriptive force reduces to "reduce L_E, observe outcome" without a mechanism between load reduction and schema formation.
- Discriminating test: Measure fMRI/EEG activation indices of schema formation (hippocampal encoding activity, alpha suppression in schema-relevant regions) concurrently with extraneous load manipulation. If activation-rate correlates independently with L_E manipulation outcome, germane load can be given a neural proxy.
- Source: N6 density report (weakest link); N7 V1 PARTIAL; divergence D-3.

**Probe 3 — Resonant Cache dual-prediction test [MEDIUM PRIORITY]**
Does the "Resonant Cache" dual-function claim (schema pre-construction = both cache prefetch AND resonant amplification) generate empirically distinct predictions from standard advance-organizer research?
- Why it matters: Standard advance-organizer research (Ausubel) typically measures a single outcome (retention improvement). The Resonant Cache blend predicts two mechanistically independent effects: (1) reduced WM load during encoding (prefetch) and (2) deeper retrieval depth on delayed tests (resonance amplification). Neither effect alone confirms the blend; both must be elevated simultaneously vs. a no-organizer control.
- Discriminating test: Schema pre-activation condition with dual dependent measures: secondary-task WM load during learning (prefetch prediction) and cued-recall depth at 1-week delay (resonance prediction). Measure both independently; confirm both are elevated.
- Source: N2 Resonant Cache emergent structure; N7 depth score contributor.

**Probe 4 — Expertise reversal domain-regularity threshold [MEDIUM PRIORITY]**
Does the expertise-reversal effect apply in all schema-formable domains, or is there a minimum regularity threshold below which the inversion does not emerge?
- Why it matters: CA-2 and G3 both flag that expertise reversal requires learnable regularities. The minimum regularity structure for the inversion to occur is unspecified.
- Discriminating test: Compare expertise-reversal onset across domains varying in regularity (chess > mathematics > creative writing > random lists). Measure the point of worked-example inversion along the novice-to-expert continuum. A regularity-threshold curve would bound the generalization of CA-2.
- Source: N6 CA-2; G3 limitation; extreme scenario (random domain).

**Probe 5 — High-span anxiety paradox in declarative domains [LOW PRIORITY]**
Does the high-span anxiety vulnerability paradox (CA-6) generalize from procedural performance tasks (Beilock choking-under-pressure) to declarative learning tasks?
- Why it matters: CA-6 is currently supported primarily by procedural data. If the paradox does not generalize to declarative domains, adaptive instructional design targeting high-span learners under anxiety requires different interventions by domain type.
- Discriminating test: Replicate Beilock-style anxiety induction in a declarative knowledge-test condition with high-span and low-span learners. Measure performance decrement as a function of WM span.
- Source: N6 CA-6 (confidence 0.48); I-3; Millikan flag.

---

```
key_findings:     [synthesis complete]
named_entities:   []
confidence_flags: [H]
signal_flags:     []
```
