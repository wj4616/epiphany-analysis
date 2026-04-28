---
stage_id: S4
stage_name: Dynamic Simulation
module_version: 1.1.0
status: complete
scale: DEEP
---

<dynamic_simulation>

<system_model>

## System Model

**Components and initial states:**

| Component | Initial State |
|-----------|---------------|
| Certainty Framing Engine | Active — "project has completely failed" is asserted as fact |
| Psychological Safety Field | Hypothetical — claimed to emerge from the certainty frame |
| Individual Brainstorm Buffer | Empty — each participant holds an isolated information set |
| Group Convergence Layer | Inactive during Step 2; activated in Step 3 |
| Voting Prioritization Filter | Inactive until Step 4; threshold output = top 5–7 |
| Prevention Strategy Register | Empty — output container for Steps 5–6 |
| Ownership Assignment Mechanism | Inactive until Step 6; maps strategy → (owner, timeline, success measure) |
| Facilitator Control Function | Active throughout — mediates all component transitions |

**Driving forces (what causes the system to evolve):**
- The certainty frame (prospective hindsight) activates the psychological permission structure.
- Isolation in Step 2 separates individual information reservoirs before they can anchor each other.
- Step 3 pooling applies convergence pressure — group dynamics re-enter.
- Voting applies a social salience filter, not a probability × impact filter.
- Step 6 produces a closure signal that may or may not be connected to actual execution.

**Coupling between components:**
- Certainty Frame → Psychological Safety Field: strong coupling; degrading the frame (hedge words) likely degrades safety perception.
- Psychological Safety Field → Individual Brainstorm Buffer: conditional coupling; safety field modulates how much each participant's buffer is actually released. In high-blame cultures, this coupling is near-zero.
- Individual Brainstorm Buffer → Group Convergence Layer: one-way flush at Step 3; no feedback path from group layer back to individual buffer.
- Group Convergence Layer → Voting Filter: direct feed; the group layer pre-shapes what enters the vote (dominant voices, visible salience cues).
- Voting Filter → Prevention Register: threshold gate — items below threshold are permanently discarded with no archival mechanism.
- Prevention Register → Ownership Mechanism: linear assignment; no verification that assigned owner has authority to execute.
- Ownership Mechanism → External Execution Environment: handoff point — the system's causal chain terminates here; what happens after the session is outside the technique's scope.

**Nominal operating condition ("working correctly"):**
A session is working correctly when:
1. The certainty frame is maintained without hedging throughout.
2. Step 2 produces genuinely independent individual outputs (no pre-session anchoring has occurred).
3. Step 3 grouping does not collapse distinct failure modes into a single cluster.
4. Step 4 voting surfaces the highest-expected-impact failures (not merely the most feared).
5. Step 5 prevention strategies are specific and testable.
6. Step 6 owners have both accountability and authority to act.
7. Facilitator successfully manages psychological safety without suppressing directionally inconvenient concerns.

</system_model>

<observer_frames>

## Observer Frame Analyses

*(All three frames constructed simultaneously per machine-advantage instruction — frames do not depend on each other.)*

---

### Frame 1 — Insider Frame (Team Member)

**Position:** I am a mid-level team member inside the Pre-Mortem session. I have local information: I know my own concerns, the interpersonal dynamics of this team, the organizational history, and who the dominant voices are. I do not have access to the facilitator's intent or the full map of what other participants are holding back.

**What is visible from this position:**
- I can observe which failures feel safe to write on a card and which ones feel career-sensitive. The certainty frame gives me partial permission, but I know my manager is in the room.
- I can observe that in Step 2, I am writing independently — but I have already been in project kickoff meetings, so I have been anchored by prior group discussion before the session started. My "independent" brainstorm is already influenced.
- In Step 3, I watch certain items get grouped under dominant-voice framings. My item about "communication breakdown with the product team" gets merged into "People failures" — losing its specificity.
- In Step 4, I observe that the items voted up are the ones that were presented most vividly or by the most senior person. My item about the external vendor dependency — which I believe is the highest actual risk — gets two votes. The most-feared technical failure gets eight.
- In Step 6, I am assigned ownership of a prevention strategy. I do not have budget authority or the ability to compel the vendor to change their timeline. I have accountability without agency.

**Limit case (insider at boundary):** When the session is run by organizational leadership as a box-checking exercise, my insider information tells me immediately that the session output will be filed and not acted on. I experience this as psychological double-jeopardy: I have now documented concerns that leadership can later claim were "addressed in pre-planning." I self-censor at the boundary — writing only the safe failures.

**Frame-exclusive findings (visible only from inside):**
- Pre-session anchoring: individual brainstorming in Step 2 is structurally compromised if the team has already discussed risks in prior meetings. The guide assumes blank-slate isolation; real organizational life does not provide it.
- The insider knows whether the session is genuine or performative — but has no in-session mechanism to surface or test this.
- Ownership assignment (Step 6) is experienced differently by insiders with authority vs. insiders without it. The guide treats all owners as equivalent.

---

### Frame 2 — Outsider Frame (External Observer)

**Position:** I observe the Pre-Mortem session and its organizational context from outside. I can see inputs and outputs, and aggregate behavioral patterns, but not internal reasoning or individual psychology during the session.

**What is visible from this position:**
- I observe that the session produces a list of prevention strategies with assigned owners. From outside, this looks identical whether the session was psychologically genuine or performatively compliant.
- I observe that the 30% improvement figure (Klein) was generated in a research context, not an organizational intervention context. The transfer of this finding to the current session is invisible from inside but apparent from outside: I can see the gap between laboratory prospective hindsight and facilitated group sessions in politically charged organizations.
- I observe that the technique's output (Step 6 artifacts: owner, timeline, success measure) is structurally identical to a risk register — yet the guide positions Pre-Mortem as distinct from traditional risk assessment. From outside, the outputs converge even if the processes differ.
- I observe the long-tail items discarded in Step 4. These items vanish from the visible output. From outside, I see a system that systematically discards low-salience risks without aggregating them — the classic tail-risk blindness pattern known from financial stress testing.

**Limit case (outsider at boundary):** At the boundary where the Pre-Mortem output is used as accountability evidence in a post-project review, I observe the following from outside: the "low stakes hypothetical" frame has retrospectively become "you documented this risk and didn't prevent it." The psychological safety that made the session productive has been weaponized. From outside, this transformation is structurally predictable; from inside, it is a trap.

**Frame-exclusive findings (visible only from outside):**
- The technique produces no observable difference between a genuine session and a performative one. There is no output-level quality signal.
- The guide's claim that Pre-Mortem is distinct from traditional risk assessment is contradicted by the structural identity of the outputs at Step 6.
- The tail-risk aggregation problem is only visible from outside: individual items below the voting threshold may represent a systemic pattern, but the system's output mechanism discards them individually without looking for clusters.

**Cross-frame conflict (Frames 1 and 2):** The insider experiences psychological safety as a local phenomenon — it depends on who is in the room. The outsider observes that the organizational context (blame culture, documentation policy) determines whether safety actually emerged. These frames predict different behavior: the insider may report feeling safe; the outsider may observe that self-censorship occurred. This is a theory collision candidate (see Theory Collisions below).

---

### Frame 3 — Measurement-Instrument Frame

**Position:** I am the measurement apparatus of the Pre-Mortem itself — specifically, the vote in Step 4. My action of measuring (aggregating individual risk assessments via dot voting) changes what I am measuring.

**What I measure:** The social salience of failure scenarios, expressed as vote count per item.

**What my measurement changes:** By making vote counts visible, I create a real-time feedback signal. Items that receive early votes attract more votes (preference cascade / bandwagon effect). My measurement is not independent of the phenomenon being measured — I am a social amplifier.

**What I cannot distinguish:** High-probability/low-salience failures (risks everyone knows are real but no one finds dramatic) vs. low-probability/high-salience failures (catastrophic scenarios that are emotionally vivid but unlikely). My resolution is insufficient to separate these.

**What my measurement costs:** 10 minutes of session time, the permanent discard of below-threshold items, and the replacement of a probability × impact matrix with a social consensus signal.

**Can "most likely to occur" and "most impactful if it occurs" be measured simultaneously?** No. A dot vote asks a combined implicit question ("most likely × most scary") that confounds both dimensions. These cannot be disentangled in a 10-minute vote. This is a fundamental measurement constraint of the technique.

**Limit case of measurement (precision → 0):** When team size approaches 4 (minimum), each participant controls 25% of the vote. One senior person can determine the top 5–7 failures unilaterally. The measurement resolves to social hierarchy, not risk reality.

**Limit case of measurement (precision → ∞):** If the team used a formal probability × impact matrix with calibrated estimates per item, the session would require hours, not 10 minutes. The time constraint is a precision budget that the guide has allocated to speed.

**Frame-exclusive findings (visible only from measurement frame):**
- The voting mechanism is a single-dimension proxy for a two-dimensional construct (probability × impact). This is a structural measurement error, not a calibration error — it cannot be fixed by better facilitation.
- The measurement instrument (vote count) creates the outcome it claims to measure (priority) — this is a performative instrument, not an observational one.
- The "success measure" required in Step 6 is a future measurement commitment, but no baseline measurement is taken during the session. There is no pre/post comparison possible.

**Cross-frame conflict (Frames 2 and 3):** The outsider observes that the output looks like a risk register. The measurement frame reveals that the prioritization mechanism (social vote) is systematically different from how risk registers are typically populated (probability × impact analysis). The outputs converge in form but diverge in epistemic basis. The guide does not acknowledge this divergence.

</observer_frames>

<imbalances>

## Imbalances

**[IMBALANCE] Certainty Frame → Psychological Safety Field: asymmetric dependency**
The certainty frame (certainty that failure has occurred) is asserted in 5 minutes (Step 1). Psychological safety — the claimed output of that frame — requires sustained organizational conditions that cannot be created in 5 minutes. The frame consumes session time much faster than it can produce the safety field. In high-blame cultures, the safety field never reaches operating condition regardless of how the frame is maintained.

*Genuineness check: Structural. The production time of psychological safety is fundamentally mismatched with the consumption speed of the framing step. Not an artifact of framing. PROMOTED.*

---

**[IMBALANCE] Individual Brainstorm Buffer → Group Convergence Layer: one-way flush, no feedback path**
Step 2 produces individual outputs; Step 3 groups them. There is no mechanism by which the group convergence stage can feed back to expand or revise individual outputs. Once Step 3 begins, the individual buffer is frozen. Items not written in Step 2 are permanently absent. The system has a one-shot input window.

*Genuineness check: Structural. The step sequence is irreversible by design. PROMOTED.*

---

**[IMBALANCE] Voting Filter → Prevention Register: permanent discard of sub-threshold items**
The voting step (Step 4) produces a hard threshold: top 5–7 items proceed; all others are permanently discarded. There is no accumulator, no archival path, no aggregation of the discarded set. In a team of 10, a session may generate 40–60 individual failure cards; 33–55 of them are silently dropped. The discarded set may contain systemic patterns that are invisible when items are evaluated individually.

*Genuineness check: Structural. The discard is a design feature, not an incidental failure mode. PROMOTED.*

---

**[IMBALANCE] Ownership Mechanism → External Execution Environment: accountability-authority mismatch**
Step 6 assigns ownership with required deliverables (owner, timeline, success measure). The technique has no mechanism for verifying that the assigned owner has budget authority, organizational authority, or the ability to compel other parties to act. The ownership assignment is a closure signal that may not correspond to execution capacity.

*Genuineness check: Structural. The technique terminates at assignment; execution environment is out of scope by design. The imbalance is real. PROMOTED.*

---

**[IMBALANCE] Klein's 30% finding → Prevention Effectiveness claim: measurement domain mismatch**
Klein's research measures reason-identification improvement (prospective hindsight → 30% more reasons identified). The guide's implicit claim is that this translates into prevention effectiveness (fewer project failures). These are different outcomes. The system drives resource allocation toward a technique whose effectiveness is validated at the wrong level of the causal chain.

*Genuineness check: Structural. The elision is present in the guide itself (S1 stripped irreducible #2). PROMOTED.*

</imbalances>

<singularities>

## Singularities

**[SINGULARITY] at Psychological Safety Field → 0 (high-blame culture): all outputs become performative**
When organizational blame culture is high, the psychological safety field approaches zero regardless of the certainty frame. At this limit, all participant outputs are self-censored to safe failures. The technique's core mechanism (surfacing concerns that wouldn't otherwise emerge) becomes undefined — the technique is running, producing artifacts, but the causal mechanism is absent. The session cannot be distinguished from a box-checking exercise.

*Genuineness check: Structural. The singularity is not a parameterization artifact — it reflects a real organizational condition. PROMOTED.*

---

**[SINGULARITY] at team size → minimum (4 people): voting resolves to social hierarchy**
As team size approaches 4, individual vote weight approaches 25%. A single senior participant controls a quarter of all votes. At team size = 1 (degenerate case), the vote is meaningless — but even at 4, a 2-person coalition can determine the top 5–7 outcomes. The measurement instrument (vote) stops functioning as a crowd-wisdom aggregator and becomes a social dominance signal.

*Genuineness check: Structural for small teams; incidental for nominal team sizes. ADVISORY for teams of 6+; PROMOTED for teams of 4–5.*

---

**[SINGULARITY] at certainty frame → weakened (hedge language introduced): 30% improvement degrades toward baseline**
Klein's 30% finding is contingent on the prospective hindsight framing being fully engaged. If the facilitator or organizational context weakens the frame (e.g., "imagine the project might have failed"), the mechanism degrades. The exact degradation curve is unknown, but the theoretical boundary is clear: at "might fail" framing, the technique converges to traditional risk assessment (where the 30% improvement does not apply). The improvement percentage is a function of frame strength — the guide does not acknowledge this dependency.

*Genuineness check: Structural. The frame-strength dependency is implicit in Klein's research design. PROMOTED.*

---

**[SINGULARITY] at facilitator skill → 0: all coupling between components fails simultaneously**
The facilitator mediates every component transition: frame creation, individual isolation, group convergence pacing, voting fairness, prevention depth, ownership specificity. At facilitator skill → 0, all these transitions degrade simultaneously. The technique is not a self-executing process — it is a facilitated process. The guide's underspecification of facilitator requirements means the technique has an undefined behavior boundary at low facilitator skill.

*Genuineness check: Structural. The facilitator dependency is endemic to the design. PROMOTED.*

</singularities>

<limit_cases>

## Limit Cases

**[LIMIT-CASE] at all parameters minimum (small team, minimal time, low facilitator skill, high blame culture):**
Expected behavior: technique should surface failure risks.
Observed behavior: Session runs, produces 4–8 failure cards (all safe/obvious), votes are determined by the most senior person, prevention strategies are generic, ownership is assigned without authority verification. Output is formally complete but informationally empty. The system does not fail visibly — it succeeds performatively.
*Structural failure: The technique has no quality signal. A null-output session looks identical to a high-quality session from outside.*

---

**[LIMIT-CASE] at certainty frame → maximum (extreme emotional staging, catastrophic language):**
Expected behavior: enhanced activation of prospective hindsight, more failure reasons surfaced.
Observed behavior: For some team compositions, extreme framing produces anxiety rather than analytical engagement. Participants focus on catastrophic but unlikely scenarios (availability bias) rather than probable systemic failures. "Wild scenarios" encouraged by facilitation tips (Step 2) amplify this. The system enters a low-probability/high-salience attractor.
*Structural failure: The certainty frame has an optimal range. Above a threshold of vividness, it activates availability bias rather than systematic risk enumeration. The guide does not specify this range.*

---

**[LIMIT-CASE] at Rolling Pre-Mortem (repeated sessions, same team):**
Expected behavior: repeated application improves risk identification over project lifetime.
Observed behavior: Habituation degrades the certainty frame's novelty effect. By the third or fourth session, participants have a learned script of "failure reasons to write." The prospective hindsight mechanism — which depends on psychological novelty of the failure-as-certain frame — produces diminishing returns. The technique converges toward a routinized checklist, losing the psychological mechanism that differentiated it from traditional risk assessment.
*Structural failure: The technique's effectiveness is partly a novelty effect. Rolling application systematically degrades the mechanism it claims to employ.*

---

**[LIMIT-CASE] at step 4 discard (tail risk ignored → system encounters discarded failure):**
Expected behavior: top 5–7 failures are addressed; residual risk is acceptable.
Observed behavior: A failure from the discarded set (item ranked 15th by vote) actually occurs. The discard mechanism has no review trigger — there is no protocol for re-examining the tail once voting has occurred. The system treats the top-7 selection as complete coverage. When a tail item manifests, it was technically "known" (written on a card) but institutionally invisible (discarded in Step 4).
*Structural failure: The voting threshold creates a false-coverage signal. The guide does not distinguish between "addressed" and "known but deprioritized."*

---

**[LIMIT-CASE] at distributed/async team (no co-location or synchronous participation):**
Expected behavior: technique runs as described.
Observed behavior: Step 1 (scene-setting) loses immersive effect in asynchronous format. Step 2 (individual brainstorming) works asynchronously. Step 3 (group convergence) requires synchronous judgment about similarity — async grouping introduces facilitator interpretation bias. Voting in async contexts allows participants to see partial vote counts, destroying the independence assumption. The certainty frame is harder to maintain over a 48-hour async window than in a 60-minute synchronous session.
*Structural failure: The technique's psychological mechanism (certainty frame + isolation + convergence) assumes synchronous co-presence. Async adaptation requires redesigning the mechanism, not just the logistics.*

</limit_cases>

<theory_collisions>

## Theory Collisions

**[COLLISION 1]**
Claim A (S1 Irreducible #7): *"Psychological safety is an output of the technique — the hypothetical framing creates permission to voice concerns."*
Claim B (S1 Tension #2): *"Group dynamics (hierarchy, politics) that suppress concerns in normal meetings are still present during the Pre-Mortem."*
Predicted outcome A: The certainty frame transforms the social environment; participants voice concerns they would suppress in a normal meeting.
Predicted outcome B: The same power differentials that suppress concerns normally are still active; the framing provides surface-level permission but does not change the underlying social calculus.
Discriminating condition: Run matched pairs of Pre-Mortems in high-hierarchy vs. low-hierarchy teams. Measure whether the concerns voiced in Pre-Mortems in high-hierarchy teams are: (a) concerns that were already voiced in normal planning meetings, or (b) genuinely novel concerns not previously surfaced. If novel concern rate in high-hierarchy Pre-Mortems is not significantly higher than in normal meetings, Claim B governs. If novel concern rate is higher, Claim A governs (partially — the degree of culture-dependence would still need quantification).

---

**[COLLISION 2]**
Claim A (S1 Category 2, item 16): *"Individual card-writing in Step 2 prevents anchoring and groupthink."*
Claim B (S1 Category 7, item 12): *"Prior group discussion (e.g., project kickoff meetings) may already have introduced anchoring before Step 2 begins."*
Predicted outcome A: Step 2 produces genuinely independent risk assessments — the parallel individual brainstorm taps each person's unique information reservoir.
Predicted outcome B: The independence assumption is violated by pre-session organizational exposure; Step 2 produces correlated outputs from a common prior, not independent outputs from distinct priors.
Discriminating condition: Compare failure card content between: (a) teams running a Pre-Mortem as their first structured discussion of the project, vs. (b) teams who have run multiple planning meetings before the Pre-Mortem. Measure cosine similarity of failure card sets across participants. High within-team similarity in group (b) vs. group (a) would confirm Claim B governs once pre-session exposure has occurred.

---

**[COLLISION 3]**
Claim A (Klein / S1 Irreducible #1): *"Prospective hindsight increases reason-identification by 30%."*
Claim B (S1 Irreducible #2 — elision): *"The technique's value is ultimately measured in prevention effectiveness (reduced project failure rate), not reason-identification rate."*
Predicted outcome A: Techniques that maximize prospective hindsight quality (stronger certainty framing, longer individual brainstorm time) will produce the most valuable Pre-Mortems.
Predicted outcome B: The causal chain from reason-identification → prevention strategy quality → execution quality → failure rate reduction may be weak or broken at any point; maximizing reason-identification may not maximize project success rate.
Discriminating condition: Longitudinal study comparing projects with Pre-Mortem sessions that produced many reasons (high Klein-metric) vs. few reasons, measuring actual project failure rates and implementation rates of prevention strategies. If failure rates are not correlated with reason count, Claim B governs and the 30% finding is not decision-relevant for practitioners.

---

**[COLLISION 4]**
Claim A (S1 Tension #5): *"Prevention focus is the right output — identified risks should be prevented."*
Claim B (standard risk management practice): *"Not all identified risks should be prevented; some should be accepted, transferred, or monitored based on cost-benefit analysis."*
Predicted outcome A: The Pre-Mortem's Step 5 (develop prevention strategies) correctly treats all top-7 failures as prevention targets.
Predicted outcome B: Some of the top-7 failures are cheaper to accept, insure against, or monitor than to prevent. Forcing prevention framing on all risks misallocates resources.
Discriminating condition: Take a set of top-7 Pre-Mortem outputs from real sessions. Have an independent risk management analyst classify each as: prevent / accept / transfer / monitor based on expected value analysis. Measure the proportion misclassified as "prevent" by the Pre-Mortem's forced framing. If >30% would optimally be handled by a non-prevention strategy, Claim B governs and the guide's Step 5 framing systematically misallocates resources.

---

## Genuineness Assessment Summary

| Item | Classification | Verdict |
|------|---------------|---------|
| IMBALANCE: Certainty Frame → Safety Field mismatch | Structural | PROMOTED |
| IMBALANCE: One-way brainstorm flush | Structural | PROMOTED |
| IMBALANCE: Permanent discard of sub-threshold items | Structural | PROMOTED |
| IMBALANCE: Accountability-authority mismatch | Structural | PROMOTED |
| IMBALANCE: Klein 30% → prevention claim elision | Structural | PROMOTED |
| SINGULARITY: Blame culture → safety field = 0 | Structural | PROMOTED |
| SINGULARITY: Team size → 4, vote = hierarchy | Structural for small teams | PROMOTED (small), ADVISORY (nominal) |
| SINGULARITY: Frame weakened → 30% degrades | Structural | PROMOTED |
| SINGULARITY: Facilitator skill → 0 | Structural | PROMOTED |
| LIMIT-CASE: Minimum parameters → performative output | Structural | PROMOTED |
| LIMIT-CASE: Maximum framing → availability bias | Structural (at extremes) | PROMOTED with range caveat |
| LIMIT-CASE: Rolling Pre-Mortem → habituation | Structural | PROMOTED |
| LIMIT-CASE: Tail risk discard → false coverage | Structural | PROMOTED |
| LIMIT-CASE: Async/distributed team | Structural | PROMOTED |
| COLLISION 1: Psych safety created vs. suppressed | Both claims active; culture-dependent | Discriminating condition specified |
| COLLISION 2: Individual independence vs. pre-anchoring | Both claims active; timing-dependent | Discriminating condition specified |
| COLLISION 3: Reason-identification vs. prevention effectiveness | Both claims active; chain-linkage unclear | Discriminating condition specified |
| COLLISION 4: Prevention-only vs. full risk response taxonomy | Both claims active; risk-type-dependent | Discriminating condition specified |

**Finding on theory collision count:** 4 theory collisions found at DEEP scale — meets the ≥2 threshold. The input is not internally consistent: it contains at least four claim pairs that predict different outcomes under identifiable conditions.

</theory_collisions>

</dynamic_simulation>
