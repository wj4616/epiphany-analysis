# kb-cognitive

---
name: kb-cognitive
description: Resolution Procedure for querying cognitive knowledge base. Not invoked directly by users. Consumption skills reference this inline.
---

## Overview

This is a **procedure skill** that provides a standardized resolution procedure for accessing the cognitive knowledge base during skill design. It follows the kb-route pattern: consumption skills do not invoke this directly, but instead reference the Resolution Procedure inline.

## Input Parameters

| Parameter | Purpose | Example | Behavior |
|-----------|---------|---------|----------|
| `concept` | Semantic search across all layers | `concept=first-principles` | Searches research, genius-minds, and traits layers for matching content |
| `genius` | Query genius-minds layer | `genius=feynman` | Returns all files about Feynman with confidence scores |
| `trait` | Query traits layer | `trait=T1` | Returns all T1-tier trait files |
| `explore` | Browse layer contents | `explore=genius-minds` | Lists all files in genius-minds layer with titles and confidence |
| `bridge` | Cross-layer concept linking | `bridge=incubation` | Returns research → genius-minds → traits chain for a concept |
| `kb` | Target specific registered KB | `kb=epiphany-cognitive concept=test` | Searches only the named KB |

Natural language queries (free-form text) are also supported and will search across all layers.

## Resolution Procedure

### Step 1: Setup and Registry Loading

1. Read `~/.claude/kb-registry.json` to get available KB configurations
2. If `kb` parameter provided, select that KB by name
3. If no `kb` parameter, use `epiphany-cognitive` as default
4. Load the master-index.json from the KB's configured path
5. Initialize cache if not already loaded (see Step 7 for caching)

### Step 2: Query Format Detection

Detect the query format by examining input:

**Structured Parameter Detection:**
- If input contains `param=value` pattern anywhere, parse as structured parameters
- Extract all parameter-value pairs (concept, genius, trait, explore, kb)
- Multiple parameters are supported: `genius=feynman trait=T1`

**Natural Language Detection:**
- Free-form text without `param=value` patterns
- Extract keywords for semantic search
- Apply to all layers

### Step 3: Search Execution

Execute search based on query type:

**For `concept=<term>`:**
- Search all layers: research, genius-minds, traits
- Match term against title, tags, and content
- Return up to 5 results ordered by relevance

**For `genius=<name>`:**
- Search only genius-minds layer
- Match name against file titles and content
- Return all matching files (up to 5)

**For `trait=<tier>`:**
- Search only traits layer, filtering by tier subdirectory
- T1 → traits/T1/*.md
- T2 → traits/T2/*.md
- T3 → traits/T3-BATCH-RESEARCH.md
- Return all matching files with confidence scores

**For `explore=<layer>`:**
- List all files in specified layer
- Return titles, confidence scores, and brief summaries
- No search performed

**For `bridge=<concept>`:**
- Load bridge-index.json from KB root
- Find concept in concept_bridges array
- Return complete chain: research → genius-minds → traits
- Include all related files for each layer
- Format as connected knowledge graph

**For natural language query:**
- Extract key terms from query
- Search title and content across all layers
- Apply relevance scoring (Step 5)
- Return top 5 results

### Step 4: Confidence Scoring (NFR-3 Algorithm)

Each KB entry has a confidence score (0.40-1.00) calculated as:

```
base_score = 0.70

if entry_has_##_headings:
    base_score += 0.15

if entry_has_references_or_sources:
    base_score += 0.10

if entry_has_frontmatter:
    base_score += 0.05

if entry_has_TODO_or_FIXME_markers:
    base_score -= 0.20

if entry_word_count < 500:
    base_score -= 0.10

final_score = clamp(base_score, 0.40, 1.00)
```

Confidence scores are pre-calculated in master-index.json and retrieved from cache.

### Step 5: Relevance Ordering (NFR-4 Algorithm)

Results are ordered by relevance score:

```
for each matching entry:
    relevance = 0
    
    # Title match bonus
    if query_term_in_title:
        relevance += 3
    
    # Content occurrence bonus
    content_matches = count(query_term in content)
    relevance += min(content_matches, 5)  # Max +5
    
    # Apply confidence multiplier
    relevance = relevance * confidence_score
    
    # Apply layer priority
    if layer == "genius-minds":
        relevance *= 1.2
    elif layer == "traits":
        relevance *= 1.1
    # research layer: no multiplier (1.0)

return results sorted by relevance descending
```

### Step 6: Result Formatting

Output structured knowledge entries:

```json
{
  "results": [
    {
      "source_file": "gm-feynman-first-principles.md",
      "layer": "genius-minds",
      "confidence": 0.90,
      "relevance_score": 4.86,
      "title": "Feynman: First Principles Thinking",
      "content_summary": "Brief summary of content...",
      "applicability": "How this knowledge applies to skill design",
      "tags": ["feynman", "first-principles", "reasoning"]
    }
  ],
  "query": "first principles",
  "total_results": 3,
  "returned": 3
}
```

**Maximum 5 results per query.**

### Step 6a: Confidence Gate (Validation Before Use)

Before applying KB knowledge, apply the confidence gate:

**HIGH CONFIDENCE (≥ 0.85):**
- USE normally
- No warning required
- Knowledge is well-structured with references

**MEDIUM CONFIDENCE (0.70 - 0.84):**
- USE with attribution and caution
- Prepend: `⚠ Medium confidence (0.XX) — attribute and verify critical claims`
- Suitable for supporting points, not primary assertions

**LOW CONFIDENCE (< 0.70):**
- DO NOT USE without explicit user approval
- Prepend: `⛔ Low confidence (0.XX) — requires user validation before use`
- Present to user with: "This knowledge has low confidence. Approve use? [Y/n]"

**CONFLICTING RESULTS:**
- PRESENT BOTH sources with full attribution
- Show: "Conflicting claims detected:"
  - Source A: [claim] (confidence: X.XX)
  - Source B: [claim] (confidence: X.XX)
- Require: User judgment on which to use, or synthesize with both attributed

### Step 7: Confidence Warning Format

If any result requires a confidence warning, format as:

```
⚠ Confidence Advisory

HIGH (≥0.85): gm-feynman-first-principles.md (0.95)
MEDIUM (0.70-0.84): research-conceptual-blending.md (0.85)
LOW (<0.70): [none]

Medium confidence entries should be attributed and critical claims verified.
Low confidence entries require explicit approval before use.
```

### Step 8: Caching

**In-Memory Session Cache Structure:**
```
cache = {
  loaded: boolean,
  kb_name: string,
  parsed_entries: {
    research: [...],
    genius-minds: [...],
    traits: [...]
  },
  file_timestamps: {
    "file1.md": timestamp,
    "file2.md": timestamp,
    ...
  },
  confidence_scores: {
    "entry_id": score,
    ...
  }
}
```

**Cache Loading:**
1. On first query, check if cache.loaded is false
2. Read all .md files from KB directory
3. Parse metadata, calculate confidence scores
4. Store file modification timestamps
5. Set cache.loaded = true

**Cache Validation:**
- Before each query, check if any file timestamps have changed
- If changed, rebuild cache for affected layer only
- Cache is session-scoped (not persisted to disk)

### Step 9: Error Handling

**Missing KB Entry:**
```
No results found for [query].
Available layers: research, genius-minds, traits
Try: concept=<term>, genius=<name>, trait=<tier>, explore=<layer>
```

**Malformed File:**
- Skip file with warning
- Set confidence to 0.40 (minimum)
- Continue processing other files

**Empty Directory:**
```
Layer [layer_name] is empty. No files found.
Available layers: [list of non-empty layers]
```

**Invalid Parameter:**
```
Unknown parameter: [param]. 
Valid parameters: concept, genius, trait, explore, kb
```

## Usage Instructions

Consumption skills (thinking skills like epiphany-genius, brainstorming, prompt-epiphany) reference this procedure inline:

```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-cognitive/SKILL.md
```

During skill design, when you need cognitive KB knowledge:

1. Invoke the Resolution Procedure with your query
2. Receive up to 5 ranked results with confidence scores
3. **Apply the Confidence Gate (Step 6a):**
   - HIGH (≥0.85): Use normally
   - MEDIUM (0.70-0.84): Use with attribution and caution
   - LOW (<0.70): Requires explicit user approval
   - CONFLICTING: Present both, require user judgment
4. Apply the knowledge to your skill design

**Example Invocation:**
```
Query: "how does feynman approach first principles"
Resolution: Search all layers → genius-minds matches → return top results
Confidence Gate: HIGH (0.95) → USE normally
```

**Example Structured Query:**
```
Query: "genius=feynman"
Resolution: Search genius-minds layer only → return all Feynman files
Confidence Gate: All HIGH → USE normally
```

## Verification

This skill implements:
- FR-1 through FR-13 (Functional Requirements)
- NFR-1 through NFR-6 (Non-Functional Requirements)
- IR-1 through IR-7 (Interface Requirements)
- DR-1 through DR-5 (Data Requirements)
- C-1 through C-6 (Constraints)

See specification at `~/docs/epiphany/prompts/10-04-kb-cognitive-access-system-spec.md` for complete requirements.