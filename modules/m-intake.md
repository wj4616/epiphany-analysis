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

# M-INTAKE — Intake and Detection

## Purpose

Load Node A and Node B. Detect Node A source kind. Detect Node B type per the Genius Detection table in SKILL.md §8. Stage both to `{session_dir}`. Seed `session.json`.

## Hard Gate HG-1

Before any write, verify the target path for `enhanced.md` (= `{session_dir}/enhanced.md`) does NOT equal the Node A source path. If equal, abort with error:
> "HG-1 violation: Node A source path equals enhanced.md target. The skill never overwrites Node A."

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

## Verbatim preservation

The stage files in `{session_dir}/` are byte-for-byte copies of the source. Do NOT:
- normalize whitespace or line endings
- strip or add BOMs
- collapse blank lines
- inject headers, footers, or provenance markers

**Invariant.** After step 3, `sha256(stage_file_content) == sha256(source_file_content)` for both Node A and Node B. If a source is inline (Node A only), the stage file hash is recorded against the captured inline text. Any mismatch is an m-intake error.

## Return

"m-intake complete. Wrote: node-a.md, node-b.md. Detected: node_a.source_kind=<inline|file>, node_b.genius_detection=<genius-current|genius-drift|generic-fallback>."
