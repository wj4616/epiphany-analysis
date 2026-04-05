---
name: kb-route
description: Resolution Procedure for querying multi-layer Knowledge Bases. Consumption skills reference this inline to find concept entries, bridge translations, and explore KB contents. Not invoked directly by users.
---

# kb-route — KB Resolution Procedure

Shared resolution procedure for querying Knowledge Bases. Consumption skills include this by adding:

```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
```

This skill is NOT invoked directly by users. It is a procedure that consumption skills (sound-design-bridge, dsp-implementation, ui-bridge, plugin-spec, daw-testing) reference inline.

## Input Parameters

Consumption skills provide whichever parameters apply to their current task:

| Parameter | Purpose | Example |
|---|---|---|
| `concept` | Find entries about a technical concept | `"filter resonance"` |
| `bridge_descriptor` | Find bridge translation for a subjective descriptor | `"warm"` |
| `explore` + `layer` | Browse what's available in a layer | `layer="technical"` |
| `kb` (optional) | Target a specific registered KB | `"juce-agent-prototype"` |

If both `concept` and `bridge_descriptor` are provided, Steps 2 and 4 both run. Results are returned grouped (concept results, then bridge results). The consumption skill reconciles — it has the domain context.

## Resolution Procedure

### Step 1: Setup *(always runs)*

1. Read `~/.claude/kb-registry.json`. Get list of registered KBs from the `registries[]` array, each with `name`, `path`, `layers[]`, `bridge_eligible_layers[]`.
2. Narrow to relevant KB(s):
   - If consumption skill specifies `kb=<name>` → use that KB only
   - Otherwise → use all registered KBs
3. For each relevant KB, read its master-index at `<kb.path>/master-index.json`.
   - **New format** (PBCPB-generated): look for `kb_layers[]` array, each with `name`, `topics[]`, `authority_score`
   - **Prototype format**: look for `knowledge_bases{}` object, each key being a layer name with `topics[]`
   - Extract: layer names, topic lists, `cross_layer_mappings[]` if present
4. If registry file doesn't exist → report "No KB registry found at ~/.claude/kb-registry.json. Run kb-harvest to initialize." **Stop.**
5. If registry exists but `registries[]` is empty → report "No KBs registered. Run `kb-harvest --kb <name>` to create one." **Stop.**
6. If master-index is missing for a registered KB → skip that KB, warn: "KB [name] registered but master-index.json not found — run `kb-sync --repair`"
7. If master-index format is unrecognized (neither `kb_layers` array nor `knowledge_bases` object) → skip that KB, warn: "KB [name] master-index format not recognized"

### Step 2: Concept Lookup *(skip if no `concept` parameter)*

1. For each layer listed in the master-index, use Grep to search entry files:
   ```
   Grep for concept term (case-insensitive, literal string) in *.json files
   recursively under <kb.path>/<layer>/.
   Exclude infrastructure files: manifest.json, index.json.
   ```
   Grep is a coarse filter — it finds candidate files. Read the actual files and evaluate relevance.

2. Read up to 5 matching entry files. If Grep returns more than 5, prioritize:
   - `status`: curated/synced over harvested over placeholder
   - Match strength: concept in `title` (strong) > `tags` (medium) > `concepts[].name` (weak)
   - Higher `domain_relevance` score
   - If 20+ matches, note: "Many matches — showing top 5. Narrow concept term for more targeted results."

3. For each entry, check `status`:
   - `curated` or `synced` → usable, proceed to Step 5
   - `harvested` → usable, proceed to Step 5 for confidence check
   - `placeholder` → invoke kb-harvest to fill:
     ```
     kb-harvest --kb <kb_name> --auto --entry <entry_id> --batch 1
     ```
     Wait for completion. Re-read entry. If harvest fails or takes too long, note failure and continue with other results.

**Failures:**
- Grep returns no matches across all layers → fall through to Step 6 (gap detection)
- Entry file is malformed JSON → skip entry, warn: "Entry [filename] invalid JSON — run `kb-sync --verify`"
- Placeholder harvest reports error → note: "Harvest failed for [entry_id]." Continue with other results.
- Placeholder harvest appears to hang → agent uses own judgment. Note and continue — don't block indefinitely.
- Multiple entries cover same concept → return all, ordered by status (curated > synced > harvested) then `domain_relevance`. Consumption skill picks.

### Step 2b: Explore *(skip if no `explore` parameter)*

1. Read the manifest at `<kb.path>/<layer>/manifest.json`.
2. List topics (or `categories` for bridge layers) and their entries with status counts.
3. If manifest is missing or has empty entries, fall back to listing files in the layer directory via Glob: `<kb.path>/<layer>/**/*.json`.
4. Return the overview — do not read individual entries unless asked.

**Failures:**
- Specified layer doesn't exist in master-index → list available layers: "Layer [name] not found. Available: [list]"
- Manifest missing → report, fall back to Glob listing
- Manifest exists but entries arrays empty → fall back to Glob, note: "Manifest out of date — run `kb-sync --repair`."

### Step 3: Cross-Reference Follow *(skip if Step 2 was skipped or found no entries with `cross_references[]`)*

Follows **entry-level** cross-references — links from one entry to a specific entry in another layer.

1. For each entry found in Step 2, check its `cross_references[]` field:
   ```json
   "cross_references": [
     { "kb": "technical", "entry_id": "vst_technical_filter-design", "relationship": "implements" }
   ]
   ```
2. Read the referenced entries. Locate by: Glob for `<kb.path>/<cross_ref.kb>/**/<entry_id>.json` (entry IDs match filenames). If Glob finds nothing, fall back to Grep for `"id": "<entry_id>"` in that layer directory.
3. These are secondary results — include them but mark as cross-referenced.
4. If entries have `related_topics[]`, note them in results as "Related topics: [list]". Do not auto-follow — the consumption skill decides whether to explore further.

**Failures:**
- `cross_references[]` target not found on disk → skip, note: "Cross-reference to [entry_id] not found"
- `cross_references[]` target KB not in registry → skip, note: "Cross-reference to KB [name] — not registered"
- Entry has no `cross_references` field → normal, skip Step 3 for that entry

### Step 4: Bridge Resolution *(skip if no `bridge_descriptor` parameter)*

1. Locate the bridge layer directory from `cross_layer_mappings` in the master-index (look for `"relationship": "translates"`). If no `cross_layer_mappings`, check if any layer is named `bridge` and use that.
2. **Direct lookup**: Bridge filenames follow `bridge_{category}_{descriptor}.json`. Try Glob: `<kb.path>/bridge/*/bridge_*_<descriptor>.json`.
3. **Grep fallback**: If direct lookup fails, Grep the bridge directory for the descriptor string in `"descriptor"` fields.
4. Read matching bridge entry. Key fields:
   - `parameters[]` — each with `parameter`, `value_range [min, max]`, `typical_default`, `notes`
   - `confidence` — bridge-level confidence score (top-level field)
   - `why` — human-readable rationale
   - `anti_patterns[]` — each with `mistake` and `reason`
   - `combinations[]` — each with `compatible_with`, `confidence_modifier`, `notes`
5. **Composing multiple bridge entries** (e.g., "warm analog" = warm + analog):
   - Read each bridge entry separately
   - Check `combinations[]` for `compatible_with` references between them
   - If compatible: merge parameter lists, apply `confidence_modifier`, intersection of `anti_patterns`
   - If not listed as compatible: compose with lowered confidence (multiply each by 0.8), union of `anti_patterns`

**Failures:**
- No bridge layer in KB → skip Step 4 entirely (normal)
- Bridge descriptor not found → continue without bridge data if concept results exist; otherwise fall to Step 6
- Conflicting `anti_patterns` between composed bridges → return both, warn: "Conflicting anti_patterns between [A] and [B]: [details]. Review before combining."
- Bridge entry confidence < 0.40 → exclude, note: "Bridge entry [id] excluded (confidence X.XX)."

### Step 5: Confidence Filter *(always runs on collected results)*

For each result entry, determine confidence:
1. Check `harvest_metadata.overall_confidence` (present on harvested entries from kb-harvest)
2. If absent, check top-level `confidence` field (present on bridge entries)
3. If neither exists → **unscored** — usable with note "confidence unknown"

Apply thresholds:

| Confidence | Action |
|---|---|
| >= 0.60 | Use normally |
| 0.40 - 0.59 | Use with warning: "Medium confidence (X.XX) — verify before relying on this" |
| < 0.40 | Exclude. Note: "Entry [id] excluded (confidence X.XX)" |
| Unscored | Use with note: "No confidence score available" |

Order results: scored entries sorted by confidence descending, then unscored entries grouped after.

**Edge cases:**
- All results filtered out by confidence → fall to Step 6 with note
- Mix of scored and unscored → return both, scored first by confidence descending, unscored after
- `authority_score` present on some layers, absent on others → use where present for ordering, treat absent as equal priority

### Step 6: Gap Detection *(runs if no usable results from any previous step)*

Differentiated by cause:

| Why no results | Report | Suggestion |
|---|---|---|
| No registered KB has layers relevant to the query | "No KB with relevant layers for [concept]" | "Register a KB covering this domain, or check with `kb-harvest --list`" |
| KB exists, no entries match the concept | "No entries found for [concept] in [KB name]" | "`kb-harvest --kb <name> --topic <suggested_topic>`" |
| Topic exists but all entries are placeholders and harvest failed | "Topic exists but content not yet harvested" | "`kb-harvest --kb <name> --auto --entry <id>`" |
| Entries found but all below confidence threshold | "Entries exist but all below confidence threshold" | "`kb-harvest --kb <name> --auto --refresh`" |

**Do not auto-harvest.** The agent decides whether to harvest now or proceed with its own knowledge.

### Result Summary

After completing the procedure, summarize collected results for the consumption skill:

For each **concept result**: entry ID, title, status, confidence (or "unscored"), layer, KB name. If cross-referenced entries were followed, list them as secondary with their relationship type.

For each **bridge result**: descriptor, parameters with value ranges, confidence, anti-patterns, combination notes (if composed).

For **gaps**: the gap report message and suggested kb-harvest command.

The consumption skill uses these results directly — kb-route does not format output into a specific structure. The results are in the agent's working context from having read the entry files during the procedure.