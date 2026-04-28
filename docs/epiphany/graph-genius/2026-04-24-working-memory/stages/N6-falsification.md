# N6 — Falsification (ADVERSARIAL)
## Executed: 2026-04-24 | Scale: STANDARD | Hat: Popper+Millikan
## scale_gate: STANDARD

<falsification>

<inference_modes>

<abductive>

## Abductive mode results

**Anomaly identified (Step A1):**
The most surprising element: despite working memory being a fixed 3–4 chunk architectural constraint, skilled learners demonstrably handle far more complexity than that limit would predict — yet the limit never changes. If the ceiling is real, why does expertise appear to transcend it without raising it?

**Candidate explanations generated (Step A2):**

**H-1 — Chunking is the primary mechanism (Chunk-Compression Hypothesis)**
IF hierarchical compression of primitives into higher-order chunks is the dominant mechanism, THEN complexity growth in expertise is explained without violating the slot ceiling. The anomaly resolves: the ceiling holds, but slot contents are denser.
- Parsimony: 0.90 (single mechanism, already empirically established)
- Explanatory power: 0.80 (explains expert–novice gap; less clear on why chunking forms at the rate it does)
- Uberty: 0.65 (also explains language comprehension, musical reading, pattern recognition)
- Testability: High — falsification condition: if experts' recall from brief exposure is NOT superior to novices when controlling for familiarity (random chess boards vs. game positions), the chunking account fails. (De Groot 1946 already tested; Ericsson replications exist.)

**H-2 — LTM-as-WM Extension (Ericsson & Kintsch Long-Term WM Hypothesis)**
IF expert performance operates by building retrieval structures in LTM that can be accessed as if they were in WM (via retrieval cues held in the small WM buffer), THEN experts effectively extend their WM without increasing chunk count. The anomaly: the "4 slots" are real, but they contain cue pointers to large LTM structures, not raw information.
- Parsimony: 0.70 (introduces new construct: LT-WM retrieval structure)
- Explanatory power: 0.85 (explains expert behavior in chess, text recall, medical diagnosis)
- Uberty: 0.75 (extends to all deliberate practice domains)
- Testability: High — falsification condition: if degrading LTM access (e.g., interference task during expert recall) collapses expert performance to novice level, LT-WM hypothesis is supported; if it does not, chunking-only account survives.

**H-3 — Central Executive Efficiency Hypothesis**
IF training does not increase slot count but does improve executive routing efficiency (faster schema selection, reduced attentional switching cost), THEN experts appear to handle more because the executive operates faster on the same 4 slots, not because they have more slots.
- Parsimony: 0.75 (single efficiency parameter)
- Explanatory power: 0.60 (explains speed differences; harder to explain the qualitative structure of expert recall)
- Uberty: 0.55 (applies to dual-task performance, attentional control training)
- Testability: Medium — falsification condition: if reaction time in executive control tasks does NOT decrease with expertise in a domain, the efficiency account is undermined.

**H-4 — Embodied/Environmental Scaffolding Offload**
IF experts routinely offload WM content to the environment (notes, gesture, workspace organization, external representations), THEN what looks like expanded WM is actually a human-environment cognitive system. The individual WM limit is unchanged; effective capacity scales with scaffolding.
- Parsimony: 0.65 (requires external-scaffold variable)
- Explanatory power: 0.55 (explains laboratory-naturalistic discrepancy; less useful in controlled recall tasks)
- Uberty: 0.50 (applies broadly in distributed cognition literature)
- Testability: High — falsification condition: if expert advantage disappears when all external aids are removed (clean-desk experiment), the scaffolding account is supported; if the advantage persists, it fails.

**H-5 — Attentional Filtering Sharpening**
IF expertise sharpens early attentional filtering so irrelevant items never enter WM (they are inhibited before encoding), THEN the effective load is lower for experts on equivalent tasks — not because they have more capacity, but because they load less.
- Parsimony: 0.80 (attentional selection mechanism)
- Explanatory power: 0.65 (explains faster encoding; harder to explain the hierarchical structure of expert chunk recall)
- Uberty: 0.60 (applies to perceptual expertise in radiology, chess, music reading)
- Testability: Medium — falsification condition: if experts show equivalent early neural load markers (N2pc, MMN amplitude) to novices on domain-relevant stimuli, filtering sharpening is undermined.

**Step A4 — Top 3 ranked by combined score:**

| Hypothesis | Parsimony | Explan. | Uberty | Test. | Combined |
|---|---|---|---|---|---|
| H-2 (LT-WM Extension) | 0.70 | 0.85 | 0.75 | High | **2.30** |
| H-1 (Chunk-Compression) | 0.90 | 0.80 | 0.65 | High | **2.35** |
| H-3 (Executive Efficiency) | 0.75 | 0.60 | 0.55 | Med | **1.90** |
| H-5 (Attentional Filtering) | 0.80 | 0.65 | 0.60 | Med | **2.05** |

H-4 — combined score: 0.65+0.55+0.50 = **1.70** — below development threshold.
`H-4 — Environmental Scaffolding Offload: combined score 1.70 — below development threshold, reasoning used for consolidation only.`

**Top 3 abductive candidates:**
1. **H-1 — Chunk-Compression** | combined: 2.35 | falsification: novice/expert parity on random (non-chunked) stimuli
2. **H-2 — LT-WM Extension** | combined: 2.30 | falsification: LTM interference collapses expert advantage
3. **H-5 — Attentional Filtering** | combined: 2.05 | falsification: equivalent early neural load in experts and novices

</abductive>

<deductive>

## Deductive mode results

**Step D1 — Premises from N1 irreducibles (serving as the deductive base; N5 not used per module note):**

- P1: WM item capacity ≈ 3–4 meaningful chunks (Cowan 2001, empirical)
- P2: Phonological loop decays in ~2 seconds without rehearsal (Baddeley 1974)
- P3: Central executive has limited capacity for attentional control; cannot fully multi-process
- P4: Chunking compresses multiple primitives into one chunk — circumvents item count, does not remove the limit
- P5: Extraneous cognitive load competes with germane load (Sweller CLT)
- P6: Dual coding exploits channel independence; loading verbal and visuospatial simultaneously partially doubles effective capacity
- P7: Automatization offloads procedural sub-skills from WM to LTM via spaced repetition
- P8: WM span is a stable individual-differences predictor of fluid intelligence

**Step D2 — Derive consequences:**

*Inference D-A:* P1 + P4 →
Premise: Total slot count is fixed at ~4. Premise: Chunking fills slots with higher-order units.
Therefore: **The maximum number of manageable primitives scales with chunk hierarchy depth, not with slot count.** Validity: valid (transitivity).
→ Consequence: Novice-to-expert progression should be characterized by increased chunk hierarchy depth, not by anything else.

*Inference D-B:* P3 + P5 →
Premise: Central executive capacity is limited. Premise: Extraneous load competes with germane load.
Therefore: **Any instructional design that adds extraneous load to a task at the central-executive level will reduce learning, not improve it.** Validity: valid (modus ponens from "competing for same resource").
→ Consequence: Worked examples should outperform problem-solving during initial learning (reduced extraneous load = more germane capacity).

*Inference D-C:* P7 + P1 →
Premise: Automatization offloads sub-skills to LTM. Premise: Capacity is ~4 chunks.
Therefore: **Spaced repetition's benefit is specifically mediated by freeing WM capacity, not by any direct encoding benefit.**
→ Consequence: Spaced practice should produce a measurable reduction in concurrent WM task performance degradation (i.e., dual-task cost should decrease with automatization). 
[Test: YES — well-supported in motor learning; PARTIAL in declarative learning]

*Inference D-D:* P6 + P1 →
Premise: Dual coding uses independent channels. Premise: Total WM is ~4 chunks (per modality independently).
Therefore: **Dual-coded instruction has a ceiling: if both channels are separately overloaded, dual coding provides no benefit or harm.**
→ Consequence: Multimedia that separately overloads both verbal and visuospatial channels should produce worse outcomes than single-channel instruction.
[Test: YES — split-attention and modality effects confirm this; matches CTML predictions]

*Inference D-E:* P8 + P5 →
Premise: WM span predicts learning outcomes. Premise: Extraneous load competes with germane.
Therefore: **Low-WM-span learners should benefit MORE from extraneous load reduction than high-span learners.**
→ Consequence: Instructional interventions that reduce extraneous load should show differential benefit (interaction effect: low-span × reduced-load > high-span × reduced-load).
[Test: YES — empirically confirmed in several CLT studies; validity: HIGH]

**Step D3 — Consequence testing:**
- D-A: Holds (chess expertise research supports hierarchy depth as primary change)
- D-B: Holds at initial learning; FAILS for experts (expertise reversal: worked examples become harmful)
- D-C: Partial — holds in procedural/motor domain; evidence weaker in purely declarative domains
- D-D: Holds (confirmed by multimedia learning research)
- D-E: Holds (confirmed, high-span learners less sensitive to extraneous load manipulations)

**Step D4 — Flagged premises and alternative hypotheses from failures:**

*D-B failure for experts:* P5 ("extraneous load competes with germane") may be **incomplete as stated** — it assumes extraneous load is always harmful. The failure at expertise level implies:
→ **Alternative D-1:** The premise P5 should be restricted to the condition "novice learner, undeveloped schema." For experts, the same design element that was extraneous becomes redundant and triggers a different cost (redundancy effect). P5 is domain-valid but learner-state-dependent.

*D-C partial failure in declarative domain:* P7 ("automatization frees WM") may over-generalize — in declarative knowledge, the "offloading" mechanism is retrieval-based rather than procedural, and the WM-freeing effect may be weaker.
→ **Alternative D-2:** Automatization's WM-freeing mechanism operates primarily in procedural/skill domains. In declarative domains, spaced repetition benefits may operate through a different mechanism (consolidation during sleep, not WM-freeing at time of learning).

**Alternative hypotheses from deductive mode:**
- D-1: CLT's extraneous load principle is learner-state-dependent — harmful for novices, sign-reversed for experts
- D-2: Automatization's WM benefit is domain-type-specific (procedural > declarative)

</deductive>

<inductive>

## Inductive mode results

**Step I1 — Observed instances from N1 and N3:**

I1a: Chess masters (de Groot 1946) recall full game positions after 5s exposure; fail on random boards.
I1b: Arithmetic fluency — single-digit addition becomes procedural; adult WM freed for algebra.
I1c: Reading comprehension — beginning readers load WM with decoding; fluent readers decode automatically, freeing WM for meaning-making.
I1d: Medical diagnosis (radiology) — expert radiologists encode disease patterns as single perceptual chunks.
I1e: Musical sight-reading — expert musicians process multi-note phrases as single rhythmic gestures.
I1f: Basketball plays — team tactics stored as named schemas by experienced players.
I1g: Language production — novice L2 speakers occupy WM with grammar rules; fluent speakers speak automatically.
I1h: Anxiety during math tests depletes WM capacity (Ashcraft & Kirk 2001 pattern).
I1i: Worked examples reduce WM load for novices; become redundant/harmful for experts.
I1j: Desirable difficulties (interleaving) worsen short-term performance but improve long-term retention.

*10 instances. Induction proceeds.*

**Step I2 — Generalizations:**

**G1 — Domain-specific automatization is universal across expertise acquisition**
"Across all observed instances of expertise (chess, arithmetic, reading, radiology, music, basketball, language), the pattern 'bottleneck-critical sub-skills become automatic' appears because repeated retrieval re-encodes procedural sequences as LTM schemas, continuously freeing WM."
→ Extension beyond instances: this should hold in any domain where sub-skills can be decomposed and practiced in isolation.

**G2 — The WM bottleneck is always present; expertise changes information density per slot, not slot count**
"Across all observed instances, experts show superior recall on structured (meaningful) stimuli but not on random stimuli — consistently. The pattern appears because chunking exploits domain regularities, and random stimuli provide no chunking opportunities."
→ Extension: expertise advantage should collapse whenever domain regularities are removed (random stimuli, novel domains).

**G3 — Optimal learning strategy is a function of learner schema state, not of task difficulty alone**
"Across all observed instances where strategy effectiveness was measured at multiple learner levels (worked examples vs. problem solving, spaced vs. massed practice, scaffolded vs. desirable difficulty), the pattern 'optimal strategy inverts as schema density increases' appears because the bottleneck changes in character — for novices it is slot count; for experts it is redundancy tolerance."
→ Extension: any one-size-fits-all instructional design will be suboptimal for at least one learner population.

**G4 — Emotional states (anxiety) that compete for central executive resources reduce effective WM capacity**
"In instances involving anxiety (math anxiety, test anxiety), WM available for task processing decreases. The pattern appears because anxiety-related intrusive thoughts occupy central executive resources, leaving fewer attentional resources for task."
→ Extension: any sufficiently demanding concurrent cognitive task should replicate the anxiety effect (cognitive competition, not emotion-specific).

**Step I3 — Boundary case testing:**

*G1 boundary test:* "Domain-specific automatization is universal" — boundary case: creative domains where variability is rewarded (jazz improvisation, creative writing). Does the automatization of sub-skills produce the same WM-freeing effect when top-level processing ALSO requires novelty?
→ Result: G1 holds for sub-skills (harmonic patterns, grammar), but the top-level creative generation may require deliberately maintaining WM-intensive search. **G1: ROBUST for sub-skill layer; LIMITED TO sub-skill layer in high-creativity domains.**

*G2 boundary test:* "Expertise advantage collapses on random stimuli" — boundary case: experts with strong perceptual discrimination skills who may have developed domain-general chunking meta-skills.
→ Result: Literature supports G2 (de Groot random board replication; Chase & Simon 1973). The collapse on random stimuli is documented. **G2: ROBUST.**

*G3 boundary test:* "Optimal strategy inverts as schema density increases" — boundary case: a domain with extremely low regularity (e.g., pure memorization of random lists). No schemas possible; does the expertise reversal still occur?
→ Result: If there is no schema to form, the worked-examples/guided-discovery inversion cannot occur because there is no expertise state to reverse into. G3 requires that the domain permit schema formation. **G3: LIMITED TO schema-formable domains.**

*G4 boundary test:* "Emotional competition is equivalent to cognitive competition" — boundary case: high-WM-span individuals under anxiety. Do high-span individuals buffer the anxiety effect?
→ Result: Evidence is mixed. Some studies show high-span individuals suffer more from anxiety (they typically rely on WM more than low-span individuals who may use non-WM strategies by default). **G4: LIMITED TO — effect may be paradoxically stronger in high-span learners.**

**Step I4 — Alternative hypotheses from failed/limited generalizations:**

From G1 limitation:
→ **Alternative I-1:** In high-creativity/open-ended domains, automatization of sub-skills is necessary but not sufficient for expert performance — the bottleneck may deliberately NOT be reduced at the generative level. Implication: instructional models that aim to minimize all WM load may inadvertently suppress creative processing.

From G3 limitation:
→ **Alternative I-2:** The expertise-reversal effect is domain-type-dependent: it requires that the domain have learnable regularities (schema-formable). In low-regularity domains, the novice-to-expert progression follows a different curve — not the inverted-strategy model but a sustained high-load acquisition model.

From G4 limitation:
→ **Alternative I-3:** Anxiety–WM interactions are moderated by WM span in a non-intuitive direction: high-span learners may be MORE vulnerable to anxiety degradation of performance because they have more to lose when executive resources are competed for. One-size-fits-all anxiety reduction interventions may be less effective for high-span learners than expected.

</inductive>

</inference_modes>

<consolidated_hypotheses>

## Consolidated hypotheses

**Step C1 — All candidates collected:**
- Abductive: H-1 (Chunk-Compression), H-2 (LT-WM Extension), H-3 (Executive Efficiency), H-5 (Attentional Filtering), H-4 (Environmental Scaffolding — below threshold)
- Deductive: D-1 (CLT extraneous-load principle is learner-state-dependent), D-2 (Automatization WM benefit is domain-type-specific — procedural > declarative)
- Inductive: I-1 (Automatization insufficient in high-creativity domains), I-2 (Expertise-reversal limited to schema-formable domains), I-3 (High-span learners more vulnerable to anxiety degradation)

**Step C2 — Deduplication and merging:**
- H-1 (Chunk-Compression) + abductive framing of G2 (expertise advantage on structured stimuli): structurally equivalent — merge as **CA-1**.
- D-1 (CLT principle is learner-state-dependent) + G3 (inductive expertise-reversal scope) + H-2 partial overlap: D-1 and G3 share the "strategy inversion at schema formation" claim — merge as **CA-2**.
- D-2 (Automatization domain-specific) + I-1 (insufficient in creativity domains): both restrict automatization's WM benefit to a domain type — merge as **CA-3**.
- H-2 (LT-WM Extension) is distinct from CA-1 — it proposes a positive mechanism (LTM retrieval structures) not just compression. Retain as **CA-4**.
- H-5 (Attentional Filtering) is not reducible to any of above — retain as **CA-5**.
- I-3 (High-span anxiety vulnerability) is independent — retain as **CA-6**.

**Step C3 — Cross-mode validation:**

| Candidate | A | D | I | Supporting modes |
|---|---|---|---|---|
| CA-1: Chunking as primary mechanism | YES (H-1) | YES (D-A consequence) | YES (G2 robust) | **3 modes** |
| CA-2: CLT extraneous-load principle is learner-state-dependent | YES (expertise-reversal touches H-2) | YES (D-1 direct) | YES (G3 direct) | **3 modes** |
| CA-3: Automatization WM benefit is domain-type-specific | YES (implies LT-WM) | YES (D-2 direct) | YES (I-1 + G1 limitation) | **3 modes** |
| CA-4: LT-WM Extension (Ericsson-Kintsch) | YES (H-2 direct) | INDIRECT (D-A) | PARTIAL (G1) | **2 modes** |
| CA-5: Attentional Filtering Sharpening | YES (H-5 direct) | NO | PARTIAL (G2: random stimuli) | **1.5 modes** |
| CA-6: High-span anxiety vulnerability | NO | NO | YES (I-3 direct) | **1 mode** |

**Step C4 — NLI confidence scores:**

- CA-1: 0.85 — highest support; three independent modes; empirically grounded
- CA-2: 0.82 — three modes; empirically supported (expertise reversal literature)
- CA-3: 0.72 — three modes; some empirical support (motor vs. declarative automatization gap)
- CA-4: 0.65 — two modes; empirically proposed (Ericsson & Kintsch 1995) but contested
- CA-5: 0.50 — single primary mode; plausible but less-tested mechanism
- CA-6: 0.48 — single inductive mode; some empirical support but surprising prediction

**Step C5/C6 — Final ranked alternative hypotheses (≥3):**

1. **CA-1 — Chunk-Compression as primary expertise mechanism** | confidence: 0.85 | modes: A+D+I | falsification: expert advantage should disappear on random stimuli with no domain-regular structure; random-board experiments are the critical test
2. **CA-2 — CLT extraneous-load principle is learner-state-dependent** | confidence: 0.82 | modes: A+D+I | falsification: if extraneous-load reduction helps experts as much as novices, the state-dependence claim fails
3. **CA-3 — Automatization WM benefit is domain-type-specific (procedural >> declarative)** | confidence: 0.72 | modes: A+D+I | falsification: if declarative spaced repetition produces equivalent dual-task cost reduction to motor learning, domain-specificity claim is undermined
4. **CA-4 — LT-WM Extension via LTM retrieval structures** | confidence: 0.65 | modes: A+D (indirect) | falsification: LTM interference task during expert recall should collapse expert advantage if LT-WM hypothesis is correct; if advantage persists, hypothesis fails
5. **CA-5 — Attentional Filtering Sharpening** | confidence: 0.50 | modes: A+(partial I) | falsification: experts should show reduced early neural load markers (N2pc, MMN) on domain-relevant stimuli vs. novices

*6 distinct alternatives. Count ≥ 3 satisfied. S6_no_alternatives NOT raised.*

</consolidated_hypotheses>

<extreme_scenarios>

## Extreme scenarios

**Primary conclusion under test:** The core conclusion from N1+N3 is: "WM bottlenecks (3–4 chunks, phonological decay, central executive limits) directly shape learning strategies via CLT, and strategies like chunking, automatization, dual coding, and metacognition succeed precisely because they address specific bottleneck constraints."

**Checklist 1 applied — 8 boundary regimes:**

| Regime | Scenario | Claim behavior | What failure reveals |
|---|---|---|---|
| **Parameter → 0: WM capacity → 0** | Pathological case: learner with ~0 effective WM (severe WM impairment, e.g., severe ADHD, Alzheimer's onset) | Claim still holds — all WM-based strategies fail; external scaffolding becomes the only viable approach | Reveals: the bottleneck framework only provides a floor, not a ceiling; below some threshold, the framework predicts failure but prescribes nothing actionable without environmental scaffolding |
| **Parameter → ∞: WM capacity → very large** | Hypothetical: eidetic-memory individual with ~40-chunk effective WM | Claim degenerates — strategy prescriptions (chunking, minimize extraneous load) lose urgency; desirable difficulties would still produce LTR benefit through retrieval mechanisms independent of WM constraint | Reveals: CLT-derived prescriptions are bottleneck-dependent; for high-WM individuals, the framework's utility collapses to the retrieval-practice prediction only |
| **Population → 1: single learner** | A single learner — does the claim hold? | YES — holds trivially; the WM architecture claim is individual-level | N/A — degenerate case that is the intended scope |
| **Population → ∞: universal instruction** | All human learners simultaneously; universal instructional design | Claim partially fails — individual differences (WM span, anxiety, domain) make no single design optimal; the claim about "strategies shaped by bottlenecks" holds but the prescription becomes underdetermined | Reveals: the framework is a constraint language, not a unique prescription generator |
| **Time → 0: immediate (single trial, no practice)** | One exposure, no repetition — do bottleneck constraints apply? | YES — but only the encoding bottleneck (item count, phonological decay). The automatization and spaced-repetition prescriptions are inoperative. Dual coding still applies. | Reveals: the framework has two distinct timescale regimes: (1) encoding (immediate, WM constraints apply directly) and (2) consolidation (session/week, LTM mechanisms dominate). Only encoding-phase prescriptions hold at Time → 0 |
| **Time → ∞: asymptotic expertise** | A learner who has practiced for 10,000 hours in a domain | Claim partially inverts — at extreme expertise, the bottleneck-routing through WM is largely bypassed (near-total automatization); the framework's prescriptions (reduce extraneous load, minimize split attention) become irrelevant or harmful (redundancy effect) | Reveals: the claim's validity envelope is bounded to the acquisition phase; it does not describe expert performance in the limit |
| **Adversarial: designer with perfect knowledge actively maximizes extraneous load** | Adversarial instructional design: maximize distractors, split attention, redundancy | Claim holds and predicts catastrophic failure — maximum extraneous load should produce near-zero learning | This boundary case is not adversarial to the claim — it confirms it. The real adversarial test is: can you teach effectively despite high extraneous load by training learners to filter it (attentional filtering)? That's NOT predicted by CLT |
| **Random/noisy: inputs are random noise** | Learner presented with purely random stimuli (random digit strings, incoherent sentences) | Claim partially holds: phonological decay and item count constraints still apply. BUT chunking strategy FAILS entirely (no regularities to chunk). Schema formation FAILS. Automatization FAILS to produce useful units. Metacognition cannot regulate load productively. | Reveals: all regularities-exploiting strategies (chunking, schema, automatization) are parasitic on domain structure; in structureless domains, the entire CLT prescription collapses to "keep load below 4 items and use dual coding" |

**Boundary regime summary:**
- Claim ROBUST in normal parameter range
- Claim DEGENERATES at WM → 0 (strategy list reduces to environmental scaffold only)
- Claim LOSES PRESCRIPTIVE POWER at WM → ∞ and Time → ∞ (expertise ceiling)
- Claim has a HIDDEN DOMAIN-REGULARITY ASSUMPTION: structureless domains invalidate the most powerful prescriptions (chunking, schema, automatization)
- The framework contains two implicit time-regime components that are usually conflated: encoding bottleneck (immediate) vs. consolidation benefit (long-horizon). These should be treated as separate claim classes.

</extreme_scenarios>

<millikan_assessment>

## Millikan assessment

**Primary conclusion:** CLT/WM bottleneck theory and its associated learning strategies (chunking, dual coding, spaced repetition, metacognition, desirable difficulty) are well-founded, and the bottleneck framework causally explains strategy effectiveness.

---

**[MILLIKAN] CA-2 — CLT extraneous-load principle is learner-state-dependent:**
- Effort to refute: LOW — expertise reversal is a well-documented empirical phenomenon; it directly challenges the unconditional CLT extraneous-load claim with real data.
- Effort to support primary (unconditional CLT): MEDIUM — the primary account must add the caveat "for novices" to survive.
- Asymmetry: YES — refuting CA-2 (i.e., defending unconditional CLT) requires more argumentative work than accepting CA-2.
- Additional counter-evidence to seek: Are there conditions where the expertise reversal effect itself reverses? (i.e., very high domain complexity where even experts benefit from guided instruction?)
- **Millikan flag: the original CLT account is biased toward novice learner data; expertise-reversal evidence is under-weighted in popular presentations of CLT.**

**[MILLIKAN] CA-3 — Automatization WM benefit is domain-type-specific:**
- Effort to refute: MEDIUM — the declarative domain evidence is weaker, but some spaced-repetition WM-freeing evidence exists in vocabulary learning.
- Effort to support primary (universal automatization): MEDIUM
- Asymmetry: SLIGHT YES — the primary account treats automatization as universally WM-freeing; the domain-specificity challenge requires the primary account to search for declarative-domain WM-freeing evidence that it has not systematically collected.
- Additional counter-evidence to seek: Does vocabulary spaced repetition produce measurable dual-task interference reduction? If YES, CA-3 is undermined.

**[MILLIKAN] CA-4 — LT-WM Extension (Ericsson-Kintsch):**
- Effort to refute: HIGH — LT-WM is a full competing theory with its own empirical program (text recall, chess, medical expertise). Refuting it requires engaging with Ericsson & Kintsch (1995) directly.
- Effort to support primary (chunking-only): MEDIUM
- Asymmetry: YES (in the other direction) — the reasoning worked HARDER to dismiss LT-WM than it did to establish chunking. This is a Millikan flag in reverse: the primary conclusion may be preferred over LT-WM with insufficient justification.
- Additional counter-evidence to seek: LT-WM interference experiments; expert performance when LTM access is specifically degraded. Do experts recall better from mid-game positions than the pure chunking account predicts?
- **Millikan flag: LT-WM is treated as an extension of chunking when it may be a genuinely separate and competing mechanism. Underweighted.**

**[MILLIKAN] CA-6 — High-span anxiety vulnerability paradox:**
- Effort to refute: LOW — the counter-intuitive direction (high-span MORE vulnerable) is surprising; standard reasoning would quickly dismiss it as "high-span should buffer anxiety."
- Effort to support primary (high-span buffers anxiety): LOW — this is the assumption, not a tested claim.
- Asymmetry: YES — the primary assumption (high-span buffers anxiety) was never examined for disconfirming evidence. Dismissing CA-6 requires effort; supporting the standard assumption required no effort at all.
- Additional counter-evidence to seek: Ashcraft studies; Beilock "choking under pressure" research (explicitly: high-skilled performers choke more under pressure because they rely on WM-mediated control that is anxiety-disrupted).
- **Millikan flag: the standard assumption (high WM span buffers anxiety) is not empirically secure and is contradicted by "choking under pressure" literature (Beilock 2010).**

**Overall Millikan assessment:**
Three asymmetries detected (CA-2, CA-4, CA-6). The primary CLT account shows signs of:
1. Novice-learner data bias (CA-2 underweighted)
2. LT-WM mechanism underweighted vs. chunking-only
3. High-span anxiety paradox assumed away rather than tested

These are not fatal to the framework but indicate that the standard presentation of the CLT-derived prescriptions requires three precision qualifications.

</millikan_assessment>

<density_report>

## Density report

**Darwin's golden rule applied — active disconfirming-evidence search:**

**1. Known counter-examples to the primary conclusion:**

CE-1: WM training (N-back) studies — if the WM bottleneck is fixed and strategies work only by routing around it, WM training that genuinely increases capacity would partially falsify the "fixed architectural ceiling" claim. Controversy: Jaeggi et al. (2008) reported WM training transfer to fluid intelligence; subsequent replications largely failed. Counter-evidence is mixed — the ceiling is likely real, but the degree of plasticity is contested.

CE-2: Desirable difficulty paradox — CLT predicts minimizing extraneous load = maximizing learning. Desirable difficulties INCREASE load in the short term yet produce superior LTR. This is internally inconsistent with the simplest reading of CLT unless the "germane load" construct is invoked (which Sweller later distanced from CLT, calling it unfalsifiable in its original form). The desirable difficulty phenomenon is a documented anomaly for CLT.

CE-3: Beilock "choking under pressure" — highly skilled performers (high WM span) show performance degradation under high-pressure conditions. The standard CLT account predicts that higher WM span = better performance under load; choking shows the reverse when attention is explicitly directed to automatized procedures. This is a direct counter-example to the simple WM-span → performance linearity claim.

CE-4: The "activated LTM" account (Ruchkin et al., Jonides et al.) argues that WM is not a separate store but is simply activated portions of LTM. If correct, the "4-slot" model is a metaphor, not a separate architectural subsystem — this would require reframing all CLT prescriptions in terms of activation strength rather than slot count.

CE-5: The curse of the expert — highly chunked schemas can impede flexible learning of new material (Einstellung effect in chess). Expert chunks, once formed, can suppress recognition of novel patterns that do not fit the schema. This directly contradicts "more chunking = always better."

CE-6: Individual variation in phonological loop decay — the ~2s decay is a population average; there is substantial individual variability. Low phonological loop capacity individuals may require different encoding strategies that are not derivable from the average-person WM model.

CE-7: Dual coding limits — Sweller and colleagues have documented the modality effect failure under conditions where both channels are overloaded by complex material simultaneously. If both channels are sufficiently loaded, dual coding harms rather than helps — contradicting the simple "dual coding doubles capacity" framing.

**2. Classes of problem where the primary approach fails:**
- Random/structureless domains (see extreme scenarios): all regularity-exploiting strategies fail
- Open-ended creative domains: minimizing WM load may suppress generative diversity
- Clinical populations (severe ADHD, dementia): CLT prescriptions designed for typical WM fail entirely below some threshold
- Highly emotional/social learning contexts: CLT is cognitively focused; social motivation, identity threat, and emotional regulation are outside its explanatory scope

**3. Theoretical objections:**
- Cowan's "focus of attention" model (1 item maximally activated at a time) implies the real bottleneck is more severe than "3–4 chunks" — it is 1 item in the attentional spotlight. CLT's ~4 estimate may overstate available capacity.
- The activated-LTM theory (Ruchkin 2003) challenges the storage-system ontology underlying WM models — if WM is activation, not storage, then CLT's prescriptions may be correct in practice but theoretically misfounded.

**4. Literature that cuts against:**
- Germane load concept widely criticized as circular (Sweller 2010 retreat) — if it is defined as "whatever leads to schema formation," it cannot be independently measured and the CLT three-component model loses falsifiability.
- WM training transfer failure meta-analyses (Melby-Lervåg & Hulme 2013; Melby-Lervåg, Redick, & Hulme 2016) — challenge whether WM capacity is plastic at all; if true, the bottleneck is more rigid than CLT prescriptions imply (strategies become more important, not less).

**5. Weakest link in mechanism:**
The causal chain from "extraneous load reduced" → "germane load increased" → "schema formation accelerated" is the weakest link. Germane load is not independently measurable; the mechanism by which freed-up WM becomes productive schema formation (rather than mind-wandering or idle rehearsal) is not specified. This leaves a gap between the capacity-freeing intervention and the learning outcome.

**Density report:**
```
Disconfirming evidence found: 7+ items
Counter-examples: CE-1 (WM training controversy), CE-2 (desirable difficulty CLT tension), CE-3 (Beilock choking), CE-4 (activated LTM theory), CE-5 (Einstellung effect), CE-6 (phonological variability), CE-7 (dual coding overload)
Failure classes: random/structureless domains; clinical populations below WM threshold; creative/open-ended domains; emotional/social learning contexts
Strongest objection: Germane load is unfalsifiable as originally formulated (Sweller 2010 self-critique), which means CLT's three-component model loses predictive precision at the critical juncture between load reduction and learning gain
Weakest link in mechanism: extraneous-load reduction → germane load activation → schema formation (the last two steps are not independently measurable and not causally specified)
Density assessment: HIGH (7+ disconfirming items)
```

**Cargo-cult scan:**
- No instances of "one could imagine a case..." without specification found.
- No instances of "further research needed" used as falsification — each disconfirming item names a specific mechanism, study, or documented effect.
- CE-4 (activated LTM) flags the ontological assumptions of WM models; this is genuine rather than vague.
- All boundary cases in extreme scenarios name specific conditions and mechanisms.
- Cargo-cult scan: **CLEAN.**

</density_report>

</falsification>

---

## SIGNAL OUTPUT

```
key_findings:
  - Consolidated 6 distinct alternative hypotheses (CA-1 through CA-6); all 3 inference modes produced alternatives; S6_no_alternatives NOT raised
  - CA-1 (Chunk-Compression) confidence 0.85: primary mechanism; robust across all 3 inference modes; falsified by random-stimuli parity
  - CA-2 (CLT extraneous-load is learner-state-dependent) confidence 0.82: expertise reversal directly challenges universal CLT; strategy prescriptions must be conditioned on learner schema state
  - CA-3 (Automatization WM benefit is domain-type-specific: procedural > declarative) confidence 0.72: inductive G1 + deductive D-2 converge; creative domains require deliberate WM maintenance
  - CA-4 (LT-WM Extension, Ericsson-Kintsch) confidence 0.65: Millikan flag — underweighted vs. chunking-only; LTM retrieval structures may be mechanism distinct from chunk compression
  - CA-5 (Attentional Filtering Sharpening) confidence 0.50: experts may load less, not just more efficiently; single-mode support
  - CA-6 (High-span anxiety vulnerability paradox) confidence 0.48: Beilock choking-under-pressure literature directly confirms; counter-intuitive direction; standard assumption (high-span buffers anxiety) is empirically weak
  - Extreme scenarios: claim degenerates at WM→0, WM→∞, Time→∞, and structureless domains; hidden domain-regularity assumption identified
  - Millikan flags: CLT biased toward novice data (CA-2); LT-WM underweighted (CA-4); high-span anxiety assumption untested (CA-6)
  - Density: HIGH — 7 counter-examples found; weakest link is extraneous→germane→schema causal chain (not independently measurable); germane load criticized as unfalsifiable by Sweller himself
named_entities:
  - CA-1 Chunk-Compression, CA-2 learner-state-dependent CLT, CA-3 domain-type-specific automatization, CA-4 LT-WM Ericsson-Kintsch, CA-5 attentional filtering, CA-6 high-span anxiety paradox
  - Beilock (choking under pressure), Ericsson & Kintsch (1995 LT-WM), Sweller (2010 germane load retreat), Melby-Lervåg et al. (WM training meta-analysis), Ruchkin (activated LTM)
confidence_flags: [H, H, H, M, M, L]
signal_flags: []
```
