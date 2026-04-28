# create-enhanced-prompt-input-brainstorming — enhanced

```xml
<prompt>
  <meta source="epiphany-prompt"/>

  <role>
    You are an expert prompt engineering systems architect with deep knowledge of modular skill design, agent orchestration patterns, and prompt optimization pipelines. You reason at the level of architectural trade-offs — not generic design advice — and you produce structured, implementable design analysis.
  </role>

  <context>
    ## Skill Landscape

    Three skills are relevant to this design task. Their properties are anchored here as reference points for the brainstorming analysis.

    ### prompt-epiphany
    The original prompt enhancement skill. It runs quickly because it is a single-agent, single-context pass: the model reads the input prompt and applies a set of enhancement steps in sequence without spawning subagents or triggering multi-pass verification. However, because it is not run programmatically — it is invoked as a manual skill — some steps may not be followed exactly, and steps could be compartmentalized better, like running things with separate role context, like if using separate agents. It lives at `.claude/skills/prompt-epiphany skill`.

    ### epiphany-prompt
    A later skill based on prompt-epiphany, aimed to resolve the issue of running the procedure programmatically and creating a modular architecture. It is subagent-orchestrated: each stage (analysis, ideation, synthesis, verification, expansion) runs in a separate agent with its own context window and role. This enforces step discipline and enables parallel or staged execution. The trade-off is significant overhead: epiphany-prompt takes too long to run. The agent spawn cost, multi-pass verification loops, and expansion wave collectively produce a heavyweight execution profile.

    ### prompt-cog (target — not yet defined)
    The design target for this brainstorming session. We can optimize this process to achieve best of both worlds, creating a modular system from the prompt-epiphany skill, adding enhancements where possible, derived from epiphany-prompt but cutting run time down. prompt-cog is a NEW skill, separate from both prompt-epiphany and epiphany-prompt. It is inspired by prompt-epiphany's step structure and selectively imports enhancements from epiphany-prompt. prompt-epiphany and epiphany-prompt remain unchanged.

    ## Audience
    The output of this brainstorm will be consumed by a skill architect who will use it to write the prompt-cog SKILL.md. The audience is technically fluent in JUCE-agent skill design, understands modular agent architecture, and will implement the skill in Claude Code. No background on the agent system is needed; depth on the design trade-offs is needed.
  </context>

  <task>
    Design the prompt-cog skill architecture through per-enhancement trade-off analysis followed by a synthesized architecture proposal.

    ## Scope Declaration
    prompt-cog is a NEW skill, separate from both prompt-epiphany and epiphany-prompt. It is inspired by prompt-epiphany's step structure and selectively imports enhancements from epiphany-prompt. prompt-epiphany and epiphany-prompt remain unchanged. This is a new-skill creation task, not a refactor of prompt-epiphany.

    ## Structured Reasoning — Enhancement Inventory
    For each epiphany-prompt enhancement or module, reason through the following four questions in order:

    (a) What does this enhancement contribute to output quality?
    (b) What is the primary source of its overhead — agent spawn, multi-pass verification, or expansion wave?
    (c) Can the benefit be approximated in-context without spawning a separate agent?
    (d) What is the minimum viable form of this enhancement for prompt-cog?

    Organize output by enhancement/module, then synthesize a design recommendation.

    ## Escape Hatch
    If the design space for any enhancement is genuinely unclear — for example, because insufficient information about epiphany-prompt's internal structure is available to assess portability — state what specific information would be needed to resolve the uncertainty rather than guessing.
  </task>

  <constraints>
    ## Non-Negotiable Requirements

    DO — keep as many enhancements made in epiphany-prompt as possible.
    DO — make architecture modular with separable steps (compartmentalized, each step can run with separate role context).
    DO — run faster than epiphany-prompt.
    DO — run smoothly: "smoothly" means reliable, deterministic step execution with no dropped steps — not merely fast.
    DO — be more programmatic than prompt-epiphany (steps followed exactly, not loosely).

    DO NOT — replace or deprecate prompt-epiphany or epiphany-prompt.
    DO NOT — treat this as a refactor of prompt-epiphany; prompt-cog is a new skill derived from prompt-epiphany's design principles.
    DO NOT — sacrifice reliability for speed. It will take longer than prompt-epiphany to run; we can at least make prompt-epiphany modular and somewhat more programmatic without all the extra overhead.
    DO NOT — propose a degraded enhancement version that would produce lower quality output than prompt-epiphany. If an inline version would be worse than the baseline, mark it SKIP with explanation.

    ## Priority Hierarchy — Quality vs. Speed
    If quality preservation and run-time reduction conflict, prioritize run-time reduction. The goal is a fast path that is significantly better than prompt-epiphany, not an equal to epiphany-prompt. An enhancement from epiphany-prompt that cannot be ported without substantial overhead should be listed as a candidate for future inclusion (DEFER), not included at launch.

    ## Edge Case — Non-Portable Enhancements
    If an epiphany-prompt enhancement requires a dedicated agent spawn to achieve acceptable quality and cannot be approximated inline, categorize it as DEFER and note the specific quality risk of omitting it. Do not propose a compromised port that produces worse output than the prompt-epiphany baseline — mark it SKIP with explanation instead.
  </constraints>

  <output_format>
    Produce output in three sections:

    ### 1. Enhancement Inventory
    For each epiphany-prompt enhancement or module, provide a row containing:
    - Enhancement name
    - Quality contribution (1–3 sentences)
    - Overhead source (agent spawn / multi-pass verification / expansion wave / other)
    - Portability verdict: one of — PORT AS-IS / PORT SIMPLIFIED / DEFER / SKIP
    - Minimum viable form (if verdict is PORT AS-IS or PORT SIMPLIFIED)

    ### 2. Architecture Proposal
    A concrete description of prompt-cog's module/step structure. For each step, note:
    - Whether it runs in a separate context (agent) or inline within the main context
    - What role/persona context it uses
    - Its input and output

    ### 3. Design Decisions
    A list of explicit decisions made during the analysis (e.g., "No repair loops in prompt-cog"), each with a one-sentence rationale. These decisions should be directly usable as SKILL.md design principles.
  </output_format>

</prompt>
```

---
Generated by epiphany-prompt (STANDARD, normal mode) on 14-04-2026
Session: .sessions/20260414-create-enhanced-prompt-input-brainstorming/
