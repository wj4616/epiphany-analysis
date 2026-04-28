---
node_id: N6
node_type: ADVERSARIAL
hat: "Popper+Millikan"
scale_gate: STANDARD
session_id: 2026-04-25T00-24-35-7c6b
predecessor_nodes: [N1, N3, N5]
n3_1_present: false
---

<falsification>

<abductive>

## Abductive mode results

**Anomaly identified (Step A1):**
The most surprising element is that a multi-backend knowledge system with four structurally distinct backends — one with a manual-upload-only constraint (RAG_MCP), one with full user-skill delegation (MARKDOWN_FOLDER), one with direct database access (JSON_DB), and one with user-defined passthrough (CUSTOM_API) — is claimed to be adequately served by exactly three methods with no code-level branching at the business logic layer. The anomaly: the number of behavioral variants (4) exceeds the number of interface methods (3), yet the interface is claimed complete and sufficient.

**Candidate explanations generated (Steps A2–A3):**

H-A1 — "3-method contract is complete for PBCPB's access patterns":
IF the only semantically necessary KB operations for playbook creation are query, populate, and scan_gaps, THEN the anomaly (behavioral diversity behind a narrow interface) follows because all diversity resides in the implementations, not in the caller's need space.
- Parsimony: 0.90 | Explanatory power: 0.85 | Uberty: 0.80 | Combined: 0.85
- Testability: YES — falsification condition: identify a PBCPB playbook operation that requires a KB access pattern not expressible as any of the three methods (e.g., a cross-KB transactional join that requires atomicity across query + populate in a single call).

H-A4 — "PBCPB is a coordination scaffold, not an execution engine":
IF PBCPB's core purpose is to define what roles must exist and what KB operations must occur — not to execute them — THEN linked_agent=null being valid is expected, human-only configs are first-class, and the 3-method interface is sufficient because PBCPB never needs to know the details of how each adapter executes the operation.
- Parsimony: 0.85 | Explanatory power: 0.75 | Uberty: 0.70 | Combined: 0.77
- Testability: YES — falsification condition: find a PBCPB playbook operation that fails without knowing linked_agent at design time; or find a case where PBCPB must introspect adapter execution details at the business logic layer.

H-A5 — "Specification incompleteness is the primary hidden risk":
IF "validate all original pbcpb functionality" is undefined (no feature inventory was generated in Phase 1) and "accommodate different scenarios" has no closed enumeration, THEN the 12-check verification list is substituting for a missing feature inventory — and validation is structurally underspecified regardless of how well the adapter architecture is designed.
- Parsimony: 0.80 | Explanatory power: 0.65 | Uberty: 0.75 | Combined: 0.73
- Testability: YES — falsification condition: Phase 1.3 produces an explicit feature inventory; Phase 4.6 runs against that inventory with every check passing; the validation criterion becomes closed and computable.

H-A2 — "3-method contract insufficient for backend-specific requirements": combined score 0.38 — below development threshold, reasoning used for consolidation only.

H-A3 — "RAG_MCP manifest-only path is architectural constraint, not design choice": combined score 0.37 — below development threshold, reasoning used for consolidation only.

H-A6 — "schema_definition generation is the single most likely failure point": combined score 0.38 — below development threshold, reasoning used for consolidation only.

**Top 3 candidates (Step A4):**
1. H-A1 (combined 0.85): 3-method contract is complete; behavioral diversity is implementation-side
2. H-A4 (combined 0.77): PBCPB as coordination scaffold explains all null/unlinked valid states
3. H-A5 (combined 0.73): specification incompleteness is the primary hidden risk

</abductive>

<deductive>

## Deductive mode results

**Premises collected from N1 irreducibles + N5 constructive spec (Step D1):**

P1: PBCPB is JSON-based (invariant across all backends and role types)
P2: KB backend = runtime config switch (no code rebuild required)
P3: The 3-method interface (query/populate/scan_gaps) is the complete adapter contract
P4: RAG_MCP.populate() = manifest-only (architectural constraint; no direct Dify writes)
P5: MARKDOWN_FOLDER requires skill_ref; absent → ConfigError on all 3 methods
P6: CUSTOM_API: populate() non-zero exit → hard halt (ConfigError)
P7: Role binding = runtime (linked_agent in config; null valid; no rebuild to update)
P8: Bootstrap = Tier 1 first; Tier 2+ → deferred-backlog.json
P9: Tier1(G) = {v ∈ G | indegree(v) ≥ 2 ∨ required(v)} — requires G to be a DAG for topological sort
P10: Phase ordering: sequential 1→2→3→4→5; Phase 4 blocked until 2+3 complete
P11: dispatch(config.kb_adapter.adapter_type) → AdapterInterface — total function over Backend enum

**Derived consequences (Step D2):**

C1: From P2 + P11: All adapter-specific logic must reside entirely within adapter modules, never in PBCPB core.
→ Validity: YES. Follows directly from "no code-level branching at business logic layer."

C2: From P3 + P4: The 3-method interface has heterogeneous behavioral semantics — same method name, structurally different side-effect contracts per adapter. populate() for RAG_MCP returns an artifact and does not write; for all others it writes and returns status.
→ Validity: YES. Interface uniformity does not entail behavioral uniformity.

C3: From P5 + P6: A class of adapter failures exists that is user-owned and always produces hard halts. PBCPB cannot self-heal from user-owned failures; all recovery is manual.
→ Validity: YES. EC-2, EC-6 confirm.

C4: From P7 + P2: The runtime config file is the single point of variation for all system behavioral changes (KB backend, role agent binding).
→ Validity: PARTIALLY. Config integrity is critical, but no config validation schema is defined anywhere in the spec.
→ [FLAGGED PREMISE] P2 assumes config is always a well-formed valid JSON document; no config schema validation requirement is specified.

C5: From P8 + P9: If schema_definition G is derived incorrectly, Tier 1 is wrong and core content is not bootstrapped first.
→ Validity: YES. schema_definition generation correctness is a hard prerequisite for bootstrap correctness.

C6: From P9 + DAG requirement (N3, Tiered-Progressive-Delivery illumination): Tier1 computation requires G to be a DAG; tier-scoring does not terminate if G has a cycle.
→ Validity: YES — N5 Branch I.1.1 flagged this as pathological. No cycle guard is specified in the prompt.
→ [FLAGGED PREMISE] P9 assumes G is a DAG without requiring a cycle detection guard before bootstrap begins.

C7: From P10: Phase 2 must produce a complete RAG_MCP adapter interface definition before Phase 4 begins. Phase 2 is on the critical path.
→ Validity: YES. Phase ordering constraint addresses this directly.

C8: From C2 (heterogeneous populate() semantics): A caller that treats populate() uniformly across adapters will behave differently depending on adapter type — specifically, a caller assuming populate() always writes will silently fail for RAG_MCP (manifest generated, no write occurs, no error raised).
→ Validity: YES — N5 Branch B.1.2 flagged this as contradiction-class.
→ [FLAGGED PREMISE] P3's "complete adapter contract" does not specify that callers must handle non-uniform return types.

**Consequence testing (Step D3):**
- C1: holds — spec explicitly prohibits code-level KB branching
- C2: holds — RAG_MCP.populate() semantics confirmed
- C3: holds — EC-2, EC-6 confirmed
- C4: PARTIALLY — config integrity risk is unaddressed; no config schema validation specified
- C5: holds — N5 flagged this
- C6: holds — no cycle guard in spec
- C7: holds — phase ordering addressed
- C8: UNKNOWN — caller interface for populate() return types is underspecified

**Alternative hypotheses from flagged premises (Step D4):**

H-D1 — "P9 DAG assumption requires cycle-breaking rule, not just cycle detection":
The Tier1 formula assumes a DAG; a cyclic schema_definition dependency graph requires an alternative algorithm — Kosaraju's SCC decomposition would identify strongly connected components, all of which should be promoted to Tier 1 together (any cycle implies mutual foundational dependency).
Premise challenged: P9 (assumes DAG topology; silent on cyclic inputs)
Falsification condition: construct a cyclic schema_definition and observe whether bootstrap halts cleanly with a specific error or diverges.

H-D2 — "P4 manifest-only constraint is an operational boundary, not a permanent architectural invariant":
The manifest-only rule for RAG_MCP.populate() is a constraint of the current Dify upload model. If Dify releases a programmatic write API, or if the user's Dify deployment supports direct writes via MCP, the invariant breaks. The interface should treat this as a runtime capability flag on the adapter, not a permanent method contract.
Premise challenged: P4 (manifest-only as architectural invariant vs. operational constraint)
Falsification condition: identify a Dify deployment that supports direct programmatic content writes; confirm populate() still generates manifest-only output.

H-D3 — "The 3-method interface is sufficient for callers that handle adapter-specific return types, but fails for callers that assume uniform behavioral contracts":
The "interface is complete" claim requires qualification: the interface is structurally complete (3 methods exist for all adapters) but not semantically uniform (populate() has different side-effect semantics per adapter). Callers must handle non-uniform return types or downstream processing will silently produce incorrect behavior for RAG_MCP.
Premise challenged: P3 (3-method interface completeness claim does not specify return-type heterogeneity)
Falsification condition: implement a caller that treats all populate() responses uniformly; observe whether it silently fails for RAG_MCP by generating a manifest and treating it as a write confirmation.

</deductive>

<inductive>

## Inductive mode results

**Instances identified (Step I1):**

1. RAG_MCP adapter: query() → Dify API; populate() → manifest artifact (no write); scan_gaps() → Dify
2. MARKDOWN_FOLDER adapter: all 3 methods fully delegated via skill_ref
3. JSON_DB adapter: all 3 methods implemented directly against JSON/markdown DB
4. CUSTOM_API adapter: all 3 methods are passthrough; populate() non-zero → hard halt
5. Role MULTI_AGENT: linked_agent resolved at runtime; placeholder_name at design time
6. Role HUMAN: linked_agent = null; no AI execution
7. Role AI_ASSISTED: linked_agent bound to specific AI system; design-time placeholder
8. Bootstrap Tier 1: indegree ≥ 2 OR required:true; populated first
9. Bootstrap Tier 2+: all other entries; deferred to deferred-backlog.json
10. Phase ordering: Phase 4 blocked until 2+3 complete; each phase gates the next

**Generalizations (Step I2):**

G1: "Every behavioral extension point in PBCPB (KB backend, role agent, skill delegation) uses the pattern: named slot at design time + runtime binding via config + null/absent as a valid unlinked state."
Mechanism: the system prioritizes structural stability (skeleton defined at design time) over execution completeness (binding resolved at runtime). The architecture assumes the skeleton is always valid; execution may be deferred indefinitely.

G2: "Every hard failure in PBCPB corresponds to a user-owned component failing, not a PBCPB-owned component failing. PBCPB halts loudly at all user-owned boundaries."
Mechanism: PBCPB cannot diagnose user-defined component failures (skill_ref implementation, CUSTOM_API behavior, Dify config) — the only sound response is to halt and expose the error for user resolution.

G3: "Every abstraction layer in PBCPB is dispatched from a closed enumeration (adapter_type ∈ {RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API}; role_type ∈ {MULTI_AGENT, HUMAN, AI_ASSISTED}; tier ∈ {1, 2, 3+}). The closed set is defined at design time and does not grow at runtime."
Mechanism: closed enumerations guarantee exhaustive dispatchability — the system handles all cases it knows about; unknowns are excluded from the design.

G4: "Both the bootstrap tier system and the phase ordering constraint implement the same dependency-first priority pattern: structurally essential elements first, enrichment deferred."
Mechanism: dependency-first ordering is the governing invariant for both KB content population and system construction sequence.

**Boundary case testing (Step I3):**

G1 boundary test — static-binding deployment (no runtime config modification permitted):
→ G1 LIMITED: the named-slot + runtime-binding pattern assumes config file modification is always available. A baked-configuration or containerized deployment without config write access cannot use linked_agent or adapter_type switching without a rebuild. The pattern fails at this boundary.
→ Mark: G1 LIMITED TO [runtime-config-resolved deployments where config modification is permitted]

G2 boundary test — PBCPB-internal component failure (JSON_DB connection error; MCP server unavailable; malformed config JSON):
→ G2 LIMITED: the fail-loud pattern is defined only for user-owned boundaries. No error handling specification exists for PBCPB-internal failures — if the JSON_DB connection fails, if the MCP server is unreachable, or if the config file is malformed, the system behavior is undefined.
→ Mark: G2 LIMITED TO [user-owned boundary failures; PBCPB-internal failures have undefined error behavior]

G3 boundary test — user needs a 5th adapter type (e.g., VECTOR_DB, Neo4j, Weaviate):
→ G3 LIMITED: CUSTOM_API provides a workaround, but extending the adapter_type enum to include a new named backend requires modifying the enum — a code change. The closed enumeration is not open for extension without modification.
→ Mark: G3 LIMITED TO [the 4 currently enumerated backends; adding a named 5th type requires PBCPB code change]

G4 boundary test — Tier 1 items have incompletely specified dependencies (partial schema_definition):
→ G4 LIMITED: dependency-first ordering is correct only if the dependency graph is complete. A partial schema_definition produces an incomplete Tier 1 set; core content that IS foundational but is MISSING from schema_definition will be deferred to Tier 2+.
→ Mark: G4 LIMITED TO [complete and correct schema_definition inputs]

**Alternative hypotheses from failed generalizations (Step I4):**

H-I1 — "The runtime-binding model has a static-deployment gap":
The 'runtime binding via config' architecture (G1 limited) does not accommodate locked-down or baked-configuration deployment targets. A production deployment that bakes config into a container image would require a rebuild for any KB backend switch or role agent update — contradicting the stated "no rebuild required" claim.
Bounded by: runtime-config-resolved deployments; inapplicable to static or baked configurations
Falsification condition: show that PBCPB can switch adapter_type in a deployment where the config file is read-only at runtime.

H-I2 — "PBCPB has complete error handling at user-owned boundaries but undefined behavior for internal failures":
The hard-halt pattern is well-defined for user-owned failures (G2 limited). PBCPB-owned component failures (invalid config JSON, JSON_DB connection refused, MCP server timeout) have no specified error handling in the design. These failure modes are real in production and their absence from the spec constitutes a coverage gap.
Bounded by: user-owned boundary failures; PBCPB-internal failure handling is unspecified
Falsification condition: provide a running PBCPB instance with the JSON_DB connection string pointing to a nonexistent file; observe whether the system halts with a clear error or produces undefined behavior.

H-I3 — "CUSTOM_API is simultaneously an 'intended backend' and an 'extension mechanism' — the semantic boundary is undefined":
G3's closed enumeration creates an extension problem: CUSTOM_API occupies dual roles (a valid backend type AND the catch-all for any backend not in the other three). When a user implements Neo4j as CUSTOM_API, they are using the extension slot, not the intended use. The spec does not distinguish these uses, creating ambiguity in which configuration guarantees apply.
Bounded by: the 4 enumerated adapter types; CUSTOM_API as extension vs. intended use is undifferentiated
Falsification condition: specify distinct semantics for "CUSTOM_API as an intended user API backend" vs. "CUSTOM_API as an extension placeholder for a future named backend type"; demonstrate that the current spec provides equivalent guarantees for both cases.

</inductive>

<consolidated_hypotheses>

## Consolidated hypotheses

**Deduplication (Step C2):**
H-A5 (specification incompleteness) and H-I2 (undefined internal failure handling) overlap in the domain of "what the system does not define." H-A5 focuses on validation criterion vagueness; H-I2 focuses on runtime failure handling for PBCPB-owned components. Structurally distinct — both retained.
H-D3 (heterogeneous populate() semantics) and H-A1 (3-method contract complete) address the same interface from opposite angles. H-D3 is the more precise formulation and incorporates H-A1's evidence. H-A1 merged into H-D3.

**Cross-mode validation (Step C3):**
- Interface behavioral heterogeneity: abductive (H-A1), deductive (H-D3), inductive (G1 mechanism). 3-mode support.
- schema_definition cycle guard gap: deductive (C6/H-D1), inductive (G4 limited). 2-mode support.
- Error handling completeness gap: abductive (H-A5), deductive (C4 flagged), inductive (H-I2). 3-mode support.
- PBCPB as coordination scaffold: abductive (H-A4), inductive (G1 pattern). 2-mode support.
- CUSTOM_API semantic duality: deductive (B.1.2 from N5), inductive (H-I3). 2-mode support.
- Manifest-only as operational constraint: deductive (H-D2), abductive (H-A3 supporting evidence). 2-mode support.

**Final ranked consolidated hypotheses (Step C5–C6):**

H-CONS1 — Interface behavioral heterogeneity (populate() non-uniform side effects)
Hypothesis: The 3-method interface is structurally complete but semantically non-uniform. populate() for RAG_MCP returns an artifact (upload manifest) without writing; for all other adapters it writes and returns status. A caller that treats populate() uniformly will silently mishandle RAG_MCP — generating a manifest but treating it as a write confirmation, producing no error and no content in Dify.
Confidence: 0.88 | Supporting modes: A, D, I
Falsification condition: Implement a caller that treats all populate() responses uniformly and run it against all 4 adapters; demonstrate either that the RAG_MCP case raises an error OR that callers never treat populate() uniformly across adapters in the PBCPB implementation.

H-CONS2 — schema_definition cycle guard absence
Hypothesis: The Tier1 formula (indegree ≥ 2 OR required:true) assumes G is a DAG. The spec requires no cycle detection before bootstrap begins. A cyclic schema_definition — plausible when deriving from an existing master-index with mutual citations — causes the topological sort to diverge without a specified error. The correct alternative is to treat strongly connected components as Tier 1 together (Kosaraju's SCC), not to halt.
Confidence: 0.85 | Supporting modes: D, I
Falsification condition: Construct a cyclic schema_definition (entry A depends on entry B which depends on entry A); run bootstrap; observe whether a specific clean error is raised or the system diverges silently.

H-CONS3 — PBCPB error handling scope gap (internal failures undefined)
Hypothesis: PBCPB has well-defined error handling at user-owned boundaries (MARKDOWN_FOLDER missing skill_ref, CUSTOM_API non-zero exit) but undefined behavior for PBCPB-internal failures (JSON_DB connection refused, MCP server timeout, malformed config JSON). Internal failures are real production scenarios; their absence from the spec constitutes an unaddressed failure class, not merely deferred work.
Confidence: 0.80 | Supporting modes: A, D, I
Falsification condition: Point JSON_DB connection string to a nonexistent file; observe whether PBCPB halts with a clear, structured error or produces undefined behavior (exception propagation, silent empty response, or hang).

H-CONS4 — PBCPB as coordination scaffold (execution is injected, not owned)
Hypothesis: PBCPB's design intent is fundamentally a coordination scaffold — a system that registers roles, defines KB access points, and declares phases, but does not own the execution of any of them. linked_agent=null being valid, human-only configurations being first-class, and skill_ref delegation all confirm that PBCPB holds the structure while user-provided components hold the behavior. The primary risk of this model is that "structure complete, execution empty" is a successful deployment state indistinguishable from misconfiguration.
Confidence: 0.75 | Supporting modes: A, I
Falsification condition: Find a PBCPB playbook operation that fails without knowing linked_agent at design time; OR demonstrate a case where the system distinguishes "deployment with null linked_agents intentionally (human-only)" from "deployment with null linked_agents accidentally (misconfigured)."

H-CONS5 — CUSTOM_API semantic duality (intended backend vs. extension placeholder)
Hypothesis: CUSTOM_API serves two distinct roles: (1) as an intended backend for users with existing APIs, and (2) as the extension mechanism for any backend type not enumerated in {RAG_MCP, MARKDOWN_FOLDER, JSON_DB}. The spec does not distinguish these uses. A user implementing Neo4j via CUSTOM_API as an extension faces the same configuration requirements as a user with a genuine custom API — but the semantic guarantees, error handling expectations, and documentation coverage are designed for case (1), not case (2).
Confidence: 0.65 | Supporting modes: D, I
Falsification condition: Provide a distinct configuration path for "CUSTOM_API as extension placeholder" vs. "CUSTOM_API as intended user API"; demonstrate that both use cases receive equal spec coverage.

H-CONS6 — RAG_MCP manifest-only constraint may expire
Hypothesis: The manifest-only rule for RAG_MCP.populate() is documented as an architectural invariant, but it is actually an operational constraint of the current Dify upload model. If Dify releases a programmatic write API, or the user's Dify deployment supports direct writes via MCP, the manifest-only path becomes one option among several. The interface should model this as a runtime capability flag on the RAG_MCP adapter instance, not as a hard-coded method contract.
Confidence: 0.62 | Supporting modes: D, A
Falsification condition: Demonstrate that all current and planned Dify deployment configurations categorically disallow direct programmatic content writes via API or MCP; OR show that the Dify API surface has been stable on this point for ≥3 major versions.

**Count: 6 distinct alternatives. All ≥0.50 threshold. No advisory required.**

</consolidated_hypotheses>

<extreme_scenarios>

## Extreme scenarios

Testing primary conclusion: "Runtime-dispatched 3-method adapter interface with config-driven KB switching and IoC role binding is the correct and complete architecture for PBCPB unification."

**Regime 1 — Parameter → 0: Number of KB entries approaches zero (empty KB)**
Scenario: bootstrap on an empty KB; schema_definition has no entries.
Result: scan_gaps() returns empty list → Tier 1 = {} → populate_core skips all → deferred-backlog.json is empty → bootstrap reports success on an empty KB.
Claim behavior: DEGENERATE — system reports success with nothing populated; claim holds structurally but is semantically vacuous. Per EC-5 the empty KB is expected behavior, but there is no observable distinction between "bootstrap on genuinely empty schema_definition" vs. "schema_definition generation failed silently."

**Regime 2 — Parameter → ∞: Number of KB entries grows without bound**
Scenario: schema_definition with 100,000 entries; most are Tier 2+.
Result: Tier 1 populated (finite if dependency structure is sparse); deferred-backlog.json has ~99,000 entries. Bootstrap completes.
Claim behavior: holds — the tier-scoring algorithm scales with G; deferred-backlog.json becomes a large file but the architecture is correct. No failure mode introduced by scale alone. Potential concern: deferred-backlog.json write time and file size may be non-trivial; no performance specification exists.

**Regime 3 — Population → 1: Single adapter type remains (all others removed from enum)**
Scenario: PBCPB deployed with only JSON_DB adapter available; no RAG_MCP, MARKDOWN_FOLDER, or CUSTOM_API.
Result: dispatch(RAG_MCP) has no implementation → system behavior undefined.
Claim behavior: FAILS — the claim that "dispatch is a total function over Backend enum" requires all enum members to have implementations. A partial dispatch table is not a total function. This boundary exposes that the adapter_type enum and the adapter implementation set must be kept in sync; the spec does not specify what happens when an adapter_type is declared but its implementation is absent.

**Regime 4 — Population → ∞: Number of adapter types grows without bound**
Scenario: user needs 20 different KB backends.
Result: CUSTOM_API can accommodate each as a different user-provided implementation, but the adapter_type enum must remain a 4-member closed set (no new named types). Each new backend is expressed as CUSTOM_API with different connection params.
Claim behavior: LIMITED — the architecture accommodates the use case but semantically conflates all non-enumerated backends into CUSTOM_API. Diagnostic, monitoring, and documentation capabilities differ per backend but the type system cannot express these distinctions.

**Regime 5 — Time → 0: Immediate invocation before any phase completes**
Scenario: Phase 4 begins before Phase 2 finishes (RAG_MCP adapter does not exist).
Result: Phase 4.4 (refactor Dify-specific logic into RAG_MCP adapter) finds no adapter module → Dify logic remains in base codebase → coupling violation persists in pbcpb-unified.
Claim behavior: FAILS under adversarial conditions — phase ordering has no automated enforcement. If an agent begins Phase 4 prematurely, the architecture produces a coupled codebase that passes no structural tests because no tests for adapter isolation exist in the spec.

**Regime 6 — Time → ∞: Long-run asymptotic behavior (Dify upload model changes)**
Scenario: 24 months post-deployment; Dify releases programmatic write API.
Result: RAG_MCP adapter's populate() method continues generating manifests; no direct writes occur. Users who want direct Dify writes must modify the RAG_MCP adapter implementation to use the new API — which is a code change, not a config change.
Claim behavior: LIMITED — the "zero-rebuild config switch" claim applies to switching between existing adapters, not to evolving adapter implementations over time. H-CONS6 applies: the manifest-only constraint is an operational constraint, and its expiration requires an adapter code change.

**Regime 7 — Adversarial scenario: Agent with perfect information actively exploits the design**
Scenario: an adversarial implementation agent correctly identifies that config integrity is unvalidated (C4 flagged); deliberately introduces a typo in adapter_type ("RAGMCP" instead of "RAG_MCP").
Result: dispatch() receives an unknown type → total function assumption violated → behavior undefined. No config validation schema is specified; the system may throw an unhandled exception, default to JSON_DB silently, or produce a confusing error message.
Claim behavior: FAILS — the total-function dispatch claim requires all config inputs to be valid enum members. No config validation guards the dispatch entry point. The adversarial scenario degrades to a production config error with undefined consequences.

**Regime 8 — Random/noisy scenario: All config inputs are uniformly random noise**
Scenario: config.json contains random strings for all fields (adapter_type, connection, skill_ref, linked_agent).
Result: adapter_type will not match any enum member → dispatch fails (undefined behavior per above). skill_ref points to nonexistent skill → MARKDOWN_FOLDER hard error. linked_agent points to nonexistent agent → runtime resolution fails (undefined behavior — no error spec for this case).
Claim behavior: PARTIALLY FAILS — MARKDOWN_FOLDER missing skill_ref produces a defined error (ConfigError). All other noise conditions produce undefined behavior. The architecture is defensively specified only at the user-owned boundary points that were explicitly designed; all other noisy inputs pass through to undefined failure modes.

**Extreme scenario summary:**
- Regimes 1, 2: claim holds (degenerate at zero, scales at infinity)
- Regime 3: fails — partial dispatch table invalidates total-function claim
- Regime 4: limited — CUSTOM_API as catch-all conflates semantically distinct backends
- Regime 5: fails — phase ordering has no automated enforcement; premature Phase 4 produces coupled codebase
- Regime 6: limited — "zero-rebuild" applies to existing adapters; adapter evolution requires code changes
- Regime 7: fails — config not validated; dispatch total-function claim requires valid enum inputs
- Regime 8: partially fails — only user-owned boundaries have defined error behavior

</extreme_scenarios>

<millikan_assessment>

## Millikan assessment

For each hypothesis and the primary conclusion, measure differential effort in the reasoning.

**Primary conclusion:** "Runtime-dispatched 3-method adapter interface with config-driven KB switching and IoC role binding is the correct and complete architecture."

[MILLIKAN] Primary conclusion:
  Effort to support (finding evidence FOR): LOW — the primary conclusion is supported by direct spec reading; POSIX/JDBC/Spring-DI analogs reinforce it; N3 activation map converged on it with a=0.65. Multiple independent cross-domain validations.
  Effort to refute: HIGH — requires constructing edge cases (cyclic schema_definition, partial dispatch table, config validation gap, populate() semantic heterogeneity). Each counter-case required independent derivation in deductive and inductive modes.
  Asymmetry: YES — it was significantly easier to find evidence FOR the primary conclusion than to find evidence AGAINST it.
  Additional counter-evidence to seek: (1) examine whether any existing PBCPB caller code (in pbcpb-dify) treats populate() uniformly and would silently misbehave when switched to RAG_MCP; (2) confirm whether any phase in the existing pbcpb codebase implicitly assumes JSON_DB-specific scan_gaps() format; (3) check whether the MCP connection layer ever silently falls back to JSON_DB if the Dify endpoint is unreachable.

[MILLIKAN] H-CONS1 (populate() behavioral heterogeneity):
  Effort to refute: MEDIUM — requires showing that all callers of populate() explicitly handle non-uniform return types. This is a code-level check, not a reasoning check.
  Effort to support primary over H-CONS1: LOW — the spec's own definition of RAG_MCP.populate() as manifest-generator makes heterogeneity undeniable.
  Asymmetry: NO — both directions are equivalently provable from the spec text.
  Assessment: H-CONS1 is genuinely well-supported; no Millikan bias detected.

[MILLIKAN] H-CONS2 (schema_definition cycle guard absence):
  Effort to refute: LOW — the spec could be read as implying that schema_definition generation must produce a DAG (spec says "dependency relationships between entries" without stating cycles are valid). The refutation would argue the spec implicitly prohibits cycles.
  Effort to support: MEDIUM — requires showing that the spec does not explicitly require cycle detection.
  Asymmetry: YES (mild) — refuting H-CONS2 by citing spec implication requires reading intent into ambiguous text; supporting H-CONS2 requires only noting the absence of an explicit guard.
  Additional counter-evidence to seek: check whether the JSON schema for schema_definition (if defined) includes a constraint that makes cycles structurally impossible (e.g., tree-only dependency syntax).

[MILLIKAN] H-CONS3 (PBCPB internal failure handling undefined):
  Effort to refute: LOW — one could argue PBCPB "inherits" error handling from its implementation language (Python exceptions propagate naturally). The refutation is easy: internal failures raise exceptions and are visible.
  Effort to support: MEDIUM — requires distinguishing "exception propagates" from "error is handled with a defined response."
  Asymmetry: YES (moderate) — the refutation path is easier (natural exception propagation) than the support path (distinguishing propagation from handling).
  Flag: there is a moderate Millikan asymmetry here. The primary conclusion may be protected by the easy refutation of H-CONS3. The asymmetry does not invalidate H-CONS3 but warrants caution: natural exception propagation is not equivalent to defined error handling per the spec's own standard (hard halt with ConfigError is defined behavior; exception propagation is not).

[MILLIKAN] H-CONS4 (PBCPB as coordination scaffold):
  Effort to refute: LOW — H-CONS4 is easy to challenge by noting that populate() and scan_gaps() are execution operations, not coordination operations. PBCPB does execute KB writes directly (JSON_DB, CUSTOM_API).
  Effort to support: MEDIUM — requires interpreting "PBCPB holds the structure; user-provided components hold the behavior" across the full system.
  Asymmetry: YES — H-CONS4 has a ready refutation (PBCPB is also an execution engine for JSON_DB/CUSTOM_API). It is a useful framing hypothesis, not a structural claim.
  Assessment: H-CONS4 is a useful interpretive frame but is partially falsified by the counter-evidence that PBCPB directly executes operations for non-delegating adapters. Confidence should be recalibrated downward from 0.75 toward 0.55 for its strong form.

[MILLIKAN] H-CONS5 (CUSTOM_API semantic duality):
  Effort to refute: MEDIUM — the refutation would argue that CUSTOM_API is a single well-defined type and users who want extension behavior simply use it as such; the "semantic duality" is a user concern, not a spec concern.
  Effort to support: MEDIUM — equivalent difficulty on both sides.
  Asymmetry: NO — no significant differential effort detected.

[MILLIKAN] H-CONS6 (manifest-only as expiring constraint):
  Effort to refute: LOW — easy to argue that the current Dify API is what the spec targets and future capability changes are out of scope.
  Effort to support: MEDIUM — requires arguing that architectural decisions should anticipate API evolution.
  Asymmetry: YES — refuting H-CONS6 by appealing to current-spec scope is significantly easier than supporting it.
  Flag: Millikan asymmetry detected. H-CONS6 may be discounted by in-scope reasoning bias. However the asymmetry reveals a genuine scope boundary issue, not a reasoning flaw. The constraint is real today; its expiration is speculative. Mark H-CONS6 as scope-bounded, not refuted.

**Millikan summary flags:**
- Primary conclusion: ASYMMETRIC — confirming evidence required for caller-level populate() uniform treatment (H-CONS1 intersection)
- H-CONS3: MODERATE ASYMMETRY — internal failure handling may be undervalued due to easy refutation
- H-CONS4: ASYMMETRIC — partially falsified; strong form overstates PBCPB's non-execution role; confidence recalibrated to 0.55
- H-CONS6: ASYMMETRIC — scope-bounded; flagged but not refuted

</millikan_assessment>

<density_report>

## Density report

**Darwin's Golden Rule — Active disconfirming-evidence search for primary conclusion:**
"Runtime-dispatched 3-method adapter interface with config-driven KB switching and IoC role binding is the correct and complete architecture for PBCPB unification."

**1. Known counter-examples (documented cases where the architecture fails or is insufficient):**
- Counter-example 1: Any system requiring transactional KB operations (write + scan_gaps atomically) cannot satisfy the 3-method contract independently — the contract has no transaction semantics. Playbook-building systems that need "write-then-immediately-verify" in a single atomic operation expose this gap.
- Counter-example 2: POSIX file descriptor abstraction (the primary analog cited in N3) famously fails for socket-specific operations: select(), sendmsg(), ioctl() are not expressible through the read/write/close model alone. The PBCPB 3-method contract may have analogous escape hatch requirements (e.g., a Dify-specific "reindex" or JSON_DB-specific "vacuum/compact" operation).
- Counter-example 3: Spring DI (cited as analog for IoC) requires a container with lifecycle management. PBCPB's linked_agent binding has no lifecycle model — there is no "startup / shutdown / health check" for injected agents. A crashed linked_agent returns no error to PBCPB (undefined behavior per H-CONS3).

**2. Classes of problems where this approach fails:**
- Stateful KB sessions: the 3-method interface is stateless (each call is independent). KB backends that require session state (authenticated cursors, open transactions, streaming imports) cannot be cleanly expressed through the stateless interface without adapter-level workarounds that may not be visible to the business logic layer.
- Multi-KB joins: PBCPB uses a single adapter at a time (per config). A playbook operation that needs to query two KBs and correlate the results (e.g., query JSON_DB + query RAG_MCP simultaneously) is not addressed by the single-adapter dispatch model.
- Partial migrate / dual-write scenarios: switching from JSON_DB to RAG_MCP requires the user to migrate content manually (the spec acknowledges this for Dify), but provides no interface for dual-write (writing to both adapters during a migration window). The architecture treats migration as a one-time manual event, not a supported transition state.

**3. Theoretical objections (strongest objections from a dissenting perspective):**
Strongest objection: "The architecture defers all hard problems to user-provided components (skill_ref, linked_agent, CUSTOM_API) and then defines hard errors for when those components fail. This is not 'architecture' in the system design sense — it is a structured blame-assignment protocol. The actual KB intelligence, role execution intelligence, and bootstrap intelligence all live outside PBCPB. PBCPB is an empty framework that fails loudly when the real work is not done for it."

**4. Literature / established results that cut against the conclusion:**
- The "Leaky Abstraction" law (Joel Spolsky, 2002): "All non-trivial abstractions, to some degree, are leaky." The 3-method contract will leak: scan_gaps() for Dify returns Dify-document-ID-based gap_ids; scan_gaps() for JSON_DB returns JSON key-based gap_ids. These are structurally incompatible types behind the same method signature. Any downstream consumer of the gap list must know which adapter produced it to parse gap_ids — the abstraction leaks the adapter type.
- The "Impedance Mismatch" problem (C.J. Date on object-relational mapping): the mismatch between different storage models (relational, document, vector, graph) cannot be eliminated by a common interface — only hidden. The 3-method contract hides the impedance but does not resolve it. Tier scoring, gap detection, and populate semantics all have fundamentally different models across the four backends.
- Henry Spencer's "those who do not understand Unix are condemned to reinvent it, poorly": the MARKDOWN_FOLDER adapter fully delegates to user-provided skills with no guidance on what "scan_gaps()" means for a folder of markdown files. Two users implementing this independently will produce incompatible gap_list schemas, breaking tier_scoring in different ways.

**5. Mechanism gaps (weakest link in the causal chain):**
Weakest link: the causal chain from "correct schema_definition" to "correct Tier 1 bootstrap" has two under-specified steps: (a) how schema_definition is generated from an existing master-index (Phase 5 STEP 1 says "derive by inspecting the current master-index if one exists" but provides no algorithm), and (b) how Dify document IDs (returned by RAG_MCP.scan_gaps()) are mapped to schema_definition gap_ids (N5 Branch F.1.1 flagged this as a pathological case — gap_id mismatch corrupts tier scoring silently). The mechanism from "run scan_gaps() against RAG_MCP" to "get a gap list compatible with schema_definition's gap_id namespace" is the single weakest link in the bootstrap causal chain.

**Disconfirming evidence summary:**
Counter-examples: 3 (transaction semantics gap, POSIX analog failure modes, Spring DI lifecycle gap)
Failure classes: 3 (stateful KB sessions, multi-KB joins, partial migration / dual-write)
Strongest objection: structured blame-assignment rather than genuine architecture
Literature / established results: 3 (Leaky Abstraction Law, Impedance Mismatch, Unix reinvention pattern)
Mechanism gap: 1 (schema_definition to gap_id mapping; Dify ID namespace incompatibility)

Disconfirming items: 10 total

Density assessment: HIGH (≥5 items)

**Cargo-cult scan:**
Checked all 5 sections above. No cargo-cult patterns detected:
- "The theory has limitations" → replaced with: specific failure classes (stateful sessions, multi-KB joins, dual-write migration)
- "One could imagine cases where this fails" → replaced with: "Counter-example 1: transactional KB operations expose the 3-method contract's lack of transaction semantics because..."
- "Further research is needed" → replaced with: "The gap [schema_definition → gap_id mapping for Dify] is specifically what would change the conclusion about RAG_MCP bootstrap correctness if resolved"

All disconfirming items are specific with mechanism. No unfalsified patterns found.

</density_report>

</falsification>

---

## SIGNAL OUTPUT

### Digest — falsification_result

```
key_findings:     [H-CONS1: populate() has non-uniform side effects across adapters — RAG_MCP returns manifest artifact (no write); callers treating populate() uniformly will silently mishandle RAG_MCP (confidence: 0.88); H-CONS2: schema_definition cycle guard absent — cyclic dependency graph causes tier-scoring divergence; SCC alternative (Kosaraju) should be used for cyclic inputs (0.85); H-CONS3: PBCPB error handling complete at user-owned boundaries but undefined for PBCPB-internal failures (JSON_DB refused, MCP timeout, malformed config) (0.80); H-CONS4 (recalibrated): PBCPB is primarily a coordination scaffold but also directly executes for JSON_DB/CUSTOM_API — strong form of hypothesis partially falsified; recalibrated to 0.55; H-CONS5: CUSTOM_API semantic duality (intended backend vs. extension placeholder) unresolved in spec (0.65); H-CONS6: manifest-only RAG_MCP constraint is operational, not permanent architectural invariant — scope-bounded (0.62); Extreme scenarios: dispatch total-function claim fails at partial enum (Regime 3); config validation absent exposes adversarial typo failure (Regime 7); Millikan: primary conclusion asymmetrically easy to support; H-CONS3 and H-CONS6 undervalued due to easy refutation paths; Density: HIGH (10 disconfirming items); weakest mechanism link: schema_definition → Dify gap_id namespace mapping]
named_entities:   [H-CONS1, H-CONS2, H-CONS3, H-CONS4, H-CONS5, H-CONS6, populate-heterogeneity, schema_definition-cycle-guard, Kosaraju-SCC, RAG_MCP-manifest-only, CUSTOM_API-semantic-duality, Leaky-Abstraction-Law, Impedance-Mismatch, dispatch-total-function, config-validation-gap, gap_id-namespace-mismatch, Dify-upload-manifest, deferred-backlog.json, MARKDOWN_FOLDER-skill_ref]
confidence_flags: [H, H, H, M, M, M]
signal_flags:     []
```
