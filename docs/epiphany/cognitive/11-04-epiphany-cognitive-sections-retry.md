> v1.2.0 · Scale: STANDARD · Pipeline: complete at 6 of 6 planned stages · For raw state, re-invoke with --xml

## Headline Insight
Both issues share the same underlying design failure: **automatic, hidden behaviors that cannot be observed or tuned.** The section hard-coding buries a maintenance list across 4+ locations with no canonical source; the retry-once policy buries a policy decision with no rationale or user control. The unified fix: make both explicit and user-accessible.
**Confidence:** HIGH

## Theory Collisions
*(Stage S4 skipped at STANDARD scale — run DEEP for this section.)*

## Discovery vs. Proof

| Discovered | Proved |
|---|---|
| Section Registry pattern (from database normalization, legal drafting, CSS) resolves multi-touch maintenance cost | Three domains converge on the pattern; LLM execution reliability under registry is **untested** — the bridge holds for human maintainers, is probabilistic for LLMs |
| User-triggered retry (`--fix-distillation`) eliminates the untested-assumption problem by transferring control to the user | The pattern eliminates the *policy question*, not because it's proven optimal but because it makes "retry count" a user choice rather than an undocumented axiom |
| "10 sections" count appears in **4+ locations**, not 3 — Output Flow description, Changelog, Synthesis Step 2, and Verification Step 3a | A search pass is required to confirm the exact count; Discovery, not Proof |
| Both issues generalize: hybrid registry and user-triggered retry each apply to any prompt spec with similar structure | Generalization holds for both; neither solution is accidentally-right for only these two instances |

## Independence-Verified Bridges

**Bridge 1 — Hybrid Section Registry** (database normalization / legal drafting / CSS → prompt spec design)
- Structural invariant: {one canonical definition → N reference sites that point rather than re-enumerate}
- Disanalogy limit: code enforces pointer dereference; a prompt spec can only *instruct* the LLM to consult the registry. The bridge holds for human editors; it is probabilistic for LLM execution.
- Independence note: all three supporting domains share a common design-philosophy root (single-source-of-truth). Not fully independent — confidence rated MODERATE, not HIGH.

**Bridge 2 — Failed (reported for honesty):** Configurable `--distillation-retries N` flag
- Failure reason: adding this flag touches the Trigger Table, Edge Cases table, and Output Flow — the *same multi-touch problem* the flag was intended to solve. The bridge introduces more maintenance surface than it eliminates.

## Alternative Hypotheses

**AH1 — Hybrid registry (recommended for Issue 1):** Create one canonical `## Section Registry` block as the single human-editable source of truth. Retain local enumeration inside Synthesis Step 2 protocol for LLM execution reliability. Replace all count-references ("10 sections", "10 distilled sections") with "N sections (see Section Registry)."
Discriminating observation: run synthesis with vs. without local enumeration alongside the registry. If local enumeration maintains section-completeness, AH1 is correct. If registry lookup alone is reliable, full registry suffices.

**AH2 — User-triggered retry (recommended for Issue 2):** Replace automatic retry-once with: (a) emit `distillation_warning` immediately on first failure naming the specific failing check; (b) add `--fix-distillation` flag for user-triggered targeted retry of synthesis.
Discriminating observation: if users rarely invoke `--fix-distillation`, failure rate was low and retry-once was excessive token waste; if invoked frequently, failure rate is high and retry-once was insufficient. Either outcome produces the data the current spec lacks.

**AH3 — Meta-verification pre-pass (considered, rejected):** A spec-consistency check before execution to verify all enumerations are aligned. Rejected: adds complexity without addressing either root cause — detects inconsistency but neither prevents it nor fixes the retry policy.

Hypotheses eliminated by falsification:
- Pure `--distillation-retries N` flag: creates the same multi-touch problem it addresses.
- Full Section Registry with no local enumeration: eliminated by the C1 contradiction — attention-decay in an 8000-word spec may reduce LLM section-completeness relative to local enumeration.

## Density-Checked Falsification

**Issue 1 (Section Registry):** 3 supporting analogies (compiler science, legal drafting, CSS). 1 disconfirming argument (LLM attention-decay). The disconfirming argument is *mechanistically targeted* — it attacks the specific mechanism the solution depends on. Residual uncertainty: MODERATE — the hybrid approach mitigates but does not eliminate the concern.

**Issue 2 (Retry policy):** 0 supporting analogies for retry-once specifically. 1 counterargument against configurable flag. AH2 is not falsified but is also not strongly confirmed — it eliminates the untested assumption by converting policy to user action. `1 falsification vector attempted for AH2 — density low — residual uncertainty moderate`.

## Scope Limits (hypotheses non fingo)

**Applies to:** The epiphany-cognitive skill v1.2.0 Output Synthesis Pass architecture — specifically the hard-coded section list and the retry-once distillation policy.
**Does not extend to:** LLM execution behavior under the Section Registry pattern (requires empirical testing, not derivable from structural analysis); other skills with similar patterns (the principles likely transfer but were not tested against them).
**Claims refused:** That retry-once is demonstrably wrong — no usage data either way; that the Section Registry will reliably improve LLM execution quality — LLM attention behavior over long specs is empirically unknown without testing.

## Coherence Signals

Both recommended solutions share the structural form: **move control from automatic/hidden to explicit/user-visible.** Section Registry makes the canonical list visible to human editors. `--fix-distillation` makes retry control visible to users. This coherence across two independent issues suggests a broader design principle for this skill: *hidden automation is technical debt; explicit surface area is maintainable surface area.* Reported as supporting signal, not proof.

## Generalization Checks

**Hybrid registry pattern:** Generalizes to any prompt spec with a list that must be both (a) maintained by humans with low touch-cost and (b) reliably enumerated by LLMs at execution time. Tested: this skill's section list. Breaks: very short specs (< 1000 words) where attention-decay concern is negligible — full registry lookup is sufficient there, hybrid overhead unnecessary.

**User-triggered retry pattern:** Generalizes to any automatic retry in any prompt spec where optimal retry count is unknown and the failure condition is diagnosable by name. Tested: this skill's distillation verification. Breaks: automated/scripted invocation contexts where no human is present to invoke `--fix-distillation`. Specified fallback: in automated contexts, restore retry-once behavior.

## Open Questions & Next Probes

Ranked by discriminating power per unit effort:

1. **Attention-decay test** *(highest value, ~5 invocations):* Invoke with a Section Registry in place, 5 runs, verify synthesis correctly enumerates all sections. Resolves whether the hybrid is necessary or full registry lookup is reliable enough.

2. **Registry block placement** *(1 editorial decision, 2 verification runs):* Near the Output Synthesis Pass section (~5000 words in) for context-relevance, or at the top for visibility. Run synthesis from each placement; check section completeness.

3. **`--fix-distillation` flag conflict audit** *(1 spec-review pass, ~15 min):* Check `--fix-distillation` + `--xml`, `--fix-distillation` + `--quiet`, `--fix-distillation` + depth flags for trigger-table conflicts.

4. **Automated-context fallback specification** *(1 edge case entry):* If `--fix-distillation` is added, specify behavior when no TTY is present: "restore retry-once behavior in automated contexts."
