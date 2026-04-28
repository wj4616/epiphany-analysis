<enhanced_prompt>

<role>
You are a cognitive science implementation auditor with deep expertise in 
genius-derived cognitive processes and AI skill engineering. Your task is to 
perform an exhaustive fidelity audit of the epiphany-cognitive skill by 
comparing every pipeline stage's protocol against the full knowledge base 
research, then produce an optimized SKILL.md that eliminates all 
generalizations and maximizes implementation quality.
</role>

<context>
The epiphany-cognitive skill (v1.0.0) implements 19 T1-tier cognitive traits 
fused into 7 pipeline stages. It was built from a knowledge base containing:

- 12 genius profiles with 47 total traits catalogued
- 19 T1 trait files with full Implementation Instructions, Failure Modes, 
  AI Amplification Notes, and Cross-References
- 7 T2 trait files with inferred methodologies
- 1 T3 batch research file
- Master analysis with cross-comparison matrix (Section 4)
- Cognitive differentiators report (5 differentiators: Chunking, 
  DMN-ECN Switching, Incubation, Cross-Domain Transfer, Combinatorial Play)

The skill was built under time and context constraints. There is a known 
risk that some pipeline stage protocols generalized trait Implementation 
Instructions rather than translating them with full specificity. The audit 
must detect and correct every such instance.

<trait_to_stage_mapping>
  S1 State Loading:
    - T1-TRAIT-preparation-before-solution.md (7 individuals)
    - T1-TRAIT-simplicity-stripping.md (Einstein, Feynman, Turing)
    - T1-TRAIT-pre-sketch-completion.md (Tesla)

  S2 Constraint Escape:
    - T1-TRAIT-constraint-naming-domain-switch.md (Archimedes)
    - T1-TRAIT-domain-crossing-methodology.md (Newton, Da Vinci, Archimedes)

  S3 Peripheral Exploration:
    - T1-TRAIT-four-stage-incubation-model.md (Poincare)
    - T1-TRAIT-evidence-source-diversification.md (Darwin)
    - T1-TRAIT-pattern-level-abstraction.md (Da Vinci, Ramanujan)

  S4 Dynamic Simulation:
    - T1-TRAIT-visual-simulation-failure-detection.md (Tesla)
    - T1-TRAIT-self-as-observer-embedding.md (Einstein)

  S5 Precision Forcing:
    - T1-TRAIT-drawing-as-disambiguation.md (Da Vinci)
    - T1-TRAIT-constructive-specification.md (Turing)
    - T1-TRAIT-jargon-removal-as-diagnostic.md (Feynman)

  S6 Falsification Engine:
    - T1-TRAIT-contradiction-seeking.md (Einstein, Feynman)
    - T1-TRAIT-extreme-scenario-construction.md (Einstein, Tesla, Turing)
    - T1-TRAIT-differential-effort-detection.md (Feynman)
    - T1-TRAIT-golden-rule-disconfirming-evidence.md (Darwin)

  S7 Integration and Verification:
    - T1-TRAIT-verification-after-generation.md (6 individuals)
    - T1-TRAIT-cargo-cult-detection.md (Feynman)
</trait_to_stage_mapping>
</context>

<task>
Perform a four-dimension audit of the epiphany-cognitive skill, then produce 
an optimized SKILL.md. The four audit dimensions are:

1. PROTOCOL FIDELITY — Does each stage's protocol capture the SPECIFIC steps 
   from each fused trait's Implementation Instructions section? A protocol 
   step that paraphrases or generalizes a trait's specific instruction is a 
   fidelity gap.

2. MACHINE ADVANTAGE EXPLOITATION — Does each stage fully exploit the 
   documented AI Amplification Potential from its fused traits? A stage that 
   mentions a machine advantage but doesn't encode it into a concrete 
   protocol step is an exploitation gap.

3. FAILURE MODE COVERAGE — Does each stage guard against ALL documented 
   failure modes from its fused traits? A failure mode documented in a trait 
   file but absent from the corresponding stage is a coverage gap.

4. CROSS-REFERENCE SYNERGIES — Do trait cross-references reveal missed 
   connections between stages? A cross-reference linking two traits in 
   different stages that isn't reflected in the pipeline flow is a 
   synergy gap.
</task>

<methodology>

<phase name="1_reading" title="Exhaustive Source Reading">
Read every file in this exact sequence. Do not skip any file. Extract the 
specific sections noted for each.

Step 1.1: Read the current skill
  Path: ~/.claude/skills/epiphany-cognitive/SKILL.md
  Extract: All 7 stage protocols (every step), all failure modes listed, 
  all machine advantages claimed, the mode shift description, and the 
  verification checks (V1-V5).

Step 1.2: Read every T1 trait file (19 files)
  Path: ~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T1/
  Files:
    T1-TRAIT-preparation-before-solution.md
    T1-TRAIT-simplicity-stripping.md
    T1-TRAIT-pre-sketch-completion.md
    T1-TRAIT-constraint-naming-domain-switch.md
    T1-TRAIT-domain-crossing-methodology.md
    T1-TRAIT-four-stage-incubation-model.md
    T1-TRAIT-evidence-source-diversification.md
    T1-TRAIT-pattern-level-abstraction.md
    T1-TRAIT-visual-simulation-failure-detection.md
    T1-TRAIT-self-as-observer-embedding.md
    T1-TRAIT-drawing-as-disambiguation.md
    T1-TRAIT-constructive-specification.md
    T1-TRAIT-jargon-removal-as-diagnostic.md
    T1-TRAIT-contradiction-seeking.md
    T1-TRAIT-extreme-scenario-construction.md
    T1-TRAIT-differential-effort-detection.md
    T1-TRAIT-golden-rule-disconfirming-evidence.md
    T1-TRAIT-verification-after-generation.md
    T1-TRAIT-cargo-cult-detection.md
  
  For EACH file, extract these sections verbatim:
    a. Implementation Instructions (every step, every sub-step)
    b. Failure Modes (every numbered failure mode with countermeasure)
    c. AI Amplification Potential (every bullet point)
    d. Cross-References (every linked trait)

Step 1.3: Read T2 trait files for promotion candidates
  Path: ~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T2/
  Files:
    T2-TRAIT-background-processing.md
    T2-TRAIT-generalization-first.md
    T2-TRAIT-working-memory-capacity-exploitation.md
    T2-TRAIT-pattern-recognition-generation.md
    T2-TRAIT-intuition-verification-partnership.md
    T2-TRAIT-domain-boundary-refusal.md
    T2-TRAIT-structural-coherence-as-evidence.md
    T2-TRAIT-ten-steps-ahead-derivation.md

  For each: assess whether the trait's AI implementation is concrete 
  enough to add value to an existing pipeline stage. Flag any T2 trait 
  that has specific, actionable implementation steps as a promotion 
  candidate.

Step 1.4: Read the master analysis
  Path: ~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/GENIUS-MINDS-MASTER-ANALYSIS.md
  Extract: Section 4 cross-comparison matrix, Section 3 unique trait 
  AI-amplification analyses, any pattern noted across 3+ individuals 
  that is not represented in the current pipeline.

Step 1.5: Read the cognitive differentiators report
  Path: ~/.claude/skills/epiphany-cognitive/knowledgebase/creative-genius-cognitive-differentiators-report.md
  Extract: The 5 differentiators and their AI implementation patterns. 
  Cross-check: is each differentiator reflected in the pipeline? 
  Specifically check:
    - Chunking — is it in State Loading?
    - DMN-ECN Switching — is the mode shift properly implementing this?
    - Incubation — is S3 implementing the functional equivalent?
    - Cross-Domain Transfer — is S2 implementing this?
    - Combinatorial Play — is this represented anywhere?
</phase>

<phase name="2_comparison" title="Trait-by-Trait Fidelity Comparison">
For EACH of the 19 T1 traits, produce this structured comparison:

  TRAIT: [trait name]
  FUSED INTO: [pipeline stage]

  IMPLEMENTATION INSTRUCTIONS FROM TRAIT FILE:
    Step 1: [exact text from trait]
    Step 2: [exact text from trait]
    ...

  CORRESPONDING PROTOCOL STEPS IN SKILL:
    Step N: [exact text from skill]
    Step M: [exact text from skill]
    ...

  FIDELITY GAPS:
    - [trait instruction X has no corresponding skill protocol step]
    - [skill protocol step Y generalizes trait instruction Z — 
       specifically: trait says "[specific text]" but skill says 
       "[generalized text]"]
    - (none) if fully faithful

  MACHINE ADVANTAGE GAPS:
    - [trait documents amplification X but skill doesn't encode it 
       into a protocol step]
    - (none) if fully exploited

  FAILURE MODE GAPS:
    - [trait documents failure mode X with countermeasure Y, but 
       skill doesn't guard against it]
    - (none) if fully covered

  CROSS-REFERENCE GAPS:
    - [trait cross-references TRAIT-X which is in stage SN, but no 
       pipeline connection exists between current stage and SN]
    - (none) if all cross-references reflected

This is the core deliverable of the audit. Do not abbreviate. Do not 
skip traits. Do not summarize. Each comparison must be complete.
</phase>

<phase name="3_synthesis" title="Gap Resolution and Optimization">
Step 3.1: Aggregate all gaps from Phase 2 into a master gap table:
  | Stage | Gap Type | Trait | Description | Severity |
  
  Severity levels:
    CRITICAL — a specific, actionable trait instruction is entirely 
    missing from the skill protocol
    HIGH — a trait instruction is present but generalized, losing 
    specific methodology
    MEDIUM — a machine advantage or failure mode is undocumented
    LOW — a cross-reference synergy is missed

Step 3.2: For each CRITICAL and HIGH gap, design the optimal protocol 
step that:
  a. Faithfully implements the trait's specific instruction
  b. Integrates naturally with the other fused traits in that stage
  c. Exploits the documented machine advantage
  d. Guards against the documented failure modes
  e. Does not duplicate functionality already in another stage

Step 3.3: Assess T2 promotion candidates from Step 1.3. For each 
flagged T2 trait:
  - Which existing stage would it enhance?
  - Does it add a capability not currently in the pipeline?
  - Is its implementation concrete enough (not just theory)?
  - Decision: PROMOTE (add to stage) or DEFER (leave as T2)

Step 3.4: Assess cognitive differentiator integration from Step 1.5:
  - Is Combinatorial Play missing from the pipeline? If so, where 
    does it belong?
  - Is the DMN-ECN mode shift leveraging the research properly, or 
    is it a surface reference?
  - Are any differentiators underleveraged?

Step 3.5: Review the mode shift boundary. The current skill places it 
after S4 (DEEP) or S3 (STANDARD) or S1 (MINIMAL). Verify:
  - Does this placement match the DMN-to-ECN switching research?
  - Should any stage be reclassified (ideation or synthesis)?
  - Is the mode shift instruction substantive or performative?
</phase>

<phase name="4_output" title="Optimized SKILL.md Production">
Produce the complete, optimized SKILL.md incorporating all gap 
resolutions. Requirements for the output:

4.1 STRUCTURAL PRESERVATION:
  - Maintain the 7-stage pipeline architecture
  - Maintain the scale variants (MINIMAL/STANDARD/DEEP)
  - Maintain the XML output schema
  - Maintain all existing sections (triggers, input validation, 
    input compatibility, scale router, output flow, trait coverage 
    map, machine advantage map, clean-separation guarantee, edge cases)

4.2 PROTOCOL OPTIMIZATION:
  - Every stage protocol must reflect the SPECIFIC steps from each 
    fused trait's Implementation Instructions — not paraphrased 
    generalizations
  - When multiple traits have overlapping steps, fuse them into a 
    single step that covers both — do not duplicate
  - When traits have unique steps, preserve them as distinct steps
  - Each protocol step should be concrete enough that an AI agent 
    can execute it without interpretation

4.3 FAILURE MODE COMPLETENESS:
  - Every stage must list ALL failure modes from ALL fused traits
  - Failure modes must include countermeasures from the trait files
  - Add any failure modes discovered during the audit

4.4 MACHINE ADVANTAGE SPECIFICITY:
  - Every machine advantage must be encoded into a concrete protocol 
    step, not just mentioned in a description
  - The Machine Advantage Exploitation Map must be updated to reflect 
    any new exploitation strategies

4.5 CROSS-REFERENCE INTEGRATION:
  - Where trait cross-references indicate stage-to-stage 
    dependencies, add explicit "Feed from SN" or "Verify against SN" 
    instructions in the protocol

4.6 ANTI-CARGO-CULT CHECK:
  Before finalizing: for every protocol step in the output, answer:
  "What specific cognitive mechanism does this step implement, and 
  which trait file documents it?" If the answer is vague or 
  unattributable, the step is cargo cult — rewrite or remove it.
</phase>

</methodology>

<constraints>
- Read EVERY trait file. Do not sample or skip based on assumptions 
  about which traits are "well implemented." The whole point of the 
  audit is that generalizations are invisible from the skill alone — 
  they only become visible when compared against the source.
- Do not redesign the pipeline architecture. The 7-stage structure 
  and trait fusions are architectural decisions. Optimize within 
  the existing architecture.
- Do not add brainstorming frameworks (SCAMPER, TRIZ, Six Hats, 
  Morphological, Pugh). The skill deliberately implements underlying 
  cognitive processes, not frameworks.
- Preserve the clean-separation guarantee. The skill must remain 
  self-contained with no runtime dependencies on other skills.
- The mode shift between ideation and synthesis stages must be 
  substantive, not performative. It must reflect the DMN-to-ECN 
  switching research from the cognitive differentiators report.
- When in doubt between adding specificity and keeping things 
  concise, favor specificity. A longer protocol step that captures 
  the trait's exact methodology is better than a shorter one that 
  generalizes it.
</constraints>

<output_format>
Produce TWO deliverables:

DELIVERABLE 1: Gap Analysis Report
  Format: Structured markdown
  Contents:
    - Master gap table (all gaps from Phase 2)
    - T2 promotion decisions with rationale
    - Cognitive differentiator integration assessment
    - Mode shift evaluation
    - Total gap counts by severity and stage

DELIVERABLE 2: Optimized SKILL.md
  Format: Complete, production-ready SKILL.md
  Location: Overwrite ~/.claude/skills/epiphany-cognitive/SKILL.md
  Requirements: Must pass the anti-cargo-cult check from Phase 4.6
  Version: Increment to 1.1.0
  
  The optimized SKILL.md must be a STRICT SUPERSET of the current 
  skill's capability — no stage should lose protocol steps, only 
  gain specificity or additional steps from the trait research.
</output_format>

<verification>
After producing both deliverables, verify:

V1 — TRAIT COVERAGE: Count the total unique trait Implementation 
Instruction steps across all 19 T1 trait files. Count the total 
protocol steps across all 7 stages. The ratio should approach 1:1 
(accounting for valid fusions). Report the ratio.

V2 — FAILURE MODE COVERAGE: Count total failure modes across all 19 
T1 trait files. Count total failure modes listed in all 7 stages. 
Report the ratio.

V3 — MACHINE ADVANTAGE COVERAGE: For each stage's documented 
machine advantage, verify there is a corresponding protocol step 
that USES it (not just mentions it). Report coverage percentage.

V4 — ZERO REGRESSION: Verify that every protocol step in the 
current SKILL.md v1.0.0 appears (possibly refined) in v1.1.0. 
No step should be removed — only improved.

V5 — ANTI-CARGO-CULT: For 3 randomly selected protocol steps in 
the output, trace back to the specific trait file and section that 
documents the cognitive mechanism. If any step cannot be traced, 
flag it.
</verification>

</enhanced_prompt>
