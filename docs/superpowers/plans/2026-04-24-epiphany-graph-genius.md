# epiphany-graph-genius Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a new Claude Code skill `epiphany-graph-genius` at `~/.claude/skills/epiphany-graph-genius/` that reimplements `epiphany-genius` v1.1.0 as a Graph-of-Thought topology declared in `graph.json`, reducing DEEP-scale spawn count from 8+ to 5 with target wall-clock ≤45 min while preserving all 19 Tier-1 cognitive traits and the V1–V7 verification battery.

**Architecture:** Three-layer design. (1) `SKILL.md` orchestrator runs a ready-set execution loop with inline-to-fixpoint per wave then parallel spawn fire. (2) `graph.json` declares 11 nodes + 20 edges + per-node schema. (3) `modules/Nx.md` files contain per-node PROTOCOL bodies (10 copied verbatim from epiphany-genius S* modules + SIGNAL OUTPUT block; N8 authored fresh as a router). KB directory holds 18 files copied from epiphany-genius. Four scripts: session-init.sh (byte-identical reuse), xml-assemble.sh / validate-node.sh / test-runner.sh (modified reuses with S→N rename + index.json→graph.json swap).

**Tech Stack:** Markdown (SKILL.md, modules/), JSON (graph.json), Bash + Python3 (scripts/), Claude Code Agent tool for spawns.

**Spec reference:** `/home/myuser/docs/superpowers/specs/2026-04-24-epiphany-graph-genius-design.md` (1131 lines). All design decisions, edge cases, and behavioral rules are codified there. This plan implements that spec verbatim.

---

## File Structure

Files to be created (or modified-from-source):

```
~/.claude/skills/epiphany-graph-genius/
├── SKILL.md                          # NEW — orchestrator, ~600 lines
├── graph.json                        # NEW — 11 nodes + 20 edges, ~400 lines
├── modules/
│   ├── N1.md                         # COPY+EXTEND from S1-state-loading.md (multi-digest SIGNAL OUTPUT)
│   ├── N2.md                         # COPY+EXTEND from S2-constraint-escape.md
│   ├── N3.md                         # COPY+EXTEND from S3-peripheral-exploration.md
│   ├── N3.1.md                       # COPY+EXTEND from S3-1-defixation.md
│   ├── N4.md                         # COPY+EXTEND from S4-dynamic-simulation.md
│   ├── N5.md                         # COPY+EXTEND from S5-precision-forcing.md
│   ├── N6.md                         # COPY+EXTEND from S6-falsification.md
│   ├── N6.1.md                       # COPY+EXTEND from S6-1-conjecture.md
│   ├── N7.md                         # COPY+EXTEND from S7-integration-verification.md
│   ├── N8.md                         # NEW — router, authored fresh
│   └── N9.md                         # COPY+EXTEND from output-synthesis-pass.md
├── kb/                               # COPY 18 files from epiphany-genius/kb/
│   └── ... (18 files)
└── scripts/
    ├── session-init.sh               # COPY byte-identical from epiphany-genius
    ├── xml-assemble.sh               # MODIFIED PORT (S→N rename, index.json→graph.json)
    ├── validate-node.sh              # MODIFIED PORT from validate-stage.sh
    └── test-runner.sh                # MODIFIED PORT from epiphany-genius
```

---

## Task 1: Skill scaffold + KB copy

**Files:**
- Create directory: `~/.claude/skills/epiphany-graph-genius/`
- Create subdirs: `modules/`, `kb/`, `scripts/`
- Copy 18 files: `~/.claude/skills/epiphany-genius/kb/*.md` → `~/.claude/skills/epiphany-graph-genius/kb/`
- Test: simple shell verification

- [ ] **Step 1: Create skill directory tree**

```bash
mkdir -p ~/.claude/skills/epiphany-graph-genius/{modules,kb,scripts}
```

- [ ] **Step 2: Copy 18 KB files from epiphany-genius**

```bash
cp ~/.claude/skills/epiphany-genius/kb/*.md ~/.claude/skills/epiphany-graph-genius/kb/
```

- [ ] **Step 3: Verify KB copy — count and total line count**

Run:
```bash
ls ~/.claude/skills/epiphany-graph-genius/kb/ | wc -l
wc -l ~/.claude/skills/epiphany-graph-genius/kb/*.md | tail -1
```
Expected: `18` and `1789 total`

- [ ] **Step 4: Verify no KB files were modified (byte-identical)**

Run:
```bash
diff -r ~/.claude/skills/epiphany-genius/kb/ ~/.claude/skills/epiphany-graph-genius/kb/
```
Expected: no output (identical).

- [ ] **Step 5: Commit scaffold**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git init                                   # idempotent; safe to call on existing repo
git add kb/
git commit -m "feat: scaffold epiphany-graph-genius with 18 KB files copied from epiphany-genius v1.1.0"
```
(Audit A7 fix: removed unnecessary `2>/dev/null || true` since `git init` is idempotent.)

---

## Task 2: graph.json — node registry + 20 edges

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/graph.json`
- Test: `~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py` (transient, used to verify graph.json parses correctly; can be removed after task)

- [ ] **Step 1: Write the failing static validation test**

Create `~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py`:

```python
#!/usr/bin/env python3
"""Static validation of graph.json. Used during Task 2 to verify the
graph file parses, has correct counts, and matches spec §4."""
import json
import sys
from pathlib import Path

GRAPH = Path(__file__).parent.parent / "graph.json"

def fail(msg):
    print(f"FAIL: {msg}")
    sys.exit(1)

def ok(msg):
    print(f"OK: {msg}")

with open(GRAPH) as f:
    g = json.load(f)

# Node count
if len(g["nodes"]) != 11:
    fail(f"expected 11 nodes, got {len(g['nodes'])}")
ok(f"11 nodes present")

expected_ids = {"N1", "N2", "N3", "N3.1", "N4", "N5", "N6", "N6.1", "N7", "N8", "N9"}
actual_ids = {n["id"] for n in g["nodes"]}
if actual_ids != expected_ids:
    fail(f"node ids mismatch. expected {expected_ids}, got {actual_ids}")
ok("all 11 expected node ids present")

# Required per-node fields
required_fields = {"id", "module_file", "module_version", "type", "exec_type",
                   "hat", "kb_files", "context_budget_lines", "scale_gates",
                   "activation", "enabled_when", "input_dependencies",
                   "output_file", "xml_element", "required_output_sections",
                   "raises_signals"}
for n in g["nodes"]:
    missing = required_fields - set(n.keys())
    if missing:
        fail(f"node {n['id']}: missing fields {missing}")
ok("all nodes have all 16 required fields")

# Edge count
if len(g["edges"]) != 20:
    fail(f"expected 20 edges, got {len(g['edges'])}")
ok("20 edges present")

# All edge source/target resolve
node_ids = actual_ids | {"input", "output"}
for i, e in enumerate(g["edges"]):
    if e["source"] not in node_ids:
        fail(f"edge {i+1}: source {e['source']} does not resolve")
    if e["target"] not in node_ids:
        fail(f"edge {i+1}: target {e['target']} does not resolve")
ok("all edge source/target ids resolve")

# Signal field validation
DECLARED_DIGESTS = {"primitives_digest", "irreducibles_digest", "system_model_seed",
                    "activation_digest", "breakthrough_digest", "simulation_digest",
                    "precision_digest", "falsification_result", "falsification_digest",
                    "domain_mappings_digest", "conjecture_digest", "integration_digest"}
for i, e in enumerate(g["edges"]):
    sf = e.get("signal_field")
    if sf in (None, "—"):
        continue
    if sf.startswith("gate:"):
        continue
    if sf not in DECLARED_DIGESTS:
        fail(f"edge {i+1}: signal_field {sf} not declared")
ok("all signal_fields resolve to declared digests or gate literals")

# DAG check — exclude back-edges
back_edge_indices = [i for i, e in enumerate(g["edges"]) if e["type"] == "back-edge"]
if len(back_edge_indices) != 2:
    fail(f"expected 2 back-edges, got {len(back_edge_indices)}")
ok("exactly 2 back-edges present (edges 9, 15)")

# Build adjacency excluding back-edges and check for cycles via topological sort
from collections import defaultdict, deque
adj = defaultdict(list)
indegree = defaultdict(int)
for i, e in enumerate(g["edges"]):
    if e["type"] == "back-edge":
        continue
    if e["source"] in actual_ids and e["target"] in actual_ids:
        adj[e["source"]].append(e["target"])
        indegree[e["target"]] += 1

queue = deque([n for n in actual_ids if indegree[n] == 0])
visited = 0
while queue:
    node = queue.popleft()
    visited += 1
    for tgt in adj[node]:
        indegree[tgt] -= 1
        if indegree[tgt] == 0:
            queue.append(tgt)
if visited != len(actual_ids):
    fail(f"DAG check failed — only {visited}/{len(actual_ids)} nodes reachable in topological sort (cycle present)")
ok("graph is a DAG when back-edges excluded")

print("\nALL CHECKS PASSED")
```

- [ ] **Step 2: Make script executable, run it (should fail — graph.json missing)**

```bash
chmod +x ~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py
python3 ~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py
```
Expected: error opening `graph.json` (file does not exist yet).

- [ ] **Step 3: Write graph.json with 11 nodes**

Create `~/.claude/skills/epiphany-graph-genius/graph.json`. Reference spec §4.1 nodes table and §4.3bis schema:

```json
{
  "version": "1.0.0",
  "kb_base": "~/.claude/skills/epiphany-graph-genius/kb/",
  "session_output_base": "~/docs/epiphany/graph-genius/",
  "nodes": [
    {
      "id": "N1",
      "module_file": "modules/N1.md",
      "module_version": "1.0.0",
      "type": "DECOMPOSITION",
      "exec_type": "inline",
      "hat": "Einstein/Feynman/VonNeumann/Darwin/Tesla/Turing",
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
    },
    {
      "id": "N2",
      "module_file": "modules/N2.md",
      "module_version": "1.0.0",
      "type": "LATERAL",
      "exec_type": "spawn",
      "hat": "de Bono",
      "kb_files": ["domain-catalog.md", "blend-template.md", "debono-techniques.md"],
      "context_budget_lines": 1200,
      "scale_gates": ["STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": ["stages/00-processed-input.md"],
      "output_file": "stages/N2-constraint-escape.md",
      "xml_element": "constraint_escape",
      "required_output_sections": [
        "Named constraint",
        "Domain candidates surveyed",
        "Domain mappings (5)",
        "Activation provenance",
        "Constraint-escape path"
      ],
      "raises_signals": []
    },
    {
      "id": "N3",
      "module_file": "modules/N3.md",
      "module_version": "1.0.0",
      "type": "SPREADING",
      "exec_type": "inline",
      "hat": "spreading-activation+elegance-rubric",
      "kb_files": ["spreading-activation.md", "elegance-rubric.md"],
      "context_budget_lines": 1000,
      "scale_gates": ["STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": ["stages/00-processed-input.md"],
      "output_file": "stages/N3-peripheral-exploration.md",
      "xml_element": "peripheral_exploration",
      "required_output_sections": [
        "Activation map",
        "Convergent nodes",
        "Selected illuminations",
        "Verification"
      ],
      "raises_signals": ["S3_thin_or_empty"]
    },
    {
      "id": "N3.1",
      "module_file": "modules/N3.1.md",
      "module_version": "1.0.0",
      "type": "DEFIXATION",
      "exec_type": "inline",
      "hat": "Ohlsson",
      "kb_files": ["ohlsson-defixation.md"],
      "context_budget_lines": 700,
      "scale_gates": ["STANDARD", "DEEP"],
      "activation": ["conditional:back-edge"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/00-processed-input.md",
        "stages/N3-peripheral-exploration.md"
      ],
      "output_file": "stages/N3-1-defixation.md",
      "xml_element": "defixation",
      "required_output_sections": [
        "Constraint relaxations",
        "Chunk decompositions",
        "Re-encodings",
        "Breakthrough candidate"
      ],
      "raises_signals": []
    },
    {
      "id": "N4",
      "module_file": "modules/N4.md",
      "module_version": "1.0.0",
      "type": "SIMULATION",
      "exec_type": "spawn",
      "hat": "Tesla+observer-frames",
      "kb_files": ["observer-frames.md", "simulation-checklist.md"],
      "context_budget_lines": 1200,
      "scale_gates": ["DEEP"],
      "activation": ["scale:DEEP"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/N1-state-loading.md",
        "stages/00-processed-input.md"
      ],
      "output_file": "stages/N4-dynamic-simulation.md",
      "xml_element": "dynamic_simulation",
      "required_output_sections": [
        "System model",
        "Observer frame analyses",
        "Imbalances and singularities",
        "Limit cases",
        "Theory collisions"
      ],
      "raises_signals": []
    },
    {
      "id": "N5",
      "module_file": "modules/N5.md",
      "module_version": "1.0.0",
      "type": "PRECISION",
      "exec_type": "inline",
      "hat": "Feynman",
      "kb_files": ["vocabulary-rubric.md", "forward-chain-template.md"],
      "context_budget_lines": 1100,
      "scale_gates": ["MINIMAL", "STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/N1-state-loading.md",
        "stages/00-processed-input.md"
      ],
      "output_file": "stages/N5-precision-forcing.md",
      "xml_element": "precision_forcing",
      "required_output_sections": [
        "5-level vocabulary strip",
        "3-representation rendering",
        "Constructive specification",
        "Flagged vague claims",
        "Forward-chain consequence tree"
      ],
      "raises_signals": []
    },
    {
      "id": "N6",
      "module_file": "modules/N6.md",
      "module_version": "1.0.0",
      "type": "ADVERSARIAL",
      "exec_type": "spawn",
      "hat": "Popper+Millikan",
      "kb_files": ["tot-templates.md", "falsification-checklists.md"],
      "context_budget_lines": 1500,
      "scale_gates": ["STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/N3-peripheral-exploration.md",
        "stages/00-processed-input.md",
        "stages/N1-state-loading.md"
      ],
      "output_file": "stages/N6-falsification.md",
      "xml_element": "falsification",
      "required_output_sections": [
        "Abductive mode results",
        "Deductive mode results",
        "Inductive mode results",
        "Consolidated hypotheses",
        "Extreme scenarios",
        "Millikan assessment",
        "Density report"
      ],
      "raises_signals": ["S6_no_alternatives"]
    },
    {
      "id": "N6.1",
      "module_file": "modules/N6.1.md",
      "module_version": "1.0.0",
      "type": "CONJECTURE",
      "exec_type": "inline",
      "hat": "Ramanujan",
      "kb_files": ["pattern-taxonomy.md"],
      "context_budget_lines": 900,
      "scale_gates": ["CONJECTURE_FLAG"],
      "activation": ["flag:--conjecture"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/N1-state-loading.md",
        "stages/00-processed-input.md"
      ],
      "output_file": "stages/N6-1-conjecture.md",
      "xml_element": "conjecture",
      "required_output_sections": [
        "Detected patterns",
        "Conjectures (≥3)",
        "Verification instances"
      ],
      "raises_signals": []
    },
    {
      "id": "N7",
      "module_file": "modules/N7.md",
      "module_version": "1.0.0",
      "type": "AGGREGATION",
      "exec_type": "spawn",
      "hat": "Feynman+Boden",
      "kb_files": [
        "verification-gates.md",
        "boden-types.md",
        "scope-template.md",
        "representation-frames.md",
        "elegance-rubric.md"
      ],
      "context_budget_lines": 1500,
      "scale_gates": ["MINIMAL", "STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": [
        "stages/00-processed-input.md",
        "stages/N1-state-loading.md",
        "stages/N5-precision-forcing.md",
        "stages/N2-constraint-escape.md",
        "stages/N3-peripheral-exploration.md",
        "stages/N3-1-defixation.md",
        "stages/N4-dynamic-simulation.md",
        "stages/N6-falsification.md",
        "stages/N6-1-conjecture.md",
        "stages/N8-router-record.md"
      ],
      "output_file": "stages/N7-integration.md",
      "xml_element": "integration",
      "required_output_sections": [
        "Convergence signals",
        "Primary conclusion",
        "Confidence band",
        "Creativity type",
        "Generalization check",
        "Verification report (V1-V7)"
      ],
      "raises_signals": []
    },
    {
      "id": "N8",
      "module_file": "modules/N8.md",
      "module_version": "1.0.0",
      "type": "ROUTER",
      "exec_type": "inline",
      "hat": null,
      "kb_files": [],
      "context_budget_lines": 50,
      "scale_gates": ["STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": null,
      "input_dependencies": [],
      "output_file": "stages/N8-router-record.md",
      "xml_element": null,
      "required_output_sections": [
        "resolved_gate",
        "falsification_digest",
        "signal_flags_checked",
        "decision_rationale"
      ],
      "raises_signals": []
    },
    {
      "id": "N9",
      "module_file": "modules/N9.md",
      "module_version": "1.0.0",
      "type": "SYNTHESIS",
      "exec_type": "spawn",
      "hat": "Feynman+Boden(synthesizer)",
      "kb_files": [],
      "context_budget_lines": 2000,
      "scale_gates": ["MINIMAL", "STANDARD", "DEEP"],
      "activation": ["always"],
      "enabled_when": "NOT --xml",
      "input_dependencies": [
        "stages/N1-state-loading.md",
        "stages/N5-precision-forcing.md",
        "stages/N7-integration.md",
        "stages/N7-v6-scope.txt",
        "input.md"
      ],
      "output_file": "stages/N9-output-distilled.md",
      "xml_element": null,
      "required_output_sections": [
        "Pipeline Status Header",
        "Headline Insight",
        "Theory Collisions",
        "Discovery vs. Proof",
        "Independence-Verified Bridges",
        "Alternative Hypotheses",
        "Density-Checked Falsification",
        "Scope Limits",
        "Coherence Signals",
        "Generalization Checks",
        "Open Questions & Next Probes"
      ],
      "raises_signals": []
    }
  ],
  "edges": [
    { "id": 1, "source": "input", "target": "N1", "type": "required", "scale_gates": ["MINIMAL", "STANDARD", "DEEP"], "signal_field": "—", "gate_condition": null },
    { "id": 2, "source": "N1", "target": "N2", "type": "required", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "primitives_digest", "gate_condition": null },
    { "id": 3, "source": "N1", "target": "N3", "type": "required", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "primitives_digest", "gate_condition": null },
    { "id": 4, "source": "N1", "target": "N4", "type": "required", "scale_gates": ["DEEP"], "signal_field": "system_model_seed", "gate_condition": null },
    { "id": 5, "source": "N1", "target": "N5", "type": "required", "scale_gates": ["MINIMAL", "STANDARD", "DEEP"], "signal_field": "irreducibles_digest", "gate_condition": null },
    { "id": 6, "source": "N2", "target": "N7", "type": "optional", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "domain_mappings_digest", "gate_condition": null },
    { "id": 7, "source": "N3", "target": "N6", "type": "required", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "activation_digest", "gate_condition": null },
    { "id": 8, "source": "N3", "target": "N7", "type": "optional", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "activation_digest", "gate_condition": null },
    { "id": 9, "source": "N3", "target": "N3.1", "type": "back-edge", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "gate:S3_thin_or_empty", "gate_condition": "SIGNAL_STATE[(N3, activation_digest)].signal_flags includes 'S3_thin_or_empty'" },
    { "id": 10, "source": "N3.1", "target": "N6", "type": "optional", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "breakthrough_digest", "gate_condition": null },
    { "id": 11, "source": "N3.1", "target": "N7", "type": "optional", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "breakthrough_digest", "gate_condition": null },
    { "id": 12, "source": "N4", "target": "N7", "type": "optional", "scale_gates": ["DEEP"], "signal_field": "simulation_digest", "gate_condition": null },
    { "id": 13, "source": "N5", "target": "N7", "type": "required", "scale_gates": ["MINIMAL", "STANDARD", "DEEP"], "signal_field": "precision_digest", "gate_condition": null },
    { "id": 14, "source": "N6", "target": "N8", "type": "required", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "falsification_result", "gate_condition": null },
    { "id": 15, "source": "N8", "target": "N3.1", "type": "back-edge", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "gate:S6_no_alternatives AND NOT N3.1_ran", "gate_condition": "SIGNAL_STATE[(N6, falsification_result)].signal_flags includes 'S6_no_alternatives' AND 'N3.1' not in executed_nodes" },
    { "id": 16, "source": "N8", "target": "N7", "type": "gate-open", "scale_gates": ["STANDARD", "DEEP"], "signal_field": "falsification_digest", "gate_condition": null },
    { "id": 17, "source": "N6.1", "target": "N7", "type": "optional", "scale_gates": ["CONJECTURE_FLAG"], "signal_field": "conjecture_digest", "gate_condition": null },
    { "id": 18, "source": "N7", "target": "N9", "type": "required", "scale_gates": ["MINIMAL", "STANDARD", "DEEP"], "signal_field": "integration_digest", "gate_condition": null },
    { "id": 19, "source": "N9", "target": "output", "type": "terminal", "scale_gates": ["MINIMAL", "STANDARD", "DEEP"], "signal_field": "—", "gate_condition": null },
    { "id": 20, "source": "N1", "target": "N6.1", "type": "required", "scale_gates": ["CONJECTURE_FLAG"], "signal_field": "primitives_digest", "gate_condition": null }
  ]
}
```

- [ ] **Step 4: Run validate-graph-static.py — should pass**

Run:
```bash
python3 ~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py
```
Expected: `ALL CHECKS PASSED` with all OK lines including "20 edges present", "all 11 expected node ids present", "graph is a DAG when back-edges excluded".

- [ ] **Step 5: Remove the transient validation script (no longer needed)**

```bash
rm ~/.claude/skills/epiphany-graph-genius/scripts/validate-graph-static.py
```

- [ ] **Step 6: Commit graph.json**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add graph.json
git commit -m "feat: add graph.json with 11 nodes and 20 edges per spec §4"
```

---

## Task 3: Scripts — port from epiphany-genius

### Task 3a: session-init.sh — byte-identical copy

**Files:**
- Copy: `~/.claude/skills/epiphany-genius/scripts/session-init.sh` → `~/.claude/skills/epiphany-graph-genius/scripts/session-init.sh`

- [ ] **Step 1: Copy the script byte-identical**

```bash
cp ~/.claude/skills/epiphany-genius/scripts/session-init.sh \
   ~/.claude/skills/epiphany-graph-genius/scripts/session-init.sh
```

- [ ] **Step 2: Verify byte-identical**

```bash
diff ~/.claude/skills/epiphany-genius/scripts/session-init.sh \
     ~/.claude/skills/epiphany-graph-genius/scripts/session-init.sh
```
Expected: no output.

- [ ] **Step 3: Make executable**

```bash
chmod +x ~/.claude/skills/epiphany-graph-genius/scripts/session-init.sh
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add scripts/session-init.sh
git commit -m "feat: add session-init.sh (byte-identical reuse from epiphany-genius)"
```

### Task 3b: validate-node.sh — modified port from validate-stage.sh

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh` (from `~/.claude/skills/epiphany-genius/scripts/validate-stage.sh`)

- [ ] **Step 1: Copy validate-stage.sh as starting point**

```bash
cp ~/.claude/skills/epiphany-genius/scripts/validate-stage.sh \
   ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
chmod +x ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
```

- [ ] **Step 2: Update header comments for new skill name**

Edit `~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh`:

Replace lines 2-6 (the header comment block) with:
```bash
# validate-node.sh — epiphany-graph-genius v1.0.0
# Usage: validate-node.sh <session_dir> <node_id>
# Checks: output file exists + non-empty + required sections present.
# Called by orchestrator STEP 5.1 and from --resume validation in STEP 2.3.b.
# Exit 0 = PASS, Exit 1 = FAIL.
```

- [ ] **Step 3: Update parameter names from stage_id to node_id (audit M2 — also rename STAGE_LC to NODE_LC)**

Replace all occurrences of `STAGE_ID`, `STAGE_LC`, `stage_id`, etc. in the bash and Python code:

```bash
sed -i \
  -e 's/STAGE_ID/NODE_ID/g' \
  -e 's/STAGE_LC/NODE_LC/g' \
  -e 's/stage_id/node_id/g' \
  -e 's/<stage_id>/<node_id>/g' \
  ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
```

- [ ] **Step 4: Update registry references from index.json to graph.json**

```bash
sed -i 's|INDEX_FILE="${SKILL_DIR}/index.json"|GRAPH_FILE="${SKILL_DIR}/graph.json"|g; \
        s|index.json|graph.json|g; \
        s|INDEX_FILE|GRAPH_FILE|g' \
    ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
```

- [ ] **Step 5: Update Python lookup paths**

Open `~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh` in an editor. Inside the python heredoc (lines around 25-65), find:

```python
# Search core stages, conditionals, and OSP
entries = idx.get("stages", []) + idx.get("conditional_modules", [])
```

Replace with:
```python
# Search nodes (graph.json has unified node list)
entries = idx.get("nodes", [])
```

Find:
```python
if stage_id.lower() == "osp":
```

Replace with (and update the OSP-equivalent block to use N9):
```python
if node_id.upper() == "N9":
```

Inside that N9 block, replace `stages/output-distilled.md` with `stages/N9-output-distilled.md` and update `required_sections` to use N9's required_output_sections from graph.json. Actually — simplify: remove the special-case N9 block entirely and let the generic `for entry in entries` loop handle N9 like any other node, since graph.json includes N9 as a normal node entry.

The simplified Python lookup section becomes:
```python
for entry in entries:
    if entry.get("id") == node_id:
        output_file = entry.get("output_file", "")
        required_sections = entry.get("required_output_sections", [])
        print(f"OUTPUT_FILE={output_file}")
        print(f"REQUIRED_SECTIONS={chr(0x1f).join(required_sections)}")
        sys.exit(0)

print(f"ERROR=Node {node_id} not found in graph.json")
sys.exit(1)
```

- [ ] **Step 6: Verify the fallback glob is consistent with N-naming**

The Step 3 sed already renamed `STAGE_LC` → `NODE_LC`. Verify by reading:

```bash
grep -n "NODE_LC" ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
```

Expected: two lines, the variable assignment using `tr '[:upper:]' '[:lower:]'` and the `ls` glob using `${NODE_LC}*.md`. The lowercase n-prefix matches the new skill's stage-file naming (n1, n3-1, n6-1, n7, etc.).

- [ ] **Step 7: Update N7 special case — replace S7 references with N7**

Find the block:
```bash
# S7 special case: also verify S7-v6-scope.txt was written
if [ "${NODE_ID}" = "S7" ] && [ $FAIL -eq 0 ]; then
  V6_SCOPE="${STAGES_DIR}/S7-v6-scope.txt"
  ...
```

Replace with:
```bash
# N7 special case: also verify N7-v6-scope.txt was written (required by T3 and N9)
if [ "${NODE_ID}" = "N7" ] && [ $FAIL -eq 0 ]; then
  V6_SCOPE="${STAGES_DIR}/N7-v6-scope.txt"
  if [ ! -f "$V6_SCOPE" ]; then
    echo "FAIL: N7 — N7-v6-scope.txt not found (required for N9 V6 verbatim carve-out and T3)"
    FAIL=1
  elif [ ! -s "$V6_SCOPE" ]; then
    echo "FAIL: N7 — N7-v6-scope.txt exists but is empty"
    FAIL=1
  else
    echo "PASS: N7 — N7-v6-scope.txt present and non-empty"
  fi
fi
```

- [ ] **Step 8: Verify the script parses (syntax check)**

```bash
bash -n ~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh
```
Expected: no output (no syntax errors).

- [ ] **Step 9: Smoke-test against a non-existent session (should error gracefully)**

```bash
~/.claude/skills/epiphany-graph-genius/scripts/validate-node.sh /tmp/nonexistent-session N1
```
Expected: error mentioning the session_dir or output file not found, exit 1.

- [ ] **Step 10: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add scripts/validate-node.sh
git commit -m "feat: add validate-node.sh (port of validate-stage.sh with S→N rename + graph.json lookup)"
```

### Task 3c: test-runner.sh — modified port

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh` (from `~/.claude/skills/epiphany-genius/scripts/test-runner.sh`)

- [ ] **Step 1: Copy test-runner.sh as starting point**

```bash
cp ~/.claude/skills/epiphany-genius/scripts/test-runner.sh \
   ~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh
chmod +x ~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh
```

- [ ] **Step 2: Update header to reflect new skill name and registry**

Edit lines 2-9 of the new file to:
```bash
# test-runner.sh — epiphany-graph-genius v1.0.0
# Usage: test-runner.sh <session_dir> <scale> <active_conditionals_csv> <output_mode>
# Runs T1–T5 checks against N-prefixed stage files. Writes stages/test-report.md.
# Called by orchestrator STEP 5.4.
# T1: Every active node in node_list has non-empty output file.
# T2: Every node NOT in node_list has canonical empty-section marker in distilled output.
# T3: N7-v6-scope.txt content appears byte-for-byte in distilled Section 8.
# T4: Structural completeness — minimum depth per section (sourced from complete nodes).
# T5: If --xml: all top-level schema elements present.
```

- [ ] **Step 3: Substitute index.json → graph.json and S → N references**

```bash
sed -i 's|INDEX_FILE="${SKILL_DIR}/index.json"|GRAPH_FILE="${SKILL_DIR}/graph.json"|g; \
        s|index.json|graph.json|g; \
        s|INDEX_FILE|GRAPH_FILE|g; \
        s/S7-v6-scope.txt/N7-v6-scope.txt/g' \
    ~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh
```

- [ ] **Step 4: Update Python heredoc — entries, stage_id field, S7 references**

Open `~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh` in an editor. Inside the Python heredoc:

Replace:
```python
entries = idx.get("stages", []) + idx.get("conditional_modules", [])
```
With:
```python
entries = idx.get("nodes", [])
```

Replace `stage_id` (Python variable) with `node_id` throughout the heredoc.

Replace `entry.get("stage_id"` with `entry.get("id"`.

Replace any references to `S7` (as the integration node check) with `N7`.

Replace any references to `output-distilled.md` with `N9-output-distilled.md` (matching new skill's stage file naming).

- [ ] **Step 5: Update output_synthesis_pass references**

If the Python references `idx.get("output_synthesis_pass", {})` — replace with logic that finds the N9 entry in `nodes` (since graph.json has N9 as a normal node entry, not separated like epiphany-genius's index.json).

```python
# Old: osp = idx.get("output_synthesis_pass", {})
# New: find N9 entry
n9 = next((n for n in idx.get("nodes", []) if n.get("id") == "N9"), {})
osp_output = n9.get("output_file", "stages/N9-output-distilled.md")
```

- [ ] **Step 6: Verify the script parses**

```bash
bash -n ~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh
```
Expected: no output.

- [ ] **Step 7: Smoke-test against a non-existent session**

```bash
~/.claude/skills/epiphany-graph-genius/scripts/test-runner.sh /tmp/nonexistent-session DEEP "none" distilled
```
Expected: error or skip; exit gracefully.

- [ ] **Step 8: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add scripts/test-runner.sh
git commit -m "feat: add test-runner.sh (port with S→N rename + graph.json lookup)"
```

### Task 3d: xml-assemble.sh — modified port

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/scripts/xml-assemble.sh` (from `~/.claude/skills/epiphany-genius/scripts/xml-assemble.sh`)

- [ ] **Step 1: Copy as starting point**

```bash
cp ~/.claude/skills/epiphany-genius/scripts/xml-assemble.sh \
   ~/.claude/skills/epiphany-graph-genius/scripts/xml-assemble.sh
chmod +x ~/.claude/skills/epiphany-graph-genius/scripts/xml-assemble.sh
```

- [ ] **Step 2: Update header**

Edit lines 1-6 of the new file:
```bash
#!/usr/bin/env bash
# xml-assemble.sh — epiphany-graph-genius v1.0.0
# Assembles stages/output.xml from stage output files, driven by graph.json.
# Called by orchestrator STEP 4.1 (under --xml mode).
# N9 (OSP) is NOT invoked under --xml; this script produces output.xml directly.
```

- [ ] **Step 3: Substitute references**

```bash
sed -i 's|INDEX_FILE="${SKILL_DIR}/index.json"|GRAPH_FILE="${SKILL_DIR}/graph.json"|g; \
        s|index.json|graph.json|g; \
        s|INDEX_FILE|GRAPH_FILE|g; \
        s/S7-v6-scope.txt/N7-v6-scope.txt/g' \
    ~/.claude/skills/epiphany-graph-genius/scripts/xml-assemble.sh
```

- [ ] **Step 4: Update Python heredoc**

Inside the Python heredoc, replace `idx.get("stages", [])` (and any `idx.get("conditional_modules", [])` concatenation) with `idx.get("nodes", [])`. Replace `stage_id` field reads with `id`. Replace `stage_presence.get("S7")` with `stage_presence.get("N7")`. Replace any S-prefix file references with N-prefix.

Specifically find around line 95:
```python
active_conditionals.append(entry.get("stage_id", ""))
```
Replace with:
```python
active_conditionals.append(entry.get("id", ""))
```

Find around line 140:
```python
stage_id = entry.get("stage_id", "?")
```
Replace with:
```python
node_id = entry.get("id", "?")
```

Update all subsequent `stage_id` variable references to `node_id`.

Find:
```python
if stage_presence.get("S7"):
```
Replace with:
```python
if stage_presence.get("N7"):
```

- [ ] **Step 5: Verify the script parses**

```bash
bash -n ~/.claude/skills/epiphany-graph-genius/scripts/xml-assemble.sh
```
Expected: no output.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add scripts/xml-assemble.sh
git commit -m "feat: add xml-assemble.sh (port with S→N rename + graph.json lookup)"
```

---

## Task 4: Module files

### Field convention reference (audit A6)

Two related fields appear in two places:

- **Module frontmatter `input_ports`** (per spec §7.1): documents the digest deps the node consumes via incoming signal edges. Each entry: `{from: <node_id>, signal_field: <digest_name>, required: <bool>, type: <required|optional|gate-open|back-edge>}`. Documentation only — orchestrator reads the actual edge graph from graph.json.

- **graph.json node `input_dependencies`** (per spec §4.3bis): documents the on-disk file paths the node reads (beyond what signal digests carry). Used by the orchestrator to construct spawn prompts (the disk-read enumeration in Task 5e Step 3d).

A node may have BOTH (e.g., N7 has digest deps in input_ports AND disk reads in input_dependencies for full N1 stage file content). They serve distinct purposes; do not conflate them.

### Canonical S→N body substitution (referenced by Tasks 4b–4i per audit P1)

After copying each S<X>-*.md module body, the module's PROTOCOL prose
contains internal references to other stage files (e.g., "read
stages/S1-state-loading.md", "write stages/S7-v6-scope.txt"). These must
be rewritten S→N before commit, otherwise subagents will look for
non-existent files.

Each Task 4b through 4i runs this sed command against its respective
module file (replace `<NX>` with the target node id like `N3` or `N3.1`):

```bash
sed -i \
  -e 's|stages/S1-state-loading\.md|stages/N1-state-loading.md|g' \
  -e 's|stages/S2-constraint-escape\.md|stages/N2-constraint-escape.md|g' \
  -e 's|stages/S3-peripheral-exploration\.md|stages/N3-peripheral-exploration.md|g' \
  -e 's|stages/S3-1-defixation\.md|stages/N3-1-defixation.md|g' \
  -e 's|stages/S4-dynamic-simulation\.md|stages/N4-dynamic-simulation.md|g' \
  -e 's|stages/S5-precision-forcing\.md|stages/N5-precision-forcing.md|g' \
  -e 's|stages/S6-falsification\.md|stages/N6-falsification.md|g' \
  -e 's|stages/S6-1-conjecture\.md|stages/N6-1-conjecture.md|g' \
  -e 's|stages/S7-integration\.md|stages/N7-integration.md|g' \
  -e 's|S7-v6-scope\.txt|N7-v6-scope.txt|g' \
  -e 's|\bS3\.1\b|N3.1|g' \
  -e 's|\bS6\.1\b|N6.1|g' \
  -e 's|\bS1\b|N1|g' \
  -e 's|\bS2\b|N2|g' \
  -e 's|\bS3\b|N3|g' \
  -e 's|\bS4\b|N4|g' \
  -e 's|\bS5\b|N5|g' \
  -e 's|\bS6\b|N6|g' \
  -e 's|\bS7\b|N7|g' \
  ~/.claude/skills/epiphany-graph-genius/modules/<NX>.md
```

Order matters: longer patterns first (S3.1 before S3, S6.1 before S6) so
prefix substitutions don't clobber numbered variants. Word boundaries
(`\b`) prevent matching parts of other words.

After each module's sed, verify with:
```bash
grep -nE '\bS[0-9]\b|S[0-9]-[a-z]' ~/.claude/skills/epiphany-graph-genius/modules/<NX>.md
```
Expected: no output (no remaining S-prefix references).

This sed is added as a NEW step between "Append SIGNAL OUTPUT" and "Commit"
in each of Tasks 4b through 4i below. Task 4a (N1) does not need it (N1
has no incoming dependencies on other stages). Task 4j (N9) already
includes its own S→N sed in Step 3 of that sub-task. Task 4k (N8) is
authored fresh, no S-references.

### Task 4a: N1.md — multi-digest with three SIGNAL OUTPUT subsections

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N1.md` (from `~/.claude/skills/epiphany-genius/modules/S1-state-loading.md` + frontmatter + multi-digest SIGNAL OUTPUT)

- [ ] **Step 1: Copy the source module as starting point**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S1-state-loading.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N1.md
```

- [ ] **Step 2: Prepend frontmatter**

Edit `~/.claude/skills/epiphany-graph-genius/modules/N1.md`. At the very top of the file, insert (before existing content):

```yaml
---
node_id: N1
node_type: DECOMPOSITION
hat: "Einstein/Feynman/VonNeumann/Darwin/Tesla/Turing"
exec_type: inline
scale_gates: [MINIMAL, STANDARD, DEEP]
kb_files:
  - input-preloading-templates.md
context_budget_lines: 150
input_ports:
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N2
    signal_field: primitives_digest
    type: required
  - to: N3
    signal_field: primitives_digest
    type: required
  - to: N4
    signal_field: system_model_seed
    type: required
  - to: N5
    signal_field: irreducibles_digest
    type: required
  - to: N6.1
    signal_field: primitives_digest
    type: required
required_output_sections:
  - "Input class"
  - "Primitives"
  - "Simplicity-stripped irreducibles"
  - "Tesla measurement plan"
  - "Verification"
raises_signals: []
---

```

- [ ] **Step 3a (NEW per audit P1): Append digest derivation guide BEFORE SIGNAL OUTPUT**

Append at the end of the file (before the SIGNAL OUTPUT block):

```markdown

## DIGEST DERIVATION (added for N1's multi-digest emission per spec §5.4)

After completing the standard N1 protocol output, derive the three digests
as follows. Each digest is 5–15 lines, independent, with the four-field
schema. The same primitive may appear in multiple digests with different
framings.

- **primitives_digest** — distill the "Primitives" section: enumerate
  concrete entities, relations, processes, constraints by category.
  Captures categorical structure for downstream lateral (N2) and
  spreading-activation (N3) work.

- **irreducibles_digest** — distill the "Simplicity-stripped irreducibles"
  section: what remains after decoration is removed. Captures the bare
  irreducible core for downstream precision forcing (N5).

- **system_model_seed** — synthesize from the "Tesla measurement plan" +
  "Primitives" sections: identify the physical/logical/informational
  structure that downstream dynamic simulation (N4) will operate on.
  Capture conserved quantities, observer frames implied by the input,
  system boundaries, and time/scale axes. This is the NEW digest
  introduced by the GoT design (no direct equivalent in epiphany-genius
  S1); on inputs without obvious physical/logical structure, emit a
  minimal seed describing the abstract state space.
```

- [ ] **Step 3b: Append multi-digest SIGNAL OUTPUT block**

Append at the end of the file:

```markdown

## SIGNAL OUTPUT

N1 emits THREE digests from this single stage execution. Each digest is
5–15 lines of payload (counted as non-empty content lines inside the
code-block fences). Write all three SIGNAL OUTPUT subsections at the
end of your stage output file.

### Digest 1 of 3 — primitives_digest (5–15 lines)
Schema:
```
key_findings:     [primitives enumerated by category — concrete entities, relations, processes, constraints]
named_entities:   [named primitives extracted verbatim from input]
confidence_flags: [H | M | L per finding, reflecting evidence strength]
signal_flags:     []
```

### Digest 2 of 3 — irreducibles_digest (5–15 lines)
Schema:
```
key_findings:     [simplicity-stripped irreducibles — what remains after removing decoration]
named_entities:   [named irreducible entities or relations]
confidence_flags: [H | M | L per finding]
signal_flags:     []
```

### Digest 3 of 3 — system_model_seed (5–15 lines)
Schema:
```
key_findings:     [system model seed: physical / logical / informational structure for simulation]
named_entities:   [named components, interactions, conserved quantities]
confidence_flags: [H | M | L per finding]
signal_flags:     []
```
```

- [ ] **Step 4: Verify file structure (frontmatter + body + 3 SIGNAL OUTPUT subsections)**

```bash
head -30 ~/.claude/skills/epiphany-graph-genius/modules/N1.md
grep -c "^### Digest" ~/.claude/skills/epiphany-graph-genius/modules/N1.md
```
Expected: frontmatter visible at top; `3` (three Digest subsections).

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N1.md
git commit -m "feat: add N1.md module (state loading, multi-digest emit)"
```

### Task 4b: N2.md — single-digest module

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N2.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S2-constraint-escape.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N2.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top of `~/.claude/skills/epiphany-graph-genius/modules/N2.md`:

```yaml
---
node_id: N2
node_type: LATERAL
hat: "de Bono"
exec_type: spawn
scale_gates: [STANDARD, DEEP]
kb_files:
  - domain-catalog.md
  - blend-template.md
  - debono-techniques.md
context_budget_lines: 1200
input_ports:
  - from: N1
    signal_field: primitives_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N7
    signal_field: domain_mappings_digest
    type: optional
required_output_sections:
  - "Named constraint"
  - "Domain candidates surveyed"
  - "Domain mappings (5)"
  - "Activation provenance"
  - "Constraint-escape path"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT block**

Append at end:

```markdown

## SIGNAL OUTPUT

Emits: domain_mappings_digest (5–15 lines)

Schema:
```
key_findings:     [5 cross-domain mappings; each one-line description]
named_entities:   [source domain, target domain, blend operator per mapping]
confidence_flags: [H | M | L per mapping, reflecting fitness]
signal_flags:     []
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N2.md
git commit -m "feat: add N2.md module (constraint escape, lateral thinking)"
```

### Task 4c: N3.md — single-digest with W2 literal requirement

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N3.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S3-peripheral-exploration.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N3.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N3
node_type: SPREADING
hat: "spreading-activation+elegance-rubric"
exec_type: inline
scale_gates: [STANDARD, DEEP]
kb_files:
  - spreading-activation.md
  - elegance-rubric.md
context_budget_lines: 1000
input_ports:
  - from: N1
    signal_field: primitives_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N6
    signal_field: activation_digest
    type: required
  - to: N7
    signal_field: activation_digest
    type: optional
  - to: N3.1
    signal_field: activation_digest
    type: back-edge
    gate: S3_thin_or_empty
required_output_sections:
  - "Activation map"
  - "Convergent nodes"
  - "Selected illuminations"
  - "Verification"
raises_signals:
  - S3_thin_or_empty
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT with W2 literal requirement**

Append at end:

```markdown

## SIGNAL OUTPUT

Emits: activation_digest (5–15 lines)

Schema:
```
key_findings:     [3–7 selected illuminations from spreading activation]
named_entities:   [convergent nodes: list 3+ proper-noun anchors]
confidence_flags: [H | M | L per illumination]
signal_flags:     [include "S3_thin_or_empty" if activation map is sparse, < 3 convergent nodes, or all confidence_flags = L]
```

**Required literal (W2):** the activation_digest output must contain the exact string:
```
activation map found: [nodes]; top convergent: [X,Y,Z]; activation strength: [H/M/L]
```
where [nodes] is the comma-separated activation map, [X,Y,Z] is the top three convergent nodes, and [H/M/L] is the dominant activation strength. This literal is enforced by orchestrator STEP 3c digest validation and re-checked by test-runner.sh at STEP 5.4.
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N3.md
git commit -m "feat: add N3.md module (peripheral exploration, W2 literal required)"
```

### Task 4d: N3.1.md — defixation conditional

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N3.1.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S3-1-defixation.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N3.1.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N3.1
node_type: DEFIXATION
hat: "Ohlsson"
exec_type: inline
scale_gates: [STANDARD, DEEP]
kb_files:
  - ohlsson-defixation.md
context_budget_lines: 700
input_ports:
  - from: N3
    signal_field: activation_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N6
    signal_field: breakthrough_digest
    type: optional
  - to: N7
    signal_field: breakthrough_digest
    type: optional
required_output_sections:
  - "Constraint relaxations"
  - "Chunk decompositions"
  - "Re-encodings"
  - "Breakthrough candidate"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT**

Append:

```markdown

## SIGNAL OUTPUT

Emits: breakthrough_digest (5–15 lines)

Schema:
```
key_findings:     [constraint relaxations applied; chunk decompositions; re-encodings; breakthrough candidate]
named_entities:   [original constraint, relaxed constraint, breakthrough name]
confidence_flags: [H | M | L per relaxation/decomposition]
signal_flags:     []
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N3.1.md
git commit -m "feat: add N3.1.md module (defixation, conditional repair)"
```

### Task 4e: N4.md — simulation, DEEP only

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N4.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S4-dynamic-simulation.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N4.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N4
node_type: SIMULATION
hat: "Tesla+observer-frames"
exec_type: spawn
scale_gates: [DEEP]
kb_files:
  - observer-frames.md
  - simulation-checklist.md
context_budget_lines: 1200
input_ports:
  - from: N1
    signal_field: system_model_seed
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N7
    signal_field: simulation_digest
    type: optional
required_output_sections:
  - "System model"
  - "Observer frame analyses"
  - "Imbalances and singularities"
  - "Limit cases"
  - "Theory collisions"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT**

Append:

```markdown

## SIGNAL OUTPUT

Emits: simulation_digest (5–15 lines)

Schema:
```
key_findings:     [observer frame analyses; imbalances; singularities; limit cases; theory collisions]
named_entities:   [observer frames named; collision parties named]
confidence_flags: [H | M | L per finding]
signal_flags:     []
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N4.md
git commit -m "feat: add N4.md module (dynamic simulation, DEEP only)"
```

### Task 4f: N5.md — precision forcing

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N5.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S5-precision-forcing.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N5.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N5
node_type: PRECISION
hat: "Feynman"
exec_type: inline
scale_gates: [MINIMAL, STANDARD, DEEP]
kb_files:
  - vocabulary-rubric.md
  - forward-chain-template.md
context_budget_lines: 1100
input_ports:
  - from: N1
    signal_field: irreducibles_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N7
    signal_field: precision_digest
    type: required
required_output_sections:
  - "5-level vocabulary strip"
  - "3-representation rendering"
  - "Constructive specification"
  - "Flagged vague claims"
  - "Forward-chain consequence tree"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT**

Append:

```markdown

## SIGNAL OUTPUT

Emits: precision_digest (5–15 lines)

Schema:
```
key_findings:     [5-level vocabulary strip results; constructive specification core; vague claims flagged]
named_entities:   [3 representations enumerated; key terms vocabulary-stripped]
confidence_flags: [H | M | L per finding]
signal_flags:     []
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N5.md
git commit -m "feat: add N5.md module (precision forcing, Feynman)"
```

### Task 4g: N6.md — adversarial falsification

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N6.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S6-falsification.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N6.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N6
node_type: ADVERSARIAL
hat: "Popper+Millikan"
exec_type: spawn
scale_gates: [STANDARD, DEEP]
kb_files:
  - tot-templates.md
  - falsification-checklists.md
context_budget_lines: 1500
input_ports:
  - from: N3
    signal_field: activation_digest
    required: true
  - from: N3.1
    signal_field: breakthrough_digest
    required: false
  - from: N1
    signal_field: irreducibles_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N8
    signal_field: falsification_result
    type: required
required_output_sections:
  - "Abductive mode results"
  - "Deductive mode results"
  - "Inductive mode results"
  - "Consolidated hypotheses"
  - "Extreme scenarios"
  - "Millikan assessment"
  - "Density report"
raises_signals:
  - S6_no_alternatives
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT**

Append:

```markdown

## SIGNAL OUTPUT

Emits: falsification_result (5–15 lines)

Schema:
```
key_findings:     [consolidated hypotheses; extreme scenarios; Millikan assessment summary; density report]
named_entities:   [hypotheses named; falsifier candidates]
confidence_flags: [H | M | L per hypothesis]
signal_flags:     [include "S6_no_alternatives" if no viable alternative hypothesis survives the abductive/deductive/inductive mode results]
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N6.md
git commit -m "feat: add N6.md module (adversarial falsification, Popper+Millikan)"
```

### Task 4h: N6.1.md — conjecture (CONJECTURE_FLAG only)

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N6.1.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S6-1-conjecture.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N6.1.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N6.1
node_type: CONJECTURE
hat: "Ramanujan"
exec_type: inline
scale_gates: [CONJECTURE_FLAG]
kb_files:
  - pattern-taxonomy.md
context_budget_lines: 900
input_ports:
  - from: N1
    signal_field: primitives_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N7
    signal_field: conjecture_digest
    type: optional
required_output_sections:
  - "Detected patterns"
  - "Conjectures (≥3)"
  - "Verification instances"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT**

Append:

```markdown

## SIGNAL OUTPUT

Emits: conjecture_digest (5–15 lines)

Schema:
```
key_findings:     [≥3 conjectures; pattern-taxonomy classifications]
named_entities:   [pattern names; conjecture names]
confidence_flags: [H | M | L per conjecture, based on verification instances]
signal_flags:     []
```
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N6.1.md
git commit -m "feat: add N6.1.md module (conjecture, Ramanujan, --conjecture flag)"
```

### Task 4i: N7.md — aggregation with V1-V7 battery

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N7.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/S7-integration-verification.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N7.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N7
node_type: AGGREGATION
hat: "Feynman+Boden"
exec_type: spawn
scale_gates: [MINIMAL, STANDARD, DEEP]
kb_files:
  - verification-gates.md
  - boden-types.md
  - scope-template.md
  - representation-frames.md
  - elegance-rubric.md
context_budget_lines: 1500
input_ports:
  - from: N5
    signal_field: precision_digest
    required: true
  - from: N8
    signal_field: falsification_digest
    required: true
    type: gate-open                # required-when-active per spec §4.5
  - from: N2
    signal_field: domain_mappings_digest
    required: false
  - from: N3
    signal_field: activation_digest
    required: false
  - from: N3.1
    signal_field: breakthrough_digest
    required: false
  - from: N4
    signal_field: simulation_digest
    required: false
  - from: N6.1
    signal_field: conjecture_digest
    required: false
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: N9
    signal_field: integration_digest
    type: required
required_output_sections:
  - "Convergence signals"
  - "Primary conclusion"
  - "Confidence band"
  - "Creativity type"
  - "Generalization check"
  - "Verification report (V1-V7)"
raises_signals: []
---

```

- [ ] **Step 3: Append SIGNAL OUTPUT** (note: N7 also writes the auxiliary `N7-v6-scope.txt` file per epiphany-genius pattern)

Append:

```markdown

## SIGNAL OUTPUT

Emits: integration_digest (5–15 lines)

Schema:
```
key_findings:     [primary conclusion; convergence signals; key V1–V7 outcomes]
named_entities:   [conclusion entities; falsifiers; representations agreeing]
confidence_flags: [H | M | L summarizing battery]
signal_flags:     []
```

## AUXILIARY OUTPUT (parity with epiphany-genius S7)

In addition to the main stage file (`{session_dir}/stages/N7-integration.md`),
N7 must write `{session_dir}/stages/N7-v6-scope.txt` containing the V6 scope
verbatim carve-out. This file is required by N9 (OSP) and by test-runner.sh
T3 check. The carve-out is the exact scope statement language from V4 +
V6 limit-case probes, byte-for-byte; it appears in Section 8 of N9's
distilled output verbatim.
```

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N7.md
git commit -m "feat: add N7.md module (aggregation + V1-V7 battery + V6 scope file)"
```

### Task 4j: N9.md — output synthesis (OSP)

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N9.md`

- [ ] **Step 1: Copy source**

```bash
cp ~/.claude/skills/epiphany-genius/modules/output-synthesis-pass.md \
   ~/.claude/skills/epiphany-graph-genius/modules/N9.md
```

- [ ] **Step 2: Prepend frontmatter**

Insert at top:

```yaml
---
node_id: N9
node_type: SYNTHESIS
hat: "Feynman+Boden(synthesizer)"
exec_type: spawn
scale_gates: [MINIMAL, STANDARD, DEEP]
enabled_when: "NOT --xml"
kb_files: []
context_budget_lines: 2000
input_ports:
  - from: N7
    signal_field: integration_digest
    required: true
  - from: input
    signal_field: "—"
    required: true
output_ports:
  - to: output
    signal_field: "—"
    type: terminal
required_output_sections:
  - "Pipeline Status Header"
  - "Headline Insight"
  - "Theory Collisions"
  - "Discovery vs. Proof"
  - "Independence-Verified Bridges"
  - "Alternative Hypotheses"
  - "Density-Checked Falsification"
  - "Scope Limits"
  - "Coherence Signals"
  - "Generalization Checks"
  - "Open Questions & Next Probes"
input_dependencies:
  - "{session_dir}/stages/N1-state-loading.md"
  - "{session_dir}/stages/N5-precision-forcing.md"
  - "{session_dir}/stages/N7-integration.md"
  - "{session_dir}/stages/N7-v6-scope.txt"
  - "{session_dir}/input.md"
raises_signals: []
---

```

- [ ] **Step 3: Update body — replace S* references with N* throughout**

```bash
sed -i 's/S1-state-loading.md/N1-state-loading.md/g; \
        s/S5-precision-forcing.md/N5-precision-forcing.md/g; \
        s/S7-integration.md/N7-integration.md/g; \
        s/S7-v6-scope.txt/N7-v6-scope.txt/g; \
        s/output-distilled.md/N9-output-distilled.md/g' \
   ~/.claude/skills/epiphany-graph-genius/modules/N9.md
```

- [ ] **Step 4: Append a brief SIGNAL OUTPUT note (terminal node — no consumers but parser expects the block)**

Append at end:

```markdown

## SIGNAL OUTPUT

N9 is the terminal node. It emits no digest consumed by other nodes; its
output is the distilled report.md (or report.xml via xml-assemble.sh in
--xml mode, in which case N9 is skipped per `enabled_when: "NOT --xml"`).

For parser consistency, N9 may include a single trivial SIGNAL OUTPUT
block at the end of N9-output-distilled.md:

```
key_findings:     [synthesis complete]
named_entities:   []
confidence_flags: [H]
signal_flags:     []
```

This block is parsed by --resume but is not consumed by any downstream
node.
```

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N9.md
git commit -m "feat: add N9.md module (output synthesis pass, --xml-gated)"
```

### Task 4k: N8.md — NEW router module (authored fresh)

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/modules/N8.md` (NEW — no source module)

- [ ] **Step 1: Create N8.md from scratch with frontmatter + protocol + signal output**

Create `~/.claude/skills/epiphany-graph-genius/modules/N8.md` with the following content:

```markdown
---
node_id: N8
node_type: ROUTER
hat: null
exec_type: inline
scale_gates: [STANDARD, DEEP]
kb_files: []
context_budget_lines: 50
input_ports:
  - from: N6
    signal_field: falsification_result
    required: true
output_ports:
  - to: N3.1
    signal_field: "gate:S6_no_alternatives AND NOT N3.1_ran"
    type: back-edge
    gate: "S6_no_alternatives"
  - to: N7
    signal_field: falsification_digest
    type: gate-open
required_output_sections:
  - "resolved_gate"
  - "falsification_digest"
  - "signal_flags_checked"
  - "decision_rationale"
raises_signals: []
---

# N8 — Router (NEW module, authored fresh)

N8 is a deterministic signal router. Unlike cognitive nodes (N1–N7, N9), N8 performs no analysis, reads no KB files, and applies no domain expertise. It executes a 6-step procedure to evaluate which of two outgoing edges should fire based on N6's `falsification_result` digest and the running record of `executed_nodes` in `session.md`.

## PROTOCOL

When the orchestrator's STEP 3a inline-to-fixpoint phase determines that N8 is in READY_SET (because edge 14 N6→N8 required is satisfied), execute these steps in order:

1. **Read N6's falsification_result.** Locate `SIGNAL_STATE[(N6, falsification_result)]` (held in the orchestrator's working memory). Bind to local variable `FR`.

2. **Read executed_nodes.** From `{session_dir}/stages/session.md`, parse the `executed_nodes` field (an append-only list of node_ids that have completed). Bind to local variable `EXE`.

3. **Evaluate back-edge condition (edge 15).** Compute:

   ```
   back_edge_fires = ("S6_no_alternatives" ∈ FR.signal_flags) AND ("N3.1" ∉ EXE)
   ```

4. **Write SIGNAL_STATE passthrough (UNCONDITIONAL).** Set:

   ```
   SIGNAL_STATE[(N8, falsification_digest)] = FR
   ```

   This activates edge 16 (N8→N7 gate-open) regardless of whether the back-edge fires. The gate-open edge is required-when-active per spec §4.5; it always resolves once N8 has executed. (B3 fix.)

5. **Conditional back-edge action.** If `back_edge_fires`:
   - Sub-case A: If "N3.1" ∈ EXE (defensive check; should be false here per step 3): annotate `session.md.repair_cap_hit = true`. Do NOT enqueue N3.1.
   - Sub-case B: Else (the normal case when back-edge fires): enqueue N3.1 to READY_SET. Edge 10 (N3.1→N6 optional) is DEAD-LETTER in this case because N6 has already executed and PRC2 forbids overwrite of `SIGNAL_STATE[(N6, falsification_result)]`. N3.1's breakthrough_digest reaches N7 only via edge 11 (N3.1→N7 optional). N6 does NOT re-execute. The V2 falsifier check of the breakthrough is deferred to N7's V1–V7 battery.

6. **Write N8-router-record.md.** Write to `{session_dir}/stages/N8-router-record.md`:

   ```yaml
   ---
   resolved_gate: <"back-edge-fired" | "gate-open-only">
   falsification_digest: |
     <verbatim contents of FR — the passthrough digest, 5–15 lines>
   signal_flags_checked: [list of FR.signal_flags]
   decision_rationale: <one-line explanation, e.g., "S6_no_alternatives raised AND N3.1 not yet run; back-edge fired" OR "S6_no_alternatives not raised; gate-open only">
   ---
   ```

## EDGE-CASE NOTES

- **Repair cap (EC4):** Per spec EC4, if `back_edge_fires` would compute true but "N3.1" is already in `EXE` (i.e., the early back-edge from N3 already fired N3.1), the late back-edge from N8 is suppressed and `repair_cap_hit = true` is annotated. The gate-open edge to N7 still fires (B3 fix).

- **--resume:** On --resume re-entry, if N8's stage file already exists and validates, the rehydrated SIGNAL_STATE includes `(N8, falsification_digest)`. N3.1 status is read from rehydrated `executed_nodes`. No re-execution.

## SIGNAL OUTPUT

Emits: falsification_digest (5–15 lines, passthrough of N6's falsification_result)

Schema:
```
key_findings:     [verbatim from N6's falsification_result.key_findings]
named_entities:   [verbatim from N6's falsification_result.named_entities]
confidence_flags: [verbatim from N6's falsification_result.confidence_flags]
signal_flags:     [verbatim from N6's falsification_result.signal_flags — typically including "S6_no_alternatives" if N6 raised it]
```

The digest is a structural alias of `falsification_result` per spec §5.2 footnote: "Edge N8→N7 references signal_field `falsification_digest`, which is a passthrough alias for falsification_result emitted verbatim by N8".
```

- [ ] **Step 2: Verify file is well-formed (frontmatter + body + signal output)**

```bash
head -30 ~/.claude/skills/epiphany-graph-genius/modules/N8.md
grep -c "^## " ~/.claude/skills/epiphany-graph-genius/modules/N8.md
```
Expected: frontmatter visible, multiple `## ` headers (PROTOCOL, EDGE-CASE NOTES, SIGNAL OUTPUT).

- [ ] **Step 3: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add modules/N8.md
git commit -m "feat: add N8.md module (NEW router, authored fresh from spec §7.4)"
```

---

## Task 5: SKILL.md orchestrator

This is the largest deliverable. SKILL.md will be roughly 600–700 lines. It is broken into per-STEP sub-tasks below.

**Files:**
- Create: `~/.claude/skills/epiphany-graph-genius/SKILL.md`

### Task 5a: SKILL.md frontmatter + ARCHITECTURE + CONFIGURATION

- [ ] **Step 1: Create SKILL.md with frontmatter (per spec §2.5)**

Create `~/.claude/skills/epiphany-graph-genius/SKILL.md`:

```markdown
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

# epiphany-graph-genius v1.0.0 — Orchestrator

You are the **orchestrator** for the `epiphany-graph-genius` skill. You execute a Graph-of-Thought cognitive pipeline declared in `graph.json`. Some nodes run **inline** in your own context (role-switched blocks); other heavy nodes run as **subagent spawns** via the `Agent` tool.

Your job: parse flags → load graph.json + run PRC1 → init session → run the ready-set execution loop with inline-to-fixpoint then parallel spawn fire → handle terminal logic (--xml branch or N9 spawn) → run post-run validation and test battery → emit summary line. Follow STEP 0–6 below exactly.

## ARCHITECTURE

- **This file (SKILL.md):** orchestrator. You are the main agent.
- **`graph.json`:** node + edge registry. The single source of truth for topology, scale gates, exec types, and required output sections.
- **`modules/N*.md`:** per-node protocols. INLINE nodes are read by you and executed in your own context (role-switched). SPAWN nodes are executed by subagents you dispatch via the `Agent` tool.
- **`scripts/*.sh`:** shell helpers (session init, validation, XML assembly, test battery).
- **`kb/`:** 18 operational KB files. Read by inline nodes (in your context) or spawn nodes (in subagent contexts).

**Three-layer rule:** You never modify graph.json mid-run. Inline nodes follow their module's PROTOCOL strictly during a role-switched block. Spawn nodes hold no orchestrator state — they receive their predecessor digests as inlined prompt text and the relevant on-disk file paths.

## CONFIGURATION

| Variable | Default | Purpose |
|----------|---------|---------|
| `{skill_path}` | `~/.claude/skills/epiphany-graph-genius/` | Skill install location |
| `{kb_base}` | `~/.claude/skills/epiphany-graph-genius/kb/` | KB location (derived) |
| `{graph_file}` | `~/.claude/skills/epiphany-graph-genius/graph.json` | Graph registry |
| `{session_output_base}` | `~/docs/epiphany/graph-genius/` | Where session folders are written |

All script calls and KB paths below substitute these variables. Shell scripts derive their own location internally via `SCRIPT_DIR`.

---
```

- [ ] **Step 2: Commit frontmatter + architecture**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add SKILL.md
git commit -m "feat: SKILL.md frontmatter + architecture overview"
```

### Task 5b: SKILL.md STEP 0 — Flag resolution + graph load + PRC1

- [ ] **Step 1: Append STEP 0 to SKILL.md**

Append to `~/.claude/skills/epiphany-graph-genius/SKILL.md`:

```markdown
## STEP 0 — FLAG RESOLUTION + GRAPH LOAD + PRC1

### 0.1 Parse flags

Accepted flags (per spec C4): `--minimal`, `--standard`, `--deep`, `--conjecture`, `--xml`, `--quiet`, `--verbose`, `--no-save`, `--resume <session_dir>`.

Determine **scale**:
- explicit `--minimal` / `--standard` / `--deep` → use it.
- `--conjecture` alone (no scale flag) → MINIMAL + CONJECTURE_FLAG.
- no flags → STANDARD.
- `--conjecture` + explicit scale → that scale + CONJECTURE_FLAG.

Determine **modifiers**: which of `--xml`, `--quiet`, `--verbose`, `--no-save`, `--resume` are set.

#### Invalid combinations (HALT with explicit error):
- Two or more of {`--minimal`, `--standard`, `--deep`} set simultaneously.
- Unknown flag (not in the accepted set).
- `--resume` without a `<session_dir>` positional argument.
- `--resume` with scale flag mismatch vs original session's scale (compare to `session.md.scale`).
- `--resume` with `--conjecture` mismatch (original used it but resume omits, or vice versa).

On HALT: print `FAILED: <reason>` to stderr, exit non-zero.

#### --verbose behavior
Emit a one-line progress annotation per node as it starts (inline or spawn) and completes. Also log the Agent() spawn prompts to `session.md.verbose_trace[]` for post-hoc inspection. Default (no --verbose): silent per-node progress; only summary line + terminal line printed at end.

#### --quiet behavior
Suppresses no output that is required (summary line at STEP 6.2 always prints). Saves silently — the "Saved to ..." line at STEP 6.3 still prints, matching epiphany-genius STEP 8 precedent.

#### --resume syntax
`/epiphany-graph-genius --resume <session_dir>` where `<session_dir>` is a directory like `~/docs/epiphany/graph-genius/2026-04-24-abc123/`. Validate that `<session_dir>/stages/session.md` exists; HALT if missing.

### 0.2 Load graph.json

Read `{graph_file}` (`{skill_path}graph.json`). Apply the **scale-gate filter** to nodes and edges:

A node `n` is ACTIVE iff `(scale ∈ n.scale_gates) OR ("CONJECTURE_FLAG" ∈ n.scale_gates AND --conjecture is set)`.

An edge `e` is ACTIVE iff:
1. `e.source` node is active (or source = "input").
2. `e.target` node is active (or target = "output").
3. `e.scale_gates` includes current scale OR `("CONJECTURE_FLAG" ∈ e.scale_gates AND --conjecture is set)`.

Filter inactive nodes and edges out of the working topology.

### 0.3 PRC1 validation (inline)

Verify on the active topology:

1. **DAG check:** excluding back-edges (edge 9 N3→N3.1 and edge 15 N8→N3.1), the active subgraph contains no cycles. Run a topological sort; it must visit every active node.

2. **Edge resolution:** every active edge's `source` and `target` resolve to a declared active node id (or `"input"` / `"output"` sentinels).

3. **Signal field validity:** every active edge's `signal_field` is one of the 11 declared digest names (per spec §5.2) or a gate literal (begins with `"gate:"`) or `"—"`.

4. **Connectivity:** the active subgraph (back-edges INCLUDED for connectivity) is connected with N1 as source and N9 (or `"output"` in --xml mode) as sink.

If any check fails: HALT with explicit error. Do not proceed to STEP 1.
```

- [ ] **Step 2: Commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git add SKILL.md
git commit -m "feat: SKILL.md STEP 0 (flag resolution + graph load + PRC1)"
```

### Task 5c: SKILL.md STEP 1 — Session init + input

- [ ] **Step 1: Append STEP 1**

Append:

```markdown
---

## STEP 1 — SESSION INIT + INPUT PROCESSING

### 1.1 Invoke session-init.sh

Run:
```bash
RESOLVED_DIR=$(bash {skill_path}scripts/session-init.sh {session_dir} | head -n1)
```

If --resume: pass the session_dir from the --resume argument; the script verifies it exists and prints the canonical path.

If new session: pass `{session_output_base}` (no specific session_id); the script generates a session_id and creates the directory.

Use `RESOLVED_DIR` as the authoritative `{session_dir}` for all subsequent steps.

### 1.2 Input capture and normalization

If new session (not --resume):

1. **Read raw user input** from the command argument or stdin.

2. **Write verbatim to `{session_dir}/input.md`.** This is the audit archive of the user's literal submission. Never overwritten; never skipped (not even under --no-save).

3. **Apply light normalization:** strip leading/trailing whitespace; Unicode NFC normalize; preserve internal structure. Write the normalized content to `{session_dir}/stages/00-processed-input.md`. This is the file nodes read during execution.

4. **Distinction between the two files:** `input.md` = user's literal submission (auditable); `00-processed-input.md` = pipeline's working copy (can diverge from input.md in v2 if preprocessing is added). v1 normalization is near-identity.

If --resume: skip; both files already exist.

### 1.3 Initialize session.md

Write `{session_dir}/stages/session.md` with these fields (preserving any fields already written by session-init.sh):

```yaml
session_id: <from session-init.sh>
scale: <MINIMAL | STANDARD | DEEP>
modifiers: [list of active flag names]
wall_seconds_start: <epoch seconds>
spawns_total: 0
executed_nodes: []
abort_reason: null
spawn_budget_resolution:
  option: A
  rationale: "N4 simulation quality preserved; ≤45-min wall-clock is user metric"
  chosen_at_phase: design
wave_demotions: []
repair_cap_hit: false
warnings: []
verbose_trace: []
```

If --resume: do NOT overwrite session.md; read existing fields and continue from there.

---
```

- [ ] **Step 2: Commit**

```bash
git add SKILL.md
git commit -m "feat: SKILL.md STEP 1 (session init + input processing)"
```

### Task 5d: SKILL.md STEP 2 — READY_SET init + --resume rehydration

- [ ] **Step 1: Append STEP 2**

Append:

```markdown
## STEP 2 — READY_SET INIT

### 2.1 Initial READY_SET

`READY_SET = {N1}` — N1 is the only node with no required incoming edges (other than from "input").

`SIGNAL_STATE = {}` — empty in-memory map keyed by `(node_id, signal_field)` tuples.

### 2.2 --resume rehydration

If `--resume` is set:

a. Scan `{session_dir}/stages/` for `N*-*.md` files.

b. For each candidate file: invoke `scripts/validate-node.sh {session_dir} <node_id>`. Only files that PASS validation proceed to rehydration. Invalid or partial files are treated as not-yet-executed (their node will re-execute on this run).

c. For each VALID file: parse its SIGNAL OUTPUT block(s) per the grammar in spec §7.3bis. Populate `SIGNAL_STATE[(node_id, signal_field)] = digest_text` per each output_port the node declared. For multi-digest nodes (N1), populate three entries from one file.

d. Append validated `node_id`s to `session.md.executed_nodes`.

e. **PRC2 re-entry rule:** nodes in `executed_nodes` are SKIPPED during STEP 3 (they never re-execute). SIGNAL_STATE keys populated in 2.2c are pre-loads; STEP 3 never overwrites them. Append-only invariant holds across the resumed run.

f. Run STEP 3e (ready-set recompute) once to seed READY_SET with nodes whose required incoming edges are already satisfied via rehydrated SIGNAL_STATE.

If not --resume: skip 2.2 entirely; READY_SET stays `{N1}`.

---
```

- [ ] **Step 2: Commit**

```bash
git add SKILL.md
git commit -m "feat: SKILL.md STEP 2 (READY_SET init + --resume rehydration)"
```

### Task 5e: SKILL.md STEP 3 — Execution loop (the big one)

- [ ] **Step 1: Append STEP 3 — termination + spawn-failure preamble**

Append:

```markdown
## STEP 3 — READY-SET EXECUTION LOOP

### Termination / failure HALT (audit M3 — clarified two-case logic)

At the top of each iteration of the outer `while READY_SET ≠ ∅` loop, evaluate the HALT condition:

```
HALT condition (per scale + flag combination):
  IF NOT --xml: HALT iff (READY_SET is empty AND "N9" ∉ executed_nodes)
  IF --xml:    HALT iff (READY_SET is empty AND xml-assemble.sh has NOT
              yet been invoked, i.e., we have not yet reached STEP 4.1)
```

If HALT triggers:
- This indicates a cascade failure (e.g., N1 produced no digests, blocking all downstream nodes).
- Write to session.md:
  - `abort_reason`: "READY_SET empty before terminal; missing digests: <list of (node_id, signal_field) expected but absent from SIGNAL_STATE>"
  - `wall_seconds`: now − wall_seconds_start
- Print to stderr: `FAILED: epiphany-graph-genius aborted — see {session_dir}stages/session.md`
- Do NOT emit the normal summary line.
- Exit non-zero.

If NOT HALT (READY_SET is empty AND we've reached the terminal naturally): exit the while loop and proceed to STEP 4.

### Spawn failure handling

If any `Agent()` spawn returns with an error (tool error, timeout, unparseable output, exception):

1. Log to `session.md.warnings[]` with `{node_id, error_message}`.
2. Do NOT write SIGNAL_STATE for the failed node.
3. Do NOT add the failed node to `executed_nodes`.
4. Block dependents: dependents fail their ready-set required-edge check, naturally cascading.
5. Continue the ready-set loop with remaining nodes.
6. If the failure cascade empties READY_SET before N9 completes, the termination HALT rule triggers.

A failed N2 (optional input to N7) logs a warning but doesn't abort. A failed N5 (required input to N7) cascades to abort. --resume can retry after fixes.
```

- [ ] **Step 2: Append STEP 3 main loop**

Append:

```markdown
### Main loop: While READY_SET ≠ ∅

Each iteration of the outer `while` loop is a "wave." Within each wave: run all currently-eligible inline nodes to fixpoint (3a), then compute SPAWN_NODES (3b), then fire spawns in parallel (3d). Recompute READY_SET (3e). Check O8 budget (3f).

#### 3a. Inline-to-fixpoint phase

```
REPEAT:
  INLINE_NODES = { n ∈ READY_SET | n.exec_type == "inline" AND n ∉ executed_nodes }
  IF INLINE_NODES is empty: BREAK (fixpoint reached for this wave).
  FOR EACH n ∈ INLINE_NODES (sequentially):
    Execute n inline per 3c below.
    After completion: run 3e ready-set recompute to surface newly-eligible nodes
    (which may include further inline nodes — e.g., N3.1 enqueued by N3 back-edge,
    or N8 newly eligible after N6 returns from a prior wave's spawn,
    or N6.1 newly eligible after N1).
END REPEAT.
```

The fixpoint exhausts all inline work before computing SPAWN_NODES. This lets newly-eligible spawn nodes (e.g., N6 after N3 inline writes activation_digest) join the current wave's spawn fire.

#### 3b. Compute SPAWN_NODES (after fixpoint)

```
SPAWN_NODES = { n ∈ READY_SET | n.exec_type == "spawn" AND n ∉ executed_nodes }
```

#### 3c. Per-inline-node execution (called from 3a)

For each inline node `n`:

1. **Emit role-switched preamble** (in your own context):

   ```
   You are now executing {n.id} as {n.hat}. Read modules/{n.id}.md
   and its kb_files. The raw input is at
   {session_dir}/stages/00-processed-input.md.
   Execute the module's PROTOCOL. Do not reason about orchestrator
   state during this block.
   ```

2. **Read** `modules/{n.id}.md` and each file in `n.kb_files` (paths relative to `{kb_base}`).

3. **Execute the module's PROTOCOL.** Apply its cognitive operations to the raw input + predecessor digests in SIGNAL_STATE.

4. **Write full output** to `{session_dir}/{n.output_file}` (per graph.json `output_file` field, which already includes the `stages/` prefix — e.g., `"stages/N1-state-loading.md"`). Do NOT prepend `stages/` again. (Audit A2 fix.)

5. **Extract SIGNAL OUTPUT block(s)** per the parsing grammar in spec §7.3bis. Validate against `signal_digest_schema`:
   - Required fields present: `key_findings`, `named_entities`, `confidence_flags`, `signal_flags`.
   - Line count inside code-block fences: 5–15 inclusive.
   - For N3: W2 literal string present.
   
   Multi-digest nodes (N1) have 3 SIGNAL OUTPUT subsections; parse each.

6. **On validation failure (EC7):** log warning to `session.md.warnings[]` with `{node_id, signal_field, parse_error}`. Mark digest as MISSING in SIGNAL_STATE (do not write). Block dependents naturally. Continue.

7. **On success:** for each output_port `p`, write `SIGNAL_STATE[(n.id, p.signal_field)] = digest_text`. PRC2: never overwrite an existing key — abort if attempted.

8. **Append `n.id`** to `session.md.executed_nodes`.

9. **POST-EXECUTION BACK-EDGE CHECKS:**
   - **IF n == N3** AND `SIGNAL_STATE[(N3, activation_digest)].signal_flags` includes `"S3_thin_or_empty"` AND `"N3.1" ∉ executed_nodes`: enqueue N3.1 to READY_SET. (Early back-edge case: edge 10 N3.1→N6 is LIVE because N6 has not yet executed. N6 will receive both activation_digest and breakthrough_digest as inputs when it later fires.)
   - **IF n == N8:** N8's protocol handles routing. Per N8.md PROTOCOL, N8 writes `SIGNAL_STATE[(N8, falsification_digest)]` UNCONDITIONALLY (gate-open edge 16 always resolves once N8 executes). If `SIGNAL_STATE[(N6, falsification_result)].signal_flags` includes `"S6_no_alternatives"` AND `"N3.1" ∉ executed_nodes`: ALSO enqueue N3.1 to READY_SET (late back-edge case). In this case, edge 10 (N3.1→N6) is DEAD-LETTER because N6 has already executed; breakthrough_digest reaches N7 only via edge 11. N6 does NOT re-execute.
   - **IF n == N8 AND "N3.1" ∈ executed_nodes AND signal_flags includes "S6_no_alternatives":** repair-cap-hit (EC4). Annotate `session.md.repair_cap_hit = true`. Do not enqueue N3.1. Gate-open to N7 still fires.

#### 3d. Spawn fire (after inline fixpoint, with recomputed SPAWN_NODES)

If `SPAWN_NODES ≠ ∅`:

1. **Construct spawn prompts** for all nodes in SPAWN_NODES. The prompt template:

   ```
   You are executing {n.id} as {n.hat} for the epiphany-graph-genius pipeline.
   
   Session directory: {session_dir}
   Module file path: modules/{n.id}.md
   Scale: {scale}
   Active modifiers (flags): {modifiers}
   Executed nodes so far: {executed_nodes_list}
   
   Predecessor digests (each inlined verbatim from SIGNAL_STATE):
     {for each required incoming edge, emit:}
       Predecessor {edge.source} digest ({edge.signal_field}):
       <verbatim digest payload from SIGNAL_STATE[(edge.source, edge.signal_field)]>
     {for each optional incoming edge whose source is in executed_nodes:}
       Predecessor {edge.source} digest ({edge.signal_field}) [OPTIONAL]:
       <verbatim digest payload>
   
   {Generic disk-read enumeration (audit A3 fix — replaces N7/N9 special cases):
     For each path in n.input_dependencies:
       Skip "stages/00-processed-input.md" (inline nodes already see it; spawn
       nodes can read it directly if needed without explicit listing).
       IF the file exists at {session_dir}/{path}:
         Emit "Read this on-disk file: {session_dir}/{path}"
       ELSE IF the path corresponds to an optional source (i.e., the source
            node's outgoing edge to this node is type=optional):
         Skip silently.
       ELSE (required input file is missing):
         Log a warning to session.md.warnings — required disk read missing.
     For N9 specifically: also append "You may optionally read any other
     N*-*.md files present in {session_dir}/stages/ for additional synthesis
     context."
   }
   
   Instructions: Execute the PROTOCOL in your module file. Read your kb_files. 
   Write your full output to {session_dir}/{n.output_file} (the output_file
   field already includes the stages/ prefix; do not prepend it again — audit A2 fix).
   Return a SIGNAL OUTPUT block matching your module's declared schema (per the grammar in spec §7.3bis).
   ```

2. **Fire all spawns in parallel.** In a SINGLE message, emit all `Agent()` calls (one per node in SPAWN_NODES). Per O4 maximum-parallel-fire rule.

3. **Wait for all returns.** Each subagent returns its SIGNAL OUTPUT block (and writes its full stage file to disk).

4. **For each return:**
   - Parse the SIGNAL OUTPUT block per spec §7.3bis grammar.
   - Validate against signal_digest_schema (per 3c step 5).
   - On success: write `SIGNAL_STATE[(n.id, p.signal_field)] = digest_text` per each output_port. PRC2 invariant.
   - On failure (EC7): apply the same handling as 3c step 6.
   - Append `n.id` to `session.md.executed_nodes`.

5. **Increment** `session.md.spawns_total` by `len(SPAWN_NODES)`.

#### 3e. READY_SET recompute

For each active node `n` (per scale gate) that is NOT in `executed_nodes`:

- **Back-edge-only-target rule (audit A1 fix — CRITICAL):** if `n`'s set of incoming edges contains ONLY back-edge type entries (no required, no gate-open, no optional incoming edges), `n` is NEVER activated by 3e's standard rule. `n` activates solely via explicit enqueue from per-node post-execution checks (specifically: 3c step 9 IF n == N3 enqueues N3.1; or N8's protocol enqueues N3.1 when its back-edge gate fires). Skip such nodes here. (In the current 11-node graph, N3.1 is the only node with this property.)
- **Required edges:** every `required` incoming edge to `n` must have `SIGNAL_STATE[(edge.source, edge.signal_field)]` present.
- **Gate-open edges:** if `n` has any incoming `gate-open` edge, that edge's `signal_field` must be in SIGNAL_STATE (gate-open is required-when-active per spec §4.5; per the B3 fix, N8→N7 always resolves once N8 has executed).
- **Optional + back-edge incoming edges:** do NOT block activation EXCEPT per the early-back-edge sync rule below.
- **Early back-edge sync rule:** IF `n == N6` AND `SIGNAL_STATE[(N3, activation_digest)].signal_flags` includes `"S3_thin_or_empty"` AND `"N3.1" ∉ executed_nodes`: N6 is BLOCKED (do not add to READY_SET). N6 must wait for N3.1 to complete so its breakthrough_digest is available via edge 10 optional. (Without this sync, N6 would fire parallel to N3.1 and miss the breakthrough.)

If the back-edge-only rule does not exclude `n` AND all required + gate-open checks pass AND the early-back-edge sync rule does not block AND `n ∉ executed_nodes`: add `n` to READY_SET.

Remove just-executed nodes from READY_SET.

#### 3f. O8 budget check (before each wave)

Before the next iteration's 3a phase:

- Compute the sum of `context_budget_lines` across all nodes that would be inline in the next wave.
- If sum > 600 lines: demote the KB-heaviest inline node to spawn (tie-break: lex node_id, per EC2). Record demotion in `session.md.wave_demotions[]` with `{wave_index, demoted_node_id, kb_total_lines}`.

In practice, the maximum inline wave sum is 277 lines (N5=198 + N3.1=79 if early back-edge), well under 600. This rule should never trip in v1, but the mechanism is present for safety.

---
```

- [ ] **Step 2: Commit**

```bash
git add SKILL.md
git commit -m "feat: SKILL.md STEP 3 (ready-set execution loop with C2 inline-to-fixpoint)"
```

### Task 5f: SKILL.md STEP 4-6 — Terminal, post-run, summary

- [ ] **Step 1: Append STEP 4-6**

Append:

```markdown
## STEP 4 — TERMINAL

### 4.1 If --xml mode

- N9 has been SKIPPED throughout (per `enabled_when: "NOT --xml"` on its graph.json entry; ready-set logic never activated it).
- Invoke `scripts/xml-assemble.sh {session_dir}`. Produces `{session_dir}/stages/output.xml`.
- Copy output.xml to session root: `{session_dir}/report.xml = {session_dir}/stages/output.xml`.
- Spawn count for --xml runs is one less across all scales (N9 not fired).

### 4.2 If NOT --xml mode

- N9 was activated by ready-set recompute earlier in STEP 3 and ran as a spawn.
- N9 wrote `{session_dir}/stages/N9-output-distilled.md`.
- Copy to session root: `{session_dir}/report.md = {session_dir}/stages/N9-output-distilled.md`.

---

## STEP 5 — POST-RUN ASSERTIONS + VALIDATION

### 5.1 Per-node validation

For each `n ∈ executed_nodes`: run `scripts/validate-node.sh {session_dir} {n.id}`. Collect pass/fail results in `session.md.warnings[]` (advisory; failures do NOT halt STEP 5).

### 5.2 PRA1 — spawn count assertion

Assert: `session.md.spawns_total ≤ scale_target`, where:
- MINIMAL: ≤ 2
- STANDARD: ≤ 4
- DEEP: ≤ 5
- CONJECTURE alone (which defaults to MINIMAL + CONJECTURE_FLAG): ≤ 2

In --xml mode, spawn counts drop by 1 (N9 not fired); the cap above still satisfied trivially.

On fail: log to `session.md.warnings[]`. Continue.

### 5.3 PRA2 — artifact presence

Assert all required artifacts present:

**Required (always):**
- `{session_dir}/input.md`
- `{session_dir}/stages/00-processed-input.md`
- `{session_dir}/stages/session.md` (with all init schema fields)
- `{session_dir}/stages/validation-log.md` (written by validate-node.sh)
- `{session_dir}/stages/N*-*.md` for each `n ∈ executed_nodes`

**Required (conditional):**
- `{session_dir}/stages/N7-v6-scope.txt` (if N7 executed)
- `{session_dir}/stages/N9-output-distilled.md` (if N9 executed, i.e., NOT --xml)
- `{session_dir}/stages/output.xml` (if --xml mode)
- `{session_dir}/stages/test-report.md` (after STEP 5.4 runs)
- `{session_dir}/report.md` (if NOT --xml AND NOT --no-save)
- `{session_dir}/report.xml` (if --xml AND NOT --no-save)

On fail: log missing-artifact list to `session.md.warnings[]`.

### 5.4 Test battery (T1–T5 via test-runner.sh)

Compute `active_conditionals`: comma-separated list of conditional node_ids in `executed_nodes` (subset of {N3.1, N6.1}). If none: pass literal `"none"`.

Compute `output_mode`: `"xml"` if --xml else `"distilled"`.

Run:
```bash
bash scripts/test-runner.sh {session_dir} {scale} {active_conditionals} {output_mode}
```

The script writes `{session_dir}/stages/test-report.md`. Review in summary.

### 5.5 Wall-time

Write `session.md.wall_seconds = now − wall_seconds_start`.

---

## STEP 6 — SAVE + SUMMARY

### 6.1 Save report

If NOT `--no-save`:

- NOT `--xml`: copy `{session_dir}/stages/N9-output-distilled.md` → `{session_dir}/report.md`.
- `--xml`: copy `{session_dir}/stages/output.xml` → `{session_dir}/report.xml` (already done in 4.1).

`input.md` is unconditional (already in `{session_dir}/`).

### 6.2 Print summary line (always, even under --quiet)

The summary line format (matches epiphany-genius STEP 8 exactly, with `stages` → `nodes` as the only domain adaptation):

```
Cognitive enhancement: [N] nodes executed | Confidence: [band] |
[M] contradictions surfaced | Verification: [pass/fail count] |
Creativity type: [...] | Scope: [limited/broad]
```

**Value extraction (from FULL N7 file `{session_dir}/stages/N7-integration.md`, NOT from the integration_digest):**

| Placeholder | Source |
|---|---|
| `[N] nodes executed` | `len(executed_nodes)` |
| `[band]` | parse "Confidence band:" line from N7-integration.md (one of: high \| medium \| low per Millikan assessment) |
| `[M] contradictions` | parse "Contradictions surfaced:" line from N7-integration.md. By scale: MINIMAL/CONJECTURE-alone = 0 (no N4, no N6); STANDARD = N6 falsification counts; DEEP = N4 theory collisions + N6 counts |
| `[pass/fail count]` | parse "Verification report (V1-V7)" section in N7-integration.md, count "PASS"/"FAIL" markers; emit `<pass>/<total>` (e.g., `6/7`) |
| `[creativity type]` | parse "Creativity type:" from N7-integration.md (combinational \| exploratory \| transformational) |
| `[limited/broad]` | parse Scope section from N7-integration.md; classify per scope-template KB |

### 6.3 Print "Saved to" line

If NOT `--no-save`: print `Saved to {session_dir}report.{md|xml}`. Always printed when save occurred (including under --quiet).

### 6.4 Print terminal line

```
Session: {session_dir}
```

---
```

- [ ] **Step 2: Commit**

```bash
git add SKILL.md
git commit -m "feat: SKILL.md STEP 4-6 (terminal, post-run, summary)"
```

### Task 5g: SKILL.md final structure check

- [ ] **Step 1: Verify SKILL.md is well-formed**

```bash
head -25 ~/.claude/skills/epiphany-graph-genius/SKILL.md
echo "---STEPS---"
grep -n "^## STEP" ~/.claude/skills/epiphany-graph-genius/SKILL.md
echo "---LINE COUNT---"
wc -l ~/.claude/skills/epiphany-graph-genius/SKILL.md
```

Expected:
- frontmatter with `name: epiphany-graph-genius` visible
- All 7 STEP headers (STEP 0 through STEP 6) present
- Total lines roughly 400–700 (informational, not a pass/fail check — audit A5)

- [ ] **Step 2: Final commit if anything was tweaked**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git status
# If any uncommitted changes:
git add SKILL.md
git commit -m "feat: SKILL.md final integrity check"
```

---

## Task 6: Skill registration verification + smoke test

**Files:**
- Test: temporary `/tmp/test-input.md`
- No new files created

- [ ] **Step 1: Verify skill discoverability via filesystem listing**

```bash
ls ~/.claude/skills/epiphany-graph-genius/
ls ~/.claude/skills/epiphany-graph-genius/modules/ | sort
ls ~/.claude/skills/epiphany-graph-genius/kb/ | wc -l
ls ~/.claude/skills/epiphany-graph-genius/scripts/
```

Expected:
- Top-level: `SKILL.md`, `graph.json`, `modules/`, `kb/`, `scripts/` (exactly these 5 entries plus any `.git/`)
- modules/: `N1.md`, `N2.md`, `N3.1.md`, `N3.md`, `N4.md`, `N5.md`, `N6.1.md`, `N6.md`, `N7.md`, `N8.md`, `N9.md` (exactly 11)
- kb/: `18`
- scripts/: `session-init.sh`, `validate-node.sh`, `test-runner.sh`, `xml-assemble.sh` (exactly 4)

- [ ] **Step 2: Verify SKILL.md frontmatter parses (audit M4 — no PyYAML dependency)**

```bash
python3 -c "
import re
with open('/home/myuser/.claude/skills/epiphany-graph-genius/SKILL.md') as f:
    content = f.read()
m = re.match(r'---\n(.*?)\n---', content, re.DOTALL)
if not m:
    print('FAIL: no frontmatter')
    exit(1)
fm_text = m.group(1)
required_keys = ['name:', 'version:', 'description:', 'trigger:', 'skill_path:', 'kb_base:', 'session_output_base:']
missing = [k for k in required_keys if k not in fm_text]
assert not missing, f'missing fields: {missing}'
name_m = re.search(r'^name:\s*(\S+)', fm_text, re.MULTILINE)
version_m = re.search(r'^version:\s*(\S+)', fm_text, re.MULTILINE)
print(f'PASS: frontmatter valid, name={name_m.group(1)}, version={version_m.group(1)}')
"
```
Expected: `PASS: frontmatter valid, name=epiphany-graph-genius, version=1.0.0`

- [ ] **Step 3: Verify graph.json static validation passes (re-run ad-hoc)**

```bash
python3 -c "
import json
from pathlib import Path
with open('/home/myuser/.claude/skills/epiphany-graph-genius/graph.json') as f:
    g = json.load(f)
assert len(g['nodes']) == 11
assert len(g['edges']) == 20
print('PASS: 11 nodes + 20 edges')
"
```
Expected: `PASS: 11 nodes + 20 edges`

- [ ] **Step 4: Verify each module has frontmatter + SIGNAL OUTPUT block**

```bash
for m in ~/.claude/skills/epiphany-graph-genius/modules/N*.md; do
  name=$(basename "$m")
  has_fm=$(head -1 "$m" | grep -c '^---$')
  has_sig=$(grep -c '^## SIGNAL OUTPUT' "$m")
  echo "$name: frontmatter=$has_fm signal_output_blocks=$has_sig"
done
```

Expected: every module reports `frontmatter=1` and `signal_output_blocks=1` (except possibly N1 which has 1 SIGNAL OUTPUT header with 3 sub-sections — also reports 1).

- [ ] **Step 5: Verify scripts are executable and parse**

```bash
for s in ~/.claude/skills/epiphany-graph-genius/scripts/*.sh; do
  if [ -x "$s" ]; then
    bash -n "$s" && echo "PASS: $(basename $s)" || echo "FAIL: $(basename $s)"
  else
    echo "FAIL: $(basename $s) not executable"
  fi
done
```

Expected: 4 lines each `PASS: <script_name>`.

- [ ] **Step 6: Smoke-test session-init.sh against a temp dir**

```bash
TEST_DIR=$(mktemp -d)
TEST_SESSION="$TEST_DIR/test-session/"
RESOLVED=$(bash ~/.claude/skills/epiphany-graph-genius/scripts/session-init.sh "$TEST_SESSION" 2>&1 | head -1)
echo "Resolved: $RESOLVED"
ls -la "$TEST_SESSION/stages/" 2>/dev/null
rm -rf "$TEST_DIR"
```

Expected: `Resolved: ...` shows a session directory; `stages/` directory created with `session.md` inside.

- [ ] **Step 7: Final commit**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git status
git log --oneline | head -20
```
Expected: clean working tree; commit history shows the full task progression.

---

## Task 7: End-to-end integration test

**Files:**
- Test input: temporary file
- No persistent new files

- [ ] **Step 1: Restart Claude Code so the new skill is registered**

```bash
# Manually: exit Claude Code, then restart it
# (No automated step possible — the skill registry is loaded at startup)
```

- [ ] **Step 2: Verify the skill is discoverable**

In a fresh Claude Code session, ask Claude:

```
Is epiphany-graph-genius available as a skill? List its trigger.
```

Expected response: confirms `/epiphany-graph-genius` and `user says "epiphany-graph-genius"` triggers exist.

- [ ] **Step 3: Run a MINIMAL smoke test**

Test prompt to give to Claude:

```
/epiphany-graph-genius --minimal --quiet
What is the relationship between entropy and information in Shannon's formulation?
```

Expected outcome:
- Skill activates.
- Session directory created at `~/docs/epiphany/graph-genius/<session_id>/`.
- Stage files present: `N1-state-loading.md`, `N5-precision-forcing.md`, `N7-integration.md`, `N7-v6-scope.txt`, `N9-output-distilled.md`.
- session.md shows `spawns_total: 2` (only N7 and N9 spawned).
- `report.md` produced at session root.
- Summary line printed in epiphany-genius STEP 8 format.
- Wall-clock target ≤10 minutes per spec §12 (audit M5: this is a target, not a hard pass/fail; record actual `wall_seconds` from session.md and flag if substantially over — e.g., >15 min — for diagnosis but don't block on it).

- [ ] **Step 4: Verify spawn count assertion (PRA1)**

After the smoke test completes:

```bash
SESS=$(ls -dt ~/docs/epiphany/graph-genius/*/ | head -1)
grep "spawns_total" "$SESS/stages/session.md"
```
Expected: `spawns_total: 2`

- [ ] **Step 5: Verify artifact tree (PRA2)**

```bash
SESS=$(ls -dt ~/docs/epiphany/graph-genius/*/ | head -1)
ls "$SESS"
ls "$SESS/stages/"
```
Expected files at session root: `input.md`, `report.md`. Expected files in stages/: `00-processed-input.md`, `N1-state-loading.md`, `N5-precision-forcing.md`, `N7-integration.md`, `N7-v6-scope.txt`, `N9-output-distilled.md`, `session.md`, `validation-log.md`, `test-report.md`.

- [ ] **Step 6: Run a STANDARD smoke test (more substantive)**

Test prompt:

```
/epiphany-graph-genius --standard
What are the cognitive bottlenecks of human working memory and how do they shape learning strategies?
```

Expected outcome:
- All MINIMAL outputs PLUS: `N2-constraint-escape.md`, `N3-peripheral-exploration.md`, `N6-falsification.md`, `N8-router-record.md`.
- session.md shows `spawns_total: 4` (N2, N6, N7, N9).
- Wall-clock target ≤25 minutes per spec §12 (target, not hard pass/fail per audit M5).

- [ ] **Step 7: Verify STANDARD spawn count**

```bash
SESS=$(ls -dt ~/docs/epiphany/graph-genius/*/ | head -1)
grep "spawns_total" "$SESS/stages/session.md"
```
Expected: `spawns_total: 4`

- [ ] **Step 8: Final verification — eyeball comparison vs epiphany-genius (per spec §10.5)**

Optional manual quality check: pick a canonical input you've previously run through epiphany-genius STANDARD. Run the same input through epiphany-graph-genius STANDARD. Read both `report.md` files side-by-side. Assess:
- Is the primary conclusion substantively the same?
- Are V1-V7 gates passing in both?
- Is contradictions count similar (within ±1)?
- Is the writing quality comparable?

If yes → ship. If significantly worse → diagnose (likely B10 sync issue, EC7 malformed digest, or O5 context trim).

- [ ] **Step 9: Tag v1.0.0 release**

```bash
cd ~/.claude/skills/epiphany-graph-genius
git tag -a v1.0.0 -m "epiphany-graph-genius v1.0.0 — initial release per spec 2026-04-24"
```

---

## Self-review

After writing the plan, I checked it against the spec at `/home/myuser/docs/superpowers/specs/2026-04-24-epiphany-graph-genius-design.md`:

**Spec coverage:**
- §2 locked decisions (5): all addressed in tasks (Decision 1 baked into PRA1 in Task 5f; Decision 2 = full copy in Task 1; Decision 3 = required_output_sections in graph.json Task 2; Decision 4 = no IV1 tooling — confirmed absent; Decision 5 = 4 scripts in Task 3).
- §2.5 SKILL.md frontmatter: Task 5a.
- §3 file tree: Tasks 1, 2, 3, 4, 5.
- §4 graph topology (11 nodes, 20 edges, schema, scale gates): Task 2.
- §5 signal digests + W2 literal: covered in module SIGNAL OUTPUT blocks (Tasks 4a-4k) + N3 W2 in Task 4c + parser in spec §7.3bis (referenced from SKILL.md STEP 3c step 5).
- §6 orchestrator loop (STEP 0-6): Tasks 5b-5f.
- §7 module structure: Tasks 4a-4k.
- §8 scripts: Task 3a-3d.
- §9 artifacts: STEP 5.3 in Task 5f.
- §10 verification (PRC1, PRC2, V1-V7, PRA1, PRA2): PRC1 in Task 5b; PRC2 in Task 5e; V1-V7 inside N7 module Task 4i; PRA1/PRA2 in Task 5f.
- §11 edge cases: orchestrator handles them in Task 5e.
- §12 performance targets: validated in Task 7.
- §13 v2 deferrals: not implemented (correct).
- §14 acceptance criteria: implicit checks in Task 6 + Task 7.
- §15 implementation notes: incorporated throughout.

**Placeholder scan:** No "TBD"/"TODO"/"fill in details" — all code blocks are concrete. All sed substitutions show exact strings. All file content is shown literally.

**Type consistency:** Module file names (N1.md, N3.1.md with dot) consistent with stage file names (N1-state-loading.md, N3-1-defixation.md with hyphen) per spec §9. Field names (`exec_type`, `scale_gates`, `required_output_sections`, etc.) consistent across graph.json (Task 2) and module frontmatter (Task 4).

Plan is implementation-ready.

---

## Execution Handoff

**Plan complete and saved to `/home/myuser/docs/superpowers/plans/2026-04-24-epiphany-graph-genius.md`. Two execution options:**

**1. Subagent-Driven (recommended)** — Dispatch a fresh subagent per task, review between tasks, fast iteration. Best for this plan because the tasks are largely mechanical (copy-edit-commit) with clear acceptance criteria, well-suited to one-task-per-subagent pattern.

**2. Inline Execution** — Execute tasks in this session using executing-plans, batch execution with checkpoints. Slightly slower but keeps context within one conversation if you want to make on-the-fly adjustments.

**Which approach?**
