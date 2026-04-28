# S2 — Constraint Escape

```xml
<constraint_escape>

<named_constraint>
Optimism bias is structurally embedded in the team doing the planning — the same minds that
generated the plan must also generate reasons to doubt it. Social pressure and cognitive
consistency (identity investment in the plan) suppress divergent failure-thinking from within
the very agent group that needs to produce it.

Structural form: [coupling] — improving plan quality (commitment) degrades failure-imagination
ability; making failure feel real enough requires the planner to contradict their own
investment. Evaluator role and producer role are fused in a single entity.

Secondary equally-binding constraint (marked for V6): the temporal window — the exercise must
occur before failure, when emotional stakes are still low enough to speak freely, yet after
enough plan detail exists to generate specific failure modes. This timing gate is fragile and
rarely maintained under project pressure.
</named_constraint>

<domain_candidates>
Domain-agnostic constraint description (Da Vinci: strip all vocabulary from the problem domain):

"An agent that produced a design must subsequently evaluate the design's failure modes, but
productive investment in the design structurally suppresses the agent's ability to surface
its own design's weaknesses. The evaluation role and the production role are occupied by the
same entity. The agent's bias toward the design is not a personality flaw — it is a
structural consequence of having been the generator."

Parallel survey of 25 catalog domains against this structural form:

1.  Evolutionary Biology         — niche specialization + immune surveillance separate producer
                                   from evaluator roles. VIABLE.
2.  Thermodynamics               — no direct structural match for role-fusion constraint. WEAK.
3.  Fluid Dynamics               — no direct match. WEAK.
4.  Immunology                   — self/non-self discrimination: system trained on self cannot
                                   reliably flag self-derived threats. Self-tolerance is the exact
                                   structural analog. HIGHLY VIABLE.
5.  Information Theory           — error-correcting codes require redundancy that does NOT come
                                   from the original signal — the codeword embeds independent
                                   checking chains. VIABLE.
6.  Architecture / Engineering   — independent structural review; red-team load testing by
                                   parties not involved in design. VIABLE.
7.  Ecology / Food Web           — role differentiation across trophic levels prevents
                                   over-optimization in one direction. MODERATE.
8.  Game Theory / Mechanism Design — changing the payoff matrix so revealing failures is
                                   individually rational, not defective. HIGHLY VIABLE.
9.  Developmental Biology        — no direct match. WEAK.
10. Control Theory / Cybernetics — observer separation principle: state estimator is structurally
                                   independent of controller; you cannot use the same model to
                                   control and to accurately estimate the state it produces.
                                   HIGHLY VIABLE.
11. Crystallography              — grain boundary as deliberate fault site; defect engineering.
                                   MODERATE.
12. Network Science              — structural holes: external broker who spans disconnected
                                   sub-graphs sees information unavailable to either cluster.
                                   VIABLE.
13. Linguistics                  — pragmatic disambiguation layer independent of syntax; the
                                   "failure frame" is a pragmatic override. MODERATE.
14. Epidemiology                 — threshold/R₀ logic: once optimism-suppression exceeds a
                                   threshold, no local intervention can contain it. WEAK.
15. Acoustics / Wave Physics     — destructive interference: a second signal phase-shifted 180°
                                   cancels the first. VIABLE (external counter-signal neutralizes
                                   bias-signal).
16. Economics / Market           — asymmetric information; auction design separates information
                                   production from price discovery. VIABLE.
17. Optics / Photonics           — no direct structural match. WEAK.
18. Psychology / Cognitive Sci   — framing and choice architecture; temporal distancing heuristics.
                                   ON-DOMAIN — confirms the structural form but does not escape it.
19. Pharmacology                 — prodrug: decouples activation from delivery; active agent
                                   generated only under specific environmental trigger. MODERATE.
20. Urban Planning / Traffic     — induced demand: building a road generates the traffic it was
                                   meant to solve. Exact structural analog: planning generates the
                                   optimism it should evaluate. VIABLE.
21. Cryptography / Security      — zero-knowledge proofs: verify a property without the verifier
                                   gaining the prover's private bias. VIABLE.
22. Geology / Plate Tectonics    — slow stress accumulation / rapid release; elastic rebound logic
                                   for timing-gate fragility. VIABLE (secondary constraint).
23. Animal Behavior / Ethology   — sign stimuli unlock fixed-action patterns otherwise suppressed;
                                   the failure-certainty framing acts as a sign stimulus. VIABLE.
24. Computation / Complexity     — oracle separation: computability results that cannot be achieved
                                   by any Turing machine acting on its own output; require external
                                   oracle not constructible from within. HIGHLY VIABLE.
25. Chemistry / Catalysis        — activation energy barrier lowered by a catalyst that does not
                                   come from the substrate itself. VIABLE.

Viable candidates identified: 16 of 25 domains (domains 1, 4, 5, 6, 8, 10, 12, 15, 16, 20,
21, 22, 23, 24, 25, and moderate candidates 7, 11, 13, 19).
Step 2 failure guard: 16 viable > 5 threshold. Continue.
</domain_candidates>

<domain_mappings>

<mapping id="M1">
  Source domain: Immunology
  Target domain: Pre-Mortem methodology (team evaluating its own plan)
  Structural analog: Self-tolerance failure — an immune system trained exclusively on self-
    antigens cannot mount a response to self-derived threats; it must learn to recognize
    the difference between self (legitimate tissue) and non-self (pathogen), but when the
    pathogen mimics self, the immune system is blind to it.
  What solves it in source: Introducing alloreactive T-cells from a separate lineage (MHC
    mismatch); clonal deletion tuned by thymic selection that uses cells NOT from the same
    organism; NK cells that kill when self-MHC is *absent* (rather than flagging non-self
    presence). Solution class: negative-space recognition — flag the ABSENCE of expected
    markers, not the presence of foreign ones.
  Transfer to target: A Pre-Mortem redesigned on NK-cell logic would not ask "what could
    go wrong?" (positive-space recognition — easily suppressed) but instead: "What familiar
    markers of success are absent in this plan?" Missing safety checks, absent dissenting
    voices, and absent contingency lines become failure signals — the plan fails NOT because
    bad things were detected but because expected-good things were not found.
  Structural distinctness check: No surface feature shared with M2–M5 (none involve
    biological recognition or negative-space detection).
</mapping>

<mapping id="M2">
  Source domain: Control Theory — Observer Separation Principle (Luenberger observer,
    Kalman filter separation theorem)
  Target domain: Pre-Mortem methodology
  Structural analog: In optimal control, the separation theorem proves that the state
    estimator (observer) and the controller must be designed independently; the same model
    cannot be used both to drive a system toward a target and to accurately estimate whether
    the system is deviating from that target. Using the controller's model as the observer
    produces systematic blind spots at exactly the deviations the controller is designed to
    suppress.
  What solves it in source: The Luenberger observer is driven by the error between the
    system's actual output and the model's predicted output — crucially, this error is visible
    only if the observer has a gain matrix independent of the control law. Kalman filtering
    adds noise covariance estimation from a model trained on disturbances, not on the nominal
    trajectory.
  Transfer to target: The Pre-Mortem's structural flaw is that it uses the planning model
    (controller) as the failure-detection model (observer). Separation principle applied:
    the failure-detection process must be driven by a model of deviations — scenarios that
    the plan systematically fails to represent — not by the plan itself. Concretely: seed
    the Pre-Mortem with a "disturbance catalog" (failure mode typologies from outside the
    project domain) rather than asking participants to extrapolate from the plan. The observer
    gain comes from prior-project failure databases, not from the current project's logic.
  Structural distinctness check: No surface overlap with M1 (not biological), M3 (not
    incentive-based), M4 (not signal-cancellation), M5 (not computability).
</mapping>

<mapping id="M3">
  Source domain: Game Theory / Mechanism Design
  Target domain: Pre-Mortem methodology
  Structural analog: In mechanism design, the challenge is to construct a game whose Nash
    equilibrium produces socially optimal outcomes even when each player pursues private
    interests. The planner's optimism bias is a dominant strategy in the current game: voicing
    plan doubts signals incompetence or disloyalty; staying silent is privately rational. The
    Pre-Mortem attempts to override this by normative framing ("it's safe to critique here")
    but does not change the underlying payoff matrix, so the Nash equilibrium is unchanged.
  What solves it in source: Mechanism design changes the payoff structure. Classic solutions:
    (a) make the failure-finder's claim verifiable and reward it (prediction markets);
    (b) use adversarial assignment — explicitly assign players to roles whose success depends
    on the failure of the plan, making failure-finding individually rational;
    (c) use second-price auction logic — each participant's contribution is evaluated relative
    to others', so defection (silence) has a cost.
  Transfer to target: Pre-Mortem redesigned as a mechanism: assign each participant a specific
    failure domain they are personally accountable for (adversarial role assignment); their
    performance in the exercise is evaluated on whether their identified risk subsequently
    materialized, and this record is visible. This shifts the dominant strategy from
    "protect the plan" to "find the real failure first." The facilitator becomes a mechanism
    designer, not a permission-giver.
  Structural distinctness check: No surface overlap with M1–M2 or M4–M5. The insight is
    incentive-structural, not biological, control-theoretic, signal-based, or computability-based.
</mapping>

<mapping id="M4">
  Source domain: Acoustics / Wave Physics — Destructive Interference and Active Noise
    Cancellation
  Target domain: Pre-Mortem methodology
  Structural analog: Optimism bias in a team functions as a standing wave — a resonant
    frequency maintained by mutual reinforcement (social proof, shared history, common
    information). Like acoustic resonance, it is self-sustaining and grows louder when
    participants share the same phase. Active noise cancellation works not by suppressing
    the original signal from outside but by injecting a precisely phase-inverted signal from
    within the medium itself — the cancellation wave travels through the same space as the
    noise.
  What solves it in source: The anti-noise signal must be (a) precisely phase-inverted
    relative to the original, (b) injected at the right spatial point (before the ear, not
    after), and (c) continuously updated via adaptive filter as the original signal changes.
  Transfer to target: A structurally analogous intervention inserts a "phase-inverted"
    cognitive signal into the team's shared space before the optimism wave consolidates.
    Mechanism: the facilitator introduces a mandatory "anti-scenario" that is as detailed and
    emotionally vivid as the success scenario. The anti-scenario is not a list of risks (which
    can be dismissed as abstract) but a narrative at exactly the same resolution and
    confidence level as the project plan — a mirror document. The anti-scenario acts as the
    cancellation wave: same medium (narrative), same amplitude (detail level), opposite phase
    (failure). Residual signal after cancellation = genuine, resistant risks.
  Structural distinctness check: Wave / signal domain; no biological, incentive-game,
    control-theory, or computability structure.
</mapping>

<mapping id="M5">
  Source domain: Computation / Complexity Theory — Oracle Separation and Gödel Incompleteness
    (structural analog)
  Target domain: Pre-Mortem methodology
  Structural analog: In computability theory, oracle separation results prove that a Turing
    machine M cannot decide certain properties of its own output using only its own
    computational model — it requires an oracle external to M's tape. Gödel's incompleteness
    theorem shows that within a sufficiently powerful formal system, there exist true
    statements that cannot be proven using only the axioms of that system. The planning team
    is the formal system: they operate within a shared set of assumptions (axioms) that
    define what counts as a risk, what counts as success, and what scenarios are considered
    live. The Pre-Mortem failure modes they can generate are bounded by this axiomatic space.
    Risks that fall outside the team's axiom set — unknown unknowns — are not merely hard
    to think of; they are structurally unreachable within the system.
  What solves it in source: Oracle access — input from a system that was not constructed from
    the same axioms. In computation: relativized complexity classes require oracle gates;
    in practice, this is implemented as external provers, formal verification tools, or
    automated enumeration of cases that the human proof-writer's intuition missed.
  Transfer to target: The Pre-Mortem's failure-identification capacity is bounded by the
    team's axiom set. Structural escape: (a) inject an "external oracle" — participants who
    share no history with the project and have been given only the plan document, not the
    context, motivations, or backstory; (b) use structured enumeration from a domain-external
    failure taxonomy (not generated by the team) as a mandatory survey before free
    brainstorming; (c) treat the team's inability to understand why the oracle's risks seem
    implausible as a signal of axiom-boundary, not as a signal of irrelevance.
  Structural distinctness check: Computability / formal-system domain; no biological,
    control-theory, incentive-game, or wave-physics structure.
</mapping>

</domain_mappings>

<activation_provenance>
M1 — Immunology (negative-space recognition)
  Activated from: domain 4 scan only.
  Provenance: single-source.

M2 — Control Theory / Observer Separation
  Activated from: domain 10 scan only.
  Provenance: single-source.

M3 — Game Theory / Mechanism Design
  Activated from: domain 8 scan only.
  Provenance: single-source.

M4 — Acoustics / Destructive Interference
  Activated from: domain 15 (destructive interference / cancellation wave) AND domain 5
  (error-correcting codes require external redundancy not derived from the original signal).
  Both independently produced the structural insight: "a second signal not generated from
  the original source neutralizes the bias carried in the first signal."
  Provenance: MULTI-SOURCE CONVERGENT (domains 5 + 15).

M5 — Oracle Separation / Incompleteness
  Activated from: domain 24 (oracle separation, computability) AND domain 21 (zero-knowledge
  proofs, cryptographic verification by external party who does not acquire the prover's
  private state / bias).
  Both independently produced the structural insight: "the evaluator must be structurally
  external to the system being evaluated, and the evaluation method must not be derived from
  the system's own model."
  Provenance: MULTI-SOURCE CONVERGENT (domains 24 + 21).

Failure guard check: 2 of 5 mappings have multi-source convergence. Pass.
</activation_provenance>

<conceptual_blend>
<!-- Step 6 tagged M3 (Game Theory / Mechanism Design) as blend-worthy. Step 7 fires. -->

## Input Space 1 — Mechanism Design / Game Theory
Elements: players, payoff matrices, dominant strategies, Nash equilibria, mechanism designer,
  adversarial role assignment, prediction markets, verifiable claims, second-price auctions.
Structure: Each player maximizes private payoff; system designer constructs the game rules
  such that individually rational play produces collectively optimal outcomes. The mechanism
  is valid if truth-telling becomes the dominant strategy.
Relevance: The Pre-Mortem's social suppression of failure-voicing is a game-theoretic
  equilibrium problem, not a normative one.

## Input Space 2 — Pre-Mortem Retrospective (original problem domain)
Elements: planning team, facilitator, plan document, optimism bias, failure framing,
  brainstorming session, prevention strategies, ownership assignment, psychological safety.
Structure: Facilitator creates psychological safety via normative framing; team collectively
  imagines failure; risks are surfaced and prioritized; owners are assigned. Success depends
  on participants voluntarily overriding their own bias.
Note: this is the problem domain.

## Generic Space — Shared Skeleton
Shared structure: Multiple agents hold private information; collective output quality depends
  on whether private information (failure risks / private strategic information) is revealed;
  a designer/facilitator controls the rules of interaction; there is a tension between private
  and collective incentives.
Cross-space mapping:
  — Facilitator ↔ Mechanism designer
  — Team members ↔ Game players with private information
  — Optimism bias (dominant strategy: silence) ↔ Defection strategy in prisoner's dilemma
  — Pre-Mortem session rules ↔ Game rules (the mechanism)
  — Surfacing a risk ↔ Revealing private type in a Bayesian game
  — Prevention strategy ownership ↔ Verifiable commitment

## Blended Space — The Adversarial Pre-Mortem Mechanism
Selective projection:
  - From Input 1: adversarial role assignment; verifiable performance record; payoff structure
    that rewards finding real failures; second-price logic (relative contribution matters)
  - From Input 2: the failure-certainty framing; the facilitated session format; the plan
    document as shared knowledge; the prevention strategy output

Operations applied:
  - Composition: Each team member is assigned a specific failure domain before the session
    (not during). Their role is adversarial to the plan — they are the "prosecutor" for their
    domain. Their performance is evaluated post-project on whether their assigned risks
    materialized. The facilitator sets the payoff rules (evaluation criteria), not just the
    emotional tone.
  - Completion: From game theory, background knowledge activates: prediction markets work
    because individual reputations are staked on accuracy; from Pre-Mortem, background
    knowledge activates: time-boxed sessions, facilitation norms, physical/digital boards.
  - Elaboration: Running the blend — participant A is assigned "external dependency failures."
    Under old rules, A might stay quiet because voicing these risks implies they haven't
    managed these risks well. Under the blended mechanism, A's silence is recorded as a null
    prediction, while B and C who covered A's domain (overlap assignments) surface risks A
    missed. A's track record suffers. The new dominant strategy is to surface risks first
    and most specifically. The facilitator's role shifts from emotional permission-giver to
    rule-setter and auditor.

## Emergent Structure — CRITICAL SECTION
[EMERGENT-SOLUTION] The blend produces a "failure prediction market" structure embedded inside
  the Pre-Mortem format. This does not exist in either input alone: game theory has prediction
  markets but they are asynchronous and anonymous; Pre-Mortems have sessions but no post-hoc
  accountability. In the blend: session + adversarial assignment + post-project scoring =
  a synchronous failure-prediction market with social reputation stakes. This creates
  incentive-compatible risk revelation without requiring participants to suppress their identity
  investment — instead, their identity becomes invested in finding the failure.

[EMERGENT-FRAME] The facilitator's role is reframed not as "creating safety" but as "designing
  the mechanism." This reframe is absent in both source domains independently (game theory
  does not consider group facilitation; Pre-Mortem literature does not consider the facilitator
  as an incentive architect). The blend reveals that the facilitator's most powerful
  intervention is structural, not psychological.

[EMERGENT-CONSTRAINT] The blend reveals a new constraint invisible in either input: adversarial
  role assignment only works if failure domains can be pre-enumerated with enough specificity
  to assign before the session. If the project is too novel or failure domains are undefined,
  the mechanism degrades to the original format. This constraint (pre-enumeration requirement)
  is not visible in game theory (which assumes well-defined strategy spaces) nor in Pre-Mortem
  (which assumes all risks emerge from open brainstorming).

</conceptual_blend>

<escape_path>
Ranking by convergence × structural fit:

Rank 1 (tied): M5 — Oracle Separation
  Convergence: 2 (domains 21 + 24)
  Structural fit: very high — directly names the incompleteness property of the planning
    team's axiomatic space; offers concrete operational mechanism (external oracle participants
    + pre-enumerated external taxonomy)
  Escape: Treat the planning team's failure-imagination as a bounded formal system and
    introduce external axioms (oracle participants, external failure databases) as mandatory
    inputs before team brainstorming. The inability to dismiss oracle inputs is the signal
    of reaching axiom boundaries.

Rank 1 (tied): M4 — Destructive Interference (Acoustic Cancellation)
  Convergence: 2 (domains 5 + 15)
  Structural fit: high — the mirror-document mechanism (anti-scenario at identical resolution
    to the plan) is a directly implementable intervention; the "residual signal after
    cancellation = genuine resistant risk" provides a concrete filtering logic.
  Escape: Create a mandatory "anti-plan" narrative document at equal resolution and
    confidence to the project plan before running the Pre-Mortem session. Anti-plan is
    written by the same team but assigned as a separate creative task. The residual risks
    that survive this cancellation step are the Pre-Mortem's prioritized input, not open
    brainstorming.

Tie rule: Both paths emitted (per failure guard on ties).

Recommended primary path for downstream stages: M5 (Oracle Separation) — because it directly
  addresses the unknown-unknown horizon (axiomatic incompleteness) that the standard
  Pre-Mortem cannot reach by design. M4 is a high-value implementation path for improving
  the Pre-Mortem's signal-to-noise within the known-unknown space.

Mechanism Design blend (M3) feeds as secondary enhancement to both: once external oracle
  or anti-plan inputs are generated, the adversarial assignment mechanism ensures the team
  processes them under incentive-compatible conditions rather than dismissing them as
  implausible.
</escape_path>

</constraint_escape>
```
