# Plugin Specification Template

> Template for Phase 0 plugin specification. Fill out all sections before proceeding to Phase 1.

---

## Plugin Identity

### Name
[Plugin name]

### Type
- [ ] Synthesizer
- [ ] Effect (filter, delay, reverb, distortion, dynamics, etc.)
- [ ] Utility (meter, analyzer, tool)
- [ ] Hybrid

### One-Line Description
[One sentence describing what the plugin does]

### Purpose Statement
[One paragraph explaining why this plugin exists, what problem it solves, and for whom]

---

## Sound Identity

### Primary Character
[Use KB bridges to translate sonic descriptions]

**Sonic Descriptors** (check all that apply):
- [ ] Warm
- [ ] Bright
- [ ] Lush
- [ ] Punchy
- [ ] Psychedelic
- [ ] Ambient
- [ ] Evolving
- [ ] Aggressive
- [ ] Gentle
- [ ] Other: [describe]

### Parameter Ranges (from KB bridges)
| Parameter | Range | Default | Notes |
|-----------|-------|---------|-------|
| Filter Cutoff | 0.2 - 0.4 | 0.3 | For "warm" character |
| ... | ... | ... | ... |

---

## Technical Constraints

### Build Requirements
| Requirement | Value |
|-------------|-------|
| JUCE Version | 8.0.x |
| C++ Standard | 17 |
| CMake Version | 3.22+ |
| VST3 SDK | 3.8.x |

### Target Platforms
| Platform | Priority | Format |
|----------|----------|--------|
| Linux | Primary | VST3 |
| Windows | Required | VST3 |
| macOS | Required | VST3, AU |
| Pro Tools | Optional | AAX |

### Target DAWs
| DAW | Platform | Priority |
|-----|----------|----------|
| REAPER | All | Primary |
| Ableton Live | All | Secondary |
| Logic Pro | macOS | Secondary |

### Performance Requirements
| Metric | Target |
|--------|--------|
| CPU Budget | < 5% single core at 48kHz, 256 buffer |
| Memory | < 50MB |
| Instance Limit | 4+ instances simultaneously |

---

## Signal Flow

### Block Diagram
```
[Input]
    │
    ▼
[Module 1]
    │
    ▼
[Module 2]
    │
    ▼
[Output]
```

### Signal Flow Description
[Describe the signal flow in words]

---

## Parameters

### Parameter Table
| Parameter | Type | Range | Default | Smoothing | Automation |
|-----------|------|-------|---------|-----------|------------|
| [Name] | Continuous | 0.0 - 1.0 | 0.5 | 20ms | Yes |
| [Name] | Discrete | {A, B, C} | A | N/A | Yes |
| ... | ... | ... | ... | ... | ... |

### Parameter Categories
- **Primary**: [Most important parameters]
- **Secondary**: [Less important parameters]
- **Advanced**: [Expert parameters]

---

## Presets

### Factory Presets
| Preset Name | Character | Key Parameters |
|-------------|-----------|---------------|
| [Name] | [Character] | [Key settings] |
| ... | ... | ... |

### Preset Categories
- [ ] Category 1
- [ ] Category 2
- [ ] Category 3

---

## State Management

### State Persistence
- [ ] Save/restore all parameters
- [ ] Save/restore preset name
- [ ] Save/restore user settings

### Automation Support
- [ ] All parameters automatable
- [ ] Smooth parameter changes
- [ ] No zipper noise

---

## UI Requirements

### Layout
[Describe UI layout or attach sketch]

### Control Types
| Control | Type | Range |
|---------|------|-------|
| [Parameter] | Knob | 0-100% |
| [Parameter] | Slider | 0-100% |
| [Parameter] | Dropdown | {A, B, C} |

### Visual Design
- [ ] Modern minimal
- [ ] Vintage style
- [ ] Dark theme
- [ ] Light theme
- [ ] Custom: [describe]

### Accessibility
- [ ] Keyboard navigation
- [ ] Screen reader support
- [ ] High contrast mode

---

## Out of Scope

### What This Plugin Will NOT Do
- [Feature/behavior explicitly excluded]
- [Feature/behavior for future version]

---

## Risks and Mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| [Risk 1] | Medium | High | [Mitigation] |
| [Risk 2] | Low | Medium | [Mitigation] |

---

## Approval

### Stakeholder Sign-off
- [ ] Sound identity approved
- [ ] Technical constraints approved
- [ ] Parameters approved
- [ ] UI concept approved

### Decisions Recorded
- [ ] All decisions logged in decisions-ledger.md

---

## Next Steps

After approval, proceed to:
- Phase 1: Performance Audit (if upgrading)
- Phase 2: Architecture & Planning (if new plugin)