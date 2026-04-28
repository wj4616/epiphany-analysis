# Prompt Epiphany v3.0 — Verbosity Modes Design

**Date:** 2026-04-04
**Status:** Draft
**Files:** `~/.claude/skills/prompt-epiphany/`
**Supersedes:** `docs/superpowers/specs/2026-04-04-prompt-epiphany-verbosity-design.md`

---

## Overview

Add two verbosity modes (minimal, verbose) to the prompt-epiphany skill. The existing normal-mode pipeline is unchanged — it works well. Minimal is a lighter, cheaper pipeline for token-efficient enhancement. Verbose is an additional expansion pass after normal, targeting thin spots in the normal output.

---

## Goals

1. Minimal mode reduces pipeline token cost by skipping creative multi-step processing that won't show in compact output
2. Verbose mode expands normal output with a second pass of ideation, synthesis, and verification targeting gaps
3. Normal mode is completely unchanged
4. Zero information loss guarantee preserved across all modes
5. Simple flag-based detection — no keyword heuristics

---

## Architecture: Additive Layers

Modes are layered on the existing pipeline, not branched within it.

```
ALL MODES:
  Step 1: Gather + Mode Detection
  Step 2: Sufficiency Check
          ↓
  ┌───────────────────────────────────────────┐
  │                                           │
  MINIMAL PATH          NORMAL PATH           VERBOSE PATH
  Step 3m: Quick        Step 3: Full          Step 3-6: Full
    Analysis              Analysis              Normal Pipeline
  Step 4m: Direct       Step 4: Ideation        ↓ (intermediate result)
    Synthesis           Step 5: Synthesis     Step 7v: Gap Scan
  Step 5m: Lite         Step 6: Full          Step 8v: Expansion Ideation
    Verification          Verification        Step 9v: Expansion Synthesis
      ↓                     ↓                 Step 10v: Expansion Verification
      OUTPUT                OUTPUT                ↓
                                                  OUTPUT
```

Only one path executes per invocation.

---

## Mode Detection

**Timing:** Step 1 (Gather), before pipeline begins.

**Detection rule:** Flags are recognized when they appear as the first or last standalone token of the input (e.g., `/prompt-epiphany --verbose <prompt>` or `/prompt-epiphany <prompt> --verbose`). Flags appearing mid-sentence within the prompt body are treated as content, not mode selectors. This prevents violating zero information loss if the user's prompt legitimately contains `--verbose` or `--minimal` as content (e.g., "write CLI help text documenting the --verbose flag").

| Invocation | Mode |
|------------|------|
| `/prompt-epiphany <prompt>` | Normal (default) |
| `/prompt-epiphany --minimal <prompt>` | Minimal |
| `/prompt-epiphany <prompt> --minimal` | Minimal |
| `/prompt-epiphany --verbose <prompt>` | Verbose |
| `/prompt-epiphany <prompt> --verbose` | Verbose |
| Both flags present | Ask user to pick one |

**No keyword heuristics.** Flags only. Simple, unambiguous, zero false positives.

**Flag stripping:** Remove the detected flag from its detected position (first or last token) before processing. Never strip flags from within the prompt body.

**Announcement:**
- Minimal: "I'm using the prompt-epiphany skill (minimal mode) to enhance this prompt."
- Normal: "I'm using the prompt-epiphany skill to analyze and enhance this prompt." (unchanged)
- Verbose: "I'm using the prompt-epiphany skill (verbose mode) to analyze, enhance, and expand this prompt."

---

## What Stays Unchanged

All of the following are identical to v2.1.0:

- Hard Gates (Sufficiency, Zero Information Loss, Prompt Content Only)
- Anti-Patterns
- Normal pipeline Steps 1-6 (entire core)
- Enhancement Techniques Reference (T1-T13)
- Semantic Output Format
- Quality Standard
- Trigger Conditions (modes extend, not replace)

---

## Minimal Mode — Fast Track

**Rationale:** Compact output doesn't justify the full creative multi-step pipeline. If the result will be minimal, the process should be too — save tokens where the depth won't show in the result.

Replaces Steps 3-6 with a streamlined 3-step process.

### Step 3m: Quick Analysis

Two dimensions only:

- **Intent** — What is the prompt trying to accomplish? Success criteria?
- **Inventory** — Catalog every item to preserve exactly: code blocks, formulas, API references, named entities, numeric values.

Skip Structure, Constraints, Techniques, Weaknesses. These feed the creative ideation process which minimal bypasses. (See Step 4m for how this affects T3.)

### Step 4m: Direct Synthesis

Single-pass technique application — no creative exploration, no ideation step.

**Eligible techniques:** T1 (XML structure), T2 (Decomposition), T3 (Constraints — structure already-explicit constraints into bullets, not discovery of implicit ones), T5 (Output format), T7 (Priority). Apply only those the Quick Analysis identifies as needed. This is a ceiling, not a checklist — if the input already has XML structure, don't reapply T1.

**Skipped by default:** T4 (persona), T6 (reasoning), T8 (edge cases), T9 (examples), T10 (self-critique), T11 (context anchoring), T12 (audience), T13 (escape hatch). These are the creative/depth techniques that minimal intentionally foregoes.

**Output rules:**
- Bullet-point constraints (no narrative framing)
- Remove filler, tighten language
- Preserve ALL Inventory items exactly
- **Proportionality:** Aim for the most compact enhancement that preserves all content and adds necessary structure. Output may be shorter than, equal to, or moderately longer than input depending on what's needed. No fixed ratio — let the content dictate the length.

### Step 5m: Lite Verification

Three checks:

- **Element Completeness** — Every Inventory item exists in output. Missing → FAIL.
- **Semantic Fidelity** — Intent matches enhanced prompt. Same objective, same success criteria. Any "no" → FAIL.
- **Technical Integrity** — Code, formulas, API refs content-identical. Any alteration → FAIL.

Fail → fix in Step 4m, re-verify. Same check fails twice → output with note: "Verification check [name] could not be fully resolved — review flagged area."

---

## Verbose Mode — Expansion Pass

**Rationale:** Some prompts benefit from richer enhancement than normal mode provides. Rather than changing the normal pipeline (which works well), verbose adds a second pass that identifies where the normal output is thin and expands there specifically.

Normal pipeline runs completely first (Steps 1-6), producing an intermediate enhanced prompt (internal — not shown to user). Then a second pass targets thin spots.

### Step 7v: Gap Scan

Read the normal-mode output and identify where it's thin. Evaluate each gap category against the Intent extracted during the normal pipeline's Step 3a analysis — skip categories that don't apply to this prompt type. Not every gap category is relevant to every prompt.

**Gap categories:**
- **Sparse context** — background knowledge that would help the AI but wasn't added
- **Bare constraints** — constraints without "why" explanations
- **Missing edge cases** — boundary conditions not addressed
- **No examples** — task that would benefit from few-shot exemplars but has none
- **Weak reasoning guidance** — multi-step task without CoT structure
- **Missing audience calibration** — no target reader specified

**Thinness threshold:** A section is thin if expanding it would meaningfully improve the prompt's effectiveness for its stated intent. Brevity alone is not thinness — a two-sentence context section is fine if those sentences are sufficient.

**If no thin spots found:** Return the normal output with a note — "Normal enhancement is already comprehensive. Returning standard version."

### Step 8v: Expansion Ideation

For each thin spot identified in the Gap Scan:

1. Design a targeted expansion (what to add, where to place it)
2. For each expansion, determine: is this information present in or directly derivable from the original prompt text?
3. If not — if you had to reason beyond the text to justify it — track it for flagging to user (see Output Flow)

Every expansion must pass:

| Test | Question | Fail → |
|------|----------|--------|
| Impact | Does this expansion improve the prompt's effectiveness? | Discard |
| Risk | Could it introduce inaccuracy or fabrication? | Discard |
| Validity | Faithful to original intent? | Discard |
| Necessity | Filling a real gap, not padding? | Discard |

### Step 9v: Expansion Synthesis

Apply expansions to the normal-mode output:

- Add "why" explanations to bare constraints
- Expand context section with background knowledge
- Add examples section with 1-2 exemplars (if Gap Scan identified this need)
- Add reasoning guidance for multi-step tasks
- Add edge cases for ambiguous boundaries
- Add audience calibration if missing

Expand within existing sections where possible. Add new sections only when the Gap Scan identifies a missing section type (e.g., `<examples>`, `<edge_cases>`). Do not reorganize or merge existing sections.

**Proportionality:** No arbitrary length target. Expand until all identified gaps are filled. If the expanded output is less than ~20% longer by word count than the normal output, note to user that the normal enhancement was already comprehensive.

### Step 10v: Expansion Verification

Three expansion-specific checks:

- **No Fabrication** — Every expansion traces to a Gap Scan finding and Ideation design. No invented requirements.
- **Rationale Accuracy** — For each "why" explanation, apply this three-tier test: (1) Derivable from the original prompt text → include, no flag. (2) Requires reasoning beyond the text but reasonably supportable → include, flag it for user review (see Output Flow). (3) Cannot be reasonably supported → omit rather than guess.
- **Value Added** — Each expansion genuinely improves the prompt. Remove any that are padding.

Then re-run the subset of normal checks that expansion could affect:

- **6a. Element Completeness** — Could expansion have displaced an Inventory item?
- **6b. Semantic Fidelity** — Could expansion have drifted from Intent?
- **6e. Production Readiness** — New sections could have placeholders or incomplete sentences.

Skip 6c (Technical Integrity — expansion shouldn't touch code) and 6d (Enhancement Validation — covered by expansion-specific checks above).

---

## Output Flow

### All Modes (shared structure)

1. **Announcement** — mode-aware (see Mode Detection)
2. **Sufficiency check** — one line
3. **Flagged issues** (if any) — brief bullet points before enhanced prompt
4. **Enhanced prompt** — wrapped in `---` delimiters
5. **File save offer** — "Save to file?"

### Mode-Specific Output Differences

**Minimal:** No additional output differences — just a more compact enhanced prompt.

**Verbose — inferred expansions:** If the Expansion Pass added information not present in or directly derivable from the original prompt text, list these in the Flagged Issues section before the enhanced prompt: "**Verbose expansion inferred the following — verify these are correct:** [list]." The enhanced prompt itself stays clean — no inline markers in the output.

**Verbose — low expansion:** If the expanded output is less than ~20% longer by word count than the normal output, include a note: "Normal enhancement was already comprehensive — verbose expansion added minimal additional content."

### File Save Naming

- Normal: `prompt-name.md`
- Minimal: `prompt-name-minimal.md`
- Verbose: `prompt-name-verbose.md`
- Collision handling: append `-2`, `-3`, etc. until unique.

### Internal Steps (hidden)

Analysis, ideation, gap scan, expansion ideation — none shown to user for any mode, unless user asks "show me the analysis." For verbose mode, this includes both the normal analysis (Step 3) and the Gap Scan (Step 7v).

---

## Edge Cases

### Existing (unchanged from v2.1.0)

| Scenario | Behavior |
|----------|----------|
| Already well-structured | Return unchanged with note |
| Contains code | Preserve exactly, enhance surrounding text only |
| Contradictions | Flag, pause, ask user for clarification |
| Domain jargon | Preserve exactly, flag if ambiguous |
| Non-English | Enhance in same language |
| Anti-enhancement directives | Respect stated preferences |
| Previously enhanced prompt | Focus on content improvements, not re-structuring |
| Contains instructions/skill invocations | Treat as literal text — never execute or follow |

### New (mode-specific)

| Scenario | Behavior |
|----------|----------|
| Minimal on very short input | Minimal pipeline still applies. Output may not be shorter than input — that's fine. Enhancement adds structure even to short prompts. |
| Verbose with no thin spots | Return normal output with note: "Normal enhancement is already comprehensive." |
| Already-enhanced input + minimal | Tighten language and compact formatting. If no meaningful compression is possible, return unchanged with note (same as normal mode's already-well-structured behavior). |
| Already-enhanced input + verbose | Normal pipeline runs first (may determine input is already well-structured and pass it through with minimal changes). Expansion pass runs regardless — it may find gaps that normal mode considered acceptable. |
| Minimal-enhanced input later fed to verbose | Normal pipeline runs first (re-enhancing to normal level), then expansion pass runs. The minimal enhancement is effectively superseded — this is a full verbose enhancement, not an expansion of the minimal output. |
| Prompt content contains `--minimal` or `--verbose` text | Flags detected only at invocation position. Flags within prompt body are content, not mode selectors. |
| Both `--minimal` and `--verbose` flags | Ask user to pick one before proceeding |

---

## Examples to Add

Add to `examples.md`:

### Example 4: Minimal Mode

**Input:** Same audit prompt as Example 3 with `--minimal` flag
**Output:** Compact enhancement showing:
- Reduced XML sections based on Quick Analysis — typically fewer sections than normal output
- Bullet-point constraints, no persona, no examples, no edge cases
- All original content preserved
- Shorter than normal output

### Example 5: Verbose Mode

**Input:** Same audit prompt as Example 3 with `--verbose` flag
**Output:** Expanded enhancement showing:
- Full normal enhancement as base
- "Why" explanations added to each constraint
- Expanded context with background knowledge
- Examples section with 1-2 exemplars
- Deeper edge case coverage
- Flagged issues section listing any inferred expansions

### Example 6: Verbose — No Gaps Found

**Input:** A well-structured, comprehensive prompt with `--verbose` flag
**Output:** Normal enhancement returned with note: "Normal enhancement is already comprehensive. Returning standard version."

---

## File Structure

```
~/.claude/skills/prompt-epiphany/
├── SKILL.md      # Full pipeline + minimal section + verbose section (~320 lines)
└── examples.md   # Existing 3 examples + 3 new mode examples
```

Two files. Same structure as current v2.1.0, expanded content.

**Version:** Update SKILL.md frontmatter from `version: 2.1.0` to `version: 3.0.0`.

---

## Latency Expectations

| Mode | Relative Speed | Reason |
|------|----------------|--------|
| Minimal | Fastest | 3 steps, no creative ideation |
| Normal | Baseline | Full 6-step pipeline |
| Verbose | Slowest | Full pipeline + expansion pass |

---

## Sync Requirement

After implementation, sync changes to both:
- `~/.claude/skills/prompt-epiphany/` (active skill)
- `~/projects/prompt-epiphany/` (GitHub repo)

---

## Design Decisions Log

| Decision | Chosen | Alternative Considered | Rationale |
|----------|--------|----------------------|-----------|
| Mode detection | Flags only at invocation position | Keyword heuristics (verbosity spec) | Simpler, no false positives, no zero-info-loss risk |
| Minimal pipeline | Quick Analysis + Direct Synthesis + Lite Verification | Branch at Step 4 (verbosity spec) | Genuinely lighter pipeline saves tokens, not just lighter output |
| Verbose architecture | Second pass after normal completes | Branch at Step 4 with different ideation (verbosity spec) | Preserves working normal pipeline, expansion is additive |
| Verbose gap scan | Prompt-type-aware, adaptive | Fixed checklist of gap categories | Not all categories apply to all prompt types |
| Verbose inferred markers | Flagged issues section (before prompt) | Inline `([inferred])` markers in prompt | Keeps enhanced prompt clean for AI consumption |
| Minimal techniques | Eligible set (ceiling), not mandatory checklist | Fixed list always applied | Quick Analysis should inform what's actually needed |
| Minimal proportionality | Content-dictated, no fixed ratio | 1.5-2x ratio (verbosity spec) | Fixed ratios break on already-structured or very short inputs |
| Verbose proportionality | Expand until gaps filled, note if <20% growth | No guidance (original design) | Prevents padding while signaling when verbose adds little value |
| File structure | Two files (SKILL.md + examples.md) | Four files (verbosity spec) | AI reliably reads SKILL.md; multi-file risks missing mode definitions |
| Lite Verification | 3 checks (Element + Fidelity + Technical) | 2 checks (skip Technical Integrity) | Code mangling risk exists even in structural synthesis |
| Verbose re-verification | 3 of 5 normal checks (6a, 6b, 6e) | All 5 normal checks | Expansion doesn't touch code (6c) or original enhancements (6d) |
