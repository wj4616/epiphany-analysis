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

## Merge consolidation (one-time sub-pass)

After the verdict pass, scan accepted entries for merge candidates. Two entries merge when:
(a) both target the same `target_section`, AND
(b) one entry's idea is subsumed by or reinforces the other.

Merge output:
- `from_idea_id` becomes a list (e.g., `[I05, I07]`)
- `verdict` = stronger of the two (ACCEPT > ACCEPT-CONDITIONAL)
- `gain` = concatenation of both gain strings separated by ` + `, deduped on exact-substring match

REJECT entries never merge. This sub-pass runs exactly once — no iteration.

## Progress reporting

Emit one status line after each batch of 10 reviewed ideas, and a final summary:

- During verdict pass: `[m-filter] reviewed={N_so_far}/{N_total} | accepted={K} | rejected={N_so_far-K}`
- After merge consolidation: `[m-filter] merged={M_merged_pairs} | final_accepted={final_D_count}`
- Final summary: `[m-filter] complete | ideas_in={N_total} | accepted_out={final_D_count}`

Write `ideas_in` and `accepted_out` to `session.json.modules.m-filter`.

## Return

"m-filter complete. Ideas in: {N}. Accepted: {M}. Rejected: {N-M}. Merged: {K}."
