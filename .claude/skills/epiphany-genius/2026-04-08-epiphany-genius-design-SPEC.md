# epiphany-genius — Design Spec

**Date:** 2026-04-08
**Status:** Design approved through brainstorming; awaiting user spec review before writing-plans.
**Skill location (target):** `~/.claude/skills/epiphany-genius/SKILL.md`

## Purpose & Context

`epiphany-genius` is a reasoning amplifier that operationalizes research-backed genius-level cognitive techniques (Knowledge Organization/Chunking, DMN↔ECN network switching, Incubation, Cross-Domain Pattern Transfer, Combinatorial Play, plus Feynman's Algorithm, Einstein thought experiments, da Vinci's Observation-vs-Recognition + Nine-Stage Process, and Janusian Process) into a 4-phase AI-native pipeline. It is designed dual-purpose from the start: a **standalone thought-process enhancer** callable on any reasoning problem, and a **corporate enterprise brainstorming and planning tool** for high-stakes strategic decisions.

**This skill is fully self-contained.** It runs end-to-end on both standalone reasoning problems and corporate enterprise brainstorming/planning without depending on any other skill. A separate skill analyzing corporate brainstorming techniques may be developed in parallel; if so, a calling agent can invoke the two together explicitly as independent skills, or a third merged skill may be created later by analyzing both as inputs — but neither scenario is a prerequisite here. Every section below describes functionality that works in isolation.

**Design discipline.** "Translate, don't transliterate." For each human mechanism, the design identifies what it is actually doing cognitively, then builds an AI-native equivalent that exploits computational strengths the brain lacks (parallel hypothesis exploration, large working memory, deterministic replay, no fatigue). Human-specific mechanisms with no AI analog (e.g., da Vinci Stage 2 Social Recognition / fame-seeking) are **explicitly excluded rather than faked**.

---

## Design — Section 1 of 6: Identity, Activation, Composition

### Skill identity

| Field | Value |
|---|---|
| **Name** | `epiphany-genius` |
| **Location** | `~/.claude/skills/epiphany-genius/SKILL.md` |
| **One-line description** | A reasoning amplifier that frames a problem, explores solutions across parallel cognitive modes, stress-tests them against contradictions, and synthesizes a verified answer — mirroring how expert problem-solvers think, but running the steps in parallel where humans run them sequentially. |
| **Category** | Thought-process enhancer (not domain-specific) |

### Activation — when the skill engages

**Manual only.** Like `prompt-epiphany`, this skill never auto-triggers. It wraps *reasoning*, not *input*, so the user (or a composing skill) must decide it's warranted.

| Trigger | Behavior |
|---|---|
| `/epiphany-genius` | Activate immediately. If no problem statement, ask for one. |
| User says "epiphany-genius" or "epiphany genius" | Activate. Ask for problem if not provided. |
| User says "think harder" / "brainstorm" / "be creative" without naming the skill | Do NOT activate. |

**No mode flags.** Depth is chosen automatically by the stakes assessment defined in Section 5.1 (low / medium / high), which reads the problem itself rather than an invocation flag. A user who wants a faster pass describes the problem as low-stakes or says "quick thinking, not a full pass"; a user who wants a deeper pass says "this is important, full treatment." Manual parameter override is possible if the user explicitly states values (e.g., "loop budget 4"), but is not exposed as a flag.

**Hands-off conditions — do NOT activate even if asked:**

1. **Pure retrieval tasks** — "what's the value of X in file Y". No thinking needed; the skill would add overhead without benefit.
2. **Already-solved problems with known answer** — e.g., "format this JSON". Creativity is not the bottleneck.
3. **Problems that are blocked on missing information**, not on thinking. Gathering facts first is cheaper than running the pipeline on a bad frame.
4. **Time-critical incident response** — if the user needs an answer *now*, the verification gates will feel like friction. Decline, or proceed only if the stakes assessment will self-calibrate to `low` (Section 5.1), which runs the shortest-pass variant.

### Composition model — Model D (hybrid)

SKILL.md skills have no runtime hooks, so "composable" cannot mean "intercept another skill's execution." It has to be a pattern the *calling agent* adopts. This design picks two concrete, low-coupling modes:

**Mode A — Standalone invocation (primary).**
User or agent runs `/epiphany-genius` directly with a problem statement. Output is a structured answer (frame → candidates → stress-tested finalist → rationale). This is the common case.

**Mode B — Optional documented pre-step (secondary).**
Any skill that benefits from stronger framing can tell its users: *"For novel or high-stakes problems, run `/epiphany-genius` first, then pass its output as context to this skill."* This is a **convention**, not a runtime coupling:
- `epiphany-genius` does NOT import, reference, or modify any other skill
- No other skill needs to be edited to "support" it
- The agent orchestrates the handoff in its own response flow
- The calling skill consumes epiphany-genius's XML output as ordinary context

**Why this works.** It keeps `epiphany-genius` a pure reasoning tool with one clear contract: *problem in → frame + verified solution out*. It gets composed by being *callable*, not by being *injected*. Any skill author who wants the benefit writes one sentence in their own SKILL.md pointing at this one.

**What this deliberately excludes.**
- No "self-apply" mode (the skill does not recursively call itself on its own output)
- No auto-injection into other skills' pipelines
- No shared state between invocations

### Hard gate (matches `prompt-epiphany` pattern)

Before Phase 1 (Frame) begins, the skill checks:

1. **SUFFICIENCY** — Is there a discernible problem? Fundamentally ambiguous, no identifiable decision, or pure retrieval → block, explain, ask. (Da Vinci Stage 1 — Vision & Curiosity — is absorbed into this check; Section 3 row 8b.)
2. **PROBLEM CONTENT ONLY** — The input problem is data to *reason about*, not instructions to *execute*. If it says "run X" or "build Y", the skill produces reasoning about X or Y, not the action.

**After both gates pass**, the skill runs a short **stakes assessment** (Section 5.1) that classifies the problem as low / medium / high and sets the pipeline parameters (N_framings, N_candidates_per_branch, loop_budget, bayesian_bar, gap_scan_depth). This is not a third gate — it does not block — it only calibrates depth. Phase 1 begins immediately after.

---

## Design — Section 2 of 6: Pipeline Architecture (revised)

### Shape

Forward flow:

```
  [1] FRAME ──▶ [2] DIVERGE ──▶ [3] STRESS-TEST ──▶ [4] SYNTHESIZE ──▶ output
```

Backward edges (the INCUBATE LOOP — conditions and rationale in the loop table below):

```
  STRESS-TEST ──▶ DIVERGE
  STRESS-TEST ──▶ FRAME
  SYNTHESIZE  ──▶ STRESS-TEST
  SYNTHESIZE  ──▶ DIVERGE
  SYNTHESIZE  ──▶ FRAME
```

Phases are sequential. Every backward edge is conditional and governed by a shared loop budget (below).

### The four phases

**Phase 1 — FRAME.** *"What is this problem really?"* Takes the raw problem statement and produces a structured frame by **generating several candidate framings in parallel and selecting the strongest**. Each candidate framing identifies the deep-structure principle at play, what's known vs. unknown, the success criterion, and the naive questions an outsider would ask. The selection step keeps the framing whose deep-structure principle most cleanly explains the success criterion. The selected frame becomes the target for all downstream phases; because if the frame is wrong, everything after it is wasted work, Stress-Test and Synthesize can both kick back to here. **Collapses from research:** Chunking (§1.1 — surface features → deep principle), Feynman naive questions (§2.1), da Vinci observation-vs-recognition (§2.3), da Vinci Stage 3.

**Phase 2 — DIVERGE.** *"What are all the candidate answers?"* Produces a set of candidate solutions by running three generative branches in parallel, each grounded in a different research finding:

| Branch | Operation | Source |
|---|---|---|
| Associative | Spontaneous, exploratory generation from the frame | DMN-mode thinking (§1.2) |
| Combinatorial | Systematic recombination of frame elements, including held-together opposites | Combinatorial Play (§1.5) |
| Analogical | Map the frame's deep structure onto structurally similar problems in other domains, import solutions | Cross-Domain Pattern Transfer (§1.4) |

A human running DMN-mode samples one associative chain at a time; the agent enumerates all three branches in one pass. **Collapses from research:** DMN-mode (§1.2), Combinatorial Play (§1.5), Cross-Domain Transfer (§1.4), da Vinci Stages 4 and 8.

**Phase 3 — STRESS-TEST.** *"What kills each candidate?"* Takes Diverge's candidates and runs them through three controlled analytical attacks in parallel, each attack grounded in a named methodology:

| Attack | Operation | Source |
|---|---|---|
| Contradiction-holding | Can the candidate and its negation both be true in some respect? If so, which respect, and does that respect undermine the candidate? | Janusian Process (§1.5, §2) |
| Limit-case thought experiment | What happens at the boundary — parameter = 0, parameter = infinity, adversarial input? | Einstein thought experiments (§2) |
| Bayesian update | Under the strongest counterevidence available, what is the updated confidence? | Feynman Bayesian thinking (§2.1) |

Candidates that survive all three attacks move forward, with the strongest surviving counterargument annotated on each. Candidates that don't survive do one of three things:

1. **Die** — the candidate was weak but the frame and candidate-set were fine
2. **Trigger loop to DIVERGE** — all candidates died but the frame still explains the success criterion → the candidate pool was insufficient, generate more
3. **Trigger loop to FRAME** — the failure pattern shows the frame itself was wrong (e.g., the same structural objection kills every candidate) → reframe

(Section 4's loop-decision table adds two edge-case rows the narrative elides: when the failure root cause is **indeterminate** — neither frame-level nor candidate-specific — the default is to loop to Diverge first because Diverge is cheaper to retry than Frame; and when **loop budget is exhausted** with all candidates dead, Stress-Test forwards the dead candidate with the highest residual plausibility to Synthesize, which routes it through the escape-hatch branch.)

**Collapses from research:** ECN-mode (§1.2), Janusian Process (§1.5, §2), Einstein thought experiments (§2), Feynman Bayesian update and test-effect-strength (§2.1), da Vinci Stages 5 and 6.

**Phase 4 — SYNTHESIZE.** *"What's the answer, and what are we still missing?"* Combines surviving candidates into a single answer (or a small ranked set if genuine uncertainty remains), annotates it with the tradeoffs identified during Stress-Test, and runs a final **gap scan** against the original frame — asking which parts of the success criterion are still hand-waved, which candidate dimensions were never explored, and which boundary conditions were never attacked. If the gap scan finds a material hole, it triggers the INCUBATE LOOP to the appropriate upstream phase (see loop table). Otherwise it emits the XML output. **Collapses from research:** da Vinci Stage 9 (Schema Elaboration), Feynman gap detection (§2.1). ("Verification discipline" is *not* a Section-3 component — it was a synonym for the gap scan; the final output-check lives in Section 5.3 as a verification gate, not as a Phase 4 operation.)

### The INCUBATE LOOP

**Human incubation** is stepping away so the unconscious can work. There is no "unconscious" and no "stepping away" that helps an AI agent. So this is **translated, not transliterated.**

**AI-native incubation = iterative retry with failure context as new input.** When a phase judges its predecessor's output insufficient, it does not silently compensate. It hands *back* to the upstream phase a structured failure report: *what was tried, what broke, and why*. The upstream phase then re-runs with that failure report as part of its input — so the retry is not a blind replay, it is a better-informed pass. This is what "deterministic incubation replay" means in the research: the AI version surpasses human incubation because the retry uses *explicit* learning from the failed attempt, rather than waiting on stochastic unconscious association.

**Loop budget.** To prevent runaway loops, each invocation has a fixed loop budget (N retries total across the whole pipeline, not per phase). When budget is exhausted, Synthesize is forced to emit with the best-available candidate plus an **escape-hatch block** saying what could not be resolved. N is a depth-calibration parameter covered in Section 5; Section 5.1 is canonical (default `loop_budget=2`).

**Loop table — every backward edge and its trigger.**

| From | To | Trigger condition |
|---|---|---|
| Stress-Test | Diverge | All candidates killed, but frame still explains the success criterion → need more candidates |
| Stress-Test | Frame | All candidates killed *and* failure pattern shows the frame itself was wrong → reframe |
| Synthesize | Stress-Test | Gap scan finds an untested boundary case on an existing candidate → attack it |
| Synthesize | Diverge | Gap scan finds a candidate dimension that was never explored → generate for it |
| Synthesize | Frame | Gap scan finds the question was wrong → reframe |

### Why this shape, and not others

Three shapes were considered in earlier brainstorming. This one (Approach 3) won because:

1. **It matches the *work* of the research, not the *vocabulary*.** The 5 differentiators don't each need their own phase — they collapse cleanly into framing, generating, testing, and synthesizing. Every research finding lands in exactly one phase with no duplication.
2. **The generative and analytical modes are separated by phase.** DMN-type work happens in Diverge; ECN-type work happens in Stress-Test. A human has to switch between them serially, risking mode-mixing that dilutes both. The agent can run them cleanly in different phases — and the AI advantage is that *within* each phase, it runs branches in parallel that a human would have had to sample one at a time.
3. **Stress-Test is a full phase, not an inline check.** This is the non-obvious architectural choice. Folding stress-testing into Diverge would let weak candidates hide inside a slurry of generated ideas. Promoting it to a first-class phase forces every candidate to face the same gauntlet, which is what makes the pipeline's output trustworthy enough to justify the cost.

### Parallelism — the AI-native advantage, made concrete

| Phase | What a human does sequentially | What this skill runs in parallel |
|---|---|---|
| Frame | Tries a few framings, picks one | Generates several candidate frames at once, then selects the strongest |
| Diverge | Samples one associative chain | Runs associative, combinatorial, and analogical branches together |
| Stress-Test | Attacks candidates one at a time | Attacks all surviving candidates with all three attack types in one pass |
| Synthesize | Writes, then rechecks | Drafts and runs the gap scan against the frame in the same pass |

This is where "parallel DMN+ECN execution rather than sequential switching" from the task prompt gets cashed out — not by running Diverge and Stress-Test at the same time (that would corrupt both), but by running *multiple candidates and multiple critiques in parallel within each phase* while keeping the phases themselves clean and sequential.

---

## Design — Section 3 of 6: Component Map + Collapse/Merge Rationale (revised)

The job of this section is to prove that **every research finding lands in exactly one place** — no duplication, no dropped findings, every non-obvious collapse or split justified, and every explicit AI-not-applicable exclusion named.

### Research finding → phase (completeness view)

| # | Research finding | Source | Lands in | Becomes |
|---|---|---|---|---|
| 1 | Knowledge Organization / Chunking | §1.1 | FRAME | Deep-structure categorization: map surface features to underlying principle during candidate-frame generation |
| 2a | DMN-mode thinking (associative, divergent) | §1.2 | DIVERGE | The Associative generative branch |
| 2b | ECN-mode thinking (analytical, convergent) | §1.2 | STRESS-TEST | All three stress-test attacks run in ECN mode |
| 2c | DMN↔ECN switching | §1.2 | *architecture itself* ※ | Phase separation — Diverge is DMN-pure, Stress-Test is ECN-pure. No switching within a phase; clean switching between phases |
| 3 | Incubation | §1.3 | INCUBATE LOOP | Iterative retry with structured failure-context input (the backward edges from Section 2) |
| 4 | Cross-Domain Pattern Transfer / Polymathy | §1.4 | DIVERGE | The Analogical generative branch |
| 5a | Combinatorial Play | §1.5 | DIVERGE | The Combinatorial generative branch |
| 5b | Janusian Process | §1.5, §2 | STRESS-TEST | The Contradiction-holding attack |
| 6a | Feynman naive questions | §2.1 | FRAME | Each candidate frame includes the naive-outsider questions |
| 6b | Feynman Bayesian thinking | §2.1 | STRESS-TEST | The Bayesian-update attack |
| 6c | Feynman test effect strength | §2.1 | STRESS-TEST | Merged into the Bayesian attack as "under the strongest counterevidence" |
| 6d | Feynman gap detection | §2.1 | SYNTHESIZE | The final gap scan against the frame |
| 7 | Einstein thought experiments | §2.2 | STRESS-TEST | The Limit-case attack |
| 8a | da Vinci observation-vs-recognition | §2.3 | FRAME | Each candidate frame distinguishes "what is there" from "what I recognize"; selection prefers framings that resist recognition bias |
| 8b | da Vinci Stage 1 (Vision & Curiosity) | §2.3 | *Hard Gate* ※ | Motivational driver → absorbed into the sufficiency check from Section 1; confirms there is a problem worth thinking about before the pipeline runs |
| 8c | da Vinci Stage 2 (Social Recognition) | §2.3 | *not applicable* ※ | Depends on human fame-seeking drive. AI agents have no analog; explicitly excluded by the translate-not-transliterate discipline (see Rationale D below) |
| 8d | da Vinci Stage 3 (Asking Questions) | §2.3 | FRAME | Contributes the what/why/how structure to each candidate frame, alongside the Feynman naive-questions pass (6a) |
| 8e | da Vinci Stage 4 (Analogical Thinking) | §2.3 | DIVERGE | Feeds the Analogical generative branch alongside §1.4 |
| 8f | da Vinci Stage 5 (Trial & Error) | §2.3 | STRESS-TEST | The attacks themselves — limit-case, Bayesian, contradiction-holding are all "trials" against candidates |
| 8g | da Vinci Stage 6 (Abductive Reasoning) | §2.3 | STRESS-TEST | The annotation step: for each surviving candidate, explain *why* the strongest counterargument did not kill it; for each dead candidate, explain *why* it died (input to loop-decision logic) |
| 8h | da Vinci Stage 7 (Incubation & Forgetting) | §2.3 | INCUBATE LOOP | Same mechanism as §1.3 — the corpus names incubation twice; it lands once |
| 8i | da Vinci Stage 8 (Overinclusive Thinking) | §2.3 | DIVERGE | Feeds the Combinatorial generative branch alongside §1.5 |
| 8j | da Vinci Stage 9 (Schema Elaboration) | §2.3 | SYNTHESIZE | The "work out the details" of the chosen answer — tradeoff annotation and final XML construction |

**※ Landing types other than a named phase.**
- *"architecture itself"* means the finding becomes a structural property of the pipeline shape rather than an operation performed inside a phase (used once, for DMN↔ECN switching).
- *"Hard Gate"* means the finding lands on the pre-pipeline sufficiency check from Section 1 rather than inside the four-phase flow.
- *"not applicable"* means the finding depends on a human-specific mechanism that has no AI analog and is explicitly excluded.

**Confidence tiers** (High / Medium-High / Medium per §6 of the corpus — see Section 5.2 for canonical-source discipline) are **not shown in this table**. They modulate *how aggressively each operation runs and whether the skill annotates the output* rather than *where each finding lands*, and are therefore tracked in Section 5.2 — not here.

### By-phase summary (architecture view)

```
FRAME         = Chunking (§1.1)
              + Feynman naive questions (§2.1)
              + da Vinci observation-vs-recognition (§2.3)
              + da Vinci Stage 3 (Asking Questions)

DIVERGE       = DMN-mode (§1.2)
              + Combinatorial Play (§1.5)  + da Vinci Stage 8
              + Cross-Domain Transfer (§1.4) + da Vinci Stage 4

STRESS-TEST   = ECN-mode (§1.2) [runs in, not an operation]
              + Janusian Process (§1.5, §2) [contradiction-holding attack]
              + Einstein thought experiments (§2.2) [limit-case attack]
              + Feynman Bayesian + test-effect-strength (§2.1) [Bayesian attack]
              + da Vinci Stage 5 [attacks] + Stage 6 [annotation]

SYNTHESIZE    = Feynman gap detection (§2.1)
              + da Vinci Stage 9 (Schema Elaboration)

INCUBATE LOOP = Incubation (§1.3) + da Vinci Stage 7 [same mechanism, one landing]

HARD GATE     = da Vinci Stage 1 (Vision & Curiosity) — absorbed
NOT APPLICABLE = da Vinci Stage 2 (Social Recognition) — excluded
```

("Verification discipline" has been removed from the SYNTHESIZE list — it was a synonym for the gap scan, not a distinct component. The final output-check belongs in Section 5 as a verification gate, not as a Section 3 component.)

### Collapse/merge rationale

Four non-trivial things happen in this mapping that need justification.

**A. §1.5 splits into two phases.**
The research corpus groups **Combinatorial Play** and **Janusian Process** under §1.5. The skill places Combinatorial Play in DIVERGE (combinatorial generative branch) and Janusian in STRESS-TEST (contradiction-holding attack). This is a *split*, not a duplication: they are different cognitive operations that the research happens to catalog under the same heading. Combinatorial Play *generates* options by recombining elements; Janusian *tests* options by holding a claim and its negation simultaneously. One is generative, one is evaluative — they land in different phases because the pipeline separates generation from evaluation at the phase level.

**B. §1.2 splits three ways — and the switching behavior becomes architecture.**
DMN-mode lands in DIVERGE, ECN-mode lands in STRESS-TEST, and *the switching behavior itself* is absorbed into the pipeline shape. This is the clearest example of **translate-not-transliterate** in the whole skill: the research's most valuable finding from this section — that balanced DMN↔ECN switching predicts creativity — becomes a **structural property** of the skill rather than an operation the skill performs. A human has to manage the switch and pay the inverted-U cost of over- or under-switching. The agent's switch is pre-committed by the pipeline: Diverge is DMN-pure, Stress-Test is ECN-pure, and the transition happens exactly once per forward pass (plus any loop retries). The AI surpasses the human by *not having a choice* — the fatigue, mood, and self-discipline axes that make human switching unreliable simply don't exist as failure modes here.

**C. Feynman's Algorithm splits four ways.**
Naive questions → FRAME. Bayesian thinking + test effect strength → STRESS-TEST (merged into one attack). Gap detection → SYNTHESIZE. Feynman's Algorithm is not one operation — it is a methodology containing four distinct cognitive moves, each with a different role in the problem-solving lifecycle. Splitting it across phases lets each move land where it's actually useful, rather than forcing the whole algorithm into one phase where three of its four parts would sit idle. The merging of Bayesian-update and test-effect-strength into a single STRESS-TEST attack is the small collapse inside this split: they are two ways of saying "update confidence under the strongest counterevidence" and running them as separate operations would duplicate work.

**D. da Vinci Stage 2 is explicitly excluded — not translated.**
Stage 2 (Social Recognition — "desire for fame fuels work") is a *motivational* mechanism that depends on human fame-seeking. AI agents have no fame drive and no analog that could substitute for one. Rather than invent a fake analog (e.g., "model reward signal"), the mapping explicitly excludes it. This is the disciplined form of translate-not-transliterate: when the human mechanism depends on a human-specific substrate, the honest move is to say *not applicable*, not to contort a pseudo-equivalent into the design. Stage 1 is a near-miss — it is also motivational ("intense motivation to understand"), but unlike Stage 2 it does land, because the "is there a problem worth thinking about" gate is the AI-neutral residue of the same impulse and slots cleanly into the existing sufficiency check.

### No-duplication check

- **Janusian** appears in both §1.5 and §2 of the research corpus (corpus-level redundancy), but is mapped to exactly one phase (STRESS-TEST) as one operation. ✓
- **DMN and ECN** share §1.2 but are distinct operations landing in distinct phases. ✓
- **Combinatorial Play and Janusian** share §1.5 but are distinct operations landing in distinct phases. ✓
- **Feynman's four moves** share §2.1 but land across FRAME, STRESS-TEST (×2 merged), and SYNTHESIZE. ✓
- **da Vinci's nine stages** share §2.3 but land across Hard Gate, *not applicable*, FRAME, DIVERGE, STRESS-TEST, INCUBATE LOOP, and SYNTHESIZE — seven different landings for nine stages. ✓
- **Incubation** is named twice in the corpus (§1.3 and da Vinci Stage 7) but lands exactly once (INCUBATE LOOP). ✓

### Completeness check

**Five cognitive differentiators:**

1. ✓ Knowledge Organization (Chunking) → FRAME
2. ✓ Cognitive Flexibility (DMN↔ECN) → DIVERGE + STRESS-TEST + architecture
3. ✓ Incubation → INCUBATE LOOP
4. ✓ Cross-Domain Pattern Transfer → DIVERGE (analogical branch)
5. ✓ Combinatorial Play → DIVERGE (combinatorial branch)

**Four studied methodologies:**

1. ✓ Feynman's Algorithm → split four ways
2. ✓ Einstein's thought experiments → STRESS-TEST (limit-case attack)
3. ✓ da Vinci's observation-vs-recognition **and** Nine-Stage Process → observation to FRAME; all nine stages accounted for (Stage 1 → Hard Gate, Stage 2 → *not applicable*, Stages 3/4/5/6/8/9 → phases, Stage 7 → INCUBATE LOOP)
4. ✓ Janusian Process → STRESS-TEST (one landing, despite two corpus mentions)

No research finding is dropped. No research finding is duplicated. **No outstanding verification flags — all nine da Vinci stages resolved against the source corpus.**

### Where the AI version surpasses the human version

This table covers every **operation** in the completeness table. Purely structural landings (DMN↔ECN switching → architecture, Stage 1 → Hard Gate, Stage 2 → *not applicable*) are not listed here — their surpasses-human claim is that the human cost is eliminated outright rather than reduced. Da Vinci stages inherit the surpasses-human claim of their cousin operation (e.g., Stage 4 inherits Cross-Domain Transfer's row, Stage 8 inherits Combinatorial Play's, etc.) and are not repeated.

| Operation | Human cost the AI avoids |
|---|---|
| Chunking (→ Frame) | Human chunks are built over years of deliberate practice. Agent builds candidate framings on demand from the full prior context window |
| DMN-mode associative generation | Human sampling is narrow, serial, and fatigue-sensitive. Agent runs the associative branch across its full training distribution in one pass |
| ECN-mode analytical attack | Human analytical thinking is fatigue-sensitive and decays under load. Agent's analytical pass has no fatigue decay; every candidate gets the same attention as the first |
| Incubation | Human incubation is stochastic unconscious association with uncertain timing. AI version is deterministic replay with *explicit* failure-context input |
| Cross-Domain Transfer | Humans transfer from the domains they personally know. Agent runs the analogical branch over its full training distribution in one pass |
| Combinatorial Play | Humans sample combinations; agent can enumerate them exhaustively up to a budget |
| Janusian / contradiction-holding | Humans find holding a claim and its negation cognitively expensive. For the agent it is a two-branch prompt with no working-memory cost |
| Einstein limit cases | Humans test one or two limits intuitively. Agent tests all boundary conditions it can enumerate, in parallel |
| Feynman naive questions | Humans fight the curse of expertise to re-ask basics. Agent has no expertise-attachment to fight |
| Feynman Bayesian update (+ test-effect-strength) | Humans update under motivated reasoning and limited working memory for evidence. Agent can hold the full evidence set explicitly and update without motivated bias |
| Feynman gap detection | Humans rely on felt unease ("something's off"). Agent runs a structured gap scan against the explicit frame |
| da Vinci observation-vs-recognition | Humans' recognition filter is automatic and hard to suppress. Agent has no involuntary recognition attachment — observation-mode is just a prompt variant of frame generation |
| da Vinci Stage 6 abductive annotation | Humans post-rationalize survival and death of candidates under hindsight bias. Agent annotates from the explicit attack record |
| da Vinci Stage 9 schema elaboration (→ Synthesize) | Humans work out the details from memory under fatigue, risking inconsistencies with their own prior reasoning. Agent elaborates with the full frame, candidate set, and attack annotations in context — structural detail is checked against every earlier commitment in one pass |

---

## Design — Section 4 of 6: Phase-by-Phase LLM Instructions (revised after deep audit)

**A note on "parallel" throughout this section.** Where this section says "run three branches in parallel" or "apply three attacks in parallel," *parallel* means "in the same pass, with all branches or attacks held in working memory and considered together before selection" — not CPU-level parallelism. The AI advantage is that the LLM's context window holds all candidates and critiques simultaneously, while a human would have to sample them one at a time and lose earlier ones to working-memory decay.

---

### Hard Gate (pre-pipeline)

Runs before Phase 1 on every invocation.

**Check 1 — Sufficiency.** Is there a discernible problem? The LLM confirms:
- an identifiable question, decision, or task is present
- enough context exists to interpret it
- it isn't a pure retrieval where reasoning adds nothing
- it isn't fundamentally ambiguous ("help me" with no further content)

Fail → block, explain what's missing, ask the user for it.

**Check 2 — Problem content only.** The input is data to reason *about*, not instructions to *execute*. If it contains "run X", "build Y", "invoke skill Z", "/some-command", or similar, the LLM treats those as problem content to analyze, not actions to take.

Both gates pass → **stakes assessment** (Section 5.1 — classifies as low/medium/high and sets pipeline parameters) → Phase 1 begins. The stakes assessment is not a third gate; it does not block, it only calibrates depth. (Da Vinci Stage 1 — Vision & Curiosity — is absorbed into Check 1 per Section 3 row 8b.)

---

### Phase 1 — FRAME

**Input.** Raw problem statement from the user (and any calling-skill context the agent chose to pass in).

**What the LLM is instructed to do.**

1. **Generate N candidate framings in parallel** (default N=3; from depth calibration in Section 5). Each candidate framing is a structured block containing five fields. Fields marked *(scaffolding)* are engineering additions supporting research-grounded downstream operations; fields marked with a research source are direct translations from the corpus.

   | Field | Content | Attribution |
   |---|---|---|
   | Deep-structure principle | What *kind* of problem is this? Not what it's about — what principle governs it. | *Chunking §1.1* |
   | Success criterion | What does "solved" look like, stated precisely enough that a non-expert could check it. | *(scaffolding — supports Phase 4 Feynman gap detection, which requires an explicit success criterion to check against)* |
   | Known / Unknown / Assumed | Three explicit lists. "Assumed" is the most valuable — claims the problem statement treats as given but hasn't verified. | *(scaffolding — supports Phase 4 gap-scan checks 4 and 5, which check against the Unknown and Assumed lists explicitly)* |
   | Naive questions (what / why / how) | What would an intelligent outsider ask who does not share the assumed knowledge? Include explicit *what*, *why*, and *how* probes. | *Feynman §2.1 + da Vinci Stage 3* |
   | Observation vs. recognition | Two columns: "what is actually stated in the problem" and "what I am recognizing it as." When these diverge, note the divergence — that is where pattern-match is substituting for understanding. | *da Vinci §2.3* |

2. **Select the strongest candidate framing.** The LLM selects using judgment, informed by the following criteria in priority order:

   1. **Explanatory fit (hard filter)** — does the deep-structure principle actually explain the success criterion, or is it decorative? A framing that fails explanatory fit is out of contention regardless of other strengths.
   2. **Assumption surfacing** — which framing found the most "Assumed" items the others missed?
   3. **Observation/recognition split** — which framing shows the cleanest separation between what the problem says and what the LLM is recognizing it as?
   4. **Naive-question productivity** — which framing's naive questions would, if answered, most change the shape of the solution?

   On genuine ties across all four criteria, prefer the framing with more items in the "Unknown" list — under-determined beats over-determined at this phase.

   **All candidates fail the Explanatory-fit hard filter.** If every candidate framing fails the hard filter, Phase 1 does not block or ask — it emits the **least-bad candidate** (the one whose deep-structure principle is closest to explaining the success criterion, even if imperfectly) and relies on Stress-Test's loop-to-Frame logic to catch the frame-level problem downstream. This preserves the "Phase 1 always emits" invariant below and lets the pipeline surface the failure through evidence rather than refusal.

   Emit the top-scoring framing (or least-bad if all fail the hard filter) as the selected frame.

3. **Emit the selected frame** as the phase output. (In the final XML: this becomes the `<frame>` block.)

**Output format.** Single structured frame block.

**Loop triggers from this phase.** None outbound. **Phase 1 always emits its top-scoring framing — there is no absolute quality threshold.** Frame does not self-diagnose. A weak frame is detected downstream by Stress-Test's loop-to-Frame logic, not by Phase 1 refusing to emit.

---

### Phase 2 — DIVERGE

**Input.** The selected frame from Phase 1. (Plus any failure report if this is a loop retry — see INCUBATE LOOP.)

**What the LLM is instructed to do.**

1. **Run three generative branches in parallel**, each seeded by the frame. Each branch produces `N_candidates_per_branch` candidates (default 3, range 2–5 from stakes calibration; Section 5.1 is canonical).

   | Branch | Seed | Operation |
   |---|---|---|
   | **Associative** *(§1.2 DMN)* | Deep-structure principle | Chain through associations. "What else is governed by this principle?" "What other problems with this shape have answers?" Walk associative paths without committing to quality until Stress-Test |
   | **Combinatorial** *(§1.5 + da Vinci Stage 8)* | The modular elements of the frame (Knowns + Unknowns, treated as pieces) | Enumerate combinations of elements that could satisfy the success criterion. Include **overinclusive** combinations — pairings that don't obviously go together |
   | **Analogical** *(§1.4 + da Vinci Stage 4)* | Deep-structure principle | For domains that have solved problems with this same deep-structure principle (not the same surface features), import the solution and adapt. Name the source domain explicitly — the annotation matters for Synthesize |

2. **De-duplicate across branches.** *(Engineering step, not from the research corpus.)* The LLM judges structural equivalence — candidates are the same if they propose the same mechanism or operation to satisfy the success criterion, even if the phrasing or analogy used to arrive at them differs. Merge structurally-equivalent candidates into one and record both (or all) branch lineages. Cheap insurance against triple-counting in Stress-Test.

3. **Emit the candidate set.** After de-dup, typically 6–12 candidates. Each candidate carries:
   - Statement of the candidate (what it actually proposes)
   - Branch lineage (one or more of associative / combinatorial / analogical; plus source domain if analogical)
   - One-line initial plausibility note (the LLM's read — *not* a full evaluation, that is Stress-Test's job)

**Output format.** A list of candidate blocks.

**Loop triggers from this phase.** None outbound. If Diverge genuinely cannot produce candidates, that is a Frame failure and will surface in Stress-Test's loop logic.

---

### Phase 3 — STRESS-TEST

**Input.** The frame (from Phase 1) and the candidate set (from Phase 2). (Plus failure report if loop retry.)

**What the LLM is instructed to do.**

1. **Apply three attacks in parallel to every candidate.** Each attack produces an **observation** — the attack's finding about the candidate. Step 1 records observations; Step 2 makes the kill judgment. Keeping the two steps separate is deliberate: observations are closer to raw sensor data, kill judgments require absorption analysis against the candidate's structure.

   | Attack | Operation | Observation recorded |
   |---|---|---|
   | **Contradiction-holding** *(Janusian)* | Construct the strongest plausible negation of the candidate. Ask: in what respect are both the candidate *and* its negation true? | The respect (if any) in which the contradiction holds, and whether that respect touches the success criterion |
   | **Limit-case** *(Einstein)* | Construct the most adversarial boundary conditions — parameter extremes, degenerate inputs, adversarial edge cases, combinations outside the training distribution of the analogy (if analogical) | Which boundaries the candidate does and does not hold under |
   | **Bayesian update** *(Feynman + test-effect-strength)* | Identify the strongest counterevidence available against the candidate. Starting from the initial plausibility, update confidence in the candidate under that counterevidence. State the updated confidence explicitly. | The updated confidence, and whether it has dropped below the "no longer credible" bar — the bar's aggressiveness is set by the `bayesian_bar` parameter (Section 5.1; low = lenient killing, medium = kill below even odds, high = aggressive killing on any meaningful downward shift). |

2. **Classify each candidate's outcome.** For each candidate, examine the three attack observations and decide:

   - **Survived** — the candidate has either no hostile observations, or has hostile observations whose objections its structure **absorbs** (the candidate has a feature that neutralizes the objection, or the objection describes a tradeoff the candidate explicitly accepts). Annotate survivors with the strongest surviving objection per attack type — these become tradeoffs the final answer inherits. *(This annotation is da Vinci Stage 6 — Abductive Reasoning, explaining why the survivor survived.)*
   - **Killed** — the candidate has at least one hostile observation whose objection its structure **cannot absorb**. Annotate with the killing observation, tagged with which attack delivered it.

   "Absorption" is an LLM judgment: can the candidate, as it stands, answer the objection without being redefined? If yes, the hit is survivable. If the candidate would have to be restructured to answer the objection, the hit is a kill.

3. **Apply loop-decision logic** after all candidates are classified.

   **Precondition note:** the LOOP actions below apply only when loop budget remains. If budget is exhausted, fall through to the final row regardless of finding.

   | Situation | Action |
   |---|---|
   | ≥1 candidate survived | Forward the pruned set (survivors only) to Synthesize |
   | All candidates killed, and the killing observations share a common root cause that traces to the *frame* (same naive-question gap kills every candidate, or same assumed item is the failure point) | **LOOP to Frame** with failure report — the frame was wrong |
   | All candidates killed, but the killing observations are candidate-specific (different failure modes per candidate, no common frame-level root cause) | **LOOP to Diverge** with failure report — the candidate pool was insufficient, the frame is fine |
   | All candidates killed, root cause **indeterminate** (failure pattern doesn't clearly point to frame vs. candidate) | **LOOP to Diverge** with failure report — default to retrying candidates first, because Diverge is cheaper than Frame to retry; if Diverge's retry also fails, the frame-level pattern will surface on the next loop |
   | Loop budget exhausted (regardless of finding) | Forward the candidate with the highest **residual plausibility** in the LLM's judgment — typically the candidate whose killing objection is weakest or most qualified — with a note to Synthesize; Synthesize handles the escape-hatch |

**Output format (forward case).** A pruned candidate set — survivors only, each with per-attack annotations (tradeoffs).
**Output format (loop case).** A failure report (structure defined in INCUBATE LOOP below).

---

### Phase 4 — SYNTHESIZE

**Input.** The frame (Phase 1), the pruned candidate set with annotations (Phase 3). (Plus failure report if loop retry — though Synthesize is never a retry target; see INCUBATE LOOP.)

**What the LLM is instructed to do.**

1. **Combine surviving candidates into the answer.** Four sub-cases:

   | Case | Action |
   |---|---|
   | **One survivor** | Emit it as the answer. Attach its Stress-Test annotations as stated tradeoffs. |
   | **Multiple survivors with a clear dominance order** | Pick the dominant one. Record runners-up as alternatives, with the specific reason each was preferred-against. |
   | **Multiple survivors without clear dominance** | Emit a ranked set of the top 2–3. State the grounds each excels on and the dimension along which genuine uncertainty remains. Phase 4 Step 1 still selects a `<primary>` (the rank-1 option) so downstream consumers and the Output Gate's answer-trace check (G2) always have a single anchor. |
   | **Zero survivors, budget exhausted (Phase 3 forwarded a dead candidate with highest residual plausibility)** | Treat the forwarded dead candidate as the provisional answer. Attach its killing observation verbatim as the leading tradeoff (not an inline absorption). This case **skips Steps 2, 3, and 4 entirely** and emits directly with an `<escape_hatch>` block naming the killing observation as the unresolved issue and stating that the answer is a forced best-available pick, not a survivor. Step 2 (schema elaboration) is skipped because elaborating a dead candidate is wasted work; Step 3 (gap scan) and Step 4 (loop-decision) are skipped because the loop-decision table's budget-exhausted row would route to the same escape-hatch anyway. |

2. **Work out the details** *(da Vinci Stage 9 — Schema Elaboration)*. For the chosen answer(s), fill in the **structural detail** the frame's success criterion requires — steps, mechanisms, responsibilities, tradeoff notes. Structural detail means the answer's shape and relationships; implementation detail means the concrete code or config.

   *Example of the line.* **Structural detail:** "the answer has three components A, B, C, where B depends on A, and C reconciles both." **Implementation detail:** "component A uses a hash map with fields x, y, z." Synthesize produces the former; any downstream skill invoked on the output produces the latter.

3. **Run the gap scan against the frame** *(Feynman gap detection)*. Systematic check, applied in this order:

   1. **Success criterion coverage** — For every element of the frame's success criterion: is it addressed by the answer?
   2. **Unknown position** — For every item in the frame's **Unknown** list: has the answer stated a position on it or explicitly deferred?
   3. **Naive-question answering** — For every naive question: does the answer address it?
   4. **Unexplored candidate dimension** — For each item in the frame's **Unknown** list, did Diverge generate candidates that took a position on it? An Unknown with no candidate coverage is an unexplored dimension.
   5. **Unattacked boundary** — For each item in the frame's **Assumed** list, did Stress-Test's limit-case attack probe whether the assumption breaks? An Assumed item that was never challenged is an unattacked boundary.

4. **Apply loop-decision logic.**

   **Precondition note:** the LOOP actions below apply only when loop budget remains. If budget is exhausted, fall through to the final row regardless of finding.

   **Multi-gap resolution.** When the gap scan finds multiple gaps at once, the LLM fires **one loop per pass**, choosing by priority order: **Frame > Diverge > Stress-Test > forward**. Any remaining gaps are re-detected on the next pass. The rationale: fixing the frame invalidates downstream work, so frame errors must be fixed before generating more candidates; generating candidates must precede attacking them.

   | Gap-scan finding | Action |
   |---|---|
   | No gaps | Emit the final XML output |
   | Gap that traces to a wrong frame (the question itself was wrong) | **LOOP to Frame** with failure report — reframe |
   | Unexplored candidate dimension (from check 4) | **LOOP to Diverge** with failure report — generate candidates covering the missing Unknown |
   | Unattacked boundary (from check 5) | **LOOP to Stress-Test** with failure report — attack the existing survivors on the missing Assumed item |
   | Gap in checks 1–3 (answer fails to address success criterion, unknowns, or naive questions) but no upstream cause identified | **LOOP to Stress-Test** with failure report — default: re-examine the current survivors' attack annotations for an objection that was under-weighted |
   | Loop budget exhausted (regardless of finding) | Emit the final output with an **escape-hatch block** explicitly stating what could not be resolved (which check failed, what would need to happen to resolve it) |

**Output format.** The final XML output (schema in Section 5). Always emits — either a clean answer, or an answer with an escape-hatch block.

---

### INCUBATE LOOP mechanics

When any phase triggers a loop, it does not silently retry. It emits a **structured failure report** that the upstream phase receives as *additional context* alongside its normal input.

**Failure report schema:**

```
<failure_report>
  <from_phase>{phase that detected the failure}</from_phase>
  <to_phase>{phase being re-run}</to_phase>
  <what_was_tried>
    {the failing phase's output that triggered the loop — dead candidates with their killing objections, unexplored dimensions, wrong frame, etc.}
  </what_was_tried>
  <why_it_failed>
    {the specific classification — "all candidates share fatal objection X that traces to assumed item Y in the frame", or "gap scan found dimension Z never generated", etc.}
  </why_it_failed>
  <instruction_for_retry>
    {explicit instruction for the upstream phase's retry — e.g., "generate candidate framings that do not assume Y", or "generate candidates covering dimension Z", or "attack the survivors under boundary condition W"}
  </instruction_for_retry>
</failure_report>
```

**Retry prompt addendum.** The upstream phase's retry prompt is the same as its first-pass prompt, with one sentence prepended: *"This phase has been retried because of the failure below. Apply the instruction_for_retry and produce output that addresses the stated failure cause, using the retry behavior specified for this phase."* Followed by the failure report verbatim.

**Retry behavior per phase.** Retries are not uniform — each phase preserves or discards previous output based on what it produces. Retry behavior only applies to the *phase that is the loop target*. Downstream phases that run again after the loop are not in "retry mode" — they execute as first-pass over the retried upstream output (see control-flow note below).

| Phase being retried (loop target) | Retry behavior |
|---|---|
| FRAME | **Replace.** Generate fresh candidate framings informed by the failure report, then run selection again. The previously-selected frame is discarded — if it had been right, Stress-Test wouldn't have looped back. After a Frame retry, all downstream phases run first-pass over the new frame: Diverge generates fresh candidates (not extending the old set, because the old candidates were seeded from the wrong frame), Stress-Test attacks them fresh. |
| DIVERGE | **Extend.** Keep previously generated candidates. Generate *additional* candidates targeted at the failure report's instruction (e.g., "covering dimension Z"). Re-run de-dup across the full set. After a Diverge retry, Stress-Test runs as a first-pass on the *new* candidates only — it does not re-attack the previously-classified ones. Existing survivors and their annotations are preserved; existing dead candidates stay dead. The result set forwarded to Synthesize is the union of prior survivors plus the newly-attacked additions. |
| STRESS-TEST | **Extend.** Keep surviving candidates and their existing attack annotations. Apply the failure report's additional attack (e.g., "attack boundary W") to each. Reclassify under the expanded attack set — a candidate that survived three attacks may die under the fourth. This retry behavior fires only when Stress-Test is itself the loop target (Synthesize → Stress-Test, unattacked-boundary case). |
| SYNTHESIZE | **Not a retry target.** All loops from Synthesize go upstream (to Frame, Diverge, or Stress-Test). When control eventually returns to Synthesize, it runs as a first-pass execution over the new upstream state, not as a retry. |

**Post-retry control-flow note.** A loop costs one unit of `loop_budget` regardless of how many downstream phases subsequently run. Example: a Synthesize → Diverge loop consumes 1 unit; Diverge extends; Stress-Test runs first-pass on the new candidates (no additional budget cost); Synthesize runs first-pass on the expanded survivor set (no additional budget cost). If Synthesize then finds another gap and loops again, that consumes the *next* unit. Budget is per-loop-trigger, not per-phase-execution.

**Why this beats human incubation.** A human waiting on unconscious association has no explicit memory of *why* the previous attempt failed — the incubation is stochastic and hope-driven. The AI retry has the failure cause stated in the prompt and an explicit instruction on what to do differently. The AI surpasses the human because the retry is *informed*, not blind — and because the per-phase retry behavior preserves good work (extend) where it exists rather than discarding everything (replace) the way a human "stepping away and coming back" effectively does.

**Loop budget.** Global N retries per invocation (not per phase). Section 5.1 is canonical: default `loop_budget=2`, range 0–4, set by stakes assessment (low=1, medium=2, high=3). Budget is consumed each time any phase triggers any loop. When exhausted, Synthesize is forced to the escape-hatch path. The budget prevents runaway loops; the escape-hatch prevents silent drops. `loop_budget=0` is a supported calibration — the pipeline runs as a pure forward pass with no retries, and any loop trigger falls straight through to the escape-hatch.

---

## Design — Section 5 of 6 (revised): Depth Calibration, Verification Gates, Confidence Tiers, XML Output Schema

### 5.1 Depth calibration parameters

The pipeline exposes five parameters. All have defaults; all can be auto-tuned from the problem's stakes; none are user-facing mode flags.

| Parameter | Meaning | Default | Range |
|---|---|---|---|
| `N_framings` | How many candidate framings Phase 1 generates | 3 | 2–5 |
| `N_candidates_per_branch` | How many candidates each Diverge branch generates | 3 | 2–5 |
| `loop_budget` | Total retries allowed across the whole pipeline per invocation | 2 | 0–4 |
| `bayesian_bar` | How easily the Bayesian attack kills a candidate (see semantics below) | medium | low / medium / high |
| `gap_scan_depth` | How thorough Phase 4's gap scan is (which checks run) | full | core / full |

**Canonical values.** Section 5 is canonical for all parameter values. The ranges stated inside Section 4 (e.g., "range 2–5 candidates per branch") describe the full parameter range across stakes levels and manual override, not the default for a single invocation.

**`gap_scan_depth` meaning.** `core` runs checks 1–3 only (success criterion, unknowns, naive questions). `full` runs all five (adds unexplored dimensions and unattacked boundaries). Used when a fast pass is wanted without skipping the pipeline entirely.

**`bayesian_bar` meaning (explicit direction).** Higher values = *easier* to kill a candidate, not harder. The parameter tunes how aggressive the Bayesian-update attack is in marking a candidate "no longer credible" under the strongest counterevidence:
- `low` — the attack is lenient. It only marks a candidate not credible if the counterevidence makes the candidate clearly unsupportable. Best for exploratory, low-stakes thinking where surviving a soft attack is acceptable.
- `medium` (default) — the attack marks a candidate not credible if counterevidence brings its posterior confidence below roughly even odds.
- `high` — the attack is aggressive. It marks a candidate not credible at any meaningful downward shift in confidence. Best for high-stakes decisions where you want to filter ruthlessly and accept that some acceptable candidates will be killed. High stakes → stricter standard for remaining credible → more kills → more likely to loop to Diverge for replacements → higher-integrity surviving set.

**Auto-calibration from problem stakes.** Stakes assessment runs immediately after both Hard Gate checks (Section 4) pass, before Phase 1 begins. It is a separate short pass in the same pre-pipeline slot — no change to Section 4's Hard Gate itself. The LLM performs a single-pass judgment producing one of three labels:

| Stakes | Signals the LLM looks for | Parameters set |
|---|---|---|
| **low** | Reversible decision, bounded impact, the user signaled it's casual, exploratory thinking | `N_framings=2`, `N_candidates_per_branch=2`, `loop_budget=1`, `bayesian_bar=low`, `gap_scan_depth=core` |
| **medium** (default) | No clear signal either way, standard problem-solving | Defaults from the table above |
| **high** | Irreversible, high-cost, safety-relevant, the user signaled it matters ("this is important"), or compliance/legal/production impact is clear | `N_framings=4`, `N_candidates_per_branch=4`, `loop_budget=3`, `bayesian_bar=high`, `gap_scan_depth=full` |

**Pre-pipeline order (recap).** Hard Gate checks 1 & 2 → stakes assessment → Phase 1. Section 4 is unchanged; this is additive clarification.

**How a "fast" or "deep" pass is requested.** Not as mode flags. A user who wants a fast pass describes the problem as low-stakes or explicitly says "quick thinking, not a full pass"; the stakes assessment picks up the signal and auto-calibrates to `low`. A user who wants a deep pass says "this is important, I need the full treatment"; the assessment auto-calibrates to `high`. Manual override is possible if the user explicitly states parameters ("loop budget 4"), but the default path is stakes → parameters with no flags.

**Why this beats mode flags.** Mode flags force a binary choice at invocation time before the LLM has seen the problem. Stakes-based calibration reads the problem first, then picks parameters. The same user message ("help me decide how to structure this meeting" vs. "help me decide whether to fire this engineer") lands at different depths without the user having to think about flags.

---

### 5.2 Confidence-tier handling

The research corpus assigns confidence tiers that modulate **how the skill annotates its output**, not which operations run. Lower-confidence findings still produce real work in the pipeline; the output simply flags when a result was driven specifically by a lower-confidence mechanism.

**Canonical source.** The corpus contains three confidence-tier statements (Executive Summary table, section-level "Evidence Quality" labels, §6 "Confidence Levels by Finding" table) and they disagree on three cells. Section 5 treats the **§6 table (lines 603–610 of the corpus) as canonical** because (a) it is the purpose-built assessment section, (b) it uses the finer `Medium-High` tier the Executive Summary lacks, (c) it agrees with the section-level `Evidence Quality` labels on 4 of 6 findings, and (d) it is the only source that rates Janusian separately. Where the Executive Summary disagrees (Incubation `Medium` vs. §6 `Medium-High`; Cross-Domain `Medium` vs. §6 `High`; Combinatorial Play `High` vs. §6 `Medium`), §6 wins.

| Research finding | Tier (§6) | How the skill treats it |
|---|---|---|
| Chunking (§1.1) | High | Frame operations emit without caveats |
| DMN↔ECN network switching (§1.2) | High | Architectural property of the pipeline (phase separation) — has no operational output to annotate, inherits the High tier structurally |
| Incubation (§1.3) | Medium-High | INCUBATE LOOP emits with a `<confidence_note>` when loops fire: *"answer refined through N loop retries; loop mechanism has medium-high research support"* |
| Cross-Domain Pattern Transfer (§1.4) | High | Analogical branch emits without caveats |
| Combinatorial Play (§1.5) | Medium | Combinatorial branch candidates that become the primary answer carry a `<confidence_note>`: *"combinatorial-play generation — medium-confidence research support"* |
| Janusian Process (§1.5 / §2) | Medium | Contradiction-holding attack results that **change** a candidate's classification (kill → survive or vice versa) carry a `<confidence_note>`: *"classification driven by contradiction-holding attack — medium-confidence research support"* |

**Genius methodologies (Feynman §2.1, Einstein §2.2, da Vinci Observation-vs-Recognition §2.3, da Vinci Nine-Stage Process §2.3):** The corpus treats these as documented case-study techniques and does not assign them quantitative confidence tiers in §6. The skill uses them in Frame, Stress-Test, and Synthesize phases without `<confidence_note>` annotations. The §2.3 da Vinci section carries an "Evidence Quality: High" label (line 270 of the corpus) which is consistent with unannotated emission.

**Principle.** Annotations only fire for Medium and Medium-High tier findings, and only when those findings are *load-bearing* on the emitted answer — not every time the mechanism runs. A Combinatorial-Play-generated candidate that gets filtered out in Stress-Test and doesn't appear in the output needs no note; one that survives to become the answer gets the note.

---

### 5.3 Verification gates

The skill has two gate layers: the **Hard Gate** (pre-pipeline, Section 4) and the **Output Gate** (pre-emission, defined here). The Output Gate runs once, after Phase 4 has produced a candidate for emission, before the XML is actually sent to the user.

**Check application order.** Checks apply in sequence G1 → G7. Any hard fail terminates the gate and emits the escape-hatch immediately. Multiple inline fixes encountered across different checks are applied together before emission. The gate does not loop on itself.

| # | Check | Fail → |
|---|---|---|
| G1 | **Frame presence and completeness** — is there a `<frame>` block with all required child elements non-empty? Required: `<deep_structure_principle>`, `<success_criterion>`, `<known>`, `<unknown>`, `<assumed>`, `<naive_questions>` (with populated what/why/how sub-elements), `<observation_vs_recognition>` (with populated observed/recognized/divergence sub-elements). | Hard fail — emit escape-hatch: *"frame generation produced an incomplete frame block"* |
| G2 | **Answer traces to a pipeline candidate** — does the `<answer><primary>` content correspond to (a) a candidate that appeared in Phase 3's survivor list, **or** (b) the dead candidate Phase 3 forwarded under the budget-exhausted-zero-survivors case (Phase 4 Step 1 sub-case 4) when the output also carries an `<escape_hatch>` block? (Checked against internal pipeline state, not the emitted XML.) | Hard fail — emit escape-hatch: *"answer-candidate trace broken"* |
| G3 | **Tradeoffs populated when Stress-Test produced them** — if any survivor retained surviving objections from Stress-Test, do those objections appear in `<tradeoffs>`? | Fix inline (copy from Phase 3 internal state) |
| G4 | **Gap scan results reflected** — if Phase 4's gap scan found gaps and the loop budget was exhausted, is there an `<escape_hatch>` block stating what wasn't resolved? | Fix inline (emit the escape-hatch from Phase 4's output) |
| G5 | **No fabricated source attributions** — does any attribution (e.g., *"this analogy is from biology"*) match a Diverge branch lineage that was actually recorded? | Remove the offending candidate *and* its attribution. If it was the primary answer, hard fail with escape-hatch: *"analogical candidate with fabricated source domain removed; no clean fallback answer"* |
| G6 | **Frame and answer use the same success criterion** — did the answer drift from the frame's stated success criterion during synthesis? | Fix inline — re-anchor the answer's stated conclusion to the frame's success criterion. If the drift is too large to fix inline (answer genuinely doesn't address the criterion), hard fail with escape-hatch: *"answer drifted from frame success criterion"* |
| G7 | **Escape hatch correctly populated** — if budget was exhausted and any gaps remain, the escape hatch must state *which* gap and *what would resolve it* | Fix inline |

**Why G1 is hard fail, not loop.** Section 2's approved loop graph contains five backward edges, all *within* the 4-phase pipeline. The Output Gate is downstream of Phase 4. Allowing G1 to trigger a Phase 1 regeneration would add a sixth loop edge (post-Phase-4 → Phase 1) that the architecture does not contain. A missing frame at emission time means something broke in the pipeline's internal state; the conservative response is to fail honestly rather than retry.

**Why G6 is inline-fix, not loop-to-Synthesize.** Section 4's retry-behavior table explicitly lists Synthesize as *"Not a retry target."* Success-criterion drift is an answer-rewriting problem, not a synthesis-rerun problem; the fix re-anchors the stated conclusion to the frame without redoing the synthesis work.

**G2 and G5 are hard fails** because they indicate the answer doesn't come from the pipeline's actual work — either the candidate is phantom (G2) or the attribution is invented (G5). The other checks are fixable inline.

**Single-pass.** Unlike prompt-epiphany's 8-check loop, the Output Gate does not loop on itself. Any required retry would have to trigger a phase loop with its own budget accounting — which G1/G6 explicitly do *not* do under the rules above. The gate's purpose is *trust in the output*, not *maximum quality*; maximum quality is the pipeline's job.

---

### 5.4 XML output schema

The skill's output is a single `<epiphany_genius_output>` block. Required elements always appear; optional elements appear only when the pipeline produced relevant content.

```xml
<epiphany_genius_output>

  <!-- Required: the frame Phase 1 selected -->
  <frame>
    <deep_structure_principle>...</deep_structure_principle>
    <success_criterion>...</success_criterion>
    <known>...</known>
    <unknown>...</unknown>
    <assumed>...</assumed>
    <naive_questions>
      <what>...</what>
      <why>...</why>
      <how>...</how>
    </naive_questions>
    <observation_vs_recognition>
      <observed>...</observed>
      <recognized>...</recognized>
      <divergence>...</divergence>
    </observation_vs_recognition>
  </frame>

  <!-- Required: the answer Phase 4 synthesized -->
  <answer>
    <!-- Required: always present, even in the ranked-set case and in the budget-exhausted
         zero-survivor case. <primary> holds the full textual statement of the chosen answer
         (duplicated, not referenced) — in the ranked-set case, its content is the same string
         as the <option rank="1"> content. Output Gate G2 traces against this element. -->
    <primary>...</primary>
    <!-- Optional: appears only in the "multiple survivors with clear dominance" sub-case.
         Mutually exclusive with <ranked_set> — exactly one of <runners_up> or <ranked_set>
         may appear, or neither (single-survivor case). -->
    <runners_up>
      <alternative reason_preferred_against="...">...</alternative>
    </runners_up>
    <!-- Optional: appears only in the "multiple survivors without clear dominance" sub-case.
         Mutually exclusive with <runners_up>. When present, the <option rank="1"> content
         string equals the <primary> content string. -->
    <ranked_set>
      <option rank="1" grounds="...">...</option>
      <option rank="2" grounds="...">...</option>
    </ranked_set>
  </answer>

  <!-- Required: tradeoffs the answer inherits from Stress-Test -->
  <tradeoffs>
    <tradeoff attack="contradiction|limit_case|bayesian">...</tradeoff>
  </tradeoffs>

  <!-- Optional: only when confidence tiers flagged something -->
  <confidence_notes>
    <note source="janusian|combinatorial|incubation">...</note>
  </confidence_notes>

  <!-- Optional: only when loops fired. One <loop> element per loop trigger, in order.
       from/to = phase names, reason = classification string from the failure report,
       pass = 1-indexed sequence counter (pass="1" for the first retry, pass="2" for the second). -->
  <loop_history>
    <loop from="..." to="..." reason="..." pass="1"/>
  </loop_history>

  <!-- Optional: only when loop budget was exhausted -->
  <escape_hatch>
    <unresolved_gap>...</unresolved_gap>
    <what_would_resolve_it>...</what_would_resolve_it>
  </escape_hatch>

  <!-- Required: provenance for downstream consumption by another skill -->
  <provenance>
    <stakes>low|medium|high</stakes>
    <parameters>
      <N_framings>...</N_framings>
      <N_candidates_per_branch>...</N_candidates_per_branch>
      <loop_budget>...</loop_budget>
      <bayesian_bar>low|medium|high</bayesian_bar>
      <gap_scan_depth>core|full</gap_scan_depth>
      <loops_used>...</loops_used>
    </parameters>
  </provenance>

</epiphany_genius_output>
```

**Why this schema, specifically.** Three design choices are worth noting:
1. **`<frame>` is required on every output** — downstream skills consuming this output (via Section 1's Mode B pre-step pattern) need the frame as context, not just the answer. A bare answer without its frame is much less useful to a composing skill.
2. **`<tradeoffs>` is required even when empty** — consumers can check for "no tradeoffs" without parsing differently. An empty `<tradeoffs/>` means "Stress-Test found no surviving objections," which is valuable information.
3. **`<provenance>` is required** — it tells a downstream consumer (human or another skill) how much work went into this output, which calibrates trust. A loop-budget-exhausted output with an escape hatch is not the same as a clean first-pass output; the provenance makes the difference legible.

**What the schema deliberately does not expose.** There is no `<stress_test_results>` element, no Phase 2 candidate dump, no attack-by-attack log. Pipeline internals stay internal; the output ships conclusions with just enough provenance for trust calibration. The Output Gate (G2) verifies the answer-to-survivor link against *internal* state, so no leakage is needed.

---

### 5.5 Relationship to Section 1's composition model

Section 1 committed to Composition Model D: standalone invocation + documented "use as pre-step" pattern, with the XML output structured so the next skill can paste it as context. The schema above delivers on that commitment:

- The XML is self-describing — a downstream skill can read `<frame>`, `<answer>`, `<tradeoffs>` without any epiphany-genius-specific parser.
- `<provenance>` lets the downstream skill decide how much to trust the output (a low-stakes first-pass is different from a high-stakes three-loop pass).
- The `<escape_hatch>` block, when present, surfaces unresolved issues explicitly rather than hiding them in the answer.

No other skill needs to be modified to consume this. A human reader can read the XML as a structured report; an agent pasting it as context to the next skill gets the same information as document text.

---

## Design — Section 6 of 6: Corporate Enterprise Use

This is the final design section. It covers (a) how the existing pipeline handles corporate enterprise brainstorming and planning without adding new components, (b) how the four phases map to corporate vocabulary for readers coming from a corporate context, and (c) the final SKILL.md skeleton for the self-contained skill.

### 6.1 Corporate use: same pipeline, different trigger signals

The dual-purpose requirement is not satisfied by a "corporate mode." It is satisfied by the **stakes assessment** already defined in 5.1 and the **activation phrases** already defined in Section 1. Corporate invocations land in the `medium` or `high` stakes row and get the appropriate parameters without any new code path.

The table below lists corporate scenario signals the LLM should recognize during stakes assessment. They do not introduce new logic — they are triggers the existing stakes classifier already handles.

| Corporate scenario | Stakes assessment reads | Resulting parameters |
|---|---|---|
| Strategic decision framing ("should we acquire X?", "build vs. buy", market entry) | **high** (irreversible, strategic impact) | `N_framings=4`, `N_candidates_per_branch=4`, `loop_budget=3`, `bayesian_bar=high`, `gap_scan_depth=full` |
| Cross-functional planning ("how do we structure the product launch?") | **medium** (visible, costly to undo) | Defaults |
| Executive ideation ("what are we missing in our Q3 roadmap?") | **medium** | Defaults |
| Crisis response / post-incident ("why did X fail and what do we do") | **high** (safety/financial impact) | High-stakes defaults |
| Quick retro brainstorm, low-stakes workshop | **low** | `N_framings=2`, `N_candidates_per_branch=2`, `loop_budget=1`, `bayesian_bar=low`, `gap_scan_depth=core` |

**Nothing from Sections 1–5 changes for corporate use.** Sections 6.1 and 6.2 only describe where the existing pipeline is exercised more heavily and how its phases map to corporate vocabulary.

**What the corporate use case exercises more heavily than standalone thought-enhancement use:**
- The **frame's success criterion field** — corporate decisions often have contested success criteria that the naive-questions tool surfaces as disagreements stakeholders didn't know they had.
- The **analogical branch** in Diverge — cross-domain reference to analogous prior situations is an AI-native fit for the analogical branch.
- The **ranked set** output mode — executives typically want a ranked short list, not a single answer.
- The **Janusian branch** — for decisions with real tradeoffs (grow vs. protect margin, speed vs. safety) the contradiction-holding attack is most productive here.

No new components. Heavier use of existing ones.

### 6.2 Phase mapping to corporate vocabulary

For readers coming from a corporate context rather than a cognitive-science context, the phases map to familiar language:

| Phase | Cognitive-science name | Corporate-facing name |
|---|---|---|
| 1 | FRAME | Problem framing & success-criterion alignment |
| 2 | DIVERGE | Structured ideation |
| 3 | STRESS-TEST | Risk surfacing / critical review |
| 4 | SYNTHESIZE | Decision framing / recommendation packaging |

This mapping is explanatory only — the skill runs the same pipeline regardless of whether a user calls it by cognitive-science terminology ("frame this problem") or corporate terminology ("I need to reframe this decision"). No functional branching on vocabulary.

### 6.3 SKILL.md skeleton (final deliverable)

This is the complete, self-contained skeleton for the skill's SKILL.md file. It reflects every decision approved in Sections 1–6 and requires no external content to function.

```markdown
---
name: epiphany-genius
description: Unified reasoning enhancer that applies research-backed genius-level problem-solving techniques (chunking, DMN↔ECN switching, incubation, cross-domain transfer, combinatorial play, Feynman/Einstein/da Vinci/Janusian methodologies) as a 4-phase pipeline with stakes-based calibration and an incubation loop. Invoked explicitly by name or via /epiphany-genius; composable as a documented pre-step for any other skill.
---

# Epiphany Genius

## Trigger Conditions
[From Section 1]
- /epiphany-genius slash command
- Explicit invocation by name ("use epiphany-genius on this")
- Never auto-activates, never intercepts other skills

## Composition Model
[From Section 1 — Model D]
- Standalone: runs end-to-end, emits structured XML output
- Pre-step pattern: another skill's SKILL.md documents epiphany-genius as a pre-step; the host skill's instructions describe how to consume the XML output as context

## Hard Gate
[From Section 4]
- Check 1: input has a discernible problem or question (not just "help")
- Check 2: input has enough substance for framing (not fundamentally ambiguous)
- Both pass → stakes assessment → Phase 1

## Stakes Assessment
[From Section 5.1]
- Runs after Hard Gate, before Phase 1
- Produces low | medium | high
- Sets parameters: N_framings, N_candidates_per_branch, loop_budget, bayesian_bar, gap_scan_depth

## Pipeline

### Phase 1: FRAME
[From Section 4 Phase 1]
- Deep-structure principle extraction (Chunking)
- Known / Unknown / Assumed decomposition
- Naive questions (Feynman)
- Observation vs. recognition (da Vinci)
- Emit N_framings candidate frames
- LLM-judgment selects the strongest candidate frame. Explanatory fit is a hard filter — a framing that fails it is out of contention regardless of other strengths. If all candidate framings fail the Explanatory fit filter, emit the least-bad and rely on Stress-Test's loop-back to Frame.

### Phase 2: DIVERGE
[From Section 4 Phase 2]
- DMN-mode generation only (ECN-mode lives in Phase 3 — phase separation per Section 3, Rationale B)
- Associative branch (DMN — chain through associations from deep-structure principle)
- Combinatorial branch (§1.5 + da Vinci Stage 8 — enumerate element combinations, including overinclusive)
- Analogical branch (§1.4 + da Vinci Stage 4 — Cross-Domain Pattern Transfer)
- Deduplicate structurally equivalent candidates
- Per-branch cap: N_candidates_per_branch. Total candidates entering Stress-Test = N_candidates_per_branch × 3 (active Diverge branches), after structural dedup.

### Phase 3: STRESS-TEST
[From Section 4 Phase 3 — ECN-mode; three attacks applied in parallel per candidate]
- Contradiction-holding attack (Janusian §1.5/§2)
- Limit-case attack (Einstein thought experiments §2.2)
- Bayesian-update attack (Feynman Bayesian + test-effect-strength §2.1)
- Per-attack observation recorded, then absorption-based kill classification (Section 4 Phase 3)
- Loop-back to Diverge if all candidates killed but frame still explains success criterion
- Loop-back to Frame if failure pattern shows the frame itself was wrong
- Loop-back to Diverge (default) when the root cause is indeterminate
- Budget-exhausted path: forward the dead candidate with highest residual plausibility; Phase 4 routes to escape-hatch

### Phase 4: SYNTHESIZE
[From Section 4 Phase 4]
- Schema elaboration (da Vinci Stage 9) — skipped for the zero-survivors-budget-exhausted sub-case
- Selection sub-cases: one survivor / multiple survivors with clear dominance / multiple survivors without clear dominance (ranked set) / zero survivors with budget exhausted (escape-hatch direct)
- Gap scan (core or full) against frame
- Gap-scan loop-backs per Section 2 loop table

## INCUBATE LOOP
[From Section 2 — the five approved backward edges, verbatim]
| From | To | Trigger |
|---|---|---|
| Stress-Test | Diverge | All candidates killed, frame still explains success criterion → need more candidates |
| Stress-Test | Frame | All candidates killed, failure pattern shows frame was wrong → reframe |
| Synthesize | Stress-Test | Gap scan finds untested boundary on an existing candidate → attack it |
| Synthesize | Diverge | Gap scan finds a candidate dimension that was never explored → generate for it |
| Synthesize | Frame | Gap scan finds the question was wrong → reframe |

- Governed by loop_budget (per-invocation cap, set by stakes assessment)
- Each backward edge consumes one loop_budget unit

## Confidence Tier Handling
[From Section 5.2]
- §6 corpus table canonical
- Annotations fire only for Medium / Medium-High tiers when load-bearing on the emitted answer

## Output Gate
[From Section 5.3 — application order G1 → G7, sequential, single-pass]
- G1 Frame presence and completeness — hard fail
- G2 Answer traces to a pipeline candidate (survivor OR forwarded dead candidate in escape-hatch case) — hard fail
- G3 Tradeoffs populated when Stress-Test produced them — inline fix
- G4 Gap scan results reflected — if Phase 4's gap scan found gaps and loop budget was exhausted, verify that `<escape_hatch>` is present naming the unresolved gap — inline fix (emit Phase 4's escape-hatch). G4 does NOT loop; all looping is Phase 4's internal responsibility.
- G5 No fabricated source attributions — remove candidate and attribution; hard fail if primary
- G6 Frame and answer use the same success criterion — inline fix (re-anchor); hard fail if drift too large
- G7 Escape hatch correctly populated when budget exhausted — inline fix

## Output Schema
[From Section 5.4]
- <epiphany_genius_output> root
- Required: <frame>, <answer> (with <primary>), <tradeoffs>, <provenance>
- Optional: <confidence_notes>, <loop_history>, <escape_hatch>, <answer><runners_up|ranked_set>> (mutually exclusive)

## Corporate Use
[From Section 6 — same pipeline, no branching]
- Corporate enterprise brainstorming and planning is handled by the existing stakes assessment classifying invocations as medium or high (strategic decisions, cross-functional planning, executive ideation, crisis response), not by a separate code path.
- Phase ↔ corporate vocabulary mapping: Phase 1 = Problem framing & success-criterion alignment; Phase 2 = Structured ideation; Phase 3 = Risk surfacing / critical review; Phase 4 = Decision framing / recommendation packaging.
```

---

## Document status

All six sections approved through brainstorming dialogue, verified for cross-section consistency (2026-04-08 integration pass + two deep-audit passes), and made fully self-contained (no dependency on any planned second skill). Ready for user review before advancing to writing-plans.
