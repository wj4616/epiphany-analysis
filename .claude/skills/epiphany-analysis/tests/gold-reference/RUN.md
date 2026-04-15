# Gold-Reference Acceptance Protocol

Manual test per spec §14.1. Run this after every material change to the skill's module prompts.

## Fixture

- **Node A:** the spec document that Node B analyzes. Locate by reading the frontmatter / opening section of `report.md` — it names the analyzed document.
- **Node B:** `/home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md`
- **Gold C/D/E:** `node-c-ideas.md`, `node-d-accepted.md`, `node-e-solutions.md` in the same directory

## Protocol

1. Invoke: `/epiphany-analysis <node-a-path> <node-b-path>`
2. Wait for completion (expect ~10-15 min under LLM rate limits).
3. Diff the skill's output stage files against the gold reference.

## PASS criteria (v1-seed thresholds — recalibrate after 3 runs)

- **C-coverage:** ≥70% of 28 gold idea topics present (paraphrases count)
- **D-coverage:** ≥80% of 14 gold D entries (D01-D14) present, matched by (category, target_section, action) tuple
- **E-coverage:** ≥80% of 14 gold E entries (E01-E14) present, matched by target-string similarity ≥0.7
- **E-format:** 100% of E entries use the structured 4-kind locator (no natural-language targets)
- **Integration:** every E entry appears in `enhanced.md` at its declared target
- **No-regression:** every `## Constraints` entry from A1 is still satisfied in `enhanced.md`

## Recording results

Append each run's results to `run-log.md` (sibling of this file) with date, thresholds met/missed, and one-sentence notes on variance.
