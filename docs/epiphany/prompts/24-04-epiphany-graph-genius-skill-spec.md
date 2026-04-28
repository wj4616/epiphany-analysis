<prompt>
<role>senior Claude Code skill architect specializing in Graph-of-Thought orchestration and multi-subagent cognitive pipelines; deep familiarity with the epiphany-genius v1.1.0 codebase (stages S1–S7, OSP, 18-file kb/, session-init.sh, xml-assemble.sh).</role>

<context>
<meta_framing>
- create a solution developed into claude code skill which develops ultimate prompt enhancement system.
- we have developed a system very well in epiphany-genius skill which fully analyzes input text with a variety of methods, producing a report
- We actually are going to perform a task using epiphany-genius skill not touching the existing skill but using it in full as reference making a new skill called epiphany-graph-genius
- designing it to take advantages of all possible optimizations and enhancements provided if we think about it as a graph/nodes based skill design with nodes, edges, contained in a modular programatticly run design which is not taking too long to run, can take some time but not so long human user gets frustrated when enhancing simple prompt.
- we are focused on creating a system as described defining nodes, modules, edges, described in tables, creating optimizations and fixes relating to this cognitive enhancement claude code skill based system
- and orchestration logic implementing a GoT controller mechanism orchestrator which can provide enhancements and fixes, but we want to make the system flow well, not bog it down with excessive engineering that does not add value and only makes the system slower, value utility to human
- Create optimal cognitive model utilizing graph of thought AI concepts and methodologies to create the best thought process, we already have a complex system running cognitive processes, we can make this system even more ideal by implementing graph of thought
- epiphany-genius skill curtently takes input and runs it through a wide range of processes modeled after real genius mind cognitive processes and traits. we want to do the same thing, but even better by using graph and nodes graph of thought concepts
- review epiphany-genius, and develop a full integrated solution, creating a new enhanced skill by using graph of thought, keeping in mind the skill already takes almost an hour to run, we can't let it take much more time, so have to optimize as well
</meta_framing>

<vocabulary>
- Node: A pipeline processing unit. Has a stage ID, hat, module file, input ports, output ports, and scale gates.
- Module file: The `.md` file under `modules/` that defines a node's PROTOCOL. One module file per node.
- Edge: A directed data connection from one node's output file to another node's input port. Declared in the PIPELINE GRAPH.
- Port: A named attachment point on a node. INPUT ports consume files; OUTPUT ports produce files.
- Wave: A set of nodes that execute in parallel before the next sequential step.
- Hat: A cognitive role assigned to a node. Determines which model tier the node runs on.
- Tier: A model slot (model-large / model-medium / model-small). Maps to a concrete model via hats.json.
- Conditional edge: An edge that only activates when a declared signal or flag condition is true.
- Join semantics: How a node with multiple incoming edges decides when to fire (AND = wait for all; XOR = exactly one fires).
</vocabulary>

<piece_1>
[PIECE 1 — Core Goal Statement]
Design a new Claude Code agent skill called `epiphany-graph-genius`. It is a cognitive enhancement pipeline that applies all 19 Tier-1 genius-mind cognitive traits from the `epiphany-genius` skill (v1.1.0), but reimplements the execution architecture as a true Graph-of-Thought (GoT) topology. The output is a complete SKILL.md orchestrator + graph.json node/edge registry + supporting module structure. The skill must produce the same output artifacts as epiphany-genius: session directory with stage files, distilled report.md (or report.xml with --xml), and final summary line. It must accept all the same flags as epiphany-genius: --minimal, --standard, --deep, --conjecture, --xml, --quiet, --verbose, --no-save, --resume. Performance parity or better is a hard constraint: epiphany-genius takes ~1 hour at DEEP scale due to sequential subagent spawning. The new design must reduce total Agent() spawns from 8+ (DEEP) to ≤4, achieving ~30–45 minutes at DEEP scale and significantly faster at STANDARD and MINIMAL.
</piece_1>

<piece_2>
[PIECE 2 — epiphany-genius cognitive pipeline]
epiphany-genius v1.1.0 runs 7 core stages in sequential waves plus a final synthesis pass. Every stage is currently a full Agent() subagent spawn.

S1 State Loading [MINIMAL/STANDARD/DEEP, Wave 1, spawn] — Einstein/Feynman/Von Neumann/Darwin/Tesla/Turing perspectives. KB: input-preloading-templates.md. Output: primitive enumeration by category, simplicity-stripped irreducibles, Tesla measurement plan, verification block.

S2 Constraint Escape [STANDARD/DEEP, Wave 2 parallel with S3, spawn] — de Bono (lateral thinking). KB: domain-catalog.md, blend-template.md, debono-techniques.md (3 large files). Output: named constraint, 5 domain mappings, activation provenance, constraint-escape path.

S3 Peripheral Exploration [STANDARD/DEEP, Wave 2 parallel with S2, spawn] — spreading activation, elegance rubric. KB: spreading-activation.md, elegance-rubric.md. Output: activation map, convergent nodes, selected illuminations. Raises: S3_thin_or_empty.

S3.1 Representational Change / De-fixation [conditional, spawn] — Ohlsson. KB: ohlsson-defixation.md. Triggers: S3_thin_or_empty OR S6_no_alternatives (via STEP 6 retry Path B). Output: constraint relaxations, chunk decompositions, re-encodings, breakthrough candidate.

S4 Dynamic Simulation [DEEP only, Wave 3, spawn] — Tesla, observer frames. KB: observer-frames.md, simulation-checklist.md. Input deps: S1 output + raw input. Output: system model, observer frame analyses, imbalances, singularities, limit cases, theory collisions.

S5 Precision Forcing [MINIMAL/STANDARD/DEEP, Wave 4 parallel with S6, spawn] — Feynman. KB: vocabulary-rubric.md, forward-chain-template.md. Input deps: raw input + S1 output. Output: 5-level vocabulary strip, 3-representation rendering, constructive specification, flagged vague claims, forward-chain consequence tree.

S6 Falsification Engine [STANDARD/DEEP, Wave 4 parallel with S5, spawn] — Popper, Millikan. KB: tot-templates.md, falsification-checklists.md (2 large files). Input deps: S3 output + raw input + S1 output; optional: S3.1 output. Output: abductive/deductive/inductive mode results, consolidated hypotheses, extreme scenarios, Millikan assessment, density report. Raises: S6_no_alternatives.

S6.1 Conjecture [conditional on --conjecture flag, Wave 2, spawn] — Ramanujan. KB: pattern-taxonomy.md. Output: detected patterns, ≥3 conjectures, verification instances.

S7 Integration & Verification [MINIMAL/STANDARD/DEEP, Wave 5, spawn] — Feynman, Boden. KB: verification-gates.md, boden-types.md, scope-template.md, representation-frames.md, elegance-rubric.md (5 KB files). Required deps: raw input + S1 + S5. Optional deps: S2, S3, S3.1, S4, S6, S6.1. Output: convergence signals, primary conclusion, confidence band, Boden creativity type, generalization check, V1–V7 verification battery.

OSP Output Synthesis Pass [always, final, spawn] — deps: S7 + S7-v6-scope.txt + S1 + S5 + raw input. Optional: all other stage outputs. Output: distilled report.md (or skipped if --xml).

<![CDATA[
SCALE → WAVE PLANS:
MINIMAL:    S1 → S5 → S7 → OSP                     [4 spawns serial]
STANDARD:   S1 → S2+S3 → S5+S6 → S7 → OSP          [7 spawns, 2 parallel pairs]
DEEP:       S1 → S2+S3 → S4 → S5+S6 → S7 → OSP     [8 spawns, 2 parallel pairs]
CONJECTURE: S1 → S6.1 → S7 → OSP                   [4 spawns serial]
]]>

PERFORMANCE BOTTLENECK: every stage is an Agent() spawn. Each spawn requires context reconstruction from disk (session.md, stage files, KB files), costing multiple minutes. 8 sequential spawns × ~7 min average = ~1 hour.

All 19 genius-mind traits live in 18 KB files under kb/ — this directory is reused unchanged.
</piece_2>

<piece_3>
[PIECE 3 — GoT design principles]
1. AGGREGATION NODES — Multiple independent paths converge into one synthesizer.
2. FAN-OUT NODES — One node spawns multiple parallel explorations via outgoing edges.
3. BUDGETED BACK-EDGES — Output loops through refinement node with hard cap (max 1 repair).
4. TYPED EDGES WITH SIGNAL DIGESTS — 5–15 lines carrying key findings/named entities/confidence flags instead of full text dumps. Full output on disk.
5. GRAPH TOPOLOGY DECLARED IN graph.json — analogue of index.json with typed edges.
6. NODE TYPE TAXONOMY — DECOMPOSITION (N1, inline) / LATERAL (N2, spawn) / SPREADING (N3, inline default) / DEFIXATION (N3.1, inline) / SIMULATION (N4, spawn) / PRECISION (N5, inline) / ADVERSARIAL (N6, spawn) / CONJECTURE (N6.1, inline-if-small) / AGGREGATION (N7, spawn) / ROUTER (N8, always inline) / SYNTHESIS (N9, spawn).
7. CORE INSIGHT — spawn only for heavy nodes; inline role-switching at zero spawn cost for the rest. Cuts spawns from 8+ to 3–4.
</piece_3>

<piece_4>
[PIECE 4 — Performance rules O1–O8]
O1 INLINE FIRST — KB <150 lines combined AND deps in-context → inline, not spawn.
O2 SPAWN ONLY FOR HEAVY — aggregate-multi-source OR large-KB OR simulation-level.
O3 SIGNAL EDGES NOT CONTENT EDGES — 5–15 line digests; full text on disk.
O4 MAXIMUM PARALLEL FIRE — all ready-set nodes fire in a single message.
O5 CONTEXT BUDGET ENFORCEMENT — if over budget, pass digests only with "[CONTEXT TRIMMED] full output at {path}" annotation.
O6 SCALE-AWARE TOPOLOGY COLLAPSE — MINIMAL = {N1 inline, N5 inline, N7 spawn, N9 spawn}; only 2 spawns.
O7 REPAIR CAP — 1 attempt max on back-edge (N3.1 repair cycle); on failure annotate in session.md and continue to N9.
O8 INLINE KB PRE-LOAD BUDGET — ≤600 lines/wave; if over, demote KB-heavy inline node to spawn for that wave.

Target: MINIMAL ≤10 min, STANDARD ≤25 min, DEEP ≤45 min.
</piece_4>

<piece_5>
[PIECE 5 — Weaknesses in epiphany-genius]
W1 EVERY STAGE SPAWNS (primary bottleneck) — fix by inlining S1, S3, S3.1, S5, S6.1.
W2 CONTENT EDGES TOO HEAVY — fix with signal-edge S3→S6 digest ("activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]").
W3 IMPLICIT STAGE TOPOLOGY — fix with typed edges in graph.json.
W4 VERIFICATION ONLY AT END — full V1–V7 still at N7, but GoT allows micro-checks earlier.
W5 NO EXPLICIT GRAPH REGISTRY — add graph.json.
NOTE: weighted edges deferred to v2.
</piece_5>

<piece_6>
[PIECE 6 — Target architecture]

FILES:
- SKILL.md — orchestrator/graph executor loop reading graph.json topology, executing ready-set loop, running inline nodes, spawning heavy nodes.
- graph.json — node registry + typed edge declarations + scale gates.
- modules/N*.md — node protocols (rename from epiphany-genius stage modules) with added SIGNAL OUTPUT section.
- kb/ — REUSE epiphany-genius kb/ UNCHANGED (18 files).
- scripts/ — reuse session-init.sh and xml-assemble.sh unchanged; rename validate-stage.sh → validate-node.sh (same logic).

<![CDATA[
ORCHESTRATOR LOOP (SKILL.md pseudocode):
0. Parse flags → detect scale → load graph.json → filter nodes/edges by scale
1. Init session (reuse session-init.sh)
2. READY_SET = {nodes with no incoming required edges} = {N1} always
3. SIGNAL_STATE = {} (key: node_id, value: signal_digest string)
4. While READY_SET not empty:
   a. INLINE_NODES = {n in READY_SET where n.exec_type == inline}
   b. SPAWN_NODES  = {n in READY_SET where n.exec_type == spawn}
   c. Execute each inline node sequentially (role-switched block). After each: extract signal digest → SIGNAL_STATE[node_id] = digest. Write full output to disk as usual.
   d. If SPAWN_NODES not empty: fire ALL in one message (parallel Agent calls). Wait for all. Extract signal digests from returns.
   e. Evaluate ROUTER nodes (N8): read SIGNAL_STATE, determine which conditional edges are hot, add target nodes to activation queue.
   f. Update READY_SET: add any node whose ALL required incoming edges have signal digests in SIGNAL_STATE. Remove just-executed nodes.
5. Terminal node N9 (SYNTHESIS) completes → output written.
6. Run validation, observability write-back, test battery (reuse scripts).
]]>

<![CDATA[
GRAPH TOPOLOGY (graph.json sketch):

Nodes:
N1  decomposition   inline  [MINIMAL,STANDARD,DEEP]
N2  lateral         spawn   [STANDARD,DEEP]
N3  spreading       inline  [STANDARD,DEEP]  (spawn if KB pre-load over budget)
N3.1 defixation     inline  [STANDARD,DEEP]  conditional
N4  simulation      spawn   [DEEP]
N5  precision       inline  [MINIMAL,STANDARD,DEEP]
N6  adversarial     spawn   [STANDARD,DEEP]
N6.1 conjecture     inline  [any scale + --conjecture]
N7  aggregation     spawn   [MINIMAL,STANDARD,DEEP]
N8  router          inline  [STANDARD,DEEP]  (evaluates N6 output signal)
N9  synthesis       spawn   [MINIMAL,STANDARD,DEEP]

Edges (format: SOURCE → TARGET | type | scale | signal_field):
input → N1    | required  | all scales  | —
N1 → N2       | required  | STD,DEEP    | primitives_digest
N1 → N3       | required  | STD,DEEP    | primitives_digest
N1 → N4       | required  | DEEP        | system_model_seed
N1 → N5       | required  | all scales  | irreducibles_digest
N2 → N7       | optional  | STD,DEEP    | domain_mappings_digest
N3 → N6       | required  | STD,DEEP    | activation_digest
N3 → N7       | optional  | STD,DEEP    | activation_digest
N3 → N3.1     | back-edge | STD,DEEP    | gate:S3_thin_or_empty
N3.1 → N6     | optional  | STD,DEEP    | breakthrough_digest
N3.1 → N7     | optional  | STD,DEEP    | breakthrough_digest
N4 → N7       | optional  | DEEP        | simulation_digest
N5 → N7       | required  | all scales  | precision_digest
N6 → N8       | required  | STD,DEEP    | falsification_result
N8 → N3.1     | back-edge | STD,DEEP    | gate:S6_no_alternatives AND NOT N3.1_ran
N8 → N7       | gate-open | STD,DEEP    | falsification_digest (when no back-edge)
N6.1 → N7     | optional  | conjecture  | conjecture_digest
N7 → N9       | required  | all scales  | integration_digest
N9 → output   | terminal  | all scales  | —

EXECUTION TYPES AND RATIONALE:
INLINE: N1 (1 KB file, simple), N3 (2 small KB files), N3.1 (1 KB file, 700 line budget), N5 (2 KB files, 1100 line budget, only 2 input deps), N6.1 (1 KB file, small), N8 (no KB, pure signal routing).
SPAWN:  N2 (3 large KB files incl. domain-catalog), N4 (simulation-level reasoning + 2 KB files), N6 (2 large KB files + 1500 line budget + 3 input deps), N7 (5 KB files + all optional prior deps), N9 (final synthesis, always fresh context for quality).

SPAWN COUNTS BY SCALE (target vs epiphany-genius):
MINIMAL:     N7 + N9            = 2 spawns   (vs 4 in epiphany-genius)
STANDARD:    N2 + N6 + N7 + N9  = 4 spawns   (vs 7 in epiphany-genius). N2 and N6 fire in parallel (wave 2 and 4 respectively)
DEEP:        N2 + N4 + N6 + N7 + N9 = 5 spawns  (vs 8+ in epiphany-genius). N4 and N6 fire in parallel (both ready after N1+N3 complete)
CONJECTURE:  N7 + N9            = 2 spawns   (vs 4 in epiphany-genius)

PARALLEL FIRE OPPORTUNITIES:
STANDARD Wave 2:  N2 + N3 fire in parallel (both depend only on N1+input)
DEEP Wave 3:      N4 + N6 fire in parallel; N5 runs inline simultaneously (N4 deps: N1+input; N6 deps: N1+N3+input — all available)
All scales:       N5 and N6 are in the same wave (4) in epiphany-genius. In GoT they still run in parallel (same deps satisfied at the same time), but N5 is now inline instead of spawned.

OUTPUT FORMAT: identical to epiphany-genius.
{session_dir}/report.md (or report.xml with --xml)
{session_dir}/stages/N*-*.md for each node output
{session_dir}/stages/session.md with spawns_total, wall_seconds, etc.
Summary line: same format as epiphany-genius STEP 8.
All flags respected identically: --quiet, --xml, --verbose, --no-save, --resume.
]]>
</piece_6>

<domain_analogy>
Graph.json + SKILL.md is architecturally a dependency-resolving build system over cognitive stages; every node is a compilation unit; every edge is a declared include; ready-set = tasks with all deps built.
</domain_analogy>
</context>

<task>
Produce the complete file tree for a new Claude Code skill `epiphany-graph-genius` that executes the epiphany-genius 19-trait cognitive battery as a Graph-of-Thought topology declared in graph.json. Four numbered deliverables, each with an acceptance criterion:

<deliverable id="D1">
SKILL.md — orchestrator that implements the ORCHESTRATOR LOOP pseudocode verbatim from piece_6, parses flags (--minimal, --standard, --deep, --conjecture, --xml, --quiet, --verbose, --no-save, --resume), filters graph.json by scale gates, executes inline nodes as role-switched blocks and spawn nodes as parallel Agent() calls per O4, emits same summary line format as epiphany-genius STEP 8.
Acceptance: SKILL.md, when dry-run-traced, produces spawns_total ≤ 2 at MINIMAL, ≤ 4 at STANDARD, and ≤ 5 at DEEP, and yields the identical artifact tree shape ({session_dir}/report.md|xml, {session_dir}/stages/N*-*.md, {session_dir}/stages/session.md) as epiphany-genius.
</deliverable>

<deliverable id="D2">
graph.json — node registry + typed edge declarations for the 11 nodes (N1, N2, N3, N3.1, N4, N5, N6, N6.1, N7, N8, N9) and 19 edges listed in piece_6, with scale_gates per node, signal_field per edge, gate_condition on back-edges and conditional edges, and exec_type (inline|spawn) per node.
Acceptance: graph.json parses to a DAG when back-edges are excluded (PRC1), every edge's source and target resolve to a declared node id, every signal_field matches one of the 11 declared signal digests, and filtering by any scale in {MINIMAL, STANDARD, DEEP, CONJECTURE} yields a connected subgraph with N1 as source and N9 as sink.
</deliverable>

<deliverable id="D3">
modules/N*.md — one module file per node (N1.md, N2.md, N3.md, N3.1.md, N4.md, N5.md, N6.md, N6.1.md, N7.md, N8.md, N9.md) renamed from epiphany-genius stage modules, each adding a SIGNAL OUTPUT section specifying which of the 11 signal digests it emits and the exact field schema (key_findings[], named_entities[], confidence_flags[], signal_flags[]) within 5–15 lines.
Acceptance: every module file lists (a) the node's hat (cognitive role), (b) its exec_type, (c) its input ports (matching graph.json incoming edges), (d) its output ports (matching outgoing edges), (e) its scale gates, and (f) a SIGNAL OUTPUT block whose digest schema validates against the signal_digest_schema enumeration in constraints.
</deliverable>

<deliverable id="D4">
scripts/ — reuse session-init.sh unchanged, reuse xml-assemble.sh unchanged, rename validate-stage.sh → validate-node.sh with the same validation logic applied to N*-*.md files instead of S*-*.md files.
Acceptance: all three scripts are present; session-init.sh and xml-assemble.sh are byte-identical to their epiphany-genius counterparts; validate-node.sh differs only in stage→node naming and validates the same well-formedness properties (required sections present, signal digest present, within line budget).
</deliverable>
</task>

<constraints>
<constraint id="C1">All 19 Tier-1 genius-mind cognitive traits from epiphany-genius v1.1.0 must be executed by the 11 nodes; no trait may be dropped. Use the original hat assignments (Einstein/Feynman/Von Neumann/Darwin/Tesla/Turing for N1; de Bono for N2; spreading activation + elegance rubric for N3; Ohlsson for N3.1; Tesla + observer frames for N4; Feynman for N5; Popper + Millikan for N6; Ramanujan for N6.1; Feynman + Boden for N7).</constraint>
<constraint id="C2">kb/ directory is reused UNCHANGED from epiphany-genius (18 files). Do not modify, add, or remove any KB file. The 18 KB files referenced across the pipeline include input-preloading-templates.md, domain-catalog.md, blend-template.md, debono-techniques.md, spreading-activation.md, elegance-rubric.md, ohlsson-defixation.md, observer-frames.md, simulation-checklist.md, vocabulary-rubric.md, forward-chain-template.md, tot-templates.md, falsification-checklists.md, pattern-taxonomy.md, verification-gates.md, boden-types.md, scope-template.md, representation-frames.md.</constraint>
<constraint id="C3">[INPUT-DIRECTIVE] Spawn-count contradiction: Piece 1 declares "reduce total Agent() spawns from 8+ (DEEP) to ≤4" while Piece 6 declares DEEP = N2 + N4 + N6 + N7 + N9 = 5 spawns. This conflict is NOT resolved at synthesis; the implementer MUST, at first detection: (a) surface a contradiction report naming both source constraints and their interaction, (b) propose resolution options (Option A: relax Piece 1 to ≤5 for DEEP while preserving ≤4 for STANDARD and ≤2 for MINIMAL/CONJECTURE; Option B: collapse one DEEP spawn by inlining N4 or merging N4 content into N6; Option C: accept 5 with documented rationale), (c) select an option, (d) record the selected option in session.md under `spawn_budget_resolution: {option, rationale, chosen_at_phase}`, and (e) continue with that option as the operative spawn cap. Do NOT halt the entire build; flag, choose, document, proceed. See T13 escape hatch in edge_cases for the reporting template.</constraint>
<constraint id="C4">All epiphany-genius flags must be accepted and respected identically: --minimal, --standard, --deep, --conjecture, --xml, --quiet, --verbose, --no-save, --resume. No flag may change meaning or be silently dropped.</constraint>
<constraint id="C5">Apply optimization rules O1–O8 verbatim from piece_4. O1 INLINE FIRST (KB <150 lines combined AND deps in-context → inline). O2 SPAWN ONLY FOR HEAVY (aggregate-multi-source OR large-KB OR simulation-level). O3 SIGNAL EDGES NOT CONTENT EDGES (5–15 line digests). O4 MAXIMUM PARALLEL FIRE (all ready-set nodes fire in a single message). O5 CONTEXT BUDGET ENFORCEMENT ("[CONTEXT TRIMMED] full output at {path}"). O6 SCALE-AWARE TOPOLOGY COLLAPSE (MINIMAL = {N1 inline, N5 inline, N7 spawn, N9 spawn} = 2 spawns). O7 REPAIR CAP (1 attempt max on back-edge N3.1; on failure annotate session.md and continue to N9). O8 INLINE KB PRE-LOAD BUDGET (≤600 lines/wave; over budget → demote KB-heavy inline node to spawn for that wave).</constraint>
<constraint id="C6">Fix the five weaknesses W1–W5 from piece_5. W1 EVERY STAGE SPAWNS → inline S1/N1, S3/N3, S3.1/N3.1, S5/N5, S6.1/N6.1. W2 CONTENT EDGES TOO HEAVY → emit signal-edge N3→N6 digest exactly of the form "activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]". W3 IMPLICIT STAGE TOPOLOGY → typed edges in graph.json. W4 VERIFICATION ONLY AT END → keep full V1–V7 at N7 but allow micro-checks earlier. W5 NO EXPLICIT GRAPH REGISTRY → add graph.json. Weighted edges are EXPLICITLY deferred to v2.</constraint>
<constraint id="C7">Performance targets are hard: MINIMAL ≤10 min, STANDARD ≤25 min, DEEP ≤45 min (per O8 block in piece_4). Any design change that risks exceeding these must be logged.</constraint>
<constraint id="C8">Output artifacts must match epiphany-genius structure exactly: {session_dir}/report.md (or report.xml with --xml), {session_dir}/stages/N*-*.md per node, {session_dir}/stages/session.md with spawns_total and wall_seconds fields, and a summary line in the identical format as epiphany-genius STEP 8.</constraint>
<constraint id="C9">Back-edges and conditional edges obey declared gates. N3 → N3.1 back-edge fires on gate:S3_thin_or_empty. N8 → N3.1 back-edge fires on gate:S6_no_alternatives AND NOT N3.1_ran. N8 → N7 gate-open edge fires when no back-edge was selected. N6.1 → N7 fires only under --conjecture scale filter.</constraint>
<constraint id="C10">All 11 signal digests enumerated in signal_digest_schema are 5–15 lines max, carry key findings + named entities + confidence flags per O3, and full text is always persisted to disk regardless of digest use.</constraint>
<constraint id="C11">Modules must expose a SIGNAL OUTPUT section. Module files are one-per-node and live under modules/. Hats map to a Tier via hats.json ONLY in v2 (see hat_tier_v2_defer); v1 uses default Claude Code main-model tier for all nodes regardless of hat.</constraint>
<constraint id="C12">Router state mechanism: session.md contains a field `executed_nodes: [list of node_id]` updated after each node completes. N8's gate condition `NOT N3.1_ran` reads this field exclusively (executed_nodes is the single source of truth for node-completion state). SIGNAL_STATE is an in-memory working map during a run and is only reconstructed from on-disk stage files when --resume is used. SIGNAL_STATE is keyed by (node_id, signal_field) — a node that emits multiple digests (e.g., N1 emits primitives_digest, irreducibles_digest, system_model_seed) writes three separate SIGNAL_STATE entries: SIGNAL_STATE[("N1","primitives_digest")], SIGNAL_STATE[("N1","irreducibles_digest")], SIGNAL_STATE[("N1","system_model_seed")]. Equivalent implementations may nest as SIGNAL_STATE[node_id][signal_field] — the key invariant is that one node-id can hold N distinct digest payloads indexed by signal_field.</constraint>
<constraint id="C13">Scripts are reused. session-init.sh unchanged. xml-assemble.sh unchanged. validate-stage.sh → renamed validate-node.sh with the same validation logic (no behavior change beyond naming).</constraint>
<constraint id="C14">N9 OSP input parity: the SYNTHESIS spawn N9 receives N7's integration_digest via its required incoming edge, and is additionally given file paths to Read from disk during spawn execution: {session_dir}/stages/N1-*.md (S1 equivalent), {session_dir}/stages/N5-*.md (S5 equivalent), {session_dir}/stages/N7-*.md (S7 equivalent), {session_dir}/stages/N7-v6-scope.txt (scope file emitted by N7, filename parity with epiphany-genius S7-v6-scope.txt but N-numbered), and the raw input file. Optional disk reads: all other N*-*.md stage files present at run time. This preserves OSP input parity with epiphany-genius (S7 + S7-v6-scope.txt + S1 + S5 + raw_input + optional all-others) without inflating the signal edge into a content edge — full content comes via Read, not via the digest channel.</constraint>
<constraint id="C15">N6.1 scale_gate notation: the sentinel `["CONJECTURE_FLAG"]` in graph.json indicates the node activates whenever `--conjecture` is set, regardless of the scale flag (MINIMAL | STANDARD | DEEP). This is distinct from scale values MINIMAL/STANDARD/DEEP/CONJECTURE. The source piece_6 CDATA notation `[any scale + --conjecture]` is preserved verbatim inside piece_6 as authoritative source text; in graph.json, it is expressed as the CONJECTURE_FLAG sentinel. Filter rule: a node is active iff (scale ∈ node.scale_gates) OR ("CONJECTURE_FLAG" ∈ node.scale_gates AND --conjecture is set).</constraint>

<signal_digest_schema>
All digests share the required-field schema: key_findings[], named_entities[], confidence_flags[], signal_flags[]. All digests cap at 5–15 lines.

- primitives_digest — emitted by N1; consumed by N2, N3; required fields as above; max 5–15 lines. (Per edge table: N1→N2 and N1→N3 carry primitives_digest; N1→N5 carries irreducibles_digest — N5 is NOT a primitives_digest consumer.)
- irreducibles_digest — emitted by N1; consumed by N5 (and N7 via N5 chain); required fields as above; max 5–15 lines.
- system_model_seed — emitted by N1; consumed by N4 (DEEP only); required fields as above; max 5–15 lines.
- activation_digest — emitted by N3; consumed by N6 (required) and N7 (optional); required fields as above; max 5–15 lines; must contain the exact W2 form "activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]".
- breakthrough_digest — emitted by N3.1; consumed by N6 (optional) and N7 (optional); required fields as above; max 5–15 lines.
- simulation_digest — emitted by N4; consumed by N7 (optional, DEEP only); required fields as above; max 5–15 lines.
- precision_digest — emitted by N5; consumed by N7 (required); required fields as above; max 5–15 lines.
- falsification_result — emitted by N6; consumed by N8 (required); required fields as above plus signal_flag S6_no_alternatives; max 5–15 lines. Edge N8→N7 references signal_field `falsification_digest`, which is a passthrough alias for falsification_result emitted verbatim by N8 when the back-edge is not selected. The two names refer to the same digest payload.
- domain_mappings_digest — emitted by N2; consumed by N7 (optional); required fields as above; max 5–15 lines.
- conjecture_digest — emitted by N6.1; consumed by N7 (optional, --conjecture only); required fields as above; max 5–15 lines.
- integration_digest — emitted by N7; consumed by N9 (required); required fields as above; max 5–15 lines.
</signal_digest_schema>

<join_semantics>
- N7 AGGREGATION: AND-join on its single required incoming edge {N5 precision_digest}. N1 content reaches N7 transitively (N1 emissions are captured in the precision_digest that N5 produces, and N7 may Read the on-disk N1-*.md stage file directly to reconstruct raw S1 content when needed). Non-blocking optional-edge read on {N2 domain_mappings_digest, N3 activation_digest, N3.1 breakthrough_digest, N4 simulation_digest, N6.1 conjecture_digest, N8 falsification_digest}: N7 fires as soon as the required edge is satisfied; optional digests are included if present at fire time, otherwise skipped.
- N9 SYNTHESIS: trivial join — 1 required incoming edge (N7 integration_digest).
- N3.1 DEFIXATION: XOR-join of back-edges — fires if (N3 → N3.1 back-edge hot via gate:S3_thin_or_empty) XOR (N8 → N3.1 back-edge hot via gate:S6_no_alternatives AND NOT N3.1_ran). The two back-edges are mutually exclusive: at most one fires per run, enforced by the `NOT N3.1_ran` router condition.
- All other nodes: trivial joins (≤1 required incoming edge).
</join_semantics>

<hat_tier_v2_defer>
Hat and Tier vocabulary are RECOGNIZED in this spec but DEFERRED to v2. v1 ships WITHOUT hats.json. All nodes run on the default Claude Code main-model tier. Node module files still declare a hat field (cognitive role label) for documentation and future v2 mapping, but the orchestrator does not route on it. Tier slots (model-large / model-medium / model-small) are part of the vocabulary for forward compatibility only.
</hat_tier_v2_defer>
</constraints>

<output_format>
Deliver the artifact tree:

```
.claude/skills/epiphany-graph-genius/
├── SKILL.md
├── graph.json
├── modules/
│   ├── N1.md
│   ├── N2.md
│   ├── N3.md
│   ├── N3.1.md
│   ├── N4.md
│   ├── N5.md
│   ├── N6.md
│   ├── N6.1.md
│   ├── N7.md
│   ├── N8.md
│   └── N9.md
├── kb/
│   └── (18 files, symlink or copy from epiphany-genius — UNCHANGED)
└── scripts/
    ├── session-init.sh      (reused unchanged)
    ├── xml-assemble.sh      (reused unchanged)
    └── validate-node.sh     (renamed from validate-stage.sh)
```

graph.json JSON schema sketch:

```json
{
  "nodes": [
    {
      "id": "N1",
      "type": "DECOMPOSITION",
      "exec_type": "inline",
      "kb_files": ["input-preloading-templates.md"],
      "context_budget_lines": 150,
      "scale_gates": ["MINIMAL", "STANDARD", "DEEP"]
    }
  ],
  "edges": [
    {
      "source": "N1",
      "target": "N2",
      "type": "required",
      "signal_field": "primitives_digest",
      "scale_gates": ["STANDARD", "DEEP"],
      "gate_condition": null
    },
    {
      "source": "N3",
      "target": "N3.1",
      "type": "back-edge",
      "signal_field": "gate:S3_thin_or_empty",
      "scale_gates": ["STANDARD", "DEEP"],
      "gate_condition": "SIGNAL_STATE[N3].signal_flags includes S3_thin_or_empty"
    }
  ]
}
```

Every node object carries: id, type (one of DECOMPOSITION/LATERAL/SPREADING/DEFIXATION/SIMULATION/PRECISION/ADVERSARIAL/CONJECTURE/AGGREGATION/ROUTER/SYNTHESIS), exec_type (inline|spawn), kb_files (array), context_budget_lines (integer per O8 ≤600/wave), scale_gates (array subset of {MINIMAL, STANDARD, DEEP, CONJECTURE}).

Every edge object carries: source (node id or "input"), target (node id or "output"), type (required|optional|back-edge|gate-open|terminal), signal_field (one of the 11 declared digests or a gate literal or "—"), scale_gates (array), gate_condition (string expression over SIGNAL_STATE and executed_nodes, or null).

Output identical to epiphany-genius: {session_dir}/report.md (or report.xml with --xml), {session_dir}/stages/N*-*.md per node, {session_dir}/stages/session.md with spawns_total and wall_seconds, summary line in epiphany-genius STEP 8 format.
</output_format>

<verification>
<v_gate id="V1">Primary conclusion is stated in terms of N1 irreducibles (Feynman precision check from epiphany-genius V1).</v_gate>
<v_gate id="V2">At least one falsifier exists and is recorded (Popper gate from epiphany-genius V2).</v_gate>
<v_gate id="V3">Boden creativity type is classified (combinational/exploratory/transformational per epiphany-genius V3).</v_gate>
<v_gate id="V4">Scope statement declares what is in and out of frame (epiphany-genius V4).</v_gate>
<v_gate id="V5">At least two independent representations agree on the conclusion (epiphany-genius V5 Feynman triangulation).</v_gate>
<v_gate id="V6">Extreme/boundary cases have been probed (epiphany-genius V6 limit-case check).</v_gate>
<v_gate id="V7">Generalization check: the conclusion is tested against at least one out-of-distribution instance (epiphany-genius V7).</v_gate>

<pre_run_check id="PRC1">graph.json parses to a valid DAG when back-edges ({N3 → N3.1, N8 → N3.1}) are excluded; all edge source/target ids resolve to declared nodes; every signal_field references a declared digest in signal_digest_schema or a declared gate literal.</pre_run_check>
<pre_run_check id="PRC2">SIGNAL_STATE is append-only within a single run: a node_id key, once written, is never overwritten (enforced by the orchestrator loop step 4c and 4d). Violation → abort with explicit error.</pre_run_check>

<post_run_assertion id="PRA1">spawns_total ≤ target per scale — MINIMAL ≤ 2, STANDARD ≤ 4, DEEP ≤ 5, CONJECTURE ≤ 2. Measured from session.md spawns_total field after the run completes.</post_run_assertion>
<post_run_assertion id="PRA2">Artifact parity diff vs epiphany-genius reference session: {session_dir}/report.md (or report.xml), {session_dir}/stages/N*-*.md, {session_dir}/stages/session.md shape and key fields must match epiphany-genius output shape. The summary line must match epiphany-genius STEP 8 format exactly.</post_run_assertion>

<inverse_verification id="IV1">Differential test against epiphany-genius DEEP (contract A1, anti-conformity:inversion): run the new epiphany-graph-genius DEEP pipeline on the SAME input used to validate epiphany-genius DEEP; diff stage-level outputs at matched stages (S1 vs N1, S5 vs N5, S7 vs N7). Semantic drift is measured per matched-stage pair as the MAX of three checks — FAIL migration if ANY check fails at ANY matched stage: (a) key_findings Jaccard similarity < 0.80 (tokenize on whitespace+punctuation, lowercase, stopword-removed; count shared vs union); (b) named_entities set Jaccard similarity < 0.80 (case-preserving; exact-string match); (c) primary-conclusion confidence band divergence (different Millikan band OR different Boden creativity type OR V1–V7 pass/fail vector differs in ≥2 gates). Rationale: static parity (PRA2) misses cognitive-fidelity regressions, so a differential test catches quality loss hidden by shape-matching artifacts.</inverse_verification>
</verification>

<edge_cases>
<edge_case id="EC1">W1 contradiction between "inline S1/N1, S3/N3, S3.1/N3.1, S5/N5, S6.1/N6.1" and any concrete demand to spawn one of them → escalate to T13 escape hatch (see escape_hatch below). Do not resolve silently; surface the contradiction with both source constraints labeled.</edge_case>
<edge_case id="EC2">O8 demotion tie-breaker: if the ≤600-line inline KB pre-load budget is exceeded in a wave and multiple inline nodes could be demoted to spawn, the largest kb_preload (most KB lines) wins the demotion. Ties broken by node_id lexicographic order.</edge_case>
<edge_case id="EC3">N8 gate `NOT N3.1_ran` reads session.md.executed_nodes (the single source of truth per C12). If executed_nodes does NOT contain "N3.1", then N3.1_ran = false and the N8 → N3.1 back-edge is eligible. Otherwise the back-edge is blocked. SIGNAL_STATE is not consulted for this check — it is an in-memory working map and may be absent on --resume before N3.1's digest is re-hydrated from disk.</edge_case>
<edge_case id="EC4">Repair cap hit (per O7): if the N3.1 back-edge has already fired once within this run, any subsequent attempt is blocked. Node marks the situation as best-effort, annotates in session.md under a repair_cap_hit entry, and the orchestrator continues to N9 without further retries.</edge_case>
<edge_case id="EC5">--resume recomputes READY_SET from on-disk stage files: scan {session_dir}/stages/ for existing N*-*.md files, populate SIGNAL_STATE from their embedded SIGNAL OUTPUT blocks, mark their node_ids in executed_nodes, then run the ORCHESTRATOR LOOP step 4f to seed READY_SET with any node whose required incoming edges are already satisfied on disk.</edge_case>
<edge_case id="EC6">--minimal + --conjecture combination: per O6 MINIMAL collapses to {N1 inline, N5 inline, N7 spawn, N9 spawn}; N6.1 fires inline under --conjecture; N3 and N6 remain suppressed (not in MINIMAL scale_gates). Result: spawn count stays at 2 (N7, N9); N6.1 adds one inline step only.</edge_case>
<edge_case id="EC7">Malformed signal digest: if a node emits a digest that fails the signal_digest_schema validation (missing required field, over 15-line cap, wrong format), mark the digest as missing in SIGNAL_STATE, block any dependent nodes that required it, log a warning in session.md, and continue with non-blocking optional-edge reads where applicable. Do NOT silently pass a malformed digest.</edge_case>

<escape_hatch id="T13_contract_8">
On detecting the Piece 1 / Piece 6 spawn-count contradiction (≤4 spawns vs DEEP=5 spawns, see C3), the implementer produces a contradiction report naming BOTH constraints verbatim, describes their interaction (Piece 1's ≤4 cap is infringed by Piece 6's DEEP topology of N2+N4+N6+N7+N9=5), proposes resolution options (Option A: relax Piece 1 to ≤5 for DEEP while keeping ≤4 for STANDARD; Option B: collapse N4 into N6 or inline one of the DEEP spawns to reach ≤4; Option C: accept 5 with documented rationale), selects one, records the selection in session.md under `spawn_budget_resolution`, and continues. Does not ship silently; does not halt the build.
</escape_hatch>

<counterfactual id="A2_contract">
If, during development, the ≤4-spawn target (or the agreed resolution from T13) is found to cost measurable output-quality degradation versus the epiphany-genius baseline DEEP on the IV1 cross-check (>20% semantic drift OR user-visible regression on V1–V7 battery), the implementer must surface the trade-off with an a/b quality delta report (epiphany-genius baseline vs epiphany-graph-genius output side-by-side) rather than ship silently. Rationale: T13 handles declared-constraint conflicts; A2 catches constraints discovered only at test time. Does not ship silently.
</counterfactual>
</edge_cases>
</prompt>
