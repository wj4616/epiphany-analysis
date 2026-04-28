## Pipeline Status Header

> v1.1.0 · Scale: STANDARD · Pipeline: complete at 7 of 7 planned · Active conditionals: none · For raw state, re-invoke with --xml

---

## Headline Insight

The PBCPB unification problem is structurally solved by a runtime-dispatched 3-method adapter interface (`query` / `populate` / `scan_gaps`) over a closed Backend enum {RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API}, where the dispatch key is a single runtime config token (`kb_adapter.adapter_type`), the caller layer is invariant across all backend substitutions, and role binding uses the same IoC pattern (`linked_agent` in runtime config; design-time placeholders only). This architecture is independently confirmed by five convergence signals (CS1–CS5), each supported by 3–4 independent source nodes, and is structurally isomorphic to the POSIX fd model / JDBC / TLS session / IP routing / Spring-DI patterns across five cross-domain analogs.

**Precision-bounded form:** The dispatch function is total over the Backend enum if and only if (1) config validation gates the dispatch entry point before any backend is selected, (2) `populate()` callers explicitly handle the RAG_MCP manifest-artifact return branch rather than treating all populate() responses as write confirmations, and (3) `schema_definition` generation includes cycle detection with SCC-based Tier 1 promotion (Kosaraju) for any detected strongly connected component. Without these three conditions, the architecture is structurally sound but carries three specific, independently identified failure modes.

Confidence: **HIGH** for structural correctness. **MODERATE** for behavioral completeness claims (populate() semantic uniformity, config validation coverage, PBCPB-internal failure handling).

**Creativity type:** Combinatorial

---

## Theory Collisions

*(Node N4 skipped at STANDARD scale — run DEEP for this section.)*

---

## Discovery vs. Proof

**Discovery — N2 domain mappings (constraint-escape):**

N2 surveyed 25 cross-domain candidate fields and identified 5 structurally distinct mappings against the binding constraint (runtime backend switching with caller-side invariance):

- **M1 — Computation / Strategy Pattern:** Abstract interface + concrete strategy classes; context object selects active strategy from config registry. No conditional at call site. Transfer: `KBAdapterFactory` reads `adapter_type` from config and returns the correct concrete instance; PBCPB business logic calls `adapter.query(...)` exclusively.
- **M2 — Control Theory / Gain Scheduling:** Scheduling variable selects a pre-computed parameter set at runtime; controller-plant interface unchanged. Transfer: `adapter_type` is the scheduling variable; interface is the invariant control law; adapter's method bodies are the parameter set.
- **M3 — Chemistry / Catalyst Substitution:** Catalyst mediates the reaction equation without altering stoichiometry from the caller's view. Transfer: the KB operation equation (question→answer, content→stored, schema→gap list) is invariant; the adapter is the catalyst. `connection` block = enzyme active site specification.
- **M4 — Cryptography / Cipher Suite Negotiation (blend-worthy):** Two-phase: handshake → session. Application code holds a session object, never a cipher. The session encapsulates the negotiated implementation; callers only see the session API. Transfer: the TLS × PBCPB conceptual blend produced the **AdapterSession** emergent structure — a `bind(config_path)` initialization phase that validates config and returns a session handle, followed by an operational phase where all KB calls go through the session. This structure does not exist in either source domain alone.
- **M5 — Network Science / Routing Protocols:** Routers expose a uniform packet-forwarding interface; routing algorithm selected by config file. Transfer: `adapter_type` is the routing protocol selection; `query/populate/scan_gaps` is the forwarding API; bootstrap, role system, and playbook phases are hosts that only know the forwarding API.

Four structural principles emerged with multi-source convergence:
- P1 (uniform interface, divergent implementation): 4 independent mapping chains
- P2 (external config token drives dispatch): 3 independent chains
- P3 (caller-side code remains invariant): 4 independent chains
- P4 (two-phase separation: config/negotiation → operational): 2 independent chains

**Proof — N5 constructive spec + N7 verification:**

N5 produced an 8-step constructive specification:
1. Start PBCPB with `adapter_type = "JSON_DB"`; call all three methods — verify correct JSON_DB behavior.
2. Change only `adapter_type = "RAG_MCP"` (no code change); re-run all three methods — verify `populate()` produces `dify-upload-manifest.json` without Dify writes; verify `query()` routes to Dify.
3. Set `adapter_type = "MARKDOWN_FOLDER"` with `skill_ref` absent — verify ConfigError on all three methods (not silent degradation).
4. Set `adapter_type = "CUSTOM_API"` with a `populate()` returning exit code 1 — verify `Halt(ConfigError)`.
5. Configure a role_system with MULTI_AGENT + HUMAN + AI_ASSISTED simultaneously — confirm all three parse without conflict; update `linked_agent` in config only and confirm role dispatch updates without rebuild.
6. Execute bootstrap on a `schema_definition` with a known Tier 1 entry (≥2 dependents or `required:true`) — confirm `deferred-backlog.json` contains Tier 2+ entries; Tier 1 is populated.
7. Introduce an invalid `adapter_type` value ("RAGMCP") — confirm ConfigError rather than undefined behavior.
8. Construct a cyclic `schema_definition` — confirm bootstrap raises a structured error or promotes all SCC members to Tier 1 (Kosaraju path).

N7 verified: all 8 steps are demonstrable in principle; no logically impossible observation required. Spec is minimal — no step eliminable without losing coverage of a primary-conclusion claim.

*(Node N6.1 not activated — condition --conjecture not met.)*

---

## Independence-Verified Bridges

**Bridge 1 — 3-method adapter contract (CS1)**
Source: N1 (irreducible #3), N3 (Multi-Backend-Abstraction, combined score 1.95), N2 (P1 uniform interface, 4-source convergence), N5 (Level 4 formal dispatch as total function)
Structural invariant: The method surface (query/populate/scan_gaps) is the socket; all 4 adapter types are file descriptors. Caller code never holds a reference to backend-specific types.
Disanalogy limit: Does not extend to multi-KB join operations (single adapter per session), transaction semantics across methods, or stateful session backends requiring cursor lifecycle management.

**Bridge 2 — Runtime config as single dispatch token (CS2)**
Source: N1 (irreducible #6), N3 (Runtime-Polymorphism, a=0.60), N2 (P2 external config token, 3-source convergence), N5 (Level 5 pseudocode: `b = config.kb_adapter.adapter_type`)
Structural invariant: The adapter_type value in the config JSON is the vtable selector; business logic never reads adapter_type directly. Equivalent to a gain-scheduling variable, a TLS cipher suite selection, and an IP routing protocol config — all structurally exhibit: external token → implementation selection → caller invariance.
Disanalogy limit: Requires that config file modification is available at runtime; fails in baked/container deployments where config is read-only (G1 boundary, N6).

**Bridge 3 — Inversion of Control / skill_ref and linked_agent as DI injection points (CS3)**
Source: N3 (Inversion-of-Control, combined 1.67), N2 (P4 two-phase separation; P1/P3 from M1/M3 blend), N1 (irreducibles #5 MARKDOWN_FOLDER full delegation, #8 role binding = runtime)
Structural invariant: PBCPB is a framework (declares what roles exist and what KB operations must occur) rather than a library (called by user code). `linked_agent` and `skill_ref` are injection tokens; the concrete implementations are provided by the user at runtime. The AdapterSession emergent structure (N2 TLS×PBCPB blend) formalizes this: `bind(config_path)` = the DI container wiring step.
Disanalogy limit: PBCPB is also an execution engine for JSON_DB and CUSTOM_API (H-CONS4 partially falsified the strong IoC-only frame); it is simultaneously a framework and an executor depending on adapter type.

**Bridge 4 — Fail-safe invariant at user-owned boundaries (CS4)**
Source: N3 (Fail-Safe-Invariant, a=0.52), N1 (irreducible #11: CUSTOM_API non-zero = hard halt; missing skill_ref = hard error), N6 (H-CONS3 confirms the pattern is well-defined at user-owned boundaries and stops there)
Structural invariant: Every adapter boundary where the user owns the implementation (MARKDOWN_FOLDER skill_ref, CUSTOM_API populate(), Dify upload path) fails loudly with a ConfigError. Analogous to Erlang "let it crash," SIGPIPE in Unix, and dead-letter queues — silent degradation at user-owned boundaries is never acceptable because PBCPB cannot self-diagnose user-defined component failures.
Disanalogy limit: The fail-safe pattern is defined only for user-owned boundaries. PBCPB-internal failures (JSON_DB connection refused, MCP server timeout, malformed config JSON) have no specified error handling in the design (H-CONS3, confirmed by N6 density report).

**Bridge 5 — Tiered-progressive delivery / topological sort as tier-scoring algorithm (CS5)**
Source: N3 (Tiered-Progressive-Delivery, a=0.50; topological sort = tier-scoring), N5 (Level 4: Tier1(G) = {v ∈ G | indegree(v) ≥ 2 ∨ required(v)}), N6 (H-CONS2: cycle guard absent → topological sort diverges on cyclic G; SCC alternative needed)
Structural invariant: The `schema_definition` document is the authoritative ordering artifact. Tier 1 items are those with in-degree ≥2 in the dependency graph or marked `required:true`; all others are Tier 2+. Hardcoded tier tables are fragile. Topological sort is the correct algorithm for acyclic graphs; Kosaraju SCC is the correct extension for cyclic inputs.
Disanalogy limit: Tier-scoring correctness depends entirely on the completeness and acyclicity of `schema_definition`. A partial or cyclic `schema_definition` produces incorrect Tier 1 identification. Dependency-first ordering is correct only for complete, accurate graph inputs (G4 limited, N6).

**Emergent bridge — AdapterSession two-phase initialization (N2 TLS×PBCPB blend)**
Source: N2 (emergent structure from M4 conceptual blend; does not exist in either input space alone)
Structural invariant: The `AdapterSession.bind(config_path)` call produces a session handle; all subsequent KB operations go through the session handle's three-method surface. All configuration errors (missing skill_ref for MARKDOWN_FOLDER, invalid adapter_type) are caught at bind time, not at first call time. The session object structurally prevents business logic from accessing backend-specific APIs.
Disanalogy limit: AdapterSession is a design recommendation derived from convergent analogical reasoning (N2 Step 4 is analogical, not probative); it is not a proven necessary consequence of the spec. Its integration with Phase 4 merge sequence requires explicit verification (N7 open probe #4).

---

## Alternative Hypotheses

**H-CONS1 — Interface behavioral heterogeneity (populate() non-uniform side effects)**
Hypothesis: The 3-method interface is structurally complete but semantically non-uniform. `populate()` for RAG_MCP returns an upload manifest artifact without writing; for all other adapters it writes and returns status. A caller treating `populate()` uniformly across adapters will silently mishandle RAG_MCP: generating a manifest but treating it as a write confirmation, with no error raised and no content in Dify.
Confidence: 0.88 | Supporting modes: Abductive, Deductive, Inductive (3-mode)
Falsification condition: Implement a caller treating all `populate()` responses uniformly; run against all 4 adapters; demonstrate either that the RAG_MCP case raises an error, OR that no existing PBCPB caller ever treats `populate()` uniformly across adapters.

**H-CONS2 — schema_definition cycle guard absence**
Hypothesis: The Tier1 formula (indegree ≥ 2 OR `required:true`) assumes G is a DAG. The spec requires no cycle detection before bootstrap begins. A cyclic `schema_definition` — plausible when deriving from an existing master-index with mutual citations — causes the topological sort to diverge without a specified error. The correct resolution is Kosaraju SCC: treat all members of a strongly connected component as Tier 1 together, rather than halting.
Confidence: 0.85 | Supporting modes: Deductive, Inductive (2-mode)
Falsification condition: Construct a cyclic `schema_definition` (entry A depends on B which depends on A); run bootstrap; observe whether a clean structured error is raised or the system diverges silently.

**H-CONS3 — PBCPB error handling scope gap (internal failures undefined)**
Hypothesis: PBCPB has well-defined error handling at user-owned boundaries (MARKDOWN_FOLDER missing skill_ref → ConfigError; CUSTOM_API non-zero exit → Halt) but undefined behavior for PBCPB-internal failures: JSON_DB connection refused, MCP server timeout, malformed config JSON. These are real production scenarios; their absence from the spec is an unaddressed failure class. Natural exception propagation (the easy refutation path) is not equivalent to the spec's own standard of defined ConfigError behavior.
Confidence: 0.80 | Supporting modes: Abductive, Deductive, Inductive (3-mode); Millikan flag: moderate asymmetry — internal failure handling may be undervalued due to easier refutation
Falsification condition: Point JSON_DB connection string to a nonexistent file; observe whether PBCPB halts with a clear, structured error or produces undefined behavior (exception propagation, silent empty response, or hang).

**H-CONS4 — PBCPB as coordination scaffold (partially falsified)**
Hypothesis: PBCPB's design intent is fundamentally a coordination scaffold — registers roles, defines KB access points, and declares phases without owning execution. `linked_agent=null` being valid and human-only configurations being first-class confirm this framing.
Status: **Partially falsified by N7.** PBCPB also directly executes `populate()` and `scan_gaps()` for JSON_DB and CUSTOM_API adapters — it is simultaneously a scaffold (for MARKDOWN_FOLDER, role binding) and an execution engine (for JSON_DB, CUSTOM_API). The strong IoC-only form of this hypothesis overstates PBCPB's non-execution role.
Confidence recalibrated: 0.55 (from 0.75) | Supporting modes: Abductive, Inductive (2-mode)
Falsification condition: Demonstrate a PBCPB playbook operation that fails without knowing `linked_agent` at design time; OR show the system distinguishes "null linked_agents intentionally (human-only)" from "null linked_agents accidentally (misconfigured)."

**H-CONS5 — CUSTOM_API semantic duality (intended backend vs. extension placeholder)**
Hypothesis: CUSTOM_API serves two distinct roles: (1) as an intended backend for users with existing APIs, and (2) as the extension mechanism for any backend type not enumerated in {RAG_MCP, MARKDOWN_FOLDER, JSON_DB}. The spec does not distinguish these uses; a user implementing Neo4j as CUSTOM_API extension faces the same configuration requirements as a user with a genuine custom API, but the semantic guarantees are designed for case (1) only.
Confidence: 0.65 | Supporting modes: Deductive, Inductive (2-mode)
Falsification condition: Provide a distinct configuration path for both use cases; demonstrate equal spec coverage for both.

**H-CONS6 — RAG_MCP manifest-only constraint may expire (scope-bounded)**
Hypothesis: The manifest-only rule for RAG_MCP `populate()` is documented as an architectural invariant but is actually an operational constraint of the current Dify upload model. If Dify releases a programmatic write API, the constraint becomes one option among several. The interface should model this as a runtime capability flag, not a hard-coded method contract.
Status: Scope-bounded, not refuted. The constraint is real for the current Dify deployment model; its expiration is speculative.
Confidence: 0.62 | Supporting modes: Deductive, Abductive (2-mode); Millikan flag: asymmetric — refuting by in-scope reasoning is easier than supporting
Falsification condition: Demonstrate all current and planned Dify deployments categorically disallow direct programmatic writes; OR show the Dify API surface has been stable on this point for ≥3 major versions.

*(Node N6.1 not activated — condition --conjecture not met.)*

---

## Density-Checked Falsification

**Density result from N6:** 10 disconfirming vectors attempted — density HIGH — residual uncertainty MODERATE

Disconfirming vectors:

1. Counter-example: Transactional KB operations (write + scan_gaps atomically) expose the 3-method contract's lack of transaction semantics. PBCPB has no cross-method atomic operation capability.

2. Counter-example: POSIX fd analog failure modes — select(), sendmsg(), ioctl() are not expressible through read/write/close alone. Analogous PBCPB escape hatch requirements (e.g., Dify-specific "reindex" or JSON_DB "vacuum") may exist outside the 3-method contract.

3. Counter-example: Spring DI lifecycle gap — PBCPB's `linked_agent` binding has no lifecycle model (no startup/shutdown/health-check for injected agents). A crashed `linked_agent` returns no error to PBCPB (undefined behavior per H-CONS3).

4. Failure class: Stateful KB session backends (open cursors, authenticated sessions, streaming imports) cannot be expressed cleanly through the stateless 3-method interface.

5. Failure class: Multi-KB join operations (query two KBs simultaneously and correlate results) are not addressed by the single-adapter dispatch model.

6. Failure class: Partial migrate / dual-write scenarios (writing to both adapters during a migration window) have no supported transition state in the architecture.

7. Theoretical objection (strongest): "The architecture defers all hard problems to user-provided components and defines hard errors when they fail. This is a structured blame-assignment protocol, not system architecture. The actual KB intelligence, role execution intelligence, and bootstrap intelligence all live outside PBCPB." — N6 density report; mechanism is not falsified, only scope-bounded.

8. Established result: Leaky Abstraction Law (Spolsky 2002) — `scan_gaps()` for Dify returns Dify-document-ID-based gap_ids; for JSON_DB returns JSON-key-based gap_ids. Downstream gap_list consumers must know which adapter produced it to parse gap_ids — the abstraction leaks the adapter type.

9. Established result: Impedance Mismatch (C.J. Date) — the mismatch between relational, document, vector, and graph storage models cannot be eliminated by a common interface, only hidden. Tier scoring, gap detection, and populate semantics have fundamentally different models across the four backends.

10. Mechanism gap (weakest link): The causal chain from `scan_gaps()` on RAG_MCP to a gap list compatible with `schema_definition`'s gap_id namespace is under-specified. Dify document IDs may not match `schema_definition` gap_ids — if they diverge, tier scoring silently corrupts (N5 Branch F.1.1; N6 density report). This is the single weakest link in the bootstrap causal chain.

---

## Scope Limits

**Applies to:** Runtime-dispatched, config-driven multi-backend abstraction systems where (1) a closed set of ≥2 backend types is known at design time, (2) the caller's operational surface can be expressed as a fixed, uniform method contract, (3) runtime configuration is modifiable without code rebuild, and (4) behavioral divergence resides entirely in the adapter implementations rather than in the calling layer. Specifically: PBCPB-class KB unification tasks — JSON-based playbook tools integrating ≥2 KB backend types (RAG/document/DB/custom) with IoC role binding — where the spec defines explicit adapter_type enumerations, skill-delegation contracts, and tier-based bootstrap ordering.
**Does not extend to:** (1) Stateful KB session backends (open cursors, active transactions, streaming imports) — the 3-method interface is stateless and cannot model session lifecycle; (2) Multi-KB join operations requiring simultaneous dispatch across two adapter types in a single query; (3) Locked-down or baked-configuration deployments (container images with read-only config) where runtime adapter_type switching is structurally unavailable; (4) PBCPB-internal failure handling (JSON_DB connection refused, MCP server timeout, malformed config JSON) — the conclusion covers user-owned boundary failures only; (5) Adapter evolution scenarios (new Dify programmatic write API releasing post-deployment) — the conclusion holds for the current operational envelope of each adapter type, not their future capability sets; (6) Conclusions stated in the original procedural-specification frame of the input prompt — the primary conclusion was derived in the declarative-structural frame and requires translation before applying to implementation step sequences.
**Claims refused:** "The 3-method interface contract is semantically uniform across all adapter types" — refused: populate() has non-uniform side-effect semantics (RAG_MCP returns manifest artifact without writing; all others write and return status); callers assuming uniform behavior will silently mishandle RAG_MCP (H-CONS1, confidence 0.88). "dispatch() is a total function over the Backend enum in production deployments" — refused: no config validation gate guards the dispatch entry point; an invalid or partial enum value (typo, missing implementation) produces undefined behavior rather than a clean ConfigError (N6 Regime 3, Regime 7). "PBCPB is a pure coordination scaffold with no direct execution responsibility" — refused: strong form partially falsified; PBCPB directly executes populate() and scan_gaps() for JSON_DB and CUSTOM_API adapters (H-CONS4 recalibrated to 0.55).

---

## Coherence Signals

**CS1 — Multi-backend abstraction via uniform 3-method interface**
Cross-domain recurrence: POSIX file descriptor model (read/write/close over files, sockets, pipes, devices), JDBC over relational databases, cloud SDK HAL over AWS/GCP/Azure, OS device driver model, nginx module loading, HTTP semantics. ≥5 independent domain instances over 50 years of validation. Confirmed by 4 independent nodes (N1/N2/N3/N5).

**CS2 — Runtime config as single dispatch control point**
Cross-domain recurrence: Gain-scheduling variable (Control Theory) — scheduling variable external to controller selects parameter set; TLS cipher suite (negotiated from external config exchange); IP routing protocol config (external to forwarding logic); environment variable injection (12-factor app). Confirmed by 4 independent nodes (N1/N2/N3/N5).

**CS3 — Inversion of Control (PBCPB is a framework; user components injected)**
Cross-domain recurrence: Spring/CDI dependency injection, pytest fixtures, Angular DI, Unix stdin/stdout as injectable computation, Makefile compiler injection. `linked_agent` and `skill_ref` are structurally identical to constructor injection tokens. Confirmed by 3 independent nodes (N1/N2/N3).

**CS4 — Fail-safe invariant at user-owned boundaries**
Cross-domain recurrence: Erlang "let it crash" supervisor trees, SIGPIPE in Unix (loud failure at pipe boundary), AWS SQS dead-letter queues (message routed to dead letter rather than silently discarded), Hystrix circuit-breaker (trips loudly rather than degrading silently). Confirmed by 3 independent nodes (N1/N3/N6). Note: signal is bounded — applies only at user-owned boundaries; PBCPB-internal failures are unspecified.

**CS5 — Topological sort / SCC as tier-scoring algorithm**
Cross-domain recurrence: Debian package essential set (critical packages bootstrapped first), npm peerDependencies resolution, Kubernetes operator reconciliation loop (critical state before cosmetic state), academic curriculum prerequisites (foundational courses before enrichment). Confirmed by 3 independent nodes (N3/N5/N6). Extension: for cyclic graphs, Kosaraju SCC decomposes the cycle into jointly-Tier-1 components — the same dependency-first principle applied to non-DAG inputs.

---

## Generalization Checks

**Generalization claim (from N7):** The architecture generalizes to any software system that must route a fixed operation set across a structurally diverse but closed set of backend implementations, where the routing decision must not require caller-layer code changes and must be expressible as a single external configuration token.

**Holds at:**
- Any ORM (query/insert/delete over RDBMS/NoSQL/file) — operation set closed; dispatch token = db dialect config
- Any cloud SDK HAL (same API over AWS/GCP/Azure) — adapter_type analog = cloud_provider config
- Any message broker client (produce/consume/inspect over Kafka/RabbitMQ/SQS) — same 3-method structure

**Conditions required for generalization:**
1. The operation set is closed and known at design time (3 methods here; equivalent in other domains)
2. The backend set is closed (open-ended extension requires CUSTOM_API or equivalent catch-all)
3. The caller layer must never hold a reference to backend-specific types (enforced by AdapterSession pattern)
4. Error semantics at user-owned boundaries must be explicitly defined (not inherited from implementation language)

**Breaks at:**
- Systems requiring transaction semantics across multiple methods (atomicity across query + populate in a single call — the 3-method contract has no transaction primitive)
- Multi-backend dispatch (single adapter per session; no join operations)
- Static/baked-configuration deployments where config file modification is unavailable at runtime (G1 boundary, N6 — the "no rebuild required" claim does not hold in container images with read-only config)
- Open-ended backend extension without CUSTOM_API catch-all (adding a named 5th backend type requires modifying the enum — a code change, not a config change)

**V7 frame shift noted (N7):** The primary conclusion is expressed in a declarative-structural frame; the original input prompt was expressed in a procedural-specification frame. The generalization checks above hold for the declarative frame. Translation to the procedural frame (what to do in Phase 2/3/4/5) is required before applying these results to implementation step sequences.

---

## Representational Signals

**V7 frame shift detected:** Procedural (input) → Declarative (conclusion)

The input prompt (`stages/00-processed-input.md`) is expressed as a sequenced 5-phase task plan with explicit phase ordering, step-by-step instructions, edge cases, constraints, and verification checklists. The dominant frame is Procedural — sequences of operations to execute, conditional logic, and imperative constructions.

The primary conclusion (from N7) is expressed as declarative structural claims: "the PBCPB unification problem is structurally solved by X," "the dispatch function is total if and only if conditions A/B/C hold." These are statements about what is structurally correct, not instructions for how to execute. The dominant frame is Declarative.

**Shift significance:** Moderate. The conclusion answers "what architectural invariants must hold" rather than "execute this implementation in these steps." This is a valid and valuable re-representation — understanding the architecture precedes executing it — but requires explicit translation back to procedural step sequences before applying to Phase 4 implementation. This constraint is captured in Scope Limits item (6): "Does not extend to conclusions stated in the original procedural-specification frame."

*(Node N3.1 not activated — condition not met.)*

---

## Open Questions & Next Probes

**Probe 1 — Does any existing PBCPB caller code (in pbcpb-dify) treat populate() uniformly across adapter types?**
This is the primary H-CONS1 discriminating test. If yes, a silent RAG_MCP mishandling bug exists in the current codebase and must be patched before unification. Source: N7 open question #1.
Priority: **HIGH** — directly determines whether H-CONS1 is a design warning or an existing production bug requiring an immediate fix before Phase 4 begins.

**Probe 2 — Does schema_definition generation from the existing pbcpb master-index produce a cyclic dependency graph?**
This tests H-CONS2. If cycles are present, the bootstrap tier-scoring algorithm diverges on the actual production data. The fix (Kosaraju SCC treatment for SCCs) must be implemented in Phase 5 before bootstrap is run on any real KB. Source: N7 open question #2.
Priority: **HIGH** — a cyclic master-index would make the bootstrap algorithm non-functional for the primary real-world input case.

**Probe 3 — What is PBCPB's current behavior when JSON_DB connection string points to a nonexistent file?**
This tests H-CONS3 (PBCPB-internal failure handling undefined). The test requires changing one config field and observing the system response. If the result is undefined behavior rather than a structured ConfigError, the internal failure handling gap is confirmed as a production risk. Source: N7 open question #3; identified as lowest-effort / highest-information probe in the set.
Priority: **HIGH** — single lowest-effort probe; directly tests the H-CONS3 failure class.

**Probe 4 — Can the AdapterSession two-phase initialization pattern (emergent from N2 TLS×PBCPB blend) be integrated into the PBCPB design without breaking the Phase 4 merge sequence?**
N5 (Branch D.1.2) identified that phase ordering has no automated enforcement. If AdapterSession is added as a design requirement in Phase 2, it must be reflected in Phase 4.5 integration. This probe verifies whether the emergent design implication is compatible with the stated phase constraints. Source: N7 open question #4.
Priority: **MEDIUM** — design implication is valuable (forces validation at bind time, structurally prevents business logic from accessing backend-specific APIs) but its integration point is not yet verified.

**Probe 5 — Is CUSTOM_API intended as an extension mechanism, a first-class backend type, or both? Does the spec provide equal coverage for both use cases?**
This tests H-CONS5. A concrete test: find one scenario where CUSTOM_API is used as "intended API backend" and one where it is used as "extension placeholder for an unnamed backend type"; demonstrate that both receive equal configuration guarantees, error handling, and documentation coverage. Source: N7 open question #5.
Priority: **MEDIUM** — resolving this determines whether CUSTOM_API should be split into two distinct adapter types in the unified PBCPB.

**Probe 6 — Can Phase 1.3 produce an explicit feature inventory of pbcpb before Phase 4.6 validation runs?**
N5 flagged that "validate all original pbcpb functionality" is structurally undefined without a feature inventory. Phase 4.6 validation is non-executable without it. The fix is a documentation step (require Phase 1.3 to explicitly output a feature inventory), not a code change. Source: N7 open question #6; N5 vague-claim flag.
Priority: **LOW** — fix is a documentation gate, not a code change; however, absence makes the Phase 4 validation step non-executable regardless of architectural correctness.

---

key_findings:     [synthesis complete]
named_entities:   []
confidence_flags: [H]
signal_flags:     []
