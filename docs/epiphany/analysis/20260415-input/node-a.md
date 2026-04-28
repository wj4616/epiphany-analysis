```xml
<role>
You are an expert VST plugin development strategist and playbook architect with deep knowledge of: JUCE framework audio plugin development in C++, real-time DSP algorithm design and optimization, sound synthesis theory (subtractive, additive, granular, spectral, FM, wavetable, physical modeling), psychedelic and ambient sound design aesthetics, modern audio plugin UI/UX standards, VST marketplace business strategy and pricing, software licensing and anti-piracy systems for audio software, and AI-assisted software development workflows using Claude Code and agent-based systems.
</role>

<audience>
This playbook will be consumed by two readers:
1. **Claude Code AI agents** — executing development phases, referencing KB content, generating code and specifications
2. **A solo developer** — C++ beginner (experienced in Python/JavaScript), basic sound design knowledge, no DSP algorithm expertise. Must understand every phase well enough to review AI output and make creative decisions.

Write for both: technically precise enough for AI execution, clearly explained enough for developer comprehension and oversight.
</audience>

<context>
<market_opportunity>
The VST audio plugin marketplace has a creativity gap. Existing plugins cluster around conventional designs — they are either expensive flagship products from established studios or budget options that lack creative ambition. This creates an underserved segment: consumers who want creative, sonically adventurous plugins at accessible price points.

AI-assisted development changes the competitive landscape. Complex DSP algorithms and creative effects that previously required teams of experienced C++ audio engineers are now achievable by a solo developer using AI tools. This makes creative, complex, abstract audio effects commercially viable for small operations — effects that would not have been economically feasible to develop before AI agent tool use.
</market_opportunity>

<developer_profile>
- **Team size:** Solo developer — one person handles all development, business, and marketing, using AI and AI agents to develop both the product and the entire business
- **AI tools:** Claude Code (currently $100/month plan, may expand to $200/month maximum) with a playbook knowledge base (KB) system that enhances AI output quality and consistency
- **Development approach:** Multiple Claude Code sessions per project; total effort must remain reasonable for one person to complete the coding aspect
- **Programming skills:** Proficient in Python and JavaScript; familiar with basic C++ theory but new to C++ development in practice — first VST plugin project
- **Audio skills:** Experienced VST user with basic sound design knowledge; no DSP algorithm knowledge (AI fills this critical gap — AI is strong at DSP implementation)
- **Agent skills available:**
  - `brainstorming` — for generating specifications
  - `writing-plans` — for creating implementation plans
  - `execute-plans` or `subagent-driven-development` — for executing plans
</developer_profile>

<technical_stack>
Core:
- C++17
- CMake 3.22+
- GCC 7+ or Clang 6+
- JUCE 8.0.x
- VST3 SDK 3.8.x
- AAX SDK

Linux development dependencies (including but not limited to):
- libx11-xcb-dev (X11 integration)
- libxcb-util-dev (XCB utilities)
- libxcb-cursor-dev (Cursor support)
- libxcb-xkb-dev (Keyboard handling)
- libxkbcommon-dev (Keyboard common)
- libxkbcommon-x11-dev (X11 keyboard)
- libfontconfig1-dev (Font rendering)
- libcairo2-dev (Graphics)
- libgtkmm-3.0-dev (GTK integration)
- libsqlite3-dev (SQLite)
- libxcb-keysyms1-dev (Key symbols)
</technical_stack>
</context>

<task>
Generate the Phase 0 commission brief section for a comprehensive VST audio plugin development playbook. This brief defines the complete system for creating commercial VST audio plugins — spanning development, sound design, marketing, licensing, and business strategy — optimized for a solo AI-assisted developer.

The playbook must address these seven domains:

**1. VST Plugin Development (Primary Focus)**
JUCE framework plugin development in C++, CMake build system, DSP algorithm implementation. Research and reference the highest-quality, best-sounding DSP algorithms and C++ audio algorithms suitable for JUCE. Focus on producing the best-sounding VST synthesizers and effects possible. Must support creating any type of audio plugin, with special emphasis on creative, complex, abstract effects that are newly achievable through AI agent tool use.

**2. Sound Design Knowledge Base**
Advanced synthesis techniques, synthesizer patch creation theory, synth module sound design. Core creative focus areas to seed and generate research topics from (not exclusive — these are starting points, not boundaries):
- Psychedelic sound design
- Ambient sound design
- Pad sound design
- Creative/abstract sound design
- Psybient sound design

The KB must also cover general sound design theory sufficient to create all sound types well. Psybient is one creative focus, not the only one. Reference the best possible quality sources: academic papers, professional standards, standard reference material, instructional guides from recognized experts, professionals, and academics.

**3. UI/UX Design for Audio Plugins**
Modern audio plugin interface design standards, synthesizer control layout theory, clean modern audio tool interface design. Research expert information on how to design clean modern audio tool interfaces. Get current best practices for how professional audio plugins present controls, visualizations, and workflows.

**4. Marketing and Sales Strategy**
Strategies relevant to the modern marketplace and state of VST plugin sales in 2026, from the perspective of an enthusiast developer wishing to develop VST audio plugins using JUCE and introduce them to the market commercially. Cover all aspects of marketing VST plugins including marketplace positioning, pricing strategy that keeps plugins in the budget of the average consumer, and modern promotional approaches.

**5. Licensing and Anti-Piracy**
Commercial licensing systems available for commercial VST plugins. How VST developers sell licenses for their products and list them on audio plugin marketplaces for sale. Anti-piracy countermeasures and systems most effective for KVR Audio and other marketplaces. Ensure software cannot be easily pirated.

**6. Knowledge Base Architecture**
Design a complex, multi-layered KB system with bridge systems connecting domains to each other. Why bridges matter: the developer needs to move from creative intent ("I want a warm pad") to DSP implementation without manual translation at each boundary. Design bridges that cover the developer's end-to-end translation journey: **creative intent → sound design concept → DSP parameter → C++ implementation → UI control → marketing descriptor**. Each arrow in this chain is a required bridge. Additional bridges between non-adjacent domains are also needed: UI design principles ↔ audio-specific UX standards, marketing positioning ↔ pricing strategy. Build the KB in two tiers: Tier 1 (core KB — all named domain layers, all bridge systems with full specifications, and baseline content across all domains) must be complete before any development phase begins; Tier 2 (supplemental KB — gap content discovered mid-project) is harvestable on demand during development and does not block phase progression. The KB must be ready to accept and organize a broad range of information across all covered topics: sound design, JUCE/VST development, UI design, C++, marketing, licensing, and related subjects.

**7. C++ Reference (Scoped)**
Include C++ documentation relevant to audio plugin development — real-time audio constraints, memory management for audio threads, template patterns for DSP, and similar patterns directly useful for JUCE plugin coding. Do NOT create a general C++ reference. Include only what is needed; do not overload.
</task>

<constraints>
**Budget and Resources:**
- DO use freely available libraries unless the planning phase specifically determines a paid alternative is necessary and justified
- DO NOT require significant capital investment beyond the AI subscription ($100–$200/month for Claude Code is the only planned cost)
- DO scope every phase for completion by a single developer using AI assistance — no phase should assume a team
- DO design phases to produce shippable milestones — define "shippable" as **technically functional** (VST3 binary loads in DAW, produces correct audio output) for all intermediate phases; a **commercially viable** milestone (marketplace-distributable, with full bridge chain coverage, licensing system in place) is only achievable at final release. These two definitions must be explicitly distinguished: a technically functional plugin that lacks a marketing bridge or licensing system is not commercially shippable

**Technical:**
- DO target the specified technical stack exactly (C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK). NOTE: **AAX SDK applies to the final commercial release target only** — all phase milestones prior to release use VST3 functional builds exclusively; AAX compilation and certification are gated to the release phase
- DO include all specified Linux development dependencies

**Quality Priority (when trade-offs arise, resolve in this order):**
1. DSP audio quality and best-sounding output — highest priority
2. Creative sound design capability and breadth
3. Marketing strategy and commercial viability
4. Licensing and anti-piracy protection
5. UI polish and modern design
6. C++ reference completeness — lowest priority (keep scoped)

**Knowledge Base:**
- DO build the KB system in two tiers: **Tier 1** (core KB — required before any development phase begins) covers all named domain layers, all named bridge systems with full six-component specifications, and baseline content for sound design, JUCE/VST, UI/UX, marketing, and licensing. **Tier 2** (supplemental KB — harvestable on demand during development) covers gap content discovered mid-project and does not block phase progression
- DO NOT overload the C++ reference section — scope to JUCE plugin development only
- DO cover sound design broadly (all types) while using the specified creative focuses as research seed topics to generate new topics from
- DO include bridge systems connecting every relevant domain pair

**Agent Workflow:**
- DO map agent skills to phases: `brainstorming` for specifications, `writing-plans` for implementation plans, `execute-plans` or `subagent-driven-development` for execution
- DO define clear boundaries between AI-generated work and human creative decisions at each phase
- **Agent workflow sequencing:** Gate `brainstorming` and `writing-plans` after Tier 1 KB completion. Plans written before Tier 1 KB is populated will embed assumptions about synthesis techniques and DSP parameters that KB content may contradict. If planning must precede KB completion, include a mandatory "Tier 1 assumption audit" revision pass after KB population — audit every plan for embedded DSP or synthesis assumptions and correct them before development phases begin
- **DSP decision opacity:** When a phase requires choosing between competing DSP algorithms and the trade-offs require domain expertise to evaluate, the developer cannot make a substantive technical choice. Resolve this boundary: AI presents options using **perceptual descriptors** ("Option A: dense harmonic texture with slower transients; Option B: sharper attack, more metallic character"), and the developer selects by DAW listening test — never by evaluating technical parameters the developer does not understand

**Bridge and Vocabulary:**
- DO require creative intent phrases to be preserved **verbatim** throughout the KB bridge chain — the original phrase is the cache key; AI agents must not paraphrase it at any bridge node, as paraphrasing corrupts the key and forces re-derivation from scratch
- DO require each bridge entry's coverage criterion to include both a **structural completeness test** (the entry conforms to the required form) and a **semantic/perceptual validation test** (a developer listening to the DSP output confirms it perceptually matches the original creative phrase). Structural completeness alone is insufficient: a well-formed entry with wrong parameter values will not be caught until DAW testing
- DO specify the bridge chain as **forward-direction only**: creative intent → sound design → DSP parameter → C++ implementation → UI control → marketing descriptor. Reverse traversal (market opportunity → creative intent) is outside the system's scope; acknowledge this boundary explicitly

**Scope Exclusions:**
- DO NOT apply the creativity-gap market opportunity framing to plugin types where UI quality is inseparable from core product value (spectrum analyzers, oscilloscopes, metering and visual tools) — the DSP > UI quality priority ordering fails for this class; exclude it explicitly in the System Vision section
- DO NOT apply the creativity-gap market opportunity framing to conventional saturated segments (reverb, compressor, equalizer) — the uniqueness thesis does not hold where the market is already well-served; include this as an explicit scope boundary

</constraints>

<edge_cases>
- **Library cost:** If a required library is not freely available, evaluate build-vs-buy cost and present the trade-off — default to building in-house or finding a free alternative
- **KB gaps discovered during development:** KB harvesting operates in two tiers (see Constraints). When a gap is found mid-build, classify it as: Tier 1 omission (blocking — must fill before the current phase proceeds) or Tier 2 addition (non-blocking — fill and continue). Gap-filling must not require restructuring existing layers or bridge systems
- **Budget limit hit mid-project:** Each phase should produce a functional milestone; if the developer must pause across sessions, they have a working intermediate product
- **DSP complexity exceeds developer's C++ ability:** Specify how the AI agent handles full implementation while the developer handles review, testing, and creative direction
- **Marketplace conditions change:** Marketing strategy should include durable principles alongside 2026-specific tactics, so it remains useful if platforms shift
- **Bridge entry produces incorrect DSP output:** When a KB bridge entry causes DSP output that does not perceptually match the original creative phrase (detectable during DAW testing), it is a bridge entry defect, not a code defect. Correction path: (a) identify the bridge node where the semantic error was introduced, (b) update only that bridge entry with correct parameters, (c) re-run downstream steps from that bridge node forward only — no full-phase restart required. The bridge system must be structured to support point corrections without cascading restructuring
- **Creative intent outside established synthesis vocabulary:** When a developer provides a creative intent phrase for which no existing synthesis or DSP vocabulary mapping exists (genuinely novel concept with no established vocabulary mapping), the bridge chain cannot produce a valid lookup. In this case: (a) do not force-map the phrase to an incorrect synthesis category, (b) define a new bridge vocabulary entry by working with explicit sound design references (academic papers, expert instructional content) before traversal, (c) treat this as a Tier 2 KB gap that blocks bridge traversal for that specific phrase until filled. This is a hard boundary of the bridge system: it handles established vocabulary, not novel concepts
</edge_cases>

<output_format>
Produce a Phase 0 commission brief structured with these sections:

1. **System Vision** — Problem statement, market opportunity, value proposition, why AI-assisted solo development is viable for this market. Establish that Phase 0 is the **base-case computation** for the entire system: the semantic quality of creative intent captured here is a multiplier on all downstream phase quality — a structural checklist (16/16 items checked) is a floor, not a quality signal; poor creative specificity at Phase 0 causes cache misses at every downstream phase. Include the two explicit scope exclusions for the creativity-gap opportunity: (a) visual-UI-as-product plugins are outside scope, (b) conventional saturated segments (reverb, EQ, compression) are outside the creativity-gap framing
2. **Developer Profile and Constraints** — Skills inventory, tool budget, time budget, skill gaps and how AI fills each gap
3. **Technical Architecture** — Full stack specification, dependencies, build system, target plugin formats, platform targets
4. **Knowledge Base Design** — Layer structure with named layers, bridge systems with explicit domain pairs and rationale, topic taxonomy per domain, data ingestion and quality strategy. For each bridge system, specify all six components: (a) direction (A→B only, or bidirectional) — the primary bridge chain is **forward-only** (creative intent → artifact); state this explicitly and document the boundary: reverse traversal (market opportunity → creative intent) is outside the system's scope and will not be served by these bridges, (b) source domain vocabulary (key terms as they appear in the source domain) — these terms must be treated as **verbatim cache keys**; AI agents must not paraphrase them at any bridge node, (c) target domain vocabulary (the mapped equivalents in the target domain), (d) transformation rule or mapping method (how the translation is performed — lookup table, formula, decision procedure, or structured instruction), (e) one concrete example input/output pair, (f) coverage criterion (what qualifies as fully bridged vs. requiring a new entry) — must include **both** a structural completeness test (entry conforms to required form) **and** a semantic/perceptual validation test (a developer listening to the resulting DSP output confirms it perceptually matches the source phrase). A structurally complete entry with incorrect parameter values will pass a structural-only gate and produce wrong DSP output — the bridge entry, not the code, is the site of correction
5. **Development Phases** — All relevant phases from specification through release, with: phase objectives, required agent skills, AI vs. human responsibilities, phase-specific strategies, and exit criteria. Derive phase structure from the developer profile and technical constraints defined above. Each phase must produce a shippable milestone: for KB-construction phases, gate-ready and operational; for code phases, a technically functional VST3 binary that loads in DAW and produces correct audio output. **Commercial viability is a property of the final release phase only** — do not conflate technical milestone shippability with commercial viability. Map the KB tiering model (Tier 1 / Tier 2) to specific phase gates — state explicitly at which phase Tier 1 must be complete before development proceeds, and explicitly gate `brainstorming` and `writing-plans` agent skills after Tier 1 KB completion (or mandate a Tier 1 assumption audit revision pass). Specify AAX compilation and certification as a **final release phase milestone only** — all earlier phases target VST3 functional builds. For DSP phases where competing algorithm options require domain expertise to evaluate, specify the AI/human boundary: AI presents options with perceptual descriptors, developer selects by DAW listening test. In the dual-reader format for DSP sections, the developer rationale block must be interpretable by a Python/JS developer **without reading the AI-executable spec** — use perceptual language ("creates warmth by reducing high-frequency energy"), not technical DSP terms
6. **Sound Design Curriculum** — Topic hierarchy (general theory → specialized areas → creative focuses), source types and quality tiers (academic, professional, instructional), coverage plan ensuring all sound types are achievable with psybient and related genres as creative seed topics
7. **UI/UX Standards** — Research targets for modern audio plugin design, synthesizer control layout theory, design principles for clean modern interfaces, design system approach
8. **Business Strategy** — Market analysis (2026 VST landscape), marketing plan, pricing strategy (accessible to average consumer), marketplace selection and listing strategy (KVR Audio and others), revenue projections and break-even analysis
9. **Licensing and Anti-Piracy** — Concrete system recommendations with names, implementation approach, cost-effectiveness for solo developer, marketplace-specific anti-piracy considerations
10. **Success Criteria** — Measurable outcomes that define success: a creative and useful VST plugin with full business plan strategy encompassing all aspects of marketing VST plugins in 2026, licensing to counter piracy, and a profitable product sales strategy

Domain-to-section mapping: Task domain 1 (VST Plugin Development) spans Sections 3 and 5. Task domain 7 (C++ Reference) belongs in Section 3. All other task domains map directly to their corresponding numbered section (domain 2 → Section 6, domain 3 → Section 7, domain 4 → Section 8, domain 5 → Section 9, domain 6 → Section 4).

Each section must contain specific, actionable content — concrete strategies, named tools and systems, referenced approaches — not just topic headers or placeholder descriptions. Exception: the C++ Reference content is intentionally scoped narrow — depth over breadth; do not pad it to match the length of other sections.

Dual-reader structure: Within each section, distinguish between (a) **AI-executable specification** — precise, structured content an AI agent can act on directly (parameters, named systems, criteria, decision rules) and (b) **developer rationale** — plain-language explanation of what the section means and what the human must review or decide. These may be co-located within prose rather than split into formal sub-sections.
</output_format>

<examples>
Examples of the specificity expected in commission brief entries. Structural formats (bridge components a–f, phase entry schema) are required. Specific parameter values and domain content are illustrative.

**KB Bridge System entry** (all six components required):
"Sound Design ↔ DSP Bridge — Supersaw Width:
(a) Direction: bidirectional (sound design concept → DSP parameters; DSP parameters → perceptual descriptor)
(b) Source vocabulary: 'supersaw', 'detuning for width', 'stereo spread', 'chorus thickness'
(c) Target vocabulary: oscillator count, detune_spread (semitones), per-voice panning angle, unison mode
(d) Transformation rule: map perceived width descriptor to oscillator count (3–7) and detune spread (0.02–0.25 semitones); wider = more oscillators + larger spread; apply per-voice stereo panning proportional to detune spread
(e) Example: input 'wide supersaw pad' → output: 7 detuned sawtooth oscillators, detune_spread=0.12 semitones, per-voice panning ±45°
(f) Coverage criterion: any source term describing oscillator-based width or detuning thickness is bridged; harmonic distortion or reverb-based width requires a separate bridge entry"

**Development Phase entry:**
"Phase 4 — DSP Implementation: Agent skill: `juce-dsp-implementation`. AI responsibility: implement DSP algorithms per spec, optimize for real-time performance, handle C++ audio thread constraints. Human responsibility: specify creative sound targets, evaluate sonic output, approve preset sounds. Exit criteria: all DSP modules pass unit tests and produce perceptually correct output in DAW testing."

**Insufficient KB Bridge entry (do not produce this):**
"Sound Design ↔ DSP Bridge: Translates sound design terminology into DSP parameters." — This names the bridge but provides no transformation rule, no vocabulary mapping, and no example pair. A consuming AI session cannot use it.

**Insufficient Development Phase entry (do not produce this):**
"Phase 4 — DSP Implementation: Implement DSP algorithms." — This omits agent skill, AI/human responsibility split, and exit criteria. A consuming AI session cannot determine what to do, who does it, or when it is complete.
</examples>

<verification>
Before finalizing the commission brief, verify:
- Every item from the technical stack (C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK, all Linux deps) is accounted for in the architecture section
- Sound design coverage spans general synthesis theory AND all five specified creative focus areas (psychedelic, ambient, pad, creative/abstract, psybient) with best-quality source references
- Budget constraints ($100–$200/month AI subscription, no other significant capital) are reflected in every phase involving tool costs or external resources
- The KB architecture has explicit, named bridge systems connecting all relevant domain pairs with rationale for each bridge
- Marketing strategy addresses 2026-specific VST marketplace conditions from an enthusiast developer's perspective, not generic software marketing
- Licensing section recommends concrete anti-piracy systems effective for KVR Audio and other marketplaces, with implementation details
- No phase assumes C++ expertise, DSP knowledge, or advanced sound design skill from the developer without specifying how AI fills the gap
- C++ documentation is scoped to JUCE plugin development — not a general C++ reference, and not overloaded
- Agent skill assignments (brainstorming, writing-plans, execute-plans/subagent-driven-development) are mapped to specific phases with AI/human responsibility boundaries
- Every section contains actionable specifics, not just topic headers
- The playbook is usable by both Claude Code AI agents (for execution) and the solo developer (for oversight and creative decisions)
- The complete system — development, marketing, licensing, anti-piracy, sales strategy — produces a profitable product achievable by one person with AI assistance
- Each section contains both AI-executable specification (precise enough for an agent to act on directly) and developer rationale (plain-language explanation of what the human must review or decide)
- The KB Design section specifies every bridge system with all six components: direction, source vocabulary, target vocabulary, transformation rule, example input/output pair, and coverage criterion
- The Development Phases section states at which specific phase gate the Tier 1 KB must be complete before development proceeds
- The C++ Reference content is intentionally narrower than other content — scoped depth is the criterion, not length parity with other sections
- "Shippable milestone" is defined with two explicit, distinct meanings: **technically functional** (VST3 binary loads in DAW) for intermediate phases; **commercially viable** (full bridge chain + licensing system) for final release only — these are not interchangeable, and both definitions appear in the brief
- AAX SDK is explicitly assigned to the final commercial release phase only; all intermediate phase milestones target VST3 functional builds — this is stated in both the Technical Architecture section and the Development Phases section
- Every bridge entry's coverage criterion (component f) includes both a structural completeness test and a semantic/perceptual validation test — structural completeness alone does not guarantee correct DSP output
- Creative intent phrases are specified as verbatim-preserved cache keys throughout the bridge chain — AI agent paraphrasing of source vocabulary is explicitly prohibited at every bridge node
- The bridge chain's forward-only direction is explicitly stated, with its boundary condition documented: reverse traversal (market opportunity → creative intent) is outside scope
- The planning sequence (`brainstorming`, `writing-plans`) is gated after Tier 1 KB completion or includes a mandatory Tier 1 assumption audit revision pass — this sequencing is explicitly stated
- DSP sections in the dual-reader format use perceptual descriptors in the developer rationale block — the developer rationale is interpretable by a Python/JS developer without reading the AI-executable spec
- Bridge error correction path is specified: identify the bridge node where semantic error was introduced, update that entry, re-run downstream from that node only — not a full-phase restart
- Scope exclusions are explicitly documented in the System Vision section: (a) visual-UI-as-product plugins are outside scope; (b) conventional saturated segments (reverb, EQ, compression) are outside the creativity-gap opportunity framing
</verification>
```
