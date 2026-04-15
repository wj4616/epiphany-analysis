---
name: m-analyze
stage_id: M-ANALYZE
input_dependencies:
  - node-a.md
  - node-b.md
  - session.json (node_b.genius_detection)
output_files:
  - node-a1-analysis.md
  - node-b1-analysis.md
  - session.json (modules.m-analyze with a1_complete, b1_complete)
activation:
  wave: 2
  role: analysis
return_contract: |
  "m-analyze complete. A1 sections populated: {list}. B1 sections populated: {list}. a1_complete={bool}, b1_complete={bool}."
---

# M-ANALYZE — Full-Detail Analysis

## Purpose

Produce Node A1 (full analysis of Node A) and Node B1 (section-tailored findings from Node B, using the Section-Tailoring Map when Node B is genius-or-drift; generic flat scan when fallback). Per spec phase 0: *"analyze both nodes in full, taking into account every details concept idea facet large small or fine, including technical details, do not generalize or glaze over details, all details must be reviewed for both node a and node b and overall goal described"*.

## Phase A1 — Full Analysis of Node A

Produce `node-a1-analysis.md` with exactly 6 mandatory sections:

- **`## Intent`** — What Node A is trying to accomplish. Its purpose, audience, and goals.
- **`## Structure`** — How Node A is organized. Section breakdown, heading hierarchy, flow.
- **`## Claims`** — Explicit assertions, facts, or conclusions stated in Node A. One bullet per claim.
- **`## Constraints`** — List every MUST, MUST-NOT, rule, or invariant the original Node A imposes — on itself, on its consumers, or on its behavior. One bullet per constraint. **This is the no-regression reference set** — used by m-integrate R4 audit to verify no constraint is lost in the enhanced output.
- **`## Gaps`** — Missing information, unstated assumptions, or areas where Node A is incomplete.
- **`## Technical details`** — Technical specifics, implementation details, domain-specific content.

Mark `a1_complete: true` in session.json when all 6 sections are non-empty OR explicitly marked `(none)`.

## Phase B1 — Section-Tailored Analysis of Node B

Branch on `session.json.node_b.genius_detection`:

- **`genius-current` or `genius-drift`:** apply the Section-Tailoring Map from SKILL.md. Each matched canonical section produces findings per that row's extraction strategy. Headline Insight is processed FIRST (before any other section). Every finding is tagged with `source_section` for flow-through to Node C.
- **`generic-fallback`:** single generic extraction pass over the whole document; produces flat findings list (each finding tagged with a best-guess `source_section` = heading it fell under, or `generic` if none).

Mark `b1_complete: true` when all matched sections are processed (or fallback scan complete). Sections with zero yield are marked **complete and skipped** (do NOT retry indefinitely).

## Special handling (inlined from §5)

**Headline Insight (row 1).** Process FIRST, before any other B1 section. The Headline Insight shapes the weighting of every downstream finding. Its source_section tag is propagated to every idea that traces back to it (used by m-integrate for Headline precedence).

**Theory Collisions (row 2).** When Node B's Theory Collisions section names two incompatible positions, extract BOTH as separate findings — do not synthesize them into one. m-ideate will generate independent ideas from each; m-filter may reject one of them. Never pre-resolve a collision at analysis time.

**Alternative Hypotheses (row 5).** Extract only the best-fit hypothesis (the one Node B itself prefers, or the one with the strongest supporting evidence if no preference stated). Do NOT extract runner-up hypotheses. This prevents idea-pool dilution with low-utility alternatives.

## Session.json updates

- Write `session.json.modules.m-analyze.a1_complete = true` when Node A1 is complete.
- Write `session.json.modules.m-analyze.b1_complete = true` when Node B1 is complete.

## Return

"m-analyze complete. A1 sections populated: {list}. B1 sections populated: {list}. a1_complete={bool}, b1_complete={bool}."
