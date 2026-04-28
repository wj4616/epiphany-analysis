# Knowledge Base Architecture

## Overview

The PBCPB Knowledge Base uses a **5-layer structured architecture** with a **bridge layer** connecting Sound Design vocabulary to Technical parameters. This design supports the commission brief's requirement for "informed implementations at every phase."

## Layer Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     REFERENCE KB (Layer 5)                      │
│   Academic sources, textbooks, expert tutorials indexed         │
├─────────────────────────────────────────────────────────────────┤
│                    COMMERCIAL KB (Layer 4)                       │
│   Licensing systems, marketplaces, pricing, marketing            │
├─────────────────────────────────────────────────────────────────┤
│                      UI/UX KB (Layer 3)                          │
│   Control design, layout theory, color/typography, LookAndFeel  │
├─────────────────────────────────────────────────────────────────┤
│                   SOUND DESIGN KB (Layer 2)                      │
│   Synthesis theory, effect theory, genre techniques              │
├─────────────────────────────────────────────────────────────────┤
│                     TECHNICAL KB (Layer 1)                       │
│   C++17 patterns, JUCE framework, DSP, audio thread safety       │
└─────────────────────────────────────────────────────────────────┘

                    ┌───────────────────┐
                    │   BRIDGE LAYER    │
                    │ Descriptor → DSP  │
                    └───────────────────┘
```

## Layer 1: Technical KB

### Directory Structure

```
kb/technical/
├── cpp-patterns/
│   ├── audio-thread-safety.md
│   ├── memory-management.md
│   ├── raii-patterns.md
│   └── modern-cpp17.md
├── juce-framework/
│   ├── audioprocessor.md
│   ├── apvts.md
│   ├── smoothedvalue.md
│   ├── gui-components.md
│   └── state-management.md
├── build-system/
│   ├── cmake-configuration.md
│   ├── fetchcontent-setup.md
│   └── cross-platform.md
├── dsp/
│   ├── oscillators.md
│   ├── filters.md
│   ├── effects.md
│   ├── dynamics.md
│   └── modulation.md
└── testing/
    ├── catch2-setup.md
    ├── unit-testing.md
    ├── pluginval.md
    └── profiling.md
```

### Entry Schema

```json
{
  "id": "TECH-001",
  "title": "Audio Thread Safety",
  "category": "cpp-patterns",
  "summary": "Rules for real-time audio processing",
  "content": "# Audio Thread Safety\n...",
  "code_examples": ["..."],
  "failure_modes": ["FM-01", "FM-02"],
  "related_entries": ["TECH-002", "TECH-005"],
  "sources": ["ross-bencina-realtime"]
}
```

### Content Requirements

| Category | Minimum Entries | Content Focus |
|----------|-----------------|---------------|
| cpp-patterns | 5 | Audio-relevant C++17, thread safety |
| juce-framework | 10 | Core JUCE classes, patterns |
| build-system | 4 | CMake, FetchContent |
| dsp | 15 | Algorithm references, implementation |
| testing | 5 | Test frameworks, procedures |

## Layer 2: Sound Design KB

### Directory Structure

```
kb/sound-design/
├── synthesis/
│   ├── subtractive.md
│   ├── fm-synthesis.md
│   ├── wavetable.md
│   ├── granular.md
│   └── additive.md
├── effects/
│   ├── reverb.md
│   ├── delay.md
│   ├── distortion.md
│   ├── modulation.md
│   └── dynamics.md
├── genres/
│   ├── psychedelic.md
│   ├── ambient.md
│   ├── psybient.md
│   └── experimental.md
├── patches/
│   ├── pad-creation.md
│   ├── lead-design.md
│   ├── bass-synthesis.md
│   └── texture-design.md
└── descriptors/
    ├── warm.md
    ├── bright.md
    ├── lush.md
    ├── punchy.md
    └── ethereal.md
```

### Entry Schema

```json
{
  "id": "SD-001",
  "title": "Subtractive Synthesis",
  "category": "synthesis",
  "summary": "Filter-based sound shaping",
  "content": "# Subtractive Synthesis\n...",
  "dsp_translations": {
    "oscillator_type": ["saw", "square", "pwm"],
    "filter_type": ["lowpass", "highpass", "bandpass"]
  },
  "genre_applications": ["psychedelic", "ambient"],
  "related_entries": ["SD-002", "BRIDGE-001"]
}
```

### Content Requirements

| Category | Minimum Entries | Content Focus |
|----------|-----------------|---------------|
| synthesis | 5 | Core synthesis types |
| effects | 5 | Effect categories |
| genres | 4 | Psychedelic, ambient, psybient, experimental |
| patches | 4 | Patch creation methodology |
| descriptors | 10+ | Subjective sonic vocabulary |

## Layer 3: UI/UX KB

### Directory Structure

```
kb/ui-ux/
├── controls/
│   ├── knobs.md
│   ├── sliders.md
│   ├── buttons.md
│   ├── displays.md
│   └── graphs.md
├── layout/
│   ├── grouping.md
│   ├── signal-flow.md
│   ├── hierarchy.md
│   └── responsive.md
├── visual/
│   ├── color-theory.md
│   ├── typography.md
│   ├── themes.md
│   └── accessibility.md
├── juce-implementation/
│   ├── lookandfeel.md
│   ├── custom-components.md
│   └── animations.md
└── examples/
    ├── synth-layout.md
    ├── effect-layout.md
    └── modern-design.md
```

### Entry Schema

```json
{
  "id": "UI-001",
  "title": "Knob Design",
  "category": "controls",
  "summary": "Rotary control design patterns",
  "content": "# Knob Design\n...",
  "dimensions": {"large": "80-100px", "medium": "50-70px"},
  "interaction_patterns": ["drag-vertical", "double-click-reset"],
  "code_example": "..."
}
```

### Content Requirements

| Category | Minimum Entries | Content Focus |
|----------|-----------------|---------------|
| controls | 5 | Knob, slider, button patterns |
| layout | 4 | Grouping, flow, hierarchy |
| visual | 4 | Color, typography, themes |
| juce-implementation | 3 | LookAndFeel, custom components |

## Layer 4: Commercial KB

### Directory Structure

```
kb/commercial/
├── licensing/
│   ├── ilok.md
│   ├── musehub.md
│   ├── custom.md
│   └── implementation.md
├── marketplaces/
│   ├── kvr-audio.md
│   ├── plugin-boutique.md
│   ├── direct-sales.md
│   └── listing-process.md
├── pricing/
│   ├── strategies.md
│   ├── tiers.md
│   └── sales-cycles.md
├── marketing/
│   ├── pre-launch.md
│   ├── launch.md
│   ├── post-launch.md
│   └── content-strategy.md
└── business/
    ├── business-plan.md
    ├── projections.md
    └── legal.md
```

### Entry Schema

```json
{
  "id": "COM-001",
  "title": "KVR Audio Marketplace",
  "category": "marketplaces",
  "summary": "Product database and marketplace strategy",
  "content": "# KVR Audio\n...",
  "requirements": ["database entry", "manufacturer account"],
  "fee_structure": "dealer margin covers fees",
  "url": "https://www.kvraudio.com/marketplace/"
}
```

### Content Requirements

| Category | Minimum Entries | Content Focus |
|----------|-----------------|---------------|
| licensing | 4 | iLok, MuseHub, custom, implementation |
| marketplaces | 4 | KVR, Plugin Boutique, direct sales |
| pricing | 3 | Strategies, tiers, sales |
| marketing | 4 | Pre-launch, launch, post-launch, content |
| business | 3 | Business plan, projections, legal |

## Layer 5: Reference KB

### Directory Structure

```
kb/reference/
├── academic/
│   ├── dsp-papers.md
│   ├── audio-research.md
│   └── psychoacoustics.md
├── textbooks/
│   ├── pirkle.md
│   ├── zolzer.md
│   └── smith.md
├── tutorials/
│   ├── juce-official.md
│   ├── community-tutorials.md
│   └── video-courses.md
└── tools/
    ├── juce-documentation.md
    ├── vst3-sdk.md
    └── cmake-docs.md
```

### Entry Schema

```json
{
  "id": "REF-001",
  "title": "Designing Audio Effect Plugins (Pirkle)",
  "category": "textbooks",
  "summary": "Comprehensive DSP plugin development guide",
  "content": "# Pirkle Textbook\n...",
  "topics_covered": ["filters", "dynamics", "reverb"],
  "isbn": "978-1138591936",
  "recommendation_level": "essential"
}
```

### Content Requirements

| Category | Minimum Entries | Content Focus |
|----------|-----------------|---------------|
| academic | 3 | Key DSP papers, research |
| textbooks | 3 | Pirkle, Zölzer, Smith |
| tutorials | 3 | JUCE official, community, video |
| tools | 3 | JUCE docs, VST3 SDK, CMake |

## Bridge Layer: Sound Design → Technical

### Purpose

Translate subjective sonic descriptors into specific DSP parameter ranges. Enables AI to implement "warm pad" without human specifying exact values.

### Schema

```json
{
  "id": "BRIDGE-001",
  "descriptor": "warm",
  "category": "timbre",
  "translations": {
    "filter_cutoff": {
      "range": [0.2, 0.4],
      "normalized": true,
      "notes": "Lower cutoff removes harsh high frequencies"
    },
    "filter_resonance": {
      "range": [0.05, 0.15],
      "normalized": true,
      "notes": "Subtle resonance adds character without harshness"
    },
    "saturation_type": {
      "value": "soft_clip",
      "notes": "Soft clipping adds even-order harmonics"
    },
    "saturation_amount": {
      "range": [0.1, 0.3],
      "normalized": true,
      "notes": "Subtle saturation for warmth"
    }
  },
  "genre_affinity": ["ambient", "psybient"],
  "conflicts_with": ["bright", "aggressive"],
  "examples": ["Analog pad", "Vinyl warmth"]
}
```

### Required Translations

| Descriptor | Primary Parameters | Secondary Parameters |
|------------|-------------------|---------------------|
| warm | filter_cutoff ↓, saturation | resonance ↓ |
| bright | filter_cutoff ↑, presence | high shelf |
| lush | detune, chorus, reverb | slow modulation |
| punchy | fast attack, filter env | transient emphasis |
| ethereal | long reverb, high-pass, modulation | slow LFO |
| aggressive | distortion ↑, odd harmonics | fast release |
| deep | low emphasis, slow mod | sub content |
| crisp | high-mid boost, fast attack | compression |
| dark | low-pass, low emphasis | reduced presence |
| glassy | FM, high harmonic content | bright filter |

### Conflict Resolution

| Conflict | Resolution |
|----------|------------|
| warm + bright | Split: warm in lows, bright in highs (presence) |
| aggressive + ethereal | Unusual but valid: distorted reverb |
| deep + punchy | Conflicting: prioritize one or use different bands |

## KB Integration Points

### Phase → KB Population

| Phase | KB Activity |
|-------|-------------|
| Phase 0 | Read: Sound Design descriptors |
| Phase 2 | Create: KB directory structure |
| Phase 3 | Populate: Technical KB placeholders |
| Phase 4 | Populate: DSP entries, Bridge mappings |
| Phase 7 | Populate: UI/UX entries |
| Phase 13-16 | Populate: Commercial entries |

### KB → Implementation

| Implementation Task | KB Source |
|--------------------|-----------|
| DSP algorithm selection | Technical KB + Sound Design KB |
| Parameter ranges | Bridge Layer |
| UI layout | UI/UX KB |
| Licensing choice | Commercial KB |
| Marketplace listing | Commercial KB |

## Validation Requirements

### KB Completeness Check

Each layer must have:
- [ ] Minimum entries per category populated
- [ ] All entry schema fields valid
- [ ] Cross-references resolve correctly
- [ ] Bridge layer covers all descriptor terms

### Quality Metrics

| Metric | Target |
|--------|--------|
| Technical KB entries | 39+ |
| Sound Design KB entries | 28+ |
| UI/UX KB entries | 16+ |
| Commercial KB entries | 18+ |
| Reference KB entries | 12+ |
| Bridge translations | 10+ |
| Total entries | 123+ |

## Sources Index

All KB entries must cite sources:
- Technical: JUCE docs, Ross Bencina, Pirkle
- Sound Design: Artists in DSP, genre resources
- UI/UX: Audio-UI, Voger Design, JUCE tutorials
- Commercial: KVR, Plugin Boutique, market reports
- Reference: Academic papers, textbooks