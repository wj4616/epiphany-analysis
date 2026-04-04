# PBCPB Meta-System Changes Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Update the PBCPB meta-system so that all future generated playbooks include multi-backend web harvesting support, KB registry integration, and rich entry extraction guidance in their Phase 2/3 architecture.

**Architecture:** Two files change: `templates/output-schema.json` (the JSON Schema governing valid playbook output) and `playbook-creator-playbook.json` (the meta-playbook with phases, tasks, gates, failure modes). Changes span Phase 2 (KB Architecture — 5 task description updates, 1 new task, 9 new gate conditions), Phase 3 (KB Bootstrapping — 1 new task, 1 modified task, 2 new tasks, 1 modified auditor, 8 new gate conditions, handoff updates), Phase 7 (Output Configuration — 2 task description updates), and failure modes (2 new entries). All changes trace from spec Section 7.

**Tech Stack:** Python 3 for JSON manipulation, PBCPB validation scripts (validate_playbook.py, validate_semantic.py, pytest suite)

**Spec:** `docs/superpowers/specs/2026-04-04-kb-harvest-pbcpb-integration-design.md` — Section 7 (PBCPB Meta-System Changes), Section 12 (Change Summary)

**Related Plans:**
- Plan B: KB Infrastructure + kb-harvest Skill (parallel, being executed now)
- Plan C: Integration Changes — kb-sync, kb-validate, consumption skills (depends on Plan B)

**Working Directory:** `/home/myuser/Documents/pbcpb/`

---

## Pre-existing Validation State

Before any changes, the PBCPB has known validation issues:

| Validator | Result | Issue |
|-----------|--------|-------|
| `validate_playbook.py --schema` | 3 errors | `kb_status.total_entries/harvested/placeholder` are `null` but schema says `integer` |
| `validate_semantic.py` | 1 error | FM-028 references non-existent phase `"All phases with gates"` |
| `pytest scripts/` | 64 passed | Clean |

Task 1 fixes the schema errors. The semantic error (FM-028) is pre-existing and unrelated to this plan — do NOT fix it here.

## FM ID Collision

The spec (Section 7.3.8) labels new failure modes as FM-020 and FM-021. However, the PBCPB already has FM-020 through FM-029. This plan uses **FM-030** and **FM-031** instead.

## File Structure

### Files to Modify

```
/home/myuser/Documents/pbcpb/
├── templates/
│   └── output-schema.json              # JSON Schema for playbook output (§7.1)
└── playbook-creator-playbook.json      # Meta-playbook with 16 phases (§7.2-7.4, §7.3.8)
```

No new files are created by this plan.

---

### Task 1: Fix pre-existing output-schema kb_status validation errors

**Files:**
- Modify: `templates/output-schema.json` (lines ~372-380, `kb_status` properties)

The Phase 3 handoff `kb_status` fields use `null` to mean "not yet measured" but the schema only allows `integer`. Fix by accepting both types.

- [ ] **Step 1: Run baseline validation to confirm the 3 errors**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | grep -c "kb_status"`
Expected: `3`

- [ ] **Step 2: Fix kb_status property types to accept null**

```python
import json

schema_path = '/home/myuser/Documents/pbcpb/templates/output-schema.json'
with open(schema_path) as f:
    schema = json.load(f)

# Navigate to kb_status properties
kb_status = schema['properties']['checklists']['items']['properties']['items']['items']['properties']['handoff']['properties']['kb_status']['properties']

for field in ['total_entries', 'harvested', 'placeholder']:
    kb_status[field]['type'] = ['integer', 'null']

with open(schema_path, 'w') as f:
    json.dump(schema, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Fixed: kb_status.total_entries, kb_status.harvested, kb_status.placeholder now accept null')
```

- [ ] **Step 3: Run validation to confirm schema errors are resolved**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -5`
Expected: `PASS` (or only the pre-existing semantic error, not schema errors)

- [ ] **Step 4: Run test suite to confirm no regressions**

Run: `cd /home/myuser/Documents/pbcpb && python3 -m pytest scripts/ -q`
Expected: `64 passed`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add templates/output-schema.json
git commit -m "fix(schema): allow null values in kb_status handoff fields

Phase 3 handoff uses null for kb_status fields before execution populates
them. Schema now accepts [integer, null] instead of integer-only.

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 2: Expand output-schema.json — population_strategy and kb_registry (§7.1.1, §7.1.3)

**Files:**
- Modify: `templates/output-schema.json`

Two changes:
1. Replace `population_strategy` with expanded version supporting web backends, confidence weights, domain rankings, import adapters, skill references (§7.1.1)
2. Add `kb_registry` field to `knowledge_base` (§7.1.3)

- [ ] **Step 1: Write the modification script**

```python
import json

schema_path = '/home/myuser/Documents/pbcpb/templates/output-schema.json'
with open(schema_path) as f:
    schema = json.load(f)

kb = schema['properties']['knowledge_base']['properties']

# §7.1.1: Replace population_strategy with expanded version
kb['population_strategy'] = {
    "type": "object",
    "properties": {
        "placeholder_seeding": {"type": "string"},
        "harvesting_sources": {
            "oneOf": [
                {"type": "array", "items": {"type": "string"}},
                {
                    "type": "object",
                    "properties": {
                        "research_documents": {"type": "array", "items": {"type": "string"}},
                        "web_backends": {
                            "type": "array",
                            "items": {
                                "type": "object",
                                "required": ["name", "search_tool", "scrape_tool"],
                                "properties": {
                                    "name": {"type": "string"},
                                    "search_tool": {"type": "string"},
                                    "scrape_tool": {"type": "string"},
                                    "requirements": {"type": "array", "items": {"type": "string"}},
                                    "quality_floor": {"type": "number", "minimum": 0, "maximum": 1}
                                }
                            }
                        }
                    }
                }
            ]
        },
        "curation_rules": {"type": "string"},
        "sync_rules": {"type": "string"},
        "versioning_protocol": {"type": "string"},
        "search_term_strategy": {"type": "string"},
        "confidence_weights": {
            "type": "object",
            "additionalProperties": {"type": "number"}
        },
        "source_domain_rankings": {"type": "object"},
        "bridge_eligible_layers": {"type": "array", "items": {"type": "string"}},
        "import_adapters": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "format": {"type": "string"},
                    "description": {"type": "string"},
                    "field_map_path": {"type": "string"}
                }
            },
            "description": "Supported import formats for this KB. Built-in adapters (prototype, json-entries, markdown-dir, obsidian, csv) are always available. List custom adapters with field_map paths here."
        },
        "skill_references": {
            "type": "object",
            "properties": {
                "harvesting": {"type": "string"},
                "sync": {"type": "string"},
                "validation": {"type": "string"}
            }
        }
    }
}

# §7.1.3: Add kb_registry to knowledge_base
kb['kb_registry'] = {
    "type": "object",
    "properties": {
        "name": {"type": "string"},
        "default_backend": {"type": "string"},
        "bridge_eligible_layers": {"type": "array", "items": {"type": "string"}}
    }
}

with open(schema_path, 'w') as f:
    json.dump(schema, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Updated: population_strategy expanded with web_backends, confidence_weights, source_domain_rankings, import_adapters, skill_references')
print('Added: kb_registry to knowledge_base')
```

- [ ] **Step 2: Run the modification script**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "<script above>"`

- [ ] **Step 3: Validate schema is still valid JSON and passes self-validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "import json; json.load(open('templates/output-schema.json')); print('Valid JSON')" && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -5`
Expected: `Valid JSON` then `PASS`

- [ ] **Step 4: Run test suite**

Run: `cd /home/myuser/Documents/pbcpb && python3 -m pytest scripts/ -q`
Expected: `64 passed`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add templates/output-schema.json
git commit -m "feat(schema): add multi-backend harvesting to population_strategy and kb_registry

population_strategy gains: web_backends (with oneOf for backward compat),
confidence_weights, source_domain_rankings, bridge_eligible_layers,
import_adapters, skill_references, search_term_strategy.

knowledge_base gains: kb_registry (name, default_backend,
bridge_eligible_layers).

Spec §7.1.1, §7.1.3

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 3: Update Phase 2 task descriptions (§7.2.1–7.2.5)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 2 (checklists[2]), items[1] through items[5]

Append spec-defined guidance to 5 existing Phase 2 task descriptions. Each append adds domain-specific guidance that will appear in generated playbooks.

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase2 = data['checklists'][2]
items = phase2['items']

# §7.2.1: Task 2 — Define knowledge layers (items[1])
items[1]['description'] += (
    "\n\nPrefer granular layers over broad categories. The question is not just "
    "\"do knowledge types differ?\" but \"can an agent find what it needs "
    "without searching through unrelated content?\"\n\n"
    "A single \"technical\" layer spanning DSP algorithms, build systems, "
    "and language reference is too broad. Split into independently queryable "
    "domains where each layer serves a distinct agent need.\n\n"
    "Example: instead of one \"technical\" layer, consider separate layers for "
    "DSP algorithms, JUCE framework patterns, C++ audio patterns, and build "
    "system configuration — each independently searchable."
)

# §7.2.2: Task 3 — Define KB entry schema (items[2])
items[2]['description'] += (
    "\n\nSource provenance (required for all entries):\n"
    "- source.backend: which harvesting backend produced this entry "
    "(research-docs | firecrawl | ddg+webfetch | websearch+webfetch | webfetch | imported). "
    "Default for existing entries without this field: \"research-docs\"\n"
    "- source.url: source URL (required for web-harvested entries)\n"
    "- source.retrieved_date: ISO date when content was fetched\n\n"
    "Original content (required for all non-placeholder entries):\n"
    "- original_markdown: full source content in markdown format. For web-harvested "
    "entries this is the WebFetch output. For research-doc entries this is the "
    "source document section text. This is the most valuable field for agent "
    "consumption — agents can read complete tutorials and reference material, "
    "not just extracted fragments.\n\n"
    "Harvest metadata (required for web-harvested and imported entries):\n"
    "- harvest_metadata.overall_confidence: 0.0-1.0 composite score\n"
    "- harvest_metadata.field_provenance: per-field {method, confidence} "
    "where method is \"direct-extracted\" | \"direct-mapped\" | \"ai-synthesized\" | \"ai-inferred\"\n"
    "- harvest_metadata.source_content_length_chars: character count of source content\n"
    "- harvest_metadata.extraction_prompt_version: version string\n"
    "- harvest_metadata.source_urls: array of URLs content was fetched from\n"
    "- harvest_metadata.fetch_date: ISO date\n"
    "- harvest_metadata.backend_used: which backend was used\n"
    "- harvest_metadata.review_flag: boolean, true if confidence near threshold\n\n"
    "Entry schema must be designed for agent consumption — not reference storage. "
    "Entries should include implementation-ready content:\n"
    "- code_blocks with working examples (not pseudocode) where domain involves code\n"
    "- Parameter ranges with specific values in description field\n"
    "- cross_references linking to related entries across KB layers\n"
    "- original_markdown preserving full source context\n\n"
    "Code preservation: code_blocks must preserve exact original formatting. "
    "C++ code examples must remain valid, compilable code — no reformatting, "
    "simplification, or modification during extraction."
)

# §7.2.3: Task 4 — Define bridge entry schema (items[3])
items[3]['description'] += (
    "\n\nBridge entries from web harvesting gain provenance tracking:\n"
    "- source.type: \"expert-knowledge\" | \"auto-detected\" | \"web-harvested\" | \"external-import\"\n"
    "- source.backend: same enum as entry schema\n"
    "- harvest_metadata: same structure as entry schema (for web-sourced bridges)\n\n"
    "Auto-detected bridges (found during web harvesting of bridge-eligible layers) "
    "start at confidence 0.50 with source.type \"auto-detected\" and review_flag true. "
    "Manual bridges retain their existing confidence levels."
)

# §7.2.4: Task 5 — Define population strategy (items[4])
items[4]['description'] += (
    "\n\nWeb harvesting backends (define all available for this KB):\n"
    "For each backend specify: name, search_tool, scrape_tool, requirements, "
    "and quality_floor (minimum overall_confidence to accept from this backend).\n\n"
    "Standard backends:\n"
    "1. ddg+webfetch — DDG CLI search + WebFetch scrape. Requires DDG CLI. quality_floor: 0.45\n"
    "2. websearch+webfetch — Claude WebSearch + WebFetch. No requirements. quality_floor: 0.40\n"
    "3. webfetch — User provides URLs, WebFetch scrapes. No requirements. quality_floor: 0.50 (higher because user curated the source)\n"
    "4. firecrawl — Firecrawl search + scrape. Requires API key + credits. quality_floor: 0.55\n"
    "5. imported — Local files via format adapter + Claude extraction. No requirements. quality_floor: 0.45\n\n"
    "Confidence weights: define per-field weights for overall_confidence "
    "calculation. Weight fields by importance to agent consumption in this domain. "
    "Default weights: description 0.25, concepts 0.15, code_blocks 0.25, "
    "title 0.05, summary 0.05, tags 0.05, related_topics 0.05, "
    "difficulty 0.05, domain_relevance 0.05, cross_references 0.05. "
    "All weighted fields correspond to extraction prompt fields in the kb-harvest skill.\n\n"
    "Source domain rankings: define quality tiers for web sources relevant "
    "to this KB's domain. Academic and official documentation sources rank "
    "highest. Include specific domain names where possible.\n\n"
    "Bridge-eligible layers: list which KB layers may contain content that "
    "maps subjective descriptors to technical parameters. Only these layers "
    "trigger bridge auto-detection during harvesting.\n\n"
    "Search term strategy: define how KB topics and placeholder entries are "
    "expanded into search queries from multiple perspectives:\n"
    "- Academic: '[topic] algorithm', '[topic] computational method', '[topic] IEEE paper'\n"
    "- Practitioner: '[topic] implementation', '[topic] best practices', '[topic] production code'\n"
    "- Educator: '[topic] tutorial advanced', '[topic] deep dive'\n"
    "- Domain expert: domain-specific terminology variants, alternative names\n"
    "Include code-targeting queries: '[topic] C++ source code', "
    "'[topic] JUCE tutorial code example', '[topic] site:github.com'\n\n"
    "Import adapters: list any custom import formats this KB needs beyond "
    "the built-in adapters (prototype, json-entries, markdown-dir, obsidian, csv). "
    "For each custom adapter, provide a field mapping file path.\n\n"
    "Skill references: specify which skills execute each population task:\n"
    "- Harvesting + import: kb-harvest skill\n"
    "- Sync/verification: kb-sync skill\n"
    "- Validation: kb-validate skill"
)

# §7.2.5: Task 6 — Define index and directory structure (items[5])
items[5]['description'] += (
    "\n\nMaster index format: use concept-level cross-referencing format. "
    "The master-index maps concepts to which KBs contain relevant content. "
    "Structure: knowledge_bases{} listing each KB with path, description, "
    "source, status, file_count, topics, and layer authority scores. "
    "cross_references{} mapping each concept/tag to which KBs contain it.\n\n"
    "Per-KB manifest format: per-file status tracking with metadata. "
    "Each file entry in the manifest must include: status, harvested_at, "
    "synced_at, has_semantic, source (backend used), markdown_length, "
    "code_block_count, confidence, review_flag.\n\n"
    "Manifests MUST accurately reflect actual files on disk. The Phase 3 "
    "auditor will verify this with automated file counting."
)

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Updated 5 Phase 2 task descriptions:')
print('  items[1]: Define knowledge layers — added granularity guidance')
print('  items[2]: Define KB entry schema — added source provenance, original_markdown, harvest_metadata')
print('  items[3]: Define bridge entry schema — added bridge provenance tracking')
print('  items[4]: Define population strategy — added web backends, weights, rankings, skill refs')
print('  items[5]: Define index and directory structure — added master-index and manifest formats')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`

- [ ] **Step 3: Verify changes applied correctly**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
items = data['checklists'][2]['items']
checks = [
    ('items[1]', 'granular layers' in items[1]['description']),
    ('items[2]', 'source.backend' in items[2]['description']),
    ('items[3]', 'auto-detected' in items[3]['description']),
    ('items[4]', 'ddg+webfetch' in items[4]['description']),
    ('items[5]', 'concept-level cross-referencing' in items[5]['description']),
]
for name, ok in checks:
    print(f'  {name}: {\"PASS\" if ok else \"FAIL\"}')
"`
Expected: all 5 PASS

- [ ] **Step 4: Run full validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3 && python3 -m pytest scripts/ -q`
Expected: `PASS` + `64 passed`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase2): add web harvesting guidance to KB architecture tasks

Tasks 2-6 gain guidance for: granular layer design, source provenance
fields (source.backend, original_markdown, harvest_metadata), bridge
provenance tracking, multi-backend population strategy (5 backends,
confidence weights, domain rankings, skill refs), and unified
master-index/manifest formats.

Spec §7.2.1–§7.2.5

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 4: Add Phase 2 new task (KB access spec) + update gate conditions (§7.2.6, §7.2.7)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 2 (checklists[2])

Four changes:
1. Insert new task "Define KB access specification" before the gate (§7.2.6)
2. Add 9 new gate conditions to the gate (§7.2.7)
3. Add `kb-access-spec.md` to gate handoff (output_artifacts + next_phase_context)
4. Add 3 new success_criteria to Phase 2 compilation

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase2 = data['checklists'][2]
items = phase2['items']

# §7.2.6: Insert new task before gate (currently items[6])
new_task = {
    "title": "[Architect] — Define KB access specification",
    "owner": "[Architect]",
    "description": (
        "Define how consumption skills find and read KB entries. This specification "
        "replaces hardcoded KB paths in skills.\n\n"
        "Specify:\n"
        "- KB registry entry format (name, path, schema, layers, bridge_eligible_layers)\n"
        "- Entry lookup pattern: how a skill resolves a query to specific KB entries via master-index cross_references\n"
        "- Cross-layer query pattern: how a skill follows cross_references[] across KB layers to find related content\n"
        "- Bridge lookup pattern: how a skill finds bridge entries for a given descriptor via master-index\n"
        "- Fallback behavior: when a KB entry is placeholder or below confidence threshold, invoke kb-harvest --auto --kb <name> --entry <id>\n"
        "- Multi-KB resolution: when multiple registered KBs contain relevant content for a query, which takes precedence (by authority_score)"
    ),
    "output": "kb-access-spec.md"
}
items.insert(6, new_task)
# Gate is now items[7]

# §7.2.7: Add 9 new gate conditions to items[7]
gate = items[7]
new_conditions = [
    "Entry schema includes source.backend, original_markdown, and harvest_metadata fields",
    "Population strategy defines at least one web harvesting backend with quality_floor",
    "Population strategy includes confidence_weights and source_domain_rankings",
    "Population strategy identifies bridge-eligible layers",
    "Population strategy references kb-harvest, kb-sync, and kb-validate skills",
    "KB access specification defined for consumption skills",
    "KB registry entry format defined",
    "Master-index format uses concept-level cross-referencing (not layer-level only)",
    "Per-KB manifest format includes per-file status, markdown_length, code_block_count, confidence"
]
gate['gate_conditions'].extend(new_conditions)

# Add kb-access-spec.md to gate handoff
gate['handoff']['output_artifacts'].append("kb-access-spec.md")
gate['handoff']['next_phase_context'].append("kb-access-spec.md")

# Add success_criteria to Phase 2 compilation
phase2['compilation']['success_criteria'].extend([
    "Entry schema includes source provenance (source.backend) and harvest metadata fields",
    "Population strategy includes web harvesting backends with confidence weights and domain rankings",
    "KB access specification defined for consumption skills"
])

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Inserted: new task "Define KB access specification" at items[6]')
print(f'Added: {len(new_conditions)} gate conditions to Phase 2 gate (now items[7])')
print('Added: kb-access-spec.md to handoff output_artifacts and next_phase_context')
print('Added: 3 new success_criteria to Phase 2 compilation')
print(f'Phase 2 now has {len(items)} items (was 7)')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`
Expected: Phase 2 now has 8 items

- [ ] **Step 3: Verify changes**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
p2 = data['checklists'][2]
items = p2['items']
print(f'Item count: {len(items)} (expected 8)')
print(f'items[6] title: {items[6][\"title\"]}')
print(f'items[7] title: {items[7][\"title\"]}')
print(f'Gate conditions: {len(items[7][\"gate_conditions\"])} (expected 18)')
print(f'Handoff artifacts: {len(items[7][\"handoff\"][\"output_artifacts\"])} (expected 6)')
print(f'Success criteria: {len(p2[\"compilation\"][\"success_criteria\"])} (expected 8)')
"`
Expected:
- Item count: 8
- items[6]: `[Architect] — Define KB access specification`
- items[7]: `[Coordinator] — Phase gate: KB architecture specified`
- Gate conditions: 18 (9 original + 9 new)
- Handoff artifacts: 6
- Success criteria: 8

- [ ] **Step 4: Run full validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3 && python3 -m pytest scripts/ -q`
Expected: `PASS` + `64 passed`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase2): add KB access spec task and gate conditions

New task: 'Define KB access specification' — defines how consumption
skills find KB entries via registry, cross-references, and bridge lookups.

Gate gains 9 conditions enforcing: source provenance fields, web backends
with quality floors, confidence weights, domain rankings, bridge-eligible
layers, skill references, KB access spec, registry format, manifest format.

Spec §7.2.6, §7.2.7

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 5: Add Phase 3 Register KB task + update compilation (§7.3.1)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 3 (checklists[3])

Four changes:
1. Insert "Register KB in system registry" task after item[0] (directory creation)
2. Update Phase 3 `compilation.objective` to include registration and web harvesting
3. Add `kb-access-spec.md` to Phase 3 `context_load` and `context_budget.priority`
4. Add FM-030, FM-031 to `failure_modes_relevant` (FM IDs will be created in Task 9)

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase3 = data['checklists'][3]
items = phase3['items']

# §7.3.1: Insert Register KB task after items[0] (directory creation)
register_task = {
    "title": "[Builder] — Register KB in system registry",
    "owner": "[Builder]",
    "description": (
        "Register this KB in ~/.claude/kb-registry.json using the format defined in kb-access-spec.md.\n\n"
        "If ~/.claude/kb-registry.json does not exist, create it with version 1.0.0 and empty registries array.\n\n"
        "Add or update entry with: name (from playbook title, slugified), path (absolute path to kb/ directory), "
        "schema_path (path to entry-schema.json), bridge_schema_path (if applicable), master_index_path, "
        "generated_by ('pbcpb'), layers (from kb-architecture.md), bridge_eligible_layers (from population-strategy.md), "
        "default_backend (from population-strategy.md).\n\n"
        "Verify: registry entry has all required fields and paths resolve to existing files/directories."
    ),
    "output": "Updated ~/.claude/kb-registry.json"
}
items.insert(1, register_task)

# Update Phase 3 objective to include registration and web harvesting
phase3['compilation']['objective'] = (
    "Create the KB directory tree, register KB in system registry, seed all placeholder entries, "
    "harvest initial content from research documents, generate search terms for web harvesting, "
    "and fill remaining gaps via web harvesting"
)

# Add kb-access-spec.md to context_load and context_budget
phase3['compilation']['context_load'].append("kb-access-spec.md")
phase3['compilation']['context_budget']['priority']['kb-access-spec.md'] = 3

# Add new FM IDs to failure_modes_relevant (will be created in Task 9)
phase3['compilation']['failure_modes_relevant'].extend(["FM-030", "FM-031"])

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Inserted: "Register KB in system registry" at items[1]')
print('Updated: Phase 3 objective to include registration and web harvesting')
print('Added: kb-access-spec.md to context_load and priority')
print('Added: FM-030, FM-031 to failure_modes_relevant')
print(f'Phase 3 now has {len(items)} items (was 7, now 8)')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`
Expected: Phase 3 now has 8 items

- [ ] **Step 3: Verify item positions after insertion**

After inserting at position 1, the Phase 3 items are now:
```
[0] Create KB directory tree
[1] Register KB in system registry (NEW)
[2] Seed placeholder entries
[3] Seeding milestone gate
[4] Harvest initial content from research documents
[5] Create bridge entries
[6] Validate file paths and schema conformance
[7] Phase gate: KB bootstrapped and populated
```

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
items = data['checklists'][3]['items']
for i, item in enumerate(items):
    print(f'  [{i}] {item[\"title\"][:60]}')
print(f'Total: {len(items)} items')
comp = data['checklists'][3]['compilation']
print(f'FM relevant: {comp[\"failure_modes_relevant\"]}')
print(f'Context has kb-access-spec.md: {\"kb-access-spec.md\" in comp[\"context_load\"]}')
"`
Expected: 8 items in correct order, FM-030/FM-031 present, kb-access-spec.md in context

- [ ] **Step 4: Run validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3`
Expected: `PASS`

Note: `validate_semantic.py` may warn about FM-030/FM-031 not existing yet — that's expected. They get created in Task 9.

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase3): add Register KB task and update compilation

New task: 'Register KB in system registry' — creates/updates
~/.claude/kb-registry.json after directory creation.

Phase 3 objective expanded to include registration, search term
generation, and web harvesting. Context now includes kb-access-spec.md.
failure_modes_relevant includes FM-030, FM-031 (created later).

Spec §7.3.1

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 6: Update Phase 3 harvest task + add search terms + web harvest tasks (§7.3.2–7.3.4)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 3 (checklists[3])

After Task 5, Phase 3 items are:
```
[0] Create KB directory tree
[1] Register KB in system registry
[2] Seed placeholder entries
[3] Seeding milestone gate
[4] Harvest initial content from research documents  ← MODIFY
[5] Create bridge entries
[6] Validate file paths
[7] Phase gate
```

Three changes:
1. Append implementation-ready content requirements to items[4] (research harvest) — §7.3.2
2. Insert "Generate search terms for web harvesting" after items[4] — §7.3.3
3. Insert "Web harvest to fill KB gaps" after the search terms task — §7.3.4

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase3 = data['checklists'][3]
items = phase3['items']

# §7.3.2: Modify items[4] — Harvest initial content from research documents
# Append implementation-ready content requirements
items[4]['description'] += (
    "\n\nHarvested entries MUST include:\n"
    "- original_markdown: the full text of the source section being harvested\n"
    "- code_blocks with working implementation examples (not pseudocode) where "
    "the domain involves code. An entry without code_blocks in a code-oriented "
    "KB layer is incomplete — search the research documents for code examples "
    "related to each topic.\n"
    "- Parameter ranges with specific values in description field\n"
    "- source.backend set to \"research-docs\"\n\n"
    "After harvesting, count remaining placeholder entries per layer. These "
    "placeholders are candidates for web harvesting. Record the count in "
    "harvest-status.json (track research_harvested, web_harvested, imported, "
    "placeholder, and failed per layer)."
)

# §7.3.3: Insert "Generate search terms" after items[4]
search_terms_task = {
    "title": "[Researcher] — Generate search terms for web harvesting",
    "owner": "[Researcher]",
    "description": (
        "For each placeholder entry remaining after research document harvesting, "
        "generate diverse search terms to maximize discovery of high-quality, "
        "implementation-ready content.\n\n"
        "For each placeholder, generate queries from at least 4 perspectives:\n"
        "- Academic: '[topic] algorithm', '[topic] computational method', '[topic] IEEE/AES paper'\n"
        "- Practitioner: '[topic] implementation C++', '[topic] best practices', '[topic] production code'\n"
        "- Educator: '[topic] tutorial advanced', '[topic] explained in depth'\n"
        "- Domain expert: domain-specific terminology variants, alternative names for the same concept\n\n"
        "Include code-targeting queries:\n"
        "- '[topic] C++ source code example'\n"
        "- '[topic] JUCE tutorial code'\n"
        "- '[topic] site:github.com implementation'\n\n"
        "Prioritize queries likely to surface content with actual code examples, "
        "parameter ranges, and implementation detail — not just conceptual descriptions.\n\n"
        "Output: search-terms.json mapping each placeholder entry ID to its generated "
        "search queries with perspective labels. Store alongside KB directory for reuse "
        "by kb-harvest skill."
    ),
    "output": "search-terms.json"
}
items.insert(5, search_terms_task)

# §7.3.4: Insert "Web harvest to fill KB gaps" after search terms (now items[6])
web_harvest_task = {
    "title": "[Researcher] — Web harvest to fill KB gaps",
    "owner": "[Researcher]",
    "description": (
        "Using search-terms.json and web harvesting backends from population-strategy.md, "
        "fill remaining placeholder entries.\n\n"
        "User selects backend. If no preference, use default from population-strategy.md. "
        "Invoke kb-harvest skill:\n"
        "  kb-harvest --kb <this-kb-name> --auto --backend <backend>\n\n"
        "The skill handles: search, fetch via WebFetch, extraction via Claude, quality gating, "
        "entry writing, and cascade updates (manifest, master-index, cross-references, bridge detection).\n\n"
        "If DDG backend fails (bot detection, 0 results), skill auto-falls back to websearch+webfetch.\n\n"
        "If kb-harvest skill is unavailable, fall back to manual process:\n"
        "1. Use WebSearch to find URLs for each topic\n"
        "2. Use WebFetch to retrieve content from each URL\n"
        "3. Extract structured fields manually in this session\n"
        "4. Write entries following entry-schema.json\n"
        "5. Update manifests and master-index manually\n\n"
        "EXECUTION: Work one KB layer at a time. Update manifest after each layer.\n\n"
        "CRITICAL: Preserve all code blocks with exact original formatting. "
        "C++ code examples must remain valid."
    ),
    "output": "kb/ web-harvested entries, updated manifests"
}
items.insert(6, web_harvest_task)

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Modified: items[4] (research harvest) — added implementation-ready content requirements')
print('Inserted: "Generate search terms for web harvesting" at items[5]')
print('Inserted: "Web harvest to fill KB gaps" at items[6]')
print(f'Phase 3 now has {len(items)} items')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`
Expected: Phase 3 now has 10 items

- [ ] **Step 3: Verify item positions**

After insertions, Phase 3 items should be:
```
[0] Create KB directory tree
[1] Register KB in system registry
[2] Seed placeholder entries
[3] Seeding milestone gate
[4] Harvest initial content (MODIFIED)
[5] Generate search terms (NEW)
[6] Web harvest to fill KB gaps (NEW)
[7] Create bridge entries
[8] Validate file paths
[9] Phase gate
```

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
items = data['checklists'][3]['items']
for i, item in enumerate(items):
    print(f'  [{i}] {item[\"title\"][:65]}')
print(f'Total: {len(items)} items')
print(f'items[4] has original_markdown: {\"original_markdown\" in items[4][\"description\"]}')
print(f'items[5] has search-terms.json: {\"search-terms.json\" in items[5][\"description\"]}')
print(f'items[6] has kb-harvest: {\"kb-harvest\" in items[6][\"description\"]}')
"`
Expected: 10 items in correct order, all 3 content checks PASS

- [ ] **Step 4: Run validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3`
Expected: `PASS`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase3): add search terms and web harvest tasks

Modified: research harvest task — requires original_markdown, code_blocks,
source.backend, and harvest-status.json tracking.

New task: 'Generate search terms for web harvesting' — produces
search-terms.json with queries from 4 perspectives + code-targeting.

New task: 'Web harvest to fill KB gaps' — invokes kb-harvest skill
to fill remaining placeholders from web sources.

Spec §7.3.2–§7.3.4

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 7: Update Phase 3 auditor + gate + handoff + success_criteria (§7.3.5–7.3.7)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 3 (checklists[3])

After Task 6, Phase 3 items are:
```
[0]-[6]: ... (tasks)
[7] Create bridge entries
[8] Validate file paths and schema conformance  ← MODIFY description
[9] Phase gate  ← MODIFY gate_conditions, handoff
```

Four changes:
1. Append 4 new validation checks to auditor (items[8]) — §7.3.5
2. Add 8 new gate conditions + modify 1 existing (items[9]) — §7.3.6
3. Add 3 output artifacts to handoff (items[9]) — §7.3.7
4. Add success_criteria for registration, search terms, web harvesting

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase3 = data['checklists'][3]
items = phase3['items']

# Verify we're at the right position
assert '[Auditor]' in items[8]['title'], f'Expected auditor at [8], got: {items[8]["title"]}'
assert 'Phase gate' in items[9]['title'], f'Expected gate at [9], got: {items[9]["title"]}'

# §7.3.5: Append 4 new validation checks to auditor (items[8])
items[8]['description'] += (
    "\n\nAdditional validation for web-harvested entries:\n"
    "5. harvest_metadata presence: every non-research-doc entry (source.backend != \"research-docs\", "
    "i.e. web-harvested and imported entries) must have harvest_metadata with overall_confidence, "
    "field_provenance, and extraction_prompt_version.\n"
    "6. Code block integrity: for entries with code_blocks[], verify code blocks also appear in "
    "original_markdown (if present). Any code_block not found in original_markdown may indicate "
    "extraction error.\n"
    "7. Manifest accuracy: for each manifest.json, count actual .json files in directory (excluding "
    "manifest.json itself) and compare to sum of entry counts in manifest. Report any mismatch. "
    "Automated check:\n"
    "  for manifest in kb/*/manifest.json; do\n"
    "    dir=$(dirname \"$manifest\")\n"
    "    actual=$(find \"$dir\" -name \"*.json\" ! -name \"manifest.json\" -maxdepth 2 | wc -l)\n"
    "    # Compare against manifest entries\n"
    "  done\n"
    "8. Master-index consistency: verify master-index knowledge_bases entry counts match actual "
    "per-KB file counts. Verify cross_references map to KBs that actually contain relevant entries."
)

# §7.3.6: Modify gate conditions (items[9])
gate = items[9]

# Modify existing condition
old_condition = "All harvestable research content has been captured in KB entries — zero skipped harvestable items"
new_condition = (
    "All harvestable content captured from research documents — zero skipped items. "
    "Web harvest attempted for all remaining placeholders. Entries that could not be "
    "filled documented in harvest-status.json with reason."
)
for i, cond in enumerate(gate['gate_conditions']):
    if cond == old_condition:
        gate['gate_conditions'][i] = new_condition
        break

# Add 8 new gate conditions
new_gate_conditions = [
    "KB registered in ~/.claude/kb-registry.json with valid path, schema, and layer references",
    "Web-harvested entries include harvest_metadata with overall_confidence >= backend quality_floor",
    "Entries with confidence near threshold have review_flag = true",
    "No entry accepted with overall_confidence below 0.30",
    "search-terms.json exists with queries for all web-harvested entries",
    "Per-KB manifest.json entry counts match actual file counts on disk (zero mismatches)",
    "Master-index cross_references populated for all concepts appearing in 2+ KB layers",
    "All code_blocks preserve original formatting (no reformatting or simplification detected)"
]
gate['gate_conditions'].extend(new_gate_conditions)

# §7.3.7: Add output artifacts to handoff
gate['handoff']['output_artifacts'].extend([
    "search-terms.json",
    "harvest-status.json",
    "~/.claude/kb-registry.json (updated)"
])

# Update success_criteria
phase3['compilation']['success_criteria'].extend([
    "KB registered in ~/.claude/kb-registry.json with valid entry",
    "search-terms.json generated for placeholder entries",
    "Web harvest attempted for remaining placeholders with results documented in harvest-status.json"
])

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Modified: items[8] auditor — added 4 web-harvest validation checks')
print(f'Modified: items[9] gate — {len(gate["gate_conditions"])} conditions total')
print(f'Modified: items[9] handoff — {len(gate["handoff"]["output_artifacts"])} output artifacts')
print(f'Modified: success_criteria — {len(phase3["compilation"]["success_criteria"])} total')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`

- [ ] **Step 3: Verify changes**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
p3 = data['checklists'][3]
gate = p3['items'][9]
print(f'Auditor has harvest_metadata check: {\"harvest_metadata presence\" in p3[\"items\"][8][\"description\"]}')
print(f'Gate conditions: {len(gate[\"gate_conditions\"])} (expected 18)')
print(f'Old condition replaced: {\"Web harvest attempted\" in str(gate[\"gate_conditions\"])}')
print(f'Output artifacts: {len(gate[\"handoff\"][\"output_artifacts\"])} (expected 7)')
print(f'Success criteria: {len(p3[\"compilation\"][\"success_criteria\"])} (expected 8)')
"`
Expected: all checks pass, 18 gate conditions, 7 artifacts, 8 success criteria

- [ ] **Step 4: Run validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3`
Expected: `PASS`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase3): update auditor, gate conditions, and handoff for web harvesting

Auditor gains 4 checks: harvest_metadata presence, code block integrity,
manifest accuracy (automated file count), master-index consistency.

Gate gains 8 conditions enforcing: registry, confidence thresholds,
review flags, search-terms.json, manifest accuracy, cross-references,
code preservation. Existing condition updated to require web harvest.

Handoff adds: search-terms.json, harvest-status.json, kb-registry.json.

Spec §7.3.5–§7.3.7

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 8: Update Phase 7 task descriptions (§7.4)

**Files:**
- Modify: `playbook-creator-playbook.json` — Phase 7 (checklists[7])

Two changes:
1. Update items[3] (Define phase-to-KB-layer mapping) — add bridge layer and web-harvested layer guidance
2. Update items[4] (Define skill activation mapping) — add kb-harvest, kb-sync, kb-validate

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

phase7 = data['checklists'][7]
items = phase7['items']

# Verify we're at the right tasks
assert 'phase-to-KB-layer mapping' in items[3]['title'], f'Expected KB mapping at [3], got: {items[3]["title"]}'
assert 'skill activation' in items[4]['title'], f'Expected skill activation at [4], got: {items[4]["title"]}'

# §7.4: Update phase_kb_mapping task (items[3])
items[3]['description'] += (
    "\n\nThe phase_kb_mapping must include:\n"
    "- Bridge layer mapped to phases that use creative-to-technical translation "
    "(typically DSP implementation, DAW testing, plugin spec)\n"
    "- All KB layers mapped to their consuming phases\n"
    "- Web-harvested KB layers included in mapping (they follow the same schema as "
    "research-harvested layers)"
)

# §7.4: Update skill_activation task (items[4])
items[4]['description'] += (
    "\n\nThe skill_activation must include these KB-related skills:\n"
    "- \"kb-harvest\": \"Phase 3 web harvesting and ongoing KB population\"\n"
    "- \"kb-sync\": \"Post-harvest verification and curated->synced status promotion\"\n"
    "- \"kb-validate\": \"Post-harvest claim validation and pre-implementation confidence gate\"\n\n"
    "These skills are always relevant for playbooks with structured KBs. Map them to "
    "the phase where they are first used (typically Phase 3 for kb-harvest, post-Phase 3 "
    "for kb-sync and kb-validate)."
)

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print('Updated: items[3] (phase_kb_mapping) — added bridge layer and web-harvested layer guidance')
print('Updated: items[4] (skill_activation) — added kb-harvest, kb-sync, kb-validate requirements')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`

- [ ] **Step 3: Verify changes**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
items = data['checklists'][7]['items']
print(f'KB mapping has bridge: {\"Bridge layer\" in items[3][\"description\"]}')
print(f'Skill activation has kb-harvest: {\"kb-harvest\" in items[4][\"description\"]}')
print(f'Skill activation has kb-sync: {\"kb-sync\" in items[4][\"description\"]}')
print(f'Skill activation has kb-validate: {\"kb-validate\" in items[4][\"description\"]}')
"`
Expected: all 4 checks True

- [ ] **Step 4: Run validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3`
Expected: `PASS`

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(phase7): add KB skill activation and bridge layer mapping guidance

phase_kb_mapping task: bridge layer must be mapped to phases using
creative-to-technical translation, web-harvested layers included.

skill_activation task: must include kb-harvest (Phase 3), kb-sync
(post-harvest verification), kb-validate (confidence gate).

Spec §7.4

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 9: Add failure modes FM-030, FM-031 (§7.3.8)

**Files:**
- Modify: `playbook-creator-playbook.json` — `failure_modes` array (line ~2847)

Two new failure modes. Note: spec labels these FM-020 and FM-021, but those IDs are already taken (FM-001 through FM-029 exist). Using FM-030 and FM-031.

- [ ] **Step 1: Write the modification script**

```python
import json

pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'
with open(pb_path) as f:
    data = json.load(f)

fms = data['failure_modes']

# Verify no FM-030 or FM-031 already exist
existing_ids = {fm['id'] for fm in fms}
assert 'FM-030' not in existing_ids, 'FM-030 already exists'
assert 'FM-031' not in existing_ids, 'FM-031 already exists'

# §7.3.8 (renumbered): FM-030 — Synthesis hallucination
fm_030 = {
    "id": "FM-030",
    "symptom": "Web-harvested KB entries contain plausible but inaccurate synthesized content that passes quality thresholds",
    "root_cause": "AI synthesis from web content produces confident-sounding but incorrect field values, especially domain_relevance and difficulty assessments. Single-source entries are most vulnerable.",
    "fix": "Human review of entries with review_flag=true. Cross-reference synthesized claims against multiple sources. Lower confidence scores for single-source entries.",
    "prevention": "Track field_provenance per entry — fields with method 'ai-inferred' at low confidence get flagged. Include extraction_prompt_version in harvest_metadata for reproducibility. Bridge auto-detection starts at 0.50 confidence, not 0.85.",
    "phase": "Phase 3: KB Bootstrapping",
    "severity": "degraded",
    "source": "web-harvesting-design"
}

# §7.3.8 (renumbered): FM-031 — Broken manifests
fm_031 = {
    "id": "FM-031",
    "symptom": "PBCPB-generated manifests show zero entries despite KB files existing on disk",
    "root_cause": "Phase 3 bootstrapping creates entries but does not update manifests, or updates manifests before entries are written. The auditor gate does not run automated file-count verification.",
    "fix": "Run kb-harvest --rebuild-manifest to reconstruct manifests from actual files. Or manually count files and update manifest entries.",
    "prevention": "Phase 3 auditor MUST run automated file-count verification: compare actual JSON file count per directory against manifest entry counts. Gate fails on any mismatch.",
    "phase": "Phase 3: KB Bootstrapping",
    "severity": "error",
    "source": "audit-finding-2026-04-04"
}

fms.append(fm_030)
fms.append(fm_031)

with open(pb_path, 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')

print(f'Added: FM-030 (synthesis hallucination) and FM-031 (broken manifests)')
print(f'Total failure modes: {len(fms)}')
```

- [ ] **Step 2: Run the modification script**

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`
Expected: Total failure modes: 31

- [ ] **Step 3: Verify FM entries**

Run: `cd /home/myuser/Documents/pbcpb && python3 -c "
import json
with open('playbook-creator-playbook.json') as f:
    data = json.load(f)
fms = data['failure_modes']
fm_ids = [fm['id'] for fm in fms]
print(f'Total FMs: {len(fms)}')
print(f'FM-030 exists: {\"FM-030\" in fm_ids}')
print(f'FM-031 exists: {\"FM-031\" in fm_ids}')
print(f'FM-030 phase: {next(fm[\"phase\"] for fm in fms if fm[\"id\"] == \"FM-030\")}')
print(f'FM-031 severity: {next(fm[\"severity\"] for fm in fms if fm[\"id\"] == \"FM-031\")}')
# Verify no duplicate IDs
print(f'No duplicate IDs: {len(fm_ids) == len(set(fm_ids))}')
"`
Expected: 31 FMs, both exist, correct phase/severity, no duplicates

- [ ] **Step 4: Run full validation (both validators)**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json 2>&1 | tail -3 && python3 scripts/validate_semantic.py playbook-creator-playbook.json 2>&1 | tail -3`

Expected for validate_playbook: `PASS`
Expected for validate_semantic: 1 error (pre-existing FM-028 issue only — NOT new errors for FM-030/FM-031)

- [ ] **Step 5: Commit**

```bash
cd /home/myuser/Documents/pbcpb
git add playbook-creator-playbook.json
git commit -m "feat(failure-modes): add FM-030 (synthesis hallucination) and FM-031 (broken manifests)

FM-030: web-harvested entries may contain plausible but inaccurate AI-
synthesized content. Prevention: field_provenance tracking, review_flag.

FM-031: manifests show zero entries despite files on disk. Prevention:
automated file-count verification in Phase 3 auditor gate.

Note: spec labels these FM-020/FM-021 but those IDs were taken (existing
FM-001 through FM-029). Renumbered to FM-030/FM-031.

Spec §7.3.8

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

### Task 10: Full validation + cross-check all 21 spec changes

**Files:**
- No modifications — verification only

Run all validators and a custom cross-check script that verifies every change from spec Section 12 is present in the modified files.

- [ ] **Step 1: Run structural + schema validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_playbook.py playbook-creator-playbook.json --schema templates/output-schema.json`
Expected: `PASS` (0 errors)

- [ ] **Step 2: Run semantic validation**

Run: `cd /home/myuser/Documents/pbcpb && python3 scripts/validate_semantic.py playbook-creator-playbook.json`
Expected: 1 error (pre-existing FM-028 only)

- [ ] **Step 3: Run test suite**

Run: `cd /home/myuser/Documents/pbcpb && python3 -m pytest scripts/ -q`
Expected: `64 passed`

- [ ] **Step 4: Run cross-check script for all 21 spec changes**

```python
import json

schema_path = '/home/myuser/Documents/pbcpb/templates/output-schema.json'
pb_path = '/home/myuser/Documents/pbcpb/playbook-creator-playbook.json'

with open(schema_path) as f:
    schema = json.load(f)
with open(pb_path) as f:
    data = json.load(f)

results = []

def check(num, desc, condition):
    status = "PASS" if condition else "FAIL"
    results.append((num, status, desc))
    return condition

kb_schema = schema['properties']['knowledge_base']['properties']
phase2 = data['checklists'][2]
phase3 = data['checklists'][3]
phase7 = data['checklists'][7]

# Schema changes
check(1, "population_strategy has web_backends", 
    'web_backends' in kb_schema['population_strategy']['properties']['harvesting_sources']['oneOf'][1]['properties'])
check(2, "harvesting_sources uses oneOf (backward compat)",
    'oneOf' in kb_schema['population_strategy']['properties']['harvesting_sources'])
check(3, "kb_registry exists in knowledge_base",
    'kb_registry' in kb_schema)

# Phase 2 task description updates
check(4, "Phase 2 Task 2: granularity guidance",
    'granular layers' in phase2['items'][1]['description'])
check(5, "Phase 2 Task 3: source.backend, original_markdown, harvest_metadata",
    all(x in phase2['items'][2]['description'] for x in ['source.backend', 'original_markdown', 'harvest_metadata']))
check(6, "Phase 2 Task 4: bridge provenance",
    'auto-detected' in phase2['items'][3]['description'])
check(7, "Phase 2 Task 5: web backends, weights, rankings, skill refs",
    all(x in phase2['items'][4]['description'] for x in ['ddg+webfetch', 'confidence_weights', 'source_domain_rankings', 'kb-harvest skill']))
check(8, "Phase 2 Task 6: master-index and manifest formats",
    all(x in phase2['items'][5]['description'] for x in ['concept-level cross-referencing', 'code_block_count']))

# Phase 2 structural changes
check(9, "Phase 2 new task: KB access specification",
    phase2['items'][6]['title'] == '[Architect] — Define KB access specification')
check(10, "Phase 2 gate: new conditions present",
    any('source.backend' in c for c in phase2['items'][7]['gate_conditions']))

# Phase 3 structural changes
check(11, "Phase 3 new task: Register KB",
    phase3['items'][1]['title'] == '[Builder] — Register KB in system registry')
check(12, "Phase 3 research harvest: implementation-ready requirements",
    'original_markdown' in phase3['items'][4]['description'] and 'source.backend' in phase3['items'][4]['description'])
check(13, "Phase 3 new task: Generate search terms",
    'search terms' in phase3['items'][5]['title'].lower())
check(14, "Phase 3 new task: Web harvest",
    'Web harvest' in phase3['items'][6]['title'])
check(15, "Phase 3 auditor: manifest accuracy + code integrity",
    all(x in phase3['items'][8]['description'] for x in ['harvest_metadata presence', 'Manifest accuracy']))

# Phase 3 gate changes
p3_gate = phase3['items'][9]
check(16, "Phase 3 gate: new conditions present",
    any('quality_floor' in c for c in p3_gate['gate_conditions']))
check(17, "Phase 3 handoff: search-terms.json and harvest-status.json",
    'search-terms.json' in p3_gate['handoff']['output_artifacts'] and 
    'harvest-status.json' in p3_gate['handoff']['output_artifacts'])

# Phase 7 changes
check(18, "Phase 7: phase_kb_mapping includes bridge, skill_activation includes kb-harvest",
    'Bridge layer' in phase7['items'][3]['description'] and 'kb-harvest' in phase7['items'][4]['description'])

# Failure modes
fm_ids = {fm['id'] for fm in data['failure_modes']}
check(19, "FM-030 and FM-031 exist",
    'FM-030' in fm_ids and 'FM-031' in fm_ids)

# Import adapters (part of change #1/#20)
check(20, "population_strategy has import_adapters",
    'import_adapters' in kb_schema['population_strategy']['properties'])

# Change #21 is kb-harvest skill --import mode — that's Plan B, not Plan A
# Verify it's correctly excluded
check(21, "Change #21 (kb-harvest --import) is Plan B scope — no PBCPB change needed",
    True)  # Always passes — this is just documentation

# Summary
print("\n=== CROSS-CHECK: Spec Section 12 — All 21 Traced Changes ===\n")
all_pass = True
for num, status, desc in results:
    marker = "✓" if status == "PASS" else "✗"
    print(f"  {marker} #{num:2d} [{status}] {desc}")
    if status == "FAIL":
        all_pass = False

print(f"\n{'ALL CHECKS PASSED' if all_pass else 'SOME CHECKS FAILED'}: {sum(1 for _, s, _ in results if s == 'PASS')}/{len(results)}")
```

Save the Step 1 code block as a temporary `.py` file, then run it:
`cd /home/myuser/Documents/pbcpb && python3 /tmp/task_step.py`
Expected: `ALL CHECKS PASSED: 21/21`

- [ ] **Step 5: If any checks fail, fix and re-run**

If any cross-check fails, return to the relevant task and apply the missing change. Re-run the cross-check script until 21/21 pass.

- [ ] **Step 6: Final commit (if fixes were needed)**

```bash
cd /home/myuser/Documents/pbcpb
git add templates/output-schema.json playbook-creator-playbook.json
git commit -m "fix(pbcpb): cross-check fixes for spec compliance

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

Only commit if Step 5 required fixes. If 21/21 passed on first run, skip this step.

---

## Summary of Changes

| Spec Section | PBCPB File | What Changed |
|---|---|---|
| §7.1.1 | output-schema.json | `population_strategy` expanded: web_backends, confidence_weights, source_domain_rankings, bridge_eligible_layers, import_adapters, skill_references, search_term_strategy, oneOf backward compat |
| §7.1.3 | output-schema.json | `kb_registry` added to `knowledge_base` |
| §7.2.1 | playbook.json Phase 2 | Task 2: granularity guidance for layers |
| §7.2.2 | playbook.json Phase 2 | Task 3: source provenance, original_markdown, harvest_metadata, code preservation |
| §7.2.3 | playbook.json Phase 2 | Task 4: bridge provenance tracking |
| §7.2.4 | playbook.json Phase 2 | Task 5: web backends, weights, rankings, search terms, import adapters, skill refs |
| §7.2.5 | playbook.json Phase 2 | Task 6: master-index and manifest format specifications |
| §7.2.6 | playbook.json Phase 2 | New task: Define KB access specification |
| §7.2.7 | playbook.json Phase 2 | Gate: 9 new conditions |
| §7.3.1 | playbook.json Phase 3 | New task: Register KB in system registry |
| §7.3.2 | playbook.json Phase 3 | Task: Research harvest — implementation-ready content requirements |
| §7.3.3 | playbook.json Phase 3 | New task: Generate search terms for web harvesting |
| §7.3.4 | playbook.json Phase 3 | New task: Web harvest to fill KB gaps |
| §7.3.5 | playbook.json Phase 3 | Auditor: 4 new validation checks |
| §7.3.6 | playbook.json Phase 3 | Gate: 8 new conditions + 1 modified |
| §7.3.7 | playbook.json Phase 3 | Handoff: 3 new output artifacts |
| §7.4 | playbook.json Phase 7 | KB mapping: bridge layer guidance; Skill activation: kb-harvest, kb-sync, kb-validate |
| §7.3.8 | playbook.json failure_modes | FM-030 (synthesis hallucination), FM-031 (broken manifests) |
| (fix) | output-schema.json | kb_status types accept null |
| (update) | playbook.json Phase 2 | 3 new success_criteria |
| (update) | playbook.json Phase 3 | Objective, success_criteria, failure_modes_relevant, context_load updated |
