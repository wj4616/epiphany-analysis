<?xml version="1.0" encoding="UTF-8"?>
<!-- source: prompt-cog | audit: v2 — 9 issues resolved -->
<prompt>
  <meta source="prompt-cog"/>

  <role>
You are a Graph-of-Thought prompt architecture specialist with deep expertise in
cognitive AI systems, Claude Code skill pipeline design, and the prompt-cog/prompt-epiphany
methodology. You design new Claude Code skills by decomposing enhancement tasks into
typed graph nodes connected by directed edges, executing subsets in optimal order per
mode, and synthesizing a verified SKILL.md that preserves all behavioral detail of the
baseline skill while adding genuine graph topology value.

Your design sensibility prizes utility: a graph that runs fast on simple inputs and
scales depth on complex ones. You treat "ultimate prompt enhancement system" and "best
methodology" as measurable targets, not aspirations — each must be justified by
observable improvement on the 6-dimension SKILL.md quality rubric. You bring "all
possible optimizations" only when they add real throughput; dead-weight engineering is
a defect, not a feature.
  </role>

  <context>
**What this prompt is**

This is a SKILL DESIGN prompt. Executing it produces a SKILL.md artifact defining the
prompt-graph skill. It is not the SKILL.md itself. Runtime behavior of the resulting
skill — how prompt-graph processes a user's input at execution time — is specified in
the Pipeline narrative section (Phase 8 output), not in the design phases here.

**Baseline skill lineage**

Key term anchoring:
- prompt-epiphany — original proven prompt enhancement design; the root ancestor
- prompt-cog — sequential 7-step pipeline skill that inherits from prompt-epiphany;
  read-only reference at `~/.claude/skills/prompt-cog/SKILL.md`
- prompt-graph — new skill being designed by executing this prompt
- GoT (Graph of Thought) — the cognitive methodology structuring prompt-graph's
  execution as a node/edge graph rather than a flat sequence
- GoT controller — inline orchestration decision point selecting which nodes to
  activate and in what order based on input complexity class; NOT a program loop
- node — named processing unit with typed inputs, a defined transformation, and
  typed outputs
- edge — directed channel between two nodes carrying structured data with a defined
  cardinality and optional activation condition
- mode activation matrix — table mapping skill invocation flags (--minimal, --quiet,
  normal) to the active node subset for each mode
- INVENTORY YAML — the structured 8-category extraction of key entities from a user's
  input prompt, produced during prompt-cog's Step 3 (Analysis) phase; schema:
  {urls, file_paths, tech_version, code_blocks, named_entities, key_constraints,
  tone_markers, structural_elements}; used as a precision targeting mechanism for
  KB queries and synthesis contracts


  ## VOCABULARY — use these terms exactly, no synonyms

  - **Node**: A pipeline processing unit. Has a stage ID, hat, module file, input ports, output
   ports, and scale gates.
  - **Module file**: The `.md` file under `modules/` that defines a node's PROTOCOL. One module
   file per node.
  - **Edge**: A directed data connection from one node's output file to another node's input
  port. Declared in the PIPELINE GRAPH.
  - **Port**: A named attachment point on a node. INPUT ports consume files; OUTPUT ports
  produce files.
  - **Wave**: A set of nodes that execute in parallel before the next sequential step.

  - **Conditional edge**: An edge that only activates when a declared signal or flag condition
  is true.
  - **Join semantics**: How a node with multiple incoming edges decides when to fire (AND =
  wait for all; XOR = exactly one fires).
**Embedded INVENTORY (from design-time analysis of this prompt's source)**

```yaml
inventory:
  urls: []
  file_paths:
    - "~/.claude/skills/prompt-cog/SKILL.md"
  tech_version: []
  code_blocks: []
  named_entities:
    - "prompt-cog"
    - "prompt-graph"
    - "prompt-epiphany"
    - "GoT (Graph of Thought)"
    - "GoT controller"
    - "MCP"
    - "cognitive KB"
    - "thought KB"
    - "SKILL.md"
    - "Node registry"
    - "edge/channel table"
    - "mode activation matrix"
    - "parallel groups"
    - "optimization strategies"
  key_constraints:
    - "do not touch existing prompt-cog skill"
    - "new skill named prompt-graph"
    - "Node registry, edge/channel table, mode activation matrix, parallel groups,
       optimization strategies will be provided separately"
    - "base design on prompt-cog in detail, preserve small details"
    - "implement GoT controller mechanism"
    - "must not run so long human user gets frustrated on simple prompt"
    - "value utility to human"
    - "modular programatticly run design"
  tone_markers:
    - "ultimate prompt enhancement system"
    - "best methodology"
    - "all possible optimizations"
  structural_elements:
    - "tables (nodes, edges described in tables)"
    - "modular design"
    - "parallel groups"
```

**KB access pattern (runtime behavior of the produced skill)**

The following describes how prompt-graph will use KBs at runtime — when executing on a
user's input — not during the SKILL.md design task performed by this prompt:

- cognitive KB (dify-cognitive-kb): queried once during the cognitive-trait-mapping
  node to identify which of the 19 Tier-1 genius traits apply to the user's input
  domain; result flows into the synthesis node as a trait overlay. Query input is
  derived from the user's INVENTORY YAML named_entities and tone_markers, not the
  raw input text.
- thought KB (dify-thought-kb): queried once during the GoT path evaluation node to
  select the optimal reasoning topology (CoT, ToT, or GoT) for the input's complexity
  class; result sets the active graph traversal strategy. Query input is derived from
  the user's INVENTORY YAML key_constraints and structural_elements.



**Audience calibration**

The output SKILL.md is consumed by a Claude Code orchestrator executing a skill. It
must use exact structural markers, YAML frontmatter keys, and section headers that
Claude Code's skill loader expects. Structural ambiguity causes invocation failure.

**Preliminary Technical Structure**

---

## Component A — Node Registry

| ID  | Node Name                 | Role                                                                                                                                                       | Inputs (normal mode)                                                         | Outputs                                              | Mode Gates               |
|-----|---------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------|------------------------------------------------------|--------------------------|
| N01 | InputRouter               | Normalizes input, detects type A/B/C/D, strips flags, routes to sufficiency gate                                                                           | Raw invocation string                                                        | Normalized input, type flag, mode flags              | all                      |
| N02 | SufficiencyGate           | Hard gate — blocks pipeline if input has no identifiable task, concept, or goal                                                                            | Normalized input, mode flags                                                 | PASS signal + normalized input                       | all                      |
| N03 | IntentExtractor           | Extracts goal, desired end state, success criteria                                                                                                         | Normalized input                                                             | INTENT block                                         | normal, verbose, minimal |
| N04 | InventoryCollector        | Catalogs all preservation-critical items into typed YAML schema                                                                                            | Normalized input                                                             | INVENTORY (20-key YAML)                              | all                      |
| N05 | StructureAnalyzer         | Identifies current organization, missing structural elements (role, format, constraints)                                                                   | Normalized input                                                             | STRUCTURE block                                      | normal, verbose          |
| N06 | ConstraintAuditor         | Surfaces explicit and implicit constraints; detects conflicts                                                                                              | Normalized input                                                             | CONSTRAINTS block                                    | normal, verbose          |
| N07 | TechniqueGapAnalyst       | Evaluates T1–T13: already present / needed / impact                                                                                                       | Normalized input, INTENT                                                     | TECHNIQUES block                                     | normal, verbose          |
| N08 | WeaknessDetector          | Identifies vagueness, misinterpretation risk, contradictions — emits numbered entries (W1, W2...) each with impact score (high/medium/low) and causal explanation | Normalized input, INTENT, STRUCTURE, CONSTRAINTS                        | WEAKNESSES block (numbered, scored + causal)         | normal, verbose          |
| N09 | PrimaryContractGen        | Generates enhancement contracts from analysis; allocates more contracts to high-impact weaknesses. Minimal: checks technique presence against raw text, derives gaps from INTENT, applies T1–T13 ceiling directly | Normal: INTENT, STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES — Minimal: normalized input, INTENT | Contract list (primary) | normal, verbose, minimal |
| N10 | AntiConformityPass        | Contrarian re-read of primary contracts; novelty gate filters any contract a sequential T1–T13 pass would have generated; appends survivors to primary list | Normalized input, primary contract list                                      | Combined contract list (primary + anti-conformity additions) | normal, verbose   |
| N11 | ContractConflictResolver  | Groups contracts by (technique, target_section); detects incompatible actions on same slot; logs INTERNAL conflicts; merges INPUT-DIRECTIVE conflicts       | Combined contract list (normal/verbose) or primary contract list (minimal), input directives | Resolved contract list, conflict log         | normal, verbose, minimal |
| N12 | CoherenceGate             | Verifies every high-impact weakness has a mapped contract using a technique relevant to that weakness's causal explanation — not just presence; non-blocking | WEAKNESSES block, resolved contract list                                    | advisory \| none                                     | normal, verbose          |
| N13 | SynthesisAgent            | Applies resolved contracts in priority order; places INVENTORY items first per placement mapping; builds output XML with meta source tag                   | Normalized input, INVENTORY, resolved contracts, conflict log, coherence advisory (normal/verbose, from N12) | Draft XML                                            | normal, verbose, minimal |
| N14 | PreservationVerifier      | Checks every INVENTORY item appears verbatim in draft XML, in semantically correct section (checks 6a–6e)                                                  | INVENTORY, draft XML                                                         | Verification report (6a–6e)                          | all                      |
| N15 | SemanticFidelityChecker   | Confirms INTENT matches draft XML — same objective, same success criteria (check 6f)                                                                       | INTENT, draft XML                                                            | Verification result (6f)                             | normal, verbose, minimal |
| N16 | QualityGate               | Runs checks 6g–6l: technical integrity, enhancement validation (6h traces additions to analysis findings), production readiness, no fabrication, rationale accuracy, value added. In minimal mode, check 6h runs on INTENT + INVENTORY only (N05–N08 not active) | Draft XML, analysis blocks (N05–N08 outputs, normal/verbose only) | Verification results (6g–6l)           | all                      |
| N17 | RepairRouter              | Aggregates verification reports from N14, N16, and N15 (normal/verbose/minimal only — N15 absent in spec/plan); decides repair vs. output; retains last draft XML for fallback on repair_count=2; in verbose mode, retains first-pass verified XML as internal state (first_pass_xml) when routing to N20 via E22, used for Wave 9 FAIL revert; logs failing check IDs | Verification reports from N14, N16 (+ N15 in normal/verbose/minimal), draft XML (fallback retention) | Repair signal or PASS-to-output         | all                      |
| N18 | OutputFormatter           | Wraps verified XML in delimiters; generates preservation/coverage summary line; appends recovery guidance on FAIL path                                     | Verified XML, INVENTORY counts, mode flags                                   | Final output string                                  | all                      |
| N19 | SaveHandler               | Generates DD-MM filename slug; checks collision; creates directory if needed; writes file                                                                  | Final output, mode flags, quiet flag                                         | Saved file path                                      | all                      |
| N20 | ExpansionNode             | Gap scan on first-pass verified output; identifies thin spots (sparse context, bare constraints, missing edge cases, weak reasoning guidance); generates targeted expansions | First-pass verified XML (from N17 PASS), INTENT, INVENTORY            | Expanded XML                                         | verbose only             |
| N21 | SpecDomainAnalyzer        | Analyzes domain, stakeholders, scope (included/excluded), existing constraints, success criteria                                                           | Normalized input                                                             | DOMAIN block                                         | spec                     |
| N22 | SpecRequirementsExtractor | Exhaustive decomposition + IEEE 29148 requirement extraction (SHALL/SHOULD/MAY) with domain gap detection                                                  | Normalized input, DOMAIN block                                               | DECOMPOSITION block, REQUIREMENTS block              | spec                     |
| N23 | SpecSynthesisNode         | Synthesizes specification XML from domain analysis and requirements; selects mode A/B/C based on input type; surfaces open questions                       | Normalized input, INVENTORY, DOMAIN, DECOMPOSITION, REQUIREMENTS            | Spec draft XML                                       | spec                     |
| N24 | PlanGoalAnalyzer          | Decomposes goal into sub-goals, prerequisites, constraints, risks; atomic action decomposition with dependency mapping                                     | Normalized input                                                             | GOAL_ANALYSIS block, ACTION_DECOMPOSITION block, DEPENDENCY_MAP | plan          |
| N25 | PlanSafeguardDesigner     | For every action and checkpoint: verification test, failure recovery, rollback procedure, escalation trigger                                               | ACTION_DECOMPOSITION, DEPENDENCY_MAP                                         | SAFEGUARDS block                                     | plan                     |
| N26 | PlanSynthesisNode         | Writes full plan from action decomposition, dependency map, safeguards; execution simulation; gap audit                                                    | Normalized input, INVENTORY, GOAL_ANALYSIS, ACTION_DECOMPOSITION, DEPENDENCY_MAP, SAFEGUARDS | Plan draft XML                    | plan                     |

---

## Component B — Edge / Channel Table

| Edge ID | From → To                        | Channel Name               | Data Type                                              | Condition                                            |
|---------|----------------------------------|----------------------------|--------------------------------------------------------|------------------------------------------------------|
| E00a    | N02 → N05                        | normalized_input           | string                                                 | normal \| verbose                                    |
| E00b    | N02 → N06                        | normalized_input           | string                                                 | normal \| verbose                                    |
| E00c    | N02 → N07                        | normalized_input           | string                                                 | normal \| verbose                                    |
| E00d    | N02 → N08                        | normalized_input           | string                                                 | normal \| verbose                                    |
| E00e    | N02 → N09                        | normalized_input           | string                                                 | minimal only (N09 needs raw text to check technique presence) |
| E01     | N01 → N02                        | raw_normalized_input       | string + flags                                         | always                                               |
| E02     | N02 → N03, N04                   | input_pass                 | string                                                 | sufficiency PASS only                                |
| E03     | N03 → N07, N08                   | intent_block               | text block                                             | normal \| verbose                                    |
| E04     | N03 → N09                        | intent_block               | text block                                             | normal \| verbose \| minimal                         |
| E05     | N04 → N13, N14, N23, N26         | inventory_yaml             | YAML (20-key schema)                                   | always (target nodes vary by mode)                   |
| E06     | N05 → N08, N09                   | structure_block            | text block                                             | normal \| verbose                                    |
| E07     | N06 → N08, N09                   | constraints_block          | text block                                             | normal \| verbose                                    |
| E08     | N07 → N09                        | techniques_block           | text block                                             | normal \| verbose                                    |
| E09     | N08 → N09                        | weaknesses_block           | numbered + scored + causal text block                  | normal \| verbose                                    |
| E10     | N09 → N10                        | primary_contracts          | contract list (v1 schema)                              | normal \| verbose                                    |
| E11     | N10 → N11                        | combined_contracts         | contract list (primary + anti-conformity)              | normal \| verbose                                    |
| E12     | N09 → N11                        | primary_contracts          | contract list (v1 schema)                              | minimal only (bypasses N10)                          |
| E13     | N08, N11 → N12                   | weakness_contract_pair     | paired data (weaknesses + resolved contracts)          | normal \| verbose                                    |
| E13b    | N12 → N13                        | coherence_advisory         | advisory text \| null                                  | normal \| verbose — advisory (not blocking); N13 uses this to prioritize synthesis effort on under-covered high-impact weaknesses |
| E14     | N11 → N13                        | resolved_contracts         | contract list + conflict log                           | normal \| verbose \| minimal                         |
| E15     | N13 → N14, N15, N16              | draft_xml                  | XML string                                             | normal \| verbose \| minimal                         |
| E15b    | N13 → N17                        | draft_xml_fallback         | XML string                                             | normal \| verbose \| minimal — retained by N17 for use only when repair_count=2 |
| E16     | N14 → N17                        | preservation_report        | verification results 6a–6e                             | always                                               |
| E17     | N15 → N17                        | fidelity_result            | verification result 6f                                 | normal \| verbose \| minimal                         |
| E18     | N16 → N17                        | quality_results            | verification results 6g–6l                             | always                                               |
| E19     | N17 → N13 \| N23 \| N26          | repair_signal              | mode-conditional payload (see Component H)             | FAIL, repair_count < 2; target is N13 (normal/verbose/minimal), N23 (spec), N26 (plan) |
| E20     | N17 → N18                        | verified_xml               | XML string                                             | PASS — normal/minimal; or second-pass PASS — verbose |
| E21     | N18 → N19                        | formatted_output           | string                                                 | quiet mode or user confirms save                     |
| E22     | N17 → N20                        | first_pass_verified_xml    | XML string                                             | first-pass PASS, verbose only                        |
| E23     | N20 → N14, N15, N16              | expanded_xml               | XML string                                             | verbose only (second verification pass)              |
| E30     | N02 → N21, N24                   | normalized_input           | string                                                 | spec (→ N21) \| plan (→ N24)                        |
| E31     | N04 → N24                        | inventory_yaml             | YAML (19-key schema)                                   | plan only (N24 may reference technical inventory for scope context) |
| E32     | N21 → N22                        | domain_block               | DOMAIN analysis block                                  | spec                                                 |
| E33     | N22 → N23                        | decomp_requirements        | DECOMPOSITION + REQUIREMENTS blocks                    | spec                                                 |
| E34     | N21 → N23                        | domain_block               | DOMAIN analysis block                                  | spec                                                 |
| E35     | N23 → N14, N16                   | spec_draft_xml             | XML string                                             | spec                                                 |
| E35b    | N23 → N17                        | draft_xml_fallback         | XML string                                             | spec — retained by N17 for use only when repair_count=2 |
| E36     | N24 → N25                        | goal_action_dependency     | GOAL_ANALYSIS + ACTION_DECOMPOSITION + DEPENDENCY_MAP  | plan                                                 |
| E37     | N25 → N26                        | safeguards_block           | SAFEGUARDS block                                       | plan                                                 |
| E38     | N24 → N26                        | goal_action_dependency     | GOAL_ANALYSIS + ACTION_DECOMPOSITION + DEPENDENCY_MAP  | plan                                                 |
| E39     | N26 → N14, N16                   | plan_draft_xml             | XML string                                             | plan                                                 |
| E39b    | N26 → N17                        | draft_xml_fallback         | XML string                                             | plan — retained by N17 for use only when repair_count=2 |
| E40a    | N02 → N10                        | normalized_input           | string                                                 | normal \| verbose (N10 contrarian re-read requires original text to detect already-present techniques) |
| E40b    | N02 → N13                        | normalized_input           | string                                                 | normal \| verbose \| minimal (synthesis node needs original text on first pass; repair also delivers it via E19 repair_signal) |
| E40c    | N02 → N23                        | normalized_input           | string                                                 | spec                                                 |
| E40d    | N02 → N26                        | normalized_input           | string                                                 | plan                                                 |
| E41     | N05, N06, N07, N08 → N16         | analysis_blocks            | STRUCTURE + CONSTRAINTS + TECHNIQUES + WEAKNESSES      | normal \| verbose (in minimal, N16 check 6h runs on INTENT + INVENTORY only) |

---

## Component C — Mode Activation Matrix

| Node | minimal | normal | verbose | spec | plan |
|------|:-------:|:------:|:-------:|:----:|:----:|
| N01 InputRouter               | ✓ | ✓ | ✓ | ✓ | ✓ |
| N02 SufficiencyGate           | ✓ | ✓ | ✓ | ✓ | ✓ |
| N03 IntentExtractor           | ✓ | ✓ | ✓ | – | – |
| N04 InventoryCollector        | ✓ | ✓ | ✓ | ✓ | ✓ |
| N05 StructureAnalyzer         | – | ✓ | ✓ | – | – |
| N06 ConstraintAuditor         | – | ✓ | ✓ | – | – |
| N07 TechniqueGapAnalyst       | – | ✓ | ✓ | – | – |
| N08 WeaknessDetector          | – | ✓ | ✓ | – | – |
| N09 PrimaryContractGen        | ✓ | ✓ | ✓ | – | – |
| N10 AntiConformityPass        | – | ✓ | ✓ | – | – |
| N11 ContractConflictResolver  | ✓ | ✓ | ✓ | – | – |
| N12 CoherenceGate             | – | ✓ | ✓ | – | – |
| N13 SynthesisAgent            | ✓ | ✓ | ✓ | – | – |
| N14 PreservationVerifier      | ✓ | ✓ | ✓ | ✓ | ✓ |
| N15 SemanticFidelityChecker   | ✓ | ✓ | ✓ | – | – |
| N16 QualityGate               | ✓ | ✓ | ✓ | ✓ | ✓ |
| N17 RepairRouter              | ✓ | ✓ | ✓ | ✓ | ✓ |
| N18 OutputFormatter           | ✓ | ✓ | ✓ | ✓ | ✓ |
| N19 SaveHandler               | ✓ | ✓ | ✓ | ✓ | ✓ |
| N20 ExpansionNode             | – | – | ✓ | – | – |
| N21 SpecDomainAnalyzer        | – | – | – | ✓ | – |
| N22 SpecRequirementsExtractor | – | – | – | ✓ | – |
| N23 SpecSynthesisNode         | – | – | – | ✓ | – |
| N24 PlanGoalAnalyzer          | – | – | – | – | ✓ |
| N25 PlanSafeguardDesigner     | – | – | – | – | ✓ |
| N26 PlanSynthesisNode         | – | – | – | – | ✓ |

**Notes:**
- N15 skipped in spec/plan: output is not an enhanced prompt; fidelity is covered by spec/plan-specific checks in N16. N17 aggregates N14 + N16 only in these modes.
- N04 runs in all modes: preservation is required across all output types.
- N09 in minimal: receives normalized input (via E00e) + INTENT (via E04); N05–N08 not active, so N09 checks technique presence directly against raw text and applies the T1/T2/T3/T5/T7 ceiling.
- N16 in minimal: check 6h runs on INTENT + INVENTORY only (analysis blocks from N05–N08 not available via E41).

---

## Component D — Parallel Execution Groups

Execution order resolved by topological sort on the edge graph. Nodes with no data dependency between them execute in the same wave.

### Normal Mode

```
Wave 0 — Sequential prerequisite:
  N01 → N02

Wave 1 — Parallel (both depend only on N02):
  N03 || N04

Wave 2a — Parallel (N05, N06 need raw input only; N07 needs N03 output):
  N05 || N06 || N07

Wave 2b — N08 alone (depends on N05 + N06 from Wave 2a):
  N08

Wave 3 — Sequential contract pipeline:
  N09 → N10 → N11

Wave 4 — Sequential (E13b advisory makes this intentional, not a topological accident):
  N12 (advisory, non-blocking)
  ↓ E13b (coherence_advisory)
  N13 (synthesis — receives advisory before constructing output)

Wave 5 — Parallel verification:
  N14 || N15 || N16

Wave 6:
  N17
    FAIL + repair_count < 2  → E19 → N13 (return to Wave 4)
    FAIL + repair_count = 2  → annotate retained draft → N18 → N19
    PASS                     → E20 → N18 → N19
```

### Verbose Mode

```
Waves 0–5: identical to normal mode (including Wave 4 E13b sequential advisory).

Wave 6 (first-pass routing):
  N17
    FAIL + repair_count < 2  → E19 → N13 (return to Wave 4)
    FAIL + repair_count = 2  → annotate retained draft → N18 → N19
    PASS                     → E22 → N20
                               [N17 retains first_pass_xml as internal state at this point]

Wave 7 — Expansion:
  N20 (reads first-pass verified XML from N17)

Wave 8 — Parallel expansion verification:
  N14 || N15 || N16

Wave 9 (second-pass routing):
  N17
    FAIL  → retrieve retained first_pass_xml; output with note:
            "Expansion verification failed — reverting to pre-expansion output"
            → N18 → N19
    PASS  → E20 → N18 → N19
```

### Minimal Mode Path

```
Wave 0:   N01 → N02

Wave 1 — Parallel:
  N03 || N04
  (Waves 2a, 2b skipped — N05–N08 not active)

Wave 3-minimal — Contract generation, no anti-conformity:
  N09 → N11   (N10 bypassed; N09 receives normalized input via E00e + INTENT via E04)

Wave 4:   N12 skipped. N13 (synthesis).
Wave 5:   N14 || N15 || N16
Wave 6:   N17 → [FAIL: repair/annotate] | [PASS: N18 → N19]
```

### Spec Mode Wave Plan

```
Wave 0:   N01 → N02

Wave 1 — Parallel (both depend only on N02):
  N04 || N21

Wave 2:   N22 (depends on N21 from Wave 1)

Wave 3:   N23 (depends on N21, N22 from Waves 1–2, and N04 from Wave 1)

Wave 4 — Parallel verification (N15 not active in spec mode):
  N14 || N16

Wave 5:
  N17 (aggregates N14 + N16 only)
    FAIL + repair_count < 2  → E19 → N23 (return to Wave 3)
    FAIL + repair_count = 2  → annotate retained draft → N18 → N19
    PASS                     → E20 → N18 → N19
```

### Plan Mode Wave Plan

```
Wave 0:   N01 → N02

Wave 1 — Parallel (both depend only on N02):
  N04 || N24

Wave 2:   N25 (depends on N24 from Wave 1)

Wave 3:   N26 (depends on N24, N25 from Waves 1–2, and N04 from Wave 1)

Wave 4 — Parallel verification (N15 not active in plan mode):
  N14 || N16

Wave 5:
  N17 (aggregates N14 + N16 only)
    FAIL + repair_count < 2  → E19 → N26 (return to Wave 3)
    FAIL + repair_count = 2  → annotate retained draft → N18 → N19
    PASS                     → E20 → N18 → N19
```

---

## Component E — Optimization Strategies

**O1 — Edge Pruning on Empty INVENTORY**
If N04 output has all INVENTORY lists empty: skip N14 checks 6a–6e entirely (E05 → N14 edge conditional on at least one non-empty list).
Rationale: preservation checks on an empty inventory are no-ops that consume verification budget with no value.

**O2 — Impact-Budget Contract Allocation**
N09 allocates contract generation budget proportionally to weakness impact score from N08 (scores are machine-readable from numbered entries W1, W2...):
  High-impact → 2–3 contracts minimum.
  Medium-impact → 1–2 contracts.
  Low-impact → 0–1 contracts (best-effort only).
Rationale: synthesis quality scales with contract specificity on high-leverage weaknesses.

**O3 — Novelty Gate on Anti-Conformity (N10)**
Each candidate anti-conformity contract must pass: "Would a sequential T1–T13 analyst running techniques in order against the analysis findings have generated this?"
If yes or borderline → discard. Contract survives only if a specific primary-pass exclusion reason can be articulated.
Rationale: without the gate, anti-conformity produces duplicate contracts with extra overhead.

**O4 — Same-Slot Conflict Pruning (N11)**
Group all contracts by (technique, target_section). On incompatible conflict: keep higher-priority contract, log the other as INTERNAL conflict. Merge INTERNAL and INPUT-DIRECTIVE conflicts into a single conflict log for N13.
Rationale: incompatible contracts targeting the same section produce incoherent synthesis output.

**O5 — Coherence Advisory Short-Circuit (N12)**
If N12 detects a high-impact weakness with no adequately mapped contract (presence-only mapping does not satisfy): emit advisory, do NOT block. Synthesis proceeds with reduced quality floor for that weakness.
Rationale: blocking on coherence failure is too aggressive — degraded output is more useful than a halt.

**O6 — Repair Loop Cap**
N17 maintains repair_count per run. If repair_count ≥ 2: route to N18 with annotated failure note using retained draft XML. Do NOT re-route to synthesis.
Rationale: prevents infinite synthesis loops; user receives best-effort output with specific diagnostic information.

**O7 — Token Budget Prioritization (synthesis node context assembly)**
Applies to N13, N23, and N26. If assembled content exceeds ~15,000 tokens, truncate in ascending priority order (1=lowest, truncate first):
  1. Analysis blocks (STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES) — trim to high-priority items only
  2. Contract list — high-priority contracts only
  3. INVENTORY YAML — never truncate
  4. Normalized input — never truncate (highest priority)
Rationale: INVENTORY and input are load-bearing for preservation guarantees; analysis blocks are informational.

**O8 — Verbose Thin-Spot Gating (N20)**
N20 only adds content where the first-pass verified output is measurably thin. Thinness = expanding this section would meaningfully improve effectiveness for the stated intent. Brevity alone is not thinness.
If no thin spots: return verified output unchanged with diagnostic note. Prevents verbose mode from padding already-sufficient output.

**O9 — Mode-Aware Technique Ceiling (N09, N10, N13)**
Minimal mode ceiling: T1, T2, T3, T5, T7 only.
Depth techniques (T4, T6, T8, T9, T10, T11, T12, T13) require full analysis context from N05–N08. These nodes do not run in minimal mode, so N09 must not generate contracts for depth techniques, N10 does not run (novelty detection requires full weakness + technique context), and N13 must not apply depth techniques when only minimal analysis is available.

---

## Component F — Contract Schema (Typed Arc Data)

Each contract is a typed arc carrying a specific transformation instruction from the analysis-ideation subgraph to the active synthesis node (N13, N23, or N26).

```
Fields:
  technique        : T1–T13 | "anti-conformity:[name]"
  target_section   : <role> | <context> | <task> | <constraints> |
                     <output_format> | <verification> | <edge_cases>
                     (valid for normal/minimal/verbose output XML only;
                      contracts are not used in spec/plan mode)
  action           : imperative string — what to add/change in target_section
  rationale        : why this contract improves the prompt
                     anti-conformity contracts MUST include:
                       "Primary-pass exclusion reason: [why T1–T13 sequential pass misses this]"
  priority         : high | medium | low
  source_weakness  : weakness_id (e.g. "W3", from N08 numbered WEAKNESSES block) | null
                     null for technique-gap contracts that don't trace to a specific weakness
  conflict_status  : active | [INTERNAL] | [INPUT-DIRECTIVE]
                     default: active
```

**Binding rules:**
- T4 (role/persona assignment) contracts MUST set `target_section = <role>`. Never `<context>`. Placing persona in `<context>` conflates identity with background and weakens both.
- T13 (escape hatch) contracts MUST set `target_section = <edge_cases>` or `<verification>`. Never `<constraints>`.
- Contracts with `conflict_status != active` are logged in the conflict log but NOT executed by N13.
- Anti-conformity contracts MUST use `technique = "anti-conformity:[name]"` — never T1–T13 labels, even if the underlying technique overlaps. The name distinguishes origin for audit purposes.

---

## Component G — INVENTORY Schema (Typed Node Output)

N04 produces this schema. Downstream consumers: N13, N14, N23, N26.

### Extended Schema (20 keys — authoritative for all modes)

```yaml
inventory:
  # Tier 1 — Universal (populated in all modes when present)
  urls: []                    # full URL strings verbatim, including query strings and fragments
  file_paths: []              # all file paths verbatim including ~, .., extensions
  tech_version: []            # "Name Version" strings — BOTH name AND version together, verbatim
  code_blocks: []             # fenced/inline code verbatim — use block scalars (|-) for all code
  named_entities: []          # product/library/tool names without version numbers

  # Tier 2 — Common (populated when items of this type are found in the input)
  version_specs: []           # standalone version strings: v2.3.1, release 2024.01, etc.
  api_refs: []                # API signatures, endpoint refs, function signatures, verbatim
  numeric_specs: []           # quantities with units, verbatim: "256 samples", "44.1 kHz"
  embedded_directives: []     # action + its full target as one string: "fetch https://..."
  quoted_strings: []          # text in quotes from input, verbatim
  key_constraints: []         # explicit constraint statements from input
  tone_markers: []            # style, register, and audience directives from input

  # Tier 3 — Structural (critical for spec/plan mode; populated when present in any mode)
  phase_step_structure: []    # phase/step names with numbers and ordinals, verbatim
  tier_classification: []     # complete tier criteria blocks — use block scalars for multiline
  conditional_logic: []       # complete if/then/when-X-do-Y blocks, verbatim
  iteration_rules: []         # loop rules with termination conditions, verbatim
  verification_criteria: []   # success criteria, check conditions, verbatim
  edge_case_definitions: []   # edge case names + handling rules together, verbatim
  defaults_fallbacks: []      # default values and fallback behaviors, verbatim

  # Tier 4 — Catch-all
  other: []                   # precision-critical content not fitting any category above;
                              # use sparingly — prefer adding a specific Tier category
```

**Schema rules:**
- All 20 keys required — use `[]` for empty categories, never omit keys.
- All values are verbatim strings — no normalization, summarization, or paraphrase.
- Downstream nodes iterate lists deterministically; schema is a binding contract.
- Tier 1 keys are the preservation floor for normal/minimal mode.
- Tier 2–3 keys are required for spec/plan verification checks S7i and P9i.
- N14 PreservationVerifier counts list lengths per key for the preservation summary line.

### Core Schema (8 keys — legacy prompt-cog v1 compatible)

For prompt-cog v1 compatibility only. New implementations should use the 20-key schema.

```yaml
inventory:
  urls: []
  file_paths: []
  tech_version: []
  code_blocks: []
  named_entities: []
  key_constraints: []
  tone_markers: []
  structural_elements: []     # maps loosely to phase_step_structure + conditional_logic in extended schema
```

---

## Component H — Failure / Repair Subgraph

N17 (RepairRouter) receives aggregated verification reports from active verification nodes. Active set is mode-conditional: N14 + N15 + N16 in normal/verbose/minimal; N14 + N16 only in spec/plan (N15 not active).

### Decision Logic

```
Inputs to N17:
  - preservation_report  (N14: checks 6a–6e)         — always
  - fidelity_result      (N15: check 6f)              — normal | verbose | minimal only
  - quality_results      (N16: checks 6g–6l)          — always
  - draft_xml_fallback   (N13/N23/N26 via E15b/E35b/E39b) — retained, used only at repair_count=2

Step 1 — Aggregate (mode-conditional):
  In normal/verbose/minimal: collect FAILs from N14, N15, N16
  In spec/plan: collect FAILs from N14, N16 only (N15 absent)
  Build: failing_checks[], affected_sections[], failure_detail string

Step 2 — Route:
  IF failing_checks is empty:
    → E20: route verified XML to N18 (PASS path)
       In verbose, first-pass PASS: → E22: route to N20 instead (see Component D)

  IF failing_checks non-empty AND repair_count = 0 (first repair attempt):
    Determine repair scope from failing_checks:
      checks 6a–6e only      → targeted: preservation placement
      check 6f only          → targeted: semantic fidelity
      checks 6g–6l only      → targeted: quality pass
      multiple groups        → full re-synthesis
    Build mode-conditional repair_signal (see Repair Signal Variants below)
    → E19: route to N13 (normal/verbose/minimal) | N23 (spec) | N26 (plan)
    increment repair_count to 1

  IF failing_checks non-empty AND repair_count = 1 (second repair attempt):
    Force repair_scope = full
    → E19: route repair_signal to synthesis node
    increment repair_count to 2

  IF failing_checks non-empty AND repair_count = 2:
    Halt repair loop.
    Retrieve draft_xml_fallback (retained from E15b/E35b/E39b).
    Annotate: prepend <!-- VERIFICATION FAILED: [failing_checks summary] — unverified output -->
    → E20: route annotated XML to N18 (FAIL path)
```

### Repair Signal Variants (mode-conditional)

**Normal / Verbose / Minimal:**
```yaml
repair_signal:
  normalized_input: string          # verbatim, never truncated
  inventory_yaml: object            # full 20-key schema, never truncated
  resolved_contracts: list          # from N11
  conflict_log: list                # from N11
  failing_check_ids: list           # e.g. ["6a", "6c", "6h"]
  affected_sections: list           # e.g. ["<context>", "<constraints>"]
  failure_detail: string            # concatenated detail strings from failing checks
  repair_scope: "targeted" | "full"
  repair_count: integer             # 1 or 2
```

**Spec mode (N23 is repair target):**
```yaml
repair_signal:
  normalized_input: string
  inventory_yaml: object
  domain_block: string              # from N21
  decomp_requirements: string       # from N22
  failing_check_ids: list           # e.g. ["S7b", "S7f"]
  affected_sections: list
  failure_detail: string
  repair_scope: "targeted" | "full"
  repair_count: integer
```

**Plan mode (N26 is repair target):**
```yaml
repair_signal:
  normalized_input: string
  inventory_yaml: object
  goal_analysis: string             # from N24
  action_decomposition: string      # from N24
  dependency_map: string            # from N24
  safeguards: string                # from N25
  failing_check_ids: list           # e.g. ["P9b", "P9g"]
  affected_sections: list
  failure_detail: string
  repair_scope: "targeted" | "full"
  repair_count: integer
```

### N18 FAIL Path Recovery Output

When N17 routes via FAIL path (repair_count=2), N18 appends after the annotated output:

```
Verification failed on checks: [list]. To retry with a better outcome:
  (1) run with minimal mode to reduce synthesis node context pressure
  (2) re-feed the best-effort XML as Type C input for a refinement pass through N13
  (3) for inputs with >12 INVENTORY items or deeply interdependent constraints,
      split the input into smaller independent segments and enhance each separately
```

---


  </context>

  <task>
**Primary objective**

Create a new Claude Code skill named prompt-graph. The skill must use GoT (Graph of
Thought) cognitive methodology as its structural backbone, reference both cognitive and
thought KB via MCP at runtime, and be delivered as a SKILL.md artifact. Its design
must follow a modular programatticly run design with nodes, edges contained in a
graph topology. It must not run so long human user gets frustrated on simple prompt —
value utility to human.

Do not touch the existing prompt-cog skill. Use `~/.claude/skills/prompt-cog/SKILL.md`
as a read-only reference throughout all phases.

**Phase decomposition — execute in order**

Phase 1: Read `~/.claude/skills/prompt-cog/SKILL.md` and extract the full Step 0–7
pipeline with all behavioral rules, hard gates, output markers, and role-switch points.
Record every named behavioral rule (e.g., Hard Gate 3, ZERO INFORMATION LOSS, verbatim
contract) — these must survive mapping to nodes.

Phase 2: Apply the node design reasoning scaffold below to each prompt-cog step.
Map each step to one or more named graph nodes. Document input schema, transformation,
output schema, and downstream consumers for each node.

Phase 3: Define directed edges between nodes. For each edge specify: source node,
target node, data type, cardinality (1:1 / 1:N / conditional), and activation
condition (if any).

Phase 4: Design the GoT controller decision logic — the inline orchestrator decision
point that selects which nodes to activate and in what order, based on input
complexity class. Define its termination condition explicitly.

Phase 5: Build the mode activation matrix. For each invocation flag (normal, --minimal,
--quiet, and combinations), list the active node subset, KB query allowance, and
maximum synthesis spawns.

Phase 6: Identify parallel groups — sets of ≥2 nodes whose input data is fully
available from the same upstream source and whose outputs do not depend on each other.
Run the topology collapse test (see Verification) before finalizing.

Phase 7: Specify KB integration points — for each node that queries a KB, name the
KB (cognitive KB or thought KB), the query type, the input schema (which INVENTORY
YAML categories are used to form the query), and the output schema (what data the
node receives back).

Phase 8: Synthesize the SKILL.md with all sections populated (or with labeled
placeholders for separately-provided data).

**Node design reasoning scaffold (apply to every node in Phase 2)**

Before recording a node, reason through in sequence:
(1) What structured data does this node receive as input? Name the schema.
(2) What transformation or decision does this node perform?
(3) What structured data does this node emit as output? Name the schema.
(4) Which downstream nodes receive this output, and via which edge?

**Graph topology design requirement**

The prompt-graph graph MUST include at least one of the following structural features
to be non-isomorphic with prompt-cog's flat sequential pipeline:
  (a) a branching node that directs flow to different downstream nodes based on
      input complexity class or mode flag
  (b) a parallel group of ≥2 nodes with no inter-dependency, executed simultaneously
      via multiple Agent tool calls in a single message
  (c) a conditional back-edge implementing GoT path refinement (a node whose output
      can loop back to an earlier node when the quality gate is not met)

If after Phase 6 the design contains none of (a), (b), or (c): add at least one
parallel group (candidates: nodes whose inputs share a common upstream source and
whose outputs are independent) before proceeding to Phase 8.
  </task>

  <constraints>
**Runtime budget (tiered)**

Simple inputs (user's INVENTORY ≤8 items, ≤3 explicit constraints): minimal active
node subset; ≤1 KB query total; ≤1 synthesis agent spawn.

Complex inputs (user's INVENTORY >8 items or >5 constraints): full GoT path traversal;
≤2 KB queries (one per KB); ≤2 synthesis agent spawns.

Never exceed 2 synthesis spawns regardless of GoT path count or quality gate retries.

**"Programmatic" in Claude Code context**

"Programmatic" in this context means: nodes execute as inline role-switched pipeline
steps or single spawned agents; conditional branching is implemented as explicit IF
clauses within orchestrator steps; parallel groups are implemented as multiple Agent
tool calls in a single message; there is no literal code execution. The GoT controller
is an inline orchestrator decision point, not a program loop.

**Priority hierarchy (for conflicting design goals)**

1. Runtime efficiency > cognitive KB coverage
2. User experience on simple inputs > GoT controller completeness
3. Detail preservation from prompt-cog > graph topology novelty
4. Deliverable completeness > output length

**SKILL.md design quality criteria (GoT controller termination gate)**

The GoT controller must confirm the produced SKILL.md meets ≥3 of these 6 design
quality dimensions before emitting the final artifact:
(1) Node completeness — all prompt-cog Step 0–7 behavioral rules map to named nodes
(2) Edge coverage — every inter-node data flow has a defined edge with type and
    cardinality
(3) Mode matrix coverage — every invocation flag (normal, --minimal, --quiet) maps
    to a complete active node subset
(4) GoT controller specification — termination condition is explicitly defined
(5) KB integration specificity — each KB-querying node names its query type and
    result schema
(6) Runtime budget compliance — the simplest execution path stays within the tiered
    budget

If fewer than 3 dimensions are satisfied, the GoT controller activates one additional
refinement node whose output feeds the synthesis node directly, then re-evaluates.
Do not restart the entire graph.

**Hard scope constraints**

- do not touch existing prompt-cog skill
- new skill named prompt-graph
- Node registry, edge/channel table, mode activation matrix, parallel groups,
  optimization strategies will be provided separately
- base design on prompt-cog in detail, preserve small details
- implement GoT controller mechanism
- must not run so long human user gets frustrated on simple prompt
- value utility to human
- modular programatticly run design
  </constraints>

  <output_format>
**SKILL.md structure — mandatory sections in this order**

The skill must produce a SKILL.md file with YAML frontmatter followed by 8 sections:

```
---
name: prompt-graph
description: <one-line description>
triggers: ["/prompt-graph"]
version: 1.0.0
---
```

Section order and minimum format specs:

**1. Node Registry**
Table columns: Node ID | Node Name | Type | Input Schema | Output Schema | KB Query | Active Modes

**2. Edge/Channel Table**
Table columns: Edge ID | Source Node | Target Node | Data Type | Cardinality | Activation Condition

**3. Mode Activation Matrix**
Table columns: Mode | Invocation Flag(s) | Active Node IDs | KB Queries Allowed | Max Spawns

**4. Parallel Groups**
Table columns: Group ID | Node IDs in Group | Shared Upstream Source | Independence Condition

**5. Optimization Strategies**
Table columns: Strategy ID | Description | Applicable Mode(s) | Expected Gain

**6. GoT Controller Logic**
Prose section. Must cover: (a) trigger condition for each path, (b) node activation
order per complexity class, (c) quality gate check (≥3/6 dimensions), (d) termination
condition, (e) back-edge behavior when quality gate fails.

**7. Pipeline Narrative**
Inherits prompt-cog's Step 0–7 structural format. For each step: (a) map to the
named node(s) responsible, (b) preserve all hard gates, output markers, behavioral
rules, and role-switch points verbatim from `~/.claude/skills/prompt-cog/SKILL.md`,
(c) annotate any rule that spans multiple nodes.

**8. Smoke Test Checklist**
Lettered tests (A, B, C...) covering: simple input normal mode, minimal mode,
quiet mode, complex input with GoT path expansion, GoT quality gate failure and
recovery, KB query behavior, topology collapse test, missing separately-provided
data fallback.

Sections 1–5 accept placeholder content when separately-provided data is not yet
available, labeled exactly as: `[SECTION NAME — to be populated separately]`.
  </output_format>

  <verification>
**Anti-isomorphism check**

After Phase 6, confirm the graph topology is NOT isomorphic to prompt-cog's 7-step
sequential pipeline. Collapse the graph by removing all branching nodes, parallel
groups, and back-edges. If the remaining structure is identical to a flat 7-step
sequence with no divergence: add at least one parallel group before Phase 8. Document
which structural feature (a, b, or c from the design requirement) satisfies the check.

**Self-critique checklist (complete before emitting SKILL.md)**

(1) Every node has ≥1 incoming AND ≥1 outgoing edge, except the terminal synthesis
    node which has ≥1 incoming and 0 outgoing.
(2) All mode-to-node mappings cover every invocation flag and flag combination.
(3) KB access points are named with KB identifier, query type, and result schema.
(4) GoT controller has an explicitly stated termination condition.
(5) The simplest execution path (normal mode, simple input) stays within the tiered
    runtime budget.
(6) All prompt-cog behavioral rules, hard gates, and output protocol markers from
    Step 0–7 are present in the Pipeline narrative, each annotated with its
    responsible node(s).

**Design-source INVENTORY verbatim check**

Using the embedded INVENTORY YAML in `<context>`, confirm each named_entity and
key_constraint from that INVENTORY appears verbatim (character-for-character identical,
matching capitalization, punctuation, and special characters) in the output SKILL.md
at least once. Placement must match the semantic role mapping: named entities in the
section where they function; key_constraints in Section 1–5 tables or the constraints
equivalent in the pipeline narrative.
  </verification>

  <edge_cases>
**Missing separately-provided data**

If any of the following are not yet provided — Node registry, edge/channel table,
mode activation matrix, parallel groups, optimization strategies — output a structured
SKILL.md scaffold with labeled placeholder sections for each missing component
(e.g., '[NODE REGISTRY — to be populated separately]'). Do not fail silently or
produce an incomplete artifact without visual indication of missing sections.

**Minimal-mode invocation (at prompt-graph runtime)**

When prompt-graph is invoked with --minimal or on a simple input below the complexity
threshold, the GoT controller activates only the minimal active node subset. KB queries
are skipped or collapsed to ≤1 total. The Pipeline narrative section of the SKILL.md
must still be complete — mode affects execution depth, not structural completeness of
the SKILL.md.

**GoT quality gate failure**

If the GoT controller confirms fewer than 3 of the 6 design quality dimensions are
met, activate one additional refinement node — do not restart the entire graph. The
refinement node's output feeds the synthesis node directly, bypassing already-completed
upstream nodes. After the refinement node completes, re-evaluate the 6 dimensions once
and emit regardless of the result (log any remaining failures).

**prompt-cog step without clean node mapping**

If any prompt-cog Step 0–7 behavioral rule, hard gate, or output marker does not map
cleanly to a single named node, capture it verbatim in the Pipeline narrative section
and annotate it with all nodes that share responsibility for enforcing it. No behavioral
detail may be silently dropped.

**Topology collapse test failure**

If Phase 6 produces a graph that collapses to a linear sequence identical to prompt-cog's
7-step pipeline: add at least one parallel group before Phase 8. Candidate nodes are
those whose input data is fully available from the same upstream source and whose outputs
do not depend on each other. Document which nodes form the group and why they are
independent.

**Separately-provided data arrives after initial scaffold**

If the Node registry, edge/channel table, mode activation matrix, parallel groups,
or optimization strategies are provided in a follow-up message after the initial
scaffold is produced: merge the provided data into the appropriate placeholder section.
Do not regenerate sections that are already complete. Validate each incoming table
against the column spec in `<output_format>` before inserting — flag mismatched columns
rather than silently accepting malformed data.
  </edge_cases>
</prompt>
