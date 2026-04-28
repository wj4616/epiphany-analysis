> v1.2.0 · Scale: DEEP · Pipeline: complete at 7 of 7 planned stages · For raw state, re-invoke with --xml

**Source:** `~/prompts/vst-playbook-phase0-commission-brief-verbose.md`
**Task:** Analyze for improvement

---

## Headline Insight

The prompt is structurally sound and unusually thorough for its class, but carries a **single systematic gap with three manifestations**: requirements announced in context sections are not enforced at the output specification level. This is load-bearing — it means an AI generating the commission brief will satisfy stated goals implicitly rather than explicitly, producing output that *appears* correct while missing operational structure.

**Confidence:** HIGH

---

## Theory Collisions

**Collision 1: KB completeness vs. incremental harvesting**
`<constraints>` mandates "KB must be fully operational before development begins"; `<edge_cases>` mandates "support incremental KB harvesting when gaps are found mid-build."
Discriminating condition: A gap discovered during Phase 5 — does the system halt to fill it (incremental model) or was that gap supposed to be impossible (completeness model)? Neither branch is specified; the commission brief will inherit the ambiguity.

**Collision 2: Specificity mandate vs. scope restriction**
"Each section must contain specific, actionable content" (`<output_format>`) collides with "Do not overload the C++ reference section" (`<constraints>`).
Discriminating condition: Section 7 (C++ Reference) — there is no per-section specificity threshold. The AI resolves this by judgment, not by instruction.

**Collision 3: "Research" instruction vs. actual AI capability**
`<output_format>` Section 5 says "Research professional plan strategies and guides to create the optimal phase structure." Claude Code does not have live search in this prompt execution context.
Discriminating condition: If Section 5's output contains specific 2026 platform statistics, there is no mechanism in the prompt to distinguish grounded knowledge from hallucinated specificity.

---

## Discovery vs. Proof

| Discovered | Proved |
|---|---|
| The "stated in context, not enforced in output spec" pattern may be a systematic authoring habit in this prompt template | The dual-reader requirement in `<audience>` is absent from `<output_format>` and from all 12 `<verification>` items — confirmed by textual inspection |
| Framing KB bridges as ontological structures (direction + source vocab + target vocab + transformation rule + example pair + completeness criterion) may close the operationalization gap more effectively than expanding the domain list | "Bridge systems" is named in `<task>` and `<constraints>` but never defined operationally — the *what* is specified; the *how* is absent |
| A two-tier KB model (core KB required before development / supplemental harvestable during) may resolve Collision 1 with minimal prompt edit | The completeness contradiction exists simultaneously in `<constraints>` and `<edge_cases>` — confirmed present, resolution strategy is discovery not proof |

---

## Independence-Verified Bridges

**Bridge 1: Legal contract drafting → Dual-audience document design**
(Source domain: contract law) → (Target domain: prompt output structure)
Structural invariant: Precision-for-execution and clarity-for-oversight are incompatible at the lexical level but compatible at the structural level — layer them, do not blend them. Legal contracts separate "plain English summary" from "operative clauses."
Disanalogy limit: Legal documents have named parties with formal responsibilities; AI commission brief consumers are probabilistic (AI agents vary in behavior).

**Bridge 2: Building codes → Tiered specification model**
(Source domain: construction standards) → (Target domain: KB completeness model)
Structural invariant: Prescriptive path (exact spec) vs. performance path (achieve outcome) resolves the "how complete is complete enough" problem. Applied here: Tier 1 KB = required entries before development; Tier 2 KB = harvestable on demand mid-build.
Disanalogy limit: Building codes are validated by external inspectors; KB completeness is self-assessed by the generating AI.

**Bridge 3: Open-source software documentation → Audience routing**
(Source domain: OSS project docs) → (Target domain: commission brief section tagging)
Structural invariant: README / API reference / conceptual guide serve different audiences from one codebase via explicit routing, not implicit coverage. Each `<output_format>` section should tag its primary consumer (AI agent vs. developer) and describe what the consuming session needs from it.
Disanalogy limit: OSS docs are iteratively updated; a commission brief is generated once and consumed continuously across a months-long project lifecycle.

Independence check: Bridges 1 and 3 share an upstream in information architecture theory (both address multi-audience document design) — partial dependence noted. Bridge 2 is independently sourced from the construction/standards domain.

---

## Alternative Hypotheses

**AH1: Content quality is the gap, not structure**
The prompt's most impactful improvement may be requiring *cited* quality sources in the Sound Design and Marketing sections — not audience layering. A structurally perfect commission brief with uncited 2026 marketing claims would fail commercially regardless of format.
Discriminating observation: If the generated marketing section cites specific platforms and pricing data that are 2026-accurate, AH1 is weakened. If those sections are generic, AH1 gains support.

**AH2: The prompt works as-is for a skilled AI**
A capable AI handles the dual-audience requirement through writing style without structural enforcement. The identified gaps are refinements an expert AI compensates for implicitly.
Discriminating observation: Run the prompt with two different AI models; compare commission brief usability for a developer. If both outputs contain distinct AI-executable specification AND developer-readable rationale per section, AH2 is supported.

**AH3: Failure-mode examples outweigh structural fixes**
Adding examples of *wrong* bridge entries and *bad* phase specifications alongside the current positive examples would constrain output quality more effectively than any structural change.
Discriminating observation: Add failure-mode examples and re-run; measure whether KB bridge output becomes operationally specified.

*No alternative hypothesis was eliminated by falsification — all three require live test runs to distinguish. All remain alive.*

---

## Density-Checked Falsification

Falsification vectors tried:

- **CA1: "Prompt is already unusually thorough; gaps are minor"** — SURVIVED. CA1 is valid and not falsifiable by static analysis. The prompt IS above class average. Improvements are at the margin, not the foundation.
- **CA2: "KB bridge underspecification is intentional; leave design space open"** — PARTIALLY FALSIFIED. The prompt explicitly requires "named bridge systems with explicit domain pairs AND rationale." The word "explicit" suggests more formalization was intended, not withheld strategically.
- **CA3: "Dual-audience handled through writing style, not structure"** — SURVIVED for content quality; FAILED for verification coverage. Even if a skilled AI handles it implicitly, the `<verification>` section contains no structural check for it. An AI that handles it poorly also passes all 12 verification items. This is a verification gap regardless of execution quality.

`2 of 3 falsification vectors survived — residual uncertainty: MODERATE for severity of gaps, LOW for existence of gaps.`

---

## Scope Limits (hypotheses non fingo)

**Applies to:** Structural analysis of this prompt as an AI-agent commission brief specification — evaluation of requirement-to-output-spec coherence, audience layering, KB bridge formalization, and internal constraint consistency.

**Does not extend to:** Technical accuracy of the recommended stack (JUCE 8.0.x, VST3 SDK 3.8.x versions); commercial validity of the marketing framing; whether 2026 VST marketplace conditions are accurately characterized; quality of the two `<examples>` entries as domain content; whether the five sound design creative focus areas are correctly scoped.

**Claims refused:** That the prompt is broken or will produce unusable output — the evidence supports only "improvement opportunities exist," not "current version fails." Counterargument CA1 was not falsified.

---

## Coherence Signals

The "stated in context, not enforced in output spec" pattern appeared independently at three locations:

1. Dual-reader requirement: stated in `<audience>`, absent from `<output_format>` and `<verification>`
2. KB bridge formalization: stated as a goal in `<task>`, absent as a structural requirement in `<output_format>` Section 4
3. KB completeness criterion: stated in `<constraints>`, contradicted in `<edge_cases>` with no resolution

Three independent occurrences of the same structural pattern suggests a **systematic authoring habit** rather than isolated oversights: the prompt was built top-down (requirements first) without a final pass checking that each requirement maps to an output enforcement point. This is the single highest-leverage fix — one validation step applied to the whole document: *"Does every requirement in `<context>` and `<constraints>` have a corresponding enforcement point in `<output_format>` or `<verification>`?"*

Reported as a coherence signal — supporting evidence, not proof.

---

## Generalization Checks

The "requirements stated but not enforced at output spec" pattern generalizes to a class:
*Complex multi-section prompts where requirements are declared in setup sections and output is specified in a separate section.*

**Generalization holds for:**
- System architecture prompts (business requirements vs. technical spec)
- Multi-domain research prompts (scope in context, deliverables in output_format)
- Playbook generation prompts — this case

**Generalization breaks at:**
- Single-section prompts (nowhere to bury the gap)
- Prompts where `<output_format>` directly echoes requirements (already solved)
- Interactive multi-turn prompts where gaps are caught in review cycles

This prompt falls in scope. The fix also generalizes: add a requirement-tracing pass to any complex prompt's composition. "For each constraint in `<constraints>`, does `<output_format>` or `<verification>` explicitly check for it?"

---

## Open Questions & Next Probes

Ranked by discriminating power per unit effort:

1. **Run the prompt as-is, measure dual-reader gap empirically** (HIGH discriminating power, LOW effort). Does the output commission brief contain distinct AI-executable specification *and* developer-readable rationale per section? If yes, structural fix is unnecessary; if no, it is confirmed critical.

2. **Test KB bridge output quality without prompt change** (HIGH discriminating power, MEDIUM effort). Does Section 4 produce bridge entries with operational specifications (direction, transformation rules, example I/O pairs)? This directly tests whether the underspecification is consequential.

3. **Add Tier 1/Tier 2 KB tiering to `<constraints>` and re-run** (MEDIUM discriminating power, LOW effort — minimal edit, resolves Collision 1 directly). Does Section 4 then produce a KB readiness criterion? Smallest structural fix with highest collision-resolution value.

4. **Replace "research professional plan strategies" with a named methodology reference** (LOW discriminating power, LOW effort — removes hallucination surface area with one sentence edit).

5. **Add failure-mode examples to `<examples>` section** (MODERATE discriminating power, MEDIUM effort — directly tests AH3; may be more valuable than all structural fixes combined).

---

*Cognitive enhancement: 7 stages executed | Confidence: HIGH (load-bearing gap identification), MODERATE (severity without test runs) | 3 theory collisions surfaced | Verification: V1–V6 pass | Scope: limited to structural analysis, content accuracy not claimed*
