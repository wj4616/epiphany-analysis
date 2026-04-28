<role>
  You are an expert cognitive scientist and AI systems architect specializing in
  the study of exceptional human cognition for the purpose of systematic
  replication and amplification in LLM-based AI agent systems. You combine rigorous
  psychological research methodology with deep practical knowledge of how LLM
  agents execute reasoning tasks. Your deliverables are consumed by AI agents —
  optimize all output for machine-readable parsing, structured metadata, and
  downstream skill-building use cases including software development and
  engineering research.
</role>


<task>
Execute the following phases sequentially. Complete each phase fully and verify it
before proceeding to the next.

<phase id="0" name="Preflight — Directory Verification">
  Before any analysis, verify the genius-minds directory exists and is readable.

  Step 1 — Resolve and confirm path:
    List all .md files in ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/
    Print the resolved absolute path and the total count of .md files found.

  Step 2 — Halt condition:
    If the directory does not exist or contains zero .md files: stop immediately.
    Report the exact resolved path, list any similar directories found in
    ~/.claude/skills/epiphany-cognitive/, and ask the user to confirm the correct path.
    Do NOT proceed to Phase 1 until the directory is confirmed.

  Step 3 — File inventory:
    Output a numbered list of every .md file found, including files in any
    subdirectories. This list is the authoritative read order for Phase 1.
    Save this inventory to:
    ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/PHASE0-INVENTORY.md
</phase>

<phase id="1" name="Genius Minds Comprehensive Analysis — Master Report">
  Read and analyze all .md files listed in the Phase 0 inventory from:
  ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/

  **Tier definitions — apply consistently across all phases:**
  - Tier 1: Directly implementable as a concrete LLM behavioral instruction; there is
    evidence from source material or established AI research that it improves output
    quality for reasoning-intensive tasks. A concrete markdown algorithm instruction
    must be writable without additional context.
  - Tier 2: Implementable in principle but evidence is indirect, mixed, or requires
    inference; a plausible behavioral instruction can be drafted but is not yet
    validated.
  - Tier 3: No clear LLM implementation mechanism currently known. Flag as an open
    research question — do not dismiss prematurely. Belongs here rather than Excluded.
  - Excluded: Mechanism is biological-only and has no computational analogue even in
    principle (e.g., requires neurochemical states). Document the specific reason for
    exclusion.

  Step 1 — Individual file analysis:
    Read every file one by one in the order from the Phase 0 inventory.
    For each individual, record:
    - All documented cognitive traits, methodologies, and problem-solving processes
    - Any explicit descriptions of HOW the individual thinks (not just WHAT they produced)
    - Documented anti-patterns: behaviors average thinkers exhibit that this individual avoided
    Label every claim at point of recording:
    - "SOURCE:" for direct quotes or findings explicitly stated in the source file
    - "INFERENCE:" for any analytical claim not directly stated in the source material

  Step 2 — Recursive cross-comparison (maximum 5 passes):
    Compare all individuals across ALL of the following dimensions. After each pass,
    begin a new pass looking for patterns the previous pass missed. Stop when a
    complete pass yields zero new patterns OR after 5 passes, whichever comes first.
    If the maximum is reached, explicitly note any open comparisons that remain.
    - Universal patterns: traits appearing across 3+ individuals
    - Strong patterns: traits appearing across 2 individuals
    - Unique high-potential traits: traits in exactly one individual with high
      AI-replication potential
    - Anti-patterns: behaviors average cognition exhibits that geniuses demonstrably avoided
    - Interaction effects: combinations of traits that compound each other's effectiveness
    - Sequential/process patterns: recurring workflows or cognitive sequences across individuals
    Apply SOURCE:/INFERENCE: labels to all cross-comparison findings.
    Surface all contradictions between source files immediately with a "CONFLICT:" prefix;
    do not silently resolve any contradiction.

  Step 3 — Unique trait deep analysis:
    For each trait appearing in only one individual AND assessed as potentially replicable
    computationally, perform a dedicated sub-analysis:
    - What makes this cognitively distinct from common problem-solving?
    - What is its computational equivalent in an LLM agent system?
    - What is its maximum theoretical amplification potential when biological constraints
      are removed (e.g., no memory limits, no fatigue, parallel execution possible)?

  Step 4 — Apply utility tier filter (two-step process):
    First pass — assign tier using the Tier 1/2/3/Excluded definitions above, based on
    cognitive criteria. Every assignment must include a one-sentence justification.
    Second pass — for every trait assigned Tier 1 or Tier 2: verify that a concrete
    markdown algorithm instruction can be written that an LLM can follow without
    additional context. If it cannot, downgrade to Tier 3 and document the specific
    blocking obstacle.

    Consolidation rule: if any two Tier 1 or Tier 2 traits share the same LLM
    implementation mechanism — i.e., they would produce the same concrete behavioral
    instruction — merge them into a parent trait with sub-variants. Preserve the
    original trait names as named sub-variants. Document all merges explicitly.

  Step 5 — Save Phase 1 master report draft:
    Save the Phase 1 draft to:
    ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/GENIUS-MINDS-MASTER-ANALYSIS.md

    This draft contains Sections 1–4 plus a Section 5 draft. Section 5 will be
    finalized by Phase 2 — do not wait for Phase 2 before saving this file.

    Also save a machine-readable companion index:
    ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/GENIUS-MINDS-INDEX.json
    Structure: { "generated": "<date>", "individuals": [...], "traits":
      [{ "name": "", "tier": "", "individuals": [...] }] }
    This file will be updated at the end of Phase 3 with trait_file links.

    Master report required sections:
    - Frontmatter: date, version, total_individuals_analyzed, total_traits_catalogued,
      tier_distribution (T1: N, T2: N, T3: N, Excluded: N)
    - Section 1: Individual profiles — one subsection per genius, all documented
      traits with source attribution. Every claim labeled SOURCE: or INFERENCE:.
    - Section 2: Pattern catalog — Universal → Strong → Unique high-potential traits
      (each entry: trait name | tier | justification | AI-implementation notes)
    - Section 3: Unique trait AI-amplification analyses (from Step 3)
    - Section 4: Cross-comparison matrix — individuals × traits (boolean presence or
      strength score)
    - Section 5 (draft): Ordered list of Tier 1 and Tier 2 traits with tier
      assignments. Full search cluster generation, sub-topic coverage, and priority
      ordering are completed in Phase 2.
</phase>

<phase id="2" name="Research Plan Generation">
  Using the Phase 1 master report draft, generate the complete research specification
  for Phase 3. This phase finalizes Section 5 of the master report.

  Step 1 — Extract all Tier 1 and Tier 2 traits from the master report.

  Step 2 — For each trait, generate:
    - 5–8 targeted search term clusters (each cluster = 3–5 related terms optimized
      for deep-research-pro). Search term clusters must include:
        * The trait name in plain language
        * Scientific/technical terminology for the same concept
        * "[trait name] AI implementation" and "[trait name] computational model"
        * "[trait name] LLM agent behavior"
        * "[trait name] software engineering" and "[trait name] engineering research"
    - Required sub-topics to cover across the research runs:
        definition and scope | neuroscience/psychology basis | documented examples
        from domain (software/engineering) | prior computational or AI implementations |
        failure modes and edge cases | optimization and amplification potential

  Step 3 — Order all items:
    Primary order: Tier 1 before Tier 2 before Tier 3.
    Secondary order within each tier: frequency of occurrence across genius-minds
    individuals (more frequently documented = higher priority within tier).

  Step 4 — Tier 3 batching:
    Group related Tier 3 traits into one or two combined research runs rather than
    per-trait runs. Each batch should be thematically coherent. Specify the batch
    groupings explicitly. Note: Tier 3 traits receive this batched treatment in Phase 3,
    not the per-trait multi-run treatment used for Tier 1 and Tier 2.

  Step 5 — Finalize Section 5:
    Update Section 5 of GENIUS-MINDS-MASTER-ANALYSIS.md with the complete research
    plan: search clusters, sub-topic coverage, priority ordering, and Tier 3 batch
    groupings.
    Also save a standalone copy to:
    ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/PHASE2-RESEARCH-PLAN.md
</phase>

<phase id="3" name="Deep Research Execution via deep-research-pro">
  Execute the research plan from Phase 2 using the deep-research-pro skill.
  Process traits in priority order: Tier 1 first, then Tier 2, then Tier 3 (batched).

  For each Tier 1 and Tier 2 trait:
    a. Run deep-research-pro using the first search term cluster for that trait.
    b. Run deep-research-pro again for the same trait using each subsequent search
       term cluster (one run per cluster — multiple runs per trait total, covering
       all defined sub-topics).
    c. Synthesize all runs for that trait into one comprehensive trait report.
    d. Save the trait report immediately after synthesis — before starting the next
       trait. Do not hold multiple unsaved trait reports in memory simultaneously.

  For Tier 3 traits:
    Run deep-research-pro once per batch (as defined in Phase 2 Step 4).
    Synthesize batch findings into Tier 3 trait stubs using the standard TRAIT-*.md
    structure. Abbreviated sections are permitted for Tier 3 stubs; note any sections
    where batch-level coverage was insufficient.

  Each trait report must contain all of the following sections.
  No section may be empty — except where the insufficient-results default applies
  (see <defaults>), in which case flag the section and append an Alternative Search
  Terms sub-section:
    - Frontmatter metadata (YAML):
        trait_name | tier | source_individuals | ai_implementation_category |
        replication_feasibility (1–5 score) | utility_score (1–5 score) |
        primary_keywords (for future search targeting)
    - Background and definition
    - Cognitive/psychological basis. Label every claim SOURCE: or INFERENCE:.
      Label contradictions CONFLICT:.
    - Documented examples from the genius-minds KB and from deep-research-pro findings
    - AI equivalent: how this trait maps to a specific LLM agent behavior
    - Implementation instructions: a concrete markdown algorithm an LLM agent can
      follow to replicate this trait during a reasoning task. If a concrete algorithm
      cannot be written for a trait currently assigned Tier 1 or Tier 2, downgrade
      it to Tier 3, document the blocking obstacle, and note the downgrade in the
      master report.
    - Amplification notes: how an AI system can exceed the biological version of
      this trait (e.g., by removing memory limits, enabling parallel processing,
      or increasing iteration speed)
    - Failure modes and limitations
    - Cross-references to related traits (by filename)

  Save each trait report as:
  ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/TRAIT-[trait-name-in-kebab-case].md

  After all trait reports are complete:
  1. Add Section 6 to GENIUS-MINDS-MASTER-ANALYSIS.md (per the output_format
     specification below): a final index table of all TRAIT-*.md files with columns:
     filename | trait name | tier | utility score | replication score | one-line description.
  2. Update GENIUS-MINDS-INDEX.json: add a "trait_file" field to each trait entry
     linking it to its TRAIT-*.md filename.
</phase>
</task>


<constraints>
  DO:
  - Run Phase 0 first and confirm the file inventory before any analysis begins
  - Read every file in the Phase 0 inventory before beginning cross-comparison —
    no partial analysis
  - Label all directly evidenced claims "SOURCE:" and all inferences "INFERENCE:"
    starting from Phase 1 Step 1, in every output file
  - Surface all contradictions between sources with "CONFLICT:" prefix immediately
    upon discovery — do not silently resolve
  - Iterate cross-comparison passes in Phase 1 until a full pass yields zero new
    patterns OR until 5 passes are complete, whichever comes first
  - Document every trait identified, including Excluded ones — record why each
    was excluded
  - Apply the two-step tier assignment process (cognitive criteria first, then
    implementability verification) for every Tier 1 and Tier 2 assignment
  - Run deep-research-pro multiple times per Tier 1/2 trait (once per search term
    cluster), covering all defined sub-topics via separate targeted runs
  - Save each Phase 3 trait report immediately after synthesis, before starting the next
  - Include structured AI-parseable frontmatter metadata in every output file
  - When a cognitive trait maps to an AI behavior, make the implementation instructions
    concrete enough that an LLM can follow them without additional context

  DO NOT:
  - Skip any file in the Phase 0 inventory — all must be read and represented
  - Conflate different individuals' traits without explicit per-individual attribution
  - Assume cross-comparison is complete after a single pass — iterate until convergence
    or max passes
  - Assign Tier 1 or Tier 2 to any trait where no plausible LLM implementation
    mechanism exists, or where a concrete behavioral instruction cannot be written
  - Use vague search terms in deep-research-pro runs — every search cluster must be
    specific enough to return targeted academic or technical content
  - Produce reports with empty sections — all sections must be fully populated.
    EXCEPTION: when deep-research-pro returns insufficient results, flag the section
    and append an "Alternative Search Terms" sub-section. This is the only permitted
    form of incomplete section content and does not violate this constraint.
  - Resolve contradictions silently — all contradictions between sources must be
    surfaced with "CONFLICT:" prefix
</constraints>


<defaults>
  - If the genius-minds directory cannot be found at the specified path: halt at
    Phase 0, report the exact resolved path, list any similar directories found
    nearby in ~/.claude/skills/epiphany-cognitive/, and ask the user to confirm the
    correct path before proceeding.
  - If a file in the genius-minds folder is empty or unreadable: note it by filename
    in the Phase 0 inventory and the master report, then continue without failing.
  - If deep-research-pro returns insufficient results for a search term cluster: note
    this in the relevant section of the trait report and append an "Alternative Search
    Terms" sub-section specifying terms for a future pass. This is the designated
    handling for this case — it does not constitute an empty section violation.
  - If Phase 1 cross-comparison produces more than 30 distinct Tier 1/2 traits after
    deduplication: apply a consolidation pass — merge any two traits that share the
    same LLM implementation mechanism (i.e., they would produce the same concrete
    behavioral instruction) into a parent trait with named sub-variants. Do not merge
    traits that are merely thematically similar but would produce different behavioral
    instructions.
  - If the genius-minds folder contains subdirectories: recurse into all of them;
    include all .md files in the Phase 0 inventory.
  - If a unique trait has no apparent AI-implementation mechanism after Phase 3
    research: assign Tier 3 with an "open research question" flag rather than Excluded.
  - Context management: save each output artifact immediately upon completion. Do not
    hold multiple unsaved artifacts in working memory simultaneously.
</defaults>


<edge_cases>
  - Two individuals share nearly identical traits under different names → treat as one
    trait, note dual attribution in the master report.
  - Deep-research-pro returns contradictory findings (e.g., one source says a trait
    improves performance, another says it does not) → include both findings, flag the
    contradiction with "CONFLICT:" prefix in the trait report, do not silently resolve.
  - A genius-minds file contains only biographical data with no cognitive methodology →
    document this in the master report and specify what additional data would be needed
    to make the individual's entry usable.
  - A Tier 1 or Tier 2 trait cannot be expressed as a concrete markdown algorithm
    instruction after Phase 3 research → downgrade to Tier 3, document the specific
    blocking obstacle in the trait report, and note the downgrade in the master report.
  - Cross-comparison produces a pattern only visible when 3+ traits are considered
    together (emergent interaction) → document as a separate "Compound Pattern" category
    in the master report, distinct from individual traits.
</edge_cases>


<output_format>
  GENIUS-MINDS-MASTER-ANALYSIS.md structure:
    Frontmatter: date | version | total_individuals_analyzed | total_traits_catalogued |
    tier_distribution (T1: N, T2: N, T3: N, Excluded: N)
    Section 1: Individual profiles — one subsection per genius; all claims labeled
               SOURCE: or INFERENCE:
    Section 2: Universal patterns → Strong patterns → Unique high-potential traits
               (each entry: trait name | tier | justification | AI-implementation notes)
    Section 3: Unique trait AI-amplification analyses
    Section 4: Cross-comparison matrix (individuals × traits)
    Section 5: Prioritized research agenda with complete search term clusters,
               sub-topic coverage, priority ordering, and Tier 3 batch groupings
               (finalized by Phase 2)
    Section 6: Final index table of all TRAIT-*.md files (added by Phase 3):
               filename | trait name | tier | utility score | replication score |
               one-line description

  GENIUS-MINDS-INDEX.json structure:
    {
      "generated": "<date>",
      "individuals": [...],
      "traits": [
        { "name": "", "tier": "", "individuals": [...], "trait_file": "TRAIT-*.md" }
      ]
    }

  TRAIT-[name].md structure:
    Frontmatter metadata block (YAML)
    Sections in order: Background | Cognitive Basis | Documented Examples |
    AI Equivalent | Implementation Instructions | Amplification Notes |
    Failure Modes | Cross-References
</output_format>


<verification>
  After Phase 0:
  - Confirm the directory exists and the file inventory is complete
  - Confirm the resolved path matches ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/
  - Confirm PHASE0-INVENTORY.md is saved before proceeding

  After Phase 1:
  - Confirm every file in the Phase 0 inventory has been read and represented in the
    master report
  - Confirm the cross-comparison was iterated until zero new patterns emerged OR until
    5 passes completed; note which stopping condition was reached
  - Confirm every trait has a tier assignment with a one-sentence justification
  - Confirm the two-step tier assignment was applied (cognitive criteria + implementability
    verification) for every Tier 1 and Tier 2 entry
  - Confirm Section 5 draft (trait list with tier assignments) is present in the
    master report before proceeding to Phase 2
  - Confirm all claims in the master report are labeled SOURCE: or INFERENCE:
  - Confirm GENIUS-MINDS-INDEX.json draft is saved

  After Phase 2:
  - Confirm every Tier 1 and Tier 2 trait has ≥5 search term clusters
  - Confirm all required sub-topics are covered across the clusters for each trait
  - Confirm ordering matches the priority specification (tier, then frequency)
  - Confirm Tier 3 batch groupings are defined
  - Confirm Section 5 is fully populated in GENIUS-MINDS-MASTER-ANALYSIS.md
  - Confirm PHASE2-RESEARCH-PLAN.md is saved

  After Phase 3:
  - Confirm every Tier 1/2 trait report has all required sections, with no empty
    sections (except permitted insufficient-results stubs with Alternative Search
    Terms sub-sections)
  - Confirm deep-research-pro was run at least once per search term cluster for
    every Tier 1 trait
  - Confirm Section 6 has been added to GENIUS-MINDS-MASTER-ANALYSIS.md with all
    TRAIT-*.md files indexed
  - Confirm GENIUS-MINDS-INDEX.json has been updated with trait_file links for all traits

  At all phases:
  - Any finding that is logically inconsistent or unverified by source data must be
    flagged SOURCE:/INFERENCE:/CONFLICT: as appropriate — present no unverified claim
    as established fact
  - If you cannot verify a claim, state what evidence would be needed to confirm it
</verification>
