# Processed Input

**Input type:** prompt_epiphany XML (Type B)
**Source file:** ~/prompts/vst-playbook-phase0-commission-brief-verbose.md
**User directive:** analyze

---

## Primary Task (from <task>)

Generate the **Phase 0 commission brief** section for a comprehensive VST audio plugin development playbook. The brief defines the complete system for creating commercial VST audio plugins — spanning development, sound design, marketing, licensing, and business strategy — optimized for a solo AI-assisted developer.

The playbook must address seven domains:

1. **VST Plugin Development (Primary Focus)** — JUCE framework plugin development in C++, CMake build system, DSP algorithm implementation. Best-quality DSP/C++ audio algorithms suitable for JUCE. Must support any audio plugin type, with special emphasis on creative, complex, abstract effects newly achievable through AI agent tool use.

2. **Sound Design Knowledge Base** — Advanced synthesis techniques, synthesizer patch creation theory, synth module sound design. Creative focus areas (seed topics, not boundaries): psychedelic, ambient, pad, creative/abstract, psybient. Must also cover general sound design theory for all sound types. Reference academic papers, professional standards, instructional guides.

3. **UI/UX Design for Audio Plugins** — Modern audio plugin interface design standards, synthesizer control layout theory, clean modern audio tool interface design.

4. **Marketing and Sales Strategy** — 2026 VST marketplace strategy from enthusiast-developer perspective. Marketplace positioning, budget-accessible pricing, modern promotional approaches.

5. **Licensing and Anti-Piracy** — Commercial licensing systems for VST plugins. How to sell licenses and list on marketplaces (KVR Audio and others). Anti-piracy countermeasures most effective for KVR Audio.

6. **Knowledge Base Architecture** — Complex, multi-layered KB with bridge systems connecting domains. Bridge journey: creative intent → sound design concept → DSP parameter → C++ implementation → UI control → marketing descriptor. Additional cross-bridges (UI ↔ audio UX, marketing ↔ pricing). Two-tier structure: Tier 1 (core KB — all domain layers, all bridge systems with full six-component specifications, baseline content) complete before development; Tier 2 (supplemental, gap-filled mid-project, non-blocking).

7. **C++ Reference (Scoped)** — C++ docs relevant to audio plugin development only (real-time audio constraints, memory management for audio threads, template patterns for DSP). Not a general C++ reference. Depth over breadth.

---

## Role Frame (from <role>)

Expert VST plugin development strategist and playbook architect with deep knowledge of: JUCE framework C++ audio plugin development, real-time DSP algorithm design/optimization, sound synthesis theory (subtractive, additive, granular, spectral, FM, wavetable, physical modeling), psychedelic/ambient sound design aesthetics, modern audio plugin UI/UX standards, VST marketplace business strategy and pricing, software licensing and anti-piracy systems for audio software, AI-assisted software development workflows using Claude Code and agent-based systems.

## Dual Audience (from <audience>)

1. **Claude Code AI agents** — executing development phases, referencing KB content, generating code and specifications.
2. **Solo developer** — C++ beginner (experienced Python/JS), basic sound design knowledge, no DSP algorithm expertise. Must understand every phase well enough to review AI output and make creative decisions.

Output must be technically precise enough for AI execution AND clearly explained enough for developer comprehension.

---

## Context (from <context>)

### Market opportunity

VST audio plugin marketplace has a creativity gap. Existing plugins cluster around conventional designs — either expensive flagship products from established studios or budget options lacking creative ambition. Underserved segment: consumers wanting creative, sonically adventurous plugins at accessible price points. AI-assisted development changes competitive landscape — complex DSP/creative effects that previously required teams of experienced C++ audio engineers are now achievable by solo developer using AI tools.

### Developer profile

- **Team size:** Solo. One person handles development, business, marketing, using AI agents.
- **AI tools:** Claude Code (currently $100/month; max $200/month) with playbook KB system.
- **Development approach:** Multiple Claude Code sessions per project; total coding effort must remain reasonable for one person.
- **Programming skills:** Proficient in Python/JavaScript; familiar with basic C++ theory but new to C++ development in practice — first VST plugin project.
- **Audio skills:** Experienced VST user with basic sound design knowledge; no DSP algorithm knowledge (AI fills this gap).
- **Agent skills available:** `brainstorming` (specs), `writing-plans` (implementation plans), `execute-plans` / `subagent-driven-development` (execution).

### Technical stack

- **Core:** C++17, CMake 3.22+, GCC 7+ or Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK
- **Linux dev deps:** libx11-xcb-dev, libxcb-util-dev, libxcb-cursor-dev, libxcb-xkb-dev, libxkbcommon-dev, libxkbcommon-x11-dev, libfontconfig1-dev, libcairo2-dev, libgtkmm-3.0-dev, libsqlite3-dev, libxcb-keysyms1-dev

---

## Constraints (from <constraints>)

### Budget and resources
- DO use freely available libraries unless paid alternative is specifically necessary and justified
- DO NOT require capital investment beyond AI subscription ($100–$200/month)
- DO scope every phase for solo completion with AI
- DO design phases to produce shippable milestones (working intermediate product if paused)

### Technical
- DO target specified stack exactly (C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK)
- DO include all specified Linux dev dependencies

### Quality priority (trade-off order, highest → lowest)
1. DSP audio quality and best-sounding output
2. Creative sound design capability and breadth
3. Marketing strategy and commercial viability
4. Licensing and anti-piracy protection
5. UI polish and modern design
6. C++ reference completeness (keep scoped)

### Knowledge base
- DO build KB in two tiers: Tier 1 (core, complete before dev) with all named layers, all bridge systems with full six-component specs, baseline content for sound design, JUCE/VST, UI/UX, marketing, licensing; Tier 2 (supplemental, mid-project gap fills, non-blocking)
- DO NOT overload the C++ reference — scope to JUCE plugin development only
- DO cover sound design broadly while using specified creative focuses as seed topics
- DO include bridge systems connecting every relevant domain pair

### Agent workflow
- DO map skills to phases: `brainstorming` → specs, `writing-plans` → impl plans, `execute-plans` / `subagent-driven-development` → execution
- DO define clear boundaries between AI-generated work and human creative decisions at each phase

---

## Edge cases (from <edge_cases>)

- **Library cost:** If required library not free, evaluate build-vs-buy trade-off; default to building in-house or finding free alternative.
- **KB gaps mid-dev:** Two-tier handling — Tier 1 omission (blocking, must fill before current phase); Tier 2 addition (non-blocking, fill and continue). Gap-filling must not restructure existing layers/bridges.
- **Budget limit mid-project:** Each phase produces functional milestone; pause-safe across sessions.
- **DSP complexity > dev's C++ ability:** AI agent handles full implementation; developer handles review, testing, creative direction.
- **Marketplace shifts:** Marketing strategy combines durable principles + 2026-specific tactics.

---

## Required output format (from <output_format>)

Phase 0 commission brief with sections:

1. **System Vision** — problem, market opportunity, value proposition, why AI-assisted solo is viable
2. **Developer Profile and Constraints** — skills inventory, budgets, skill gaps + AI fills
3. **Technical Architecture** — full stack spec, deps, build system, plugin formats, platform targets
4. **Knowledge Base Design** — layer structure, bridge systems (six components: direction, source vocab, target vocab, transformation rule, concrete I/O example, coverage criterion), topic taxonomy, ingestion/quality strategy
5. **Development Phases** — all phases from spec through release: objectives, agent skills, AI-vs-human responsibility, strategies, exit criteria. Each phase solo-completable with AI, producing shippable milestone. Map Tier 1/Tier 2 KB model to specific phase gates.
6. **Sound Design Curriculum** — topic hierarchy, source-type quality tiers, coverage plan
7. **UI/UX Standards** — research targets, layout theory, principles, design-system approach
8. **Business Strategy** — 2026 market analysis, marketing plan, accessible pricing, marketplace selection (KVR Audio and others), revenue projections, break-even
9. **Licensing and Anti-Piracy** — concrete system recommendations (named), implementation approach, solo-dev cost-effectiveness, marketplace-specific
10. **Success Criteria** — measurable outcomes: creative/useful VST + full business plan (marketing, licensing, anti-piracy, profitable sales)

**Domain-to-section mapping:** Task domain 1 → Sections 3 & 5. Task domain 7 → Section 3. Domain 2 → Section 6. Domain 3 → Section 7. Domain 4 → Section 8. Domain 5 → Section 9. Domain 6 → Section 4.

**Specificity requirement:** Each section must contain concrete strategies, named tools/systems, referenced approaches — not just topic headers. Exception: C++ Reference intentionally narrow (depth over breadth; do not pad).

**Dual-reader structure within each section:** (a) AI-executable specification (precise, structured — parameters, named systems, criteria, decision rules) and (b) developer rationale (plain-language explanation of what human must review/decide). May be co-located in prose.

---

## Examples provided (from <examples>)

- **KB Bridge System entry** (all six components required): Example "Sound Design ↔ DSP Bridge — Supersaw Width" with direction, source vocab, target vocab, transformation rule (oscillator count 3–7, detune 0.02–0.25 semitones), concrete I/O pair ("wide supersaw pad" → 7 oscillators, detune_spread=0.12, panning ±45°), coverage criterion.
- **Development Phase entry:** Example "Phase 4 — DSP Implementation" with agent skill, AI/human responsibility split, exit criteria.
- **Insufficient examples** of each (no transformation rule, no exit criteria) — do not produce.

---

## Verification checklist (from <verification>)

15 items covering: full tech stack accounted for; sound design covers general theory + all 5 creative focuses; budget constraints reflected; KB has explicit bridges with rationale; 2026-specific marketing; concrete named anti-piracy systems for KVR; no phase assumes C++/DSP/sound-design expertise without specifying AI gap-fill; C++ scope narrow; agent skills mapped to phases with responsibility boundaries; every section contains actionable specifics; dual-reader usability; profitability achievable solo; AI/human split per section; bridges have all six components; Tier 1 phase gate stated; C++ Reference intentionally narrower.

---

## Analysis directive

User passed `analyze` directive with no flags. Default scale auto-detection applied. This is a multi-section specification document — treat as DEEP scale input. Analysis should apply the full cognitive pipeline (S1 → S7 + OSP) to assess this prompt's coherence, completeness, hidden assumptions, risks, and effectiveness as a Phase 0 commission brief generator.
