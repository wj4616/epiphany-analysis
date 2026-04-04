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