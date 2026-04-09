# Epiphany Omnipotent — Output, Persistence, and Handoff Spec

- **Date:** 2026-04-09
- **Status:** Design complete, ready for implementation
- **Supersedes:** `2026-04-08-epiphany-omnipotent-output-design.md` (distillation design — cancelled)
- **Target:** `~/.claude/skills/epiphany-omnipotent/SKILL.md` v1.5.0 (output, file save, and downstream handoff sections)
- **Forward dependency:** `~/.claude/skills/epiphany-analysis/SKILL.md` (placeholder created — see §6)

---

## 1. Scope

This spec defines what happens **after** the epiphany-omnipotent pipeline completes. It covers:

1. What gets emitted (full `<omnipotent_output_v1>` — schema unchanged from SKILL.md v1.4.3)
2. Where it gets saved on disk (mandatory, automatic)
3. Filename convention
4. Chunked write protocol
5. Handoff prompt to `epiphany-analysis`
6. Required SKILL.md changes (v1.4.3 → v1.5.0)
7. `epiphany-analysis` forward dependency and input contract

**This spec does NOT:**
- Redefine the XML schema — canonical source remains SKILL.md v1.4.3
- Add distillation, summarization, or insight extraction
- Add intent detection or deliverable routing
- Add output-routing flags — `--brainstorm`, `--plan`, `--skip-clarification`, `--auto-save`, `--no-save` are removed

The 5-lens pipeline (all stages, injections, gates, verification) is **unchanged**.

---

## 2. Output

The pipeline produces `<omnipotent_output_v1>` as defined in SKILL.md v1.4.3. This block is the sole output artifact. It is **not emitted in-conversation** — it goes directly to disk.

The skill emits no in-conversation content during or after the pipeline except:
- The one-line save confirmation
- The handoff prompt
- Error messages on save failure

---

## 3. Disk Persistence

### Save Path

```
~/epiphany/omnipotent/<filename>.xml
```

**Directory creation:** if `~/epiphany/omnipotent/` does not exist, create it (and `~/epiphany/` if missing) before writing. Use default user permissions.

### Filename Convention

```
<slug>-<YYYYMMDD>-<HHMM>UTC.xml
```

**Slug derivation:**

| Input type | Source for slug |
|------------|----------------|
| Raw text | First non-empty line of input |
| prompt-epiphany | First non-empty text content inside `<task>` |
| epiphany-context | First non-empty text content inside `<problem_statement>` |

Take the first 6 words of the source text, lowercase, strip non-alphanumeric characters, join with hyphens.

**Fallback:** if the slug would be empty after stripping, use `omnipotent-output`.

**Timestamp:** UTC at time of write.

**Collision handling:** append `-2`, `-3`, etc. before the extension until the filename is unique.

**Examples:**
```
redesign-auth-middleware-for-compliance-20260409-1430UTC.xml
omnipotent-output-20260409-1431UTC.xml          ← fallback slug
redesign-auth-middleware-for-compliance-20260409-1430UTC-2.xml  ← collision
```

### Chunked Write Protocol

The file MUST be written in sequential chunks. Do not write the entire XML in a single operation. Verify each chunk write completed without error before proceeding to the next chunk.

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

### Failure Behavior

**Pre-write failure** (directory creation failure, permission denied, file cannot be opened):
- Report error with reason
- Skip handoff prompt entirely — no file exists to analyze

**Chunk write failure** (any chunk fails after writing begins):
- Report error, naming the specific chunk/section that failed
- Leave partial file in place (do not delete)
- Report the partial file path
- Skip handoff prompt entirely — file is malformed

---

## 4. Handoff Prompt

**Only shown if all 13 chunks completed successfully.**

Print in this exact order:

```
Saved: ~/epiphany/omnipotent/<filename>.xml
⚠ Output is degraded — review XML before analysis.   ← only if status=degraded OR reasoning_status=shallow
Analyze this XML with /epiphany-analysis? (yes / no)
```

**Affirmative responses** (case-insensitive): `yes`, `y`, `yeah`, `sure`, `ok`

**All other responses:** treat as no, exit cleanly. The XML file is already saved.

**If yes:** check if epiphany-analysis is available as a skill. If available, invoke `/epiphany-analysis <filepath>` with the saved file path. If not available, show the not-installed message rather than attempting invocation:
```
epiphany-analysis is not installed. To analyze this XML, install the
epiphany-analysis skill and run: /epiphany-analysis <filepath>
```

---

## 5. Required SKILL.md Changes (v1.4.3 → v1.5.0)

| # | Location in SKILL.md | Change |
|---|---|---|
| 1 | File Save Behavior — Location | `~/prompts/omnipotent/` → `~/epiphany/omnipotent/` |
| 2 | File Save Behavior — Extension | `.md` → `.xml` |
| 3 | File Save Behavior — Save trigger | Remove opt-in prompt; save is mandatory and automatic after pipeline completes |
| 4 | File Save Behavior — Emission | Remove in-conversation XML emission; disk-only; update pipeline diagram terminal label to: `OUTPUT: Saved to ~/epiphany/omnipotent/<filename>.xml` |
| 5 | File Save Behavior — Write method | Replace single-write with chunked write protocol (this spec §3) |
| 6 | File Save Behavior — Slug derivation | Add structured-input slug rules: prompt-epiphany → from `<task>`; epiphany-context → from `<problem_statement>` (this spec §3) |
| 7 | Downstream Handoff — logic table | Replace 7-rule table with 2-rule table (this spec §5.1) |
| 8 | Downstream Handoff — recommended_next_skill | Closed set: `prompt-epiphany\|writing-plans\|none` → `epiphany-analysis\|none` |
| 9 | Downstream Handoff — matched_rule values | New values: priority 1 = `degraded_shallow_or_escape`; priority 2 = `epiphany_analysis_ready` |
| 10 | Downstream Handoff — closed-set prose note | Update to: "The `recommended_next_skill` value MUST be `epiphany-analysis` or `none`." |
| 11 | Trigger Conditions table | Remove rows: `--brainstorm`, `--plan`, `--skip-clarification`, `--auto-save`, `--no-save` |
| 12 | Post-save interaction | Add handoff prompt logic (this spec §4): save confirmation, degraded warning, yes/no prompt, not-installed message |
| 13 | Integration Notes | Remove "With `writing-plans`" and "With `prompt-epiphany`" entries; add: "With `epiphany-analysis`: XML saved to `~/epiphany/omnipotent/` is the primary input. Does not call epiphany-analysis at runtime." |
| 14 | Version | Bump `1.4.3` → `1.5.0` |

### 5.1 New Downstream Handoff Logic

Replaces the existing 7-rule priority table.

| Priority | Condition | `recommended_next_skill` | `matched_rule` |
|----------|-----------|--------------------------|----------------|
| 1 | `status=degraded` OR any escape_hatch present OR `reasoning_status=shallow` | `none` | `degraded_shallow_or_escape` |
| 2 | All other cases | `epiphany-analysis` | `epiphany_analysis_ready` |

---

## 6. epiphany-analysis Forward Dependency

`epiphany-analysis` does not yet exist. A placeholder skill has been created at:

```
~/.claude/skills/epiphany-analysis/SKILL.md
```

### Input Contract

| Field | Value |
|-------|-------|
| Input | File path to a readable `.xml` file at `~/epiphany/omnipotent/` |
| Schema | `<omnipotent_output_v1>` as defined in SKILL.md v1.4.3+ |
| Degraded inputs | Valid — `status=degraded`, `pg2_status=contaminated`, `reasoning_status=shallow` must be handled gracefully, not errored on |

### Fields Carrying Previously-Lost Mid-Pipeline Reasoning

The cancelled distillation design dropped these fields from the emitted output. They are now fully preserved in the XML and are the primary analysis targets for epiphany-analysis.

| Field | Content | Status in old design |
|-------|---------|---------------------|
| `<lens_outputs>` — all 5 lenses | Raw SCAMPER ideas, Lateral moves, Morphological box, Six Hats perspectives, TRIZ contradictions, Reverse Brainstorming failure modes | **Previously lost** |
| `<synthesis>.<disagreement>` | Lens conflicts, value tensions, steel-manned positions per lens | **Previously lost** |
| `<synthesis>.<dialectical_options>` | Synthesis-generated alternatives integrating competing value positions | **Previously lost** |
| `<synthesis>.<gaps_from_frame>` | Unresolved gaps from frame coverage check | **Previously lost** |
| `<process_notes>.<reasoning_self_assessment>` | Runner's honest self-critique against 8 reasoning criteria | **Previously lost** |
| `<decision>.<matrix>` | Per-cell Pugh Matrix scores and rationale | **Previously lost** |
| `<framing_context>` | Deep structure principle, known/unknown/assumed, naive questions, observation vs recognition | **Partially lost** |
