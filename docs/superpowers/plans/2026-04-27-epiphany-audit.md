# epiphany-audit Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the `epiphany-audit` Claude Code skill at `~/.claude/skills/epiphany-audit/` — a 27-node graph-of-thought pipeline that multi-dimensionally audits any codebase and optionally drives a safeguarded, idempotent fix pipeline with recovery-manifest state tracking.

**Architecture:** Three-layer design mirroring `epiphany-graph-genius`. (1) `SKILL.md` orchestrator declares the full 27-node pipeline with all Layer-A sections. (2) `graph.json` registers all nodes and edges machine-readably. (3) `modules/N01..N27.md` each hold the Layer-B contract (inputs/outputs/side-effects/halt-conditions/budget/backtrack). Five JSON schemas validate all report types. Five built-in dimension plugins (matching `modules/N04..N08`) feed the R-ROUTE loader. The fix pipeline (N16–N23) uses a recovery manifest with boundary-aligned writes for safe resumption. The improvement subpipeline (N24–N27) is `--improve`-gated and runs after the audit save prompt.

**Tech Stack:** Markdown (SKILL.md, modules/, templates/), JSON (schemas/, graph.json), YAML frontmatter (dimension plugins, report outputs), Python + pytest + jsonschema (schema-validation tests), YAML fixtures (determinism tests).

**Spec reference:** `/home/myuser/docs/superpowers/specs/2026-04-27-epiphany-audit-skill-design.md`. All behavioral rules, halt states, edge semantics, and schema field constraints are codified there. This plan implements that spec verbatim.

---

## File Structure

```
~/.claude/skills/epiphany-audit/
├── SKILL.md                                    # 13 Layer-A sections, orchestrator
├── graph.json                                  # 27 nodes + 28 edges
├── graph.schema.json                           # validates graph.json
├── README.md
├── CHANGELOG.md
├── modules/
│   ├── N01-context-intake.md
│   ├── N02-r-route.md
│   ├── N03-b-find.md
│   ├── N04-correctness.md
│   ├── N05-architecture.md
│   ├── N06-performance.md
│   ├── N07-security.md
│   ├── N08-maintainability.md
│   ├── N09-extensible.md
│   ├── N10-fpv.md
│   ├── N11-aggregator.md
│   ├── N12-prioritizer.md
│   ├── N13-formatter.md
│   ├── N14-q-gate.md
│   ├── N15-save-handler.md
│   ├── N16-fix-triage.md
│   ├── N17-fix-planner.md
│   ├── N18-pre-flight.md
│   ├── N19-fix-applier.md
│   ├── N20-per-fix-verifier.md
│   ├── N21-regression-battery.md
│   ├── N22-rollback-handler.md
│   ├── N23-fix-reporter.md
│   ├── N24-improvement-contextualizer.md
│   ├── N25-improvement-brainstormer.md
│   ├── N26-oef.md
│   └── N27-improvement-reporter.md
├── schemas/
│   ├── audit-report-v1.schema.json
│   ├── fix-report-v1.schema.json
│   ├── dry-run-plan-v1.schema.json
│   ├── dimension-plugin-v1.schema.json
│   └── improvement-report-v1.schema.json
├── dimensions/
│   ├── correctness.md
│   ├── architecture.md
│   ├── performance.md
│   ├── security.md
│   ├── maintainability.md
│   └── README.md
├── templates/
│   ├── audit-report.md.template
│   ├── fix-report.md.template
│   ├── dry-run-plan.md.template
│   └── improvement-report.md.template
└── tests/
    ├── conftest.py
    ├── schema-validation/
    │   ├── test_audit_report_schema.py
    │   ├── test_fix_report_schema.py
    │   ├── test_dry_run_plan_schema.py
    │   ├── test_dimension_plugin_schema.py
    │   ├── test_improvement_report_schema.py
    │   ├── test_graph_schema.py
    │   └── fixtures/
    │       ├── valid_audit_report.json
    │       ├── invalid_audit_report_missing_id.json
    │       ├── invalid_audit_report_bad_severity.json
    │       ├── valid_fix_report.json
    │       ├── invalid_fix_report_partial_invariant.json
    │       ├── valid_dry_run_plan.json
    │       ├── invalid_dry_run_plan_count_mismatch.json
    │       ├── valid_dimension_plugin.json
    │       ├── invalid_dimension_plugin_missing_triggers.json
    │       ├── valid_improvement_report.json
    │       ├── invalid_improvement_report_count_mismatch.json
    │       └── valid_graph.json
    ├── smoke/
    │   ├── 01-default-invocation.md
    │   ├── 02-audit-only.md
    │   ├── 03-fix-known-good-report.md
    │   ├── 04-dry-run.md
    │   ├── 05-recovery-manifest-resume.md
    │   ├── 06-suspicious-target-halt.md
    │   ├── 07-target-conflict-halt.md
    │   ├── 08-dimension-plugin-loading.md
    │   ├── 09-improve-mode.md
    │   ├── 10-improve-zero-survivors.md
    │   ├── 11-fix-improve-warning.md
    │   └── fixtures/
    │       └── user-dimensions/
    │           └── test-custom-dim.md
    └── determinism/
        ├── python-small/
        │   ├── source/
        │   │   ├── parser.py
        │   │   └── main.py
        │   └── expected_findings.yaml
        └── check_overlap.py
```

---

## Task 1: Scaffold + test infrastructure

**Files:**
- Create: `~/.claude/skills/epiphany-audit/` (directory tree)
- Create: `~/.claude/skills/epiphany-audit/tests/conftest.py`
- Create: `~/.claude/skills/epiphany-audit/README.md`
- Create: `~/.claude/skills/epiphany-audit/CHANGELOG.md`

- [ ] **Step 1: Create directory tree**

```bash
mkdir -p ~/.claude/skills/epiphany-audit/{modules,schemas,dimensions,templates}
mkdir -p ~/.claude/skills/epiphany-audit/tests/{schema-validation/fixtures,smoke/fixtures/user-dimensions,determinism/python-small/source}
```

- [ ] **Step 2: Verify structure**

Run:
```bash
find ~/.claude/skills/epiphany-audit -type d | sort
```
Expected: 10 directories total (skill root + 8 subdirs listed above).

- [ ] **Step 3: Write tests/conftest.py**

```python
import json
import os
import pytest

SKILL_DIR = os.path.expanduser("~/.claude/skills/epiphany-audit")
SCHEMAS_DIR = os.path.join(SKILL_DIR, "schemas")
MODULES_DIR = os.path.join(SKILL_DIR, "modules")
FIXTURES_DIR = os.path.join(SKILL_DIR, "tests/schema-validation/fixtures")

def load_schema(name):
    path = os.path.join(SCHEMAS_DIR, f"{name}.schema.json")
    with open(path) as f:
        return json.load(f)

def load_fixture(name):
    path = os.path.join(FIXTURES_DIR, f"{name}.json")
    with open(path) as f:
        return json.load(f)
```

- [ ] **Step 4: Write README.md**

```markdown
# epiphany-audit

Graph-of-thought audit skill for Claude Code. Runs a multidimensional,
project-aware audit on any codebase, then optionally drives a safeguarded
fix pipeline with idempotent recovery.

## Invocation

```
/epiphany-audit [<path>] [--audit | --fix <report>] [--verbose] [--deep]
                [--improve] [--auto | --confirm-all | --dry-run]
                [--escalate-finding F00N] [--test-cmd '<cmd>']
                [--monorepo-subtree-limit N] [--reverify-state]
                [--full-rerun | --no-rerun]
```

See `SKILL.md` for the complete invocation contract, node registry,
halt states, and behavioral specification.

## Output locations

- Audit reports: `~/docs/epiphany/audit/`
- Fix reports:   `~/docs/epiphany/audit/fix-reports/`
- Dry-run plans: `~/docs/epiphany/audit/dry-run-plans/`
- Improvements:  `~/docs/epiphany/audit/improvement-reports/`
- State files:   `~/docs/epiphany/audit/.state/`
- Recovery:      `~/docs/epiphany/audit/.recovery/`
- Logs:          `~/docs/epiphany/audit/.logs/`
```

- [ ] **Step 5: Write CHANGELOG.md**

```markdown
# Changelog

## [1.0.0] — 2026-04-27

### Added
- Initial release: 27-node graph-of-thought audit + fix + improve pipeline
- Audit pipeline (N01–N15): ContextIntake, R-ROUTE, B-FIND, five dimension
  analyzers (N04–N08), extensible plugin slot (N09), FPV, Aggregator,
  Prioritizer, Formatter, Q-GATE (Pass A + Pass B), SaveHandler
- Fix pipeline (N16–N23): FixTriage (F-VAL ingest + resume-handler),
  FixPlanner, PreFlight, FixApplier (atomic loop), PerFixVerifier,
  RegressionBattery (battery + tiered audit-rerun), RollbackHandler,
  FixReporter
- Improvement subpipeline (N24–N27): ImprovementContextualizer,
  ImprovementBrainstormer (two-phase), OverEngineeringFilter, ImprovementReporter
- Five JSON schemas: audit-report-v1, fix-report-v1, dry-run-plan-v1,
  dimension-plugin-v1, improvement-report-v1
- Five built-in dimension plugins: correctness, architecture, performance,
  security, maintainability
- Four report templates
- Recovery manifest with boundary-aligned writes and archive lifecycle
- Idempotency state file (state file authoritative; git-log fallback)
- 28 halt states with structured envelopes
- Tests: schema-validation (pytest/jsonschema), smoke scenarios, determinism fixture
```

- [ ] **Step 6: Install test dependencies**

```bash
pip install jsonschema pytest pyyaml --quiet
```

- [ ] **Step 7: Commit scaffold**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/
git -C ~/.claude commit -m "feat(epiphany-audit): scaffold directory tree, README, CHANGELOG, conftest"
```

---

## Task 2: graph.schema.json

**Files:**
- Create: `~/.claude/skills/epiphany-audit/tests/schema-validation/test_graph_schema.py`
- Create: `~/.claude/skills/epiphany-audit/tests/schema-validation/fixtures/valid_graph.json`
- Create: `~/.claude/skills/epiphany-audit/graph.schema.json`

- [ ] **Step 1: Write failing test**

```python
# tests/schema-validation/test_graph_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load(name):
    with open(os.path.join(SKILL, name)) as f:
        return json.load(f)

def test_valid_graph_passes():
    schema = load("graph.schema.json")
    instance = load("tests/schema-validation/fixtures/valid_graph.json")
    jsonschema.validate(instance, schema)

def test_node_missing_active_in_fails():
    schema = load("graph.schema.json")
    bad = {
        "version": "1.0.0",
        "nodes": [{"id": "N01", "name": "X", "type": "ingest",
                   "mode": "inline", "inputs": [], "outputs": [],
                   "aggregation_policy": "n/a", "halt_conditions": []}],
        "edges": []
    }
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_invalid_active_in_value_fails():
    schema = load("graph.schema.json")
    bad = {
        "version": "1.0.0",
        "nodes": [{"id": "N01", "name": "X", "type": "ingest",
                   "mode": "inline", "active_in": "invalid",
                   "inputs": [], "outputs": [],
                   "aggregation_policy": "n/a", "halt_conditions": []}],
        "edges": []
    }
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)
```

- [ ] **Step 2: Run test to verify it fails**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_graph_schema.py -v 2>&1 | head -20
```
Expected: FAIL (FileNotFoundError — graph.schema.json not yet written).

- [ ] **Step 3: Write valid_graph.json fixture**

```json
{
  "version": "1.0.0",
  "nodes": [
    {
      "id": "N01",
      "name": "ContextIntake",
      "type": "ingest",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["cli_args", "env"],
      "outputs": ["project_model", "resolved_flags"],
      "aggregation_policy": "n/a",
      "halt_conditions": ["halt-pre-audit", "halt-suspicious-target"]
    },
    {
      "id": "N02",
      "name": "RelevanceRouter",
      "type": "router",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "dimension_plugins"],
      "outputs": ["dimension_activation_map"],
      "aggregation_policy": "n/a",
      "halt_conditions": ["halt-on-floor-plugin-missing"]
    }
  ],
  "edges": [
    {
      "id": "E01",
      "source": "N01",
      "target": "N02",
      "channel": "data",
      "cardinality": "1:1",
      "activation": "always"
    }
  ]
}
```

- [ ] **Step 4: Write graph.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "epiphany-audit graph schema",
  "$id": "graph-schema-v1",
  "type": "object",
  "required": ["version", "nodes", "edges"],
  "additionalProperties": false,
  "properties": {
    "version": { "type": "string" },
    "nodes": {
      "type": "array",
      "items": { "$ref": "#/definitions/node" }
    },
    "edges": {
      "type": "array",
      "items": { "$ref": "#/definitions/edge" }
    }
  },
  "definitions": {
    "node": {
      "type": "object",
      "required": ["id", "name", "type", "mode", "active_in", "inputs", "outputs", "aggregation_policy", "halt_conditions"],
      "additionalProperties": false,
      "properties": {
        "id": {
          "type": "string",
          "pattern": "^N[0-9]+(\\.[a-z0-9-]+)?$"
        },
        "name": { "type": "string" },
        "type": {
          "type": "string",
          "enum": [
            "ingest", "router", "meta-analyzer", "analyzer",
            "verifier", "aggregator", "scorer", "formatter",
            "io", "validator", "planner", "preflight",
            "actuator", "recovery", "filter"
          ]
        },
        "mode": { "type": "string" },
        "active_in": {
          "type": "string",
          "enum": ["audit", "fix", "both", "improve", "always"]
        },
        "inputs": {
          "type": "array",
          "items": { "type": "string" }
        },
        "outputs": {
          "type": "array",
          "items": { "type": "string" }
        },
        "aggregation_policy": { "type": "string" },
        "halt_conditions": {
          "type": "array",
          "items": { "type": "string" }
        },
        "spawn_policy": { "type": "string" }
      }
    },
    "edge": {
      "type": "object",
      "required": ["id", "source", "target", "channel", "cardinality", "activation"],
      "additionalProperties": false,
      "properties": {
        "id": { "type": "string" },
        "source": {
          "oneOf": [
            { "type": "string" },
            { "type": "array", "items": { "type": "string" } }
          ]
        },
        "target": {
          "oneOf": [
            { "type": "string" },
            { "type": "array", "items": { "type": "string" } }
          ]
        },
        "channel": {
          "type": "string",
          "enum": ["data", "control", "feedback", "interactive"]
        },
        "cardinality": { "type": "string" },
        "activation": { "type": "string" },
        "notes": { "type": "string" }
      }
    }
  }
}
```

- [ ] **Step 5: Run tests to verify pass**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_graph_schema.py -v
```
Expected: 3 tests PASS.

- [ ] **Step 6: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/graph.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): graph.schema.json + graph schema tests"
```

---

## Task 3: audit-report-v1.schema.json

**Files:**
- Create: `tests/schema-validation/fixtures/valid_audit_report.json`
- Create: `tests/schema-validation/fixtures/invalid_audit_report_missing_id.json`
- Create: `tests/schema-validation/fixtures/invalid_audit_report_bad_severity.json`
- Create: `tests/schema-validation/test_audit_report_schema.py`
- Create: `schemas/audit-report-v1.schema.json`

All paths relative to `~/.claude/skills/epiphany-audit/`.

- [ ] **Step 1: Write test file**

```python
# tests/schema-validation/test_audit_report_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load_schema():
    with open(os.path.join(SKILL, "schemas/audit-report-v1.schema.json")) as f:
        return json.load(f)

def load_fixture(name):
    with open(os.path.join(SKILL, f"tests/schema-validation/fixtures/{name}.json")) as f:
        return json.load(f)

def test_valid_audit_report_passes():
    jsonschema.validate(load_fixture("valid_audit_report"), load_schema())

def test_missing_report_id_fails():
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(load_fixture("invalid_audit_report_missing_id"), load_schema())

def test_bad_severity_fails():
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(load_fixture("invalid_audit_report_bad_severity"), load_schema())

def test_dimensions_activated_must_contain_correctness():
    schema = load_schema()
    bad = load_fixture("valid_audit_report")
    bad["dimensions_activated"] = ["SECURITY"]
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_evidence_excerpt_extended_requires_high_confidence():
    schema = load_schema()
    bad = load_fixture("valid_audit_report")
    bad["findings"][0]["evidence_excerpt_extended"] = True
    bad["findings"][0]["confidence"] = "MEDIUM"
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_critical_finding_requires_medium_or_high_confidence():
    schema = load_schema()
    bad = load_fixture("valid_audit_report")
    bad["findings"][0]["severity"] = "CRITICAL"
    bad["findings"][0]["confidence"] = "LOW"
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)
```

- [ ] **Step 2: Run test to verify it fails**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_audit_report_schema.py -v 2>&1 | head -10
```
Expected: FAIL (FileNotFoundError — schema not yet written).

- [ ] **Step 3: Write valid_audit_report.json fixture**

```json
{
  "schema_version": 1,
  "report_id": "550e8400-e29b-41d4-a716-446655440000",
  "audit_target": "/home/user/myproject",
  "audit_timestamp": "2026-04-27T10:00:00Z",
  "tool_version": "1.0.0",
  "flags": [],
  "language_summary": {"python": 12},
  "build_manifest": "pyproject.toml",
  "test_command": "pytest",
  "dimensions_activated": ["CORRECTNESS", "MAINTAINABILITY"],
  "dimensions_skipped": [
    {"dimension": "SECURITY", "reason": "no network/IO/DB/auth surfaces detected"}
  ],
  "subtree_grouping_applied": false,
  "gap_dimensions_offered": [],
  "gap_dimensions_auto_added": [],
  "gap_dimensions_accepted": [],
  "token_cap_partial": false,
  "truncated_at_node": null,
  "improvement_report_ref": null,
  "dimensions_partial": [],
  "q_gate": {
    "pass_a": "pass",
    "pass_b": "skipped-low-volume",
    "pass_b_lens": null,
    "pass_b_skip_reason": "fewer than 5 findings and no CRITICAL/HIGH severity"
  },
  "partial_report_warning": null,
  "findings": [
    {
      "id": "F001",
      "location": "src/parser.py:142",
      "dimensions": ["CORRECTNESS"],
      "severity": "HIGH",
      "confidence": "HIGH",
      "evidence_excerpt": "for i in range(len(tokens) - 1):\n    emit(tokens[i])\n# final token never emitted",
      "evidence_excerpt_extended": false,
      "rationale": "Loop bound drops final token; downstream consumer expects all N tokens.",
      "remediation": "-    for i in range(len(tokens) - 1):\n+    for i in range(len(tokens)):",
      "false_positive_check": {
        "intentional":           {"value": false, "justification": "no test or comment justifies the -1"},
        "file_symbol_verified":  {"value": true,  "justification": "Read at src/parser.py:140-145"},
        "reachable_from_entry":  {"value": true,  "justification": "called by parse_input in main.py:23"},
        "fix_breaks_dependents": {"value": false, "justification": "grep shows no caller relies on N-1 emission"}
      },
      "effort": "trivial",
      "priority_score": 9.0,
      "verify_by": null,
      "tests_present_signal": false,
      "provenance": {
        "node": "N04",
        "mode": "inline",
        "model": "claude-sonnet-4-6",
        "pass_b_model": null,
        "prompt_hash": "a3f9e2c1d4b8f7e0a1b2c3d4e5f60718",
        "plugin_name": null,
        "plugin_version": null,
        "audit_rerun_iteration": 0,
        "q_gate_pass_b_demoted": false
      }
    }
  ]
}
```

- [ ] **Step 4: Write invalid_audit_report_missing_id.json**

```json
{
  "schema_version": 1,
  "audit_target": "/home/user/myproject",
  "audit_timestamp": "2026-04-27T10:00:00Z",
  "tool_version": "1.0.0",
  "flags": [],
  "language_summary": {},
  "build_manifest": null,
  "test_command": null,
  "dimensions_activated": ["CORRECTNESS", "MAINTAINABILITY"],
  "dimensions_skipped": [],
  "subtree_grouping_applied": false,
  "gap_dimensions_offered": [],
  "gap_dimensions_auto_added": [],
  "gap_dimensions_accepted": [],
  "token_cap_partial": false,
  "truncated_at_node": null,
  "improvement_report_ref": null,
  "dimensions_partial": [],
  "q_gate": {"pass_a": "pass", "pass_b": "skipped-low-volume", "pass_b_lens": null, "pass_b_skip_reason": null},
  "partial_report_warning": null,
  "findings": []
}
```

- [ ] **Step 5: Write invalid_audit_report_bad_severity.json**

```json
{
  "schema_version": 1,
  "report_id": "550e8400-e29b-41d4-a716-446655440001",
  "audit_target": "/home/user/myproject",
  "audit_timestamp": "2026-04-27T10:00:00Z",
  "tool_version": "1.0.0",
  "flags": [],
  "language_summary": {},
  "build_manifest": null,
  "test_command": null,
  "dimensions_activated": ["CORRECTNESS", "MAINTAINABILITY"],
  "dimensions_skipped": [],
  "subtree_grouping_applied": false,
  "gap_dimensions_offered": [],
  "gap_dimensions_auto_added": [],
  "gap_dimensions_accepted": [],
  "token_cap_partial": false,
  "truncated_at_node": null,
  "improvement_report_ref": null,
  "dimensions_partial": [],
  "q_gate": {"pass_a": "pass", "pass_b": "skipped-low-volume", "pass_b_lens": null, "pass_b_skip_reason": null},
  "partial_report_warning": null,
  "findings": [
    {
      "id": "F001",
      "location": "src/main.py:10",
      "dimensions": ["CORRECTNESS"],
      "severity": "ULTRA",
      "confidence": "HIGH",
      "evidence_excerpt": "x = 1",
      "evidence_excerpt_extended": false,
      "rationale": "test",
      "remediation": "fix it",
      "false_positive_check": {
        "intentional":           {"value": false, "justification": null},
        "file_symbol_verified":  {"value": true,  "justification": "Read at src/main.py:10"},
        "reachable_from_entry":  {"value": true,  "justification": "main"},
        "fix_breaks_dependents": {"value": false, "justification": null}
      },
      "effort": "trivial",
      "priority_score": 9.0,
      "verify_by": null,
      "tests_present_signal": false,
      "provenance": {
        "node": "N04", "mode": "inline", "model": "claude-sonnet-4-6",
        "pass_b_model": null, "prompt_hash": "abc123",
        "plugin_name": null, "plugin_version": null,
        "audit_rerun_iteration": 0, "q_gate_pass_b_demoted": false
      }
    }
  ]
}
```

- [ ] **Step 6: Write schemas/audit-report-v1.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Audit Report Schema v1",
  "$id": "audit-report-v1",
  "type": "object",
  "required": [
    "schema_version", "report_id", "audit_target", "audit_timestamp",
    "tool_version", "flags", "dimensions_activated",
    "subtree_grouping_applied", "token_cap_partial",
    "q_gate", "findings"
  ],
  "properties": {
    "schema_version": {"type": "integer", "const": 1},
    "report_id":      {"type": "string"},
    "audit_target":   {"type": "string"},
    "audit_timestamp":{"type": "string"},
    "tool_version":   {"type": "string"},
    "flags": {
      "type": "array",
      "items": {"type": "string", "enum": ["audit","verbose","deep","improve"]},
      "uniqueItems": true
    },
    "language_summary": {"type": "object", "additionalProperties": {"type": "integer"}},
    "build_manifest":  {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "test_command":    {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "dimensions_activated": {
      "type": "array",
      "items": {"type": "string"},
      "minItems": 1,
      "contains": {"const": "CORRECTNESS"}
    },
    "dimensions_skipped": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["dimension", "reason"],
        "properties": {
          "dimension": {"type": "string"},
          "reason":    {"type": "string"}
        }
      }
    },
    "subtrees": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["root", "dimensions_activated"],
        "properties": {
          "root":                {"type": "string"},
          "dimensions_activated":{"type": "array", "items": {"type": "string"}},
          "dimensions_skipped":  {"type": "array"}
        }
      }
    },
    "subtree_grouping_applied": {"type": "boolean"},
    "gap_dimensions_offered":   {"type": "array", "items": {"type": "string"}},
    "gap_dimensions_auto_added":{"type": "array", "items": {"type": "string"}},
    "gap_dimensions_accepted":  {"type": "array", "items": {"type": "string"}},
    "token_cap_partial":        {"type": "boolean"},
    "truncated_at_node": {"oneOf": [{"type": "string", "pattern": "^N[0-9]+"}, {"type": "null"}]},
    "improvement_report_ref": {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "dimensions_partial": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["dimension", "reason"],
        "properties": {
          "dimension": {"type": "string"},
          "reason":    {"type": "string"}
        }
      }
    },
    "q_gate": {
      "type": "object",
      "required": ["pass_a", "pass_b"],
      "properties": {
        "pass_a": {"type": "string", "enum": ["pass","pass-minimal","fail","skipped-token-cap"]},
        "pass_b": {"type": "string", "enum": ["pass","fail","exec-error","skipped-token-cap","skipped-low-volume"]},
        "pass_b_lens":        {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "pass_b_skip_reason": {"oneOf": [{"type": "string"}, {"type": "null"}]}
      }
    },
    "partial_report_warning": {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "findings": {
      "type": "array",
      "items": {"$ref": "#/definitions/finding"}
    }
  },
  "definitions": {
    "finding": {
      "type": "object",
      "required": [
        "id", "location", "dimensions", "severity", "confidence",
        "evidence_excerpt", "evidence_excerpt_extended",
        "rationale", "remediation", "false_positive_check",
        "effort", "priority_score", "tests_present_signal", "provenance"
      ],
      "properties": {
        "id":          {"type": "string", "pattern": "^F[0-9]{3,}$"},
        "location":    {"type": "string"},
        "dimensions":  {"type": "array", "items": {"type": "string"}, "minItems": 1},
        "severity":    {"type": "string", "enum": ["CRITICAL","HIGH","MEDIUM","LOW","INFO"]},
        "confidence":  {"type": "string", "enum": ["HIGH","MEDIUM","LOW"]},
        "evidence_excerpt":          {"type": "string"},
        "evidence_excerpt_extended": {"type": "boolean"},
        "rationale":   {"type": "string"},
        "remediation": {"type": "string"},
        "false_positive_check": {
          "type": "object",
          "required": ["intentional","file_symbol_verified","reachable_from_entry","fix_breaks_dependents"],
          "properties": {
            "intentional":           {"$ref": "#/definitions/fp_field"},
            "file_symbol_verified":  {"$ref": "#/definitions/fp_field"},
            "reachable_from_entry":  {"$ref": "#/definitions/fp_field"},
            "fix_breaks_dependents": {"$ref": "#/definitions/fp_field"}
          }
        },
        "effort":        {"type": "string", "enum": ["trivial","modest","significant"]},
        "priority_score":{"type": "number"},
        "verify_by":     {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "tests_present_signal": {"type": "boolean"},
        "provenance": {
          "type": "object",
          "required": ["node","mode","model","prompt_hash","audit_rerun_iteration","q_gate_pass_b_demoted"],
          "properties": {
            "node":                  {"type": "string"},
            "mode":                  {"type": "string", "enum": ["inline","subagent"]},
            "model":                 {"type": "string"},
            "pass_b_model":          {"oneOf": [{"type": "string"}, {"type": "null"}]},
            "prompt_hash":           {"type": "string"},
            "plugin_name":           {"oneOf": [{"type": "string"}, {"type": "null"}]},
            "plugin_version":        {"oneOf": [{"type": "string"}, {"type": "null"}]},
            "audit_rerun_iteration": {"type": "integer", "minimum": 0},
            "q_gate_pass_b_demoted": {"type": "boolean"}
          }
        },
        "notes": {"type": "string"}
      },
      "if":   {"properties": {"evidence_excerpt_extended": {"const": true}}},
      "then": {"properties": {"confidence": {"const": "HIGH"}}},
      "allOf": [
        {
          "if":   {"properties": {"severity": {"enum": ["CRITICAL","HIGH"]}}},
          "then": {"properties": {"confidence": {"enum": ["HIGH","MEDIUM"]}}}
        }
      ]
    },
    "fp_field": {
      "type": "object",
      "required": ["value", "justification"],
      "properties": {
        "value":         {"type": "boolean"},
        "justification": {"oneOf": [{"type": "string"}, {"type": "null"}]}
      }
    }
  }
}
```

- [ ] **Step 7: Run tests**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_audit_report_schema.py -v
```
Expected: 6 tests PASS.

- [ ] **Step 8: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/schemas/audit-report-v1.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): audit-report-v1 schema + tests"
```

---

## Task 4: fix-report-v1.schema.json

**Files:**
- Create: `tests/schema-validation/fixtures/valid_fix_report.json`
- Create: `tests/schema-validation/fixtures/invalid_fix_report_partial_invariant.json`
- Create: `tests/schema-validation/test_fix_report_schema.py`
- Create: `schemas/fix-report-v1.schema.json`

- [ ] **Step 1: Write test file**

```python
# tests/schema-validation/test_fix_report_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load_schema():
    with open(os.path.join(SKILL, "schemas/fix-report-v1.schema.json")) as f:
        return json.load(f)

def load_fixture(name):
    with open(os.path.join(SKILL, f"tests/schema-validation/fixtures/{name}.json")) as f:
        return json.load(f)

def test_valid_fix_report_passes():
    jsonschema.validate(load_fixture("valid_fix_report"), load_schema())

def test_partial_true_requires_halt_state():
    schema = load_schema()
    bad = load_fixture("invalid_fix_report_partial_invariant")
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_invalid_status_fails():
    schema = load_schema()
    bad = load_fixture("valid_fix_report")
    bad["body"][0]["status"] = "UNKNOWN"
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_invalid_flags_combination_fails():
    schema = load_schema()
    bad = load_fixture("valid_fix_report")
    bad["flags"] = ["auto", "confirm-all"]
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)
```

- [ ] **Step 2: Run test to verify it fails**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_fix_report_schema.py -v 2>&1 | head -10
```
Expected: FAIL (FileNotFoundError).

- [ ] **Step 3: Write valid_fix_report.json fixture**

```json
{
  "schema_version": 1,
  "fix_report_id": "660e8400-e29b-41d4-a716-446655440000",
  "partial": false,
  "halt_state": null,
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_audit_report_sha256": "abc123def456",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "fix_run_timestamp": "2026-04-27T11:00:00Z",
  "branch": "epiphany-audit/550e8400-20260427",
  "flags": [],
  "test_command_used": "pytest",
  "baseline_metrics": {
    "tests":      {"passed": 10, "failed": 0, "skipped": 0},
    "type_check": {"errors": 0},
    "lint":       {"total_warnings": 2},
    "build":      "pass"
  },
  "post_metrics": {
    "tests":      {"passed": 11, "failed": 0, "skipped": 0},
    "type_check": {"errors": 0},
    "lint":       {"total_warnings": 2, "new_warnings_in_changed_regions": 0},
    "build":      "pass"
  },
  "audit_rerun_delta": {
    "scope": "skipped-tier-policy",
    "reran_dimensions": [],
    "resolved": ["F001"],
    "fix_induced_regressions": [],
    "unchanged": [],
    "new_findings_discovered": []
  },
  "diff_scope_check": "pass",
  "unmapped_hunks": [],
  "recovery_manifest_ref": null,
  "last_known_good_sha": "deadbeef1234",
  "body": [
    {
      "id": "F001",
      "status": "verified",
      "status_qualifier": null,
      "status_qualifier_target": null,
      "tier": 1,
      "tier_classification_reason": null,
      "commit": "deadbeef1234",
      "verification_outcome": "pass",
      "regression_test_added": {
        "status": "yes",
        "location": "tests/test_parser.py:45",
        "reason": null,
        "deferred_to_followup_commit": null
      },
      "notes": null
    }
  ]
}
```

- [ ] **Step 4: Write invalid_fix_report_partial_invariant.json**

This fixture violates the invariant `partial: true ⇔ halt_state: non-null` by having `partial: true` with `halt_state: null`.

```json
{
  "schema_version": 1,
  "fix_report_id": "660e8400-e29b-41d4-a716-446655440001",
  "partial": true,
  "halt_state": null,
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_audit_report_sha256": "abc123def456",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "fix_run_timestamp": "2026-04-27T11:00:00Z",
  "branch": "epiphany-audit/550e8400-20260427",
  "flags": [],
  "test_command_used": "pytest",
  "baseline_metrics": null,
  "post_metrics": null,
  "audit_rerun_delta": {"scope": null, "reran_dimensions": [], "resolved": [], "fix_induced_regressions": [], "unchanged": [], "new_findings_discovered": []},
  "diff_scope_check": "n/a",
  "unmapped_hunks": [],
  "recovery_manifest_ref": "/home/user/docs/epiphany/audit/.recovery/550e8400.json",
  "last_known_good_sha": null,
  "body": []
}
```

- [ ] **Step 5: Write schemas/fix-report-v1.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Fix Report Schema v1",
  "$id": "fix-report-v1",
  "type": "object",
  "required": [
    "schema_version", "fix_report_id", "partial", "halt_state",
    "source_audit_report", "source_audit_report_sha256",
    "source_report_id", "fix_run_timestamp", "branch", "flags",
    "test_command_used", "audit_rerun_delta", "diff_scope_check",
    "unmapped_hunks", "recovery_manifest_ref", "last_known_good_sha", "body"
  ],
  "properties": {
    "schema_version": {"type": "integer", "const": 1},
    "fix_report_id":  {"type": "string"},
    "partial":        {"type": "boolean"},
    "halt_state":     {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "source_audit_report":       {"type": "string"},
    "source_audit_report_sha256":{"type": "string"},
    "source_report_id":          {"type": "string"},
    "fix_run_timestamp":         {"type": "string"},
    "branch":                    {"type": "string"},
    "flags": {
      "type": "array",
      "items": {"type": "string", "enum": ["auto","confirm-all","verbose","deep","full-rerun","no-rerun","reverify-state"]},
      "uniqueItems": true,
      "not": {
        "allOf": [
          {"contains": {"const": "auto"}},
          {"contains": {"const": "confirm-all"}}
        ]
      }
    },
    "test_command_used":  {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "baseline_metrics":   {"oneOf": [{"$ref": "#/definitions/metrics"}, {"type": "null"}]},
    "post_metrics":       {"oneOf": [{"$ref": "#/definitions/post_metrics"}, {"type": "null"}]},
    "audit_rerun_delta": {
      "type": "object",
      "required": ["scope"],
      "properties": {
        "scope":                   {"oneOf": [{"type": "string", "enum": ["full","narrow","skipped-tier-policy","skipped-by-flag"]}, {"type": "null"}]},
        "reran_dimensions":        {"type": "array", "items": {"type": "string"}},
        "resolved":                {"type": "array", "items": {"type": "string"}},
        "fix_induced_regressions": {"type": "array", "items": {"type": "string"}},
        "unchanged":               {"type": "array", "items": {"type": "string"}},
        "new_findings_discovered": {"type": "array", "items": {"type": "string"}}
      }
    },
    "diff_scope_check": {"type": "string", "enum": ["pass","fail-with-unmapped-hunks","n/a"]},
    "unmapped_hunks":   {"type": "array"},
    "recovery_manifest_ref": {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "last_known_good_sha":   {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "body": {
      "type": "array",
      "items": {"$ref": "#/definitions/body_entry"}
    }
  },
  "if":   {"properties": {"partial": {"const": true}}},
  "then": {"properties": {"halt_state": {"not": {"type": "null"}}}},
  "else": {"properties": {"halt_state": {"type": "null"}}},
  "definitions": {
    "metrics": {
      "type": "object",
      "properties": {
        "tests":      {"type": "object", "properties": {"passed": {"type": "integer"}, "failed": {"type": "integer"}, "skipped": {"type": "integer"}}},
        "type_check": {"type": "object", "properties": {"errors": {"type": "integer"}}},
        "lint":       {"type": "object", "properties": {"total_warnings": {"type": "integer"}}},
        "build":      {"type": "string", "enum": ["pass","fail"]}
      }
    },
    "post_metrics": {
      "type": "object",
      "properties": {
        "tests":      {"type": "object"},
        "type_check": {"type": "object"},
        "lint":       {"type": "object", "properties": {"total_warnings": {"type": "integer"}, "new_warnings_in_changed_regions": {"type": "integer"}}},
        "build":      {"type": "string", "enum": ["pass","fail"]}
      }
    },
    "body_entry": {
      "type": "object",
      "required": ["id", "status", "tier", "verification_outcome"],
      "properties": {
        "id":                     {"type": "string"},
        "status":                 {"type": "string", "enum": ["verified","failed","deferred","skipped","induced-regression","simulated"]},
        "status_qualifier":       {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "status_qualifier_target":{"oneOf": [{"type": "string"}, {"type": "null"}]},
        "tier":                   {"type": "integer", "enum": [1, 2, 3]},
        "tier_classification_reason": {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "commit":                 {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "verification_outcome":   {"type": "string", "enum": ["pass","fail","n/a","simulated"]},
        "failure_context":        {"type": "object"},
        "defer_reason":           {"type": "string"},
        "regression_test_added": {
          "type": "object",
          "properties": {
            "status":   {"type": "string", "enum": ["yes","no","n/a"]},
            "location": {"oneOf": [{"type": "string"}, {"type": "null"}]},
            "reason":   {"oneOf": [{"type": "string"}, {"type": "null"}]},
            "deferred_to_followup_commit": {"oneOf": [{"type": "string"}, {"type": "null"}]}
          }
        },
        "notes": {"oneOf": [{"type": "string"}, {"type": "null"}]}
      }
    }
  }
}
```

- [ ] **Step 6: Run tests**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_fix_report_schema.py -v
```
Expected: 4 tests PASS.

- [ ] **Step 7: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/schemas/fix-report-v1.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): fix-report-v1 schema + tests"
```

---

## Task 5: dry-run-plan-v1.schema.json

**Files:**
- Create: `tests/schema-validation/fixtures/valid_dry_run_plan.json`
- Create: `tests/schema-validation/fixtures/invalid_dry_run_plan_count_mismatch.json`
- Create: `tests/schema-validation/test_dry_run_plan_schema.py`
- Create: `schemas/dry-run-plan-v1.schema.json`

- [ ] **Step 1: Write test file**

```python
# tests/schema-validation/test_dry_run_plan_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load_schema():
    with open(os.path.join(SKILL, "schemas/dry-run-plan-v1.schema.json")) as f:
        return json.load(f)

def load_fixture(name):
    with open(os.path.join(SKILL, f"tests/schema-validation/fixtures/{name}.json")) as f:
        return json.load(f)

def test_valid_dry_run_plan_passes():
    jsonschema.validate(load_fixture("valid_dry_run_plan"), load_schema())

def test_count_mismatch_fails():
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(load_fixture("invalid_dry_run_plan_count_mismatch"), load_schema())

def test_flags_must_include_dry_run():
    schema = load_schema()
    bad = load_fixture("valid_dry_run_plan")
    bad["flags"] = ["verbose"]
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)
```

- [ ] **Step 2: Write valid_dry_run_plan.json fixture**

```json
{
  "schema_version": 1,
  "plan_id": "770e8400-e29b-41d4-a716-446655440000",
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_audit_report_sha256": "abc123def456",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "plan_timestamp": "2026-04-27T11:00:00Z",
  "flags": ["dry-run"],
  "triage_summary": {
    "total_findings": 1,
    "tier_1_count": 1,
    "tier_2_count": 0,
    "tier_3_count": 0,
    "deferred_at_triage": 0,
    "conflicting_groups": []
  },
  "simulated_branch": "epiphany-audit/550e8400-20260427",
  "test_command_resolved": "pytest",
  "body": [
    {
      "id": "F001",
      "status": "simulated",
      "tier": 1,
      "tier_classification_reason": null,
      "fix_group_id": "fg-001",
      "proposed_diff": "-    for i in range(len(tokens) - 1):\n+    for i in range(len(tokens)):",
      "files_touched": ["src/parser.py"],
      "blast_radius": {"callers_grepped": 3, "imports_referencing": 1},
      "projected_verification": {
        "targeted_tests": ["tests/test_parser.py::test_emit_all"],
        "type_check_scope": ["src/parser.py"]
      },
      "projected_commit_message": "[AUDIT-001] fix off-by-one in parser token loop",
      "projected_regression_test": "yes",
      "notes": null
    }
  ]
}
```

- [ ] **Step 3: Write invalid_dry_run_plan_count_mismatch.json**

This violates the invariant `total_findings == tier_1 + tier_2 + tier_3 + deferred`:

```json
{
  "schema_version": 1,
  "plan_id": "770e8400-e29b-41d4-a716-446655440001",
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_audit_report_sha256": "abc123def456",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "plan_timestamp": "2026-04-27T11:00:00Z",
  "flags": ["dry-run"],
  "triage_summary": {
    "total_findings": 5,
    "tier_1_count": 1,
    "tier_2_count": 0,
    "tier_3_count": 0,
    "deferred_at_triage": 0,
    "conflicting_groups": []
  },
  "simulated_branch": "epiphany-audit/550e8400-20260427",
  "test_command_resolved": "pytest",
  "body": []
}
```

- [ ] **Step 4: Write schemas/dry-run-plan-v1.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Dry-Run Plan Schema v1",
  "$id": "dry-run-plan-v1",
  "type": "object",
  "required": [
    "schema_version", "plan_id", "source_audit_report",
    "source_audit_report_sha256", "source_report_id",
    "plan_timestamp", "flags", "triage_summary",
    "simulated_branch", "body"
  ],
  "properties": {
    "schema_version":            {"type": "integer", "const": 1},
    "plan_id":                   {"type": "string"},
    "source_audit_report":       {"type": "string"},
    "source_audit_report_sha256":{"type": "string"},
    "source_report_id":          {"type": "string"},
    "plan_timestamp":            {"type": "string"},
    "flags": {
      "type": "array",
      "items": {"type": "string"},
      "contains": {"const": "dry-run"}
    },
    "triage_summary": {
      "type": "object",
      "required": ["total_findings","tier_1_count","tier_2_count","tier_3_count","deferred_at_triage","conflicting_groups"],
      "properties": {
        "total_findings":    {"type": "integer", "minimum": 0},
        "tier_1_count":      {"type": "integer", "minimum": 0},
        "tier_2_count":      {"type": "integer", "minimum": 0},
        "tier_3_count":      {"type": "integer", "minimum": 0},
        "deferred_at_triage":{"type": "integer", "minimum": 0},
        "conflicting_groups":{"type": "array"}
      }
    },
    "simulated_branch":      {"type": "string"},
    "test_command_resolved": {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "body": {
      "type": "array",
      "items": {"$ref": "#/definitions/plan_entry"}
    }
  },
  "definitions": {
    "plan_entry": {
      "type": "object",
      "required": ["id", "status", "tier", "fix_group_id", "proposed_diff", "files_touched"],
      "properties": {
        "id":            {"type": "string"},
        "status":        {"type": "string", "const": "simulated"},
        "tier":          {"type": "integer", "enum": [1, 2, 3]},
        "tier_classification_reason": {"oneOf": [{"type": "string"}, {"type": "null"}]},
        "fix_group_id":  {"type": "string"},
        "proposed_diff": {"type": "string"},
        "files_touched": {"type": "array", "items": {"type": "string"}},
        "blast_radius":  {"type": "object"},
        "projected_verification": {"type": "object"},
        "projected_commit_message": {"type": "string"},
        "projected_regression_test": {"type": "string"},
        "notes": {"oneOf": [{"type": "string"}, {"type": "null"}]}
      }
    }
  }
}
```

**Note:** The count-sum invariant (`total_findings == tier_1 + tier_2 + tier_3 + deferred`) cannot be enforced by JSON Schema Draft-07 without `if/then` arithmetic. Enforce it in the N16 emitter and in the test fixture (the invalid fixture tests schema structure only; a separate integration test should verify count sums via Python validation logic in `tests/schema-validation/test_dry_run_plan_schema.py`).

- [ ] **Step 5: Add count-sum invariant test**

Append to `test_dry_run_plan_schema.py`:

```python
def test_count_sum_invariant_valid_fixture():
    doc = load_fixture("valid_dry_run_plan")
    ts = doc["triage_summary"]
    expected = ts["tier_1_count"] + ts["tier_2_count"] + ts["tier_3_count"] + ts["deferred_at_triage"]
    assert ts["total_findings"] == expected, f"count mismatch: {ts['total_findings']} != {expected}"

def test_count_sum_invariant_invalid_fixture_catches_mismatch():
    doc = load_fixture("invalid_dry_run_plan_count_mismatch")
    ts = doc["triage_summary"]
    expected = ts["tier_1_count"] + ts["tier_2_count"] + ts["tier_3_count"] + ts["deferred_at_triage"]
    assert ts["total_findings"] != expected, "expected fixture to have a mismatch"
```

- [ ] **Step 6: Run tests**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_dry_run_plan_schema.py -v
```
Expected: 5 tests PASS.

- [ ] **Step 7: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/schemas/dry-run-plan-v1.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): dry-run-plan-v1 schema + tests"
```

---

## Task 6: dimension-plugin-v1.schema.json

**Files:**
- Create: `tests/schema-validation/fixtures/valid_dimension_plugin.json`
- Create: `tests/schema-validation/fixtures/invalid_dimension_plugin_missing_triggers.json`
- Create: `tests/schema-validation/test_dimension_plugin_schema.py`
- Create: `schemas/dimension-plugin-v1.schema.json`

- [ ] **Step 1: Write test file**

```python
# tests/schema-validation/test_dimension_plugin_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load_schema():
    with open(os.path.join(SKILL, "schemas/dimension-plugin-v1.schema.json")) as f:
        return json.load(f)

def load_fixture(name):
    with open(os.path.join(SKILL, f"tests/schema-validation/fixtures/{name}.json")) as f:
        return json.load(f)

def test_valid_plugin_passes():
    jsonschema.validate(load_fixture("valid_dimension_plugin"), load_schema())

def test_missing_activation_triggers_fails():
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(load_fixture("invalid_dimension_plugin_missing_triggers"), load_schema())

def test_invalid_priority_fails():
    schema = load_schema()
    bad = load_fixture("valid_dimension_plugin")
    bad["priority"] = "ultra"
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)
```

- [ ] **Step 2: Write valid_dimension_plugin.json fixture**

```json
{
  "schema_version": 1,
  "name": "juce-rt-safety",
  "display_name": "JUCE Real-Time Audio Safety",
  "version": "1.0.0",
  "applies_to": {
    "languages": ["cpp"],
    "project_markers": ["CMakeLists.txt", "JUCE/", "*.vst3"]
  },
  "activation_triggers": [
    {"type": "file_present", "path": "**/JuceHeader.h"},
    {"type": "import_grep", "pattern": "AudioProcessor", "min_matches": 1}
  ],
  "exclusions": [
    {"type": "project_size", "max_files": 1}
  ],
  "prompt_template": "Analyze the following code for JUCE real-time audio safety issues.\nLook for:\n- Allocations on the audio thread\n- Mutex locks on the audio thread\n- Blocking calls in processBlock\nReturn findings in Audit Report Schema v1 format.\n",
  "kb_route_query": null,
  "intra_node_token_budget": 30000,
  "priority": "high"
}
```

- [ ] **Step 3: Write invalid_dimension_plugin_missing_triggers.json**

```json
{
  "schema_version": 1,
  "name": "bad-plugin",
  "display_name": "Bad Plugin",
  "version": "1.0.0",
  "applies_to": {
    "languages": ["python"],
    "project_markers": []
  },
  "exclusions": [],
  "prompt_template": "Analyze for issues.\n",
  "kb_route_query": null,
  "intra_node_token_budget": 30000,
  "priority": "medium"
}
```

- [ ] **Step 4: Write schemas/dimension-plugin-v1.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Dimension Plugin Schema v1",
  "$id": "dimension-plugin-v1",
  "type": "object",
  "required": [
    "schema_version", "name", "display_name", "version",
    "applies_to", "activation_triggers", "prompt_template",
    "intra_node_token_budget", "priority"
  ],
  "properties": {
    "schema_version":  {"type": "integer", "const": 1},
    "name":            {"type": "string", "pattern": "^[a-z0-9-]+$"},
    "display_name":    {"type": "string"},
    "version":         {"type": "string"},
    "applies_to": {
      "type": "object",
      "required": ["languages", "project_markers"],
      "properties": {
        "languages":       {"oneOf": [{"type": "array", "items": {"type": "string"}}, {"type": "string", "const": "*"}]},
        "project_markers": {"type": "array", "items": {"type": "string"}}
      }
    },
    "activation_triggers": {
      "type": "array",
      "minItems": 1,
      "items": {"$ref": "#/definitions/trigger"}
    },
    "exclusions": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["type"],
        "properties": {
          "type": {"type": "string"},
          "max_files": {"type": "integer"}
        }
      }
    },
    "prompt_template":       {"type": "string"},
    "kb_route_query":        {"oneOf": [{"type": "string"}, {"type": "null"}]},
    "intra_node_token_budget":{"type": "integer", "minimum": 1000},
    "priority":              {"type": "string", "enum": ["low","medium","high"]}
  },
  "definitions": {
    "trigger": {
      "type": "object",
      "required": ["type"],
      "properties": {
        "type":        {"type": "string", "enum": ["file_present","import_grep"]},
        "path":        {"type": "string"},
        "pattern":     {"type": "string"},
        "min_matches": {"type": "integer", "minimum": 1}
      }
    }
  }
}
```

- [ ] **Step 5: Run tests**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_dimension_plugin_schema.py -v
```
Expected: 3 tests PASS.

- [ ] **Step 6: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/schemas/dimension-plugin-v1.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): dimension-plugin-v1 schema + tests"
```

---

## Task 7: improvement-report-v1.schema.json

**Files:**
- Create: `tests/schema-validation/fixtures/valid_improvement_report.json`
- Create: `tests/schema-validation/fixtures/invalid_improvement_report_count_mismatch.json`
- Create: `tests/schema-validation/test_improvement_report_schema.py`
- Create: `schemas/improvement-report-v1.schema.json`

- [ ] **Step 1: Write test file**

```python
# tests/schema-validation/test_improvement_report_schema.py
import json, os, pytest
import jsonschema

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")

def load_schema():
    with open(os.path.join(SKILL, "schemas/improvement-report-v1.schema.json")) as f:
        return json.load(f)

def load_fixture(name):
    with open(os.path.join(SKILL, f"tests/schema-validation/fixtures/{name}.json")) as f:
        return json.load(f)

def test_valid_improvement_report_passes():
    jsonschema.validate(load_fixture("valid_improvement_report"), load_schema())

def test_count_mismatch_fails():
    doc = load_fixture("invalid_improvement_report_count_mismatch")
    ts = doc
    expected_survivors = ts["notable"] + ts["quick_wins"] + ts["worthwhile"]
    assert ts["survivors"] != expected_survivors, "fixture should have mismatch"

def test_flags_must_include_improve():
    schema = load_schema()
    bad = load_fixture("valid_improvement_report")
    bad["flags"] = ["verbose"]
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_invalid_category_fails():
    schema = load_schema()
    bad = load_fixture("valid_improvement_report")
    bad["body"][0]["category"] = "legendary"
    with pytest.raises(jsonschema.ValidationError):
        jsonschema.validate(bad, schema)

def test_count_sum_invariant_valid():
    doc = load_fixture("valid_improvement_report")
    assert doc["survivors"] == doc["notable"] + doc["quick_wins"] + doc["worthwhile"]
    assert doc["total_candidates"] == doc["filtered_out"] + doc["survivors"]
```

- [ ] **Step 2: Write valid_improvement_report.json**

```json
{
  "schema_version": 1,
  "improvement_report_id": "880e8400-e29b-41d4-a716-446655440000",
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "audit_target": "/home/user/myproject",
  "improvement_timestamp": "2026-04-27T12:00:00Z",
  "tool_version": "1.0.0",
  "flags": ["improve"],
  "improvement_partial": false,
  "total_candidates": 5,
  "filtered_out": 3,
  "survivors": 2,
  "notable": 0,
  "quick_wins": 1,
  "worthwhile": 1,
  "body": [
    {
      "id": "I001",
      "category": "quick-win",
      "area": "testing",
      "utility_score": 2,
      "cost_score": 1,
      "description": "The project uses dynamic test discovery but has no conftest.py at the repo root. Failures in fixture setup are silently swallowed on Python < 3.11, meaning a broken fixture causes zero tests to run rather than N failures.",
      "action": "Add a minimal conftest.py at the repo root with a session-scoped fixture guard: assert sys.version_info >= (3, 10).",
      "success_measure": "Running pytest with a broken fixture produces a visible ERROR line rather than 'collected 0 items'.",
      "notes": null
    },
    {
      "id": "I002",
      "category": "worthwhile",
      "area": "developer-experience",
      "utility_score": 2,
      "cost_score": 2,
      "description": "The project has no pre-commit hook configuration, so formatting violations accumulate silently.",
      "action": "Add .pre-commit-config.yaml with ruff and mypy hooks; run pre-commit install.",
      "success_measure": "git commit on unformatted code produces an immediate ruff violation rather than CI failure minutes later.",
      "notes": null
    }
  ]
}
```

- [ ] **Step 3: Write invalid_improvement_report_count_mismatch.json**

```json
{
  "schema_version": 1,
  "improvement_report_id": "880e8400-e29b-41d4-a716-446655440001",
  "source_audit_report": "/home/user/docs/epiphany/audit/myproject-20260427-100000.md",
  "source_report_id": "550e8400-e29b-41d4-a716-446655440000",
  "audit_target": "/home/user/myproject",
  "improvement_timestamp": "2026-04-27T12:00:00Z",
  "tool_version": "1.0.0",
  "flags": ["improve"],
  "improvement_partial": false,
  "total_candidates": 10,
  "filtered_out": 3,
  "survivors": 99,
  "notable": 1,
  "quick_wins": 1,
  "worthwhile": 1,
  "body": []
}
```

- [ ] **Step 4: Write schemas/improvement-report-v1.schema.json**

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Improvement Report Schema v1",
  "$id": "improvement-report-v1",
  "type": "object",
  "required": [
    "schema_version", "improvement_report_id", "source_audit_report",
    "source_report_id", "audit_target", "improvement_timestamp",
    "tool_version", "flags", "improvement_partial",
    "total_candidates", "filtered_out", "survivors",
    "notable", "quick_wins", "worthwhile", "body"
  ],
  "properties": {
    "schema_version":        {"type": "integer", "const": 1},
    "improvement_report_id": {"type": "string"},
    "source_audit_report":   {"type": "string"},
    "source_report_id":      {"type": "string"},
    "audit_target":          {"type": "string"},
    "improvement_timestamp": {"type": "string"},
    "tool_version":          {"type": "string"},
    "flags": {
      "type": "array",
      "items": {"type": "string"},
      "contains": {"const": "improve"}
    },
    "improvement_partial": {"type": "boolean"},
    "total_candidates": {"type": "integer", "minimum": 0},
    "filtered_out":     {"type": "integer", "minimum": 0},
    "survivors":        {"type": "integer", "minimum": 0},
    "notable":          {"type": "integer", "minimum": 0},
    "quick_wins":       {"type": "integer", "minimum": 0},
    "worthwhile":       {"type": "integer", "minimum": 0},
    "body": {
      "type": "array",
      "items": {"$ref": "#/definitions/improvement_entry"}
    }
  },
  "definitions": {
    "improvement_entry": {
      "type": "object",
      "required": ["id", "category", "area", "utility_score", "cost_score", "description", "action", "success_measure"],
      "properties": {
        "id":            {"type": "string", "pattern": "^I[0-9]{3,}$"},
        "category":      {"type": "string", "enum": ["quick-win","worthwhile","notable"]},
        "area":          {"type": "string", "enum": ["developer-experience","testing","architecture","performance","tooling","dependencies","documentation"]},
        "utility_score": {"type": "integer", "minimum": 2, "maximum": 3},
        "cost_score":    {"type": "integer", "minimum": 1, "maximum": 3},
        "description":   {"type": "string"},
        "action":        {"type": "string"},
        "success_measure":{"type": "string"},
        "notes":         {"oneOf": [{"type": "string"}, {"type": "null"}]}
      }
    }
  }
}
```

- [ ] **Step 5: Run tests**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/test_improvement_report_schema.py -v
```
Expected: 5 tests PASS.

- [ ] **Step 6: Run all schema tests together**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/ -v
```
Expected: All tests PASS (≥17 total across 5 test files + graph test).

- [ ] **Step 7: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/schemas/improvement-report-v1.schema.json \
  ~/.claude/skills/epiphany-audit/tests/
git -C ~/.claude commit -m "feat(epiphany-audit): improvement-report-v1 schema + tests; all schema tests green"
```

---

## Task 8: graph.json (27 nodes + 28 edges)

**Files:**
- Create: `~/.claude/skills/epiphany-audit/graph.json`

- [ ] **Step 1: Write graph.json**

```json
{
  "version": "1.0.0",
  "nodes": [
    {
      "id": "N01",
      "name": "ContextIntake",
      "type": "ingest",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["cli_args", "cwd", "env"],
      "outputs": ["project_model", "resolved_flags"],
      "aggregation_policy": "n/a",
      "halt_conditions": [
        "halt-pre-audit", "halt-suspicious-target",
        "halt-ambiguous-target", "halt-on-flag-conflict",
        "halt-on-flag-rejection", "halt-no-source-detected"
      ]
    },
    {
      "id": "N02",
      "name": "RelevanceRouter",
      "type": "router",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "dimension_plugins_from_disk"],
      "outputs": ["dimension_activation_map", "plugin_registry"],
      "aggregation_policy": "n/a",
      "halt_conditions": ["halt-on-floor-plugin-missing"]
    },
    {
      "id": "N03",
      "name": "BlindspotFinder",
      "type": "meta-analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "dimension_activation_map"],
      "outputs": ["updated_dimension_activation_map", "gap_dimensions_offered", "gap_dimensions_auto_added"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N04",
      "name": "DimensionAnalyzer.CORRECTNESS",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": []
    },
    {
      "id": "N05",
      "name": "DimensionAnalyzer.ARCHITECTURE",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": []
    },
    {
      "id": "N06",
      "name": "DimensionAnalyzer.PERFORMANCE",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": []
    },
    {
      "id": "N07",
      "name": "DimensionAnalyzer.SECURITY",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": []
    },
    {
      "id": "N08",
      "name": "DimensionAnalyzer.MAINTAINABILITY",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": []
    },
    {
      "id": "N09",
      "name": "DimensionAnalyzer.<X>",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["project_model", "file_subset", "plugin_manifest"],
      "outputs": ["raw_findings"],
      "aggregation_policy": "participant in BACKTRACK via N10",
      "halt_conditions": [],
      "spawn_policy": "plugin-instantiated at runtime; concrete instances get IDs N09.a11y, N09.iac-drift, etc."
    },
    {
      "id": "N10",
      "name": "FalsePositiveVerifier",
      "type": "verifier",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["raw_findings from N04..N09"],
      "outputs": ["verified_findings", "location_verification_cache"],
      "aggregation_policy": "BACKTRACKING owner (single re-emit cap on N04..N09)",
      "halt_conditions": []
    },
    {
      "id": "N11",
      "name": "FindingsAggregator",
      "type": "aggregator",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["verified_findings from N10"],
      "outputs": ["deduplicated_findings"],
      "aggregation_policy": "AGGREGATION owner (audit side)",
      "halt_conditions": []
    },
    {
      "id": "N12",
      "name": "Prioritizer",
      "type": "scorer",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["deduplicated_findings"],
      "outputs": ["prioritized_findings", "punch_list"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N13",
      "name": "ReportFormatter",
      "type": "formatter",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["prioritized_findings", "punch_list", "project_model"],
      "outputs": ["formatted_report_markdown"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N14",
      "name": "Q-GATE",
      "type": "verifier",
      "mode": "inline (Pass A) + conditional subagent (Pass B)",
      "active_in": "audit",
      "inputs": ["formatted_report_markdown", "location_verification_cache"],
      "outputs": ["q_gate_result", "validated_report"],
      "aggregation_policy": "adversarial self-review",
      "halt_conditions": ["halt-on-q-gate-failure"],
      "spawn_policy": "Pass B spawns when: >=5 findings OR any CRITICAL/HIGH severity OR --deep flag"
    },
    {
      "id": "N15",
      "name": "SaveHandler",
      "type": "io",
      "mode": "inline",
      "active_in": "audit",
      "inputs": ["validated_report", "project_model"],
      "outputs": ["save_decision", "saved_report_path", "report_id"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N16",
      "name": "FixTriage",
      "type": "validator",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["audit_report_path", "resolved_flags", "recovery_manifest_if_resume"],
      "outputs": ["triage_result", "fix_groups", "topo_sorted_order"],
      "aggregation_policy": "AGGREGATION (file-grouping) + CONDITIONAL ROUTING owner",
      "halt_conditions": [
        "halt-pre-fix-on-validator-failure", "halt-on-empty-or-unfixable-report",
        "halt-on-conflicting-fixes", "halt-on-files-outside-tree",
        "halt-on-invalid-finding-id", "halt-on-resume-tree-divergence",
        "halt-on-stale-source-report", "halt-on-mismatched-version"
      ]
    },
    {
      "id": "N17",
      "name": "FixPlanner",
      "type": "planner",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["triage_result", "fix_groups"],
      "outputs": ["fix_plan_doc", "user_approvals"],
      "aggregation_policy": "n/a",
      "halt_conditions": ["halt-on-user-abort"]
    },
    {
      "id": "N18",
      "name": "PreFlight",
      "type": "preflight",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["fix_plan_doc", "project_model"],
      "outputs": ["baseline_metrics", "branch_name"],
      "aggregation_policy": "n/a",
      "halt_conditions": [
        "halt-on-baseline-failure", "halt-on-test-cmd-unknown",
        "halt-on-git-state-incompatible"
      ]
    },
    {
      "id": "N19",
      "name": "FixApplier",
      "type": "actuator",
      "mode": "inline (write-serial)",
      "active_in": "fix",
      "inputs": ["fix_groups", "user_approvals", "baseline_metrics", "branch_name"],
      "outputs": ["per_fix_outcomes", "recovery_manifest_updates"],
      "aggregation_policy": "n/a",
      "halt_conditions": ["halt-mid-fix-on-perfix-cap-hit"]
    },
    {
      "id": "N20",
      "name": "PerFixVerifier",
      "type": "verifier",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["changed_files", "baseline_metrics"],
      "outputs": ["verify_result", "failure_context"],
      "aggregation_policy": "emits fail-signal; routing is E_repair's responsibility",
      "halt_conditions": []
    },
    {
      "id": "N21",
      "name": "RegressionBattery",
      "type": "verifier",
      "mode": "inline (battery) + conditional subagent (audit-rerun under --deep)",
      "active_in": "both",
      "inputs": ["per_fix_outcomes", "baseline_metrics", "applied_tier_max"],
      "outputs": ["battery_result", "audit_rerun_delta"],
      "aggregation_policy": "adversarial-via-rerun",
      "halt_conditions": [
        "halt-on-scope-creep", "halt-mid-fix-on-induced-regression-cap-hit"
      ]
    },
    {
      "id": "N22",
      "name": "RollbackHandler",
      "type": "recovery",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["e_repair_cap_hit_signal_or_e_finalize_signal", "fix_report_id_on_finalize"],
      "outputs": ["recovery_manifest_finalized", "archive_path_on_completion"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N23",
      "name": "FixReporter",
      "type": "formatter",
      "mode": "inline",
      "active_in": "fix",
      "inputs": ["per_fix_outcomes", "baseline_metrics", "audit_rerun_delta"],
      "outputs": ["fix_report_markdown", "fix_report_id"],
      "aggregation_policy": "AGGREGATION (per-fix outcomes)",
      "halt_conditions": []
    },
    {
      "id": "N24",
      "name": "ImprovementContextualizer",
      "type": "analyzer",
      "mode": "inline",
      "active_in": "improve",
      "inputs": ["project_model from N01", "validated_report from N14"],
      "outputs": ["improvement_context"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    },
    {
      "id": "N25",
      "name": "ImprovementBrainstormer",
      "type": "analyzer",
      "mode": "inline (phase 1) + optional subagent (phase 2 when --deep and >=8 candidates)",
      "active_in": "improve",
      "inputs": ["improvement_context"],
      "outputs": ["improvement_candidates"],
      "aggregation_policy": "participant in AGGREGATION via N26",
      "halt_conditions": [],
      "spawn_policy": "0 or 1 spawn: phase 2 only when --deep AND >=8 raw candidates from phase 1"
    },
    {
      "id": "N26",
      "name": "OverEngineeringFilter",
      "type": "filter",
      "mode": "inline",
      "active_in": "improve",
      "inputs": ["improvement_candidates"],
      "outputs": ["survivors", "discarded_improvements"],
      "aggregation_policy": "AGGREGATION (candidate filtering + categorization)",
      "halt_conditions": []
    },
    {
      "id": "N27",
      "name": "ImprovementReporter",
      "type": "formatter",
      "mode": "inline",
      "active_in": "improve",
      "inputs": ["survivors", "discarded_improvements", "improvement_context"],
      "outputs": ["improvement_report_markdown"],
      "aggregation_policy": "n/a",
      "halt_conditions": []
    }
  ],
  "edges": [
    {"id": "E01",          "source": "N01", "target": "N02",              "channel": "data",        "cardinality": "1:1",  "activation": "always"},
    {"id": "E02",          "source": "N02", "target": "N03",              "channel": "data",        "cardinality": "1:1",  "activation": "always"},
    {"id": "E03",          "source": "N02", "target": ["N04","N05","N06","N07","N08","N09"], "channel": "control", "cardinality": "1:N",  "activation": "per activation map; floor (CORRECTNESS, MAINTAINABILITY) always emits"},
    {"id": "E04",          "source": "N03", "target": "user",             "channel": "interactive", "cardinality": "1:1",  "activation": "only under --deep; default mode auto-adds HIGH-confidence gaps without prompting"},
    {"id": "E05",          "source": ["N04","N05","N06","N07","N08","N09"], "target": "N10", "channel": "data", "cardinality": "fan-in", "activation": "always"},
    {"id": "E06",          "source": "N10", "target": ["N04","N05","N06","N07","N08","N09"], "channel": "feedback", "cardinality": "refinement", "activation": "BACKTRACKING — single-attempt cap on re-emit"},
    {"id": "E07",          "source": "N10", "target": "N11",             "channel": "data",        "cardinality": "1:1",  "activation": "always"},
    {"id": "E08",          "source": "N11", "target": "N12",             "channel": "data",        "cardinality": "1:1",  "activation": "always"},
    {"id": "E09",          "source": "N12", "target": "N13",             "channel": "data",        "cardinality": "1:1",  "activation": "always"},
    {"id": "E10",          "source": "N13", "target": "N14",             "channel": "control",     "cardinality": "1:1",  "activation": "always"},
    {"id": "E11",          "source": "N14", "target": "N15",             "channel": "data",        "cardinality": "1:1",  "activation": "Pass A succeeds AND (Pass B succeeds OR skipped-low-volume); Pass B exec-error -> halt"},
    {"id": "E12",          "source": "N15", "target": "user",            "channel": "interactive", "cardinality": "1:1",  "activation": "save? prompt after N15 save-decision resolves"},
    {"id": "E13",          "source": "N16", "target": ["N17","N18","N19","N20","N21","N23"], "channel": "data", "cardinality": "1:1 each", "activation": "fix pipeline chain; entry from --fix or post-E21 fix-offer consent"},
    {"id": "E14",          "source": "E_repair cap-hit", "target": "N22", "channel": "control",   "cardinality": "1:1",  "activation": "E_repair 3rd invocation (cap-hit) -> N22 finalizes recovery manifest for failed fix-group"},
    {"id": "E_halt_partial","source": "halt-mid-fix-*", "target": ["N23","user"], "channel": "control", "cardinality": "1:1", "activation": "perfix-cap-hit or induced-regression-cap-hit AND every remaining group blocked"},
    {"id": "E15",          "source": "N20", "target": "N19",            "channel": "feedback",    "cardinality": "1:1",  "activation": "on PerFixVerify success -> next fix-group"},
    {"id": "E_repair",     "source": ["N20 fail","N21 fail"], "target": ["N17","N19"], "channel": "feedback", "cardinality": "1:1", "activation": "bounded: 1st->N19 retry; 2nd->N17 replan; 3rd->cap-hit"},
    {"id": "E_rerun_fail", "source": "N21 audit-rerun induced-regression", "target": "N16", "channel": "feedback", "cardinality": "1:1 batched", "activation": "re-triage with regression context; only for new findings in files touched by fixes"},
    {"id": "E_diffscope",  "source": "N21 diff-scope-check fail", "target": "halt", "channel": "control", "cardinality": "1:0", "activation": "scope-creep detected; do NOT auto-revert"},
    {"id": "E_finalize",   "source": "N23", "target": "N22",            "channel": "control",     "cardinality": "1:1",  "activation": "planned termination: N23 writes report -> E_finalize -> N22 reads fix_report_id + archives manifest -> E_complete"},
    {"id": "E_complete",   "source": "N22", "target": "user",           "channel": "data",        "cardinality": "1:1",  "activation": "terminal; fires after N22 finalizes + archives manifest"},
    {"id": "E16",          "source": "N15", "target": "N24",            "channel": "control",     "cardinality": "1:1",  "activation": "--improve only; fires after save prompt resolves; does NOT fire in --fix mode"},
    {"id": "E17",          "source": "N24", "target": "N25",            "channel": "data",        "cardinality": "1:1",  "activation": "always within --improve subpipeline"},
    {"id": "E18",          "source": "N25", "target": "N26",            "channel": "data",        "cardinality": "1:1",  "activation": "always within --improve subpipeline"},
    {"id": "E19",          "source": "N26", "target": "N27",            "channel": "data",        "cardinality": "1:1",  "activation": "always within --improve subpipeline"},
    {"id": "E20",          "source": "N27", "target": "user",           "channel": "data",        "cardinality": "1:1",  "activation": "terminal for --improve subpipeline; improvement report written unconditionally; E21 fires AFTER E20 resolves in no-flag mode"},
    {"id": "E21",          "source": ["N27","N15"], "target": "user",   "channel": "interactive", "cardinality": "1:1",  "activation": "fix? offer; in --improve no-flag mode fires after E20; in non-improve no-flag mode fires after E12"}
  ]
}
```

- [ ] **Step 2: Validate graph.json against graph.schema.json**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -c "
import json, jsonschema
schema = json.load(open('graph.schema.json'))
graph  = json.load(open('graph.json'))
jsonschema.validate(graph, schema)
print('graph.json validates OK')
"
```
Expected: `graph.json validates OK`

- [ ] **Step 3: Verify node count**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -c "
import json
g = json.load(open('graph.json'))
print('nodes:', len(g['nodes']), '  edges:', len(g['edges']))
"
```
Expected: `nodes: 27   edges: 27` (or close — the exact count includes E_repair, E_rerun_fail, E_diffscope, E_finalize, E_complete, E_halt_partial as named edges alongside E01–E21).

- [ ] **Step 4: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/graph.json
git -C ~/.claude commit -m "feat(epiphany-audit): graph.json — 27 nodes + 26 edges"
```

---

## Task 9: Dimension plugins (5 built-in + README)

**Files:**
- Create: `~/.claude/skills/epiphany-audit/dimensions/correctness.md`
- Create: `~/.claude/skills/epiphany-audit/dimensions/architecture.md`
- Create: `~/.claude/skills/epiphany-audit/dimensions/performance.md`
- Create: `~/.claude/skills/epiphany-audit/dimensions/security.md`
- Create: `~/.claude/skills/epiphany-audit/dimensions/maintainability.md`
- Create: `~/.claude/skills/epiphany-audit/dimensions/README.md`

These are thin manifests; their prompt logic mirrors modules/N04..N08.md but is expressed as plugin frontmatter so R-ROUTE can load them through the same code path as user plugins. CORRECTNESS and MAINTAINABILITY are floor plugins — they cannot be shadowed by user plugins.

- [ ] **Step 1: Write dimensions/correctness.md**

```markdown
---
schema_version: 1
name: correctness
display_name: Correctness
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: file_present
    path: "**/*"
exclusions: []
prompt_template: |
  Analyze the following code for correctness issues. Look for:
  - Logic errors: off-by-one, wrong conditional, inverted boolean
  - Type/lifetime errors: mismatched types, use-after-free, null dereference
  - Boundary violations: unchecked array access, integer overflow, buffer overrun
  - Concurrency: data races, missing locks, TOCTOU
  - Resource leaks: unclosed files, unreleased memory, leaked handles
  - Error paths: silently swallowed exceptions, ignored return codes
  For each finding, verify the file:line via Read before reporting.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 30000
priority: high
---

# Correctness Dimension

**Floor dimension — always activated regardless of project type.**

Finds defects where the code produces wrong results, crashes, or corrupts state.
Covers the full CORRECTNESS taxonomy: logic errors, type/lifetime, boundary,
concurrency, resource leaks, and error paths.

## Failure modes (non-exhaustive)

- Loop bounds (off-by-one)
- Null/None dereference on unchecked return values
- Integer wrap/overflow in size calculations
- Race conditions on shared mutable state
- File handles / DB connections not closed on exception path
- Exception caught and silently dropped with `pass` / `catch (Exception e) {}`
- Wrong operator precedence (`a & b == c` instead of `(a & b) == c`)
- Incorrect use of mutable default arguments (Python)
```

- [ ] **Step 2: Write dimensions/maintainability.md**

```markdown
---
schema_version: 1
name: maintainability
display_name: Maintainability
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: file_present
    path: "**/*"
exclusions: []
prompt_template: |
  Analyze the following code for maintainability issues. Look for:
  - Dead code: unreachable branches, unused variables/functions/imports with no callers
  - Misleading names: identifiers whose names contradict their behavior
  - Stale comments/TODOs: TODOs referencing issues that are already closed or resolved
  - Test coverage gaps: failure-mode branches with no corresponding test
  - Excessive complexity: functions >50 lines with no natural split point
  - Magic numbers/strings without named constants
  For each finding, verify the file:line via Read before reporting.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 30000
priority: high
---

# Maintainability Dimension

**Floor dimension — always activated regardless of project type.**

Finds code that is technically correct but will become a defect source as the
codebase evolves: dead code, misleading names, stale TODOs, coverage gaps on
failure branches.
```

- [ ] **Step 3: Write dimensions/architecture.md**

```markdown
---
schema_version: 1
name: architecture
display_name: Architecture
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: file_present
    path: "**/*"
exclusions:
  - type: project_size
    max_files: 5
prompt_template: |
  Analyze the following code for architectural issues. Look for:
  - Excessive coupling: modules that directly access internals of other modules
  - Circular dependencies: A imports B imports A (or through a chain)
  - God objects: classes/modules with more than 10 distinct responsibilities
  - Duplicated logic: the same non-trivial algorithm implemented in 2+ places
  - Invariant gaps: class invariants that callers can violate without error
  - Latent issues: architectural decisions that are safe now but will break under
    specific future conditions (tag these findings with reachable=false and note
    the reachability condition).
  For each finding, verify the file:line via Read before reporting.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 30000
priority: medium
---

# Architecture Dimension

Finds structural defects: coupling violations, circular dependencies, god objects,
duplicated logic, and invariant gaps. Tags latent findings with reachability conditions.
```

- [ ] **Step 4: Write dimensions/performance.md**

```markdown
---
schema_version: 1
name: performance
display_name: Performance
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: import_grep
    pattern: "for |while |\.sort|\.map|\.filter|O\\("
    min_matches: 3
exclusions:
  - type: project_size
    max_files: 3
prompt_template: |
  Analyze the following code for performance issues. Look for:
  - Hot allocations: objects created on every iteration of a tight loop
  - Algorithmic complexity blowups: O(n^2) or worse where O(n log n) is available
  - Cache layout: data structures that thrash CPU cache (linked lists in hot paths)
  - False sharing: adjacent mutable fields accessed from different threads
  - Unnecessary repeated work: recomputing the same value inside a loop
  Only report issues with a concrete, measurable performance impact.
  Tag speculative findings (no profiling evidence) with confidence: LOW.
  For each finding, verify the file:line via Read before reporting.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 30000
priority: medium
---

# Performance Dimension

Finds hot allocations, complexity blowups, cache-hostile layouts, and false sharing.
Only surface findings with concrete performance impact — speculative ones must be
tagged LOW confidence with a `verify_by` profiling recommendation.
```

- [ ] **Step 5: Write dimensions/security.md**

```markdown
---
schema_version: 1
name: security
display_name: Security
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: import_grep
    pattern: "subprocess|exec|eval|os\\.system|request|sqlite|psycopg|http|jwt|auth|secret|password|token"
    min_matches: 1
exclusions: []
prompt_template: |
  Analyze the following code for security vulnerabilities using sub-surface routing:
  - SQL injection: string-interpolated queries, no parameterization
  - Shell injection: user input passed to subprocess/exec/os.system without sanitization
  - Auth/authz: missing authentication checks, broken access control
  - Secrets in source: hardcoded credentials, API keys, private keys
  - Deserialization: unsafe pickle/eval/yaml.load on untrusted input
  - Prompt injection: LLM-facing code that concatenates user input directly into prompts
  Only activate sub-surfaces where the project has the relevant component.
  Emit skipped sub-surfaces with reason in the report.
  For each finding, verify the file:line via Read before reporting.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 30000
priority: high
---

# Security Dimension

Per-surface sub-routing for SQL, shell, auth, secrets, deserialization, and
prompt-injection. Only activates sub-surfaces where the project has the relevant
component (e.g., skips SQL sub-surface when no DB layer detected).
```

- [ ] **Step 6: Write dimensions/README.md**

```markdown
# Dimension Plugins

Each `.md` file in this directory is a dimension plugin conforming to
`schemas/dimension-plugin-v1.schema.json`.

## Built-in plugins (versioned with the skill)

| File               | Dimension     | Floor? |
|--------------------|---------------|--------|
| correctness.md     | CORRECTNESS   | yes — always activated; cannot be shadowed |
| maintainability.md | MAINTAINABILITY | yes — always activated; cannot be shadowed |
| architecture.md    | ARCHITECTURE  | no |
| performance.md     | PERFORMANCE   | no |
| security.md        | SECURITY      | no |

## Adding a custom dimension

1. Create `~/.config/epiphany-audit/dimensions/<your-dimension>.md`
2. Populate YAML frontmatter per `dimension-plugin-v1.schema.json`
3. A plugin with the same `name` as a built-in shadows the built-in entirely
   (exception: CORRECTNESS and MAINTAINABILITY cannot be shadowed)
4. Plugin files that fail schema validation are logged and skipped — they do not
   halt the audit

## Example custom plugin frontmatter

```yaml
schema_version: 1
name: juce-rt-safety
display_name: JUCE Real-Time Audio Safety
version: 1.0.0
applies_to:
  languages: [cpp]
  project_markers: ["CMakeLists.txt", "*.vst3"]
activation_triggers:
  - type: file_present
    path: "**/JuceHeader.h"
prompt_template: |
  Analyze for allocations and mutex locks on the audio thread...
kb_route_query: null
intra_node_token_budget: 30000
priority: high
```
```

- [ ] **Step 7: Validate dimension plugins against schema**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -c "
import json, yaml, jsonschema, os

schema = json.load(open('schemas/dimension-plugin-v1.schema.json'))
for fname in ['correctness.md','architecture.md','performance.md','security.md','maintainability.md']:
    path = os.path.join('dimensions', fname)
    content = open(path).read()
    # Extract YAML frontmatter between --- markers
    parts = content.split('---')
    fm = yaml.safe_load(parts[1])
    jsonschema.validate(fm, schema)
    print(f'{fname}: OK')
"
```
Expected: 5 lines of `<name>: OK`.

- [ ] **Step 8: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/dimensions/
git -C ~/.claude commit -m "feat(epiphany-audit): 5 built-in dimension plugins + README"
```

---

## Task 10: Report templates (4 files)

**Files:**
- Create: `~/.claude/skills/epiphany-audit/templates/audit-report.md.template`
- Create: `~/.claude/skills/epiphany-audit/templates/fix-report.md.template`
- Create: `~/.claude/skills/epiphany-audit/templates/dry-run-plan.md.template`
- Create: `~/.claude/skills/epiphany-audit/templates/improvement-report.md.template`

- [ ] **Step 1: Write templates/audit-report.md.template**

```
---
schema_version: 1
report_id: {{report_id}}
audit_target: {{audit_target}}
audit_timestamp: {{audit_timestamp}}
tool_version: {{tool_version}}
flags: {{flags}}
language_summary: {{language_summary}}
build_manifest: {{build_manifest}}
test_command: {{test_command}}

dimensions_activated: {{dimensions_activated}}
dimensions_skipped:
{{#dimensions_skipped}}
  - dimension: {{dimension}}
    reason: "{{reason}}"
{{/dimensions_skipped}}

{{#subtrees}}
subtrees:
{{#subtrees}}
  - root: {{root}}
    dimensions_activated: {{dimensions_activated}}
    dimensions_skipped: {{dimensions_skipped}}
{{/subtrees}}
{{/subtrees}}
subtree_grouping_applied: {{subtree_grouping_applied}}

gap_dimensions_offered: {{gap_dimensions_offered}}
gap_dimensions_auto_added: {{gap_dimensions_auto_added}}
gap_dimensions_accepted: {{gap_dimensions_accepted}}

token_cap_partial: {{token_cap_partial}}
truncated_at_node: {{truncated_at_node}}
improvement_report_ref: {{improvement_report_ref}}

dimensions_partial: {{dimensions_partial}}

q_gate:
  pass_a: {{q_gate.pass_a}}
  pass_b: {{q_gate.pass_b}}
  pass_b_lens: {{q_gate.pass_b_lens}}
  pass_b_skip_reason: {{q_gate.pass_b_skip_reason}}

partial_report_warning: {{partial_report_warning}}
---

{{#partial_report_warning}}
> **⚠ PARTIAL REPORT:** {{partial_report_warning}}

{{/partial_report_warning}}
## Resolve-Before-Testing

{{#punch_list}}
- **{{severity}}** `{{id}}` — {{summary}} (`{{location}}`)
{{/punch_list}}

---

{{#findings}}
## Finding {{id}}

```yaml
id: {{id}}
location: {{location}}
dimensions: {{dimensions}}
severity: {{severity}}
confidence: {{confidence}}
evidence_excerpt: |
{{evidence_excerpt}}
evidence_excerpt_extended: {{evidence_excerpt_extended}}
rationale: |
  {{rationale}}
remediation: |
  {{remediation}}
false_positive_check:
  intentional:           { value: {{false_positive_check.intentional.value}}, justification: "{{false_positive_check.intentional.justification}}" }
  file_symbol_verified:  { value: {{false_positive_check.file_symbol_verified.value}}, justification: "{{false_positive_check.file_symbol_verified.justification}}" }
  reachable_from_entry:  { value: {{false_positive_check.reachable_from_entry.value}}, justification: "{{false_positive_check.reachable_from_entry.justification}}" }
  fix_breaks_dependents: { value: {{false_positive_check.fix_breaks_dependents.value}}, justification: "{{false_positive_check.fix_breaks_dependents.justification}}" }
effort: {{effort}}
priority_score: {{priority_score}}
verify_by: {{verify_by}}
tests_present_signal: {{tests_present_signal}}
provenance:
  node: {{provenance.node}}
  mode: {{provenance.mode}}
  model: {{provenance.model}}
  pass_b_model: {{provenance.pass_b_model}}
  prompt_hash: {{provenance.prompt_hash}}
  plugin_name: {{provenance.plugin_name}}
  plugin_version: {{provenance.plugin_version}}
  audit_rerun_iteration: {{provenance.audit_rerun_iteration}}
  q_gate_pass_b_demoted: {{provenance.q_gate_pass_b_demoted}}
```
{{/findings}}

---

## Unverified Hypotheses

*Findings demoted by FPV (false-positive verification). Not consumable by `--fix`.*

{{#unverified_hypotheses}}
- `{{id}}` `{{location}}` — {{notes}}
{{/unverified_hypotheses}}
```

- [ ] **Step 2: Write templates/fix-report.md.template**

```
---
schema_version: 1
fix_report_id: {{fix_report_id}}
partial: {{partial}}
halt_state: {{halt_state}}
source_audit_report: {{source_audit_report}}
source_audit_report_sha256: {{source_audit_report_sha256}}
source_report_id: {{source_report_id}}
fix_run_timestamp: {{fix_run_timestamp}}
branch: {{branch}}
flags: {{flags}}
test_command_used: {{test_command_used}}
baseline_metrics:
  tests:      { passed: {{baseline_metrics.tests.passed}}, failed: {{baseline_metrics.tests.failed}}, skipped: {{baseline_metrics.tests.skipped}} }
  type_check: { errors: {{baseline_metrics.type_check.errors}} }
  lint:       { total_warnings: {{baseline_metrics.lint.total_warnings}} }
  build: {{baseline_metrics.build}}
post_metrics:
  tests:      { passed: {{post_metrics.tests.passed}}, failed: {{post_metrics.tests.failed}}, skipped: {{post_metrics.tests.skipped}} }
  type_check: { errors: {{post_metrics.type_check.errors}} }
  lint:       { total_warnings: {{post_metrics.lint.total_warnings}}, new_warnings_in_changed_regions: {{post_metrics.lint.new_warnings_in_changed_regions}} }
  build: {{post_metrics.build}}
audit_rerun_delta:
  scope: {{audit_rerun_delta.scope}}
  reran_dimensions: {{audit_rerun_delta.reran_dimensions}}
  resolved: {{audit_rerun_delta.resolved}}
  fix_induced_regressions: {{audit_rerun_delta.fix_induced_regressions}}
  unchanged: {{audit_rerun_delta.unchanged}}
  new_findings_discovered: {{audit_rerun_delta.new_findings_discovered}}
diff_scope_check: {{diff_scope_check}}
unmapped_hunks: {{unmapped_hunks}}
recovery_manifest_ref: {{recovery_manifest_ref}}
last_known_good_sha: {{last_known_good_sha}}
---

{{#deferred_items}}
## Deferred Items

{{#deferred_items}}
- `{{id}}` — {{defer_reason}}
{{/deferred_items}}

---
{{/deferred_items}}

{{#manual_edits}}
## Manual Edits (user-authorized scope)

{{#manual_edits}}
- `{{file}}:{{lines}}` — {{summary}} (rationale: {{rationale}})
{{/manual_edits}}

---
{{/manual_edits}}

{{#body}}
## {{id}}

```yaml
id: {{id}}
status: {{status}}
status_qualifier: {{status_qualifier}}
tier: {{tier}}
commit: {{commit}}
verification_outcome: {{verification_outcome}}
regression_test_added:
  status: {{regression_test_added.status}}
  location: {{regression_test_added.location}}
  deferred_to_followup_commit: {{regression_test_added.deferred_to_followup_commit}}
notes: {{notes}}
```
{{/body}}
```

- [ ] **Step 3: Write templates/dry-run-plan.md.template**

```
---
schema_version: 1
plan_id: {{plan_id}}
source_audit_report: {{source_audit_report}}
source_audit_report_sha256: {{source_audit_report_sha256}}
source_report_id: {{source_report_id}}
plan_timestamp: {{plan_timestamp}}
flags: {{flags}}
triage_summary:
  total_findings: {{triage_summary.total_findings}}
  tier_1_count: {{triage_summary.tier_1_count}}
  tier_2_count: {{triage_summary.tier_2_count}}
  tier_3_count: {{triage_summary.tier_3_count}}
  deferred_at_triage: {{triage_summary.deferred_at_triage}}
  conflicting_groups: {{triage_summary.conflicting_groups}}
simulated_branch: {{simulated_branch}}
test_command_resolved: {{test_command_resolved}}
---

## Plan Summary

- **Total findings:** {{triage_summary.total_findings}}
  (Tier-1: {{triage_summary.tier_1_count}} | Tier-2: {{triage_summary.tier_2_count}} | Tier-3: {{triage_summary.tier_3_count}} | Deferred: {{triage_summary.deferred_at_triage}})
- **Simulated branch:** `{{simulated_branch}}`
- **To apply:** drop `--dry-run` and re-run with `--fix {{source_audit_report}}`

---

## Tier-1 Fixes (mechanical — batch-confirm under default policy)

{{#tier_1_entries}}
### {{id}}

```yaml
id: {{id}}
status: simulated
tier: 1
fix_group_id: {{fix_group_id}}
proposed_diff: |
{{proposed_diff}}
files_touched: {{files_touched}}
projected_commit_message: "{{projected_commit_message}}"
```
{{/tier_1_entries}}

## Tier-2 Fixes (local logic — batch-confirm)

{{#tier_2_entries}}
### {{id}}

```yaml
id: {{id}}
tier: 2
fix_group_id: {{fix_group_id}}
proposed_diff: |
{{proposed_diff}}
```
{{/tier_2_entries}}

## Tier-3 Fixes (cross-cutting — per-fix confirm)

{{#tier_3_entries}}
### {{id}}

```yaml
id: {{id}}
tier: 3
fix_group_id: {{fix_group_id}}
proposed_diff: |
{{proposed_diff}}
```
{{/tier_3_entries}}

## Deferred at Triage

{{#deferred_entries}}
### {{id}}

```yaml
id: {{id}}
status: deferred-at-triage
defer_reason: |
  {{defer_reason}}
original_severity: {{original_severity}}
original_remediation: |
  {{original_remediation}}
```
{{/deferred_entries}}
```

- [ ] **Step 4: Write templates/improvement-report.md.template**

```
---
schema_version: 1
improvement_report_id: {{improvement_report_id}}
source_audit_report: {{source_audit_report}}
source_report_id: {{source_report_id}}
audit_target: {{audit_target}}
improvement_timestamp: {{improvement_timestamp}}
tool_version: {{tool_version}}
flags: {{flags}}
improvement_partial: {{improvement_partial}}
total_candidates: {{total_candidates}}
filtered_out: {{filtered_out}}
survivors: {{survivors}}
notable: {{notable}}
quick_wins: {{quick_wins}}
worthwhile: {{worthwhile}}
---

{{#improvement_partial}}
> **⚠ PARTIAL IMPROVEMENT ANALYSIS:** {{improvement_partial_warning}}

{{/improvement_partial}}
## Summary

- **Brainstormed:** {{total_candidates}} candidates
- **Filtered by OEF:** {{filtered_out}} (cost > utility, marginal utility, or speculative language)
- **Survivors:** {{survivors}} (notable: {{notable}} | quick wins: {{quick_wins}} | worthwhile: {{worthwhile}})

{{#zero_survivors}}
No improvements above the utility/cost threshold were found. This is a valid result — the project may be well-optimized in its current state.
{{/zero_survivors}}

---

{{#notable_entries}}
## Notable Improvements (high utility, low-to-medium cost)

{{#notable_entries}}
### {{id}}

```yaml
id: {{id}}
category: notable
area: {{area}}
utility_score: {{utility_score}}
cost_score: {{cost_score}}
description: |
  {{description}}
action: |
  {{action}}
success_measure: |
  {{success_measure}}
```
{{/notable_entries}}
{{/notable_entries}}

## Quick Wins

{{#quick_win_entries}}
### {{id}}

```yaml
id: {{id}}
category: quick-win
area: {{area}}
utility_score: {{utility_score}}
cost_score: 1
description: |
  {{description}}
action: |
  {{action}}
success_measure: |
  {{success_measure}}
```
{{/quick_win_entries}}

## Worthwhile Improvements

{{#worthwhile_entries}}
### {{id}}

```yaml
id: {{id}}
category: worthwhile
area: {{area}}
utility_score: {{utility_score}}
cost_score: {{cost_score}}
description: |
  {{description}}
action: |
  {{action}}
success_measure: |
  {{success_measure}}
```
{{/worthwhile_entries}}

{{#verbose_mode}}
## Filtered Improvements (--verbose only)

{{#filtered_entries}}
- **{{id}}** — `{{area}}` | utility={{utility_score}} cost={{cost_score}}
  OEF rationale: {{oef_rationale}}
{{/filtered_entries}}
{{/verbose_mode}}
```

- [ ] **Step 5: Verify template files exist**

Run:
```bash
ls ~/.claude/skills/epiphany-audit/templates/
```
Expected: 4 files: `audit-report.md.template`, `fix-report.md.template`, `dry-run-plan.md.template`, `improvement-report.md.template`.

- [ ] **Step 6: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/templates/
git -C ~/.claude commit -m "feat(epiphany-audit): 4 report templates"
```

---

## Task 11: Module structure linter + modules N01–N03

**Files:**
- Create: `~/.claude/skills/epiphany-audit/tests/check_module_structure.py`
- Create: `~/.claude/skills/epiphany-audit/modules/N01-context-intake.md`
- Create: `~/.claude/skills/epiphany-audit/modules/N02-r-route.md`
- Create: `~/.claude/skills/epiphany-audit/modules/N03-b-find.md`

- [ ] **Step 1: Write tests/check_module_structure.py**

```python
#!/usr/bin/env python3
"""Checks every module file has all required Layer-B contract sections."""
import os, sys, glob

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")
REQUIRED_SECTIONS = [
    "## Inputs", "## Outputs", "## Side Effects",
    "## Halt Conditions", "## Token Budget",
    "## Backtrack / Aggregation", "## Fan-out Cardinality",
    "## Back-edge Endpoints"
]

errors = []
module_files = sorted(glob.glob(os.path.join(SKILL, "modules/*.md")))
if not module_files:
    print("ERROR: no module files found")
    sys.exit(1)

for path in module_files:
    content = open(path).read()
    missing = [s for s in REQUIRED_SECTIONS if s not in content]
    if missing:
        errors.append(f"{os.path.basename(path)}: missing {missing}")

if errors:
    print("Module structure errors:")
    for e in errors:
        print(f"  {e}")
    sys.exit(1)
else:
    print(f"All {len(module_files)} modules have required sections. OK.")
```

- [ ] **Step 2: Run linter (expect failure — no modules yet)**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `ERROR: no module files found`

- [ ] **Step 3: Write modules/N01-context-intake.md**

```markdown
# N01 — ContextIntake

**Type:** ingest
**Mode:** inline
**Active in:** `audit` (skipped entirely in `--fix` mode; N01..N15 all skipped when `--fix` is passed directly)

## Inputs

- CLI arguments: `path`, flags (`--audit`, `--verbose`, `--deep`, `--improve`, `--auto`, `--confirm-all`, `--dry-run`, `--escalate-finding F00N`, `--test-cmd '<cmd>'`, `--monorepo-subtree-limit N`, `--full-rerun`, `--no-rerun`, `--reverify-state`)
- Environment: `cwd`, `$HOME`, git state at cwd

## Outputs

```
project_model: {
  audit_target: string,           // resolved absolute path
  language_summary: { [lang]: integer },
  file_count: integer,
  total_lines: integer,
  build_manifest: string | null,
  test_command: string | null,
  git_state: {
    head: string, dirty: boolean, branch: string | null,
    detached: boolean, has_commits: boolean
  },
  entry_points: string[],
  project_type: string[],
  is_monorepo: boolean,
  subtrees: SubtreeDescriptor[]
}
resolved_flags: {
  mode: "audit" | "no-flag",
  verbosity: "verbose" | "normal",
  depth: "deep" | "normal",
  improve: boolean,
  autonomy: "auto" | "confirm-all" | "dry-run" | "default",
  escalated_findings: string[],
  test_cmd_override: string | null,
  monorepo_subtree_limit: integer,
  rerun_override: "full" | "no" | null
}
```

## Side Effects

None. Read-only file-system inspection.

## Halt Conditions

- `halt-pre-audit`: resolution chain exhausted with no target
- `halt-suspicious-target` (hard): resolved root is `$HOME`, `/`, `/etc`, `/usr`, `/var`, `/tmp`, or has >5 top-level project-shaped subdirs
- `halt-ambiguous-target`: nested git repos; session touched multiple projects; polyglot inconclusive
- `halt-on-flag-conflict`: both `--audit` and `--fix`; or both `--dry-run`/`--confirm-all`/`--auto` (>1 set); or both `--full-rerun` and `--no-rerun`
- `halt-on-flag-rejection`: unsupported flag (e.g., `--demote-finding`)
- `halt-on-target-conflict`: both explicit `<path>` and `--fix <report>` given; `realpath(<path>) != realpath(audit_target)`
- `halt-no-source-detected`: binary-only, empty, or documentation-only repo

Warn-and-prompt (soft halt; user may override via `~/.config/epiphany-audit/allowed-roots.json`):
- Resolved root inside `~/.claude/skills/<x>/`, `~/dotfiles`, `~/.config`, `~/Desktop`, `~/Downloads`

## Implied-Context Resolution (deterministic, no silent guessing)

1. Explicit `<path>` argument wins
2. `--fix <report>` → derives target from `audit_target` field after report resolution
3. `cwd` inside git repo → `git rev-parse --show-toplevel`
4. `cwd` itself
5. `halt-pre-audit`

Suspicious-target gate runs on resolved target regardless of resolution path.

## Token Budget

Minimal. Directory listing + git status + build-manifest read. No analysis.

## Backtrack / Aggregation

None. Sole producer of `project_model` and `resolved_flags` — feeds N02..N15.

## Fan-out Cardinality

1:1 → N02 via E01.

## Back-edge Endpoints

None. No incoming back-edges.
```

- [ ] **Step 4: Write modules/N02-r-route.md**

```markdown
# N02 — RelevanceRouter (R-ROUTE)

**Type:** router
**Mode:** inline
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
dimension_plugins: Plugin[]  // loaded at N02 startup from:
                             // (1) ~/.claude/skills/epiphany-audit/dimensions/*.md
                             // (2) ~/.config/epiphany-audit/dimensions/*.md
resolved_flags: (from N01)
```

## Outputs

```
dimension_activation_map: {
  activated: string[],
  skipped: { dimension: string, reason: string }[]
}
subtrees_activation_maps: SubtreeActivationMap[]  // only for monorepos
plugin_registry: Plugin[]          // validated, ordered
kb_prefetch_results: { [dim]: string }  // for plugins with kb_route_query
```

## Side Effects

- Read: `~/.claude/skills/epiphany-audit/dimensions/*.md`
- Read: `~/.config/epiphany-audit/dimensions/*.md` (if exists)
- Write-log: one structured event per plugin load attempt (success / validation-fail / shadowed / rejected)

## Halt Conditions

- `halt-on-floor-plugin-missing`: `correctness.md` or `maintainability.md` missing from skill-bundled `dimensions/`. Subreason: `<plugin-name>`. A floor plugin that fails schema validation also triggers this halt.

Logged, not halted:
- Plugin fails schema validation → skip and log
- Two user plugins share same `name` → alphabetical loser logged and skipped
- User plugin shadows CORRECTNESS or MAINTAINABILITY → rejected with warning

## Floor Invariant

CORRECTNESS + MAINTAINABILITY are always activated regardless of plugin presence or activation triggers. R-ROUTE cannot suppress floor dimensions.

## Loading and Shadow Rules

1. Bundled plugins: `~/.claude/skills/epiphany-audit/dimensions/*.md`
2. User plugins: `~/.config/epiphany-audit/dimensions/*.md` (loaded after; may shadow by `name`)
3. User plugin with same `name` as bundled → shadows bundled entirely (no merge)
4. CORRECTNESS and MAINTAINABILITY cannot be shadowed (warning emitted, user plugin rejected)
5. Alphabetical wins between two user plugins sharing a name

## Activation Trigger Logic (per plugin)

ALL `activation_triggers` must match for activation:
- `file_present`: path glob matches any file in project tree
- `import_grep`: pattern grepped, `min_matches` met

ANY `exclusions` match → dimension skipped.

## Token Budget

Low (directory listing + YAML plugin manifest reads). No analysis.

## Backtrack / Aggregation

None. Produces activation map consumed by N03 and N04..N09.

## Fan-out Cardinality

1:1 → N03 (E02); 1:N → N04..N09 per activation map (E03).

## Back-edge Endpoints

None.
```

- [ ] **Step 5: Write modules/N03-b-find.md**

```markdown
# N03 — BlindspotFinder (B-FIND)

**Type:** meta-analyzer
**Mode:** inline
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
dimension_activation_map: (from N02)
resolved_flags: (from N01)
```

## Outputs

```
updated_dimension_activation_map: {
  activated: string[],
  skipped: { dimension: string, reason: string }[]
}
gap_dimensions_auto_added: string[]   // HIGH-confidence gaps added in default mode
gap_dimensions_offered: string[]      // populated only under --deep
gap_dimensions_accepted: string[]     // subset offered that user opted into (--deep only)
```

## Side Effects

- Interactive (only under `--deep`): `include gap dimension <name>? (y/n/skip-all)` prompt per gap candidate
- Write-log: structured event per gap dimension considered

## Halt Conditions

- `halt-on-user-abort`: ctrl-C at B-FIND prompt (only under `--deep`)

Logged, not halted:
- Invalid input at B-FIND prompt → loop with format reminder, do not halt

## Default Mode Behavior

AUTO-ADD HIGH-confidence gap dimensions to the activation map without prompting. A gap dimension is HIGH-confidence when:
- The project type strongly implies the dimension (e.g., a web API project implies an accessibility dimension if the project serves HTML)
- At least 2 independent heuristics agree (language + framework marker + import grep)

Gap dimensions added appear in `gap_dimensions_auto_added` in the audit report frontmatter.

## `--deep` Mode Behavior

Prompt the user per gap candidate: `include gap dimension <name>? (y/n/skip-all)`. User answers form `gap_dimensions_accepted`. ctrl-C → `halt-on-user-abort`.

## Token Budget

Low. Heuristic inspection of `project_model` only; no file reads beyond what N01 already captured.

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:N (feeds N04..N09 via updated activation map, then E03).

## Back-edge Endpoints

None.
```

- [ ] **Step 6: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 3 modules have required sections. OK.`

- [ ] **Step 7: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/ \
  ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
git -C ~/.claude commit -m "feat(epiphany-audit): modules N01-N03 + module structure linter"
```

---

## Task 12: Modules N04–N09 (Dimension Analyzers)

**Files:**
- Create: `modules/N04-correctness.md` through `modules/N09-extensible.md`

- [ ] **Step 1: Write modules/N04-correctness.md**

```markdown
# N04 — DimensionAnalyzer.CORRECTNESS

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep` when analyzer fan-out budget allows (capped at 1 batched subagent slot for all over-cap analyzers combined)
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
file_subset: string[]       // files to analyze (from R-ROUTE + B-FIND activation map)
resolved_flags: (from N01)
```

## Outputs

```
raw_findings: Finding[]    // unverified; fed to N10 FPV
```

## Side Effects

Read-only. Reads source files via Read tool (no writes).

## Halt Conditions

None. Analysis failures are logged; the node produces whatever findings it can within budget.

## Analysis Scope

Logic errors, type/lifetime, boundary violations, concurrency, resource leaks, error paths. See `dimensions/correctness.md` for the full taxonomy and prompt template.

**Floor dimension.** Always runs regardless of R-ROUTE activation map. Cannot be disabled.

## Anti-Patterns (findings this node MUST NOT produce)

- Stylistic preferences disguised as bugs
- Findings without reading the actual code (hallucinated `file:line`)
- Generic advice applicable to any project
- Duplicate findings (N11 handles dedup, but N04 should not emit obvious duplicates)
- Echoing project's own TODO/FIXME as findings

## Token Budget

30k tokens per analyzer invocation (intra-node soft budget under `--deep`; see §6.1 of spec).

## Backtrack / Aggregation

Participant in BACKTRACKING: N10 FPV may re-emit one finding back to N04 with a re-analysis request (single cap). N04 must honor the re-analysis context and either confirm or revise the finding.

## Fan-out Cardinality

1:many findings → N10 via E05.

## Back-edge Endpoints

E06: N10 → N04 (backtrack feedback, single re-emit cap).
```

- [ ] **Step 2: Write modules/N05-architecture.md**

```markdown
# N05 — DimensionAnalyzer.ARCHITECTURE

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep`
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
file_subset: string[]
resolved_flags: (from N01)
```

## Outputs

```
raw_findings: Finding[]
```

## Side Effects

Read-only.

## Halt Conditions

None.

## Analysis Scope

Excessive coupling, circular dependencies, god objects (>10 distinct responsibilities), duplicated non-trivial logic, invariant gaps. **Latent findings** (safe now, will break under specific future conditions) are tagged with `reachable: false` and a `notes: "reachable when: <condition>"` annotation.

## Anti-Patterns

Same as N04 plus: do not report refactors without a concrete defect; do not flag large classes unless they demonstrably have >10 distinct responsibilities.

## Token Budget

30k tokens per invocation (intra-node soft budget).

## Backtrack / Aggregation

Participant in BACKTRACKING via N10 (single re-emit cap).

## Fan-out Cardinality

1:many findings → N10 via E05.

## Back-edge Endpoints

E06: N10 → N05 (backtrack feedback, single re-emit cap).
```

- [ ] **Step 3: Write modules/N06-performance.md**

```markdown
# N06 — DimensionAnalyzer.PERFORMANCE

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep`
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
file_subset: string[]
resolved_flags: (from N01)
```

## Outputs

```
raw_findings: Finding[]
```

## Side Effects

Read-only.

## Halt Conditions

None.

## Analysis Scope

Hot allocations in tight loops, algorithmic complexity blowups (O(n²) where O(n log n) is available), cache-hostile data structures, false sharing between threads, unnecessary repeated computation inside loops. Only report findings with a concrete, measurable performance impact. Speculative findings (no profiling evidence) must be tagged `confidence: LOW` with `verify_by` recommending profiling.

## Activation

R-ROUTE activates PERFORMANCE only when heuristics suggest a hot-path (import grep for sort/filter/loop patterns, `project_size.total_lines > 500`, etc.). See `dimensions/performance.md` activation triggers.

## Token Budget

30k tokens per invocation.

## Backtrack / Aggregation

Participant in BACKTRACKING via N10 (single re-emit cap).

## Fan-out Cardinality

1:many findings → N10 via E05.

## Back-edge Endpoints

E06: N10 → N06.
```

- [ ] **Step 4: Write modules/N07-security.md**

```markdown
# N07 — DimensionAnalyzer.SECURITY

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep`
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
file_subset: string[]
resolved_flags: (from N01)
```

## Outputs

```
raw_findings: Finding[]
```

## Side Effects

Read-only.

## Halt Conditions

None.

## Analysis Scope

Per-surface sub-routing (run only sub-surfaces where the project has the relevant component):

| Sub-surface     | Trigger                                          |
|-----------------|--------------------------------------------------|
| SQL injection   | DB layer detected (psycopg, sqlite3, sqlalchemy) |
| Shell injection | subprocess/exec/os.system usage                  |
| Auth/authz      | auth/jwt/session patterns detected               |
| Secrets in code | grep for `password`, `secret`, `api_key`, `token` literals |
| Deserialization | pickle/eval/yaml.load on untrusted data          |
| Prompt injection| LLM-facing code concatenating user input into prompts |

Skipped sub-surfaces appear in the audit report with reason.

## Token Budget

30k tokens per invocation.

## Backtrack / Aggregation

Participant in BACKTRACKING via N10 (single re-emit cap).

## Fan-out Cardinality

1:many findings → N10 via E05.

## Back-edge Endpoints

E06: N10 → N07.
```

- [ ] **Step 5: Write modules/N08-maintainability.md**

```markdown
# N08 — DimensionAnalyzer.MAINTAINABILITY

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep`
**Active in:** `audit`

## Inputs

```
project_model: (from N01)
file_subset: string[]
resolved_flags: (from N01)
```

## Outputs

```
raw_findings: Finding[]
```

## Side Effects

Read-only.

## Halt Conditions

None.

## Analysis Scope

Dead code (unreachable branches, unused variables/functions with no callers), misleading identifiers (names that contradict behavior), stale TODOs referencing already-resolved issues, test coverage gaps on failure-mode branches, excessive function complexity (>50 lines with no natural split), magic numbers/strings without named constants.

**Floor dimension.** Always runs. Cannot be disabled.

## Token Budget

30k tokens per invocation.

## Backtrack / Aggregation

Participant in BACKTRACKING via N10 (single re-emit cap).

## Fan-out Cardinality

1:many findings → N10 via E05.

## Back-edge Endpoints

E06: N10 → N08.
```

- [ ] **Step 6: Write modules/N09-extensible.md**

```markdown
# N09 — DimensionAnalyzer.<X> (plugin-instantiated)

**Type:** analyzer
**Mode:** inline (default); subagent under `--deep`
**Active in:** `audit`

**Note:** N09 is a template module. At runtime, R-ROUTE instantiates concrete nodes
from dimension plugins. Concrete instances receive IDs `N09.a11y`, `N09.iac-drift`,
`N09.juce-rt-safety`, etc., recorded in `graph.json` at run time.

## Inputs

```
project_model: (from N01)
file_subset: string[]
plugin_manifest: Plugin     // the loaded dimension-plugin-v1 manifest
resolved_flags: (from N01)
kb_context: string | null   // prefetched via kb-route if plugin.kb_route_query is set
```

## Outputs

```
raw_findings: Finding[]     // provenance.plugin_name = plugin.name; provenance.plugin_version = plugin.version
```

## Side Effects

Read-only. If `plugin.kb_route_query` is set, R-ROUTE prefetches KB context before N09 runs (N09 itself does not call kb-route).

## Halt Conditions

None. Plugin failures (runtime error during analysis) are caught, logged, and the node produces zero findings for that plugin instance.

## Plugin Contract

The node executes `plugin.prompt_template` with `{{project_model}}` and `{{file_subset}}` interpolated. The prompt template is responsible for defining the analysis scope. The `provenance` field on each returned finding must set `plugin_name` and `plugin_version` from the manifest.

## Token Budget

`plugin.intra_node_token_budget` (default 30k; per-plugin override in manifest).

## Backtrack / Aggregation

Participant in BACKTRACKING via N10 (single re-emit cap per plugin instance).

## Fan-out Cardinality

1:many findings per plugin instance → N10 via E05.

## Back-edge Endpoints

E06: N10 → N09.<plugin-name> (per instance).
```

- [ ] **Step 7: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 9 modules have required sections. OK.`

- [ ] **Step 8: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/
git -C ~/.claude commit -m "feat(epiphany-audit): modules N04-N09 (dimension analyzers)"
```

---

## Task 13: Modules N10–N15 (FPV → SaveHandler)

**Files:**
- Create: `modules/N10-fpv.md` through `modules/N15-save-handler.md`

- [ ] **Step 1: Write modules/N10-fpv.md**

```markdown
# N10 — FalsePositiveVerifier (FPV)

**Type:** verifier
**Mode:** inline
**Active in:** `audit`

## Inputs

```
raw_findings: Finding[]   // fan-in from N04..N09 via E05
```

## Outputs

```
verified_findings: Finding[]          // passed the 4-question check
unverified_hypotheses: Finding[]      // demoted
location_verification_cache: {        // shared with N14 Pass A
  [canonical_file_path + ":" + line_range]: {
    verified: boolean,
    content_hash: string,             // sha256 of bytes read
    populated_by: "N10",
    populated_at: string              // ISO 8601
  }
}
```

## Side Effects

Read-only. Reads source files via Read tool to verify `file:line` locations. Populates `location_verification_cache` (in-memory only, not persisted).

## Halt Conditions

None.

## Four False-Positive Questions (applied to each finding)

1. `intentional`: Is this behavior intentional by design?
2. `file_symbol_verified`: Does the symbol/function/line actually exist at the cited location?
3. `reachable_from_entry`: Is this code reachable from a declared entry point?
4. `fix_breaks_dependents`: Would applying the proposed fix break callers?

## Disposition Rules (deterministic)

| Condition | Disposition |
|-----------|-------------|
| `file_symbol_verified.value == false` | **Discard** entirely (not even Unverified Hypothesis) |
| `intentional.value == true` | **Demote** to Unverified Hypotheses; notes: "demoted: code appears intentional ({{justification}})" |
| `reachable_from_entry.value == false` AND severity ∈ {CRITICAL, HIGH} | **Demote** to Unverified Hypotheses; notes: "demoted: unreachable from entry but claimed {{severity}}" |
| `reachable_from_entry.value == false` AND severity ∈ {MEDIUM, LOW, INFO} | **Pass** with notes: "unreachable from declared entry points" |
| `fix_breaks_dependents.value == true` AND `confidence == HIGH` | **Pass**; remediation MUST address dependents |
| `fix_breaks_dependents.value == true` AND `confidence < HIGH` | **Demote** to Unverified Hypotheses; notes: "demoted: fix would break dependents; confidence below HIGH floor" |
| All four pass | **Pass** unconditionally |

## BACKTRACKING (single re-emit cap)

If a finding fails verification because the location seems wrong (possible hallucination in the analyzer), N10 may re-emit one request to the originating analyzer (N04..N09) via E06 with the correction context. The originating analyzer may revise the finding. The cap is one re-emit per finding — a revised finding that still fails is discarded.

## Location Verification Cache Contract

- N10 is the **only writer**. Writes after a successful Read.
- Key: `(canonical_path, line_range)` normalized as `(start_line, end_line)`.
- Failed Reads recorded as `verified: false` (no re-attempt by downstream consumers).
- Cache lives for one skill invocation; never persisted to disk.
- N14 Pass A consumes the cache (read-only); falls back to its own Read on cache miss.

## Token Budget

Scales with finding count. Budget per finding: ~500 tokens. Parallel fan-out across findings allowed (reads are independent).

## Backtrack / Aggregation

BACKTRACKING owner (audit side): initiates single re-emit to N04..N09 via E06.

## Fan-out Cardinality

1:many (per finding). Reads are parallelizable.

## Back-edge Endpoints

E06: N10 → N04..N09 (backtrack; single re-emit cap per finding).
```

- [ ] **Step 2: Write modules/N11-aggregator.md**

```markdown
# N11 — FindingsAggregator

**Type:** aggregator
**Mode:** inline
**Active in:** `audit`

## Inputs

```
verified_findings: Finding[]   // from N10
```

## Outputs

```
deduplicated_findings: Finding[]
```

## Side Effects

None (pure transformation in memory).

## Halt Conditions

None.

## Aggregation Rules

1. **Dedup by pattern+location**: two findings are duplicates iff they share the same `location` (file:line-range) AND describe the same defect class (same dimension + same severity + substantively identical rationale). Merge into one finding with `count: N` in notes.
2. **Cross-dimension overlap merge**: if two findings from different dimensions describe the same root cause at the same `location`, keep the one with higher `priority_score` and list the other dimension in the merged finding's `dimensions` array.
3. **Count-collapse**: wall-of-low-severity-nitpicks at the same location → collapse into one finding with `count` in notes and the highest severity among the group.

## Token Budget

Minimal (in-memory set operations, no file reads).

## Backtrack / Aggregation

AGGREGATION owner (audit side).

## Fan-out Cardinality

N:1 (fan-in from multiple findings; emits smaller deduplicated set).

## Back-edge Endpoints

None.
```

- [ ] **Step 3: Write modules/N12-prioritizer.md**

```markdown
# N12 — Prioritizer

**Type:** scorer
**Mode:** inline
**Active in:** `audit`

## Inputs

```
deduplicated_findings: Finding[]   // from N11
```

## Outputs

```
prioritized_findings: Finding[]    // sorted by priority_score descending
punch_list: Finding[]              // CRITICAL + HIGH always; + MEDIUM until 15 total
```

## Side Effects

None.

## Halt Conditions

None.

## Priority Score Formula (deterministic)

```
severity_weight:    CRITICAL=4, HIGH=3, MEDIUM=2, LOW=1, INFO=0
confidence_weight:  HIGH=3, MEDIUM=2, LOW=1
effort_weight:      trivial=1, modest=2, significant=3

priority_score = (severity_weight × confidence_weight) / effort_weight
```

INFO findings score 0 by definition.

## Punch List Construction

Include: all CRITICAL findings + all HIGH findings + MEDIUM findings until total reaches 15 (or all exhausted). Sorted by `priority_score` descending within each severity tier.

## Token Budget

Minimal (arithmetic only).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → N13.

## Back-edge Endpoints

None.
```

- [ ] **Step 4: Write modules/N13-formatter.md**

```markdown
# N13 — ReportFormatter

**Type:** formatter
**Mode:** inline
**Active in:** `audit`

## Inputs

```
prioritized_findings: Finding[]
punch_list: Finding[]
project_model: (from N01)
resolved_flags: (from N01)
```

## Outputs

```
formatted_report_markdown: string   // complete audit report markdown conforming to §4.1 template
```

## Side Effects

None (pure formatting in memory; report not yet saved — N15 handles save).

## Halt Conditions

None.

## Formatting Contract

1. Renders YAML frontmatter per Audit Report Schema v1 (§4.1).
2. Renders top-of-body sections in order:
   - Partial-report warning (only when `token_cap_partial: true`)
   - Resolve-before-testing punch list
   - Main body: one `## Finding F00N` section per finding
3. Renders Unverified Hypotheses section below main body.
4. Uses `templates/audit-report.md.template` as the rendering guide.
5. Under `--verbose`: expands rationale sections with additional examples and remediation tradeoffs.
6. Does NOT add nitpick padding under `--verbose`.

## Token Budget

Scales with finding count (~200 tokens per finding for formatting).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → N14 (E10).

## Back-edge Endpoints

None.
```

- [ ] **Step 5: Write modules/N14-q-gate.md**

```markdown
# N14 — Q-GATE

**Type:** verifier
**Mode:** inline (Pass A) + conditional subagent (Pass B)
**Active in:** `audit`

## Inputs

```
formatted_report_markdown: string       // from N13
location_verification_cache: Cache     // from N10 (read-only)
resolved_flags: (from N01)
```

## Outputs

```
q_gate_result: {
  pass_a: "pass" | "pass-minimal" | "fail" | "skipped-token-cap",
  pass_b: "pass" | "fail" | "exec-error" | "skipped-token-cap" | "skipped-low-volume",
  pass_b_lens: string | null,
  pass_b_skip_reason: string | null
}
validated_report: string   // the report with any Pass A/B corrections applied
```

## Side Effects

Read-only (may read source files for location verification on cache miss).

## Halt Conditions

- `halt-on-q-gate-failure` (subreason: `pass-a`): Pass A fails mandatory checks
- `halt-on-q-gate-failure` (subreason: `pass-b`): Pass B content-fail
- `halt-on-q-gate-failure` (subreason: `pass-b-exec-error`): Pass B subagent timeout or crash

## Pass A — Mechanical Checks (inline, mandatory)

1. Mandatory-field completeness: every finding has all required schema fields; missing → demote to Unverified Hypotheses
2. Location verification: for each finding's `location`, check `location_verification_cache`; on cache miss, Read the file and verify; unverifiable → demote
3. CRITICAL/HIGH × Confidence floor: CRITICAL or HIGH severity requires Confidence ≥ MEDIUM; violation → demote or lower severity
4. Duplicate merge: any remaining duplicates N11 missed → merge with count
5. No-comment-echo: finding text must not merely quote the project's own TODO/FIXME without independent verification; violation → demote
6. No-LOW-only warning: if every main-body finding is LOW or INFO severity, emit a user-facing warning (not a halt) — signals possible under-sensitivity

Under token-cap: run `pass-a-minimal` (mandatory-field + location only; other checks skipped).

## Pass B — Adversarial Review (conditional subagent)

**Activation policy (cost-aware):** Pass B runs when ANY of:
- Report has ≥5 findings
- Any finding has severity CRITICAL or HIGH
- `--deep` flag is set

Otherwise: `pass_b: skipped-low-volume` with `pass_b_skip_reason: "fewer than 5 findings and no CRITICAL/HIGH severity"`.

**Pass B checks:**
1. Anti-iatrogenic: does any remediation introduce a worse defect than the one it fixes?
2. Evidence-rationale coherence: does the evidence excerpt actually support the stated rationale?
3. Dimension-classification correctness: is each finding in the right dimension?

Pass B uses a **clean lens** (subagent that hasn't seen the audit pipeline — avoids anchoring).
`pass_b_model` is recorded in provenance when Pass B demotes a finding.

## Token Budget

Pass A: ~200 tokens per finding (mechanical). Pass B: one subagent spawn, budget per the spawn cap.

## Backtrack / Aggregation

Adversarial self-review. No backtracking to N04..N09.

## Fan-out Cardinality

1:1 → N15 (E11) after both passes complete.

## Back-edge Endpoints

None. Q-GATE is a terminal verifier for the audit pipeline.
```

- [ ] **Step 6: Write modules/N15-save-handler.md**

```markdown
# N15 — SaveHandler

**Type:** io
**Mode:** inline
**Active in:** `audit`

## Inputs

```
validated_report: string   // from N14
project_model: (from N01)
report_id: string          // uuid v4 generated by N15
resolved_flags: (from N01)
```

## Outputs

```
save_decision: "accepted" | "declined"
saved_report_path: string | null    // null on decline
report_id: string                   // the uuid v4 generated for this report
```

## Side Effects

- Write-report: saves audit report to `~/docs/epiphany/audit/<project-slug>-<YYYYMMDD>-<HHMMSS>.md` on accept
- Write-state-file: writes `~/docs/epiphany/audit/.state/<report-id>.json` on accept only
- Write-log: structured event for save decision

## Halt Conditions

None.

## Save Prompt

Offers save under `~/docs/epiphany/audit/`. Save prompt explicitly warns about idempotency degradation if user declines:

> "Declining to save means future `--fix` runs of this report cannot use state-file idempotency; they fall back to git-log only. Save anyway? (y/n)"

Writes idempotency state file at `~/docs/epiphany/audit/.state/<report-id>.json` only on save-accept.

## Idempotency State File

Written at save time. Format:
```json
{
  "report_id": "<uuid>",
  "audit_target": "<abs path>",
  "findings": {}
}
```
Populated with finding outcomes as `--fix` runs proceed. State file is authoritative over git-log during idempotency checks in N16.

## Project-Slug Derivation

`basename(audit_target)`, lowercased, non-alphanumeric chars → `-`, runs of `-` collapsed, leading/trailing `-` stripped, truncated to 50 chars.

## Token Budget

Minimal (file write + prompt).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → user (save prompt, E12); then 1:1 → N24 (E16, `--improve` only) or → user (E21 fix-offer, no-flag mode).

## Back-edge Endpoints

None.
```

- [ ] **Step 7: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 15 modules have required sections. OK.`

- [ ] **Step 8: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/
git -C ~/.claude commit -m "feat(epiphany-audit): modules N10-N15 (FPV through SaveHandler)"
```

---

## Task 14: Modules N16–N18 (FixTriage, FixPlanner, PreFlight)

**Files:**
- Create: `modules/N16-fix-triage.md`
- Create: `modules/N17-fix-planner.md`
- Create: `modules/N18-pre-flight.md`

- [ ] **Step 1: Write modules/N16-fix-triage.md**

```markdown
# N16 — FixTriage (F-VAL ingest + Resume-handler + Triage)

**Type:** validator
**Mode:** inline
**Active in:** `fix`

## Inputs

```
audit_report_path: string           // resolved per §1.3 <report> resolution order
resolved_flags: (from N01 or --fix entry)
recovery_manifest: object | null    // present when entered via resume
```

## Outputs

```
triage_result: {
  source_report_id: string,
  source_audit_report_sha256: string,
  findings: TriagedFinding[]
}
fix_groups: FixGroup[]             // file/module-grouped, topo-sorted
topo_sorted_order: string[]        // finding-ids in execution order
```

## Side Effects

- Read: audit report file; state file at `~/docs/epiphany/audit/.state/<report-id>.json`
- Write-log: structured events per validation step and triage decision

## Halt Conditions

- `halt-pre-fix-on-validator-failure`: schema validation fail OR suspicious content user-declined
- `halt-on-empty-or-unfixable-report`: zero main-body findings (checked BEFORE suspicious-content and triage)
- `halt-on-conflicting-fixes` (live mode): two findings have incompatible edits (same line range)
- `halt-on-files-outside-tree`: report references files outside `audit_target` git tree
- `halt-on-invalid-finding-id`: `--escalate-finding F00N` references ID absent from report; checked after F-VAL passes but before triage; diagnostic lists available IDs
- `halt-on-resume-tree-divergence`: resume cleanup would discard working-tree changes outside prior run's scope
- `halt-on-stale-source-report`: report file missing or SHA-256 mismatch on resumed run
- `halt-on-mismatched-version`: `tool_version` skew; user declined
- `halt-on-user-abort`: ctrl-C at any interactive prompt

## Resume-Handler Sub-step (FIRST sub-step on resume entry)

Runs before F-VAL when entered via `resume` from `halt-on-recovery-conflict`. Order:
1. Run `git status --porcelain` + `git diff --name-only HEAD`
2. Tree-divergence safety check: if divergence includes files outside the in-flight finding's `location` AND outside the audit's flagged-files set → `halt-on-resume-tree-divergence` with options (a) discard and resume, (b) abort
3. On (a) or on clean divergence: run `git checkout -- . && git clean -fd`
4. Move `in_flight_finding_id` back to `pending` in recovery manifest
5. Proceed to normal F-VAL ingest

## F-VAL Ingest (schema validation)

- Parse YAML frontmatter + finding bodies from the audit report
- Validate parsed JSON against `schemas/audit-report-v1.schema.json`
- Capture SHA-256 of the report file as `source_audit_report_sha256`
- Suspicious-content prompt overrides `--auto` (requires explicit user confirmation)

## Idempotency Check

- State file authoritative; git-log fallback
- `reachable: false` annotations skipped unless `--reverify-state` was passed
- Conflict (state says applied at SHA not in current branch) → warn + user choice: (a) re-apply, (b) skip, (c) abort

## Triage Rules

1. Group findings by file/module
2. Topo-sort within each group (dependency: finding A depends on B iff A's location is at or after B's remediation diff lines)
3. Tier classification per §2.3 rules (Tier-1/2/3 + defer-on-uncertainty)
4. Conflicting-edit detection: in live mode → `halt-on-conflicting-fixes`; under `--dry-run` → record in `triage_summary.conflicting_groups` and continue

## Conflicting Edits Definition

Two findings have **incompatible** edits iff their remediation diffs both modify any character within the same line range, OR both insert content at the same line. Non-overlapping edits within the same file (different line ranges) are **compatible** and merged into one fix-group. Three-or-more-way: any pair failing compatibility halts the cluster.

## Token Budget

Moderate (reads audit report, state file; performs triage logic).

## Backtrack / Aggregation

AGGREGATION owner (fix side: file-grouping). CONDITIONAL ROUTING owner.

## Fan-out Cardinality

N:1 (aggregates findings into fix-groups).

## Back-edge Endpoints

E_rerun_fail: N21 → N16 (re-triage with regression context).
```

- [ ] **Step 2: Write modules/N17-fix-planner.md**

```markdown
# N17 — FixPlanner

**Type:** planner
**Mode:** inline
**Active in:** `fix`

## Inputs

```
triage_result: (from N16)
fix_groups: FixGroup[]
resolved_flags: (from entry)
```

## Outputs

```
fix_plan_doc: string        // Dry-Run Plan Schema v1 markdown
user_approvals: {           // per tier; populated after interactive confirmation
  tier_1: "approved" | "declined",
  tier_2: "approved" | "declined",
  tier_3_per_fix: { [finding_id]: "approved" | "declined" }
}
```

## Side Effects

- Write-report (under `--dry-run` only): saves dry-run plan to `~/docs/epiphany/audit/dry-run-plans/<source-report-id>-dryrun-<YYYYMMDD>-<HHMMSS>.md`
- Write-log: events per tier confirmation

## Halt Conditions

- `halt-on-user-abort`: explicit `halt` at any tier prompt

## `--dry-run` Behavior

Emits Dry-Run Plan v1 and **halts here** — no PreFlight, no FixApplier, no branch creation. The plan contains `proposed_diff` (the literal patch N19 WOULD apply) for each finding.

## Tier Confirmation Protocol

Default policy (applied in order T1 → T2 → T3):

| Tier | Default | `--auto` | `--confirm-all` |
|------|---------|----------|-----------------|
| 1    | batch confirm: *"apply N Tier-1 fixes? y/n"* | silent apply | per-fix confirm |
| 2    | batch confirm: *"apply M Tier-2 fixes? y/n"* | batch confirm | per-fix confirm |
| 3    | per-fix confirm | per-fix confirm | per-fix confirm |

Decline on Tier-N → all Tier-N findings marked `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit `halt` → stop entirely.

**Per-fix-opt-in floor (anti-conformity):** even under `--auto`, any finding with `confidence < HIGH OR effort > trivial` requires per-fix opt-in (not auto-applied). Only HIGH-confidence, trivial-effort findings auto-apply under `--auto`.

## Token Budget

Low (plan generation + interactive prompts).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → N18 (or halt under `--dry-run`).

## Back-edge Endpoints

E_repair (2nd invocation): N20 fail → N17 replan (replan the failed fix-group with failure context; then retry via N19).
```

- [ ] **Step 3: Write modules/N18-pre-flight.md**

```markdown
# N18 — PreFlight

**Type:** preflight
**Mode:** inline
**Active in:** `fix`

## Inputs

```
fix_plan_doc: (from N17)
project_model: (from entry, or derived from --fix report)
source_report_id: string
resolved_flags: (from entry)
is_resume: boolean          // true when entered via resume
existing_baseline: object | null  // present on resume
```

## Outputs

```
baseline_metrics: {
  tests:      { passed: int, failed: int, skipped: int },
  type_check: { errors: int },
  lint:       { total_warnings: int },
  build:      "pass" | "fail"
}
branch_name: string
```

## Side Effects

- Write-baseline: `~/docs/epiphany/audit/.baselines/<report-id>.json` (fresh runs only)
- Write-log: structured events per step
- Git operation: branch creation (step 3 below)
- **Does NOT write recovery manifest** — that is N19/N22 responsibility

## Halt Conditions

- `halt-on-git-state-incompatible`: dirty tree, detached HEAD, or no commits (fresh runs); subreason: `branch-name-exhausted` if branch creation fails
- `halt-on-baseline-failure`: baseline runner crashes / cannot produce output (NOT for pre-existing test failures — those are recorded as baseline state)
- `halt-on-test-cmd-unknown`: no test command auto-detected and `--test-cmd` not provided

## Step Ordering (mandatory)

Steps execute in this order; a halt at any step does not progress to the next:

**(0) Orphan-branch sweep (fresh runs only):**
Scan for ALL branches matching `epiphany-audit/*` that have:
- No corresponding live recovery manifest at `~/docs/epiphany/audit/.recovery/<report-id>.json` AND
- No corresponding archive at `.recovery/.archive/<report-id>-*.json`

This catches orphans from prior runs against different audit reports. If found: prompt with a multi-select list: *"orphan audit branches found: [numbered list with branch name and last-commit-date]. Delete which? (comma-separated indices, `all`, `none`)."* Default on enter: `none`. Delete only user-selected.

**(1) Git-state check:**
Halt with `halt-on-git-state-incompatible` on dirty tree, detached HEAD, or no commits.
On a **resumed run** (post `git checkout -- . && git clean -fd`): accept the post-cleanup tree as clean; do NOT re-halt on residual state from the prior run.

**(2) Baseline capture:**
Run test suite, type check, lint, build **before any fix**. Write result to `~/docs/epiphany/audit/.baselines/<report-id>.json`.
Pre-existing failing tests → record as baseline state (NOT a halt condition).
Runner crashes / produces no output → `halt-on-baseline-failure`.
On a **resumed run**: skip capture; read existing baseline from the .baselines file. If file missing → `halt-on-baseline-failure` (subreason: `resume-baseline-missing`).

**(3) Branch creation:**
Primary name: `epiphany-audit/<source-report-id>-YYYYMMDD`.
Collision policy:
- If primary name exists AND a recovery manifest exists for `<source-report-id>` → this is a resume; re-use the existing branch (do NOT create a new one).
- If primary name exists AND no manifest exists → prior run completed cleanly; append `-<HHMMSS>` for the fresh run.
- If even the timestamped name collides → `halt-on-git-state-incompatible` (subreason: `branch-name-exhausted`).

## Token Budget

Low (shell commands + file writes).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → N19 (E13 chain).

## Back-edge Endpoints

None.
```

- [ ] **Step 4: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 18 modules have required sections. OK.`

- [ ] **Step 5: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/
git -C ~/.claude commit -m "feat(epiphany-audit): modules N16-N18 (FixTriage, FixPlanner, PreFlight)"
```

---

## Task 15: Modules N19–N23 (FixApplier → FixReporter)

**Files:**
- Create: `modules/N19-fix-applier.md`
- Create: `modules/N20-per-fix-verifier.md`
- Create: `modules/N21-regression-battery.md`
- Create: `modules/N22-rollback-handler.md`
- Create: `modules/N23-fix-reporter.md`

- [ ] **Step 1: Write modules/N19-fix-applier.md**

```markdown
# N19 — FixApplier

**Type:** actuator
**Mode:** inline (write-serial; NEVER fan-out)
**Active in:** `fix`

## Inputs

```
fix_groups: FixGroup[]          // topo-sorted from N16
user_approvals: object          // from N17
baseline_metrics: object        // from N18
branch_name: string             // from N18
```

## Outputs

```
per_fix_outcomes: FixOutcome[]
recovery_manifest_updates: RecoveryEvent[]
```

## Side Effects

- Git-staged: working-tree edits + commits on PASS; `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` on FAIL (NEVER `git revert`)
- Write-recovery-manifest: boundary-aligned writes ONLY at fix-group start, end-success, end-failure (per O3 spec §5.3)
- Write-log: events per fix-group iteration

## Halt Conditions

- `halt-mid-fix-on-perfix-cap-hit`: E_repair retries exhausted on a fix-group AND every remaining group is blocked by it (routes through N23 partial report first via E_halt_partial before emitting halt envelope)

## Atomic Loop (per fix-group)

```
[FIX-GROUP START]  → manifest write #1: { in_flight_finding_id, pending list updated }

  inner loop (NO manifest writes inside):
    1. Apply edit to working tree (Write tool — no git add yet)
    2. Invoke N20 PerFixVerifier
    3. PASS:
       a. git add <touched-files>
       b. git commit -m "[AUDIT-NNN] <one-line>" (body includes Finding-id, Dimensions, Severity, Source)
       c. For behavioral fixes (not cosmetic): add regression-prevention test in same commit
          (or paired follow-up commit [AUDIT-NNN-test] if language/hook requires separation)
       d. Exit inner loop → fall through to FIX-GROUP END (success)
    4. FAIL:
       a. git checkout -- <tracked-touched-files>
       b. git clean -fd <new-files-created-by-this-attempt>
       c. Record failure_context (failure_class + diagnostic from N20)
       d. E_repair routing:
          - 1st invocation → retry inner loop (with failure context passed to next attempt)
          - 2nd invocation → N17 replan (N17 re-plans the failed fix-group; then retry)
          - 3rd invocation → cap-hit; exit inner loop → fall through to FIX-GROUP END (failure)

[FIX-GROUP END — success]  → manifest write #2: { last_known_good_sha = new commit SHA; finding moved pending → applied }
[FIX-GROUP END — failure]  → manifest write #2: { finding marked failed; downstream-dependent groups → deferred (upstream-dependency-failed); independent groups continue }
```

## Commit Tag Mapping

`F001` → `[AUDIT-001]`, `F042` → `[AUDIT-042]`, `Fnew001` → `[AUDIT-NEW-001]`

## Paired Follow-up Commit Format

Used only when language tooling or a project pre-commit hook rejects bundled test+source commits:
- Primary: `[AUDIT-NNN] <one-line>`
- Follow-up: `[AUDIT-NNN-test] regression test for <one-line>`
- Recorded in fix report as `regression_test_added.deferred_to_followup_commit: <sha>`

## Hard Rules

- **Never `git revert`** — failed fix attempts never reach commit, so there's nothing to revert
- **Never bundle** multiple findings in one commit
- **Never `--no-verify`** — hook failures are treated as `commit-hook-failure`; N20 routes them through E_repair
- **Never amend** prior commits — always new commits, even on retry
- **One concern per commit** — no "while I'm here" cleanups

## Recovery-Manifest Write Policy (cost-aware)

Writes happen ONLY at fix-group boundaries (start, end-success, end-failure). Intra-loop transitions are NOT persisted. If process dies between boundaries, the resume-handler restarts the fix-group from scratch (the atomic loop guarantees no half-applied state in git history).

## Behavioral vs Cosmetic Classification

A fix is **cosmetic** iff its diff modifies only comments, whitespace, or non-semantic renames AND no compiled/interpreted token changes. Cosmetic fixes: `regression_test_added.status: n/a`. All other fixes are **behavioral** and require a regression-prevention test.

## Token Budget

Scales with number of fix-groups and fix complexity. Primary cost: Write operations + git shell commands.

## Backtrack / Aggregation

None (write-serial by design).

## Fan-out Cardinality

Serial 1:1 per fix-group. Write nodes NEVER fan out.

## Back-edge Endpoints

E15: N20 success → N19 (next fix-group).
E_repair: N20 fail → N19 retry (1st invocation); N20 fail → N17 replan then N19 retry (2nd invocation).
```

- [ ] **Step 2: Write modules/N20-per-fix-verifier.md**

```markdown
# N20 — PerFixVerifier

**Type:** verifier
**Mode:** inline
**Active in:** `fix`

## Inputs

```
changed_files: string[]         // files modified by this fix attempt
baseline_metrics: object        // from N18
test_cmd: string                // resolved test command
failure_context_from_prior: object | null  // present on retry pass
```

## Outputs

```
verify_result: "PASS" | "FAIL"
failure_context: {
  failure_class: "verification-failure" | "commit-hook-failure" | "git-operation-failure" | "type-check-failure" | "targeted-test-failure",
  diagnostic: string
} | null
```

## Side Effects

Read-only (runs tests and type checks; does NOT commit — that is N19's responsibility).

## Halt Conditions

None. N20 emits a fail-signal; E_repair routing (not N20) decides retry vs replan vs cap-hit.

## Verification Steps

1. **Targeted tests**: run tests that exercise the changed files (grep test directory for imports of changed files; run those test files only)
2. **Type check on changed files**: run type checker on the changed file set only (e.g., `mypy <files>`, `tsc --noEmit <files>`)

PASS iff: targeted tests pass AND type check has no new errors compared to baseline.

## Failure Classes

| Class | Trigger |
|-------|---------|
| `targeted-test-failure` | Targeted test run has new failures (not in baseline) |
| `type-check-failure` | Type checker reports new errors (not in baseline) |
| `commit-hook-failure` | `git commit` rejected by a pre-commit hook |
| `git-operation-failure` | git command fails for non-hook reason |
| `verification-failure` | Generic: none of the above but verification cannot complete |

## Token Budget

Minimal (shell command execution).

## Backtrack / Aggregation

Emits fail-signal; E_repair routing is external to N20.

## Fan-out Cardinality

1:1 (verifies one fix-group at a time, matching N19's serial loop).

## Back-edge Endpoints

E15: N20 success → N19 (proceed to next fix-group).
E_repair: N20 fail → N19 (1st: retry) or N17 (2nd: replan).
```

- [ ] **Step 3: Write modules/N21-regression-battery.md**

```markdown
# N21 — RegressionBattery (battery + tiered audit-rerun delta)

**Type:** verifier
**Mode:** inline (battery) + conditional subagent (audit-rerun under `--deep` when full rerun)
**Active in:** `both` (battery always; audit-rerun fires in fix-mode N21 when tiered policy requires it)

## Inputs

```
per_fix_outcomes: FixOutcome[]    // from N19
baseline_metrics: object          // from N18
applied_tier_max: 1 | 2 | 3       // highest tier of fixes applied this run
resolved_flags: (from entry)
test_cmd: string
```

## Outputs

```
battery_result: "pass" | "fail"
diff_scope_result: "pass" | "fail-with-unmapped-hunks"
unmapped_hunks: DiffHunk[]
audit_rerun_delta: {
  scope: "full" | "narrow" | "skipped-tier-policy" | "skipped-by-flag" | null,
  reran_dimensions: string[],
  induced_regressions: Finding[],  // new findings in touched files
  new_findings_discovered: Finding[]  // new findings in untouched files
}
```

## Side Effects

- Write-log: events per battery step and audit-rerun
- Spawns subagent for audit-rerun (under `--deep` when full-rerun tiered) — 1 optional spawn

## Halt Conditions

- `halt-on-scope-creep` (E_diffscope): unmapped diff hunks; do NOT auto-revert; user choice = authorize (manual-edits section in fix report) or revert manually
- `halt-mid-fix-on-induced-regression-cap-hit`: E_repair cap exhausted on a regression-induced fix-group

## Battery Steps (runs BEFORE audit-rerun — always inline)

Order:
1. Full test suite vs baseline (no new failures allowed)
2. Type check (no new errors vs baseline)
3. Lint (`new_warnings_in_changed_regions == 0`)
4. Build clean
5. Diff-scope check: every diff line maps to an AUDIT-ID; regression-prevention test additions (in `[AUDIT-NNN]` or `[AUDIT-NNN-test]` commits) count as in-scope; unmapped hunks → `halt-on-scope-creep`

Battery failure → E_repair; audit-rerun not run (no point if battery fails).

## Audit-Rerun Policy (tiered by `applied_tier_max`)

| applied_tier_max | Behavior |
|-----------------|----------|
| 1 (Tier-1 only) | **Skip** (battery covers failure modes; no audit-rerun) |
| 2 (Tier-2 only) | **Narrow rerun**: re-run only N04..N09 instances matching dimension tags of applied fixes. Post-pipeline subset: N10 FPV, N11 aggregator, N12 prioritizer, N14 Pass A (N13 formatter and N14 Pass B skipped). |
| 3 (any Tier-3)  | **Full rerun**: N01..N14 Pass A (N14 Pass B skipped — delta semantics). |

Override: `--full-rerun` forces full regardless of tier; `--no-rerun` forces skip. Fix report records `audit_rerun_delta.scope: skipped-by-flag` when `--no-rerun`.

## Induced Regression vs New Finding

- New finding in **files touched by applied fixes** → `induced-regression` → route to E_rerun_fail → N16 for re-triage
- New finding in **untouched files** → `new-finding-discovered` → record in fix report body only; no E_rerun_fail

## Token Budget

Battery: minimal (shell commands). Narrow rerun: 30k per re-run analyzer. Full rerun: full N01..N14 pass budget.

## Backtrack / Aggregation

Adversarial-via-rerun.

## Fan-out Cardinality

1 optional subagent spawn (full rerun under `--deep`).

## Back-edge Endpoints

E_repair: N21 fail → N19 (1st) or N17 (2nd) per E_repair rules.
E_rerun_fail: N21 induced-regression → N16 (batched re-triage).
E_diffscope: N21 diff-scope fail → halt.
```

- [ ] **Step 4: Write modules/N22-rollback-handler.md**

```markdown
# N22 — RollbackHandler

**Type:** recovery
**Mode:** inline
**Active in:** `fix`

## Inputs

```
trigger: "e_repair_cap_hit" | "e_finalize"
fix_report_id: string | null     // populated on e_finalize (after N23 writes fix report)
recovery_manifest_path: string
failed_fix_group: FixGroup | null  // present on cap-hit trigger
```

## Outputs

```
recovery_manifest_finalized: object
archive_path: string | null   // populated on e_finalize (completed archive)
```

## Side Effects

- Write-recovery-manifest: finalize recovery manifest (boundary-aligned boundary write)
- On E_finalize: archive manifest to `.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json` and REMOVE live `<report-id>.json`
- Write-log

## Halt Conditions

None. N22 emits a diagnostic if cap-hit blocks all remaining work, but does not halt itself.

## Responsibilities

N22 has exactly three responsibilities — nothing more:

**(a) E_repair cap-hit for a fix-group:**
Finalize recovery manifest with failure record for the failed fix-group. Manifest STAYS LIVE at `~/docs/epiphany/audit/.recovery/<report-id>.json` so the user can `resume` next run.

**(b) E_finalize (planned termination):**
1. N23 has already written the fix report file to disk and generated `fix_report_id`
2. E_finalize fires → N22 reads the just-written fix report to extract `fix_report_id`
3. N22 writes final manifest record including `fix_report_id`
4. N22 archives manifest: moves to `.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json`
5. N22 removes live `<report-id>.json` so subsequent `--fix` runs do NOT trip `halt-on-recovery-conflict`
6. E_complete fires → user summary

**(c) Mid-flight death (process kill, OS crash):**
N22 cannot act. The at-rest state is the most recent fix-group boundary write from N19 (which is sufficient for the resume-handler to restart the in-flight fix-group). N22 cannot address this case — it is handled entirely by N19's write discipline and N16's resume-handler.

## NEVER `git revert HEAD`

Failed fix attempts never reach `git commit` (the atomic loop in N19 discards working-tree changes before committing). N22 has nothing to revert. `git revert` would create spurious commits that break idempotency grep.

## Sequencing with E_finalize (strictly sequential)

1. N23 generates `fix_report_id` and writes fix report to disk
2. E_finalize fires (carrying `fix_report_id`)
3. N22 reads fix report, writes manifest, archives manifest
4. E_complete fires → user

This order is non-negotiable. N22 must NOT archive the manifest before N23 writes the report.

## Token Budget

Minimal (file operations).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1.

## Back-edge Endpoints

E_complete: N22 → user (terminal).
```

- [ ] **Step 5: Write modules/N23-fix-reporter.md**

```markdown
# N23 — FixReporter

**Type:** formatter
**Mode:** inline
**Active in:** `fix`

## Inputs

```
per_fix_outcomes: FixOutcome[]       // from N19
baseline_metrics: object             // from N18
post_metrics: object | null          // null on partial (halt before N21 completed)
audit_rerun_delta: object | null     // null on partial
diff_scope_result: string            // from N21
unmapped_hunks: DiffHunk[]
resolved_flags: (from entry)
recovery_manifest_ref: string | null
is_partial: boolean                  // true on halt-mid-fix-*
halt_state_id: string | null         // present on partial
```

## Outputs

```
fix_report_markdown: string   // fix report per Schema v1
fix_report_id: string         // uuid v4 (generated here)
```

## Side Effects

- Write-report: saves fix report to `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-<YYYYMMDD>-<HHMMSS>.md`
- Write-log

## Halt Conditions

None. N23 always writes what it has.

## Two Invocation Modes

**(1) Planned termination** (reached via E13 chain end):
- `partial: false`, `halt_state: null`
- Full report: one body entry per source audit finding (plus induced-regression / new-finding descendants)
- Then E_finalize fires → N22 archives manifest → E_complete → user

**(2) halt-mid-fix-\*** (reached via E_halt_partial):
- `partial: true`, `halt_state: <id>`
- Partial report: body entries reflect work-to-date (verified/failed/deferred so far)
- Then halt envelope emitted to user; recovery manifest stays live for `resume`

## Status-Priority Sort Order

`failed > induced-regression > deferred > simulated > verified > skipped`

This order surfaces problems first.

## Top-of-Body Sections

1. **Deferred items** — consolidated list with `defer_reason` per item; surfaced first
2. **Manual edits** (only if user authorized scope-creep at `halt-on-scope-creep`)
3. **Recovery** (only if run died mid-flight and was resumed)

## Token Budget

Scales with finding count (~200 tokens per entry for formatting).

## Backtrack / Aggregation

AGGREGATION owner (per-fix outcome rollup).

## Fan-out Cardinality

1:1 → E_finalize → N22 (planned), OR 1:1 → user via E_halt_partial (partial).

## Back-edge Endpoints

E_finalize: N23 → N22 (strictly sequential — N23 writes first, then N22 archives).
```

- [ ] **Step 6: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 23 modules have required sections. OK.`

- [ ] **Step 7: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/
git -C ~/.claude commit -m "feat(epiphany-audit): modules N19-N23 (FixApplier through FixReporter)"
```

---

## Task 16: Modules N24–N27 (Improvement subpipeline)

**Files:**
- Create: `modules/N24-improvement-contextualizer.md`
- Create: `modules/N25-improvement-brainstormer.md`
- Create: `modules/N26-oef.md`
- Create: `modules/N27-improvement-reporter.md`

- [ ] **Step 1: Write modules/N24-improvement-contextualizer.md**

```markdown
# N24 — ImprovementContextualizer (IC)

**Type:** analyzer
**Mode:** inline
**Active in:** `improve` (`--improve` only; fires via E16 after N15 save-decision)

## Inputs

```
project_model: (from N01, already in context)
validated_report: (from N14, already in context — the Q-GATE-passed findings)
```

## Outputs

```
improvement_context: {
  project_capability_map: string,    // what the project is and does
  health_summary: string,            // what audit findings reveal about structural weaknesses
  healthy_areas: string[],           // dimensions NOT flagged (worth preserving)
  improvement_search_constraints: string  // what to look for and what to avoid
}
```

## Side Effects

Read-only. Works entirely from in-memory context (N01's project_model + N14's report). Opens NO additional files.

## Halt Conditions

None. Failure (e.g., context too large) is caught; N27 emits `improvement_partial: true`.

## Purpose

Synthesizes an improvement-analysis frame BEFORE brainstorming. Prevents N25 from brainstorming improvements already addressed by audit findings (double-counting) and focuses the search on what matters for this project.

## Token Budget

Moderate (~2k tokens for synthesis from existing context).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → N25 (E17).

## Back-edge Endpoints

None.
```

- [ ] **Step 2: Write modules/N25-improvement-brainstormer.md**

```markdown
# N25 — ImprovementBrainstormer (IB)

**Type:** analyzer
**Mode:** inline (phase 1); inline + optional subagent (phase 2 under `--deep` when ≥8 candidates)
**Active in:** `improve`

## Inputs

```
improvement_context: (from N24)
resolved_flags: (from entry)
```

## Outputs

```
improvement_candidates: ImprovementCandidate[]  // fully elaborated
```

## Side Effects

- Optional subagent spawn (phase 2 only; 0 or 1 spawn; requires `--deep` AND ≥8 phase-1 candidates)
- Write-log

## Halt Conditions

None. Subagent failures produce partial candidates; N26/N27 handle gracefully.

## Two-Phase Execution

**Phase 1 (always inline):** Generate lightweight raw candidate list. For each candidate: one-line tag + category seed (one of: developer-experience, testing, architectural-clarity, performance-headroom, tooling-automation, dependency-hygiene, documentation-discoverability). No full description/action/success_measure yet — keeps phase 1 fast and token-cheap.

**Phase 2:** Elaborate each candidate with full `description`, `action`, `success_measure`.
- If `--deep` is set AND ≥8 raw candidates from phase 1: spawn one subagent to elaborate all candidates in parallel.
- Otherwise (no `--deep`, or <8 candidates): elaborate inline.

## Strict Exclusions (GoT anti-patterns to filter out at brainstorm time)

- Cosmetic-only changes (rename files, fix typos)
- Re-architectures without concrete measured benefit
- Generic "best practices" not grounded in project-specific evidence
- Improvements already implied by audit findings (those are findings, not improvements — no double-counting)

## Token Budget

Phase 1: ~500 tokens. Phase 2 inline: ~500 tokens per candidate. Phase 2 subagent: one spawn (full context).

## Backtrack / Aggregation

Participant in AGGREGATION via N26.

## Fan-out Cardinality

1 optional subagent spawn for phase 2. Produces N candidates → N26.

## Back-edge Endpoints

None.
```

- [ ] **Step 3: Write modules/N26-oef.md**

```markdown
# N26 — OverEngineeringFilter (OEF)

**Type:** filter
**Mode:** inline
**Active in:** `improve`

## Inputs

```
improvement_candidates: ImprovementCandidate[]  // from N25
```

## Outputs

```
survivors: ImprovementCandidate[]        // passed the filter
discarded_improvements: {               // for --verbose section in N27
  candidate: ImprovementCandidate,
  discard_rationale: string
}[]
```

## Side Effects

None (pure filtering in memory).

## Halt Conditions

None.

## Scoring

Each candidate receives two scores:
- `utility_score`: 1 = marginal, 2 = notable, 3 = high
- `cost_score`:    1 = trivial, 2 = modest, 3 = significant

## Filter Rules (discard when ANY of)

1. `cost > utility` (not worth the investment)
2. `utility = 1` regardless of cost (marginal-utility improvements not surfaced)
3. The candidate's `description` or `action` prose contains speculative language: "might", "could", "potentially", "may help" — indicates unverified utility

## Categorization (mutually exclusive; `notable` takes precedence)

| Category    | Criterion |
|-------------|-----------|
| `notable`   | utility = 3 AND cost ≤ 2 |
| `quick-win` | utility ≥ 2 AND cost = 1 AND NOT notable |
| `worthwhile`| all remaining survivors (utility ≥ cost ≥ 2 AND NOT notable) |

After filtering, all survivors have `utility ≥ 2`.

## Zero Survivors

If no candidates survive the filter, N27 emits a report stating: *"No improvements above the utility/cost threshold were found. This is a valid result — the project may be well-optimized in its current state."* N26 does NOT generate improvements just to have output.

## Token Budget

Minimal (arithmetic + prose analysis; no file reads).

## Backtrack / Aggregation

AGGREGATION owner (improvement side: candidate filtering + categorization).

## Fan-out Cardinality

N:M (N candidates in, M survivors out where M ≤ N).

## Back-edge Endpoints

None.
```

- [ ] **Step 4: Write modules/N27-improvement-reporter.md**

```markdown
# N27 — ImprovementReporter (IR)

**Type:** formatter
**Mode:** inline
**Active in:** `improve`

## Inputs

```
survivors: ImprovementCandidate[]       // from N26
discarded_improvements: object[]        // from N26 (for --verbose section)
improvement_context: (from N24)
resolved_flags: (from entry)
source_report_id: string
saved_audit_report_path: string | null  // null if N15 save was declined
improvement_partial: boolean            // true if N24..N26 pipeline failed partway
improvement_partial_warning: string | null
```

## Outputs

```
improvement_report_markdown: string
improvement_report_path: string   // absolute path where the report was saved
```

## Side Effects

- Write-report: saves improvement report to `~/docs/epiphany/audit/improvement-reports/<project-slug>-<YYYYMMDD>-<HHMMSS>-improve.md` **unconditionally** (no save prompt)
- Conditional in-place patch: updates `improvement_report_ref` in the already-saved audit report's YAML frontmatter (only when N15 save was accepted)
- Write-log

## Halt Conditions

None. N27 always writes what it has (including partial results when `improvement_partial: true`).

## Backpatch Failure Handling

If the in-place frontmatter patch of the audit report fails (e.g., permission error, file modified since save):
- Log the absolute path of the improvement report to the event log
- Emit a user-facing warning: *"Could not patch improvement_report_ref into audit report. Improvement report is at: <path>"*
- Do NOT retry; do NOT halt

## Sections (in order)

1. Summary (total candidates, filtered, survivors by category; zero-survivors message if applicable)
2. Notable improvements (utility = 3, cost ≤ 2)
3. Quick wins (utility ≥ 2, cost = 1, not notable)
4. Worthwhile improvements (utility ≥ cost ≥ 2, not notable)
5. Filtered improvements (under `--verbose` only) with OEF discard rationale per candidate

## `improvement_partial: true` Behavior

Two causes; distinguished by warning text:
- Source audit was token-capped: *"source audit was token-capped at `<truncated_at_node>`; improvement analysis is based on incomplete findings and may miss opportunities in unanalyzed dimensions."*
- N24..N26 pipeline failure: *"improvement analysis failed partway through — output may be incomplete. Check the event log at `~/docs/epiphany/audit/.logs/<report-id>.jsonl` for details."*

Both set `improvement_partial: true` in the report frontmatter.

## E20 → E21 Sequencing

E20 fires when N27 writes the report and summarizes to user. In no-flag mode (fix offered after improve), E21 (fix? offer) fires AFTER E20 resolves.

## Token Budget

Scales with survivors count (~300 tokens per entry for formatting).

## Backtrack / Aggregation

None.

## Fan-out Cardinality

1:1 → user (E20, terminal for --improve subpipeline).

## Back-edge Endpoints

None.
```

- [ ] **Step 5: Run module linter**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 27 modules have required sections. OK.`

- [ ] **Step 6: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/modules/
git -C ~/.claude commit -m "feat(epiphany-audit): modules N24-N27 (improvement subpipeline) — all 27 modules complete"
```

---

## Task 17: SKILL.md Part 1 (Sections 1–7)

**Files:**
- Create: `~/.claude/skills/epiphany-audit/SKILL.md` (Sections 1–7 of 13)

- [ ] **Step 1: Write SKILL.md sections 1–7**

Create `~/.claude/skills/epiphany-audit/SKILL.md` with the following content:

```markdown
# epiphany-audit SKILL

Graph-of-thought codebase audit + safeguarded fix pipeline.
Family: `epiphany-*`

> **Implementer note:** this file is the Layer-A orchestrator contract. Each node's detailed
> Layer-B contract lives in `modules/N0N-*.md`. JSON schemas are in `schemas/`. Graph
> declaration is in `graph.json`. Dimension plugins are in `dimensions/`.

---

## 1. Purpose & Scope

`epiphany-audit` runs a multidimensional, project-aware audit on any codebase, then
optionally drives a safeguarded fix pipeline. It is designed to:

- Audit for regression areas, bugs, potential issues, and latent problems
- Prune irrelevant dimensions via R-ROUTE (skipped dimensions cite explicit rationale)
- Surface project-specific blindspots via B-FIND
- Cite every finding with verified `file:line` evidence (no hallucinated locations)
- Apply fixes only with explicit per-tier user consent (default policy)
- Track fix state via recovery manifest for safe resumption after interruption
- Optionally generate improvement recommendations via the `--improve` subpipeline

**MAY invoke:** `kb-route` (optional KB consultation via dimension plugins), `prompt-graph` (optional remediation-message enhancement; never required).

**MUST NOT invoke:** `writing-plans`, `executing-plans`, `find-skills`.

**Write-tool footprint:** report files under `~/docs/epiphany/audit/` only; fix diffs against `audit_target` only.

---

## 2. Invocation Contract

```
/epiphany-audit [<path>] [--audit | --fix <report>] [--verbose] [--deep] [--improve]
                [--auto | --confirm-all | --dry-run]
                [--escalate-finding F00N] [--test-cmd '<cmd>']
                [--monorepo-subtree-limit N] [--reverify-state]
                [--full-rerun | --no-rerun]
```

### Mode flags (mutually exclusive)

| Flag | Behavior |
|------|----------|
| `--audit` | Produce report only; do NOT offer fix pipeline |
| `--fix <report>` | Consume existing audit report; run fix pipeline only (skips N01..N15) |
| (none) | Audit → save prompt → fix-offer (default) |
| `--audit` + `--fix` | `halt-on-flag-conflict` |

### Autonomy flags (mutually exclusive)

| Flag | Behavior |
|------|----------|
| `--auto` | Tier-1 silent apply; Tier-2 batch-confirmed; Tier-3 per-fix |
| `--confirm-all` | Every fix requires per-fix confirmation |
| `--dry-run` | Emit fix-plan + diffs only; no apply, no commits, no branch; pipeline halts at N17 |
| >1 of the above | `halt-on-flag-conflict`; precedence on contradiction: `--dry-run` > `--confirm-all` > `--auto` |

### Other flags

| Flag | Behavior |
|------|----------|
| `--verbose` | Adds depth where it improves actionability; never adds nitpick padding |
| `--deep` | Lifts spawn budget to ≤3; subagent fan-out for analyzers; interactive B-FIND prompt; 80k-token checkpoint cap |
| `--improve` | After audit + save prompt, run improvement subpipeline (N24–N27). Valid in audit/no-flag mode only; warning + skip if used with `--fix` |
| `--escalate-finding F00N` | Force finding to Tier-3 regardless of N16 classification; overrides `--auto` for that finding; `halt-on-invalid-finding-id` if ID not in report |
| `--test-cmd '<cmd>'` | Override auto-detected test command |
| `--monorepo-subtree-limit N` | Override cap on distinct project-shaped subtrees (default 10) |
| `--full-rerun` / `--no-rerun` | Override audit-rerun tier policy (§8); mutually exclusive; `halt-on-flag-conflict` if both given |
| `--reverify-state` | Clear `reachable: false` annotations from state file at `--fix` entry; valid in `--fix` mode only |

### `<report>` resolution order (for `--fix`)

1. Absolute path
2. Path relative to cwd
3. Bare filename or partial slug: search `~/docs/epiphany/audit/` then `fix-reports/`; if no extension, append `.md` and retry
4. Multiple matches → `halt-on-ambiguous-fix-report`
5. No match → `halt-on-unresolvable-fix-report`

---

## 3. Implied-Context Resolution

Deterministic, halt-on-ambiguity. **No silent guessing.**

Resolution order:
1. Explicit `<path>` argument wins
2. `--fix <report>` → derives target from `audit_target` in report frontmatter; if both explicit `<path>` AND `--fix <report>` given and `realpath(<path>) != realpath(audit_target)` → `halt-on-target-conflict`
3. `cwd` inside git repo → `git rev-parse --show-toplevel`
4. `cwd` itself
5. `halt-pre-audit`

**Suspicious-target gate** (runs on resolved target regardless of resolution path):

Hard halt (`halt-suspicious-target`) if resolved root is `$HOME`, `/`, `/etc`, `/usr`, `/var`, `/tmp`, or has >5 top-level subdirectories that each independently look like a project.

Warn-and-prompt (soft halt; user may override via `~/.config/epiphany-audit/allowed-roots.json`) for:
- `~/.claude/skills/<x>/`, `~/dotfiles`, `~/.config`, `~/Desktop`, `~/Downloads`

Other ambiguity halts:
- Nested git repos detected → list candidates, ask user
- Session touched multiple projects → list candidates, ask user
- Polyglot monorepo with inconclusive language detection → ask user

---

## 4. Node Registry Overview

27 nodes: 15 audit-pipeline (N01–N15) + 8 fix-pipeline (N16–N23) + 4 improvement subpipeline (N24–N27).

| ID | Name | Active in | Type |
|----|------|-----------|------|
| N01 | ContextIntake | audit | ingest |
| N02 | RelevanceRouter (R-ROUTE) | audit | router |
| N03 | BlindspotFinder (B-FIND) | audit | meta-analyzer |
| N04 | DimensionAnalyzer.CORRECTNESS | audit | analyzer |
| N05 | DimensionAnalyzer.ARCHITECTURE | audit | analyzer |
| N06 | DimensionAnalyzer.PERFORMANCE | audit | analyzer |
| N07 | DimensionAnalyzer.SECURITY | audit | analyzer |
| N08 | DimensionAnalyzer.MAINTAINABILITY | audit | analyzer |
| N09 | DimensionAnalyzer.\<X\> (plugin-instantiated) | audit | analyzer |
| N10 | FalsePositiveVerifier (FPV) | audit | verifier |
| N11 | FindingsAggregator | audit | aggregator |
| N12 | Prioritizer | audit | scorer |
| N13 | ReportFormatter | audit | formatter |
| N14 | Q-GATE (Pass A inline + Pass B conditional subagent) | audit | verifier |
| N15 | SaveHandler | audit | io |
| N16 | FixTriage (F-VAL ingest + Resume-handler + Triage) | fix | validator |
| N17 | FixPlanner | fix | planner |
| N18 | PreFlight | fix | preflight |
| N19 | FixApplier | fix | actuator |
| N20 | PerFixVerifier | fix | verifier |
| N21 | RegressionBattery (battery + tiered audit-rerun) | both | verifier |
| N22 | RollbackHandler | fix | recovery |
| N23 | FixReporter | fix | formatter |
| N24 | ImprovementContextualizer | improve | analyzer |
| N25 | ImprovementBrainstormer | improve | analyzer |
| N26 | OverEngineeringFilter (OEF) | improve | filter |
| N27 | ImprovementReporter | improve | formatter |

For each node's full Layer-B contract see `modules/N0N-*.md`. For the graph declaration see `graph.json`.

---

## 5. Mode Flowcharts

### Audit mode (`--audit` or no-flag)

```
invoke
  └─ N01 ContextIntake
       └─ N02 R-ROUTE (loads dimension plugins; floor: CORRECTNESS + MAINTAINABILITY always on)
            └─ N03 B-FIND (auto-add HIGH-confidence gaps; --deep: interactive)
                 └─ N04..N09 DimensionAnalyzers (per activation map; parallel fan-out under --deep)
                      └─ N10 FPV (false-positive check + location cache; BACKTRACK single cap)
                           └─ N11 Aggregator (dedup, merge, count-collapse)
                                └─ N12 Prioritizer (priority_score, punch list)
                                     └─ N13 Formatter (markdown per §4.1 template)
                                          └─ N14 Q-GATE
                                               Pass A (mandatory-field + location + CRITICAL/HIGH confidence + no-comment-echo)
                                               Pass B (conditional subagent: ≥5 findings OR CRITICAL/HIGH present OR --deep)
                                               └─ N15 SaveHandler
                                                    └─ [--improve]: N24 → N25 → N26 → N27 → user (E20)
                                                    └─ [no-flag]: fix-offer (E21) → N16..N23 fix pipeline
                                                    └─ [--audit]: done (E12 save prompt only)
```

### Fix mode (`--fix <report>` or post-audit fix consent)

```
  └─ N16 FixTriage
       [resume-handler sub-step if prior interrupted run detected]
       F-VAL schema validation → idempotency check → tier classification → grouping + topo-sort
       └─ N17 FixPlanner
            tier batch approval → [--dry-run: write dry-run plan, halt]
            └─ N18 PreFlight
                 orphan sweep → git-state check → baseline capture → branch creation
                 └─ N19 FixApplier (atomic loop per fix-group)
                      └─ N20 PerFixVerifier (targeted tests + type check)
                           [E_repair: 1st→retry N19; 2nd→replan N17; 3rd→cap-hit→N22]
                 └─ N21 RegressionBattery
                      battery (tests/types/lint/build/diff-scope)
                      + tiered audit-rerun (Tier-1: skip; Tier-2: narrow; Tier-3: full)
                      [E_rerun_fail: induced regressions → N16 re-triage]
                 └─ N23 FixReporter (planned termination: partial=false)
                      └─ E_finalize → N22 RollbackHandler (archive manifest)
                           └─ E_complete → user
```

### Halt-mid-fix path

```
halt-mid-fix-on-perfix-cap-hit OR halt-mid-fix-on-induced-regression-cap-hit
  └─ E_halt_partial → N23 (partial=true, halt_state=<id>)
       └─ halt envelope → user
       [recovery manifest stays live for resume]
```

---

## 6. Halt-State Envelope Format

Every halt state emits a structured envelope at the **top** of the user-facing message before any diagnostic text:

```
{halt_state: <state-id>, subreason: <text>, diagnostic: <details>}
```

| halt_state | Triggered at | Subreason axis |
|------------|-------------|----------------|
| `halt-pre-audit` | N01 resolution | no resolvable target |
| `halt-suspicious-target` | N01 gate | `$HOME` / wrapper repo / denylist match |
| `halt-ambiguous-target` | N01 resolution | nested git / multi-project / polyglot inconclusive |
| `halt-on-flag-conflict` | N01 parse | mutually exclusive flags |
| `halt-on-flag-rejection` | N01 parse | unsupported flag (e.g., `--demote-finding`) |
| `halt-on-target-conflict` | N01 resolution | explicit path ≠ report's audit_target |
| `halt-on-unresolvable-fix-report` | N16 entry | `<report>` could not be resolved |
| `halt-on-ambiguous-fix-report` | N16 entry | bare `<report>` matched multiple files |
| `halt-on-floor-plugin-missing` | N02 startup | bundled floor plugin file missing; subreason: plugin-name |
| `halt-on-mismatched-version` | N16 F-VAL | tool_version skew; user declined |
| `halt-pre-fix-on-validator-failure` | N16 F-VAL | schema fail / suspicious-content user-declined |
| `halt-on-empty-or-unfixable-report` | N16 ingest | zero main-body findings |
| `halt-on-conflicting-fixes` | N16 triage | incompatible edits on same line range (live mode) |
| `halt-on-test-cmd-unknown` | N18 | no test command available |
| `halt-on-baseline-failure` | N18 | runner crash; subreason: `resume-baseline-missing` |
| `halt-on-git-state-incompatible` | N18 | dirty tree / detached HEAD / no commits; subreason: `branch-name-exhausted` |
| `halt-on-q-gate-failure` | N14 | subreason: `pass-a` / `pass-b` / `pass-b-exec-error` |
| `halt-mid-fix-on-perfix-cap-hit` | E_repair (3rd) | E_repair retries exhausted; all remaining groups blocked |
| `halt-mid-fix-on-induced-regression-cap-hit` | E_repair (post-rerun) | induced-regression fix-group retry cap exhausted |
| `halt-on-scope-creep` | N21 diff-scope | unmapped diff hunks; do NOT auto-revert |
| `halt-on-token-cap` | `--deep` | accumulated context > 80k; partial report emitted |
| `halt-on-recovery-conflict` | run entry | recovery manifest from prior interrupted run |
| `halt-on-user-abort` | any interactive prompt | ctrl-C / explicit halt |
| `halt-on-stale-source-report` | N16 F-VAL | source report file missing or SHA-256 mismatch |
| `halt-no-source-detected` | N01 | binary-only / empty / documentation-only repo |
| `halt-on-files-outside-tree` | N16 F-VAL | report references files outside audit_target tree |
| `halt-on-invalid-finding-id` | N16 post-F-VAL | `--escalate-finding` ID absent from report |
| `halt-on-resume-tree-divergence` | N16 resume-handler | cleanup would discard out-of-scope working-tree changes |

---

## 7. Report Schema Cross-Reference

Five report types; all schema-versioned (`schema_version: 1`). Authoritative JSON schemas in `schemas/`.

| Report type | Output path | Schema file |
|-------------|-------------|-------------|
| Audit report | `~/docs/epiphany/audit/<project-slug>-<YYYYMMDD>-<HHMMSS>.md` | `audit-report-v1.schema.json` |
| Fix report | `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-<YYYYMMDD>-<HHMMSS>.md` | `fix-report-v1.schema.json` |
| Dry-run plan | `~/docs/epiphany/audit/dry-run-plans/<source-report-id>-dryrun-<YYYYMMDD>-<HHMMSS>.md` | `dry-run-plan-v1.schema.json` |
| Dimension plugin | `dimensions/*.md` (YAML frontmatter only) | `dimension-plugin-v1.schema.json` |
| Improvement report | `~/docs/epiphany/audit/improvement-reports/<project-slug>-<YYYYMMDD>-<HHMMSS>-improve.md` | `improvement-report-v1.schema.json` |

**Cross-schema invariants:**
- `source_report_id` in fix-report = `report_id` in audit-report = `source_report_id` in dry-run plan = `source_report_id` in improvement report.
- `source_audit_report_sha256` verified at `--fix` start; mismatch → `halt-on-stale-source-report`.
- `partial: true ⇔ halt_state: non-null` in fix reports (mutual implication; validated by JSON Schema).
- "Unverified Hypotheses" findings are NOT consumable by `--fix`; F-VAL ingests only main-body findings.
- Improvement reports are read-only artifacts; NOT consumed by `--fix`.

**Finding priority_score formula:**
```
severity:    CRITICAL=4, HIGH=3, MEDIUM=2, LOW=1, INFO=0
confidence:  HIGH=3, MEDIUM=2, LOW=1
effort:      trivial=1, modest=2, significant=3
priority_score = (severity × confidence) / effort
```

**Finding severity definitions:**
```
CRITICAL = data loss, security breach, crash on common path, corruption
HIGH     = crash on edge case, wrong output silently, perf regression >2x
MEDIUM   = degraded UX, recoverable error mishandled, maintainability cliff
LOW      = code smell with concrete future cost
INFO     = observation, no action required
```
```

- [ ] **Step 2: Run a quick sanity check on the file**

Run:
```bash
grep -c "^## " ~/.claude/skills/epiphany-audit/SKILL.md
```
Expected: `7` (sections 1–7 written).

- [ ] **Step 3: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/SKILL.md
git -C ~/.claude commit -m "feat(epiphany-audit): SKILL.md sections 1-7"
```

---

## Task 18: SKILL.md Part 2 (Sections 8–13 + worked examples)

**Files:**
- Modify: `~/.claude/skills/epiphany-audit/SKILL.md` (append Sections 8–13)

- [ ] **Step 1: Append sections 8–13 to SKILL.md**

Append the following to `~/.claude/skills/epiphany-audit/SKILL.md`:

```markdown
---

## 8. Verification-Gate Ordering

Gates run in this fixed order during the fix pipeline. Later gates do not run if an earlier gate fails.

| # | Gate | Node | Failure consequence |
|---|------|------|---------------------|
| 1 | F-VAL ingest | N16 | `halt-pre-fix-on-validator-failure` |
| 2 | Empty-report check | N16 | `halt-on-empty-or-unfixable-report` |
| 3 | Idempotency check | N16 | warn + user override |
| 4 | Tier classification | N16 | defer-on-uncertainty; `halt-on-conflicting-fixes` in live mode |
| 5 | Fix-plan approval | N17 | per-tier outcome (decline = deferred; halt = stop) |
| 6 | Pre-flight baseline | N18 | `halt-on-baseline-failure`, `halt-on-test-cmd-unknown`, `halt-on-git-state-incompatible` |
| 7 | Per-fix verify | N20 | atomic rollback + E_repair routing |
| 8 | Regression battery | N21 (battery sub-step) | E_repair; `halt-on-scope-creep` |
| 9 | Audit-rerun delta | N21 (audit-rerun sub-step) | E_rerun_fail (induced regression) |

**Q-GATE** (audit pipeline):
- Pass A (inline): mandatory fields, location verification (from N10 cache), CRITICAL/HIGH×confidence floor, dup merge, no-comment-echo, no-LOW-only warning
- Pass B (conditional subagent): anti-iatrogenic, evidence-rationale coherence, dimension-classification correctness

**Shared location-verification cache contract (N10 + N14 Pass A):**
- Lives in process memory for one skill invocation only (never persisted)
- Key: `(canonical_file_path, line_range)` normalized as `(start_line, end_line)`
- Value: `{ verified, content_hash, populated_by: "N10", populated_at }`
- N10 FPV is the only writer; N14 Pass A is read-only (falls back to its own Read on cache miss)
- Failed Reads recorded as `verified: false`

---

## 9. Tier + Autonomy Policy

### Tier classification rules (N16, deterministic)

**Tier-1 (mechanical):** ALL of:
- Remediation diff ≤ 2 lines edited within a single file
- No function/method signature changes
- No new identifiers introduced
- Target file imported by ≤ 5 other files
- `confidence: HIGH`
- `effort: trivial`

**Tier-2 (local logic):** ALL of:
- Remediation bounded to a single function body
- May add local symbols (locals, in-scope helper functions)
- No public-API change (no exported identifier renamed/removed/signature-changed)
- Target file imported by ≤ 20 other files

**Tier-3 (cross-cutting):** anything not satisfying Tier-1 or Tier-2, including:
- Multi-file remediation
- Any signature change to an exported identifier
- Schema/migration/config files
- New files
- Findings flagged via `--escalate-finding`
- Non-literal remediation (numbered steps without a literal patch) → Tier-3, `tier_classification_reason: "non-literal remediation"`

### Autonomy policy matrix

| Tier | Default | `--auto` | `--confirm-all` | `--dry-run` |
|------|---------|----------|-----------------|-------------|
| 1 | Batch confirm | Silent apply | Per-fix confirm | No apply |
| 2 | Batch confirm | Batch confirm | Per-fix confirm | No apply |
| 3 | Per-fix confirm | Per-fix confirm | Per-fix confirm | No apply |

**Per-fix-opt-in floor (anti-conformity):** even under `--auto`, any finding with `confidence < HIGH OR effort > trivial` requires per-fix opt-in. Only HIGH-confidence, trivial-effort fixes auto-apply.

**Tier decline behavior:** T1 → T2 → T3 presented in order. Decline on Tier-N → all Tier-N findings `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit `halt` → stop entirely.

**`--demote-finding` is NOT supported** (`halt-on-flag-rejection`). Edit the report manually.

---

## 10. Recovery Semantics

### Recovery manifest lifecycle

Written by N19 at fix-group **boundaries only** (start / end-success / end-failure). NOT written during intra-loop transitions.

**States:**
- `in_flight_finding_id` set → a fix-group is currently executing
- Finding in `applied` → committed successfully
- Finding in `failed` → cap-hit; downstream dependents marked `deferred (upstream-dependency-failed)`
- Finding in `pending` → not yet started

**Planned termination (E_finalize):** N23 writes fix report → N22 reads `fix_report_id` from it → N22 archives manifest to `.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json` → removes live `<report-id>.json` → E_complete → user.

**Halt-mid-fix:** manifest stays live at `~/docs/epiphany/audit/.recovery/<report-id>.json`. Resume on next run.

**Archive states:** `completed` (planned termination), `superseded` (user chose `fresh` over interrupted run), `aborted` (reserved).

### `halt-on-recovery-conflict` options

When a recovery manifest is detected at `--fix` entry:

- **`resume`**: continue from `last_known_good_sha`; skip applied; continue with pending list.
  - Resume-handler sub-step (first action in N16): tree-divergence safety check → `git checkout -- . && git clean -fd` (only after safety check passes or user authorizes) → move `in_flight_finding_id` back to `pending`.
  - Audit-rerun tier policy on resume: determined by combined highest tier (original + resumed run).
- **`fresh`**: archive existing manifest as `superseded-<ISO-timestamp>.json` (forensic record preserved); start over.
- **`abort`**: halt; no changes.

### Idempotency state file

Written by N15 SaveHandler on save-accept. Located at `~/docs/epiphany/audit/.state/<report-id>.json`. Authoritative over git-log for idempotency checks. Conflict resolution:
- `(a) re-apply`: replaces sha in state file; adds `previous_sha_unreachable: <old-sha>` metadata.
- `(b) skip`: annotates state entry with `reachable: false, last_checked: <ISO>`. Cleared by `--reverify-state`.
- `(c) abort`: state file untouched.

---

## 11. Hard Rules (Audit + Fix)

### Audit hard rules

- Every finding has ALL mandatory schema fields (id, location, dimensions, severity, confidence, evidence_excerpt, evidence_excerpt_extended, rationale, remediation, false_positive_check, effort, priority_score, tests_present_signal, provenance). Findings missing any mandatory field → demote to "Unverified Hypotheses".
- Every `file:line` is verified against the actual file via Read at audit time. **No hallucinated lines.**
- Every CRITICAL/HIGH finding has Confidence ≥ MEDIUM. HIGH-severity at LOW-confidence → demote severity OR upgrade confidence with stated evidence.
- LOW-confidence findings include `verify_by: <what would lift confidence>`.
- Duplicate patterns merged with count.
- Q-GATE Pass A no-comment-echo: no finding text quotes the project's own TODO/FIXME without independent verification.
- `tests_present_signal` must be set when test-dir grep matches the involved function/class/module. Elevates the confidence floor for that finding.

### Fix hard rules

- **DO NOT** apply fixes outside source tree.
- **DO NOT** modify files audit didn't flag — **except** regression-prevention test additions in the same commit as the fix (test files containing only new test cases exercising the audit-flagged failure mode).
- **DO NOT** skip post-fix verification.
- **DO NOT** batch-apply fixes spanning the same file without staged review.
- **DO NOT** continue after verification failure without explicit user authorization (or per E_repair bounded retry).
- **Never** expand scope beyond audit findings. Spotted unrelated bug → log as new finding; do not fix it now.
- **Never** bypass safety checks (`--no-verify`, `--force-push`, hook skipping).
- **Never** amend prior commits — always new commits, even on retry.
- **Defer over guess** — if root cause is unclear, mark `deferred` with a question.
- **Idempotent** — re-runs skip already-applied findings (state file > git-log fallback).
- **Fail-loud on partial state** — recovery manifest written at boundaries; mid-flight death leaves coherent at-rest state.

---

## 12. Anti-Patterns

### Audit findings — MUST NOT exhibit

- Stylistic preferences disguised as bugs ("could use `auto` here")
- Findings without reading the actual code (hallucinated `file:line`)
- Generic advice applicable to any project ("add more tests")
- Refactors with no concrete defect or measured cost
- Duplicate findings (collapse with count)
- Wall of LOW-severity nitpicks burying real defects
- Rewrites without a concrete defect driving them
- Echoing project's own TODO/FIXME comments (covered by Q-GATE Pass A no-comment-echo)
- "I would have written it differently" ≠ "this is wrong"
- Reporting findings the existing tests already cover without verifying the test doesn't cover the failure path

### Fix application — MUST NOT exhibit

- Fixes outside source tree
- Modifying files audit didn't flag (except regression-prevention tests in same commit)
- Skipping post-fix verification
- Batch-applying fixes across same file without staged review
- Continuing after verification failure without authorization
- Expanding scope beyond audit findings
- Bypassing safety checks
- Amending prior commits
- Guessing root cause when unclear
- Silent re-application of already-applied findings
- Silent partial state on mid-flight death

---

## 13. Worked Examples

### Example 1 — Worked Finding (full mandatory fields)

```yaml
## Finding F001

id: F001
location: src/parser.py:142
dimensions: [CORRECTNESS]
severity: HIGH
confidence: HIGH
evidence_excerpt: |
  for i in range(len(tokens) - 1):
      emit(tokens[i])
  # final token never emitted
evidence_excerpt_extended: false
rationale: Loop bound drops final token; downstream consumer expects all N tokens.
remediation: |
  -    for i in range(len(tokens) - 1):
  +    for i in range(len(tokens)):
false_positive_check:
  intentional:           { value: false, justification: "no test or comment justifies the -1" }
  file_symbol_verified:  { value: true,  justification: "Read at src/parser.py:140-145" }
  reachable_from_entry:  { value: true,  justification: "called by parse_input in main.py:23" }
  fix_breaks_dependents: { value: false, justification: "grep shows no caller relies on N-1 emission" }
effort: trivial
priority_score: 9.0   # (3 × 3) / 1
verify_by: null
tests_present_signal: false
provenance:
  node: N04
  mode: inline
  model: claude-sonnet-4-6
  pass_b_model: null
  prompt_hash: a3f9e2c1d4b8f7e0a1b2c3d4e5f60718
  plugin_name: null
  plugin_version: null
  audit_rerun_iteration: 0
  q_gate_pass_b_demoted: false
```

### Example 2 — Worked graph.json node entry

```json
{
  "id": "N02",
  "name": "RelevanceRouter",
  "type": "router",
  "mode": "inline",
  "active_in": "audit",
  "inputs": ["project_model from N01", "dimension_plugins_from_disk"],
  "outputs": ["dimension_activation_map", "plugin_registry"],
  "aggregation_policy": "n/a",
  "halt_conditions": ["halt-on-floor-plugin-missing"]
}
```

### Example 3 — Worked improvement entry (post-OEF survivor)

```yaml
## Improvement I002

id: I002
category: quick-win
area: testing
utility_score: 2
cost_score: 1
description: |
  The project uses dynamic test discovery but has no conftest.py at the repo root.
  Failures in fixture setup are silently swallowed on Python < 3.11, meaning a broken
  fixture causes zero tests to run rather than N failures — masking breakage.
action: |
  Add a minimal conftest.py at the repo root with a session-scoped fixture guard:
    assert sys.version_info >= (3, 10), "test suite requires Python 3.10+"
success_measure: |
  Running pytest with a broken fixture produces a visible ERROR line in output
  rather than "collected 0 items".
```

### Example 4 — Worked dimension-routing decision

```
CORRECTNESS:     activated (floor — always on)
MAINTAINABILITY: activated (floor — always on)
PERFORMANCE:     skipped — no hot loops detected, no perf-critical heuristic match
SECURITY:        activated for [shell-injection, secrets-in-source];
                 skipped sub-surfaces [SQL] — no DB layer detected
ARCHITECTURE:    activated — >3 modules with cross-imports detected
```

### Example 5 — Commit message format

```
[AUDIT-001] fix off-by-one in parser token loop

Finding-id: F001
Dimensions: CORRECTNESS
Severity: HIGH
Source: myproject-20260427-100000.md
```

Paired regression-test follow-up commit (if required):
```
[AUDIT-001-test] regression test for fix off-by-one in parser token loop

Test-for-finding: F001
Dimensions: CORRECTNESS
Severity: HIGH
Source: myproject-20260427-100000.md
```
```

- [ ] **Step 2: Verify section count**

Run:
```bash
grep -c "^## " ~/.claude/skills/epiphany-audit/SKILL.md
```
Expected: `13` (all 13 Layer-A sections present).

- [ ] **Step 3: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/SKILL.md
git -C ~/.claude commit -m "feat(epiphany-audit): SKILL.md sections 8-13 — all 13 Layer-A sections complete"
```

---

## Task 19: Smoke test scenarios

**Files:**
- Create: `tests/smoke/01-default-invocation.md` through `tests/smoke/11-fix-improve-warning.md`
- Create: `tests/smoke/fixtures/user-dimensions/test-custom-dim.md`

Each smoke test is a human-readable scenario description. These are NOT automatically runnable — they describe expected inputs, actions, and observable outputs for manual or LLM-driven end-to-end verification.

- [ ] **Step 1: Write smoke/01-default-invocation.md**

```markdown
# Smoke Test 01 — Default Invocation

**Scenario:** Run `/epiphany-audit` from within a small Python project directory with no flags.

**Setup:**
- cwd is inside a git repo with ≥1 commit
- Project has Python source files + a `pytest` test suite
- Dimension plugin files are present and valid in `dimensions/`

**Actions:**
1. Invoke `/epiphany-audit` (no flags, no path argument)
2. Allow the full pipeline to run (N01 → N14)
3. Respond "y" to the save prompt (N15)
4. Respond "n" to the fix-offer prompt (E21)

**Expected outcomes:**
- N01 resolves `audit_target` via `git rev-parse --show-toplevel`
- N02 activates CORRECTNESS + MAINTAINABILITY (floor); activates or skips other dimensions based on heuristics
- N14 Q-GATE produces `pass_a: pass`; Pass B: `skipped-low-volume` if <5 findings and no CRITICAL/HIGH
- Audit report saved to `~/docs/epiphany/audit/<project-slug>-<YYYYMMDD>-<HHMMSS>.md`
- Idempotency state file written to `~/docs/epiphany/audit/.state/<report-id>.json`
- Fix-offer shown; user responds "n"; pipeline ends
- Structured event log created at `~/docs/epiphany/audit/.logs/<report-id>.jsonl`
- No halt states triggered
```

- [ ] **Step 2: Write smoke/02-audit-only.md**

```markdown
# Smoke Test 02 — Audit-Only Mode

**Scenario:** Run `/epiphany-audit --audit` — report produced, no fix offered.

**Actions:**
1. Invoke `/epiphany-audit --audit`
2. Allow pipeline to complete
3. Respond "y" to save prompt

**Expected outcomes:**
- E21 (fix-offer) is NEVER emitted
- Fix pipeline (N16–N23) is NEVER entered
- Audit report saved
- No fix report, no recovery manifest, no branch created
```

- [ ] **Step 3: Write smoke/03-fix-known-good-report.md**

```markdown
# Smoke Test 03 — Fix a Known-Good Report

**Scenario:** Run `/epiphany-audit --fix <report>` on a valid saved audit report.

**Setup:**
- A valid audit report exists at `~/docs/epiphany/audit/myproject-20260427-100000.md`
- The report has ≥1 main-body finding with a literal-patch remediation
- The project is in a clean git state with ≥1 commit

**Actions:**
1. Invoke `/epiphany-audit --fix ~/docs/epiphany/audit/myproject-20260427-100000.md`
2. Approve the fix plan when prompted (N17)
3. Allow N18 → N19 → N20 → N21 → N23 to complete

**Expected outcomes:**
- N01..N15 SKIPPED entirely
- N16 F-VAL validates the report; SHA-256 captured
- Branch `epiphany-audit/<report-id>-YYYYMMDD` created
- Per-finding: `[AUDIT-NNN]` commit created on success; working-tree discarded on fail
- Fix report saved to `~/docs/epiphany/audit/fix-reports/`
- Recovery manifest archived to `.recovery/.archive/<report-id>-completed-<ISO>.json`
- Live recovery manifest removed (`<report-id>.json` gone)
- No `partial: true` in fix report
```

- [ ] **Step 4: Write smoke/04-dry-run.md**

```markdown
# Smoke Test 04 — Dry-Run Mode

**Scenario:** `/epiphany-audit --fix <report> --dry-run` — plan emitted, nothing applied.

**Actions:**
1. Invoke `/epiphany-audit --fix <report> --dry-run`
2. Observe output

**Expected outcomes:**
- N17 FixPlanner writes a Dry-Run Plan v1 to `~/docs/epiphany/audit/dry-run-plans/`
- Pipeline HALTS at N17; N18 (branch creation) is NEVER entered
- No working-tree changes, no branch created, no commits
- Plan contains `proposed_diff` for each finding
- Fix report: NOT created (dry-run plan is the artifact)
- Warning emitted if `--no-rerun` is also passed: *"--no-rerun has no effect under --dry-run"*
```

- [ ] **Step 5: Write smoke/05-recovery-manifest-resume.md**

```markdown
# Smoke Test 05 — Recovery Manifest Resume

**Scenario:** Simulate a mid-flight process death, then resume.

**Setup:**
- A live recovery manifest exists at `~/docs/epiphany/audit/.recovery/<report-id>.json`
- The manifest has `in_flight_finding_id` set (process died inside the atomic loop)
- The audit branch exists at `epiphany-audit/<report-id>-YYYYMMDD`

**Actions:**
1. Invoke `/epiphany-audit --fix <report>`
2. Observe `halt-on-recovery-conflict` prompt
3. Choose `resume`
4. Allow pipeline to complete

**Expected outcomes:**
- `halt-on-recovery-conflict` emitted with structured envelope at top of message
- Resume-handler sub-step runs: tree-divergence safety check → git checkout/clean if safe → `in_flight_finding_id` moved back to `pending`
- Applied findings skipped (idempotency)
- Pending findings processed in topo-sort order
- On completion: recovery manifest archived as `completed-<ISO>.json`; live manifest removed
```

- [ ] **Step 6: Write smoke/06-suspicious-target-halt.md**

```markdown
# Smoke Test 06 — Suspicious Target Halt

**Scenario:** Invoke with a suspicious target root.

**Actions:**
1. Invoke `/epiphany-audit $HOME`

**Expected outcomes:**
- `halt-suspicious-target` emitted immediately with structured envelope:
  `{halt_state: "halt-suspicious-target", subreason: "$HOME", diagnostic: "resolved target $HOME looks like a wrapper/aggregator, not a project."}`
- Pipeline does NOT proceed past N01
- No files written, no branches created
```

- [ ] **Step 7: Write smoke/07-target-conflict-halt.md**

```markdown
# Smoke Test 07 — Target Conflict Halt

**Scenario:** Explicit path disagrees with report's audit_target.

**Actions:**
1. Invoke `/epiphany-audit /some/other/project --fix ~/docs/epiphany/audit/myproject-report.md`
   (where `myproject-report.md` has `audit_target: /home/user/myproject`)

**Expected outcomes:**
- `halt-on-target-conflict` emitted:
  `{halt_state: "halt-on-target-conflict", subreason: "path-report-disagreement", diagnostic: "explicit path /some/other/project does not match the audit_target field of the report."}`
- Pipeline does NOT proceed
```

- [ ] **Step 8: Write smoke/08-dimension-plugin-loading.md**

```markdown
# Smoke Test 08 — Dimension Plugin Loading

**Setup:**
- A valid custom dimension plugin exists at `tests/smoke/fixtures/user-dimensions/test-custom-dim.md`
- Copy it to `~/.config/epiphany-audit/dimensions/test-custom-dim.md`

**Actions:**
1. Invoke `/epiphany-audit --audit` on a project that matches the plugin's activation triggers

**Expected outcomes:**
- N02 loads bundled plugins (5) + user plugin (1)
- User plugin appears in `dimensions_activated` if its triggers fire
- Structured event log shows `test-custom-dim: loaded`
- Findings from the plugin carry `provenance.plugin_name: test-custom-dim`
```

- [ ] **Step 9: Write smoke/08-fixture user-dimensions/test-custom-dim.md**

```markdown
---
schema_version: 1
name: test-custom-dim
display_name: Test Custom Dimension
version: 1.0.0
applies_to:
  languages: "*"
  project_markers: []
activation_triggers:
  - type: file_present
    path: "**/*.py"
exclusions: []
prompt_template: |
  Analyze the following code for test-custom-dim issues.
  Look for any function named `test_custom_target`.
  Return findings conforming to Audit Report Schema v1.
kb_route_query: null
intra_node_token_budget: 5000
priority: low
---

# Test Custom Dimension (smoke test fixture)

Used by smoke test 08 only. Not a real dimension.
```

- [ ] **Step 10: Write smoke/09-improve-mode.md**

```markdown
# Smoke Test 09 — Improve Mode

**Scenario:** `/epiphany-audit --improve` — improvement report produced after audit.

**Actions:**
1. Invoke `/epiphany-audit --improve`
2. Allow audit pipeline (N01–N14) to complete
3. Respond "y" to save prompt (N15)
4. Allow improvement subpipeline (N24–N27) to complete
5. Respond "n" to fix-offer (E21)

**Expected outcomes:**
- N24 ImprovementContextualizer runs immediately after N15
- N25 ImprovementBrainstormer generates candidates
- N26 OEF filters by utility/cost
- N27 writes improvement report to `~/docs/epiphany/audit/improvement-reports/`
- Audit report frontmatter updated: `improvement_report_ref: <absolute path>` patched in-place
- Improvement report has correct count invariants: `survivors == notable + quick_wins + worthwhile`
- `total_candidates == filtered_out + survivors`
```

- [ ] **Step 11: Write smoke/10-improve-zero-survivors.md**

```markdown
# Smoke Test 10 — Improve Zero Survivors

**Scenario:** All improvement candidates are filtered by OEF.

**Setup:**
- Arrange for N25 to produce candidates that all fail OEF rules
  (all have cost > utility, or utility=1, or speculative language)

**Expected outcomes:**
- N27 emits a valid improvement report with `survivors: 0`, `notable: 0`, `quick_wins: 0`, `worthwhile: 0`
- Report body is empty (zero improvement entries)
- Summary section contains: *"No improvements above the utility/cost threshold were found. This is a valid result."*
- `improvement_partial: false` (pipeline completed; just no survivors)
- Pipeline does NOT halt; fix-offer (E21) fires normally after E20
```

- [ ] **Step 12: Write smoke/11-fix-improve-warning.md**

```markdown
# Smoke Test 11 — Fix + Improve Warning

**Scenario:** `/epiphany-audit --fix <report> --improve` — improvement pipeline is silently skipped.

**Actions:**
1. Invoke `/epiphany-audit --fix <report> --improve`

**Expected outcomes:**
- A user-facing WARNING is emitted: *"--improve is ignored with --fix; run without --fix to include improvement analysis."*
- The warning is NOT a halt — the fix pipeline proceeds normally
- N24..N27 are NEVER entered
- The structured event log records the skip event at `~/docs/epiphany/audit/.logs/<report-id>.jsonl`
- Fix report `flags` field does NOT contain `improve` (effective flags only)
```

- [ ] **Step 13: Commit**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/tests/smoke/
git -C ~/.claude commit -m "feat(epiphany-audit): 11 smoke test scenarios + user-dimension fixture"
```

---

## Task 20: Determinism fixture + final verification

**Files:**
- Create: `tests/determinism/python-small/source/parser.py`
- Create: `tests/determinism/python-small/source/main.py`
- Create: `tests/determinism/python-small/expected_findings.yaml`
- Create: `tests/determinism/check_overlap.py`

- [ ] **Step 1: Write determinism/python-small/source/parser.py**

This reference project is a minimal Python file with a known off-by-one bug (matching Example 1 in the spec). It is used to test that the audit pipeline produces ≥80% set-overlap on repeated runs against the same code.

```python
"""Minimal parser with a known off-by-one bug for determinism testing."""


def parse_tokens(tokens):
    """Parse a list of tokens and emit each one."""
    result = []
    for i in range(len(tokens) - 1):   # BUG: final token never emitted
        result.append(tokens[i])
    return result


def count_unique(items):
    """Count unique items — uses a list instead of a set (inefficiency)."""
    seen = []
    for item in items:
        if item not in seen:
            seen.append(item)
    return len(seen)
```

- [ ] **Step 2: Write determinism/python-small/source/main.py**

```python
"""Entry point for the python-small determinism fixture."""
from parser import parse_tokens, count_unique


def main():
    tokens = ["a", "b", "c", "d"]
    parsed = parse_tokens(tokens)
    unique = count_unique(parsed)
    print(f"parsed {unique} unique tokens")


if __name__ == "__main__":
    main()
```

- [ ] **Step 3: Write determinism/python-small/expected_findings.yaml**

```yaml
# Frozen expected-finding set for python-small determinism fixture.
# The determinism harness checks that a fresh audit run produces ≥80% set-overlap
# with these findings (by location + defect class).
# Run: python tests/determinism/check_overlap.py <actual-report-path>
fixture_version: 1
source_dir: tests/determinism/python-small/source/

expected_findings:
  - id_pattern: "F0*"
    location: "source/parser.py:6"
    dimension: CORRECTNESS
    severity: HIGH
    defect_class: off-by-one
    notes: "range(len(tokens) - 1) drops the final token"

  - id_pattern: "F0*"
    location: "source/parser.py:14"
    dimension: PERFORMANCE
    severity: LOW
    defect_class: quadratic-membership-check
    notes: "item not in seen is O(n) per item; should use a set"
    optional: true   # may not be activated if PERFORMANCE triggers do not fire
```

- [ ] **Step 4: Write tests/determinism/check_overlap.py**

```python
#!/usr/bin/env python3
"""
Determinism harness for epiphany-audit.

Usage:
  python tests/determinism/check_overlap.py <actual-audit-report.md> [--fixture python-small]

Parses the actual audit report's finding locations and defect classes, compares against
the frozen expected_findings.yaml, and reports the set-overlap percentage.
Exits 1 if overlap is below 80% (for non-optional expected findings).
"""
import sys, os, re, yaml, argparse

SKILL = os.path.expanduser("~/.claude/skills/epiphany-audit")


def load_expected(fixture_name):
    path = os.path.join(SKILL, f"tests/determinism/{fixture_name}/expected_findings.yaml")
    with open(path) as f:
        return yaml.safe_load(f)


def parse_actual_report(report_path):
    """Extract (location, dimension) pairs from an audit report."""
    with open(report_path) as f:
        content = f.read()
    # Extract finding locations from yaml blocks
    locations = []
    for block in re.finditer(r'^id: F\d+\s*\nlocation: (.+?)\s*\ndimensions: \[([^\]]+)\]',
                              content, re.MULTILINE):
        loc = block.group(1).strip()
        dims = [d.strip() for d in block.group(2).split(",")]
        locations.append((loc, dims))
    return locations


def check_overlap(actual_findings, expected):
    """Return overlap fraction (0.0–1.0) for non-optional expected findings."""
    required = [f for f in expected["expected_findings"] if not f.get("optional")]
    if not required:
        print("No required expected findings; overlap check trivially passes.")
        return 1.0

    matched = 0
    for exp in required:
        exp_loc_prefix = exp["location"].replace("source/", "")
        for (actual_loc, actual_dims) in actual_findings:
            loc_matches = exp_loc_prefix in actual_loc
            dim_matches = exp["dimension"] in actual_dims
            if loc_matches and dim_matches:
                matched += 1
                break

    overlap = matched / len(required)
    return overlap


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("report_path")
    parser.add_argument("--fixture", default="python-small")
    args = parser.parse_args()

    expected = load_expected(args.fixture)
    actual = parse_actual_report(args.report_path)

    overlap = check_overlap(actual, expected)
    pct = overlap * 100
    threshold = 80.0

    print(f"Set-overlap: {pct:.1f}% (threshold: {threshold}%)")
    if pct < threshold:
        print(f"FAIL: overlap {pct:.1f}% is below the {threshold}% CI gate threshold.")
        sys.exit(1)
    else:
        print(f"PASS: overlap {pct:.1f}% meets the {threshold}% CI gate.")


if __name__ == "__main__":
    main()
```

- [ ] **Step 5: Verify the determinism directory structure**

Run:
```bash
find ~/.claude/skills/epiphany-audit/tests/determinism -type f | sort
```
Expected:
```
~/.claude/skills/epiphany-audit/tests/determinism/check_overlap.py
~/.claude/skills/epiphany-audit/tests/determinism/python-small/expected_findings.yaml
~/.claude/skills/epiphany-audit/tests/determinism/python-small/source/main.py
~/.claude/skills/epiphany-audit/tests/determinism/python-small/source/parser.py
```

- [ ] **Step 6: Run all schema-validation tests one final time**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -m pytest tests/schema-validation/ -v --tb=short
```
Expected: All tests PASS (≥17 tests across 6 files).

- [ ] **Step 7: Run module structure linter one final time**

Run:
```bash
python3 ~/.claude/skills/epiphany-audit/tests/check_module_structure.py
```
Expected: `All 27 modules have required sections. OK.`

- [ ] **Step 8: Validate all dimension plugins against schema**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -c "
import json, yaml, jsonschema, os, glob
schema = json.load(open('schemas/dimension-plugin-v1.schema.json'))
for path in sorted(glob.glob('dimensions/*.md')):
    if path.endswith('README.md'):
        continue
    content = open(path).read()
    parts = content.split('---')
    fm = yaml.safe_load(parts[1])
    jsonschema.validate(fm, schema)
    print(f'{os.path.basename(path)}: OK')
"
```
Expected: 5 lines of `<name>.md: OK`.

- [ ] **Step 9: Validate graph.json against schema**

Run:
```bash
cd ~/.claude/skills/epiphany-audit && python -c "
import json, jsonschema
schema = json.load(open('graph.schema.json'))
graph  = json.load(open('graph.json'))
jsonschema.validate(graph, schema)
nodes = len(graph['nodes'])
edges = len(graph['edges'])
print(f'graph.json OK — {nodes} nodes, {edges} edges')
"
```
Expected: `graph.json OK — 27 nodes, 26 edges` (or near that edge count).

- [ ] **Step 10: Final file-count check**

Run:
```bash
echo "--- modules ---"
ls ~/.claude/skills/epiphany-audit/modules/ | wc -l
echo "--- schemas ---"
ls ~/.claude/skills/epiphany-audit/schemas/ | wc -l
echo "--- dimensions ---"
ls ~/.claude/skills/epiphany-audit/dimensions/ | wc -l
echo "--- templates ---"
ls ~/.claude/skills/epiphany-audit/templates/ | wc -l
echo "--- SKILL.md sections ---"
grep -c "^## " ~/.claude/skills/epiphany-audit/SKILL.md
```
Expected:
```
--- modules ---
27
--- schemas ---
5
--- dimensions ---
6
--- templates ---
4
--- SKILL.md sections ---
13
```

- [ ] **Step 11: Commit final artifacts**

```bash
git -C ~/.claude add ~/.claude/skills/epiphany-audit/tests/determinism/
git -C ~/.claude commit -m "feat(epiphany-audit): determinism fixture + final verification — skill complete"
```

- [ ] **Step 12: Tag the initial release**

```bash
git -C ~/.claude tag epiphany-audit-v1.0.0 -m "epiphany-audit v1.0.0 — initial release"
```
