# Node E — Solutions
Each accepted improvement from Node D engineered into a precise, ready-to-integrate spec change.
Format: target location → exact replacement or insertion text.

---

## E01 — Role-as-Channel-Code structural headers [D01]
**Target:** Step 3 end block; Step 4 end block; Step 5 checklist

**Step 3 — Add after the output bullet list, as a structural protocol note:**
```
- **Output structure:** Wrap all Step 3 output in structural markers:
  ```
  === ANALYST OUTPUT BEGIN ===
  [INTENT block]
  [STRUCTURE block]  ← Normal mode only
  [CONSTRAINTS block]  ← Normal mode only
  [TECHNIQUES block]  ← Normal mode only
  [WEAKNESSES block]  ← Normal mode only
  [INVENTORY YAML]
  === ANALYST OUTPUT END ===
  ```
  The markers allow the Step 5 spawn assembler to extract analyst output by structural address.
```

**Step 4 — Add after the Output bullet list:**
```
- **Output structure:** Wrap all Step 4 output in structural markers:
  ```
  === IDEATION OUTPUT BEGIN ===
  [Primary contract list]
  [Anti-conformity additions]  ← Normal mode only
  [Conflict log]
  === IDEATION OUTPUT END ===
  ```
```

**Step 5 — Add checklist item 5 (renumber existing item 5 to item 6):**
```
5. **Channel boundaries present:** `=== ANALYST OUTPUT BEGIN/END ===` and `=== IDEATION OUTPUT BEGIN/END ===` markers are present and non-empty. The synthesis spawn prompt extracts from these sections by structural address — if boundaries are missing, assembly is unreliable.
```
*(old item 5 → item 6)*

**Step 5 — Add spawn assembly instruction after checklist:**
```
- **Spawn prompt assembly:** Extract content from structural markers only (`=== ANALYST OUTPUT ===` section for analysis blocks + INVENTORY; `=== IDEATION OUTPUT ===` section for contracts + conflict log). Do not dump unstructured orchestrator prose into the spawn prompt body. The synthesis agent's context quality depends on this extraction discipline.
```

---

## E02 — INVENTORY placement verification [D02]
**Target:** Step 6, synthesis protocol, step 5 (inline verification)

**Replace:**
> confirm every INVENTORY item appears verbatim in the draft; confirm all high-priority contracts were applied or logged as skipped

**With:**
> confirm every INVENTORY item appears verbatim in the draft AND is placed in a semantically appropriate XML section per this mapping: `code_blocks` → `<task>` or `<constraints>`; `urls` → section most contextually relevant to the URL's content; `tech_version` → `<context>` or `<constraints>`; `named_entities` → section matching their semantic role; `file_paths` → `<task>` or `<context>` (input source vs. output target); `key_constraints` → `<constraints>`; `tone_markers` → `<role>` or `<context>`; `structural_elements` → section matching their structural function. An item placed in a wrong section fails this check even if it appears verbatim. Confirm all high-priority contracts were applied or logged as skipped.

---

## E03 — Anti-conformity novelty gate [D03]
**Target:** Step 4, Normal mode protocol, step 4 (anti-conformity second pass)

**After the five tests, add:**
```
  - **Novelty gate (6th test):** "Would a primary-pass T1–T13 analyst — running the techniques in order against the analysis findings — have generated this contract?" If yes or genuinely borderline: discard. Only append a contract if you can articulate a specific exclusion reason. **Required rationale field addition:** each anti-conformity contract must include "Primary-pass exclusion reason: [why a sequential T1–T13 pass misses this]" as part of its rationale.
```

---

## E04 — Complexity detection advisory [D04]
**Target:** Step 2, Announce block

**Add after the mode-aware announce line, before the sufficiency check:**
```
- **Complexity advisory:** Before proceeding, quick-scan the input for INVENTORY density signals: count distinct code blocks, URLs, version strings, named technical entities, and explicit constraint statements. If the scan suggests >12 INVENTORY items OR >5 distinct high-impact constraints, append to the announce message:
  - "Advisory: this input appears above the moderate-complexity threshold (~12 INVENTORY items). The quality floor covers moderate-complexity inputs in the expected case — for this input, results may be less reliable. Consider epiphany-prompt DEEP for higher-stakes enhancements."
- **Minimal mode + high complexity:** If both minimal mode AND high complexity are detected, issue a single coordinated advisory: "Minimal mode with complex input: analysis limited to intent and inventory; input appears above the moderate-complexity threshold. For coverage of this input's full constraint space, use normal mode or epiphany-prompt DEEP."
```

---

## E05 — INVENTORY anti-paraphrase explicit instruction [D05]
**Target:** Step 6, synthesis spawn prompt body content (after Hard Gate 3 reminder, before synthesis protocol)

**Add as a new bullet in spawn prompt body:**
```
  - INVENTORY verbatim contract: "Verbatim means character-for-character identical — matching capitalization, punctuation, whitespace, and special characters exactly. Any improvement to surrounding prose must not alter a single character of an INVENTORY item. When your preferred phrasing conflicts with verbatim placement: adjust the surrounding text, not the item. There is no exception to this rule."
```

---

## E06 — Type D agentic workflow detection [D06]
**Target:** Step 1 Detection rules; Step 2 Announce block

**Step 1 — Add after Type A rule:**
```
  - **Type D (flag only — advisory, not routing change)** — Agentic/executable workflow inputs: detect any of: SKILL.md YAML frontmatter (triple-dash `---` followed by `name:` / `description:` / `triggers:`), 3+ consecutive lines starting with `$` or shell command sequences, `<step>` or `<skill:` XML tags within the input, or numbered sequences that describe agent invocation patterns. Type D is tagged as a flag on a Type A input — routing remains Type A. Pass Type D flag to Step 2.
```

**Step 1 Output — Change to:**
```
- **Output:** input type (A / B / C), Type D flag (yes/no), normalized input content
```

**Step 2 — Add after complexity advisory:**
```
- **Agentic workflow advisory:** If Type D flag is set, prepend to the announce message: "Advisory: this input appears to describe an executable workflow or runnable skill. Hard Gate 3 reduces but cannot eliminate the risk of synthesis treating enhancement as execution for this input class. Review the output carefully before use." Do not block — proceed with normal routing.
```

---

## E07 — Same-target contract conflict detection [D07]
**Target:** Step 4, end of Normal mode and Minimal mode protocols (before output block)

**Add as a "Contract Finalization" sub-step at the end of Step 4:**
```
- **Contract Finalization (all modes):**
  1. **Same-target conflict scan:** Group all contracts by (technique, target_section) pair. If two or more contracts in a group specify incompatible actions (one adds / one removes the same element; or two specify mutually exclusive content for the same section with the same technique), flag as an **internal conflict**: keep the higher-priority contract, log the other with reason "Internal conflict: superseded by higher-priority contract targeting same [technique, target_section]."
  2. **Conflict log update:** Merge internal conflicts into the conflict log alongside input-directive conflicts. Conflict log now contains two types: `[INPUT-DIRECTIVE]` and `[INTERNAL]`.
```

---

## E08 — Quality floor expected-value language [D08]
**Target:** Design Constraints table, quality rule

**Replace:**
> DO NOT produce lower quality output than prompt-epiphany baseline for moderate-complexity inputs (see Complexity Scope below)

**With:**
> Maintain expected output quality at or above the prompt-epiphany baseline for moderate-complexity inputs — this is a statistical expected-value design target, not a per-invocation guarantee (see Complexity Scope below)

---

## E09 — VERIFICATION:FAIL recovery guidance [D09]
**Target:** Step 7, FAIL path, after display + save prompt

**Add at end of FAIL path:**
```
  - **Recovery suggestions:** After displaying failure summary, append: "Synthesis verification failed. To retry with a better outcome: (1) run with `--minimal` to reduce context pressure on the synthesis agent; (2) pass the best-effort XML back to prompt-cog as a Type C input for a refinement pass; (3) if the input is complex (>12 INVENTORY items), switch to epiphany-prompt for this enhancement."
```

---

## E10 — Role transition deactivation line [D10]
**Target:** Step 4 preamble (before role switch declaration)

**Add before the Step 4 role declaration:**
```
- **Role transition:** Before entering the ideation role, explicitly close the analyst role in context: "The analyst role has concluded. All analyst output is captured in the ANALYST OUTPUT section above. You are no longer in analysis mode." Then apply the ideation specialist role declaration.
```

---

## E11 — Step 5 Interface 2 coherence strengthening [D11]
**Target:** Step 5, checklist item 5 (or item 6 after E01 renumbering)

**Replace existing item 5:**
> **Interface 2 coherence:** at least one contract maps to each high-impact weakness in the WEAKNESSES block. A checklist PASS with contracts that only address low-impact weaknesses is a silent quality failure — this item prevents it.

**With:**
> **Interface 2 coherence:** for each high-impact weakness in the WEAKNESSES block, at least one mapped contract must (a) reference that weakness and (b) use a technique and action that plausibly address the specific gap described in that weakness's causal explanation. Presence-only mapping (a contract exists that mentions the weakness but uses an irrelevant technique or a non-specific action) does not satisfy this check. (Minimal mode: skip — WEAKNESSES block is not produced.)

---

## E12 — Minimal mode transparency advisory [D12]
**Target:** Step 2, minimal mode announce line

**Replace:**
> Minimal: "Using prompt-cog (minimal mode) to enhance this prompt."

**With:**
> Minimal: "Using prompt-cog (minimal mode) to enhance this prompt. Analysis limited to intent and inventory — technique gap coverage and weakness scoring are skipped. Use normal mode for prompts requiring full technique application."

---

## E13 — Flag disambiguation for prose-like --token [D13]
**Target:** Step 0, Flag Detection, after unknown flag handling

**Add disambiguation rule:**
```
- **Unknown flag disambiguation:** Before surfacing an unknown-flag error, check: is the unrecognized `--token` followed by non-flag words forming a natural phrase (i.e., it reads as `--analyze this algorithm`, not `--verbose`)? The known flag set is closed and exact: `--minimal`, `--quiet`, `--verbose`, `--specification`, `--plan`. If a `--` token is unrecognized AND appears to continue into prose content, issue a soft advisory instead of halting: "Token '[...]' resembles a flag but is not a recognized prompt-cog flag. Treating as prompt content. If you intended a mode flag, check spelling." Proceed with execution. If the unrecognized token is at a flag position AND followed by no content or a clearly separate sentence: halt with the standard unknown-flag error.
```

---

## E14 — Inline quality checkpointing post-analysis [D14]
**Target:** End of Step 3 output block (inside ANALYST OUTPUT section, before the closing marker)

**Add as a self-check requirement at the end of Step 3 output:**
```
- **Step 3 self-check (informational, non-blocking):** Before closing the ANALYST OUTPUT section, verify:
  - INTENT block: is the intent specific (names the goal, success criteria, and target audience or use case)? If it is vague ("improve this prompt"), note: "INTENT: THIN — ideation may be underconstrained."
  - WEAKNESSES block (normal mode): does each weakness have a causal explanation (not just a label)? If any weakness is label-only (e.g., "vague"), note: "WEAKNESS: LABEL-ONLY — contract targeting this weakness may miss the root cause."
  - INVENTORY YAML: are all 8 keys present, even if empty (`[]`)? If not, note which key is missing.
  These notes are for the ideation step — they are informational and do not block execution. No re-run is initiated; the notes accompany the ANALYST OUTPUT into the ideation context.
```

---

## Cross-Reference Verification
All 14 solutions (E01–E14) verified against:
- [ ] No new agent spawn required: YES for all
- [ ] No contradiction to 1-spawn constraint: YES for all  
- [ ] No deprecation of prompt-epiphany or epiphany-prompt: YES for all
- [ ] Consistent with existing Design Decisions: YES for all
- [ ] v1-scoped (no repair loops, no DEEP, no stage files): YES for all
