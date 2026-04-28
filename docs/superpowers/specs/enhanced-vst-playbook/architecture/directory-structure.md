# KB Directory Structure

> Defines the exact directory tree for the knowledge base with naming conventions.

---

## 1. Root Structure

```
kb/
├── master-index.json              # Master index of all KB entries
│
├── dsp-kb/                        # DSP algorithms and theory
│   ├── manifest.json
│   ├── filters/
│   ├── oscillators/
│   ├── effects/
│   ├── dynamics/
│   ├── modulation/
│   └── synthesis/
│
├── sound-design-kb/               # Sound design knowledge (bridged)
│   ├── manifest.json
│   ├── bridges/                   # Sonic → Parameter translations
│   ├── synthesis-techniques/
│   ├── ambient-textures/
│   ├── creative-effects/
│   ├── preset-templates/
│   └── sound-identity/
│
├── ui-kb/                         # UI/GUI design knowledge
│   ├── manifest.json
│   ├── layouts/
│   ├── aesthetics/
│   ├── components/
│   └── accessibility/
│
├── failure-modes-kb/              # Documented failure modes
│   ├── manifest.json
│   ├── audio-thread/
│   ├── parameters/
│   ├── state/
│   ├── memory/
│   └── platform/
│
├── platform-kb/                   # Platform-specific concerns
│   ├── manifest.json
│   ├── linux/
│   ├── macos/
│   ├── windows/
│   └── formats/
│
├── market-kb/                     # Commercial strategies (NEW)
│   ├── manifest.json
│   ├── positioning/
│   ├── pricing/
│   ├── distribution/
│   └── marketing/
│
├── testing-kb/                    # Validation and testing
│   ├── manifest.json
│   ├── automated/
│   ├── manual/
│   └── stress/
│
├── cpp-kb/                        # C++ patterns for audio
│   ├── manifest.json
│   ├── realtime-safety/
│   ├── patterns/
│   └── modern-cpp/
│
└── juce-kb/                       # JUCE framework specific
    ├── manifest.json
    ├── audio-processor/
    ├── gui/
    ├── dsp/
    └── utilities/
```

---

## 2. Directory Details

### 2.1 DSP KB

```
kb/dsp-kb/
├── manifest.json
├── filters/
│   ├── index.json
│   ├── lowpass.md
│   ├── highpass.md
│   ├── bandpass.md
│   ├── notch.md
│   ├── tpt-svf.md
│   ├── biquad.md
│   └── ladder.md
├── oscillators/
│   ├── index.json
│   ├── sawtooth.md
│   ├── square.md
│   ├── triangle.md
│   ├── sine.md
│   ├── wavetable.md
│   ├── noise.md
│   └── polyblep.md
├── effects/
│   ├── index.json
│   ├── delay/
│   │   ├── basic-delay.md
│   │   ├── feedback-delay.md
│   │   └── modulated-delay.md
│   ├── reverb/
│   │   ├── fdn-reverb.md
│   │   ├── convolution-reverb.md
│   │   └── algorithmic-reverb.md
│   ├── modulation/
│   │   ├── chorus.md
│   │   ├── flanger.md
│   │   ├── phaser.md
│   │   └── tremolo.md
│   └── distortion/
│       ├── waveshaping.md
│       ├── soft-clip.md
│       └── hard-clip.md
├── dynamics/
│   ├── index.json
│   ├── compressor.md
│   ├── limiter.md
│   ├── gate.md
│   └── expander.md
├── modulation/
│   ├── index.json
│   ├── lfo.md
│   ├── envelope.md
│   ├── mod-matrix.md
│   └── step-sequencer.md
└── synthesis/
    ├── index.json
    ├── subtractive.md
    ├── fm.md
    ├── additive.md
    ├── wavetable.md
    └── granular.md
```

### 2.2 Sound Design KB (Bridged)

```
kb/sound-design-kb/
├── manifest.json
├── bridges/
│   ├── index.json
│   ├── bridge-warm.json
│   ├── bridge-bright.json
│   ├── bridge-lush.json
│   ├── bridge-punchy.json
│   ├── bridge-evolving.json
│   ├── bridge-psychedelic.json
│   ├── bridge-ambient.json
│   ├── bridge-harsh.json
│   ├── bridge-gentle.json
│   └── bridge-aggressive.json
├── synthesis-techniques/
│   ├── index.json
│   ├── subtractive-basics.md
│   ├── fm-basics.md
│   ├── wavetable-basics.md
│   ├── additive-basics.md
│   ├── granular-basics.md
│   └── physical-modeling-basics.md
├── ambient-textures/
│   ├── index.json
│   ├── psychedelic-design.md
│   ├── pad-design.md
│   ├── evolving-textures.md
│   ├── atmospheric-pads.md
│   └── drone-creation.md
├── creative-effects/
│   ├── index.json
│   ├── feedback-networks.md
│   ├── granular-processing.md
│   ├── spectral-effects.md
│   └── modulation-chains.md
├── preset-templates/
│   ├── index.json
│   ├── warm-pad.json
│   ├── bright-lead.json
│   ├── psychedelic-ambient.json
│   ├── evolving-texture.json
│   └── bass-foundation.json
└── sound-identity/
    ├── index.json
    ├── describing-sound.md
    ├── parameter-mapping.md
    └── creative-workflow.md
```

### 2.3 UI KB

```
kb/ui-kb/
├── manifest.json
├── layouts/
│   ├── index.json
│   ├── synth-layout.md
│   ├── effect-layout.md
│   ├── analyzer-layout.md
│   └── compact-layout.md
├── aesthetics/
│   ├── index.json
│   ├── modern-minimal.md
│   ├── vintage-style.md
│   ├── dark-theme.md
│   ├── light-theme.md
│   └── color-theory.md
├── components/
│   ├── index.json
│   ├── knobs.md
│   ├── sliders.md
│   ├── buttons.md
│   ├── displays.md
│   ├── meters.md
│   └── labels.md
└── accessibility/
    ├── index.json
    ├── keyboard-nav.md
    ├── screen-readers.md
    └── color-contrast.md
```

### 2.4 Failure Modes KB

```
kb/failure-modes-kb/
├── manifest.json
├── audio-thread/
│   ├── index.json
│   ├── allocation.md
│   ├── locking.md
│   ├── blocking.md
│   └── denormals.md
├── parameters/
│   ├── index.json
│   ├── smoothing.md
│   ├── automation.md
│   ├── thread-safety.md
│   └── ids.md
├── state/
│   ├── index.json
│   ├── serialization.md
│   ├── presets.md
│   └── automation-state.md
├── memory/
│   ├── index.json
│   ├── leaks.md
│   ├── buffer-overflow.md
│   └── instance-isolation.md
└── platform/
    ├── index.json
    ├── macos-au.md
    ├── windows-vst3.md
    ├── linux-vst3.md
    └── cross-platform.md
```

### 2.5 Platform KB

```
kb/platform-kb/
├── manifest.json
├── linux/
│   ├── index.json
│   ├── dependencies.md
│   ├── build-setup.md
│   ├── vst3-path.md
│   └── common-issues.md
├── macos/
│   ├── index.json
│   ├── code-signing.md
│   ├── notarization.md
│   ├── au-path.md
│   ├── vst3-path.md
│   └── common-issues.md
├── windows/
│   ├── index.json
│   ├── msvc-runtime.md
│   ├── vst3-path.md
│   └── common-issues.md
└── formats/
    ├── index.json
    ├── vst3.md
    ├── au.md
    ├── aax.md
    ├── clap.md
    └── standalone.md
```

### 2.6 Market KB (NEW)

```
kb/market-kb/
├── manifest.json
├── positioning/
│   ├── index.json
│   ├── market-analysis.md
│   ├── target-audience.md
│   ├── competitive-analysis.md
│   └── unique-value.md
├── pricing/
│   ├── index.json
│   ├── pricing-strategies.md
│   ├── intro-offers.md
│   ├── bundle-strategies.md
│   └── update-pricing.md
├── distribution/
│   ├── index.json
│   ├── platforms.md
│   ├── github-releases.md
│   ├── plugin-stores.md
│   ├── direct-sales.md
│   └── demo-versions.md
└── marketing/
    ├── index.json
    ├── website.md
    ├── demo-videos.md
    ├── presets-showcase.md
    ├── community.md
    └── support.md
```

### 2.7 Testing KB

```
kb/testing-kb/
├── manifest.json
├── automated/
│   ├── index.json
│   ├── pluginval.md
│   ├── unit-tests.md
│   └── ci-cd.md
├── manual/
│   ├── index.json
│   ├── daw-testing.md
│   ├── preset-audition.md
│   ├── automation-test.md
│   └── user-testing.md
└── stress/
    ├── index.json
    ├── sample-rate.md
    ├── buffer-size.md
    ├── multi-instance.md
    └── long-duration.md
```

### 2.8 C++ KB

```
kb/cpp-kb/
├── manifest.json
├── realtime-safety/
│   ├── index.json
│   ├── atomic-operations.md
│   ├── lock-free-patterns.md
│   ├── no-allocations.md
│   └── pitfalls.md
├── patterns/
│   ├── index.json
│   ├── smoothed-value.md
│   ├── apvts-patterns.md
│   ├── listener-patterns.md
│   └── singleton-avoidance.md
└── modern-cpp/
    ├── index.json
    ├── smart-pointers.md
    ├── move-semantics.md
    └── constexpr.md
```

### 2.9 JUCE KB

```
kb/juce-kb/
├── manifest.json
├── audio-processor/
│   ├── index.json
│   ├── processblock.md
│   ├── preparetoplay.md
│   ├── state-management.md
│   └── parameters.md
├── gui/
│   ├── index.json
│   ├── components.md
│   ├── lookandfeel.md
│   ├── responsiveness.md
│   └── graphics.md
├── dsp/
│   ├── index.json
│   ├── dsp-module.md
│   ├── oversampling.md
│   └── smoothing.md
└── utilities/
    ├── index.json
    ├── logger.md
    ├── thread.md
    └── timer.md
```

---

## 3. Naming Conventions

### Directory Names

| Convention | Example |
|------------|---------|
| Lowercase | `dsp-kb/`, `filters/` |
| Kebab-case | `realtime-safety/`, `audio-thread/` |
| No spaces | `sound-design-kb/` (not `sound design kb/`) |
| No special chars | `waveshaping/` (not `waveshaping!/`) |

### File Names

| Type | Convention | Example |
|------|------------|---------|
| Markdown entry | `kebab-case.md` | `tpt-svf.md` |
| JSON bridge | `bridge-kebab.json` | `bridge-warm.json` |
| JSON index | `index.json` or `manifest.json` | `manifest.json` |
| Template | `template-name.json` | `warm-pad.json` |

### Entry IDs

| Convention | Example |
|------------|---------|
| Prefixed by category | `dsp-tpt-svf` |
| Kebab-case | `bridge-psychedelic-ambient` |
| No numbers at start | `fm-basics` (not `1-fm-basics`) |
| Descriptive | `failure-allocation-audio-thread` |

---

## 4. File Format Standards

### Markdown Entries (.md)

```markdown
# [Title]

> [One-line summary]

## Description

[Detailed description]

## Algorithm

[Mathematical formulas and explanations]

## Implementation

```cpp
// Code example
```

## Parameters

| Parameter | Range | Description |
|-----------|-------|-------------|
| ... | ... | ... |

## Sources

- [Source name](url)

## See Also

- [Related entry 1]
- [Related entry 2]

## Warnings

- [Warning 1]
- [Warning 2]
```

### JSON Entries (.json)

```json
{
  "id": "unique-id",
  "title": "Title",
  "category": "category",
  "tags": ["tag1", "tag2"],
  "description": "...",
  "confidence": 0.85,
  "source": { ... },
  "see_also": ["related-id-1", "related-id-2"]
}
```

### Manifest Files (manifest.json)

```json
{
  "kb_name": "dsp-kb",
  "version": "1.0.0",
  "updated": "2026-04-02",
  "entry_count": 45,
  "categories": {
    "filters": { "count": 7, "entries": [...] },
    "oscillators": { "count": 7, "entries": [...] }
  }
}
```

### Index Files (index.json)

```json
{
  "category": "filters",
  "description": "Filter algorithms and implementations",
  "entries": [
    { "id": "dsp-lowpass", "title": "Lowpass Filter" },
    { "id": "dsp-tpt-svf", "title": "TPT State Variable Filter" }
  ]
}
```

---

## 5. KB Integration Points

### Phase → KB Lookup

| Phase | KB Lookups |
|-------|------------|
| 0 | sound-design-kb (bridges), market-kb |
| 1 | failure-modes-kb, platform-kb |
| 4 | dsp-kb, sound-design-kb, cpp-kb |
| 5 | sound-design-kb (presets), preset-templates |
| 6 | dsp-kb, failure-modes-kb |
| 7 | ui-kb, market-kb |
| 8 | platform-kb, testing-kb |
| 9 | sound-design-kb (bridges) |
| 10 | market-kb |
| 11 | dsp-kb, cpp-kb |

---

## 6. Migration from Existing KB

### Source: /home/myuser/agents/juce-agent/playbookdata/

| Source Directory | Target Directory | Action |
|-----------------|------------------|--------|
| `dsp-kb/` | `kb/dsp-kb/` | Migrate and enhance |
| `sound-design-kb/` | `kb/sound-design-kb/` | Migrate and enhance |
| `ui-kb/` | `kb/ui-kb/` | Migrate and enhance |
| `cpp-kb/` | `kb/cpp-kb/` | Migrate |
| `testing-kb/` | `kb/testing-kb/` | Migrate |
| `references/` | `kb/failure-modes-kb/`, `kb/juce-kb/` | Split and migrate |

### New KBs

| KB | Created In | Populated In |
|----|------------|--------------|
| `market-kb/` | Phase 2 | Phase 3 |
| `platform-kb/` | Phase 2 | Phase 3 |
| Enhanced bridges | Phase 2 | Phase 3 |