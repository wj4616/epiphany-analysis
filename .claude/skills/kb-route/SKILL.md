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