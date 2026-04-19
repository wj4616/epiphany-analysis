# epiphany-genius

> What if you could get an AI to think like Einstein *and* Feynman *and* Darwin
> *and* seven other history-changing minds — all at once, on the same problem?

`epiphany-genius` is a Claude Code skill that runs your problem through a
multi-stage reasoning pipeline. Each stage is designed around a specific
cognitive move that a genius thinker used habitually — stripping to
irreducibles, mapping across distant domains, hunting for disconfirming
evidence, forcing precision through jargon removal. The pipeline makes the AI
actually *do* these moves instead of just sounding insightful.

**One prompt in, a verified, scope-limited, falsification-tested conclusion
out.**

**Version:** 1.1.0 · **Status:** Stable · **Runtime:** Claude Code skill (self-contained)

---

## How it works — the big picture

You give it a problem. Here's what happens:

```
Your problem
     │
     ▼
┌─────────────────────┐
│  S1: Break it apart │  ← Einstein: find the irreducible core
│  into primitives     │  ← Darwin: gather evidence before guessing
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  S2: Find analogies │  ← Newton: transfer structure, not surface
│  across domains     │  ← Archimedes: name the binding constraint
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  S3: Explore the     │  ← Poincaré: let weak associations surface
│  periphery           │  ← Da Vinci: cross boundaries between fields
└─────────┬───────────┘
          │
     ─── Mode Shift ───  ← Stop generating. Start evaluating.
          │
          ▼
┌─────────────────────┐
│  S5: Force precision │  ← Feynman: can you explain it to a 5-year-old?
│                      │  ← Da Vinci: draw it, equation it, describe it
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  S6: Try to break it│  ← Darwin: hunt disconfirming evidence
│                      │  ← Feynman: which hypothesis are you protecting?
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  S7: Integrate and   │  ← Newton: does it generalize?
│  verify              │  ← von Neumann: weave it all together
└─────────┬───────────┘
          │
          ▼
     Final report
  (11 sections, V1–V7 verified)
```

The key insight: **most AI reasoning fails not because the model isn't smart
enough, but because it skips the steps that produce real insight.** It jumps to
the fluent, confident, locally plausible answer. This pipeline structurally
prevents that. Each stage forces a specific cognitive move, persists the
intermediate artifacts, and subjects the final synthesis to a seven-gate
verification battery before you see it.

---

## Installation

```bash
git clone https://github.com/wj4616/epiphany-genius.git ~/.claude/skills/epiphany-genius
```

The `/epiphany-genius` slash command will be available in
[Claude Code](https://github.com/anthropics/claude-code) on the next session
start. No other dependencies — the skill is self-contained (Bash + `jq` +
`python3` for the validator).

---

## Quick start

```bash
# Auto-detects depth from your input length and complexity
/epiphany-genius What is the irreducible core of a pre-mortem procedure?

# Force a depth scale
/epiphany-genius --deep    [paste a long technical spec]
/epiphany-genius --minimal [short question]

# Pattern-detection mode (Ramanujan pipeline)
/epiphany-genius --conjecture [numerical or pattern-heavy data]

# Machine-readable XML output
/epiphany-genius --xml [problem]

# Resume an interrupted session
/epiphany-genius --resume 20260413-pre-mortem-procedures
```

Every run produces a self-contained session folder under
`~/docs/epiphany/genius/YYYYMMDD-topic-slug/` with every stage's raw output,
the final report, and verification/test reports.

---

## The 19 cognitive traits — and how the AI exploits them

These aren't personality traits. They're **reproducible cognitive moves** —
specific things you can tell an AI to do that produce better reasoning. Each one
is based on a thinking habit that a historical genius used consistently.

The table below shows all 19 traits, what they mean in plain language, and
which pipeline stage runs them.

### Einstein's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 1 | **Reduce to irreducible primitives** | Before solving anything, strip it down to the simplest pieces that can't be broken further. Don't start building on assumptions. | The AI enumerates 20+ primitives from your input, then simplicity-strips them to the ones that can't be decomposed further. This prevents solving the wrong problem. | S1 |
| 2 | **Multiple observer frames** | Don't switch between perspectives — hold several at once. An insider sees the system working; an outsider sees where it breaks; a measurement instrument sees what's quantifiable. | The AI embeds your problem in three simultaneous frames and runs a simulation checklist in each. What one frame misses, another catches. | S4 |
| 3 | **Extreme scenario construction** | Don't just test the normal case. Push every parameter to its boundary and see what breaks. | The AI constructs 8 boundary-regime scenarios and looks for singularities, imbalances, and limit cases where the reasoning collapses. | S6 |
| 4 | **Einstellung override** | The biggest block to insight is the solution you already found. Explicitly abandon it. | When the pipeline gets stuck (S3 comes back empty), the AI is instructed to drop its current approach entirely and try three different ways to reframe the problem. | S3.1 |

### Feynman's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 5 | **Jargon removal (the five-year-old test)** | If you can't explain it simply, you don't understand it. Strip away the technical language and see what's left. | The AI renders every claim at five vocabulary levels: from "explain it to a five-year-old" up to formal mathematical notation. What survives all five levels is the real content; what collapses at level 2 was jargon hiding a gap. | S5 |
| 6 | **Error-seeking / symmetric scrutiny** | Don't look for confirmation. Look for what's wrong. Give alternative explanations the same scrutiny you gave your favorite. | The AI runs three inference modes (abductive, deductive, inductive) in parallel and requires that each alternative hypothesis gets the same depth of analysis as the leading one. | S6, S7 |
| 7 | **Millikan test** | When you work harder to defend one hypothesis than another, that differential effort is a signal — you might be protecting a weak claim. | The AI compares how much reasoning effort each hypothesis required and flags cases where one hypothesis was "protected" (defended with more elaborate reasoning than the alternatives). | S6 |

### Newton's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 8 | **Structural analogy transfer** | The deep structure of one domain can solve problems in another — but you have to transfer the *structure*, not the surface features. "Heat flows like water" transfers the math of fluid flow, not the wetness. | The AI surveys 20+ candidate domains and produces five structurally-distinct mappings. Each mapping transfers a mathematical or logical structure from the source domain to your problem, not just a metaphor. | S2 |
| 9 | **Generalization-first** | Don't just ask "does this work?" — ask "does this work *in general*, or only for this specific case?" | After integration, the AI explicitly tests whether the conclusion generalizes beyond the specific input or is a one-off result. The verdict appears in the final report. | S7 |

### Turing's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 10 | **Constructive specification** | Don't say "it would work." Say what observable outcome would demonstrate it works. If you can't name the observable, the claim isn't concrete. | For every major claim, the AI produces a "minimal demonstrating object" — the smallest concrete thing that would confirm or refute the claim. Vague claims get flagged. | S1, S5 |
| 11 | **Formal consequence testing** | If your claim implies X, does X actually hold? Chase the logical consequences. | The AI generates ten parallel forward-chain consequence trees (3–5 steps deep) from each claim. If a consequence contradicts known facts, the claim is weakened. | S5, S6 |

### Da Vinci's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 12 | **Domain-agnostic description** | Describe the phenomenon without using the jargon of any single field. "Something that builds up and then releases suddenly" applies to earthquakes, capacitors, and emotional outbursts. | The AI re-describes the binding constraint in terms that could belong to any domain. This is the bridge between your problem and distant analogies — it makes the structure visible without the domain-specific labels. | S2 |
| 13 | **Multi-representation** | One representation hides what another reveals. A diagram shows spatial relationships; an equation shows quantitative ones; a procedure shows temporal ones. | The AI renders every key claim in three representations: a diagram, an equation, and a step-by-step procedure. What's invisible in one may be obvious in another. | S5 |
| 14 | **Cross-domain boundary observation** | The most interesting things happen at the boundaries between fields, not in the well-explored centers. | The AI's spreading activation (S3) deliberately propagates across domain boundaries, looking for nodes activated from multiple independent fields. These convergent nodes are the most likely sources of non-obvious insight. | S3 |

### Tesla's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 15 | **Pre-visualize the complete system** | Before building anything, imagine the whole thing running. Identify what you'd measure to confirm it works. | The AI constructs a full system model from the S1 primitives and runs Tesla's simulation checklist: energy imbalances, singularities, limit cases. It also produces a "measurement plan" — what specific observable would confirm success. | S1, S4 |

### Darwin's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 16 | **Evidence before hypotheses** | Accumulate observations first. Generate explanations second. If you hypothesize too early, you filter what you see. | The AI explicitly separates evidence gathering (S1, S3) from hypothesis generation (S6). The Mode Shift boundary between ideation and synthesis enforces this discipline. | S1, S3 |
| 17 | **Golden-rule disconfirming search** | Treat your own hypothesis the way you'd treat a competitor's. Search hardest for the evidence that would defeat it. | The AI runs a dedicated disconfirming-evidence search: counter-examples, failure classes, mechanism gaps. It reports a density score — how many disconfirming items were found. High density doesn't kill the conclusion, but it lowers the confidence band. | S6 |

### von Neumann's moves

| # | Trait | What it means | How the AI exploits it | Stage |
|---|-------|---------------|----------------------|-------|
| 18 | **Unbounded working memory** | Don't compress early. Enumerate in parallel. Let every option coexist until the evaluation phase forces selection. | The AI runs parallel enumeration at multiple stages — 20+ primitives in S1, 20+ domain candidates in S2, 10 forward-chain trees in S5. No option is discarded until the synthesis phase evaluates it. | S1, S5, S7 |
| 19 | **Ten-steps-ahead derivation** | Follow the chain further than seems necessary. The interesting consequences are often 5–8 steps deep, not 1–2. | The AI generates forward-chain consequence trees of depth 3–5 (each branch 2 steps), producing ~30 total consequence paths per claim. Shallow conclusions get exposed; deep structural consequences surface. | S5, S7 |

### Additional contributors (not counted in the 19, but deeply embedded)

These thinkers contribute key mechanisms that are wired into specific stages:

| Thinker | Mechanism | How the AI exploits it | Stage |
|---------|-----------|----------------------|-------|
| **Poincaré** | Incubation and remote association | The spreading-activation algorithm (S3) is an explicit model of Poincaré's observation that weak associations surface during incubation. The AI propagates activation across concepts and lets the "incubated" associations emerge as convergent nodes. | S3 |
| **Ramanujan** | Intuitive pattern detection, then rigorous verification | The AI detects structural patterns and generates conjectures, then verifies each on independent test instances not drawn from the generative set. | S3, S6.1, S7 |
| **Archimedes** | Name the binding constraint first | Before searching for solutions, the AI identifies the single constraint that makes the problem hard. This is Archimedes' "eureka" move — the constraint IS the insight. | S2 |
| **Ohlsson** | Three paths to insight: constraint relaxation, chunk decomposition, re-encoding | When the pipeline gets stuck, the AI applies all three Ohlsson mechanisms in parallel. Constraint relaxation: what opens if you remove a constraint? Chunk decomposition: break apart something that seemed indivisible. Re-encoding: describe the same situation in a completely different way. | S3.1 |
| **Boden** | Creativity type classification | The AI classifies the conclusion as combinatorial (new combination of existing elements), exploratory (new path in an existing space), or transformational (new space entirely). This tells you what *kind* of insight you got. | S7 |
| **Millikan** | Differential-effort detection | Beyond Feynman's observation, the AI applies a formal test: does one hypothesis require significantly more defensive reasoning than others? More defense = weaker claim. | S6 |

---

## The pipeline — stage by stage

### How stages run together

The pipeline uses **waves** — groups of stages that run in parallel when they're
independent, and sequentially when one depends on another's output. There's a
deliberate **Mode Shift** between ideation (generating material) and synthesis
(evaluating material). Evaluating while generating is the #1 cause of
plausible-sounding but shallow AI reasoning. The Mode Shift makes skipping it
impossible.

```
DEEP scale (all stages):

  Wave 1    ┌─ S1: State Loading
            │
  Wave 2    ├─ S2: Constraint Escape ──┐
            │                          ├─ (parallel)
            └─ S3: Peripheral Explore ─┘
              [S3.1 if S3 comes back thin]
            │
  Wave 3    └─ S4: Dynamic Simulation
            │
            ─── Mode Shift: stop generating, start evaluating ───
            │
  Wave 4    ├─ S5: Precision Forcing ──┐
            │                          ├─ (parallel)
            └─ S6: Falsification     ─┘
            │
  Wave 5    └─ S7: Integration & Verification
              [S3.1 + S6 + S7 retry if S6 comes back empty]
            │
  Wave 6    └─ OSP: Output Synthesis Pass

STANDARD: same but no S4
MINIMAL:  S1 → S5 → S7 → OSP (fast path)
CONJECTURE: S1 → S6.1 → S7 → OSP (pattern-detection path)
```

### S1 — State Loading

**What it does:** Breaks your input into the smallest pieces that can't be
simplified further. Identifies what type of problem this is. Produces a
"measurement plan" — a concrete thing you could observe to confirm or refute
the core claim.

**Why it matters:** If you don't find the irreducibles first, you end up
solving a paraphrase of the problem, not the problem itself. Most bad
reasoning starts here.

**Traits at work:** Einstein (strip to primitives), Feynman (jargon removal),
Turing (constructive specification — name the observable), Tesla (measurement
plan), von Neumann (enumerate 20+ in parallel, no early compression), Darwin
(gather evidence before hypothesizing).

**AI advantage:** A human struggles to enumerate 20+ primitives without
compressing or filtering. The AI enumerates all of them in parallel, then
strips — no premature selection.

### S2 — Constraint Escape

**What it does:** Names the single binding constraint — the thing that makes
this problem hard. Then searches 20+ other domains for situations where that
same structural constraint exists and was solved. Produces five
structurally-distinct domain mappings with escape paths.

**Why it matters:** The constraint is the key. Once you name it, you can find
it in unexpected places. "Heat dissipation limits" and "population growth
limits" share the same math — one domain's solution transfers to the other.

**Traits at work:** Archimedes (name the binding constraint first), Da Vinci
(describe the constraint in domain-agnostic terms), Newton (transfer the
structure, not the surface).

**AI advantage:** A human knows 3–5 domains well enough to map between them.
The AI can survey 20+ domains in parallel, looking for structural matches
without being anchored to the most familiar one.

### S3 — Peripheral Exploration

**What it does:** Runs a spreading-activation algorithm inspired by how human
memory works. Starts from the S1 primitives, propagates activation through
weak associations across 3–5 steps, and identifies **convergent nodes** —
concepts that were reached from two or more independent starting paths. These
are the most likely sources of non-obvious insight.

**Why it matters:** The best insights come from weak associations, not strong
ones. Strong associations lead to obvious answers. Weak associations that
converge from independent directions are where surprises live.

**Traits at work:** Poincaré (incubation — let weak associations surface),
Darwin (independent evidence paths), Da Vinci (cross boundaries between
fields), Ramanujan (verify patterns on independent instances).

**AI advantage:** A human can hold maybe 7±2 concepts in working memory while
tracing associations. The AI propagates activation across dozens of nodes
simultaneously, logging every step with provenance. No associations are lost
to working memory limits.

### S3.1 — Representational Change / De-fixation *(conditional)*

**What it does:** Only runs when S3 comes back with too few convergent nodes
(i.e., the pipeline is stuck). Applies Ohlsson's three mechanisms of insight
in parallel: constraint relaxation (what opens if you remove a constraint?),
chunk decomposition (break apart something that seemed indivisible), and
re-encoding (describe the same situation in a completely different way). Each
mechanism runs under an explicit "abandon your prior approach" instruction.

**Why it matters:** Being stuck usually means you're locked into a
representation. The fix isn't to try harder in the same frame — it's to
change the frame entirely. This is the Einstellung effect: the solution you
already found blocks you from finding a better one.

**Traits at work:** Einstein (Einstellung override — explicitly abandon the
prior approach), Ohlsson (the three formal mechanisms of insight).

**AI advantage:** A human resisting the Einstellung effect is fighting their
own cognitive inertia. The AI is instructed to treat its prior output as
wrong by default. It can apply all three mechanisms simultaneously without
the psychological cost of "starting over."

### S4 — Dynamic Simulation *(DEEP scale only)*

**What it does:** Builds a complete model of the system from S1 primitives.
Embeds it simultaneously in three observer frames (insider, outsider,
measurement instrument). Runs Tesla's simulation checklist to find energy
imbalances, singularities, limit cases, and theory collisions (situations
where two parts of the reasoning contradict each other).

**Why it matters:** Theories that look consistent from one angle often
contradict each other when you check from another. Running three frames in
parallel catches contradictions that any single frame misses.

**Traits at work:** Tesla (pre-visualize the whole system, then stress-test
it), Einstein (hold multiple observer frames simultaneously).

**AI advantage:** A human switching between frames loses context with each
switch. The AI holds all three frames in parallel and cross-references them
without the context decay of sequential attention-shifting.

### S5 — Precision Forcing

**What it does:** Takes every claim and forces it through five levels of
vocabulary stripping (from "explain it to a five-year-old" to formal
notation). Renders each claim in three representations (diagram, equation,
procedure). Generates ten forward-chain consequence trees. Flags anything
that can't survive all five vocabulary levels as "vague."

**Why it matters:** Imprecision hides in jargon. A claim that sounds
impressive in technical language often collapses when you try to explain it
simply. The five-year-old test isn't about dumbing things down — it's about
finding out what's real and what's just words.

**Traits at work:** Feynman (five-level jargon stripping), Da Vinci
(three-representation rendering), Turing (constructive specification — the
minimal demonstrating object), von Neumann (ten parallel forward-chain trees).

**AI advantage:** A human can do the five-year-old test on one or two claims.
The AI does it on every claim simultaneously, plus generates 30+ consequence
paths in parallel. No claim escapes the precision filter.

### S6 — Falsification Engine

**What it does:** Spawns three parallel sub-subagents — one using abductive
reasoning (inference to the best explanation), one using deductive reasoning
(following logical consequences), and one using inductive reasoning
(generalizing from evidence). A consolidator merges their output into 3+
ranked alternative hypotheses. Then runs the disconfirming-evidence search:
counter-examples, failure classes, mechanism gaps. Applies the Millikan test
(is the AI working harder to protect one hypothesis?). Produces a density
report of disconfirming items.

**Why it matters:** This is the engine that prevents the AI from giving you a
confident, plausible-sounding wrong answer. It doesn't just check if the
answer could be right — it actively searches for reasons the answer might be
wrong. And it checks whether the AI is unconsciously defending a weak
hypothesis.

**Traits at work:** Einstein (extreme scenarios), Feynman (Millikan test —
notice differential effort), Darwin (golden-rule disconfirming search), Turing
(formal consequence testing).

**AI advantage:** Humans naturally seek confirming evidence. The AI is
explicitly instructed to seek *disconfirming* evidence and to treat
alternative hypotheses with symmetric scrutiny. It can run three inference
modes simultaneously, something a human can only do sequentially (with
context loss between switches).

### S6.1 — Conjecture *(conjecture mode only)*

**What it does:** Replaces S2–S5 when you use `--conjecture`. Detects
patterns across the input data (cross-domain structural matches, recurrence
relations, symmetry groups). Generates 3+ conjectures with novelty and
elegance scores. Verifies each conjecture on 2+ independent test instances
not drawn from the generative set.

**Why it matters:** Some problems are pattern-detection problems, not
reasoning problems. Ramanujan's genius was seeing patterns where others saw
noise — then proving the pattern held on cases he hadn't looked at yet.

**Traits at work:** Ramanujan (intuitive pattern detection, then rigorous
verification on independent instances).

### S7 — Integration & Verification

**What it does:** Collects convergence signals from every stage that ran.
Identifies claims that were confirmed by 3+ independent stages. Forms the
primary conclusion with a confidence band (HIGH / MODERATE / LOW). Classifies
the conclusion by Boden creativity type (combinatorial / exploratory /
transformational). Scores for elegance. Runs the V1–V7 verification battery
as seven independent gates.

**Why it matters:** This is where the whole pipeline comes together. The
convergence check ensures that only claims confirmed by multiple independent
reasoning paths make it into the conclusion. The verification battery catches
logic gaps, cargo-cult reasoning, and premature generalization.

**Traits at work:** Feynman (error-seeking, symmetric scrutiny), Newton
(does it generalize?), Ramanujan (structural-coherence elegance), von Neumann
(systematic integration across all stages).

### OSP — Output Synthesis Pass

**What it does:** Distills S7's raw integration into the final 11–13 section
report. Selects only the load-bearing claims. Tags every claim with its source
stage. Verifies that the V6 scope statement is copied byte-for-byte (no
paraphrasing allowed). Self-verifies that every tagged fragment appears in
the output or is explicitly marked omitted.

**Why it matters:** The raw integration is dense and technical. The OSP
produces a document you can actually read and use — without losing any
content or introducing any paraphrase drift.

---

## Verification battery (V1–V7)

S7 runs seven independent checks. Each records PASS, PARTIAL, or FAIL:

| Gate | Name | What it catches |
|------|------|-----------------|
| V1 | **Logic** | Missing steps in reasoning — every inferential link must be explicit and valid |
| V2 | **Cargo-cult detection** | Steps that *look* rigorous without *being* rigorous — the AI went through the motions without the substance |
| V3 | **Symmetric scrutiny** | Alternative hypotheses that got less analysis than the leading one — you can't favor your preferred answer |
| V4 | **Completeness** | A stage produced thin or empty output — something was skipped that shouldn't have been |
| V5 | **Constructive** | No concrete observable was named — the claim can't be tested, so it's not a claim |
| V6 | **Scope** | The conclusion doesn't state what it applies to and what it doesn't — scope is carved out verbatim, cannot be paraphrased |
| V7 | **Representational coherence** | The conclusion changes depending on how you describe it — if it only works in one representation, it might be a framing artifact |

V6 is special: its scope statement is copied **byte-for-byte** into the final
report. It cannot be summarized, reworded, or improved. If it contradicts a
casual reading of the conclusion, the scope statement wins. This is the
skill's "confession" — it always tells you where it might be wrong.

---

## Test battery (T1–T5)

These are automated post-hoc quality checks on the session artifacts. They
run after every session and are informational (they don't block output):

| Check | What it verifies |
|-------|-----------------|
| T1 | Every declared stage produced a non-empty output file |
| T2 | Every skipped stage has an empty-section marker in the report |
| T3 | V6 scope appears byte-for-byte in the final report (no drift) |
| T4 | Every required section meets its minimum depth |
| T5 | In XML mode, every top-level schema element is present |

---

## Scales and flags

### Scales (auto-detected from input, or forced with flags)

| Scale | When it activates | Stages that run |
|-------|-------------------|-----------------|
| **MINIMAL** | Under 300 chars, no code, no technical terms | S1 → S5 → S7 → OSP |
| **STANDARD** | 300–5000 chars, or contains code/technical content | S1 → S2+S3 → S5+S6 → S7 → OSP (+ conditionals) |
| **DEEP** | Over 5000 chars, multi-section spec, or `--deep` | S1 → S2+S3 → S4 → S5+S6 → S7 → OSP (+ conditionals) |
| **CONJECTURE** | `--conjecture` flag | S1 → S6.1 → S7 → OSP |

### Flags

| Flag | Effect |
|------|--------|
| `--minimal` | Force MINIMAL scale |
| `--standard` | Force STANDARD scale |
| `--deep` | Force DEEP scale |
| `--conjecture` | Pattern/conjecture pipeline (Ramanujan mode) |
| `--xml` | Emit XML output (each stage's markdown in CDATA) |
| `--quiet` | Suppress terminal output; save silently |
| `--verbose` | Expand section depth (adds Genius Mind Attributions section) |
| `--no-save` | Skip final report; stage artifacts still written |
| `--resume` | Resume a prior session at the next pending stage |

---

## Output shape

The final report has 11 always-present sections and up to 2 conditional ones:

1. **Pipeline Status Header** — version, scale, stages run, active conditionals
2. **Headline Insight** — the primary conclusion + creativity type + confidence
3. **Theory Collisions** — where two parts of the reasoning contradict
4. **Discovery vs. Proof** — discovered claims vs. their proof depth
5. **Independence-Verified Bridges** — cross-domain mappings verified by independent chains
6. **Alternative Hypotheses** — ranked competing explanations with confidence and falsification conditions
7. **Density-Checked Falsification** — counter-examples, failure classes, density score, strongest objection, weakest link
8. **Scope Limits** — what the conclusion applies to, doesn't extend to, and claims refused (V6 verbatim)
9. **Coherence Signals** — claims confirmed by 3+ independent stages (STRONG) or 2 stages (MODERATE)
10. **Generalization Checks** — where the conclusion holds and where it breaks
11. **Representational Signals** *(conditional — only if S3.1 ran or V7 detected a frame shift)*
12. **Open Questions & Next Probes** — ranked by discriminating power
13. **Genius Mind Attributions** *(verbose only — which stages contributed which claims, with trait citations)*

---

## Architecture

The skill enforces a **three-layer rule** that keeps context usage bounded and
reasoning auditable:

```
┌──────────────────────────────────────────────────┐
│  Orchestrator (SKILL.md)                        │
│  - Parses flags, validates input, inits session  │
│  - Detects scale, builds wave plan              │
│  - Spawns stages wave-by-wave via Agent tool     │
│  - Handles conditionals, retries, and OSP        │
│  - NEVER reads KB files or runs stage protocols  │
└──────────────────────┬───────────────────────────┘
                       │ spawns (Agent tool)
                       ▼
┌──────────────────────────────────────────────────┐
│  Stage subagents (modules/*.md)                  │
│  - Each loads ONE module file + its declared     │
│    kb_sources + input_dependencies               │
│  - Runs its PROTOCOL                             │
│  - Writes one output file to stages/             │
│  - Does NOT see the full pipeline state          │
└──────────────────────┬───────────────────────────┘
                       │ reads
                       ▼
┌──────────────────────────────────────────────────┐
│  Knowledge base (kb/ — 18 operational files)     │
│  - Spreading-activation, de Bono lateral,        │
│    Ohlsson de-fixation, Collins & Loftus,        │
│    verification gates, Boden types, elegance     │
│    rubric, Tesla simulation, observer frames,    │
│    domain catalog, vocabulary rubric, etc.       │
└──────────────────────────────────────────────────┘
```

**Why it matters:** The orchestrator never grows in token count as the
pipeline runs deeper. Each subagent is born with minimal context (one module +
its declared inputs) and dies with its output written to disk. The skill can
run a DEEP session with retries without ever overflowing the orchestrator's
context window.

### Conditional activation

Two signals can reshape the pipeline at runtime:

- **S3 thin or empty** — S3's spreading-activation found fewer than two
  convergent nodes. This fires S3.1 (de-fixation) inline, using Ohlsson's
  three mechanisms to break the block.
- **S6 no alternatives** — S6's falsification engine produced zero alternative
  hypotheses. This fires a retry chain: S3.1 + S6 + S7 re-run. If S3.1
  already ran, the retry is suppressed and an advisory is logged instead.

### Session output

Every run creates a session folder you can inspect later:

```
~/docs/epiphany/genius/YYYYMMDD-topic-slug/
├── report.md              Final report
├── report.xml            (if --xml)
└── stages/
    ├── session.md         Manifest — id, flags, observability counters
    ├── 00-processed-input.md   Routed input
    ├── S1-state-loading.md     ...through S7-integration.md
    ├── S7-v6-scope.txt         V6 scope statement (verbatim)
    ├── output-distilled.md     Full distilled document
    ├── output.xml              (if --xml)
    ├── test-report.md          T1–T5 results
    └── validation-log.md      Per-stage PASS/FAIL audit trail
```

---

## File layout

```
~/.claude/skills/epiphany-genius/
├── SKILL.md                 Orchestrator (the main agent reads this)
├── README.md                This file
├── index.json               Module registry — waves, activation, dependencies
├── LICENSE                  MIT
├── modules/                 10 stage protocol files
│   ├── S1-state-loading.md
│   ├── S2-constraint-escape.md
│   ├── S3-peripheral-exploration.md
│   ├── S3-1-defixation.md
│   ├── S4-dynamic-simulation.md
│   ├── S5-precision-forcing.md
│   ├── S6-falsification.md
│   ├── S6-1-conjecture.md
│   ├── S7-integration-verification.md
│   └── output-synthesis-pass.md
├── kb/                      18 operational knowledge-base files
│   ├── spreading-activation.md
│   ├── ohlsson-defixation.md
│   ├── debono-techniques.md
│   ├── domain-catalog.md
│   ├── blend-template.md
│   ├── verification-gates.md
│   ├── boden-types.md
│   ├── elegance-rubric.md
│   ├── tot-templates.md
│   ├── falsification-checklists.md
│   ├── pattern-taxonomy.md
│   ├── observer-frames.md
│   ├── simulation-checklist.md
│   ├── representation-frames.md
│   ├── vocabulary-rubric.md
│   ├── forward-chain-template.md
│   ├── input-preloading-templates.md
│   └── scope-template.md
└── scripts/
    ├── session-init.sh      Collision-safe session folder creation
    ├── validate-stage.sh    Per-stage output verification
    ├── xml-assemble.sh      Index-driven XML envelope assembly
    ├── test-runner.sh       T1–T5 post-run test battery
    └── validate-pipeline.sh Dev/CI — 12 DAG and schema integrity checks
```

---

## Development and CI

```bash
bash ~/.claude/skills/epiphany-genius/scripts/validate-pipeline.sh
```

All 12 checks must pass before merging changes to `index.json`, stage
modules, or KB files. The validator catches common breakages (missing XML
elements, changed signal names, referenced KB files that don't exist, DAG
cycles).

### Adding a new stage

1. Write `modules/SN-new-stage.md` with the required frontmatter.
2. Register it in `index.json`.
3. Update `SKILL.md` wave-plan tables.
4. Run `validate-pipeline.sh`.

No edits needed to scripts — they read `index.json` at runtime.

---

## Why the stages are in this order

The sequence follows a **generate-then-evaluate** discipline:

1. **Ideation phase** (S1–S4) generates material *without judging it*.
   Stripping, domain-mapping, spreading-activation, and simulation all
   produce candidate content. Evaluation here contaminates generation.
2. **Mode Shift** marks the commitment to stop generating.
3. **Synthesis phase** (S5–S7) commits to precision, searches for falsifying
   evidence, and integrates — operations that require the full generated pool
   and that penalize premature closure.
4. **OSP** does last-mile distillation — it doesn't add new reasoning, it
   selects load-bearing fragments and lays them out.

Skipping the boundary — evaluating while generating — is the failure mode
that produces fluent but shallow AI output. The pipeline's structure makes
skipping it impossible.

---

## Trait × Stage integration map

This table shows which traits are active in which stages, making the
connections explicit:

| Trait | S1 | S2 | S3 | S3.1 | S4 | S5 | S6 | S6.1 | S7 |
|-------|----|----|-----|------|----|----|-----|------|-----|
| Einstein: irreducible primitives | ● | | | | | | | | |
| Einstein: multiple observer frames | | | | | ● | | | | |
| Einstein: extreme scenarios | | | | | | | ● | | |
| Einstein: Einstellung override | | | | ● | | | | | |
| Feynman: jargon removal | ● | | | | | ● | | | |
| Feynman: error-seeking / symmetric scrutiny | | | | | | | ● | ● | ● |
| Feynman: Millikan test | | | | | | | ● | | |
| Newton: structural analogy transfer | | ● | | | | | | | |
| Newton: generalization-first | | | | | | | | | ● |
| Turing: constructive specification | ● | | | | | ● | | | |
| Turing: formal consequence testing | | | | | | ● | ● | | |
| Da Vinci: domain-agnostic description | | ● | | | | | | | |
| Da Vinci: multi-representation | | | | | | ● | | | |
| Da Vinci: cross-domain observation | | | ● | | | | | | |
| Tesla: pre-visualization + measurement | ● | | | | ● | | | | |
| Darwin: evidence before hypotheses | ● | | ● | | | | | | |
| Darwin: disconfirming-evidence search | | | | | | | ● | | |
| von Neumann: parallel enumeration | ● | | | | | ● | | | ● |
| von Neumann: ten-steps-ahead | | | | | | ● | | | ● |
| Poincaré: incubation / remote association | | | ● | | | | | | |
| Ramanujan: pattern detection + verification | | | ● | | | | | ● | ● |
| Archimedes: name the binding constraint | | ● | | | | | | | |
| Ohlsson: 3 mechanisms of insight | | | | ● | | | | | |
| Boden: creativity type classification | | | | | | | | | ● |
| Millikan: differential-effort detection | | | | | | | ● | | |

● = trait is actively invoked in this stage's protocol steps.

---

## License

MIT — see [LICENSE](LICENSE).

## Contributing

Issues and pull requests welcome. Before opening a PR:

1. Run `scripts/validate-pipeline.sh` — all 12 checks must pass.
2. If you add a stage, module, or KB file, update `index.json` and the
   relevant `GENIUS PERSPECTIVES` frontmatter so trait attribution stays
   auditable.
3. Keep KB files operational, not encyclopedic — each KB file should be
   load-bearing for a specific stage protocol step.