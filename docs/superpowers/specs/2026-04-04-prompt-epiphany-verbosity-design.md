# Prompt Epiphany Verbosity Feature Design

**Date:** 2026-04-04
**Status:** Approved
**Files:** `~/.claude/skills/prompt-epiphany/`

---

## Overview

Add three verbosity modes to the prompt-epiphany skill: minimal, normal (enhanced), and verbose. All modes use the same rigorous pipeline (Analysis → Ideation → Synthesis → Verification) but differ in synthesis focus and verification checks.

---

## Goals

1. Allow users to request compact output via `--minimal` flag or explicit mode keywords
2. Allow users to request expanded output via `--verbose` flag or explicit mode keywords
3. Maintain the same content integrity guarantees across all modes (ZERO INFORMATION LOSS)
4. Keep SKILL.md under 200 lines by splitting into modular files
5. Prevent semantic damage from over-compression or fabricated expansion

---

## File Structure

```
~/.claude/skills/prompt-epiphany/
├── SKILL.md          # Entry point, core pipeline, hard gates, triggers (~200 lines)
├── techniques.md     # T1-T13 reference matrix, application order, T2 reversal for minimal
├── verbosity.md      # Minimal/normal/verbose logic, detection, verification, mode suitability
└── examples.md       # Before/after examples (existing + 3 new + mode suitability guidance)
```

---

## Verbosity Modes

| Mode | Trigger | Output Characteristic |
|------|---------|------------------------|
| **Minimal** | `--minimal` flag, or "minimal version", "compact output", "concise prompt" | Compressed structure, tightened language, ALL content preserved |
| **Normal** | No flag/keyword (default) | Optimal output — current behavior |
| **Verbose** | `--verbose` flag, or "verbose version", "expanded output", "detailed prompt" | Expanded context, "why" explanations, deeper reasoning |

**Mode Suitability Guidance:**

| Mode | Best For |
|------|----------|
| Minimal | Input is verbose, redundant, over-structured. User wants copy-paste efficiency. |
| Normal | Input is medium complexity, needs structure. Most common use case. |
| Verbose | Input is sparse, missing context, no constraints. User wants comprehensive prompt. |

---

## Detection Rules

**Timing:** Detect at Step 1 (Gather) before pipeline begins.

### Flag Syntax (Always Triggers)
- `--minimal` anywhere in input → minimal mode
- `--verbose` anywhere in input → verbose mode

### Keyword Syntax (Requires Explicit Mode Language)
| Keyword | Required Phrase | Does NOT Trigger |
|---------|-----------------|------------------|
| minimal | "minimal version", "compact output", "concise prompt" | "minimal example", "make it shorter" |
| verbose | "verbose version", "expanded output", "detailed prompt" | "verbose explanation", "make it longer" |

**Detection heuristic:** Keyword must modify "version/output/prompt" explicitly. Keywords modifying other nouns (example, explanation, description) or verbs (make, create, give) do NOT trigger.

### Conflict Handling
If conflicting flags/keywords detected (both minimal and verbose), ask for clarification before proceeding:
> "I see both 'minimal' and 'verbose' in your request. Which should I prioritize? Please specify one mode."

### Already-Enhanced Input Detection
If input already contains `<role>`, `<task>`, `<constraints>` tags (signs of prior enhancement):
- **Minimal:** Compress the existing structure
- **Normal:** Note "Input already well-structured" and focus on content improvements only
- **Verbose:** Expand existing sections rather than adding new structure

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
- **T2 reversal:** Where can sections be merged? (undo decomposition from normal synthesis)
- **Redundancy identification:** What phrases repeat meaning?
- **Constraint compaction:** Can DO/DO NOT be combined?
- **Skips:** T4 (persona), T9 (examples), T11 (context anchor), T12 (audience)
- **Applies:** T1 (XML structure with merging), T3 (constraints as bullets), T7 (priority), T8 (edge cases brief), T10 (self-critique), T13 (escape hatch)

### Normal Ideation Focus
- Standard gap analysis across T1-T13
- Apply only what gap analysis identifies
- Anti-pattern: Do not apply all techniques regardless of need

### Verbose Ideation Focus
- **First pass:** Standard gap analysis (same as normal)
- **Second pass:** Explore T1-T13 not applied in first pass
- **Consider (not forced):** T4 (persona depth), T6 (reasoning guidance), T9 (examples), T12 (audience calibration)
- **Only apply if gap analysis confirms value:** If T4 would add nothing, skip with note
- **"Why" extraction:** For each constraint, identify underlying reason from intent. If uncertain, add "([inferred])" marker or skip.
- **Context expansion:** What background knowledge would help?

---

## Synthesis by Mode

### Minimal Synthesis
- **Merge rules (safe merges only):**
  - CAN merge: `<context>` + `<defaults>` → `<context>` (background includes defaults)
  - CAN merge: `<constraints>` + `<edge_cases>` → `<constraints>` (edge cases as bullets)
  - NEVER merge: `<task>`, `<role>` — these are anchors, must remain separate
- Bullet-point constraints (remove narrative framing)
- Remove examples section unless critical to understanding
- Tighten all language: remove filler words, compress phrases
- Preserve ALL INVENTORY items exactly (code, formulas, named entities, numeric values)
- **Compression guardrail:** Stop when removal would lose meaning. If compression ratio exceeds 50%, flag for user review.

### Normal Synthesis
- Current behavior: optimal output based on gap analysis
- Proportionality: Short (<50 words): up to 4x. Medium (50-500): 3-4x. Long (500+): similar length.

### Verbose Synthesis
- Expand context section with background knowledge and assumptions
- Add "why" explanation to each constraint (with inference marker if not explicit in source)
- Include examples section with 1-2 exemplars (only if gap analysis confirms value)
- Expand reasoning guidance in task section
- Add T4 (persona) if gap analysis confirms it adds value, with deeper calibration
- **No fabrication rule:** All expansions must trace to ideation. If adding "why" to a constraint, must come from explicit or strongly implied intent.

---

## Verification by Mode

### Shared Checks (all modes)
- **6a. Element Completeness** — Every INVENTORY item exists in output
- **6b. Semantic Fidelity** — INTENT matches enhanced prompt
- **6c. Technical Integrity** — Code, formulas, API refs content-identical
- **6d. Enhancement Validation** — Every enhancement traces to ideation
- **6e. Production Readiness** — No placeholders, incomplete sentences, empty tags

### Minimal-Specific Checks
- **6f. Compression Achieved:** Output is more compact than normal would be
- **6g. Semantic Integrity:** No sections merged that shouldn't be (task, role preserved)
- **6h. Compression Limit:** If compression >50% of original, flag for user review

### Verbose-Specific Checks
- **6f. Expansion Validated:** Expansions trace to ideation (no fabrication)
- **6g. Rationale Accuracy:** If adding "why" to constraints, must trace to explicit or strongly implied intent. If uncertain, marked as "([inferred])" or skipped.
- **6h. Value Added:** Expansion improves clarity/accuracy. If no expansion possible, return normal with note.

### Already-Optimal Handling
If verification finds no meaningful improvement possible:
- **Minimal:** Return normal version with note: "Input already minimal — returning optimized version instead."
- **Normal:** Return unchanged with note (current behavior)
- **Verbose:** Return normal version with note: "Input already comprehensive — returning optimized version instead."

---

## Proportionality Rules

| Mode | Proportionality |
|------|-----------------|
| Minimal | Compress until removal would lose meaning. Guardrail: if >50% compression, flag for review. |
| Normal | Short (<50 words): up to 4x. Medium (50-500): 3-4x. Long (500+): similar length. |
| Verbose | Expand until clarity achieved. No arbitrary limit, but check value at each expansion. |

---

## Technique Application Matrix

| Technique | Minimal | Normal | Verbose |
|-----------|---------|--------|---------|
| T1 XML structuring | Apply (merge safe sections) | Apply (if needed) | Apply (full structure) |
| T2 Decomposition | Reversal (merge sections) | Apply (if needed) | Apply |
| T3 Constraints | Apply (compact bullets) | Apply | Apply (expanded with why) |
| T4 Persona | Skip | Apply (if needed) | Consider (if adds value) |
| T5 Output format | Apply (minimal) | Apply | Apply (full) |
| T6 Reasoning | Skip | Apply (if needed) | Consider (if adds value) |
| T7 Priority | Apply | Apply | Apply |
| T8 Edge cases | Apply (brief) | Apply | Apply (expanded) |
| T9 Examples | Skip (unless critical) | Apply (if needed) | Consider (if adds value) |
| T10 Self-critique | Apply | Apply | Apply |
| T11 Context anchor | Skip | Apply (if needed) | Consider (if adds value) |
| T12 Audience | Skip | Apply (if needed) | Consider (if adds value) |
| T13 Escape hatch | Apply | Apply | Apply |

**Note:** Verbose "Consider" means apply only if gap analysis confirms value. Never force-apply techniques.

---

## Output Flow

**What user sees:**

1. **Announcement:** "I'm using the prompt-epiphany skill to create a [minimal/normal/verbose] enhanced prompt."
2. **Sufficiency check:** One line — either "Sufficient — [reason]" or blocking explanation
3. **Already-enhanced detection (if applicable):** "Input already contains structured sections. [Mode-specific behavior]."
4. **Flagged issues:** Brief bullet points (if any)
5. **Enhanced prompt:** Wrapped in `---` delimiters
6. **Mode note (if applicable):** "Input already [minimal/comprehensive] — returning optimized version instead."
7. **File save offer:** "Save to file?" — filename appends `-minimal` or `-verbose` if applicable

### File Naming Collision Handling
If file already exists:
- `prompt-name-minimal.md` → `prompt-name-minimal-2.md`
- `prompt-name-verbose.md` → `prompt-name-verbose-2.md`
- Increment counter until unique filename found

---

## Examples to Add

Add to `examples.md`:

### Example 4: Minimal Version
**BEFORE:** Same audit prompt as Example 3
**MINIMAL OUTPUT:** Compressed version showing:
- Structural merging (context+defaults combined)
- Bullet constraints (no narrative)
- No examples section
- All content preserved

### Example 5: Verbose Version
**BEFORE:** Same audit prompt as Example 3
**VERBOSE OUTPUT:** Expanded version showing:
- "Why" for each constraint
- Deeper context section
- Examples section with 2 exemplars
- Expanded reasoning

### Example 6: Conflict Resolution
**INPUT:** "create a minimal but verbose version of this prompt"
**OUTPUT:** Clarification request explaining the conflict

### Example 7: Mode Suitability
**GUIDANCE SECTION:** When to use each mode:
- Use minimal when: Input is verbose, redundant, over-structured
- Use normal when: Input is medium complexity, needs structure
- Use verbose when: Input is sparse, missing context, no constraints

---

## SKILL.md Changes

### Update Frontmatter Description
```
description: "Enhances prompts via 13-technique pipeline. Supports --minimal/normal/verbose modes. ONLY on /prompt-epiphany or explicit name mention. Do NOT activate for generic 'enhance' requests."
```

### Add to Trigger Conditions

```
| Trigger | Behavior |
|---------|----------|
| `/prompt-epiphany --minimal` | Activate with minimal mode |
| `/prompt-epiphany --verbose` | Activate with verbose mode |
| "minimal version/compact output/concise prompt" | Activate with minimal mode |
| "verbose version/expanded output/detailed prompt" | Activate with verbose mode |
| Conflicting flags/keywords | Ask for clarification before proceeding |
```

### Add File References
```
See [techniques.md](techniques.md) for T1-T13 reference and T2 reversal for minimal mode.
See [verbosity.md](verbosity.md) for minimal/normal/verbose mode details and mode suitability.
See [examples.md](examples.md) for before/after examples and mode guidance.
```

---

## Latency Expectations

| Mode | Expected Speed | Reason |
|------|----------------|--------|
| Minimal | Fastest | One pass, compression focus |
| Normal | Baseline | Single pass gap analysis |
| Verbose | Slowest | Two ideation passes |

**Progress indicator for verbose (optional):** "Analyzing... Expanding..."

---

## Sync Requirement

After implementation, sync changes to both:
- `~/.claude/skills/prompt-epiphany/` (active skill)
- `~/projects/prompt-epiphany/` (GitHub repo)