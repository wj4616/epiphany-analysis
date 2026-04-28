# JUCE Audio Plugin Coding Agent System

A comprehensive knowledge-based system for developing professional VST3/AU/AAX audio plugins using JUCE.

## Overview

This system provides:
- **Phase-based workflow** (Phases 0-12) for complete plugin development
- **Sound Design Knowledge Base** for translating sonic concepts to DSP parameters
- **UI Design Knowledge Base** for JUCE LookAndFeel implementations
- **Technical Knowledge Base** covering JUCE, C++, CMake, and DSP patterns
- **Validation logging** for learning from DAW testing results

## Quick Start

### 1. Define Your Plugin (Phase 0)

Use the `juce-plugin-spec` skill to define your plugin:

```
Invoke the juce-plugin-spec skill to define a new plugin.
Answer the questions about plugin type, sound identity, and technical constraints.
```

### 2. Implement Your Plugin (Phases 1-7)

Follow the phase workflow:
- **Phase 1:** Performance audit (if upgrading existing code)
- **Phase 2:** Architecture and task breakdown
- **Phase 3:** Project setup and test framework
- **Phase 4:** DSP implementation
- **Phase 5:** State management
- **Phase 6:** Integration
- **Phase 7:** GUI implementation

### 3. Test in DAW (Phase 9)

Use the `juce-daw-testing` skill to test in REAPER or your preferred DAW.

### 4. Release (Phases 10-12)

- Phase 10: Release preparation
- Phase 11: Profiling and optimization
- Phase 12: Retrospective

## Key Files

| File | Purpose |
|------|---------|
| `playbooks/vst-plugin-playbook-v7-unified.json` | Main playbook (3500+ lines) |
| `validation-logs/GROUND_TRUTH_PRESETS.md` | Verified preset templates |
| `validation-logs/global-patterns.json` | Aggregated validation patterns |
| `validation-logs/harvested/` | Harvested educational content |

## Skills

| Skill | When to Use |
|-------|-------------|
| `juce-plugin-spec` | Phase 0 - Define plugin concept |
| `juce-dsp-implementation` | Phase 4 - Implement DSP modules |
| `juce-daw-testing` | Phase 9 - Test in DAW |
| `juce-sound-design-bridge` | Translate sonic descriptors to parameters |
| `juce-ui-bridge` | Design and implement GUI |
| `juce-audio-thread-audit` | Verify audio thread safety |
| `juce-smoothedvalue-audit` | Verify parameter smoothing |

## Testing

Run integration tests:

```bash
cd tests/
./test-e2e-workflow.sh      # End-to-end workflow
./test-sound-design-bridge.sh
./test-ui-bridge.sh
./test-capability-schema.sh
./test-phase-transitions.sh
./test-plugin-spec.sh
./test-smoothedvalue-audit.sh
./test-audio-thread-audit.sh
./test-preset-validation.sh
```

## Knowledge Bases

### Sound Design KB

Translates sonic descriptors to DSP parameters:
- "Warm" → `filter_cutoff: [0.2, 0.4]`
- "Bright" → `filter_cutoff: [0.6, 1.0]`
- "Punchy" → `fast attack, filter envelope`

### UI Design KB

Translates visual concepts to JUCE implementations:
- "Modern clean" → `LookAndFeel_V4 Dark`
- "Vintage hardware" → Custom LookAndFeel with textures
- Control sizing → 48-64px for primary controls

### Technical KB

JUCE patterns, failure modes, and best practices:
- Audio thread safety patterns
- SmoothedValue usage patterns
- State save/restore patterns
- Common failure modes and prevention

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Playbook (v7)                            │
│  ┌──────────────┐ ┌──────────────┐ ┌──────────────────────┐  │
│  │  Technical   │ │ Sound Design │ │      UI Design       │  │
│  │  Knowledge   │ │  Knowledge   │ │      Knowledge       │  │
│  │     Base     │ │     Base     │ │        Base          │  │
│  └──────────────┘ └──────────────┘ └──────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
              │                    │                    │
              ▼                    ▼                    ▼
┌─────────────────────────────────────────────────────────────┐
│                      Skill Layer                             │
│  ┌────────────┐ ┌────────────┐ ┌────────────────────────────┐ │
│  │ juce-plugin│ │ juce-dsp   │ │ juce-sound-design-bridge  │ │
│  │   -spec    │ │implementation│ │    juce-ui-bridge        │ │
│  └────────────┘ └────────────┘ └────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
              │                    │
              ▼                    ▼
┌─────────────────────────────────────────────────────────────┐
│                   Validation Layer                            │
│  ┌──────────────────┐ ┌─────────────────────────────────────┐│
│  │ Ground Truth     │ │         Global Patterns             ││
│  │    Presets       │ │    (Confidence Scoring)             ││
│  └──────────────────┘ └─────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Plugin Types Supported

| Type | Workflow | Key Phases |
|------|----------|------------|
| Synthesizer | Oscillator → Filter → Envelope → Effects | 0, 2, 4, 7, 9 |
| Effect | Input → Processing → Output → Mix | 0, 1, 4, 7, 9 |
| Analyzer | Input → Analysis → Display | 0, 4, 7, 9 |
| Hybrid | Synthesis + Effects + Routing | 0, 2, 4, 6, 7, 9 |

## Ground Truth Presets

Verified parameter ranges for common sounds:

| Preset | Category | Key Parameters |
|--------|----------|-----------------|
| Bright Pad | Pad | cutoff: 0.75, resonance: 0.15 |
| Warm Pad | Pad | cutoff: 0.30, chorus: 0.40 |
| Aggressive Bass | Bass | cutoff: 0.35, drive: 0.70 |
| Subtle Chorus Pad | Pad | chorus: 0.30, cutoff: 0.50 |

## JUCE Version Compatibility

| JUCE Version | Status | Notes |
|--------------|--------|-------|
| 7.0.6+ | Recommended | VBlankAttachment available |
| 7.0.12 | Tested | Current stable |
| 8.x | Partial | Animation module requires JUCE 8 |

## Getting Help

1. Check the playbook: `playbooks/vst-plugin-playbook-v7-unified.json`
2. Review the skills: `.claude/skills/juce-*/SKILL.md`
3. Run tests: `tests/test-*.sh`
4. Review ground truth presets: `validation-logs/GROUND_TRUTH_PRESETS.md`

## Contributing

To add new presets or translations:
1. Add to `validation-logs/GROUND_TRUTH_PRESETS.md`
2. Add to `sound_design.preset_templates` in playbook
3. Verify in DAW testing
4. Update confidence scores in `global-patterns.json`

## Version History

| Version | Name | Changes |
|---------|------|---------|
| 7.0.0 | Sound Design Integration | Added Sound Design KB, UI KB, plugin types |
| 6.0.0 | Unification | Merged multiple playbooks |
| 5.0.0 | Phase Refinement | Added prevention rules, failure modes |

## License

Internal use only. Contains proprietary development patterns and knowledge bases.