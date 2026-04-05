# kb-route Skill Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Create a single `kb-route` skill that provides a shared Resolution Procedure for querying the multi-layer Knowledge Base, plus a comprehensive operator's manual for the entire KB system.

**Architecture:** One SKILL.md file containing a 6-step linear Resolution Procedure with skip gates. Consumption skills reference it inline ("Read and follow Resolution Procedure in kb-route/SKILL.md"). A separate KB-SYSTEM-MANUAL.md documents all KB skills and operations.

**Tech Stack:** Pure markdown SKILL.md (no Python). Uses agent tools (Read, Grep, Glob) for all KB access.

**Spec:** `docs/superpowers/specs/2026-04-04-kb-route-skill-design.md`

---

## File Structure

| File | Responsibility |
|------|---------------|
| Create: `~/.claude/skills/kb-route/SKILL.md` | The routing skill — frontmatter, invocation, Resolution Procedure (Steps 1-6), Result Summary, edge cases, usage instructions for consumption skills |
| Create: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md` | Operator's guide — system overview, all KB skills, entry lifecycle, confidence scoring, bridge system, how-to guides, quick reference |

---

### Task 1: Create kb-route SKILL.md — Frontmatter, Invocation, and Input Parameters

**Files:**
- Create: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Write the SKILL.md with frontmatter, description, invocation, and input parameters section**

````markdown
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
````

- [ ] **Step 2: Verify the file was created correctly**

Run: `head -30 ~/.claude/skills/kb-route/SKILL.md`
Expected: frontmatter with name/description, title, inline reference instructions, parameter table

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): create skill with frontmatter, invocation, and input parameters"
```

---

### Task 2: Write Step 1 — Setup

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append Step 1 (Setup) to the Resolution Procedure section**

Append after the `## Resolution Procedure` line:

```markdown

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
```

- [ ] **Step 2: Verify the appended content**

Run: `grep -c "Step 1: Setup" ~/.claude/skills/kb-route/SKILL.md`
Expected: `1`

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add Step 1 — Setup with registry resolution and format auto-detection"
```

---

### Task 3: Write Step 2 — Concept Lookup

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append Step 2 (Concept Lookup) to the SKILL.md**

Append after Step 1:

````markdown

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
   - `curated` or `synced` �� usable, proceed to Step 5
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
````

- [ ] **Step 2: Verify**

Run: `grep -c "Step 2: Concept Lookup" ~/.claude/skills/kb-route/SKILL.md`
Expected: `1`

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add Step 2 — Concept Lookup with Grep search and status handling"
```

---

### Task 4: Write Step 2b — Explore and Step 3 — Cross-Reference Follow

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append Step 2b (Explore) and Step 3 (Cross-Reference Follow)**

Append after Step 2:

````markdown

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
````

- [ ] **Step 2: Verify both sections present**

Run: `grep -c "Step 2b\|Step 3:" ~/.claude/skills/kb-route/SKILL.md`
Expected: `2`

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add Step 2b — Explore and Step 3 — Cross-Reference Follow"
```

---

### Task 5: Write Step 4 — Bridge Resolution

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append Step 4 (Bridge Resolution)**

Append after Step 3:

````markdown

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
````

- [ ] **Step 2: Verify**

Run: `grep -c "Step 4: Bridge Resolution" ~/.claude/skills/kb-route/SKILL.md`
Expected: `1`

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add Step 4 — Bridge Resolution with composition and fallbacks"
```

---

### Task 6: Write Step 5 — Confidence Filter, Step 6 — Gap Detection, and Result Summary

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append Steps 5, 6, and Result Summary**

Append after Step 4:

````markdown

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
````

- [ ] **Step 2: Verify all steps present**

Run: `grep -c "^### Step" ~/.claude/skills/kb-route/SKILL.md`
Expected: `7` (Step 1, Step 2, Step 2b, Step 3, Step 4, Step 5, Step 6)

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add Steps 5-6 — Confidence Filter, Gap Detection, and Result Summary"
```

---

### Task 7: Write Cross-Cutting Edge Cases and Usage Instructions

**Files:**
- Modify: `~/.claude/skills/kb-route/SKILL.md`

- [ ] **Step 1: Append cross-cutting edge cases and usage instructions for consumption skills**

Append after Result Summary:

````markdown

## Cross-Cutting Edge Cases

| Condition | Behavior |
|---|---|
| Both `concept` and `bridge_descriptor` provided | Steps 2 and 4 both run. Return results grouped. Consumption skill reconciles. |
| Multiple registered KBs match | Return from all, grouped by KB. Order within each by status then domain_relevance. |
| Entry version field varies | Currently one file per entry. If future append-only versioning creates multiple files, prefer highest version with best status. |
| Multiple entries cover same concept | Return all, ordered by status (curated > synced > harvested) then `domain_relevance`. Consumption skill picks. |
| Entry file is malformed JSON | Skip entry, warn: "Entry [filename] invalid JSON — run `kb-sync --verify`" |

## Usage Instructions for Consumption Skills

To use kb-route from a consumption skill, add this line to the skill's procedure:

```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: concept="<value>", bridge_descriptor="<value>", kb="<value>"
```

Provide only the parameters relevant to the current task. The Resolution Procedure will skip steps that don't apply.

**Example — concept lookup:**
```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: concept="filter resonance"
```

**Example — bridge lookup:**
```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: bridge_descriptor="warm"
```

**Example — explore a layer:**
```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: explore=true, layer="bridge"
```

**Example — both concept and bridge:**
```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: concept="filter", bridge_descriptor="warm", kb="juce-agent-prototype"
```
````

- [ ] **Step 2: Count total lines to verify completeness**

Run: `wc -l ~/.claude/skills/kb-route/SKILL.md`
Expected: approximately 180-220 lines

- [ ] **Step 3: Commit**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "feat(kb-route): add cross-cutting edge cases and consumption skill usage instructions"
```

---

### Task 8: Create KB-SYSTEM-MANUAL.md — System Overview and Architecture

**Files:**
- Create: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`

- [ ] **Step 1: Write the manual header and system overview sections**

````markdown
# KB System Manual

Comprehensive operator's guide for the Knowledge Base infrastructure. Covers all KB skills, data structures, and operational procedures.

## System Overview

The KB system stores structured domain knowledge that AI agents query during playbook execution. Knowledge is organized in **layers** (e.g., technical, sound-design, bridge) containing **entries** (JSON files) grouped by **topic**.

### Architecture

```
~/.claude/kb-registry.json          ← Single source of truth for KB discovery
  │
  ├─ KB: juce-agent-prototype       ← Prototype KB (rich data, legacy format)
  │    └─ master-index.json
  │         ├─ dsp-kb/              ← Layer
  │         │    ├─ manifest.json
  │         │    ├─ filters/        ← Topic
  │         │    │    └─ *.json     ← Entries
  │         │    └─ synthesis/
  │         ├─ sound-design-kb/
  │         └─ juce-kb/
  │
  └─ KB: vst-product-lifecycle      ← New KB (current schema, sparse data)
       └─ master-index.json
            ├─ technical/
            ├─ sound-design/
            ├─ bridge/              ← Bridge layer (descriptor → parameters)
            │    ├─ timbre/
            │    │    └─ bridge_timbre_warm.json
            │    └─ dynamics/
            └─ ui-ux/
```

### Component Relationships

```
PBCPB (Playbook Creator Playbook)
  │ generates playbooks that define KB architecture
  ▼
kb-harvest ──── populates entries from web/local sources
  │               writes entries + runs cascade (manifest, master-index, cross-refs)
  ▼
kb-sync ─────── verifies consistency, repairs cascades
  │               populates entry-level cross-references
  │               promotes entry status (curated → synced)
  ▼
kb-validate ─── validates technical claims
  │               assigns confidence scores (5-factor formula)
  ▼
kb-route ────── Resolution Procedure for querying KBs
  │               consumption skills reference this inline
  ▼
Consumption Skills (sound-design-bridge, dsp-implementation, etc.)
  │ query KB via kb-route procedure during playbook execution
  ▼
Agent Output ── code, specs, designs informed by KB knowledge
```

## KB Registry

**Location:** `~/.claude/kb-registry.json`

The registry maps KB names to filesystem paths. Every KB skill reads this first.

**Structure:**
```json
{
  "version": "1.0.0",
  "registries": [
    {
      "name": "juce-agent-prototype",
      "path": "/home/myuser/agents/juce-agent/playbookdata",
      "layers": ["dsp-kb", "sound-design-kb", "juce-kb", ...],
      "bridge_eligible_layers": ["sound-design-kb"],
      "default_backend": "ddg+webfetch"
    }
  ],
  "default_kb": "juce-agent-prototype"
}
```

**Key fields:**
- `name`: Unique identifier for the KB
- `path`: Absolute path to the KB root directory
- `layers[]`: List of layer directory names
- `bridge_eligible_layers[]`: Layers that can participate in bridge translations
- `default_backend`: Default harvest backend for this KB

## Master Index

**Location:** `<kb.path>/master-index.json`

Cross-references entries across all layers. Two formats exist:

**New format (PBCPB-generated):**
```json
{
  "kb_layers": [
    { "name": "technical", "topics": [...], "authority_score": 1.0 }
  ],
  "cross_layer_mappings": [
    { "from": "bridge", "to": "technical", "relationship": "translates" }
  ]
}
```

**Prototype format:**
```json
{
  "knowledge_bases": {
    "dsp-kb": { "path": "dsp-kb", "topics": [...], "file_count": 53 }
  }
}
```

kb-route auto-detects the format at Step 1.
````

- [ ] **Step 2: Verify the file was created**

Run: `head -20 ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`
Expected: title, system overview heading, architecture diagram

- [ ] **Step 3: Commit**

```bash
git add ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md
git commit -m "docs(kb-system): create manual — system overview, architecture, registry, master-index"
```

---

### Task 9: KB-SYSTEM-MANUAL.md — Entry Lifecycle, Confidence Scoring, and Bridge System

**Files:**
- Modify: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`

- [ ] **Step 1: Append Entry Lifecycle and Confidence Scoring sections**

Append to the manual:

````markdown

## Entry Lifecycle

Every KB entry progresses through a lifecycle:

```
placeholder → harvested → curated → synced
```

| Status | Meaning | How it gets here | Trust level |
|--------|---------|-----------------|-------------|
| `placeholder` | Empty shell — title and tags only, description starts with "TODO:" | PBCPB Phase 3 bootstrapping | None — triggers harvest or is skipped |
| `harvested` | Content populated from web/local sources | `kb-harvest` fills it | Check confidence score before using |
| `curated` | Human-reviewed and verified | Manual review + edit | High — use normally |
| `synced` | Promoted after passing validation | `kb-sync --promote` | Full trust |

**Entry file structure** (new KB schema):
```json
{
  "id": "vst_sound-design_subtractive",
  "kb": "sound-design",
  "topic": "synthesis-types",
  "status": "harvested",
  "version": "1.0.0",
  "title": "Subtractive Synthesis",
  "summary": "Rich oscillator → filter → amp envelope...",
  "description": "Full description text...",
  "source": { "type": "expert-tutorial", "reference": "...", "url": "..." },
  "concepts": [
    { "name": "Oscillator", "description": "Sound source generating raw waveforms" }
  ],
  "tags": ["synthesis", "subtractive", "analog"],
  "related_topics": ["parameter-mapping", "preset-methodology"],
  "cross_references": [
    { "kb": "technical", "entry_id": "vst_technical_filter-design", "relationship": "implements" }
  ],
  "domain_relevance": 9,
  "difficulty": "beginner"
}
```

**Harvested entries also have:**
```json
{
  "harvest_metadata": {
    "overall_confidence": 0.75,
    "field_provenance": { ... },
    "harvested_at": "2026-04-04T...",
    "backend": "ddg+webfetch"
  }
}
```

## Confidence Scoring

Entries carry confidence scores from two sources:

1. **harvest_metadata.overall_confidence** — assigned by kb-harvest during population, refined by kb-validate's 5-factor formula
2. **Top-level `confidence`** — present on bridge entries (manually assigned)

### Thresholds

| Confidence | Action |
|---|---|
| >= 0.60 | Use normally |
| 0.40 - 0.59 | Use with warning — verify before relying on this |
| < 0.40 | Exclude — too unreliable |
| Absent | Use with note — "No confidence score available" |

### kb-validate 5-Factor Formula

kb-validate scores claims using:
1. **Source authority** — is the source reputable?
2. **Recency** — how old is the information?
3. **Corroboration** — do multiple sources agree?
4. **Specificity** — is it concrete or vague?
5. **Internal consistency** — does it contradict other KB entries?

These factors blend into `harvest_metadata.overall_confidence`.
````

- [ ] **Step 2: Append Bridge System section**

Append to the manual:

````markdown

## Bridge System

Bridges translate between domains — mapping subjective descriptors (e.g., "warm", "punchy") to technical parameters (e.g., `filter_cutoff: [0.2, 0.4]`).

### Bridge Entry Structure

Bridge entries live in `<kb.path>/bridge/<category>/bridge_<category>_<descriptor>.json`:

```json
{
  "id": "bridge_timbre_warm",
  "category": "timbre",
  "descriptor": "warm",
  "parameters": [
    {
      "parameter": "filter_cutoff",
      "value_range": [0.2, 0.4],
      "typical_default": 0.3,
      "unit": "normalized",
      "notes": "Low-pass filter reduces high frequencies for warmth"
    }
  ],
  "confidence": 0.85,
  "why": "Human-readable rationale for this mapping",
  "anti_patterns": [
    { "mistake": "Heavy saturation", "reason": "Too much saturation creates harshness" }
  ],
  "combinations": [
    {
      "compatible_with": "bridge_character_analog",
      "notes": "Analog character enhances warmth through drift",
      "confidence_modifier": 0.1
    }
  ]
}
```

### Bridge Categories

Categories group related descriptors. Current categories in the new KB:

| Category | Example descriptors |
|----------|-------------------|
| timbre | warm, bright, dark |
| dynamics | punchy, soft, aggressive |
| space | wide, intimate, cavernous |
| movement | evolving, static, rhythmic |
| character | analog, digital, lo-fi |

### Bridge Composition

When a consumption skill needs multiple descriptors (e.g., "warm analog"), kb-route composes them:

1. Read each bridge entry separately
2. Check `combinations[]` for `compatible_with` references
3. **If compatible:** merge parameter lists, apply `confidence_modifier`, intersection of `anti_patterns`
4. **If not listed as compatible:** compose with lowered confidence (multiply each by 0.8), union of `anti_patterns`

### Bridge Manifest

The bridge layer uses `categories` (not `topics`) in its manifest:
```json
{
  "categories": [
    { "name": "timbre", "entry_count": 1, "entries": [...] }
  ]
}
```

### Bridge-Eligible Layers

The registry's `bridge_eligible_layers[]` identifies which layers can participate in bridge translations. The master-index's `cross_layer_mappings` with `"relationship": "translates"` connects bridge layers to their source/target layers.
````

- [ ] **Step 3: Verify all data model sections present**

Run: `grep -c "## Entry Lifecycle\|## Confidence Scoring\|## Bridge System" ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`
Expected: `3`

- [ ] **Step 4: Commit**

```bash
git add ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md
git commit -m "docs(kb-system): add entry lifecycle, confidence scoring, and bridge system"
```

---

### Task 10: KB-SYSTEM-MANUAL.md — Skills Reference, How-To Guides, and Troubleshooting

**Files:**
- Modify: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`

- [ ] **Step 1: Append All KB Skills section with quick reference commands**

Append to the manual:

````markdown

## KB Skills Reference

### kb-harvest — Populate Entries

Populates KB entries from web sources, local files, or user-provided URLs. Every harvest operation maintains all KB infrastructure (manifests, master-index, cross-references).

**Common commands:**
```
kb-harvest --kb <name>                     # Interactive harvest session
kb-harvest --kb <name> --auto              # Auto-fill all placeholders
kb-harvest --kb <name> --auto --refresh    # Re-harvest below confidence threshold
kb-harvest --kb <name> --auto --entry <id> # Fill specific entry
kb-harvest --kb <name> --urls <url1> ...   # Harvest specific URLs
kb-harvest --kb <name> --layer <layer>     # Target specific layer
kb-harvest --kb <name> --topic <topic>     # Target specific topic
kb-harvest --kb <name> --batch <N>         # Set batch size (default 5)
kb-harvest --kb <name> --import <path>     # Import external KB
kb-harvest --kb <name> --rebuild-manifest  # Reconstruct manifests from files
kb-harvest --status                        # Show harvest status across all KBs
kb-harvest --list                          # List registered KBs
```

**Cascade:** After each batch, kb-harvest automatically updates: manifest → master-index → cross-references → bridge detection → search terms.

### kb-sync — Verify and Repair

Verifies KB consistency, repairs incomplete cascades, populates entry-level cross-references, promotes entry status.

**Common commands:**
```
kb-sync --verify                              # Full consistency check (all KBs)
kb-sync --verify --kb <name>                  # Verify specific KB
kb-sync --repair                              # Fix inconsistencies
kb-sync --repair --kb <name>                  # Repair specific KB
kb-sync --promote --kb <name> --entry <id>    # Promote curated → synced
kb-sync --status                              # Show sync status
```

**Scheduled:** Runs every 30 minutes via cron, checking for cascade journals from recent harvests.

### kb-validate — Validate Claims

Validates technical knowledge claims before implementation. Assigns confidence scores using the 5-factor formula.

**Common commands:**
```
kb-validate --claim "JUCE SmoothedValue uses getNextValue() per sample"
kb-validate --auto --source ${last_skill_output}
kb-validate --gate --check-session
kb-validate --report
```

**Triggers:** Auto-triggers after kb-harvest and firecrawl operations. Also runs as pre-implementation gate.

### kb-route — Query Resolution

Shared Resolution Procedure for querying KBs. Not invoked directly — consumption skills reference it inline.

**How consumption skills use it:**
```
Read and follow the Resolution Procedure in ~/.claude/skills/kb-route/SKILL.md
with parameters: concept="filter resonance"
```

**Steps:** Setup → Concept Lookup → Explore → Cross-Ref Follow → Bridge Resolution → Confidence Filter → Gap Detection → Result Summary

See `~/.claude/skills/kb-route/SKILL.md` for the full procedure.
````

- [ ] **Step 2: Append How-To Guides and Troubleshooting sections**

Append to the manual:

````markdown

## How-To Guides

### Add a New KB

1. Create the KB directory structure with layers:
   ```
   mkdir -p /path/to/new-kb/{layer1,layer2}/
   ```
2. Create a `master-index.json` at the KB root (use new format with `kb_layers[]`)
3. Register in `~/.claude/kb-registry.json` — add entry to `registries[]`:
   ```json
   {
     "name": "my-new-kb",
     "path": "/path/to/new-kb",
     "layers": ["layer1", "layer2"],
     "bridge_eligible_layers": [],
     "default_backend": "ddg+webfetch"
   }
   ```
4. Run `kb-sync --verify --kb my-new-kb` to validate structure

### Add a Layer to an Existing KB

1. Create the layer directory: `mkdir -p <kb.path>/new-layer/`
2. Create a manifest.json in the layer directory:
   ```json
   { "kb_name": "new-layer", "version": "1.0.0", "topics": [], "status_counts": {} }
   ```
3. Update `master-index.json` — add layer to `kb_layers[]` (or `knowledge_bases{}` for prototype format)
4. Update `~/.claude/kb-registry.json` — add layer name to the KB's `layers[]`
5. Run `kb-sync --verify --kb <name>` to validate

### Populate Entries

**Fill all placeholders in a KB:**
```
kb-harvest --kb <name> --auto
```

**Fill a specific entry:**
```
kb-harvest --kb <name> --auto --entry <entry-id> --batch 1
```

**Fill a specific topic:**
```
kb-harvest --kb <name> --topic <topic-name>
```

**Import from external source:**
```
kb-harvest --kb <name> --import /path/to/data --dry-run   # Preview first
kb-harvest --kb <name> --import /path/to/data              # Execute
```

### Promote Entry Status

Entries progress: `placeholder` → `harvested` → `curated` → `synced`.

- `placeholder → harvested`: Automatic via kb-harvest
- `harvested → curated`: Manual review — edit the entry file, set `"status": "curated"`
- `curated → synced`: Via kb-sync: `kb-sync --promote --kb <name> --entry <id>`

### Add a Bridge Direction

1. Create bridge entries in `<kb.path>/bridge/<category>/bridge_<category>_<descriptor>.json`
2. Add `cross_layer_mappings` to master-index.json:
   ```json
   { "from": "bridge", "to": "<target-layer>", "relationship": "translates" }
   ```
3. Add the source layer to `bridge_eligible_layers[]` in the registry
4. Run `kb-sync --verify` to validate

## Troubleshooting

| Problem | Cause | Fix |
|---------|-------|-----|
| "No KB registry found" | `~/.claude/kb-registry.json` missing | Run `kb-harvest --kb <name>` to create, or create manually |
| "KB [name] registered but master-index.json not found" | Master-index deleted or KB path wrong | Check `path` in registry, run `kb-sync --repair` |
| "KB [name] master-index format not recognized" | Corrupted or incompatible master-index | Regenerate master-index from layer directories |
| Manifest shows 0 entries but files exist on disk | Manifest out of date | Run `kb-sync --repair --kb <name>` or `kb-harvest --kb <name> --rebuild-manifest` |
| "Entry [filename] invalid JSON" | Corrupted entry file | Fix JSON syntax or delete and re-harvest |
| Confidence scores all 0 or missing | Entries not validated after harvest | Run `kb-validate --auto` on harvested entries |
| Cross-references point to missing entries | Entry deleted or renamed | Run `kb-sync --verify` then `kb-sync --repair` |
| Bridge lookup returns nothing | Bridge entries not created for this descriptor | Create bridge entry or run `kb-harvest` targeting bridge layer |
| "Harvest failed for [entry_id]" | Backend unavailable or rate-limited | Retry with different backend: `kb-harvest --kb <name> --backend websearch+webfetch --entry <id>` |
````

- [ ] **Step 3: Verify all operational sections present**

Run: `grep -c "## KB Skills Reference\|## How-To Guides\|## Troubleshooting" ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`
Expected: `3`

- [ ] **Step 4: Commit**

```bash
git add ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md
git commit -m "docs(kb-system): add skills reference, how-to guides, and troubleshooting"
```

---

### Task 11: Validate Both Deliverables Against Spec

**Files:**
- Read: `~/.claude/skills/kb-route/SKILL.md`
- Read: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`
- Read: `docs/superpowers/specs/2026-04-04-kb-route-skill-design.md`

- [ ] **Step 1: Validate kb-route SKILL.md — full read and spec checklist**

Read `~/.claude/skills/kb-route/SKILL.md` in full. Verify against spec Section 3 (Resolution Procedure) and Section 4 (Edge Cases):

- [ ] Step 1 handles both master-index formats
- [ ] Step 1 has all 4 failure conditions from spec Section 4.1
- [ ] Step 2 uses per-layer Grep with infrastructure file exclusion
- [ ] Step 2 has 5-entry cap with prioritization criteria
- [ ] Step 2 has placeholder harvest invocation
- [ ] Step 2b handles `categories` for bridge layers
- [ ] Step 2b has Glob fallback
- [ ] Step 3 uses Glob then Grep for cross-ref resolution
- [ ] Step 3 notes related_topics without auto-following
- [ ] Step 4 skip condition is `bridge_descriptor` only
- [ ] Step 4 has composition rules (compatible vs not-compatible)
- [ ] Step 5 has all 4 threshold rows
- [ ] Step 6 has all 4 differentiated gap causes
- [ ] Result Summary section present
- [ ] Cross-cutting edge cases table present
- [ ] Usage instructions with examples present

- [ ] **Step 2: Validate KB-SYSTEM-MANUAL.md — spec Section 7 checklist**

Read `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md` in full. Verify against spec Section 7 deliverable 2:

- [ ] System overview: registry → master-index → layers → entries → bridges
- [ ] All KB skills: kb-route, kb-harvest, kb-sync, kb-validate
- [ ] Entry lifecycle: placeholder → harvested → curated → synced
- [ ] Confidence scoring: harvest_metadata, 5-factor formula, thresholds
- [ ] Bridge system: structure, composition, anti-patterns, combinations
- [ ] How-to guides: add KB, add layer, populate entries, troubleshoot
- [ ] Architecture diagram
- [ ] Quick reference commands for each skill

- [ ] **Step 3: Fix any gaps found in either file**

If any spec requirement is missing, add it to the appropriate file.

- [ ] **Step 4: Commit fixes if any**

```bash
git add ~/.claude/skills/kb-route/SKILL.md ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md
git commit -m "fix(kb-route): validation pass — fill gaps found in spec review"
```

Skip if no fixes needed.

---

### Task 12: Run Manual Walkthrough — Tests 1, 2, and 4 (Concept, Bridge, Gap Detection)

**Files:**
- Modify: `~/.claude/kb-registry.json`
- Read: `~/.claude/skills/kb-route/SKILL.md` (follow procedure)
- Read: `/home/myuser/playbooks/vst-product-lifecycle-playbook/kb/master-index.json`

- [ ] **Step 1: Register the new KB (prerequisite for walkthroughs)**

The new KB is not yet in the registry. Add it so walkthrough tests can find it. Read `~/.claude/kb-registry.json`, then add this entry to the `registries[]` array:

```json
{
  "name": "vst-product-lifecycle",
  "path": "/home/myuser/playbooks/vst-product-lifecycle-playbook/kb",
  "schema_path": null,
  "bridge_schema_path": null,
  "master_index_path": "master-index.json",
  "generated_by": "pbcpb",
  "layers": ["technical", "sound-design", "ui-ux", "commercial", "reference", "bridge"],
  "bridge_eligible_layers": ["sound-design"],
  "default_backend": "ddg+webfetch",
  "registered_at": "2026-04-04T00:00:00Z"
}
```

- [ ] **Step 2: Walkthrough Test 1 — concept="subtractive synthesis"**

Follow the kb-route Resolution Procedure manually:
1. **Step 1:** Read registry → find vst-product-lifecycle KB → read master-index → detect `kb_layers[]` format → extract layers: technical, sound-design, ui-ux, commercial, reference, bridge
2. **Step 2:** Grep for "subtractive synthesis" (case-insensitive) in `*.json` files under each layer directory, excluding manifest.json and index.json
   - Expected: finds `sound-design/synthesis-types/vst_sound-design_subtractive.json`
   - Read entry → status=harvested, no harvest_metadata → unscored
3. **Step 3:** Entry has `cross_references: [{ "kb": "technical", "entry_id": "vst_technical_filter-design" }]`
   - Glob for `technical/**/vst_technical_filter-design.json` → finds it in `technical/dsp-algorithms/`
   - Read entry → status=placeholder → note gap
4. **Step 5:** Primary entry unscored → "No confidence score available". Cross-ref entry is placeholder → noted.
5. **Result Summary:** Primary: vst_sound-design_subtractive (harvested, unscored, sound-design layer). Secondary: vst_technical_filter-design (placeholder, cross-referenced via "implements")

Verify: does the procedure produce the expected output from spec Test 1?

- [ ] **Step 3: Walkthrough Test 2 — bridge_descriptor="warm"**

1. **Step 1:** Same registry/master-index read
2. **Step 4:** `cross_layer_mappings` shows bridge layer with `translates` relationship. Glob for `bridge/*/bridge_*_warm.json` → finds `bridge/timbre/bridge_timbre_warm.json`. Read entry → 3 parameters, confidence 0.85, 2 anti_patterns, 1 combination.
3. **Step 5:** confidence 0.85 >= 0.60 → use normally
4. **Result Summary:** Bridge: "warm" — filter_cutoff [0.2, 0.4], filter_resonance [0.1, 0.25], saturation_amount [0.1, 0.3]. Confidence 0.85. Anti-patterns: heavy saturation, high filter resonance.

Verify: does the procedure produce the expected output from spec Test 2?

- [ ] **Step 4: Walkthrough Test 4 — concept="granular delay" (Gap Detection)**

1. **Step 1:** Read registry, read master-indexes for both KBs
2. **Step 2:** Grep for "granular delay" across all layers in both KBs
   - Expected: no matches (or only tangential)
3. **Step 6:** Gap detection fires — "No entries found for 'granular delay' in [KB names]"
   - Suggestion: `kb-harvest --kb <name> --topic <suggested_topic>`

Verify: gap detection differentiates correctly (no matches → row 2 of gap table).

- [ ] **Step 5: Document results and fix issues**

Note any issues found during walkthroughs. If the procedure doesn't match expected output, fix the SKILL.md.

- [ ] **Step 6: Commit**

```bash
git add ~/.claude/kb-registry.json ~/.claude/skills/kb-route/SKILL.md
git commit -m "test(kb-route): register new KB, walkthrough Tests 1, 2, and 4"
```

---

### Task 13: Run Manual Walkthrough — Tests 3, 5, and 6 (Prototype KB, Explore, Multiple KBs)

**Files:**
- Read: `~/.claude/skills/kb-route/SKILL.md` (follow procedure)

**Prerequisite:** Task 12 must complete first (new KB registered in registry).

- [ ] **Step 1: Walkthrough Test 3 — concept="reverb" against prototype KB**

Follow the procedure:
1. **Step 1:** Read registry → prototype KB at `/home/myuser/agents/juce-agent/playbookdata` → read master-index → detect `knowledge_bases{}` format → extract layers: juce-kb, dsp-kb, sound-design-kb, etc.
2. **Step 2:** Grep for "reverb" in each layer's `*.json` files. Expected: multiple matches in `dsp-kb/reverb/`. Read up to 5, prioritize by status + domain_relevance.
3. **Step 4:** No bridge layer in prototype KB → skip
4. **Step 5:** Check each entry for harvest_metadata.overall_confidence or top-level confidence
5. **Result Summary:** List found entries with status/confidence notes

Verify: procedure handles prototype `knowledge_bases{}` format correctly.

- [ ] **Step 2: Walkthrough Test 5 — explore=true, layer="bridge"**

Follow the procedure against new KB:
1. **Step 1:** Same setup
2. **Step 2b:** Read manifest at `bridge/manifest.json` → has `categories[]` (not `topics[]`): timbre, dynamics, space, movement, character. Entry counts show 0 in manifest (out of date).
3. Fall back to Glob: `bridge/**/*.json` → finds 5 bridge entries (one per category)
4. Return overview with category names and counts from Glob

Verify: procedure handles bridge manifest `categories` field and Glob fallback.

- [ ] **Step 3: Walkthrough Test 6 — concept="filter" across multiple KBs**

Both KBs are now registered (prototype + new). Follow the procedure:
1. **Step 1:** Read registry → both KBs. Read both master-indexes (prototype = `knowledge_bases{}` format, new = `kb_layers[]` format).
2. **Step 2:** Grep for "filter" across all layers in both KBs.
   - Prototype KB: expected matches in `dsp-kb/filters/` and possibly `juce-kb/`
   - New KB: expected match on `vst_technical_filter-design.json` (placeholder) and `vst_sound-design_subtractive.json` (tags include "filter")
3. **Step 5:** Apply confidence filter to all results
4. **Result Summary:** Results grouped by KB name. Within each KB, ordered by status then domain_relevance.

Verify: results come from both KBs, grouped correctly.

- [ ] **Step 4: Document results and fix issues**

- [ ] **Step 5: Commit fixes if any**

```bash
git add ~/.claude/skills/kb-route/SKILL.md
git commit -m "fix(kb-route): fixes from manual walkthrough Tests 3, 5, and 6"
```

Skip if no fixes needed.

---

### Task 14: Final Commit — Tag as Complete

**Files:**
- Read: `~/.claude/skills/kb-route/SKILL.md`
- Read: `~/Documents/pbcpb/KB-SYSTEM-MANUAL.md`

- [ ] **Step 1: Final read of both deliverables**

Read both files in full. Confirm:
- kb-route SKILL.md is a complete, self-contained Resolution Procedure
- KB-SYSTEM-MANUAL.md covers all sections listed in spec Section 7

- [ ] **Step 2: Final commit if any outstanding changes**

```bash
git status
```

If there are uncommitted changes:
```bash
git add ~/.claude/skills/kb-route/SKILL.md ~/Documents/pbcpb/KB-SYSTEM-MANUAL.md
git commit -m "feat(kb-route): complete kb-route skill and KB system manual"
```

If everything is already committed, this step is a no-op.

Now let me verify completeness with the self-review checklist.

**Self-Review:**

**1. Spec coverage:**
- Spec Section 1 (Overview) → Task 1 frontmatter
- Spec Section 2 (Architecture) → Task 1 parameters, Task 7 usage instructions
- Spec Section 3 (Resolution Procedure) → Tasks 2-6 cover all steps
- Spec Section 4 (Edge Cases) → embedded in each step's Failures subsections + Task 7 cross-cutting
- Spec Section 5 (Maintainability) → inherent in the design (registry-driven, no hardcoded paths)
- Spec Section 6 (Testing) → Tasks 12-13 walkthrough Tests 1-6 (Test 7 skipped — requires file corruption)
- Spec Section 7 Deliverable 1 (SKILL.md) → Tasks 1-7
- Spec Section 7 Deliverable 2 (KB-SYSTEM-MANUAL.md) → Tasks 8-10
- Task 11 is the spec validation pass for both deliverables

**2. Placeholder scan:** No TBDs, TODOs, or "add appropriate..." in any task. All code blocks contain complete content.

**3. Type consistency:** Field names match across tasks: `kb_layers[]` / `knowledge_bases{}`, `cross_layer_mappings`, `cross_references[]`, `harvest_metadata.overall_confidence`, `categories` vs `topics` for bridge manifests — all consistent with actual JSON structures verified by reading the files.

Plan complete and saved to `docs/superpowers/plans/2026-04-04-kb-route-skill.md`. Two execution options:

**1. Subagent-Driven (recommended)** - I dispatch a fresh subagent per task, review between tasks, fast iteration

**2. Inline Execution** - Execute tasks in this session using executing-plans, batch execution with checkpoints

Which approach?