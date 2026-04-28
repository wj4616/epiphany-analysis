---
stage_id: S6
stage_name: Falsification Engine
module_version: 1.1.0
status: complete
scale_gate: DEEP
wave: 4
---

<falsification>

<inference_modes>

## Abductive mode results

### Anomaly

The most surprising, unexpected, or hard-to-explain element of this problem: A commission brief must simultaneously serve as an AI-executable specification AND a human-readable rationale for a C++ beginner who cannot evaluate the specifications it contains — yet must make quality priority #1 (DSP audio quality) decisions by ear. The brief must bridge domains the developer cannot navigate alone, while encoding decisions the developer must personally validate.

### Candidate Explanations (scored)

**H-A1: The Brief IS the Architecture**
IF the commission brief encodes sound identity as an architectural invariant, THEN the anomaly resolves because the brief is not a planning document but the system's actual architectural specification. The dual-reader constraint is the brief acting as a bridge system itself — translating creative intent to executable spec.
Parsimony: 0.85 | Explanatory: 0.80 | Uberty: 0.75 | Testability: 0.90
Falsification condition: Produce a brief treating itself as planning-only; if the resulting system architecture differs from what the brief specifies, the brief was insufficiently architectural.

**H-A2: MVQ Threshold Encoding**
IF the brief encodes minimum viable quality thresholds per phase rather than feature requirements, THEN the C++ beginner can evaluate quality by ear (pass/fail against perceptual criteria) without needing to evaluate code quality. The quality priority resolves into testable gates.
Parsimony: 0.90 | Explanatory: 0.70 | Uberty: 0.55 | Testability: 0.85
Falsification condition: Remove MVQ thresholds; if developers cannot verify quality priority #1 without DSP expertise, thresholds are necessary.

**H-A3: Perceptual Vocabulary as Universal Interface Contract**
IF the brief's vocabulary section IS the system's actual interface (not a glossary), THEN each domain implements the same perceptual terms differently but references the same vocabulary entry. The brief defines API contracts in perceptual language.
Parsimony: 0.85 | Explanatory: 0.75 | Uberty: 0.70 | Testability: 0.90
Falsification condition: Use different vocabulary in different domains; if the bridge chain produces incorrect mappings, a unified vocabulary is necessary.

**H-A4: Creative Intent as Identity Preservation**
IF the verbatim creative intent constraint exists because any paraphrase destroys the bridge chain's integrity, THEN the cache key is not just a lookup mechanism but an identity anchor. The creative intent phrase IS the system's unit of identity.
Parsimony: 0.80 | Explanatory: 0.65 | Uberty: 0.50 | Testability: 0.85
Falsification condition: Allow paraphrase of cache keys; if bridge chain mappings produce inconsistent results, verbatim preservation is necessary.

**H-A5: Brief as Kalman Filter Schedule**
IF the brief specifies a calibration schedule (early phases: 80% KB model weight, 20% ear weight; later phases: 20% KB, 80% ear), THEN the developer's improving competence becomes part of the architecture, resolving the "C++ beginner making quality #1 decisions" paradox.
Parsimony: 0.70 | Explanatory: 0.85 | Uberty: 0.90 | Testability: 0.75
Falsification condition: Remove calibration schedule from the brief; if developers make poor model-vs-ear decisions at phase transitions, the schedule is necessary.

**H-A6: Scope Exclusion as Competitive Moat**
IF the scope exclusions (no reverb/EQ/compression, no visual-UI plugins) are the primary positioning mechanism, THEN exclusion IS the product definition strategy — the brief defines the product by what it is NOT.
Parsimony: 0.85 | Explanatory: 0.70 | Uberty: 0.55 | Testability: 0.80
Falsification condition: Remove scope exclusions; if the resulting product competes in saturated segments and fails to differentiate, exclusions are strategically necessary.

**H-A7: Forward-Only as Thermodynamic Constraint**
IF the forward-only bridge direction reflects a fundamental property of creative processes (creative intent cannot be recovered from artifacts), THEN reverse traversal is not just architecturally excluded but physically impossible — the brief should encode this as a thermodynamic constraint.
Parsimony: 0.90 | Explanatory: 0.60 | Uberty: 0.50 | Testability: 0.70
Falsification condition: Attempt reverse traversal on a completed bridge chain; if creative intent can be recovered, forward-only is a design choice, not a constraint.

### Top 3 Abductive Candidates

1. **H-A1: The Brief IS the Architecture** — combined score: 0.83 | falsification: produce a planning-only brief and test whether architecture diverges
2. **H-A5: Brief as Kalman Filter Schedule** — combined score: 0.80 | falsification: remove calibration schedule and test model-vs-ear decision quality
3. **H-A3: Perceptual Vocabulary as Universal Interface Contract** — combined score: 0.80 | falsification: use domain-specific vocabulary and test bridge mapping accuracy

## Deductive mode results

### Premises (from S1 facts)

- P1: Developer is a C++ beginner
- P2: Developer has no DSP expertise
- P3: AI presents DSP options with perceptual descriptors; developer selects by DAW listening test
- P4: DSP audio quality is priority #1
- P5: Bridge chain is forward-only
- P6: Creative intent phrases preserved verbatim as cache keys
- P7: Brief must serve dual-reader format (AI-executable + developer rationale)
- P8: Budget is $100-200/month
- P9: Each phase must produce a functional milestone
- P10: Tier 1 KB must complete before development starts

### Derived Consequences

- **D1**: P1 + P3 => Brief MUST specify perceptual descriptors a C++ beginner can evaluate by ear, not code comprehension. **Holds: YES**
- **D2**: P4 + P1 => Brief MUST prioritize DSP quality specifications in terms a non-expert can verify (DAW listening tests with perceptual criteria). **Holds: YES**
- **D3**: P5 + P6 => Brief MUST define creative intent vocabulary upfront because forward-only chain cannot retroactively define terms. **Holds: YES**
- **D4**: P7 + P1 => Developer-rationale sections MUST use perceptual language, not C++ jargon. **Holds: YES**
- **D5**: P8 + P9 => Each phase milestone MUST be achievable within the monthly budget window. **Holds: PARTIALLY — brief lacks explicit per-phase budget decomposition.**
- **D6**: P4 + P5 => Bridge chain structural completeness requires every creative intent phrase to have complete forward mapping. Any gap is a Tier 1 KB gap. **Holds: YES**
- **D7**: P3 + P5 => AI-presented DSP options must include a creative intent phrase (cache key) that the developer can reference when selecting. **Holds: YES**
- **D8**: P10 + P4 => Tier 1 KB must include sufficient perceptual vocabulary for the developer to make DSP quality decisions. **Holds: YES**

### Flagged Premises

- **D5 partial hold**: The global budget constraint ($100-200/month) is not decomposed per phase. If each phase must produce a functional milestone within budget, the brief should specify per-phase budget awareness.

### Alternative Hypotheses from Consequence Testing

- **H-D1: Per-phase budget decomposition is a necessary brief section.** If each phase must produce a functional milestone within a $100-200/month budget, the brief must specify which phase activities fit within that window. Premise challenged: the budget constraint is global but the milestone constraint is per-phase.
- **H-D2: Tier 1 KB sound design vocabulary completeness is a blocking gate condition.** If Tier 1 KB does not include sufficient perceptual descriptor mappings for the creative focus domains (psychedelic, ambient, pad, creative/abstract, psybient), the quality priority cannot be enforced. Premise challenged: Tier 1 KB completion must explicitly include vocabulary sufficiency verification.

## Inductive mode results

### Observed Instances

I1-I10: (1) 7-domain brief scope, (2) quality priority ordering, (3) C++ beginner evaluates by ear, (4) forward bridge chain, (5) $100-200/month budget, (6) per-phase milestones, (7) Tier 1 KB gate, (8) scope exclusions, (9) verbatim creative intent, (10) dual-reader format.

### Generalizations

**G1: Resource allocation follows quality priority** — Across all domains, resource allocation should follow the same priority ordering (DSP first, then sound design, etc.). The brief should allocate more specification depth to higher-priority domains.
ROBUST — holds across all phases and domains.

**G2: The brief is a translation layer, not a specification layer** — Across all interface instances, the brief's function is to translate between creative intent and executable specification, not to specify implementation. Creative language in, executable language out.
ROBUST — holds for DSP decisions, bridge entries, dual-reader format.

**G3: Every constraint is either a quality gate or a scope boundary** — All 18 irreducible constraints classify as either quality gates (things that must be true for viability) or scope boundaries (things explicitly excluded). No other constraint type exists.
ROBUST — verified across all irreducible constraints.

**G4: Perceptual language is the system's lingua franca** — All cross-domain communication uses perceptual vocabulary.
LIMITED TO creative and evaluation domains. Does not generalize to legal/commercial domains (licensing, anti-piracy). Boundary failure: licensing uses legal language, C++ reference uses technical language.

**G5: Phase milestones are the brief's unit of currency** — Every verification of brief completeness can be expressed as "does this phase produce its milestone?"
LIMITED TO phase-specific content. Cross-cutting concerns (bridge system, vocabulary, KB architecture) span all phases and need separate organization.

### Alternative Hypotheses from Failed Generalizations

- **H-I1: Two vocabulary strata** — The brief has two strata: perceptual (creative/evaluation domains — DSP, sound design, UI) and structural (legal/commercial/architectural domains — licensing, KB architecture, technical stack). The brief must define how these strata interface. From G4 boundary failure.
- **H-I2: Phase-milestone AND cross-cutting matrix organization** — The brief needs both vertical organization (phase milestones) and horizontal organization (cross-cutting concerns: bridge system, vocabulary, KB architecture). Pure sequential organization misses cross-cutting dependencies. From G5 boundary failure.

</inference_modes>

<consolidated_hypotheses>

## Consolidated hypotheses

### H-1: Brief as Architecture (Architecture-IS-Brief)

The commission brief is the system's architectural specification, not a planning document. Sound identity determines architecture (CN8), perceptual vocabulary defines interfaces (CN1), scope exclusions define system boundaries (CN3), and the forward-only bridge is a thermodynamic constraint (CN10). The brief encodes these as architectural requirements, not aesthetic preferences.

- **Confidence:** 0.90
- **Supporting modes:** Abductive (H-A1), Inductive (G2 — translation layer)
- **Falsification:** Produce a brief treating itself as planning-only; if the resulting system architecture diverges from what the brief specifies, the brief must be architectural.

### H-2: Dual-Vocabulary Interface Contract

The brief must define two vocabulary strata: perceptual (creative/evaluation domains — DSP, sound design, UI, marketing language) and structural (legal/commercial/architectural — licensing, KB architecture, technical stack, error correction protocols). The brief's primary interface function is translating between these strata, with the bridge system handling perceptual-to-structural conversion.

- **Confidence:** 0.82
- **Supporting modes:** Abductive (H-A3), Deductive (H-D2), Inductive (H-I1)
- **Falsification:** Write the brief with only perceptual vocabulary; if licensing/commercial/architectural sections are vague or unusable, a second vocabulary stratum is needed.

### H-3: Brief as Kalman Filter Schedule (Calibration Schedule)

The brief should specify a calibration schedule: early phases rely heavily on KB model predictions (80% model weight), later phases shift to developer perceptual measurement (80% ear weight). This makes the developer's improving competence part of the architecture, resolving the paradox of a C++ beginner making quality #1 decisions.

- **Confidence:** 0.78
- **Supporting modes:** Abductive (H-A5), Deductive (H-D2 — Tier 1 vocabulary as gate), Inductive (G1 — resource allocation follows priority)
- **Falsification:** Remove calibration milestones from the brief; if developers make poor model-vs-ear decisions at phase transitions, the schedule is necessary.

### H-4: Scope Exclusion as Competitive Moat

The scope exclusions (no reverb/EQ/compression, no visual-UI plugins) are not constraints but the primary product positioning mechanism. In an ecology where saturated segments consume development resources for marginal differentiation, the exclusion IS the competitive strategy. The brief should frame exclusions as strategic definitions, not limitations.

- **Confidence:** 0.72
- **Supporting modes:** Abductive (H-A6), Inductive (G3 — constraints as quality gates or scope boundaries)
- **Falsification:** Remove scope exclusions; if the resulting product competes in saturated segments and fails to differentiate, exclusions are strategically necessary.

### H-5: Phase-Milestone + Cross-Cutting Matrix Organization

The brief should be organized as a matrix: phase milestones on the vertical axis (per-phase functional deliverables) and cross-cutting concerns on the horizontal axis (bridge system, vocabulary, KB architecture). Pure sequential organization misses cross-cutting dependencies that span all phases.

- **Confidence:** 0.68
- **Supporting modes:** Inductive (H-I2), Deductive (H-D1 — per-phase budget)
- **Falsification:** Write the brief as a pure sequence; if cross-cutting concerns are specified redundantly across phases or dependencies are missed, matrix organization is needed.

### H-6: Per-Phase Budget Decomposition

The brief must decompose the global $100-200/month budget into per-phase allocations. Without per-phase budget estimates, the "each phase produces a functional milestone" constraint cannot be verified against budget reality.

- **Confidence:** 0.50
- **Combined score:** 0.50 — below development threshold, reasoning used for consolidation only.

H-6 — Per-phase budget decomposition: combined score 0.50 — below development threshold, reasoning used for consolidation only.

</consolidated_hypotheses>

<extreme_scenarios>

## Extreme scenarios

Testing the primary conclusion (the brief should encode the consolidated architectural insights) at boundary regimes:

| Regime | Primary Conclusion Behavior | Failure Analysis |
|--------|----------------------------|------------------|
| Budget -> 0 | FAILS — The AI-assisted development model collapses entirely at $0. The brief has no budget floor fallback. | Reveals: the brief needs a minimum viable budget specification and a recommended budget specification. At $0, the brief should specify what is still achievable (manual development with free tools). |
| Budget -> infinity | DEGENERATE — With unlimited budget, the priority ordering becomes irrelevant, the calibration schedule unnecessary (hire experts), and scope exclusions become strategic choices not budget-driven ones. | Reveals: some architectural decisions are budget-conditioned. The brief should mark which decisions hold regardless of budget (quality priority, forward-only bridge, verbatim cache keys) and which are budget-dependent (per-phase milestone timing, AI tool reliance). |
| Population -> 1 (solo developer) | HOLDS — The brief is designed for solo AI-assisted development. At N=1, all conclusions hold. | Confirmed: the brief's architecture is correctly coupled to the solo+AI model. |
| Population -> infinity (large team) | FAILS — Dual-reader format changes (multiple C++ developers need different specs), calibration schedule changes (different developers at different skill levels), and AI-presents-developer-selects boundary changes (DSP experts don't need AI presentation). | Reveals: the brief is architecturally coupled to the solo+AI model. Should document this coupling explicitly as a scope boundary. |
| Time -> 0 (instant) | DEGENERATE — The calibration schedule is meaningless (no time for developer skill growth), and per-phase milestones collapse to a single deliverable. | Reveals: the calibration schedule requires minimum phase durations for skill calibration. The brief should specify time-dependent and time-independent components. |
| Time -> infinity (infinite development) | PARTIAL — Quality priority and scope exclusions still hold, but budget constraint becomes meaningless. Per-phase milestones still provide structure, but urgency drivers vanish. | Reveals: the brief needs both time-independent principles (quality priority, scope, forward-only bridge) and time-dependent specifications (budget, milestones, calibration schedule). |
| Adversarial scenario | FAILS — An adversary exploiting known priorities could: (1) attack DSP quality first (priority #1 failure cascades), (2) target the solo developer bottleneck (single point of failure), (3) exhaust the $200/month budget. | Reveals: three adversarial vulnerabilities — single-developer dependency, quality ordering creates attack surface at priority #1, fixed budget limits defensive responses. The brief should include adversarial resilience considerations. |
| Random/noisy scenario | PARTIAL — Perceptual vocabulary still functions as a translation layer, but the verbatim cache key system breaks (random inputs create meaningless cache keys). Forward-only direction still works, but noisy entries produce noisy outputs. | Reveals: the brief needs input validation criteria for creative intent phrases — not all phrases deserve cache key status. A "creative intent quality gate" should filter noise before bridge chain entry. |

</extreme_scenarios>

<millikan_assessment>

## Millikan assessment

Differential-effort detection for each hypothesis vs. the primary conclusion (H-1: Brief as Architecture):

**[MILLIKAN] H-1 (Brief as Architecture)**
- Effort to refute: MEDIUM — Well-supported by S3 convergent nodes (CN8, CN10, CN6). Refuting requires showing a planning-only brief can still produce good architecture.
- Effort to support primary: LOW — Aligns with quality priority and S3 illumination.
- Asymmetry: NO — Both sides receive reasonable scrutiny.

**[MILLIKAN] H-2 (Dual-Vocabulary Interface)**
- Effort to refute: MEDIUM — Requires showing perceptual vocabulary alone covers all domains, including licensing.
- Effort to support primary: LOW — Intuitively, legal/commercial domains use different language.
- Asymmetry: YES — Low effort to support, medium to refute. The hypothesis is favored too easily. Counter-evidence: many marketing strategies DO use perceptual language ("warm sound," "deep bass"). The structural vocabulary may be narrower than proposed.
- Action: Seek evidence that perceptual vocabulary can cover marketing/licensing domains before committing to two strata.

**[MILLIKAN] H-3 (Kalman Filter Schedule)**
- Effort to refute: HIGH — Requires showing a fixed model-to-ear ratio works throughout development, or that developer skill improvement is not measurable.
- Effort to support primary: LOW — The Kalman filter metaphor is appealing and matches S2's emergent structure.
- Asymmetry: YES — The metaphor is seductive. The schedule assumes developer perceptual skill improvement is predictable and measurable, which may not hold.
- Action: Verify that perceptual skill improvement can be measured before committing to weight percentages.

**[MILLIKAN] H-4 (Scope Exclusion as Competitive Moat)**
- Effort to refute: MEDIUM — Requires showing exclusions are budget-driven, not strategic.
- Effort to support primary: LOW — CN3 supports creative differentiation as competitive moat.
- Asymmetry: SLIGHT — Some counter-evidence exists: successful VST companies DO offer reverbs and EQs (FabFilter, Valhalla). The exclusion may be budget-driven rather than purely strategic.
- Action: Test whether the scope exclusions hold strategic value independent of budget constraints.

**[MILLIKAN] H-5 (Matrix Organization)**
- Effort to refute: LOW — A sequential brief with cross-cutting references could achieve the same without matrix complexity.
- Effort to support primary: MEDIUM — The brief is a single document; matrix organization may add unnecessary structural complexity.
- Asymmetry: NO — Both sides receive reasonable scrutiny.

**[MILLIKAN] H-6 (Per-Phase Budget Decomposition)**
- Effort to refute: MEDIUM — Requires showing that global budget management works without per-phase allocation.
- Effort to support primary: LOW — It's intuitively obvious that per-phase budgets help.
- Asymmetry: SLIGHT — Per-phase budgets may be premature estimation for a commission brief. The brief should specify budget awareness, not per-phase dollar amounts.

</millikan_assessment>

<density_report>

## Density report

Disconfirming evidence found: 10 items

**Counter-examples:**
1. Many successful VST plugins were developed by solo developers WITHOUT a formal commission brief or architectural specification (Valhalla DSP early products, u-he early synths).
2. Several successful VST plugins launched with minimal UI polish (early Vital synthesizer) and succeeded on DSP quality alone.
3. Other successful VST plugins succeeded WITH poor DSP quality but excellent UI/branding (some sample pack players).
4. Agile development success stories show architecture emerging from iterative code, not upfront specification.

**Failure classes:**
5. Creative vision changes mid-development (forward-only constraint prevents pivoting).
6. Creative intent vocabulary is incomplete at Phase 0 (new sonic terms emerge during development).
7. AI tool ecosystem changes (pricing, new tools, model capability shifts).
8. Target market shifts (psybient becomes mainstream, losing niche differentiation).

**Theoretical objections:**
9. The dual-vocabulary hypothesis may be over-engineered — a single well-constructed perceptual vocabulary with appropriate mapping functions might cover all domains.
10. The Kalman filter schedule assumes developer skill improvement is predictable and measurable, which may not hold for perceptual skills that plateau or develop non-linearly.

**Strongest objection:** The bridge chain's forward-only constraint has no mechanism for creative intent evolution — when "warm ambient pad" develops a new connotation during development, the system has no way to update the cache key without violating the forward-only constraint.

**Weakest link in mechanism:** The calibration schedule has no measurable definition of "developer skill improvement," making the model-to-ear weight shift unverifiable.

**Density assessment:** HIGH (10 items)

</density_report>

</falsification>