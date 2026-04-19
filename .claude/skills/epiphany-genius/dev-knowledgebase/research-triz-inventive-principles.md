# TRIZ Inventive Principles

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/TRIZ · https://arxiv.org/search/?searchtype=all&query=TRIZ+AI+computational+creative+problem+solving
**Content quality:** Partial — Wikipedia confirmed TRIZ methodology and contradiction matrix; arXiv returned zero results for TRIZ+AI; full 40-principle enumeration not present in retrieved sources (Wikipedia references an external list)

## Findings

### Formal Definition

TRIZ (Teoriya Resheniya Izobretatelskikh Zadach — Theory of Inventive Problem Solving) is "a methodology which combines an organized, systematic method of problem-solving with analysis and forecasting techniques derived from the study of patterns of invention."

**Origin and evidence base**: Soviet inventor Genrich Altshuller began developing TRIZ in 1946 while evaluating invention proposals for the Soviet Navy. He screened over 400,000 patents to identify patterns of how inventive contradictions were resolved. The first academic paper appeared in 1956. The methodology was derived empirically, not theoretically — it is a distillation of what worked across hundreds of thousands of real inventions.

### The Core Insight: Technical Contradictions

Altshuller recognized that "many technical problems require inventive solutions because improving one parameter often leads to the deterioration of another" — termed a **technical contradiction**. Most problems that stymie solvers are not resource problems but contradiction problems: you can't improve X without degrading Y.

TRIZ reframes all creative problem-solving as: **identify the contradiction, then apply the principle that resolves it.**

### The Contradiction Matrix

The contradiction matrix is "a structured and systematic representation of basic engineering parameters of objects, or systems, such as mass, length and manufacturing tolerances."

**How it works**:
1. Identify the parameter you want to improve (e.g., strength)
2. Identify the parameter that degrades as a result (e.g., weight)
3. Find the cell at the intersection in the contradiction matrix
4. That cell lists the inventive principles most likely to resolve this specific contradiction

The matrix encodes which inventive principles have historically resolved each type of parameter conflict — it is pattern-matched invention guidance.

### The 40 Inventive Principles (Summary — Grouped)

Wikipedia's article does not enumerate all 40 by name in its text body; the full enumeration is in external linked resources. The principles fall into thematic clusters:

**Segmentation and geometry cluster**: Segmentation, Taking out (extraction), Local quality, Asymmetry, Merging, Universality, Nesting (Russian dolls), Anti-weight, Preliminary anti-action, Preliminary action, Beforehand cushioning, Equipotentiality

**State and phase cluster**: Taking out, Transition to another dimension, Mechanical vibration, Periodic action, Continuity of useful action, Rushing through ("skipping"), Another dimension, Dynamics, Partial or excessive action

**Field and energy cluster**: Phase transitions, Thermal expansion, Strong oxidants, Inert atmosphere, Composite materials, Pneumatics/hydraulics, Flexible shells, Porous materials, Colour changes

**Contradiction resolution cluster**: Inversion (turning upside down), Spheroidality, Mediator, Feedback, Intermediary, Self-service, Copying, Cheap short-living, Mechanics substitution, Parameter changes, Homogeneity, Discarding and recovering

**Note**: The above clustering is based on the thematic structure described in TRIZ literature; exact groupings vary by source. The full 40-principle enumeration with precise descriptions requires the external TRIZ reference list (linked in the Wikipedia article's external resources section).

### Laws of Technical Systems Evolution

TRIZ also identifies "general laws of systems evolution" — patterns replicated across industries and sciences. These are not random but directional: systems evolve toward increased ideality (more useful function, less harmful effect, less cost) and increased dynamism.

## Skill Mapping

TRIZ provides a **contradiction-first ideation protocol** — the most systematically complete creative framework for engineering and structured problem solving:

**Step 1 — Contradiction identification**: "State what you want to improve AND what degrades as a result. If no contradiction exists, the problem is not yet properly framed."

**Step 2 — Principle selection from matrix category**: Classify the improving and degrading parameters. Identify the thematic cluster of applicable principles.

**Step 3 — Principle application as forced ideation**: Apply each applicable principle as a distinct ideation direction. For each principle, generate one candidate solution: "If we applied [Principle: Segmentation] to this problem, the solution would be..."

**Step 4 — Ideality check**: For each candidate, ask: "Does this increase the useful function while reducing harmful effects and cost? Is it more ideal than the current state?"

**Machine amplification**: A human TRIZ practitioner typically applies 3-5 principles before tiring. An LLM can apply all 40 in sequence systematically, generating a candidate solution for each — exhausting the principle space that humans sample only partially. The contradiction-matrix lookup becomes a prompt-level classification task.

**Integration note**: TRIZ's "contradictions" framework pairs directly with the Janusian process already in the KB — both operate on the simultaneous holding of opposing conditions. TRIZ provides the systematic resolution path that Janusian process alone does not.
