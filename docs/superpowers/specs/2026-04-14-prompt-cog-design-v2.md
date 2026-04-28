# prompt-cog Skill Design Spec — v2
**Date:** 2026-04-14
**Status:** Draft v2 — integrated from epiphany genius analysis pipeline (Nodes C→D→E)
**Audience:** Skill architect implementing prompt-cog SKILL.md in Claude Code
**Changes from v1:** E01–E14 integrated (Role-as-Channel-Code, placement verification, novelty gate, complexity advisory, anti-paraphrase instruction, Type D detection, contract finalization, quality floor language, FAIL recovery, role deactivation, Interface 2 coherence, minimal mode transparency, flag disambiguation, inline quality checkpoint)

---

## Overview

`prompt-cog` is a new prompt enhancement skill that targets the design gap between `prompt-epiphany` (fast, single-pass, loose step discipline) and `epiphany-prompt` (modular, subagent-orchestrated, too slow). It is **not** a refactor of either. It inherits prompt-epiphany's step structure and selectively imports epiphany-prompt's enhancements, with a primary constraint: run faster than epiphany-prompt while being more programmatic than prompt-epiphany.

### Source Skills (unchanged)
- `prompt-epiphany` at `.claude/skills/prompt-epiphany/` — baseline quality reference; T1–T13 technique definitions are authoritative here
- `epiphany-prompt` at `.claude/skills/epiphany-prompt/` — enhancement source

### Design Constraints
- DO keep as many epiphany-prompt enhancements as possible
- DO make architecture modular with separable steps (each step with distinct role context)
- DO run faster than epiphany-prompt
- DO run smoothly: reliable, deterministic step execution with no dropped steps
- DO be more programmatic than prompt-epiphany (steps followed exactly, not loosely)
- DO NOT replace or deprecate prompt-epiphany or epiphany-prompt
- DO NOT sacrifice reliability for speed
- Maintain expected output quality at or above the prompt-epiphany baseline for moderate-complexity inputs — this is a statistical expected-value design target, not a per-invocation guarantee (see Complexity Scope below) *(E08)*

### Complexity Scope
The quality floor is an **expected-value claim for moderate-complexity inputs**, not a per-invocation guarantee across all input classes. The architecture holds its floor when: INVENTORY ≤ ~12 items, constraints are not deeply interdependent, and synthesis does not require cross-constraint judgment at scale. On complex inputs beyond this threshold, three degradation vectors compound: context contamination from inline Steps 3–4, self-review bias in Step 6, and possible spawn prompt truncation. A lightweight complexity advisory exists in v1 (Step 2) that warns the user when input appears to exceed the threshold, but it does not block execution or auto-escalate — a blocking escalation trigger to DEEP mode is the planned v2 path. Implementers and users should not rely on the quality floor as an absolute per-invocation guarantee on complex technical prompts.

---

## Flags

| Flag | Effect | Combines with |
|------|--------|---------------|
| *(none)* | Normal mode (default) | — |
| `--minimal` | Minimal mode: reduced analysis, skip anti-conformity pass | `--quiet` |
| `--quiet` | Skip terminal display; save directly without asking | Any mode flag |
| `--verbose` | Deferred (v2) — surface warning and suggest `--minimal` or none | — |
| `--specification` | Deferred (v2) — surface warning | — |
| `--plan` | Deferred (v2) — surface warning | — |

Flag position rule (inherited from source skills): flags are valid only as the first or last standalone token of the invocation string. A flag mid-sentence within the prompt body is treated as prompt content, not a mode selector.

Conflicting flags: `--minimal` + `--verbose` → block, ask user to pick one.

Deferred flags (`--verbose`, `--specification`, `--plan`): do not proceed. Surface: "The `[flag]` flag is not yet supported in prompt-cog. Run without a flag for normal mode, or use `--minimal` for a lighter-weight pass."

---

## Enhancement Inventory

For each epiphany-prompt enhancement, verdict is one of: PORT AS-IS / PORT SIMPLIFIED / DEFER / SKIP.

| Enhancement | Quality Contribution | Overhead Source | Verdict | Minimum Viable Form |
|---|---|---|---|---|
| YAML INVENTORY schema | Preservation-first synthesis — prevents verbatim content (URLs, paths, code blocks, version strings) from being paraphrased or dropped. Primary driver of output fidelity on technical prompts. | None | PORT AS-IS | Keep full schema; hold as in-context artifact, pass to synthesis agent in spawn prompt body |
| 6-dimension analysis | Structured gap detection across intent, structure, constraints, techniques, weaknesses, and domain specifics. Catches non-obvious gaps that single-pass analysis misses. | None — runs inline in prompt-epiphany already | PORT AS-IS | Run inline in orchestrator context under analyst role framing |
| Enhancement contracts schema (v1) | Forces ideation output into actionable, testable units (`technique | target_section | action | rationale | priority`). Enables deterministic synthesis execution. | None | PORT AS-IS | Same YAML v1 schema, held in-context, passed to synthesis agent in spawn prompt body |
| Anti-conformity second pass | Targets unconventional enhancements that primary T1–T13 pass defaults away from. The +32.9% novelty figure was measured in epiphany-prompt's DEEP architecture with an independent spawned agent; novelty transfer to an inline in-context re-read has not been empirically validated and may be lower due to context contamination. | Agent spawn (DEEP M12 sub-step) | PORT SIMPLIFIED | Run as second contrarian re-read immediately after primary ideation, same context — no spawn. Novelty gate added (E03). |
| Hard Gate 3 reminder | Prevents synthesis agent from executing the input prompt rather than enhancing it. Critical for inputs that describe runnable skills or agentic workflows. | Zero | PORT AS-IS | Embed verbatim in synthesis spawn prompt header |
| Return value contracts | `VERIFICATION: PASS\n\n<XML>` / `VERIFICATION: FAIL — [summary]` — deterministic parse, no NLP routing needed. | Zero | PORT AS-IS | Inherit exact format; orchestrator parses on `VERIFICATION:` prefix |
| T4 element binding rule | Prevents persona content from leaking into `<context>`; keeps `<role>` and `<context>` semantically separate. Prevents a well-documented structural failure mode. | Zero | PORT AS-IS | Include as binding rule in synthesis spawn prompt |
| Contract conflict rule | Skip + log contracts conflicting with explicit input directives. Prevents enhancement from overriding user intent. | Zero | PORT AS-IS | Applied inline during ideation and enforced in synthesis; extended to internal contract conflicts (E07) |
| Three-layer routing rule | Orchestrator reads return messages only — prevents routing decisions on stale or partial content. Ensures determinism. | Zero | PORT AS-IS (adapted) | In 1-spawn model: orchestrator reads synthesis return message only; no stage files exist |
| Type A/B/C input routing | Correct handling of raw prompts vs. prior prompt-epiphany XML vs. prior epiphany-prompt/prompt-cog XML. Prevents re-enhancement artifacts. | Zero | PORT AS-IS | Same detection logic in orchestrator Step 1; Type D agentic flag added (E06) |
| Flag detection + unknown flag warning | Catches unsupported flags early with clear error rather than silent misbehavior. | Zero | PORT AS-IS | Same STEP 0 logic; prose disambiguation added (E13) |
| Tilde expansion + collision handling | Normalizes `~` paths; prevents output overwrites when save path already exists. | Zero | PORT AS-IS | Same path rules in output step |
| CreativeDC framing | Divergent-convergent methodology increases ideation range before convergence to contracts. Produces more varied contract set than linear T1–T13 scan. | None — framing only | PORT SIMPLIFIED | 2–3 sentence framing idiom at ideation role entry; no dedicated agent |
| Weakness impact scoring | Scores weaknesses high/medium/low; drives enhancement budget allocation toward high-impact gaps. | None for scoring itself | PORT SIMPLIFIED | Inline scoring during analysis step; used to prioritize contract generation order |
| KB sources in module headers | Each module references specific KB entries, ensuring agents apply scoped technique knowledge. | File reads inside spawned agents | PORT SIMPLIFIED | Embed 3 fixed KB snippets inline in synthesis spawn prompt body (~300 tokens); skip file-read instructions. See Step 6 for selection rationale. |
| Pre-spawn module validation | 8-key frontmatter check before each agent spawn catches misconfigured modules before wasted agent calls. | Minimal per-spawn overhead | PORT SIMPLIFIED | Pre-spawn checklist: analysis non-empty, INVENTORY populated, contracts non-empty, spawn prompt assembled within token budget, channel boundaries present (E01) |
| Repair loops | Retry-on-verification-failure up to 1× per stage. Safety net for synthesis errors. | 1–2 additional agent spawns per failure | DEFER | Quality risk: rare synthesis failures surface without recovery. Acceptable given low baseline failure rate on a well-specified synthesis prompt. Include in v2. |
| Session directory + stage files | File-based inter-agent communication layer — independently inspectable, debuggable stage outputs. | Session init + per-stage file writes + per-spawn reads | SKIP | Eliminated by architecture: stage content passed in spawn prompt body via structured channel extraction (E01). Not a quality regression — the session directory was infrastructure, not quality. Final output is still saved for inspection. |
| DEEP expansion wave (M5-exp) | Dedicated expansion agent produces richer output for complex prompts. Meaningful quality gain on long technical prompts. | 1 additional agent spawn + repair structure | DEFER | Quality risk: less elaborated output on complex prompts. The DEEP wave is prompt-cog's primary quality upgrade path for v2. |
| Scale variants (STANDARD/DEEP) | Two execution modes with different quality/speed trade-offs. | Architectural overhead to support both code paths | DEFER | prompt-cog ships STANDARD-equivalent only. Add DEEP after STANDARD is stable. |

---

## Architecture

prompt-cog uses a **1-spawn inline orchestration** model. All analysis and ideation run in the orchestrator's context under role-switched framing with structured channel markers. One synthesis agent handles synthesis, inline verification, and return.

```
Orchestrator (inline)
  Step 0: Flag Detection
  Step 1: Input Routing (Type A/B/C + Type D advisory flag)
  Step 2: Announce + Complexity Advisory + Sufficiency
  Step 3: Analysis [role: analyst] → === ANALYST OUTPUT BEGIN/END ===
  Step 4: Ideation [role: ideation specialist] → === IDEATION OUTPUT BEGIN/END ===
  Step 5: Pre-Spawn Checkpoint (channel-extraction assembly)
       ↓
  Synthesis Agent (1 spawn) [role: synthesis specialist]
  Step 6: Synthesis + Inline Verification → return VERIFICATION: PASS/FAIL + XML
       ↓
Orchestrator (inline)
  Step 7: Output + Save (with recovery guidance on FAIL)
```

### Step 0 — Flag Detection
- **Context:** Inline, orchestrator
- **Role:** none (structural parsing)
- **Input:** raw invocation string
- **Output:** validated flag set (mode), stripped invocation string with flags removed
- Strip detected flags from first/last token position only. Never strip flags from within the prompt body.
- Deferred flags: surface the deferred-flag warning message (see Flags table) and halt.
- Conflicting flags (`--minimal` + `--verbose`): surface conflict warning and halt until user picks one.
- Unknown flags: surface warning and halt rather than silently proceeding.
- **Unknown flag disambiguation (E13):** Before surfacing an unknown-flag error, check: is the unrecognized `--token` followed by non-flag words forming a natural phrase (i.e., it reads as prose rather than a flag invocation)? The known flag set is closed and exact: `--minimal`, `--quiet`, `--verbose`, `--specification`, `--plan`. If a `--` token is unrecognized AND appears to continue into prose content: issue a soft advisory instead of halting: "Token '[...]' resembles a flag but is not a recognized prompt-cog flag. Treating as prompt content. If you intended a mode flag, check spelling." Proceed with execution. If the unrecognized token is at a flag position AND followed by no content or a clearly separate sentence: halt with the standard unknown-flag error.

### Step 1 — Input Routing
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** step 0 output (stripped invocation string)
- **Detection rules:**
  - **Type B** — Prior `prompt-epiphany` output: detect `<meta source="prompt-epiphany"/>` as direct child of root `<prompt>` element. Strip the `<prompt>` XML wrapper; use the inner content as the normalized input.
  - **Type C** — Prior `epiphany-prompt` or `prompt-cog` output: detect `<meta source="epiphany-prompt"/>` or `<meta source="prompt-cog"/>` as direct child of root `<prompt>` element. Same extraction rule as Type B: strip outer `<prompt>` wrapper, use inner content.
  - **Type A** — Everything else (raw prompt, plain text, partial XML, file path). If input starts with `~/`, `/`, `./`, or `../` and refers to an existing file, read file contents as input. Otherwise treat as inline text.
  - **Type D advisory flag (E06):** Independently of A/B/C routing, detect inputs whose structure IS an executable/agentic workflow — not inputs that merely discuss or reference one. Detection patterns: SKILL.md YAML frontmatter where the input's root structure is the skill definition itself (triple-dash `---` header with `name:` / `description:` / `triggers:` as top-level keys), inputs where the primary content is 3+ consecutive executable shell command lines (not shell commands mentioned as examples inside prose), `<step>` or `<skill:` XML tags forming the document's top-level structure, or numbered sequences that define an agent invocation chain as the document's primary purpose. A prompt that asks Claude to write or analyze a SKILL.md is NOT Type D — the SKILL.md content is the target of enhancement, not the input structure. Type D is a flag only — routing remains A/B/C. Pass Type D flag to Step 2.
- If Type B or C XML is malformed (root element not parseable): treat as Type A; do not attempt partial extraction.
- **Output:** input type (A / B / C), Type D flag (yes/no), normalized input content

### Step 2 — Announce + Sufficiency
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** normalized input + type + mode flag + Type D flag
- **Agentic workflow advisory (E06):** If Type D flag is set, output FIRST (before the announce message): "Advisory: this input appears to describe an executable workflow or runnable skill. Hard Gate 3 reduces but cannot eliminate the risk of synthesis treating enhancement as execution for this input class. Review the output carefully before use."
- **Announce (mode-aware):**
  - Normal: "Using prompt-cog to analyze and enhance this prompt."
  - Minimal: "Using prompt-cog (minimal mode) to enhance this prompt." — then on the next line: "Analysis limited to intent and inventory — technique gap coverage and weakness scoring are skipped. Use normal mode for prompts requiring full technique application." *(E12)*
  - Quiet: "Using prompt-cog (quiet mode) to enhance this prompt."
  - Quiet + Minimal: "Using prompt-cog (quiet + minimal mode) to enhance this prompt."
- **Complexity advisory (E04):** After the announce, quick-scan the input for INVENTORY density signals: count distinct code blocks, URLs, version strings, named technical entities, and explicit constraint statements visible in the raw input text. Apply:
  - If scan suggests >12 INVENTORY items OR >5 explicit constraint statements: append "Advisory: this input appears above the moderate-complexity threshold (~12 INVENTORY items). The quality floor covers moderate-complexity inputs in the expected case — for this input, results may be less reliable. Consider epiphany-prompt DEEP for higher-stakes enhancements."
  - If minimal mode AND high complexity both detected: issue combined advisory instead: "Minimal mode with complex input: analysis limited to intent and inventory; input appears above the moderate-complexity threshold. For coverage of this input's full constraint space, use normal mode or epiphany-prompt DEEP."
- **Sufficiency check:** Block if input has no discernible task, is fundamentally ambiguous, or has no identifiable intent. Explain what's missing and wait. Do not proceed to analysis until input is adequate. An input with some structure (even a rough draft) passes. Single words, fragments with no context, or blank inputs fail. An input with no INVENTORY (empty code blocks, URLs, constraints) is a valid input — enhancement will focus on structural and technique improvements. Do not treat low-INVENTORY inputs as insufficient.
- **Output:** sufficiency decision (proceed or block with explanation)

### Step 3 — Analysis *(role switch)*
- **Context:** Inline, orchestrator — role-switched
- **Role:** `"You are a structured prompt analyst. Your task is to analyze the input prompt across 6 dimensions and produce the authoritative INVENTORY."`
- **T1–T13 reference:** The 13 technique definitions are in `prompt-epiphany`'s SKILL.md `## Techniques` section. Read that file if the definitions are not in context.
- **Input:** normalized input
- **Normal mode output (held in context):**
  - INTENT block — what the prompt is trying to accomplish, desired end state, success criteria
  - STRUCTURE block — current organization, missing elements
  - CONSTRAINTS block — explicit and implicit constraints, conflicts
  - TECHNIQUES block — T1–T13 gap analysis (already present / needed / impact)
  - WEAKNESSES block — vagueness, likely misinterpretations, contradictions; each weakness scored high / medium / low impact with a **causal explanation** (not just a label — state why the weakness exists and what failure mode it causes)
  - INVENTORY YAML — full schema (urls, file_paths, tech_version, code_blocks, named_entities, key_constraints, tone_markers, structural_elements); all values verbatim, no normalization; use `[]` for empty categories, do not omit keys
- **Minimal mode output (held in context):**
  - INTENT block only (abbreviated: 3–5 sentences on purpose + success criteria)
  - INVENTORY YAML (full schema — preservation is non-negotiable in all modes)
  - Skip STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES blocks
- INVENTORY is the authoritative preservation list referenced throughout Steps 4–6. Never paraphrase or summarize INVENTORY entries.
- **Step 3 self-check — informational, non-blocking (E14):** At the end of analyst output, before closing the section marker, verify:
  - INTENT: is it specific? (names goal, success criteria, target audience or use case) — if vague, note "INTENT: THIN — ideation may be underconstrained"
  - WEAKNESSES (normal mode): does each weakness have a causal explanation? — if any is label-only (e.g., "vague" with no cause), note "WEAKNESS: LABEL-ONLY — contract targeting this may miss root cause"
  - INVENTORY YAML: are all 8 keys present, even if empty? — if missing, note which key
  These notes accompany the analyst output into the ideation context. They do not block execution and do not trigger a re-run.
- **Output structure (E01):** Wrap all Step 3 output in structural markers:
  ```
  === ANALYST OUTPUT BEGIN ===
  [INTENT block]
  [STRUCTURE block]   ← Normal mode only
  [CONSTRAINTS block] ← Normal mode only
  [TECHNIQUES block]  ← Normal mode only
  [WEAKNESSES block]  ← Normal mode only
  [INVENTORY YAML]
  [Step 3 self-check notes]
  === ANALYST OUTPUT END ===
  ```
  These markers allow the Step 5 spawn assembler to extract analyst output by structural address.

### Step 4 — Ideation *(role switch)*
- **Context:** Inline, orchestrator — role-switched
- **Role transition (E10):** Before applying the ideation role declaration, output the following text into the orchestrator context (not user-facing): "The analyst role has concluded. All analyst output is captured in the ANALYST OUTPUT section above. You are no longer in analysis mode." This creates an explicit cognitive boundary in the inline context before the role switch.
- **Role:** `"You are a divergent-convergent enhancement designer. You transform analysis findings into actionable enhancement contracts. You think laterally before converging."`
- **Input:** step 3 output (inline, same context, referenced from the ANALYST OUTPUT section)
- **Normal mode protocol:**
  1. For every weakness: identify an enhancement contract or note why not viable
  2. For every needed technique from TECHNIQUES block: design a specific application as a contract
  3. Allocate more contracts to high-impact weaknesses (weakness impact scoring drives budget)
  4. **Anti-conformity second pass:** after primary contract list, re-read input + primary contracts with contrarian framing. Ask: what unconventional enhancement did the primary pass miss? Append 1–3 contracts if they pass six tests (Impact / Risk / Validity / Necessity / Preservation / **Novelty (E03)**)
     - **Novelty gate (6th test, E03):** "Would a primary-pass T1–T13 analyst — running the techniques in order against the analysis findings — have generated this contract?" If yes or genuinely borderline: discard. Only append a contract if you can articulate a specific exclusion reason. Each anti-conformity contract's rationale field MUST include: "Primary-pass exclusion reason: [why a sequential T1–T13 pass misses this]"
  5. Apply contract conflict rule: skip contracts that conflict with explicit input directives; log them as `[INPUT-DIRECTIVE]` conflicts
- **Minimal mode protocol:**
  - Run steps 1 and 2 only (weakness contracts and technique contracts from INTENT analysis)
  - Skip anti-conformity second pass (sub-step 4 of the normal mode protocol above)
  - Skip weakness impact scoring allocation (step 3) — treat all weaknesses as equal priority
- **Contract Finalization — all modes (E07):**
  1. **Same-target conflict scan:** Group all contracts by (technique, target_section) pair. If two or more contracts in a group specify incompatible actions (one adds / one removes the same element; or two specify mutually exclusive content for the same section with the same technique): keep the higher-priority contract, log the other as an `[INTERNAL]` conflict: "Internal conflict: superseded by higher-priority contract targeting same [technique, target_section]."
  2. **Conflict log update:** Merge `[INTERNAL]` conflicts into the conflict log alongside `[INPUT-DIRECTIVE]` conflicts.
- **Output (held in context):**
  - Primary contract list
  - Anti-conformity additions (if any; normal mode only)
  - Conflict log (skipped contracts + type `[INPUT-DIRECTIVE]` or `[INTERNAL]` + reason)
- **Contract format (v1):** `technique | target_section | action | rationale | priority`
- **T4 binding rule:** contracts with `technique: T4` MUST set `target_section: "<role>"`, never `"<context>"`
- **Output structure (E01):** Wrap all Step 4 output in structural markers:
  ```
  === IDEATION OUTPUT BEGIN ===
  [Primary contract list]
  [Anti-conformity additions]  ← Normal mode only
  [Conflict log]
  === IDEATION OUTPUT END ===
  ```

### Step 5 — Pre-Spawn Checkpoint
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** steps 3–4 structured output
- **Checklist (abort if any fail):**
  1. Analysis blocks produced (INTENT present; INVENTORY present in all modes)
  2. INVENTORY has ≥ 1 entry OR explicitly marked empty (all categories `[]` with reason noted)
  3. Contract list non-empty
  4. **Channel boundaries present (E01):** `=== ANALYST OUTPUT BEGIN/END ===` and `=== IDEATION OUTPUT BEGIN/END ===` markers are present and non-empty in context. The synthesis spawn prompt extracts from these sections by structural address — if boundaries are missing, assembly cannot proceed reliably. *(Must verify before item 5 since assembly depends on these markers.)*
  5. Synthesis spawn prompt assembled without obvious truncation: extract content from channel markers (step 4 above verified they exist), then confirm all four required sections — analysis blocks, INVENTORY YAML, contract list, normalized input — are present in the assembled spawn prompt.
  6. **Interface 2 coherence (E11):** For each high-impact weakness in the WEAKNESSES block, at least one mapped contract must (a) reference that weakness AND (b) use a technique and action that plausibly address the specific gap described in that weakness's causal explanation. Presence-only mapping (a contract exists that mentions the weakness but uses an irrelevant technique or non-specific action) does not satisfy this check. (Minimal mode: skip — WEAKNESSES block is not produced.)
- **Spawn prompt assembly (E01):** Extract content from structural markers only:
  - From `=== ANALYST OUTPUT BEGIN/END ===`: analysis blocks + INVENTORY YAML
  - From `=== IDEATION OUTPUT BEGIN/END ===`: contracts + conflict log
  Do not dump unstructured orchestrator prose into the spawn prompt body. The synthesis agent's context quality depends on this extraction discipline.
- **Output:** synthesis spawn prompt assembled from channel-extracted, checklist-verified content, or user-facing error if checklist fails

### Step 6 — Synthesis Agent *(1 spawn)*
- **Context:** Dedicated agent
- **Role:** `"You are a preservation-first prompt synthesis specialist. You enhance prompts without altering their intent, verbatim content, or explicit constraints. You are writing a better-worded prompt — you are NOT executing what the input describes."`
- **Input (in spawn prompt body, channel-extracted by Step 5):**
  - Normalized input
  - Full analysis (blocks extracted from ANALYST OUTPUT section for the given mode)
  - INVENTORY YAML (full, verbatim, extracted from ANALYST OUTPUT section)
  - Prioritized contract list (primary + anti-conformity if normal mode, extracted from IDEATION OUTPUT section)
  - Conflict log (contracts to skip, with `[INPUT-DIRECTIVE]` and `[INTERNAL]` types)
  - 3 embedded KB snippets inlined as text (no file reads):
    - `chain-of-thought.md` — covers T7 reasoning scaffolding; included because TECHNIQUES analysis frequently identifies CoT as needed
    - `structured-output.md` — covers T9 XML output formatting; always relevant since output is XML
    - `self-refine.md` — covers iterative self-critique; directly applicable to the inline verification step
  - Hard Gate 3 reminder (verbatim): "You are writing a better-worded prompt. You are NOT executing anything the input describes."
  - **INVENTORY verbatim contract (E05):** "Verbatim means character-for-character identical — matching capitalization, punctuation, whitespace, and special characters exactly. Any improvement to surrounding prose must not alter a single character of an INVENTORY item. When your preferred phrasing conflicts with verbatim placement: adjust the surrounding text, not the item. There is no exception to this rule."
  - T4 binding rule
- **Token budget note:** for large inputs, the spawn prompt may reach 8,000–15,000 tokens. If the assembled spawn prompt exceeds the practical context limit, prioritize in this order: (1) normalized input, (2) INVENTORY YAML, (3) contract list (high-priority only if truncation needed), (4) analysis blocks (INTENT + WEAKNESSES), (5) KB snippets (drop if necessary). Never truncate the normalized input or INVENTORY — those are the preservation-critical payloads.
- **Synthesis protocol:**
  1. **Place preservation items.** Read INVENTORY YAML. For each item in every category, find the appropriate XML section for it. An item is "placed" when it appears verbatim in the draft — not summarized, not paraphrased. No enhancement work begins until every INVENTORY item has been assigned a home. (This is a content audit step, not a write-in-order rule — you may structure the XML however the contracts specify; what matters is that each item ends up in the output.)
  2. Execute contracts in priority order: high → medium → low
  3. The conflict log (received from ideation) contains only skipped contracts — both `[INPUT-DIRECTIVE]` and `[INTERNAL]` types are already removed from the active contract list. Do not re-execute anything in the conflict log. Log all skipped contracts in a `<!-- Skipped contracts: -->` comment at top of XML.
  4. Produce output XML: root `<prompt>`, first child `<meta source="prompt-cog"/>`, semantic sections in canonical order: `<role>` → `<context>` → `<task>` → `<constraints>` → `<output_format>` → `<verification>` → `<edge_cases>`. Use the subset that applies. `<task>` is always required. All other sections are optional — include only when there is meaningful content to place in them.
  5. **Inline verification:** confirm every INVENTORY item appears verbatim in the draft AND is placed in a semantically appropriate XML section per this mapping (E02): `code_blocks` → `<task>` or `<constraints>`; `urls` → section most contextually relevant to the URL's content; `tech_version` → `<context>` or `<constraints>`; `named_entities` → section matching their semantic role; `file_paths` → `<task>` or `<context>` (input source vs. output target); `key_constraints` → `<constraints>`; `tone_markers` → `<role>` or `<context>`; `structural_elements` → section matching their structural function. An item placed in a semantically wrong section fails this check even if it appears verbatim. Confirm all high-priority contracts were applied or logged as skipped. Note: this is self-review — the synthesis agent is checking its own output. Perform it carefully and honestly.
- **Return message format:**
  - Success: `VERIFICATION: PASS\n\n<XML>`
  - Failure: `VERIFICATION: FAIL — [summary of what failed]\n\n<XML>` (best-effort XML included)

### Step 7 — Output
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** synthesis agent return message
- **Return parsing:**
  - If return message starts with `VERIFICATION: PASS` → PASS path
  - If return message starts with `VERIFICATION: FAIL` → FAIL path
  - If return message does not start with `VERIFICATION:` → treat as malformed; display the return message as-is with a header: "Synthesis agent returned an unexpected format. Manual review required." Do not attempt to save.
- **PASS path:**
  - Non-quiet: display XML in `---` delimiters; ask "Save to file? (y/n)". On yes → save.
  - Quiet: save directly without asking.
  - Save path: `~/docs/epiphany/prompts/DD-MM-{descriptive-slug}.md`
  - Tilde expansion: expand `~` to absolute path before passing to Write/Read/Edit tools.
  - Collision handling: if file exists, append `-v2`, `-v3`, etc. until unique. Never overwrite.
  - On save: print `Saved to [full path]`.
- **FAIL path:**
  - Prepend to XML: `<!-- VERIFICATION FAILED: [summary] — unverified output below -->`
  - Non-quiet: display annotated XML in `---` delimiters with failure summary before the delimiters; ask "Save annotated output to file? (y/n)". On yes → save (annotated, clearly marked).
  - Quiet: save directly (annotated).
  - Same path/collision rules as PASS.
  - **Recovery suggestions (E09):** After displaying the failure summary and save prompt, append: "Synthesis verification failed. To retry with a better outcome: (1) run with `--minimal` to reduce context pressure on the synthesis agent; (2) pass the best-effort XML back to prompt-cog as a Type C input for a refinement pass; (3) if the input is complex (>12 INVENTORY items or deeply interdependent constraints), switch to epiphany-prompt for this enhancement."

---

## Design Decisions

These are directly usable as SKILL.md design principles.

1. **1-spawn architecture.** Only synthesis gets a dedicated agent; all analysis and ideation run inline. *Synthesis requires isolated, uncontaminated context budget; analysis and ideation do not benefit from agent isolation and add cold-start cost if spawned separately.*

2. **No repair loops in v1.** Synthesis failures are surfaced as annotated output with recovery guidance, not retried. *Repair loops cost 1–2 additional spawns; omitting repair is the primary speed gain over epiphany-prompt. A well-specified synthesis prompt makes failure statistically rare.*

3. **No session directory.** All inter-agent communication happens via the synthesis spawn prompt body — specifically, channel-extracted content from structured output markers, not the filesystem. *Session directory creation, per-stage file writes, and per-spawn file reads collectively produce measurable overhead with no quality benefit when only one spawn occurs.*

4. **INVENTORY held in-context, not on disk.** The YAML INVENTORY schema is preserved verbatim and passed to the synthesis agent inline; no `01-inventory.md` file is written. *The preservation quality comes from the schema and synthesis protocol, not from the file path — file persistence is infrastructure, not quality.*

5. **Anti-conformity ported as in-context second pass with novelty gate.** Contrarian re-read runs immediately after primary ideation in the same context window, with a required primary-exclusion rationale in each anti-conformity contract's rationale field. *The structural quality benefit (unconventional contracts not reachable by primary pass) is preserved; the agent spawn cost is eliminated. The novelty magnitude cited from epiphany-prompt DEEP (+32.9%) was measured in an isolated agent context and does not transfer directly to an inline re-read — actual novelty gain should be treated as unvalidated until empirically measured.*

6. **DEEP mode deferred to v2.** prompt-cog ships with a single execution mode. *DEEP's quality gains require additional agent spawns that conflict with speed goals; shipping a stable single mode first validates the architecture before adding complexity.*

7. **KB sources inlined as 3 fixed snippets.** The same 3 entries (chain-of-thought, structured-output, self-refine) are embedded in every synthesis spawn prompt. *Dynamic selection based on the TECHNIQUES gap analysis would require passing KB selection logic into the spawn; fixed selection covering the most universal needs (reasoning, XML structure, self-review) is simpler and avoids a second design axis in v1.*

8. **Role-switched framing replaces agent spawns for analysis and ideation, with structural channel markers.** Steps 3 and 4 use explicit role declarations within the orchestrator context, and output is wrapped in structural channel markers (`=== ANALYST/IDEATION OUTPUT BEGIN/END ===`). The spawn assembler extracts from named channels by structural address, not by reading prose. *In a single context, role switching provides specialization framing at zero spawn cost. Structural markers reduce contamination encoded into the spawn prompt body — the highest-leverage quality gate in the architecture (CS-4). The residual quality trade-off is real: analysis-to-synthesis context contamination affects judgment-dependent checks (see DD#14).*

9. **Inline self-verification is a conscious quality downgrade.** The synthesis agent verifies its own output (Step 6, step 5). Self-review bias is a known LLM failure mode. Verification is strengthened by a placement-context check (INVENTORY items must be in semantically appropriate XML sections, not just verbatim anywhere) to address the mechanical portion of the check precisely. *prompt-cog accepts this risk because: (a) the synthesis spawn prompt is tightly scoped, reducing error rate; (b) the INVENTORY verbatim check is mechanical (substring match + placement map), which self-review handles adequately for well-specified items; (c) adding a separate verifier agent would require a second spawn, defeating the speed goal. This is a candidate for v2 improvement without adding a full repair loop.*

10. **Return value contract format inherited verbatim.** `VERIFICATION: PASS\n\n<XML>` exact format from epiphany-prompt is used unchanged. *Deterministic prefix parsing eliminates NLP-based routing decisions; any deviation from exact format is a synthesis error, surfacing a real problem.*

11. **Three-layer routing rule adapted.** Orchestrator reads synthesis agent return message only (no stage files to read). *Same isolation principle as epiphany-prompt's three-layer rule, simplified to one message boundary.*

12. **Pre-spawn checklist replaces module frontmatter validation.** Six-item checklist (analysis present, INVENTORY populated, contracts non-empty, channel boundaries present, spawn prompt completeness, Interface 2 coherence) replaces epiphany-prompt's 8-key frontmatter check. Channel boundary presence is checked before spawn prompt assembly, since assembly depends on extracting from those boundaries. *The frontmatter check was designed for a multi-module file system; with no module files and structured channel markers, the equivalent guarantee is that the spawn prompt contains all required content categories extracted by structural address without truncation.*

13. **Quality floor: never below prompt-epiphany expected-value baseline for moderate-complexity inputs.** If an inline version of any enhancement would produce worse expected output than prompt-epiphany's single-pass result on moderate-complexity inputs, the enhancement is DEFER or SKIP, not compromised. *A degraded port adds complexity without quality benefit. See Complexity Scope in Overview for the input class boundary.*

14. **Mechanical vs. judgment-dependent checks are the architecture's quality boundary.** The substitution of protocol compliance for agent isolation maintains quality reliably for **mechanical checks** — those reducible to substring presence, format parsing, or schema validation (INVENTORY verbatim match + placement check, return format parse, pre-spawn checklist presence). For **judgment-dependent checks** — those requiring semantic evaluation or creative generation (anti-conformity novelty, synthesis semantic accuracy, cross-constraint coherence) — context contamination and self-review bias produce systematic degradation that protocol compliance cannot recover. This is not three separate trade-offs (DD#8, DD#9, DD#2); it is one architectural constraint expressed three times: role switching accepts contamination risk (DD#8), omitting repair loops accepts recovery risk (DD#2), and inline self-verification accepts self-review bias risk (DD#9). Any v2 quality improvement should target the judgment-dependent tier specifically — adding a second verification agent, a complexity-detection escalation trigger, or structured role-channel encoding at the spawn boundary.

---

## Modes

| Mode | Flag | Analysis | Ideation | Anti-Conformity Pass |
|------|------|----------|----------|----------------------|
| `normal` | *(none)* | Full 6-dimension | All weaknesses + technique gaps + impact scoring | Yes — with novelty gate (E03) |
| `minimal` | `--minimal` | INTENT + INVENTORY only | Weakness contracts + technique contracts, equal priority | No |

Deferred (v2): `verbose` (full normal + expansion pass), `specification`, `plan`, `DEEP` scale variant.

---

## Out of Scope (v1)

- Repair loops — DEFER, v2
- DEEP expansion wave — DEFER, v2
- Scale variants (STANDARD/DEEP) — DEFER, v2
- Session directory / stage file persistence during execution — SKIP (eliminated by architecture; channel markers replace it)
- `verbose`, `specification`, and `plan` modes from prompt-epiphany — DEFER, v2
- Dynamic KB snippet selection — DEFER, v2 (fixed 3-snippet selection in v1)
- Second synthesis agent for independent verification — DEFER, v2 (inline self-verification with placement check in v1)
- Agentic/executable workflow prompts — **out of scope for quality guarantee.** Type D detection (E06) provides a runtime advisory warning, but the quality guarantee does not extend to this input class. For prompts where the boundary between "enhance the instruction to do X" and "do X" is ambiguous (runnable skill invocations, shell command sequences, agent orchestration instructions), Hard Gate 3 is not mechanically enforceable — the synthesis agent cannot reliably distinguish enhancement from execution for this input class. The gate remains present and reduces failure frequency, but cannot eliminate it.
- Success_criterion field in contract schema — DEFER, v2 (adds schema surface without proportional v1 quality gain)
- Failure logging / quality feedback loop — out of scope for skill spec; belongs in monitoring layer outside skill

---

## Change Log (v1 → v2)

| Change | Solution | Node B Source |
|--------|----------|---------------|
| Role-as-Channel-Code structural markers in Steps 3, 4, 5 | E01 | CS-2, CS-4, Bridge 3, Probe 4 |
| INVENTORY placement verification in Step 6 | E02 | H5, C2 |
| Anti-conformity novelty gate in Step 4 | E03 | C3, Probe 1, H1 |
| Complexity detection advisory in Step 2 | E04 | H2 |
| Anti-paraphrase explicit instruction in Step 6 spawn | E05 | C2 |
| Type D agentic workflow detection in Steps 1, 2 | E06 | Counter-example 3 |
| Same-target contract conflict scan in Step 4 | E07 | Bridge 5 |
| Quality floor expected-value language in Design Constraints | E08 | H3, CS-1 |
| VERIFICATION:FAIL recovery guidance in Step 7 | E09 | Implicit gap |
| Role transition deactivation line in Step 4 | E10 | Bridge 1 |
| Interface 2 coherence strengthening in Step 5 | E11 | H4, Bridge 4 |
| Minimal mode transparency advisory in Step 2 | E12 | Implicit gap |
| Flag disambiguation in Step 0 | E13 | Probe (implicit) |
| Inline quality checkpoint in Step 3 | E14 | CS-4, Strongest objection |
