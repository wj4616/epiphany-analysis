<prompt>

<meta>
  <source>epiphany-graph-genius skill improvement audit — session 2026-04-25T00-24-35-7c6b</source>
  <version>prompt-graph normal mode | session-driven analysis</version>
  <input_type>skill improvement task with evidence from completed run</input_type>
</meta>

<role>
  You are a skill-improvement agent for the `epiphany-graph-genius` cognitive pipeline skill.
  You have access to a completed STANDARD-mode session and the skill's full source.
  Your deliverable is a classified finding registry (Tier 1 bugs + Tier 2 improvements)
  followed by direct implementation — edits to SKILL.md, module files, and graph.json
  where warranted. Analysis without implementation is an incomplete deliverable.

  Ground every finding in a session artifact. Do not propose changes that aren't
  supported by observed session behavior or a traceable design-spec discrepancy.
</role>

<context>

  <session_evidence>
    Session directory: /home/myuser/docs/epiphany/graph-genius/2026-04-25T00-24-35-7c6b/
    Scale: STANDARD (no modifier flags)
    Input: 457-line XML-structured implementation prompt (procedural frame: 5-phase task)
    Executed nodes: [N1, N3, N5, N2, N6, N8, N7, N9]
    Skipped by design: N4 (DEEP only), N3.1 (not triggered), N6.1 (--conjecture only)
    spawns_total: 4 (AT the STANDARD cap ≤4)
    wall_seconds: 1781 (29.7 min — exceeds STANDARD ≤25 min specification)
    Test battery: 5/5 PASS (T1–T5)
    Verification: 6 PASS / 1 PARTIAL (V7 flagged Procedural→Declarative frame shift)
  </session_evidence>

  <design_vs_actual_table>
    | Dimension           | Expected (SKILL.md spec) | Observed (session)      | Delta | Class  |
    |---------------------|--------------------------|-------------------------|-------|--------|
    | Wall-clock STANDARD | ≤25 min                  | 29.7 min                | +19%  | Bug    |
    | SKILL.md version    | v1.0.0                   | v1.0.0                  | —     | OK     |
    | N9 output version   | (matches skill)          | v1.1.0 hardcoded        | mismatch | Bug |
    | N9 section map      | N3.1 OR V7 → Rep.Signals | "N3.1 only" in map     | gap   | Bug    |
    | Spawns total        | ≤4 (STANDARD)            | 4                       | at cap | OK   |
    | N4 skip marker      | canonical form           | correct form used       | —     | OK     |
    | Rep. Signals section| present (V7 triggered)   | present + correct       | —     | OK     |
    | Report Coherence Signals | ~300 words          | substantially duplicates Independence-Verified Bridges | redundancy | Gap |
    | Cognitive trait attribution | visible in output | --verbose only (Section 13) | not default | Gap |
    | Frame-shift translation | V7 flags + V6 notes | flagged but not translated | no bridge | Gap |
    | Audit-fix annotations in SKILL.md | clean protocol | 15+ inline maintenance tags | noise | Thin |
    | Section 3f O8 budget | applicable rule | says "does not trip in v1" | dead code | Thin |
  </design_vs_actual_table>

  <skill_source>
    Skill: /home/myuser/.claude/skills/epiphany-graph-genius/
    SKILL.md: orchestrator protocol (STEP 0–6)
    Modules: N1.md through N9.md — per-node protocols
    graph.json: topology, scale gates, exec types
    kb/: 18 KB files read by nodes during execution
  </skill_source>

</context>

<task>

  Execute the following analysis + implementation phases. Each phase must complete
  before the next begins. For each finding: assign ID (B1…Bn for bugs, G1…Gn for
  gaps, T1…Tn for thin, I1…In for improvements), cite source evidence, propose fix.

  PHASE 1 — Classify Tier 1 Findings (Bugs + Documentation Errors)
  ─────────────────────────────────────────────────────────────────
  Tier 1 = must fix before next use. Criteria: wrong behavior, wrong documentation
  that would mislead an agent following the protocol.

  Known candidates from session evidence:
  B1 — Wall-clock exceeds STANDARD target (1781s observed vs ≤1500s specified).
       Fix scope: STEP 6 summary line; add wall_seconds vs target comparison;
       emit advisory if exceeded. Also consider revising the target in SKILL.md
       to reflect real measured times on complex inputs.
  B2 — Version mismatch: SKILL.md frontmatter says v1.0.0; N9.md pipeline header
       hardcodes v1.1.0. One must match the other. The N9 "v1.1.0 additions"
       section adds Representational Signals + creativity type — these are real
       functional additions. Recommended fix: bump SKILL.md to v1.1.0.
  B3 — N9.md Section Map shows "N3.1 (if didn't run, skip section entirely)" for
       Representational Signals, but the actual trigger is "N3.1 ran OR V7 flagged
       shift." The session correctly included the section (V7 triggered), but the
       section map documentation is wrong and would mislead an agent.
       Fix: update N9.md section map to read:
       "| 11. Representational Signals (conditional) | N3.1, V7 (include if N3.1 ran
       OR V7 flagged shift; skip entirely otherwise) |"

  Discover any additional Tier 1 candidates by reading all module files.

  PHASE 2 — Classify Tier 2 Findings (Value Gaps)
  ─────────────────────────────────────────────────
  Tier 2 = significant output quality improvements. Criteria: the skill could produce
  more actionable output for the user with bounded additions to module protocols.

  Known candidates:
  G1 — Frame-shift translation gap: V7 correctly flagged Procedural→Declarative shift
       but the report provides no translation back. For procedural inputs, the user
       needs the architectural analysis mapped to implementation phases. Fix scope:
       N9.md — add conditional "Implementation Bridges" section triggered when V7
       flags a Procedural→Declarative shift (same trigger as Representational Signals).
       Content: for each convergence signal (CS1–CS5), one sentence mapping the
       architectural insight to a specific phase or implementation decision.
  G2 — Cognitive trait attribution invisible in default output. The skill uses
       Tesla/Feynman/Newton/Ramanujan/Von Neumann mechanisms but they only appear
       in --verbose mode. Fix scope: N9.md — add 3–5 lines in Headline Insight
       section attributing the top 2 cognitive traits applied (e.g., "Tesla
       constructive test (N1) → measurement plan" and "Von Neumann integration
       (N7) → 4-source convergence"). This makes the GoT cognitive amplification
       visible without the verbosity of full Section 13.
  G3 — Coherence Signals section substantially duplicates Independence-Verified
       Bridges. Both sections list the same CS1–CS5 signals with cross-domain
       recurrences. Fix scope: N9.md — make Coherence Signals a compressed
       summary (2–3 lines per signal, no duplication of bridge detail) that adds
       the "unexpected symmetry" framing that Bridges doesn't have.

  Discover additional value gaps by comparing report.md sections against what an
  implementation agent would actually need from this analysis.

  PHASE 3 — Thin Explanation Audit
  ──────────────────────────────────
  Apply the operationalized "thin" test to each module and SKILL.md section:
    (a) Does the protocol specify HOW to perform the cognitive operation (not just WHAT)?
    (b) Does the role declaration have a clear cognitive trait attribution?
    (c) Does the output section definition specify minimum-depth and non-fabrication rules?
  A section is thin if it fails ≥2 of the above.

  Known candidates:
  T1 — SKILL.md Section 3f (O8 budget check): explicitly says "does not trip in v1."
       12 lines of dead documentation. Fix: condense to a 2-line forward-compatibility
       note; remove the implementation detail that doesn't apply.
  T2 — SKILL.md contains 15+ inline audit-fix annotations (F-06, F-07, M1, A1, A2,
       A3, M3, M4, B3, L2, etc.). These are maintenance changelogs that add cognitive
       load. Fix: extract to a new CHANGELOG.md in the skill directory; replace inline
       references with a single pointer: "(see CHANGELOG.md for audit history)."
  T3 — N7.md MACHINE ADVANTAGE section and GENIUS PERSPECTIVES section document
       which genius traits are applied and where, but this attribution never flows
       through to the N9 output except in --verbose mode. The modules correctly
       specify the traits but the signal doesn't propagate. Fix: add a "trait_applications"
       optional field to the integration_digest schema in N7.md SIGNAL OUTPUT,
       listing the 2 most distinctive trait applications. N9 reads this field and
       surfaces it in the Headline Insight section.

  PHASE 4 — GoT Topology Optimization Analysis
  ──────────────────────────────────────────────
  Analyze the current graph topology against the user's directive to "exploit graph
  nodes GoT system in AI agent skills" most effectively.

  For each node pair where information flow is currently absent but would be valuable:
    - State the missing edge
    - State what the downstream node would gain
    - State whether this requires graph.json modification

  Known candidate:
  I1 — N5 (precision-forcing) currently receives only N1 output (via required edge).
       N5 runs in the same wave as N3 and N2 (per session: all complete before N7).
       If N5 received N3's activation_digest as an optional input, N5's pathological
       case enumeration would be informed by the activated concept nodes (e.g., knowing
       "Multi-Backend-Abstraction" activated would direct N5 to specifically stress-test
       the backend-switching invariant). This would make N5's precision-forcing targeted
       rather than general.
       Fix scope: graph.json — add optional edge N3→N5 with signal_field "activation_digest".
       N5.md — add optional input protocol: "If N3 activation_digest is available, read
       it first. Use activated nodes as precision-forcing targets — formalize claims
       about those specific concepts first."
       Tradeoff: adds topology dependency; N5 must wait for N3 in any wave where N3 runs.
       At STANDARD scale, N3 and N5 currently fire in the same wave; making N5 dependent
       on N3 serializes them. Acceptable if the quality gain exceeds the latency cost.

  I2 — N9 synthesis does not explicitly model inter-node relationships — it treats all
       node outputs as independent sources. A true GoT synthesis would map how N2's
       emergent structures EXTEND N3's activation, how N6's falsification TESTS N5's
       precision claims. Fix scope: N9.md protocol Step 1 — add a "dependency trace"
       sub-step: for each finding in the report, trace its chain (which node first
       raised it, which subsequent nodes confirmed/challenged it). This makes the
       GoT graph structure visible in the output.

  PHASE 5 — Implement All Tier 1 Fixes
  ──────────────────────────────────────
  For each Tier 1 finding (B1, B2, B3, plus any discovered): implement the fix
  directly. Use Edit tool to modify the relevant file. Verify each fix with the
  specific check that would detect the original bug.

  PHASE 6 — Implement Selected Tier 2 Improvements
  ──────────────────────────────────────────────────
  For each Tier 2 finding: assess implementation complexity (simple = ≤10 lines
  changed; moderate = module protocol change; complex = graph.json topology change).
  Implement all simple and moderate improvements in this session.
  For complex improvements (I1 N5 optional dependency): document the change proposal
  in a IMPROVEMENT-BACKLOG.md file in the skill directory.

  PHASE 7 — Methodology Documentation
  ─────────────────────────────────────
  Write a SDIR-METHODOLOGY.md (Session-Driven Iterative Refinement) in the skill
  directory. Contents:
    - 6-step methodology for future skill improvement runs
    - Classification taxonomy (bug/gap/thin/improvement) with criteria
    - The Design vs Actual comparison table template (from context above)
    - The "thin test" (3-criteria operationalization from Phase 3)
    - Which session artifacts to read in what order for efficient triage

</task>

<constraints>
  ALL_FINDINGS_GROUNDED
    Every finding must cite a specific session artifact, file path, and observed value.
    "Might be a problem" without evidence is not a valid finding.

  SCOPE_BOUNDED
    Tier 1 fixes: implement fully in this session.
    Tier 2 improvements: implement if ≤10 lines changed per improvement; otherwise document.
    Complex GoT topology changes (graph.json edges): document only; do not modify graph.json
    without running the modified topology through PRC1 validation.

  NO_REGRESSION
    Any change to a module file must verify that the existing test battery (T1–T5 in
    test-runner.sh) still passes on the modified skill. If a test would need updating due
    to the fix, update the test and document why.

  EVIDENCE_FIRST
    For each phase, read the relevant file BEFORE proposing the fix. Do not propose changes
    based only on the analysis above — verify the current state of the file first.

  METHODOLOGY_LAST
    SDIR-METHODOLOGY.md is written last, after all finding classification is complete,
    so it accurately reflects the process that was actually followed.
</constraints>

<output_format>
  SECTION 1 — Finding Registry
    A table with columns: ID | Class | File | Section | Evidence | Proposed Fix | Status
    Rows ordered: Tier 1 (B, T) first, then Tier 2 (G, I).

  SECTION 2 — Implemented Changes
    For each implemented fix: file path, before/after diff (3–5 lines context),
    and a one-line verification check.

  SECTION 3 — GoT Topology Proposals (not yet implemented)
    For each complex improvement: topology diagram fragment showing the new edge,
    rationale, estimated quality gain, and implementation prerequisites.

  SECTION 4 — SDIR Methodology
    The 6-step methodology in a compact, reusable format.
</output_format>

<edge_cases>
  EC-1 — Wall-clock target is aspirational, not a hard constraint.
    The ≤25 min STANDARD target reflects typical inputs. The session input was
    unusually dense (457 lines, XML-structured). Distinguish between "the estimate
    is wrong" vs "this input is above the typical complexity threshold." Fix the
    estimate if it's consistently wrong; add a complexity advisory if it's
    input-dependent.

  EC-2 — Version bump requires consistency across all files.
    If SKILL.md is bumped to v1.1.0, check: (a) graph.json has a version field?
    (b) README.md mentions a version? (c) kb-registry.json references a version?
    All must be consistent.

  EC-3 — Removing audit-fix annotations must not lose the audit rationale.
    Some audit-fix annotations (A1 "back-edge-only-target rule — CRITICAL") carry
    important behavioral context. When moving to CHANGELOG.md, the SKILL.md must
    retain a pointer or brief summary for the most critical ones. Do NOT silently
    delete the rationale.

  EC-4 — The N3→N5 optional edge creates a wave serialization.
    At STANDARD scale, N3 and N5 currently fire in parallel (same ready-set wave).
    Adding N3→N5 (optional) does NOT block N5 — optional inputs don't prevent
    activation per the ready-set rule. N5 would fire with or without N3's output;
    if N3 finishes first in the same wave, N5 can use it on the next fixpoint
    iteration. Verify this is correct behavior before implementing.
</edge_cases>

<verification>
  [ ] 1. All Tier 1 findings are implemented (B1 wall-clock advisory, B2 version bump,
         B3 N9 section map fix).
  [ ] 2. SKILL.md version matches N9 module version after fix.
  [ ] 3. N9.md section map correctly states "N3.1 OR V7" as the Representational Signals trigger.
  [ ] 4. CHANGELOG.md exists in skill directory and contains all previously-inline audit-fix entries.
  [ ] 5. Section 3f in SKILL.md is condensed to ≤3 lines.
  [ ] 6. N9.md Headline Insight section includes cognitive trait attribution (2 trait applications).
  [ ] 7. N9.md Implementation Bridges section defined and correctly triggered by V7 Procedural shift.
  [ ] 8. SDIR-METHODOLOGY.md written and contains the 6-step process + classification taxonomy.
  [ ] 9. No test in test-runner.sh fails after the above changes.
  [ ] 10. GoT topology proposals (N3→N5 edge) documented in IMPROVEMENT-BACKLOG.md.
</verification>

</prompt>
