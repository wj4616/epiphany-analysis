# Processed Input — epiphany-genius

## Input Type
prompt_epiphany (Type B)

## Primary Problem (from `<task>`)
Generate the Phase 0 commission brief section for a comprehensive VST audio plugin development playbook. This brief defines the complete system for creating commercial VST audio plugins — spanning development, sound design, marketing, licensing, and business strategy — optimized for a solo AI-assisted developer.

The playbook must address seven domains:
1. VST Plugin Development (Primary Focus) — JUCE framework, CMake, DSP algorithms, best-sounding plugins possible
2. Sound Design Knowledge Base — Advanced synthesis, psychedelic/ambient/pad/creative/psybient focus areas
3. UI/UX Design for Audio Plugins — Modern interface standards, control layout theory
4. Marketing and Sales Strategy — 2026 marketplace, positioning, pricing, promotional approaches
5. Licensing and Anti-Piracy — Commercial systems, marketplace listings, anti-piracy countermeasures
6. Knowledge Base Architecture — Dify RAG-powered KB replacing manual bridges, cross-domain translation
7. C++ Reference (Scoped) — Audio-relevant C++ patterns only, not general reference

## Known Facts (from `<context>`)
- **Market opportunity:** VST marketplace has a creativity gap — expensive flagships vs. budget options lacking ambition. AI-assisted solo development makes creative, complex, abstract effects commercially viable.
- **Developer profile:** Solo developer, proficient Python/JS, C++ beginner, basic sound design knowledge, no DSP expertise. Uses Claude Code ($100-200/month) + Dify RAG KB system. Multiple sessions per project.
- **Technical stack:** C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK (release only). Full Linux dependency list specified.
- **KB system:** Dify RAG — semantic retrieval across all domains, replaces 9 manual bridge systems with 6-component schemas. Cross-domain translation handled natively. Confidence scoring replaces manual Tier 1/2 classification.
- **Agent skills:** brainstorming, juce-plugin-spec, writing-plans, subagent-driven-development, juce-dsp-implementation, juce-ui-bridge, juce-daw-testing, juce-audio-thread-audit, juce-smoothedvalue-audit, kb-cognitive

## Constraints (for S5 input, from `<constraints>`)
- Budget: $100-200/month AI subscription only. Per-phase budget allocations defined. DSP build-vs-buy decision procedure with $12-24 per library ceiling.
- Technical: Exact stack targeting. AAX = final release only. All Linux deps required.
- Quality priority ordering: DSP audio quality > Creative sound design > Marketing > Licensing > UI > C++ reference
- KB: Dify RAG primary. Content quality tiers A/B/C. Dynamic gap detection via confidence scoring. No domain-level gating.
- Agent workflow: Skills mapped to all 13 phases. RAG queries replace manual bridge invocations. DSP decision opacity resolved via perceptual descriptors + calibration schedule (AI-guided → Collaborative → Developer-led).
- Cross-domain: Creative intent → sound design → DSP → C++ → UI → marketing. Forward-only. RAG semantic retrieval. Original creative phrases as query anchors (no paraphrasing).
- Scope exclusions: Visual-UI-as-product plugins; conventional saturated segments (standalone reverb/EQ/compression). Scope exclusions govern product-category positioning, not DSP module selection.
- Shippable = technically functional (VST3 loads in DAW) for intermediate phases; commercially viable only at final release. Commercial viability checklist enumerated.
- Architectural thesis: creative-to-DSP precision gap is the organizing principle. RAG KB manages this gap.
- Brief = governing specification with documented-override provision.
- System boundaries: solo AI-assisted dev with asymmetric expertise, budget ≤$200/month, forward-only translation, brief authority required, equal-expertise systems excluded, RAG handles established vocabulary only.

## Edge Cases (for reference)
- Library cost, KB gaps during development, budget limit mid-project, DSP complexity exceeds developer ability, marketplace changes, RAG retrieval produces incorrect output, RAG returns conflicting info, creative intent outside RAG coverage, creative vision changes mid-development, RAG system unavailable

## Output Format Requirements
11-section commission brief with dual-reader structure (AI-executable specification + developer rationale). Verification checklist with 25+ items.

## Input Inventory
- Text: Full prompt-epiphany XML document (~257 lines)
- Code: None
- URLs: None
- File references: None
- Prior cognitive output: None