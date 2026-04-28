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

  <background>
    PBCPB is a JSON-based playbook creator tool (Playbook Creator Playbook Builder). Two
    existing projects are present on the filesystem:

    - Original PBCPB (old-school database system):
        file:///home/myuser/Documents/pbcpb
        ~/Documents/pbcpb/
        Uses a conventional JSON/markdown database with master-index and cross-referencing system.

    - Updated Dify/MCP variant:
        file:///home/myuser/Documents/pbcpb-dify
        ~/Documents/pbcpb-dify
        Replaces the old-school database with a Dify-based RAG knowledge system accessed via
        MCP server. This is considered the improved KB intake architecture.

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
      populate(content, tier, metadata)   — Write or upload content into the KB.
      scan_gaps(schema_definition)        — Scan KB for gaps/placeholders; return gap list.

    schema_definition is a JSON document describing the expected KB structure. It specifies:
      - which topics/sections must exist (entry list with ids and categories)
      - required fields per entry (e.g., title, body, tier)
      - dependency relationships between entries (entry A depends on entry B)
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
  </operational_constraints>

</context>

<task>
  Execute the following 5 ordered phases sequentially. Do not begin a phase until the
  previous one is complete and validated.

  PHASE 1 — Explore and Catalogue Both Codebases
  ────────────────────────────────────────────────
  Before any implementation, build a working model of both projects.

  1.1 Identify the primary language(s), runtime, and entry points of
      file:///home/myuser/Documents/pbcpb.
  1.2 Identify the primary language(s), runtime, and entry points of
      file:///home/myuser/Documents/pbcpb-dify.
  1.3 Map the top-level module/directory structure of each project.
  1.4 Identify any existing abstraction layer in pbcpb that KB access routes through
      (if one exists, the adapter interface will extend it rather than replace it).
  1.5 Diff file:///home/myuser/Documents/pbcpb vs file:///home/myuser/Documents/pbcpb-dify
      to enumerate all delta files and changes.
  1.6 Categorize delta items as:
        (a) Dify-specific additions — will move into RAG_MCP adapter module
        (b) general improvements applicable to all backends — will apply to pbcpb base
        (c) removals/regressions — exclude from merge
  Output: a written catalogue (phase1-catalogue.md in staging dir) listing findings
  from 1.1–1.6 before proceeding to Phase 2.

  PHASE 2 — Design KB Adapter Interface
  ───────────────────────────────────────
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
  2.8 KB backend selection is a runtime config decision — same PBCPB instance switches
      backends via config file. No code-level branching at the business-logic layer.

  PHASE 3 — Design Role System Abstraction
  ──────────────────────────────────────────
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
  Directive: Scan the KB for gaps or placeholders and fill the KB with some content.
  Strategy: Focus on bootstrapping and beginning to fill with the most core necessary
  content first. More detailed content comes after bootstrapping.

  CoT Bootstrap Sequence:

    STEP 1 — scan_gaps:
      Generate or load schema_definition (see kb_scope for format). For a new KB, derive
      from PBCPB playbook KB phase definitions. For an existing KB, derive from master-index.
      Invoke adapter.scan_gaps(schema_definition).
      Output: raw gap list — each item has: { gap_id, location, type, schema_node }
      where type ∈ { missing | placeholder | incomplete }.

    STEP 2 — tier_scoring:
      Score each gap using these criteria:
        Tier 1 (core-necessary): gap_id is referenced by ≥2 other entries in schema_definition,
                                 OR the schema_definition marks it required: true,
                                 OR it represents a foundational node (glossary, key process
                                 definitions) that other entries cite as a dependency.
        Tier 2 (important, deferrable): referenced by exactly 1 other entry; no required flag.
        Tier 3+ (enrichment): leaf node — zero dependents, no required flag.
      Sort gap list ascending by tier, then alphabetically by gap_id within each tier.

    STEP 3 — populate_core:
      For Tier 1 gaps only: invoke adapter.populate(content, tier=1, metadata) using
      available KB population skills and harvesting tools.
      For RAG_MCP: skip auto-populate — generate Dify upload manifest entries for Tier 1
      items instead (consumed in Step 5).

    STEP 4 — defer_tier2_plus:
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
      These entries are not populated during bootstrap. Detailed content population comes
      after bootstrap phase completes by re-running Phase 5 with a higher tier_limit.

    STEP 5 — dify_upload_manifest (conditional, only when adapter_type == RAG_MCP):
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

  RUNTIME_CONFIG
    KB backend selection is a runtime configuration decision, NOT a code-level switch.
    Same PBCPB instance switches backends via config file. No rebuild required to change
    KB backend. Enables zero-rebuild KB migration and multi-user deployment with different
    KB backends on the same tool version.
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
</edge_cases>

<verification>
  Implementation success criteria — runtime checks, not prompt structure checks:

  [ ] 1. ~/Documents/pbcpb-original-backup/ exists and contains the unmodified original
         before any changes are applied to ~/Documents/pbcpb/.
  [ ] 2. ~/Documents/pbcpb/ is the final unified codebase (pbcpb-unified/ was renamed
         into place after validation).
  [ ] 3. Switching kb_adapter.adapter_type in config (without code changes) routes KB
         operations to the correct adapter implementation at runtime.
  [ ] 4. All three adapter methods (query, populate, scan_gaps) are callable for each
         of the four adapter types: RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API.
  [ ] 5. MARKDOWN_FOLDER adapter raises a configuration error when skill_ref is absent.
  [ ] 6. RAG_MCP adapter's populate() produces a dify-upload-manifest.json and does not
         write directly to Dify.
  [ ] 7. Role system config with role_type MULTI_AGENT, HUMAN, and AI_ASSISTED all parse
         and operate correctly; mixed configurations coexist without conflict.
  [ ] 8. A placeholder with linked_agent == null does not cause an error; updating
         linked_agent in config takes effect without rebuild.
  [ ] 9. Bootstrap Phase 5 produces deferred-backlog.json at the defined path and schema.
  [ ] 10. Bootstrap Tier 1 scoring correctly identifies entries with ≥2 dependents or
          required: true in schema_definition as Tier 1.
  [ ] 11. CUSTOM_API adapter raises a hard error (not silent skip) when populate() fails.
  [ ] 12. All role_type JSON values in config use the exact enum strings MULTI_AGENT |
          HUMAN | AI_ASSISTED; HUMAN_ONLY is not a valid enum value.
</verification>

</prompt>
