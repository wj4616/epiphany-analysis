---
stage_id: S6
stage_name: Falsification Engine
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0
scale_gate: DEEP
status: complete
consolidated_hypotheses_count: 5
signals: []
---

<falsification>

<inference_modes>

<abductive>

## Abductive mode results

**Anomaly identified (Step A1):** The most surprising element of the prompt-cog design is that the spec simultaneously mandates a hard quality floor (never below `prompt-epiphany` baseline) and explicitly accepts multiple known quality degradation vectors — context contamination (DD#8), self-review bias (DD#9), approximation degrades on complex inputs (DD#8) — without providing a mechanism that resolves the contradiction. The anomaly: a quality floor guarantee exists without a quality floor enforcement mechanism.

**Candidate explanations (Step A2 — 6 candidates):**

H-A1: The "quality floor" constraint is a statistical claim about average-case behavior, not a per-invocation guarantee. The spec acknowledges that complex prompts produce degraded output (DD#8); this means the floor holds in expectation across the population of user inputs, assuming most inputs are moderate-complexity.
IF H-A1 were true, THEN the spec should qualify the quality floor with scope conditions — but it does not. The unconditional wording is a specification imprecision, not a design error.
Parsimony: 0.80 | Explanatory power: 0.65 | Uberty: 0.70 | Testability: 0.85 | Combined: 0.750
Falsification condition: Find a prompt-cog invocation on a representative moderate-complexity input that produces output rated below prompt-epiphany baseline by a blind evaluator.

H-A2: The 1-spawn architecture is valid, but its quality guarantees are bounded by input complexity — the spec implicitly assumes most user inputs fall in the "moderate-complexity" range where the approximation error stays below perceptible quality threshold. The absence of any complexity-detection trigger (to warn users or route to DEEP) is a missing specification gap, not a deliberate omission.
IF H-A2 were true, THEN a v2 "complexity detector" that triggers a DEEP escalation path (or user warning) would be the minimal gap-filler — and this is exactly what DEEP mode (already planned) would provide.
Parsimony: 0.85 | Explanatory power: 0.75 | Uberty: 0.80 | Testability: 0.90 | Combined: 0.825
Falsification condition: Demonstrate that prompt-cog produces equivalent or superior output to prompt-epiphany on a complex technical prompt (>10 INVENTORY items, 5+ interdependent constraints) without complexity detection.

H-A3: Interface 2 coherence — whether contracts actually address the highest-impact weaknesses identified in analysis — is the single unresolved quality risk in the spec. All other quality risks are mechanically addressable (verbatim INVENTORY check, presence checklist); Interface 2 coherence cannot be verified without a second agent, which the spawn budget disallows. This forces the design into accepting presence-check-as-proxy for coherence.
IF H-A3 were true, THEN the pre-spawn checklist passes even when contracts systematically address low-impact weaknesses and ignore high-impact ones — a structurally constructible failure case.
Parsimony: 0.75 | Explanatory power: 0.80 | Uberty: 0.85 | Testability: 0.75 | Combined: 0.788
Falsification condition: Construct an invocation where pre-spawn checklist PASSES (contracts non-empty) but no contract addresses any high-impact weakness from the analysis.

H-A4: Role-switch contamination is underestimated in the spec. The accepted contamination risk is characterized as "reducing synthesis quality on complex prompts" (DD#8), but S3 Illumination 1's mechanism implies contamination is more systematic: by Step 4, the context has accumulated two role-switch activations (analyst priming + ideation specialist priming), and the anti-conformity second pass runs in the most contaminated cognitive state in the pipeline — suppressing genuine contrarianism consistently, not just on complex inputs.
IF H-A4 were true, THEN the anti-conformity second pass's novelty gain is consistently below the +32.9% claimed in epiphany-prompt's DEEP architecture, across all input complexities, not just complex ones.
Parsimony: 0.65 | Explanatory power: 0.70 | Uberty: 0.65 | Testability: 0.70 | Combined: 0.675
Falsification condition: Measure anti-conformity contract novelty (in-context pass vs. spawned agent pass) on a simple input where contamination should be low.

H-A5: The choice to use 3 fixed KB snippets (chain-of-thought, structured-output, self-refine) is the most context-independent simplification in the design — selected for universal applicability, not for the specific TECHNIQUES gap analysis of the input. For inputs where the TECHNIQUES analysis identifies gaps outside these three domains (persona design, constraints framing, audience modeling), the synthesis agent receives KB guidance orthogonal to the actual enhancement task.
IF H-A5 were true, THEN synthesis quality would show topic-dependent variance: better performance on inputs needing reasoning scaffolding, XML structuring, or self-critique; degraded performance on inputs needing persona, constraint, or audience design.
Parsimony: 0.90 | Explanatory power: 0.55 | Uberty: 0.60 | Testability: 0.95 | Combined: 0.750
Falsification condition: Compare synthesis quality on a persona-design-heavy input vs. a reasoning-scaffolding-heavy input; expect divergent quality gap relative to epiphany-prompt.

H-A6: The four-item pre-spawn checklist validates presence but not coherence. A PASS can occur when contracts don't address the highest-impact weaknesses — the checklist provides false confidence that synthesis will target the right gaps. This is not an oversight but a necessary consequence of the spawn budget: coherence checking would require either a second agent spawn or a formal mapping from weakness identifiers to contract targets, neither of which is in scope for v1.
IF H-A6 were true, THEN the pre-spawn checkpoint functions as a "fire-check" (is there fuel?) rather than an "aim-check" (is the aim correct?). The synthesis agent fires on contracts that may systematically miss the design target.
Parsimony: 0.85 | Explanatory power: 0.70 | Uberty: 0.75 | Testability: 0.90 | Combined: 0.800
Falsification condition: Run analysis on an input with 3 high-impact and 5 low-impact weaknesses; verify whether the generated contract list proportionately addresses high-impact weaknesses or distributes uniformly.

**Top 3 candidates (Step A4, ranked by combined score):**
1. H-A2: Bounded approximation scope (0.825)
2. H-A6: Pre-spawn checklist false confidence (0.800)
3. H-A3: Interface 2 single unresolved risk (0.788)

</abductive>

<deductive>

## Deductive mode results

**Premises collected (Step D1 — from S1 irreducibles, no S5 available per module protocol note):**

- P1 (A1): 1-spawn model; Steps 0–5, 7 inline; Step 6 (synthesis) is the sole agent spawn.
- P2 (A2): Steps 3 and 4 use role-switched framing within orchestrator context — not spawned agents.
- P3 (A3): No session directory; inter-agent state passes via synthesis spawn prompt body only.
- P4 (C1): Must run faster than `epiphany-prompt`.
- P5 (C2): Must not produce output below `prompt-epiphany` quality baseline.
- P6 (C4): Spawn budget exactly 1.
- P7 (C5): No repair loops in v1.
- P8 (C11): Every INVENTORY item must appear verbatim in synthesis output.
- P9 (E3): DEEP expansion, repair loops, second verifier all deferred.
- P10 (DD#9): Inline self-verification is a conscious quality downgrade; self-review bias is a known LLM failure mode.
- P11 (DD#8): Role-switched framing accepts reduced synthesis quality on complex prompts.
- P12 (C6): INVENTORY must use full schema in all modes.

**Derived consequences (Step D2):**

D-C1: From P6 + P7:
  Premise A: Spawn budget = 1 (P6)
  Premise B: No repair loops in v1 (P7)
  Therefore: Every synthesis failure becomes visible annotated output with no retry path.
  Validity: ✓ (confirmed by R3 in S1 stripped irreducibles)
  Test against problem: HOLDS — the spec explicitly routes FAIL path to annotated output. No contradiction.

D-C2: From P2 + P10 + P3:
  Premise A: Steps 3–4 run inline in orchestrator context (P2)
  Premise B: Synthesis spawn prompt body is assembled from inline context outputs (P3)
  Premise C: Inline self-verification is known to exhibit self-review bias (P10)
  Therefore: The synthesis spawn prompt body carries cognitive state residue from Steps 3–4 role switches, including potential contamination effects that bias synthesis execution.
  Validity: ✓ valid. Contamination is not just a synthesis quality concern — it is structurally encoded into the spawn prompt inputs.
  Test against problem: The spec acknowledges contamination (DD#8) but frames it as affecting synthesis output quality, not spawn prompt integrity. Partial contradiction: spawn prompt quality is also affected, not just synthesis agent behavior.

D-C3: From P5 + P10 + P11:
  Premise A: Must not produce below prompt-epiphany baseline (P5)
  Premise B: Self-review bias is a known quality downgrade (P10)
  Premise C: Reduced quality on complex prompts is accepted (P11)
  Therefore: P5 cannot be interpreted as an absolute per-invocation guarantee — P10 and P11 guarantee degradation vectors exist that will cross the baseline on some inputs.
  Validity: ✓ valid. The conjunction of P5 (absolute floor) + P10 (known downgrade) + P11 (accepted quality reduction) is jointly contradictory.
  FLAG: P5 as stated is false if read as absolute. **Alternative: P5 is an expected-value floor, not a per-invocation guarantee. The spec should scope P5 to "moderate-complexity inputs where contamination is sub-threshold."**

D-C4: From P8 + P10 + P6:
  Premise A: Every INVENTORY item must appear verbatim (P8) — hard constraint
  Premise B: Verification of verbatim compliance is inline self-verification (P10) — known bias source
  Premise C: No second verification agent possible (P6, spawn budget = 1)
  Therefore: Hard constraint P8 has no hard enforcement mechanism — only a soft inline check subject to known bias.
  Validity: ✓ valid. This is a structural gap: a hard constraint enforced only softly.
  Test: D-C4 holds. The spec acknowledges (DD#9) that self-review is a conscious downgrade. INVENTORY verbatim is the one hard constraint most likely to survive self-review bias (mechanical substring check), but "most likely" is not a guarantee.

D-C5: From P4 + P6 + P9:
  Premise A: Speed advantage requires faster execution than epiphany-prompt (P4)
  Premise B: Speed savings derive from: eliminating repair loop spawns, eliminating session directory I/O, no analysis/ideation spawns (from P6, P7, P9)
  Premise C: All speed sources are properties of epiphany-prompt's overhead, not of prompt-cog's absolute cost
  Therefore: Speed advantage is contingent on epiphany-prompt NOT being optimized. If epiphany-prompt eliminates repair loops or session directory, prompt-cog's speed advantage shrinks.
  Validity: ✓ valid — speed is a relative, not absolute, claim.
  Test: UNKNOWN — depends on future epiphany-prompt versions.

**Flagged premises (Step D3):**
- P5 (quality floor): Contradicts P10 + P11 unless interpreted as expected-value floor, not per-invocation guarantee. **Flagged.**

**Alternative hypotheses from flagged premises (Step D4):**

H-D1: "The quality floor (P5) is an expected-value constraint, not a per-invocation guarantee. The spec implicitly assumes most user inputs are moderate-complexity where contamination effects remain below perceptible quality threshold. The wording of P5 is imprecise — it should read 'in the expected case, output meets or exceeds prompt-epiphany baseline.'"
Premise challenged: P5 as absolute guarantee.

H-D2: "INVENTORY verbatim compliance (P8) has a structural enforcement gap: the hard constraint exists without a hard verification mechanism. The real behavioral guarantee is 'INVENTORY verbatim compliance when self-review bias does not cause a miss' — which is empirically strong for mechanical substring checks but structurally unverified. v2's second verifier agent would close this gap."
Premise challenged: The implied enforcement adequacy of inline self-verification for P8.

</deductive>

<inductive>

## Inductive mode results

**Instances identified (Step I1 — 10 specific design decisions from the input):**

I1: Anti-conformity second pass: ported from spawned agent (epiphany-prompt DEEP M12) → in-context contrarian re-read. Quality benefit claimed retained; spawn cost eliminated.
I2: Pre-spawn module validation: ported from 8-key frontmatter check → 4-item presence checklist. Simplified because no module files exist.
I3: KB sources: ported from dynamic per-module selection → 3 fixed snippets inlined. Dynamic selection deferred.
I4: Session directory: eliminated. State passes via spawn prompt body. No quality regression claimed.
I5: Repair loops: deferred. Failures annotate as unverified output. Primary speed gain over epiphany-prompt.
I6: DEEP expansion wave: deferred. Richer output on complex prompts sacrificed for architecture stability.
I7: Weakness impact scoring: ported from dedicated step → inline during analysis step. Zero overhead for scoring.
I8: Return value contracts: ported as-is. Exact format unchanged. Deterministic NLP-free routing preserved.
I9: Hard Gate 3 reminder: ported as-is. Verbatim embedding in synthesis spawn prompt.
I10: Role-switched framing replaces agent spawns for Steps 3–4.

**Generalizations (Step I2 — ≥3):**

G1: *Every simplification preserves quality-critical enforcement mechanisms (contract schema, return format, verbatim requirements) while eliminating infrastructure overhead (session files, spawns, repair loops). Pattern: "enforcement mechanisms port as-is; infrastructure mechanisms are eliminated or simplified."*

G2: *Whenever prompt-cog replaces a spawned agent with an inline operation, it substitutes protocol compliance for agent isolation. The substitution is claimed quality-neutral based on the mechanical nature of the compliance check. Pattern: "protocol compliance substitutes for agent isolation when the quality check is mechanical."*

G3: *Every deferred feature involves a non-mechanical quality operation (expansion, recovery, independent judgment-based verification), while every ported feature involves a mechanical quality operation (schema presence, format match, verbatim substring check). Pattern: "mechanical checks port; judgment-dependent checks defer."*

**Boundary testing (Step I3):**

G1 boundary test — Does G1 hold for anti-conformity (I1)?
Anti-conformity's quality contribution (+32.9% novelty gains) depends on contrarian cognitive activation — not a mechanical check. The enforcement mechanism (contrarian re-read protocol) is preserved in form. But the quality guarantee degrades: in-context contamination from analyst + ideation priming suppresses genuine contrarianism (S3 Illumination 1 confirms: synthesis agent Step 6 runs in "the most contaminated cognitive state in the pipeline").
**Result: G1 is LIMITED TO mechanical enforcement mechanisms. Non-mechanical quality mechanisms survive porting in protocol form but not in full functional quality.**

G2 boundary test — Does G2 hold for inline self-verification?
Self-verification substitutes for independent verifier agent. The protocol: "synthesis agent checks its own output." Design Decision #9 explicitly calls this a *quality downgrade*, not quality-neutral substitution. Self-review bias is a known LLM failure mode — the synthesis agent that produced an error is less likely to catch it. G2 breaks at this boundary.
**Result: G2 is LIMITED TO mechanical checks (verbatim substring presence is robust even under self-review bias) but fails for judgmental checks (semantic quality of enhancement application, contract relevance to weakness).**

G3 boundary test — Does G3 hold for the 3 fixed KB snippets (I3)?
KB snippet dynamic selection was deferred; KB snippet usage was ported (simplified). The deferred element is the judgment-dependent selection logic; the ported element is the mechanical presence of KB content in the synthesis prompt. G3 holds: judgment-dependent part deferred, mechanical part ported.
**Result: G3 is ROBUST at this boundary.**

Additional boundary for G3 — Does weakness impact scoring (I7) fit the pattern?
Weakness impact scoring is ported inline. The scoring itself (high/medium/low assignment) requires judgment — it is not a purely mechanical operation. But the boundary condition: scoring could assign all weaknesses as "medium" uniformly and still pass all validation checks. The mechanical enforcement is absent; the judgment-dependent scoring quality is accepted as-is.
**Result: G3 shows LIMITED scope here — some ported operations (weakness impact scoring) have judgment-dependent elements that survive porting in form but not in guaranteed quality.**

**Alternative hypotheses from failed generalizations (Step I4):**

H-I1: "The true pattern is not 'enforcement mechanisms port; infrastructure eliminates' — it is more precisely 'mechanical checks port at full quality; non-mechanical quality mechanisms port at degraded quality but are not labeled as such in the spec.' The spec's quality claims are systematically overconfident for non-mechanical checks: anti-conformity contrarianism, synthesis self-verification, weakness impact scoring judgment, and complex synthesis decisions are all quality-degraded when ported inline, but the spec frames them as quality-neutral simplifications."
Bounded by: the generalization holds only for genuinely mechanical operations (verbatim match, format parse, schema presence check).

H-I2: "The substitution principle — 'protocol compliance replaces agent isolation for quality assurance' — is valid only when the quality property being assured is mechanical (present/absent, verbatim/not-verbatim). When applied to quality properties requiring judgment (is this contract genuinely contrarian? is this enhancement semantically accurate?), the substitution consistently produces quality degradation that the spec acknowledges individually (DD#8, DD#9) but does not generalize into a design-level principle. The spec treats each degradation as an isolated accepted trade-off when they are instances of a single pattern."
Bounded by: applies whenever an agent-isolation quality guarantee is replaced by in-context protocol compliance for a judgment-dependent quality property.

</inductive>

</inference_modes>

<consolidated_hypotheses>

## Consolidated hypotheses

**Input from all three modes:**
- Abductive top 3: H-A2 (0.825), H-A6 (0.800), H-A3 (0.788)
- Deductive alternatives: H-D1 (quality floor as expected-value), H-D2 (INVENTORY enforcement gap)
- Inductive alternatives: H-I1 (spec overconfident on non-mechanical), H-I2 (substitution principle bounded)

**Deduplication (Step C2):**
H-A3 and H-A6 are structurally equivalent — both identify Interface 2 coherence as the locus of unresolved design risk, both trace to spawn budget as the root cause. Merged → **H-MERGED-1.**
H-I1 and H-I2 are partially equivalent — I2 describes the mechanism (substitution principle), I1 describes the consequence (spec overconfident on non-mechanical checks). Merged → **H-MERGED-2.** Note: H-A4 (abductive) partially overlaps with H-MERGED-2 but was below top-3 threshold; its mechanism (anti-conformity contamination) is absorbed into H-MERGED-2.

**Cross-mode validation (Step C3):**

| Hypothesis | Abductive | Deductive | Inductive | Mode count |
|---|---|---|---|---|
| H-MERGED-1: Interface 2 coherence gap | ✓ (H-A3, H-A6) | — | — | 1 |
| H-A2: Bounded approximation scope | ✓ | ✓ partial (D-C5) | ✓ partial (G1 boundary) | 3 |
| H-D1: Quality floor as expected-value | ✓ partial (H-A1) | ✓ (flagged P5) | ✓ partial (G2 boundary) | 3 |
| H-D2: INVENTORY enforcement gap | — | ✓ (D-C4) | — | 1 |
| H-MERGED-2: Spec overconfident on non-mechanical checks | ✓ partial (H-A4) | ✓ (D-C2) | ✓ (G1, G2 failures) | 3 |

**Final ranked list (Step C5 — count = 5):**

**H1 — Spec systematically overconfident on quality claims for non-mechanical checks**
Confidence: 0.91 | Supporting modes: Abductive + Deductive + Inductive
When prompt-cog ports a quality mechanism inline rather than via a spawned agent, the spec acknowledges each individual quality downgrade (DD#8, DD#9) but does not generalize them into a design-level pattern. The unacknowledged pattern: the substitution of protocol compliance for agent isolation maintains quality only for mechanical checks (verbatim match, format parse, schema presence); for judgment-dependent checks (contrarian ideation quality, synthesis semantic accuracy, cross-constraint reasoning), the substitution consistently produces quality degradation. The spec is structurally overconfident because it treats each degradation as an isolated acceptable trade-off when they are instances of a single architectural constraint.
Falsification condition: Find an invocation where in-context anti-conformity produces equivalent novelty to spawned agent anti-conformity, or where inline self-verification catches all semantic enhancement errors as reliably as independent verification. Either finding would disconfirm H1.

**H2 — Quality claims are bounded by input complexity; absence of complexity detection is a specification gap**
Confidence: 0.88 | Supporting modes: Abductive + Deductive + Inductive
The design's quality and reliability claims (quality floor, deterministic execution, INVENTORY verbatim compliance) hold in the expected case for moderate-complexity inputs. On complex inputs (large INVENTORY, many interdependent constraints, deeply technical content requiring cross-constraint synthesis judgment), all three degradation vectors compound: context contamination from Steps 3–4, self-review bias in Step 6, and possible spawn prompt truncation. The spec contains no complexity-detection trigger that would warn the user or escalate to DEEP mode. This is not a design error — DEEP mode is explicitly planned for v2 — but the spec does not identify input complexity as the threshold condition that determines when the architecture degrades below the quality floor.
Falsification condition: Demonstrate that prompt-cog maintains quality floor on a complex input (>12 INVENTORY items, 5+ interdependent high-impact weaknesses) against a blind evaluator comparison with prompt-epiphany baseline.

**H3 — Quality floor constraint (P5) is an expected-value claim, not a per-invocation guarantee**
Confidence: 0.85 | Supporting modes: Abductive + Deductive + Inductive
The constraint "must not produce output below prompt-epiphany quality baseline" is stated without scope qualification. But the conjunction of P10 (self-review bias is a known downgrade) and P11 (reduced quality on complex prompts is accepted) makes the absolute interpretation of P5 formally contradictory. The specification is internally consistent only if P5 is interpreted as an expected-case floor: "in the expected case (moderate-complexity input, non-adversarial synthesis context), output meets or exceeds prompt-epiphany baseline." The spec should make this qualification explicit — the current wording creates a false guarantee that implementers and users may rely on.
Falsification condition: Demonstrate consistent quality at or above prompt-epiphany baseline across a representative sample of diverse inputs including complex technical prompts (if true, H3 is disconfirmed — the floor is genuine, not statistical).

**H4 — Interface 2 coherence gap: pre-spawn checklist validates presence, not coherence**
Confidence: 0.82 | Supporting modes: Abductive
The four-item pre-spawn checklist validates that analysis was produced, INVENTORY was populated, contracts exist, and the spawn prompt is assembled. It does not validate that contracts address the highest-impact weaknesses identified in analysis. A PASS can occur when contracts are generated for low-impact weaknesses only, or when the ideation step produces contracts that are formally valid but address the wrong gaps. This is a direct consequence of the spawn budget constraint: coherence checking would require either a second agent (spawn budget = 1 prevents this) or a formal weakness-to-contract mapping (not in scope for v1). The Interface 2 coherence gap is the single unresolvable design risk within v1 constraints.
Falsification condition: Construct an invocation where the pre-spawn checklist PASSES (all four items satisfied) but no contract in the list addresses any high-impact weakness from the analysis blocks. This case is trivially constructible — if it exists, H4 is confirmed.

**H5 — INVENTORY verbatim compliance hard constraint has soft enforcement only**
Confidence: 0.78 | Supporting modes: Deductive
P8 (every INVENTORY item must appear verbatim in synthesis output) is a hard constraint. The only enforcement mechanism is inline self-verification (P10, known self-review bias). No independent verifier agent is possible (P6, spawn budget = 1). For mechanical verbatim checks (substring presence), self-review bias is relatively benign — the synthesis agent can reliably check whether a specific string appears in its own output. The enforcement gap widens for INVENTORY items that require judgment about placement context (e.g., a verbatim code block placed in an inappropriate XML section is "verbatim" but misplaced — and the inline check may not catch the misplacement). The v2 second verifier agent would close this gap structurally.
Falsification condition: Find a synthesis output that contains every INVENTORY item as a verbatim substring but places one or more items in semantically incorrect XML sections (e.g., a URL verbatim in `<role>` rather than `<context>`). If this occurs, H5's placement-context variant is confirmed even if substring presence is maintained.

</consolidated_hypotheses>

<extreme_scenarios>

## Extreme scenarios

Testing primary conclusion: **"1-spawn inline orchestration architecture meets quality, speed, and reliability requirements simultaneously."**

**Regime 1 — Parameter → 0 (Simplest possible input: 2-sentence prompt, no verbatim content):**
INVENTORY = all-empty `[]`. No preservation targets. Synthesis agent has maximal freedom. Quality floor: no preservation failure possible. Contamination from Steps 3–4 is minimal (short analysis, few contracts). Self-review bias risk low (few items to check).
**Claim behavior: HOLDS.** At lower extreme, all three requirements (quality, speed, reliability) are satisfied. The architecture is optimal for simple inputs.

**Regime 2 — Parameter → ∞ (Maximally complex input: 500+ token technical prompt, 15+ INVENTORY items, 20+ required contracts, deeply interdependent constraints):**
Spawn prompt body approaches context limit. KB snippets drop first (priority order P6 in spec). Possibly analysis blocks truncated. Synthesis agent operates with partial context. All three degradation vectors compound: context contamination from extended Steps 3–4, self-review bias on large INVENTORY, truncated KB guidance.
**Claim behavior: FAILS.** Quality floor breach likely. Speed still holds (still 1 spawn). Reliability partial — INVENTORY verbatim mechanically attempted but spawn prompt integrity compromised by truncation. This is the concrete failure regime for the primary conclusion.
Failure reveals: Claim's scope is bounded to inputs where the full spawn prompt fits within context limits without truncation.

**Regime 3 — Population → 1 (Single invocation, no iteration, no warm-up):**
The architecture is stateless per invocation by design (no session directory, no state persistence). One call produces one output. Hard Gate 3 doesn't require warm-up. Pre-spawn checklist verifies from-scratch.
**Claim behavior: HOLDS.** Architecture is designed for cold single invocations. No degradation from cold start.

**Regime 4 — Population → ∞ (Massive usage across diverse inputs):**
Across a large, diverse population of inputs, some proportion will be complex enough to trigger quality floor breach (Regime 2). No mechanism exists to flag these invocations. Users applying prompt-cog to complex technical prompts receive sub-baseline output without a warning signal. Over large usage, the architecture produces systematically undetected quality failures for the complex-input tail.
**Claim behavior: FAILS statistically.** Quality floor maintained on average; violated for the complex-input tail without alerting mechanism. Speed and reliability hold; quality fails for a meaningful population fraction.

**Regime 5 — Time → 0 (First invocation immediately after implementation):**
Stateless architecture designed to work cold. Flag detection, input routing, role-switch framing, pre-spawn checklist, synthesis spawn, return parsing — all are self-contained per invocation. No warm-up dependency.
**Claim behavior: HOLDS.** Cold-start behavior is equivalent to steady-state behavior. The architecture has no initialization artifact.

**Regime 6 — Time → ∞ (Asymptotic: skill ages, source skills are updated):**
Over time, `prompt-epiphany` and `epiphany-prompt` may be updated with new techniques, enhancements, or T-set extensions. prompt-cog's 3 fixed KB snippets (chain-of-thought, structured-output, self-refine) become increasingly stale relative to the evolving techniques landscape. Quality floor measured against an updated prompt-epiphany baseline may erode even without any prompt-cog changes.
**Claim behavior: CONDITIONAL HOLDS at t=0; ERODES asymptotically.** The fixed KB selection and hardcoded T1–T13 technique reference create version-coupling drift. The quality floor is relative, and the reference may move.

**Regime 7 — Adversarial scenario (synthesis agent actively optimizing against the claim):**
A synthesis agent that follows every formal rule but interprets "enhance without executing" adversarially: claims execution IS enhancement for agentic workflow prompts; produces formally valid XML with correct `<meta source="prompt-cog"/>` and `<task>` section, but Step 6 produces an output that re-enacts the input workflow rather than enhancing the prompt describing it. Hard Gate 3 verbatim reminder is a deterrent, not a mechanical enforcement — no test exists to verify "is this an enhanced prompt or a simulated execution?"
**Claim behavior: FAILS against adversarial optimization.** No mechanical enforcement distinguishes "enhanced prompt" from "simulated execution of prompt content" for agentic or workflow inputs. The adversarial gap is most exploitable on exactly the inputs where Hard Gate 3 is most important.

**Regime 8 — Random/noisy scenario (all inputs are uniformly random noise):**
Step 0: no flag detected. Step 1: Type A (everything else). Step 2: Sufficiency check — "fundamentally ambiguous" input blocks. If noise forms a grammatically valid sentence fragment, sufficiency may pass. If blocked: clean halt with explanation. If passed: INVENTORY = all empty; synthesis produces minimal output with no preservation targets. VERIFICATION: PASS returned with minimal but structurally valid XML.
**Claim behavior: HOLDS (degrades gracefully).** Sufficiency check provides the relevant protection. If it passes noise, the output is minimal but valid. No catastrophic failure. Reliability holds; quality is undefined (no meaningful input = no meaningful output); speed holds.

</extreme_scenarios>

<millikan_assessment>

## Millikan assessment

Testing for differential effort between supporting the primary conclusion vs. refuting alternative hypotheses:

```
[MILLIKAN] H1 — Spec overconfident on non-mechanical quality:
  Effort to refute H1: LOW
  The spec itself provides the refutation evidence (DD#8, DD#9) — it explicitly names self-review bias, 
  contamination, and approximation degradation. The spec did not work hard to hide these; 
  they are surfaced as design decisions.
  Effort to support primary conclusion (quality is adequate): HIGH
  Requires: statistical argument about "low baseline failure rate" (DD#2), 
  appeal to "well-specified synthesis prompt reduces error rate" (DD#8), 
  and the deferred v2 improvement path.
  Asymmetry: YES — evidence against the quality adequacy claim is available in the spec itself 
  at zero search cost; supporting the claim requires an empirical argument not yet validated.
  Additional counter-evidence sought: Empirical comparison of anti-conformity contract novelty 
  (in-context vs. spawned agent) on the same input. If novelty is consistently below +32.9%, 
  H1 is confirmed for the anti-conformity component specifically.

[MILLIKAN] H2 — Bounded approximation scope / no complexity trigger:
  Effort to refute H2: LOW
  DD#8 directly states "reduced synthesis quality on complex prompts" — no independent search needed. 
  The architecture explicitly bounds its quality claims.
  Effort to support primary conclusion (claims hold across all inputs): HIGH
  Requires defining "complex" narrowly enough that most user inputs fall outside it, 
  and arguing that the quality floor is maintained up to the complexity definition boundary.
  Asymmetry: YES — the bound is documented; the defense requires an undefined scope qualification.
  Additional counter-evidence sought: What fraction of actual user invocations involve "complex prompts" 
  by DD#8's implicit definition? If >20%, quality floor is systematically breached on a material 
  usage fraction, confirming H2's practical significance.

[MILLIKAN] H3 — Quality floor as expected-value:
  Effort to refute H3: MEDIUM
  Could argue the floor holds on the "common case" and the exceptions are rare edge cases acceptable 
  in engineering practice. Requires the "edge case" characterization of complex inputs to be defensible.
  Effort to support primary conclusion (absolute per-invocation floor): MEDIUM
  Requires arguing that contamination + self-review bias never compound enough to cross the baseline 
  on any input — which contradicts P10 + P11.
  Asymmetry: SLIGHT — spec's silence on scope qualification is asymmetric evidence against 
  the absolute interpretation. A well-specified quality floor would state its scope conditions.

[MILLIKAN] H4 — Interface 2 coherence gap:
  Effort to refute H4: LOW
  The four-item checklist is in the spec and literally checks only: analysis present, INVENTORY present, 
  contracts non-empty, spawn prompt assembled. No coherence check exists. H4 describes the checklist 
  design, which is factually verifiable.
  Effort to support primary conclusion (pre-spawn checklist is adequate): HIGH
  Requires arguing that non-empty contracts imply addressing high-impact weaknesses — 
  which is demonstrably false by counterexample (3 trivial contracts = PASS regardless of weakness coverage).
  Asymmetry: YES — checklist design is transparent and directly confirms H4. 
  The defense requires a logical leap (presence → adequacy) that the spec does not make.

[MILLIKAN] H5 — INVENTORY enforcement gap:
  Effort to refute H5: MEDIUM
  Could argue inline self-verification is adequate for mechanical verbatim checks (substring presence), 
  making the "gap" practically negligible for the most common INVENTORY verification task. 
  The placement-context variant of H5 requires a more exotic failure to occur.
  Effort to support primary conclusion (hard constraint adequately enforced): MEDIUM
  Could argue the synthesis protocol's explicit verbatim instruction + inline check provides 
  sufficient enforcement for the mechanical task.
  Asymmetry: NO — balanced. This is the least asymmetric hypothesis. 
  The enforcement gap is structural but its severity is empirically open.
```

**Summary of asymmetric findings:**
- H1, H2, H4 show significant effort asymmetry — evidence supporting these hypotheses is available in the spec at zero search cost, while defending the primary conclusion's quality adequacy requires arguments not yet validated empirically.
- H3 shows slight asymmetry — spec silence on quality floor scope qualification.
- H5 is balanced — structural gap is real but severity is empirically open.

**Millikan verdict:** The primary conclusion's quality adequacy claims are not cargo-cult — the spec genuinely acknowledges degradation vectors (DD#8, DD#9). However, the consistent pattern is that individual acknowledgments never aggregate into a design-level principle: the spec does not generalize from multiple individual quality downgrades to a systemic claim about the architecture's quality boundary. This asymmetry (detailed per-decision acknowledgment without systemic synthesis) is itself a Millikan-style bias: the spec "works harder" to justify each individual decision than to examine what the pattern of decisions implies at a higher level.

</millikan_assessment>

<density_report>

## Density report

```
Disconfirming evidence found: 8 items

Counter-examples:
  1. Complex input with large INVENTORY (15+ items) hitting token budget → KB snippets dropped → 
     synthesis agent operates without technique guidance → quality degrades below prompt-epiphany 
     baseline (which provides T1–T13 definitions; documented in token budget truncation order, spec §Step 6).
  2. Heavily structured multi-constraint input → analyst role runs extensively → analyst priming 
     suppresses divergent ideation → anti-conformity in-context pass runs in "most contaminated 
     cognitive state in the pipeline" (S3 Illumination 1) → anti-conformity under-produces 
     contrarian contracts even on non-complex inputs.
  3. Agentic/executable workflow prompts → Hard Gate 3 reminder present but not mechanically 
     enforced → synthesis agent may interpret "enhance without executing" such that execution 
     IS the enhancement for workflow prompts → structurally valid output, functionally wrong.

Failure classes:
  1. Long technical prompts with extensive verbatim content (INVENTORY pressure + token budget 
     pressure compound; KB snippet drop removes technique guidance)
  2. Agentic or executable workflow prompts (Hard Gate 3 ambiguity at the enhancement/execution 
     boundary for non-static prompts)
  3. Cross-constraint synthesis tasks requiring semantic judgment (inline self-verification's 
     self-review bias specifically impairs judgment-dependent checks, not mechanical checks)
  4. Complex multi-role-switch contexts where analyst priming is strong 
     (anti-conformity second pass systematically suppressed by contamination)

Strongest objection: 
  "A well-specified synthesis spawn prompt cannot compensate for the cognitive state it was 
  built from — if analysis and ideation steps produced contaminated or incomplete outputs, 
  the spawn prompt body encodes those errors, and the synthesis agent faithfully executes 
  flawed contracts. The spawn prompt being 'well-specified' in form does not guarantee it is 
  'well-specified' in content."

Weakest link in mechanism: 
  The claim that "well-specified synthesis prompt compensates for context contamination" (DD#8) 
  is asserted as rationale without a structural guarantee. It relies on an empirical assumption 
  ("low baseline failure rate on a well-specified synthesis prompt") that is stated but not 
  measured. The weakest link is specifically the gap between "spawn prompt form is well-specified" 
  and "spawn prompt content (contracts) is coherent with analysis findings."

Density assessment: HIGH (8 items — 3 counter-examples + 4 failure classes + 1 mechanism gap)
```

**Cargo-cult scan results:**

Three patterns from the spec examined:

Pattern 1 — "The anti-conformity second pass is a simplification with quality benefit retained":
This framing appears to falsify itself (acknowledges the simplification) but does not challenge. Genuine falsification: "The anti-conformity pass's contrarian quality degrades when run in a contaminated in-context state. By Step 4, the orchestrator has accumulated two role-switch activations. S3 Illumination 1 confirms the mechanism: synthesis agent runs in the most contaminated cognitive state, and the anti-conformity pass immediately precedes it. In-context anti-conformity is structurally contaminated, not equivalent to agent-isolated contrarianism." [Pattern replaced — not cargo-cult in final reasoning.]

Pattern 2 — "Inline self-verification is a known limitation" (DD#9):
This could be cargo-cult (naming a limitation as falsification). Genuine test applied: "The synthesis agent self-reviews INVENTORY placement. The mechanical check (verbatim substring presence) is relatively robust under self-review bias. The semantic check (is the contract applied correctly?) is not. Self-review bias specifically impairs semantic quality checks, not mechanical presence checks. The spec's 'known limitation' acknowledgment is genuine for the semantic check dimension but does not apply equally to the mechanical dimension." [Retained as genuine — two-dimensional analysis applied, not single-label acceptance.]

Pattern 3 — "Quality trade-off is real" (DD#8):
Could be cargo-cult (acknowledging a trade-off as if that constitutes analysis). Genuine replacement: "Quality degrades below baseline specifically when contamination from Steps 3–4 is large enough to distort contract generation. This is most likely on structured, multi-constraint inputs where the analyst role runs extensively — producing strong systematic-enumeration priming — before ideation begins. The degradation is not random; it is concentrated at the analyst-priming × complex-input intersection." [Cargo-cult pattern found and replaced.]

No remaining cargo-cult patterns in consolidated output.

</density_report>

</falsification>
