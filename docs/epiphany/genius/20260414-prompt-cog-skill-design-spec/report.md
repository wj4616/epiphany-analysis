## Pipeline Status Header

> v1.1.0 · Scale: STANDARD · Pipeline: complete at 7 of 7 planned · Active conditionals: none · For raw state, re-invoke with --xml

---

## Headline Insight

The `prompt-cog` 1-spawn inline orchestration architecture is a well-formed, internally consistent approximation scheme that achieves genuine speed superiority over `epiphany-prompt` (robustly confirmed by 3 independent chains) and maintains a quality floor comparable to `prompt-epiphany` for moderate-complexity inputs — but the quality floor is an expected-value claim bounded by input complexity, not a per-invocation guarantee. The architecture's primary structural risk is that context contamination is not merely a behavioral execution risk but is encoded into the spawn prompt body itself, making spawn prompt construction the single highest-leverage quality gate in the system. The substitution of protocol compliance for agent isolation is architecturally sound for mechanical quality checks (verbatim match, format parse, schema presence) but produces systematic quality degradation for judgment-dependent checks (contrarian ideation, cross-constraint synthesis, semantic enhancement quality) — a pattern the spec acknowledges individually in design decisions but does not synthesize into a design-level principle.

The primary conclusion is this compound claim:

> The `prompt-cog` architecture is a well-specified polynomial-time approximation of `epiphany-prompt`'s optimal architecture. Speed superiority is robust and super-linear in latency. Quality floor holds for moderate-complexity inputs. The architecture's quality boundary is determined by the mechanical/non-mechanical distinction in its enforcement mechanisms: mechanical checks (INVENTORY verbatim, return format, pre-spawn presence) are reliable within v1 constraints; judgment-dependent checks (anti-conformity novelty, synthesis semantic accuracy, coherence of contracts-to-weaknesses) are degraded by context contamination and self-review bias in ways the spec documents individually but does not yet unify into a complexity-detection threshold or design-level escalation trigger. The highest-leverage v1 improvement opportunity is the spawn prompt construction quality: specifically, Interface 2 coherence (contracts-to-weakness alignment), because it is the one quality gap that affects mechanical and judgment-dependent checks equally and is not addressed by any existing mechanism in v1.

**Confidence: MODERATE** — Speed claim: HIGH confidence (3 independent chains, no divergence). Mechanical/non-mechanical quality boundary: HIGH confidence (3 independent chains). Overall architecture claim: MODERATE, because the quality floor scope (DP-1) and anti-conformity novelty transfer (DP-2) remain empirically open.

**Creativity type:** Exploratory

---

## Theory Collisions

**Collision 1 — Speed (C1) vs. Quality Floor (C2)**
- Claim A: Must run faster than `epiphany-prompt` — speed is the primary architectural constraint; 1-spawn model and elimination of repair loops exist specifically for this.
- Claim B: Must not produce lower quality output than `prompt-epiphany` baseline — inline self-verification (Design Decision #9) is acknowledged as a quality downgrade from epiphany-prompt's separate M4M5 verifier.
- Discriminating condition: Run both `prompt-cog` (normal mode) and `prompt-epiphany` on 20 representative prompts spanning simple, complex, and technically dense inputs under blind evaluation. The discriminating input class is long technical prompts (5,000+ tokens with high INVENTORY density) where self-review bias has maximum opportunity to fail. Neither claim can be softened without changing the architecture.

**Collision 2 — INVENTORY Verbatim Mandate (C11) vs. Synthesis Enhancement Quality**
- Claim A: Every INVENTORY item must appear verbatim in synthesis output — hard constraint, non-negotiable across all modes.
- Claim B: The synthesis agent's primary goal is to produce a better-worded prompt that enhances the input, including idiomatic integration, coherent flow, and natural phrasing.
- Discriminating condition: Submit a prompt where several INVENTORY items are awkwardly phrased. Evaluate whether the item appears verbatim (satisfying C11) or whether the synthesis agent inadvertently shifts it while improving surrounding text (satisfying enhancement quality at the cost of C11). Self-review bias makes the synthesis agent more likely to miss the verbatim failure when it produces a "better-sounding" result. These are different observables (verbatim substring check vs. blind quality preference) requiring different experimental setups.

**Collision 3 — Spawn Budget = 1 (C4) vs. Anti-Conformity Novelty Transfer**
- Claim A: Architecture limited to exactly 1 synthesis agent spawn — no additional spawn for anti-conformity.
- Claim B: The anti-conformity second pass claims "+32.9% novelty gains" measured in epiphany-prompt's DEEP architecture with an independent agent context. In-context re-read in an orchestrator context already containing analysis, contracts, and role framings is not the same cognitive operation.
- Discriminating condition: Run normal-mode `prompt-cog` on 10 prompts and evaluate whether anti-conformity contracts are consistently distinguishable from primary contracts in novelty. Compare against epiphany-prompt DEEP M12 additional contracts on the same inputs. If `prompt-cog`'s second-pass additions are qualitatively indistinct from primary contracts, the "+32.9% novelty gains" claim does not transfer to the inline implementation.

---

## Discovery vs. Proof

**Discovery (S2 — Constraint Escape)** | **Proof (S5 — Precision Forcing + S7 Verification)**

The binding constraint is that agent isolation (spawning a dedicated agent per step) is the quality mechanism AND the latency cost simultaneously — removing one removes the other. The escape must break this coupling. | The architecture claim reduces to three jointly necessary conditions: spawn_count = 1, latency(prompt-cog) < latency(epiphany-prompt), quality(prompt-cog) ≥ quality(prompt-epiphany). All three are necessary; removing any one loses the claim.

**Domain escape path (S2):** The Role-as-Channel-Code architecture — role declarations written as structural output markers simultaneously activate role-specific behavior (morphogen function from developmental biology) AND create addressable channel-labeled sections in the context window (multiplexing function from information theory). The synthesis spawn prompt constructor operates as a demultiplexer: it extracts from named channels by structural address, not by reading the full context. Contamination becomes an addressability problem, not a purity problem — and addressability is solved by structural encoding, not by spawning a separate agent per step. This escape path is multi-source-convergent: confirmed independently by Information Theory (FDMA/TDMA demultiplexing), Developmental Biology (morphogen + selective gene expression), and Optics (Fourier-plane spatial filtering pre-filter at conjugate representation). | **Constructive spec (S5 post-minimization):** An agent that (a) executes Steps 0–5 and 7 inline without spawning, (b) spawns exactly one synthesis agent for Step 6, (c) receives a synthesis return message, and (d) produces an output XML where every INVENTORY item string appears verbatim, within wall-clock time less than an equivalent `epiphany-prompt` run on the same input, constitutes a full demonstration. Demonstrated via: implement 1-spawn SKILL.md and verify spawn count = 1, INVENTORY verbatim presence, and wall-clock time < epiphany-prompt on a representative 200–2000 word technical prompt.

**Emergent structure from discovery (S2):** Contamination is not a purity problem but an addressability problem — this reframing is not available in either source domain alone. A new constraint becomes visible: if role declarations function as channel codes, they must appear *before* the content they govern, and output sections must be structurally terminated so the demultiplexer knows where the channel ends. Neither morphogens (Developmental Biology) nor channel codes in FDMA suggest this termination-marker requirement independently.

**V1 partial link (S5):** The speed condition requires the empirical assumption that T_cold_start + T_spawn(S_analysis) + T_spawn(S_ideation) > T_inline(S₃, S₄). This is expected to hold for non-trivially short inputs but has not been formally proven. All other inferential links are valid or validly analogical with explicit structural mapping.

---

## Independence-Verified Bridges

**Bridge 1: Representational Interference (S3, score 1.65 — highest)**
- Source concept: context contamination (S10) × role-switching framing (S4)
- Target insight: The most vulnerable pipeline boundary is Step 4→Step 6: by Step 4, the context has accumulated two role-contaminations (analyst priming + ideation specialist priming), and the synthesis agent's self-verification runs in the most contaminated cognitive state in the pipeline.
- Structural invariant: Role residue accumulates monotonically as steps progress inline; the synthesis agent inherits the full accumulation.
- Disanalogy limit: This illumination suggests a context-reset framing at the spawn boundary (not a new agent) as a v2 intervention — but it is not a proof that contamination levels are critically high; the severity is empirically open.
- Independence verified: S10 (architectural risk) and S4 (mitigation technique) have no shared ancestor within activation radius.

**Bridge 2: Approximation Architecture (S3, score 1.38)**
- Source concept: speed-vs-quality-tradeoff (S3) × no-repair-loops (S9) × inline-self-verification (S8)
- Target insight: The three deliberate quality downgrades — inline analysis/ideation, no repair loops, inline self-verification — form a coherent polynomial-time approximation scheme relative to epiphany-prompt's optimal architecture. The approximation degrades specifically at the boundary conditions where shortcuts collide with worst-case inputs: complex prompts with many interdependent contracts, deeply technical INVENTORY items, and synthesis requiring genuine judgment rather than mechanical execution.
- Structural invariant: Each quality downgrade trades optimality for elimination of one spawn overhead; together they define a measurable approximation ratio.
- Disanalogy limit: Approximation algorithms in CS have provable approximation ratios; prompt-cog's ratio is empirically estimable but not formally provable.
- Independence verified: S3, S9, and S8 are independent design decisions (speed constraint vs. recovery policy vs. verification policy).

**Bridge 3: Role-as-Channel-Code (S2, emergent from blend)**
- Source concept: Information Theory (multiplexing) × Developmental Biology (differentiation)
- Target insight: Role declarations written as structural output markers simultaneously function as behavioral activators (morphogen) AND channel codes (multiplexing). The synthesis spawn prompt is the demultiplexer. Neither source domain alone suggests this dual function.
- Structural invariant: The contamination problem is a receiver problem, not a source problem — if the receiver specification is precise, the shared medium can carry contaminated content without quality loss at output.
- Disanalogy limit: FDMA signals are continuous; role-switched context sections are discrete bounded blocks. Channel coding has formal error bounds; role-coded context extraction has no formal guarantees.
- Independence verified: Multi-source-convergent across 7 of 9 candidate concepts; 3 independent chains for the core "logical separation over shared medium" concept.

**Bridge 4: Three-Interface Architecture (S3, score 0.88)**
- Source concept: prompt-enhancement (S1) × enhancement-contracts-schema (S6) × 1-spawn-inline-orchestration (S2)
- Target insight: The 1-spawn architecture is a three-interface design problem. Interface 1 (analysis-to-ideation): defined by INVENTORY schema. Interface 2 (ideation-to-synthesis): defined by contract schema + pre-spawn checklist — validated for presence but NOT coherence. Interface 3 (synthesis-to-orchestrator): defined by VERIFICATION: PASS/FAIL return contract. Interface 2 is the reliability weak point because the pre-spawn checklist validates presence of contracts, not that contracts address the high-impact weaknesses identified in analysis.
- Structural invariant: Well-specified form does not guarantee well-specified content.
- Disanalogy limit: Software interface contracts are formally verifiable; Interface 2 coherence cannot be formally verified within v1 spawn budget.
- Independence verified: S1 (functional purpose), S6 (data structure), and S2 (orchestration mechanism) are independent problem dimensions.

**Bridge 5: Enhancement Contracts as Type System (S3, score 1.25)**
- Source concept: enhancement-contracts-schema (S6) × deterministic-execution (S11)
- Target insight: The contract schema (`technique | target_section | action | rationale | priority`) is structurally isomorphic to a typed function signature. The T4 binding rule is a type constraint. The pre-spawn checklist is a type-checking pass. Contract conflicts are type conflicts. This equivalence opens a path to contract validation more rigorous than the current presence checklist — detecting when two contracts specify conflicting operations on the same target_section with the same technique.
- Structural invariant: Typed contracts admit automated consistency checking that schema-presence checks do not.
- Disanalogy limit: Type systems in programming languages have decidable type inference; enhancement contract type checking would require semantic understanding of action fields, which is not mechanically decidable.
- Independence verified: S6 (schema design) and S11 (execution reliability) are orthogonal problem dimensions.

---

## Alternative Hypotheses

**H1 — Spec systematically overconfident on quality claims for non-mechanical checks**
Confidence: 0.91 | Supporting modes: Abductive + Deductive + Inductive
When prompt-cog ports a quality mechanism inline, the spec acknowledges each individual quality downgrade (DD#8, DD#9) but does not generalize them into a design-level pattern. The unacknowledged pattern: the substitution of protocol compliance for agent isolation maintains quality only for mechanical checks (verbatim match, format parse, schema presence); for judgment-dependent checks (contrarian ideation quality, synthesis semantic accuracy, cross-constraint reasoning), the substitution consistently produces quality degradation. The spec is structurally overconfident because it treats each degradation as an isolated acceptable trade-off when they are instances of a single architectural constraint.
Falsification condition: Find an invocation where in-context anti-conformity produces equivalent novelty to spawned agent anti-conformity, or where inline self-verification catches all semantic enhancement errors as reliably as independent verification. Either finding would disconfirm H1.

**H2 — Quality claims are bounded by input complexity; absence of complexity detection is a specification gap**
Confidence: 0.88 | Supporting modes: Abductive + Deductive + Inductive
The design's quality and reliability claims hold in the expected case for moderate-complexity inputs. On complex inputs (large INVENTORY, many interdependent constraints, deeply technical content requiring cross-constraint synthesis judgment), all three degradation vectors compound: context contamination from Steps 3–4, self-review bias in Step 6, and possible spawn prompt truncation. The spec contains no complexity-detection trigger that would warn the user or escalate to DEEP mode. This is not a design error — DEEP mode is explicitly planned for v2 — but the spec does not identify input complexity as the threshold condition that determines when the architecture degrades below the quality floor.
Falsification condition: Demonstrate that prompt-cog maintains quality floor on a complex input (>12 INVENTORY items, 5+ interdependent high-impact weaknesses) against a blind evaluator comparison with prompt-epiphany baseline.

**H3 — Quality floor constraint is an expected-value claim, not a per-invocation guarantee**
Confidence: 0.85 | Supporting modes: Abductive + Deductive + Inductive
The constraint "must not produce output below prompt-epiphany quality baseline" is stated without scope qualification. The conjunction of P10 (self-review bias is a known downgrade) and P11 (reduced quality on complex prompts is accepted) makes the absolute interpretation formally contradictory: P5 as absolute + P10 + P11 are jointly contradictory (D-C3). The spec is internally consistent only if P5 is interpreted as an expected-case floor. The current wording creates a false guarantee that implementers and users may rely on.
Falsification condition: Demonstrate consistent quality at or above prompt-epiphany baseline across a representative sample of diverse inputs including complex technical prompts. If the floor holds universally, H3 is disconfirmed.

**H4 — Interface 2 coherence gap: pre-spawn checklist validates presence, not coherence**
Confidence: 0.82 | Supporting modes: Abductive
The four-item pre-spawn checklist validates that analysis was produced, INVENTORY was populated, contracts exist, and the spawn prompt is assembled. It does not validate that contracts address the highest-impact weaknesses identified in analysis. A PASS can occur when contracts are generated for low-impact weaknesses only. This is a direct consequence of the spawn budget constraint: coherence checking would require either a second agent (spawn budget = 1 prevents this) or a formal weakness-to-contract mapping (not in scope for v1). The Interface 2 coherence gap is the single unresolvable design risk within v1 constraints — and is simultaneously the highest-leverage v1 improvement opportunity: adding a coherence check to the pre-spawn checklist (at least one contract per high-impact weakness) requires neither a second spawn nor a new mechanism.
Falsification condition: Construct an invocation where the pre-spawn checklist PASSES (all four items satisfied) but no contract in the list addresses any high-impact weakness from the analysis blocks. This case is trivially constructible — if it exists, H4 is confirmed.

**H5 — INVENTORY verbatim compliance hard constraint has soft enforcement only**
Confidence: 0.78 | Supporting modes: Deductive
P8 (every INVENTORY item must appear verbatim in synthesis output) is a hard constraint. The only enforcement mechanism is inline self-verification (P10, known self-review bias). No independent verifier agent is possible (P6, spawn budget = 1). For mechanical verbatim checks (substring presence), self-review bias is relatively benign. The enforcement gap widens for INVENTORY items that require judgment about placement context — a verbatim code block placed in an inappropriate XML section is "verbatim" but misplaced, and the inline check may not catch the misplacement.
Falsification condition: Find a synthesis output that contains every INVENTORY item as a verbatim substring but places one or more items in semantically incorrect XML sections. If this occurs, H5's placement-context variant is confirmed even if substring presence is maintained.

---

## Density-Checked Falsification

Disconfirming evidence found: 8 items — density HIGH — residual uncertainty MODERATE

**Counter-example 1:** Complex input with large INVENTORY (15+ items) hitting token budget → KB snippets dropped → synthesis agent operates without technique guidance → quality degrades below prompt-epiphany baseline (which provides T1–T13 definitions; documented in token budget truncation order, spec §Step 6). 8 vectors attempted — density HIGH — residual uncertainty: severity of quality degradation without KB snippets is empirically open.

**Counter-example 2:** Heavily structured multi-constraint input → analyst role runs extensively → analyst priming suppresses divergent ideation → anti-conformity in-context pass runs in most contaminated cognitive state in the pipeline (S3 Illumination 1) → anti-conformity under-produces contrarian contracts even on non-complex inputs. 8 vectors attempted — density HIGH — residual uncertainty: whether novelty differential is near-zero or merely reduced is not yet measured.

**Counter-example 3:** Agentic/executable workflow prompts → Hard Gate 3 reminder present but not mechanically enforced → synthesis agent may interpret "enhance without executing" such that execution IS the enhancement for workflow prompts → structurally valid output, functionally wrong. 8 vectors attempted — density HIGH — residual uncertainty: frequency of this failure in practice is unknown.

**Failure classes:** (1) Long technical prompts with extensive verbatim content — INVENTORY pressure + token budget pressure compound; KB snippet drop removes technique guidance. (2) Agentic or executable workflow prompts — Hard Gate 3 ambiguity at enhancement/execution boundary for non-static prompts. (3) Cross-constraint synthesis tasks requiring semantic judgment — inline self-verification's self-review bias specifically impairs judgment-dependent checks, not mechanical checks. (4) Complex multi-role-switch contexts where analyst priming is strong — anti-conformity second pass systematically suppressed by contamination.

**Strongest objection:** "A well-specified synthesis spawn prompt cannot compensate for the cognitive state it was built from — if analysis and ideation steps produced contaminated or incomplete outputs, the spawn prompt body encodes those errors, and the synthesis agent faithfully executes flawed contracts. The spawn prompt being 'well-specified' in form does not guarantee it is 'well-specified' in content."

**Weakest link in mechanism:** The claim that "well-specified synthesis prompt compensates for context contamination" (DD#8) is asserted as rationale without a structural guarantee. It relies on an empirical assumption ("low baseline failure rate on a well-specified synthesis prompt") that is stated but not measured. The weakest link is specifically the gap between "spawn prompt form is well-specified" and "spawn prompt content (contracts) is coherent with analysis findings."

---

## Scope Limits

**Applies to:** prompt enhancement skill architectures that use inline orchestration (analysis and ideation in a single shared context window) with exactly one isolated synthesis agent spawn, operating on moderate-complexity prompt inputs (INVENTORY ≤ ~12 items, no deeply interdependent multi-constraint synthesis tasks), where speed relative to a multi-spawn baseline is the primary architectural goal and the quality floor is defined as meeting or exceeding a single-pass non-modular baseline on those inputs
**Does not extend to:** (1) complex technical prompt inputs with large INVENTORY density (15+ items) or deeply interdependent constraints, where spawn prompt truncation, context contamination, and self-review bias compound to breach the quality floor; (2) agentic or executable workflow prompt inputs where the distinction between "enhance" and "execute" is ambiguous, rendering Hard Gate 3 non-mechanically enforceable; (3) quality claims interpreted as per-invocation guarantees across all input classes — the architecture holds its quality floor in the expected case for moderate-complexity inputs, not universally; (4) contexts where the conclusion is stated in a procedural frame — the conclusion was derived in a declarative/structural frame and requires translation before application as implementation instructions
**Claims refused:** (none)

---

## Coherence Signals

**CS-1: Quality floor is statistical, not per-invocation absolute**
Independent appearance in: S5 (precision-forcing flagged "quality" as undefined, measurement conditions unspecified); S4 (theory collision C1 vs. C2: "neither can be softened without changing the architecture"); S6 (H3, confidence 0.85: "quality floor as expected-value claim"; D-C3 formally demonstrates P5 + P10 + P11 are jointly contradictory).
Three independent chains, all pointing to the same scope qualification needed. **Strength: STRONG.**
Cross-domain recurrence: S5 reached it through precision-stripping the formal claim; S4 through dynamic theory-collision analysis; S6 through deductive contradiction of premises.

**CS-2: Context contamination is architecturally encoded at the spawn boundary, not just a behavioral risk**
Independent appearance in: S2 (binding constraint: contamination is the quality-isolation coupling; escape path via channel-code marking); S3 (Illumination 1: representational interference most acute at Step 4→6 boundary; synthesis self-verification runs in most contaminated cognitive state); S4 (Frame 1 insider: spawn prompt body carries contamination from Steps 3–4 as encoded content); S6 (D-C2: spawn prompt integrity is affected by contamination, not just synthesis output quality — partial spec contradiction).
Four independent chains, escalating from behavioral risk to structural encoding. **Strength: STRONG.**

**CS-3: The substitution of protocol compliance for agent isolation preserves quality only for mechanical checks**
Independent appearance in: S3 (Illumination 2: three quality downgrades form a coherent approximation scheme with identifiable failure boundary conditions); S6 (H1, confidence 0.91: inductive generalization G1/G2 boundary failures; H-MERGED-2; Millikan assessment asymmetry); S4 (theory collision C3: anti-conformity in-context vs. spawned agent — novelty transfer may be zero in contaminated context).
Three independent chains reaching the same mechanical/non-mechanical boundary. **Strength: STRONG.**

**CS-4: The spawn prompt construction is the highest-leverage quality determinant in the architecture**
Independent appearance in: S2 (Mapping 1 multiplexing analogy: receiver-side filter design is the quality-critical engineering point; "synthesis spawn prompt is the demultiplexer"); S3 (Illumination 5, Three-Interface Architecture: Interface 2 is the reliability weak point); S6 (H4: pre-spawn checklist validates presence but not coherence; strongest objection: "well-specified form does not guarantee well-specified content").
Three independent chains all pointing to spawn prompt construction as the single highest-leverage quality gate. **Strength: STRONG.**

**CS-5: Speed advantage is real, robust, and super-linear in model latency**
Independent appearance in: S4 (Frame E: speed gain amplified under high-latency conditions; E.1.1: "speed gain is super-linear in model latency"); S5 (formal latency equation: ΔT = (N_spawns_ep - 1) × T_cold_start + Σ T_spawn(S_analysis, S_ideation); speed holds if T_cold_start > T_inline(S3,S4)); S6 (Extreme Scenario Regime 2: "Speed still holds" even in the complex-input failure regime; H2 confirms speed claim survives even where quality claim degrades).
Three independent chains, no divergence — speed claim is the most robustly confirmed sub-claim in the architecture. **Strength: STRONG.**

---

## Generalization Checks

**Holds at:** Agentic orchestration architectures where (a) a shared context window is used for multiple sequential role-switched processing steps feeding a single isolated downstream agent, and (b) the downstream agent's quality depends on both mechanical compliance checks and judgment-dependent synthesis operations. The mechanical/non-mechanical quality boundary finding generalizes to any such architecture: mechanical checks are reliable under inline execution + self-verification; judgment-dependent checks degrade predictably with context contamination depth. This applies beyond prompt-enhancement to classification agents, planning agents, and code-generation agents using similar inline-then-isolated-spawn patterns.

**Breaks at (one-off aspects):** The specific quality floor threshold (vs. `prompt-epiphany` baseline) and the specific speed comparison (vs. `epiphany-prompt`) are instance-specific, depending on the source skills' characteristics and not transferable as absolute claims to other architectures.

**Holds at (moderate-complexity inputs):** All five convergence signals (CS-1 through CS-5) hold; the architecture operates within its designed parameters; speed, quality floor, and reliability are simultaneously satisfiable.

**Breaks at (complex inputs, INVENTORY > ~12 items or deeply interdependent constraints):** Quality floor breach likely (Regime 2 extreme scenario confirmed by S6); contamination + truncation + self-review bias compound; speed claim still holds but quality claim fails — architecture is not wrong, just out of scope.

**Breaks at (v2 additions):** If repair loops are added (v2), the speed claim becomes conditional: T_pc_v2 = T_pc_base + P(failure) × T_repair_spawn. If DEEP mode is added, the 1-spawn claim becomes mode-conditional. Both are known planned changes per the spec; the v1 claim scope remains intact.

**Holds at (asymptotic cold-start behavior):** Architecture is stateless per invocation by design; no initialization artifact; cold-start behavior equivalent to steady-state behavior (Regime 3 and Regime 5 in S6 extreme scenarios).

---

## Open Questions & Next Probes

**Probe 1 (Priority: HIGH):** Measure the novelty differential between primary contracts and anti-conformity second-pass contracts in normal-mode `prompt-cog` runs, compared to DEEP M12 spawned contrarian contracts in `epiphany-prompt` runs, on 10 diverse inputs.
Why it matters: Resolves DP-2 (the single unresolved divergence). If the novelty differential is consistently near-zero in-context, the "+32.9% novelty gains" claim is invalidated for the inline implementation, and the anti-conformity pass cost (time for second re-read) buys no measurable quality gain — making it a candidate for removal or de-emphasis in v1.

**Probe 2 (Priority: HIGH):** Construct a pre-spawn checklist PASS case where no contract addresses any high-impact weakness from the analysis blocks (H4 falsification condition). Verify how often this occurs in practice across 20 diverse invocations.
Why it matters: Resolves the Interface 2 coherence gap — the single v1 quality risk with no existing mitigation. If confirmed, a minimal fix is available without adding a spawn: add a coherence check to the pre-spawn checklist (at least one contract per high-impact weakness).

**Probe 3 (Priority: HIGH):** Run both `prompt-cog` (normal mode) and `prompt-epiphany` on a corpus of ≥5 inputs spanning simple (100 words, empty INVENTORY) to complex (2000+ words, 15+ INVENTORY items), recording: INVENTORY verbatim compliance, blind quality preference, wall-clock time, and spawn count.
Why it matters: Provides the empirical quality floor measurement that the spec treats as an assumption. Resolves CS-1 (quality floor scope) by determining where the input-complexity boundary lies in practice. Also provides the V1.1 speed condition empirical data (does T_cold_start > T_inline(S3,S4) hold across input sizes?).

**Probe 4 (Priority: MEDIUM):** Test the Role-as-Channel-Code intervention from S2: implement role declarations as structurally-named output headers (e.g., `## ANALYST OUTPUT ... ---`) and verify whether the synthesis spawn prompt assembler can extract from named channels by structural address, reducing contamination noise in the spawn prompt body.
Why it matters: The S2 constraint-escape path is the highest-leverage structural improvement identified for spawn prompt quality. If channel-code discipline demonstrably reduces contamination encoded into the spawn prompt body (measurable by comparing spawn prompt coherence under named-header vs. unstructured inline execution), it closes CS-2's gap without adding a spawn.

**Probe 5 (Priority: LOW):** Measure token consumption of `prompt-cog` vs. `epiphany-prompt` on identical inputs (S4 Branch E.2.1: prompt-cog may cost more tokens despite being faster).
Why it matters: The speed claim is validated; the cost claim has not been examined. If prompt-cog's orchestrator context accumulation produces higher total token consumption than epiphany-prompt's summed spawn prompts, the architecture is faster but more expensive — a relevant trade-off for token-sensitive usage environments.
