# epiphany-graph-genius — Design Input Pieces for prompt-graph
# Paste any/all of these into /prompt-graph as raw material.
# Audited and corrected from initial draft — 2026-04-24

---

## PIECE 1 — Core Goal Statement

Design a new Claude Code agent skill called `epiphany-graph-genius`. It is a
cognitive enhancement pipeline that applies all 19 Tier-1 genius-mind cognitive
traits from the `epiphany-genius` skill (v1.1.0), but reimplements the execution
architecture as a true Graph-of-Thought (GoT) topology. The output is a complete
SKILL.md orchestrator + graph.json node/edge registry + supporting module
structure. The skill must produce the same output artifacts as epiphany-genius:
session directory with stage files, distilled report.md (or report.xml with
--xml), and final summary line. It must accept all the same flags as
epiphany-genius: --minimal, --standard, --deep, --conjecture, --xml, --quiet,
--verbose, --no-save, --resume. Performance parity or better is a hard
constraint: epiphany-genius takes ~1 hour at DEEP scale due to sequential
subagent spawning. The new design must reduce total Agent() spawns from 8+
(DEEP) to ≤4, achieving ~30–45 minutes at DEEP scale and significantly faster
at STANDARD and MINIMAL.

---

## PIECE 2 — What epiphany-genius does (cognitive pipeline to replicate)

epiphany-genius v1.1.0 runs 7 core stages in sequential waves plus a final
synthesis pass. Every stage is currently a full Agent() subagent spawn, which
is the primary source of the ~1 hour wall time.

STAGES AND WHAT THEY DO:

  S1 State Loading [MINIMAL/STANDARD/DEEP, Wave 1, spawn]
     Genius perspectives: Einstein (irreducible primitives), Feynman (jargon-free
     enumeration), Von Neumann (≥20 items per category, no compression), Darwin
     (evidence first), Tesla (measurable success plan), Turing (constructive spec).
     KB: input-preloading-templates.md
     Output: primitive enumeration by category, simplicity-stripped irreducibles,
     Tesla measurement plan, verification block.

  S2 Constraint Escape [STANDARD/DEEP, Wave 2 parallel with S3, spawn]
     Genius perspectives: de Bono (lateral thinking, domain analogy).
     KB: domain-catalog.md, blend-template.md, debono-techniques.md (3 large files)
     Output: named constraint, 5 domain mappings, activation provenance,
     constraint-escape path.

  S3 Peripheral Exploration [STANDARD/DEEP, Wave 2 parallel with S2, spawn]
     Genius perspectives: spreading activation, elegance rubric.
     KB: spreading-activation.md, elegance-rubric.md
     Output: activation map, convergent nodes, selected illuminations.
     Raises: S3_thin_or_empty signal if content is insufficient.

  S3.1 Representational Change / De-fixation [conditional, spawn]
     Genius perspectives: Ohlsson (representational change theory).
     KB: ohlsson-defixation.md
     Triggers: S3_thin_or_empty OR S6_no_alternatives (via STEP 6 retry Path B).
     Output: constraint relaxations, chunk decompositions, re-encodings,
     breakthrough candidate.

  S4 Dynamic Simulation [DEEP only, Wave 3, spawn]
     Genius perspectives: Tesla (full system simulation before execution),
     observer frames.
     KB: observer-frames.md, simulation-checklist.md
     Input deps: S1 output + raw input.
     Output: system model, observer frame analyses, imbalances, singularities,
     limit cases, theory collisions.

  S5 Precision Forcing [MINIMAL/STANDARD/DEEP, Wave 4 parallel with S6, spawn]
     Genius perspectives: Feynman (vocabulary stripping, constructive spec).
     KB: vocabulary-rubric.md, forward-chain-template.md
     Input deps: raw input + S1 output.
     Output: 5-level vocabulary strip, 3-representation rendering, constructive
     specification, flagged vague claims, forward-chain consequence tree.

  S6 Falsification Engine [STANDARD/DEEP, Wave 4 parallel with S5, spawn]
     Genius perspectives: Popper (falsification), Millikan (density assessment).
     KB: tot-templates.md, falsification-checklists.md (2 large files)
     Input deps: S3 output + raw input + S1 output; optional: S3.1 output.
     Output: abductive/deductive/inductive mode results, consolidated hypotheses,
     extreme scenarios, Millikan assessment, density report.
     Raises: S6_no_alternatives if zero alternative hypotheses found.

  S6.1 Conjecture [conditional on --conjecture flag, Wave 2, spawn]
     Genius perspectives: Ramanujan (pattern→conjecture).
     KB: pattern-taxonomy.md
     Output: detected patterns, ≥3 conjectures, verification instances.

  S7 Integration & Verification [MINIMAL/STANDARD/DEEP, Wave 5, spawn]
     Genius perspectives: Feynman, Boden (creativity classification).
     KB: verification-gates.md, boden-types.md, scope-template.md,
     representation-frames.md, elegance-rubric.md (5 KB files)
     Required input deps: raw input + S1 + S5.
     Optional input deps: S2, S3, S3.1, S4, S6, S6.1.
     Output: convergence signals, primary conclusion, confidence band,
     Boden creativity type, generalization check, V1–V7 verification battery.

  OSP Output Synthesis Pass [always, final, spawn]
     Input deps: S7 + S7-v6-scope.txt + S1 + S5 + raw input.
     Optional: all other stage outputs.
     Output: distilled final report (report.md) or skipped if --xml.

SCALE → WAVE PLANS in epiphany-genius:
  MINIMAL:    S1 → S5 → S7 → OSP                     [4 spawns serial]
  STANDARD:   S1 → S2+S3 → S5+S6 → S7 → OSP          [7 spawns, 2 parallel pairs]
  DEEP:       S1 → S2+S3 → S4 → S5+S6 → S7 → OSP     [8 spawns, 2 parallel pairs]
  CONJECTURE: S1 → S6.1 → S7 → OSP                   [4 spawns serial]

PERFORMANCE BOTTLENECK: every stage is an Agent() spawn. Each spawn requires
context reconstruction from disk (session.md, stage files, KB files), costing
multiple minutes. 8 sequential spawns × ~7 min average = ~1 hour.

All 19 genius-mind traits live in 18 KB files under kb/ — this directory is
reused unchanged in the new design.

---

## PIECE 3 — Graph-of-Thought design principles to exploit

Graph-of-Thought (GoT) differs from Chain-of-Thought and Tree-of-Thought in
these architecturally exploitable ways:

1. AGGREGATION NODES
   Multiple independent reasoning paths converge into a single synthesizer. In
   a chain, each step sees only its predecessor. In GoT, a synthesis node
   receives outputs from N parallel branches simultaneously, producing richer
   conclusions with no sequential bottleneck.

2. FAN-OUT NODES
   One node's output spawns multiple parallel explorations via outgoing edges.
   This replaces epiphany-genius "parallel waves" with a first-class typed
   relationship: the fan-out IS the graph topology, not a timing coincidence.

3. BUDGETED BACK-EDGES (refinement loops)
   A node's output can loop back through a refinement node (critique + improve).
   In GoT this is a declared edge with a hard cap (max 1 repair attempt), not
   an ad-hoc retry. Prevents infinite loops while still allowing recovery.

4. TYPED EDGES WITH SIGNAL DIGESTS
   This is the key performance optimization. Edges carry typed cognitive
   SIGNAL DIGESTS (5–15 lines: key findings, named entities, confidence flags)
   rather than full stage text dumps. Full output is written to disk; the
   orchestrator reads the digest for routing decisions and passes it to
   downstream nodes instead of 500–1500 lines of full output. This slashes
   context size at synthesis nodes and enables inline execution of more nodes.

5. GRAPH TOPOLOGY DECLARED IN graph.json
   The analogue of epiphany-genius's index.json — but instead of declaring
   stages and waves, it declares NODES and EDGES with: source, target, edge
   type (required/optional/conditional/back-edge), signal field name, and
   scale gate. The SKILL.md orchestrator is a graph executor that reads this
   topology and fires nodes as their input edges are satisfied.

6. NODE TYPE TAXONOMY

   DECOMPOSITION  — breaks input into irreducibles (S1 → N1)
                    Execution: INLINE (KB: 1 file, manageable context)
   LATERAL        — constraint escape via domain analogy (S2 → N2)
                    Execution: SPAWN (KB: 3 large files including domain-catalog)
   SPREADING      — fan-out activation map through semantic space (S3 → N3)
                    Execution: INLINE if KB files small; SPAWN if large
                    KB: spreading-activation.md, elegance-rubric.md
   DEFIXATION     — representational change, conditional (S3.1 → N3.1)
                    Execution: INLINE (1 KB file, 700 line budget)
   SIMULATION     — observer-frame system dynamics (S4 → N4)
                    Execution: SPAWN (heavy reasoning, 2 KB files, DEEP only)
   PRECISION      — vocabulary stripping + constructive spec (S5 → N5)
                    Execution: INLINE (2 KB files, 1100 line budget, simple deps)
   ADVERSARIAL    — falsification + alternative hypothesis (S6 → N6)
                    Execution: SPAWN (2 large KB files, 1500 line budget)
   CONJECTURE     — Ramanujan pattern→conjecture (S6.1 → N6.1)
                    Execution: INLINE if KB file small; SPAWN otherwise
   AGGREGATION    — convergence + V1–V7 verification (S7 → N7)
                    Execution: SPAWN (5 KB files + all prior optional deps)
   ROUTER         — conditional gate, evaluates signals, decides edges (N8)
                    Execution: always INLINE (no KB, just signal evaluation)
   SYNTHESIS      — final distilled output (OSP → N9)
                    Execution: SPAWN (aggregates all prior + 5 file deps)

7. THE CORE INSIGHT
   In epiphany-genius, every stage is a separate spawn. In GoT, only nodes
   that need large KB files or heavy multi-source aggregation need to spawn.
   Simple analysis nodes run orchestrator-inline via role-switching at zero
   spawn cost. The orchestrator pre-loads small KB files once and passes their
   content to inline role-switched blocks. This cuts spawns from 8+ to 3–4.

---

## PIECE 4 — Performance optimization rules

Performance budget: total execution must not exceed epiphany-genius DEEP (~1 hour).
Target: MINIMAL ≤ 10 min, STANDARD ≤ 25 min, DEEP ≤ 45 min.
Rationale: epiphany-genius DEEP has 8 spawns averaging ~7 min each. Reducing
to 4 spawns with inline execution of the rest targets ~30–40 min.

RULE O1 — INLINE FIRST
Any node that (a) does NOT require loading large KB files (>150 lines combined)
AND (b) whose input deps are already available in the orchestrator's working
context MUST run orchestrator-inline (role-switched block), not as an Agent()
spawn. Spawning costs ~5–10 minutes per invocation for context reconstruction.
The orchestrator MAY pre-load small KB files itself and pass them to inline
role blocks.

RULE O2 — SPAWN ONLY FOR HEAVY NODES
Reserve Agent() spawns for nodes that: (a) aggregate multiple large stage
outputs + multiple KB sources simultaneously (AGGREGATION, SYNTHESIS), OR
(b) require large KB files that would overflow orchestrator context (LATERAL,
ADVERSARIAL), OR (c) perform complex simulation-level reasoning (SIMULATION).

RULE O3 — SIGNAL EDGES, NOT CONTENT EDGES
When a completed node passes output to a downstream node, pass a SIGNAL DIGEST
(5–15 lines: key findings, named entities, signal flags, confidence level) not
the full output text. Full text is written to disk as a stage file. Signal
digests are extracted inline by the orchestrator and stored in session state.
Downstream inline nodes receive the digest. Downstream spawn nodes receive
the digest + are told to read the full file from disk if they need it.

RULE O4 — MAXIMUM PARALLEL FIRE
All nodes whose input edge requirements are simultaneously satisfied fire in a
single message with multiple Agent() calls. In DEEP scale, N4 (Simulation) and
N6 (Adversarial) can fire in parallel because they share the same input deps
(N1 + N3 outputs, which are both complete before wave 3). Do not serialize
nodes that have no dependency on each other.

RULE O5 — CONTEXT BUDGET ENFORCEMENT
Each node declares context_budget_lines (inherited from epiphany-genius
index.json). Before spawning, the orchestrator checks whether the sum of
input signal digest lines + KB file lines exceeds budget. If over budget:
pass signal digests only (not full file contents) and annotate the spawn prompt
with "[CONTEXT TRIMMED] full output at {path}; summary follows."

RULE O6 — SCALE-AWARE TOPOLOGY COLLAPSE
At MINIMAL scale, the graph collapses to N1(inline) → N5(inline) → N7(spawn)
→ N9(spawn). Only 2 spawns. No S2, S3, S4, S6 nodes are instantiated.
Conditional nodes (N3.1, N8 router) do not activate in MINIMAL.

RULE O7 — REPAIR CAP
Back-edge repair nodes (N3.1 when triggered by S6_no_alternatives Path B)
have a hard cap of 1 attempt. If the repair cycle (N3.1 → N6 re-run → N7
re-run) fails to produce ≥1 alternative, accept best-effort output, annotate
in session.md, and continue to N9. No further retries.

RULE O8 — INLINE KB PRE-LOAD BUDGET
The orchestrator may pre-load KB files for inline nodes if the total KB
pre-load across all inline nodes in a wave does not exceed 600 lines. If it
does, demote the KB-heavy inline node to a spawn for that wave.

---

## PIECE 5 — Key structural weaknesses in epiphany-genius to fix

WEAKNESS 1 — EVERY STAGE IS A SPAWN (primary bottleneck)
epiphany-genius spawns every stage as a full subagent, even stages with small
KB files and simple input deps (S1, S3, S3.1, S5). The new design runs these
inline, eliminating 3–5 spawns at DEEP scale.

WEAKNESS 2 — CONTENT EDGES ARE TOO HEAVY
S6 receives the full S3 output (up to 1000 lines) as an input_dependency. In
GoT, N3→N6 carries only a SIGNAL EDGE: "activation map found: [nodes]; top
convergent: [X,Y,Z]; activation strength: [H/M/L]." The full S3 text stays on
disk for S7/N7 to read if needed. Context at N6 drops from 1000+ lines to ~10.

WEAKNESS 3 — STAGE TOPOLOGY IS IMPLICIT
The wave plan encodes ordering but not WHY dependencies exist. GoT makes it
explicit: each edge has a type (feeds-into / refines / aggregates / gates /
back-edge) and a signal field, so the orchestrator knows what cognitive
artifact flows where and can extract the right digest.

WEAKNESS 4 — VERIFICATION ONLY AT END
V1–V7 battery runs only in S7 after all other stages complete. In GoT, a
micro-verification check can fire after each aggregation node, catching issues
before they propagate downstream. Full battery still runs at N7; micro-checks
at earlier aggregation points are lightweight inline checks.

WEAKNESS 5 — NO EXPLICIT GRAPH REGISTRY
index.json declares stages with waves and deps but not as a node/edge graph.
Adding graph.json with explicit edge declarations enables the orchestrator to
implement a generic ready-set executor rather than hardcoded wave plans. This
also makes it trivial to add new nodes or edges without rewriting the
orchestrator logic.

NOTE: "Weighted edges" (partial node activation based on signal strength) is a
v2 enhancement — do NOT include in the initial design. Binary activation/
deactivation is sufficient for v1 and keeps the orchestrator logic simple.

---

## PIECE 6 — Target architecture (constraint the prompt-graph output design)

The prompt's output should specify a skill with this exact structure:

FILES:
  SKILL.md         — orchestrator (graph executor loop, replaces wave-based
                     orchestrator). Reads graph.json topology, executes
                     ready-set loop, runs inline nodes, spawns heavy nodes.
  graph.json       — node registry + typed edge declarations + scale gates.
                     Analogue of index.json but expressed as nodes+edges.
  modules/N*.md    — node protocols. Reuse epiphany-genius module files
                     (renamed N1–N9) with minimal changes: add SIGNAL OUTPUT
                     section specifying what digest to emit.
  kb/              — REUSE epiphany-genius kb/ directory UNCHANGED (18 files).
  scripts/         — Reuse session-init.sh, xml-assemble.sh unchanged.
                     Replace validate-stage.sh with validate-node.sh
                     (same logic, updated naming).

ORCHESTRATOR LOOP (SKILL.md pseudocode):
  0. Parse flags → detect scale → load graph.json → filter nodes/edges by scale
  1. Init session (reuse session-init.sh)
  2. READY_SET = {nodes with no incoming required edges} = {N1} always
  3. SIGNAL_STATE = {} (key: node_id, value: signal_digest string)
  4. While READY_SET not empty:
     a. INLINE_NODES = {n in READY_SET where n.exec_type == inline}
     b. SPAWN_NODES  = {n in READY_SET where n.exec_type == spawn}
     c. Execute each inline node sequentially (role-switched block).
        After each: extract signal digest → SIGNAL_STATE[node_id] = digest
        Write full output to disk as usual.
     d. If SPAWN_NODES not empty: fire ALL in one message (parallel Agent calls).
        Wait for all. Extract signal digests from returns.
     e. Evaluate ROUTER nodes (N8): read SIGNAL_STATE, determine which
        conditional edges are hot, add target nodes to activation queue.
     f. Update READY_SET: add any node whose ALL required incoming edges have
        signal digests in SIGNAL_STATE. Remove just-executed nodes.
  5. Terminal node N9 (SYNTHESIS) completes → output written.
  6. Run validation, observability write-back, test battery (reuse scripts).

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

NODE EXECUTION TYPES AND RATIONALE:
  INLINE: N1 (1 KB file, simple), N3 (2 small KB files), N3.1 (1 KB file,
          700 line budget), N5 (2 KB files, 1100 line budget, only 2 input
          deps), N6.1 (1 KB file, small), N8 (no KB, pure signal routing)
  SPAWN:  N2 (3 large KB files incl. domain-catalog),
          N4 (simulation-level reasoning + 2 KB files),
          N6 (2 large KB files + 1500 line budget + 3 input deps),
          N7 (5 KB files + all optional prior deps),
          N9 (final synthesis, always fresh context for quality)

SPAWN COUNTS BY SCALE (target vs epiphany-genius):
  MINIMAL:     N7 + N9            = 2 spawns   (vs 4 in epiphany-genius)
  STANDARD:    N2 + N6 + N7 + N9  = 4 spawns   (vs 7 in epiphany-genius)
               N2 and N6 fire in parallel (wave 2 and 4 respectively)
  DEEP:        N2 + N4 + N6 + N7 + N9 = 5 spawns  (vs 8+ in epiphany-genius)
               N4 and N6 fire in parallel (both ready after N1+N3 complete)
  CONJECTURE:  N7 + N9            = 2 spawns   (vs 4 in epiphany-genius)

PARALLEL FIRE OPPORTUNITIES:
  STANDARD Wave 2:  N2 + N3 fire in parallel (both depend only on N1+input)
  DEEP Wave 3:      N4 + N6 fire in parallel; N5 runs inline simultaneously
                    (N4 deps: N1+input; N6 deps: N1+N3+input — all available)
  All scales:       N5 and N6 are in the same wave (4) in epiphany-genius.
                    In GoT they still run in parallel (same deps satisfied
                    at the same time), but N5 is now inline instead of spawned.

OUTPUT FORMAT: identical to epiphany-genius.
  {session_dir}/report.md (or report.xml with --xml)
  {session_dir}/stages/N*-*.md for each node output
  {session_dir}/stages/session.md with spawns_total, wall_seconds, etc.
  Summary line: same format as epiphany-genius STEP 8.
  All flags respected identically: --quiet, --xml, --verbose, --no-save, --resume.
