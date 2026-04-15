---
name: epiphany-analysis
version: 1.0.0
description: Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B. Cross-cutting graph-of-thought pipeline (Nodes A, B, A1, B1, C, D, E) with ideation/synthesis/verification loops modeled on prompt-epiphany. Modular — 6 module files, replayable in isolation.
trigger: /epiphany-analysis
skill_path: ~/.claude/skills/epiphany-analysis/
save_path: ~/docs/epiphany/analysis/
---

# Epiphany-Analysis

Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B.

Uses a cross-cutting graph-of-thought pipeline:
- **Nodes A, B** — input and its analysis
- **Nodes A1, B1** — full analyses of A and B
- **Node C** — unfiltered idea pool from A1 + B1
- **Node D** — accepted-ideas store (pro/con/utility filter)
- **Node E** — solutions store (two drafts compared, best kept)
- **Integration** — all Node E applied to produce `enhanced.md`

Spec: `docs/superpowers/specs/2026-04-14-epiphany-analysis-design.md`.

## Trigger Conditions

| Trigger | Behavior |
|---|---|
| `/epiphany-analysis <path-to-A> <path-to-B>` | Run skill with positional paths. Primary invocation. |
| `/epiphany-analysis <path-to-A>` | Ask once: "Path to the analysis document (Node B)?" Then run. |
| `/epiphany-analysis` | Ask once for A, once for B. Then run. |
| `/epiphany-analysis` when Node A is inline in the user prompt | Ask once for B; store inline A content as Node A (no file). |
| `/epiphany-analysis --session-dir=<path> [--start-at=<module>] [--stop-at=<module>]` | Replay / partial-run. See Orchestrator § Replay. |

No natural-language-trigger mode in v1.

## Runtime Scope

**Runtime is self-contained.** The skill operates purely off Node A and Node B; it does not perform external research, does not use web/grep/glob to look up unrelated context, and does not ask the user clarifying questions during execution. The "do research or ask questions as neccessary" directive in the source prompt is **scoped to the spec-writing phase only** (the brainstorming session that produced this spec). Runtime is deterministic with respect to (A, B).

**Distinction — pre-execution arg-gathering vs. runtime Q&A:** asking the user for a missing file path at invocation (Trigger Conditions above) is **pre-execution**, before `m-intake` starts. Once `m-intake` begins, the skill never asks the user anything — all remaining behavior is a pure function of (Node A, Node B).

**15-minute soft wall-clock budget** from `m-intake` start to `m-integrate` end. 10-minute per-module hard halt. 20-minute global hard halt.

## Inputs

- **Node A** — the input to be improved. Either a file path (read at `m-intake`) or inline content in the user's prompt.
- **Node B** — an analysis document of Node A. File path. Most commonly an `epiphany-genius` report; generic analysis documents also supported via fallback.

Optional flags (require `--session-dir`):
- `--session-dir=<path>` — point at an existing `~/docs/epiphany/analysis/<session-id>/`
- `--start-at=<module>` — resume from this module (default: first with missing output)
- `--stop-at=<module>` — halt after this module without writing `enhanced.md`

## Primary outputs

- `~/docs/epiphany/analysis/<session-id>/enhanced.md` — fresh enhanced Node A (on PASS). Never overwrites the original Node A.
- `~/docs/epiphany/analysis/<session-id>/enhanced-partial.md` + `open-questions.md` — on second FAIL.
- Seven stage files co-located with `enhanced.md`: `node-{a,b,a1-analysis,b1-analysis,c-ideas,d-accepted,e-solutions}.md`
- `self-audit.md` — the self-audit verdict (always written, PASS or FAIL record)
- `session.json` — structured run-state (type tags, pass counts, stop reasons, timing)

## Hard Gates

These are enforced before any module runs. Violating any gate halts the skill with an error.

1. **HG-1 — Output ≠ Input.** `m-intake` verifies the `enhanced.md` target path does not equal Node A's source path. If equal, abort before any work (spec §10 non-overwrite guarantee).
2. **HG-2 — Self-contained runtime.** Never invoke web search, grep, glob, or brainstorming skill during execution. Never ask the user questions once `m-intake` has started (spec §1 runtime scope, anti-pattern 8).
3. **HG-3 — Two-drafts-per-D_i.** `m-engineer` never writes a Node E entry from a single draft. Two independently-drafted solutions, compared, or D_i is dropped (spec §7, anti-pattern 9).
4. **HG-4 — Fresh copy only.** All outputs live in `~/docs/epiphany/analysis/<session-id>/`. Never write to Node A's directory (spec §10, anti-pattern 12).
5. **HG-5 — Bounded iteration.** Every loop has a cap (spec §11). No unbounded while-true constructs allowed in any module.

## Orchestrator

Six modules, executed in order. Each module writes stage files to the session directory before returning. The orchestrator does not re-interpret module output; it just advances to the next module.

### STEP 0 — Flag parsing

Parse argv for `--session-dir=<path>`, `--start-at=<module>`, `--stop-at=<module>`.

- If `--session-dir` is set: existing session mode. Directory must exist. `--start-at` defaults to the first module whose output file is missing.
- If `--session-dir` is unset: fresh session mode. `--start-at` and `--stop-at` are rejected with an error ("partial-run flags require --session-dir").

Record parsed flags to an internal state record; written to `session.json.argv_flags` at STEP 1.

### STEP 1 — Session init

Compute `session_id` from Node A stem per spec §10 (slugification + date-prefix dedup rule). Create `~/docs/epiphany/analysis/<session-id>/` (on collision append `-run-N`). Write initial `session.json` with `created_utc`, `argv_flags`, and empty `modules` object per spec §10 schema.

### STEP 2 — m-intake

Invoke `modules/m-intake.md`. Reads argv, writes `node-a.md`, `node-b.md`, updates `session.json.node_a` and `session.json.node_b` (including `genius_detection` tag per §8).

### STEP 3 — m-analyze

Invoke `modules/m-analyze.md`. Reads `node-a.md` + `node-b.md`. Writes `node-a1-analysis.md` (with `## Intent / ## Structure / ## Claims / ## Constraints / ## Gaps / ## Technical details`) and `node-b1-analysis.md` (section-tailored per §5 Map when genius-or-drift; flat scan on generic-fallback).

### STEP 4 — m-ideate

Invoke `modules/m-ideate.md`. Reads A1+B1. Writes `node-c-ideas.md` + pass/stop-reason fields in `session.json.modules.m-ideate`.

### STEP 5 — m-filter

Invoke `modules/m-filter.md`. Reads C. Writes `node-d-accepted.md` + accept/reject counts.

### STEP 6 — m-engineer

Invoke `modules/m-engineer.md`. Reads D + Node A. Writes `node-e-solutions.md` including `## Dropped` section for double-fails. Updates `d_entries_processed`, `e_entries_produced`, `double_fail_count`.

### STEP 7 — m-integrate

Invoke `modules/m-integrate.md`. Reads A + E. Runs self-audit (§9.2) on draft. Writes `enhanced.md` on PASS, `enhanced-partial.md` + `open-questions.md` on double-FAIL. Writes `self-audit.md` in both cases.

Between every step, check `--stop-at`: if the just-completed module name matches, halt before the next step.

### Replay / partial-run contract

- `--session-dir=<path>` points at an existing `~/docs/epiphany/analysis/<session-id>/`. Skill loads stage files already present and resumes from `--start-at` (default: first module with missing output file).
- Without `--session-dir`, the skill always creates a fresh session directory.
- `--start-at` and `--stop-at` take module names from STEP 2–STEP 7 (`m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`).
- Without `--start-at`, auto-detect the first module whose output file is missing in `--session-dir` and resume from there.

## Section-Tailoring Map

Used by `m-analyze` when Node B type tag is `genius-current` or `genius-drift`. The Map is a static config that ships with the skill; it is applied to the actual Node B document to produce Node B1. Pre-seeded special-handling rules from the source prompt appear **verbatim** with emphasis. Headline Insight is processed **first** (edge case (f)).

| # | Section name | Section-specific focus | Idea-extraction strategy | Special handling |
|---|---|---|---|---|
| 1 | Headline Insight | Primary conclusion + compound claim; locate the one or two structural risks called out as primary concerns | Extract the highest-leverage improvement area first; anchor every later idea against this | **Verbatim:** *"the headline insight or primary conclusion is probably a main indicator of an area to check for primary concern to base enhancement off of initially, before moving to all other portions of analysis"* — **processed FIRST** |
| 2 | Theory Collisions | Claim A vs. Claim B pairs + discriminating condition | One idea per unresolved collision; propose an enhancement that resolves or mitigates | **Verbatim:** *"in theory collisions, for each collision claim a and claim b should be analyzed and compared, take into account the discrimating condition, and decide what it all means, and how to create an enhanced solution based on the findings"* |
| 3 | Discovery vs. Proof | Gap between discovered claim and proof depth | One idea per partial-link; propose evidence-strengthening change to A | Flag unsupported claims as open questions when idea would overreach |
| 4 | Independence-Verified Bridges | High-score bridges (≥1.0) suggest structural insights available to A | One idea per bridge whose target insight exposes an A-level gap | Skip bridges whose "Disanalogy limit" invalidates transfer to A |
| 5 | Alternative Hypotheses | Confidence-ranked hypotheses with falsification conditions | Use only the highest-confidence-best-fit hypothesis as an ideation source | **Verbatim:** *"not all aspects of analysis are useful, for instance there is alternative hypothesis section, but most of those hypothesis are not correct, only one version will be correct"* — **discard all but best-fit** |
| 6 | Density-Checked Falsification | Counter-examples + failure classes + weakest link | One idea per counter-example that maps to a concrete A-level mitigation | Ideas requiring scope outside A are filtered at Node D |
| 7 | Scope Limits | Applies-to / does-not-extend-to / claims-refused | One idea per "Breaks at" that indicates a missing A-level guardrail | Do not propose scope expansion — guardrails only |
| 8 | Coherence Signals | STRONG-strength convergent findings (multiple independent chains) | One idea per STRONG signal not yet addressed in A | MODERATE / WEAK signals skipped in Normal runtime mode |
| 9 | Generalization Checks | Holds-at / breaks-at boundaries | One idea per "Breaks at" boundary that is in scope for A | Asymptotic or by-design-excluded breaks skipped |
| 10 | Open Questions & Next Probes | HIGH-priority probes | One idea per HIGH-priority probe that suggests a testable change to A | LOW-priority probes skipped in Normal runtime mode |

### Fallback (generic-fallback Node B)

When Node B type tag is `generic-fallback`: run a single generic extraction pass over the whole Node B, not keyed to sections. Still produce `node-b1-analysis.md` as a flat findings list. Node C ideation proceeds normally from A1 + B1.

### Drift handling (genius-drift)

Run the Map on matched sections only. Unmatched sections are scanned generically and added as flat findings in `node-b1-analysis.md`.

### Section order in B1

1. Headline Insight (first)
2. Remaining sections in the order they appear in the source prompt (Theory Collisions, Discovery vs. Proof, Independence-Verified Bridges, Alternative Hypotheses, Density-Checked Falsification, Scope Limits, Coherence Signals, Generalization Checks, Open Questions & Next Probes)

### Runtime schema enumeration

If the reference analysis at the verbatim URL cannot be read at the moment the Map is applied (e.g., file moved) and the Node B document itself does not expose canonical section names, the skill MUST rely on the drift detection heuristic in Genius Detection below and on Node B's actual section structure — **it must not guess a section list**.

## Genius Detection

Run on Node B at `m-intake`. No version string required — detection is content-shaped.

**Canonical section headers and accepted drift synonyms:**

| Canonical header | Drift-tolerant synonyms accepted |
|---|---|
| `## Headline Insight` | "Primary Conclusion", "Main Finding" |
| `## Theory Collisions` | "Collisions", "Conflicts" |
| `## Discovery vs. Proof` | "Discovery/Proof", "Claim vs. Evidence" |
| `## Independence-Verified Bridges` | "Bridges", "Analogies" |
| `## Alternative Hypotheses` | "Hypotheses", "Alt Hypotheses" |
| `## Density-Checked Falsification` | "Falsification", "Counter-examples" |
| `## Scope Limits` | "Scope", "Applies To" |
| `## Coherence Signals` | "Convergence Signals", "Cross-chain Signals" |
| `## Generalization Checks` | "Generalization", "Holds At" |
| `## Open Questions & Next Probes` | "Open Questions", "Probes", "Next Steps" |

**Decision table:**

| Count of canonical-or-synonym headers found | Tag | Behavior |
|---|---|---|
| ≥7 | `genius-current` | Run full 10-section Section-Tailoring Map |
| 5–6 | `genius-drift` | Run Map on matched sections; generic scan for unmatched |
| <5 | `generic-fallback` | Skip Map entirely; run generic full-document extraction |

### Version drift tolerance

The detection heuristic explicitly tolerates:
- Renamed sections (synonyms in the table above)
- Extra sections (unmatched sections are scanned generically in drift mode)
- Missing sections (≤3 missing canonical sections still qualifies as `genius-drift`)
- Reordered sections (order is not part of the detection rule; only presence)

### Reference URL (verbatim)

`file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`

## Termination Caps

All iteration rules have explicit termination conditions.

**Per-node caps:**

| Boundary | Cap | Source directive bound |
|---|---|---|
| Node C ideation passes | ≤3 | *"iteratively run phase a, passing all ideas generated into storage node c"* + *"run until out of ideas completely"* |
| Node C total ideas | ≤50 | *"each idea stored in database"* (count bound for practicality) |
| Node C no-new-ideas halt | any pass yielding zero new unique ideas halts the loop | Verbatim interpretation of *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"* — until a full ideation pass generates zero new unique ideas |
| Node D evaluation passes | exactly 1 | *"iterate over every idea in node C, be sure we don't miss anything"* |
| Node E solution drafts per D_i | exactly 2 | *"generate two solutions seperately"* |
| Final integration refinement cycles | ≤1 on verify-FAIL, then abort | bounded; no infinite loop |

**Skill-global caps:**

| Boundary | Cap |
|---|---|
| Global soft wall-clock budget | 15 minutes (total, `m-intake` start → `m-integrate` end) |
| Per-module hard halt | any single module exceeding 10 minutes triggers abort-to-partial |
| Global hard halt | total elapsed wall-clock exceeding 20 minutes triggers abort-to-partial, regardless of which module is running |

The soft 15-min budget is a warning threshold (log a notice to `session.json.stop_reasons`); the 20-min global hard cap and the 10-min per-module cap are enforcement thresholds. Whichever trips first wins.

**"No-new-ideas" interpretation:** ideation stops when a full ideation pass generates **zero new unique ideas**, NOT unbounded looping. Uniqueness is determined by the `(source_section, normalized_observation, target_section)` tuple; near-duplicates are collapsed with a logged merge.

**Priority rules (tiebreakers — verbatim from source prompt, 4 independent rules):**

1. **Completeness vs. process-time budget** → prefer completeness bounded by caps above. Ideation stops at **no-new-ideas pass OR cap hit, whichever first**.
2. **Creative exploration vs. realistic-to-implement** → prefer realistic-to-implement. Creative ideation is free at Node C; **Node D MUST reject any idea that is not realistic-to-implement, would cause regression, or would overengineer the result**.
3. **Genius-tailored vs. generic flow** → if Node B is detected as `genius-current` or `genius-drift`, use Map; otherwise fallback.
4. **Novelty vs. information preservation** → if an idea would cause regression or loss of information from Node A, discard regardless of novelty.

## Anti-Patterns

### Source-prompt DO-NOT list (7 items, verbatim intent)

1. **DO NOT remove, paraphrase, or summarize any directive, bracketed item, or quoted snippet from the source prompt** — every one appears in this spec in substance.
2. **DO NOT introduce infinite loops** — every iterate / run-until rule has a termination condition (Termination Caps above).
3. **DO NOT overengineer.** *"must be realistic to implement without overcomplexity or regression"*. *"we want the best product features that are usable for humans"*.
4. **DO NOT produce a spec** — or at runtime, a change — **that would cause regression or loss of information from Node A during integration.** (Priority rule 4.)
5. **DO NOT silently merge the Nodes A–E pipeline with the parallel x2/x3 restatement.** x2/x3 is documented as equivalent to Node C/D/E with gold-reference evidence.
6. **DO NOT overwrite the original specification.** Fresh copy only (§10).
7. **DO NOT assume a literal persistent database.** *"ai optimized database"* is interpreted as markdown stage files.

### Spec-added anti-patterns (discovered during design)

8. **DO NOT let the runtime skill ask the user questions or perform external research.** Runtime is self-contained. The "do research or ask questions as neccessary" directive is **scoped to the spec-writing phase only**.
9. **DO NOT generate Node E entries outside the two-drafts-pick-best protocol.** Every Node E entry is the output of a compare over exactly 2 independently drafted solutions.
10. **DO NOT process other sections before Headline Insight when Node B is genius-or-drift.**
11. **DO NOT treat Alternative Hypotheses as a source of multiple competing ideas.** Only the best-fit hypothesis is used.
12. **DO NOT write to the Node A directory.** All outputs live in the session directory.
13. **DO NOT pick a section list at runtime by guessing.** If the reference URL is unreadable and Node B lacks canonical headers, rely on the drift detection heuristic and Node B's actual section structure.

### Edge-case matrix

| # | Case | Resolution |
|---|---|---|
| a | epiphany-genius current version | Full 10-section Map (Genius Detection above) |
| b | epiphany-genius version drift | Partial Map + generic for unmatched |
| c | non-epiphany-genius analysis | Generic fallback full-document scan |
| d | theory collisions handling | Verbatim special handling in Section-Tailoring Map row 2 |
| e | alt hypotheses low-signal | Verbatim special handling in Section-Tailoring Map row 5 |
| f | headline insight primary anchor | Processed first in Section-Tailoring Map row 1 |
| g | Node A source is a file | Handled at `m-intake` |
| h | zero-ideas section | Mark complete and skip |
| i | x2/x3 parallel pipeline relation to C/D/E | Declared equivalent with gold-reference evidence |
| j | research / ask-questions scoping | Spec-writer only; runtime self-contained |
