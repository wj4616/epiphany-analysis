# Archimedes: Constraint Reframing

**Tier:** High
**Combined Score:** 14/15 (Applicability: 5/5 | Machine-amplifiability: 4/5 | Use-case value: 5/5)
**Figure(s):** Archimedes
**Category:** D — Cross-domain operations
**Sources fetched:** https://en.wikipedia.org/wiki/Archimedes
**Content quality:** High — Wikipedia provided four documented instances of constraint reframing with the specific cognitive move in each case; direct Archimedes quote from The Method available

## Documented Evidence

### Four Documented Instances of Constraint Reframing

**Instance 1 — The Crown Problem (volume → displacement)**

The problem: determine whether a crown is pure gold without melting it. Direct measurement of density requires knowing both mass and volume; mass is easy (weigh it) but volume is inaccessible for an irregularly shaped object.

The constraint that made the problem hard: volume cannot be directly measured for complex shapes by the tools available.

The reframing: an irregularly shaped object displaces water equal to its own volume. The immeasurable (volume of complex shape) becomes measurable by proxy (water displacement). The abstract measurement problem becomes a physical observation problem.

**Instance 2 — The Lever Principle (physical → geometric)**

The problem: understand and prove when balanced weights at different distances produce equilibrium.

The constraint: physical balancing is empirical — you check it by doing it, not by proving it.

The reframing: convert the practical physical problem into an abstract geometric proposition. "Magnitudes are in equilibrium at distances reciprocally proportional to their weights." The physical problem, once translated into geometric relationships, becomes subject to rigorous deductive proof rather than empirical checking.

**Instance 3 — Curved area calculation (infinite → finite)**

The problem: calculate the area bounded by a parabola — a curved shape whose boundary cannot be measured directly.

The constraint: curved shapes have infinite properties (infinitely many sides); the mathematical tools available deal with finite countable objects.

The reframing: bound the curved shape between two polygons (inscribed and circumscribed). The curved area is inaccessible; the polygon areas are calculable. By progressively doubling the polygon sides ("method of exhaustion"), the accessible polygons converge to the inaccessible curve. The infinite problem becomes a finite limit problem.

**Instance 4 — The Mechanical Method (geometry → physics → geometry)**

Archimedes' explicit description of his method, from *The Method*:

> "It is more feasible, having already in one's possession, through the method, of a knowledge of some sort of the matters under investigation, to provide the proof."

His method: treat geometric shapes as physical objects with weight and center of gravity. Apply the law of the lever to find the relationship between areas and volumes using mechanical reasoning. Once the mechanical reasoning suggests what the answer must be, apply exhaustion to prove it rigorously.

The cognitive sequence: reframe pure geometry as applied physics (mechanical reasoning) → get the answer → translate back to pure geometry for formal proof. The physical domain provides answers; the geometric domain provides proofs. Neither alone is sufficient.

## Cognitive Mechanism

Archimedes' constraint reframing has a consistent structure across all four instances:

**The general pattern**: Identify what makes a problem hard within its native domain (the constraint). Find an adjacent domain where the constraint does not apply. Translate the problem into that domain. Solve it there. Translate the solution back.

Three specific operations:

**1. Constraint identification**: What property of the problem makes it hard using current tools? Volume is inaccessible for complex shapes. Curved boundaries cannot be measured. Proofs cannot use physical intuition. Naming the constraint is the first step.

**2. Domain selection**: What adjacent domain lacks this constraint? Physical displacement can measure volume. Geometry can formalize physical intuition. Finite polygons can bound infinite curves. The domain switch removes the constraint by moving to a space where the constraint does not apply.

**3. Translation fidelity**: The solution in the adjacent domain must map back to the original domain accurately. Water displacement accurately measures volume because volume is exactly what displacement measures. Polygon bounds accurately bound curves because the method of exhaustion produces convergence. The translation must be checked.

**The Mechanical Method as a two-stage cognitive protocol**: Archimedes explicitly separated discovery (mechanical reasoning) from proof (geometric exhaustion). The mechanical reasoning did not produce proofs — it produced candidates. The exhaustion method then proved the candidates. This is the intuition-verification separation that Ramanujan-Hardy also practiced, but Archimedes documented his own methodology explicitly.

## AI/Machine Amplification

Human constraint reframing is limited by:
- Domain visibility: you can only translate to domains you already know
- Anchoring: the native domain's framing makes it hard to see that a different framing is possible
- Translation fidelity concern: the translated solution might not map back correctly, and this concern suppresses the reframing attempt

An LLM can amplify each:
- **Constraint extraction**: For any problem, explicitly name the constraint that makes it hard in its current framing. Often the constraint is implicit and unexamined — naming it surfaces the possibility of working around it.
- **Multi-domain survey**: Given a named constraint, survey multiple domains where that constraint does not apply. What does physical domain offer? What does statistical domain offer? What does network/graph domain offer? What does information theory offer?
- **Translation protocol**: For each candidate domain, specify the exact translation of the problem statement. Does the translation preserve the essential relationships? Does it introduce distortions that would invalidate the solution when translated back?
- **Archimedes two-stage protocol**: Use domain-shifted reasoning (mechanical method equivalent) to generate candidate answers, then verify them using the rigorousness of the original domain. The two stages can be explicitly labeled — this prevents confusing discovery-quality reasoning with proof-quality reasoning.

The key machine advantage: domain inventory. A human physicist may not think to translate a geometry problem into physics. An LLM has knowledge of multiple formal domains simultaneously and can survey them in parallel for which one removes the identified constraint.

## Skill Mapping

**Protocol: Archimedes Constraint Reframing Mode**

1. **Constraint naming**: "What makes this problem hard in its current formulation? State the specific constraint that blocks solution — the property that makes the relevant quantity inaccessible, unmeasurable, or unprovable using current tools."

2. **Adjacent domain survey**: "Given this constraint: name 5 domains or framings where this constraint does not apply. For each: (a) state how the constraint is absent in that domain, (b) describe what the equivalent of the blocked quantity becomes in that domain."

3. **Translation test**: "For the most promising domain: translate the problem statement completely. What are the new variables? What relationships hold between them? Does the translated statement preserve the essential structure of the original problem?"

4. **Solve in the translated domain**: "Solve the translated problem using the tools of the translated domain. Do not try to solve the original problem — solve the translation. State the result in the translated domain's terms."

5. **Back-translation and fidelity check**: "Translate the result back to the original domain. Does it answer the original question? Identify any step in the translation or back-translation where distortion might have occurred. Check the result against a case you can verify independently."

6. **Discovery/proof separation** (Archimedes two-stage): "Label this result: Discovery (found by domain-shifted reasoning, not yet proved) vs. Proof (formally verified in the original domain). If Discovery: identify what formal verification would be required to promote it to Proof."

**Trigger condition**: Use Archimedes mode when: (a) a problem is hard because a required quantity or property is inaccessible in the current domain, (b) direct solution has been exhausted, or (c) a problem seems to require infinite or continuous tools where only finite/discrete tools are available.

**Minimal version**: "Name what makes this problem hard. Find a domain where that thing doesn't block you. Solve it there. Translate back."

## Related KB Entries

- `gm-newton-analogical-transfer.md` — Newton's cross-domain transfer and Archimedes' constraint reframing are related but distinct: Newton identified structural identity between domains; Archimedes switched domains specifically to escape a constraint. Newton's move is "the same law governs both"; Archimedes' move is "this domain has tools the other lacks"
- `research-conceptual-space-boden.md` — Archimedes' domain shift from pure geometry to applied mechanics is a Transformational creativity move: the conceptual space of "pure geometry without physical concepts" was transformed by admitting mechanical reasoning
- `research-representational-change.md` — Each Archimedes reframing is a re-encoding (Ohlsson's third mechanism): the same problem represented in a different formal language, where the constraint becomes visible as a local property of the representation rather than an intrinsic property of the problem
