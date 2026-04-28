# Enhanced VST Audio Plugin Development Playbook

## Purpose Statement

This playbook guides developers through creating professional-quality VST3 audio plugins using the JUCE framework, covering the complete lifecycle from DSP algorithm design through commercial release. It solves the problem of fragmented, inconsistent guidance across forums, outdated tutorials, and incomplete documentation by providing a structured, phase-by-phase workflow that integrates advanced sound design theory, modern UI design principles, and practical market launch strategies—all within a budget constraint of $200/month for AI-assisted development. This is an **optimization of an existing playbook** (the JUCE agent system at `/home/myuser/agents/juce-agent/`), enhancing it with deeper sound design knowledge, expanded UI design guidance, and commercial marketing integration.

## Problem Statement

Without this playbook, developers face:
- **Fragmented DSP knowledge**: Algorithms scattered across papers, forums, and commercial secrets
- **Sound design gaps**: No structured approach to psychedelic/ambient/creative sound design
- **UI design mystery**: Modern plugin aesthetics require specialized knowledge rarely documented
- **Market naivety**: Technical excellence doesn't guarantee commercial success
- **Budget waste**: Trial-and-error learning consumes resources inefficiently

## Target Users

| Role | Skill Level | Tools Available |
|------|-------------|-----------------|
| Primary User | C++ beginner (knows Python/JavaScript) | Claude AI ($100-200/mo), Linux Mint, REAPER DAW |
| Secondary User | Intermediate C++ developer | Same tools, seeking deeper DSP/sound design knowledge |

**Calibration**: Detail level targets the least experienced user who will execute this playbook—the C++ beginner building their first VST3 plugin.

## Project Overview

This project creates an enhanced VST plugin development playbook by:

1. **Preserving what works** from the existing JUCE agent playbook (3500+ lines, 13 phases)
2. **Enhancing weak areas**: sound design knowledge, UI design theory, market strategies
3. **Adding new capabilities**: psychedelic/ambient sound design techniques
4. **Integrating professional sources**: academic papers, professional references, expert guidance

### Relationship to Existing Work

| Existing Resource | Status | Action |
|------------------|--------|--------|
| `/home/myuser/agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json` | Active, 3500+ lines | Audit, enhance, extend |
| Sound Design KB at `playbookdata/` | Active | Enhance with psychedelic/ambient techniques |
| UI KB at `playbookdata/` | Active | Enhance with modern interface design |
| Validation logs at `validation-logs/` | Active | Preserve, extend |

## Technical Stack

| Component | Version | Notes |
|-----------|---------|-------|
| JUCE | 8.0.x | Cross-platform audio framework |
| C++ | 17 | Modern C++ features |
| CMake | 3.22+ | Build system |
| VST3 SDK | 3.8.x | Steinberg plugin format |
| AAX SDK | Latest | Pro Tools format (optional) |
| GCC | 7+ | Linux compiler |
| Clang | 6+ | Alternative compiler |

### Linux Dependencies
- libx11-xcb-dev, libxcb-util-dev, libxcb-cursor-dev
- libxcb-xkb-dev, libxkbcommon-dev, libxkbcommon-x11-dev
- libfontconfig1-dev, libcairo2-dev, libgtkmm-3.0-dev
- libsqlite3-dev, libxcb-keysyms1-dev

## Budget Constraints

| Resource | Limit | Notes |
|----------|-------|-------|
| Claude AI subscription | $100-200/month | Primary AI assistance |
| Session usage | Multiple sessions OK | Project spans multiple Claude sessions |
| External purchases | $0 | No additional paid resources required |

## Quick Links

- [Scope](./scope.md) - What's included and excluded
- [Constraints](./constraints.md) - Strategic constraints
- [Success Criteria](./success-criteria.md) - Measurable quality bar
- [Decisions Ledger](./decisions-ledger.md) - Decision tracking
- [Artifact Manifest](./artifact-manifest.md) - Output tracking