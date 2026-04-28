# Creative VST Plugin Development Playbook

## Purpose Statement

This playbook guides a single developer — assisted by Claude Code — through building and commercially releasing creative VST audio plugins using JUCE and C++. It covers the full product lifecycle: from sound design research and DSP implementation through UI design, DAW testing, licensing integration, marketplace listing, and sales strategy.

### Problem Being Solved

The VST plugin marketplace in 2026 is saturated with uninspired, overpriced products that lack creative ambition. Most plugins copy existing designs rather than exploring novel sonic territory. This playbook enables an enthusiast developer to create genuinely creative, high-quality plugins and bring them to market at consumer-friendly price points — competing on imagination rather than budget.

### Target User

A single developer with:
- **Strong:** Python/JavaScript, general programming concepts, VST usage as a musician
- **Basic:** C++ theory (knows concepts, not fluent), sound design fundamentals
- **None:** JUCE framework, DSP algorithm implementation, commercial plugin distribution
- **AI tooling:** Claude Code ($200/month plan maximum), willing to use multiple sessions per feature
- **Platform:** Linux Mint, REAPER for DAW testing

### What This Playbook Produces

A developer following this playbook can:
1. Build a functional VST3 plugin using JUCE and C++17
2. Implement DSP algorithms informed by sound design research
3. Design a clean, modern UI following audio plugin interface standards
4. Integrate a commercial licensing system
5. List and sell the plugin on KVR Audio, Plugin Boutique, or equivalent
6. Execute a marketing strategy achievable by a solo developer

## Related Documents

- [Scope](scope.md) — In/out/adjacent boundaries
- [Constraints](constraints.md) — Technical, budget, and process constraints
- [Success Criteria](success-criteria.md) — Measurable outcomes
- [Decisions Ledger](decisions-ledger.md) — Rationale for key decisions
- [Artifact Manifest](artifact-manifest.md) — All produced files

## Project Structure

```
.
├── README.md           # This file
├── research/           # Phase 1: Domain research
├── architecture/       # Phase 3: Process architecture
├── drafts/             # Phase 6+: JSON drafts
├── audits/             # Phase 9-10: Quality audits
├── testing/            # Phase 12: Pilot testing
├── final/              # Phase 13: Final playbook
├── decisions-ledger.md # Decision rationale (persistent)
├── artifact-manifest.md # File index (persistent)
└── metrics-tracker.md  # KPI tracking (persistent)
```