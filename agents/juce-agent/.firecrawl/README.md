# Firecrawl Configuration for JUCE Audio Plugin Development

This directory contains configuration and helper scripts for harvesting web content into the playbook.

## Quick Start

```bash
# Source the configuration
source /home/myuser/agents/juce-agent/.firecrawl/config.sh

# Get help
firecrawl-help

# Check status
firecrawl-status
```

## Helper Functions

### Search Functions

```bash
# Search JUCE documentation and forums
firecrawl-search-juce "filter design"

# Search DSP educational content
firecrawl-search-dsp "biquad filter"
```

### Harvest Functions

```bash
# Harvest a single page
firecrawl-harvest-page "https://example.com/dsp-article"

# Harvest JUCE documentation
firecrawl-harvest-juce-docs "tutorials/audio-plugin"

# Harvest a DSP blog
firecrawl-harvest-dsp-blog "https://www.earlevel.com/main/2023/article/"

# Harvest and add to Knowledge Base
firecrawl-harvest-to-kb "https://soundonsound.com/synth-secrets" "sound-design" "synth-secrets"
```

### Crawl Functions

```bash
# Crawl an entire documentation section
firecrawl-crawl-docs "https://docs.juce.com/master/" "juce-docs"

# Map a site to find URLs
firecrawl-map-site "https://melatonin.dev/blog/"
```

### Status Functions

```bash
# Check configuration and credits
firecrawl-status

# List harvested content
firecrawl-list-harvested

# List by category
firecrawl-list-harvested "juce"
firecrawl-list-harvested "dsp"
```

## Output Location

All harvested content is saved to:
```
/home/myuser/agents/juce-agent/playbookdata/firecrawl/
```

## File Naming Convention

| Type | Pattern | Example |
|------|---------|---------|
| Search | `search-{query}.json` | `search-filter-design.json` |
| JUCE Doc | `juce-{path}.json` | `juce-audioprocessor.json` |
| DSP Blog | `dsp-{name}.json` | `dsp-earlevel-filter.json` |
| Sound Design | `sound-design-{name}.json` | `sound-design-synth-secrets.json` |
| Site Map | `map-{domain}.json` | `map-juce-forum.json` |

## Categories

The system organizes harvested content by category:

| Category | Description | Include Hosts |
|----------|-------------|---------------|
| `juce` | JUCE documentation and forums | juce.com, forum.juce.com, docs.juce.com |
| `dsp` | DSP educational content | earlevel.com, ccrma.stanford.edu, musicdsp.org |
| `sound-design` | Sound design theory | soundonsound.com, attackmagazine.com |
| `testing` | Testing frameworks | github.com/catchorg, github.com/sudara |

## Integration with Playbook

Harvested content is indexed in:
```
/home/myuser/agents/juce-agent/playbookdata/firecrawl/ENHANCED_MASTER_INDEX.md
```

To add content to the playbook:

1. Harvest the content:
   ```bash
   firecrawl-harvest-to-kb "https://example.com" "category" "name"
   ```

2. Update the playbook with references:
   ```bash
   # Edit the playbook to reference the new content
   # Add source to research_sources section
   ```

## Rate Limiting

- Max concurrent requests: 2 (free tier)
- Delay between requests: 0.5 seconds
- Be respectful of source servers

## Credits

Check your credit balance:
```bash
firecrawl credit-usage
```

## Troubleshooting

### "Not authenticated"
```bash
firecrawl login --browser
```

### "No credits remaining"
- Check usage: `firecrawl credit-usage`
- Wait for credit reset or upgrade plan

### "Rate limited"
- Wait a few minutes
- Reduce concurrent requests
- Increase delay between requests

## Raw Firecrawl Commands

```bash
# Basic scrape
firecrawl scrape "https://example.com" -o output.md

# Scrape with format options
firecrawl scrape "https://example.com" --format markdown,links -o output.json

# Search and scrape results
firecrawl search "juce filter" --scrape --limit 5

# Map a site
firecrawl map "https://juce.com" --search "tutorial" -o map.json

# Crawl a site section
firecrawl crawl "https://docs.juce.com/master/" --limit 50
```