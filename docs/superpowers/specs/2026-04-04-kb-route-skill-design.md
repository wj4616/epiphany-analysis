# kb-route Skill — Design Specification

> **Scope:** This spec covers the `kb-route` skill only. Consumption skill rewrites (sound-design-bridge, dsp-implementation, ui-bridge, plugin-spec, daw-testing) are a separate spec after kb-route is built and tested.

## 1. Overview

**Problem:** Consumption skills duplicate identical KB access logic (registry resolution, placeholder detection, confidence checking). None can do cross-KB queries, bridge lookups, or master-index-driven concept search. The KB is sparse now (25/30 placeholders) and will be populated over time via kb-harvest — the routing system must handle this gracefully.

**Solution:** A single `kb-route` skill (`~/.claude/skills/kb-route/SKILL.md`) containing a Resolution Procedure that consumption skills reference inline. Pure SKILL.md — no Python.

**Key decisions:**
- Dedicated skill with inline reference (not separate invocation, not duplicated template)
- Single linear procedure with skip gates per input parameter
- Pure SKILL.md — Python escape hatch available later if scale demands
- Arbitrary bridge directions supported (not hardcoded to sound-design ↔ technical)
- Gap detection signals missing coverage + suggests kb-harvest, does not auto-harvest
- Scope: kb-route only. Consumption skill rewrites are a separate project.

## 2. Architecture

### 2.1 How It Works

```
Consumption Skill (e.g., future sound-design-bridge replacement)
  │
  │  "Read and follow Resolution Procedure in kb-route/SKILL.md"
  │
  ▼
kb-route Resolution Procedure
  │
  ├─ Step 1: Setup (always)
  │    Read registry → narrow to relevant KB(s) → read master-index(es)
  │
  ├─ Step 2: Concept Lookup (if concept= provided)
  │    Grep KB files for concept → read top matches → handle placeholder/status
  │
  ├─ Step 2b: Explore (if explore= provided)
  │    Read manifest → list topics and entries
  │
  ├─ Step 3: Cross-Reference Follow (if Step 2 found entries with cross_references[])
  │    Read entry-level cross-references → follow to other layers/KBs
  │
  ├─ Step 4: Bridge Resolution (if bridge_descriptor= or bridge-eligible results)
  │    Find bridge entries → compose if multiple → check anti_patterns
  │
  ├─ Step 5: Confidence Filter (always, on collected results)
  │    ≥0.60 use │ 0.40-0.59 warn │ <0.40 exclude │ unscored note
  │
  └─ Step 6: Gap Detection (if no usable results)
       Report gap → suggest kb-harvest command → agent decides
```

### 2.2 Input Parameters

Consumption skills provide whichever parameters apply:

| Parameter | Purpose | Example |
|---|---|---|
| `concept` | Find entries about a technical concept | `"filter resonance"` |
| `bridge_descriptor` | Find bridge translation for a subjective descriptor | `"warm"` |
| `explore` + `layer` | Browse what's available in a layer | `layer="technical"` |
| `kb` (optional) | Target a specific registered KB | `"juce-agent-prototype"` |

If both `concept` and `bridge_descriptor` are provided, Steps 2 and 4 both run. Results returned grouped (concept results, then bridge results). Consumption skill reconciles — it has the domain context.

### 2.3 File Location

```
~/.claude/skills/kb-route/
  SKILL.md          # The routing skill with Resolution Procedure
```

Single file. No subdirectories, no templates, no config files. All configuration lives in existing infrastructure (kb-registry.json, kb-harvest-config.json).

## 3. Resolution Procedure (Detailed)

### Step 1: Setup *(always runs)*

1. Read `~/.claude/kb-registry.json`. Get list of registered KBs, each with `name`, `path`, `layers[]`, `bridge_eligible_layers[]`.
2. Narrow to relevant KB(s):
   - If consumption skill specifies `kb=<name>` → use that KB only
   - Otherwise → use all registered KBs
3. For each relevant KB, read its master-index at `<kb.path>/master-index.json`.
   - **New format** (PBCPB-generated): look for `kb_layers[]` array, each with `name`, `topics[]`, `authority_score`
   - **Prototype format**: look for `knowledge_bases{}` object, each key being a layer name with `topics[]`
   - Extract: layer names, topic lists, `cross_layer_mappings[]` if present

### Step 2: Concept Lookup *(skip if no `concept` parameter)*

1. Use Grep to search across entry files in the relevant KB(s):
   ```
   Grep for concept term (case-insensitive, literal string) in *.json files
   under <kb.path>/<layer>/<topic>/ directories.
   Exclude infrastructure files: manifest.json, master-index.json, index.json.
   ```
   Grep is a coarse filter — it finds candidate files. The agent reads the actual files and evaluates relevance.
2. Read up to 5 matching entry files. If Grep returns more than 5, prioritize using these criteria:
   - `status`: curated/synced over harvested over placeholder
   - Match strength: concept in `title` (strong) > `tags` (medium) > `concepts[].name` (weak)
   - Higher `domain_relevance` score
   - If 20+ matches, note: "Many matches — showing top 5. Narrow concept term for more targeted results."
4. For each entry, check `status`:
   - `curated` or `synced` → usable, proceed to Step 5
   - `harvested` → usable, proceed to Step 5 for confidence check
   - `placeholder` → invoke kb-harvest to fill:
     ```
     kb-harvest --kb <kb_name> --auto --entry <entry_id> --batch 1
     ```
     Wait for completion. Re-read entry. If harvest fails or agent judges it's taking too long, note failure and continue with other results.

### Step 2b: Explore *(skip if no `explore` parameter)*

1. Read the manifest at `<kb.path>/<layer>/manifest.json`.
2. List topics and their entries with status counts.
3. If manifest is missing or has empty entries, fall back to listing files in the layer directory via Glob.
4. Return the overview — do not read individual entries unless asked.

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
4. Also check `related_topics[]` — these are topic-name references within the same layer. If the agent needs broader context, Grep for entries in those topics.

### Step 4: Bridge Resolution *(skip if no `bridge_descriptor` parameter AND Step 2/3 found no entries in bridge-eligible layers)*

**Bridge-eligible** means: the master-index has `cross_layer_mappings` with `"relationship": "translates"` pointing to/from a bridge layer, OR the registry lists the source layer in `bridge_eligible_layers`.

1. Locate the bridge layer directory from `cross_layer_mappings` in the master-index.
2. **Direct lookup**: Bridge filenames follow `bridge_{category}_{descriptor}.json`. Try reading `<kb.path>/bridge/*/bridge_*_<descriptor>.json` via Glob.
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

### Step 5: Confidence Filter *(always runs on collected results)*

For each result entry, determine confidence:
1. Check `harvest_metadata.overall_confidence` (present on harvested entries from kb-harvest)
2. If absent, check top-level `confidence` field (present on bridge entries)
3. If neither exists → **unscored** — usable with note "confidence unknown"

Apply thresholds:

| Confidence | Action |
|---|---|
| ≥ 0.60 | Use normally |
| 0.40 – 0.59 | Use with warning: "Medium confidence (X.XX) — verify before relying on this" |
| < 0.40 | Exclude. Note: "Entry [id] excluded (confidence X.XX)" |
| Unscored | Use with note: "No confidence score available" |

Order results: scored entries sorted by confidence descending, then unscored entries grouped after.

### Step 6: Gap Detection *(runs if no usable results from any previous step)*

Differentiated by cause:

| Why no results | Report | Suggestion |
|---|---|---|
| No registered KB has layers relevant to the query | "No KB with relevant layers for [concept]" | "Register a KB covering this domain, or check with kb-harvest --list" |
| KB exists, no entries match the concept | "No entries found for [concept] in [KB name]" | "kb-harvest --kb <name> --topic <suggested_topic>" |
| Topic exists but all entries are placeholders and harvest failed | "Topic exists but content not yet harvested" | "kb-harvest --kb <name> --auto --entry <id>" |
| Entries found but all below confidence threshold | "Entries exist but all below confidence threshold" | "kb-harvest --kb <name> --auto --refresh" |

**Do not auto-harvest.** The agent decides whether to harvest now or proceed with its own knowledge.

## 4. Edge Cases & Error Handling

### Step 1 Failures

| Condition | Behavior |
|---|---|
| Registry file doesn't exist | Report "No KB registry found. Run kb-harvest to initialize." Stop. |
| Registry exists but no registries | Report "No KBs registered. Run kb-harvest --kb <name> to create one." Stop. |
| Master-index missing for a registered KB | Skip that KB, warn: "KB [name] registered but master-index.json not found — run kb-sync --repair" |
| Master-index unrecognized format (neither `kb_layers` array nor `knowledge_bases` object) | Skip that KB, warn: "KB [name] master-index format not recognized" |

### Step 2 Failures

| Condition | Behavior |
|---|---|
| Grep returns no matches | Fall through to Step 6 (gap detection) |
| Entry file is malformed JSON | Skip entry, warn: "Entry [filename] invalid JSON — run kb-sync --verify" |
| Placeholder harvest reports error | Note: "Harvest failed for [entry_id]." Continue with other results. |
| Placeholder harvest appears to hang | Agent uses own judgment. Note and continue — don't block indefinitely. |
| Multiple entries cover same concept | Return all, ordered by status (curated > synced > harvested) then `domain_relevance`. Consumption skill picks. |

### Step 2b Failures

| Condition | Behavior |
|---|---|
| Specified layer doesn't exist in master-index | List available layers: "Layer [name] not found. Available: [list]" |
| Manifest missing for layer | Report, fall back to Glob listing of layer directory. |
| Manifest exists but entries arrays empty | Fall back to Glob. Note: "Manifest out of date — run kb-sync --repair." |

### Step 3 Failures

| Condition | Behavior |
|---|---|
| `cross_references[]` target not found on disk | Skip, note: "Cross-reference to [entry_id] not found" |
| `cross_references[]` target KB not in registry | Skip, note: "Cross-reference to KB [name] — not registered" |
| Entry has no `cross_references` field | Normal. Skip Step 3 for that entry. |

### Step 4 Failures

| Condition | Behavior |
|---|---|
| No bridge layer in KB | Skip Step 4 entirely. Normal. |
| Bridge descriptor not found | Continue without bridge data if concept results exist; otherwise fall to Step 6. |
| Conflicting `anti_patterns` between composed bridges | Return both, warn: "Conflicting anti_patterns between [A] and [B]: [details]. Review before combining." |
| Bridge entry confidence < 0.40 | Exclude. Note: "Bridge entry [id] excluded (confidence X.XX)." |

### Step 5 Edge Cases

| Condition | Behavior |
|---|---|
| All results filtered out by confidence | Fall to Step 6 with note: "Entries found but all below confidence threshold." |
| Mix of scored and unscored | Return both. Scored first by confidence descending, unscored after with note. |
| `authority_score` present on some layers, absent on others | Use where present for ordering. Where absent, treat as equal priority. |

### Cross-Cutting

| Condition | Behavior |
|---|---|
| Both `concept` and `bridge_descriptor` provided | Run Steps 2 and 4. Return results grouped. Consumption skill reconciles. |
| Multiple registered KBs match | Return from all, grouped by KB. Order within each by status then domain_relevance. |
| Entry version field varies | Currently one file per entry. If future append-only versioning creates multiple files, prefer highest version with best status. |

## 5. Maintainability

| Change | How the system handles it |
|---|---|
| New KB registered | Added to kb-registry.json → kb-route discovers at Step 1 |
| New layer added to existing KB | Master-index updated → kb-route sees at Step 1 |
| New bridge direction | cross_layer_mappings added to master-index → Step 4 follows |
| Schema field renamed | Update field references in ONE file (kb-route SKILL.md) |
| New entries added via kb-harvest | Grep-based search finds them immediately — no routing update needed |
| Manifest accuracy drift | Grep doesn't depend on manifests. Explore mode falls back to Glob. kb-sync --repair fixes manifests. |
| Master-index format (new vs prototype) | Step 1 auto-detects format |
| Confidence thresholds need tuning | Update ONE table in kb-route SKILL.md |

## 6. Testing & Validation

### Test 1: Concept Lookup — New KB

**Precondition:** New KB registered with entry `vst_sound-design_subtractive` (status=harvested).

**Input:** `concept="subtractive synthesis"`

**Expected:**
- Step 1: reads registry, reads new KB master-index
- Step 2: Grep finds `vst_sound-design_subtractive.json` (title match)
- Step 3: follows `cross_references` → `vst_technical_filter-design` (placeholder — notes gap or triggers harvest)
- Step 5: primary entry is unscored (no harvest_metadata) — returns with note
- Result: primary entry + cross-referenced entry with status notes

### Test 2: Bridge Lookup — New KB

**Input:** `bridge_descriptor="warm"`

**Expected:**
- Step 4: finds `bridge/timbre/bridge_timbre_warm.json` via Glob
- Returns: 3 parameters (filter_cutoff, filter_resonance, saturation_amount), confidence 0.85, anti_patterns (2), combinations (1)

### Test 3: Concept Lookup — Prototype KB

**Input:** `concept="reverb"`

**Expected:**
- Step 1: reads prototype master-index (detects `knowledge_bases{}` format)
- Step 2: Grep finds multiple entries in `dsp-kb/reverb/`
- Picks top 5 by status + domain_relevance
- No bridge layer → Step 4 skipped
- Returns: ranked entries with status/confidence notes

### Test 4: Gap Detection

**Input:** `concept="granular delay"`

**Expected:**
- Step 2: Grep finds no matches (or only tangential)
- Step 6: "No entries found for 'granular delay' in [KB name]"
- Suggests: `kb-harvest --kb <name> --topic <suggested_topic>`

### Test 5: Explore Mode

**Input:** `explore=true, layer="bridge"`

**Expected:**
- Step 2b: reads bridge manifest (or falls back to Glob if manifest has empty entries)
- Returns: categories (timbre, dynamics, space, movement, character) with entry counts

### Test 6: Multiple KBs

**Precondition:** Both prototype and new KB registered.

**Input:** `concept="filter"`

**Expected:**
- Step 2: Grep searches both KBs
- Returns results from both, grouped by KB name

### Test 7: Error Resilience

**Setup:** Corrupt one entry file.

**Input:** concept that would match the corrupted file + other valid files.

**Expected:**
- Warns about corrupted file, continues with valid results

**Validation approach:** Manual walkthrough of Tests 1-6 after skill is written. Agent follows procedure, we verify each step.

## 7. Deliverables

1. **`~/.claude/skills/kb-route/SKILL.md`** — The routing skill containing the Resolution Procedure, edge cases, and usage instructions for consumption skills.

2. **`~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`** — Comprehensive operator's guide:
   - System overview: registry → master-index → layers → entries → bridges
   - All KB skills: kb-route, kb-harvest, kb-sync, kb-validate
   - Entry lifecycle: placeholder → harvested → curated → synced
   - Confidence scoring: harvest_metadata integration, 5-factor formula, thresholds
   - Bridge system: structure, composition, anti-patterns, combinations
   - How-to guides: add a KB, add a layer, populate entries, troubleshoot common issues
   - Architecture diagram: how all components connect
   - Quick reference: common commands for each skill

## 8. Out of Scope (Future Work)

- **Consumption skill rewrites** — separate brainstorming/spec/plan cycle after kb-route is tested
- **Python query helper** — escape hatch if KB scale eventually exceeds agent tool efficiency
- **New bridge directions** — kb-route supports any direction found in cross_layer_mappings, but creating new bridge types is PBCPB's job
- **KB population** — kb-route queries existing data; kb-harvest populates it
- **Adaptive routing / caching** — creative analysis dimensions identified in prompt; deferred until real usage patterns reveal whether they're needed
