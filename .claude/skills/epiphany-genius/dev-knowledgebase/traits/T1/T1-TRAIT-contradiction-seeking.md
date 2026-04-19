# TRAIT: Contradiction-Seeking

---
trait_name: Contradiction-Seeking
tier: Tier 1
source_individuals:
  - Einstein
  - Feynman (Error-Seeking)
ai_implementation_category: Falsification / Disconfirmation Testing
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - falsification
  - disconfirmation
  - contradiction detection
  - hypothesis testing
  - active error-seeking
  - Popperian method
---

## Background and Definition

**Contradiction-Seeking** is the cognitive practice of actively searching for evidence and scenarios that would disprove a hypothesis, rather than evidence that would confirm it. The key insight: a single contradiction is more informative than any amount of confirmation.

Feynman's canonical principle:
> "The first principle is that you must not fool yourself—and you are the easiest person to fool."

And his operational definition of scientific integrity:
> "Details that could throw doubt on your interpretation must be given, if you know them. You must report everything that you think might make it invalid."

SOURCE: gm-feynman-error-seeking.md; [Feynman on Scientific Method](https://course-building.s3-us-west-2.amazonaws.com/Chemistry/transcripts/RichardFeynmanOnScientificMethod1964_transcript.txt)

Einstein's practice:
> Einstein constructed thought experiments specifically designed to produce contradictions. The light beam paradox—"what would I see if I traveled alongside a light beam?"—generated a contradiction with Maxwell's equations that "contained the germ of special relativity."

The contradiction was not an obstacle—it was the signal pointing toward new structure.

SOURCE: gm-einstein-thought-experiments.md; [Einstein's Most Famous Thought Experiment](https://sites.pitt.edu/~jdnorton/Goodies/Chasing_the_light/)

## Cognitive/Psychological Basis

### Confirmation Bias

The Wason Selection Task (1966):
- ~96% of participants choose cards that **confirm** rather than **falsify** rules
- People test hypotheses by seeking confirmation, not disconfirmation
- This is systematic, not random error

SOURCE: [Distinguish Validation from Confirmation](https://howtothink.ai/learn/distinguish-validation-from-confirmation)

### Klayman's Positive Test Strategy

The pattern is better understood as **"positive test strategy"**:
- People test cases expected to have the property of interest
- This strategy can be effective under realistic conditions
- But it leads to systematic errors when property is rare or alternatives exist

SOURCE: [Confirmation, disconfirmation, and information in hypothesis testing](https://colab.ws/articles/10.1037%2F0033-295X.94.2.211)

### Popper's Asymmetry

Karl Popper identified the fundamental asymmetry:
- No amount of confirmation proves a universal claim
- But a single counterexample can refute it
- Therefore: **falsification is more informative than confirmation**

SOURCE: [Invalidation is More Informative Than Validation](https://howtothink.ai/learn/invalidation-is-more-informative-than-validation)

### Cognitive Dissonance and Avoidance

Festinger's Cognitive Dissonance Theory (1957):
- Inconsistent cognitions create aversive discomfort
- People reduce dissonance through: attitude change, behavior change, adding consonant cognitions, trivialization, denial, distraction
- The discomfort explains why people **avoid** contradiction rather than seek it

SOURCE: [Cognitive Dissonance Theory](https://open.ncl.ac.uk/theory-library/cognitive-dissonance-theory.pdf)

### The Productive Path: Accommodation

Piaget's distinction:
- **Assimilation**: Force new information into existing schema
- **Accommodation**: Restructure schema to incorporate contradiction
- Accommodation is the productive response—but requires holding discomfort

## Documented Examples

### Feynman's Error-Seeking Protocol

From his 1964 lecture:
> "If it disagrees with experiment, it's wrong. And that simple statement is the key to science. It doesn't make a difference how beautiful your guess is... if it disagrees with experiment, it's wrong."

And: "You can always prove any definite theory wrong. Notice, however, we never prove it right."

Feynman's Cargo Cult Science principle:
> "You must report everything that you think might make it invalid—not only what you think is right about it."

SOURCE: [Cargo Cult Science](https://calteches.library.caltech.edu/51/2/CargoCult.htm)

### The Millikan Effect

Feynman's canonical example:
> "When they got a number that was too high above Millikan's, they thought something must be wrong—and they would look for and find a reason why something might be wrong. When they got a number closer to Millikan's value they didn't look so hard."

This asymmetric scrutiny—the **contradiction avoidance**—delayed convergence to the true value by decades.

SOURCE: gm-feynman-error-seeking.md

### Einstein's Contradiction Construction

Einstein's method:
1. Construct a scenario
2. Apply all accepted principles
3. Identify where they predict different results
4. The contradiction is the signal—not an obstacle

The light beam thought experiment:
- Maxwell's equations predict waves must propagate
- A frozen wave contradicts both experience and equations
- This contradiction pointed toward the need for relativity

SOURCE: [Twin Paradox - Wikipedia](https://en.wikipedia.org/wiki/Twin_paradox)

### Strong Inference (Platt, 1964)

John Platt's method:
1. Devise alternative hypotheses
2. Design crucial experiments that **exclude** hypotheses
3. Perform the experiment
4. Refine remaining hypotheses
5. Repeat

The key: design experiments to **exclude**, not confirm.

SOURCE: [Seek Disconfirming Evidence](https://howtothink.ai/learn/seek-disconfirming-evidence)

## AI Equivalent

The AI equivalent is **systematic falsification testing**—explicitly generating and testing disconfirming scenarios.

### Why This Matters for AI

LLMs experience **anchoring on recent framing**:
- Once a conclusion develops, contradictory evidence may be implicitly down-weighted
- The model doesn't have beliefs to protect—but it has developing narratives
- Contradiction-seeking must be explicitly requested

### Implementation Pattern

```python
# Contradiction-Seeking Protocol
def active_falsification(hypothesis):
    """
    Actively seek contradictions rather than confirmations.
    """
    
    # Step 1: Explicit preference check
    desired_outcome = identify_implicit_preference()
    # "What outcome am I implicitly hoping for?"
    
    # Step 2: Disconfirming scenario generation
    # NOT "How could this be true?"
    # BUT "What would make this FALSE?"
    disconfirming_conditions = []
    for assumption in extract_assumptions(hypothesis):
        condition = generate_condition_where(assumption, value=FALSE)
        disconfirming_conditions.append(condition)
    
    # Step 3: Crucial experiment design
    # Design experiments that EXCLUDE hypotheses
    for condition in disconfirming_conditions:
        if test_is_feasible(condition):
            result = test(condition)
            if result == FALSE:
                # Hypothesis excluded
                return HYPOTHESIS_REFUTED, condition
    
    # Step 4: Alternative hypothesis generation
    alternatives = generate_alternative_hypotheses(hypothesis)
    for alt in alternatives:
        # Design experiment that distinguishes between original and alternative
        crucial_test = design_crucial_experiment(hypothesis, alt)
        result = test(crucial_test)
        if result favors alt:
            return HYPOTHESIS_WEAKENED, alt
    
    # Step 5: Reporting standard
    # Report everything that could invalidate, not just what confirms
    return {
        "hypothesis": hypothesis,
        "disconfirming_conditions_tested": disconfirming_conditions,
        "alternative_hypotheses": alternatives,
        "remaining_uncertainty": identify_untested_conditions()
    }
```

### Key Methodological Interventions

1. **Flip the question**: Not "How can I prove X?" but "What would disprove X?"
2. **Generate alternatives**: The most disconfirming test is one that would exclude the hypothesis
3. **Crucial experiments**: Design tests that distinguish between hypothesis and alternatives
4. **Full reporting**: Include everything that could invalidate, not just confirming evidence

## Implementation Instructions

**Protocol: Contradiction-Seeking Mode**

### Step 1: Preference Check
"Before testing this hypothesis: identify what outcome you are implicitly hoping for. State it explicitly. The preference creates bias—name it first."

### Step 2: Disconfirming Condition Generation
"List every condition under which this hypothesis would be false. For each condition:
- (a) Have you checked whether it holds?
- (b) If not, why not?

Unchecked conditions are not absent conditions—they are unexamined conditions."

### Step 3: Crucial Experiment Design
"Design an experiment that would exclude this hypothesis:
- What result would prove this hypothesis FALSE?
- How can you test for that result specifically?
- What alternative hypotheses would remain if this one were excluded?"

### Step 4: Alternative Hypothesis Generation
"Generate at least 3 alternative hypotheses that could explain the same observations. For each alternative:
- What observation would distinguish this alternative from the original hypothesis?
- Can you test for that distinguishing observation?"

### Step 5: Full Reporting
"Report everything that could throw doubt on your interpretation:
- What evidence would you need to see to change your conclusion?
- What evidence did you see that you're not mentioning?
- What conditions haven't you tested?"

### Minimal Version
"What's the single observation that would prove this hypothesis wrong? Have you tested for that specifically?"

## Amplification Notes

### Human Limitations
- Confirmation bias is automatic and unconscious
- Discomfort from cognitive dissonance motivates avoidance
- "Positive test strategy" is the default mode
- Publication bias suppresses negative results

### AI Amplification Potential
- **No beliefs to protect**: The instruction to seek contradictions can be executed without motivated suppression
- **Systematic disconfirmation**: Can enumerate and test disconfirming conditions mechanically
- **Alternative generation**: Can generate multiple alternative hypotheses without attachment to the first
- **Full reporting**: Can report disconfirming conditions without the motivated omission humans display

**Key Machine Advantage**: The discomfort that prevents humans from looking for contradictions does not apply. The instruction to falsify can be followed without resistance.

## Failure Modes

### 1. Performative Disconfirmation
- Generating weak counterarguments to satisfy the requirement
- Not genuinely seeking the strongest counterarguments
- Countermeasure: Generate top 3 strongest counterarguments, not just any counterarguments

### 2. Asymmetric Effort
- Applying more effort to confirming than disconfirming
- Countermeasure: Measure and compare effort applied

### 3. Alternative Hypothesis Avoidance
- Not generating genuine alternatives
- Countermeasure: Require minimum 3 alternative hypotheses

### 4. Incomplete Disconfirming Inventory
- Missing conditions under which hypothesis would fail
- Countermeasure: Systematic enumeration before accepting

### 5. Motivated Reporting
- Omitting disconfirming evidence from reports
- Countermeasure: Explicit "full reporting" requirement

## Cross-References

- TRAIT-differential-effort-detection.md — Companion: detecting asymmetric scrutiny
- TRAIT-verification-after-generation.md — Verification phase includes active falsification
- TRAIT-golden-rule-disconfirming-evidence.md — Darwin's asymmetric memory intervention
- TRAIT-extreme-scenario-construction.md — Extremes as disconfirming scenarios

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources (Einstein, Feynman documentation), 5 web sources*
*Confidence: High*