# Prompt Epiphany Verbosity Feature Design

**Date:** 2026-04-04
**Status:** Approved
**Files:** `~/.claude/skills/prompt-epiphany/`

---

## Overview

Add three verbosity modes to the prompt-epiphany skill: minimal, normal (enhanced), and verbose. All modes use the same rigorous pipeline (Analysis → Ideation → Synthesis → Verification) but differ in synthesis focus and verification checks.

---

## Goals

1. Allow users to request compact output via `--minimal` flag or "minimal/compact/concise" keywords
2. Allow users to request expanded output via `--verbose` flag or "verbose/expanded/long/detailed" keywords
3. Maintain the same content integrity guarantees across all modes (ZERO INFORMATION LOSS)
4. Keep SKILL.md under 400 lines by splitting into modular files

---

## File Structure

```
~/.claude/skills/prompt-epiphany/
├── SKILL.md          # Entry point, core pipeline, hard gates, triggers (~200 lines)
├── techniques.md     # T1-T13 reference matrix, application order
├── verbosity.md      # Minimal/normal/verbose logic, detection, verification
└── examples.md       # Before/after examples (existing + 3 new)
```

---

## Verbosity Modes

| Mode | Trigger | Output Characteristic |
|------|---------|------------------------|
| **Minimal** | `--minimal` or keywords: "minimal", "compact", "concise", "shorter" | Compressed structure, tightened language, ALL content preserved |
| **Normal** | No flag/keyword (default) | Optimal output — current behavior |
| **Verbose** | `--verbose` or keywords: "verbose", "expanded", "long", "detailed" | Expanded context, "why" explanations, deeper reasoning |

---

## Detection Rules

**Timing:** Detect at Step 1 (Gather) before pipeline begins.

**Matching rules:**
- `--minimal` or `--verbose` flags anywhere in input → trigger that mode
- Keywords must be **unambiguous context**: "minimal version", "compact output", "verbose prompt" → trigger
- "minimal example" (describing the example itself) → does NOT trigger minimal
- "verbose explanation" (describing the content) → does NOT trigger verbose

**Detection heuristic:** Keyword must modify "version/output/prompt" or appear as flag. Keyword modifying other nouns does not trigger.

**Conflict handling:** If conflicting keywords detected (both minimal and verbose triggers), ask for clarification before proceeding.

---

## Pipeline Architecture

All modes share Steps 1-3. Branch at Step 4 (Ideation), reunite at Step 6 (Verification).

```
Step 1: Gather (shared)
        ↓ Detect verbosity mode
Step 2: Sufficiency Check (shared)
        ↓
Step 3: Analysis — 6 dimensions (shared)
        ↓
        ┌─────────────────┬─────────────────┬─────────────────┐
        ↓                 ↓                 ↓
Step 4: Ideation     Step 4: Ideation   Step 4: Ideation
        ↓                 ↓                 ↓
        └─────────────────┴─────────────────┘
                          ↓
Step 5: Synthesis (one path: minimal | normal | verbose)
        ↓
Step 6: Verification (mode-specific checks)
        ↓
Step 7: Output
```

**Only one path executes.** Analysis is shared, then one synthesis branch runs.

---

## Ideation by Mode

### Minimal Ideation Focus
- T2 reversal: Where can sections be merged? (undo decomposition from normal synthesis)
- Redundancy identification: What phrases repeat meaning?
- Constraint compaction: Can DO/DO NOT be combined?
- Skips: T4 (persona), T9 (examples), T11 (context anchor), T12 (audience)
- Applies: T1 (XML structure with merging), T3 (constraints as bullets), T7 (priority), T8 (edge cases brief), T10 (self-critique), T13 (escape hatch)

### Normal Ideation Focus
- Standard gap analysis across T1-T13
- Apply only what gap analysis identifies

### Verbose Ideation Focus
- **Second ideation pass** after standard first pass completes
- First pass: standard gap analysis
- Second pass: explore T1-T13 not applied in first pass
- Always consider: T4 (persona depth), T6 (reasoning guidance), T9 (examples), T12 (audience calibration)
- "Why" extraction: For each constraint, identify and articulate the underlying reason
- Context expansion: What background knowledge would help?

---

## Synthesis by Mode

### Minimal Synthesis
- Merge adjacent sections where semantically safe (context+task → context/task hybrid)
- Bullet-point constraints (remove narrative framing)
- Remove examples section unless critical to understanding
- Tighten all language: remove filler words, compress phrases
- Preserve ALL INVENTORY items exactly (code, formulas, named entities, numeric values)
- Same XML tag structure, fewer words inside

### Normal Synthesis
- Current behavior: optimal output based on gap analysis
- Proportionality: Short (<50 words): up to 4x. Medium (50-500): 3-4x. Long (500+): similar length.

### Verbose Synthesis
- Expand context section with background knowledge and assumptions
- Add "why" explanation to each constraint
- Always include examples section with 1-2 exemplars
- Expand reasoning guidance in task section
- Add T4 (persona) if not present, with deeper calibration
- Same XML tag structure, more words inside

---

## Verification by Mode

### Shared Checks (all modes)
- **6a. Element Completeness** — Every INVENTORY item exists in output
- **6b. Semantic Fidelity** — INTENT matches enhanced prompt
- **6c. Technical Integrity** — Code, formulas, API refs content-identical
- **6d. Enhancement Validation** — Every enhancement traces to ideation
- **6e. Production Readiness** — No placeholders, incomplete sentences, empty tags

### Mode-Specific Checks

| Mode | Additional Check |
|------|-------------------|
| Minimal | **6f. Compression Achieved**: Output is more compact than normal would be. If input was already minimal, return with note. |
| Verbose | **6f. Expansion Validated**: Expansions trace to ideation (no fabrication), expansions improve clarity/accuracy. If input was already verbose and no expansion possible, return with note. |

---

## Already-Optimal Inputs

| Mode | Behavior |
|------|----------|
| Minimal | If no meaningful compression possible, return with note: "Input already minimal — no compression applied." |
| Normal | Return unchanged with note (current behavior) |
| Verbose | If no meaningful expansion possible, return with note: "Input already comprehensive — minimal expansion applied." |

---

## Proportionality Rules

| Mode | Proportionality |
|------|-----------------|
| Minimal | Compress regardless of input length. May be shorter than input. |
| Normal | Short (<50 words): up to 4x. Medium (50-500): 3-4x. Long (500+): similar length. |
| Verbose | Expand regardless of input length. May be longer than normal. |

---

## Technique Application Matrix

| Technique | Minimal | Normal | Verbose |
|-----------|---------|--------|---------|
| T1 XML structuring | Apply (merge) | Apply (if needed) | Apply (full) |
| T2 Decomposition | Skip (reverse) | Apply (if needed) | Apply |
| T3 Constraints | Apply (compact) | Apply | Apply (expanded) |
| T4 Persona | Skip | Apply (if needed) | Always apply |
| T5 Output format | Apply (minimal) | Apply | Apply (full) |
| T6 Reasoning | Skip | Apply (if needed) | Always apply |
| T7 Priority | Apply | Apply | Apply |
| T8 Edge cases | Apply (brief) | Apply | Apply (expanded) |
| T9 Examples | Skip | Apply (if needed) | Always include |
| T10 Self-critique | Apply | Apply | Apply |
| T11 Context anchor | Skip | Apply (if needed) | Apply |
| T12 Audience | Skip | Apply (if needed) | Always apply |
| T13 Escape hatch | Apply | Apply | Apply |

---

## Output Flow

**What user sees:**

1. **Announcement:** "I'm using the prompt-epiphany skill to create a [minimal/normal/verbose] enhanced prompt."
2. **Sufficiency check:** One line — either "Sufficient — [reason]" or blocking explanation
3. **Flagged issues:** Brief bullet points (if any)
4. **Enhanced prompt:** Wrapped in `---` delimiters
5. **File save offer:** "Save to file?" — filename appends `-minimal` or `-verbose` if applicable

---

## Examples to Add

Add three new examples to `examples.md`:

### Example 4: Minimal Version
- BEFORE: Same audit prompt as Example 3
- MINIMAL OUTPUT: Compressed version showing structural merging, bullet constraints, no examples section

### Example 5: Verbose Version
- BEFORE: Same audit prompt as Example 3
- VERBOSE OUTPUT: Expanded with "why" for each constraint, deeper context, examples section

### Example 6: Conflict Resolution
- INPUT: "create a minimal but verbose version of this prompt"
- OUTPUT: Clarification request explaining the conflict

---

## SKILL.md Changes

### Add to Trigger Conditions

```
| Trigger | Behavior |
|---------|----------|
| `/prompt-epiphany --minimal` | Activate with minimal output mode |
| `/prompt-epiphany --verbose` | Activate with verbose output mode |
| User mentions "minimal/compact/concise" | Activate with minimal mode |
| User mentions "verbose/expanded/long" | Activate with verbose mode |
| Conflicting keywords | Ask for clarification before proceeding |
```

### Add to Output Flow

```
1. Announcement includes mode: "I'm using the prompt-epiphany skill to create a [minimal/normal/verbose] enhanced prompt."
```

### Add File References

```
See [techniques.md](techniques.md) for T1-T13 reference.
See [verbosity.md](verbosity.md) for minimal/normal/verbose mode details.
See [examples.md](examples.md) for before/after examples.
```

---

## Sync Requirement

After implementation, sync changes to both:
- `~/.claude/skills/prompt-epiphany/` (active skill)
- `~/projects/prompt-epiphany/` (GitHub repo)