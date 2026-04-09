---
name: playbook-integrate
description: "Integrate session execution data into PBCPB playbooks. Extracts failures, patterns, and decisions from cctrace XML session data and produces versioned playbook updates with full cross-reference integrity."
---

# playbook-integrate — Playbook Integration Skill

Integrates session execution data into PBCPB playbooks. Extracts failures, patterns, decisions, and workflow improvements from cctrace session data and produces versioned playbook updates with full cross-reference integrity and comprehensive audit trails.

**Spec:** `docs/superpowers/specs/2026-04-08-playbook-integrate-skill-design.md`

## Invocation

```
playbook-integrate --playbook <path> --session <path>                    # Primary invocation
playbook-integrate --playbook <path> --session-dir <dir>                 # Multiple sessions in directory
playbook-integrate --playbook <path> --session-json <json>               # Inline JSON session data
playbook-integrate --playbook <path> --knowledge-dir <path>              # Load domain knowledge
playbook-integrate --playbook <path> --session <path> --knowledge-dir <path>  # Combined
playbook-integrate --playbook <path> --session <path> --audit            # Audit single session
playbook-integrate --playbook <path> --session-dir <dir> --audit         # Audit multiple sessions
playbook-integrate --verify-only                                         # Check playbook integrity
playbook-integrate --status                                              # Show playbook statistics
playbook-integrate --resume                                             # Continue from last pass
playbook-integrate --clean                                              # Remove cache before starting
playbook-integrate --audit-resume                                       # Show resume state without continuing
```

## Defaults

| Flag | Default |
|------|---------|
| `--playbook` | REQUIRED — no default |
| `--session` | REQUIRED if no `--session-dir` or `--session-json` |
| `--output-dir` | Same directory as playbook |
| `--version-increment` | `minor` (X.Y → X.Y+1) |
| `--knowledge-dir` | None (use playbook's knowledge_base section) |
| `--audit` | `false` (when absent, apply changes) |

## Mode Routing (Priority Order)

Flags are evaluated in priority order — first match wins:

| Priority | Flags Present | Mode | Jump To |
|----------|---------------|------|---------|
| 1 | `--verify-only` | Verify playbook integrity | Verify Mode |
| 2 | `--status` | Show statistics | Status Mode |
| 3 | `--audit-resume` | Show resume state | Audit Resume Mode |
| 4 | `--session-dir <dir>` [+ `--audit`] | Multi-session (with optional audit) | Batch Mode |
| 5 | `--session-json <json>` [+ `--audit`] | Inline JSON (with optional audit) | Single Mode |
| 6 | `--session <path>` [+ `--audit`] | Single file (with optional audit) | Single Mode |

**Flag combinations:**
- `--audit` can be combined with any session input flag (`--session`, `--session-dir`, `--session-json`)
- `--knowledge-dir` can be combined with any session input flag
- `--resume` and `--clean` are mutually exclusive — using both is an error

## Input Formats

The skill accepts multiple session data formats.

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

### Input Parsing Procedure

**Step 1: Detect format**
- If `--session <path>`: check file extension (.xml → cctrace, .json → JSON, .jsonl → JSONL)
- If `--session-dir <dir>`: scan for .xml, .json, .jsonl files
- If `--session-json <json>`: parse inline JSON

**Step 2: Parse XML (cctrace format)**
```
1. Read file with Read tool
2. Extract metadata: session-id, start-time, end-time, statistics
3. Parse messages in order:
   - Extract event-type from <event-type>
   - Extract text content from <text>
   - Extract thinking content from <thinking> (if present)
   - Extract tool-use: name attribute and content
4. Build message index: message_index → {uuid, timestamp, type, content}
5. Track file mentions: scan for file paths in content
```

**Step 3: Parse JSON (structured format)**
```
1. Read and parse JSON
2. Validate required fields: session_id, messages array
3. Normalize to cctrace-equivalent structure
4. Build message index same as XML
```

**Step 4: Parse JSONL (line-delimited format)**
```
1. Read file line by line
2. Parse each line as JSON object
3. Accumulate messages array
4. Normalize to cctrace-equivalent structure
```

**Step 5: Validate parsed session**
```
1. Check session_id exists
2. Check at least one message
3. Check timestamps are valid ISO format
4. Build extraction-ready session object
5. Write to cache: session_metadata.json
```

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

### 1. Parse Session Structure

```
1. Load cctrace manifest from parsed session
2. Parse messages in order
3. Extract: user messages, assistant responses, tool calls
4. Track file history: scan for file paths in content
5. AUDIT: Log parsing errors, malformed XML, skipped content
```

**Audit fields:**
- `xml_valid`: true/false
- `messages_parsed`: count
- `messages_skipped`: count with reasons
- `files_touched`: array of file paths

### 2. Extract Failure Patterns

Scan for:

| Pattern Type | Detection Heuristic |
|--------------|---------------------|
| `error_message` | Tool result with `<error>` or `<failure>` tag |
| `user_correction` | User message containing "no", "wrong", "incorrect", "try again", "that's not" |
| `crash` | Messages containing "crashed", "segfault", "assertion failed", "SIGABRT" |
| `build_error` | Tool result from build command with non-zero exit code |

For each failure:
1. Extract message_index and timestamp
2. Capture content (error message or correction text)
3. Get context_before (preceding 2 messages)
4. Get context_after (following 2 messages)
5. Look for resolution in subsequent messages

**Resolution detection:**
- Fixed: subsequent message shows successful completion
- Workaround: subsequent message shows alternative approach
- Unresolved: no resolution within session

**AUDIT:** Log extraction confidence (0.0-1.0), ambiguous matches

### 3. Extract Decision Points

Scan for:

| Decision Type | Detection Heuristic |
|---------------|---------------------|
| `approval` | User message containing "yes", "proceed", "approved", "ok", "go ahead" |
| `rejection` | User message containing "no", "don't", "rejected", "cancel" |
| `clarification` | User message containing "?", "clarify", "explain", "what do you mean" |
| `creative_choice` | User selecting from multiple options, or explicit creative direction |

For each decision:
1. Extract message_index and timestamp
2. Capture user_input (exact user message)
3. Look back for ai_proposal (preceding assistant message)
4. Mark `human_only: true` if decision matches human_only task in playbook

**AUDIT:** Log heuristic matching decisions, confidence scores

### 4. Extract Workflow Patterns

Scan for repeated sequences:

| Pattern Type | Detection Heuristic |
|--------------|---------------------|
| `sequence` | Edit → Build → Test → Fix cycle appearing 2+ times |
| `repetition` | Same tool called 3+ times in succession |
| `handoff` | Session start with context restatement |

For each pattern:
1. Identify message_range [start_index, end_index]
2. Extract pattern description
3. Count occurrences within session

**AUDIT:** Log pattern detection thresholds, repetition counts

### 5. Extract Knowledge References

Scan for:

| Reference Type | Detection Heuristic |
|-----------------|---------------------|
| `api_mention` | Framework class names (e.g., "SmoothedValue", "AudioProcessor") |
| `concept_mention` | Domain terms from vocabulary |
| `kb_reference` | Explicit KB lookups in tool calls |

For each reference:
1. Extract content (exact mention)
2. Get surrounding context
3. Match against vocabulary from knowledge directory (if provided)

**AUDIT:** Log vocabulary matches, synonym expansions

### 6. Build Session Index

Construct three indexes:

```json
{
  "by_message_id": {
    "uuid-1": { "timestamp": "...", "type": "user", "content": "..." }
  },
  "by_timestamp": {
    "2026-04-08T10:00:00Z": "uuid-1"
  },
  "by_file": {
    "src/Module.h": [45, 67, 89]
  }
}
```

**AUDIT:** Log index completeness, orphan references

### Output

Write to cache directory: `.playbook-integrate-cache/pass_1/raw_extractions.json`

Write audit: `.playbook-integrate-cache/pass_1/pass_1_audit.json`

## Pass 2: Phase Correlation

**Input:** raw_extractions.json, original playbook, optional knowledge directory

**Processing:**

### 1. Load Playbook Phases

```
1. Read playbook JSON with Read tool
2. Parse phase definitions from phases array
3. Extract per-phase: title, gates, checklists, compilation blocks
4. Build phase index: phase_title → {gates, checklists, keywords}
5. Extract phase keywords from:
   - Phase title
   - Compilation blocks (objective, context_load)
   - Checklist items
   - Gate conditions
6. AUDIT: Log phase count, gate conditions per phase
```

**Phase index structure:**
```json
{
  "Phase 0": {
    "title": "Specification",
    "gates": ["SPEC.md exists", "Sound identity documented"],
    "checklists": ["[Stakeholder] — Define sound identity words"],
    "keywords": ["specification", "sound identity", "creative direction"],
    "human_only_tasks": ["[Stakeholder] — Define sound identity words"]
  }
}
```

### 2. Match Failures to Phases

For each failure in raw_extractions.failures:

```
1. Extract task context keywords from content and context_before/context_after
2. Match against phase keywords (fuzzy match, case-insensitive)
3. Extract file context from files mentioned in context
4. Match files against typical phase file patterns:
   - Phase 0: *.md (specs)
   - Phase 2: CMakeLists.txt, build files
   - Phase 4: src/*.cpp, src/*.h (DSP implementation)
   - Phase 5: preset code
   - Phase 7: GUI files
   - Phase 8: test files
5. Calculate phase_match_confidence:
   - task_context match: +0.4
   - file_context match: +0.3
   - timestamp match (if session has phase markers): +0.3
6. Select phase with highest confidence ≥ 0.5
7. If confidence < 0.5, mark as unmatched
8. AUDIT: Log matching heuristics, confidence scores
```

### 3. Match Decisions to Phases

For each decision in raw_extractions.decisions:

```
1. Match approvals/rejections to nearest gate condition
2. Match creative decisions to human_only tasks
3. Look for gate_condition in surrounding context
4. Calculate confidence based on context proximity
5. Mark human_only flag from playbook task
6. AUDIT: Log decision-to-task mappings
```

### 4. Match Patterns to Phases

For each workflow pattern in raw_extractions.workflow_patterns:

```
1. Map message_range to phase transitions
2. Identify phase sequence from message timestamps
3. Count pattern occurrences per phase
4. AUDIT: Log pattern-to-phase mappings
```

### 5. Detect Phase Boundaries

Scan for phase transitions:

```
1. Session start → identify initial phase from first message context
2. Gate condition completions → look for success/complete markers
3. Explicit phase mentions → look for "starting phase X", "now moving to"
4. Context restatements → often indicate phase boundaries
5. AUDIT: Log detected vs expected boundaries
```

### 6. Identify Unmatched Items

For each extraction with no phase match:

```
1. Collect into unmatched_items array
2. Categorize by reason:
   - no_phase_match: No keywords or files suggest any phase
   - ambiguous: Multiple phases with similar confidence
   - novel: New topic not in playbook
3. Add suggested_phases for ambiguous items
4. Add to gap analysis
```

### Output

Write to cache: `.playbook-integrate-cache/pass_2/correlated_extractions.json`

Write audit: `.playbook-integrate-cache/pass_2/pass_2_audit.json`

## Pass 3: Integrity Validation

**Input:** correlated_extractions.json, original playbook, optional knowledge directory

**Processing:**

### 1. Validate Cross-References

For each proposed cross-reference:

```
1. Verify target phase exists in playbook
   - Check phase number is within valid range
   - Check phase title matches
2. Verify target failure mode exists
   - Check FM-XXX ID exists in playbook
   - Check FM-XXX phase matches source phase
3. Verify KB layer exists
   - Check layer name in playbook knowledge_base section
   - Check topic exists within layer
4. Check bidirectional references
   - If A references B, B should reference A
5. AUDIT: Log all reference validations, pass/fail status
```

**Invalid reference handling:**
- Phase not found → mark as blocked, suggest closest phase
- Failure mode not found → mark as blocked, check for ID range gap
- KB layer not found → mark as warning, continue

### 2. Validate ID Uniqueness

```
1. Read existing playbook IDs:
   - failure_modes[].id (FM-XXX pattern)
   - workflow_patterns[].id (WP-XXX pattern)
   - cross_cutting_concerns[].id (CCC-XXX pattern)
   - decisions[].id (CD-XXX pattern)
2. Generate proposed new IDs:
   - Find max FM-XXX, increment for new failures
   - Find max WP-XXX, increment for new patterns
   - Find max CD-XXX, increment for new decisions
3. Check for conflicts with proposed IDs
4. Resolve conflicts by incrementing
5. AUDIT: Log ID assignments, conflicts resolved
```

### 3. Detect Contradictions

Compare session content vs playbook content:

```
1. For each matched failure:
   - Check if root cause matches existing FM
   - Check if prevention rule conflicts with checklist
2. For each matched decision:
   - Check if user choice contradicts playbook gate
3. For each workflow pattern:
   - Check if pattern conflicts with expected flow
4. Categorize by severity:
   - minor: Case variation, wording difference
   - major: Logic conflict, gate condition mismatch
5. AUDIT: Log all contradictions with severity
```

**Contradiction resolution:**
- Playbook is authoritative
- Session contradictions go to warnings
- Major contradictions → blocked, require human resolution

### 4. Verify KB Layer References

```
1. For each knowledge reference:
   - Check referenced layer exists
   - Check topic exists within layer
   - If knowledge directory provided, validate entry exists
2. Mark references for enrichment
3. AUDIT: Log KB reference validations
```

### 5. Verify Structural Integrity

```
1. Check all phases have required fields:
   - title, gates, checklists, compilation
2. Check all checklists have required fields:
   - role, task
3. Check all failure modes have required fields:
   - id, symptom, root_cause, fix, prevention, phase
4. Check all CCCs have required fields:
   - id, title, applies_to
5. AUDIT: Log structural validation results
```

### 6. Build Integration Plan

```
1. Collect ready_for_integration:
   - Items with valid phase match
   - Items with no contradictions
   - Items with unique IDs
2. Collect blocked items:
   - No phase match
   - Invalid cross-references
   - Minor contradictions (can be auto-resolved)
3. Collect needs_human_resolution:
   - Major contradictions
   - Ambiguous phase matches
4. AUDIT: Log integration plan summary
```

### Output

Write to cache: `.playbook-integrate-cache/pass_3/validation_report.json`

Write audit: `.playbook-integrate-cache/pass_3/pass_3_audit.json`

## Pass 4: Playbook Generation

**Input:** validation_report.json, original playbook

**Processing:**

### 1. Load Original Playbook

```
1. Read playbook JSON with Read tool
2. Deep copy for modification
3. Track all changes for diff
4. AUDIT: Log playbook version, section counts
```

### 2. Merge Failure Modes

For each item in validation_report.integration_plan.ready_for_integration where type == "failure_mode":

```
1. Assign final ID from validation_report.id_assignments
2. Build failure mode object:
   {
     "id": "FM-XXX",
     "symptom": "...",
     "root_cause": "...",
     "fix": "...",
     "prevention": "...",
     "phase": "Phase N",
     "severity": "critical|major|minor",
     "source": "session_X_line_Y"
   }
3. Add to playbook.failure_modes array
4. Add prevention rule to phase.checklist:
   - Format: "[Prevention] — <prevention text> (FM-XXX)"
5. Create cross-reference from failure mode to phase
6. AUDIT: Log each addition, location
```

### 3. Merge Workflow Patterns

For each item where type == "workflow_pattern":

```
1. Check if workflow_patterns section exists in playbook
   - If not, create: "workflow_patterns": []
2. Check for existing pattern with same signature
   - If exists, increment occurrences, update last_seen
   - If new, add entry:
     {
       "id": "WP-XXX",
       "pattern": "...",
       "phase_sequence": [...],
       "occurrences": N,
       "source_sessions": [...],
       "first_seen": "ISO",
       "last_seen": "ISO"
     }
3. AUDIT: Log pattern additions
```

### 4. Merge Decisions

For each item where type == "decision" and human_only == true:

```
1. Check if decisions array exists in playbook
   - If not, create: "decisions": []
2. Add decision ledger entry:
   {
     "id": "CD-XXX",
     "decision": "...",
     "made_by": "human",
     "phase": "Phase N",
     "context": "...",
     "source_session": "..."
   }
3. Update phase gate with decision evidence:
   - Add note to gate condition: "Decision: <description>"
4. AUDIT: Log decision additions
```

### 5. Update Compilation Blocks

For each phase with new content:

```
1. Add failure mode to compilation.failure_modes_relevant
2. Update context_load with new artifacts from session_boundaries
3. Update success_criteria if new gates validated
4. AUDIT: Log compilation block updates
```

### 6. Update Phase Handoffs

For each session_boundary:

```
1. Find from_phase and to_phase in playbook
2. Update phase_handoff for from_phase:
   - Update carry_forward: list artifacts mentioned in content_restated
   - Update drop: list artifacts in content_dropped
3. AUDIT: Log phase handoff updates
```

### 7. Rebuild Indexes

```
1. Rebuild failure_mode → phase index:
   - Scan all failure_modes
   - Build map: FM-XXX → "Phase N"
2. Rebuild CCC → phase index:
   - Scan all cross_cutting_concerns
   - Build map: CCC-XXX → ["Phase A", "Phase B"]
3. Rebuild KB layer → topic index:
   - Scan knowledge_base.layers
   - Build map: layer_name → [topics]
4. AUDIT: Log index rebuild statistics
```

### 8. Generate Gap Analysis

```
1. Identify thin-coverage phases:
   - Calculate coverage = (checklist_items / total_failures_referencing)
   - Flag phases with coverage < 20%
2. List unmatched extractions with suggested actions
3. Generate recommended questions:
   - For each thin-coverage phase: "What validation should exist for <topic>?"
   - For each unmatched item: "Where should <content> be integrated?"
4. AUDIT: Log gap analysis items
```

### 9. Increment Version and Write Output

```
1. Increment playbook version number
2. Write playbook-v{N+1}.json to output directory
3. Write integration_report.json
4. Write pass_4_audit.json
5. Write consolidated audit to audits/<session_id>_<timestamp>/full_audit_report.json
```

### Output Files

| File | Location | Purpose |
|------|----------|---------|
| playbook-v{N+1}.json | Same dir as original | Updated playbook |
| integration_report.json | Same dir as original | Summary report |
| full_audit_report.json | audits/<session_id>_<timestamp>/ | Consolidated audit |

## Audit Mode

Runs all passes but stops before Pass 4 (generation). Outputs a detailed audit report without modifying any files.

```
playbook-integrate --playbook playbook.json --session session.xml --audit
```

### Behavior

| Step | Action |
|------|--------|
| Pass 1 | Run fully, write `raw_extractions.json` and `pass_1_audit.json` to cache |
| Pass 2 | Run fully, write `correlated_extractions.json` and `pass_2_audit.json` to cache |
| Pass 3 | Run fully, write `validation_report.json` and `pass_3_audit.json` to cache |
| Pass 4 | Skip entirely — no playbook generation, no file modifications |
| Output | Write `audit_report.json` to audits directory |

### Output

```json
{
  "audit_mode": true,
  "playbook": "playbook.json",
  "session": "session.xml",
  "status": "audit_complete",
  "passes_completed": [1, 2, 3],
  "pass_4_skipped": true,
  "would_produce": {
    "failure_modes_added": 3,
    "workflow_patterns_added": 1,
    "decisions_added": 1,
    "checklist_items_added": 2,
    "compilation_updates": 3,
    "version_change": {"before": 1, "after": 2}
  },
  "issues_found": {
    "unmatched_items": 6,
    "contradictions": 2,
    "human_resolution_needed": 1
  },
  "audit_files": {
    "pass_1_audit": "pass_1_audit.json",
    "pass_2_audit": "pass_2_audit.json",
    "pass_3_audit": "pass_3_audit.json"
  }
}
```

## Batch Mode

Processes multiple sessions from a directory. Each session is processed independently, then results are merged.

```
playbook-integrate --playbook playbook.json --session-dir ./sessions/
playbook-integrate --playbook playbook.json --session-dir ./sessions/ --audit
```

### Behavior

| Step | Action |
|------|--------|
| Discovery | Scan directory for `.xml`, `.json`, `.jsonl` files |
| Per-session | Run Passes 1-3 for each session, accumulate extractions |
| Merge | Combine extractions from all sessions, deduplicate |
| Single Pass 4 | Run Pass 4 once on merged extractions |
| Output | Single playbook update, multi-session integration report |

### Session Conflict Resolution

When the same failure/pattern is found in multiple sessions:

1. Use earliest timestamp as `source`
2. Merge evidence from all sessions
3. Keep all session citations in `session_evidence_citations`

### Output

```json
{
  "status": "integrated|partial|blocked",
  "sessions_processed": 5,
  "sessions": [
    {"session_id": "abc123", "file": "session1.xml", "status": "processed", "extractions": 47},
    {"session_id": "def456", "file": "session2.xml", "status": "processed", "extractions": 32}
  ],
  "merged_extractions": {
    "failures_unique": 45,
    "failures_duplicates_merged": 12,
    "decisions_unique": 23,
    "patterns_unique": 8
  },
  "playbook_version": {"before": 1, "after": 2}
}
```

## Verify Mode

Checks playbook integrity without session data:

```
playbook-integrate --verify-only --playbook playbook.json
```

### Checks

1. **Structural integrity:** All required sections present (phases, failure_modes, cross_cutting_concerns, knowledge_base)
2. **Cross-reference integrity:** All references point to valid entries
3. **Phase consistency:** All phase references valid, no orphans
4. **ID uniqueness:** No duplicate IDs in any collection

### Output

```json
{
  "playbook": "playbook.json",
  "verification": {
    "structural_integrity": {"valid": true, "missing_required_fields": [], "issues": []},
    "cross_reference_integrity": {"valid": true, "orphaned_references": [], "issues": []},
    "phase_consistency": {"valid": true, "issues": []},
    "id_uniqueness": {"valid": true, "duplicates": [], "issues": []}
  },
  "summary": {"total_checks": 12, "passed": 12, "failed": 0}
}
```

## Status Mode

Shows playbook statistics:

```
playbook-integrate --status --playbook playbook.json
```

### Output

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

## Cache & Resume

### Cache Directory Structure

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

### Resume Behaviors

| Flag | Behavior |
|------|----------|
| `--resume` | Load status.json, validate fingerprints, continue from last completed pass |
| `--clean` | Delete cache directory before starting |
| `--audit-resume` | Show what would resume without resuming |
| Default | Error if cache exists with fingerprint mismatch |

### Resume Procedure

```
1. Check for .playbook-integrate-cache/status.json
2. If --clean flag: delete entire cache directory, start fresh
3. If --resume flag:
   a. Read status.json
   b. Validate input_fingerprint.json matches current inputs
   c. If fingerprint mismatch: error with message
   d. Load resume_state.json
   e. Continue from last_completed_pass + 1
   f. Load intermediate files from previous pass
4. If --audit-resume flag:
   a. Read resume_state.json
   b. Output resume status without continuing
   c. Exit without processing
5. If cache exists without --resume:
   a. Error: "Cache exists. Use --resume to continue or --clean to start fresh"
```

### Fingerprint Validation

```json
{
  "playbook_hash": "sha256:<hash>",
  "session_hash": "sha256:<hash>",
  "knowledge_dir_hash": "sha256:<hash> or null"
}
```

If any hash differs from current input, the cache is stale and must be cleaned.

### Atomic Writes

All output files use atomic write:
1. Write to temp file (`.tmp` extension)
2. Rename temp to final name
3. This prevents partial writes on interruption

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

Every error is logged in `pass_N_error_audit.json`:

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

### Recovery Procedures

| Error | Recovery |
|-------|----------|
| E001 - parse_error | Stop. Report file, line number, snippet. Suggest fix. |
| E002 - playbook_invalid | Stop. Report which section is missing/malformed. |
| E003 - phase_not_found | Continue. Add to unmatched_items. Suggest closest phase. |
| E004 - kb_not_found | Continue. Log warning. Skip KB enrichment. |
| E005 - contradiction | Continue in audit mode. Add to blocked_reason. |

### Partial Processing

When errors occur, continue processing what's possible:

1. Log each error in appropriate pass's error audit
2. Mark affected items as blocked
3. Continue with unaffected items
4. Generate final report with blocked items
5. If `--audit` mode: complete all passes that can run
6. If normal mode: stop at Pass 4 if any blocking errors

## Contradiction Handling

Playbook is authoritative. Session contradictions go to warnings.

### Severity Levels

| Severity | Example | Action |
|----------|---------|--------|
| minor | Case variation ("REAPER" vs "Reaper") | Auto-resolve (use playbook version) |
| major | Gate condition mismatch | Block, require human resolution |

### Minor Contradictions

Auto-resolved by preferring playbook:

1. Case differences in strings
2. Minor wording variations
3. Order differences in lists
4. Documentation formatting

Log in warnings:
```json
{"warning": "W001", "type": "minor_contradiction", "field": "phase_title", "playbook_value": "DSP Implementation", "session_value": "dsp implementation", "resolution": "used_playbook"}
```

### Major Contradictions

Block and require human resolution:

1. Gate condition conflicts
2. Phase ordering conflicts
3. Required field mismatches
4. Human-only decision conflicts

Log in blocked_reason:
```json
{"error": "E005", "type": "major_contradiction", "field": "gate_condition", "playbook_value": "All DSP modules implemented", "session_evidence": "Phase 5 started before filter implementation complete", "resolution": "blocked", "action_needed": "Human must resolve gate condition conflict"}
```

## Gap Analysis Output

Generated during Pass 4, included in integration report.

### Thin Coverage Detection

A phase is considered "thin coverage" if:
1. Few checklist items (< 3) AND many failure modes referencing it (> 5)
2. OR coverage percentage < 20%

Coverage calculation:
```
coverage = (checklist_items / max(1, failure_modes_referencing)) * 100
```

### Gap Analysis Structure

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

## Output File Locations

```
<path/to/playbook>/
├── playbook.json                    # Original (unchanged)
├── playbook-v2.json                 # Updated version
├── integration_report.json          # Full report
├── .playbook-integrate-cache/       # Cache directory
│   ├── status.json
│   ├── input_fingerprint.json
│   ├── session_metadata.json
│   ├── resume_state.json
│   ├── pass_1/
│   │   ├── raw_extractions.json
│   │   └── pass_1_audit.json
│   ├── pass_2/
│   │   ├── correlated_extractions.json
│   │   └── pass_2_audit.json
│   ├── pass_3/
│   │   ├── validation_report.json
│   │   └── pass_3_audit.json
│   └── pass_4/
│       └── pass_4_audit.json
└── audits/
    └── <session_id>_<timestamp>/
        ├── full_audit_report.json
        ├── pass_1_audit.json
        ├── pass_2_audit.json
        ├── pass_3_audit.json
        └── pass_4_audit.json
```

## Workflow Patterns Storage Location

Workflow patterns extracted from sessions are stored in the playbook under:

```json
{
  "workflow_patterns": [
    {
      "id": "WP-007",
      "pattern": "Edit → Build → Test → Fix",
      "phase_sequence": ["Phase 4", "Phase 4", "Phase 8", "Phase 4"],
      "occurrences": 4,
      "source_sessions": ["abc123", "def456"],
      "first_seen": "ISO timestamp",
      "last_seen": "ISO timestamp"
    }
  ]
}
```

If `workflow_patterns` does not exist in the playbook, it is created as a new top-level array.

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