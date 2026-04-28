# Graph-Node Prompt Enhancement System — Component Library v1.1
*v1.0 audited and corrected. See Audit Log for all changes across both passes.*

---

## Audit Log

### v1.0 — Initial audit fixes (A1–H3)
- A1–A8: Node input gaps, N10 output mislabel, N13 spurious INTENT edge, added N21–N26
- B1–B7: E03 wrong targets, missing E00a–E00d, E04 N13 removal, E11 relabel, E12 clarification, spec/plan edges, condition standardisation
- C1–C3: Added N21–N26 rows, removed * on N13
- D1–D4: Wave 2 split, minimal path rename, spec/plan wave plans added
- E1–E2: O7 language, O9 N10 omission
- F1–F3: source_weakness optional, conflict_status default, anti-conformity technique rule
- G1–G2: Extended to 19-key schema
- H1–H3: Repair count terminology, repair signal payload, N17 check ID logging

### v1.2 — Third audit fixes (BUG A–B, LOGIC A–B, DOC A, IMPROVE A)

**BUG A** N04 output and E05 said "19-key YAML/schema" — schema was extended to 20 keys in v1.1 L2 (tone_markers added). Updated N04 output column and E05 data type to "20-key."

**BUG B** No edge delivered normalized_input to N10, N13, N23, or N26 on first synthesis pass. These nodes all list "Normalized input" as an input in Component A, but Component B had no corresponding delivery edge. (Repair pass receives it via repair_signal E19; first pass was blind to original text.) Added E40a–E40d.

**LOGIC A** Wave 4 showed N12 → N13 sequential but Component B had no edge N12 → N13, and N13's inputs did not include N12's advisory. Without a data dependency, topological sort places them in parallel — but sequential ordering is intentional: N12's coherence advisory should inform N13's synthesis priorities before it begins. Added E13b (N12 → N13, coherence_advisory, normal | verbose) and updated N13 inputs.

**LOGIC B** Verbose Wave 9 FAIL path reverts to "first-pass verified XML" with no defined retention mechanism. Unlike repair_count=2 fallback (explicit E15b/E35b/E39b), the first-pass XML had no explicit path back to N17. Added note to N17 row (Component A) and Wave 9 description specifying N17 retains first-pass verified XML as named internal state (first_pass_xml) when routing to N20 via E22.

**DOC A** Footer said "Total fixes applied: 26 (v1.0: 8 | v1.1: 18)." The "8" was the number of section categories (A–H), not the fix count. v1.0 had 32 individual fixes; v1.1 had 18; v1.2 adds 6. Total corrected to 56.

**IMPROVE A** O7 priority labels counted 4→1 where 1=highest, but the qualifier said "(lowest first)" — natural reading expects 1=lowest. Relabeled as 1=lowest (truncate first) through 4=highest (never truncate).

### v1.1 — Second audit fixes (BUG 1–6, L1–7, I1–5)

**BUG 1** E22 / verbose ordering: N20 was receiving unverified N13 draft output. E22 changed from `N13 → N20` to `N17 → N20` (first-pass PASS only). Component D verbose wave plan restructured: N20 now runs after first-pass N17 PASS, followed by a second verification pass. N20's inputs updated to "first-pass verified XML."

**BUG 2** N09 minimal inputs: INTENT-only was insufficient — N09 cannot detect which techniques are already present without the raw text. Added `E00e: N02 → N09 (normalized_input, minimal)`. N09 minimal inputs updated to "normalized input, INTENT."

**BUG 3** N17 missing draft_xml: on repair_count=2, N17 had no XML to annotate. Added `E15b: N13 → N17`, `E35b: N23 → N17`, `E39b: N26 → N17` (draft fallback edges, retained on repair_count=2 only). N17 inputs updated.

**BUG 4** E31 spurious N21 target: removed N21 from E31. SpecDomainAnalyzer does not use INVENTORY. INVENTORY reaches N23 via E05.

**BUG 5** N17 mode-conditional inputs: N15 does not run in spec/plan mode. N17 inputs note updated; Component H Decision Logic updated with mode-conditional aggregation rule.

**BUG 6** Spec/plan wave parallelism: N04 and N21 both depend only on N02 — made parallel (Spec Wave 1: `N04 || N21`). Same fix for plan (Plan Wave 1: `N04 || N24`). Spec and plan wave plans renumbered accordingly.

**L1** source_weakness references weakness_id but N08 had no ID system. N08 outputs updated to emit numbered entries (W1, W2...). Component F source_weakness description updated.

**L2** tone_markers missing from 19-key schema. Added as Tier 2 key.

**L3** O7 only covered N13. Updated to cover all synthesis nodes: N13, N23, N26.

**L4** E19 only listed N13 as repair target. Updated to `N17 → N13 | N23 | N26 (mode-conditional)`.

**L5** Repair signal included contracts/conflict_log fields absent in spec/plan mode. Component H now defines mode-conditional repair signal variants.

**L6** Recovery guidance item (3) referenced "deeper orchestration scale" (stale epiphany-prompt language). Replaced with graph-system-appropriate guidance.

**L7** N16 had no edges for analysis blocks despite needing them for check 6h. Added `E41: N05, N06, N07, N08 → N16 (analysis_blocks, normal | verbose)`. N16 inputs updated. Note added: in minimal mode, check 6h runs on INTENT + INVENTORY only (N05–N08 not active).

**I1** N12 output included meaningless "PASS" for a non-blocking node. Output simplified to `advisory | none`.

**I2** N08 numbered IDs: covered by L1 fix above.

**I3** key_constraints was in Tier 4 (catch-all). Moved to Tier 2.

**I4** Tier 2 "required when present" contradicted "all 19 keys required" schema rule. Reworded to "populated when items of this type are found in the input."

**I5** E00e added for N09 normalized input in minimal mode: covered by BUG 2 fix above.

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

*File: /home/myuser/docs/epiphany/graph-node-components-v1.md*
*Version: 1.2 | Components: A–H | Nodes: N01–N26 | Edges: E00a–E41 (+ E13b, E40a–E40d) | Optimizations: O1–O9*
*Total fixes applied: 56 (v1.0: 32 across 8 categories | v1.1: 18 | v1.2: 6)*
