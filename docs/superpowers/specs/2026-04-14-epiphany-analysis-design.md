# epiphany-analysis — Skill Design Specification

- **Date:** 2026-04-14
- **Consumer:** implementation-agent + human developer
- **Supersedes:** `~/.claude/skills/epiphany-analysis/SKILL.md` v0.1.0-placeholder (the placeholder skill, a forward dependency of `epiphany-omnipotent`, is replaced by this skill; the omnipotent-XML-consumer role is not preserved here and may be re-created later under a new name if needed)
- **Source prompt:** epiphany-prompt-enhanced input produced 2026-04-14 (session `.sessions/20260414-create-enhanced-prompt-which-will/`)
- **Reference analysis URL (verbatim):** `file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`
- **Gold-reference artifacts:** `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` co-located with `report.md` — hand-executed run of the C/D/E pipeline, used as the ground-truth example throughout this spec.

> This is a detailed specification, not code. A future implementation agent uses this document to build `epiphany-analysis`.

---

## §1. Skill identity and activation

### Name and one-line description

- **Name:** `epiphany-analysis`
- **Description:** Takes an input (Node A) and an analysis of that input (Node B) and produces a fresh enhanced Node A that integrates every solution derivable from Node B. Uses a cross-cutting graph-of-thought pipeline (Nodes A, B, A1, B1, C, D, E) with ideation/synthesis/verification loops modeled on `prompt-epiphany`.

### Activation triggers

| Trigger | Behavior |
|---|---|
| `/epiphany-analysis <path-to-A> <path-to-B>` | Run skill with positional paths. Primary invocation. |
| `/epiphany-analysis <path-to-A>` | Ask once: "Path to the analysis document (Node B)?" Then run. |
| `/epiphany-analysis` | Ask once for A, once for B. Then run. |
| `/epiphany-analysis` when Node A is inline in the user prompt | Ask once for B; store inline A content as Node A (no file). |
| `/epiphany-analysis --session-dir=<path> [--start-at=<module>] [--stop-at=<module>]` | Replay / partial-run mode. `--session-dir` points at an existing `~/docs/epiphany/analysis/<session-id>/`; skill loads stage files already present and resumes from `--start-at` (default: first module with missing output). See §12. |

No natural-language-trigger mode in v1.

### Inputs

- **Node A** — the input to be improved. Either a file path (read at `m-intake`) or inline content in the user's prompt.
- **Node B** — an analysis document of Node A. File path. Most commonly an `epiphany-genius` report; generic analysis documents also supported via fallback.

### Primary outputs

- `~/docs/epiphany/analysis/<session-id>/enhanced.md` — fresh enhanced Node A. Never overwrites the original Node A.
- Seven stage files (see §3 Node data model) co-located with `enhanced.md`.
- `self-audit.md` — the self-audit verdict (PASS or the FAIL record that preceded partial abort).

### Runtime scope

**Runtime is self-contained.** The skill operates purely off Node A and Node B; it does not perform external research, does not use web/grep/glob to look up unrelated context, and does not ask the user clarifying questions during execution. The "do research or ask questions as neccessary" directive in the source prompt is **scoped to the spec-writing phase only** (the brainstorming session that produced this spec). Runtime is deterministic with respect to (A, B).

**Distinction — pre-execution arg-gathering vs. runtime Q&A:** asking the user for a missing file path at invocation (§1 Activation triggers) is **pre-execution**, before `m-intake` starts. Once `m-intake` begins, the skill never asks the user anything — all remaining behavior is a pure function of (Node A, Node B).

---

## §2. Module inventory

Six modules, one per major pipeline stage. Each module reads and writes stage files in the session directory so any module can be replayed or replaced in isolation — that is the forward-integration contract with the `epiphany-genius` modular system.

### m-intake

- **Purpose:** Load Node A and Node B; detect Node A source type (inline vs. file); detect Node B type (`genius-current` / `genius-drift` / `generic-fallback` per §8); stage both.
- **Inputs:** User prompt + args `<path-to-A>`, `<path-to-B>`.
- **Outputs:** `node-a.md`, `node-b.md` (both verbatim copies), plus type tags written to `session.json`.
- **Invocation:** Inline in the orchestrator context.
- **Forward-integration note:** Exposes `input_primary` and `input_analysis` handles consumable by any downstream `epiphany-*` skill that wants to chain off the same intake.

### m-analyze

- **Purpose:** Produce Node A1 (full analysis of A) and Node B1 (section-tailored findings from B, using the Section-Tailoring Map when Node B is genius-or-drift; generic flat scan when fallback).
- **Inputs:** Node A, Node B, type tags, Section-Tailoring Map (shipped with skill as §5 below).
- **Outputs:** `node-a1-analysis.md`, `node-b1-analysis.md`.
- **Invocation:** Inline. Per source prompt's **phase 0** directive: *"analyze both nodes in full, taking into account every details concept idea facet large small or fine, including technical details, do not generalize or glaze over details, all details must be reviewed for both node a and node b and overall goal described"*.
- **Forward-integration note:** Exposes `analysis_of_primary` and `analysis_of_analysis` handles.

### m-ideate

- **Purpose:** Cross-cutting idea generation producing Node C — the unfiltered idea pool. Each idea is ID-keyed and carries a `source_section` tag. Implements the source prompt's directive: *"iteratively run phase a, passing all ideas generated into storage node c"*, *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"*.
- **Inputs:** Node A1, Node B1.
- **Outputs:** `node-c-ideas.md`.
- **Invocation:** Inline. Bounded-pass loop (§11 caps). Each pass runs with the source prompt's creativity rule: *"be creative and analyze from new perspectives with different roles and goals"*.
- **Pass independence mechanism (G7 fix):** each pass uses a distinct role-and-goal framing — pass 1 = "skeptical reviewer" goal = "find every issue", pass 2 = "contrarian first-principles designer" goal = "propose non-obvious restructurings", pass 3 = "user-empathy advocate" goal = "find usability/maintainability gaps". The framing is written as a visible preamble in the pass, not as an LLM parameter, so successive passes are not correlated by identical context.
- **Uniqueness / dedup rule (G4 fix):** Node C entries are deduped by the tuple `(source_section, normalized_observation, target_section)`, where `normalized_observation = lowercase + collapse whitespace + strip common filler words ("the", "a", "that", "this") + first 120 chars`. When two entries collide, the earlier ID is kept and the later is logged under a `## Merged` section with its ID and a `merged_into: I0N` field.
- **Progress reporting (M1 fix):** before each pass, emit one status line — `[m-ideate pass N/3] seed-count=<A1+B1 findings>, ideas-so-far=<count>, elapsed=<mm:ss>`. After each pass, emit `[m-ideate pass N complete] new-ideas=<N>, merged=<N>, stop-reason=<if halt>`.
- **Forward-integration note:** Exposes `idea_pool` handle with pass count and stop-reason recorded.

### m-filter

- **Purpose:** Produce Node D — the accepted-ideas store — by running a pro/con/utility/over-engineering/regression filter over every Node C idea. Implements the source prompt's directives: *"iterate over each idea we generated which is held in node C database data"*, *"each idea in database analyzed for pro/con/utlity/etc and decide if we use the idea"*, *"iterate over every idea in node C, be sure we don't miss anything"*.
- **Inputs:** Node C.
- **Outputs:** `node-d-accepted.md`.
- **Invocation:** Inline, single pass (§11).
- **Judgment criteria (verbatim from source prompt):** *"considering overall utility and relevancy, and if it will create a better product without excessive overengineering or complexity, or resolves any potential issues, we want the best product features that are usable for humans, must be realistic to implement without overcomplexity or regression"*.
- **Merge procedure (G1 fix):** after the verdict pass, a second consolidation sub-pass scans accepted entries for related improvements targeting the same `target_section`. Two entries may merge when (a) both target the same `target_section` AND (b) one entry's idea is subsumed by or reinforces the other. Merged entries produce a single Node D record with `from_idea_id: [I05, I07]` (list), merged verdict = stronger of the two (ACCEPT > ACCEPT-CONDITIONAL), merged `gain` = concatenation of both gain strings separated by ` + `, deduped on exact-substring match. Reject entries are never merged. Merge is a one-time sub-pass — no iteration.
- **Progress reporting (M1 fix):** emit one status line per 10 processed Node C entries — `[m-filter] reviewed=<N>/<total>, accepted=<N>, rejected=<N>, elapsed=<mm:ss>`.
- **Forward-integration note:** Exposes `accepted_improvements` handle.

### m-engineer

- **Purpose:** For each accepted Node D idea, generate two solutions separately, compare them, and store the best one in Node E. Implements the source prompt's directives: *"When all analyzed, analyze the first improvement node D. compare to spec in node A, and engineer the optimal solution to achieve the improvement or resolution we are working on. pass this solution into node E which stores our solutions"*, *"generate two solutions seperately"*, *"analyze solutions compare pro/con and utility and over-engineering limited"*, *"pass only best solution"*, *"we want to use a process of ideation, synthesis, and verification for each individual idea. referene the process seen in prompt-epiphany skill"*, *"continue until all improvements in improvement storage node are fully resolved with solutions stored in node E"*.
- **Inputs:** Node D, Node A (for context of the change).
- **Outputs:** `node-e-solutions.md`.
- **Invocation:** Inline. Outer loop over D_i; inner per-draft ideation/synthesis/verification loop per §6 Level A.
- **A-context excerpt extraction (G5 fix):** for each D_i, the excerpt passed to the solution drafter is the Node A region identified by D_i's `target_section` field. If `target_section` is a section heading, excerpt = that heading + all content up to the next heading of equal or higher level. If `target_section` is a line range, excerpt = those lines ± 10 lines of surrounding context. If `target_section` is a natural-language locator, excerpt = the paragraph containing the best substring match + 2 surrounding paragraphs. If no match, the full Node A is passed (with a log warning).
- **Double-fail rule (B2 fix):** if **both** Solution_1 and Solution_2 fail their verify step in §6 Level A, D_i is **dropped** — no third draft, no retry. A record is written to `node-e-solutions.md` under a `## Dropped` section with fields `{from_d_id, reason: "double-draft-verify-failed", failed_checks_1: [...], failed_checks_2: [...]}`. The skill advances to the next D_i. Dropped entries are surfaced in the final self-audit report.
- **Progress reporting (M1 fix):** emit one status line per D_i — `[m-engineer D_i N/<total>] drafting solution 1... | drafting solution 2... | compare+pick | stored=E0K`. Dropped D_i emits `[m-engineer D_i N dropped] reason=double-draft-verify-failed`.
- **Forward-integration note:** Exposes `solutions_store` handle.

### m-integrate

- **Purpose:** Produce the draft enhanced Node A by flush-integrating all Node E solutions, run the self-audit verification block (§9), and write the fresh output file. Implements the source prompt's directives: *"create a final version of the input originally held in node A which flushly integrates all solutions held in node E, enhancing the original based on the analysis verified for accuracy and completeness"*, *"we want to use a process of ideation, synthesis, and verification"*, *"verify all changes integrated flush with self-audit focusing on being bug-free, no errors issues or potential problems"*, and *"create a new seperate final specification integrating all solutions from node E, do not overwrite existing specification with the changes, save a fresh copy of the full enhanced output only"*.
- **Inputs:** Node A, Node E (all entries).
- **Outputs:** `enhanced.md`, `self-audit.md`.
- **Invocation:** Inline. Outer integration loop per §6 Level B with at most 1 refinement cycle on FAIL, then abort.
- **Conflict resolution rule (G3 fix):** two Node E entries **conflict** when their `target` regions overlap by ≥1 line of Node A. On detection:
  1. If both entries' `from_d_id` trace back to the same `target_section`, attempt a **compose** — a synthesized patch applying both changes in sequence. Verify the composed patch still preserves info (priority rule 4). If compose succeeds, use it.
  2. If compose fails OR entries target different `target_section`s, apply the entry with **higher utility** (from Node D). If equal utility, apply the entry whose `from_idea_id` has the lower ID (stable ordering).
  3. The losing entry is dropped with a log record in `self-audit.md` under `## Integration conflicts`.
- **Progress reporting (M1 fix):** emit `[m-integrate] applying E_K/<total>: <brief-target>` per integration step, then `[m-integrate] self-audit running...` before §9.2 runs.
- **Forward-integration note:** Exposes `enhanced_output` as the terminal handle.

---

## §3. Node data model

Seven nodes. All stage files live in `~/docs/epiphany/analysis/<session-id>/`. Labels A, B, A1, B1, C, D, E are preserved verbatim throughout this spec, per source prompt's DO directive.

### Node A

- **Purpose:** Original user input. Source of truth for the content being improved. Per source prompt: *"node a is the input. it could be specified directly in the user prompt context, or be stored in a file or other format provided by the user"*.
- **Directive (verbatim):** *"store original context provided by user into node A"*.
- **Inputs:** Inline user prompt content OR file path.
- **Outputs:** Verbatim copy + source type tag (`inline` or `file:<path>`).
- **Storage schema:** `node-a.md` — exact byte-for-byte copy of user input. No transformation.
- **Termination condition:** Read once at `m-intake`; immutable thereafter.
- **Integration point (epiphany-genius modular system):** `input_primary` handle.

### Node B

- **Purpose:** The analysis document of Node A. Primary driver of improvement ideas. The reference form is an `epiphany-genius` report (see Reference URL). Non-genius analyses are supported via fallback.
- **Inputs:** File path.
- **Outputs:** Verbatim copy + Node B type tag (`genius-current` / `genius-drift` / `generic-fallback`).
- **Storage schema:** `node-b.md` — byte-for-byte copy of the analysis document.
- **Termination condition:** Read once at `m-intake`; immutable thereafter.
- **Integration point:** `input_analysis` handle.

### Node A1

- **Purpose:** Full analysis of Node A.
- **Directive (verbatim):** *"analysis of node a goes in node a1 and analysis of node b goes in node b1"*.
- **Inputs:** Node A.
- **Outputs:** Structured findings document covering: what A is, structural breakdown, intent, explicit claims, **constraints** (rules, invariants, and MUST/MUST-NOT statements the original Node A imposes on itself or its consumers), gaps, ambiguities, technical details. Per source prompt phase 0: *"taking into account every details concept idea facet large small or fine, including technical details, do not generalize or glaze over details"*.
- **Storage schema:** `node-a1-analysis.md` with sections `## Intent`, `## Structure`, `## Claims`, `## Constraints`, `## Gaps`, `## Technical details`. `## Constraints` is the reference surface for R4 no-regression checks (§9.2).
- **Termination condition:** One-shot analysis pass. No loop.
- **Integration point:** `analysis_of_primary` handle.

### Node B1

- **Purpose:** Full analysis of Node B — specifically, the section-tailored findings produced by applying the Section-Tailoring Map (§5) to Node B. B1 is the **populated per-invocation instance**; the Map is the **static config that ships with the skill**.
- **Directive (verbatim):** *"analysis of node a goes in node a1 and analysis of node b goes in node b1"*.
- **Inputs:** Node B + Section-Tailoring Map + Node B type tag.
- **Outputs:** 10-section findings document when genius-or-drift (matched sections only in drift mode). Flat findings list when generic-fallback. Each finding tagged with `source_section`.
- **Storage schema:** `node-b1-analysis.md`. Heading per matched section; under each heading, findings in the extraction-strategy format defined by the Map.
- **Termination condition:** Exactly one entry per section processed. Sections with zero yield are marked **complete and skipped** (edge case (h)): *"Ideation produces zero ideas for a section → mark section complete and move on. Do NOT retry indefinitely."*
- **Integration point:** `analysis_of_analysis` handle.

### Node C

- **Purpose:** Unfiltered idea pool. Every generated improvement candidate stored here as an individual element.
- **Directives (verbatim):**
  - *"create and synthesize ideas and pass each idea as an indivudal element stored in node C"*
  - *"create idea storage node C. node C stores each idea generated by previous step analysis in an ai optimized database"*
  - *"generate new ideas for improvements based on node b analysis, to make a better spec node a, storing in database"*
  - *"iteratively run phase a, passing all ideas generated into storage node c"*
  - *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"*
  - *"each idea stored in database,"* (preserved fragmentary as in source)
- **Creativity rule (verbatim):** *"be creative and analyze from new perspectives with different roles and goals"*.
- **Storage hint (verbatim):** *"all databases in system expand and create relationships as neccessary for ideal design"*.
- **"AI optimized database" interpretation:** Markdown stage file matching the gold example's schema. See Storage schema below. This interpretation is mandatory per DO-NOT-7.
- **Inputs:** Node A1, Node B1.
- **Outputs:** ID-keyed idea matrix. Each idea: `id` (I01, I02, …), `source_section` (which B1 section or A1 bucket produced the insight), `target_section` (which region of Node A the change would apply to — set at idea-creation time by `m-ideate`; flows C → D → E), `observation`, `idea`, `category`, `effort`.
- **Storage schema:** `node-c-ideas.md`. Header `# Node C — Idea Matrix`. Meta block records `source_nodes`, `generation_method`, `pass_count`, `stop_reason`. Per-idea block uses the gold example's schema exactly.
- **Termination condition:** *"until a full ideation pass generates zero new unique ideas"* **OR** 3 passes cap **OR** 50 ideas cap — whichever first. See §11.
- **Integration point:** `idea_pool` handle with pass count + stop reason logged.

### Node D

- **Purpose:** Accepted-ideas store. Ideas from Node C that pass the pro/con/utility/realistic-to-implement filter.
- **Directives (verbatim):**
  - *"now that we have thought and stored the ideas in node C, they must be analyzed"*
  - *"iterate over each idea we generated which is held in node C database data"*
  - *"create idea storage node d"*
  - *"decide if we should use the idea from node C to improve the node A input when creating the final improved version output from this skill"*
  - *"either accept the idea if adequate and store in node D improvement storage node which holds the ideas we actually want to use or discard ideas we don't want to use"*
  - *"iterate over every idea in node C, be sure we don't miss anything"*
  - *"each idea in database analyzed for pro/con/utlity/etc and decide if we use the idea"*
- **Judgment criteria (verbatim):** *"considering overall utility and relevancy, and if it will create a better product without excessive overengineering or complexity, or resolves any potential issues, we want the best product features that are usable for humans, must be realistic to implement without overcomplexity or regression"*.
- **Creativity rule (verbatim):** *"be creative and analyze from new perspectives with different roles and goals"*.
- **Inputs:** Node C.
- **Outputs:** ID-keyed verdict matrix. Each entry: `id` (D01, D02, …), `from_idea_id` (I_n, optionally merged-from list), `verdict` (ACCEPT / ACCEPT-CONDITIONAL / REJECT), `utility`, `gain`, `target_section`, optional `condition`, optional `rejection_reason`. (The gold example's `no_added_spawn` field is dropped — epiphany-analysis is inline with no spawns, so the field carries no information.)
- **Storage schema:** `node-d-accepted.md`. Header `# Node D — Accepted Improvements`. Evaluation-criteria block at top; per-accepted block below. Matches gold example schema. REJECT entries logged separately.
- **Termination condition:** Single evaluation pass over all Node C entries.
- **Integration point:** `accepted_improvements` handle.

### Node E

- **Purpose:** Solutions store. For each accepted D_i, two solutions are generated separately, compared, and only the best is stored here.
- **Directives (verbatim):**
  - *"When all analyzed, analyze the first improvement node D. compare to spec in node A, and engineer the optimal solution to achieve the improvement or resolution we are working on. pass this solution into node E which stores our solutions"*
  - *"continue analyzing each improvement listed in node d database data. develop the solution for all improvements, storing the solution individually in node E"*
  - *"generate two solutions seperately"*
  - *"analyze solutions compare pro/con and utility and over-engineering limited"*
  - *"pass only best solution"*
  - *"we want to use a process of ideation, synthesis, and verification for each individual idea. referene the process seen in prompt-epiphany skill"*
  - *"continue until all improvements in improvement storage node are fully resolved with solutions stored in node E"*
- **Inputs:** Node D + Node A context.
- **Outputs:** ID-keyed solutions matrix. Each entry: `id` (E01, E02, …), `from_d_id`, `target` (structured locator, see below), `action` (one of `replace` / `insert-before` / `insert-after` / `append` / `prepend`), exact `replacement_text` or `insertion_text`, `rationale`, `compare_notes` (pro/con log from losing draft).
- **`target` format (G2 fix):** the locator is a JSON-like structured field with **exactly one** of these shapes:
  - `{"kind": "heading_path", "path": ["## §4", "### Branching points"]}` — matches by sequential heading traversal.
  - `{"kind": "line_range", "start": 123, "end": 145}` — matches by exact line numbers in Node A (valid only when Node A is read from a stable file).
  - `{"kind": "anchor", "before": "<exact substring>", "after": "<exact substring>"}` — matches the region between two verbatim anchor strings that each appear exactly once in Node A.
  - `{"kind": "verbatim_block", "text": "<multi-line exact substring>"}` — matches the first occurrence of a verbatim multi-line block (for `replace` actions only).
  Natural-language locators like "Step 3 end block" are **forbidden** in `node-e-solutions.md` — they must be resolved to one of the four structured shapes at draft time. If no shape resolves, the draft fails verify (counts toward double-fail rule).
- **Storage schema:** `node-e-solutions.md`. Header `# Node E — Solutions`. Per-entry block with `## E01 — <title>` / `**Target:** <JSON-like locator>` / `**Action:** <replace|insert-before|insert-after|append|prepend>` / `**Replacement:** …` (or `**Insertion:** …`) / `**Rationale:** …` / `**Compare notes:** …`. A separate `## Dropped` section at the file's end records any D_i that failed both drafts (see m-engineer double-fail rule).
- **Termination condition:** Exactly 2 drafts per D_i; compare; store best as one E entry; iterate until all D_i consumed.
- **Integration point:** `solutions_store` handle.

---

## §4. Pipeline topology design — graph-of-thought

### (a) Topology choice and criteria

**Graph-of-thought**, not tree-of-thought. Tree requires strict parent → many-children with no convergent edges. This pipeline has **three convergent edges**:

1. Node A1 + Node B1 → Node C (single idea pool drawing from two analyses)
2. Per-D_i: Solution_1 draft + Solution_2 draft → Node E entry (two parallel drafts merged via compare-and-pick)
3. All Node E entries → Final integration (many solutions merged into one enhanced A)

Criteria satisfied by the topology:
- Acyclic by construction (all loops are bounded and terminate; no back-edges between named nodes).
- Modular — each node and each module can be inspected, replayed, or replaced in isolation.
- Matches the gold-reference C/D/E shape (empirical evidence).

### (b) Branching points

| Node | Branching rule |
|---|---|
| Node D | Each accepted D_i branches into **2 parallel solution drafts** (source-mandated: *"generate two solutions seperately"*). |
| (optional, `m-ideate` internally) | Each ideation pass may branch across sections internally — all branches funnel into one cross-cutting Node C. No per-section C/D/E sub-pipeline is spawned. |

### (c) Convergence / merge points

| Into | Merge rule |
|---|---|
| Node C | Union of findings from A1 and B1. Dedup per §2 `m-ideate` uniqueness rule: `(source_section, normalized_observation, target_section)` tuple; colliding entries merged with a `merged_into` log. |
| Node E entry (per D_i) | Compare Solution_1 vs. Solution_2 on **pro/con + utility + over-engineering-bounded + info-preservation**. Keep winning draft; log losing draft's key cons in `compare_notes`. On double-fail, D_i is dropped per §2 `m-engineer` double-fail rule. |
| Final integration | Apply all Node E entries in **integration order** (below). Overlap → conflict resolution per §2 `m-integrate` conflict rule (compose if same `target_section`; else higher-utility wins with stable tiebreaker). |

**Integration order (A3 fix):** Node E entries are applied in this precedence:

1. Entries whose `from_d_id`'s `from_idea_id` originated in the Headline Insight section (edge case (f)) — applied first. For merged D entries where `from_idea_id` is a list, the entry qualifies for Headline-first precedence if **any** constituent `from_idea_id` traces to the Headline Insight section.
2. Remaining entries in **stable order** — the order they appear in `node-e-solutions.md`, which itself follows the order entries appear in `node-d-accepted.md`, which follows Node D's single-pass iteration over Node C (which is ID-ascending).

This order is deterministic given (A, B) and is required for reproducibility.

### (d) Edge semantics

| Edge | Flows | Shape |
|---|---|---|
| A → A1 | Raw A content for analysis | Stage-file content |
| B → B1 | Raw B content + Section-Tailoring Map | Stage-file content + Map config |
| A1 → C | Structured A findings (claims, gaps) as ideation seeds | ID-keyed contributions |
| B1 → C | Section-tailored findings as ideation seeds | ID-keyed contributions with `source_section` tag |
| C → D | Idea records, one by one | Idea matrix entries |
| D → (Sol_1 ∥ Sol_2) | Accepted idea + A-context excerpt | Idea record + relevant A region |
| Sol_1, Sol_2 → compare | Two drafted solutions | Each: target + replacement text + rationale |
| compare → E entry | Verdict + winning solution | Solution record with compare notes |
| E (all) → Final integration | Ordered solution set | Full `node-e-solutions.md` |
| Final integration → Self-audit | Draft enhanced Node A | In-memory draft |
| Self-audit → Output | PASS verdict + final document | `enhanced.md` on PASS; re-loop once on FAIL; on second FAIL, write partial + halt |

### (e) Dynamic-growth rules

- The **node set is fixed at 7** (A, B, A1, B1, C, D, E). No new node types appear at runtime.
- Entry counts **inside** Node C, Node D, and Node E are dynamic and grow during execution.
- Growth is bounded by the caps in §11. Per source prompt's storage hint: *"all databases in system expand and create relationships as neccessary for ideal design"*.
- Relationships (e.g., "D03 merged from I05 + I07") are represented as fields on entries, not as new nodes.

### (f) Loop-pattern integration

The `prompt-epiphany` ideation / synthesis / verification loop is embedded at **two distinct levels** in this topology (full detail in §6):

- **Level A — per-idea solution engineering** (inside `m-engineer`, per D_i): each of the 2 parallel drafts is itself an ideate → synthesize → verify chain.
- **Level B — final integration** (inside `m-integrate`): produce the enhanced A by an ideate → synthesize → verify chain, with at most 1 refinement cycle on FAIL.

### (g) Cycle / acyclicity rule

The topology **must be acyclic**. No back-edges between named nodes. The only "loops" are:
- `m-ideate`'s bounded-pass counter (terminates by cap or zero-new-ideas halt)
- `m-engineer`'s iteration over D_i (terminates when D is exhausted)
- `m-integrate`'s single refinement cycle (terminates by FAIL-count ≤ 1)

All three are counting loops, not graph cycles. Each has a termination guarantee stated in §11.

### (h) Concrete diagram

```
                        ┌─────────────────────────────────┐
                        │  Phase 0: full-detail analysis  │
                        │  (read A and B completely)      │
                        └───────┬────────────────┬────────┘
                                │                │
                                ▼                ▼
                       ┌────────────────┐  ┌────────────────┐
                       │ Node A (input) │  │ Node B (analy- │
                       │ stored verbatim│  │ sis doc)       │
                       └───────┬────────┘  └────────┬───────┘
                               │                    │
                               ▼                    ▼
                       ┌────────────────┐  ┌────────────────┐
                       │ Node A1        │  │ Node B1        │
                       │ full analysis  │  │ section-tailored│
                       │ of Node A      │  │ findings       │
                       │                │  │ (10 sections)  │
                       └───────┬────────┘  └────────┬───────┘
                               └─────────┬──────────┘
                                         │
                                         ▼
                      ┌──────────────────────────────────────┐
                      │ Node C — unfiltered idea pool        │
                      │ (cross-cutting, section-tagged)      │
                      │ Loop terminates when:                │
                      │  - no new unique ideas, OR           │
                      │  - 3 passes, OR                      │
                      │  - 50 ideas, whichever first         │
                      └──────────────────┬───────────────────┘
                                         │
                                         ▼
                      ┌──────────────────────────────────────┐
                      │ Node D — accepted ideas              │
                      │ 1 pass pro/con/utility filter        │
                      │ (reject regression, over-engineering)│
                      └──────────────────┬───────────────────┘
                                         │
                                ┌────────┴────────┐
                                │  for each D_i:  │
                                ▼                 ▼
                        ┌───────────────┐ ┌───────────────┐
                        │ Solution_1    │ │ Solution_2    │   (two parallel drafts
                        │ ideate→       │ │ ideate→       │    via prompt-epiphany
                        │  synthesize→  │ │  synthesize→  │    loop pattern,
                        │  verify       │ │  verify       │    LEVEL A)
                        └───────┬───────┘ └───────┬───────┘
                                └────────┬────────┘
                                         ▼
                                ┌──────────────────┐
                                │ compare pro/con/ │
                                │ utility/over-eng │
                                │ pick best        │
                                └────────┬─────────┘
                                         ▼
                                ┌──────────────────┐
                                │ Node E entry     │
                                │ (best solution)  │
                                └────────┬─────────┘
                                         │
                       ... (all accepted ideas processed) ...
                                         │
                                         ▼
                      ┌──────────────────────────────────────┐
                      │ Final integration (LEVEL B):         │
                      │ ideate → synthesize → verify         │
                      │ flush integrate all Node E into      │
                      │ a new version of Node A              │
                      │ precondition: "analysis verified     │
                      │  for accuracy and completeness"      │
                      └──────────────────┬───────────────────┘
                                         │
                                         ▼
                      ┌──────────────────────────────────────┐
                      │ Self-audit (§9)                      │
                      │ "verify all changes integrated flush │
                      │  with self-audit focusing on being   │
                      │  bug-free, no errors issues or       │
                      │  potential problems"                 │
                      └──────────────────┬───────────────────┘
                                         │
                                         ▼
                      ┌──────────────────────────────────────┐
                      │ Fresh output file (never overwrites  │
                      │ Node A):                             │
                      │ ~/docs/epiphany/analysis/            │
                      │   <session-id>/enhanced.md           │
                      └──────────────────────────────────────┘
```

### Adjacency table (compact alternative to diagram)

| From | To | Edge label |
|---|---|---|
| user-input | A | intake (inline) |
| file | A | intake (file read) |
| file | B | intake |
| A | A1 | full-detail analysis |
| B | B1 | section-tailored analysis |
| A1 | C | ideation contribution |
| B1 | C | ideation contribution (section-tagged) |
| C | D | accept/reject filter |
| D_i | Sol_1 | engineer draft 1 (Level A loop) |
| D_i | Sol_2 | engineer draft 2 (Level A loop) |
| Sol_1, Sol_2 | E_i | compare + pick best |
| E (all) | draft-enhanced-A | final integration (Level B loop) |
| draft-enhanced-A | self-audit | verify |
| self-audit (PASS) | `enhanced.md` | write output |
| self-audit (FAIL, 1st) | draft-enhanced-A | refine once |
| self-audit (FAIL, 2nd) | partial-output + halt | abort |

### x2 / x3 resolution

The source prompt's `<section_pipeline>` restatement introduces `x2` node and `x3` node. **Resolution (chosen, with evidence):** x2 and x3 are declared **equivalent in substance to Node C/D and Node E respectively** — i.e., the source prompt restates the C/D/E pipeline in a second vocabulary rather than specifying a nested per-section sub-pipeline.

**Evidence:** the gold-reference artifacts (`node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` in the reference URL's directory) execute the pipeline exactly once, cross-cutting across all 10 sections, with no per-section nested sub-pipelines. If x2/x3 had been a distinct nesting, the gold example would contain per-section C/D/E files; it does not.

**This resolution is an architectural choice, not a silent merge.** It is documented here, it carries through the whole spec, and the design satisfies source-prompt edge case (i) via option (i) — "declare them equivalent to Node C/D/E (with evidence)".

---

## §5. Section-tailored analysis map

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

### Fallback (non-genius Node B)

When Node B type tag is `generic-fallback`: run a single generic extraction pass over the whole Node B, not keyed to sections. Still produce `node-b1-analysis.md` as a flat findings list. Node C ideation proceeds normally from A1 + B1.

### Drift handling (genius-drift)

Run the Map on matched sections only. Unmatched sections are scanned generically and added as flat findings in `node-b1-analysis.md`.

### Section order in B1

1. Headline Insight (first)
2. Remaining sections in the order they appear in the source prompt (Theory Collisions, Discovery vs. Proof, Independence-Verified Bridges, Alternative Hypotheses, Density-Checked Falsification, Scope Limits, Coherence Signals, Generalization Checks, Open Questions & Next Probes)

### Runtime schema enumeration

If the reference analysis at the verbatim URL cannot be read at the moment the Map is applied (e.g., file moved) and the Node B document itself does not expose canonical section names, the skill MUST rely on the drift detection heuristic in §8 and on Node B's actual section structure — **it must not guess a section list** (source prompt escape hatch).

---

## §6. Ideation / synthesis / verification loops

Both levels model the `prompt-epiphany` loop pattern.

### Level A — per-idea solution engineering (in `m-engineer`)

For each accepted idea D_i, run **two separate draft chains** and compare.

```
for d_i in Node D:
    draft_1 = ideate(d_i, A-context)
              → synthesize(target + exact replacement/insertion text + rationale)
              → verify(bug-free, no regression vs. A, not overengineered,
                       info-preserving, target valid)
    draft_2 = ideate(d_i, A-context)       # independent seeding
              → synthesize(...)
              → verify(...)
    best = compare(draft_1, draft_2,
                   criteria = [pro/con, utility, over-engineering-bounded,
                               info-preservation])
    store best in Node E with losing-draft compare_notes
```

- **Inputs:** D_i record + Node A excerpt for `target`.
- **Outputs:** one Node E entry.
- **Termination:** exactly 2 drafts, then compare, then advance. No retry loop inside Level A.
- **"Generate two solutions separately, compare pro/con and utility, over-engineering limited, pass only best solution"** is the mandated rule here (source prompt DO-4; verbatim).

### Level B — final integration (in `m-integrate`)

```
integration_draft = ideate(how to sequence and apply all Node E entries into A)
                    → synthesize(produce draft enhanced A, flush-integrated)
                    → verify(self-audit block §9;
                             precondition: "analysis verified for accuracy
                             and completeness")

if verify fails:
    refine_draft = ideate(fixes based on verify findings)
                   → synthesize(revised draft)
                   → verify(same)
    if verify still fails:
        write partial output + open-questions document
        halt without claiming completion
else:
    write enhanced.md
```

- **Inputs:** Node A + all Node E entries.
- **Outputs:** `enhanced.md` (on PASS) or partial + halt (on second FAIL).
- **Termination:** at most 1 refinement cycle after first-pass FAIL. Bounded.
- **Final-integration precondition (verbatim):** *"create a final version of the input originally held in node A which flushly integrates all solutions held in node E, enhancing the original based on the analysis verified for accuracy and completeness"*.

---

## §7. Two-solutions compare-and-pick rule (Node D → Node E)

Mandated at every D_i → E_i transition.

**Rule (verbatim source language preserved):**
- *"generate two solutions seperately"*
- *"analyze solutions compare pro/con and utility and over-engineering limited"*
- *"pass only best solution"*

**Compare criteria (spec-assigned names for clarity; semantics verbatim from source):**

| Criterion | Meaning |
|---|---|
| pro/con | On-balance gain of the change |
| utility | Practical value of the change to Node A's downstream consumer |
| over-engineering-bounded | Change must be realistic to implement without overcomplexity |
| info-preservation | Change must not cause regression or loss of information from Node A (priority rule 4) |

**Tiebreaker:** if the two drafts are indistinguishable on all four criteria, pick the draft with smaller surface area (fewest lines changed in A). Log both drafts' compare notes.

**Anti-pattern:** a single draft routed to Node E without a compared second draft **violates** DO-NOT-9 (new anti-pattern added to §13).

---

## §8. epiphany-genius version detection and fallback

### Detection heuristic (structural, drift-tolerant)

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

### Decision table

| Count of canonical-or-synonym headers found | Tag | Behavior |
|---|---|---|
| ≥7 | `genius-current` | Run full 10-section Section-Tailoring Map |
| 5–6 | `genius-drift` | Run Map on matched sections; generic scan for unmatched |
| <5 | `generic-fallback` | Skip Map entirely; run generic full-document extraction |

The fallback path preserves the source prompt's intent: *"if it is not [an epiphany-genius report], we still want to be able to perform the improvement to the original input"*.

### Version drift tolerance

The detection heuristic explicitly tolerates:
- Renamed sections (synonyms in the table above)
- Extra sections (unmatched sections are scanned generically in drift mode)
- Missing sections (≤3 missing canonical sections still qualifies as `genius-drift`)
- Reordered sections (order is not part of the detection rule; only presence)

### Reference URL (verbatim, per DO-2)

`file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`

---

## §9. Self-audit verification block

The source prompt's 14-item `<self_audit_checklist>` is a **spec-writing-time** audit: it asks whether the *produced specification document* carries forward the source input's directives. Most items (1, 2, 3, 4, 5, 6, 7, 9, 10, 12, 13, 14) reference *the spec's own text* and cannot be re-run at skill runtime — by runtime the spec is already frozen. Only a subset is runtime-relevant. This section splits the two cleanly: §9.1 is archival, §9.2 is what `m-integrate` actually executes.

### §9.1 — Spec-audit archive (authored-time, not re-run at skill runtime)

The following 14 items are the source-prompt audit of **this spec document itself**, preserved verbatim per DO-1. They were evaluated once when this spec was produced, not by the skill at execution time.

1. Every directive, bracketed item, and quoted snippet in this prompt appears in the spec, unchanged in substance.
2. Every node (A, B, A1, B1, C, D, E) has: purpose, inputs, outputs, storage schema, termination condition, and integration points defined.
3. Every iteration rule from the source input has an explicit termination condition in the spec (caps declared in `<termination_and_budget>` are honored).
4. Both paths are specified: (a) epiphany-genius detection (including version drift) AND (b) the generic fallback for non-genius analyses.
5. The "generate two solutions separately, compare pro/con and utility, over-engineering limited, pass only best solution" rule appears at the Node D → Node E solution-engineering step.
6. The fresh-copy output rule appears in the spec's output-file section: "create a new seperate final specification integrating all solutions from node E, do not overwrite existing specification with the changes, save a fresh copy of the full enhanced output only".
7. No DO NOT / anti-pattern from `<constraints>` is violated by any spec section.
8. Node labels A, B, A1, B1, C, D, E appear verbatim; the reference URL appears verbatim.
9. Ideation / synthesis / verification loops modeled on prompt-epiphany are specified at BOTH (a) per-idea solution-engineering and (b) final-integration.
10. The full self-audit rule is carried forward in the produced spec: "verify all changes integrated flush with self-audit focusing on being bug-free, no errors issues or potential problems".
11. "analysis verified for accuracy and completeness" is honored as a final-integration precondition. *(This item is also runtime-testable — see §9.2 R1.)*
12. The x2/x3 relation to C/D/E is explicitly documented (equivalent, nested sub-pipeline, or flagged ambiguity) — NOT silently merged.
13. The "research / ask questions" directive is explicitly scoped (spec-writer-time, runtime, or both).
14. The pipeline topology is explicitly declared (tree-of-thought OR graph-of-thought) with branching points, convergence/merge points, edge semantics, dynamic-growth rules, loop-pattern integration, cycle/acyclicity rule, AND a concrete diagram or adjacency table. Pure prose descriptions or unjustified topology choices FAIL. Linear-chain-only descriptions FAIL.

### §9.2 — Runtime audit (executed by `m-integrate` before writing `enhanced.md`)

This is the actual audit `m-integrate` runs. It evaluates the **draft `enhanced.md`** and the **run state**, not the spec text.

**Precondition gate** (must be satisfied before integration starts):
- Nodes A1 and B1 exist and are non-empty.
- Node E exists; either non-empty, or empty with a logged reason in `session.json.stop_reasons`.
- Verbatim clause honored: *"analysis verified for accuracy and completeness"* — `m-analyze` must have marked A1 and B1 as complete.

**Runtime checks** (run after the draft `enhanced.md` is assembled, before it is finalized):

- **R1.** *Verbatim:* *"analysis verified for accuracy and completeness"* — A1 and B1 completion flags are set in `session.json`.
- **R2.** Every accepted Node E entry appears in the draft at the location specified by its `target` locator, dispatched by `target.kind`: `heading_path` → traverse heading path; `line_range` → range is preserved structurally (may shift); `anchor` → region between `before` and `after` anchors is present and modified; `verbatim_block` → replacement has been applied at the block's first occurrence.
- **R3.** No Node A content is removed without a matching Node E entry of action `replace` whose replacement text is shorter than the original region AND whose `rationale` cites the removed content. (Content removal is expressed as `replace` with shorter text; there is no standalone `remove` action. Info-preservation; priority rule 4.)
- **R4.** Enhanced A still satisfies every constraint listed in A1's `## Constraints` section (no regression). For each constraint, a substring or semantic-paraphrase match must be present in `enhanced.md`. Any missing constraint is a FAIL.
- **R5.** *Verbatim:* *"verify all changes integrated flush with self-audit focusing on being bug-free, no errors issues or potential problems"* — no **newly-introduced** syntactic defects (unclosed code fences, stray merge markers, unclosed XML tags) that were not already present in Node A; no **newly-duplicated** paragraphs (duplication absent from Node A and not originating from a Node E `insert-*` action); no **newly-introduced** TODO/TBD placeholder tokens (tokens absent from Node A and not present in any applied Node E entry's text). Defects already present in Node A are preserved as-is — info-preservation dominates cleanup.
- **R6.** Output is a fresh file — the original Node A file is untouched on disk (verified by comparing mtime/hash of Node A file, if file-sourced).

### FAIL handling

- **First FAIL:** log findings to `self-audit.md`, run a single refinement cycle (§6 Level B) targeting only the failed checks, re-verify.
- **Second FAIL:** write `enhanced-partial.md` + `open-questions.md` listing unresolved R1–R6 failures with specific check IDs, halt. Do not claim completion. Do not write `enhanced.md`.

Outcome record is always written to `self-audit.md` with PASS or FAIL per check.

---

## §10. Fresh-copy output file convention

### Mandate (verbatim, per DO-6)

*"create a new seperate final specification integrating all solutions from node E, do not overwrite existing specification with the changes, save a fresh copy of the full enhanced output only"*.

### Filename strategy

- **Final output:** `enhanced.md` — always this name, within the per-session directory.
- **Session directory:** `~/docs/epiphany/analysis/<session-id>/`
- **Session ID format:** `YYYYMMDD-<nodeA-stem-slugified>`.
  - Slugification: lowercase → replace non-alphanumerics (except `-`) with `-` → collapse consecutive `-` → trim leading/trailing `-`.
  - **Date-prefix dedup rule:** if the resulting slug starts with `YYYY-MM-DD-` (i.e., the Node A stem was already date-prefixed), strip that leading date-segment before prepending the session's own `YYYYMMDD-`. This prevents redundant-date session IDs.
  - Example: Node A is `docs/superpowers/specs/2026-04-14-prompt-cog-design.md` → stem `2026-04-14-prompt-cog-design` → slugified `2026-04-14-prompt-cog-design` → date-prefix stripped → `prompt-cog-design` → session ID `20260414-prompt-cog-design`.
  - For inline Node A (no file): stem = first 5 non-stopword tokens of the inline content, slugified; fall back to `inline-input` if those tokens yield fewer than 3 characters.

### Collision rule

If `~/docs/epiphany/analysis/<session-id>/` already exists, append `-run-N` with N auto-incremented (2, 3, …). The first collision becomes `<session-id>-run-2`.

### Non-overwrite guarantee (implementation level)

- `m-intake` MUST verify the final output path does not correspond to the Node A source path. If they match (e.g., user accidentally pointed both at the same file), abort before any work with a hard error. Original Node A is never written to by this skill.
- All seven stage files (`node-a.md`, `node-b.md`, `node-a1-analysis.md`, `node-b1-analysis.md`, `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md`) live in the session directory — never in the Node A directory.

### Session directory contents (final)

```
~/docs/epiphany/analysis/<session-id>/
├── node-a.md
├── node-b.md
├── node-a1-analysis.md
├── node-b1-analysis.md
├── node-c-ideas.md
├── node-d-accepted.md
├── node-e-solutions.md
├── enhanced.md            ← final output
├── self-audit.md          ← PASS or FAIL record
├── session.json           ← type tags, pass counts, stop reasons, timing
└── open-questions.md      ← only present on second-FAIL partial
```

### `session.json` schema

`session.json` is the structured run-state record. Format is JSON (the spec mandates structure; implementers may swap to YAML if justified in the plan).

```json
{
  "session_id": "string — YYYYMMDD-<slug>",
  "created_utc": "ISO-8601 timestamp",
  "node_a": {
    "source_kind": "file | inline",
    "source_path": "string or null",
    "content_hash": "sha256 of node-a.md contents"
  },
  "node_b": {
    "source_kind": "file | inline",
    "source_path": "string or null",
    "genius_detection": "genius-current | genius-drift | generic-fallback",
    "matched_sections": ["array of canonical section names matched in detection"],
    "drift_notes": "string — present only if genius-drift"
  },
  "modules": {
    "m-intake":    { "status": "pending|in_progress|completed|skipped|failed", "start_utc": "...", "end_utc": "...", "notes": "string" },
    "m-analyze":   { "status": "...", "a1_complete": true, "b1_complete": true, "start_utc": "...", "end_utc": "..." },
    "m-ideate":    { "status": "...", "passes_run": 0, "ideas_total": 0, "stop_reason": "no-new-ideas | pass-cap | idea-cap" },
    "m-filter":    { "status": "...", "ideas_in": 0, "accepted_out": 0 },
    "m-engineer": { "status": "...", "d_entries_processed": 0, "e_entries_produced": 0, "double_fail_count": 0 },
    "m-integrate": { "status": "...", "refinement_cycles": 0, "audit_verdict": "PASS | FAIL | PARTIAL" }
  },
  "caps_applied": {
    "ideation_passes": 3,
    "ideation_total": 50,
    "drafts_per_d": 2,
    "refinement_cycles": 1,
    "global_wall_clock_sec": 900
  },
  "stop_reasons": ["array of strings — one per module that halted early"],
  "argv_flags": { "start_at": "string or null", "stop_at": "string or null" }
}
```

Unknown-but-present fields are tolerated by consumers (forward-compatibility). Missing required fields fail `m-integrate`'s precondition check.

---

## §11. Termination conditions and iteration caps

All iteration rules from the source prompt have explicit termination conditions (source-prompt DO-12, self-audit item 3).

### Per-node caps

| Boundary | Cap | Source directive bound |
|---|---|---|
| Node C ideation passes | ≤3 | *"iteratively run phase a, passing all ideas generated into storage node c"* + *"run until out of ideas completely"* |
| Node C total ideas | ≤50 | *"each idea stored in database"* (count bound for practicality) |
| Node C no-new-ideas halt | any pass yielding zero new unique ideas halts the loop | Verbatim interpretation of *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"* **as** *"until a full ideation pass generates zero new unique ideas"* (per source-prompt termination_and_budget (c)) |
| Node D evaluation passes | exactly 1 | *"iterate over every idea in node C, be sure we don't miss anything"* |
| Node E solution drafts per D_i | exactly 2 | *"generate two solutions seperately"* |
| Final integration refinement cycles | ≤1 on verify-FAIL, then abort | bounded; no infinite loop |

### Skill-global caps

| Boundary | Cap |
|---|---|
| Global soft wall-clock budget | 15 minutes (total, `m-intake` start → `m-integrate` end) |
| Per-module hard halt | any single module exceeding 10 minutes triggers abort-to-partial |
| Global hard halt | total elapsed wall-clock exceeding 20 minutes triggers abort-to-partial, regardless of which module is running |

The soft 15-min budget is a warning threshold (log a notice to `session.json.stop_reasons`); the 20-min global hard cap and the 10-min per-module cap are enforcement thresholds. Whichever trips first wins.

### "Until a pass generates no new ideas" — the termination interpretation

Source prompt directive: *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"*.

**Interpretation (mandated by source prompt, carried forward here):** ideation stops when a full ideation pass generates **zero new unique ideas**, NOT unbounded looping. Uniqueness is determined by the `(source_section, normalized_observation, target_section)` tuple defined in §2 `m-ideate`; near-duplicates are collapsed with a logged merge.

### Soft time / token budget guidance

Soft default: 15-minute wall-clock budget, measured from `m-intake` start to `m-integrate` end. Modules log their own elapsed time to `session.json`. These are defaults; an implementation may adjust based on measured performance (e.g., if empirical runs consistently complete under 5 minutes, the global budget may be tightened).

### Priority rule (tiebreakers — verbatim from source prompt, 4 independent rules)

1. **Completeness vs. process-time budget** → prefer completeness bounded by caps above. Ideation stops at **no-new-ideas pass OR cap hit, whichever first**.
2. **Creative exploration vs. realistic-to-implement** → prefer realistic-to-implement. Creative ideation is free at Node C; **Node D MUST reject any idea that is not realistic-to-implement, would cause regression, or would overengineer the result**.
3. **Genius-tailored vs. generic flow** → if Node B is detected as `genius-current` or `genius-drift`, use Map; otherwise fallback.
4. **Novelty vs. information preservation** → if an idea would cause regression or loss of information from Node A, discard regardless of novelty.

---

## §12. Integration points with the epiphany-genius modular system

Each module in §2 exposes a named handle (input/output port) consumable by other `epiphany-*` skills. Integration is file-based: other skills can run off any stage file in the session directory.

### Module handle summary

| Module | Exposes | Consumable by |
|---|---|---|
| `m-intake` | `input_primary` (= `node-a.md`), `input_analysis` (= `node-b.md`), type tags (`session.json`) | Any downstream `epiphany-*` skill that wants the same intake |
| `m-analyze` | `analysis_of_primary` (= `node-a1-analysis.md`), `analysis_of_analysis` (= `node-b1-analysis.md`) | Skills that want pre-computed analysis to skip re-analysis |
| `m-ideate` | `idea_pool` (= `node-c-ideas.md`) + pass count + stop reason | Skills that want to consume a raw idea pool for their own filtering |
| `m-filter` | `accepted_improvements` (= `node-d-accepted.md`) | Skills that want the filtered-but-not-yet-engineered list |
| `m-engineer` | `solutions_store` (= `node-e-solutions.md`) | Skills that want the ready-to-apply changeset |
| `m-integrate` | `enhanced_output` (= `enhanced.md`), `self-audit` verdict | Terminal |

### Replay / partial-run contract

- Each module is independently replayable.
- **How the caller targets an existing session:** via `--session-dir=<path>` (see §1 activation triggers). Without `--session-dir`, the skill always creates a fresh session directory (new `YYYYMMDD-<slug>` or `-run-N` on collision).
- **Partial-run flags:** `--start-at=<module-name>` and `--stop-at=<module-name>` (module names from §2: `m-intake`, `m-analyze`, `m-ideate`, `m-filter`, `m-engineer`, `m-integrate`). Both require `--session-dir`. `--start-at` requires all prior stage files to exist in that directory. `--stop-at` halts after the named module without writing `enhanced.md`.
- **Manual replay fallback:** when invoked with `--session-dir` and no `--start-at`, the skill auto-detects the first module whose output file is missing and resumes from there.

### Forward-compatibility with `epiphany-genius` modular system

- All stage files use markdown with structured headers, matching the gold example's schema — this is the same format `epiphany-genius` produces for its report. A future `epiphany-genius` module can read any stage file directly.
- Module handle names (`input_primary`, `idea_pool`, etc.) are chosen to be generic across the `epiphany-*` family, not skill-specific.
- Storage hint from source prompt (*"all databases in system expand and create relationships as neccessary for ideal design"*) is honored by using cross-referenced IDs (I01 → D03 → E07) rather than flat copies.

---

## §13. Anti-patterns and DO NOT list

### Source-prompt DO-NOT list (7 items, verbatim intent)

1. **DO NOT remove, paraphrase, or summarize any directive, bracketed item, or quoted snippet from the source prompt** — every one appears in this spec in substance.
2. **DO NOT introduce infinite loops** — every iterate / run-until rule has a termination condition (§11).
3. **DO NOT overengineer.** *"must be realistic to implement without overcomplexity or regression"*. *"we want the best product features that are usable for humans"*.
4. **DO NOT produce a spec** — or at runtime, a change — **that would cause regression or loss of information from Node A during integration.** (Priority rule 4.)
5. **DO NOT silently merge the Nodes A–E pipeline with the parallel x2/x3 restatement.** x2/x3 is documented as equivalent in §4 with gold-reference evidence.
6. **DO NOT overwrite the original specification.** Fresh copy only (§10).
7. **DO NOT assume a literal persistent database.** *"ai optimized database"* is interpreted as markdown stage files (§3).

### Spec-added anti-patterns (discovered during design)

8. **DO NOT let the runtime skill ask the user questions or perform external research.** Runtime is self-contained (§1). The "do research or ask questions as neccessary" directive is **scoped to the spec-writing phase only** (§1 runtime scope).
9. **DO NOT generate Node E entries outside the two-drafts-pick-best protocol.** Every Node E entry is the output of a compare over exactly 2 independently drafted solutions (§7).
10. **DO NOT process other sections before Headline Insight when Node B is genius-or-drift.** (§5 edge case (f).)
11. **DO NOT treat Alternative Hypotheses as a source of multiple competing ideas.** Only the best-fit hypothesis is used (§5 edge case (e)).
12. **DO NOT write to the Node A directory.** All outputs live in the session directory (§10).
13. **DO NOT pick a section list at runtime by guessing.** If the reference URL is unreadable and Node B lacks canonical headers, rely on the drift detection heuristic and Node B's actual section structure (source-prompt escape hatch).

### Edge-case matrix (10 cases from source prompt, all resolved)

| # | Case | Resolution |
|---|---|---|
| a | epiphany-genius current version | Full 10-section Map (§5, §8) |
| b | epiphany-genius version drift | Partial Map + generic for unmatched (§8) |
| c | non-epiphany-genius analysis | Generic fallback full-document scan (§5, §8) |
| d | theory collisions handling | Verbatim special handling in §5 row 2 |
| e | alt hypotheses low-signal | Verbatim special handling in §5 row 5 |
| f | headline insight primary anchor | Processed first in §5 row 1 |
| g | Node A source is a file | Handled at `m-intake` (§1, §2) |
| h | zero-ideas section | Mark complete and skip (§3 Node B1) |
| i | x2/x3 parallel pipeline relation to C/D/E | Declared equivalent with gold-reference evidence (§4) |
| j | research / ask-questions scoping | Spec-writer only; runtime self-contained (§1) |

### Open questions

**None blocking.** All source-prompt ambiguities were resolved during the brainstorming that produced this spec:

- Naming — `epiphany-analysis` takes over the placeholder slot.
- Pipeline shape — hybrid cross-cutting C/D/E with section-tailored extraction in B1.
- x2/x3 — equivalent to C/D/E, evidenced by gold-reference artifacts.
- Storage — markdown stage files matching gold example.
- Output location — `~/docs/epiphany/analysis/<session-id>/`.
- Session ID format — `YYYYMMDD-<nodeA-stem-slugified>`.
- Activation — slash command with optional file-path args.
- Termination caps — Node C ≤3 passes / ≤50 ideas / no-new-ideas halt; Node D 1 pass; Node E 2 drafts per D_i; Final integration ≤1 refinement cycle; global 15-min soft budget.
- Research/ask-questions scope — spec-writer only; runtime self-contained.

Implementation-phase open questions (to be resolved by the writing-plans skill, not by this spec):
- Concrete idea-ID generation scheme (simple counter vs. hash-prefixed).
- Exact slugification function (pick a Python / shell implementation).
- Whether `session.json` is JSON or YAML (the spec requires structured metadata; format is implementer's choice).
- Failure-logging verbosity (structured fields vs. prose).

---

## §14. Test plan

The implementation agent MUST provide the following tests before the skill is declared complete. Tests live in `~/.claude/skills/epiphany-analysis/tests/`. The gold-reference artifacts at `/home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/` are the primary acceptance oracle.

### §14.1 — Gold-reference acceptance test (primary oracle)

**Fixture:**
- Node A: the spec document Node B analyzes. Locate by reading the frontmatter or opening section of `report.md` — it names the analyzed document directly (typical form: "analyzing specification X at `<path>`"). The implementer follows that reference to identify the exact Node A used to produce the gold C/D/E.
- Node B: `/home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`.
- Gold C/D/E: `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` in the same directory.

**Protocol:**
1. Run `epiphany-analysis` pointed at the Node A + Node B pair above.
2. Diff the skill's `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` against the gold reference.
3. Diff the skill's `enhanced.md` against the outcome a spec-review run would produce if the gold D/E were applied by hand.

**PASS criteria (thresholds, not exact match — LLM non-determinism is expected). Thresholds below are v1-seed values based on one gold run; recalibrate after three empirical runs against varied Node A/B pairs and adjust to reflect observed spread.**
- **C-coverage:** skill's Node C contains ≥70% of the 28 gold idea topics (by normalized observation; paraphrases count). Missing ideas must be in structurally low-signal sections (Alternative Hypotheses, Tensions with higher-priority items).
- **D-coverage:** ≥80% of the 14 gold-accepted ideas (D01–D14) are present in the skill's Node D, identifiable by (technique, target_section, action) tuple.
- **E-coverage:** ≥80% of the 14 gold E entries (E01–E14) are present in the skill's Node E, identifiable by target-string-similarity ≥0.7 against the gold's target description (compare against whichever target-field string representation the entry uses: `heading_path` joined, `verbatim_block` first-line, `anchor.before`, or `line_range` normalized).
- **E-format:** 100% of skill's Node E entries have the structured 4-field target locator shape (§3 Node E).
- **Integration:** every skill-produced Node E entry appears in `enhanced.md` at its declared target.
- **No-regression:** every constraint A1 extracted from the original Node A is still satisfied in `enhanced.md`.

### §14.2 — Runtime-audit unit tests (`m-integrate` §9.2)

- **R1 test:** run with `m-analyze` aborted before A1 completion; expect precondition failure.
- **R2 test:** inject a forged Node E entry with a nonexistent `target.anchor`; expect R2 FAIL + single refinement cycle + second-FAIL partial output.
- **R3 test:** author Node E entries whose accepted `remove` action strips a requirement that A1 flagged as a constraint; expect R3 FAIL.
- **R5 test:** inject malformed residue (unclosed code fence) into draft; expect R5 FAIL.
- **R6 test:** verify source Node A file mtime+hash is unchanged after a full run.

### §14.3 — Pipeline-structural tests

- **Two-solutions rule:** scan `session.json.modules.m-engineer.d_entries_processed == e_entries_produced + double_fail_count`. Any delta indicates silent drops — FAIL.
- **Termination caps:** feed an adversarial Node B designed to elicit runaway ideation; verify `m-ideate` halts at cap 3 passes OR cap 50 ideas, whichever first.
- **No-new-ideas halt:** feed a trivially-exhausted Node B; verify ideation halts on pass 1 with `stop_reason: "no-new-ideas"`.
- **Empty-E graceful halt:** feed a Node B with zero actionable content; verify `m-filter` produces empty D, `m-engineer` produces empty E, `m-integrate` emits `enhanced.md` == `node-a.md` (no changes) + `session.json.modules.m-integrate.audit_verdict: "PASS"`.

### §14.4 — Non-genius fallback test

Run with Node B = a generic prose analysis (no canonical headers). Verify:
- `session.json.node_b.genius_detection == "generic-fallback"`.
- `m-analyze` uses generic full-document scan path (§5 Fallback).
- Pipeline completes with non-empty Node E.

### §14.5 — Drift-detection test

Run with Node B = a modified copy of the gold report with two canonical section headers renamed synonymously (e.g., "Headline Insight" → "Principal Finding"). Verify:
- `session.json.node_b.genius_detection == "genius-drift"`.
- `matched_sections` contains the unaltered canonical headers.
- `drift_notes` names the two renamed sections.
- Fallback is applied for those two sections; canonical Map for the rest.

### §14.6 — Partial-run contract test

- Run full pipeline to produce all stage files.
- Delete `node-e-solutions.md`.
- Re-invoke with `--start-at=m-engineer`. Verify only `m-engineer` + `m-integrate` execute and earlier stage files are unchanged (mtime/hash preserved).
- Re-invoke with `--stop-at=m-filter` from a fresh session; verify `enhanced.md` is NOT written and modules after `m-filter` are not run.

### §14.7 — Non-regression against existing skills

- `epiphany-analysis` never modifies `~/.claude/skills/prompt-epiphany/` or `~/.claude/skills/epiphany-genius/` directories.
- `epiphany-analysis` never invokes the brainstorming skill at runtime (anti-pattern 8).

---

## End of specification

This spec is self-contained. A future implementation agent builds the skill strictly from these 14 sections. All source-prompt directives, iteration rules, verification rules, edge cases, and bracketed items are captured in substance. All 14 source-prompt self-audit checklist items are carried forward verbatim in §9.1. The reference URL and node labels (A, B, A1, B1, C, D, E) appear verbatim throughout.
