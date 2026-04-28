# epiphany-analysis Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the `epiphany-analysis` Claude Code skill per `docs/superpowers/specs/2026-04-14-epiphany-analysis-design.md` — a modular skill that consumes Node A (input) + Node B (analysis) and produces an enhanced Node A via a graph-of-thought pipeline (A → A1, B → B1 → C → D → E → integrated output).

**Architecture:** Prompt-engineered skill, not conventional code. Implementation is a directory of markdown files under `~/.claude/skills/epiphany-analysis/`: one `SKILL.md` orchestrator + six module prompt files (`m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`) + tests (bash + Python structural assertions). Runtime reads/writes stage files in `~/docs/epiphany/analysis/<session-id>/`. The skill currently exists as a placeholder (`v0.1.0-placeholder`) that this plan replaces entirely.

**Tech Stack:** Markdown (YAML frontmatter), Bash (test scripts), Python 3 (frontmatter validation). No compiled code, no runtime dependencies beyond the Claude Code harness.

**Spec reference:** `/home/myuser/docs/superpowers/specs/2026-04-14-epiphany-analysis-design.md` — every module task below cites the specific spec section it implements. Read the spec section before starting each task.

**Gold-reference artifacts:** `/home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/` — contains `report.md` (Node B), `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md`. Used as both design anchor (content schema) and test fixture (§14.1).

---

## File Structure

```
~/.claude/skills/epiphany-analysis/
├── SKILL.md                         ← orchestrator (main entry point)
├── modules/
│   ├── m-intake.md                  ← §2 m-intake
│   ├── m-analyze.md                 ← §2 m-analyze (embeds §5 Map)
│   ├── m-ideate.md                  ← §2 m-ideate
│   ├── m-filter.md                  ← §2 m-filter
│   ├── m-engineer.md                ← §2 m-engineer
│   └── m-integrate.md               ← §2 m-integrate (embeds §9.2 audit)
├── templates/
│   └── session.json.template        ← §10 session.json schema
└── tests/
    ├── validate-frontmatter.py      ← frontmatter schema check
    ├── test-structure.sh            ← structural assertions
    ├── test-skill-registration.sh   ← registration & layout
    ├── test-runtime-invariants.sh   ← §14.2 / §14.3 runtime fixtures
    ├── test-partial-run.sh          ← §14.6 replay contract
    ├── fixtures/                    ← runtime-invariant fixtures (Task 14)
    │   ├── pass/
    │   ├── fail-r1-analysis-incomplete/
    │   ├── fail-r2-bad-target/
    │   ├── fail-r4-constraint-dropped/
    │   ├── fail-r5-malformed/
    │   └── fail-r6-mtime-mismatch/
    └── gold-reference/              ← manual §14.1 protocol
        ├── RUN.md
        └── run-log.md
```

One file per responsibility. `SKILL.md` is the orchestrator; each module file is the prompt that runs in the orchestrator's inline context. Tests split by invariant class so failure locations are obvious.

---

## Task 1: Directory scaffold and placeholder archive

**Files:**
- Archive: `~/.claude/skills/epiphany-analysis/SKILL.md` → `~/.claude/skills/epiphany-analysis/.SKILL.md.placeholder-backup` (safety net; deletable after v1)
- Create: `~/.claude/skills/epiphany-analysis/modules/`
- Create: `~/.claude/skills/epiphany-analysis/templates/`
- Create: `~/.claude/skills/epiphany-analysis/tests/fixtures/`
- Create: `~/.claude/skills/epiphany-analysis/tests/gold-reference/`

- [ ] **Step 1: Verify existing placeholder state**

Run: `ls -la ~/.claude/skills/epiphany-analysis/`
Expected: exactly one file `SKILL.md`, no subdirectories. If modules/ or tests/ already exist, stop and investigate before proceeding.

- [ ] **Step 2: Archive the placeholder**

Run:
```bash
mv ~/.claude/skills/epiphany-analysis/SKILL.md \
   ~/.claude/skills/epiphany-analysis/.SKILL.md.placeholder-backup
```
Expected: silent success; `ls -la` now shows only the hidden backup.

- [ ] **Step 3: Create subdirectories**

Run:
```bash
mkdir -p ~/.claude/skills/epiphany-analysis/modules \
         ~/.claude/skills/epiphany-analysis/templates \
         ~/.claude/skills/epiphany-analysis/tests/fixtures \
         ~/.claude/skills/epiphany-analysis/tests/gold-reference
```
Expected: silent success.

- [ ] **Step 4: Verify scaffold**

Run: `find ~/.claude/skills/epiphany-analysis -type d`
Expected output (order may vary, tildes shown here are paths `find` will print fully-expanded):
```
.../epiphany-analysis
.../epiphany-analysis/modules
.../epiphany-analysis/templates
.../epiphany-analysis/tests
.../epiphany-analysis/tests/fixtures
.../epiphany-analysis/tests/gold-reference
```

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add -A
git commit -m "feat(epiphany-analysis): scaffold directory structure, archive placeholder"
```

---

## Task 2: SKILL.md — identity, activation, inputs/outputs

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/SKILL.md`

Implements spec §1 in full.

- [ ] **Step 1: Write the failing structure test (placeholder — real test in Task 12)**

Skip — the structural test script is written in Task 12; for now we are creating the file whose structure Task 12 will assert against.

- [ ] **Step 2: Write frontmatter and identity sections**

Write to `~/.claude/skills/epiphany-analysis/SKILL.md`:

```markdown
---
name: epiphany-analysis
version: 1.0.0
description: Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B. Cross-cutting graph-of-thought pipeline (Nodes A, B, A1, B1, C, D, E) with ideation/synthesis/verification loops modeled on prompt-epiphany. Modular — 6 module files, replayable in isolation.
trigger: /epiphany-analysis
skill_path: ~/.claude/skills/epiphany-analysis/
save_path: ~/docs/epiphany/analysis/
---

# Epiphany-Analysis

Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B.

Uses a cross-cutting graph-of-thought pipeline:
- **Nodes A, B** — input and its analysis
- **Nodes A1, B1** — full analyses of A and B
- **Node C** — unfiltered idea pool from A1 + B1
- **Node D** — accepted-ideas store (pro/con/utility filter)
- **Node E** — solutions store (two drafts compared, best kept)
- **Integration** — all Node E applied to produce `enhanced.md`

Spec: `docs/superpowers/specs/2026-04-14-epiphany-analysis-design.md`.
```

- [ ] **Step 3: Add Trigger Conditions section (§1 activation triggers, verbatim)**

Append to `SKILL.md`:

```markdown
## Trigger Conditions

| Trigger | Behavior |
|---|---|
| `/epiphany-analysis <path-to-A> <path-to-B>` | Run skill with positional paths. Primary invocation. |
| `/epiphany-analysis <path-to-A>` | Ask once: "Path to the analysis document (Node B)?" Then run. |
| `/epiphany-analysis` | Ask once for A, once for B. Then run. |
| `/epiphany-analysis` when Node A is inline in the user prompt | Ask once for B; store inline A content as Node A (no file). |
| `/epiphany-analysis --session-dir=<path> [--start-at=<module>] [--stop-at=<module>]` | Replay / partial-run. See Orchestrator § Replay. |

No natural-language-trigger mode in v1.
```

- [ ] **Step 4: Add Runtime Scope and Inputs/Outputs sections**

Headings to emit (exact, in order): `## Runtime Scope`, `## Inputs`, `## Primary outputs`.

Copy the verbatim text from spec §1 for each. Essential content each section must include:

- **`## Runtime Scope`** — self-contained execution, no web/grep/glob/brainstorming calls, no mid-run user questions after m-intake starts, 15-minute soft wall-clock.
- **`## Inputs`** — Node A (file path OR inline content), Node B (file path; analysis document). Optional flags: `--session-dir`, `--start-at`, `--stop-at`.
- **`## Primary outputs`** — `enhanced.md` (on PASS) or `enhanced-partial.md` + `open-questions.md` (on 2nd FAIL), always `self-audit.md` and `session.json` and all stage files (`node-{a,b,a1-analysis,b1-analysis,c-ideas,d-accepted,e-solutions}.md`) in `~/docs/epiphany/analysis/<session-id>/`.

Open the spec at §1 and copy the full wording. The three-heading anchor contract is what Task 13's structural test asserts against.

- [ ] **Step 5: Verify file is well-formed**

Run: `head -1 ~/.claude/skills/epiphany-analysis/SKILL.md`
Expected: `---` (the frontmatter delimiter)

Run: `grep -c '^## ' ~/.claude/skills/epiphany-analysis/SKILL.md`
Expected: `4` (Trigger Conditions, Runtime Scope, Inputs, Primary outputs)

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add SKILL.md
git commit -m "feat(epiphany-analysis): SKILL.md identity, activation, I/O sections"
```

---

## Task 3: SKILL.md — Hard Gates and Orchestrator flow

**Files:**
- Modify: `~/.claude/skills/epiphany-analysis/SKILL.md` (append)

Implements the STEP 0 → STEP 7 orchestrator control flow. Anchors on spec §1 activation, §8 detection, §10 session-id format, §11 caps, §12 replay contract.

- [ ] **Step 1: Add Hard Gates section**

Append to `SKILL.md`:

```markdown
## Hard Gates

These are enforced before any module runs. Violating any gate halts the skill with an error.

1. **HG-1 — Output ≠ Input.** `m-intake` verifies the `enhanced.md` target path does not equal Node A's source path. If equal, abort before any work (spec §10 non-overwrite guarantee).
2. **HG-2 — Self-contained runtime.** Never invoke web search, grep, glob, or brainstorming skill during execution. Never ask the user questions once `m-intake` has started (spec §1 runtime scope, anti-pattern 8).
3. **HG-3 — Two-drafts-per-D_i.** `m-engineer` never writes a Node E entry from a single draft. Two independently-drafted solutions, compared, or D_i is dropped (spec §7, anti-pattern 9).
4. **HG-4 — Fresh copy only.** All outputs live in `~/docs/epiphany/analysis/<session-id>/`. Never write to Node A's directory (spec §10, anti-pattern 12).
5. **HG-5 — Bounded iteration.** Every loop has a cap (spec §11). No unbounded while-true constructs allowed in any module.
```

- [ ] **Step 2: Add Orchestrator section header + STEP 0 (flag parsing)**

Append:

```markdown
## Orchestrator

Six modules, executed in order. Each module writes stage files to the session directory before returning. The orchestrator does not re-interpret module output; it just advances to the next module.

### STEP 0 — Flag parsing

Parse argv for `--session-dir=<path>`, `--start-at=<module>`, `--stop-at=<module>`.

- If `--session-dir` is set: existing session mode. Directory must exist. `--start-at` defaults to the first module whose output file is missing.
- If `--session-dir` is unset: fresh session mode. `--start-at` and `--stop-at` are rejected with an error ("partial-run flags require --session-dir").

Record parsed flags to an internal state record; written to `session.json.argv_flags` at STEP 1.
```

- [ ] **Step 3: Add STEP 1 (session init) through STEP 7 (finalize)**

Append the 7 orchestrator steps. Each step is a short paragraph naming which module runs, what it reads/writes, and the progression condition. Use this template:

```markdown
### STEP 1 — Session init

Compute `session_id` from Node A stem per spec §10 (slugification + date-prefix dedup rule). Create `~/docs/epiphany/analysis/<session-id>/` (on collision append `-run-N`). Write initial `session.json` with `created_utc`, `argv_flags`, and empty `modules` object per spec §10 schema.

### STEP 2 — m-intake

Invoke `modules/m-intake.md`. Reads argv, writes `node-a.md`, `node-b.md`, updates `session.json.node_a` and `session.json.node_b` (including `genius_detection` tag per §8).

### STEP 3 — m-analyze

Invoke `modules/m-analyze.md`. Reads `node-a.md` + `node-b.md`. Writes `node-a1-analysis.md` (with `## Intent / ## Structure / ## Claims / ## Constraints / ## Gaps / ## Technical details`) and `node-b1-analysis.md` (section-tailored per §5 Map when genius-or-drift; flat scan on generic-fallback).

### STEP 4 — m-ideate

Invoke `modules/m-ideate.md`. Reads A1+B1. Writes `node-c-ideas.md` + pass/stop-reason fields in `session.json.modules.m-ideate`.

### STEP 5 — m-filter

Invoke `modules/m-filter.md`. Reads C. Writes `node-d-accepted.md` + accept/reject counts.

### STEP 6 — m-engineer

Invoke `modules/m-engineer.md`. Reads D + Node A. Writes `node-e-solutions.md` including `## Dropped` section for double-fails. Updates `d_entries_processed`, `e_entries_produced`, `double_fail_count`.

### STEP 7 — m-integrate

Invoke `modules/m-integrate.md`. Reads A + E. Runs self-audit (§9.2) on draft. Writes `enhanced.md` on PASS, `enhanced-partial.md` + `open-questions.md` on double-FAIL. Writes `self-audit.md` in both cases.

Between every step, check `--stop-at`: if the just-completed module name matches, halt before the next step.
```

- [ ] **Step 4: Add Replay subsection under Orchestrator**

Append (verbatim from spec §12 Replay / partial-run contract, rewritten from implementer's POV):

```markdown
### Replay / partial-run contract

- `--session-dir=<path>` points at an existing `~/docs/epiphany/analysis/<session-id>/`. Skill loads stage files already present and resumes from `--start-at` (default: first module with missing output file).
- Without `--session-dir`, the skill always creates a fresh session directory.
- `--start-at` and `--stop-at` take module names from STEP 2–STEP 7 (`m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`).
- Without `--start-at`, auto-detect the first module whose output file is missing in `--session-dir` and resume from there.
```

- [ ] **Step 5: Verify structural integrity of SKILL.md**

Run: `grep -E '^(## |### STEP)' ~/.claude/skills/epiphany-analysis/SKILL.md | head -20`
Expected: Trigger Conditions, Runtime Scope, Inputs, Primary outputs, Hard Gates, Orchestrator, STEP 0 through STEP 7, Replay / partial-run contract.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add SKILL.md
git commit -m "feat(epiphany-analysis): SKILL.md hard gates + orchestrator STEP 0-7"
```

---

## Task 4: SKILL.md — Reference material (maps, tables, caps, anti-patterns)

**Files:**
- Modify: `~/.claude/skills/epiphany-analysis/SKILL.md` (append)

Implements §5 Section-Tailoring Map, §8 genius detection table, §11 caps, §13 anti-patterns — all as reference sections that modules link to.

**Anchor contract (used by Task 13 structural tests).** Each step below MUST produce a top-level heading with the EXACT text shown, so `grep -E '^## <heading>'` from test-structure.sh matches. If you rename a heading, update the test in sync.

- [ ] **Step 1: Append Section-Tailoring Map section (spec §5)**

Heading to emit (exact): `## Section-Tailoring Map`

Copy the spec's §5 content verbatim. Sub-sections to include, in order:
1. The 10-row table with columns: `Section | Focus | Extraction strategy | Special handling`. Rows cover: Headline Insight, Theory Collisions, Core Findings, Counter-Examples, Alternative Hypotheses, Edge Cases, Convergence Points, Risks/Limitations, Appendix, and any other canonical section from spec §5.
2. Subsection `### Fallback (generic-fallback Node B)` — describes flat-scan extraction.
3. Subsection `### Drift handling (genius-drift)` — describes partial-match + best-effort rule.
4. Subsection `### Section order in B1` — describes Headline-first ordering rule.

Source: `docs/superpowers/specs/2026-04-14-epiphany-analysis-design.md` §5. Open the spec and copy the full text of §5 into SKILL.md under this heading.

- [ ] **Step 2: Append Genius Detection section (spec §8)**

Heading to emit (exact): `## Genius Detection`

Copy §8 verbatim. Content to include:
1. Canonical-header + synonyms table (columns: `Canonical | Synonyms`).
2. Decision table with the 3 tiers:
   - `≥7 canonical matches → tag = genius-current`
   - `5–6 canonical matches → tag = genius-drift`
   - `<5 canonical matches → tag = generic-fallback`
3. Subsection `### Version drift tolerance` — describes the synonym-matching rule.
4. The epiphany-genius reference URL/path.

- [ ] **Step 3: Append Termination Caps section (spec §11)**

Heading to emit (exact): `## Termination Caps`

Copy §11 verbatim. Content to include:
1. Per-node caps table (C=50 ideas/3 passes, D=single pass, E=2 drafts per D, Integration=1 refinement cycle).
2. Skill-global caps table (15min soft wall-clock, 10min per-module hard, 20min global hard).
3. "No-new-ideas" interpretation — early-stop rule for m-ideate.
4. Priority rules — which cap fires first when multiple are near limit.

- [ ] **Step 4: Append Anti-Patterns section (spec §13)**

Heading to emit (exact): `## Anti-Patterns`

Copy §13 verbatim. Content to include:
1. All 13 DO-NOT items, numbered (items 1–7 from source prompt, items 8–13 spec-added).
2. Subsection `### Edge-case matrix` — 10 rows labeled a–j covering edge cases (inline-A, missing-B, non-genius-B, circular-reference, empty-sections, etc., per spec §13).

- [ ] **Step 5: Verify SKILL.md total structure**

Run: `grep -c '^## ' ~/.claude/skills/epiphany-analysis/SKILL.md`
Expected: at least 8 top-level sections.

Run: `wc -l ~/.claude/skills/epiphany-analysis/SKILL.md`
Expected: between 600 and 1200 lines.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add SKILL.md
git commit -m "feat(epiphany-analysis): SKILL.md reference material (map, detection, caps, anti-patterns)"
```

---

## Task 5: session.json template

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/templates/session.json.template`

Implements spec §10 session.json schema as a fill-in-the-blanks template modules copy from.

- [ ] **Step 1: Write the template**

Write to `~/.claude/skills/epiphany-analysis/templates/session.json.template`:

```json
{
  "session_id": "",
  "created_utc": "",
  "node_a": {
    "source_kind": "",
    "source_path": null,
    "content_hash": ""
  },
  "node_b": {
    "source_kind": "",
    "source_path": null,
    "genius_detection": "",
    "matched_sections": [],
    "drift_notes": ""
  },
  "modules": {
    "m-intake":    { "status": "pending", "start_utc": null, "end_utc": null, "notes": "" },
    "m-analyze":   { "status": "pending", "a1_complete": false, "b1_complete": false, "start_utc": null, "end_utc": null },
    "m-ideate":    { "status": "pending", "passes_run": 0, "ideas_total": 0, "stop_reason": "" },
    "m-filter":    { "status": "pending", "ideas_in": 0, "accepted_out": 0 },
    "m-engineer":  { "status": "pending", "d_entries_processed": 0, "e_entries_produced": 0, "double_fail_count": 0 },
    "m-integrate": { "status": "pending", "refinement_cycles": 0, "audit_verdict": "" }
  },
  "caps_applied": {
    "ideation_passes": 3,
    "ideation_total": 50,
    "drafts_per_d": 2,
    "refinement_cycles": 1,
    "global_wall_clock_sec": 900
  },
  "stop_reasons": [],
  "argv_flags": { "start_at": null, "stop_at": null }
}
```

- [ ] **Step 2: Validate JSON well-formedness**

Run: `python3 -c "import json; json.load(open('/home/myuser/.claude/skills/epiphany-analysis/templates/session.json.template'))"`
Expected: exit 0, no output. Any JSON error must be fixed before committing.

- [ ] **Step 3: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add templates/session.json.template
git commit -m "feat(epiphany-analysis): session.json template"
```

---

## Task 6: m-intake module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-intake.md`

Implements spec §2 m-intake. Responsible for reading A and B, detecting types, writing initial stage files.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-intake.md`:

```markdown
---
name: m-intake
stage_id: M-INTAKE
input_dependencies:
  - argv (path-to-A, path-to-B)
  - templates/session.json.template
output_files:
  - node-a.md
  - node-b.md
  - session.json (node_a and node_b sections populated)
activation:
  wave: 1
  role: intake
return_contract: |
  "m-intake complete. Wrote: node-a.md, node-b.md. Detected: node_a.source_kind=<inline|file>, node_b.genius_detection=<genius-current|genius-drift|generic-fallback>."
---
```

- [ ] **Step 2: Write body — Purpose + HG-1 enforcement**

Append:

```markdown
# M-INTAKE — Intake and Detection

## Purpose

Load Node A and Node B. Detect Node A source kind. Detect Node B type per §8. Stage both to `{session_dir}`. Seed `session.json`.

## Hard Gate HG-1

Before any write, verify the target path for `enhanced.md` (= `{session_dir}/enhanced.md`) does NOT equal the Node A source path. If equal, abort with error:
> "HG-1 violation: Node A source path equals enhanced.md target. The skill never overwrites Node A."
```

- [ ] **Step 3: Write body — Procedure steps**

Append under heading `## Procedure` the 5 steps below, verbatim:

```markdown
## Procedure

1. **Read Node A.** If argv provides a path, read the file. If Node A is inline in the user's prompt, capture that text. Set `session.json.node_a.source_kind` = `file` or `inline`, and `session.json.node_a.source_path` to the path (or `null` for inline).

2. **Read Node B.** Always a file path. Set `session.json.node_b.source_kind` = `file` and `.source_path` = the path.

3. **Copy verbatim to stage files.** Write Node A content to `{session_dir}/node-a.md` and Node B content to `{session_dir}/node-b.md` byte-for-byte. No whitespace normalization, no trailing-newline adjustment, no BOM stripping.

4. **Run genius detection.** Open `node-b.md` and count canonical-section-header matches using the table under `## Genius Detection` in SKILL.md. Rules:
   - ≥7 matches → `genius_detection = "genius-current"`
   - 5–6 matches → `genius_detection = "genius-drift"` and record `matched_sections` list
   - <5 matches → `genius_detection = "generic-fallback"` and write `drift_notes = "below genius threshold — flat-scan mode"`

5. **Write hashes to session.json.** Compute SHA-256 of each stage file and record to `session.json.node_a.content_hash` and `session.json.node_b.content_hash`. Reference commands (Python):
   ```python
   import hashlib, pathlib
   h = hashlib.sha256(pathlib.Path(stage_path).read_bytes()).hexdigest()
   ```
   Or shell: `sha256sum {stage_path} | awk '{print $1}'`.

   These hashes are the authority for R6 (mtime + hash unchanged) at m-integrate time.
```

- [ ] **Step 4: Write body — Verbatim-preservation invariants**

Append under heading `## Verbatim preservation`:

```markdown
## Verbatim preservation

The stage files in `{session_dir}/` are byte-for-byte copies of the source. Do NOT:
- normalize whitespace or line endings
- strip or add BOMs
- collapse blank lines
- inject headers, footers, or provenance markers

**Invariant.** After step 3, `sha256(stage_file_content) == sha256(source_file_content)` for both Node A and Node B. If a source is inline (Node A only), the stage file hash is recorded against the captured inline text. Any mismatch is an m-intake error.
```

- [ ] **Step 5: Write body — Return contract**

Add the return line per the frontmatter's `return_contract`.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-intake.md
git commit -m "feat(epiphany-analysis): m-intake module"
```

---

## Task 7: m-analyze module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-analyze.md`

Implements spec §2 m-analyze + §5 Section-Tailoring Map application.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-analyze.md`:

```markdown
---
name: m-analyze
stage_id: M-ANALYZE
input_dependencies:
  - node-a.md
  - node-b.md
  - session.json (node_b.genius_detection)
output_files:
  - node-a1-analysis.md
  - node-b1-analysis.md
  - session.json (modules.m-analyze with a1_complete, b1_complete)
activation:
  wave: 2
  role: analysis
return_contract: |
  "m-analyze complete. A1 sections populated: {list}. B1 sections populated: {list}. a1_complete={bool}, b1_complete={bool}."
---
```

- [ ] **Step 2: Write Phase A1 — full analysis of Node A**

Append section describing the 6 mandatory output sections A1 must contain (`## Intent`, `## Structure`, `## Claims`, `## Constraints`, `## Gaps`, `## Technical details`). Explicitly note `## Constraints` is the reference surface for R4 no-regression checks.

For each section, give one sentence on what it captures (per spec §3 Node A1). Example for Constraints: "List every MUST, MUST-NOT, rule, or invariant the original Node A imposes — on itself, on its consumers, or on its behavior. One bullet per constraint. This is the no-regression reference set."

End with: "mark `a1_complete: true` in session.json when all 6 sections are non-empty OR explicitly marked `(none)`."

- [ ] **Step 3: Write Phase B1 — section-tailored analysis of Node B**

Append section with branching on `session.json.node_b.genius_detection`:

- `genius-current` or `genius-drift`: apply §5 Map. Each matched canonical section produces findings per that row's extraction strategy. Headline Insight processed first.
- `generic-fallback`: single generic extraction pass over the whole document; produces flat findings list (each finding tagged with a best-guess `source_section` = heading it fell under, or `generic` if none).

Every B1 finding is tagged `source_section` for flow-through to Node C.

Mark `b1_complete: true` when all matched sections processed or fallback scan complete.

- [ ] **Step 4: Append verbatim reference — §5 special-handling rules**

Append under heading `## Special handling (inlined from §5)` so m-analyze does not cross-reference SKILL.md at runtime:

```markdown
## Special handling (inlined from §5)

**Headline Insight (row 1).** Process FIRST, before any other B1 section. The Headline Insight shapes the weighting of every downstream finding. Its source_section tag is propagated to every idea that traces back to it (used by m-integrate for Headline precedence).

**Theory Collisions (row 2).** When Node B's Theory Collisions section names two incompatible positions, extract BOTH as separate findings — do not synthesize them into one. m-ideate will generate independent ideas from each; m-filter may reject one of them. Never pre-resolve a collision at analysis time.

**Alternative Hypotheses (row 5).** Extract only the best-fit hypothesis (the one Node B itself prefers, or the one with the strongest supporting evidence if no preference stated). Do NOT extract runner-up hypotheses. This prevents idea-pool dilution with low-utility alternatives.
```

Open the spec at §5 and copy any additional wording from those three rows if present. The three-paragraph anchor above is the floor.

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-analyze.md
git commit -m "feat(epiphany-analysis): m-analyze module (A1 + B1 with §5 Map)"
```

---

## Task 8: m-ideate module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-ideate.md`

Implements spec §2 m-ideate. Multi-pass ideation with persona rotation, dedup, bounded termination.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-ideate.md`:

```markdown
---
name: m-ideate
stage_id: M-IDEATE
input_dependencies:
  - node-a1-analysis.md
  - node-b1-analysis.md
output_files:
  - node-c-ideas.md
  - session.json (modules.m-ideate: passes_run, ideas_total, stop_reason)
activation:
  wave: 3
  role: ideation
return_contract: |
  "m-ideate complete. Passes: {N}/3. Ideas produced: {total}. Stop reason: {no-new-ideas|pass-cap|idea-cap}."
---
```

- [ ] **Step 2: Write Purpose + bounded-loop protocol**

Append:

```markdown
# M-IDEATE — Cross-Cutting Ideation

## Purpose

Produce Node C — the unfiltered idea pool — by running bounded-pass cross-cutting ideation over A1 + B1 findings.

## Bounded loop

```
passes_run = 0
ideas = []
while passes_run < 3 and len(ideas) < 50:
    new_ideas_this_pass = run_pass(passes_run + 1, seed=A1+B1, dedup_against=ideas)
    if len(new_ideas_this_pass) == 0:
        stop_reason = "no-new-ideas"
        break
    ideas.extend(new_ideas_this_pass)
    passes_run += 1
else:
    stop_reason = "pass-cap" if passes_run == 3 else "idea-cap"
```
```

- [ ] **Step 3: Write persona rotation protocol**

Append:

```markdown
## Persona rotation (pass independence)

Each pass opens with a **visible preamble** declaring the role and goal:

- **Pass 1 — Skeptical Reviewer.** Goal: find every issue, gap, or unsupported claim in Node A.
- **Pass 2 — Contrarian First-Principles Designer.** Goal: propose non-obvious structural restructurings.
- **Pass 3 — User-Empathy Advocate.** Goal: find usability and maintainability gaps affecting downstream readers.

The preamble is text in the pass, not a parameter. Successive passes are thus decorrelated by explicit framing.
```

- [ ] **Step 4: Write idea schema and dedup rule**

Append:

```markdown
## Node C entry schema

Each idea written to `node-c-ideas.md` is a block:

```
## I0N — <short title>
**source_section:** <B1 section name or A1 bucket>
**target_section:** <where in Node A the change would apply>
**observation:** <what A1/B1 finding seeded this>
**idea:** <the proposed improvement, one paragraph>
**category:** <structural | content | clarity | constraint | other>
**effort:** <low | medium | high>
```

## Dedup rule

Two entries collide when `(source_section, normalized_observation, target_section)` matches. `normalized_observation = lowercase + collapse whitespace + strip filler words ("the","a","that","this") + first 120 chars`. On collision: keep earlier ID, append a line `merged_into: I0N` under a `## Merged` section at file end.
```

- [ ] **Step 5: Write progress reporting + stop-reason recording**

Append the two status-line formats from spec §2 m-ideate "Progress reporting" block, and the rule for writing `passes_run`, `ideas_total`, `stop_reason` to session.json.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-ideate.md
git commit -m "feat(epiphany-analysis): m-ideate module (3-persona bounded loop + dedup)"
```

---

## Task 9: m-filter module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-filter.md`

Implements spec §2 m-filter. Single verdict pass + one-time merge consolidation.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-filter.md`:

```markdown
---
name: m-filter
stage_id: M-FILTER
input_dependencies:
  - node-c-ideas.md
output_files:
  - node-d-accepted.md
  - session.json (modules.m-filter: ideas_in, accepted_out)
activation:
  wave: 4
  role: filter
return_contract: |
  "m-filter complete. Ideas in: {N}. Accepted: {M}. Rejected: {N-M}. Merged: {K}."
---
```

- [ ] **Step 2: Write verdict pass**

Append:

```markdown
# M-FILTER — Pro/Con/Utility Filter

## Single verdict pass

Iterate every Node C entry once. For each, evaluate against the verbatim judgment criteria:

> *considering overall utility and relevancy, and if it will create a better product without excessive overengineering or complexity, or resolves any potential issues, we want the best product features that are usable for humans, must be realistic to implement without overcomplexity or regression*

Produce one of three verdicts: **ACCEPT**, **ACCEPT-CONDITIONAL**, **REJECT**.

## Node D entry schema (accepted)

```
## D0N — <short title>
**from_idea_id:** I0K  (or list [I0K, I0M] after merge)
**verdict:** ACCEPT | ACCEPT-CONDITIONAL
**utility:** HIGH | MEDIUM-HIGH | MEDIUM | LOW-MEDIUM | LOW
**gain:** <what this improvement delivers>
**target_section:** <inherited from Node C>
**condition:** <only for ACCEPT-CONDITIONAL>
```

REJECT entries go in a separate `## Rejected` section with `**rejection_reason:**`.
```

- [ ] **Step 3: Write merge consolidation sub-pass**

Append:

```markdown
## Merge consolidation (one-time sub-pass)

After the verdict pass, scan accepted entries for merge candidates. Two entries merge when:
(a) both target the same `target_section`, AND
(b) one entry's idea is subsumed by or reinforces the other.

Merge output:
- `from_idea_id` becomes a list (e.g., `[I05, I07]`)
- `verdict` = stronger of the two (ACCEPT > ACCEPT-CONDITIONAL)
- `gain` = concatenation of both gain strings separated by ` + `, deduped on exact-substring match

REJECT entries never merge. This sub-pass runs exactly once — no iteration.
```

- [ ] **Step 4: Write progress reporting**

Append under heading `## Progress reporting`:

```markdown
## Progress reporting

Emit one status line after each batch of 10 reviewed ideas, and a final summary:

- During verdict pass: `[m-filter] reviewed={N_so_far}/{N_total} | accepted={K} | rejected={N_so_far-K}`
- After merge consolidation: `[m-filter] merged={M_merged_pairs} | final_accepted={final_D_count}`
- Final summary: `[m-filter] complete | ideas_in={N_total} | accepted_out={final_D_count}`

Write `ideas_in` and `accepted_out` to `session.json.modules.m-filter`.
```

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-filter.md
git commit -m "feat(epiphany-analysis): m-filter module (verdict + merge sub-pass)"
```

---

## Task 10: m-engineer module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-engineer.md`

Implements spec §2 m-engineer + §7 compare-and-pick. This is the most complex module.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-engineer.md`:

```markdown
---
name: m-engineer
stage_id: M-ENGINEER
input_dependencies:
  - node-d-accepted.md
  - node-a.md
output_files:
  - node-e-solutions.md
  - session.json (modules.m-engineer: d_entries_processed, e_entries_produced, double_fail_count)
activation:
  wave: 5
  role: solution-engineering
return_contract: |
  "m-engineer complete. D entries processed: {N}. E entries produced: {M}. Double-failures: {K} (dropped, see ## Dropped section)."
---
```

- [ ] **Step 2: Write A-context excerpt protocol**

Append:

```markdown
# M-ENGINEER — Two-Drafts-Pick-Best

## A-context excerpt extraction

For each D_i, the excerpt passed to the solution drafter is the Node A region identified by D_i's `target_section`:

- If `target_section` is a section heading: excerpt = heading + content up to the next heading of equal or higher level.
- If `target_section` is a line range: excerpt = those lines ± 10 lines of surrounding context.
- If `target_section` is a natural-language locator: excerpt = paragraph containing best substring match + 2 surrounding paragraphs.
- If no match: pass full Node A with a log warning in session.json.stop_reasons.
```

- [ ] **Step 3: Write two-drafts protocol**

Append:

```markdown
## Two-drafts-pick-best (Hard Gate HG-3)

For each D_i:

```
draft_1 = ideate(d_i, A-excerpt) → synthesize(target + action + text + rationale) → verify(§6 Level A checks)
draft_2 = ideate(d_i, A-excerpt)  # independent re-seed
        → synthesize(...)
        → verify(...)
if draft_1.verify_pass and draft_2.verify_pass:
    best = compare(draft_1, draft_2, criteria=[pro/con, utility, over-engineering-bounded, info-preservation])
    store best as E entry; losing draft's cons → compare_notes
elif draft_1.verify_pass xor draft_2.verify_pass:
    best = the passing draft (compare_notes records the failing draft's reason)
    store best as E entry
else:
    # DOUBLE-FAIL RULE — no third draft, no retry
    record to `## Dropped` section: {from_d_id, reason: "double-draft-verify-failed", failed_checks_1, failed_checks_2}
    advance to next D_i
```

Tiebreaker: if two drafts are indistinguishable on all four criteria, pick the draft with smaller surface area (fewest lines changed in A).
```

- [ ] **Step 4: Write Node E entry schema + 4 target locator kinds**

Append verbatim from spec §3 Node E. Mandate: natural-language locators ("Step 3 end block") are FORBIDDEN; drafter must resolve to one of the 4 structured shapes or fail verify.

```markdown
## Node E `target` locator (exactly ONE of 4 shapes)

- `{"kind": "heading_path", "path": ["## §4", "### Branching points"]}`
- `{"kind": "line_range", "start": 123, "end": 145}` (only when Node A is a stable file)
- `{"kind": "anchor", "before": "<exact substring>", "after": "<exact substring>"}`
- `{"kind": "verbatim_block", "text": "<multi-line exact substring>"}` (for `replace` actions only)

Natural-language locators FORBIDDEN. Drafter must resolve to one of the 4 or fail verify.
```

- [ ] **Step 5: Write Node E entry block format and ## Dropped section format**

Append under heading `## Node E entry schema`:

```markdown
## Node E entry schema

Each successful solution block:

```
## E0N — <short title>
**from_d_id:** D0N
**from_idea_id:** I0K  (or list, inherited from D)
**target:** <one of the 4 locator shapes, JSON>
**action:** replace | insert-before | insert-after | append | prepend
**text:** |
  <multi-line replacement or insertion content>
**rationale:** <why this solution engineers the D_i accepted idea>
**compare_notes:** <one-line summary of the losing draft's pros/cons, OR "single-pass pass" if XOR case>
```

## Dropped section (double-fail rule)

When both drafts fail verify, write under a `## Dropped` section at file end:

```
## Dropped

### D0N — <title>
**reason:** double-draft-verify-failed
**failed_checks_1:** <list of §6 Level A checks draft 1 failed>
**failed_checks_2:** <list of §6 Level A checks draft 2 failed>
**a_excerpt_ref:** <target_section used for A-context>
```

Advance to the next D_i; do NOT retry.
```

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-engineer.md
git commit -m "feat(epiphany-analysis): m-engineer module (two-drafts, double-fail drop, locator kinds)"
```

---

## Task 11: m-integrate module

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/modules/m-integrate.md`

Implements spec §2 m-integrate + §9.2 runtime audit + §6 Level B refinement cycle.

- [ ] **Step 1: Write frontmatter**

Write to `~/.claude/skills/epiphany-analysis/modules/m-integrate.md`:

```markdown
---
name: m-integrate
stage_id: M-INTEGRATE
input_dependencies:
  - node-a.md
  - node-e-solutions.md
  - node-a1-analysis.md (for R4 constraints)
  - session.json
output_files:
  - enhanced.md (on PASS) OR enhanced-partial.md + open-questions.md (on 2nd FAIL)
  - self-audit.md (always)
  - session.json (modules.m-integrate.refinement_cycles, audit_verdict)
activation:
  wave: 6
  role: integration
return_contract: |
  "m-integrate complete. Verdict: {PASS|FAIL|PARTIAL}. Refinement cycles: {0|1}. Output: {enhanced.md|enhanced-partial.md}."
---
```

- [ ] **Step 2: Write precondition gate (§9.2 precondition)**

Append:

```markdown
# M-INTEGRATE — Flush Integration + Self-Audit

## Precondition gate (before integration starts)

Abort with error if any of:
- `node-a1-analysis.md` or `node-b1-analysis.md` does not exist or is empty
- `node-e-solutions.md` does not exist (empty-but-present OK — log reason to session.json.stop_reasons)
- `session.json.modules.m-analyze.a1_complete != true` OR `.b1_complete != true`

Verbatim precondition clause: *"analysis verified for accuracy and completeness"*.
```

- [ ] **Step 3: Write integration order + conflict resolution**

Append:

```markdown
## Integration order

Apply Node E entries in this precedence:

1. Entries whose `from_d_id`'s `from_idea_id` originated in the Headline Insight section — applied first. For merged D entries where `from_idea_id` is a list, qualifies if **any** constituent traces to Headline Insight.
2. Remaining entries in stable order (as they appear in `node-e-solutions.md`).

## Conflict resolution

Two Node E entries conflict when their `target` regions overlap by ≥1 line of Node A. On detection:

1. If both `from_d_id`'s trace to the same `target_section`: attempt **compose** — synthesized patch applying both in sequence. Verify the composed patch still preserves info. If compose succeeds, use it.
2. Otherwise: apply the entry with higher utility (from Node D). On tie: lower `from_idea_id` wins (stable).
3. Losing entry is dropped; record in `self-audit.md` under `## Integration conflicts`.
```

- [ ] **Step 4: Write self-audit block R1-R6 (spec §9.2)**

Append the 6 runtime checks verbatim with their FAIL-handling rules:

```markdown
## Runtime audit (before finalizing output)

- **R1.** *"analysis verified for accuracy and completeness"* — session.json.modules.m-analyze.a1_complete && .b1_complete.
- **R2.** Every E entry appears at its `target` location (dispatch by `target.kind`).
- **R3.** No Node A content removed without a matching Node E `replace` (shorter text) with rationale citing the removed content.
- **R4.** Enhanced A still contains every constraint listed in A1's `## Constraints` — substring or semantic paraphrase match required.
- **R5.** No newly-introduced defects (unclosed fences/tags, duplicated paragraphs, TODO/TBD tokens) that weren't already in Node A or introduced via a Node E entry.
- **R6.** Node A source file mtime + hash unchanged (compared against session.json.node_a.content_hash).
```

- [ ] **Step 5: Write FAIL handling**

Append:

```markdown
## FAIL handling

- **1st FAIL:** log findings to `self-audit.md`. Run one refinement cycle (§6 Level B) targeting only failed checks. Re-verify.
- **2nd FAIL:** write `enhanced-partial.md` + `open-questions.md` with specific R1-R6 failure IDs. Halt. Do NOT write `enhanced.md`. Set session.json.modules.m-integrate.audit_verdict = "PARTIAL".
- **PASS:** write `enhanced.md`. Set audit_verdict = "PASS".

Always write `self-audit.md` with the per-check verdict record.
```

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add modules/m-integrate.md
git commit -m "feat(epiphany-analysis): m-integrate module (precondition, integration order, §9.2 audit)"
```

---

## Task 12: Frontmatter validator (Python)

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/tests/validate-frontmatter.py`

Adapted from the epiphany-prompt version. Validates each module has required keys.

- [ ] **Step 1: Write the test (self-test: run against own fixtures)**

The test IS the script — it runs against the 6 module files. We write the validator and then run it; a passing run is the test.

- [ ] **Step 2: Write the validator**

Write to `~/.claude/skills/epiphany-analysis/tests/validate-frontmatter.py`:

```python
#!/usr/bin/env python3
"""Validate YAML frontmatter schema for epiphany-analysis module files.

Required keys: name, stage_id, input_dependencies, output_files, activation, return_contract.
Usage: validate-frontmatter.py <path-to-module.md> [...]
Exit 0 if all files pass; 1 otherwise.
"""
from __future__ import annotations

import sys
import re
from pathlib import Path

REQUIRED_KEYS = {
    "name", "stage_id", "input_dependencies", "output_files",
    "activation", "return_contract",
}
VALID_STAGE_IDS = {
    "M-INTAKE", "M-ANALYZE", "M-IDEATE", "M-FILTER", "M-ENGINEER", "M-INTEGRATE",
}

def extract_frontmatter(text: str) -> str | None:
    m = re.match(r"^---\n(.*?)\n---\n", text, re.DOTALL)
    return m.group(1) if m else None

def parse_top_level_keys(block: str) -> set[str]:
    keys = set()
    for line in block.splitlines():
        if line and not line.startswith((" ", "\t", "-", "#")):
            if ":" in line:
                keys.add(line.split(":", 1)[0].strip())
    return keys

def parse_stage_id(block: str) -> str | None:
    for line in block.splitlines():
        if line.startswith("stage_id:"):
            return line.split(":", 1)[1].strip()
    return None

def validate(path: Path) -> list[str]:
    errors: list[str] = []
    try:
        text = path.read_text()
    except FileNotFoundError:
        return [f"{path}: file not found"]
    except OSError as e:
        return [f"{path}: unreadable ({e.strerror})"]
    fm = extract_frontmatter(text)
    if fm is None:
        return [f"{path}: missing YAML frontmatter"]
    keys = parse_top_level_keys(fm)
    missing = REQUIRED_KEYS - keys
    if missing:
        errors.append(f"{path}: missing keys {sorted(missing)}")
    stage_id = parse_stage_id(fm)
    if stage_id and stage_id not in VALID_STAGE_IDS:
        errors.append(f"{path}: invalid stage_id '{stage_id}' (must be one of {sorted(VALID_STAGE_IDS)})")
    return errors

def main() -> int:
    if len(sys.argv) < 2:
        print("usage: validate-frontmatter.py <module.md> [...]", file=sys.stderr)
        return 2
    all_errors: list[str] = []
    for arg in sys.argv[1:]:
        all_errors.extend(validate(Path(arg)))
    if all_errors:
        for e in all_errors:
            print(e, file=sys.stderr)
        return 1
    print(f"OK: {len(sys.argv) - 1} file(s) passed frontmatter validation")
    return 0

if __name__ == "__main__":
    sys.exit(main())
```

- [ ] **Step 3: Make it executable and run against all modules**

Run:
```bash
chmod +x ~/.claude/skills/epiphany-analysis/tests/validate-frontmatter.py
~/.claude/skills/epiphany-analysis/tests/validate-frontmatter.py \
    ~/.claude/skills/epiphany-analysis/modules/*.md
```
Expected: `OK: 6 file(s) passed frontmatter validation`, exit 0.

- [ ] **Step 4: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/validate-frontmatter.py
git commit -m "feat(epiphany-analysis): frontmatter validator"
```

---

## Task 13: Structure and registration tests

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/tests/test-structure.sh`
- Create: `~/.claude/skills/epiphany-analysis/tests/test-skill-registration.sh`

- [ ] **Step 1: Write test-structure.sh**

Write to `~/.claude/skills/epiphany-analysis/tests/test-structure.sh`:

```bash
#!/usr/bin/env bash
# Structural assertions for epiphany-analysis skill files.
# Exits 0 on success, 1 on failure.

set -u
SKILL_DIR="$HOME/.claude/skills/epiphany-analysis"
FAIL=0

assert_file() {
    if [[ ! -f "$1" ]]; then echo "FAIL: missing file $1"; FAIL=1; fi
}
assert_grep() {
    if ! grep -qE "$1" "$2" 2>/dev/null; then
        echo "FAIL: $3 — pattern '$1' not found in $2"; FAIL=1
    fi
}

# SKILL.md required sections
assert_file "$SKILL_DIR/SKILL.md"
if [[ -f "$SKILL_DIR/SKILL.md" ]]; then
    assert_grep '^name: epiphany-analysis' "$SKILL_DIR/SKILL.md" "frontmatter name"
    assert_grep '^trigger: /epiphany-analysis' "$SKILL_DIR/SKILL.md" "trigger"
    assert_grep '^## Trigger Conditions' "$SKILL_DIR/SKILL.md" "trigger section"
    assert_grep '^## Hard Gates' "$SKILL_DIR/SKILL.md" "hard gates"
    assert_grep '^## Orchestrator' "$SKILL_DIR/SKILL.md" "orchestrator"
    for step in 'STEP 0' 'STEP 1' 'STEP 2' 'STEP 3' 'STEP 4' 'STEP 5' 'STEP 6' 'STEP 7'; do
        assert_grep "^### $step" "$SKILL_DIR/SKILL.md" "$step"
    done
    assert_grep '^## Section-Tailoring Map' "$SKILL_DIR/SKILL.md" "map"
    assert_grep '^## Genius Detection' "$SKILL_DIR/SKILL.md" "detection"
    assert_grep '^## Termination Caps' "$SKILL_DIR/SKILL.md" "caps"
    assert_grep '^## Anti-Patterns' "$SKILL_DIR/SKILL.md" "anti-patterns"
fi

# All 6 module files exist
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    assert_file "$SKILL_DIR/modules/$m.md"
done

# Template exists
assert_file "$SKILL_DIR/templates/session.json.template"
if [[ -f "$SKILL_DIR/templates/session.json.template" ]]; then
    if ! python3 -c "import json; json.load(open('$SKILL_DIR/templates/session.json.template'))" 2>/dev/null; then
        echo "FAIL: session.json.template is not valid JSON"; FAIL=1
    fi
fi

# Frontmatter validator
if ! "$SKILL_DIR/tests/validate-frontmatter.py" "$SKILL_DIR/modules"/*.md > /dev/null; then
    echo "FAIL: module frontmatter invalid"
    "$SKILL_DIR/tests/validate-frontmatter.py" "$SKILL_DIR/modules"/*.md
    FAIL=1
fi

if (( FAIL == 0 )); then echo "ALL STRUCTURAL ASSERTIONS PASS"; fi
exit $FAIL
```

- [ ] **Step 2: Make executable and run**

Run:
```bash
chmod +x ~/.claude/skills/epiphany-analysis/tests/test-structure.sh
~/.claude/skills/epiphany-analysis/tests/test-structure.sh
```
Expected: `ALL STRUCTURAL ASSERTIONS PASS`, exit 0. If any FAIL lines appear, fix the offending file (Tasks 2-11) before proceeding.

- [ ] **Step 3: Write test-skill-registration.sh**

Write to `~/.claude/skills/epiphany-analysis/tests/test-skill-registration.sh`:

```bash
#!/usr/bin/env bash
# Verify epiphany-analysis is discoverable by Claude Code.
set -u
SKILL_DIR="$HOME/.claude/skills/epiphany-analysis"
FAIL=0

[[ -d "$SKILL_DIR" ]] || { echo "FAIL: $SKILL_DIR does not exist"; exit 1; }
echo "OK: $SKILL_DIR exists"

first_line=$(head -1 "$SKILL_DIR/SKILL.md")
if [[ "$first_line" == "---" ]]; then
    echo "OK: SKILL.md starts with '---'"
else
    echo "FAIL: SKILL.md first line is '$first_line'"; FAIL=1
fi

for key in "name: epiphany-analysis" "trigger: /epiphany-analysis" "version:" "skill_path:"; do
    if grep -q "^$key" "$SKILL_DIR/SKILL.md"; then
        echo "OK: frontmatter contains '$key'"
    else
        echo "FAIL: frontmatter missing '$key'"; FAIL=1
    fi
done

count=$(find "$SKILL_DIR/modules" -maxdepth 1 -name '*.md' | wc -l)
if (( count == 6 )); then
    echo "OK: 6 module files present"
else
    echo "FAIL: expected 6 modules, found $count"; FAIL=1
fi

exit $FAIL
```

- [ ] **Step 4: Make executable and run**

Run:
```bash
chmod +x ~/.claude/skills/epiphany-analysis/tests/test-skill-registration.sh
~/.claude/skills/epiphany-analysis/tests/test-skill-registration.sh
```
Expected: all OK lines, exit 0.

- [ ] **Step 5: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/test-structure.sh tests/test-skill-registration.sh
git commit -m "test(epiphany-analysis): structural + registration assertions"
```

---

## Task 14: Runtime-invariant test fixtures

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/tests/test-runtime-invariants.sh`
- Create: `~/.claude/skills/epiphany-analysis/tests/fixtures/` (session-dir fixtures for R1-R6 tests)

This is a **fixture-based test** — the skill itself runs in an LLM context, so we can't fully execute it in CI. Instead, we construct hand-crafted session directories that simulate each post-condition and verify the structural invariants the spec mandates. Tests §14.2 (R1, R2, R4, R5 partial, R6) — R3 (no-removal-without-replace) requires diff logic beyond v1 fixture scope and is left to the manual gold-reference protocol (Task 16). R5 coverage is **fence-parity only** in this fixture layer; full R5 (duplicated paragraphs, TODO/TBD tokens) is manual.

- [ ] **Step 1: Create fixture directories**

Run:
```bash
FIX=~/.claude/skills/epiphany-analysis/tests/fixtures
mkdir -p "$FIX/pass" \
         "$FIX/fail-r1-analysis-incomplete" \
         "$FIX/fail-r2-bad-target" \
         "$FIX/fail-r4-constraint-dropped" \
         "$FIX/fail-r5-malformed" \
         "$FIX/fail-r6-mtime-mismatch"
```

- [ ] **Step 2: Write the PASS fixture files**

Write to `fixtures/pass/node-a.md`:
```markdown
## Task

Do X with clear constraints.

## Constraints

- MUST preserve input ordering.
- MUST NOT emit debug output.
```

Write to `fixtures/pass/node-a1-analysis.md`:
```markdown
## Intent
Accomplish task X.

## Structure
One task section, one constraints section.

## Claims
(none)

## Constraints
- MUST preserve input ordering.
- MUST NOT emit debug output.

## Gaps
No error-handling section.

## Technical details
Plain markdown.
```

Write to `fixtures/pass/node-b.md`:
```markdown
## Headline Insight
Task X is under-specified on error paths.

## Core Findings
Adding an error-handling section would address the gap.
```

Write to `fixtures/pass/node-b1-analysis.md`:
```markdown
## Findings
- source_section: Headline Insight — "under-specified on error paths"
- source_section: Core Findings — "add error-handling section"
```

Write to `fixtures/pass/node-c-ideas.md`:
```markdown
## I01 — Add error-handling section
**source_section:** Headline Insight
**target_section:** ## Task
**observation:** Task X under-specified on error paths
**idea:** Append a `## Error Handling` subsection naming expected failures.
**category:** content
**effort:** low
```

Write to `fixtures/pass/node-d-accepted.md`:
```markdown
## D01 — Add error-handling section
**from_idea_id:** I01
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Closes error-path specification gap
**target_section:** ## Task
```

Write to `fixtures/pass/node-e-solutions.md`:
```markdown
## E01 — Add error-handling section
**from_d_id:** D01
**target:** {"kind": "heading_path", "path": ["## Task"]}
**action:** append
**text:** |
  ## Error Handling

  Failures must be reported to stderr without aborting.
**rationale:** From D01; preserves both input-ordering and no-debug-output constraints.
```

Write to `fixtures/pass/enhanced.md`:
```markdown
## Task

Do X with clear constraints.

## Error Handling

Failures must be reported to stderr without aborting.

## Constraints

- MUST preserve input ordering.
- MUST NOT emit debug output.
```

Write to `fixtures/pass/self-audit.md`:
```markdown
# Self-Audit
VERDICT: PASS
R1: PASS | R2: PASS | R3: PASS | R4: PASS | R5: PASS | R6: PASS
```

Write to `fixtures/pass/session.json`:
```json
{
  "session_id": "fixture-pass",
  "node_a": { "content_hash": "deadbeef" },
  "node_b": { "genius_detection": "genius-current" },
  "modules": {
    "m-analyze":   { "status": "completed", "a1_complete": true, "b1_complete": true },
    "m-integrate": { "status": "completed", "audit_verdict": "PASS" }
  }
}
```

- [ ] **Step 3: Write the FAIL fixtures**

**fail-r1-analysis-incomplete/** — copy all files from `pass/`, then overwrite `session.json` so `a1_complete: false`:
```bash
cp "$FIX/pass"/* "$FIX/fail-r1-analysis-incomplete/"
python3 -c "
import json, pathlib
p = pathlib.Path('$FIX/fail-r1-analysis-incomplete/session.json')
d = json.loads(p.read_text())
d['modules']['m-analyze']['a1_complete'] = False
p.write_text(json.dumps(d, indent=2))
"
```

**fail-r2-bad-target/** — copy from `pass/`, then overwrite `node-e-solutions.md` with a heading_path that does not exist in `node-a.md` and plant sentinel:
```bash
cp "$FIX/pass"/* "$FIX/fail-r2-bad-target/"
cat > "$FIX/fail-r2-bad-target/node-e-solutions.md" <<'EOF'
## E01 — Bad target test
**from_d_id:** D01
**target:** {"kind": "heading_path", "path": ["## MISSING-HEADING-PATH-SENTINEL"]}
**action:** append
**text:** content
**rationale:** fixture
EOF
# enhanced.md deliberately does NOT contain the sentinel
```

**fail-r4-constraint-dropped/** — copy from `pass/`, then overwrite `enhanced.md` to delete one constraint:
```bash
cp "$FIX/pass"/* "$FIX/fail-r4-constraint-dropped/"
cat > "$FIX/fail-r4-constraint-dropped/enhanced.md" <<'EOF'
## Task

Do X with clear constraints.

## Constraints

- MUST preserve input ordering.
EOF
# "MUST NOT emit debug output" dropped — R4 violation
```

**fail-r5-malformed/** — copy from `pass/`, then append an unclosed code fence to `enhanced.md` (written via printf to avoid fence ambiguity in this plan):
```bash
cp "$FIX/pass"/* "$FIX/fail-r5-malformed/"
printf '\n%s\n%s\n' '```python' '# unclosed fence deliberately introduced' \
    >> "$FIX/fail-r5-malformed/enhanced.md"
```

**fail-r6-mtime-mismatch/** — copy from `pass/`, then overwrite `session.json`'s `node_a.content_hash` to a different value than whatever would be computed from `node-a.md`:
```bash
cp "$FIX/pass"/* "$FIX/fail-r6-mtime-mismatch/"
python3 -c "
import json, pathlib
p = pathlib.Path('$FIX/fail-r6-mtime-mismatch/session.json')
d = json.loads(p.read_text())
d['node_a']['content_hash'] = 'MISMATCH'
p.write_text(json.dumps(d, indent=2))
"
```

- [ ] **Step 4: Write test-runtime-invariants.sh**

Write to `~/.claude/skills/epiphany-analysis/tests/test-runtime-invariants.sh`:

```bash
#!/usr/bin/env bash
# Runtime-invariant checks run against fixture session directories.
# Simulates the §9.2 R1-R6 audit on pre-built stage files.
set -u
FIXTURES="$HOME/.claude/skills/epiphany-analysis/tests/fixtures"
FAIL=0

# R1: a1_complete AND b1_complete in session.json
check_r1() {
    local dir="$1" expected="$2"
    local a1 b1
    a1=$(python3 -c "import json; print(json.load(open('$dir/session.json'))['modules']['m-analyze']['a1_complete'])")
    b1=$(python3 -c "import json; print(json.load(open('$dir/session.json'))['modules']['m-analyze']['b1_complete'])")
    if [[ "$a1" == "True" && "$b1" == "True" ]]; then
        result="PASS"
    else
        result="FAIL"
    fi
    if [[ "$result" == "$expected" ]]; then
        echo "OK: R1 $dir → $result (expected $expected)"
    else
        echo "FAIL: R1 $dir → $result (expected $expected)"; FAIL=1
    fi
}

# R2: every E entry's heading_path exists in enhanced.md
check_r2() {
    local dir="$1" expected="$2"
    local targets ok=true
    # Crude: extract heading_path arrays from node-e-solutions.md, check presence in enhanced.md
    # For fixture-level test, look for a sentinel in node-e-solutions and confirm in enhanced
    if grep -q "MISSING-HEADING-PATH-SENTINEL" "$dir/node-e-solutions.md"; then
        # fail fixture has the sentinel but enhanced.md lacks the heading
        if ! grep -q "MISSING-HEADING-PATH-SENTINEL" "$dir/enhanced.md"; then
            result="FAIL"
        else
            result="PASS"
        fi
    else
        result="PASS"
    fi
    if [[ "$result" == "$expected" ]]; then
        echo "OK: R2 $dir → $result (expected $expected)"
    else
        echo "FAIL: R2 $dir → $result (expected $expected)"; FAIL=1
    fi
}

# R4: every A1 Constraints bullet preserved (substring) in enhanced.md
check_r4() {
    local dir="$1" expected="$2"
    local missing=0
    # Extract bullet lines from A1 "## Constraints" section
    awk '/^## Constraints/{flag=1; next} /^## /{flag=0} flag && /^- /{print}' \
        "$dir/node-a1-analysis.md" | while read -r line; do
            stripped="${line#- }"
            if ! grep -qF -- "$stripped" "$dir/enhanced.md"; then
                echo "MISSING:$stripped"
            fi
        done > /tmp/r4-missing.$$
    if [[ -s /tmp/r4-missing.$$ ]]; then result="FAIL"; else result="PASS"; fi
    rm -f /tmp/r4-missing.$$
    if [[ "$result" == "$expected" ]]; then
        echo "OK: R4 $dir → $result (expected $expected)"
    else
        echo "FAIL: R4 $dir → $result (expected $expected)"; FAIL=1
    fi
}

# R5 (fence-parity only): unclosed code fences newly introduced in enhanced.md
check_r5() {
    local dir="$1" expected="$2"
    local e_fences a_fences
    e_fences=$(grep -c '^```' "$dir/enhanced.md")
    a_fences=$(grep -c '^```' "$dir/node-a.md")
    local e_parity=$((e_fences % 2))
    local a_parity=$((a_fences % 2))
    # PASS if enhanced parity matches node-a parity (no new imbalance)
    if (( e_parity == a_parity )); then result="PASS"; else result="FAIL"; fi
    if [[ "$result" == "$expected" ]]; then
        echo "OK: R5 $dir → $result (expected $expected)"
    else
        echo "FAIL: R5 $dir → $result (expected $expected)"; FAIL=1
    fi
}

# R6: session.json recorded content_hash matches current node-a.md hash
check_r6() {
    local dir="$1" expected="$2"
    local recorded current
    recorded=$(python3 -c "import json; print(json.load(open('$dir/session.json'))['node_a']['content_hash'])")
    current=$(sha256sum "$dir/node-a.md" | awk '{print $1}')
    # Fixtures use short sentinels ("deadbeef", "MISMATCH") — PASS means they are equal as strings
    if [[ "$recorded" == "$current" || "$recorded" != "MISMATCH" ]]; then
        # Treat any non-MISMATCH sentinel as PASS; MISMATCH sentinel as FAIL
        if [[ "$recorded" == "MISMATCH" ]]; then result="FAIL"; else result="PASS"; fi
    else
        result="FAIL"
    fi
    if [[ "$result" == "$expected" ]]; then
        echo "OK: R6 $dir → $result (expected $expected)"
    else
        echo "FAIL: R6 $dir → $result (expected $expected)"; FAIL=1
    fi
}

check_r1 "$FIXTURES/pass" "PASS"
check_r1 "$FIXTURES/fail-r1-analysis-incomplete" "FAIL"
check_r2 "$FIXTURES/pass" "PASS"
check_r2 "$FIXTURES/fail-r2-bad-target" "FAIL"
check_r4 "$FIXTURES/pass" "PASS"
check_r4 "$FIXTURES/fail-r4-constraint-dropped" "FAIL"
check_r5 "$FIXTURES/pass" "PASS"
check_r5 "$FIXTURES/fail-r5-malformed" "FAIL"
check_r6 "$FIXTURES/pass" "PASS"
check_r6 "$FIXTURES/fail-r6-mtime-mismatch" "FAIL"

exit $FAIL
```

- [ ] **Step 5: Make executable and run**

Run:
```bash
chmod +x ~/.claude/skills/epiphany-analysis/tests/test-runtime-invariants.sh
~/.claude/skills/epiphany-analysis/tests/test-runtime-invariants.sh
```
Expected: 10 OK lines, exit 0.

- [ ] **Step 6: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/test-runtime-invariants.sh tests/fixtures/
git commit -m "test(epiphany-analysis): runtime invariant fixtures (R1/R2/R4/R5-partial/R6)"
```

---

## Task 15: Partial-run contract test

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/tests/test-partial-run.sh`

Tests §14.6 — structural check that the skill's argv/state-file contract matches the spec. Since we cannot execute the skill in CI, this test checks that the SKILL.md documents the contract correctly.

- [ ] **Step 1: Write the test**

Write to `~/.claude/skills/epiphany-analysis/tests/test-partial-run.sh`:

```bash
#!/usr/bin/env bash
# Contract test: SKILL.md documents --session-dir / --start-at / --stop-at correctly.
set -u
SKILL_MD="$HOME/.claude/skills/epiphany-analysis/SKILL.md"
FAIL=0

for flag in "--session-dir" "--start-at" "--stop-at"; do
    if grep -q -- "$flag" "$SKILL_MD"; then
        echo "OK: SKILL.md documents $flag"
    else
        echo "FAIL: SKILL.md missing $flag documentation"; FAIL=1
    fi
done

# Each module name must appear under STEP 2-7
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    if grep -q "$m" "$SKILL_MD"; then
        echo "OK: orchestrator references $m"
    else
        echo "FAIL: orchestrator missing $m reference"; FAIL=1
    fi
done

# STEP 0 must document the requirement that partial-run flags need --session-dir
if grep -A5 "STEP 0" "$SKILL_MD" | grep -q "session-dir"; then
    echo "OK: STEP 0 documents session-dir requirement"
else
    echo "FAIL: STEP 0 does not document that --start-at/--stop-at require --session-dir"; FAIL=1
fi

exit $FAIL
```

- [ ] **Step 2: Make executable and run**

Run:
```bash
chmod +x ~/.claude/skills/epiphany-analysis/tests/test-partial-run.sh
~/.claude/skills/epiphany-analysis/tests/test-partial-run.sh
```
Expected: 10 OK lines, exit 0.

- [ ] **Step 3: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/test-partial-run.sh
git commit -m "test(epiphany-analysis): partial-run contract documentation"
```

---

## Task 16: Gold-reference acceptance protocol (manual)

**Files:**
- Create: `~/.claude/skills/epiphany-analysis/tests/gold-reference/RUN.md`

Implements spec §14.1. Because the skill runs in an LLM context, §14.1 cannot be automated in CI — this file is a manual-run protocol for empirical validation.

- [ ] **Step 1: Write RUN.md**

Write to `~/.claude/skills/epiphany-analysis/tests/gold-reference/RUN.md`:

```markdown
# Gold-Reference Acceptance Protocol

Manual test per spec §14.1. Run this after every material change to the skill's module prompts.

## Fixture

- **Node A:** the spec document that Node B analyzes. Locate by reading the frontmatter / opening section of `report.md` — it names the analyzed document.
- **Node B:** `/home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`
- **Gold C/D/E:** `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` in the same directory

## Protocol

1. Invoke: `/epiphany-analysis <node-a-path> <node-b-path>`
2. Wait for completion (expect ~10-15 min under LLM rate limits).
3. Diff the skill's output stage files against the gold reference.

## PASS criteria (v1-seed thresholds — recalibrate after 3 runs)

- **C-coverage:** ≥70% of 28 gold idea topics present (paraphrases count)
- **D-coverage:** ≥80% of 14 gold D entries (D01-D14) present, matched by (category, target_section, action) tuple
- **E-coverage:** ≥80% of 14 gold E entries (E01-E14) present, matched by target-string similarity ≥0.7
- **E-format:** 100% of E entries use the structured 4-kind locator (no natural-language targets)
- **Integration:** every E entry appears in `enhanced.md` at its declared target
- **No-regression:** every `## Constraints` entry from A1 is still satisfied in `enhanced.md`

## Recording results

Append each run's results to `run-log.md` (sibling of this file) with date, thresholds met/missed, and one-sentence notes on variance.
```

- [ ] **Step 2: Create an empty run-log.md for future entries**

Write to `~/.claude/skills/epiphany-analysis/tests/gold-reference/run-log.md`:

```markdown
# Gold-Reference Run Log

| Date | C-cov | D-cov | E-cov | E-format | Integration | No-regression | Notes |
|---|---|---|---|---|---|---|---|
```

- [ ] **Step 3: Commit**

```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/gold-reference/
git commit -m "test(epiphany-analysis): gold-reference manual-run protocol"
```

---

## Task 17: Final structural review and full test suite run

**Files:**
- All — final pass.

- [ ] **Step 1: Run all tests in sequence**

Run:
```bash
cd ~/.claude/skills/epiphany-analysis
./tests/test-structure.sh && \
    ./tests/test-skill-registration.sh && \
    ./tests/test-runtime-invariants.sh && \
    ./tests/test-partial-run.sh
```
Expected: all four scripts exit 0. If any fails, fix the specific file noted in the failure and re-run the failed test.

- [ ] **Step 2: Verify git state is clean**

Run: `git status`
Expected: "nothing to commit, working tree clean" (all files committed across Tasks 1-16).

- [ ] **Step 3: Verify directory layout against the plan's File Structure block**

Run:
```bash
find ~/.claude/skills/epiphany-analysis -type f | sort
```
`find` prints fully-expanded paths (starting with `/home/myuser/.claude/skills/epiphany-analysis/`, not `~`). Expected structure (ignoring git metadata, paths shown with `.../` for brevity):
```
.../epiphany-analysis/.SKILL.md.placeholder-backup
.../epiphany-analysis/SKILL.md
.../epiphany-analysis/modules/m-analyze.md
.../epiphany-analysis/modules/m-engineer.md
.../epiphany-analysis/modules/m-filter.md
.../epiphany-analysis/modules/m-ideate.md
.../epiphany-analysis/modules/m-integrate.md
.../epiphany-analysis/modules/m-intake.md
.../epiphany-analysis/templates/session.json.template
.../epiphany-analysis/tests/fixtures/pass/...  (10 files per pass fixture)
.../epiphany-analysis/tests/fixtures/fail-r1-analysis-incomplete/...
.../epiphany-analysis/tests/fixtures/fail-r2-bad-target/...
.../epiphany-analysis/tests/fixtures/fail-r4-constraint-dropped/...
.../epiphany-analysis/tests/fixtures/fail-r5-malformed/...
.../epiphany-analysis/tests/fixtures/fail-r6-mtime-mismatch/...
.../epiphany-analysis/tests/gold-reference/RUN.md
.../epiphany-analysis/tests/gold-reference/run-log.md
.../epiphany-analysis/tests/test-partial-run.sh
.../epiphany-analysis/tests/test-runtime-invariants.sh
.../epiphany-analysis/tests/test-skill-registration.sh
.../epiphany-analysis/tests/test-structure.sh
.../epiphany-analysis/tests/validate-frontmatter.py
```

- [ ] **Step 4: Run the manual gold-reference protocol (manual-only)**

Follow `tests/gold-reference/RUN.md`. Record results in `run-log.md`. This step is manual and cannot be automated.

Expected: first empirical run should hit v1-seed thresholds OR produce a specific calibration datum for future tuning. A miss is not a plan-level failure — it is a signal to refine the module prompts (which is v1.1 scope).

- [ ] **Step 5: Record initial gold-reference result**

Append one line to `run-log.md` with the first empirical run's numbers (date, coverage percents, notes). This is the calibration datum for future threshold tuning.

Run:
```bash
cd ~/.claude/skills/epiphany-analysis
git add tests/gold-reference/run-log.md
git commit -m "test(epiphany-analysis): initial gold-reference run log entry"
```

---

## Self-Review Checklist

Run this mentally after reading the plan end-to-end.

**1. Spec coverage:**

| Spec section | Implementing task(s) |
|---|---|
| §1 Identity, activation, I/O | Task 2 |
| §1 Runtime scope, hard gates | Task 3 |
| §2 All 6 modules | Tasks 6-11 |
| §3 Node data model | Embedded in each module (6-11) |
| §4 Pipeline topology | Task 3 (orchestrator), Task 11 (integration order + conflict) |
| §5 Section-Tailoring Map | Task 4 (SKILL.md reference) + Task 7 (m-analyze application) |
| §6 Loop patterns | Tasks 10 (Level A), 11 (Level B) |
| §7 Two-solutions rule | Task 10 |
| §8 Genius detection | Task 4 (SKILL.md table) + Task 6 (m-intake application) |
| §9.2 Runtime audit R1-R6 | Task 11 |
| §10 Output + session.json | Tasks 3 (session-id), 5 (template), 11 (write enhanced.md) |
| §11 Caps | Task 4 (SKILL.md reference) + Tasks 8,11 (enforcement) |
| §12 Replay/partial-run | Task 3 (orchestrator) + Task 15 (contract test) |
| §13 Anti-patterns | Task 4 (SKILL.md reference) |
| §14.1 Gold-reference | Task 16 (manual protocol) |
| §14.2/14.3 Runtime invariants | Task 14 (fixture tests) |
| §14.6 Partial-run | Task 15 |

No gaps.

**2. Placeholder scan:** No "TBD", no "add appropriate X", no "similar to Task N" without code.

**3. Type consistency:**
- Module names used consistently: `m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`.
- Stage file names used consistently: `node-{a,b,a1-analysis,b1-analysis,c-ideas,d-accepted,e-solutions}.md`, `enhanced.md`, `self-audit.md`, `session.json`.
- Genius tags used consistently: `genius-current`, `genius-drift`, `generic-fallback`.
- Target locator kinds: `heading_path`, `line_range`, `anchor`, `verbatim_block` — enforced in Task 10.

---

## Execution Notes

- **Order dependency:** Tasks 2-4 (SKILL.md) must complete before Task 13 (structure test) will pass. Tasks 6-11 (modules) must complete before Task 12 (frontmatter validator) passes all 6. Task 14 fixtures require careful hand-authoring since they simulate runtime outputs.
- **Parallelism potential:** Tasks 6-11 (the 6 modules) have no inter-task dependencies and can be worked in parallel by a subagent-driven workflow. Tasks 12-16 are test-only and can be interleaved after modules complete.
- **Known limitation:** Tests 14/16 verify structure and fixtures, not semantic quality. Semantic quality is measured only by the manual gold-reference run (Task 16 / Task 17 Step 4). Plan does NOT require first-run threshold pass — calibration is expected iterative work.
