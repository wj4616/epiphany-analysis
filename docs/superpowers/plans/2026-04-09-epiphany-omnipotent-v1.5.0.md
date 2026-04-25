# Epiphany Omnipotent v1.5.0 — Output Persistence and Handoff Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Update `epiphany-omnipotent` SKILL.md from v1.4.3 to v1.5.0 — replace opt-in save with mandatory chunked disk write to `~/epiphany/omnipotent/`, remove distillation-era output-routing flags, replace 7-rule downstream handoff with 2-rule `epiphany-analysis` handoff, and add a post-save interaction section.

**Architecture:** All changes are to a single file: `~/.claude/skills/epiphany-omnipotent/SKILL.md`. The 5-lens pipeline (all stages, injections, gates, verification) is untouched. The epiphany-analysis placeholder (`~/.claude/skills/epiphany-analysis/SKILL.md`) is already created — no changes needed there.

**Tech Stack:** Markdown SKILL.md prompt engineering. Edit tool for precise string replacement. Grep to locate and verify. Git for commits.

---

## Files

| Action | Path |
|--------|------|
| Modify | `~/.claude/skills/epiphany-omnipotent/SKILL.md` |
| Reference (no changes) | `~/.claude/skills/epiphany-analysis/SKILL.md` |
| Spec | `docs/superpowers/specs/2026-04-09-epiphany-omnipotent-output-design.md` |

---

### Task 1: Update File Save Behavior Section

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (File Save Behavior section, ~line 1439)

- [ ] **Step 1: Locate the current section**

```bash
grep -n "File Save Behavior" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected output: a line number around 1439 with `## File Save Behavior`

- [ ] **Step 2: Replace the entire File Save Behavior section**

Find and replace this exact block:

**OLD:**
```
## File Save Behavior

- **Location:** `~/prompts/omnipotent/<slug>.md`
- **Directory creation:** If `~/prompts/omnipotent/` does not exist, create it (and `~/prompts/` if missing) before writing. Create with default user permissions.
- **Slug derivation:** First 6 words of input's first non-empty line, lowercased, non-alphanumeric stripped, joined by hyphens. If slug would be empty (e.g., all-symbol input), use `omnipotent-output-<UTC-timestamp>`.
- **Collision handling:** Append `-2`, `-3`, etc. until unique
- **Offer timing:** AFTER emitting output, one-line prompt: "Save to `~/prompts/omnipotent/<slug>.md`?"
- **What is saved:** Full `<omnipotent_output_v1>` block, unchanged
- **On save failure:** Report the error inline, leave the in-conversation output untouched, do not retry silently.
```

**NEW:**
```
## File Save Behavior

Save the full `<omnipotent_output_v1>` block to disk. **Do not emit it in-conversation.** Save is mandatory and automatic — no user prompt before writing.

**Location:** `~/epiphany/omnipotent/<filename>.xml`

**Directory creation:** If `~/epiphany/omnipotent/` does not exist, create it (and `~/epiphany/` if missing) before writing. Use default user permissions.

**Filename convention:** `<slug>-<YYYYMMDD>-<HHMM>UTC.xml`

**Slug derivation:**

| Input type | Source for slug |
|------------|----------------|
| Raw text | First non-empty line of input |
| prompt-epiphany | First non-empty text content inside `<task>` |
| epiphany-context | First non-empty text content inside `<problem_statement>` |

Take the first 6 words of the source text, lowercase, strip non-alphanumeric characters, join with hyphens. If the slug would be empty after stripping, use `omnipotent-output`.

**Timestamp:** UTC at time of write.

**Collision handling:** Append `-2`, `-3`, etc. before the extension until the filename is unique.

**Chunked write protocol:** Write the file in sequential chunks. Do not write the entire XML in a single operation. Verify each chunk write completed without error before proceeding to the next chunk.

| Chunk | Content |
|-------|---------|
| 1 | XML declaration + `<omnipotent_output_v1>` root opening tag + `<meta>` block |
| 2 | `<framing_context>` |
| 3 | `<lens_outputs>` opening tag + `<lens name="divergent_ideation">` block |
| 4 | `<lens name="systematic_completeness">` block |
| 5 | `<lens name="multi_perspective_critique">` block |
| 6 | `<lens name="contradiction_resolution">` block |
| 7 | `<lens name="deep_risk_exploration">` block + `</lens_outputs>` closing tag |
| 8 | `<synthesis>` |
| 9 | `<decision>` |
| 10 | `<inventory>` |
| 11 | `<verification_report>` |
| 12 | `<process_notes>` |
| 13 | `<downstream_handoff>` + `</omnipotent_output_v1>` root closing tag |

**Pre-write failure** (directory creation failure, permission denied, file cannot be opened):
- Report error with reason
- Skip post-save interaction entirely — no file exists to analyze

**Chunk write failure** (any chunk fails after writing begins):
- Report error, naming the specific chunk/section that failed
- Leave partial file in place (do not delete)
- Report the partial file path
- Skip post-save interaction entirely — file is malformed
```

- [ ] **Step 3: Verify the replacement**

```bash
grep -n "epiphany/omnipotent" ~/.claude/skills/epiphany-omnipotent/SKILL.md | head -5
grep -n "prompts/omnipotent" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: first command shows 2+ matches in File Save Behavior. Second command returns nothing (old path gone).

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): update File Save Behavior to chunked mandatory write at ~/epiphany/omnipotent/"
```

---

### Task 2: Update Pipeline Diagram Output Label

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (pipeline diagram, ~line 355)

- [ ] **Step 1: Locate the output label in the diagram**

```bash
grep -n "OUTPUT: <omnipotent_output_v1>" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: one match around line 355.

- [ ] **Step 2: Replace the label**

**OLD:**
```
│  OUTPUT: <omnipotent_output_v1>                                        │
```

**NEW:**
```
│  OUTPUT: Saved to ~/epiphany/omnipotent/<filename>.xml                     │
```

> **Note:** The original line 355 is 78 chars (2 chars short of the 80-char box). The new line above is 78 chars to match. Verify alignment visually after editing — if nearby box lines are 80 chars, add 2 more spaces before the closing `│`.

- [ ] **Step 3: Verify**

```bash
grep -n "OUTPUT:" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: one match showing the new save path.

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): update pipeline diagram to show disk-only output"
```

---

### Task 3: Update Downstream Handoff Logic

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (Downstream Handoff section, ~lines 1385–1406)

This task has three sub-edits: (a) the XML schema element, (b) the logic table, (c) the closed-set prose note.

- [ ] **Step 1: Locate the downstream handoff schema**

```bash
grep -n "recommended_next_skill" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: two matches — one in the XML schema block, one in the prose note.

- [ ] **Step 2: Replace the XML schema element and matched_rule priority range**

**OLD:**
```
<downstream_handoff>
  <recommended_next_skill>prompt-epiphany|writing-plans|none</recommended_next_skill>
  <matched_rule priority="1|2|3|4|5|6|7">brief rule name</matched_rule>
  <usage_hint>One sentence on how to use this output</usage_hint>
</downstream_handoff>
```

**NEW:**
```
<downstream_handoff>
  <recommended_next_skill>epiphany-analysis|none</recommended_next_skill>
  <matched_rule priority="1|2">brief rule name</matched_rule>
  <usage_hint>One sentence on how to use this output</usage_hint>
</downstream_handoff>
```

- [ ] **Step 3: Replace the matched_rule prose and logic table**

**OLD:**
```
The `<matched_rule>` element lets downstream consumers branch on which rule fired without parsing `<usage_hint>` natural language. Its `priority` attribute matches the rule numbers in the table below; its text content is the short rule name (e.g., "degraded_or_escape", "shallow_reasoning", "unresolved_gap", "prompt_epiphany_complete", "epiphany_context_complete", "complete_no_decision", "raw_complete"). The order in this list matches the priority order below — first match wins.

**Handoff logic — first matching rule wins (top to bottom):**

| Priority | Condition | Recommendation |
|----------|-----------|----------------|
| 1 | `<status>degraded</status>` OR any escape_hatch present | `none` — needs human review before any downstream skill |
| 2 | `<reasoning_status>shallow</reasoning_status>` | `none` — runner self-flagged the output as cognitively shallow; re-run with deeper engagement, do not auto-pipeline |
| 3 | `<gaps_from_frame>` contains any gap with `status="unresolved"` | `prompt-epiphany` — enhance the framing and iterate |
| 4 | `input_pre_processing="prompt-epiphany"` AND status complete AND reasoning_status sound | `writing-plans` — input was already pre-enhanced; proceed to implementation |
| 5 | `input_pre_processing="epiphany-context"` AND status complete AND reasoning_status sound | `writing-plans` — context was pre-gathered; proceed to implementation |
| 6 | status complete but no actionable decision (e.g. exploratory only — `<decision_quality>` is `insufficient_alternatives` or `degraded`, OR `<decision>` is empty) | `prompt-epiphany` — refine into an actionable prompt |
| 7 | status complete AND reasoning_status sound (raw text input) AND an actionable `<decision>` exists | `writing-plans` — proceed to implementation |

The `<recommended_next_skill>` value MUST be `prompt-epiphany`, `writing-plans`, or `none` (closed set). The `<matched_rule>` MUST be present and its `priority` attribute MUST match the rule number that fired. The `<usage_hint>` is human-facing prose; programmatic consumers should branch on `<matched_rule>` instead.
```

**NEW:**
```
The `<matched_rule>` element lets downstream consumers branch on which rule fired without parsing `<usage_hint>` natural language. Its `priority` attribute matches the rule numbers in the table below; its text content is the short rule name. The order in this list matches the priority order below — first match wins.

**Handoff logic — first matching rule wins (top to bottom):**

| Priority | Condition | `recommended_next_skill` | `matched_rule` |
|----------|-----------|--------------------------|----------------|
| 1 | `status=degraded` OR any escape_hatch present OR `reasoning_status=shallow` | `none` | `degraded_shallow_or_escape` |
| 2 | All other cases | `epiphany-analysis` | `epiphany_analysis_ready` |

The `<recommended_next_skill>` value MUST be `epiphany-analysis` or `none` (closed set). The `<matched_rule>` MUST be present and its `priority` attribute MUST match the rule number that fired (1 or 2). The `<usage_hint>` is human-facing prose; programmatic consumers should branch on `<matched_rule>` instead.
```

- [ ] **Step 4: Verify**

```bash
grep -n "writing-plans\|prompt-epiphany" ~/.claude/skills/epiphany-omnipotent/SKILL.md | grep -v "##\|With\|Input\|input_pre\|input_format\|epiphany-context\|input can"
```

Expected: no matches. (The `-v` filter excludes: Integration Notes headers, `With \`...\`` entries, input processing lines including `<input_format>prompt-epiphany</input_format>` on ~line 165, and trigger condition lines.)

```bash
grep -n "epiphany_analysis_ready\|degraded_shallow_or_escape" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: two matches (both in the logic table, one per row — the closed-set prose note does not contain these rule name strings).

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): replace 7-rule handoff table with 2-rule epiphany-analysis handoff"
```

---

### Task 4: Add Post-Save Interaction Section

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (insert new section after File Save Behavior)

- [ ] **Step 1: Locate the insertion point**

```bash
grep -n "^## Integration Notes" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: one match. The new section goes immediately before Integration Notes (after the `---` separator that precedes it).

- [ ] **Step 2: Insert the Post-Save Interaction section**

> **Formatting note:** The OLD and NEW blocks below use 4-space indentation to avoid nested backtick rendering issues in this plan document. When you perform the actual file edit, the content you insert into SKILL.md is the indented text **without** the 4-space leading indent — it should be normal top-level markdown prose.

Find this exact text:

**OLD** (unique anchor — the Integration Notes heading and its first line):

    ## Integration Notes

    **With `epiphany-genius`:**

**NEW** (replace the above with this full block):

    ## Post-Save Interaction

    Shown only if all 13 chunks completed successfully. Print in this exact order:

        Saved: ~/epiphany/omnipotent/<filename>.xml
        ⚠ Output is degraded — review XML before analysis.   ← only if status=degraded OR reasoning_status=shallow
        Analyze this XML with /epiphany-analysis? (yes / no)

    **Affirmative responses** (case-insensitive): `yes`, `y`, `yeah`, `sure`, `ok`

    **All other responses:** treat as no, exit cleanly. The XML file is already saved.

    **If yes:** check if epiphany-analysis is available as a skill. If available, invoke `/epiphany-analysis <filepath>` with the saved file path. If not available, show this message instead of attempting invocation:

        epiphany-analysis is not installed. To analyze this XML, install the
        epiphany-analysis skill and run: /epiphany-analysis <filepath>

    ---

    ## Integration Notes

    **With `epiphany-genius`:**

- [ ] **Step 3: Verify**

```bash
grep -n "Post-Save Interaction\|Affirmative responses\|epiphany-analysis is not installed" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: three matches confirming the new section is present.

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): add Post-Save Interaction section with epiphany-analysis handoff prompt"
```

---

### Task 5: Update Integration Notes

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (Integration Notes section, ~line 1451)

- [ ] **Step 1: Locate the section**

```bash
grep -n "With .writing-plans\|With .prompt-epiphany" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: two matches in the Integration Notes section.

- [ ] **Step 2: Replace the prompt-epiphany and writing-plans entries**

**OLD:**
```
**With `prompt-epiphany`:** Output can be fed directly to prompt-epiphany for enhancement. Input can be prompt-epiphany output. The XML structures are compatible.

**With `writing-plans`:** Output's `<decision>` can inform implementation planning. Recommend as downstream skill when output is complete.
```

**NEW:**
```
**With `prompt-epiphany`:** Input can be prompt-epiphany output; the XML structures are compatible for this direction.

**With `epiphany-analysis`:** XML saved to `~/epiphany/omnipotent/` is the primary input for epiphany-analysis. Does not call epiphany-analysis at runtime.
```

- [ ] **Step 3: Verify**

```bash
grep -n "With .epiphany-analysis\|With .writing-plans\|Output can be fed" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: one match for `epiphany-analysis`, zero for `writing-plans`, zero for `Output can be fed`.

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): update Integration Notes — remove writing-plans, add epiphany-analysis"
```

---

### Task 6: Version Bump and Document Status Entry

**Files:**
- Modify: `~/.claude/skills/epiphany-omnipotent/SKILL.md` (frontmatter + Document Status section)

- [ ] **Step 1: Update frontmatter version and date**

**OLD:**
```
version: 1.4.3
last_modified: 2026-04-08
```

**NEW:**
```
version: 1.5.0
last_modified: 2026-04-09
```

- [ ] **Step 2: Add v1.5.0 entry to Document Status**

Find this exact text:

**OLD:**
```
## Document Status

Version 1.4.3 (2026-04-08): Second project-audit pass
```

> **Why this anchor:** The full line is `Version 1.4.3 (2026-04-08): Second project-audit pass — 24 findings...` (text continues after the colon). The old_string must include enough of that line to match uniquely. Do NOT use `Version 1.4.3 (2026-04-08):\n` — it won't match.

**NEW:**
```
## Document Status

Version 1.5.0 (2026-04-09): Output persistence and handoff redesign — XML-only output, mandatory chunked disk write, epiphany-analysis handoff. No pipeline changes.
- **Output goes disk-only.** `<omnipotent_output_v1>` is no longer emitted in-conversation; it is written to `~/epiphany/omnipotent/<filename>.xml` automatically after every run.
- **Chunked write protocol.** File written in 13 sequential chunks (one per top-level XML section, with `<lens_outputs>` split one lens per chunk) to prevent write failures on large outputs. Each chunk verified before proceeding.
- **Mandatory save.** The opt-in "Save to file?" prompt is removed. Save always happens.
- **Downstream handoff simplified.** 7-rule table replaced with 2-rule table: degraded/shallow/escape → `none`; all other cases → `epiphany-analysis`. `writing-plans` and `prompt-epiphany` removed from recommendation set.
- **Post-Save Interaction section added.** Defines save confirmation, degraded warning, and epiphany-analysis handoff prompt with yes/no behavior and not-installed fallback.
- **Integration Notes updated.** `writing-plans` entry removed. `prompt-epiphany` entry scoped to input direction only. `epiphany-analysis` entry added.
- **epiphany-analysis placeholder created** at `~/.claude/skills/epiphany-analysis/SKILL.md` as a forward dependency with a defined input contract.

Version 1.4.3 (2026-04-08): Second project-audit pass
```

- [ ] **Step 3: Verify**

```bash
grep -n "version:\|last_modified:\|Version 1.5.0\|Version 1.4.3" ~/.claude/skills/epiphany-omnipotent/SKILL.md | head -6
```

Expected: frontmatter shows `1.5.0` / `2026-04-09`; Document Status shows both `Version 1.5.0` and `Version 1.4.3`.

- [ ] **Step 4: Final sanity check — confirm no old save path or old flag rows remain**

```bash
grep -n "prompts/omnipotent\|--brainstorm\|--plan\|--auto-save\|--no-save\|--skip-clarification" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: zero matches. (These were never in the SKILL.md trigger table — this confirms they were not accidentally added.)

```bash
grep -n "writing-plans\|prompt-epiphany" ~/.claude/skills/epiphany-omnipotent/SKILL.md
```

Expected: matches only in these valid locations:
- Integration Notes (`With 'prompt-epiphany': Input can be prompt-epiphany output...`)
- Input Processing section (prompt-epiphany as an input format)
- process_notes schema (`input_pre_processing="prompt-epiphany"` as an enum value)
- Trigger Conditions (recognising prompt-epiphany input format)

Zero matches in the Downstream Handoff section.

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/epiphany-omnipotent/SKILL.md
git commit -m "feat(omnipotent): bump version to 1.5.0, add Document Status entry"
```
