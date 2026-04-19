# TRAIT: Evidence Source Diversification

---
trait_name: Evidence Source Diversification
tier: Tier 1
source_individuals:
  - Darwin
ai_implementation_category: Evidence Integration / Triangulation
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - evidence triangulation
  - multiple sources
  - converging evidence
  - independent corroboration
  - source diversity
  - bias independence
---

## Background and Definition

**Evidence Source Diversification** is the cognitive practice of seeking evidence from multiple independent sources with different bias profiles, then assessing whether they converge on the same conclusion. The key principle: when different approaches with different weaknesses point toward the same conclusion, that conclusion is more credible than if only one approach supports it.

Darwin's explicit methodology:
> "In considering the Origin of Species, it is quite conceivable that a naturalist, reflecting on the mutual affinities of organic beings, on their embryological relations, their geographical distribution, geological succession, and other such facts, might come to the conclusion that each species had not been independently created, but had descended, like varieties, from other species."

SOURCE: gm-darwin-evidence-accumulation.md; [On the Origin of Species Introduction](https://en.wikisource.org/wiki/On_the_Origin_of_Species_(1859)/Introduction)

Darwin's patient accumulation:
> "It occurred to me, in 1837, that something might perhaps be made out on this question by patiently accumulating and reflecting on all sorts of facts which could possibly have any bearing on it. After five years' work I allowed myself to speculate..."

Twenty years of evidence gathering from multiple independent sources before publication.

SOURCE: [Darwin Online](https://darwin-online.org.uk/Variorum/1859/1859-459-c-1861.html)

## Cognitive/Psychological Basis

### Evidence Triangulation

Kuorikoski & Marchionni (2016):
- Triangulation uses multiple independent sources to reason from data to phenomenon
- Epistemic value lies in controlling for likely errors and biases in particular data-generating procedures
- Addresses objections concerning fallibility, scope, independence, and discordance of evidence

SOURCE: [Evidential Diversity and Triangulation](https://www.cambridge.org/core/journals/philosophy-of-science/article/abs/evidential-diversity-and-the-triangulation-of-phenomena/53FA073FD9560A4F07230E7C4D8E19C7)

### Bias Independence Principle

Gutierrez, Glymour, & Davey Smith (2025):
- Key principle: identify the most important weaknesses for each study approach
- Then seek new evidence sources that don't share those weaknesses
- When results are consistent across studies resting on different assumptions with unrelated biases, conclusions are much more robust

SOURCE: [Evidence Triangulation in Health Research](https://link.springer.com/article/10.1007/s10654-024-01194-6)

### Convergent Evidence

When different methods with different potential biases converge:
- Systematic errors in one method are unlikely to affect others
- Random errors average across methods
- The probability of all methods being wrong in the same direction decreases
- Conclusions become more robust

### The Hypothetico-Deductive Method

Darwin's actual methodology:
1. Formulate hypothesis
2. Derive predictions from it
3. Test predictions against observations from multiple sources
4. Seek out evidence that could potentially falsify the theory

SOURCE: [Darwin and the Scientific Method](https://pmc.ncbi.nlm.nih.gov/articles/PMC2702794/)

## Documented Examples

### Darwin's Multiple Lines of Evidence

From his barnacle research (1846-1854) and other work, Darwin integrated:

| Evidence Type | Example |
|--------------|---------|
| Comparative anatomy | Homology of cement glands with ovarian tubes |
| Embryology | Larval stages showing crustacean affinity |
| Variation studies | Extensive variation within species |
| Reproductive systems | Complemental males showing sexual transitions |
| Geographical distribution | Biogeography patterns |
| Geological succession | Fossil record evidence |

SOURCE: [Darwin's Study of the Cirripedia](https://darwin-online.org.uk/EditorialIntroductions/Richmond_cirripedia.html)

### The Barnacle Research

Darwin's 8-year study of barnacles:
- Originally intended to describe a single anomalous species
- Expanded to study the entire subclass
- Discovered complemental males—minute males parasitic on hermaphrodites
- This corroboration across sources showed transition states in nature

### Darwin's Method

> "The line of argument often pursued throughout my theory is to establish a point as a probability by induction and to apply it as a hypothesis to other parts and see whether it will solve them."

Testing predictions against multiple independent sources.

### Modern Applications

Health research triangulation:
- Randomized controlled trials
- Observational studies
- Mendelian randomization
- Instrumental variables
- Negative controls
- Cross-context comparisons

Each approach has different bias profiles; convergence strengthens conclusions.

## AI Equivalent

The AI equivalent is **multi-source evidence integration with bias diversity analysis**.

### Why This Matters for AI

LLMs typically reason from available information without:
- Explicitly seeking sources with different bias profiles
- Assessing whether sources are independent
- Checking for convergence across diverse evidence types

### Implementation Pattern

```python
# Evidence Source Diversification Protocol
def triangulate_evidence(hypothesis):
    """
    Seek evidence from multiple independent sources with different bias profiles.
    """
    
    # Step 1: Source type identification
    source_types = identify_evidence_types(hypothesis)
    # Examples: experimental, observational, theoretical, historical, expert opinion
    
    # Step 2: Bias profile analysis
    bias_profiles = {}
    for source_type in source_types:
        bias_profiles[source_type] = {
            "primary_biases": identify_primary_biases(source_type),
            "direction_bias": identify_direction_bias(source_type),
            "independence_from_other_sources": assess_independence(source_type, source_types)
        }
    
    # Step 3: Evidence collection from each type
    evidence = {}
    for source_type in source_types:
        evidence[source_type] = collect_evidence(hypothesis, source_type)
    
    # Step 4: Convergence assessment
    conclusions = {}
    for source_type, ev in evidence.items():
        conclusions[source_type] = assess_conclusion(hypothesis, ev)
    
    # Step 5: Consistency check
    consistent = all_same_direction(conclusions)
    
    # Step 6: Bias independence verification
    # Are the sources actually independent?
    independence_matrix = verify_independence(bias_profiles)
    
    # Step 7: Weighted convergence
    if consistent and independence_matrix.all_independent():
        confidence = "HIGH - multiple independent sources converge"
    elif consistent and not independence_matrix.all_independent():
        confidence = "MODERATE - sources may share biases"
    else:
        confidence = "LOW - sources disagree"
    
    return {
        "hypothesis": hypothesis,
        "source_types": source_types,
        "bias_profiles": bias_profiles,
        "evidence": evidence,
        "conclusions": conclusions,
        "consistent": consistent,
        "confidence": confidence
    }
```

### Bias Independence Matrix

```python
def verify_independence(bias_profiles):
    """
    Check whether sources actually have independent biases.
    """
    independence_matrix = []
    for type_a in bias_profiles:
        row = []
        for type_b in bias_profiles:
            if type_a == type_b:
                row.append(False)  # Same source type
            else:
                # Are primary biases overlapping?
                biases_a = set(bias_profiles[type_a]["primary_biases"])
                biases_b = set(bias_profiles[type_b]["primary_biases"])
                overlap = biases_a.intersection(biases_b)
                row.append(len(overlap) == 0)  # True if no overlap
        independence_matrix.append(row)
    
    return IndependenceMatrix(independence_matrix)
```

## Implementation Instructions

**Protocol: Evidence Source Diversification Mode**

### Step 1: Source Type Identification
"What types of evidence could bear on this question? List all source types:
- Experimental evidence
- Observational evidence
- Historical evidence
- Theoretical evidence
- Expert opinion
- Statistical evidence
- Comparative evidence"

### Step 2: Bias Profile Analysis
"For each source type, identify:
- What are the primary biases of this evidence type?
- What direction would those biases push conclusions?
- What weaknesses does this evidence type have?"

### Step 3: Independence Verification
"Are these source types independent? Do they share:
- The same data sources?
- The same methodological biases?
- The same assumptions?
- The same systematic errors?"

### Step 4: Evidence Collection
"Collect evidence from each source type. What does each type tell us about the hypothesis?"

### Step 5: Convergence Assessment
"Do all source types point in the same direction?
- If yes: What is the probability that different biases would produce the same direction?
- If no: Which sources disagree? Why might they disagree?"

### Step 6: Confidence Rating
"Based on convergence and independence:
- HIGH: Multiple independent sources converge
- MODERATE: Sources converge but may share biases
- LOW: Sources disagree or share significant biases"

### Minimal Version
"What evidence sources would have different bias profiles? Do they all point in the same direction?"

## Amplification Notes

### Human Limitations
- Availability bias: Easily accessible evidence overrepresented
- Confirmation bias: Seek confirming evidence preferentially
- Single-source reliance: Trust familiar sources
- Independence assumption: Assume sources independent when they're not

### AI Amplification Potential
- **Multi-source search**: Can systematically search diverse source types
- **Bias profile analysis**: Can identify and compare bias profiles across sources
- **Independence verification**: Can check whether sources share data, methodology, assumptions
- **Convergence assessment**: Can assess whether diverse sources converge

**Key Machine Advantage**: Machines don't have convenience preferences. They can systematically seek sources with maximally different bias profiles.

## Failure Modes

### 1. False Independence Assumption
- Assuming sources independent when they share biases
- Countermeasure: Explicit independence verification

### 2. Availability Bias
- Using easily accessible sources preferentially
- Countermeasure: Systematic source type enumeration before collection

### 3. Convergence Without Independence
- Multiple sources converge but share same bias
- Countermeasure: Bias profile analysis before convergence assessment

### 4. Discorinance Ignored
- Sources disagree but treated as converging
- Countermeasure: Explicit direction assessment

### 5. Single Source Overweighting
- One source type dominates reasoning
- Countermeasure: Minimum source type count requirement

## Cross-References

- TRAIT-golden-rule-disconfirming-evidence.md — Darwin's asymmetric memory for disconfirming evidence
- TRAIT-differential-effort-detection.md — Symmetric scrutiny across sources
- TRAIT-contradiction-seeking.md — Seeking disconfirming evidence
- TRAIT-verification-after-generation.md — Verification from multiple angles

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources (Darwin documentation), 4 web sources*
*Confidence: High*