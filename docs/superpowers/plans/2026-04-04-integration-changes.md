# Integration Changes Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Update kb-sync, kb-validate, and all five consumption skills to work with the new kb-harvest skill, KB registry, and harvest_metadata infrastructure established by Plan B.

**Architecture:** Seven skill SKILL.md files are modified. kb-sync gets a rewrite (verification/repair role replaces index updater). kb-validate gets harvest_metadata integration in its confidence scorer. Five consumption skills (sound-design-bridge, dsp-implementation, ui-bridge, plugin-spec, daw-testing) replace hardcoded KB paths with registry lookups, replace kb-autofill with kb-harvest, and add confidence awareness. Two retired skills (kb-autofill, harvest-data) are verified removed.

**Tech Stack:** Claude Code skills (SKILL.md markdown files), JSON (kb-registry.json)

**Spec:** `docs/superpowers/specs/2026-04-04-kb-harvest-pbcpb-integration-design.md` — Section 9 (Integration Changes)

**Dependencies:**
- Plan B: KB Infrastructure + kb-harvest Skill (must be complete — provides registry, kb-harvest skill, harvest_metadata format)
- Plan A: PBCPB Meta-System Changes (independent — Plan C doesn't touch PBCPB)

**Working Directory:** `~/.claude/skills/`

---

## Prerequisite Check

Before starting, verify Plan B artifacts exist:

```bash
# Registry exists
test -f ~/.claude/kb-registry.json && echo "OK: registry" || echo "MISSING: registry"

# kb-harvest skill exists
test -f ~/.claude/skills/kb-harvest/SKILL.md && echo "OK: kb-harvest" || echo "MISSING: kb-harvest"

# Config exists
test -f ~/.claude/kb-harvest-config.json && echo "OK: config" || echo "MISSING: config"
```

If any are missing, complete Plan B first.

## Key Interfaces (from Plan B)

These are the interfaces Plan C skills will consume:

**KB Registry** (`~/.claude/kb-registry.json`):
```json
{
  "version": "1.0.0",
  "registries": [
    {
      "name": "kb-name",
      "path": "/absolute/path/to/kb",
      "schema_path": "../architecture/entry-schema.json",
      "master_index_path": "master-index.json",
      "layers": ["layer1", "layer2"],
      "bridge_eligible_layers": ["layer1"],
      "default_backend": "ddg+webfetch"
    }
  ],
  "default_kb": "kb-name"
}
```

**kb-harvest invocation** (replaces kb-autofill):
```
kb-harvest --kb <name> --auto --entry <entry-id> --batch 1
```

**harvest_metadata** (on KB entries):
```json
{
  "harvest_metadata": {
    "overall_confidence": 0.72,
    "field_provenance": {
      "description": { "method": "ai-synthesized", "confidence": 0.80 }
    },
    "backend_used": "ddg+webfetch",
    "review_flag": false
  }
}
```

**cascade-journal.json** (kb-sync reads this):
```json
{
  "session_id": "2026-04-04-001",
  "batch_id": "batch_1",
  "started_at": "2026-04-04T10:00:00Z",
  "entries_in_batch": ["entry-id-1", "entry-id-2"],
  "steps": {
    "manifest": "done",
    "master_index": "done",
    "cross_refs": "pending",
    "bridges": "pending",
    "search_terms": "pending"
  }
}
```

## File Structure

### Files to Modify

```
~/.claude/skills/
├── kb-sync/
│   └── SKILL.md                          # Full rewrite (§9.1)
├── kb-validate/
│   ├── SKILL.md                          # Update references (§9.2)
│   ├── confidence-scorer.md              # Add harvest_metadata factor (§9.2)
│   ├── auto-investigate.md               # Replace firecrawl-search refs (§9.2)
│   └── INTEGRATION.md                    # Update hook matchers (§9.2)
├── juce-sound-design-bridge/
│   └── SKILL.md                          # Registry + kb-harvest + confidence (§9.3)
├── juce-dsp-implementation/
│   └── SKILL.md                          # Registry + kb-harvest + confidence (§9.3)
├── juce-ui-bridge/
│   └── SKILL.md                          # Registry awareness (§9.3)
├── juce-plugin-spec/
│   └── SKILL.md                          # Registry awareness (§9.3)
└── juce-daw-testing/
    └── SKILL.md                          # Registry + confidence awareness (§9.3)
```

### Files to Verify Removed

```
~/.claude/skills/
├── kb-autofill.bak/                      # Should already be renamed by Plan B
└── harvest-data.bak/                     # Should already be renamed by Plan B
```

No new files are created by this plan.

---

## Task 1: Rewrite kb-sync SKILL.md (§9.1)

**Files:**
- Modify: `~/.claude/skills/kb-sync/SKILL.md`

kb-sync changes role from "primary index updater" to "verification, repair, and status promotion." The harvest cascade (from kb-harvest) now handles index updates during harvesting. kb-sync handles post-harvest consistency verification, repair of incomplete cascades, entry-level cross_reference population, and status promotion.

- [ ] **Step 1: Read current kb-sync SKILL.md**

```bash
cat ~/.claude/skills/kb-sync/SKILL.md
```

Confirm current content matches what we analyzed (163 lines, hardcoded playbookdata paths, sync_engine.py references).

- [ ] **Step 2: Write the new kb-sync SKILL.md**

Replace the entire file with:

```markdown
---
name: kb-sync
description: Verify KB consistency, repair incomplete cascades, populate entry-level cross-references, and promote entry status. Runs on cascade-journal triggers or manual invocation.
---

# KB Sync

Verification, repair, and status promotion for Knowledge Bases. After kb-harvest writes entries and runs its cascade, kb-sync ensures everything is consistent and complete.

**Role shift:** kb-harvest's cascade handles index updates during harvesting. kb-sync handles post-harvest verification, repair, and the operations too expensive to run during cascade (entry-level cross-references).

## Invocation

``` 
/kb-sync --verify                         # Full consistency check (all registered KBs)
/kb-sync --verify --kb <name>             # Verify specific KB only
/kb-sync --repair                         # Fix inconsistencies found by --verify
/kb-sync --repair --kb <name>             # Repair specific KB only
/kb-sync --promote --kb <name> --entry <id>  # Promote entry: curated → synced
/kb-sync --status                         # Show sync status across all KBs
```

## Prerequisites

- `~/.claude/kb-registry.json` must exist (created by kb-harvest or PBCPB Phase 3)
- At least one KB registered

## Registry Resolution

All commands start by reading `~/.claude/kb-registry.json`:

1. Parse registry, get list of registered KBs
2. If `--kb <name>` specified, filter to that KB
3. If `--kb <name>` not found, list registered KBs and ask user
4. Resolve absolute paths: `registry.path + "/" + registry.master_index_path`

## Verify Command (`--verify`)

Full consistency check across all registered KBs (or specific KB with `--kb`).

**Checks performed:**

1. **Manifest accuracy:** For each layer directory, count actual .json files (excluding manifest.json). Compare against manifest entry count. Report mismatches.

2. **Master-index consistency:** Verify `knowledge_bases` entry counts match actual per-KB file counts. Verify all listed topics have corresponding directories.

3. **Cross-reference validity:** For each entry in `master-index.cross_references`, verify the referenced KBs exist in the registry and contain entries matching the referenced concepts.

4. **Bridge entry validity:** For each bridge entry, verify the referenced source entries exist and the parameters reference valid KB entries.

5. **Orphan detection:** Find entries on disk not listed in any manifest. Find manifest entries pointing to non-existent files.

6. **Cascade journal check:** If `cascade-journal.json` exists with incomplete steps, report which steps need replay.

7. **Entry-level cross-reference completeness:** For each concept appearing in master-index cross_references with 2+ KBs, check whether the corresponding entries in those KBs have `cross_references[]` populated with bidirectional links.

**Output:**

```
Verification Report for [KB name]:
  Manifest accuracy:     OK (3 layers, 45 entries match)
  Master-index:          OK (file counts match)
  Cross-references:      WARNING: 3 orphaned references
  Bridge entries:        OK (12 entries valid)
  Orphan detection:      OK (no orphans)
  Cascade journals:      WARNING: 1 incomplete journal
  Entry cross-refs:      WARNING: 8 concepts missing bidirectional links
  
  Overall: 2 warnings, 1 incomplete cascade
  Run --repair to fix.
```

## Repair Command (`--repair`)

Fixes inconsistencies found by `--verify`. Always runs `--verify` first to identify issues.

**Repair actions:**

1. **Rebuild manifests:** For each layer with mismatched counts, scan directory and reconstruct manifest.json from actual files. For each entry file, read: status, check for original_markdown (calculate length), count code_blocks, read harvest_metadata.overall_confidence if present.

2. **Remove orphaned cross-references:** Delete cross_reference entries pointing to non-existent KBs or entries.

3. **Replay incomplete cascade journals:** For each cascade-journal.json with steps not marked "done":
   - Read the journal to find which steps are pending
   - For each pending step, execute it:
     - `manifest`: rebuild manifest for affected layers
     - `master_index`: recount files, update topics
     - `cross_refs`: scan batch entries for tags/concepts, update master-index cross_references
     - `bridges`: re-run bridge detection for entries from bridge-eligible layers
     - `search_terms`: mark used terms, discover new terms from batch entries
   - Mark steps as "done" in journal after completion
   - Delete journal when all steps complete

4. **Populate entry-level cross-references:** This is the most expensive operation — it reads entries across KBs.
   - For each concept in master-index `cross_references` that spans 2+ KBs:
     - For each KB containing that concept:
       - Find the specific entry (scan layer directory, match by concept name in tags or concepts[].name)
       - Add cross_references[] entries pointing to entries in other KBs containing the same concept
       - Write bidirectional links (A→B and B→A)
   - This is the ONLY place entry-level cross_references are written. The cascade only updates master-index-level cross_references.

5. **Fix orphaned entries:** Add orphaned on-disk entries to their layer's manifest. Remove manifest entries pointing to non-existent files.

**Output:**

```
Repair Report for [KB name]:
  Manifests rebuilt: 1 layer (sound-design-kb)
  Orphaned refs removed: 3
  Cascade journals replayed: 1 (5 steps completed)
  Entry cross-refs populated: 8 concepts, 24 bidirectional links added
  Orphaned entries fixed: 0
```

## Status Promotion (`--promote`)

Promotes a curated entry to synced status:

1. Read entry file
2. Verify status is "curated"
3. Set status to "synced"
4. Update manifest (status field)
5. Update master-index if needed (status counts)

```
/kb-sync --promote --kb juce-agent-prototype --entry dsp-kb_reverb_algorithmic-reverb
```

## Scheduled Task (Cron)

The cron checks for cascade journals indicating recent harvest activity. Runs every 30 minutes instead of the old 5-minute interval:

```json
{
  "cron": "*/30 * * * *",
  "prompt": "Check for cascade-journal.json in all registered KBs (read ~/.claude/kb-registry.json for KB paths). If any journals exist with incomplete steps, run kb-sync --repair for those KBs. If no journals found, skip.",
  "recurring": true
}
```

**Old cron (remove):** The previous `*/5 * * * *` cron that checked for `status='harvested'` entries is no longer needed. The cascade handles index updates during harvesting.

## Integration with Other Skills

**Called after:**
- `kb-harvest` — Cascade journals trigger repair on next cron cycle
- Manual invocation for full verification

**Consumed by:**
- Consumption skills — Read the cross-references that kb-sync populates
- `kb-validate` — Reads consistent manifests and indexes that kb-sync maintains

## Error Handling

### Registry Missing

```
Error: ~/.claude/kb-registry.json not found.
Run kb-harvest first to create the registry, or create it manually.
```

### Lock Conflict During Repair

If a KB has an active `harvest.lock` (from a running kb-harvest session):
- Skip that KB
- Report: "Skipped [KB name] — harvest in progress"
- The cascade journal will be replayed on the next cron cycle

### Entry File Unreadable

If an entry .json file can't be parsed:
- Log the file path and error
- Skip it during manifest rebuild
- Report in verification output
```

- [ ] **Step 3: Verify the new file**

Read back `~/.claude/skills/kb-sync/SKILL.md` and confirm:
- Frontmatter has name and description
- `--verify`, `--repair`, `--promote` commands documented
- Cron updated to `*/30` with cascade journal check
- Entry-level cross-reference population documented
- Cascade journal replay documented
- Registry resolution documented
- No references to old `sync_engine.py`, `playbookdata`, or `kb-autofill`

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/kb-sync/SKILL.md
git commit -m "feat(kb-sync): rewrite as verification/repair skill (§9.1)

Replace index-updater role with verification, repair, and status
promotion. Harvest cascade now handles index updates. kb-sync handles
post-harvest consistency checks, incomplete cascade replay, entry-level
cross-reference population, and curated→synced promotion.

Spec: Section 9.1

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 2: Update kb-validate confidence-scorer.md (§9.2)

**Files:**
- Modify: `~/.claude/skills/kb-validate/confidence-scorer.md`

Add a sixth scoring factor: `harvest_confidence`. When a KB entry has `harvest_metadata.overall_confidence`, factor it into the validation score. Entries with `direct-extracted` code_blocks get a boost; single-source entries get a penalty.

- [ ] **Step 1: Read current confidence-scorer.md**

```bash
cat ~/.claude/skills/kb-validate/confidence-scorer.md
```

Confirm the current 5-factor formula (source_completeness 0.30, cross_reference_match 0.25, recency 0.15, contradiction_check 0.20, implementation_history 0.10).

- [ ] **Step 2: Add harvest_confidence factor**

After the `## Multi-Factor Score Formula` section (line 7), replace the formula block with:

````markdown
## Multi-Factor Score Formula

```
# Base formula (for claims without harvest_metadata)
confidence_score = (
    source_completeness * 0.30 +
    cross_reference_match * 0.25 +
    recency * 0.15 +
    contradiction_check * 0.20 +
    implementation_history * 0.10
)

# When harvest_metadata is present on the source KB entry,
# blend the harvest confidence into the score:
if entry.harvest_metadata:
    harvest_factor = calculate_harvest_factor(entry.harvest_metadata)
    confidence_score = confidence_score * 0.70 + harvest_factor * 0.30
```
````

- [ ] **Step 3: Add harvest factor definition**

After the `### Implementation History (10%)` section (after line 87), add:

````markdown
### Harvest Confidence (blended at 30% when present)

When the source KB entry includes `harvest_metadata`, this factor blends into the overall score. It does NOT replace the base 5-factor formula — it adjusts the result.

**Calculation:**

```
harvest_factor = overall_confidence * method_modifier * source_count_modifier

method_modifier:
  - If any code_blocks have field_provenance.method == "direct-extracted": 1.1 (cap at 1.0 after multiply)
  - If all field_provenance methods are "ai-inferred": 0.8
  - Otherwise: 1.0

source_count_modifier:
  - If source_urls has 1 entry: 0.9 (single source penalty)
  - If source_urls has 2+ entries: 1.0
  - If source_urls is empty or absent: 1.0 (not applicable, e.g., research-doc entries)
```

| Scenario | harvest_factor | Effect |
|----------|---------------|--------|
| High confidence, direct code extraction, 3 sources | 0.72 × 1.0 × 1.0 = 0.72 | Strong positive signal |
| Medium confidence, all ai-inferred, 1 source | 0.55 × 0.8 × 0.9 = 0.40 | Drags score down |
| No harvest_metadata | N/A | Base formula only |

**How to determine:**
- Read the KB entry JSON file
- Check for `harvest_metadata` key
- If present: read `overall_confidence`, `field_provenance`, and `source_urls` from `harvest_metadata`
- Calculate `harvest_factor` per formula above
- Blend: `final_score = base_score * 0.70 + harvest_factor * 0.30`
````

- [ ] **Step 4: Update the score calculation example**

Replace the existing `## Score Calculation Example` section with:

````markdown
## Score Calculation Example

**Claim:** "JUCE SmoothedValue uses getNextValue() per sample"

**Base Factors:**
- Source completeness: 1.0 (full file read)
- Cross-reference: 0.7 (2 sources confirm)
- Recency: 1.0 (< 1 year)
- Contradiction: 1.0 (none found)
- Implementation history: 0.5 (similar pattern used)

**Base Calculation:**
```
base_score = 1.0 * 0.30 + 0.7 * 0.25 + 1.0 * 0.15 + 1.0 * 0.20 + 0.5 * 0.10
base_score = 0.30 + 0.175 + 0.15 + 0.20 + 0.05
base_score = 0.875
```

**Harvest metadata present?** Yes — source entry has:
- `overall_confidence`: 0.78
- `field_provenance.code_blocks.method`: "direct-extracted"
- `source_urls`: 3 URLs

**Harvest Factor:**
```
harvest_factor = 0.78 * min(1.1, 1.0) * 1.0 = 0.78
```

**Final Score:**
```
score = 0.875 * 0.70 + 0.78 * 0.30
score = 0.6125 + 0.234
score = 0.847
```

**Result:** MEDIUM (0.85) — close to HIGH threshold
````

- [ ] **Step 5: Verify changes**

Read `~/.claude/skills/kb-validate/confidence-scorer.md` and confirm:
- Formula section shows both base and blended versions
- Harvest factor section has complete calculation with method_modifier and source_count_modifier
- Example updated with harvest_metadata blending
- No broken references

- [ ] **Step 6: Commit**

```bash
git add ~/.claude/skills/kb-validate/confidence-scorer.md
git commit -m "feat(kb-validate): add harvest_metadata confidence factor (§9.2)

Blend harvest_metadata.overall_confidence into validation scoring when
present. Direct-extracted code_blocks get boost, single-source entries
get penalty. Base 5-factor formula unchanged when no harvest_metadata.

Spec: Section 9.2

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 3: Update kb-validate SKILL.md, auto-investigate.md, and INTEGRATION.md (§9.2)

**Files:**
- Modify: `~/.claude/skills/kb-validate/SKILL.md`
- Modify: `~/.claude/skills/kb-validate/auto-investigate.md`
- Modify: `~/.claude/skills/kb-validate/INTEGRATION.md`

Three targeted changes: (1) update SKILL.md prerequisites to mention harvest_metadata, (2) replace firecrawl-search references in auto-investigate.md with WebSearch, (3) update INTEGRATION.md hook matchers to reference kb-harvest instead of kb-autofill.

- [ ] **Step 1: Update SKILL.md prerequisites**

In `~/.claude/skills/kb-validate/SKILL.md`, find the `## Prerequisites` section (line 69):

```markdown
## Prerequisites

- Source files must be readable (KB files, playbooks)
- Session cache initialized in memory
- For auto-investigate: firecrawl-search available
```

Replace with:

```markdown
## Prerequisites

- Source files must be readable (KB files, playbooks)
- Session cache initialized in memory
- For auto-investigate: WebSearch available (replaces firecrawl-search)
- When validating KB entries: reads `harvest_metadata` if present to factor `overall_confidence` into scoring (see confidence-scorer.md)
```

- [ ] **Step 2: Update SKILL.md hook trigger list**

In `~/.claude/skills/kb-validate/SKILL.md`, find the trigger list (line 23):

```markdown
1. **Post-Knowledge Hook**: After firecrawl-scrape, firecrawl-search, kb-harvest, kb-autofill, kb-sync return technical content
```

Replace with:

```markdown
1. **Post-Knowledge Hook**: After kb-harvest, kb-sync, firecrawl-scrape, firecrawl-search return technical content
```

- [ ] **Step 3: Update auto-investigate.md**

In `~/.claude/skills/kb-validate/auto-investigate.md`, find the cross-reference investigation steps (line 37):

```markdown
**For cross-reference gaps:**
```markdown
1. Extract key terms from claim
2. Use firecrawl-search to find additional sources
3. Use WebSearch for public documentation
4. Use Grep to search local KB files
5. Record supporting sources found
6. Update cross_reference score
7. Re-score
```
```

Replace with:

```markdown
**For cross-reference gaps:**
```markdown
1. Extract key terms from claim
2. Use Grep to search local KB files (check all registered KBs via ~/.claude/kb-registry.json)
3. Use WebSearch for public documentation
4. Record supporting sources found
5. Update cross_reference score
6. Re-score
```
```

Also find the recency investigation steps (line 48):

```markdown
**For recency issues:**
```markdown
1. Identify date/version from source
2. Use firecrawl-search for recent documentation
3. Use WebSearch for latest version docs
4. Compare information for changes
5. Update recency score
6. Re-score
```
```

Replace with:

```markdown
**For recency issues:**
```markdown
1. Identify date/version from source
2. Use WebSearch for latest version docs
3. Compare information for changes
4. Update recency score
5. Re-score
```
```

Also update the integration section at the end (line 140):

```markdown
**Uses:**
- `firecrawl-search` - Search for additional sources
- `WebSearch` - Search public documentation
- `Grep` - Search local KB files
- `Read` - Read full source files
```

Replace with:

```markdown
**Uses:**
- `WebSearch` - Search for additional sources and public documentation
- `Grep` - Search local KB files (across all registered KBs)
- `Read` - Read full source files
```

- [ ] **Step 4: Update INTEGRATION.md hook matchers**

In `~/.claude/skills/kb-validate/INTEGRATION.md`, find the post-knowledge hook (line 21):

```json
"matcher": "firecrawl-scrape|firecrawl-search|kb-harvest|kb-autofill|kb-sync",
```

Replace with:

```json
"matcher": "kb-harvest|kb-sync|firecrawl-scrape|firecrawl-search",
```

- [ ] **Step 5: Verify all three files**

Read each modified file and confirm:
- No remaining references to `kb-autofill` in any kb-validate file
- `firecrawl-search` removed as primary investigation tool (WebSearch replaces it)
- harvest_metadata mentioned in prerequisites
- Hook matchers updated

```bash
grep -r "kb-autofill" ~/.claude/skills/kb-validate/
grep -r "firecrawl-search" ~/.claude/skills/kb-validate/
```

Expected: no matches for kb-autofill, no matches for firecrawl-search.

- [ ] **Step 6: Commit**

```bash
git add ~/.claude/skills/kb-validate/SKILL.md
git add ~/.claude/skills/kb-validate/auto-investigate.md
git add ~/.claude/skills/kb-validate/INTEGRATION.md
git commit -m "feat(kb-validate): update references for kb-harvest integration (§9.2)

Replace firecrawl-search with WebSearch in auto-investigate. Remove
kb-autofill from hook matchers. Add harvest_metadata prerequisite note.

Spec: Section 9.2

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 4: Update juce-sound-design-bridge KB dependency (§9.3)

**Files:**
- Modify: `~/.claude/skills/juce-sound-design-bridge/SKILL.md`

Four changes per spec §9.3: (1) KB registry integration, (2) placeholder detection → kb-harvest, (3) confidence awareness, (4) bridge lookup via master-index.

- [ ] **Step 1: Read current KB Dependency section**

In `~/.claude/skills/juce-sound-design-bridge/SKILL.md`, read the `## KB Dependency` section (starts at line 544). This section has `### Placeholder Detection` and `### Sound Design KB Structure`.

- [ ] **Step 2: Replace the KB Dependency section**

Find the entire section from `## KB Dependency` (line 544) through `### Fallback Content` (ending at line 589). Replace with:

```markdown
## KB Dependency

This skill reads from the Sound Design Knowledge Base. KB location is resolved via the registry — not hardcoded paths.

### Registry Resolution

1. Read `~/.claude/kb-registry.json`
2. Find the registered KB that has a `sound-design` layer (or `sound-design-kb` for prototype)
3. Resolve path: `registry.path + "/" + layer_name + "/"`
4. If no registered KB has a sound-design layer, use fallback content (see below)

```python
# Pseudocode for KB resolution
import json

def resolve_sound_design_kb():
    registry_path = os.path.expanduser("~/.claude/kb-registry.json")
    if not os.path.exists(registry_path):
        return None  # Use fallback content
    
    with open(registry_path) as f:
        registry = json.load(f)
    
    for kb in registry["registries"]:
        for layer in kb["layers"]:
            if "sound-design" in layer:
                return {"kb_name": kb["name"], "kb_path": kb["path"], "layer": layer}
    
    return None  # Use fallback content
```

### Placeholder Detection

1. **Read manifest for the sound-design layer:**
   ```bash
   cat <kb_path>/<layer>/manifest.json | jq '.entries[] | select(.status == "placeholder")'
   ```

2. **If entry status == "placeholder":**
   - Log: "Placeholder detected in [kb_name]/[layer]/[topic]/[filename]"
   - Invoke kb-harvest to fill it:
     ```
     kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1
     ```
   - Wait for completion
   - Re-read KB file

3. **Read KB file and use content:**
   ```python
   import json
   kb_file = f"{kb_path}/{layer}/{topic}/{filename}"
   with open(kb_file) as f:
       data = json.load(f)
   markdown = data.get("original_markdown", "")
   ```

### Confidence Awareness

When reading a KB entry, check `harvest_metadata.overall_confidence` if present:

| Confidence | Action |
|------------|--------|
| >= 0.60 | Use content normally |
| 0.40 - 0.59 | Use content but warn: "Low confidence translation (confidence: X.XX). Recommend verifying in DAW testing." |
| < 0.40 | Do not use content. Warn user and use fallback content instead. |

### Bridge Lookup via Master-Index

Instead of hardcoded bridge paths, use the master-index cross_references:

1. Read `<kb_path>/master-index.json`
2. For a descriptor (e.g., "warm"), search `cross_references` for entries containing that concept
3. Follow cross_references to find bridge entries in bridge-eligible layers
4. Return the bridge entry with its confidence and parameter mappings

### Sound Design KB Structure

Resolved dynamically from registry. Typical structure:

| Sonic Goal | KB Path (relative to layer) | Parameter Guidance |
|------------|---------|---------------------|
| Warm | filter/warm.json | Filter settings for warmth |
| Bright | filter/bright.json | High frequency emphasis |
| Punchy | envelope/punchy.json | Transient shaping |
| Lush | modulation/lush.json | Chorus/detune settings |

### Fallback Content

If registry is missing, KB not registered, or entry below confidence threshold:

```json
{
  "title": "Sound Design: Warm",
  "status": "placeholder",
  "fallback": {
    "filter_cutoff": [0.2, 0.4],
    "filter_resonance": [0.1, 0.2],
    "filter_type": "lowpass"
  }
}
```
```

- [ ] **Step 3: Update Key Files section**

Find the `## Key Files` section (line 388). Replace:

```markdown
## Key Files

- **Playbook:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json` — sound_design section
- **Ground Truth Presets:** `~/.agents/juce-agent/validation-logs/GROUND_TRUTH_PRESETS.md`
- **Validation Logs:** `~/.agents/juce-agent/validation-logs/<project-name>/`
- **Global Patterns:** `~/.agents/juce-agent/validation-logs/global-patterns.json`
- **Harvested Content:** `~/.agents/juce-agent/validation-logs/harvested/`
```

With:

```markdown
## Key Files

- **KB Registry:** `~/.claude/kb-registry.json` — resolves KB locations
- **Playbook:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json` — sound_design section
- **Ground Truth Presets:** `~/.agents/juce-agent/validation-logs/GROUND_TRUTH_PRESETS.md`
- **Validation Logs:** `~/.agents/juce-agent/validation-logs/<project-name>/`
- **Global Patterns:** `~/.agents/juce-agent/validation-logs/global-patterns.json`
```

- [ ] **Step 4: Verify changes**

Read `~/.claude/skills/juce-sound-design-bridge/SKILL.md` and confirm:
- `## KB Dependency` section uses registry resolution
- No references to `kb-autofill` remain
- Placeholder detection uses `kb-harvest --kb <name> --auto --entry <id> --batch 1`
- Confidence awareness table present
- Bridge lookup uses master-index cross_references
- Fallback content still present

```bash
grep -n "kb-autofill" ~/.claude/skills/juce-sound-design-bridge/SKILL.md
grep -n "playbookdata" ~/.claude/skills/juce-sound-design-bridge/SKILL.md
```

Expected: no matches for kb-autofill. The only `playbookdata` references should be in the playbook path (which is still valid — the playbook itself hasn't moved).

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/juce-sound-design-bridge/SKILL.md
git commit -m "feat(sound-design-bridge): registry + kb-harvest + confidence (§9.3)

Replace hardcoded KB paths with registry lookup. Replace kb-autofill
with kb-harvest invocation. Add confidence awareness (warn below 0.60,
block below 0.40). Add bridge lookup via master-index cross-references.

Spec: Section 9.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 5: Update juce-dsp-implementation KB dependency (§9.3)

**Files:**
- Modify: `~/.claude/skills/juce-dsp-implementation/SKILL.md`

Same four changes as Task 4, adapted for DSP KB content.

- [ ] **Step 1: Read current KB Dependency section**

In `~/.claude/skills/juce-dsp-implementation/SKILL.md`, read the `## KB Dependency` section (starts at line 452). This section has `### Placeholder Detection`, `### Common KB References`, and `### Fallback if Harvest Fails`.

- [ ] **Step 2: Replace the KB Dependency section**

Find the entire section from `## KB Dependency` (line 452) through `### Fallback if Harvest Fails` (ending at line 506). Replace with:

```markdown
## KB Dependency

This skill reads from the DSP Knowledge Base and related technical KBs. KB location is resolved via the registry — not hardcoded paths.

### Registry Resolution

1. Read `~/.claude/kb-registry.json`
2. Find the registered KB that has a `dsp` layer (or `dsp-kb` for prototype)
3. Resolve path: `registry.path + "/" + layer_name + "/"`
4. If no registered KB has a DSP layer, use fallback content (see below)

```python
# Pseudocode for KB resolution
import json

def resolve_dsp_kb():
    registry_path = os.path.expanduser("~/.claude/kb-registry.json")
    if not os.path.exists(registry_path):
        return None  # Use fallback content
    
    with open(registry_path) as f:
        registry = json.load(f)
    
    for kb in registry["registries"]:
        for layer in kb["layers"]:
            if "dsp" in layer:
                return {"kb_name": kb["name"], "kb_path": kb["path"], "layer": layer}
    
    return None  # Use fallback content
```

### Placeholder Detection

1. **Read manifest for the DSP layer:**
   ```bash
   cat <kb_path>/<layer>/manifest.json | jq '.entries[] | select(.status == "placeholder")'
   ```

2. **If entry status == "placeholder":**
   - Log: "Placeholder detected in [kb_name]/[layer]/[topic]/[filename]"
   - Invoke kb-harvest to fill it:
     ```
     kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1
     ```
   - Wait for completion
   - Re-read KB file

3. **Read KB file and use content:**
   ```python
   import json
   kb_file = f"{kb_path}/{layer}/{topic}/{filename}"
   with open(kb_file) as f:
       data = json.load(f)
   markdown = data.get("original_markdown", "")
   code_blocks = data.get("code_blocks", [])
   ```

### Confidence Awareness

When reading a KB entry, check `harvest_metadata.overall_confidence` if present:

| Confidence | Action |
|------------|--------|
| >= 0.60 | Use content normally |
| 0.40 - 0.59 | Use content but warn: "Low confidence DSP reference (confidence: X.XX). Verify algorithm correctness before implementation." |
| < 0.40 | Do not use content. Warn user and use fallback content instead. |

**Code block confidence:** For entries with `field_provenance.code_blocks.method == "direct-extracted"`, code examples can be trusted more than `ai-synthesized` code blocks which may contain errors.

### Common KB References

Resolved dynamically from registry. Typical structure:

| DSP Topic | KB Path (relative to layer) | Description |
|-----------|---------|-------------|
| Reverb | reverb/algorithmic-reverb.json | Algorithmic reverb design |
| Reverb | reverb/convolution-reverb.json | Convolution reverb |
| Dynamics | dynamics/compressor.json | Compression algorithms |
| Dynamics | dynamics/limiter-design.json | Limiter design |

### Fallback if Harvest Fails

If registry is missing, KB not registered, or entry below confidence threshold:

```json
{
  "title": "DSP Topic",
  "status": "placeholder",
  "fallback": {
    "brief": "Basic description available",
    "key_concepts": ["concept1", "concept2"],
    "common_algorithms": ["algo1", "algo2"]
  }
}
```
```

- [ ] **Step 3: Verify changes**

```bash
grep -n "kb-autofill" ~/.claude/skills/juce-dsp-implementation/SKILL.md
```

Expected: no matches.

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/juce-dsp-implementation/SKILL.md
git commit -m "feat(dsp-implementation): registry + kb-harvest + confidence (§9.3)

Replace hardcoded KB paths with registry lookup. Replace kb-autofill
with kb-harvest invocation. Add confidence awareness with code block
provenance checking. Same pattern as sound-design-bridge update.

Spec: Section 9.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 6: Add KB registry awareness to juce-ui-bridge (§9.3)

**Files:**
- Modify: `~/.claude/skills/juce-ui-bridge/SKILL.md`

juce-ui-bridge currently reads UI design data from the playbook's `ui_design` section, not from KB files directly. Add a KB dependency section so it can optionally read from a registered UI KB layer when available.

- [ ] **Step 1: Read end of current SKILL.md**

Read `~/.claude/skills/juce-ui-bridge/SKILL.md` lines 410-612 to find where to add the new section.

- [ ] **Step 2: Add KB Dependency section**

Before the `## Fallback and Error Handling` section (line 443), add:

```markdown
## KB Dependency

This skill primarily reads from the playbook's `ui_design` section. When a registered KB has a `ui` or `ui-ux` layer, the skill can also read UI knowledge entries for richer guidance.

### Registry Resolution

1. Read `~/.claude/kb-registry.json`
2. Find a registered KB with a `ui` or `ui-ux` layer (or `ui-kb` for prototype)
3. If found: read entries from that layer for design patterns, component examples, color schemes
4. If not found: use playbook `ui_design` section only (current behavior)

### Confidence Awareness

When reading KB entries with `harvest_metadata`:

| Confidence | Action |
|------------|--------|
| >= 0.60 | Use UI guidance normally |
| 0.40 - 0.59 | Use guidance but note: "Low confidence UI reference — verify recommendations" |
| < 0.40 | Skip KB entry, use playbook fallback |

### Placeholder Detection

If a UI KB entry has status "placeholder":
```
kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1
```
```

- [ ] **Step 3: Update Key Files section**

Find `## Key Files` (line 414). Add at the top of the list:

```markdown
- **KB Registry:** `~/.claude/kb-registry.json` — resolves KB locations (optional — skill works without it)
```

- [ ] **Step 4: Verify changes**

```bash
grep -n "kb-registry" ~/.claude/skills/juce-ui-bridge/SKILL.md
grep -n "kb-autofill" ~/.claude/skills/juce-ui-bridge/SKILL.md
```

Expected: kb-registry found, kb-autofill not found.

- [ ] **Step 5: Commit**

```bash
git add ~/.claude/skills/juce-ui-bridge/SKILL.md
git commit -m "feat(ui-bridge): add KB registry awareness (§9.3)

Add optional KB dependency section for reading from registered UI KB
layers. Skill still works without registry (falls back to playbook).
Add confidence awareness for KB entries.

Spec: Section 9.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 7: Add KB registry awareness to juce-plugin-spec (§9.3)

**Files:**
- Modify: `~/.claude/skills/juce-plugin-spec/SKILL.md`

juce-plugin-spec delegates KB access to sound-design-bridge and ui-bridge. Add a note about registry-aware KB resolution so the skill knows the bridge skills will use the registry.

- [ ] **Step 1: Read the Sound Design Integration section**

In `~/.claude/skills/juce-plugin-spec/SKILL.md`, read section `### 2a. Sound Design Integration` (line 72).

- [ ] **Step 2: Add registry note to Sound Design Integration**

After line 76 (`**Map sonic descriptors to capabilities:**`), before the bullet list, add:

```markdown
**KB Resolution:** juce-sound-design-bridge resolves KB paths via `~/.claude/kb-registry.json`. If no registry exists, the bridge uses built-in fallback translations.

```

- [ ] **Step 3: Add registry note to UI Design Questions**

After line 199 (`### 2b. UI Design Questions (human_only)`), before the first question, add:

```markdown
**KB Resolution:** juce-ui-bridge resolves KB paths via `~/.claude/kb-registry.json`. If no registry exists, the bridge reads from the playbook's `ui_design` section.

```

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/juce-plugin-spec/SKILL.md
git commit -m "feat(plugin-spec): add KB registry awareness notes (§9.3)

Document that sound-design-bridge and ui-bridge resolve KB paths via
registry. No behavioral changes — plugin-spec delegates KB access.

Spec: Section 9.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 8: Add KB registry awareness to juce-daw-testing (§9.3)

**Files:**
- Modify: `~/.claude/skills/juce-daw-testing/SKILL.md`

juce-daw-testing has feedback translation tables and validation logging. Add registry awareness and confidence notes.

- [ ] **Step 1: Read the Validation Logging section**

In `~/.claude/skills/juce-daw-testing/SKILL.md`, read the `## Validation Logging (CRITICAL)` section (line 349).

- [ ] **Step 2: Add KB registry note**

Before the `## Validation Logging (CRITICAL)` section (line 349), add:

```markdown
## KB Registry Awareness

The feedback translation tables in this skill are built-in references. When a registered KB has a `sound-design` layer with richer translations, prefer KB content over the built-in table.

### Resolution

1. Read `~/.claude/kb-registry.json`
2. If a KB with `sound-design` layer exists: read bridge entries for feedback descriptor mappings
3. If not: use the built-in Feedback Translation Reference table above

### Confidence Check

When using KB entries for feedback translation:
- If `harvest_metadata.overall_confidence >= 0.60`: use KB translation
- If confidence < 0.60: use built-in table instead, note: "KB translation has low confidence — using built-in defaults"

```

- [ ] **Step 3: Update validation logging for new KB structure**

In the `### Sound Design Validation Logging` section (line 353), after step 4 (updating global-patterns.json), add:

```markdown
5. **Feed back to KB:** If a translation was significantly adjusted during testing (user feedback changed parameters by >20% from suggested), log this as a candidate for KB entry review:
   ```
   kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1
   ```
   This triggers re-harvest which may find better sources for that translation.
```

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/juce-daw-testing/SKILL.md
git commit -m "feat(daw-testing): add KB registry awareness + confidence (§9.3)

Prefer registered KB sound-design translations over built-in table when
available and confidence >= 0.60. Add feedback loop to trigger
re-harvest when translations need significant adjustment.

Spec: Section 9.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 9: Verify skill retirement (§9.4)

**Files:**
- Check: `~/.claude/skills/kb-autofill.bak/` (should exist)
- Check: `~/.claude/skills/harvest-data.bak/` (should exist)
- Check: All modified skills for stale references

Plan B Task 14 handles the actual retirement (renaming to .bak). This task verifies the retirement is complete and no stale references remain.

- [ ] **Step 1: Verify .bak directories exist**

```bash
test -d ~/.claude/skills/kb-autofill.bak && echo "OK: kb-autofill.bak exists" || echo "MISSING: kb-autofill.bak"
test -d ~/.claude/skills/harvest-data.bak && echo "OK: harvest-data.bak exists" || echo "MISSING: harvest-data.bak"
test -d ~/.claude/skills/kb-autofill && echo "ERROR: kb-autofill still active!" || echo "OK: kb-autofill removed"
test -d ~/.claude/skills/harvest-data && echo "ERROR: harvest-data still active!" || echo "MISSING: harvest-data removed"
```

Expected: both .bak exist, neither active directory exists. If active directories still exist, Plan B Task 14 hasn't been executed yet — wait for Plan B completion.

- [ ] **Step 2: Scan all modified skills for stale references**

```bash
echo "=== Checking for kb-autofill references ==="
grep -rl "kb-autofill" ~/.claude/skills/kb-sync/ ~/.claude/skills/kb-validate/ \
  ~/.claude/skills/juce-sound-design-bridge/ ~/.claude/skills/juce-dsp-implementation/ \
  ~/.claude/skills/juce-ui-bridge/ ~/.claude/skills/juce-plugin-spec/ \
  ~/.claude/skills/juce-daw-testing/ 2>/dev/null || echo "None found (good)"

echo "=== Checking for harvest-data references ==="
grep -rl "harvest-data" ~/.claude/skills/kb-sync/ ~/.claude/skills/kb-validate/ \
  ~/.claude/skills/juce-sound-design-bridge/ ~/.claude/skills/juce-dsp-implementation/ \
  ~/.claude/skills/juce-ui-bridge/ ~/.claude/skills/juce-plugin-spec/ \
  ~/.claude/skills/juce-daw-testing/ 2>/dev/null || echo "None found (good)"

echo "=== Checking for hardcoded playbookdata KB paths ==="
grep -rn "playbookdata.*manifest\|playbookdata.*\.json" ~/.claude/skills/kb-sync/ \
  ~/.claude/skills/juce-sound-design-bridge/ ~/.claude/skills/juce-dsp-implementation/ \
  2>/dev/null || echo "None found (good)"
```

Expected: all "None found (good)". If any stale references found, fix them before proceeding.

- [ ] **Step 3: Verify kb-harvest exists and is active**

```bash
test -f ~/.claude/skills/kb-harvest/SKILL.md && echo "OK: kb-harvest active" || echo "MISSING: kb-harvest"
head -5 ~/.claude/skills/kb-harvest/SKILL.md
```

Expected: kb-harvest SKILL.md exists with correct frontmatter.

- [ ] **Step 4: Commit (only if fixes were needed)**

If Step 2 found stale references that needed fixing:

```bash
git add -A ~/.claude/skills/
git commit -m "fix: remove stale kb-autofill/harvest-data references (§9.4)

Clean up remaining references to retired skills found during
integration verification.

Spec: Section 9.4

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

If no fixes needed, skip this step.

---

## Task 10: Cross-skill consistency verification

**Files:**
- Read: All 7 modified skill SKILL.md files
- Read: `~/.claude/kb-registry.json`

Final verification that all skills use consistent interfaces.

- [ ] **Step 1: Verify registry path consistency**

All skills should reference the same registry path:

```bash
grep -rn "kb-registry.json" ~/.claude/skills/kb-sync/SKILL.md \
  ~/.claude/skills/kb-validate/ \
  ~/.claude/skills/juce-sound-design-bridge/SKILL.md \
  ~/.claude/skills/juce-dsp-implementation/SKILL.md \
  ~/.claude/skills/juce-ui-bridge/SKILL.md \
  ~/.claude/skills/juce-plugin-spec/SKILL.md \
  ~/.claude/skills/juce-daw-testing/SKILL.md
```

Expected: all references use `~/.claude/kb-registry.json` (not variations like `$HOME/.claude/` or absolute paths).

- [ ] **Step 2: Verify kb-harvest invocation consistency**

All consumption skills should use the same invocation pattern:

```bash
grep -rn "kb-harvest" ~/.claude/skills/juce-sound-design-bridge/SKILL.md \
  ~/.claude/skills/juce-dsp-implementation/SKILL.md \
  ~/.claude/skills/juce-ui-bridge/SKILL.md \
  ~/.claude/skills/juce-daw-testing/SKILL.md
```

Expected: all use `kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1` pattern.

- [ ] **Step 3: Verify confidence thresholds consistency**

All consumption skills should use the same confidence thresholds:

```bash
grep -rn "0.60\|0.40" ~/.claude/skills/juce-sound-design-bridge/SKILL.md \
  ~/.claude/skills/juce-dsp-implementation/SKILL.md \
  ~/.claude/skills/juce-ui-bridge/SKILL.md \
  ~/.claude/skills/juce-daw-testing/SKILL.md
```

Expected: all use 0.60 as the "use normally" threshold and 0.40 as the "do not use" threshold.

- [ ] **Step 4: Verify no orphaned cross-references**

Check that skills don't reference skills that no longer exist:

```bash
echo "=== References to retired skills ==="
grep -rn "kb-autofill\|harvest-data" ~/.claude/skills/*/SKILL.md 2>/dev/null || echo "None (good)"

echo "=== References to sync_engine.py ==="
grep -rn "sync_engine" ~/.claude/skills/*/SKILL.md 2>/dev/null || echo "None (good)"

echo "=== References to migrate_content.py ==="
grep -rn "migrate_content" ~/.claude/skills/*/SKILL.md 2>/dev/null || echo "None (good)"
```

Expected: all "None (good)".

- [ ] **Step 5: Summary report**

Print a summary of all changes made by this plan:

```
Integration Changes Summary:
  
  kb-sync:                    REWRITTEN — verification/repair role
  kb-validate:                UPDATED — harvest_metadata confidence factor + reference cleanup
  juce-sound-design-bridge:   UPDATED — registry + kb-harvest + confidence + bridge lookup
  juce-dsp-implementation:    UPDATED — registry + kb-harvest + confidence
  juce-ui-bridge:             UPDATED — registry awareness (optional)
  juce-plugin-spec:           UPDATED — registry awareness notes
  juce-daw-testing:           UPDATED — registry + confidence + feedback loop
  kb-autofill:                VERIFIED retired (.bak)
  harvest-data:               VERIFIED retired (.bak)
  
  Consistency checks:
  - Registry path:            ~/.claude/kb-registry.json (all skills)
  - kb-harvest invocation:    kb-harvest --kb <name> --auto --entry <id> --batch 1
  - Confidence thresholds:    >= 0.60 normal, 0.40-0.59 warn, < 0.40 block
  - Stale references:         None
```
