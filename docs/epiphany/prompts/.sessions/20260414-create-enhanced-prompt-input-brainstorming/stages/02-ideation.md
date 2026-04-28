# 02-ideation.md — Enhancement Contracts for M3
# Scale: STANDARD

---

## Primary Contracts

- technique: T4
  target_section: "<role>"
  action: "Add a role element defining the AI as an expert prompt engineering systems architect with deep knowledge of modular skill design, agent orchestration patterns, and prompt optimization pipelines"
  rationale: "The brainstorming skill receiving this prompt needs a calibrated persona to reason at the right level of technical abstraction. Without role framing, the brainstorm may produce generic design advice rather than targeted skill architecture decisions."
  priority: high

---

- technique: T2
  target_section: "global"
  action: "Decompose the prompt into clearly labeled sections: (1) Background — what prompt-epiphany and epiphany-prompt are and why they exist; (2) Problem Statement — why epiphany-prompt is too slow; (3) Design Goal — what prompt-cog must achieve; (4) Constraints — non-negotiable requirements; (5) Brainstorm Task — what the brainstorming output must contain"
  rationale: "All three skills, the problem, and the task are currently merged in flowing bullet prose. Sectioning disambiguates context from constraint from task, which allows the brainstorming skill to address each dimension systematically."
  priority: high

---

- technique: T1
  target_section: "global"
  action: "Wrap each decomposed section in semantic XML elements: <role>, <context>, <task>, <constraints>, <output_format>"
  rationale: "XML semantic structuring gives the brainstorming skill unambiguous section boundaries and enables it to parse the input reliably without inference about where one section ends and another begins."
  priority: high

---

- technique: T11
  target_section: "<context>"
  action: "Add an anchor block defining all three skills precisely before any design discussion: (1) prompt-epiphany — what it is, how it runs, why it is fast but imprecise; (2) epiphany-prompt — what it is, how it differs (modular, programmatic, subagent-orchestrated), why it is slow; (3) prompt-cog — the target design space, not yet defined. Include verbatim preserved descriptions from input for prompt-epiphany and epiphany-prompt."
  rationale: "Weakness #2: the skill descriptions are too brief for a brainstorming agent to reason about what 'modular' or 'programmatic' means in practice. Anchoring these concepts early prevents the brainstorm from hallucinating system properties."
  priority: high

---

- technique: T3
  target_section: "<constraints>"
  action: "Convert all implicit and explicit constraints into DO/DO NOT format: DO — keep as many epiphany-prompt enhancements as possible; DO — make architecture modular with separable steps; DO — run faster than epiphany-prompt; DO NOT — replace or deprecate prompt-epiphany or epiphany-prompt; DO NOT — sacrifice reliability for speed ('run smoothly'); DO NOT — treat this as a refactor of prompt-epiphany (this is a new skill, prompt-cog, derived from prompt-epiphany's design)"
  rationale: "Weakness #5 (modify vs. derive ambiguity) and Weakness #7 ('run smoothly' vagueness) are directly addressed by making these constraints explicit and actionable. Converts prose assumptions into enforceable design rules."
  priority: high

---

- technique: T7
  target_section: "<constraints>"
  action: "Add a priority hierarchy for the quality-vs-speed trade-off: 'If quality preservation and run-time reduction conflict, prioritize run-time reduction — the goal is a fast path that is significantly better than prompt-epiphany, not an equal to epiphany-prompt. An enhancement from epiphany-prompt that cannot be ported without substantial overhead should be listed as a candidate for future inclusion, not included at launch.'"
  rationale: "Weakness #3 and Weakness #4: the trade-off is currently framed as 'best of both worlds' which gives the brainstorming skill no resolution strategy when specific enhancements conflict with speed. This contract adds a tiebreaker."
  priority: high

---

- technique: T6
  target_section: "<task>"
  action: "Add structured reasoning guidance inside the task element: 'For each epiphany-prompt enhancement or module, reason through: (a) What does this enhancement contribute to output quality? (b) What is the primary source of its overhead (agent spawn, multi-pass verification, expansion wave)? (c) Can the benefit be approximated in-context without spawning a separate agent? (d) What is the minimum viable form of this enhancement for prompt-cog? Organize output by enhancement/module, then synthesize a design recommendation.'"
  rationale: "Weakness #1 and Weakness #4: without CoT scaffolding, the brainstorming skill will produce surface-level comparisons rather than per-enhancement trade-off analysis. This is the core intellectual task and needs explicit step-by-step guidance."
  priority: high

---

- technique: T5
  target_section: "<output_format>"
  action: "Add an output format specification for the brainstorming result: 'Produce output in three sections: (1) Enhancement Inventory — for each epiphany-prompt enhancement, a row with: enhancement name, quality contribution (1–3 sentences), overhead source, portability verdict (port as-is / port simplified / defer / skip), and minimum viable form if portable; (2) Architecture Proposal — a concrete description of prompt-cog's module/step structure, noting which steps run in separate context and which inline; (3) Design Decisions — a list of explicit decisions made (e.g., \"No repair loops in prompt-cog\"), each with a one-sentence rationale.'"
  rationale: "Weakness #6: without an output format, the brainstorming skill will choose its own structure, likely producing a generic discursive essay rather than the structured design analysis needed to inform skill creation."
  priority: high

---

- technique: T12
  target_section: "<context>"
  action: "Add audience specification: 'The output of this brainstorm will be consumed by a skill architect who will use it to write the prompt-cog SKILL.md. The audience is technically fluent in JUCE-agent skill design, understands modular agent architecture, and will implement the skill in Claude Code. No background on the agent system is needed; depth on the design trade-offs is needed.'"
  rationale: "Weakness #6 (partially): calibrating for the specific downstream consumer (skill architect writing SKILL.md) focuses the brainstorm on actionable design decisions rather than general principles."
  priority: medium

---

- technique: T8
  target_section: "<constraints>"
  action: "Add edge case handling for enhancements that cannot be ported: 'If an epiphany-prompt enhancement requires a dedicated agent spawn to achieve acceptable quality and cannot be approximated inline, categorize it as DEFER and note the specific quality risk of omitting it. Do not propose a degraded version that would produce lower quality than prompt-epiphany — if the inline version would be worse than the baseline, mark SKIP with explanation.'"
  rationale: "Weakness #4 and Weakness #3: prevents the brainstorming skill from proposing compromised ports that produce worse output than the existing baseline. Adds a floor condition for portability decisions."
  priority: medium

---

- technique: T13
  target_section: "<task>"
  action: "Add escape hatch: 'If the design space for any enhancement is genuinely unclear (e.g., insufficient information about epiphany-prompt's internal structure to assess portability), state what specific information would be needed to resolve the uncertainty rather than guessing.'"
  rationale: "Weakness #8 (skill paths not provided): the brainstorming skill may not have access to the actual skill files. Rather than producing fabricated architecture claims, it should surface what it cannot determine."
  priority: low

---

- technique: "other:disambiguation-of-scope"
  target_section: "<task>"
  action: "Explicitly resolve the ambiguity between 'modify prompt-epiphany in-place' vs 'create a new skill prompt-cog derived from prompt-epiphany's design.' Add a single declarative sentence: 'prompt-cog is a NEW skill, separate from both prompt-epiphany and epiphany-prompt. It is inspired by prompt-epiphany's step structure and selectively imports enhancements from epiphany-prompt. prompt-epiphany and epiphany-prompt remain unchanged.'"
  rationale: "Weakness #5 is a structural ambiguity that could cause the brainstorming output to pursue a refactoring direction rather than a new-skill design direction. This contract eliminates that risk with a single explicit scope declaration."
  priority: high
