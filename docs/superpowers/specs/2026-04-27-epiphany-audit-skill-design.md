# epiphany-audit — Skill Design Specification

- **Status:** design — awaiting implementation
- **Date:** 2026-04-27
- **Author:** brainstorming session (critical-mode) on top of `~/docs/epiphany/prompts/27-04-epiphany-audit-skill-design.md`
- **Family:** epiphany-*
- **Implementer audience:** Claude Code agent (general-purpose / prompt-architect subagent type) producing `SKILL.md`, `graph.json`, `modules/N01..N23.md`, `schemas/*.json`, `templates/*`, and `tests/`.
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
/epiphany-audit [<path>] [--audit | --fix <report>] [--verbose] [--deep]
                [--auto | --confirm-all | --dry-run]
                [--escalate-finding F00N] [--test-cmd '<cmd>']
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

- `--escalate-finding F00N` — force a specific finding to Tier-3 (per-fix confirm) regardless of N16 classification. Valid only with `--fix`; ignored with warning otherwise. Overrides `--auto` for the named finding.
- `--demote-finding F00N` — **NOT supported.** Passing it → `halt-on-flag-rejection`: *"--demote-finding is not supported. Edit the report manually to change tier classification."*
- `--test-cmd '<cmd>'` — explicit project test command. Always overrides auto-detection.

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
│   └── N23-fix-reporter.md
├── schemas/
│   ├── audit-report-v1.schema.json
│   └── fix-report-v1.schema.json
├── templates/
│   ├── audit-report.md.template
│   └── fix-report.md.template
└── tests/
    ├── smoke/
    └── schema-validation/
```

**External paths used at runtime:**

- `~/docs/epiphany/audit/` — saved audit reports
- `~/docs/epiphany/audit/fix-reports/` — saved fix reports
- `~/docs/epiphany/audit/.state/<source-report-id>.json` — idempotency state per source report
- `~/docs/epiphany/audit/.recovery/<report-id>.json` — recovery manifest (in-flight fix state)
- `~/docs/epiphany/audit/.recovery/.archive/<report-id>-<timestamp>.json` — archived prior recovery manifests
- `~/docs/epiphany/audit/.logs/<report-id>.jsonl` — structured per-node event log (not stdout)

---

## 2. Node Registry (23 nodes, compressed from input's 26)

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
| N02 | RelevanceRouter (R-ROUTE) | router | inline | Per-dimension activation map. **Floor:** CORRECTNESS + MAINTAINABILITY always on. Skipped dimensions emit `skipped because: <trigger>` rationale. Heterogeneous monorepo emits per-subtree activation maps. | — |
| N03 | BlindspotFinder (B-FIND) | meta-analyzer | inline | Project-type-specific gap dimensions. **Default mode:** auto-add HIGH-confidence gaps; list in `gap_dimensions_auto_added`. **`--deep` mode:** prompt user (`include gap dimension <name>? (y/n/skip-all)`). | — |
| N04 | DimensionAnalyzer.CORRECTNESS | analyzer | inline / subagent under `--deep` (capped) | Logic, types/lifetime, boundaries, races, leaks, error paths. | participant in BACKTRACK via N10 |
| N05 | DimensionAnalyzer.ARCHITECTURE | analyzer | inline / subagent under `--deep` | Coupling, circular deps, god objects, duplicated logic, invariant gaps. Tags `latent` findings with reachability. | participant in BACKTRACK |
| N06 | DimensionAnalyzer.PERFORMANCE | analyzer | inline / subagent under `--deep` | Hot allocations, complexity blowups, cache layout, false sharing. Tags `latent`. | participant in BACKTRACK |
| N07 | DimensionAnalyzer.SECURITY | analyzer | inline / subagent under `--deep` | Per-surface sub-routing (SQL/shell/auth/secrets/deserialization/prompt-injection). | participant in BACKTRACK |
| N08 | DimensionAnalyzer.MAINTAINABILITY | analyzer | inline / subagent under `--deep` | Dead code, misleading names, stale TODOs, test coverage gaps on failure-mode branches. | participant in BACKTRACK |
| N09 | DimensionAnalyzer.\<X\> (template) | analyzer | inline / subagent under `--deep` | **Instantiable** per project-specific dimension (a11y, reproducibility, IaC drift, model-card, schema-evolution, prompt-injection). Concrete instances get IDs `N09.a11y`, `N09.iac-drift`, etc., in `graph.json`. | participant in BACKTRACK |
| N10 | FalsePositiveVerifier (FPV) | verifier | inline | Re-reads source per finding; runs the 4 false-positive questions; demotes/discards. **Owns audit-side BACKTRACKING (single re-emit cap).** | refinement back-edge to N04..N09 |
| N11 | FindingsAggregator | aggregator | inline | Dedup by pattern+location, merge cross-dimension overlaps, count-collapse. **Owns audit-side AGGREGATION.** | aggregator |
| N12 | Prioritizer | scorer | inline | (Severity × Confidence) / Effort. Emits resolve-before-testing punch list. | — |
| N13 | ReportFormatter | formatter | inline | Markdown per Audit Report Schema v1. | — |
| N14 | Q-GATE | verifier | inline (Pass A) + subagent (Pass B) | **Folded from input-prompt N14+N15.** *Pass A* (mechanical, inline): mandatory-field check, location verification (every `file:line` re-Read), CRITICAL/HIGH × Confidence ≥ MEDIUM floor, dup merge, no-comment-echo, no-LOW-only warning. *Pass B* (adversarial, subagent — clean lens): anti-iatrogenic, evidence-rationale coherence, dimension-classification correctness. **Default's only spawn slot.** | adversarial self-review |
| N15 | SaveHandler | io | inline | Offers save under `~/docs/epiphany/audit/`. **Save prompt explicitly warns about idempotency degradation** if user declines: *"declining to save means future `--fix` runs of this report cannot use state-file idempotency; they fall back to git-log only. Save anyway? (y/n)"*. Writes idempotency state file at `~/docs/epiphany/audit/.state/<report-id>.json` only on save-accept. | — |
| N16 | FixTriage (F-VAL ingest + Triage) | validator + triage | inline | **Folded from input-prompt N17+N18.** Schema-validates input report (`schemas/audit-report-v1.schema.json`); halt with precise field/finding-id error on schema fail; SHA-256 of source audit report captured for `source_audit_report_sha256`. **Empty-or-unfixable check (early halt):** if source audit report has zero main-body findings (excluding "Unverified Hypotheses"), halt with `halt-on-empty-or-unfixable-report` *before* running idempotency / triage / suspicious-content checks. **Idempotency check at ingest time** (state file authoritative; git-log fallback). Suspicious-content prompt overrides `--auto`. Groups by file/module; topo-sorts; tier 1/2/3 classification (rules per §2.3); defer-on-uncertainty; conflicting-edit detection (`halt-on-conflicting-fixes`). **Owns fix-side CONDITIONAL ROUTING.** | aggregator (file-grouping) |
| N17 | FixPlanner | planner | inline | Emits fix-plan doc; awaits per-tier batch approval (default policy); Tier-1 silent under `--auto`; per-fix under `--confirm-all`; `--dry-run` halts here with plan + diffs emitted. Each tier presented in order T1 → T2 → T3; decline on Tier-N → all Tier-N findings `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit user `halt` → stop entirely. | — |
| N18 | PreFlight | preflight | inline | Captures baseline manifest (tests/types/lint/build) **before any fix**. Creates branch `epiphany-audit/<report-id>-YYYYMMDD`. **`halt-on-baseline-failure` triggers ONLY when the runner crashes / cannot produce any output** (no exit code, missing binary, broken tool config) — pre-existing failing tests are recorded as the project's baseline state and do NOT halt. Halt on unknown test command / git-state incompatible (dirty tree, detached HEAD, no commits). Writes only the baseline-metrics file under `~/docs/epiphany/audit/.baselines/<report-id>.json`. **Does NOT write recovery manifest** (that is N19/N22 responsibility). | — |
| N19 | FixApplier | actuator | inline (write-serial) | Atomic loop per fix-group (definition in §2.3): **apply edit (working tree only) → invoke N20 → commit `[AUDIT-NNN] <one-line>` (finding-id in commit body) on PASS, or `git checkout -- <touched-files>` on FAIL (no commit ever made for failed attempts).** Critical: **never `git revert HEAD`** — that creates redundant commits that break idempotency grep. **One concern per commit. No bundling. No "while I'm here" cleanups.** Writes/updates recovery manifest at every state transition (`in_flight_state: idle \| applying \| verifying \| committing`; `last_known_good_sha`; `applied`/`pending`/`in_flight_finding_id`). **Regression-prevention test policy: same-commit by default.** Paired follow-up commit allowed only when the language tooling rejects bundled test+source commits OR a project-level pre-commit hook rejects the bundled commit (recorded as `regression_test_added.deferred_to_followup_commit: <sha>` in fix report). Test failure during per-fix verify discards the working-tree changes (no commit). | — |
| N20 | PerFixVerifier | verifier | inline | Per-fix targeted tests + type check on changed files. PASS → commit (in N19). FAIL → emit fail-signal + `failure_class` enum (`verification-failure` \| `commit-hook-failure` \| `git-operation-failure` \| `type-check-failure` \| `targeted-test-failure`); N19 discards working-tree changes; **E_repair edge-layer logic** (not N20) decides retry vs replan vs cap-hit per the rules in §3.1. | emits fail-signal; routing is E_repair's responsibility |
| N21 | RegressionBattery (battery + audit-rerun delta) | verifier | inline (battery) + subagent under `--deep` (audit-rerun clean lens) | **Folded from input-prompt N23+N24.** Battery (always inline): full test suite vs baseline (no new failures), type check (no new errors), lint (`new_warnings_in_changed_regions == 0`), build clean, **diff-scope check** (every diff line maps to AUDIT-ID; regression-prevention test additions count as in-scope via the `[AUDIT-NNN]` commit they ride). **Audit-rerun sub-step: re-run N01..N13 only** (the analytical chain — N14 Q-GATE and N15 SaveHandler are NOT re-executed in fix mode; we do not save a second audit report). Classify new findings — those in **files touched by fixes** → `induced-regression` (route to E_rerun_fail); those in **untouched files** → `new-finding-discovered` (record in fix report body, no E_rerun_fail). | adversarial-via-rerun |
| N22 | RollbackHandler | recovery | inline | `git revert HEAD` on N20/N21 failure inside atomic loop; halt-with-diagnostic on cap-hit; finalizes recovery manifest on planned termination; manifest is at-rest accurate after every state transition (incremental updates from N19). | — |
| N23 | FixReporter | formatter | inline | Closing-the-loop fix report per Fix Report Schema v1: per-finding status, diff summary by AUDIT-ID, baseline-vs-post metrics, audit-rerun delta, deferred items, manual-edits section (if user authorized scope-creep), recovery-manifest reference (if applicable). Status-priority sort: `failed > induced-regression > deferred > verified > skipped`. | aggregator (per-fix outcomes) |

### 2.1 Spawn budget reconciliation

- **Default invocation:** **1 spawn** (N14 Pass B adversarial). Inside spec's ≤2 cap.
- **`--deep` invocation:** up to **3 spawns** — N14 Pass B + optional **analyzer fan-out spawn** + N21 audit-rerun clean-lens.
- **Analyzer fan-out spawn (`--deep` only):** when more than 3 dimension analyzers (N04..N09) are activated AND the project is large (>500 files OR >50k lines), one batched subagent runs the over-cap analyzers in parallel with the inline ones. The subagent receives the project model from N01 and returns a fan-in result to N10. This is a single spawn slot regardless of how many analyzers it covers. Skipped if ≤3 analyzers or project is small.

### 2.2 Mechanism ownership

- BACKTRACKING (audit): N10 (single re-emit cap on N04..N09)
- BACKTRACKING (fix): E_repair from N20/N21 → N17/N19 (max 1 replan per fix-group, max 1 retry per fix; first failure → retry; second → replan; third → cap-hit)
- AGGREGATION (audit): N11
- AGGREGATION (fix): N16 (file-grouping) + N23 (per-fix outcome rollup)
- CONDITIONAL ROUTING: N02 (audit dimensions), N16 (fix tier classification)
- ADVERSARIAL SELF-REVIEW: N14 Pass B (audit), N21 audit-rerun delta (fix)
- PARALLEL VERIFICATION: read-only fan-out in N04..N09 under `--deep`; N10 may fan out across findings. **Write nodes (N19, N20, N22) never fan out.**

### 2.3 Definitions

**Fix-group:** the unit produced by N16's `group-by-file/module` + topo-sort positioning. A fix-group contains 1+ findings sharing a file/module, ordered by dependency. The atomic loop in §5.3 operates on one fix-group at a time. Fix-groups are the unit of E_repair retry-or-replan, the unit of `failed`/`deferred (upstream-dependency-failed)` status assignment, and the unit of recovery-manifest `pending` tracking.

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
| E11 | N14 → N15 | data | 1:1 | only on Q-GATE Pass A AND Pass B both succeed (Pass B subagent exec-error treated as failure → halt) |
| E12 | N15 → user | interactive | 1:1 | "save?" then "fix?" — fires only after N15 (`--fix <report>` mode skips N01..N15 entirely; E12 never fires there) |
| E13 | N16 → N17 → N18 → N19 → N20 → N21 → N23 | data/control chain | 1:1 each | fix pipeline; entry from `--fix` mode or post-`save?-fix?` consent |
| E14 | N20 → N22 | control | 1:1 | on PerFixVerify failure → working-tree discard (no `git revert` — see §5.3) |
| E15 | N20 → N19 | feedback | 1:1 | on PerFixVerify success → next fix-group |
| E_repair | N20 fail OR N21 fail → N17 (replan) OR N19 (retry-with-failure-context) | feedback | 1:1 | bounded per fix-group: **1st E_repair invocation → N19 retry; 2nd → N17 replan; 3rd → cap-hit**, mark group `failed`, continue with independent groups |
| E_rerun_fail | N21 audit-rerun delta detects induced regression → N16 | feedback | 1:1 (batched) | re-triage with regression context; only for new findings in **files touched by fixes**. **Batching rule:** all induced regressions from a single audit-rerun are passed to N16 in one batched re-invocation, not N separate invocations. |
| E_diffscope | N21 diff-scope-check fail → halt | control | 1:0 | scope-creep detected; do NOT auto-revert; user authorize-or-revert |
| E_complete | N23 → user | data | 1:1 | terminal — fix report emitted |

### 3.2 State Machine

| Start | Flag | Save offered? | Fix offered? | Reachable end states |
|---|---|---|---|---|
| invoke | (none) | yes | yes (per-tier batch confirm default) | `saved+fixed`, `saved+not-fixed`, `not-saved+not-fixed-emitted-only` |
| invoke | `--audit` | yes | **no** (explicit) | `saved+audit-only`, `not-saved-emitted-only` |
| invoke | `--fix <report>` | n/a (skip N01..N15) | yes (gated by N16 F-VAL ingest) | `fixed`, `partial-fixed-with-deferred`, `halt-pre-fix-on-validator-failure`, `halt-mid-fix-on-perfix-cap-hit`, `halt-mid-fix-on-induced-regression-cap-hit` |
| invoke | `--verbose` | combines | combines | verbose variants |
| invoke | `--deep` | combines | combines | deep variants; +interactive B-FIND prompt; +token-cap partial-report state |
| invoke | `--auto` | combines (fix-mode) | combines | Tier-1 silent; Tier-2 batch-confirmed; Tier-3 per-fix |
| invoke | `--confirm-all` | combines (fix-mode) | combines | per-fix confirmation regardless of tier |
| invoke | `--dry-run` | combines (fix-mode) | n/a | terminates after N17 with plan + diffs emitted; no apply, no commits, no branch |
| invoke | `--dry-run` without `--fix` and no audit-pending offer | n/a | n/a | warning emitted: *"--dry-run has no effect without --fix or post-audit fix offer"*; ignored, run continues as if not passed |

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
| `halt-on-conflicting-fixes` | N16 | two findings target same `file:line` with incompatible edits |
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

gap_dimensions_offered: [...]      # populated only under --deep (B-FIND interactive)
gap_dimensions_auto_added: [...]   # populated under default mode (HIGH-confidence auto-add)
gap_dimensions_accepted: [...]     # subset of offered the user opted into

token_cap_partial: false
truncated_at_node: null            # which node was active when cap fired (if partial)

dimensions_partial:                # populated only when intra-node soft budget tripped on a dimension (--deep)
  - dimension: SECURITY
    reason: "intra-node soft budget exceeded — coverage incomplete for dimension SECURITY"

q_gate:
  pass_a: pass | pass-minimal | fail | skipped-token-cap
  pass_b: pass | fail | exec-error | skipped-token-cap
  pass_b_lens: <subagent identifier or null>

partial_report_warning: null | "<warning text>"   # rendered at top of body when truncated
```

#### 4.1.1 `flags` field — allowed combinations

`flags` is a mutually-exclusive set (validated by schema):

```
allowed: [audit] | [audit, verbose] | [audit, deep] | [audit, verbose, deep]
       | [fix]   | [fix, verbose]   | [fix, deep]   | [fix, verbose, deep]
       | []      | [verbose]        | [deep]        | [verbose, deep]
       (autonomy/oversight flags appear in fix report, not audit report)
```

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
priority_score: 9.0                  # see §4.1.2
verify_by: null                      # only present when confidence: LOW; what would lift confidence
tests_present_signal: false          # complementary metadata, not a 5th false-positive question.
                                     # Set true when test-dir grep matches the involved fn/class/module.
                                     # Effect: elevates confidence floor — finding may still pass if the
                                     # test demonstrably doesn't cover the failure path, but Confidence < MEDIUM
                                     # is rejected by Q-GATE Pass A when this signal is true.
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

#### 4.1.4 Top-of-body sections

1. **"Resolve-before-testing" punch list:** all CRITICAL + all HIGH findings always; then add MEDIUM until total reaches 15 or all findings exhausted, whichever first. Sorted by `priority_score` descending.
2. **Partial-report warning** (only present when `token_cap_partial: true`).
3. **Main body:** all findings, grouped by dimension or by file (implementer choice; recommend by file for actionability).
4. **"Unverified Hypotheses" section** (below main body, separated): demoted findings that failed false-positive checks. **Not consumable by `--fix`**; advisory only.

### 4.2 Fix Report Schema v1

Saved to `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-<YYYYMMDD>-<HHMMSS>.md`. Authoritative JSON schema: `schemas/fix-report-v1.schema.json`. Emitted only on planned termination (N23). Unplanned termination leaves only the recovery manifest.

**Frontmatter (YAML):**

```yaml
schema_version: 1
fix_report_id: <uuid v4>
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
  resolved:                 [F001, F003, ...]
  fix_induced_regressions:  [Fnew001, ...]
  unchanged:                [F004, ...]
  new_findings_discovered:  [Fnew099, ...]      # untouched-file new findings (not regressions)
                  # null under --dry-run

diff_scope_check: pass | fail-with-unmapped-hunks | n/a   # n/a under --dry-run
unmapped_hunks: []          # only when fail

recovery_manifest_ref: null | <absolute path to .recovery/<report-id>.json>
                  # null on planned-completion runs that did not need recovery; populated on resumed runs.
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

- `source_report_id` in fix-report = `report_id` in audit-report.
- `source_audit_report_sha256` is verified at `--fix` start; halt with `halt-on-stale-source-report` if file missing or hash mismatched.
- Status-priority sort order intentionally surfaces problems first.
- F-VAL strict schema validation applies to **all** `--fix` ingests, including hand-edited audit reports.
- "Unverified Hypotheses" findings are **not consumable** by `--fix`; F-VAL ingests only main-body findings.
- Authoritative schemas are `schemas/audit-report-v1.schema.json` and `schemas/fix-report-v1.schema.json`; this section is the human-readable companion.

---

## 5. Verification Gates + Fix Safeguard Pipeline

### 5.1 Verification gates summary

| Gate | Node | Owner of | Failure consequence |
|---|---|---|---|
| Q-GATE Pass A (mechanical) | N14 inline | Mandatory-field completeness; location verification (every `file:line` re-Read at audit time); CRITICAL/HIGH × Confidence ≥ MEDIUM floor; dup merge; no-comment-echo; no-LOW-only warning; under token-cap, runs as `pass-a-minimal` (mandatory-field + location only) | `halt-on-q-gate-failure` (subreason: `pass-a`) |
| Q-GATE Pass B (adversarial) | N14 subagent | Anti-iatrogenic check (does remediation introduce a worse defect?); evidence-rationale coherence; dimension-classification correctness | `halt-on-q-gate-failure` (subreason: `pass-b` for content-fail; `pass-b-exec-error` for subagent timeout/crash) |
| F-VAL ingest | N16 inline | Schema validation; SHA-256 capture; suspicious-content prompt (overrides `--auto`) | `halt-pre-fix-on-validator-failure` |
| Idempotency | N16 inline | State file authoritative; git-log fallback. State-file-vs-git-log conflict → warn-and-prompt user (state may show applied at sha not findable in current branch). | warn + user override |
| Tier classification | N16 inline | Tier-1/2/3 routing + defer-on-uncertainty + conflicting-edit detection | `halt-on-conflicting-fixes` |
| Fix-plan approval | N17 inline | Per-tier batch confirmation default; tier presented in order T1 → T2 → T3; decline-N → all Tier-N `deferred (user-declined-batch)`; user explicit `halt` → stop | per-tier outcome |
| Pre-flight baseline | N18 inline | Baseline captured before any fix (test counts, type errors, lint warnings, build status — pre-existing failures are recorded as the project's baseline state, NOT a halt condition); branch creation | `halt-on-baseline-failure` (only when runner crashes / cannot produce any output), `halt-on-test-cmd-unknown`, `halt-on-git-state-incompatible` |
| Per-fix verify | N20 inline | Targeted tests + type check on changed files; PASS → commit; FAIL → atomic-loop rollback + E_repair routing | After cap-hit on a group: mark group `failed`; downstream groups in topo-sort blocked → `deferred (upstream-dependency-failed)`; independent groups continue. `halt-mid-fix-on-perfix-cap-hit` only if every remaining group is blocked. |
| Regression battery | N21 inline | Full test suite vs baseline; type check; lint (`new_warnings_in_changed_regions == 0`); build clean; **diff-scope check** (every diff line maps to AUDIT-ID; regression-prevention test additions count as in-scope) | Test/type/lint/build fail → E_repair; scope-creep → `halt-on-scope-creep` (do NOT auto-revert) |
| Audit-rerun delta | N21 (subagent under `--deep`) | Re-run N01..N13 only (NOT N14 Q-GATE or N15 SaveHandler); classify new findings — touched-files → `induced-regression` (route to E_rerun_fail); untouched-files → `new-finding-discovered` (record only, no E_rerun_fail) | `halt-mid-fix-on-induced-regression-cap-hit` after retry cap exhaustion |

### 5.2 Fix safeguard pipeline (10-step canonical sequence)

| Step | Node | MUST-do |
|---|---|---|
| 1. Ingest | N16 | F-VAL schema validation; SHA-256 capture; idempotency check (state file > git-log fallback); conflicting-edits scan; suspicious-content prompt (overrides `--auto`) |
| 2. Triage | N16 | Group by file/module; topo-sort; tier 1/2/3; defer-on-uncertainty |
| 3. Plan | N17 | Emit fix-plan doc; await per-tier approval (default policy); `--dry-run` halts here |
| 4. Pre-flight | N18 | Capture baseline (tests/types/lint/build); create branch; halt on baseline/test-cmd/git-state failures |
| 5. Apply (atomic loop) | N19 | Per fix-group: apply edit → invoke N20 → commit `[AUDIT-NNN] <one-line>` (finding-id in commit body) on PASS, rollback on FAIL. **One concern per commit. No bundling.** Recovery manifest updated at every state transition. |
| 6. Per-fix verify | N20 | Targeted tests + type check on changed files; embedded inside step 5 atomic loop |
| 7. Audit-rerun | N21 (audit-rerun sub-step) | Re-run N01..N13 on fixed code (NOT N14 Q-GATE or N15 SaveHandler); classify new findings (touched vs. untouched files) |
| 8. Regression battery | N21 (battery sub-step) | Full test suite + type check + lint + build clean + diff-scope check |
| 9. Regression prevention | N19 (deferred to commit) | For behavioral fixes (not cosmetic): add a test that would have caught the original bug, in same commit (or paired follow-up if language requires separation). Test goes into per-fix verify; failing test rolls back entire commit. If no test framework detected → `regression_test_added: no (no test framework detected — manual test recommended)`, do NOT block. |
| 10. Reporting | N23 | Closing-the-loop fix report per Schema v1; status-priority sort |

### 5.3 Atomic execution loop (per fix-group, inside step 5)

**Critical primitive choice:** the loop **does not commit until verification passes**. This means a verification failure leaves zero history artifacts — no failed commit, no revert commit. Use `git checkout -- <touched-files>` (or equivalent working-tree discard) on failure, NOT `git revert HEAD`.

```
1. Apply edit (working tree only — no commit yet)     (manifest: in_flight_state=applying)
2. Run targeted tests (file/module level)             ──┐
3. Type check on changed files                        ──┤  steps 2-3 = N20 PerFixVerifier
                                                      ──┤  (manifest: in_flight_state=verifying)
4. PASS  → git add <touched-files> + git commit -m "[AUDIT-NNN] <one-line>"
            (commit body includes finding-id; regression-prevention test included if behavioral fix)
            (manifest: in_flight_state=committing → idle)
   FAIL → git checkout -- <touched-files>             (working tree restored; no commit ever made)
          → record failure_context (failure_class + diagnostic from N20)
          → E_repair routing:
              1st E_repair invocation → N19 retry-with-failure-context
              2nd E_repair invocation → N17 replan
              3rd E_repair invocation → cap-hit; mark group `failed`;
                                        downstream-dependent groups → `deferred (upstream-dependency-failed)`;
                                        independent groups continue
5. Update recovery manifest with new last_known_good_sha + revised pending list
   (manifest: in_flight_state=idle)
```

**Pre-commit hook failures** are routed through the same rollback path: if `git commit` fails because a hook (project-level pre-commit, formatter, linter) rejected the commit, N20 reports `failure_class: commit-hook-failure` and the working tree is restored with `git checkout -- <touched-files>`. The skill **never** passes `--no-verify` to bypass hooks (per §5.5 hard rules).

**One concern per commit. No bundling. No "while I'm here" cleanups.**

### 5.4 Tier + autonomy policy (default flipped to confirm-tier-batch — see brainstorm Critique 2)

| Tier | Definition | Default behavior | `--auto` | `--confirm-all` |
|---|---|---|---|---|
| 1 (mechanical) | Typo, missing import, dead code with no callers | Batch-confirm: *"apply N Tier-1 fixes? y/n"* | Silent apply | Per-fix confirm |
| 2 (local logic) | Single-function changes with bounded blast radius | Batch-confirm: *"apply M Tier-2 fixes? y/n"* | Batch-confirm | Per-fix confirm |
| 3 (cross-cutting) | Schema changes, public-API edits, multi-file refactors | Per-fix confirm | Per-fix confirm | Per-fix confirm |

**`--dry-run`:** no apply for any tier; emit plan + diffs only.

**Per-fix-opt-in floor (anti-conformity):** even under `--auto`, any individual fix where `evidence_confidence < HIGH AND remediation_complexity > trivial` is NOT auto-applied — emitted as recommendation requiring per-fix opt-in. Only HIGH-confidence trivial fixes (typo, missing import, dead code with no callers) auto-apply under `--auto`.

**`--escalate-finding F00N`** forces a specific finding to Tier-3 regardless of N16 classification. Overrides `--auto` for the named finding. `--demote-finding` is NOT supported (`halt-on-flag-rejection`; user must edit the report).

**Tier decline behavior:** each tier presented independently in order T1 → T2 → T3; decline on Tier-N → all Tier-N findings marked `deferred (user-declined-batch)`; pipeline proceeds to Tier-N+1. Explicit user `halt` → stop entirely.

### 5.5 Hard rules (audit + fix)

**Audit:**

- Every finding has all 4 mandatory fields (location, severity, confidence, remediation). Findings missing any → demoted to "Unverified Hypotheses".
- Every `file:line` is verified against the actual file via Read at audit time. No hallucinated lines.
- Every CRITICAL/HIGH finding has Confidence ≥ MEDIUM. HIGH-severity at LOW-confidence → demote in severity OR upgrade in confidence with stated evidence.
- LOW-confidence findings include `verify_by: <what would lift confidence>`.
- Duplicate patterns merged with count.
- Q-GATE Pass A no-comment-echo: no finding text quotes the project's own TODO/FIXME without independent verification of the underlying defect.

**Fix:**

- DO NOT apply fixes outside source tree.
- DO NOT modify files audit didn't flag.
- DO NOT skip post-fix verification.
- DO NOT batch-apply fixes spanning the same file without staged review.
- DO NOT continue after verification failure without explicit user authorization (or per E_repair bounded retry).
- Never expand scope beyond audit findings. Spotted unrelated bug → log it as a new finding, don't fix it now.
- Never bypass safety checks (`--no-verify`, `--force-push`, hook skipping).
- Never amend prior commits — always new commits, even on retry.
- Defer over guess — if root cause is unclear, mark `deferred` with a question.
- **Idempotent** — re-runs skip already-applied findings (state file > git-log fallback).
- **Fail-loud on partial state** — recovery manifest at every state transition; mid-flight death leaves a coherent file at rest.

**Anti-conformity — prior-fix temporal check:** at `--fix` mode entry, inspect (1) git log for `[AUDIT-NNN]` commit-message tags and (2) state files in `~/docs/epiphany/audit/.state/` matching `file:line` or finding-id from prior `epiphany-audit` runs. On match: emit warning *"this finding has been addressed before — re-applying may revert intentional behavior"*; require explicit user override per such fix.

---

## 6. Edge Cases, Anti-Patterns, Worked Examples, Production-Grade Definition

### 6.1 Edge cases

**Implied-context resolution:**

- Nested git repos, multi-project session, polyglot inconclusive → list candidates, ask user (no silent default).
- Resolved root is `$HOME` / wrapper repo / denylist match → `halt-suspicious-target`.

**R-ROUTE:**

- Heterogeneous monorepo → run dimensions per sub-tree; emit per-subtree maps in `subtrees: [...]` frontmatter; top-level `dimensions_activated` is the union.
- Binary-only repo → halt with *"no source detected, nothing to audit"*.
- Unknown language → fall through to language-agnostic dimensions only; emit *"language unidentified — running language-agnostic subset only"*.
- Floor (CORRECTNESS + MAINTAINABILITY) always emits.

**Fix-mode:**

- Input report from a different `tool_version` → F-VAL emits version-skew warning, prompts before proceeding (`halt-on-mismatched-version` if declined).
- Input report references files that no longer exist in `audit_target` → `skipped (stale-reference)`, do NOT abort whole run.
- Input report references files outside `audit_target` git tree → halt; refuse to write outside source tree.
- Two findings propose conflicting edits to same `file:line` → `halt-on-conflicting-fixes`; ask user to choose ordering.
- Recovery manifest detected at run start → `halt-on-recovery-conflict`; user choice:
    - `resume` — pick up from `last_known_good_sha`; skip applied; continue with pending list.
    - `fresh` — archive existing manifest to `.recovery/.archive/<report-id>-<timestamp>.json` (preserve forensic record, do NOT overwrite); start over.
    - `abort` — halt; no changes.
- Idempotency on re-run → state file authoritative; git-log fallback. Conflict (state says applied at sha not in current branch — possibly because of branch deletion, history rewrite, or squash merge) → emit prompt: *"State file claims F00N applied at sha &lt;X&gt; but commit not found in current branch. Options: (a) re-apply [override state], (b) skip [trust state], (c) abort. Choice?"* — answer required per affected finding; no auto-default.
- Scope-creep at N21 → `halt-on-scope-creep` + per-hunk listing; user choice = authorize (recorded as manual edit in fix report) OR revert manually.
- Fix-induced regression at N21 → route through E_rerun_fail back to N16 with regression context; if retry cap exhausted → `failed (induced-regression: <new-finding-id>)`, new finding `deferred (caused-by: <originating-finding-id>)`.
- Tier override → `--escalate-finding F00N` forces Tier-3 for one specific finding; overrides `--auto` for that finding.
- Test command unknown → `halt-on-test-cmd-unknown`.
- Behavioral fix without test framework → `applied` with `regression_test_added: no (no test framework detected — manual test recommended)`; do NOT block.
- B-FIND prompt invalid input under `--deep` → loop with format *"include gap dimension <name>? (y/n/skip-all)"*; ctrl-C → `halt-on-user-abort`.
- Git-state-incompatible at N18 → `halt-on-git-state-incompatible`.
- F-VAL suspicious content (zero findings, all-Unverified-Hypotheses, conflicting fixes) → halt-and-prompt regardless of `--auto`.

**`--deep` / `--verbose`:**

- `--verbose` adds depth where it improves actionability; never adds nitpick padding.
- `--deep` lifts coverage and spawn budget but is bounded by checkpoints at two scales:
    - **Inter-node checkpoint (80k aggregate cap):** between dimension analyzers (N04..N09 transitions) and before N14. Cap-trigger → emit partial report with `token_cap_partial: true`, `truncated_at_node: <id>`, `q_gate.pass_a: pass-minimal`, `q_gate.pass_b: skipped-token-cap`. Findings that fail minimal Pass A → Unverified Hypotheses.
    - **Intra-node soft budget (30k per analyzer):** inside a single dimension analyzer (e.g., N07 SECURITY on a large codebase), if accumulated context for that node exceeds 30k tokens, the analyzer aborts gracefully. Findings produced so far are kept. The dimension is marked in frontmatter as `dimension_partial: <X>` with reason `"intra-node soft budget exceeded — coverage incomplete for dimension X"`. The aggregate inter-node check still applies; the intra-node abort prevents one dimension from monopolizing the budget.

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
- Modify files audit didn't flag.
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
false_positive_check:
  intentional:           { value: false, justification: "no test or comment justifies the -1" }
  file_symbol_verified:  { value: true,  justification: "Read at src/parser.py:140-145" }
  reachable_from_entry:  { value: true,  justification: "called by parse_input in main.py:23" }
  fix_breaks_dependents: { value: false, justification: "grep shows no caller relies on N-1 emission" }
priority_score: 9.0   # (severity 3 × confidence 3) / effort 1
tests_present_signal: false
```

**Example 2 — Worked Node Registry row (`graph.json` fragment, conforms to `graph.schema.json`):**

```json
{
  "id": "N02",
  "name": "RelevanceRouter",
  "type": "router",
  "mode": "inline",
  "active_in": ["audit", "both"],
  "inputs": ["project_model from N01"],
  "outputs": ["dimension_activation_map", "skip_reasons", "subtrees (optional)"],
  "aggregation_policy": "n/a",
  "halt_conditions": ["language detection inconclusive in polyglot mode → halt-ambiguous-target"]
}
```

**Example 3 — Worked dimension-routing decision:**

```
PERFORMANCE:     skipped — no hot loops detected, no perf-critical heuristic match
SECURITY:        activated for [shell-injection, secrets-in-source];
                 skipped sub-surfaces [SQL] — no DB layer detected
CORRECTNESS:     activated (floor)
MAINTAINABILITY: activated (floor)
```

### 6.4 "Production-grade" — operationalized definition

The skill is *production-grade* iff:

1. **Deterministic routing** — R-ROUTE outputs the same activation map for the same project state (`project_model` from N01). Finding generation is **substantively-stable**: re-runs on identical code target ≥80% set-overlap. Verified via determinism fixture at `tests/determinism/<lang>-<size>/` containing reference projects (one per language × size cell — e.g., `python-small/`, `ts-large/`) with frozen expected-finding sets in `expected_findings.yaml`. Re-run set-overlap below 80% on a fixture → CI failure. Strict identity is not claimed because LLM-driven analyzers are probabilistic.
2. **Idempotent re-runs** — re-audit produces same findings modulo intentional code changes; `--fix` re-run on same report skips already-applied findings (state file authoritative; git-log fallback).
3. **Schema-versioned output** — `schema_version: 1` mandatory in both audit and fix reports; F-VAL strict validation on `--fix` ingest.
4. **Halt envelope on every halt state** — structured `{halt_state, subreason, diagnostic}` at the top of the user-facing message; downstream tooling parses the envelope.
5. **Structured per-node event log** — every node emits a single-line JSON event on entry/exit (`{"node": "N02", "phase": "exit", "decision": "..."}`) to `~/docs/epiphany/audit/.logs/<report-id>.jsonl`. **Stdout shows only user-facing milestones** (gate-passed, finding-being-verified, halt-state); the JSONL log carries the full trace.

---

## 7. Implementation Guidance for the Consumer

The consumer of this spec produces:

1. `SKILL.md` covering §1–§6 in human-readable form, with all 13 Layer-A sections from the source prompt populated.
2. `graph.json` declaring all 23 nodes + edges per §2 + §3, validated by `graph.schema.json`.
3. `modules/N01..N23.md` — one file per node, each following the Layer-B contract template:
    - **Inputs (typed)** — prose-with-shape annotations (e.g., `project_model: { language: string, files: list[Path], build_manifest: Path | null, test_command: string | null, git_state: { head: sha, dirty: bool, ... } }`). JSON-Schema-style optional but not required; the goal is unambiguous handoff between nodes, not full machine validation of every field.
    - **Outputs (typed)** — same shape conventions as inputs.
    - **Side effects** (none / read-only / git-staged / write-state-file / write-recovery-manifest / write-baseline / write-log / write-report / network)
        - Analyzer nodes (N04..N09): `none` or `read-only`.
        - **N15 SaveHandler:** `write-report` (audit report) + `write-state-file` (idempotency state, only on save-accept) + `write-log`.
        - **N18 PreFlight:** `write-baseline` (baseline metrics file) + `git-staged` (branch creation only — no edits) + `write-log`. **Does NOT write recovery manifest.**
        - **N19 FixApplier:** `git-staged` (working-tree edits + commits on PASS) + `write-recovery-manifest` (incremental updates at every state transition) + `write-log`.
        - **N22 RollbackHandler:** `git-staged` (working-tree discard via `git checkout --`) + `write-recovery-manifest` (finalize on planned termination or mid-flight death) + `write-log`.
        - **N23 FixReporter:** `write-report` (fix report) + `write-log`.
        - All other nodes: `read-only` or `none`, plus `write-log` for structured event emission.
    - **Halt conditions** (mapping to halt states in §3.3)
    - **Token budget hint** (per-node soft budget; analyzer nodes 30k under `--deep` per §6.1 intra-node cap)
    - **Backtrack/aggregation participation**
    - **Fan-out cardinality** (N04..N09 under `--deep`; N10 across findings)
    - **Back-edge endpoints** (N20 fail-signal → E_repair → N17/N19; N21 induced-regression → E_rerun_fail → N16)
4. `schemas/audit-report-v1.schema.json` and `schemas/fix-report-v1.schema.json` — authoritative JSON schemas matching §4.
5. `templates/audit-report.md.template` and `templates/fix-report.md.template`.
6. `tests/smoke/` — minimal end-to-end smoke runs covering: default invocation, `--audit` only, `--fix` on a known-good report, `--dry-run`, recovery-manifest resume, suspicious-target halt.
7. `tests/schema-validation/` — JSON-schema validation tests for both report schemas.

The consumer does **not** invoke `writing-plans` or `executing-plans`; the implementation plan for this spec is produced separately by the brainstorming-skill caller after spec approval.

---

## 8. Source Material

- Original input prompt: `~/docs/epiphany/prompts/27-04-epiphany-audit-skill-design.md`
- Brainstorm critiques resolved (10): node-count compression, default-confirm-tier-batch flipped, write-serial/read-parallel, R-ROUTE floor, state-file idempotency, per-node inline/subagent annotation, suspicious-target gate, B-FIND non-interactive default, token-cap checkpoints, file-layout choice (`epiphany-graph-genius` style).
- Self-audit fixes applied across all 6 design sections: 7 + 13 + 14 + 13 + 13 = 60 corrections.
