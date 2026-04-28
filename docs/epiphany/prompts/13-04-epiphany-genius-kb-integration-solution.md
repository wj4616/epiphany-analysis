<role>You are a cognitive systems architect specializing in AI agent skill design, knowledge engineering, and genius-mind cognitive theory integration. You have deep expertise in modular skill architectures, MCP-connected knowledge bases, and translating cognitive science research into programmable AI reasoning stages.</role>

<context>
We are redesigning the **epiphany-genius** skill — an AI agent skill for brainstorming and deep thought enhancement. The current proposed design lives at `docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md`.

This design descends from the original **epiphany-cognitive** skill, which was built with a subpar knowledge system — limited access to the full cognitive knowledge base, resulting in incomplete trait integration and missed enhancement opportunities.

We now have access to the **Dify cognitive knowledgebase via MCP** (`mcp__dify-cognitive__Cognitive_KB`), which contains the complete corpus of genius-mind cognitive theory: 19 primary traits (T1–T19) identified from analyzing genius individuals, methods for exploiting machine/AI advantage through cognitive theory, and the full conceptual framework.

The core transformation goal: take the original epiphany-cognitive concept and make it **modular and programmatic** while simultaneously leveraging the now-complete knowledge access to create a fundamentally better system than the original design could achieve.
</context>

<task>
Perform a comprehensive knowledge-driven redesign of the epiphany-genius skill by executing these phases in order:

**Phase 1 — Knowledge Audit**
Query the Dify cognitive knowledgebase via MCP to retrieve the complete inventory of:
- All 19 primary traits (T1–T19) with their full definitions, mechanisms, and interaction patterns
- All machine/AI advantage exploitation methods documented in the knowledge base
- All cognitive enhancement techniques and their implementation patterns
- Any concept hierarchies, trait interdependencies, or synergy maps

Catalog everything retrieved. This is the authoritative knowledge inventory for all subsequent phases.

**Phase 2 — Comparative Analysis**
Read the current proposed design at `docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md` and the original epiphany-cognitive skill design. For each element, answer:
- Which traits from the 19 primary traits does this element activate?
- Which traits are NOT represented anywhere in the current design?
- Where does the current design use incomplete or shallow knowledge that the full KB can deepen?
- What cognitive mechanisms from the KB are entirely absent from the design?

Produce a gap matrix: rows = all 19 traits + all KB concepts, columns = [present in design | partially present | absent | could be strengthened].

**Phase 3 — Enhancement Discovery**
Using the gap matrix and full KB access, identify concrete enhancements:
- **Missing traits**: For each absent or partially-present trait, design how it integrates into the skill architecture
- **New stages**: Propose additional processing stages that the full knowledge base justifies (stages the original couldn't include due to incomplete knowledge)
- **Deepened existing stages**: Where existing stages use shallow trait activation, specify how full KB knowledge deepens them
- **New features**: Identify capabilities the full KB enables that weren't possible before — new output types, new reasoning modes, new trait combinations
- **Synergies**: Identify trait interactions or concept combinations from the KB that produce emergent value when combined in the skill pipeline
- **AI-advantage methods**: For each machine/AI exploitation method in the KB, determine if and where it should be wired into the skill

Every enhancement must trace to a specific KB entry or concept. No speculative additions.

**Phase 4 — Modular Architecture Redesign**
Restructure the skill design to be modular and programmatic:
- Each cognitive trait or trait-cluster becomes an activatable module
- Stages are composable — users or the system can select which cognitive lenses to apply
- The trait activation pipeline is configurable, not hardcoded
- KB queries are integrated at specific, defined points in the pipeline (not ad-hoc)
- The architecture supports adding new traits or stages without restructuring existing ones

Preserve the core identity of the brainstorming and deep thought agent skill while making it architecturally superior.

**Phase 5 — Integration Synthesis**
Produce the final enhanced skill design that:
- Integrates every enhancement from Phase 3 into the modular architecture from Phase 4
- Ensures all 19 primary traits are optimally represented — none underutilized, none redundant
- Includes explicit KB query points where the skill pulls knowledge during execution
- Documents how each stage maps to cognitive theory concepts
- Specifies the complete processing pipeline from input to output
</task>

<constraints>
**DO:**
- Query the Dify cognitive KB (`mcp__dify-cognitive__Cognitive_KB`) for every knowledge claim — do not rely on assumptions about what the KB contains
- Preserve every element of the current design at `docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md` that the KB review confirms as sound
- Trace every design decision to a specific KB concept or trait
- Account for all 19 primary traits — if any trait cannot be meaningfully integrated, explicitly state why
- Keep the architecture modular — each component independently testable and replaceable

**DO NOT:**
- Invent cognitive concepts not found in the KB — if the KB does not support an enhancement, do not include it
- Remove existing design elements without KB-backed justification for why they are inferior
- Collapse distinct traits into a single module unless the KB explicitly documents them as a unified mechanism
- Design monolithic stages that bundle multiple unrelated cognitive processes
- Assume the original epiphany-cognitive skill design was wrong — it was knowledge-limited, not conceptually flawed
</constraints>

<output_format>
Deliver a single document structured as:

1. **Knowledge Inventory** — Complete catalog of all KB concepts retrieved, organized by category
2. **Gap Matrix** — Visual matrix showing trait/concept coverage in current design vs. what KB enables
3. **Enhancement Register** — Each enhancement with: source KB concept, what it improves, how it integrates, expected impact
4. **Architecture Specification** — Modular skill architecture with: module definitions, pipeline flow, KB query integration points, configurability model
5. **Final Skill Design** — The complete, production-ready epiphany-genius skill design incorporating all enhancements
6. **Diff Summary** — Concise list of what changed from the original proposed design and why (traced to KB)
</output_format>

<verification>
Before finalizing, verify:
- [ ] All 19 primary traits appear in the design — map each trait to its module(s)
- [ ] Every enhancement traces to a specific KB entry — no unsourced additions
- [ ] The gap matrix shows zero unaddressed absences (every gap either filled or explicitly justified as not applicable)
- [ ] Architecture is modular — each module can be described independently without referencing others' internals
- [ ] KB query points are explicit — document which MCP call retrieves what knowledge at which pipeline stage
- [ ] The design at `docs/epiphany/prompts/13-04-epiphany-genius-skill-design.md` was fully read and compared
- [ ] No cognitive concept from the KB was overlooked — cross-reference the Knowledge Inventory against the final design

If any check fails, revise the affected section before delivering. If a check cannot be satisfied, state what is missing and why.
</verification>
