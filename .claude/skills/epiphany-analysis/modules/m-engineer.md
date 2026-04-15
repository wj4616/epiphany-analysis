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

# M-ENGINEER — Two-Drafts-Pick-Best

## A-context excerpt extraction

For each D_i, the excerpt passed to the solution drafter is the Node A region identified by D_i's `target_section`:

- If `target_section` is a section heading: excerpt = heading + content up to the next heading of equal or higher level.
- If `target_section` is a line range: excerpt = those lines ± 10 lines of surrounding context.
- If `target_section` is a natural-language locator: excerpt = paragraph containing best substring match + 2 surrounding paragraphs.
- If no match: pass full Node A with a log warning in session.json.stop_reasons.

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

## Node E `target` locator (exactly ONE of 4 shapes)

- `{"kind": "heading_path", "path": ["## §4", "### Branching points"]}`
- `{"kind": "line_range", "start": 123, "end": 145}` (only when Node A is a stable file)
- `{"kind": "anchor", "before": "<exact substring>", "after": "<exact substring>"}`
- `{"kind": "verbatim_block", "text": "<multi-line exact substring>"}` (for `replace` actions only)

Natural-language locators FORBIDDEN. Drafter must resolve to one of the 4 or fail verify.

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

## Progress reporting

Emit one status line per D_i:
- `[m-engineer D_i N/<total>] drafting solution 1... | drafting solution 2... | compare+pick | stored=E0K`
- Dropped: `[m-engineer D_i N dropped] reason=double-draft-verify-failed`

## Session.json updates

Write `d_entries_processed`, `e_entries_produced`, and `double_fail_count` to `session.json.modules.m-engineer`.

## Return

"m-engineer complete. D entries processed: {N}. E entries produced: {M}. Double-failures: {K} (dropped, see ## Dropped section)."
