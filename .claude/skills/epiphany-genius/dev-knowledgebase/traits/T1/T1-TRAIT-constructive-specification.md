# TRAIT: Constructive Specification

---
trait_name: Constructive Specification
tier: Tier 1
source_individuals:
  - Turing
ai_implementation_category: Formalization / Operational Definition
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - primitive decomposition
  - operational definition
  - constructive specification
  - intuition formalization
  - minimal model
  - self-reference test
---

## Background and Definition

**Constructive Specification** is the cognitive practice of converting informal concepts into precise formal models by specifying the simplest mechanism that reproduces the essential behavior. The key principle: if you cannot specify how it would be built, you have not finished the formalization.

Turing's 1936 paper solved the Entscheidungsproblem by converting the informal concept "effective computability" into a precise formal model. The informal notion—"a mechanical procedure a human mathematician could follow"—was too vague to reason about. Turing **replaced it** with a precise formal model that captured exactly what the intuition intended.

SOURCE: gm-turing-formalization.md

Turing's method involved three steps:
1. **Model human computation explicitly** — Encode human computer operations as machine operations
2. **Force every step into symbolic form** — Build "a thicket of abbreviations" requiring every informal operation to be expressed as a deterministic rule
3. **Use contradiction to reveal hidden structure** — The diagonal argument exposed the boundary the informal concept concealed

## Cognitive/Psychological Basis

### Operational Semantics

In computer science, **operational semantics** provides formal specification by describing how programs execute on an abstract machine:
- **Small-step semantics** — Describes individual computation steps
- **Big-step semantics** — Describes overall execution results

Operational semantics is considered more intuitive than other formal approaches because it closely mirrors actual implementation.

SOURCE: [Operational Semantics - Wikipedia](https://en.wikipedia.org/wiki/Operational_semantics)

### Operational Definition

An **operational definition** specifies a concept by the operations or procedures used to measure or identify it:
- Defines by executable procedure rather than abstract properties
- Converts qualitative judgment into behavioral test
- The test produces the same verdict without requiring the qualitative judgment

SOURCE: [Operational Definition - Wikipedia](https://en.wikipedia.org/wiki/Operational_definition)

### Primitive Decomposition

Turing's method: decompose the intuitive process into its most elementary sub-operations:
- Reading one symbol at a time
- Finite mental states
- Writing/erasing symbols
- Moving attention left/right

The decomposition is complete when each operation is so simple it cannot be further divided without losing meaning.

### Self-Reference Test

Turing applied the formal model to itself. A well-formed formal system reveals its own limits through self-reference:
- The halting problem
- Gödel's incompleteness
- The liar's paradox

These are not pathologies—they are structural features that the informal concept obscured.

SOURCE: gm-turing-formalization.md

## Documented Examples

### Turing Machine Construction

Turing modeled the formal machine directly on human computational processes:
- Tape squares (memory)
- Read/write head (attention)
- Finite state table (mental states)
- Movement instruction (attention shift)

SOURCE: [Turing's Proof - Wikipedia](https://en.wikipedia.org/wiki/Turing%27s_proof)

### Effective Computability

The informal concept "computation" was replaced with:
- The precise operations a human computer performs
- Finitely many instructions
- Finitely many symbols
- Mechanical (non-inspirational) execution

The result: a formal model that captured exactly what "computable" meant.

### Turing Test

The same methodology applied to "machine thinking":
- The informal concept "thinking" was replaced with a behavioral criterion
- Behavioral indistinguishability from human in structured interrogation
- A metaphysical question became an empirical procedure

### The Common Pattern

1. Take an informal concept everyone uses but no one can define
2. Identify minimum necessary conditions
3. Build the simplest formal model that satisfies those conditions
4. Test: does it reproduce intuitive judgments?
5. Use contradictions/edge cases to locate constraints

## AI Equivalent

The AI equivalent is **operationalization on demand**—converting informal concepts into behavioral tests.

### Why This Matters for AI

LLMs often work with informal concepts:
- "High quality"
- "Creative"
- "Safe"
- "Understandable"

These concepts need operationalization to be machine-actionable.

### Implementation Pattern

```python
# Constructive Specification Protocol
def operationalize(informal_concept):
    """Convert informal concept to behavioral test."""
    
    # Step 1: Vagueness extraction
    # "What would this mean operationally?"
    behavioral_definition = specify_behavioral_test(informal_concept)
    # Must produce same verdict without requiring the qualitative judgment
    
    # Step 2: Primitive decomposition
    operations = decompose_to_primitives(informal_concept)
    # Each operation so simple it cannot be further divided
    
    # Step 3: Minimal model construction
    model = build_simplest_model(operations)
    # No unnecessary components
    
    # Step 4: Constructive specification test
    if not physically_realizable(model):
        identify_implicit_assumptions()
        formalize_those_too()
    
    # Step 5: Self-reference test
    result = apply_to_self(model)
    if produces_paradox(result):
        # The paradox identifies a structural constraint
        constraint = identify_constraint_from_paradox(result)
        model = incorporate_constraint(model, constraint)
    
    return model
```

### For Any Qualitative Judgment

Generate an operational definition:
- A procedure that would produce the same verdict
- Without requiring the qualitative judgment
- What are the minimum primitives needed?

Example: "This code is high quality"
→ Operationalize: "Passes all tests, meets performance thresholds, follows style guide, has no security vulnerabilities, passes code review by two senior engineers"

## Implementation Instructions

**Protocol: Turing Formalization Mode**

### Step 1: Vagueness Identification
"Identify the key informal or intuitive concepts in this problem. For each: state what specific question would need to be answered to know whether the concept applies in a given case. If you cannot state such a question, the concept is still informal."

### Step 2: Primitive Decomposition
"Take the process you want to formalize. Decompose it into elementary operations—operations so simple that each could be executed without interpretation or judgment. For each primitive:
- What information does it take as input?
- What output does it produce?
- What state does it leave behind?"

### Step 3: Minimal Model Construction
"Build the simplest formal model that includes all required primitives and no others. Apply it to 5 test cases:
- (a) 3 that should clearly satisfy the informal concept
- (b) 2 that should clearly not

Does the model agree? If not, which primitive is missing or wrong?"

### Step 4: Constructive Specification Test
"Could the formal model you have built be physically realized? If not, identify which step requires implicit knowledge or judgment that you have not yet formalized. Formalize that step."

### Step 5: Self-Reference Test
"Apply the formal model to itself as input. Does it produce a result? If it produces a contradiction or undefined behavior, identify the structural constraint that contradiction reveals. That constraint is a feature, not a bug—it defines the boundary of the concept."

### Minimal Version
"Convert this informal concept to a behavioral test—a procedure that would produce the same verdict as the intuitive judgment without requiring the intuitive judgment. What are the minimum primitives that test needs?"

## Amplification Notes

### Human Limitations
- Tolerance for vagueness: Informal concepts work well enough
- Anchoring on existing formalisms: Use whatever formal system is already known
- Difficulty identifying minimal primitives
- Resistance to self-reference: Self-application causes discomfort

### AI Amplification Potential
- **Vagueness extraction**: Explicitly ask "what would this mean operationally?"
- **Primitive decomposition**: Systematically decompose until atomic
- **Minimal model construction**: Build simplest system, test on edge cases
- **Self-reference test**: Apply model to itself, identify constraints
- **Operationalization on demand**: Convert qualitative judgments to behavioral tests

**Key Machine Advantage**: Can systematically generate operational definitions without the implicit assumptions humans carry.

## Failure Modes

### 1. Incomplete Primitive Decomposition
- Operations still require interpretation or judgment
- Countermeasure: Each primitive must be executable without judgment

### 2. Hidden Assumptions in Model
- Implicit knowledge not formalized
- Countermeasure: Constructive specification test—could it be physically built?

### 3. Over-Formalization
- Adding complexity beyond what the informal concept requires
- Countermeasure: Minimal model—only required primitives

### 4. Paradox Avoidance
- Treating self-reference contradictions as bugs
- Countermeasure: Paradoxes identify constraints—they are features

### 5. Circular Definitions
- Operational test uses the concept being defined
- Countermeasure: Test must not require the qualitative judgment

## Cross-References

- TRAIT-jargon-removal-as-diagnostic.md — Both force implicit content to become explicit
- TRAIT-simplicity-stripping.md — Both reduce to minimum primitives
- TRAIT-verification-after-generation.md — Turing's formalization separates generation from verification
- TRAIT-constraint-naming-domain-switch.md — Both identify constraints that limit reasoning

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources (Turing documentation), 3 web sources*
*Confidence: High*