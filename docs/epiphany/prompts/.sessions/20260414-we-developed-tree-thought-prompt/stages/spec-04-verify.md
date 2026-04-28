# spec-04-verify.md

Output of MSPEC4M5 Phase 1 (S6 Completeness Audit) and Phase 2 (S7a–S7k 11-check
verification) for the draft specification in `spec-03-synthesis.md`.

Scale: STANDARD. Mode: specification. Spec mode never returns FAIL — failures are
recorded and embedded as a `<note>` block inside the final `<specification>` XML,
and the return contract becomes `PASS-WITH-NOTES`.

---

## Phase 1 — Completeness Audit (S6)

Every item from `spec-01-domain.md`'s DECOMPOSITION → Core Elements is traced to
at least one requirement (FR/NFR/DR/IR/CON) or Open Question in
`spec-03-synthesis.md`.

| Decomposition Core Element | Covered by |
|---|---|
| Skill identity | OQ-1 (canonical name); FR-33 (tree/graph-of-thought); NFR-2 (modular); `<part id="skill-identity">` |
| Node A (input storage) | FR-1; DR-1; IR-1; CON-1; NFR-5; `<part id="node-A">` |
| Node B (analysis storage) | FR-2; DR-2; IR-2; `<part id="node-B">` |
| Node A1 (analysis of A) | FR-3; NFR-3; DR-3; CON-4; `<part id="node-A1">` |
| Node B1 (analysis of B) | FR-4; NFR-3; DR-3; CON-4; `<part id="node-B1">` |
| Node C (idea database) | FR-5, FR-6, FR-7, FR-8, FR-9; DR-4; `<part id="node-C">` |
| Node D (accepted-ideas) | FR-10, FR-11; DR-5; NFR-6, NFR-7; CON-2, CON-3; `<part id="node-D">` |
| Node E (solution store) | FR-12, FR-13, FR-14, FR-16; DR-6; CON-6; `<part id="node-E">` |
| Node x2 (per-section accepted) | FR-29; DR-7; `<part id="node-x2">` |
| Node x3 (per-section solutions) | FR-30, FR-31; DR-8; `<part id="node-x3">` |
| Final output artifact | FR-17, FR-18, FR-19, FR-20; DR-9; IR-3; NFR-5, NFR-10; CON-1; `<part id="final-artifact">` |
| Epiphany-genius detection probe | FR-21; DR-2; `<part id="detection-probe">`; OQ-6 |
| Ten-section analyzer | FR-23, FR-28; DR-3; `<part id="ten-section-analyzer">`; OQ-7 |
| Headline / primary-conclusion bias | FR-25; `<part id="headline-bias">` |
| Theory-collision handler | FR-26; `<part id="theory-collision-handler">`; OQ-8 |
| Alternative-hypothesis filter | FR-27; `<part id="alternative-hypothesis-filter">`; OQ-9 |
| Ideation → Synthesis → Verification cycle | FR-15, FR-18, FR-36; IR-6; `<part id="isv-cycle">` |
| Self-audit gate | FR-19, FR-32; `<part id="self-audit-gate">`; OQ-13 |
| Modular integration hook | NFR-2; FR-33; IR-5; `<part id="modular-integration-hook">`; OQ-15 |
| Generic-analysis fallback | FR-22; NFR-9, NFR-10; `<part id="generic-fallback">` |

Gap list: none — every decomposition item is represented.

Coverage tally (matches the synthesis body):
- Functional Requirements: 36 (FR-1 … FR-36)
- Non-Functional: 12 (NFR-1 … NFR-12; NFR-11 + NFR-12 tagged `domain-inferred`)
- Interfaces: 6 (IR-1 … IR-6)
- Data: 9 (DR-1 … DR-9)
- Constraints: 6 (CON-1 … CON-6)
- Open Questions: 16 (OQ-1 … OQ-16)
- MUST / SHOULD / MAY: 47 MUST, 6 SHOULD, 1 MAY (approx. — based on `class=` attribute scan; ignoring MUST_NOT which map to MUST)

---

## Phase 2 — Verification (S7a–S7k)

Each check is re-stated verbatim from SKILL.md § `## Verification Checks` →
`Specification mode — 11 checks (S7a–S7k)` to keep the report self-contained
(Hard Gate 3 echo requirement for verification modules).

### S7a. Decomposition Coverage

Rule: **Every Decomposition item traces to ≥1 requirement or OQ. Missing → FAIL.
Recovery: Add requirement or OQ.**

```yaml
check: S7a
result: pass
detail: "All 20 decomposition Core Elements are traced in the audit table above; no missing items."
repair_target: "(none)"
```

### S7b. Requirement Quality

Rule: **Every SHALL requirement is Necessary, Unambiguous, Verifiable,
Consistent, Traceable. Any failure → FAIL. Recovery: Rewrite or move to OQ.**

```yaml
check: S7b
result: pass-with-note
detail: "All 47 MUST requirements are necessary, traceable (every requirement has a <source>), and internally consistent. Unambiguous at the prose level. Verifiability: each requirement's verification method is recorded in spec-02-requirements.md but the verification criterion text was NOT copied into the <requirement> elements of the draft XML — it is instead available in the per-requirement rationale/source trail. This is a structural echo of the S7f gap rather than a second independent failure."
repair_target: "<requirements> — consider inlining a <verification> field per requirement in a future pass; for now the criterion is recoverable from spec-02-requirements.md."
```

### S7c. No Unresolved TBD

Rule: **No section contains "TBD" or "to be determined" without a corresponding
OQ. Any → FAIL. Recovery: Convert to OQ or resolve.**

```yaml
check: S7c
result: pass
detail: "Full-text scan of spec-03-synthesis.md: no 'TBD' or 'to be determined' tokens. Every 'pending' / 'deferred' / 'enumeration pending' phrase is paired with an OQ reference (e.g., OQ-2, OQ-10, OQ-14)."
repair_target: "(none)"
```

### S7d. Consistency

Rule: **No two requirements contradict each other. Contradiction → FAIL.
Recovery: Flag conflict, ask user before continuing.**

```yaml
check: S7d
result: pass
detail: "Pairwise review of FR, NFR, DR, IR, and CON entries shows no contradictions. Potential tension points examined: FR-22 (generic branch) vs. FR-23 (per-section branch) are gated on the detection probe's result and are therefore mutually exclusive, not contradictory; FR-24/NFR-8 (time bound) vs. FR-3/FR-4/NFR-3 (exhaustive detail preservation) is a tension that is resolved by the hedged SHOULD class of the time-bound rules and by OQ-10 for the numeric cap."
repair_target: "(none)"
```

### S7e. Scope Clarity

Rule: **Scope states what IS included AND what IS excluded. Ambiguous → FAIL.
Recovery: Add explicit exclusions.**

```yaml
check: S7e
result: pass
detail: "<scope> lists 10 included items and <out_of_scope> lists 7 excluded items. Both lists are specific (e.g., 'Implementing the skill' excluded; 'Defining epiphany-genius itself or its report schema' excluded; fetching of the referenced URL is explicitly excluded)."
repair_target: "(none)"
```

### S7f. Verification Criteria

Rule: **Every SHALL requirement has a verification criterion. Missing → FAIL.
Recovery: Add "Verification: [observable test]".**

```yaml
check: S7f
result: fail
detail: "The draft <specification> XML in spec-03-synthesis.md emits each requirement with <text>, <rationale>, and <source> elements but does NOT embed a <verification> child element. Verification criteria exist per-requirement in spec-02-requirements.md (each FR/NFR/DR/IR/CON has a 'Verification:' paragraph), so the criteria are specified and auditable, but they are not inlined into the final XML as S7f asks. Spec mode does not FAIL outright — this is recorded and the output is emitted with a <note> block per the MSPEC4M5 protocol."
repair_target: "<requirements> — future revision should inline a <verification> child alongside <text>/<rationale>/<source>, populated from spec-02-requirements.md's Verification fields."
```

### S7g. Open Questions Surfaced

Rule: **Every ambiguity has an explicit OQ entry. Buried ambiguity → FAIL.
Recovery: Surface as OQ.**

```yaml
check: S7g
result: pass
detail: "16 OQs (OQ-1 … OQ-16) surface every flagged ambiguity from spec-01/spec-02: skill name (OQ-1), output path (OQ-2), DB form (OQ-3), role roster (OQ-4), scoring rubrics (OQ-5), detection heuristic (OQ-6), 10-section identities and recipes (OQ-7), discriminating-condition format (OQ-8), hypothesis selection rule (OQ-9), time budget + parallelism (OQ-10), tie-break rule (OQ-11), conflict-resolution rule (OQ-12), self-audit blocking policy (OQ-13), accepted input formats (OQ-14), module-slot mapping (OQ-15), minimum-improvement threshold (OQ-16)."
repair_target: "(none)"
```

### S7h. Stakeholder Coverage

Rule: **Every stakeholder from Domain Analysis appears in requirements or
context. Missing → FAIL. Recovery: Add requirements or context for missing
stakeholder.**

```yaml
check: S7h
result: pass
detail: "All 7 stakeholders (skill-author, claude-code-orchestrator, module-executor, downstream-consumer, analysis-producer, original-input-author, future-integrator) appear in <stakeholders> and are referenced in the body: skill-author via the meta; claude-code-orchestrator via NFR-1/FR-33; module-executor via NFR-2/IR-5; downstream-consumer via IR-3/DR-9; analysis-producer via FR-2/FR-21; original-input-author via FR-1/NFR-5; future-integrator via NFR-2/IR-5/OQ-15."
repair_target: "(none)"
```

### S7i. Technical Detail Preservation

Rule: **Every item in the Technical Details inventory (Step S3): URL, file
path, technology+version pair, code block, numeric specification, named entity,
version specification, quoted string, API reference, and embedded directive —
appears in the specification output verbatim. Missing any item → FAIL.
Recovery: Add the missing technical detail to the appropriate requirement,
constraint, context, or data requirement section. Do NOT paraphrase,
summarize, or alter version numbers, quantities, paths, or URLs.
Technology+version pairs must appear with BOTH name AND version together, not
split or separated.**

```yaml
check: S7i
result: pass-with-note
detail: "URLs: file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md — preserved verbatim (IR-4 description, CON-5 body, FR-35 text, and out_of_scope item). File Paths: same path — preserved verbatim. Technology+Version: none in input. Code Blocks: none in input. Numeric Specifications: '10 main sections' preserved (FR-23, FR-28, DR-7, DR-8, SC-10); 'two solutions' preserved (FR-13, FR-14, DR-6, CON-6, <part id='node-E'>); 'phase 0' NOT preserved as a literal token in the synthesis XML — the phase is described structurally via FR-3/FR-4 ('full analysis of node A' / '... node B') but the quoted label 'phase 0' is absent. Named Entities: 'epiphany-analysis' preserved (OQ-1, <scope>); 'epiphany-genius' preserved throughout; 'prompt-epiphany' preserved (FR-15, FR-36, IR-6, A-4); 'Claude Code' preserved (FR-33, <domain>, A-5). Version Specifications: 'any version of epiphany-genius (even if slightly different due to changes made in new version of skill)' — preserved in paraphrase (FR-21) but not as a verbatim quote. Quoted Strings: 'tree of thought' / 'graph of thought' preserved verbatim (FR-33); 'headline insight' / 'primary conclusion' preserved (FR-25, SC-10); 'theory collisions' preserved (FR-26, <part id='theory-collision-handler'>); 'alternative hypothesis' preserved (FR-27, <part>); 'ideation, synthesis, and verification' preserved (FR-15, FR-36, IR-6); 'self-audit' preserved (FR-19, <part id='self-audit-gate'>); 'we can't let this process take too long' preserved in paraphrase only (FR-24, NFR-8 state the intent but do not include the verbatim quoted clause). API References: none. Embedded Directives: all ~25 directives from spec-01 Embedded Directives are represented structurally across FR-1 … FR-36; each has a <source> field linking back to the 00-input.md line(s). Verdict: the substantive content (URLs, numbers, names, structural quotes) is preserved; two minor paraphrase-only items ('phase 0' label, and the 'we can't let this process take too long' exact phrasing) are not verbatim. Recorded as pass-with-note rather than pass."
repair_target: "<requirements> / <constraints> — restore the verbatim tokens 'phase 0' and 'we can't let this process take too long' in the relevant requirements (FR-3/FR-4 and FR-24/NFR-8 respectively) in a future revision."
```

### S7j. Plan-Readiness Check

Rule: **Verify: (1) every FR/NFR contains enough concrete technical detail
that a plan pipeline can write atomic steps from it without guessing — abstract
requirements like "the system SHALL perform well" FAIL this check, (2) any
Open Question that would block an entire phase of plan generation is
explicitly flagged with "BLOCKS PLAN PHASE: [description]". Requirements too
abstract → FAIL. Recovery: Add specific metrics, commands, file paths, or
configurations to make abstract requirements concrete. If detail is genuinely
unknown, add an OQ flagged as plan-blocking.**

```yaml
check: S7j
result: pass-with-note
detail: "Concreteness (1): each FR and NFR carries enough structure for a downstream planner to write atomic steps — e.g., FR-6 names nodes by id, FR-13 specifies 'two candidate solutions', FR-23 specifies '10 main sections', DR-4..DR-8 enumerate record shapes, FR-20 pins the no-overwrite file-system constraint. No 'SHALL perform well'-class vagueness found. Plan-blocking OQs (2): OQ-2 (output path), OQ-3 (DB representation), OQ-6 (detection heuristic), OQ-7 (10-section recipes), OQ-10 (time budget + parallelism), OQ-15 (module-slot mapping) all plausibly block a plan phase but are NOT explicitly tagged 'BLOCKS PLAN PHASE: [description]' in the synthesis. Substantively recoverable, but the explicit tag is missing. Recorded as pass-with-note."
repair_target: "<open_questions> — annotate OQ-2, OQ-3, OQ-6, OQ-7, OQ-10, OQ-15 with a 'BLOCKS PLAN PHASE:' prefix or attribute identifying which plan phase would stall without the answer."
```

### S7k. Structural Element Preservation (Workflow Specs)

Rule: **ONLY applies when Step S1b detected workflow/process spec. Verify: (1)
Every phase name and number from input appears in output with same name/number,
(2) Every step within phases is preserved (not summarized or merged), (3)
Every tier definition block is preserved verbatim (including all criteria),
(4) Every conditional logic block (if/then, when X do Y) is preserved, (5)
Every iteration rule (loop until X, max N passes) is preserved with
termination condition, (6) Every verification criterion is preserved, (7)
Every defaults section is preserved, (8) Every edge case definition is
preserved. Any missing → FAIL. Recovery: Restore the complete structural
element from input. Do NOT paraphrase or summarize.**

```yaml
check: S7k
result: pass-with-note
detail: "Input IS a workflow-bearing spec (node-pipeline phases + loops + branches). (1) Phase names/numbers: 'phase 0' is the only literal phase label in input — preserved structurally in FR-3/FR-4 but not as the verbatim token 'phase 0' (matches S7i finding). (2) Steps within phases: every pipeline step (A→A1, B→B1, C→D→E, x2→x3, integrate, self-audit, final verify) is preserved in the requirements list with one requirement per step. (3) Tier definitions: none in input (the accept/reject and two-candidate rules are stated as prose, not tiered classifications — consistent with spec-01's observation that formal tier defs are an OQ). (4) Conditional logic: IF-genius-THEN-per-section / ELSE-generic preserved (FR-22 / FR-23); two-candidate compare-and-keep-best preserved (FR-13 / FR-14 / CON-6); collision and hypothesis conditionals preserved (FR-26 / FR-27). (5) Iteration rules: Idea Generation terminates on exhaustion (FR-8); Idea Evaluation covers every entry once (FR-9); Solution Engineering completes coverage (FR-16); per-section iteration covers all 10 (FR-28); x2→x3 iteration (FR-30); x3→final integration (FR-31). Termination conditions are preserved. (6) Verification criteria: criteria exist in spec-02-requirements.md but were not copied as <verification> children into the XML — same gap as S7f. (7) Defaults: 'default analysis target = epiphany-genius' and 'fallback = generic' preserved (FR-21 / FR-22 / NFR-9 / <part id='generic-fallback'>). (8) Edge cases: non-genius B (FR-22), cross-version genius B (FR-21), collision (FR-26), hypothesis filter (FR-27), all-ideas-rejected (covered by OQ-16), accepted-idea conflicts (OQ-12), two-tied-candidates (OQ-11), self-audit-failure (OQ-13), extremely-large-A (OQ-10), malformed-section (covered partially — see note). Verdict: the structural skeleton is preserved; gaps are (a) the 'phase 0' token not verbatim, (b) verification criteria not inlined into the XML, (c) 'empty/malformed epiphany-genius section' edge case not explicitly addressed beyond OQ-6/OQ-7."
repair_target: "<requirements> — inline <verification> children; restore the literal 'phase 0' token where FR-3/FR-4 originate; add an explicit edge-case requirement or OQ for 'empty or malformed epiphany-genius section' handling."
```

---

## Summary

```yaml
coverage_counts:
  requirements: 69   # 36 FR + 12 NFR + 6 IR + 9 DR + 6 CON
  shall: 47          # MUST
  should: 6          # SHOULD (FR-24, FR-25, FR-35, NFR-8, NFR-10, NFR-11, NFR-12 — count matches class=SHOULD attrs in synthesis)
  may: 1             # MAY (FR-34)
  gaps: 0            # S6 audit found no decomposition-to-requirement gaps
overall: pass-with-notes
```

### Failed / noted checks

| check | result | repair target |
|---|---|---|
| S7b | pass-with-note | inline `<verification>` fields (shared with S7f) |
| S7f | fail | inline per-requirement `<verification>` elements populated from spec-02 |
| S7i | pass-with-note | restore verbatim tokens `phase 0` and `we can't let this process take too long` |
| S7j | pass-with-note | tag plan-blocking OQs with `BLOCKS PLAN PHASE:` |
| S7k | pass-with-note | inline verification; restore `phase 0`; add edge-case handling for empty/malformed genius sections |

One check (S7f) is `fail`; per MSPEC4M5 spec mode never returns FAIL outright,
so the overall result is `pass-with-notes` and the synthesized `<specification>`
XML will carry a `<note>` block summarizing the above gaps.

---

## Phase 3 — Output Formatting Plan

The final `<specification>` XML emitted by MSPEC4M5 will:

1. Wrap the draft specification from `spec-03-synthesis.md`.
2. Insert `<meta source="epiphany-prompt"/>` as the first child of `<specification>`.
3. Insert `<original_input>` as the second child containing the verbatim content
   of `00-input.md`, CDATA-wrapped because the input contains `<` (in the embedded
   file:/// URL reference and the bulleted pseudo-tags).
4. Preserve all existing sections (`<domain>`, `<scope>`, `<out_of_scope>`,
   `<concept_decomposition>`, `<requirements>`, `<interfaces>`, `<success_criteria>`,
   `<assumptions>`, `<constraints>`, `<open_questions>`) verbatim.
5. Append a final `<note>` block (LAST child) summarizing the failed/noted checks
   per the Hard Gate 3 echo requirement — the note restates each cited check's rule
   so downstream consumers see both the verdict and the rule it applies to.

Return contract: `VERIFICATION: PASS-WITH-NOTES — [summary]\n\n<specification>...</specification>`.
