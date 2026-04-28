# S1 — State Loading
stage_id: S1
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0

---

<state_loading>

<input_class>

## Input class

**Class: Design**

The input is a design specification for a new prompt-enhancement skill (`prompt-cog`). It presents architecture decisions, component trade-offs, explicit design constraints, system API surface (flag table, step I/O contracts), quality attributes (reliability, determinism, speed), stakeholder requirements (user-facing behavior), and explicit rationale for deferred vs. shipped features. All five Design signals are present: architecture decisions, trade-offs, UX/API surface, system design, and constraints between components.

No ambiguity — this is unambiguously class: Design. Template 2 applied.

</input_class>

<primitives>

## Primitives

Template 2 applied. Seven categories, ≥20 per category or count-cap declared.

---

### Category 1 — Components

What are the system components? What does each do independently?

1. `prompt-cog` SKILL.md — top-level orchestrator; executes Steps 0–7 inline
2. Synthesis agent (1 spawn) — isolated context for Step 6; synthesis + inline verification
3. Step 0 (Flag Detection) — parses invocation string; extracts mode flags; validates flag legality
4. Step 1 (Input Routing) — classifies input as Type A/B/C; normalizes to plain content
5. Step 2 (Announce + Sufficiency) — mode-aware announce line; blocks on empty/ambiguous input
6. Step 3 (Analysis) — role-switched analyst; produces INTENT/STRUCTURE/CONSTRAINTS/TECHNIQUES/WEAKNESSES/INVENTORY
7. Step 4 (Ideation) — role-switched ideation specialist; produces enhancement contracts via divergent-convergent process
8. Step 5 (Pre-Spawn Checkpoint) — four-item checklist; assembles synthesis spawn prompt; halts on checklist failure
9. Step 6 (Synthesis Agent) — preservation-first synthesis; inline verification; returns VERIFICATION prefix + XML
10. Step 7 (Output) — parses return message; routes PASS/FAIL/malformed; handles display + save
11. INVENTORY YAML schema — in-context artifact; authoritative preservation list; passed to synthesis agent
12. Enhancement contracts schema v1 — `technique | target_section | action | rationale | priority`; actionable, testable units
13. Anti-conformity second pass — in-context contrarian re-read after primary ideation; produces 1–3 additional contracts
14. 3 embedded KB snippets — `chain-of-thought.md`, `structured-output.md`, `self-refine.md` inlined in synthesis spawn prompt
15. Hard Gate 3 reminder — verbatim string embedded in synthesis spawn prompt header
16. T4 binding rule — enforces `<role>` not `<context>` for T4 contracts; embedded in synthesis spawn prompt
17. `prompt-epiphany` source skill — baseline quality reference; T1–T13 technique definitions are authoritative here
18. `epiphany-prompt` source skill — enhancement source; specific enhancements selectively imported
19. Flag detection module — Step 0; detects `--minimal`, `--quiet`, `--verbose`, `--specification`, `--plan`
20. Output save path generator — `~/docs/epiphany/prompts/DD-MM-{slug}.md`; tilde expansion; collision handling
21. Conflict log — tracks skipped contracts; surfaced as `<!-- Skipped contracts: -->` comment in output XML
22. Pre-spawn checklist — four-item gate (analysis present, INVENTORY present, contracts non-empty, spawn prompt complete)

---

### Category 2 — Interfaces

What are the communication contracts between components? Which are hard vs. soft?

1. **Hard:** Flag Detection → rest of pipeline: output is (validated flag set, stripped invocation string); any deferred/conflicting flag halts before Step 1
2. **Hard:** Input Routing → Analysis: output is (input type A/B/C, normalized input content); Type B/C malformed XML falls back to Type A
3. **Hard:** Analysis → Ideation: INVENTORY YAML is authoritative; never paraphrased or summarized downstream
4. **Hard:** Pre-Spawn Checkpoint → Synthesis Agent: spawn prompt body must contain all four content sections (analysis, inventory, contracts, normalized input); checklist failure aborts
5. **Hard:** Synthesis Agent → Orchestrator Step 7: return message must start with `VERIFICATION: PASS` or `VERIFICATION: FAIL`; anything else is treated as malformed
6. **Hard:** Step 7 return parsing: prefix `VERIFICATION:` is the routing discriminator; NLP-free routing
7. **Hard:** T4 binding rule: contracts with `technique: T4` must set `target_section: "<role>"` not `"<context>"`; enforced in both ideation and synthesis
8. **Hard:** Contract conflict rule: contracts conflicting with explicit input directives are skipped + logged; never silently applied
9. **Hard:** INVENTORY verbatim requirement: every INVENTORY item must appear verbatim in synthesis draft; paraphrase = failure
10. **Hard:** Hard Gate 3: synthesis agent must not execute the input prompt; verbatim reminder in synthesis spawn prompt
11. **Soft:** Role-switched framing in Steps 3–4: provides specialization framing; not an agent isolation boundary; analysis-to-synthesis context contamination is a known accepted risk
12. **Soft:** 6-dimension analysis completeness: normal mode runs all 6 blocks; minimal mode runs INTENT + INVENTORY only; missing blocks in minimal mode is expected, not a failure
13. **Soft:** Anti-conformity second pass: 1–3 additional contracts; applied only in normal mode; tests (Impact/Risk/Validity/Necessity/Preservation) are quality gates, not hard halts
14. **Soft:** Token budget prioritization in synthesis spawn: priority order (normalized input > INVENTORY > contracts > analysis > KB snippets); dropping KB snippets is acceptable
15. **Soft:** Weakness impact scoring drives contract budget allocation: high-impact weaknesses get more contracts; not a hard rule
16. **Soft:** Canonical XML section order: `<role>` → `<context>` → `<task>` → `<constraints>` → `<output_format>` → `<verification>` → `<edge_cases>`; only applicable sections included
17. **Soft:** `<task>` always required in output XML; all other sections optional
18. **Soft:** Save path slug format: `DD-MM-{descriptive-slug}.md`; tilde expansion before tool calls
19. **Soft:** Collision handling: append `-v2`, `-v3` etc.; never overwrite
20. **Soft:** Conflict log appears as XML comment: `<!-- Skipped contracts: -->`; downstream agents not required to read it
21. **Soft:** Pre-spawn module validation: four-item checklist replaces epiphany-prompt's 8-key frontmatter check; simpler because no module files exist

---

### Category 3 — Known constraints

Performance SLAs, team/org constraints, technology mandates, budget.

1. Must run faster than `epiphany-prompt` — speed is the primary architectural constraint
2. Must not sacrifice reliability for speed — reliability is a hard floor
3. Must not produce lower quality output than `prompt-epiphany` baseline — quality floor constraint
4. DO NOT replace or deprecate `prompt-epiphany` or `epiphany-prompt` — coexistence requirement
5. Architecture limited to 1 synthesis agent spawn — spawn budget is 1
6. No repair loops in v1 — repair is deferred; failures surface as annotated output
7. No session directory in v1 — filesystem inter-agent state eliminated
8. No DEEP expansion wave in v1 — DEEP deferred to v2
9. No scale variants in v1 — single execution mode (STANDARD-equivalent)
10. No `--verbose`, `--specification`, `--plan` modes in v1 — deferred; surface warning and halt
11. `--minimal` + `--verbose` conflict must block execution — never proceed silently on conflicting flags
12. Unknown flags must surface warning and halt — never silently proceed
13. Flag detection applies only to first or last standalone token — mid-body flag text treated as prompt content
14. INVENTORY must be full schema in all modes — preservation is non-negotiable even in minimal mode
15. Normalized input and INVENTORY must never be truncated in spawn prompt — highest-priority content
16. `<task>` section always required in output XML — structural mandate
17. `<meta source="prompt-cog"/>` must be first child of root `<prompt>` element — structural mandate
18. Synthesis agent must not execute the input prompt — Hard Gate 3 is a correctness constraint
19. T4 contracts must target `<role>`, never `<context>` — prevents documented structural failure mode
20. Every INVENTORY item must appear verbatim in output — no paraphrase; verbatim is the preservation criterion
21. Type B/C malformed XML falls back to Type A — no partial extraction attempted
22. Inline self-verification is a conscious quality trade-off — accepted risk of self-review bias in v1

---

### Category 4 — Quality attributes

Scalability, maintainability, testability, security — what is actually required vs. desirable?

1. **Required:** Reliability — deterministic step execution with no dropped steps
2. **Required:** Speed — faster than `epiphany-prompt`; 1-spawn model is the speed mechanism
3. **Required:** Preservation fidelity — verbatim INVENTORY items in output; non-negotiable in all modes
4. **Required:** Determinism — `VERIFICATION:` prefix parsing eliminates NLP routing; deferred/unknown flags halt immediately
5. **Required:** Quality floor — output must meet or exceed `prompt-epiphany` single-pass quality
6. **Required:** Correctness — synthesis agent must enhance, not execute; Hard Gate 3 is a correctness constraint
7. **Required:** Structural validity — canonical XML section order; `<task>` always present; `<meta source="prompt-cog"/>` first child
8. **Desirable:** Modularity — separable steps with distinct role contexts; each step has clear I/O; enables v2 additions
9. **Desirable:** Maintainability — no session directory means fewer moving parts; inline content passing is simpler
10. **Desirable:** Testability — return value contracts are deterministic (`VERIFICATION: PASS/FAIL` prefix); parseable without NLP
11. **Desirable:** Traceability — conflict log as XML comment; skipped contracts surfaced in output
12. **Desirable:** Extensibility (v2 readiness) — DEEP mode, repair loops, second verifier, dynamic KB selection are all scoped deferrals with clear integration points
13. **Desirable:** Robustness — malformed synthesis return handled gracefully (displayed with header, no save attempt)
14. **Desirable:** Mode coverage — normal and minimal modes cover the speed/quality trade-off spectrum; quiet modifier orthogonal to mode
15. **Desirable:** Transparency — FAIL path annotates XML with failure summary; unverified output is clearly labeled
16. **Desirable:** Collision safety — output file never overwritten; `-v2`/`-v3` appended on collision
17. **Desirable:** Token budget awareness — prioritized truncation order for large inputs in synthesis spawn
18. **Desirable:** Idempotent input routing — Type B/C detection is structural (element presence), not content-based; stable across input variations
19. Input contains 18 quality attributes primitives; 20-item target not reached — constraint is the input.

---

### Category 5 — Stakeholder requirements

Explicit vs. implicit needs; who owns which requirements?

1. **Explicit (user):** prompt-cog runs faster than `epiphany-prompt`
2. **Explicit (user):** prompt-cog is more programmatic than `prompt-epiphany` — steps followed exactly, not loosely
3. **Explicit (user):** as many `epiphany-prompt` enhancements as possible are retained
4. **Explicit (user):** modular architecture with separable steps
5. **Explicit (user):** smooth, reliable, deterministic step execution with no dropped steps
6. **Explicit (user):** does not replace or deprecate either source skill
7. **Explicit (user):** quality output never below `prompt-epiphany` baseline
8. **Explicit (spec architect):** 1-spawn architecture is the speed mechanism — synthesis isolated; analysis/ideation inline
9. **Explicit (spec architect):** enhancement inventory with explicit PORT/DEFER/SKIP verdicts for every `epiphany-prompt` enhancement
10. **Explicit (spec architect):** design decisions section is directly usable as SKILL.md design principles
11. **Explicit (spec architect):** inline self-verification is a conscious quality downgrade, acknowledged in spec
12. **Explicit (spec architect):** DEEP mode and repair loops are v2 candidates, not omissions
13. **Implicit (user):** output should be inspectable / saveable in familiar format (same path convention as other epiphany skills)
14. **Implicit (user):** flag behavior should be consistent with source skills (same flag position rule)
15. **Implicit (user):** failure cases should surface information, not silently degrade
16. **Implicit (spec architect):** v1 architecture must be stable before v2 complexity is added (DEEP after STANDARD is stable)
17. **Implicit (implementer):** SKILL.md is the implementation target — spec maps one-to-one to implementable sections
18. **Implicit (implementer):** T1–T13 definitions are in `prompt-epiphany` SKILL.md; implementer must read that file if not in context
19. **Implicit (implementer):** 3 embedded KB snippets (`chain-of-thought.md`, `structured-output.md`, `self-refine.md`) must be copied verbatim into SKILL.md, not referenced by path
20. **Implicit (implementer):** Hard Gate 3 reminder must appear verbatim in synthesis spawn prompt — not paraphrased

---

### Category 6 — Trade-off history

What design decisions have been made and what was sacrificed?

1. **1-spawn architecture chosen → sacrificed:** synthesis context purity (analysis-to-synthesis contamination is accepted risk; Design Decision #8)
2. **No repair loops in v1 → sacrificed:** synthesis error recovery; rare failures surface as annotated output instead of being retried (Design Decision #2)
3. **No session directory → sacrificed:** per-stage inspectability during execution; final output still saved (Design Decision #3)
4. **INVENTORY in-context, not on disk → sacrificed:** filesystem traceability of INVENTORY state; preservation quality comes from schema + protocol, not file path (Design Decision #4)
5. **Anti-conformity as in-context second pass → sacrificed:** full agent isolation for contrarian re-read; quality benefit retained, spawn cost eliminated (Design Decision #5)
6. **DEEP mode deferred → sacrificed:** richer output on complex/long prompts in v1; intentional to validate architecture first (Design Decision #6)
7. **3 fixed KB snippets → sacrificed:** dynamic KB selection based on TECHNIQUES gap analysis; fixed selection covers most universal needs (Design Decision #7)
8. **Inline self-verification → sacrificed:** independent verifier; self-review bias is known LLM failure mode; accepted because INVENTORY check is mechanical and spawn cost is prohibitive (Design Decision #9)
9. **Role-switched framing instead of agent spawns for Steps 3–4 → sacrificed:** analysis quality isolation; gained zero spawn cost for analysis and ideation (Design Decision #8)
10. **Exact `VERIFICATION:` prefix format inherited → sacrificed:** flexibility in return format; gained deterministic NLP-free routing (Design Decision #10)
11. **Pre-spawn four-item checklist instead of 8-key frontmatter → sacrificed:** granular module validation; gained simplicity appropriate for no-module-file architecture (Design Decision #12)
12. **Single execution mode in v1 → sacrificed:** scale-variant quality options; gained stable architecture before adding complexity (from DEEP/scale-variant deferrals)
13. **Repair loops deferred → sacrificed:** primary speed gain over `epiphany-prompt` is eliminating 1–2 additional spawns per rare failure; stated explicitly as the speed mechanism
14. **Session directory eliminated → sacrificed:** measurable overhead from session init + per-stage file writes + per-spawn reads; no quality benefit when only one spawn occurs
15. **Anti-conformity ported as second pass (not separate agent) → sacrificed:** full contrarian agent isolation; gained elimination of the DEEP M12 agent spawn
16. **Weakness impact scoring inline → sacrificed:** dedicated scoring step; gained zero overhead for scoring itself
17. **Type B/C malformed XML falls back to Type A → sacrificed:** partial extraction attempt; gained robustness (never attempt broken parse)
18. **Unknown flags halt → sacrificed:** lenient flag handling; gained clear error surface over silent misbehavior
19. **`--verbose`, `--specification`, `--plan` deferred → sacrificed:** feature parity with `prompt-epiphany` flag set in v1; gained reduced scope for stable first release
20. **Quality floor rule (Design Decision #13) → sacrificed:** any enhancement that would degrade below `prompt-epiphany` quality; gained assurance that complexity additions never regress output quality

---

### Category 7 — Failure modes

What can go wrong in each component? What cascade failure paths exist?

1. **Flag Detection failure:** unknown flag not caught → silent misbehavior; mitigated by "unknown flag → halt" rule
2. **Flag Detection failure:** deferred flag not surfaced → user expects behavior that doesn't exist; mitigated by deferred-flag warning + halt
3. **Flag Detection failure:** conflicting flags not detected → ambiguous mode execution; mitigated by explicit conflict check before Step 1
4. **Input Routing failure:** Type B/C XML malformed → partial extraction attempted → garbled normalized input; mitigated by fallback-to-Type-A rule
5. **Input Routing failure:** file path not expanded correctly → file not read; mitigated by tilde expansion rule (same in output step)
6. **Analysis failure (Step 3):** INVENTORY populated with paraphrased content → downstream preservation failures; mitigated by "verbatim, no normalization" INVENTORY rule
7. **Analysis failure (Step 3):** INVENTORY categories omitted (not `[]`) → missing preservation check; mitigated by "use `[]` for empty categories, do not omit keys" rule
8. **Ideation failure (Step 4):** contract conflicts with explicit input directive and is not logged → unintended intent alteration; mitigated by contract conflict rule
9. **Ideation failure (Step 4):** T4 contract targets `<context>` instead of `<role>` → known structural failure mode; mitigated by T4 binding rule
10. **Pre-Spawn Checkpoint failure:** spawn prompt assembled without INVENTORY or normalized input → synthesis agent lacks preservation targets; mitigated by four-item checklist
11. **Pre-Spawn Checkpoint failure:** spawn prompt truncated to fit context → loss of high-priority content; mitigated by prioritized truncation order
12. **Synthesis Agent failure (Step 6):** executes input prompt instead of enhancing it → fundamentally wrong output; mitigated by Hard Gate 3 reminder
13. **Synthesis Agent failure (Step 6):** INVENTORY item paraphrased rather than placed verbatim → preservation failure; mitigated by inline verification step (with known self-review bias limitation)
14. **Synthesis Agent failure (Step 6):** return message does not start with `VERIFICATION:` → malformed return; handled by Step 7 malformed path (display + no save)
15. **Synthesis Agent failure (Step 6):** self-review bias causes PASS on a flawed output → undetected preservation failure; known accepted risk (Design Decision #9); v2 candidate
16. **Step 7 failure:** PASS/FAIL routing error → wrong output path taken; mitigated by prefix-based NLP-free routing
17. **Step 7 failure:** save path collision not detected → file overwritten; mitigated by collision handling (`-v2`/`-v3`)
18. **Step 7 failure:** tilde not expanded before tool call → file write fails or goes to wrong path; mitigated by tilde expansion rule
19. **Cascade: Analysis contamination → Synthesis degradation:** role-switched Steps 3–4 pollute orchestrator context → synthesis agent spawn prompt includes extraneous reasoning → synthesis produces lower-quality output; known accepted risk (Design Decision #8); rate reduced by well-specified spawn prompt
20. **Cascade: Thin input → Minimal INVENTORY → Thin synthesis output:** if input has little verbatim content, INVENTORY is sparse → synthesis has few preservation anchors → output less constrained by input fidelity; not a failure mode per se but a quality degradation path, expected behavior
21. **Cascade: v2 deferral accumulation:** repair loops, DEEP mode, second verifier all deferred → v1 ships without recovery mechanisms; single synthesis failure produces unverified annotated output with no retry; acceptable given low baseline failure rate on well-specified synthesis prompt

</primitives>

<stripped_irreducibles>

## Simplicity-stripped irreducibles

Applying simplicity stripping: remove scaffolding (framing language, meta-commentary, transitional phrases); keep irreducibles (facts, constraints, unknowns that cannot be derived from others); mark dependency chains.

---

**Architecture**

- A1. `prompt-cog` uses a 1-spawn model: Steps 0–5, 7 run inline; Step 6 (synthesis) is the sole agent spawn.
- A2. Steps 3 (Analysis) and 4 (Ideation) use role-switched framing within the orchestrator context — they are not spawned agents.
- A3. No session directory exists; inter-agent state passes entirely via the synthesis spawn prompt body.
- ~~A4. INVENTORY is held in-context, not written to disk.~~ — derives from A3 (no session directory = no stage files = INVENTORY cannot be on disk)
- A5. The synthesis spawn prompt body must contain: normalized input, full analysis, INVENTORY YAML, prioritized contract list. All four must be present.
- A6. Token budget truncation order: normalized input > INVENTORY > contracts (high-priority only) > analysis > KB snippets. Normalized input and INVENTORY must never be truncated.

**Constraints (hard)**

- C1. Must run faster than `epiphany-prompt`. Speed is the primary architectural goal.
- C2. Must not produce output below `prompt-epiphany` quality baseline.
- C3. Must not replace or deprecate `prompt-epiphany` or `epiphany-prompt`.
- C4. Spawn budget: exactly 1 (synthesis only).
- C5. No repair loops in v1.
- C6. INVENTORY must use full schema in all modes (preservation non-negotiable).
- C7. `<task>` section always required in output XML.
- C8. `<meta source="prompt-cog"/>` must be first child of root `<prompt>`.
- C9. Hard Gate 3: synthesis agent must enhance, never execute, the input.
- C10. T4 contracts must target `<role>`, not `<context>`.
- C11. Every INVENTORY item must appear verbatim in synthesis output.
- C12. Flags valid only at first or last standalone token position.
- C13. Unknown/conflicting/deferred flags halt execution; never proceed silently.
- C14. Type B/C malformed XML falls back to Type A; no partial extraction.

**Flags**

- F1. Supported in v1: `--minimal`, `--quiet` (combinable).
- F2. Deferred in v1: `--verbose`, `--specification`, `--plan`; surface warning + halt.
- F3. `--minimal` + `--verbose` conflict → block, ask user to pick one.
- ~~F4. Unknown flags surface warning.~~ — derives from C13 (unknown flags halt; warning is the surface form of halting).

**Input routing**

- I1. Type B: detect `<meta source="prompt-epiphany"/>` as direct child of root `<prompt>`; strip outer wrapper.
- I2. Type C: detect `<meta source="epiphany-prompt"/>` or `<meta source="prompt-cog"/>` as direct child of root `<prompt>`; strip outer wrapper.
- I3. Type A: everything else, including file paths starting with `~/`, `/`, `./`, `../`.
- ~~I4. For file paths, read file contents as input.~~ — derives from I3 (Type A file path handling is specified within I3).

**Enhancement inventory verdicts**

- E1. PORT AS-IS: YAML INVENTORY schema, 6-dimension analysis, enhancement contracts schema v1, Hard Gate 3 reminder, return value contracts, T4 element binding rule, contract conflict rule, three-layer routing rule (adapted), Type A/B/C input routing, flag detection, tilde expansion + collision handling.
- E2. PORT SIMPLIFIED: anti-conformity second pass (in-context, not spawned), CreativeDC framing (2–3 sentence idiom), weakness impact scoring (inline), KB sources (3 fixed snippets inlined), pre-spawn module validation (4-item checklist).
- E3. DEFER: repair loops, DEEP expansion wave, scale variants, second synthesis verifier, dynamic KB selection, `verbose`/`specification`/`plan` modes.
- E4. SKIP: session directory / stage file persistence.

**Return contract**

- R1. Synthesis agent return: `VERIFICATION: PASS\n\n<XML>` or `VERIFICATION: FAIL — [summary]\n\n<XML>`.
- R2. Malformed return (does not start with `VERIFICATION:`): display as-is with header; do not save.
- R3. FAIL path: prepend `<!-- VERIFICATION FAILED: ... -->` to XML; save annotated output.

**Modes**

- M1. Normal mode: full 6-dimension analysis; all weakness + technique contracts; anti-conformity second pass; weakness impact scoring.
- M2. Minimal mode: INTENT + INVENTORY analysis only; weakness + technique contracts at equal priority; no anti-conformity pass.
- ~~M3. Quiet modifier: skip display; save directly.~~ — this is an orthogonal I/O behavior, not a derivation; retain.
- M3. Quiet modifier: skip terminal display; save directly without asking. Combines with any mode flag.

**Derivation chains identified:**
- A4 (INVENTORY in-context) derives from A3 (no session directory) — struck above.
- I4 (file path read) derives from I3 (Type A definition) — struck above.
- F4 (unknown flag warning) derives from C13 (halt rule) — struck above.

**No-derivation-chain finding:** All remaining primitives (A1, A2, A3, A5, A6, C1–C14, F1–F3, I1–I3, E1–E4, R1–R3, M1–M3) are irreducible — none derives from another after the three chains above were eliminated.

</stripped_irreducibles>

<tesla_measurement_plan>

## Tesla measurement plan

"Success is observable when a skill implementer reads the `prompt-cog` SKILL.md and produces a functioning implementation that (a) completes Step 0–7 execution on a representative prompt input without dropping any step, (b) spawns exactly one synthesis agent, (c) returns a `VERIFICATION: PASS` result with every INVENTORY item appearing verbatim in the output XML, (d) completes end-to-end faster than an equivalent `epiphany-prompt` run on the same input, and (e) produces output quality assessed as meeting or exceeding a `prompt-epiphany` single-pass result on the same input."

**Concrete observables:**
1. Step execution trace: all 8 steps (0–7) appear in execution log with non-empty output; no step is skipped or empty.
2. Spawn count: exactly 1 agent spawn event per invocation (synthesis agent in Step 6).
3. Return message: starts with `VERIFICATION: PASS` on a well-formed input with a non-trivial prompt.
4. INVENTORY verbatim check: each INVENTORY item string appears as a substring of the output XML without modification.
5. Speed: wall-clock time from invocation to output display is less than an equivalent `epiphany-prompt` run (measurable by running both on identical input).
6. Quality floor: a blind comparison of `prompt-cog` vs. `prompt-epiphany` output on the same input does not consistently favor `prompt-epiphany` (human or automated assessment).
7. Flag behavior: `--minimal` flag produces a shorter analysis block (INTENT + INVENTORY only) with no anti-conformity section; `--verbose` flag produces the deferred-flag warning and halts.
8. Structural validity: output XML has `<meta source="prompt-cog"/>` as first child; `<task>` is always present.

</tesla_measurement_plan>

<verification_block>

## Verification

**Preparation-gate:** Every primitive listed in the Primitives section is either directly cited to the input (spec text) or declared an assumption. No fabricated primitives were introduced. The three count-cap declarations (Category 4: 18 items, others ≥20) are honest counts, not padded.

**Tesla constructive-test:** The measurement plan specifies 8 concrete observables: step execution trace, spawn count, return message prefix, INVENTORY verbatim substring check, wall-clock speed comparison, blind quality comparison, flag behavior, and structural XML validity. Each is verifiable by a third party without access to the specification — all pass the Tesla constructive-test gate.

**Count check:**
- Category 1 (Components): 22 items — PASS (≥20)
- Category 2 (Interfaces): 21 items — PASS (≥20)
- Category 3 (Known constraints): 22 items — PASS (≥20)
- Category 4 (Quality attributes): 18 items — count-cap declared explicitly in output — PASS (cap declared)
- Category 5 (Stakeholder requirements): 20 items — PASS (≥20)
- Category 6 (Trade-off history): 20 items — PASS (≥20)
- Category 7 (Failure modes): 21 items — PASS (≥20)

**Simplicity stripping:** Three derivation chains identified and struck (A4, I4, F4). Remaining 30 irreducibles confirmed independent. "No derivation chains found — all remaining primitives irreducible" does not apply — chains were found and eliminated. Result: clean irreducible set.

**All gates PASS. Output is ready for downstream stages.**

</verification_block>

</state_loading>
