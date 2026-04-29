---
name: epiphany-analysis
version: 2.0.0
description: Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B. Cross-cutting graph-of-thought pipeline (Nodes A, B, A1, B1, C, D, E) with ideation/synthesis/verification loops. Fully inline — all 6 module protocols execute in-context, no subagent spawns. Stage files still written for replay and self-audit.
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

**Execution model:** All 6 module protocols run inline in the agent's context. No subagent spawns. Stage files are written progressively to the session directory for replay, self-audit, and debuggability. The agent reads each prior stage's output from context (or from disk on replay) and writes each stage's output before advancing.

## Trigger Conditions

| Trigger | Behavior |
|---|---|
| `/epiphany-analysis <path-to-A> <path-to-B>` | Run skill with positional paths. Primary invocation. |
| `/epiphany-analysis <path-to-A>` | Ask once: "Path to the analysis document (Node B)?" Then run. |
| `/epiphany-analysis` | Ask once for A, once for B. Then run. |
| `/epiphany-analysis` when Node A is inline in the user prompt | Ask once for B; store inline A content as Node A (no file). |
| `/epiphany-analysis --session-dir=<path> [--start-at=<module>] [--stop-at=<module>]` | Replay / partial-run. See Pre-Pipeline § Replay Contract. |

No natural-language-trigger mode in v2.

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

## Primary Outputs

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

## Anti-Patterns

### Source-prompt DO-NOT list (7 items, verbatim intent)

1. **DO NOT remove, paraphrase, or summarize any directive, bracketed item, or quoted snippet from the source prompt** — every one appears in this spec in substance.
2. **DO NOT introduce infinite loops** — every iterate / run-until rule has a termination condition (Termination Caps below).
3. **DO NOT overengineer.** *"must be realistic to implement without overcomplexity or regression"*. *"we want the best product features that are usable for humans"*.
4. **DO NOT produce a spec** — or at runtime, a change — **that would cause regression or loss of information from Node A during integration.** (Priority rule 4.)
5. **DO NOT silently merge the Nodes A–E pipeline with the parallel x2/x3 restatement.** x2/x3 is equivalent to Node C/D/E.
6. **DO NOT overwrite the original specification.** Fresh copy only (§10).
7. **DO NOT assume a literal persistent database.** *"ai optimized database"* is interpreted as markdown stage files.

### Spec-added anti-patterns (discovered during design)

8. **DO NOT let the runtime skill ask the user questions or perform external research.** Runtime is self-contained. The "do research or ask questions as neccessary" directive is **scoped to the spec-writing phase only**.
9. **DO NOT generate Node E entries outside the two-drafts-pick-best protocol.** Every Node E entry is the output of a compare over exactly 2 independently drafted solutions.
10. **DO NOT process other sections before Headline Insight when Node B is genius-or-drift.**
11. **DO NOT treat Alternative Hypotheses as a source of multiple competing ideas.** Only the best-fit hypothesis is used.
12. **DO NOT write to the Node A directory.** All outputs live in the session directory.
13. **DO NOT pick a section list at runtime by guessing.** If the reference URL is unreadable and Node B lacks canonical headers, rely on the drift detection heuristic and Node B's actual section structure.

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

## Pre-Pipeline

### Flag Parsing

Parse argv for `--session-dir=<path>`, `--start-at=<module>`, `--stop-at=<module>`.

- If `--session-dir` is set: existing session mode. Directory must exist. `--start-at` defaults to the first module whose output file is missing.
- If `--session-dir` is unset: fresh session mode. `--start-at` and `--stop-at` are rejected with an error ("partial-run flags require --session-dir").

Record parsed flags to an internal state record; written to `session.json.argv_flags` at Session Init.

### Session Init

Compute `session_id` from Node A stem per spec §10 (slugification + date-prefix dedup rule). Create `~/docs/epiphany/analysis/<session-id>/` (on collision append `-run-N`). Write initial `session.json` with `created_utc`, `argv_flags`, and empty `modules` object per spec §10 schema. Use `templates/session.json.template` as the base structure.

### Replay Contract

- `--session-dir=<path>` points at an existing `~/docs/epiphany/analysis/<session-id>/`. Skill loads stage files already present and resumes from `--start-at` (default: first module with missing output file).
- Without `--session-dir`, the skill always creates a fresh session directory.
- `--start-at` and `--stop-at` take module names: `m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`.
- Without `--start-at`, auto-detect the first module whose output file is missing in `--session-dir` and resume from there.
- Between every pipeline module, check `--stop-at`: if the just-completed module name matches, halt before the next module (write session.json, skip enhanced.md).

## Pipeline

The agent executes each module inline in sequence. Each module reads from prior stage output (held in context from the previous module, or read from disk on replay) and writes its own stage files before advancing.

---

### M-INTAKE: Load and Detect

**Entry:** Read argv (Node A path/content, Node B path). Read `templates/session.json.template`.
**Preconditions:** Session directory exists (created in Session Init).
**Exit:** Write `node-a.md`, `node-b.md` to session_dir. Write `session.json` with `node_a` and `node_b` sections populated.
**Progress:** `[M-INTAKE] Loading inputs... detected: node_a.source_kind=<inline|file>, node_b.genius_detection=<tag>`

#### Purpose

Load Node A and Node B. Detect Node A source kind. Detect Node B type per the Genius Detection table above. Stage both to `{session_dir}`. Seed `session.json`.

#### Hard Gate HG-1

Before any write, verify the target path for `enhanced.md` (= `{session_dir}/enhanced.md`) does NOT equal the Node A source path. If equal, abort with error:
> "HG-1 violation: Node A source path equals enhanced.md target. The skill never overwrites Node A."

#### Procedure

1. **Read Node A.** If argv provides a path, read the file. If Node A is inline in the user's prompt, capture that text. Set `session.json.node_a.source_kind` = `file` or `inline`, and `session.json.node_a.source_path` to the path (or `null` for inline).

2. **Read Node B.** Always a file path. Set `session.json.node_b.source_kind` = `file` and `.source_path` = the path.

3. **Copy verbatim to stage files.** Write Node A content to `{session_dir}/node-a.md` and Node B content to `{session_dir}/node-b.md` byte-for-byte. No whitespace normalization, no trailing-newline adjustment, no BOM stripping.

4. **Run genius detection.** Open `node-b.md` and count canonical-section-header matches using the table under `## Genius Detection` above. Rules:
   - ≥7 matches → `genius_detection = "genius-current"`
   - 5–6 matches → `genius_detection = "genius-drift"` and record `matched_sections` list
   - <5 matches → `genius_detection = "generic-fallback"` and write `drift_notes = "below genius threshold — flat-scan mode"`

5. **Write hashes to session.json.** Compute SHA-256 of each stage file and record to `session.json.node_a.content_hash` and `session.json.node_b.content_hash`. Reference commands (Python):
   ```python
   import hashlib, pathlib
   h = hashlib.sha256(pathlib.Path(stage_path).read_bytes()).hexdigest()
   ```
   Or shell: `sha256sum {stage_path} | awk '{print $1}'`.

   These hashes are the authority for R6 (mtime + hash unchanged) at m-integrate time.

#### Verbatim preservation

The stage files in `{session_dir}/` are byte-for-byte copies of the source. Do NOT:
- normalize whitespace or line endings
- strip or add BOMs
- collapse blank lines
- inject headers, footers, or provenance markers

**Invariant.** After step 3, `sha256(stage_file_content) == sha256(source_file_content)` for both Node A and Node B. If a source is inline (Node A only), the stage file hash is recorded against the captured inline text. Any mismatch is an m-intake error.

---

### M-ANALYZE: Full-Detail Analysis

**Entry:** Read `node-a.md`, `node-b.md` from session_dir. Read `session.json` (`node_b.genius_detection`).
**Preconditions:** `session.json.node_b.genius_detection` is set.
**Exit:** Write `node-a1-analysis.md`, `node-b1-analysis.md` to session_dir. Update `session.json.modules.m-analyze` (`a1_complete`, `b1_complete`).
**Progress:** `[M-ANALYZE] A1 sections: {n}/6 | B1 sections: {n}/{total} | detection: {tag}`

#### Purpose

Produce Node A1 (full analysis of Node A) and Node B1 (section-tailored findings from Node B, using the Section-Tailoring Map when Node B is genius-or-drift; generic flat scan when fallback). Per spec phase 0: *"analyze both nodes in full, taking into account every details concept idea facet large small or fine, including technical details, do not generalize or glaze over details, all details must be reviewed for both node a and node b and overall goal described"*.

#### Phase A1 — Full Analysis of Node A

Produce `node-a1-analysis.md` with exactly 6 mandatory sections:

- **`## Intent`** — What Node A is trying to accomplish. Its purpose, audience, and goals.
- **`## Structure`** — How Node A is organized. Section breakdown, heading hierarchy, flow.
- **`## Claims`** — Explicit assertions, facts, or conclusions stated in Node A. One bullet per claim.
- **`## Constraints`** — List every MUST, MUST-NOT, rule, or invariant the original Node A imposes — on itself, on its consumers, or on its behavior. One bullet per constraint. **This is the no-regression reference set** — used by m-integrate R4 audit to verify no constraint is lost in the enhanced output.
- **`## Gaps`** — Missing information, unstated assumptions, or areas where Node A is incomplete.
- **`## Technical details`** — Technical specifics, implementation details, domain-specific content.

Mark `a1_complete: true` in session.json when all 6 sections are non-empty OR explicitly marked `(none)`.

#### Phase B1 — Section-Tailored Analysis of Node B

Branch on `session.json.node_b.genius_detection`:

- **`genius-current` or `genius-drift`:** apply the Section-Tailoring Map from `## Section-Tailoring Map` above. Each matched canonical section produces findings per that row's extraction strategy. Headline Insight is processed FIRST (before any other section). Every finding is tagged with `source_section` for flow-through to Node C.
- **`generic-fallback`:** single generic extraction pass over the whole document; produces flat findings list (each finding tagged with a best-guess `source_section` = heading it fell under, or `generic` if none).

Mark `b1_complete: true` when all matched sections are processed (or fallback scan complete). Sections with zero yield are marked **complete and skipped** (do NOT retry indefinitely).

#### Special handling (from Section-Tailoring Map)

**Headline Insight (row 1).** Process FIRST, before any other B1 section. The Headline Insight shapes the weighting of every downstream finding. Its source_section tag is propagated to every idea that traces back to it (used by m-integrate for Headline precedence).

**Theory Collisions (row 2).** When Node B's Theory Collisions section names two incompatible positions, extract BOTH as separate findings — do not synthesize them into one. m-ideate will generate independent ideas from each; m-filter may reject one of them. Never pre-resolve a collision at analysis time.

**Alternative Hypotheses (row 5).** Extract only the best-fit hypothesis (the one Node B itself prefers, or the one with the strongest supporting evidence if no preference stated). Do NOT extract runner-up hypotheses. This prevents idea-pool dilution with low-utility alternatives.

---

### M-IDEATE: Cross-Cutting Ideation

**Entry:** Read `node-a1-analysis.md`, `node-b1-analysis.md` from session_dir.
**Preconditions:** `a1_complete=true`, `b1_complete=true` in session.json.
**Exit:** Write `node-c-ideas.md` to session_dir. Update `session.json.modules.m-ideate` (`passes_run`, `ideas_total`, `stop_reason`).
**Progress:** `[M-IDEATE] pass {N}/3: {new_count} ideas | total: {total} | stop: {reason}`

#### Purpose

Produce Node C — the unfiltered idea pool — by running bounded-pass cross-cutting ideation over A1 + B1 findings. Implements: *"iteratively run phase a, passing all ideas generated into storage node c"*, *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"*.

#### Bounded loop

```
passes_run = 0
ideas = []
while passes_run < 3 and len(ideas) < 50:
    new_ideas_this_pass = run_pass(passes_run + 1, seed=A1+B1, dedup_against=ideas)
    if len(new_ideas_this_pass) == 0:
        stop_reason = "no-new-ideas"
        break
    ideas.extend(new_ideas_this_pass)
    passes_run += 1
else:
    stop_reason = "pass-cap" if passes_run == 3 else "idea-cap"
```

#### Persona rotation (pass independence)

Each pass opens with a **visible preamble** declaring the role and goal:

- **Pass 1 — Skeptical Reviewer.** Goal: find every issue, gap, or unsupported claim in Node A.
- **Pass 2 — Contrarian First-Principles Designer.** Goal: propose non-obvious structural restructurings.
- **Pass 3 — User-Empathy Advocate.** Goal: find usability and maintainability gaps affecting downstream readers.

The preamble is text in the pass, not a parameter. Successive passes are thus decorrelated by explicit framing.

#### Node C entry schema

Each idea written to `node-c-ideas.md` is a block:

```
## I0N — <short title>
**source_section:** <B1 section name or A1 bucket>
**target_section:** <where in Node A the change would apply>
**observation:** <what A1/B1 finding seeded this>
**idea:** <the proposed improvement, one paragraph>
**category:** <structural | content | clarity | constraint | other>
**effort:** <low | medium | high>
```

#### Dedup rule

Two entries collide when `(source_section, normalized_observation, target_section)` matches. `normalized_observation = lowercase + collapse whitespace + strip filler words ("the","a","that","this") + first 120 chars`. On collision: keep earlier ID, append a line `merged_into: I0N` under a `## Merged` section at file end.

---

### M-FILTER: Pro/Con/Utility Filter

**Entry:** Read `node-c-ideas.md` from session_dir.
**Preconditions:** `node-c-ideas.md` exists and is non-empty (or m-ideate completed with 0 ideas — edge case: mark complete and skip).
**Exit:** Write `node-d-accepted.md` to session_dir. Update `session.json.modules.m-filter` (`ideas_in`, `accepted_out`).
**Progress:** `[M-FILTER] reviewed: {N}/{total} | accepted: {K} | rejected: {N-K} | merged: {M}`

#### Single verdict pass

Iterate every Node C entry once. For each, evaluate against the verbatim judgment criteria:

> *considering overall utility and relevancy, and if it will create a better product without excessive overengineering or complexity, or resolves any potential issues, we want the best product features that are usable for humans, must be realistic to implement without overcomplexity or regression*

Produce one of three verdicts: **ACCEPT**, **ACCEPT-CONDITIONAL**, **REJECT**.

#### Node D entry schema (accepted)

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

#### Merge consolidation (one-time sub-pass)

After the verdict pass, scan accepted entries for merge candidates. Two entries merge when:
(a) both target the same `target_section`, AND
(b) one entry's idea is subsumed by or reinforces the other.

Merge output:
- `from_idea_id` becomes a list (e.g., `[I05, I07]`)
- `verdict` = stronger of the two (ACCEPT > ACCEPT-CONDITIONAL)
- `gain` = concatenation of both gain strings separated by ` + `, deduped on exact-substring match

REJECT entries never merge. This sub-pass runs exactly once — no iteration.

---

### M-ENGINEER: Two-Drafts-Pick-Best

**Entry:** Read `node-d-accepted.md`, `node-a.md` from session_dir.
**Preconditions:** `node-d-accepted.md` exists.
**Exit:** Write `node-e-solutions.md` to session_dir. Update `session.json.modules.m-engineer` (`d_entries_processed`, `e_entries_produced`, `double_fail_count`).
**Progress:** `[M-ENGINEER D_i {N}/{total}] drafting solution 1... | drafting solution 2... | compare+pick | stored=E0K`

#### A-context excerpt extraction

For each D_i, the excerpt passed to the solution drafter is the Node A region identified by D_i's `target_section`:

- If `target_section` is a section heading: excerpt = heading + content up to the next heading of equal or higher level.
- If `target_section` is a line range: excerpt = those lines ± 10 lines of surrounding context.
- If `target_section` is a natural-language locator: excerpt = paragraph containing best substring match + 2 surrounding paragraphs.
- If no match: pass full Node A with a log warning in session.json.stop_reasons.

#### Two-drafts-pick-best (Hard Gate HG-3)

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

#### Node E `target` locator (exactly ONE of 4 shapes)

- `{"kind": "heading_path", "path": ["## §4", "### Branching points"]}`
- `{"kind": "line_range", "start": 123, "end": 145}` (only when Node A is a stable file)
- `{"kind": "anchor", "before": "<exact substring>", "after": "<exact substring>"}`
- `{"kind": "verbatim_block", "text": "<multi-line exact substring>"}` (for `replace` actions only)

Natural-language locators FORBIDDEN. Drafter must resolve to one of the 4 or fail verify.

#### Node E entry schema

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

#### Dropped section (double-fail rule)

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

---

### M-INTEGRATE: Flush Integration + Self-Audit

**Entry:** Read `node-a.md`, `node-e-solutions.md`, `node-a1-analysis.md` from session_dir. Read `session.json`.
**Preconditions:** `a1_complete=true`, `b1_complete=true`. `node-e-solutions.md` exists.
**Exit:** Write `enhanced.md` (or `enhanced-partial.md` + `open-questions.md`). Write `self-audit.md`. Update `session.json.modules.m-integrate` (`refinement_cycles`, `audit_verdict`).
**Progress:** `[M-INTEGRATE] applying {N} solutions... self-audit: {PASS|PARTIAL}`

#### Precondition gate (before integration starts)

Abort with error if any of:
- `node-a1-analysis.md` or `node-b1-analysis.md` does not exist or is empty
- `node-e-solutions.md` does not exist (empty-but-present OK — log reason to session.json.stop_reasons)
- `session.json.modules.m-analyze.a1_complete != true` OR `.b1_complete != true`

Verbatim precondition clause: *"analysis verified for accuracy and completeness"*.

#### Integration order

Apply Node E entries in this precedence:

1. Entries whose `from_d_id`'s `from_idea_id` originated in the Headline Insight section — applied first. For merged D entries where `from_idea_id` is a list, qualifies if **any** constituent traces to Headline Insight.
2. Remaining entries in stable order (as they appear in `node-e-solutions.md`).

#### Conflict resolution

Two Node E entries conflict when their `target` regions overlap by ≥1 line of Node A. On detection:

1. If both `from_d_id`'s trace to the same `target_section`: attempt **compose** — synthesized patch applying both in sequence. Verify the composed patch still preserves info. If compose succeeds, use it.
2. Otherwise: apply the entry with higher utility (from Node D). On tie: lower `from_idea_id` wins (stable).
3. Losing entry is dropped; record in `self-audit.md` under `## Integration conflicts`.

#### Progress reporting

Emit `[M-INTEGRATE] applying E_K/<total>: <brief-target>` per integration step, then `[M-INTEGRATE] self-audit running...` before the audit runs.

#### Runtime audit (before finalizing output)

- **R1.** *"analysis verified for accuracy and completeness"* — session.json.modules.m-analyze.a1_complete && .b1_complete.
- **R2.** Every E entry appears at its `target` location (dispatch by `target.kind`).
- **R3.** No Node A content removed without a matching Node E `replace` (shorter text) with rationale citing the removed content.
- **R4.** Enhanced A still contains every constraint listed in A1's `## Constraints` — substring or semantic paraphrase match required.
- **R5.** No newly-introduced defects (unclosed fences/tags, duplicated paragraphs, TODO/TBD tokens) that weren't already in Node A or introduced via a Node E entry.
- **R6.** Node A source file mtime + hash unchanged (compared against session.json.node_a.content_hash).

#### FAIL handling

- **1st FAIL:** log findings to `self-audit.md`. Run one refinement cycle (§6 Level B) targeting only failed checks. Re-verify.
- **2nd FAIL:** write `enhanced-partial.md` + `open-questions.md` with specific R1-R6 failure IDs. Halt. Do NOT write `enhanced.md`. Set session.json.modules.m-integrate.audit_verdict = "PARTIAL".
- **PASS:** write `enhanced.md`. Set audit_verdict = "PASS".

Always write `self-audit.md` with the per-check verdict record.

---

## Completion

After `m-integrate` returns, print the final completion message to the user:

**On PASS:**
```
epiphany-analysis complete.
Enhanced document: ~/docs/epiphany/analysis/<session-id>/enhanced.md
Ideas generated: <node-c count> | Accepted: <node-d count> | Applied: <node-e count>
Self-audit: PASS
```

**On PARTIAL (double-FAIL):**
```
epiphany-analysis complete (partial).
Partial output: ~/docs/epiphany/analysis/<session-id>/enhanced-partial.md
Open questions: ~/docs/epiphany/analysis/<session-id>/open-questions.md
Self-audit: PARTIAL — see self-audit.md for failed checks (R1–R6 IDs listed)
```

**On hard abort (timeout or gate violation):**
```
epiphany-analysis aborted.
Reason: <stop_reason from session.json>
Partial session: ~/docs/epiphany/analysis/<session-id>/
```

All counts are read from `session.json` at print time. Session directory path is always printed so the user can locate outputs.

## Edge Cases

| # | Case | Resolution |
|---|---|---|
| a | epiphany-genius current version | Full 10-section Map (Genius Detection above) |
| b | epiphany-genius version drift | Partial Map + generic for unmatched |
| c | non-epiphany-genius analysis | Generic fallback full-document scan |
| d | theory collisions handling | Verbatim special handling in Section-Tailoring Map row 2 |
| e | alt hypotheses low-signal | Verbatim special handling in Section-Tailoring Map row 5 |
| f | headline insight primary anchor | Processed first in Section-Tailoring Map row 1 |
| g | Node A source is a file | Handled at m-intake |
| h | zero-ideas section | Mark complete and skip |
| i | x2/x3 parallel pipeline relation to C/D/E | Declared equivalent to Node C/D/E |
| j | research / ask-questions scoping | Spec-writer only; runtime self-contained |

## Changelog

### v2.0.0 (2026-04-18)
- **Architecture:** Inline execution — all 6 module protocols embedded in SKILL.md, no subagent spawns
- **Pre-Pipeline:** Flag parsing and session init consolidated into dedicated section
- **Pipeline:** Replaces Orchestrator; each module is an `### M-XXXXX` subsection with Entry/Exit/Progress headers
- **modules/ directory:** Retained as reference archive (not invoked at runtime)
- **Spec compliance:** All hard gates, termination caps, anti-patterns, self-audit rules preserved verbatim
- **Replay contract:** `--session-dir`/`--start-at`/`--stop-at` fully supported inline