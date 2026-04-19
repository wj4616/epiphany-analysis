# TRAIT: Differential Effort Detection

---
trait_name: Differential Effort Detection
tier: Tier 1
source_individuals:
  - Feynman (Error-Seeking)
ai_implementation_category: Bias Mitigation / Symmetric Scrutiny
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - differential scrutiny
  - confirmation bias
  - asymmetric effort
  - evidence weighting
  - Millikan effect
  - symmetric scrutiny
---

## Background and Definition

**Differential Effort Detection** is the cognitive practice of detecting and correcting asymmetric scrutiny applied to evidence based on whether it confirms or disconfirms prior beliefs. The bias operates through effort allocation: more scrutiny to disconfirming results, less to confirming results.

Feynman's canonical example from Millikan's oil drop experiment:
> "When they got a number that was too high above Millikan's, they thought something must be wrong—and they would look for and find a reason why something might be wrong. When they got a number closer to Millikan's value they didn't look so hard."

SOURCE: gm-feynman-error-seeking.md; Cargo Cult Science speech (Caltech 1974)

The mechanism: scientists applying **more scrutiny to results that deviated from expectation** than to results that confirmed it. The asymmetry in error-seeking was the bias. Each researcher who accepted a slightly-wrong-but-not-too-wrong result anchored the next researcher's prior.

## Cognitive/Psychological Basis

### Disconfirmation Bias

Edwards and Smith (1996) documented:
- Arguments incompatible with prior beliefs are scrutinized longer
- More extensive refutational analyses applied to belief-incompatible information
- Judgment of incompatible arguments as weaker than compatible arguments
- The bias is **accentuated when prior beliefs are accompanied by emotional conviction**

SOURCE: [A Disconfirmation Bias in the Evaluation of Arguments](https://fbaum.unc.edu/teaching/articles/JSPS-1996-Edwards.pdf)

### Evidence Weighting Mechanisms

Confirmation bias operates through three cognitive components:

1. **Information search** — Seeking confirming over disconfirming information
2. **Evidence weighting** — Rating confirming information as more informative
3. **Memory recall** — Better memory for confirming information

SOURCE: [A common factor underlying individual differences in confirmation bias](https://preview-www.nature.com/articles/s41598-024-78053-7)

### The Millikan Effect Pattern

The pattern Feynman identified:
- Results **much higher than expected** → heavy scrutiny, search for errors
- Results **close to expected** → light scrutiny, acceptance
- This creates systematic directional bias in reported values
- Error accumulates in one direction across the scientific community

INFERENCE: The bias is not in what evidence is collected, but in **how hard you look at it**.

### Motivated Reasoning

Kunda (1990) documented:
- People are motivated to reach preferred conclusions while maintaining a "sense of objectivity"
- Evidence consistent with prior beliefs is accepted more readily
- Evidence quality is judged based on whether it supports or opposes existing beliefs

SOURCE: [Nickerson Review of Confirmation Bias](https://psy.ucsd.edu/~mckenzie/nickersonConfirmationBias.pdf)

## Documented Examples

### Millikan Oil Drop Data

Historical context:
- Millikan's original value was about 0.6% below the currently accepted value (1.5924×10⁻¹⁹ C vs. 1.602176634×10⁻¹⁹ C)
- Subsequent measurements gradually increased over time
- The "creep" toward the true value was not a random walk but a systematic convergence
- Scientists subconsciously filtered their own results

SOURCE: [Oil Drop Experiment - Wikipedia](https://en.wikipedia.org/wiki/Millikan_Experiment)

### Feynman's Cargo Cult Science Principle

The first principle:
> "The first principle is that you must not fool yourself—and you are the easiest person to fool."

This is not advice about fooling others. It is a calibration claim: **the primary threat to accurate belief is not external deception—it is systematic self-deception that operates invisibly during your own reasoning.**

SOURCE: [Cargo Cult Science](https://calteches.library.caltech.edu/51/2/CargoCult.htm)

### The Reporting Standard

Feynman's operationalized requirement:
> "You must report everything that you think might make it invalid—not only what you think is right about it: other causes that could possibly explain your results; and things you thought of that you've eliminated by some other experiment, and how they worked—to make sure the other fellow can tell they have been eliminated."

And: "Details that could throw doubt on your interpretation must be given, if you know them."

SOURCE: gm-feynman-error-seeking.md; direct Feynman quote

### Cargo Cult Detection

The negative example: practitioners "follow all the apparent precepts and forms of scientific investigation, but they're missing something essential"—the planes don't land. What they are missing is the **active error-seeking behavior**.

The surface structure of science (hypotheses, measurements, reports) can be reproduced without the epistemic discipline that gives it value.

## AI Equivalent

The AI equivalent is **symmetric scrutiny enforcement**—explicitly allocating equal effort to confirming and disconfirming evidence, regardless of preference.

### Why This Matters for AI

LLMs do not have beliefs they want to protect—but they experience **anchoring on recent framing**. When a conclusion is developing, evidence that contradicts it may be implicitly down-weighted.

The analog: maintain an explicit running list of disconfirming observations rather than implicitly down-weighting them when they become inconvenient.

### Implementation Pattern

```python
# Differential Effort Detection Protocol
def symmetric_scrutiny(conclusion, evidence):
    """
    Apply equal effort to confirming and disconfirming evidence.
    Detect and correct asymmetric scrutiny.
    """
    
    # Step 1: Self-deception prompt
    desired_outcome = identify_implicit_preference()
    # "What outcome am I implicitly hoping for?"
    
    # Step 2: Symmetric scrutiny pass
    confirming = [e for e in evidence if supports(e, conclusion)]
    disconfirming = [e for e in evidence if contradicts(e, conclusion)]
    
    # Apply AT LEAST as much effort to disconfirming
    for e in disconfirming:
        scrutiny_level = measure_effort_applied(e)
        # If effort < effort_applied_to(confirming), increase
    
    # Step 3: Steel-man disconfirmation
    strongest_counterarguments = generate_top_3_counterarguments(conclusion)
    for arg in strongest_counterarguments:
        if stronger_than_any_confirming(arg):
            # Don't suppress—update the conclusion
    
    # Step 4: Millikan audit
    # "If I were unconsciously motivated to reach this conclusion,
    #  what evidence would I be systematically under-weighting?"
    under_weighted = identify_under_weighted_evidence(conclusion)
    
    # Step 5: Differential effort test
    effort_confirming = measure_effort(confirming)
    effort_disconfirming = measure_effort(disconfirming)
    if asymmetric(effort_confirming, effort_disconfirming):
        rebalance_effort()
```

### Key Machine Advantage

An LLM does not have beliefs it wants to protect. It can apply disconfirmation effort **mechanically**, without the unconscious dampening that makes human error-seeking fail.

The instruction to seek errors can be executed without the motivated suppression that prevents humans from looking too hard for problems in their preferred conclusions.

## Implementation Instructions

**Protocol: Feynman Calibration Mode**

### Step 1: Self-Deception Prompt
"Before evaluating this conclusion: identify what outcome you are implicitly hoping for. State it explicitly. Now apply the Millikan test: if you were unconsciously filtering evidence toward this outcome, what would that look like? What evidence would you examine less carefully?"

### Step 2: Symmetric Scrutiny Pass
"Generate the three strongest arguments against this conclusion. Apply **at least as much effort** to each as you applied to your best argument for it. If you find a disconfirming argument stronger than any confirming argument, update the conclusion."

### Step 3: Active Disconfirmation Inventory
"List every condition under which this conclusion would be false. For each condition:
- (a) Have you checked whether it holds?
- (b) If not, why not?

Unchecked conditions are not absent conditions—they are unexamined conditions."

### Step 4: Cargo Cult Audit
"Identify the form of this conclusion vs. the substance. What would someone who was performing the appearance of reasoning—without the epistemic discipline—have produced? Does your conclusion differ from that? Where?"

### Step 5: Reporting Standard Check
"What would you need to report to give someone else a full picture of how you reached this conclusion—including the things that might make it wrong? Write that report. If there are items in it you are reluctant to include, those are the highest-priority items to check."

### Minimal Version
"List the top 3 reasons this conclusion might be wrong. For each: have you looked at it as hard as you looked for reasons it was right?"

## Amplification Notes

### Human Limitations
- Confirmation bias operates through differential memory decay
- Fluent, easy-to-construct explanations feel more correct
- Anchoring on prior estimates biases evaluation of new results
- Publication selection: results that confirm hypotheses are reported; null results are not

### AI Amplification Potential
- **Symmetric scrutiny enforcement**: Can apply equal effort mechanically
- **Steel-man disconfirmation**: Can generate strongest counterarguments
- **Millikan audit**: Can explicitly ask what evidence would be under-weighted if motivated
- **Uniform reporting**: Can require disconfirming conditions without motivated suppression

**Key Machine Advantage**: An LLM does not have beliefs it wants to protect. The instruction to seek errors can be executed without the unconscious dampening that prevents humans from looking too hard for problems in their preferred conclusions.

## Failure Modes

### 1. False Symmetry
- Applying equal scrutiny to all evidence regardless of quality
- Not all evidence deserves equal weight
- Countermeasure: Scrutiny should be symmetric by direction, not by quality

### 2. Performative Disconfirmation
- Generating weak counterarguments to satisfy the requirement
- Not genuinely seeking the strongest counterarguments
- Countermeasure: Generate top 3 strongest counterarguments, not just any counterarguments

### 3. Missing the Asymmetry
- Not detecting that scrutiny was asymmetric
- Assuming effort was equal when it wasn't
- Countermeasure: Explicit effort measurement and comparison

### 4. Motivated Suppression
- Unconsciously avoiding the strongest counterarguments
- Countermeasure: Explicit prompt for "what evidence would I be systematically under-weighting?"

### 5. Incomplete Disconfirmation Inventory
- Not listing all conditions under which conclusion would be false
- Countermeasure: Systematic enumeration before accepting

## Cross-References

- TRAIT-golden-rule-disconfirming-evidence.md — Darwin's asymmetric memory intervention addresses the same bias
- TRAIT-verification-after-generation.md — Verification requires symmetric effort
- TRAIT-cargo-cult-detection.md — Companion file: surface structure without epistemic discipline
- TRAIT-evidence-source-diversification.md — Darwin's multi-source evidence collection

---

*Trait report generated: 2026-04-09*
*Sources: 4 primary sources (Feynman quotes), 4 web sources*
*Confidence: High*