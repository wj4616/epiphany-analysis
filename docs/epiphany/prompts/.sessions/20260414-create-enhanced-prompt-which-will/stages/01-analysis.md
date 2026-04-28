# 01-analysis.md — M12 Phase 1 (STANDARD)

## INTENT

**Primary goal:** Produce an enhanced prompt that will be fed as input to the `brainstorming` skill so the brainstorming skill can write a detailed spec for a new Claude Code skill.

**Desired end state:** A single, well-structured prompt (not the skill itself, not the spec) that, when passed to the `brainstorming` skill, yields a detailed specification for a skill called (informally) `epiphany-analysis` — a skill that:
1. Takes an input (Node A) and an analysis of that input (Node B, ideally produced by `epiphany-genius`).
2. Thinks about how the analysis can be used to improve the original input.
3. Produces a new improved version of the input.
4. Runs programmatically and is modular, designed to integrate with `epiphany-genius`'s modular architecture.
5. Follows a tree-of-thought / graph-of-thought pipeline (A → B → A1/B1 → C → D → E → final enhanced output).
6. Borrows the ideation/synthesis/verification process from `prompt-epiphany`.
7. Tailors analysis to each of the 10 main sections of an `epiphany-genius` report.
8. Performs self-audit / verification for correctness and completeness.
9. Saves a fresh copy of the enhanced output (never overwrites the original).

**Success criteria (the enhanced brainstorming prompt should enable a spec that):**
- Captures the full Node A → Node E pipeline verbatim, including all iteration rules.
- Preserves the reference analysis artifact (`file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`) as the canonical example.
- Specifies detection logic for `epiphany-genius` reports (including slight version drift) AND a fallback path for arbitrary analyses.
- Encodes the 10-section tailored-analysis requirement with performance-bounded scope.
- Mandates ideation / synthesis / verification loops (modeled on `prompt-epiphany`).
- Preserves the "two-solutions, compare pro/con, pass only best" rule.
- Preserves the final self-audit + fresh-copy-save behavior.
- Retains all user anti-patterns (no overengineering, no regression, realistic to implement, human-usable).

## STRUCTURE

**Current organization of the input:** A single free-form markdown blob, ~84 lines, containing:
- A preamble paragraph stating the meta-goal (enhanced brainstorming prompt).
- An `#`-heading sub-description of the skill-to-be-built.
- A free-associative bullet list describing the Node A–E pipeline.
- A second bullet cluster (near line 68+) repeating a parallel pipeline with node variables `x2`, `x3` (likely a restatement of the same concept from a different angle).
- Embedded references to sibling skills (`prompt-epiphany`, `epiphany-genius`, `brainstorming`).
- An embedded file URL to a concrete reference analysis.

**Structural issues:**
- **No role / persona framing.** The consumer (brainstorming skill) has no declared target audience or expert framing.
- **No output format spec.** Nothing tells the brainstorming skill what shape its resulting spec should take.
- **No constraint block.** DO/DO NOT rules are scattered across bullets.
- **Redundant pipeline.** The Node A–E pipeline and the parallel `x2/x3` pipeline describe largely the same flow; whether they are two specs or one restatement is ambiguous.
- **No decomposition.** Intent, signal-flow nodes, iteration rules, KB references, anti-patterns, and verification requirements are all interleaved.
- **No priority hierarchy.** When "run until out of ideas" conflicts with "can't let this process take too long," there's no tiebreaker.
- **Mixed imperative voice.** Some bullets are design directives ("create idea storage node C"), some are meta ("be creative"), some are runtime-agent behavior ("do research or ask questions as necessary").

**Missing structural elements:**
- `<role>` — declaration that the consumer is the brainstorming skill, writing a spec for a Claude Code agent skill.
- `<context>` — what `prompt-epiphany`, `epiphany-genius`, and the existing Node pipeline convention are.
- `<task>` — single clear ask: write a detailed spec for the described skill.
- `<constraints>` — DO/DO NOT distilled.
- `<pipeline>` — the canonical Node A → E flow.
- `<section_tailoring>` — the 10-section report handling requirement.
- `<references>` — file URL + sibling skill names.
- `<verification>` — self-audit + fresh-copy-save rules.
- `<output_format>` — shape of the spec the brainstorming skill should return.

## CONSTRAINTS

**Explicit constraints in input:**
- Must run programmatically and be modular.
- Must be ready to integrate with `epiphany-genius` modular system.
- Must allow precise, optimal step-following.
- Full node pipeline (A, B, A1, B1, C, D, E) is required; the outline is only a rough sketch and must be enhanced into a full tree-of-thought or graph-of-thought.
- Every detail must be reviewed — no generalizing, no glossing over (Phase 0).
- Each idea stored as an individual element (Node C / `x2`).
- Each idea must be analyzed for pro/con/utility before adoption (Node D / `x2`).
- Two separate solutions generated, compared, only best passed (Node E / `x3`).
- Ideation / synthesis / verification pattern borrowed from `prompt-epiphany`.
- Iterate over every idea — don't miss anything.
- Run until out of ideas completely.
- Must check whether analysis is product of any version of `epiphany-genius` (including slightly different versions); if not, still perform enhancement.
- Each of the 10 main sections of an `epiphany-genius` report gets independent analysis tailored to that section.
- Headline insight / primary conclusion is the primary initial enhancement anchor.
- Theory collisions: for each collision, analyze claim A vs. claim B, account for discriminating condition, decide meaning, craft enhanced solution.
- Alternative hypotheses: most are wrong; only one is correct — treat accordingly.
- Final integration must be "flush" (bug-free, no errors or potential problems).
- Final verification / self-audit required.
- Fresh separate final spec saved — do NOT overwrite existing specification.
- Optimized for Claude Code AI agent system.
- May do research or ask questions as necessary.

**Implicit constraints that should be made explicit:**
- The enhanced prompt's consumer is the `brainstorming` skill, not a human.
- Output of the brainstorming skill is a "detailed spec" (not code, not the skill itself).
- Process must be fast enough to be practical ("can't let this process take too long") — this is a soft performance budget.
- The node pipeline is a conceptual model; it need not imply a literal graph database — "AI optimized database" likely means a structured in-context store.
- Usable for humans: outputs should be human-readable.
- Realistic to implement: no fantasy features, no infinite loops without termination.
- No regression: enhanced version must not lose information from original.

**Conflicts / tensions (flagged, NOT resolved):**
- **CONFLICT 1:** "run until out of ideas completely" vs. "can't let this process take too long." No tiebreaker or budget expressed.
- **CONFLICT 2:** The input restates the pipeline twice (Nodes A–E and nodes referenced as `x2`, `x3`). It is unclear whether these are (a) the same pipeline described twice for emphasis, (b) a nested sub-pipeline that runs per-section, or (c) a second parallel design the user wants merged. The most plausible reading is (b) — the `x2/x3` wording appears in the 10-section-per-report context — but this is not stated outright.
- **CONFLICT 3:** "create the ideal skill" (broad creative license) vs. "must be realistic to implement without overcomplexity or regression" (conservative). These are complementary guardrails, but can clash in spec detail.
- **CONFLICT 4:** "do research or ask questions as necessary" is a runtime-agent directive, but this prompt is being fed to the brainstorming skill to produce a SPEC. Is the "research / ask questions" behavior (a) something the brainstorming skill should do while writing the spec, or (b) something the resulting skill should do at runtime? Most plausibly (b), but ambiguous.

## TECHNIQUES

| ID | Name | Present? | Needed? | Impact | Notes |
|----|------|----------|---------|--------|-------|
| T1 | XML semantic structuring | No | **Yes** | High | Current prompt is a single blob; needs `<role>`, `<context>`, `<task>`, `<pipeline>`, `<constraints>`, `<references>`, `<verification>`, `<output_format>`. |
| T2 | Prompt decomposition | Partial (bullets) | **Yes** | High | Bullets exist but are not labeled or grouped semantically. Needs explicit sections. |
| T3 | Explicit constraint specification | No | **Yes** | High | DO/DO NOT list is not consolidated. |
| T4 | Role/persona assignment | No | **Yes** | Medium | Persona: expert spec-writer for Claude Code modular skills, familiar with `prompt-epiphany` and `epiphany-genius`. Goes in `<role>` per T4 element rule. |
| T5 | Output format templates | No | **Yes** | High | Brainstorming skill needs shape for its spec: sections, required fields, schema cues. |
| T6 | Structured reasoning injection | No | **Yes** | Medium | Encourage the brainstorming skill to reason through the A→E pipeline node-by-node before writing the spec. |
| T7 | Priority hierarchy | No | **Yes** | Medium | Resolves CONFLICT 1 and CONFLICT 3 (completeness vs. speed, creativity vs. realism). |
| T8 | Boundary/edge case spec | Partial | **Yes** | High | Input mentions epiphany-genius-version-drift edge case and non-genius-analysis fallback; make these explicit edge cases with handling rules. |
| T9 | Few-shot exemplar injection | No | Optional | Low | The reference report URL effectively IS the exemplar; labeling it as such would help but is not essential. |
| T10 | Self-critique/validation | No | **Yes** | Medium | Final verification is in input; bake a self-audit step into the brainstorming output contract. |
| T11 | Context preservation anchoring | No | **Yes** | Medium | Labels needed early: "Node A", "Node B", "A1/B1", "Node C", "Node D", "Node E", "10 main sections", "theory collisions", "alternative hypotheses". |
| T12 | Audience calibration | No | **Yes** | High | Consumer = brainstorming skill producing spec for Claude Code agent; assumed knowledge = `prompt-epiphany` + `epiphany-genius` + modular-skill conventions. |
| T13 | Escape hatch provision | No | **Yes** | Low-Medium | If analysis structure cannot be parsed as epiphany-genius and has unknown shape, state what's missing. |

**Application order (per SKILL.md):** T2→T1→T4→T3→T7→T6→T5→T8→T12→T9→T11→T10→T13.

## WEAKNESSES

1. **No consumer declaration.** Nothing tells the brainstorming skill that it IS the consumer, or that its job is to produce a detailed spec. The prompt drifts between describing a meta-task (enhance brainstorming input) and describing the skill-to-be-built. Likely misinterpretation: brainstorming skill treats this as a request to build the skill itself.

2. **Pipeline duplication ambiguity.** Two parallel pipelines (Nodes A–E and the x2/x3 restatement) — unclear if they are the same. Risk: spec ends up with either a missing step or double-counted steps.

3. **Completeness vs. speed tension has no tiebreaker.** "Run until out of ideas" + "can't take too long" without priority = unbounded ideation risk. Likely misinterpretation: unbounded loop in the resulting skill.

4. **"Creative / realistic" tension has no tiebreaker.** "Be creative, explore new perspectives" + "no overengineering / no complexity / realistic to implement" — need priority rule.

5. **Vague storage language.** "AI optimized database," "databases in system expand and create relationships as necessary for ideal design" is evocative but implementation-ambiguous. A Claude Code skill doesn't literally maintain a database between runs — most plausibly this means structured in-context storage (stage files or in-context YAML). Unclear.

6. **Ambiguous "research / ask questions" directive.** Applies to the spec-writer, the runtime skill, or both? Unspecified.

7. **10-section tailoring is under-specified.** Input says each of the 10 main sections gets independent analysis "tailored for that specific section," but does not enumerate what each section IS or what tailoring looks like. The reference report file URL must be loaded to discover this.

8. **Headline insight vs. theory collision vs. alternative hypothesis handling is scattered.** Three section-specific behaviors are named but not unified into a section-handling map.

9. **Verbatim preservation of node names is critical but fragile.** Node A/B/A1/B1/C/D/E naming is structural. Paraphrasing (e.g., calling Node C "the idea store") would break the referent chain the user has carefully constructed.

10. **Filename / file-URL preservation.** The embedded file URL (`file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`) is a precision-critical reference. Any truncation, scheme change, or path mangling would break the spec.

11. **Embedded directive "check if analysis is product of epiphany-genius" is both a runtime behavior AND a spec-level requirement.** Current wording blurs these.

12. **Output file naming / location for the "fresh copy" is unspecified.** "save a fresh copy of the full enhanced output only" — where? Under what name? This is a spec gap the brainstorming skill should be prompted to resolve.

13. **No explicit anti-pattern list.** DO NOTs are implied (no overengineering, no regression, no overwriting original, no infinite loops) but not stated as a block.

14. **Success criteria for the enhanced output are unstated.** What makes the enhanced Node A "enhanced"? No measurable check described.

15. **The meta-task framing is thin.** The opening sentence ("create an enhanced prompt which will work for input to brainstorming skill to write detailed spec from:") is the only meta-framing; everything else is skill-content. Easy for the brainstorming skill to lose track of who it is and what it is producing.
