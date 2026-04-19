# TRAIT: Jargon-Removal as Diagnostic

---
trait_name: Jargon-Removal as Diagnostic
tier: Tier 1
source_individuals:
  - Feynman (First-Principles)
ai_implementation_category: Understanding Verification / Gap Detection
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - Feynman technique
  - jargon removal
  - fluency illusion
  - conceptual understanding
  - sixth grader test
  - gap detection
---

## Background and Definition

**Jargon-Removal as Diagnostic** is the cognitive practice of stripping technical vocabulary from an explanation as a test of genuine understanding. The inability to explain a concept without using its technical terms reveals exactly where understanding breaks down.

Feynman's explicit statement:
> "I learned very early the difference between knowing the name of something and knowing something."

His bird metaphor:
> "See that bird? It's a brown-throated thrush, but in Germany it's called a Halzenfugel, and in Chinese they call it a Chung Ling and even if you know all those names for it, you still know nothing about the bird. You only know something about people."

SOURCE: gm-feynman-first-principles.md; direct Feynman quotes from fs.blog

The operational test:
> "If you have to test your knowledge about the concept of something, say energy, then without using the word 'energy', try to rephrase what you know about it in your own language."

If you cannot name-drop your way through the explanation, you must actually understand it.

## Cognitive/Psychological Basis

### The Fluency Illusion

The **fluency illusion** is a metacognitive error where the brain mistakes ease of processing for actual mastery. When material feels smooth and familiar in the moment, you assume you've learned it—when you may have only achieved surface-level recognition.

SOURCE: [Fluency Illusions: Why Students Think They Know More](http://structural-learning.com/post/fluency-illusions-students-think-think-they-know)

Key findings:
- Recognition ≠ Recall: Following along while reading is completely different from producing information from memory
- Processing Fluency: When information arrives through polished presentation, it flows with little friction; the brain uses this ease as a proxy for mastery
- Foresight Bias: During study, you have access to information that won't be available at test time

### The Core Distinction

**Naming/Recognizing (Illusion)**:
- Following along when someone else explains
- Recognizing terms when you see them again
- Feeling like material "makes sense" while reading
- Nodding along during clear lectures

**Conceptual Understanding (Reality)**:
- Explaining in your own words without aids
- Applying to novel situations not seen before
- Answering follow-up questions and handling variations
- Transferring knowledge to new contexts

SOURCE: [Illusions of Learning: Why You Think You Know It](https://scholarly.so/blog/illusions-of-learning-why-you-think-you-know-it-but-you-dont)

### Why Jargon Masks Understanding

Feynman identified that "we often talk in fact-deficient, obfuscating generalities to cover up our lack of understanding."

Technical vocabulary carries two loads:
1. **Conceptual content** — actual meaning
2. **The ability to hide absence of conceptual content** — fluency without understanding

Stripping jargon is a diagnostic test—what remains is either genuine understanding or exposed ignorance. There is no third option.

SOURCE: gm-feynman-first-principles.md

### The Teaching Attempt as Gap Detector

When you attempt to explain something at a level another person can understand, gaps in your own understanding become structurally visible. You cannot explain what you do not understand; you can only repeat formulations.

The locations where the explanation breaks down are not approximate—they are structurally precise markers of understanding boundaries.

## Documented Examples

### Feynman's Name/Concept Distinction

From childhood radio repair:
> Feynman "would analyze the issues theoretically and arrive at the solutions" — reasoning from physics principles, not applying memorized repair patterns.

He created custom mathematical notation before college because standard notation obscured the relationships he wanted to think with.

SOURCE: gm-feynman-first-principles.md; Wikipedia documentation

### The Feynman Lectures Principle

From the preface:
> "When new ideas came in, I would try either to deduce them if they were deducible or to explain that it was a new idea... and which was not supposed to be provable."

He distinguished rigorously between what follows from prior principles and what must be accepted as a new primitive.

### The Self-Test Mechanism

Feynman identified the precise failure mode:
> "One of the ways we mask our lack of understanding is by using complicated vocabulary and jargon."

The simplicity test breaks this ambiguity: explain to a sixth-grader, without technical vocabulary. The child's failure to understand is not the child's limitation—it is information about the explainer's understanding.

## AI Equivalent

The AI equivalent is **forced vocabulary prohibition** followed by **gap scanning**.

### Why This Matters for AI

LLMs can generate fluent technical explanations without understanding. The fluency illusion applies:
- Model output can feel smooth and authoritative
- Technical terms are pattern-matched, not conceptually understood
- The model can produce explanations it cannot apply

### Implementation Pattern

```python
# Jargon-Removal Diagnostic Protocol
def understanding_diagnostic(topic):
    """Test understanding by forcing jargon removal."""
    
    # Step 1: Name-stripping pass
    technical_terms = extract_technical_terms(topic)
    definitions_without_terms = {}
    
    for term in technical_terms:
        # Must define without using the term or synonyms
        definition = define_without_using(term, banned=[term, *synonyms(term)])
        if definition is None:
            definitions_without_terms[term] = "GAP: Cannot define without circular reference"
        else:
            definitions_without_terms[term] = definition
    
    # Step 2: Sixth-grader explanation
    explanation = explain_to_sixth_grader(topic, banned_vocabulary=technical_terms)
    gaps = identify_breakdown_points(explanation)
    
    # Step 3: Gap inventory
    for gap in gaps:
        # What would need to be true for explanation to hold?
        requirement = identify_requirement(gap)
        # Is this deducible or a new primitive?
        classification = classify_derived_vs_axiom(requirement)
    
    # Step 4: Derivation vs. Axiom classification
    for component in explanation.components:
        classification = classify(component)
        if classification == "DERIVED":
            derivation_path = trace_derivation(component)
        elif classification == "AXIOM":
            reason = explain_why_accepted(component)
```

### Forced Vocabulary Prohibition

The key intervention:
> Explicitly ban domain vocabulary in explanation-generation. This forces reconstruction from structure, not from pattern-matched formulations.

After generating an explanation:
> Run a second pass that asks: "At which step does this explanation assume rather than derive? Mark each assumption."

### The Sixth-Grader Test at Scale

Generate 5 explanations at progressively lower vocabulary levels:
- Level 1: Graduate student (technical terms allowed)
- Level 2: Undergraduate (some technical terms)
- Level 3: High school student (limited technical terms)
- Level 4: Middle schooler (no technical terms)
- Level 5: Elementary schooler (simple language only)

The level where the explanation dissolves is where understanding was relying on vocabulary rather than structure.

SOURCE: INFERENCE from Feynman's method; AI implementation pattern

## Implementation Instructions

**Protocol: Feynman Jargon-Removal Mode**

### Step 1: Name-Stripping Pass
"Identify every technical term in this problem or explanation. For each term, provide a definition that does not use the term itself or any synonym of it. If you cannot provide such a definition, mark the term as a 'borrowed label.'"

Prompt template:
```
## Name-Stripping Pass
Technical terms: [term1, term2, term3, ...]

For each term:
- Term: [term]
- Definition without using the term: [definition]
- Status: DEFINED | BORROWED LABEL (cannot define without circular reference)
```

### Step 2: Sixth-Grader Explanation
"Explain this concept to a sixth-grader using no technical vocabulary. Write the explanation as a narrative, not a list. If you find yourself writing a phrase you cannot explain further without using technical terms, stop and flag it as a gap."

### Step 3: Gap Inventory
"List every gap you identified in Step 2. For each gap:
- (a) State exactly what would need to be true for the explanation to hold at that point
- (b) Identify whether this is a deducible consequence of prior principles or a new primitive that must be accepted"

### Step 4: Derivation vs. Axiom Classification
"Classify each component of this explanation as either:
- **DERIVED**: Follows from prior principles—state which ones
- **AXIOM**: Accepted as a starting point—state why

Any component classified as derived must have its derivation path made explicit."

### Step 5: Reconstruction Test
"Reconstruct the full explanation using only the AXIOM-level primitives and explicit derivations. Does the original conclusion still follow? If not, identify which assumed derivation was actually a hidden axiom."

### Minimal Version
"Explain this without using any technical vocabulary. Find the first sentence where you cannot continue without borrowing a term. That is the boundary of your actual understanding."

## Amplification Notes

### Human Limitations
- Fluency illusion makes jargon feel like understanding
- Anchoring on inherited explanations
- Inability to identify exactly which step in a multi-step explanation fails
- "Feeling right" until tested

### AI Amplification Potential
- **Forced jargon prohibition**: Can be explicitly instructed to reconstruct from structure
- **Gap scanning**: After generating an explanation, can run a second pass asking "At which step does this assume rather than derive?"
- **Sixth-grader test at scale**: Can generate explanations at 5 vocabulary levels and check where structural relationships dissolve
- **Name/concept audit**: Can enumerate every technical term and demand a definition without using the term

**Key Machine Advantage**: Mechanical enforcement of vocabulary constraints surfaces understanding gaps that would feel smooth in human reasoning.

## Failure Modes

### 1. Over-Simplification
- Removing jargon can strip necessary precision
- Some technical terms capture distinctions that everyday language blurs
- Countermeasure: Flag where precision is lost, provide both simplified and precise versions

### 2. Hidden Gaps Not Exposed
- Explanation flows smoothly but still relies on borrowed formulations
- Fluency illusion operates even with jargon removed
- Countermeasure: Reconstruction test—rebuild from primitives

### 3. False Confidence from Simplicity
- "I can explain this simply" feels like understanding
- But simplification can be superficial
- Countermeasure: Application test—can you apply to novel situations?

### 4. Borrowed Analogies
- Replacing technical terms with analogies that don't map
- Analogies can obscure as much as jargon
- Countermeasure: Test analogy boundaries explicitly

### 5. Derivation Assumptions
- Assuming steps are "obvious" when they're actually axioms
- Hidden axioms pass as derived knowledge
- Countermeasure: Step 4—explicit classification of each component

## Cross-References

- TRAIT-simplicity-stripping.md — Related concept across Einstein, Feynman, Turing
- TRAIT-gap-identification-via-teaching.md — The teaching attempt as gap detector
- TRAIT-verification-after-generation.md — Verification phase includes understanding verification
- TRAIT-constructive-specification.md — Turing's requirement that formalizations be physically realizable

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources (Feynman quotes), 5 web sources*
*Confidence: High*