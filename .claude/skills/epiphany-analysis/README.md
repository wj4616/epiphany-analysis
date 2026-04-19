# epiphany-analysis

> You have a document. You have a deep analysis of that document. How do you
> actually *use* the analysis to make the document better — without losing
> anything that was already good?

`epiphany-analysis` is a Claude Code skill that takes two inputs — your original
document (Node A) and an analysis of it (Node B) — and produces a fresh,
enhanced version of Node A that integrates every improvement the analysis makes
possible.

It doesn't just paste analysis findings into your document. It runs a
six-stage pipeline that decomposes both inputs, cross-pollinates their
insights, filters for realistic improvements, engineers concrete solutions
with two independent drafts per improvement, and integrates them into your
document — all verified by a self-audit that checks no original content was
lost and no new defects were introduced.

**Two documents in, one better document out.**

**Version:** 2.0.0 · **Status:** Stable · **Runtime:** Claude Code skill (self-contained, inline execution)

---

## How it works — the big picture

```
  Node A (your document)     Node B (analysis of A)
        │                          │
        ▼                          ▼
  ┌──────────┐              ┌──────────┐
  │  m-intake │              │  m-intake │
  │  Load +   │              │  Load +   │
  │  detect   │              │  detect   │
  └────┬─────┘              └────┬─────┘
       │                         │
       ▼                         ▼
  ┌──────────┐              ┌──────────┐
  │  A1: Full │              │  B1: Full │
  │  analysis │              │  analysis │
  │  of A     │              │  of B     │
  │  (6 sections)│           │  (section- │
  │           │              │  tailored) │
  └────┬─────┘              └────┬─────┘
       │                         │
       └──────────┬──────────────┘
                  ▼
           ┌──────────┐
           │  C: Idea  │   ← 3 passes with different perspectives
           │  pool     │   ← Skeptical, Contrarian, Empathy
           │  (≤50)    │   ← Deduplicated, tagged by source
           └────┬─────┘
                ▼
           ┌──────────┐
           │  D: Filter │   ← Accept/reject with utility rating
           │  accepted  │   ← Over-engineered ideas removed
           │  ideas     │   ← Merge consolidation
           └────┬─────┘
                ▼
           ┌──────────┐
           │  E: Solutions│  ← 2 independent drafts per idea
           │  store     │  ← Compare, pick best
           │            │  ← Failed ideas dropped (not retried)
           └────┬─────┘
                ▼
           ┌──────────┐
           │  m-integrate│  ← Apply solutions to Node A
           │  + self-   │  ← Run R1–R6 audit
           │  audit    │  ← enhanced.md on PASS
           └──────────┘
                │
                ▼
          enhanced.md
     (fresh copy, original untouched)
```

---

## Installation

```bash
git clone https://github.com/wj4616/epiphany-analysis.git ~/.claude/skills/epiphany-analysis
```

The `/epiphany-analysis` slash command will be available in
[Claude Code](https://github.com/anthropics/claude-code) on the next session
start. No other dependencies — the skill is self-contained.

---

## Quick start

```bash
# Primary use — two file paths
/epiphany-analysis path/to/document.md path/to/analysis.md

# Provide Node A inline, ask for Node B path
/epiphany-analysis path/to/analysis.md

# Provide neither path — the skill asks for both
/epiphany-analysis

# Resume an interrupted session
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc

# Resume from a specific module
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc --start-at=m-filter

# Stop after a specific module (debugging / inspection)
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc --stop-at=m-ideate
```

Every run produces a session folder under
`~/docs/epiphany/analysis/YYYYMMDD-topic-slug/` containing every stage's output
and the final enhanced document.

---

## The six stages — what each one does

### m-intake — Load and detect

**What it does:** Reads both input files. Copies them verbatim to the session
directory (byte-for-byte — SHA-256 hashes are recorded). Detects what kind of
analysis Node B is.

**Genius detection:** If Node B is an `epiphany-genius` report (has 7+ of the 10
canonical section headers like "Headline Insight", "Theory Collisions", etc.),
the pipeline runs in **genius-current** mode — each section of the analysis
gets specialized extraction. If 5–6 headers match, it's **genius-drift** —
matched sections get specialized extraction, unmatched sections get a generic
scan. Fewer than 5: **generic-fallback** — one flat scan of the whole
document.

**Hard gate HG-1:** The skill never overwrites your original file. If the
enhanced output path would equal the input path, the skill aborts before any
work.

### m-analyze — Full analyses of both inputs

**What it does:** Produces two deep analysis documents.

**Node A1** (analysis of your document) has six mandatory sections:
1. **Intent** — what the document is trying to accomplish
2. **Structure** — how it's organized
3. **Claims** — every explicit assertion, one per bullet
4. **Constraints** — every MUST, MUST-NOT, rule, or invariant (this is the
   no-regression reference set — used by the self-audit to verify nothing was
   lost)
5. **Gaps** — missing information, unstated assumptions, incomplete areas
6. **Technical details** — domain-specific specifics

**Node B1** (analysis of the analysis) uses the Section-Tailoring Map when
Node B is a genius report. Each of the 10 canonical sections gets specialized
extraction:

| Section | How it's processed |
|---|---|
| Headline Insight | **Processed first.** The primary conclusion shapes all downstream findings. |
| Theory Collisions | Both sides of each collision extracted separately — never pre-resolved. |
| Discovery vs. Proof | Gaps between discovered claims and proof depth flagged. Unsupported claims become open questions. |
| Independence-Verified Bridges | Cross-domain mappings that expose A-level gaps. Bridges whose "disanalogy limit" invalidates transfer are skipped. |
| Alternative Hypotheses | Only the best-fit hypothesis extracted. Others discarded to prevent idea-pool dilution. |
| Density-Checked Falsification | Counter-examples, failure classes, theoretical objections mapped to A-level mitigations. |
| Scope Limits | "Breaks at" boundaries become A-level guardrails (not scope expansions). |
| Coherence Signals | STRONG signals (3+ independent stages) extracted. MODERATE/WEAK skipped. |
| Generalization Checks | "Breaks at" boundaries in scope become guardrails. Asymptotic breaks skipped. |
| Open Questions | HIGH priority probes only. LOW skipped. |

For generic-fallback Node B, a single flat scan produces a findings list.

### m-ideate — Cross-cutting idea generation

**What it does:** Combines A1 and B1 findings into an unfiltered idea pool
(Node C). Runs up to 3 passes with different perspectives:

| Pass | Perspective | Goal |
|---|---|---|
| 1 | **Skeptical Reviewer** | Find every issue, gap, or unsupported claim |
| 2 | **Contrarian First-Principles Designer** | Propose non-obvious structural alternatives |
| 3 | **User-Empathy Advocate** | Find usability and maintainability gaps |

**Termination:** Stops at 3 passes, 50 ideas, or a pass that produces zero
new unique ideas — whichever comes first. Each idea is tagged with
`source_section` and `target_section` for traceability.

**Why three perspectives?** A single perspective produces a single class of
ideas. The skeptical reviewer finds problems; the contrarian finds
alternatives; the empathy advocate finds friction points. Running all three
produces a wider idea pool with less perspective bias.

### m-filter — Accept or reject each idea

**What it does:** Evaluates every idea against the core judgment criterion:

> *Best product features that are usable for humans, must be realistic to
> implement without overcomplexity or regression.*

Each idea gets one of three verdicts:
- **ACCEPT** — high-utility, realistic, no regression risk
- **ACCEPT-CONDITIONAL** — good idea, but needs a condition (e.g., "must not
  add excessive validation overhead")
- **REJECT** — over-engineered, would cause regression, or not realistic

After the verdict pass, a one-time merge consolidation finds accepted ideas
that target the same section and subsume each other — combining them prevents
redundant changes.

### m-engineer — Two drafts, pick the best

**What it does:** For every accepted idea, produces **two independently-drafted
solutions**, then compares them and keeps the best.

**Why two drafts?** A single draft has no alternative to compare against.
Two independently-drafted solutions compete on four criteria:
1. **Pro/con** — which solution has stronger advantages?
2. **Utility** — which delivers more improvement?
3. **Over-engineering bounded** — which is simpler?
4. **Info-preservation** — which loses less of the original content?

If both drafts fail verification, the idea is **dropped** — there's no third
draft or retry. This prevents infinite loops and keeps the pipeline moving.

Each solution specifies exactly where in Node A it applies using one of four
locator types: heading path, line range, anchor (text before/after), or
verbatim block. Natural-language locators are forbidden — they're too
ambiguous for reliable integration.

### m-integrate — Apply solutions and self-audit

**What it does:** Applies all Node E solutions to Node A, then runs a six-rule
self-audit.

**Integration order:** Solutions traced to the Headline Insight section are
applied first (they're the highest-leverage changes). Remaining solutions
follow in stable order.

**Conflict resolution:** If two solutions target overlapping regions, the
pipeline attempts to compose them. If composition fails, the higher-utility
solution wins.

**Self-audit (R1–R6):**

| Rule | What it checks | Why it matters |
|---|---|---|
| R1 | Both analyses (A1, B1) are complete | Incomplete analysis means missed improvements |
| R2 | Every solution appears at its target location | A solution that wasn't applied is a lost improvement |
| R3 | No Node A content removed without a matching replacement | Your original content should never disappear silently |
| R4 | Every constraint from A1 still present in enhanced output | The enhancement shouldn't break your rules |
| R5 | No newly-introduced defects (unclosed tags, TODO tokens, etc.) | The enhancement shouldn't introduce garbage |
| R6 | Node A source file unchanged (hash verification) | The original file must never be modified |

**FAIL handling:** First FAIL triggers one refinement cycle targeting only
the failed rules. Second FAIL produces `enhanced-partial.md` +
`open-questions.md` instead of `enhanced.md` — you get what the pipeline could
produce, plus a list of unresolved issues.

---

## Genius detection — how Node B type affects the pipeline

The skill automatically detects what kind of analysis document Node B is:

| Detection | Count of canonical headers | Behavior |
|---|---|---|
| **genius-current** | ≥7 | Full 10-section Section-Tailoring Map applied to Node B. Each section gets specialized extraction with its own strategy. |
| **genius-drift** | 5–6 | Matched sections get specialized extraction. Unmatched sections get a generic scan. |
| **generic-fallback** | <5 | Skip the Map entirely. One flat scan of the whole document. |

**Synonym tolerance:** The detection matches renamed sections (e.g., "Primary
Conclusion" matches "Headline Insight"), extra sections, missing sections,
and reordered sections. It's the *presence* of the sections that matters, not
their exact names or order.

**Why this matters:** An `epiphany-genius` report has a specific structure with
specific types of findings in each section. The Section-Tailoring Map knows
how to extract improvements from each section type — e.g., Theory Collisions
are extracted as unresolved pairs (not pre-resolved), while Alternative
Hypotheses only extract the best-fit (not all five). A generic analysis
document doesn't have this structure, so a flat scan is the right approach.

---

## Hard gates — what the pipeline never does

Five hard gates are enforced before any module runs. Violating any gate halts
the skill:

| Gate | Rule | Why |
|---|---|---|
| HG-1 | Output path ≠ Node A source path | Never overwrite your original document |
| HG-2 | No external research, no user questions during runtime | The pipeline is a pure function of (A, B) — deterministic and reproducible |
| HG-3 | Two independent drafts per accepted idea | No single-draft solutions — comparison catches blind spots |
| HG-4 | All outputs in session directory only | Never write to your document's directory |
| HG-5 | Every loop has a cap | No unbounded while-true constructs — every iteration has a termination condition |

---

## Session output

Every run creates a session folder with complete artifacts:

```
~/docs/epiphany/analysis/YYYYMMDD-topic-slug/
├── node-a.md              Your original document (verbatim copy)
├── node-b.md              The analysis document (verbatim copy)
├── node-a1-analysis.md    Full 6-section analysis of Node A
├── node-b1-analysis.md    Section-tailored analysis of Node B
├── node-c-ideas.md         Unfiltered idea pool (up to 50 ideas)
├── node-d-accepted.md     Accepted ideas with utility ratings
├── node-e-solutions.md    Engineered solutions (2 drafts each)
├── enhanced.md            Fresh enhanced Node A (on PASS)
│   OR enhanced-partial.md + open-questions.md (on double-FAIL)
├── self-audit.md          R1–R6 audit verdict (always written)
└── session.json           Structured run-state
```

---

## Replay and partial runs

If a session is interrupted, you can resume it:

```bash
# Resume from the first module with missing output
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc

# Resume from a specific module
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc --start-at=m-filter

# Stop after a specific module (for inspection)
/epiphany-analysis --session-dir=~/docs/epiphany/analysis/20260415-my-doc --stop-at=m-ideate
```

Without `--session-dir`, the skill always creates a fresh session. Partial-run
flags (`--start-at`, `--stop-at`) require `--session-dir`.

---

## Termination caps

Every loop has an explicit cap. No unbounded iteration:

| Boundary | Cap |
|---|---|
| Ideation passes | ≤3 |
| Total ideas in Node C | ≤50 |
| No-new-ideas halt | Any pass yielding zero new unique ideas stops the loop |
| Node D evaluation | Exactly 1 pass (no re-evaluation) |
| Node E drafts per idea | Exactly 2 (no third draft) |
| Integration refinement | ≤1 cycle on FAIL, then abort |
| Per-module hard halt | 10 minutes |
| Global hard halt | 20 minutes |

---

## Typical use with epiphany-genius

The most common workflow pairs this skill with
[epiphany-genius](https://github.com/wj4616/epiphany-genius):

```bash
# 1. Run genius analysis on your document
/epiphany-genius my-document.md
# → produces ~/docs/epiphany/genius/20260415-my-document/report.md

# 2. Feed both to epiphany-analysis
/epiphany-analysis my-document.md ~/docs/epiphany/genius/20260415-my-document/report.md
# → produces ~/docs/epiphany/analysis/20260415-my-document/enhanced.md

# 3. Your original is untouched; enhanced.md is the improved version
```

This pairing is powerful because `epiphany-genius` produces structured analysis
with the 10 canonical sections that `epiphany-analysis` knows how to extract
specialized improvements from. The Section-Tailoring Map ensures that each type
of finding (theory collisions, counter-examples, coherence signals, etc.) is
processed with the right extraction strategy.

---

## Architecture

```
┌──────────────────────────────────────────────────┐
│  SKILL.md (v2.0 — inline pipeline)               │
│  - Pre-Pipeline: flag parsing, session init       │
│  - Pipeline: 6 inline module protocols            │
│    M-INTAKE → M-ANALYZE → M-IDEATE →             │
│    M-FILTER → M-ENGINEER → M-INTEGRATE            │
│  - All modules execute in-context (no subagents)  │
│  - Stage files written progressively              │
│  - Enforces hard gates and termination caps       │
└──────────────────────┬───────────────────────────┘
                       │ reads/writes
                       ▼
┌──────────────────────────────────────────────────┐
│  Session directory                                │
│  - Stage files (node-a.md through node-e.md)     │
│  - session.json, self-audit.md, enhanced.md      │
└──────────────────────┬───────────────────────────┘
                       │ reference
                       ▼
┌──────────────────────────────────────────────────┐
│  modules/ (reference archive, not invoked)        │
│  - Preserved for frontmatter validation & audit   │
│                                                    │
│  templates/                                        │
│  - session.json.template — run-state schema       │
│                                                    │
│  tests/                                            │
│  - 5 test scripts + 1 Python validator            │
│  - 6 fixtures (1 pass + 5 fail scenarios)          │
└──────────────────────────────────────────────────┘
```

**v2.0 change:** All 6 module protocols now execute inline in the agent's context.
No subagent spawns. The modules/ directory is retained as a reference archive for
frontmatter validation and migration auditing. Each module still writes its stage
files progressively, so session state remains fully recoverable: if a session is
interrupted, any completed module's output is on disk and `--start-at` resumes
from the next module.

---

## File layout

```
~/.claude/skills/epiphany-analysis/
├── SKILL.md                         Inline pipeline (all 6 modules embedded)
├── README.md                        This file
├── LICENSE                           MIT
├── modules/                          Reference archive (not invoked at runtime)
│   ├── README.md                    Marks as reference-only
│   ├── m-intake.md
│   ├── m-analyze.md
│   ├── m-ideate.md
│   ├── m-filter.md
│   ├── m-engineer.md
│   └── m-integrate.md
├── templates/
│   └── session.json.template        Run-state schema
└── tests/
    ├── test-structure.sh             SKILL.md inline section structure
    ├── test-skill-registration.sh    Discoverability and frontmatter
    ├── test-runtime-invariants.sh    R1–R6 audit against 6 fixtures
    ├── test-partial-run.sh           --session-dir / --start-at / --stop-at
    ├── validate-frontmatter.py       YAML frontmatter validation (reference files)
    └── fixtures/
        ├── pass/                     Golden path (all R1–R6 should PASS)
        ├── fail-r1-analysis-incomplete/
        ├── fail-r2-bad-target/
        ├── fail-r4-constraint-dropped/
        ├── fail-r5-malformed/
        └── fail-r6-mtime-mismatch/
```

---

## Testing

```bash
# Structure tests
bash ~/.claude/skills/epiphany-analysis/tests/test-structure.sh

# Runtime invariant tests (R1–R6 audit against all 6 fixtures)
bash ~/.claude/skills/epiphany-analysis/tests/test-runtime-invariants.sh

# Partial-run flag tests
bash ~/.claude/skills/epiphany-analysis/tests/test-partial-run.sh

# Frontmatter validation
python3 ~/.claude/skills/epiphany-analysis/tests/validate-frontmatter.py
```

Each test fixture contains a complete set of 10 stage files simulating a
different outcome — the golden path (all audit rules PASS) and five failure
scenarios (one audit rule fails per fixture). The runtime-invariants test
verifies that the audit catches the right failures and passes the right
successes.

---

## Anti-patterns — what the pipeline never does

1. **Never overwrites the original** — enhanced.md is always a fresh copy
2. **Never asks questions during runtime** — once m-intake starts, behavior is
   a pure function of (A, B)
3. **Never produces a single-draft solution** — every Node E entry is the
   output of a compare over exactly 2 drafts
4. **Never writes to your document's directory** — all outputs live in the
   session directory
5. **Never runs an unbounded loop** — every iteration has a termination cap
6. **Never over-engineers** — the filter rejects anything that would cause
   regression, over-complexity, or information loss
7. **Never pre-resolves theory collisions** — both sides are extracted
   separately for independent ideation
8. **Never processes other sections before Headline Insight** — the headline
   anchors all downstream work
9. **Never treats Alternative Hypotheses as multiple competing ideas** — only
   the best-fit hypothesis is used
10. **Never guesses section lists at runtime** — if the genius detection can't
    match headers, it uses the actual document structure

---

## License

MIT — see [LICENSE](LICENSE).