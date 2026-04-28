## INTENT

**Primary objective:** Design and create a new skill called `prompt-cog` that occupies a middle ground between two existing skills: `prompt-epiphany` (fast but imprecise, manual, not fully programmatic) and `epiphany-prompt` (highly structured, modular, and programmatic, but too slow/heavyweight).

**Desired end state:** A skill that:
- Is modular and at least somewhat programmatic (improvement over prompt-epiphany)
- Retains the high-quality enhancements from epiphany-prompt where possible
- Runs significantly faster than epiphany-prompt
- Is still slower than prompt-epiphany (acknowledged trade-off)

**Success criteria:**
- New `prompt-cog` skill created and functional
- Noticeable speed improvement over epiphany-prompt
- Enhancement quality preserved or only minimally degraded vs. epiphany-prompt
- Modular architecture (steps can run with separate role context, like separate agents)
- More programmatic than prompt-epiphany (steps followed exactly, not loosely)

---

## STRUCTURE

**Current organization:** The input is structured as nested bullet points with two heading levels (`##` and `#`). It introduces three skills (prompt-epiphany, epiphany-prompt, prompt-cog) and their relationships, then states the design goal. The organization is informal and exploratory.

**Present elements:**
- Problem statement: epiphany-prompt is too slow
- Context on existing skills: prompt-epiphany and epiphany-prompt with brief capability descriptions
- Design goal: best-of-both-worlds hybrid via modular prompt-epiphany

**Missing elements:**
- No `<role>` — no expert framing for the AI reading this (skill architect? system designer?)
- No `<constraints>` — what must NOT be done (e.g., don't break existing skills, don't remove key enhancement techniques)
- No `<output_format>` — what format should the brainstorming output take? (ideas list? design doc? comparison table?)
- No `<context>` — no background on what the brainstorming session should produce, who will consume it, or what decisions it informs
- No explicit scope boundary — what is in vs. out of scope for prompt-cog?
- No success definition for the brainstorming itself (not the skill — the brainstorm output)
- No audience specification for the brainstorming output

---

## CONSTRAINTS

**Explicit constraints:**
- prompt-cog MUST be faster than epiphany-prompt
- prompt-cog MUST be slower than prompt-epiphany (acknowledged trade-off, not a flaw)
- prompt-cog should retain "as many enhancements made in epiphany-prompt as possible"
- Architecture must be modular ("compartmentalized steps")
- Steps must be more programmatic than prompt-epiphany (more reliably followed)
- Should "derive from epiphany-prompt" conceptually

**Implicit constraints that should be explicit:**
- The new skill should not break or replace prompt-epiphany or epiphany-prompt (they are established and referenced as baselines)
- The brainstorming session is focused on design decisions, not implementation code
- Separate role context per step (like separate agents) is a desired architectural property — this implies the skill may leverage separate agent invocations or well-isolated context windows
- "Run smoothly" implies reliability, not just speed

**Conflicts:**
- The phrase "make prompt-epiphany modular and somewhat more programmatic" appears alongside "creating a modular system from the prompt-epiphany skill" — these could mean: (a) modify prompt-epiphany in-place, or (b) derive a new skill from prompt-epiphany's design. The input also says the new skill is `prompt-cog` — so this is likely (b), but the language is ambiguous.
- "adding enhancements where possible, derived from epiphany-prompt" vs "cut run time down" — these are in tension and the trade-off criteria are not defined (how much quality for how much speed?)

---

## TECHNIQUES

**T1 — XML semantic structuring:** NOT present. Input is raw markdown with bullet nesting. Needed — the enhanced prompt should use `<role>`, `<context>`, `<task>`, `<constraints>`, `<output_format>` elements. Impact: HIGH — AI consuming this for brainstorming needs clear section delineation.

**T2 — Prompt decomposition:** Partially present (bullet points decompose some sub-goals). Improvement needed — the task, context, and constraints are all mixed in a single flowing narrative. Impact: HIGH.

**T3 — Explicit constraint specification:** NOT present. Constraints are embedded in prose and implied. Needed — convert key constraints to DO/DO NOT format. Impact: MEDIUM-HIGH.

**T4 — Role/persona assignment:** NOT present. No expert framing. Needed — the brainstorming skill performing this analysis needs a role (e.g., expert skill architect / prompt engineering systems designer). Impact: MEDIUM.

**T5 — Output format template:** NOT present. No specification of what the brainstorming output should look like. Needed — particularly relevant because this is input to a brainstorming skill; the output format of the brainstorm affects downstream decision quality. Impact: MEDIUM.

**T6 — Structured reasoning injection:** NOT present. The design problem involves trade-off analysis (quality vs. speed, modular vs. simple). Needed — add CoT guidance to walk through comparison, trade-off, and design decision steps systematically. Impact: HIGH.

**T7 — Priority hierarchy:** NOT present. The tension between quality preservation and speed reduction has no resolution priority. Needed — specify which takes precedence when they conflict. Impact: MEDIUM.

**T8 — Boundary/edge case spec:** NOT present. Edge cases like "what if an enhancement from epiphany-prompt can't be ported without overhead?" are unspecified. Partially needed. Impact: LOW-MEDIUM.

**T9 — Few-shot exemplar injection:** NOT applicable. This is a design/brainstorming task, not a pattern-matching or formatting task. Not needed.

**T10 — Self-critique/validation:** NOT present. Useful for ensuring brainstorming outputs are self-consistent and don't propose designs that contradict the stated constraints. Partially needed. Impact: LOW.

**T11 — Context preservation anchoring:** Partially addressed by the three-skill framing, but the definitions of the three skills are too brief for a brainstorming consumer to act on. Needed — define prompt-epiphany, epiphany-prompt, and prompt-cog upfront with enough detail to anchor the brainstorm. Impact: MEDIUM.

**T12 — Audience calibration:** NOT present. The brainstorming skill receiving this prompt has no information about who will consume its output or what decisions it informs. Needed. Impact: MEDIUM.

**T13 — Escape hatch provision:** NOT present. If the brainstorming skill cannot determine a clear design direction, there's no guidance on what to output. Partially needed. Impact: LOW.

---

## WEAKNESSES

1. **Ambiguous task boundary** — It's unclear whether the brainstorm should produce: (a) a high-level design concept, (b) a list of features/techniques to port, (c) a comparison table, or (d) a full design document. Without this, the brainstorming skill will default to its own interpretation.

2. **Underspecified skill definitions** — The descriptions of prompt-epiphany and epiphany-prompt are very brief. A brainstorming skill given this prompt won't have enough context to reason about what "modular" or "programmatic" means in these systems.

3. **Tension not framed as a design space** — The quality-vs-speed trade-off is stated as a goal ("best of both worlds") but not framed as a design exploration space. Brainstorming is most effective when the design dimensions are explicit.

4. **Missing reference to specific enhancements** — Which specific enhancements from epiphany-prompt are most valuable? Which are responsible for most of the overhead? Without this, the brainstorming skill cannot prioritize.

5. **Structural ambiguity: modify vs. derive** — "make prompt-epiphany modular" vs. "creating a new skill prompt-cog" reads as two different project scopes. This could misdirect the brainstorming output toward refactoring vs. new-skill creation.

6. **No output format for brainstorming result** — The brainstorming skill is being given a design problem but no specification of what its output should contain (structured ideas? decision frameworks? ranked candidates?).

7. **"Run smoothly" is vague** — This phrase needs operationalization: does it mean reliability, latency, fewer errors, or deterministic execution order?

8. **Skill paths not provided** — `.claude/skills/prompt-epiphany` is mentioned by reference only. No path, no version, no structural details. A brainstorming skill cannot inspect or reason about the skills without this context.
