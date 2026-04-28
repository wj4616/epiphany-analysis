# Thought Topology Knowledge Base

**Version:** 1.0  
**Date:** 2026-04-15  
**Source:** Deep research on chain-of-thought, tree-of-thought, and graph-of-thought reasoning architectures  
**Target audience:** AI developers with Python and LLM API experience

---

## Chain of Thought (CoT)

### 1. Overview

Chain of Thought (CoT) is the foundational LLM reasoning topology: a **linear sequence of intermediate reasoning steps** that connects an input to an output. Introduced by Wei et al. (NeurIPS 2022), CoT prompting elicits step-by-step reasoning from language models by providing exemplars that demonstrate intermediate reasoning traces, rather than asking the model to produce answers directly.

**Topology**: A path graph with max out-degree = 1 and max in-degree = 1. Each thought has exactly one predecessor and one successor. There is no branching, no backtracking, and no merging.

**Key result**: On GSM8K (math word problems), CoT prompting with PaLM 540B achieved **57% accuracy** vs. 18% for standard prompting — a 3x improvement from adding reasoning traces alone.

**Extended variant — Self-Consistency (CoT-SC)**: Wang et al. (2022) showed that sampling multiple independent CoT paths (temperature > 0) and taking the majority vote improves accuracy further — GSM8K improved from 57% to 74% with 40 samples. CoT-SC is a **forest of chains** (multiple independent paths, no intermediate evaluation or merging).

**Reference**: Wei, J. et al. (2022). *Chain-of-Thought Prompting Elicits Reasoning in Large Language Models*. NeurIPS 2022. arXiv:2201.11903

**Extended reference**: Wang, X. et al. (2022). *Self-Consistency Improves Chain of Thought Reasoning in Language Models*. arXiv:2203.11171


### 2. Core Algorithm

```
ALGORITHM: ChainOfThought

Input:
  x          -- the problem instance
  exemplars  -- list of (question, reasoning_trace, answer) triples
  model      -- LLM with chat completions API

Output: final answer with reasoning trace

1.  Construct prompt by prepending exemplars to the problem:
      prompt = ""
      FOR each (q, trace, a) in exemplars:
          prompt += "Q: {q}\nA: {trace}\nThe answer is {a}.\n\n"
      prompt += "Q: {x}\nA:"

2.  Generate completion from LLM:
      response = model.generate(prompt, temperature=0, max_tokens=512)

3.  Extract final answer from response:
      Parse the last line or the text after "The answer is"
      Return (reasoning_trace, final_answer)

ALGORITHM: SelfConsistency (CoT-SC)

Input:
  x          -- the problem instance
  exemplars  -- list of (question, reasoning_trace, answer) triples
  model      -- LLM with chat completions API
  k          -- number of independent reasoning paths to sample
  temperature -- sampling temperature (>= 0.5 for diversity)

Output: final answer (majority vote across k paths)

1.  answers = []
2.  FOR i = 1 TO k:
3.      response = model.generate(CoT_prompt(x, exemplars), temperature=temperature)
4.      answer_i = extract_answer(response)
5.      answers.append(answer_i)
6.  END FOR
7.  final_answer = majority_vote(answers)
8.  RETURN final_answer
```

**Key properties**:
- No branching: the model commits to a single reasoning path at each step
- No backtracking: errors in early steps compound irreversibly
- No intermediate evaluation: there is no checkpoint mechanism to assess partial progress
- The algorithm is entirely prompt-driven — no orchestration code is needed beyond API calls


### 3. Prompt Template(s)

**Template A: Few-Shot Chain of Thought (original Wei et al. format)**

```
Q: Roger has 5 tennis balls. He buys 2 more cans of tennis balls. Each can has 3 tennis balls. How many tennis balls does he have now?
A: Roger started with 5 balls. 2 cans of 3 tennis balls each is 6 tennis balls. 5 + 6 = 11.
The answer is 11.

Q: The cafeteria had 23 apples. If they used 20 to make lunch and bought 6 more, how many apples do they have?
A: They started with 23 apples. They used 20, so 23 - 20 = 3. Then they bought 6 more, so 3 + 6 = 9.
The answer is 9.

Q: {your_question_here}
A:
```

**Template B: Zero-Shot Chain of Thought (Kojima et al., 2022)**

```
{your_question_here}

Let's think step by step.
```

This template requires no exemplars — simply appending "Let's think step by step." to the question elicits CoT reasoning. It works best with models >= 100B parameters and is most effective on mathematical and logical reasoning tasks.

**Template C: Self-Consistency with Chain of Thought**

```python
import openai
from collections import Counter
import re

def self_consistency_query(question: str, num_samples: int = 5, temperature: float = 0.7) -> dict:
    """
    Sample multiple CoT reasoning paths and return the majority-vote answer.
    Requires openai package: pip install openai
    """
    client = openai.OpenAI()
    exemplars = """Q: Roger has 5 tennis balls. He buys 2 more cans of tennis balls. Each can has 3 tennis balls. How many tennis balls does he have now?
A: Roger started with 5 balls. 2 cans of 3 tennis balls each is 6. 5 + 6 = 11.
The answer is 11.

Q: The cafeteria had 23 apples. If they used 20 to make lunch and bought 6 more, how many apples do they have?
A: They started with 23. Used 20, so 23 - 20 = 3. Bought 6 more, so 3 + 6 = 9.
The answer is 9."""

    prompt = f"{exemplars}\n\nQ: {question}\nA:"
    answers = []
    traces = []

    for _ in range(num_samples):
        response = client.chat.completions.create(
            model="gpt-4",
            messages=[{"role": "user", "content": prompt}],
            temperature=temperature,
            max_tokens=256,
        )
        text = response.choices[0].message.content
        traces.append(text)
        # Extract answer after "The answer is"
        match = re.search(r"(?:the answer is|answer:)\s*(.+?)(?:\.|$)", text, re.IGNORECASE)
        answers.append(match.group(1).strip() if match else text.strip())

    # Majority vote
    answer_counts = Counter(answers)
    most_common, count = answer_counts.most_common(1)[0]

    return {
        "final_answer": most_common,
        "confidence": f"{count}/{num_samples}",
        "all_answers": answers,
        "all_traces": traces,
    }

# Usage
result = self_consistency_query(
    "Janet's ducks lay 16 eggs per day. She eats three for breakfast "
    "every morning and bakes muffins for her friends every day with four. "
    "She sells the remainder for $2 per egg. How much does she make every day?"
)
print(f"Answer: {result['final_answer']} (confidence: {result['confidence']})")
```


### 4. Implementation Pseudocode

```
ALGORITHM: ChainOfThought_Pure

Input:
  question    -- the problem to solve
  exemplars   -- few-shot examples with reasoning traces (may be empty for zero-shot)
  model       -- LLM API endpoint

Output: (reasoning_trace, final_answer)

1.  IF exemplars is empty:
2.      prompt = question + "\n\nLet's think step by step."
3.  ELSE:
4.      prompt = ""
5.      FOR each (q, trace, a) in exemplars:
6.          prompt += "Q: {q}\nA: {trace}\nThe answer is {a}.\n\n"
7.      prompt += "Q: {question}\nA:"
8.  END IF
9.
10. response = model.generate(prompt, temperature=0, max_tokens=512)
11. trace = extract_reasoning_trace(response)
12. answer = extract_final_answer(response)
13. RETURN (trace, answer)


ALGORITHM: SelfConsistency_CoT

Input:
  question     -- the problem to solve
  exemplars    -- few-shot examples
  model        -- LLM API endpoint
  k            -- number of independent paths (default: 5)
  temperature  -- sampling temperature (default: 0.7)

Output: (majority_answer, confidence, all_paths)

1.  prompt = build_fewshot_prompt(question, exemplars)
2.  answers = []
3.  paths = []
4.  FOR i = 1 TO k:
5.      response = model.generate(prompt, temperature=temperature, max_tokens=512)
6.      trace_i = extract_reasoning_trace(response)
7.      answer_i = extract_final_answer(response)
8.      answers.append(answer_i)
9.      paths.append(trace_i)
10. END FOR
11. majority_answer = mode(answers)  -- most frequent answer
12. confidence = count(majority_answer in answers) / k
13. RETURN (majority_answer, confidence, paths)
```

**Orchestration note**: Unlike ToT and GoT, CoT requires **no external orchestration code** beyond the LLM API call. Self-consistency adds only a simple majority-vote loop. Both are pure-prompt architectures.


### 5. Worked Example

**Problem**: "Roger has 5 tennis balls. He buys 2 more cans of tennis balls. Each can has 3 tennis balls. How many tennis balls does he have now?"

**CoT trace**:

```
Roger started with 5 tennis balls.
He bought 2 cans, each containing 3 tennis balls.
So he bought 2 × 3 = 6 tennis balls.
Now he has 5 + 6 = 11 tennis balls.
The answer is 11.
```

**Self-Consistency trace (k=5, temperature=0.7)**:

| Path | Reasoning | Answer |
|------|-----------|--------|
| 1 | Roger started with 5. Bought 2 cans of 3 = 6. Total: 5+6=11 | 11 |
| 2 | 5 tennis balls + 2×3=6 new ones = 11 | 11 |
| 3 | Started: 5. Each can: 3. Cans: 2. Total: 5+6=11 | 11 |
| 4 | He has 5 and buys 2×3=6 more. 5+6=11 | 11 |
| 5 | 5 + (2×3) = 11 | 11 |

**Majority vote**: 11 (confidence: 5/5 = 100%)

**Example where CoT fails and Self-Consistency helps**:

**Problem**: "A baker makes 3 loaves of bread each morning. If each loaf requires 2 cups of flour and she starts with 15 cups, how many cups of flour are left after making the bread?"

**Single CoT path (potential error)**:
```
3 loaves × 2 cups = 6 cups of flour used.
15 - 6 = 9 cups remaining.
Wait, she also made 2 extra loaves for the afternoon.
9 + 2 = 11. The answer is 11.
```
(Hallucinated "2 extra loaves" — no such information in the problem.)

**Self-Consistency (k=5)**:
| Path | Answer |
|------|--------|
| 1 | 9 |
| 2 | 9 |
| 3 | 11 (hallucinated extra loaves) |
| 4 | 9 |
| 5 | 9 |

**Majority vote**: 9 (confidence: 4/5 = 80%) — the hallucinated path is outvoted.


### 6. Comparison to Other Topologies

| Property | Chain of Thought (CoT) | Tree of Thought (ToT) | Graph of Thought (GoT) |
|----------|----------------------|----------------------|------------------------|
| **Topology** | Linear chain (path graph) | K-ary tree (branching only) | Directed graph (branching + merging) |
| **Max out-degree** | 1 | k (branching factor) | k (branching factor) |
| **Max in-degree** | 1 | 1 (single parent) | >1 (aggregation allowed) |
| **Backtracking** | No | Yes (DFS variant) | Yes (via cycles) |
| **Intermediate evaluation** | No | Yes (value/vote) | Yes (value/vote + merge) |
| **Orchestration code needed** | No (pure prompt) | Yes (tree management, pruning) | Yes (graph management, merging) |
| **LLM calls per problem** | 1 (or k for self-consistency) | 10-100+ | Variable |
| **Information flow to answer** | Full (all tokens influence answer) | Partial (pruned branches are discarded) | Full (merged branches preserve information) |

**Concrete structural distinction from ToT**: CoT commits to a single path at every step — out-degree = 1 at every node. If step 3 is wrong, the entire chain fails. ToT generates k alternatives at each step and evaluates them, allowing recovery from errors.

**Concrete structural distinction from GoT**: CoT has no mechanism for merging information from multiple sources. In CoT, each token can only attend to its left context — there is no way to combine two independently-derived insights. GoT explicitly supports aggregation, allowing partial solutions to be synthesized.


### 7. Limitations and Failure Modes

#### 7.1 Unfaithful reasoning (post-hoc rationalization)

Recent research (Arcuschin et al., 2025; Turpin et al., 2023) demonstrates that CoT reasoning traces **do not faithfully represent how the model arrived at its answer**. Two key patterns:

- **Implicit post-hoc rationalization**: The model constructs plausible-sounding reasoning to justify a predetermined answer, rather than allowing reasoning to determine the answer.
- **Illogical shortcuts**: Models sometimes arrive at correct answers through clearly illogical intermediate steps without acknowledging the shortcut.

Unfaithfulness rates measured across models:
| Model | Unfaithfulness Rate |
|-------|-------------------|
| GPT-4o-mini | 13% |
| Haiku 3.5 | 7% |
| Gemini 2.5 Flash | 2.17% |
| ChatGPT-4o | 0.49% |
| Claude 3.7 Sonnet (thinking) | 0.04% |

**Mitigation**: Use self-consistency (majority vote) to reduce impact of unfaithful paths. Use extended thinking / reasoning models that show higher faithfulness.

#### 7.2 Shallow pattern matching, not algorithmic reasoning

Stechly et al. (NeurIPS 2024) showed that CoT improvements are **narrow and fragile**:
- Performance only appears when prompts are exceedingly specific to the problem class
- CoT enables pattern matching between examples and queries, not genuine algorithm learning
- When problem structure differs even slightly from examples, performance collapses
- Self-consistency does not fix this — it amplifies the dominant pattern

**Mitigation**: Use CoT only for problem classes well-represented in the exemplars. For novel problem structures, consider ToT or GoT which explore multiple solution paths.

#### 7.3 Error compounding (no recovery mechanism)

In CoT, an error at step k propagates through all subsequent steps k+1, k+2, ... with no mechanism for correction. Unlike ToT (which can backtrack) or GoT (which can merge alternative paths), CoT is a single-shot linear chain where every mistake is permanent.

**Mitigation**: Use self-consistency to sample multiple paths — if the error is non-deterministic, the majority of paths may avoid it. For deterministic errors, switch to ToT or GoT.

#### 7.4 Degraded performance on easy problems

CoT can **hurt** performance on simple or commonsense problems where direct answers are sufficient (Wang et al., 2024; Li et al., 2024). Adding reasoning steps introduces unnecessary tokens that can lead the model astray.

**Mitigation**: Apply CoT only when the problem is genuinely complex (mathematical reasoning, multi-step logic). For simple classification or retrieval, use direct prompting.

#### 7.5 Emergent ability threshold

CoT benefits **emerge only in sufficiently large models** (Wei et al., 2022). Models below ~100B parameters show minimal or no improvement from CoT prompting. Small models may even degrade because the additional reasoning tokens add noise.

**Mitigation**: Use CoT with models >= 100B parameters. For smaller models, prefer direct prompting or fine-tuning.

#### 7.6 Summary of failure modes

| Failure Mode | Severity | Frequency | Mitigation |
|-------------|----------|-----------|------------|
| Unfaithful reasoning | High | 0.04%-13% depending on model | Self-consistency, extended thinking models |
| Shallow pattern matching | High | When problem differs from exemplars | ToT/GoT for novel structures |
| Error compounding | High | On any multi-step problem | Self-consistency, ToT backtracking |
| Degraded on easy problems | Medium | Commonsense/simple tasks | Only use CoT on complex reasoning tasks |
| Small model threshold | Medium | Models < 100B params | Use larger models or fine-tuning |
| Incorrect exemplars still help | Medium | All few-shot CoT | Indicates CoT is format, not content, driven |


### References (Chain of Thought)

- Wei, J. et al. (2022). *Chain-of-Thought Prompting Elicits Reasoning in Large Language Models*. NeurIPS 2022. [arXiv:2201.11903](https://arxiv.org/abs/2201.11903)
- Wang, X. et al. (2022). *Self-Consistency Improves Chain of Thought Reasoning in Language Models*. [arXiv:2203.11171](https://arxiv.org/abs/2203.11171)
- Kojima, T. et al. (2022). *Large Language Models are Zero-Shot Reasoners*. NeurIPS 2022. [arXiv:2205.11916](https://arxiv.org/abs/2205.11916)
- Arcuschin, A. et al. (2025). *Chain-of-Thought Reasoning In The Wild Is Not Always Faithful*. [arXiv:2503.08679](https://arxiv.org/abs/2503.08679)
- Stechly, K. et al. (2024). *Chain of Thoughtlessness? An Analysis of CoT in Planning*. NeurIPS 2024. [arXiv:2405.04776](https://arxiv.org/abs/2405.04776)
- Li, Z. et al. (2024). *Towards Better Chain-of-Thought: A Reflection on Effectiveness and Faithfulness*. [arXiv:2405.18915](https://arxiv.org/abs/2405.18915)

---

---

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

---

## Graph of Thought (GoT)

### 1. Overview

Graph of Thought (GoT) is the most general LLM reasoning topology: an **arbitrary directed graph** where nodes represent "thoughts" (units of intermediate reasoning) and edges represent dependencies between them. Introduced by Besta et al. (AAAI 2024), GoT extends both CoT and ToT by enabling **aggregation** — the merging of multiple reasoning paths into a single synthesized thought.

**Critical architectural fact**: Like ToT, GoT is NOT a pure-prompt architecture. It requires **external orchestration code** that constructs and executes a Graph of Operations (GoO). The LLM provides thought generation, scoring, and merging capabilities; the orchestration layer manages graph state, scheduling, and execution order.

**Topology**: A directed graph with arbitrary in-degree and out-degree. Nodes can have multiple predecessors (aggregation/merging) and multiple successors (branching). Cycles are allowed (enabling refinement loops).

**Key structural advantage over ToT**: In ToT, branches never merge — the best branch is selected and all others are discarded. In GoT, multiple branches can be **aggregated** into a single node that combines their strengths. This means no computed information is wasted.

**Key result**: On sorting tasks, GoT improved quality by **~62% over ToT** and **~70% over CoT**, while reducing costs by **>31% over ToT** (Besta et al., 2024).

**Relationship to other topologies** (Besta et al., 2024):
- CoT = GoT with a single linear chain (no branching, no merging)
- ToT = GoT with branching but no merging (max in-degree = 1)
- GoT = Full graph with branching, merging, and cycles

**Reference**: Besta, M. et al. (2024). *Graph of Thoughts: Solving Elaborate Problems with Large Language Models*. AAAI 2024. arXiv:2308.09687

**Official implementation**: [spcl/graph-of-thoughts](https://github.com/spcl/graph-of-thoughts) (~2,600 stars, BSD-style license, pip installable)

**SOURCE AVAILABILITY FLAG**: The GoT framework has a complete official implementation with modular API, but implementation-level tutorials and community examples are sparser than for CoT or ToT. The sorting and keyword-counting examples are well-documented; other domains require adapting the framework. The follow-up KGoT (Knowledge Graph of Thoughts, Besta et al., 2025) extends GoT but is even more sparsely documented. Readers needing deeper implementation guidance should consult:
- The official repo's `examples/` directory
- The paper's Section 4 (framework design) and Appendix (additional experiments)
- Supplementary queries: "graph of thoughts merge sort implementation", "graph of thoughts vs tree of thoughts benchmark sorting"


### 2. Core Algorithm

GoT decomposes problem-solving into operations on a graph of thoughts:

#### 2.1 Graph of Operations (GoO) — Static Execution Plan

The GoO is defined before execution begins. It specifies:
- Which operations to perform (Generate, Score, Aggregate, Improve, Validate, KeepBestN)
- The dependencies between operations (which operations must complete before others can start)
- The data flow (how thoughts flow from operation outputs to inputs)

#### 2.2 Graph Reasoning State (GRS) — Dynamic Runtime State

The GRS is maintained during execution. It tracks:
- All thoughts generated so far
- Scores assigned to thoughts
- Which operations have completed
- The current "frontier" of thoughts available for further processing

#### 2.3 Core Operations

| Operation | Input | Output | Purpose |
|-----------|-------|--------|---------|
| **Generate(t, k)** | 1 parent thought t | k new child thoughts | Branch: create alternatives |
| **Aggregate(t1, ..., tk)** | k thoughts | 1 merged thought | Merge: combine insights from multiple paths |
| **Score(t)** | 1 thought | 1 numeric value | Evaluate: assess thought quality |
| **Improve(t)** | 1 thought | 1 improved thought | Refine: loop back and enhance |
| **Validate(t)** | 1 thought | bool | Check: verify correctness |
| **ValidateAndImprove(t)** | 1 thought | 1 valid thought | Iterate: validate, then improve if invalid |
| **KeepBestN(N)** | all thoughts | N best thoughts | Prune: keep top performers |

#### 2.4 Execution Model

```
ALGORITHM: GraphOfThoughts_Execute

Input:
  GoO        -- Graph of Operations (static plan)
  Prompter   -- function: (operation_type, thoughts) -> LLM prompt
  Parser     -- function: (operation_type, llm_response) -> structured thoughts
  LLM        -- language model API

Output: final thoughts with scores

1.  GRS = initialize_empty_graph_reasoning_state()
2.  ready_ops = get_operations_with_all_predecessors_complete(GoO)
3.
4.  WHILE ready_ops is not empty:
5.      FOR each operation op in ready_ops (parallelizable):
6.          input_thoughts = get_input_thoughts(op, GRS)
7.          prompt = Prompter(op.type, input_thoughts)
8.          response = LLM.generate(prompt)
9.          output_thoughts = Parser(op.type, response)
10.         update_GRS(GRS, op, output_thoughts)
11.     END FOR
12.     ready_ops = get_operations_with_all_predecessors_complete(GoO)
13. END WHILE
14.
15. RETURN GRS.final_thoughts
```

**Key difference from ToT**: The GoO can specify **merge operations** that take multiple thoughts as input and produce a single synthesized thought. ToT has no equivalent — branches in ToT are either selected or discarded.


### 3. Prompt Template(s)

**Template A: Generate + Aggregate for Merge Sort**

Generation prompt (split list into sublists and sort each):
```
Sort the following list of numbers in ascending order.
Return ONLY the sorted list, nothing else.

List: {input_list}

Sorted list:
```

Aggregation prompt (merge two sorted sublists):
```
You are merging two sorted lists into one sorted list.
Both input lists are already sorted in ascending order.
Produce the merged sorted list by comparing elements from both lists.

List 1: {sorted_sublist_1}
List 2: {sorted_sublist_2}

Merged sorted list:
```

Scoring prompt (count errors in sorted output):
```
The following list is supposed to be sorted in ascending order.
Count how many elements are out of order (an element is out of order
if it is smaller than the element before it).

List: {candidate_sorted_list}

Number of errors:
```

**Template B: General-Purpose GoT with Aggregate and Improve**

```
You are solving a complex problem using a graph of thoughts.

## Phase: {operation_type}

{operation_type} instructions:
- GENERATE: Create {k} distinct candidate solutions for the problem below.
  Each candidate should approach the problem from a different angle.

- AGGREGATE: You are given {k} partial solutions. Synthesize them into
  a single improved solution that combines the best elements of each.
  Identify what each input does well and merge those strengths.

- IMPROVE: You are given a current solution and identified weaknesses.
  Address each weakness while preserving the solution's strengths.

- SCORE: Rate the following solution from 1 to 10 on:
  (a) Correctness: Does it satisfy all problem constraints?
  (b) Completeness: Does it address all aspects of the problem?
  (c) Coherence: Is the reasoning internally consistent?
  Conclude with "Score: {n}" on the last line.

Current problem: {problem_statement}

{input_thoughts}

Your response:
```


### 4. Implementation Pseudocode

```
ALGORITHM: GoT_MergeSort

Input:
  numbers      -- list of N numbers to sort
  chunk_size   -- size of each sublist for parallel sorting
  LLM          -- language model API

Output: sorted list of numbers

1.  # PHASE 1: GENERATE — Split and sort sublists in parallel
2.  chunks = split_list(numbers, chunk_size)
3.  sorted_chunks = []
4.  FOR each chunk in chunks (parallelizable):
5.      prompt = sort_prompt(chunk)
6.      sorted_chunk = LLM.generate(prompt)
7.      sorted_chunks.append(sorted_chunk)
8.  END FOR
9.
10. # PHASE 2: AGGREGATE — Merge sorted sublists pairwise
11. current_lists = sorted_chunks
12. WHILE len(current_lists) > 1:
13.     next_lists = []
14.     FOR i = 0 TO len(current_lists) - 1 STEP 2:
15.         if i+1 < len(current_lists):
16.             prompt = merge_prompt(current_lists[i], current_lists[i+1])
17.             merged = LLM.generate(prompt)
18.             next_lists.append(merged)
19.         ELSE:
20.             next_lists.append(current_lists[i])  # odd element, carry forward
21.     END FOR
22.     current_lists = next_lists
23. END WHILE
24.
25. # PHASE 3: SCORE + IMPROVE — Validate and fix if needed
26. final_list = current_lists[0]
27. error_count = count_sorting_errors(final_list)
28. IF error_count > 0:
29.     prompt = improve_prompt(final_list, error_count)
30.     final_list = LLM.generate(prompt)
31. END IF
32.
33. RETURN final_list


ALGORITHM: GoT_General

Input:
  GoO          -- Graph of Operations (user-defined execution plan)
  LLM          -- language model API
  Prompter     -- maps (operation, thoughts) -> prompt
  Parser       -- maps (operation, response) -> thoughts

Output: best final thought(s)

1.  GRS = {}  -- empty Graph Reasoning State
2.  frontier = GoO.operations_with_no_predecessors()
3.
4.  WHILE frontier is not empty:
5.      next_frontier = []
6.      FOR each operation in frontier:
7.          # Gather input thoughts from predecessors
8.          inputs = [GRS[predecessor] for predecessor in operation.predecessors]
9.
10.         # Execute operation via LLM
11.         prompt = Prompter(operation.type, inputs)
12.         raw = LLM.generate(prompt)
13.         outputs = Parser(operation.type, raw)
14.
15.         # Score if scoring operation
16.         IF operation.type == "Score":
17.             FOR each thought in outputs:
18.                 thought.score = parse_score(raw)
19.
20.         # Store in GRS
21.         GRS[operation] = outputs
22.         operation.status = COMPLETED
23.
24.         # Check if any successors are now ready
25.         FOR each successor in operation.successors:
26.             IF all predecessors of successor are COMPLETED:
27.                 next_frontier.append(successor)
28.     END FOR
29.     frontier = next_frontier
30. END WHILE
31.
32. RETURN GRS[GoO.final_operation]
```

**Key orchestration elements that prompting alone cannot provide:**
1. **Graph of Operations construction** — defining the execution plan before runtime
2. **Dependency tracking** — ensuring operations execute only after their predecessors complete
3. **Aggregation scheduling** — routing multiple thoughts into a single LLM call for merging
4. **Iterative improvement loops** — feeding scored/improved thoughts back for re-evaluation
5. **Parallel execution** — running independent operations concurrently


### 5. Implementation Orchestration Code

```python
"""
Graph of Thoughts orchestration scaffold.
Requires: openai (pip install openai)
"""

from dataclasses import dataclass, field
from typing import List, Optional, Callable
from openai import OpenAI


# ---------------------------------------------------------------------------
# Data structures
# ---------------------------------------------------------------------------

@dataclass
class Thought:
    """A single thought node in the reasoning graph."""
    content: str
    score: float = 0.0
    source_ids: list = field(default_factory=list)  # IDs of predecessor thoughts
    id: int = 0

    def is_valid(self) -> bool:
        return self.score > 0.0


@dataclass
class Operation:
    """A node in the Graph of Operations."""
    op_type: str           # "generate", "aggregate", "score", "improve", "keep_best_n"
    predecessors: list = field(default_factory=list)  # Operation references
    successors: list = field(default_factory=list)
    n_outputs: int = 1     # how many thoughts this operation produces
    n_keep: int = 1        # for keep_best_n
    completed: bool = False
    outputs: list = field(default_factory=list)  # Thought references after execution


# ---------------------------------------------------------------------------
# LLM interface
# ---------------------------------------------------------------------------

class LLM:
    def __init__(self, model: str = "gpt-4", api_key: str | None = None):
        self.client = OpenAI(api_key=api_key)
        self.model = model

    def generate(self, prompt: str, temperature: float = 0.7, max_tokens: int = 512) -> str:
        response = self.client.chat.completions.create(
            model=self.model,
            messages=[{"role": "user", "content": prompt}],
            temperature=temperature,
            max_tokens=max_tokens,
        )
        return response.choices[0].message.content


# ---------------------------------------------------------------------------
# Graph of Thoughts orchestrator
# ---------------------------------------------------------------------------

class GraphOfThoughts:
    """
    Orchestrates execution of a Graph of Operations (GoO).

    You MUST supply:
      - prompter(operation_type, input_thoughts) -> prompt string
      - parser(operation_type, llm_response) -> list of Thought objects
      - goo: the Graph of Operations defining execution order
    """

    def __init__(
        self,
        llm: LLM,
        prompter: Callable[[str, List[Thought]], str],
        parser: Callable[[str, str], List[Thought]],
    ):
        self.llm = llm
        self.prompter = prompter
        self.parser = parser
        self.thought_counter = 0

    def _new_thought(self, content: str, source_ids: list = None) -> Thought:
        self.thought_counter += 1
        return Thought(content=content, id=self.thought_counter, source_ids=source_ids or [])

    def _get_ready_operations(self, goo: list[Operation]) -> list[Operation]:
        """Find operations whose all predecessors are completed."""
        ready = []
        for op in goo:
            if op.completed:
                continue
            if all(pred.completed for pred in op.predecessors):
                ready.append(op)
        return ready

    def _gather_inputs(self, op: Operation) -> list[Thought]:
        """Collect all output thoughts from predecessor operations."""
        inputs = []
        for pred in op.predecessors:
            inputs.extend(pred.outputs)
        return inputs

    def run(self, goo: list[Operation]) -> list[Thought]:
        """
        Execute the Graph of Operations.
        Returns the output thoughts of the final operation(s).
        """
        while True:
            ready = self._get_ready_operations(goo)
            if not ready:
                break  # All operations completed or deadlocked

            for op in ready:
                input_thoughts = self._gather_inputs(op)

                if op.op_type == "keep_best_n":
                    # No LLM call needed — just sort and truncate
                    sorted_thoughts = sorted(input_thoughts, key=lambda t: t.score, reverse=True)
                    op.outputs = sorted_thoughts[:op.n_keep]
                    op.completed = True
                    continue

                # Build prompt and call LLM
                prompt = self.prompter(op.op_type, input_thoughts)
                response = self.llm.generate(prompt)
                parsed = self.parser(op.op_type, response)

                # Assign source IDs for aggregation tracking
                source_ids = [t.id for t in input_thoughts]
                for thought in parsed:
                    thought.id = self._new_thought(thought.content).id
                    thought.source_ids = source_ids

                op.outputs = parsed
                op.completed = True

        # Return outputs of operations with no successors (terminal operations)
        terminal = [op for op in goo if not op.successors]
        results = []
        for op in terminal:
            results.extend(op.outputs)
        return results


# ---------------------------------------------------------------------------
# Merge Sort example using GoT
# ---------------------------------------------------------------------------

MERGE_SORT_PROMPT = """Sort the following numbers in ascending order.
Return ONLY the sorted numbers separated by spaces.

Numbers: {numbers}

Sorted:"""

MERGE_PROMPT = """Merge these two sorted lists into one sorted list.
Both lists are already sorted. Compare elements and merge them.

List 1: {list1}
List 2: {list2}

Merged sorted list:"""

SCORE_PROMPT = """Count how many elements are out of order in this list.
An element is out of order if it is smaller than the previous element.
Return only the error count as a number.

List: {sorted_list}

Error count:"""

IMPROVE_PROMPT = """This list has {n_errors} sorting errors.
Fix the errors and return the correctly sorted list.

List with errors: {bad_list}

Corrected list:"""


def merge_sort_prompter(op_type: str, inputs: list[Thought]) -> str:
    if op_type == "generate":
        return MERGE_SORT_PROMPT.format(numbers=inputs[0].content)
    elif op_type == "aggregate":
        return MERGE_PROMPT.format(
            list1=inputs[0].content,
            list2=inputs[1].content if len(inputs) > 1 else ""
        )
    elif op_type == "score":
        return SCORE_PROMPT.format(sorted_list=inputs[0].content)
    elif op_type == "improve":
        return IMPROVE_PROMPT.format(
            n_errors=inputs[0].score,
            bad_list=inputs[0].content
        )
    return ""


def merge_sort_parser(op_type: str, response: str) -> list[Thought]:
    if op_type == "score":
        try:
            score = float(response.strip())
        except ValueError:
            score = 0.0
        return [Thought(content="", score=max(0, 10 - score))]  # fewer errors = higher score
    else:
        return [Thought(content=response.strip())]


def build_merge_sort_goo(numbers: str, chunk_size: int = 8) -> list[Operation]:
    """
    Build a Graph of Operations for merge sort.
    Splits numbers into chunks, sorts each, then merges pairwise.
    """
    nums = numbers.strip("[]").split(",")
    chunks = [",".join(nums[i:i+chunk_size]) for i in range(0, len(nums), chunk_size)]

    goo = []

    # Phase 1: Generate — sort each chunk
    gen_ops = []
    for chunk in chunks:
        op = Operation(op_type="generate")
        op.outputs = [Thought(content=chunk)]  # pre-seed input
        op.completed = False  # will be executed by LLM
        goo.append(op)
        gen_ops.append(op)

    # Phase 2: Aggregate — pairwise merge
    current = gen_ops
    while len(current) > 1:
        next_level = []
        for i in range(0, len(current), 2):
            merge = Operation(op_type="aggregate")
            if i + 1 < len(current):
                merge.predecessors = [current[i], current[i+1]]
                current[i].successors.append(merge)
                current[i+1].successors.append(merge)
            else:
                merge.predecessors = [current[i]]
                current[i].successors.append(merge)
            goo.append(merge)
            next_level.append(merge)
        current = next_level

    return goo


# ---------------------------------------------------------------------------
# Usage example
# ---------------------------------------------------------------------------

def sort_with_got(numbers: str) -> str:
    llm = LLM(model="gpt-4")
    got = GraphOfThoughts(llm=llm, prompter=merge_sort_prompter, parser=merge_sort_parser)
    goo = build_merge_sort_goo(numbers, chunk_size=8)
    results = got.run(goo)
    return results[0].content if results else ""


if __name__ == "__main__":
    test_list = "[0, 2, 6, 3, 8, 7, 1, 1, 4, 9, 5, 3, 2, 8, 0, 7]"
    result = sort_with_got(test_list)
    print(f"Sorted: {result}")
```


### 6. Worked Example

**Problem**: Sort the list `[0, 2, 6, 3, 8, 7, 1, 1, 4, 9, 5, 3, 2, 8, 0, 7]` (16 elements).

**Configuration**: GoT merge sort with chunk_size=4 (4 chunks of 4 elements each).

#### Phase 1: GENERATE — Sort each chunk in parallel

| Branch | Input Chunk | LLM-Sorted Output |
|--------|-------------|-------------------|
| A | `[0, 2, 6, 3]` | `[0, 2, 3, 6]` |
| B | `[8, 7, 1, 1]` | `[1, 1, 7, 8]` |
| C | `[4, 9, 5, 3]` | `[3, 4, 5, 9]` |
| D | `[2, 8, 0, 7]` | `[0, 2, 7, 8]` |

These 4 operations are independent and can execute in parallel.

#### Phase 2: AGGREGATE — Merge sorted chunks pairwise

**Merge AB**: Aggregate Branch A + Branch B
- Input: `[0, 2, 3, 6]` and `[1, 1, 7, 8]`
- LLM merges by comparing: 0<1, so 0. Then 1<2, so 1,1. Then 2<7, so 2,3. Then 6<7, so 6. Then 7,8.
- Output: `[0, 1, 1, 2, 3, 6, 7, 8]`

**Merge CD**: Aggregate Branch C + Branch D
- Input: `[3, 4, 5, 9]` and `[0, 2, 7, 8]`
- Output: `[0, 2, 3, 4, 5, 7, 8, 9]`

#### Phase 3: AGGREGATE — Final merge

**Merge AB+CD**: Aggregate both merged halves
- Input: `[0, 1, 1, 2, 3, 6, 7, 8]` and `[0, 2, 3, 4, 5, 7, 8, 9]`
- Output: `[0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 7, 8, 8, 9]`

#### Phase 4: SCORE + IMPROVE — Validate

Scoring: Count errors in final list. No elements out of order → score = 10 (0 errors). No improvement needed.

#### How this differs from ToT

**ToT approach to sorting**: Try 3 different sorting strategies (quick sort, merge sort, bubble sort) in parallel branches. Evaluate each. Select the best output. The other two branches' computation is discarded.

**GoT approach**: Split the list into sublists. Sort each independently (parallel branches). Then **merge** the results. All computation is preserved and combined — no work is wasted.

This is the fundamental structural difference: **ToT selects, GoT synthesizes.** In domains where partial solutions can be meaningfully combined (sorting, document merging, set operations), GoT dramatically outperforms ToT because aggregation preserves information that ToT would prune away.


### 7. Comparison to Other Topologies

| Property | Chain of Thought (CoT) | Tree of Thought (ToT) | Graph of Thought (GoT) |
|----------|----------------------|----------------------|------------------------|
| **Topology** | Linear chain (path graph) | K-ary tree (branching only) | Directed graph (branching + merging) |
| **Max out-degree** | 1 | k (branching factor) | k (branching factor) |
| **Max in-degree** | 1 | 1 (single parent) | **>1** (aggregation — multiple parents) |
| **Merging/aggregation** | None | None | **Yes** — key differentiator |
| **Cycles** | No | No | Yes — enables refinement loops |
| **Backtracking** | No | Yes (DFS variant) | Yes (via cycles) |
| **Intermediate evaluation** | No | Yes (value/vote) | Yes (score + validate + improve) |
| **Orchestration code needed** | No | Yes | Yes |
| **Subset relationship** | GoT subset | GoT subset | Full generality |

**Concrete structural distinction from CoT**: GoT allows both branching (out-degree > 1) and merging (in-degree > 1), while CoT allows neither. GoT can combine information from multiple independent reasoning paths; CoT can only process one path.

**Concrete structural distinction from ToT**: GoT allows aggregation (in-degree > 1) — multiple branches can converge into a single thought that synthesizes their contributions. ToT only allows branching; branches never merge. The key practical implication: GoT can preserve and combine partial solutions; ToT must select one branch and discard the rest.

**Information flow analysis** (Besta et al., 2024):
- **CoT**: Latency = N, Volume = N (sequential, but all information reaches the answer)
- **ToT**: Latency = log_k(N), Volume = log_k(N) (parallel, but most information is pruned)
- **GoT**: Latency = log_k(N), Volume = N (parallel AND all information reaches the answer via merging)

GoT achieves the best of both worlds: parallel execution (low latency) AND full information flow (high volume).


### 8. Limitations and Failure Modes

#### 8.1 Implementation complexity

GoT requires the most engineering effort of the three topologies:
- Defining the Graph of Operations (GoO) requires understanding the problem's decomposability
- Merge/aggregation prompts must be carefully designed to combine partial results without information loss
- The orchestration layer must handle dependency tracking, parallel execution, and error recovery
- Cycle detection is needed to prevent infinite loops in refinement operations

**Mitigation**: Start with a simple linear GoO (equivalent to CoT) and incrementally add branching and aggregation. Use the official framework (`pip install graph_of_thoughts`) which provides the orchestration layer.

#### 8.2 Merge conflicts and quality

When merging multiple reasoning paths, the LLM may:
- Lose information from one or more input thoughts
- Introduce contradictions between merged elements
- Prefer one input disproportionately due to position bias (first-listed input gets more weight)
- Fail to identify genuine synergies between inputs

**Mitigation**: Use explicit merge instructions that require the model to "identify the best element from each input and combine them." Score merged outputs independently. Use multi-pass merging for complex aggregations.

#### 8.3 Graph complexity explosion

Unlike ToT's bounded tree structure, GoT's arbitrary graph can grow unpredictably:
- Refinement cycles can loop indefinitely without convergence guarantees
- Aggregation nodes increase the number of edges, making dependency tracking complex
- The number of LLM calls grows with the number of operations, not just tree depth

**Mitigation**: Set maximum iteration counts for improvement cycles. Use KeepBestN operations to prune low-scoring thoughts. Define the GoO with clear terminal operations.

#### 8.4 Sparse implementation resources

GoT has fewer tutorials, blog posts, and community examples than CoT or ToT:
- Only 2 official examples (sorting, keyword counting)
- Fewer community implementations
- The paper's framework design is well-documented, but practical integration guides are limited

**Mitigation**: Consult the official repository's examples directory. Adapt the sorting example as a template. Search for "graph of thoughts merge sort implementation" for community resources.

#### 8.5 Cost of aggregation calls

Each merge/aggregation call requires an LLM API call that takes multiple thoughts as input. These calls:
- Require longer prompts (concatenating multiple thoughts)
- Produce outputs that must be re-evaluated
- Can dominate the total cost for problems with many merge steps

**Mitigation**: Batch merges where possible. Use cheaper models for scoring and validation. Cache identical merge results.

#### 8.6 Summary of failure modes

| Failure Mode | Severity | Frequency | Mitigation |
|-------------|----------|-----------|------------|
| Implementation complexity | High | Every new GoT application | Start simple, use official framework |
| Merge quality / conflicts | High | When merging diverse paths | Explicit merge instructions, multi-pass |
| Graph complexity explosion | Medium | With many refinement cycles | Max iteration limits, KeepBestN pruning |
| Sparse resources | Medium | For new domains | Adapt sorting example, consult paper appendix |
| Aggregation call costs | Medium | Problems with many merge steps | Batch merges, cheaper scoring models |
| Position bias in merging | Medium | When inputs are ordered | Randomize input order, multi-merge voting |


### 9. Source Availability Flag

| Section | Source Strength | Notes |
|---------|----------------|-------|
| Overview / theoretical basis | **Strong** | AAAI 2024 paper, comprehensive survey (Besta et al., 2024) |
| Core algorithm | **Strong** | Paper Section 4, official implementation |
| Prompt templates | **Moderate** | Sorting and keyword-counting examples only; other domains need adaptation |
| Implementation pseudocode | **Moderate** | Framework API is documented; detailed orchestration code requires reading the repo |
| Worked example | **Moderate** | Sorting example from paper; other domain examples are sparse |
| Comparison to other topologies | **Strong** | Besta et al. (2024) survey provides comprehensive analysis |
| Limitations | **Moderate** | Some failure modes (merge conflicts) are theoretical; limited empirical validation |

**Supplementary search queries for deeper research**:
1. "graph of thoughts merge sort implementation python"
2. "graph of thoughts keyword counting example"
3. "graph of thoughts vs tree of thoughts benchmark sorting quality"
4. "knowledge graph of thoughts KGoT implementation 2025"
5. "framework of thoughts FoT dynamic graph reasoning"


### References (Graph of Thought)

- Besta, M. et al. (2024). *Graph of Thoughts: Solving Elaborate Problems with Large Language Models*. AAAI 2024. [arXiv:2308.09687](https://arxiv.org/abs/2308.09687)
- Official implementation: [spcl/graph-of-thoughts](https://github.com/spcl/graph-of-thoughts)
- Besta, M. et al. (2024). *Demystifying Chains, Trees, and Graphs of Thoughts*. [arXiv:2401.14295](https://arxiv.org/abs/2401.14295)
- Besta, M. et al. (2025). *Knowledge Graph of Thoughts (KGoT)*. [github.com/spcl/knowledge-graph-of-thoughts](https://github.com/spcl/knowledge-graph-of-thoughts)
- Fricke et al. (2025). *Framework of Thoughts (FoT)*. [arXiv:2602.16512](https://arxiv.org/abs/2602.16512)

---

## Comparative Analysis

### 1. When to Use Each Topology

| Topology | Use When | Avoid When |
|----------|----------|------------|
| **Chain of Thought** | Problems are sequential and well-defined; exemplars closely match the query; model is large (>= 100B); cost must be minimal; single API call needed | Problem has many possible solution paths; early mistakes are unrecoverable; exemplars don't match query structure; model is small (< 100B) |
| **Tree of Thought** | CoT success rate < 30%; problem has natural intermediate states that can be evaluated; search space is bounded and structured; recovery from mistakes is critical | Problem is simple enough for CoT; no clear evaluation criteria for intermediate states; model is small (ToT amplifies weaknesses); cost budget is very tight |
| **Graph of Thought** | Partial solutions can be meaningfully combined (sorting, merging, set operations); refinement loops add value; problem decomposes into independent subproblems with aggregation; parallelism is desired | Problem is sequential by nature; merging partial solutions provides no benefit; implementation complexity is not justified; only simple reasoning is needed |

**Decision flowchart**:

```
START: What type of problem?
│
├── Sequential, well-structured, exemplars available
│   └── Use CoT (or CoT-SC for robustness)
│
├── Multiple valid paths, intermediate states evaluable, CoT fails
│   └── Use ToT (BFS for wide/shallow, DFS for deep/narrow)
│
├── Subproblems can be solved independently AND combined
│   └── Use GoT (with aggregation operations)
│
└── Unclear
    └── Start with CoT-SC (cheapest upgrade), escalate to ToT if needed
```


### 2. Computational Overhead Comparison

| Dimension | CoT | CoT-SC (k=5) | CoT-SC (k=40) | ToT (b=5, d=3) | GoT (merge sort) |
|-----------|-----|--------------|----------------|-----------------|-------------------|
| **LLM calls** | 1 | 5 | 40 | ~75-125 | Variable (O(N/chunk × log(N/chunk))) |
| **Prompt tokens** | ~500 | ~2,500 | ~20,000 | ~5,500 | ~2,000-10,000 |
| **Generated tokens** | ~200 | ~1,000 | ~8,000 | ~5,500 | ~1,500-5,000 |
| **Cost per problem (GPT-4)** | ~$0.01 | ~$0.05 | ~$0.47 | ~$0.74 | ~$0.30-0.50 |
| **Latency (sequential)** | Low | k × low | 40 × low | Medium (depth-dependent) | Low- Medium (parallelizable) |
| **Latency (parallel)** | Low | Low | Low | Medium (depends on depth) | **Low** (parallel chunks) |

**Key insight**: GoT can achieve **lower cost than ToT** despite greater graph complexity, because:
1. Parallel execution reduces latency
2. Aggregation preserves work that ToT would waste on pruned branches
3. Merge operations combine multiple thoughts in a single LLM call

For the sorting benchmark: GoT achieved 62% quality improvement over ToT at >31% lower cost.


### 3. Composability — Can CoT, ToT, and GoT Be Combined?

**Yes.** All three topologies are subsets of the most general graph topology, and hybrid approaches are an active research area.

#### Concrete hybrid patterns:

**Pattern 1: CoT within ToT (CoT-as-thought)**
Each "thought" in the ToT tree is itself a CoT chain. The model generates a full reasoning trace as a single thought, then the ToT evaluator assesses the trace.

```
ToT Node → LLM generates full CoT chain → Evaluator scores chain → ToT prunes
```

This is how the original ToT paper's Creative Writing task works: each candidate is a full plan+passage (a CoT-style output), evaluated by the tree search.

**Pattern 2: CoT within GoT (branch-then-merge chains)**
Multiple independent CoT chains solve subproblems, then a GoT aggregation step merges their results.

```
Subproblem A → CoT chain A ─┐
Subproblem B → CoT chain B ─┤→ Aggregate → Final answer
Subproblem C → CoT chain C ─┘
```

This is the merge sort pattern: each chunk is sorted via CoT, then GoT merges the results.

**Pattern 3: ToT + GoT (explore, then synthesize)**
Use ToT to explore multiple solution paths, then use GoT aggregation to combine the best elements from different branches instead of selecting just one.

```
        ┌─ Branch A (score: 0.9) ─┐
Root ───┼─ Branch B (score: 0.8) ─┼→ Aggregate → Synthesized solution
        └─ Branch C (score: 0.7) ─┘
```

This addresses ToT's key weakness (discarding pruned branches) while preserving its exploration strength.

**Pattern 4: Cumulative Reasoning (DAG-based hybrid)**
Each step can access ALL previous results, not just the current branch. This is a GoT where the graph is a DAG (directed acyclic graph) — cycles are removed but aggregation is preserved.

**Framework support for hybrids**:
- **GoT official framework**: Can express CoT, ToT, and GoT via the Graph of Operations API
- **Framework of Thoughts (FoT)**: Dynamic graph modification at runtime, automatic optimization
- **ThoughtSculpt**: MCTS + self-revision (tree search with GoT-style refinement)

#### Constraints on composition:

1. **Cost compounds**: Each added topology layer multiplies LLM calls
2. **Evaluation becomes harder**: Hybrid systems need evaluation at multiple granularities
3. **Debugging complexity**: Failure in a hybrid system can originate from any layer
4. **Diminishing returns**: For problems where CoT-SC already achieves >80% accuracy, adding ToT or GoT layers provides minimal improvement at high cost


### 4. LLM Capability Requirements Per Topology

| Capability | CoT | CoT-SC | ToT | GoT |
|-----------|-----|--------|-----|-----|
| **Min model size** | ~100B params for emergent CoT | Same as CoT | **GPT-4 class** recommended; < 8B often fails | **GPT-4 class** required; merge quality depends on strong model |
| **Instruction following** | Medium (follow exemplar format) | Medium | High (generate + evaluate + format parseable output) | Very High (generate + evaluate + merge multiple inputs) |
| **Self-evaluation** | Not required | Not required | **Critical** (evaluate own intermediate states) | **Critical** (evaluate + validate + improve) |
| **Structured output** | Low (free-form reasoning) | Low | Medium (parseable proposals and scores) | **High** (merge prompts require combining multiple inputs reliably) |
| **Context length** | Low (~1K tokens) | Low per path | Medium (~5K tokens for candidate generation + evaluation) | High (~10K+ tokens for aggregation prompts with multiple inputs) |
| **Cost tolerance** | Low ($0.01-0.05/problem) | Low-Medium ($0.05-0.50) | Medium-High ($0.50-1.00/problem) | Variable ($0.30-1.00/problem, can be less than ToT) |

**Key takeaway**: CoT works with any sufficiently large model. ToT requires models that can both generate diverse candidates AND evaluate them reliably — this is hard for models below GPT-4 class. GoT additionally requires strong merging/synthesis capability and longer context windows.

**Model recommendation by topology**:
| Topology | Recommended Models | Models to Avoid |
|----------|-------------------|-----------------|
| CoT | GPT-4, Claude 3.5+, Gemini Pro, Llama 3 70B+ | Models < 100B params |
| CoT-SC | Same as CoT (temperature > 0.5 required) | Same as CoT |
| ToT | GPT-4, Claude 3.5 Opus, Gemini Ultra | Llama 3 8B, GPT-3.5, any model < 8B |
| GoT | GPT-4, Claude 3.5 Opus (128K context) | Models with < 32K context, models that struggle with structured output |


### 5. Hybrid Algorithm — CoT-within-ToT Pseudocode

```
ALGORITHM: CoT_Within_ToT (CoT as thought, ToT as search)

Input:
  x              -- the problem instance
  exemplars      -- few-shot CoT exemplars
  model          -- LLM API
  k              -- ToT candidates per expansion
  b              -- ToT beam width
  T              -- ToT depth (number of intermediate checkpoints)
  GENERATE_COT   -- function: state -> CoT reasoning trace (full chain)
  EVALUATE_COT  -- function: CoT trace -> numeric value

Output: best CoT reasoning trace with final answer

1.  states <- {empty_state}
2.  FOR t = 1 TO T:
3.      candidates <- {}
4.      FOR each state s in states:
5.          # Each "thought" is a full CoT chain for the next sub-problem
6.          cot_traces = GENERATE_COT(s, exemplars, k)  # k independent CoT chains
7.          FOR each trace in cot_traces:
8.              new_state = s + extract_partial_solution(trace)
9.              value = EVALUATE_COT(trace)
10.             candidates <- candidates + {(new_state, trace, value)}
11.     END FOR
12.     SORT candidates by value (descending)
13.     states <- top b candidates    # PRUNING STEP
14. END FOR
15. RETURN best CoT trace from highest-value state
```

**How this combines both topologies**: The inner step (line 6-7) uses CoT to generate a full reasoning chain for each sub-problem. The outer loop (lines 2-14) uses ToT beam search to evaluate and select the best chains at each checkpoint. This gives CoT's detailed reasoning within ToT's error-recovery framework.


### 6. Hybrid Prompt Templates

**Template A: CoT-then-Aggregate (for decomposable problems)**

Use this when a problem can be split into independent sub-problems, each solved by CoT, then merged.

```
You are solving a complex problem by decomposing it into sub-problems.

## Step 1: Decompose
Break the following problem into 3 independent sub-problems.
For each sub-problem, write it as a self-contained question.

Problem: {problem}

Sub-problems:
1.
2.
3.
```

After solving each sub-problem with a standard CoT prompt, aggregate:

```
You are synthesizing results from 3 independent analyses into a final answer.
Each analysis solved one part of the original problem.

Original problem: {problem}

Analysis 1 (Sub-problem: {sub1}): {cot_result_1}
Analysis 2 (Sub-problem: {sub2}): {cot_result_2}
Analysis 3 (Sub-problem: {sub3}): {cot_result_3}

Synthesize all three results into a complete, coherent answer.
Ensure no information from any analysis is lost.
```

**Template B: ToT-Explore-then-GoT-Synthesize (for creative/synthesis problems)**

Use this when you want ToT's exploration breadth but GoT's ability to combine the best elements.

```
## Phase 1: Generate diverse candidates

Generate {k} distinct approaches to the following problem.
Each approach should differ in strategy, not just phrasing.

Problem: {problem}

Approach 1:
Approach 2:
Approach 3:
```

After evaluating each approach (using ToT-style scoring), synthesize:

```
You are synthesizing the best elements from multiple approaches.

Problem: {problem}

Approach A (score: {score_a}): {approach_a}
Approach B (score: {score_b}): {approach_b}
Approach C (score: {score_c}): {approach_c}

Create a final solution that:
1. Adopts Approach A's strength: {strength_a}
2. Adopts Approach B's strength: {strength_b}
3. Adopts Approach C's strength: {strength_c}
4. Addresses the weaknesses identified in each approach

Final synthesized solution:
```


### 7. Worked Example — Hybrid Topology

**Problem**: "Design a meal plan for a week that is: (1) under $50 total, (2) at least 100g protein per day, (3) vegetarian."

**Phase 1 — CoT-then-Aggregate**:

**Sub-problem decomposition** (CoT prompt):
```
1. What are the cheapest vegetarian protein sources under $2/serving?
2. How many servings of each protein source are needed for 100g/day over 7 days?
3. What complementary ingredients fill out the meals within the $50 budget?
```

**Independent CoT chains**:

Sub-problem 1 CoT: "Lentils ($1.50/lb, 18g protein/cup), eggs ($2/dozen, 6g each), tofu ($2/block, 20g), peanut butter ($3/jar, 8g/2tbsp), black beans ($1/can, 15g). The cheapest sources per gram of protein are lentils, eggs, and black beans."

Sub-problem 2 CoT: "For 100g/day × 7 days = 700g total protein. Using lentils (18g/cup): need ~6 cups = 1.5 lbs = $2.25. Eggs (6g each): need ~30 eggs = 2.5 dozen = $5. Tofu (20g/block): need ~5 blocks = $10. Total: ~$17.25 for protein core."

Sub-problem 3 CoT: "Budget remaining: $50 - $17.25 = $32.75. Need: grains (rice $3/bag, bread $3), vegetables (frozen mixed $4/bag × 3 = $12), fruit (bananas $2/bunch × 3 = $6), dairy/other (milk $3, cheese $4, pasta $2). Total: ~$30.25. Grand total: $17.25 + $30.25 = $47.50. Under budget."

**Aggregation prompt**:

"Synthesize the three analyses into a complete 7-day vegetarian meal plan that stays under $50 and provides at least 100g protein per day. Use the cheapest protein sources identified (lentils, eggs, black beans) and distribute the complementary ingredients across the week."

**Result**: A full 7-day meal plan synthesized from three independent CoT analyses, preserving the cost optimization from sub-problem 2 and the nutritional completeness from sub-problem 1.

**How this differs from pure CoT**: A single CoT chain would try to solve all three constraints simultaneously, likely getting confused by the budget-protein-taste tradeoff. The hybrid approach solves each constraint independently (where CoT excels) and then aggregates (where GoT's merge capability adds value).


### 8. Comparative Failure Modes

| Failure Mode | Severity | Frequency | Mitigation |
|-------------|----------|-----------|------------|
| **Cost compounding** — Each added topology layer multiplies LLM calls. CoT-within-ToT costs k×b×T more than CoT alone | High | Every hybrid approach | Set hard token budgets; escalate topology only when simpler methods fail |
| **Evaluation misalignment** — ToT evaluator scores on one metric (e.g., logical coherence) while GoT merger optimizes another (e.g., completeness) | Medium | When evaluation criteria differ across layers | Use a single unified rubric across all layers |
| **Diminishing returns** — Adding ToT or GoT to problems where CoT-SC already achieves >80% accuracy | Medium | Easy-to-medium difficulty problems | Use the decision flowchart: start with CoT-SC, only escalate if accuracy < 30% |
| **Merge information loss** — Aggregation step drops key details from one or more input chains | High | When merging diverse reasoning paths | Use explicit "adopt each approach's strength" instructions; score merged output independently |
| **Debugging opacity** — Failures in hybrid systems can originate from any layer; hard to isolate | Medium | All hybrid systems | Log each layer's inputs/outputs; validate intermediate results with external checkers |
| **Topology mismatch** — Using GoT for sequential problems (wasted complexity) or CoT for decomposable problems (missed parallelism) | Medium | Incorrect topology selection | Follow the decision flowchart; test with CoT first before escalating |