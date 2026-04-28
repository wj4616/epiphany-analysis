# PBCPB — Playbook for Building Creative VST Plugins

## Purpose Statement

Build a playbook for developing and commercially releasing creative VST audio plugins using the JUCE framework in C++. The playbook guides a single developer — assisted by Claude Code — through the full product lifecycle: from sound design research and DSP implementation through UI design, DAW testing, licensing integration, marketplace listing, and sales strategy.

### Problem Being Solved

The VST plugin marketplace in 2026 is saturated with uninspired, overpriced products that lack creative ambition. Most plugins copy existing designs rather than exploring novel sonic territory. Meanwhile, the tools to build creative alternatives (JUCE, C++, AI coding assistants) have become accessible to solo developers. This playbook enables an enthusiast developer to create genuinely creative, high-quality plugins and bring them to market at consumer-friendly price points — competing on imagination rather than budget.

### Target User

A single developer with:
- **Strong:** Python/JavaScript, general programming concepts, VST usage as a musician
- **Basic:** C++ theory (knows concepts, not fluent), sound design fundamentals
- **None:** JUCE framework, DSP algorithm implementation, commercial plugin distribution
- **AI tooling:** Claude Code ($200/month plan maximum), willing to use multiple sessions per feature
- **Platform:** Linux Mint, REAPER for DAW testing

## Project Structure

```
pbcpb-test1/
├── README.md           # This file
├── research/           # Domain research (Phase 1)
├── architecture/        # KB architecture, phase structure (Phase 2)
├── drafts/             # Playbook drafts (Phases 5-6)
├── audits/             # Quality audits (Phases 9-12)
├── testing/            # Dry-run validation (Phases 13-15)
├── final/              # Final playbook JSON
├── decisions-ledger.md  # Decision log (all phases)
├── artifact-manifest.md # File tracking (all phases)
└── metrics-tracker.md  # Metrics (all phases)
```

## Quick Links

- [Scope](scope.md) — In/out/adjacent boundaries
- [Constraints](constraints.md) — Budget, technical stack, process limits
- [Success Criteria](success-criteria.md) — Measurable outcomes
- [Decisions Ledger](decisions-ledger.md) — All recorded decisions
