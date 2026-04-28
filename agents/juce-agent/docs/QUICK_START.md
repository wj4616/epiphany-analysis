# Quick Start Guide

> Get your first plugin running in under 2 hours

## Prerequisites

- JUCE 7.0.12+ installed
- C++17 compiler
- CMake 3.15+
- DAW (REAPER recommended for testing)

## 15-Minute Setup

### 1. Invoke the Plugin Spec Skill (Phase 0)

```
Use the juce-plugin-spec skill to define my plugin.

Plugin type: Simple effect
Purpose: A compressor with threshold, ratio, attack, release, and makeup gain
Target: VST3, Linux/macOS/Windows, 44.1-192kHz
```

The skill will ask clarifying questions. Answer with:
- **Sound identity:** "Transparent compression, gentle gain reduction"
- **Character words:** Clean, subtle, transparent
- **Reference plugins:** Built-in DAW compressor, TDR Kotelnikoff

### 2. Create Implementation Plan (Phase 2)

```
Use the writing-plans skill to create an implementation plan.

The spec is at: docs/superpowers/specs/YYYY-MM-DD-plugin-design.md
```

### 3. Implement DSP (Phase 4)

```
Use the juce-dsp-implementation skill to implement the compressor module.
```

### 4. Test in DAW (Phase 9)

```
Use the juce-daw-testing skill to test the compressor in REAPER.
```

---

## Using the Example Project

The `examples/WarmPadSynth/` directory contains a complete, working example:

### Explore the Spec

```bash
cat examples/WarmPadSynth/README.md
```

Key sections:
- **Plugin Specification** - What we're building
- **Signal Flow** - How audio flows through the plugin
- **Parameters** - All controllable values
- **Gain Staging** - How levels are managed
- **Presets** - Ready-made sounds

### Key Code Patterns

The example includes working code for:

| Pattern | File | Purpose |
|---------|------|---------|
| TPT Filter | `DSP/Filter.cpp` | Zero-delay filter for smooth response |
| PolyBLEP Oscillator | `DSP/Oscillator.cpp` | Alias-free oscillator |
| SmoothedValue | `PluginProcessor.cpp` | Click-free parameter changes |
| Custom LookAndFeel | `GUI/LookAndFeel.cpp` | Consistent UI styling |

### Session State

See how session state tracks progress:
```bash
cat examples/WarmPadSynth/README.md | grep -A 30 "Session State Example"
```

---

## Common Workflows

### Creating a New Plugin

```
1. Invoke juce-plugin-spec (Phase 0)
2. Answer questions about plugin type, sound, constraints
3. Invoke writing-plans (Phase 2)
4. Invoke juce-dsp-implementation (Phase 4)
5. Build and test (Phase 8-9)
```

### Adding a New DSP Module

```
1. Invoke juce-dsp-implementation
2. Specify which module to add
3. Follow the two-stage review process
4. Invoke juce-smoothedvalue-audit if parameters need smoothing
```

### Debugging Sound Issues

```
1. Invoke juce-daw-testing (Phase 9)
2. Describe what you hear: "too bright", "muddy", "no movement"
3. Use juce-sound-design-bridge to translate to parameters
4. Adjust parameters
5. Log validation results
```

### Testing Audio Thread Safety

```
1. Invoke juce-audio-thread-audit
2. Point to your source files
3. Review audit report
4. Fix any issues found
```

---

## Time Estimates

| Complexity | Typical Time | Example |
|------------|---------------|---------|
| Simple Effect | 8-16 hours | Compressor, EQ, Delay |
| Moderate Effect | 16-32 hours | Multi-band compressor |
| Simple Synth | 16-32 hours | Basic subtractive synth |
| Moderate Synth | 32-60 hours | Full polysynth |
| Complex Plugin | 60-120+ hours | Granular, physical modeling |

---

## When to Start Fresh

Start a new session when:
- After completing a phase gate
- After 3-4 implementation tasks
- When context feels fragmented
- When switching from implementation to DAW testing
- After 2+ hours

Carry forward:
- Session state file
- Spec file + key decisions
- Plan file + task progress
- Commit hashes for completed work

---

## Getting Help

1. **Check the playbook:** `playbooks/vst-plugin-playbook-v7-unified.json`
2. **Read skill docs:** `.claude/skills/juce-*/SKILL.md`
3. **Run tests:** `tests/test-*.sh`
4. **See example:** `examples/WarmPadSynth/README.md`
5. **Review ground truth:** `validation-logs/GROUND_TRUTH_PRESETS.md`

---

## Next Steps

1. Explore the WarmPadSynth example
2. Try creating your own simple effect
3. Graduate to a synthesizer
4. Use session state to track progress across sessions