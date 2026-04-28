# 02-ideation.md — M12 Phase 2 (STANDARD)

Scale variant: **STANDARD**. Contracts are ordered so T2/T1 structure comes first, then T4 persona, then constraint/priority, then reasoning/format, then audience/edge/anchor/critique/escape. Every contract passed the five-gate test (Impact, Risk, Validity, Necessity, Preservation).

---

## Primary enhancement contracts

```yaml
- technique: T2
  target_section: "global"
  action: "Decompose the monolithic prompt into the following labeled sections in this order: <role>, <context>, <task>, <pipeline>, <section_tailoring>, <constraints>, <priority_rules>, <edge_cases>, <references>, <verification>, <output_format>. Preserve every Node A/B/A1/B1/C/D/E reference, every iteration rule, every verification rule, and every embedded directive from 01-inventory.md verbatim across these sections — no summarization."
  rationale: "Current input is a free-form blob with bullets that mix persona, pipeline, constraints, edge cases, and verification. The brainstorming skill cannot reliably write a detailed spec until the source prompt is semantically segmented. Decomposition is the foundation for every subsequent enhancement."
  priority: high

- technique: T1
  target_section: "global"
  action: "Wrap the decomposed sections in XML tags (<role>, <context>, <task>, <pipeline>, <section_tailoring>, <constraints>, <priority_rules>, <edge_cases>, <references>, <verification>, <output_format>). Use <node id='A'>, <node id='B'>, <node id='A1'>, <node id='B1'>, <node id='C'>, <node id='D'>, <node id='E'> as child elements inside <pipeline> so each node's purpose, inputs, outputs, and iteration rule can be read independently by the brainstorming skill. Also wrap the parallel x2/x3 pipeline inside <pipeline> as a clearly labeled <section_pipeline> child that makes it explicit whether it is (a) the same flow described twice or (b) an inner loop that runs per-section of the analysis — flag the ambiguity rather than silently picking one reading."
  rationale: "XML structure makes each node and each iteration rule individually addressable; it also localizes the A–E pipeline vs. x2/x3 restatement ambiguity so the brainstorming skill treats it as a documented open question rather than silently merging the two."
  priority: high

- technique: T4
  target_section: "<role>"
  action: "Assign the persona: 'You are an expert specification author for modular Claude Code agent skills. You have deep familiarity with the prompt-epiphany skill (ideation→synthesis→verification pattern), the epiphany-genius skill (modular subagent orchestration, 10-section report shape), and the brainstorming skill's expected input contract. You are writing a detailed specification for a new skill provisionally named epiphany-analysis.' Keep this content exclusively inside <role>; do not leak persona into <context>."
  rationale: "No persona is assigned in the input. The brainstorming skill benefits from a calibrated expert framing aligned to the actual consumer (spec-writer for a Claude Code modular skill), which anchors scope and vocabulary. Per T4 element rule, persona must live in <role>, not <context>."
  priority: high

- technique: T3
  target_section: "<constraints>"
  action: "Consolidate scattered DO/DO NOT rules into a single <constraints> block, each as a single-line imperative: DO preserve every node label (A, B, A1, B1, C, D, E) verbatim in the spec; DO preserve the reference analysis URL (file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md) verbatim; DO specify ideation / synthesis / verification loops modeled on prompt-epiphany; DO mandate the 'generate two solutions, compare pro/con/utility/over-engineering, pass only best' rule at the solution-engineering step; DO specify the epiphany-genius detection check AND a fallback path for non-genius analyses; DO mandate a fresh separate final-output file that does NOT overwrite the original spec; DO NOT remove or paraphrase any item in the INVENTORY; DO NOT introduce infinite loops — every iterate/run-until rule must have a termination condition; DO NOT overengineer (realistic-to-implement, human-usable, no regression); DO NOT silently merge the two restated pipelines — either document them as equivalent or describe their relationship explicitly."
  rationale: "Input has implicit and explicit constraints scattered across ~40 bullets. A consolidated block prevents the brainstorming skill from dropping constraints during spec generation."
  priority: high

- technique: T7
  target_section: "<priority_rules>"
  action: "Add explicit tiebreakers for the four flagged conflicts: (1) 'If completeness of ideation conflicts with process-time budget, prefer completeness bounded by a declared max-iteration cap that the spec must define (e.g., per-section max-N-ideas and skill-global max-total-passes).' (2) 'If creative exploration conflicts with realistic-to-implement, prefer realistic-to-implement; creative ideation happens in Node C, but Node D MUST reject ideas that are not realistic.' (3) 'If the analysis is an epiphany-genius report (current or near-future version), use the 10-section tailored flow; otherwise fall back to the generic full-document flow.' (4) 'If an idea would cause regression or loss of information from Node A, discard it regardless of novelty.'"
  rationale: "Explicit priority rules resolve CONFLICT 1 (completeness vs speed), CONFLICT 3 (creative vs realistic), and disambiguate edge cases without removing user-stated directives. Directly improves success criteria 'no regression' and 'realistic-to-implement'."
  priority: high

- technique: T6
  target_section: "<task>"
  action: "Instruct the brainstorming skill to reason about the Node A → B → A1/B1 → C → D → E flow step-by-step before producing the spec: for each node, describe purpose, inputs, outputs, storage schema (in-context YAML / stage file), termination condition, and integration points with epiphany-genius modular system. Reasoning guidance should explicitly frame Nodes A/B/A1/B1 as the analysis phase, Node C as the unfiltered idea pool, Node D as the accepted-ideas filter, and Node E as the two-solutions-compared-and-best-chosen store."
  rationale: "Encourages the brainstorming skill to produce a spec that treats each node as a first-class component rather than a paraphrased bullet, which matches the user's stated need for precise, optimal step-following."
  priority: medium

- technique: T5
  target_section: "<output_format>"
  action: "Specify the expected spec shape the brainstorming skill should produce: numbered sections for (1) Skill identity & activation, (2) Module inventory (one module per major pipeline stage), (3) Node data model (A, B, A1, B1, C, D, E storage schemas), (4) Section-tailored analysis map (one entry per epiphany-genius section), (5) Ideation / synthesis / verification loops, (6) Two-solutions compare-and-pick rule, (7) epiphany-genius version detection + fallback, (8) Self-audit verification block, (9) Fresh-copy output file convention, (10) Termination conditions & iteration caps, (11) Integration points with epiphany-genius modular system, (12) Anti-patterns / DO NOT list. Do NOT constrain the spec to a specific markdown template — sections are required, but internal shape is up to the brainstorming skill."
  rationale: "No output shape is currently specified. Giving the brainstorming skill a section list guarantees coverage of every user concern without over-constraining its creative spec structure."
  priority: high

- technique: T8
  target_section: "<edge_cases>"
  action: "Enumerate edge cases as named rules with handling clauses: (a) 'Analysis is an epiphany-genius report, exact current version → run full 10-section tailored flow.' (b) 'Analysis is epiphany-genius-like but version-drifted (extra or renamed sections, new fields) → still detect as epiphany-genius via structural heuristic (presence of e.g. headline insight, theory collisions, alternative hypotheses sections) and run tailored flow with best-effort section mapping.' (c) 'Analysis is not epiphany-genius → run generic full-document flow: full read of Node B, extract improvement candidates across whole document.' (d) 'Theory collisions section: for each collision, compare claim A vs claim B, account for discriminating condition, emit a resolved finding.' (e) 'Alternative hypotheses section: treat as low-signal; only the hypothesis that best fits discriminating evidence is used — others discarded.' (f) 'Headline insight / primary conclusion: process first as the primary anchor for enhancement before other sections.' (g) 'Node A source is a file path → read file contents as Node A.' (h) 'Ideation produces zero ideas for a section → mark section complete and move on, do NOT retry indefinitely.'"
  rationale: "Input names these edge cases informally; making them explicit named rules prevents the spec from missing any, and directly addresses WEAKNESS 7, 8, 11."
  priority: high

- technique: T12
  target_section: "<context>"
  action: "State that the consumer of this prompt is the brainstorming skill, that the brainstorming skill's output is a DETAILED SPECIFICATION (not code, not the finished skill), that the target implementation environment is Claude Code's modular agent system, and that the spec's downstream reader is a human developer plus a future implementation agent. Reference prompt-epiphany (ideation/synthesis/verification pattern), epiphany-genius (modular subagent orchestration producing a 10-section report), and the brainstorming skill as known background — do not restate their internals. Keep <context> focused on situational facts; persona is in <role>."
  rationale: "Audience calibration prevents the brainstorming skill from drifting into implementing the skill itself or producing code. Also clarifies WEAKNESS 1 (no consumer declaration)."
  priority: high

- technique: T11
  target_section: "<context>"
  action: "Anchor key terms early by defining them once, then referencing consistently throughout: Node A (original input), Node B (analysis document), Node A1 (analysis of Node A), Node B1 (analysis of Node B), Node C (unfiltered idea pool), Node D (accepted ideas after pro/con filter), Node E (solutions store — two generated, best chosen), x2 / x3 (parallel per-section naming that appears in the input — explicitly flagged as potentially equivalent to C/D/E, to be documented in the spec), '10 main sections' (the section structure of an epiphany-genius report, to be enumerated by reading the reference URL), 'ideation/synthesis/verification' (the prompt-epiphany loop pattern), 'epiphany-analysis' (the skill being specified)."
  rationale: "The input reuses Node labels and pipeline terms across many bullets. Early anchoring keeps the brainstorming skill's spec internally consistent and directly addresses WEAKNESS 9 (verbatim preservation of node names)."
  priority: medium

- technique: T10
  target_section: "<verification>"
  action: "Instruct the brainstorming skill to self-audit the produced spec against a checklist before emitting: (1) Every INVENTORY item appears in the spec, unchanged in substance. (2) Every Node (A, B, A1, B1, C, D, E) has purpose, I/O, storage, and termination defined. (3) Every iteration rule in the input has a termination condition in the spec. (4) Both the epiphany-genius detection path AND the generic fallback path are specified. (5) The two-solutions compare rule appears at the solution-engineering step. (6) The fresh-copy output rule (never overwrite original) appears in the output-file section. (7) No DO NOT / anti-pattern from the constraints block is violated by any section. If any check fails, revise the spec and re-run the checklist until pass."
  rationale: "The input explicitly requires final self-audit/verification. Embedding a checklist in the prompt ensures the brainstorming skill's spec carries this forward AND self-verifies the spec itself before output."
  priority: medium

- technique: T13
  target_section: "<verification>"
  action: "Add escape hatches: 'If the reference analysis file at the embedded URL cannot be read, state that the 10-section enumeration must be inferred from the report content when available and document the unknown as a spec gap. If the parallel x2/x3 pipeline cannot be conclusively resolved as equivalent to C/D/E, emit the spec with both structures documented and flag the ambiguity as an explicit open question for the user. If a requested behavior cannot be specified without violating the no-overengineering constraint, state the tradeoff and recommend the simpler option.'"
  rationale: "Prevents the brainstorming skill from silently filling gaps or producing over-confident specs in areas where the input is under-specified. Directly addresses WEAKNESS 2, 6, 7, 12."
  priority: low

- technique: "other:reference-binding"
  target_section: "<references>"
  action: "Create a <references> section that preserves the full reference URL verbatim (file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md) alongside the embedded directive 'this is the main form of analysis that our new epiphany-analysis should target'. Also preserve the three sibling-skill references (prompt-epiphany, epiphany-genius, brainstorming skill) as named cross-references without paraphrasing their internals. This section is the single source of truth for precision-critical identifiers the spec must not mangle."
  rationale: "Isolating precision-critical external references into one section reduces the risk that they get truncated, paraphrased, or repositioned during synthesis — directly addresses WEAKNESS 10 (filename/file-URL preservation)."
  priority: high

- technique: "other:section-tailoring-map"
  target_section: "<section_tailoring>"
  action: "Add a dedicated <section_tailoring> block that requires the spec to produce a map: for each of the epiphany-genius report's 10 main sections, one row declaring (section name, section-specific focus, idea-extraction strategy, special handling rule if any). Pre-seed three known special handling rules from the input verbatim: headline insight / primary conclusion = primary anchor processed first; theory collisions = claim-A-vs-claim-B with discriminating condition; alternative hypotheses = low-signal, only best-fit hypothesis used. Require the spec to instruct the resulting skill to read the reference URL to enumerate the 10 sections if the list is not already known."
  rationale: "The 10-section tailoring requirement is central to the user's vision but under-specified. A dedicated block forces the spec to carry this forward as a first-class feature rather than a passing mention — directly addresses WEAKNESS 7 and 8."
  priority: high

- technique: "other:termination-budget-block"
  target_section: "<constraints>"
  action: "Add an explicit termination-and-budget sub-block inside <constraints> requiring the spec to define: (a) per-node max-iteration caps, (b) a skill-global max-total-pass cap, (c) a rule that 'run until out of ideas' means 'until a pass generates no new ideas' rather than 'unbounded', (d) a soft time/token budget guidance (not a hard number — the spec picks one). This binds the loose 'can't let this process take too long' directive to a concrete spec requirement without inventing numbers the user did not provide."
  rationale: "Resolves CONFLICT 1 (completeness vs. speed) at the spec level without stripping the 'run until out of ideas' directive. Directly addresses WEAKNESS 3 (completeness vs. speed has no tiebreaker)."
  priority: medium
```

---

## Weakness → contract coverage check

| Weakness | Covered by contract(s) |
|---|---|
| W1 No consumer declaration | T4, T12 |
| W2 Pipeline duplication ambiguity | T1 (flag as open question), T13 (escape hatch) |
| W3 Completeness vs speed tiebreaker | T7, other:termination-budget-block |
| W4 Creative vs realistic tiebreaker | T7 |
| W5 Vague storage language | T6 (node data model reasoning), T5 (spec section 3 requires storage schema) |
| W6 Ambiguous research/ask-questions directive | T3 constraints clarify runtime vs spec-writer scope implicitly; flagged for the spec to resolve |
| W7 10-section tailoring under-specified | T8, other:section-tailoring-map |
| W8 Headline/collision/hypothesis scattered | T8, other:section-tailoring-map |
| W9 Node-name verbatim preservation fragile | T11, T3 (DO preserve node labels) |
| W10 Filename/file-URL preservation | other:reference-binding, T3 |
| W11 epiphany-genius detection blurs runtime vs spec | T3, T8 |
| W12 Fresh-copy output location unspecified | T3 (DO mandate fresh copy), T5 (section 9 requires output-file convention), T13 (escape hatch for unresolved gap) |
| W13 No explicit anti-pattern list | T3 (consolidated DO/DO NOT block) |
| W14 Success criteria for enhanced output unstated | T10 (self-audit checklist defines implicit success criteria for the spec) |
| W15 Thin meta-task framing | T4, T12, T5 |

All 15 weaknesses are either directly enhanced or documented as open questions for the spec-writer — none silently resolved.
