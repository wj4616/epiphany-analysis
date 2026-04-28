<prompt><meta source="prompt-graph"/><role>You are a Graph-of-Thought architecture analyst with deep expertise in multi-agent reasoning topologies, prompt-engineering pipelines, and the Claude Code skill + agent ecosystem. You evaluate topologies against the formal GoT model (Besta et al.) and against the practical capability surface of modern AI agent systems. Tone register spans "fully exploiting and harnessing", "fully integrating exploiting and using", "fully harness", and "fully exploit/harness" — these intensifiers are operationalized in the constraints, not treated as rhetoric.</role><context>

**Target artifact:** the prompt-graph skill located at `/home/myuser/.claude/skills/prompt-graph` — specifically its graph of thought pipeline of our prompt-graph skill prompt enhancement system. The skill is a graph-of-thought (GoT) prompt-enhancement pipeline. It has been audited and is currently working; this engagement is additive improvement, not remediation.

**Status framing (verbatim from input):** "the prompt-graph skill has been audited", "The skill is working", and the directive "lets improve it".

**Current architecture anchors (v1):**
- Spawn discipline: 1-spawn baseline, ≤2 spawns when repair invoked.
- Existing aggregation primitives: N11 (contract merge) + N17 (verifier aggregation) + N13 (INVENTORY fan-in).
- Existing backtracking: edge E19, capped at one attempt by orchestrator rule O6.
- Roadmap: v1.1+ names `--strict-verify`; v2 names `--spec` / `--plan` and adds nodes N21–N26.

**Topology baseline to evaluate against:** the existing 20-node / 35-edge GoT topology, including parallel groups PG1–PG4.

**Claude Code capability surface (each item to be classified as {used | deliberately excluded with rationale | unexploited}):**
- Skills system
- Agent types: general-purpose, Explore, Plan, prompt-architect, juce-agent, brainstorm-architect, claude-code-guide
- Agent tool with `subagent_type`
- MCP servers: dify-cognitive-kb, dify-thought-kb
- Tool gating per HG3
- Parallel agent dispatch
- Background agents
- Scheduling primitives: CronCreate, ScheduleWakeup, /loop
- PlanMode
- Worktree isolation (`isolation:worktree`)
- Memory system
- Hooks
- Keybindings
- ToolSearch (dynamic tool loading)
- Monitor (long-running synthesis observation)
- SendMessage (resume a synthesis agent rather than respawning)

**Theoretical anchor:** Besta et al., "Graph of Thoughts: Solving Elaborate Problems with Large Language Models" (2023). Key claims to evaluate prompt-graph against:
- CoT: latency N, volume N (simple sequential reasoning).
- ToT: latency O(log_k N), volume O(log_k N) (branching, independent thoughts).
- GoT: latency O(log_k N), volume N (aggregation + refinement loops + arbitrary transformations) — optimal latency-volume tradeoff.
- Aggregation is the GoT-distinguishing operation; refinement loops are realized via back-edges.

**Auxiliary technique anchors (apply where relevant in recommendations):**
- Self-Refine: generate → self-feedback → revise loop; ~20% absolute improvement; 1–2 iterations sufficient.
- Intuition-Verification Partnership (agent separation): generation specializes in conjecture, verification specializes in proof.
- TRIZ: identify contradiction, apply resolving principle.
- Structured Output Prompting: (1) define schema, (2) provide example, (3) state strict formatting rules, (4) include self-validation instruction; temperature 0.0–0.1 for format-critical outputs.

**Architectural constraints inherited from prompt-graph (must not be silently violated):**
- ≤2-spawn discipline.
- Wave-modular attention-reset semantics.
- Standalone-no-MCP-runtime guarantee.

</context><task>

**Embedded directives (verbatim, all to be honored as scope):**
- "orchestrate system which will provide analysis and improvement of graph and nodes architecture"
- "ensure we are fully exploiting and harnessing all possibilities of the system"
- "need multi-dimensional analysis and gap scan of graph of thought in prompt-graph skill"
- "lets improve it and fully exploit/harness all systems in modern machine computing artificial intelligence agent system"

Additional input phrasings to be preserved as scope-defining: "graph of thought pipeline of our prompt-graph skill prompt enhancement system", "aggregation and backtracking of nodes", "modern ai systems which have reasoning ability", "claude code ai agent skill system", "modern machine computing artificial intelligence agent system".

**Primary task:** Produce a multi-dimensional analysis and gap scan of the graph-of-thought pipeline inside the prompt-graph skill, then propose concrete, prioritized architectural improvements covering nodes, edges, aggregation, backtracking, and integration with the Claude Code AI agent skill system — ensuring we are fully exploiting and harnessing all possibilities of the system including aggregation and backtracking of nodes fully integrating exploiting and using all capabilities offerered in modern ai systems which have reasoning ability such as claude code ai agent skill system.

**Decomposition — address each dimension D1–D10 under its own explicit heading:**
- **D1 Topology completeness** — nodes, edges, parallel groups, conditional edges, cardinalities; benchmark against the 20-node / 35-edge baseline and against the formal GoT model.
- **D2 Aggregation patterns** — N11 contract merge, N17 verifier aggregation, INVENTORY fan-in (N13); identify any node with >1 input edge lacking a documented aggregation policy.
- **D3 Backtracking & repair** — E19 single-attempt cap (rule O6), fallback retention, recovery semantics; identify verification/routing decisions lacking a documented back-edge target or an explicit rationale for its absence.
- **D4 Parallelism** — PG1–PG4 logical-vs-literal Agent-tool concurrency; cost/benefit of literal parallel dispatch vs wave-modular attention-reset semantics.
- **D5 Spawn budget exploitation** — current ≤2-spawn discipline vs alternatives; quantify what is foreclosed by the ceiling and what would change at 3+.
- **D6 Claude Code integration surface** — for each capability listed in `<context>` (skills, agent types, MCP, hooks, parallel dispatch, plan mode, memory, scheduling, tool whitelisting, plus ToolSearch / Monitor / SendMessage / worktree), classify {used | deliberately excluded with rationale | unexploited}.
- **D7 Knowledge-base integration** — embedded vs runtime KB; hybrid v2+ pathway; dify-cognitive-kb and dify-thought-kb posture.
- **D8 Verification topology** — orchestrator-inline vs agent-separated verification; evaluate Intuition-Verification Partnership applicability.
- **D9 Hard-gate enforcement** — HG1 / HG2 / HG3 mechanical strength; observed failure modes; tool-call whitelist coverage.
- **D10 Mode coverage** — minimal / normal / verbose feature parity; identify intermediate-mode opportunity.

**Anti-conformity passes (execute BEFORE finalizing recommendations):**

- **AC01 — Adversarial self-audit / steelman pass.** For each "obvious" gap, first ask "what would be lost if this gap were closed?" Worked examples that MUST be considered: agent-separated verification gains rigor but loses speed; runtime KB gains freshness but loses determinism; literal parallel agent dispatch gains throughput but breaks wave-modular attention-reset semantics. Steelman first, then critique.

- **AC02 — Capability-overhang scan.** Identify Claude Code capabilities that are (a) extant, (b) unused by prompt-graph, AND (c) compatible with no architectural conflict. Distinguish "unexploited because incompatible" vs "unexploited because untried". Investigate at minimum: ToolSearch (dynamic loading), Monitor (long-running synthesis), `isolation:worktree` (refactor-under-test), ScheduleWakeup (delayed re-verification), CronCreate (periodic smoke tests), SendMessage (resume synthesis agent rather than respawn).

</task><constraints>

**Verbatim input constraints (binding):**
- "the prompt-graph skill has been audited"
- "The skill is working"
- "fully exploiting and harnessing all possibilities of the system including aggregation and backtracking of nodes"
- "fully integrating exploiting and using all capabilities offerered in modern ai systems which have reasoning ability"
- "such as claude code ai agent skill system"
- "lets improve it"

**Operational definitions of "fully" (intensifiers from input made measurable):**
- *fully-exploit-aggregation* = every node with >1 input edge has a documented aggregation policy.
- *fully-exploit-backtracking* = every verification/routing decision has a documented back-edge target OR an explicit rationale for absence.
- *fully-harness-modern-AI* = each Claude Code capability enumerated in `<context>` is classified as {used | deliberately excluded with rationale | unexploited}.

**Architectural compatibility (recommendations must respect or explicitly flag relaxation):**
- ≤2-spawn discipline.
- Wave-modular attention-reset semantics.
- Standalone-no-MCP-runtime guarantee.

**Meta-recursive scope guard (AC03, HIGH):** the analysis target IS prompt-graph itself. Therefore:
- Do NOT use prompt-graph on this prompt to produce the analysis.
- Do NOT propose recommendations that would break in-flight pipeline runs without explicitly flagging runnability impact.
- Deliver the analysis as plain markdown produced by a single agent, without invoking prompt-graph.
- For every recommendation, consider runnability impact on in-flight runs.

</constraints><output_format>

Produce a single plain-markdown document with the following fixed sections, in order:

1. **Executive Summary** — 3–5 sentences.
2. **Per-Dimension Gap Scan** — one subsection per dimension D1 through D10, each using this fixed sub-template (for diff-ability):
   - **Current State:**
   - **Gap Found:**
   - **Severity:** (low | medium | high)
   - **Recommendation:**
   - **Estimated Effort:** (S | M | L)
3. **Cross-Dimension Findings** — patterns spanning multiple dimensions.
4. **Prioritized Improvement Backlog** — ordered by effort vs impact.
5. **Architecture-Constraint Compatibility Check** — explicit pass/flag for each of: ≤2-spawn discipline, wave-modular attention-reset, standalone-no-MCP-runtime.
6. **Open Questions / User-Decision Items.**

Schema rules: every D1–D10 subsection MUST include all five sub-template fields. For format-critical sections, treat the sub-template as strict.

</output_format><verification>

Before emitting the final document, run verification phase V1–V4:

- **V1 — Coverage:** every D1–D10 has Current State + Gap Found + Recommendation populated.
- **V2 — Actionability:** every recommendation carries Estimated Effort AND a Compatibility note.
- **V3 — Consistency:** no recommendation silently violates ≤2-spawn discipline, wave-modular attention-reset, or standalone-no-MCP-runtime; if a recommendation does relax one of these, it MUST be explicitly flagged as a budget-relaxation proposal.
- **V4 — Anchoring:** at least 60% of recommendations cite a specific node, edge, wave, or module identifier (e.g., N11, N13, N17, E19, O6, PG1–PG4, N21–N26, HG1–HG3).

On FAIL: revise once. If still failing after one revision, emit the output annotated with the failing checks rather than silently truncating.

The deliverable must satisfy the input verification criterion "multi-dimensional analysis and gap scan."

</verification><edge_cases>

- **No-gap-found case:** if a dimension has no gap, state explicitly: "No gap identified; current implementation appears optimal because [reasoning]." Do not invent a gap.
- **Already-on-roadmap case:** if a recommendation is already scheduled for v1.1 (`--strict-verify`) or v2 (`--spec` / `--plan`, N21–N26), flag it as "Already on roadmap (v1.1 / v2)" rather than presenting as novel.
- **Insufficient-information case:** state what information is missing and what assumption was used to proceed.
- **Length / compoundness escape hatch:** if the analysis exceeds ~3000 words or becomes too compound, either (a) propose splitting into per-dimension sub-analyses, or (b) deliver a tier-1 cut plus an appendix listing deferred dimensions. Never silently truncate.
- **Meta-recursion edge case:** if a proposed improvement would alter the very pipeline being analyzed in a way that prevents in-flight execution, flag it under Architecture-Constraint Compatibility Check rather than burying it inside a recommendation.

</edge_cases></prompt>
