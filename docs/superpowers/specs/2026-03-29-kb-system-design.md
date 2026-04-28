# Knowledge Base System Design Specification

> **Created:** 2026-03-29
> **Status:** Approved
> **Version:** 1.1.0

## Overview

Production-grade knowledge base (KB) system for JUCE VST plugin development. Six specialized KBs with standardized structure, cross-referencing, and skill integration.

## Goals

1. **Centralized Knowledge**: Single source of truth for all JUCE development knowledge
2. **Skill Integration**: Skills query KBs dynamically instead of embedding knowledge
3. **Audio Relevance**: Content scored for audio plugin development relevance
4. **Maintainability**: Clear structure for adding and updating content
5. **Discoverability**: Topic-based cross-referencing across all KBs

## Knowledge Base Architecture

### KB Structure

```
playbookdata/
├── master-index.json          # Central registry
├── kb-lookup.py               # Query interface
├── __init__.py                # Package init
│
├── juce-kb/                   # JUCE framework (harvested)
│   ├── index.json             # Topic → file mapping
│   ├── validation.json        # Integrity + scoring
│   ├── manifest.json          # Harvest tracking
│   └── juce-official/         # Source content
│       ├── classAudioProcessor.json
│       ├── classAudioBuffer.json
│       └── ...
│
├── dsp-kb/                    # DSP theory (harvested + curated)
│   ├── index.json
│   ├── validation.json
│   ├── stanford-ccrma/        # Harvested courses
│   │   ├── courses/
│   │   └── papers/
│   └── curated/               # Hand-crafted content
│       ├── filter-design.json
│       └── fft-implementations.json
│
├── sound-design-kb/           # Sound design (curated)
│   ├── index.json
│   ├── validation.json
│   ├── presets/               # Synth presets
│   ├── techniques/            # Sound design methods
│   └── genre-specific/        # Genre patterns
│
├── ui-kb/                     # Plugin UI (curated)
│   ├── index.json
│   ├── validation.json
│   ├── components/            # UI components
│   ├── lookandfeel/           # Custom L&F
│   └── accessibility/         # Accessibility patterns
│
├── cpp-kb/                    # C++ patterns (curated)
│   ├── index.json
│   ├── validation.json
│   ├── patterns/              # Design patterns
│   ├── thread_safety/         # Concurrency
│   └── memory/                # Memory management
│
└── cmake-kb/                  # Build system (curated)
    ├── index.json
    ├── validation.json
    ├── targets/               # JUCE targets
    ├── platforms/             # Platform-specific
    └── configuration/         # Build configs
```

### KB Content Types

| KB | Source Type | Content Focus | Audio Relevance Baseline |
|---|---|---|---|
| **juce-kb** | Harvested | JUCE API documentation, tutorials | 8 |
| **dsp-kb** | Harvested + Curated | DSP theory, algorithms, implementations | 9 |
| **sound-design-kb** | Curated | Presets, synthesis techniques | 8 |
| **ui-kb** | Curated | UI components, LookAndFeel, accessibility | 6 |
| **cpp-kb** | Curated | C++ patterns, thread safety, memory | 4 |
| **cmake-kb** | Curated | CMake configuration, targets, platforms | 3 |

---

## Index Structure

### Master Index (`master-index.json`)

```json
{
  "version": "1.0.0",
  "last_updated": "2026-03-29T...",
  "knowledge_bases": {
    "juce-kb": {
      "path": "juce-kb",
      "source_type": "harvested",
      "pages": 301,
      "status": "ready"
    },
    "dsp-kb": {
      "path": "dsp-kb",
      "source_type": "harvested+curated",
      "pages": 19,
      "status": "ready"
    },
    "sound-design-kb": {
      "path": "sound-design-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "empty"
    },
    "ui-kb": {
      "path": "ui-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "empty"
    },
    "cpp-kb": {
      "path": "cpp-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "empty"
    },
    "cmake-kb": {
      "path": "cmake-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "empty"
    }
  },
  "topic_cross_references": {
    "audio_processing": ["juce-kb/AudioProcessor.json", "juce-kb/AudioBuffer.json"],
    "dsp": ["juce-kb/dsp/", "dsp-kb/filters/", "dsp-kb/fft/"],
    "thread_safety": ["juce-kb/Thread.json", "juce-kb/CriticalSection.json", "cpp-kb/concurrency/"],
    "parameters": ["juce-kb/AudioProcessorParameter.json", "sound-design-kb/presets/"],
    "gui": ["juce-kb/Component.json", "ui-kb/components/", "ui-kb/lookandfeel/"],
    "state_management": ["juce-kb/ValueTree.json", "juce-kb/AudioProcessorValueTreeState.json"]
  },
  "audio_specific_index": {
    "plugin_types": {
      "effects": ["juce-kb/AudioProcessor.json", "dsp-kb/effects/"],
      "synthesizers": ["juce-kb/dsp/Oscillator.json", "dsp-kb/synthesis/", "sound-design-kb/synthesis/"],
      "meters": ["juce-kb/LevelMeter.json", "ui-kb/meters/"],
      "analyzers": ["juce-kb/dsp/FFT.json", "dsp-kb/spectral/"]
    },
    "signal_flow": {
      "input": ["juce-kb/AudioDeviceManager.json"],
      "processing": ["juce-kb/dsp/", "dsp-kb/"],
      "output": ["juce-kb/AudioFormatWriter.json"]
    }
  },
  "metadata": {
    "total_files": 320,
    "total_topics": 10,
    "harvest_sources": ["juce-official", "melatonin", "stanford-ccrma", "awesome-juce"]
  }
}
```

### Per-KB Index (`kb-name/index.json`)

```json
{
  "kb_name": "juce-kb",
  "version": "1.0.0",
  "last_updated": "2026-03-29T...",
  "description": "JUCE framework documentation and tutorials",
  "topics": {
    "audio_processing": ["AudioProcessor.json", "AudioBuffer.json"],
    "dsp": ["dsp/IIR.json", "dsp/FFT.json", "dsp/Oscillator.json"],
    "thread_safety": ["Thread.json", "CriticalSection.json"],
    "gui": ["Component.json", "Slider.json", "ComboBox.json"],
    "state_management": ["ValueTree.json", "AudioProcessorValueTreeState.json"],
    "midi": ["MidiBuffer.json", "MidiMessage.json"],
    "parameters": ["AudioProcessorParameter.json", "RangedAudioParameter.json"],
    "testing": ["pluginval.json"],
    "ci_cd": ["github-actions.json"],
    "deployment": ["signing.json", "notarization.json"]
  },
  "sources": {
    "juce-official": {
      "status": "completed",
      "pages_harvested": 272,
      "harvest_completed": "2026-03-29T..."
    },
    "melatonin": {
      "status": "completed",
      "pages_harvested": 21,
      "harvest_completed": "2026-03-29T..."
    },
    "github-awesome-juce": {
      "status": "completed",
      "pages_harvested": 8,
      "harvest_completed": "2026-03-29T..."
    }
  },
  "metadata": {
    "total_files": 301,
    "source_type": "harvested",
    "audio_relevance_baseline": 8
  }
}
```

### Validation File (`kb-name/validation.json`)

```json
{
  "kb_name": "juce-kb",
  "last_validated": "2026-03-29T...",
  "checksum": "sha256:abc123...",
  "total_files": 301,
  "total_size_bytes": 27500000,
  "files": {
    "AudioProcessor.json": {
      "hash": "sha256:def456...",
      "audio_relevance": 10,
      "difficulty": "intermediate",
      "implementation_examples": 5,
      "size_bytes": 125000,
      "validated_at": "2026-03-29T..."
    },
    "dsp/IIR.json": {
      "hash": "sha256:ghi789...",
      "audio_relevance": 9,
      "difficulty": "advanced",
      "implementation_examples": 3,
      "size_bytes": 95000,
      "validated_at": "2026-03-29T..."
    }
  },
  "defaults": {
    "audio_relevance": 5,
    "difficulty_level": "intermediate"
  },
  "statistics": {
    "avg_audio_relevance": 7.8,
    "by_difficulty": {
      "beginner": 25,
      "intermediate": 85,
      "advanced": 28
    }
  }
}
```

---

## Query Interface

### KBQuery Class

Central lookup for all knowledge bases with caching and error handling.

**Primary Methods:**

| Method | Purpose | Returns |
|--------|---------|---------|
| `query_topic(topic, kb_names)` | Query KBs for a topic | `list[KBResult]` |
| `get_file_content(file_path)` | Load full file content | `KBFile` |
| `get_implementation_examples(topic)` | Get high-relevance examples | `list[KBResult]` |
| `search_content(query, kb_names)` | Full-text search | `list[KBResult]` |

**Specialized Methods:**

| Method | Purpose |
|--------|---------|
| `get_related_topics(topic)` | Find related topics via cross-references |
| `get_skill_recommendations(topic)` | Suggest skills for a topic |
| `get_dependencies(file_path)` | Get related files in other KBs |
| `health_check()` | Validate all KBs |

**Usage Example:**

```python
from playbookdata.kb_lookup import get_kb

kb = get_kb()

# Query for topic
results = kb.query_topic("thread_safety", ["juce-kb", "cpp-kb"])

# Get implementation examples
examples = kb.get_implementation_examples("dsp", min_audio_relevance=7)

# Get full content
content = kb.get_file_content(results[0].file_path)

# Get related topics
related = kb.get_related_topics("dsp")  # ['audio_processing', 'parameters']

# Get skill recommendations
skills = kb.get_skill_recommendations("dsp")  # ['juce-dsp-implementation']
```

### KBResult Dataclass

```python
@dataclass
class KBResult:
    kb_name: str
    file_path: str
    relevance_score: float      # 0-10 based on authority + specificity
    audio_relevance: int        # 0-10 based on content analysis
    title: str                  # Extracted from content
    source: str                 # Source attribution
```

### KBFile Dataclass

```python
@dataclass
class KBFile:
    kb_name: str
    file_path: str
    content: dict              # Full JSON content
    metadata: dict             # source, harvested_at, source_url, etc.
```

---

## Cross-Referencing System

### Topic Cross-References

Topics map to files across multiple KBs:

```
topic_cross_references = {
    "dsp": [
        "juce-kb/dsp/IIR.json",
        "juce-kb/dsp/FFT.json",
        "dsp-kb/filters/butterworth.json",
        "dsp-kb/spectral/fft-algorithms.json"
    ],
    "thread_safety": [
        "juce-kb/Thread.json",
        "juce-kb/CriticalSection.json",
        "cpp-kb/concurrency/atomic.json",
        "cpp-kb/patterns/lock-free-queue.json"
    ]
}
```

### Audio-Specific Index

Plugin-specific organization:

```json
{
  "plugin_types": {
    "effects": [
      "juce-kb/AudioProcessor.json",
      "dsp-kb/effects/",
      "sound-design-kb/effects/"
    ],
    "synthesizers": [
      "juce-kb/dsp/Oscillator.json",
      "dsp-kb/synthesis/",
      "sound-design-kb/synthesis/"
    ]
  },
  "signal_flow": {
    "input": ["juce-kb/AudioDeviceManager.json"],
    "processing": ["juce-kb/dsp/", "dsp-kb/"],
    "output": ["juce-kb/AudioFormatWriter.json"]
  }
}
```

### Resolution Algorithm

1. Check master-index for topic → KB mappings
2. Query relevant KB indices for specific files
3. Rank results by:
   - KB authority (juce-kb = 1.0, cpp-kb = 0.5)
   - Audio relevance score
   - Implementation example count
4. Cache common queries

---

## Audio Relevance Scoring

### Scoring Algorithm

```python
def score_audio_relevance(content: dict, kb_name: str) -> int:
    """Score audio relevance (0-10)."""
    text = content.get('markdown', '') or content.get('html', '')
    text_lower = text.lower()

    # KB-specific baselines
    baselines = {
        'juce-kb': 7,
        'dsp-kb': 9,
        'sound-design-kb': 8,
        'ui-kb': 5,
        'cpp-kb': 3,
        'cmake-kb': 2
    }
    score = baselines.get(kb_name, 5)

    # Audio-specific keywords boost
    audio_keywords = [
        'AudioProcessor', 'AudioBuffer', 'MidiBuffer', 'processBlock',
        'sample', 'buffer', 'DSP', 'filter', 'oscillator', 'envelope',
        'ADSR', 'FFT', 'reverb', 'delay', 'compression'
    ]

    keyword_count = sum(1 for kw in audio_keywords if kw.lower() in text_lower)
    score += min(keyword_count, 3)  # +3 max boost

    # Implementation examples boost
    if '```cpp' in text or '```c++' in text:
        score += 1

    return min(score, 10)
```

### Difficulty Level

```python
def determine_difficulty(content: dict) -> str:
    """Determine difficulty level: beginner, intermediate, advanced."""
    text = content.get('markdown', '') or content.get('html', '')

    advanced = ['thread', 'lock', 'atomic', 'SIMD', 'optimization', 'real-time']
    intermediate = ['class', 'inherit', 'virtual', 'override', 'component']

    adv_count = sum(1 for w in advanced if w.lower() in text.lower())
    int_count = sum(1 for w in intermediate if w.lower() in text.lower())

    if adv_count >= 3:
        return 'advanced'
    elif int_count >= 2:
        return 'intermediate'
    else:
        return 'beginner'
```

---

## Skill-to-KB Integration

### Integration Pattern

```python
# In skill file:
from playbookdata.kb_lookup import get_kb, KBError

def get_topic_content(topic: str, fallback: str = None) -> str:
    """Get content for a topic with fallback."""
    try:
        kb = get_kb()
        results = kb.query_topic(topic)
        if results:
            content = kb.get_file_content(results[0].file_path)
            return content.content.get('markdown', '')
    except KBError:
        pass

    return fallback or f"# {topic}\n\nContent not available."
```

### Skill-to-KB Mapping

| Skill | Primary KBs | Query Patterns |
|-------|--------------|----------------|
| `juce-plugin-spec` | juce-kb | `query_topic("audio_processing")`, `query_topic("parameters")` |
| `juce-dsp-implementation` | dsp-kb, juce-kb | `query_topic("dsp")`, `get_implementation_examples("dsp")` |
| `juce-daw-testing` | juce-kb | `query_topic("testing")`, `search_content("pluginval")` |
| `juce-sound-design-bridge` | sound-design-kb | `query_topic("parameters")`, `get_file_content("presets/")` |
| `juce-ui-bridge` | ui-kb, juce-kb | `query_topic("gui")`, `search_content("LookAndFeel")` |

### Fallback Content

When KB is unavailable, skills use embedded minimal fallback:

```json
// fallback-content.json
{
  "audio_processing": {
    "title": "Audio Processing Basics",
    "content": "processBlock handles audio buffers...",
    "minimal": true
  },
  "dsp": {
    "title": "DSP Fundamentals",
    "content": "Digital signal processing for audio...",
    "minimal": true
  }
}
```

---

## Harvest System

### Current Harvest Scripts

| Script | Source | Target KB |
|--------|--------|-----------|
| `harvest-juce-docs.sh` | docs.juce.com | juce-kb |
| `harvest-melatonin.sh` | melatonin.dev | juce-kb |
| `harvest-stanford.sh` | ccrma.stanford.edu | dsp-kb |
| `harvest-awesome-juce.sh` | github.com/sudara/awesome-juce | juce-kb |

### Harvest Output Format

```json
{
  "markdown": "# AudioProcessor\n\n...",
  "html": "<!DOCTYPE html>...",
  "links": ["https://docs.juce.com/...", "..."],
  "source_url": "https://docs.juce.com/classAudioProcessor.html",
  "source": "juce-official",
  "juce_version": "7.0.12",
  "harvested_at": "2026-03-29T...",
  "kb_type": "technical"
}
```

### Manifest Tracking

```json
{
  "phases": [
    {
      "name": "juce-official",
      "status": "completed",
      "started_at": "2026-03-29T...",
      "completed_at": "2026-03-29T...",
      "pages_total": 120,
      "pages_completed": 120
    }
  ],
  "status": "completed",
  "started_at": "2026-03-29T...",
  "completed_at": "2026-03-29T..."
}
```

---

## Validation System

### Validation Script (`validate_kb.py`)

Generates validation.json for each KB:

1. Calculate SHA256 hash per file
2. Score audio relevance (0-10)
3. Determine difficulty level
4. Count implementation examples
5. Calculate statistics

### Health Check

```python
def health_check() -> dict:
    """Run health check on all KBs."""
    results = {}
    for kb_name in list_kbs():
        try:
            results[kb_name] = validate_kb(kb_name)
        except KBError as e:
            results[kb_name] = {"valid": False, "error": str(e)}

    return {
        "overall_valid": all(r.get("valid", False) for r in results.values()),
        "kbs": results,
        "timestamp": datetime.now().isoformat()
    }
```

---

## Implementation Phases

### Phase 0: Preparation (30 minutes)

**Purpose:** Safe migration with rollback capability

**Tasks:**
1. Create backup of playbookdata directory
2. Create rollback script
3. Run dry-run validation

**Deliverables:**
- Backup directory with timestamp
- rollback.sh script
- Pre-migration validation report

**Success Criteria:**
- Backup verified
- Rollback script tested
- Dry-run passes

---

### Phase 1: Foundation Setup (1 hour)

**Purpose:** Create infrastructure without touching existing data

**Tasks:**
1. Create KB directory structure
2. Create master-index.json template
3. Create kb-lookup.py module with tests
4. Create __init__.py for package

**Deliverables:**
- Empty KB directories
- master-index.json template
- kb-lookup.py module
- Unit tests

**Success Criteria:**
- Package importable
- Tests pass
- JSON valid

---

### Phase 2: Reorganization (2 hours)

**Purpose:** Migrate existing data with integrity checks

**Tasks:**
1. Rename technical-kb → juce-kb
2. Extract Stanford CCRMA → dsp-kb
3. Create empty KBs (sound-design, ui, cpp, cmake)
4. Update master-index.json

**Deliverables:**
- juce-kb populated with JUCE content
- dsp-kb populated with Stanford content
- Empty KBs with skeleton structure
- Updated master-index

**Success Criteria:**
- All 6 KBs exist
- No files lost
- File counts match expectations

---

### Phase 3: Validation Infrastructure (1.5 hours)

**Purpose:** Add quality metrics and integrity checks

**Tasks:**
1. Create validation generator script
2. Generate validation.json per KB
3. Add audio relevance scoring
4. Add difficulty levels

**Deliverables:**
- validate_kb.py script
- validation.json per KB
- Scoring metadata

**Success Criteria:**
- All KBs have validation.json
- Scores calculated
- Statistics accurate

---

### Phase 4: Skill Integration (2-3 hours)

**Purpose:** Update skills to use KB queries

**Incremental Approach:** Update one skill at a time, test after each

**Tasks:**
1. Create skill integration template
2. Create fallback content file
3. Update each skill:
   - juce-plugin-spec
   - juce-dsp-implementation
   - juce-daw-testing
   - juce-sound-design-bridge
   - juce-ui-bridge
4. Test each skill after update

**Deliverables:**
- Updated skills with KB queries
- Fallback content
- Integration tests

**Success Criteria:**
- Skills query KB successfully
- Fallback works when KB unavailable
- All skill tests pass

---

### Phase 5: Playbook Migration (2 hours)

**Purpose:** Extract reference content to KBs, preserve operational knowledge

**Tasks:**
1. Analyze playbook for migration targets
2. Extract reference content to appropriate KBs
3. Preserve operational knowledge in playbook
4. Update playbook with KB references

**Deliverables:**
- Extracted reference content in KBs
- Updated playbook with references
- Operational knowledge preserved

**Success Criteria:**
- Playbook references KB correctly
- No information lost
- Operational steps remain intact

---

### Phase 6: Validation & Gap Analysis (1 hour)

**Purpose:** Verify system integrity and identify gaps

**Tasks:**
1. Run full validation suite
2. Test skill integration
3. Verify playbook references
4. Identify missing/low-quality content
5. Generate harvesting priority list

**Deliverables:**
- Health check report
- Gap analysis document
- Harvesting priority list

**Success Criteria:**
- All KBs pass health check
- All skills integrated
- Gap analysis complete

---

## Success Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| KB health | 100% valid | `health_check()` returns `overall_valid: true` |
| Skill integration | 5/5 skills | All skills query KB successfully |
| Audio relevance avg | ≥ 6.0 | `validation.json` statistics |
| Missing topics | < 3 | Gap analysis count |
| Playbook refs valid | 100% | All `kb_ref` paths resolve |
| Query latency | < 100ms | Topic query response time |

---

## Future Harvesting Priorities

### cpp-kb Sources
- cppreference.com - C++ standard library reference
- isocpp.org - Modern C++ guidelines
- C++ Core Guidelines

### cmake-kb Sources
- cmake.org/documentation - Official CMake docs
- Modern CMake articles

### ui-kb Sources
- JUCE LookAndFeel tutorials
- UI design patterns for audio

### dsp-kb Sources
- Additional DSP resources beyond Stanford
- Real-world implementation examples

---

## Maintenance

### Adding New Content

1. Harvest or curate content
2. Run `validate_kb.py` to update validation.json
3. Update `index.json` with new topics
4. Run `health_check()` to verify

### Updating Content

1. Re-harvest or edit content
2. Re-run validation
3. Update cross-references if topics change

### Removing Content

1. Remove file from KB
2. Update index.json to remove reference
3. Re-run validation
4. Update cross-references

---

## Appendix A: File Structure

```
playbookdata/
├── __init__.py
├── kb-lookup.py
├── master-index.json
│
├── juce-kb/
│   ├── index.json
│   ├── validation.json
│   ├── manifest.json
│   └── juce-official/
│       ├── classAudioProcessor.json
│       ├── classAudioBuffer.json
│       ├── dsp/
│       │   ├── IIR.json
│       │   ├── FFT.json
│       │   └── ...
│       └── tutorials/
│           └── ...
│
├── dsp-kb/
│   ├── index.json
│   ├── validation.json
│   ├── stanford-ccrma/
│   │   ├── courses/
│   │   └── papers/
│   └── curated/
│       └── ...
│
├── sound-design-kb/
│   ├── index.json
│   ├── validation.json
│   ├── presets/
│   ├── techniques/
│   └── genre-specific/
│
├── ui-kb/
│   ├── index.json
│   ├── validation.json
│   ├── components/
│   ├── lookandfeel/
│   └── accessibility/
│
├── cpp-kb/
│   ├── index.json
│   ├── validation.json
│   ├── patterns/
│   ├── thread_safety/
│   └── memory/
│
└── cmake-kb/
    ├── index.json
    ├── validation.json
    ├── targets/
    ├── platforms/
    └── configuration/
```

## Appendix B: Topic Keywords

Standard topics for cross-referencing:

| Topic | Keywords |
|-------|----------|
| `audio_processing` | AudioProcessor, processBlock, AudioBuffer, prepareToPlay |
| `dsp` | dsp::, IIR, FIR, FFT, Filter, Gain, Delay, Reverb |
| `thread_safety` | Thread, CriticalSection, Atomic, Lock, MessageThread |
| `gui` | Component, Slider, Button, Graphics, paint, resized |
| `state_management` | ValueTree, State, APVTS, Parameter, getState, setState |
| `midi` | MidiBuffer, MidiMessage, MIDI, Note, noteOn, noteOff |
| `parameters` | Parameter, AudioProcessorParameter, RangedParameter |
| `testing` | test, Test, Catch2, GoogleTest, pluginval |
| `ci_cd` | GitHub Actions, CI, CD, workflow, build, pipeline |
| `deployment` | deploy, install, sign, notarize, distribute |

---

## Appendix C: Query Cache

The `master-index.json` includes a query cache for common lookups:

```json
{
  "query_cache": {
    "how_to_create_parameter": [
      "juce-kb/AudioProcessorParameter.json",
      "juce-kb/AudioProcessorValueTreeState.json"
    ],
    "how_to_handle_midi": [
      "juce-kb/MidiBuffer.json",
      "juce-kb/MidiMessage.json"
    ],
    "how_to_add_gui": [
      "juce-kb/Component.json",
      "ui-kb/components/"
    ],
    "how_to_implement_dsp": [
      "juce-kb/dsp/IIR.json",
      "dsp-kb/filters/"
    ],
    "how_to_save_state": [
      "juce-kb/ValueTree.json",
      "juce-kb/AudioProcessorValueTreeState.json"
    ]
  }
}
```

Cache is updated after each harvest or content addition. Maximum cache size: 100 entries (LRU eviction).

---

## Appendix D: Testing Requirements

### Unit Tests for kb-lookup.py

```python
# tests/test_kb_lookup.py

import pytest
from playbookdata.kb_lookup import get_kb, KBQuery, KBError

class TestKBQuery:
    """Unit tests for KB query functionality."""

    def test_query_topic_returns_results(self):
        """Query for a known topic should return results."""
        kb = get_kb()
        results = kb.query_topic("audio_processing")
        assert len(results) > 0
        assert all(r.audio_relevance >= 0 for r in results)

    def test_query_topic_unknown_returns_empty(self):
        """Query for unknown topic should return empty list."""
        kb = get_kb()
        results = kb.query_topic("nonexistent_topic_xyz")
        assert results == []

    def test_query_topic_filtered_by_kb(self):
        """Query can be filtered to specific KBs."""
        kb = get_kb()
        results = kb.query_topic("dsp", kb_names=("dsp-kb",))
        assert all(r.kb_name == "dsp-kb" for r in results)

    def test_query_topic_min_audio_relevance(self):
        """Query respects minimum audio relevance filter."""
        kb = get_kb()
        results = kb.query_topic("dsp", min_audio_relevance=7)
        assert all(r.audio_relevance >= 7 for r in results)

    def test_get_file_content_returns_dict(self):
        """get_file_content returns KBFile with content."""
        kb = get_kb()
        results = kb.query_topic("audio_processing")
        if results:
            content = kb.get_file_content(results[0].file_path)
            assert content.content is not None
            assert content.kb_name == results[0].kb_name

    def test_get_file_content_not_found_raises(self):
        """get_file_content raises KBNotFoundError for missing file."""
        kb = get_kb()
        with pytest.raises(KBError):
            kb.get_file_content("/nonexistent/path/file.json")

    def test_get_implementation_examples_filters_relevance(self):
        """get_implementation_examples respects min relevance."""
        kb = get_kb()
        results = kb.get_implementation_examples("dsp", min_audio_relevance=8)
        assert all(r.audio_relevance >= 8 for r in results)

    def test_search_content_finds_matches(self):
        """search_content finds text matches."""
        kb = get_kb()
        results = kb.search_content("AudioProcessor")
        assert len(results) > 0

    def test_search_content_case_insensitive(self):
        """search_content is case insensitive by default."""
        kb = get_kb()
        results_lower = kb.search_content("audioprocessor")
        results_upper = kb.search_content("AUDIOPROCESSOR")
        assert len(results_lower) == len(results_upper)

    def test_get_related_topics_returns_list(self):
        """get_related_topics returns related topics."""
        kb = get_kb()
        related = kb.get_related_topics("dsp")
        assert isinstance(related, list)
        # dsp relates to audio_processing, parameters, etc.

    def test_get_skill_recommendations_returns_list(self):
        """get_skill_recommendations returns skill names."""
        kb = get_kb()
        skills = kb.get_skill_recommendations("dsp")
        assert "juce-dsp-implementation" in skills

    def test_health_check_returns_valid_structure(self):
        """health_check returns proper structure."""
        kb = get_kb()
        health = kb.health_check()
        assert "overall_valid" in health
        assert "kbs" in health
        assert "timestamp" in health

    def test_clear_cache_resets_caches(self):
        """clear_cache empties all caches."""
        kb = get_kb()
        # Populate cache
        kb.query_topic("dsp")
        # Clear
        kb.clear_cache()
        # Verify cache is cleared
        assert kb.query_topic.cache_info().currsize == 0

    def test_warm_cache_populates_topics(self):
        """warm_cache pre-populates common queries."""
        kb = get_kb()
        kb.clear_cache()
        kb.warm_cache(["dsp", "gui"])
        # Cache should have entries
        assert kb.query_topic.cache_info().currsize > 0


class TestKBErrorHandling:
    """Tests for error handling."""

    def test_missing_kb_returns_empty_results(self):
        """Query for missing KB returns empty results."""
        kb = get_kb()
        results = kb.query_topic("dsp", kb_names=("nonexistent-kb",))
        assert results == []

    def test_missing_file_raises_kb_error(self):
        """Accessing missing file raises KBError."""
        kb = get_kb()
        with pytest.raises(KBError):
            kb.get_file_content("/nonexistent/file.json")

    def test_corrupt_json_handled_gracefully(self):
        """Corrupt JSON files are handled gracefully."""
        # This would be tested by creating a corrupt file
        # and verifying health_check reports it
        pass

    def test_fallback_content_used_when_kb_unavailable(self):
        """Skills use fallback when KB is unavailable."""
        # This would be tested in skill integration tests
        pass


class TestKBValidation:
    """Tests for validation functionality."""

    def test_validate_kb_returns_report(self):
        """validate_kb returns validation report."""
        kb = get_kb()
        report = kb.validate_kb("juce-kb")
        assert "kb_name" in report
        assert "valid" in report
        assert "file_count" in report

    def test_validate_kb_missing_raises(self):
        """validate_kb raises for missing KB."""
        kb = get_kb()
        with pytest.raises(KBError):
            kb.validate_kb("nonexistent-kb")

    def test_validation_detects_missing_files(self):
        """Validation detects files referenced but missing."""
        # This would be tested by modifying index to reference
        # a nonexistent file
        pass

    def test_audio_relevance_in_valid_range(self):
        """Audio relevance scores are always 0-10."""
        kb = get_kb()
        health = kb.health_check()
        for kb_name, report in health["kbs"].items():
            if report.get("valid"):
                validation = kb._get_kb_index(kb_name)
                # All files should have relevance 0-10
                # (validated in validation.json)
```

### Integration Tests

```python
# tests/test_kb_integration.py

import pytest
from playbookdata.kb_lookup import get_kb

class TestSkillIntegration:
    """Integration tests for skill-to-KB queries."""

    def test_juce_plugin_spec_queries(self):
        """juce-plugin-spec can query required topics."""
        kb = get_kb()

        # Required queries for juce-plugin-spec
        audio_results = kb.query_topic("audio_processing")
        param_results = kb.query_topic("parameters")

        assert len(audio_results) > 0, "audio_processing topic must have results"
        assert len(param_results) > 0, "parameters topic must have results"

    def test_juce_dsp_implementation_queries(self):
        """juce-dsp-implementation can query required topics."""
        kb = get_kb()

        dsp_results = kb.query_topic("dsp")
        examples = kb.get_implementation_examples("dsp")

        assert len(dsp_results) > 0, "dsp topic must have results"

    def test_juce_daw_testing_queries(self):
        """juce-daw-testing can query required topics."""
        kb = get_kb()

        test_results = kb.query_topic("testing")

        assert len(test_results) > 0, "testing topic must have results"

    def test_cross_kb_query(self):
        """Queries can span multiple KBs."""
        kb = get_kb()

        # Thread safety spans juce-kb and cpp-kb
        results = kb.query_topic("thread_safety", kb_names=("juce-kb", "cpp-kb"))

        # Results should be from both KBs
        kb_names = {r.kb_name for r in results}
        assert len(kb_names) > 0
```

---

## Appendix E: Error Handling and Recovery

### Error Types

| Error | Cause | Recovery |
|-------|-------|----------|
| `KBNotFoundError` | KB directory doesn't exist | Return empty results, log warning |
| `KBFileNotFoundError` | File in index but missing on disk | Mark invalid in validation.json, continue |
| `KBIntegrityError` | JSON parse failed | Skip file, log error, continue |
| `KBValidationError` | Checksum mismatch | Re-run validation, log warning |
| `KBTimeoutError` | Query took > 5 seconds | Return cached results, log warning |

### Fallback Chain

```python
def get_content_with_fallback(topic: str) -> str:
    """Get content with progressive fallbacks."""
    kb = get_kb()

    # 1. Try KB query
    results = kb.query_topic(topic)
    if results:
        try:
            content = kb.get_file_content(results[0].file_path)
            return content.content.get('markdown', '')
        except KBError:
            pass

    # 2. Try fallback content file
    fallback_path = f"playbookdata/fallback-content.json"
    if Path(fallback_path).exists():
        fallback = json.load(open(fallback_path))
        if topic in fallback:
            return fallback[topic]['content']

    # 3. Return placeholder
    return f"# {topic}\n\nContent not available. KB may be rebuilding."
```

### Recovery Procedures

**KB Corruption:**
1. Stop all queries (set KB to maintenance mode)
2. Run `validate_kb.py` to identify corrupt files
3. Re-harvest corrupt files from source
4. Re-run validation
5. Resume queries

**Missing Index:**
1. Check backup directory
2. If backup exists, restore index.json
3. If no backup, regenerate from file scan:
   ```bash
   python3 scripts/rebuild_index.py --kb juce-kb
   ```

**Performance Degradation:**
1. Check cache hit rate: `kb.query_topic.cache_info()`
2. If low (< 50%), warm cache: `kb.warm_cache()`
3. If still slow, check file sizes in validation.json
4. Consider splitting large files (> 1MB)

---

## Appendix F: Security Considerations

### Input Validation

```python
def validate_topic(topic: str) -> str:
    """Validate and sanitize topic input."""
    # Allow only alphanumeric, underscore, hyphen
    if not re.match(r'^[a-zA-Z0-9_-]+$', topic):
        raise ValueError(f"Invalid topic name: {topic}")

    # Limit length
    if len(topic) > 64:
        raise ValueError(f"Topic too long: {topic}")

    return topic.lower()
```

### Path Traversal Prevention

```python
def safe_path(kb_name: str, file_path: str) -> Path:
    """Construct safe file path, preventing traversal."""
    kb_root = KB_ROOT / kb_name
    full_path = (kb_root / file_path).resolve()

    # Ensure path is within KB root
    if not str(full_path).startswith(str(kb_root.resolve())):
        raise KBError(f"Path traversal attempt: {file_path}")

    return full_path
```

### JSON Parsing Safety

```python
def safe_load_json(path: Path) -> dict:
    """Load JSON with size limit and timeout."""
    MAX_SIZE = 10 * 1024 * 1024  # 10MB

    if path.stat().st_size > MAX_SIZE:
        raise KBError(f"File too large: {path}")

    try:
        with open(path) as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        raise KBError(f"Invalid JSON in {path}: {e}")
```

---

## Appendix G: Performance Targets

### Latency Requirements

| Operation | Target | Maximum |
|-----------|--------|---------|
| `query_topic()` | < 10ms | 100ms |
| `get_file_content()` | < 5ms | 50ms |
| `search_content()` | < 100ms | 500ms |
| `health_check()` | < 500ms | 2s |
| `validate_kb()` | < 5s | 30s |

### Memory Requirements

| Component | Target | Maximum |
|-----------|--------|---------|
| Per-KB index cache | < 1MB | 5MB |
| File content cache | < 10MB | 50MB |
| Query cache (LRU) | < 100 entries | 1000 entries |
| Total memory | < 50MB | 200MB |

### Optimization Strategies

1. **Lazy Loading**: KB indices loaded only when first queried
2. **LRU Cache**: Query results cached with LRU eviction
3. **File Content Cache**: Recently accessed files kept in memory
4. **Index Pre-compilation**: Topic cross-references pre-computed
5. **Parallel Queries**: Multiple KB queries run in parallel

---

## Appendix H: Monitoring and Logging

### Log Levels

```python
import logging

logger = logging.getLogger('playbookdata.kb_lookup')

# Log levels:
# DEBUG: Detailed query information
# INFO: KB initialization, cache operations
# WARNING: Missing files, fallback usage
# ERROR: Parse failures, integrity issues
# CRITICAL: Complete KB unavailability
```

### Metrics to Track

```python
# Metrics collected via health_check()
metrics = {
    "query_count": 0,           # Total queries since start
    "cache_hit_rate": 0.0,      # Percentage of cache hits
    "avg_query_latency_ms": 0,  # Average query time
    "kb_file_count": 0,         # Total files across all KBs
    "validation_errors": 0,      # Files failing validation
    "last_health_check": None   # Timestamp of last health check
}
```

### Health Check Endpoint

```python
def health_check() -> dict:
    """
    Run health check and return report.

    Returns:
        {
            "overall_valid": bool,
            "kbs": {kb_name: {"valid": bool, "file_count": int, "issues": []}},
            "timestamp": str,
            "metrics": {
                "query_count": int,
                "cache_hit_rate": float,
                "avg_query_latency_ms": float
            }
        }
    """
```

---

## Appendix I: Rollback Procedures

### Per-Phase Rollback

**Phase 0-1 Rollback:**
```bash
# No data modified, just delete created files
rm -rf playbookdata/juce-kb playbookdata/dsp-kb playbookdata/sound-design-kb
rm -rf playbookdata/ui-kb playbookdata/cpp-kb playbookdata/cmake-kb
rm playbookdata/master-index.json playbookdata/kb-lookup.py playbookdata/__init__.py
```

**Phase 2 Rollback:**
```bash
# Restore from backup
./rollback.sh playbookdata-backup-YYYYMMDD-HHMMSS

# Verify restoration
python3 -c "
from pathlib import Path
import json
index = json.load(open('playbookdata/technical-kb/index.json'))
print(f'Restored: {len(index.get(\"topics\", {}))} topics')
"
```

**Phase 3-4 Rollback:**
```bash
# Restore from backup
./rollback.sh playbookdata-backup-YYYYMMDD-HHMMSS

# Clear Python cache
find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null
python3 -c "from playbookdata.kb_lookup import reset_kb; reset_kb()"
```

**Phase 5-6 Rollback:**
```bash
# Restore from backup
./rollback.sh playbookdata-backup-YYYYMMDD-HHMMSS

# Restore playbook
cp playbook-backup.json playbooks/vst-plugin-playbook-v7-unified.json
```

### Verification After Rollback

```bash
# Run validation suite
python3 -c "
from playbookdata.kb_lookup import get_kb
kb = get_kb()
health = kb.health_check()
print(f'Overall valid: {health[\"overall_valid\"]}')
for kb_name, report in health['kbs'].items():
    print(f'{kb_name}: {\"✓\" if report.get(\"valid\") else \"✗\"}')
"
```

---

## Appendix J: Concurrent Access

### Thread Safety

The `KBQuery` class is thread-safe for read operations. The LRU cache uses
thread-safe operations internally.

```python
# Thread-safe usage example
from threading import Thread
from playbookdata.kb_lookup import get_kb

def worker(topic: str, results: list):
    kb = get_kb()
    results.extend(kb.query_topic(topic))

threads = []
results = []
for topic in ["dsp", "gui", "parameters"]:
    t = Thread(target=worker, args=(topic, results))
    threads.append(t)
    t.start()

for t in threads:
    t.join()

# Results safely collected from all threads
```

### Write Operations

Write operations (cache clear, warm) should be synchronized:

```python
import threading

_write_lock = threading.Lock()

def clear_cache(self) -> None:
    with _write_lock:
        self._kb_cache.clear()
        self._file_cache.clear()
        self.query_topic.cache_clear()
```

### Singleton Pattern

The `get_kb()` singleton is thread-safe:

```python
_kb_lookup: Optional[KBQuery] = None
_init_lock = threading.Lock()

def get_kb(initialize: bool = True) -> Optional[KBQuery]:
    global _kb_lookup
    if _kb_lookup is None and initialize:
        with _init_lock:
            if _kb_lookup is None:  # Double-check locking
                _kb_lookup = KBQuery()
    return _kb_lookup
```

---

## Document History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2026-03-29 | Claude | Initial specification |
| 1.1.0 | 2026-03-29 | Claude | Added testing, error handling, security, performance, monitoring, rollback, and concurrency appendices |