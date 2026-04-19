# Turing: Formalization of Intuition

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Figure(s):** Alan Turing
**Category:** A — Reasoning/problem-solving
**Sources fetched:** https://en.wikipedia.org/wiki/Alan_Turing · https://en.wikipedia.org/wiki/Turing%27s_proof · https://www.philocomp.net/computing/turing1936.htm
**Content quality:** High — Wikipedia confirmed Turing machine construction method and Turing test operationalization; Turing's proof article provided the diagonal argument as structural revealer; constructive method documented

## Documented Evidence

### The Central Achievement

Turing's 1936 paper "On Computable Numbers, with an Application to the Entscheidungsproblem" solved a problem that had defeated mathematicians who treated it as purely philosophical: what does it mean for something to be "effectively computable"? The intuitive notion — "a mechanical procedure a human mathematician could follow" — was too vague to reason about formally.

Turing's cognitive move: he did not argue about the intuitive concept. He **replaced it** with a precise formal model, then showed the model captured exactly what the intuition intended.

### The Formalization Method

Turing's method for converting the informal concept of "computation" to formal mathematics involved three steps, all documented:

**Step 1 — Model human computation explicitly**: Turing modeled the formal machine directly on the functional processes of a human carrying out mathematical computation. A human computer: reads one symbol at a time, has a finite number of mental states, writes or erases symbols, moves attention left or right. Turing encoded each of these primitives as machine operations: tape squares, read/write head, finite state table, movement instruction. The abstraction was grounded in concrete, physically realizable operations.

**Step 2 — Force every step into symbolic form**: He built "a thicket of abbreviations" and standardized descriptions, requiring every informal operation to be expressed as a deterministic rule. This forcing function made implicit assumptions explicit: computation requires finitely many instructions, finitely many symbols, and mechanical (non-inspirational) execution.

**Step 3 — Use contradiction to reveal hidden structure**: The diagonal argument did not merely prove impossibility — it exposed what the informal concept had concealed. When machine H attempts to compute its own number, the self-referential circularity reveals that "computable" contains a structural boundary: some operations appear computable from outside but are not computable by the system attempting them. The halting problem proof made the boundary of the concept precise.

### The Turing Test: The Same Pattern Applied to Intelligence

Turing applied the identical cognitive methodology to "machine thinking" in 1950. Rather than debating the philosophical question "can machines think?", he operationalized it:

> "A computer could be said to 'think' if a human interrogator could not tell it apart, through conversation, from a human being."

The vague concept "thinking" was replaced with a precise operational criterion: behavioral indistinguishability from a human in a structured interrogation. This converted a metaphysical debate into an empirical procedure — the same move as converting "effective computability" into a Turing machine.

### The Common Pattern

Both achievements share the same cognitive structure:
1. Take an informal concept that everyone uses but no one can rigorously define
2. Identify what the concept must include at minimum (necessary conditions)
3. Build the simplest formal model that satisfies those conditions
4. Test the model by asking whether it reproduces the intuitive judgments — and whether its formal implications align with what the informal concept implies
5. Use contradictions or edge cases to locate where the formal model reveals constraints the informal concept hid

## Cognitive Mechanism

Turing's formalization methodology operates through **intuition-to-mechanism reduction**: find the simplest mechanical system whose behavior is indistinguishable from the intuitive concept.

Three specific operations:

**1. Primitive decomposition**: Break the intuitive process into its most elementary sub-operations. For computation: reading, writing, erasing, moving, state transition. The decomposition is complete when each operation is so simple it cannot be further divided without losing meaning.

**2. Constructive specification**: Specify the mechanism concretely enough that it could be physically built. This is not mere formalism — it is a constraint that prevents hidden assumptions from surviving the formalization. If you cannot specify how it would be built, you have not finished the formalization.

**3. Diagonal/self-reference test**: Apply the formal model to itself. A well-formed formal system will reveal its own limits through self-reference — the halting problem, Gödel's incompleteness, the liar's paradox. These are not pathologies; they are structural features that the informal concept obscured. Turing used the diagonal to find and characterize the precise boundary of computability.

## AI/Machine Amplification

Human formalization of intuition is limited by:
- Tolerance for vagueness: informal concepts work well enough that the cost of formalizing them is rarely worth paying
- Anchoring on existing formalisms: people use whatever formal system they already know, rather than building a new one fitted to the concept
- Difficulty identifying which primitives are truly minimal
- Resistance to applying the mechanism to itself (self-reference causes discomfort)

An LLM can amplify each of these:
- **Vagueness extraction**: For any informal concept being used in reasoning, explicitly ask "what would this mean operationally?" — produce a behavioral definition that could be checked without interpretation
- **Primitive decomposition**: Take any informal process and systematically decompose it until each step is atomic — verifiable without further interpretation
- **Minimal model construction**: Build the smallest formal system that reproduces the essential behavior of the informal concept. Then test it: does it capture the easy cases? Does it handle the edge cases? Does it produce contradictions at the right places?
- **Self-reference test**: Apply the formal model to itself. If it produces a paradox, that paradox identifies a structural constraint of the concept. If it runs without paradox, the formalization may be incomplete.
- **Operationalization on demand**: For any qualitative judgment ("this is high quality," "this is creative"), generate an operational definition — a behavioral test that would produce the same verdict without requiring the qualitative judgment

## Skill Mapping

**Protocol: Turing Formalization Mode**

1. **Vagueness identification**: "Identify the key informal or intuitive concepts in this problem. For each: state what specific question would need to be answered to know whether the concept applies in a given case. If you cannot state such a question, the concept is still informal."

2. **Primitive decomposition**: "Take the process you want to formalize. Decompose it into elementary operations — operations so simple that each could be executed without interpretation or judgment. For each primitive: what information does it take as input? What output does it produce? What state does it leave behind?"

3. **Minimal model construction**: "Build the simplest formal model that includes all required primitives and no others. Apply it to 5 test cases: (a) 3 that should clearly satisfy the informal concept, (b) 2 that should clearly not. Does the model agree? If not, which primitive is missing or wrong?"

4. **Constructive specification test**: "Could the formal model you have built be physically realized? If not, identify which step requires implicit knowledge or judgment that you have not yet formalized. Formalize that step."

5. **Self-reference test**: "Apply the formal model to itself as input. Does it produce a result? If it produces a contradiction or undefined behavior, identify the structural constraint that contradiction reveals. That constraint is a feature, not a bug — it defines the boundary of the concept."

**Trigger condition**: Use Turing mode when: (a) a problem contains informal concepts that everyone uses but no one has defined precisely, (b) two people disagree because they are using the same term to mean different things, or (c) a solution requires a judgment that cannot be made algorithmically without first specifying the judgment's formal basis.

**Minimal version**: "Convert this informal concept to a behavioral test — a procedure that would produce the same verdict as the intuitive judgment without requiring the intuitive judgment. What are the minimum primitives that test needs?"

## Related KB Entries

- `research-abductive-reasoning.md` — Peirce's falsifiability constraint (hypotheses must specify conditions under which they are wrong) is a partial operationalization of the same move: forcing informal claims to specify their test conditions
- `gm-feynman-first-principles.md` — Feynman's jargon-removal test and Turing's formalization are parallel: both force implicit content to become explicit by demanding that the concept work without borrowed vocabulary
- `research-representational-change.md` — Re-encoding (Ohlsson's third mechanism) is the cognitive operation underlying Turing's move: replacing one representation of a concept with another that makes different features visible
