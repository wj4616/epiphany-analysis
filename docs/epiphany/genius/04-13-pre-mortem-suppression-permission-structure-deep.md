---
stage_id: OSP
stage_name: Output Synthesis Pass
module_version: 1.1.0
status: complete
source_session: 20260413-190845
---

> v1.1.0 · Scale: DEEP · Pipeline: complete at 7 of 7 planned · Active conditionals: none (S3.1 not activated; S6.1 not activated) · For raw state, re-invoke with --xml

---

## Headline Insight

The Pre-Mortem retrospective technique's documented effectiveness — Klein's 30% improvement in failure reason-identification — is most parsimoniously explained as the *removal of social suppression barriers* (voice suppression, hierarchy, optimism norms) that cause teams to under-report known risks under baseline conditions, rather than as a genuine cognitive prospective-hindsight effect that generates new information. The technique's primary structural mechanism is therefore a *permission structure for orphan knowledge* (S3 IL2), not a cognitive enhancement of individual reasoning. This conclusion yields four structural predictions not available in the original guide: (1) effectiveness is inversely proportional to pre-existing psychological safety; (2) the Cassandra gap — detection without uptake — is the technique's most dangerous unaddressed failure mode; (3) the voting mechanism is structurally biased and unfixable by facilitation; (4) certainty framing has a dose-response ceiling that the guide's instruction to maximize vividness ignores.

Confidence: **MODERATE** — four independent convergence signals from six stages support HIGH territory, but one unresolved logic gap (Klein's control condition, V1 PARTIAL) and one unresolved divergence (H-C1 vs. H-C3 as primary mechanism) reduce the rating.

**Creativity type:** Exploratory

Rationale (S7): The conclusion does not violate foundational constraints of cognitive science, organizational behavior, or risk management theory, and does not introduce a new conceptual space. It identifies unexplored corners of the Pre-Mortem's possibility space (suppression-release mechanism, Cassandra gap, vote as structural measurement error) that were theoretically accessible within existing frameworks but unmapped by the guide or the mainstream Pre-Mortem literature. The conceptual space's rules remain intact; the insight explores rather than transforms.

---

## Theory Collisions

Source: S4 Dynamic Simulation — four theory collisions with discriminating conditions.

**Collision 1 — Psychological safety created by the technique vs. suppressed by the organization:**
- Claim A (S1 Irreducible #7): The certainty frame transforms the social environment; participants voice concerns they would suppress in a normal meeting.
- Claim B (S1 Tension #2): The same power differentials that suppress concerns normally remain active; framing provides surface-level permission but does not change the underlying social calculus.
- Discriminating condition: Run matched Pre-Mortems in high-hierarchy vs. low-hierarchy teams; measure whether concerns voiced in high-hierarchy Pre-Mortems are novel (not previously surfaced in planning meetings) or merely recapitulations. If novel concern rate in high-hierarchy teams is not significantly higher than in normal meetings, Claim B governs.

**Collision 2 — Individual independence vs. pre-session anchoring:**
- Claim A (S1 Cat2 item 16): Step 2 individual card-writing prevents anchoring.
- Claim B (S1 Cat7 item 12): Prior group discussion in project kickoff meetings has already introduced anchoring before Step 2 begins.
- Discriminating condition: Compare failure card cosine similarity within teams running a Pre-Mortem as first structured discussion vs. teams with prior planning meetings. High within-team similarity in the latter group confirms Claim B.

**Collision 3 — Reason-identification gain vs. prevention effectiveness:**
- Claim A (Klein / S1 Irreducible #1): Techniques maximizing prospective hindsight quality will produce the most valuable Pre-Mortems.
- Claim B (S1 Irreducible #2 elision): The causal chain from reason-identification → prevention quality → execution → failure-rate reduction may be weak or broken; maximizing reason-identification may not maximize project success.
- Discriminating condition: Longitudinal study comparing project failure rates in high-reason-count vs. low-reason-count Pre-Mortem groups. If failure rates are not correlated with reason count, Claim B governs and the 30% finding is not decision-relevant for practitioners.

**Collision 4 — Prevention-only framing vs. full risk response taxonomy:**
- Claim A (S1 Tension #5): Step 5 correctly treats all top-7 failures as prevention targets.
- Claim B (standard risk management): Some top-7 failures should be accepted, transferred, or monitored based on cost-benefit analysis. Forcing prevention framing misallocates resources.
- Discriminating condition: Have an independent risk analyst classify top-7 Pre-Mortem outputs as prevent/accept/transfer/monitor. If >30% would optimally use a non-prevention response, Claim B governs.

---

## Discovery vs. Proof

Source: S2 Constraint Escape (Discovery) · S5 Precision Forcing + S7 Verification (Proof)

| Discovery (S2 Domain Mappings) | Proof (S5 Constructive Spec + S7 Verification) |
|-------------------------------|------------------------------------------------|
| M5 (Oracle Separation): The planning team is a formal axiomatic system bounded by its own shared assumptions; failure modes outside the team's axiom set are structurally unreachable, not merely hard to think of. External oracle injection is the structural escape. | S5 Level 4 formalisation: |R(F_c, C₀)| ≥ 1.30 × |R(F_p, C₀)| is well-formed; the guide's implicit extension to prevention effectiveness (P_eff) is not bounded below by this ratio — confirmed deductive consequence, not hypothesis. |
| M4 (Destructive Interference): A mandatory "anti-plan" narrative at equal resolution to the project plan would function as the cancellation wave; residual risks surviving cancellation are the genuine Pre-Mortem input. | S5 Constructive Spec (minimal model): anonymous written risk survey vs. standard Pre-Mortem across teams with measured Edmondson psychological safety; all five elements non-eliminable; three operational ambiguities noted but resolvable. |
| M3 (Mechanism Design blend): The facilitator's most powerful intervention is structural (payoff architecture), not psychological (creating safety); adversarial role assignment + post-project scoring produces incentive-compatible risk revelation. | V5 PASS: constructive spec is minimal, determinate, and traces directly to S1 primitives and S6 consolidated hypotheses. |
| M1 (Immunology, negative-space): NK-cell logic applied to Pre-Mortem: ask "what expected-good markers are absent from this plan?" rather than "what could go wrong?" — surfaces absence signals. | V1 PARTIAL: Step 2 inference (social suppression as primary mechanism) requires one additional assumption — that Klein's control condition included social dynamics. All other inferential steps are explicitly valid. |
| M2 (Observer Separation): The planning model cannot serve as its own failure-detection model; seed the Pre-Mortem with a disturbance catalog from prior-project failure databases rather than from the current project's logic. | D2 Divergence unresolved: H-C1 (suppression removal primary) and H-C3 (commitment-and-consistency primary) predict different intervention priorities; no cross-stage convergence resolves this. |

---

## Independence-Verified Bridges

Source: S3 Peripheral Exploration convergent nodes · S2 conceptual blend emergent structure

**Bridge 1 — Social suppression removal as the primary mechanism:**
- Source concept (S3 IL2): Orphan knowledge — expertise that exists in the team but is trapped behind authority gradients, politeness norms, or optimism pressure.
- Target insight: The Pre-Mortem's 30% gain is a lower bound; the technique is successful if the lowest-status person who knew the critical failure mode was able to surface it.
- Structural invariant: The pattern "person knows, cannot speak, system fails" recurs identically in Challenger disaster O-ring suppression, organizational silence research (Morrison & Milliken), intelligence community failures, and Cassandra mythology — this is a genuine structural invariant, not an analogy.
- Disanalogy limit: The Challenger and intelligence cases involved suppression of known threats by identifiable individuals; Pre-Mortem suppression is diffuse and normative, not concentrated in a single gatekeeper. Structural mechanisms of release may differ.
- Independent source chains: S3 (IL2), S6 (H-C1, confidence 0.82), S4 (singularity: blame culture → safety field = 0), S5 (Branch A: Potemkin accountability layer). Confirmed multi-stage convergence.

**Bridge 2 — Cassandra gap as structural detection-uptake separation:**
- Source concept (S3 IL3): The Cassandra failure-of-uptake — detection and uptake are structurally distinct problems; the Pre-Mortem addresses only the former.
- Target insight: A project can fail exactly as predicted in a Pre-Mortem whose outputs were never implemented; the Cassandra gap is introduced structurally by the team format's separation of detection (distributed across participants) from decision authority (concentrated elsewhere).
- Structural invariant: The detection-uptake gap appears identically in intelligence analysis, medical diagnosis vs. treatment compliance, weather forecast accuracy vs. evacuation compliance, and security red-team findings adoption. In all cases, detection quality is high while uptake is low; the structure is a handoff problem, not a competence problem.
- Disanalogy limit: In intelligence and security contexts, detection and decision-authority are separated by institutional design; in Pre-Mortem, the separation is incidental (a team exercise terminating at ownership assignment). The gap could in principle be closed within the technique by modifying Step 6 — which is not true in the intelligence-community case.
- Confirmed as deductive consequence D-I3 in S6: no flagged premise.

**Bridge 3 — Voting mechanism as structural measurement error:**
- Source concept (S4 Frame 3): The vote in Step 4 is a single-dimension proxy (social salience) for a two-dimensional construct (probability × impact); this is a measurement architecture flaw, not a calibration error.
- Target insight: The voting mechanism cannot be fixed by better facilitation because the information required (two-dimensional probability × impact) is not recoverable from a single-dimension social vote; the output systematically discards low-salience, high-probability systemic risks.
- Structural invariant: Dot voting produces socially-salient prioritization across contexts (product roadmaps, retrospectives, risk workshops); the finding from S6 G4 is robust across expertise levels and anonymity conditions.
- Disanalogy limit: In low-stakes, high-expertise, low-hierarchy teams, the convergence of social salience with actual probability × impact may be higher — the systematic bias may shrink. S6 G4 survives boundary tests for expertise and anonymity, but low-stakes conditions are not directly tested.

*(S2 emergent-structure bridges from conceptual blend: the Adversarial Pre-Mortem mechanism — session + adversarial domain assignment + post-project scoring — emerges from the M3 blend; it does not exist in either game theory or standard Pre-Mortem separately. Bridge traces to S2 emergent structure, independent of S3 convergent nodes.)*

---

## Alternative Hypotheses

Source: S6 Consolidated Hypotheses (H-C1 through H-C8)

**H-C1 — Baseline-suppression artifact (primary mechanism):**
The 30% improvement is an artifact of releasing socially suppressed knowledge, not a cognitive gain from prospective hindsight.
- Confidence: 0.82
- Supporting modes: Abductive (H-A2), Deductive (H-D2), Inductive (instances 2, 10, G1)
- Falsification condition: Administer anonymous written risk survey to same team; if output count is equivalent to Pre-Mortem, H-C1 confirmed; if Pre-Mortem substantially outperforms full anonymity, H-C1 refuted and cognitive mechanism must be retained.

**H-C2 — Voice suppression intensity as primary variance driver:**
Pre-Mortem effectiveness is determined by pre-existing voice suppression intensity, not by facilitation quality or framing precision.
- Confidence: 0.78
- Supporting modes: Abductive (H-A1), Deductive (implied D-I2), Inductive (G1, H-I1)
- Falsification condition: Correlate Pre-Mortem gains with Edmondson psychological safety scores; inverse correlation confirms H-C2.

**H-C5 — Dose-optimal vividness (inverted-U response):**
Moderate vividness maximizes output quality; maximum vividness (as the guide instructs) may cross the nocebo threshold and reduce quality in non-expert teams.
- Confidence: 0.71
- Supporting modes: Deductive (H-D1 + H-D3), Inductive (G2 partial), Abductive (S3 IL5)
- Falsification condition: Three-condition vividness manipulation (low/moderate/high) with matched teams; expert vs. novice sub-group analysis.

**H-C3 — Commitment-and-consistency as primary long-term mechanism:**
Prevention value is delivered through public ownership declarations (Cialdini commitment), not through the prevention strategy document; Step 6 is the highest-value step.
- Confidence: 0.67
- Supporting modes: Abductive (H-A5), Deductive (D-I3 confirmed consequence)
- Falsification condition: Compare implementation rates of items with public verbal declaration vs. written list only.

**H-C4 — Team-size-dependent voting validity:**
The guide's minimum team size of 4 is too small for reliable prioritization; status-based vote influence cannot be neutralized below approximately 8 participants.
- Confidence: 0.62
- Supporting modes: Deductive (H-D4), Inductive (G4 analysis)
- Falsification condition: Matched Pre-Mortem sessions at team sizes 4, 8, 12 with and without a high-status authority figure; measure vote outcome variance.

**H-C7 — Cassandra gap as handoff problem (team-format artifact):**
Solo or co-located-authority Pre-Mortems have near-zero Cassandra gap; the team format introduces the gap by separating detection from decision authority.
- Confidence: 0.64
- Supporting modes: Inductive (H-I3), Deductive (D-I3 partial)
- Falsification condition: Compare implementation rates solo vs. team Pre-Mortem; if solo shows substantially higher implementation despite lower identification breadth, H-C7 confirmed.

**H-C6 — Nocebo risk bounded by team expertise:**
The self-defeating prophecy mechanism is primarily a novice-team phenomenon; expert teams tolerate maximum vividness without expectation contagion because expertise modulates threat appraisal.
- Confidence: 0.58
- Supporting modes: Inductive (H-I2)
- Falsification condition: Pre/post morale and confidence metrics in expert-only vs. mixed-expertise Pre-Mortem sessions.

**H-C8 — Primary value in motivated-reasoning bypass, not identification:**
The technique's value chain runs through Steps 5–6 (prevention commitment), not Steps 1–4 (identification); identification is context-setting for the motivated-reasoning bypass.
- Confidence: 0.55
- Supporting modes: Abductive (H-A3)
- Falsification condition: Compare prevention strategy implementation rates when framing is vs. is not present, given identical risk lists.

---

## Density-Checked Falsification

Source: S6 Falsification Engine

**H-C1 (baseline suppression artifact):**
3 inference-mode vectors attempted (abductive, deductive, inductive) — density: HIGH — residual uncertainty: MODERATE (single critical assumption: Klein's control condition).

**H-C2 (voice suppression intensity drives effectiveness):**
3 inference-mode vectors attempted (abductive, deductive, inductive) — density: HIGH — residual uncertainty: MODERATE (no published study directly tests the Edmondson-scale interaction with Pre-Mortem gain).

**H-C5 (dose-response ceiling):**
3 inference-mode vectors attempted (deductive, inductive, abductive) — density: HIGH — residual uncertainty: MODERATE (dose-optimal vividness regime not directly tested in Pre-Mortem contexts; ethics of affect manipulation adds experimental friction).

**H-C3 (commitment-and-consistency primary):**
2 inference-mode vectors attempted (abductive, deductive) — density: MODERATE — residual uncertainty: HIGH (no empirical test of public declaration vs. written-list implementation rates in Pre-Mortem literature).

**H-C4 (team-size-dependent voting validity):**
2 inference-mode vectors attempted (deductive, inductive) — density: MODERATE — residual uncertainty: MODERATE (G4 is robust across boundary tests; H-D4 is a valid structural inference; direct team-size controlled study absent).

**H-C7 (Cassandra gap as handoff problem):**
2 inference-mode vectors attempted (inductive, deductive) — density: MODERATE — residual uncertainty: MODERATE (D-I3 confirmed deductive; H-I3 boundary-tested; solo Pre-Mortem empirical data absent).

**Millikan asymmetries flagged (S6):** H-C1 and H-C5 both show significant asymmetry — the primary conclusion's competing claim is making a stronger assertion than the evidence supports, and the alternative hypothesis is easier to test. Neither asymmetry has been resolved by published research.

---

## Scope Limits

**Applies to:** The claim that Pre-Mortem retrospectives improve risk-identification and motivate prevention, specifically for team-based sessions (6–20 participants) conducted in organizations with moderate pre-existing voice suppression, led by a neutral facilitator who maintains certainty framing, with participants holding domain-relevant knowledge, in a session context that is novel (not a repeated ritual), and where the outcome of interest is the quality and implementation rate of prevention strategies.
**Does not extend to:** (1) Highly hierarchical or high-blame organizations where psychological safety approaches zero — in these contexts the technique produces performative compliance, not genuine risk surfacing. (2) Flat, psychologically safe teams with near-zero pre-existing voice suppression — the technique's marginal value shrinks toward zero as baseline suppression decreases. (3) Claims about downstream project failure-rate reduction — Klein's 30% finding applies to reason-identification counts in controlled settings, not to prevention-strategy implementation rates or project outcomes; the guide's implicit extension of this finding to prevention effectiveness and project success is unvalidated. (4) Remote or asynchronous team adaptations without redesign of the certainty framing and convergence mechanism. (5) Conclusions stated in the narrative framing of the original guide — the primary conclusion was derived by applying formal analysis (declarative/symbolic frames) to a narrative-frame input; the analysis applies within the analytic frame, not as a re-endorsement of the guide's narrative claims.
**Claims refused:** The claim that maximizing failure-framing vividness monotonically improves Pre-Mortem outputs — refused because the nocebo/expectation-contagion literature (S3 IL5, S6 H-C5) establishes a dose-response ceiling effect not addressed by the guide, and the guide's instruction to maximize vividness conflicts with this finding. The claim that the Pre-Mortem's 6-step format as described is sufficient for complex projects within 45–60 minutes — refused because S5 structural analysis shows time allocation produces slogan-level prevention strategies at Step 5 for high-complexity inputs.

---

## Coherence Signals

Source: S7 structural-coherence weighting + elegance scoring

**Signal CS-1 — Social suppression as the operative variable:**
Appears independently in S3 (IL2: orphan knowledge surfacing), S4 (singularity: blame culture → safety field = 0), S5 (Branch A: Potemkin accountability layer), S6 (H-C1 confidence 0.82, H-C2 confidence 0.78). Cross-domain recurrence: Challenger O-ring suppression; organizational silence research (Morrison & Milliken); intelligence community failures; Cassandra mythology. Structural invariant confirmed: "person knows, cannot speak, system fails" pattern across ≥4 independent domains. **Strongest convergence signal in the session.**

**Signal CS-2 — Detection-uptake separation (Cassandra gap):**
Appears independently in S1 (key tension #8: ownership assignment vs. authority to act), S3 (IL3: elegance 3.0), S4 (structural imbalance: ownership mechanism → external execution), S5 (Branch D: accountability-without-agency), S6 (D-I3: confirmed deductive consequence). Cross-domain: intelligence analysis, medical diagnosis-treatment gap, weather forecasting vs. evacuation compliance, security red-team findings adoption. **Second strongest convergence signal.**

**Signal CS-3 — Voting as structural measurement error:**
Appears independently in S1 (key tension #4), S3 (IL1 structural implication), S4 (Frame 3: single-dimension proxy), S5 (Branch C: politically salient vs. probability × impact), S6 (G4: robust across expertise and anonymity boundary tests). Note: finding is classified as structural (unfixable by facilitation), confirmed by S4 and S6 independently. **Third convergence signal.**

**Signal CS-4 — Dose-response ceiling on certainty framing:**
Appears independently in S2 (M4: anti-plan must match plan resolution level, not exceed it), S3 (IL5: self-defeating prophecy, elegance 2.4), S4 (limit case: maximum framing → availability bias attractor), S5 (Level 4 formalisation reveals guide's monotone-positive assumption is not derived from Klein), S6 (H-C5: dose-optimal vividness, 3 inference modes, confidence 0.71). **Fourth convergence signal.**

Elegance score of primary conclusion (S7): **2.7 (HIGH)** — simplicity 0.7, symmetry 1.0 (cross-domain recurrence in ≥3 independent domains confirmed), depth 1.0 (≥5 independently verifiable implications).

---

## Generalization Checks

Source: S7 Generalization Check (Newton)

**Holds at:** Any group deliberation technique whose stated benefit is "surfacing risks that would not otherwise emerge" — Red Team exercises, Futurespectives, FMEA facilitated sessions, sprint retrospectives, devil's advocate processes, anonymous survey methods in organizational settings. The structural prediction — effectiveness is determined by pre-existing suppression intensity, not technique fidelity — holds for the entire class of "facilitated permission structures for voicing known-but-suppressed concerns."

**Holds at (Cassandra gap):** Any process where detection and decision authority are structurally separated — intelligence analysis, medical diagnosis, security red-teaming, scientific peer review. The gap is introduced by the handoff structure, not by the specific technique.

**Breaks at (technique-specific elements):** The dose-response ceiling claim (CS-4) is most specific to the Pre-Mortem's immersive failure-certainty framing; techniques that do not rely on vivid imaginative commitment do not face this specific ceiling. The voting mechanism finding (CS-3) applies to any dot-voting prioritization, which is common but not universal.

**Breaks at (suppression mechanism):** In genuinely flat organizations with near-zero hierarchy but strong interpersonal political dynamics, the suppression mechanism operates through relationships rather than authority gradients; the Pre-Mortem's ritual permission structure may not address this form of suppression, and Pre-Mortem gains may approach zero despite not being a high-hierarchy environment. (S6 G1 boundary test: FAILS at this boundary.)

---

## Representational Signals

V7 detected a representational shift: **Narrative (Frame 6) → Declarative/Symbolic (Frame 4/1)**

The input was a ~7500-word practitioner guide using time words, facilitation instructions in imperative form, example scenarios, before/after comparisons, and a causal narrative structure. Its dominant frame is narrative.

The primary conclusion is expressed as declarative property claims about mechanism and failure conditions: "The Pre-Mortem's effectiveness is most parsimoniously explained as social suppression removal, not prospective-hindsight cognitive enhancement." This is supported by formal constructs (S5 Level 4/5 rendering: |R(F_c, C₀)| ≥ 1.30 × |R(F_p, C₀)|) and structural predictions.

**What this shift opens:** The conclusion answers "what is mechanically true about the Pre-Mortem" rather than "how to run the technique." These are different questions. The analysis is valid and more illuminating than the guide's self-description, but does not directly translate back to facilitation instructions without an additional translation step. Practitioners seeking implementation guidance must bridge from the declarative analysis back to the narrative-imperative register the guide uses.

**V6 scope narrowed accordingly:** Scope Limits entry (5) explicitly states that the primary conclusion applies within the analytic frame, not as a re-endorsement of the guide's narrative claims.

---

## Open Questions & Next Probes

Source: S6 + S7 divergence points and V-gate findings, ranked by discriminating power per unit effort

**Probe 1 — What was Klein's control condition? (Priority: HIGH)**
*Why it matters:* The entire H-C1 hypothesis (suppression-removal as primary mechanism) hinges on whether Klein's baseline included social dynamics. If Klein's control was already fully anonymous, H-C1 is partially refuted and the cognitive mechanism must be retained. This single factual question resolves the V1 PARTIAL logic gap.
*Effort:* Literature retrieval only — no new study required. Highest discriminating power per unit effort in this session.

**Probe 2 — Randomized comparison: Pre-Mortem vs. anonymous risk survey, stratified by Edmondson psychological safety score (Priority: HIGH)**
*Why it matters:* Directly tests H-C1 and H-C2. If Pre-Mortem gain over anonymous survey is moderated by pre-existing safety score (larger in low-safety teams, near-zero in high-safety teams), the suppression-removal mechanism is confirmed.
*Effort:* Approximately one research-lab Pre-Mortem study with matched controls and Edmondson pre-measurement.

**Probe 3 — Post-Pre-Mortem audit: implementation rate of ownership assignments at 30, 60, and 90 days (Priority: HIGH)**
*Why it matters:* The Cassandra gap (CS-2) predicts that a substantial fraction of Pre-Mortem-identified risks will not have implemented prevention strategies by project execution. No published measurement of this rate exists. A single organizational audit would provide a baseline and test the "false mitigation confidence" prediction.
*Effort:* Organizational access required, not a controlled experiment; lower friction than Probe 2.

**Probe 4 — Vividness manipulation study to locate the dose-response ceiling (Priority: MEDIUM)**
*Why it matters:* The guide's instruction to maximize vividness is potentially counterproductive per S3 IL5 and S6 H-C5. Locating the dose-optimal vividness level would allow the guide's facilitation instructions to be refined. Expert vs. novice sub-group analysis tests H-C6.
*Effort:* Requires controlled experimental design with affect measurement; ethics of emotion manipulation adds friction.

**Probe 5 — Solo vs. team Pre-Mortem comparison on implementation rate (Priority: MEDIUM)**
*Why it matters:* The Cassandra gap hypothesis predicts that solo Pre-Mortems (decision-maker only, no handoff) will have dramatically higher implementation rates than team Pre-Mortems, despite lower identification breadth. If confirmed, the guide's assumption that team format is necessary is challenged.
*Effort:* Requires access to comparable solo and team decision-makers running Pre-Mortems on matched decisions; feasible in organizational research settings.

**Probe 6 — Tail-risk aggregation: what patterns exist in discarded below-threshold items from Step 4? (Priority: LOW)**
*Why it matters:* S4 (structural imbalance: permanent discard of sub-threshold items) and S5 (Branch C) identify the discarded set as a potential systemic-risk repository. A qualitative study comparing discarded items against actual project failure causes would determine whether the voting threshold creates systematic blindspots.
*Effort:* Requires post-project retrospective data collection; lower discriminating power on the primary mechanism.
