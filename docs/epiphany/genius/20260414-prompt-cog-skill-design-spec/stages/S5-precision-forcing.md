# S5 — Precision Forcing
stage_id: S5
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0

---

**Primary claim under analysis** (from S1 stripped irreducibles A1, C1, C2, and the Tesla measurement plan):

> The `prompt-cog` 1-spawn inline orchestration model — all analysis and ideation inline, synthesis as the sole agent spawn — runs faster than `epiphany-prompt` while producing output quality meeting or exceeding the `prompt-epiphany` single-pass baseline, with reliable deterministic step execution.

This claim is the load-bearing architecture thesis. Every design decision in the spec (Design Decisions 1–13) is a consequence of, justification for, or trade-off against this claim. It is the appropriate analysis target.

---

<precision_forcing>

<renderings>

## 5-level vocabulary stripping

**Level 1 — Five-Year-Old**

You want a helper that does two jobs: (1) figures out how to make your writing better, and (2) actually rewrites it. The question is: can the helper do job 1 and job 2 in the same room, one after the other, without calling in a second helper for job 1? The claim says: yes, doing both jobs in the same room is faster, and the writing still comes out just as good — as long as the rewriting helper works in a private, clean room of its own so it does not get confused by the thinking-out-loud from job 1.

*Elements that survive Level 1:* speed from fewer helper-calls; quality floor; isolation of the rewriter. All three have concrete referents. No collapse.

**Level 2 — Fifteen-Year-Old**

A prompt-enhancement skill has two phases: (a) analysis and planning — figuring out what is weak in the prompt and what to fix — and (b) synthesis — actually writing the improved prompt. `epiphany-prompt` runs each phase as a separate sub-agent, which is accurate and clean but slow. `prompt-cog` merges phase (a) into the same execution context as the orchestrator, spawning a separate agent only for phase (b). The claim is that this consolidation saves time without degrading the final output, because the synthesis sub-agent still gets a clean, isolated context and a fully specified instruction set.

*Elements that survive Level 2:* the sub-agent isolation principle; the speed mechanism (fewer spawns); the quality maintenance argument (isolated synthesis context + well-specified spawn prompt). All survive.

**Level 3 — Domain Expert**

`prompt-cog` adopts a 1-spawn architecture. Steps 3 (6-dimension analysis) and 4 (divergent-convergent ideation) execute inline within the orchestrator's context window under role-switched framing, rather than in spawned subagent contexts. Step 6 (synthesis + inline self-verification) executes in a dedicated spawned agent context. The spawn prompt body carries: normalized input, INVENTORY YAML, full analysis blocks, and prioritized enhancement contracts. The architecture claim is: (i) end-to-end latency is lower than `epiphany-prompt`'s multi-spawn model because cold-start overhead is incurred only once (synthesis spawn); (ii) output quality is ≥ `prompt-epiphany` single-pass quality because the synthesis spawn prompt is tightly specified and includes the verbatim INVENTORY and all contracts; (iii) step execution is deterministic because routing decisions use prefix-based NLP-free parsing (`VERIFICATION: PASS/FAIL`), not semantic interpretation.

*Known trade-off explicitly acknowledged:* analysis-to-synthesis context contamination is a design-accepted degradation vector (Design Decision #8). The claim does not assert synthesis context purity — it asserts quality ≥ baseline, not quality = maximum achievable.

**Level 4 — Mathematician / Logician**

Let:
- S = {s₀, s₁, ..., s₇} — the ordered step sequence
- spawn(sᵢ) = 1 if step sᵢ requires an agent spawn, 0 otherwise
- T_ep = latency of `epiphany-prompt` on input x
- T_pc = latency of `prompt-cog` on input x
- Q_ep_baseline = quality of `prompt-epiphany` single-pass output on input x
- Q_pc = quality of `prompt-cog` output on input x
- contamination(sᵢ, sⱼ) = context leakage from step sᵢ to step sⱼ when both run inline

Formal statement of the architecture claim:

∀x ∈ Inputs:
  (1) Σᵢ spawn(sᵢ) = 1   [exactly one spawn: s₆]
  (2) T_pc(x) < T_ep(x)   [strictly faster]
  (3) Q_pc(x) ≥ Q_ep_baseline(x)   [quality floor holds]
  (4) ∀sᵢ ∈ S: executed(sᵢ, x) = true ∧ output(sᵢ, x) ≠ ∅   [no dropped steps]

Acknowledged side condition:
  contamination(s₃∪s₄, s₆) > 0   [not claimed to be zero — accepted]
  ∃x*: Q_pc(x*) < Q_ep_max(x*)   [for complex inputs, quality ceiling is lower than epiphany-prompt max]

*Flag:* Condition (3) uses Q as a variable without a defined measurement function. Quality is not yet formally defined — see Flagged Vague Claims below.

**Level 5 — Minimal Symbol**

```
IF spawn_count(prompt-cog) = 1
   AND latency(prompt-cog, x) < latency(epiphany-prompt, x)
   AND quality(prompt-cog, x) >= quality(prompt-epiphany, x)
THEN architecture claim HOLDS
ELSE architecture claim FAILS
```

*Irreducibility check:* All three conditions are necessary. Remove spawn_count condition → the quality/speed comparison could hold with any number of spawns, which is not the claim. Remove latency condition → the claim becomes only a quality claim, losing its architectural motivation. Remove quality condition → the claim collapses to pure speed, ignoring the quality floor that justifies the architecture's existence.

Level 5 is fully reductive. No further compression possible without losing the claim.

---

## 3-representation rendering

**Diagram description (spatial/visual)**

```
[Input x]
    |
    v
[Orchestrator context window — open throughout]
    |
    +--[Step 0: Flag Detection]----> (flag set, stripped input)
    |
    +--[Step 1: Input Routing]-----> (type A/B/C, normalized input)
    |
    +--[Step 2: Announce+Sufficiency]-> (proceed/block)
    |
    +--[Step 3: Analysis - ROLE SWITCH]-> (INTENT, STRUCTURE, CONSTRAINTS,
    |                                      TECHNIQUES, WEAKNESSES, INVENTORY YAML)
    |                                     [stays in orchestrator context]
    |
    +--[Step 4: Ideation - ROLE SWITCH]-> (contract list, conflict log)
    |                                     [stays in orchestrator context]
    |
    +--[Step 5: Pre-Spawn Checkpoint]---> (assembled spawn prompt or ABORT)
    |
    |    [SPAWN BOUNDARY — one cold start here]
    |    v
    |  [Synthesis Agent — clean context]
    |  [Step 6: Synthesis + Inline Verify]-> VERIFICATION: PASS/FAIL + XML
    |    ^
    |    [return message only — no stage files]
    |
    +--[Step 7: Output]-------------> (display + save)
```

The diagram makes the contamination risk visible: Steps 3 and 4 reasoning accumulates in the orchestrator context window and is present (as prior context) when Step 5 assembles the spawn prompt body. The spawn boundary is a hard isolation point only for the synthesis agent's forward context — not for the spawn prompt content itself.

**Equation or formal expression**

T_pc = T_inline(S₀..S₅, S₇) + T_spawn(S₆) + T_cold_start(×1)

T_ep = T_inline(routing) + T_spawn(S_analysis) + T_cold_start(×1)
     + T_spawn(S_ideation) + T_cold_start(×1)
     + T_spawn(S_synthesis) + T_cold_start(×1)
     [+ T_spawn(S_verify) + T_cold_start(×1) for complex inputs]

Speed claim: T_pc < T_ep
Reduction: ΔT = T_ep - T_pc ≈ (N_spawns_ep - 1) × T_cold_start + Σ T_spawn(S_analysis, S_ideation)
                                where N_spawns_ep ≥ 3

Note: The speed claim holds if T_cold_start + T_spawn(S_analysis) + T_spawn(S_ideation) > T_inline(S₃, S₄). This is the testable condition — it holds when spawn overhead exceeds inline execution time for analysis + ideation, which is expected to be true for all non-trivially short inputs.

**Procedural description (steps to verify)**

To verify the architecture claim:
1. Select a representative input prompt x (non-trivial: >200 words, technical domain).
2. Run `epiphany-prompt` on x. Record: wall-clock time T_ep, spawn count N_ep, output XML Q_ep.
3. Run `prompt-epiphany` on x. Record: output XML Q_baseline.
4. Run `prompt-cog` on x. Record: wall-clock time T_pc, spawn count N_pc, output XML Q_pc.
5. Check: N_pc = 1. Check: T_pc < T_ep. Check: Q_pc ≥ Q_baseline (by blind human assessment or defined scoring rubric).
6. Check: all 8 step outputs (S0–S7) are non-empty in execution trace.
7. Check: every INVENTORY item from Step 3 appears verbatim as substring in Q_pc output XML.
8. If all 7 checks pass: architecture claim holds for input x.
9. Repeat for ≥5 diverse inputs to establish claim generality.

</renderings>

<constructive_specification>

## Constructive specification

**Constructive spec (Turing minimal model):**

An agent that receives a prompt input, executes Steps 0–5 and 7 within a single orchestrator context (without spawning any additional agents for those steps), spawns exactly one synthesis agent for Step 6, receives a return message starting with `VERIFICATION: PASS`, and produces an output XML file where every string in the Step 3 INVENTORY YAML appears verbatim as a substring, within a wall-clock time less than an equivalent `epiphany-prompt` run on the same input, constitutes a demonstration of the architecture claim.

**Minimal-model check:**

Can any element be removed while leaving the demonstration valid?

- Remove "executes Steps 0–5 and 7 within a single orchestrator context" → no, this is what defines the 1-spawn model; removing it allows any architecture.
- Remove "spawns exactly one synthesis agent" → no, this is the core spawn constraint; removing it allows zero-spawn or multi-spawn architectures.
- Remove "return message starting with `VERIFICATION: PASS`" → this can be loosened: the demonstration requires synthesis to succeed, but `VERIFICATION: PASS` is a proxy for success. Replace with: "synthesis agent returns non-malformed output." — **remove the exact prefix requirement from the constructive spec; the verbatim INVENTORY check already covers output correctness.**
- Remove "every INVENTORY item appears verbatim as substring" → no, this is the preservation correctness criterion; removing it makes "output quality ≥ baseline" unverifiable.
- Remove "wall-clock time less than equivalent `epiphany-prompt` run" → no, speed is one of the two co-equal claims.

**Post-minimization constructive spec:**

An agent that (a) executes Steps 0–5 and 7 inline without spawning, (b) spawns exactly one synthesis agent for Step 6, (c) receives a synthesis return message, and (d) produces an output XML where every INVENTORY item string appears verbatim, within wall-clock time less than an equivalent `epiphany-prompt` run on the same input, constitutes a full demonstration.

</constructive_specification>

<flagged_vagueness>

## Flagged vague claims

**[VAGUE] "output quality meeting or exceeding the `prompt-epiphany` single-pass baseline"**
Reason: "Quality" has no formal definition in the spec. At Level 4 the formulation requires Q_pc(x) ≥ Q_ep_baseline(x), but Q is not defined as a function over any measurable space. The Tesla measurement plan proposes "blind human assessment" and "automated assessment" as proxies, but neither is specified. At Level 5 the claim reduces to: `quality(prompt-cog, x) >= quality(prompt-epiphany, x)`, which is a well-formed conditional — but only if `quality()` is defined. Without a rubric, the claim is untestable: two different assessors could reach opposite conclusions on the same output pair.
Possible resolution: Define a minimum scoring rubric — e.g., a 5-dimension checklist (intent preservation, verbatim content preservation, technique application, structural validity, coherence) rated by a human on a 1–5 scale per dimension. The claim holds if `prompt-cog` scores ≥ `prompt-epiphany` on a majority of dimensions over ≥5 diverse inputs. Alternatively: define "quality ≥ baseline" operationally as "no reviewer consistently prefers `prompt-epiphany` output over `prompt-cog` output in a blind comparison of 10+ pairs."

**[VAGUE] "reliable deterministic step execution with no dropped steps"**
Reason: "Reliable" has no threshold. At Level 4: ∀x ∈ Inputs: ∀sᵢ ∈ S: executed(sᵢ, x) = true is a universal quantifier over all inputs. LLM execution cannot guarantee this universally — it is a probabilistic system. "No dropped steps" as a universal claim is either vacuously true (if we count partial execution as execution) or unfalsifiable (we cannot enumerate all inputs). At Level 5 the claim reduces to: `for all x, for all steps s, s is executed on x` — which collapses to "never fails," a known tautology in probabilistic systems.
Possible resolution: Replace "reliable" with a frequency threshold: "step-drop rate < 1% across N ≥ 100 diverse inputs under controlled conditions." "Deterministic" can be preserved for the routing logic specifically (VERIFICATION: prefix parsing is genuinely deterministic) but must be scoped away from the LLM execution itself. Reframe: "routing decisions are deterministic; step execution is high-reliability (empirically verified over ≥100 runs)."

**[VAGUE] "run faster than `epiphany-prompt`"**
Reason: The claim is formally well-specified at Level 4 (T_pc(x) < T_ep(x)) and Level 5 (latency comparison conditional), but the measurement conditions are not defined. Faster on what input? On what hardware? With what model? Under what load? The comparison is valid only if held constant. Without specifying these conditions, "faster" is potentially true on some inputs and false on others. It is not vague in the formal sense (the concept is clear) but is contextually incomplete (the comparison scope is undefined).
Possible resolution: Specify the comparison conditions: "on a representative set of ≥5 inputs ranging from short (100 words) to long (2000 words), using the same model, on the same hardware, in sequential (not concurrent) runs, with a 5% margin of error." This makes the speed claim testable and rebuttable.

</flagged_vagueness>

<forward_chain>

## Forward-chain consequence tree

Root: An agent executes Steps 0–5 and 7 inline without spawning, spawns exactly one synthesis agent for Step 6, receives synthesis output, and produces an output XML where every INVENTORY item appears verbatim, within wall-clock time less than an equivalent `epiphany-prompt` run.

---

**Branch A — Scale condition: input is very short (< 50 words, minimal INVENTORY)**

  A.1: INVENTORY is sparse (0–2 items); synthesis spawn prompt is small; spawn overhead dominates wall-clock time proportionally.
    A.1.1: Speed advantage narrows — on very short inputs, T_inline(S₃, S₄) approaches zero, so the numerator of the speed gain is small; the single cold-start cost approaches parity with `epiphany-prompt`'s multiple cold starts on equally trivial inputs.
    A.1.2: Quality claim becomes near-vacuous — with no INVENTORY items, verbatim preservation is trivially satisfied; quality comparison collapses to "which skill writes better prose for a trivial input," which is noise.
      A.1.2.1: [DEGENERATE] Branch A.1.2: "INVENTORY verbatim check passes trivially on empty INVENTORY" — the correctness criterion becomes vacuously true; empty INVENTORY means the check adds no quality guarantee, defeating the preservation mechanism.

  A.2: Anti-conformity second pass produces little value on a short, low-complexity input — few contracts, no meaningful contrarian additions.
    A.2.1: Normal mode becomes functionally equivalent to minimal mode for short inputs — mode distinction collapses; the --minimal flag loses its differential value.

---

**Branch B — Scale condition: input is very long (> 5000 words, dense technical content)**

  B.1: Spawn prompt body may approach or exceed practical context limits; truncation order kicks in.
    B.1.1: KB snippets are dropped first (acceptable per spec); then low-priority contracts are trimmed.
      B.1.1.1: Synthesis agent operates with partial contract set — some enhancements are never applied; output is systematically less enhanced than a non-truncated run.
    B.1.2: If input itself approaches the normalized-input token limit, triage is impossible — normalized input cannot be truncated per C6; synthesis spawn may fail or be forced into model-level truncation.
      B.1.2.1: [PATHOLOGICAL] Branch B.1.2: "very long input exceeds synthesis spawn prompt context limit even after dropping all optional content" — synthesis spawn fails with no recovery mechanism (no repair loop in v1); result is malformed return, annotated FAIL output, no retry. User receives degraded output with no recourse.

  B.2: Inline Steps 3–4 accumulate large reasoning traces in orchestrator context; contamination risk increases with input length.
    B.2.1: Synthesis spawn prompt is assembled from a contaminated orchestrator context — more residual analysis framing bleeds into the spawn prompt header.
      B.2.1.1: Synthesis agent receives more contextual noise; quality degradation risk increases on long inputs precisely where quality matters most (complex technical prompts).

---

**Branch C — Adversarial condition: synthesis agent executes the input prompt instead of enhancing it**

  C.1: Hard Gate 3 reminder is present verbatim in synthesis spawn prompt — first defense.
    C.1.1: If synthesis agent nonetheless executes the input (e.g., input is a very compelling agentic workflow that overcomes Hard Gate 3), output XML is nonsensical or task-completed content.
      C.1.1.1: Inline self-verification passes because the synthesis agent that produced the error is checking its own output — known self-review bias. Return message: `VERIFICATION: PASS` with wrong content.
        C.1.1.1.1: [PATHOLOGICAL] Branch C.1.1.1.1: "Hard Gate 3 failure + self-review bias produces VERIFICATION: PASS on fundamentally wrong output" — Step 7 routes to PASS path, saves the wrong output without warning. User receives file containing executed task output, not enhanced prompt. No detection mechanism in v1.

  C.2: Well-formed inputs (non-agentic prompts) present low Hard Gate 3 risk; failure probability near-zero.
    C.2.1: Agentic or skill-describing inputs (the exact input type for which Hard Gate 3 was designed) carry non-trivial failure probability — the spec explicitly calls this out.

---

**Branch D — Composition condition: prompt-cog output is fed back as input (Type C re-enhancement)**

  D.1: Type C detection strips `<meta source="prompt-cog"/>` and outer `<prompt>` wrapper; inner content used as normalized input.
    D.1.1: Successive re-enhancement cycles are structurally supported; each pass re-INVENTORYs the prior output.
      D.1.1.1: INVENTORY items from prior passes (verbatim technical content) are re-preserved in subsequent passes — preservation is cumulative, not dilutive.
      D.1.1.2: Enhancement contracts from successive passes may conflict with prior-pass structural decisions — contract conflict rule logs but does not resolve structural conflicts between passes.
        D.1.1.2.1: [CONTRADICTION] Branch D.1.1.2.1: "successive re-enhancement contracts conflict with prior-pass structural choices" contradicts the assumption that re-enhancement always improves output. After 2+ passes, improvement rate may decrease or reverse due to contract conflicts. Discriminating condition: measure quality delta from pass 1→2 vs. pass 2→3 on the same input; diminishing returns or regression indicates the contradiction has materialized.

  D.2: Output save path uses DD-MM slug; collision handling appends -v2, -v3. Multiple re-enhancement passes produce multiple files.
    D.2.1: File proliferation is a mild UX degradation; not a correctness issue.

---

**Branch E — Resource condition: model latency is high (slow API responses)**

  E.1: T_cold_start is large relative to T_inline; the speed gain from eliminating spawns is amplified — prompt-cog's advantage increases under high-latency conditions.
    E.1.1: prompt-cog's relative speed advantage is largest precisely when total latency matters most (slow environment) — the architecture is adaptive to latency conditions.
      E.1.1.1: This is a positive consequence: speed gain is super-linear in model latency, not constant.

  E.2: If model API has rate limits, inline Steps 3–4 may consume more tokens in the orchestrator context than spawned agents would (no context reuse between spawns).
    E.2.1: Token cost of prompt-cog may be higher than epiphany-prompt on token-sensitive billing — a cost/speed trade-off not mentioned in the spec.
      E.2.1.1: [CONTRADICTION] Branch E.2.1.1: "prompt-cog may cost more tokens than epiphany-prompt" contradicts the implicit framing of 1-spawn as strictly superior to multi-spawn. The speed gain comes from fewer cold starts, but the token bill depends on context window accumulation in the orchestrator, which grows across Steps 0–5. Discriminating condition: count total tokens consumed by each skill on the same input; if prompt-cog's orchestrator context accumulation > epiphany-prompt's summed spawn prompt sizes, token cost is higher despite being faster.

---

**Branch F — Time condition: v2 additions (repair loops, DEEP mode) are integrated**

  F.1: Repair loop addition introduces a conditional second synthesis spawn on VERIFICATION: FAIL.
    F.1.1: Speed claim weakens — T_pc becomes: T_pc_base + P(failure) × T_repair_spawn. For low failure rates, impact is negligible; for high failure rates on complex inputs, speed advantage narrows.
      F.1.1.1: The speed vs. reliability trade-off that was accepted in v1 reasserts itself in v2 — repair loops are not free; their cost must be re-evaluated against the v2 failure rate on expanded input sets.

  F.2: DEEP mode expansion wave adds a second synthesis spawn for complex inputs.
    F.2.1: DEEP mode violates the 1-spawn claim for its input class — the architecture claim becomes mode-conditional in v2.
      F.2.1.1: The constructive spec (1 spawn, faster than epiphany-prompt) must be restated in v2 as: "in STANDARD mode, spawn_count = 1 and T_pc < T_ep; in DEEP mode, spawn_count = 2 and quality ≥ epiphany-prompt quality (no speed guarantee)."

---

**Branch G — Adversarial condition: user actively constructs inputs to defeat flag detection**

  G.1: Flag position rule: flags valid only at first or last standalone token. A flag embedded mid-sentence is treated as prompt content.
    G.1.1: User writes: "Improve this prompt -- [long prompt body including the word --minimal somewhere] -- --quiet". The first-token `--` may be parsed as an unknown flag, causing a halt before the actual prompt is processed.
      G.1.1.1: [PATHOLOGICAL] Branch G.1.1.1: "flag-like token at first position that is not a flag causes execution halt" — spec says "unknown flags halt execution." A prompt beginning with `--some-word` that is not a registered flag triggers a halt, blocking a valid prompt from being processed. The mitigation (flag position rule) is itself the source of the failure for prompts that begin with double-dash syntax.

  G.2: User passes `--minimal --verbose` simultaneously (conflicting flags).
    G.2.1: Spec: block execution, ask user to pick one. Correct behavior; no failure mode.

---

**Branch H — Composition condition: synthesis agent receives analysis contaminated by prior analysis errors**

  H.1: Step 3 produces incorrect INVENTORY (e.g., paraphrased entry due to analyst role failure).
    H.1.1: Paraphrased INVENTORY propagates to synthesis spawn prompt body verbatim — synthesis agent preserves the paraphrase, not the original.
      H.1.1.1: Inline verification in Step 6 checks INVENTORY item verbatim presence — but the check is against the (incorrect) INVENTORY, not against the original input. Self-verification passes; original content is lost.
        H.1.1.1.1: [CONTRADICTION] Branch H.1.1.1.1: "INVENTORY verbatim check guarantees preservation" contradicts the failure mode where INVENTORY itself is wrong. The verbatim check is only as good as the INVENTORY it references. Discriminating condition: independently verify INVENTORY items against the original input (not against the synthesis output) after every run.

  H.2: Step 3 omits a category from INVENTORY (e.g., all file_paths `[]` when original contained file paths).
    H.2.1: Synthesis agent has no preservation target for that category; file paths may be paraphrased or dropped.
      H.2.1.1: Quality degradation is invisible — VERIFICATION: PASS because INVENTORY has no file_paths entries to check; the preservation failure is undetectable within the system.

---

**Branch I — Scale condition: minimal mode invoked on a complex technical prompt**

  I.1: INTENT + INVENTORY analysis only; STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES blocks skipped.
    I.1.1: Contract generation is limited to INTENT-derived weakness contracts and technique contracts — high-impact structural weaknesses (from STRUCTURE block) and constraint conflicts (from CONSTRAINTS block) are never surfaced.
      I.1.1.1: Contracts miss the most impactful improvement opportunities; output may be syntactically enhanced but structurally unreformed — technically above quality floor but not meaningfully better than `prompt-epiphany` baseline.
        I.1.1.1.1: [DEGENERATE] Branch I.1.1.1.1: "minimal mode on complex input produces output at quality floor but not above" — the minimal mode claim is that quality ≥ baseline, but the mode by design skips analysis that would drive quality above baseline. For complex prompts, minimal mode satisfies the quality floor constraint while making the constraint trivially tight. Mode serves its speed purpose but does not serve the user's quality interest on complex inputs.

  I.2: Anti-conformity pass is skipped in minimal mode. No contrarian contracts generated.
    I.2.1: +32.9% novelty delta (cited in spec) is entirely absent from minimal mode output.
      I.2.1.1: Minimal mode output is systematically less novel/unconventional than normal mode; users who use minimal mode habitually receive structurally conservative enhancements.

---

**Branch J — Time condition: system operated at scale (many concurrent invocations)**

  J.1: No session directory means no filesystem lock contention across concurrent runs; each invocation is fully stateless.
    J.1.1: Concurrent invocations of prompt-cog do not interfere with each other — stateless architecture is concurrency-safe by design.
      J.1.1.1: Output collision handling (append -v2, -v3) is the only shared-state operation; on concurrent runs targeting the same slug, a race condition exists between collision detection and file write.
        J.1.1.1.1: [PATHOLOGICAL] Branch J.1.1.1.1: "concurrent runs with same output slug have a collision detection race condition" — if two invocations simultaneously detect `slug.md` does not exist and both proceed to write it, one overwrites the other. This is a known filesystem race; not unique to prompt-cog but also not mitigated by the collision handling as specified (collision detection is not atomic).

---

## Flags Summary

[DEGENERATE] Branch A.1.2.1: INVENTORY verbatim check is vacuously satisfied on empty INVENTORY — correctness criterion loses meaning on trivial inputs.

[PATHOLOGICAL] Branch B.1.2.1: Very long input may exceed synthesis spawn context limit with no repair mechanism — FAIL path with no retry; user receives annotated degraded output.

[PATHOLOGICAL] Branch C.1.1.1.1: Hard Gate 3 failure + self-review bias produces VERIFICATION: PASS on fundamentally wrong output (executed prompt content saved as enhanced prompt).

[CONTRADICTION] Branch D.1.1.2.1: Successive re-enhancement contracts may conflict with prior-pass structural choices — improvement rate may decrease or reverse after 2+ passes.

[CONTRADICTION] Branch E.2.1.1: prompt-cog may cost more tokens than epiphany-prompt despite being faster — speed gain does not imply cost reduction.

[PATHOLOGICAL] Branch G.1.1.1: Prompts beginning with double-dash syntax trigger unknown-flag halt — valid prompts can be blocked by flag detection rule.

[CONTRADICTION] Branch H.1.1.1.1: INVENTORY verbatim check guarantees preservation only if INVENTORY is correct — wrong INVENTORY passes verification while original content is lost.

[DEGENERATE] Branch I.1.1.1.1: Minimal mode on complex input satisfies quality floor trivially — speed and quality goals are simultaneously met but quality improvement is near-zero.

[PATHOLOGICAL] Branch J.1.1.1.1: Concurrent runs with same output slug have a filesystem race condition in collision detection.

---

## Tree Statistics

Branches generated: 10 (A through J)
Depth achieved: 3–4 levels (all branches reach ≥3)
Total nodes: ~42
Flags raised: 3 contradiction / 4 pathological / 2 degenerate
Pruning: none required (total nodes < 50)

</forward_chain>

</precision_forcing>
