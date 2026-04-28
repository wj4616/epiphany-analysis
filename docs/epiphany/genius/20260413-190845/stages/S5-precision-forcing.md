---
stage_id: S5
stage_name: Precision Forcing
module_version: 1.1.0
status: complete
---

<precision_forcing>

## Primary Claim Selected for Analysis

From S1 stripped irreducibles (items 1–3, synthesized):

> "Prospective hindsight — imagining that failure has already occurred — increases reason-identification ability by 30% (Klein). The load-bearing mechanism is certainty framing: treating failure as already happened, not merely possible. Weakening this framing likely degrades the technique's effectiveness."

This is the foundational claim of the Pre-Mortem methodology. All other claims (psychological safety, prevention strategy output, team size recommendations) are downstream of this. It is the appropriate root for precision forcing.

---

<renderings>

## 5-Level Vocabulary Stripping

### Level 1 — Five-Year-Old

When you want to figure out what might go wrong with a plan, it helps to pretend the bad thing has **already** happened. If you say "let's imagine it broke," people only think a little bit. But if you say "it IS broken — what happened?", people think much harder and come up with more reasons. Scientists checked and found that saying "it already broke" helps people think of about a third more reasons than saying "it might break." The trick only works if you really act like the bad thing happened for sure.

*Diagnostic:* All elements survive Level 1. No collapse.

---

### Level 2 — Fifteen-Year-Old

When planning a project, people are naturally optimistic — they focus on how things will succeed. One way to fight this is to run a thought experiment: imagine the project has already failed, then figure out why. A psychologist named Gary Klein found that this "already happened" framing helps people identify about 30% more reasons for failure compared to just asking "what might go wrong?" The key is the certainty — you have to commit to the idea that it failed, not just that it might. If you weaken that (saying "imagine if it failed") you get weaker results. The output of this exercise is a list of risks you can prevent.

*Diagnostic:* All elements survive Level 2. No collapse.

---

### Level 3 — Domain Expert

Prospective hindsight — operationalized as pre-mortem methodology — exploits a cognitive asymmetry: known-outcome reasoning activates a richer causal inference network than open-possibility reasoning. Klein's empirical finding: a 30% increase in reason-identification under prospective-hindsight conditions vs. standard risk elicitation. The load-bearing variable is the epistemic framing of the outcome: counterfactual certainty ("failure has occurred, P=1") vs. conditional possibility ("failure might occur, P<1"). Degradation of the certainty framing (hedged language, facilitator equivocation) is predicted to reduce retrieval depth. The technique produces actionable prevention strategies as its terminal output, derived from the elicited failure reasons.

*Diagnostic:* All elements survive Level 3. Critical precision point: the 30% figure is indexed to reason-identification, not to prevention effectiveness or downstream project success rate — the guide elides this distinction.

---

### Level 4 — Mathematician / Logician

Let:
- R(F, C) = set of failure reasons retrieved by an agent under framing F and conditions C
- F_c = certainty framing ("failure has occurred, P=1")
- F_p = possibility framing ("failure might occur, P<1")
- |R(F, C)| = cardinality of the retrieved reason set

**Klein's empirical claim:**
∃ experimental conditions C₀ such that |R(F_c, C₀)| ≥ 1.30 × |R(F_p, C₀)|

**Load-bearing mechanism claim (guide):**
∀F such that F approaches F_p (certainty weakened): |R(F, C)| → |R(F_p, C)|
That is: effectiveness is monotonically decreasing in the distance of F from F_c.

**Critical elision (flagged):**
Let P_eff = probability that at least one prevention strategy derived from R(F_c, C₀) is implemented and reduces failure probability.
The guide asserts: P_eff > 0 given R(F_c, C₀) ≠ ∅.
But Klein's result establishes only: |R(F_c, C₀)| ≥ 1.30 × |R(F_p, C₀)|.
P_eff is not bounded below by the 30% figure. The relationship between |R| and P_eff is unspecified.

**Vagueness candidate detected:** "deeper insights" (guide claim) cannot be formalized at Level 4 — see flagged vagueness section.

*Diagnostic:* Primary claim survives Level 4. The 30% claim is well-formed. The "deeper insights" claim collapses.

---

### Level 5 — Minimal Symbol

```
IF frame(outcome) = CERTAIN_FAILURE
THEN |reasons_retrieved| >= 1.30 * |reasons_retrieved under frame(outcome) = POSSIBLE_FAILURE|
WHERE frame degradation → output degradation (monotone)
```

Or as a conditional:
**IF** the facilitator commits to "failure has occurred" (not "might occur"), **THEN** teams identify ~30% more causal reasons than under standard risk questioning. This increase is in reasons found, not in projects saved.

*Diagnostic:* Fully expressible. Minimal symbol form confirms: the 30% claim is about a count increase in an elicitation exercise, not about downstream outcomes. The elision is visible at this level.

---

## 3-Representation Rendering

### Representation 1 — Diagram Description (Spatial / Visual)

A horizontal axis represents **epistemic certainty of outcome**, ranging from "failure is possible (P<1)" on the left to "failure has occurred (P=1)" on the right.

A vertical axis represents **number of causal reasons retrieved**.

The Klein finding plots as a point at the far right (P=1) being approximately 1.3× the height of the point at the left (P<1). A curve connecting them is predicted to be monotonically increasing — but the intermediate points (hedged language, partial commitment) are **unmeasured** and uncharted in the guide.

A second diagram shows the technique's claim structure as a dependency chain:
```
[Certainty framing] → [Optimism bias bypass] → [More failure reasons] → [Better prevention strategies] → [Higher project success rate]
```
Each arrow is a claim. Only the first arrow (→ more failure reasons) has empirical support (Klein's 30%). All subsequent arrows are **assumed**, not validated.

---

### Representation 2 — Equation / Formal Expression

Let the technique's core pipeline be:

```
E(outcome) = f(Frame) × g(Team) × h(Facilitator) × k(Culture)
```

where E = effectiveness (reasons identified × prevention quality × follow-through rate).

The guide models this as:
```
E ≈ f(Frame)     [all other factors treated as constant or ignored]
```

The Klein finding gives:
```
f(F_c) / f(F_p) ≈ 1.30
```

But the guide's implicit claim that E(F_c) / E(F_p) ≈ 1.30 requires:
```
g, h, k ≈ constant     [untested assumption]
```

This is the load-bearing hidden assumption: team dynamics, facilitator skill, and organizational culture are treated as non-contributors to variation, which is almost certainly false.

---

### Representation 3 — Procedural Description (Steps to Verify)

To verify the primary claim:

1. Randomly assign teams of 4–12 people to two conditions: (A) certainty framing, (B) possibility framing.
2. Hold constant: project domain, team composition, facilitator, session duration, instructions.
3. Run a Pre-Mortem session; collect all written cards from Step 2 (individual brainstorm).
4. Count unique failure reasons per team (de-duplicated). This replicates Klein's measure.
5. **Additional step not in Klein:** Track which identified failure reasons produced prevention strategies (Step 5). Track which prevention strategies were assigned owners (Step 6). Track which owners implemented their strategy before project execution.
6. Compare group A vs. group B at each stage: reasons identified → strategies developed → strategies implemented → project outcomes.
7. The guide's implicit claim holds only if the ratio ≥ 1.30 is preserved at each stage. If the ratio decays to near 1.0 by the strategy-implementation stage, the 30% figure is technically true but practically irrelevant.

*This procedural description does not exist in the guide. It is the verification protocol that the guide implicitly requires but does not provide.*

</renderings>

---

<constructive_specification>

## Constructive Specification (Turing Minimal Model)

**Full constructive spec:**

An agent that:
1. Assigns two groups of ≥4 people to identical planning tasks with the single manipulation: Group A receives "the project has failed" (certainty), Group B receives "the project might fail" (possibility);
2. Collects written individual responses before any group discussion;
3. Counts unique causal reasons per group (deduplicated);
4. Finds |Group A| ≥ 1.30 × |Group B|

constitutes a **demonstration of the primary claim** (Klein's 30% finding).

**Minimal-model check — can any element be removed?**

- "≥4 people" → cannot be removed; individual variation requires minimum sample.
- "identical planning tasks" → cannot be removed; confound otherwise.
- "single manipulation: framing" → cannot be removed; this is the causal claim.
- "written individual responses before group discussion" → cannot be removed; Step 3 grouping reintroduces anchoring which would contaminate the measure.
- "|Group A| ≥ 1.30 × |Group B|" → cannot be removed; this is the threshold claim.

**Result:** No elements are eliminable. Constructive spec is minimal.

**Determinacy check:** Is the spec fully determinate?

- Ambiguity 1: "unique causal reasons" — requires inter-rater reliability protocol to count consistently. Not specified in the guide. Partially indeterminate.
- Ambiguity 2: "identical planning tasks" — requires matched projects or simulated projects to operationalize. Feasible but not automatic.
- Ambiguity 3: The spec tests only Klein's reason-identification claim. It does not test the guide's implicit extension to prevention effectiveness. A second constructive spec is required for the full claim.

**Second constructive spec (for the guide's extended claim):**

An agent that:
1. Runs the above experiment;
2. Follows both groups through the full 6-step protocol;
3. Tracks: (a) prevention strategies developed, (b) prevention strategies assigned an owner, (c) prevention strategies implemented before project start;
4. Measures project outcome (success/failure, scoped definition required);
5. Finds statistically significant improvement in project outcomes for Group A vs. Group B

constitutes a demonstration of the **guide's implicit extended claim**.

**Status:** This second constructive spec has not been run. The guide asserts its conclusion without having run this test.

</constructive_specification>

---

<flagged_vagueness>

## Flagged Vague Claims

**[VAGUE] "deeper insights"**
— From: "The Pre-Mortem unlocks deeper insights by making failure feel real."
— Reason: At Level 4, "deeper" requires a partial order on an insight space. No such order is defined. "Deeper than what?" is unanswerable without a metric. At Level 5, "deeper insights → prevention" collapses to tautology: better thinking produces better output (trivially true).
— Possible resolution: Specify the metric for insight quality — e.g., number of novel risk categories identified (not duplicated from standard risk register), predictive accuracy of identified risks (how many actually materialized), or actionability score (% of identified risks that produced implemented prevention strategies).

---

**[VAGUE] "makes concerns speakable"**
— From: "Framing failure as 'already happened' gives permission to voice worries."
— Reason: "Permission" is a social construct whose instantiation is entirely dependent on organizational culture, power dynamics, and individual psychology — none of which the technique controls. At Level 4, this claim requires: ∀team members m, certainty framing F_c raises m's probability of voicing concern c by δ > 0. This is almost certainly false for ∀m — it likely holds for some m in some cultures and fails for others. The ∀ quantifier makes it a vague claim.
— Possible resolution: Restrict claim scope: "In low-hierarchy, psychologically safe team cultures, certainty framing increases willingness to voice concerns vs. possibility framing." Requires cultural pre-condition specification.

---

**[VAGUE] "surfaces hidden risks"**
— From: "Surfaces hidden risks — different team members see different risks."
— Reason: "Hidden" is undefined. A risk hidden from whom? If "hidden from the project manager," this is a claim about information asymmetry, not about the Pre-Mortem mechanism per se — the individual brainstorm step could be replaced by anonymous surveys with equivalent effect. At Level 5, the claim reduces to: individual diversity → risk set union > any individual's risk set. This is trivially true (set theory: union ≥ any member) and provides no information about the quality or novelty of the marginal risks surfaced.
— Possible resolution: Specify: "Certainty framing (vs. possibility framing) surfaces risks that individual team members knew but did not consider worth mentioning." This is a precise, testable claim about the framing effect on disclosure, distinct from mere information aggregation.

---

**[VAGUE] "effective facilitation"**
— From: Guide's facilitation tips section, implying facilitation quality determines outcome.
— Reason: Facilitation quality is never operationalized. No observable success criterion is given for "committing to the failure premise" or "making it safe." At Level 4, this is an unquantified moderator variable with no specified measurement instrument.
— Possible resolution: Specify behavioral indicators: facilitator does not use hedged language about the failure premise; facilitator responds to pushback with re-affirmation of the certainty frame; facilitator redistributes speaking time when one voice dominates (measurable as % variance in speaking time). These are observable and checkable.

---

**[VAGUE] "follow through"**
— From: "Follow through on identified strategies" (facilitation tip).
— Reason: The guide names follow-through as a tip but provides no mechanism, timeline, or accountability structure for it beyond Step 6 ownership assignment. "Follow through" at Level 5 either means "owner implements strategy before project start" (a specific, testable outcome) or it means nothing. The guide does not specify this.
— Possible resolution: Define follow-through as: within N days of the Pre-Mortem session, each owner confirms in writing that their assigned prevention strategy has been implemented or explicitly deferred with documented rationale. N must be specified relative to project timeline.

</flagged_vagueness>

---

<forward_chain>

## Consequence Tree

**Root:** An agent running a Pre-Mortem under certainty framing ("the project has failed") produces ~30% more unique failure reasons than under possibility framing, and uses those reasons to generate prevention strategies with assigned owners, timelines, and success measures.

---

### Branch A — When organizational culture is high-blame

A.1: Team members self-censor during individual card-writing (Step 2), knowing outputs will be archived and reviewed.
  A.1.1: The 30% advantage of certainty framing is partially or fully canceled by self-censorship — the measure of "reasons identified" drops toward the possibility-framing baseline.
  A.1.2: Prevention strategies produced are politically safe rather than technically accurate — they identify external and technical risks, not people/process risks involving senior stakeholders.
    A.1.2.1: [PATHOLOGICAL] Step 6 ownership assignment for politically safe prevention strategies creates a Potemkin accountability layer — visible compliance with no risk-reduction effect.
A.2: The Pre-Mortem session is completed and documented, creating a paper trail that later serves as retrospective accountability evidence against the participants who voiced concerns.
  A.2.1: [PATHOLOGICAL] Future teams in the organization learn this dynamic and reduce candor in subsequent Pre-Mortem sessions — the technique degrades over time in high-blame cultures, converging to performative compliance.

---

### Branch B — When the facilitator weakens the certainty framing

B.1: Participants mentally recategorize the session as a standard risk brainstorm (possibility framing) — Klein's 30% advantage is not activated.
  B.1.1: The session produces a risk list qualitatively equivalent to a standard risk register exercise, with no psychological safety benefit beyond normal brainstorming.
  B.1.2: [DEGENERATE] The technique is executed (all 6 steps completed, cards written, owners assigned) but the load-bearing mechanism was never engaged — the Pre-Mortem becomes an elaborate risk list exercise. Output is formally valid but substantively equivalent to the simpler alternative it was meant to improve upon.
B.2: Without the certainty frame, some participants experience the session as ambiguous — "are we predicting this will fail?"
  B.2.1: Defensive behavior emerges: participants whose work is in scope begin to argue against failure scenarios rather than explore them.
  B.2.2: [CONTRADICTION] Branch B.2.1 contradicts the guide's claim that the Pre-Mortem creates "low-stakes hypothetical" safety. Without the certainty frame, the stakes are perceived as real, reversing the psychological safety mechanism. Discriminating condition: certainty framing is the necessary precondition for low-stakes perception, not a feature of the method independent of framing.

---

### Branch C — When voting (Step 4) drives prioritization

C.1: The most emotionally salient risks (familiar, vivid, recent-precedent) receive the most votes regardless of actual probability × impact.
  C.1.1: Resources are allocated to high-visibility, low-probability risks while low-visibility, high-probability systemic risks remain in the discarded long tail.
  C.1.2: [CONTRADICTION] C.1.1 contradicts the guide's implicit claim that the Pre-Mortem surfaces the most important risks. It surfaces the most politically salient risks. Discriminating condition: compare voted-top risks against an independent probability × impact scoring; measure divergence.
C.2: Dominant or senior voices influence how cards are grouped in Step 3, which then influences which clusters receive votes in Step 4.
  C.2.1: [PATHOLOGICAL] Group convergence is reintroduced via the clustering step — anchoring that Step 2 (individual isolation) was designed to prevent is restored by Step 3, undermining the method's own design rationale.

---

### Branch D — When ownership is assigned (Step 6) without commensurate authority

D.1: The assigned owner cannot implement the prevention strategy without resources, budget, or decisions controlled by someone else.
  D.1.1: The owner experiences accountability without agency — the Pre-Mortem has transferred risk awareness without transferring the capacity to act.
  D.1.2: [PATHOLOGICAL] The owner either (a) does nothing (prevention strategy unimplemented), or (b) escalates, which requires the organizational hierarchy the Pre-Mortem was designed to work around.
D.2: The assigned owner implements a watered-down version of the prevention strategy within their authority, creating the appearance of follow-through.
  D.2.1: The actual risk is partially mitigated, but the gap between the stated strategy and the implemented version is not tracked — success measures from Step 6 are satisfied by a weaker intervention than intended.
  D.2.2: [DEGENERATE] Step 6 (ownership assignment) creates false closure — the session ends with the feeling that risks are managed when the management capacity was never verified. This is a structurally stable failure mode: it recurs reliably because it produces subjective satisfaction at session end.

---

### Branch E — When the Rolling Pre-Mortem is used across multiple project stages

E.1: Psychological novelty of certainty framing degrades with repetition — participants have "done this before" and the imaginative commitment to failure-as-real weakens.
  E.1.1: Reason-identification yield decreases per session over time — the 30% advantage attenuates toward zero as habituation sets in.
  E.1.2: Teams develop stock responses: they retrieve the same failure categories session after session, reducing the diversity of risks surfaced.
    E.1.2.1: [DEGENERATE] The Rolling Pre-Mortem converges to a periodic ritual that produces familiar risk lists — substantively equivalent to checking a static risk register, with no prospective hindsight advantage. The technique has been hollowed out by repetition.
E.2: If earlier sessions produced prevention strategies that were implemented, later sessions produce fewer novel failure reasons (because risks were resolved).
  E.2.1: This is a desirable outcome — but it makes the session appear less productive, potentially causing organizations to abandon the technique at exactly the point it is working.

---

### Branch F — When the technique is applied to projects with high interdependency risk

F.1: The individual brainstorm (Step 2) surfaces failure reasons from each domain independently. Cross-domain cascades (failure in component A causing failure in B causing failure in C) require simultaneous reasoning about multiple components — which individual card-writing does not facilitate.
  F.1.1: Interdependency risks are systematically underrepresented in the Pre-Mortem output — the technique's individual-isolation design creates a blind spot for cascade failure modes.
  F.1.2: [PATHOLOGICAL] For complex systems (software integration, supply chains, multi-team programs), the Pre-Mortem may produce high confidence in isolated risk prevention while leaving cascade risks entirely unaddressed. The technique's psychological safety benefit may actively increase overconfidence in the completeness of the analysis.
F.2: The grouping step (Step 3) clusters similar risks, which may collapse cascade failures into a single category, masking their systemic nature.

---

### Branch G — When "wild scenarios" are actively encouraged by the facilitator

G.1: Low-probability, high-imagination failure scenarios dominate the card set and receive votes for being vivid and interesting.
  G.1.1: Resources are allocated to low-probability, high-imagination risks (availability bias amplification) rather than high-probability, mundane risks.
  G.1.2: [CONTRADICTION] G.1.1 contradicts the guide's own facilitation tip to include wild scenarios. The tip creates a mechanism for making the technique worse. Discriminating condition: measure the voted-top-5 risks; determine if they skew toward vivid/unusual or toward statistically common failure categories. If the former, the "wild scenario" tip is net harmful.
G.2: Wild scenarios that are genuinely novel surface failure modes not on any standard risk register.
  G.2.1: This is the intended benefit — but it competes directly with G.1. The facilitator has no reliable way to distinguish valuable novel scenarios from vivid-but-unlikely noise at the time of card collection.

---

### Branch H — When external stakeholders are absent (as in all described variants)

H.1: Failure reasons from outside the team's knowledge boundary (customer behavior, regulatory change, market shifts, partner failures) are systematically underrepresented.
  H.1.1: The resulting prevention strategies address internally-controllable risks but leave externally-driven failure modes unmitigated.
  H.1.2: Teams develop a distorted map of risk — high confidence in coverage, actual coverage limited to the team's information horizon.
H.2: For projects where external factors are the primary failure mode (market-dependent products, regulatory-sensitive projects, supply chain projects), the Pre-Mortem may produce a comprehensive list of internal risks while the actual failure occurs from an external cause that was never on any card.
  H.2.1: [PATHOLOGICAL] Post-project, the Pre-Mortem is cited as "we identified all the risks" — the external failure cause is retroactively attributed to bad luck rather than to the technique's systematic blind spot.

---

### Branch I — When the time constraint (45–60 minutes) is applied to complex projects

I.1: Six structured steps in 45–60 minutes allocates 7–10 minutes per step. Step 5 (prevention strategy development) receives 15–20 minutes for ≥5 risks.
  I.1.1: Prevention strategies are necessarily high-level and underspecified — they do not survive contact with implementation realities.
  I.1.2: [DEGENERATE] The time constraint converts prevention strategy development into slogan generation: "ensure better communication," "clarify requirements early," "get executive buy-in." These are not prevention strategies; they are names for the absence of prevention strategies.
I.2: Teams with complex projects attempt to extend the session, but the guide provides no scaling guidance — only one-size time allocation.
  I.2.1: Extended sessions lose the psychological crispness of the defined format — participants disengage, and the certainty framing weakens over time.

---

### Branch J — When the Pre-Mortem is combined with an existing formal risk management process

J.1: The Pre-Mortem produces a risk list that is separate from the project's official risk register.
  J.1.1: Two parallel risk tracking systems create maintenance overhead and divergence — risks identified in the Pre-Mortem may not be transferred to the register.
  J.1.2: [CONTRADICTION] J.1.1 contradicts the guide's implicit claim that Pre-Mortem outputs drive action. If outputs are not integrated into existing risk management infrastructure, the action-driving mechanism is broken. Discriminating condition: measure what percentage of Pre-Mortem-identified risks appear in the project risk register one week after the session.
J.2: The Pre-Mortem is positioned by the organization as a replacement for formal risk assessment.
  J.2.1: The narrative/story-based format displaces probability × impact analysis entirely.
  J.2.2: [PATHOLOGICAL] For projects requiring regulatory compliance or insurance underwriting, informal story-based risk identification is insufficient — the Pre-Mortem's narrative format creates a liability gap where the formal rigor was required.

---

## Flags Summary

[CONTRADICTION] Branch B.2.2: "Without certainty framing, stakes are perceived as real" contradicts "Pre-Mortem creates low-stakes hypothetical safety." Discriminating condition: certainty framing is a necessary precondition for low-stakes perception.

[CONTRADICTION] Branch C.1.2: "Voting surfaces politically salient risks" contradicts "Pre-Mortem surfaces the most important risks." Discriminating condition: compare voted-top risks against independent probability × impact scoring.

[CONTRADICTION] Branch G.1.2: "Wild scenario encouragement produces vivid/unlikely risk dominance" contradicts the guide's facilitation tip to encourage wild scenarios as beneficial. Discriminating condition: measure post-session vote distribution skew toward vivid vs. statistically common failure categories.

[CONTRADICTION] Branch J.1.2: "Pre-Mortem outputs not integrated into risk register" contradicts "Pre-Mortem drives proactive action." Discriminating condition: measure risk transfer rate from Pre-Mortem cards to formal risk register.

[PATHOLOGICAL] Branch A.1.2.1: Potemkin accountability layer — ownership assignment without candid risk identification produces visible compliance with zero risk-reduction effect.

[PATHOLOGICAL] Branch A.2.1: Technique degrades over time in high-blame cultures due to learned self-censorship; converges to performative compliance.

[PATHOLOGICAL] Branch C.2.1: Group convergence (anchoring) reintroduced by Step 3 clustering, undermining Step 2's individual-isolation design.

[PATHOLOGICAL] Branch D.1.2: Accountability without agency — owner cannot implement strategy; either nothing happens or escalation reinstates the hierarchy the technique was designed to work around.

[PATHOLOGICAL] Branch F.1.2: Individual-isolation design creates systematic blind spot for cascade failures; may increase overconfidence in risk coverage for complex interdependent systems.

[PATHOLOGICAL] Branch H.2.1: External failure cause retrospectively attributed to bad luck; the technique's external-stakeholder blind spot is invisible in post-project analysis.

[PATHOLOGICAL] Branch J.2.2: Pre-Mortem as risk-register replacement creates liability gap in regulatory/compliance contexts.

[DEGENERATE] Branch B.1.2: Technique executed without certainty framing = elaborate risk list exercise substantively equivalent to the simpler alternative it was meant to improve.

[DEGENERATE] Branch D.2.2: Step 6 false closure — structurally stable failure mode; recurs reliably because it produces subjective satisfaction at session end.

[DEGENERATE] Branch E.1.2.1: Rolling Pre-Mortem converges to periodic ritual with static risk list — technique hollowed out by repetition.

[DEGENERATE] Branch I.1.2: Time constraint converts prevention strategy development into slogan generation ("better communication," "earlier requirements clarification").

---

## Tree Statistics

Branches generated: 10 (A–J)
Total nodes: 42
Flags raised: 4 contradiction / 7 pathological / 4 degenerate = 15 total
Pruning applied: None (within 50-node limit)

</forward_chain>

---

## Step 6 Verification

**Constructive-test:** The constructive spec in Step 3 is fully determinate with two noted ambiguities (inter-rater reliability for "unique reason" counting; need for matched-project operationalization). Both ambiguities are resolvable with standard experimental protocols. A second constructive spec for the extended claim is also provided. Determinacy: PASS with noted resolvables.

**Minimal-model check:** All five elements of the constructive spec are non-eliminable. PASS.

**Jargon-purge residue:** Level 1 phrasing contains no unexplained technical terms. "Certainty framing" is introduced at Level 2 as "saying the bad thing already happened" — no residue. PASS.

</precision_forcing>
