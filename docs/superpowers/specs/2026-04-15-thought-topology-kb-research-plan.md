# Thought Topology KB — Deep Research Plan

**Source prompt:** `docs/epiphany/prompts/15-04-deep-research-pro-thought-topology-kb-plan.md`
**Date:** 2026-04-15
**Status:** Plan (Sub-task A)

---

## Invocation 1: Chain of Thought

- **Topic:** Chain of Thought reasoning in LLM systems — prompting architecture, algorithmic structure, and implementation patterns
- **Queries:**
    1. "chain of thought prompting LLM implementation python step by step"
    2. "Wei et al 2022 chain of thought prompting elicits reasoning large language models"
    3. "chain of thought zero-shot vs few-shot prompting examples"
    4. "chain of thought reasoning limitations failure modes hallucination"
    5. "chain of thought self-consistency Wang et al 2022 implementation"
- **Required sections:** Overview, Core Algorithm, Prompt Template(s), Implementation Pseudocode, Worked Example, Comparison to Other Topologies, Limitations and Failure Modes
- **Depth signals:** pseudocode shows step-by-step token generation with reasoning trace; at least 2 prompt templates (zero-shot and few-shot CoT) are complete and usable; worked example traces a multi-step arithmetic or logic problem through the full chain; failure modes include hallucinated reasoning, shallow chains, and compounding errors; structural distinction from ToT/GoT is linear sequential vs. branching

---

## Invocation 2: Tree of Thought

- **Topic:** Tree of Thought reasoning in LLM systems — branching search algorithm, orchestration code, and implementation
- **Queries:**
    1. "tree of thought prompting LLM implementation python"
    2. "Yao et al 2023 tree of thoughts deliberate problem solving"
    3. "tree of thought BFS DFS search strategy language model"
    4. "tree of thought vs chain of thought benchmark evaluation"
    5. "tree of thought orchestration code open source"
- **Required sections:** Overview, Core Algorithm, Prompt Template(s), Implementation Pseudocode, Worked Example, Comparison to Other Topologies, Limitations and Failure Modes
- **Depth signals:** pseudocode specifies branching condition, scoring step, and pruning logic; at least 1 prompt template includes evaluator instruction for node scoring; worked example shows ≥2 branches explored before selection; explicit separation of prompt-driven vs. code-driven components; failure modes include search space explosion and scoring noise

---

## Invocation 3: Graph of Thought

- **Topic:** Graph of Thought reasoning in LLM systems — graph-structured reasoning, merging paths, and implementation
- **Queries:**
    1. "graph of thought prompting LLM reasoning implementation"
    2. "Besta et al 2023 graph of thoughts solving problems with large language models"
    3. "graph of thought merging reasoning paths LLM orchestration"
    4. "graph of thought vs tree of thought vs chain of thought comparison"
    5. "graph of thought open source implementation code"
- **Required sections:** Overview, Core Algorithm, Prompt Template(s), Implementation Pseudocode, Implementation Orchestration Code, Worked Example, Comparison to Other Topologies, Limitations and Failure Modes, Source Availability Flag
- **Depth signals:** pseudocode shows graph construction, merge operations, and ranking; at least 1 prompt template includes merge/rank instruction; worked example shows ≥2 reasoning paths merged; explicit flag for sparse source availability; failure modes include merge conflicts and graph complexity explosion

---

## Invocation 4: Comparative Analysis

- **Topic:** Cross-topology comparison — chain of thought, tree of thought, graph of thought for LLM reasoning
- **Queries:**
    1. "chain of thought vs tree of thought vs graph of thought comparison benchmark"
    2. "when to use chain of thought vs tree of thought vs graph of thought LLM"
    3. "computational cost comparison reasoning topologies LLM token usage"
    4. "composing reasoning topologies chain tree graph thought hybrid"
    5. "LLM capability requirements reasoning topology selection guide"
- **Required sections:** When to Use Each Topology, Computational Overhead Comparison, Composability (can CoT/ToT/GoT be combined?), LLM Capability Requirements Per Topology
- **Depth signals:** each subsection contains a concrete decision criterion or metric; computational overhead includes token cost estimates; composability section includes at least one concrete hybrid pattern; LLM capability table maps topology to minimum model capability