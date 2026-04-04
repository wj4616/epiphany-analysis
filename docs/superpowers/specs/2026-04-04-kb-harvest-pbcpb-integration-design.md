# KB Harvest & PBCPB Integration Design Spec

**Date:** 2026-04-04
**Status:** Draft
**Scope:** PBCPB meta-system changes, kb-harvest skill replacement, KB infrastructure unification

---

## 1. Problem Statement

The PBCPB (Playbook Creator Playbook) meta-system generates domain-specific playbooks that include structured Knowledge Bases. The KB population pipeline was designed around Firecrawl (full web page extraction with API credits), but Firecrawl credits are unavailable. The system needs to work with free alternatives — primarily WebFetch (Claude built-in) combined with DDG or WebSearch for URL discovery.

Three critical gaps exist in the current system:

1. **All harvesting skills are Firecrawl-only.** kb-harvest, kb-autofill, and harvest-data all require `FIRECRAWL_API_KEY`. No free backend exists.
2. **PBCPB-generated KBs are thin.** The prototype KB at `agents/juce-agent/playbookdata/` has rich entries (13 code blocks per entry, full original markdown, 8500+ chars), while PBCPB-generated entries have 4 concepts and a paragraph. The PBCPB schema allows rich content — Phase 3 just doesn't extract it.
3. **PBCPB-generated manifests are broken.** Every per-layer manifest.json shows `entry_count: 0, entries: []` despite entries existing on disk. The Phase 3 auditor gate failed to enforce this.

Additionally, the KB system lacks:
- Multi-KB targeting (skills hardcode KB paths)
- Automatic index/cross-reference/bridge maintenance during harvesting
- Integration between harvesting skills and consumption skills (sound-design-bridge, dsp-implementation, etc.)

### Goals

1. PBCPB generates playbooks that produce implementation-ready KBs optimized for agent consumption
2. A unified kb-harvest skill replaces three Firecrawl-only skills with multi-backend support
3. Every harvest operation automatically maintains all KB infrastructure (manifests, master-index, cross-references, bridges)
4. The system is optimized for WebFetch data constraints while preserving C++ code quality
5. Multiple KBs can coexist and be targeted independently

### Non-Goals (Future Scope)

- Skill routing layer mapping agent behavior to KB access during playbook execution (see memory: `project_kb_skill_routing.md`)
- Migration of existing prototype KBs to unified schema
- A/B testing of extraction prompt quality

---

## 2. System Architecture

The KB ecosystem has three layers. PBCPB must generate playbooks that produce all three.

```
┌─────────────────────────────────────────────────┐
│  CONSUMPTION LAYER                              │
│  Skills that READ KB during execution           │
│  sound-design-bridge, dsp-implementation,       │
│  ui-bridge, daw-testing, plugin-spec            │
│                                                 │
│  Need: code_blocks, parameter ranges,           │
│  bridge mappings, original_markdown,            │
│  cross_references                               │
├─────────────────────────────────────────────────┤
│  MAINTENANCE LAYER                              │
│  Skills that MANAGE KB quality                  │
│  kb-validate (confidence scoring)               │
│  kb-sync (verification, repair, status promote) │
│                                                 │
│  Need: harvest_metadata, confidence scores,     │
│  manifest accuracy, master-index consistency     │
├─────────────────────────────────────────────────┤
│  POPULATION LAYER                               │
│  Skill that WRITES to KB                        │
│  kb-harvest (replaces kb-harvest, kb-autofill,  │
│  harvest-data)                                  │
│                                                 │
│  Need: search backends, extraction pipeline,    │
│  quality gates, cascade updates                 │
└─────────────────────────────────────────────────┘
```

### Generation Chain Constraint

All improvements must target the PBCPB meta-system at `/home/myuser/Documents/pbcpb/`. The generated playbook is a read-only reference — if PBCPB regenerates it, direct edits are overwritten.

```
PBCPB meta-system (what we modify)
  |  generates
  +-> playbook.json (read-only output)
        |  when executed, runs Phase 2 then Phase 3
        +-> KB architecture (schemas, layers, population strategy)
        +-> KB entries (placeholder -> harvested -> curated -> synced)
        +-> Bridge entries
        +-> Master index, manifests, cross-references
```

Every change in this spec traces: **PBCPB modification -> generated playbook change -> KB improvement**.

---

## 3. Backend Architecture

Five user-selectable backends, all producing schema-conformant entries through a common pipeline.

| Backend | Search Tool | Scrape Tool | Requirements | Quality Floor |
|---------|-------------|-------------|--------------|---------------|
| `ddg+webfetch` | DDG CLI (`ddg-search -f json`) | WebFetch | DDG CLI installed | 0.45 |
| `websearch+webfetch` | Claude WebSearch | WebFetch | None (built-in) | 0.40 |
| `webfetch` | None (user provides URLs) | WebFetch | None | 0.50 |
| `firecrawl` | Firecrawl search | Firecrawl scrape or WebFetch | API key + credits | 0.55 |
| `imported` | None (local files) | Format adapter + Claude extraction | None | 0.45 |

### Backend Selection Logic

- User specifies backend explicitly: `--backend ddg+webfetch`
- Default: `ddg+webfetch` if DDG CLI is installed, else `websearch+webfetch`
- Auto-fallback: if DDG fails (bot detection, 0 results), fall back to `websearch+webfetch` automatically
- `--urls` mode always uses `webfetch` backend regardless of --backend flag

### Quality Thresholds

Per-backend quality floors (configurable per-KB in population-strategy):

```
overall_confidence >= quality_floor       -> Accept, status = "harvested"
0.30 <= conf < quality_floor             -> Accept with review_flag = true
confidence < 0.30 (absolute minimum)     -> Discard, log reason
```

### DDG Integration Details

```bash
# Search with JSON output, extract URLs
ddg-search "[query]" -f json -n 10 | jq -r '.items[].link'

# With region and time filter
ddg-search -r us-en -t y "[query]" -f json -n 10

# Code-targeting queries
ddg-search "[topic] site:github.com C++ implementation" -f json -n 5
ddg-search "[topic] JUCE tutorial code example" -f json -n 5
```

DDG has built-in 800-2900ms delays between page fetches. Bot detection may trigger early stop — use whatever results are returned.

### WebSearch Integration Details

WebSearch (Claude built-in) returns markdown containing search results with inline links. URL extraction:

1. Call WebSearch with query
2. Parse the returned markdown for URLs — extract all `[text](url)` links and bare URLs
3. Filter out search engine chrome (google.com, bing.com, duckduckgo.com links)
4. Remaining URLs are the search result targets — pass to WebFetch

WebSearch has no JSON output mode. The skill extracts URLs via regex: `https?://[^\s\)\]]+` from the markdown response, deduplicated and filtered.

**WebSearch limitations vs DDG:**
- No result count control (DDG has `-n` flag)
- No region/time filtering (DDG has `-r` and `-t` flags)
- No structured output (DDG has `-f json`)
- Results may include more noise (search page elements mixed with results)
- Advantage: zero dependencies, always available

### WebFetch Integration Details

WebFetch is the universal content extraction layer. It converts HTML to markdown and processes with a small, fast model.

**Critical constraint:** WebFetch processes content with a "small, fast model" — NOT the main Claude model. Complex structured extraction prompts will not work reliably in WebFetch. Therefore, WebFetch is used ONLY for content fetching, not field extraction.

**Two-step extraction architecture:**
1. **WebFetch** (small model): Simple fetch prompt — get the content as markdown
2. **Claude** (main session, full model): Structured field extraction from the markdown

WebFetch prompt for content fetch:
```
Extract all content about [topic]. Preserve all code blocks with their 
original formatting exactly as they appear. Include parameter values, 
implementation details, and technical explanations. Return as markdown.
```

**Large page summarization:** WebFetch may summarize very large pages, potentially losing code blocks. Mitigation:
1. First pass: General content fetch (prompt above)
2. Check if result contains code blocks (look for ``` fences)
3. If none found but expected (code-oriented KB layer): second pass with targeted prompt:
   ```
   Extract specifically the code examples, parameter values, and 
   implementation patterns about [topic]. Preserve exact code formatting.
   ```
4. Merge results from both passes into `original_markdown`

**WebFetch limitations:**
- Cannot access authenticated/paywalled sites
- Cannot render JavaScript-heavy SPAs
- 15-minute cache per URL (useful for re-fetch with different prompts)
- HTTP auto-upgrades to HTTPS
- Redirects require manual follow-up with new URL

---

## 4. Entry Schema Specification

### Unified Entry Schema

The entry schema is defined per-KB in `architecture/entry-schema.json` (generated by PBCPB Phase 2). This section defines the target schema that PBCPB should generate. All fields from the current schema are preserved. New fields are additions.

#### New Required Fields

**`source.backend`** (string, required on all entries):
```json
"source": {
  "type": "web-harvested | research-doc | expert-knowledge | external-import",
  "reference": "Page title or document section name",
  "url": "...",
  "backend": "research-docs | firecrawl | ddg+webfetch | websearch+webfetch | webfetch | imported",
  "retrieved_date": "2026-04-04",
  "import_source": {
    "path": "/absolute/path/to/original/file",
    "format": "json-entries | markdown-dir | obsidian | csv | prototype | custom",
    "field_map_used": true,
    "import_date": "2026-04-04"
  }
}
```

`source.import_source` (object, present only on imported entries): tracks the original file path, detected format, whether a custom field map was applied, and import date. See Section 8.8 for full import provenance details.

`source.type` values for new entries:
- `"web-harvested"` — content fetched from web via any web backend
- `"research-doc"` — extracted from local research documents (Phase 3 initial harvest)
- `"expert-knowledge"` — manually authored bridge entries and curated content
- `"external-import"` — imported from an external KB via `--import`

`source.reference` is auto-populated: for web-harvested entries, set to the page `<title>` or first `<h1>` from fetched content. For research docs, set to document name and section heading. For imports, set to source filename and path.

For existing entries that lack `source.backend`, validators and skills infer `"research-docs"` as default. For entries lacking `source.type`, infer from `source.backend`: research-docs → `"research-doc"`, all web backends → `"web-harvested"`, imported → `"external-import"`. This is a non-breaking addition — old entries remain valid.

#### New Optional Fields

**`original_markdown`** (string, present on all non-placeholder entries):

The full source content in markdown format. For web-harvested entries, this is the WebFetch output. For research-doc entries, this is the source document section text. This is the most valuable field for agent consumption — agents can read complete tutorials, not just extracted fragments.

```json
"original_markdown": "# Subtractive Synthesis\n\n## Core Concepts\n\n..."
```

**`supersedes`** (string, present on re-harvested entries only):

The entry ID of the previous version this entry replaces. Used by re-harvest versioning (Section 8.7). When present, the superseded entry has been moved to `_archive/`. Allows tracing entry lineage.

```json
"supersedes": "dsp-kb_filters_biquad"
```

**`harvest_metadata`** (object, present on all non-research-doc entries — web-harvested and imported):

```json
"harvest_metadata": {
  "overall_confidence": 0.72,
  "field_provenance": {
    "description": { "method": "ai-synthesized", "confidence": 0.85 },
    "concepts": { "method": "ai-synthesized", "confidence": 0.80 },
    "code_blocks": { "method": "direct-extracted", "confidence": 0.95 },
    "difficulty": { "method": "ai-inferred", "confidence": 0.65 },
    "domain_relevance": { "method": "ai-inferred", "confidence": 0.60 }
  },
  "source_content_length_chars": 4500,
  "extraction_prompt_version": "1.0",
  "source_urls": ["https://...", "https://..."],
  "fetch_date": "2026-04-04",
  "backend_used": "ddg+webfetch",
  "review_flag": false
}
```

Field provenance `method` values:
- `"direct-extracted"` — taken verbatim from source (code blocks, URLs)
- `"direct-mapped"` — field copied 1:1 from a known-format import via format adapter
- `"ai-synthesized"` — generated by Claude from source content (descriptions, summaries)
- `"ai-inferred"` — estimated by Claude with limited basis (difficulty, domain_relevance)

#### Confidence Weight Configuration

Confidence weights are per-KB, defined in population-strategy (not global). Different KB domains weight fields differently:

```json
"confidence_weights": {
  "description": 0.25,
  "concepts": 0.15,
  "code_blocks": 0.25,
  "title": 0.05,
  "summary": 0.05,
  "tags": 0.05,
  "related_topics": 0.05,
  "difficulty": 0.05,
  "domain_relevance": 0.05,
  "cross_references": 0.05
}
```

For KBs without configured weights, the skill uses the defaults above. A DSP-focused KB might increase `code_blocks` to 0.35 and decrease `concepts` to 0.10.

`overall_confidence = sum(field_confidence * weight) * 0.85 + source_domain_weight * 0.15`

The formula weights field extraction quality at 85% and source authority at 15%. This prevents source_domain_weight from suppressing otherwise good extractions — a perfect extraction from an unknown domain (default_weight 0.5) scores `1.0 * 0.85 + 0.5 * 0.15 = 0.925`, not 0.50.

#### Source Domain Quality Ranking

Per-KB in population-strategy, with built-in defaults:

```json
"source_domain_rankings": {
  "academic": {
    "domains": ["ieee.org", "aes.org", "arxiv.org", "ccrma.stanford.edu"],
    "weight": 1.0
  },
  "official_docs": {
    "domains": ["docs.juce.com", "cppreference.com", "cmake.org"],
    "weight": 0.9
  },
  "expert_blogs": {
    "domains": ["earlevel.com", "musicdsp.org", "theaudioprogrammer.com"],
    "weight": 0.8
  },
  "code_repos": {
    "domains": ["github.com", "gitlab.com"],
    "weight": 0.7
  },
  "community": {
    "domains": ["kvraudio.com", "forum.juce.com", "stackoverflow.com"],
    "weight": 0.6
  },
  "default_weight": 0.5
}
```

### Bridge Schema Additions

Bridge entries (like `bridge_timbre_warm.json`) gain provenance tracking:

```json
{
  "...existing bridge fields...",
  "source": {
    "type": "expert-knowledge | auto-detected | web-harvested | external-import",
    "backend": "research-docs | ddg+webfetch | websearch+webfetch | webfetch | firecrawl | imported",
    "reference": "...",
    "url": "..."
  },
  "harvest_metadata": {
    "overall_confidence": 0.50,
    "...same structure as entry harvest_metadata..."
  }
}
```

Auto-detected bridge entries start at confidence 0.50 (vs manual bridges at 0.85). Promotion path:
- Corroborated by second source: +0.10
- Manually reviewed and approved: set to manual confidence level
- Successfully used in DAW test: +0.05 per success

### Code Preservation Requirements

All code blocks must survive the pipeline with exact original formatting:

1. **WebFetch HTML→markdown:** Code in `<pre><code>` tags converts to markdown fences. Verify fences have language tags.
2. **original_markdown storage:** Raw WebFetch output stored verbatim — code blocks preserved as-is.
3. **Claude extraction to code_blocks[]:** Extraction prompt MUST include: *"Preserve code blocks EXACTLY as they appear — do not reformat, fix, simplify, or modify any code. Copy verbatim including whitespace, indentation, and comments."*
4. **Entry validation:** Code blocks in `code_blocks[]` must match code blocks in `original_markdown`. Any mismatch indicates extraction error.

---

## 5. Master Index & Cross-Referencing System

### Unified Master Index Format

The prototype master-index format (at `agents/juce-agent/playbookdata/master-index.json`) is adopted as the standard. PBCPB Phase 3 must generate master indexes in this format.

Structure:

```json
{
  "kb_name": "master-index",
  "version": "1.0.0",
  "knowledge_bases": {
    "<kb-name>": {
      "path": "<relative-or-absolute-path>",
      "description": "...",
      "source": "harvested+curated | curated | placeholder",
      "status": "ready | building | error",
      "file_count": 53,
      "topics": ["topic1", "topic2", "..."],
      "layers": {
        "<layer-name>": {
          "authority": 0.85,
          "entries": 14
        }
      }
    }
  },
  "cross_references": {
    "<concept>": {
      "description": "...",
      "kbs": ["kb1", "kb2"],
      "topics": ["topic1", "topic2"]
    }
  },
  "last_updated": "2026-04-04T00:00:00Z"
}
```

**Cross-references** map concepts to which KBs contain relevant content. This is the primary lookup mechanism for agents:
- Agent needs information about "filter design" → checks cross_references["filter"] → finds [dsp-kb, juce-kb, sound-design-kb]
- Agent needs bridge for "warm" → checks cross_references["warm"] → finds [sound-design-kb, playbook-kb]

**PBCPB-generated master-index must match this format.** The current PBCPB format (`kb_layers[]` + `cross_layer_mappings[]`) is replaced.

### Unified Manifest Format

Per-KB manifest.json adopts the prototype format with additions:

```json
{
  "kb_name": "<name>",
  "version": "1.0.0",
  "created": "...",
  "last_sync": "...",
  "status": "ready",
  "topics": [
    {
      "name": "<topic>",
      "files": {
        "<filename>.json": {
          "status": "placeholder | harvested | curated | synced",
          "harvested_at": "...",
          "synced_at": "...",
          "synced_timestamp": 1774873496,
          "has_semantic": true,
          "source": "ddg+webfetch | websearch+webfetch | webfetch | firecrawl | research-docs | imported",
          "markdown_length": 8532,
          "code_block_count": 13,
          "confidence": 0.72,
          "review_flag": false
        }
      }
    }
  ]
}
```

New fields vs prototype: `code_block_count`, `confidence`, `review_flag`.

### Cross-Reference Maintenance

Cross-references are maintained automatically by kb-harvest during the cascade (Section 8). The maintenance algorithm:

**Master-index cross_references** (maintained by kb-harvest cascade):

1. For each new/updated entry, read its `tags[]`, `concepts[].name`, and `related_topics[]`
2. For each term found:
   a. Look up term in master-index `cross_references`
   b. If exists: add this entry's KB to the `kbs[]` array if not already present
   c. If doesn't exist: create new cross_reference entry with this KB as first reference

**Entry-level cross_references[]** (maintained by kb-sync, NOT cascade):

Entry-level cross_references require scanning entries in other registered KBs to find exact concept name matches — this is O(N) per other KB and risks lock conflicts with concurrent harvests. Therefore, entry-level cross_references are populated only by `kb-sync --repair`, which reads all registered KBs safely during its verification pass. Rule: only populate when a concept name in one entry exactly matches a concept name in an entry in another registered KB. Don't infer from tags alone.

### Bridge Auto-Detection

During harvesting of bridge-eligible KB layers (sound-design, effects — specified in population-strategy), the extraction prompt includes bridge detection:

```
From this content, also extract any mappings between sound descriptions 
or subjective terms and specific DSP parameter settings. For each found:
- descriptor: the subjective quality (warm, bright, punchy, etc.)
- parameter: which DSP parameter (filter_cutoff, resonance, etc.)  
- value_range: [min, max] recommended range
- typical_default: common starting value
- rationale: why this mapping works
- anti_patterns: what NOT to do

Only extract explicit mappings with specific parameter values mentioned 
in the source. Do not invent mappings from general descriptions.
```

Auto-detected bridges:
- Start at confidence 0.50
- Source type: `"auto-detected"`
- Get `review_flag: true` in manifest
- Are stored in the bridge layer alongside manual bridges
- Bridge combination scan runs after creation (same-category bridges only)

---

## 6. KB Registry System

### Registry File

Location: `~/.claude/kb-registry.json`

```json
{
  "version": "1.0.0",
  "registries": [
    {
      "name": "vst-product-lifecycle",
      "path": "/home/myuser/playbooks/vst-product-lifecycle-playbook/kb",
      "schema_path": "../architecture/entry-schema.json",
      "bridge_schema_path": "../architecture/bridge-schema.json",
      "master_index_path": "master-index.json",
      "generated_by": "pbcpb",
      "layers": ["technical", "sound-design", "ui-ux", "commercial", "reference", "bridge"],
      "bridge_eligible_layers": ["sound-design"],
      "default_backend": "ddg+webfetch",
      "registered_at": "2026-04-04T00:00:00Z"
    },
    {
      "name": "juce-agent-prototype",
      "path": "/home/myuser/agents/juce-agent/playbookdata",
      "schema_path": null,
      "master_index_path": "master-index.json",
      "generated_by": "manual",
      "layers": ["dsp-kb", "sound-design-kb", "juce-kb", "ui-kb", "cmake-kb", "midi-kb", "testing-kb", "cpp-kb"],
      "bridge_eligible_layers": ["sound-design-kb"],
      "default_backend": "ddg+webfetch",
      "registered_at": "2026-04-04T00:00:00Z"
    }
  ],
  "default_kb": "vst-product-lifecycle"
}
```

### Path Convention

- `path`: always absolute (the KB root directory)
- `schema_path`, `bridge_schema_path`, `master_index_path`: always relative to `path`
- Skills resolve: `path + "/" + schema_path` to get the absolute schema location

### Required vs Optional Fields

| Field | Required | Notes |
|-------|----------|-------|
| `name` | yes | Unique identifier for this KB |
| `path` | yes | Absolute path to KB root |
| `master_index_path` | yes | Relative to path |
| `generated_by` | yes | `"pbcpb"` or `"manual"` |
| `layers` | yes | Array of layer names |
| `default_backend` | yes | Default harvesting backend |
| `registered_at` | yes | ISO timestamp |
| `schema_path` | no | null for prototype/manual KBs — skill uses built-in default |
| `bridge_schema_path` | no | null if no bridge layer |
| `bridge_eligible_layers` | no | Empty array or absent if no bridge detection needed |

### Registry Behavior

- **kb-harvest** reads registry to resolve `--kb <name>` to filesystem paths
- **Consumption skills** read registry to find KB locations (replacing hardcoded paths)
- If registry doesn't exist, kb-harvest creates it on first run
- If `--kb <name>` not found, skill lists registered KBs and asks user
- KBs without `schema_path` (prototype KBs): skill uses built-in default schema for harvesting

### PBCPB Registration

PBCPB Phase 3 includes a new task to register the KB after creation (see Section 7.3).

---

## 7. PBCPB Meta-System Changes

All changes target `/home/myuser/Documents/pbcpb/`. File references use paths relative to that directory.

### 7.1 Output Schema Changes

**File:** `templates/output-schema.json`

#### 7.1.1 knowledge_base.population_strategy (lines 178-186)

**Current:**
```json
"population_strategy": {
  "type": "object",
  "properties": {
    "placeholder_seeding": { "type": "string" },
    "harvesting_sources": { "type": "array", "items": { "type": "string" } },
    "curation_rules": { "type": "string" },
    "sync_rules": { "type": "string" },
    "versioning_protocol": { "type": "string" }
  }
}
```

**Changed to:**
```json
"population_strategy": {
  "type": "object",
  "properties": {
    "placeholder_seeding": { "type": "string" },
    "harvesting_sources": {
      "oneOf": [
        { "type": "array", "items": { "type": "string" } },
        {
          "type": "object",
          "properties": {
            "research_documents": { "type": "array", "items": { "type": "string" } },
            "web_backends": {
              "type": "array",
              "items": {
                "type": "object",
                "required": ["name", "search_tool", "scrape_tool"],
                "properties": {
                  "name": { "type": "string" },
                  "search_tool": { "type": "string" },
                  "scrape_tool": { "type": "string" },
                  "requirements": { "type": "array", "items": { "type": "string" } },
                  "quality_floor": { "type": "number", "minimum": 0, "maximum": 1 }
                }
              }
            }
          }
        }
      ]
    },
    "curation_rules": { "type": "string" },
    "sync_rules": { "type": "string" },
    "versioning_protocol": { "type": "string" },
    "search_term_strategy": { "type": "string" },
    "confidence_weights": {
      "type": "object",
      "additionalProperties": { "type": "number" }
    },
    "source_domain_rankings": { "type": "object" },
    "bridge_eligible_layers": { "type": "array", "items": { "type": "string" } },
    "import_adapters": {
      "type": "array",
      "items": {
        "type": "object",
        "properties": {
          "format": { "type": "string" },
          "description": { "type": "string" },
          "field_map_path": { "type": "string" }
        }
      },
      "description": "Supported import formats for this KB. Built-in adapters (prototype, json-entries, markdown-dir, obsidian, csv) are always available. List custom adapters with field_map paths here."
    },
    "skill_references": {
      "type": "object",
      "properties": {
        "harvesting": { "type": "string" },
        "sync": { "type": "string" },
        "validation": { "type": "string" }
      }
    }
  }
}
```

`harvesting_sources` uses `oneOf` to accept EITHER the old string array (backward compatible) OR the new object format (for new playbooks). Existing playbooks remain valid.

#### 7.1.2 knowledge_base.entry_schema (line 176)

**Current:** `"entry_schema": { "type": "object" }`

**Unchanged.** The output-schema stays as a bare object — entry schema details are enforced by Phase 2 gate conditions rather than by output-schema rigidity. This allows different domains to have different entry schemas while ensuring required fields through gate checks.

#### 7.1.3 knowledge_base — new field: kb_registry

Add after `directory_structure`:
```json
"kb_registry": {
  "type": "object",
  "properties": {
    "name": { "type": "string" },
    "default_backend": { "type": "string" },
    "bridge_eligible_layers": { "type": "array", "items": { "type": "string" } }
  }
}
```

#### 7.1.4 phase_kb_mapping (line 423)

No schema change needed (it's already flexible). But PBCPB Phase 7 task descriptions must instruct the architect to map bridge layer access to relevant phases, and include web-harvested KB layers in the mapping.

#### 7.1.5 skill_activation (line 427)

No schema change needed. But PBCPB Phase 7 must include:
```json
"kb-harvest": "Phase 3 web harvesting and ongoing KB population",
"kb-sync": "Post-harvest verification and curated->synced promotion",
"kb-validate": "Post-harvest claim validation and pre-implementation gate"
```

### 7.2 Phase 2 Changes (KB Architecture)

**File:** `playbook-creator-playbook.json`, Phase 2 section (lines 652-805)

#### 7.2.1 Task 2 — Define knowledge layers (line 727)

**Add to description:**
```
Prefer granular layers over broad categories. The question is not just
"do knowledge types differ?" but "can an agent find what it needs 
without searching through unrelated content?"

A single "technical" layer spanning DSP algorithms, build systems, 
and language reference is too broad. Split into independently queryable 
domains where each layer serves a distinct agent need.

Example: instead of one "technical" layer, consider separate layers for
DSP algorithms, JUCE framework patterns, C++ audio patterns, and build 
system configuration — each independently searchable.
```

#### 7.2.2 Task 3 — Define KB entry schema (line 733)

**Add to description:**
```
Source provenance (required for all entries):
- source.backend: which harvesting backend produced this entry
  (research-docs | firecrawl | ddg+webfetch | websearch+webfetch | webfetch | imported)
  Default for existing entries without this field: "research-docs"
- source.url: source URL (required for web-harvested entries)
- source.retrieved_date: ISO date when content was fetched

Original content (required for all non-placeholder entries):
- original_markdown: full source content in markdown format. For web-harvested
  entries this is the WebFetch output. For research-doc entries this is the
  source document section text. This is the most valuable field for agent 
  consumption — agents can read complete tutorials and reference material,
  not just extracted fragments.

Harvest metadata (required for web-harvested and imported entries):
- harvest_metadata.overall_confidence: 0.0-1.0 composite score
- harvest_metadata.field_provenance: per-field {method, confidence}
  where method is "direct-extracted" | "direct-mapped" | "ai-synthesized" | "ai-inferred"
- harvest_metadata.source_content_length_chars: character count of source content
- harvest_metadata.extraction_prompt_version: version string
- harvest_metadata.source_urls: array of URLs content was fetched from
- harvest_metadata.fetch_date: ISO date
- harvest_metadata.backend_used: which backend was used
- harvest_metadata.review_flag: boolean, true if confidence near threshold

Entry schema must be designed for agent consumption — not reference storage.
Entries should include implementation-ready content:
- code_blocks with working examples (not pseudocode) where domain involves code
- Parameter ranges with specific values in description field
- cross_references linking to related entries across KB layers
- original_markdown preserving full source context

Code preservation: code_blocks must preserve exact original formatting.
C++ code examples must remain valid, compilable code — no reformatting,
simplification, or modification during extraction.
```

#### 7.2.3 Task 4 — Define bridge entry schema (line 740)

**Add to description:**
```
Bridge entries from web harvesting gain provenance tracking:
- source.type: "expert-knowledge" | "auto-detected" | "web-harvested" | "external-import"
- source.backend: same enum as entry schema
- harvest_metadata: same structure as entry schema (for web-sourced bridges)

Auto-detected bridges (found during web harvesting of bridge-eligible layers) 
start at confidence 0.50 with source.type "auto-detected" and review_flag true.
Manual bridges retain their existing confidence levels.
```

#### 7.2.4 Task 5 — Define population strategy (line 746)

**Add to description:**
```
Web harvesting backends (define all available for this KB):
For each backend specify: name, search_tool, scrape_tool, requirements,
and quality_floor (minimum overall_confidence to accept from this backend).

Standard backends:
1. ddg+webfetch — DDG CLI search + WebFetch scrape. Requires DDG CLI.
   quality_floor: 0.45
2. websearch+webfetch — Claude WebSearch + WebFetch. No requirements.
   quality_floor: 0.40
3. webfetch — User provides URLs, WebFetch scrapes. No requirements.
   quality_floor: 0.50 (higher because user curated the source)
4. firecrawl — Firecrawl search + scrape. Requires API key + credits.
   quality_floor: 0.55
5. imported — Local files via format adapter + Claude extraction. No requirements.
   quality_floor: 0.45

Confidence weights: define per-field weights for overall_confidence 
calculation. Weight fields by importance to agent consumption in this domain.
Default weights: description 0.25, concepts 0.15, code_blocks 0.25,
title 0.05, summary 0.05, tags 0.05, related_topics 0.05, 
difficulty 0.05, domain_relevance 0.05, cross_references 0.05.
All weighted fields correspond to extraction prompt fields (Section 8.2).

Source domain rankings: define quality tiers for web sources relevant 
to this KB's domain. Academic and official documentation sources rank 
highest. Include specific domain names where possible.

Bridge-eligible layers: list which KB layers may contain content that 
maps subjective descriptors to technical parameters. Only these layers
trigger bridge auto-detection during harvesting.

Search term strategy: define how KB topics and placeholder entries are
expanded into search queries from multiple perspectives:
- Academic: '[topic] algorithm', '[topic] computational method', '[topic] IEEE paper'
- Practitioner: '[topic] implementation', '[topic] best practices', '[topic] production code'
- Educator: '[topic] tutorial advanced', '[topic] deep dive'
- Domain expert: domain-specific terminology variants, alternative names
Include code-targeting queries: '[topic] C++ source code', 
'[topic] JUCE tutorial code example', '[topic] site:github.com'

Import adapters: list any custom import formats this KB needs beyond 
the built-in adapters (prototype, json-entries, markdown-dir, obsidian, csv).
For each custom adapter, provide a field mapping file path.

Skill references: specify which skills execute each population task:
- Harvesting + import: kb-harvest skill
- Sync/verification: kb-sync skill  
- Validation: kb-validate skill
```

#### 7.2.5 Task 6 — Define index and directory structure (line 753)

**Add to description:**
```
Master index format: use concept-level cross-referencing format.
The master-index maps concepts to which KBs contain relevant content.
Structure: knowledge_bases{} listing each KB with path, description,
source, status, file_count, topics, and layer authority scores.
cross_references{} mapping each concept/tag to which KBs contain it.

Per-KB manifest format: per-file status tracking with metadata.
Each file entry in the manifest must include: status, harvested_at,
synced_at, has_semantic, source (backend used), markdown_length,
code_block_count, confidence, review_flag.

Manifests MUST accurately reflect actual files on disk. The Phase 3
auditor will verify this with automated file counting.
```

#### 7.2.6 New Task — Define KB access specification (after Task 6)

```json
{
  "title": "[Architect] — Define KB access specification",
  "owner": "[Architect]",
  "description": "Define how consumption skills find and read KB entries. This specification replaces hardcoded KB paths in skills.\n\nSpecify:\n- KB registry entry format (name, path, schema, layers, bridge_eligible_layers)\n- Entry lookup pattern: how a skill resolves a query to specific KB entries via master-index cross_references\n- Cross-layer query pattern: how a skill follows cross_references[] across KB layers to find related content\n- Bridge lookup pattern: how a skill finds bridge entries for a given descriptor via master-index\n- Fallback behavior: when a KB entry is placeholder or below confidence threshold, invoke kb-harvest --auto --kb <name> --entry <id>\n- Multi-KB resolution: when multiple registered KBs contain relevant content for a query, which takes precedence (by authority_score)",
  "output": "kb-access-spec.md"
}
```

#### 7.2.7 Phase 2 Gate Conditions (lines 761-770)

**Add to gate_conditions array:**
```json
"Entry schema includes source.backend, original_markdown, and harvest_metadata fields",
"Population strategy defines at least one web harvesting backend with quality_floor",
"Population strategy includes confidence_weights and source_domain_rankings",
"Population strategy identifies bridge-eligible layers",
"Population strategy references kb-harvest, kb-sync, and kb-validate skills",
"KB access specification defined for consumption skills",
"KB registry entry format defined",
"Master-index format uses concept-level cross-referencing (not layer-level only)",
"Per-KB manifest format includes per-file status, markdown_length, code_block_count, confidence"
```

### 7.3 Phase 3 Changes (KB Bootstrapping)

**File:** `playbook-creator-playbook.json`, Phase 3 section (lines 807-989)

#### 7.3.1 New Task — Register KB in system registry (after Task 1, directory creation)

```json
{
  "title": "[Builder] — Register KB in system registry",
  "owner": "[Builder]",
  "description": "Register this KB in ~/.claude/kb-registry.json using the format defined in kb-access-spec.md.\n\nIf ~/.claude/kb-registry.json does not exist, create it with version 1.0.0 and empty registries array.\n\nAdd or update entry with: name (from playbook title, slugified), path (absolute path to kb/ directory), schema_path (path to entry-schema.json), bridge_schema_path (if applicable), master_index_path, generated_by ('pbcpb'), layers (from kb-architecture.md), bridge_eligible_layers (from population-strategy.md), default_backend (from population-strategy.md).\n\nVerify: registry entry has all required fields and paths resolve to existing files/directories.",
  "output": "Updated ~/.claude/kb-registry.json"
}
```

#### 7.3.2 Modified Task — Harvest from research documents (line 903)

**Add to existing description:**
```
Harvested entries MUST include:
- original_markdown: the full text of the source section being harvested
- code_blocks with working implementation examples (not pseudocode) where
  the domain involves code. An entry without code_blocks in a code-oriented 
  KB layer is incomplete — search the research documents for code examples
  related to each topic.
- Parameter ranges with specific values in description field
- source.backend set to "research-docs"

After harvesting, count remaining placeholder entries per layer. These 
placeholders are candidates for web harvesting. Record the count in 
harvest-status.json per the format defined in Section 7.3.3 (track 
research_harvested, web_harvested, imported, placeholder, and failed per layer).
```

#### 7.3.3 New Task — Generate search terms for web harvesting (after research harvest)

```json
{
  "title": "[Researcher] — Generate search terms for web harvesting",
  "owner": "[Researcher]",
  "description": "For each placeholder entry remaining after research document harvesting, generate diverse search terms to maximize discovery of high-quality, implementation-ready content.\n\nFor each placeholder, generate queries from at least 4 perspectives:\n- Academic: '[topic] algorithm', '[topic] computational method', '[topic] IEEE/AES paper'\n- Practitioner: '[topic] implementation C++', '[topic] best practices', '[topic] production code'\n- Educator: '[topic] tutorial advanced', '[topic] explained in depth'\n- Domain expert: domain-specific terminology variants, alternative names for the same concept\n\nInclude code-targeting queries:\n- '[topic] C++ source code example'\n- '[topic] JUCE tutorial code'\n- '[topic] site:github.com implementation'\n\nPrioritize queries likely to surface content with actual code examples, parameter ranges, and implementation detail — not just conceptual descriptions.\n\nOutput: search-terms.json mapping each placeholder entry ID to its generated search queries with perspective labels. Store alongside KB directory for reuse by kb-harvest skill.",
  "output": "search-terms.json"
}
```

**search-terms.json format:**
```json
{
  "version": "1.0.0",
  "generated_at": "2026-04-04T10:00:00Z",
  "entries": {
    "dsp-kb_filters_biquad": {
      "status": "pending",
      "queries": [
        { "perspective": "academic", "query": "biquad filter algorithm digital signal processing", "used": false },
        { "perspective": "academic", "query": "biquad filter transfer function IEEE", "used": false },
        { "perspective": "practitioner", "query": "biquad filter C++ implementation real-time audio", "used": false },
        { "perspective": "educator", "query": "biquad filter tutorial explained in depth", "used": false },
        { "perspective": "domain_expert", "query": "second order IIR filter audio DSP", "used": false },
        { "perspective": "code", "query": "biquad filter JUCE tutorial code example", "used": false },
        { "perspective": "code", "query": "biquad filter site:github.com C++ implementation", "used": false }
      ]
    }
  }
}
```

**harvest-status.json format:**
```json
{
  "version": "1.0.0",
  "last_updated": "2026-04-04T12:00:00Z",
  "layers": {
    "dsp-kb": {
      "total": 25,
      "research_harvested": 12,
      "web_harvested": 8,
      "imported": 0,
      "placeholder": 5,
      "failed": {
        "count": 2,
        "entries": {
          "dsp-kb_reverb_schroeder": { "reason": "all URLs returned empty content", "last_attempt": "2026-04-04" },
          "dsp-kb_dynamics_vca": { "reason": "below 0.30 confidence threshold", "last_attempt": "2026-04-04" }
        }
      }
    }
  }
}
```

#### 7.3.4 New Task — Web harvest to fill KB gaps (after search terms)

```json
{
  "title": "[Researcher] — Web harvest to fill KB gaps",
  "owner": "[Researcher]",
  "description": "Using search-terms.json and web harvesting backends from population-strategy.md, fill remaining placeholder entries.\n\nUser selects backend. If no preference, use default from population-strategy.md. Invoke kb-harvest skill:\n  kb-harvest --kb <this-kb-name> --auto --backend <backend>\n\nThe skill handles: search, fetch via WebFetch, extraction via Claude, quality gating, entry writing, and cascade updates (manifest, master-index, cross-references, bridge detection).\n\nIf DDG backend fails (bot detection, 0 results), skill auto-falls back to websearch+webfetch.\n\nIf kb-harvest skill is unavailable, fall back to manual process:\n1. Use WebSearch to find URLs for each topic\n2. Use WebFetch to retrieve content from each URL\n3. Extract structured fields manually in this session\n4. Write entries following entry-schema.json\n5. Update manifests and master-index manually\n\nEXECUTION: Work one KB layer at a time. Update manifest after each layer.\n\nCRITICAL: Preserve all code blocks with exact original formatting. C++ code examples must remain valid.",
  "output": "kb/ web-harvested entries, updated manifests"
}
```

#### 7.3.5 Modified Task — Validate file paths and schema conformance (line 917)

**Add to existing description:**
```
Additional validation for web-harvested entries:
5. harvest_metadata presence: every non-research-doc entry (source.backend != "research-docs",
   i.e. web-harvested and imported entries) must have harvest_metadata with overall_confidence, 
   field_provenance, and extraction_prompt_version.
6. Code block integrity: for entries with code_blocks[], verify code blocks also appear in 
   original_markdown (if present). Any code_block not found in original_markdown may indicate 
   extraction error.
7. Manifest accuracy: for each manifest.json, count actual .json files in directory (excluding 
   manifest.json itself) and compare to sum of entry counts in manifest. Report any mismatch.
   Automated check:
   for manifest in kb/*/manifest.json; do
     dir=$(dirname "$manifest")
     actual=$(find "$dir" -name "*.json" ! -name "manifest.json" -maxdepth 2 | wc -l)
     # Compare against manifest entries
   done
8. Master-index consistency: verify master-index knowledge_bases entry counts match actual 
   per-KB file counts. Verify cross_references map to KBs that actually contain relevant entries.
```

#### 7.3.6 Phase 3 Gate Conditions (lines 926-937)

**Add to gate_conditions array:**
```json
"KB registered in ~/.claude/kb-registry.json with valid path, schema, and layer references",
"Web-harvested entries include harvest_metadata with overall_confidence >= backend quality_floor",
"Entries with confidence near threshold have review_flag = true",
"No entry accepted with overall_confidence below 0.30",
"search-terms.json exists with queries for all web-harvested entries",
"Per-KB manifest.json entry counts match actual file counts on disk (zero mismatches)",
"Master-index cross_references populated for all concepts appearing in 2+ KB layers",
"All code_blocks preserve original formatting (no reformatting or simplification detected)"
```

**Modify existing condition:**
```
BEFORE: "All harvestable research content has been captured in KB entries — zero skipped harvestable items"
AFTER:  "All harvestable content captured from research documents — zero skipped items. Web harvest attempted for all remaining placeholders. Entries that could not be filled documented in harvest-status.json with reason."
```

#### 7.3.7 Phase 3 Handoff (lines 959-986)

**Add to output_artifacts:**
```json
"search-terms.json",
"harvest-status.json",
"~/.claude/kb-registry.json (updated)"
```

#### 7.3.8 New Failure Mode

**Add to failure_modes array in PBCPB:**
```json
{
  "id": "FM-020",
  "symptom": "Web-harvested KB entries contain plausible but inaccurate synthesized content that passes quality thresholds",
  "root_cause": "AI synthesis from web content produces confident-sounding but incorrect field values, especially domain_relevance and difficulty assessments. Single-source entries are most vulnerable.",
  "fix": "Human review of entries with review_flag=true. Cross-reference synthesized claims against multiple sources. Lower confidence scores for single-source entries.",
  "prevention": "Track field_provenance per entry — fields with method 'ai-inferred' at low confidence get flagged. Include extraction_prompt_version in harvest_metadata for reproducibility. Bridge auto-detection starts at 0.50 confidence, not 0.85.",
  "phase": "Phase 3",
  "severity": "degraded",
  "source": "web-harvesting-design"
}
```

```json
{
  "id": "FM-021",
  "symptom": "PBCPB-generated manifests show zero entries despite KB files existing on disk",
  "root_cause": "Phase 3 bootstrapping creates entries but does not update manifests, or updates manifests before entries are written. The auditor gate does not run automated file-count verification.",
  "fix": "Run kb-harvest --rebuild-manifest to reconstruct manifests from actual files. Or manually count files and update manifest entries.",
  "prevention": "Phase 3 auditor MUST run automated file-count verification: compare actual JSON file count per directory against manifest entry counts. Gate fails on any mismatch.",
  "phase": "Phase 3",
  "severity": "error",
  "source": "audit-finding-2026-04-04"
}
```

### 7.4 Phase 7 Changes (Output Configuration)

PBCPB Phase 7 assembles the output playbook's configuration sections.

**phase_kb_mapping must include:**
- Bridge layer mapped to phases that use sound-design-to-technical translation (typically DSP implementation, DAW testing, plugin spec)
- All KB layers mapped to their consuming phases
- Web-harvested KB layers included in mapping

**skill_activation must include:**
```json
"kb-harvest": "Phase 3 web harvesting and ongoing KB population",
"kb-sync": "Post-harvest verification and curated->synced status promotion",
"kb-validate": "Post-harvest claim validation and pre-implementation confidence gate"
```

---

## 8. kb-harvest Skill Design

### Overview

Replaces: `~/.claude/skills/kb-harvest/`, `~/.claude/skills/kb-autofill/`, `~/.claude/skills/harvest-data/`

Location: `~/.claude/skills/kb-harvest/SKILL.md`

The unified harvesting skill for all KB population operations. Multi-backend, multi-KB, WebFetch-native. Every harvest operation triggers a cascade that maintains all KB infrastructure automatically.

### Invocation

```
kb-harvest --kb <name>                              # Interactive harvest session
kb-harvest --kb <name> --auto                       # Auto-fill all placeholders
kb-harvest --kb <name> --auto --refresh             # Re-harvest entries below confidence threshold
kb-harvest --kb <name> --auto --all                 # Re-harvest all entries
kb-harvest --kb <name> --urls <url1> <url2> ...     # Harvest specific URLs
kb-harvest --kb <name> --backend <backend>          # Specify backend
kb-harvest --kb <name> --layer <layer>              # Target specific KB layer
kb-harvest --kb <name> --topic <topic>              # Target specific topic
kb-harvest --kb <name> --entry <entry-id>           # Target specific entry
kb-harvest --kb <name> --batch <N>                  # Set batch size (default 5)
kb-harvest --kb <name> --resume                     # Resume from checkpoint
kb-harvest --kb <name> --import <source-path>        # Import external KB (auto-detect format)
kb-harvest --kb <name> --import <path> --format <f> # Import with explicit format
kb-harvest --kb <name> --import <path> --field-map <mapping.json>  # Import with custom mapping
kb-harvest --kb <name> --import <path> --dry-run    # Preview import without writing
kb-harvest --kb <name> --rebuild-manifest           # Reconstruct manifests from files
kb-harvest --kb <name> --review                     # Show staged entries awaiting approval
kb-harvest --status                                 # Show harvest status across all KBs
kb-harvest --list                                   # List registered KBs
```

**Defaults:**
- `--kb`: required (no default — user must specify target)
- `--backend`: `ddg+webfetch` if DDG CLI installed, else `websearch+webfetch`
- `--batch`: 5
- Without `--auto` or `--urls`: interactive mode

### Mode Details

**Interactive mode (default):**
1. Show KB status: layers, placeholder count, harvest status
2. Show placeholders needing content (from manifest, or directory scan if manifest broken)
3. User selects entries to fill (or all)
4. Generate search terms, display for user to edit/add
5. Search → show ranked results → user selects URLs
6. Fetch → extract → show entry for approval
7. User approves → store and cascade

**Auto mode (`--auto`):**
1. Find all placeholder entries (manifest scan, directory scan fallback)
2. Generate search terms from entry topics (4 perspectives + code-targeting)
3. Search → rank by source quality → fetch top URLs
4. Extract → score → gate
5. Store passing entries → cascade
6. Report results: N filled, N flagged for review, N failed

**Auto refresh mode (`--auto --refresh`):**
1. Find entries with `confidence < 0.60` (or configurable threshold)
2. Re-harvest with fresh search terms
3. New entry supersedes old (version bump, archive old)

**Auto all mode (`--auto --all`):**
1. Re-harvest ALL non-placeholder entries regardless of confidence
2. Same as `--auto --refresh` but targets every entry, not just low-confidence ones
3. Useful for bulk re-harvesting after extraction prompt improvements or when switching backends

**URL mode (`--urls`):**
1. WebFetch each provided URL
2. Extract → score → gate
3. Match to existing placeholder or prompt user for entry target
4. Store → cascade

**Import mode (`--import <source-path>`):**
1. Scan source path — list files, detect directory structure
2. Detect or use specified format (see Section 8.8 for format adapters)
3. Preview: show user N files found, detected format, proposed target layer(s)
4. For each source file:
   a. Read source content
   b. Apply format adapter (known format → direct field mapping) or Claude extraction (unknown format)
   c. Store source content as `original_markdown`
   d. Set `source.backend = "imported"`, `source.type = "external-import"`
   e. Generate `harvest_metadata` with `field_provenance` per field (`"direct-mapped"` for adapter fields, `"ai-synthesized"` for Claude-generated fields)
5. Validate against target KB's entry-schema.json
6. Quality gate (per-KB quality_floor for `imported` backend, default 0.45)
7. Show import summary: N accepted, N flagged, N rejected — user approves
8. Write entries to target KB
9. Cascade (see Section 8.1 — same cascade as web harvest)

`--dry-run` stops after step 6 and reports what would be imported without writing.

### Harvest Pipeline (Single Entry)

```
1.  RESOLVE: Read kb-registry.json → resolve KB path
2.  SCHEMA:  Read entry-schema.json → know target fields
             (if no schema: use built-in default)
3.  MANIFEST: Read manifest → find target entry
              (if manifest broken: --rebuild-manifest first)
4.  TERMS:   Load search-terms.json → get/generate search queries
             (if no search-terms.json: generate fresh, save for reuse)
5.  SEARCH:  Execute search via backend
             - DDG: ddg-search "[query]" -f json -n 10 | extract URLs
             - WebSearch: search → extract URLs from markdown
             - webfetch: skip (user provided URLs)
             - Firecrawl: search → get URLs
6.  DEDUP:   Check seen_urls set → skip already-fetched URLs
7.  RANK:    Rank URLs by source domain quality ranking
8.  LIMIT:   Take top max_urls_per_entry (default 5)
9.  FETCH:   For each URL:
             a. WebFetch with simple content prompt
             b. Check if code blocks present
             c. If expected but missing: second pass with code-targeting prompt
             d. Merge passes
             e. Store to <kb>/harvested/raw/<entry-id>-<N>.md
10. EXTRACT: For each fetched page (PER-PAGE, not per-batch):
             a. Claude reads raw markdown
             b. Extracts structured fields per entry-schema.json
             c. Records field_provenance per field
             d. If bridge-eligible layer: run bridge detection extraction
             e. Check for multi-topic coverage (one page covering multiple entries)
                If multi-topic: produce separate extraction results per entry.
                Each result proceeds through steps 11-17 independently.
             f. Result stored per-page, markdown discarded from active context
11. MERGE:   If multiple pages for same entry:
             a. Combine code_blocks from all sources (deduplicate identical blocks)
             b. Synthesize description from all sources (prefer longest/most detailed)
             c. Take highest-confidence value per field
             d. Concatenate original_markdown from all sources with source headers
             e. Multi-source entries get confidence boost (+0.05 per corroborating source)
12. SCORE:   Calculate overall_confidence:
             sum(field_confidence * weight) * 0.85 + source_domain_weight * 0.15
13. GATE:    Check against backend's quality_floor:
             >= floor:              accept
             0.30 to floor:        accept with review_flag = true
             < 0.30:               discard, log reason
14. STAGE:   Write to <kb>/harvested/staged/<entry-id>.json
             (in --auto mode: promote directly to final location)
             (in interactive: show to user for approval)
15. WRITE:   Write entry to final KB location
16. CASCADE: (see Section 8.1)
17. CHECKPOINT: Update harvest-checkpoint.json
```

### 8.1 Cascade — Automatic Infrastructure Updates

The cascade runs ONCE per batch (not per entry). In auto mode, a batch is N entries (configurable via `--batch`, default 5). In interactive mode, the cascade runs after each user-approved entry (effectively batch size 1). After a batch of entries is written:

```
BATCH COMPLETE (N entries written)
    |
    ├── 1. LOCK: Acquire <kb>/harvest.lock (PID + timestamp)
    |         If lock exists and < 30 min old: wait/fail
    |         Stale locks (> 30 min): auto-clear
    |
    ├── 2. JOURNAL: Create cascade-journal.json (standalone file for kb-sync discovery)
    |         AND update cascade_journal in harvest-checkpoint.json (for session resume)
    |         Track each step's completion status in both locations
    |
    ├── 3. MANIFEST UPDATE (once per KB layer touched):
    |     ├── Read manifest.json for each affected layer
    |     ├── Add/update entries: status, harvested_at, source, markdown_length,
    |     |   code_block_count, confidence, review_flag
    |     ├── Update topic entry counts
    |     └── Write manifest.json
    |
    ├── 4. MASTER-INDEX UPDATE (once):
    |     ├── Read master-index.json (once, cache in memory)
    |     ├── Update knowledge_bases[kb].file_count
    |     ├── Update knowledge_bases[kb].topics[] if new topics added
    |     ├── Update knowledge_bases[kb].status
    |     └── Write master-index.json
    |
    ├── 5. CROSS-REFERENCE UPDATE (once, for all entries in batch):
    |     ├── Collect all tags, concept names, related_topics from batch entries
    |     ├── For each term:
    |     |   ├── Lookup in master-index cross_references
    |     |   ├── If exists: add this KB to kbs[] if not present
    |     |   └── If new: create cross_reference entry
    |     ├── Entry-level cross_references[] are NOT updated during cascade.
    |     |   Rationale: finding the specific target entry in another KB requires
    |     |   scanning that KB's entries (O(N) per KB), and writing to another KB
    |     |   without holding its lock risks conflicts with concurrent harvests.
    |     |   Instead, entry-level cross_references are populated by kb-sync
    |     |   during its verification pass, which can safely read all registered KBs.
    |     └── Write updated master-index.json
    |
    ├── 6. BRIDGE DETECTION (conditional — only if bridge-eligible layer):
    |     ├── For each entry from bridge-eligible layer:
    |     |   ├── Check extraction results for descriptor→parameter mappings
    |     |   ├── For each mapping found:
    |     |   |   ├── Check if bridge entry exists for this descriptor
    |     |   |   ├── If exists: compare parameter ranges
    |     |   |   |   ├── Corroborating: increase confidence +0.10
    |     |   |   |   ├── Contradicting: flag for review
    |     |   |   |   └── New parameters: add to existing bridge entry
    |     |   |   └── If new: create bridge entry
    |     |   |       ├── confidence: 0.50
    |     |   |       ├── source.type: "auto-detected"
    |     |   |       └── review_flag: true
    |     |   └── Done
    |     ├── BRIDGE COMBINATION SCAN (same-category only):
    |     |   ├── For each new/updated bridge entry:
    |     |   |   ├── Scan existing bridges in same category (timbre, dynamics, etc.)
    |     |   |   ├── Check parameter range overlap/complement
    |     |   |   ├── If compatible: add combination in both bridges
    |     |   |   └── If anti_patterns conflict: flag incompatible
    |     |   └── Done
    |     ├── Update bridge manifest.json
    |     └── Update master-index bridge section
    |
    ├── 7. SEARCH TERMS UPDATE:
    |     ├── Mark used search terms in search-terms.json (add "used": true)
    |     ├── Discover new terms from harvested entries:
    |     |   - Extract tags[], concepts[].name, and related_topics[] from batch entries
    |     |   - For each term, check if it matches an unfilled placeholder entry
    |     |   - If match found and no search terms exist for that placeholder: generate
    |     |     search queries using the discovered term as seed (4 perspectives)
    |     └── Write search-terms.json
    |
    ├── 8. JOURNAL COMPLETE: Mark all steps done
    |
    └── 9. UNLOCK: Release <kb>/harvest.lock
```

**Cascade failure recovery:** If interrupted mid-cascade, the journal records which steps completed. kb-sync can replay incomplete journals on its next verification pass.

**Cascade ordering:** All steps are sequential: 3 → 4 → 5 → 6 → 7. Step 6 (bridge detection) creates new entries and writes to master-index cross_references — the same target as step 5. Step 6 also needs accurate master-index state from step 4/5 to avoid stale file_counts. If no bridge-eligible entries exist in the batch, step 6 is skipped entirely.

### 8.2 Extraction Prompt Design

The extraction runs in the main Claude session (full model), NOT in WebFetch.

**Standard extraction prompt (per page):**

```
You are extracting structured knowledge from web content for a Knowledge Base.

TARGET: KB entry for "[topic]" in the "[kb-layer]" layer of "[kb-name]"

SOURCE METADATA (auto-populated, do not extract):
- source.reference: will be set to page title or first <h1>
- source.url: will be set to the fetched URL
- source.backend: will be set to the backend used
- source.retrieved_date: will be set to today's date

ENTRY SCHEMA FIELDS TO EXTRACT:
1. title: Clear, descriptive title for this knowledge entry
2. summary: One line, max 100 characters
3. description: Substantive content, minimum 100 characters. Include specific 
   parameter ranges, implementation details, and practical guidance.
4. concepts: Key concepts as [{name, description, related[]}]. Minimum 2.
5. code_blocks: ALL code examples from the source. 
   CRITICAL: Preserve code blocks EXACTLY as they appear — do not reformat, 
   fix, simplify, or modify any code. Copy verbatim including whitespace, 
   indentation, and comments. Include language tag and description for each.
6. tags: Searchable keywords. Minimum 3.
7. difficulty: beginner | intermediate | advanced
8. domain_relevance: 1-10 for [domain description]
9. related_topics: Other topics this content connects to
10. cross_references: If content explicitly references concepts from other 
    domains (e.g., DSP concepts in a sound design article), note them.

For each field, also provide:
- confidence: 0.0-1.0 how confident you are in this extraction
- method: "direct-extracted" (verbatim from source), "ai-synthesized" 
  (generated from source content), or "ai-inferred" (estimated)

Return as JSON matching the entry schema.
```

**Bridge detection extraction prompt (appended for bridge-eligible layers):**

```
ADDITIONALLY — Bridge Detection:
From this content, extract any mappings between sound descriptions or 
subjective terms and specific DSP parameter settings. For each found:
- descriptor: the subjective quality (warm, bright, punchy, etc.)
- parameter: which DSP parameter (filter_cutoff, resonance, etc.)
- value_range: [min, max] recommended range with units
- typical_default: common starting value
- rationale: why this mapping works
- anti_patterns: what NOT to do with this mapping

Only extract EXPLICIT mappings with specific parameter values mentioned 
in the source text. Do NOT invent mappings from general descriptions.

Return bridge mappings as a separate "bridge_detections" array in the JSON.
```

**Multi-topic detection:** After extraction, if the content covers multiple topics that match different placeholder entries, the extraction should produce multiple entry results from one page. The prompt includes:

```
If this content covers multiple distinct topics that could each be their 
own KB entry, produce separate extraction results for each. Indicate 
which placeholder entry each result maps to, or suggest a new entry ID 
if no matching placeholder exists.
```

### 8.3 Request Budget Configuration

File: `~/.claude/kb-harvest-config.json`

```json
{
  "max_urls_per_session": 30,
  "max_urls_per_entry": 5,
  "max_search_queries_per_entry": 8,
  "cooldown_between_fetches_ms": 1000,
  "auto_harvest_max_urls_per_entry": 3,
  "auto_harvest_timeout_seconds": 120,
  "batch_size_default": 5,
  "refresh_confidence_threshold": 0.60,
  "default_backend": "ddg+webfetch",
  "source_domain_rankings_default": {
    "academic": {
      "domains": ["ieee.org", "aes.org", "arxiv.org", "ccrma.stanford.edu", "dafx.de"],
      "weight": 1.0
    },
    "official_docs": {
      "domains": ["docs.juce.com", "cppreference.com", "cmake.org", "steinberg.help"],
      "weight": 0.9
    },
    "expert_blogs": {
      "domains": ["earlevel.com", "musicdsp.org", "theaudioprogrammer.com", "kvraudio.com/forum"],
      "weight": 0.8
    },
    "code_repos": {
      "domains": ["github.com", "gitlab.com"],
      "weight": 0.7
    },
    "community": {
      "domains": ["kvraudio.com", "forum.juce.com", "stackoverflow.com"],
      "weight": 0.6
    },
    "default_weight": 0.5
  }
}
```

### 8.4 Checkpointing

File: `<kb-path>/harvest-checkpoint.json`

```json
{
  "session_id": "2026-04-04-001",
  "started_at": "2026-04-04T10:00:00Z",
  "backend": "ddg+webfetch",
  "batch_size": 5,
  "current_batch": 2,
  "entries": {
    "entry-id-1": { "status": "completed", "confidence": 0.72 },
    "entry-id-2": { "status": "completed", "confidence": 0.58, "review_flag": true },
    "entry-id-3": { "status": "fetch_failed", "reason": "all URLs returned empty content" },
    "entry-id-4": { "status": "in_progress", "urls_fetched": 3, "urls_remaining": 2 },
    "entry-id-5": { "status": "pending" }
  },
  "mode": "auto",
  "seen_urls": ["https://...", "https://..."],
  "import_state": null,
  "cascade_journal": {
    "batch_1": { "manifest": "done", "master_index": "done", "cross_refs": "done", "bridges": "done" },
    "batch_2": { "manifest": "done", "master_index": "pending" }
  }
}
```

For import mode, `import_state` replaces `seen_urls`:
```json
"import_state": {
  "source_path": "/home/myuser/external-kb",
  "format": "json-entries",
  "files_total": 45,
  "files_processed": 23,
  "files_remaining": ["path/to/file24.json", "..."]
}
```

Resume: `kb-harvest --kb <name> --resume` reads checkpoint and continues from `in_progress` / `pending` entries. Skips `completed` and `fetch_failed`. If staged/ contains entries from the interrupted session (interactive mode), show them for approval first before continuing with remaining entries.

### 8.5 Staged Review Buffer

Directory structure under KB path:

```
<kb>/
  harvested/
    raw/                   # Raw WebFetch markdown per URL
      <entry-id>-1.md
      <entry-id>-2.md
    staged/                # Extracted entries awaiting approval
      <entry-id>.json      # (interactive mode only)
  harvest-checkpoint.json
  harvest.lock
  search-terms.json
```

In `--auto` mode, entries bypass staged/ and go directly to final KB location. In interactive mode, entries land in staged/ until user approves via `kb-harvest --review`.

### 8.6 Manifest Rebuild

`kb-harvest --kb <name> --rebuild-manifest`

For each layer directory in the KB:
1. Scan for all .json files (excluding manifest.json)
2. Read each entry: extract status, check for original_markdown (calculate length), count code_blocks, read harvest_metadata.overall_confidence
3. Build manifest entries with all tracked fields
4. Write manifest.json

This fixes broken PBCPB-generated manifests (all zeros) by reconstructing from actual files.

### 8.7 Re-Harvest Versioning

When kb-harvest targets an existing non-placeholder entry:
1. Read existing entry
2. Bump version: `1.0.0 → 1.1.0` (minor bump for content update)
3. Set `supersedes` field to existing entry's ID
4. Move existing entry to `<kb>/<layer>/<topic>/_archive/<entry-id>-v<old-version>.json`
5. Write new entry to original location
6. Update manifest (new version, new timestamps, new confidence)

Append-only principle: old entries remain in `_archive/` for reference and rollback.

### 8.8 External KB Import System

Supports importing external KBs in arbitrary formats. Claude acts as the universal format adapter — for any source format, it reads the content and extracts to our schema.

#### Built-in Format Adapters

| Format ID | Detection Signal | Strategy |
|-----------|-----------------|----------|
| `prototype` | manifest.json with `kb_name`, entries have `concepts[]` | Direct field mapping with minor schema transforms |
| `json-entries` | Directory of JSON files with entry-like fields (title/name, description/content, etc.) | Claude maps source field names to our schema |
| `markdown-dir` | Directory of .md files organized by topic | Each file → `original_markdown`, Claude extracts structured fields |
| `obsidian` | .md files with `[[wikilinks]]` + YAML frontmatter | Frontmatter → fields, body → extraction, wikilinks → `cross_references` |
| `csv` | .csv/.tsv file with header row | Column headers → field mapping, Claude synthesizes complex fields |
| `custom` | User provides `--field-map` | Explicit mapping file drives conversion |

**Auto-detection** (when `--format` not specified): scan source path for detection signals in order listed. If no adapter matches, fall back to `json-entries` for JSON files or `markdown-dir` for .md files.

#### Custom Field Mapping File

For arbitrary JSON formats, the user provides a mapping file:

```json
{
  "source_type": "json",
  "entry_pattern": "**/*.json",
  "field_map": {
    "title": "name",
    "description": "content",
    "summary": "brief",
    "tags": "labels",
    "difficulty": {
      "field": "level",
      "transform": { "easy": "beginner", "medium": "intermediate", "hard": "advanced" }
    }
  },
  "unmapped_fields": "ai-synthesize",
  "id_field": "slug",
  "id_prefix": "imported"
}
```

- `field_map`: keys are our schema fields, values are source field names (or objects with `field` + `transform` for value mapping)
- `unmapped_fields`: `"ai-synthesize"` (Claude generates from available content) or `"skip"` (leave empty/default)
- `id_field`: source field to use for generating entry IDs (optional — if absent, generate from title)
- `id_prefix`: prefix for generated entry IDs (default: KB name)

For markdown source types, most fields are extracted by Claude regardless of mapping — the field_map is primarily for YAML frontmatter fields.

#### Import Extraction Prompt

Adapts the standard extraction prompt (Section 8.2) for import context:

```
You are importing content from an external knowledge base into our KB system.

SOURCE FORMAT: [detected or specified format]
TARGET: "[kb-layer]" layer of "[kb-name]"

The source content below is from a file at [source-path].
[If field_map provided: Field mapping: title=name, description=content, ...]

Extract all fields per the entry schema. For fields that have a direct 
mapping in the source, copy the value exactly (method: "direct-mapped").
For fields that must be derived from the content, synthesize them 
(method: "ai-synthesized"). For fields that require estimation, infer 
them (method: "ai-inferred").

Preserve ALL code blocks with exact original formatting.

[Standard schema fields list from Section 8.2]
[Bridge detection prompt if bridge-eligible layer]
```

#### Import-Specific Error Handling

| Scenario | Behavior |
|----------|----------|
| Source file unreadable | Skip, log path and error |
| Source format detection fails | Ask user to specify `--format` |
| Field mapping produces empty required fields | Claude synthesizes from `original_markdown` if possible, else set review_flag |
| Source entry already exists in target KB | Treat as re-harvest (version bump, archive old) |
| Source path is a single file (not directory) | Import as single entry, prompt user for target layer/topic |
| Source has more granular entries than target KB structure | Merge related source entries into one target entry |
| Source has broader entries than target KB structure | Split into multiple target entries via multi-topic detection |

#### Import Provenance

Imported entries carry full provenance chain:

```json
{
  "source": {
    "type": "external-import",
    "backend": "imported",
    "reference": "original-kb/dsp/filters/biquad.json",
    "import_source": {
      "path": "/home/myuser/external-kb/dsp/filters/biquad.json",
      "format": "json-entries",
      "field_map_used": true,
      "import_date": "2026-04-04"
    }
  },
  "harvest_metadata": {
    "overall_confidence": 0.78,
    "field_provenance": {
      "title": { "method": "direct-mapped", "confidence": 0.95 },
      "description": { "method": "direct-mapped", "confidence": 0.95 },
      "concepts": { "method": "ai-synthesized", "confidence": 0.75 },
      "code_blocks": { "method": "direct-mapped", "confidence": 0.95 },
      "difficulty": { "method": "ai-inferred", "confidence": 0.60 }
    },
    "source_content_length_chars": 3200,
    "extraction_prompt_version": "1.0",
    "source_urls": [],
    "fetch_date": "2026-04-04",
    "backend_used": "imported",
    "review_flag": false
  }
}
```

`source.import_source` is unique to imported entries — tracks where the content originally came from for audit trail.

---

## 9. Integration Changes

### 9.1 kb-sync Changes

**Current role:** Primary index updater (runs on 5-minute cron)
**New role:** Verification, repair, and status promotion

**New capabilities:**
- `kb-sync --verify`: Full consistency check
  - Manifest entry counts match actual files
  - Master-index cross_references point to existing KBs/entries
  - Bridge entries reference valid entries
  - Orphaned cross_references detected
- `kb-sync --repair`: Fix inconsistencies found by --verify
  - Rebuild manifests from files
  - Remove orphaned cross_references
  - Replay incomplete cascade journals
  - Populate entry-level `cross_references[]` fields: for each concept in each entry, check master-index cross_references to find other KBs containing the same concept, then scan those KBs for the target entry and add bidirectional cross_references. This is the only place entry-level cross_references are written (cascade only updates master-index-level cross_references).
- Status promotion: `curated → synced` (after human marks entry as curated)

**Cron job update:** Instead of checking for unharvested content, the cron runs consistency verification. Only triggers if a cascade journal exists (indicating recent harvest activity):
```json
{
  "cron": "*/30 * * * *",
  "prompt": "Check for cascade-journal.json in all registered KBs. If any exist with incomplete steps, run kb-sync --repair for those KBs. If no journals found, skip.",
  "recurring": true
}
```

Manual full verification: `kb-sync --verify` (user-initiated, not cron — too expensive for routine automation).

### 9.2 kb-validate Changes

**New capability:** Read `harvest_metadata` when present.

- Factor `overall_confidence` into validation scoring
- Flag entries where `field_provenance` shows `ai-inferred` at low confidence
- Higher weight for entries with `direct-extracted` code_blocks
- Lower weight for single-source entries (only one URL in `source_urls`)

### 9.3 Consumption Skill Changes

All consumption skills (sound-design-bridge, juce-dsp-implementation, juce-ui-bridge, juce-plugin-spec, juce-daw-testing) need:

1. **KB registry integration:** Read `~/.claude/kb-registry.json` to find KB paths instead of hardcoding
2. **Placeholder detection update:** Replace kb-autofill invocation with:
   ```
   kb-harvest --kb <name> --auto --entry <entry-id> --batch 1
   ```
3. **Confidence awareness:** When reading a KB entry, check `harvest_metadata.overall_confidence` if present. If below 0.60, warn user that entry has low confidence.
4. **Bridge lookup via master-index:** Use `cross_references` to find bridge entries for descriptors, rather than hardcoded paths.

### 9.4 Skills Retired

- `~/.claude/skills/kb-autofill/` — absorbed into `kb-harvest --auto`
- `~/.claude/skills/harvest-data/` — absorbed into `kb-harvest --urls`

---

## 10. Error Handling

### DDG Failures

| Scenario | Behavior |
|----------|----------|
| Bot detection (partial results) | Use whatever results were returned |
| Bot detection (0 results) | Auto-fallback to websearch+webfetch |
| DDG CLI not installed | Fall back to websearch+webfetch, log warning |
| Network error | Retry once, then fall back to websearch+webfetch |

### WebSearch Failures

| Scenario | Behavior |
|----------|----------|
| Zero results returned | Try alternative query phrasing from search-terms.json, then log as no results |
| Results contain no extractable URLs | Log warning, try next query |
| Network error | Retry once, then skip query |
| All queries exhausted with no URLs | Entry stays placeholder, log in harvest-status.json |

### WebFetch Failures

| Scenario | Behavior |
|----------|----------|
| URL redirects | Follow redirect URL with new WebFetch call |
| Auth-required page | Skip, log failed URL for user (suggest --urls with manual access) |
| Empty/login page content | Skip, log as unusable |
| JavaScript-only page | Skip, note limitation |
| Summarized content (no code blocks) | Second pass with code-targeting prompt |
| Timeout | Skip URL, try next |

### Cascade Failures

| Scenario | Behavior |
|----------|----------|
| Manifest write fails | Log in journal, kb-sync repairs on next pass |
| Master-index write fails | Log in journal, entries still valid on disk |
| Cross-reference target KB unreachable | Skip cross-ref for that KB, log |
| Lock acquisition fails | Wait 30s, retry once, then fail with user message |
| Concurrent session detected | Inform user, suggest waiting or using different KB |

### Quality Failures

| Scenario | Behavior |
|----------|----------|
| All URLs produce < 0.30 confidence | Entry stays placeholder, log in harvest-status.json with reason |
| Extraction produces no code blocks for code-oriented KB | Lower overall_confidence, set review_flag |
| Bridge auto-detection conflict with existing bridge | Flag for review, don't modify existing bridge |
| Entry schema validation fails | Don't write entry, log specific validation errors |

---

## 11. File Locations Summary

| File | Purpose | Created By |
|------|---------|------------|
| `~/.claude/kb-registry.json` | Multi-KB registry | PBCPB Phase 3 or kb-harvest first run |
| `~/.claude/kb-harvest-config.json` | Request budgets, default rankings | User or kb-harvest first run |
| `<kb>/master-index.json` | Cross-reference index | PBCPB Phase 3, maintained by kb-harvest cascade |
| `<kb>/<layer>/manifest.json` | Per-layer file tracking | PBCPB Phase 3, maintained by kb-harvest cascade |
| `<kb>/search-terms.json` | Reusable search queries | PBCPB Phase 3 or kb-harvest |
| `<kb>/harvest-status.json` | Placeholder fill tracking | PBCPB Phase 3 or kb-harvest |
| `<kb>/harvest-checkpoint.json` | Session resume point | kb-harvest |
| `<kb>/harvest.lock` | Concurrent access lock | kb-harvest cascade |
| `<kb>/cascade-journal.json` | Cascade step completion tracking | kb-harvest cascade, read by kb-sync |
| `<kb>/harvested/raw/*.md` | Raw WebFetch output | kb-harvest |
| `<kb>/harvested/staged/*.json` | Entries awaiting approval | kb-harvest (interactive mode) |
| `<kb>/<layer>/<topic>/_archive/` | Superseded entry versions | kb-harvest re-harvest |
| `<playbook>/architecture/entry-schema.json` | Entry validation schema | PBCPB Phase 2 |
| `<playbook>/architecture/bridge-schema.json` | Bridge validation schema | PBCPB Phase 2 |
| `<playbook>/architecture/kb-access-spec.md` | Consumption skill access patterns | PBCPB Phase 2 |
| `<playbook>/architecture/population-strategy.md` | Backends, weights, rankings | PBCPB Phase 2 |
| `<kb>/import-field-maps/*.json` | Custom field mapping files for imports | User-provided |

---

## 12. PBCPB Change Summary

Every change traced through the generation chain:

| # | PBCPB Location | Change | Generated Playbook Effect | KB Effect |
|---|---|---|---|---|
| 1 | output-schema: population_strategy | Add web_backends, confidence_weights, source_domain_rankings, skill_references, bridge_eligible_layers | Phase 2 architect defines multi-backend harvesting config | KB has configurable harvesting pipeline |
| 2 | output-schema: population_strategy.harvesting_sources | oneOf: string array OR object (backward compat) | New playbooks use rich format, old ones still valid | Smooth migration path |
| 3 | output-schema: knowledge_base | Add kb_registry field | Generated playbook includes registry metadata | KB can be found by any skill |
| 4 | Phase 2 Task 2 | Granularity guidance for layers | Narrower, more focused KB layers | Agents find content faster |
| 5 | Phase 2 Task 3 | source.backend, original_markdown, harvest_metadata | Entry schema includes provenance and full content | Entries are traceable and agent-consumable |
| 6 | Phase 2 Task 4 | Bridge provenance tracking | Bridge schema supports auto-detected sources | Web-harvested bridges are trackable |
| 7 | Phase 2 Task 5 | Web backends, weights, rankings, skill refs | Population strategy is complete and actionable | Phase 3 executor knows exactly how to harvest |
| 8 | Phase 2 Task 6 | Unified master-index and manifest formats | Index/manifest formats match prototype quality | Rich cross-referencing works |
| 9 | Phase 2 New Task | KB access specification | Consumption skills have a spec to follow | Skills find KB content consistently |
| 10 | Phase 2 Gate | 8 new gate conditions | Phase 2 can't pass without complete KB spec | No ambiguity for Phase 3 |
| 11 | Phase 3 New Task | Register KB in registry | KB is discoverable by all skills | Multi-KB targeting works |
| 12 | Phase 3 Task 4 | Implementation-ready content requirement | Research harvest produces rich entries | Entries have code blocks and detail |
| 13 | Phase 3 New Task | Search term generation | Structured, reusable search queries | Web harvesting is targeted |
| 14 | Phase 3 New Task | Web harvest via kb-harvest skill | Remaining placeholders filled from web | KB is more complete |
| 15 | Phase 3 Auditor | Manifest accuracy verification, code integrity | Broken manifests caught before gate | Data integrity ensured |
| 16 | Phase 3 Gate | 8 new gate conditions | Phase 3 enforces quality and completeness | KB ships correctly |
| 17 | Phase 3 Handoff | Additional output artifacts | Phase 4 has search-terms, harvest-status, registry | Continuity across phases |
| 18 | Phase 7 | phase_kb_mapping includes bridge, skill_activation includes kb-harvest | Execution phases know which KBs to read | Agents reference correct KBs |
| 19 | Failure Modes | FM-020 (synthesis hallucination), FM-021 (broken manifests) | Known risks documented with prevention | Operators know what to watch for |
| 20 | output-schema: population_strategy | Add import_adapters array | Population strategy can list custom import formats | KB supports external imports |
| 21 | kb-harvest skill | --import mode with format adapters | External KBs importable in any format | KB can be populated from existing knowledge |

---

## 13. Verification Checklist

Before implementation, verify this spec covers:

- [ ] Every entry-schema.json field has a documented data source for each backend — or explicit fallback
- [ ] WebFetch two-step architecture: fetch with small model, extract with full Claude
- [ ] Code blocks preserved verbatim through entire pipeline (WebFetch → raw → extraction → entry → validation)
- [ ] original_markdown stored on all non-placeholder entries regardless of source
- [ ] Master-index uses concept-level cross-referencing (prototype format)
- [ ] Manifests use per-file tracking with code_block_count, confidence, review_flag
- [ ] Master-index cross_references maintained during cascade; entry-level cross_references by kb-sync
- [ ] Bridge auto-detection only on bridge-eligible layers at 0.50 confidence
- [ ] Quality gates calibrated to not waste tokens: accept >= floor, flag near-floor, discard < 0.30
- [ ] Per-page extraction (not per-batch) to manage context window
- [ ] URL deduplication via seen_urls set
- [ ] Concurrent access locking with stale lock detection
- [ ] Backward compatibility: harvesting_sources oneOf, missing source.backend defaults to research-docs
- [ ] Broken manifest recovery via --rebuild-manifest
- [ ] Checkpointing and session resume
- [ ] DDG auto-fallback to websearch+webfetch
- [ ] phase_kb_mapping and skill_activation include KB-related skills
- [ ] Consumption skills updated to use registry and kb-harvest --auto for placeholders
- [ ] kb-sync role shifted to verification/repair
- [ ] kb-validate reads harvest_metadata for confidence scoring
- [ ] Search terms persisted and reusable across sessions
- [ ] Re-harvest versioning with append-only archive
- [ ] Multi-topic page detection (one URL → multiple entries)
- [ ] Source domain quality rankings per-KB with sensible defaults
- [ ] Cascade batched (not per-entry) for token efficiency
- [ ] Journal-based cascade recovery for interrupted sessions
- [ ] FM-020 and FM-021 failure modes documented with prevention strategies
- [ ] External KB import supports arbitrary formats via 6 built-in adapters + custom field mapping
- [ ] Import provenance tracked (source.import_source with path, format, import_date)
- [ ] Import uses same quality gates, cascade, and infrastructure updates as web harvest
- [ ] source.type defined for all entry origins (web-harvested, research-doc, expert-knowledge, external-import)
- [ ] source.reference auto-populated (page title for web, doc name for research, filename for import)
- [ ] WebSearch URL extraction documented (regex from markdown, filter search engine chrome)
- [ ] search-terms.json and harvest-status.json have concrete JSON format examples
- [ ] Confidence formula: source_domain_weight is 15% additive factor, not global multiplier
- [ ] Entry-level cross_references populated by kb-sync (not cascade) to avoid cross-KB lock conflicts
- [ ] --auto --all mode described for bulk re-harvesting
- [ ] --resume handles staged entries from interrupted interactive sessions
