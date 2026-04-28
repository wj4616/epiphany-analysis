# S7 — Integration & Verification
stage_id: S7
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0
status: complete

---

<integration>

## Convergence signals

Stages that ran: S1, S2, S3, S4, S5, S6 (all six prior core and optional stages present).

### Signal collection — key claim per stage

| Stage | Key claim or finding |
|-------|----------------------|
| S1 | The load-bearing architecture thesis: 1-spawn inline orchestration achieves speed > epiphany-prompt, quality ≥ prompt-epiphany baseline, and deterministic execution — with three explicitly accepted trade-offs: context contamination, self-review bias, no repair loops. |
| S2 | The binding constraint (quality–isolation coupling) is escapable by reframing context contamination as an addressability problem: Role-as-Channel-Code architecture — role declarations as structural output markers — simultaneously activate behavior and create demultiplexable channels, allowing synthesis spawn prompt to filter by structural address rather than requiring source-context purity. |
| S3 | Five illuminations; highest-ranked: (1) Representational Interference — the most vulnerable pipeline boundary is between Step 4 ideation and Step 6 synthesis, because by Step 4 the context has two accumulated role-contaminations and the synthesis agent's self-verification runs in the most contaminated cognitive state. (2) Approximation Architecture — prompt-cog's three deliberate quality downgrades form a coherent polynomial-time approximation scheme relative to epiphany-prompt's optimal architecture, with identifiable boundary conditions for degradation. |
| S4 | Three theory collisions confirmed: (a) speed C1 vs. quality floor C2 — resolvable only statistically, not per-invocation; (b) INVENTORY verbatim mandate vs. synthesis quality — competing objectives with no tiebreaker; (c) anti-conformity quality gain from spawned agent vs. in-context re-read — claimed +32.9% novelty may not transfer to the inline contaminated context. Three singularities: Step 5 total-halt with no output; empty INVENTORY makes preservation vacuous; spawn-context-overflow produces indistinguishable failure from Step 5 halt. |
| S5 | The architecture claim reduces to three jointly necessary conditions: spawn_count = 1, latency(prompt-cog) < latency(epiphany-prompt), quality(prompt-cog) ≥ quality(prompt-epiphany). "Quality" is flagged as formally undefined without a measurement rubric. "Faster" requires controlled comparison conditions. "Reliable deterministic execution" must be scoped to routing logic, not universal LLM execution. |
| S6 | Five consolidated hypotheses ranked by confidence: H1 (0.91) — spec systematically overconfident on quality claims for non-mechanical checks; H2 (0.88) — quality claims bounded by input complexity, absence of complexity detection is a specification gap; H3 (0.85) — quality floor is an expected-value claim, not per-invocation guarantee; H4 (0.82) — Interface 2 coherence gap: pre-spawn checklist validates presence, not coherence; H5 (0.78) — INVENTORY verbatim hard constraint has soft enforcement only. |

### Convergence signals (independent appearance in ≥2 stages)

**CS-1: Quality floor is statistical, not per-invocation absolute**
Appears in: S5 (flagged vague claim: "quality" undefined, measurement conditions unspecified), S4 (theory collision C1 vs. C2: "neither can be softened without changing the architecture"), S6 (H3, confidence 0.85: "quality floor as expected-value claim"; D-C3 formally contradicts P5 + P10 + P11 as conjoint).
Independent activation: S5 reached it through precision-stripping the formal claim; S4 reached it through dynamic theory-collision analysis; S6 reached it through deductive contradiction of premises.
**Strength: STRONG (3 independent chains, all pointing to the same scope qualification needed).**

**CS-2: Context contamination is architecturally encoded at the spawn boundary, not just a behavioral risk**
Appears in: S2 (binding constraint: contamination is the quality–isolation coupling; escape path via channel-code marking), S3 (Illumination 1: representational interference is most acute at Step 4→6 boundary; synthesis self-verification runs in the most contaminated cognitive state), S4 (Frame 1 insider: spawn prompt body carries contamination from Steps 3–4 as encoded content, not just as behavioral priming), S6 (D-C2: spawn prompt integrity is affected by contamination, not just synthesis output quality — partial spec contradiction).
**Strength: STRONG (4 independent chains, escalating from behavioral risk to structural encoding).**

**CS-3: The substitution of protocol compliance for agent isolation preserves quality only for mechanical checks**
Appears in: S3 (Illumination 2: three quality downgrades form a coherent approximation scheme with identifiable failure boundary conditions), S6 (H1, confidence 0.91: inductive generalization G1/G2 boundary failures; H-MERGED-2; Millikan assessment asymmetry), S4 (theory collision C3: anti-conformity in-context vs. spawned agent — novelty transfer may be zero in contaminated context).
**Strength: STRONG (3 independent chains reaching the same mechanical/non-mechanical boundary).**

**CS-4: The spawn prompt construction is the highest-leverage quality determinant in the architecture**
Appears in: S2 (Mapping 1 multiplexing analogy: receiver-side filter design is the quality-critical engineering point; "synthesis spawn prompt is the demultiplexer"); S3 (Illumination 5, Three-Interface Architecture: Interface 2 is the reliability weak point); S6 (H4: pre-spawn checklist validates presence but not coherence; strongest objection: "well-specified form does not guarantee well-specified content").
**Strength: STRONG (3 independent chains all pointing to the spawn prompt construction as the single highest-leverage quality gate).**

**CS-5: Speed advantage is real, robust, and super-linear in model latency**
Appears in: S4 (Frame E: speed gain amplified under high-latency conditions; E.1.1: "speed gain is super-linear in model latency"), S5 (formal latency equation: ΔT = (N_spawns_ep - 1) × T_cold_start + Σ T_spawn(S_analysis, S_ideation); speed holds if T_cold_start > T_inline(S3,S4)), S6 (Extreme Scenario Regime 2: "Speed still holds" even in the complex-input failure regime; H2 confirms speed claim survives even where quality claim degrades).
**Strength: STRONG (3 independent chains, no divergence — speed claim is the most robustly confirmed sub-claim in the architecture).**

### Divergence points

**DP-1: Whether the quality floor holds in practice**
S6 H1/H2/H3 (confidence 0.91/0.88/0.85): floor is bounded, statistical, and not per-invocation. S6 Extreme Scenario Regime 1: "architecture is optimal for simple inputs; all three requirements satisfied." The divergence is resolved by scope: the floor holds for moderate-complexity inputs and fails for complex inputs with compounding degradation vectors. This is a scope limitation, not a contradiction.

**DP-2: Whether the anti-conformity in-context pass provides meaningful novelty gain**
S3 (ported from DEEP M12 under Illumination 3 and Illumination 2): quality benefit retained in approximation sense, though ceiling is lower. S4 (theory collision C3): novelty gain may be zero in contaminated context. S6 (H-MERGED-2): spec overconfident on non-mechanical checks. Discriminating condition is empirical (measure novelty differential between primary and anti-conformity contracts). Unresolved divergence: genuine open question. **Feeds to V6 as a scope limitation and to Open Questions.**

---

<primary_conclusion>

## Primary conclusion

**Headline:** The `prompt-cog` 1-spawn inline orchestration architecture is a well-formed, internally consistent approximation scheme that achieves genuine speed superiority over `epiphany-prompt` (robustly confirmed: CS-5) and maintains a quality floor comparable to `prompt-epiphany` for moderate-complexity inputs — but the quality floor is an expected-value claim bounded by input complexity, not a per-invocation guarantee (CS-1). The architecture's primary structural risk is that context contamination is not merely a behavioral execution risk but is encoded into the spawn prompt body itself (CS-2), making spawn prompt construction the single highest-leverage quality gate in the system (CS-4). The substitution of protocol compliance for agent isolation is architecturally sound for mechanical quality checks (verbatim match, format parse, schema presence) but produces systematic quality degradation for judgment-dependent checks (contrarian ideation, cross-constraint synthesis, semantic enhancement quality), a pattern the spec acknowledges individually in design decisions but does not synthesize into a design-level principle (CS-3).

The primary conclusion is therefore this compound claim:

> The `prompt-cog` architecture is a well-specified polynomial-time approximation of `epiphany-prompt`'s optimal architecture. Speed superiority is robust and super-linear in latency. Quality floor holds for moderate-complexity inputs. The architecture's quality boundary is determined by the mechanical/non-mechanical distinction in its enforcement mechanisms: mechanical checks (INVENTORY verbatim, return format, pre-spawn presence) are reliable within v1 constraints; judgment-dependent checks (anti-conformity novelty, synthesis semantic accuracy, coherence of contracts-to-weaknesses) are degraded by context contamination and self-review bias in ways the spec documents individually but does not yet unify into a complexity-detection threshold or design-level escalation trigger. The highest-leverage v1 improvement opportunity is the spawn prompt construction quality: specifically, Interface 2 coherence (contracts-to-weakness alignment), because it is the one quality gap that affects mechanical and judgment-dependent checks equally and is not addressed by any existing mechanism in v1.

### Supporting evidence chain

1. (S1) Architecture primitives establish the 1-spawn design as an explicit set of constraints and trade-offs.
2. (S2) Constraint-escape analysis confirms the coupling between quality isolation and spawn cost is escapable via structural channel-coding of context; the escape path is valid and operationally actionable.
3. (S5) Precision-forcing establishes the three jointly necessary conditions for the architecture claim to hold; flags "quality" as undefined without a measurement rubric.
4. (S6-H1, S6-H2, S6-H3) Falsification identifies the scope boundary: quality floor is statistical, bounded by input complexity, and the spec contains no complexity-detection trigger.
5. (S3-Illumination 2, S6-H1, S4-Collision 3) Three independent chains confirm the mechanical/non-mechanical quality boundary as the unifying design-level principle.
6. (CS-4, S6-H4) Spawn prompt construction is the single unresolved highest-leverage quality gate in v1.

<creativity_type>

## Creativity type

**Boden type: Exploratory**

The `prompt-cog` architecture operates within the established conceptual space of prompt-enhancement skill design — the space defined by the rules: (a) orchestration via agent spawns or inline execution, (b) quality through isolation or approximation, (c) determinism via prefix-based parsing. No foundational constraint of this space is negated. The insight is that a previously unexplored region of the possibility space exists: the 1-spawn inline orchestration point, where analysis and ideation are inlined but synthesis is isolated, and where structural channel-coding substitutes for spawn-based isolation. This region was theoretically reachable but not visited by either source skill (`prompt-epiphany` uses loose single-pass; `epiphany-prompt` uses full-spawn isolation).

The primary conclusion is itself exploratory: it identifies where in the possibility space the architecture's quality guarantees break down (the mechanical/non-mechanical boundary), which was not previously mapped for this class of prompt-enhancement architectures.

**Distinguishing test passed:** "Was this conclusion strictly impossible before, or just unexplored?" Answer: unexplored. The 1-spawn architecture and the mechanical/non-mechanical quality boundary were derivable from existing principles; they were not previously articulated. → Exploratory, not Transformational.

**Elegance scoring (applied to primary conclusion):**

- **Simplicity:** 0.7 — The claim is expressible compactly (polynomial-time approximation with mechanical/non-mechanical quality boundary) but requires the compound form above to be complete. One element (Interface 2 coherence as highest-leverage gap) could in principle be deferred to a corollary without losing the core claim. Near-irreducible.
- **Symmetry:** 0.7 — The mechanical/non-mechanical distinction recurs independently in: S6 inductive generalizations (G1/G2 boundary failures), S3 approximation architecture (three quality downgrades as coherent scheme), S4 theory collisions (C2 and C3 both resolve on the same axis). Dual-domain recurrence confirmed within two independent analytical chains; not yet confirmed in an unrelated domain.
- **Depth:** 1.0 — Generates ≥5 independently verifiable implications: (1) complexity-detection trigger as v2 minimal gap-filler; (2) anti-conformity in-context novelty gap is measurable empirically; (3) Interface 2 coherence check as the single v1 improvement not requiring a second spawn; (4) KB snippet staleness as an asymptotic quality erosion risk; (5) spawn prompt construction quality (structural vs. content adequacy) as a separable quality dimension not addressed by the pre-spawn checklist. Each implication opens a new problem category.

**Elegance score: 0.7 + 0.7 + 1.0 = 2.4 (MODERATE)**

Cross-domain recurrence (Symmetry 0.7) provides meaningful structural coherence support. Symmetry < 0.8 — no special Transformational scope narrowing triggered (Exploratory type with moderate symmetry: conservative V6 narrowing applied).

</creativity_type>

</primary_conclusion>

## Confidence band

**MODERATE**

Reasoning:
- CS-1 through CS-5 are all strong (3–4 independent chains each). This would support HIGH.
- However: two conditions prevent HIGH:
  1. DP-2 (anti-conformity novelty transfer) is an unresolved divergence. S3, S4, and S6 point to different projected outcomes; the discriminating condition is empirical and not yet tested.
  2. V1 Logic check (Step 6 below) will identify the quality floor claim as requiring an additional assumption (expected-value interpretation) not fully stated in the primary input. This is a partial inference gap.
- No major single-V1 failure; no catastrophic unresolved divergences. MODERATE is the correct band.

**Summary:** Convergence on the speed claim (HIGH confidence), convergence on the mechanical/non-mechanical quality boundary (HIGH confidence), MODERATE on overall architecture claim because the quality floor scope and anti-conformity novelty remain empirically open.

## Generalization check

**Generalizes to:** Agentic orchestration architectures where (a) a shared context window is used for multiple sequential role-switched processing steps feeding a single isolated downstream agent, and (b) the downstream agent's quality depends on both mechanical compliance checks and judgment-dependent synthesis operations. The mechanical/non-mechanical quality boundary finding generalizes to any such architecture: mechanical checks are reliable under inline execution + self-verification; judgment-dependent checks degrade predictably with context contamination depth. This applies beyond prompt-enhancement to classification agents, planning agents, and code-generation agents using similar inline-then-isolated-spawn patterns.

**One-off aspects (applies only to this specific instance):** The specific quality floor threshold (vs. `prompt-epiphany` baseline) and the specific speed comparison (vs. `epiphany-prompt`) are instance-specific, depending on the source skills' characteristics and not transferable as absolute claims to other architectures.

## Verification report (V1-V7)

### V1 — Logic

**Inferential chain: S1 primitives → primary conclusion**

Step V1.1: From S1 irreducibles A1 (1-spawn model), C1 (must run faster), C4 (spawn budget = 1) → speed advantage derives from eliminating spawn overhead for Steps 3–4.
Inference form: modus ponens (if fewer spawns and cold starts, then lower latency, given T_cold_start > T_inline(S3,S4)).
Additional assumption required: T_cold_start + T_spawn(S3,S4) > T_inline(S3,S4). This is expected to hold for non-trivially short inputs but has not been formally proven; S5 establishes it as the testable speed condition.
**Step valid with stated assumption — PARTIAL at this link.**

Step V1.2: From C2 (quality floor), DD#9 (self-review bias acknowledged), DD#8 (contamination accepted) → quality floor cannot be an absolute per-invocation guarantee.
Inference form: reductio (P5 as absolute + P10 + P11 jointly contradictory; D-C3 in S6 formally demonstrates this).
**Step valid — no additional assumption required.**

Step V1.3: From S2 constraint-escape path (Role-as-Channel-Code) → contamination is an addressability problem not a purity problem, solvable by structural channel-coding.
Inference form: analogical reasoning from 5 domain mappings (multiplexing, differentiation, spatial filtering, catalysis, time-slicing), consolidated into multi-source-convergent blend.
**Step valid as analogical inference (not deductive proof). Structural mapping is detailed and verified for systematicity. No CC-ANALOGY cargo cult — the analogy identifies a concrete design intervention (role declarations as output markers + spawn prompt as demultiplexer), not just a metaphorical framing.**

Step V1.4: From CS-3 (substitution preserves quality for mechanical checks only) → the mechanical/non-mechanical boundary is the unifying design-level principle.
Inference form: inductive generalization from S6 G1/G2 boundary failures, S3 Illumination 2, S4 Collision 3 — all independently reaching the same boundary.
**Step valid via inductive convergence (3 independent chains).**

Step V1.5: From CS-4 (spawn prompt construction as highest-leverage gate) + H4 (Interface 2 coherence gap is structurally unresolvable within v1 spawn budget) → Interface 2 coherence is the primary v1 improvement opportunity.
Inference form: elimination (other quality gaps either have existing mitigation or require additional spawns; Interface 2 coherence requires neither a second spawn nor a new mechanism — only a coherence-checking addition to the pre-spawn checklist).
**Step valid — elimination is sound given enumeration from S1 primitives and S6 H4.**

**V1 Logic: PARTIAL — one weak link at V1.1 (speed condition requires empirical assumption about T_cold_start vs. T_inline(S3,S4) that is plausible but unverified). All other inferential links valid or validly analogical with explicit structural mapping.**

---

### V2 — Cargo-Cult Detection

Scanning primary conclusion and supporting evidence chain:

- **[CC-PRECISION] scan:** "+32.9% novelty gains" cited from epiphany-prompt spec for anti-conformity pass. This is a precision claim from a source document; it is not presented as verified evidence but as a transfer claim whose applicability is explicitly questioned in S4 Collision 3 and S6 H-MERGED-2. The primary conclusion does not rely on this number — it notes the transfer may be zero. No CC-PRECISION violation in the conclusion.
- **[CC-STAT] scan:** No anonymous statistics. Speed comparison references "≥3 spawns in epiphany-prompt" — this is derivable from the epiphany-prompt architecture description, not an anonymous statistic.
- **[CC-AUTH] scan:** No authority appeals. Design decisions cited are directly from the input specification.
- **[CC-ANALOGY] scan:** S2 uses 5 domain analogies. These are used as structural evidence for the constraint-escape path, not as proof. Each analogy was verified for systematicity (no two share surface features). The analogical reasoning informs but does not constitute the primary conclusion — the conclusion is stated as a structural claim. No CC-ANALOGY violation.
- **[CC-JARGON] scan:** "Polynomial-time approximation scheme" — this phrase has a concrete referent (spawn count = 1, latency bounded by cold-start elimination, quality approximation ratio measurable). Not jargon-without-referent.
- **[CC-CAUSAL] scan:** "Context contamination is encoded into spawn prompt body" — stated as a structural consequence of Steps 3–4 running inline and Step 5 assembling spawn prompt from that context. Mechanism is explicit (D-C2 in S6). Not correlation-as-causation.

**V2 Cargo-Cult: PASS — none detected.**

---

### V3 — Symmetric Scrutiny

Primary conclusion challenges applied:
- Quality floor bounded to moderate complexity (CS-1, DP-1) — challenged and confirmed as limitation in V6 scope.
- Anti-conformity novelty transfer (DP-2) — left as open question, not asserted.
- Speed superiority requires empirical comparison conditions (S5 flagged vagueness) — noted as partial in V1.
- Self-verification bias means hard INVENTORY constraint has soft enforcement (H5) — acknowledged in primary conclusion.

Rival hypotheses challenges applied (from S6):
- H1 (spec overconfident): subjected to Millikan effort asymmetry analysis — effort to refute H1 is LOW (spec provides refutation evidence itself). The primary conclusion does not dismiss H1 — it incorporates it as the mechanical/non-mechanical boundary principle. The rival is absorbed, not dismissed.
- H2 (bounded approximation scope): directly incorporated into primary conclusion scope and V6 statement.
- H3 (quality floor as expected-value): incorporated as a scope qualification of P5.
- H4 (Interface 2 coherence gap): promoted to the highest-leverage improvement finding.
- H5 (INVENTORY enforcement gap): noted as a structural gap; the primary conclusion accepts this risk as empirically open.

The primary conclusion faces at least as many objections as each rival hypothesis, and rivals are not dismissed — they are integrated or incorporated as scope limitations.

**V3 Symmetric Scrutiny: PASS — equal burden of proof applied; rivals absorbed into primary conclusion rather than rejected.**

---

### V4 — Completeness

| Stage | Cited in conclusion | Notes |
|-------|--------------------|----|
| S1 | Yes — primitives A1, C1, C2, C4, C6, DD#8, DD#9 are the load-bearing premises for the primary conclusion | |
| S2 | Yes — constraint-escape path (Role-as-Channel-Code) cited as structural evidence for contamination-as-addressability reframe | |
| S3 | Yes — Illumination 1 (representational interference), Illumination 2 (approximation architecture) directly cited in CS-2 and CS-3 | |
| S4 | Yes — theory collisions C1/C2/C3 cited; frames contribute to "PASS label reliability" finding and speed super-linearity | |
| S5 | Yes — precision-forcing establishes the three-condition formal structure and flags the quality measurement gap; V1 partial link traces to S5 | |
| S6 | Yes — consolidated hypotheses H1–H5 are the primary falsification source; all five incorporated into primary conclusion | |

**V4 Completeness: PASS — all six stages are substantively cited.**

---

### V5 — Constructive

**Constructive spec (post-minimization from S5):**

Implement a skill that executes Steps 0–5 and 7 inline without spawning, spawns exactly one synthesis agent for Step 6, receives synthesis return, and produces output XML where every INVENTORY item string appears verbatim as a substring, within wall-clock time less than an equivalent `epiphany-prompt` run on the same representative input.

**Minimality check:** All four conditions necessary; removing any one leaves the demonstration incomplete (see S5 constructive specification section). The Interface 2 coherence improvement can be demonstrated by a variant that includes a coherence check at Step 5 (verifying that at least one contract per high-impact weakness exists) — this is demonstrable without modifying the spawn architecture.

**Is this demonstration possible in principle?** Yes. The skill is a SKILL.md text file; its execution is observable; INVENTORY verbatim check is a string search; wall-clock time is measurable. No logical impossibility.

**V5 Constructive: PASS — constructive spec: implement the 1-spawn SKILL.md per the spec and compare execution trace, spawn count, INVENTORY verbatim presence, and wall-clock time against an epiphany-prompt run on a representative 200–2000 word technical prompt.**

---

### V6 — Scope

*Boden type: Exploratory — conservative narrowing applied. V7 detected frame shift (declarative → procedural); shift reflected in "Does not extend to" item 4.*

File written to: `stages/S7-v6-scope.txt`

Contents:
```
**Applies to:** prompt enhancement skill architectures that use inline orchestration (analysis and ideation in a single shared context window) with exactly one isolated synthesis agent spawn, operating on moderate-complexity prompt inputs (INVENTORY ≤ ~12 items, no deeply interdependent multi-constraint synthesis tasks), where speed relative to a multi-spawn baseline is the primary architectural goal and the quality floor is defined as meeting or exceeding a single-pass non-modular baseline on those inputs
**Does not extend to:** (1) complex technical prompt inputs with large INVENTORY density (15+ items) or deeply interdependent constraints, where spawn prompt truncation, context contamination, and self-review bias compound to breach the quality floor; (2) agentic or executable workflow prompt inputs where the distinction between "enhance" and "execute" is ambiguous, rendering Hard Gate 3 non-mechanically enforceable; (3) quality claims interpreted as per-invocation guarantees across all input classes — the architecture holds its quality floor in the expected case for moderate-complexity inputs, not universally; (4) contexts where the conclusion is stated in a procedural frame — the conclusion was derived in a declarative/structural frame and requires translation before application as implementation instructions
**Claims refused:** (none)
```

**V6 Scope: PASS — wrote stages/S7-v6-scope.txt**

---

### V7 — Representational Coherence

**Input frame (from 00-processed-input.md):**
The input is a design specification document. It is expressed primarily in **declarative** frame: statements about what is true — what the components do, what constraints hold, what trade-offs were made, what enhancements port vs. defer. It contains procedural elements (step-by-step architecture description, I/O contracts per step) but the dominant frame is declarative: "X is Y," "X must do Z," "A is accepted; B is deferred."

**Conclusion frame:**
The primary conclusion is also expressed in **declarative** frame: claims about what is true of the architecture ("the quality floor is an expected-value claim," "spawn prompt construction is the highest-leverage gate," "protocol compliance substitutes for agent isolation only for mechanical checks"). The verification conclusions (V1–V5 above) are procedural in form but serve declarative assertions.

**Frame comparison:** Same dominant frame (declarative) for both input and conclusion. No representational shift detected.

**V7 Representational: No shift — conclusion is in the same frame as input (declarative). V6 scope unchanged by V7.**

---

## Open questions & next probes

**Probe 1:** Measure the novelty differential between primary contracts and anti-conformity second-pass contracts in normal-mode `prompt-cog` runs, compared to DEEP M12 spawned contrarian contracts in `epiphany-prompt` runs, on 10 diverse inputs.
**Why it matters:** Resolves DP-2 (the single unresolved divergence). If the novelty differential is consistently near-zero in-context, the "+32.9% novelty gains" claim is invalidated for the inline implementation, and the anti-conformity pass cost (time for second re-read) buys no measurable quality gain — making it a candidate for removal or de-emphasis in v1.1.
**Priority: HIGH**

**Probe 2:** Construct a pre-spawn checklist PASS case where no contract addresses any high-impact weakness from the analysis blocks (H4 falsification condition). Verify how often this occurs in practice across 20 diverse invocations.
**Why it matters:** Resolves the Interface 2 coherence gap — the single v1 quality risk with no existing mitigation. If the gap is empirically confirmed (constructible and observed in practice), a minimal fix is available without adding a spawn: add a coherence check to the pre-spawn checklist (at least one contract per high-impact weakness). This is the highest-leverage single improvement available in v1.
**Priority: HIGH**

**Probe 3:** Run both `prompt-cog` (normal mode) and `prompt-epiphany` on a corpus of ≥5 inputs spanning simple (100 words, empty INVENTORY) to complex (2000+ words, 15+ INVENTORY items), recording: INVENTORY verbatim compliance, blind quality preference, wall-clock time, and spawn count.
**Why it matters:** Provides the empirical quality floor measurement that the spec treats as an assumption. Resolves CS-1 (quality floor scope) by determining where the input-complexity boundary lies in practice. Also provides the V1.1 speed condition empirical data (does T_cold_start > T_inline(S3,S4) hold across input sizes?).
**Priority: HIGH**

**Probe 4:** Test the Role-as-Channel-Code intervention from S2: implement role declarations as structurally-named output headers (e.g., `## ANALYST OUTPUT ... ---`) and verify whether the synthesis spawn prompt assembler can extract from named channels by structural address, reducing contamination noise in the spawn prompt body.
**Why it matters:** The S2 constraint-escape path is the highest-leverage structural improvement identified for spawn prompt quality. If channel-code discipline demonstrably reduces the contamination encoded into the spawn prompt body (measurable by comparing spawn prompt coherence under named-header vs. unstructured inline execution), it closes CS-2's gap without adding a spawn.
**Priority: MEDIUM**

**Probe 5:** Measure token consumption of `prompt-cog` vs. `epiphany-prompt` on identical inputs (S4 Branch E.2.1 contradiction: prompt-cog may cost more tokens despite being faster).
**Why it matters:** The speed claim is validated; the cost claim has not been examined. If prompt-cog's orchestrator context accumulation produces higher total token consumption than epiphany-prompt's summed spawn prompts, the architecture is faster but more expensive — a relevant trade-off for token-sensitive usage environments.
**Priority: LOW**

---

## Verification summary block

V1 Logic: PARTIAL — one weak link: speed condition requires empirical assumption (T_cold_start > T_inline(S3,S4)) that is plausible but unverified; all other inferential links valid
V2 Cargo-Cult: PASS — none detected; "+32.9% novelty" is a cited source claim explicitly questioned, not asserted as proof
V3 Symmetric Scrutiny: PASS — rivals (H1–H5) incorporated into primary conclusion, not dismissed; equal burden applied
V4 Completeness: PASS — all 6 stages (S1–S6) substantively cited in conclusion
V5 Constructive: PASS — constructive spec: implement 1-spawn SKILL.md and verify spawn count = 1, INVENTORY verbatim presence, and wall-clock time < epiphany-prompt on a representative 200–2000 word technical prompt
V6 Scope: PASS — wrote stages/S7-v6-scope.txt
V7 Representational: No shift — conclusion is in the same declarative frame as input

Overall: 6 PASS / 1 PARTIAL out of 7 gates

</integration>
