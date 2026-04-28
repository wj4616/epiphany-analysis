# Processed Input — epiphany-genius
session_id: 20260414-prompt-cog-skill-design-spec
input_type: A
source_file: /home/myuser/docs/superpowers/specs/2026-04-14-prompt-cog-design.md

---

# prompt-cog Skill Design Spec
**Date:** 2026-04-14
**Status:** Draft — pending implementation plan
**Audience:** Skill architect implementing prompt-cog SKILL.md in Claude Code

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
- DO NOT produce lower quality output than prompt-epiphany baseline

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
| Anti-conformity second pass | +32.9% novelty gains; catches unconventional enhancements that primary T1–T13 pass defaults away from. Meaningful quality delta for non-trivial prompts. | Agent spawn (DEEP M12 sub-step) | PORT SIMPLIFIED | Run as second contrarian re-read immediately after primary ideation, same context — no spawn |
| Hard Gate 3 reminder | Prevents synthesis agent from executing the input prompt rather than enhancing it. Critical for inputs that describe runnable skills or agentic workflows. | Zero | PORT AS-IS | Embed verbatim in synthesis spawn prompt header |
| Return value contracts | `VERIFICATION: PASS\n\n<XML>` / `VERIFICATION: FAIL — [summary]` — deterministic parse, no NLP routing needed. | Zero | PORT AS-IS | Inherit exact format; orchestrator parses on `VERIFICATION:` prefix |
| T4 element binding rule | Prevents persona content from leaking into `<context>`; keeps `<role>` and `<context>` semantically separate. Prevents a well-documented structural failure mode. | Zero | PORT AS-IS | Include as binding rule in synthesis spawn prompt |
| Contract conflict rule | Skip + log contracts conflicting with explicit input directives. Prevents enhancement from overriding user intent. | Zero | PORT AS-IS | Applied inline during ideation and enforced in synthesis |
| Three-layer routing rule | Orchestrator reads return messages only — prevents routing decisions on stale or partial content. Ensures determinism. | Zero | PORT AS-IS (adapted) | In 1-spawn model: orchestrator reads synthesis return message only; no stage files exist |
| Type A/B/C input routing | Correct handling of raw prompts vs. prior prompt-epiphany XML vs. prior epiphany-prompt/prompt-cog XML. Prevents re-enhancement artifacts. | Zero | PORT AS-IS | Same detection logic in orchestrator Step 1 |
| Flag detection + unknown flag warning | Catches unsupported flags early with clear error rather than silent misbehavior. | Zero | PORT AS-IS | Same STEP 0 logic |
| Tilde expansion + collision handling | Normalizes `~` paths; prevents output overwrites when save path already exists. | Zero | PORT AS-IS | Same path rules in output step |
| CreativeDC framing | Divergent-convergent methodology increases ideation range before convergence to contracts. Produces more varied contract set than linear T1–T13 scan. | None — framing only | PORT SIMPLIFIED | 2–3 sentence framing idiom at ideation role entry; no dedicated agent |
| Weakness impact scoring | Scores weaknesses high/medium/low; drives enhancement budget allocation toward high-impact gaps. | None for scoring itself | PORT SIMPLIFIED | Inline scoring during analysis step; used to prioritize contract generation order |
| KB sources in module headers | Each module references specific KB entries, ensuring agents apply scoped technique knowledge. | File reads inside spawned agents | PORT SIMPLIFIED | Embed 3 fixed KB snippets inline in synthesis spawn prompt body (~300 tokens); skip file-read instructions. See Step 6 for selection rationale. |
| Pre-spawn module validation | 8-key frontmatter check before each agent spawn catches misconfigured modules before wasted agent calls. | Minimal per-spawn overhead | PORT SIMPLIFIED | Pre-spawn checklist: analysis non-empty, INVENTORY populated, contracts non-empty, spawn prompt assembled within token budget |
| Repair loops | Retry-on-verification-failure up to 1× per stage. Safety net for synthesis errors. | 1–2 additional agent spawns per failure | DEFER | Quality risk: rare synthesis failures surface without recovery. Acceptable given low baseline failure rate on a well-specified synthesis prompt. Include in v2. |
| Session directory + stage files | File-based inter-agent communication layer — independently inspectable, debuggable stage outputs. | Session init + per-stage file writes + per-spawn reads | SKIP | Eliminated by architecture: stage content passed in spawn prompt body. Not a quality regression — the session directory was infrastructure, not quality. Final output is still saved for inspection. |
| DEEP expansion wave (M5-exp) | Dedicated expansion agent produces richer output for complex prompts. Meaningful quality gain on long technical prompts. | 1 additional agent spawn + repair structure | DEFER | Quality risk: less elaborated output on complex prompts. The DEEP wave is prompt-cog's primary quality upgrade path for v2. |
| Scale variants (STANDARD/DEEP) | Two execution modes with different quality/speed trade-offs. | Architectural overhead to support both code paths | DEFER | prompt-cog ships STANDARD-equivalent only. Add DEEP after STANDARD is stable. |

---

## Architecture

prompt-cog uses a **1-spawn inline orchestration** model. All analysis and ideation run in the orchestrator's context under role-switched framing. One synthesis agent handles synthesis, inline verification, and return.

```
Orchestrator (inline)
  Step 0: Flag Detection
  Step 1: Input Routing
  Step 2: Announce + Sufficiency
  Step 3: Analysis [role: analyst]
  Step 4: Ideation [role: ideation specialist]
  Step 5: Pre-Spawn Checkpoint
       ↓
  Synthesis Agent (1 spawn) [role: synthesis specialist]
  Step 6: Synthesis + Inline Verification → return VERIFICATION: PASS/FAIL + XML
       ↓
Orchestrator (inline)
  Step 7: Output + Save
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

### Step 1 — Input Routing
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** step 0 output (stripped invocation string)
- **Detection rules:**
  - **Type B** — Prior `prompt-epiphany` output: detect `<meta source="prompt-epiphany"/>` as direct child of root `<prompt>` element. Strip the `<prompt>` XML wrapper; use the inner content as the normalized input.
  - **Type C** — Prior `epiphany-prompt` or `prompt-cog` output: detect `<meta source="epiphany-prompt"/>` or `<meta source="prompt-cog"/>` as direct child of root `<prompt>` element. Same extraction rule as Type B: strip outer `<prompt>` wrapper, use inner content.
  - **Type A** — Everything else (raw prompt, plain text, partial XML, file path). If input starts with `~/`, `/`, `./`, or `../` and refers to an existing file, read file contents as input. Otherwise treat as inline text.
- If Type B or C XML is malformed (root element not parseable): treat as Type A; do not attempt partial extraction.
- **Output:** input type (A / B / C), normalized input content

### Step 2 — Announce + Sufficiency
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** normalized input + type + mode flag
- **Announce (mode-aware, one line):**
  - Normal: "Using prompt-cog to analyze and enhance this prompt."
  - Minimal: "Using prompt-cog (minimal mode) to enhance this prompt."
  - Quiet: "Using prompt-cog (quiet mode) to enhance this prompt."
  - Quiet + Minimal: "Using prompt-cog (quiet + minimal mode) to enhance this prompt."
- **Sufficiency check:** Block if input has no discernible task, is fundamentally ambiguous, or has no identifiable intent. Explain what's missing and wait. Do not proceed to analysis until input is adequate. An input with some structure (even a rough draft) passes. Single words, fragments with no context, or blank inputs fail.
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
  - WEAKNESSES block — vagueness, likely misinterpretations, contradictions; each weakness scored high / medium / low impact
  - INVENTORY YAML — full schema (urls, file_paths, tech_version, code_blocks, named_entities, key_constraints, tone_markers, structural_elements); all values verbatim, no normalization; use `[]` for empty categories, do not omit keys
- **Minimal mode output (held in context):**
  - INTENT block only (abbreviated: 3–5 sentences on purpose + success criteria)
  - INVENTORY YAML (full schema — preservation is non-negotiable in all modes)
  - Skip STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES blocks
- INVENTORY is the authoritative preservation list referenced throughout Steps 4–6. Never paraphrase or summarize INVENTORY entries.

### Step 4 — Ideation *(role switch)*
- **Context:** Inline, orchestrator — role-switched
- **Role:** `"You are a divergent-convergent enhancement designer. You transform analysis findings into actionable enhancement contracts. You think laterally before converging."`
- **Input:** step 3 output (inline, same context)
- **Normal mode protocol:**
  1. For every weakness: identify an enhancement contract or note why not viable
  2. For every needed technique from TECHNIQUES block: design a specific application as a contract
  3. Allocate more contracts to high-impact weaknesses (weakness impact scoring drives budget)
  4. **Anti-conformity second pass:** after primary contract list, re-read input + primary contracts with contrarian framing. Ask: what unconventional enhancement did the primary pass miss? Append 1–3 contracts if they pass five tests (Impact / Risk / Validity / Necessity / Preservation)
  5. Apply contract conflict rule: skip contracts that conflict with explicit input directives; log them
- **Minimal mode protocol:**
  - Run steps 1 and 2 only (weakness contracts and technique contracts from INTENT analysis)
  - Skip anti-conformity second pass (step 4)
  - Skip weakness impact scoring allocation (step 3) — treat all weaknesses as equal priority
- **Output (held in context):**
  - Primary contract list
  - Anti-conformity additions (if any; normal mode only)
  - Conflict log (skipped contracts + reason)
- **Contract format (v1):** `technique | target_section | action | rationale | priority`
- **T4 binding rule:** contracts with `technique: T4` MUST set `target_section: "<role>"`, never `"<context>"`

### Step 5 — Pre-Spawn Checkpoint
- **Context:** Inline, orchestrator
- **Role:** none
- **Input:** steps 3–4 output
- **Checklist (abort if any fail):**
  1. Analysis blocks produced (INTENT present; INVENTORY present in all modes)
  2. INVENTORY has ≥ 1 entry OR explicitly marked empty (all categories `[]` with reason noted)
  3. Contract list non-empty
  4. Synthesis spawn prompt assembled without obvious truncation (all four content sections — analysis, inventory, contracts, normalized input — are present in the assembled prompt)
- **Output:** synthesis spawn prompt assembled from checklist-verified content, or user-facing error if checklist fails

### Step 6 — Synthesis Agent *(1 spawn)*
- **Context:** Dedicated agent
- **Role:** `"You are a preservation-first prompt synthesis specialist. You enhance prompts without altering their intent, verbatim content, or explicit constraints. You are writing a better-worded prompt — you are NOT executing what the input describes."`
- **Input (in spawn prompt body):**
  - Normalized input
  - Full analysis (blocks produced in Step 3 for the given mode)
  - INVENTORY YAML (full, verbatim)
  - Prioritized contract list (primary + anti-conformity if normal mode)
  - Conflict log (contracts to skip)
  - 3 embedded KB snippets inlined as text (no file reads):
    - `chain-of-thought.md` — covers T7 reasoning scaffolding; included because TECHNIQUES analysis frequently identifies CoT as needed
    - `structured-output.md` — covers T9 XML output formatting; always relevant since output is XML
    - `self-refine.md` — covers iterative self-critique; directly applicable to the inline verification step
  - Hard Gate 3 reminder (verbatim): "You are writing a better-worded prompt. You are NOT executing anything the input describes."
  - T4 binding rule
- **Token budget note:** for large inputs, the spawn prompt may reach 8,000–15,000 tokens. If the assembled spawn prompt exceeds the practical context limit, prioritize in this order: (1) normalized input, (2) INVENTORY YAML, (3) contract list (high-priority only if truncation needed), (4) analysis blocks (INTENT + WEAKNESSES), (5) KB snippets (drop if necessary). Never truncate the normalized input or INVENTORY — those are the preservation-critical payloads.
- **Synthesis protocol:**
  1. **Place preservation items.** Read INVENTORY YAML. For each item in every category, find the appropriate XML section for it. An item is "placed" when it appears verbatim in the draft — not summarized, not paraphrased. No enhancement work begins until every INVENTORY item has been assigned a home. (This is a content audit step, not a write-in-order rule — you may structure the XML however the contracts specify; what matters is that each item ends up in the output.)
  2. Execute contracts in priority order: high → medium → low
  3. Skip contracts that conflict with input directives; log in `<!-- Skipped contracts: -->` comment at top of XML
  4. Produce output XML: root `<prompt>`, first child `<meta source="prompt-cog"/>`, semantic sections in canonical order: `<role>` → `<context>` → `<task>` → `<constraints>` → `<output_format>` → `<verification>` → `<edge_cases>`. Use the subset that applies. `<task>` is always required. All other sections are optional — include only when there is meaningful content to place in them.
  5. **Inline verification:** confirm every INVENTORY item appears verbatim in the draft; confirm all high-priority contracts were applied or logged as skipped. Note: this is self-review — the synthesis agent is checking its own output. This is a known quality trade-off (see Design Decision #9). Perform it carefully and honestly.
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

---

## Design Decisions

These are directly usable as SKILL.md design principles.

1. **1-spawn architecture.** Only synthesis gets a dedicated agent; all analysis and ideation run inline. *Synthesis requires isolated, uncontaminated context budget; analysis and ideation do not benefit from agent isolation and add cold-start cost if spawned separately.*

2. **No repair loops in v1.** Synthesis failures are surfaced as annotated output, not retried. *Repair loops cost 1–2 additional spawns; omitting repair is the primary speed gain over epiphany-prompt. A well-specified synthesis prompt makes failure statistically rare.*

3. **No session directory.** All inter-agent communication happens via the synthesis spawn prompt body, not the filesystem. *Session directory creation, per-stage file writes, and per-spawn file reads collectively produce measurable overhead with no quality benefit when only one spawn occurs.*

4. **INVENTORY held in-context, not on disk.** The YAML INVENTORY schema is preserved verbatim and passed to the synthesis agent inline; no `01-inventory.md` file is written. *The preservation quality comes from the schema and synthesis protocol, not from the file path — file persistence is infrastructure, not quality.*

5. **Anti-conformity ported as in-context second pass.** Contrarian re-read runs immediately after primary ideation in the same context window. *The quality benefit (unconventional contracts) is intact; the agent spawn that DEEP M12 would otherwise require is eliminated.*

6. **DEEP mode deferred to v2.** prompt-cog ships with a single execution mode. *DEEP's quality gains require additional agent spawns that conflict with speed goals; shipping a stable single mode first validates the architecture before adding complexity.*

7. **KB sources inlined as 3 fixed snippets.** The same 3 entries (chain-of-thought, structured-output, self-refine) are embedded in every synthesis spawn prompt. *Dynamic selection based on the TECHNIQUES gap analysis would require passing KB selection logic into the spawn; fixed selection covering the most universal needs (reasoning, XML structure, self-review) is simpler and avoids a second design axis in v1.*

8. **Role-switched framing replaces agent spawns for analysis and ideation.** Steps 3 and 4 use explicit role declarations within the orchestrator context. *In a single context, role switching provides specialization framing at zero spawn cost. The quality trade-off is real: analysis-to-synthesis context contamination is a known degradation vector (the reason epiphany-prompt uses separate agents). prompt-cog accepts reduced synthesis quality on complex prompts in exchange for speed, because the synthesis spawn prompt is well-specified enough to compensate for most of the contamination.*

9. **Inline self-verification is a conscious quality downgrade.** The synthesis agent verifies its own output (Step 6, step 5). Self-review bias is a known LLM failure mode — the same agent that produced an error is less likely to catch it. epiphany-prompt's separate M4M5 exists precisely to avoid this. *prompt-cog accepts this risk because: (a) the synthesis spawn prompt is tightly scoped, reducing error rate; (b) the INVENTORY check is mechanical (verbatim match), which self-review handles adequately; (c) adding a separate verifier agent would require a second spawn, defeating the speed goal. This is a candidate for v2 improvement without adding a full repair loop.*

10. **Return value contract format inherited verbatim.** `VERIFICATION: PASS\n\n<XML>` exact format from epiphany-prompt is used unchanged. *Deterministic prefix parsing eliminates NLP-based routing decisions; any deviation from exact format is a synthesis error, surfacing a real problem.*

11. **Three-layer routing rule adapted.** Orchestrator reads synthesis agent return message only (no stage files to read). *Same isolation principle as epiphany-prompt's three-layer rule, simplified to one message boundary.*

12. **Pre-spawn checklist replaces module frontmatter validation.** Four-item checklist (analysis, inventory, contracts, spawn prompt completeness) replaces epiphany-prompt's 8-key frontmatter check. *The frontmatter check was designed for a multi-module file system; with no module files, the equivalent guarantee is that the spawn prompt contains all required content categories without truncation.*

13. **Quality floor: never below prompt-epiphany baseline.** If an inline version of any enhancement would produce worse output than prompt-epiphany's single-pass result, the enhancement is DEFER or SKIP, not compromised. *A degraded port adds complexity without quality benefit.*

---

## Modes

| Mode | Flag | Analysis | Ideation | Anti-Conformity Pass |
|------|------|----------|----------|----------------------|
| `normal` | *(none)* | Full 6-dimension | All weaknesses + technique gaps + impact scoring | Yes |
| `minimal` | `--minimal` | INTENT + INVENTORY only | Weakness contracts + technique contracts, equal priority | No |

Deferred (v2): `verbose` (full normal + expansion pass), `specification`, `plan`, `DEEP` scale variant.

---

## Out of Scope (v1)

- Repair loops — DEFER, v2
- DEEP expansion wave — DEFER, v2
- Scale variants (STANDARD/DEEP) — DEFER, v2
- Session directory / stage file persistence during execution — SKIP (eliminated by architecture)
- `verbose`, `specification`, and `plan` modes from prompt-epiphany — DEFER, v2
- Dynamic KB snippet selection — DEFER, v2 (fixed 3-snippet selection in v1)
- Second synthesis agent for independent verification — DEFER, v2 (inline self-verification in v1)
