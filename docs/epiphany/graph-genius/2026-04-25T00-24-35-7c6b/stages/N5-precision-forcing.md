<precision_forcing>

<renderings>

## 5-level vocabulary stripping

**Primary claim selected from N1 irreducibles:** "The KB adapter interface (query/populate/scan_gaps) plus runtime configuration enables a PBCPB system that switches KB backends with no code change, while the role system's linked_agent binding enables agent-system substitution with no code change."

**Level 1 — Five-year-old:**
We're building a tool that helps write instruction books called "playbooks." The tool needs to remember things — it saves what it knows in a special place. There are four different kinds of places you can use: a smart search system, a folder of notes, an old-style filing box, or a place you build yourself. You pick which place by changing one line in a settings file. The tool itself never changes — just the settings. The tool also has helpers who do jobs. Each job has a name like "@builder" or "@checker." You write down who each job is for later — you don't have to decide right now. If you want to change who does the job, you just change the settings file.

**Level 2 — Fifteen-year-old:**
PBCPB is a tool that creates and uses "playbooks" — structured instruction sets. It needs to look up and store knowledge; it can use four different knowledge backends (a Dify AI search system, a folder of markdown files, a JSON database, or a custom API). You switch between them by editing one line in a config file — no code rewrite needed. The same three operations work on every backend: search for content (query), add content (populate), and check what's missing (scan_gaps). The "Dify" backend is special: adding content doesn't actually upload to Dify automatically — it just creates a checklist that a human uploads manually. The tool also supports roles: named slots like "@architect" that you can assign to a real person, a Claude instance, or leave empty. The assignment is done in config, not in the code.

**Level 3 — Domain expert:**
PBCPB implements a polymorphic KB adapter interface with three required methods: `query(query_string, filters?)`, `populate(content, tier, metadata)`, and `scan_gaps(schema_definition)`. Four adapter implementations: RAG_MCP (Dify MCP server or API; populate() is manifest-only — no direct Dify write), MARKDOWN_FOLDER (all three methods delegated entirely to the skill referenced by skill_ref; absent skill_ref → config error), JSON_DB (direct against existing JSON/markdown DB), CUSTOM_API (user-defined passthrough; populate() non-zero exit → hard halt). The dispatch key is `kb_adapter.adapter_type` in the runtime config. The role system defines `placeholder_name` slots with `role_type ∈ {MULTI_AGENT, HUMAN, AI_ASSISTED}` and a `linked_agent` field for runtime injection; null is valid (unlinked). Bootstrap applies tier-scoring over `schema_definition` (a dependency graph): Tier 1 = entries with ≥2 dependents OR `required: true`; Tier 2+ are deferred to `deferred-backlog.json`.

**Level 4 — Mathematician/logician:**
Let B = {RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API}.  
Let AdapterInterface = { query: String × Filters? → Results, populate: Content × Tier × Metadata → Status, scan_gaps: SchemaDefinition → GapList }.  
∀b ∈ B: ∃ impl_b: AdapterInterface such that dispatch(b) = impl_b.  
Let Config = { kb_adapter: { adapter_type: b ∈ B, connection: C, skill_ref: R?, bootstrap_tier_limit: N } }.  
∀c₁, c₂ ∈ Config: c₁.codebase = c₂.codebase ∧ c₁.kb_adapter.adapter_type ≠ c₂.kb_adapter.adapter_type → dispatch(c₁) ≠ dispatch(c₂). [KB switching is config-only]  
∀c: c.kb_adapter.adapter_type = RAG_MCP → dispatch(c).populate(·) returns UploadManifest ∧ ¬writes(Dify). [manifest-only invariant]  
∀c: c.kb_adapter.adapter_type = MARKDOWN_FOLDER ∧ c.kb_adapter.skill_ref = ∅ → ∀f ∈ AdapterInterface: f(·) = ConfigError. [absent skill_ref = hard error]  
∀c: c.kb_adapter.adapter_type = CUSTOM_API ∧ exitcode(dispatch(c).populate(·)) ≠ 0 → Halt(ConfigError). [non-zero = hard halt]  
Let G = (V, E) be the dependency graph of schema_definition.  
Tier1 = { v ∈ V | indegree(v) ≥ 2 ∨ v.required = true }.  
Bootstrap populates only Tier1; ∀v ∈ V \ Tier1: v ∈ deferred-backlog.json.

**Level 5 — Minimal symbol:**
```
dispatch: Config → AdapterInterface
b = config.kb_adapter.adapter_type
IF b = RAG_MCP: populate → UploadManifest  (NOT DirectWrite)
IF b = MARKDOWN_FOLDER ∧ skill_ref = ∅: ALL_METHODS → ⊥(ConfigError)
IF b = CUSTOM_API ∧ populate() ≠ 0: → ⊥(ConfigError)
Tier1(G) = { v ∈ G | indegree(v) ≥ 2 ∨ required(v) }
role.linked_agent ∈ AgentSystems ∪ {null}  (resolved at runtime)
```

## 3-representation rendering

**Representation 1 — Diagram description (spatial/visual):**
Layered architecture diagram (4 layers, left-to-right):
- Bottom layer: 4 KB Backend boxes: [RAG_MCP|Dify] [MARKDOWN_FOLDER|skill_ref→user_skill] [JSON_DB|direct] [CUSTOM_API|passthrough]
- Adapter layer (above backends): single "Adapter Interface" box with 3 method slots: [query] [populate] [scan_gaps]. Arrows from each backend box connect upward into this layer (each backend implements all 3 slots).
- Business Logic layer (above adapter): "PBCPB Core" box. Arrow exits PBCPB Core downward to Adapter Interface only — never to backend boxes directly.
- Config layer (top): "Runtime Config JSON" box. Arrow from Config points to the dispatch arrow between PBCPB Core and Adapter Interface, labeled "adapter_type dispatch."
- Side column: "Role System" box containing role placeholder nodes [P1: @architect → linked_agent:A] [P2: @executor → null] [P3: @reviewer → linked_agent:B]. Config arrow also points to role system, labeled "runtime binding."
- Parallel to bottom: "Bootstrap" module box connecting to Adapter Interface via the same 3 methods; "schema_definition" input box feeds Bootstrap; "deferred-backlog.json" exits Bootstrap.

**Representation 2 — Equation / formal expression:**
```
PBCPB(input, config) = Playbook(dispatch(config.kb_adapter.adapter_type))

where:
dispatch(RAG_MCP)         → (query=Dify_query, populate=ManifestGen, scan_gaps=Dify_scan)
dispatch(MARKDOWN_FOLDER) → (query=skill(·), populate=skill(·), scan_gaps=skill(·))
dispatch(JSON_DB)         → (query=json_query, populate=json_write, scan_gaps=json_scan)
dispatch(CUSTOM_API)      → (query=api(·), populate=api(·)|⊥, scan_gaps=api(·))

role_dispatch(role, config) = config.role_system.roles[role].linked_agent  (or HUMAN if null)
```

**Representation 3 — Procedural description (steps to verify):**
1. Initialize PBCPB with `config.kb_adapter.adapter_type = "JSON_DB"` and a valid JSON_DB connection.
2. Call `adapter.query("test_query")` — confirm response comes from JSON_DB.
3. Call `adapter.populate("content", 1, {source: "test"})` — confirm content written to JSON_DB.
4. Change ONLY `config.kb_adapter.adapter_type = "RAG_MCP"` (no code change).
5. Call `adapter.query("test_query")` — confirm response now routes to Dify endpoint.
6. Call `adapter.populate("content", 1, {source: "test"})` — confirm `dify-upload-manifest.json` is written; confirm no direct Dify API write is made.
7. Remove `config.kb_adapter.skill_ref` and set `adapter_type = "MARKDOWN_FOLDER"` — confirm all three methods raise ConfigError.
8. Set `adapter_type = "CUSTOM_API"` with a populate() that returns exit code 1 — confirm Halt(ConfigError).

</renderings>

<constructive_specification>

## Constructive specification

**Primary spec:**
An agent that (1) starts PBCPB with adapter_type=JSON_DB, executes query()+populate()+scan_gaps() and observes correct JSON_DB behavior, then (2) changes only config.kb_adapter.adapter_type to RAG_MCP (no code change), re-executes the same three methods, and observes that: populate() produces dify-upload-manifest.json without Dify writes, query() routes to the Dify API, and scan_gaps() returns a gap list from Dify — constitutes a full demonstration of KB backend-agnosticism.

**Minimal-model check:**
- Both backend tests (JSON_DB + RAG_MCP) required: removing one loses the switch verification. Cannot remove.
- All 3 method tests required: the interface contract specifies exactly 3 methods; removing any one leaves the contract unverified. Cannot remove.
- The manifest-no-write check for RAG_MCP.populate() is the critical discriminating observable. Cannot remove.
- The "no code change" condition is the core claim; it must be verified by literal inspection (same source, different config). Cannot remove.

Spec is minimal. No elements eliminable.

**Constructive spec for role system (addendum):**
An agent that starts PBCPB with a config containing role_type=MULTI_AGENT (linked_agent="claude-code"), role_type=HUMAN (linked_agent=null), and role_type=AI_ASSISTED (linked_agent="openclaw") in the same role_system block — and confirms all three parse without conflict and that updating linked_agent in config routes role dispatch to the new agent without rebuild — constitutes a demonstration of the role binding system.

</constructive_specification>

<flagged_vagueness>

## Flagged vague claims

**[VAGUE] "accommodate for different scenarios ahead of time"**
Reason: "different scenarios" has no enumerated set; "ahead of time" is not operationally defined — what scenarios are included vs excluded is unspecified.
Possible resolution: enumerate the scenario set explicitly. The spec partially does this (KB backends × role_types × bootstrap states) but does not claim completeness. A complete accommodation claim requires a closed enumeration. Resolution: restate as "accommodates the following explicitly enumerated scenario matrix: [4 KB backends] × [3 role_type values] × [2 bootstrap states (new KB / existing KB)]."

**[VAGUE] "validate that ~/Documents/pbcpb-unified/ retains all original pbcpb functionality"**
Reason: "all original pbcpb functionality" is not enumerated anywhere in the spec. Without a feature inventory (produced in Phase 1), the validation step is undefined — an agent cannot determine what it is validating against.
Possible resolution: Phase 1.3 must produce an explicit feature inventory of pbcpb. Phase 4.6 validation must run against that specific inventory. The validation criterion is: ∀f ∈ feature_inventory(pbcpb): f(pbcpb-unified) = expected(f).

**[MILD VAGUE] "general improvements applicable to all backends"**
Reason: "general improvements" requires the implementation agent to make a judgment call during Phase 1.6 categorization. The boundary between category (a) "Dify-specific" and category (b) "general" is not formally defined — it is left to agent judgment from reading the diff.
Possible resolution: add a decision rule to Phase 1.6: "category (b) = any delta change whose behavior is unchanged regardless of adapter_type selection; category (a) = any delta change that references Dify-specific APIs, config keys, or data formats."

</flagged_vagueness>

<forward_chain>

## Forward-chain consequence tree

Root: "PBCPB dispatches KB operations via adapter_type config key to one of 4 backends implementing a 3-method interface; role binding dispatches via linked_agent at runtime."

**Branch A — New KB backend required (e.g., Neo4j graph DB):**
  A.1: User implements query/populate/scan_gaps for Neo4j → registers as CUSTOM_API
    A.1.1: No PBCPB code change required — CUSTOM_API passthrough accommodates it ✓
    A.1.2: Neo4j requires graph_traverse() not in 3-method contract → [CONTRADICTION] Branch A.1.2: "3-method contract is sufficient for all backends" contradicts "Neo4j requires a 4th method." Discriminating condition: whether graph traversal can be expressed as a filtered query() call.
  A.2: User provides skill via skill_ref for Neo4j as MARKDOWN_FOLDER workaround
    A.2.1: scan_gaps() returns Neo4j-specific gap format → tier_scoring expects {gap_id, location, type, schema_node} → if format differs: [PATHOLOGICAL] Branch A.2.1: non-standard gap_list schema breaks tier_scoring algorithm; gap_id field missing or renamed → tier scoring diverges.

**Branch B — Backend switched mid-session (active session state):**
  B.1: JSON_DB content not replicated to RAG_MCP → queries return different results after switch
    B.1.1: Playbook behavior changes mid-session due to different KB content
    B.1.2: [CONTRADICTION] Branch B.1.2: spec claims "same PBCPB instance switches backends" implying continuity, but no content migration is specified. Discriminating condition: whether "switching" means "same content base" or "empty new backend." Spec is silent on this — a content migration protocol must be specified if continuity is required.
  B.2: deferred-backlog.json is backend-agnostic (references schema_definition nodes, not DB keys)
    B.2.1: Backlog portable across backend switch → re-running bootstrap on new backend works ✓

**Branch C — skill_ref returns error on scan_gaps():**
  C.1: scan_gaps() call fails → gap list returns empty or throws
    C.1.1: tier_scoring receives empty list → no Tier 1 items found → populate_core skips all → [PATHOLOGICAL] Branch C.1.1: empty scan_gaps() response makes an empty KB indistinguishable from a fully-bootstrapped KB; no error is raised; system appears healthy.
    C.1.2: [DEGENERATE] Branch C.1.2: if KB is genuinely fully populated, empty scan_gaps() response IS the correct no-op behavior — valid degenerate case.
  C.2: skill_ref skill missing only populate() implementation
    C.2.1: Bootstrap populate_core fails → MARKDOWN_FOLDER protocol raises ConfigError → correct hard halt ✓

**Branch D — Phase 4 attempted before Phase 2 is complete:**
  D.1: RAG_MCP adapter module does not yet exist at start of Phase 4.4
    D.1.1: Dify-specific logic cannot be refactored into non-existent adapter → coupling violation persists in pbcpb-unified
    D.1.2: [DEGENERATE] Branch D.1.2: Phase sequencing requires implementation agent to track completion state; no automated enforcement mechanism exists in the spec. Agent must self-enforce the "complete Phase N before starting Phase N+1" constraint.

**Branch E — bootstrap_tier_limit set to 0:**
  E.1: No Tier 1 items are populated → all items enter deferred-backlog.json
    E.1.1: [PATHOLOGICAL] Branch E.1.1: deployed system with bootstrap_tier_limit=0 has empty KB but reports successful bootstrap; all queries return empty results.
  E.2: [DEGENERATE] Branch E.2: bootstrap_tier_limit=0 is valid per spec but semantically vacuous — bootstrap phase runs and produces only a backlog.

**Branch F — RAG_MCP scan_gaps() retrieves Dify document IDs:**
  F.1: Dify internal document naming diverges from schema_definition gap_ids
    F.1.1: gap_list entries have unresolvable schema_node references → tier_scoring misattributes → [PATHOLOGICAL] Branch F.1.1: gap_id mismatch between Dify and schema_definition corrupts tier scoring; Tier 1 items miscategorized; wrong content bootstrapped first.
  F.2: Dify document IDs match schema_definition exactly
    F.2.1: Tier 1 items identified → Dify upload manifest generated correctly → user uploads manually ✓

**Branch G — linked_agent changed in config without PBCPB restart:**
  G.1: PBCPB resolves linked_agent at session start (eager binding)
    G.1.1: In-session config change has no effect until restart → spec's "takes effect without rebuild" is satisfied (no code change) but restart may still be required → [MILD CONTRADICTION] Branch G.1.1: "takes effect without rebuild" vs "may require restart." Discriminating condition: whether "rebuild" means recompile vs restart process.
  G.2: PBCPB resolves linked_agent at each role invocation (lazy binding)
    G.2.1: Mid-playbook agent swap creates state inconsistency (agent A started a task; agent B resumes it mid-execution)

**Branch H — CUSTOM_API populate() fails on Tier 1 items:**
  H.1: Hard halt triggered by non-zero exit during populate_core (Tier 1 only)
    H.1.1: deferred-backlog.json may be incomplete (Tier 1 items that failed are not written there — halt occurred before defer step) → user must inspect which Tier 1 items failed from error message
    H.1.2: Tier 2+ items are never reached because bootstrap only runs Tier 1 → Tier 2+ deferred correctly ✓

**Branch I — schema_definition derived from master-index with circular dependencies:**
  I.1: Dependency graph G has cycle → topological sort diverges
    I.1.1: [PATHOLOGICAL] Branch I.1.1: cyclic schema_definition → tier-scoring (topological sort) does not terminate; bootstrap hangs or throws an unhandled exception.
    I.1.2: Guard required: schema_definition generator must detect cycles in dependency graph before bootstrap begins.
  I.2: Master-index has no dependency declarations (all entries are leaf nodes)
    I.2.1: indegree(v) = 0 for all v → no Tier 1 items by dependency count → only entries with required:true become Tier 1
    I.2.2: [DEGENERATE] Branch I.2.2: if no required:true entries and no dependencies, Tier 1 is empty → bootstrap populates nothing.

**Branch J — All roles are HUMAN (human-only configuration):**
  J.1: No AI agent invoked → PBCPB is pure coordination scaffold
    J.1.1: All 12 verification checks still apply → human workflow must satisfy system invariants ✓
    J.1.2: [DEGENERATE] Branch J.1.2: human-only config tests no AI integration; system degenerates to structured document management.
  J.2: Bootstrap phase still runs (KB layer independent of role system)
    J.2.1: Human operator manually reviews deferred-backlog.json and populates Tier 2+ manually ✓

---

## Flags Summary

[CONTRADICTION] Branch A.1.2: Neo4j requiring a 4th adapter method violates the closed 3-method contract.
[CONTRADICTION] Branch B.1.2: "same instance switches backends" implies continuity, but no content migration protocol is specified.
[PATHOLOGICAL] Branch A.2.1: non-standard gap_list schema from custom backends breaks tier_scoring.
[PATHOLOGICAL] Branch C.1.1: empty scan_gaps() response makes empty KB indistinguishable from bootstrapped KB.
[PATHOLOGICAL] Branch E.1.1: bootstrap_tier_limit=0 creates deployed-but-empty system with no error.
[PATHOLOGICAL] Branch F.1.1: Dify gap_id mismatch with schema_definition corrupts tier scoring.
[PATHOLOGICAL] Branch I.1.1: cyclic schema_definition dependency graph → tier-scoring does not terminate.
[DEGENERATE] Branch C.1.2: empty scan_gaps() when KB fully populated is a valid no-op.
[DEGENERATE] Branch D.1.2: phase ordering has no automated enforcement; relies on agent self-discipline.
[DEGENERATE] Branch E.2: bootstrap_tier_limit=0 is valid but semantically vacuous.
[DEGENERATE] Branch I.2.2: master-index with no dependencies or required:true entries yields empty Tier 1.
[DEGENERATE] Branch J.1.2: human-only config is valid but tests no AI integration.

## Tree Statistics

Branches generated: 10 (A–J)
Nodes: ~32 total
Flags raised: 2 contradiction / 5 pathological / 5 degenerate

</forward_chain>

</precision_forcing>

---

## SIGNAL OUTPUT

### Digest — precision_digest

```
key_findings:     [5-level stripping: claim survives all 5 levels; Level 4 formalizes dispatch as total function over Backend enum; Level 5 reduces to 6-line dispatch pseudocode; Constructive spec: 8-step procedural test requiring 2-backend switch with no code change + manifest-only RAG_MCP.populate() verification; Vague claims: "accommodate different scenarios" (no closed scenario enumeration), "validate all original pbcpb functionality" (no feature inventory generated in Phase 1); Forward chain: 5 pathological cases including cyclic schema_definition, empty scan_gaps, Dify gap_id mismatch, bootstrap_tier_limit=0, backend switch without content migration]
named_entities:   [dispatch-function, AdapterInterface, Backend-enum, RAG_MCP.populate-manifest-only, MARKDOWN_FOLDER-skill_ref-hard-error, CUSTOM_API-hard-halt, schema_definition-dependency-graph, Tier1-formula, deferred-backlog.json, linked_agent-runtime-binding, dify-upload-manifest.json]
confidence_flags: [H, H, H, H, M]
signal_flags:     []
```
