# Playbook-Integrate Skill Design

**Status:** Approved  
**Created:** 2026-04-08  
**Version:** 1.0.0

## Overview

The `playbook-integrate` skill integrates session execution data into PBCPB playbooks. It extracts failures, patterns, decisions, and workflow improvements from cctrace session data and produces versioned playbook updates with full cross-reference integrity and comprehensive audit trails.

## Skill Header

```yaml
---
name: playbook-integrate
description: "Integrate session execution data into PBCPB playbooks. Extracts failures, patterns, and decisions from cctrace XML session data and produces versioned playbook updates with full cross-reference integrity."
---
```

## Invocation

```
playbook-integrate --playbook <path> --session <path>                    # Primary invocation
playbook-integrate --playbook <path> --session-dir <dir>                 # Multiple sessions in directory
playbook-integrate --playbook <path> --session-json <json>               # Inline JSON session data
playbook-integrate --playbook <path> --knowledge-dir <path>              # Load domain knowledge
playbook-integrate --playbook <path> --session <path> --knowledge-dir <path>  # Combined
playbook-integrate --verify-only                                         # Check playbook integrity
playbook-integrate --status                                              # Show playbook statistics
playbook-integrate --audit                                              # Audit without applying changes
playbook-integrate --resume                                             # Continue from last pass
playbook-integrate --clean                                              # Remove cache before starting
```

## Defaults

| Flag | Default |
|------|---------|
| `--playbook` | REQUIRED — no default |
| `--session` | REQUIRED if no `--session-dir` or `--session-json` |
| `--output-dir` | Same directory as playbook |
| `--version-increment` | `minor` (X.Y → X.Y+1) |
| `--knowledge-dir` | None (use playbook's knowledge_base section) |

## Mode Routing

| Flags Present | Mode | Jump To |
|---------------|------|---------|
| `--verify-only` | Verify playbook integrity | Verify Mode |
| `--status` | Show statistics | Status Mode |
| `--audit` | Audit without applying | Audit Mode |
| `--session-dir <dir>` | Multi-session batch | Batch Mode |
| `--session-json <json>` | Inline JSON session | Single Mode |
| `--session <path>` | Single session file | Single Mode |

## Input Formats

The skill accepts multiple session data formats:

### Primary Format: cctrace XML

The standard cctrace XML export from Claude Code sessions:

```xml
<?xml version="1.0" ?>
<claude-session xmlns="https://claude.ai/session-export/v1" export-version="1.0">
  <metadata>
    <session-id>uuid</session-id>
    <working-directory>/path</working-directory>
    <start-time>ISO timestamp</start-time>
    <end-time>ISO timestamp</end-time>
    <statistics>
      <total-messages>N</total-messages>
      <user-messages>N</user-messages>
      <assistant-messages>N</assistant-messages>
      <tool-uses>N</tool-uses>
    </statistics>
  </metadata>
  <messages>
    <message uuid="..." timestamp="...">
      <event-type>user|assistant|tool-use</event-type>
      <content>
        <text>...</text>
        <thinking>...</thinking>
        <tool-use name="...">...</tool-use>
      </content>
    </message>
  </messages>
</claude-session>
```

### Secondary Formats

- **JSON session data**: Structured JSON with same structure as cctrace XML
- **JSONL session logs**: Line-delimited JSON from Claude Code's native logging
- **Directory of artifacts**: Multiple session files processed in batch

### Knowledge Directory

Optional `--knowledge-dir <path>` provides supplementary knowledge:

| Type | Pattern | Purpose |
|------|---------|---------|
| Failure catalogs | `failures/*.json` | Known failure patterns |
| Pattern libraries | `patterns/*.json` | Reusable workflow patterns |
| KB layers | `kb/**/*.json` | Domain knowledge |
| Phase definitions | `phases/*.json` | Custom phase definitions |
| Vocabulary | `vocabulary.json` | Terms and synonyms |

## Multi-Pass Pipeline

```
┌─────────────────────────────────────────────────────────────────────┐
│                     PLAYBOOK-INTEGRATE PIPELINE                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  INPUT: Playbook JSON + Session Data + Optional Knowledge            │
│                                                                      │
│  PASS 1: Pattern Extraction                                         │
│     • Parse session structure                                       │
│     • Extract failures, decisions, patterns, references            │
│     • Build session index                                           │
│     • Output: raw_extractions.json + pass_1_audit.json              │
│                                                                      │
│  PASS 2: Phase Correlation                                          │
│     • Match extractions to playbook phases                          │
│     • Detect phase boundaries                                       │
│     • Identify unmatched items                                      │
│     • Output: correlated_extractions.json + pass_2_audit.json        │
│                                                                      │
│  PASS 3: Integrity Validation                                       │
│     • Validate cross-references                                     │
│     • Check ID uniqueness                                           │
│     • Detect contradictions                                         │
│     • Output: validation_report.json + pass_3_audit.json            │
│                                                                      │
│  PASS 4: Playbook Generation                                        │
│     • Merge validated extractions                                   │
│     • Assign new IDs                                                │
│     • Rebuild indexes                                               │
│     • Generate gap analysis                                         │
│     • Output: playbook-v{N+1}.json + integration_report.json        │
│                                                                      │
│  OUTPUT: Updated Playbook + Integration Report + Audit Trail        │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

## Pass 1: Pattern Extraction

**Input:** cctrace XML, optional knowledge directory

**Processing:**

1. **Parse Session Structure**
   - Load cctrace manifest
   - Parse messages in order
   - Extract: user messages, assistant responses, tool calls
   - Track file history
   - AUDIT: Log parsing errors, malformed XML, skipped content

2. **Extract Failure Patterns**
   - Error messages in tool results
   - User corrections ("that's wrong", "no, try again")
   - Explicit failure mentions ("crashed", "doesn't work")
   - Stack traces and error codes
   - Build attempts with errors
   - AUDIT: Log extraction confidence, ambiguous matches

3. **Extract Decision Points**
   - User approval moments
   - User rejections
   - Clarification requests
   - Multiple-choice selections
   - Human-only creative decisions
   - AUDIT: Log heuristic matching decisions, confidence scores

4. **Extract Workflow Patterns**
   - Repeated task sequences
   - Tool invocation patterns
   - File access patterns
   - Context restatements at session starts
   - AUDIT: Log pattern detection thresholds, repetition counts

5. **Extract Knowledge References**
   - Mentions of framework classes/APIs
   - Mentions of domain concepts
   - KB layer references
   - AUDIT: Log vocabulary matches, synonym expansions

6. **Build Session Index**
   - Message ID → message content
   - Timestamp → message ID
   - File → messages that touched it
   - AUDIT: Log index completeness, orphan references

**Output:** `raw_extractions.json` + `pass_1_audit.json`

### Pass 1 Audit Schema

```json
{
  "pass": 1,
  "pass_name": "pattern_extraction",
  "status": "complete|partial|failed",
  "timing": {"start": "ISO", "end": "ISO", "duration_ms": 1234},
  "parsing": {
    "xml_valid": true,
    "messages_parsed": 350,
    "messages_skipped": 2,
    "skip_reasons": [{"message_index": 23, "reason": "..."}]
  },
  "extractions": {
    "failures": {"count": 12, "by_type": {...}, "confidence_scores": [...]},
    "decisions": {"count": 8, "by_type": {...}, "human_only_flagged": 5},
    "workflow_patterns": {"count": 6, "patterns_found": [...]},
    "knowledge_references": {"count": 23, "vocabulary_matches": [...]},
    "session_boundaries": {"count": 3, "context_restatements": 2}
  },
  "warnings": [...],
  "errors": []
}
```

## Pass 2: Phase Correlation

**Input:** raw_extractions.json, original playbook, optional knowledge directory

**Processing:**

1. **Load Playbook Phases**
   - Parse phase definitions, gates, checklists
   - Build phase index
   - Extract phase keywords
   - AUDIT: Log phase count, gate conditions per phase

2. **Match Failures to Phases**
   - Match by task context (keywords)
   - Match by file context (which files edited)
   - Match by timestamp (if session has phase markers)
   - AUDIT: Log matching heuristics, confidence scores

3. **Match Decisions to Phases**
   - Match approvals/rejections to nearest gate condition
   - Match creative decisions to human_only tasks
   - AUDIT: Log decision-to-task mappings

4. **Match Patterns to Phases**
   - Match workflow patterns to phase sequences
   - Match knowledge references to KB layers
   - AUDIT: Log pattern-to-phase mappings

5. **Detect Phase Boundaries**
   - Identify session_start → phase transitions
   - Identify gate condition completions
   - Identify explicit phase mentions
   - AUDIT: Log detected vs expected boundaries

6. **Identify Unmatched Items**
   - Collect extractions with no phase match
   - Categorize by reason (no context, ambiguous, novel)
   - Add to gap analysis

**Output:** `correlated_extractions.json` + `pass_2_audit.json`

### Pass 2 Audit Schema

```json
{
  "pass": 2,
  "pass_name": "phase_correlation",
  "status": "complete|partial|failed",
  "timing": {...},
  "playbook_analysis": {
    "phases_loaded": 13,
    "total_gate_conditions": 47,
    "total_checklist_items": 89,
    "human_only_tasks": 23,
    "kb_layers": [...]
  },
  "phase_matching": {
    "failures": {"matched": 10, "unmatched": 2, "by_phase": {...}},
    "decisions": {"matched": 8, "unmatched": 0},
    "workflow_patterns": {"matched": 5, "unmatched": 1}
  },
  "phase_boundaries": {
    "detected": [...],
    "expected": 13,
    "missing_boundaries": [...]
  },
  "unmatched_items": [...],
  "warnings": [...],
  "errors": []
}
```

## Pass 3: Integrity Validation

**Input:** correlated_extractions.json, original playbook, optional knowledge directory

**Processing:**

1. **Validate Cross-References**
   - Verify target IDs exist
   - Verify target phases exist
   - Verify KB layers exist
   - Check bidirectional references
   - AUDIT: Log all reference validations

2. **Validate ID Uniqueness**
   - Generate proposed new IDs
   - Check against existing IDs
   - Resolve conflicts by incrementing
   - AUDIT: Log ID assignments, conflicts resolved

3. **Detect Contradictions**
   - Compare session vs playbook content
   - Categorize: minor (documentation), major (logic conflict)
   - AUDIT: Log all contradictions with severity

4. **Verify KB Layer References**
   - Check knowledge references point to valid layers
   - Check knowledge directory entries well-formed
   - AUDIT: Log KB reference validations

5. **Verify Structural Integrity**
   - All phases have required fields
   - All checklists have required fields
   - All failure modes have required fields
   - AUDIT: Log structural validation results

6. **Build Integration Plan**
   - List items ready for integration
   - List items blocked by issues
   - List items needing human resolution
   - AUDIT: Log integration plan summary

**Output:** `validation_report.json` + `pass_3_audit.json`

### Pass 3 Audit Schema

```json
{
  "pass": 3,
  "pass_name": "integrity_validation",
  "status": "complete|partial|failed",
  "timing": {...},
  "cross_reference_validation": {
    "total_proposed": 45,
    "valid": 43,
    "invalid": 2,
    "invalid_details": [...]
  },
  "id_uniqueness": {
    "proposed_new_ids": [...],
    "conflicts_resolved": [...],
    "next_available_ids": {...}
  },
  "contradictions": {
    "total": 2,
    "by_severity": {"minor": 1, "major": 1},
    "details": [...]
  },
  "kb_validation": {...},
  "structural_integrity": {...},
  "integration_plan": {
    "ready_for_integration": [...],
    "blocked": [...],
    "needs_human_resolution": [...]
  },
  "warnings": [...],
  "errors": [...]
}
```

## Pass 4: Playbook Generation

**Input:** validation_report.json, original playbook

**Processing:**

1. **Load Original Playbook**
   - Parse complete playbook JSON
   - Deep copy for modification
   - Track all changes for diff
   - AUDIT: Log playbook version, section counts

2. **Merge Failure Modes**
   - Assign final IDs
   - Add to failure_modes array
   - Add to phase.checklist as prevention rules
   - Create cross-references
   - AUDIT: Log each addition, location

3. **Merge Workflow Patterns**
   - Add new patterns to workflow_patterns section
   - Update phase checklists
   - AUDIT: Log pattern additions

4. **Merge Decisions**
   - Add human_only decisions to decisions ledger
   - Update phase gates with decision evidence
   - AUDIT: Log decision additions

5. **Update Compilation Blocks**
   - Add failure_modes to compilation.failure_modes_relevant
   - Update context_load with new artifacts
   - Update success_criteria
   - AUDIT: Log compilation block updates

6. **Update Phase Handoffs**
   - Update carry_forward based on session_boundaries
   - Update drop based on content_dropped
   - AUDIT: Log phase handoff updates

7. **Rebuild Indexes**
   - Rebuild failure_mode → phase index
   - Rebuild CCC → phase index
   - Rebuild KB layer → topic index
   - AUDIT: Log index rebuild statistics

8. **Generate Gap Analysis**
   - Identify thin-coverage phases
   - List unmatched extractions
   - Generate recommended questions
   - AUDIT: Log gap analysis items

9. **Increment Version and Write Output**
   - Increment playbook version
   - Write playbook-v{N+1}.json
   - Write integration_report.json
   - Write pass_4_audit.json

**Output:** `playbook-v{N+1}.json` + `integration_report.json` + `pass_4_audit.json`

### Pass 4 Audit Schema

```json
{
  "pass": 4,
  "pass_name": "playbook_generation",
  "status": "complete|partial|failed",
  "timing": {...},
  "input_summary": {
    "ready_for_integration": 41,
    "blocked": 6,
    "needs_human_resolution": 1
  },
  "changes_made": {
    "failure_modes": {"added": [...], "updated": [...], "total_before": 47, "total_after": 50},
    "workflow_patterns": {"added": [...], "total_before": 6, "total_after": 7},
    "decisions": {"added": [...], "total_before": 19, "total_after": 20},
    "compilation_blocks": {"updated": [...]},
    "phase_handoffs": {"updated": [...]},
    "checklists": {"added": [...]}
  },
  "indexes_rebuilt": {...},
  "gap_analysis": {...},
  "version_change": {"before": 1, "after": 2, "change_summary": "..."},
  "output_files": {...},
  "warnings": [],
  "errors": []
}
```

## Integration Report Schema

```json
{
  "status": "integrated|partial|blocked",
  "playbook_version": {"before": 1, "after": 2},
  "session_info": {
    "session_id": "abc123",
    "session_file": "session.xml",
    "timestamp": "ISO",
    "message_count": 350,
    "tool_uses": 89
  },
  "summary": {
    "extractions_processed": 47,
    "matched_to_phases": 41,
    "unmatched": 6,
    "contradictions_found": 2,
    "human_resolution_needed": 1,
    "items_added": {
      "failure_modes": 3,
      "workflow_patterns": 1,
      "decisions": 1,
      "checklist_items": 2,
      "compilation_updates": 3
    }
  },
  "integration_details": {
    "failure_modes_added": [...],
    "failure_modes_updated": [...],
    "checklist_items_added": [...],
    "compilation_blocks_updated": [...],
    "phase_handoffs_updated": [...],
    "cross_references_added": [...]
  },
  "session_evidence_citations": {
    "FM-048": "Session abc123, message 45-52 (crash + resolution)",
    "WP-007": "Session abc123, messages 120-145 (repeated 4x)",
    "CD-020": "Session abc123, message 78 (user approval)"
  },
  "gap_analysis": {
    "phases_with_thin_coverage": [...],
    "missing_validation_steps": [...],
    "questions_to_fill_holes": [...]
  },
  "contradictions": [...],
  "unmatched_items": [...],
  "warnings": [...],
  "audit_files": {
    "pass_1_audit": "pass_1_audit.json",
    "pass_2_audit": "pass_2_audit.json",
    "pass_3_audit": "pass_3_audit.json",
    "pass_4_audit": "pass_4_audit.json"
  },
  "blocked_reason": null
}
```

## Verify Mode

Checks playbook integrity without session data:

```json
{
  "playbook": "playbook.json",
  "verification": {
    "structural_integrity": {"valid": true, "missing_required_fields": [], "issues": []},
    "cross_reference_integrity": {"valid": true, "orphaned_references": [], "issues": []},
    "phase_consistency": {"valid": true, "issues": []},
    "id_uniqueness": {"valid": true, "duplicates": [], "issues": []},
    "compilation_blocks": {"valid": true, "missing_failure_mode_refs": [], "issues": []}
  },
  "summary": {"total_checks": 12, "passed": 12, "failed": 0}
}
```

## Status Mode

Shows playbook statistics:

```json
{
  "playbook": "playbook.json",
  "version": 1,
  "statistics": {
    "phases": 13,
    "total_checklist_items": 89,
    "total_failure_modes": 47,
    "total_workflow_patterns": 6,
    "total_decisions": 19,
    "total_cccs": 9,
    "kb_layers": 6,
    "compilation_blocks": 13
  },
  "coverage": {
    "by_phase": {
      "Phase 0": {"checklist_items": 7, "failure_modes": 2, "coverage": "high"},
      "Phase 4": {"checklist_items": 12, "failure_modes": 15, "coverage": "high"},
      "Phase 10": {"checklist_items": 2, "failure_modes": 0, "coverage": "low"}
    }
  },
  "last_modified": "ISO",
  "file_size_bytes": 45000
}
```

## Audit Mode

Runs all passes but stops before Pass 4 generation. Outputs detailed audit without modifying files.

## Error Handling

### Status Values

| Status | Meaning |
|--------|---------|
| `integrated` | All items processed, playbook updated successfully |
| `partial` | Some items processed, some blocked or unmatched |
| `blocked` | Major contradiction or error, requires human resolution |

### Error Codes

| Code | Category | Severity | Recovery |
|------|----------|----------|----------|
| E001 | parse_error | fatal | Stop, report line number |
| E002 | playbook_invalid | fatal | Reject playbook |
| E003 | phase_not_found | warning | Add to unmatched items |
| E004 | kb_not_found | warning | Continue without enrichment |
| E005 | contradiction | blocking | Add to blocked_reason |

### Error Audit Schema

```json
{
  "errors": [{
    "code": "E001",
    "category": "parse_error",
    "message": "Malformed cctrace XML at line 234",
    "severity": "fatal",
    "recovery": "Stop processing",
    "audit": {
      "pass": 1,
      "step": "parse_session_structure",
      "input_snippet": "...",
      "line_number": 234,
      "recovery_attempted": false,
      "recovery_result": null
    }
  }]
}
```

## Cache & Resume

```
.playbook-integrate-cache/
├── status.json
├── input_fingerprint.json
├── session_metadata.json
├── resume_state.json
├── pass_1/
│   ├── raw_extractions.json
│   ├── pass_1_audit.json
│   └── pass_1_error_audit.json
├── pass_2/
│   ├── correlated_extractions.json
│   ├── pass_2_audit.json
│   └── pass_2_error_audit.json
├── pass_3/
│   ├── validation_report.json
│   ├── pass_3_audit.json
│   └── pass_3_error_audit.json
└── pass_4/
    ├── pass_4_audit.json
    └── pass_4_error_audit.json
```

**Resume Behaviors:**

- `--resume`: Load status.json, validate fingerprints, continue from last completed pass
- `--clean`: Delete cache directory before starting
- `--audit-resume`: Show what would resume without resuming
- Default: Error if cache exists with fingerprint mismatch

## Output File Locations

```
<path/to/playbook>/
├── playbook.json                    # Original (unchanged)
├── playbook-v2.json                 # Updated version
├── integration_report.json          # Full report
├── .playbook-integrate-cache/       # Cache directory
│   └── ...
└── audits/
    └── <session_id>_<timestamp>/
        ├── full_audit_report.json
        ├── pass_1_audit.json
        ├── pass_2_audit.json
        ├── pass_3_audit.json
        └── pass_4_audit.json
```

## Contradiction Handling

Playbook is authoritative. Session contradictions go to warnings:

| Severity | Example | Action |
|----------|---------|--------|
| minor | Case variation ("REAPER" vs "Reaper") | Auto-resolve |
| major | Gate condition mismatch | Block, require human resolution |

## Gap Analysis Output

```json
{
  "gap_analysis": {
    "phases_with_thin_coverage": [
      {
        "phase": "Phase 1",
        "coverage_percent": 15,
        "checklist_items": 4,
        "failure_modes_referencing": 1,
        "missing": ["Performance baseline recording", "Existing code audit"]
      }
    ],
    "unmatched_extractions": [
      {
        "extraction_id": "F007",
        "type": "failure",
        "content": "Plugin crashed when loading preset",
        "suggested_action": "Manual review: could be Phase 5 or Phase 6"
      }
    ],
    "recommended_questions": [
      "What validation should exist for buffer size testing?",
      "How should licensing phase integrate with existing workflow?"
    ]
  }
}
```

## Skill Dependencies

- `kb-route` (optional): For knowledge directory queries
- File I/O: Read playbook JSON, write updated playbook
- XML parsing: For cctrace format

## Implementation Notes

1. The skill should be implemented as a standalone skill file in `~/.claude/skills/playbook-integrate/SKILL.md`
2. Each pass should be a distinct function for testability
3. Audit logs are written atomically (write to temp, then rename)
4. Resume state should track exact position for interrupted runs
5. Knowledge directory loading should be lazy (load on first reference)