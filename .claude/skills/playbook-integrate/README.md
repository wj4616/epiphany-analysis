# playbook-integrate

Integrate session execution data into PBCPB playbooks. Extracts failures, patterns, and decisions from cctrace XML session data and produces versioned playbook updates with full cross-reference integrity.

## Quick Start

```bash
# Basic usage - integrate a single session
playbook-integrate --playbook playbook.json --session session.xml

# Audit mode - analyze without modifying
playbook-integrate --playbook playbook.json --session session.xml --audit

# Process multiple sessions
playbook-integrate --playbook playbook.json --session-dir ./sessions/

# Verify playbook integrity
playbook-integrate --verify-only --playbook playbook.json
```

## What It Does

The skill runs a 4-pass pipeline:

1. **Pattern Extraction** - Parses session data, extracts failures, decisions, workflow patterns, knowledge references
2. **Phase Correlation** - Matches extractions to playbook phases, detects phase boundaries
3. **Integrity Validation** - Validates cross-references, checks ID uniqueness, detects contradictions
4. **Playbook Generation** - Merges validated extractions, assigns new IDs, rebuilds indexes

## Output

| File | Purpose |
|------|---------|
| `playbook-v{N+1}.json` | Updated playbook with integrated content |
| `integration_report.json` | Summary of what was integrated |
| `.playbook-integrate-cache/` | Intermediate files for resume |
| `audits/` | Full audit trail |

## Modes

| Mode | Flags | Purpose |
|------|-------|---------|
| Single | `--session <path>` | Process one session file |
| Batch | `--session-dir <dir>` | Process multiple sessions |
| Audit | `--audit` | Analyze without modifying |
| Verify | `--verify-only` | Check playbook integrity |
| Status | `--status` | Show playbook statistics |
| Resume | `--resume` | Continue interrupted run |

## Session Input Formats

- **cctrace XML** - Primary format from Claude Code session exports
- **JSON** - Structured session data
- **JSONL** - Line-delimited session logs

## Knowledge Directory

Optional supplementary knowledge:

```
--knowledge-dir ./knowledge/
├── failures/*.json      # Known failure patterns
├── patterns/*.json       # Reusable workflow patterns
├── kb/**/*.json          # Domain knowledge
├── phases/*.json         # Custom phase definitions
└── vocabulary.json       # Terms and synonyms
```

## Contradiction Handling

- **Minor contradictions** (case variations, wording): Auto-resolved using playbook version
- **Major contradictions** (gate conflicts, logic mismatches): Blocked for human resolution

## Output Files

| File | Purpose |
|------|---------|
| `playbook-v{N+1}.json` | Updated playbook with integrated content |
| `integration_report.json` | Summary of what was integrated |
| `.playbook-integrate-cache-<hash>/` | Intermediate files for resume |
| `audits/` | Full audit trail |

## Resume Support

If interrupted, resume with:

```bash
playbook-integrate --playbook playbook.json --session session.xml --resume
```

Clean slate:

```bash
playbook-integrate --playbook playbook.json --session session.xml --clean
```

## Example Integration

**Input session shows:**
```
Error: 'SmoothedValue' was not declared
User: "that's wrong, we need to include the header"
```

**Output adds to playbook:**
```json
{
  "id": "FM-048",
  "symptom": "SmoothedValue not declared error",
  "root_cause": "Missing #include for juce_SmoothedValue.h",
  "fix": "Add #include at top of module header",
  "prevention": "Check includes before using JUCE classes",
  "phase": "Phase 4",
  "severity": "minor"
}
```

## Schemas

All output schemas are in `templates/`:
- `raw_extractions_schema.json` - Pass 1 output
- `correlated_extractions_schema.json` - Pass 2 output
- `validation_report_schema.json` - Pass 3 output
- `integration_report_schema.json` - Pass 4 output
- `resume_state_schema.json` - Cache state
- `full_audit_report_schema.json` - Consolidated audit

## Related Skills

- `kb-harvest` - Populate KB entries from web sources
- `kb-sync` - Verify KB consistency
- `kb-route` - Query KB during playbook execution

## Spec

Full specification: `docs/superpowers/specs/2026-04-08-playbook-integrate-skill-design.md`