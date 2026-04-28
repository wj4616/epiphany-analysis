# Knowledge Base Architecture

> Defines the structure, layers, and integration of the knowledge base for the enhanced VST playbook.

---

## 1. Architecture Overview

### Complexity Classification

| Dimension | Level | Justification |
|-----------|-------|---------------|
| **Process** | Standard | 13-phase workflow exists, proven structure |
| **Knowledge** | **Bridged** | DSP requires translation layer; sound design needs expert mapping |
| **Roles** | Complex | Multiple perspectives: developer, sound designer, marketer |
| **Overall** | Complex | Multi-dimensional expertise required |

### Architecture Type: Bridged

The KB uses a **bridged architecture** with translation layers between domains:

```
┌─────────────────────────────────────────────────────────────────┐
│                        PLAYBOOK                                 │
│  (Phases reference KB entries via lookup)                      │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    KNOWLEDGE BASE                                │
│                                                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   DSP KB     │  │ Sound Design │  │   UI KB      │          │
│  │  (Technical) │  │   KB         │  │  (Visual)    │          │
│  │              │  │              │  │              │          │
│  │ - Filters    │  │ - Psychedelic│  │ - Layouts    │          │
│  │ - Oscillators│  │ - Ambient    │  │ - Colors     │          │
│  │ - Effects    │  │ - Creative   │  │ - Components │          │
│  │ - Algorithms │  │ - Presets    │  │ - Animation  │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│                              │                                  │
│                              ▼                                  │
│                    ┌──────────────┐                           │
│                    │ BRIDGE ENTRIES│                           │
│                    │ (Translation) │                           │
│                    │              │                           │
│                    │ "warm" → [    │                           │
│                    │   filter: {  │                           │
│                    │     cutoff:  │                           │
│                    │       [0.2,  │                           │
│                    │        0.4],  │                           │
│                    │     resonance│                           │
│                    │       0.15    │                           │
│                    │   }          │                           │
│                    │ ]            │                           │
│                    └──────────────┘                           │
│                                                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │ Failure Modes│  │ Platform KB  │  │  Market KB  │          │
│  │   KB         │  │              │  │  (New)      │          │
│  │              │  │ - Linux      │  │              │          │
│  │ - FM-01...   │  │ - macOS      │  │ - Positioning│          │
│  │ - EM-01...   │  │ - Windows    │  │ - Pricing    │          │
│  │ - CM-01...   │  │ - VST3       │  │ - Distribution│          │
│  │              │  │ - AU         │  │ - Marketing  │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 2. KB Layer Structure

### Layer 0: Reference KB (Flat)

Technical reference entries with no translation needed.

| Subdirectory | Purpose | Entry Count (Est.) |
|--------------|---------|---------------------|
| `dsp-kb/` | DSP algorithms, filters, oscillators | 50-100 |
| `cpp-kb/` | C++ patterns, real-time safety | 30-50 |
| `failure-modes-kb/` | Documented failure modes | 50-100 |
| `platform-kb/` | Platform-specific concerns | 20-30 |
| `testing-kb/` | Validation patterns | 20-30 |

### Layer 1: Design KB (Bridged)

Design knowledge requiring translation between domains.

| Subdirectory | Purpose | Bridge Required | Entry Count (Est.) |
|--------------|---------|-----------------|---------------------|
| `sound-design-kb/` | Synthesis techniques, presets | **Yes** | 100-200 |
| `ui-kb/` | Interface design, aesthetics | Optional | 30-50 |
| `market-kb/` | Commercial strategies | No | 20-40 |

### Layer 2: Integration KB

Knowledge connecting multiple domains.

| Subdirectory | Purpose | Entry Count (Est.) |
|--------------|---------|---------------------|
| `presets-kb/` | Complete preset templates | 50-100 |
| `workflows-kb/` | Multi-phase workflows | 10-20 |

---

## 3. Bridge Schema Purpose

### Why Bridged Architecture?

The **sound design KB** requires translation between:

| Domain | Type | Example |
|--------|------|---------|
| **Sonic** | Qualitative | "warm, lush, atmospheric" |
| **Technical** | Quantitative | `filter_cutoff: 0.3, resonance: 0.15` |

### Bridge Entry Structure

```json
{
  "id": "bridge-psychedelic-ambient",
  "sonic_description": "swirling, evolving, psychedelic ambient texture",
  "translation": {
    "filter": {
      "cutoff_range": [0.2, 0.5],
      "resonance_range": [0.1, 0.3],
      "modulation": {
        "source": "lfo",
        "depth": [0.1, 0.3],
        "rate_range": [0.05, 0.2]
      }
    },
    "reverb": {
      "mix_range": [0.4, 0.8],
      "decay_range": [2.0, 8.0],
      "modulation": "chorus_after_reverb"
    }
  },
  "confidence": 0.85,
  "source": "expert_synthesist_interview",
  "why": "Filters in mid-range with modulation create evolving textures..."
}
```

### When Bridge Is Used

| Phase | Bridge Usage |
|-------|--------------|
| Phase 0 (Spec) | Translate sonic descriptions to parameter ranges |
| Phase 4 (DSP) | Look up algorithms for specified ranges |
| Phase 5 (Presets) | Generate presets from sonic descriptions |
| Phase 9 (DAW Testing) | Validate sonic descriptions match output |

---

## 4. KB-to-Phase Integration

### Phase Dependencies on KB

| Phase | KB Dependencies | Lookup Type |
|-------|-----------------|-------------|
| **Phase 0** | Sound Design KB (bridged), Market KB | Bridge lookup |
| **Phase 1** | Failure Modes KB, Platform KB | Direct lookup |
| **Phase 2** | All KBs | Architecture reference |
| **Phase 3** | None (project setup) | N/A |
| **Phase 4** | DSP KB, C++ KB, Sound Design KB (bridged) | Mixed lookup |
| **Phase 5** | Presets KB, Sound Design KB | Direct + Bridge |
| **Phase 6** | DSP KB, Failure Modes KB | Direct lookup |
| **Phase 7** | UI KB, Market KB (for aesthetics) | Mixed lookup |
| **Phase 8** | Platform KB, Testing KB | Direct lookup |
| **Phase 9** | Sound Design KB (bridged) | Bridge lookup |
| **Phase 10** | Market KB | Direct lookup |
| **Phase 11** | DSP KB, C++ KB | Direct lookup |
| **Phase 12** | None (retrospective) | N/A |

---

## 5. Entry Schema Overview

### Base Entry Schema (All KBs)

```json
{
  "id": "unique-entry-id",
  "title": "Human-readable title",
  "category": "category-name",
  "tags": ["tag1", "tag2"],
  "created": "2026-04-02",
  "updated": "2026-04-02",
  "confidence": 1.0,
  "source": "source-reference"
}
```

### DSP Entry Schema (Extended)

```json
{
  "id": "dsp-tpt-svf",
  "title": "Topology-Preserving State Variable Filter",
  "category": "filters",
  "tags": ["filter", "lowpass", "highpass", "bandpass", "modulatable"],
  "algorithm": {
    "description": "State variable filter with stable modulation",
    "math": "y_lp = y_lp + g * (input - y_hp); y_hp = input - y_lp - r * y_bp; y_bp = y_bp + g * y_hp",
    "coefficients": {
      "g": "tan(π * cutoff / sampleRate)",
      "r": "1 / (2 * Q)"
    }
  },
  "implementation": {
    "language": "C++",
    "juce_class": "dsp::StateVariableFilter",
    "sample_usage": "..."
  },
  "source": {
    "author": "Andy Simper (Cytomic)",
    "paper": "The Art of VA Filter Design",
    "url": "https://cytomic.com/"
  },
  "confidence": 1.0
}
```

### Sound Design Entry Schema (Extended)

```json
{
  "id": "sd-psychedelic-ambient",
  "title": "Psychedelic Ambient Texture Design",
  "category": "ambient-textures",
  "tags": ["psychedelic", "ambient", "evolving", "texture"],
  "description": "Creating swirling, evolving psychedelic ambient textures",
  "techniques": [
    {
      "name": "Filter Modulation",
      "description": "Slow LFO modulating filter cutoff in mid-range",
      "parameters": {
        "filter_cutoff_range": [0.2, 0.5],
        "lfo_rate_range": [0.05, 0.2],
        "lfo_depth_range": [0.1, 0.3]
      }
    }
  ],
  "presets": ["preset-swirl-01", "preset-evolve-02"],
  "source": {
    "type": "expert_synthesis",
    "reference": "..."
  },
  "confidence": 0.85
}
```

### Market Entry Schema (New)

```json
{
  "id": "market-pricing-strategies",
  "title": "VST Plugin Pricing Strategies",
  "category": "pricing",
  "tags": ["pricing", "commercial", "strategy"],
  "strategies": [
    {
      "name": "Tiered Pricing",
      "description": "Multiple price points for different features",
      "pros": ["Maximizes revenue", "Appeals to different segments"],
      "cons": ["Complex implementation", "Feature management"]
    }
  ],
  "examples": [
    {
      "plugin": "X Plugin",
      "price": "$49 intro / $99 full",
      "strategy": "introductory"
    }
  ],
  "source": {
    "type": "market_research",
    "reference": "..."
  },
  "confidence": 0.8
}
```

---

## 6. KB Source Hierarchy

### Source Quality Levels

| Level | Source Type | Confidence | Example |
|-------|-------------|------------|---------|
| **1** | Academic Paper | 1.0 | Julius Smith papers, AES papers |
| **2** | Professional Reference | 0.95 | Will Pirkle books, Cytomic papers |
| **3** | Expert Interview | 0.85 | Sound designer interviews |
| **4** | Community Best Practice | 0.75 | JUCE forum, melatonin.dev |
| **5** | Tutorial/Guide | 0.65 | YouTube tutorials, blog posts |
| **6** | Forum Discussion | 0.50 | KVR, DSP Stack Exchange |

### Citation Required

All entries must include source citation:

```json
"source": {
  "type": "academic_paper",
  "author": "Julius O. Smith III",
  "title": "Physical Audio Signal Processing",
  "url": "https://ccrma.stanford.edu/~jos/pasp/",
  "accessed": "2026-04-02"
}
```

---

## 7. KB Directory Structure

```
kb/
├── master-index.json          # Index of all KB entries
├── dsp-kb/                    # Layer 0: Technical DSP
│   ├── manifest.json
│   ├── filters/
│   │   ├── lowpass.md
│   │   ├── highpass.md
│   │   ├── bandpass.md
│   │   └── tpt-svf.md
│   ├── oscillators/
│   │   ├── sawtooth.md
│   │   ├── square.md
│   │   ├── wavetable.md
│   │   └── polyblep.md
│   ├── effects/
│   │   ├── delay.md
│   │   ├── reverb-fdn.md
│   │   ├── distortion.md
│   │   └── chorus.md
│   └── dynamics/
│       ├── compressor.md
│       └── limiter.md
├── sound-design-kb/           # Layer 1: Bridged
│   ├── manifest.json
│   ├── bridges/               # Translation entries
│   │   ├── bridge-psychedelic.json
│   │   ├── bridge-ambient.json
│   │   ├── bridge-warm.json
│   │   └── bridge-bright.json
│   ├── ambient-textures/
│   │   ├── psychedelic-design.md
│   │   ├── pad-design.md
│   │   └── evolving-textures.md
│   ├── synthesis-techniques/
│   │   ├── subtractive.md
│   │   ├── fm.md
│   │   ├── wavetable.md
│   │   └── granular.md
│   └── preset-templates/
│       ├── warm-pad.md
│       ├── bright-lead.md
│       └── psychedelic-ambient.md
├── ui-kb/                     # Layer 1: Design
│   ├── manifest.json
│   ├── layouts/
│   │   ├── synth-layout.md
│   │   └── effect-layout.md
│   ├── aesthetics/
│   │   ├── modern-minimal.md
│   │   └── vintage-style.md
│   └── components/
│       ├── knobs.md
│       ├── sliders.md
│       └── displays.md
├── failure-modes-kb/          # Layer 0: Technical
│   ├── manifest.json
│   ├── audio-thread/
│   │   ├── allocation.md
│   │   └── locking.md
│   ├── parameters/
│   │   ├── smoothing.md
│   │   └── automation.md
│   └── state/
│       ├── serialization.md
│       └── presets.md
├── platform-kb/               # Layer 0: Technical
│   ├── manifest.json
│   ├── linux.md
│   ├── macos.md
│   ├── windows.md
│   └── formats/
│       ├── vst3.md
│       ├── au.md
│       └── aax.md
├── market-kb/                 # Layer 1: Strategy
│   ├── manifest.json
│   ├── positioning.md
│   ├── pricing.md
│   ├── distribution.md
│   └── marketing.md
├── testing-kb/                # Layer 0: Technical
│   ├── manifest.json
│   ├── pluginval.md
│   ├── stress-tests.md
│   └── validation.md
└── cpp-kb/                    # Layer 0: Technical
    ├── manifest.json
    ├── realtime-safety/
    │   ├── atomic-operations.md
    │   └── lock-free-patterns.md
    └── patterns/
        ├── smoothed-value.md
        └── apvts-patterns.md
```

---

## 8. Population Strategy

### Initial Population (Phase 3)

| KB | Source | Method |
|----|--------|--------|
| DSP KB | Existing playbook | Migrate from v7 |
| Failure Modes KB | Existing playbook | Migrate FM-01 to FM-23 |
| C++ KB | Existing playbook | Migrate cpp-kb |
| Platform KB | Research docs | Harvest from Phase 1 |
| Sound Design KB | Expert research | Create new entries |
| UI KB | Existing + research | Enhance existing |
| Market KB | Research | Create new entries |
| Testing KB | Existing playbook | Migrate from v7 |

### Ongoing Population

| Method | When | Source |
|--------|------|--------|
| **Harvesting** | New topic identified | Academic papers, expert interviews |
| **Curation** | Entry quality review | Existing entries |
| **Bridge Creation** | Sonic term encountered | Expert synthesists |
| **Validation** | Before Phase 4 | Confidence scoring |

---

## 9. Validation Framework

### Entry Confidence Scoring

| Score | Meaning | Action |
|-------|---------|--------|
| 1.0 | Verified, academic source | Use directly |
| 0.85 | Expert interview, verified | Use with note |
| 0.75 | Community best practice | Use, verify if critical |
| 0.50 | Forum discussion | Verify before use |
| < 0.50 | Unverified | Do not use |

### KB Health Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| Entry count | 200-500 | Automated count |
| Bridge coverage | 80% sonic terms | Audit |
| Source citation | 100% entries | Validation |
| Confidence avg | > 0.75 | Average score |

---

## 10. Next Steps

Phase 3 will:
1. Create the KB directory structure
2. Seed placeholder entries
3. Harvest initial content from research
4. Create bridge entries for sound design terms
5. Validate entry confidence scores