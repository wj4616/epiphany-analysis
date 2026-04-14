# epiphany-genius

> A modular, subagent-orchestrated reasoning pipeline that applies the cognitive
> traits of history's deepest thinkers — Einstein, Feynman, Newton, Turing,
> Da Vinci, Tesla, Darwin, von Neumann, Poincaré, Ramanujan, Archimedes — to
> any problem you give it. One prompt in, a verified, scope-limited,
> falsification-tested conclusion out.

**Version:** 1.1.0 &nbsp;·&nbsp; **Status:** Stable &nbsp;·&nbsp;
**Runtime:** Claude Code skill (self-contained) &nbsp;·&nbsp; **Output:** Markdown or XML

---

## Table of contents

- [What it does](#what-it-does)
- [Installation](#installation)
- [Quick start](#quick-start)
- [The core idea](#the-core-idea)
- [The 19 cognitive traits](#the-19-cognitive-traits)
- [Pipeline design](#pipeline-design)
- [Stages and their genius attributions](#stages-and-their-genius-attributions)
- [Verification battery (V1–V7)](#verification-battery-v1v7)
- [Test battery (T1–T5)](#test-battery-t1t5)
- [Scales and flags](#scales-and-flags)
- [Output shape](#output-shape)
- [Architecture](#architecture)
- [File layout](#file-layout)
- [Development and CI](#development-and-ci)

---

## What it does

You throw a problem at `/epiphany-genius`. It returns a structured, distilled
reasoning document that has:

- been **decomposed to irreducible primitives** (Einstein, Feynman);
- been **explored across ≥20 adjacent domains** for structural analogies
  (Newton, Da Vinci, Archimedes);
- had **weak associations traced explicitly** instead of relying on intuition
  (Poincaré, Darwin);
- been **simulated under multiple observer frames** and stress-tested at
  singularities and limit cases (Tesla, Einstein);
- been **forced into five vocabulary levels and three representations**, with
  ten-step consequence chains rendered in parallel (Feynman, Da Vinci,
  von Neumann, Turing);
- had **three inference modes** (abductive, deductive, inductive) run in
  parallel against three alternative hypotheses — a disconfirming-evidence
  search, not a confirming one (Darwin, Feynman, Turing);
- been **put through a seven-gate verification battery** for logic,
  cargo-cult detection, symmetric scrutiny, completeness, constructability,
  scope, and representational coherence (Feynman, Newton, Ramanujan,
  von Neumann);
- been **classified by creativity type** (Boden: combinatorial / exploratory
  / transformational) and **scored for elegance** (simplicity + symmetry +
  depth).

The result is not a best-effort answer. It is a conclusion you can
trust to come with its own confession — where it might be wrong, what it does
not cover, and what would defeat it.

---

## Installation

`epiphany-genius` is a [Claude Code](https://github.com/anthropics/claude-code)
skill. To install, clone the repo into your Claude Code skills directory:

```bash
git clone https://github.com/wj4616/epiphany-genius.git ~/.claude/skills/epiphany-genius
```

The `/epiphany-genius` slash command will be available in Claude Code on the
next session start. No other dependencies — the skill is self-contained
(Bash + `jq` + `python3` for the validator).

---

## Quick start

```bash
# Basic use — scale is auto-detected from input length and complexity
/epiphany-genius What is the irreducible core of a pre-mortem procedure?

# Force a scale
/epiphany-genius --deep    [long technical spec]
/epiphany-genius --minimal [short question]

# Pattern-detection / conjecture mode (Ramanujan pipeline)
/epiphany-genius --conjecture [numerical or pattern-heavy data]

# Machine-readable XML output (each stage's full markdown in CDATA)
/epiphany-genius --xml [problem]

# Silent save (writes to disk, no terminal echo)
/epiphany-genius --quiet [problem]

# Resume an interrupted session
/epiphany-genius --resume 20260413-pre-mortem-procedures
```

Every run produces a self-contained session folder under
`{session_output_base}/{YYYYMMDD-topic-slug}/` — configured in `index.json`,
default `~/docs/epiphany/genius/` — containing every stage's raw output,
the final report, a V1–V7 verification report, and a T1–T5 test report.

---

## The core idea

Most reasoning aids help you think *faster*. `epiphany-genius` helps you
think *with more cognitive range*.

The skill is built on a simple claim: the reason Einstein, Feynman, and
Poincaré reached conclusions no one else could reach is not that they were
magical. It is that they habitually applied **specific, operationalizable
cognitive moves** that most people never apply together:

- Strip the problem to irreducibles *before* looking for a solution.
- Name the binding constraint out loud, then search for structural analogs
  in distant domains.
- Hold multiple observer frames in parallel instead of switching between them.
- Generate the full space of candidate explanations before picking one.
- Actively seek what would defeat the claim rather than what would confirm it.
- Test the claim on instances that did not generate it.
- Verify that the claim generalizes, or admit that it does not.

LLMs have a latent capacity to do all of these, but when asked casually they
collapse back to mean-reversion: the fluent, confident, locally plausible
answer. `epiphany-genius` constructs a scaffold that forces the model to
actually run the genius moves, to persist the intermediate artifacts, and to
subject the final synthesis to a verification battery before you see it.

The result is not an LLM trying to *sound* insightful. It is an LLM that
has been structurally prevented from skipping the steps that produce insight.

---

## The 19 cognitive traits

The skill maps 19 Tier-1 cognitive traits — one or more per named thinker — into
explicit stage protocols. Each trait is a reproducible *move*, not a
personality:

| Thinker      | Trait / mechanism                                                    | Operationalized in |
|--------------|----------------------------------------------------------------------|--------------------|
| Einstein     | Reduce to irreducible primitives before solving                      | S1                 |
| Einstein     | Embed the observer in multiple frames simultaneously                 | S4                 |
| Einstein     | Construct extreme scenarios to test under boundary regimes           | S6                 |
| Einstein     | Einstellung override — explicit instruction to abandon prior approach | S3.1              |
| Feynman      | Jargon removal as diagnostic — the five-year-old test                | S5                 |
| Feynman      | Error-seeking / symmetric scrutiny — look for what is wrong          | S6, S7             |
| Feynman      | Millikan test — notice when reasoning works harder to protect a claim | S6                |
| Newton       | Transfer structural relationships (not surface features) across domains | S2              |
| Newton       | Generalization-first — does the conclusion generalize or is it a one-off? | S7            |
| Turing       | Constructive specification — what observable would demonstrate success? | S1, S5          |
| Turing       | Formal consequence testing — if the claim implies X, does X hold?    | S5, S6             |
| Da Vinci     | Describe phenomena in domain-agnostic terms                          | S2                 |
| Da Vinci     | Multi-representation — diagram + equation + procedural description   | S5                 |
| Da Vinci     | Cross-domain observation across boundaries                           | S3                 |
| Tesla        | Pre-visualize the complete system; identify measurable success       | S1, S4             |
| Darwin       | Evidence accumulation before hypothesis generation                   | S1, S3             |
| Darwin       | Golden-rule disconfirming-evidence search                            | S6                 |
| von Neumann  | Unbounded working memory — parallel enumeration, no compression      | S1, S5, S7         |
| von Neumann  | Ten-steps-ahead derivation; systematic integration across states     | S5, S7             |
| Poincaré     | Incubation and remote association traversal                          | S3                 |
| Ramanujan    | Intuitive pattern detection followed by rigorous verification        | S3, S7, S6.1       |
| Archimedes   | Name the binding constraint before searching for solutions           | S2                 |
| Ohlsson      | Constraint relaxation, chunk decomposition, re-encoding (3 paths to insight) | S3.1       |
| Boden        | Creativity-type classification (combinatorial / exploratory / transformational) | S7      |
| Millikan     | Differential-effort detection across hypothesis defenses             | S6                 |

Each module file's `GENIUS PERSPECTIVES` block names which traits it invokes
and which protocol step each trait maps to, so the attribution is
auditable — not decorative.

---

## Pipeline design

The pipeline is **wave-based**: stages run in ordered parallel groups so that
independent reasoning happens concurrently, dependent reasoning runs
downstream, and at no point does a subagent need to hold the full pipeline
state in its context window.

A **Mode Shift** line is printed between the last ideation wave and the first
synthesis wave — a deliberate cognitive boundary borrowed from the observation
that generative and evaluative reasoning interfere with each other when
interleaved.

```
DEEP      │ Wave 1: S1              │        (ideation)
          │ Wave 2: S2 ∥ S3         │
          │   [S3.1 if S3 thin]     │
          │ Wave 3: S4              │
          │ ─── Mode Shift ───      │        (synthesis)
          │ Wave 4: S5 ∥ S6         │
          │ Wave 5: S7              │
          │   [S3.1+S6+S7 retry     │
          │    if S6 empty]         │
          │ Wave 6: OSP             │

STANDARD  │ Wave 1: S1
          │ Wave 2: S2 ∥ S3   [S3.1 if S3 thin]
          │ ─── Mode Shift ───
          │ Wave 3: S5 ∥ S6
          │ Wave 4: S7        [S3.1+S6+S7 retry if S6 empty]
          │ Wave 5: OSP

MINIMAL   │ S1 → (Mode Shift) → S5 → S7 → OSP
CONJECTURE│ S1 → S6.1 → S7 → OSP
```

The `∥` denotes parallel execution. `[...]` denotes conditional modules that
activate only under specific signals.

### Conditional activation

Two signals can reshape the pipeline at runtime:

- **`S3_thin_or_empty`** — S3's spreading-activation produced fewer than
  two convergent nodes. Fires `S3.1 Representational Change (de-fixation)`
  inline, using Ohlsson's three mechanisms to break the block.
- **`S6_no_alternatives`** — S6's falsification engine produced zero
  alternative hypotheses. Fires an S3.1 + S6 + S7 retry chain. If S3.1
  already ran upstream, retry is suppressed and an advisory is logged
  instead (the problem space may be genuinely underdetermined).

### Mode Shift

The line `— Mode Shift: switching from generative to evaluative reasoning —`
is printed at the ideation-to-synthesis boundary. This is not cosmetic. It
resets the subagent's framing: generation up to the line, falsification and
integration after. Cross-frame leakage is the single most common failure mode
in informal LLM reasoning — making the boundary explicit prevents it.

---

## Stages and their genius attributions

Each stage is a short, self-contained module file with a strict protocol, a
`MACHINE ADVANTAGE` block (what capability this stage exploits that humans
lack), and a `GENIUS PERSPECTIVES` block (which traits are invoked and where).

### S1 — State Loading

**Traits:** Einstein (irreducible primitives), Feynman (jargon removal),
Turing (constructive specification), Tesla (pre-visualization, measurement
plan), von Neumann (parallel enumeration), Darwin (evidence accumulation).

Decomposes the input into five classes (algorithmic / design / proof /
diagnostic / other), enumerates ≥20 primitives per category, strips them to
irreducibles, and produces a Tesla measurement plan — a concrete observable
that would confirm or refute the claim.

### S2 — Constraint Escape

**Traits:** Archimedes (name the binding constraint), Da Vinci (domain-agnostic
description), Newton (structural analogy transfer).

Names the single binding constraint, surveys ≥20 candidate domains in
parallel, produces five structurally distinct domain mappings, and identifies
an escape path via analogy. A de Bono lateral fallback (provocation /
random-entry / reversal) is available if analogy yields too little.

### S3 — Peripheral Exploration

**Traits:** Poincaré (incubation / remote association), Darwin (independence
of evidence paths), Da Vinci (cross-domain observation), Ramanujan (pattern
verification on independent instances).

Makes the spreading-activation map an explicit artifact. Propagates
activation across 3–5 steps using a Collins & Loftus update equation, logs
every activated node with provenance, identifies convergent nodes that reach
threshold via ≥2 independent paths.

### S3.1 — Representational Change (de-fixation) *(conditional)*

**Traits:** Einstein (Einstellung override), Ohlsson (the three mechanisms
of insight: constraint relaxation, chunk decomposition, re-encoding).

Runs when S3 comes back thin *or* when S6 finds no alternatives. Applies all
three Ohlsson mechanisms in parallel, each under an explicit "abandon prior
solution" instruction, and selects a single breakthrough candidate with an
Einstellung-residue check.

### S4 — Dynamic Simulation *(DEEP only)*

**Traits:** Tesla (full-system pre-simulation), Einstein (multiple observer
frames in parallel).

Embeds the system simultaneously in three observer frames (insider,
outsider, measurement-instrument), runs Tesla's simulation checklist to flag
imbalances, singularities, limit cases, and theory collisions.

### S5 — Precision Forcing

**Traits:** Feynman (jargon removal, five-year-old test), Da Vinci
(multi-representation), Turing (constructive specification), von Neumann
(ten-steps-ahead forward chains).

Renders the claim at five vocabulary levels in parallel (five-year-old →
formal symbols), in three representations (diagram / equation / procedural),
and produces ten forward-chain consequence trees. What survives all five
levels is the real content; what collapses is a vague claim.

### S6 — Falsification Engine

**Traits:** Einstein (extreme scenarios), Feynman (Millikan differential-effort
detection), Darwin (disconfirming-evidence search), Turing (formal
consequence testing).

Spawns three parallel sub-subagents — abductive, deductive, inductive —
each generating candidate hypotheses independently, then a consolidator that
produces ≥3 genuine alternatives. Applies the Millikan test (did we work
harder to protect one hypothesis?) and a density check to prevent thin
falsification.

### S6.1 — Conjecture *(`--conjecture` only)*

**Traits:** Ramanujan (pattern detection followed by verification on
independent instances).

Replaces S2–S5 under `--conjecture`. Detects structural patterns across
cross-domain matches, recurrence relations, and symmetry groups; produces
≥3 conjectures with novelty, elegance, and falsification conditions; verifies
each on ≥2 test instances not drawn from the generative set.

### S7 — Integration & Verification

**Traits:** Feynman (error-seeking, symmetric scrutiny), Newton (generalization
check), Ramanujan (structural-coherence elegance), von Neumann (systematic
integration).

Collects convergence signals across every stage that ran, forms the primary
conclusion with a confidence band, classifies the conclusion by Boden
creativity type, scores elegance, and runs the V1–V7 verification battery as
seven independent explicit gates.

### OSP — Output Synthesis Pass

Distills S7's raw integration into the final 11–13-section report. Tags every
claim with its source stage, verifies that every tagged fragment either
appears in output or is explicitly marked omitted with reason, and writes
V6's scope statement byte-for-byte (no paraphrase).

---

## Verification battery (V1–V7)

S7 applies all seven gates as independent checks. Each gate records PASS,
PARTIAL, or FAIL with a justification.

| Gate | Name                       | Purpose                                                        |
|------|----------------------------|----------------------------------------------------------------|
| V1   | Logic                      | Every inferential link is explicit and valid                   |
| V2   | Cargo-cult detection       | No step looks rigorous without being rigorous                  |
| V3   | Symmetric scrutiny         | Alternative hypotheses received the same scrutiny as the primary |
| V4   | Completeness               | Every declared stage produced non-thin output                  |
| V5   | Constructive               | A concrete observable demonstrates the claim                   |
| V6   | Scope                      | What the claim does and does not extend to (verbatim carved out) |
| V7   | Representational coherence | If the representation shifted, the conclusion is consistent under both |

V6 is the only gate whose output is carved out verbatim into the final
report — it cannot be paraphrased, summarized, or reworded. If the scope
statement contradicts a casual reading of the conclusion, the scope statement
wins.

---

## Test battery (T1–T5)

Distinct from the verification battery, T1–T5 are automated post-hoc
quality checks on the session artifacts themselves. They run after every
session and are informational only (they do not block output).

| Check | Purpose                                                                       |
|-------|-------------------------------------------------------------------------------|
| T1    | Every declared stage produced a non-empty output file                         |
| T2    | Every skipped stage has a canonical empty-section marker in the distilled output |
| T3    | V6 scope appears byte-for-byte in the final report (no drift)                 |
| T4    | Every required section meets its minimum-depth regex                          |
| T5    | In XML mode, every top-level schema element is present                        |

A test failure does not fail the run. It surfaces as an advisory so you can
decide whether to re-run or inspect.

---

## Scales and flags

### Scales

The skill auto-detects a depth scale from input length and content, or you can
force one.

| Scale       | Trigger (auto)                                           | Active stages                              |
|-------------|----------------------------------------------------------|--------------------------------------------|
| MINIMAL     | < 300 chars, no code, no technical terms                 | S1, S5, S7, OSP                            |
| STANDARD    | 300–5000 chars, or contains code / technical content      | S1, S2, S3, S5, S6, S7, OSP (+ conditionals) |
| DEEP        | > 5000 chars, or multi-section spec, or `--deep`          | S1, S2, S3, S4, S5, S6, S7, OSP (+ conditionals) |
| CONJECTURE  | `--conjecture` flag                                      | S1, S6.1, S7, OSP                          |

### Flags

| Flag           | Effect                                                                          |
|----------------|----------------------------------------------------------------------------------|
| `--minimal`    | Force MINIMAL scale                                                              |
| `--standard`   | Force STANDARD scale                                                             |
| `--deep`       | Force DEEP scale                                                                 |
| `--conjecture` | Pattern/conjecture pipeline (S1 → S6.1 → S7)                                     |
| `--xml`        | Emit `output.xml` (envelope model, each stage's markdown wrapped in CDATA)       |
| `--quiet`      | Suppress terminal output; save silently                                          |
| `--verbose`    | Expand section depth in distilled output (no-op with `--xml`)                    |
| `--no-save`    | Skip `report.{md,xml}`; stage artifacts still written under `stages/`            |
| `--resume`     | Resume a prior session at the next pending stage                                 |

---

## Output shape

The default distilled report contains 11 always-present sections and up to
2 conditional ones, each separated by `---`:

1. Pipeline Status Header
2. Headline Insight (+ creativity type: combinatorial / exploratory / transformational)
3. Theory Collisions
4. Discovery vs. Proof
5. Independence-Verified Bridges
6. Alternative Hypotheses
7. Density-Checked Falsification
8. Scope Limits *(V6 verbatim)*
9. Coherence Signals
10. Generalization Checks
11. **Representational Signals** *(conditional — appears only if S3.1 ran or V7 detected a frame shift)*
12. Open Questions & Next Probes
13. **Genius Mind Attributions** *(`--verbose` only — lists which stages contributed which claims, with trait citations)*

Stages that didn't run at your scale emit a canonical empty-section marker,
e.g. `*(Stage S4 skipped at STANDARD scale.)*`, so the structure is stable
across scales — a downstream parser never has to handle missing sections.

---

## Architecture

The skill enforces a **three-layer rule** that keeps context usage bounded
and reasoning auditable:

```
┌──────────────────────────────────────────────────┐
│  Orchestrator (SKILL.md — the main agent)        │
│  - parses flags, validates input, inits session  │
│  - detects scale, builds wave plan               │
│  - spawns stages wave-by-wave via Agent tool     │
│  - handles conditionals, retries, and OSP        │
│  - runs tests, writes session.md observability   │
│  - NEVER reads KB files or runs stage protocols  │
└──────────────────────┬───────────────────────────┘
                       │ spawns (Agent tool)
                       ▼
┌──────────────────────────────────────────────────┐
│  Stage subagents (modules/*.md)                  │
│  - each loads ONE module file + its declared     │
│    kb_sources + input_dependencies               │
│  - runs its PROTOCOL                             │
│  - writes one output file to stages/             │
│  - returns {status, summary, signals}            │
│  - does NOT see the full pipeline state          │
└──────────────────────┬───────────────────────────┘
                       │ reads
                       ▼
┌──────────────────────────────────────────────────┐
│  Knowledge base (kb/ — 18 operational files)     │
│  - spreading-activation, de Bono lateral,        │
│    Ohlsson de-fixation, Collins & Loftus,        │
│    verification gates, Boden types, elegance     │
│    rubric, Tesla simulation, observer frames,    │
│    domain catalog, vocabulary rubric, etc.       │
└──────────────────────────────────────────────────┘
```

**Why it matters:** The orchestrator never grows in token count as the
pipeline runs deeper — each subagent is born with a minimal context
(one module + its declared inputs) and dies with its output written to disk.
The skill can run a DEEP session with S3.1 retry and S6 re-run without ever
overflowing the orchestrator's window, because the orchestrator never
holds any stage's reasoning.

### Index-as-truth

`index.json` is the single source of truth for:

- stage metadata (wave, scale gates, activation predicates)
- KB and input dependencies
- XML element mapping (for envelope output)
- signal → conditional module routing
- scale auto-detection thresholds

Scripts (`test-runner.sh`, `xml-assemble.sh`, `validate-pipeline.sh`) read
`index.json` rather than hard-coding stage lists — adding a new stage is a
one-file edit, not a seven-file sweep.

### Collision-safe sessions

Session directories are named `YYYYMMDD-{topic-slug}`. If you run two
sessions on the same topic on the same day, `session-init.sh` appends `-1`,
`-2`, etc. so nothing overwrites a prior run. Every session is recoverable.

### Observability

Each session's `stages/session.md` records:

- `spawns_total` — how many subagents fired (including retries and OSP)
- `wall_seconds` — total wall-clock time from start to test report
- `retry_path_taken` — `pathA` / `pathB` / `pathB_skipped` / `none`

So you can answer "what did this session actually cost?" without scraping logs.

---

## File layout

```
~/.claude/skills/epiphany-genius/
├── SKILL.md                 Orchestrator (the main agent reads this)
├── README.md                This file
├── index.json               Module registry — waves, activation, dependencies
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
│   ├── spreading-activation.md        (Collins & Loftus update equation)
│   ├── ohlsson-defixation.md          (3 mechanisms of insight)
│   ├── debono-techniques.md           (provocation / random-entry / reversal)
│   ├── domain-catalog.md              (25+ domains for S2 analogy search)
│   ├── blend-template.md              (conceptual blending)
│   ├── verification-gates.md          (V1–V7 operational reference)
│   ├── boden-types.md                 (creativity classification)
│   ├── elegance-rubric.md             (simplicity + symmetry + depth)
│   ├── tot-templates.md               (tree-of-thought inference templates)
│   ├── falsification-checklists.md    (extreme scenarios, Millikan test)
│   ├── pattern-taxonomy.md            (Ramanujan-mode patterns)
│   ├── observer-frames.md             (Einstein 3-frame embedding)
│   ├── simulation-checklist.md        (Tesla pre-visualization)
│   ├── representation-frames.md       (symbolic ↔ spatial ↔ procedural)
│   ├── vocabulary-rubric.md           (5-level jargon stripping)
│   ├── forward-chain-template.md      (10-step consequence trees)
│   ├── input-preloading-templates.md  (5 S1 input-class templates)
│   └── scope-template.md              (V6 scope-statement format)
├── scripts/
│   ├── session-init.sh      Collision-safe session folder creation
│   ├── validate-stage.sh    Per-stage output verification
│   ├── xml-assemble.sh      Index-driven XML envelope assembly
│   ├── test-runner.sh       T1–T5 post-run test battery
│   └── validate-pipeline.sh Dev/CI — 12 DAG and schema integrity checks
```

### Session output

```
~/docs/epiphany/genius/{session_id}/
├── report.md                   Final report (copy of output-distilled.md)
├── report.xml                  (if --xml)
└── stages/
    ├── session.md              Manifest — id, flags, observability counters
    ├── 00-processed-input.md   Routed input (handles prompt-epiphany XML etc.)
    ├── S1-state-loading.md     …through S7-integration.md
    ├── S7-v6-scope.txt         V6 scope statement (verbatim carve-out target)
    ├── output-distilled.md     Full distilled document (source of report.md)
    ├── output.xml              XML envelope (when --xml)
    ├── test-report.md          T1–T5 results
    └── validation-log.md       Per-stage PASS/FAIL audit trail
```

---

## Development and CI

```bash
# Validate pipeline integrity — 12 checks covering schema, DAG ordering,
# XML element uniqueness, signal routing, optional-dependency resolution,
# mode-shift references, KB file existence, and cycle detection.
bash ~/.claude/skills/epiphany-genius/scripts/validate-pipeline.sh
```

All 12 checks must pass before merging changes to `index.json`, stage
modules, or KB files. Running the validator is ~1 second and catches the
common breakages (added a stage but forgot to register its `xml_element`;
changed a signal name but didn't update the `signals` map; referenced a KB
file that doesn't exist; introduced a DAG cycle through `input_dependencies`).

### Adding a new stage

1. Write `modules/SN-new-stage.md` with frontmatter (stage_id, wave,
   scale_gates, activation, kb_sources, input_dependencies, output_file,
   xml_element, required_output_sections, raises_signals).
2. Register it in `index.json` under `stages` or `conditional_modules`.
3. Update `SKILL.md` wave-plan tables for any affected scale.
4. Update the Quick Reference stages table.
5. Run `validate-pipeline.sh` — it will flag anything missing.

No edits are needed to `test-runner.sh`, `xml-assemble.sh`, or
`validate-stage.sh` — they read `index.json` at runtime.

---

## Why the stages are in this order

The sequence is not arbitrary. It follows the **generate-then-evaluate**
discipline that distinguishes deep reasoning from plausible-sounding
reasoning:

- **Ideation phase** (S1–S4) generates material without judging it.
  Stripping, domain-mapping, spreading-activation, and simulation all produce
  candidate content. Evaluation here contaminates generation.
- **Mode Shift** marks the commitment to stop generating.
- **Synthesis phase** (S5–S7) commits to precision, searches for falsifying
  evidence, and integrates — three operations that require the full generated
  pool and that penalize premature closure.
- **OSP** does the last-mile distillation: it does not add new reasoning,
  only selects load-bearing fragments from the synthesis stages and lays
  them out in the canonical structure.

Skipping the boundary — evaluating while generating — is the failure mode
that produces fluent but shallow LLM output. The pipeline's structure makes
skipping it impossible.

---

## Versioning and status

`epiphany-genius` is a living artifact — each module is versioned in its
frontmatter, so any session's output tells you which version of each stage
produced which artifact.

- If a stage's behavior changes, bump its `module_version`.
- If the pipeline shape changes (new stage, new signal, new verification
  gate, altered wave topology), bump `index.json`'s top-level `version`.

## License

MIT — see [`LICENSE`](LICENSE).

## Contributing

Issues and pull requests are welcome. Before opening a PR:

1. Run `scripts/validate-pipeline.sh` — all 12 checks must pass.
2. If you add a stage, module, or KB file, update `index.json` and the
   relevant `GENIUS PERSPECTIVES` frontmatter block so trait attribution
   stays auditable.
3. Keep KB files operational, not encyclopedic — each KB file should be
   load-bearing for a specific stage protocol step.
