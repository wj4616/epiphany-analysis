# epiphany-graph-genius — Design Spec

**Date:** 2026-04-24
**Status:** Approved for implementation planning
**Target skill path:** `~/.claude/skills/epiphany-graph-genius/`
**Reference skill:** `~/.claude/skills/epiphany-genius/` (v1.1.0, unmodified)

---

## 1. Purpose and scope

Build a new Claude Code skill, `epiphany-graph-genius`, that applies the same 19 Tier-1 genius-mind cognitive traits as `epiphany-genius` v1.1.0 but reimplements the execution architecture as a Graph-of-Thought topology declared in `graph.json`. The new skill:

- Produces the same output artifacts as epiphany-genius (`report.md`, `stages/N*-*.md`, `session.md`, summary line).
- Accepts all the same flags (`--minimal`, `--standard`, `--deep`, `--conjecture`, `--xml`, `--quiet`, `--verbose`, `--no-save`, `--resume`).
- Reduces spawn count on DEEP from 8+ to 5, on STANDARD from 7 to 4, on MINIMAL/CONJECTURE from 4 to 2.
- Targets wall-clock: MINIMAL ≤10 min, STANDARD ≤25 min, DEEP ≤45 min.
- Preserves per-run quality via the V1–V7 verification battery (unchanged) and orchestrator-inline post-run assertions.

Weighted edges, hats-to-tier routing, and automated differential-quality tooling are deferred to v2.

---

## 2. Locked design decisions

| # | Decision | Value | Rationale |
|---|---|---|---|
| 1 | T13 spawn budget | **Option A** — DEEP ≤5, STD ≤4, MIN/CONJECTURE ≤2 | Preserves N4 simulation quality in fresh spawn; ≤45-min wall-clock target is the user-facing metric |
| 2 | KB reuse mechanism | **Option B** — full copy of 18 KB files into `kb/` | Skill is fully self-contained; no cross-skill filesystem dependency |
| 3 | Required-sections registry | **Option A** — extend `graph.json` with `required_output_sections` per node | Single registry file; graph.json becomes the node contract |
| 4 | IV1 / A2 / --differential | **Cut** | V1–V7 per run + PRA1/PRA2 orchestrator-inline sufficient; differential test is YAGNI for a 1–3-run check |
| 5 | F2 test scripts | **Option A** — 4 scripts (session-init.sh byte-identical; xml-assemble.sh, validate-node.sh, test-runner.sh are rename-ports per B12) | Preserves T1–T5 per-run checks (notably T3 scope carve-out integrity, backs V6); drops validate-pipeline.sh in favor of orchestrator-inline PRC1 |

All five decisions are recorded in the session.md of every run via the `spawn_budget_resolution` field and the overall skill-level schema.

---

## 2.5 SKILL.md frontmatter (B15)

The orchestrator file `SKILL.md` opens with this frontmatter verbatim
(structurally parallel to epiphany-genius; paths/name/trigger adjusted):

```yaml
---
name: epiphany-graph-genius
version: 1.0.0
description: >
  Graph-of-Thought reimplementation of epiphany-genius. Applies the same 19
  Tier-1 cognitive traits via an 11-node graph topology declared in
  graph.json. Targets 5/4/2 spawn budget on DEEP/STANDARD/MINIMAL (and
  CONJECTURE alone). Wall-clock: MINIMAL ≤10 min, STANDARD ≤25 min,
  DEEP ≤45 min. V1–V7 verification battery preserved.
trigger:
  - "/epiphany-graph-genius"
  - user says "epiphany-graph-genius"
skill_path: ~/.claude/skills/epiphany-graph-genius/
kb_base: ~/.claude/skills/epiphany-graph-genius/kb/
graph_file: ~/.claude/skills/epiphany-graph-genius/graph.json
session_output_base: ~/docs/epiphany/graph-genius/
---
```

`session_output_base` default: `~/docs/epiphany/graph-genius/` (new-skill default per B16).

---

## 3. File tree

```
~/.claude/skills/epiphany-graph-genius/
├── SKILL.md                        # orchestrator / graph executor
├── graph.json                      # 11 nodes, 20 edges (B11), extended node schema per §4.3bis
├── modules/
│   ├── N1.md                       # State Loading (DECOMPOSITION, inline)
│   ├── N2.md                       # Constraint Escape (LATERAL, spawn)
│   ├── N3.md                       # Peripheral Exploration (SPREADING, inline)
│   ├── N3.1.md                     # De-fixation (DEFIXATION, inline, conditional)
│   ├── N4.md                       # Dynamic Simulation (SIMULATION, spawn)
│   ├── N5.md                       # Precision Forcing (PRECISION, inline)
│   ├── N6.md                       # Falsification (ADVERSARIAL, spawn)
│   ├── N6.1.md                     # Conjecture (CONJECTURE, inline, flag-gated)
│   ├── N7.md                       # Integration & Verification (AGGREGATION, spawn)
│   ├── N8.md                       # Router (ROUTER, inline, NEW — no epiphany-genius analogue)
│   └── N9.md                       # Output Synthesis (SYNTHESIS, spawn)
├── kb/                             # 18 files copied verbatim from epiphany-genius
│   ├── blend-template.md           (67)
│   ├── boden-types.md              (81)
│   ├── debono-techniques.md        (92)
│   ├── domain-catalog.md           (141)
│   ├── elegance-rubric.md          (74)
│   ├── falsification-checklists.md (97)
│   ├── forward-chain-template.md   (103)
│   ├── input-preloading-templates.md (112)
│   ├── observer-frames.md          (82)
│   ├── ohlsson-defixation.md       (79)
│   ├── pattern-taxonomy.md         (121)
│   ├── representation-frames.md    (98)
│   ├── scope-template.md           (79)
│   ├── simulation-checklist.md     (110)
│   ├── spreading-activation.md     (56)
│   ├── tot-templates.md            (139)
│   ├── verification-gates.md       (163)
│   └── vocabulary-rubric.md        (95)
└── scripts/
    ├── session-init.sh             # 69 lines, byte-identical reuse from epiphany-genius
    ├── xml-assemble.sh             # 183 lines, byte-identical reuse from epiphany-genius
    ├── validate-node.sh            # renamed from validate-stage.sh; reads graph.json; N* file naming
    └── test-runner.sh              # 4th script (F2); ported from epiphany-genius; reads graph.json; T1–T5 per-run checks
```

---

## 4. Graph topology

### 4.1 Nodes (11 total)

| Node | Type | Exec | Scale gates | KB files (line count) | Hat (v2-deferred) |
|---|---|---|---|---|---|
| N1 | DECOMPOSITION | inline | MIN/STD/DEEP | input-preloading-templates (112) | Einstein/Feynman/VN/Darwin/Tesla/Turing |
| N2 | LATERAL | spawn | STD/DEEP | domain-catalog, blend-template, debono-techniques (300) | de Bono |
| N3 | SPREADING | inline | STD/DEEP | spreading-activation, elegance-rubric (130) | spreading activation + elegance rubric |
| N3.1 | DEFIXATION | inline (conditional) | STD/DEEP | ohlsson-defixation (79) | Ohlsson |
| N4 | SIMULATION | spawn | DEEP | observer-frames, simulation-checklist (192) | Tesla + observer frames |
| N5 | PRECISION | inline | MIN/STD/DEEP | vocabulary-rubric, forward-chain-template (198) | Feynman |
| N6 | ADVERSARIAL | spawn | STD/DEEP | tot-templates, falsification-checklists (236) | Popper + Millikan |
| N6.1 | CONJECTURE | inline | CONJECTURE_FLAG sentinel | pattern-taxonomy (121) | Ramanujan |
| N7 | AGGREGATION | spawn | MIN/STD/DEEP | verification-gates, boden-types, scope-template, representation-frames, elegance-rubric (495) | Feynman + Boden |
| N8 | ROUTER | inline | STD/DEEP | — (pure signal routing) | — (inherits STEP 6 router logic) |
| N9 | SYNTHESIS | spawn | MIN/STD/DEEP (skipped under --xml per B13) | reads on-disk N1/N5/N7 stage files + N7-v6-scope.txt + raw input (B24/OSP disk-read contract) | (OSP synthesizer) |

**Scale gate activation rule:** a node is active iff `(scale ∈ node.scale_gates)` OR `("CONJECTURE_FLAG" ∈ node.scale_gates AND --conjecture is set)`. Only N6.1 uses the CONJECTURE_FLAG sentinel.

**Default scale:** no flags → STANDARD. `--conjecture` alone → MINIMAL + CONJECTURE_FLAG (satisfies PRA1 CONJECTURE ≤2). `--conjecture` + explicit scale → inherits scale cap + N6.1 inline step.

### 4.2 Edges (20 total; was 19 — edge 20 added per B11 fix)

| # | Source → Target | Type | Scale gates | Signal field | Gate condition |
|---|---|---|---|---|---|
| 1 | input → N1 | required | all | — | null |
| 2 | N1 → N2 | required | STD, DEEP | primitives_digest | null |
| 3 | N1 → N3 | required | STD, DEEP | primitives_digest | null |
| 4 | N1 → N4 | required | DEEP | system_model_seed | null |
| 5 | N1 → N5 | required | all | irreducibles_digest | null |
| 6 | N2 → N7 | optional | STD, DEEP | domain_mappings_digest | null |
| 7 | N3 → N6 | required | STD, DEEP | activation_digest | null |
| 8 | N3 → N7 | optional | STD, DEEP | activation_digest | null |
| 9 | N3 → N3.1 | back-edge | STD, DEEP | gate:S3_thin_or_empty | `SIGNAL_STATE[(N3, activation_digest)].signal_flags includes "S3_thin_or_empty"` |
| 10 | N3.1 → N6 | optional | STD, DEEP | breakthrough_digest | null |
| 11 | N3.1 → N7 | optional | STD, DEEP | breakthrough_digest | null |
| 12 | N4 → N7 | optional | DEEP | simulation_digest | null |
| 13 | N5 → N7 | required | all | precision_digest | null |
| 14 | N6 → N8 | required | STD, DEEP | falsification_result | null |
| 15 | N8 → N3.1 | back-edge | STD, DEEP | gate:S6_no_alternatives AND NOT N3.1_ran | `SIGNAL_STATE[(N6, falsification_result)].signal_flags includes "S6_no_alternatives" AND "N3.1" ∉ executed_nodes` |
| 16 | N8 → N7 | gate-open | STD, DEEP | falsification_digest (passthrough alias of falsification_result) | fires when back-edge (#15) did not |
| 17 | N6.1 → N7 | optional | CONJECTURE_FLAG | conjecture_digest | null |
| 18 | N7 → N9 | required | all | integration_digest | null |
| 19 | N9 → output | terminal | all | — | null |
| 20 | N1 → N6.1 | required | CONJECTURE_FLAG | primitives_digest | null (B11 fix — mirrors epiphany-genius S6.1 input_dependencies on S1) |

### 4.3 Spawn counts and iteration plans (D2 fix — updated post-C2 inline-to-fixpoint)

Iteration count = orchestrator outer-loop trips. Within each iteration,
all currently-eligible inline nodes run sequentially to fixpoint (3a),
then all spawn nodes fire in parallel (3d). Spawn count is the sum of
spawn nodes fired across all iterations.

Default (non --xml):

| Scale | Spawn count | Iteration plan |
|---|---|---|
| MINIMAL | 2 (N7, N9) | Iter 1: {N1 inline → N5 inline} fixpoint → SPAWN={N7}. Iter 2: SPAWN={N9}. |
| STANDARD | 4 (N2, N6, N7, N9) | Iter 1: {N1 → N3 → N5} inline fixpoint → SPAWN={N2, N6} parallel (N6 joins because N3 inline finished mid-fixpoint). Iter 2: {N8 inline} fixpoint → SPAWN={N7}. Iter 3: SPAWN={N9}. |
| DEEP | 5 (N2, N4, N6, N7, N9) | Iter 1: {N1 → N3 → N5} inline fixpoint → SPAWN={N2, N4, N6} parallel (N6 joins via fixpoint refresh). Iter 2: {N8 inline} fixpoint → SPAWN={N7}. Iter 3: SPAWN={N9}. |
| CONJECTURE alone | 2 (N7, N9) + N6.1 inline | Defaults to MINIMAL + CONJECTURE_FLAG. Iter 1: {N1 → N5 → N6.1} inline fixpoint → SPAWN={N7}. Iter 2: SPAWN={N9}. |

Back-edge effects on iteration plan:
- **N3 → N3.1 (early back-edge)** on STANDARD/DEEP: in Iter 1's fixpoint, N3 raises S3_thin_or_empty, N3.1 enqueues, N3.1 runs inline before fixpoint exits. N6 (spawn) sees both activation_digest and breakthrough_digest at fire time. No extra iteration.
- **N8 → N3.1 (late back-edge)** on STANDARD/DEEP: in Iter 2's fixpoint, N8 enqueues N3.1, N3.1 runs inline. Iter 3 unchanged. No extra iteration.
- Both cases preserve the iteration count above.

`--xml` mode (B13 fix — N9 disabled):

| Scale | Spawn count | Note |
|---|---|---|
| MINIMAL + --xml | 1 (N7 only) | N9 skipped; xml-assemble.sh runs instead |
| STANDARD + --xml | 3 (N2, N6, N7) | N9 skipped |
| DEEP + --xml | 4 (N2, N4, N6, N7) | N9 skipped |
| CONJECTURE + --xml | 1 (N7) | N9 skipped |

Back-edges (N3→N3.1, N8→N3.1) are conditional; when fired, N3.1 adds one inline step (no additional spawn). Edge 20 (N1→N6.1, B11 fix) activates N6.1 inline when `--conjecture` flag is set.

PRA1 cap enforcement uses the default (non --xml) column; --xml runs
always satisfy the cap trivially since they have fewer spawns.

### 4.3bis graph.json node schema (B14 fix — extended beyond original output_format)

Every node object in graph.json carries the following fields (expanded from
the original spec's minimal sketch, to cover all fields that scripts need):

```json
{
  "id": "N1",
  "module_file": "modules/N1.md",
  "module_version": "1.0.0",
  "type": "DECOMPOSITION",
  "exec_type": "inline",
  "hat": "Einstein/Feynman/VN/Darwin/Tesla/Turing",
  "kb_files": ["input-preloading-templates.md"],
  "context_budget_lines": 150,
  "scale_gates": ["MINIMAL", "STANDARD", "DEEP"],
  "activation": ["always"],
  "enabled_when": null,
  "input_dependencies": ["stages/00-processed-input.md"],
  "output_file": "stages/N1-state-loading.md",
  "xml_element": "state_loading",
  "required_output_sections": [
    "Input class",
    "Primitives",
    "Simplicity-stripped irreducibles",
    "Tesla measurement plan",
    "Verification"
  ],
  "raises_signals": []
}
```

Field purposes:
- `id`, `type`, `exec_type`, `hat`, `kb_files`, `context_budget_lines`,
  `scale_gates`, `required_output_sections`: per original decisions.
- `module_file`: explicit path to the Nx.md module (used by orchestrator
  spawn prompts and inline read).
- `module_version`: independent versioning per node (matches epiphany-genius
  pattern). v1 convention (D6): all 11 nodes ship at `"1.0.0"`. Bump per
  node when its module's PROTOCOL or SIGNAL OUTPUT schema changes.
- `activation`: list like `["always"]`, `["flag:--conjecture"]`,
  `["scale:DEEP"]` — read by test-runner.sh to identify active conditionals.
- `enabled_when`: null OR expression like `"NOT --xml"` (per B13: N9 has
  `"NOT --xml"` here). Orchestrator STEP 3 ready-set recompute checks this.
- `input_dependencies`: list of on-disk file paths the node reads (beyond
  what signal digests cover). Used for --resume validation and subagent
  prompt construction.
- `output_file`: read by validate-node.sh, test-runner.sh, xml-assemble.sh
  to locate the stage file.
- `xml_element`: read by xml-assemble.sh to map to XML schema element.
  `null` for nodes that don't appear in XML output (e.g., N8, N9's OSP).
- `raises_signals`: list like `["S3_thin_or_empty"]` — documentation only;
  runtime uses SIGNAL OUTPUT block's `signal_flags` field.

Every edge object carries: source, target, type, signal_field, scale_gates,
gate_condition (as §4.2 table).

### 4.3ter Edge activation filter rule (D4 fix — explicit rule)

An edge `e` is ACTIVE in the current run iff ALL of the following hold:
1. `e.source` node is active per scale-gate rule (§4.1).
2. `e.target` node is active per scale-gate rule (§4.1).
3. `e.scale_gates` includes the current scale, OR `"CONJECTURE_FLAG" ∈
    e.scale_gates AND --conjecture is set`.

Inactive edges are filtered out before STEP 0.3 PRC1 connectivity check
and before STEP 3 ready-set recompute.

When `e.source == "input"`: condition 1 reduces to "always true" (input is
the abstract source of the pipeline, not a node).
When `e.target == "output"`: condition 2 reduces to "always true" (output
is the abstract sink).

### 4.4 DAG and connectivity invariants

- **PRC1 cycle check:** When back-edges {9, 15} are excluded, the graph is a DAG. Verified by orchestrator STEP 0 after graph.json load.
- **PRC1 connectivity check:** Scale-filtered subgraph (including back-edges for connectivity purposes) has N1 as single source and N9 as single sink. Verified at STEP 0.
- **Edge resolution:** Every edge source and target resolves to a declared node id. Every `signal_field` references one of 11 declared digests or a declared gate literal. Verified at STEP 0.

### 4.5 Edge type semantics

| Type | Activation behavior |
|---|---|
| `required` | Target blocks until source digest is in SIGNAL_STATE. |
| `gate-open` | Treated as required-when-active. Target blocks until the gate resolves (either source digest arrives OR the back-edge alternative fires and post-repair source digest arrives). Not optional. |
| `optional` | Does not block target activation. Included in target's context if present at fire time; skipped otherwise. |
| `back-edge` | Target (N3.1) activates only if gate condition evaluates true AND target ∉ executed_nodes. XOR semantics between the two back-edges ensures N3.1 fires at most once per run (O7). |
| `terminal` | N9 → output; pipeline completion. |

---

## 5. Signal digests

### 5.1 Schema (all 11 digests share this shape)

Every digest is 5–15 lines and carries four required fields (signal digest invariant):

```
key_findings:     [bulleted list of 3–7 items]
named_entities:   [set of proper-noun references / convergent nodes]
confidence_flags: [H | M | L per finding]
signal_flags:     [any gate-raising flags, e.g., S3_thin_or_empty, S6_no_alternatives]
```

Full stage output is always persisted to `{session_dir}/stages/N*-*.md`. The digest is an index into the full file; it does not replace it.

### 5.2 Digest catalog

| Digest | Emitter | Consumers | Notes |
|---|---|---|---|
| primitives_digest | N1 | N2, N3 | primitive enumeration by category |
| irreducibles_digest | N1 | N5 (N7 transitively via N5) | simplicity-stripped core |
| system_model_seed | N1 | N4 (DEEP only) | physical/logical seed for simulation |
| activation_digest | N3 | N6 (required), N7 (optional) | **must contain W2 literal** (below) |
| breakthrough_digest | N3.1 | N6 (optional), N7 (optional) | de-fixation result when back-edge fires |
| simulation_digest | N4 | N7 (optional, DEEP only) | observer-frame analyses + limit cases |
| precision_digest | N5 | N7 (required) | 5-level vocab strip + forward chain |
| falsification_result | N6 | N8 (required) | raises `S6_no_alternatives` when applicable |
| falsification_digest | N8 | N7 (gate-open edge 16) | passthrough alias of falsification_result |
| domain_mappings_digest | N2 | N7 (optional) | 5 domain mappings from lateral thinking |
| conjecture_digest | N6.1 | N7 (optional, --conjecture only) | ≥3 conjectures + verification instances |
| integration_digest | N7 | N9 (required) | convergence + primary conclusion + V1–V7 vector |

### 5.3 W2 literal (activation-digest exact-string requirement)

N3's `activation_digest` must contain the exact string:
```
activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]
```
Enforced by orchestrator STEP 3c digest validation and re-checked by test-runner.sh at STEP 5.4 (C10 fix — was erroneously written as "STEP 9" from epiphany-genius numbering).

### 5.4 SIGNAL_STATE data structure (single-source-of-truth invariant)

In-memory map during a single run. Key is `(node_id, signal_field)`; value is digest text.

```
SIGNAL_STATE[("N1", "primitives_digest")]     = "<digest>"
SIGNAL_STATE[("N1", "irreducibles_digest")]   = "<digest>"
SIGNAL_STATE[("N1", "system_model_seed")]     = "<digest>"
SIGNAL_STATE[("N3", "activation_digest")]     = "<digest>"
...
```

N1 emits three distinct digests → three SIGNAL_STATE entries. N8's passthrough → one entry keyed `(N8, falsification_digest)` whose value equals N6's falsification_result.

**PRC2 invariant:** SIGNAL_STATE is append-only within a single run. Attempting to overwrite a `(node_id, signal_field)` key aborts with an explicit error.

**`executed_nodes: [node_id]` in session.md is the sole source of truth** for node-completion state (single-source-of-truth invariant). N8's `NOT N3.1_ran` gate reads executed_nodes exclusively; SIGNAL_STATE is an in-memory working map (may be empty on `--resume` before digest rehydration).

---

## 6. Orchestrator loop (SKILL.md pseudocode)

```
STEP 0 — FLAG RESOLUTION + GRAPH LOAD + PRC1
  0.1 Parse flags. Determine scale:
      - explicit --minimal/--standard/--deep → use it.
      - --conjecture alone (no scale) → MINIMAL + CONJECTURE_FLAG.
      - no flags → STANDARD.
      - --conjecture + explicit scale → that scale + CONJECTURE_FLAG.
      --resume SYNTAX (B19 clarification): `--resume <session_dir>` takes
      a positional argument after the flag. E.g.,
      `/epiphany-graph-genius --resume ~/docs/epiphany/graph-genius/2026-04-24-abc123/`
      The session_dir is validated for presence of stages/session.md; if
      missing → HALT with explicit error.
      --resume FLAG INHERITANCE (B20): the resumed run MUST use the original
      session's scale flag. If the user passes a scale flag (--minimal /
      --standard / --deep) on --resume, compare to session.md.scale:
      - match → allowed, proceed.
      - mismatch → HALT with explicit error "scale flag conflict on
        --resume; original session used {X}; re-run without scale flag
        or with matching flag."
      --conjecture on --resume: if original session was --conjecture and
      user omits it on resume → HALT (original topology included N6.1).
      If original was not --conjecture and user adds it → HALT (would
      require re-activating N6.1 mid-graph; not supported in v1).
      INVALID COMBINATIONS (HALT with explicit error):
      - Two or more of {--minimal, --standard, --deep} set simultaneously.
      - Unknown flag (not in the accepted set).
      - --resume without a session_dir positional argument.
      - --resume with scale/flag mismatch vs original session (per above).
      --verbose behavior: emit a one-line progress annotation per node as
      it starts (inline or spawn) and completes. Under --verbose, the
      orchestrator also logs the Agent() spawn prompts to
      session.md.verbose_trace[] for post-hoc inspection. Default (no
      --verbose): silent per-node progress; only summary line + terminal
      line printed at end.
      --quiet behavior: suppresses the "Saved to ..." line at STEP 6.3 but
      NEVER suppresses the summary line at 6.2 (parity with epiphany-genius).
  0.2 Load graph.json. Apply scale-gate filter per 4.1 rule.
  0.3 PRC1 validation (inline — no separate script):
      - Excluding back-edges {9, 15}, check DAG (no cycles).
      - All edge source/target ids resolve to declared nodes.
      - All signal_fields reference declared digests or gate literals.
      - Scale-filtered subgraph (back-edges included) is connected with N1 source, N9 sink.
      On failure: HALT with explicit error.

STEP 1 — SESSION INIT + INPUT PROCESSING (C5 fix)
  1.1 Invoke scripts/session-init.sh (byte-identical reuse; creates
      {session_dir}/stages/ and writes initial stages/session.md).
  1.2 Input capture and normalization:
      a. Read raw user input (from command args or stdin).
      b. Write verbatim copy to {session_dir}/input.md. This file is
         the audit archive of the original user input. Never overwritten;
         never skipped (not even under --no-save).
      c. Apply light normalization (strip leading/trailing whitespace;
         Unicode NFC normalize; no semantic changes) and write to
         {session_dir}/stages/00-processed-input.md. This is the file
         nodes read during execution. v1 behavior: normalization is
         near-identity; v2 may add classification/preprocessing per
         the epiphany-genius STEP 3 pattern.
      d. Distinction between the two files: input.md is the user's
         literal submission (auditable); 00-processed-input.md is the
         pipeline's working copy (can diverge from input.md in v2 if
         preprocessing is added).
  1.3 Init session.md with:
      - session_id, scale, modifiers
      - wall_seconds_start = <epoch>
      - spawns_total = 0
      - executed_nodes = []
      - abort_reason = null                 # C6 fix: reserve field
      - spawn_budget_resolution:
          option: A
          rationale: "N4 simulation quality preserved; ≤45-min wall-clock is user metric"
          chosen_at_phase: design
      - wave_demotions: []
      - repair_cap_hit: false
      - warnings: []
      - verbose_trace: []                   # populated only under --verbose

STEP 2 — READY_SET INIT
  2.1 READY_SET = {N1} (only node with no required incoming edges).
  2.2 SIGNAL_STATE = {}.
  2.3 If --resume:
      a. Scan {session_dir}/stages/ for N*-*.md files.
      b. For each candidate file: invoke scripts/validate-node.sh
         {session_dir} {node_id}. Only VALID files proceed to rehydration;
         invalid or partial files are treated as not-yet-executed (their node
         must re-execute). This prevents PRC2 violation from corrupted/partial
         on-disk state being trusted as authoritative.
      c. For each validated file: parse its SIGNAL OUTPUT block(s) →
         populate SIGNAL_STATE entries keyed by (node_id, signal_field) per
         the node's output_ports. Nodes that emit multiple digests (N1)
         populate multiple SIGNAL_STATE entries from one stage file.
      d. Append validated node_ids to executed_nodes.
      e. PRC2 re-entry rule: nodes in executed_nodes are SKIPPED during
         STEP 3 re-entry (they never re-execute). SIGNAL_STATE keys
         populated in step 2.3c are pre-loads; STEP 3 never overwrites
         them. Append-only invariant holds across the resumed run.
      f. Run STEP 3e once to seed READY_SET with nodes whose required
         incoming edges are already satisfied via rehydrated SIGNAL_STATE.
         (Corrected cross-reference per B18 — was "STEP 3f" before
         ready-set recompute was renumbered.)

STEP 3 — READY-SET EXECUTION LOOP (C2 fix: inline-to-fixpoint → spawn)

  TERMINATION / FAILURE HALT (B17 fix):
    At the top of each iteration, if READY_SET is empty AND N9 has NOT
    executed AND (--xml mode has not reached STEP 4 xml-assemble.sh call):
      HALT. This indicates a cascade failure (e.g., N1 produced no
      digests, blocking all downstream). Write:
        - session.md.abort_reason = "READY_SET empty before terminal;
          missing digests: <list of (node_id, signal_field) expected
          but absent from SIGNAL_STATE>"
        - session.md.wall_seconds = now − wall_seconds_start
      Print failure line to stderr:
        `FAILED: epiphany-graph-genius aborted — see {session_dir}stages/session.md`
      Do NOT emit the normal summary line. Exit non-zero.

  SPAWN FAILURE HANDLING (C7 fix):
    If any Agent() spawn returns with an error (tool error, timeout,
    unparseable output, exception), the orchestrator:
      1. Logs the failure to session.md.warnings with node_id + error.
      2. Does NOT write SIGNAL_STATE for the failed node.
      3. Does NOT add failed node to executed_nodes.
      4. Blocks dependents of the failed node (they fail the ready-set
         required-edge check).
      5. Continues the ready-set loop with remaining nodes.
      6. If the failure cascade empties READY_SET before N9 completes,
         the B17 HALT rule triggers and aborts the run.
    This gives graceful degradation: a failed N2 (optional input to N7)
    logs a warning but doesn't abort; a failed N5 (required input to N7)
    cascades to abort. --resume can retry after fixes.

  While READY_SET ≠ ∅:
    3a. INLINE-TO-FIXPOINT PHASE (C2 fix):
        REPEAT:
          INLINE_NODES = { n ∈ READY_SET | n.exec_type == "inline" }
          If INLINE_NODES is empty: break (fixpoint reached).
          For each n ∈ INLINE_NODES (sequentially): execute per 3c below.
            After each inline completion, run 3e ready-set recompute to
            surface newly-eligible nodes (which may be inline — e.g.,
            N3.1 back-edge, N8 after N6, N6.1 after N1 on --conjecture).
        END REPEAT.
        Rationale: exhausting all inline work first lets SPAWN_NODES
        (computed in 3b below) include every spawn that became eligible
        via inline emissions. Eliminates the "delayed spawn" bug where
        N6 would have waited a full wave for N3 to complete in STEP 3c.

    3b. SPAWN_NODES = { n ∈ READY_SET | n.exec_type == "spawn" }
        (Computed AFTER the inline-to-fixpoint phase.)

    3c. For each n ∈ INLINE_NODES (sequentially):
        - Emit role-switched preamble (B22 fix — includes raw-input path):
            "You are now executing {n.id} as {n.hat}. Read modules/{n.id}.md
             and its kb_files. The raw input is at
             {session_dir}/stages/00-processed-input.md. Execute the
             module's PROTOCOL. Do not reason about orchestrator state
             during this block."
        - Read modules/{n.id}.md + n.kb_files.
        - Execute protocol. Write full output to {session_dir}/stages/{n.id}-<descriptor>.md.
        - Extract SIGNAL OUTPUT block(s). Validate against signal_digest_schema.
          NODES EMITTING MULTIPLE DIGESTS: N1 emits three digests
          (primitives_digest, irreducibles_digest, system_model_seed) and
          writes three SIGNAL_STATE entries from one stage file. Its module
          SIGNAL OUTPUT section has three schema sub-blocks. All other nodes
          emit exactly one digest (see §5.2 catalog).
          On validation failure (EC7): log warning, mark digest as missing
          in SIGNAL_STATE (do not write), block dependents, continue.
        - On success: for each output_port p, write
          SIGNAL_STATE[(n.id, p.signal_field)] = digest. (PRC2: never overwrite.)
        - Append n.id to executed_nodes.
        - POST-EXECUTION BACK-EDGE CHECKS (evaluated immediately after the
          inline node completes, before the next iteration of STEP 3):
          - IF n == N3 AND SIGNAL_STATE[(N3, activation_digest)].signal_flags
            includes "S3_thin_or_empty" AND "N3.1" ∉ executed_nodes:
              enqueue N3.1 to READY_SET. (Early back-edge: edge 10 N3.1→N6
              is LIVE because N6 has not yet executed; N6 will receive
              both activation_digest and breakthrough_digest as optional
              inputs when it fires.)
          - IF n == N8: N8's own protocol handles routing; see §7 N8.md
            protocol sketch. N8 writes:
              * SIGNAL_STATE[(N8, falsification_digest)] UNCONDITIONALLY
                (passthrough of N6's falsification_result; gate-open edge 16
                always resolves once N8 has executed).
              * IF signal_flags includes "S6_no_alternatives" AND
                "N3.1" ∉ executed_nodes: ALSO enqueue N3.1 to READY_SET
                (late back-edge). Edge 10 (N3.1→N6) is DEAD-LETTER here
                because N6 has already executed; breakthrough_digest
                reaches N7 only via edge 11 (N3.1→N7 optional). N6 does
                NOT re-execute; V2 falsifier check of the breakthrough is
                deferred to N7's V1–V7 battery.
              * Write {session_dir}/stages/N8-router-record.md with
                resolved_gate, passthrough digest, signal_flags checked,
                1-line decision rationale.
          - IF n == N8 AND "N3.1" ∈ executed_nodes AND signal_flags
            includes "S6_no_alternatives": this is a repair-cap-hit
            situation (EC4). Annotate session.md.repair_cap_hit = true.
            Do not enqueue N3.1 again. Gate-open to N7 still fires.

    3d. IF SPAWN_NODES ≠ ∅:
        - Fire ALL spawns in ONE message (parallel Agent calls per O4).
        - Spawn prompt structure (matches epiphany-genius pattern — see
          epiphany-genius SKILL.md STEP 5 for the canonical Agent()
          invocation template). The orchestrator INLINES digest payloads
          directly into the prompt text; subagents never "read" the
          orchestrator's SIGNAL_STATE map (B23 clarification — it is a
          string-substitution into the prompt, not a remote state read).
          Each spawn prompt contains:
            * Session directory: {session_dir}
            * Module file path: modules/{n.id}.md
            * Scale: {scale}
            * Active modifiers (flags): {modifiers}
            * Executed nodes so far: {executed_nodes list} (B21 — needed
              by N9 to emit empty-section markers for inactive nodes)
            * For each required incoming edge, the orchestrator emits:
                "Predecessor {source} digest ({signal_field}):
                 <literal digest payload inlined here>"
            * For each optional incoming edge whose source is in
              executed_nodes, emit the same inlined form (subagent
              treats as optional context).
            * For N7 (AGGREGATION) additionally (B24): disk-read path
                {session_dir}/stages/N1-state-loading.md
              is included as an explicit Read reference in the prompt
              (N1 is transitively required; precision_digest carries
              irreducibles but not full primitives enumeration).
            * For N9 only (OSP disk-read contract): additional Read paths —
                {session_dir}/stages/N1-state-loading.md
                {session_dir}/stages/N5-precision-forcing.md
                {session_dir}/stages/N7-integration.md
                {session_dir}/stages/N7-v6-scope.txt
                {session_dir}/input.md
                (plus optionally any other N*-*.md present at fire time)
            * Instructions: "Execute the PROTOCOL in your module file.
              Write full output to {session_dir}/stages/{output_file}.
              Return a SIGNAL OUTPUT block matching your module's
              declared schema."
        - Wait for all returns.
        - For each return: extract SIGNAL OUTPUT → validate → write to SIGNAL_STATE
          → append to executed_nodes.
        - session.md.spawns_total += len(SPAWN_NODES).

    3e. READY_SET RECOMPUTE (renumbered from 3f):
        For each node n not in executed_nodes and active per scale gate:
          - Check all `required` incoming edges: source digest must be in
            SIGNAL_STATE.
          - Check any `gate-open` incoming edges: source digest must be in
            SIGNAL_STATE (gate-open is required-when-active per §4.5; per
            the B3 fix, N8→N7 gate-open always resolves once N8 has
            executed, so this reduces to waiting for N8).
          - `optional` and `back-edge` incoming edges do not block activation
            EXCEPT per the early-back-edge sync rule below.
          - EARLY BACK-EDGE SYNC RULE (B10 fix):
            IF n == N6 AND SIGNAL_STATE[(N3, activation_digest)].signal_flags
            includes "S3_thin_or_empty" AND "N3.1" ∉ executed_nodes:
              N6 is BLOCKED (do not add to READY_SET) until N3.1 completes.
            Rationale: edge 10 (N3.1→N6 optional) is meant to deliver
            breakthrough_digest to N6 in the early back-edge case. Without
            this sync, N6 fires parallel to N3.1 and misses the breakthrough,
            defeating the cognitive purpose of edge 10.
            This rule applies ONLY in the early back-edge case. In the late
            back-edge case (N8→N3.1 after N6 has executed), edge 10 is
            already dead-letter; no sync needed.
          If all required + gate-open checks pass AND the early-back-edge
          sync rule does not block: add n to READY_SET.
        Remove just-executed nodes from READY_SET.

    3f. O8 BUDGET CHECK (renumbered from 3g; before each wave):
        Sum context_budget_lines of all INLINE_NODES in this wave.
        If > 600: demote KB-heaviest inline node to spawn (tie-break: lex node_id, EC2).
        Record demotion in session.md.wave_demotions[].
        (In practice, max wave sum = 277 lines; this rule should never trip.)

STEP 4 — TERMINAL (B13 fix: N9 is --xml-gated)
  4.1 If --xml flag is set:
      - N9 is SKIPPED per `enabled_when: "NOT --xml"` on N9's graph.json
        entry (mirrors epiphany-genius output_synthesis_pass.enabled_when).
        This means ready-set logic never activates N9 in --xml runs.
      - Instead, invoke scripts/xml-assemble.sh (modified per B12 —
        S→N renames + graph.json lookup). Writes
        {session_dir}/stages/output.xml from stage files.
      - report.xml = copy of output.xml to session root.
      - Spawn count drops by 1 in --xml mode (N9 removed): MINIMAL ≤1,
        STANDARD ≤3, DEEP ≤4, CONJECTURE ≤1.
  4.2 If NOT --xml:
      - N9 runs (was activated by ready-set recompute in STEP 3e).
      - N9 writes {session_dir}/stages/N9-output-distilled.md.
      - report.md = copy of N9-output-distilled.md to session root.

STEP 5 — POST-RUN ASSERTIONS + VALIDATION
  5.1 For each n ∈ executed_nodes: invoke scripts/validate-node.sh
      {session_dir} {n.id}. Collect pass/fail results without halting on
      individual failures (C12 fix): a failed node-level validation logs
      to session.md.warnings[] and contributes to the verdict in STEP 6.2
      but does not abort STEP 5. This mirrors epiphany-genius where
      validate-stage.sh failures are advisory post-run (the run has
      already completed; we're reporting validation outcomes, not
      gating execution).
  5.2 PRA1: assert spawns_total ≤ scale target (MIN≤2, STD≤4, DEEP≤5, CONJECTURE≤2).
      On fail: log to session.md.warnings, continue.
  5.3 PRA2: assert artifact presence (D3 fix — full list):
      Required (always):
        - {session_dir}/input.md
        - {session_dir}/stages/00-processed-input.md
        - {session_dir}/stages/session.md (with all init schema fields)
        - {session_dir}/stages/validation-log.md (written by validate-node.sh)
        - {session_dir}/stages/N*-*.md for each n ∈ executed_nodes
      Required (conditional):
        - {session_dir}/stages/N7-v6-scope.txt (if N7 executed)
        - {session_dir}/stages/N9-output-distilled.md (if N9 executed,
          i.e., NOT --xml)
        - {session_dir}/stages/output.xml (if --xml mode)
        - {session_dir}/stages/test-report.md (after STEP 5.4 runs)
        - {session_dir}/report.md (if NOT --xml AND NOT --no-save)
        - {session_dir}/report.xml (if --xml AND NOT --no-save)
      On fail: log missing-artifact list to session.md.warnings.
  5.4 Invoke scripts/test-runner.sh {session_dir} {scale} {active_conditionals} {output_mode}.
      - active_conditionals: comma-separated list of conditional node_ids
        present in executed_nodes (subset of {N3.1, N6.1}). If none, pass
        literal "none".
      - output_mode: "xml" if --xml else "distilled".
      T1–T5 run as in epiphany-genius. Writes stages/test-report.md.
  5.5 Write session.md.wall_seconds = now − wall_seconds_start.

STEP 6 — SAVE + SUMMARY
  6.1 If NOT --no-save: copy N9-output-distilled.md (or output.xml) to
      {session_dir}/report.md (or report.xml). Unconditional for input.md.
  6.2 Print summary line (always, even under --quiet, per epiphany-genius
      STEP 8 parity):
        Cognitive enhancement: [N] nodes executed | Confidence: [band] |
        [M] contradictions surfaced | Verification: [pass/fail count] |
        Creativity type: [...] | Scope: [limited/broad]
      Value extraction mapping (D1 fix — values come from FULL N7 file
      stages/N7-integration.md, not the integration_digest. The 5-15 line
      digest schema does not contain Millikan/Boden/scope as fields; those
      are in N7's required_output_sections of the full file):
        [N] nodes executed   ← len(executed_nodes)
        [band]               ← parse "Confidence band:" line from
                               stages/N7-integration.md (one of: high |
                               medium | low per Millikan assessment)
        [M] contradictions   ← parse "Contradictions surfaced:" from
                               stages/N7-integration.md.
                               Source by scale:
                                 MINIMAL: 0 (no N4, no N6 to produce any)
                                 CONJECTURE alone: 0 (same as MINIMAL)
                                 STANDARD: from N6 falsification counts
                                   (abductive + deductive pass failures)
                                 DEEP: N4 theory collisions + N6 counts
                               On MINIMAL/CONJECTURE-alone, N7's
                               integration file emits contradictions = 0.
                               (B26 + D1 fix.)
        [pass/fail count]    ← parse "Verification report (V1-V7)" section
                               from stages/N7-integration.md, count
                               "PASS"/"FAIL" markers; emit "<pass>/<total>"
                               (e.g., "6/7")
        [creativity type]    ← parse "Creativity type:" from
                               stages/N7-integration.md (one of:
                               combinational | exploratory | transformational)
        [limited/broad]      ← parse Scope section from
                               stages/N7-integration.md; classify based
                               on scope-template KB (limited | broad)
      Format identical to epiphany-genius STEP 8 with `stages` → `nodes` only.
  6.3 If not --no-save: print `Saved to {session_dir}report.{md|xml}`.
      (Always printed when save occurred, including under --quiet, per
      epiphany-genius STEP 8 precedent.)
  6.4 Print terminal line: `Session: {session_dir}`.
```

---

## 7. Module file structure (D3)

Each `modules/Nx.md` has YAML frontmatter + PROTOCOL body (copied verbatim from epiphany-genius's corresponding stage module, except N8 which is authored fresh) + SIGNAL OUTPUT section.

### 7.1 Frontmatter schema

```yaml
---
node_id: <N1 | N2 | ... | N9>
node_type: <DECOMPOSITION | LATERAL | SPREADING | DEFIXATION | SIMULATION |
           PRECISION | ADVERSARIAL | CONJECTURE | AGGREGATION | ROUTER | SYNTHESIS>
hat: "<cognitive role label>"
exec_type: <inline | spawn>
scale_gates: [<MINIMAL, STANDARD, DEEP>] | ["CONJECTURE_FLAG"]
kb_files:
  - <filename from kb/>
context_budget_lines: <integer>
input_ports:
  - from: <source node id | "input">
    signal_field: <one of 11 digests | "—">
    required: <true | false>
output_ports:
  - to: <target node id | "output">
    signal_field: <one of 11 digests | gate literal | "—">
    type: <required | optional | back-edge | gate-open | terminal>
    gate: <gate name, if applicable>
required_output_sections:
  - "<section heading from epiphany-genius original>"
raises_signals: [<S3_thin_or_empty | S6_no_alternatives | ...>]
---
```

### 7.2 Module-to-source mapping

| Module | Source |
|---|---|
| N1.md | epiphany-genius modules/S1-state-loading.md (copied + frontmatter + SIGNAL OUTPUT) |
| N2.md | modules/S2-constraint-escape.md |
| N3.md | modules/S3-peripheral-exploration.md |
| N3.1.md | modules/S3-1-defixation.md |
| N4.md | modules/S4-dynamic-simulation.md |
| N5.md | modules/S5-precision-forcing.md |
| N6.md | modules/S6-falsification.md |
| N6.1.md | modules/S6-1-conjecture.md |
| N7.md | modules/S7-integration-verification.md |
| N8.md | **NEW — authored fresh**, extracting router logic from epiphany-genius SKILL.md STEP 6 (retry Path A/B + gate resolution) |
| N9.md | modules/output-synthesis-pass.md |

### 7.3 SIGNAL OUTPUT section template

**Single-digest nodes (N2, N3, N3.1, N4, N5, N6, N6.1, N7, N8, N9)** end with:

```markdown
## SIGNAL OUTPUT

Emits: <digest_name> (5–15 lines)

Schema:
\`\`\`
key_findings:     [3–7 bulleted items describing <domain>]
named_entities:   [proper nouns, entity references]
confidence_flags: [H | M | L per finding]
signal_flags:     [applicable gate flags]
\`\`\`

<If W2 literal required (N3 only):>
**Required literal (W2):** output must contain the exact string
`activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]`
```

**Multi-digest node N1** ends with THREE SIGNAL OUTPUT subsections, one per emitted digest (C12):

```markdown
## SIGNAL OUTPUT

### Digest 1 of 3 — primitives_digest (5–15 lines)
Schema:
\`\`\`
key_findings:     [primitives enumerated by category]
named_entities:   [named primitives]
confidence_flags: [H | M | L per finding]
signal_flags:     []
\`\`\`

### Digest 2 of 3 — irreducibles_digest (5–15 lines)
Schema:
\`\`\`
key_findings:     [simplicity-stripped irreducibles]
named_entities:   [named irreducible entities]
confidence_flags: [H | M | L per finding]
signal_flags:     []
\`\`\`

### Digest 3 of 3 — system_model_seed (5–15 lines)
Schema:
\`\`\`
key_findings:     [system model seed — physical/logical structure for simulation]
named_entities:   [named components / interactions]
confidence_flags: [H | M | L per finding]
signal_flags:     []
\`\`\`
```

The orchestrator STEP 3c parses all three subsections when extracting N1's digests, writing one SIGNAL_STATE entry per subsection keyed by `(N1, {digest_name})`.

### 7.3bis SIGNAL OUTPUT parsing grammar (C9 fix)

To eliminate parser ambiguity, the SIGNAL OUTPUT block follows this
formal grammar:

```
signal_output_block  := "## SIGNAL OUTPUT" NEWLINE body
body                 := single_digest_body | multi_digest_body
single_digest_body   := "Emits: " digest_name " (" digest_spec ")" NEWLINE
                        NEWLINE "Schema:" NEWLINE code_block [literal_block]
multi_digest_body    := (digest_subsection){2,}
digest_subsection    := "### Digest " INT " of " INT " — " digest_name
                        " (" digest_spec ")" NEWLINE
                        "Schema:" NEWLINE code_block [literal_block]
code_block           := "```" NEWLINE schema_fields "```" NEWLINE
schema_fields        := "key_findings:" list_expr NEWLINE
                        "named_entities:" list_expr NEWLINE
                        "confidence_flags:" list_expr NEWLINE
                        "signal_flags:" list_expr NEWLINE
list_expr            := "[" any_text_to_line_end "]"
literal_block        := "**Required literal ... `" literal_string "`"
digest_name          := identifier_matching_11_declared_digests
digest_spec          := "5–15 lines" (the digest payload itself)
```

Parser contract:
1. Locate `## SIGNAL OUTPUT` header (exactly this string).
2. For N1: expect 3 `### Digest N of 3 — <name>` subsections. For all
   other nodes: expect 0 subsections and 1 `Emits:` line at the top.
3. Extract `digest_name` from each subsection or top-level `Emits:` line.
4. Validate digest_name is one of the 11 declared signals + `falsification_digest` alias.
5. Extract code-block contents between triple backticks. Parse into
   four named fields (key_findings, named_entities, confidence_flags,
   signal_flags). Values are list-shaped strings.
6. Count non-empty content lines INSIDE the code block (i.e., the lines
   between the triple-backtick fences, excluding the fences themselves
   and excluding the "Schema:" header). The count must be 5–15 inclusive.
   For multi-line list values (e.g., key_findings spanning multiple lines),
   each non-empty line counts. (D5 clarification.)
7. If W2 literal block present: extract literal_string and verify it
   matches the required W2 form for N3's activation_digest.
8. On any parse failure: apply EC7 (mark missing, warn, continue).

The parser is stateless per stage file; no cross-file context needed.

### 7.4 N8.md protocol sketch (NEW module content)

N8 has no KB files, no cognitive analysis, no PROTOCOL body copied from
epiphany-genius. It is a small deterministic router. Its module body:

```markdown
## PROTOCOL

N8 is a pure signal router. It has already been reached because N6's
required edge (falsification_result) is in SIGNAL_STATE. N8 performs
the following deterministic steps:

1. Read SIGNAL_STATE[(N6, falsification_result)] into local variable FR.
2. Read session.md.executed_nodes into local variable EXE.
3. Evaluate back-edge condition (edge 15):
     back_edge_fires = ("S6_no_alternatives" ∈ FR.signal_flags)
                        AND ("N3.1" ∉ EXE)
4. Write SIGNAL_STATE[(N8, falsification_digest)] = FR
   (UNCONDITIONAL passthrough; gate-open edge 16 always resolves).
5. If back_edge_fires:
     a. If "N3.1" ∈ EXE (should be false here by step 3, but double-check):
          Annotate session.md.repair_cap_hit = true; do NOT re-enqueue.
     b. Else: enqueue N3.1 to READY_SET.
6. Write {session_dir}/stages/N8-router-record.md:
   ---
   resolved_gate: <back-edge-fired | gate-open-only>
   falsification_digest: <verbatim passthrough of FR>
   signal_flags_checked: <list>
   decision_rationale: <1-line explanation>
   ---
```

N8 emits one SIGNAL OUTPUT block per the single-digest template above,
with `Emits: falsification_digest (passthrough of N6's falsification_result)`.

**N8 graph.json field specifics (C8 clarification):**
- `xml_element: null` — N8's router-record does not appear in assembled XML.
- `hat: ""` or `hat: null` — N8 has no cognitive role; it is a pure router.
- `kb_files: []` — N8 reads no KB.
- `input_dependencies: []` — N8 consumes only the signal digest from N6
  via the edge; it reads no on-disk files beyond session.md.executed_nodes.
- `raises_signals: []` — N8 is a passthrough; it doesn't raise new flags.

---

## 8. Scripts

### 8.1 session-init.sh
Byte-identical reuse from epiphany-genius. No modifications.

### 8.2 xml-assemble.sh (B12 fix: NOT byte-identical — modified reuse)

Ported from epiphany-genius. **Cannot be byte-identical** because the
script hardcodes S-prefixed stage-file references (e.g.,
`stages_dir / "S7-v6-scope.txt"` and `stage_presence.get("S7")`) and reads
`index.json`. The port applies the same rename pattern as validate-node.sh:

- `S*-*.md` → `N*-*.md` across all stage-file references
- `S7-v6-scope.txt` → `N7-v6-scope.txt`
- `stage_presence.get("S7")` → `stage_presence.get("N7")`
- `INDEX_FILE="${SKILL_DIR}/index.json"` → `GRAPH_FILE="${SKILL_DIR}/graph.json"`
- Python lookup: `idx["stages"]` and `idx["conditional_modules"]` →
  `graph["nodes"]` (unified list in graph.json)
- `stage_id` field → `node_id` field
- `xml_element` field behavior unchanged (preserved in graph.json per B14)

**Scope of change:** rename + registry path, analogous to validate-node.sh.
No semantic behavior change beyond name/path substitutions.

Decision 5 in §2 is hereby updated: "byte-identical reuse" applies only to
session-init.sh. xml-assemble.sh and validate-node.sh are MODIFIED reuses
(rename-and-repoint). test-runner.sh is also a modified reuse.

### 8.3 validate-node.sh
Renamed from validate-stage.sh. Differences:
- Parameter: `<node_id>` instead of `<stage_id>`.
- Registry: reads `graph.json` instead of `index.json`.
- Lookup path: `graph["nodes"][node_id]` instead of `idx["stages"][stage_id]`.
- Fallback glob: `${STAGES_DIR}/n*` instead of `${STAGES_DIR}/s*`.
- Special case: N7-v6-scope.txt (instead of S7-v6-scope.txt).

**Validation scope (unchanged from validate-stage.sh):**
1. Output file exists and is non-empty.
2. All `required_output_sections` (from graph.json) present in output.
3. For N7: `N7-v6-scope.txt` exists and is non-empty.

**Not in scope:** SIGNAL OUTPUT schema validation (that's orchestrator STEP 3c per EC7); PRC1 graph integrity (that's orchestrator STEP 0); PRA1/PRA2 (that's orchestrator STEP 5).

### 8.4 test-runner.sh
Ported from epiphany-genius. Differences:
- Reads graph.json instead of index.json.
- T-checks apply to N-prefixed stage files instead of S-prefixed.
- T3 scope carve-out verifies `N7-v6-scope.txt` byte-for-byte in distilled output Section 8 (instead of S7-v6-scope.txt).

**T1–T5 checks (unchanged semantically):**
- T1: every active node in session's node_list has non-empty output file.
- T2: every inactive node (not in node_list) has canonical empty-section marker in distilled output.
- T3: N7-v6-scope.txt content appears byte-for-byte in Section 8 of distilled output.
- T4: structural completeness — minimum depth per section.
- T5: if --xml, all schema elements present in output.xml.

Invoked at STEP 5.4 of the orchestrator loop (per-run).

---

## 9. Artifacts

```
{session_output_base}/{session_id}/
├── input.md                              # normalized input verbatim; unconditional
├── report.md                             # copy of N9-output-distilled.md (or report.xml with --xml)
└── stages/
    ├── 00-processed-input.md             # preprocessed input
    ├── N1-state-loading.md               # always
    ├── N2-constraint-escape.md           # STD/DEEP
    ├── N3-peripheral-exploration.md      # STD/DEEP
    ├── N3-1-defixation.md                # conditional (back-edge fired); HYPHEN filename
    ├── N4-dynamic-simulation.md          # DEEP
    ├── N5-precision-forcing.md           # always
    ├── N6-falsification.md               # STD/DEEP
    ├── N6-1-conjecture.md                # --conjecture only; HYPHEN filename
    ├── N7-integration.md                 # always; matches epiphany-genius S7-integration.md naming
    ├── N7-v6-scope.txt                   # scope carve-out (parity with epiphany-genius S7-v6-scope.txt)
    ├── N8-router-record.md               # STD/DEEP; terse routing record
    ├── N9-output-distilled.md            # always; N9 stage output
    ├── session.md                        # metadata + spawn_budget_resolution
    ├── validation-log.md                 # pass/fail per node
    └── test-report.md                    # T1–T5 results from test-runner.sh
```

**Filename convention:**
- Module files (`modules/`): dot notation for variants (`N3.1.md`, `N6.1.md`), per D3 literal.
- Stage output files (`stages/`): hyphen for variants (`N3-1-defixation.md`, `N6-1-conjecture.md`), matching epiphany-genius filesystem pattern.

**Session.md schema:**
```yaml
session_id: <uuid>
scale: <MINIMAL | STANDARD | DEEP>
modifiers: [list of active flags]
wall_seconds_start: <epoch>
wall_seconds: <elapsed integer>
spawns_total: <integer>
executed_nodes: [append-only list of node_ids]
abort_reason: null                    # C6 fix: populated on B17 HALT
spawn_budget_resolution:
  option: A
  rationale: "N4 simulation quality preserved; ≤45-min wall-clock is user metric"
  chosen_at_phase: design
wave_demotions: []                    # O8 fallback events; expected empty
repair_cap_hit: false                 # EC4 annotation
warnings: []                          # EC7 malformed-digest warnings + PRA + spawn failures + validate-node failures
verbose_trace: []                     # populated only under --verbose
```

**Summary line (after STEP 6):**
```
Cognitive enhancement: [N] nodes executed | Confidence: [band] |
[M] contradictions surfaced | Verification: [pass/fail count] |
Creativity type: [...] | Scope: [limited/broad]
```
(Format identical to epiphany-genius STEP 8 with `stages` → `nodes` as the only domain adaptation.)

**Terminal line:**
```
Saved to {session_dir}report.{md|xml}
Session: {session_dir}
```
(`Saved to …` is suppressed under `--no-save`.)

---

## 10. Verification

### 10.1 Pre-run checks
- **PRC1** — orchestrator STEP 0: graph.json parses as DAG when back-edges excluded; edge ids resolve; signal_fields valid; scale-filtered subgraph connected with N1 source / N9 sink.
- **PRC2** — runtime in STEP 3c/3d: SIGNAL_STATE append-only. Any attempt to overwrite a `(node_id, signal_field)` key aborts with explicit error.

### 10.2 Per-run verification (V1–V7 inside N7)
Preserved verbatim from epiphany-genius `kb/verification-gates.md`:
- V1: Primary conclusion stated in terms of N1 irreducibles.
- V2: At least one falsifier recorded.
- V3: Boden creativity type classified.
- V4: Scope statement in/out of frame.
- V5: Two independent representations agree on conclusion.
- V6: Extreme/boundary cases probed.
- V7: Generalization tested against OOD instance.

### 10.3 Post-run assertions
- **PRA1** — orchestrator STEP 5.2: `spawns_total ≤ scale target`. MINIMAL ≤2, STANDARD ≤4, DEEP ≤5, CONJECTURE ≤2.
- **PRA2** — orchestrator STEP 5.3: artifact presence (input.md, report.md|xml, stages/*, session.md fields, N7-v6-scope.txt if N7 executed).

### 10.4 Per-run post-run test battery (T1–T5 via test-runner.sh)
- T1: every active node has non-empty output.
- T2: inactive nodes have canonical empty-section marker in distilled output.
- T3: N7-v6-scope.txt content appears byte-for-byte in distilled Section 8 (backs V6 scope check).
- T4: minimum structural depth per section.
- T5: --xml schema completeness.

### 10.5 Not in scope (explicitly cut)
- IV1 differential test vs epiphany-genius DEEP.
- A2 counterfactual machinery.
- `--differential` user-facing flag.
- `scripts/differential-test.sh`.
- `validate-pipeline.sh` (its function is covered by orchestrator STEP 0 PRC1 inline).

Quality regression against epiphany-genius baseline is detected by human eyeball comparison of 1 canonical input's report.md during skill ship — not by automated tooling.

---

## 11. Edge cases and escape hatches

| ID | Scenario | Handling |
|---|---|---|
| EC1 | W1 "inline Nx" conflicts with a concrete demand to spawn same node | T13 escape hatch: surface contradiction, propose A/B/C options, select, record under spawn_budget_resolution, proceed |
| EC2 | O8 budget exceeded, multiple inline nodes eligible for demotion | Largest kb_preload wins. Tie-break: lex node_id |
| EC3 | N8 gate `NOT N3.1_ran` check | Reads session.md.executed_nodes only. SIGNAL_STATE not consulted (may be absent on --resume before digest rehydration) |
| EC4 | Repair cap already hit (N3.1 fired once) but N8's back-edge gate condition still true | Annotate session.md.repair_cap_hit = true. Do NOT re-enqueue N3.1. N8's gate-open to N7 STILL fires unconditionally (B3 fix); continue to N7 → N9 |
| EC5 | --resume on partial session | Scan stages/. For each candidate file: invoke validate-node.sh first; only VALID files rehydrate SIGNAL_STATE and populate executed_nodes. Invalid/partial files trigger re-execution of that node (B5 fix). Rerun STEP 3e to seed READY_SET |
| EC6 | --minimal + --conjecture | MINIMAL collapses to {N1, N5, N7, N9}; N6.1 fires inline under CONJECTURE_FLAG. Spawn count stays at 2 |
| EC7 | Malformed signal digest (missing field, over line cap, W2 literal missing) | Mark digest missing in SIGNAL_STATE, block dependents, log to session.md.warnings, continue. Do NOT silently pass |
| T13 | Spawn-budget contradiction | Resolved at design as Option A. Recorded in session.md every run |

---

## 12. Performance targets and observability

| Scale | Target wall-clock | Spawn count | vs epiphany-genius |
|---|---|---|---|
| MINIMAL | ≤10 min | 2 | 4 → 2 (50% reduction) |
| STANDARD | ≤25 min | 4 | 7 → 4 (43% reduction) |
| DEEP | ≤45 min | 5 | 8+ → 5 (37%+ reduction) |
| CONJECTURE alone | ≤10 min | 2 | 4 → 2 |

Observability: `session.md` carries `wall_seconds` and `spawns_total` per run. `wave_demotions[]` logs O8 fallbacks (expected to remain empty — max inline wave sum = 277 lines, well under 600 budget).

---

## 13. Explicit out-of-scope (v2 candidates)

- Weighted edges (priority/confidence weights on edges).
- `hats.json` runtime routing of nodes to model tiers (large/medium/small).
- `--differential` flag + automated IV1 comparison.
- Automated pipeline-level regression testing beyond T1–T5.
- Dev-side `validate-pipeline.sh` (replaced by orchestrator-inline PRC1).

---

## 14. Acceptance criteria mapping

| Deliverable | Acceptance |
|---|---|
| D1 (SKILL.md) | Dry-run-trace produces spawns_total ≤ 2 at MINIMAL, ≤ 4 at STANDARD, ≤ 5 at DEEP, ≤ 2 at CONJECTURE. Artifact tree matches §9. |
| D2 (graph.json) | Parses as DAG when back-edges excluded (§4.4). All source/target ids resolve. All signal_fields match declared digests or gate literals. Scale-filter yields connected subgraph with N1 source / N9 sink. 20 edges total (was 19; edge 20 N1→N6.1 added per B11). Every node carries the extended field set per §4.3bis (id, module_file, module_version, type, exec_type, hat, kb_files, context_budget_lines, scale_gates, activation, enabled_when, input_dependencies, output_file, xml_element, required_output_sections, raises_signals). `required_output_sections` present on every node per §2 decision 3. |
| D3 (modules/Nx.md × 11) | Every module declares hat, exec_type, scale_gates, input_ports, output_ports, required_output_sections, and SIGNAL OUTPUT block with signal_digest_schema. Module filename uses dot notation for variants (N3.1.md, N6.1.md). Protocol body copied verbatim from epiphany-genius source (N8.md authored fresh). |
| D4 + F2 extension (scripts/) | Four scripts present. session-init.sh + xml-assemble.sh byte-identical. validate-node.sh differs only in S→N naming + index.json→graph.json lookup. test-runner.sh ported with N-file naming and graph.json lookup; runs every invocation at STEP 5.4. |

---

## 15. Implementation notes

- Decision records (§2 table row 1) must be reflected in both the written SKILL.md orchestrator prose and the per-run session.md `spawn_budget_resolution` field.
- KB directory is populated by copying (not symlinking) 18 files from `~/.claude/skills/epiphany-genius/kb/`. Self-contained per §2 decision 2 (KB reuse = copy).
- Every module's PROTOCOL body must be byte-for-byte identical to its epiphany-genius source except N8.md (authored fresh per §7.4 protocol sketch). The SIGNAL OUTPUT section is appended, not replacing anything. N1.md receives three SIGNAL OUTPUT subsections per §7.3 multi-digest template.
- N8.md's content is the terse routing protocol in §7.4, drawing from epiphany-genius SKILL.md STEP 6's Path A/B logic. It has no KB files and emits the `falsification_digest` as a passthrough alias of N6's `falsification_result` UNCONDITIONALLY (gate-open resolution fix per B3).
- Graph.json is the single source of truth for node `required_output_sections` per §2 decision 3 (registry = graph.json extended). Both validate-node.sh and test-runner.sh read from graph.json (not any index.json).
- Agent() spawn prompt template is referenced from epiphany-genius SKILL.md STEP 5 (canonical pattern). New skill follows the same pattern with updated module paths, digest field names, and (for N9) the additional disk-read paths enumerated in STEP 3d.
- Repair cycle semantic (per B3/B4): if the late back-edge fires (N8→N3.1 after N6 has executed), edge 10 (N3.1→N6) is DEAD-LETTER — N6 does NOT re-execute. N3.1's breakthrough_digest reaches N7 only via edge 11 (N3.1→N7 optional). N7's V2 falsifier gate tests the breakthrough at aggregation time. This preserves PRC2 append-only invariant while retaining cognitive coverage.
