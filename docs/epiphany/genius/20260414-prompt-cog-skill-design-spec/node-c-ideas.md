# Node C — Idea Matrix
Generated from cross-analysis of Node A (prompt-cog spec) and Node B (epiphany genius report).
Each idea is an independent element keyed by ID. Matrix is AI-optimized for downstream filtering.

---

## Meta
- Source: Node A (`docs/superpowers/specs/2026-04-14-prompt-cog-design.md`) × Node B (`report.md`)
- Generation method: Exhaustive cross-analysis across all Node B sections (Headline, Collisions, Bridges, Hypotheses, Counter-examples, Probes, Scope Limits, Coherence Signals)
- Pass count: 2 (primary + contrarian re-read)

---

## Idea Matrix

### I01
**Source:** Node B Headline Insight — "substitution of protocol compliance for agent isolation is architecturally sound for mechanical quality checks... produces systematic quality degradation for judgment-dependent checks — a pattern the spec acknowledges individually in design decisions but does not synthesize into a design-level principle"
**Observation:** DD#14 in Node A does synthesize this pattern, but it is the 14th design decision — buried after 13 others. Its structural position underweights its importance as the primary architectural constraint.
**Idea:** Promote DD#14 (mechanical/judgment boundary) to DD#1 and renumber subsequent decisions. Make the mechanical/non-mechanical quality boundary the framing principle for all other decisions.
**Category:** Structural prominence / Architecture documentation
**Effort:** Low (reorganization only)

### I02
**Source:** Node B CS-4 — "spawn prompt construction is the highest-leverage quality determinant in the architecture" confirmed by 3 independent chains
**Observation:** Node A describes what must be IN the spawn prompt (components, token budget) but gives no guidance on HOW it is assembled or how contamination-encoded content from Steps 3-4 is extracted for inclusion.
**Idea:** Add a spawn prompt assembly protocol to Step 5 specifying HOW to extract role-switched content from orchestrator context — not just WHAT to include. The assembly step is currently a pre-spawn checklist, not a construction specification.
**Category:** Architecture gap
**Effort:** Medium

### I03
**Source:** Node B Bridge 3 — "Role-as-Channel-Code: role declarations written as structural output markers simultaneously function as behavioral activators AND channel codes. The synthesis spawn prompt is the demultiplexer."
**Observation:** Node A says Steps 3-4 use "role-switched framing" but does not specify output structure. The synthesis spawn assembler reads the orchestrator context but has no structural address markers to distinguish analyst output from ideation output from prose.
**Idea:** Implement structural output section headers in Steps 3 and 4 (e.g., `--- ANALYST OUTPUT ---` ... `--- END ANALYST ---`) so the spawn assembler can extract from named channels by structural address, not by parsing full prose context.
**Category:** Architecture improvement — context contamination reduction
**Effort:** Low-Medium (text specification + assembly logic)

### I04
**Source:** Node B H5 — "A verbatim code block placed in an inappropriate XML section is 'verbatim' but misplaced, and the inline check may not catch the misplacement."
**Observation:** Node A's synthesis verification step (Step 6, step 5) says "confirm every INVENTORY item appears verbatim in the draft" — substring presence only. Placement in a wrong XML section passes this check.
**Idea:** Strengthen synthesis verification to check semantic placement: each INVENTORY item must appear verbatim AND be placed in an XML section appropriate to its category (code_blocks in task/constraints, urls in most relevant section, key_constraints in constraints, tone_markers in role/context, etc.).
**Category:** Verification enhancement
**Effort:** Low (instruction text addition)

### I05
**Source:** Node B C3 — "anti-conformity second pass claims '+32.9% novelty gains' measured in epiphany-prompt's DEEP architecture with an independent agent context. In-context re-read in an orchestrator context already containing analysis, contracts, and role framings is not the same cognitive operation."
**Observation:** Node A Step 4 says anti-conformity contracts must pass 5 tests but gives no test to discriminate them from primary contracts. A contaminated context will produce anti-conformity contracts that feel novel to the orchestrator but are indistinct from primary contracts in character.
**Idea:** Add a 6th novelty gate to the anti-conformity test battery: "Would a primary-pass T1–T13 analyst have generated this contract? If yes or borderline, discard. Only append contracts you can justify with a specific primary-exclusion reason." Require each anti-conformity contract's rationale to include "Primary-pass exclusion reason: [...]".
**Category:** Anti-conformity quality gate
**Effort:** Low (instruction text addition)

### I06
**Source:** Node B Bridge 1 — "Role residue accumulates monotonically as steps progress inline; the synthesis agent inherits the full accumulation." + Illumination: "context-reset framing at the spawn boundary (not a new agent) as a v2 intervention"
**Observation:** The synthesis spawn prompt is a new agent context and does not inherit orchestrator state, so the "contamination" is what gets ENCODED in the spawn prompt body. A preamble on the spawn prompt does not fix encoded content — but it does orient the synthesis agent to treat its provided context as authoritative rather than supplementary to prior state it does not have.
**Idea:** Add to synthesis spawn prompt (Step 6) a brief context boundary statement: "[You are a fresh agent. Your complete context is below — treat it as your sole input. Disregard any behavioral assumptions from role labels; execute the protocol specified.]" This orients the synthesis agent correctly for the information it is given.
**Category:** Spawn orientation improvement
**Effort:** Minimal (one-line addition)

### I07
**Source:** Node B Counter-example 3 — "Agentic/executable workflow prompts → Hard Gate 3 reminder present but not mechanically enforced → synthesis agent may interpret 'enhance without executing' such that execution IS the enhancement for workflow prompts"
**Observation:** Node A Out of Scope section acknowledges this failure class but the spec gives no mechanism to warn users when their input falls into this category. A user running a SKILL.md description through prompt-cog will receive no signal that their output quality guarantee is degraded.
**Idea:** Add a Type D detection heuristic in Step 1 (Input Routing) for inputs that appear to be agentic/executable workflows: patterns include SKILL.md YAML frontmatter, 3+ consecutive shell command lines, `<step>` or `<skill:` XML tags, or sequences resembling agent orchestration instructions. Tag as Type D. In Step 2, surface a targeted advisory before proceeding.
**Category:** User-facing safety signal
**Effort:** Low-Medium (detection heuristic + advisory message)

### I08
**Source:** Node B Bridge 5 — "The contract schema is structurally isomorphic to a typed function signature... detecting when two contracts specify conflicting operations on the same target_section with the same technique."
**Observation:** Node A Step 4 has a contract conflict rule for input-directive conflicts but no check for internal contract conflicts (two contracts both targeting `<role>` with T4 specifying incompatible actions). Internal conflicts generate synthesis errors that the pre-spawn checklist cannot catch.
**Idea:** In Step 4, after generating all contracts, add a same-target scan: group by (technique, target_section). If two contracts in a group specify incompatible actions (one adds, one removes; or two specify different content for the same element), mark as an internal conflict, keep higher-priority contract, log.
**Category:** Contract quality
**Effort:** Low (instruction text addition to Step 4)

### I09
**Source:** Node B H2 — "The spec contains no complexity-detection trigger that would warn the user or escalate to DEEP mode... The spec does not identify input complexity as the threshold condition that determines when the architecture degrades below the quality floor."
**Observation:** Node A Complexity Scope section describes the threshold (~12 INVENTORY items, non-deeply-interdependent constraints) but this is documentation for implementers, not a runtime advisory for users. A user running a complex input gets no signal.
**Idea:** Add a lightweight complexity assessment to Step 2 (Announce): quick-scan the input for INVENTORY density indicators (number of apparent code blocks, URLs, named technical entities, constraint statements). If count suggests >12 INVENTORY items OR >5 high-impact constraints, append to the announce message: "Advisory: this input appears above the moderate-complexity threshold — results may fall outside the quality floor scope. Consider epiphany-prompt DEEP for this input."
**Category:** User-facing quality signal
**Effort:** Low (assessment + advisory text in Step 2)

### I10
**Source:** Node B Counter-example 1 — "Complex input with large INVENTORY (15+ items) hitting token budget → KB snippets dropped → synthesis agent operates without technique guidance → quality degrades below prompt-epiphany baseline"
**Observation:** Node A Step 6 token budget note specifies KB snippets are dropped last-but-one in truncation order. Users don't know when this happens or what to do if it does.
**Idea:** In Step 2, if normalized input is estimated to be >3000 words, surface a separate advisory: "Note: Large input detected. The synthesis spawn prompt may approach token budget limits, which could cause KB snippet truncation and reduced technique coverage. Enhancement quality may be reduced for technically dense inputs of this size."
**Category:** User-facing token budget warning
**Effort:** Minimal (advisory text in Step 2)

### I11
**Source:** Node B H3 — "Quality floor constraint is an expected-value claim, not a per-invocation guarantee... The constraint 'must not produce output below prompt-epiphany quality baseline' is stated without scope qualification."
**Observation:** Node A Design Constraints table says "DO NOT produce lower quality output than prompt-epiphany baseline for moderate-complexity inputs (see Complexity Scope below)." The "for moderate-complexity inputs" qualifier IS present. However, the phrasing "DO NOT" still reads as a prohibition (per-invocation absolute) rather than an expected-value design constraint. An implementer might interpret "DO NOT" as a guarantee they must enforce per invocation rather than as a statistical design target.
**Idea:** Change "DO NOT produce lower quality output than prompt-epiphany baseline for moderate-complexity inputs" to "Maintain expected output quality at or above prompt-epiphany baseline for moderate-complexity inputs (expected-value statistical claim; see Complexity Scope)." Making "expected-value" explicit in the constraint removes the false absolutism.
**Category:** Spec language precision
**Effort:** Minimal (one-line wording change)

### I12
**Source:** Node B FAIL path gap (implicit from Step 7 analysis)
**Observation:** Node A Step 7 FAIL path says display annotated XML + ask to save, but gives no guidance on what to do next. A user receiving VERIFICATION: FAIL has no path forward specified in the skill itself.
**Idea:** In Step 7 FAIL path, after displaying failure summary and annotated XML, add recovery guidance: "To reduce synthesis errors, try: (1) `--minimal` flag to lower context pressure, (2) run the best-effort output through prompt-cog again, or (3) use epiphany-prompt for this input if it is complex." This is informational; it does not change architecture.
**Category:** User experience / error recovery
**Effort:** Minimal

### I13
**Source:** Node B C2 — "INVENTORY verbatim mandate vs. synthesis enhancement quality... self-review bias makes the synthesis agent more likely to miss the verbatim failure when it produces a 'better-sounding' result."
**Observation:** Node A synthesis protocol says "find the appropriate XML section for it. An item is 'placed' when it appears verbatim in the draft." The synthesis agent's motivation to improve phrasing might inadvertently cause paraphrasing of INVENTORY items — and self-verification will not catch this if the paraphrase reads better.
**Idea:** Add an explicit anti-paraphrase instruction in the synthesis spawn prompt (Step 6): "Verbatim means character-for-character identical, including capitalization, punctuation, and spacing. Any improvement to surrounding prose must not alter INVENTORY item characters. When surrounding text conflicts with verbatim placement, the verbatim item wins — adjust the surrounding text, not the item."
**Category:** INVENTORY fidelity reinforcement
**Effort:** Minimal (one instruction addition)

### I14
**Source:** Node B Probe 4 — "implement role declarations as structurally-named output headers... verify whether the synthesis spawn prompt assembler can extract from named channels by structural address, reducing contamination noise"
**Observation:** This is the implementation detail of I03 (Role-as-Channel-Code). The probe asks to validate that structural extraction works in practice.
**Idea:** (Same as I03 — Role-as-Channel-Code headers.) The spec should define the exact structural header format so there is no ambiguity in extraction. Format proposal: `=== ANALYST OUTPUT BEGIN ===` / `=== ANALYST OUTPUT END ===` and `=== IDEATION OUTPUT BEGIN ===` / `=== IDEATION OUTPUT END ===`.
**Category:** Architecture implementation detail (pairs with I03)
**Effort:** Low

### I15
**Source:** Node B strongest objection — "A well-specified synthesis spawn prompt cannot compensate for the cognitive state it was built from — if analysis and ideation steps produced contaminated or incomplete outputs, the spawn prompt body encodes those errors."
**Observation:** The pre-spawn checklist (Step 5) validates four presence items. None of them check QUALITY of analysis/ideation outputs — only that they exist. A low-quality analysis that is present passes the checklist identically to a high-quality one.
**Idea:** Add a minimal quality gate to Step 5 checklist: after validating presence, require a brief self-assessment of analysis quality. "ANALYSIS QUALITY GATE: Do the WEAKNESSES identified have clear causal explanations (not just labels)? Is the TECHNIQUES gap analysis specific (technique + gap + impact, not just 'T5 could help')? If either answer is 'no,' flag and re-run analysis step before proceeding to spawn."
**Category:** Analysis quality gate
**Effort:** Medium (adds a re-run conditional)

### I16
**Source:** Contrarian re-read pass — questioning the architectural decision to use inline Steps 3-4
**Observation:** Node A accepts context contamination as a trade-off (DD#8) but the spec doesn't give the implementer any mechanism to detect when contamination has materially affected ideation output. If Step 3 analyst framing is still active during Step 4 ideation, the ideation specialist role may not fully suppress it.
**Idea:** At the start of Step 4, add an explicit deactivation line before the role switch: "End of analyst role. You are no longer in structured analysis mode." Then apply the ideation specialist role declaration. This creates a cognitive boundary within the inline context and may reduce role bleed.
**Category:** Role transition quality
**Effort:** Minimal (one instruction line)

### I17
**Source:** Contrarian re-read pass — questioning whether the contract format is complete
**Observation:** Node A contract format v1 is `technique | target_section | action | rationale | priority`. The `rationale` field is underspecified. Node B (Bridge 5) notes contracts are structurally isomorphic to typed function signatures. A typed function signature has a return type. Enhancement contracts lack a success criterion — how does the synthesis agent know when a contract has been successfully applied?
**Idea:** Add an optional `success_criterion` field to the contract format v1: `technique | target_section | action | rationale | priority | success_criterion`. The success criterion is a verifiable condition ("The `<role>` section now contains a specific domain + behavior declaration" rather than "role section improved"). This makes inline verification more precise.
**Category:** Contract schema enhancement
**Effort:** Low-Medium (schema addition + synthesis protocol update)

### I18
**Source:** Contrarian re-read pass — questioning whether Step 5 checklist item 5 is actually sufficient
**Observation:** Node A Step 5, item 5: "at least one contract maps to each high-impact weakness in the WEAKNESSES block." This IS present. But the check is binary — pass/fail — with no requirement that the mapped contract is actually capable of addressing the weakness (not just that some contract references it). A contract can be mapped to a weakness with a low-relevance technique.
**Idea:** Strengthen Step 5 item 5: "For each high-impact weakness, verify that at least one mapped contract uses a technique that plausibly addresses the specific gap identified. A presence-only mapping (contract exists that mentions the weakness) does not satisfy this check — the contract's technique and action must be coherent with the weakness's causal description."
**Category:** Interface 2 coherence quality
**Effort:** Low (instruction strengthening)

### I19
**Source:** Contrarian re-read pass — questioning the minimal mode design
**Observation:** Minimal mode skips STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES blocks — keeping only INTENT + INVENTORY. This means all contracts in minimal mode derive from INTENT only (no weakness scoring, no technique gap analysis). A user running a complex prompt in --minimal mode gets extremely thin contract generation. The quality degradation is not surfaced to the user.
**Idea:** In Step 2, when minimal mode is detected, add to the announce message: "Minimal mode: analysis limited to intent + inventory preservation. Contract generation will be narrower than normal mode — use normal mode for prompts requiring technique gap coverage."
**Category:** Mode transparency
**Effort:** Minimal

### I20
**Source:** Contrarian re-read pass — questioning the sufficiency check boundary
**Observation:** Node A Step 2 sufficiency check says block if input has "no discernible task, is fundamentally ambiguous, or has no identifiable intent." The examples given (single words, fragments, blank inputs) are the obvious cases. But there's a gray zone: inputs that ARE tasks but have no anchor points for enhancement (e.g., "write a poem about my dog"). These are valid prompts but have empty INVENTORY and no structural elements to preserve.
**Idea:** Add to Step 2 a sufficiency note: "An input with no INVENTORY (empty code blocks, URLs, constraints) is a valid input — enhancement will focus on structural and technique improvements. Proceed without blocking. Do not treat low-INVENTORY inputs as insufficient."
**Category:** Sufficiency check boundary clarity
**Effort:** Minimal

---

## Contrarian Second Pass — Additional Ideas

_Re-reading Node A from a contrary position: what does the primary analysis miss or get wrong?_

### I21
**Contrarian claim:** The 6-dimension analysis (INTENT, STRUCTURE, CONSTRAINTS, TECHNIQUES, WEAKNESSES, INVENTORY) is borrowed wholesale from epiphany-prompt's design. For prompt-cog's lighter architecture, 6 dimensions may generate analysis that is too granular for the synthesis agent to use, producing context bloat that degrades synthesis quality.
**Idea:** Consider whether STRUCTURE and CONSTRAINTS can be collapsed into WEAKNESSES (structural problems ARE weaknesses; constraint conflicts ARE weaknesses). Reducing from 6 to 4 dimensions (INTENT, TECHNIQUES, WEAKNESSES, INVENTORY) would produce a tighter analysis output with less context overhead.
**Category:** Analysis streamlining
**Effort:** Medium (architecture change)

### I22
**Contrarian claim:** The single synthesis agent must perform synthesis, verbatim placement, contract execution, AND inline verification. This is four distinct cognitive tasks in one response. Even in a fresh agent context, this is a high cognitive load per generation.
**Idea:** Split the synthesis spawn prompt into two sequential invocations: (1) Synthesis+Placement (produce draft XML with all INVENTORY items placed), (2) Verification+Finalization (check verbatim compliance, contract application). This costs one additional spawn but trades spawn latency for verification quality.
**Category:** Architecture change (adds spawn)
**Effort:** High — directly contradicts 1-spawn constraint

### I23
**Contrarian claim:** The FAIL path in Step 7 is architecturally passive — it just annotates and optionally saves. But a FAIL result is evidence that the synthesis prompt was flawed. Discarding this evidence wastes information.
**Idea:** On VERIFICATION: FAIL, before saving, capture the failure summary as a structured entry for future spawn prompt quality improvement. In v1, this could be as simple as logging `[timestamp, failure_summary, input_complexity_estimate]` to a local file. Even a passive log creates a feedback mechanism absent today.
**Category:** Quality feedback loop
**Effort:** Low (log append on fail)

### I24
**Contrarian claim:** The flag detection in Step 0 strips flags from "first or last standalone token." The spec says "A flag mid-sentence within the prompt body is treated as prompt content." But what about a prompt that STARTS with a flag-like string that is NOT a mode flag, e.g., a prompt beginning with "--analyze this algorithm"? Currently this is treated as an unknown flag and halts execution.
**Idea:** Add a disambiguation rule to Step 0: if the token starting with `--` does not match any known or deferred flag name exactly, before surfacing an unknown-flag error, check if it is followed by non-flag words forming a syntactic phrase (i.e., it reads as prose, not a flag). Known mode flags are an exact closed set: `--minimal`, `--quiet`, `--verbose`, `--specification`, `--plan`. Anything not in this list that starts with `--` but continues as natural prose should generate a softer advisory: "Token '[...]' looks like a flag but is not recognized. Treating as prompt content — if you intended a flag, check spelling."
**Category:** Flag detection edge case handling
**Effort:** Low

### I25
**Contrarian claim:** The spec defines output format as always XML with a specific canonical section order. But some users may want to enhance prompts that are deliberately non-XML (markdown, numbered list format). The spec's synthesis protocol writes XML regardless of input format.
**Idea:** Add to Step 6 synthesis protocol: "If the normalized input is a non-XML structured format (e.g., numbered instructions, markdown headers), maintain format type in the output. If the input is XML or unstructured text, produce XML. Do not impose XML wrapping on inputs that are semantically non-XML."
**Category:** Output format sensitivity
**Effort:** Low-Medium

---

## Iteration 2 — Derived Ideas (second generative pass over matrix)

### I26
**Derived from I03 + I15:** If Role-as-Channel-Code headers are implemented (I03), the quality gate in I15 becomes more tractable — the analyst output section can be mechanically evaluated for structural completeness (does it have INTENT block? INVENTORY? WEAKNESSES with causal descriptions?) before the ideation role begins. This integration is more powerful than either idea alone.
**Idea:** Pair I03 (structural headers) with a post-header completeness check at the end of each role section before the next role begins (not in a separate Step 5 checklist — inline, immediately after each role output). This catches thin analysis before ideation propagates the error.
**Category:** Inline quality checkpointing
**Effort:** Medium

### I27
**Derived from I05 + I08:** Anti-conformity novelty gate (I05) and same-target contract conflict (I08) are both contract quality mechanisms. They could share a single "contract finalization" sub-step in Step 4: (a) novelty gate for anti-conformity contracts, (b) same-target conflict scan for all contracts, (c) conflict log update. Rather than two separate instructions, structure them as a single finalization block.
**Idea:** Add a "Step 4 Contract Finalization" block at the end of Step 4 that runs three sequential checks: (1) anti-conformity novelty gate, (2) same-target conflict scan, (3) conflict log update. Single block, three checks, one locus in the spec.
**Category:** Step consolidation
**Effort:** Low (restructuring)

### I28
**Derived from I09 + I19:** Complexity detection advisory (I09) and minimal mode advisory (I19) both output advisory messages in Step 2. If minimal mode is active AND complexity is high, both advisories would fire. They should be merged into a single coordinated message rather than two separate advisory outputs that might seem contradictory (e.g., minimal mode announced, then immediately warned that minimal mode gives thin coverage on a complex input).
**Idea:** Coordinate Step 2 advisory messages: if minimal mode + high complexity both detected, produce a single combined advisory: "Minimal mode with high-complexity input: analysis will be limited to intent and inventory, and this input appears above the moderate-complexity threshold. Consider normal mode or epiphany-prompt DEEP for this input."
**Category:** Message coordination
**Effort:** Minimal

---

## Summary — Total Ideas Generated: 28 (I01–I28)
- Primary pass: I01–I20
- Contrarian pass: I21–I25
- Derived pass: I26–I28
