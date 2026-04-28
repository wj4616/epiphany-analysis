# Node E — Engineered Solutions (two-drafts-pick-best)

> Each entry: 2 independent drafts → §6 Level A verify → compare on (pro/con, utility, over-engineering bounded, info-preservation) → pick best.
> Tiebreaker: smaller surface area (fewest lines changed in A).

---

## E01 — Discriminated-union return type for `populate()`
**from_d_id:** D01
**from_idea_id:** I01
**target:** `{"kind": "verbatim_block", "text": "      query(query_string, filters?)       — Retrieve KB content matching a query.\n      populate(content, tier, metadata)   — Write or upload content into the KB.\n      scan_gaps(schema_definition)        — Scan KB for gaps/placeholders; return gap list."}`
**action:** replace
**text:** |
      query(query_string, filters?)       — Retrieve KB content matching a query.
                                            Return: list of result objects, each
                                            { entry_id, content, metadata, source_adapter }.
      populate(content, tier, metadata)   — Write content to the KB OR emit a manifest.
                                            Return is a discriminated union:
                                              { kind: "write_status", success: bool,
                                                written_count: int, errors: [] }
                                                — for write-capable adapters
                                                  (MARKDOWN_FOLDER, JSON_DB, CUSTOM_API)
                                              { kind: "manifest", manifest_path: str,
                                                items: [{ entry_id, target_location,
                                                          payload }, ...] }
                                                — for RAG_MCP (no direct Dify write)
                                            Callers MUST switch on `kind` before
                                            consuming the rest of the payload. Treating
                                            both branches uniformly is a behavioral bug.
      scan_gaps(schema_definition)        — Scan KB for gaps/placeholders; return gap list.
                                            Return: list of gap objects, each
                                            { gap_id, location, type, schema_node }.
                                            See gap_id namespace coherence rule below.
**rationale:** Closes H-CONS1 (confidence 0.88). Discriminated union is the lightweight, 3-method-preserving fix. Documents both branches and explicitly forbids uniform treatment. Pulls return shapes for query and scan_gaps inline (folds in D25 partial scope at the same site to avoid two-edit churn).
**compare_notes:** Draft 2 used a tagged-union via subtype names (e.g. `WriteStatus | UploadManifest`) presented as two separate type definitions outside the method block. Pros: cleaner type-language separation. Cons: increased surface area (additional definitions block) and required a forward reference from the method line to the type definitions, hurting one-pass readability. Draft 1 (chosen) is fewer lines and reads top-to-bottom in place.

---

## E02 — `AdapterSession.bind(config_path)` two-phase initialization
**from_d_id:** D02
**from_idea_id:** [I02, I15]
**target:** `{"kind": "anchor", "before": "  2.7 Define user-configuration schema (kb_adapter block with skill_ref field).", "after": "  2.8 KB backend selection is a runtime config decision"}`
**action:** insert-after
**text:** |2

  2.7a Define `AdapterSession` as the public, business-logic-facing handle. The
       only construction path is `AdapterSession.bind(config_path) → AdapterSession`,
       which (a) loads `kb_adapter` and `role_system` config from the canonical config
       file, (b) validates both blocks against their schemas — including exact-string
       enum values — (c) instantiates the concrete adapter for the configured
       `adapter_type`, and (d) returns a session handle exposing only `query`,
       `populate`, `scan_gaps`. On any validation failure raise a structured
       `ConfigError` describing the offending field; the dispatch layer is
       unreachable until validation passes.
  2.7b Business-logic code MUST hold an `AdapterSession` reference, never a
       concrete-adapter reference. All KB calls go through the session handle's
       three-method surface.
**rationale:** Closes Headline Insight gap #1 (config validation gate before any backend selection) and structurally enforces caller-invariance (CALLER_INVARIANCE — see E09). Mirrors TLS handshake / JDBC `getConnection()` / Spring-DI container `bind` patterns confirmed across 4 nodes (CS1, CS2, CS3, IB-6). Inserts into Phase 2 immediately after the user-config-schema step so design-time still precedes Phase 4 implementation.
**compare_notes:** Draft 2 added a new top-level `<adapter_session>` sub-block in `<context>` rather than a Phase 2 step. Pros: prominent visibility. Cons: split the design directive across two blocks (description in `<context>`, implementation in `<task>`) — fragile under prompt edits. Draft 1 (chosen) keeps the directive in Phase 2 where the agent actually executes the design step.

---

## E03 — Cycle detection + Kosaraju SCC for tier scoring
**from_d_id:** D03
**from_idea_id:** I03
**target:** `{"kind": "verbatim_block", "text": "    STEP 2 — tier_scoring:\n      Score each gap using these criteria:\n        Tier 1 (core-necessary): gap_id is referenced by ≥2 other entries in schema_definition,\n                                 OR the schema_definition marks it required: true,\n                                 OR it represents a foundational node (glossary, key process\n                                 definitions) that other entries cite as a dependency.\n        Tier 2 (important, deferrable): referenced by exactly 1 other entry; no required flag.\n        Tier 3+ (enrichment): leaf node — zero dependents, no required flag.\n      Sort gap list ascending by tier, then alphabetically by gap_id within each tier."}`
**action:** replace
**text:** |2
        STEP 2 — tier_scoring:
          Pre-step (cycle handling): build the schema_definition dependency graph G
          where edges go from each entry to its declared dependencies. Run cycle
          detection. If G is a DAG, proceed to scoring below. If G contains any
          strongly connected components (SCCs), apply Kosaraju decomposition: every
          member of any SCC is jointly promoted to Tier 1, recorded with a
          `scc_group_id` for later debugging. Acyclic remainder is then scored
          normally. (Without this, topological sort diverges silently on cyclic
          inputs — H-CONS2.)

          Score each gap using these criteria:
            Tier 1 (core-necessary): gap_id is referenced by ≥2 other entries in
                                     schema_definition, OR the schema_definition
                                     marks it required: true, OR it represents a
                                     foundational node (glossary, key process
                                     definitions) that other entries cite as a
                                     dependency, OR it is a member of any SCC
                                     promoted in the pre-step.
            Tier 2 (important, deferrable): referenced by exactly 1 other entry;
                                            no required flag.
            Tier 3+ (enrichment): leaf node — zero dependents, no required flag.
          Sort gap list ascending by tier, then alphabetically by gap_id within
          each tier.
**rationale:** Closes H-CONS2 (confidence 0.85). Bootstrap is non-functional on cyclic master-index input without this. Kosaraju is the standard linear-time SCC algorithm; the joint-Tier-1 promotion preserves the dependency-first principle from CS5.
**compare_notes:** Draft 2 raised a structured `CyclicSchemaError` and halted on any cycle detection (forcing the user to repair the schema). Pros: simpler, more conservative. Cons: blocks bootstrap on real-world inputs where cycles are plausible (mutual citations in a master-index — OQ-2 explicitly flags this as expected). Draft 1's joint-promotion path is more permissive and exactly matches the convergent recommendation from N6 / IB-5.

---

## E04 — EC-7: PBCPB-internal failure handling
**from_d_id:** D04
**from_idea_id:** I04
**target:** `{"kind": "anchor", "before": "  EC-6 — CUSTOM_API populate() failure:", "after": "</edge_cases>"}`
**action:** insert-before
**text:** |2

  EC-7 — PBCPB-internal failure paths:
    PBCPB-internal failures (as distinct from user-owned-boundary failures covered by
    EC-2 and EC-6) include: JSON_DB connection refused or filesystem unavailable;
    MCP server timeout or unreachable; malformed config JSON detected at
    AdapterSession.bind(); invalid adapter_type or role_type enum value (e.g. "RAGMCP"
    or "HUMAN_ONLY"). All four MUST surface as structured errors with a uniform shape
    `{ error_class, cause, adapter_type|null, recoverable: bool }`. Bind-time errors
    halt with `ConfigError` (recoverable: false); per-call I/O errors raise to the
    caller as `AdapterIOError` (recoverable: true) with a configurable timeout
    (default 60s) before the caller sees a definitive failure. Silent degradation
    (empty response, hang, undefined exception) MUST NOT occur.
**rationale:** Closes H-CONS3 (confidence 0.80). Symmetric counterpart to EC-2/EC-6 — user-boundary failures are loud; internal failures must be too. Distinguishes bind-time (halt) from runtime-call (raise to caller) so the framework-vs-executor dual role (E06) is respected: bind-time halts the whole system; per-call errors are propagated.
**compare_notes:** Draft 2 split this into two edge cases (EC-7 for bind-time and EC-8 for runtime) for sharper separation. Pros: more granular. Cons: doubled the edge-case count for closely related failure modes. Draft 1 (chosen) keeps them in one EC-7 with the `recoverable` flag distinguishing the two sub-cases — single anchor, lower edit churn.

---

## E05 — gap_id namespace coherence rule
**from_d_id:** D05
**from_idea_id:** I05
**target:** `{"kind": "anchor", "before": "    schema_definition is a JSON document describing the expected KB structure.", "after": "    For a new KB, generate schema_definition from the PBCPB playbook's KB phase definitions."}`
**action:** insert-before
**text:** |2

    gap_id namespace coherence rule (binds adapters to schema_definition):
      Every `gap_id` returned by `scan_gaps(schema_definition)` MUST resolve to a
      `entry.id` in the supplied `schema_definition`. Adapters internally map their
      native identifier system (Dify document IDs, JSON keys, custom URIs) to
      canonical `schema_definition.entry.id` before returning. Phase 5 STEP 1
      validates this on receipt: any unresolved `gap_id` halts bootstrap with a
      structured `SchemaMismatchError` carrying the offending `gap_id` and source
      adapter. Without this rule, the bootstrap causal chain has a silent corruption
      mode (DF-10): tier-scoring against mismatched IDs produces incorrect Tier 1
      identification.
**rationale:** Closes the weakest link in the bootstrap causal chain (DF-10) and partially closes the Leaky Abstraction concern (DF-8). Adapter contract becomes self-validating against schema_definition.
**compare_notes:** Draft 2 placed this rule as a new bullet inside the adapter interface contract method list (after the three required_methods). Pros: collocated with the method that produces gap_ids. Cons: the rule is *about* schema_definition (which the user provides) more than the method itself; placing it adjacent to the schema_definition explanation paragraph (Draft 1's location) groups concerns better.

---

## E06 — Document PBCPB's dual role (framework + executor)
**from_d_id:** D06
**from_idea_id:** I06
**target:** `{"kind": "anchor", "before": "    The goal is to produce a unified PBCPB tool that:", "after": "    1. Integrates pbcpb-dify changes back to the main system."}`
**action:** insert-before
**text:** |2

    Note on PBCPB's role: PBCPB is simultaneously a coordination scaffold and an
    execution engine, depending on adapter type:
      - For MARKDOWN_FOLDER, PBCPB is a coordination scaffold — KB operations are
        delegated to user-provided skills via skill_ref.
      - For JSON_DB and CUSTOM_API, PBCPB is an execution engine — it directly
        invokes the adapter's three methods.
      - For RAG_MCP, PBCPB is a hybrid — query and scan_gaps execute against the
        Dify MCP server; populate produces a manifest for user-driven manual upload.
    This dual role is reflected in the error model: failures at user-owned
    boundaries (EC-2, EC-6) and PBCPB-internal failures (EC-7) follow distinct
    paths.

**rationale:** Surfaces the partial-falsification result of H-CONS4. Reduces reader confusion when JSON_DB and CUSTOM_API behave differently from MARKDOWN_FOLDER. Documentation-only; zero implementation cost. Forward-references EC-2/EC-6/EC-7.
**compare_notes:** Draft 2 placed this as a footer to the kb_scope block. Pros: collocated with adapter-type definitions. Cons: by the time the reader reaches the kb_scope footer they have already absorbed the misleading framing in `<background>`. Draft 1 (chosen) is upstream of that misframing.

---

## E07 — Out-of-scope guardrails sub-block
**from_d_id:** D07
**from_idea_id:** [I07, I18, I21]
**target:** `{"kind": "anchor", "before": "    - All implementation is staged to ~/Documents/pbcpb-unified/ before replacing the\n      original at ~/Documents/pbcpb/.", "after": "  </operational_constraints>"}`
**action:** insert-after
**text:** |2

    - Out of scope (PBCPB provides no primitives; users build at the application
      layer if required):
        (a) Cross-method transactions or atomic compound operations across
            query/populate/scan_gaps.
        (b) Adapter-specific operations outside the 3-method surface (e.g. Dify
            reindex, JSON_DB vacuum, MCP-specific telemetry). If a backend exposes
            such operations they are accessed through an adapter-specific extension
            hook and are NOT portable.
        (c) Stateful KB sessions (open cursors, authenticated multi-call sessions,
            streaming imports). Any session state MUST be encapsulated within the
            adapter's `connection` config and not exposed through method signatures.
        (d) Multi-KB join operations (single adapter per AdapterSession; users
            requiring cross-KB join must compose at the application layer).
        (e) Adapter migration with dual-write or transition-state primitives.
            Recommended cutover pattern: (1) freeze writes, (2) export current KB,
            (3) update config to new adapter, (4) re-run Phase 5 bootstrap (or
            generate manifest for RAG_MCP), (5) resume writes.
        (f) Agent lifecycle management. Startup, health-check, shutdown, and crash
            recovery for any system bound via `linked_agent` are the user's
            responsibility. PBCPB invokes `linked_agent` synchronously per role
            action; an unresponsive agent surfaces as an `AdapterIOError` after the
            configurable timeout (default 60s).
**rationale:** Consolidates six guardrails (DF-1, DF-2, DF-3, DF-4, DF-5, DF-6) into one anchored sub-block. Each guardrail states the limit and (where appropriate) names the user-side recommended pattern. No primitive is added; this is documentation-only and purely additive. Closes ambiguity around assumed-but-unsupported features.
**compare_notes:** Draft 2 created a brand-new top-level XML block `<out_of_scope>` between `<constraints>` and `<output_format>`. Pros: maximum visibility; could be referenced by name. Cons: a new top-level block changes the prompt's overall structure; the items are constraints-style limits, so the natural home is `<operational_constraints>`. Draft 1 (chosen) keeps the structural change minimal.

---

## E08 — Strengthen verification item 12
**from_d_id:** D08
**from_idea_id:** I08
**target:** `{"kind": "verbatim_block", "text": "  [ ] 12. All role_type JSON values in config use the exact enum strings MULTI_AGENT |\n          HUMAN | AI_ASSISTED; HUMAN_ONLY is not a valid enum value."}`
**action:** replace
**text:** |2
    [ ] 12. The role_system parser rejects pseudo-values for role_type (HUMAN_ONLY,
            MULTIAGENT, ai-assisted, etc.) with a structured ConfigError at
            AdapterSession.bind() time; only the exact strings MULTI_AGENT, HUMAN,
            AI_ASSISTED parse successfully. The same exact-string rule applies to
            adapter_type — only RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API parse
            successfully; pseudo-values (RAGMCP, markdown-folder, etc.) raise
            ConfigError. (ROLE_SYSTEM, KB_BACKEND_SUPPORT)
**rationale:** Active-rejection rule replaces passive "not a valid enum value" wording. Makes the parser the gatekeeper, not the documentation. Extends the same rule to adapter_type symmetrically. Adds parenthetical constraint reference (per D22 pattern). Anchors the test at AdapterSession.bind() time per E02.
**compare_notes:** Draft 2 split this into two verification items (one for role_type, one for adapter_type). Pros: more granular, each enum testable separately. Cons: the rule is identical for both; duplicating it doubles surface area. Draft 1 (chosen) keeps a single combined item.

---

## E09 — CALLER_INVARIANCE constraint
**from_d_id:** D09
**from_idea_id:** I09
**target:** `{"kind": "anchor", "before": "  RUNTIME_CONFIG", "after": "    KB backend selection is a runtime configuration decision, NOT a code-level switch."}`
**action:** insert-before
**text:** |2

  CALLER_INVARIANCE
    Business-logic code MUST NOT import or reference any backend-specific module,
    class, or type. All KB access goes through the AdapterSession handle's
    three-method surface (query, populate, scan_gaps) only. Statically enforceable
    via lint or import-graph rules; verified in CI. This is a precondition for the
    runtime-config-driven dispatch model: a single backend-specific import in
    business logic invalidates the no-rebuild guarantee.
**rationale:** Makes the implicit caller-invariance precondition (GC-2) explicit and statically enforceable. Forward-references AdapterSession (E02). Names the verification path (lint / CI).
**compare_notes:** Draft 2 added the rule as a sub-bullet within RUNTIME_CONFIG. Pros: collocated with the related dispatch constraint. Cons: blends two distinct invariants (runtime-config-driven vs. caller-invariance) into one row. Draft 1 (chosen) keeps each constraint as its own row, matching the rest of the constraint table.

---

## E10 — Phase 1.7: cyclic-master-index detection
**from_d_id:** D10
**from_idea_id:** I10
**target:** `{"kind": "anchor", "before": "  Output: a written catalogue (phase1-catalogue.md in staging dir) listing findings", "after": "  from 1.1–1.6 before proceeding to Phase 2."}`
**action:** insert-before
**text:** |2
  1.7 If a master-index exists in pbcpb, build its dependency graph (entries as
      nodes; declared dependencies as directed edges) and run cycle detection. For
      each strongly connected component found, record { scc_group_id, member
      entry_ids } in phase1-catalogue.md. This output feeds Phase 5 step 5.2
      (tier-scoring algorithm selection: DAG topological sort vs. SCC-aware
      Kosaraju path).

**rationale:** Diagnostic feed-forward to E03. Phase 1 already explores both codebases; cycle detection on the existing master-index is a natural extension and closes OQ-2 production-risk.
**compare_notes:** Draft 2 added cycle detection as a sub-step inside 1.4 (existing-abstraction-layer identification). Pros: collocated with master-index discovery. Cons: 1.4 is about whether an abstraction layer exists; cycle detection is a separate concern about the master-index *content*. Draft 1 (chosen) places it as its own numbered step where the prerequisites (1.4 already ran) are clear.

---

## E11 — Phase 1.3: explicit feature inventory requirement
**from_d_id:** D11
**from_idea_id:** I11
**target:** `{"kind": "verbatim_block", "text": "  1.3 Map the top-level module/directory structure of each project."}`
**action:** replace
**text:** |2
    1.3 Map the top-level module/directory structure of each project. For pbcpb,
        additionally produce an explicit feature inventory — one row per
        user-facing feature: { name, entry point, expected behavior }. This
        inventory is the source-of-truth for Phase 4.6 validation ("retains all
        original pbcpb functionality"). Without it, Phase 4.6 is structurally
        non-executable.
**rationale:** Closes OQ-2's MEDIUM probe / Probe 6 / A1 Gap #1. Phase 4.6 explicitly relies on a comparable feature inventory; making it a Phase 1.3 deliverable removes the structural blocker.
**compare_notes:** Draft 2 added the feature inventory as a brand-new step 1.8 alongside cyclic-master-index detection. Pros: groups all "diagnostic outputs" in late-1.x. Cons: feature inventory is a *cataloguing* output (matches 1.3's intent — "map the structure"), not a diagnostic. Draft 1 (chosen) is in-place strengthening of 1.3.

---

## E12 — Verification items 13 and 14
**from_d_id:** D12
**from_idea_id:** I12
**target:** `{"kind": "anchor", "before": "  [ ] 12.", "after": "</verification>"}`
**action:** insert-after
**text:** |2
    [ ] 13. Invalid adapter_type or role_type values (e.g. "RAGMCP",
            "markdown-folder", "HUMAN_ONLY") in config are rejected at
            AdapterSession.bind() with a structured ConfigError that names the
            offending field; the dispatch layer is never reached. (E02, E08,
            KB_BACKEND_SUPPORT, ROLE_SYSTEM)
    [ ] 14. A cyclic schema_definition (entry A depends on B; B depends on A) is
            handled deterministically by Phase 5 step 5.2 — either rejected with a
            structured CyclicSchemaError, OR every member of the cycle is jointly
            promoted to Tier 1 via Kosaraju SCC decomposition. Silent topological-
            sort divergence MUST NOT occur. (E03, KB_POPULATION)

**rationale:** Adds runtime-testable coverage for the two highest-confidence design risks (H-CONS1 covered by E08 + E12, and H-CONS2 covered by E03 + E12.14). Aligns the verification block with N5's 8-step constructive spec.
**compare_notes:** Draft 2 phrased item 14 as accepting only the rejection path (CyclicSchemaError, no SCC promotion). Pros: simpler. Cons: contradicts E03's chosen behavior (joint Tier 1 promotion). Draft 1 (chosen) accepts either deterministic resolution, matching E03.

---

## E13 — Deferred-backlog idempotency and re-run semantics
**from_d_id:** D13
**from_idea_id:** I13
**target:** `{"kind": "verbatim_block", "text": "      These entries are not populated during bootstrap. Detailed content population comes\n      after bootstrap phase completes by re-running Phase 5 with a higher tier_limit."}`
**action:** replace
**text:** |2
          These entries are not populated during bootstrap. Detailed content
          population comes after bootstrap completes by re-running Phase 5 with a
          higher `bootstrap_tier_limit`. Re-run semantics: (i) any existing
          deferred-backlog.json is read for context but is NOT trusted as
          authoritative; (ii) scan_gaps runs fresh for idempotency; (iii) only gaps
          falling within the new tier_limit are populated; gaps already populated
          on the previous run are re-checked via scan_gaps and skipped if no longer
          present; (iv) a fresh deferred-backlog.json is written with `generated_at`
          updated and `tier_limit_used` reflecting the new value. Re-runs are safe
          to repeat; running with the same tier_limit twice is a no-op modulo
          timestamp.
**rationale:** Closes A1 Gap #6. Idempotency is the correct default for any "re-run with higher limit" pattern. Specifies the four sub-rules unambiguously so adapter implementers do not invent divergent behaviors.
**compare_notes:** Draft 2 specified that deferred-backlog.json is the authoritative source for re-runs (read it; populate from its entries directly). Pros: faster re-runs. Cons: not idempotent — a stale deferred-backlog could populate an entry that is no longer a gap, regressing user content. Draft 1 (chosen) prioritizes correctness via fresh scan_gaps.

---

## E14 — STATELESS_INTERFACE constraint
**from_d_id:** D14
**from_idea_id:** I14
**target:** `{"kind": "anchor", "before": "  CALLER_INVARIANCE", "after": "  RUNTIME_CONFIG"}`
**action:** insert-before
**text:** |2

  STATELESS_INTERFACE
    The 3-method adapter contract is stateless across calls. Any session state
    (auth tokens, open cursors, streaming connections, retained results) MUST be
    encapsulated within the adapter implementation behind the `connection` config
    block — never exposed through method signatures and never shared between
    successive method invocations. Stateless invocation is a precondition for
    runtime backend switching: the same logical KB call must produce equivalent
    results regardless of when the AdapterSession was bound.
**rationale:** Makes the implicit invariant explicit. Pairs with E07 (out-of-scope guardrail (c)) and E09 (CALLER_INVARIANCE). Adapter implementers are unambiguously instructed where to encapsulate session state.
**compare_notes:** Draft 2 placed STATELESS_INTERFACE as a sub-bullet under KB_BACKEND_SUPPORT. Pros: collocated with the adapter type list. Cons: KB_BACKEND_SUPPORT is about *which* backends are supported; statelessness is about *how* they are accessed. Draft 1 (chosen) keeps these as separate constraint rows.

---

## E15 — Reference architectures in System Overview
**from_d_id:** D15
**from_idea_id:** I16
**target:** `{"kind": "anchor", "before": "    pbcpb-dify at\n    file:///home/myuser/Documents/pbcpb-dify), the merge rationale, and the unified target\n    at ~/Documents/pbcpb/. Summary of supported KB backend types and role system types.", "after": "  SECTION 2 — KB Adapter Module (code + config schema)"}`
**action:** insert-after
**text:** |2

    Reference architecture orientation (include in System Overview):
      The PBCPB adapter interface follows the POSIX file-descriptor pattern —
      uniform method surface across heterogeneous backends; backend-specific
      behavior encapsulated inside the adapter. Configuration-driven dispatch
      mirrors JDBC connection-string selection. The AdapterSession.bind(config_path)
      initialization phase mirrors TLS handshake — config errors are caught at
      bind, not at first call. `linked_agent` and `skill_ref` are dependency-
      injection tokens in the Spring-DI sense; runtime-only binding. These four
      reference architectures are the structural ancestors of the PBCPB design and
      provide an orientation aid for readers familiar with any of them.
**rationale:** Two-line orientation per analog (POSIX fd, JDBC, TLS, Spring-DI) accelerates reader comprehension. Surfaces CS1/CS2/CS3 cross-domain recurrence supported by 4 nodes each. Forward-references AdapterSession (E02), `linked_agent`, `skill_ref`.
**compare_notes:** Draft 2 included an extended explanation per analog (3-4 lines each). Pros: more thorough. Cons: significantly increased surface area for documentation-only content; the two-liner is sufficient for orientation. Draft 1 (chosen) is the minimum useful framing.

---

## E16 — Reframe `<background>` to lead with the unification problem
**from_d_id:** D16
**from_idea_id:** I17
**target:** `{"kind": "verbatim_block", "text": "  <background>\n    PBCPB is a JSON-based playbook creator tool (Playbook Creator Playbook Builder). Two\n    existing projects are present on the filesystem:"}`
**action:** replace
**text:** |2
  <background>
    Goal: produce a unified, backend-agnostic PBCPB tool that integrates the
    Dify/MCP improvements from `pbcpb-dify` back into the original `pbcpb`,
    abstracts KB access behind a pluggable adapter interface (any backend the user
    chooses), and supports all human-only / AI-assisted / multi-agent role
    configurations declared at runtime.

    PBCPB (Playbook Creator Playbook Builder) is a JSON-based playbook creator
    tool. Two existing projects are present on the filesystem:
**rationale:** Restructures `<background>` to lead with the goal sentence. The two-paragraph reorder preserves all existing content; only the opening sentence of the block changes. Improves first-impression clarity per Pass 3 user-empathy review.
**compare_notes:** Draft 2 rewrote the entire `<background>` block from scratch with tighter prose. Pros: cleaner overall. Cons: high regression risk — every fact in `<background>` is referenced elsewhere in the prompt; rewriting could lose subtle phrasing. Draft 1 (chosen) is minimum-surface-area reframe — only inserts a goal-sentence paragraph above the existing content, preserving every other word verbatim.

---

## E17 — Tier 0 cross-cutting concerns (opt-in)
**from_d_id:** D17
**from_idea_id:** I19
**target:** `{"kind": "anchor", "before": "      Sort gap list ascending by tier, then alphabetically by gap_id within each tier.", "after": "    STEP 3 — populate_core:"}`
**action:** insert-before
**text:** |2

          Optional: Tier 0 (foundational cross-cutting concerns; opt-in). When
          `bootstrap_tier_zero_threshold` is set in `kb_adapter` config to a
          positive integer N, any entry with in-degree ≥ N is promoted to Tier 0
          and bootstrapped strictly before Tier 1. Default value 0 disables Tier 0
          (all behavior unchanged from above). Use case: schemas with shared
          glossary or error-code-catalog entries referenced by most other entries.
          A negative or non-integer threshold raises ConfigError at bind() time.
**rationale:** Generalizes the dependency-first principle to cross-cutting concerns. Behind a config flag default-disabled, so zero regression risk. Closes I19 / D17 conditional acceptance gate cleanly.
**compare_notes:** Draft 2 made Tier 0 unconditional (any entry with in-degree ≥ floor(total × 0.5) auto-promotes). Pros: simpler — no extra config field. Cons: changes default behavior for all existing PBCPB users — a regression. Draft 1 (chosen) is opt-in and preserves backward compatibility. Honors the D17 conditional ("MUST be behind an opt-in config field").

---

## E18 — Single-config-file invariant
**from_d_id:** D18
**from_idea_id:** I20
**target:** `{"kind": "verbatim_block", "text": "  RUNTIME_CONFIG\n    KB backend selection is a runtime configuration decision, NOT a code-level switch.\n    Same PBCPB instance switches backends via config file. No rebuild required to change\n    KB backend. Enables zero-rebuild KB migration and multi-user deployment with different\n    KB backends on the same tool version."}`
**action:** replace
**text:** |2
  RUNTIME_CONFIG
    KB backend selection is a runtime configuration decision, NOT a code-level
    switch. PBCPB reads its complete runtime configuration from a single canonical
    file: default `pbcpb.config.json` in the project root, overridable via
    `PBCPB_CONFIG_PATH` env var. Backend switching is achieved by editing this file
    or pointing `PBCPB_CONFIG_PATH` at a different file. Multiple config files MUST
    NOT compose. The same PBCPB instance switches backends via config file; no
    rebuild required. Enables zero-rebuild KB migration and multi-user deployment
    with different KB backends on the same tool version. Note: this guarantee
    assumes the deployment supports runtime config-file modification — container
    images with read-only baked config are out of scope (see operational_constraints
    item (e) and Out-of-scope (a)).
**rationale:** Stabilizes the no-rebuild-required guarantee on a known config-file location. Names a default and an override env var. Forbids multi-file composition (which would break the runtime-switch promise in subtle ways). Adds the baked-config scope qualifier from SL-3 / GC-3. Replacement preserves the original constraint text (every original sentence is retained; new content only adds specifics).
**compare_notes:** Draft 2 specified composition rules (e.g. base config + env-specific overrides) and made multiple files allowed under defined merge precedence. Pros: more flexible. Cons: precisely the failure mode the constraint is meant to prevent — an override file changing `adapter_type` mid-deployment is the exact scenario that breaks the no-rebuild guarantee in subtle ways. Draft 1 (chosen) explicitly forbids composition.

---

## E19 — Per-adapter `kb_adapter` config snippets
**from_d_id:** D19
**from_idea_id:** I22
**target:** `{"kind": "anchor", "before": "    Note: skill_ref is used consistently throughout. It is required when adapter_type is\n    MARKDOWN_FOLDER; optional for CUSTOM_API; unused for RAG_MCP and JSON_DB.", "after": "    IMPORTANT — Dify upload constraint:"}`
**action:** insert-before
**text:** |2

    Per-adapter examples (use these as templates for the docs/ snippets required
    by `<output_format>` Section 2):

      RAG_MCP:
        { "kb_adapter": {
            "adapter_type": "RAG_MCP",
            "connection": { "mcp_server_url": "http://localhost:8080/mcp",
                            "dify_api_key_env": "DIFY_API_KEY" },
            "bootstrap_tier_limit": 1 } }

      MARKDOWN_FOLDER:
        { "kb_adapter": {
            "adapter_type": "MARKDOWN_FOLDER",
            "connection": { "root_path": "~/kb/markdown" },
            "skill_ref": "user-skills/markdown-kb-skill",
            "bootstrap_tier_limit": 1 } }

      JSON_DB:
        { "kb_adapter": {
            "adapter_type": "JSON_DB",
            "connection": { "db_path": "~/Documents/pbcpb/master-index.json" },
            "bootstrap_tier_limit": 1 } }

      CUSTOM_API:
        { "kb_adapter": {
            "adapter_type": "CUSTOM_API",
            "connection": { "endpoint": "https://kb.example.com/api/v1",
                            "auth_token_env": "PBCPB_KB_TOKEN" },
            "skill_ref": null,
            "bootstrap_tier_limit": 1 } }
**rationale:** Closes A1 Gap (missing concrete examples). Each snippet shows realistic `connection` block and skill_ref usage per adapter. These become the doc-snippet examples Section 2 of `<output_format>` requires.
**compare_notes:** Draft 2 used a single combined example with all four adapters as commented variants. Pros: compact. Cons: harder to copy-paste a single snippet; reduces example utility. Draft 1 (chosen) keeps four standalone snippets — direct copy-paste-ready.

---

## E20 — Phase 5 dotted-decimal sub-step renumbering
**from_d_id:** D20
**from_idea_id:** I23
**target:** `{"kind": "verbatim_block", "text": "  CoT Bootstrap Sequence:\n\n    STEP 1 — scan_gaps:"}`
**action:** replace
**text:** |2
    CoT Bootstrap Sequence (steps 5.1 → 5.5):

        5.1 — scan_gaps:
**rationale:** Renumbering Phase 5 sub-steps to dotted-decimal aligns with Phase 1.x / 2.x / 3.x / 4.x style and improves cross-reference precision throughout the prompt. **Note:** This is the *first* of five rename edits; sister edits at STEPs 2/3/4/5 are part of the same conceptual change but require separate verbatim-block targets. Integration applies all five renames together; the conflict resolver merges them.
**compare_notes:** Draft 2 deferred renumbering to a follow-up cleanup task. Pros: less churn. Cons: leaves the inconsistency in place — every later cross-reference is harder to read. Draft 1 (chosen) renumbers in this pass.

---

## E20b — Phase 5 sub-step rename (STEP 2 → 5.2)
**from_d_id:** D20
**from_idea_id:** I23
**target:** `{"kind": "verbatim_block", "text": "    STEP 2 — tier_scoring:"}`
**action:** replace
**text:** |2
        5.2 — tier_scoring:
**rationale:** Sister rename to E20.
**compare_notes:** Same as E20.

---

## E20c — Phase 5 sub-step rename (STEP 3 → 5.3)
**from_d_id:** D20
**from_idea_id:** I23
**target:** `{"kind": "verbatim_block", "text": "    STEP 3 — populate_core:"}`
**action:** replace
**text:** |2
        5.3 — populate_core:
**rationale:** Sister rename to E20.
**compare_notes:** Same as E20.

---

## E20d — Phase 5 sub-step rename (STEP 4 → 5.4)
**from_d_id:** D20
**from_idea_id:** I23
**target:** `{"kind": "verbatim_block", "text": "    STEP 4 — defer_tier2_plus:"}`
**action:** replace
**text:** |2
        5.4 — defer_tier2_plus:
**rationale:** Sister rename to E20.
**compare_notes:** Same as E20.

---

## E20e — Phase 5 sub-step rename (STEP 5 → 5.5)
**from_d_id:** D20
**from_idea_id:** I23
**target:** `{"kind": "verbatim_block", "text": "    STEP 5 — dify_upload_manifest (conditional, only when adapter_type == RAG_MCP):"}`
**action:** replace
**text:** |2
        5.5 — dify_upload_manifest (conditional, only when adapter_type == RAG_MCP):
**rationale:** Sister rename to E20.
**compare_notes:** Same as E20.

---

## E21 — Glossary block
**from_d_id:** D21
**from_idea_id:** I24
**target:** `{"kind": "anchor", "before": "<context>", "after": "  <background>"}`
**action:** insert-after
**text:** |2

  <glossary>
    PBCPB             — Playbook Creator Playbook Builder. JSON-based playbook
                        authoring tool; the system being unified in this prompt.
    MCP               — Model Context Protocol. Standardized server protocol for
                        exposing tool/RAG capabilities to LLM clients.
    Dify              — RAG knowledge platform; accessed by PBCPB via MCP server.
                        Supports retrieval but requires manual upload for writes.
    RAG_MCP           — adapter_type for the Dify-via-MCP backend; populate()
                        produces a manifest, never writes directly.
    schema_definition — JSON document describing the expected KB structure: entry
                        list with ids/categories, required fields, dependency
                        relationships. Authoritative ordering artifact for Phase 5.
    master-index      — Existing pbcpb on-disk index document used in JSON_DB
                        backends; source for deriving schema_definition when
                        unifying an existing KB.
  </glossary>
**rationale:** Six-row glossary at the top of `<context>`. Defines the highest-leverage terms once. Improves first-time reader experience.
**compare_notes:** Draft 2 used a 12-row glossary with every PBCPB-specific term defined. Pros: more thorough. Cons: duplicates `<kb_scope>` / `<role_scope>` content; risks divergence under future edits. Draft 1 (chosen) is the minimum viable glossary — six terms not defined anywhere else.

---

## E22 — Verification items cross-referenced to constraints
**from_d_id:** D22
**from_idea_id:** I25
**target:** `{"kind": "verbatim_block", "text": "  [ ] 1. ~/Documents/pbcpb-original-backup/ exists and contains the unmodified original\n         before any changes are applied to ~/Documents/pbcpb/.\n  [ ] 2. ~/Documents/pbcpb/ is the final unified codebase (pbcpb-unified/ was renamed\n         into place after validation).\n  [ ] 3. Switching kb_adapter.adapter_type in config (without code changes) routes KB\n         operations to the correct adapter implementation at runtime.\n  [ ] 4. All three adapter methods (query, populate, scan_gaps) are callable for each\n         of the four adapter types: RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API.\n  [ ] 5. MARKDOWN_FOLDER adapter raises a configuration error when skill_ref is absent.\n  [ ] 6. RAG_MCP adapter's populate() produces a dify-upload-manifest.json and does not\n         write directly to Dify.\n  [ ] 7. Role system config with role_type MULTI_AGENT, HUMAN, and AI_ASSISTED all parse\n         and operate correctly; mixed configurations coexist without conflict.\n  [ ] 8. A placeholder with linked_agent == null does not cause an error; updating\n         linked_agent in config takes effect without rebuild.\n  [ ] 9. Bootstrap Phase 5 produces deferred-backlog.json at the defined path and schema.\n  [ ] 10. Bootstrap Tier 1 scoring correctly identifies entries with ≥2 dependents or\n          required: true in schema_definition as Tier 1.\n  [ ] 11. CUSTOM_API adapter raises a hard error (not silent skip) when populate() fails."}`
**action:** replace
**text:** |2
    [ ] 1. ~/Documents/pbcpb-original-backup/ exists and contains the unmodified
           original before any changes are applied to ~/Documents/pbcpb/. (Phase 4.0)
    [ ] 2. ~/Documents/pbcpb/ is the final unified codebase (pbcpb-unified/ was
           renamed into place after validation). (Phase 4.7)
    [ ] 3. Switching kb_adapter.adapter_type in config (without code changes)
           routes KB operations to the correct adapter implementation at runtime.
           (RUNTIME_CONFIG, KB_BACKEND_SUPPORT)
    [ ] 4. All three adapter methods (query, populate, scan_gaps) are callable for
           each of the four adapter types: RAG_MCP, MARKDOWN_FOLDER, JSON_DB,
           CUSTOM_API. (KB_BACKEND_SUPPORT)
    [ ] 5. MARKDOWN_FOLDER adapter raises a configuration error when skill_ref is
           absent. (USER_DELEGATION, EC-2)
    [ ] 6. RAG_MCP adapter's populate() produces a dify-upload-manifest.json and
           does not write directly to Dify; the return value's `kind` is
           "manifest", distinguishable from a write-status return. (KB_POPULATION,
           EC-1, E01)
    [ ] 7. Role system config with role_type MULTI_AGENT, HUMAN, and AI_ASSISTED
           all parse and operate correctly; mixed configurations coexist without
           conflict. (ROLE_SYSTEM, EC-4)
    [ ] 8. A placeholder with linked_agent == null does not cause an error;
           updating linked_agent in config takes effect without rebuild. (ROLE_BINDING,
           EC-3)
    [ ] 9. Bootstrap Phase 5 produces deferred-backlog.json at the defined path
           and schema. (KB_POPULATION)
    [ ] 10. Bootstrap Tier 1 scoring correctly identifies entries with ≥2
            dependents or required: true in schema_definition as Tier 1.
            (KB_POPULATION)
    [ ] 11. CUSTOM_API adapter raises a hard error (not silent skip) when
            populate() fails. (KB_BACKEND_SUPPORT, EC-6)
**rationale:** Adds parenthetical constraint references (or phase / E-id references where the test maps to a specific phase or solution rather than a named constraint). Improves bidirectional traceability without renumbering. Lightweight; preserves all existing test text.
**compare_notes:** Draft 2 added a separate "Reference: <constraint>" line under each verification item. Pros: visually clearer separation. Cons: doubled the line count of the verification block. Draft 1 (chosen) is parenthetical inline — minimum visual disruption.

---

## E23 — Dedicated `<dify_constraint>` sub-block
**from_d_id:** D23
**from_idea_id:** I26
**target:** `{"kind": "anchor", "before": "  </background>", "after": "  <kb_scope>"}`
**action:** insert-after
**text:** |2

  <dify_constraint>
    Constraint: The Dify RAG system requires uploading knowledge separately on the
                user's own initiative. PBCPB does not write directly to Dify.
    Affected adapters: RAG_MCP only.
    What populate() returns for RAG_MCP: a `{ kind: "manifest", ... }` object
                                         pointing to a generated upload manifest
                                         file (see E01 for the discriminated-union
                                         schema). No content is written to Dify.
    What the user must do: open the manifest file and apply each item to Dify
                           using the Dify UI or upload tooling. PBCPB cannot
                           automate this step.
    Failure mode if treated uniformly: a caller treating populate() responses
                                       uniformly across adapters generates a
                                       manifest but treats it as a write
                                       confirmation — Dify ends up empty with no
                                       error raised. This is the highest-confidence
                                       (0.88) supported behavioral risk identified
                                       in the architecture analysis.
  </dify_constraint>
**rationale:** Lifts the Dify upload constraint into a structured anchored block. Compresses what is currently scattered across `<kb_scope>` IMPORTANT paragraph, EC-1, verification item 6, and the AH-1 finding. Reader who skims `<context>` cannot miss it. Closes the highest-confidence design risk by making it the most prominent constraint in the prompt.
**compare_notes:** Draft 2 used a single hard-uppercase-IMPORTANT paragraph in `<kb_scope>` (the existing pattern). Pros: minimal disruption. Cons: leaves the constraint in the same position where it has demonstrably been missed (H-CONS1 confidence 0.88). Draft 1 (chosen) elevates it to its own anchor block.

---

## E24 — Phase prerequisites annotation
**from_d_id:** D24
**from_idea_id:** I27
**target:** `{"kind": "verbatim_block", "text": "  PHASE 1 — Explore and Catalogue Both Codebases\n  ────────────────────────────────────────────────\n  Before any implementation, build a working model of both projects."}`
**action:** replace
**text:** |2
    PHASE 1 — Explore and Catalogue Both Codebases
    ────────────────────────────────────────────────
    Prerequisite: none.
    Before any implementation, build a working model of both projects.
**rationale:** Adds explicit "Prerequisite:" annotation to Phase 1; sister edits add prerequisites to Phases 2/3/5 (Phase 4 already has implicit prerequisites stated in its preface). Standardizes phase-gate annotation across all phases.
**compare_notes:** Draft 2 added prerequisites only to phases that did not already have them (i.e. all but Phase 4). Pros: minimum churn. Cons: inconsistent — Phase 4's prerequisite is stated as prose, not a labeled "Prerequisite:" line. Draft 1 (chosen) standardizes the annotation but preserves Phase 4's existing prose intact (sister edits add the line to Phases 2/3/5 only; Phase 4 is left alone since its prose is already explicit and adding another line would be redundant).

---

## E24b — Phase 2 prerequisite annotation
**from_d_id:** D24
**from_idea_id:** I27
**target:** `{"kind": "verbatim_block", "text": "  PHASE 2 — Design KB Adapter Interface\n  ───────────────────────────────────────\n  Design first, implement after. This phase must complete before Phase 4 begins."}`
**action:** replace
**text:** |2
    PHASE 2 — Design KB Adapter Interface
    ───────────────────────────────────────
    Prerequisite: Phase 1 complete (phase1-catalogue.md produced, including the
                  feature inventory from 1.3 and any cyclic-master-index findings
                  from 1.7).
    Design first, implement after. This phase must complete before Phase 4 begins.
**rationale:** Sister edit to E24. Forward-references Phase 1.3 (E11) and 1.7 (E10).
**compare_notes:** Same as E24.

---

## E24c — Phase 3 prerequisite annotation
**from_d_id:** D24
**from_idea_id:** I27
**target:** `{"kind": "verbatim_block", "text": "  PHASE 3 — Design Role System Abstraction\n  ──────────────────────────────────────────"}`
**action:** replace
**text:** |2
    PHASE 3 — Design Role System Abstraction
    ──────────────────────────────────────────
    Prerequisite: Phase 1 complete. (Phase 3 is independent of Phase 2 design and
                  may run in parallel.)
**rationale:** Sister edit to E24. Documents the independence of Phase 3 from Phase 2 (a non-obvious sequencing fact).
**compare_notes:** Same as E24.

---

## E24d — Phase 5 prerequisite annotation
**from_d_id:** D24
**from_idea_id:** I27
**target:** `{"kind": "verbatim_block", "text": "  PHASE 5 — Implement KB Bootstrap Strategy\n  ───────────────────────────────────────────"}`
**action:** replace
**text:** |2
    PHASE 5 — Implement KB Bootstrap Strategy
    ───────────────────────────────────────────
    Prerequisite: Phase 4 complete (unified pbcpb is in place; AdapterSession.bind
                  succeeds against the configured backend).
**rationale:** Sister edit to E24. Forward-references AdapterSession.bind (E02).
**compare_notes:** Same as E24.

---

## E25 — Adapter return-type schemas in interface contract
**from_d_id:** D25
**from_idea_id:** I28
**target:** _Subsumed by E01._
**action:** _no separate edit — see E01_
**text:** _E01 already incorporates the return-type schemas for query, populate, and scan_gaps inline as part of the discriminated-union work. Adding a second edit at the same site would be redundant and risk conflict._
**rationale:** D25 partially subsumed by D01 / E01; the consolidated edit at E01 is the cleanest place for return-type shapes. This entry is recorded for traceability but produces no separate edit.
**compare_notes:** Draft 1 wrote a separate adapter-return-types block immediately after the interface contract (independent of E01). Draft 2 consolidated into E01. Both verify, but Draft 2 (consolidated) wins on smaller surface area and avoids the conflict-resolver having to compose two adjacent edits.

---

## Dropped

_(no entries — all 25 D entries produced at least one verifying draft.)_

---

**Engineer summary:**
- d_entries_processed: 25
- e_entries_produced: 24 unique edits + 4 sister edits (E20b/c/d/e for the rename block; E24b/c/d for prerequisite annotations) = **31 total Node E edits** at integration time.
- E25 is a no-op pointer to E01 (subsumed; recorded for traceability).
- double_fail_count: 0
