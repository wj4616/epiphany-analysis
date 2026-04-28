# Processed Input — 20260413-phase-0-commission-brief

input_type: prompt_epiphany
source_file: ~/prompts/vst-playbook-phase0-commission-brief-verbose.md

---

## PRIMARY PROBLEM (extracted from <task>)

Generate the Phase 0 commission brief section for a comprehensive VST audio plugin development playbook. This brief defines the complete system for creating commercial VST audio plugins — spanning development, sound design, marketing, licensing, and business strategy — optimized for a solo AI-assisted developer.

The playbook must address these seven domains:

1. **VST Plugin Development (Primary Focus)** — JUCE C++, CMake, DSP algorithms. Focus on best-sounding VST synthesizers and effects. Must support any plugin type, with special emphasis on creative, complex, abstract effects newly achievable through AI agent tool use.

2. **Sound Design Knowledge Base** — Advanced synthesis techniques, synth patch creation, synth module sound design. Creative seed topics (not exclusive): Psychedelic, Ambient, Pad, Creative/abstract, Psybient sound design. Must also cover general sound design theory for all sound types. Best-quality sources: academic papers, professional standards, expert instructional guides.

3. **UI/UX Design for Audio Plugins** — Modern audio plugin interface standards, synthesizer control layout theory, clean modern audio tool interface design. Current best practices for professional audio plugin control presentation, visualizations, and workflows.

4. **Marketing and Sales Strategy** — 2026 VST marketplace conditions from an enthusiast developer's perspective. All aspects: marketplace positioning, accessible pricing, modern promotional approaches.

5. **Licensing and Anti-Piracy** — Commercial licensing systems for VST plugins. How to sell licenses on audio plugin marketplaces. Anti-piracy countermeasures most effective for KVR Audio and other marketplaces.

6. **Knowledge Base Architecture** — Complex, multi-layered KB system with bridge systems connecting domains. Bridge chain: creative intent → sound design concept → DSP parameter → C++ implementation → UI control → marketing descriptor. Two-tier KB: Tier 1 (core, required before development begins) and Tier 2 (supplemental, harvestable on demand). Full six-component bridge specifications: (a) direction, (b) source vocabulary, (c) target vocabulary, (d) transformation rule, (e) example input/output, (f) coverage criterion.

7. **C++ Reference (Scoped)** — Only what is needed for JUCE plugin development: real-time audio constraints, memory management for audio threads, template patterns for DSP. Not a general C++ reference.

---

## KNOWN FACTS (extracted from <context>)

### Market Opportunity
- VST marketplace has a creativity gap — conventional designs dominate
- AI-assisted development enables solo developers to build complex DSP effects previously requiring teams
- Creative, complex, abstract audio effects are now commercially viable for solo operations

### Developer Profile
- Solo developer — one person handles all development, business, and marketing
- AI tools: Claude Code ($100/month, may expand to $200/month max)
- Programming: Python, JavaScript proficient; C++ — knows theory, first actual project
- Audio: experienced VST user, basic sound design; no DSP algorithm knowledge (AI fills this)
- Agent skills available: brainstorming, writing-plans, execute-plans / subagent-driven-development

### Technical Stack
- C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK
- Linux development dependencies: libx11-xcb-dev, libxcb-util-dev, libxcb-cursor-dev, libxcb-xkb-dev, libxkbcommon-dev, libxkbcommon-x11-dev, libfontconfig1-dev, libcairo2-dev, libgtkmm-3.0-dev, libsqlite3-dev, libxcb-keysyms1-dev

---

## CONSTRAINTS (extracted from <constraints>)

**Budget:** Freely available libraries only (unless proven necessary); $100–$200/month AI subscription is the only planned cost. Each phase produces a shippable milestone.

**Technical:** Exact stack (C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK) with all specified Linux deps.

**Quality Priority (when trade-offs arise):**
1. DSP audio quality and best-sounding output
2. Creative sound design capability and breadth
3. Marketing strategy and commercial viability
4. Licensing and anti-piracy protection
5. UI polish
6. C++ reference completeness (intentionally narrow)

**KB:** Two-tier system — Tier 1 complete before dev begins; Tier 2 harvestable on demand. Full six-component bridge specs for all domain pairs. C++ reference scoped to JUCE only.

**Agent Workflow:** brainstorming → specifications; writing-plans → plans; execute-plans/subagent-driven-development → execution. Clear AI vs. human responsibility boundaries at each phase.

---

## EDGE CASES (for S5/S6 consideration)
- Library cost: build-vs-buy trade-off if paid library required; default to free alternatives
- KB gaps mid-development: classify as Tier 1 omission (blocking) or Tier 2 addition (non-blocking)
- Budget limit mid-project: each phase must produce a functional milestone
- DSP complexity exceeds developer's C++ ability: AI handles full implementation; human handles review, testing, creative direction
- Marketplace conditions change: durable principles + 2026-specific tactics

---

## OUTPUT STRUCTURE REQUIRED (from <output_format>)
10 sections: System Vision, Developer Profile & Constraints, Technical Architecture, KB Design (with full six-component bridge specs), Development Phases (all phases with AI/human split, agent skills, exit criteria, Tier 1/2 phase gate), Sound Design Curriculum, UI/UX Standards, Business Strategy, Licensing & Anti-Piracy, Success Criteria.

Dual-reader format per section: (a) AI-executable specification, (b) developer rationale.

---

## INPUT INVENTORY
- Role definition block
- Audience block (dual-reader: AI agents + solo developer)
- Context: market opportunity, developer profile, technical stack
- Task: 7 domains with detailed specifications
- Constraints: budget, technical, quality priority, KB, agent workflow
- Edge cases: 5 scenarios
- Output format: 10 sections with specificity requirements
- Examples: KB bridge entry, development phase entry, insufficient examples (negative)
- Verification: 16-item checklist
