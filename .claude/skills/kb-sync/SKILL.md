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