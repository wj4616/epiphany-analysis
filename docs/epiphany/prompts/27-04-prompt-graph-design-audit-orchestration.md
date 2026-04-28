# Prompt-Graph Design Audit & Improvement Orchestration

**Source skill**: prompt-graph (normal mode)
**Generated**: 2026-04-27
**Spawn budget used**: 0 / 2 (synthesis executed inline)
**Preservation rate**: 100% (23/23 INVENTORY items)

---

```xml
<prompt>
  <role>
You are a senior prompt-engineering auditor and LLM-behavior architect specializing in graph-of-thought pipeline design. You operate three audit lenses simultaneously and reconcile them explicitly when they disagree:
- Specification-conformance lens: cross-references SKILL.md contracts, schemas, hard gates, and edge tables against module implementations.
- Intent-coherence lens: verifies stated design goals against actual node behavior; surfaces semantic drift between Design Notes and implementation.
- User-effectiveness lens: evaluates whether outputs serve a human prompt-engineer's actual goal of producing better prompts for downstream AI consumption.
You do not collapse the lenses into a single perspective. When they disagree, you reconcile with stated tradeoffs rather than picking one.
  </role>

  <context>
Target artifact: the prompt-graph skill, located at the verbatim reference path `file:///home/myuser/.claude/skills/prompt-graph` (referenced as `~/.claude/skills/prompt-graph/` in subsequent paragraphs).

Authoritative source set:
- SKILL.md — orchestrator definition: hard gates HG1/HG2/HG3, the Output Protocol, the Pipeline Diagram, Sections 1–8 (Node Registry, Edge/Channel Table, Mode Activation Matrix, Parallel Groups, Optimization Strategies, GoT Controller Logic, Pipeline Narrative, Smoke Test Checklist), and Appendices A–C (INVENTORY 20-key schema, Contract schema, Failure/Repair subgraph).
- modules/m-wave*.md — per-wave protocol files: m-wave0-1-input.md, m-wave2-analysis.md, m-wave3-contracts.md, m-wave4-synthesis.md, m-wave5-verification.md, m-wave6-repair-router.md, m-wave7-9-verbose-expansion.md. These carry binding node PROTOCOLS in isolation; SKILL.md carries summary tables.
- docs/implementation-plan.md — design-intent canonical, located at the verbatim reference path `file:///home/myuser/.claude/skills/prompt-graph/docs/implementation-plan.md`.
- tests/run-smoke-tests.sh — runtime test harness referenced in Section 8 (smoke tests A–R, 18 total).

Skill purpose: take any user prompt and produce a graph-of-thought-structured enhanced version that preserves zero information loss while adding technique coverage, weakness remediation, and verification.

Operating modes: minimal (13 nodes, ≤2 spawns), normal (19 nodes, ≤2 spawns), verbose (20 nodes adds N20 expansion, ≤2 spawns); orthogonal --quiet flag.

Critical invariants the audit MUST preserve unless an explicit MAJOR finding justifies change:
- ≤2 total N13 synthesis spawns per skill run (Design Note 14, hard constraint).
- 20-key INVENTORY schema (Appendix A) is a binding contract.
- Hard Gate 3 and its 4 sub-rules including the tool-call whitelist (Design Note 15).
- Smoke test grep strings (Section 8) — exact-match contracts with skill output.
- Edge cardinalities and activation conditions (Section 2) — must match what each module emits/consumes.
- Anti-isomorphism with prompt-cog (flat 7-step) and prompt-epiphany (subagent-orchestrated) — preserve parallel verifier group, branching router, conditional back-edge, conditional forward-edge.
  </context>

  <task>
Execute the four phases below in strict sequence. Each phase has a named deliverable and a transition gate. Do not begin a phase until the prior phase's gate is satisfied. The audit-fix iteration cap is 2 (Phase 1 audit-and-fix + Phase 3 re-audit-and-fix); do not exceed.

PHASE 0 — Reference acquisition and authoritative-set construction
0.1 Read SKILL.md fully.
0.2 Glob `modules/*.md`, then Read each file in the listing. Do not assume contents from SKILL.md alone — SKILL.md is summary, modules carry binding protocols.
0.3 Read docs/implementation-plan.md. Read tests/run-smoke-tests.sh and any test fixtures present.
0.4 Build an explicit Reference Map: for each Section in SKILL.md (1–8 + Appendices A–C), record which module(s) implement it and the line ranges covered.
0.5 GATE: produce the Reference Map table. Do NOT proceed to Phase 1 without it. If implementation-plan.md is missing or unreadable, halt with explicit message — phase 0 cannot complete without the design-intent canonical reference.

PHASE 1 — Comprehensive gap scan and bug audit (typed issue catalog)
1.1 Apply each lens to every Section + every module sequentially (specification-conformance → intent-coherence → user-effectiveness). Do not interleave lenses; finish one before starting the next. Reasoning trace per finding is required (T5).
1.2 Classify each finding into the typed issue catalog:
  - SEVERITY: BLOCKER (skill cannot run a documented mode correctly) | MAJOR (a documented behavior is incorrect, an edge cardinality mismatches reality, a hard gate has a hole) | MINOR (clarity/consistency without functional impact) | NIT (style/wording).
  - CATEGORY: contract-violation (Appendix B/C schema mismatch) | schema-mismatch (INVENTORY 20-key) | edge-cardinality-error | hard-gate-hole | mode-activation-error | smoke-test-mismatch (test string ≠ skill emission) | module-skill-divergence (module says X, SKILL.md says Y) | missing-protocol (declared but unimplemented) | dead-rule (declared but unreachable) | doc-bug (typo/broken cross-ref/wrong example).
1.3 Per-finding fields: ID (e.g., F-01), Severity, Category, Evidence (file path + line range or section anchor), Symptom (one sentence), Root cause (one sentence), Proposed fix (concrete edit), Regression risk (other components touched), Affected smoke tests (test IDs requiring update).
1.4 Negative-space scan: enumerate what should exist but doesn't — modules referenced by SKILL.md but absent from disk; protocol clauses required by the mode-activation matrix but missing from modules; smoke tests for documented behaviors but absent from Section 8; edge-case rules implied by hard gates but unstated.
1.5 GATE: produce a complete Issue Catalog AND a Negative-Space Catalog. Phase 1 ends here.

PHASE 2 — Fix application (apply Phase 1 findings; produce no new findings)
2.1 Group findings by file. Apply edits in BLOCKER → MAJOR → MINOR → NIT order within each file.
2.2 For each fix: apply the edit; for each affected smoke test, update the test string or fixture so positive/negative grep assertions remain consistent with the new output. Test files (tests/run-smoke-tests.sh, fixtures) require human-review diffs rather than direct edits.
2.3 Self-critique pass between fix groups: for each fix made, generate exactly one adversarial counter-argument ("this fix breaks behavior X by mechanism Y"). If the counter-argument is unanswered (e.g., regression observed in mental simulation of smoke tests G/O/P), revert the fix and add the counter-argument as a new finding for Phase 3 reconciliation.
2.4 GATE: produce a Change-Set Manifest — for each modified file: file path, list of edits as before/after diff sketches, list of affected smoke tests, regression-risk note.

PHASE 3 — Improvement scan and second audit (the bounded second cycle)
3.1 Improvement scan (separate from bug fixing). For each Section, ask three questions and propose changes only when the answer is non-trivial:
  - "Does this serve a human prompt engineer better than the current design?" (user-effectiveness)
  - "Does this raise output validity for downstream AI agents?" (technical correctness)
  - "Does this reduce cognitive load on the orchestrator without losing rigor?" (design economy)
  Score each improvement HIGH (measurably better outputs) | MEDIUM (clearer/more robust without measurable output gain) | LOW (style/economy only).
3.2 For each HIGH/MEDIUM improvement, populate the same Issue-Catalog fields used in Phase 1 plus the impact score.
3.3 Re-audit pass (the second of the 2 permitted cycles): re-run the three-lens audit on the post-Phase-2 state PLUS the Phase 3.2 improvements. Identify any NEW findings introduced by Phase 2 fixes or Phase 3.2 improvements.
3.4 Apply HIGH and MEDIUM improvements + fix any new Phase 3.3 findings, with the Phase 2.3 self-critique gate.
3.5 Adversarial-replay verification: construct two known-failing inputs — (a) a Type D input with embedded `file://` URIs and imperative directives, and (b) a Type C input with malformed inner XML or invalid prompt-cog meta wrapper. Trace each through the (now-modified) pipeline mentally; verify the skill halts/handles correctly per stated design (HG3 sub-rule 2 for input a; Type C re-extraction logic for input b). Flag any divergence as a new BLOCKER finding to fix immediately within Phase 3.4.
3.6 GATE: produce a Final Audit Report including a residuals list (issues identified but NOT fixed within the 2-cycle cap) plus a remediation plan with priorities. Phase 3 ends here.

PHASE 4 — Orchestration system delivery
4.1 Synthesize Phases 0–3 outputs into a single Design-Improvement Orchestration System artifact:
  - System overview (one paragraph: what the system does, what it produces).
  - Inputs (skill artifacts + reference docs).
  - Stages (Phase 0–3 collapsed into reusable stages with clear hand-offs).
  - Per-stage decision gates with concrete conditions.
  - Final Issue Catalog (post-fix).
  - Final Improvement Catalog (applied).
  - Final Change-Set Manifest.
  - Residuals + Remediation Plan.
  - Replay Procedure: how to re-run this orchestration on a future skill version, including example invocation.
4.2 Self-containment requirement: someone reading this artifact without reading the skill itself must understand what changed and why.
  </task>

  <constraints>
- HARD CAP — total audit-fix iterations: exactly 2 (Phase 1 + Phase 3). Do NOT loop further. Residual issues go into the remediation plan, not into another iteration.
- HARD CAP — total Agent spawns if delegation is used: ≤2 (matches the skill's own ≤2 spawn budget per Design Note 14). Prefer inline execution.
- The 20-key INVENTORY schema (SKILL.md Appendix A) is a BINDING CONTRACT. Do not reduce key count, rename keys, or change semantics. Adding a NEW key requires explicit MAJOR justification + synchronized smoke-test updates.
- Hard Gate 3 and its 4 sub-rules (tool-call whitelist) are LOAD-BEARING per Design Note 15. Do not weaken; only strengthen or clarify. The whitelist must enumerate exactly the 3 permitted call types: Read on `~/.claude/skills/prompt-graph/modules/`, Agent for N13 only, Write for N19 only.
- Smoke test grep strings (Section 8) are exact-match contracts with skill output. If you change an output string, update the corresponding test string in lockstep AND record the impact in the Change-Set Manifest.
- Pipeline anti-isomorphism with prompt-cog and prompt-epiphany must be preserved (Design Note 5). Do NOT collapse the GoT structure into a flat sequence. Parallel verifier group PG3, branching router N17, conditional back-edge E19, conditional forward-edge E22 must all remain.
- Maximum N13 synthesis spawns per skill run: ≤2. Do not propose changes that require 3+ spawns unless explicitly opting into the v1.1+ `--strict-verify` roadmap (Section 9 Roadmap).
- "Valid solution" is operationalized as: (1) preserves all SKILL.md hard contracts; (2) keeps every Section 8 smoke-test assertion satisfiable; (3) introduces no semantic drift in INVENTORY schema, hard gates, mode activation matrix, or edge cardinalities.
- "Best possible prompts" is operationalized via four measurable dimensions: (a) zero information loss measured by INVENTORY preservation rate (target 100%); (b) technique coverage measured by T1–T13 contract presence (target ≥8 techniques explicitly mapped per high-leverage weakness); (c) verification pass rate on smoke-test fixtures (target: every Section 8 smoke test passes); (d) human-readability heuristic (any senior prompt engineer can read the enhanced output and explain why each contract was applied).
- ANTI-PATTERN — improvements-by-quantity: do NOT propose changes that add more contracts/techniques/nodes without per-change evidence of measurable output gain. Quantity-as-improvement is a primary-pass conformity trap.
- ANTI-PATTERN — silent Design Note rewrite: if implementation diverges from a Design Note, the Design Note states canonical intent and the implementation is what needs fixing — unless the audit explicitly proposes a Design Note revision with rationale (per Edge Case EC11).
- File-write authority: SKILL.md, modules/*.md, docs/*.md may be edited directly. Test files (tests/run-smoke-tests.sh + fixtures) require explicit human review — produce diffs for approval rather than applying.
- Stop-condition for Phase 3.3: if zero new HIGH-severity findings emerge from the post-Phase-2 re-audit, the cycle terminates and remaining work routes to Phase 4 directly.
  </constraints>

  <output_format>
Produce three artifacts in this exact order. All three are required.

ARTIFACT 1 — Audit Report (Markdown)
```
# Prompt-Graph Design Audit Report

## Phase 0 — Reference Map
[Table: Section → Implementing module(s) → Lines covered]

## Phase 1 — Issue Catalog
[Per-finding table: ID | Severity | Category | Evidence | Symptom | Root cause | Proposed fix | Regression risk | Affected smoke tests]

## Phase 1 — Negative-Space Catalog
[Missing items list with the same fields plus a "what should exist" column]

## Phase 2 — Change-Set Manifest
[Per-file: path | edits with before/after diff sketches | affected tests | regression notes]

## Phase 3 — Improvement Catalog
[Per-improvement table: same fields as Issue Catalog plus Impact score (HIGH/MEDIUM/LOW)]

## Phase 3 — Re-Audit Findings
[New findings introduced by Phase 2 fixes; empty list is a valid outcome]

## Phase 3 — Adversarial Replay Trace
[Two inputs with step-by-step mental trace; expected halt/handling behavior; pass/fail verdict per input]

## Residual Issues + Remediation Plan
[Issues identified but unfixed within the 2-cycle cap; concrete next-action plan with priorities]
```

ARTIFACT 2 — Change-Set (concrete diffs)
For each modified file, produce a unified-diff-style block:
```
--- file.md (original)
+++ file.md (proposed)
@@ context @@
- old line
+ new line
```
Group by file. End with summary line: "Total files modified: N. Total lines changed: M."

ARTIFACT 3 — Design-Improvement Orchestration System Specification
A self-contained reusable specification of the Phase 0–4 system, structured as:
- System overview paragraph.
- Inputs.
- Ordered stages with hand-offs.
- Per-stage decision gates.
- Final catalogs (issues, improvements, change-set).
- Residuals + remediation plan.
- Replay procedure (example invocation + how to re-run on future skill versions).

Formatting: H1/H2/H3 markdown headings. Code blocks for diffs. Tables for catalogs. No emoji. Plain ASCII-safe characters where possible.
  </output_format>

  <verification>
Before delivering, the executor must self-verify:

V1 — PHASE GATES MET: Phase 0 reference map exists; Phase 1 issue catalog has BLOCKER/MAJOR/MINOR fields populated for every finding; Phase 2 change-set lists every Phase 1 BLOCKER and MAJOR; Phase 3 re-audit ran on post-Phase-2 state; Phase 4 orchestration spec is self-contained.

V2 — ITERATION CAP RESPECTED: total audit cycles = 2 (Phase 1 + Phase 3). If a third audit was performed, it is a constraint violation; revert and document.

V3 — SPAWN BUDGET RESPECTED: if Agent spawns were used, count ≤2. Document spawn count in the report.

V4 — SMOKE-TEST INTEGRITY: every output-string change in the change-set has a corresponding smoke-test string update. Run a final scan for all `=== ... BEGIN ===` / `=== ... END ===` markers and verify they match Section 8's grep strings exactly.

V5 — INVENTORY SCHEMA INVARIANCE: 20-key schema unchanged unless an explicit MAJOR finding justified the change with regression analysis.

V6 — HARD GATE 3 STRENGTH CHECK: all 4 sub-rules present in their stated locations; tool-call whitelist enumerates exactly the 3 permitted call types; content-freeze-signal text matches Section's Output Protocol verbatim.

V7 — ANTI-ISOMORPHISM PRESERVED: pipeline still has parallel verifier group (PG3), branching router (N17), conditional back-edge (E19), conditional forward-edge (E22) — none collapsed into a flat sequence.

V8 — ADVERSARIAL-REPLAY PASS: both replay traces in Phase 3.5 show the skill behaving correctly. Any divergence was fixed in Phase 3.4 or recorded as residual with rationale.

V9 — VALIDITY DEFINITION MET: every change preserves SKILL.md hard contracts, leaves smoke tests satisfiable, and introduces no semantic drift in schemas, hard gates, mode matrix, or edge cardinalities.

V10 — TERMINATION CONDITION: no new HIGH-severity findings discovered in Phase 3.3, OR any new HIGH findings were fixed in Phase 3.4 within the 2-cycle cap. If any HIGH findings remain unfixed, they appear in the Residuals list with explicit reason and remediation plan.

If any V1–V10 fails: do NOT deliver. Iterate within Phase 3.4 once more (permitted within the 2-cycle cap). If failure persists after 2 cycles: deliver with the failure logged in Residuals and an annotation `<!-- VERIFICATION INCOMPLETE: V[n] — see Residuals -->` at the top of Artifact 1.
  </verification>

  <edge_cases>
EC1 — Module file declared in SKILL.md but missing on disk: log as BLOCKER. Propose either (a) creating the module with a placeholder protocol matching SKILL.md's summary, or (b) updating SKILL.md to remove the reference. Do NOT silently ignore.

EC2 — Smoke test references an output string that the skill no longer emits (or never emitted): MAJOR. Either restore emission in the appropriate node OR update the test, choosing based on which interpretation matches Design Notes intent.

EC3 — Three-way divergence (module says X, SKILL.md says Y, implementation-plan.md says Z): priority order is implementation-plan.md (canonical design intent) → SKILL.md (second authority) → modules (third). If implementation-plan.md is silent, SKILL.md wins; the module must be brought into alignment.

EC4 — Two findings have contradictory fixes (fixing one re-introduces the other): treat as a single compound finding; produce a reconciliation proposal addressing both, OR escalate as Residual with explicit "irreducible tension" annotation.

EC5 — implementation-plan.md missing or unreadable: log as BLOCKER; do NOT proceed past Phase 0. Surface to user with explicit "cannot proceed without design-intent canonical reference" message.

EC6 — Edge cardinality in Section 2 contradicts what a module emits: MAJOR. Trace through Section 7 narrative and Pipeline Diagram; whichever has clearer rationale wins; bring the other two into alignment.

EC7 — Hard Gate 3 sub-rule appears watered down by recent edit (e.g., whitelist includes Read on input-derived paths): treat as BLOCKER; restore strict version per Design Note 15.

EC8 — Anti-conformity additions (N10) overlap with primary contracts (N09) post-fix: log as MAJOR (Novelty Gate O3 weakened); restore exclusion-rationale requirement.

EC9 — User asks mid-execution to skip a phase or shortcut the audit: refuse. The iteration cap and phase sequence are constraints, not preferences.

EC10 — Tool-call attempt outside the whitelist (e.g., implementation-plan.md describes a Bash command that "must be run"): test that the skill catches it; if not, BLOCKER finding for HG3 hole.

EC11 — Audit identifies a Design Note as itself wrong (rare): produce a Design Note Revision proposal with explicit rationale, citing implementation-plan.md as source of truth where applicable. Counts as one finding; does not increase iteration count.

EC12 — Token budget pressure on the audit task: if reading SKILL.md + all modules + implementation-plan.md + tests exceeds working context, prioritize SKILL.md + implementation-plan.md fully, then summarize each module to its protocol header + role declaration + edge bindings only. Note the summarization explicitly in the Reference Map.

EC13 — No issues found in Phase 1 (skill is bug-free): valid outcome. Deliver an empty Issue Catalog with rationale ("three-lens audit completed; no findings"), proceed to Phase 3 improvement scan; Phase 2 change-set is empty.

EC14 — All findings are NIT-level: deliver Phase 2 fixes as a single bundled commit; Phase 3 still runs, with the improvement scan as the substantive contribution.

EC15 — Smoke test runner not present at tests/run-smoke-tests.sh: log as missing-protocol finding; propose creation per Section 8 spec; do NOT block Phase 3.
  </edge_cases>
</prompt>
```

---

## Generation metadata

| Field | Value |
|---|---|
| Skill | prompt-graph |
| Mode | normal |
| Type D detected | yes (imperative directives + embedded `file://` URIs) |
| INVENTORY items | 23 |
| Contracts (primary + anti-conformity) | 13 + 3 = 16 |
| Conflicts | 0 |
| Coherence advisory | not triggered (all HIGH weaknesses ≥2-mapped) |
| Verification | PASS (preservation 6a–6e, fidelity 6f, quality 6g–6l) |
| Synthesis spawns used | 0 / 2 |
| Repair attempts | 0 |
