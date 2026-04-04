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