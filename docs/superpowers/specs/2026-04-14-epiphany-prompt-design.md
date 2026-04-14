# epiphany-prompt — Design Specification
**Date:** 2026-04-14
**Status:** Approved for implementation planning
**Trigger:** `/epiphany-prompt`
**Install path:** `~/.claude/skills/epiphany-prompt/`

---

## Goal

Build `epiphany-prompt` — a modular, subagent-orchestrated prompt enhancement skill that retains the complete `prompt-epiphany` methodology (all 5 modes, all 13+ techniques, all verification gates, zero-information-loss guarantee) but restructures delivery as a modular pipeline with swappable stage files, scale-aware depth, and per-stage specialization that produces meaningfully better output than the monolithic version.

**What makes it better than prompt-epiphany:**
- Each stage runs in an isolated Agent subagent with full context budget for its specific task
- Stages are swappable — replace `modules/m2-ideation.md` with a better version, nothing else changes
- Three depth scales (FAST/STANDARD/DEEP) map to existing flags, each unlocking genuinely different capabilities
- Formal document handoffs (enhancement contracts, verification reports) create explicit quality gates between stages
- Stage introspection: any stage's output is inspectable on request

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
                      Never runs stage logic inline.

modules/*.md          Stage protocols. Each spawned as an isolated Agent
                      subagent. One file per stage. Replace the file to
                      upgrade the stage. Declares input_dependencies in
                      frontmatter.

stages/ (session)     File-based state. Each subagent reads declared
                      input files from stages/, writes one output file.
                      Orchestrator never reads stage files during pipeline
                      execution for routing decisions — it reads module
                      return values instead. Exception: stage introspection
                      reads and displays stage files on user request after
                      a wave completes (display-only, not for routing).
```

Session directories: `~/docs/epiphany/prompts/.sessions/{session_id}/stages/`
- `session_id = YYYYMMDD-[topic-slug]`
- Stage files persist after run (available for stage introspection)
- New session creates a new session_id directory; old ones are not auto-deleted

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
| `--minimal` | FAST | 4 agents |
| *(default)* | STANDARD | 5 agents (7 if W4 repair: +M3 +M4) |
| `--verbose` | DEEP | 7 agents (+2 per repair loop; max 11 if both W4+W6 repair) |

`--quiet` is display-only — does not affect scale or wave plan.

Scale does not apply to specification or plan modes — both always run STANDARD regardless of flag. Announce if flag was passed: "`--minimal`/`--verbose` does not apply to [spec/plan] mode — proceeding with STANDARD."

---

## Module Set — 14 files

```
~/.claude/skills/epiphany-prompt/modules/

Normal mode (5 files):
  m1-analysis.md          6-dimension analysis + INVENTORY extraction
  m2-ideation.md          Enhancement contracts, DEEP adds anti-conformity pass
  m3-synthesis.md         Preservation-first synthesis, DEEP adds self-critique
  m4-verification.md      12-check battery, scale-aware repair behaviour
  m5-expansion.md         DEEP only — gap scan + targeted expansion synthesis

Specification mode (4 files):
  mspec-1-domain.md       S2–S3: domain analysis + concept decomposition
  mspec-2-requirements.md S4: requirement extraction (SHALL/SHOULD/MAY)
  mspec-3-synthesis.md    S5: specification synthesis
  mspec-4-verify.md       S6–S7: completeness audit + 11-check verification

Plan mode (4 files):
  mplan-1-analysis.md     P2–P3: goal analysis + action decomposition
  mplan-2-design.md       P4–P5: dependency mapping + safeguard design
  mplan-3-synthesis.md    P6–P7: plan synthesis + execution simulation
  mplan-4-verify.md       P8–P9: gap audit + 9-check verification

Shared (1 file):
  m5-output.md            Display routing, file save, summary — used by all modes
```

---

## Wave Plans

### Normal mode

```
FAST  (--minimal)       STANDARD  (default)     DEEP  (--verbose)

W1: M1-Analysis         W1: M1-Analysis          W1: M1-Analysis
W2: M3-Synthesis        W2: M2-Ideation          W2: M2-Ideation
W3: M4-Verification     W3: M3-Synthesis         W3: M3-Synthesis
W4: M5-Output           W4: M4-Verification      W4: M4-Verification
                        W5: M5-Output            W5: M5-Expansion
                                                 W6: M4-Verification*
                                                 W7: M5-Output

4 agents                5 agents                 7 agents
```

*W6 re-runs M4 against `05-expansion.md` output. Same module file, different input_dependencies passed by orchestrator.

### Specification and plan modes

```
SPECIFICATION                   PLAN

W1: MSPEC-1-Domain              W1: MPLAN-1-Analysis
W2: MSPEC-2-Requirements        W2: MPLAN-2-Design
W3: MSPEC-3-Synthesis           W3: MPLAN-3-Synthesis
W4: MSPEC-4-Verify              W4: MPLAN-4-Verify
W5: M5-Output                   W5: M5-Output

5 agents                        5 agents
```

### Mode × Scale matrix

| | FAST | STANDARD | DEEP |
|---|---|---|---|
| **normal** | M1→M3→M4→M5 | M1→M2→M3→M4→M5 | M1→M2→M3→M4→M5-exp→M4→M5 |
| **specification** | → STANDARD | MSPEC-1→2→3→4→M5 | → STANDARD |
| **plan** | → STANDARD | MPLAN-1→2→3→4→M5 | → STANDARD |
| **spec then plan** | → STANDARD both | spec waves → confirm → plan waves | → STANDARD both |
| **+ `--quiet`** | display suppressed, file saved | same | same |

---

## Information Architecture

### Stage files

**Normal mode:**
```
stages/00-config.md         mode, scale, flags { quiet }, date (DD-MM),
                            session_id, input_type (A/B/C),
                            filename_slug (generated inline at STEP 2),
                            contract_schema: v1  [write-once by orchestrator]

stages/00-input.md          processed original input (flags stripped,
                            XML/prior-output extracted if type B/C)

stages/01-analysis.md       full 6-dimension analysis (3a–3f)
stages/01-inventory.md      INVENTORY checklist only — extracted from
                            01-analysis, standalone preservation checklist

stages/02-ideation.md       enhancement contracts (see schema below)
stages/03-synthesis.md      enhanced prompt draft
                            (STANDARD repair: overwritten in place;
                             DEEP repair: saved as 03-synthesis-failed.md first)
stages/04-verification.md   12-check results (see schema below)
stages/05-expansion.md      DEEP only — gap scan findings + expanded output
stages/06-verification-2.md DEEP only — M4 re-run against expansion output
stages/output-meta.md       Written by M5-Output (all modes) — contains
                            target_filename (resolved save path).
                            Orchestrator reads this after M5 to confirm save path.
```

**Specification mode** (00-config.md and 00-input.md shared):
```
stages/spec-01-domain.md    MSPEC-1 output: domain analysis + decomposition
stages/spec-02-requirements.md MSPEC-2 output: extracted requirements
stages/spec-03-synthesis.md MSPEC-3 output: specification document
stages/spec-04-verify.md    MSPEC-4 output: completeness audit + 11 checks
```

**Plan mode** (00-config.md and 00-input.md shared):
```
stages/plan-01-analysis.md  MPLAN-1 output: goal analysis + decomposition
stages/plan-02-design.md    MPLAN-2 output: dependency map + safeguards
stages/plan-03-synthesis.md MPLAN-3 output: plan document + simulation
stages/plan-04-verify.md    MPLAN-4 output: gap audit + 9 checks
```

### Module input_dependencies

| Module | Reads |
|--------|-------|
| M1 Analysis | `00-config` + `00-input` |
| M2 Ideation | `00-config` + `00-input` + `01-analysis` |
| M3 Synthesis (FAST) | `00-config` + `00-input` + `01-analysis` + `01-inventory` |
| M3 Synthesis (STANDARD/DEEP initial) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` |
| M3 Synthesis (STANDARD repair) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` + `04-verification` (no failed draft) |
| M3 Synthesis (DEEP repair) | `00-config` + `00-input` + `01-analysis` + `01-inventory` + `02-ideation` + `03-synthesis-failed` + `04-verification` |
| M4 Verification | `00-config` + `00-input` + `01-inventory` + `03-synthesis` |
| M4 Verification (W6 DEEP) | `00-config` + `00-input` + `01-inventory` + `05-expansion` |
| M5 Expansion | `00-config` + `00-input` + `01-inventory` + `03-synthesis` (latest) |
| M5 Output (FAST/STANDARD) | `00-config` + `03-synthesis` + `04-verification` |
| M5 Output (DEEP expanded) | `00-config` + `05-expansion` + `06-verification-2` |
| M5 Output (DEEP no-op exp) | `00-config` + `03-synthesis` + `06-verification-2` |
| M5 Output (spec) | `00-config` + `spec-03-synthesis` + `spec-04-verify` |
| M5 Output (plan) | `00-config` + `plan-03-synthesis` + `plan-04-verify` |
| MSPEC-1 Domain | `00-config` + `00-input` |
| MSPEC-2 Requirements | `00-config` + `00-input` + `spec-01-domain` |
| MSPEC-3 Synthesis | `00-config` + `00-input` + `spec-01-domain` + `spec-02-requirements` |
| MSPEC-4 Verify | `00-config` + `00-input` + `spec-01-domain` + `spec-02-requirements` + `spec-03-synthesis` |
| MPLAN-1 Analysis | `00-config` + `00-input` |
| MPLAN-2 Design | `00-config` + `00-input` + `plan-01-analysis` |
| MPLAN-3 Synthesis | `00-config` + `00-input` + `plan-01-analysis` + `plan-02-design` |
| MPLAN-4 Verify | `00-config` + `00-input` + `plan-01-analysis` + `plan-02-design` + `plan-03-synthesis` |

### Segregation rationale

- M2 Ideation never sees a synthesis draft — keeps enhancement design generative, not editorial
- M4 Verification never sees M2 Ideation reasoning — checks output quality, not intent
- STANDARD repair: M3 receives no failed draft — clean regeneration, no anchoring
- DEEP repair: M3 receives failed draft deliberately — targeted surgical fix only

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

---

## Per-Module Enhancements

### M1 Analysis — what isolation enables

- Full context budget for analysis only
- Two formal outputs: `01-analysis.md` (full findings) + `01-inventory.md` (structured standalone checklist). Verification reads the checklist directly — no parsing required.
- **DEEP: Enhancement Opportunity Scoring.** After identifying weaknesses, M1 scores each by impact potential before handing off to M2-Ideation:
  - high = weakness would cause the AI to fail the stated task
  - medium = weakness would reduce output quality or completeness
  - low = stylistic issue with no functional impact
- M2-Ideation uses scores to prioritize enhancement budget.

### M2 Ideation — what isolation enables

- Full context for creative enhancement design with no pressure to begin synthesizing
- Output is enhancement contracts (formal, structured) — M3 executes, doesn't rediscover
- **DEEP: Anti-Conformity internal second pass.** First pass: T1–T13 gap analysis (systematic). Second pass: Anti-Conformity — "what would a contrarian enhancer do that the systematic pass missed?" Both passes internal to M2 (single agent, two reasoning passes — no sub-spawning). Merged output prioritized by M1 opportunity scores.
- Clean upgrade path: replace `m2-ideation.md` with any new ideation method; M3 receives the same contract schema (v1) and requires no changes.

### M3 Synthesis — what isolation enables

- Full context for synthesis with no competing analysis or verification context
- Executes enhancement contracts rather than rediscovering enhancement rationale mid-synthesis
- Preservation-first protocol more rigorous: full budget to place every INVENTORY item before adding any enhancement
- **DEEP: Iterative self-critique.** Draft → internal self-critique against INVENTORY + contracts → targeted revision → final. Two reasoning passes within single M3 agent. No separate spawn.
- FAST quality floor: applies T1, T2, T3, T5, T7 directly from Quick Analysis (Intent + Inventory). Matches prompt-epiphany minimal mode quality — no regression.

### M4 Verification — what isolation enables

- Fresh eyes on synthesis output — no memory of how it was produced
- **STANDARD repair:** M3 re-spawned fresh (no failed draft) — clean regeneration, not a patch
- **DEEP repair:** M3 re-spawned targeted (failed draft + failure report) — surgical fix using exact failure details from formal verification report. Only possible because failures are a structured document, not prose.
- Max 1 repair attempt at any scale; fail twice → output with note (same rule as prompt-epiphany source)
- Writes structured verification report as formal output — M5-Output reads preservation counts as data

### M5 Expansion — what isolation enables (DEEP only)

- Fresh eyes on verified synthesis — no memory of synthesis reasoning
- Runs against verified output (post-M4), not raw synthesis — expands content already known to be preservation-complete
- Clean no-op path: if gap scan finds nothing thin, passes through with note; W6 M4-Verification runs unchanged against the pass-through output
- Gap scan findings written to `05-expansion.md` — inspectable via stage introspection

### Spec and Plan modules — what isolation enables

- Formal document handoffs at each stage boundary: Decomposition output → Requirements must trace to it
- Execution Simulation (MPLAN-3) has full context for the mental walkthrough
- Verification modules (MSPEC-4, MPLAN-4) see specification/plan without memory of how it was written

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
    --specification + --plan → confirm sequential run
    scale flag + spec/plan mode → ignore scale, announce dismissal
  Strip flags from input body. Mid-sentence flags = content.

STEP 1 — INPUT ROUTING + SUFFICIENCY (inline, no spawn)
  Detect input type:
    A — raw text (default)
    B — prompt-epiphany XML → root element contains <prompt> or <enhanced_prompt>
        with no <meta source="epiphany-prompt"/> marker
        → extract <task>, <context>, <constraints>
    C — prior epiphany-prompt output → contains <meta source="epiphany-prompt"/>
        → extract original input section, start fresh pipeline
        (Note: epiphany-prompt output includes this marker to enable type C detection)
  Sufficiency check: discernible task? If not → BLOCK.
  Emit one line: "Sufficient — [reason]"
  Mode routing signal (no flag given):
    concept/problem → suggest --specification
    spec/requirements doc → suggest --plan
    either → suggest full pipeline chain

STEP 2 — SESSION INIT
  Generate topic_slug: lowercase first 3–5 meaningful words of input,
    joined with hyphens (stop words: a, an, the, is, for, to, of, in, ...)
  session_id = YYYYMMDD-{topic_slug}
  Collision: if session_dir already exists, append -2, -3, etc. to
    both session_id and topic_slug until unique
    (e.g., build-prompt-skill → build-prompt-skill-2)
  filename_slug = topic_slug  [same value, stored in 00-config for M5-Output]
  session_dir = ~/docs/epiphany/prompts/.sessions/{session_id}/stages/
  Write 00-config.md: mode, scale, flags, date (DD-MM),
    session_id, input_type, filename_slug, contract_schema: v1  [write-once]
  Write 00-input.md: processed input

STEP 3 — ANNOUNCE
  "I'm using the epiphany-prompt skill ([SCALE], [mode] mode)
   to [enhance this prompt / develop a specification /
   develop a step-by-step plan]."

STEP 4 — WAVE EXECUTION
  Select wave plan from mode × scale matrix.
  Per wave:
    Single-stage: spawn Agent(module_file, input_dependencies), wait,
      validate output file exists and non-empty → HALT if missing or empty
      HALT format: "[HALT] {module}: output file missing or empty.
        Check session_dir path and module output instructions."
    Multi-stage: spawn all Agents in one message (parallel),
      wait for all, validate all output files → HALT if any missing or empty
      (Note: current wave design has no parallel stages — rule reserved for future expansion)
    M4 return value: M4 ends its run with "VERIFICATION: PASS" or
      "VERIFICATION: FAIL — [summary]". Orchestrator reads the Agent return
      message to decide whether to trigger repair. Orchestrator never reads
      04-verification.md directly (three-layer rule).
    MSPEC-4 / MPLAN-4 return value: same contract — "VERIFICATION: PASS"
      or "VERIFICATION: FAIL — [summary]". No repair loop for spec/plan modes.
      On FAIL: orchestrator passes the failure summary to M5-Output as context;
      output is delivered with flagged gaps noted in the summary line.
  After each wave, check for stage introspection request:
    "show me [analysis/ideation/synthesis/expansion/verification]"
      → display corresponding stages/*.md, then continue
    "show me the analysis" → display 01-analysis.md

STEP 5 — REPAIR LOOP (on M4 failure at W4)
  repair_count_w4 = 0
  On M4 failure (W4):
    repair_count_w4++
    If repair_count_w4 > 1 → proceed to output with note
    If FAST → no repair, proceed with note immediately
    If STANDARD → spawn M3-Synthesis fresh (no failed draft)
    If DEEP → save 03-synthesis-failed.md before overwriting;
      spawn M3-Synthesis targeted (with 03-synthesis-failed.md + 04-verification)
    Respawn M4-Verification against new synthesis
    Overwrite 03-synthesis.md and 04-verification.md

STEP 6 — EXPANSION (DEEP normal mode only)
  Spawn M5-Expansion (reads 00-input + 00-config + 01-inventory
    + 03-synthesis [latest, post-repair if applicable])
  If gap scan finds nothing thin → pass-through, note "already comprehensive"
  Spawn M4-Verification second run (W6):
    reads: 00-input + 01-inventory + 05-expansion
    writes: 06-verification-2.md
  Repair loop (W6), max 1 attempt:
    repair_count_w6 = 0
    On M4 failure (W6):
      repair_count_w6++
      If repair_count_w6 > 1 → proceed to output with note
      Save 05-expansion-failed.md before overwriting
      Respawn M5-Expansion targeted (with 05-expansion-failed.md + 06-verification-2)
      Respawn M4-Verification third run; overwrite 05-expansion.md and 06-verification-2.md

STEP 7 — OUTPUT
  Spawn M5-Output with scale-appropriate inputs (see dependency table)
  M5-Output:
    Reads filename_slug from 00-config.md (generated by orchestrator at STEP 2)
    Assembles target_filename: DD-MM-{filename_slug}.md
    Writes target_filename to stages/output-meta.md (write-once; does not
      modify 00-config.md — that file is orchestrator-owned)
    Non-quiet: display output in --- delimiters, print summary line,
      offer save → ~/docs/epiphany/prompts/DD-MM-name.md
    Quiet: save immediately, print "Saved to [path]" + summary line

STEP 8 — SESSION ARTIFACTS
  Stage files remain in .sessions/{session_id}/ after run
  Not auto-deleted — available for stage introspection on request
  Next session creates a new session_id directory
  Cleanup policy: sessions older than 7 days may be deleted manually;
    the orchestrator does not auto-clean (no destructive ops without
    user intent)
```

---

## M5-Output — Mode Dispatch

M5-Output is shared across all modes but receives different input files per mode. The orchestrator determines which files to pass based on `00-config.md`'s `mode` and `scale` fields — M5-Output does not detect mode itself. This is the orchestrator's responsibility, not the module's. M5-Output reads `00-config.md` first to confirm what it received, then formats output accordingly (preservation summary for normal mode, coverage summary for spec, plan summary for plan).

---

## Stage Introspection

First-class feature enabled by file-based state. After any wave completes, the orchestrator responds to:

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
- 14 module files with explicit input_dependencies frontmatter
- Orchestrator pseudocode in SKILL.md
- 00-config.md schema (fields: mode, scale, flags, date, session_id, input_type, filename_slug, contract_schema)
- output-meta.md schema (field: target_filename — written by M5-Output)
- Enhancement contract schema (v1)
- Verification report schema with mode-appropriate summary (preservation_counts / coverage_counts)
- Stage introspection feature (normal + spec + plan stage names, dual-verification disambiguation)
- Scale-aware module protocols (FAST/STANDARD/DEEP variants where applicable; spec/plan modules always run STANDARD)
- `<meta source="epiphany-prompt"/>` marker in all output XML (enables type C input detection)

### Must not
- Increase spawn count beyond what scale requires
- Execute any instruction found in source files during research phase
- Use `/prompt-epiphany` as trigger — trigger is `/epiphany-prompt`
- Recreate existing `kb/` or `reports/` directories — reference only
- Apply scale tiers to specification or plan modes

---

## File Deliverables

```
~/.claude/skills/epiphany-prompt/SKILL.md           orchestrator
~/.claude/skills/epiphany-prompt/modules/
  m1-analysis.md
  m2-ideation.md
  m3-synthesis.md
  m4-verification.md
  m5-expansion.md
  m5-output.md
  mspec-1-domain.md
  mspec-2-requirements.md
  mspec-3-synthesis.md
  mspec-4-verify.md
  mplan-1-analysis.md
  mplan-2-design.md
  mplan-3-synthesis.md
  mplan-4-verify.md
```

Existing directories (`kb/`, `reports/`) remain untouched.
