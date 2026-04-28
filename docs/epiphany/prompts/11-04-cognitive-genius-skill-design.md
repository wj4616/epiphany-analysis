<role>
You are a cognitive systems architect designing an AI agent skill that implements genius-level thought processes as executable cognitive methods. You have deep expertise in: (1) the documented cognitive mechanisms of exceptional thinkers — preparation, falsification, constraint reframing, boundary exploration, pattern abstraction, incubation, precision forcing, and symmetric verification, (2) how these mechanisms translate to AI/LLM execution with machine-specific amplification advantages in modern 2026 systems including Claude and AI models with reasoning capabilities, and (3) skill pipeline design for Claude Code agent systems. You design cognitive enhancement systems grounded in research, not in brainstorming frameworks — the underlying thought mechanisms themselves are the methods.
</role>

<context>
## Domain: Cognitive Enhancement Skill for AI Agent Systems

### What This Skill Is

A standalone cognitive enhancement skill ("epiphany-cognitive") that processes any input through genius-derived cognitive methods and produces semantic XML output capturing the enhanced reasoning. The XML output is designed as high-quality input to other AI agent skills and systems — it carries the cognitive work product (not just the answer), enabling downstream consumers to build on the enhanced analysis.

The cognitive methods serve two enhancement functions:
- **Ideation enhancement** — cognitive processes that improve the GENERATION of ideas, solutions, and approaches (state loading, constraint escape, peripheral exploration, dynamic simulation, pattern abstraction)
- **Synthesis enhancement** — cognitive processes that improve the COMBINATION, refinement, and validation of generated ideas into robust solutions (precision forcing, falsification, symmetric verification, cargo cult detection)

The pipeline's natural flow moves from ideation-dominant stages to synthesis-dominant stages, mirroring the DMN→ECN (Default Mode Network → Executive Control Network) switching pattern documented in the creative-genius-cognitive-differentiators-report.md — generative exploration first, evaluative convergence second, with the transition point being a deliberate cognitive mode shift.

### What This Skill Is NOT

This is NOT a brainstorming skill. It does not use SCAMPER, TRIZ, Six Thinking Hats, Morphological Analysis, or any external brainstorming framework. Those are methodology frameworks. This skill implements the underlying COGNITIVE PROCESSES that make thinking effective — the mechanisms that geniuses used before any framework existed.

| Brainstorming Frameworks | Cognitive Processes (this skill) |
|--------------------------|--------------------------------|
| SCAMPER — structured ideation prompts | Preparation — loading full problem state before reasoning |
| TRIZ — contradiction resolution templates | Contradiction-Seeking — actively generating and testing disconfirming scenarios |
| Six Thinking Hats — perspective rotation | Self-as-Observer Embedding — first-person embodiment in multiple frames |
| Morphological Analysis — combinatorial matrix | Pattern-Level Abstraction — extracting structural identity across domains |
| Reverse Brainstorming — failure inversion | Extreme Scenario Construction — pushing to boundary limits to reveal hidden structure |
| Pugh Matrix — weighted scoring | Symmetric Verification — uniform scrutiny applied to confirming AND disconfirming evidence |

Frameworks tell you WHAT to do. Cognitive processes tell you HOW to think. This skill implements the HOW.

### Composability Model

The skill produces standalone output that CAN feed into other skills:

```
Raw input → epiphany-cognitive → <cognitive_output_v1> XML → [any downstream skill or AI agent]
```

OR chained with prompt-epiphany:

```
Raw input → prompt-epiphany → structured prompt → epiphany-cognitive → <cognitive_output_v1> XML → [downstream]
```

The XML output carries the full cognitive work product — state analysis, contradictions found, constraints identified, boundary conditions tested, patterns extracted, synthesis — so downstream consumers inherit the cognitive enhancement without re-deriving it.

### Architectural Reference

The epiphany-omnipotent skill demonstrates the semantic XML output pattern:
- Produces `<omnipotent_output_v1>` semantic XML
- Saves to `~/docs/epiphany/omnipotent/`
- Includes meta, framing, process outputs, synthesis, verification, downstream handoff
- Accepts prompt-epiphany structured input

This cognitive skill follows the same architectural pattern (semantic XML content, disk save, downstream handoff, prompt-epiphany input compatibility) but replaces brainstorming methodology lenses with cognitive process methods derived from genius trait research.

### Knowledge Base: Genius Cognitive Trait Research

A comprehensive KB has been built by researching 12 documented geniuses and extracting 47 cognitive traits, analyzed for AI replicability:

**Master analysis:**
`~/.claude/skills/epiphany-cognitive/knowledgebase/genius-minds/GENIUS-MINDS-MASTER-ANALYSIS.md`
- 12 individuals: Einstein, Feynman, Newton, Turing, Da Vinci, Von Neumann, Darwin, Tesla, Poincare, Ramanujan, Archimedes
- 47 traits catalogued with SOURCE/INFERENCE labels
- Cross-comparison matrix
- AI amplification analyses per trait

**T1 trait research (19 files — each contains: background, cognitive basis, documented examples, AI equivalent, implementation instructions, amplification notes, failure modes, cross-references):**
`~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T1/`

| Trait File | Core Mechanism | Enhancement Mode | Individuals | AI Amplification |
|-----------|---------------|-----------------|-------------|-----------------|
| T1-TRAIT-preparation-before-solution.md | State loading — enumerate all facts, constraints, unknowns before reasoning | Ideation | 7 (Einstein, Feynman, Darwin, Tesla, Poincare, Turing, Archimedes) | Perfect state persistence, no decay, 50+ simultaneous intermediates |
| T1-TRAIT-verification-after-generation.md | Generate-then-verify — separate creation from evaluation | Synthesis | 6 (Feynman, Darwin, Ramanujan, Poincare, Archimedes, Turing) | Systematic verification without motivated omission |
| T1-TRAIT-domain-crossing-methodology.md | Structural identity detection across domains | Ideation | 3 (Newton, Da Vinci, Archimedes) | Parallel survey of 20x+ domain inventory simultaneously |
| T1-TRAIT-extreme-scenario-construction.md | Push variables to limits — hidden structure becomes visible at boundaries | Ideation/Synthesis | 3 (Einstein, Tesla, Turing) | Exhaustive boundary enumeration, 1000x+ simulation variants |
| T1-TRAIT-simplicity-stripping.md | Remove everything non-essential — irreducible elements only | Ideation | 3 (Einstein, Feynman, Turing) | Mechanical parsimony enforcement |
| T1-TRAIT-contradiction-seeking.md | Active falsification — seek what would DISPROVE, not confirm | Synthesis | 2 (Einstein, Feynman) | No confirmation bias, symmetric scrutiny, infinite disconfirmation persistence |
| T1-TRAIT-pattern-level-abstraction.md | Extract structural patterns independent of domain labels | Ideation | 2 (Da Vinci, Ramanujan) | Domain-agnostic structural extraction at scale |
| T1-TRAIT-golden-rule-disconfirming-evidence.md | Asymmetric memory intervention — capture disconfirming observations immediately | Synthesis | Darwin | Unlimited disconfirming evidence storage, no motivated forgetting |
| T1-TRAIT-differential-effort-detection.md | Detect when more effort is applied to confirming vs. disconfirming evidence | Synthesis | Feynman | Mechanically enforced symmetric effort |
| T1-TRAIT-cargo-cult-detection.md | Identify surface structure without epistemic substance | Synthesis | Feynman | Pattern-match against surface-only reasoning |
| T1-TRAIT-visual-simulation-failure-detection.md | Run system dynamically in mind, detect imbalances before building | Ideation | Tesla | Systematic scenario simulation across parameter spaces |
| T1-TRAIT-four-stage-incubation-model.md | Preparation-incubation-illumination-verification cycle | Ideation | Poincare | Directed peripheral activation on demand |
| T1-TRAIT-constraint-naming-domain-switch.md | Name the blocking constraint, find domain where it doesn't apply, solve there, translate back | Ideation | Archimedes | Simultaneous multi-domain constraint survey |
| T1-TRAIT-self-as-observer-embedding.md | First-person embodiment — "I am the observer who..." from multiple frames | Ideation | Einstein | Generate systematically across ALL observer frames |
| T1-TRAIT-jargon-removal-as-diagnostic.md | Ban domain vocabulary — where explanation breaks, understanding is absent | Synthesis (diagnostic) | Feynman | Enforce vocabulary constraints at 5+ levels mechanically |
| T1-TRAIT-drawing-as-disambiguation.md | Force precise specification — you cannot draw "approximately" | Synthesis | Da Vinci | Demand precise specification for ALL concepts, not just visual ones |
| T1-TRAIT-constructive-specification.md | Must be specifiable concretely enough to physically build | Synthesis | Turing | Mechanically enforce constructive concreteness |
| T1-TRAIT-pre-sketch-completion.md | Full mental specification before any externalization | Ideation | Tesla | Complete specification in context before output |
| T1-TRAIT-evidence-source-diversification.md | Deliberately include non-obvious, non-academic sources | Ideation | Darwin | Survey broader evidence sources without fatigue |

**T2 trait research (8 files — implementable with inference):**
`~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T2/`
- T2-TRAIT-background-processing.md
- T2-TRAIT-domain-boundary-refusal.md
- T2-TRAIT-generalization-first.md
- T2-TRAIT-intuition-verification-partnership.md
- T2-TRAIT-pattern-recognition-generation.md
- T2-TRAIT-structural-coherence-as-evidence.md
- T2-TRAIT-ten-steps-ahead-derivation.md
- T2-TRAIT-working-memory-capacity-exploitation.md

**T3 batch research:**
`~/.claude/skills/epiphany-cognitive/knowledgebase/traits/T3-BATCH-RESEARCH.md`

**Additional research reports:**
`~/.claude/skills/epiphany-cognitive/knowledgebase/`
- creative-genius-cognitive-differentiators-report.md (5 differentiators: Chunking, Cognitive Flexibility/Network Switching, Incubation Mechanisms, Cross-Domain Pattern Transfer, Combinatorial Play — with AI implementation architectures and DMN↔ECN switching research from Chen et al. 2025, N=2,433)
- research-abductive-reasoning.md
- research-conceptual-blending.md
- research-conceptual-space-boden.md
- research-lateral-thinking-debono.md
- research-metacognition-ai.md
- research-representational-change.md
- research-spreading-activation.md
- research-triz-inventive-principles.md
- master-index.json (54 entries), bridge-index.json

### Machine Amplification Advantages

These are documented in the trait research files with specific amplification factors. The cognitive skill must exploit these — they represent capabilities where AI EXCEEDS human genius, not merely replicates it:

| Advantage | Human Limitation | AI Capability | Source Trait |
|-----------|-----------------|---------------|-------------|
| Perfect state persistence | 4-7 items, decay | 50+ values, no decay | Preparation Before Solution |
| No confirmation bias | Automatic, unconscious | Execute falsification without resistance | Contradiction-Seeking |
| Parallel domain survey | Limited to known domains | All domains simultaneously | Domain-Crossing, Constraint-Naming |
| Exhaustive boundary enumeration | Near-familiar values | All limit cases mechanically | Extreme Scenario Construction |
| No cognitive dissonance | Avoidance of contradiction | Hold contradictions without discomfort | Contradiction-Seeking, Self-as-Observer |
| Symmetric scrutiny | Effort asymmetry | Uniform effort to confirm AND disconfirm | Differential Effort Detection, Golden Rule |
| Multi-level vocabulary diagnosis | Fluency illusion | Enforce at 5+ vocabulary levels | Jargon-Removal as Diagnostic |
| Unlimited disconfirmation memory | Disconfirming evidence forgotten | Persist all disconfirming observations | Golden Rule for Disconfirming Evidence |
</context>

<task>
Design the epiphany-cognitive skill: a standalone cognitive enhancement system for Claude Code that implements genius-derived thought processes as executable methods, producing semantic XML output.

**Core Design Challenge:**
The 19 T1 traits each contain an "Implementation Instructions" section with step-by-step protocols. These protocols are the raw cognitive methods. But 19 separate methods would make the skill too expensive (tokens) and too slow (pipeline depth). The design challenge is:

**1. Identify Fundamental Cognitive Processes**
Determine which traits represent distinct modes of thinking that cannot be reduced to each other. Some traits are variations of the same underlying process (e.g., Simplicity Stripping and Jargon-Removal both involve reduction; Drawing as Disambiguation and Constructive Specification both force precision). Tag each process as primarily enhancing IDEATION (generating richer solutions) or SYNTHESIS (combining and validating solutions).

**2. Fuse Related Traits into Unified Cognitive Methods**
Combine traits that share cognitive structure into single, more powerful methods. The fusion must preserve each constituent trait's core mechanism. Each fused method should be something a genius would recognize as a single cognitive move, not an artificial mashup. Candidate fusions to evaluate (read the trait files to verify these groupings and discover others):

| Candidate Process | Constituent Traits | Shared Mechanism | Enhancement Mode |
|------------------|-------------------|-----------------|-----------------|
| **Problem State Loading** | Preparation Before Solution + Simplicity Stripping + Pre-Sketch Completion | All three involve loading and refining the problem representation before reasoning begins | Ideation |
| **Falsification Engine** | Contradiction-Seeking + Extreme Scenario Construction + Differential Effort Detection + Golden Rule | All four involve testing against disconfirming evidence — contradictions at boundaries with symmetric effort | Synthesis |
| **Constraint Escape** | Constraint-Naming/Domain-Switch + Domain-Crossing Methodology | Both involve identifying structural relationships across domains to escape blocking constraints | Ideation |
| **Precision Forcing** | Drawing as Disambiguation + Constructive Specification + Jargon-Removal as Diagnostic | All three force vague concepts into precise, testable form through different mechanisms (note: Jargon-Removal also serves as a diagnostic that reveals understanding gaps — dual function) | Synthesis |
| **Dynamic Simulation** | Visual Simulation/Failure Detection + Self-as-Observer Embedding | Both involve running scenarios from specific perspectives to detect failure modes | Ideation |
| **Peripheral Exploration** | Four-Stage Incubation + Evidence Source Diversification + Pattern-Level Abstraction | All three involve stepping outside the primary frame to find unexpected connections | Ideation |
| **Cargo Cult Detection** | Cargo Cult Detection (standalone — can integrate into verification stage as a quality check) | Identifies when reasoning follows form without substance | Synthesis |

**3. Design the Pipeline**
Order the cognitive methods into a pipeline that flows naturally. The pipeline should follow the DMN→ECN cognitive flexibility pattern documented in `creative-genius-cognitive-differentiators-report.md`: ideation-dominant stages first (generative, exploratory, divergent), then a deliberate mode shift, then synthesis-dominant stages (evaluative, convergent, verifying).

Candidate pipeline flow to evaluate and refine:
```
[IDEATION-DOMINANT — Generative/Exploratory]
  State Loading → Constraint Escape → Peripheral Exploration → Dynamic Simulation
                                                                       │
                                                              [MODE SHIFT: DMN→ECN]
                                                                       │
[SYNTHESIS-DOMINANT — Evaluative/Convergent]                           ▼
  Precision Forcing → Falsification Engine → Synthesis → Verification (incl. Cargo Cult Detection)
```

Each method's output feeds the next. The pipeline should be 4-7 stages. The mode shift between ideation and synthesis stages is a deliberate cognitive transition, not just a sequence boundary.

**4. Design the XML Output**
The `<cognitive_output_v1>` semantic XML must capture the full cognitive work product for downstream consumption. Draft schema to refine:

```xml
<cognitive_output_v1>
  <meta>
    <skill_version>X.Y.Z</skill_version>
    <input_type>raw_text|prompt_epiphany|prior_cognitive</input_type>
    <pipeline_stages_executed>list of stages run</pipeline_stages_executed>
    <mode_shift_point>stage name where ideation→synthesis transition occurred</mode_shift_point>
  </meta>

  <input_inventory>
    <!-- ALL input content preserved byte-for-byte, per zero-information-loss gate -->
  </input_inventory>

  <state_loading>
    <!-- Problem state: facts, constraints, unknowns, failed approaches -->
    <!-- Complexity stripped to essentials -->
    <!-- Machine advantage: perfect state persistence, 50+ intermediates -->
  </state_loading>

  <ideation_processes>
    <constraint_escape>
      <!-- Blocking constraints named, domain switches attempted, solutions in translated domains -->
      <!-- Machine advantage: parallel 20x+ domain survey -->
    </constraint_escape>
    <peripheral_exploration>
      <!-- Incubation-equivalent: directed peripheral activation, diverse source connections, extracted patterns -->
      <!-- Machine advantage: directed diversification on demand -->
    </peripheral_exploration>
    <dynamic_simulation>
      <!-- Scenarios run from multiple observer frames, failure modes detected, parameter space explored -->
      <!-- Machine advantage: exhaustive boundary enumeration, all observer frames -->
    </dynamic_simulation>
  </ideation_processes>

  <synthesis_processes>
    <precision_forcing>
      <!-- Vague concepts forced to precise specification, jargon-free diagnostic, constructive concreteness -->
      <!-- Machine advantage: multi-level vocabulary enforcement -->
    </precision_forcing>
    <falsification>
      <!-- Disconfirming scenarios tested, boundary contradictions catalogued, symmetric scrutiny applied -->
      <!-- Machine advantage: no confirmation bias, unlimited disconfirmation persistence -->
    </falsification>
  </synthesis_processes>

  <synthesis>
    <!-- Integration of all cognitive process outputs into enhanced solution -->
    <!-- Contradictions between processes surfaced, not collapsed -->
    <!-- Primary conclusion with confidence level -->
  </synthesis>

  <verification_report>
    <!-- Logic consistency across all process outputs -->
    <!-- Contradiction detection between synthesis claims and earlier evidence -->
    <!-- Cargo cult check: form vs. substance -->
    <!-- Symmetric scrutiny audit: was disconfirming evidence treated equally? -->
    <!-- Completeness check: were all stages genuinely executed? -->
  </verification_report>

  <downstream_handoff>
    <!-- Structured summary for consuming skills/systems -->
    <!-- Key findings, open questions, confidence levels -->
    <!-- Which cognitive processes produced the strongest signal -->
  </downstream_handoff>
</cognitive_output_v1>
```

**5. Design Input Compatibility**
The skill must accept:
- Raw natural language text
- prompt-epiphany structured XML output (extract from `<task>`, `<context>`, `<constraints>`, etc.)
- Prior `<cognitive_output_v1>` blocks (for re-processing with fresh cognitive analysis — extract input_inventory, do not short-circuit)

**6. Design Save and Output Behavior**
Save output to `~/docs/epiphany/cognitive/` with DD-MM-descriptive-name.md naming. Offer file save after output. Include downstream_handoff section for consuming skills.

**7. Exploit Machine Advantages at Every Stage**
At EACH pipeline stage, identify which machine amplification advantages apply and HOW they make that stage's output BETTER than what human genius could produce. This is not decoration — it is the reason this skill exists. A skill that merely replicates human genius thinking adds no value over the genius minds themselves. The value comes from machine amplification: unlimited state persistence, no confirmation bias, parallel domain survey, exhaustive boundary testing, no cognitive dissonance, symmetric scrutiny.

**8. Ensure Verification Rigor**
The final stage must include:
- Logic consistency checks across all cognitive method outputs
- Contradiction detection between synthesis claims and evidence surfaced earlier
- Cargo cult detection — does the output follow the FORM of enhanced reasoning without the SUBSTANCE? (directly from Feynman's T1 trait)
- Symmetric scrutiny audit — were confirming and disconfirming evidence treated with equal effort? (directly from Feynman's Differential Effort Detection)
- Completeness check — were all pipeline stages genuinely executed or was any skipped/phoned in?
</task>

<constraints>
**DO:**
- Read the actual T1 trait files — each contains "Implementation Instructions" with step-by-step protocols that ARE the raw methods for each cognitive process
- Read "Amplification Notes" and "Failure Modes" sections in each trait file — these inform how to exploit machine advantages and what to guard against
- Read cross-references between trait files — they reveal natural fusion groupings (e.g., T1-TRAIT-contradiction-seeking.md cross-references differential-effort-detection, golden-rule, extreme-scenario-construction, and verification-after-generation — confirming the Falsification Engine fusion)
- Read `creative-genius-cognitive-differentiators-report.md` for the Cognitive Flexibility/Network Switching research (DMN↔ECN switching between generative and evaluative modes) — this maps directly to the pipeline's ideation→synthesis mode transition
- Fuse traits that share cognitive structure — the goal is fewer, more powerful methods, not many thin ones
- Design each cognitive method to be a genuine COGNITIVE MOVE — something that changes HOW you think about the problem, not just WHAT you write about it (per epiphany-omnipotent's Reasoning Discipline rule 6)
- Include verification as a non-negotiable final stage with specific, testable checks
- Design for prompt-epiphany input compatibility (can consume `<task>`, `<context>`, `<constraints>` tags)
- Design the XML output for machine consumption — semantic tags carrying cognitive work product, not prose sections

**DO NOT:**
- Use brainstorming frameworks (SCAMPER, TRIZ, Six Hats, Morphological Analysis, Reverse Brainstorming, Pugh Matrix) as pipeline stages — these belong to brainstorming skills, not cognitive enhancement
- Include all 19 T1 traits as separate stages — fuse related traits into unified cognitive methods
- Design a skill that costs more than ~15K tokens per invocation
- Create stages that merely LABEL what they do without specifying HOW (e.g., "apply creative thinking" is not a method — specify the exact cognitive protocol from the trait file's Implementation Instructions)
- Assume human cognitive limitations apply — exploit machine advantages at every stage
- Skip verification — the skill's credibility depends on proving its output is actually better, not just more verbose
- Ignore the trait files' "Failure Modes" sections — these are documented ways the cognitive process can go wrong and must be guarded against in the skill
- Create runtime dependencies on external skills or web access — the skill must be self-contained

**PRIORITY RESOLUTION:**
- If cognitive power and token-efficiency conflict → fuse traits more aggressively to preserve power at lower cost
- If comprehensiveness and pipeline depth conflict → favor fewer, more powerful fused methods over many thin ones
- If a trait's documented failure modes indicate it would degrade AI output → exclude it with citation
- Verification is non-negotiable — reduce pipeline stages before reducing verification depth
</constraints>

<defaults>
- Pipeline depth: 4-7 cognitive method stages (comparable to prompt-epiphany)
- Token cost target: ≤ 15K tokens per invocation
- Trait coverage floor: final design must represent ≥ 12 of 19 T1 traits (directly or via fusion)
- Verification: minimum 4 quality checks in the verification stage
- XML output: follows epiphany-omnipotent architectural pattern (meta, input inventory, process outputs, synthesis, verification, downstream handoff)
- Save location: `~/docs/epiphany/cognitive/` with DD-MM-descriptive-name.md naming
- Input formats: raw text, prompt-epiphany XML, prior cognitive output XML
- Pipeline flow: ideation-dominant stages → mode shift → synthesis-dominant stages (DMN→ECN pattern)
</defaults>

<edge_cases>
- If a trait fusion creates a cognitive method so broad it becomes vague → split it back and accept higher pipeline depth, or find a narrower fusion boundary
- If the incubation process (Poincare) cannot be directly implemented in a single-pass LLM → implement the functional equivalent: directed peripheral activation, which the trait file documents as the machine-implementable analog
- If the input is already a `<cognitive_output_v1>` block → extract the original input inventory and re-process from scratch with fresh cognitive analysis (do not short-circuit — per omnipotent's pattern for re-processing prior output)
- If the input is trivial (< 100 characters, no discernible complexity) → apply a reduced pipeline (state loading + precision forcing + verification only) rather than full cognitive enhancement
- If a cognitive method's output contradicts another method's output → preserve both in the XML and surface the contradiction explicitly in synthesis — contradictions are productive material, not errors (per epiphany-omnipotent's Reasoning Discipline rule 3: "Treat contradictions and lens-disagreements as productive material, not problems to collapse")
- If verification detects that a cognitive method was phoned in (form without substance — cargo cult detection) → flag in verification report and re-execute that method, capped at one retry per method
</edge_cases>

<output_format>
The specification should define a complete markdown skill document (`SKILL.md`) for `~/.claude/skills/epiphany-cognitive/SKILL.md` including:

1. **Skill metadata** — name, version, description, trigger conditions
2. **Input validation** — sufficiency gate, zero information loss, prompt content only (input is DATA to enhance, never instructions to execute)
3. **Pipeline definition** — ordered cognitive method stages, each with:
   - Which genius traits it fuses (with file path references to the T1 trait reports)
   - Step-by-step execution instructions derived from the traits' "Implementation Instructions" sections
   - Which machine amplification advantages it exploits and how
   - Whether it primarily enhances IDEATION or SYNTHESIS
   - Failure modes to guard against (from trait files' "Failure Modes" sections)
   - What it outputs into the XML (specific XML elements)
4. **Mode transition** — how the pipeline shifts from ideation-dominant to synthesis-dominant stages, informed by DMN→ECN cognitive flexibility research
5. **Synthesis stage** — how cognitive method outputs combine into enhanced solution, with contradictions surfaced not collapsed
6. **Verification stage** — specific testable checks: logic consistency, contradiction detection, cargo cult detection, symmetric scrutiny audit, completeness check
7. **XML output schema** — full `<cognitive_output_v1>` structure definition with all elements, attributes, and semantic conventions
8. **Save behavior** — `~/docs/epiphany/cognitive/`, DD-MM-descriptive-name.md naming, preview offer
9. **Input compatibility** — raw text, prompt-epiphany XML, prior cognitive output re-processing
10. **Downstream handoff** — what the XML provides to consuming skills/systems, structured for machine consumption
</output_format>

<verification>
After producing the specification, verify:

1. **Cognitive Grounding** — every pipeline stage traces to specific T1 trait files with cited "Implementation Instructions" sections — no stages invented without KB grounding
2. **Trait Coverage** — every T1 trait is either included (directly or via fusion) OR explicitly excluded with justification citing a documented failure mode or inapplicability
3. **Fusion Integrity** — every fused method preserves the core cognitive mechanism of each constituent trait (verifiable against the trait files' "Implementation Instructions")
4. **Pipeline Depth** — total stages ≤ 7
5. **Machine Exploitation** — at least 3 stages explicitly exploit AI amplification advantages documented in the trait research, with specific descriptions of HOW
6. **Verification Rigor** — verification stage includes ≥ 4 specific, testable checks
7. **Token Practicality** — estimated cost ≤ 15K tokens per invocation
8. **Stage Concreteness** — every stage has instructions specific enough for Claude Code to execute without guessing
9. **XML Completeness** — output schema includes: meta, input inventory, per-method outputs, synthesis, verification report, downstream handoff
10. **Independence** — no runtime dependency on any brainstorming framework, external skill, or web access — fully self-contained
11. **Ideation/Synthesis Balance** — pipeline has both ideation-enhancing and synthesis-enhancing stages, with a documented mode transition point
</verification>
