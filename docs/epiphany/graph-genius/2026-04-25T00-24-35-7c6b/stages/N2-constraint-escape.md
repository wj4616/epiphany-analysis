---
node: N2
hat: de Bono
scale: STANDARD
session: 2026-04-25T00-24-35-7c6b
input_from: [N1, 00-processed-input.md]
---

<constraint_escape>

## Named constraint

**Binding constraint — RUNTIME_CONFIG:**
"The KB backend must switch at runtime via a configuration file alone, with no code-level branching at the business-logic layer and no rebuild. Every adapter type (RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API) must expose an identical three-method contract (query / populate / scan_gaps) so the same PBCPB instance routes operations without knowing the concrete backend at invocation time."

This is the single binding constraint because it governs the entire architecture: it prohibits naive conditional branching, mandates interface abstraction at the adapter boundary, and constrains how all dependent components (bootstrap, role system, skill delegation) must be layered. All other constraints in the input (KB_BACKEND_SUPPORT, KB_POPULATION, ROLE_BINDING, FORMAT, USER_DELEGATION) presuppose that this runtime-dispatch invariant has been solved first.

*No secondary constraints of equal binding force identified; no V6 escalation required.*

</constraint_escape>

<domain_candidates>

## Domain candidates surveyed

**Domain-agnostic expression of the binding constraint (Da Vinci):**
"A single caller must invoke functionally identical operations across structurally different implementations, where the active implementation is selected by an external configuration token — not by the caller's own logic. Behavioral divergence exists at the implementation layer but is hidden from the invocation layer. The constraint is that caller-side code must remain invariant across all implementation substitutions."

Structural class to match: **substitutable implementations behind a uniform interface; runtime dispatch from an external configuration token; caller-side invariance**.

---

**Survey of all 25 catalog domains (parallel evaluation):**

| # | Domain | Structural Match | Viability | Domain-Agnostic Occurrence |
|---|--------|-----------------|-----------|---------------------------|
| 1 | Evolutionary Biology | Polymorphism / niche specialization — organisms share behavioral triggers; biochemistry differs | VIABLE-WEAK | Same stimulus, divergent underlying mechanism; no external config token equivalent |
| 2 | Thermodynamics | Phase transitions — water/ice/steam respond differently to same thermodynamic operations; phase is config-like state | VIABLE | Phase state = external condition selecting implementation; equations of state diverge |
| 3 | Fluid Dynamics | Valve / channel routing — same pipe entry, valve position routes to different channels; downstream differs | VIABLE-STRONG | Valve setting = config token; source pressure (caller) invariant |
| 4 | Immunology | Receptor-class dispatch — same antigen triggers different effector pathways depending on receptor class | VIABLE | Receptor class = config; antigen (caller) is invariant |
| 5 | Information Theory | Codec selection — same bitstream, codec selected at decode time; encoder/decoder pairs substitutable behind uniform bit interface | VIABLE-STRONG | Codec identifier = config token; bit-stream interface is invariant |
| 6 | Architecture | Interchangeable bearing elements — uniform connection points; bearing material (masonry/steel/rubber) swapped without modifying the beam above | VIABLE | Connection geometry = interface; bearing material = config-selected implementation |
| 7 | Ecology | Functional role substitution — trophic role filled by different species; food web structure invariant to species substitution | VIABLE-WEAK | Role = interface; species = implementation; no external token |
| 8 | Game Theory | Type-contingent mechanism — mechanism works for all agent types without knowing type; message space uniform | VIABLE-WEAK | Partial structural match; config token is private (type), not external |
| 9 | Developmental Biology | Positional information / cell fate — all cells receive same molecular signals; gene expression response depends on which receptor cluster is active | VIABLE | Receptor cluster activity = runtime config; molecular signal (caller) invariant |
| 10 | Control Theory | Gain scheduling — controller parameters switch based on external scheduling variable (altitude, speed, load) without changing controller–plant interface | VIABLE-STRONG | Scheduling variable = config token; plant interface = uniform invocation |
| 11 | Crystallography | Polymorphic crystal forms — same chemical formula, different lattice structure triggered by temperature/pressure; bulk properties diverge | VIABLE | Temperature/pressure = config token; stoichiometric formula = uniform caller representation |
| 12 | Network Science | Routing protocols — routers expose uniform forwarding interface; routing algorithm (OSPF/BGP/static) selected by config | VIABLE-STRONG | Routing protocol config = adapter_type; forwarding interface = query/populate/scan_gaps |
| 13 | Linguistics | Morphological paradigms — same root, different inflectional endings selected by grammatical context; semantic role invariant | VIABLE-WEAK | Grammatical context = config; surface form = implementation; weak config token |
| 14 | Epidemiology | Intervention strategy selection — same population model, different containment strategy by R₀ threshold | LOW | Threshold is a computed property, not an external config token; poor structural fit |
| 15 | Acoustics | Acoustic impedance matching — same source signal; matching layer selected to bridge source and medium; source invariant | VIABLE | Matching layer = adapter; source = caller; medium = backend |
| 16 | Economics | Smart order routing — same order; router selects execution venue at runtime based on config; trader (caller) invariant | VIABLE-STRONG | Venue selection config = adapter_type; order interface = uniform method contract |
| 17 | Optics | Optical filter stack — same beam enters; filter physically swapped; downstream detection invariant | VIABLE | Filter = adapter; beam = caller invocation; detector = backend |
| 18 | Psychology | Cognitive schema activation — same stimulus interpreted differently depending on active schema; perceptual system is invariant caller | VIABLE-WEAK | Schema = config; intrinsic rather than external token |
| 19 | Pharmacology | Prodrug activation — same compound administered; activation depends on which enzyme is present (biological config); active metabolite diverges | VIABLE | Enzyme profile = config token; compound administration = caller; metabolite = result |
| 20 | Urban Planning | Signal timing plan selection — same intersection geometry; active timing plan selected by time-of-day schedule; driver behavior invariant | VIABLE | Schedule config = adapter_type; intersection interface = uniform entry point |
| 21 | Cryptography | Cipher suite negotiation — same message; cipher suite selected at handshake time by configuration; encrypt/decrypt expose uniform API | VIABLE-STRONG | Cipher suite = adapter_type; handshake = config binding; message API = method contract |
| 22 | Geology | Isostatic adjustment — same gravitational load; response depends on crust type (intrinsic property, not external config) | VIABLE-WEAK | Config token is intrinsic, not external; poor fit |
| 23 | Animal Behavior | Fixed action patterns with releasers — same environmental stimulus; behavioral response depends on species/developmental state | VIABLE-WEAK | State is intrinsic, not runtime-configurable from outside |
| 24 | Computation | Strategy pattern / polymorphic dispatch — algorithms expose uniform interface; concrete algorithm selected by runtime config | VIABLE-STRONG | Direct structural isomorphism; adapter_type = strategy selector |
| 25 | Chemistry | Catalyst substitution — same substrate + reaction equation; catalyst swapped without changing equation from caller's view; rate/selectivity diverge | VIABLE-STRONG | Catalyst = adapter; substrate = KB content; reaction equation = method contract |

**Viable domains:** 2, 3, 4, 5, 6, 9, 10, 11, 12, 15, 16, 17, 19, 20, 21, 24, 25 — **17 viable candidates** (threshold of 5 met; proceed to Step 3).

</domain_candidates>

<domain_mappings>

## Domain mappings (5)

**Structural-distinctness check:** Each mapping must share no surface features.
- M1 = pure polymorphic dispatch (abstract interface + concrete class)
- M2 = external scheduling variable modulating internal parameters
- M3 = third-party mediator physically substituted; caller-equation invariant
- M4 = two-phase: negotiated agreement → uniform operational session
- M5 = physical network topology routing by forwarding table config

All five are structurally distinct — no surface features shared. PASS.

---

### Mapping 1 — Computation / Strategy Pattern

**Source domain:** Computation / Complexity Theory (Strategy Pattern, polymorphic dispatch)
**Target domain:** PBCPB runtime KB backend selection
**Structural analog:** An abstract interface defines a method contract; concrete strategy classes implement it; a context object holds a reference to the active strategy selected from a config registry. The caller invokes the interface, never the concrete class.
**What solves it in source domain:** The Strategy pattern separates the invariant algorithm skeleton (caller) from the variant parts (implementations). The active strategy is injected at runtime — from a factory, registry, or config file — without any conditional at the call site.
**Transfer to PBCPB:** Define an `KBAdapter` abstract interface with exactly `query()`, `populate()`, `scan_gaps()`. Implement four concrete adapter classes: `RAGMCPAdapter`, `MarkdownFolderAdapter`, `JSONDBAdapter`, `CustomAPIAdapter`. A `KBAdapterFactory` reads `adapter_type` from the config JSON and returns the correct concrete instance. All PBCPB business logic calls `adapter.query(...)` — never `if adapter_type == "RAG_MCP": ...`. Swapping backends = editing one config field.

---

### Mapping 2 — Control Theory / Gain Scheduling

**Source domain:** Control Theory — gain scheduling (scheduling variable → parameter set switch without interface change)
**Target domain:** PBCPB adapter_type as a scheduling variable
**Structural analog:** A gain-scheduled controller exposes a single control law interface to the plant. An external scheduling variable (altitude, speed, load) selects a pre-computed parameter set at runtime. The plant never knows which parameter set is active. The control law's mathematical form is invariant; only the parameters differ.
**What solves it in source domain:** Pre-parameterize the controller for each operating region. At runtime, read the scheduling variable, look up the parameter set, substitute into the control law. The controller-plant interface is unchanged; only the internal parameter table switches.
**Transfer to PBCPB:** `adapter_type` is the scheduling variable. PBCPB pre-parameterizes each adapter's behavior (method bodies, connection strings, delegation paths). At runtime, reading the config file = reading the scheduling variable. The `KBAdapter` interface = the invariant control law. The adapter's internal implementation = the parameter set. Critically: gain scheduling teaches that parameter sets can be interpolated (partial config migration) and that the scheduler itself must be real-time-safe (config reads must not require restart) — both directly applicable.

---

### Mapping 3 — Chemistry / Catalyst Substitution

**Source domain:** Chemistry / Catalysis — catalyst substitution without modifying the reaction equation
**Target domain:** PBCPB adapter as a catalyst mediating KB operations
**Structural analog:** The reaction equation (substrate → product via [catalyst]) is invariant from the caller's perspective. The catalyst is a third-party mediator that lowers the activation energy barrier for the transformation. Swapping the catalyst changes the reaction rate and selectivity but does not change the stoichiometry or the substrate/product identity the caller sees.
**What solves it in source domain:** Modularity of catalytic function: the reaction's thermodynamic driving force and the catalyst's mechanistic role are separated by design. Catalyst substitution is routine (heterogeneous → homogeneous → enzymatic) — the substrate-product interface is the invariant.
**Transfer to PBCPB:** The KB operation (query: question → answer, populate: content → stored, scan_gaps: schema → gap list) is the stoichiometric equation. The adapter is the catalyst. RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API differ in how they lower the "activation barrier" (latency, cost, delegation path) but the KB operation equation is invariant. Insight: just as enzyme kinetics (Michaelis-Menten) reveals that the catalyst's specificity (adapter's skill_ref or connection config) is the real operational variable — PBCPB's `connection` block in the config is the "enzyme active site" specification.

---

### Mapping 4 — Cryptography / Cipher Suite Negotiation

**Source domain:** Cryptography / Security — TLS cipher suite negotiation (two-phase: config handshake → uniform session)
**Target domain:** PBCPB adapter binding (config read phase → uniform operational phase)
**Structural analog:** TLS separates two phases: (1) handshake, where client and server exchange capabilities and agree on a cipher suite from config; (2) session, where all application data flows through the negotiated cipher's encrypt/decrypt API — which is identical regardless of which suite was negotiated. The application layer never performs cipher selection; it only sees the session API.
**What solves it in source domain:** The session abstraction encapsulates the negotiated cipher behind a uniform stream interface. The handshake produces a session object; the session object is the caller's only handle. All operational calls go through the session.
**Transfer to PBCPB:** PBCPB should implement a two-phase initialization: (1) config-binding phase where `adapter_type` and `connection` are read and an `AdapterSession` object is constructed and validated; (2) operational phase where all KB operations are called on the `AdapterSession` — never on the config directly. The `AdapterSession` is the invariant operational handle. Swapping backends = re-running phase 1 (re-reading config and constructing a new session). This pattern also naturally handles EC-2 (MARKDOWN_FOLDER with missing skill_ref fails at session construction, not at the first query).

---

### Mapping 5 — Network Science / Routing Protocols

**Source domain:** Network Science / Graph Theory — routing protocol abstraction in IP networking
**Target domain:** PBCPB method routing across adapter backends
**Structural analog:** An IP router exposes a single, uniform packet-forwarding interface to the network above it. Internally, it may run OSPF, BGP, static routes, or policy-based routing — selected by config (routing.conf, routing tables). The host sending a packet never knows which routing protocol is active. Protocol changes are operational (config file reload); no hosts need to be updated.
**What solves it in source domain:** The forwarding table abstraction hides the routing protocol behind a next-hop lookup. The routing protocol populates the forwarding table (the interface's backing data structure) but has no direct relationship with the forwarding API callers.
**Transfer to PBCPB:** Each adapter backend populates its own "forwarding table" — a routing table of method implementations. The `KBAdapter` interface is the forwarding API. `adapter_type` in config = the routing protocol selection. The bootstrap module, role system, and playbook phases are "hosts" — they only know the forwarding API. This analogy also suggests PBCPB could implement hot-reloading: just as a router can reload its routing table without dropping forwarded traffic, PBCPB could support config-file reload without restarting running playbook sessions.

</domain_mappings>

<activation_provenance>

## Activation provenance

Four conceptual nodes were activated during the domain survey. Provenance is declared per node.

---

**Node P1: "Uniform interface, divergent implementation"**
Activation sources:
- M1 (Computation/Strategy Pattern): core concept — abstract interface + concrete implementations
- M3 (Chemistry/Catalyst Substitution): reaction equation invariant while catalyst varies
- M4 (Cryptography/Cipher Suite): session API invariant while cipher varies
- M5 (Network Science/Routing): forwarding API invariant while routing protocol varies
**Classification: MULTI-SOURCE CONVERGENT** (activated from 4 independent chains)

---

**Node P2: "External configuration token drives backend dispatch"**
Activation sources:
- M2 (Control Theory/Gain Scheduling): scheduling variable is explicitly external to the controller
- M4 (Cryptography/Cipher Suite): cipher suite selected at handshake from external config exchange
- M5 (Network Science/Routing): routing protocol selected from config file external to forwarding logic
**Classification: MULTI-SOURCE CONVERGENT** (activated from 3 independent chains)

---

**Node P3: "Caller-side code remains invariant across substitutions"**
Activation sources:
- M1 (Computation): caller invokes interface; never the concrete class
- M3 (Chemistry): substrate-caller sees invariant reaction equation
- M4 (Cryptography): application layer sees only session API
- M5 (Network Science): hosts never see routing protocol
**Classification: MULTI-SOURCE CONVERGENT** (activated from 4 independent chains)

---

**Node P4: "Two-phase separation: config/negotiation phase then operational phase"**
Activation sources:
- M4 (Cryptography): explicit handshake → session separation is the defining structure of TLS
- M2 (Control Theory): gain-scheduling initialization (parameter lookup) precedes the real-time control loop
**Classification: MULTI-SOURCE CONVERGENT** (activated from 2 independent chains — threshold met)

*All four conceptual nodes have multi-source provenance. No widening of Step 2 required.*

</activation_provenance>

<conceptual_blend>

*(Step 6 tagged M4 as blend-worthy: the combination of TLS two-phase protocol × PBCPB config-and-run produces emergent structure — a stateful AdapterSession object — that exists in neither input space alone. Step 7 fires.)*

---

## Input Space 1 — TLS Cipher Suite Negotiation

**Elements:** Client, server, cipher suite list (client config), cipher suite list (server config), handshake protocol, negotiated cipher suite, session object, encrypt/decrypt operations, application data stream.

**Structure:** Two-phase protocol. Phase 1 (handshake): client and server exchange capabilities from their respective configs, agree on a mutually supported cipher suite, generate session keys. Phase 2 (session): all data flows through the session object's uniform encrypt/decrypt API. The session object is stateful — it holds the negotiated cipher and keys. Application code only ever holds a reference to the session object.

**Relevance:** TLS demonstrates that a two-phase separation between "agree on implementation" and "use implementation uniformly" is a proven, production-grade solution to the caller-invariance requirement. The session object is the artifact of the negotiation.

---

## Input Space 2 — PBCPB Runtime Backend Selection

**Elements:** PBCPB system, config file (`kb_adapter` block), `adapter_type` value, four backend implementations (RAG_MCP, MARKDOWN_FOLDER, JSON_DB, CUSTOM_API), three-method contract (query/populate/scan_gaps), bootstrap module, role system, playbook phases.

**Structure:** Single-phase current design. Config file is read; adapter_type determines which code path executes. The playbook phases call KB operations directly, with the backend selection implicit. There is no explicit session object — the config is read at call time or held as a raw dict.

**Note:** This is the original problem domain.

---

## Generic Space — Shared Skeleton

**Shared structure:**
- A configuration artifact (cipher suite config / `adapter_type` config) that encodes which implementation to use
- A uniform operational interface (encrypt/decrypt API / query-populate-scan_gaps) that callers must see
- A dispatch mechanism that connects config to implementation without exposing the mapping to callers
- A caller layer that must remain invariant across all implementation substitutions

**Cross-space mapping:**
| TLS / Input 1 | PBCPB / Input 2 |
|---|---|
| Cipher suite config | `adapter_type` + `connection` config block |
| Handshake protocol | Config-reading + adapter instantiation |
| Session object | (missing — no current equivalent) |
| Encrypt/decrypt API | query / populate / scan_gaps |
| Application code | Bootstrap module, role system, playbook phases |
| Session key material | Adapter's internal connection state (credentials, endpoint URLs, skill_ref) |

---

## Blended Space — The AdapterSession Protocol

**Selective projection:**
- From Input 1 (TLS): the two-phase separation structure; the session object as the binding artifact; the property that the session object fully encapsulates the negotiated implementation
- From Input 2 (PBCPB): the three-method KB contract; the four adapter types; the config-file-based backend selection; the bootstrap and playbook caller layers

**Operations applied:**
- **Composition:** The handshake protocol from TLS is composed with PBCPB's config-reading: the result is an explicit `AdapterSession.bind(config_path)` call that reads `adapter_type`, validates required fields (e.g., `skill_ref` for MARKDOWN_FOLDER), establishes the connection, and returns a session object.
- **Completion:** TLS background knowledge activates: session objects are reusable, can be re-negotiated without restarting, and can be validated at bind time (certificate verification → config validation). PBCPB background knowledge activates: the bootstrap module already has a phase concept (Steps 1–5), which maps naturally to post-session operations.
- **Elaboration:** Running the blend: PBCPB callers (bootstrap, role system, playbook phases) hold an `AdapterSession` handle. They call `session.query(...)`, `session.populate(...)`, `session.scan_gaps(...)` — never the config dict. Switching backends = calling `AdapterSession.bind(new_config_path)` and distributing the new session handle. EC-2 (missing skill_ref) is caught at `bind()` time, not at query time.

---

## Emergent Structure — CRITICAL SECTION

`[EMERGENT-SOLUTION]` **The AdapterSession object.** In TLS, application code holds a session object, not a cipher. In PBCPB, business logic currently holds a config dict reference, not a session. The blend produces a new artifact — `AdapterSession` — that does not exist in TLS (which has no concept of KB adapters) and does not exist in current PBCPB design (which has no explicit session lifecycle). The session object is the solution to the caller-invariance constraint: callers become structurally incapable of accessing backend-specific code because the session object exposes only the three-method interface.

`[EMERGENT-FRAME]` **Backend switching as re-negotiation, not reconfiguration.** TLS sessions can be renegotiated without dropping the connection. This frames PBCPB backend switching not as "edit config and restart" but as "re-bind the session at runtime" — supporting hot-reload of the adapter backend. Neither TLS alone (no KB concept) nor PBCPB alone (no session lifecycle) implies this framing.

`[EMERGENT-CONSTRAINT]` **Validation must occur at bind time, not at call time.** In TLS, a handshake failure (unsupported cipher) is detected immediately and surfaces before any data flows. This blend reveals that PBCPB currently defers errors (e.g., missing skill_ref) to the first query call. The blend mandates a new constraint: all configuration errors must be caught at `AdapterSession.bind()` — before any KB method is invoked. This constraint is invisible in both input spaces individually.

`[EMERGENT-ANALOGY]` **Session key material → adapter connection state.** TLS session keys are generated during the handshake and held inside the session object, never exposed to the application. This maps to: the adapter's connection state (endpoint URLs, credentials, skill_ref resolution, MCP handles) is generated during `bind()` and encapsulated inside the session, never accessible to callers. This prevents business logic from inadvertently depending on backend-specific connection details.

</conceptual_blend>

<escape_path>

## Constraint-escape path

**Selected path: M4 — Cryptography / Cipher Suite Negotiation, augmented by M1 and M2 convergence.**

**Ranking basis:** Convergence (M4 participates in all 4 multi-source-convergent nodes P1–P4) × structural fit (M4 is the only mapping that surfaces the two-phase separation, the session object, and the validation-at-bind constraint — all directly actionable for PBCPB implementation).

---

**Constraint-escape mechanism:**

The binding constraint (runtime backend switching without caller-side code changes) escapes via the following structural path:

1. **Two-phase initialization (from M4):** Separate PBCPB execution into a config-binding phase (`AdapterSession.bind(config_path)`) and an operational phase. All KB operations are called on the session object, never on raw config.

2. **Session object as the invariant handle (emergent from blend):** The `AdapterSession` exposes exactly three methods: `query()`, `populate()`, `scan_gaps()`. No backend-specific method or field is visible above the session layer. All four adapter types implement this interface.

3. **Validation at bind time (emergent constraint from blend):** EC-2 (missing skill_ref for MARKDOWN_FOLDER) and EC-6 (CUSTOM_API populate failure) are caught at `bind()`, not at call time. The session object is only returned if configuration is valid.

4. **Scheduling variable pattern (from M2):** `adapter_type` in the config JSON is treated as a scheduling variable, not a code-level switch. The factory/dispatcher reads it once at bind time and selects the concrete adapter implementation. Business logic never reads `adapter_type`.

5. **Hot-reload path (from blend elaboration):** Switching backends at runtime = calling `AdapterSession.bind(new_config)` and distributing the new session handle. No restart, no rebuild — satisfying the RUNTIME_CONFIG constraint in full.

---

**Implementation sequence derived from escape path:**

```
Step A — Define KBAdapter interface (3 methods, no others)
Step B — Implement 4 concrete adapters: RAGMCPAdapter, MarkdownFolderAdapter,
          JSONDBAdapter, CustomAPIAdapter
Step C — Implement AdapterSession: bind(config_path) → validates config,
          constructs correct concrete adapter, holds as private member, 
          exposes only the 3-method surface
Step D — Implement AdapterFactory called inside bind(): reads adapter_type,
          instantiates correct concrete class (no conditional in business logic)
Step E — Replace all PBCPB business-logic KB calls with session.method() calls
Step F — Bootstrap module (Phase 5) takes a session handle as input, 
          never touches adapter_type directly
Step G — Role system passes session handle to any KB-accessing role operation
```

**Downstream signal to N7 (domain_mappings_digest):** See SIGNAL OUTPUT block.

</escape_path>

</constraint_escape>
