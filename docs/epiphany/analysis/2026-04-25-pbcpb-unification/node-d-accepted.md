# Node D — Accepted Ideas (with Pro/Con/Utility Filter)

> Single-pass verdict over every Node C entry. Verdict ∈ {ACCEPT, ACCEPT-CONDITIONAL, REJECT}.
> Filter criteria (verbatim from spec): "considering overall utility and relevancy, and if it will create a better product without excessive overengineering or complexity, or resolves any potential issues, we want the best product features that are usable for humans, must be realistic to implement without overcomplexity or regression."

---

## D01 — Discriminated-union return type for `populate()`
**from_idea_id:** I01
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Closes H-CONS1 (confidence 0.88 — the highest-confidence supported hypothesis in Node B). Eliminates the silent-mishandle failure mode for RAG_MCP. Type-distinguishes manifest-emission from write-status without breaking the 3-method contract.
**target_section:** `<kb_scope>` interface contract (lines ~73–79)

## D02 — `AdapterSession.bind(config_path)` two-phase initialization
**from_idea_id:** [I02, I15]
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Single highest-leverage architectural improvement. Closes Headline Insight gap #1 (config validation gate) and structurally enforces caller-invariance (GC-2). Catches all config errors at startup. Mirrors TLS / JDBC / Spring-DI patterns. I15 subsumes I02; merging consolidates them. + Specifies validation-at-startup behavior so an invalid `adapter_type` value never reaches the dispatch layer.
**target_section:** `<task>` Phase 2 (new step 2.9) + `<kb_scope>` interface contract preamble + `<operational_constraints>`

## D03 — Cycle detection + Kosaraju SCC for tier scoring
**from_idea_id:** I03
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Closes H-CONS2 (confidence 0.85). Bootstrap is non-functional on cyclic master-index input without this fix. Algorithmic correctness change with bounded scope (Phase 5 STEP 2 only).
**target_section:** `<task>` Phase 5 STEP 2 (tier_scoring)

## D04 — EC-7: PBCPB-internal failure handling
**from_idea_id:** I04
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Closes H-CONS3 (confidence 0.80). Fills the symmetric edge-case slot — user-owned-boundary failures already documented (EC-2, EC-6); internal failures previously undefined. Production-risk closure.
**target_section:** `<edge_cases>` (after EC-6)

## D05 — gap_id namespace coherence rule
**from_idea_id:** I05
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Closes the weakest link in the bootstrap causal chain (DF-10) and partially closes the Leaky Abstraction concern (DF-8). Adapter contract becomes self-validating against `schema_definition`. Makes downstream consumers adapter-agnostic.
**target_section:** `<kb_scope>` adapter interface contract + `<task>` Phase 5 STEP 1

## D06 — Document PBCPB's dual role (framework + executor)
**from_idea_id:** I06
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Reduces reader confusion when JSON_DB and CUSTOM_API behave differently from MARKDOWN_FOLDER. Reflects the H-CONS4 partial-falsification result (PBCPB is simultaneously framework and executor). Documentation-only; zero implementation cost.
**target_section:** `<context><background>`

## D07 — Out-of-scope guardrails sub-block
**from_idea_id:** [I07, I18, I21]
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Consolidates four scope-limit-guardrail-only items: (DF-1 transactions), (DF-2 adapter-specific operations), (DF-4 stateful sessions), (DF-5 multi-KB join), (DF-3 agent lifecycle), (DF-6 migration cutover). All are guardrails (state limit; provide no primitive). Single sub-block prevents scattered prose. Lightweight; no implementation regression.
**target_section:** New "Out of scope" sub-block in `<operational_constraints>` (or as a new sibling block `<out_of_scope>`)

## D08 — Strengthen verification item 12 to require explicit pseudo-value rejection
**from_idea_id:** I08
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Verification item 12 currently says "HUMAN_ONLY is not a valid enum value" but does not require active rejection. Strengthening makes the parser the gatekeeper, not just documentation. Same pattern extended to adapter_type. Closes a regression-risk surface.
**target_section:** `<verification>` item 12

## D09 — CALLER_INVARIANCE constraint (explicit)
**from_idea_id:** I09
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Makes GC-2 generalization-precondition explicit and statically enforceable. Lint/import-graph rule is achievable in CI. Prevents silent regression where a refactor leaks backend-specific imports into business logic.
**target_section:** `<constraints>` table (new row CALLER_INVARIANCE)

## D10 — Phase 1.7: cyclic-master-index detection
**from_idea_id:** I10
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Diagnostic feed-forward into D03's algorithmic fix. Phase 1 already explores both codebases; adding cycle detection on the existing master-index is a natural extension. Output (cyclic? where?) feeds Phase 5 algorithm selection. Closes OQ-2 production-risk probe.
**target_section:** `<task>` Phase 1 (new step 1.7)

## D11 — Phase 1.3: explicit feature inventory requirement
**from_idea_id:** I11
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Closes Probe 6 — Phase 4.6 validation is structurally non-executable without a feature inventory. Strengthening Phase 1.3 is the cheapest fix; defers the actual inventory work to the agent at runtime.
**target_section:** `<task>` Phase 1.3

## D12 — Verification items 13 and 14 (invalid enum, cyclic schema)
**from_idea_id:** I12
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Brings verification block into alignment with the constructive 8-step spec from N5. Adds runtime-testable coverage for D02 (invalid enum caught at startup) and D03 (cyclic schema handled).
**target_section:** `<verification>` block (append items 13, 14)

## D13 — Deferred-backlog idempotency and re-run semantics
**from_idea_id:** I13
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Closes A1 Gap #6. Re-running Phase 5 with a higher tier_limit is mentioned in Node A as the path to "detailed content population" but never specified. Idempotency is the correct default; explicit specification prevents implementer ambiguity.
**target_section:** `<task>` Phase 5 STEP 4 + STEP 5

## D14 — STATELESS_INTERFACE constraint (explicit)
**from_idea_id:** I14
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Makes the implicit invariant explicit. Pairs with D07 scope-limit guardrails. Adapter implementers are unambiguously instructed where to encapsulate session state.
**target_section:** `<constraints>` table (new row STATELESS_INTERFACE)

## D15 — Reference architectures in System Overview
**from_idea_id:** I16
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Two-line orientation per analog (POSIX fd, JDBC, TLS, Spring-DI) accelerates reader comprehension. No implementation cost. CS1/CS2/CS3 cross-domain recurrence is well-supported and worth surfacing.
**target_section:** `<output_format>` Section 1 (System Overview)

## D16 — Reframe `<background>` to lead with the unification problem
**from_idea_id:** I17
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** Improves opening-sentence clarity — the goal precedes the inventory. Three-paragraph restructure of `<background>` only; no semantic loss.
**target_section:** `<context><background>`

## D17 — Tier 0 cross-cutting concerns (opt-in)
**from_idea_id:** I19
**verdict:** ACCEPT-CONDITIONAL
**utility:** LOW-MEDIUM
**gain:** Generalizes the dependency-first principle to cross-cutting concerns (glossary, error-code catalog). Recognizes that some entries are pre-foundational. **Conditional:** ship behind an opt-in config flag `bootstrap_tier_zero_threshold` (default: disabled / 0); no regression unless enabled. Without the conditional, this is mild overengineering — the standard Tier 1 ≥2-dependents rule already captures most cases. With the conditional flag, it's a low-risk, high-value generalization.
**target_section:** `<task>` Phase 5 STEP 2 + `<kb_scope>` user-config schema (`bootstrap_tier_zero_threshold` field)
**condition:** Implementation MUST be behind an opt-in `bootstrap_tier_zero_threshold` config field (default 0 = disabled). Default behavior is unchanged from current Tier 1 logic.

## D18 — Single-config-file invariant
**from_idea_id:** I20
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Stabilizes the no-rebuild-required guarantee on a known config-file location. Names a default and an override env var. Forbids multi-file composition (which would break the runtime-switch promise in subtle ways).
**target_section:** `<constraints>` table (extend RUNTIME_CONFIG)

## D19 — Per-adapter `kb_adapter` config snippets
**from_idea_id:** I22
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Provides four concrete examples (one per adapter_type) where Node A currently has only the abstract schema. Becomes the doc-snippet examples Section 2 of `<output_format>` requires. Reduces implementer guesswork.
**target_section:** `<kb_scope>` (after the schema block)

## D20 — Phase 5 dotted-decimal sub-step renumbering
**from_idea_id:** I23
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** Cosmetic but valuable: aligns Phase 5's "STEP 1/2/3/4/5" to "5.1/5.2/5.3/5.4/5.5" matching all other phases. Improves cross-reference precision throughout the prompt.
**target_section:** `<task>` Phase 5 step headers

## D21 — Glossary block for high-leverage domain terms
**from_idea_id:** I24
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Six-row glossary at the top of `<context>`. Defines the highest-leverage terms once; downstream prose can reference without re-defining. Improves first-time reader experience without semantic redundancy.
**target_section:** New sub-block `<glossary>` at top of `<context>`

## D22 — Verification items cross-referenced to constraints
**from_idea_id:** I25
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** Parenthetical constraint reference per verification item. Improves bidirectional traceability for maintenance. Lightweight; no semantic change.
**target_section:** `<verification>` block

## D23 — Dedicated `<dify_constraint>` sub-block
**from_idea_id:** I26
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Lifts the Dify upload constraint into a structured anchor. Pairs naturally with D01's discriminated-union return type. Reduces the chance the constraint is missed despite being the highest-confidence design risk (H-CONS1, 0.88).
**target_section:** `<context>` (new sub-block `<dify_constraint>` after `<background>`, before `<kb_scope>`)

## D24 — Phase prerequisites annotation
**from_idea_id:** I27
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** "Prerequisite:" line per phase. Phase 4 already has it implicitly; standardize across all phases. Supports automated phase-gate checks. Lightweight.
**target_section:** `<task>` each phase opening

## D25 — Adapter return-type schemas in interface contract
**from_idea_id:** I28
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Closes A1 Gap #2. Hoists `scan_gaps`'s gap-list shape from Phase 5 STEP 1 into the interface contract; references it from Phase 5. Adds shapes for `query()` and `populate()` (D01 already specifies populate; this consolidates). Single source of truth for return shapes.
**target_section:** `<kb_scope>` (after interface contract method list)

## D26 — Verification ordering by phase
**from_idea_id:** I29
**verdict:** REJECT
**rejection_reason:** Pure cosmetic reordering; the items themselves are unchanged. The cross-reference improvement from D22 (constraint references in parens) provides equivalent navigability without reshuffling. Reordering also risks renumbering churn that breaks any external reference to verification item numbers. Marginal benefit, non-zero risk.

---

## Rejected

### REJ-1 — I29 (verification reordering)
See D26 above.

---

## Merge Consolidation Sub-Pass

Scanning accepted entries for merge candidates (same target_section AND one entry's idea is subsumed by or reinforces the other):

- **D02 already merged from [I02, I15]** during the verdict pass (I15 structurally subsumed I02).
- **D07 already merged from [I07, I18, I21]** during the verdict pass (all three are scope-limit guardrails with the same conceptual home).
- **D03 + D10:** Same conceptual chain (cycle handling) but different target_section — D03 is `<task>` Phase 5 STEP 2; D10 is `<task>` Phase 1 (new step 1.7). Different phases; do not merge.
- **D01 + D23:** Both touch the Dify upload behavior but at different levels — D01 is the interface contract return type; D23 is a documentation anchor block. Different target_section; do not merge.
- **D08 + D22 + D12:** All touch `<verification>` but at different specific items (D08 = item 12 strengthening; D22 = constraint cross-references on all items; D12 = new items 13 and 14). Different specific items within the same block; do not merge — they will combine cleanly at integration.
- **D14 + D07:** D14 is the explicit STATELESS_INTERFACE constraint; D07 is the out-of-scope guardrails sub-block. The stateless invariant is referenced by D07 (DF-4) but D14 places it as a positive constraint (not a guardrail). Distinct; do not merge.

No further merges identified.

---

**Filter summary:** 29 ideas in → 25 ACCEPT (1 ACCEPT-CONDITIONAL = D17) + 1 REJECT (D26) + 3 already merged at verdict time (I02, I18, I21 absorbed into D02 and D07).
**Counts:** ideas_in = 29; accepted_out = 25.
