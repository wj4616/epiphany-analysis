# Epiphany Omnipotent Output — Design Spec

- **Date:** 2026-04-08
- **Status:** Validated design, ready for implementation
- **Target:** `~/.claude/skills/epiphany-omnipotent/SKILL.md` (output section replacement)
- **Related:** `epiphany-genius-design.md`, `epiphany-brainstorm-skill-design.md`

---

## Relationship to Existing SKILL.md

This spec **replaces** the `<omnipotent_output_v1>` section in the current `epiphany-omnipotent/SKILL.md`.

**What changes:**
- Output assembly logic (final stage only)
- Output XML structure (verbose → distilled)
- Intent detection (new capability)
- Deliverable routing (new capability)
- File save location (new default)

**What stays the same:**
- All pipeline stages (Context Gather → PG1 → FRAME → S1-S6 → Synthesis → PG2 → Decision → PG3)
- All injections (0-5)
- All gates (PG1, PG2, PG3)
- All verification checks (V1-V4)
- All methodology stages (SCAMPER, Lateral, Morphological, Six Hats, TRIZ, Reverse Brainstorming, Pugh Matrix)

**Relationship to PG3 Verification:**

The 4 validation checks in this spec (success_criterion, constraint_satisfaction, risk_coverage, alternative_comparison) are **OUTPUT VALIDATION** — they run AFTER the pipeline completes and validate the distilled output.

They are **SEPARATE FROM** and **ADDITIONAL TO** the PG3 verification checks (V1-V4) which validate the pipeline execution itself.

| Check Type | When | What |
|------------|------|------|
| PG3 V1-V4 | During pipeline | Content preservation, knowledge claims, logic, format |
| Output validation | After distillation | Success criterion, constraints, risks, alternatives |

Both run. PG3 produces `<verification_report>` in verbose XML. Output validation produces `<validation>` in distilled output.

---

## Design Decisions Log

| # | Decision | Rationale |
|---|---|---|
| D1 | Memory storage: Hybrid approach | Full XML in-memory during session; optional file save for audit trail |
| D2 | Intent detection: Combined heuristic | Flags override; keywords + context + confidence; low confidence → ambiguous (both outputs) |
| D3 | XML depth: Soft limit, target 3, allow up to 4 | Structured data (alternatives, risks, phases) needs depth; prose sections stay shallow |
| D4 | Validation checks: Mandatory with extended status | 4 required checks; status can be passed/partial/failed/not_applicable/insufficient_context |
| D5 | Deliverable content: Flexible with required elements | Nothing left out; required elements anchor each type; optional elements adapt to input |
| D6 | Output approach: Two-phase with conditional deliverables | Core always present; deliverables based on intent; clean separation |
| D7 | File save location: `~/prompts/epiphany-omnipotent/` | Matches prompt-epiphany pattern; user-visible location |
| D8 | Risk distinction: Core vs implementation | Core risks from analysis (S3/S4/S5); planning risks from implementation; source attribution |
| D9 | Zero confidence fallback: Clarification prompt | If confidence < 0.3 and no flag, ask user for intent |

---

## 1. Purpose

Design the final output stage of epiphany-omnipotent to produce an **optimized result** that:

1. **Stores complete analysis internally** — Full XML audit trail in memory during session
2. **Distills actionable insights** — Extract key findings from verbose XML
3. **Routes based on intent** — Brainstorming, planning, or both outputs
4. **Preserves all technical details** — Code/formulas byte-identical
5. **Validates the synthesis** — 4 required checks with human-readable evidence

---

## 2. Output Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                    EPIPHANY-OMNIPOTENT PIPELINE                  │
│  (Context Gather → PG1 → FRAME → S1-S6 → Synthesis → PG2 → S6)   │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      OUTPUT ASSEMBLY STAGE                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. Store verbose XML in memory (audit trail)                   │
│                                                                  │
│  2. Run intent detection algorithm                              │
│     - Check explicit flags (--brainstorm, --plan)               │
│     - Analyze keyword signals                                    │
│     - Analyze structural signals                                 │
│     - Compute confidence score                                   │
│                                                                  │
│  3. Distill: extract core elements from verbose XML              │
│     - framing_context → problem_statement                       │
│     - survivors_pool → primary + runners_up                     │
│     - S3/S4/S5 → risks_and_mitigations                           │
│     - verification → validation checks                           │
│     - input → technical_preservation                             │
│                                                                  │
│  4. Generate deliverable(s) based on intent                     │
│     - confidence >= 0.8 → one deliverable                        │
│     - confidence 0.5-0.79 → one deliverable, note confidence    │
│     - confidence < 0.5 → both deliverables                       │
│     - confidence < 0.3 AND no flag → ask clarification           │
│                                                                  │
│  5. Run validation checks (4 required)                          │
│                                                                  │
│  6. Emit <optimized_omnipotent_output>                          │
│                                                                  │
│  7. Ask: "Save full audit trail to file?"                       │
│                                                                  │
│  8. If yes: save to ~/prompts/epiphany-omnipotent/              │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 3. Intent Detection Algorithm

### Detection Priority (highest to lowest)

1. **Explicit flag override** — `--brainstorm` or `--plan` at first/last token
2. **Explicit text request** — "give me a plan" or "I want to brainstorm" in input
3. **Keyword signals** — presence of intent keywords
4. **Structural signals** — input structure characteristics
5. **Default to ambiguous** — if confidence < 0.5

### Keyword Signals

```xml
<keyword_signals>
  <brainstorming>
    <keyword>brainstorm</keyword>
    <keyword>ideate</keyword>
    <keyword>possibilities</keyword>
    <keyword>explore</keyword>
    <keyword>ideas</keyword>
    <keyword>alternatives</keyword>
    <keyword>options</keyword>
    <keyword>what if</keyword>
    <keyword>could we</keyword>
    <keyword>help me think</keyword>
  </brainstorming>
  <planning>
    <keyword>plan</keyword>
    <keyword>implement</keyword>
    <keyword>build</keyword>
    <keyword>create</keyword>
    <keyword>develop</keyword>
    <keyword>code</keyword>
    <keyword>step by step</keyword>
    <keyword>how do I</keyword>
    <keyword>what steps</keyword>
    <keyword>execute</keyword>
  </planning>
</keyword_signals>
```

### Structural Signals

```xml
<structural_signals>
  <brainstorming_indicators>
    <signal>Input contains question mark but no explicit deliverable keyword</signal>
    <signal>Input contains "or" separating alternatives (choice exploration)</signal>
    <signal>Input contains "what if" or "could we" (hypothetical exploration)</signal>
    <signal>No code blocks AND no file paths AND no implementation context</signal>
    <signal>Multiple question marks (open-ended exploration)</signal>
  </brainstorming_indicators>
  <planning_indicators>
    <signal>Input contains code blocks or file paths</signal>
    <signal>Input contains specific deliverable noun (plan, implementation, code)</signal>
    <signal>Input contains success criteria or acceptance criteria</signal>
    <signal>Input contains constraints (time, budget, technical)</signal>
    <signal>Input contains "need to" or "have to" (action requirement)</signal>
  </planning_indicators>
</structural_signals>
```

### Confidence Scoring

| Signal Type | Weight |
|-------------|--------|
| Explicit flag | 1.0 (override) |
| Explicit text request | 0.9 |
| Keyword match (3+) | 0.7 |
| Keyword match (1-2) | 0.5 |
| Structural signal (2+) | +0.2 to matching intent |
| Contradicting signals | -0.2 from winning intent |

### Intent Detection Algorithm (Pseudocode)

```
function detect_intent(input):
  # Step 1: Check explicit flag override
  if input contains "--brainstorm" at first/last token:
    return {intent: "brainstorming", confidence: 1.0, source: "flag"}
  if input contains "--plan" at first/last token:
    return {intent: "planning", confidence: 1.0, source: "flag"}

  # Step 2: Check explicit text request
  if input contains "give me a plan" or "I want a plan" or "create a plan":
    return {intent: "planning", confidence: 0.9, source: "text_request"}
  if input contains "brainstorm" or "I want to explore" or "help me think":
    return {intent: "brainstorming", confidence: 0.9, source: "text_request"}

  # Step 3: Count keyword matches
  brainstorm_keywords = count_matches(input, BRAINSTORMING_KEYWORDS)
  planning_keywords = count_matches(input, PLANNING_KEYWORDS)

  # Step 4: Count structural signals
  brainstorm_signals = count_matches(input, BRAINSTORMING_STRUCTURAL_SIGNALS)
  planning_signals = count_matches(input, PLANNING_STRUCTURAL_SIGNALS)

  # Step 5: Calculate base score
  if brainstorm_keywords >= 3:
    brainstorm_score = 0.7
    brainstorm_intent = True
  elif brainstorm_keywords >= 1:
    brainstorm_score = 0.5
    brainstorm_intent = True
  else:
    brainstorm_score = 0.0
    brainstorm_intent = False

  if planning_keywords >= 3:
    planning_score = 0.7
    planning_intent = True
  elif planning_keywords >= 1:
    planning_score = 0.5
    planning_intent = True
  else:
    planning_score = 0.0
    planning_intent = False

  # Step 6: Add structural signal bonus
  if brainstorm_signals >= 2:
    brainstorm_score += 0.2
  if planning_signals >= 2:
    planning_score += 0.2

  # Step 7: Apply contradiction penalty
  if brainstorm_intent and planning_intent:
    # Both have signals - this is contradictory
    if brainstorm_score > planning_score:
      brainstorm_score -= 0.2  # Contradiction penalty to winner
    elif planning_score > brainstorm_score:
      planning_score -= 0.2  # Contradiction penalty to winner
    # If equal, no penalty (already low confidence)

  # Step 8: Determine winner
  if brainstorm_score > planning_score:
    return {intent: "brainstorming", confidence: brainstorm_score}
  elif planning_score > brainstorm_score:
    return {intent: "planning", confidence: planning_score}
  else:
    # Tied or both zero
    return {intent: "ambiguous", confidence: 0.4}
```

**Thresholds (evaluated in order):**
1. If `confidence < 0.3` AND no explicit flag AND no explicit text request → emit ONLY `<clarification_needed>` (no core, no deliverables), then STOP
2. Else if `confidence < 0.5` → emit both deliverables (ambiguous)
3. Else if `confidence < 0.8` → emit one deliverable, include `<confidence_note>` in process_notes
4. Else `confidence >= 0.8` → emit one deliverable

---

## 4. Core Output Structure

**Always emitted regardless of intent.**

```xml
<optimized_omnipotent_output version="2.0">
  <intent type="brainstorming|planning|ambiguous">
    <detection>
      <explicit_flag><!-- if present --></explicit_flag>
      <keyword_signals><!-- matches found --></keyword_signals>
      <structural_signals><!-- matches found --></structural_signals>
      <confidence>0.0 to 1.0</confidence>
      <rationale>One-sentence explanation</rationale>
    </detection>
  </intent>
  
  <core>
    <problem_statement>
      <!-- Deep-structure principle + success criterion from framing_context -->
    </problem_statement>
    <primary_answer>
      <!-- The synthesized solution/recommendation from synthesis -->
    </primary_answer>
    <key_insights>
      <!-- 3-5 most important non-obvious insights -->
      <insight>...</insight>
    </key_insights>
  </core>
  
  <alternatives>
    <primary id="A_X">
      <statement>What it proposes</statement>
      <rationale>Why selected</rationale>
      <evidence_score>
        <value>0.6</value>
        <signals s1="1" s2="1" s3_yellow="1" s3_black="0" s4_triz="1"/>
      </evidence_score>
      <surviving_objection>Strongest objection addressed</surviving_objection>
    </primary>
    <runners_up>
      <alternative id="A_Y" score="...">
        <statement>What it proposes</statement>
        <why_not_selected>Why not chosen</why_not_selected>
      </alternative>
    </runners_up>
  </alternatives>
  
  <risks_and_mitigations>
    <risk id="R1" severity="high|medium|low" source="s3_black|s4_contradiction|s5_reverse">
      <description>What could go wrong</description>
      <mitigation>How to address it</mitigation>
    </risk>
    <!-- Additional risks with sequential IDs: R2, R3, etc. -->
    <!-- Up to 4 levels deep if needed for nested risk details -->
  </risks_and_mitigations>
  
  <validation>
    <check name="success_criterion" status="passed|partial|failed|not_applicable|insufficient_context">
      <evidence>How the criterion is addressed</evidence>
      <reason><!-- if not_applicable or insufficient_context --></reason>
    </check>
    <check name="constraint_satisfaction" status="passed|partial|failed|not_applicable|insufficient_context">
      <evidence>Which constraints are honored</evidence>
      <reason><!-- if no constraints in input --></reason>
    </check>
    <check name="risk_coverage" status="passed|partial|failed|not_applicable|insufficient_context">
      <evidence>Which risks are mitigated</evidence>
      <reason><!-- if no risks identified --></reason>
    </check>
    <check name="alternative_comparison" status="passed|partial|failed|not_applicable|insufficient_context">
      <evidence>Why primary beats alternatives</evidence>
      <reason><!-- if only one alternative --></reason>
    </check>
  </validation>
  
  <technical_preservation>
    <code_blocks>
      <!-- Each code block from input, byte-identical -->
      <code language="detected|unknown"><![CDATA[original code here]]></code>
    </code_blocks>
    <formulas>
      <!-- Mathematical expressions, byte-identical -->
      <formula>original formula here</formula>
    </formulas>
    <technical_specs>
      <!-- Named requirements, constraints, specifications -->
      <spec type="requirement|constraint|specification">original text here</spec>
    </technical_specs>
  </technical_preservation>
  
  <gaps_and_escapes>
    <unresolved>
      <!-- Gaps not addressed from synthesis -->
      <gap>...</gap>
    </unresolved>
    <escape_hatch>
      <!-- Conditions requiring user decision -->
      <condition>...</condition>
    </escape_hatch>
  </gaps_and_escapes>
  
  <process_notes>
    <intent_contradiction><!-- If flag contradicts input signals --></intent_contradiction>
    <assumptions_made><!-- Assumptions during analysis --></assumptions_made>
    <skipped_stages><!-- Any stages skipped due to scale/depth --></skipped_stages>
    <confidence_note><!-- If confidence 0.5-0.79, note the confidence level --></confidence_note>
  </process_notes>
  
  <!-- CONDITIONAL DELIVERABLES (based on intent) -->
  <!-- See Section 5 and Section 6 -->
  
</optimized_omnipotent_output>
```

---

## 5. Brainstorming Deliverable

**Emitted when:** `intent = brainstorming` OR `intent = ambiguous`

```xml
<brainstorming_deliverable>
  <themes>
    <theme name="...">
      <ideas>
        <idea source="scamper|lateral|diverge_associative|diverge_combinatorial|diverge_analogical|morphological|synthesis">
          <statement>What this idea proposes</statement>
          <potential>high|medium|low</potential>
          <considerations>Trade-offs or dependencies to consider</considerations>
        </idea>
        <!-- Additional ideas -->
      </ideas>
    </theme>
    <!-- Additional themes -->
  </themes>
  
  <directions>
    <direction name="...">
      <description>What this direction entails</description>
      <tradeoffs>
        <pro>Benefit of this direction</pro>
        <con>Cost or risk of this direction</con>
      </tradeoffs>
      <when_to_choose>Criteria for selecting this direction</when_to_choose>
    </direction>
    <!-- 2-4 key directions -->
  </directions>
  
  <decision_criteria>
    <criterion>What to consider when choosing between directions</criterion>
    <!-- 3-5 criteria -->
  </decision_criteria>
  
  <open_questions>
    <question>What remains undecided or needs exploration</question>
    <!-- Optional: questions that emerged but weren't answered -->
  </open_questions>
</brainstorming_deliverable>
```

**Required elements:** `<themes>`, `<directions>`, `<decision_criteria>`
**Optional elements:** `<open_questions>`

---

## 5.5. Clarification Prompt (Separate Emission)

**Emitted when:** `confidence < 0.3` AND no explicit flag AND no explicit text request

**This is NOT part of `<optimized_omnipotent_output>`.** It replaces the entire output.

```xml
<epiphany_clarification version="1.0">
  <message>I couldn't determine your intent from the input.</message>
  <question>Are you looking for brainstorming (exploring possibilities) or planning (implementation steps)?</question>
  <options>
    <option value="brainstorming">Explore ideas and possibilities</option>
    <option value="planning">Create implementation steps</option>
    <option value="both">Both brainstorming and planning</option>
  </options>
  <detected_signals>
    <keyword_signals><!-- what was detected --></keyword_signals>
    <structural_signals><!-- what was detected --></structural_signals>
  </detected_signals>
  <fallback_behavior>If user says "just pick one" or doesn't respond, default to ambiguous (emit both deliverables).</fallback_behavior>
</epiphany_clarification>
```

**User response handling:**
- If user picks option → re-run with that intent (confidence = 1.0)
- If user says "just pick one" or "doesn't matter" → emit both deliverables (ambiguous)
- If user provides more context → re-run intent detection with new input
- If user gives partial response ("probably planning", "more like brainstorming") → use that intent with confidence = 0.8
- If user gives unclear response → default to ambiguous (emit both deliverables)

---

## 6. Planning Deliverable

**Emitted when:** `intent = planning` OR `intent = ambiguous`

```xml
<planning_deliverable>
  <implementation_overview>
    <objective>What the implementation accomplishes</objective>
    <scope>
      <in_scope>What is included</in_scope>
      <out_of_scope>What is explicitly not included</out_of_scope>
    </scope>
    <success_definition>How to know when implementation is complete</success_definition>
  </implementation_overview>
  
  <prerequisites>
    <prerequisite>
      <requirement>What must exist or be ready</requirement>
      <verification>How to check if satisfied</verification>
      <if_missing>Action to take if not satisfied</if_missing>
    </prerequisite>
    <!-- All prerequisites before starting -->
  </prerequisites>
  
  <phases>
    <phase name="..." order="1">
      <goal>What this phase accomplishes</goal>
      
      <context_gather>
        <files_to_read>
          <file>Files to examine before starting this phase</file>
        </files_to_read>
        <questions_to_answer>
          <question>What must be understood before proceeding</question>
        </questions_to_answer>
        <assumptions_to_verify>
          <assumption>Assumptions that must be confirmed</assumption>
        </assumptions_to_verify>
      </context_gather>
      
      <steps>
        <step order="1">
          <action>Specific action to take</action>
          <implementation_details>
            <!-- Detailed instructions for the AI agent -->
            <!-- Files to modify, functions to create, logic to implement, patterns to follow -->
          </implementation_details>
          <expected_output>What successful completion looks like</expected_output>
          <common_pitfalls>What to avoid</common_pitfalls>
        </step>
        <!-- Additional steps -->
      </steps>
      
      <review_checkpoint>
        <verify>
          <check>What to verify after this phase</check>
        </verify>
        <questions_to_ask>
          <question>Question to answer before proceeding</question>
        </questions_to_ask>
        <rollback>If review fails, how to recover</rollback>
      </review_checkpoint>
      
      <dependencies>
        <dependency>What must complete before this phase</dependency>
      </dependencies>
      
      <estimated_complexity>low|medium|high</estimated_complexity>
    </phase>
    <!-- 2-8 phases typical -->
  </phases>
  
  <inter_phase_analysis>
    <description>Analysis to perform between phases</description>
    <regather_context>
      <files_to_reexamine>Files that may need re-reading after changes</files_to_reexamine>
      <patterns_to_check>New patterns that may have emerged</patterns_to_check>
      <dependencies_impacted>What else may be affected by changes</dependencies_impacted>
    </regather_context>
    <reflection_questions>
      <question>Question to ask before moving to next phase</question>
    </reflection_questions>
  </inter_phase_analysis>
  
  <final_verification>
    <end_to_end_test>
      <scenario>How to verify the complete implementation works</scenario>
      <expected_behavior>What correct behavior looks like</expected_behavior>
    </end_to_end_test>
    <integration_checks>
      <check>How to verify this works with existing systems</check>
    </integration_checks>
    <rollback_plan>
      <trigger>What triggers rollback</trigger>
      <steps>Steps to undo changes</steps>
    </rollback_plan>
  </final_verification>
  
  <risk_mitigations>
    <mitigation risk_ref="R1">
      <action>Specific action to reduce risk</action>
      <trigger>When to apply this mitigation</trigger>
      <verification>How to know if mitigation worked</verification>
    </mitigation>
    <!-- References core risks by id: R1, R2, etc. -->
  </risk_mitigations>
  
  <validation_gates>
    <gate after_phase="...">
      <check>What to verify before proceeding</check>
      <criteria>How to know if ready for next phase</criteria>
      <blocker>What blocks proceeding</blocker>
    </gate>
  </validation_gates>
  
  <context_requirements>
    <initial_context>Context needed before starting</initial_context>
    <context_to_preserve>Context that must not be lost during implementation</context_to_preserve>
    <context_to_regather>Points where fresh context gathering is needed</context_to_regather>
  </context_requirements>
</planning_deliverable>
```

**Required elements:** `<implementation_overview>`, `<prerequisites>`, `<phases>`, `<risk_mitigations>`, `<final_verification>`
**Optional elements:** `<inter_phase_analysis>`, `<validation_gates>`, `<context_requirements>`

### Phase Count Guidance

The number of phases should reflect input complexity:

| Input Characteristics | Phase Count |
|----------------------|-------------|
| Single component, clear scope | 2-3 phases |
| Multiple components with dependencies | 4-6 phases |
| Complex system with integration points | 6-8 phases |
| Scale-based: MINIMAL input → 2-3 phases; STANDARD → 3-5 phases; DEEP → 4-8 phases |

Phase granularity rule: Each phase should be completable in one focused session (30-90 minutes for AI agent work).

### UX Considerations

**Clarification override flag:**
Users who want to skip clarification prompts can use `--skip-clarification` or explicitly specify intent with `--brainstorm` / `--plan`.

**File save preference:**
Users who want to skip the save prompt can use `--auto-save` (always save) or `--no-save` (never ask). Default is to ask.

**Planning deliverable verbosity:**
For simple inputs, the planning deliverable should be more concise:
- MINIMAL scale: Fewer phases, simpler steps, omit optional elements
- STANDARD scale: Full structure
- DEEP scale: Comprehensive structure with all optional elements

---

## 7. Validation Checks Detail

### Check 1: Success Criterion Coverage

**Status values:**
- `passed` — Success criterion from framing_context is addressed in primary_answer
- `partial` — Addressed but with gaps or caveats
- `failed` — Not addressed
- `not_applicable` — No success criterion was defined in framing_context
- `insufficient_context` — Cannot determine from available information

**Evidence format:**
```
The primary_answer addresses [criterion] by [mechanism]. 
Remaining gap: [any gap].
```

### Check 2: Constraint Satisfaction

**Status values:**
- `passed` — All constraints from input are honored
- `partial` — Some constraints honored, others acknowledged but not addressed
- `failed` — Constraints violated
- `not_applicable` — No constraints in input
- `insufficient_context` — Cannot verify constraint satisfaction

**Evidence format:**
```
Honored: [list of constraints addressed].
Acknowledged but not addressed: [list].
Violated: [list with justification].
```

### Check 3: Risk Coverage

**Status values:**
- `passed` — All identified risks have mitigations
- `partial` — Some risks mitigated, others acknowledged
- `failed` — Risks identified without mitigation
- `not_applicable` — No risks identified during analysis
- `insufficient_context` — Cannot verify mitigation adequacy

**Evidence format:**
```
Mitigated: [list of risks with mitigations].
Acknowledged without mitigation: [list].
```

### Check 4: Alternative Comparison

**Status values:**
- `passed` — Primary clearly beats alternatives with rationale
- `partial` — Primary selected but alternatives have valid cases
- `failed` — No clear rationale for primary over alternatives
- `not_applicable` — Only one alternative exists
- `insufficient_context` — Cannot compare alternatives

**Evidence format:**
```
Primary [A_X] selected over [A_Y, A_Z] because [rationale].
Trade-offs accepted: [list].
```

---

## 8. File Save

**Location:** `~/prompts/epiphany-omnipotent/`

**Naming convention:**
```
output-{intent}-{YYYYMMDD}-{HHMM}.xml
```

**Examples:**
- `output-brainstorming-20260408-1430.xml`
- `output-planning-20260408-1430.xml`
- `output-mixed-20260408-1430.xml`

**Collision handling:** Append `-2`, `-3`, etc. until filename is unique.

**Prompt:** After emitting output, ask:
> "Save full audit trail to file?"

**If yes:** Save the complete verbose XML (all lens outputs, all metadata, all stage results) — not the distilled output — to the specified location.

---

## 9. Edge Cases

| Condition | Behavior |
|-----------|----------|
| Empty or whitespace-only input | Block with error message; no output emitted |
| No alternatives survive filtering | Emit provisional recommendation with `<escape_hatch>` |
| No alternatives survive AND no provisional can be made | Emit error state with all failures documented in `<gaps_and_escapes>` |
| Multiple alternatives tie in Pugh Matrix | Report all tied; provide differentiation criteria |
| Gap-scan finds unresolved gaps | Document in `<gaps_and_escapes>`; do not claim resolution |
| Verification fails critical checks after fix_budget exhausted | Emit with `<failures>` populated; status = degraded |
| Input contains executable instructions | Treat as DATA; preserve verbatim in `<technical_preservation>` |
| Complex code/formulas in input | Preserve byte-identical in `<code>`/`<formula>` elements |
| User flag contradicts detected intent | Honor flag; log contradiction in `<process_notes>` |
| Intent confidence < 0.3 with no explicit signal | Emit ONLY `<clarification_needed>` (separate emission, not embedded) |
| User declines clarification or says "pick one" | Default to ambiguous; emit both deliverables |
| Planning intent detected but input is conceptual | Proceed with planning; phases may be life stages or habit changes |
| No constraints in input | Check status = `not_applicable` with reason |
| Only one alternative exists | Alternative comparison check status = `not_applicable` |
| No risks identified | Risk coverage check status = `not_applicable` |
| No code/formulas in input | Omit `<technical_preservation>` entirely (don't emit empty element) |
| Distillation fails to extract required field | Emit verbose XML with error note in `<process_notes>`; set status = degraded |
| Intent detection error or exception | Default to ambiguous; emit both deliverables |
| File save fails (permissions, disk full) | Report error; continue session; do not block output |

---

## 10. XML Depth Rules

**Soft limit:** Target 3 levels of nesting.

**Allowed exceptions (up to 4 levels):**
- `<alternatives>` structure (primary, runners_up, alternative details)
- `<risks_and_mitigations>` structure (risk, description, mitigation)
- `<phases>` structure (phase, steps, step details)
- `<validation>` structure (check, evidence)

**Rationale:** Structured data needs depth to organize. Prose sections (`<core>`, `<problem_statement>`, `<primary_answer>`) stay at 2 levels.

---

## 11. Memory Storage (Hybrid)

**During session:** Complete verbose XML lives in conversation context (LLM memory).

**On file save offer:** If user accepts, write to `~/prompts/epiphany-omnipotent/` with timestamp.

**Audit trail contents (verbose XML, not emitted output):**
- All `<lens_outputs>` with full methodology-specific structures
- Complete `<framing_context>`
- Full `<synthesis>` with agreement/disagreement analysis
- Complete `<decision>` with Pugh Matrix scoring
- Full `<inventory>` with input_inventory, methodology_inventory
- Complete `<verification_report>` with V1-V4 details
- All `<process_notes>` and pipeline metadata

**Distilled output (what gets emitted):**
- `<optimized_omnipotent_output>` structure defined in this spec
- Does NOT include verbose lens outputs
- DOES include core, alternatives, risks, validation, technical preservation, deliverables

---

## 12. Backward Compatibility

### Output Version Migration

| From | To | Migration |
|------|------|----------|
| `<omnipotent_output_v1>` | `<optimized_omnipotent_output version="2.0">` | New structure; v1 consumers need to parse v2 |

**Key structural changes:**
- Root element renamed and versioned
- Lens outputs removed from emitted output (available in verbose XML on request)
- New `<intent>` element at start
- New `<clarification_needed>` emission path (separate from main output)
- New deliverable sections (`<brainstorming_deliverable>`, `<planning_deliverable>`)

**Migration path for downstream consumers:**
1. Check root element: if `<omnipotent_output_v1>`, use v1 parser
2. If `<optimized_omnipotent_output>`, use v2 parser
3. v2 parser should check `<intent>` first to determine deliverable structure

### Trigger Conditions Update

Add to SKILL.md trigger conditions:

| Trigger | Behavior |
|---------|----------|
| `/epiphany-omnipotent --brainstorm` | Force intent = brainstorming (confidence = 1.0) |
| `/epiphany-omnipotent --plan` | Force intent = planning (confidence = 1.0) |
| `/epiphany-omnipotent --skip-clarification` | If confidence < 0.3, default to ambiguous (both deliverables) instead of asking |
| `/epiphany-omnipotent --auto-save` | Skip save prompt; always save verbose XML |
| `/epiphany-omnipotent --no-save` | Skip save prompt; never save verbose XML |

---

## 13. Distillation Rules

Map verbose XML fields to output fields:

| Verbose XML Source | Output Field | Transformation Rule |
|---|---|---|
| `framing_context.deep_structure_principle` | `core.problem_statement` (partial) | Extract principle, format as "The core problem is: [principle]" |
| `framing_context.success_criterion` | `core.problem_statement` (partial) | Append "Success means: [criterion]" |
| `synthesis.primary_answer` or `decision.recommendation.primary` | `core.primary_answer` | Copy directly; if multiple alternatives tied, synthesize into combined answer |
| All lens insights | `core.key_insights` | Select 3-5 non-obvious insights that emerged from cross-stage synthesis |
| `survivors_pool[0]` | `alternatives.primary` | Copy id, statement; derive rationale from Pugh Matrix scores |
| `survivors_pool[1..n]` | `alternatives.runners_up` | Copy id, statement; derive why_not_selected from comparison |
| `survivors_pool[0].evidence_share` | `alternatives.primary.evidence_score.value` | Copy decimal value |
| `survivors_pool[0].signals` | `alternatives.primary.evidence_score.signals` | Copy signal attributes |
| `survivors_pool[0].surviving_objection` | `alternatives.primary.surviving_objection` | Copy from Evidence-Share filter output |
| S3 Black risks + S5 failure_modes | `risks_and_mitigations` | Merge; assign sequential IDs (R1, R2...); add source attribute |
| S4 contradictions (if unresolved) | `risks_and_mitigations` | Convert to risk with source="s4_contradiction" |
| PG3 verification failures | `validation.check` | Map to 4 validation checks; set status appropriately |
| `input_inventory.code_blocks` | `technical_preservation.code_blocks` | Copy byte-identical with detected language |
| `input_inventory.formulas` | `technical_preservation.formulas` | Copy byte-identical |
| `gaps_from_frame` | `gaps_and_escapes.unresolved` | Copy gaps with status="unresolved" |
| `escape_hatch` from any stage | `gaps_and_escapes.escape_hatch` | Copy condition |

### Key Insights Selection Rules

An insight qualifies as "non-obvious" if it:
1. Emerged from synthesis of multiple stages (not from a single lens)
2. Challenges or refines the initial framing
3. Would not be immediately apparent from a surface reading of the input
4. Connects ideas from different methodologies (e.g., SCAMPER + TRIZ resolution)
5. Reveals a hidden assumption or dependency

Select 3-5 insights. If fewer qualify, include the most significant single-lens insights.

### Risk Merging Rules

Risks from multiple sources may overlap. Merge when:
- S3 Black Hat and S5 Reverse Brainstorming identify the same risk
- S4 contradiction becomes a risk when unresolved

When merging:
- Use the most specific description
- Combine mitigations (all mitigations are relevant)
- Set source to the earlier stage that identified it

---

## 14. Deliverable Generation Rules

### Brainstorming Deliverable Generation

**Theme Formation:**
1. Group S1 ideas (SCAMPER, Lateral, Diverge) by common thread
2. Name each theme after the underlying pattern (not methodology name)
3. Each idea gets `source` attribute indicating origin

**Direction Formation:**
1. Extract 2-4 coherent paths from `synthesis.agreement` points
2. Each direction = a potential path forward
3. Include tradeoffs from `synthesis.disagreement` value conflicts
4. `when_to_choose` = criteria from Pugh Matrix or synthesis

**Decision Criteria Formation:**
1. Extract from `decision.criteria` in Pugh Matrix
2. Add criteria from `synthesis.uncovered` (unaddressed topics)
3. 3-5 criteria total

**Open Questions Formation:**
1. Extract from `gaps_from_frame` with status="unresolved"
2. Add from `synthesis.uncovered` topics that weren't addressed
3. Optional — omit if empty

### Planning Deliverable Generation

**Phase Formation:**
1. Analyze `primary_answer` for natural sequencing
2. Create 2-8 phases based on:
   - Dependencies (what must happen before what)
   - Logical grouping (related work together)
   - Risk mitigation ordering (address risks early)
3. Each phase = one `<phase>` element

**Step Formation:**
1. Decompose each phase into concrete actions
2. Each action = one `<step>`
3. `implementation_details` = specific instructions for AI agent
4. `expected_output` = how to verify step completion

**Context Gather Per Phase:**
1. Identify files/knowledge needed for that phase
2. `questions_to_answer` = unknowns that must be resolved
3. `assumptions_to_verify` = assumptions that need confirmation

**Review Checkpoint Formation:**
1. Create checkpoint after each phase
2. `verify` = what to check before proceeding
3. `questions_to_ask` = reflection prompts
4. `rollback` = how to undo if checkpoint fails

**Risk Mitigation Mapping:**
1. Each `<risk>` in core gets `<mitigation>` in planning
2. `risk_ref` = ID reference (R1, R2, etc.)
3. `action` = specific implementation step
4. `trigger` = when to apply (which phase)
5. `verification` = how to confirm mitigation worked

---

## 15. Definitions

### Non-Obvious Insights

An insight qualifies as "non-obvious" if it meets **at least 2** of the following criteria:

1. **Cross-stage emergence:** Originated from synthesis of multiple stages (not from a single lens)
2. **Framing refinement:** Challenges or refines the initial framing in a meaningful way
3. **Surface invisibility:** Would not be immediately apparent from a surface reading of the input
4. **Methodology connection:** Connects ideas from different methodologies (e.g., SCAMPER + TRIZ resolution)
5. **Hidden dependency:** Reveals an assumption, dependency, or constraint not explicit in the input

**Example:**
- Obvious: "The solution should handle errors gracefully" — explicit in most inputs
- Non-obvious: "The SCAMPER 'eliminate' option surfaced a dependency we assumed away in the morphological analysis" — cross-stage, reveals hidden assumption

### Implementation Details Structure

`<implementation_details>` in planning deliverable should contain:

```xml
<implementation_details>
  <files_to_modify>
    <file>path/to/file.ext</file>
  </files_to_modify>
  <functions_to_create>
    <function name="functionName" params="param1, param2">Brief description</function>
  </functions_to_create>
  <logic>Step-by-step logic in plain language</logic>
  <patterns>Existing patterns in codebase to follow (if known)</patterns>
  <edge_cases>Edge cases to handle</edge_cases>
</implementation_details>
```

### Context Regathering Triggers

`<context_to_regather>` should specify when fresh context is needed:

- A phase modifies shared state that other code depends on
- A phase introduces new dependencies not in prerequisites
- A phase changes the architecture in ways that affect later phases
- A phase creates new files that other phases will read
- A phase removes files that later phases expected to exist