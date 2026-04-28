# KB Directory Structure

## Overview

This document defines the exact directory tree, file naming conventions, and index formats for the Knowledge Base.

---

## Directory Tree

```
kb/
├── master-index.json           # Registry of all KBs, cross-layer mappings
├── technical-kb/
│   ├── manifest.json           # File listing, status, sync history
│   ├── cpp-audio/
│   │   ├── _archive/           # Superseded entries
│   │   ├── real-time-safety.json
│   │   └── memory-management.json
│   ├── juce-patterns/
│   │   ├── _archive/
│   │   ├── apvts-setup.json
│   │   ├── smoothedvalue.json
│   │   └── state-save-restore.json
│   ├── dsp-algorithms/
│   │   ├── _archive/
│   │   ├── filters.json
│   │   ├── oscillators.json
│   │   └── dynamics.json
│   ├── audio-thread-safety/
│   │   ├── _archive/
│   │   ├── allocations.json
│   │   ├── locking.json
│   │   └── denormals.json
│   ├── cmake-build/
│   │   ├── _archive/
│   │   ├── fetchcontent-setup.json
│   │   └── ci-cd.json
│   ├── vst3-sdk/
│   │   ├── _archive/
│   │   ├── plugin-format.json
│   │   └── state-persistence.json
│   ├── parameter-management/
│   │   ├── _archive/
│   │   └── parameter-layout.json
│   └── state-persistence/
│       ├── _archive/
│       └── apvts-state.json
├── sound-design-kb/
│   ├── manifest.json
│   ├── bridges/                 # Bridge entries (Sound Design → Technical)
│   │   ├── bridge-warm.json
│   │   ├── bridge-bright.json
│   │   ├── bridge-lush.json
│   │   ├── bridge-punchy.json
│   │   ├── bridge-psychedelic.json
│   │   ├── bridge-ambient.json
│   │   └── bridge-dark.json
│   ├── synthesis-theory/
│   │   ├── _archive/
│   │   ├── subtractive.json
│   │   ├── fm.json
│   │   ├── wavetable.json
│   │   └── granular.json
│   ├── oscillator-design/
│   │   └── ...
│   ├── filter-design/
│   │   └── ...
│   ├── modulation/
│   │   └── ...
│   ├── effects/
│   │   └── ...
│   ├── psychedelic/
│   │   ├── _archive/
│   │   └── techniques.json
│   ├── ambient/
│   │   └── ...
│   ├── pad/
│   │   └── ...
│   ├── creative/
│   │   └── ...
│   ├── psybient/
│   │   └── ...
│   └── presets/
│       └── methodology.json
├── ui-ux-kb/
│   ├── manifest.json
│   ├── visual-standards/
│   │   └── ...
│   ├── control-layout/
│   │   └── ...
│   ├── knobs/
│   │   └── design-patterns.json
│   ├── sliders/
│   │   └── ...
│   ├── meters/
│   │   └── ...
│   ├── theming/
│   │   └── ...
│   ├── lookandfeel/
│   │   └── juce-patterns.json
│   └── accessibility/
│       └── ...
├── commercial-kb/
│   ├── manifest.json
│   ├── marketplaces/
│   │   ├── kvr-audio.json
│   │   └── plugin-boutique.json
│   ├── licensing/
│   │   ├── pace-ilok.json
│   │   ├── elicenser.json
│   │   └── custom.json
│   ├── pricing/
│   │   └── indie-strategy.json
│   ├── marketing/
│   │   └── solo-developer.json
│   ├── anti-piracy/
│   │   └── measures.json
│   ├── distribution/
│   │   └── channels.json
│   └── business-planning/
│       └── template.json
├── reference-kb/
│   ├── manifest.json
│   ├── textbooks/
│   │   ├── pirkle.json
│   │   ├── zolzer.json
│   │   └── smith.json
│   ├── academic-papers/
│   │   └── index.json
│   ├── official-docs/
│   │   ├── juce.json
│   │   └── vst3-sdk.json
│   ├── tutorials/
│   │   └── index.json
│   ├── videos/
│   │   └── index.json
│   └── community/
│       └── resources.json
└── failure-modes-kb/           # Cross-cutting: referenced by all
    ├── manifest.json
    ├── audio-thread/
    │   ├── fm-01-allocation.json
    │   ├── fm-02-buffer-overflow.json
    │   ├── fm-04-denormals.json
    │   ├── fm-05-feedback.json
    │   └── fm-07-thread-safety.json
    ├── parameters/
    │   ├── fm-03-state.json
    │   ├── fm-09-smoothing.json
    │   └── fm-16-automation.json
    └── signal/
        ├── fm-08-mono.json
        ├── fm-13-aliasing.json
        └── fm-14-mix.json
```

---

## File Naming Conventions

| Element | Convention | Example |
|---------|-----------|----------|
| KB layer directory | `{layer-name}/` | `technical-kb/` |
| Topic directory | `{topic}/` | `dsp-algorithms/` |
| Entry file | `{entry-name}.json` | `apvts-setup.json` |
| Archive directory | `_archive/` | `_archive/` |
| Archived entry | `{id}_{version}.json` | `technical-kb_juce-patterns_apvts-setup_0.1.0.json` |
| Bridge entry | `bridge-{descriptor}.json` | `bridge-warm.json` |
| Manifest | `manifest.json` | `manifest.json` |

### Naming Rules

1. **Lowercase with hyphens** — `dsp-algorithms`, not `DSPAlgorithms`
2. **Descriptive entry names** — `apvts-setup`, not `entry-001`
3. **Bridge prefix for bridges** — `bridge-warm.json`
4. **FM prefix for failure modes** — `fm-01-allocation.json`
5. **No spaces or special characters** — Use hyphens

---

## Master Index Format

**File:** `kb/master-index.json`

```json
{
  "schema_version": "1.0.0",
  "last_updated": "2026-04-02",
  "layers": {
    "technical-kb": {
      "entry_count": 25,
      "topics": ["cpp-audio", "juce-patterns", "dsp-algorithms", ...],
      "authority_score": 0.95
    },
    "sound-design-kb": {
      "entry_count": 18,
      "topics": ["synthesis-theory", "psychedelic", ...],
      "authority_score": 0.85,
      "bridge_count": 7
    },
    "ui-ux-kb": {
      "entry_count": 12,
      "topics": ["visual-standards", "knobs", ...],
      "authority_score": 0.80
    },
    "commercial-kb": {
      "entry_count": 10,
      "topics": ["marketplaces", "licensing", ...],
      "authority_score": 0.75
    },
    "reference-kb": {
      "entry_count": 15,
      "topics": ["textbooks", "official-docs", ...],
      "authority_score": 1.0
    },
    "failure-modes-kb": {
      "entry_count": 40,
      "topics": ["audio-thread", "parameters", "signal"],
      "authority_score": 0.90
    }
  },
  "cross_layer_mappings": {
    "bridge_entries": 7,
    "failure_mode_references": 25
  },
  "status_summary": {
    "placeholder": 40,
    "harvested": 30,
    "curated": 20,
    "synced": 15
  }
}
```

---

## Per-Layer Manifest Format

**File:** `kb/{layer}/manifest.json`

```json
{
  "kb_name": "technical-kb",
  "schema_version": "1.0.0",
  "last_updated": "2026-04-02",
  "authority_score": 0.95,
  "topics": {
    "cpp-audio": {
      "entry_count": 3,
      "entries": [
        {"id": "technical-kb_cpp-audio_real-time-safety", "status": "curated", "version": "1.0.0"},
        {"id": "technical-kb_cpp-audio_memory-management", "status": "harvested", "version": "0.2.0"}
      ]
    },
    "juce-patterns": {
      "entry_count": 4,
      "entries": [
        {"id": "technical-kb_juce-patterns_apvts-setup", "status": "curated", "version": "1.0.0"}
      ]
    }
  },
  "status_counts": {
    "placeholder": 5,
    "harvested": 10,
    "curated": 8,
    "synced": 2
  },
  "sync_history": [
    {"date": "2026-04-02", "action": "initial_population", "entries_added": 25}
  ]
}
```

---

## Cross-Reference Rules

### Query Priority

1. **Authority score** — Higher authority entries ranked first
2. **Status priority** — `synced` > `curated` > `harvested` > `placeholder`
3. **Domain relevance** — Higher relevance entries ranked first

### Cross-Reference Format

```json
{
  "related_topics": [
    "technical-kb:parameter-management",
    "sound-design-kb:filter-design"
  ],
  "failure_modes": ["FM-01", "FM-09"],
  "bridge_entry": "bridge-warm"
}
```

### Reference Format

```json
{
  "reference": {
    "kb": "reference-kb",
    "entry": "textbooks_pirkle",
    "section": "Chapter 5: Filters"
  }
}
```

---

## Entry Counts Summary

| Layer | Topics | Min Entries | Archive |
|-------|--------|-------------|----------|
| Technical KB | 8 | 25 | Yes |
| Sound Design KB | 11 | 20 | Yes |
| UI/UX KB | 8 | 12 | Yes |
| Commercial KB | 7 | 10 | Yes |
| Reference KB | 6 | 15 | Yes |
| Failure Modes KB | 3 | 40 | Yes |
| **Total** | **43** | **122** | |
| **Bridge Entries** | 7 | 7 | No |

---

## Validation Checklist

- [ ] All directories exist
- [ ] All `_archive/` directories exist
- [ ] All manifest.json files exist
- [ ] master-index.json exists
- [ ] Entry files match schema
- [ ] File names follow conventions
- [ ] No duplicate IDs within layer
- [ ] All related_topics resolve
- [ ] All failure_modes IDs exist