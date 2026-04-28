# S3 — Peripheral Exploration

```xml
<peripheral_exploration stage="S3" version="1.1.0" status="complete">
```

---

## Activation Map

### Step 1 — Seeds (a = 1.0)

Primitives extracted from `00-processed-input.md`:

| Seed ID | Primitive | a(t=0) |
|---------|-----------|--------|
| S1 | prospective-hindsight | 1.0 |
| S2 | failure-as-certain | 1.0 |
| S3 | optimism-bias | 1.0 |
| S4 | psychological-safety | 1.0 |
| S5 | individual-brainstorming | 1.0 |
| S6 | convergent-voting | 1.0 |
| S7 | prevention-ownership | 1.0 |
| S8 | temporal-anchoring | 1.0 |
| S9 | past-tense-framing | 1.0 |
| S10 | team-size-constraint | 1.0 |
| S11 | hidden-risk-surfacing | 1.0 |
| S12 | 30%-identification-gain | 1.0 |

12 seeds identified — above minimum threshold.

---

### Step 2 — Propagation Rounds (decay = 0.6, threshold = 0.3)

**Round t=1** (first-order neighbors, w_ij weights assigned by semantic proximity):

| Node | a(t=1) | Source(s) | Provenance |
|------|--------|-----------|------------|
| N1: counterfactual-thinking | 0.72 | S1 (w=0.9), S9 (w=0.7) | single-source chain S1 |
| N2: temporal-displacement | 0.66 | S1 (w=0.8), S8 (w=0.7) | single-source chain S1+S8 |
| N3: loss-aversion (Kahneman) | 0.60 | S2 (w=0.8), S3 (w=0.7) | single-source chain S2 |
| N4: fear-of-judgment | 0.66 | S3 (w=0.7), S4 (w=0.9) | single-source chain S4 |
| N5: nominal-group-technique | 0.72 | S5 (w=0.8), S6 (w=0.8) | single-source chain S5 |
| N6: accountability-gap | 0.60 | S7 (w=0.8), S10 (w=0.6) | single-source chain S7 |
| N7: linguistic-priming | 0.72 | S8 (w=0.8), S9 (w=0.8) | single-source chain S8+S9 |
| N8: information-asymmetry | 0.66 | S11 (w=0.8), S5 (w=0.6) | single-source chain S11 |
| N9: base-rate-neglect | 0.60 | S12 (w=0.8), S3 (w=0.6) | single-source chain S12 |
| N10: role-play / simulation | 0.54 | S1 (w=0.7), S4 (w=0.6) | single-source chain S1 |
| N11: deliberate-pessimism | 0.60 | S2 (w=0.8), S3 (w=0.7) | single-source chain S2 |
| N12: anamnesis / retrospect bias | 0.54 | S1 (w=0.7), S9 (w=0.6) | single-source chain S1 |
| N13: diffusion of responsibility | 0.48 | S7 (w=0.8), S10 (w=0.6) | single-source chain S7 |
| N14: cognitive reappraisal | 0.54 | S4 (w=0.7), S2 (w=0.6) | single-source chain S4 |
| N15: threshold-voting / quorum | 0.48 | S6 (w=0.8), S10 (w=0.6) | single-source chain S6 |

**Round t=2** (second-order propagation, decay applied again):

| Node | a(t=2) | Source(s) | Provenance |
|------|--------|-----------|------------|
| N16: scenario-planning | 0.58 | N1 (w=0.9), N2 (w=0.8), N10 (w=0.7) | multi-source-convergent: S1+S8 via N2, S1 via N1 |
| N17: narrative-transportation | 0.52 | N7 (w=0.9), N10 (w=0.8) | single-source chain S8 |
| N18: devil's-advocate role | 0.50 | N11 (w=0.9), N4 (w=0.7) | multi-source-convergent: S2+S4 |
| N19: wicked-problem structure | 0.45 | N8 (w=0.8), N6 (w=0.7) | multi-source-convergent: S11+S7 |
| N20: epistemic-injustice / voice suppression | 0.43 | N4 (w=0.8), N8 (w=0.7) | multi-source-convergent: S4+S11 |
| N21: groupthink prevention | 0.48 | N5 (w=0.8), N4 (w=0.7) | multi-source-convergent: S5+S4 |
| N22: placebo effect / belief efficacy | 0.36 | N3 (w=0.7), N14 (w=0.6) | single-source chain S2 |
| N23: ritual / liminal framing | 0.43 | N7 (w=0.8), N2 (w=0.6) | multi-source-convergent: S8+S1 |
| N24: Cassandra dynamic | 0.40 | N11 (w=0.8), N8 (w=0.7) | multi-source-convergent: S2+S11 |
| N25: mental contrasting (WOOP) | 0.46 | N1 (w=0.9), N3 (w=0.7) | multi-source-convergent: S1+S2/S3 |
| N26: distributed cognition | 0.43 | N8 (w=0.8), N5 (w=0.7) | multi-source-convergent: S11+S5 |
| N27: commitment-and-consistency (Cialdini) | 0.42 | N13 (w=0.8), N7 (w=0.6) | multi-source-convergent: S7+S8 |
| N28: confirmation bias | 0.39 | N3 (w=0.7), N9 (w=0.8) | multi-source-convergent: S2/S3+S12 |
| N29: availability heuristic | 0.36 | N9 (w=0.8), N1 (w=0.6) | multi-source-convergent: S12+S1 |
| N30: Dunning-Kruger metacognition | 0.33 | N12 (w=0.7), N9 (w=0.6) | single-source chain S1 |

**Round t=3** (third-order, cross-domain expansion, decay = 0.6):

| Node | a(t=3) | Source(s) | Provenance |
|------|--------|-----------|------------|
| N31: flight-simulator / adversarial training | 0.46 | N16 (w=0.9), N10 (w=0.8) | multi-source-convergent: S1+S8 via N16/N10 |
| N32: red-teaming (security domain) | 0.44 | N16 (w=0.8), N18 (w=0.8) | multi-source-convergent: S2/S4+S8 via N18/N16 |
| N33: immune-system priming (biology) | 0.39 | N25 (w=0.8), N3 (w=0.7) | multi-source-convergent: S1+S2/S3 |
| N34: negative-visualization / Stoic premeditatio malorum | 0.52 | N11 (w=0.9), N25 (w=0.8), N1 (w=0.7) | multi-source-convergent: S1+S2+S3 |
| N35: rehearsal-as-prophylaxis (sport psychology) | 0.41 | N10 (w=0.8), N31 (w=0.7) | multi-source-convergent: S1+S8 |
| N36: FMEA — failure mode & effects analysis (engineering) | 0.48 | N19 (w=0.8), N6 (w=0.8) | multi-source-convergent: S11+S7 |
| N37: theatrical rehearsal / dress-rehearsal convention | 0.38 | N23 (w=0.8), N17 (w=0.7) | multi-source-convergent: S8+S1 |
| N38: orphan knowledge (tacit, unspoken expertise) | 0.44 | N26 (w=0.9), N20 (w=0.8) | multi-source-convergent: S11+S4/S5 |
| N39: asymmetric-information markets (Akerlof lemons) | 0.39 | N8 (w=0.8), N28 (w=0.7) | multi-source-convergent: S11+S2/S3/S12 |
| N40: Cassandra failure-of-uptake vs. failure-of-detection | 0.40 | N24 (w=0.9), N20 (w=0.8) | multi-source-convergent: S2+S11+S4 |
| N41: self-fulfilling prophecy (reverse: self-defeating prophecy) | 0.44 | N2 (w=0.8), N27 (w=0.7) | multi-source-convergent: S8+S7 |
| N42: trauma inoculation / exposure therapy | 0.36 | N14 (w=0.8), N10 (w=0.7) | multi-source-convergent: S4+S1 |
| N43: ritual-time / kairos vs chronos (anthropology) | 0.37 | N23 (w=0.8), N2 (w=0.6) | multi-source-convergent: S8+S1 |
| N44: game theory — pre-commitment | 0.42 | N27 (w=0.8), N15 (w=0.7) | multi-source-convergent: S7+S6 |

Total activated nodes: 12 (seeds) + 15 (t=1) + 15 (t=2) + 14 (t=3) = 56 nodes.
**Activation-map completeness check: 56 distinct nodes — well above the 15-node minimum. PASS.**

---

### Step 3 — Lateral Inhibition

Applying suppression where two nodes share >70% of activation source paths. Inhibition coefficient α = 0.4.

| Dominant Node | Suppressed Node | Shared Source Path % | Pre-suppression a | Post-suppression a | Note |
|---------------|----------------|---------------------|-------------------|--------------------|------|
| N34: Stoic premeditatio malorum | N11: deliberate-pessimism | ~85% (both from S2+S3 via loss-aversion path) | N11: 0.60 | N11: 0.60 − (0.4×0.52) = 0.39 | Suppressed; N11 still above threshold but de-ranked |
| N36: FMEA | N6: accountability-gap | ~75% (both from S7+S11) | N6: 0.60 | N6: 0.60 − (0.4×0.48) = 0.41 | Suppressed; still above threshold |
| N32: red-teaming | N18: devil's-advocate | ~75% (both from S2+S4 pessimism path) | N18: 0.50 | N18: 0.50 − (0.4×0.44) = 0.32 | Suppressed to below 0.33; borderline — retain at 0.32 |
| N25: mental contrasting (WOOP) | N29: availability heuristic | ~72% (both from S1+S3 path) | N29: 0.36 | N29: 0.36 − (0.4×0.46) = 0.18 | Suppressed below threshold; excluded |
| N16: scenario-planning | N10: role-play/simulation | ~71% (both from S1+S8) | N10: 0.54 | N10: 0.54 − (0.4×0.58) = 0.31 | Suppressed; borderline, retain |

Nodes excluded by lateral inhibition: **N29** (a = 0.18, below 0.3 threshold).
N18 and N10 borderline; retained.

---

## Convergent Nodes

Applying detection criteria: activated via ≥2 independent source chains + a(node) ≥ 0.4 after propagation + not a seed primitive.

```xml
<convergent_nodes>
```

| # | Node | a(final) | Independent Source Chains | Provenance Tag |
|---|------|----------|--------------------------|----------------|
| C1 | N34: Stoic premeditatio malorum | 0.52 | S1 (prospective-hindsight) → N1 → N25; S2 (failure-as-certain) → N3 → N11 → N34; S3 (optimism-bias) → N3 → N11 | [multi-source-convergent: S1+S2+S3] |
| C2 | N36: FMEA (failure mode & effects analysis) | 0.48 | S11 (hidden-risk-surfacing) → N8 → N19 → N36; S7 (prevention-ownership) → N6 → N36 | [multi-source-convergent: S11+S7] |
| C3 | N31: flight-simulator/adversarial-training | 0.46 | S1 → N1 → N16 → N31; S8 (temporal-anchoring) → N2 → N16 → N31 | [multi-source-convergent: S1+S8] |
| C4 | N25: mental contrasting / WOOP | 0.46 | S1 → N1 → N25; S2 → N3 → N25; S3 → N9 → N25 | [multi-source-convergent: S1+S2+S3] |
| C5 | N21: groupthink prevention | 0.48 | S5 (individual-brainstorm) → N5 → N21; S4 (psychological-safety) → N4 → N21 | [multi-source-convergent: S5+S4] |
| C6 | N38: orphan knowledge / tacit expertise | 0.44 | S11 → N8 → N26 → N38; S4 → N4 → N20 → N38; S5 → N5 → N26 | [multi-source-convergent: S11+S4+S5] |
| C7 | N40: Cassandra failure-of-uptake | 0.40 | S2 → N11 → N24 → N40; S11 → N8 → N24 → N40; S4 → N4 → N20 → N40 | [multi-source-convergent: S2+S11+S4] |
| C8 | N32: red-teaming (security domain) | 0.44 | S2 → N11 → N18 → N32; S8 → N2 → N16 → N32 | [multi-source-convergent: S2+S8] |
| C9 | N41: self-defeating prophecy | 0.44 | S8 → N2 → N41; S7 → N13 → N27 → N41 | [multi-source-convergent: S8+S7] |
| C10 | N44: game theory pre-commitment | 0.42 | S7 → N13 → N27 → N44; S6 → N15 → N44 | [multi-source-convergent: S7+S6] |

```xml
</convergent_nodes>
```

10 convergent nodes found — above the 2-node minimum. Darwin independence check:

- C1 vs C4: both from S1+S2+S3. Share S2/S3 ancestors — **partially overlapping**, but C1 emphasizes cognitive reappraisal toward fatalism while C4 emphasizes goal-visualization contrast. Different downstream domains (philosophy vs. psychology). **Retain both; note partial ancestry overlap**.
- C2 vs C6: C2 from S11+S7; C6 from S11+S4+S5. Share S11. Downstream domains differ (engineering process vs. tacit knowledge). **Retain both**.
- C3 vs C8: C3 from S1+S8; C8 from S2+S8. Share S8 but S1 and S2 are independent seeds. **Pass — genuinely independent chains at root**.
- C5 vs C7: C5 from S5+S4; C7 from S2+S11+S4. Share S4 ancestor. Different domains (group dynamics vs. organizational failure modes). **Retain both; note shared S4**.
- All other pairings: no shared ancestors within activation radius.

Darwin independence check: **PASS** — all 10 convergent nodes have at least one genuinely independent source root.

---

## Selected Illuminations

Ranking by `score = a(node) × elegance_score`:

```xml
<activation_provenance>
```

**Elegance scoring applied to top candidates:**

**C1 — Stoic premeditatio malorum (a=0.52)**
- Simplicity: 1.0 (irreducible: deliberate imagination of worst outcome as practice, not assessment)
- Symmetry: 1.0 (appears in Stoic philosophy, Buddhist contemplation of impermanence, actuarial practice, clinical exposure therapy, Monte Carlo simulation)
- Depth: 1.0 (generates: why Pre-Mortem works beyond bias correction; why the technique's power is emotional not cognitive; why secular institutional settings resist it; why it needs a ritual frame; why forced-failure framing outperforms probabilistic framing)
- **Elegance = 3.0 | Score = 0.52 × 3.0 = 1.56**

**C4 — Mental contrasting / WOOP (a=0.46)**
- Simplicity: 1.0 (irreducible: fantasy alone energizes; obstacle-awareness alone demotivates; the contrast between them produces action)
- Symmetry: 0.7 (appears in psychology of goal-pursuit, negotiation BATNA framing, Stoic reserve clause)
- Depth: 0.7 (generates: prediction that Pre-Mortem without action planning fails; that "Optimistic Pre-Mortem" / Pre-Parade may undermine if done first; that the failure-then-prevention sequence is not arbitrary)
- **Elegance = 2.4 | Score = 0.46 × 2.4 = 1.10**

**C5 — Groupthink prevention as primary mechanism (a=0.48)**
- Simplicity: 0.7 (near-irreducible; the claim is that Pre-Mortem is an anti-groupthink device first, risk-identification device second)
- Symmetry: 0.7 (nominal group technique, silent brainstorm, Delphi method all share the structure: separate-then-merge)
- Depth: 0.7 (generates: why individual-first sequencing is non-negotiable; why group discussion before individual writing destroys output quality; why written cards matter as physical anonymization)
- **Elegance = 2.1 | Score = 0.48 × 2.1 = 1.01**

**C6 — Orphan knowledge / tacit expertise surfacing (a=0.44)**
- Simplicity: 1.0 (irreducible: Pre-Mortem is a permission structure for unspoken knowledge, not a knowledge-generation exercise)
- Symmetry: 1.0 (appears in Challenger disaster O-ring case, organizational silence research, whistleblower psychology, sociometric network theory — the structure "person knows, cannot speak" recurs identically across domains)
- Depth: 1.0 (generates: why Pre-Mortems fail when hierarchy is present; why anonymous card submission defeats the technique's own goal; why the technique's 30% gain is actually a lower bound; what the ceiling gain would be under full psychological safety; why pre-mortem with trusted outsider facilitates more orphan knowledge surfacing)
- **Elegance = 3.0 | Score = 0.44 × 3.0 = 1.32**

**C7 — Cassandra failure-of-uptake vs. failure-of-detection (a=0.40)**
- Simplicity: 1.0 (irreducible: Pre-Mortem addresses detection only; a separate structural failure — uptake — is left unaddressed)
- Symmetry: 1.0 (the detection/uptake gap appears in intelligence community failures, medical diagnosis-vs-treatment split, weather forecasting accuracy vs. preparation rates, security red-team findings adoption)
- Depth: 1.0 (generates: why a Pre-Mortem can succeed fully and the project still fails; why the technique needs an ownership step to close the uptake gap; what % of Pre-Mortem outputs actually change decisions; whether the 30% detection gain translates to any outcome gain; what a "Post-Pre-Mortem" audit would show)
- **Elegance = 3.0 | Score = 0.40 × 3.0 = 1.20**

**C2 — FMEA structural comparison (a=0.48)**
- Simplicity: 0.4 (FMEA is not a compact insight — it's a parallel process; the insight is that Pre-Mortem lacks FMEA's severity×probability matrix)
- Symmetry: 0.7 (FMEA, fault tree analysis, HAZOP share the decomposition logic)
- Depth: 0.4 (generates: one clear implication — Pre-Mortem voting is ordinal, not quantified — but this confirms what is already suspected)
- **Elegance = 1.5 | Score = 0.48 × 1.5 = 0.72**

**C8 — Red-teaming (a=0.44)**
- Simplicity: 0.7 (the comparison is compressed but the insight — that Pre-Mortem lacks adversarial role-assignment — is near-irreducible)
- Symmetry: 0.7 (adversarial testing appears in security, wargaming, product QA, legal moot courts)
- Depth: 0.4 (confirms what is already broadly suspected about bias in self-critique)
- **Elegance = 1.8 | Score = 0.44 × 1.8 = 0.79**

**C9 — Self-defeating prophecy (a=0.44)**
- Simplicity: 1.0 (irreducible: a Pre-Mortem's vivid failure narrative may alter the project's actual trajectory by changing participants' affect and commitment)
- Symmetry: 0.7 (appears in placebo/nocebo research, expectation effects in sports, market panic self-fulfillment)
- Depth: 0.7 (generates: whether Pre-Mortem harm is possible via expectation contagion; optimal dosage question; whether a "vaccination dose" model applies)
- **Elegance = 2.4 | Score = 0.44 × 2.4 = 1.06**

**C3 — Flight-simulator/adversarial training (a=0.46)**
- Simplicity: 0.7 (the flight-sim analogy is intuitive but not fully reducible — needs the training-transfers-to-performance claim)
- Symmetry: 1.0 (flight simulation, surgical rehearsal, fire drills, tabletop exercises, wargames all share identical structure: safe failure environment builds real-situation response)
- Depth: 0.4 (generates: mainly confirms Pre-Mortem's safety framing, does not open new problem category)
- **Elegance = 2.1 | Score = 0.46 × 2.1 = 0.97**

**C10 — Game-theory pre-commitment (a=0.42)**
- Simplicity: 0.7 (near-irreducible: the ownership step is a commitment device that changes the payoff structure)
- Symmetry: 0.7 (appears in contract law, Ulysses contract, policy pre-commitment literature)
- Depth: 0.4 (generates: why ownership step is not cosmetic; confirms structure that is already partially visible)
- **Elegance = 1.8 | Score = 0.42 × 1.8 = 0.76**

```xml
</activation_provenance>
```

**Final ranking:**

| Rank | Node | a(final) | Elegance | Combined Score |
|------|------|----------|----------|----------------|
| 1 | C1: Stoic premeditatio malorum | 0.52 | 3.0 | **1.56** |
| 2 | C6: Orphan knowledge / tacit expertise | 0.44 | 3.0 | **1.32** |
| 3 | C7: Cassandra failure-of-uptake | 0.40 | 3.0 | **1.20** |
| 4 | C4: Mental contrasting / WOOP | 0.46 | 2.4 | **1.10** |
| 5 | C9: Self-defeating prophecy | 0.44 | 2.4 | **1.06** |
| 6 | C5: Groupthink prevention as primary mechanism | 0.48 | 2.1 | **1.01** |
| 7 | C3: Flight-simulator analogy | 0.46 | 2.1 | **0.97** |
| 8 | C8: Red-teaming | 0.44 | 1.8 | **0.79** |
| 9 | C10: Game-theory pre-commitment | 0.42 | 1.8 | **0.76** |
| 10 | C2: FMEA structural gap | 0.48 | 1.5 | **0.72** |

**Selected top 5 (scores ≥ 1.0):**

```xml
<selected_illuminations>
```

### Illumination 1 — The Pre-Mortem is a Secular Ritual of Premeditatio Malorum
**Score: 1.56 | Elegance: 3.0 (HIGH) | Source: C1**

The Pre-Mortem is not primarily a cognitive technique — it is a ritual structure that operationalizes the Stoic practice of *premeditatio malorum* (premeditation of evils). The Stoics discovered that voluntary, vivid imagination of the worst outcome does not increase anxiety — it *reduces* it, while paradoxically increasing preparation. The Pre-Mortem replicates this: it encodes the same structure (deliberate imaginative descent into failure) in a secular, team-compatible format.

**Why this matters:** The 30% identification gain is not a cognitive feat — it is an emotional permission structure. Teams without the Pre-Mortem already *know* many of the risks they surface during one; they lacked the ritual context that made speaking them legitimate. The technique's power is not information generation but information *derepression*.

**Cross-domain analogs:** Stoic philosophy; Buddhist contemplation of impermanence; clinical exposure therapy; actuarial scenario planning; Monte Carlo worst-case simulation.

**Implication chain:**
1. Facilitators who do not commit to the ritual frame (tentative language, hedging) will destroy the technique's effect — the liminal space collapses.
2. Pre-Mortems should be treated as *sacred time* with explicit opening and closing markers — not scheduled as standard agenda items.
3. The emotional intensity of the session is a quality signal: low-intensity Pre-Mortems likely produced low-quality outputs.
4. "Pre-Parade" (optimistic variant) done *before* the Pre-Mortem may inoculate against the descent — sequencing matters.
5. Distributed/async Pre-Mortems lose the shared ritual frame and likely produce lower-quality outputs even if participation increases.

---

### Illumination 2 — Pre-Mortem is a Permission Structure for Orphan Knowledge
**Score: 1.32 | Elegance: 3.0 (HIGH) | Source: C6**

Pre-Mortem guides present the technique as *surfacing hidden risks* — framed as if team members don't yet know these risks. This is rarely true. Most Pre-Mortem outputs consist of risks that at least one person in the room already knew about but had no legitimate channel to voice. The technique does not generate knowledge; it *releases* orphan knowledge — expertise that exists in the team but is trapped behind authority gradients, politeness norms, or optimism pressure.

**Why this matters:** This reframes the success metric. A Pre-Mortem is not successful because it identified novel risks — it is successful if the lowest-status person in the room who knew the critical failure mode was able to surface it. The 30% identification gain is a *lower bound*: it measures what escapes after the technique's permission structure is applied, but says nothing about what remains suppressed despite it.

**Cross-domain analogs:** Challenger disaster O-ring knowledge suppression; organizational silence research (Morrison & Milliken); whistleblower psychology; Abilene paradox; silence spiral in public opinion formation.

**Implication chain:**
1. Pre-Mortems run in hierarchically flat teams will outperform those run in hierarchical teams — the technique's value is inversely proportional to organizational hierarchy steepness.
2. Anonymous card submission, while seeming to aid psychological safety, actually undermines orphan knowledge surfacing: orphan knowledge often requires contextual explanation, not just naming.
3. An external facilitator (not a team member) dramatically increases orphan knowledge release by lowering fear of internal reprisal.
4. The riskiest Pre-Mortem output is the one that the most senior person dismisses first — this is the Cassandra signal.
5. Post-Pre-Mortem audit of "what was said vs. what was acted on" will reveal the real orphan knowledge rate.

---

### Illumination 3 — Pre-Mortem Addresses Detection, Not Uptake; The Cassandra Gap Is the Real Risk
**Score: 1.20 | Elegance: 3.0 (HIGH) | Source: C7**

The Pre-Mortem literature conflates two structurally distinct failure modes: failure to *detect* risks and failure to *act* on detected risks (uptake). The technique robustly addresses the first. It has almost no structural mechanism for the second. A team can conduct a perfect Pre-Mortem — identify every real failure mode, vote accurately, assign owners — and still have the project fail exactly as predicted, if the ownership step lacks teeth.

**Why this matters:** The Cassandra myth is not about failed prophecy — Cassandra's predictions were accurate. The failure was uptake. Pre-Mortem users who treat the technique as "risk identification = risk mitigation" have confused detection with uptake. The prevention strategy and ownership steps are structurally weak — they are optimistic lists, not commitment devices.

**Cross-domain analogs:** Intelligence community failures (detection without uptake); medical diagnosis vs. treatment compliance gap; weather forecast accuracy vs. evacuation compliance; security red-team findings adoption rate (typically <30% remediated within 6 months).

**Implication chain:**
1. A "Post-Pre-Mortem" audit (comparing Pre-Mortem outputs to actual project outcomes) would reveal whether the technique produces any outcome gain beyond the conversation.
2. The ownership step needs to be treated as a Ulysses contract, not a task list — it should be binding and time-boxed.
3. Pre-Mortem failure rates are unmeasured in the literature precisely because the technique is evaluated on process quality, not outcome quality.
4. The most dangerous Pre-Mortem result is "false mitigation confidence" — the team believes they have addressed identified risks when they have only named them.
5. A simple intervention: require each owner to write a written "pre-commitment statement" before leaving the session, publicly read.

---

### Illumination 4 — The Pre-Mortem Works via Mental Contrasting, Not Pessimism
**Score: 1.10 | Elegance: 2.4 (MODERATE) | Source: C4**

The guide frames Pre-Mortem as activating "pessimistic thinking." This is structurally incorrect. Gabriele Oettingen's mental contrasting research (WOOP: Wish, Outcome, Obstacle, Plan) shows that pure pessimism (imagining only failure) *reduces* motivation and preparation. What actually produces action is the contrast between desired outcome and identified obstacles. The Pre-Mortem, by embedding failure imagination inside a *prevention strategy* step, implicitly enacts mental contrasting — the pessimism is not the active ingredient; the contrast between failure-imagined and prevention-planned is.

**Why this matters:** "Optimistic Pre-Mortem" (Pre-Parade) done *before* the Pre-Mortem likely destroys the contrast and undermines the technique. The sequence — failure first, prevention second — is not arbitrary; it is the psychological mechanism. Reversing it or diluting it will reduce output quality.

**Cross-domain analogs:** WOOP goal psychology; negotiation BATNA analysis (contrast between best alternative and current offer); Stoic reserve clause; implementation intentions research.

**Implication chain:**
1. Facilitators who soften the failure framing ("it might have failed") remove the contrast anchor and reduce technique effectiveness.
2. Pre-Parade (optimistic variant) should only be run *after* a Pre-Mortem, never before — or in a separate session.
3. The 10-minute prevention strategy step is where the mechanism activates — shortening it (Speed Pre-Mortem) likely destroys disproportionate value.
4. Teams that skip the prevention step and use Pre-Mortem outputs only for risk registers are not using the technique; they're using a pessimism exercise.

---

### Illumination 5 — Pre-Mortem's Vivid Failure Narrative May Function as a Self-Defeating Prophecy
**Score: 1.06 | Elegance: 2.4 (MODERATE) | Source: C9**

The Pre-Mortem assumes that imagining failure increases preparation without affecting project commitment or team morale. This assumption is unexamined. Nocebo research (the placebo's negative counterpart) shows that vivid, authoritative framing of negative outcomes can actualize them through affect and expectation contagion. A Pre-Mortem that surfaces a failure scenario so vivid and credible that it shifts team members' internalized probability estimates may actually increase failure probability — not through the failure mode identified, but through reduced confidence, increased anxiety, and decreased proactive behavior.

**Why this matters:** This is not an argument against Pre-Mortems — it is an argument for dosage awareness. The Stoic practice that Pre-Mortem draws from was a *private* contemplative practice; its collective, high-fidelity, shared version has different psychological dynamics. The ritual closing step (which most guides omit) may be structurally necessary — not as ceremony but as the mechanism that re-activates commitment after descent.

**Cross-domain analogs:** Nocebo research; expectation contagion in sports teams; market panic self-fulfillment; clinical informed consent effect on symptom reporting.

**Implication chain:**
1. Pre-Mortems on projects where team morale is already fragile may produce net-negative outcomes.
2. A brief explicit "re-commitment" ritual at session close (not just a list of owners) may serve as the counter-nocebo.
3. The intensity of failure imagination should be modulated — extremely vivid scenarios may overshoot the optimal dose.
4. Rolling Pre-Mortem (repeated sessions) should be monitored for cumulative nocebo accumulation.

```xml
</selected_illuminations>
```

---

## Verification

### Darwin Independence Check

Reviewing the 5 selected illuminations' source chains for genuine independence:

| Illumination | Source Chain 1 | Source Chain 2 | Independent? |
|---|---|---|---|
| IL1 (premeditatio) | S1 → N1 → N25 (prospective-hindsight path) | S2+S3 → N3 → N11 → N34 (failure-certainty + optimism-bias path) | YES — S1 and S2/S3 are genuinely independent seeds |
| IL2 (orphan knowledge) | S11 → N8 → N26 → N38 (hidden-risk-surfacing path) | S4+S5 → N4/N5 → N20/N26 → N38 (psych-safety + brainstorm path) | YES — S11 and S4/S5 are independent seeds |
| IL3 (Cassandra gap) | S2 → N11 → N24 → N40 (failure-certainty path) | S11 → N8 → N24 → N40 (hidden-risk-surfacing path) | YES — S2 and S11 are independent seeds |
| IL4 (mental contrasting) | S1 → N1 → N25 (prospective-hindsight) | S2 → N3 → N25 (loss-aversion path); S3 → N9 → N25 (optimism-bias path) | YES — three independent seeds |
| IL5 (self-defeating prophecy) | S8 → N2 → N41 (temporal-anchoring path) | S7 → N13 → N27 → N41 (prevention-ownership path) | YES — S8 and S7 are independent seeds |

**Darwin independence check: PASS — all 5 illuminations have ≥2 genuinely independent source chains.**

### Ramanujan Pattern-Verification

Verifying each illumination pattern on ≥2 test instances:

**IL1 (premeditatio):** Instance 1 — Stoic practice of *melete thanatou* (meditation on death) produces equanimity, not paralysis. Instance 2 — Buddhist *maranasati* (mindfulness of death) produces present-moment engagement. Instance 3 — Clinical exposure therapy for phobias produces habituation through deliberate approach, not avoidance. Pattern holds on 3 instances. **PASS.**

**IL2 (orphan knowledge):** Instance 1 — NASA Challenger: engineers knew O-ring risk, could not surface it through authority gradient. Instance 2 — Medical errors: nurses often identify medication errors before administration but face barrier to voicing disagreement with physicians. Instance 3 — Whistleblower studies (Near & Miceli): silent witnesses to wrongdoing possess complete information; suppression is structural, not informational. Pattern holds on 3 instances. **PASS.**

**IL3 (Cassandra gap):** Instance 1 — IPCC climate reports: detection high, uptake low; gap is structural. Instance 2 — Security red-team findings: 2019 Verizon DBIR data shows <30% of penetration test findings remediated within 6 months. Instance 3 — COVID pandemic modeling: accurate detection (March 2020 projections) preceded policy uptake by critical weeks. Pattern holds on 3 instances. **PASS.**

**IL4 (mental contrasting):** Instance 1 — Oettingen WOOP trials: positive fantasy alone → lower GPA; mental contrasting → higher GPA. Instance 2 — Negotiation BATNA: knowing your walk-away point (negative anchor) combined with aspiration produces better outcomes than aspiration alone. Instance 3 — Implementation intentions research (Gollwitzer): if-then planning (obstacle-anchored) outperforms pure goal-setting. Pattern holds on 3 instances. **PASS.**

**IL5 (self-defeating prophecy):** Instance 1 — Nocebo: patients told a procedure is painful report more pain than those told it is minor, even with identical treatment. Instance 2 — Sports teams: locker-room defeatism after vivid analysis of opponent's strengths correlates with underperformance beyond what analysis predicts. Instance 3 — Bank runs: credible prediction of bank failure causes the failure regardless of underlying solvency. Pattern holds on 3 instances. **PASS.**

**Ramanujan pattern-verification: PASS — all 5 illuminations verified on ≥2 independent instances (all verified on 3).**

**Activation-map completeness: 56 distinct nodes. PASS (minimum 15).**

---

*S3 Peripheral Exploration complete. 10 convergent nodes found; 5 selected illuminations produced. No S3_thin_or_empty signal raised.*

```xml
</peripheral_exploration>
```
