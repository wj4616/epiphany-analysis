<state_loading>

## Input class

**Class: Design** — The input is a structured implementation prompt defining a unified PBCPB (Playbook Creator Playbook Builder) system. It contains: component definitions (adapter types, role types), API surface specifications (query/populate/scan_gaps interface), system design decisions (runtime config switching, role binding), and explicit trade-off history (Dify manual-only path, skill delegation, staging strategy). Template 2 — Design applies.

---

## Primitives

### Category 1: Components (24 items)
1. PBCPB core (JSON-based playbook creator/builder engine)
2. KB Adapter Layer (abstraction over KB backends)
3. RAG_MCP adapter (Dify MCP server or Dify API backend)
4. MARKDOWN_FOLDER adapter (user skill-delegated access)
5. JSON_DB adapter (old-school JSON/markdown database)
6. CUSTOM_API adapter (user-defined knowledge API)
7. Role System Module (placeholder + binding logic)
8. Bootstrap Module (5-step CoT gap-fill sequence)
9. Runtime Configuration File (JSON config, selects KB + roles)
10. pbcpb original codebase (~/Documents/pbcpb) — JSON_DB path
11. pbcpb-dify codebase (~/Documents/pbcpb-dify) — RAG_MCP path
12. pbcpb-unified staging directory (merge target before promotion)
13. pbcpb-original-backup (immutable pre-merge snapshot)
14. Dify Upload Manifest generator (Tier 1 items → manual upload artifact)
15. Deferred Backlog File (deferred-backlog.json, Tier 2+ entries)
16. schema_definition document (expected KB structure, entry list, dependencies)
17. Skill reference system (skill_ref: PBCPB invokes user-provided skills)
18. Role placeholder registry (named placeholders at design time)
19. linked_agent binding (runtime resolution from config)
20. phase1-catalogue.md (written record of codebase exploration)
21. scan_gaps() gap-list output ({gap_id, location, type, schema_node})
22. tier_scoring module (Tier 1/2/3 scoring by dependency count)
23. populate_core module (fills Tier 1 gaps via adapter)
24. MCP server connection layer (for RAG_MCP adapter)

### Category 2: Interfaces (22 items)
1. query(query_string, filters?) → results — KB retrieval method
2. populate(content, tier, metadata) → status — KB write method
3. scan_gaps(schema_definition) → gap_list — KB gap detection method
4. kb_adapter JSON config block (adapter_type, connection, skill_ref, bootstrap_tier_limit)
5. role_system JSON config block (syntax_mode, syntax_pattern, roles[])
6. skill_ref invocation — PBCPB invokes user skill for MARKDOWN_FOLDER
7. Dify MCP server protocol (RAG_MCP primary connection)
8. Dify API (RAG_MCP alternative connection)
9. bootstrap_tier_limit config param (default 1; limits bootstrap depth)
10. adapter_type enum: RAG_MCP | MARKDOWN_FOLDER | JSON_DB | CUSTOM_API
11. role_type enum: MULTI_AGENT | HUMAN | AI_ASSISTED
12. syntax_mode: @name | custom
13. syntax_pattern (user-defined pattern when syntax_mode == custom)
14. placeholder_name field (role identifier, design-time)
15. linked_agent field (agent system binding, runtime)
16. connection block (adapter-specific connection params, opaque to core)
17. gap item schema: {gap_id, location, type ∈ {missing|placeholder|incomplete}, schema_node}
18. deferred-backlog.json schema: {generated_at, tier_limit_used, deferred[{gap_id, location, type, tier, schema_node}]}
19. dify-upload-manifest.json schema (Tier 1 items with KB locations)
20. schema_definition JSON document (entries with ids, categories, required flags, dependency relationships)
21. phase1-catalogue.md (written delta catalogue of pbcpb vs pbcpb-dify per categories a/b/c)
22. Verification checklist (12 runtime checks, explicitly enumerated in <verification>)

### Category 3: Known constraints (22 items)
1. PBCPB must remain JSON-based throughout all backends and role configurations
2. KB backend selection is a runtime configuration decision — no code rebuild required
3. All implementation staged to ~/Documents/pbcpb-unified/ before replacing original
4. Phases are sequential: must complete N before starting N+1
5. Phase 4 cannot begin until Phases 2 AND 3 are complete
6. RAG_MCP: populate() MUST NOT auto-write to Dify — manifest-only path
7. MARKDOWN_FOLDER: all three adapter methods delegated via skill_ref
8. MARKDOWN_FOLDER requires skill_ref present; absent → configuration error (not silent degradation)
9. CUSTOM_API: populate() non-zero exit → halt with configuration error (not silent skip)
10. Backup required before any modification: cp -r ~/Documents/pbcpb ~/Documents/pbcpb-original-backup
11. linked_agent == null is valid (unlinked; human executes manually)
12. Updating linked_agent in config takes effect without rebuild
13. HUMAN_ONLY is NOT a valid enum value — use all-HUMAN role list instead
14. Bootstrap: Tier 1 (core-necessary) first; Tier 2+ deferred to deferred-backlog.json
15. Role placeholders named at design time; agent system binding is runtime-only
16. No code-level branching at business-logic layer (adapter routing purely via config)
17. Phase 4.7: rename pbcpb/ → pbcpb-superseded/ then pbcpb-unified/ → pbcpb/
18. deferred-backlog.json path: ~/Documents/pbcpb/bootstrap/deferred-backlog.json
19. dify-upload-manifest.json path: ~/Documents/pbcpb/bootstrap/dify-upload-manifest.json
20. Mixed role types (MULTI_AGENT + HUMAN + AI_ASSISTED) must coexist in same role_system block
21. scan_gaps() schema_definition: entries with ids, required flags, dependency relationships
22. For RAG_MCP: Steps 1–2 scan/score; Step 3 generates manifest only; Step 5 emits manifest file

### Category 4: Quality attributes (20 items)
1. Maintainability: adapter pattern decouples KB access from PBCPB business logic
2. Extensibility: new KB backends addable without changes to PBCPB core
3. Configurability: runtime KB backend switching via config file
4. Portability: explicit support for Dify, MCP, Mattermost, claude code, openclaw, codex
5. Reliability: staging + backup strategy before production promotion
6. Correctness: 12 runtime verification checks, explicitly enumerated
7. Testability: matrix test all role configs × all KB backends (required in Phase 3.6)
8. Safety: CUSTOM_API non-zero exit → hard halt (prevents silent corruption)
9. Transparency: Dify manifest-only (no stealth uploads; user controls)
10. Explicitness: configuration errors over silent degradation
11. Composability: mixed role types in same playbook without conflict (EC-4)
12. Reversibility: original preserved as pbcpb-original-backup; pbcpb-superseded/ also retained
13. Backward compatibility: JSON_DB path must work after merge (verification check #2, #3)
14. Forward compatibility: role system accommodates future agent systems via linked_agent config
15. Decoupling: skill_ref delegates MARKDOWN_FOLDER logic; PBCPB has no hardcoded markdown KB behavior
16. Auditability: phase1-catalogue.md provides written record before Phase 2 begins
17. Simplicity: no code-level KB switch; config change only
18. Robustness: bootstrap on empty KB is valid; large deferred backlog is expected behavior (EC-5)
19. Timeliness: bootstrap fills Tier 1 first; enrichment content explicitly deferred
20. Determinism: CUSTOM_API failure always halts (never continues silently)

### Category 5: Stakeholder requirements (21 items)
1. User defines the KB backend (not locked to Dify or flat files)
2. User switches KB backends without code changes or rebuilds
3. User defines role configurations (human-only, AI, mixed)
4. User provides their own skills for MARKDOWN_FOLDER (skill_ref)
5. User manually applies Dify upload manifest (RAG_MCP path)
6. Mattermost-style @name addressing supported for MULTI_AGENT
7. Custom syntax supported (syntax_mode: custom + syntax_pattern)
8. All named systems must work: PBCPB, Dify, MCP, Mattermost, claude code, openclaw, codex
9. System accommodates different scenarios ahead of time (role and KB backend flexibility)
10. Unified build must be validated before replacing original
11. Original codebase preserved (backup required, not optional)
12. All original pbcpb functionality retained (JSON_DB path works after merge)
13. Dify/MCP improvements from pbcpb-dify included in unified build
14. Dify-specific logic refactored into RAG_MCP adapter (not polluting base)
15. General improvements applied to all backends (applied at base layer)
16. Roles can be unlinked (linked_agent == null) for human execution
17. Placeholder linking requires only config update (no schema migration, no rebuild)
18. Bootstrap produces Tier 1 content first; detailed content explicitly deferred
19. CUSTOM_API users can provide user-defined populate() behavior
20. System handles empty KB gracefully (large backlog is expected, not error)
21. All 12 verification checks pass at runtime

### Category 6: Trade-off history (20 items)
1. Unified codebase vs dual-fork maintenance: chose unification (pbcpb-unified) over two forks
2. Dify auto-populate vs manifest-only: chose manifest-only for RAG_MCP (Dify upload constraint)
3. skill_ref delegation vs PBCPB-native markdown logic: chose full delegation (keeps core agnostic)
4. Design-time binding vs runtime binding for roles: chose runtime (linked_agent in config)
5. Code-level switch vs runtime config for KB: chose runtime config (no rebuild)
6. Phase sequencing: explicit "Phase 4 after 2+3" to ensure adapter + role modules exist before merge
7. Staging directory vs in-place modification: chose staging (pbcpb-unified) for safety/rollback
8. Rename vs copy for final promotion: rename (space-efficient; backup preserved separately)
9. Tier 1 first vs comprehensive bootstrap: chose core-first (Tier 1 only on first pass)
10. CUSTOM_API hard halt vs silent skip: chose hard halt (fail-fast for user-defined APIs)
11. @name syntax vs custom syntax: both supported via syntax_mode field
12. HUMAN_ONLY as separate enum vs all-HUMAN config: chose all-HUMAN (no special-case enum needed)
13. General improvements applied to base (not kept dify-only): applied to base for all backends to benefit
14. Dify-specific additions isolated to RAG_MCP adapter: isolated, not spread to base
15. Removals/regressions from pbcpb-dify excluded from merge (Phase 1.6 category c)
16. bootstrap_tier_limit default = 1: conservative (only Tier 1 on first bootstrap)
17. skill_ref required for MARKDOWN_FOLDER, optional for CUSTOM_API, unused for RAG_MCP/JSON_DB
18. Phase 1 must produce phase1-catalogue.md before Phase 2 begins (documentation gate)
19. Validation checks defined as runtime checks, not prompt structure checks (more rigorous)
20. Linking placeholder to agent requires only config update (no schema migration, no code change)

### Category 7: Failure modes (21 items)
1. MARKDOWN_FOLDER with absent/unavailable skill_ref → configuration error (EC-2; hard, not silent)
2. RAG_MCP adapter attempts auto-write to Dify → EC-1 violation; must not happen
3. CUSTOM_API populate() returns non-zero exit → halt with configuration error (EC-6)
4. Role placeholder with linked_agent == null → valid, no error (EC-3)
5. Mixed role configs coexist → must work without conflict (EC-4)
6. Bootstrap on empty KB → large deferred backlog expected, not error (EC-5)
7. scan_gaps() returns entire KB as gaps → Tier 1 populated only; rest deferred
8. Phase 4 started before Phase 2/3 complete → RAG_MCP adapter doesn't exist for 4.4 refactor
9. Modification to original pbcpb before backup → data loss (backup step is mandatory)
10. pbcpb-unified fails validation → must NOT rename into place (verification must pass first)
11. skill_ref unavailable at bootstrap runtime → configuration error cascade (all 3 methods fail)
12. Phase 1 catalogue not written before Phase 2 → no factual basis for adapter design
13. Dify upload manifest emitted before Tier 1 content generated → empty or incomplete manifest
14. CUSTOM_API not configured for writes → populate() fails; hard error, not silent continue
15. schema_definition derived incorrectly from master-index → incorrect gap scoring, wrong Tier 1
16. Tier 1 scoring error: Tier 1 item miscategorized → core content not bootstrapped first
17. pbcpb-dify regressions accidentally included → regression in unified build (Phase 1.6c filters these)
18. General improvements miscategorized as Dify-specific → improvement not applied to all backends
19. linked_agent not resolved from config at runtime → role dispatch fails silently
20. syntax_mode mismatch: @name used but agent system expects custom syntax → role dispatch fails
21. pbcpb/ not archived to pbcpb-superseded/ before pbcpb-unified/ rename → overwrite without archive

---

## Simplicity-stripped irreducibles

Removed derivable items (with derivation notes):
- "No rebuild required to change KB backend" derives from: KB backend = runtime config
- "linked_agent == null is valid" derives from: role binding = runtime, not compile-time
- "Dify upload manifest produced; user applies manually" derives from: RAG_MCP populate() = manifest-only
- "All role configs must coexist" derives from: role_type enum supports mixed configurations
- "Validation must pass before rename" derives from: staging + validation before promotion constraint

**Surviving irreducibles:**

1. **PBCPB is JSON-based** — invariant across all backends and role types; non-negotiable
2. **Two source codebases** — pbcpb (JSON_DB) and pbcpb-dify (RAG_MCP); merge target is pbcpb-unified → pbcpb
3. **Adapter interface: 3 methods on 4 types** — query/populate/scan_gaps; RAG_MCP | MARKDOWN_FOLDER | JSON_DB | CUSTOM_API
4. **RAG_MCP populate() = manifest-only** — architectural Dify constraint; auto-upload not possible
5. **MARKDOWN_FOLDER = full skill_ref delegation** — PBCPB has no hardcoded markdown KB logic; skill_ref required
6. **KB backend = runtime config** — no code change, no rebuild to switch backends
7. **Role system: 3 types** — MULTI_AGENT | HUMAN | AI_ASSISTED; HUMAN_ONLY is not a valid enum
8. **Role binding = runtime** — linked_agent in config; null is valid; update without rebuild
9. **Bootstrap = Tier 1 first** — core-necessary (≥2 dependents OR required: true); Tier 2+ to deferred-backlog.json
10. **Phase ordering constraint** — sequential 1→2→3→4→5; Phase 4 blocked until 2 AND 3 complete
11. **Safety invariants** — backup before modify; CUSTOM_API failure = hard halt; MARKDOWN_FOLDER missing skill_ref = hard error

No derivation chains found beyond the 5 removed items above — remaining 11 irreducibles are independent.

---

## Tesla measurement plan

Success is observable when a deployed ~/Documents/pbcpb/ system satisfies all five of the following concurrent observables:

(a) **KB runtime switching**: an operator changes kb_adapter.adapter_type in config.json from "JSON_DB" to "RAG_MCP" and re-invokes PBCPB; KB operations route to the Dify adapter without code changes, a dify-upload-manifest.json is produced on populate() call, and no auto-write to Dify occurs.

(b) **Skill delegation error**: an operator configures adapter_type = "MARKDOWN_FOLDER" with skill_ref absent; the system raises a configuration error (not silent degradation) on any of the three adapter methods.

(c) **Mixed role parsing**: a config block containing MULTI_AGENT, HUMAN, and AI_ASSISTED roles simultaneously parses without conflict; each role_type value exactly matches the enum strings; HUMAN_ONLY is rejected as invalid.

(d) **Bootstrap correctness**: executing the bootstrap phase on a seeded schema_definition produces (i) Tier 1 items populated (≥2 dependents or required: true), (ii) deferred-backlog.json at ~/Documents/pbcpb/bootstrap/deferred-backlog.json with correct schema and Tier 2+ entries, (iii) Tier 2+ items NOT populated during bootstrap.

(e) **Promotion integrity**: ~/Documents/pbcpb-original-backup/ exists unmodified before any changes; ~/Documents/pbcpb/ is the final unified codebase (all 12 verification checks pass); pbcpb-superseded/ contains the old pbcpb/.

---

## Verification

- **Preparation-gate:** all 11 irreducibles cited directly to input sections (role_scope, kb_scope, context, task, constraints, edge_cases). No assumptions introduced.
- **Tesla constructive-test:** measurement plan specifies 5 concrete observables (a)–(e), each with a specific agent/action/outcome triplet. ✓
- **Count check:** Category 1: 24 ✓ | Category 2: 22 ✓ | Category 3: 22 ✓ | Category 4: 20 ✓ | Category 5: 21 ✓ | Category 6: 20 ✓ | Category 7: 21 ✓. All ≥20.

All gates pass. No revisions required.

</state_loading>

---

## SIGNAL OUTPUT

### Digest 1 of 3 — primitives_digest

```
key_findings:     [Components: PBCPB core + 4 adapter types (RAG_MCP/MARKDOWN_FOLDER/JSON_DB/CUSTOM_API) + Role System + Bootstrap Module + Config File + Dify manifest generator + deferred-backlog; Interfaces: 3-method adapter contract (query/populate/scan_gaps), kb_adapter+role_system JSON config blocks, skill_ref delegation, linked_agent runtime binding; Constraints: JSON-based invariant, runtime KB switching, sequential phase ordering (Phase4 blocked on 2+3), backup-before-modify; Design template applied — 7 categories with ≥20 items each]
named_entities:   [PBCPB, pbcpb, pbcpb-dify, pbcpb-unified, RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API, MULTI_AGENT, HUMAN, AI_ASSISTED, skill_ref, linked_agent, placeholder_name, syntax_mode, bootstrap_tier_limit, schema_definition, deferred-backlog.json, dify-upload-manifest.json, phase1-catalogue.md, query, populate, scan_gaps, pbcpb-original-backup, pbcpb-superseded]
confidence_flags: [H, H, H, H, H, H, H]
signal_flags:     []
```

### Digest 2 of 3 — irreducibles_digest

```
key_findings:     [1. PBCPB is JSON-based (invariant); 2. Two source codebases — pbcpb (JSON_DB) and pbcpb-dify (RAG_MCP); 3. Adapter contract: query/populate/scan_gaps on 4 types; 4. RAG_MCP populate() = manifest-only (Dify architectural constraint, cannot auto-upload); 5. MARKDOWN_FOLDER = full skill_ref delegation (no hardcoded markdown logic); 6. KB backend = runtime config switch (no rebuild); 7. Role system: MULTI_AGENT/HUMAN/AI_ASSISTED — HUMAN_ONLY not valid enum; 8. Role binding = runtime (linked_agent in config; null valid); 9. Bootstrap = Tier 1 first; Tier 2+ deferred; 10. Phase ordering: sequential 1→2→3→4→5, Phase 4 blocked until 2+3 done; 11. Safety: backup required, CUSTOM_API non-zero = hard halt, MARKDOWN_FOLDER missing skill_ref = hard error]
named_entities:   [PBCPB, RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API, MULTI_AGENT, HUMAN, AI_ASSISTED, skill_ref, linked_agent, pbcpb-unified, Tier 1, Tier 2, deferred-backlog.json, dify-upload-manifest.json]
confidence_flags: [H, H, H, H, H, H, H, H, H, H, H]
signal_flags:     []
```

### Digest 3 of 3 — system_model_seed

```
key_findings:     [System boundary: unified PBCPB at ~/Documents/pbcpb/ with 4 swappable KB adapters and configurable role system; Conserved quantities: JSON format (all backends/roles), 3-method adapter interface, role placeholder names (design-time stable), phase sequential ordering; Observer frames: (a) runtime user switching KB via config, (b) implementation agent executing 5-phase merge, (c) bootstrap agent filling KB tiers; System axes: config-time vs runtime (KB backend binding, role-agent binding), Tier 1 vs Tier 2+ (bootstrap depth), staging vs production (pbcpb-unified vs pbcpb); State machine: {pbcpb + pbcpb-dify} → catalogue → adapter/role design → staged merge → validate → promote]
named_entities:   [PBCPB, pbcpb-unified, kb_adapter, role_system, query, populate, scan_gaps, Tier 1, deferred-backlog.json, dify-upload-manifest.json, schema_definition, linked_agent, skill_ref, pbcpb-original-backup]
confidence_flags: [H, H, H, H, H]
signal_flags:     []
```
