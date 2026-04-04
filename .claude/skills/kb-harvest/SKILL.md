---
name: kb-harvest
description: "Unified KB population — multi-backend web harvesting, import, and infrastructure maintenance. Use for all KB content population tasks."
---

# kb-harvest — Unified KB Population Skill

Populates Knowledge Bases from web sources (DDG, WebSearch, WebFetch, Firecrawl), local files (import), or user-provided URLs. Every harvest operation automatically maintains all KB infrastructure: manifests, master-index, cross-references, and bridge entries.

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
kb-harvest --kb <name> --firecrawl                  # Use Firecrawl (requires FIRECRAWL_API_KEY)
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
| `--batch` | `batch_size_default` from config (default 5) |
| mode | Interactive (no `--auto`, `--urls`, or `--import`) |

`--urls` forces `webfetch` backend. `--import` forces `imported` backend. `--firecrawl` forces `firecrawl` backend.

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

3. **Firecrawl API Key:** If `--firecrawl` flag or user says "use firecrawl":
   - Check if `FIRECRAWL_API_KEY` environment variable is set
   - If NOT set:
     - Prompt user: "Firecrawl requires an API key. Set FIRECRAWL_API_KEY environment variable or press Enter to use default backend instead."
     - If user provides key: `export FIRECRAWL_API_KEY="<key>"` for this session
     - If user declines: fall back to `--backend ddg+webfetch`
   - If set: proceed with Firecrawl backend

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
3. If `--firecrawl` present → `firecrawl` (after API key check)
4. If `--backend <name>` specified → use that backend
5. Else → registry entry's `default_backend`
6. If resolved backend is `ddg+webfetch`:
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

### Firecrawl Usage

When backend is `firecrawl`:
- Use Firecrawl CLI: `firecrawl search "[query]" --scrape -o .firecrawl/result.json --json`
- `--scrape` fetches full page content for each result
- Results include full markdown content, not just snippets
- Credit tracking: each search + scrape uses credits (check `firecrawl credit-usage`)
- Limits: use `--limit N` to cap results (default 10)
- For specific URLs: `firecrawl scrape "<url>" -o .firecrawl/page.md`

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

**Firecrawl backend:**
```bash
firecrawl search "[query]" --scrape --limit 10 -o .firecrawl/result.json --json
```
Extract URLs from results.

**webfetch backend:** Skip — user provided URLs via `--urls`.

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