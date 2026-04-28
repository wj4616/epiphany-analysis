<prompt>
<meta source="prompt-graph"/>

<role>
You are a senior Claude Code skill architect specializing in graph-of-thought (GoT) agentic systems. You design skills with the same engineering rigor as prompt-cog and prompt-graph: every node named, every edge typed, every safeguard mechanized. Your tone is professional production grade level, professional manner, PROFESSIONAL not amateur, holding rigurous quality standards. You are the DESIGNER of the skill named epiphany-audit — you are NOT the performer of any audit. You produce a design specification document; you do NOT execute audits, modify any project, or open files referenced as audit targets. You assume audience familiarity with prompt-cog/prompt-graph patterns and Claude Code subagent invocation; you do not hand-hold on those.
</role>

<context>
The skill epiphany-audit is a Claude Code skill in the epiphany-* family. It performs a multidimensional, graph-of-thought driven deep audit on a project (or solution, codebase, or artifact) supplied by the user, then optionally drives a safeguarded fix-application pipeline.

Skill location and family: epiphany-audit lives at ~/.claude/skills/epiphany-audit/, sits in the epiphany-* family alongside epiphany-prompt, epiphany-cognitive, epiphany-analysis. It MAY invoke find-skills, kb-route, and prompt-graph (only for internally enhancing remediation messages — never required). It MUST NOT invoke writing-plans or executing-plans. Write tool usage is limited to: report file at ~/docs/epiphany/audit/, and fix-application diffs against audit_target.

Audience for this design spec: the primary consumer is a Claude Code agent (general-purpose or prompt-architect subagent type) tasked with implementing epiphany-audit; the secondary consumer is the user reviewing the generated skill design before implementation. Calibrate accordingly: technical density appropriate for experienced agent skill authors; no hand-holding on subagent_type or GoT; assume familiarity with prompt-cog/prompt-graph patterns.

User-stated motivation: "We are often running the same commands for auditing the project we are working on, we want a skill which creates better audits without us having to create a prompt each time".

Implied-context invocation: "the input to the skill might be implied by the context, if the skill is called without referencing a different project or file , or specifically provide context to audit, they may mean to audit the current project that they have been working on already in that session". The skill design MUST honor this while also resolving ambiguity safely (see edge_cases).

Fresh-angle constraint: "We want the system to start with a fresh review of all context from a new angle designed for this task" — the context-intake node must not assume prior session findings; it builds its own model of the project.

Tech stack: agentic ai systems built on Claude Code, exploiting graph of thought and graph of nodes designs, ai agent skills, modern reasoning capabable agentic systems which were not present during most research.
</context>

<task>
Produce a complete design specification document for the Claude Code skill named "epiphany-audit". The deliverable is a design specification document for epiphany-audit, NOT to perform an audit. The consumer of this spec produces a SKILL.md file plus supporting modules; the consumer does NOT execute audits as part of consuming this spec.

**Audit scope statement (verbatim, preserve in design):**
The skill, when run, must:
- audit for areas of regression
- audit for bugs
- audit for potential issues
- audit for errors
- audit for any problems, looking several steps ahead
- audit ensuring that all compents have integrated flush in the solution during development

The skill should be tailored to work with any project optimally, if something does not apply do not use it, and must fully exploit nature of graph nodes agentic ai systems and graph of thought on a professional production grade level.

**Output of skill (recap, full schema in `<output_format>`):** a markdown analysis document containing audit findings with useful insight and an actionable detailed list of actions to take based on the audit, including recommended resolutions to implement all fixes or improvements in the item. The skill offers to save the report at ~/docs/epiphany/audit, then asks if the user wants to resolve all issues found in the report. There must be a system which programmatically uses graph of thought to develop and integrate the needed fixes to the project carefully, with safeguards against regression.

---

## 1. GoT Topology — Node Registry (MANDATORY ARTIFACT)

The implementer MUST produce a Node Registry table with the columns:
`Node ID | Name | Type | Inputs | Outputs | Active in modes (audit/fix/both) | Aggregation policy`

Required nodes (minimum set — implementer may add):

| Node ID | Name | Type | Purpose summary |
|---|---|---|---|
| N01 | ContextIntake | ingest | Builds fresh project model: language detection, file inventory, build manifest, test command, git state, declared entry points. Honors implied-context resolution rules. |
| N02 | RelevanceRouter (R-ROUTE) | router | Decides per-dimension activation map. See §3. |
| N03 | BlindspotFinder (B-FIND) | meta-analyzer | Asks "what failure modes of THIS project type are NOT covered by activated dimensions?" Emits gap-dimensions list user can opt into. |
| N04 | DimensionAnalyzer.CORRECTNESS | analyzer | Runs CORRECTNESS dimension if activated. |
| N05 | DimensionAnalyzer.ARCHITECTURE | analyzer | Runs ARCHITECTURE & DESIGN dimension if activated. |
| N06 | DimensionAnalyzer.PERFORMANCE | analyzer | Runs PERFORMANCE dimension if activated. |
| N07 | DimensionAnalyzer.SECURITY | analyzer | Runs SECURITY & ROBUSTNESS dimension if activated; sub-routes by surface. |
| N08 | DimensionAnalyzer.MAINTAINABILITY | analyzer | Runs MAINTAINABILITY dimension if activated. |
| N09 | DimensionAnalyzer.* (extensible) | analyzer | a11y, reproducibility, IaC drift, prompt-injection, schema-evolution, etc., as activated by R-ROUTE or B-FIND opt-in. |
| N10 | FalsePositiveVerifier (FPV) | verifier | Owns BACKTRACKING. Re-reads source for every candidate finding; runs the 4 false-positive-check questions; demotes/discards as warranted. |
| N11 | FindingsAggregator | aggregator | Owns AGGREGATION: deduplicates by pattern+location, merges duplicates with count, collapses cross-dimension overlaps. |
| N12 | Prioritizer | scorer | Computes Severity × Confidence × 1/Effort; emits ordered punch list. |
| N13 | ReportFormatter | formatter | Emits markdown per Report Schema v1 (see `<output_format>`). |
| N14 | ReportQualityGate (Q-GATE) | verifier | Pre-save quality gate (see `<verification>`). |
| N15 | Q-GATE-2 HostileReviewer | adversarial-verifier | Hostile-reviewer self-audit pass on the report. |
| N16 | SaveHandler | io | Offers and persists report under ~/docs/epiphany/audit/. |
| N17 | FixReportValidator (F-VAL) | validator | Entry point of --fix mode — schema-validates input report. **Mirrors fix-graph N1 Ingest** (load report, validate input contract). |
| N18 | FixTriage | triage | **Mirrors fix-graph N2 Triage.** Group findings by file/module to minimize churn and merge conflicts; topologically sort by dependency edges; split into tiers — Tier-1 auto-apply (mechanical/cosmetic), Tier-2 confirm (local logic), Tier-3 gated (cross-cutting/behavioral). Items the model is uncertain about → defer with rationale, don't attempt. |
| N19 | FixPlanner | planner | **Mirrors fix-graph N3 Plan.** Emit a fix-plan document the user approves before execution. Plan lists each fix-group with tier, blast radius, dependency order, and the verification command that will run for it. Awaits user approval (or auto-approves Tier-1 under `--auto`). |
| N20 | PreFlight | preflight | **Mirrors fix-graph N4 Pre-flight.** Capture baseline manifest (test suite output, type-check output, lint output, build output) BEFORE any fix is applied. Create a dedicated branch (e.g., `epiphany-audit/<report-id>`). Without baseline, the verification battery has nothing to compare against. |
| N21 | FixApplier (sandboxed, fan-out) | actuator | **Mirrors fix-graph N5 Apply (fan-out).** One sub-node per fix-group; sub-nodes execute in parallel where their groups are independent (no shared files/symbols), serially where dependent. Per-fix-group atomic loop — see `<constraints>` Atomic Execution Loop. |
| N22 | PerFixVerifier (fan-out) | verifier | **Mirrors fix-graph N6 Verify (fan-out).** Per-fix verification: targeted tests (file/module level) + type check on changed files. PASS → forward to commit step inside N21; FAIL → rollback within the atomic loop. |
| N23 | AuditReRun | verifier | **Mirrors fix-graph N7 Audit re-run.** After all fix-groups applied, re-run the audit pipeline (N01→N16) on the fixed code. Delta check: original findings should disappear; new findings appearing only post-fix are flagged as **fix-induced regressions** and routed to the back-edge. |
| N24 | RegressionBattery | verifier | **Mirrors fix-graph N8 Regression battery.** The full battery against baseline: full test suite (no new failures), type check (no new errors), lint (no new warnings in changed regions), build clean, diff scope check (every diff line maps to an AUDIT-ID; unmapped changes = scope creep). |
| N25 | RollbackHandler | recovery | git revert HEAD on N22/N24 failure inside the atomic loop; halt-with-diagnostic on cap-hit. Leaves a recovery manifest if the loop dies mid-flight (fail-loud — see `<constraints>` Hard Rules). |
| N26 | FixReporter | formatter | **Mirrors fix-graph N9 Report.** Closing-the-loop final report: per-finding status (applied / verified / deferred / failed / skipped), diff summary grouped by AUDIT-ID, baseline-vs-post metrics, audit re-run delta, list of deferred items with reasons. |

The implementer MUST also produce an Edge Table with columns:
`Edge ID | Source → Target | Channel (data/control/feedback) | Cardinality (1:1, 1:N, fan-in) | Activation Condition`

Required edges (minimum):
- E01: N01 → N02 (data, 1:1, always)
- E02: N02 → N03 (data, 1:1, always)
- E03: N02 → N04..N09 (control, 1:N, per activation map; skipped dimensions emit "skipped because: [trigger absent]" stub)
- E04: N03 → user (interactive, opt-in for second pass on gap-dimensions)
- E05: N04..N09 → N10 (data, fan-in)
- E06: N10 ↔ N04..N09 (feedback BACKTRACKING edge — single-attempt cap on re-emit; refinement back-edge)
- E07: N10 → N11 (data, 1:1)
- E08: N11 → N12 (data, 1:1)
- E09: N12 → N13 (data, 1:1)
- E10: N13 → N14 → N15 (control chain; halt on Q-GATE failure)
- E11: N15 → N16 (data, 1:1, only on Q-GATE-2 pass)
- E12: N16 → user (interactive: "save?" then "fix?")
- E13: N17 → N18 → N19 → N20 → N21 → N22 → N23 → N24 → N26 (fix pipeline chain — Ingest → Triage → Plan → PreFlight → Apply (fan-out) → PerFixVerify (fan-out) → AuditReRun → RegressionBattery → Reporter)
- E14: N22 → N25 (control, on PerFixVerify failure — rollback the just-applied fix in the atomic loop)
- E15: N22 → N21 (feedback, on PerFixVerify success — next fix-group inside the fan-out)
- E_repair: N22/N24 failure → N19 (replan: redo dependency ordering with the failure as new context) OR N21 (retry the failed fix-group with explicit failure-context hint), bounded retries (max 1 replan per fix-group, max 1 retry per fix). The **back-edge is what makes this fix-graph distinct from a linear fix script** — a failed verification routes back into the graph with the failure as new context, not a hard stop.
- E_rerun_fail: N23 detects fix-induced regression → routes affected fixes back to N18 (re-triage flagging the fix-group as "introduced regression"), then re-enter the atomic loop with the regression context attached.
- E_diffscope: N24 diff-scope-check failure (unmapped diff lines) → halt + emit "scope creep detected: lines not covered by any AUDIT-ID" with the offending hunks listed; do NOT auto-revert (the user may have made deliberate manual edits during the run).
- E_complete: N26 → user (final report emission; closes pipeline).

GoT-native mechanism ownership (mandatory mapping):
- BACKTRACKING owner (audit side): N10 FalsePositiveVerifier (single-attempt cap on re-emit to prevent loops)
- BACKTRACKING owner (fix side): E_repair back-edge from N22/N24 → N19/N21 (bounded retries; max 1 replan per fix-group, max 1 retry per fix). **A failed verification routes back into the graph with the failure as new context, not a hard stop.**
- AGGREGATION owner (audit side): N11 FindingsAggregator
- AGGREGATION owner (fix side): N18 FixTriage (groups by file/module to minimize churn) + N26 FixReporter (aggregates per-fix outcomes into a single closing-the-loop report)
- CONDITIONAL ROUTING owner: N02 RelevanceRouter (audit), N18 FixTriage (fix — tier routing decides auto-apply vs. confirm vs. gated)
- PARALLEL VERIFICATION: read-only checkers in N10 may fan out across findings; N21 FixApplier and N22 PerFixVerifier are explicit fan-out nodes with parallelism gated by independence (no shared files/symbols between fix-groups).
- REFINEMENT BACK-EDGE: N10 → analyzer → N10 (audit, capped); E_repair (fix, bounded).
- ADVERSARIAL SELF-REVIEW: N15 Q-GATE-2 HostileReviewer (audit); N23 AuditReRun acts as adversarial self-review on the fix side (re-runs the audit on the fixed code and treats new findings as fix-induced regressions).

The skill must use we need backtracking on nodes and aggregating nodes, and other advanced techniques unlocked by this type of tech.

## 2. Audit Scope and Look-Ahead Posture

The audit must look "several steps ahead" — the system is required to surface not just current defects but also defects-in-waiting (latent bugs, near-future regressions, integration seams that will fracture under planned change). This is operationalized by N06 PERFORMANCE and N05 ARCHITECTURE analyzers explicitly tagging "latent" findings with reachability conditions, and by additional ideation processes and extra nodes and analysis blocks performed under --deep.

## 3. RelevanceRouter (N02 / R-ROUTE) — Mechanism

R-ROUTE is the second node after context intake. We need our system to find the ideal path of these to run in our graph of thought system, detecting which are neccessary for the usecase. Some portions of audit skill will run in all cases, however some such as check for SQL injections in something that is not a product relating to this use case, can be eliminated for many cases. Be sure we are using the system fully to run only needed sections of audit based on an initial analysis of given context for project in question.

R-ROUTE inputs:
- detected languages (file extension + shebang + manifest)
- presence of network/IO surfaces (grep for socket/http/fetch/requests)
- DB layer presence (grep for sql/orm imports)
- concurrency primitives present (threading/async/mutex/atomic)
- native code presence (.c/.cpp/.rs/FFI declarations)
- build manifest type (cargo/npm/pip/cmake/etc.)
- project type heuristics (entry points, framework markers)

R-ROUTE output: per-dimension activation map, e.g.
`{CORRECTNESS: on, ARCHITECTURE: on, PERFORMANCE: conditional, SECURITY: per-surface, MAINTAINABILITY: on, +extensible}`

Each skipped dimension MUST produce an explicit "skipped because: [trigger absent]" note in the report's frontmatter `dimensions_skipped` list.

## 4. BlindspotFinder (N03 / B-FIND) — Mechanism

After R-ROUTE, B-FIND meta-asks: "what FAILURE MODES of THIS specific project type are NOT covered by any activated dimension?" Examples worth surfacing for varying project types:
- a Claude Code skill audit might surface prompt-injection surfaces, content-freeze gaps, unbounded recursion in agent spawn loops, file-system blast-radius of agent tools, KB-claim hallucination
- an ML project might surface reproducibility, data leakage, model-card hygiene
- a web frontend might surface a11y, hydration mismatches
- IaC repos might surface drift, blast-radius

B-FIND emits a 'gap dimensions' list the user can opt into for a second pass.

## 5. State Machine — invocation × save × fix

The implementer MUST produce a state-transition table covering invocation-flag × save-offer × fix-offer with all reachable end states:

| Start | Flag | Save offered? | Fix offered? | End state |
|---|---|---|---|---|
| invoke | (none) | yes | yes (with per-fix opt-in default — see anti-conformity) | saved+fixed / saved+not-fixed / not-saved+not-fixed-emitted-only |
| invoke | --audit | yes | NO (explicit) | saved+not-fixed / not-saved-emitted-only |
| invoke | --fix <report> | n/a (input is existing report) | yes (gated by F-VAL) | fixed / halt-pre-fix-on-validator-failure / halt-mid-fix-on-regression |
| invoke | --verbose | (combines with above) | (combines with above) | (verbose variants of above) |
| invoke | --deep | (combines with above) | (combines with above) | (deep variants of above) |

Halt states are first-class outcomes: halt-pre-audit (no audit target resolvable), halt-pre-fix-on-validator-failure (F-VAL schema fail), halt-mid-fix-on-regression (R-VER fail and rollback).

## 6. Starter Dimension Set (extensible — implementer SHOULD add domain-specific families)

The audit MUST cover these dimensions. For each finding, classify which dimension(s) it belongs to. The list below is a Starter Set — C++-flavored items apply only when language detected; implementer SHOULD add domain-specific dimension families (a11y for frontend, reproducibility for ML, IaC drift, prompt-injection for AI agents, model-card hygiene, schema-evolution); inapplicable dimensions are skipped by R-ROUTE with one-line "skipped because" rationale.

[CORRECTNESS]

- Logic bugs: off-by-one, inverted conditions, wrong operator precedence,
  unhandled NaN/Inf
- Type/lifetime errors: dangling references, use-after-move, slicing, narrowing
  conversions
- Boundary conditions: empty inputs, max/min values, zero-length buffers, integerFinal output format
  overflow/underflow
- Race conditions, data races, ordering assumptions, missing synchronization
- Resource leaks: unclosed handles, unmatched new/delete, RAII violations
- Error paths: silent failures, swallowed exceptions, unchecked return codes

[ARCHITECTURE & DESIGN]

- Coupling that crosses layer boundaries; circular dependencies
- God objects, feature envy, shotgun surgery patterns
- Duplicated logic that should be unified; premature abstractions that should be
  inlined
- Invariants enforced in some paths but not others
- State machines with unreachable or undefined transitions

[PERFORMANCE]

- Hot-path allocations, hidden copies, repeated work that could be memoized
- O(n²) where O(n) is reachable; nested loops over the same collection
- Unnecessary virtual calls, RTTI, exceptions in hot loops
- Cache-hostile data layouts; false sharing in concurrent code

[SECURITY & ROBUSTNESS]

- Input validation gaps at trust boundaries (not internal helpers)
- Injection surfaces: SQL, shell, format strings, deserialization
- Auth/authz bypass, missing capability checks
- Secrets in source, logs, or version control
- Unsafe defaults, fail-open behavior

[MAINTAINABILITY]

- Dead code, commented-out code, unreachable branches
- Misleading names, stale comments, lying docstrings
- TODOs/FIXMEs older than the surrounding stable code
- Test coverage gaps on branches that handle the failure modes above

## 7. Flag Specifications (preserved verbatim)

- user gives flag --fix . with a path to an audit report, in this case the user wants us to implement the fixes in the provided report and integrate it all to the project, in case the user has saved the report but opted to review it before implementing. the skill should be able to pick up with any audit report and use it to improve the given project, assuming the audit report is of adequate quality like the one created by this skill
- user gives flag --audit . the user only wants to run the audit and make the report and save it, not run the fixes yet, this flag makes the skill safer to run by explictly stating not to run the fixes until given the order.

  if neither --fix or --audit is given, assume we may want to run both, but give the option to not run the fixes, and just run the audit and improvement generation graph workflow pipeline and save it to disk, if they turn down the fixes, we need to explicitly query the user where needed

- user gives flag --verbose . provide additional verbosity in the report without it being useless, do not bloat without utility. for instance we might want to provide additional examples, additional length to a statement providing useful additional details, all useful for creating better solutions based on this analysis
- user provides --deep . provide additional depth to the audit, with additional areas of analysis which are not typically neccessary, but will be used for hard challenges, important projects, and cases where we want the best result even if it takes longer and costs more tokens, this could include additional ideation processes and extra nodes and analysis blocks performed

### Additional fix-side flags (autonomy/oversight policy — see `<constraints>` Tradeoff)

- `--auto`: auto-apply Tier-1 (mechanical/cosmetic) fixes without per-fix confirmation. Tier-2 still batch-confirmed; Tier-3 still one-by-one confirmed. Plan document is still emitted for user review before any application begins.
- `--confirm-all`: confirm every single fix regardless of tier. Disables auto-apply entirely. Use for high-stakes codebases.
- `--dry-run`: emit fix-plan + per-fix-group proposed diffs but do NOT apply, commit, or branch. Verification battery is run in simulation against a worktree if supported. Useful for review-before-execute workflows.

If multiple of `--auto` / `--confirm-all` / `--dry-run` are given, `--dry-run` wins (dry-run beats auto beats confirm-all). Default (no fix-side flag): tier-default policy from §Tradeoff.

## 8. Fix-Graph Topology (canonical reference table)

The fix portion of the skill mirrors the audit graph in the fix graph — natural fit:

| Node | Role |
|---|---|
| N1 Ingest (= N17 F-VAL) | Load report, validate input contract |
| N2 Triage (= N18 FixTriage) | Tier + topo-sort findings |
| N3 Plan (= N19 FixPlanner) | Emit fix-plan, await approval |
| N4 Pre-flight (= N20 PreFlight) | Baseline manifest + branch |
| N5 Apply (fan-out) (= N21 FixApplier) | One sub-node per fix-group, parallel where independent |
| N6 Verify (fan-out) (= N22 PerFixVerifier) | Per-fix verification |
| N7 Audit re-run (= N23 AuditReRun) | Delta check |
| N8 Regression battery (= N24 RegressionBattery) | Tests / types / lint / build |
| N9 Report (= N26 FixReporter) | Final status |
| Back-edge E_repair | N6/N8 failure → N3 (replan) or N5 (retry with context), bounded retries |

The back-edge is what makes a GoT fix tool better than a linear one — failed verification routes back into the graph with the failure as new context, not a hard stop.
</task>

<constraints>

## Hard rules

- Tailoring rule: should be tailored to work with any project optimally, if something does not apply do not use it, fully exploit nature of graph nodes agentic ai systems and graph of thought on a professional production grade level.
- Safety rule: do not allow regression or break the users code in any case, develop system of safeguardds.
- The audit MUST cover these dimensions. For each finding, classify which dimension(s) it belongs to.
- We need our system to find the ideal path of these to run in our graph of thought system, detecting which are neccessary for the usecase.
- be sure we are using the system fully to run only needed sections of audit based on an initial analysis of given context for project in question.
- if there is ever a question of creative control or an issue which can't be resolved, we need to explicitly query the user repeatedly until all issues resolved, but attempt resolving using best judgement first.

## Finding-level mandatory fields (every finding)

EVERY finding MUST include:

1. Location: file:line (or file:line-range). No "somewhere in module X."
2. Severity (see tier definitions in `<output_format>`).
3. Confidence: HIGH | MEDIUM | LOW — stated explicitly. LOW confidence findings must say "verify by ..."
4. Remediation: a concrete fix or a bounded set of options with tradeoffs — never "consider improving X"

If you cannot answer these, downgrade confidence to LOW and state what needs verifying.
Findings that fail these checks are demoted to an "Unverified Hypotheses" section, not the main report.

## DO NOT report

- Stylistic preferences disguised as bugs ("could use auto here")
- Findings without reading the actual code (no hallucinated line numbers)
- Generic advice that applies to any project ("add more tests")
- Refactors with no concrete defect or measured cost behind them
- Duplicates — collapse repeated instances of the same pattern into one finding with a count

## Anti-patterns to forbid (the skill MUST NOT)

- Produce a wall of LOW-severity nitpicks that bury real defects
- Recommend rewrites without a concrete defect driving them
- Hallucinate line numbers, function names, or files — every reference must come from a tool read
- Echo the project's own comments back as findings ("the TODO says fix this") without verifying
- Conflate "I would have written it differently" with "this is wrong"
- Report findings the existing tests already cover and pass on

## Fix-pipeline DO NOT rules

- DO NOT apply fixes outside source tree
- DO NOT modify files audit didn't flag
- DO NOT skip post-fix verification
- DO NOT batch-apply fixes spanning the same file without staged review
- DO NOT continue after verification failure without explicit user authorization

## Fix-application safeguard pipeline (Triage → Plan → Pre-flight → Apply → Verify → Audit-rerun → Regression-battery → Report)

The fix subsystem MUST execute exactly this sequence; each numbered step is a separate node and each is a MUST.

### (1) Ingest (N17 F-VAL) — MUST
Parse + schema-validate input report against Report Schema v1. Halt on schema fail with precise schema error (which field, which finding-id).

### (2) Triage (N18 FixTriage) — MUST — "before touching code"

- Group by file/module to minimize churn and merge conflicts
- Topologically sort by dependency edges
- Split into tiers: **Tier-1 auto-apply** (mechanical/cosmetic — typo, missing import, dead code with no callers), **Tier-2 confirm** (local logic — single-function changes with bounded blast radius), **Tier-3 gated** (cross-cutting/behavioral — schema changes, public-API edits, multi-file refactors)
- Emit a fix-plan document the user approves before execution
- Items the model is uncertain about → defer with rationale, don't attempt

### (3) Plan (N19 FixPlanner) — MUST
Emit fix-plan, await approval. Plan document lists each fix-group with: tier, files touched, blast radius (callers grep), dependency order rationale, the verification command that will run, and the proposed commit message format `[AUDIT-NNN] <one-line>`. Under `--auto`: Tier-1 auto-approved; Tier-2/3 still require approval. Under `--confirm-all`: every fix-group requires approval. Under `--dry-run`: plan is emitted but execution does not proceed past this step.

### (4) Pre-flight (N20 PreFlight) — MUST
Capture baseline manifest BEFORE any fix is applied: full test suite output, type-check output, lint output, build output. Without baseline, the verification battery has nothing to compare against. Create a dedicated branch (e.g., `epiphany-audit/<report-id>-YYYYMMDD`).

### (5) Apply (N21 FixApplier, fan-out) — MUST — Atomic Execution Loop (per fix or fix-group)

For each unit:

1. Apply edit
2. Run targeted tests (file/module level)
3. Type check on changed files
4. If pass → commit with `[AUDIT-NNN] <one-line>` + finding ID in body
5. If fail → rollback, mark finding as failed, capture failure context, continue (don't cascade)

**One concern per commit. No bundling unrelated fixes. No "while I'm here" cleanups.**

Fan-out parallelism: independent fix-groups (no shared files/symbols) may execute in parallel; dependent fix-groups serialize per the topo-sort from N18.

### (6) Per-Fix Verify (N22 PerFixVerifier, fan-out) — MUST
Embedded inside step (5) — targeted tests + type check on changed files. PASS → forward to commit. FAIL → enter atomic loop's rollback branch, then route to E_repair if retries remain.

### (7) Audit Re-Run (N23 AuditReRun) — MUST
After all fix-groups have completed (or been deferred), re-run the audit pipeline (N01→N16) on the fixed code. **Original findings should disappear; new findings should be flagged as fix-induced regressions** and routed through E_rerun_fail back to N18 for re-triage with the regression context attached.

### (8) Verification Battery (N24 RegressionBattery) — MUST — "this is where most fix-skills cut corners"

Required:

- Full test suite vs. baseline (no new failures)
- Type check (no new errors)
- Lint (no new warnings in changed regions)
- Build clean
- Re-run the audit on the fixed code — the original findings should disappear; new findings should be flagged as fix-induced regressions
- Diff scope check — every change in the diff must map to an audit ID; unmapped changes flagged as scope creep

### (9) Regression Prevention — MUST for behavioral fixes
For behavioral fixes (not cosmetic), the prompt should require adding a test that would have caught the original bug, where the language/codebase supports it. Otherwise fixes silently drift back in later. Test added in same commit as the fix (or as a paired follow-up commit if the language requires separation).

### (10) Reporting (N26 FixReporter) — MUST — closing the loop
Final output should include:

- Per-finding status: `applied | verified | deferred | failed | skipped`
- Diff summary grouped by AUDIT-ID
- Baseline-vs-post metrics (tests, types, lint, build)
- Audit re-run delta
- List of deferred items + reasons (so the next pass can pick them up)

### Back-edge E_repair (bounded retries)
N22 (PerFixVerify) failure or N24 (RegressionBattery) failure → routes back to N19 (replan) or N21 (retry the failed fix-group with explicit failure-context hint as new input). Bounded: max 1 replan per fix-group, max 1 retry per fix. After cap-hit: rollback all in-flight changes for that fix-group, mark finding as `failed`, capture failure context for the closing-the-loop report, continue with remaining independent fix-groups.

### Hard rules to bake into the fix pipeline (additional MUST NOT)

- **Never expand scope beyond audit findings.** Spotted unrelated bug → log it as a new finding, don't fix it now.
- **Never bypass safety checks** (`--no-verify`, `force-push`, hook skipping) to make fixes "go through".
- **Never amend prior commits** — always new commits, even on retry.
- **Defer over guess** — if the root cause is unclear, mark the finding `deferred` with a question for the user.
- **Idempotent** — re-running on a partially-applied report skips already-applied findings (detect by AUDIT-ID in commit messages).
- **Fail-loud on partial state** — if the loop dies mid-flight, leave a recovery manifest at `~/docs/epiphany/audit/.recovery/<report-id>.json` listing applied/in-flight/pending findings, branch name, and last-known-good commit. Don't pretend success.

## Tradeoff to call out: autonomy vs oversight

The big one: autonomy vs. oversight. The skill needs a clear policy for when to auto-apply vs. confirm. Default policy:

- **Tier-1 (mechanical/cosmetic):** auto-apply
- **Tier-2 (local logic):** batch-confirm (one prompt for the batch)
- **Tier-3 (cross-cutting/behavioral):** one-by-one confirm

Make the thresholds configurable via flags: `--auto` (auto-apply Tier-1 only), `--confirm-all` (confirm every fix regardless of tier), `--dry-run` (emit plan + diffs, don't apply).

This default tier-policy defers to the per-fix anti-conformity rule above ("per-fix opt-in default") only where they overlap — the per-fix opt-in rule is the floor: even Tier-1 cannot auto-apply if `evidence_confidence < HIGH AND remediation_complexity > trivial`. The tier policy adds the autonomy axis on top of the confidence/complexity floor.

## Anti-conformity: per-fix opt-in default (inverted from flag-level)

Even with no flag, default for any individual fix where `evidence_confidence < HIGH AND remediation_complexity > trivial` is to NOT apply automatically — emit as recommendation requiring per-fix user opt-in. Only HIGH-confidence trivial fixes (typo, missing import, dead code with no callers) auto-apply under default-no-flag mode. This inverts at finding-granularity what the flag system controls at run-granularity.

## Anti-conformity: prior-fix temporal check on --fix mode

On --fix mode, before applying any fix, grep `audit_target` git log for prior commits whose messages reference the same `file:line` or finding-id from any prior epiphany-audit run. If found, emit warning: "this finding has been addressed before — re-applying may revert intentional behavior"; require explicit user override per such fix. This guards against re-running --fix on a stale report silently reverting prior intentional decisions.

## Spawn / token budget

- Spawn budget ≤2 sub-agents by default (sandboxed fix-applier + 1 optional repair). ≤3 under --deep. Inline role-switched nodes have no spawn cost.
- --deep adds at most 2 additional analyzer nodes per dimension; enables 1 ideation expansion node before findings; lifts spawn budget from ≤2 to ≤3; adds explicit token-budget cap (abort with partial report if accumulated context >80k tokens).

## "Production grade" — operationalized definition

(1) deterministic node-activation given same project state; (2) idempotent re-runs (re-audit produces same findings modulo intentional code changes); (3) schema-versioned report output; (4) non-zero exit on validator/quality-gate/regression failure; (5) structured stdout/log emissions usable by downstream tooling.

## PRIORITIZATION rule

Final report ranks findings by (Severity × Confidence × 1/Effort). Top of report is the user's resolve-before-testing punch list.

</constraints>

<output_format>

The deliverable of THIS prompt is a design specification for epiphany-audit, presented in three labeled layers:

## Layer A — SKILL.md outline

The implementer must produce a SKILL.md with at minimum these sections, populated:
1. Frontmatter: name, description (when-to-use centric), version (start at 1.0.0)
2. Invocation contract: flags (--audit, --fix <path>, --verbose, --deep, --auto, --confirm-all, --dry-run) and implied-context resolution
3. Node Registry table (full, per §1 — includes audit nodes N01..N16 and fix nodes N17..N26)
4. Edge Table (full, per §1 — includes E01..E15, E_repair, E_rerun_fail, E_diffscope, E_complete)
5. State-transition table (per §5)
6. Fix-Graph Topology canonical reference table (per §8)
7. Report Schema v1 — audit report schema AND closing-the-loop fix report schema (per Layer C)
8. Fix-application safeguard pipeline (10-step Triage→Plan→Pre-flight→Apply→Verify→Audit-rerun→Regression-battery→Report sequence, per `<constraints>`)
9. Atomic Execution Loop spec (5-step per-fix-group inner loop, per `<constraints>`)
10. Verification Battery spec (per `<constraints>`)
11. Tier policy + autonomy/oversight tradeoff (per `<constraints>`)
12. Hard Rules block (audit anti-patterns + fix-pipeline DO NOTs + fix Hard Rules including idempotence + recovery-manifest contract)
13. Quality-gate definitions (per `<verification>`)

## Layer B — Per-Node Runtime Behavior Contract

For each node N01..N26 (and any extension nodes), specify:
- Inputs (typed)
- Outputs (typed)
- Side effects (none / read-only / git-staged / network / write-fs — analyzer nodes must be `none` or `read-only`; only N21 FixApplier is `git-staged`; only N16 SaveHandler and N20 PreFlight + N25 RollbackHandler may write the recovery manifest)
- Halt conditions
- Token budget hint
- Backtrack/aggregation participation if any
- Fan-out cardinality if applicable (N21, N22)
- Back-edge endpoints if applicable (N22 → N19/N21 via E_repair; N23 → N18 via E_rerun_fail)

## Layer C — Report Output Schema (Report Schema v1)

The schema both `--audit` produces and `--fix` consumes.

**Frontmatter (YAML/JSON-ish):**
```
schema_version: 1
audit_target: <absolute path>
audit_timestamp: <ISO 8601>
tool_version: <epiphany-audit semver>
flags: [audit|fix|verbose|deep]
dimensions_activated: [CORRECTNESS, ARCHITECTURE, ...]
dimensions_skipped:
  - dimension: SECURITY
    reason: "no network/IO/DB/auth surfaces detected"
  - dimension: PERFORMANCE
    reason: "no hot loops detected, no perf-critical heuristic match"
gap_dimensions_offered: [...]   # from B-FIND
gap_dimensions_accepted: [...]
```

**Body — one `## Finding F00N` section per finding with mandatory keys:**
- `id`: F001, F002, ...
- `location`: file:line (or file:line-range)
- `dimensions`: list (e.g., [CORRECTNESS, MAINTAINABILITY])
- `severity`: CRITICAL | HIGH | MEDIUM | LOW | INFO
- `confidence`: HIGH | MEDIUM | LOW (LOW must include "verify by ...")
- `evidence_excerpt`: verbatim ≤8 lines from source
- `rationale`: why this is a defect
- `remediation`: concrete fix block (diff or numbered steps) — never "consider improving X"
- `false_positive_check`: 4 questions answered (see `<verification>`)
- `priority_score`: numeric (Severity × Confidence × 1/Effort)

**Severity tier definitions (verbatim):**

```
CRITICAL = data loss, security breach, crash on common path, corruption
HIGH = crash on edge case, wrong output silently, perf regression > 2x
MEDIUM = degraded UX, recoverable error mishandled, maintainability cliff
LOW = code smell with concrete future cost
INFO = observation, no action required
```

**Top-of-report punch list:** "Resolve-before-testing" — first N findings sorted by `priority_score` descending.

**"Unverified Hypotheses" section:** demoted findings that failed false-positive checks; not in main report ranking but preserved for user review.

**Final output format:** Final output format should be in markdown format and offered to be save in ~/docs/epiphany/audit.

### Fix Report Schema v1 (closing-the-loop output of `--fix` mode, emitted by N26 FixReporter)

The fix report is a separate markdown document with frontmatter and per-finding status entries. Saved to `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-YYYYMMDD-HHMMSS.md`.

**Frontmatter:**
```
schema_version: 1
source_audit_report: <absolute path to the audit report consumed>
fix_run_timestamp: <ISO 8601>
branch: <git branch created by N20 PreFlight>
flags: [auto|confirm-all|dry-run]
baseline_metrics:
  tests: <pass/fail counts>
  type_check: <error counts>
  lint: <warning counts in changed regions>
  build: <pass/fail>
post_metrics:
  tests: <pass/fail counts>
  type_check: <error counts>
  lint: <warning counts in changed regions>
  build: <pass/fail>
audit_rerun_delta:
  resolved: [F001, F003, ...]            # original findings now absent
  fix_induced_regressions: [...]         # new findings appearing only post-fix
  unchanged: [F004, ...]                 # findings that survived (deferred or failed)
diff_scope_check: pass | fail-with-unmapped-hunks
```

**Body — one entry per finding from the source audit report:**

- `id`: F001 (matches source report)
- `status`: `applied` | `verified` | `deferred` | `failed` | `skipped`
- `commit`: `<sha>` (if applied or verified)
- `tier`: 1 | 2 | 3 (from N18 FixTriage)
- `verification_outcome`: `pass` | `fail` | `n/a` (n/a for deferred/skipped)
- `failure_context`: free text — only present for `failed`; captured rollback diagnostic
- `defer_reason`: free text — only present for `deferred`; question for the user
- `regression_test_added`: `yes (file:line) | no | n/a (cosmetic)` — per Regression Prevention rule
- `notes`: optional free text

**Deferred items section:** consolidated list of `deferred` findings with their reasons, surfaced at the top of the body so the next pass can pick them up.

**Recovery manifest reference:** if the run died mid-flight and `~/docs/epiphany/audit/.recovery/<report-id>.json` exists, the fix report MUST cite it explicitly with the absolute path and the last-known-good commit sha.

## Worked Examples (MUST appear in design spec)

The implementer MUST embed in the design spec at least:

**(1) One worked Finding example with all mandatory fields populated.** Example skeleton (the implementer fills with a plausible Python off-by-one):
```
## Finding F001
- id: F001
- location: src/parser.py:142
- dimensions: [CORRECTNESS]
- severity: HIGH
- confidence: HIGH
- evidence_excerpt: |
    for i in range(len(tokens) - 1):
        emit(tokens[i])
    # final token never emitted
- rationale: loop bound drops final token; downstream consumer expects all N tokens
- remediation: |
    -    for i in range(len(tokens) - 1):
    +    for i in range(len(tokens)):
- false_positive_check:
    - intentional? no — no test or comment justifies the -1
    - verified file/symbol exists? yes (Read at src/parser.py:140-145)
    - reachable from real entry? yes (called by parse_input in main.py:23)
    - fix breaks dependents? no — grep shows no caller relies on N-1 emission
- priority_score: 9.0  # severity 3 × confidence 3 × 1/effort 1
```

**(2) One worked Node Registry row example:**
```
N02 | RelevanceRouter | router |
  Inputs: project_model from N01
  Outputs: dimension_activation_map, skip_reasons
  Active in: audit, both
  Aggregation policy: n/a (router, not aggregator)
```

**(3) One worked dimension-routing decision:**
```
PERFORMANCE: skipped — no hot loops detected, no perf-critical heuristic match
SECURITY: activated for [shell-injection, secrets-in-source]; skipped sub-surfaces [SQL] — no DB layer detected
```

</output_format>

<verification>

## False-positive controls (preserve verbatim)

Before reporting any finding, run this check:

- Is the "bug" actually intentional? Search for tests, comments, or commit
  messages that justify it.
- Does the finding rely on a function/flag/file you have not verified exists in
  the current tree?
- Is the failure mode reachable from any real entry point, or only from contrived
  inputs?
- Would the fix you're suggesting break a behavior some other code depends on?
  Grep for callers.

If you cannot answer these, downgrade confidence to LOW and state what needs verifying. Findings that fail these checks are demoted to an "Unverified Hypotheses" section, not the main report.

## Q-GATE (N14) — ReportQualityGate, runs before save

(a) **No-LOW-only report:** if >5 findings and 0 MEDIUM+ severity, demote whole report to advisory and emit warning to user.
(b) **Mandatory-field completeness:** every finding has all 4 mandatory fields (location, severity, confidence, remediation) populated.
(c) **Location verification:** every `<file>:<line>` is verified against the actual file via Read at audit time. No hallucinated lines.
(d) **CRITICAL/HIGH × Confidence floor:** every CRITICAL or HIGH finding must have Confidence ≥ MEDIUM. HIGH-severity at LOW-confidence must either be demoted in severity or upgraded in confidence with stated evidence.
(e) **Duplicate merge:** duplicate patterns are merged with count.
(f) **No-comment-echo:** no finding text quotes the project's own TODO/FIXME without independent verification of the underlying defect.

Halt-on-Q-GATE-failure is mandatory: skill must not save a report that fails Q-GATE.

## Q-GATE-2 (N15) — Hostile-Reviewer self-audit pass

Adversarial test of the skill's own findings:
(a) **Anti-iatrogenic check:** flag findings whose remediation could plausibly introduce a worse defect than the original.
(b) **Evidence-rationale coherence:** flag findings whose evidence excerpt does not actually support the stated rationale.
(c) **Dimension classification correctness:** flag findings whose dimension classification does not match the failure mode described.

Findings that survive Q-GATE-2 proceed to save; findings that fail are demoted to Unverified Hypotheses or rewritten.

## F-VAL (N17) — FixReportValidator, entry point of --fix mode

- Schema-validate input report against Report Schema v1.
- On schema fail: emit precise schema error (which field, which finding-id) and halt.
- On schema pass but suspicious content (zero findings, all-Unverified-Hypotheses, conflicting fixes for same file:line): emit warning and require confirm-before-apply.

## R-VER (N20) — RegressionVerifier, after each applied fix

- Re-runs the project's declared test/build commands, auto-detected (pytest, npm test, ctest, cmake --build) with user override flag.
- On test failure or build break: automatic `git revert HEAD` + halt-with-diagnostic.
- The skill MUST NOT proceed past R-VER failure without explicit user override.

## Self-knowledge escape hatch (mandatory)

If you cannot determine the audit target, the input report's schema-version, the project's test command, or whether a fix's regression check actually exercised the affected code path, STATE WHAT IS UNKNOWN and ask the user — do not infer.

## Verification gates summary

- Q-GATE (N14) gates save (audit-report quality).
- Q-GATE-2 (N15) gates save (adversarial second pass on audit findings).
- F-VAL (N17) gates entry into fix application (Report Schema v1 validation).
- N18 FixTriage gates with tier classification + topo-sort + defer-on-uncertainty.
- N19 FixPlanner gates with **user approval of the fix-plan document** before any code is touched.
- N20 PreFlight gates on baseline-capture success (no baseline → no battery → halt).
- N22 PerFixVerifier gates each fix-group commit (per-fix targeted tests + type check).
- N23 AuditReRun gates closing-the-loop: re-run of audit on fixed code; new findings = fix-induced regressions, routed to E_rerun_fail.
- N24 RegressionBattery gates the run as a whole: full test suite vs baseline, type check, lint, build clean, **diff scope check** (every diff line maps to AUDIT-ID; unmapped = scope creep, halt).
- E_repair back-edge: bounded retries (max 1 replan per fix-group, max 1 retry per fix); after cap-hit, mark `failed` and continue with independent fix-groups.
- Final audit report ranks findings by (Severity × Confidence × 1/Effort).
- Final fix report (N26) ranks per-finding entries by status priority: failed > fix-induced-regression > deferred > applied > verified > skipped (so the user's eye lands on the things needing attention first).

</verification>

<edge_cases>

## Constraint contradiction surfaces (preserved, not silently resolved)

The original requirements contain two constraint pairs that pull in opposite directions; the design must surface and route them, not pick silently:

1. "be sure we are using the system fully to run only needed sections of audit based on an initial analysis of given context for project in question" vs. "fully deep audit in a professional manner, aiming to create a production grade solution". Resolution mechanism: R-ROUTE prunes inapplicable dimensions (with explicit "skipped because" rationale); --deep flag opts back into broader coverage. Both phrasings are honored, neither overrides the other.

2. "attempt resolving using best judgement first" vs. "if there is ever a question of creative control or an issue which can't be resolved, we need to explicitly query the user repeatedly until all issues resolved". Resolution mechanism: best-judgment is permitted only on mechanical / non-creative ambiguity; any creative-control or unresolvable issue triggers iterative user query loop with no auto-resolution.

## Implied-context resolution order (deterministic — no silent guessing)

When skill is invoked without an explicit target argument:
1. Explicit path argument wins.
2. Else if `--fix <report>` given → derive target from report's `audit_target` frontmatter field.
3. Else if cwd is inside a git repo → use `git rev-parse --show-toplevel`.
4. Else cwd.
5. Else halt with: "no audit target — pass a path or run from inside a project".

## Ambiguity halts (do not pick silently)

- cwd contains nested git repos → list candidate roots and ask which.
- session has touched multiple projects → list candidates and ask which (the skill must not silently default to "the most recently mentioned").
- project type ambiguous (e.g., polyglot monorepo, language detection inconclusive) → ask user, do not guess.

## R-ROUTE edge cases

- **Heterogeneous monorepo:** run dimensions per sub-tree; emit per-sub-tree dimensions_activated lists in frontmatter.
- **Binary-only repo (no source files):** halt with "no source detected, nothing to audit".
- **Unknown language:** fall through to language-agnostic dimensions only (architecture, maintainability, security-by-pattern); emit explicit "language unidentified — running language-agnostic subset only".
- **Project type ambiguous:** ask user, do not guess silently.
- **Inapplicable-dimension example (preserve from input):** check for SQL injections in something that is not a product relating to this use case can be eliminated for many cases.

## Fix-mode edge cases

- Input report from a different version of epiphany-audit than current → F-VAL emits version-skew warning and asks before proceeding.
- Input report references files that no longer exist in `audit_target` → that finding skipped with "stale reference" annotation; do not abort whole run.
- Input report references files outside `audit_target` git tree → halt; refuse to write outside source tree (per DO NOT rules in `<constraints>`).
- Two findings in same report propose conflicting edits to the same `file:line` → halt; ask user to choose ordering. (N18 FixTriage detects this during topo-sort.)
- **Recovery manifest detected at run start** (`~/docs/epiphany/audit/.recovery/<report-id>.json` exists from a prior interrupted run) → emit "prior run interrupted at commit <sha>; pending findings: [list]; resume from there or start fresh? (resume / fresh / abort)". Do NOT silently resume; do NOT silently overwrite the manifest.
- **Idempotency on re-run** (per Hard Rules) → at run start, grep `audit_target` git log for `[AUDIT-NNN]` commit-message tags matching findings in the input report; mark those findings as `skipped (already-applied at <sha>)` and exclude from the fix plan.
- **Scope-creep detected at N24** (diff contains lines not mapped to any AUDIT-ID) → halt with "scope creep detected" + per-hunk listing; do NOT auto-revert (the user may have made deliberate manual edits during the run); ask the user to either authorize the unmapped diff (it gets recorded as a manual edit in the fix report) or revert it manually.
- **Fix-induced regression at N23** (audit re-run surfaces a new finding absent from the original report) → route the affected fix-group through E_rerun_fail back to N18 for re-triage with the regression context attached; if the retry cap is already exhausted for that group, mark the originating finding as `failed (induced-regression: <new-finding-id>)` and the new finding as `deferred (caused-by: <originating-finding-id>)` in the fix report.
- **Tier override** — if the user disagrees with N18's tier classification (e.g., a finding routed to Tier-1 that the user feels is risky), the user may flag it `--escalate-finding F00N` to force Tier-3 (one-by-one confirm) for that specific finding. Conversely `--demote-finding F00N` is NOT supported (we never relax tier safety on user instruction; the user must edit the report).
- **Test command unknown** — if N20 PreFlight cannot auto-detect a project test command (no `pytest`/`npm test`/`ctest`/`Makefile` target/etc.), halt at PreFlight with "no test command detected — pass `--test-cmd '<cmd>'` or run from a project with a recognized test runner". Do NOT proceed with a no-op verification battery.
- **Behavioral fix without test framework** — if Regression Prevention requires adding a test for a behavioral fix but the project has no recognized test framework, mark the finding `applied` but with `regression_test_added: no (no test framework detected — manual test recommended)` in the fix report rather than blocking on it.

## Verbose vs. bloat

`--verbose` adds depth where it improves actionability (additional examples, expanded rationale, expanded remediation tradeoffs) — never add length without per-line utility. The Q-GATE check (a) (no-LOW-only report) applies equally under verbose; verbose does not justify nitpick padding.

## Deep-mode bound

`--deep` lifts coverage and spawn budget but is bounded by the explicit token cap (>80k accumulated context → abort with partial report rather than silently truncating). The design must specify graceful partial-report emission on cap-trigger.

</edge_cases>
</prompt>
