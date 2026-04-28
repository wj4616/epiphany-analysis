<prompt>

<role>You are an AI system design architect specializing in decision science
methodologies, computational optimization, and knowledge base analysis.</role>

<context>
A Pugh Matrix — also known as a decision matrix or Pugh chart — is a systematic
decision-making tool that helps teams evaluate multiple alternatives against
predefined criteria. It provides a structured approach for comparing concepts,
solutions, or options across engineering design, product development, and business
decision-making processes.

The goal of this work is to build an AI agent skill (Claude Code, markdown format)
that does not merely digitize the Pugh Matrix methodology but fundamentally
reimagines it to exploit every computational, machine, and AI advantage available.

Knowledge base path containing prior research on Pugh Matrix methodology:
<path>~/clawd/research/pugh-matrix/</path>
</context>

<tasks>

<task id="1" label="KB Analysis — Prerequisite for Tasks 2 and 3">
Review the knowledge base at <path>~/clawd/research/pugh-matrix/</path> in full.

Phase A — Per-file analysis:
For each `.md` file, produce:
  - Core thesis or contribution of the file
  - Key concepts, algorithms, or frameworks introduced
  - Gaps, limitations, or open questions identified

Phase B — Cross-file synthesis:
Analyze every combination of files together:
  - Thematic clusters: which files reinforce, extend, or contradict each other
  - Capability map: what the combined KB reveals about the full scope of Pugh
    methodology
  - Machine-advantage gaps: where the existing KB is silent on computational
    optimization — these are the highest-value targets for external research

Output: A structured KB insight report with three sections:
  (a) Per-file summaries
  (b) Cross-file synthesis table
  (c) Ranked list of machine-advantage research gaps

This output is a prerequisite for both Task 2 and Task 3.
</task>

<task id="2" label="Deep-Research-Pro Prompt — Informed by KB Gap Analysis">
Using the machine-advantage gap list produced in Task 1, provide a prompt
optimized for deep-research-pro that will generate an optimal, professional,
highly detailed, full multi-dimensional research report.

The report must expose the best methods for using machine, computer, and AI
advantage when optimizing and enhancing Pugh Matrix systems and algorithms —
enabling human users to achieve optimal solutions in product design and decision
making.

The deep-research-pro prompt must be structured with these explicit sections:
  - Context block: what Pugh Matrix is; what AI-naive implementations look like today
  - Research directive: the specific machine-advantage gaps to investigate
    (drawn directly from Task 1 output, not generic)
  - Output format specification: dimensions, depth, and structure expected in
    the final report
  - Quality constraints: no surface-level coverage; each dimension must include
    mechanisms, evidence, and implementation guidance
</task>

<task id="3" label="Specification — AI Agent Skill for Pugh Matrix (via brainstorming Skill)">
Using the brainstorming skill, develop a specification for a plan to create an
AI agent skill in markdown format for Claude Code.

The skill must:
  - Fully implement the entire Pugh Matrix methodology
  - Adapt, enhance, and exploit machine advantage, computer advantage, and the
    advantages offered by artificial intelligence technologies in 2026
  - Include all useful elements of Pugh methodology
  - Be grounded in the full KB analysis from Task 1

Scope of machine and AI optimization the specification must address:
  - Automated criteria weighting via preference elicitation or ML-derived scores
  - Algorithmic alternative generation beyond human brainstorming limits
  - Multi-model consistency checking across scorer perspectives
  - Dynamic re-evaluation as new information arrives mid-process
  - Uncertainty quantification on scores (distributions, not point estimates)
  - Natural language interface for non-expert users

The specification must cover:
  - Skill purpose and trigger conditions
  - Input / output contract
  - Step-by-step pipeline with AI-augmented steps explicitly labeled
  - Quality gates and verification checkpoints
  - Integration points with other Claude Code skills (e.g., brainstorming skill,
    deep-research-pro)
  - Example invocation and expected output format
</task>

</tasks>

<execution_order>
Complete tasks in this sequence:
  1. Task 1 (KB analysis) — required before Tasks 2 and 3 can be executed
  2. Task 2 (deep-research-pro prompt) — uses gap list from Task 1
  3. Task 3 (skill specification via brainstorming skill) — uses KB insights
     from Task 1
</execution_order>

</prompt>
