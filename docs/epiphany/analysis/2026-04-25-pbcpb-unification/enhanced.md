<prompt>

<meta>
  <source>user-input: pbcpb unification and kb-adapter enhancement request</source>
  <version>enhanced-v1.1 | prompt-graph normal mode | audit-fixed</version>
  <fixes>C1 path-collision, C2 phase-ordering, H1 enum-consistency, H2 field-name,
         H3 output-scope, H4 scan_gaps-schema, M1 populate-delegation, M2 custom-api,
         M3 tier-criteria, M4 explore-step, M5 backlog-format, L1 verification, L2 header</fixes>
</meta>

<role>
  You are an AI implementation agent responsible for designing and implementing a unified,
  extended version of the PBCPB (Playbook Creator Playbook Builder) system. Your deliverable
  is working code and configuration files — not a design document. You will:
    1. Explore both existing codebases to understand their structure and language.
    2. Design a KB adapter interface and role system abstraction.
    3. Merge the Dify/MCP improvements from pbcpb-dify into the original pbcpb codebase,
       refactoring Dify-specific logic into the new adapter module.
    4. Implement a bootstrap strategy for KB population.
  Output: a runnable PBCPB system at ~/Documents/pbcpb-unified/, validated before replacing
  the original at ~/Documents/pbcpb/.
</role>

<context>

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

  <background>
    Goal: produce a unified, backend-agnostic PBCPB tool that integrates the
    Dify/MCP improvements from `pbcpb-dify` back into the original `pbcpb`,
    abstracts KB access behind a pluggable adapter interface (any backend the user
    chooses), and supports all human-only / AI-assisted / multi-agent role
    configurations declared at runtime.

    PBCPB (Playbook Creator Playbook Builder) is a JSON-based playbook creator
    tool. Two existing projects are present on the filesystem:

    - Original PBCPB (old-school database system):
        file:///home/myuser/Documents/pbcpb
        ~/Documents/pbcpb/
        Uses a conventional JSON/markdown database with master-index and cross-referencing system.

    - Updated Dify/MCP variant:
        file:///home/myuser/Documents/pbcpb-dify
        ~/Documents/pbcpb-dify
        Replaces the old-school database with a Dify-based RAG knowledge system accessed via
        MCP server. This is considered the improved KB intake architecture.

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

    The goal is to produce a unified PBCPB tool that:
    1. Integrates pbcpb-dify changes back to the main system.
    2. Extends KB support to any backend the user chooses — not locked to Dify or flat files.
    3. Accommodates all human and AI-based role system configurations ahead of time.

    The user will define the KB backend. All system configurations must be possible within
    this pbcpb system. The design intention is to accommodate for different scenarios ahead
    of time. Named systems: PBCPB, Dify, MCP, Mattermost, claude code, openclaw, codex.

    Staging and promotion strategy: all implementation work is staged to
    ~/Documents/pbcpb-unified/. The original ~/Documents/pbcpb/ is backed up before any
    modification. Only after validation does the unified build replace the original.
  </background>

  <dify_constraint>
    Constraint: The Dify RAG system requires uploading knowledge separately on the
                user's own initiative. PBCPB does not write directly to Dify.
    Affected adapters: RAG_MCP only.
    What populate() returns for RAG_MCP: a `{ kind: "manifest", ... }` object
                                         pointing to a generated upload manifest
                                         file (see the populate() return-type
                                         schema in `<kb_scope>` for the
                                         discriminated-union shape). No content
                                         is written to Dify.
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

  <kb_scope>
    The KB backend is a runtime configuration decision — the user selects which backend to
    use through a configuration file. The same PBCPB instance switches backends via config,
    with no code-level switch required.

    Supported KB backend types (adapter_type enum):

      RAG_MCP          — Dify MCP server or Dify API (RAG system).
      MARKDOWN_FOLDER  — Series of organized markdown files in structured folders. User must
                         provide their own skills allowing access to the knowledgebase. All
                         three adapter methods (query, populate, scan_gaps) are delegated to
                         the user-provided skill referenced by skill_ref.
      JSON_DB          — Old-school JSON, database, or markdown-database hybrid system.
      CUSTOM_API       — Any other user-provided knowledge API or custom adapter. The
                         adapter's populate() behavior is implementation-defined by the user;
                         the bootstrap phase invokes it but treats non-zero exit as a
                         configuration error, not an auto-populate failure.

    KB Adapter Interface Contract:
    Every adapter must expose these required_methods:

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

    schema_definition is a JSON document describing the expected KB structure. It specifies:
      - which topics/sections must exist (entry list with ids and categories)
      - required fields per entry (e.g., title, body, tier)
      - dependency relationships between entries (entry A depends on entry B)

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

    For a new KB, generate schema_definition from the PBCPB playbook's KB phase definitions.
    For an existing KB, derive it by inspecting the current master-index if one exists.

    User-configuration schema (PBCPB JSON config):
      {
        "kb_adapter": {
          "adapter_type": "RAG_MCP | MARKDOWN_FOLDER | JSON_DB | CUSTOM_API",
          "connection": { ... adapter-specific connection params ... },
          "skill_ref": "<optional: path or name of user-provided skill>",
          "bootstrap_tier_limit": 1
        }
      }

    Note: skill_ref is used consistently throughout. It is required when adapter_type is
    MARKDOWN_FOLDER; optional for CUSTOM_API; unused for RAG_MCP and JSON_DB.

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

    IMPORTANT — Dify upload constraint: The Dify RAG system requires uploading knowledge
    separately on the user's own initiative. The playbook auto-population path does NOT
    apply to RAG_MCP backends. Dify is a manual-upload-only path. The bootstrap phase
    produces a Dify upload manifest; the user applies it manually.

    For MARKDOWN_FOLDER backends: all KB access (query, populate, scan_gaps) is delegated
    to user-provided skills via skill_ref. PBCPB does not hardcode any markdown KB logic.
  </kb_scope>

  <role_scope>
    The PBCPB system must support all types of human and AI-based role systems. Roles are
    defined in the tool as named placeholders. Placeholder names can later be linked to
    arbitrary agent systems at runtime — this binding is not design-time.

    Supported role_type enum values (use these exact strings in JSON config):

      MULTI_AGENT  — Multiple AI agents with distinct roles, using @name-style addressing
                     (Mattermost-style @name syntax, or other syntax as defined by user).
                     Each role links to a specific agent system (claude code, openclaw,
                     codex, or any arbitrary system) via runtime config.

      HUMAN        — No AI agents. This role is filled by a human participant. A role
                     system where all roles are HUMAN constitutes a human-only configuration.

      AI_ASSISTED  — A human role augmented by AI assistance (claude code, openclaw, codex).

    Role system defined in PBCPB JSON config:
      {
        "role_system": {
          "syntax_mode": "@name | custom",
          "syntax_pattern": "<user-defined pattern if not @name>",
          "roles": [
            {
              "placeholder_name": "@architect",
              "role_type": "MULTI_AGENT | HUMAN | AI_ASSISTED",
              "linked_agent": null
            }
          ]
        }
      }

    Concrete examples:

    (a) Multi-agent:
      { "role_system": { "syntax_mode": "@name", "roles": [
          { "placeholder_name": "@planner",   "role_type": "MULTI_AGENT", "linked_agent": "claude-code-instance-1" },
          { "placeholder_name": "@executor",  "role_type": "MULTI_AGENT", "linked_agent": "openclaw-instance-1" },
          { "placeholder_name": "@reviewer",  "role_type": "MULTI_AGENT", "linked_agent": "codex-instance-1" }
      ]}}

    (b) Human-only (all roles are HUMAN):
      { "role_system": { "syntax_mode": "@name", "roles": [
          { "placeholder_name": "@lead",    "role_type": "HUMAN", "linked_agent": null },
          { "placeholder_name": "@support", "role_type": "HUMAN", "linked_agent": null }
      ]}}

    (c) AI-assisted:
      { "role_system": { "syntax_mode": "@name", "roles": [
          { "placeholder_name": "@human",     "role_type": "HUMAN",       "linked_agent": null },
          { "placeholder_name": "@assistant", "role_type": "AI_ASSISTED", "linked_agent": "claude-code" }
      ]}}

    All system configurations must be possible with this PBCPB JSON-based tool.
  </role_scope>

  <operational_constraints>
    - PBCPB must remain JSON-based throughout all backends and role configurations.
    - The user will define the KB backend; the system accommodates all defined variants.
    - Bootstrapping prioritizes core-necessary content first; detailed content comes after.
    - For text-based KBs (MARKDOWN_FOLDER, JSON_DB): the playbook helps populate the
      knowledge base by running a KB population phase which will scan the KB for gaps or
      placeholders and fill the KB with content. Other skills can harvest KB content and
      can be developed further as needed.
    - Role placeholders are linked to agent systems via runtime config, not design-time.
    - KB backend selection is a runtime configuration decision. No rebuild required.
    - All implementation is staged to ~/Documents/pbcpb-unified/ before replacing the
      original at ~/Documents/pbcpb/.
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
        (g) Locked-down or baked-configuration deployments where runtime
            adapter_type switching is structurally unavailable (container images
            with read-only baked config). The no-rebuild guarantee assumes the
            deployment supports runtime config-file modification.
  </operational_constraints>

</context>

<task>
  Execute the following 5 ordered phases sequentially. Do not begin a phase until the
  previous one is complete and validated.

  PHASE 1 — Explore and Catalogue Both Codebases
  ────────────────────────────────────────────────
  Prerequisite: none.
  Before any implementation, build a working model of both projects.

  1.1 Identify the primary language(s), runtime, and entry points of
      file:///home/myuser/Documents/pbcpb.
  1.2 Identify the primary language(s), runtime, and entry points of
      file:///home/myuser/Documents/pbcpb-dify.
  1.3 Map the top-level module/directory structure of each project. For pbcpb,
      additionally produce an explicit feature inventory — one row per
      user-facing feature: { name, entry point, expected behavior }. This
      inventory is the source-of-truth for Phase 4.6 validation ("retains all
      original pbcpb functionality"). Without it, Phase 4.6 is structurally
      non-executable.
  1.4 Identify any existing abstraction layer in pbcpb that KB access routes through
      (if one exists, the adapter interface will extend it rather than replace it).
  1.5 Diff file:///home/myuser/Documents/pbcpb vs file:///home/myuser/Documents/pbcpb-dify
      to enumerate all delta files and changes.
  1.6 Categorize delta items as:
        (a) Dify-specific additions — will move into RAG_MCP adapter module
        (b) general improvements applicable to all backends — will apply to pbcpb base
        (c) removals/regressions — exclude from merge
  1.7 If a master-index exists in pbcpb, build its dependency graph (entries as
      nodes; declared dependencies as directed edges) and run cycle detection. For
      each strongly connected component found, record { scc_group_id, member
      entry_ids } in phase1-catalogue.md. This output feeds Phase 5 step 5.2
      (tier-scoring algorithm selection: DAG topological sort vs. SCC-aware
      Kosaraju path).
  Output: a written catalogue (phase1-catalogue.md in staging dir) listing findings
  from 1.1–1.7 before proceeding to Phase 2.

  PHASE 2 — Design KB Adapter Interface
  ───────────────────────────────────────
  Prerequisite: Phase 1 complete (phase1-catalogue.md produced, including the
                feature inventory from 1.3 and any cyclic-master-index findings
                from 1.7).
  Design first, implement after. This phase must complete before Phase 4 begins.

  2.1 Define the adapter_type enum: RAG_MCP | MARKDOWN_FOLDER | JSON_DB | CUSTOM_API.
  2.2 Define the three-method adapter interface contract:
        query(query_string, filters?) → results
        populate(content, tier, metadata) → status
        scan_gaps(schema_definition) → gap_list
  2.3 For RAG_MCP: populate() produces a Dify upload manifest (no auto-populate).
      query() and scan_gaps() call the Dify MCP server or Dify API.
  2.4 For MARKDOWN_FOLDER: all three methods delegate to the user-provided skill
      referenced by skill_ref. If skill_ref is absent, raise a configuration error.
  2.5 For JSON_DB: implement all three methods directly against the existing old-school
      database used in file:///home/myuser/Documents/pbcpb.
  2.6 For CUSTOM_API: configurable passthrough to user-defined endpoints. populate()
      result is implementation-defined by the user's API; non-zero exit is a hard error.
  2.7 Define user-configuration schema (kb_adapter block with skill_ref field).
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
  2.8 KB backend selection is a runtime config decision — same PBCPB instance switches
      backends via config file. No code-level branching at the business-logic layer.

  PHASE 3 — Design Role System Abstraction
  ──────────────────────────────────────────
  Prerequisite: Phase 1 complete. (Phase 3 is independent of Phase 2 design and
                may run in parallel.)
  3.1 Define role_type enum: MULTI_AGENT | HUMAN | AI_ASSISTED.
  3.2 Implement syntax_mode: @name (Mattermost-style) and custom (user-defined pattern).
      Syntax is defined by user; PBCPB does not impose a specific syntax.
  3.3 Each role is stored as a placeholder_name in PBCPB JSON. The placeholder can later
      be linked to an arbitrary agent system (claude code, openclaw, codex, or any
      user-provided system) via linked_agent field at runtime.
  3.4 Implement role binding: design-time = named placeholders; runtime = linked_agent
      field resolved from config. No design-time code dependency on agent type.
  3.5 Implement and validate all three configuration variants using role_scope examples
      as reference schemas.
  3.6 Confirm all role configurations work across all KB backend types (matrix test).

  PHASE 4 — Execute Merge with Backup and Staging
  ─────────────────────────────────────────────────
  Runs after Phases 2 and 3 are complete. The RAG_MCP adapter module must exist before
  step 4.4 can refactor Dify-specific logic into it.

  4.0 Back up the original: cp -r ~/Documents/pbcpb ~/Documents/pbcpb-original-backup
  4.1 Create staging directory: ~/Documents/pbcpb-unified/
  4.2 Copy pbcpb base into staging directory as the starting point.
  4.3 Apply category (b) general improvements (from Phase 1.6) to the staging copy.
  4.4 Refactor category (a) Dify-specific logic into the RAG_MCP adapter module
      (designed in Phase 2) within the staging copy.
  4.5 Integrate the KB adapter interface and role system abstraction (Phases 2 and 3)
      into the staging copy.
  4.6 Validate that ~/Documents/pbcpb-unified/ retains all original pbcpb functionality
      (JSON_DB path works) plus the Dify/MCP improvements (RAG_MCP adapter works).
  4.7 On successful validation: rename ~/Documents/pbcpb/ →
      ~/Documents/pbcpb-superseded/ and rename ~/Documents/pbcpb-unified/ →
      ~/Documents/pbcpb/.

  PHASE 5 — Implement KB Bootstrap Strategy
  ───────────────────────────────────────────
  Prerequisite: Phase 4 complete (unified pbcpb is in place; AdapterSession.bind
                succeeds against the configured backend).
  Directive: Scan the KB for gaps or placeholders and fill the KB with some content.
  Strategy: Focus on bootstrapping and beginning to fill with the most core necessary
  content first. More detailed content comes after bootstrapping.

  CoT Bootstrap Sequence (steps 5.1 → 5.5):

    5.1 — scan_gaps:
      Generate or load schema_definition (see kb_scope for format). For a new KB, derive
      from PBCPB playbook KB phase definitions. For an existing KB, derive from master-index.
      Invoke adapter.scan_gaps(schema_definition).
      Output: raw gap list — each item has: { gap_id, location, type, schema_node }
      where type ∈ { missing | placeholder | incomplete }.

    5.2 — tier_scoring:
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

      Optional: Tier 0 (foundational cross-cutting concerns; opt-in). When
      `bootstrap_tier_zero_threshold` is set in `kb_adapter` config to a
      positive integer N, any entry with in-degree ≥ N is promoted to Tier 0
      and bootstrapped strictly before Tier 1. Default value 0 disables Tier 0
      (all behavior unchanged from above). Use case: schemas with shared
      glossary or error-code-catalog entries referenced by most other entries.
      A negative or non-integer threshold raises ConfigError at bind() time.

      Sort gap list ascending by tier, then alphabetically by gap_id within
      each tier.

    5.3 — populate_core:
      For Tier 1 gaps only: invoke adapter.populate(content, tier=1, metadata) using
      available KB population skills and harvesting tools.
      For RAG_MCP: skip auto-populate — generate Dify upload manifest entries for Tier 1
      items instead (consumed in Step 5).

    5.4 — defer_tier2_plus:
      Record all Tier 2+ gaps in the deferred backlog file:
        Path: ~/Documents/pbcpb/bootstrap/deferred-backlog.json
        Schema:
          {
            "generated_at": "<ISO-8601 timestamp>",
            "tier_limit_used": 1,
            "deferred": [
              {
                "gap_id": "<string>",
                "location": "<string>",
                "type": "missing | placeholder | incomplete",
                "tier": 2,
                "schema_node": "<string>"
              }
            ]
          }
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

    5.5 — dify_upload_manifest (conditional, only when adapter_type == RAG_MCP):
      Emit structured Dify upload manifest at:
        ~/Documents/pbcpb/bootstrap/dify-upload-manifest.json
      Listing all Tier 1 content items with target KB locations, ready for manual upload.
      The Dify RAG system requires uploading knowledge separately on the user's own
      initiative — this manifest is the artifact for that manual process.

  Conditional logic:
    - if text KB (MARKDOWN_FOLDER / JSON_DB / CUSTOM_API): Steps 1–4 auto-populate via adapter.
    - if Dify RAG (RAG_MCP): Steps 1–2 scan and score; Step 3 generates manifest only;
      Step 5 emits the manifest file. No auto-populate.
    - if user provides own skills (MARKDOWN_FOLDER): all three adapter methods are
      delegated to those skills via skill_ref. Bootstrap calls the same interface.
    - if CUSTOM_API and populate() returns a non-zero exit: halt with configuration error;
      do not silently continue.
</task>

<constraints>
  Constraint table (7 constraints):

  KB_BACKEND_SUPPORT
    Must support: RAG_MCP (Dify MCP server or Dify API), MARKDOWN_FOLDER, JSON_DB,
    CUSTOM_API.

  KB_POPULATION
    Text KBs (MARKDOWN_FOLDER, JSON_DB, CUSTOM_API): playbook auto-bootstrap, core-first
    (Tier 1 before Tier 2+).
    Dify RAG (RAG_MCP): separate manual upload only. Playbook produces manifest; user
    uploads manually.

  ROLE_SYSTEM
    Must support role_type values: MULTI_AGENT (@name or custom syntax), HUMAN (used for
    human-only configurations — all roles HUMAN), AI_ASSISTED (claude code, openclaw,
    codex, any user system). These are the exact enum strings used in JSON config.

  ROLE_BINDING
    Roles defined as named placeholders at design time. Linked to arbitrary agent systems
    via runtime config, not design-time. Roles are already defined in the tool, held by a
    placeholder name, which can later be linked to arbitrary systems.

  FORMAT
    PBCPB must remain JSON-based throughout.

  USER_DELEGATION
    MARKDOWN_FOLDER KB access requires user-provided skills. User will have to provide
    their own skills allowing access to the knowledgebase. All three adapter methods
    (query, populate, scan_gaps) are delegated via skill_ref. PBCPB invokes them;
    it does not implement markdown KB logic directly.

  STATELESS_INTERFACE
    The 3-method adapter contract is stateless across calls. Any session state
    (auth tokens, open cursors, streaming connections, retained results) MUST be
    encapsulated within the adapter implementation behind the `connection` config
    block — never exposed through method signatures and never shared between
    successive method invocations. Stateless invocation is a precondition for
    runtime backend switching: the same logical KB call must produce equivalent
    results regardless of when the AdapterSession was bound.

  CALLER_INVARIANCE
    Business-logic code MUST NOT import or reference any backend-specific module,
    class, or type. All KB access goes through the AdapterSession handle's
    three-method surface (query, populate, scan_gaps) only. Statically enforceable
    via lint or import-graph rules; verified in CI. This is a precondition for the
    runtime-config-driven dispatch model: a single backend-specific import in
    business logic invalidates the no-rebuild guarantee.

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
    item (g)).
</constraints>

<output_format>
  Deliverable: working code and configuration files, not a design document.

  The implementation must produce a unified PBCPB system with the following four
  architectural sections present in the codebase:

  SECTION 1 — System Overview (README or equivalent)
    High-level description of the unified PBCPB system, its two source projects
    (pbcpb at file:///home/myuser/Documents/pbcpb and pbcpb-dify at
    file:///home/myuser/Documents/pbcpb-dify), the merge rationale, and the unified target
    at ~/Documents/pbcpb/. Summary of supported KB backend types and role system types.

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

  SECTION 2 — KB Adapter Module (code + config schema)
    For each adapter_type (RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API):
      - Adapter implementation or delegation stub
      - Required configuration fields documented
      - populate() behavior documented (auto vs. manifest-only for Dify)
      - scan_gaps() behavior documented
      - Example kb_adapter JSON config snippet in docs/

  SECTION 3 — Role System Module (code + config schema)
    For each role_type (MULTI_AGENT, HUMAN, AI_ASSISTED):
      - Role system implementation
      - syntax_mode support (@name and custom)
      - placeholder_name / linked_agent binding logic
      - Example role_system JSON config snippet in docs/

  SECTION 4 — Bootstrap Module (code + output artifacts)
    Implementation of the 5-step CoT bootstrap sequence:
      scan_gaps → tier_scoring → populate_core → defer_tier2_plus → dify_upload_manifest
    Produces deferred-backlog.json and (conditionally) dify-upload-manifest.json.
</output_format>

<edge_cases>
  EC-1 — Dify RAG upload-only path:
    When adapter_type == RAG_MCP, populate() must NOT attempt to auto-write content into
    Dify. It generates a structured upload manifest only. User applies manually. This path
    is categorically separate from the text-KB auto-populate path.

  EC-2 — Markdown KB skill delegation failure:
    When adapter_type == MARKDOWN_FOLDER and skill_ref is absent or the referenced skill
    is unavailable, the system must fail with a clear configuration error — not silently
    degrade. All three adapter methods require the skill; none fall back to direct access.

  EC-3 — Role placeholder with null linked_agent:
    A placeholder with linked_agent == null is valid (unlinked; human executes manually).
    Linking a placeholder requires only updating linked_agent in config — no schema
    migration or code change. Roles are already defined in the tool, held by a placeholder
    name, which can later be linked to arbitrary systems.

  EC-4 — Mixed role configurations:
    A single playbook may contain MULTI_AGENT, HUMAN, and AI_ASSISTED roles simultaneously.
    All role_type values must coexist in the same role_system block without conflict.

  EC-5 — Bootstrap on empty KB:
    If scan_gaps() returns the entire KB schema as gaps, bootstrap still applies: populate
    Tier 1 only. A large deferred backlog is expected behavior, not an error condition.

  EC-6 — CUSTOM_API populate() failure:
    When adapter_type == CUSTOM_API and populate() returns a non-zero exit or error, halt
    the bootstrap phase with a configuration error. Do not silently skip or continue.
    CUSTOM_API's populate() behavior is user-defined and not guaranteed to be writable;
    the user must configure their API to accept populate() calls before running bootstrap.

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
</edge_cases>

<verification>
  Implementation success criteria — runtime checks, not prompt structure checks:

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
         "manifest", distinguishable from a write-status return.
         (KB_POPULATION, EC-1)
  [ ] 7. Role system config with role_type MULTI_AGENT, HUMAN, and AI_ASSISTED
         all parse and operate correctly; mixed configurations coexist without
         conflict. (ROLE_SYSTEM, EC-4)
  [ ] 8. A placeholder with linked_agent == null does not cause an error;
         updating linked_agent in config takes effect without rebuild.
         (ROLE_BINDING, EC-3)
  [ ] 9. Bootstrap Phase 5 produces deferred-backlog.json at the defined path
         and schema. (KB_POPULATION)
  [ ] 10. Bootstrap Tier 1 scoring correctly identifies entries with ≥2
          dependents or required: true in schema_definition as Tier 1.
          (KB_POPULATION)
  [ ] 11. CUSTOM_API adapter raises a hard error (not silent skip) when
          populate() fails. (KB_BACKEND_SUPPORT, EC-6)
  [ ] 12. The role_system parser rejects pseudo-values for role_type (HUMAN_ONLY,
          MULTIAGENT, ai-assisted, etc.) with a structured ConfigError at
          AdapterSession.bind() time; only the exact strings MULTI_AGENT, HUMAN,
          AI_ASSISTED parse successfully. The same exact-string rule applies to
          adapter_type — only RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API parse
          successfully; pseudo-values (RAGMCP, markdown-folder, etc.) raise
          ConfigError. (ROLE_SYSTEM, KB_BACKEND_SUPPORT)
  [ ] 13. Invalid adapter_type or role_type values (e.g. "RAGMCP",
          "markdown-folder", "HUMAN_ONLY") in config are rejected at
          AdapterSession.bind() with a structured ConfigError that names the
          offending field; the dispatch layer is never reached. (KB_BACKEND_SUPPORT,
          ROLE_SYSTEM)
  [ ] 14. A cyclic schema_definition (entry A depends on B; B depends on A) is
          handled deterministically by Phase 5 step 5.2 — either rejected with a
          structured CyclicSchemaError, OR every member of the cycle is jointly
          promoted to Tier 1 via Kosaraju SCC decomposition. Silent topological-
          sort divergence MUST NOT occur. (KB_POPULATION)
</verification>

</prompt>
