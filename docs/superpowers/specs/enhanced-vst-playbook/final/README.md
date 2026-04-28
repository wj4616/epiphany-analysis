# Enhanced VST Plugin Development Playbook

> A comprehensive playbook for creating professional VST3 audio plugins using the JUCE framework, with enhanced sound design knowledge and commercial strategies.

---

## Quick Start

### Using This Playbook

1. **Phase 0**: Start with `templates/spec-template.md` to define your plugin concept
2. **Phase 1-3**: Set up project architecture
3. **Phase 4**: Use `phases/phase-4-dsp.md` for DSP implementation
4. **Phase 9**: Use `phases/phase-9-daw-test.md` for human testing
5. **Release**: Use `validation/pluginval-checklist.md` for pre-release validation

### Knowledge Base Access

Query the KB using bridge entries:

```
kb/sound-design-kb/bridges/bridge-warm.json     → Warm sound translation
kb/sound-design-kb/bridges/bridge-bright.json   → Bright sound translation
kb/sound-design-kb/bridges/bridge-psychedelic.json → Psychedelic sound translation
```

---

## Project Structure

```
enhanced-vst-playbook/
├── README.md                    # Project overview
├── scope.md                     # Scope boundaries (in/out/adjacent)
├── constraints.md                # Technical and budget constraints
├── success-criteria.md           # Measurable quality criteria
├── decisions-ledger.md           # 34 decisions recorded
├── artifact-manifest.md          # Output tracking
├── metrics-tracker.md            # Progress metrics
│
├── architecture/
│   ├── kb-architecture.md        # KB structure
│   ├── entry-schema.json         # Entry JSON schema
│   ├── bridge-schema.json        # Bridge JSON schema
│   ├── population-strategy.md    # KB population strategy
│   ├── directory-structure.md    # KB directory tree
│   └── process-architecture.md   # 13-phase structure
│
├── research/
│   ├── domain-analysis.md        # VST domain research
│   ├── best-practices.md         # Standards and recommendations
│   ├── competitive-templates.md  # Competitive analysis
│   ├── existing-playbook-audit.md # v7 playbook audit
│   ├── cross-cutting-concerns.md  # Cross-cutting concerns
│   ├── platform-concerns.md      # Platform-specific concerns
│   └── requirements.md           # 122 requirements
│
├── phases/
│   ├── phase-0-spec.md           # Phase 0 detailed guide
│   ├── phase-4-dsp.md            # Phase 4 detailed guide
│   └── phase-9-daw-test.md       # Phase 9 detailed guide
│
├── templates/
│   └── spec-template.md          # Plugin specification template
│
├── validation/
│   └── pluginval-checklist.md    # Pre-release validation
│
├── final/
│   ├── playbook.json             # Complete playbook JSON
│   └── integration-report.md     # Integration verification
│
└── kb/                           # Knowledge Base
    ├── master-index.json
    ├── dsp-kb/                   # DSP algorithms
    ├── sound-design-kb/          # Sound design (bridged)
    ├── ui-kb/                     # UI design
    ├── failure-modes-kb/          # Failure modes
    ├── platform-kb/                # Platform concerns
    ├── market-kb/                 # Market strategies
    ├── testing-kb/                # Testing patterns
    ├── cpp-kb/                    # C++ patterns
    └── juce-kb/                   # JUCE specifics
```

---

## Key Features

### 🎵 Enhanced Sound Design

6 bridge entries for sonic-to-parameter translation:
- **warm** → Filter cutoff [0.2-0.4], resonance [0.1-0.2]
- **bright** → Filter cutoff [0.6-1.0]
- **lush** → Detune [0.05-0.15], chorus [0.3-0.5], reverb [0.3-0.6]
- **psychedelic** → Filter modulation, phaser, long reverb
- **ambient** → Long attack/release, heavy reverb
- **punchy** → Fast attack, filter envelope

### 🏪 Commercial Focus (NEW)

Phase 10: Market Preparation
- Market positioning strategies
- Pricing strategies
- Distribution channels
- Demo/limitation strategies

### 🔧 39 Failure Modes

Critical issues prevented:
- FM-01: Heap allocation on audio thread
- FM-04: Denormal CPU explosion
- FM-05: Feedback loop instability
- FM-09: SmoothedValue not advanced
- And 35 more...

### 📚 Knowledge Base

- **DSP KB**: Filters, oscillators, effects, dynamics
- **Sound Design KB**: Techniques, presets, bridges
- **UI KB**: Layouts, aesthetics, components
- **Failure Modes KB**: Audio thread, parameters, state
- **Platform KB**: Linux, macOS, Windows
- **Market KB**: Positioning, pricing, distribution
- **Testing KB**: Automated, manual, stress
- **C++ KB**: Real-time safety, patterns
- **JUCE KB**: Framework specifics

---

## Phase Overview

| Phase | Name | Purpose | Skills |
|-------|------|---------|--------|
| 0 | Plugin Concept | Define WHAT to build | juce-plugin-spec |
| 1 | Performance Audit | Audit existing code | juce-audio-thread-audit |
| 2 | Architecture | Turn spec to plan | writing-plans |
| 3 | Project Setup | Build infrastructure | - |
| 4 | DSP Implementation | Build audio engine | juce-dsp-implementation |
| 5 | State & Presets | State management | - |
| 6 | Integration | Wire modules | juce-smoothedvalue-audit |
| 7 | GUI | Build interface | juce-ui-bridge |
| 8 | Build Verification | Multi-platform | - |
| 9 | DAW Testing | Human testing | juce-daw-testing |
| 10 | **Market Prep** | Prepare for release | **NEW** |
| 11 | Optimization | Performance tuning | - |
| 12 | Release | Deploy and learn | - |

---

## Critical Rules

### Audio Thread Safety

**NEVER in processBlock**:
- `new`, `delete`, `malloc`, `free`
- `std::vector::push_back`, `std::string`
- `DBG()`, `std::cout`
- Locks, mutexes
- File I/O, network I/O

**ALWAYS in processBlock**:
- Pre-allocated buffers
- `std::atomic<bool>` flags
- `ScopedNoDenormals`
- `getNextValue()` per sample

### SmoothedValue Pattern

```cpp
// prepareToPlay
smoothedValue.reset(sampleRate, 0.02);
smoothedValue.setCurrentAndTargetValue(initialValue);

// processBlock (once per block)
smoothedValue.setTargetValue(newValue);

// per-sample loop
for (int i = 0; i < numSamples; ++i) {
    float value = smoothedValue.getNextValue();  // MUST call every sample
}
```

### Equal-Power Mixing

```cpp
// For dry/wet mixing
float dryGain = std::cos(mix * juce::MathConstants<float>::halfPi);
float wetGain = std::sin(mix * juce::MathConstants<float>::halfPi);
```

---

## Requirements Achieved

From the original commission brief:

| Requirement | Status |
|-------------|--------|
| VST3 plugin using JUCE framework | ✅ Covered |
| C++17, CMake 3.22+, JUCE 8.0.x, VST3 SDK 3.8.x | ✅ Specified |
| Advanced sound design techniques | ✅ KB with bridges |
| Psychedelic/ambient/creative sound design | ✅ Bridge entries |
| Modern UI design standards | ✅ UI KB |
| Marketing information and strategies | ✅ Phase 10, Market KB |
| Budget: $200/month AI tools | ✅ Constraints documented |
| Academic/professional source citations | ✅ Source hierarchy |

---

## Decisions Made

34 decisions recorded across all phases:

- **Phase 0 (8)**: Purpose, user, tech stack, budget, focus, commercial, platform, complexity
- **Phase 1 (6)**: Workflow, KB expansion, market, citations, state fix, cross-platform
- **Phase 2 (5)**: Bridged KB, directories, Market KB, sources, confidence
- **Phase 3 (5)**: KB structure, failure modes, bridges, placeholders, thresholds
- **Phase 4 (5)**: 13-phase workflow, Phase 10, two-stage review, human testing, cross-cutting
- **Phase 5 (5)**: Output structure, phase files, templates, KB integration, cross-cutting

---

## Next Steps

### To Use This Playbook

1. Copy the project structure to your workspace
2. Start with `templates/spec-template.md` for Phase 0
3. Use `phases/phase-*.md` for detailed guidance
4. Query `kb/` for specific knowledge

### To Extend This Playbook

1. Add more bridge entries in `kb/sound-design-kb/bridges/`
2. Harvest DSP algorithms into `kb/dsp-kb/`
3. Add market strategies to `kb/market-kb/`
4. Create remaining phase definition files

---

## License

This playbook is provided for educational and development purposes.

---

## Contact

For questions about this playbook, refer to the original commission brief or the decisions-ledger.md for decision rationale.