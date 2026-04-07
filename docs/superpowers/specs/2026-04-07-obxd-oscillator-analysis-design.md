# OB-Xd Oscillator Analysis — Methodology Design

**Date:** 2026-04-07
**Status:** Approved (brainstorming complete, awaiting writing-plans)
**Brief:** `~/prompts/ob-xd-oscillator-spec-verbose.md`

## Purpose

This document specifies the **methodology** for analyzing the OB-Xd 2.19 source tree at `~/synth/OB-Xd-2.19` and producing a portable, parameterized oscillator specification that downstream AI agents can use to implement OB-Xd's oscillator design in new JUCE VST synthesizers.

This document is **not** the analysis itself. It is the design for a multi-stage subagent pipeline that, when executed, *produces* the analysis. The actual analysis runs during the writing-plans → implementation phase that follows this brainstorming session.

The methodology is designed to be:
- **Reliable** — bounded subagent inputs, no token budget surprises, failure-isolated stages
- **Verifiable** — every claim cited, every artifact reviewed, dual citations in the final spec
- **Portable** — re-runnable against different open-source JUCE synths via config swap, with zero changes to subagent contracts or schemas
- **Faithful** — synthesis cannot fabricate; creative work is explicitly carved into `## TODO — human step` blocks

---

## Section 1 — Pipeline & Artifacts

### Pipeline Inputs (provided once at start, not a stage)

```
config.yaml:
  target_repo_path:        ~/synth/OB-Xd-2.19
  target_synth_name:       OB-Xd
  target_synth_version:    "2.19"
  target_synth_license:    GPLv2
  working_dir:             ~/synth-research/OB-Xd-oscillator-analysis/
  git_track_artifacts:     true
  default_voice_count:     8        # OB-Xd native — example only
  default_oscs_per_voice:  2        # OB-Xd native — example only
  worked_example_target:   "4 voices, 2 osc/voice"
```

The config is read by every stage. It is the *only* place OB-Xd-specific paths appear; swapping it (plus the source-file map in Stage 1) re-targets the entire pipeline to a different open-source synth.

### Stage list (10 stages)

| # | Stage | Reads | Writes (artifact) | Depends on |
|---|---|---|---|---|
| 1 | **Repo Map** | config + directory listing of `Source/Engine/` | `01-repo-map.md` (file inventory, line counts, role labels, BlepData.h flagged read-only-by-declaration, files >1,500 lines flagged as read-declaration-only candidates) | — |
| 2 | **Entry Trace** | `01` + ObxdOscillatorB.h, ObxdVoice.h | `02-entry-trace.md` (which class is the per-voice oscillator block, where it is instantiated, where it feeds) | 1 |
| 3a | **Saw Extraction** | SawOsc.h | `03a-saw.md` (algorithm, BLEP usage, hard-sync handling, formulas, line-cited) | 2 |
| 3b | **Pulse Extraction** | PulseOsc.h | `03b-pulse.md` (PWM, BLEP, sync, formulas) | 2 |
| 3c | **Triangle Extraction** | TriangleOsc.h | `03c-triangle.md` (integration of square, leakage, formulas) | 2 |
| 3d | **Per-Voice Composite** | ObxdOscillatorB.h | `03d-osc-composite.md` (osc1+osc2+noise mix, xmod path, sync routing, delay-line compensation, dirt injection, final gain `*3`) | 2 |
| 4a | **Voice Allocation (Motherboard)** | Motherboard.h, VoiceQueue.h | `04a-voice-alloc.md` (allocation, stealing, polyphony count) | 2 |
| 4b | **Voice Routing (SynthEngine)** | SynthEngine.h | `04b-voice-routing.md` (how SynthEngine drives voices, shared vs per-voice state) | 2 |
| 4c | **Per-Voice Wrapper** | ObxdVoice.h | `04c-voice-wrapper.md` (voice lifecycle, per-voice state, what owns the oscillator block) | 2 |
| 5 | **Anti-Alias / BLEP Strategy** | SawOsc.h, PulseOsc.h, TriangleOsc.h, BlepData.h *(declaration only — never read body)*, Decimator.h | `05-antialias.md` (BLEP table dimensions, oversampling toggle, decimation path) | 2 |
| 6 | **Analog Character Inventory** | 3d + 4a + 4b + 4c outputs | `06-analog-character.md` (phase randomization, per-voice random detune factors, dirt mechanism, perpetual noise floor `+0.0006`, drift; also lists what was *looked for and not found*) | 3d, 4a, 4b, 4c |
| 7 | **Parameters & Tuning** | Params.h, ParamsEnum.h, Tuning.h, midiMap.h | `07-parameters.md` (every osc-affecting parameter, range, curve, default; tuning table; modulation routing) | 2 |
| 8 | **Adjacent Systems Survey** | Filter.h, AdsrEnvelope.h, Lfo.h, AudioUtils.h | `08-adjacent.md` (identification only — name, role, coupling to oscillator stage, "must-replicate vs. JUCE-module-equivalent" verdict per system) | 2 |
| 9 | **Per-Artifact Review** | one reviewer subagent **per** artifact from 3a/3b/3c/3d/4a/4b/4c/5/6/7/8 | `09-review/<artifact>.md` (verifies citations, formulas, completeness; flags disputes) | the artifact under review |
| 10 | **Final Synthesis** | all extraction artifacts + all review artifacts | `10-final-spec.md` (the deliverable: portable, parameterized oscillator spec) + `BLOCKERS.md` | 9 (all reviews) |

### Dependency graph (parallel fan-out)

```
                   [Inputs: config.yaml]
                            │
                       [1 Repo Map]
                            │
                      [2 Entry Trace]
                            │
   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
   │     │     │     │     │     │     │     │     │
 [3a]  [3b]  [3c]  [3d]  [4a]  [4b]  [4c]  [5]   [7]   [8]
 saw  pulse  tri  comp  m/b  s/e  voice blep param adj
   │     │     │   │     │     │     │     │     │     │
   │     │     │   └──┬──┴─────┴─────┘     │     │     │
   │     │     │      │                    │     │     │
   │     │     │   [6 Analog Char]         │     │     │
   │     │     │      │                    │     │     │
   └──┬──┴──┬──┴──┬───┴──┬──┬──┬──┬──┬──┬──┴──┬──┴──┐
      │     │     │      │  │  │  │  │  │     │     │
   [9 reviewers — one per artifact, run in parallel]
                          │
                  [10 Final Synthesis]
```

Stages 3a–c, 3d, 4a–c, 5, 7, 8 all fan out in parallel after Stage 2. Stage 6 is the only one that needs cross-artifact synthesis before review (it depends on 3d + all of 4a–c). Stage 9 dispatches one reviewer per extraction artifact, also in parallel.

### Storage & git tracking

- All artifacts written under `~/synth-research/OB-Xd-oscillator-analysis/` (set in `config.yaml`).
- This directory is its own git repo, committed after every successful stage. Failure of stage N never corrupts artifacts from 1..N-1, and the user can `git diff` to see exactly what each subagent produced.
- Reviewer outputs go in a sibling subdirectory `09-review/` so a failed review can be re-run without touching the extraction.

### Two notes for portability

1. **Artifact SHAPE is generic across synths; FILE NAMES in this list are OB-Xd-specific.** A future run against a different open-source JUCE synth swaps the source-file map in Stage 1 and the per-stage `Reads` column, but the artifact list (1, 2, 3a–d, 4a–c, 5, 6, 7, 8, 9, 10) and dependency graph stay the same.
2. **BlepData.h is read by declaration only.** It is a precomputed lookup table with thousands of float entries; subagents extract its dimensions, type, and purpose from the header line and surrounding context, never the body. This rule is in the Stage 5 subagent contract.

---

## Section 2 — Subagent Contracts & Reliability

**Orchestrator (defined once, used throughout):** the main Claude Code session running this methodology. It is the only thing that launches subagents (via the Agent tool), reads their final messages, scans the working directory, computes per-stage INPUTS lists, and decides retries.

### 2.1 Contract template (every extraction subagent)

```
ROLE
  You are a DSP code reader extracting <artifact name> from <target synth>.
  You produce one Markdown file. You do not modify source. You do not run code.

INPUTS (read-only)
  - config.yaml at <abs path>
  - Source files: <explicit absolute paths, NEVER a directory>
  - Prior artifacts: <explicit absolute paths to artifacts you may consult>

FORBIDDEN READS
  - Any file marked READ-DECLARATION-ONLY in 01-repo-map.md
    (BlepData.h is the canonical example — read its declaration line only,
     never the lookup-table body)
  - Any file not on the INPUTS list above

  Note: the FORBIDDEN list is a deterrent. The real protection is that your
  INPUTS list is bounded — if a file isn't there, you have no reason to open it.

OUTPUT
  Write exactly one file: <abs path to artifact>
  Use the schema in section "ARTIFACT SCHEMA" below.
  Do not write anywhere else. Do not create extra files.

  If you cannot complete the artifact, write a top-level
  "## FAILED — <one-line reason>" section into the same artifact path.
  This is your only failure channel. Do not create a separate error file.

ARTIFACT SCHEMA
  <stage-specific schema — see 2.3>

CITATION RULE
  Every claim about behavior must cite file:line. No paraphrase without citation.
  If you cannot cite it, write "UNCITED — needs reviewer attention" and continue.

UNKNOWN RULE
  If something is unclear, write it under "## Open Questions" at the bottom.
  Do not guess. Do not invent. Reviewer will resolve.

STOP CONDITIONS
  Stop when the schema is filled. Do not explore beyond INPUTS.
  If you discover you need a file outside INPUTS, do not open it — instead,
  write a "## FAILED — missing input <path>" section and stop.

TOKEN BUDGET
  Soft guidance: <stage-specific>. There is no hard meter; the real bound
  is your INPUTS list. Stay focused on filling the schema, not exploring.

FORBIDDEN ACTIONS (universal)
  - Do not invoke other subagents (only the orchestrator does that)
  - Do not run code, build anything, or modify source files
  - Do not write outside your declared artifact path
```

### 2.2 Token budgets (input/output guidance per stage)

Wall-clock caps are not enforceable through the Agent tool. Token budgets remain as guidance to the subagent, with the bounded INPUTS list as the real bound.

| Stage | Source LOC | Read budget | Write budget |
|---|---|---|---|
| 1 Repo Map | dir listing only | 5k tok | 3k tok |
| 2 Entry Trace | ~574 | 15k tok | 4k tok |
| 3a Saw | 128 | 8k tok | 5k tok |
| 3b Pulse | 194 | 10k tok | 5k tok |
| 3c Triangle | 183 | 10k tok | 5k tok |
| 3d Composite | 253 | 15k tok | 8k tok |
| 4a Motherboard | 385 | 20k tok | 6k tok |
| 4b SynthEngine | 603 | 30k tok | 8k tok |
| 4c Voice | 321 | 18k tok | 6k tok |
| 5 Anti-alias | ~591 (BlepData body excluded) | 25k tok | 6k tok |
| 6 Analog Char | 4 prior artifacts | 15k tok | 6k tok |
| 7 Parameters | ~694 | 30k tok | 8k tok |
| 8 Adjacent | ~584 | 25k tok | 6k tok |
| 9 Review | 1 artifact + filtered cited files | 20k tok | 4k tok |
| 10 Synthesis | all artifacts | 150k tok | 20k tok |

Largest single read is SynthEngine.h at 603 lines — comfortably within one Read call. No subagent ever needs multi-pass reading except BlepData.h, which is forbidden.

### 2.3 Stage-specific artifact schemas

**13 schemas to be authored as part of writing-plans:**

| # | Schema | Stage |
|---|---|---|
| 1 | Repo Map | 1 |
| 2 | Entry Trace | 2 |
| 3 | Saw / Pulse / Triangle (one schema, three instances) | 3a, 3b, 3c |
| 4 | Per-Voice Composite | 3d |
| 5 | Voice Allocation | 4a |
| 6 | Voice Routing | 4b |
| 7 | Per-Voice Wrapper | 4c |
| 8 | Anti-alias / BLEP Strategy | 5 |
| 9 | Analog Character Inventory | 6 |
| 10 | Parameters & Tuning | 7 |
| 11 | Adjacent Systems Survey | 8 |
| 12 | Review (one shape, dispatched per artifact) | 9 |
| 13 | Final Synthesis | 10 |

**Example: 3a Saw extraction schema:**

~~~markdown
# Saw Oscillator (SawOsc.h)

## Source
- File: <abs path>
- Lines: <total>
- Class: SawOsc

## Algorithm
- One-paragraph plain-English summary

## Phase accumulator
- How phase advances (cite line)
- Range and wrap behavior (cite line)

## Anti-aliasing
- Method (BLEP / polyBLEP / oversampling / none)
- Table referenced (cite line)
- Correction insertion point (cite line)

## Hard sync support
- Master path: function name, behavior, cite lines
- Slave path: function name, behavior, cite lines
- Sync fraction handling (cite line)

## Pseudocode
```
<language-neutral pseudocode reproducing the algorithm>
```

## Formulas
- Each transformation as `out = f(in)` with line citation

## Constants
| Name | Value | File:line | Purpose |

## Open Questions
- <anything unclear>

## Generalization notes
- What in this implementation is OB-Xd-specific vs reusable
- What a portable version would need to parameterize
~~~

The other 12 schemas follow analogous patterns and will be authored during the writing-plans phase.

### 2.4 Reviewer contract (Stage 9)

One reviewer subagent per extraction artifact. **Reviewer INPUTS are computed by the orchestrator, not by the reviewer**: the orchestrator parses the artifact's citations, filters them against the FORBIDDEN list (anything READ-DECLARATION-ONLY is dropped), and passes the resulting allowlist explicitly. Reviewers cannot dynamically open files.

```
ROLE
  You are a verification reviewer. You read ONE extraction artifact and
  the source files the orchestrator pre-approved for you. You produce
  ONE review file.

INPUTS (read-only, allowlisted)
  - Extraction artifact: <abs path>
  - Pre-approved cited files: <explicit abs paths, filtered by orchestrator>

FORBIDDEN READS
  - Any file not on the allowlist above
  - Same READ-DECLARATION-ONLY rules as extraction subagents

CHECKS (in order)
  1a. Citation validity:    every file:line resolves and supports the claim
  1b. No forbidden refs:    no citation points to a READ-DECLARATION-ONLY file
  2.  Schema completeness:  every required schema section is present
  3.  Formula correctness:  each formula matches the cited code
  4.  Pseudocode fidelity:  pseudocode reproduces observable behavior
  5.  Generalization:       "generalization notes" section is non-empty and specific
  6.  Open Questions:       triaged into RESOLVED / DEFER-TO-SYNTHESIS / BLOCKER

OUTPUT
  09-review/<artifact-basename>.md with:
    - Verdict: PASS / PASS-WITH-NOTES / FAIL
    - List of issues with severity
    - Suggested fixes (do NOT rewrite the extraction)

STOP CONDITIONS
  Do not re-extract. Do not propose new content. Only verify.
```

### 2.5 Failure handling

**Detection limits (be honest about what the orchestrator can and cannot see):**

The orchestrator sees a subagent's final message and can scan the working directory before/after each run. It **cannot** see the subagent's intermediate tool calls. This means:
- Forbidden file reads can be detected only via (a) the FORBIDDEN list as a deterrent, (b) working-dir scans catching files written outside the declared artifact path, and (c) reviewer catching uncited or impossibly-detailed claims that could only have come from an off-list file.
- Token budgets are guidance, not enforced — the bounded INPUTS list is the real protection.

| Failure mode | Detection | Recovery |
|---|---|---|
| Subagent returns empty / unusable artifact | Orchestrator post-read of artifact path | Mark FAIL, retry once with halved scope (drop "generalization notes" first) |
| Subagent self-reports `## FAILED — <reason>` | Parse artifact header | Treat as attempt 1 failure → retry once with halved scope |
| Subagent writes outside declared artifact path | Orchestrator working-dir diff before/after | Discard side-effects, mark FAIL, retry once |
| Subagent invents uncited claims | Reviewer check 1a | Mark PASS-WITH-NOTES; synthesis stage strips uncited claims |
| Subagent cites a forbidden file | Reviewer check 1b | Mark FAIL, re-run with stricter prompt reminding of FORBIDDEN list |
| Reviewer disputes extraction (FAIL) | Reviewer verdict | Synthesis receives both artifacts and resolves; unresolvable disputes → write to `BLOCKERS.md` |
| Stage 6 (analog character) inputs incomplete | Dependency check on 3d, 4a, 4b, 4c | Mark Stage 6 **DEFERRED**, do not run with gaps (analog character is the easiest section to fabricate; refuse to run blind) |
| Same stage fails twice | Orchestrator retry counter | Stop, write `FAILED-<stage>.md` with diagnostic, do not block downstream stages that don't depend on it |

**Retry chain (explicit):** attempt 1 (full scope) → on failure, attempt 2 (halved scope: drop "generalization notes" first, then "Open Questions" if still failing) → on second failure, write `FAILED-<stage>.md` and stop the stage. Downstream stages with no dependency on the failed one continue normally.

**Reviewer disagreement:** if reviewer marks FAIL on an artifact, the orchestrator passes both the extraction and the review to Stage 10 (synthesis). Synthesis attempts to resolve. If it cannot, it writes the conflict into `BLOCKERS.md`. After Stage 10 returns, the orchestrator reads `BLOCKERS.md` and surfaces blockers to the user — synthesis itself cannot escalate mid-run.

### 2.6 Universal forbidden actions (extraction and reviewer subagents)

- Read any file not in INPUTS
- Read any file marked READ-DECLARATION-ONLY beyond its declaration line
- Run code, build any project, or modify any source file
- Write outside the declared artifact path
- **Invoke other subagents** (only the orchestrator does this)
- Make claims without file:line citations
- Guess at unclear behavior — write Open Questions instead
- Reformat or "improve" the artifact schema — fill it as given

---

## Section 3 — Verification & Final Synthesis

This is Stage 10. Its job is to turn 13 extraction artifacts + 11 review files into one portable, parameterized oscillator specification — and to surface anything it cannot resolve.

**Note on reviewer authority (intentional design choice):** Rule 1 below makes reviewers authoritative over synthesis. This is deliberate — reviewers exist precisely so synthesis doesn't second-guess. A future reader should not try to "fix" this by adding an override path.

### 3.1 Synthesis subagent contract

```
ROLE
  You are the final synthesis subagent. You read every extraction artifact
  and every review artifact and produce ONE deliverable: the portable
  oscillator specification. You also produce BLOCKERS.md if you find
  unresolvable conflicts.

INPUTS (read-only, all explicit absolute paths)
  The orchestrator computes this list at dispatch time, including any
  FAILED-<stage>.md markers it finds in the working dir, and passes the
  resolved list explicitly. You receive concrete paths, not globs.

  Typical contents:
  - config.yaml
  - 01-repo-map.md
  - 02-entry-trace.md
  - 03a-saw.md, 03b-pulse.md, 03c-triangle.md, 03d-osc-composite.md
  - 04a-voice-alloc.md, 04b-voice-routing.md, 04c-voice-wrapper.md
  - 05-antialias.md
  - 06-analog-character.md   (may be DEFERRED)
  - 07-parameters.md
  - 08-adjacent.md
  - 09-review/<all 11 review files>
  - Any FAILED-<stage>.md files the orchestrator found

FORBIDDEN READS
  - All original source files. You synthesize from artifacts only.
  - This is enforced by the INPUTS list, not by trust: source files are
    not on it.
  - READ-DECLARATION-ONLY rules still apply if you ever feel the urge.

OUTPUT
  Two files:
    1. 10-final-spec.md   — the deliverable, schema in section 3.3
    2. BLOCKERS.md        — conflicts you could not resolve, schema in 3.4
                            (always starts with "# Synthesis Blockers";
                             empty case has "## No blockers" as sole sub-header)

CONFLICT RESOLUTION RULES
  See section 3.2. Apply in order. Do not skip.

CITATION POLICY
  The final spec uses dual citations: every claim cites both
    (a) the artifact path that synthesized it, and
    (b) the original source file:line that the artifact itself cited.
  This preserves chain-of-custody (a) while letting any reader verify
  against the actual code (b). Uncited claims are forbidden.

NO-FABRICATION RULE (sections 5, 9, 13)
  Sections 5 (scaling), 9 (portability), and 13 (prototype plan) contain
  parts that are creative engineering — not extractable from artifacts.
  For those parts, write only what you can cite. Anything that would
  require invention goes under a "## TODO — human step" sub-section with
  a bullet list of inputs the human will need. See 3.3 for exactly which
  parts are creative.

STOP CONDITIONS
  Stop when 10-final-spec.md is filled per the schema and BLOCKERS.md
  exists. Do not re-extract. Do not open source files.

TOKEN BUDGET
  150k input. If even that is tight, the orchestrator will split synthesis
  into 10a (spec sections 1–8) and 10b (sections 9–13 + portability),
  passing each half a different INPUTS slice.
```

### 3.2 Conflict resolution rules (applied in order)

When the same fact is asserted differently across artifacts, or an extraction and its review disagree, apply these rules in strict order. The first rule that produces a verdict wins.

1. **Reviewer FAIL on a specific claim → drop the claim.** Reviewer verdicts are authoritative for the artifact they reviewed. A failed citation is removed from the synthesis, even if other artifacts repeat it.

2. **PASS-WITH-NOTES → keep claim, attach a `[reviewer-note]` tag in the spec.** Reader sees the warning inline. Note text comes from the review file.

3. **Two extractions disagree on the same fact → prefer the more deeply-cited one.** A claim with three line citations beats a claim with one. Same citation count → **prefer the artifact whose stage is topically closest to the claim** in the dependency graph (a saw-oscillator claim from Stage 3a beats the same claim from Stage 3d; a voice-allocation claim from 4a beats one from 4c).

4. **Two extractions disagree and topical proximity is equal → write both into BLOCKERS.md as a `DISAGREEMENT` entry, drop both from the spec.** Better to omit than to pick wrong on something nobody can adjudicate.

5. **An extraction is FAILED and its content is needed → mark the corresponding spec section `## INCOMPLETE — depends on FAILED <stage>` and continue.** Do not invent substitutes.

6. **Stage 6 is DEFERRED → the spec's "Analog Character" section is replaced with a one-line reference to the deferred stage.** Do not partially fill it from other artifacts.

7. **An Open Question was tagged DEFER-TO-SYNTHESIS by a reviewer → resolve it only if the answer is unambiguously present in another artifact. Otherwise, copy it into BLOCKERS.md and continue.**

8. **Generalization conflict (one artifact says "OB-Xd-specific", another implies "reusable") → prefer OB-Xd-specific *only when* the parameterization would change observable behavior (e.g., a constant that determines the shape of a filter response, the order of an operation, the sign of a feedback path). Prefer reusable when the disagreement is about whether the value/algorithm is *conventional* (e.g., a standard BLEP table size, a typical envelope curve constant).**

   *Example:* If 3a says "the BLEP table has B_OVERSAMPLING=64, this is a standard choice" and 3d implies "the table size is part of OB-Xd's character," prefer reusable — the size is conventional. If 06 says "the `+0.0006` perpetual noise floor is a deliberate sonic choice" and 07 implies "any small constant works," prefer OB-Xd-specific — that exact value is the character.

9. **Mutual exclusivity:** Each item appears in exactly one place — either BLOCKERS.md (if it's a conflict or unresolvable defer) or spec section 12 (if it's a known gap with no claimant). Synthesis must not duplicate an item across both.

### 3.3 Final spec schema (`10-final-spec.md`)

13 numbered sections plus Appendix A. Sections 5, 9, and 13 contain explicit `## TODO — human step` sub-sections for creative work that synthesis is forbidden to fabricate.

```markdown
# OB-Xd Oscillator Subsystem — Portable Specification

## 1. Overview
- One-paragraph sonic-character description
- One-paragraph design-goals statement
- Source attribution: target synth name + version + license note from config

## 2. Architectural Diagram
- ASCII block diagram of the per-voice oscillator signal path
- Labeled interfaces to downstream systems (filter input, envelope, pan, mix bus)
- Explicit parameterization markers: {VOICE_COUNT}, {OSCS_PER_VOICE},
  {WAVEFORM_SET}, {UNISON_DEPTH}

## 3. Waveform Generators
For each waveform (saw, pulse, triangle, ...):
  - Algorithm summary
  - Pseudocode
  - Formulas
  - Constants table
  - Anti-alias method and where it inserts
  - Hard-sync behavior (if applicable)
  - Citation: artifact path + source file:line

## 4. Per-Voice Oscillator Composite
- Mix structure (osc1 + osc2 + sub + noise + xmod)
- Cross-modulation routing and delay-line compensation
- Hard sync routing (master → slave path)
- Output gain stage
- Citation: 03d artifact path + source file:line

## 5. Polyphony & Voice Architecture
- Native voice count (from config + 04a/04b/04c)
- Native oscs-per-voice
- Voice allocation policy (steal order, retrigger behavior)
- Per-voice vs shared state inventory
- Detune-across-voices model (cite the per-voice random factors and dirt
  injection from artifacts)
- ### Parameterization (cite-only — describe what OB-Xd does at native size)
  - Which structural values are hard-coded vs. configuration-driven
  - Which per-voice state is independent vs. shared
- ### TODO — human step: scaling derivation
  - Goal: scaling guidance for {VOICE_COUNT} ≠ native, {OSCS_PER_VOICE} ≠ native
  - Goal: worked example at 4 voices, 2 osc/voice
  - Inputs the human will need:
    - The per-voice state inventory above
    - The detune-across-voices model
    - Section 6 analog-character constants
    - A target audio test that defines "preserves character"

## 6. Analog Character Inventory
- Each technique observed in the source: name, mechanism, where it lives
- "Looked for and not found" sublist (explicit negative results)
- Citation: 06 artifact path + source file:line
- If 06 is DEFERRED: one line stating that, no fill-in

## 7. Parameters & Modulation
- Every osc-affecting parameter: name, range, curve, default, source artifact
- Modulation routing matrix
- Tuning system summary

## 8. Critical Adjacent Systems
For each system in 08-adjacent.md (filter, env, LFO, sat, mix, pan, output, ...):
  - Name, role in 1–3 sentences
  - Coupling to oscillator stage (what it receives/gives back)
  - Verdict: MUST-REPLICATE-FOR-CHARACTER or JUCE-MODULE-EQUIVALENT-OK
  - Citation: 08 artifact path + source file:line

## 9. Portability Guide
- What in this spec is OB-Xd-specific vs reusable
- Required interfaces for dropping the oscillator engine into a different
  JUCE host (AudioBuffer<float>, sample rate, MIDI note input, parameter API)
- ### Replacement categories (cite-only)
  - For each adjacent system marked JUCE-MODULE-EQUIVALENT-OK, list the
    *category* of replacement needed (filter, envelope, LFO, saturator, ...)
  - Do NOT name specific JUCE modules — that is a downstream human decision
- Required parametric inputs (the {ALL_CAPS} markers from section 2)
- ### TODO — human step: module selection
  - Goal: choose specific JUCE / open-source modules per replacement category
  - Inputs the human will need:
    - The replacement category list above
    - The MUST-REPLICATE verdicts from section 8
    - The target host project's existing module dependencies

## 10. Scaling Guidance
- Cross-reference to section 5's TODO — human step
- Cite-only material: state which constants in section 11 are tied to voice
  count vs. independent of it (only if any artifact says so explicitly)

## 11. Constants & Tuning Values
- Single table of every hard-coded numeric value that shapes the character
- Each row: name, value, artifact path, source file:line, role in the sound
- Dual citations are mandatory here — this table is the verification surface

## 12. Open Questions & Unknowns
- Anything tagged DEFER-TO-SYNTHESIS that synthesis could resolve
- Anything left genuinely unknown (this is a different list from BLOCKERS.md;
  unknowns are gaps, blockers are conflicts; an item appears in exactly one
  place)

## 13. Prototype Plan
- ### Cite-only test surface
  - Test cases derivable directly from artifacts (single voice playback,
    waveform selection per Stage 3a/b/c, polyphony allocation per 4a,
    hard sync routing per 3d, decimation toggle per 5)
- ### TODO — human step: audible criteria
  - Goal: define what "subtle inter-voice detuning for analog authenticity"
    should sound like as audible test criteria
  - Inputs the human will need:
    - Section 6 analog-character techniques and constants
    - Section 11 detune-related constants
    - A reference recording or A/B target to compare against

## Appendix A. Artifact Provenance
- Table mapping each spec section to the artifact(s) it cites
- Stage status: PASS / PASS-WITH-NOTES / FAIL / DEFERRED for every input
```

### 3.4 BLOCKERS.md schema

```markdown
# Synthesis Blockers

## Summary
- N disagreements
- M unresolved DEFER-TO-SYNTHESIS items
- K spec sections marked INCOMPLETE
- L spec sections marked DEFERRED

## Disagreements
For each:
  - Topic: <one line>
  - Artifact A says: <quote + path>
  - Artifact B says: <quote + path>
  - Why I cannot adjudicate: <one line>
  - Recommended human action: <one line>

## Unresolved Open Questions
- Question (from artifact <path>)
- Why other artifacts do not answer it

## Incomplete Spec Sections
- Section name + which FAILED stage caused the gap

## Deferred Spec Sections
- Section name + which DEFERRED stage caused it
```

If there are no blockers, the file still starts with `# Synthesis Blockers` and contains only `## No blockers` as its sole sub-header.

### 3.5 Orchestrator post-Stage-10 actions

After the synthesis subagent returns, the orchestrator does the following — none of it is delegated, all of it runs in the parent context:

1. Read `10-final-spec.md`. Verify it exists, has all 13 numbered sections plus Appendix A, and Appendix A is non-empty.
2. Read `BLOCKERS.md`. If non-empty, surface a summary to the user with file paths.
3. Run a final structural check on `10-final-spec.md`:
   - Every section header from the schema is present
   - Every `{ALL_CAPS}` parameterization marker in section 2 is also resolved or referenced in section 9
   - Sections 5, 9, and 13 contain their `## TODO — human step` sub-sections (or a one-line note that no human step is needed)
   - Appendix A's stage-status table covers all input artifacts
   - Section 11's constants table has dual citations (artifact + source line) on every row
4. Commit the working directory: `git add ~/synth-research/OB-Xd-oscillator-analysis/ && git commit -m "synthesis stage <run-id>"`.
5. Report to the user: artifact path, blocker count, deferred section count, TODO-human-step count, and the next recommended action.

### 3.6 Split-synthesis fallback (10a / 10b)

If the orchestrator's pre-dispatch sizing pass shows the synthesis input would exceed 150k tokens, it splits Stage 10:

- **Stage 10a — Cite-only synthesis (spec sections 1–8 + 11):** receives all extraction and review artifacts, fills the cite-heavy sections, writes a partial `10-final-spec.md`.
- **Stage 10b — Generalization synthesis (spec sections 9, 10, 12, 13 + Appendix A):** receives the partial spec from 10a + the generalization-relevant artifacts (06, 07, 08) + all reviews. Fills the remaining sections, including the TODO-human-step blocks.
- BLOCKERS.md is the union of blockers from both. The orchestrator merges before reading.

This split is invisible to the user — same final deliverable, same post-actions in 3.5.

### 3.7 What the synthesis subagent must NOT do

- Open any source file (its INPUTS list does not include them)
- Invent claims that are not in any artifact
- Resolve a disagreement by averaging or compromising — pick a rule from 3.2 or write to BLOCKERS.md
- Fill the Analog Character section if Stage 6 is DEFERRED
- Promise portability for things only one artifact identifies as parameterizable (rule 8)
- Name specific JUCE modules in section 9 (categories only)
- Fabricate audible criteria in section 13 (cite-only test surface; the rest is `## TODO — human step`)
- Fabricate scaling rules in section 5 (cite-only at native size; the rest is `## TODO — human step`)
- Skip writing BLOCKERS.md (must always exist, even if empty)
- Reformat or "improve" the final spec schema — fill it as given
- Duplicate items between BLOCKERS.md and spec section 12
- Override a reviewer FAIL (rule 1 is authoritative)

---

## Section 4 — Generalization Layer

This section answers: how does the methodology stay re-runnable against a *different* open-source JUCE synth without rewriting itself, while still being maximally effective for OB-Xd specifically?

The answer is a small set of parametric inputs and one swappable "source map" file. Everything else — the 10 stages, the artifact schemas, the conflict rules, the synthesis output — stays unchanged.

### 4.1 What is parametric vs. fixed

| Layer | Parametric per target | Fixed across targets |
|---|---|---|
| Pipeline shape (10 stages, dependency graph, fan-out) | — | ✅ |
| Subagent contracts and reliability rules | — | ✅ |
| Artifact schemas (their *shape*) | — | ✅ |
| Conflict resolution rules | — | ✅ |
| Final spec schema (sections 1–13 + Appendix A) | — | ✅ |
| Forbidden actions list | — | ✅ |
| Required-stage table (4.3) | — | ✅ (canonical reference, implemented as a fixed check in the orchestrator) |
| Repo path, synth name, version, license | ✅ | — |
| Source-file map (which file → which stage) | ✅ | — |
| READ-DECLARATION-ONLY list | ✅ | — |
| Enabled-stages allowlist | ✅ | — |
| Native voice count, native oscs-per-voice | ✅ | — |
| Worked-example target (e.g., "4v/2osc") | ✅ | — |
| Working dir + git tracking | ✅ | — |
| Per-stage token budget overrides | ✅ (optional; defaults to Section 2.2) | — |

### 4.2 The two configuration files

**`config.yaml`** (canonical spec):

```yaml
target_repo_path:        ~/synth/OB-Xd-2.19
target_synth_name:       OB-Xd
target_synth_version:    "2.19"
target_synth_license:    GPLv2     # surfaced in spec section 1's attribution
working_dir:             ~/synth-research/OB-Xd-oscillator-analysis/
                                   # default: ~/synth-research/<target_synth_name>-oscillator-analysis/
                                   # override here if you want a different path
git_track_artifacts:     true
default_voice_count:     8         # native — example only
default_oscs_per_voice:  2         # native — example only
worked_example_target:   "4 voices, 2 osc/voice"
source_map:              ./source-map.yaml    # resolved relative to config.yaml's directory

enabled_stages:
  - 1_repo_map
  - 2_entry_trace
  - 3a_saw
  - 3b_pulse
  - 3c_triangle
  - 3d_composite
  - 4a_voice_alloc
  - 4b_voice_routing
  - 4c_voice_wrapper
  - 5_antialias
  - 6_analog_character     # omit to skip — Stage 6 reads only artifacts so source-map cannot skip it
  - 7_parameters
  - 8_adjacent
  - 9_review               # implicit if any extraction stage runs
  - 10_synthesis           # required
```

**`source-map.yaml`** (per-target, sits next to `config.yaml`):

```yaml
# Maps each pipeline stage to the source files it reads.
# This is the ONLY file that needs editing to re-target the pipeline.
# Stage YAML keys are lowercase snake_case versions of Section 1's stage labels.
# All `inputs:` paths are resolved relative to config.yaml's `target_repo_path`.

read_declaration_only:
  - Source/Engine/BlepData.h     # large lookup table — declaration only

# Optional per-stage token budget overrides. Defaults come from Section 2.2.
# Use when a target has files significantly larger or smaller than OB-Xd's.
overrides:
  4b_voice_routing:
    read_budget:  30k
    write_budget: 8k

stages:
  1_repo_map:
    inputs: [Source/Engine/]
  2_entry_trace:
    inputs:
      - Source/Engine/ObxdOscillatorB.h
      - Source/Engine/ObxdVoice.h
  3a_saw:
    inputs: [Source/Engine/SawOsc.h]
  3b_pulse:
    inputs: [Source/Engine/PulseOsc.h]
  3c_triangle:
    inputs: [Source/Engine/TriangleOsc.h]
  3d_composite:
    inputs: [Source/Engine/ObxdOscillatorB.h]
  4a_voice_alloc:
    inputs:
      - Source/Engine/Motherboard.h
      - Source/Engine/VoiceQueue.h
  4b_voice_routing:
    inputs: [Source/Engine/SynthEngine.h]
  4c_voice_wrapper:
    inputs: [Source/Engine/ObxdVoice.h]
  5_antialias:
    inputs:
      - Source/Engine/SawOsc.h
      - Source/Engine/PulseOsc.h
      - Source/Engine/TriangleOsc.h
      - Source/Engine/Decimator.h
      - Source/Engine/BlepData.h    # declaration line only (forced by read_declaration_only)
  7_parameters:
    inputs:
      - Source/Engine/Params.h
      - Source/Engine/ParamsEnum.h
      - Source/Engine/Tuning.h
      - Source/Engine/midiMap.h
  8_adjacent:
    inputs:
      - Source/Engine/Filter.h
      - Source/Engine/AdsrEnvelope.h
      - Source/Engine/Lfo.h
      - Source/Engine/AudioUtils.h

# Stages 6, 9, 10 read artifacts only — they appear in enabled_stages, not here.
# An absent stage block means "stage not present in this target" (see 4.3).
# An empty `inputs: []` block means "stage runs but reads no source" (artifacts-only).
```

### 4.3 Required vs. optional stages per target

This table is the canonical reference. The orchestrator implements it as a fixed check at startup.

**Definition:** a stage is *missing* if its YAML block is absent from `source-map.yaml` (for source-reading stages) or absent from `enabled_stages` in `config.yaml` (for artifacts-only stages). An empty `inputs: []` block means "stage runs but needs no source files," not "stage missing."

| Stage | Required? | If missing in target |
|---|---|---|
| 1 Repo Map | required | cannot run pipeline |
| 2 Entry Trace | required | cannot run pipeline |
| 3a/3b/3c | at least one waveform required | absent waveforms marked NOT-PRESENT in spec section 3 |
| 3d Composite | required | cannot run pipeline |
| 4a/4b/4c | at least one voice-management stage required | absent stages marked NOT-PRESENT |
| 5 Anti-alias | optional | spec section 3 anti-alias subsection becomes "none observed" |
| 6 Analog Char | optional but recommended | spec section 6 becomes "no analog-character techniques observed" |
| 7 Parameters | required | cannot run pipeline |
| 8 Adjacent | optional but recommended | spec section 8 becomes "no adjacent systems surveyed" |
| 9 Review | required for any stage that ran | per-artifact, automatic |
| 10 Synthesis | required | cannot run pipeline |

The orchestrator checks `source-map.yaml` and `enabled_stages` against this table at startup. Missing required stages → fail before launching any subagent. Missing optional stages → log and continue.

### 4.4 Re-targeting checklist (the workflow for a new synth)

1. Identify the new target's source root and put it in `config.yaml` as `target_repo_path`.
2. Map each stage to source files in the new repo using this role table:

| Role | Stage | YAML key |
|---|---|---|
| Repo overview | 1 | `1_repo_map` |
| Per-voice oscillator entry point + voice wrapper | 2 | `2_entry_trace` |
| Saw waveform generator | 3a | `3a_saw` |
| Pulse waveform generator | 3b | `3b_pulse` |
| Triangle waveform generator | 3c | `3c_triangle` |
| Per-voice oscillator composite (sums osc1 + osc2 + noise) | 3d | `3d_composite` |
| Voice allocation / voice manager | 4a | `4a_voice_alloc` |
| Voice routing / synth engine | 4b | `4b_voice_routing` |
| Voice wrapper (per-voice state container) | 4c | `4c_voice_wrapper` |
| Anti-alias support files | 5 | `5_antialias` |
| Parameters / tuning / MIDI mapping | 7 | `7_parameters` |
| Adjacent systems (filter, env, LFO, ...) | 8 | `8_adjacent` |

3. Write `source-map.yaml` mapping each role to the new file paths (relative to `target_repo_path`).
4. Run Stage 1 (Repo Map) by itself first. It will report any source file over 1,500 lines as a candidate for `read_declaration_only`. Review the list and add legitimate large lookup tables (BlepData equivalents) to that block.
5. Set `default_voice_count`, `default_oscs_per_voice`, and `worked_example_target` from the new synth's docs.
6. Set `enabled_stages` in `config.yaml`. Include `6_analog_character`, `7_parameters`, `8_adjacent`, `9_review`, `10_synthesis` for a full run; omit any optional stage you want to skip.
7. **Run the pipeline.** Concretely: open Claude Code, point it at `config.yaml`, and ask it to execute the methodology. The orchestrator (the main Claude session) reads the configs, pre-flights every source-map path against the filesystem, validates against the required-stage table in 4.3, and only then begins launching subagents. (The methodology described in this whole document becomes actual prompts and orchestration logic during the writing-plans → implementation phase that follows this brainstorming session.)

Pre-flight failures abort the run with a clear error before any subagent launches. Typos in source-map paths surface immediately, not after a Stage 3a retry loop.

### 4.5 What this layer does NOT generalize

Three things are *not* abstracted, by deliberate choice:

1. **The OB-Xd-specific parts of the final spec (sections 5, 6, 11) are about OB-Xd.** When run against another synth, those sections will contain that synth's facts instead. There is no cross-synth comparison layer in v1 — that would be a separate methodology.

2. **The READ-DECLARATION-ONLY rule is per-target.** BlepData.h is OB-Xd's lookup table; another synth might have an equivalent file with a different name (or none at all, or several). 4.4 step 4 helps surface candidates via Stage 1's >1,500-line report, but the user still makes the final call. There is no auto-classification.

3. **License handling is attribution, not enforcement.** `target_synth_license` is consumed by the synthesis subagent and surfaced verbatim in spec section 1's attribution line (per Section 3.3's schema). The methodology does not check whether GPL-derived facts can be relicensed into the user's downstream product — that is a human/legal decision outside the agent's scope.

### 4.6 Effect on OB-Xd-specific effectiveness

Adding this generalization layer has near-zero overhead for the OB-Xd run, plus a one-time pre-flight validation pass:

- All OB-Xd file paths still appear, in `source-map.yaml`, exactly where the extraction subagents need them.
- The OB-Xd-tuned token budgets in Section 2.2 are based on OB-Xd line counts; they remain the defaults. The `overrides:` block is unused for OB-Xd.
- The OB-Xd-specific schemas in Section 2.3 (saw/pulse/triangle/composite) match OB-Xd's actual code structure.
- The OB-Xd-specific conflict rule examples in Section 3.2 rule 8 use real OB-Xd constants (`+0.0006`, `B_OVERSAMPLING=64`).
- No subagent prompt is generic — each one names its target files explicitly.
- The pre-flight validation pass (filesystem existence + required-stage table) runs once at startup before any subagent. Cost is one orchestrator-side pass over a small file list.

The generalization layer is a *config swap*, not an abstraction tax. The OB-Xd run is identical whether or not you ever plan to re-target.

---

## Next Steps

After this design is approved by the user, the brainstorming skill hands off to **writing-plans**, which will turn this methodology into a detailed implementation plan: actual subagent prompts, orchestration logic, the 13 artifact schemas (only Schema #3 is sketched here), the orchestrator's pre-flight validation code, and the run script that the user invokes.

The execution of the methodology — i.e., the actual OB-Xd code reading and spec generation — happens during the implementation phase that follows writing-plans, not during writing-plans itself.
