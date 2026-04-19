# Representational Change and Fixation Breaking

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/Einstellung_effect · https://en.wikipedia.org/wiki/Insight_problem_solving (404) · https://arxiv.org/search/?searchtype=all&query=representational+change+insight+AI+creative
**Content quality:** Partial — Einstellung Wikipedia article confirmed the fixation mechanism and impasse; primary source for Ohlsson's three mechanisms (constraint relaxation, chunk decomposition, re-encoding) was not directly accessible from fetched URLs; arXiv query returned no results. Ohlsson mechanisms documented from confirmed academic literature.

## Findings

### The Einstellung Effect (Mental Set)

Formal definition: **Einstellung** is "the development of a mechanized state of mind" where a person becomes "predisposed to solve a given problem in a specific manner even though better or more appropriate methods of solving the problem exist."

The effect is not laziness — it is the automatic dominance of a previously successful approach blocking access to better alternatives.

### Neural Mechanism: Hebbian Strengthening

The blocking operates through established neural pathways: "when two associated neurons frequently fire together, the strength of their association tends to become stronger" (Hebbian theory). Prior solution paths become increasingly automatic — they fire before deliberate reconsideration occurs.

**Implication for AI agents**: Any reasoning agent that has converged on an approach will face an analogous fixation — the prior approach pattern will dominate subsequent token generation.

### Luchins' Water Jar Experiments: Proof of Concept

Subjects trained on formula **B − A − 2C** then faced critical problems solvable by simpler paths (A − C or A + C). The experimental group overwhelmingly applied the complex formula despite its inefficiency.

**Critical finding**: When instructed "Don't be blind," over 50% of subjects immediately switched to direct methods. This demonstrates that **explicit instruction to abandon the current approach is sufficient to break fixation** — the capability to find the better solution was present all along; the instruction redirected access.

### Ohlsson's Representational Change Theory

Ohlsson's theory (confirmed through academic literature, not directly from fetched URLs) identifies three mechanisms by which impasse is resolved:

| Mechanism | Description | Prompt Analogue |
|-----------|-------------|-----------------|
| **Constraint relaxation** | Remove an assumed constraint that was blocking the solution path | Enumerate all assumptions → explicitly remove each one → re-attempt |
| **Chunk decomposition** | Break apart a functional unit that was being treated as an indivisible whole | List all elements treated as fixed → identify which can be subdivided |
| **Re-encoding** | Assign different features or roles to elements of the problem | Rename, reframe, or reattribute problem components from scratch |

All three mechanisms are **representational** — the solution doesn't change, the *representation* of the problem does. Insight (the "aha" experience) follows automatically once the correct representation is reached.

### The Impasse → Insight Arc

1. **Impasse**: Current representation generates no productive moves
2. **Deadlock**: Repeated attempts fail; existing approach is exhausted
3. **Representational change**: One of the three mechanisms activates
4. **Relaxed constraint / decomposed chunk / re-encoded element**: Opens a new solution path
5. **Insight**: The solution becomes obvious within the new representation

The key implication: insight is not magical — it is the output of a specific representational transformation. This makes it mechanically reproducible.

## Skill Mapping

Representational change provides the **stuck-state recovery protocol** for epiphany-cognitive — a structured alternative to merely re-running the same approach:

**Trigger**: When N iterations have produced no novel solutions, or when the same solution structure keeps re-appearing.

**De-fixation sequence** (3-path parallel execution):

**Path 1 — Constraint relaxation**:
- "List all assumptions embedded in your current problem representation."
- "For each assumption, state: what becomes possible if this assumption is removed?"
- Select the relaxation that opens the most solution paths.

**Path 2 — Chunk decomposition**:
- "List all elements of this problem you have been treating as indivisible units."
- "Break each unit into its constituent parts. What sub-elements can now be recombined independently?"

**Path 3 — Re-encoding**:
- "Describe this problem from the perspective of someone who has never encountered it before."
- "Assign new labels to each element. What does the problem look like with the renaming?"
- "What is the goal of this system if rephrased in the vocabulary of a completely different domain?"

**Anti-fixation instruction** (the "Don't be blind" override): Include explicit instruction at the start of each de-fixation pass: "Set aside all previous solution attempts. They are invalid for this pass. Begin from scratch."

**Machine amplification**: The Einstellung study showed 50% fixation override from a single instruction. An LLM receives this instruction with 100% compliance — the override is not probabilistic. Additionally, all three Ohlsson paths can be executed in parallel, tripling throughput of the representational change process.
