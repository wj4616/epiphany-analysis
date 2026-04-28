# epiphany-audit — Skill Design Specification

- **Status:** design — awaiting implementation
- **Date:** 2026-04-27
- **Author:** brainstorming session (critical-mode) on top of `~/docs/epiphany/prompts/27-04-epiphany-audit-skill-design.md`
- **Family:** epiphany-*
- **Implementer audience:** Claude Code agent (general-purpose / prompt-architect subagent type) producing `SKILL.md`, `graph.json`, `modules/N01..N27.md`, `schemas/*.json`, `templates/*`, and `tests/`.
- **Hard gate:** the consumer of this spec produces the skill files; the consumer does NOT execute audits as part of consuming this spec.

---

## 1. Purpose, Invocation Contract, File Layout

### 1.1 Purpose

`epiphany-audit` is a graph-of-thought skill that runs a multidimensional, project-aware audit on a target codebase, then optionally drives a safeguarded fix pipeline. It is tailored: irrelevant dimensions are pruned by R-ROUTE; project-specific blindspots are surfaced by B-FIND; every finding cites verified `file:line` evidence; fixes never apply without explicit per-tier user consent under the default policy.

The skill, when invoked, must:

- audit for regression areas
- audit for bugs
- audit for potential issues
- audit for errors
- audit for any problems, looking several steps ahead (latent findings tagged with reachability conditions)
- audit ensuring all components have integrated flush in the solution during development

The skill must be tailored to work with any project optimally — if a dimension does not apply, R-ROUTE prunes it with explicit `skipped because: <trigger>` rationale. Inapplicable items are not run.

### 1.2 Skill location and family

- **Location:** `~/.claude/skills/epiphany-audit/`
- **Family:** `epiphany-*` (alongside `epiphany-prompt`, `epiphany-cognitive`, `epiphany-analysis`, `epiphany-graph-genius`)
- **MAY invoke:** `kb-route` (optional KB consultation), `prompt-graph` (optional remediation-message enhancement; never required)
- **MUST NOT invoke:** `writing-plans`, `executing-plans`, `find-skills`
- **Write-tool footprint:** report files under `~/docs/epiphany/audit/`; fix-application diffs against `audit_target` only.

### 1.3 Invocation contract

```
/epiphany-audit [<path>] [--audit | --fix <report>] [--verbose] [--deep] [--improve]
                [--auto | --confirm-all | --dry-run]
                [--escalate-finding F00N] [--test-cmd '<cmd>'] [--monorepo-subtree-limit N]
```

**Mode flags (mutually exclusive):**

- `--audit` — produce report only; **do not** offer fix pipeline.
- `--fix <report>` — consume an existing audit report; run fix pipeline only.
- (no flag) — run audit, save (offered), then offer fix pipeline (per-tier confirmation default).
- Passing both `--audit` and `--fix` → `halt-on-flag-conflict`.

**Verbosity / depth:**

- `--verbose` — adds depth where it improves actionability (additional examples, expanded rationale, expanded remediation tradeoffs). Never adds nitpick padding. Q-GATE no-LOW-only check applies equally.
- `--deep` — additional dimension passes; subagent fan-out for analyzers (capped); lifts spawn budget to ≤3; enables interactive B-FIND prompt; enforces 80k-token checkpoint cap.

**Autonomy / oversight (mutually exclusive):**

- `--auto` — opt-in: silent Tier-1 application; Tier-2 still batch-confirmed; Tier-3 still per-fix.
- `--confirm-all` — every fix-group requires confirmation regardless of tier.
- `--dry-run` — emit fix-plan + diffs only; no application, no commits, no branch.
- Passing more than one → `halt-on-flag-conflict`.
- **Precedence on contradiction (defensive):** `--dry-run` > `--confirm-all` > `--auto` > default. Conservative-on-contradiction.

**Other flags:**

- `--escalate-finding F00N` — force a specific finding to Tier-3 (per-fix confirm) regardless of N16 classification. Valid with `--fix` or no-flag mode (where fix pipeline may run). Ignored with warning under `--audit` (no fix pipeline is offered). Overrides `--auto` for the named finding. **Invalid-id handling:** if `F00N` is not present in the resolved audit report, halt with `halt-on-invalid-finding-id` at N16 ingest (after F-VAL schema validation passes but before triage begins) — diagnostic lists the available finding-ids in the report.
- `--demote-finding F00N` — **NOT supported.** Passing it → `halt-on-flag-rejection`: *"--demote-finding is not supported. Edit the report manually to change tier classification."*
- `--test-cmd '<cmd>'` — explicit project test command. Always overrides auto-detection.
- `--full-rerun` / `--no-rerun` — override the §2.5 tiered audit-rerun policy. `--full-rerun` forces full N01..N13 rerun (high-stakes runs); `--no-rerun` skips the audit-rerun sub-step entirely (token-budget-constrained runs; fix-report records `audit_rerun_delta: skipped-by-flag`). Mutually exclusive; passing both → `halt-on-flag-conflict`. Both valid only with `--fix` or no-flag (fix-mode); ignored with warning under `--audit`.
- `--improve` — after the audit pipeline completes (post-Q-GATE, post-save), run the Improvement Analysis subpipeline (N24..N27). Analyzes the project for tangible improvement opportunities beyond the audit's defect findings. Each improvement candidate is scored for utility (1-3) vs. implementation cost (1-3); speculative suggestions and improvements where cost > utility are discarded by the Over-Engineering Filter (N26). Survivors are categorized as `quick-win`, `worthwhile`, or `notable`. Produces a separate Improvement Report at `~/docs/epiphany/audit/improvement-reports/`. **Only valid in audit mode or no-flag mode.** Emits a warning (not a halt) if combined with `--fix`; improvement analysis is then silently skipped. `--improve` is orthogonal to `--verbose`, `--deep`, `--dry-run`, and autonomy flags.
- `--monorepo-subtree-limit N` — override the default cap of 10 on distinct project-shaped subtrees detected in heterogeneous monorepos. Projects beyond the cap are grouped into a synthetic `rest-of-repo` subtree with the union of detected dimension triggers; `subtree_grouping_applied: true` is emitted in the audit report frontmatter. Default: 10. Valid in all modes.

**`<report>` resolution order (for `--fix`):**

1. Absolute path (resolves directly).
2. Path relative to cwd.
3. Bare filename or partial slug:
    a. Search `~/docs/epiphany/audit/` (top-level audit reports) first.
    b. Then search `~/docs/epiphany/audit/fix-reports/` (a fix report can be re-resolved into its `source_audit_report` for re-runs).
    c. If `<bare>` has no extension, append `.md` and retry both locations.
    d. If multiple matches, halt with `halt-on-ambiguous-fix-report` listing matches.
4. Halt with `halt-on-unresolvable-fix-report` if none of the above resolves to an existing file.

**Project-slug derivation** (used in audit-report filenames `<project-slug>-<YYYYMMDD>-<HHMMSS>.md`): basename of the resolved `audit_target`, lowercased, non-alphanumeric characters replaced with `-`, runs of `-` collapsed, leading/trailing `-` stripped, truncated to 50 characters.

### 1.4 Implied-context resolution

Deterministic, halt-on-ambiguity, no silent guessing:

1. Explicit path argument wins (when `--fix` is also given, see step 2 conflict rule).
2. `--fix <report>` derives target from the report's `audit_target` frontmatter field (after `<report>` itself is resolved per §1.3).
    - **Path-vs-report-target conflict:** if both an explicit `<path>` argument AND `--fix <report>` are given, and `<path>` does not resolve to the same canonical absolute path as the report's `audit_target` field, halt with `halt-on-target-conflict` and emit *"explicit path `<path>` does not match the `audit_target` field of `<report>`. Pass only one, or ensure they agree."* Two paths are "the same" iff `realpath(<path>) == realpath(audit_target)`.
3. `cwd` inside a git repo → `git rev-parse --show-toplevel`.
4. Else `cwd` itself.
5. Else `halt-pre-audit`: *"no audit target — pass a path or run from inside a project."*

**Suspicious-target gate** runs on the resolved target *regardless of resolution path* (cwd, git toplevel, or explicit path). Halt with `halt-suspicious-target` (hard halt) if any of:

- resolved root equals `$HOME`, `/`, `/etc`, `/usr`, `/var`, `/tmp`
- resolved root has >5 top-level subdirs that each independently look like a project (each containing a `.git` dir, a build manifest, or a recognized framework marker)

Warn-and-prompt (soft halt — user may override) for:

- resolved root inside `~/.claude/skills/<x>/` (legitimate skill-development case)
- resolved root inside `~/dotfiles`, `~/.config`, `~/Desktop`, `~/Downloads`
- implementer SHOULD allow further user override via per-user config file (e.g., `~/.config/epiphany-audit/allowed-roots.json`)

Diagnostic: *"resolved target $TARGET looks like a wrapper/aggregator, not a project. Pass an explicit path or cd into the project."*

**Other ambiguity halts:**

- cwd contains nested git repos → list candidate roots, ask user.
- session has touched multiple projects → list candidates, ask user.
- project type ambiguous (polyglot monorepo, language detection inconclusive) → ask user.

### 1.5 File layout

`epiphany-graph-genius` style: declarative graph + per-node modules + schemas + templates.

```
~/.claude/skills/epiphany-audit/
├── SKILL.md
├── graph.json                           # declarative N + E registry, machine-readable
├── graph.schema.json
├── README.md
├── CHANGELOG.md
├── modules/
│   ├── N01-context-intake.md
│   ├── N02-r-route.md
│   ├── N03-b-find.md
│   ├── N04-correctness.md
│   ├── N05-architecture.md
│   ├── N06-performance.md
│   ├── N07-security.md
│   ├── N08-maintainability.md
│   ├── N09-extensible.md                # template; instances N09.<dimension>
│   ├── N10-fpv.md
│   ├── N11-aggregator.md
│   ├── N12-prioritizer.md
│   ├── N13-formatter.md
│   ├── N14-q-gate.md                    # mechanical Pass A + adversarial Pass B
│   ├── N15-save-handler.md
│   ├── N16-fix-triage.md                # F-VAL ingest + Triage (folded)
│   ├── N17-fix-planner.md
│   ├── N18-pre-flight.md
│   ├── N19-fix-applier.md
│   ├── N20-per-fix-verifier.md
│   ├── N21-regression-battery.md        # battery + audit-rerun delta (folded)
│   ├── N22-rollback-handler.md
│   ├── N23-fix-reporter.md
│   ├── N24-improvement-contextualizer.md  # --improve subpipeline
│   ├── N25-improvement-brainstormer.md
│   ├── N26-oef.md                         # over-engineering filter
│   └── N27-improvement-reporter.md
├── schemas/
│   ├── audit-report-v1.schema.json
│   ├── fix-report-v1.schema.json
│   ├── dry-run-plan-v1.schema.json      # I3 — first-class dry-run plan
│   ├── dimension-plugin-v1.schema.json  # I1 — pluggable dimension manifest
│   └── improvement-report-v1.schema.json
├── dimensions/                          # I1 — built-in + user dimension plugins
│   ├── correctness.md                  # built-in (mirrors N04)
│   ├── architecture.md                 # built-in (mirrors N05)
│   ├── performance.md                  # built-in (mirrors N06)
│   ├── security.md                     # built-in (mirrors N07)
│   ├── maintainability.md              # built-in (mirrors N08)
│   └── README.md                       # how to add a custom dimension
├── templates/
│   ├── audit-report.md.template
│   ├── fix-report.md.template
│   ├── dry-run-plan.md.template         # I3
│   └── improvement-report.md.template
└── tests/
    ├── smoke/
    ├── schema-validation/
    └── determinism/                     # B30 — fixture root for ≥80% set-overlap CI gate
        ├── python-small/
        │   ├── source/                  # reference project tree
        │   └── expected_findings.yaml   # frozen finding set
```

**External paths used at runtime:**

- `~/docs/epiphany/audit/` — saved audit reports
- `~/docs/epiphany/audit/fix-reports/` — saved fix reports
- `~/docs/epiphany/audit/dry-run-plans/` — saved dry-run plans (I3)
- `~/docs/epiphany/audit/improvement-reports/` — saved improvement reports (`--improve`)
- `~/docs/epiphany/audit/.state/<source-report-id>.json` — idempotency state per source report
- `~/docs/epiphany/audit/.baselines/<report-id>.json` — pre-flight baseline manifest (N18)
- `~/docs/epiphany/audit/.recovery/<report-id>.json` — recovery manifest (in-flight fix state)
- `~/docs/epiphany/audit/.recovery/.archive/<report-id>-<state>-<ISO-timestamp>.json` — archived prior recovery manifests; `<state>` ∈ {`completed` (planned-termination archive via E_finalize), `superseded` (user chose `fresh` over a prior interrupted run), `aborted` (reserved for future use)}
- `~/docs/epiphany/audit/.logs/<report-id>.jsonl` — structured per-node event log (not stdout)
- `~/.config/epiphany-audit/dimensions/` — user-installed dimension plugins (loaded after skill-bundled ones)
- `~/.config/epiphany-audit/allowed-roots.json` — per-user override list for suspicious-target gate

---

## 2. Node Registry (27 nodes: 23 core + 4 improvement subpipeline)

Compressions (vs. input prompt's 26-node registry; this spec's renumbering closes the gaps):

| Input prompt nodes (folded) | This spec's node | Rationale |
|---|---|---|
| input N14 (ReportQualityGate) + input N15 (HostileReviewer) | **this spec's N14 — Q-GATE** | Single node, two passes: mechanical inline Pass A + adversarial subagent Pass B |
| input N17 (FixReportValidator / F-VAL) + input N18 (FixTriage) | **this spec's N16 — FixTriage** | F-VAL schema validation is the ingest sub-step of Triage, not a separate node |
| input N23 (AuditReRun) + input N24 (RegressionBattery) | **this spec's N21 — RegressionBattery** | Audit-rerun delta is one *check* within the verification battery |

Net effect: 26 → 23 nodes. All other input-prompt nodes are renumbered downward to fill the gaps (e.g., input N16 SaveHandler → this spec's N15; input N19 FixPlanner → this spec's N17; input N25 RollbackHandler → this spec's N22; input N26 FixReporter → this spec's N23). **All node-ID references elsewhere in this spec use the renumbered IDs.**

| ID | Name | Type | Mode | Purpose | Backtrack / Aggregation |
|---|---|---|---|---|---|
| N01 | ContextIntake | ingest | inline | Fresh project model: language detection, file inventory, build manifest, test command, git state, declared entry points. Honors implied-context resolution + suspicious-target gate. Forbids reference to prior session conclusions. | — |
| N02 | RelevanceRouter (R-ROUTE) | router | inline | Per-dimension activation map. Loads dimension plugins from (1) skill-bundled `dimensions/*.md`, (2) user `~/.config/epiphany-audit/dimensions/*.md`; user dimensions are loaded *after* and may shadow bundled ones by `name`. Inspects each plugin's `activation_triggers` per §2.4. **Floor:** CORRECTNESS + MAINTAINABILITY always on (regardless of plugin presence). Skipped dimensions emit `skipped because: <trigger>` rationale. Heterogeneous monorepo emits per-subtree activation maps. | — |
| N03 | BlindspotFinder (B-FIND) | meta-analyzer | inline | Project-type-specific gap dimensions. **Default mode:** auto-add HIGH-confidence gaps; list in `gap_dimensions_auto_added`. **`--deep` mode:** prompt user (`include gap dimension <name>? (y/n/skip-all)`). | — |
| N04 | DimensionAnalyzer.CORRECTNESS | analyzer | inline / subagent under `--deep` (capped) | Logic, types/lifetime, boundaries, races, leaks, error paths. | participant in BACKTRACK via N10 |
| N05 | DimensionAnalyzer.ARCHITECTURE | analyzer | inline / subagent under `--deep` | Coupling, circular deps, god objects, duplicated logic, invariant gaps. Tags `latent` findings with reachability. | participant in BACKTRACK |
| N06 | DimensionAnalyzer.PERFORMANCE | analyzer | inline / subagent under `--deep` | Hot allocations, complexity blowups, cache layout, false sharing. Tags `latent`. | participant in BACKTRACK |
| N07 | DimensionAnalyzer.SECURITY | analyzer | inline / subagent under `--deep` | Per-surface sub-routing (SQL/shell/auth/secrets/deserialization/prompt-injection). | participant in BACKTRACK |
| N08 | DimensionAnalyzer.MAINTAINABILITY | analyzer | inline / subagent under `--deep` | Dead code, misleading names, stale TODOs, test coverage gaps on failure-mode branches. | participant in BACKTRACK |
| N09 | DimensionAnalyzer.\<X\> (plugin-instantiated) | analyzer | inline / subagent under `--deep` | **Instantiated by R-ROUTE from dimension plugins** (see §2.4) — built-in (a11y, reproducibility, IaC drift, model-card, schema-evolution, prompt-injection live as plugin files in `dimensions/`) and user-installed plugins from `~/.config/epiphany-audit/dimensions/`. Concrete instances get IDs `N09.a11y`, `N09.iac-drift`, etc., recorded in `graph.json` at run time. Plugin manifest declares `activation_triggers`, `prompt_template`, and optional `kb_route_query` for KB consultation. | participant in BACKTRACK |
| N10 | FalsePositiveVerifier (FPV) | verifier | inline | Re-reads source per finding; runs the 4 false-positive questions; demotes/discards. **Owns audit-side BACKTRACKING (single re-emit cap).** **Populates the shared location-verification cache** (per §7 cache contract): for each finding's `location: <file:line>`, after Read, writes `{file, line_range, verified: true|false, last_read_at}` to an in-memory cache. N14 Pass A consumes this cache instead of re-Reading; identical `(file, line_range)` keys hit the cache. | refinement back-edge to N04..N09 |
| N11 | FindingsAggregator | aggregator | inline | Dedup by pattern+location, merge cross-dimension overlaps, count-collapse. **Owns audit-side AGGREGATION.** | aggregator |
| N12 | Prioritizer | scorer | inline | (Severity × Confidence) / Effort. Emits resolve-before-testing punch list. | — |
| N13 | ReportFormatter | formatter | inline | Markdown per Audit Report Schema v1. | — |
| N14 | Q-GATE | verifier | inline (Pass A) + conditional subagent (Pass B) | **Folded from input-prompt N14+N15.** *Pass A* (mechanical, inline): mandatory-field check, location verification (consumes shared location-cache populated by N10 FPV — see §7 cache contract), CRITICAL/HIGH × Confidence ≥ MEDIUM floor, dup merge, no-comment-echo, no-LOW-only warning (user-facing warning — not a halt — when every main-body finding is LOW/INFO severity). *Pass B* (adversarial, subagent — clean lens): anti-iatrogenic, evidence-rationale coherence, dimension-classification correctness. **Pass B activation policy (cost-aware):** runs when ANY of: (a) report has ≥5 findings, (b) any finding has severity CRITICAL or HIGH, (c) `--deep` flag set. Otherwise Pass B is skipped (`q_gate.pass_b: skipped-low-volume`) — Pass A's mechanical checks are sufficient for small low-stakes reports. | adversarial self-review |
| N15 | SaveHandler | io | inline | Offers save under `~/docs/epiphany/audit/`. **Save prompt explicitly warns about idempotency degradation** if user declines: *"declining to save means future `--fix` runs of this report cannot use state-file idempotency; they fall back to git-log only. Save anyway? (y/n)"*. Writes idempotency state file at `~/docs/epiphany/audit/.state/<report-id>.json` only on save-accept. | — |
| N16 | FixTriage (F-VAL ingest + Triage) | validator + triage | inline | **Folded from input-prompt N17+N18.** Schema-validates input report (`schemas/audit-report-v1.schema.json`); halt with precise field/finding-id error on schema fail; SHA-256 of source audit report captured for `source_audit_report_sha256`. **Empty-or-unfixable check (early halt):** if source audit report has zero main-body findings (excluding "Unverified Hypotheses"), halt with `halt-on-empty-or-unfixable-report` *before* running idempotency / triage / suspicious-content checks. **Idempotency check at ingest time** (state file authoritative; git-log fallback). Suspicious-content prompt overrides `--auto`. Groups by file/module; topo-sorts; tier 1/2/3 classification (rules per §2.3); defer-on-uncertainty; conflicting-edit detection: in live mode → `halt-on-conflicting-fixes`; under `--dry-run` → record in `triage_summary.conflicting_groups` and continue (the plan serves as the conflict report; no halt). **Owns fix-side CONDITIONAL ROUTING.** | aggregator (file-grouping) |
| N17 | FixPlanner | planner | inline | Emits fix-plan doc per Dry-Run Plan Schema v1 (§4.4); awaits per-tier batch approval (default policy); Tier-1 silent under `--auto`; per-fix under `--confirm-all`. **`--dry-run` halts here after writing the plan to `~/docs/epiphany/audit/dry-run-plans/<source-report-id>-dryrun-<YYYYMMDD>-<HHMMSS>.md`** (machine-parseable, same downstream tools as fix reports). Each tier presented in order T1 → T2 → T3; decline on Tier-N → all Tier-N findings `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit user `halt` → stop entirely. | — |
| N18 | PreFlight | preflight | inline | **Step ordering (mandatory, in this order):** (0) orphan-branch sweep, (1) git-state check, (2) baseline capture, (3) branch creation. Halt at any step does not progress to the next. **(0) orphan-branch sweep (fresh runs only):** scan for branches matching `epiphany-audit/<source-report-id>-*` with no live recovery manifest at `~/docs/epiphany/audit/.recovery/<source-report-id>.json` AND no archive at `.recovery/.archive/<source-report-id>-*.json`. If any orphan branch found, prompt the user: *"orphan audit branch `<name>` found from a prior aborted run with no recoverable state. Delete? (y/n/keep-as-is)."* On `y`, delete the orphan branch before proceeding; on `n`/`keep-as-is`, proceed (the collision policy in step 3 will append `-<HHMMSS>`). **(1) git-state check:** halt with `halt-on-git-state-incompatible` on dirty tree, detached HEAD, or no commits. **On a resumed run** (entered via `resume` from `halt-on-recovery-conflict`, after the resume's pre-flight `git checkout -- . && git clean -fd` ran), accept the post-cleanup tree as clean; do NOT halt on residual state from the prior interrupted run. **(2) baseline capture:** tests/types/lint/build **before any fix**, written to `~/docs/epiphany/audit/.baselines/<report-id>.json`. `halt-on-baseline-failure` triggers ONLY when the runner crashes / cannot produce any output (no exit code, missing binary, broken tool config) — pre-existing failing tests are recorded as the project's baseline state and do NOT halt. Halt on unknown test command (`halt-on-test-cmd-unknown`). **On a resumed run:** baseline capture is **skipped** — read existing baseline from `~/docs/epiphany/audit/.baselines/<report-id>.json` instead. If the baseline file is missing on resume, halt with `halt-on-baseline-failure` (subreason: `resume-baseline-missing`). **(3) branch creation:** branch name resolution: primary `epiphany-audit/<source-report-id>-YYYYMMDD`. **Collision policy:** if the primary name already exists in the repo, check whether a recovery manifest exists for `<source-report-id>` — if yes, the user reached this point via `resume` and the existing branch IS the resume target (re-use, no creation); if no manifest exists (the prior run completed cleanly and was archived per N22), append `-<HHMMSS>` for the fresh run. If even the timestamped name collides, halt with `halt-on-git-state-incompatible` (subreason: `branch-name-exhausted`). **Does NOT write recovery manifest** (that is N19/N22 responsibility). | — |
| N19 | FixApplier | actuator | inline (write-serial) | Atomic loop per fix-group (definition in §2.3): **apply edit (working tree only) → invoke N20 → commit `[AUDIT-NNN] <one-line>` (finding-id in commit body) on PASS, or `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` on FAIL (no commit ever made for failed attempts).** Critical: **never `git revert HEAD`** — that creates redundant commits that break idempotency grep. **One concern per commit. No bundling. No "while I'm here" cleanups.** **Recovery manifest write policy (cost-aware):** writes only at fix-group **boundaries** — (a) start of fix-group (manifest: `in_flight_finding_id` set, `pending` updated), (b) end of fix-group with success (manifest: `last_known_good_sha` updated, finding moved to `applied`), (c) end of fix-group with failure/cap-hit (manifest: failure recorded). The intra-loop applying/verifying/committing transitions are **NOT** persisted; if a process death occurs between fix-group boundaries, the resume-handler treats the in-flight fix-group as `failed-mid-flight` and restarts that fix-group from scratch. **Regression-prevention test policy: same-commit by default.** Paired follow-up commit allowed only when the language tooling rejects bundled test+source commits OR a project-level pre-commit hook rejects the bundled commit (recorded as `regression_test_added.deferred_to_followup_commit: <sha>` in fix report). Test failure during per-fix verify discards the working-tree changes (no commit). | — |
| N20 | PerFixVerifier | verifier | inline | Per-fix targeted tests + type check on changed files. PASS → commit (in N19). FAIL → emit fail-signal + `failure_class` enum (`verification-failure` \| `commit-hook-failure` \| `git-operation-failure` \| `type-check-failure` \| `targeted-test-failure`); N19 discards working-tree changes; **E_repair edge-layer logic** (not N20) decides retry vs replan vs cap-hit per the rules in §3.1. | emits fail-signal; routing is E_repair's responsibility |
| N21 | RegressionBattery (battery + tiered audit-rerun delta) | verifier | inline (battery) + conditional subagent under `--deep` (audit-rerun clean lens) | **Folded from input-prompt N23+N24.** Battery (always inline): full test suite vs baseline (no new failures), type check (no new errors), lint (`new_warnings_in_changed_regions == 0`), build clean, **diff-scope check** (every diff line maps to AUDIT-ID; regression-prevention test additions count as in-scope via the `[AUDIT-NNN]` commit they ride). **Audit-rerun sub-step is TIERED by the highest tier of fixes applied in this run** (per §2.5): Tier-1-only run → audit-rerun **skipped** (battery is sufficient); Tier-2-only run → **narrow audit-rerun** (re-run only the dimension analyzers matching the dimension tags of the applied fixes — e.g., all-CORRECTNESS fixes → re-run only N04); any Tier-3 fix → **full audit-rerun** (N01..N13). Classify new findings — those in **files touched by fixes** → `induced-regression` (route to E_rerun_fail); those in **untouched files** → `new-finding-discovered` (record in fix report body, no E_rerun_fail). | adversarial-via-rerun |
| N22 | RollbackHandler | recovery | inline | **No `git revert HEAD`** (the atomic loop in §5.3 uses `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` to discard pre-commit working-tree state — there is nothing to revert because failed attempts never reach `git commit`). N22's responsibilities: (a) on E_repair cap-hit for a fix-group, finalize the recovery manifest with the failure record (boundary-aligned write per O3) — manifest stays live at `~/docs/epiphany/audit/.recovery/<report-id>.json` so the user can `resume` on next run; (b) on **planned termination** of the run (reached via E_finalize after N23 has written the fix report file to disk), read the just-written fix report to extract `fix_report_id`, finalize the manifest with run-complete state including `fix_report_id`, then **archive** the manifest to `~/docs/epiphany/audit/.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json` and remove the live `<report-id>.json` so subsequent `--fix` runs do NOT trip `halt-on-recovery-conflict`; (c) emit halt-with-diagnostic when a cap-hit blocks all remaining work. **Mid-flight death case:** N22 cannot finalize after process death; the at-rest accuracy is provided by the most recent fix-group boundary write from N19, which is sufficient for the resume-handler to restart the in-flight fix-group from scratch. | — |
| N23 | FixReporter | formatter | inline | Closing-the-loop fix report per Fix Report Schema v1: per-finding status, diff summary by AUDIT-ID, baseline-vs-post metrics, audit-rerun delta, deferred items, manual-edits section (if user authorized scope-creep), recovery-manifest reference (if applicable). Status-priority sort: `failed > induced-regression > deferred > simulated > verified > skipped`. **Two invocation modes:** (1) **planned termination** — full report, `partial: false`, reached via E13 chain end; (2) **halt-mid-fix-\*** — partial report, `partial: true`, `halt_state: <id>`, reached via E_halt_partial (see §3.1). On halt-mid-fix-\*, N23 emits the partial fix report FIRST, then the halt envelope is emitted to the user; the recovery manifest stays live for `resume`. | aggregator (per-fix outcomes) |
| N24 | ImprovementContextualizer (IC) | analyzer | inline | **`--improve` only.** Ingests `project_model` from N01 and the Q-GATE-validated findings report (formatted by N13, validated by N14). Synthesizes an improvement-analysis frame: what the project is and does; what it's optimizing for; what the audit findings reveal about structural weaknesses; which audit dimensions were NOT flagged (healthy areas worth preserving). Output: `improvement_context` (project capability map, health summary, improvement search constraints). Reads no new files — works only from N01's project model and N14's Q-GATE-passed report already in context. Side effects: `read-only`. | — |
| N25 | ImprovementBrainstormer (IB) | analyzer | inline (phase 1) / inline + 1 subagent under `--deep` (phase 2, only when ≥8 raw candidates from phase 1) | **`--improve` only.** GoT-style brainstorm of concrete improvement candidates. **Two-phase execution:** **Phase 1 (always inline):** generate raw candidate list — for each: a one-line tag and a category seed (developer experience, testing strategy, architectural clarity, performance headroom, tooling/automation, dependency hygiene, documentation/discoverability). No full description/action/success_measure yet — phase 1 output is lightweight by design. **Phase 2:** if `--deep` is set AND phase 1 produced ≥8 raw candidates, spawn one subagent to deeply elaborate each candidate (full description, action, success_measure). Otherwise (no `--deep`, or fewer than 8 candidates), elaborate inline. The 0-or-1 spawn budget applies to phase 2 only. **Strict exclusions:** cosmetic-only changes; re-architectures without concrete measured benefit; generic "best practices" not grounded in project-specific evidence; improvements already implied by audit findings (those are findings, not improvements — no double-counting). Output: `improvement_candidates` list with full elaboration. | participant in AGGREGATION via N26 |
| N26 | OverEngineeringFilter (OEF) | filter | inline | **`--improve` only.** Anti-over-engineering gate. Assigns each candidate `utility_score` (1=marginal, 2=notable, 3=high) and `cost_score` (1=trivial, 2=modest, 3=significant). **Filter rule:** discard when (a) `cost > utility`, OR (b) `utility = 1` regardless of cost (marginal-utility improvements are not worth surfacing), OR (c) the candidate's `description` or `action` prose contains speculative language ("might", "could", "potentially", "may help") indicating unverified utility. After filtering, all survivors have `utility ≥ 2`. **Categories (mutually exclusive; `notable` takes precedence):** `notable` (utility = 3, cost ≤ 2); `quick-win` (utility ≥ 2, cost = 1, and not `notable`); `worthwhile` (all remaining survivors: utility ≥ cost ≥ 2). Discarded candidates + rationale recorded in `discarded_improvements`; surfaced only under `--verbose`. If zero survivors, N27 emits a report stating no improvements above the utility/cost threshold were found — does NOT generate improvements just to have output. | aggregator |
| N27 | ImprovementReporter (IR) | formatter | inline | **`--improve` only.** Formats the improvement report per Improvement Report Schema v1 (§4.5). **Written unconditionally** (no save prompt) to `~/docs/epiphany/audit/improvement-reports/<project-slug>-<YYYYMMDD>-<HHMMSS>-improve.md`. Sections: (1) summary (total candidates, filtered, survivors by category); (2) notable improvements; (3) quick wins; (4) worthwhile improvements. Each entry: description, utility/cost scores, specific action, how to measure success. Under `--verbose`: appends filtered improvements section with discard rationale. Patches `improvement_report_ref` in the audit report frontmatter only when the audit report was saved (N15 save-accepted); if the backpatch fails, logs the improvement report's absolute path to the event log and emits a user-facing warning — does NOT retry or halt. Pipeline failure in N24..N26 → N27 emits partial report with `improvement_partial: true`; **does NOT halt the skill** (improvement analysis failing does not invalidate the audit findings). | — |

### 2.1 Spawn budget reconciliation

- **Default invocation:** **0 or 1 spawn.** N14 Pass B spawns ONLY if (≥5 findings) OR (any CRITICAL/HIGH severity present). Small low-stakes audits (e.g., 3 findings, all MEDIUM/LOW) run with zero spawns. Inside spec's ≤2 cap.
- **`--deep` invocation:** up to **3 spawns** — N14 Pass B (always, under `--deep`) + optional **analyzer fan-out spawn** + N21 audit-rerun clean-lens (only if Tier-3 fixes applied, per O2).
- **`--improve` invocation:** adds **0 or 1 spawn** for N25 ImprovementBrainstormer under `--deep` only, and only when ≥8 raw improvement candidates are detected. Default `--improve` without `--deep` runs N24..N27 entirely inline.
- **`--deep --improve` invocation:** up to **4 spawns** — the 3 from `--deep` above plus the optional N25 fan-out spawn.
- **Analyzer fan-out spawn (`--deep` only):** when more than 3 dimension analyzers (N04..N09) are activated AND the project is large (>500 files OR >50k lines), one batched subagent runs the over-cap analyzers in parallel with the inline ones. The subagent receives the project model from N01 and returns a fan-in result to N10. This is a single spawn slot regardless of how many analyzers it covers. Skipped if ≤3 analyzers or project is small.

### 2.2 Mechanism ownership

- BACKTRACKING (audit): N10 (single re-emit cap on N04..N09)
- BACKTRACKING (fix): E_repair from N20/N21 → N17/N19 (max 1 replan per fix-group, max 1 retry per fix; **1st E_repair invocation → N19 retry; 2nd → N17 replan; 3rd → cap-hit**)
- AGGREGATION (audit): N11
- AGGREGATION (fix): N16 (file-grouping) + N23 (per-fix outcome rollup)
- AGGREGATION (improvement): N26 (candidate filtering + categorization)
- CONDITIONAL ROUTING: N02 (audit dimensions), N16 (fix tier classification)
- ADVERSARIAL SELF-REVIEW: N14 Pass B (audit), N21 audit-rerun delta (fix)
- ANTI-OVER-ENGINEERING FILTER: N26 OEF (`--improve` only; utility/cost scoring + speculative-language discard)
- PARALLEL VERIFICATION: read-only fan-out in N04..N09 under `--deep`; N10 may fan out across findings. **Write nodes (N19, N20, N22) never fan out.**

### 2.3 Definitions

**Fix-group:** the unit produced by N16's `group-by-file/module` + topo-sort positioning. A fix-group contains 1+ findings sharing a file/module, ordered by dependency. The atomic loop in §5.3 operates on one fix-group at a time. **Multi-finding fix-groups (N>1 findings):** the inner loop in §5.3 runs N times sequentially in topo-sort order, each iteration applying-verifying-committing exactly **one** finding (per the "one concern per commit, no bundling" hard rule). Per-finding success is **not** persisted to the recovery manifest until the entire fix-group reaches its end-success boundary; if the process dies after finding 1 commits but before finding N completes, the resume-handler restarts the fix-group from scratch — the prior commits remain in git history but are detected by the idempotency check (state file > git-log) and skipped on resume's per-finding scan. Fix-groups are the unit of E_repair retry-or-replan, the unit of `failed`/`deferred (upstream-dependency-failed)` status assignment, and the unit of recovery-manifest `pending` tracking.

**Topo-sort dependency rule (deterministic):** within a single fix-group sharing one file, finding A depends on finding B iff A's `location` line range falls **at or after** the lines B's remediation diff edits — applying B first shifts the file's line numbers, so A must apply afterward and operate on the post-B line numbers. Across files in the same fix-group (rare; only when N16 grouped them by module rather than file), A depends on B iff A's file imports B's file or A's edited region grep-references a symbol B redefines. The topo-sort produces a deterministic linear order; ties (no dependency) are broken by `priority_score` descending, then by finding-id ascending.

**Behavioral vs cosmetic classification (deterministic):** a fix is **cosmetic** iff its remediation diff (a) modifies only comments, whitespace, or non-semantic identifier renames AND (b) does not change any token that the language compiler/interpreter evaluates at runtime. All other fixes are **behavioral**. The classification is independent of tier (Tier-1 fixes can be either behavioral or cosmetic). Behavioral fixes require a regression-prevention test (§5.2 step 9); cosmetic fixes record `regression_test_added.status: n/a`.

**Tier classification rules (N16, deterministic):**

- **Tier-1 (mechanical):** ALL of:
    - remediation diff is ≤2 lines edited within a single file
    - no function/method signature changes
    - no new identifiers (variables, functions, classes) introduced
    - target file is imported/referenced by ≤5 other files in the repo
    - finding's `confidence: HIGH`
    - finding's effort tag = `trivial`
- **Tier-2 (local logic):** all of:
    - remediation bounded to a single function body (no cross-function edits)
    - may add local symbols (locals, helper functions defined within scope)
    - no public-API change (no exported identifier renamed, removed, or signature-changed)
    - target file imported by ≤20 other files
- **Tier-3 (cross-cutting):** anything not satisfying Tier-1 or Tier-2 criteria. Includes:
    - multi-file remediation
    - any signature change to an exported identifier
    - schema files / migration files / config files
    - new files
    - findings flagged via `--escalate-finding`

If the remediation diff is non-deterministic (numbered steps without a literal patch), classifier defaults to **Tier-3** and notes `tier_classification_reason: "non-literal remediation"`.

### 2.4 Dimension plugin manifest (I1 — pluggable dimension registry)

Each dimension is a single markdown file with YAML frontmatter conforming to `schemas/dimension-plugin-v1.schema.json`. Loaded by R-ROUTE at startup from two locations in order:

1. Skill-bundled: `~/.claude/skills/epiphany-audit/dimensions/*.md`
2. User-installed: `~/.config/epiphany-audit/dimensions/*.md` (loaded after, may shadow by `name`)

**Plugin frontmatter (required):**

```yaml
schema_version: 1
name: <slug>                          # unique identifier — e.g., "juce-rt-safety", "iac-drift"
display_name: <human readable>        # e.g., "JUCE Real-Time Audio Safety"
version: <semver>
applies_to:
  languages: [<lang>, ...] | "*"      # "*" matches any
  project_markers: [<file-glob>, ...] # e.g., ["CMakeLists.txt", "JUCE/", "*.vst3"]
activation_triggers:                  # ALL must match for activation
  - type: file_present
    path: "**/JuceHeader.h"
  - type: import_grep
    pattern: "AudioProcessor"
    min_matches: 1
exclusions:                           # ANY match → dimension skipped even if triggers fire
  - type: project_size
    max_files: 1                      # skip on trivial projects
prompt_template: |                    # body of the analyzer prompt; receives {{project_model}} + {{file_subset}}
  Analyze the following code for <name> issues. Look for:
  - <specific failure mode 1>
  - <specific failure mode 2>
  Return findings in Audit Report Schema v1 format.
kb_route_query: null | "<query>"      # if set, R-ROUTE prefetches KB context via kb-route skill
intra_node_token_budget: 30000        # default; per-plugin override allowed
priority: low | medium | high         # affects ordering in the activation map under budget pressure
```

**Plugin body** (after frontmatter): freeform markdown documenting the dimension — failure modes, references, examples. Not parsed by R-ROUTE; for human readers and KB ingestion.

**Loader rules:**

- Plugin files that fail schema validation are logged and skipped (do not halt the audit).
- A user plugin with the same `name` as a bundled plugin shadows the bundled one entirely (no merge).
- Bundled plugins for CORRECTNESS and MAINTAINABILITY cannot be shadowed by user plugins (floor preservation; user plugins with these names are rejected with a warning).
- If two user plugins share a `name`, alphabetical order wins; the loser is logged and skipped.

**Built-in plugin set** (versioned with the skill): `correctness.md`, `architecture.md`, `performance.md`, `security.md`, `maintainability.md`. These are simple thin manifests; their actual prompt logic lives in `modules/N04..N08.md`. New built-ins (a11y, reproducibility, IaC drift, etc.) are added as additional bundled plugin files without changing the registry.

### 2.5 Audit-rerun tier policy (cost-aware)

The audit-rerun sub-step in N21 is **tiered by the highest tier of fixes successfully applied** in the current run. Rationale: Tier-1 fixes are mechanically incapable of inducing architectural regressions; the regression battery (tests/types/lint/build) covers them entirely. Re-running the full audit chain on Tier-1-only runs is wasted token spend.

| Highest applied tier | Audit-rerun behavior | Rationale |
|---|---|---|
| Tier-1 only | **Skip** entire audit-rerun sub-step | Battery covers the failure modes Tier-1 fixes can introduce |
| Tier-2 only | **Narrow rerun** — re-run only the dimension analyzers whose dimension tag matches the dimension tag of any applied fix (e.g., 4 CORRECTNESS fixes + 2 MAINTAINABILITY fixes → re-run N04 + N08; skip N05/N06/N07/N09.\*). **Plus a minimal post-pipeline subset on any new findings produced by the rerun:** N10 FPV (false-positive check), N11 (dedup against existing findings), N12 (priority_score), and N14 Pass A (mandatory-field validation). N13 formatter and N14 Pass B subagent spawn are skipped — the regression context provides the framing and Pass B's adversarial review is unnecessary for a delta. Set `audit_rerun_iteration: <n>` (≥1) on these findings' provenance. This subset ensures induced-regression findings reach E_rerun_fail with all schema-required fields populated; otherwise N16's F-VAL would reject them. | Dimension-relevant analyzers catch the failure modes; cross-dimension regressions from local-logic fixes are rare |
| Any Tier-3 applied | **Full rerun** (N01..N14 Pass A) — full audit pipeline including N10 FPV, N11 aggregator, N12 prioritizer, N13 formatter, N14 Pass A. N14 Pass B subagent is skipped (delta semantics — Pass B already validated the original findings). | Cross-cutting changes can induce regressions in any dimension |

**Mixed-tier runs:** the highest tier present determines policy. A run with Tier-1 + Tier-3 fixes triggers full rerun.

**Override:** `--full-rerun` flag forces full rerun regardless of applied tiers (for high-stakes runs). `--no-rerun` flag forces skip regardless (for token-budget-constrained runs); when set, the fix-report records `audit_rerun_delta: skipped-by-flag`.

**Recovery semantics:** the tier policy applies to fresh runs. On a *resumed* run after `halt-on-recovery-conflict`, the audit-rerun behavior is determined by the highest tier in the *combined* applied set (original + resumed).

---

## 3. Edge Table + State Machine

### 3.1 Edge Table

| Edge ID | Source → Target | Channel | Cardinality | Activation |
|---|---|---|---|---|
| E01 | N01 → N02 | data | 1:1 | always |
| E02 | N02 → N03 | data | 1:1 | always |
| E03 | N02 → N04..N09 | control | 1:N | per activation map; floor (CORRECTNESS, MAINTAINABILITY) always emits; skipped dims emit "skipped because" stub |
| E04 | N03 → user | interactive | 1:1 | only under `--deep`; default mode auto-adds HIGH-confidence gaps to activation map without prompting |
| E05 | N04..N09 → N10 | data | fan-in | always |
| E06 | N10 ↔ N04..N09 | feedback | refinement | BACKTRACKING — single-attempt cap on re-emit |
| E07 | N10 → N11 | data | 1:1 | always |
| E08 | N11 → N12 | data | 1:1 | always |
| E09 | N12 → N13 | data | 1:1 | always |
| E10 | N13 → N14 | control | 1:1 | always (halt-on-failure handled separately — see §3.3) |
| E11 | N14 → N15 | data | 1:1 | fires when Pass A succeeds AND (Pass B succeeds OR Pass B is `skipped-low-volume` per O1 conditional-spawn policy). Pass B subagent exec-error treated as failure → halt. Pass B `skipped-token-cap` under partial-report mode also permits E11 (with the partial-report warning attached). |
| E12 | N15 → user | interactive | 1:1 | "save?" prompt — fires only after N15 save-decision resolves (`--fix <report>` mode skips N01..N15 entirely; E12 never fires there) |
| E13 | N16 → N17 → N18 → N19 → N20 → N21 → N23 | data/control chain | 1:1 each | fix pipeline; entry from `--fix` mode or post-E21 fix-offer consent |
| E14 | E_repair cap-hit → N22 | control | 1:1 | E_repair 3rd invocation (cap-hit) → N22 finalizes recovery manifest for the failed fix-group. Note: working-tree discard (`git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>`) is performed by N19 immediately after N20 fail-signal, BEFORE E_repair routing — N22 is NOT involved in working-tree discard. |
| E_halt_partial | (halt-mid-fix-\*) → N23 → user | control | 1:1 | When `halt-mid-fix-on-perfix-cap-hit` or `halt-mid-fix-on-induced-regression-cap-hit` triggers AND every remaining group is blocked: route through N23 with `partial: true, halt_state: <id>` BEFORE emitting the halt envelope. N23 writes the partial fix report (body entries reflect work-to-date), then the halt envelope is emitted to the user. The live recovery manifest is preserved (NOT archived) so the user can `resume`. |
| E15 | N20 → N19 | feedback | 1:1 | on PerFixVerify success → next fix-group |
| E_repair | N20 fail OR N21 fail → N17 (replan) OR N19 (retry-with-failure-context) | feedback | 1:1 | bounded per fix-group: **1st E_repair invocation → N19 retry; 2nd → N17 replan; 3rd → cap-hit**, mark group `failed`, continue with independent groups |
| E_rerun_fail | N21 audit-rerun delta detects induced regression → N16 | feedback | 1:1 (batched) | re-triage with regression context; only for new findings in **files touched by fixes**. **Batching rule:** all induced regressions from a single audit-rerun are passed to N16 in one batched re-invocation, not N separate invocations. |
| E_diffscope | N21 diff-scope-check fail → halt | control | 1:0 | scope-creep detected; do NOT auto-revert; user authorize-or-revert |
| E_finalize | N23 → N22 | control | 1:1 | planned termination of fix pipeline. **Sequencing (strictly sequential, NOT parallel):** (1) N23 generates `fix_report_id` and writes the fix report file to disk; (2) E_finalize fires; (3) N22 reads the just-written fix report to extract `fix_report_id`, writes a final manifest record including it, then **archives** the recovery manifest to `~/docs/epiphany/audit/.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json` and removes the live `<report-id>.json`; (4) E_complete fires last, emitting the user-facing summary. Halt-paths bypass E_finalize: on `halt-mid-fix-on-perfix-cap-hit` / `halt-mid-fix-on-induced-regression-cap-hit`, the manifest stays live so the user can `resume` next run (E_halt_partial routes through N23 for the partial report instead, without manifest archival). |
| E_complete | N22 → user | data | 1:1 | terminal — fires after both N23 has written the fix report AND N22 has finalized + archived the manifest. Emits the user-facing summary (fix report path, branch, last_known_good_sha). |
| E16 | N15 → N24 | control | 1:1 | `--improve` only. Fires regardless of whether user accepted or declined save (improvement pipeline uses the N14-validated findings already in memory from the audit run, not the saved file). Fires after save prompt resolves; does NOT fire when `--fix` mode is active (improvement pipeline skipped under `--fix`). |
| E17 | N24 → N25 | data | 1:1 | always within `--improve` subpipeline |
| E18 | N25 → N26 | data | 1:1 | always within `--improve` subpipeline |
| E19 | N26 → N27 | data | 1:1 | always within `--improve` subpipeline |
| E20 | N27 → user | data | 1:1 | terminal for `--improve` subpipeline; improvement report written unconditionally to `improvement-reports/` (no save prompt) and summarized to user. If audit is also in no-flag mode (fix offered after audit), E21 fires AFTER E20 resolves. |
| E21 | N27 → user (no-flag mode) OR N15 → user (non-improve no-flag mode) | interactive | 1:1 | "fix?" offer — in `--improve` no-flag mode, fires after E20 resolves (improvement pipeline complete); in no-flag mode without `--improve`, fires directly after E12 (save-decision resolved). Never fires under `--audit --improve` (no fix offered) or `--fix` (fix pipeline entered from E13 without a fix-offer prompt). |

### 3.2 State Machine

| Start | Flag | Save offered? | Fix offered? | Reachable end states |
|---|---|---|---|---|
| invoke | (none) | yes | yes (per-tier batch confirm default) | `saved+fixed`, `saved+not-fixed`, `not-saved+not-fixed-emitted-only` |
| invoke | `--audit` | yes | **no** (explicit) | `saved+audit-only`, `not-saved-emitted-only` |
| invoke | `--fix <report>` | n/a (skip N01..N15) | yes (gated by N16 F-VAL ingest) | `fixed`, `partial-fixed-with-deferred`, `halt-pre-fix-on-validator-failure`, `halt-mid-fix-on-perfix-cap-hit`, `halt-mid-fix-on-induced-regression-cap-hit`, `halt-on-files-outside-tree`, `halt-on-invalid-finding-id`, `halt-on-conflicting-fixes`, `halt-on-empty-or-unfixable-report`, `halt-on-stale-source-report` |
| invoke | `--verbose` | combines | combines | verbose variants |
| invoke | `--deep` | combines | combines | deep variants; +interactive B-FIND prompt; +token-cap partial-report state |
| invoke | `--auto` | combines (fix-mode) | combines | Tier-1 silent; Tier-2 batch-confirmed; Tier-3 per-fix |
| invoke | `--confirm-all` | combines (fix-mode) | combines | per-fix confirmation regardless of tier |
| invoke | `--dry-run` | combines (fix-mode) | n/a | terminates after N17 with Dry-Run Plan v1 written to `~/docs/epiphany/audit/dry-run-plans/`; no apply, no commits, no branch |
| invoke | `--dry-run` without `--fix` and no audit-pending offer | n/a | n/a | warning emitted: *"--dry-run has no effect without --fix or post-audit fix offer"*; ignored, run continues as if not passed |
| invoke | `--improve` | yes | yes (default) | improvement subpipeline (N24..N27) runs after save-prompt; improvement report saved to `improvement-reports/`; fix offered after |
| invoke | `--audit --improve` | yes | **no** | improvement subpipeline runs after save; no fix offered |
| invoke | `--fix <report> --improve` | n/a | yes | warning emitted: *"--improve is ignored with --fix; run without --fix to include improvement analysis"*; improvement pipeline skipped; fix pipeline proceeds normally |
| invoke | `--improve --dry-run` | yes (audit) | n/a (fix) | improvement analysis runs (produces improvement report); `--dry-run` suppresses fix application as normal; improvement report still written |

**Note on "Save offered?" for `--improve` rows:** this column refers to the **audit report** save prompt only. The improvement report is written **unconditionally** by N27 (no prompt) — see N27 and E20. The two saves are independent.

### 3.3 Halt states (first-class outcomes)

Every halt state emits a structured halt envelope at the top of the user-facing message:

```
{halt_state: <state-id>, subreason: <text>, diagnostic: <details>}
```

| Halt state | Triggered at | Subreason axis |
|---|---|---|
| `halt-pre-audit` | resolution chain | no resolvable target |
| `halt-suspicious-target` | resolution + gate | `$HOME` / wrapper repo / denylist match |
| `halt-ambiguous-target` | resolution | nested git / multi-project session / polyglot inconclusive |
| `halt-on-flag-conflict` | invocation parse | mutually exclusive flags violated |
| `halt-on-flag-rejection` | invocation parse | unsupported flag (e.g., `--demote-finding`) |
| `halt-on-target-conflict` | resolution | both `<path>` and `--fix <report>` given but their targets disagree |
| `halt-on-unresolvable-fix-report` | resolution | `<report>` could not be resolved |
| `halt-on-ambiguous-fix-report` | resolution | bare `<report>` matched multiple files |
| `halt-on-mismatched-version` | F-VAL | `tool_version` skew, user declined |
| `halt-pre-fix-on-validator-failure` | N16 F-VAL | schema fail / suspicious-content user-declined |
| `halt-on-empty-or-unfixable-report` | N16 ingest | source audit report has zero main-body findings |
| `halt-on-conflicting-fixes` | N16 | two findings have incompatible edits — defined as: their remediation diffs both modify any character within the same line range, OR both insert content at the same line. Non-overlapping line-range edits within the same `file:line` (e.g., one edits columns 1-10, the other edits columns 30-50 with no shared chars) are **compatible** and merged into a single fix-group rather than halting. Three-or-more-way conflicts: any pair within the conflict cluster failing the compatibility check halts the cluster. |
| `halt-on-test-cmd-unknown` | N18 | no test command auto-detected and `--test-cmd` not provided |
| `halt-on-baseline-failure` | N18 | baseline capture failed (runner crash; pre-existing failures are NOT a baseline failure) |
| `halt-on-git-state-incompatible` | N18 | dirty tree, detached HEAD, no commits |
| `halt-on-q-gate-failure` | N14 | subreason: `pass-a` / `pass-b` / `pass-b-exec-error` |
| `halt-mid-fix-on-perfix-cap-hit` | E_repair (post-N20) | E_repair retries exhausted on a fix-group AND every remaining group blocked by it |
| `halt-mid-fix-on-induced-regression-cap-hit` | E_repair (post-rerun) | retry cap exhausted on a regression-induced re-triage group; chain: N21 detects → N16 re-triages → N17/N19/N20 re-execute → fail → E_repair → cap |
| `halt-on-scope-creep` | N21 diff-scope | unmapped diff hunks; do NOT auto-revert; user choice = authorize (manual-edits section) or revert manually |
| `halt-on-token-cap` | `--deep` checkpoint | accumulated context > 80k; partial report emitted with `token_cap_partial: true` |
| `halt-on-recovery-conflict` | run start | recovery manifest from prior interrupted run; user choice = resume / fresh / abort |
| `halt-on-user-abort` | any interactive prompt | ctrl-C / explicit halt |
| `halt-on-stale-source-report` | F-VAL | `source_audit_report` file missing or `source_audit_report_sha256` mismatch |
| `halt-no-source-detected` | N01 | target resolved but contains no parseable source files (binary-only, empty, or documentation-only repo) |
| `halt-on-files-outside-tree` | N16 F-VAL | input audit report references files outside `audit_target` git tree; refuse to write outside source tree |
| `halt-on-invalid-finding-id` | N16 F-VAL post-validation | `--escalate-finding F00N` references a finding-id absent from the resolved audit report; checked after F-VAL schema validation passes but before triage begins. Diagnostic lists the available finding-ids in the report. |

---

## 4. Report Schemas v1

### 4.1 Audit Report Schema v1

Saved to `~/docs/epiphany/audit/<project-slug>-<YYYYMMDD>-<HHMMSS>.md`. Authoritative JSON schema: `schemas/audit-report-v1.schema.json`.

**Frontmatter (YAML):**

```yaml
schema_version: 1
report_id: <uuid v4>
audit_target: <absolute path>
audit_timestamp: <ISO 8601>
tool_version: <epiphany-audit semver>
flags: [<allowed sets>]                           # see §4.1.1
language_summary: { python: 12, ts: 4, ... }      # informational; snapshot at audit time
build_manifest: <path or null>
test_command: <auto-detected or --test-cmd value or null>

dimensions_activated: [CORRECTNESS, MAINTAINABILITY, ...]   # floor (CORR + MAINT) always present
dimensions_skipped:
  - dimension: SECURITY
    reason: "no network/IO/DB/auth surfaces detected"

subtrees:                                          # only present for heterogeneous monorepos
  - root: <path>
    dimensions_activated: [...]
    dimensions_skipped: [...]
subtree_grouping_applied: false                    # true when detected subtrees exceed --monorepo-subtree-limit;
                                                   # lowest-priority subtrees collapsed into synthetic rest-of-repo subtree

gap_dimensions_offered: [...]      # populated only under --deep (B-FIND interactive)
gap_dimensions_auto_added: [...]   # populated under default mode (HIGH-confidence auto-add)
gap_dimensions_accepted: [...]     # subset of offered the user opted into

token_cap_partial: false
truncated_at_node: null            # the node that was running when the inter-node 80k budget was tripped
                                    # (i.e., aborted mid-execution); findings already produced by prior nodes
                                    # are kept. Distinct from "last successful node" (= prior node) and
                                    # "first skipped node" (= next node after this one). Format: node id (e.g., "N07").

improvement_report_ref: null | <absolute path>   # populated by N27 after --improve pipeline completes;
                                                  # null if --improve not set or N27 did not run
                                                  # (catastrophic failure before N27 executed).
                                                  # N24..N26 partial failure → N27 still runs → ref is
                                                  # populated (improvement_partial: true in that report).

dimensions_partial:                # populated only when intra-node soft budget tripped on a dimension (--deep)
  - dimension: SECURITY
    reason: "intra-node soft budget exceeded — coverage incomplete for dimension SECURITY"

q_gate:
  pass_a: pass | pass-minimal | fail | skipped-token-cap
  pass_b: pass | fail | exec-error | skipped-token-cap | skipped-low-volume
  pass_b_lens: <subagent identifier or null>
  pass_b_skip_reason: null | "fewer than 5 findings and no CRITICAL/HIGH severity"

partial_report_warning: null | "<warning text>"   # rendered at top of body when truncated
```

#### 4.1.1 `flags` field — allowed combinations

`flags` is a mutually-exclusive set (validated by schema):

```
allowed: [audit] | [audit, verbose] | [audit, deep] | [audit, verbose, deep]
       | []      | [verbose]        | [deep]        | [verbose, deep]
       (autonomy/oversight flags appear in fix report, not audit report;
        [fix] combinations are absent — in --fix mode N01..N15 are skipped
        and no audit report is produced by that run)
```

The `improve` flag is orthogonal to mode and verbosity flags. Any of the above sets may additionally include `improve` (e.g., `[audit, improve]`, `[improve, deep]`, `[verbose, improve, deep]`). The schema allows `improve` as an optional member of any set above. (The runtime behavior of `--fix --improve` — warning + skip — is documented in the fix report flags constraint in §4.2, not in the audit report schema.)

**Body — one `## Finding F00N` section per finding (mandatory keys):**

```yaml
id: F001
location: src/parser.py:142          # or file:line-range; verified via Read
dimensions: [CORRECTNESS, MAINTAINABILITY]
severity: CRITICAL | HIGH | MEDIUM | LOW | INFO
confidence: HIGH | MEDIUM | LOW
evidence_excerpt: |                  # ≤12 lines default; ≤30 lines if evidence_excerpt_extended: true
  <verbatim source>
evidence_excerpt_extended: false     # boolean. Schema rule: may only be true when confidence: HIGH.
                                     # Schema validation rejects:
                                     #   - excerpts >12 lines with evidence_excerpt_extended: false
                                     #   - excerpts >30 lines (hard cap regardless)
                                     #   - evidence_excerpt_extended: true on non-HIGH-confidence findings
rationale: |
  <why this is a defect>
remediation: |
  <concrete fix as diff or numbered steps; never "consider improving X">
false_positive_check:                # 4-question integrity check; complementary to tests_present_signal (separate metadata, not a 5th question)
  intentional:           { value: false, justification: null }
  file_symbol_verified:  { value: true,  justification: "Read at <path>:<lines>" }
  reachable_from_entry:  { value: true,  justification: "called by <fn> at <path>:<line>" }
  fix_breaks_dependents: { value: false, justification: "grep clean" }
effort: trivial | modest | significant   # required; drives priority_score (§4.1.2) and N16 tier classification (§2.3)
priority_score: 9.0                  # see §4.1.2; computed as (severity × confidence) / effort
verify_by: null                      # only present when confidence: LOW; what would lift confidence
tests_present_signal: false          # complementary metadata, not a 5th false-positive question.
                                     # Set true when test-dir grep matches the involved fn/class/module.
                                     # Effect: elevates confidence floor — finding may still pass if the
                                     # test demonstrably doesn't cover the failure path, but Confidence < MEDIUM
                                     # is rejected by Q-GATE Pass A when this signal is true.
provenance:                          # I2 — finding-level trace; mandatory on every finding
  node: N04                          # which analyzer produced this finding (N04..N09 or N09.<plugin-name>)
  mode: inline | subagent            # how that node executed for this run
  model: <model-id>                  # e.g., "claude-opus-4-7"; populated even for inline (the orchestrator's model)
  prompt_hash: <sha256 of effective prompt template>
  plugin_name: null | <slug>         # null for built-in N04..N08; <slug> for plugin-instantiated N09.<x>
  plugin_version: null | <semver>    # populated when plugin_name is set
  audit_rerun_iteration: 0           # 0 on the original audit; 1+ when produced during fix-mode N21 audit-rerun
  q_gate_pass_b_demoted: false       # true if a prior Pass B run demoted this finding (kept for traceability)
```

#### 4.1.2 `priority_score` numeric mapping (deterministic)

- **Severity:** CRITICAL=4, HIGH=3, MEDIUM=2, LOW=1, INFO=0
- **Confidence:** HIGH=3, MEDIUM=2, LOW=1
- **Effort:** trivial=1, modest=2, significant=3
- `priority_score = (severity × confidence) / effort`
- INFO findings score 0 by definition.

#### 4.1.3 Severity tier definitions (verbatim)

```
CRITICAL = data loss, security breach, crash on common path, corruption
HIGH     = crash on edge case, wrong output silently, perf regression > 2x
MEDIUM   = degraded UX, recoverable error mishandled, maintainability cliff
LOW      = code smell with concrete future cost
INFO     = observation, no action required
```

#### 4.1.4 False-positive check disposition (N10 FPV rules)

After populating the four `false_positive_check` fields, N10 applies these deterministic rules:

| Condition | Disposition |
|---|---|
| `file_symbol_verified.value == false` | **Discard** the finding entirely (no evidence the location even exists; not even surfaced as Unverified Hypothesis). |
| `intentional.value == true` | **Demote to Unverified Hypotheses** (the code is likely intentional; a human should review). |
| `reachable_from_entry.value == false` AND `severity ∈ {CRITICAL, HIGH}` | **Demote to Unverified Hypotheses** (unreachable code with a high severity claim is suspicious — likely overestimated impact). |
| `reachable_from_entry.value == false` AND `severity ∈ {MEDIUM, LOW, INFO}` | **Pass** but record `notes: "unreachable from declared entry points"`. |
| `fix_breaks_dependents.value == true` | **Pass** but require `confidence: HIGH` (Pass A floor); demote to Unverified Hypotheses if confidence is lower. The remediation field MUST address the dependents (e.g., paired updates) or the finding is demoted. |
| All four pass (intentional=false, file_symbol_verified=true, reachable_from_entry=true, fix_breaks_dependents=false) | **Pass** unconditionally. |

N10 is the only node that applies these rules; N14 Pass A trusts N10's disposition and does not re-check.

#### 4.1.5 Top-of-body sections

1. **"Resolve-before-testing" punch list:** all CRITICAL + all HIGH findings always; then add MEDIUM until total reaches 15 or all findings exhausted, whichever first. Sorted by `priority_score` descending.
2. **Partial-report warning** (only present when `token_cap_partial: true`).
3. **Main body:** all findings, grouped by dimension or by file (implementer choice; recommend by file for actionability).
4. **"Unverified Hypotheses" section** (below main body, separated): demoted findings that failed false-positive checks. **Not consumable by `--fix`**; advisory only.

### 4.2 Fix Report Schema v1

Saved to `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-<YYYYMMDD>-<HHMMSS>.md`. Authoritative JSON schema: `schemas/fix-report-v1.schema.json`. Emitted on **planned termination** (N23, full report) AND on **halt-mid-fix-\*** (`halt-mid-fix-on-perfix-cap-hit` / `halt-mid-fix-on-induced-regression-cap-hit`, partial report). On halt-mid-fix-\*, the pipeline routes through N23 with `partial: true` set in frontmatter BEFORE the halt envelope is emitted to the user — body entries reflect what was applied/failed/deferred up to the halt point. Only **process death** (unplanned termination — kill -9, OS crash, network drop mid-subagent) leaves no fix report; the recovery manifest is the sole at-rest record in that case.

**Frontmatter (YAML):**

```yaml
schema_version: 1
fix_report_id: <uuid v4>
partial: false                  # true when emitted on halt-mid-fix-* (cap-hit halts);
                                # false when emitted on planned termination via N23.
                                # Partial reports include all body entries up to the halt point;
                                # the halt envelope is emitted to the user AFTER N23 writes the report.
halt_state: null | <halt-state-id>   # populated on partial reports; null on planned-completion reports
source_audit_report: <absolute path>
source_audit_report_sha256: <hash captured at F-VAL ingest of THIS run>
                  # Timing semantics:
                  #   - Always captured at F-VAL ingest of the current --fix run.
                  #   - On a fresh --fix run: hash is captured and recorded; no prior value to compare.
                  #   - On a resumed run: recompute hash of the report on disk; compare against the
                  #     hash stored in the prior fix-report or recovery manifest. Mismatch →
                  #     halt-on-stale-source-report.
source_report_id: <uuid from source>
fix_run_timestamp: <ISO 8601>
branch: <git branch — e.g., epiphany-audit/<source-report-id>-YYYYMMDD>
                  # null under --dry-run
flags: [<allowed sets — see constraint below>]
test_command_used: <auto-detected or --test-cmd value>
# flags constraint (validated by schema):
#   At most one of {auto, confirm-all, dry-run}.
#   verbose, deep are orthogonal.
#   Allowed sets:
#     []  [auto]  [confirm-all]  [dry-run]
#     [verbose]  [deep]  [verbose, deep]
#     [auto, verbose]  [auto, deep]  [auto, verbose, deep]
#     [confirm-all, verbose]  [confirm-all, deep]  [confirm-all, verbose, deep]
#     [dry-run, verbose]  [dry-run, deep]  [dry-run, verbose, deep]
#   `improve` flag: when --fix --improve is passed, the improvement pipeline is skipped
#   (warning emitted at runtime). The fix report records EFFECTIVE flags only (no `improve`
#   in flags — the improvement pipeline did not run). The skip event is logged in the
#   structured event log at ~/docs/epiphany/audit/.logs/<report-id>.jsonl.

baseline_metrics:
  tests:      { passed: N, failed: N, skipped: N }
  type_check: { errors: N }
  lint:       { total_warnings: N }
  build:      pass | fail
post_metrics:
  tests:      { passed: N, failed: N, skipped: N }
  type_check: { errors: N }
  lint:       { total_warnings: N, new_warnings_in_changed_regions: N }
  build:      pass | fail
                  # all post_metrics: null under --dry-run

audit_rerun_delta:
  scope: full | narrow | skipped-tier-policy | skipped-by-flag | null
                  # null when N21 did not execute (--dry-run, or run halted before regression battery)
                  # skipped-tier-policy: Tier-1-only run, audit-rerun skipped per §2.5
                  # skipped-by-flag: --no-rerun was passed
                  # narrow: Tier-2-only run, audit-rerun ran on dimension-matched subset
                  # full: Tier-3 present (or --full-rerun), audit-rerun ran N01..N13
  reran_dimensions: [...]                         # populated when scope=narrow; lists which N04..N09 instances ran
  resolved:                 [F001, F003, ...]    # null/empty when scope is skipped-*
  fix_induced_regressions:  [Fnew001, ...]       # null/empty when scope is skipped-*
  unchanged:                [F004, ...]          # null/empty when scope is skipped-*
  new_findings_discovered:  [Fnew099, ...]       # untouched-file new findings (not regressions); null/empty when skipped-*

diff_scope_check: pass | fail-with-unmapped-hunks | n/a   # n/a under --dry-run
unmapped_hunks: []          # only when fail

recovery_manifest_ref: null | <absolute path>
                  # Population semantics:
                  #   - null on a fresh run that completed without recovery (no prior interrupted run).
                  #   - On a RESUMED run that reaches planned termination: populated with the
                  #     archive path (~/docs/epiphany/audit/.recovery/.archive/<report-id>-completed-<ISO-timestamp>.json)
                  #     after N22 archives the manifest in E_finalize. Pointer references the on-disk
                  #     archived file, NOT the (now-removed) live <report-id>.json path.
                  #   - On a partial fix report (halt-mid-fix-*): populated with the LIVE manifest path
                  #     (~/docs/epiphany/audit/.recovery/<report-id>.json) since halt-paths preserve
                  #     the live manifest for `resume`.
last_known_good_sha: null | <sha>
                  # Population semantics:
                  #   - Always populated when ≥1 fix-group successfully committed during this run.
                  #     Holds the sha of the last successful commit on the audit branch.
                  #   - null when zero commits made (e.g., --dry-run, immediate halt before N19).
```

**Body — sorted by status priority (failed > induced-regression > deferred > simulated > verified > skipped). One entry per finding:**

```yaml
id: F001
status: verified | failed | deferred | skipped | induced-regression | simulated
                  # `simulated` only present under --dry-run (would-have-been-applied)
status_qualifier: null | upstream-dependency-failed | induced-regression | caused-by | stale-reference | user-declined-batch | already-applied
status_qualifier_target: null | <finding-id>
                  # populated when qualifier references another finding
                  # e.g., qualifier=induced-regression, target=Fnew001 (the new finding caused)
                  # e.g., qualifier=caused-by, target=F003 (the originating fix that caused this)
tier: 1 | 2 | 3
tier_classification_reason: <string>  # populated when classifier had to fall back (e.g., "non-literal remediation" → Tier-3)
commit: <sha> | null                  # null for deferred/skipped/simulated
verification_outcome: pass | fail | n/a | simulated      # simulated only under --dry-run
failure_context:                      # only when status=failed
  failure_class: verification-failure | commit-hook-failure | git-operation-failure | type-check-failure | targeted-test-failure
  diagnostic: |
    <free text from N20 fail-signal>
defer_reason: |                       # only when status=deferred
  <free text question for user>
regression_test_added:
  status: yes | no | n/a              # n/a for cosmetic fixes
  location: <file:line> | null        # populated when status=yes
  reason: <string> | null             # populated when status=no — e.g., "no test framework detected — manual test recommended"
  deferred_to_followup_commit: <sha> | null   # populated when test landed in a paired follow-up commit per N19's policy
notes: |                              # optional
  <free text>
```

**Top-of-body sections:**

1. **Deferred items** — consolidated list with `defer_reason` per item; surfaced first.
2. **Manual edits** (only present if user authorized unmapped hunks at `halt-on-scope-creep` and chose to continue rather than revert): per-hunk listing of file:line + summary + user-provided rationale.
3. **Recovery** (only present when fix run died mid-flight and was resumed): cites absolute path to recovery manifest, last-known-good sha, applied/in-flight/pending finding-ids.

### 4.3 Cross-schema invariants

- `source_report_id` in fix-report = `report_id` in audit-report = `source_report_id` in dry-run plan = `source_report_id` in improvement report.
- `source_audit_report_sha256` is verified at `--fix` start; halt with `halt-on-stale-source-report` if file missing or hash mismatched.
- Status-priority sort order intentionally surfaces problems first.
- F-VAL strict schema validation applies to **all** `--fix` ingests, including hand-edited audit reports.
- "Unverified Hypotheses" findings are **not consumable** by `--fix`; F-VAL ingests only main-body findings.
- **Count-sum invariants (validated by JSON-Schema constraints):**
    - **Dry-run plan §4.4:** `triage_summary.total_findings == triage_summary.tier_1_count + triage_summary.tier_2_count + triage_summary.tier_3_count + triage_summary.deferred_at_triage`. The number of body entries with `status: simulated` equals `tier_1_count + tier_2_count + tier_3_count`. The number of body entries with `status: deferred-at-triage` equals `triage_summary.deferred_at_triage`.
    - **Improvement report §4.5:** `survivors == notable + quick_wins + worthwhile` AND `total_candidates == filtered_out + survivors`. The number of body entries equals `survivors`.
    - **Audit report §4.1:** the number of body entries equals the count summed across `dimensions_activated`'s findings (informational; no frontmatter total — but the punch list must include all CRITICAL and HIGH findings present in the body).
- Authoritative schemas: `schemas/audit-report-v1.schema.json`, `schemas/fix-report-v1.schema.json`, `schemas/dry-run-plan-v1.schema.json`, `schemas/dimension-plugin-v1.schema.json`, `schemas/improvement-report-v1.schema.json`. This section is the human-readable companion.

### 4.4 Dry-Run Plan Schema v1 (I3)

Output of `--dry-run` mode. Saved to `~/docs/epiphany/audit/dry-run-plans/<source-report-id>-dryrun-<YYYYMMDD>-<HHMMSS>.md`. Authoritative JSON schema: `schemas/dry-run-plan-v1.schema.json`. Emitted by N17 after triage/tier-classification; pipeline halts here under `--dry-run` (no PreFlight, no FixApplier, no branch).

**Frontmatter (YAML):**

```yaml
schema_version: 1
plan_id: <uuid v4>
source_audit_report: <absolute path>
source_audit_report_sha256: <hash>
source_report_id: <uuid from source>
plan_timestamp: <ISO 8601>
flags: [dry-run, ...]                      # always includes dry-run; verbose/deep orthogonal

triage_summary:
  total_findings: 23
  tier_1_count: 12
  tier_2_count: 6
  tier_3_count: 4
  deferred_at_triage: 1                     # findings N16 deferred for uncertainty / non-literal remediation
  conflicting_groups: []                    # under --dry-run: recorded here rather than halting;
                                             # in live mode these would trigger halt-on-conflicting-fixes.
                                             # Each entry conforms to the conflicting_groups entry schema below.

simulated_branch: epiphany-audit/<source-report-id>-YYYYMMDD   # what branch WOULD have been created
test_command_resolved: <auto-detected or --test-cmd value or null>
```

**Body — sorted by tier (1 → 2 → 3) then by `priority_score` descending. One entry per finding:**

```yaml
id: F001
status: simulated
tier: 1 | 2 | 3
tier_classification_reason: <string or null>
fix_group_id: <generated id>                # which fix-group this finding belongs to
proposed_diff: |                            # the literal patch N19 WOULD have applied;
                                              # for non-literal-remediation Tier-3 entries
                                              # (tier_classification_reason: "non-literal remediation"),
                                              # this field holds the verbatim numbered-steps remediation
                                              # from the audit report and is prefixed with the literal
                                              # marker line `# NON-LITERAL REMEDIATION — manual application required`.
                                              # N19 in a live run would defer such findings rather than apply them.
  -    for i in range(len(tokens) - 1):
  +    for i in range(len(tokens)):
files_touched: [src/parser.py]
blast_radius:
  callers_grepped: 3                        # number of callers found by N16's grep
  imports_referencing: 1                    # how many files import the touched file
projected_verification:
  targeted_tests: [tests/test_parser.py::test_emit_all]
  type_check_scope: [src/parser.py]
projected_commit_message: "[AUDIT-001] fix off-by-one in parser token loop"
projected_regression_test: yes (tests/test_parser.py::test_emit_all_tokens) | no | n/a
notes: |                                    # optional
  <free text>
```

**`conflicting_groups` entry schema** (used in `triage_summary.conflicting_groups` array under `--dry-run`):

```yaml
group_id: <generated id>
findings: [F00N, F00M, ...]              # finding-ids in this conflict cluster
file: <path>
lines: <line-range — e.g., "142" or "142-145">
incompatibility_summary: |               # free text summarizing why the edits conflict
  <e.g., "F003 and F007 both rewrite line 142's loop bound; the rewrites differ —
          F003 changes `range(len(tokens) - 1)` to `range(len(tokens))`,
          F007 changes it to `range(len(tokens) - 2)`. Resolve manually before --fix.">
```

**Deferred-at-triage entry schema** (used in top-of-body section 3; one entry per deferred finding):

```yaml
id: F00N
status: deferred-at-triage
defer_reason: |                       # required — why N16 deferred
  <free text — e.g., "non-literal remediation requires manual classification",
                     "conflicting edit with F003 — resolve manually",
                     "remediation references symbol not found in current tree">
original_severity: <copied from audit report>
original_confidence: <copied from audit report>
original_remediation: |               # verbatim from audit report — no proposed_diff because N16 didn't attempt
  <copy of the audit report's remediation field>
notes: |                              # optional
  <free text>
```

Deferred-at-triage entries are NOT classified by tier (no `tier` or `fix_group_id` field) and have no `proposed_diff`, `blast_radius`, or `projected_*` fields — N16 declined to plan them. Their `priority_score` from the audit report is preserved in `notes` for sort-tie-breaking only; the deferred-at-triage section is sorted by `priority_score` descending.

**Top-of-body sections:**

1. **Plan summary** — counts, branch name, what would happen if `--dry-run` were dropped.
2. **Per-tier sections** — Tier-1, Tier-2, Tier-3 with the proposed fixes.
3. **Deferred-at-triage** — findings N16 chose not to attempt, per the schema above; surfaced separately.

**Cross-schema invariants extension:**

- `plan_id` is distinct from any future `fix_report_id`. A dry-run plan and a subsequent live fix run on the same audit report produce two different artifacts; both reference the same `source_report_id`.
- A dry-run plan can be referenced (path or SHA-256) when scheduling the corresponding live `--fix` run; if referenced, the live run's tier classifications must match the plan's, otherwise emit a `tier-drift` warning per affected finding. The warning is emitted as a user-facing message during N16 triage (one warning per drifted finding, format: `[tier-drift] F00N: plan classified Tier-N; re-triage applied Tier-M`) and recorded in the finding's `notes` field in the fix report body.

### 4.5 Improvement Report Schema v1

Output of `--improve` mode. Saved to `~/docs/epiphany/audit/improvement-reports/<project-slug>-<YYYYMMDD>-<HHMMSS>-improve.md`. Authoritative JSON schema: `schemas/improvement-report-v1.schema.json`. Emitted by N27 after the OEF filter runs; pipeline failure in N24..N26 produces a partial report with `improvement_partial: true` rather than halting.

**Frontmatter (YAML):**

```yaml
schema_version: 1
improvement_report_id: <uuid v4>
source_audit_report: <absolute path>
source_report_id: <uuid from source audit>
audit_target: <absolute path>
improvement_timestamp: <ISO 8601>
tool_version: <epiphany-audit semver>
flags: [improve, ...]
# flags constraint (validated by schema):
#   Must always include "improve". Other allowed members (orthogonal to each other):
#     [improve]  [improve, verbose]  [improve, deep]  [improve, verbose, deep]
#   No autonomy flags (auto/confirm-all/dry-run) appear in the improvement report.
#   No fix-mode flags appear in the improvement report (--improve is ignored with --fix at runtime).

improvement_partial: false             # true if N24..N26 pipeline failed partway; N27 emits best-effort output
total_candidates: 14                   # raw count from N25 before filtering
filtered_out: 9                        # discarded by N26 OEF
survivors: 5                           # passed OEF
notable: 1                             # utility = 3, cost ≤ 2 (takes precedence over other categories)
quick_wins: 2                          # utility ≥ 2, cost = 1, and not notable
worthwhile: 2                          # all remaining survivors (utility ≥ cost ≥ 2, and not notable)
```

**Body — sorted by category (notable → quick-win → worthwhile) then by `utility_score` descending. One entry per improvement:**

```yaml
## Improvement I001

id: I001
category: quick-win | worthwhile | notable
area: developer-experience | testing | architecture | performance | tooling | dependencies | documentation
utility_score: 2               # 1=marginal, 2=notable, 3=high
cost_score: 1                  # 1=trivial, 2=modest, 3=significant
description: |
  <concrete description of the improvement — no hedging language; specific to this project>
action: |
  <specific action to take — never "consider improving X"; always "do Y to achieve Z">
success_measure: |
  <observable outcome that confirms the improvement worked>
notes: |                        # optional
  <free text>
```

**Top-of-body sections:**

1. **Summary** — total candidates brainstormed, N filtered (discarded by utility=1 floor, cost > utility, or speculative language), N survivors by category. If zero survivors: *"No improvements above the utility/cost threshold were found. This is a valid result — the project may be well-optimized in its current state."*
2. **Notable improvements** (utility = 3, cost ≤ 2) — highest return on investment; listed first.
3. **Quick wins** (utility ≥ 2, cost = 1, and not notable) — fast gains.
4. **Worthwhile improvements** (utility ≥ cost ≥ 2, and not notable) — justified medium-investment improvements.
5. **Filtered improvements** (only under `--verbose`) — each discarded candidate with the OEF discard rationale.

**Cross-schema invariant:** `source_report_id` in improvement-report = `report_id` in the linked audit report. Improvement reports are read-only artifacts; they are NOT consumed by `--fix`. N27 patches `improvement_report_ref` into the already-saved audit report frontmatter (if save was accepted) after writing the improvement report. If the audit report was not saved (user declined), `improvement_report_ref` remains null in the in-memory report only.

---

## 5. Verification Gates + Fix Safeguard Pipeline

### 5.1 Verification gates summary

| Gate | Node | Owner of | Failure consequence |
|---|---|---|---|
| Q-GATE Pass A (mechanical) | N14 inline | Mandatory-field completeness; location verification (consumes N10 FPV location-cache per §7 cache contract; falls back to Read only on cache miss); CRITICAL/HIGH × Confidence ≥ MEDIUM floor; dup merge; no-comment-echo; no-LOW-only warning (emit a user-facing warning when every main-body finding is severity LOW or INFO — signals the audit may be under-sensitive; does NOT block the report); under token-cap, runs as `pass-a-minimal` (mandatory-field + location only) | `halt-on-q-gate-failure` (subreason: `pass-a`) |
| Q-GATE Pass B (adversarial) | N14 conditional subagent | Anti-iatrogenic check (does remediation introduce a worse defect?); evidence-rationale coherence; dimension-classification correctness. **Activates only when ≥5 findings OR any CRITICAL/HIGH severity OR `--deep`.** Skipped otherwise (`pass_b: skipped-low-volume`). | `halt-on-q-gate-failure` (subreason: `pass-b` for content-fail; `pass-b-exec-error` for subagent timeout/crash). When skipped-low-volume, no halt. |
| F-VAL ingest | N16 inline | Schema validation; SHA-256 capture; suspicious-content prompt (overrides `--auto`) | `halt-pre-fix-on-validator-failure` |
| Idempotency | N16 inline | State file authoritative; git-log fallback. State-file-vs-git-log conflict → warn-and-prompt user (state may show applied at sha not findable in current branch). | warn + user override |
| Tier classification | N16 inline | Tier-1/2/3 routing + defer-on-uncertainty + conflicting-edit detection | `halt-on-conflicting-fixes` |
| Fix-plan approval | N17 inline | Per-tier batch confirmation default; tier presented in order T1 → T2 → T3; decline-N → all Tier-N `deferred (user-declined-batch)`; user explicit `halt` → stop | per-tier outcome |
| Pre-flight baseline | N18 inline | Baseline captured before any fix (test counts, type errors, lint warnings, build status — pre-existing failures are recorded as the project's baseline state, NOT a halt condition); branch creation | `halt-on-baseline-failure` (only when runner crashes / cannot produce any output), `halt-on-test-cmd-unknown`, `halt-on-git-state-incompatible` |
| Per-fix verify | N20 inline | Targeted tests + type check on changed files; PASS → commit; FAIL → atomic-loop rollback + E_repair routing | After cap-hit on a group: mark group `failed`; downstream groups in topo-sort blocked → `deferred (upstream-dependency-failed)`; independent groups continue. `halt-mid-fix-on-perfix-cap-hit` only if every remaining group is blocked. |
| Regression battery | N21 inline | Full test suite vs baseline; type check; lint (`new_warnings_in_changed_regions == 0`); build clean; **diff-scope check** (every diff line maps to AUDIT-ID; regression-prevention test additions count as in-scope) | Test/type/lint/build fail → E_repair; scope-creep → `halt-on-scope-creep` (do NOT auto-revert) |
| Audit-rerun delta | N21 (tiered; subagent under `--deep` when full rerun) | **Tiered by max applied tier (per §2.5):** Tier-1-only → SKIP audit-rerun; Tier-2-only → narrow rerun (only fix-dimension analyzers); Tier-3 present → full N01..N13 rerun. Classify new findings — touched-files → `induced-regression` (route to E_rerun_fail); untouched-files → `new-finding-discovered` (record only). | `halt-mid-fix-on-induced-regression-cap-hit` after retry cap exhaustion (only reachable when audit-rerun ran) |

### 5.2 Fix safeguard pipeline (10-step canonical sequence)

| Step | Node | MUST-do |
|---|---|---|
| 1. Ingest | N16 | F-VAL schema validation; SHA-256 capture; idempotency check (state file > git-log fallback); conflicting-edits scan; suspicious-content prompt (overrides `--auto`) |
| 2. Triage | N16 | Group by file/module; topo-sort; tier 1/2/3; defer-on-uncertainty |
| 3. Plan | N17 | Emit fix-plan doc; await per-tier approval (default policy); `--dry-run` halts here |
| 4. Pre-flight | N18 | Capture baseline (tests/types/lint/build); create branch; halt on baseline/test-cmd/git-state failures |
| 5. Apply (atomic loop) | N19 | Per fix-group: apply edit (working tree only) → invoke N20 → commit `[AUDIT-NNN] <one-line>` (finding-id in commit body) on PASS, `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` on FAIL (no commit ever made for failed attempts; never `git revert`). **One concern per commit. No bundling.** Recovery manifest updated at fix-group boundaries only (start, end-success, end-failure) per §5.3 and O3. |
| 6. Per-fix verify | N20 | Targeted tests + type check on changed files; embedded inside step 5 atomic loop |
| 7. Regression battery | N21 (battery sub-step) | Full test suite + type check + lint + build clean + diff-scope check. Runs **before** audit-rerun — if the battery fails, the audit-rerun is not worth running. |
| 8. Audit-rerun (tiered, per §2.5) | N21 (audit-rerun sub-step) | Tier-1-only run → skip; Tier-2-only run → narrow rerun (fix-dimension analyzers only); Tier-3 present → full N01..N13 rerun. Classify new findings (touched vs. untouched files). Runs **after** regression battery — only proceeds if battery passes. |
| 9. Regression prevention | N19 (deferred to commit) | For behavioral fixes (not cosmetic): add a test that would have caught the original bug, in same commit (or paired follow-up if language requires separation). Test goes into per-fix verify; a failing test causes working-tree discard (no commit is made). If no test framework detected → `regression_test_added: no (no test framework detected — manual test recommended)`, do NOT block. |
| 10. Reporting | N23 | Closing-the-loop fix report per Schema v1; status-priority sort |

### 5.3 Atomic execution loop (per fix-group, inside step 5)

**Critical primitive choice:** the loop **does not commit until verification passes**. This means a verification failure leaves zero history artifacts — no failed commit, no revert commit. Use `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` on failure, NOT `git revert HEAD`.

**Commit-tag mapping rule:** the commit tag `[AUDIT-NNN]` derives from the finding's `id` field by stripping the `F` prefix and zero-padding the numeric portion to 3 digits. Examples: `F001` → `[AUDIT-001]`, `F042` → `[AUDIT-042]`, `F100` → `[AUDIT-100]`. Induced-regression findings (`Fnew001`) use `[AUDIT-NEW-001]` to disambiguate them from primary findings.

**Commit message body template:**

```
[AUDIT-NNN] <one-line summary, ≤72 chars>

Finding-id: F00N
Dimensions: <comma-separated, e.g., CORRECTNESS, MAINTAINABILITY>
Severity: <CRITICAL|HIGH|MEDIUM|LOW|INFO>
Source: <basename of source audit report>
```

Regression-prevention test additions (when present) are included in the same commit as the fix — no separate body line is required (the `Finding-id` line scopes both fix and test).

**Recovery-manifest writes happen at fix-group boundaries only**, not on every intra-loop transition. The atomic loop runs to completion (success or cap-hit) before persisting state.

```
[fix-group START]                                     (manifest write #1: in_flight_finding_id set,
                                                                          pending list updated)

  inner loop (no manifest writes in here):
    1. Apply edit (working tree only — no commit yet)
    2. Run targeted tests (file/module level)         ──┐
    3. Type check on changed files                    ──┤  steps 2-3 = N20 PerFixVerifier
    4. PASS  → git add <touched-files>
              + git commit -m "[AUDIT-NNN] <one-line>"
              (commit body includes finding-id; regression-prevention test included if behavioral fix)
              → exit inner loop, fall through to fix-group END (success path)
       FAIL → git checkout -- <tracked-touched-files>   (restores edits to pre-existing tracked files)
              + git clean -fd <new-files-created-by-this-attempt>  (removes untracked new files, e.g. regression-prevention test)
              (combined: working tree fully restored; no commit ever made)
              → record failure_context (failure_class + diagnostic from N20)
              → E_repair routing:
                  1st E_repair invocation → retry inner loop (with failure-context)
                  2nd E_repair invocation → N17 replan, then retry inner loop
                  3rd E_repair invocation → cap-hit; exit inner loop, fall through to fix-group END (failure path)

[fix-group END — success path]                        (manifest write #2: last_known_good_sha = new commit;
                                                                          finding moved from pending → applied)

[fix-group END — failure path]                        (manifest write #2: finding marked failed;
                                                                          downstream-dependent groups marked
                                                                          deferred (upstream-dependency-failed);
                                                                          independent groups continue)
```

**Resume semantics on mid-flight death:** if the process dies *between* fix-group boundaries (somewhere inside the inner loop), the recovery manifest's last write was at the most recent fix-group START. On resume, the resume-handler sees `in_flight_finding_id` set without a corresponding `applied` or `failed` entry → it treats the in-flight fix-group as `failed-mid-flight` and **restarts that fix-group from scratch**. The atomic loop's git-checkout discipline guarantees no half-applied state in the working tree (any in-flight working-tree edits are orphaned and discarded by the resume's pre-flight cleanup).

**Pre-commit hook failures** are routed through the same rollback path: if `git commit` fails because a hook (project-level pre-commit, formatter, linter) rejected the commit, N20 reports `failure_class: commit-hook-failure` and the working tree is restored with `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>`. The skill **never** passes `--no-verify` to bypass hooks (per §5.5 hard rules).

**One concern per commit. No bundling. No "while I'm here" cleanups.**

### 5.4 Tier + autonomy policy

| Tier | Definition | Default behavior | `--auto` | `--confirm-all` |
|---|---|---|---|---|
| 1 (mechanical) | Typo, missing import, dead code with no callers | Batch-confirm: *"apply N Tier-1 fixes? y/n"* | Silent apply | Per-fix confirm |
| 2 (local logic) | Single-function changes with bounded blast radius | Batch-confirm: *"apply M Tier-2 fixes? y/n"* | Batch-confirm | Per-fix confirm |
| 3 (cross-cutting) | Schema changes, public-API edits, multi-file refactors | Per-fix confirm | Per-fix confirm | Per-fix confirm |

**`--dry-run`:** no apply for any tier; emit plan + diffs only.

**Per-fix-opt-in floor (anti-conformity):** even under `--auto`, any individual fix where `confidence < HIGH OR effort > trivial` is NOT auto-applied — emitted as recommendation requiring per-fix opt-in. Only HIGH-confidence trivial-effort fixes (typo, missing import, dead code with no callers) auto-apply under `--auto`.

**`--escalate-finding F00N`** forces a specific finding to Tier-3 regardless of N16 classification. Overrides `--auto` for the named finding. `--demote-finding` is NOT supported (`halt-on-flag-rejection`; user must edit the report).

**Tier decline behavior:** each tier presented independently in order T1 → T2 → T3; decline on Tier-N → all Tier-N findings marked `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit user `halt` → stop entirely.

### 5.5 Hard rules (audit + fix)

**Audit:**

- Every finding has all mandatory schema fields (per §4.1 finding body — location, severity, confidence, effort, remediation, evidence_excerpt, evidence_excerpt_extended, false_positive_check, priority_score, provenance, and others). Findings missing any mandatory field → demoted to "Unverified Hypotheses".
- Every `file:line` is verified against the actual file via Read at audit time. No hallucinated lines.
- Every CRITICAL/HIGH finding has Confidence ≥ MEDIUM. HIGH-severity at LOW-confidence → demote in severity OR upgrade in confidence with stated evidence.
- LOW-confidence findings include `verify_by: <what would lift confidence>`.
- Duplicate patterns merged with count.
- Q-GATE Pass A no-comment-echo: no finding text quotes the project's own TODO/FIXME without independent verification of the underlying defect.

**Fix:**

- DO NOT apply fixes outside source tree.
- DO NOT modify files audit didn't flag, **except for regression-prevention test additions per §5.2 step 9** (test files containing only added test cases that exercise an audit-flagged failure mode, in the same commit as the fix). Modifying existing tests' assertions or fixtures still requires the test file to have been audit-flagged.
- DO NOT skip post-fix verification.
- DO NOT batch-apply fixes spanning the same file without staged review.
- DO NOT continue after verification failure without explicit user authorization (or per E_repair bounded retry).
- Never expand scope beyond audit findings. Spotted unrelated bug → log it as a new finding, don't fix it now.
- Never bypass safety checks (`--no-verify`, `--force-push`, hook skipping).
- Never amend prior commits — always new commits, even on retry.
- Defer over guess — if root cause is unclear, mark `deferred` with a question.
- **Idempotent** — re-runs skip already-applied findings (state file > git-log fallback).
- **Fail-loud on partial state** — recovery manifest written at fix-group boundaries (start, end-success, end-failure per O3); mid-flight death leaves a coherent file at rest because (a) the atomic loop never commits before verification passes, so no half-applied state exists in git history, and (b) the resume-handler restarts any in-flight fix-group from scratch with a `git checkout -- . && git clean -fd` pre-flight cleanup.

**Anti-conformity — prior-fix temporal check:** at `--fix` mode entry, inspect (1) git log for `[AUDIT-NNN]` commit-message tags and (2) state files in `~/docs/epiphany/audit/.state/` matching `file:line` or finding-id from prior `epiphany-audit` runs. **Fast-path:** the check runs once at entry with a single git-log scan + state-file directory listing; if zero matches found, the check is silent and the pipeline proceeds without per-finding overhead. Only when matches are found does the per-fix user-override prompt fire. On match: emit warning *"this finding has been addressed before — re-applying may revert intentional behavior"*; require explicit user override per such fix.

---

## 6. Edge Cases, Anti-Patterns, Worked Examples, Production-Grade Definition

### 6.1 Edge cases

**Implied-context resolution:**

- Nested git repos, multi-project session, polyglot inconclusive → list candidates, ask user (no silent default).
- Resolved root is `$HOME` / wrapper repo / denylist match → `halt-suspicious-target`.

**R-ROUTE:**

- Heterogeneous monorepo → run dimensions per sub-tree; emit per-subtree maps in `subtrees: [...]` frontmatter; top-level `dimensions_activated` is the union. **Subtree count cap (cost-aware):** if R-ROUTE detects >10 distinct project-shaped subtrees, group the remaining (lowest-priority by file count) into a single synthetic `rest-of-repo` subtree with the union of detected triggers; emit warning in frontmatter `subtree_grouping_applied: true`. User can override with `--monorepo-subtree-limit N` to raise/lower the cap.
- Binary-only repo → `halt-no-source-detected`: *"no source detected, nothing to audit"*.
- Unknown language → fall through to language-agnostic dimensions only; emit *"language unidentified — running language-agnostic subset only"*.
- Floor (CORRECTNESS + MAINTAINABILITY) always emits.

**Fix-mode:**

- Input report from a different `tool_version` → F-VAL emits version-skew warning, prompts before proceeding (`halt-on-mismatched-version` if declined).
- Input report references files that no longer exist in `audit_target` → `skipped (stale-reference)`, do NOT abort whole run.
- Input report references files outside `audit_target` git tree → halt; refuse to write outside source tree.
- Two findings propose conflicting edits to same `file:line` → `halt-on-conflicting-fixes` in live mode; under `--dry-run` → recorded in `triage_summary.conflicting_groups` without halting (plan serves as the conflict report for the user to resolve before a live run).
- Recovery manifest detected at run start → `halt-on-recovery-conflict`; user choice:
    - `resume` — pick up from `last_known_good_sha`; skip applied; continue with pending list. **In-flight-fix-group restart semantics (per O3):** if the manifest shows `in_flight_finding_id` set without a corresponding `applied` or `failed` entry (i.e., process died inside the atomic loop between fix-group boundaries), the resume-handler runs a pre-flight cleanup (`git checkout -- . && git clean -fd` to discard both orphaned working-tree edits and any untracked new files created by the prior attempt) and **restarts that fix-group from scratch** as fix-group iteration #1. The `in_flight_finding_id` is moved back to `pending` in the manifest. No half-applied state is possible because the atomic loop never commits before verification passes.
    - `fresh` — archive existing manifest to `.recovery/.archive/<report-id>-superseded-<ISO-timestamp>.json` (preserve forensic record, do NOT overwrite); start over.
    - `abort` — halt; no changes.
- Idempotency on re-run → state file authoritative; git-log fallback. Conflict (state says applied at sha not in current branch — possibly because of branch deletion, history rewrite, or squash merge) → emit prompt: *"State file claims F00N applied at sha &lt;X&gt; but commit not found in current branch. Options: (a) re-apply [override state], (b) skip [trust state], (c) abort. Choice?"* — answer required per affected finding; no auto-default. **Per-choice state-file outcomes:**
    - `(a) re-apply` — the new commit's sha **replaces** the prior `<X>` in the state file (single sha per finding-id; old sha lost). Add metadata `previous_sha_unreachable: <X>` to the entry for forensic trail. The fix proceeds through the normal atomic loop.
    - `(b) skip` — the state file's `<X>` is **annotated** in-place with `reachable: false, last_checked: <ISO-timestamp>` so subsequent runs do NOT re-prompt for the same finding. The user can force re-verification by passing `--reverify-state` on a future run (which clears these annotations and re-checks).
    - `(c) abort` — state file untouched; halt. Implies the user will reconcile branch state externally before re-running.
- Scope-creep at N21 → `halt-on-scope-creep` + per-hunk listing; user choice = authorize (recorded as manual edit in fix report) OR revert manually.
- Fix-induced regression at N21 → route through E_rerun_fail back to N16 with regression context; if retry cap exhausted → `failed (induced-regression: <new-finding-id>)`, new finding `deferred (caused-by: <originating-finding-id>)`.
- Tier override → `--escalate-finding F00N` forces Tier-3 for one specific finding; overrides `--auto` for that finding.
- Test command unknown → `halt-on-test-cmd-unknown`.
- Behavioral fix without test framework → `applied` with `regression_test_added: no (no test framework detected — manual test recommended)`; do NOT block.
- B-FIND prompt invalid input under `--deep` → loop with format *"include gap dimension <name>? (y/n/skip-all)"*; ctrl-C → `halt-on-user-abort`.
- Git-state-incompatible at N18 → `halt-on-git-state-incompatible`.
- F-VAL suspicious content (conflicting fixes) → halt-and-prompt regardless of `--auto`. (Zero-main-body-findings and all-Unverified-Hypotheses cases are caught by N16's early halt before suspicious-content checks run — see N16 description §2.)

**`--deep` / `--verbose`:**

- `--verbose` adds depth where it improves actionability; never adds nitpick padding.
- **`--improve` with token-cap partial audit:** if `token_cap_partial: true` AND `--improve` is set, N24..N27 still run (the improvement analysis may still yield useful candidates from the partial findings). N27 sets `improvement_partial: true` in the improvement report frontmatter and prepends the warning: *"source audit was token-capped at `<truncated_at_node>`; improvement analysis is based on incomplete findings and may miss opportunities in unanalyzed dimensions."* When `improvement_partial` is caused instead by N24..N26 pipeline failure, the warning text is: *"improvement analysis failed partway through — output may be incomplete. Check the event log at `~/docs/epiphany/audit/.logs/<report-id>.jsonl` for details."* Both causes set `improvement_partial: true`; the warning text is how the implementer and user distinguish the reason.
- `--deep` lifts coverage and spawn budget but is bounded by checkpoints at two scales:
    - **Inter-node checkpoint (80k aggregate cap):** between dimension analyzers (N04..N09 transitions) and before N14. Cap-trigger → emit partial report with `token_cap_partial: true`, `truncated_at_node: <id>`, `q_gate.pass_a: pass-minimal`, `q_gate.pass_b: skipped-token-cap`. Findings that fail minimal Pass A → Unverified Hypotheses.
    - **Intra-node soft budget (30k per analyzer):** inside a single dimension analyzer (e.g., N07 SECURITY on a large codebase), if accumulated context for that node exceeds 30k tokens, the analyzer aborts gracefully. Findings produced so far are kept. The dimension is recorded in the audit report frontmatter under `dimensions_partial` (per §4.1 schema: array of `{dimension, reason}` objects) with reason `"intra-node soft budget exceeded — coverage incomplete for dimension X"`. The aggregate inter-node check still applies; the intra-node abort prevents one dimension from monopolizing the budget.

### 6.2 Anti-patterns the skill MUST NOT exhibit

**Audit findings:**

- Stylistic preferences disguised as bugs (*"could use `auto` here"*).
- Findings without reading the actual code (no hallucinated `file:line`).
- Generic advice that applies to any project (*"add more tests"*).
- Refactors with no concrete defect or measured cost.
- Duplicate findings — collapse into one with count.
- Wall of LOW-severity nitpicks burying real defects.
- Rewrites without a concrete defect driving them.
- Echoing project's own TODO/FIXME comments (covered by Q-GATE Pass A check `no-comment-echo`).
- Conflating *"I would have written it differently"* with *"this is wrong"*.
- Reporting findings the existing tests already cover. Concrete heuristic: at finding time, grep the project's test directory for the involved function/class/module. If a matching test exists → set `tests_present_signal: true`, require Confidence ≥ MEDIUM (the bar elevates; the finding may still pass if the test demonstrably doesn't cover the failure path).

**Fix application:**

- Apply fixes outside source tree.
- Modify files audit didn't flag (except regression-prevention test additions in the same commit as the fix, per §5.5 carve-out).
- Skip post-fix verification.
- Batch-apply fixes spanning the same file without staged review.
- Continue after verification failure without authorization (or per E_repair bounded retry).
- Expand scope beyond audit findings.
- Bypass safety checks.
- Amend prior commits.
- Guess root cause when unclear.
- Silent re-application of already-applied findings.
- Silent partial state on mid-flight death.

### 6.3 Worked examples (must appear in `SKILL.md`)

**Example 1 — Worked Finding (full mandatory fields, plausible Python off-by-one):**

```yaml
## Finding F001
id: F001
location: src/parser.py:142
dimensions: [CORRECTNESS]
severity: HIGH
confidence: HIGH
evidence_excerpt: |
  for i in range(len(tokens) - 1):
      emit(tokens[i])
  # final token never emitted
rationale: loop bound drops final token; downstream consumer expects all N tokens
remediation: |
  -    for i in range(len(tokens) - 1):
  +    for i in range(len(tokens)):
effort: trivial
evidence_excerpt_extended: false
false_positive_check:
  intentional:           { value: false, justification: "no test or comment justifies the -1" }
  file_symbol_verified:  { value: true,  justification: "Read at src/parser.py:140-145" }
  reachable_from_entry:  { value: true,  justification: "called by parse_input in main.py:23" }
  fix_breaks_dependents: { value: false, justification: "grep shows no caller relies on N-1 emission" }
priority_score: 9.0   # (severity 3 × confidence 3) / effort 1
tests_present_signal: false
provenance:
  node: N04
  mode: inline
  model: claude-sonnet-4-6
  prompt_hash: a3f9e2c1d4b8f7e0a1b2c3d4e5f60718
  plugin_name: null
  plugin_version: null
  audit_rerun_iteration: 0
  q_gate_pass_b_demoted: false
```

**Example 2 — Worked Node Registry row (`graph.json` fragment, conforms to `graph.schema.json`):**

```json
{
  "id": "N02",
  "name": "RelevanceRouter",
  "type": "router",
  "mode": "inline",
  "active_in": "both",
  "inputs": ["project_model from N01"],
  "outputs": ["dimension_activation_map", "skip_reasons", "subtrees (optional)"],
  "aggregation_policy": "n/a",
  "halt_conditions": ["language detection inconclusive in polyglot mode → halt-ambiguous-target"]
}
```

**Example 3 — Worked improvement entry (full mandatory fields, post-OEF survivor):**

```yaml
## Improvement I002

id: I002
category: quick-win
area: testing
utility_score: 2               # reason: test-discovery failures currently surface only at runtime
cost_score: 1                  # trivial: one pytest.ini / conftest.py edit
description: |
  The project uses dynamic test discovery but has no conftest.py at the repo root.
  Failures in fixture setup are silently swallowed on Python < 3.11, meaning a
  broken fixture causes zero tests to run rather than N failures — masking breakage.
action: |
  Add a minimal conftest.py at the repo root with a session-scoped fixture guard:
  assert sys.version_info >= (3, 10), "test suite requires Python 3.10+"
  This ensures fixture failures surface as explicit errors, not silent zero-runs.
success_measure: |
  Running pytest with a broken fixture produces a visible ERROR line in output
  rather than "collected 0 items".
```

**Example of a correctly-discarded candidate (appears only under `--verbose`):**

```
Discarded: "Migrate to async/await throughout the service layer"
  utility_score: 2 (notable latency improvement in theory)
  cost_score: 3 (significant: touches 14 files, requires test harness changes)
  OEF rationale: cost > utility (2 < 3); also description contains speculative language
                 ("in theory" — no profiling evidence of latency being the bottleneck)
```

**Example 4 — Worked dimension-routing decision:**

```
PERFORMANCE:     skipped — no hot loops detected, no perf-critical heuristic match
SECURITY:        activated for [shell-injection, secrets-in-source];
                 skipped sub-surfaces [SQL] — no DB layer detected
CORRECTNESS:     activated (floor)
MAINTAINABILITY: activated (floor)
```

### 6.4 "Production-grade" — operationalized definition

The skill is *production-grade* iff:

1. **Deterministic routing** — R-ROUTE outputs the same activation map for the same project state (`project_model` from N01). Finding generation is **substantively-stable**: re-runs on identical code target ≥80% set-overlap. Verified via determinism fixture at `tests/determinism/<lang>-<size>/` containing reference projects (one per language × size cell — e.g., `python-small/`, `ts-large/`) with frozen expected-finding sets in `expected_findings.yaml`. Re-run set-overlap below 80% on a fixture → CI failure. **Provenance-aware diff:** when set-overlap drops, the determinism harness inspects the `provenance` field (I2) to distinguish *content drift* (same node + same model + same prompt_hash, different findings — real probabilistic noise) from *infrastructure drift* (model upgrade, prompt edit, plugin version bump — flagged separately so it isn't counted against the 80% threshold). Strict identity is not claimed because LLM-driven analyzers are probabilistic.
2. **Idempotent re-runs** — re-audit produces same findings modulo intentional code changes; `--fix` re-run on same report skips already-applied findings (state file authoritative; git-log fallback).
3. **Schema-versioned output** — `schema_version: 1` mandatory in all report types (audit, fix, dry-run plan, improvement); F-VAL strict validation on `--fix` ingest.
4. **Halt envelope on every halt state** — structured `{halt_state, subreason, diagnostic}` at the top of the user-facing message; downstream tooling parses the envelope.
5. **Structured per-node event log** — every node emits a single-line JSON event on entry/exit (`{"node": "N02", "phase": "exit", "decision": "..."}`) to `~/docs/epiphany/audit/.logs/<report-id>.jsonl`. **Stdout shows only user-facing milestones** (gate-passed, finding-being-verified, halt-state); the JSONL log carries the full trace.

---

## 7. Implementation Guidance for the Consumer

The consumer of this spec produces:

1. `SKILL.md` covering §1–§6 in human-readable form, with all 13 Layer-A sections from the source prompt populated. **Layer-A section list** (per the source prompt at `~/docs/epiphany/prompts/27-04-epiphany-audit-skill-design.md`): (1) Purpose & scope, (2) Invocation contract, (3) Implied-context resolution, (4) Node registry overview, (5) Mode flowcharts (audit / fix / improve), (6) Halt-state envelope format, (7) Report schema cross-reference, (8) Verification-gate ordering, (9) Tier + autonomy policy, (10) Recovery semantics, (11) Hard rules (audit + fix), (12) Anti-patterns, (13) Worked examples. Each section is a markdown H2 in `SKILL.md`. If the source prompt is later edited, this enumerated list is the canonical reference for the consumer; do NOT re-derive from the source prompt.
2. `graph.json` declaring all 27 nodes + edges per §2 + §3, validated by `graph.schema.json`. Improvement subpipeline nodes (N24..N27) must carry `"active_in": "improve"` to signal that they activate only under `--improve`. The `graph.json` `active_in` field is a **single string** (one of five values): `"audit"` = audit-only mode; `"fix"` = fix-only mode; `"both"` = both audit and fix (including the audit-rerun sub-step inside N21); `"improve"` = `--improve` subpipeline only; `"always"` = all modes.
3. `modules/N01..N27.md` — one file per node, each following the Layer-B contract template:
    - **Inputs (typed)** — prose-with-shape annotations (e.g., `project_model: { language: string, files: list[Path], build_manifest: Path | null, test_command: string | null, git_state: { head: sha, dirty: bool, ... } }`). JSON-Schema-style optional but not required; the goal is unambiguous handoff between nodes, not full machine validation of every field.
    - **Outputs (typed)** — same shape conventions as inputs.
    - **Side effects** (none / read-only / git-staged / write-state-file / write-recovery-manifest / write-baseline / write-log / write-report / network)
        - Analyzer nodes (N04..N09): `none` or `read-only`.
        - **N15 SaveHandler:** `write-report` (audit report) + `write-state-file` (idempotency state, only on save-accept) + `write-log`.
        - **N18 PreFlight:** `write-baseline` (baseline metrics file) + `git-staged` (branch creation only — no edits) + `write-log`. **Does NOT write recovery manifest.**
        - **N19 FixApplier:** `git-staged` (working-tree edits + commits on PASS; `git checkout -- <tracked-touched-files>` + `git clean -fd <new-files-created-by-this-attempt>` on FAIL — never `git revert`) + `write-recovery-manifest` (boundary-aligned writes only — at fix-group start, end-success, end-failure; per O3) + `write-log`.
        - **N22 RollbackHandler:** `write-recovery-manifest` (finalize on E_repair cap-hit and on planned termination; mid-flight death is handled by the most-recent boundary write from N19; no `git revert HEAD` — failed fix attempts never produce commits) + `write-log`.
        - **N23 FixReporter:** `write-report` (fix report) + `write-log`.
        - **N24 ImprovementContextualizer, N25 ImprovementBrainstormer, N26 OEF:** `read-only` + `write-log`. These nodes read from in-memory context only; they do not open additional files.
        - **N27 ImprovementReporter:** `write-report` (improvement report, written unconditionally — no save prompt) + `write-log` + conditional in-place patch of `improvement_report_ref` in the already-saved audit report frontmatter (only when N15 save was accepted). The patch MUST be an in-place YAML frontmatter line update; it MUST NOT rewrite the audit report body. **Backpatch failure handling:** if the patch fails (e.g., permission error, file modified since save), N27 logs the absolute path of the improvement report to the event log and emits a user-facing warning — it does NOT retry or halt the skill.
        - All other nodes: `read-only` or `none`, plus `write-log` for structured event emission.
    - **Halt conditions** (mapping to halt states in §3.3)
    - **Token budget hint** (per-node soft budget; analyzer nodes 30k under `--deep` per §6.1 intra-node cap)
    - **Backtrack/aggregation participation**
    - **Fan-out cardinality** (N04..N09 under `--deep`; N10 across findings)
    - **Back-edge endpoints** (N20 fail-signal → E_repair → N17/N19; N21 induced-regression → E_rerun_fail → N16)
4. `schemas/audit-report-v1.schema.json`, `schemas/fix-report-v1.schema.json`, `schemas/dry-run-plan-v1.schema.json`, `schemas/dimension-plugin-v1.schema.json`, `schemas/improvement-report-v1.schema.json` — authoritative JSON schemas matching §4 and §2.4.
5. `templates/audit-report.md.template`, `templates/fix-report.md.template`, `templates/dry-run-plan.md.template`, `templates/improvement-report.md.template`.
6. `dimensions/` — built-in dimension plugins per §2.4 (`correctness.md`, `architecture.md`, `performance.md`, `security.md`, `maintainability.md`); these mirror the prompts in `modules/N04..N08.md` but conform to the dimension-plugin-v1 schema so R-ROUTE can load them through the same code path as user plugins.
7. `tests/smoke/` — minimal end-to-end smoke runs covering: default invocation, `--audit` only, `--fix` on a known-good report, `--dry-run` (verifies dry-run plan emission + schema), recovery-manifest resume, suspicious-target halt, target-conflict halt, dimension-plugin loading (a fixture user plugin in `tests/smoke/fixtures/user-dimensions/`), `--improve` mode (verifies improvement report emission + schema + `improvement_report_ref` patch into audit report), `--improve` zero-survivors (verifies valid empty result is reported correctly), `--fix --improve` warning (verifies improvement pipeline is skipped and warning is emitted without halting).
8. `tests/schema-validation/` — JSON-schema validation tests for all five schemas.
9. `tests/determinism/` — fixture root for the ≥80% set-overlap CI gate (per §6.4).

**CI policy guidance:** the determinism harness is **expensive** (runs the full audit pipeline against reference projects). Recommend running it as a **scheduled nightly/weekly workflow**, not on every commit. Per-commit CI should run only `tests/schema-validation/` (cheap, deterministic) and `tests/smoke/` (fast end-to-end). The determinism harness gates the next release tag, not the next merge.

**Shared location-verification cache contract** (O4 — referenced by N10 and N14 Pass A):

- Cache lives in process memory for the lifetime of one **skill invocation**; cleared between invocations. (In `--fix` mode, the cache is empty at start and populates only during the N21 audit-rerun sub-step — when N10 FPV and N14 Pass A re-run on the fixed code. In default/`--audit` mode, it populates during the initial audit and is discarded after N15 SaveHandler.)
- Key: `(canonical_file_path, line_range)` where `line_range` is normalized as `(start, end)` even for single lines (`(142, 142)`).
- Value: `{ verified: bool, content_hash: <sha256 of bytes read>, populated_by: <node-id>, populated_at: <ISO 8601> }`.
- N10 FPV is the **only writer**; populates the cache the first time it Reads a `(file, line)` for false-positive verification.
- N14 Pass A is a **read-only consumer**; on cache miss, falls back to its own Read (rare — only for findings demoted to `Unverified Hypotheses` by N10 that Pass A still wants to verify).
- Cache write happens *after* a successful Read; failed Reads (file deleted, permission denied) are recorded as `verified: false` so consumers see the failure without re-attempting.
- Cache is **not persisted** to disk; structured event log records cache-miss events for telemetry only.

The consumer does **not** invoke `writing-plans` or `executing-plans`; the implementation plan for this spec is produced separately by the brainstorming-skill caller after spec approval.

---

## 8. Source Material

- Original input prompt: `~/docs/epiphany/prompts/27-04-epiphany-audit-skill-design.md`
- Brainstorm critiques resolved (10): node-count compression, default-confirm-tier-batch flipped, write-serial/read-parallel, R-ROUTE floor, state-file idempotency, per-node inline/subagent annotation, suspicious-target gate, B-FIND non-interactive default, token-cap checkpoints, file-layout choice (`epiphany-graph-genius` style).
- Self-audit fixes applied across all 6 design sections: 7 + 13 + 14 + 13 + 13 = 60 corrections.
- Post-session addition: `--improve` flag + N24-N27 improvement subpipeline (ImprovementContextualizer, ImprovementBrainstormer, OverEngineeringFilter, ImprovementReporter) + E16-E20 edges + §4.5 Improvement Report Schema v1 + `improvement_report_ref` audit-report field + `improvement-report-v1.schema.json`.
- Full multi-dimensional audit pass (20 bugs fixed): effort field added to finding schema; Example 1 completed with provenance + evidence_excerpt_extended; §5.2 steps 7/8 ordering corrected (battery before audit-rerun); N24 source node corrected (N14 not N13); OEF utility=1 filter gap closed; --escalate-finding scope fixed to include no-flag mode; §5.4 field names corrected (confidence/effort); §5.5 mandatory-fields language updated; E14 correctly attributed to N19 discard; E20 / N27 save semantics clarified (unconditional); N27 backpatch failure handling specified; audit_rerun_delta.scope null comment expanded; improvement report flags constraint documented; --improve + token-cap edge case added; §6.4 schema-versioning updated; five schemas in §7; zero-survivor smoke test added.
