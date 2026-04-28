# S4 — Dynamic Simulation
stage_id: S4
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0

---

<dynamic_simulation>

<system_model>

## System model

**Components and initial states:**

| Component | Initial State | Driving Force |
|---|---|---|
| Orchestrator (Steps 0–5, 7) | Idle; waiting for invocation string | User invocation |
| Flag Detection (Step 0) | No flags parsed | Invocation string token scan |
| Input Router (Step 1) | No input typed | Stripped invocation string from Step 0 |
| Analyst role (Step 3) | No context | Normalized input |
| Ideation role (Step 4) | No contracts | Analysis output (same context) |
| Pre-Spawn Checkpoint (Step 5) | No spawn prompt assembled | Steps 3–4 output in context |
| Synthesis Agent (Step 6) | Cold spawn; context is only the spawn prompt body | Spawn prompt from Step 5 |
| Output handler (Step 7) | No return message | Synthesis agent return string |
| INVENTORY YAML | Empty schema template | Populated during Step 3 |
| Enhancement contracts list | Empty | Populated during Step 4 |

**Coupling:**

- Steps 0→1→2→3→4→5 are serially coupled within a single shared context; each step consumes the accumulated in-context state from all prior steps.
- Step 5→6 boundary is the sole context isolation boundary: the synthesis agent receives only the explicitly assembled spawn prompt body — nothing else from the orchestrator's accumulated context.
- Step 6→7 coupling: the orchestrator receives only the synthesis agent's return message string; no access to the agent's internal reasoning.

**Nominal operating condition:**

A well-formed invocation string with a supported or absent flag, containing a syntactically valid and semantically non-trivial prompt as input. The pipeline completes Steps 0–7 without halting. The synthesis agent returns a message starting with `VERIFICATION: PASS`. Output XML is saved. The INVENTORY verbatim check passes. Wall-clock time is less than an equivalent `epiphany-prompt` run.

**What "working correctly" means:** Every INVENTORY item appears verbatim in the output XML; no step is dropped; synthesis agent spawns exactly once; return message is deterministically parseable; output is saved to a collision-free path.

</system_model>

<observer_frames>

## Observer frame analyses

All three frames are constructed simultaneously and compared directly.

---

### Frame 1 — Insider Frame

*Position: I am the synthesis agent (Step 6). I can only observe the spawn prompt body as delivered to me. I have no memory of the orchestrator's execution, the original invocation string, or the user's prior context.*

**What is visible from this position:**
I receive a single block of text that contains (in stated priority order): normalized input, INVENTORY YAML, contract list, analysis blocks, and 3 KB snippets. I see Hard Gate 3 verbatim. I see T4 binding rule. I do not know:
- Whether the INVENTORY was produced carefully or hastily (I have only the populated YAML, not the analyst's reasoning)
- Whether the contract list was already filtered for conflicts, or whether some conflicts were missed during ideation
- What the user's original intent was beyond what survived normalization into the spawn prompt
- Whether the analysis blocks in the spawn prompt are the full 6-dimension analysis or the minimal-mode subset (INTENT + INVENTORY only)

**What looks normal from inside:** A well-specified spawn prompt where all four content sections are present, INVENTORY has entries, contracts are clearly formatted, and the normalized input is recognizable as a prompt to be enhanced.

**What looks wrong from inside:** If the spawn prompt was assembled near the token budget ceiling, KB snippets may have been truncated or dropped entirely. From my position I cannot distinguish between "KB snippets were deliberately prioritized out" and "KB snippet content was truncated mid-sentence." Both arrive as the same input — but the latter causes incoherent technique guidance.

**What I am missing:** I cannot see the orchestrator's reasoning about which contracts were skipped or why. I receive only the conflict log comment. If the ideation role (Step 4) applied T4 binding rule incorrectly and a T4 contract targets `<context>` instead of `<role>`, the error is now inside my spawn prompt — and I must catch it during synthesis, which I am also checking against my own production.

**Constraint binding me that outside observer would not notice:** I must return exactly `VERIFICATION: PASS\n\n<XML>` or `VERIFICATION: FAIL — [summary]\n\n<XML>`. Any deviation from this prefix format causes the orchestrator to route my output to the malformed path — silently discarding my output even if the XML itself is correct. The prefix is a structural survival requirement, not a quality gate.

**Limit case from inside:** If the spawned context window fills completely before the synthesis protocol finishes (large input + large INVENTORY + many contracts), I must truncate something. From inside, the truncation priority is undefined at synthesis time — the token budget rule was set at spawn-prompt assembly, not inside the synthesis step. I have no way to signal that I ran out of context mid-synthesis. I simply stop. The orchestrator receives a partial return string that does not start with `VERIFICATION:` and routes it to the malformed path with no quality information.

**Cross-frame conflict with Frame 2:** The orchestrator (Frame 2) sees a `VERIFICATION: PASS` prefix and concludes synthesis succeeded. From inside (Frame 1), I may have passed my own self-review despite a preservation failure — because the INVENTORY verbatim check is self-administered. Frames 1 and 2 agree that PASS was returned; they disagree on whether it is reliable.

---

### Frame 2 — Outsider Frame

*Position: I observe prompt-cog from outside — I see the invocation string and the final output XML, nothing in between. I cannot see the orchestrator's steps, the synthesis agent's reasoning, or the INVENTORY.*

**What is visible from this position:**
I see the user's input prompt and the resulting enhanced XML output. I can check whether the output XML contains `<meta source="prompt-cog"/>` as first child. I can check whether `<task>` is present. I can visually compare the input against the output. I can read the `<!-- Skipped contracts: -->` comment if it appears.

**What appears chaotic vs. structured from outside:** The output is highly structured (canonical XML section order). The transformation from input to output appears well-controlled. The PASS/FAIL distinction is surfaced in the output. The conflict log appears as a comment — visible to me but unmarked in terms of severity.

**What I would conclude is happening without knowing the internal mechanism:** A prompt analysis pipeline that produces XML-structured enhanced output. The output appears reliable because it declares its own verification status. I have no way to verify the verification — the verification was performed by the same agent that produced the output.

**External dependencies I cannot see from outside:** The 3 fixed KB snippets (chain-of-thought, structured-output, self-refine) are embedded in the synthesis spawn prompt. If those KB files were outdated, changed, or corrupted, the output behavior changes — but from outside, the output looks unchanged (same XML structure, same verbatim INVENTORY placement). The KB content degradation is invisible in the output signal.

**Pattern visible from outside that participants cannot see:** The quality of enhancement output depends critically on the INVENTORY — but the INVENTORY is internal only. From outside, two outputs may look structurally identical while having radically different preservation fidelity. The PASS label in the return message conflates "INVENTORY verbatim check passed" with "synthesis quality is high" — these are different claims.

**Limit case from outside:** If the input is a deferred-flag invocation (e.g., `--verbose`), I observe: the skill halts with a warning message and no output. From outside, the system appears to fail. From inside (Frame 1), this is correct behavior. The external failure signal and the internal success signal are identical for deferred-flag halts. An automated test monitoring for output XML would false-positive on every deferred-flag invocation.

**Cross-frame conflict with Frame 3:** From outside, I see only that a file was saved to `~/docs/epiphany/prompts/DD-MM-{slug}.md`. I have no visibility into whether tilde expansion succeeded. Frame 3 (measurement) can detect the file at the absolute path; if tilde expansion failed and the file was written to a literal `~/` path, Frame 2 reports "file saved" (true) while Frame 3 cannot find it at the expected absolute location.

---

### Frame 3 — Measurement-Instrument Frame

*Position: I am the measurement apparatus. I measure "prompt quality improvement" and "preservation fidelity." My act of measuring changes what is being optimized.*

**My precision and resolution limits:**
- "Preservation fidelity" is measurable: INVENTORY item appears verbatim in output — binary, pass/fail per item. Resolution is exact.
- "Prompt quality improvement" is not independently measurable in v1. The only available proxy is: (a) synthesis agent self-declares PASS, or (b) human blind comparison of output vs. `prompt-epiphany` baseline. Both are noisy measurements.
- "Speed improvement over `epiphany-prompt`" is measurable: wall-clock time, two runs on identical input. Resolution: seconds. But the measurement itself consumes at least one additional `epiphany-prompt` invocation — which costs real wall-clock time in the measurement session.

**What my measurement changes:**
If I add automated INVENTORY verbatim checking as an external test (outside the skill), I incentivize the synthesis agent to be conservative about INVENTORY items — placing them verbatim even when more idiomatic integration would be higher quality. The measurement (verbatim presence) is not the same property as "preservation quality" — it is a necessary but not sufficient proxy. Optimizing for the measurement degrades the property being measured.

**What cannot be measured simultaneously:**
- INVENTORY verbatim fidelity vs. synthesis coherence: maximizing verbatim placement can produce XML that is technically compliant but syntactically fragmented (INVENTORY items as disconnected clause inserts). Both cannot simultaneously be maximized.
- Speed vs. quality: cannot run `--minimal` and normal mode on identical input simultaneously. The speed measurement is mode-dependent, but the quality measurement requires normal mode. The speed claim ("faster than `epiphany-prompt`") is valid only if the mode baseline is specified.
- Synthesis reliability vs. observable test behavior: the self-review bias means PASS reports are systematically less reliable than external checks. I cannot measure "actual error rate" vs. "self-reported error rate" without an independent verifier that does not yet exist in v1.

**What my measurement costs the system:**
Each quality evaluation run consumes one additional `epiphany-prompt` invocation (for baseline comparison). This doubles the total compute per comparison test. The measurement overhead is not acknowledged in the design spec.

**Limit case of measurement:** When synthesis agent context window fills to capacity mid-synthesis and the return string is truncated, the `VERIFICATION:` prefix may or may not appear depending on where truncation happens. If truncation removes the prefix entirely, Step 7 routes to the malformed path and the measurement records "malformed return." But the actual synthesis work may have been high quality — it was simply truncated. The measurement cannot distinguish between "synthesis error" and "synthesis success followed by infrastructure truncation."

**Cross-frame conflict with Frame 1:** From inside (Frame 1), the synthesis agent performs INVENTORY verbatim check carefully and honestly. From measurement position (Frame 3), I measure the same check as "self-review bias is a known LLM failure mode" — the same agent checking its own output. Frames 1 and 3 agree that the check runs; they disagree on the reliability of the measurement.

---

### Frame comparison summary

**Cross-frame conflicts:**
- Frame 1 vs. Frame 2: PASS label reliability. F1 reports PASS (agent checked its own output honestly). F2 sees PASS as a quality assurance signal. Neither frame has access to ground truth. This is a **theory collision candidate.**
- Frame 2 vs. Frame 3: File save location. F2 reports "file saved." F3 cannot find it at expected absolute path if tilde expansion failed. This is a **limit case candidate.**
- Frame 1 vs. Frame 3: INVENTORY verbatim measurement reliability. F1 (synthesis agent) administers the check honestly; F3 (measurement instrument) identifies the check as unreliable due to self-review bias. Both are correct simultaneously. This is a **theory collision candidate.**

**Frame-exclusive findings:**
- Frame 1 only: Spawn prompt truncation mid-synthesis is invisible to all other frames — the synthesis agent receives no signal about truncation, and the only downstream effect is a missing `VERIFICATION:` prefix.
- Frame 2 only: From outside, deferred-flag halts are indistinguishable from failures — the external failure signal is identical to an internal success (halt-on-deferred is correct behavior).
- Frame 3 only: The measurement overhead (additional `epiphany-prompt` invocation for quality comparison) is not accounted for in the design. Speed claims are mode-dependent and cannot be cleanly measured without specifying the baseline mode.

**Frame-independent findings (most robust):**
- The Step 5→6 context isolation boundary is the only real isolation in the system. Everything before Step 5 shares context; everything in Step 6 is isolated only as far as the spawn prompt body specifies.
- The INVENTORY verbatim requirement is the load-bearing preservation mechanism. Its reliability is entirely dependent on the synthesis agent's self-check honesty.
- The `VERIFICATION:` prefix is a survival gate — deviation routes to malformed path regardless of XML quality.

</observer_frames>

<imbalances>

## Imbalances

**[IMBALANCE] Orchestrator context accumulation → Synthesis agent spawn prompt assembly (starvation scenario)**

The orchestrator accumulates in-context state across Steps 0–5: the full invocation string, flag detection output, normalized input, 6-dimension analysis (potentially 800–1500 tokens for a rich prompt), INVENTORY YAML, full contract list with anti-conformity additions, conflict log, and spawn prompt assembly reasoning. This is the input reservoir that feeds the synthesis spawn prompt. The synthesis spawn prompt must contain all four content sections plus KB snippets (stated budget: 8,000–15,000 tokens). If the orchestrator's accumulated context has degraded (e.g., earlier analysis blocks have been compressed by context management, or the orchestrator context itself is near its limit), the spawn prompt assembly at Step 5 may silently produce an incomplete spawn prompt even while the four-item checklist returns passing. The checklist checks for presence of sections, not fidelity of their content.

*Genuineness assessment: Structural — inherent to any inline orchestration model where context accumulates before spawn. Not addressable within v1 architecture constraints. Promote to structural finding.*

---

**[IMBALANCE] Synthesis agent token budget → Return message truncation (overflow scenario)**

The synthesis spawn prompt is stated to reach 8,000–15,000 tokens for large inputs. The synthesis agent must consume this input, perform synthesis protocol (5 steps), perform inline verification, and produce a full `VERIFICATION: PASS\n\n<XML>` return. The return message XML may itself be large (structured output for a complex prompt). If the synthesis agent's available output context after consuming the spawn prompt is insufficient for both the synthesis work and a complete XML output, the return message will be truncated. The truncation is silent — the synthesis agent cannot signal "I ran out of output context." The orchestrator receives a truncated string and routes to malformed path. There is no recovery path.

*Genuineness assessment: Structural — inherent to the 1-spawn architecture with no repair loops. Promoted to structural finding. (Advisory downgrade considered but rejected: this failure mode occurs on large inputs in normal mode, which is a defined use case, not an edge case.)*

---

**[IMBALANCE] INVENTORY verbatim requirement → Anti-conformity second pass (asymmetric resource flow)**

The INVENTORY verbatim requirement consumes synthesis agent attention as a preservation-first constraint. The anti-conformity second pass in Step 4 was designed to generate unconventional contracts that primary T1–T13 scanning misses. But the most unconventional contracts are likely to propose structural changes — reordering, collapsing, or introducing new XML sections — that conflict with the verbatim placement mandate for INVENTORY items. The contracts flow from ideation to synthesis, but the preservation mandate limits how many unconventional contracts can be executed without creating INVENTORY placement conflicts. High-unconventionality contracts produced by the anti-conformity pass will disproportionately be skipped or reduced in synthesis, consuming ideation overhead (the second-pass re-read) for minimal net contribution.

*Genuineness assessment: Structural in design intent, but incidental in failure severity — the contract conflict rule already handles this by logging skipped contracts. Downgrade to advisory: the imbalance exists but its output is already handled by the existing conflict log mechanism. Record as advisory.*

---

**[IMBALANCE] Minimal mode analysis → Full INVENTORY schema requirement (asymmetric load)**

Minimal mode reduces analysis to INTENT + INVENTORY only. The INVENTORY full schema requirement is non-negotiable in all modes. For complex prompts, INVENTORY population is the majority of the analysis work — meaning minimal mode does not substantially reduce analysis cost for technically dense inputs (many URLs, paths, code blocks, version strings, named entities). The primary time-save of minimal mode (skipping 4 analysis dimensions) is partly negated by the full INVENTORY requirement on inputs where INVENTORY is large. The user's expectation of "minimal = fast" is not met when the input happens to have high INVENTORY density.

*Genuineness assessment: Structural — the INVENTORY mandate and the minimal mode speed expectation are simultaneous hard requirements that pull in opposite directions for INVENTORY-dense inputs. Promote to structural finding.*

</imbalances>

<singularities>

## Singularities

**[SINGULARITY] at spawn-count = 0: System produces no output**

If the Pre-Spawn Checkpoint (Step 5) fails all four checklist items, no synthesis agent is spawned. The pipeline terminates at Step 5 with a user-facing error. From the user's perspective, the skill consumed analysis and ideation work and produced nothing. There is no partial output, no recoverable artifact, and no path to resume from Step 5 without restarting. This is a hard stop with zero output, which is discontinuously different from the FAIL path (annotated output produced) or the malformed path (raw return displayed). The system has three distinct output regimes (PASS, FAIL, malformed) but the Step 5 failure is a fourth regime (nothing) with no visual similarity to the other three.

*Genuineness assessment: Structural — Step 5 checkpoint failure is a defined system state with no recovery path in v1. Singularity is real.*

---

**[SINGULARITY] at INVENTORY schema completeness = 0: Verbatim preservation undefined**

The INVENTORY verbatim requirement binds on all populated INVENTORY items. If the input is a single-sentence prompt with no URLs, no file paths, no version strings, no code blocks, no named entities, no explicit constraints, and no structural elements, all INVENTORY categories are `[]`. The INVENTORY is "complete" (full schema present, all categories explicitly marked empty). But the verbatim preservation guarantee — the load-bearing quality mechanism — is vacuous: there is nothing to preserve verbatim. In this regime, the synthesis agent's primary quality constraint is eliminated, and the output quality falls entirely to the synthesis agent's intrinsic judgment with no preservation anchor. The system transitions from "preservation-first synthesis" to "unconstrained synthesis" without any signal that this transition has occurred.

*Genuineness assessment: Structural — the empty INVENTORY state is a reachable, non-contrived input condition. The quality contract changes at this boundary without announcement. Singularity is real.*

---

**[SINGULARITY] at context-window-fill → 1.0 during synthesis: Return message prefix undefined**

As discussed in Frame 1 and the synthesis agent overflow imbalance: if the synthesis agent's context window fills to capacity mid-synthesis, the return message is truncated at an arbitrary point. The `VERIFICATION:` prefix appears only at the start of the return message. If truncation removes the entire return message content (synthesis work in progress) and the agent cannot emit even the prefix, the return message is empty or starts with synthesis work-in-progress text. Step 7's routing decision becomes undefined — it routes to malformed, but the observable behavior (no output, no save) is identical to a Step 5 checklist failure. Two distinct failure modes (Step 5 hard stop, Step 6 context overflow) are externally indistinguishable.

*Genuineness assessment: Structural — context window exhaustion is a function of input size, not of implementation quality. The indistinguishability from Step 5 failure is a diagnostic limitation. Singularity is real.*

---

**[SINGULARITY] at flag-position = mid-body: Flag detection behavior switches discontinuously**

The flag position rule specifies: flags valid only as first or last standalone token. A flag mid-sentence in the prompt body is treated as prompt content, not a mode selector. This creates a discontinuous behavioral boundary at token position. A user who writes `Enhance this prompt --minimal carefully` (flag in middle) gets different behavior than `--minimal Enhance this prompt carefully` (flag first) — not a gradual degradation but a binary switch between "mode selected" and "flag treated as body text." If the flag appears in the middle of the invocation and the user expects minimal mode, the system runs normal mode on a prompt that contains `--minimal` as text — producing output that is both slower than expected and contaminated with a literal flag string in the normalized input.

*Genuineness assessment: Structural — the position rule is a hard binary switch, not a soft preference. The contamination (flag string in normalized input → flag string in INVENTORY if applicable → flag string verbatim in output XML) is a consequence of the verbatim preservation mandate interacting with the position rule. Singularity is real.*

</singularities>

<limit_cases>

## Limit cases

**[LIMIT-CASE] at all-parameters-minimum (single-word input, no flags, empty INVENTORY):**
Input: a single word with no context (e.g., "summarize"). Step 2 Sufficiency check should block this — "single words, fragments with no context, or blank inputs fail." *Expected: halt at Step 2 with explanation. Observed: correct halt if sufficiency logic is implemented faithfully. If Step 2 is implemented loosely ("some structure passes"), a single-word input might pass, producing an INVENTORY with all `[]` categories (see SINGULARITY above), an INTENT block of one sentence, and a synthesis agent with no preservation anchors. Output quality is undefined.*

---

**[LIMIT-CASE] at all-parameters-maximum (very large input: 10,000-token prompt, --verbose flag (deferred), maximum INVENTORY density):**
Input: a 10,000-token prompt with 50+ INVENTORY items, passed with `--verbose` flag.
- Flag Detection (Step 0): `--verbose` is deferred → halt with warning. Pipeline stops before Step 1. *Expected: correct halt before any analysis cost is incurred. Observed: correct, if Step 0 is implemented faithfully. The large input is never read into context.*
- If `--verbose` is removed and the large input is passed in normal mode: spawn prompt assembly at Step 5 may exceed practical context limit. Token budget rule kicks in: KB snippets dropped first, then analysis blocks pruned to INTENT + WEAKNESSES only. The synthesis agent receives a degraded spawn prompt. *Expected: graceful degradation with priority-order truncation. Observed: depends on whether Step 5 actually implements prioritized truncation or silently passes a truncated spawn prompt through the four-item checklist (which only checks for presence, not token count).*

---

**[LIMIT-CASE] at one-parameter-minimum (empty contract list, all others nominal):**
Input: a well-formed prompt where 6-dimension analysis produces an INTENT + INVENTORY + TECHNIQUES block showing no gaps and no weaknesses.
- Ideation (Step 4) produces zero contracts (no weaknesses to address, no technique gaps).
- Pre-Spawn Checkpoint (Step 5), item 3: "Contract list non-empty." Checklist FAILS.
- Pipeline halts at Step 5 with user-facing error.
*Expected: checklist failure, no spawn. Observed: the Step 5 singularity — user receives a complete analysis but no output XML. For a "perfect" input prompt, the system produces no enhanced output. This is qualitatively different from nominal behavior and is a usability failure mode: the better the input, the more likely it produces no output.*

---

**[LIMIT-CASE] at system-under-rapid-change (invocation-string switches from Type A to Type B mid-session):**
The input routing is session-stateless — each invocation is independent. The limit case is: a Type C input (prior `prompt-cog` output) fed back into prompt-cog as input.
- Step 1 detects `<meta source="prompt-cog"/>` → Type C → strips `<prompt>` wrapper → normalized input is the inner content of the prior output.
- The inner content is itself structured XML with semantic sections.
- The INVENTORY from the prior run is not re-populated from the stripped content — it is repopulated from analysis of the stripped XML as if it were a plain prompt.
- Analysis (Step 3) runs on XML content that is already enhanced, producing INVENTORY items that include XML tags verbatim (`<task>`, `<role>`, etc.).
- Synthesis agent receives XML tags as INVENTORY verbatim items → embeds them in the output XML, potentially creating malformed XML structure.
*Expected: graceful re-enhancement. Observed: potential XML structure corruption through INVENTORY-verbatim-of-XML-tags. This is a structural interaction between the Type C routing rule and the verbatim preservation mandate.*

---

**[LIMIT-CASE] at system-empty-state (no prior context, first invocation, synthesis agent spawned cold):**
The synthesis agent always starts from empty state (cold spawn) — this is by design. The limit case is: the KB snippets embedded in the spawn prompt are stale or malformed (e.g., `self-refine.md` was edited incorrectly at the time of SKILL.md authoring). The synthesis agent has no way to detect that the KB snippet content is wrong — it received it as authoritative instruction text. *Expected: synthesis proceeds with flawed technique guidance. Observed: quality degradation on the specific technique domain covered by the corrupted snippet. Invisible from all frames except Frame 3 (measurement instrument detects quality regression against baseline).*

</limit_cases>

<theory_collisions>

## Theory collisions

**[COLLISION 1]**
Claim A (C1 — speed is primary): "Must run faster than `epiphany-prompt`. Speed is the primary architectural constraint." The 1-spawn architecture and elimination of repair loops exist specifically to achieve this.
Claim B (C2 — quality floor): "Must not produce lower quality output than `prompt-epiphany` baseline." Inline self-verification (Design Decision #9) is acknowledged as a quality downgrade from epiphany-prompt's separate M4M5 verifier.
Predicted outcome A: The design optimizes aggressively for speed — defers DEEP mode, repair loops, second verifier, session directory — achieving faster execution.
Predicted outcome B: The quality floor requires that inline self-verification be "good enough" — i.e., that the self-review bias does not cause measurable quality regressions compared to `prompt-epiphany` single-pass.
Discriminating condition: Run both `prompt-cog` (normal mode) and `prompt-epiphany` on 20 representative prompts spanning simple, complex, and technically dense inputs. Blind evaluation by a third party (human or automated quality scorer) determines which output is preferred. If `prompt-cog` loses consistently on complex or technically dense inputs (where self-review bias is most likely to fail), the quality floor constraint is violated. If `prompt-cog` wins or ties, the speed–quality trade-off is resolved in favor of the 1-spawn design. The discriminating input class is: long technical prompts (5,000+ tokens with high INVENTORY density), where the synthesis agent's self-review bias has the most opportunity to fail.

*Genuineness: Structural — the speed and quality-floor claims are simultaneously required and pull in opposite directions on the same architectural dimension (synthesis verification). Neither can be softened without changing the architecture.*

---

**[COLLISION 2]**
Claim A (C11 — INVENTORY verbatim mandate): "Every INVENTORY item must appear verbatim in synthesis output." This is a hard constraint, non-negotiable across all modes.
Claim B (C9 + Design Decision #1 — synthesis quality): The synthesis agent's primary goal is to produce a "better-worded prompt" that enhances the input. Enhancement quality includes idiomatic integration, coherent flow, and natural phrasing across sections.
Predicted outcome A: The synthesis agent maximizes verbatim placement — every INVENTORY item appears unchanged, surrounded by enhanced content.
Predicted outcome B: The synthesis agent maximizes enhancement quality — content is reshaped, rephrased, and reorganized for optimal clarity and effectiveness.
Discriminating condition: Submit a prompt where several INVENTORY items are awkwardly phrased (e.g., a version string embedded in a sentence, a URL with descriptive surrounding text). Evaluate the output XML: does the INVENTORY item appear verbatim in the XML (satisfying C11) or does the synthesis agent rephrase the surrounding text and inadvertently shift the INVENTORY item slightly (satisfying enhancement quality at the cost of C11)? The discriminating setup is: INVENTORY items that are verbatim-awkward — their exact preservation makes the output less idiomatic. Either the verbatim constraint wins (rigid, detectable by substring match) or the quality drive wins (smooth output, fails verbatim check). The self-review bias makes the synthesis agent more likely to miss the verbatim failure when it produces a "better-sounding" result.

*Genuineness: Structural — verbatim preservation and output quality are competing objectives at the synthesis step. The synthesis agent must optimize both simultaneously with no tiebreaker rule specified for the case where they conflict at the word level. Merged with COLLISION 1's discriminating condition note: both are resolvable by the same test corpus but require different evaluation dimensions (verbatim substring check vs. blind quality preference).*

---

**[COLLISION 3]**
Claim A (C4 — spawn budget = 1): "Architecture limited to 1 synthesis agent spawn." Exactly one spawn per invocation.
Claim B (anti-conformity second pass in Step 4): The anti-conformity second pass is ported from epiphany-prompt's DEEP M12 sub-step "as in-context second pass, same context." The quality claim for anti-conformity is "+32.9% novelty gains" from independent contrarian re-read.
Predicted outcome A: The 1-spawn constraint holds; no additional spawn for anti-conformity.
Predicted outcome B: The quality gain from anti-conformity (+32.9% novelty) was measured with an independent agent context. In-context re-read in the orchestrator's accumulated context (already containing analysis, contracts, and role framings) is not the same cognitive operation as an isolated contrarian agent. The novelty gain may be significantly lower — or zero — when the re-read happens in a contaminated context.
Discriminating condition: Run normal-mode `prompt-cog` on 10 prompts and evaluate whether the anti-conformity contracts (Step 4 additional contracts flagged as from the second pass) are consistently distinguishable from primary contracts in novelty and unconventionality. Separately, run `epiphany-prompt` DEEP mode on the same 10 prompts and compare the M12 additional contracts to `prompt-cog`'s second-pass additions. If `prompt-cog`'s anti-conformity additions are qualitatively indistinct from primary contracts (i.e., no novelty differential), the "+32.9% novelty gains" claim does not transfer to the inline implementation. The constraint is: measure novelty differential between primary and second-pass contracts, not overall output quality.

*Genuineness: Structural — the quality claim for anti-conformity was made in the context of an isolated agent; porting it inline changes the cognitive operation. Whether the gain survives context contamination is a structural question about LLM in-context vs. isolated reasoning, not an implementation detail.*

---

**Deduplication review:** Collisions 1 and 2 both involve the synthesis agent's self-review and share the same quality-measurement setup (blind evaluation on a test corpus). However, their discriminating conditions are distinct: Collision 1 measures quality preference (subjective); Collision 2 measures verbatim substring presence (objective). These are different observables with different experimental setups. No merge applied.

</theory_collisions>

</dynamic_simulation>
