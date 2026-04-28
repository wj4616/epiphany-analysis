# Knowledge Harvest System Redesign

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Restructure the knowledge harvesting system to save raw JSON dumps to disk in designated directories, keeping full content preserved exactly (especially C++ code formatting), with proper indexing for skill access.

**Architecture:** Three separate knowledge bases (technical, sound-design, ui) stored as raw harvested files. Compiled playbook becomes a lightweight summary with workflow rules. Skills reference raw files via index.json cross-references.

**Tech Stack:** Firecrawl CLI for web harvesting, JSON for storage, bash scripts for automation, manifest files for progress tracking.

---

## Problem Statement

The current system has been integrating harvested content into the playbook JSON, corrupting the original data and losing the raw source material. The user needs:

1. Raw JSON dumps saved to disk - full content preserved
2. Separate directories per knowledge base type
3. No concern about disk space - save everything
4. C++ code formatting preserved exactly
5. Exhaustive crawling of JUCE official docs
6. Prioritized external resources for professional VST development
7. All Stanford CCRMA content harvested

---

## Directory Structure

```
/home/myuser/agents/juce-agent/playbookdata/
├── technical-kb/
│   ├── index.json                 # Master index with cross-references
│   ├── manifest.json              # Harvest progress tracking
│   ├── juce-official/
│   │   └── v7.0.12/
│   │       ├── classAudioProcessor.json
│   │       ├── classAudioProcessorEditor.json
│   │       ├── classAudioProcessorValueTreeState.json
│   │       ├── module-audio_processors.json
│   │       ├── module-dsp.json
│   │       └── ... (all classes, modules, tutorials)
│   ├── melatonin-dev/
│   │   └── blog/
│   │       ├── thread-safety-juce.json
│   │       ├── lookAndFeel-customization.json
│   │       └── ... (all posts)
│   ├── stanford-ccrma/
│   │   ├── courses/
│   │   │   ├── 320c-audio-dsp-projects.json
│   │   │   └── ... (all courses)
│   │   └── papers/
│   │       └── ... (all papers)
│   ├── github-awesome-juce/
│   │   └── linked-resources/
│   │       └── ... (harvested links)
│   └── additional-sources/
│       └── ... (discovered high-value resources)
├── sound-design-kb/
│   ├── index.json
│   ├── manifest.json
│   └── (future harvest)
├── ui-kb/
│   ├── index.json
│   ├── manifest.json
│   └── (future harvest)
└── compiled-playbooks/
    └── v7-unified.json             # Workflow + summary only (preserved as-is)
```

---

## File Format for Harvested Content

Each harvested JSON file contains:

```json
{
  "url": "https://docs.juce.com/master/classAudioProcessor.html",
  "title": "AudioProcessor Class Reference",
  "source": "juce-official",
  "juce_version": "7.0.12",
  "harvested_at": "2026-03-29T12:00:00Z",
  "content_type": "class_reference",
  "raw_content": {
    "markdown": "...",
    "html": "...",
    "links": [
      "https://docs.juce.com/master/classAudioProcessorEditor.html",
      "..."
    ]
  },
  "code_blocks": [
    {
      "language": "cpp",
      "code": "void processBlock (AudioBuffer<float>& buffer,\n                        MidiBuffer& midiMessages) override\n{\n    // Exact spacing preserved\n}",
      "line_count": 5
    }
  ],
  "metadata": {
    "word_count": 2500,
    "has_code_examples": true,
    "code_languages": ["cpp"]
  }
}
```

**Critical:** C++ code blocks preserve exact spacing, indentation, and line breaks. No reformatting.

---

## Index Structure

`technical-kb/index.json`:

```json
{
  "kb_type": "technical",
  "version": "1.0.0",
  "juce_target_version": "7.0.12",
  "last_updated": "2026-03-29T...",
  "sources": {
    "juce-official": {
      "status": "complete",
      "pages_harvested": 800,
      "version": "7.0.12",
      "harvest_completed": "2026-03-29T..."
    },
    "melatonin-dev": {
      "status": "complete",
      "pages_harvested": 45,
      "harvest_completed": "2026-03-29T..."
    },
    "stanford-ccrma": {
      "status": "complete",
      "pages_harvested": 150,
      "harvest_completed": "2026-03-29T..."
    }
  },
  "cross_references": {
    "AudioProcessor": [
      "juce-official/v7.0.12/classAudioProcessor.json",
      "melatonin-dev/blog/thread-safety-juce.json"
    ],
    "AudioProcessorEditor": [
      "juce-official/v7.0.12/classAudioProcessorEditor.json"
    ],
    "AudioProcessorValueTreeState": [
      "juce-official/v7.0.12/classAudioProcessorValueTreeState.json",
      "melatonin-dev/blog/apvts-patterns.json"
    ],
    "DSP": [
      "juce-official/v7.0.12/module-dsp.json",
      "stanford-ccrma/courses/320c-audio-dsp-projects.json"
    ],
    "ThreadSafety": [
      "melatonin-dev/blog/thread-safety-juce.json",
      "juce-official/v7.0.12/thread-safety-guide.json"
    ]
  },
  "topics": {
    "audio_processing": [...],
    "dsp": [...],
    "thread_safety": [...],
    "gui": [...],
    "state_management": [...]
  }
}
```

---

## Manifest Structure

`technical-kb/manifest.json`:

```json
{
  "harvest_id": "tech-kb-001",
  "started_at": "2026-03-29T00:00:00Z",
  "current_phase": 1,
  "phases": [
    {
      "name": "juce-official",
      "status": "in_progress",
      "started_at": "2026-03-29T00:00:00Z",
      "pages_completed": 45,
      "pages_total": 800,
      "last_url_processed": "https://docs.juce.com/master/classAudioProcessor.html",
      "failed_urls": [],
      "completed_at": null
    },
    {
      "name": "melatonin-dev",
      "status": "pending",
      "started_at": null,
      "pages_completed": 0,
      "pages_total": null,
      "last_url_processed": null,
      "failed_urls": [],
      "completed_at": null
    }
  ],
  "failed_urls": [],
  "resume_state": {
    "next_url": "https://docs.juce.com/master/classAudioProcessorEditor.html",
    "queue": [...]
  }
}
```

---

## Crawling Strategy

### Phase 1: JUCE Official Documentation (Exhaustive)

**Starting points:**
- `https://docs.juce.com/master/index.html` - Module index
- `https://docs.juce.com/master/classes.html` - Class hierarchy
- `https://juce.com/learn/tutorials/` - Official tutorials

**Crawl depth:** Unlimited within docs.juce.com and juce.com/learn

**Process:**
1. Map site to get all URLs
2. For each URL, scrape full content
3. Save to `juce-official/v7.0.12/`
4. Update manifest after each successful page
5. Resume from failure point if interrupted

**Expected pages:** 500-800

### Phase 2: Melatonin Blog (All Posts)

**Starting point:**
- `https://melatonin.dev/blog/` - Blog index

**Process:**
1. Get all blog post URLs from index
2. Scrape each post fully
3. Save to `melatonin-dev/blog/`

**Expected pages:** 30-50

### Phase 3: Stanford CCRMA (All Content)

**Starting points:**
- `https://ccrma.stanford.edu/courses/320c/` - Audio DSP Projects
- `https://ccrma.stanford.edu/~jos/` - Julius O. Smith III's resources
- `https://ccrma.stanford.edu/` - Main site (crawl relevant sections)

**Process:**
1. Crawl all course pages
2. Crawl all papers and reference materials
3. Save to `stanford-ccrma/courses/` and `stanford-ccrma/papers/`

**Expected pages:** 100-200

### Phase 4: awesome-juce GitHub Repository

**Starting point:**
- `https://github.com/sudara/awesome-juce` - Repository

**Process:**
1. Scrape repository README for all links
2. Categorize links (tutorials, libraries, tools)
3. Prioritize high-value resources (tutorials with docs, expert blogs)
4. Scrape prioritized resources
5. Save to `github-awesome-juce/linked-resources/`

**Expected pages:** 100-200 (prioritized)

### Phase 5: Additional Discovered Sources

During Phases 1-4, discover additional high-value resources and harvest them.

---

## Firecrawl Commands

### Site Mapping

```bash
firecrawl map "https://docs.juce.com/master/" \
  --search "class module tutorial example" \
  -o "technical-kb/juce-official-site-map.json" \
  --json \
  --pretty
```

### Individual Page Scrape

```bash
firecrawl scrape "https://docs.juce.com/master/classAudioProcessor.html" \
  --format markdown,html,links \
  -o "technical-kb/juce-official/v7.0.12/classAudioProcessor.json" \
  --json \
  --pretty
```

### Batch Processing Script

```bash
#!/bin/bash
# harvest-juce-docs.sh

SITE_MAP="technical-kb/juce-official-site-map.json"
OUTPUT_DIR="technical-kb/juce-official/v7.0.12"
MANIFEST="technical-kb/manifest.json"

# Read URLs from site map
urls=$(jq -r '.links[]' "$SITE_MAP")

for url in $urls; do
  filename=$(echo "$url" | sed 's/.*\///' | sed 's/\.html//')

  echo "Harvesting: $url"
  firecrawl scrape "$url" \
    --format markdown,html,links \
    -o "$OUTPUT_DIR/${filename}.json" \
    --json \
    --pretty

  # Update manifest
  jq --arg url "$url" '.phases[0].pages_completed += 1 | .phases[0].last_url_processed = $url' \
    "$MANIFEST" > "${MANIFEST}.tmp" && mv "${MANIFEST}.tmp" "$MANIFEST"

  sleep 0.5
done
```

---

## Rate Limiting and Error Handling

**Rate Limiting:**
- Firecrawl free tier: 2 concurrent requests
- Minimum delay: 0.5 seconds between requests
- On rate limit: wait and resume (do not restart)

**Error Handling:**
- Failed URLs logged in manifest
- Resume from last successful URL
- Atomic manifest updates (write to temp, then move)
- Never delete partial progress

---

## Integration with Skills

### Skill Reference Pattern

Skills load the index to find relevant content:

```markdown
## Using the Technical KB

1. Load `technical-kb/index.json`
2. Look up topic in `cross_references` or `topics`
3. Read relevant JSON files from paths
4. Extract code exactly as formatted in `code_blocks`
5. Return implementation details
```

### Compiled Playbook Role

The compiled playbook (`v7-unified.json`) contains:
- Workflow phases (12 development phases)
- Failure modes (FM-xx, EM-xx, CM-xx)
- Conflict resolution rules
- Sound design translations (summary only)
- UI design principles (summary only)
- References to raw KB files (not embedded content)

Skills check playbook for workflow/rules, then reference raw KB for implementation details.

---

## Success Criteria

1. **Completeness:** All JUCE 7.0.12 official documentation harvested
2. **Preservation:** C++ code formatting preserved exactly
3. **Indexing:** Cross-references enable finding related content
4. **Resumability:** Harvest can be interrupted and resumed
5. **Integration:** Skills can reference raw files via index
6. **Traceability:** Source URL and timestamp preserved in each file

---

## Estimated Scale

| Source | Expected Pages | Est. Size |
|--------|---------------|-----------|
| JUCE Official | 500-800 | 50-100 MB |
| Melatonin Blog | 30-50 | 3-5 MB |
| Stanford CCRMA | 100-200 | 10-20 MB |
| awesome-juce links | 100-200 | 10-20 MB |
| Additional sources | 50-100 | 5-10 MB |
| **Total** | **800-1500** | **80-155 MB** |

Disk space is not a concern - save everything.