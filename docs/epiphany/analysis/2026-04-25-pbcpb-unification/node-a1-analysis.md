# Node A1 — Full Analysis

## Intent

Node A is an executable instruction prompt for an AI implementation agent. It directs the agent to produce a unified, runnable PBCPB (Playbook Creator Playbook Builder) system at `~/Documents/pbcpb-unified/` that:

1. Merges the Dify/MCP improvements from `~/Documents/pbcpb-dify` into the original `~/Documents/pbcpb` codebase.
2. Refactors Dify-specific logic into a pluggable KB adapter module supporting four backend types (RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API).
3. Implements a role system abstraction supporting MULTI_AGENT, HUMAN, and AI_ASSISTED roles via runtime placeholder→agent binding.
4. Implements a 5-step bootstrap strategy that scans the KB for gaps, scores them by tier, populates Tier 1 content (or generates a Dify upload manifest for RAG_MCP), and defers Tier 2+ to a backlog.
5. Promotes the staged unified build over the original at `~/Documents/pbcpb/` only after validation.

The intended audience is a code-writing AI agent with filesystem and shell access; the intended consumer of the resulting system is a human user configuring PBCPB for their own KB and role context. The deliverable is **working code and configuration files, not a design document** (stated in `<output_format>`).

## Structure

The prompt is XML-tagged and organized into the following top-level blocks:

- `<meta>` — version + audit-fix tracking (enhanced-v1.1, lists C1/C2/H1–H4/M1–M5/L1–L2 fixes).
- `<role>` — declares the agent's responsibility (5 numbered duties) and final deliverable target.
- `<context>` — three sub-blocks: `<background>` (project filesystem layout + staging strategy), `<kb_scope>` (adapter_type enum, interface contract, schema_definition shape, kb_adapter JSON config schema, Dify upload constraint, MARKDOWN_FOLDER delegation rule), `<role_scope>` (role_type enum, syntax_mode, role_system JSON schema, three concrete examples a/b/c), and `<operational_constraints>` (7 bullet rules).
- `<task>` — 5 sequentially ordered phases (Phase 1 explore, Phase 2 KB adapter design, Phase 3 role design, Phase 4 merge with backup/staging, Phase 5 KB bootstrap CoT 5-step sequence with conditional logic).
- `<constraints>` — 7-row constraint table (KB_BACKEND_SUPPORT, KB_POPULATION, ROLE_SYSTEM, ROLE_BINDING, FORMAT, USER_DELEGATION, RUNTIME_CONFIG).
- `<output_format>` — 4 architectural sections required in the codebase (system overview, KB adapter module, role system module, bootstrap module).
- `<edge_cases>` — 6 edge cases (EC-1 Dify upload-only, EC-2 Markdown delegation failure, EC-3 null linked_agent, EC-4 mixed roles, EC-5 empty KB bootstrap, EC-6 CUSTOM_API populate failure).
- `<verification>` — 12-item runtime success-criteria checklist (file existence, runtime dispatch, all four adapter types callable, ConfigError on missing skill_ref, manifest-only RAG_MCP, mixed roles, null linked_agent, deferred-backlog, Tier 1 scoring rule, CUSTOM_API hard error, exact enum strings).

Heading hierarchy is two-level XML tags; phase definitions inside `<task>` use unicode horizontal-rule banners (e.g. `────`) and `STEP 1 — scan_gaps:` style sub-headers. Bullet lists, fenced code blocks, and inline ` quoting are used throughout.

## Claims

- PBCPB is a JSON-based playbook creator tool.
- Two existing on-disk projects exist: `~/Documents/pbcpb` (old-school JSON/markdown DB with master-index) and `~/Documents/pbcpb-dify` (Dify RAG/MCP variant).
- The pbcpb-dify variant is "considered the improved KB intake architecture."
- The unified tool must integrate pbcpb-dify changes back into the main system.
- KB support must extend to any backend the user chooses, not locked to Dify or flat files.
- All system configurations must be possible within the unified PBCPB system.
- The user defines the KB backend.
- Named systems referenced: PBCPB, Dify, MCP, Mattermost, claude code, openclaw, codex.
- The KB backend is a runtime configuration decision; the same PBCPB instance switches backends via config file with no code-level switch.
- The Dify RAG system requires uploading knowledge separately on the user's own initiative; the playbook auto-population path does not apply to RAG_MCP backends.
- For MARKDOWN_FOLDER backends, all KB access (query, populate, scan_gaps) is delegated to user-provided skills via skill_ref.
- The PBCPB system must support all human and AI-based role systems.
- Roles are defined as named placeholders at design time; `linked_agent` binds them to arbitrary agent systems at runtime.
- Bootstrapping prioritizes core-necessary content first; detailed content comes after.
- All implementation is staged to `~/Documents/pbcpb-unified/` before replacing the original at `~/Documents/pbcpb/`.
- `~/Documents/pbcpb/` is backed up to `~/Documents/pbcpb-original-backup/` before any modification.
- After validation, `~/Documents/pbcpb/` is renamed to `~/Documents/pbcpb-superseded/` and `~/Documents/pbcpb-unified/` is renamed to `~/Documents/pbcpb/`.
- Tier 1 = entries with ≥2 dependents OR `required: true` OR foundational nodes that other entries cite as a dependency.
- Tier 2 = referenced by exactly 1 other entry; no required flag.
- Tier 3+ = leaf node with zero dependents and no required flag.
- For RAG_MCP, Step 3 of bootstrap skips auto-populate and emits manifest entries; Step 5 emits the manifest file.
- HUMAN_ONLY is **not** a valid enum value (verification item 12) — human-only configurations are expressed by setting all roles to `HUMAN`.
- A placeholder with `linked_agent == null` is valid (unlinked; human executes manually).
- A single playbook may contain MULTI_AGENT, HUMAN, and AI_ASSISTED roles simultaneously.

## Constraints

This is the no-regression reference set. Every MUST / MUST-NOT / rule / invariant from Node A:

1. **MUST:** PBCPB remains JSON-based throughout all backends and role configurations.
2. **MUST:** Support adapter_type enum {RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API} as the exact strings.
3. **MUST:** Every adapter exposes `query(query_string, filters?)`, `populate(content, tier, metadata)`, and `scan_gaps(schema_definition)` as required_methods.
4. **MUST NOT:** Hardcode markdown KB logic for MARKDOWN_FOLDER — all three methods delegate to user-provided skill via skill_ref.
5. **MUST:** RAG_MCP `populate()` produces a Dify upload manifest, not a direct write.
6. **MUST:** The user applies the Dify upload manifest manually (separate manual upload only).
7. **MUST:** Support role_type enum {MULTI_AGENT, HUMAN, AI_ASSISTED} as the exact strings.
8. **MUST NOT:** Use HUMAN_ONLY as an enum value (it is not valid).
9. **MUST:** Roles defined as named placeholders at design time; `linked_agent` binding is runtime, not design-time.
10. **MUST:** Bootstrap prioritizes core-necessary (Tier 1) content first; detailed content comes after.
11. **MUST:** All implementation is staged to `~/Documents/pbcpb-unified/` before replacing the original.
12. **MUST:** The original `~/Documents/pbcpb/` is backed up to `~/Documents/pbcpb-original-backup/` before any modification.
13. **MUST:** KB backend selection is a runtime configuration decision, no rebuild required, no code-level branching at the business-logic layer.
14. **MUST:** Phase ordering is strictly sequential — do not begin a phase until the previous one is complete and validated.
15. **MUST:** Phase 4 step 4.4 (refactor Dify-specific logic into RAG_MCP adapter) cannot run until the RAG_MCP adapter module exists (Phase 2 complete).
16. **MUST:** `skill_ref` is required when `adapter_type == MARKDOWN_FOLDER`; optional for CUSTOM_API; unused for RAG_MCP and JSON_DB.
17. **MUST:** When adapter_type == MARKDOWN_FOLDER and skill_ref is absent or referenced skill is unavailable, fail with a clear configuration error — not silent degradation.
18. **MUST:** When adapter_type == CUSTOM_API and populate() returns non-zero exit or error, halt the bootstrap phase with a configuration error — do not silently skip or continue.
19. **MUST:** A placeholder with `linked_agent == null` does not cause an error.
20. **MUST:** Updating `linked_agent` in config takes effect without rebuild.
21. **MUST:** Mixed role configurations (MULTI_AGENT + HUMAN + AI_ASSISTED simultaneously) coexist in the same role_system block without conflict.
22. **MUST:** Bootstrap Phase 5 produces `deferred-backlog.json` at `~/Documents/pbcpb/bootstrap/deferred-backlog.json` with the defined schema (generated_at ISO-8601, tier_limit_used, deferred[]).
23. **MUST:** When adapter_type == RAG_MCP, Phase 5 Step 5 emits `~/Documents/pbcpb/bootstrap/dify-upload-manifest.json` listing all Tier 1 content items with target KB locations.
24. **MUST:** Tier 1 scoring identifies entries with ≥2 dependents, OR `required: true`, OR foundational nodes other entries cite as a dependency.
25. **MUST:** Sort gap list ascending by tier, then alphabetically by gap_id within each tier.
26. **MUST:** Phase 4.6 validate that `~/Documents/pbcpb-unified/` retains all original pbcpb functionality (JSON_DB path works) plus the Dify/MCP improvements (RAG_MCP adapter works) before promotion.
27. **MUST:** Output is working code and configuration files, not a design document.
28. **MUST:** The four architectural sections (system overview, KB adapter module, role system module, bootstrap module) must be present in the codebase.
29. **MUST:** Phase 1 produces `phase1-catalogue.md` in the staging dir before Phase 2 begins.
30. **MUST:** Category (a) Dify-specific items move into RAG_MCP adapter; category (b) general improvements apply to pbcpb base; category (c) removals/regressions are excluded from merge.
31. **MUST:** Tier 2+ gaps are recorded in deferred-backlog.json and not populated during bootstrap; detailed content population comes after by re-running Phase 5 with a higher tier_limit.

## Gaps

1. **No test-execution definition for Phase 4.6 validation.** The verification item 4 says "all three adapter methods are callable for each of the four adapter types" but Node A does not state how callable-ness is demonstrated, what fixture data each adapter is exercised against, or what test framework / test runner is used. The required pre-existence of a "feature inventory" of pbcpb to compare against is not made explicit.
2. **No `populate()` return-shape specification.** Node A states `populate(...) → status` and that RAG_MCP returns a manifest while text KBs auto-write, but does not specify what `status` looks like as a value (success boolean? exit code? structured object?), nor whether RAG_MCP's manifest-emission semantically distinguishes itself in the return value so a uniform-treating caller would notice.
3. **No `schema_definition` cycle-handling specification.** Tier 1 is defined as `indegree ≥ 2 ∨ required:true ∨ foundational-cited`, but Node A does not specify behavior when `schema_definition` contains a cycle — e.g., entry A depends on B which depends on A. Topological sort diverges; the spec is silent.
4. **No PBCPB-internal failure handling.** Node A defines hard-error behavior at user-owned boundaries (MARKDOWN_FOLDER missing skill_ref → ConfigError; CUSTOM_API non-zero → halt) but is silent on PBCPB-internal failures: JSON_DB connection refused, MCP server timeout, malformed config JSON, invalid `adapter_type` value (typo).
5. **No config-validation gate.** Node A does not specify when or how the `kb_adapter` config block is validated. An invalid `adapter_type` value (e.g. "RAGMCP") could reach the dispatch site at first KB operation rather than at startup.
6. **No bootstrap idempotency rule.** Node A does not specify whether re-running Phase 5 against an already-partially-populated KB skips already-populated entries, replaces them, or fails.
7. **No agent lifecycle model.** `linked_agent` binds a placeholder to an agent at runtime, but Node A does not specify how a crashed or unresponsive `linked_agent` is detected, recovered from, or signaled to PBCPB.
8. **No multi-KB / migration model.** A user migrating from JSON_DB to RAG_MCP has no defined transition (dual-write, partial sync, cutover) — only a config-token swap is described.
9. **No transaction / atomicity primitive.** No method spans across `query` / `populate` / `scan_gaps` atomically; if a use case requires "scan_gaps and then populate atomically" no primitive is specified.
10. **No `gap_id` namespace coherence rule.** `scan_gaps()` returns `{ gap_id, location, type, schema_node }`, but Node A does not require that an adapter's `gap_id` match the `schema_definition`'s `entry.id`. RAG_MCP returning Dify-document-IDs while `schema_definition` uses logical IDs would silently corrupt tier scoring.
11. **No CUSTOM_API role disambiguation.** CUSTOM_API is described both as "any other user-provided knowledge API" (intended backend) and as the catch-all for unnamed backend types. The same configuration path serves both, but their semantic guarantees differ (a real custom API has known shape; an extension placeholder may not).
12. **Unicode horizontal-rule banners (`────`) in `<task>` are visual but not semantically structural** — could be lost in some markdown processors.

## Technical details

- **Filesystem paths used:**
  - Inputs: `~/Documents/pbcpb`, `~/Documents/pbcpb-dify`
  - Backup: `~/Documents/pbcpb-original-backup`
  - Staging: `~/Documents/pbcpb-unified/`
  - Final: `~/Documents/pbcpb/`
  - Superseded original (post-promotion): `~/Documents/pbcpb-superseded/`
  - Bootstrap artifacts: `~/Documents/pbcpb/bootstrap/deferred-backlog.json`, `~/Documents/pbcpb/bootstrap/dify-upload-manifest.json`
  - Staging catalogue: `phase1-catalogue.md` in staging dir.
- **Adapter interface signature:**
  - `query(query_string, filters?) → results`
  - `populate(content, tier, metadata) → status`
  - `scan_gaps(schema_definition) → gap_list`
- **Adapter behavior matrix:**
  - RAG_MCP: query/scan_gaps call Dify MCP server or Dify API; populate produces manifest.
  - MARKDOWN_FOLDER: all three delegate via skill_ref.
  - JSON_DB: all three direct against existing old-school DB.
  - CUSTOM_API: configurable passthrough; populate non-zero exit = hard halt.
- **kb_adapter JSON config schema:**
  ```json
  {
    "kb_adapter": {
      "adapter_type": "RAG_MCP | MARKDOWN_FOLDER | JSON_DB | CUSTOM_API",
      "connection": { "...": "adapter-specific" },
      "skill_ref": "<optional>",
      "bootstrap_tier_limit": 1
    }
  }
  ```
- **role_system JSON config schema:**
  ```json
  {
    "role_system": {
      "syntax_mode": "@name | custom",
      "syntax_pattern": "<custom pattern if not @name>",
      "roles": [
        { "placeholder_name": "@architect", "role_type": "MULTI_AGENT | HUMAN | AI_ASSISTED", "linked_agent": null }
      ]
    }
  }
  ```
- **Bootstrap CoT sequence (5 steps):**
  1. scan_gaps(schema_definition) → raw gap list `{ gap_id, location, type, schema_node }`, type ∈ {missing, placeholder, incomplete}
  2. tier_scoring: classify each gap into Tier 1/2/3+
  3. populate_core: invoke `adapter.populate(content, tier=1, metadata)` (or manifest-only for RAG_MCP)
  4. defer_tier2_plus: write deferred-backlog.json
  5. dify_upload_manifest (conditional on RAG_MCP)
- **deferred-backlog.json schema:**
  ```json
  {
    "generated_at": "<ISO-8601>",
    "tier_limit_used": 1,
    "deferred": [
      { "gap_id": "<string>", "location": "<string>", "type": "missing|placeholder|incomplete", "tier": 2, "schema_node": "<string>" }
    ]
  }
  ```
- **Verification checklist (12 items, all runtime checks not prompt-structure checks).**
- **Phase ordering rule:** Phase 4.4 must run after Phase 2 completes (RAG_MCP adapter must exist before Dify-specific logic is refactored into it). Phase 4.7 (rename promotion) must run after Phase 4.6 (validation) succeeds.
