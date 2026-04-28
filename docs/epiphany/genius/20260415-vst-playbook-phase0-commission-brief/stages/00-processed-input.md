# Processed Input — epiphany-genius v1.1.0

## Input Type
prompt_epiphany

## Primary Problem (extracted from `<task>`)
Generate the Phase 0 commission brief section for a comprehensive VST audio plugin development playbook. This brief defines the complete system for creating commercial VST audio plugins — spanning development, sound design, marketing, licensing, and business strategy — optimized for a solo AI-assisted developer.

Seven domains must be addressed:
1. VST Plugin Development (primary focus)
2. Sound Design Knowledge Base (with creative focuses: psychedelic, ambient, pad, creative/abstract, psybient)
3. UI/UX Design for Audio Plugins
4. Marketing and Sales Strategy (2026 VST marketplace)
5. Licensing and Anti-Piracy
6. Knowledge Base Architecture (multi-layer with bridge systems connecting domains)
7. C++ Reference (scoped to JUCE plugin development only)

## Known Facts (extracted from `<context>`)
- Market opportunity: creativity gap in VST marketplace — underserved segment wanting creative, sonically adventurous plugins at accessible prices
- AI-assisted development makes complex DSP algorithms achievable by solo developers
- Developer: solo, C++ beginner (Python/JS proficient), basic sound design knowledge, no DSP expertise
- AI tools: Claude Code ($100-200/month) with playbook KB system
- Technical stack: C++17, CMake 3.22+, GCC 7+/Clang 6+, JUCE 8.0.x, VST3 SDK 3.8.x, AAX SDK (final release only)
- Linux development environment with specified dependencies
- Agent skills available: brainstorming, writing-plans, execute-plans/subagent-driven-development

## Constraints (extracted from `<constraints>`)
- Budget: only AI subscription cost ($100-200/month), use free libraries unless justified
- Quality priority ordering: (1) DSP audio quality, (2) creative sound design capability, (3) marketing strategy, (4) licensing/anti-piracy, (5) UI polish, (6) C++ reference completeness
- KB two-tier model: Tier 1 (core, required before development) + Tier 2 (supplemental, on-demand)
- Agent workflow: gate brainstorming/writing-plans after Tier 1 KB completion
- DSP decision boundary: AI presents options with perceptual descriptors, developer selects by DAW listening test
- Bridge chain: forward-direction only (creative intent → artifact); reverse traversal outside scope
- Creative intent phrases preserved verbatim as cache keys throughout bridge chain
- Bridge coverage criteria: structural completeness + semantic/perceptual validation
- Bridge error correction: point correction at faulty node, re-run downstream only
- Scope exclusions: (a) visual-UI-as-product plugins, (b) conventional saturated segments (reverb, EQ, compression)
- Shippable milestone: two distinct definitions — technically functional (VST3 loads in DAW) for intermediate phases; commercially viable (full bridge chain + licensing) for final release only
- AAX SDK: final release phase only
- Dual-reader structure: AI-executable specification + developer rationale (perceptual language for non-C++ readers)

## Edge Cases
- Library cost: build-vs-buy evaluation
- KB gaps: Tier 1 omission (blocking) vs Tier 2 addition (non-blocking)
- Budget limit mid-project: each phase produces functional milestone
- DSP complexity exceeds developer ability: AI handles implementation, developer handles review/testing
- Marketplace conditions change: durable principles alongside 2026 tactics
- Bridge entry produces incorrect DSP output: bridge entry defect, not code defect — point correction path
- Creative intent outside established vocabulary: treat as Tier 2 KB gap, don't force-map

## Verification Checklist (from `<verification>`)
23 verification items covering: technical stack coverage, sound design scope, budget reflection, KB bridge completeness, marketing specificity, licensing concreteness, C++ developer gap coverage, C++ reference scoping, agent skill mapping, actionable specificity, dual-reader format, system profitability, bridge six-component specification, Tier 1 phase gate, AAX release-only assignment, semantic/perceptual validation, verbatim cache keys, forward-only bridge direction, planning sequence gating, perceptual descriptors in dual-reader, bridge error correction path, scope exclusions, shippable milestone definitions