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

# M-INTEGRATE — Flush Integration + Self-Audit

## Precondition gate (before integration starts)

Abort with error if any of:
- `node-a1-analysis.md` or `node-b1-analysis.md` does not exist or is empty
- `node-e-solutions.md` does not exist (empty-but-present OK — log reason to session.json.stop_reasons)
- `session.json.modules.m-analyze.a1_complete != true` OR `.b1_complete != true`

Verbatim precondition clause: *"analysis verified for accuracy and completeness"*.

## Integration order

Apply Node E entries in this precedence:

1. Entries whose `from_d_id`'s `from_idea_id` originated in the Headline Insight section — applied first. For merged D entries where `from_idea_id` is a list, qualifies if **any** constituent traces to Headline Insight.
2. Remaining entries in stable order (as they appear in `node-e-solutions.md`).

## Conflict resolution

Two Node E entries conflict when their `target` regions overlap by ≥1 line of Node A. On detection:

1. If both `from_d_id`'s trace to the same `target_section`: attempt **compose** — synthesized patch applying both in sequence. Verify the composed patch still preserves info. If compose succeeds, use it.
2. Otherwise: apply the entry with higher utility (from Node D). On tie: lower `from_idea_id` wins (stable).
3. Losing entry is dropped; record in `self-audit.md` under `## Integration conflicts`.

## Progress reporting

Emit `[m-integrate] applying E_K/<total>: <brief-target>` per integration step, then `[m-integrate] self-audit running...` before the audit runs.

## Runtime audit (before finalizing output)

- **R1.** *"analysis verified for accuracy and completeness"* — session.json.modules.m-analyze.a1_complete && .b1_complete.
- **R2.** Every E entry appears at its `target` location (dispatch by `target.kind`).
- **R3.** No Node A content removed without a matching Node E `replace` (shorter text) with rationale citing the removed content.
- **R4.** Enhanced A still contains every constraint listed in A1's `## Constraints` — substring or semantic paraphrase match required.
- **R5.** No newly-introduced defects (unclosed fences/tags, duplicated paragraphs, TODO/TBD tokens) that weren't already in Node A or introduced via a Node E entry.
- **R6.** Node A source file mtime + hash unchanged (compared against session.json.node_a.content_hash).

## FAIL handling

- **1st FAIL:** log findings to `self-audit.md`. Run one refinement cycle (§6 Level B) targeting only failed checks. Re-verify.
- **2nd FAIL:** write `enhanced-partial.md` + `open-questions.md` with specific R1-R6 failure IDs. Halt. Do NOT write `enhanced.md`. Set session.json.modules.m-integrate.audit_verdict = "PARTIAL".
- **PASS:** write `enhanced.md`. Set audit_verdict = "PASS".

Always write `self-audit.md` with the per-check verdict record.

## Session.json updates

Write `refinement_cycles` and `audit_verdict` to `session.json.modules.m-integrate`.

## Return

"m-integrate complete. Verdict: {PASS|FAIL|PARTIAL}. Refinement cycles: {0|1}. Output: {enhanced.md|enhanced-partial.md}."
