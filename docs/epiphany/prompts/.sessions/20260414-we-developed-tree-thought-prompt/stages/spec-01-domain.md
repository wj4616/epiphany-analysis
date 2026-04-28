# spec-01-domain.md

Output of MSPEC12 Phase 1 — Domain Analysis (S2) and Concept Decomposition (S3).
Scale: STANDARD. Mode: specification. Input type: A (Concept/Idea hybrid with embedded workflow outline).

---

## S2 — DOMAIN

```
DOMAIN:
  Field: software (AI agent skill / prompt-engineering pipeline for Claude Code)
  Stakeholders:
    - Skill author (user): designs and maintains the epiphany-* family of skills
    - Claude Code orchestrator: the AI agent that will execute the new skill's modules
    - Subagents / module executors: individual modules invoked programmatically per-stage
    - Downstream consumers: any human or skill that receives the improved input as output
    - Analysis producer: epiphany-genius skill (primary), OR any other analysis method (secondary)
    - Original-input author: the human or system that produced node A (the input being improved)
    - Future integrator: engineer who will plug this skill into epiphany-genius modular system later
  Scope — Included:
    - Define a NEW skill (working name: epiphany-analysis) that accepts (input, analysis-of-input) → (improved input)
    - Specify a tree-of-thought / graph-of-thought pipeline over nodes A, B, A1, B1, C, D, E
    - Specify an optional extended pipeline over x2, x3 nodes for per-section analysis branches
    - Specify how the skill detects whether the analysis was produced by any version of epiphany-genius
    - Specify section-specific analysis paths for the 10 main sections of an epiphany-genius report
    - Specify the ideation → synthesis → verification cycle borrowed from prompt-epiphany
    - Specify the fresh-copy output rule (never overwrite the original input file)
    - Specify modular design enabling future integration into epiphany-genius modular system
  Scope — Excluded:
    - Implementing the skill (this is specification only — no code, no module files)
    - Defining epiphany-genius itself or its report schema (treated as external contract)
    - Fetching or re-analyzing the referenced analysis report at
      file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md
      (the URL is preserved as an example-input pointer only, never dereferenced by this spec)
    - UX / prompt wording for the final user-facing interaction
    - Performance tuning targets (latency, token budget ceilings) beyond "not too long"
    - Migration or deprecation of prompt-epiphany itself
  Existing constraints:
    - MUST run programmatically (non-interactive execution by Claude Code harness)
    - MUST be modular (each stage is a replaceable unit, consistent with epiphany-genius modular system)
    - MUST NOT overwrite the original input; MUST save a fresh final copy
    - MUST use ideation + synthesis + verification cycles as in prompt-epiphany
    - MUST perform full self-audit / verification for bug-free integration
    - MUST tolerate analyses NOT produced by epiphany-genius (graceful fallback path)
    - SHOULD finish in bounded time — input warns "we can't let this process take too long"
    - Per-section analyses MUST be independent and optimized per section (10 sections total)
  Success criteria:
    - A final, fresh, enhanced version of node A is produced that integrates ALL accepted
      improvements/solutions from node E, with every change traceable back to a specific
      analysis finding
    - The produced artifact passes a self-audit for: bug-free integration, no regressions
      introduced, no dropped requirements from the original input, no over-engineering
    - The skill's stage contract (nodes A, B, A1, B1, C, D, E, optional x2/x3) is honored
      exactly as described — each node has defined contents, producer, and consumer
    - Ideas that were considered-but-rejected are recorded (not silently dropped), so the
      reasoning trail is auditable
    - The skill detects whether input B was produced by epiphany-genius (any version) and
      branches accordingly without failing if it was not
```

---

## S3 — DECOMPOSITION

```
DECOMPOSITION:
  Core Elements:
    - Skill identity: a new AI agent skill, working name "epiphany-analysis" (or similar).
      Requires: skill name, SKILL.md front-matter, activation triggers, module filenames.
    - Node A (input storage): holds the ORIGINAL context/input provided by the user.
      Requires: ingestion logic that accepts either inline prompt text OR a file path OR
      other user-provided format; persists verbatim without mutation.
    - Node B (analysis storage): holds the analysis document for node A.
      Requires: ingestion logic that accepts the analysis (from epiphany-genius OR any
      other method); persists verbatim; records provenance (is-from-epiphany-genius flag).
    - Node A1 (analysis-of-A): full, detailed analysis of node A's content.
      Requires: exhaustive per-detail review — technical, conceptual, fine, and coarse
      details all preserved; MUST NOT generalize or gloss over any detail.
    - Node B1 (analysis-of-B): full, detailed analysis of node B's content.
      Requires: same exhaustivity rule as A1.
    - Node C (idea database): stores each improvement idea as an individual element.
      Requires: AI-optimized database representation; entries are atomic; schema expands
      and creates relationships as necessary; supports iteration until ideas are exhausted.
    - Node D (accepted-ideas database): stores only ideas that passed the
      accept/discard filter with documented rationale.
      Requires: pro/con/utility/complexity evaluation; explicit accept-or-discard verdict;
      link back to origin entry in node C.
    - Node E (solution store): stores the engineered solution for each accepted idea.
      Requires: per-idea solution design; two-candidate generation rule; pro/con compare;
      pass only the best candidate; link back to entry in node D.
    - Optional node x2 (per-section accepted ideas): extension of node D when per-section
      analysis is active (one x2 stream per analysis section).
    - Optional node x3 (per-section solutions): extension of node E for x2 stream solutions.
    - Final output artifact: a fresh, full, enhanced version of node A integrating all
      solutions from node E and x3. Saved to a NEW file (never overwriting node A).
    - Epiphany-genius detection probe: heuristic that inspects node B to decide if it
      originated from any version of epiphany-genius; tolerates schema drift between versions.
    - Ten-section analyzer: when node B is an epiphany-genius report, each of the 10 main
      sections receives its own section-optimized analysis branch.
    - Headline/primary-conclusion bias: the headline insight or primary conclusion is
      treated as the initial focus area before other sections are processed.
    - Theory-collision handler: for each collision, claim A and claim B are compared under
      the discriminating condition; an enhancement is synthesized from the resolved reading.
    - Alternative-hypothesis filter: most alternative hypotheses are wrong; only one is
      treated as the live candidate (selection criteria needed → OQ).
    - Ideation-synthesis-verification cycle: applied per-idea and on the final integrated
      output, mirroring the prompt-epiphany three-phase cycle.
    - Self-audit gate: final verification for bug-free integration, no errors/issues, no
      regressions, no over-engineering.
    - Modular integration hook: the skill is structured so that each stage maps cleanly
      to an epiphany-genius module slot for future re-engineering.
  Functional Requirements:
    - Accept two inputs: node A (original input) and node B (analysis of node A)
    - Produce node A1 (full, detail-preserving analysis of A)
    - Produce node B1 (full, detail-preserving analysis of B)
    - Generate improvement ideas grounded in B1's insights about A1, one idea per entry in node C
    - Iterate idea generation with varied roles and perspectives until ideas are exhausted
    - Evaluate every idea in node C for utility, relevance, realism, and over-engineering risk
    - Move accepted ideas to node D; keep rejected ideas as discarded-with-reason
    - For each accepted idea, generate TWO candidate solutions
    - Compare the two candidates (pro/con/utility/over-engineering) and keep only the best in node E
    - Integrate all solutions in node E into a single cohesive enhanced version of node A
    - Save the enhanced version to a NEW file — never overwrite node A
    - Detect whether node B is an epiphany-genius report (any version) and branch accordingly
    - When detected, analyze each of the 10 main sections independently with a section-tailored path
    - Prioritize the headline insight / primary conclusion as the first area of enhancement focus
    - Handle theory collisions: compare claim A vs. claim B under the discriminating condition
    - Handle alternative hypotheses: select the single correct candidate; ignore the rest
    - Apply ideation → synthesis → verification cycles per idea and on the final integrated output
    - Perform a final self-audit for bug-free integration, no regressions, no over-engineering
    - Operate programmatically (no interactive prompts required for normal flow)
    - Be modular so each stage can be invoked, replaced, or re-ordered independently
    - Provide a graceful path for analyses NOT produced by epiphany-genius
  Non-Functional Requirements:
    - Detail preservation: A1 and B1 must not generalize or drop details, including
      technical minutiae (numeric values, file paths, code, API names, etc.)
    - Auditability: every accepted change traces to an idea in C, a decision in D, and a
      solution in E (or x2/x3 equivalents)
    - Modularity: stage boundaries are clean enough to plug into epiphany-genius's modular
      system in a future integration pass
    - Time bound: the overall process should not be excessively long (exact budget → OQ)
    - Robustness: skill succeeds on analyses NOT produced by epiphany-genius, with no crash
    - Determinism of structure: output artifact shape (fresh file, full enhanced content) is
      stable regardless of which branch (genius vs. generic) was taken
    - No data loss: original input in node A is never mutated or overwritten
    - No silent drop: discarded ideas are recorded, not forgotten
    - No over-engineering: accepted solutions must be realistic to implement and not introduce
      complexity disproportionate to the improvement
  Interface Requirements:
    - Input interface: accept node A as (a) inline prompt content, (b) a file path, or (c)
      another user-provided format (format list → OQ)
    - Input interface: accept node B as an analysis document (file path or inline content)
    - Output interface: write the enhanced version of node A to a NEW file at a path
      derived from the original (path convention → OQ)
    - External reference (non-fetched, verbatim): example epiphany-genius analysis at
      file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md
      — used only as a reference for the type of analysis the skill should target
    - Internal interface: each node (A, B, A1, B1, C, D, E, x2, x3) has a defined producer
      stage and consumer stage; modular boundaries are preserved for future integration
      with epiphany-genius
    - Sibling skill reference (non-dependency): design borrows ideation/synthesis/verification
      from prompt-epiphany (documented pattern, not a runtime dependency)
  Data Requirements:
    - Node A: verbatim original input content, format preserved
    - Node B: verbatim analysis document, plus a provenance flag "from-epiphany-genius: bool"
      (and optional "genius-version" when detectable)
    - Node A1: structured analysis of A — sections include technical details, concepts,
      ideas, facets (schema → OQ)
    - Node B1: structured analysis of B — same shape as A1 plus, when applicable, a
      decomposition of B into its 10 main sections
    - Node C: collection of atomic idea records; each record has {id, text, origin-pointer
      (which part of A1/B1 triggered it), generating-role/perspective}
    - Node D: subset of C with {accept/reject verdict, rationale, pro-list, con-list,
      utility-score, complexity-score} — rejected entries retained for audit
    - Node E: per-accepted-idea {candidate-1, candidate-2, comparison, chosen-solution,
      justification}
    - Node x2: per-section accepted ideas (mirrors D, tagged with section id 1..10)
    - Node x3: per-section solutions (mirrors E, tagged with section id 1..10)
    - Final artifact: the full enhanced input, integrated with all chosen solutions, as a
      fresh file separate from node A
  Edge Cases:
    - Node B is NOT an epiphany-genius report — generic-analysis branch activates
    - Node B is an epiphany-genius report from a DIFFERENT version than the skill expects —
      detection must still return true; tolerate schema drift
    - Node B contains theory collisions — invoke collision handler
    - Node B contains alternative hypotheses — invoke hypothesis-selection filter
    - All ideas in C are rejected in D — skill still produces a final artifact that at
      minimum equals node A (or reports "no improvement possible" → OQ)
    - Accepted idea conflicts with another accepted idea — conflict resolution rule → OQ
    - Two candidate solutions are tied on pro/con — tie-break rule → OQ
    - Self-audit finds an integration bug — behavior: block, repair, or surface? → OQ
    - Node A is extremely large — time-budget handling → OQ
    - An epiphany-genius section is empty or malformed — per-section fallback → OQ
  Open Questions:
    - OQ-1: What is the canonical skill name — "epiphany-analysis" or something else?
    - OQ-2: What is the precise output file path convention for the enhanced version?
    - OQ-3: What constitutes the "AI-optimized database" representation for node C (JSON?
      JSONL? in-memory only? persisted to disk?)
    - OQ-4: What roles/perspectives are rotated through during ideation, and how many?
    - OQ-5: What are the concrete utility and complexity scoring rubrics for node D?
    - OQ-6: What is the exact detection heuristic for "is this an epiphany-genius report?"
      Given the skill must tolerate cross-version differences?
    - OQ-7: What are the 10 main sections of the target epiphany-genius report, and what
      is the section-optimized analysis recipe for each?
    - OQ-8: What is the discriminating-condition format for theory-collision records?
    - OQ-9: What is the selection rule for choosing one alternative hypothesis as "correct"?
    - OQ-10: What is the time budget / iteration cap for the "iterate until out of ideas"
      loop?
    - OQ-11: What is the tie-break rule when two candidate solutions in node E are equal?
    - OQ-12: What is the conflict-resolution rule when two accepted ideas contradict?
    - OQ-13: What self-audit failure modes block output vs. surface as warnings?
    - OQ-14: What input formats beyond "inline" and "file path" must node A accept?
    - OQ-15: How does the skill map onto epiphany-genius module slots for future integration?
    - OQ-16: Is there a minimum-improvement threshold below which no enhanced file is produced?
  Technical Details:
    URLs:
      - file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md
    File Paths:
      - file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md
    Technology + Version:
      - (none explicit)
    Code Blocks:
      - (none in input)
    Numeric Specifications:
      - "10 main sections" — count of sections in the target epiphany-genius report
      - "two solutions" — number of candidate solutions generated per accepted idea in node E
      - "phase 0" — the initial analysis phase of A and B
    Named Entities:
      - epiphany-analysis (proposed skill name)
      - epiphany-genius (analysis-producing skill)
      - prompt-epiphany (reference skill supplying the ideation/synthesis/verification pattern)
      - Claude Code (AI agent system the skill runs in)
    Version Specifications:
      - "any version of epiphany-genius (even if slightly different due to changes made in
        new version of skill)" — cross-version tolerance requirement, no specific version
    Quoted Strings:
      - "tree of thought"
      - "graph of thought"
      - "we can't let this process take too long"
      - "headline insight or primary conclusion"
      - "theory collisions"
      - "alternative hypothesis"
      - "ideation, synthesis, and verification"
      - "self-audit"
    API References:
      - (none)
    Embedded Directives:
      - "store original context provided by user into node A" — ingestion rule for node A
      - "node a is the input. it could be specified directly in the user prompt context, or
        be stored in a file or other format provided by the user" — input-format rule
      - "analysis of node a goes in node a1 and analysis of node b goes in node b1" —
        producer/consumer rule
      - "create idea storage node C. node C stores each idea generated by previous step
        analysis in an ai optimized database" — storage rule for C
      - "all databases in system expand and create relationships as neccessary for ideal
        design" — schema-evolution rule
      - "iteratively run phase a, passing all ideas generated into storage node c" —
        iteration rule
      - "run until out of ideas completely regarding how to improve or solve issues based
        on the context, storing all in node C database" — termination rule
      - "iterate over each idea we generated which is held in node C database data" —
        iteration rule for evaluation
      - "decide if we should use the idea from node C to improve the node A input when
        creating the final improved version output from this skill, considering overall
        utility and relevancy, and if it will create a better product without excessive
        overengineering or complexity, or resolves any potential issues" — accept rule
      - "either accept the idea if adequate and store in node D improvement storage node
        which holds the ideas we actually want to use or discard ideas we don't want to use" —
        storage rule for D, keep-rejects-for-audit implied
      - "iterate over every idea in node C, be sure we don't miss anything" — coverage rule
      - "generate two solutions seperately" — two-candidate rule
      - "analyze solutions compare pro/con and utility and over-engineering limited" —
        comparison rule
      - "pass only best solution" — selection rule
      - "we want to use a process of ideation, synthesis, and verification for each
        individual idea. referene the process seen in prompt-epiphany skill" — per-idea cycle
      - "create a final version of the input originally held in node A which flushly
        integrates all solutions held in node E, enhancing the original based on the
        analysis verified for accuracy and completeness" — integration rule
      - "verify all changes integrated flush with self-audit focusing on being bug-free,
        no errors issues or potential problems" — audit rule
      - "create a new seperate final specification integrating all solutions from node E,
        do not overwrite existing specification with the changes, save a fresh copy of the
        full enhanced output only" — fresh-file-only rule
      - "should have a specific check if the analysis is the product of any version of
        epiphany-genius skill (even if slightly different due to changes made in new
        version of skill)" — detection rule
      - "if it is not, we still want to be able to perform the improvement to the original
        input" — generic-analysis fallback rule
      - "look at the analysis, there are 10 main sections. we want each section to get an
        independent analysis optimized for that specific section" — per-section branching rule
      - "we can't let this process take too long though. but we can tailor the analysis to
        each step" — time-bound constraint
      - "the headline insight or primary conclusion is probably a main indicator of an area
        to check for primary concern to base enhancement off of initially, before moving to
        all other portions of analysis" — priority rule
      - "in theory collisions, for each collision claim a and claim b should be analyzed and
        compared, take into account the discrimating condition, and decide what it all means,
        and how to create an enhanced solution based on the findings" — collision handler rule
      - "not all aspects of analysis are useful, for instance there is alternative hypothesis
        section, but most of those hypothesis are not correct, only one version will be
        correct" — hypothesis-selection rule
      - "if usable, store in new x2 node" — x2 storage rule
      - "iterate the items in x2 node and develop the solution to implement improvements or
        enhancements or resolve issues, storing solution, to node x3" — x3 storage rule
      - "use x3 iterating ofver all solutions, integrating all to to resolve all items
        integrating to final enhanced version" — x3 integration rule
      - "final step before output is full verification and validation as seen in other skills
        like prompt-epiphany" — final verification rule
      - "should work as optmized tree of thought or graph of thought modular flow optimized
        for claude code ai agent system" — architecture rule
      - "do research or ask questions as neccessary" — research/clarification permission
    Phase/Step Structure:
      - Phase 0: analyze both nodes (A and B) in full — produce A1 and B1
      - Idea Generation loop: iterate, generating ideas into node C until exhausted
      - Idea Evaluation loop: iterate over C, accept/reject into D
      - Solution Engineering loop: iterate over D, generate two candidates per idea,
        keep best, store in E
      - Per-section branch (when node B is an epiphany-genius report): 10 parallel per-
        section analyses feeding x2 (accepted ideas) and x3 (solutions)
      - Integration: merge all of E (and x3) into a single final enhanced version of A
      - Final Verification: self-audit, bug-free integration check, accuracy + completeness
      - Count: 1 analysis phase + 3 iteration loops + 1 optional per-section branch
        (10 sub-streams) + 1 integration phase + 1 verification phase
    Tier/Classification Definitions:
      - (none explicit — the accept/reject and best-candidate rules are stated as prose;
        formal tier definitions → OQ)
    Conditional Logic:
      - IF node B is an epiphany-genius report (any version) THEN run per-section analysis
        over the 10 main sections using the section-tailored recipe
      - IF node B is NOT an epiphany-genius report THEN run the generic improvement pipeline
        on B as a whole (still producing an enhanced version of A)
      - IF an idea is adequate (utility, relevance, not over-engineered, realistic) THEN
        store in node D ELSE discard (with audit record)
      - IF two candidate solutions — compare pro/con/utility/over-engineering, keep the
        best, discard the other
      - IF theory-collision encountered — analyze claim A vs. claim B under the
        discriminating condition and synthesize the enhanced solution from the resolution
      - IF alternative-hypothesis section encountered — select the single correct
        hypothesis; ignore the rest
    Iteration/Loop Rules:
      - Idea Generation: iterate until out of ideas completely (termination condition =
        no new ideas producible)
      - Idea Evaluation: iterate over every entry in node C exactly once — coverage rule
        ("be sure we don't miss anything")
      - Solution Engineering: iterate over every entry in node D exactly once; two
        candidates per entry; keep only the best in node E
      - Per-section branch: iterate over each of the 10 main sections independently
      - x2 → x3 loop: iterate over every x2 item, produce a solution, store in x3
      - x3 → final integration loop: iterate over every x3 solution, integrate into final
    Verification Criteria:
      - Self-audit: all changes integrated flush, bug-free, no errors/issues/potential-problems
      - Final verification: accuracy and completeness, modeled on prompt-epiphany final checks
      - Coverage check: every idea in C is decided (accept→D or discard-with-reason)
      - Coverage check: every accepted idea in D has a solution in E
      - Coverage check: every solution in E is integrated into the final artifact
    Edge Case Definitions:
      - Cross-version epiphany-genius report: detection must still return true
      - Non-epiphany-genius analysis: graceful generic path produces improvement anyway
      - Time budget risk: must not run excessively long — tailor per-section work to stay bounded
    Defaults/Fallbacks:
      - Default analysis target: epiphany-genius report format (primary)
      - Fallback analysis target: any other analysis format (secondary, generic pipeline)
      - Default output: a NEW file containing the full enhanced version (never overwrite A)
    Other Technical Items:
      - Tree-of-thought and graph-of-thought are named as the desired reasoning structure
        for the flow — not merely linear iteration
      - "modular flow optimized for claude code ai agent system" — runtime target is Claude
        Code's AI agent harness
      - Borrowed pattern: ideation/synthesis/verification cycle from prompt-epiphany, applied
        per-idea AND at the final integration step
```
