---
name: harvest-data
description: Ad-hoc harvesting of web data with quality filtering.
             Auto-categorizes to KB, preserves code formatting.
             Integrates with kb-harvest and kb-sync.
---

# Harvest Data

Harvest web content into the knowledge base with automatic quality filtering
and KB categorization.

## Invocation

/harvest-data --topic "TOPIC" [options]

## Options

--topic "..."          Required. Topic to harvest
--context "..."        Additional context for better search terms
--kb KB_NAME           Force specific KB (auto-detect if not specified)
--new-kb KB_NAME       Create new KB with this name
--dry-run              Show queries without executing
--max-results N        Max results per query type (default: 15)
--min-length N         Minimum content length (default: 500)
--min-relevance N      Minimum relevance score (default: 0.3)
--max-noise N          Maximum noise ratio (default: 0.5)
--credits-budget N     Credit limit for this harvest (default: 2000)

## Workflow

1. Determine KB category (or create new)
2. Create placeholder file if needed
3. Generate search queries
4. Execute Firecrawl searches
5. Deduplicate results by URL
6. Filter results by quality
7. Store raw JSON with metadata
8. Report results and credit usage

## Quality Filtering

Results must pass quality checks:
- Minimum content length: 500 characters (configurable)
- Relevance score >= 0.3 (configurable, weighted by source domain)
- Noise ratio <= 0.5 (navigation/ads)

## Integration

After harvest:
1. Review stored content in harvested/raw/
2. Run /kb-harvest --merge to merge into KB files
3. Run /kb-sync --run to sync indexes

## Examples

# Basic harvest
/harvest-data --topic "convolution reverb"

# With context for better results
/harvest-data --topic "FFT" --context "Implementing fast Fourier transform in JUCE audio plugin"

# Force specific KB
/harvest-data --topic "OpenGL" --kb ui-kb

# Create new KB
/harvest-data --topic "Faust DSP" --new-kb faust-kb

# Preview without executing
/harvest-data --topic "reverb" --dry-run