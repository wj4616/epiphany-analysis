---
name: m-ideate
stage_id: M-IDEATE
input_dependencies:
  - node-a1-analysis.md
  - node-b1-analysis.md
output_files:
  - node-c-ideas.md
  - session.json (modules.m-ideate: passes_run, ideas_total, stop_reason)
activation:
  wave: 3
  role: ideation
return_contract: |
  "m-ideate complete. Passes: {N}/3. Ideas produced: {total}. Stop reason: {no-new-ideas|pass-cap|idea-cap}."
---

# M-IDEATE — Cross-Cutting Ideation

## Purpose

Produce Node C — the unfiltered idea pool — by running bounded-pass cross-cutting ideation over A1 + B1 findings. Implements: *"iteratively run phase a, passing all ideas generated into storage node c"*, *"run until out of ideas completely regarding how to improve or solve issues based on the context, storing all in node C database"*.

## Bounded loop

```
passes_run = 0
ideas = []
while passes_run < 3 and len(ideas) < 50:
    new_ideas_this_pass = run_pass(passes_run + 1, seed=A1+B1, dedup_against=ideas)
    if len(new_ideas_this_pass) == 0:
        stop_reason = "no-new-ideas"
        break
    ideas.extend(new_ideas_this_pass)
    passes_run += 1
else:
    stop_reason = "pass-cap" if passes_run == 3 else "idea-cap"
```

## Persona rotation (pass independence)

Each pass opens with a **visible preamble** declaring the role and goal:

- **Pass 1 — Skeptical Reviewer.** Goal: find every issue, gap, or unsupported claim in Node A.
- **Pass 2 — Contrarian First-Principles Designer.** Goal: propose non-obvious structural restructurings.
- **Pass 3 — User-Empathy Advocate.** Goal: find usability and maintainability gaps affecting downstream readers.

The preamble is text in the pass, not a parameter. Successive passes are thus decorrelated by explicit framing.

## Progress reporting

Before each pass, emit: `[m-ideate pass N/3] seed-count=<A1+B1 findings>, ideas-so-far=<count>, elapsed=<mm:ss>`

After each pass, emit: `[m-ideate pass N complete] new-ideas=<N>, merged=<N>, stop-reason=<if halt>`

## Node C entry schema

Each idea written to `node-c-ideas.md` is a block:

```
## I0N — <short title>
**source_section:** <B1 section name or A1 bucket>
**target_section:** <where in Node A the change would apply>
**observation:** <what A1/B1 finding seeded this>
**idea:** <the proposed improvement, one paragraph>
**category:** <structural | content | clarity | constraint | other>
**effort:** <low | medium | high>
```

## Dedup rule

Two entries collide when `(source_section, normalized_observation, target_section)` matches. `normalized_observation = lowercase + collapse whitespace + strip filler words ("the","a","that","this") + first 120 chars`. On collision: keep earlier ID, append a line `merged_into: I0N` under a `## Merged` section at file end.

## Session.json updates

Write `passes_run`, `ideas_total`, and `stop_reason` to `session.json.modules.m-ideate` after completion.

## Return

"m-ideate complete. Passes: {N}/3. Ideas produced: {total}. Stop reason: {no-new-ideas|pass-cap|idea-cap}."
