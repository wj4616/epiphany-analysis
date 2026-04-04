# KB Infrastructure + kb-harvest Skill Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Create the unified kb-harvest skill and supporting KB infrastructure (registry, templates, prompts) that replaces three Firecrawl-only skills with multi-backend, multi-KB harvesting.

**Architecture:** A Claude Code skill (`~/.claude/skills/kb-harvest/SKILL.md`) orchestrates KB population via WebFetch + DDG/WebSearch backends. JSON templates define data structures, extraction prompt files drive field extraction, and an automatic cascade after each harvest batch maintains manifests, master-index, cross-references, and bridge entries. A KB registry (`~/.claude/kb-registry.json`) enables multi-KB targeting.

**Tech Stack:** Claude Code skill (SKILL.md), JSON Schema templates, WebFetch + WebSearch + DDG CLI, Claude extraction prompts

**Spec:** `docs/superpowers/specs/2026-04-04-kb-harvest-pbcpb-integration-design.md`

**Related Plans:**
- Plan A: PBCPB Meta-System Changes (parallel)
- Plan C: Integration Changes — kb-sync, kb-validate, consumption skills (depends on this plan)

---

## File Structure

### Files to Create

```
~/.claude/skills/kb-harvest/
├── SKILL.md                                # Main skill (overwrite existing)
├── prompts/
│   ├── webfetch-content.md                 # WebFetch general content prompt
│   ├── webfetch-code.md                    # WebFetch code-targeting second pass
│   ├── extraction-standard.md              # Claude field extraction (per page)
│   ├── extraction-bridge.md                # Bridge detection (appended for eligible layers)
│   ├── extraction-import.md                # Import extraction prompt
│   └── extraction-multitopic.md            # Multi-topic detection prompt
└── templates/
    ├── entry-schema-default.json           # Default entry schema (JSON Schema)
    ├── bridge-schema-default.json          # Default bridge schema (JSON Schema)
    ├── master-index-template.json          # Master-index initialization
    ├── manifest-template.json              # Per-layer manifest initialization
    └── kb-harvest-config-default.json      # Default config (→ ~/.claude/ on first run)
```

### Files Created at Runtime

```
~/.claude/kb-registry.json                  # Created on first run
~/.claude/kb-harvest-config.json            # Copied from template on first run
<kb>/search-terms.json                      # Per-KB search queries
<kb>/harvest-status.json                    # Per-KB placeholder tracking
<kb>/harvest-checkpoint.json                # Session resume state
<kb>/harvest.lock                           # Concurrent access lock
<kb>/cascade-journal.json                   # Cascade step tracking
<kb>/harvested/raw/*.md                     # Raw WebFetch output
<kb>/harvested/staged/*.json                # Entries awaiting approval
```

### Files to Delete

```
~/.claude/skills/kb-autofill/               # Absorbed into kb-harvest --auto
~/.claude/skills/harvest-data/              # Absorbed into kb-harvest --urls
```

---

## Task 1: Create directory structure and default config

**Files:**
- Create: `~/.claude/skills/kb-harvest/prompts/` (directory)
- Create: `~/.claude/skills/kb-harvest/templates/` (directory)
- Create: `~/.claude/skills/kb-harvest/templates/kb-harvest-config-default.json`

- [ ] **Step 1: Create directory structure**

```bash
mkdir -p ~/.claude/skills/kb-harvest/prompts
mkdir -p ~/.claude/skills/kb-harvest/templates
```

- [ ] **Step 2: Write default config template**

Create `~/.claude/skills/kb-harvest/templates/kb-harvest-config-default.json`:

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
      "domains": ["earlevel.com", "musicdsp.org", "theaudioprogrammer.com", "forum.juce.com"],
      "weight": 0.8
    },
    "code_repos": {
      "domains": ["github.com", "gitlab.com"],
      "weight": 0.7
    },
    "community": {
      "domains": ["kvraudio.com", "stackoverflow.com"],
      "weight": 0.6
    },
    "default_weight": 0.5
  }
}
```

- [ ] **Step 3: Verify directory structure**

```bash
find ~/.claude/skills/kb-harvest/ -type d | sort
```

Expected:
```
/home/myuser/.claude/skills/kb-harvest/
/home/myuser/.claude/skills/kb-harvest/prompts
/home/myuser/.claude/skills/kb-harvest/templates
```

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/kb-harvest/templates/kb-harvest-config-default.json
git commit -m "feat(kb-harvest): create directory structure and default config template"
```

---

## Task 2: Create entry schema default template

**Files:**
- Create: `~/.claude/skills/kb-harvest/templates/entry-schema-default.json`

- [ ] **Step 1: Write entry schema**

Create `~/.claude/skills/kb-harvest/templates/entry-schema-default.json`:

```json
{
  "$schema": "https://json-schema.org/draft-07/schema#",
  "title": "KB Entry Schema (Default)",
  "description": "Default entry schema for KBs without a custom schema. Used by kb-harvest when registry schema_path is null.",
  "type": "object",
  "required": ["id", "kb", "topic", "status", "version", "title", "summary", "description", "source", "concepts", "tags", "difficulty", "domain_relevance"],
  "properties": {
    "id": {
      "type": "string",
      "pattern": "^[a-z0-9_-]+$",
      "description": "Format: {kb-name}_{topic}_{entry-name}"
    },
    "kb": { "type": "string", "description": "KB layer name" },
    "topic": { "type": "string", "description": "Topic classification" },
    "status": { "type": "string", "enum": ["placeholder", "harvested", "curated", "synced"] },
    "version": { "type": "string", "pattern": "^\\d+\\.\\d+\\.\\d+$" },
    "title": { "type": "string", "minLength": 1 },
    "summary": { "type": "string", "maxLength": 100 },
    "description": {
      "type": "string",
      "minLength": 100,
      "description": "Substantive content — parameter ranges, implementation detail, practical guidance"
    },
    "source": {
      "type": "object",
      "required": ["type", "backend"],
      "properties": {
        "type": {
          "type": "string",
          "enum": ["web-harvested", "research-doc", "expert-knowledge", "external-import"]
        },
        "reference": { "type": "string", "description": "Auto-populated: page title for web, doc name for research, filename for import" },
        "url": { "type": "string", "format": "uri" },
        "backend": {
          "type": "string",
          "enum": ["research-docs", "firecrawl", "ddg+webfetch", "websearch+webfetch", "webfetch", "imported"]
        },
        "retrieved_date": { "type": "string", "format": "date" },
        "import_source": {
          "type": "object",
          "description": "Present only on imported entries",
          "properties": {
            "path": { "type": "string", "description": "Absolute path to original source file" },
            "format": { "type": "string", "enum": ["prototype", "json-entries", "markdown-dir", "obsidian", "csv", "custom"] },
            "field_map_used": { "type": "boolean" },
            "import_date": { "type": "string", "format": "date" }
          }
        }
      }
    },
    "concepts": {
      "type": "array",
      "minItems": 2,
      "items": {
        "type": "object",
        "required": ["name", "description"],
        "properties": {
          "name": { "type": "string" },
          "description": { "type": "string" },
          "related": { "type": "array", "items": { "type": "string" }, "default": [] }
        }
      }
    },
    "code_blocks": {
      "type": "array",
      "items": {
        "type": "object",
        "required": ["language", "code"],
        "properties": {
          "language": { "type": "string" },
          "code": { "type": "string", "description": "EXACT original code — never reformatted" },
          "description": { "type": "string" },
          "preserved": { "type": "boolean", "default": true }
        }
      }
    },
    "tags": { "type": "array", "items": { "type": "string" }, "minItems": 1 },
    "related_topics": { "type": "array", "items": { "type": "string" } },
    "cross_references": {
      "type": "array",
      "items": { "type": "string" },
      "description": "Populated by kb-sync --repair, not during harvest"
    },
    "difficulty": { "type": "string", "enum": ["beginner", "intermediate", "advanced"] },
    "domain_relevance": { "type": "integer", "minimum": 1, "maximum": 10 },
    "original_markdown": {
      "type": "string",
      "description": "Full source content in markdown. Present on all non-placeholder entries."
    },
    "supersedes": {
      "type": "string",
      "description": "Entry ID of previous version (present on re-harvested entries only)"
    },
    "harvest_metadata": {
      "type": "object",
      "description": "Present on all non-research-doc entries (web-harvested and imported)",
      "properties": {
        "overall_confidence": { "type": "number", "minimum": 0, "maximum": 1 },
        "field_provenance": {
          "type": "object",
          "additionalProperties": {
            "type": "object",
            "required": ["method", "confidence"],
            "properties": {
              "method": { "type": "string", "enum": ["direct-extracted", "direct-mapped", "ai-synthesized", "ai-inferred"] },
              "confidence": { "type": "number", "minimum": 0, "maximum": 1 }
            }
          }
        },
        "source_content_length_chars": { "type": "integer" },
        "extraction_prompt_version": { "type": "string" },
        "source_urls": { "type": "array", "items": { "type": "string" } },
        "fetch_date": { "type": "string", "format": "date" },
        "backend_used": { "type": "string" },
        "review_flag": { "type": "boolean" }
      }
    }
  },
  "additionalProperties": false
}
```

- [ ] **Step 2: Validate JSON is well-formed**

```bash
python3 -c "import json; json.load(open('$HOME/.claude/skills/kb-harvest/templates/entry-schema-default.json')); print('Valid JSON')"
```

Expected: `Valid JSON`

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-harvest/templates/entry-schema-default.json
git commit -m "feat(kb-harvest): add default entry schema template with provenance fields"
```

---

## Task 3: Create bridge schema, master-index, and manifest templates

**Files:**
- Create: `~/.claude/skills/kb-harvest/templates/bridge-schema-default.json`
- Create: `~/.claude/skills/kb-harvest/templates/master-index-template.json`
- Create: `~/.claude/skills/kb-harvest/templates/manifest-template.json`

- [ ] **Step 1: Write bridge schema**

Create `~/.claude/skills/kb-harvest/templates/bridge-schema-default.json`:

```json
{
  "$schema": "https://json-schema.org/draft-07/schema#",
  "title": "Bridge Entry Schema (Default)",
  "description": "Maps subjective descriptors to technical DSP parameters. Used for bridge layers.",
  "type": "object",
  "required": ["id", "category", "status", "version", "parameters", "intent_mappings"],
  "properties": {
    "id": {
      "type": "string",
      "pattern": "^[a-z0-9_-]+$",
      "description": "Format: bridge_{category}_{descriptor}"
    },
    "status": { "type": "string", "enum": ["placeholder", "harvested", "curated", "synced"] },
    "version": { "type": "string", "pattern": "^\\d+\\.\\d+\\.\\d+$" },
    "category": { "type": "string", "description": "Bridge category: timbre, dynamics, spatial, etc." },
    "parameters": { "type": "array", "items": { "type": "string" } },
    "intent_mappings": {
      "type": "object",
      "description": "Keyed by descriptor (warm, bright, punchy, etc.)",
      "additionalProperties": {
        "type": "object",
        "required": ["parameter", "value_range", "typical_default", "confidence"],
        "properties": {
          "parameter": { "type": "string" },
          "value_range": { "type": "array", "items": { "type": "number" }, "minItems": 2, "maxItems": 2 },
          "typical_default": { "type": "number" },
          "confidence": { "type": "number", "minimum": 0, "maximum": 1 },
          "source": { "type": "string" },
          "why": { "type": "string" },
          "anti_patterns": { "type": "array", "items": { "type": "string" } }
        }
      }
    },
    "source": {
      "type": "object",
      "properties": {
        "type": { "type": "string", "enum": ["expert-knowledge", "auto-detected", "web-harvested", "external-import"] },
        "backend": { "type": "string", "enum": ["research-docs", "ddg+webfetch", "websearch+webfetch", "webfetch", "firecrawl", "imported"] },
        "reference": { "type": "string" },
        "url": { "type": "string" }
      }
    },
    "harvest_metadata": {
      "type": "object",
      "description": "Same structure as entry harvest_metadata (see entry-schema-default.json)"
    }
  }
}
```

- [ ] **Step 2: Write master-index template**

Create `~/.claude/skills/kb-harvest/templates/master-index-template.json`:

```json
{
  "kb_name": "master-index",
  "version": "1.0.0",
  "knowledge_bases": {},
  "cross_references": {},
  "last_updated": ""
}
```

- [ ] **Step 3: Write manifest template**

Create `~/.claude/skills/kb-harvest/templates/manifest-template.json`:

```json
{
  "kb_name": "",
  "version": "1.0.0",
  "created": "",
  "last_sync": "",
  "status": "building",
  "topics": []
}
```

- [ ] **Step 4: Validate all three files**

```bash
for f in bridge-schema-default.json master-index-template.json manifest-template.json; do
  python3 -c "import json; json.load(open('$HOME/.claude/skills/kb-harvest/templates/$f')); print('$f: OK')"
done
```

Expected: all three print OK.

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/kb-harvest/templates/bridge-schema-default.json \
        ~/.claude/skills/kb-harvest/templates/master-index-template.json \
        ~/.claude/skills/kb-harvest/templates/manifest-template.json
git commit -m "feat(kb-harvest): add bridge schema, master-index, and manifest templates"
```

---

## Task 4: Create WebFetch prompts

**Files:**
- Create: `~/.claude/skills/kb-harvest/prompts/webfetch-content.md`
- Create: `~/.claude/skills/kb-harvest/prompts/webfetch-code.md`

- [ ] **Step 1: Write general content prompt**

Create `~/.claude/skills/kb-harvest/prompts/webfetch-content.md`:

```markdown
Extract all content about [topic]. Preserve all code blocks with their original formatting exactly as they appear. Include parameter values, implementation details, and technical explanations. Return as markdown.
```

- [ ] **Step 2: Write code-targeting prompt**

Create `~/.claude/skills/kb-harvest/prompts/webfetch-code.md`:

```markdown
Extract specifically the code examples, parameter values, and implementation patterns about [topic]. Preserve exact code formatting. Include all code blocks with language tags.
```

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-harvest/prompts/webfetch-content.md \
        ~/.claude/skills/kb-harvest/prompts/webfetch-code.md
git commit -m "feat(kb-harvest): add WebFetch content and code-targeting prompts"
```

---

## Task 5: Create extraction prompts

**Files:**
- Create: `~/.claude/skills/kb-harvest/prompts/extraction-standard.md`
- Create: `~/.claude/skills/kb-harvest/prompts/extraction-bridge.md`
- Create: `~/.claude/skills/kb-harvest/prompts/extraction-import.md`
- Create: `~/.claude/skills/kb-harvest/prompts/extraction-multitopic.md`

- [ ] **Step 1: Write standard extraction prompt**

Create `~/.claude/skills/kb-harvest/prompts/extraction-standard.md`:

````markdown
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

For each field, also provide confidence and method metadata.

Return as JSON with this structure:
```json
{
  "fields": {
    "title": "...",
    "summary": "...",
    "description": "...",
    "concepts": [...],
    "code_blocks": [...],
    "tags": [...],
    "difficulty": "...",
    "domain_relevance": 8,
    "related_topics": [...],
    "cross_references": [...]
  },
  "field_provenance": {
    "title": { "confidence": 0.9, "method": "direct-extracted" },
    "description": { "confidence": 0.8, "method": "ai-synthesized" },
    "code_blocks": { "confidence": 0.95, "method": "direct-extracted" },
    "difficulty": { "confidence": 0.6, "method": "ai-inferred" }
  },
  "bridge_detections": []
}
```

Method values: "direct-extracted" (verbatim from source), "ai-synthesized"
(generated from source content), or "ai-inferred" (estimated with limited basis).
`fields` contains entry schema values. `field_provenance` maps to harvest_metadata.
`bridge_detections` is empty unless bridge detection prompt is appended.
````

- [ ] **Step 2: Write bridge detection prompt**

Create `~/.claude/skills/kb-harvest/prompts/extraction-bridge.md`:

````markdown
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
````

- [ ] **Step 3: Write import extraction prompt**

Create `~/.claude/skills/kb-harvest/prompts/extraction-import.md`:

````markdown
You are importing content from an external knowledge base into our KB system.

SOURCE FORMAT: [format]
TARGET: "[kb-layer]" layer of "[kb-name]"

The source content below is from a file at [source-path].
[If field_map provided: Field mapping: [field_map_summary]]

Extract all fields per the entry schema. For fields that have a direct
mapping in the source, copy the value exactly (method: "direct-mapped").
For fields that must be derived from the content, synthesize them
(method: "ai-synthesized"). For fields that require estimation, infer
them (method: "ai-inferred").

Preserve ALL code blocks with exact original formatting.

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

For each field, also provide confidence and method metadata.

Return as JSON with this structure (same as extraction-standard.md):
```json
{
  "fields": { "title": "...", "description": "...", ... },
  "field_provenance": {
    "title": { "confidence": 0.95, "method": "direct-mapped" },
    "description": { "confidence": 0.85, "method": "direct-mapped" },
    "concepts": { "confidence": 0.75, "method": "ai-synthesized" }
  },
  "bridge_detections": []
}
```

Method values: "direct-mapped" (1:1 from format adapter), "direct-extracted"
(verbatim from source), "ai-synthesized" (generated from content), "ai-inferred" (estimated).
````

- [ ] **Step 4: Write multi-topic detection prompt**

Create `~/.claude/skills/kb-harvest/prompts/extraction-multitopic.md`:

````markdown
If this content covers multiple distinct topics that could each be their
own KB entry, produce separate extraction results for each. Indicate
which placeholder entry each result maps to, or suggest a new entry ID
if no matching placeholder exists.
````

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/kb-harvest/prompts/extraction-standard.md \
        ~/.claude/skills/kb-harvest/prompts/extraction-bridge.md \
        ~/.claude/skills/kb-harvest/prompts/extraction-import.md \
        ~/.claude/skills/kb-harvest/prompts/extraction-multitopic.md
git commit -m "feat(kb-harvest): add extraction prompts (standard, bridge, import, multitopic)"
```

---

## Task 6: SKILL.md — Header, invocation, mode routing, first-run setup

**Files:**
- Create: `~/.claude/skills/kb-harvest/SKILL.md` (overwrites existing 236-line Firecrawl-only skill)

- [ ] **Step 1: Write SKILL.md with header and invocation**

Create `~/.claude/skills/kb-harvest/SKILL.md` with the following content:

````markdown
---
name: kb-harvest
description: "Unified KB population — multi-backend web harvesting, import, and infrastructure maintenance. Use for all KB content population tasks."
---

# kb-harvest — Unified KB Population Skill

Populates Knowledge Bases from web sources (DDG, WebSearch, WebFetch), local files (import), or user-provided URLs. Every harvest operation automatically maintains all KB infrastructure: manifests, master-index, cross-references, and bridge entries.

Replaces: kb-harvest (old Firecrawl-only), kb-autofill, harvest-data.

**Spec:** `docs/superpowers/specs/2026-04-04-kb-harvest-pbcpb-integration-design.md`

## Invocation

```
kb-harvest --kb <name>                              # Interactive harvest session
kb-harvest --kb <name> --auto                       # Auto-fill all placeholders
kb-harvest --kb <name> --auto --refresh             # Re-harvest below confidence threshold
kb-harvest --kb <name> --auto --all                 # Re-harvest all entries
kb-harvest --kb <name> --urls <url1> <url2> ...     # Harvest specific URLs
kb-harvest --kb <name> --backend <backend>          # Specify backend
kb-harvest --kb <name> --layer <layer>              # Target specific layer
kb-harvest --kb <name> --topic <topic>              # Target specific topic
kb-harvest --kb <name> --entry <entry-id>           # Target specific entry
kb-harvest --kb <name> --batch <N>                  # Set batch size (default 5)
kb-harvest --kb <name> --resume                     # Resume from checkpoint
kb-harvest --kb <name> --import <source-path>       # Import external KB (auto-detect)
kb-harvest --kb <name> --import <path> --format <f> # Import with explicit format
kb-harvest --kb <name> --import <path> --field-map <mapping.json>
kb-harvest --kb <name> --import <path> --dry-run    # Preview import
kb-harvest --kb <name> --rebuild-manifest           # Reconstruct manifests from files
kb-harvest --kb <name> --review                     # Show staged entries awaiting approval
kb-harvest --status                                 # Show harvest status across all KBs
kb-harvest --list                                   # List registered KBs
```

## Defaults

| Flag | Default |
|------|---------|
| `--kb` | REQUIRED — no default |
| `--backend` | `ddg+webfetch` if `which ddg-search` succeeds, else `websearch+webfetch` |
| `--batch` | 5 |
| mode | Interactive (no `--auto`, `--urls`, or `--import`) |

`--urls` forces `webfetch` backend. `--import` forces `imported` backend.

## Mode Routing

Parse flags and route:

| Flags Present | Mode | Jump To |
|---------------|------|---------|
| `--list` | List KBs | § List Mode |
| `--status` | Status overview | § Status Mode |
| `--rebuild-manifest` | Fix manifests | § Manifest Rebuild |
| `--review` | Approve staged entries | § Staged Review |
| `--import <path>` | Import external KB | § Import System |
| `--urls <url...>` | Harvest URLs | § URL Mode |
| `--auto --all` | Re-harvest everything | § Auto All Mode |
| `--auto --refresh` | Re-harvest low confidence | § Auto Refresh Mode |
| `--auto` | Auto-fill placeholders | § Auto Mode |
| `--resume` | Resume interrupted session | § Resume |
| (none of above) | Interactive harvest | § Interactive Mode |

Check flags in the order listed — first match wins.

## First-Run Setup

On every invocation, check:

1. **Config:** If `~/.claude/kb-harvest-config.json` does not exist:
   - Read `templates/kb-harvest-config-default.json` (relative to this skill directory)
   - Write its contents to `~/.claude/kb-harvest-config.json`
   - Inform user: "Created default config at ~/.claude/kb-harvest-config.json"

2. **Registry:** If `~/.claude/kb-registry.json` does not exist:
   - Write: `{"version": "1.0.0", "registries": [], "default_kb": null}`
   - If `/home/myuser/agents/juce-agent/playbookdata/master-index.json` exists, offer to register it:
     "Found prototype KB at /home/myuser/agents/juce-agent/playbookdata. Register it? (y/n)"
   - If user accepts, add registry entry:
     ```json
     {
       "name": "juce-agent-prototype",
       "path": "/home/myuser/agents/juce-agent/playbookdata",
       "schema_path": null,
       "bridge_schema_path": null,
       "master_index_path": "master-index.json",
       "generated_by": "manual",
       "layers": [],
       "bridge_eligible_layers": [],
       "default_backend": "ddg+webfetch",
       "registered_at": "<ISO timestamp>"
     }
     ```
   - Discover layers by listing subdirectories that contain .json files (exclude `_archive`, `harvested`, and any directory starting with `_`)
   - Fill the `layers` array with discovered layer names
````

- [ ] **Step 2: Verify file was created and is readable**

```bash
head -5 ~/.claude/skills/kb-harvest/SKILL.md
```

Expected: Shows the YAML frontmatter.

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): SKILL.md header, invocation, mode routing, first-run setup"
```

---

## Task 7: SKILL.md — Registry resolution and backend selection

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append after First-Run Setup section)

- [ ] **Step 1: Append registry resolution section**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Registry Resolution

For any command requiring `--kb`:

1. Read `~/.claude/kb-registry.json`
2. Find entry where `name` matches the `--kb` value
3. If not found: list all registered KB names and ask user to choose, or offer to register a new KB
4. From the matched registry entry, resolve paths:
   - `kb_path` = `entry.path` (absolute path to KB root)
   - `schema_path` = `kb_path + "/" + entry.schema_path` if entry.schema_path is not null, else use `templates/entry-schema-default.json` from this skill
   - `bridge_schema_path` = `kb_path + "/" + entry.bridge_schema_path` if not null
   - `master_index_path` = `kb_path + "/" + entry.master_index_path`
   - `layers` = `entry.layers`
   - `bridge_eligible_layers` = `entry.bridge_eligible_layers` (default `[]`)
   - `default_backend` = `entry.default_backend`
5. Read the entry schema from resolved `schema_path`
6. Read config from `~/.claude/kb-harvest-config.json`
7. If `--layer` specified, verify it exists in `layers`. If not, list available layers and ask.
8. If `--topic` specified, verify it exists in the layer directory. If not, list available topics and ask.

### KB Registry Format

`~/.claude/kb-registry.json`:
```json
{
  "version": "1.0.0",
  "registries": [
    {
      "name": "unique-kb-name",
      "path": "/absolute/path/to/kb",
      "schema_path": "../architecture/entry-schema.json",
      "bridge_schema_path": "../architecture/bridge-schema.json",
      "master_index_path": "master-index.json",
      "generated_by": "pbcpb",
      "layers": ["dsp-kb", "sound-design-kb"],
      "bridge_eligible_layers": ["sound-design-kb"],
      "default_backend": "ddg+webfetch",
      "registered_at": "2026-04-04T00:00:00Z"
    }
  ],
  "default_kb": "unique-kb-name"
}
```

Path convention: `path` is always absolute. `schema_path`, `bridge_schema_path`, `master_index_path` are always relative to `path`.

## Backend Selection

1. If `--urls` present → `webfetch`
2. If `--import` present → `imported`
3. If `--backend <name>` specified → use that backend
4. Else → registry entry's `default_backend`
5. If resolved backend is `ddg+webfetch`:
   - Run: `which ddg-search`
   - If not found: fall back to `websearch+webfetch`, warn user

**Backend reference:**

| Backend | Search Tool | Fetch Tool | Quality Floor |
|---------|-------------|------------|---------------|
| `ddg+webfetch` | `ddg-search "[q]" -f json -n 10` | WebFetch | 0.45 |
| `websearch+webfetch` | Claude WebSearch tool | WebFetch | 0.40 |
| `webfetch` | None (user provides URLs) | WebFetch | 0.50 |
| `firecrawl` | Firecrawl search API | Firecrawl or WebFetch | 0.55 |
| `imported` | None (local files) | Format adapter + Claude | 0.45 |

Quality floors are defaults — override per-KB in population-strategy if available.

### WebSearch URL Extraction

WebSearch returns markdown with embedded links. Extract URLs:
1. Collect all URLs matching pattern `https?://[^\s\)\]]+` from the markdown
2. Deduplicate
3. Filter out: google.com, bing.com, duckduckgo.com, *.search.yahoo.com
4. Remaining URLs are the search results → pass to WebFetch
````

- [ ] **Step 2: Verify section appended correctly**

```bash
grep -c "## Registry Resolution" ~/.claude/skills/kb-harvest/SKILL.md
grep -c "## Backend Selection" ~/.claude/skills/kb-harvest/SKILL.md
```

Expected: both return `1`.

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add registry resolution and backend selection to SKILL.md"
```

---

## Task 8: SKILL.md — Search term generation

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append search term generation section**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Search Term Generation

For each entry that needs harvesting, generate or load search queries:

1. Check `<kb_path>/search-terms.json` — if entry ID has queries with `used: false`, use those
2. If no queries exist, generate from the entry's topic name and layer context:

**Generate 1-2 queries per perspective** (cap total at `max_search_queries_per_entry` from config, default 8):
- **Academic:** `[topic] algorithm`, `[topic] computational method IEEE`
- **Practitioner:** `[topic] C++ implementation real-time audio`, `[topic] best practices production code`
- **Educator:** `[topic] tutorial explained in depth`, `[topic] advanced guide`
- **Domain expert:** Alternative terminology and synonyms for the topic
- **Code-targeting:** `[topic] JUCE tutorial code example`, `[topic] site:github.com C++ implementation`

If generated count exceeds `max_search_queries_per_entry`, keep the highest-priority queries (academic + practitioner + code-targeting first, then educator + domain expert).

3. Save to `<kb_path>/search-terms.json`:

```json
{
  "version": "1.0.0",
  "generated_at": "<ISO timestamp>",
  "entries": {
    "<entry-id>": {
      "status": "pending",
      "queries": [
        { "perspective": "academic", "query": "biquad filter algorithm digital signal processing", "used": false },
        { "perspective": "practitioner", "query": "biquad filter C++ implementation real-time audio", "used": false },
        { "perspective": "code", "query": "biquad filter JUCE tutorial code example", "used": false }
      ]
    }
  }
}
```

In interactive mode, display generated queries and let user edit/add before searching.
````

- [ ] **Step 2: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add search term generation to SKILL.md"
```

---

## Task 9: SKILL.md — Harvest pipeline

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append harvest pipeline section**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Harvest Pipeline (Single Entry)

**Session-level limits** (from `~/.claude/kb-harvest-config.json`):
- Track total URLs fetched across all entries. Stop fetching when `max_urls_per_session` (default 30) is reached. Entries still in queue stay as placeholders.
- In auto mode, skip an entry if elapsed time exceeds `auto_harvest_timeout_seconds` (default 120) for that entry. Mark as `fetch_failed` with reason "timeout".

Execute these steps for each entry to harvest:

### Steps 1-3: Resolve
- Read kb-registry.json → resolve KB path (§ Registry Resolution)
- Read entry-schema.json → know target fields
- Read layer manifest.json → find target entry
- If manifest is broken (zero entries despite files on disk): auto-run § Manifest Rebuild first

### Step 4: Terms
Load search-terms.json → get unused queries for this entry.
If none exist, generate per § Search Term Generation.

### Step 5: Search

**DDG backend:**
```bash
ddg-search "[query]" -f json -n 10
```
Parse JSON, extract URLs from `.items[].link`. If 0 results (bot detection): auto-fallback to WebSearch for this query.

**WebSearch backend:**
Use the WebSearch tool with the query. Extract URLs per § WebSearch URL Extraction.

**webfetch backend:** Skip — user provided URLs via `--urls`.

**firecrawl backend:** Use Firecrawl search API to get URLs.

### Step 6: Dedup
Maintain a `seen_urls` set for this harvest session. Skip already-fetched URLs.

### Step 7: Rank
Look up each URL's domain in source_domain_rankings (from KB population strategy, or config defaults). Sort URLs by tier weight descending. First matching tier wins for each domain.

### Step 8: Limit
Take top N URLs: `max_urls_per_entry` (default 5) in interactive mode, `auto_harvest_max_urls_per_entry` (default 3) in auto mode.

### Step 9: Fetch
For each URL (wait `cooldown_between_fetches_ms` from config, default 1000ms, between consecutive WebFetch calls):

1. Check session URL count against `max_urls_per_session` (default 30). If reached, stop fetching — remaining entries stay as placeholders.
2. Read `prompts/webfetch-content.md`. Replace `[topic]` with the entry's topic.
3. Call **WebFetch** with the URL and the assembled prompt.
4. Check if the result contains code blocks (look for triple-backtick fences).
5. If NO code blocks found AND this layer is code-oriented (check layer name — dsp, juce, cpp, cmake, etc.):
   - Read `prompts/webfetch-code.md`. Replace `[topic]`.
   - Call **WebFetch** again with the code-targeting prompt.
   - Merge both results into one markdown document.
6. Save raw output to `<kb_path>/harvested/raw/<entry-id>-<N>.md` (N = URL index).

**WebFetch constraint:** WebFetch uses a small, fast model — NOT the main Claude model. Use it ONLY for fetching content. All structured field extraction happens in Step 10 with the full model.

### Step 10: Extract
For each fetched page (**PER-PAGE**, not per-batch — to manage context window):

1. Read the raw markdown from `<kb_path>/harvested/raw/<entry-id>-<N>.md`
2. Read `prompts/extraction-standard.md`. Substitute:
   - `[topic]` → entry topic
   - `[kb-layer]` → layer name
   - `[kb-name]` → KB name
   - `[domain description]` → from master-index `knowledge_bases[kb].description` if present, else derive from KB name (e.g., "dsp-kb" → "DSP and audio signal processing")
3. If this layer is in `bridge_eligible_layers`: read and append `prompts/extraction-bridge.md`
4. Read and append `prompts/extraction-multitopic.md`
5. Present the raw markdown to Claude with the assembled extraction prompt
6. Parse the JSON response — get all extracted fields + per-field confidence + method
7. If response includes multiple entry results (multi-topic): each result proceeds through remaining steps independently
8. **After extraction**, discard the raw markdown from active context (it's saved to disk)

### Step 11: Merge
If multiple pages were extracted for the same entry:

1. Combine `code_blocks[]` from all pages (deduplicate identical blocks by comparing `code` field)
2. For `description`: use the longest/most detailed version
3. For other fields: take the value with highest confidence
4. Concatenate `original_markdown` from all pages, separated by `\n\n---\nSource: <url>\n---\n\n`
5. Multi-source confidence boost: +0.05 per corroborating source, max +0.15

### Step 12: Score

```
overall_confidence = sum(field_confidence × weight) × 0.85 + source_domain_weight × 0.15
```

**Confidence weights** (read from KB population strategy if available, else use defaults):

| Field | Default Weight |
|-------|---------------|
| description | 0.25 |
| code_blocks | 0.25 |
| concepts | 0.15 |
| title | 0.05 |
| summary | 0.05 |
| tags | 0.05 |
| related_topics | 0.05 |
| difficulty | 0.05 |
| domain_relevance | 0.05 |
| cross_references | 0.05 |

Weights MUST sum to 1.0.

**source_domain_weight:** Look up the primary URL's domain in source_domain_rankings. Use `default_weight` (0.5) if no tier matches.

### Step 13: Gate

| Condition | Action |
|-----------|--------|
| `confidence >= quality_floor` | Accept. Set status = `"harvested"` |
| `0.30 <= confidence < quality_floor` | Accept. Set `review_flag = true` |
| `confidence < 0.30` | Discard. Log reason in harvest-status.json. Entry stays placeholder. |

### Step 14: Stage
- **Auto mode:** Skip staging — proceed to Step 15
- **Interactive mode:** Write to `<kb_path>/harvested/staged/<entry-id>.json`. Display summary to user for approval.

### Step 15: Write
Write the entry JSON to `<kb_path>/<layer>/<topic>/<entry-id>.json`.

Every written entry MUST include all required fields from the entry schema. Auto-populate:
- `source.reference` = page `<title>` or first `<h1>` from fetched content
- `source.url` = primary URL
- `source.backend` = backend used
- `source.retrieved_date` = today's date (ISO format)
- `harvest_metadata.extraction_prompt_version` = `"1.0"`
- `harvest_metadata.fetch_date` = today's date
- `harvest_metadata.backend_used` = backend used
- `harvest_metadata.source_urls` = all URLs fetched for this entry

### Step 16: Cascade
Run § Cascade after the batch is complete.
- Auto mode: cascade runs after every `--batch` entries (default 5)
- Interactive mode: cascade runs after each approved entry (batch size 1)

### Step 17: Checkpoint
Update `<kb_path>/harvest-checkpoint.json` — mark entry as completed, update seen_urls, update cascade_journal.
````

- [ ] **Step 2: Verify pipeline section is complete**

```bash
grep -c "### Step" ~/.claude/skills/kb-harvest/SKILL.md
```

Expected: `15` (Steps 1-3 as one combined header, then Steps 4-17 as individual headers = 1 + 14 = 15).

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add 17-step harvest pipeline to SKILL.md"
```

---

## Task 10: SKILL.md — Quality scoring details and cascade

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append quality scoring and cascade sections**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Quality Scoring Details

### Field Provenance Methods

| Method | Meaning | Typical Confidence |
|--------|---------|-------------------|
| `direct-extracted` | Taken verbatim from source (code, URLs) | 0.85-0.95 |
| `direct-mapped` | Field copied 1:1 from import format adapter | 0.90-0.95 |
| `ai-synthesized` | Generated by Claude from source content | 0.65-0.85 |
| `ai-inferred` | Estimated by Claude with limited basis | 0.40-0.65 |

### Source Domain Matching

Match the URL hostname against domain tiers in `source_domain_rankings`. Extract hostname only — ignore path, port, and query. For `https://github.com/user/repo/...`, match against `github.com`. For `https://forum.juce.com/t/12345`, match against `forum.juce.com`. Check tiers in weight-descending order (academic first). First matching tier wins. If no tier matches, use `default_weight` (0.5).

### Confidence Formula Rationale

The formula `sum(field_conf × weight) × 0.85 + source_domain_weight × 0.15` weights extraction quality at 85% and source authority at 15%. A perfect extraction from an unknown domain scores `1.0 × 0.85 + 0.5 × 0.15 = 0.925` — good extractions are not suppressed by unknown sources.

## Cascade — Automatic Infrastructure Updates

The cascade runs ONCE per batch. Auto mode: batch = `--batch` entries (default 5). Interactive mode: batch = 1 entry.

**All steps are sequential: 3 → 4 → 5 → 6 → 7.**

```
BATCH COMPLETE (N entries written)
│
├── 1. LOCK
│     Write PID + timestamp to <kb_path>/harvest.lock
│     If lock exists and < 30 min old: wait 30s, retry once, then fail with message
│     If lock > 30 min: stale lock, delete and acquire
│
├── 2. JOURNAL
│     Create <kb_path>/cascade-journal.json (for kb-sync discovery)
│     AND update cascade_journal in harvest-checkpoint.json (for resume)
│     Initialize all steps as "pending"
│     Standalone cascade-journal.json format:
│     ```json
│     {
│       "session_id": "<ISO-date>-<seq>",
│       "batch_id": "batch_<N>",
│       "started_at": "<ISO timestamp>",
│       "entries_in_batch": ["<entry-id-1>", "<entry-id-2>"],
│       "steps": {
│         "manifest": "pending",
│         "master_index": "pending",
│         "cross_refs": "pending",
│         "bridges": "pending",
│         "search_terms": "pending"
│       }
│     }
│     ```
│
├── 3. MANIFEST UPDATE
│     For each KB layer touched in this batch:
│       Read <kb_path>/<layer>/manifest.json
│       For each written entry in that layer:
│         Add/update file entry with: status, harvested_at, source (backend),
│         markdown_length (chars in original_markdown), code_block_count,
│         confidence, review_flag
│       Write manifest.json
│     Mark step 3 "done" in journal
│
├── 4. MASTER-INDEX UPDATE
│     Read <kb_path>/master-index.json
│     Update knowledge_bases[kb_name]:
│       file_count: recount from manifests
│       topics[]: add any new topics
│       status: "ready" if has harvested entries, "building" if only placeholders
│     Write master-index.json
│     Mark step 4 "done" in journal
│
├── 5. CROSS-REFERENCE UPDATE
│     Collect all tags[], concepts[].name, related_topics[] from batch entries
│     For each term:
│       Look up in master-index.cross_references
│       If exists: add this KB to kbs[] if not already present
│       If new: create entry {description: "", kbs: [this_kb], topics: [entry_topic]}
│     NOTE: Do NOT update entry-level cross_references[] here.
│     Entry-level cross_references are populated only by kb-sync --repair.
│     Write master-index.json
│     Mark step 5 "done" in journal
│
├── 6. BRIDGE DETECTION (skip if no bridge-eligible layer entries in batch)
│     For each entry from a bridge-eligible layer:
│       Check extraction results for bridge_detections array
│       For each detected mapping:
│         Look for existing bridge entry for this descriptor
│         If exists:
│           Corroborating ranges: increase confidence +0.10
│           Contradicting ranges: set review_flag, don't modify existing
│           New parameters: add to existing bridge entry
│         If new:
│           Create bridge entry: confidence 0.50, source.type "auto-detected",
│           review_flag true
│     Run bridge combination scan: for each new/updated bridge entry, scan
│       existing bridges in same category for compatible/conflicting parameters
│     Update bridge layer manifest.json
│     Update master-index with new bridge entries
│     Mark step 6 "done" in journal
│
├── 7. SEARCH TERMS UPDATE
│     Mark used queries as used:true in search-terms.json
│     Extract tags[], concepts[].name, related_topics[] from batch entries
│     For each term: check if it matches an unfilled placeholder entry name
│       If match found and no search terms exist for that placeholder:
│       generate search queries using the discovered term (4 perspectives)
│     Write search-terms.json
│     Mark step 7 "done" in journal
│
├── 8. JOURNAL COMPLETE — mark all steps "done"
│
└── 9. UNLOCK — delete <kb_path>/harvest.lock
```

### Cascade Recovery
If interrupted mid-cascade, the journal records which steps completed. `kb-sync --repair` replays incomplete journals on its next verification pass. Entries written to disk remain valid even if cascade is incomplete.
````

- [ ] **Step 2: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add quality scoring and cascade to SKILL.md"
```

---

## Task 11: SKILL.md — Import system

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append import system section**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Import System

Triggered by `--import <source-path>`. Converts external KB content to our schema.

### Format Detection

If `--format` not specified, auto-detect by scanning source path (check in order):

| Format | Detection Signal | Strategy |
|--------|-----------------|----------|
| `prototype` | Has manifest.json with `kb_name` field, entries have `concepts[]` | Direct field mapping |
| `json-entries` | Directory of .json files with entry-like fields | Claude maps fields to schema |
| `markdown-dir` | Directory of .md files | Each file → original_markdown + Claude extraction |
| `obsidian` | .md files with `[[wikilinks]]` and YAML frontmatter | Frontmatter → fields, wikilinks → cross_references |
| `csv` | .csv or .tsv file with header row | Column headers → field mapping |
| `custom` | User provides `--field-map <mapping.json>` | Mapping file drives conversion |

Fallback: if no format detected, use `json-entries` for .json files, `markdown-dir` for .md files.

### Import Pipeline

1. **Scan** source path — list files, count them, detect directory structure
2. **Detect** format (or use `--format`)
3. **Preview** — show user: N files found, detected format, proposed target layer(s). Ask for confirmation.
4. **For each source file:**
   a. Read source content
   b. Apply format adapter:
      - Known format with direct mapping (prototype, custom with field_map) → map fields directly
      - Other formats → read `prompts/extraction-import.md`, substitute `[format]`, `[kb-layer]`, `[kb-name]`, `[source-path]`, `[field_map_summary]` if applicable. Send content to Claude for extraction.
   c. Store full source content as `original_markdown`
   d. Set `source.backend = "imported"`, `source.type = "external-import"`
   e. Set `source.import_source = { path: "<absolute source path>", format: "<detected>", field_map_used: <bool>, import_date: "<ISO date>" }`
   f. Generate `harvest_metadata`: per-field `field_provenance` using `"direct-mapped"` for adapter fields, `"ai-synthesized"` for Claude-generated fields
5. **Validate** against entry-schema.json
6. **Quality gate** — per-KB quality_floor for `imported` backend (default 0.45)
7. **Summary** — show: N accepted, N flagged for review, N rejected
8. If `--dry-run`: stop here, report what would be imported
9. Otherwise: user approves → write entries → cascade (§ Cascade)

### Custom Field Mapping

If `--field-map <mapping.json>` provided:

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

- `field_map` keys = our schema fields, values = source field names (or objects with `field` + `transform`)
- `unmapped_fields`: `"ai-synthesize"` (Claude generates) or `"skip"` (leave empty/default)
- `id_field`: source field for entry IDs (default: generate from title)
- `id_prefix`: prefix for entry IDs (default: KB name)

### Import Error Handling

| Scenario | Action |
|----------|--------|
| Source file unreadable | Skip, log path and error |
| Format detection fails | Ask user to specify `--format` |
| Empty required fields after mapping | Claude synthesizes from original_markdown if possible, else review_flag |
| Entry already exists in target KB | Re-harvest: version bump, archive old (§ Re-Harvest Versioning) |
| Source is single file (not directory) | Import as single entry, ask user for target layer/topic |
| Source granularity finer than target | Merge related source entries |
| Source granularity coarser than target | Split via multi-topic detection |
````

- [ ] **Step 2: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add import system to SKILL.md"
```

---

## Task 12: SKILL.md — Modes (interactive, auto, URL, list, status)

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append all mode descriptions**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Interactive Mode

Default mode (no `--auto`, `--urls`, or `--import`).

1. Show KB status: layer names, entry counts, placeholder count, harvest-status summary
2. List placeholder entries needing content (read from manifests; if manifests broken, scan directories)
3. If `--layer` or `--topic` specified: filter to those. If `--entry` specified: target only that entry.
4. User selects entries to fill (or "all")
5. For each selected entry:
   a. Generate search terms (§ Search Term Generation). Display for user to edit/add/remove.
   b. Search → display ranked URLs with domain tier labels → user selects URLs (or "all")
   c. Fetch selected URLs → extract → show extracted entry summary (title, confidence, code_block_count, field_provenance)
   d. User: approve → write + cascade (batch 1) | reject → discard | edit → modify fields, then approve

## Auto Mode

`--auto`: Fill all placeholder entries automatically.

1. Find all placeholder entries: scan manifests for `status: "placeholder"`. If manifests broken, scan directories for entries with `"status": "placeholder"`.
2. If `--layer` specified: filter to that layer. If `--topic`: filter to topic. If `--entry`: target single entry.
3. Generate search terms for all targets (§ Search Term Generation)
4. For each entry: run § Harvest Pipeline steps 5-15
5. Cascade runs per batch (every `--batch` entries, default 5)
6. After all entries processed, update `<kb_path>/harvest-status.json`:

```json
{
  "version": "1.0.0",
  "last_updated": "<ISO timestamp>",
  "layers": {
    "<layer>": {
      "total": 25,
      "research_harvested": 12,
      "web_harvested": 8,
      "imported": 0,
      "placeholder": 5,
      "failed": {
        "count": 2,
        "entries": {
          "<entry-id>": { "reason": "all URLs returned empty content", "last_attempt": "<ISO date>" }
        }
      }
    }
  }
}
```

7. Report: N filled, N flagged for review, N failed (with reasons)

## Auto Refresh Mode

`--auto --refresh`: Re-harvest entries below confidence threshold.

1. Read config `refresh_confidence_threshold` (default 0.60)
2. Find entries with `harvest_metadata.overall_confidence < threshold` (exclude placeholders)
3. Re-harvest each: run pipeline, version bump old entry (§ Re-Harvest Versioning)

## Auto All Mode

`--auto --all`: Re-harvest ALL non-placeholder entries.

1. Find all entries with status != "placeholder"
2. Re-harvest each with version bump
3. Useful after extraction prompt improvements or backend changes

## URL Mode

`--urls <url1> <url2> ...`: Harvest specific user-provided URLs.

1. Backend forced to `webfetch`
2. For each URL: WebFetch → extract → score → gate
3. Match extracted content to existing placeholder entry (by topic similarity). If no match: ask user which entry to target, or create new entry.
4. Write → cascade

## List Mode

`--list`: Show registered KBs.

Read `~/.claude/kb-registry.json`. Display table:

| Name | Path | Layers | Default Backend | Registered |
|------|------|--------|-----------------|------------|

## Status Mode

`--status`: Show harvest status across all registered KBs.

For each registered KB:
1. Read `<kb_path>/harvest-status.json` if exists
2. Read manifests to count entries by status
3. Display per-layer: total / harvested / placeholder / failed / avg confidence
````

- [ ] **Step 2: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add mode descriptions to SKILL.md"
```

---

## Task 13: SKILL.md — Checkpoint, resume, rebuild, review, versioning, errors

**Files:**
- Modify: `~/.claude/skills/kb-harvest/SKILL.md` (append)

- [ ] **Step 1: Append remaining sections**

Add to end of `~/.claude/skills/kb-harvest/SKILL.md`:

````markdown

## Checkpointing

After each entry is processed (step 17), update `<kb_path>/harvest-checkpoint.json`:

```json
{
  "session_id": "<ISO-date>-<seq>",
  "started_at": "<ISO timestamp>",
  "backend": "<backend>",
  "batch_size": 5,
  "current_batch": 2,
  "entries": {
    "<entry-id>": { "status": "completed", "confidence": 0.72 },
    "<entry-id>": { "status": "fetch_failed", "reason": "all URLs empty" },
    "<entry-id>": { "status": "in_progress", "urls_fetched": 3, "urls_remaining": 2 },
    "<entry-id>": { "status": "pending" }
  },
  "mode": "auto",
  "seen_urls": ["https://..."],
  "import_state": null,
  "cascade_journal": {
    "batch_1": { "manifest": "done", "master_index": "done", "cross_refs": "done", "bridges": "done", "search_terms": "done" }
  }
}
```

For import mode, `import_state` tracks progress:
```json
"import_state": {
  "source_path": "/path/to/external-kb",
  "format": "json-entries",
  "files_total": 45,
  "files_processed": 23,
  "files_remaining": ["path/to/file24.json", "..."]
}
```

## Resume

`--resume`: Continue an interrupted harvest session.

1. Read `<kb_path>/harvest-checkpoint.json`. If file does not exist: inform user "No harvest session to resume for this KB." and exit.
2. Show session summary: started_at, mode, entries completed/pending/failed
3. If `<kb_path>/harvested/staged/` has entries from interrupted interactive session: show for approval first
4. Continue from `in_progress` entries (resume mid-fetch), then `pending` entries
5. Skip `completed` and `fetch_failed`
6. Restore `seen_urls` to avoid re-fetching

## Re-Harvest Versioning

When kb-harvest targets an existing non-placeholder entry:

1. Read existing entry
2. Bump version: `1.0.0 → 1.1.0` (minor bump)
3. Set `supersedes` field in new entry to existing entry's `id`
4. Move existing entry to `<kb_path>/<layer>/<topic>/_archive/<entry-id>-v<old-version>.json`
5. Write new entry to original file location
6. Update manifest (new version, timestamps, confidence)

Old entries remain in `_archive/` for reference and rollback.

## Manifest Rebuild

`--rebuild-manifest`: Fix broken manifests by reconstructing from actual files.

For each layer directory in the KB:
1. List all .json files (exclude manifest.json, any file starting with `_`)
2. Read each entry file:
   - Extract `status` field
   - Count `code_blocks[]` length → `code_block_count`
   - Measure `original_markdown` length → `markdown_length` (0 if absent)
   - Read `harvest_metadata.overall_confidence` → `confidence` (null if absent)
   - Read `harvest_metadata.review_flag` → `review_flag` (false if absent)
   - Read `source.backend` → `source` (infer "research-docs" if absent)
3. Group entries by topic (from entry's `topic` field)
4. Build manifest:
   ```json
   {
     "kb_name": "<layer-name>",
     "version": "1.0.0",
     "created": "<earliest entry date or now>",
     "last_sync": "<now>",
     "status": "ready",
     "topics": [
       {
         "name": "<topic>",
         "files": {
           "<filename>.json": {
             "status": "harvested",
             "harvested_at": "<from entry or now>",
             "synced_at": null,
             "synced_timestamp": null,
             "has_semantic": true,
             "source": "ddg+webfetch",
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
5. Write `<kb_path>/<layer>/manifest.json`

Report: N layers rebuilt, N total entries found.

## Staged Review

`--review`: Show entries awaiting approval in `<kb_path>/harvested/staged/`.

For each .json file in staged/:
1. Read entry, display summary:
   - Title, topic, layer
   - Confidence score, review_flag
   - Code block count, description length
   - Source URL(s)
2. User options:
   - **approve** → move to final KB location, run cascade
   - **reject** → delete from staged/
   - **skip** → leave in staged/ for later

## Error Handling

### Search Failures

| Scenario | Action |
|----------|--------|
| DDG bot detection (0 results) | Auto-fallback to websearch+webfetch for this query |
| DDG bot detection (partial) | Use whatever results were returned |
| DDG CLI not installed | Fall back to websearch+webfetch, log warning |
| Network error (any backend) | Retry once, then skip query |
| WebSearch 0 results | Try next query from search-terms.json |
| All queries exhausted, no URLs | Entry stays placeholder, log in harvest-status.json |

### Fetch Failures

| Scenario | Action |
|----------|--------|
| URL redirects | Follow with new WebFetch call on redirect URL |
| Auth-required / paywall | Skip, log URL, suggest `--urls` with manual content |
| Empty / login page | Skip, log as unusable, try next URL |
| JavaScript-only page | Skip, note limitation |
| No code blocks (code KB expected) | Second pass with webfetch-code.md prompt |
| Timeout | Skip URL, try next |

### Cascade Failures

| Scenario | Action |
|----------|--------|
| Manifest write fails | Log in journal, kb-sync repairs later |
| Master-index write fails | Log in journal, entries on disk are still valid |
| Lock acquisition fails after retry | Fail with message: "KB is locked by another harvest. Wait or use --kb with different KB." |
| Concurrent session detected | Inform user, suggest waiting |

### Quality Failures

| Scenario | Action |
|----------|--------|
| All URLs produce < 0.30 confidence | Entry stays placeholder, log reason in harvest-status.json |
| No code blocks for code-oriented KB | Lower overall_confidence by 0.10, set review_flag |
| Bridge detection conflicts with existing | Flag for review, don't modify existing bridge |
| Entry schema validation fails | Don't write entry, log specific validation errors |
````

- [ ] **Step 2: Verify SKILL.md is complete**

Check all major sections exist:

```bash
grep "^## " ~/.claude/skills/kb-harvest/SKILL.md | head -30
```

Expected sections (in order):
```
## Invocation
## Defaults
## Mode Routing
## First-Run Setup
## Registry Resolution
## Backend Selection
## Search Term Generation
## Harvest Pipeline (Single Entry)
## Quality Scoring Details
## Cascade — Automatic Infrastructure Updates
## Import System
## Interactive Mode
## Auto Mode
## Auto Refresh Mode
## Auto All Mode
## URL Mode
## List Mode
## Status Mode
## Checkpointing
## Resume
## Re-Harvest Versioning
## Manifest Rebuild
## Staged Review
## Error Handling
```

- [ ] **Step 3: Count total lines**

```bash
wc -l ~/.claude/skills/kb-harvest/SKILL.md
```

Expected: ~600-800 lines (significantly more than the old 236-line Firecrawl-only skill).

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/kb-harvest/SKILL.md
git commit -m "feat(kb-harvest): add checkpoint, resume, rebuild, review, versioning, error handling"
```

---

## Task 14: Delete retired skills

**Files:**
- Delete: `~/.claude/skills/kb-autofill/` (entire directory)
- Delete: `~/.claude/skills/harvest-data/` (entire directory)

- [ ] **Step 1: Verify retired skills exist**

```bash
ls -la ~/.claude/skills/kb-autofill/SKILL.md ~/.claude/skills/harvest-data/SKILL.md 2>&1
```

- [ ] **Step 2: Back up and delete kb-autofill**

```bash
cp -r ~/.claude/skills/kb-autofill/ ~/.claude/skills/kb-autofill.bak/
rm -rf ~/.claude/skills/kb-autofill/
```

- [ ] **Step 3: Back up and delete harvest-data**

```bash
cp -r ~/.claude/skills/harvest-data/ ~/.claude/skills/harvest-data.bak/
rm -rf ~/.claude/skills/harvest-data/
```

- [ ] **Step 4: Verify deletion**

```bash
ls ~/.claude/skills/kb-autofill/ 2>&1
ls ~/.claude/skills/harvest-data/ 2>&1
```

Expected: both show "No such file or directory".

- [ ] **Step 5: Commit**

```bash
git add -A ~/.claude/skills/kb-autofill/ ~/.claude/skills/harvest-data/
git commit -m "chore(kb-harvest): remove retired kb-autofill and harvest-data skills (absorbed into kb-harvest)"
```

---

## Task 15: Validation — initialize registry for prototype KB

**Files:**
- Runtime: `~/.claude/kb-registry.json` (created by skill)

- [ ] **Step 1: Verify prototype KB exists**

```bash
ls /home/myuser/agents/juce-agent/playbookdata/master-index.json
ls /home/myuser/agents/juce-agent/playbookdata/*/manifest.json 2>/dev/null | head -5
```

Expected: master-index.json exists. Several manifest.json files in subdirectories.

- [ ] **Step 2: Create registry manually (simulating first-run)**

```bash
python3 -c "
import json, os, glob
from datetime import datetime

kb_path = '/home/myuser/agents/juce-agent/playbookdata'
layers = []
for d in sorted(os.listdir(kb_path)):
    full = os.path.join(kb_path, d)
    if os.path.isdir(full) and not d.startswith('_') and d != 'harvested':
        jsons = glob.glob(os.path.join(full, '**/*.json'), recursive=True)
        if jsons:
            layers.append(d)

registry = {
    'version': '1.0.0',
    'registries': [{
        'name': 'juce-agent-prototype',
        'path': kb_path,
        'schema_path': None,
        'bridge_schema_path': None,
        'master_index_path': 'master-index.json',
        'generated_by': 'manual',
        'layers': layers,
        'bridge_eligible_layers': [l for l in layers if 'sound-design' in l],
        'default_backend': 'ddg+webfetch',
        'registered_at': datetime.utcnow().isoformat() + 'Z'
    }],
    'default_kb': 'juce-agent-prototype'
}

with open(os.path.expanduser('~/.claude/kb-registry.json'), 'w') as f:
    json.dump(registry, f, indent=2)
print(f'Registered {len(layers)} layers: {layers}')
"
```

Expected: prints layer names discovered from the prototype KB.

- [ ] **Step 3: Verify registry is valid**

```bash
python3 -c "
import json
r = json.load(open('$HOME/.claude/kb-registry.json'))
print(f'Version: {r[\"version\"]}')
print(f'KBs: {len(r[\"registries\"])}')
for kb in r['registries']:
    print(f'  {kb[\"name\"]}: {len(kb[\"layers\"])} layers at {kb[\"path\"]}')
print(f'Default: {r[\"default_kb\"]}')
"
```

Expected: shows 1 KB with correct layer count.

- [ ] **Step 4: Commit registry**

```bash
git add ~/.claude/kb-registry.json
git commit -m "feat(kb-harvest): initialize KB registry with prototype KB"
```

---

## Task 16: Validation — manifest rebuild on prototype KB

Test the manifest rebuild instructions by running them against the prototype KB.

- [ ] **Step 1: Check current manifest state**

```bash
python3 -c "
import json, glob
for mf in sorted(glob.glob('/home/myuser/agents/juce-agent/playbookdata/*/manifest.json')):
    m = json.load(open(mf))
    total = sum(len(t.get('files', {})) for t in m.get('topics', []))
    print(f'{mf}: {total} entries tracked')
"
```

Note: if manifests show 0 entries but .json files exist in subdirectories, the rebuild is needed.

- [ ] **Step 2: Count actual entry files per layer**

```bash
for dir in /home/myuser/agents/juce-agent/playbookdata/*/; do
  layer=$(basename "$dir")
  count=$(find "$dir" -name "*.json" ! -name "manifest.json" -maxdepth 2 2>/dev/null | wc -l)
  echo "$layer: $count entry files"
done
```

- [ ] **Step 3: Verify rebuild logic matches SKILL.md instructions**

Read the § Manifest Rebuild section of SKILL.md and confirm:
- It scans .json files excluding manifest.json
- It reads status, code_blocks count, original_markdown length, confidence, review_flag, source.backend
- It groups by topic
- It writes the manifest format matching the template

```bash
grep -A 5 "## Manifest Rebuild" ~/.claude/skills/kb-harvest/SKILL.md | head -10
```

- [ ] **Step 4: Verify complete skill file is self-consistent**

```bash
# Check all section cross-references resolve
python3 -c "
import re
content = open('$HOME/.claude/skills/kb-harvest/SKILL.md').read()
refs = re.findall(r'§ ([A-Z][^\n§|)]+)', content)
sections = re.findall(r'^## (.+)$', content, re.MULTILINE)
for ref in refs:
    ref_clean = ref.strip().rstrip('.')
    found = any(ref_clean in s for s in sections)
    if not found:
        print(f'BROKEN REF: § {ref_clean}')
if not refs:
    print('No § references found')
else:
    print(f'Checked {len(refs)} references against {len(sections)} sections')
"
```

Expected: No "BROKEN REF" lines. All `§` references match section headers.

- [ ] **Step 5: Final commit if any validation fixes were needed**

```bash
git status
# If changes: git add + commit with appropriate message
```

---

## Summary

| Task | Description | Files | Commit |
|------|-------------|-------|--------|
| 1 | Directory structure + config template | 1 JSON | ✓ |
| 2 | Entry schema template | 1 JSON Schema | ✓ |
| 3 | Bridge + master-index + manifest templates | 3 JSON | ✓ |
| 4 | WebFetch prompts | 2 markdown | ✓ |
| 5 | Extraction prompts | 4 markdown | ✓ |
| 6 | SKILL.md: header, invocation, routing, setup | SKILL.md create | ✓ |
| 7 | SKILL.md: registry, backend selection | SKILL.md append | ✓ |
| 8 | SKILL.md: search term generation | SKILL.md append | ✓ |
| 9 | SKILL.md: harvest pipeline (17 steps) | SKILL.md append | ✓ |
| 10 | SKILL.md: quality scoring + cascade | SKILL.md append | ✓ |
| 11 | SKILL.md: import system | SKILL.md append | ✓ |
| 12 | SKILL.md: all modes | SKILL.md append | ✓ |
| 13 | SKILL.md: checkpoint, resume, rebuild, review, errors | SKILL.md append | ✓ |
| 14 | Delete retired skills | delete 2 dirs | ✓ |
| 15 | Validation: registry init | registry JSON | ✓ |
| 16 | Validation: manifest rebuild test | verification | ✓ |

**Total: 16 tasks, ~16 commits, creates 12 new files + 1 runtime file, deletes 2 skill directories.**
