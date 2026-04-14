# epiphany-prompt — Design Specification
**Date:** 2026-04-14
**Status:** Approved for implementation planning
**Trigger:** `/epiphany-prompt`
**Install path:** `~/.claude/skills/epiphany-prompt/`

---

## Goal

Build `epiphany-prompt` — a modular, subagent-orchestrated prompt enhancement skill that retains the complete `prompt-epiphany` methodology (all 5 modes, all 13+ techniques, all verification gates, zero-information-loss guarantee) but restructures delivery as a modular pipeline with swappable stage files, scale-aware depth, and per-stage specialization that produces meaningfully better output than the monolithic version.

**What makes it better than prompt-epiphany:**
- STANDARD/DEEP stages run as isolated Agent subagents with full context budget per task
- Merged stages (M12, M4M5, MSPEC12, etc.) combine naturally adjacent tasks — fewer spawns without losing critical isolation
- Three depth scales (FAST/STANDARD/DEEP) map to existing flags, each unlocking genuinely different capabilities
- Formal document handoffs (enhancement contracts, verification reports) create explicit quality gates between stages
- Stage introspection: any stage's output is inspectable on request (STANDARD/DEEP only)

**What does not change:**
- All 13+ enhancement techniques (T1–T13) with triggers, ordering, application rules
- All 5 operating modes: normal, minimal (→ FAST), verbose (→ DEEP), specification, plan
- All 12 verification checks (6a–6l), all 11 specification checks (S7a–S7k), all 9 plan checks (P9a–P9i)
- Preservation methodology, output formats, file save logic, hard gates
- Save path: `~/docs/epiphany/prompts/DD-MM-descriptive-name.md`

---

## Architecture

Three layers:

```
SKILL.md              Orchestrator only. Parses flags, detects mode + scale,
                      runs sufficiency check inline, spawns wave agents,
                      manages repair loops, routes output.
                      Exception: FAST scale runs the complete pipeline
                      inline (0 spawns). Documented deviation for speed.

modules/*.md          Stage protocols. Each spawned as an isolated Agent
                      subagent. One file per stage or merged stage. Replace
                      a file to upgrade it. Declares input_dependencies in
                      frontmatter. Merged modules write one output file per
                      logical stage they cover (may be 2–3 files total).

stages/ (session)     File-based state. Each subagent reads declared
                      input files from stages/ and writes its output file(s).
                      Orchestrator never reads stage files during pipeline
                      execution for routing decisions — it reads module
                      return values instead.
                      Documented exceptions (display-only, not routing):
                        1. Stage introspection: reads and displays stage
                           files on user request after a wave completes.
                        2. Double-failure fallback output: on two failed
                           verification cycles, orchestrator reads the
                           latest synthesis/expansion stage content to
                           produce best-effort output XML (no module
                           output XML was returned).
                      FAST scale: no session directory — all processing is
                      inline, no stage files persist, introspection N/A.
```

Session directories: `~/docs/epiphany/prompts/.sessions/{session_id}/stages/`
- `session_id = YYYYMMDD-[topic-slug]`
- Stage files persist after run (available for stage introspection)
- New session creates a new session_id directory; old ones are not auto-deleted
- **FAST scale**: no session directory created — processing is inline, stage introspection unavailable

---

## Two Control Axes

**Mode** — what the output is:

| Mode | Trigger | Output |
|------|---------|--------|
| normal | default | enhanced prompt (XML structure) |
| specification | `--specification` | formal requirements document |
| plan | `--plan` | granular step-by-step plan |

**Scale** — how deep the processing goes (maps to existing flags):

| Flag | Scale | Spawns (normal mode) |
|------|-------|----------------------|
| `--minimal` | FAST | 0 agents (inline) |
| *(default)* | STANDARD | 3 agents (5 if repair: +M3 +M4M5) |
| `--verbose` | DEEP | 5 agents (+2 per repair loop; max 9 if both W3+W5 repair) |

`--quiet` is display-only — does not affect scale or wave plan.

Scale does not apply to specification or plan modes — both always run STANDARD regardless of flag. Announce if flag was passed: "`--minimal`/`--verbose` does not apply to [spec/plan] mode — proceeding with STANDARD."

---

## Module Set — 11 files

```
~/.claude/skills/epiphany-prompt/modules/

Normal mode (5 files):
  m12-analysis-ideation.md   Analysis (6-dim + INVENTORY) → Ideation (contracts)
                             Two-phase single agent. Writes 3 output files.
                             DEEP: opportunity scoring → anti-conformity pass.
  m3-synthesis.md            Preservation-first synthesis. DEEP: self-critique.
  m4-verification.md         12-check verification — DEEP W3 only (standalone,
                             before expansion). Repair triggers M3 re-spawn.
  m5-expansion.md            DEEP only — gap scan + targeted expansion synthesis.
  m4m5-verify-output.md      12-check verification + output formatting.
                             Used in: STANDARD W3 (final), DEEP W5 (post-expansion).
                             On PASS: formats + saves output. On FAIL: returns
                             VERIFICATION: FAIL, triggers repair.

Specification mode (3 files):
  mspec12-domain-req.md      Domain analysis + requirement extraction (S2–S4).
                             Two-phase single agent. Writes 2 output files.
  mspec3-synthesis.md        Specification synthesis (S5).
  mspec4m5-verify-output.md  Completeness audit + 11-check verification + output (S6–S7).

Plan mode (3 files):
  mplan12-analysis-design.md Goal analysis + dependency mapping + safeguards (P2–P5).
                             Two-phase single agent. Writes 2 output files.
  mplan3-synthesis.md        Plan synthesis + execution simulation (P6–P7).
  mplan4m5-verify-output.md  Gap audit + 9-check verification + output (P8–P9).

FAST scale: no module files — complete pipeline runs inline in the orchestrator.
```

---

## Wave Plans

### Normal mode

```
FAST (inline)                STANDARD (3 agents)      DEEP (5 agents)

Orchestrator runs            W1: M12                  W1: M12
full pipeline inline:          Analysis+Ideation        Analysis+Ideation
  Quick Analysis             W2: M3 Synthesis         W2: M3 Synthesis
  Synthesis (--minimal      W3: M4M5                 W3: M4 Verify*
    technique subset)          Verify+Output          W4: M5 Expansion
  12-check Verification                               W5: M4M5
  Output                                                Verify+Output
No session directory.
Stage introspection N/A.

0 agents                     3 agents                 5 agents
```

*W3 DEEP repair: M3 re-spawned (max 1 attempt). Double-failure: skip W4+W5,
output synthesis directly with note. Re-spawned M4 shares same module file.

### Specification and plan modes

```
SPECIFICATION (3 agents)         PLAN (3 agents)

W1: MSPEC12 Domain+Req           W1: MPLAN12 Analysis+Design
W2: MSPEC3 Synthesis             W2: MPLAN3 Synthesis
W3: MSPEC4M5 Verify+Output       W3: MPLAN4M5 Verify+Output
```

### Mode × Scale matrix

| | FAST | STANDARD | DEEP |
|---|---|---|---|
| **normal** | inline pipeline | M12→M3→M4M5 | M12→M3→M4→M5-exp→M4M5 |
| **specification** | → STANDARD | MSPEC12→MSPEC3→MSPEC4M5 | → STANDARD |
| **plan** | → STANDARD | MPLAN12→MPLAN3→MPLAN4M5 | → STANDARD |
| **spec then plan** | → STANDARD both | spec pipeline → save spec output → plan pipeline (spec output as input) | → STANDARD both |
| **+ `--quiet`** | display suppressed, file saved | same | same |

### Chained spec+plan execution

When the user passes `--specification --plan` and confirms sequential run in STEP 0:

1. **Spec pipeline runs first** — full MSPEC12 → MSPEC3 → MSPEC4M5 sequence in its own `session_dir` (session_id = `YYYYMMDD-{topic_slug}`). Spec output XML is saved to `~/docs/epiphany/prompts/DD-MM-{filename_slug}.md` per normal output handling.
2. **Plan pipeline runs second** — starts a **new session** with its own `session_dir` (session_id = `YYYYMMDD-{topic_slug}-plan`; append `-plan` suffix to differentiate). The plan pipeline's `00-input.md` is populated with the spec output content (the full XML body from step 1's return message, not the saved file — no round-trip through disk). Plan output saves to `~/docs/epiphany/prompts/DD-MM-{filename_slug}-plan.md`.
3. **No intermediate confirmation** — the single upfront confirm in STEP 0 is sufficient. The orchestrator announces the transition ("Specification complete. Starting plan pipeline with spec as input.") but does not block.
4. **`--quiet`** applies to both pipelines. Both save to disk without display.
5. **Failure in spec phase** — spec pipeline always delivers (PASS-WITH-NOTES on gaps). Plan pipeline proceeds regardless. If spec had flagged gaps, the `<note>` block carries into plan's input context as part of the spec XML.
6. **Failure in plan phase** — same PASS-WITH-NOTES policy. Plan always delivers.

---

## Information Architecture

### Stage files

**FAST scale:** No session directory. No stage files. Processing is entirely inline.

**STANDARD/DEEP normal mode:**
```
stages/00-config.md         mode, scale, flags { quiet }, date (DD-MM),
                            session_id, input_type (A/B/C),
                            filename_slug (generated inline at STEP 2),
                            contract_schema: v1  [write-once by orchestrator]

stages/00-input.md          processed original input (flags stripped,
                            XML/prior-output extracted if type B/C)

stages/01-analysis.md       M12 Phase 1: full 6-dimension analysis (3a–3f)
stages/01-inventory.md      M12 Phase 1: INVENTORY checklist — standalone
                            preservation checklist (separate from full analysis)

stages/02-ideation.md       M12 Phase 2: enhancement contracts (see schema below)
stages/03-synthesis.md      M3: enhanced prompt draft
                            (STANDARD repair: overwritten in place;
                             DEEP repair: saved as 03-synthesis-failed.md first)
stages/04-verification.md   M4: 12-check results — DEEP W3 only (see schema below)
stages/05-expansion.md      M5-exp: DEEP only — gap scan findings + expanded output
stages/06-verification-2.md M4M5: DEEP W5 — verification results for expansion output
```

Note: verify+output modules (M4M5 / MSPEC4M5 / MPLAN4M5) do NOT write an output stage file. On PASS they return the formatted output XML as part of their Agent return message. The orchestrator handles display/save based on `00-config` quiet flag. This keeps the three-layer rule intact — orchestrator only reads Agent return values, never module output stage files.

**Specification mode** (00-config.md and 00-input.md shared):
```
stages/spec-01-domain.md       MSPEC12 Phase 1: domain analysis + decomposition
stages/spec-02-requirements.md MSPEC12 Phase 2: extracted requirements
stages/spec-03-synthesis.md    MSPEC3: specification document
stages/spec-04-verify.md       MSPEC4M5: completeness audit + 11 checks
```

**Plan mode** (00-config.md and 00-input.md shared):
```
stages/plan-01-analysis.md  MPLAN12 Phase 1: goal analysis + decomposition
stages/plan-02-design.md    MPLAN12 Phase 2: dependency map + safeguards
stages/plan-03-synthesis.md MPLAN3: plan document + execution simulation
stages/plan-04-verify.md    MPLAN4M5: gap audit + 9 checks
```

### Module invocation mechanism

Every STANDARD/DEEP wave spawns a subagent via the `Agent` tool. The orchestrator passes:

1. **`subagent_type`**: `"general-purpose"` (the module protocol file is referenced by path in the prompt, not loaded as a built-in agent type).
2. **`prompt`** (constructed inline): contains
   - Path to the module protocol file (`~/.claude/skills/epiphany-prompt/modules/{module_file}`) with instruction to read and follow it.
   - Absolute `session_dir` path.
   - Explicit list of stage files the module must read (per dependency table).
   - Explicit list of output files the module must write.
   - Variant hints if applicable (e.g., "this is the DEEP repair path; failed draft is at 03-synthesis-failed.md").

The module protocol itself uses the **Read tool** to load every declared input from `session_dir`, and the **Write tool** to write its declared outputs. The orchestrator never passes stage content inline in the Agent prompt — only paths. This keeps Agent prompts short and the orchestrator's context light.

### Module input_dependencies

FAST inline has no modules; all context is in-memory within the orchestrator.

| Module | Reads | Writes |
|--------|-------|--------|
| M12 Analysis+Ideation | `00-config` + `00-input` | `01-analysis`, `01-inventory`, `02-ideation` |
| M3 Synthesis (initial) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` | `03-synthesis` |
| M3 Synthesis (STANDARD repair) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` + `04-verification` | `03-synthesis` (overwrite) |
| M3 Synthesis (DEEP repair) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` + `03-synthesis-failed` + `04-verification` | `03-synthesis` (overwrite) |
| M4 Verification (DEEP W3) | `00-config` + `00-input` + `01-inventory` + `03-synthesis` | `04-verification` |
| M5 Expansion (DEEP W4) | `00-config` + `00-input` + `01-inventory` + `03-synthesis` (latest) | `05-expansion` |
| M5 Expansion (DEEP W5 repair) | `00-config` + `00-input` + `01-inventory` + `05-expansion-failed` + `06-verification-2` | `05-expansion` (overwrite) |
| M4M5 Verify+Output (STANDARD W3) | `00-config` + `00-input` + `01-inventory` + `03-synthesis` | `04-verification`; on PASS: returns output XML in Agent return message |
| M4M5 Verify+Output (DEEP W5) | `00-config` + `00-input` + `01-inventory` + `05-expansion` | `06-verification-2`; on PASS: returns output XML in Agent return message |
| MSPEC12 Domain+Req | `00-config` + `00-input` | `spec-01-domain`, `spec-02-requirements` |
| MSPEC3 Synthesis | `00-config` + `00-input` + `spec-01-domain` + `spec-02-requirements` | `spec-03-synthesis` |
| MSPEC4M5 Verify+Output | `00-config` + `00-input` + `spec-01-domain` + `spec-02-requirements` + `spec-03-synthesis` | `spec-04-verify`; on PASS or PASS-WITH-NOTES: returns output XML in Agent return message |
| MPLAN12 Analysis+Design | `00-config` + `00-input` | `plan-01-analysis`, `plan-02-design` |
| MPLAN3 Synthesis | `00-config` + `00-input` + `plan-01-analysis` + `plan-02-design` | `plan-03-synthesis` |
| MPLAN4M5 Verify+Output | `00-config` + `00-input` + `plan-01-analysis` + `plan-02-design` + `plan-03-synthesis` | `plan-04-verify`; on PASS or PASS-WITH-NOTES: returns output XML in Agent return message |

### Segregation rationale

**What was merged and why it's safe:**
- M12 (M1+M2): The critical isolation is M3 not seeing M2 reasoning — not M1 from M2. Analysis informing ideation directly is harmless and actually beneficial (opportunity scores flow inline, no file roundtrip).
- M4M5 (*verify+output): Verification runs first; output formatting only executes on PASS. No isolation concern — M5 never influenced analysis or synthesis quality.
- MSPEC12, MPLAN12: Same logic — requirements extraction builds directly on domain analysis. Isolation point is downstream (synthesis needs fresh context, not requirements from domain).

**What was kept separate and why:**
- M3 Synthesis: full context for synthesis only. Must not see M12 reasoning or previous synthesis attempts inline. Critical isolation preserved.
- M4 Verification (DEEP W3): fresh eyes on synthesis before expansion runs. M4 must not see how synthesis was produced.
- M5 Expansion: runs against verified output, not mid-synthesis context. Fresh perspective on gaps.
- STANDARD repair: M3 re-spawned with no failed draft — clean regeneration, no anchoring to the failure.
- DEEP repair: M3 re-spawned with failed draft + report — surgical fix using structured failure data.

### Enhancement contract schema (v1)

```
{
  technique: T[N] | "other:[description]",
  target_section: <xml-tag> | "global",
  action: "[imperative — what to add/change]",
  rationale: "[why this improves the prompt]",
  priority: high | medium | low
}
```

Contract conflict rule: if a contract conflicts with an anti-pattern directive in the input (e.g., "add persona" vs "do not assign roles"), M3 skips the contract, logs the conflict in `03-synthesis.md` header, and M4 flags it in the verification report.

### Verification report schema

```
Per check:
{
  check: 6a | 6b | ... | 6l | S7a | S7b | ... | S7k | P9a | P9b | ... | P9i,
  result: pass | fail | pass-with-note,
  detail: "[specific failed item, verbatim]",
  repair_target: "[section or XML tag to fix]"
}

Summary (normal mode):
{
  preservation_counts: { urls: N, paths: N, tech_version: N, ... },
  overall: pass | fail | pass-with-notes
}

Summary (specification mode):
{
  coverage_counts: { requirements: N, shall: N, should: N, may: N, gaps: N },
  overall: pass | fail | pass-with-notes
}

Summary (plan mode):
{
  coverage_counts: { steps: N, dependencies_mapped: N, safeguards: N, gaps: N },
  overall: pass | fail | pass-with-notes
}
```

### Module frontmatter schema

Every module file MUST begin with YAML frontmatter declaring its contract. The orchestrator uses `input_dependencies` and `output_files` to validate stage state before and after spawning.

```yaml
---
name: [module filename without .md]
stage_id: M12 | M3 | M4 | M5 | M4M5 | MSPEC12 | MSPEC3 | MSPEC4M5 | MPLAN12 | MPLAN3 | MPLAN4M5
input_dependencies:
  - 00-config.md
  - 00-input.md
  - [other stage files this module reads — see dependency table]
output_files:
  - [stage file(s) this module writes — see dependency table]
scale_variants: [FAST | STANDARD | DEEP]   # only variants that apply (FAST never listed — no module files)
kb_sources:
  - kb/[path/to/relevant/entry.md]          # advisory; listed for audit/lineage. The orchestrator does not load these.
activation:
  mode: normal | specification | plan
  wave: [wave number where this module runs]
  role: [human-readable role, e.g., "analysis+ideation", "verify+output"]
return_contract: |
  [One-line description of what the Agent return message looks like. For
   verify+output modules: include the full PASS/FAIL format. For non-verify
   modules: "[Module] complete. Wrote: [file list]."]
---
```

`kb_sources` enables later auditing of which KB entries informed a module's protocol design. The module protocol body cites these entries inline by relative path.

**Variant inputs:** `input_dependencies` declares the **primary (initial)** input set only. Repair-path variants (e.g., M3 DEEP repair reads `03-synthesis-failed.md` + `04-verification.md` in addition to the primary set) are not listed in frontmatter — the orchestrator dispatches variant input lists at spawn time per the dependency table. The module protocol body documents which variant paths exist and how to detect them from the orchestrator's spawn prompt (which lists exactly which files to read for that invocation).

Frontmatter is parsed and validated by the orchestrator at pre-spawn time. Missing required fields → HALT with clear error identifying the module.

### FAST scale — inline execution

- Complete pipeline runs in the orchestrator's context window. Zero subagent spawns.
- Quick Analysis: Intent extraction + INVENTORY (abbreviated form of 6-dimension analysis).
- Synthesis: apply the same technique subset as `prompt-epiphany --minimal` mode — consult `~/.claude/skills/prompt-epiphany/SKILL.md` for the exact list. Do not hardcode the list here; prevents drift when source skill evolves.
- 12-check verification inline. Output formatting + file save inline.
- Quality floor: identical to `prompt-epiphany --minimal`. No regression.
- **Limitation:** shares context window with existing conversation. Long inputs or long sessions may produce lower quality due to competing context. Use STANDARD for complex prompts.
- No session directory. Stage introspection unavailable (tradeoff accepted for speed).

### M12 Analysis+Ideation — what merging enables

- Two-phase single agent. Phase 1: full 6-dimension analysis (3a–3f) + writes `01-analysis.md` and `01-inventory.md`. Phase 2: enhancement ideation → writes `02-ideation.md`.
- Full context budget covers both analysis AND ideation. Enhancement opportunity scores flow directly from Phase 1 to Phase 2 — no file roundtrip.
- M3 receives only the structured contract output, not M12's reasoning. Critical isolation preserved.
- **DEEP Phase 1:** scores each weakness high/medium/low by impact. Phase 2 uses scores to allocate enhancement budget, then runs Anti-Conformity second pass (contrarian enhancer perspective). Both passes internal — no sub-spawning.
- Upgrade path: replace `m12-analysis-ideation.md` to change either or both phases. M3 only requires the contract schema (v1) to be unchanged.
- Tradeoff: ideation strategy cannot be swapped independently of analysis. To update only the ideation logic, edit the Phase 2 section of the module file.

### M3 Synthesis — what isolation enables

- Full context for synthesis only — no M12 reasoning in context, no verification pressure.
- Executes enhancement contracts rather than rediscovering rationale mid-synthesis.
- Preservation-first: full context budget to place every INVENTORY item before adding any enhancement.
- **DEEP: Iterative self-critique.** Draft → internal self-critique against INVENTORY + contracts → targeted revision → final. Two passes within single agent, no sub-spawn.

### M4 Verification — what isolation enables (DEEP W3 only)

- Standalone in DEEP only. Runs after M3, before M5-Expansion.
- Fresh eyes on synthesis — no memory of how it was produced.
- **DEEP repair:** M3 re-spawned targeted (03-synthesis-failed.md + 04-verification) — surgical fix using structured failure data.
- Max 1 repair attempt. **Double-failure:** skip W4+W5. Orchestrator outputs synthesis inline with note. Do not run expansion on content that failed two verification rounds.

### M5 Expansion — what isolation enables (DEEP W4 only)

- Fresh eyes on verified synthesis — no memory of synthesis reasoning.
- Runs against post-M4 verified output — expands content known to be preservation-complete.
- No-op path: if gap scan finds nothing thin, writes pass-through to `05-expansion.md` with "already comprehensive" note. M4M5 at W5 still runs against `05-expansion.md` in both cases.

### M4M5 Verify+Output — what merging enables

- Two-phase single agent: Phase 1 (verification), Phase 2 (output formatting — only on PASS).
- Used in STANDARD (W3, final step) and DEEP (W5, post-expansion step).
- Inputs vary by context: STANDARD reads `03-synthesis`; DEEP W5 reads `05-expansion`. The orchestrator selects and passes the correct input file in the Agent prompt. The module reads whatever synthesis-like file it is told to read — module behavior does not branch on the `00-config` scale field, so the same module file transparently handles both waves.
- Fresh eyes: no memory of how synthesis/expansion was produced. Same isolation property as standalone M4.
- On PASS: generates formatted output XML; returns `"VERIFICATION: PASS\n\n<prompt>...</prompt>"` — the PASS line followed by the full output XML in the Agent return message. Orchestrator parses the return message to extract XML for display/save.
- On FAIL: writes verification report, returns `"VERIFICATION: FAIL — [summary]"`. No output. Orchestrator triggers repair (max 1 attempt).

### Spec and Plan modules — what merging enables

- **MSPEC12:** Domain analysis informs requirements extraction in same context — naturally sequential, no isolation loss. Requirements must trace to domain; running them together enforces this implicitly. Writes `spec-01-domain.md` and `spec-02-requirements.md` for introspection.
- **MSPEC3:** Specification synthesis needs fresh context. Separate agent with no MSPEC12 reasoning inline. Critical isolation.
- **MSPEC4M5:** 11-check verification then output. Same merge rationale as M4M5. Delivers with flagged gaps if verification fails (no repair loop for spec mode). Always returns `"VERIFICATION: <status>\n\n<specification>...</specification>"` — even on FAIL-with-gaps, output XML accompanies the failure summary so orchestrator can display/save the best-effort document with a `<note>` block appended.
- **MPLAN12/MPLAN3/MPLAN4M5:** Mirror pattern. MPLAN3 Execution Simulation has full context for mental walkthrough with no prior dependency-mapping context competing. MPLAN4M5 return contract matches MSPEC4M5 — output XML always returned (with `<note>` appended on failed checks).

---

## Orchestrator Pseudocode

```
STEP 0 — FLAG DETECTION
  Parse first/last token only:
    Scale:   --minimal → FAST | --verbose → DEEP | (none) → STANDARD
    Mode:    --specification → spec | --plan → plan | (none) → normal
    Display: --quiet → quiet (display only, no effect on wave plan)
  Conflicts:
    --minimal + --verbose → BLOCK: ask user to pick one
    --specification + --plan → ASK user: "Run --specification first, then
      --plan on its output sequentially? (y/n)". If yes → proceed
      sequentially (spec pipeline runs to completion, then plan pipeline
      runs with spec output as input). If no → ASK which single mode to
      run. BLOCK until resolved.
    scale flag + spec/plan mode → ignore scale, announce dismissal in STEP 2
  Strip flags from input body. Mid-sentence flags = content.

STEP 1 — INPUT ROUTING
  File path input: if input string matches a file path heuristic
    (starts with ~/, /, ./, or ../ AND refers to an existing file on
    disk), read file contents and use as processed input. Otherwise
    treat input as inline text.
  Detect input type:
    A — raw text (default)
    B — prompt-epiphany XML: root element is <prompt> or <enhanced_prompt>
        AND contains at least one of <task>, <context>, <constraints>
        as direct children AND no <meta source="epiphany-prompt"/> marker
        → extract <task>, <context>, <constraints> contents
        (Heuristic requires inner structure to avoid misclassifying
         raw prompts that merely contain the word "prompt" in tags.)
    C — prior epiphany-prompt output: contains <meta source="epiphany-prompt"/>
        marker → extract original input section, start fresh pipeline
        (All epiphany-prompt outputs include this marker — see Output
         format section.)

STEP 2 — ANNOUNCE
  Emit before any analysis:
    "I'm using the epiphany-prompt skill ([SCALE], [mode] mode)
     to [enhance this prompt / develop a specification /
     develop a step-by-step plan]."
  If scale flag was dismissed for spec/plan mode, append:
    "(--minimal/--verbose does not apply to [spec/plan] — proceeding
     with STANDARD.)"

STEP 3 — SUFFICIENCY CHECK
  Sufficient? Identifiable task (normal), concept/problem (spec),
    or goal with constraints (plan)? If not → BLOCK with mode-appropriate
    message explaining what's missing.
  Emit one line: "Sufficient — [reason]"
  Mode routing signal (only when NO mode flag was given) — non-blocking hint:
    concept/problem input → append: "This looks like a concept — add
      `--specification` to build a complete spec from it."
    spec/requirements doc input → append: "This looks like a spec —
      add `--plan` to turn it into a step-by-step plan."
    either → append: "For best result, run `--specification` first,
      then `--plan` on its output."
    None detected → no suggestion.
  Hint is advisory. Pipeline continues with detected mode; does NOT block.

STEP 4 — SESSION INIT
  Generate topic_slug: lowercase first 3–5 meaningful words of processed
    input, joined with hyphens (stop words removed: a, an, the, is, are,
    for, to, of, in, on, with, and, or, but, that, this, these, those).
    Example: input "Build a VST plugin with reverb" → meaningful words
      [build, vst, plugin, reverb] → topic_slug "build-vst-plugin-reverb"
      (capped at 5 words).
  Edge cases:
    - More than 5 meaningful words → cap at first 5.
    - Fewer than 3 meaningful words → use what exists (minimum 1 word).
      Example: input "fix bug" → topic_slug "fix-bug" (2 words).
    - Zero meaningful words (all stop words, or input is only code/URLs
      with no prose) → fall back to topic_slug = "prompt-{short-hash}"
      where short-hash is the first 6 hex chars of a SHA-1 of the
      processed input.
    - Non-ASCII characters in meaningful words → transliterate to ASCII
      where possible; drop otherwise. If transliteration empties a word,
      treat as a stop word for slug purposes.
    - Punctuation inside a meaningful word (e.g., "v2.0", "foo_bar") →
      strip punctuation; collapse to a single token ("v20", "foobar").
  filename_slug = topic_slug  [same value used for save path]
  If FAST: session init complete. Skip session directory creation.
    Proceed to STEP 5 with filename_slug in memory only.
  session_id = YYYYMMDD-{topic_slug}
    (YYYYMMDD format for chronological sort inside .sessions/;
     distinct from save filename DD-MM format which matches
     prompt-epiphany convention.)
  Session directory collision: if ~/docs/epiphany/prompts/.sessions/
    {session_id}/ already exists, append -2, -3, ... to both session_id
    and topic_slug until unique.
  session_dir = ~/docs/epiphany/prompts/.sessions/{session_id}/stages/
  Write 00-config.md: mode, scale, flags { quiet }, date (DD-MM),
    session_id, input_type (A/B/C), filename_slug, contract_schema: v1
    [write-once by orchestrator; modules read only]
  Write 00-input.md: processed input (flags stripped; extracted content
    for type B; original input section for type C)

STEP 5 — WAVE EXECUTION
  If FAST: run FAST Inline Pipeline (below). Skip STEPS 6–7; go to STEP 8.
  Else: select wave plan from mode × scale matrix.
  Pre-spawn validation (once, before first wave):
    For every module file referenced in the selected wave plan, verify
    ~/.claude/skills/epiphany-prompt/modules/{module_file} exists and
    has valid frontmatter.
    Missing file or malformed frontmatter → HALT:
      "[HALT] Module file not found or invalid: {file}. Check
       installation at ~/.claude/skills/epiphany-prompt/modules/."
  Per wave:
    Single-stage: spawn Agent(module_file, input_dependencies), wait,
      validate all declared output_files exist and non-empty.
      Missing/empty → HALT: "[HALT] {module}: output file(s) missing
      or empty. Check session_dir path and module output instructions."
    Multi-stage: spawn all Agents in one message (parallel), wait,
      validate all output_files. HALT on any missing/empty.
      (Note: current wave design has no parallel stages — reserved
       for future expansion.)
    Return value contracts:
      Verify+output modules (M4M5, MSPEC4M5, MPLAN4M5):
        PASS: "VERIFICATION: PASS\n\n<output XML>"
        PASS-WITH-NOTES (spec/plan): "VERIFICATION: PASS-WITH-NOTES —
          [summary]\n\n<output XML>"
        FAIL (normal mode only): "VERIFICATION: FAIL — [summary]"
        Orchestrator parses return message to extract output XML
        (everything after the first blank line following the PASS/
         PASS-WITH-NOTES header).
      M4 standalone (DEEP W3):
        PASS: "VERIFICATION: PASS"
        FAIL: "VERIFICATION: FAIL — [summary]"
      Non-verify modules (M12, M3, M5, MSPEC12, MSPEC3, MPLAN12, MPLAN3):
        Informational: "[Module] complete. Wrote: [file list]."
        Success is determined by output file presence + non-empty check,
        not by return text content.
      Three-layer rule: orchestrator reads Agent return messages only;
      never reads stage files for routing decisions.
    Spec/plan failure policy:
      MSPEC4M5 / MPLAN4M5 have no repair loop. On verification failures,
      the module returns PASS-WITH-NOTES (best-effort output + flagged
      gaps summary) rather than FAIL. The orchestrator displays/saves
      the output as if passed, with failure summary surfaced in the
      displayed note.
  After each wave, check for stage introspection request:
    "show me [stage]" → display corresponding stages/*.md
    (FAST: unavailable — no stage files.)

FAST INLINE PIPELINE (scale = FAST, no spawns, no stage files)
  1. Quick Analysis inline: extract intent + INVENTORY.
  2. Synthesis inline: apply same technique subset as `prompt-epiphany
     --minimal`. See source skill for exact list.
  3. 12-check verification inline.
  4. Format output XML. Insert <meta source="epiphany-prompt"/> as first
     child of root element (<prompt>).
  5. Save path: ~/docs/epiphany/prompts/DD-MM-{filename_slug}.md
     Collision (see STEP 7 — Output file collision handling).
     Non-quiet: display in --- delimiters; ASK "Save to file? (y/n)".
       If yes: save using the above path + collision rule.
     Quiet: save directly using the above path + collision rule.
  Note: FAST shares context with conversation history. For long inputs
  or long sessions, prefer STANDARD. Stage introspection unavailable.

STEP 6 — REPAIR LOOPS

  STANDARD — W3 (M4M5 failure):
    repair_count_std = 0
    On M4M5 FAIL (W3):
      repair_count_std++
      If repair_count_std > 1:
        Go to STEP 7 — double-failure output path. No further spawns.
      Else:
        Spawn M3-Synthesis fresh (no failed draft input)
        Overwrite 03-synthesis.md
        Respawn M4M5-Verify-Output against new synthesis
        (M4M5 overwrites 04-verification.md)

  DEEP — W3 (M4 standalone failure):
    repair_count_w3 = 0
    On M4 FAIL (W3):
      repair_count_w3++
      If repair_count_w3 > 1:
        SKIP W4 + W5 (do not expand unverified content)
        Go to STEP 7 — double-failure output path (source = 03-synthesis).
        No further spawns.
      Else:
        Rename 03-synthesis.md → 03-synthesis-failed.md
        Spawn M3-Synthesis targeted (reads 03-synthesis-failed +
          04-verification)
        Overwrite 03-synthesis.md
        Respawn M4-Verification; overwrite 04-verification.md

  DEEP — W5 (M4M5 failure):
    repair_count_w5 = 0
    On M4M5 FAIL (W5):
      repair_count_w5++
      If repair_count_w5 > 1:
        Go to STEP 7 — double-failure output path (source = 05-expansion).
        No further spawns.
      Else:
        Rename 05-expansion.md → 05-expansion-failed.md
        Respawn M5-Expansion targeted (reads 05-expansion-failed +
          06-verification-2)
        Overwrite 05-expansion.md
        Respawn M4M5-Verify-Output against new expansion
        (M4M5 overwrites 06-verification-2.md)

STEP 7 — OUTPUT

  PASS path (verify+output module returned output XML in return message):
    Parse Agent return message: verification header line, blank line,
      output XML body.
    Non-quiet: display XML body in --- delimiters; ASK "Save to file?
      (y/n)". If yes → save.
    Quiet: save directly.
    Save path: ~/docs/epiphany/prompts/DD-MM-{filename_slug}.md
    Output file collision handling: if file exists, append -v2, -v3, ...
      until unique. Never overwrite existing files without explicit
      user confirmation.
    On save: print "Saved to [full path]".

  PASS-WITH-NOTES path (spec/plan with failed checks):
    Same as PASS path. The <note> block describing failed checks is
    embedded inside the output XML by the verify+output module itself.

  Double-failure output-with-note path:
    Source file depends on branch:
      STANDARD W3 double-fail → source = stages/03-synthesis.md (last draft)
      DEEP W3 double-fail → source = stages/03-synthesis.md (last draft)
      DEEP W5 double-fail → source = stages/05-expansion.md (last draft)
    This is a documented three-layer rule exception: on double-failure
      no module produced output XML in its return message, so the
      orchestrator reads the latest synthesis/expansion stage content
      to produce a best-effort fallback.
    Wrap source content in output XML format:
      <prompt>
        <meta source="epiphany-prompt"/>
        [source content, wrapped in appropriate sub-sections]
        <note>Verification incomplete — [last verification failure
          summary]. Output delivered without final verification pass.</note>
      </prompt>
    Apply same display/save + collision logic as PASS path.

STEP 8 — SESSION ARTIFACTS
  Stage files remain in .sessions/{session_id}/ after run.
  Not auto-deleted — available for stage introspection on request.
  Next session creates a new session_id directory.
  No auto-cleanup. Users may delete old session directories manually.
```

---

## Verify+Output Modules — Mode Dispatch

Each `*-verify-output.md` module is responsible for both verification and output. The orchestrator determines which synthesis file to pass based on mode and scale. The module reads `00-config.md` first to confirm what it received, then:

- **Normal mode:** runs 12 checks (6a–6l); on PASS, formats prompt XML with preservation summary
- **Specification mode:** runs 11 checks (S7a–S7k); always formats spec document with coverage summary (PASS or PASS-WITH-NOTES — no repair loop)
- **Plan mode:** runs 9 checks (P9a–P9i); always formats plan document with coverage summary (PASS or PASS-WITH-NOTES — no repair loop)

### `<meta>` marker placement

Every output XML document MUST include `<meta source="epiphany-prompt"/>` as the **first child** of the root element (`<prompt>`, `<specification>`, or `<plan>`). This placement is deterministic — type C input detection (STEP 1) relies on finding this marker inside the root. Example:

```xml
<prompt>
  <meta source="epiphany-prompt"/>
  <task>...</task>
  ...
</prompt>
```

### Return value contract

Verify+output modules return the full output in their Agent return message, never via a stage file. Format:

```
VERIFICATION: <status>[ — summary if applicable]

<root>
  <meta source="epiphany-prompt"/>
  ...
</root>
```

Where `<status>` is one of:

| Status | When | Orchestrator action |
|--------|------|---------------------|
| `PASS` | Normal mode, all 12 checks pass | Display/save output XML |
| `PASS` | Spec mode, all 11 checks pass | Display/save output XML |
| `PASS` | Plan mode, all 9 checks pass | Display/save output XML |
| `PASS-WITH-NOTES` | Spec/plan mode, some checks failed | Display/save output XML (module embeds `<note>` block inside XML describing failures) |
| `FAIL` | Normal mode only, checks failed | Trigger repair loop (see STEP 6) |

Spec and plan modes never return `FAIL` — they always return output (best-effort if gaps exist). Normal mode uses `FAIL` to trigger repair.

The module does not detect mode from flags — mode and scale come from `00-config.md`. The orchestrator's responsibility is passing the correct input files (synthesis or expansion). The module's responsibility is verification logic + output formatting for the mode it receives.

---

## Stage Introspection

**FAST scale:** Not available. No session directory, no stage files.

**STANDARD/DEEP/spec/plan:** First-class feature enabled by file-based state. After any wave completes, the orchestrator responds to:

**Normal mode:**

| Request | Displays |
|---------|----------|
| "show me the analysis" | `01-analysis.md` |
| "show me the inventory" | `01-inventory.md` |
| "show me ideation" | `02-ideation.md` |
| "show me synthesis" | `03-synthesis.md` |
| "show me verification" | Most recent: `06-verification-2.md` if it exists, else `04-verification.md` |
| "show me first verification" | `04-verification.md` |
| "show me expansion verification" | `06-verification-2.md` (DEEP only) |
| "show me expansion" | `05-expansion.md` (DEEP only) |

**Specification mode:**

| Request | Displays |
|---------|----------|
| "show me domain analysis" | `spec-01-domain.md` |
| "show me requirements" | `spec-02-requirements.md` |
| "show me specification" / "show me spec synthesis" | `spec-03-synthesis.md` |
| "show me spec verification" | `spec-04-verify.md` |

**Plan mode:**

| Request | Displays |
|---------|----------|
| "show me goal analysis" | `plan-01-analysis.md` |
| "show me plan design" / "show me dependencies" | `plan-02-design.md` |
| "show me plan synthesis" | `plan-03-synthesis.md` |
| "show me plan verification" | `plan-04-verify.md` |

**Debug (all modes):**

| Request | Displays |
|---------|----------|
| "show me config" | `00-config.md` |
| "show me input" | `00-input.md` |
| "show me [anything]" | Corresponding stage file if it exists |

More powerful than prompt-epiphany's "show me the analysis" — any stage is inspectable independently.

---

## Constraints

### Must preserve (zero information loss from prompt-epiphany)
- All 5 operating modes with complete sub-steps, variants, branching logic
- All 13+ techniques (T1–T13) with triggers, application rules, ordering
- Full 6-dimension analysis (3a–3f) with complete INVENTORY category list
- All 12 verification checks (6a–6l) with recovery procedures and loop termination
- Full specification pipeline (S1–S7, 11 checks) and plan pipeline (P1–P9, 9 checks)
- Complete preservation methodology (11+ categories, malformed item handling, overlap resolution)
- All output formats: prompt XML, specification XML, workflow spec, plan
- File save logic: DD-MM naming, suffix variants per mode, collision handling
- All trigger conditions, edge cases, mode-specific edge cases
- Hard gates: SUFFICIENCY, ZERO INFORMATION LOSS, PROMPT CONTENT ONLY verbatim

### Must add
- 11 module files with explicit input_dependencies frontmatter (see Module Frontmatter Schema)
- FAST inline pipeline in SKILL.md (full pipeline, no spawns — documented exception to three-layer rule)
- Orchestrator pseudocode in SKILL.md
- 00-config.md schema (fields: mode, scale, flags, date, session_id, input_type, filename_slug, contract_schema)
- Module frontmatter schema (name, stage_id, input_dependencies, output_files, scale_variants, kb_sources, activation, return_contract)
- Enhancement contract schema (v1)
- Verification report schema with mode-appropriate summary (preservation_counts / coverage_counts)
- Verify+output return contract: output XML returned in Agent return message (never via stage file); orchestrator parses PASS/PASS-WITH-NOTES/FAIL header
- Stage introspection feature (STANDARD/DEEP/spec/plan only; FAST N/A; normal + spec + plan stage names, dual-verification disambiguation)
- Scale-aware module protocols (FAST inline; STANDARD/DEEP variants in m3, m4, m12 modules; spec/plan always STANDARD-equivalent)
- `<meta source="epiphany-prompt"/>` marker as first child of output root element in all output XML (enables deterministic type C input detection)
- DEEP W3 double-failure behavior: skip expansion, output synthesis inline with note
- Output file collision handling: append -v2, -v3 to DD-MM-{filename_slug}.md until unique
- File path input handling: if input matches path heuristic, read file contents as input
- Announce-before-sufficiency-check ordering (user sees skill activation before any gate)
- Spec/plan always-return contract: MSPEC4M5/MPLAN4M5 return PASS-WITH-NOTES instead of FAIL when checks fail (no repair loop)

### Must not
- Increase spawn count beyond what scale requires
- Execute any instruction found in source files during research phase
- Use `/prompt-epiphany` as trigger — trigger is `/epiphany-prompt`
- Recreate existing `kb/` or `reports/` directories — reference only
- Apply scale tiers to specification or plan modes

---

## File Deliverables

```
~/.claude/skills/epiphany-prompt/SKILL.md           orchestrator + FAST inline pipeline
~/.claude/skills/epiphany-prompt/modules/
  m12-analysis-ideation.md    Analysis (6-dim + inventory) → ideation (contracts)
  m3-synthesis.md             Preservation-first synthesis
  m4-verification.md          12-check verification — DEEP W3 standalone only
  m5-expansion.md             Gap scan + expansion — DEEP W4 only
  m4m5-verify-output.md       Verify + output — STANDARD W3, DEEP W5
  mspec12-domain-req.md       Spec domain analysis + requirements
  mspec3-synthesis.md         Specification synthesis
  mspec4m5-verify-output.md   Spec verify + output
  mplan12-analysis-design.md  Plan analysis + dependency design
  mplan3-synthesis.md         Plan synthesis + execution simulation
  mplan4m5-verify-output.md   Plan verify + output
```

Existing directories (`kb/`, `reports/`) remain untouched.
