# Node D — Accepted Improvements
Each idea from Node C evaluated against: utility, relevance, quality gain vs. complexity cost, non-overengineering.

---

## Evaluation Criteria
- **Accept** if: clear quality gain, no additional spawn, no architectural contradiction, implementable as spec text
- **Accept-Conditional** if: quality gain is real but requires careful scoping to avoid overengineering
- **Reject** if: duplicates existing spec content, requires new spawn, is speculative without clear gain, or belongs in tests not spec

---

## Accepted

### D01 — Role-as-Channel-Code structural headers [from I03 + I14 merged]
**Verdict:** ACCEPT
**Utility:** HIGH — directly addresses CS-2 (contamination encoded at spawn boundary) and CS-4 (spawn prompt construction is highest-leverage quality gate), both STRONG confidence in Node B
**Gain:** Synthesis spawn assembler can extract analyst/ideation outputs by structural address instead of prose parsing. Reduces contamination noise in spawn prompt body.
**No added spawn:** Yes — purely structural text protocol
**Target sections in spec:** Step 3, Step 4 (add structural markers), Step 5 (add assembly extraction instruction + checklist item)
**Format decided:** `=== ANALYST OUTPUT BEGIN ===` / `=== ANALYST OUTPUT END ===`, `=== IDEATION OUTPUT BEGIN ===` / `=== IDEATION OUTPUT END ===`

### D02 — INVENTORY placement verification [from I04]
**Verdict:** ACCEPT
**Utility:** HIGH — closes H5 placement-context gap; substring presence passes even when item is in a wrong XML section
**Gain:** Synthesis verification becomes placement-aware, catching a failure class (misplaced-but-verbatim items) that current spec allows
**No added spawn:** Yes — additional instruction in Step 6 verification
**Target sections:** Step 6 synthesis protocol, step 5 (inline verification), add placement mapping

### D03 — Anti-conformity novelty gate [from I05]
**Verdict:** ACCEPT
**Utility:** MEDIUM-HIGH — addresses C3 and Probe 1; prevents anti-conformity pass from producing primary-pass reconfirmations
**Gain:** Each anti-conformity contract includes explicit primary-exclusion rationale. Makes the pass more discriminating and accountable.
**No added spawn:** Yes — additional instruction in Step 4
**Target sections:** Step 4 anti-conformity second pass

### D04 — Complexity detection advisory [from I09 + I28 coordination]
**Verdict:** ACCEPT
**Utility:** MEDIUM — addresses H2; users running complex inputs get a runtime signal instead of silent quality degradation
**Gain:** Advisory in Step 2 prevents users from treating output as reliable when input exceeds quality floor scope
**No added spawn:** Yes — lightweight count heuristic + advisory message
**Target sections:** Step 2 (Announce + Sufficiency), coordinate with minimal mode

### D05 — INVENTORY anti-paraphrase explicit instruction [from I13]
**Verdict:** ACCEPT
**Utility:** HIGH — addresses C2 directly; "verbatim" is currently underspecified, creating self-review bias vector
**Gain:** Synthesis agent receives explicit character-level fidelity instruction; the "verbatim" contract is not ambiguous
**No added spawn:** Yes — one instruction line in Step 6 spawn prompt body
**Target sections:** Step 6 synthesis spawn prompt content

### D06 — Type D agentic workflow detection [from I07]
**Verdict:** ACCEPT-CONDITIONAL
**Condition:** Detection must be a heuristic advisory only — not a blocker. The spec's Out of Scope section already acknowledges this class; this adds a runtime signal, not a new guarantee.
**Utility:** MEDIUM — addresses Counter-example 3; gives users a runtime signal when their input is in the degraded class
**Gain:** User warned before synthesis on inputs where Hard Gate 3 is unreliable
**No added spawn:** Yes — pattern detection in Step 1 + advisory in Step 2
**Target sections:** Step 1 (Input Routing) — add Type D tag; Step 2 — add advisory message

### D07 — Same-target contract conflict detection [from I08 + I27 consolidation]
**Verdict:** ACCEPT
**Utility:** MEDIUM — addresses Bridge 5; prevents silent synthesis errors from conflicting contracts on the same target
**Gain:** Internal contract conflicts are surfaced before spawn, not discovered in synthesis output
**No added spawn:** Yes — scan instruction in Step 4 contract finalization
**Target sections:** Step 4 — add contract finalization block with conflict scan

### D08 — Quality floor expected-value language [from I11]
**Verdict:** ACCEPT
**Utility:** MEDIUM — addresses H3; removes false absolutism from Design Constraints wording
**Gain:** Implementers and users reading the spec don't misinterpret the constraint as a per-invocation guarantee
**No added spawn:** Yes — one-line wording change
**Target sections:** Design Constraints table

### D09 — VERIFICATION:FAIL recovery guidance [from I12]
**Verdict:** ACCEPT
**Utility:** MEDIUM — addresses user experience gap on FAIL path; currently there's no actionable next step
**Gain:** User has clear recovery options after a synthesis failure, reducing dead-end frustration
**No added spawn:** Yes — informational text in Step 7 FAIL path
**Target sections:** Step 7 (Output) — FAIL path

### D10 — Role transition deactivation line [from I16]
**Verdict:** ACCEPT
**Utility:** LOW-MEDIUM — minor inline quality improvement with zero cost
**Gain:** Explicit deactivation line between Steps 3 and 4 creates cognitive boundary within inline context, reducing role bleed
**No added spawn:** Yes — one instruction line
**Target sections:** Step 4 preamble (before role switch)

### D11 — Step 5 Interface 2 coherence strengthening [from I18]
**Verdict:** ACCEPT
**Utility:** MEDIUM — strengthens existing Step 5 item 5 from presence-only to relevance-required
**Gain:** Closes the gap where a mapped contract passes the checklist but uses an irrelevant technique
**No added spawn:** Yes — instruction strengthening
**Target sections:** Step 5 checklist item 5

### D12 — Minimal mode transparency advisory [from I19 + I28 coordination]
**Verdict:** ACCEPT
**Utility:** LOW-MEDIUM — informs users what minimal mode gives up
**Gain:** User knows the mode trade-off at invocation time, not after reviewing thin output
**No added spawn:** Yes — advisory text in Step 2
**Target sections:** Step 2 announce block (minimal mode announce path)

### D13 — Flag disambiguation for prose-like --token [from I24]
**Verdict:** ACCEPT
**Utility:** LOW-MEDIUM — prevents Step 0 from halting on prompts that start with `--` as prose
**Gain:** Edge case that currently produces a spurious halt is handled gracefully
**No added spawn:** Yes — disambiguation rule in Step 0
**Target sections:** Step 0 (Flag Detection) — add disambiguation rule

### D14 — Inline quality checkpointing post-analysis [from I26]
**Verdict:** ACCEPT-CONDITIONAL
**Condition:** Must NOT add a blocking re-run loop by default. Implement as a self-assessment gate: "note any thin analysis blocks before proceeding" — informational, not architectural. Full re-run loop deferred to v2.
**Utility:** MEDIUM — pairs with D01 (Role-as-Channel-Code) to create an inline completeness check without adding a Step
**Gain:** Thin analysis is flagged in context before ideation amplifies the error
**No added spawn:** Yes — inline self-check between Steps 3 and 4
**Target sections:** Between Step 3 and Step 4 (within Step 3 output block)

---

## Rejected

### R01 [from I01] — Promote DD#14 to DD#1 (structural reordering)
**Verdict:** REJECT
**Reason:** DD#14 already synthesizes the mechanical/judgment boundary as a first-class principle. Its position at #14 reflects authoring history. Renumbering all design decisions creates churn for no quality gain; readers can follow cross-references. Node B's headline predates DD#14 existing.

### R02 [from I02] — Add spawn prompt assembly protocol (general)
**Verdict:** REJECT (superseded by D01)
**Reason:** D01 (Role-as-Channel-Code headers) IS the spawn prompt assembly protocol. Adding a generic "assembly protocol" on top would duplicate D01.

### R03 [from I06] — Context reset preamble on synthesis spawn
**Verdict:** REJECT
**Reason:** The synthesis spawn is already a fresh agent — it has no prior state. A context-reset preamble addresses a non-problem (prior context bleed into a new agent). The contamination issue is content-encoded, not state-carried. D01 addresses the actual problem.

### R04 [from I10] — Large input advisory in Step 2
**Verdict:** REJECT (absorbed into D04)
**Reason:** D04 (complexity detection advisory) already covers this case — high INVENTORY density correlates with large input size. Two separate advisories in Step 2 create noise. D04 addresses the quality risk; the token budget note in Step 6 already documents the truncation behavior.

### R05 [from I15] — Analysis quality gate with re-run conditional
**Verdict:** REJECT
**Reason:** A blocking re-run conditional adds architectural complexity and latency — contrary to v1 constraints. D14 (inline self-check, informational) achieves the same signal without the re-run. Full quality gate with re-run is a v2 candidate.

### R06 [from I17] — Add success_criterion field to contract schema v1
**Verdict:** REJECT
**Reason:** The contract schema v1 is intentionally minimal. Adding a success_criterion field is a schema change that requires updating synthesis protocol, pre-spawn checklist, and verification step. Benefits don't outweigh the surface expansion in v1. Candidate for v2 enhancement.

### R07 [from I20] — Sufficiency check note for low-INVENTORY inputs
**Verdict:** REJECT
**Reason:** The sufficiency check already says "An input with some structure (even a rough draft) passes." This is not a gap — the concern (low-INVENTORY ≠ insufficient) is already implicit. Adding a note creates redundancy.

### R08 [from I21] — Collapse 6-dimension analysis to 4 dimensions
**Verdict:** REJECT
**Reason:** Architectural change contradicting an explicit design decision to PORT the 6-dimension analysis AS-IS. STRUCTURE and CONSTRAINTS blocks have distinct purposes; collapsing them would drop signal that guides ideation. Overengineering in the wrong direction.

### R09 [from I22] — Add second synthesis spawn for verification
**Verdict:** REJECT
**Reason:** Directly contradicts the 1-spawn constraint (Design Decision #1). Speed superiority is architecture-defining. This is a v2 DEEP mode feature.

### R10 [from I23] — Failure log on VERIFICATION:FAIL
**Verdict:** REJECT
**Reason:** Introduces filesystem side effect on failure path. Claude Code doesn't inherently have log file management. D09 (recovery guidance) already improves the FAIL path. A failure log belongs in a monitoring layer outside the skill spec.

### R11 [from I25] — Output format sensitivity (non-XML inputs)
**Verdict:** REJECT
**Reason:** The output is ALWAYS XML with `<meta source="prompt-cog"/>`. This is a core contract, not a preference. Non-XML inputs are normalized and enhanced into XML — this is by design, not a gap. Changing this would break the downstream Type C routing rule.

---

## Summary
- **Accepted (hard):** D01, D02, D03, D05, D07, D08, D09, D10, D11, D13
- **Accepted (conditional):** D04, D06, D12, D14
- **Rejected:** R01–R11 (11 ideas)
- **Total accepted:** 14 improvements to integrate into final spec
