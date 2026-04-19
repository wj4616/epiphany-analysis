# TRAIT: Simplicity Stripping

---
trait_name: Simplicity Stripping
tier: Tier 1
source_individuals:
  - Einstein
  - Feynman (First-Principles)
  - Turing
ai_implementation_category: Model Selection / Complexity Reduction
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - parsimony
  - Occam's razor
  - first principles
  - minimal model
  - complexity reduction
  - unnecessary components
---

## Background and Definition

**Simplicity Stripping** is the cognitive practice of systematically removing unnecessary components from explanations, models, or theories until only irreducible elements remain. The principle: strip away everything that can be removed without losing predictive power.

Einstein's formulation:
> "The supreme goal of all theory is to make the irreducible basic elements as simple and as few as possible without having to surrender the adequate representation of a single datum of experience."

SOURCE: [Einstein's 1933 Herbert Spencer Lecture](https://quoteinvestigator.com/2011/05/13/Einstein-Simple/)

Feynman's approach:
> "The first principle is that you must not fool yourself—and you are the easiest person to fool."

He distinguished between **"knowing the name of something and knowing something"**—stripping away terminology to reveal whether understanding exists beneath.

SOURCE: gm-feynman-first-principles.md; [Feynman Technique](https://fs.blog/first-principles)

Turing's method:
> The Turing machine strips computation to its irreducible primitives: read symbol, write symbol, move left/right, change state. No unnecessary operations. This minimal model proved equivalent to any more complex formulation.

SOURCE: gm-turing-formalization.md

## Cognitive/Psychological Basis

### Occam's Razor and Parsimony

The principle of parsimony (Occam's Razor) recommends:
> "Entities should not be multiplied beyond necessity."

Historical roots:
- **Aristotle**: "Nature operates in the shortest way possible"
- **Newton**: "We are to admit no more causes of natural things than such as are both true and sufficient to explain their appearances"

SOURCE: [Occam's Razor - Wikipedia](https://en.wikipedia.org/wiki/Occam%27s_razor)

### Minimum Description Length (MDL)

The MDL principle formalizes parsimony:
- **Model complexity**: Cost of describing the hypothesis
- **Data fit**: Cost of describing the data given the hypothesis
- **Optimal model**: Minimizes the sum of both costs

SOURCE: [A Compression Perspective on Simplicity Bias](https://doi.org/10.48550/arXiv.2603.25839)

### Simplicity Bias in Learning

Research shows:
- Deep neural networks naturally favor low-complexity solutions
- Real-world datasets are highly compressible (low Kolmogorov complexity)
- Better compression leads to better generalization
- Simplicity bias explains why universal learners work well on structured problems

SOURCE: [Kolmogorov Complexity and Deep Learning](https://arxiv.org/abs/2509.22445)

### Minimal Model Explanations

Scientific explanation research identifies minimal models as explanations:
- Minimal models explain by showing **which features are irrelevant**
- They demonstrate **universality**—why diverse systems show identical macro-behavior
- Only dimensionality and order parameter structure matter; microphysical constitution doesn't

SOURCE: [Minimal Model Explanations](https://www.cambridge.org/core/journals/philosophy-of-science/article/abs/minimal-model-explanations/640580F4EA5571EA1AA971C5DC063FA6)

## Documented Examples

### Einstein's Theory Construction

Einstein's special relativity:
- **Before**: Lorentz's theory required ether—unobserved entity
- **Einstein's stripping**: Remove ether, keep Lorentz transformations
- **Result**: Same predictions, simpler ontology

The ether was "shaved away" as unnecessary.

SOURCE: [Occam's Razor - Wikipedia](https://en.wikipedia.org/wiki/Occam%27s_razor)

### Feynman's First-Principles Approach

Feynman's learning method:
1. Choose concept to understand
2. Explain in simple terms (strip jargon)
3. Identify gaps where explanation breaks down
4. Review and refine until explainable from fundamentals

The stripping reveals:
- What you actually understand
- What you're merely parroting
- Where mental models have holes

SOURCE: [Feynman Technique](https://www.funblocks.net/thinking-matters/classic-mental-models/feynman-technique)

### Turing's Minimal Model

Turing's 1936 paper reduced computation to:
- Read one symbol
- Write one symbol
- Move left or right
- Change to one of finitely many states

Four primitive operations—no more. This minimal model:
- Captured exactly what "computable" means
- Proved equivalent to any more complex formulation
- Revealed uncomputability through diagonal argument

SOURCE: [Turing Machines - Stanford Encyclopedia](https://plato.stanford.edu/entries/turing-machine)

### The Common Pattern

1. **Identify all components** in the current model/explanation
2. **Test each for necessity**: What would change if removed?
3. **Remove the unnecessary** one at a time
4. **Verify adequacy**: Does stripped model still represent all data?
5. **Stop when**: Any further removal would sacrifice explanatory power

## AI Equivalent

The AI equivalent is **complexity-constrained model selection** followed by **necessity verification**.

### Why This Matters for AI

LLMs can generate complex explanations when simpler ones suffice:
- Technical vocabulary hides confusion
- Multiple components can obscure core mechanism
- Complexity is easy; simplicity requires active stripping

### Implementation Pattern

```python
# Simplicity Stripping Protocol
def strip_to_necessity(model, observations):
    """
    Remove unnecessary components until only irreducible elements remain.
    """
    
    # Step 1: Component inventory
    components = enumerate_all_components(model)
    
    # Step 2: Necessity testing
    necessary_components = []
    for component in components:
        # Remove component
        stripped_model = model_without(model, component)
        
        # Test: does stripped model still explain observations?
        if not explains(stripped_model, observations):
            necessary_components.append(component)
            # Component cannot be removed without losing predictive power
    
    # Step 3: Build minimal model
    minimal_model = build_model(necessary_components)
    
    # Step 4: Verify irreducibility
    for component in necessary_components:
        # Test if any component is actually composite
        if is_composite(component):
            sub_components = decompose(component)
            # Recursively strip each sub-component
            minimal_sub = strip_to_necessity(component, observations)
            replace_in(minimal_model, component, minimal_sub)
    
    # Step 5: Simplicity test
    # "Can you explain this without the technical terms?"
    plain_language_explanation = translate_to_plain(minimal_model)
    gaps = identify_gaps(plain_language_explanation)
    
    return minimal_model, gaps
```

### MDL-Based Selection

```python
def minimum_description_length(models, data):
    """
    Select model that minimizes: description_length(model) + description_length(data|model)
    """
    best_model = None
    best_total = float('inf')
    
    for model in models:
        model_cost = kolmogorov_complexity(model)
        data_cost = encoding_length(data, model)
        total = model_cost + data_cost
        
        if total < best_total:
            best_total = total
            best_model = model
    
    return best_model
```

### The Feynman Test at Scale

Generate explanations at multiple complexity levels:
- Level 5: Full technical detail
- Level 4: Standard technical explanation
- Level 3: Simplified technical
- Level 2: Non-technical
- Level 1: Sixth-grader explanation

The level where explanation becomes impossible reveals where complexity was hiding confusion.

## Implementation Instructions

**Protocol: Simplicity Stripping Mode**

### Step 1: Component Inventory
"List every component of this explanation or model. For each component, ask:
- What would change if this were removed?
- Is this component necessary for predictive power?
- Is this component a primitive or can it be further decomposed?"

### Step 2: Necessity Testing
"For each component, construct a version without it. Test:
- Does the stripped version still explain all observations?
- If yes, the component was unnecessary—remove it
- If no, what specific observation is lost?
- That observation defines the component's necessity"

### Step 3: Primitive Decomposition
"For each remaining component, ask:
- Can this be decomposed into simpler parts?
- What are the minimum primitives needed to reconstruct this?
- Continue decomposition until each primitive is irreducible"

### Step 4: Verification Test
"The stripped model must:
- (a) Explain every observation the original explained
- (b) Contain no component that can be removed without losing (a)
- (c) Express each component in terms that cannot be further simplified"

### Step 5: The Feynman Test
"Can you explain this stripped model without using technical vocabulary? If you find yourself reaching for jargon, that's where understanding is incomplete. The jargon isn't the problem—the gap beneath it is."

### Minimal Version
"What can you remove from this explanation? Keep removing until further removal would lose information. That's the stripped model."

## Amplification Notes

### Human Limitations
- Complexity feels sophisticated—simplicity feels reductive
- Technical vocabulary masks understanding gaps
- Multiple components provide false confidence
- "That's how it's always been done" preserves unnecessary elements

### AI Amplification Potential
- **Systematic component testing**: Can enumerate and test each component
- **MDL optimization**: Can formally minimize description length
- **Jargon prohibition**: Can enforce plain language requirements
- **Complexity measurement**: Can quantify model complexity via Kolmogorov approximation

**Key Machine Advantage**: Machines don't feel sophisticated when adding complexity—they can mechanically test whether each addition improves predictive power.

## Failure Modes

### 1. Over-Simplification
- Removing components that do contribute predictive power
- Countermeasure: Necessity testing must verify all observations still explained

### 2. Premature Stopping
- Stopping before reaching true primitives
- Countermeasure: Primitive decomposition step continues until irreducible

### 3. Jargon as Simplicity
- Technical vocabulary feels simple to experts
- Countermeasure: Feynman test at sixth-grader level

### 4. False Primitives
- Declaring components primitive when they can be decomposed
- Countermeasure: Explicit decomposition check for each component

### 5. Complexity Aversion
- Avoiding necessary complexity when nature is complex
- Countermeasure: "As simple as possible, but not simpler"—stop at adequacy, not simplicity for its own sake

## Cross-References

- TRAIT-jargon-removal-as-diagnostic.md — Companion: stripping vocabulary to test understanding
- TRAIT-constructive-specification.md — Turing's primitive decomposition
- TRAIT-extreme-scenario-construction.md — Testing stripped model at boundaries
- TRAIT-verification-after-generation.md — Verify stripped model still explains observations

---

*Trait report generated: 2026-04-09*
*Sources: 4 primary sources (Einstein, Feynman, Turing documentation), 5 web sources*
*Confidence: High*