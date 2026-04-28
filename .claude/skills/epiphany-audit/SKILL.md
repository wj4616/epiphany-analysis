# epiphany-audit SKILL

Graph-of-thought codebase audit + safeguarded fix pipeline.
Family: `epiphany-*`

> **Implementer note:** this file is the Layer-A orchestrator contract. Each node's detailed
> Layer-B contract lives in `modules/N0N-*.md`. JSON schemas are in `schemas/`. Graph
> declaration is in `graph.json`. Dimension plugins are in `dimensions/`.

---

## 1. Purpose & Scope

`epiphany-audit` runs a multidimensional, project-aware audit on any codebase, then
optionally drives a safeguarded fix pipeline. It is designed to:

- Audit for regression areas, bugs, potential issues, and latent problems
- Prune irrelevant dimensions via R-ROUTE (skipped dimensions cite explicit rationale)
- Surface project-specific blindspots via B-FIND
- Cite every finding with verified `file:line` evidence (no hallucinated locations)
- Apply fixes only with explicit per-tier user consent (default policy)
- Track fix state via recovery manifest for safe resumption after interruption
- Optionally generate improvement recommendations via the `--improve` subpipeline

**MAY invoke:** `kb-route` (optional KB consultation via dimension plugins), `prompt-graph` (optional remediation-message enhancement; never required).

**MUST NOT invoke:** `writing-plans`, `executing-plans`, `find-skills`.

**Write-tool footprint:** report files under `~/docs/epiphany/audit/` only; fix diffs against `audit_target` only.

---

## 2. Invocation Contract

```
/epiphany-audit [<path>] [--audit | --fix <report>] [--verbose] [--deep] [--improve]
                [--auto | --confirm-all | --dry-run]
                [--escalate-finding F00N] [--test-cmd '<cmd>']
                [--monorepo-subtree-limit N] [--reverify-state]
                [--full-rerun | --no-rerun]
```

### Mode flags (mutually exclusive)

| Flag | Behavior |
|------|----------|
| `--audit` | Produce report only; do NOT offer fix pipeline |
| `--fix <report>` | Consume existing audit report; run fix pipeline only (skips N01..N15) |
| (none) | Audit → save prompt → fix-offer (default) |
| `--audit` + `--fix` | `halt-on-flag-conflict` |

### Autonomy flags (mutually exclusive)

| Flag | Behavior |
|------|----------|
| `--auto` | Tier-1 silent apply; Tier-2 batch-confirmed; Tier-3 per-fix |
| `--confirm-all` | Every fix requires per-fix confirmation |
| `--dry-run` | Emit fix-plan + diffs only; no apply, no commits, no branch; pipeline halts at N17 |
| >1 of the above | `halt-on-flag-conflict`; precedence on contradiction: `--dry-run` > `--confirm-all` > `--auto` |

### Other flags

| Flag | Behavior |
|------|----------|
| `--verbose` | Adds depth where it improves actionability; never adds nitpick padding |
| `--deep` | Lifts spawn budget to ≤3 (≤4 with `--improve`); subagent fan-out for analyzers; interactive B-FIND prompt; 80k-token checkpoint cap |
| `--improve` | After audit + save prompt, run improvement subpipeline (N24–N27). Valid in audit/no-flag mode only; warning + skip if used with `--fix` |
| `--escalate-finding F00N` | Force finding to Tier-3 regardless of N16 classification; overrides `--auto` for that finding; `halt-on-invalid-finding-id` if ID not in report |
| `--test-cmd '<cmd>'` | Override auto-detected test command |
| `--monorepo-subtree-limit N` | Override cap on distinct project-shaped subtrees (default 10) |
| `--full-rerun` / `--no-rerun` | Override audit-rerun tier policy (§8); mutually exclusive; `halt-on-flag-conflict` if both given |
| `--reverify-state` | Clear `reachable: false` annotations from state file at `--fix` entry; valid in `--fix` mode only |

### `<report>` resolution order (for `--fix`)

1. Absolute path
2. Path relative to cwd
3. Bare filename or partial slug: search `~/docs/epiphany/audit/` then `fix-reports/`; if no extension, append `.md` and retry
4. Multiple matches → `halt-on-ambiguous-fix-report`
5. No match → `halt-on-unresolvable-fix-report`

---

## 3. Implied-Context Resolution

Deterministic, halt-on-ambiguity. **No silent guessing.**

Resolution order:
1. Explicit `<path>` argument wins
2. `--fix <report>` → derives target from `audit_target` in report frontmatter; if both explicit `<path>` AND `--fix <report>` given and `realpath(<path>) != realpath(audit_target)` → `halt-on-target-conflict`
3. `cwd` inside git repo → `git rev-parse --show-toplevel`
4. `cwd` itself
5. `halt-pre-audit`

**Suspicious-target gate** (runs on resolved target regardless of resolution path):

Hard halt (`halt-suspicious-target`) if resolved root is `$HOME`, `/`, `/etc`, `/usr`, `/var`, `/tmp`, or has >5 top-level subdirectories that each independently look like a project.

Warn-and-prompt (soft halt; user may override via `~/.config/epiphany-audit/allowed-roots.json`) for:
- `~/.claude/skills/<x>/`, `~/dotfiles`, `~/.config`, `~/Desktop`, `~/Downloads`

Other ambiguity halts:
- Nested git repos detected → list candidates, ask user
- Session touched multiple projects → list candidates, ask user
- Polyglot monorepo with inconclusive language detection → ask user

---

## 4. Node Registry Overview

27 nodes: 15 audit-pipeline (N01–N15) + 8 fix-pipeline (N16–N23) + 4 improvement subpipeline (N24–N27).

| ID | Name | Active in | Type |
|----|------|-----------|------|
| N01 | ContextIntake | audit | ingest |
| N02 | RelevanceRouter (R-ROUTE) | audit | router |
| N03 | BlindspotFinder (B-FIND) | audit | meta-analyzer |
| N04 | DimensionAnalyzer.CORRECTNESS | audit | analyzer |
| N05 | DimensionAnalyzer.ARCHITECTURE | audit | analyzer |
| N06 | DimensionAnalyzer.PERFORMANCE | audit | analyzer |
| N07 | DimensionAnalyzer.SECURITY | audit | analyzer |
| N08 | DimensionAnalyzer.MAINTAINABILITY | audit | analyzer |
| N09 | DimensionAnalyzer.\<X\> (plugin-instantiated) | audit | analyzer |
| N10 | FalsePositiveVerifier (FPV) | audit | verifier |
| N11 | FindingsAggregator | audit | aggregator |
| N12 | Prioritizer | audit | scorer |
| N13 | ReportFormatter | audit | formatter |
| N14 | Q-GATE (Pass A inline + Pass B conditional subagent) | audit | verifier |
| N15 | SaveHandler | audit | io |
| N16 | FixTriage (F-VAL ingest + Resume-handler + Triage) | fix | validator |
| N17 | FixPlanner | fix | planner |
| N18 | PreFlight | fix | preflight |
| N19 | FixApplier | fix | actuator |
| N20 | PerFixVerifier | fix | verifier |
| N21 | RegressionBattery (battery + tiered audit-rerun) | both | verifier |
| N22 | RollbackHandler | fix | recovery |
| N23 | FixReporter | fix | formatter |
| N24 | ImprovementContextualizer | improve | analyzer |
| N25 | ImprovementBrainstormer | improve | analyzer |
| N26 | OverEngineeringFilter (OEF) | improve | filter |
| N27 | ImprovementReporter | improve | formatter |

For each node's full Layer-B contract see `modules/N0N-*.md`. For the graph declaration see `graph.json`.

---

## 5. Mode Flowcharts

### Audit mode (`--audit` or no-flag)

```
invoke
  └─ N01 ContextIntake
       └─ N02 R-ROUTE (loads dimension plugins; floor: CORRECTNESS + MAINTAINABILITY always on)
            ├─ N03 B-FIND (E02; auto-add HIGH-confidence gaps; --deep: interactive)
            │    [N03 updates activation map; N04..N09 consume the updated map]
            └─ N04..N09 DimensionAnalyzers (E03 from N02; per updated activation map; parallel fan-out under --deep)
                      └─ N10 FPV (false-positive check + location cache; BACKTRACK single cap)
                           └─ N11 Aggregator (dedup, merge, count-collapse)
                                └─ N12 Prioritizer (priority_score, punch list)
                                     └─ N13 Formatter (markdown per §4.1 template)
                                          └─ N14 Q-GATE
                                               Pass A (mandatory-field + location + CRITICAL/HIGH confidence + no-comment-echo)
                                               Pass B (conditional subagent: ≥5 findings OR CRITICAL/HIGH present OR --deep)
                                               └─ N15 SaveHandler
                                                    └─ [--improve]: N24 → N25 → N26 → N27 → user (E20)
                                                    └─ [no-flag]: fix-offer (E21) → N16..N23 fix pipeline
                                                    └─ [--audit]: done (E12 save prompt only)
```

### Fix mode (`--fix <report>` or post-audit fix consent)

```
  └─ N16 FixTriage
       [resume-handler sub-step if prior interrupted run detected]
       F-VAL schema validation → idempotency check → tier classification → grouping + topo-sort
       └─ N17 FixPlanner
            tier batch approval → [--dry-run: write dry-run plan, halt]
            └─ N18 PreFlight
                 orphan sweep → git-state check → baseline capture → branch creation
                 └─ N19 FixApplier (atomic loop per fix-group)
                      └─ N20 PerFixVerifier (targeted tests + type check)
                           [E_repair: 1st→retry N19; 2nd→replan N17; 3rd→cap-hit→N22]
                           └─ N21 RegressionBattery
                                battery (tests/types/lint/build/diff-scope)
                                + tiered audit-rerun (Tier-1: skip; Tier-2: narrow; Tier-3: full)
                                [E_rerun_fail: induced regressions → N16 re-triage]
                                └─ N23 FixReporter (planned termination: partial=false)
                                     └─ E_finalize → N22 RollbackHandler (archive manifest)
                                          └─ E_complete → user
```

### Halt-mid-fix path

```
halt-mid-fix-on-perfix-cap-hit OR halt-mid-fix-on-induced-regression-cap-hit
  └─ E_halt_partial → N23 (partial=true, halt_state=<id>)
       └─ halt envelope → user
       [recovery manifest stays live for resume]
```

---

## 6. Halt-State Envelope Format

Every halt state emits a structured envelope at the **top** of the user-facing message before any diagnostic text:

```
{halt_state: <state-id>, subreason: <text>, diagnostic: <details>}
```

| halt_state | Triggered at | Subreason axis |
|------------|-------------|----------------|
| `halt-pre-audit` | N01 resolution | no resolvable target |
| `halt-suspicious-target` | N01 gate | `$HOME` / wrapper repo / denylist match |
| `halt-ambiguous-target` | N01 resolution | nested git / multi-project / polyglot inconclusive |
| `halt-on-flag-conflict` | N01 parse | mutually exclusive flags |
| `halt-on-flag-rejection` | N01 parse | unsupported flag (e.g., `--demote-finding`) |
| `halt-on-target-conflict` | N01 resolution | explicit path ≠ report's audit_target |
| `halt-on-unresolvable-fix-report` | N16 entry | `<report>` could not be resolved |
| `halt-on-ambiguous-fix-report` | N16 entry | bare `<report>` matched multiple files |
| `halt-on-floor-plugin-missing` | N02 startup | bundled floor plugin file missing; subreason: plugin-name |
| `halt-on-mismatched-version` | N16 F-VAL | tool_version skew; user declined |
| `halt-pre-fix-on-validator-failure` | N16 F-VAL | schema fail / suspicious-content user-declined |
| `halt-on-empty-or-unfixable-report` | N16 ingest | zero main-body findings |
| `halt-on-conflicting-fixes` | N16 triage | incompatible edits on same line range (live mode) |
| `halt-on-test-cmd-unknown` | N18 | no test command available |
| `halt-on-baseline-failure` | N18 | runner crash; subreason: `resume-baseline-missing` |
| `halt-on-git-state-incompatible` | N18 | dirty tree / detached HEAD / no commits; subreason: `branch-name-exhausted` |
| `halt-on-q-gate-failure` | N14 | subreason: `pass-a` / `pass-b` / `pass-b-exec-error` |
| `halt-mid-fix-on-perfix-cap-hit` | E_repair (3rd) | E_repair retries exhausted; all remaining groups blocked |
| `halt-mid-fix-on-induced-regression-cap-hit` | E_repair (post-rerun) | induced-regression fix-group retry cap exhausted |
| `halt-on-scope-creep` | N21 diff-scope | unmapped diff hunks; do NOT auto-revert |
| `halt-on-token-cap` | `--deep` | accumulated context > 80k; partial report emitted |
| `halt-on-recovery-conflict` | run entry | recovery manifest from prior interrupted run |
| `halt-on-user-abort` | any interactive prompt | ctrl-C / explicit halt |
| `halt-on-stale-source-report` | N16 F-VAL | source report file missing or SHA-256 mismatch |
| `halt-no-source-detected` | N01 | binary-only / empty / documentation-only repo |
| `halt-on-files-outside-tree` | N16 F-VAL | report references files outside audit_target tree |
| `halt-on-invalid-finding-id` | N16 post-F-VAL | `--escalate-finding` ID absent from report |
| `halt-on-resume-tree-divergence` | N16 resume-handler | cleanup would discard out-of-scope working-tree changes |

---

## 7. Report Schema Cross-Reference

Five report types; all schema-versioned (`schema_version: 1`). Authoritative JSON schemas in `schemas/`.

| Report type | Output path | Schema file |
|-------------|-------------|-------------|
| Audit report | `~/docs/epiphany/audit/<project-slug>-<YYYYMMDD>-<HHMMSS>.md` | `audit-report-v1.schema.json` |
| Fix report | `~/docs/epiphany/audit/fix-reports/<source-report-id>-fix-<YYYYMMDD>-<HHMMSS>.md` | `fix-report-v1.schema.json` |
| Dry-run plan | `~/docs/epiphany/audit/dry-run-plans/<source-report-id>-dryrun-<YYYYMMDD>-<HHMMSS>.md` | `dry-run-plan-v1.schema.json` |
| Dimension plugin | `dimensions/*.md` (YAML frontmatter only) | `dimension-plugin-v1.schema.json` |
| Improvement report | `~/docs/epiphany/audit/improvement-reports/<project-slug>-<YYYYMMDD>-<HHMMSS>-improve.md` | `improvement-report-v1.schema.json` |

**Cross-schema invariants:**
- `source_report_id` in fix-report = `report_id` in audit-report = `source_report_id` in dry-run plan = `source_report_id` in improvement report.
- `source_audit_report_sha256` verified at `--fix` start; mismatch → `halt-on-stale-source-report`.
- `partial: true ⇔ halt_state: non-null` in fix reports (mutual implication; validated by JSON Schema).
- "Unverified Hypotheses" findings are NOT consumable by `--fix`; F-VAL ingests only main-body findings.
- Improvement reports are read-only artifacts; NOT consumed by `--fix`.

**Finding priority_score formula:**
```
severity:    CRITICAL=4, HIGH=3, MEDIUM=2, LOW=1, INFO=0
confidence:  HIGH=3, MEDIUM=2, LOW=1
effort:      trivial=1, modest=2, significant=3
priority_score = (severity × confidence) / effort
```

**Finding severity definitions:**
```
CRITICAL = data loss, security breach, crash on common path, corruption
HIGH     = crash on edge case, wrong output silently, perf regression >2x
MEDIUM   = degraded UX, recoverable error mishandled, maintainability cliff
LOW      = code smell with concrete future cost
INFO     = observation, no action required
```
