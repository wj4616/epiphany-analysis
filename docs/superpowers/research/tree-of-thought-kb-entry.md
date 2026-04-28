# Tree of Thought (ToT)

## 1. Overview

Tree of Thought (ToT) is a reasoning framework that frames LLM problem-solving as **search over a tree of intermediate reasoning states**, rather than a single left-to-right chain. Introduced by Yao et al. (NeurIPS 2023), ToT enables an LLM to explore multiple reasoning paths, self-evaluate intermediate states, backtrack from dead ends, and select the most promising trajectory -- capabilities absent in standard Chain-of-Thought prompting.

**Critical architectural fact**: ToT is NOT a pure-prompt architecture. It requires **external orchestration code** that drives the multi-step process of generating candidates, evaluating them, pruning, and repeating. The LLM provides two distinct capabilities -- thought generation and state evaluation -- but the search algorithm (BFS, DFS, beam search) and the branching/pruning logic live in orchestration code outside the model.

**Core result**: On the Game of 24 benchmark, ToT with GPT-4 achieved **74% success** vs. 4% for standard CoT -- an 18x improvement -- because ToT can recover from early mistakes that permanently derail CoT.

**Relationship to prior methods** (Yao et al., 2023):
- IO Prompting = ToT with depth 0, breadth 1
- Chain-of-Thought = ToT with depth n, breadth 1 (single chain, no branching)
- Self-Consistency (CoT-SC) = Forest of independent chains, no intermediate evaluation or backtracking
- ToT = Full tree search with branching, evaluation, pruning, and backtracking

**Reference**: Yao, S., Yu, D., Zhao, J., Shafran, I., Griffiths, T.L., Cao, Y., & Narasimhan, K. (2023). *Tree of Thoughts: Deliberate Problem Solving with Large Language Models*. NeurIPS 2023. arXiv:2305.10601.

**Official implementation**: [princeton-nlp/tree-of-thought-llm](https://github.com/princeton-nlp/tree-of-thought-llm) (5.9k stars, MIT license)


## 2. Core Algorithm

ToT decomposes problem-solving into four components, each requiring both a prompt design and orchestration logic:

### 2.1 Thought Decomposition

The problem is broken into intermediate "thought" steps appropriate to the domain:
- **Equation tasks** (Game of 24): Each thought = one arithmetic operation (e.g., "4 + 8 = 12 (left: 4 6 12)")
- **Creative writing**: Each thought = a plan outline or a paragraph
- **Crosswords**: Each thought = a single word placement

The granularity matters: too fine and the tree explodes; too coarse and evaluation becomes unreliable.

### 2.2 Thought Generation

Two strategies, chosen based on thought-space density:

| Strategy | Method | Use When | Orchestration |
|----------|--------|----------|---------------|
| **Sample** | Generate k i.i.d. samples from a CoT-style prompt (temperature > 0) | Rich, diverse thought spaces (e.g., paragraph plans) | Call LLM k times independently, or use `n=k` parameter |
| **Propose** | Ask LLM to propose k distinct next steps in one call | Constrained, structured spaces (e.g., valid arithmetic moves) | Call LLM once, parse k numbered outputs |

### 2.3 State Evaluation

Two strategies for assessing the promise of intermediate states:

| Strategy | Method | Output | Orchestration |
|----------|--------|--------|---------------|
| **Value** | LLM rates each state independently | Categorical (sure/likely/impossible) or scalar (0-10) | Call LLM once per state, map categories to numerics |
| **Vote** | LLM compares multiple states and selects best | Ordinal ranking or single winner | Call LLM once with all states, parse selection |

### 2.4 Search Algorithm

Two primary strategies:

**BFS (Breadth-First Search)** -- for shallow, wide trees (depth 2-3):
```
For each step t = 1..T:
    1. GENERATE: For each of the b current states, generate k candidate next-thoughts
    2. EVALUATE: Score or vote on all b*k candidates
    3. SELECT: Keep the top b states by value
Return output from best final state
```

**DFS (Depth-First Search)** -- for deeper trees (depth 5-10) with backtracking:
```
DFS(state s, step t):
    If t > T: return output (terminal state)
    Generate k candidate next-thoughts from s
    Evaluate each candidate
    Sort candidates by value (descending)
    For each candidate s' in sorted order:
        If value(s') > threshold:
            result = DFS(s', t+1)
            If result is success: return result
        Else: prune s' (and all lower-ranked)
    Return FAILURE (backtrack to parent)
```

**Key DFS features absent in BFS**: heuristic threshold for pruning, explicit backtracking on dead ends, depth-first exploration that can abandon an entire subtree early.


## 3. Prompt Templates

Below are two complete, usable prompt templates. Template A is for the **propose + value** pattern (structured domains like Game of 24). Template B is for the **sample + vote** pattern (open-ended domains like creative writing). Each includes both the generation prompt and the evaluator instruction.

---

### Template A: Propose + Value (Game of 24 style)

**Thought Generation Prompt (propose strategy):**

```
Use numbers and basic arithmetic operations (+ - * /) to obtain 24.
Each step, you are only allowed to choose two of the remaining numbers
to obtain a new number.

Input: 2 8 8 14
Possible next steps:
2 + 8 = 10 (left: 8 10 14)
8 / 2 = 4 (left: 4 8 14)
14 + 2 = 16 (left: 8 8 16)
2 * 8 = 16 (left: 8 14 16)
8 - 2 = 6 (left: 6 8 14)
14 - 8 = 6 (left: 2 6 8)
14 /  2 = 7 (left: 7 8 8)
14 - 2 = 12 (left: 8 8 12)

Input: {remaining_numbers}
Possible next steps:
```

**State Evaluation Prompt (value strategy -- intermediate steps):**

```
Evaluate if the remaining numbers can reach 24.
Rate each as sure, likely, or impossible.

10 14: impossible (both numbers too big to reach 24 with + and *)
4 4 10: sure ((10 - 4) * 4 = 24)
4 4 5: likely (4 * 5 + 4 = 24 or (5 - 4) * 24 = 24 if 24 available)
5 6 6: impossible (can only get combinations like 11, 30, 36, etc.)
2 12: sure (2 * 12 = 24)
2 3 6: impossible
4 5 6: sure (4 * 6 = 24, 5 not needed, but we must use all numbers)
1 1 1 8: impossible (no combination of 1s and 8 reaches 24)
4 9 10 13: sure ((13 - 10) = 3, 9 - 3 = 6, 4 * 6 = 24)
5 5 5 9: likely ((5 + 5) + 5 + 9 = 24 but must check if all used)
2 9 10 12: sure (12 * 2 = 24, then (10 - 9) * 24 = 24)

{remaining_numbers}:
```

**State Evaluation Prompt (value strategy -- final step verification):**

```
Check if the answer correctly uses all input numbers exactly once
and equals 24. Rate as sure or impossible.

Input: 4 4 6 8
Answer: (4 + 8) * (6 - 4) = 24
sure

Input: 2 9 10 12
Answer: (12 * 2) * (10 - 9) = 24
sure

Input: 4 9 10 13
Answer: 4 * (9 - (13 - 10)) = 24
sure

Input: 1 4 8 8
Answer: (1 + 8 / 4) * 8 = 24
sure

Input: 2 * (12 - 10) = 24
impossible (not all input numbers used)

Input: {input}
Answer: {answer}
```

---

### Template B: Sample + Vote (Creative Writing style)

**Thought Generation Prompt (sample strategy -- plan generation):**

```
Write a coherent passage of 4 short paragraphs.
The end sentence of each paragraph must be:

{input_sentences}

Make a plan then write. Your output should be of the following format:

Plan:
Your plan here.

Passage:
Your passage here.
```

When generating subsequent samples, append the current state (previous plans/passages) so the model can see context.

**State Evaluation Prompt (vote strategy -- comparative):**

```
Given an instruction and several choices, decide which choice is
most promising. Analyze each choice in detail, then conclude in
the last line "The best choice is {s}", where s is the integer
id of the choice.

Instruction:
Write a coherent passage of 4 short paragraphs.
The end sentence of each paragraph must be:

{input_sentences}

Make a plan then write. Your output should be of the following format:

Plan:
Your plan here.

Passage:
Your passage here.

Choice 1:
{state_1}

Choice 2:
{state_2}

Choice 3:
{state_3}
```

The orchestrator parses the response with regex `.*best choice is .*?(\d+).*` to extract the selected choice ID.

**Scalar Scoring Prompt (value strategy -- for individual evaluation):**

```
Analyze the following passage, then at the last line conclude
"Thus the coherency score is {s}", where s is an integer from 1 to 10.

{passage}
```

---

### Template C: General-Purpose ToT with Evaluator Rubric

For domains without task-specific few-shot examples, use this evaluator-augmented template:

```
You are solving a problem with deliberate search.

Instructions:
1) Generate exactly {k} candidate next thoughts for the current state.
2) Each thought must be a single coherent step toward solving the problem.
3) After generating candidates, score each against the rubric below.
4) Output your scores, then identify the chosen next thought.

Rubric (score 0-10):
- Correctness pressure: Does it maintain invariants and constraints?
- Progress: Does it move toward a solution (not restating the problem)?
- Verifiability: Can we check this step quickly and independently?
- Risk: Does it introduce unverified assumptions? (penalize high risk)

Current state: {question}

{previous_steps}

Now generate {k} candidates and score each.
```

This template includes evaluator instructions inline, making it usable for any domain without custom prompt engineering.


## 4. Implementation Pseudocode

The following pseudocode covers both BFS and DFS variants with explicit branching, scoring, and pruning logic.

```
ALGORITHM: TreeOfThoughts_BFS

Input:
  x          -- the problem instance
  T          -- number of thought steps (tree depth)
  k          -- candidates generated per state (branching factor for generation)
  b          -- beam width (states retained per level after pruning)
  GENERATE   -- function: state -> list of k candidate thoughts
  EVALUATE   -- function: state -> numeric value

Output: best solution found

1.  states <- {empty_state}
2.  FOR t = 1 TO T:
3.      candidates <- {}
4.      FOR each state s in states:
5.          new_thoughts <- GENERATE(s)          # returns k thoughts
6.          FOR each thought t_i in new_thoughts:
7.              new_state <- s + t_i             # append thought to state
8.              candidates <- candidates + {(new_state, EVALUATE(new_state))}
9.          END FOR
10.     END FOR
11.     SORT candidates by value (descending)
12.     states <- top b candidates from sorted list    # PRUNING STEP
13. END FOR
14. RETURN state with highest value from states
```

```
ALGORITHM: TreeOfThoughts_DFS

Input:
  x              -- the problem instance
  T              -- maximum depth
  k              -- candidates per expansion
  threshold      -- minimum value to continue exploring (pruning boundary)
  GENERATE       -- function: state -> list of k candidate thoughts
  EVALUATE       -- function: state -> categorical or numeric value
  IS_TERMINAL    -- function: state -> bool (is this a complete solution?)
  IS_VALID       -- function: state -> bool (does the terminal state solve x?)

Output: solution or FAILURE

1.  FUNCTION DFS(state s, depth d):
2.      IF d > T:
3.          IF IS_VALID(s): RETURN s
4.          ELSE: RETURN FAILURE
5.      END IF
6.
7.      candidates <- GENERATE(s)                    # k candidate thoughts
8.      scored <- [(t, EVALUATE(s + t)) for t in candidates]
9.      SORT scored by value (descending)
10.
11.     FOR each (thought, value) in scored:
12.         IF value >= threshold:                    # PRUNING CONDITION
13.             result <- DFS(s + thought, d + 1)     # RECURSIVE EXPANSION
14.             IF result != FAILURE:
15.                 RETURN result                     # SUCCESS: propagate up
16.         ELSE:
17.             BREAK                                # all remaining are lower; prune subtree
18.     END FOR
19.
20.     RETURN FAILURE                               # BACKTRACK: all branches failed
21. END FUNCTION
22.
23. result <- DFS(empty_state, 1)
24. RETURN result
```

**Branching condition** (line 5-8 in BFS, line 7 in DFS): At each node, generate k candidate next-thoughts. This is where the tree branches.

**Scoring step** (line 8 in BFS, line 8 in DFS): Each candidate state is evaluated by the LLM (value prompt or vote prompt). This is the most expensive and noisy step.

**Pruning logic** (line 11-12 in BFS, line 12-17 in DFS): BFS keeps only the top-b states. DFS prunes any branch below the threshold and, because candidates are sorted, breaks out of the loop entirely once a sub-threshold candidate is hit (all remaining are guaranteed to be lower).

**Backtracking** (DFS only, line 20): When all branches from a node fail, control returns to the parent, which tries its next-best candidate. This is the mechanism that allows recovery from early mistakes -- absent in BFS.


## 5. Implementation Orchestration Code

This is the minimum Python scaffolding required to drive the multi-step ToT process. Without this orchestration layer, ToT cannot function -- prompting alone cannot manage tree state, pruning, or backtracking.

```python
"""
Tree of Thoughts orchestration scaffold.
Requires: openai (pip install openai)
"""

import re
import json
from dataclasses import dataclass, field
from typing import List, Optional, Callable
from openai import OpenAI


# ---------------------------------------------------------------------------
# Data structures
# ---------------------------------------------------------------------------

@dataclass
class TreeNode:
    """A single node in the search tree."""
    state: str          # accumulated partial solution so far
    thought: str        # the thought that led to this state (empty for root)
    value: float = 0.0  # evaluation score (0.0 = unevaluated / impossible)
    children: list = field(default_factory=list)
    depth: int = 0

    def is_terminal(self, max_depth: int) -> bool:
        return self.depth >= max_depth


# ---------------------------------------------------------------------------
# LLM interface wrapper
# ---------------------------------------------------------------------------

class LLM:
    """Thin wrapper around OpenAI chat completions."""

    def __init__(self, model: str = "gpt-4", api_key: str | None = None):
        self.client = OpenAI(api_key=api_key)
        self.model = model

    def generate(self, prompt: str, n: int = 1, temperature: float = 0.7,
                 stop: list[str] | None = None) -> list[str]:
        """Generate n i.i.d. completions from a prompt."""
        response = self.client.chat.completions.create(
            model=self.model,
            messages=[{"role": "user", "content": prompt}],
            temperature=temperature,
            max_tokens=512,
            n=n,
            stop=stop,
        )
        return [c.message.content for c in response.choices]


# ---------------------------------------------------------------------------
# Tree of Thoughts orchestrator
# ---------------------------------------------------------------------------

class TreeOfThoughts:
    """
    Orchestrates the generate-evaluate-prune loop.

    You MUST supply:
      - propose_prompt(state): returns the prompt for generating k next thoughts
      - value_prompt(state):   returns the prompt for evaluating a state
      - parse_proposals(text):  extracts candidate thoughts from LLM output
      - parse_value(text):      extracts numeric score from LLM output
    These are task-specific. The orchestration is task-agnostic.
    """

    def __init__(
        self,
        llm: LLM,
        propose_prompt: Callable[[str], str],
        value_prompt: Callable[[str], str],
        parse_proposals: Callable[[str], List[str]],
        parse_value: Callable[[str], float],
        k: int = 5,              # candidates per expansion
        b: int = 3,              # beam width (BFS) or max branches kept (DFS)
        max_depth: int = 3,      # maximum tree depth
        threshold: float = 0.5,  # minimum value to continue (DFS only)
    ):
        self.llm = llm
        self.propose_prompt = propose_prompt
        self.value_prompt = value_prompt
        self.parse_proposals = parse_proposals
        self.parse_value = parse_value
        self.k = k
        self.b = b
        self.max_depth = max_depth
        self.threshold = threshold

    # -- Thought generation (propose strategy) --------------------------------

    def generate_candidates(self, state: str) -> list[str]:
        """Generate k candidate next-thoughts from the current state."""
        prompt = self.propose_prompt(state)
        raw = self.llm.generate(prompt, n=1, temperature=0.7)[0]
        candidates = self.parse_proposals(raw)
        return candidates[: self.k]  # truncate to k

    # -- State evaluation (value strategy) -------------------------------------

    def evaluate_state(self, state: str) -> float:
        """Evaluate a state; return numeric value in [0, 1]."""
        prompt = self.value_prompt(state)
        raw = self.llm.generate(prompt, n=1, temperature=0.0)[0]
        return self.parse_value(raw)

    # -- BFS search -----------------------------------------------------------

    def solve_bfs(self, problem: str) -> Optional[str]:
        """
        BFS (beam search) variant.
        At each level: expand all current states, evaluate, keep top-b.
        """
        root = TreeNode(state=problem, depth=0)
        current_nodes = [root]

        for depth in range(1, self.max_depth + 1):
            # EXPAND: generate candidates for each current node
            all_candidates: list[TreeNode] = []
            for node in current_nodes:
                thoughts = self.generate_candidates(node.state)
                for thought in thoughts:
                    new_state = f"{node.state}\n{thought}" if node.state else thought
                    child = TreeNode(state=new_state, thought=thought, depth=depth)
                    node.children.append(child)
                    all_candidates.append(child)

            # EVALUATE: score each candidate
            for candidate in all_candidates:
                candidate.value = self.evaluate_state(candidate.state)

            # PRUNE: keep only top-b by value
            all_candidates.sort(key=lambda n: n.value, reverse=True)
            current_nodes = all_candidates[: self.b]

            if not current_nodes:
                return None  # all pruned

        # Return the best terminal state
        current_nodes.sort(key=lambda n: n.value, reverse=True)
        return current_nodes[0].state

    # -- DFS search -----------------------------------------------------------

    def solve_dfs(self, problem: str) -> Optional[str]:
        """
        DFS variant with backtracking and threshold-based pruning.
        Explores the most promising branch first; backtracks on failure.
        """
        root = TreeNode(state=problem, depth=0)

        def _dfs(node: TreeNode) -> Optional[str]:
            if node.depth >= self.max_depth:
                # Terminal: check if this is a valid solution
                return node.state  # caller should validate externally

            # GENERATE candidates
            thoughts = self.generate_candidates(node.state)
            children = []
            for thought in thoughts:
                new_state = f"{node.state}\n{thought}" if node.state else thought
                child = TreeNode(
                    state=new_state, thought=thought, depth=node.depth + 1
                )
                child.value = self.evaluate_state(child.state)
                node.children.append(child)
                children.append(child)

            # SORT by value (explore most promising first)
            children.sort(key=lambda n: n.value, reverse=True)

            # PRUNE + EXPLORE with backtracking
            for child in children:
                if child.value >= self.threshold:
                    result = _dfs(child)
                    if result is not None:
                        return result  # SUCCESS: propagate up
                else:
                    break  # sorted descending; all remaining are below threshold

            return None  # FAILURE: all branches exhausted, BACKTRACK

        return _dfs(root)


# ---------------------------------------------------------------------------
# Task-specific adapters (Game of 24 example)
# ---------------------------------------------------------------------------

PROPOSE_TEMPLATE = """Use numbers and basic arithmetic operations (+ - * /) to obtain 24.
Each step, you are only allowed to choose two of the remaining numbers to obtain a new number.

Input: 2 8 8 14
Possible next steps:
2 + 8 = 10 (left: 8 10 14)
8 / 2 = 4 (left: 4 8 14)
14 + 2 = 16 (left: 8 8 16)
2 * 8 = 16 (left: 8 14 16)
8 - 2 = 6 (left: 6 8 14)
14 - 8 = 6 (left: 2 6 8)
14 /  2 = 7 (left: 7 8 8)
14 - 2 = 12 (left: 8 8 12)

Input: {state}
Possible next steps:
"""

VALUE_TEMPLATE = """Evaluate if the remaining numbers can reach 24.
Rate as: sure, likely, or impossible.

4 4 10: sure
5 6 6: impossible
2 12: sure
4 5 6: sure
10 10 11: impossible

{state}:
"""

VALUE_MAP = {"sure": 1.0, "likely": 0.5, "impossible": 0.0}


def game24_propose_prompt(state: str) -> str:
    return PROPOSE_TEMPLATE.format(state=state)


def game24_value_prompt(state: str) -> str:
    # Extract remaining numbers from last thought line
    last_line = state.strip().split("\n")[-1]
    if "left: " in last_line:
        remaining = last_line.split("left: ")[-1].rstrip(")")
    else:
        remaining = state
    return VALUE_TEMPLATE.format(state=remaining)


def parse_game24_proposals(text: str) -> list[str]:
    """Parse numbered candidate thoughts from LLM propose output."""
    lines = text.strip().split("\n")
    proposals = []
    for line in lines:
        line = line.strip()
        # Match lines like "2 + 8 = 10 (left: 8 10 14)"
        if re.match(r".*\d+\s*[+\-*/]\s*\d+\s*=\s*\d+.*left:", line):
            proposals.append(line)
    return proposals


def parse_game24_value(text: str) -> float:
    """Parse sure/likely/impossible from value output."""
    text = text.strip().lower()
    for key, val in VALUE_MAP.items():
        if key in text:
            return val
    return 0.0  # default to impossible if unparseable


# ---------------------------------------------------------------------------
# Usage example
# ---------------------------------------------------------------------------

def solve_game24(numbers: str) -> Optional[str]:
    """
    Solve a Game of 24 puzzle.
    numbers: space-separated string like "4 8 6 4"
    """
    llm = LLM(model="gpt-4")

    tot = TreeOfThoughts(
        llm=llm,
        propose_prompt=game24_propose_prompt,
        value_prompt=game24_value_prompt,
        parse_proposals=parse_game24_proposals,
        parse_value=parse_game24_value,
        k=5,           # generate 5 candidates per step
        b=3,           # keep top-3 per level (BFS beam width)
        max_depth=3,   # 3 operations to reduce 4 numbers to 1
        threshold=0.5,  # prune "impossible" branches (DFS only)
    )

    # BFS variant (recommended for Game of 24 -- shallow tree)
    return tot.solve_bfs(numbers)


if __name__ == "__main__":
    result = solve_game24("4 8 6 4")
    print(result)
```

**What the orchestration code does that prompting alone cannot:**

1. **Maintains tree state** across LLM calls (the `TreeNode` structure with parent-child links)
2. **Drives the generate-evaluate-prune loop** iteratively across depth levels
3. **Implements pruning** (keeping top-b in BFS; threshold cutoff in DFS)
4. **Manages backtracking** in DFS by propagating FAILURE up the call stack
5. **Parses structured LLM output** (extracting candidate thoughts and numeric values from free-form text)
6. **Caches and deduplicates** evaluations (omitted above for clarity; the official implementation includes a `local_value_cache` dict)


## 6. Worked Example

**Problem**: Game of 24 with input `4 6 8 4` (make 24 using +, -, *, /).

**Configuration**: BFS with k=5 (generate 5 candidates), b=2 (keep top 2 per level), max_depth=3.

### Step 1: Generate candidates from root state "4 6 8 4"

LLM propose output (5 candidates):

| Branch | Thought | Remaining |
|--------|---------|-----------|
| A | 4 + 6 = 10 | 4 8 10 |
| B | 8 - 4 = 4 | 4 4 6 |
| C | 6 * 4 = 24 | 4 8 24 |
| D | 8 / 4 = 2 | 2 4 6 |
| E | 4 + 8 = 12 | 4 6 12 |

**Evaluate each**:

| Branch | LLM Rating | Numeric Value |
|--------|-----------|---------------|
| A | "likely" (10 is close, 4+10=14 or 10*4=40 or 10-4=6...) | 0.5 |
| B | "likely" (4,4,6 -> 4*6=24, but need to use both 4s) | 0.5 |
| C | "sure" (24 already present, just need to dispose of 4 and 8) | 1.0 |
| D | "sure" (2,4,6 -> 4*6=24, 2 is manageable) | 1.0 |
| E | "likely" (4,6,12 -> 12*2=24 if we can make 2 from 4,6) | 0.5 |

**Prune to top-2**: Keep C (1.0) and D (1.0). Discard A, B, E.

### Step 2a: Expand from branch C (state: "4 6 8 4 -> 6*4=24, left: 4 8 24")

LLM propose output (5 candidates, but only valid moves shown):

| Branch | Thought | Remaining |
|--------|---------|-----------|
| C1 | 4 + 8 = 12 | 12 24 |
| C2 | 8 - 4 = 4 | 4 24 |
| C3 | 24 * 1 = 24 | ... but no 1 available |
| C4 | 8 / 4 = 2 | 2 24 |

**Evaluate**:
- C1: "likely" (12+24=36, 24-12=12, 12*24=288... 24/12=2... hmm) -> 0.5
- C2: "sure" (4,24 -> 24 is the answer, 4 unused... need to use all numbers) -> 0.3
- C4: "sure" (2,24 -> 24 is answer, 2 unused... same issue) -> 0.3

Hmm -- none of these cleanly reach 24 using ALL four original numbers. This is a scoring artifact: the "sure" rating at step 1 was optimistic because the evaluator only saw "24 is present" without checking that all original numbers are consumed.

### Step 2b: Expand from branch D (state: "4 6 8 4 -> 8/4=2, left: 2 4 6")

LLM propose output:

| Branch | Thought | Remaining |
|--------|---------|-----------|
| D1 | 2 * 4 = 8 | 6 8 |
| D2 | 4 * 6 = 24 | 2 24 |
| D3 | 6 - 2 = 4 | 4 4 |
| D4 | 2 + 4 = 6 | 6 6 |
| D5 | 6 / 2 = 3 | 3 4 |

**Evaluate**:
- D1: "sure" (6,8 -> 6*8=48, 8-6=2... wait, we need 24 from 6 and 8... no) -> actually "impossible" -> 0.0
- D2: "likely" (24 is there but 2 unused, same problem as C) -> 0.5
- D3: "likely" (4,4 -> 4+4=8, 4*4=16, no 24) -> 0.0
- D4: "impossible" (6,6 -> 6+6=12, 6*6=36, no 24) -> 0.0
- D5: "likely" (3,4 -> 3*4=12... not 24. Or... hmm) -> 0.0

This reveals a **scoring noise problem**: the "sure" rating at step 1 was incorrect for branch D -- the path doesn't actually lead to a clean solution.

### What actually works

The correct solution for 4 6 8 4 is: **(8 - 4) * (6 - 4)** ... no, that's (8-4)*(6-4) = 4*2 = 8. Not 24.

Let's reconsider: **6 * 4 = 24** then need to absorb 4 and 8... but all numbers must be used. Try: **(8 + 4) * (6 - 4) = 12 * 2 = 24**. Yes! That uses all four numbers.

This solution would emerge from branch E (4+8=12, remaining: 4,6,12) which was **pruned at step 1**. The next step would be 6-4=2 (remaining: 2,12), then 2*12=24.

**This is the premature pruning failure mode in action**: the correct solution was on a branch rated "likely" (0.5) that was cut because only top-2 were kept. A wider beam (b=3) or DFS with threshold=0.5 would have retained this branch.

### Key observations from this trace

1. **Branching creates real alternatives** -- 5 candidates at step 1, not 1 as in CoT
2. **Evaluation is noisy** -- branch C looked "sure" but was actually a dead end
3. **Pruning is irreversible in BFS** -- once branch E is cut, it can never be recovered
4. **DFS with backtracking would handle this better** -- after exhausting C and D, it would backtrack and try E
5. **Cost is multiplicative** -- step 1 used 5 generate calls + 5 evaluate calls; step 2 used 10+10


## 7. Comparison to Other Topologies

### 7.1 Structural comparison

| Property | Chain of Thought (CoT) | Tree of Thought (ToT) | Graph of Thought (GoT) |
|----------|----------------------|----------------------|------------------------|
| **Topology** | Linear chain (path graph) | K-ary tree (branching only) | Directed graph (branching + merging) |
| **Max out-degree** | 1 | k (branching factor) | k (branching factor) |
| **Max in-degree** | 1 | **1** (each node has exactly one parent) | **>1** (nodes can have multiple parents) |
| **Merging/aggregation** | None | None | Yes -- multiple paths can converge |
| **Cycles** | No | No | Yes -- refinement loops |
| **Backtracking** | No | Yes (DFS variant) | Yes (via cycles) |
| **Intermediate evaluation** | No | Yes (value/vote) | Yes (value/vote) |

### 7.2 Concrete structural distinction

**The distinguishing property of ToT vs. CoT**: ToT allows **branching** (out-degree > 1) with **intermediate evaluation and pruning**. CoT is a degenerate tree with breadth=1 at every depth -- it commits to a single path at each step and has no mechanism to explore alternatives.

**The distinguishing property of GoT vs. ToT**: GoT allows **aggregation** (in-degree > 1). In ToT, each node has exactly one parent -- branches never merge back together. The best branch is selected, but partial solutions from different branches cannot be combined. In GoT, multiple branches can converge into a single node that synthesizes the best elements of each. Formally: CoT is a subset of ToT is a subset of GoT.

**Concrete example of the aggregation distinction**:
- **ToT on a sorting task**: Try 3 different sorting strategies in parallel branches. Pick the best one. The other two branches' work is discarded.
- **GoT on a sorting task**: Split the list into 3 sublists. Sort each independently (3 branches). Then **aggregate** (merge) the 3 sorted sublists into one final sorted list. No work is wasted.

### 7.3 Cost and quality tradeoffs

| Dimension | CoT | ToT | GoT |
|-----------|-----|-----|-----|
| **LLM calls per problem** | 1 | 10-100+ | Variable (can be fewer than ToT due to merging) |
| **Game of 24 accuracy** | ~4% | ~74% | ~78% (with aggregation of partial solutions) |
| **Cost** | Low | High (5-100x CoT) | Moderate-high (but better cost-quality ratio) |
| **Best for** | Sequential reasoning | Exploration/search problems | Synthesis/aggregation problems |
| **Failure mode** | Early mistakes compound irreversibly | Premature pruning discards correct path | Implementation complexity, graph cycles |

### 7.4 Information flow analysis (Besta et al., 2024)

For a problem of size N with cost O(N):
- **CoT (chain)**: Latency = N, Volume = N (sequential but full information)
- **ToT (k-ary tree)**: Latency = log_k(N), Volume = log_k(N) (fast but low information flow to answer)
- **GoT (double-tree graph)**: Latency = log_k(N), Volume = N (fast AND full information)

Volume measures how much of the computed information actually influences the final answer. ToT's fundamental limitation is that most branches are pruned, so most computation is wasted. GoT's aggregation preserves and combines partial results, achieving both parallelism and information completeness.


## 8. Limitations and Failure Modes

### 8.1 Search space explosion

ToT's worst-case search requires exploring b^d nodes (b = beam width, d = depth). With b=5 and d=3, that is 125 candidate evaluations -- each requiring an LLM API call. At GPT-4 pricing (~$0.03/1K tokens), a single Game of 24 problem costs approximately $0.74, vs. $0.01 for a standard CoT call.

**Mitigations**:
- Use beam search (keep top-b) rather than exhaustive BFS
- DFS with aggressive thresholding prunes early
- Semantic similarity pruning (SSDP, Kim et al. 2025) removes redundant thoughts, achieving ~90% node reduction with <5% accuracy loss
- Cache evaluations: identical (state, thought) pairs should not be re-scored
- Use cheaper models for evaluation, expensive models for generation

### 8.2 Scoring noise (the value function problem)

The most fundamental limitation: ToT uses the same LLM to generate AND evaluate thoughts. The evaluation is not ground truth -- it is the LLM's subjective judgment, which is:
- **Noisy**: The same state can receive different scores across evaluations
- **Biased**: LLMs tend to rate verbose, well-articulated states higher (verbal fluency bias) even when they are incorrect
- **Unfaithful**: Research on "pathological chain-of-thought" shows LLM reasoning traces can be plausible rationalizations unrelated to the actual answer. Scoring based on coherence rather than constraint satisfaction is actively misleading.
- **Self-consistent but wrong**: An LLM may consistently rate an incorrect path as "sure" because it follows the model's prior, not because it is correct

**Mitigations**:
- Use **deterministic verifiers** (unit tests, regex, symbolic checkers) instead of LLM scoring when possible
- Score on **constraint satisfaction** (does the partial solution violate any constraints?) rather than verbal plausibility
- Use **multi-agent validation** -- separate Reasoner and Validator agents
- Run **multiple evaluation calls** and aggregate (the vote strategy with n_evals > 1)
- For the Game of 24 specifically: verify arithmetic at each step programmatically

### 8.3 Premature pruning (the greediness trap)

Both BFS (beam search) and DFS (threshold pruning) can discard the correct path early:
- **BFS**: If the correct branch is not in the top-b at any level, it is permanently lost. This is irreversible.
- **DFS**: If the correct branch is below the threshold at any depth, the subtree is pruned and never revisited.

The worked example in Section 6 demonstrated this: the correct solution was on a "likely" (0.5) branch that was pruned when only top-2 were kept.

**Mitigations**:
- Use wider beams (b=5+ instead of b=2-3)
- Use lower thresholds in DFS (but this increases search cost)
- Use DFS with backtracking instead of BFS (allows recovery after exhausting wrong branches)
- Multi-round search with different random seeds (stochastic exploration)
- A* or MCTS variants that can revisit and re-evaluate pruned nodes

### 8.4 Model quality dependency

ToT's performance depends heavily on the base LLM's quality:
- **GPT-4 + ToT**: 74% on Game of 24
- **GPT-3.5 + ToT**: 19% on Game of 24 (same framework, weaker model)
- Models under ~8B parameters often show **negative returns** from ToT: they generate poor candidates AND evaluate them poorly, compounding errors through the search

ToT amplifies both the strengths and weaknesses of the underlying model. A weak model with ToT can perform worse than the same model with CoT.

### 8.5 Task-specific prompt engineering

ToT requires custom thought decomposition and evaluation prompts for each task domain. The propose prompt, value prompt, and thought granularity must all be designed for the specific problem. This is not a plug-and-play system -- it requires:
- Understanding what constitutes a "thought" in the domain
- Designing evaluation criteria that correlate with actual solution quality
- Tuning k (candidates), b (beam width), depth, and threshold

The general-purpose template (Template C in Section 3) reduces but does not eliminate this requirement.

### 8.6 Open-ended task evaluation failure

For creative or open-ended tasks (creative writing, brainstorming), there is no objective function to measure progress. The LLM's evaluation becomes the entire basis for pruning, which means:
- The system optimizes for whatever the evaluator prompt rewards (often surface coherence)
- There is no ground truth to validate against
- The "best" answer according to the LLM evaluator may not be the best answer by human judgment

### 8.7 Cost-benefit threshold

ToT is only justified when the problem is hard enough that CoT fails. For tasks where GPT-4 + CoT already achieves >90% accuracy (many arithmetic and commonsense benchmarks), ToT adds cost without meaningful accuracy improvement.

**Rule of thumb**: Use ToT when CoT success rate is below ~30% and the problem has natural intermediate states that can be evaluated. Otherwise, use CoT or self-consistency.

### 8.8 Summary of failure modes

| Failure Mode | Severity | Frequency | Mitigation |
|-------------|----------|-----------|------------|
| Search space explosion | High | All ToT runs | Beam search, caching, semantic pruning |
| Scoring noise / unfaithful evaluation | Critical | Common | External verifiers, constraint-based scoring |
| Premature pruning | High | When correct path is non-obvious | Wider beams, DFS with backtracking, MCTS |
| Weak model degradation | High | Models < 8B params | Don't use ToT with small models |
| Task-specific prompt design | Medium | Every new domain | Template C (general-purpose), prompt optimization |
| Open-ended evaluation failure | Medium | Creative tasks | Multi-agent validation, human-in-the-loop |
| Cost without benefit | Low | Easy tasks | Only use ToT when CoT success < 30% |


## References

- Yao, S. et al. (2023). *Tree of Thoughts: Deliberate Problem Solving with Large Language Models*. NeurIPS 2023. [arXiv:2305.10601](https://arxiv.org/abs/2305.10601)
- Official implementation: [princeton-nlp/tree-of-thought-llm](https://github.com/princeton-nlp/tree-of-thought-llm)
- Besta, M. et al. (2024). *Demystifying Chains, Trees, and Graphs of Thoughts*. [arXiv:2401.14295](https://arxiv.org/abs/2401.14295)
- Besta, M. et al. (2023). *Graph of Thoughts: Solving Elaborate Problems with Large Language Models*. AAAI 2024.
- HuggingFace tutorial: [Understanding and Implementing the Tree of Thoughts Paradigm](https://huggingface.co/blog/sadhaklal/tree-of-thoughts)
- Kim et al. (2025). Semantic Similarity-Based Dynamic Pruning (SSDP).
- Framework of Thoughts (FoT): Optimization framework achieving up to 35.4x speedup for ToT via parallelization and caching. [OpenReview](https://openreview.net/pdf?id=UZlJY3uV6P)