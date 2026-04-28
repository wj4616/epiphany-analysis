# Playbook Creator — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Transform the original playbook creator JSON into an enhanced 15-phase version with compilation blocks, handoff blocks, KB construction, failure mode tracking, context preservation, and router support.

**Architecture:** Single JSON deliverable (`playbook-creator-playbook.json`) plus updated output schema (`output-schema.json`) and a validation script. The spec at `docs/superpowers/specs/2026-03-30-playbook-creator-design.md` is the authoritative source for all content.

**Tech Stack:** JSON (output), Python (validation only). No runtime dependencies.

---

## File Structure

```
Documents/playbookdev/
├── playbook-creator-playbook.json       # MODIFY: Enhanced 15-phase playbook (was 12-phase)
├── templates/
│   └── output-schema.json               # MODIFY: Updated JSON Schema for output playbooks
└── scripts/
    └── validate_playbook.py             # CREATE: Structural validation script
```

**Reference files (read-only):**
- `docs/superpowers/specs/2026-03-30-playbook-creator-design.md` — Design spec (authoritative)
- `Documents/playbookdev/playbook-creator-playbook.json` — Original 12-phase JSON (starting point)

---

### Task 1: Create Validation Script

**Files:**
- Create: `Documents/playbookdev/scripts/validate_playbook.py`

- [ ] **Step 1: Create scripts directory**

```bash
mkdir -p /home/myuser/Documents/playbookdev/scripts
```

- [ ] **Step 2: Write the validation script**

Create `Documents/playbookdev/scripts/validate_playbook.py`:

```python
#!/usr/bin/env python3
"""Structural validator for playbook-creator-playbook.json.

Checks: JSON syntax, required top-level fields, checklist structure,
[Role] consistency, compilation blocks, handoff blocks, owner fields.
"""
import json
import re
import sys
from pathlib import Path

REQUIRED_TOP_LEVEL = [
    "title", "version", "description", "workflow_model", "roles",
    "scope", "cross_cutting_concerns", "knowledge_base", "checklists",
    "metrics", "usage_instructions", "failure_modes",
    "phase_kb_mapping", "skill_activation", "router", "context_preservation"
]

REQUIRED_COMPILATION = [
    "context_load", "role_mindset", "objective", "pre_check",
    "failure_modes_relevant"
]

REQUIRED_HANDOFF = [
    "output_artifacts", "next_phase_context", "excluded_context", "skill"
]

VALID_SEVERITIES = {"crash", "error", "degraded", "cosmetic"}


def validate(path: str) -> list[str]:
    errors = []

    # 1. JSON syntax
    try:
        with open(path) as f:
            data = json.load(f)
    except json.JSONDecodeError as e:
        return [f"JSON parse error: {e}"]

    # 2. Required top-level fields
    for field in REQUIRED_TOP_LEVEL:
        if field not in data:
            errors.append(f"Missing top-level field: {field}")

    if errors:
        return errors  # Can't continue without structure

    # 3. Roles defined
    defined_roles = set(data.get("roles", {}).keys())
    if not defined_roles:
        errors.append("No roles defined")

    # 4. Checklists structure
    role_usage: dict[str, int] = {}
    checklists = data.get("checklists", [])

    if not checklists:
        errors.append("checklists[] is empty")

    for i, phase in enumerate(checklists):
        phase_label = phase.get("title", f"checklist[{i}]")

        if "title" not in phase:
            errors.append(f"checklist[{i}]: missing title")
        if "purpose" not in phase:
            errors.append(f"{phase_label}: missing purpose")

        # Compilation block
        comp = phase.get("compilation")
        if comp is None:
            errors.append(f"{phase_label}: missing compilation block")
        else:
            for field in REQUIRED_COMPILATION:
                if field not in comp:
                    errors.append(f"{phase_label}: compilation missing '{field}'")

            # Persistent files check (skip Phase 0)
            if i > 0 and comp:
                ctx = comp.get("context_load", [])
                ctx_str = " ".join(ctx).lower()
                if "decisions-ledger" not in ctx_str:
                    errors.append(f"{phase_label}: context_load missing decisions-ledger.md")
                if "artifact-manifest" not in ctx_str:
                    errors.append(f"{phase_label}: context_load missing artifact-manifest.md")

        # Items
        items = phase.get("items", [])
        if not items:
            errors.append(f"{phase_label}: items[] is empty")

        has_gate = False
        for j, item in enumerate(items):
            title = item.get("title", "")
            item_label = f"{phase_label} item[{j}]"

            if not title:
                errors.append(f"{item_label}: missing title")
                continue

            # Owner field — required on ALL items including gates
            if "owner" not in item:
                errors.append(f"{item_label}: missing owner field")

            # [Role] extraction
            role_match = re.match(r"\[([^\]]+)\]", title)
            if not role_match:
                errors.append(f"{item_label}: title doesn't start with [Role]: {title[:50]}")
            else:
                role = role_match.group(1)
                role_usage[role] = role_usage.get(role, 0) + 1

                # Owner consistency
                owner = item.get("owner", "")
                if owner and owner != f"[{role}]":
                    errors.append(f"{item_label}: owner '{owner}' doesn't match [Role] '{role}'")

            # Gate detection
            if "gate_conditions" in item:
                has_gate = True
                if not isinstance(item["gate_conditions"], list):
                    errors.append(f"{item_label}: gate_conditions must be array")
                if "blocker_examples" not in item:
                    errors.append(f"{item_label}: gate missing blocker_examples")
                if "handoff" not in item:
                    errors.append(f"{item_label}: gate missing handoff block")
                else:
                    for field in REQUIRED_HANDOFF:
                        if field not in item["handoff"]:
                            errors.append(f"{item_label}: handoff missing '{field}'")

        # Phase 14 has no gate (documented exception)
        if not has_gate and i < len(checklists) - 1:
            errors.append(f"{phase_label}: no gate task found")

    # 5. Role consistency
    for role, count in role_usage.items():
        if role not in defined_roles:
            errors.append(f"Role [{role}] used in tasks but not defined in roles{{}}")
        if count < 3:
            errors.append(f"Role [{role}] appears only {count} time(s) (minimum 3)")

    for role in defined_roles:
        if role not in role_usage:
            errors.append(f"Role [{role}] defined but never used in any task")

    # 6. Metrics
    metrics = data.get("metrics", [])
    if not metrics:
        errors.append("metrics[] is empty")
    else:
        categories = {m.get("category") for m in metrics}
        for req in ("process", "output_quality", "domain_outcome"):
            if req not in categories:
                errors.append(f"Missing metric category: {req}")

    # 7. Failure modes
    fm = data.get("failure_modes", None)
    if fm is None:
        errors.append("failure_modes field missing")
    elif fm:  # Non-empty — validate entries
        fm_ids = set()
        for entry in fm:
            for field in ("id", "symptom", "root_cause", "fix", "prevention", "phase", "severity", "source"):
                if field not in entry:
                    errors.append(f"failure_mode {entry.get('id', '?')}: missing '{field}'")
            if entry.get("severity") and entry["severity"] not in VALID_SEVERITIES:
                errors.append(f"failure_mode {entry.get('id')}: invalid severity '{entry['severity']}'")
            fid = entry.get("id")
            if fid in fm_ids:
                errors.append(f"Duplicate FM-ID: {fid}")
            fm_ids.add(fid)

    # 8. Router
    router = data.get("router", {})
    for field in ("description", "decision_tree", "default"):
        if field not in router:
            errors.append(f"router missing '{field}'")

    # 9. Context preservation
    cp = data.get("context_preservation", {})
    for field in ("decisions_ledger", "artifact_manifest", "rules"):
        if field not in cp:
            errors.append(f"context_preservation missing '{field}'")

    # 10. Handoff chain consistency
    # Verify each phase's context_load matches the previous phase's next_phase_context
    # (plus the two implicit persistent files)
    persistent = {"decisions-ledger", "artifact-manifest"}
    for i in range(1, len(checklists)):
        prev = checklists[i - 1]
        curr = checklists[i]
        prev_label = prev.get("title", f"checklist[{i-1}]")
        curr_label = curr.get("title", f"checklist[{i}]")

        # Get previous phase's next_phase_context from its gate handoff
        prev_next = set()
        for item in prev.get("items", []):
            handoff = item.get("handoff", {})
            if "next_phase_context" in handoff:
                for f in handoff["next_phase_context"]:
                    # Normalize: strip annotations like "(full)", "(summary — ...)"
                    base = re.split(r"\s*\(", f)[0].strip().lower()
                    prev_next.add(base)

        if not prev_next:
            continue  # No handoff found — already flagged by gate check

        # Get current phase's context_load
        curr_comp = curr.get("compilation", {})
        curr_ctx = curr_comp.get("context_load", [])
        for f in curr_ctx:
            base = re.split(r"\s*\(", f)[0].strip().lower()
            # Skip persistent files (implicitly included)
            if any(p in base for p in persistent):
                continue
            # Check if this file was in the previous phase's next_phase_context
            if base not in prev_next:
                errors.append(
                    f"{curr_label}: context_load has '{f}' not in {prev_label} next_phase_context"
                )

    # 11. Artifact provenance
    # Verify context_load items were produced by some prior phase's output_artifacts
    all_artifacts: set[str] = set()
    # Phase 0 context_load is allowed to reference external inputs (commission brief)
    for i, phase in enumerate(checklists):
        phase_label = phase.get("title", f"checklist[{i}]")

        if i > 0:
            curr_comp = phase.get("compilation", {})
            for f in curr_comp.get("context_load", []):
                base = re.split(r"\s*\(", f)[0].strip().lower()
                if any(p in base for p in persistent):
                    continue
                if base not in all_artifacts:
                    errors.append(
                        f"{phase_label}: context_load has '{f}' not in any prior output_artifacts (warning)"
                    )

        # Collect this phase's output_artifacts
        for item in phase.get("items", []):
            handoff = item.get("handoff", {})
            for a in handoff.get("output_artifacts", []):
                base = re.split(r"\s*\(", a)[0].strip().lower()
                all_artifacts.add(base)
        # Also count items with "output" field
        for item in phase.get("items", []):
            out = item.get("output", "")
            if out:
                for part in out.split(","):
                    base = re.split(r"\s*\(", part)[0].strip().lower()
                    all_artifacts.add(base)

    return errors


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "playbook-creator-playbook.json"
    if not Path(path).exists():
        print(f"File not found: {path}")
        sys.exit(1)

    errors = validate(path)
    if errors:
        print(f"FAIL: {len(errors)} error(s)")
        for e in errors:
            print(f"  - {e}")
        sys.exit(1)
    else:
        print("PASS: All structural checks passed")
        sys.exit(0)
```

- [ ] **Step 3: Verify the script runs against the original JSON (expect failures — it uses old format)**

```bash
cd /home/myuser/Documents/playbookdev
python3 scripts/validate_playbook.py playbook-creator-playbook.json
```

Expected: FAIL with multiple errors (missing new fields, missing compilation blocks, etc.). This confirms the script works and shows exactly what needs to change.

- [ ] **Step 4: Commit**

```bash
git add Documents/playbookdev/scripts/validate_playbook.py
git commit -m "feat: add structural validation script for playbook creator JSON"
```

---

### Task 2: Update Output Schema

The output schema defines what playbooks PRODUCED by the creator must look like. Update it to match the spec's "Output JSON Schema (Complete)" section.

**Files:**
- Modify: `Documents/playbookdev/templates/output-schema.json`

- [ ] **Step 1: Read current schema**

```bash
cat /home/myuser/Documents/playbookdev/templates/output-schema.json
```

- [ ] **Step 2: Replace with updated schema**

Write `Documents/playbookdev/templates/output-schema.json`:

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "playbook-output-schema",
  "title": "Playbook Output Schema",
  "description": "Schema for playbooks produced by the playbook-creator-playbook. All output playbooks must conform to this structure.",
  "type": "object",
  "required": [
    "title", "version", "description", "workflow_model", "roles", "scope",
    "cross_cutting_concerns", "knowledge_base", "checklists", "metrics",
    "usage_instructions", "failure_modes", "phase_kb_mapping",
    "skill_activation", "router", "context_preservation"
  ],
  "properties": {
    "title": {
      "type": "string",
      "description": "Playbook name",
      "minLength": 1
    },
    "version": {
      "type": "integer",
      "description": "Playbook version number",
      "minimum": 1
    },
    "description": {
      "type": "string",
      "description": "1-3 sentence overview"
    },
    "workflow_model": {
      "type": "string",
      "description": "Execution model",
      "enum": ["human-in-the-loop", "fully-autonomous", "human-directed", "role-based-single-agent"]
    },
    "roles": {
      "type": "object",
      "description": "Role name to responsibility description mapping",
      "minProperties": 1,
      "additionalProperties": { "type": "string" }
    },
    "scope": {
      "type": "object",
      "required": ["in_scope", "out_of_scope", "adjacent"],
      "properties": {
        "in_scope": { "type": "array", "items": { "type": "string" } },
        "out_of_scope": { "type": "array", "items": { "type": "string" } },
        "adjacent": { "type": "array", "items": { "type": "string" } }
      }
    },
    "cross_cutting_concerns": {
      "type": "array",
      "items": { "type": "string" }
    },
    "knowledge_base": {
      "type": "object",
      "required": ["complexity", "directory_structure"],
      "properties": {
        "complexity": { "type": "string", "enum": ["flat", "structured"] },
        "layers": {
          "type": "array",
          "items": {
            "type": "object",
            "required": ["name", "domain_boundary", "authority_score", "separation_rule", "topics"],
            "properties": {
              "name": { "type": "string" },
              "domain_boundary": { "type": "string" },
              "authority_score": { "type": "number", "minimum": 0, "maximum": 1 },
              "separation_rule": { "type": "string" },
              "topics": { "type": "array", "items": { "type": "string" } }
            }
          }
        },
        "entry_schema": { "type": "object" },
        "bridge_schema": { "type": "object" },
        "population_strategy": {
          "type": "object",
          "properties": {
            "placeholder_seeding": { "type": "string" },
            "harvesting_sources": { "type": "array", "items": { "type": "string" } },
            "curation_rules": { "type": "string" },
            "sync_rules": { "type": "string" },
            "versioning_protocol": { "type": "string" }
          }
        },
        "directory_structure": { "type": "string" }
      }
    },
    "failure_modes": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["id", "symptom", "root_cause", "fix", "prevention", "phase", "severity", "source"],
        "properties": {
          "id": { "type": "string", "pattern": "^FM-\\d{3}$" },
          "symptom": { "type": "string" },
          "root_cause": { "type": "string" },
          "fix": { "type": "string" },
          "prevention": { "type": "string" },
          "phase": { "type": "string" },
          "severity": { "type": "string", "enum": ["crash", "error", "degraded", "cosmetic"] },
          "source": { "type": "string" }
        }
      }
    },
    "checklists": {
      "type": "array",
      "minItems": 1,
      "items": {
        "type": "object",
        "required": ["title", "purpose", "compilation", "items"],
        "properties": {
          "title": { "type": "string" },
          "purpose": { "type": "string" },
          "compilation": {
            "type": "object",
            "required": ["context_load", "role_mindset", "objective", "pre_check", "failure_modes_relevant"],
            "properties": {
              "context_load": { "type": "array", "items": { "type": "string" } },
              "role_mindset": { "type": "string" },
              "objective": { "type": "string" },
              "pre_check": { "type": "array", "items": { "type": "string" } },
              "failure_modes_relevant": { "type": "array", "items": { "type": "string" } }
            }
          },
          "items": {
            "type": "array",
            "minItems": 1,
            "items": {
              "type": "object",
              "required": ["title", "owner"],
              "properties": {
                "title": { "type": "string" },
                "owner": { "type": "string" },
                "description": { "type": "string" },
                "conditional": { "type": ["string", "null"] },
                "output": { "type": "string" },
                "gate_conditions": { "type": "array", "items": { "type": "string" } },
                "blocker_examples": { "type": "array", "items": { "type": "string" } },
                "handoff": {
                  "type": "object",
                  "required": ["output_artifacts", "next_phase_context", "excluded_context", "skill"],
                  "properties": {
                    "output_artifacts": { "type": "array", "items": { "type": "string" } },
                    "next_phase_context": { "type": "array", "items": { "type": "string" } },
                    "excluded_context": { "type": "array", "items": { "type": "string" } },
                    "skill": { "type": "string" }
                  }
                }
              }
            }
          }
        }
      }
    },
    "metrics": {
      "type": "array",
      "minItems": 1,
      "items": {
        "type": "object",
        "required": ["title", "description", "type", "category"],
        "properties": {
          "title": { "type": "string" },
          "description": { "type": "string" },
          "type": { "type": "string", "enum": ["metric_integer", "metric_currency", "metric_duration"] },
          "category": { "type": "string", "enum": ["process", "output_quality", "domain_outcome"] },
          "target": {},
          "measurement_method": { "type": "string" }
        }
      }
    },
    "usage_instructions": {
      "type": "object",
      "required": ["how_to_run", "session_strategy", "cost_optimization", "post_run_review"],
      "properties": {
        "how_to_run": { "type": "array", "items": { "type": "string" } },
        "session_strategy": { "type": "array", "items": { "type": "string" } },
        "cost_optimization": { "type": "array", "items": { "type": "string" } },
        "post_run_review": {
          "type": "object",
          "required": ["assess"],
          "properties": {
            "assess": { "type": "array", "items": { "type": "string" } }
          }
        }
      }
    },
    "phase_kb_mapping": {
      "type": "object",
      "additionalProperties": { "type": "array", "items": { "type": "string" } }
    },
    "skill_activation": {
      "type": "object",
      "additionalProperties": { "type": "string" }
    },
    "router": {
      "type": "object",
      "required": ["description", "decision_tree", "default"],
      "properties": {
        "description": { "type": "string" },
        "decision_tree": { "type": "array", "items": { "type": "string" } },
        "default": { "type": "string" }
      }
    },
    "context_preservation": {
      "type": "object",
      "required": ["decisions_ledger", "artifact_manifest", "rules"],
      "properties": {
        "decisions_ledger": { "type": "string" },
        "artifact_manifest": { "type": "string" },
        "rules": { "type": "array", "items": { "type": "string" } }
      }
    }
  }
}
```

- [ ] **Step 3: Validate the schema itself is valid JSON**

```bash
python3 -c "import json; json.load(open('/home/myuser/Documents/playbookdev/templates/output-schema.json')); print('Valid JSON')"
```

Expected: `Valid JSON`

- [ ] **Step 4: Commit**

```bash
git add Documents/playbookdev/templates/output-schema.json
git commit -m "feat: update output schema with compilation blocks, failure modes, router, context preservation"
```

---

### Task 3: Scaffold Enhanced Playbook JSON

Create the playbook JSON with all top-level fields populated and an empty `checklists` array. Tasks 4–9 will populate the phases incrementally. The remaining metadata sections (`metrics`, `usage_instructions`, etc.) are set to empty placeholders — Task 10 fills them.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json`

- [ ] **Step 1: Back up the original**

```bash
cp /home/myuser/Documents/playbookdev/playbook-creator-playbook.json /home/myuser/Documents/playbookdev/playbook-creator-playbook.json.bak
```

- [ ] **Step 2: Write scaffolded JSON**

Write `Documents/playbookdev/playbook-creator-playbook.json` with the complete top-level structure. All 15 phases have their compilation blocks and a single gate task. This is the skeleton that subsequent tasks will fill with real items.

The complete JSON is large. Build it by writing these sections in order into a single file:

**Top-level fields** (copy exactly):

```json
{
  "title": "Playbook Creator Playbook",
  "version": 2,
  "description": "Meta-playbook for creating domain-specific playbooks. Produces structured JSON playbooks through 15 phases of research, KB construction, architecture, task engineering, and validation. Works with single-agent systems where one agent fills multiple functional roles.",
  "workflow_model": "role-based-single-agent",
  "roles": {
    "Coordinator": "Phase gates, tracking, status updates, blocker escalation, decisions ledger and artifact manifest maintenance",
    "Researcher": "Domain research, best practices, SME knowledge, competitive analysis",
    "Architect": "Phase structure, task granularity, role design, dependency mapping, template design",
    "Builder": "Task titles/descriptions, JSON assembly, validation, implementation of fixes",
    "Auditor": "Quality review, scenario walkthroughs, gap analysis, stress testing, failure mode cataloging, contamination testing, final verification before handoff",
    "Stakeholder": "Purpose, scope, constraints, success criteria, business decisions, final approval"
  },
  "scope": {
    "in_scope": [
      "Creating domain-specific playbook JSON files",
      "Knowledge base architecture specification",
      "Quality audit and stress testing",
      "Pilot testing with human observation",
      "Failure mode cataloging across runs"
    ],
    "out_of_scope": [
      "Executing the produced playbook (that is the user's responsibility)",
      "Building the actual knowledge base (the creator produces the KB specification, not the KB itself)",
      "Platform-specific deployment or hosting",
      "Multi-agent orchestration (single-agent fills all roles)"
    ],
    "adjacent": [
      "Skills/prompting frameworks that may be activated per-phase",
      "Knowledge bases built from the produced KB specification",
      "CI/CD or version control for produced playbooks"
    ]
  },
  "cross_cutting_concerns": [
    "Quality Standard: Every produced playbook must be comprehensive enough that any team can execute it without ambiguity. Every task must have an owner. Every non-obvious task must have a description. Every phase must have a gate with explicit conditions. No placeholder content.",
    "Role Consistency: Roles defined at playbook start must be used consistently throughout all tasks. Every task title must start with [Role]. Roles cannot appear fewer than 3 times (orphaned) or reference undefined roles.",
    "Gate Enforcement: Phase gates are firewall points — nothing advances until all conditions are met. Each gate must list explicit conditions, blocker examples, and handoff blocks. Conditions must be verifiable, not subjective.",
    "Deliverable Tracking: Every task that produces output must name the file path. The artifact manifest tracks all files across all phases. Later phases reference earlier deliverables through handoff chains.",
    "Context Preservation: decisions-ledger.md and artifact-manifest.md are loaded in every phase and updated at every gate. No decision rationale is silently lost across session boundaries."
  ],
  "knowledge_base": {
    "complexity": "flat",
    "layers": [],
    "entry_schema": {
      "_note": "The creator itself uses no structured KB. Output playbooks define their own KB architecture in Phase 2."
    },
    "bridge_schema": {},
    "population_strategy": {
      "placeholder_seeding": "N/A — the creator has no KB to seed",
      "harvesting_sources": [],
      "curation_rules": "N/A",
      "sync_rules": "N/A",
      "versioning_protocol": "N/A"
    },
    "directory_structure": "Project folder: README.md, research/, architecture/, drafts/, audits/, testing/, final/"
  }
}
```

**Then `checklists`, `metrics`, `usage_instructions`, `failure_modes`, `phase_kb_mapping`, `skill_activation`, `router`, `context_preservation`** — these are added in subsequent tasks. For now, write them as empty placeholders to make the JSON parseable:

```json
{
  "checklists": [],
  "metrics": [],
  "usage_instructions": {
    "how_to_run": [],
    "session_strategy": [],
    "cost_optimization": [],
    "post_run_review": { "assess": [] }
  },
  "failure_modes": [],
  "phase_kb_mapping": {},
  "skill_activation": {},
  "router": {
    "description": "",
    "decision_tree": [],
    "default": ""
  },
  "context_preservation": {
    "decisions_ledger": "",
    "artifact_manifest": "",
    "rules": []
  }
}
```

Merge these into ONE valid JSON object. The first block contains keys `title` through `knowledge_base`. The second block contains keys `checklists` through `context_preservation`. Remove the closing `}` from the first block and the opening `{` from the second, join with a comma. The final file must be one JSON object with all 16 keys.

- [ ] **Step 3: Validate scaffold parses**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Top-level keys: {len(d)}')"
```

Expected: `Top-level keys: 16`

- [ ] **Step 4: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: scaffold enhanced playbook with 15-phase structure and new top-level fields"
```

---

### Task 4: Write Phases 0–1 Checklists

Add the complete Phase 0 (Commission & Scoping) and Phase 1 (Domain Research & Process Discovery) checklists to the `checklists` array. These phases are the closest to the original JSON and require role name changes (Approver → Stakeholder, Implementer → Builder, Reviewer → Auditor) plus new compilation/handoff structure. Note: Phase 1 Task 3 (Competitive audit) also changes from [Auditor] to [Researcher] per the spec — this is a role reassignment, not just a rename.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — replace empty `checklists: []` and start populating

**Spec reference:** Phase 0 (Commission & Scoping) and Phase 1 (Domain Research & Process Discovery) sections of the design spec.

- [ ] **Step 1: Write Phase 0 checklist**

Add to the `checklists` array. This is the canonical example — all subsequent phases follow this exact structure.

```json
{
  "title": "Phase 0: Commission & Scoping",
  "purpose": "Define exactly what playbook is being built, for whom, and under what constraints. Nothing else starts until this is locked.",
  "compilation": {
    "context_load": ["commission brief from user — the initial request describing what playbook to build"],
    "role_mindset": "Stakeholder — this phase is about decisions, not research or building",
    "objective": "Lock purpose, scope, constraints, and success criteria so every subsequent phase has a fixed target",
    "pre_check": ["User has provided a commission brief or verbal description of the playbook they want"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Coordinator] — Create project knowledge base folder",
      "owner": "[Coordinator]",
      "description": "Folder structure: README.md, research/, architecture/, drafts/, audits/, testing/, final/\n\nInitialize decisions-ledger.md — empty, with header: '# Decisions Ledger — append at every phase gate'\n\nInitialize artifact-manifest.md — empty, with header: '# Artifact Manifest — update at every phase gate' and columns: File | Phase | Status | Summary",
      "output": "README.md, project folder structure, decisions-ledger.md, artifact-manifest.md"
    },
    {
      "title": "[Stakeholder] — Define the playbook's purpose and problem statement",
      "owner": "[Stakeholder]",
      "description": "Answer precisely:\n- What process does this playbook automate/guide?\n- What problem does it solve? What goes wrong without it?\n- Is this new, an optimization of existing, or a variant?\n\nOne paragraph, no ambiguity.",
      "output": "README.md (purpose statement and project overview)"
    },
    {
      "title": "[Stakeholder] — Define target users and team composition",
      "owner": "[Stakeholder]",
      "description": "Who will RUN this playbook?\n- Team size, roles, skill levels\n- What tools/access do they have?\n\nThe playbook's detail level is calibrated to the least experienced person who will use it."
    },
    {
      "title": "[Stakeholder] — Define scope boundaries",
      "owner": "[Stakeholder]",
      "description": "IN SCOPE: what this playbook covers\nOUT OF SCOPE: what it does NOT cover\nADJACENT: what it connects to but doesn't own\n\nScope creep is the #1 killer of playbook quality.",
      "output": "scope.md"
    },
    {
      "title": "[Stakeholder] — Define success criteria for the playbook itself",
      "owner": "[Stakeholder]",
      "description": "Measurable criteria (e.g., 'an unfamiliar user can execute it without clarifying questions')\nQuality bar (e.g., 'every task has an owner, description, and exit criteria')\nPilot test pass/fail criteria",
      "output": "success-criteria.md"
    },
    {
      "title": "[Stakeholder] — Define strategic constraints",
      "owner": "[Stakeholder]",
      "description": "Technology constraints, methodology constraints, budget/resource constraints, timeline constraints, compliance/legal constraints.\n\nThese become universal rules that every phase must respect.",
      "output": "constraints.md"
    },
    {
      "title": "[Stakeholder] — Identify related existing playbooks",
      "owner": "[Stakeholder]",
      "description": "Playbooks covering overlapping territory, upstream/downstream dependencies, reference/inspiration sources.\n\nIf replacing an existing playbook, that playbook becomes input for Phase 1 audit."
    },
    {
      "title": "[Coordinator] — Phase gate: Purpose defined, scope locked, constraints documented, success criteria measurable, knowledge base initialized",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Purpose statement is one paragraph with no ambiguity",
        "Scope has explicit in/out/adjacent lists",
        "Success criteria are measurable, not subjective",
        "Strategic constraints documented",
        "Project knowledge base folder created",
        "decisions-ledger.md and artifact-manifest.md initialized"
      ],
      "blocker_examples": [
        "Purpose statement says 'improve the process' without specifying which process or what improvement looks like — too vague, rewrite",
        "Scope has no out-of-scope list — everything is implicitly in scope, which means scope creep is guaranteed",
        "Success criteria say 'playbook is good' — not measurable, define what 'good' means quantitatively"
      ],
      "handoff": {
        "output_artifacts": ["README.md", "scope.md", "constraints.md", "success-criteria.md", "decisions-ledger.md", "artifact-manifest.md"],
        "next_phase_context": ["README.md (full)", "scope.md", "constraints.md"],
        "excluded_context": ["Conversation about why scope decisions were made — resolved, captured in scope.md and decisions-ledger.md"],
        "skill": "none"
      }
    }
  ]
}
```

**Note on handoff `next_phase_context`:** Per the spec's implicit rule, `decisions-ledger.md` and `artifact-manifest.md` are always included in every `next_phase_context` and every `context_load` — they are NOT listed per-handoff to avoid repetition. The validation script checks for them in `context_load` directly.

- [ ] **Step 2: Write Phase 1 checklist**

Add to the `checklists` array after Phase 0:

```json
{
  "title": "Phase 1: Domain Research & Process Discovery",
  "purpose": "Understand the domain deeply enough to build a playbook for it. Research produces flat documents that Phase 2 will structure.",
  "compilation": {
    "context_load": ["README.md", "scope.md", "constraints.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Researcher — gather, synthesize, document. Breadth over depth at this stage.",
    "objective": "Produce comprehensive flat research documents covering domain processes, best practices, competitors, cross-cutting concerns, and platform specifics",
    "pre_check": ["Purpose statement is unambiguous", "scope has in/out/adjacent lists", "constraints documented"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Researcher] — Research the domain this playbook serves",
      "owner": "[Researcher]",
      "description": "Standard processes in this industry/discipline, common failure modes, regulatory/compliance requirements, standard tools and platforms, terminology glossary, KPIs professionals track.",
      "output": "research/domain-analysis.md"
    },
    {
      "title": "[Researcher] — Research industry best practices and standards",
      "owner": "[Researcher]",
      "description": "Published standards (ISO, industry frameworks), expert recommendations (books, courses, authoritative sources), common process frameworks, typical phase structures, common pitfalls and how experts avoid them.",
      "output": "research/best-practices.md"
    },
    {
      "title": "[Researcher] — Competitive playbook/template audit",
      "owner": "[Researcher]",
      "description": "Existing playbooks, templates, SOPs, checklists covering similar territory. For each: what it covers, what it does well, what it misses, structure, detail level.",
      "output": "research/competitive-templates.md"
    },
    {
      "title": "[Researcher] — Audit existing playbook(s) being replaced",
      "owner": "[Researcher]",
      "conditional": "Only if replacing an existing playbook",
      "description": "What the existing playbook does well (KEEP), what it does poorly (FIX), what it's missing (ADD), what contradicts current strategy (REMOVE).",
      "output": "audits/existing-playbook-audit.md"
    },
    {
      "title": "[Stakeholder] — Gather process knowledge from subject matter experts",
      "owner": "[Stakeholder]",
      "description": "The actual process (not documented — the real one), steps that aren't written down, common blockers and resolutions, decisions requiring judgment vs always-the-same, what they wish they'd known before their first time.",
      "output": "research/sme-interviews.md"
    },
    {
      "title": "[Researcher] — Identify cross-cutting concerns",
      "owner": "[Researcher]",
      "description": "Concerns that touch every phase, not just one. Technology/platform concerns affecting all phases. Compliance concerns applying to every task. Quality standards verified at every phase. Strategic constraints shaping every decision.\n\nCross-cutting concerns must be woven throughout the playbook, not isolated in one phase.",
      "output": "research/cross-cutting-concerns.md"
    },
    {
      "title": "[Researcher] — Identify platform/environment-specific concerns",
      "owner": "[Researcher]",
      "description": "If the playbook involves a specific platform or environment: how does data/content get stored, what are the safe modification methods, what should NEVER be done (anti-patterns), what platform-specific tools exist, what deployment/migration procedures apply.",
      "output": "research/platform-concerns.md"
    },
    {
      "title": "[Coordinator] — Synthesize research into requirements document",
      "owner": "[Coordinator]",
      "description": "Domain requirements (what the process must accomplish), quality requirements (standards and compliance), strategic requirements (business constraints from Phase 0), platform requirements (technical constraints), cross-cutting requirements, user requirements (what the target team needs to succeed).",
      "output": "research/requirements.md"
    },
    {
      "title": "[Coordinator] — Phase gate: Domain researched, requirements synthesized",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Domain analysis covers standard processes, failure modes, and terminology",
        "Best practices documented with sources",
        "Competitive templates audited with strengths/weaknesses",
        "Cross-cutting concerns identified with justification",
        "Requirements document synthesizes all research"
      ],
      "blocker_examples": [
        "Domain analysis missing key compliance requirements — research more",
        "No SME input gathered — interview at least one expert before proceeding",
        "Requirements document has 'TBD' sections — complete before proceeding"
      ],
      "handoff": {
        "output_artifacts": ["research/domain-analysis.md", "research/best-practices.md", "research/competitive-templates.md", "research/cross-cutting-concerns.md", "research/platform-concerns.md", "research/sme-interviews.md", "research/requirements.md", "audits/existing-playbook-audit.md (if applicable)"],
        "next_phase_context": ["research/requirements.md (full)", "research/cross-cutting-concerns.md (full)", "research/domain-analysis.md (full — needed for KB layer classification)", "scope.md", "constraints.md"],
        "excluded_context": ["SME interview transcripts — synthesized into requirements", "Rejected competitive templates — audit captured what matters", "best-practices.md and platform-concerns.md — key points captured in requirements.md"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 3: Validate JSON parses with 2 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 2`

- [ ] **Step 4: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phase 0-1 checklists with compilation blocks and handoff blocks"
```

---

### Task 5: Write Phase 2 Checklist (KB Construction — NEW)

This phase is entirely new — it has no counterpart in the original JSON. It converts flat research into a structured KB architecture specification.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — append to `checklists` array

**Spec reference:** Phase 2 (Knowledge Base Construction) section of the design spec.

- [ ] **Step 1: Write Phase 2 checklist**

Append to the `checklists` array:

```json
{
  "title": "Phase 2: Knowledge Base Construction",
  "purpose": "Convert flat research into a structured, queryable knowledge specification. This phase produces a KB ARCHITECTURE (blueprint), not the KB itself. The actual KB gets built when the output playbook is executed.",
  "compilation": {
    "context_load": ["research/requirements.md", "research/cross-cutting-concerns.md", "research/domain-analysis.md", "scope.md", "constraints.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Architect — structuring knowledge, not gathering it. Classify, separate, define schemas.",
    "objective": "Produce a KB architecture blueprint (layers, schemas, bridge, population strategy) that an agent can build from without ambiguity",
    "pre_check": ["Domain analysis exists with distinct knowledge types identified", "requirements synthesized", "cross-cutting concerns documented"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Architect] — Determine KB complexity level",
      "owner": "[Architect]",
      "description": "Decision question: 'Do users of this playbook describe goals in language that differs from implementation language?'\n  YES → structured KB with bridge layer needed (e.g., audio: 'warm' → filter cutoff 0.2-0.4)\n  NO → assess if domain has enough distinct knowledge types to warrant layers\n\nDecision question: 'Does this domain have enough distinct knowledge types to justify separate layers?'\n  YES → multi-layer KB (define layers)\n  NO → flat reference (single folder of markdown files with table of contents)\n\nThe minimal path is genuinely minimal: single reference layer, no bridge, no cross-references, flat file list. A folder of markdown with a TOC. That is the floor."
    },
    {
      "title": "[Architect] — Define knowledge layers",
      "owner": "[Architect]",
      "conditional": "Only if multi-layer KB was chosen in Task 1",
      "description": "For each layer: name and domain boundary (what belongs here and what doesn't), authority score (0.0-1.0), separation rule (why this can't merge with another layer), topics within the layer.\n\nSeparation principle: knowledge that changes independently lives in separate layers.",
      "output": "kb-architecture.md"
    },
    {
      "title": "[Architect] — Define KB entry schema",
      "owner": "[Architect]",
      "description": "Exact JSON structure every entry must follow.\n\nRequired fields: id ({kb-name}_{topic}_{entry-name}), kb (which layer), topic, status (placeholder|harvested|curated|synced), version (semver), title, summary, description, source, concepts (array of {name, description, related[]}), code_blocks (array of {language, code, preserved} — if domain involves code), tags, related_topics, difficulty (beginner|intermediate|advanced), domain_relevance (1-10).\n\nAdditional domain-specific fields defined here.",
      "output": "entry-schema.json"
    },
    {
      "title": "[Architect] — Define bridge entry schema",
      "owner": "[Architect]",
      "conditional": "Only if KB complexity decision identified a need for creative-to-technical translation",
      "description": "Bridge entry required fields: category, parameters (array), intent_mappings keyed by subjective descriptor with: parameter, value_range [min, max], typical_default, confidence (0.0-1.0), source, why (human-readable rationale), anti_patterns.\n\nBridge is hybrid: pre-seeded with common mappings from research, auto-generated when agent encounters unmapped concepts, feedback-enriched when user evaluation refines entries.\n\nCombination layer: bridge entries can be composed. Cross-reference individual entries, identify compatible parameter ranges, flag conflicts between anti_patterns, propose combined recipe with lower confidence score.",
      "output": "bridge-schema.json"
    },
    {
      "title": "[Architect] — Define population strategy",
      "owner": "[Architect]",
      "description": "Placeholder seeding: what entries get created as placeholders during playbook setup (minimum one per topic per layer, with fallback content).\nHarvesting: where content comes from, quality criteria for harvested content.\nCuration rules: what must happen before harvested content becomes curated.\nSync rules: how curated entries get indexed.\nVersioning protocol: entries are append-only. Existing entries are never edited — they are superseded by new entries tagged with the relevant version. Old entries remain for reference or rollback. Queries filter by version tag.",
      "output": "population-strategy.md"
    },
    {
      "title": "[Architect] — Define index and directory structure",
      "owner": "[Architect]",
      "description": "Master index format (registry of all KBs, cross-topic mappings), per-KB index format (file listing by topic), per-KB manifest format (file status and sync history), cross-reference rules (results ranked by authority score), file naming conventions, exact directory tree.",
      "output": "directory-structure.md"
    },
    {
      "title": "[Coordinator] — Phase gate: KB architecture specified",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "KB complexity decision documented with rationale",
        "If multi-layer: every knowledge type from Phase 1 research assigned to exactly one layer",
        "If multi-layer: no two layers cover the same knowledge type (separation rule verified)",
        "Entry schema defined with all required fields, no placeholders",
        "Bridge schema defined if domain requires translation layer",
        "If bridge not needed: explicit justification documented",
        "Population pipeline defined with source, quality criteria, and curation rules",
        "Index and cross-reference structure defined with exact file formats",
        "Directory tree specified down to file naming convention"
      ],
      "blocker_examples": [
        "Chose 'structured multi-layer' for a simple ops checklist that has one knowledge type — over-engineered, switch to flat",
        "Two layers both claim 'implementation patterns' — overlapping boundaries, merge or redefine separation rules",
        "Entry schema lists fields but no example entry — an agent can't verify the schema works without a concrete example, add one"
      ],
      "handoff": {
        "output_artifacts": ["kb-architecture.md", "entry-schema.json", "bridge-schema.json (if applicable)", "population-strategy.md", "directory-structure.md"],
        "next_phase_context": ["kb-architecture.md (summary — layer names and boundaries only)", "research/requirements.md", "research/cross-cutting-concerns.md", "scope.md", "constraints.md"],
        "excluded_context": ["Full entry schema detail — Architecture phase only needs layer names for phase-to-KB mapping", "Population strategy — relevant during execution, not during process design", "domain-analysis.md — knowledge types now captured in KB layer definitions"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 2: Validate JSON parses with 3 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 3`

- [ ] **Step 3: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phase 2 (KB Construction) — new phase for structured knowledge specification"
```

---

### Task 6: Write Phases 3–5 Checklists

Add Process Architecture (Phase 3), Role Engineering (Phase 4), and Task Engineering (Phase 5). These are the "design" phases that structure the output playbook.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — append to `checklists` array

**Spec reference:** Phase 3 (Process Architecture), Phase 4 (Role Engineering), and Phase 5 (Task Engineering) sections of the design spec.

- [ ] **Step 1: Write Phase 3 checklist**

```json
{
  "title": "Phase 3: Process Architecture",
  "purpose": "Design the output playbook's phase structure, task granularity, dependencies, and gates.",
  "compilation": {
    "context_load": ["kb-architecture.md (summary — layer names and boundaries only)", "research/requirements.md", "research/cross-cutting-concerns.md", "scope.md", "constraints.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Architect — structure, design, map. Fresh session benefits from reviewing with new eyes.",
    "objective": "Design the output playbook's complete phase structure with task granularity, dependencies, gates, and document tree",
    "pre_check": ["KB complexity decided", "layers defined (if multi-layer)", "entry schema defined", "population strategy defined"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Architect] — Define phase structure for the output playbook",
      "owner": "[Architect]",
      "description": "Phase design principles:\n1. Each phase is a coherent block of work with a clear objective\n2. A phase's output is the next phase's input\n3. Every phase ends with a gate — explicit approval criteria before proceeding\n4. Early phases: understanding (audit, research). Middle: doing (build, create). Late: verifying and operating (QA, launch, monitor)\n5. Cross-cutting concerns woven into relevant phases, not isolated",
      "output": "architecture/phase-structure.md"
    },
    {
      "title": "[Architect] — Define task granularity standard",
      "owner": "[Architect]",
      "description": "A task represents ONE deliverable or ONE decision. If a task takes more than 4 hours or requires multiple skill sets, split it. If a task can't fail independently of the next task, merge them. Every task must be completable by the assigned role without ambiguity.\n\nTitle-only vs description rules:\n- TITLE ONLY: simple, obvious actions\n- SHORT DESCRIPTION: tasks with specific requirements\n- DETAILED DESCRIPTION: tasks requiring methodology, syntax, decision criteria, or multi-step procedures",
      "output": "architecture/task-granularity.md"
    },
    {
      "title": "[Architect] — Map phase dependencies",
      "owner": "[Architect]",
      "description": "For each phase: hard dependencies (must complete first), soft dependencies (should complete), parallel opportunities (can run simultaneously). Within phases: which tasks block others.",
      "output": "architecture/dependency-map.md"
    },
    {
      "title": "[Architect] — Design phase gates",
      "owner": "[Architect]",
      "description": "For each phase: what must be TRUE before the next phase starts, what deliverables must exist, what quality checks must pass.\n\nGate conditions must be VERIFIABLE, not subjective:\n- Good: 'All 15 city pages have 300+ words of unique localized content'\n- Bad: 'City pages are done'\n\nEach gate includes blocker_examples — specific scenarios that would prevent advancement.",
      "output": "architecture/phase-gates.md"
    },
    {
      "title": "[Architect] — Identify early assessment/detection phases needed",
      "owner": "[Architect]",
      "description": "Does this domain have variable starting conditions that affect the workflow? Does detection change the workflow? Does the playbook need an environment setup phase? Does it need a deconstruction/audit phase before implementation? If yes, design these as dedicated phases BEFORE any implementation work.",
      "output": "architecture/phase-structure.md (updated)"
    },
    {
      "title": "[Architect] — Define the deliverable document tree",
      "owner": "[Architect]",
      "description": "Which tasks produce documents, file path/naming conventions, which documents feed into later tasks. Every task that says 'Document in X' must have X defined here.",
      "output": "architecture/document-tree.md"
    },
    {
      "title": "[Stakeholder] — Review and approve architecture",
      "owner": "[Stakeholder]",
      "description": "Phase structure covers the full process. No gaps between phases. Phase gates are meaningful, not rubber stamps. Task granularity matches target user capability. Architecture satisfies all requirements from research/requirements.md."
    },
    {
      "title": "[Coordinator] — Phase gate: Architecture approved",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Phase structure covers full process from start to finish",
        "Task granularity standard documented and approved",
        "Dependencies mapped with no circular dependencies",
        "Every phase has a gate with verifiable conditions and blocker examples",
        "Early assessment/detection phases included if domain requires them",
        "Document tree complete — every referenced document has a defined path",
        "Architecture satisfies requirements.md"
      ],
      "blocker_examples": [
        "Phase structure missing detection phase for platform-specific playbook — add before implementation",
        "Gates are vague ('research done') — make verifiable",
        "Document tree has placeholder paths — resolve before Phase 4"
      ],
      "handoff": {
        "output_artifacts": ["architecture/phase-structure.md", "architecture/task-granularity.md", "architecture/dependency-map.md", "architecture/phase-gates.md", "architecture/document-tree.md"],
        "next_phase_context": ["architecture/phase-structure.md", "architecture/task-granularity.md", "kb-architecture.md (layer names)", "research/cross-cutting-concerns.md", "scope.md", "constraints.md"],
        "excluded_context": ["Dependency map detail — used during architecture, referenced if questions arise during task engineering", "Full requirements.md — architecture already satisfies it", "Document tree — referenced during task engineering if needed"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 2: Write Phase 4 checklist**

```json
{
  "title": "Phase 4: Role Engineering",
  "purpose": "Define the functional roles for the output playbook. Who does what, who approves what, how do roles hand off work.",
  "compilation": {
    "context_load": ["architecture/phase-structure.md", "architecture/task-granularity.md", "kb-architecture.md (layer names)", "research/cross-cutting-concerns.md", "scope.md", "constraints.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Architect — designing roles as capabilities, not org chart positions",
    "objective": "Define all functional roles, handoff points, and escalation paths for the output playbook",
    "pre_check": ["Phase structure covers full process", "task granularity standard documented", "dependencies mapped", "every phase has a gate"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Architect] — Define all roles for the output playbook",
      "owner": "[Architect]",
      "description": "For each role: name (bracket syntax: [RoleName]), one-line responsibility description, skills/knowledge required, tools/access needed, decision authority.\n\nRole design principles:\n1. Every task must have exactly one owner\n2. No role should appear fewer than 3 times — if it does, expand scope or merge\n3. Roles map to capabilities, not people\n4. Include coordination roles, QA roles, and decision roles"
    },
    {
      "title": "[Architect] — Define handoff points between roles",
      "owner": "[Architect]",
      "description": "Where does one role's output become another role's input? For each handoff: what is transferred, in what format, what constitutes a complete handoff."
    },
    {
      "title": "[Architect] — Define escalation paths",
      "owner": "[Architect]",
      "description": "When a task is blocked, who resolves it? Technical blockers, business/strategy questions, quality disputes, resource conflicts, scope questions. For single-agent systems: escalation = ask the human."
    },
    {
      "title": "[Coordinator] — Phase gate: Roles defined, handoffs documented",
      "owner": "[Coordinator]",
      "description": "Note: orphaned/undefined role verification happens in Phase 8 (JSON Assembly) after tasks are written.",
      "gate_conditions": [
        "All roles defined with responsibilities, required skills, and decision authority",
        "Role count is appropriate for playbook complexity (minimum 2, justified if more than 6)",
        "Handoff points documented for every role transition",
        "Escalation paths defined"
      ],
      "blocker_examples": [
        "Role appears in only one task — expand scope or merge with another role",
        "Role mentioned in tasks but not defined — add definition",
        "No escalation path for technical blockers — define before proceeding"
      ],
      "handoff": {
        "output_artifacts": ["architecture/role-definitions.md", "architecture/handoff-points.md", "architecture/escalation-paths.md"],
        "next_phase_context": ["architecture/role-definitions.md", "architecture/phase-structure.md", "architecture/task-granularity.md", "architecture/phase-gates.md", "kb-architecture.md (layer names — needed for writing output playbook handoff blocks)", "research/cross-cutting-concerns.md (needed for weaving concerns into tasks)"],
        "excluded_context": ["Role engineering discussion and alternatives considered — captured in role-definitions.md", "Escalation paths and handoff points — referenced during task writing if needed"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 3: Write Phase 5 checklist**

```json
{
  "title": "Phase 5: Task Engineering",
  "purpose": "Write every task in the output playbook. This is where most of the output quality is created.",
  "compilation": {
    "context_load": ["architecture/role-definitions.md", "architecture/phase-structure.md", "architecture/task-granularity.md", "architecture/phase-gates.md", "kb-architecture.md (layer names)", "research/cross-cutting-concerns.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Builder — writing concrete tasks, not designing structure. Every task must be executable by reading title + description alone.",
    "objective": "Write all task titles, descriptions, gates with handoff blocks, and weave cross-cutting concerns throughout",
    "pre_check": ["All roles defined with responsibilities", "handoff points documented", "escalation paths defined"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Builder] — Write all task titles",
      "owner": "[Builder]",
      "description": "Title format: [Role] — Action verb + specific deliverable\n\nRules:\n1. Start with [Role] bracket\n2. Use action verbs: Build, Create, Configure, Audit, Validate, Document, Review, Deploy, Test, Fix\n3. Be specific: 'Build homepage' not 'Work on homepage'\n4. Include quantity or scope where relevant\n5. Every title must be understandable without reading the description\n\nWrite all tasks in drafts/task-list-v0.1.md first. JSON comes later.",
      "output": "drafts/task-list-v0.1.md"
    },
    {
      "title": "[Builder] — Write task descriptions for all non-obvious tasks",
      "owner": "[Builder]",
      "description": "WHEN to include: task has specific requirements, requires methodology, has acceptance criteria beyond 'done', involves a decision with criteria, has anti-patterns, produces a document, has dependencies.\n\nDESCRIPTION STRUCTURE:\n- WHAT: what exactly is being produced\n- HOW: methodology, tools, commands, syntax rules\n- SPECS: quantitative requirements\n- DON'T: anti-patterns and common mistakes\n- OUTPUT: document path or deliverable name\n- VERIFY: how to confirm the task is done correctly\n\nQUALITY CHECK: Could someone unfamiliar with this project execute this task using only the description?"
    },
    {
      "title": "[Builder] — Include implementation method on all technical tasks",
      "owner": "[Builder]",
      "description": "For every implementation task, specify: which tools/interfaces to use, where content/data lives, what scope the edit has, what syntax rules or constraints apply, what NOT to do, what to test after the edit.\n\nA task that says 'build X' without specifying how is incomplete."
    },
    {
      "title": "[Builder] — Include risk levels on tasks that affect shared resources",
      "owner": "[Builder]",
      "description": "Any task that edits a global or shared element must include: SCOPE (isolated / template-level / global), BLAST RADIUS (what else is affected), RISK LEVEL (low / medium / high), ROLLBACK (how to undo), TEST AFTER (specific things to verify)."
    },
    {
      "title": "[Builder] — Write phase compilation blocks and gate tasks with handoff blocks",
      "owner": "[Builder]",
      "description": "Every phase opens with a compilation block: context_load, role_mindset, objective, pre_check, failure_modes_relevant.\n\ncontext_load is derived from the previous phase's next_phase_context plus decisions-ledger.md and artifact-manifest.md.\npre_check conditions must be verifiable — typically the previous phase's gate conditions.\nfailure_modes_relevant is [] for v1.0 playbooks.\n\nEvery phase ends with a gate task assigned to [Coordinator].\nGate format: '[Coordinator] — Phase gate: [comma-separated verifiable conditions]'\nEvery gate includes blocker_examples and handoff block: output_artifacts, next_phase_context, excluded_context, skill."
    },
    {
      "title": "[Builder] — Weave cross-cutting concerns into relevant tasks",
      "owner": "[Builder]",
      "description": "From cross-cutting concerns identified in Phase 1. Each concern must appear in at least 3 phases. Cross-cutting concerns that only appear once are audit failures."
    },
    {
      "title": "[Auditor] — Review all tasks for completeness and clarity",
      "owner": "[Auditor]",
      "description": "Binary verification checklist:\n- Does each task have a [Role] owner? Y/N\n- Is each title specific and actionable? Y/N\n- Does each non-obvious task have a description? Y/N\n- Does each description answer what, how, and how to verify? Y/N\n- Do implementation tasks include method specification? Y/N\n- Do shared-resource tasks include risk levels? Y/N\n- Is there any placeholder text (TBD, TODO)? Y/N\n- Is terminology consistent across all tasks? Y/N\n- Do file paths in descriptions match the document tree? Y/N\n- Does the [Role] in each title match the owner field? Y/N"
    },
    {
      "title": "[Coordinator] — Phase gate: All tasks engineered",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "All tasks written with titles and descriptions per granularity standard",
        "Implementation methods specified on all technical tasks",
        "Risk levels assigned on shared-resource tasks",
        "Every phase has a compilation block with context_load, role_mindset, objective, pre_check, failure_modes_relevant",
        "Every phase has a gate task with handoff block",
        "Cross-cutting concerns appear in 3+ phases each",
        "[Auditor] review passed with no unresolved issues",
        "No placeholder text anywhere"
      ],
      "blocker_examples": [
        "A task says 'Build the dashboard' with no description — violates granularity standard, needs WHAT/HOW/SPECS/VERIFY",
        "Cross-cutting concern only appears in the design phase — must also appear in build, QA, and monitoring phases",
        "Gate handoff lists next_phase_context but no excluded_context — handoff is incomplete"
      ],
      "handoff": {
        "output_artifacts": ["drafts/task-list-v0.1.md", "all gate definitions with handoffs"],
        "next_phase_context": ["drafts/task-list-v0.1.md", "architecture/phase-structure.md", "kb-architecture.md (layer names)", "architecture/role-definitions.md"],
        "excluded_context": ["Task engineering discussion and iterations — final tasks captured in task-list", "Auditor review notes — issues resolved, fixes applied"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 4: Validate JSON parses with 6 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 6`

- [ ] **Step 5: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phases 3-5 (Architecture, Roles, Task Engineering)"
```

---

### Task 7: Write Phases 6–8 Checklists

Add Output Configuration (Phase 6), Metrics (Phase 7), and JSON Assembly (Phase 8). These are the "building" phases.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — append to `checklists` array

**Spec reference:** Phase 6 (Output Configuration), Phase 7 (Metrics & KPI Definition), and Phase 8 (JSON Assembly & Validation) sections of the design spec.

- [ ] **Step 1: Write Phase 6 checklist**

```json
{
  "title": "Phase 6: Output Configuration",
  "purpose": "Define how the output playbook is used: workflow model, session strategy, KB mapping, skill activation, router, and post-run review guidance.",
  "compilation": {
    "context_load": ["drafts/task-list-v0.1.md", "architecture/phase-structure.md", "kb-architecture.md (layer names)", "architecture/role-definitions.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Architect — designing how the playbook operates, not what it contains",
    "objective": "Define workflow model, usage instructions, session strategy, KB mapping, skill activation, router, and post-run review",
    "pre_check": ["All tasks engineered with titles and descriptions", "gates have handoff blocks", "cross-cutting concerns woven into 3+ phases each", "Auditor review passed"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Architect] — Define workflow model and role descriptions",
      "owner": "[Architect]",
      "description": "Workflow model: human-in-the-loop, fully autonomous, human-directed, or custom. Role descriptions for the output playbook (from Phase 4 role definitions, condensed for the JSON)."
    },
    {
      "title": "[Architect] — Define usage instructions",
      "owner": "[Architect]",
      "description": "How to start: what context to provide for the first phase. Session strategy: which phases can share a session, where to break, what context to carry. Cost optimization: how to minimize token usage while maintaining quality. General operating rules that apply to all phases."
    },
    {
      "title": "[Architect] — Define phase-to-KB-layer mapping",
      "owner": "[Architect]",
      "description": "For each phase in the output playbook: which KB layers are relevant. No phase loads the entire KB — only the layers it needs. Must be consistent with KB layer definitions from Phase 2."
    },
    {
      "title": "[Architect] — Define skill activation mapping",
      "owner": "[Architect]",
      "description": "For each phase: which prompting skill/mode to activate (or 'none'). Must be consistent with the skill field in each gate's handoff block. For new domains without existing skills, all values are 'none'. Skills are identified after execution runs and added in subsequent playbook versions."
    },
    {
      "title": "[Architect] — Define router/dispatcher and context preservation specification",
      "owner": "[Architect]",
      "description": "Router: a short prompt or decision tree at the entry point. Given a user request, the router determines: which KB layers to load, which playbook phase applies, and which skill to activate.\n\nFor single-layer/flat KB playbooks: router is trivial.\nFor multi-layer KB playbooks: router maps request type to minimum KB slice.\n\nContext preservation: define the two persistent files for the output playbook (decisions ledger and artifact manifest) — file paths, format, update rules, what never to exclude. File paths must match what compilation blocks reference in context_load.",
      "output": "output-config.md"
    },
    {
      "title": "[Architect] — Define post-run review structure",
      "owner": "[Architect]",
      "description": "What to assess after the output playbook completes a run: what went well, what didn't, what should change, metrics target vs actual, quality assessment. This is guidance, not a template."
    },
    {
      "title": "[Coordinator] — Phase gate: Output configuration complete",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Workflow model defined",
        "Roles defined with responsibilities and decision authority",
        "Usage instructions cover how to start, where to break sessions, what context to carry",
        "Phase-to-KB-layer mapping is consistent with KB layer definitions from Phase 2",
        "Skill activation is defined for every phase (even if 'none')",
        "Router/dispatcher defined (or explicitly trivial for flat-KB playbooks)",
        "Post-run review structure defined"
      ],
      "blocker_examples": [
        "Phase-to-KB-layer mapping references a layer not defined in Phase 2 — reconcile",
        "Skill activation inconsistent with gate handoff skill fields — fix one or the other",
        "Usage instructions don't explain how to start the first session — add initial context guidance"
      ],
      "handoff": {
        "output_artifacts": ["output-config.md"],
        "next_phase_context": ["output-config.md (summary)", "architecture/phase-structure.md", "drafts/task-list-v0.1.md"],
        "excluded_context": ["Session strategy rationale — captured in output-config.md", "Alternative workflow models considered — final choice documented"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 2: Write Phase 7 checklist**

```json
{
  "title": "Phase 7: Metrics & KPI Definition",
  "purpose": "Define measurable success criteria for the output playbook.",
  "compilation": {
    "context_load": ["output-config.md (summary)", "architecture/phase-structure.md", "drafts/task-list-v0.1.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Researcher — defining what success looks like, not building it. Domain outcome metrics require understanding what the playbook is supposed to achieve.",
    "objective": "Define process, output quality, and domain outcome metrics with targets and measurement methods",
    "pre_check": ["Workflow model defined", "usage instructions defined", "KB mapping and skill activation defined", "router defined"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Researcher] — Define process metrics",
      "owner": "[Researcher]",
      "description": "Metrics measuring the execution of the playbook itself: duration metrics (time to complete, time per phase), volume metrics (tasks completed, deliverables produced), quality metrics (issues found at QA, rework count, gate failures).\n\nFor each: title, description, type (metric_integer | metric_currency | metric_duration), category ('process'), target, measurement method."
    },
    {
      "title": "[Researcher] — Define output quality metrics",
      "owner": "[Researcher]",
      "description": "Metrics measuring the QUALITY of the work produced: gate pass rate, audit findings count, description completeness, domain-specific quality measures.\n\nFor each: title, description, type, category ('output_quality'), target, measurement method."
    },
    {
      "title": "[Researcher] — Define domain outcome metrics",
      "owner": "[Researcher]",
      "description": "Metrics measuring whether the playbook achieved its PURPOSE. These are entirely domain-specific (downloads, revenue, coverage, compliance score, etc.). For optimization playbooks: include BEFORE and AFTER variants.\n\nAt least one metric in this category is mandatory — a playbook without outcome metrics can't prove it worked."
    },
    {
      "title": "[Stakeholder] — Review and approve metrics",
      "owner": "[Stakeholder]",
      "description": "Metrics align with business goals from Phase 0. Targets are ambitious but achievable. Every metric has a clear measurement method. At least one metric in each of the three categories."
    },
    {
      "title": "[Coordinator] — Phase gate: Metrics defined and approved",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "At least one process metric defined",
        "At least one output quality metric defined",
        "At least one domain outcome metric defined",
        "All metrics have title, description, type, and measurement method",
        "Targets set where applicable",
        "[Stakeholder] approved"
      ],
      "blocker_examples": [
        "No domain outcome metric defined — a playbook that can't prove it worked is incomplete",
        "Metric has no measurement method — define how to measure before proceeding"
      ],
      "handoff": {
        "output_artifacts": ["metrics-definition.md"],
        "next_phase_context": ["metrics-definition.md", "drafts/task-list-v0.1.md", "output-config.md", "scope.md", "constraints.md", "research/cross-cutting-concerns.md", "kb-architecture.md (full)", "entry-schema.json", "bridge-schema.json (if applicable)", "population-strategy.md", "directory-structure.md", "architecture/role-definitions.md", "architecture/phase-structure.md"],
        "excluded_context": ["Metric alternatives discussed and rejected — final metrics captured", "Task granularity and dependency docs — content reflected in the task list"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 3: Write Phase 8 checklist**

```json
{
  "title": "Phase 8: JSON Assembly & Validation",
  "purpose": "Build the complete output playbook JSON and verify it is valid, complete, and consistent.",
  "compilation": {
    "context_load": ["metrics-definition.md", "drafts/task-list-v0.1.md", "output-config.md", "scope.md", "constraints.md", "research/cross-cutting-concerns.md", "kb-architecture.md (full)", "entry-schema.json", "bridge-schema.json (if applicable)", "population-strategy.md", "directory-structure.md", "architecture/role-definitions.md", "architecture/phase-structure.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Builder — assembling, validating, fixing. Mechanical precision required.",
    "objective": "Assemble the complete output playbook JSON with all fields, validate syntax/structure/consistency, include compilation blocks on every phase",
    "pre_check": ["At least one metric per category defined", "all metrics approved by Stakeholder"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Builder] — Assemble the playbook JSON structure",
      "owner": "[Builder]",
      "description": "Required top-level fields: title, version, description, workflow_model, roles, scope, cross_cutting_concerns, knowledge_base, checklists, metrics, usage_instructions, failure_modes, phase_kb_mapping, skill_activation, router, context_preservation.\n\nChecklist item fields: title (required, starts with [Role]), owner (required, must match [Role] in title), description (optional), conditional (optional), output (optional).\n\nGate item additional fields: gate_conditions (required array), blocker_examples (required array), handoff (required object with output_artifacts, next_phase_context, excluded_context, skill)."
    },
    {
      "title": "[Builder] — Validate JSON syntax",
      "owner": "[Builder]",
      "description": "Parse without errors. No trailing commas, unescaped characters, mismatched brackets. All string values properly escaped.\n\nVerification: python3 -c \"import json; json.load(open('playbook.json'))\""
    },
    {
      "title": "[Builder] — Validate structural completeness",
      "owner": "[Builder]",
      "description": "Every checklist has title, purpose, and non-empty items array. Every item has a title starting with [Role]. Every item has an owner field matching the [Role] in title. Phase numbering is sequential. metrics[] is not empty. All required top-level fields present and non-empty."
    },
    {
      "title": "[Builder] — Validate [Role] consistency",
      "owner": "[Builder]",
      "description": "Extract all [Role] references from task titles. Compare against roles defined in the roles{} section. No typos, no orphaned roles (appear < 3 times), no undefined roles. Every owner field matches the [Role] in its task title."
    },
    {
      "title": "[Builder] — Validate KB schema section",
      "owner": "[Builder]",
      "description": "If knowledge_base.layers is non-empty: every layer has name, domain boundary, authority score, separation rule. Entry schema has all required fields. Bridge schema (if present) has all required fields. Population strategy is complete. Directory structure is buildable.\n\nIf knowledge_base is minimal/flat: at minimum a description of the reference format and location."
    },
    {
      "title": "[Builder] — Validate compilation blocks on every phase",
      "owner": "[Builder]",
      "description": "Every checklist entry has a compilation block with all required fields. context_load always includes decisions-ledger.md and artifact-manifest.md (except Phase 0). pre_check conditions are verifiable (not subjective). role_mindset is defined. objective is a single clear line. failure_modes_relevant references valid FM-IDs (or is empty for v1.0). Each phase's context_load is consistent with the previous phase's next_phase_context."
    },
    {
      "title": "[Builder] — Validate summary views match gate sources",
      "owner": "[Builder]",
      "description": "phase_kb_mapping entries must be consistent with next_phase_context in gate handoffs. skill_activation entries must match skill field in gate handoffs. Flag any inconsistencies as errors."
    },
    {
      "title": "[Builder] — Validate failure_modes section",
      "owner": "[Builder]",
      "description": "failure_modes array exists (may be empty for v1.0). If populated: every entry has all required fields (id, symptom, root_cause, fix, prevention, phase, severity, source). severity values valid: crash | error | degraded | cosmetic. phase values reference actual phases. FM-IDs in compilation blocks reference existing entries. No duplicate FM-IDs."
    },
    {
      "title": "[Auditor] — Manual review of assembled JSON",
      "owner": "[Auditor]",
      "description": "Read through entire playbook as a user would: Do tasks flow logically within each phase? Are descriptions consistent in tone and detail level? Would you know how to complete every task from title + description alone? Are metrics measurable with the tools/access defined?"
    },
    {
      "title": "[Coordinator] — Phase gate: JSON valid and complete",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "JSON parses without errors",
        "All structural validation checks pass",
        "All [Role] references are consistent and defined",
        "KB schema section is valid and complete (or explicitly minimal)",
        "Every phase has a compilation block with all required fields",
        "Compilation context_load is consistent with previous phase's next_phase_context",
        "failure_modes section valid (empty or well-formed)",
        "Summary views (phase_kb_mapping, skill_activation) match gate handoffs",
        "Manual review passed"
      ],
      "blocker_examples": [
        "JSON has trailing comma after last array element — parser rejects it, fix syntax",
        "A [Role] appears in 2 task titles but is not in the roles{} section — undefined role, fix",
        "phase_kb_mapping says Phase 3 uses 'dsp-kb' but no gate handoff references that layer — reconcile"
      ],
      "handoff": {
        "output_artifacts": ["drafts/playbook-v0.1.json"],
        "next_phase_context": ["drafts/playbook-v0.1.json (full)", "research/requirements.md", "research/cross-cutting-concerns.md"],
        "excluded_context": ["All intermediate drafts — consolidated into JSON", "Validation logs — issues fixed"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 4: Validate JSON parses with 9 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 9`

- [ ] **Step 5: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phases 6-8 (Output Config, Metrics, JSON Assembly)"
```

---

### Task 8: Write Phases 9–10 Checklists

Add Gap Analysis (Phase 9) and Stress Testing (Phase 10). These are the quality audit phases. Key changes from original: all audit tasks assigned to [Auditor] (not [Coordinator]), KB buildability audit added, Auditor verification after fixes.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — append to `checklists` array

**Spec reference:** Phase 9 (Quality Audit — Gap Analysis) and Phase 10 (Quality Audit — Stress Testing) sections of the design spec.

- [ ] **Step 1: Write Phase 9 checklist**

```json
{
  "title": "Phase 9: Quality Audit — Gap Analysis",
  "purpose": "Systematic audit of the assembled playbook against requirements, cross-cutting concerns, phase flow, contradictions, completability, and detail level. All audit tasks use binary yes/no checklists.",
  "compilation": {
    "context_load": ["drafts/playbook-v0.1.json", "research/requirements.md", "research/cross-cutting-concerns.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Auditor — reviewing with fresh eyes, not defending previous work. Binary yes/no judgments, no hedging.",
    "objective": "Systematically audit the playbook for requirements gaps, cross-cutting coverage, phase flow, contradictions, completability, detail level, and KB integrity",
    "pre_check": ["JSON parses without errors", "all structural validations pass", "all Role references consistent"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Auditor] — Audit: Does the playbook satisfy all requirements?",
      "owner": "[Auditor]",
      "description": "For each requirement in research/requirements.md: Is it addressed by at least one task? Y/N. Is the task specific enough to satisfy the requirement? Y/N.",
      "output": "audits/requirements-gap-analysis.md"
    },
    {
      "title": "[Auditor] — Audit: Are all cross-cutting concerns woven throughout?",
      "owner": "[Auditor]",
      "description": "For each concern in research/cross-cutting-concerns.md: How many phases reference it? If < 3 phases: flagged as insufficiently woven.",
      "output": "audits/cross-cutting-audit.md"
    },
    {
      "title": "[Auditor] — Audit: Are there gaps between phases?",
      "owner": "[Auditor]",
      "description": "Walk through sequentially: Does Phase N's output fully satisfy Phase N+1's input needs? Y/N per transition. Is there work falling between phases? Y/N. Are there implicit tasks not written down? Y/N. Could a new user follow this playbook without asking questions? Y/N.",
      "output": "audits/phase-gap-analysis.md"
    },
    {
      "title": "[Auditor] — Audit: Are there contradictions?",
      "owner": "[Auditor]",
      "description": "Check for: Tasks contradicting strategic constraints Y/N. Tasks contradicting the implementation method guidance Y/N. Metrics targets contradicting each other Y/N. Description details conflicting across tasks Y/N.",
      "output": "audits/contradiction-audit.md"
    },
    {
      "title": "[Auditor] — Audit: Is every task assignable and completable?",
      "owner": "[Auditor]",
      "description": "For each task: Is the assigned role capable of this work? Y/N. Can the task be completed independently (or are dependencies explicit)? Y/N. Does the task have clear 'done' criteria? Y/N. Does the task require information that no previous task produces? Y/N.",
      "output": "audits/task-completability-audit.md"
    },
    {
      "title": "[Auditor] — Audit: Is the detail level appropriate?",
      "owner": "[Auditor]",
      "description": "Check calibration: Any simple tasks over-described? Y/N (list them). Any complex tasks under-described? Y/N (list them). Any implementation tasks missing method specification? Y/N. Any shared-resource tasks missing risk levels? Y/N. Would the least experienced target user understand every task? Y/N.",
      "output": "audits/detail-level-audit.md"
    },
    {
      "title": "[Auditor] — Audit: Is the KB specification buildable and integrity-safe?",
      "owner": "[Auditor]",
      "description": "Walk through knowledge_base section: Can an agent create the directory structure from the spec alone? Y/N. Can an agent create a valid KB entry from the schema alone? Y/N. Can an agent populate a bridge entry from the bridge schema alone? Y/N. Is the population strategy actionable? Y/N.\n\nLayer contamination checks (if multi-layer): Do technical layer entries contain subjective/creative language? Y/N (contamination if yes). Do creative/design layer entries contain code or implementation class names? Y/N. Does every bridge entry reference valid entries in both source KBs? Y/N. Do any bridge entries contradict each other? Y/N.",
      "output": "audits/kb-buildability-audit.md"
    },
    {
      "title": "[Auditor] — Compile audit findings into prioritized fix list",
      "owner": "[Auditor]",
      "description": "CRITICAL: contradictions, missing phases, broken dependencies. HIGH: cross-cutting gaps, incomplete descriptions, orphaned roles, unbuildable KB spec. MEDIUM: detail level issues, consistency problems. LOW: formatting, naming conventions, minor wording.",
      "output": "audits/fix-list.md"
    },
    {
      "title": "[Coordinator] — Phase gate: All audits complete, fix list compiled",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "All 7 audits completed with documented findings",
        "Fix list compiled and prioritized",
        "No unexamined areas"
      ],
      "blocker_examples": [
        "Requirements not covered — add tasks or clarify",
        "Cross-cutting concern only in one phase — weave throughout",
        "Phase gaps identified — add missing tasks"
      ],
      "handoff": {
        "output_artifacts": ["audits/requirements-gap-analysis.md", "audits/cross-cutting-audit.md", "audits/phase-gap-analysis.md", "audits/contradiction-audit.md", "audits/task-completability-audit.md", "audits/detail-level-audit.md", "audits/kb-buildability-audit.md", "audits/fix-list.md"],
        "next_phase_context": ["audits/fix-list.md", "drafts/playbook-v0.1.json"],
        "excluded_context": ["Individual audit reports — findings consolidated into fix-list.md. Only reference individual reports if a fix needs more context."],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 2: Write Phase 10 checklist**

```json
{
  "title": "Phase 10: Quality Audit — Stress Testing",
  "purpose": "Walk through the playbook under different scenarios to find issues that gap analysis misses.",
  "compilation": {
    "context_load": ["audits/fix-list.md", "drafts/playbook-v0.1.json", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Auditor — adversarial thinking. Try to break the playbook by simulating real usage scenarios.",
    "objective": "Walk through the playbook under happy path, domain novice, KB construction, blocker, variant, and edge case scenarios",
    "pre_check": ["All 7 gap analysis audits completed", "fix list compiled and prioritized"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Auditor] — Scenario walkthrough: Happy path",
      "owner": "[Auditor]",
      "description": "Execute the playbook mentally with no problems: Can you start Phase 0 with only information available at project start? Y/N. Does each phase's output give the next phase everything it needs? Y/N. At each gate, are conditions verifiable? Y/N. At the end, has the playbook produced everything required? Y/N.",
      "output": "testing/scenario-happy-path.md"
    },
    {
      "title": "[Auditor] — Scenario walkthrough: Domain novice",
      "owner": "[Auditor]",
      "description": "Walk through as someone who has NEVER done this type of work: Can they understand and complete their assigned tasks? Y/N. Do descriptions give enough context? Y/N. Are tools and methods explained? Y/N. Would they know what to do when stuck? Y/N.",
      "output": "testing/scenario-domain-novice.md"
    },
    {
      "title": "[Auditor] — Scenario walkthrough: KB construction by unfamiliar agent",
      "owner": "[Auditor]",
      "description": "Walk through KB specification as agent encountering domain for first time: Can you build directory structure from spec? Y/N. Can you create a valid entry from schema? Y/N. Can you populate a bridge entry from bridge schema? Y/N. Can you run a cross-reference lookup? Y/N.\n\nContamination test: write a sample entry for each layer and verify it naturally stays within its domain boundary.",
      "output": "testing/scenario-kb-construction.md"
    },
    {
      "title": "[Auditor] — Scenario walkthrough: Blocker scenarios",
      "owner": "[Auditor]",
      "description": "Simulate common blockers: What if access/tools are unavailable? What if the domain has unusual or undocumented aspects? What if QA fails at a phase gate? What if requirements change mid-execution? Does the playbook handle these? Are escalation paths clear?",
      "output": "testing/scenario-blockers.md"
    },
    {
      "title": "[Auditor] — Scenario walkthrough: Domain variants",
      "owner": "[Auditor]",
      "description": "If the playbook covers variants of the domain: Walk through with Variant A and Variant B. Do all tasks make sense for each variant?",
      "output": "testing/scenario-variants.md"
    },
    {
      "title": "[Auditor] — Edge case identification",
      "owner": "[Auditor]",
      "description": "Minimal case (smallest possible project) — does it still work? Maximal case (largest, most complex) — does it scale? Legacy case (old technology/process) — does it handle it? Migration case (moving from old to new) — is there a path?",
      "output": "testing/edge-cases.md"
    },
    {
      "title": "[Builder] — Fix all CRITICAL and HIGH issues from audit and stress testing",
      "owner": "[Builder]"
    },
    {
      "title": "[Builder] — Fix MEDIUM issues from audit and stress testing",
      "owner": "[Builder]"
    },
    {
      "title": "[Builder] — Re-validate JSON after all fixes",
      "owner": "[Builder]"
    },
    {
      "title": "[Auditor] — Verify fixes don't introduce new issues",
      "owner": "[Auditor]",
      "description": "Spot-check that CRITICAL/HIGH fixes are clean — no new contradictions, no regressions. Verify fix-list.md entries are resolved, not just closed. If issues found: return to [Builder], then re-verify.",
      "output": "audits/stress-test-verification.md"
    },
    {
      "title": "[Coordinator] — Phase gate: Stress testing complete, fixes applied, Auditor verified",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "All scenario walkthroughs completed",
        "Edge cases documented",
        "All CRITICAL and HIGH issues fixed",
        "MEDIUM issues fixed",
        "JSON re-validated after fixes",
        "[Auditor] verification passed — no regressions from fixes"
      ],
      "blocker_examples": [
        "CRITICAL issues remain — fix before proceeding",
        "Happy path fails at specific phase — diagnose and fix",
        "Auditor found regression from fix — return to Builder"
      ],
      "handoff": {
        "output_artifacts": ["testing/scenario-happy-path.md", "testing/scenario-domain-novice.md", "testing/scenario-kb-construction.md", "testing/scenario-blockers.md", "testing/scenario-variants.md", "testing/edge-cases.md", "audits/stress-test-verification.md", "drafts/playbook-v0.2.json"],
        "next_phase_context": ["drafts/playbook-v0.2.json", "success-criteria.md", "audits/fix-list.md (resolved status)"],
        "excluded_context": ["Scenario walkthrough details — issues extracted and fixed", "Pre-fix JSON draft — superseded by v0.2"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 3: Validate JSON parses with 11 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 11`

- [ ] **Step 4: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phases 9-10 (Gap Analysis, Stress Testing) with Auditor role assignments"
```

---

### Task 9: Write Phases 11–14 Checklists

Add Stakeholder Review (Phase 11), Pilot Test (Phase 12), Documentation (Phase 13), and Continuous Improvement (Phase 14). Key changes: Auditor verification tasks in Phases 11-12, failure mode cataloging in Phase 12, Phase 14 has no gate.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — append to `checklists` array

**Spec reference:** Phase 11 (Stakeholder Review), Phase 12 (Pilot Test), Phase 13 (Documentation & Version Control), and Phase 14 (Continuous Improvement) sections of the design spec.

- [ ] **Step 1: Write Phase 11 checklist**

```json
{
  "title": "Phase 11: Stakeholder Review & Iteration",
  "purpose": "Final human review before pilot testing.",
  "compilation": {
    "context_load": ["drafts/playbook-v0.2.json", "success-criteria.md", "audits/fix-list.md (resolved status)", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Stakeholder — business alignment, strategic review. Does this playbook achieve what was commissioned?",
    "objective": "Human review of playbook against business strategy, roles, and success criteria. Specific feedback, then final approval.",
    "pre_check": ["All scenario walkthroughs completed", "all CRITICAL/HIGH/MEDIUM issues fixed", "JSON re-validated"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Stakeholder] — Review playbook against business strategy",
      "owner": "[Stakeholder]",
      "description": "Does the playbook produce the right outcome? Are strategic constraints respected throughout? Are metrics aligned with business KPIs? Is the playbook parameterized properly (per-run variables, not hardcoded)? Would you trust this playbook to run without supervising every step?"
    },
    {
      "title": "[Stakeholder] — Review roles and ownership",
      "owner": "[Stakeholder]",
      "description": "Roles match available capabilities. No role is overloaded. Decision authority is correctly placed. Coordination role has enough visibility."
    },
    {
      "title": "[Stakeholder] — Provide feedback and change requests",
      "owner": "[Stakeholder]",
      "description": "Specific, actionable feedback: tasks to add, remove, or modify; description changes needed; phase structure changes; metric or target adjustments.\n\n'Make it better' is not feedback. Specific changes only."
    },
    {
      "title": "[Builder] — Implement stakeholder feedback",
      "owner": "[Builder]"
    },
    {
      "title": "[Builder] — Re-validate JSON after changes",
      "owner": "[Builder]"
    },
    {
      "title": "[Auditor] — Verify stakeholder changes don't introduce regressions",
      "owner": "[Auditor]",
      "description": "Spot-check that feedback implementation is clean — no new contradictions, no broken handoffs. Verify compilation blocks still consistent after any phase structure changes. If issues found: return to [Builder], then re-verify."
    },
    {
      "title": "[Stakeholder] — Final approval of playbook",
      "owner": "[Stakeholder]",
      "description": "Satisfies success criteria from Phase 0. All feedback addressed. Business strategy respected throughout. Would you run this playbook tomorrow with confidence?"
    },
    {
      "title": "[Coordinator] — Phase gate: Stakeholder approved, Auditor verified",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "All stakeholder feedback addressed",
        "[Auditor] verification passed — no regressions from changes",
        "Final approval given",
        "JSON valid after changes",
        "Success criteria from Phase 0 are satisfied"
      ],
      "blocker_examples": [
        "Stakeholder says 'looks fine' without reviewing specific phases — not approval, need documented review",
        "Feedback says 'add more detail to Phase 3' without specifying which tasks — not actionable, request specifics"
      ],
      "handoff": {
        "output_artifacts": ["drafts/playbook-v0.3.json", "stakeholder-feedback.md"],
        "next_phase_context": ["drafts/playbook-v0.3.json"],
        "excluded_context": ["All feedback discussion — changes applied to JSON", "Previous draft versions — superseded"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 2: Write Phase 12 checklist**

```json
{
  "title": "Phase 12: Pilot Test",
  "purpose": "Run the playbook on a real project to find issues that design review can't catch. The pilot is human-observed — the agent executes, the human watches for confusion, gaps, and friction.",
  "compilation": {
    "context_load": ["drafts/playbook-v0.3.json", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Coordinator — executing and observing. Document friction in real-time, don't rationalize it away.",
    "objective": "Run the playbook on a real low-stakes project, document friction, incorporate feedback, get Auditor final sign-off",
    "pre_check": ["All stakeholder feedback addressed", "final approval given", "JSON valid", "success criteria from Phase 0 satisfied"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Coordinator] — Select pilot project",
      "owner": "[Coordinator]",
      "description": "Choose a real but low-stakes project that exercises most phases. Define what 'success' looks like for the pilot."
    },
    {
      "title": "[Coordinator] — Execute pilot run",
      "owner": "[Coordinator]",
      "description": "Run the playbook on the selected project. Execute at normal pace — don't rush. Human observes and documents: tasks unclear or needing more detail, tasks taking longer than expected, tasks in wrong order, tasks that don't apply (conditional tasks needed?), phase gates too strict or too loose, KB specification sections hard to follow during construction. Track time per task and per phase."
    },
    {
      "title": "[Auditor] — Document pilot friction and catalog failure modes",
      "owner": "[Auditor]",
      "description": "Questions that arose that the playbook should answer. Tasks over-described (can be simplified). Tasks under-described (need more detail). Dependency issues (wrong order). Missing tasks. Gate adjustments needed.\n\nFor every bug, crash, or unexpected behavior during the pilot: catalog as failure mode entry (symptom, root_cause, fix, prevention, phase, severity, source). Prevention field becomes a candidate checklist item or gate condition. Phase field assigns the failure mode to where it should be caught.",
      "output": "testing/pilot-friction.md, testing/failure-modes-pilot.md"
    },
    {
      "title": "[Builder] — Incorporate pilot feedback into playbook",
      "owner": "[Builder]",
      "description": "Common updates: clarifying descriptions, reordering tasks, adjusting scope, fixing gate conditions, updating KB spec based on build experience."
    },
    {
      "title": "[Builder] — Re-validate and produce final JSON",
      "owner": "[Builder]"
    },
    {
      "title": "[Auditor] — Final verification before handoff",
      "owner": "[Auditor]",
      "description": "The Auditor is always the last role to touch the playbook before it leaves the creator process.\n\nVerify: all pilot fixes are clean — no new contradictions. JSON still validates. KB specification still buildable after pilot-driven updates. No regression from earlier audit fixes. Gate conditions accurate after task modifications.\n\nIf issues found: return to [Builder] for fixes, then re-verify. Do not advance.",
      "output": "audits/final-verification.md"
    },
    {
      "title": "[Coordinator] — Phase gate: Pilot complete, feedback incorporated, Auditor verified",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Pilot run completed on a real project",
        "Friction documented",
        "Feedback incorporated into playbook",
        "Updated JSON validated",
        "[Auditor] final verification passed — no regressions, no new contradictions"
      ],
      "blocker_examples": [
        "Pilot not completed — cannot skip real-world testing",
        "Auditor found regression from pilot fixes — return to Builder",
        "Failure modes from pilot not cataloged — document before proceeding"
      ],
      "handoff": {
        "output_artifacts": ["final/playbook-v1.0.json", "testing/pilot-friction.md", "testing/failure-modes-pilot.md", "audits/final-verification.md"],
        "next_phase_context": ["final/playbook-v1.0.json"],
        "excluded_context": ["All pilot execution history — findings captured in pilot-friction.md and testing/failure-modes-pilot.md, applied to JSON", "Previous draft versions"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 3: Write Phase 13 checklist**

```json
{
  "title": "Phase 13: Documentation & Version Control",
  "purpose": "Package the final playbook with supporting documentation.",
  "compilation": {
    "context_load": ["final/playbook-v1.0.json", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Coordinator — packaging and archiving. Completeness and clarity for future users.",
    "objective": "Create changelog, archive research/audit docs, store production JSON, write quick-start guide",
    "pre_check": ["Pilot completed", "friction documented", "feedback incorporated", "Auditor final verification passed"],
    "failure_modes_relevant": []
  },
  "items": [
    {
      "title": "[Coordinator] — Create playbook changelog",
      "owner": "[Coordinator]",
      "description": "Version history with dates and change summaries: v0.1 (initial draft), v0.2 (post-audit fixes), v0.3 (post-stakeholder feedback), v1.0 (post-pilot, production ready).",
      "output": "final/CHANGELOG.md"
    },
    {
      "title": "[Coordinator] — Archive all research and audit documents",
      "owner": "[Coordinator]",
      "description": "Preserve: research/, architecture/, audits/, testing/. These are the 'why' behind design decisions."
    },
    {
      "title": "[Builder] — Store production playbook JSON",
      "owner": "[Builder]",
      "description": "Final JSON in designated location. Naming: {playbook-name}-playbook.json. Never overwrite without backup of previous version."
    },
    {
      "title": "[Coordinator] — Create quick-start guide",
      "owner": "[Coordinator]",
      "description": "One-page guide: what this playbook is for, prerequisites before starting, how to start a run, key roles, phase overview (one line per phase), where to find documentation.",
      "output": "final/QUICKSTART.md"
    },
    {
      "title": "[Coordinator] — Phase gate: Documentation complete",
      "owner": "[Coordinator]",
      "gate_conditions": [
        "Changelog written",
        "Research and audit docs archived",
        "JSON in version control",
        "Quick-start guide complete"
      ],
      "blocker_examples": [
        "Changelog missing version history — document all versions",
        "Quick-start guide missing prerequisites — add before finalizing"
      ],
      "handoff": {
        "output_artifacts": ["final/CHANGELOG.md", "final/QUICKSTART.md", "final/playbook-v1.0.json"],
        "next_phase_context": ["final/playbook-v1.0.json", "final/QUICKSTART.md"],
        "excluded_context": ["All creation process artifacts — archived for reference, not needed for ongoing use"],
        "skill": "none"
      }
    }
  ]
}
```

- [ ] **Step 4: Write Phase 14 checklist**

Note: Phase 14 has no gate — it is a recurring process with no completion state.

```json
{
  "title": "Phase 14: Continuous Improvement",
  "purpose": "After each completed run of the output playbook, review and optionally improve it. Manual and human-initiated — no automatic feedback loops.",
  "compilation": {
    "context_load": ["final/playbook-v1.0.json (or latest version)", "final/QUICKSTART.md", "decisions-ledger.md", "artifact-manifest.md"],
    "role_mindset": "Coordinator/Stakeholder — reviewing execution data, deciding what to change",
    "objective": "After each playbook run, collect review data, catalog failure modes, decide whether to update the playbook",
    "pre_check": ["At least one run of the output playbook has been completed"],
    "failure_modes_relevant": ["all — this is where failure modes are reviewed and new ones cataloged"]
  },
  "items": [
    {
      "title": "[Coordinator] — After each playbook run: collect review data",
      "owner": "[Coordinator]",
      "description": "Review the post-run assessment (using the post_run_review structure from Output Configuration). Compare actual metrics to targets. Note tasks consistently skipped, reordered, or added ad-hoc. Note where the KB spec was insufficient during construction."
    },
    {
      "title": "[Coordinator] — Catalog failure modes from this run",
      "owner": "[Coordinator]",
      "description": "For every bug, crash, unexpected behavior, or degraded result encountered during the run: create a failure mode entry (id: FM-NNN sequential, symptom, root_cause, fix, prevention, phase, severity, source).\n\nseverity: crash (process cannot continue), error (wrong output), degraded (works but quality reduced), cosmetic (visual/formatting).\nprevention: write as a concrete checklist item or gate condition.\nphase: assign to the output playbook phase where this should be prevented or detected.\n\nAdd entries to failure_modes[] array. Update relevant phase compilation blocks with new FM-IDs."
    },
    {
      "title": "[Stakeholder] — Decide whether to update the playbook",
      "owner": "[Stakeholder]",
      "description": "Review collected data and new failure modes. Decide: update now, update later, or no changes needed. If updating: specify what changes to make and why. This is a human decision — the agent does not auto-modify the playbook."
    },
    {
      "title": "[Builder] — Implement playbook updates",
      "owner": "[Builder]",
      "conditional": "Only if Stakeholder approved updates",
      "description": "Follow same quality standards: validate JSON, check structural completeness, verify no introduced contradictions. Integrate failure mode prevention items into relevant phase tasks or gate conditions. Update changelog."
    }
  ]
}
```

- [ ] **Step 5: Validate JSON parses with all 15 phases**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Phases: {len(d[\"checklists\"])}')"
```

Expected: `Phases: 15`

- [ ] **Step 6: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add Phases 11-14 (Stakeholder Review, Pilot, Documentation, Continuous Improvement)"
```

---

### Task 10: Add Metadata Sections

Populate the remaining top-level sections: `metrics`, `usage_instructions`, `failure_modes`, `phase_kb_mapping`, `skill_activation`, `router`, and `context_preservation`.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` — replace the placeholder sections from Task 3

**Spec reference:** Output JSON Schema (Complete), Schema Rules, Creator's Own Metrics, and Creator's Own Usage Instructions sections of the design spec.

- [ ] **Step 1: Replace `metrics` array**

```json
"metrics": [
  {
    "title": "Total Phases",
    "description": "Phases in the produced playbook",
    "type": "metric_integer",
    "category": "output_quality",
    "target": null,
    "measurement_method": "Count checklists[] array length"
  },
  {
    "title": "Total Tasks",
    "description": "Tasks across all phases in the produced playbook",
    "type": "metric_integer",
    "category": "output_quality",
    "target": null,
    "measurement_method": "Count all items[] across checklists"
  },
  {
    "title": "Tasks With Descriptions",
    "description": "Percentage of tasks that have descriptions",
    "type": "metric_integer",
    "category": "output_quality",
    "target": 70,
    "measurement_method": "(tasks with description / total tasks) x 100"
  },
  {
    "title": "Metrics Defined",
    "description": "KPI metrics defined in the produced playbook (never zero)",
    "type": "metric_integer",
    "category": "output_quality",
    "target": 6,
    "measurement_method": "Count metrics[] array length"
  },
  {
    "title": "Days to Complete",
    "description": "Calendar days from commission (Phase 0) to documentation (Phase 13)",
    "type": "metric_integer",
    "category": "process",
    "target": 10,
    "measurement_method": "Calendar days between Phase 0 start and Phase 13 gate"
  },
  {
    "title": "Audit Issues Found",
    "description": "Total issues found during quality audit phases (lower = better initial drafting)",
    "type": "metric_integer",
    "category": "process",
    "target": null,
    "measurement_method": "Count entries in audits/fix-list.md"
  },
  {
    "title": "Critical Issues at Pilot",
    "description": "Issues discovered during pilot that should have been caught earlier",
    "type": "metric_integer",
    "category": "process",
    "target": 0,
    "measurement_method": "Count issues in testing/pilot-friction.md and testing/failure-modes-pilot.md rated critical/high"
  },
  {
    "title": "Pilot Completion Rate",
    "description": "Percentage of tasks in the pilot run completed as written (no ad-hoc modifications)",
    "type": "metric_integer",
    "category": "domain_outcome",
    "target": 90,
    "measurement_method": "(tasks completed as-written / total tasks attempted) x 100"
  }
]
```

- [ ] **Step 2: Replace `usage_instructions`**

```json
"usage_instructions": {
  "how_to_run": [
    "Load this playbook as context for a planning session",
    "A single agent fills all roles — switch mental context per phase",
    "Work through phases sequentially — each phase gate must pass before the next",
    "For each task: read the title and description, complete the work, verify completion",
    "At phase gates: verify all conditions are met before proceeding",
    "Final output: a domain-specific playbook JSON following output-schema.json"
  ],
  "session_strategy": [
    "Phases 0-1: Share a session (scoping and research are tightly coupled)",
    "Phase 2 (KB Construction): Focused session — schema design requires sustained architectural thinking",
    "Phase 3: Fresh session with only requirements, cross-cutting concerns, and KB layer names as context",
    "Phases 4-5 (Role + Task Engineering): Share a session — roles inform task writing directly",
    "Phases 6-8 (Output Config, Metrics, JSON Assembly): Share a session",
    "Phases 9-10 (Audits): Fresh session — reviewing with fresh eyes catches more",
    "Phase 11 (Stakeholder Review): Human review gate — break session here",
    "Phase 12 (Pilot): Full run — this will be multiple sessions",
    "Phases 13-14 (Docs, Improvement): Complete the cycle"
  ],
  "cost_optimization": [
    "Use compilation blocks — only load what the phase needs",
    "Respect excluded_context — don't re-load what was deliberately dropped",
    "For large playbooks: break JSON Assembly into multiple sessions (assemble, then validate)",
    "Audit phases benefit from fresh context — don't carry forward build-phase thinking"
  ],
  "post_run_review": {
    "assess": [
      "What went well in the process",
      "What didn't go well",
      "What should change in the playbook",
      "Metrics: target vs actual for every defined metric",
      "Quality assessment: were gates effective, were tasks clear, were roles right",
      "Catalog any failure modes encountered during the run"
    ]
  }
}
```

- [ ] **Step 3: Set `failure_modes` to empty array**

```json
"failure_modes": []
```

This is a v2.0 playbook that hasn't been run yet. Failure modes are added after execution runs.

- [ ] **Step 4: Replace `phase_kb_mapping`**

The creator itself has a flat KB (no structured layers). This mapping is for completeness — the output playbooks will define their own.

```json
"phase_kb_mapping": {
  "phase_0": [],
  "phase_1": [],
  "phase_2": [],
  "phase_3": [],
  "phase_4": [],
  "phase_5": [],
  "phase_6": [],
  "phase_7": [],
  "phase_8": [],
  "phase_9": [],
  "phase_10": [],
  "phase_11": [],
  "phase_12": [],
  "phase_13": [],
  "phase_14": []
}
```

- [ ] **Step 5: Replace `skill_activation`**

```json
"skill_activation": {
  "phase_0": "none",
  "phase_1": "none",
  "phase_2": "none",
  "phase_3": "none",
  "phase_4": "none",
  "phase_5": "none",
  "phase_6": "none",
  "phase_7": "none",
  "phase_8": "none",
  "phase_9": "none",
  "phase_10": "none",
  "phase_11": "none",
  "phase_12": "none",
  "phase_13": "none",
  "phase_14": "none"
}
```

- [ ] **Step 6: Replace `router`**

```json
"router": {
  "description": "The playbook creator is a sequential process — the router is trivial. Start at Phase 0 and proceed through each phase in order.",
  "decision_tree": [
    "New playbook commission → Start at Phase 0",
    "Resume interrupted run → Check artifact-manifest.md for latest phase, resume there",
    "Post-run improvement → Go directly to Phase 14"
  ],
  "default": "Start at Phase 0 with a commission brief"
}
```

- [ ] **Step 7: Replace `context_preservation`**

```json
"context_preservation": {
  "decisions_ledger": "decisions-ledger.md — append-only, one paragraph per phase gate: phase number, key decisions, constraints discovered, rejected alternatives with brief rationale",
  "artifact_manifest": "artifact-manifest.md — running index of every file: path, producing phase, status (active|superseded|archived), one-line summary",
  "rules": [
    "Both files are initialized in Phase 0 and updated at every gate",
    "Both files are always included in context_load — exempt from token optimization cuts",
    "The decisions ledger captures WHY; the artifact manifest captures WHERE",
    "Together they replace the need to carry full documents forward 'just in case'"
  ]
}
```

- [ ] **Step 8: Validate JSON parses with all sections**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(f'Keys: {len(d)}, Phases: {len(d[\"checklists\"])}, Metrics: {len(d[\"metrics\"])}')"
```

Expected: `Keys: 16, Phases: 15, Metrics: 8`

- [ ] **Step 9: Commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json
git commit -m "feat: add metrics, usage instructions, router, context preservation, and all metadata sections"
```

---

### Task 11: Final Validation and Cleanup

Run the validation script, fix any issues, and clean up.

**Files:**
- Modify: `Documents/playbookdev/playbook-creator-playbook.json` (if fixes needed)

- [ ] **Step 1: Run validation script**

```bash
cd /home/myuser/Documents/playbookdev
python3 scripts/validate_playbook.py playbook-creator-playbook.json
```

Expected: `PASS: All structural checks passed`

If any errors, fix them in the JSON and re-run until PASS.

- [ ] **Step 2: Count tasks and verify completeness**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "
import json
d = json.load(open('playbook-creator-playbook.json'))
total_tasks = 0
for phase in d['checklists']:
    items = len(phase['items'])
    total_tasks += items
    print(f'{phase[\"title\"]}: {items} items')
print(f'\nTotal: {total_tasks} tasks across {len(d[\"checklists\"])} phases')
print(f'Roles: {list(d[\"roles\"].keys())}')
print(f'Metrics: {len(d[\"metrics\"])}')
"
```

Expected output should show approximately:
- Phase 0: 8 items
- Phase 1: 9 items
- Phase 2: 7 items
- Phase 3: 8 items
- Phase 4: 4 items
- Phase 5: 8 items
- Phase 6: 7 items
- Phase 7: 5 items
- Phase 8: 10 items
- Phase 9: 9 items
- Phase 10: 11 items
- Phase 11: 8 items
- Phase 12: 7 items
- Phase 13: 5 items
- Phase 14: 4 items
- Total: ~110 tasks across 15 phases
- Roles: Coordinator, Researcher, Architect, Builder, Auditor, Stakeholder
- Metrics: 8

- [ ] **Step 3: Verify JSON can be pretty-printed cleanly**

```bash
cd /home/myuser/Documents/playbookdev
python3 -c "import json; d=json.load(open('playbook-creator-playbook.json')); print(json.dumps(d, indent=2))" | head -20
```

Verify the output is clean, well-formatted JSON.

- [ ] **Step 4: Remove backup file**

```bash
rm /home/myuser/Documents/playbookdev/playbook-creator-playbook.json.bak
```

- [ ] **Step 5: Final commit**

```bash
git add Documents/playbookdev/playbook-creator-playbook.json Documents/playbookdev/templates/output-schema.json Documents/playbookdev/scripts/validate_playbook.py
git commit -m "feat: complete enhanced playbook creator v2 — 15 phases, compilation blocks, handoff blocks, KB construction, failure mode tracking, context preservation, router"
```
