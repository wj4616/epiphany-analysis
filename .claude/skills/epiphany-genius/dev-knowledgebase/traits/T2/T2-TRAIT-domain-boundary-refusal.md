# TRAIT: Domain Boundary Refusal

---
trait_name: Domain Boundary Refusal
tier: Tier 2
source_individuals:
  - Newton
ai_implementation_category: Epistemic Boundary Setting / Scope Limitation
replication_feasibility: 5
utility_score: 4
primary_keywords:
  - hypotheses non fingo
  - epistemic boundaries
  - domain scope
  - speculation limit
  - experimental philosophy
  - phenomenological grounding
---

## Background and Definition

**Domain Boundary Refusal** is the cognitive practice of explicitly refusing to speculate beyond what can be grounded in evidence—setting epistemic boundaries that distinguish legitimate inquiry from unfounded speculation. The key principle: claim only what phenomena support, refusing to "feign hypotheses" for unobservable causes.

Newton's canonical formulation:
> "I have not as yet been able to discover the reason for these properties of gravity from phenomena, and I do not feign hypotheses. For whatever is not deduced from the phenomena must be called a hypothesis; and hypotheses, whether metaphysical or physical, or based on occult qualities, or mechanical, have no place in experimental philosophy."

SOURCE: gm-newton-analogical-transfer.md; [Hypotheses non fingo](https://en.wikipedia.org/wiki/Hypotheses_non_fingo)

The principle established:
- **Inside domain**: What can be deduced from phenomena and confirmed through experiment
- **Outside domain**: Speculative hypotheses about underlying causes, metaphysical explanations

Newton specifically applied this to gravity—he could describe its mathematical laws but refused to speculate on *what gravity is*.

## Cognitive/Psychological Basis

### Epistemic Humility

Newton's position reflects:
- Recognition of knowledge limits
- Distinction between description and explanation
- Rejection of unverifiable speculation
- Commitment to phenomenological grounding

SOURCE: [Philosophy Now - Hypotheses Non Fingo](http://www.philosophynow.org/issues/88/Hypotheses_Non_Fingo)

### The Translation Nuance

The Latin "fingo" carries connotations:
- "Frame" (construct)
- "Feign" (pretend/fabricate)
- "Fashion" (shape)

Newton rejected **fabricated or unverifiable speculations**, not all theoretical thinking.

### Newton's Rules of Reasoning

Four principles guiding domain boundaries:

1. **Parsimony** — Admit no more causes than necessary
2. **Uniformity** — Same effects → same causes
3. **Induction** — Qualities found in experiments apply universally
4. **Provisionality** — Propositions from induction stand until new phenomena

### Newton's Inconsistency

Scholarly analysis reveals nuance:
- In *Opticks* Query 31, he freely speculated about particles, forces, "subtle spirit"
- Between *Principia* editions, renamed "Hypotheses" as "Phenomena" or "Rules"
- Distinguished between **heuristic** hypotheses vs. **proven conclusions**

SOURCE: [Newton's Philosophy - Springer](https://link.springer.com/chapter/10.1007/978-94-010-3381-7_8)

## Documented Examples

### Gravity

Newton could describe:
- Mathematical laws governing gravity
- Observable effects on planetary motion
- Tidal phenomena

Newton refused to speculate on:
- What gravity "is"
- Why it operates at a distance
- Its underlying mechanism

### The General Scholium (1713)

Appended to *Principia* second edition:
- Established epistemological boundaries
- Responded to Cartesian critics
- Defined limits of experimental philosophy

### Response to Critics

When attacked for "occult qualities" in gravity:
- Did not offer mechanistic explanation
- Stood firm on phenomenological grounding
- Refused to speculate on causes

### The *Opticks* Contrast

In *Opticks* Query 31:
- More speculative about particles and forces
- Discussed "subtle spirit" pervading matter
- Demonstrated domain boundaries can shift

This shows: Domain boundaries are not absolute but depend on available evidence.

## AI Equivalent

The AI equivalent is **epistemic boundary setting**—explicitly defining scope limits for claims and refusing to extrapolate beyond evidence.

### Why This Matters for AI

LLMs can:
- Generate speculative content confidently
- Extrapolate beyond evidence
- Blend speculation with fact seamlessly
- Lack explicit epistemic boundaries

Domain boundary refusal requires:
- Distinguishing supported vs. unsupported claims
- Explicit scope limitations
- Refusing speculation when evidence insufficient

### Implementation Pattern

```python
# Domain Boundary Refusal Protocol
def enforce_epistemic_boundaries(claim, evidence):
    """
    Explicitly limit claims to what evidence supports.
    Refuse speculation beyond grounded domain.
    """
    
    # Step 1: Evidence grounding check
    supported = extract_supported_components(claim, evidence)
    unsupported = extract_unsupported_components(claim, evidence)
    
    # Step 2: Domain boundary definition
    domain = {
        "inside": supported,  # What phenomena support
        "boundary": [         # Where support ends
            component for component in claim
            if partially_supported(component, evidence)
        ],
        "outside": unsupported  # Beyond evidence
    }
    
    # Step 3: Hypothesis identification
    hypotheses = identify_speculative_components(unsupported)
    for hypothesis in hypotheses:
        hypothesis_type = classify_hypothesis(hypothesis)
        # Metaphysical? Physical? Mechanistic?
        domain["outside"].append((hypothesis, hypothesis_type))
    
    # Step 4: Domain boundary enforcement
    bounded_claim = {
        "supported_claim": supported,
        "evidence_basis": evidence,
        "scope_limitation": f"Does not extend to {unsupported}",
        "refusal": hypotheses if hypotheses else None
    }
    
    # Step 5: Explicit statement
    if hypotheses:
        bounded_claim["disclaimer"] = (
            f"Hypotheses about {hypotheses} are not deduced from "
            f"phenomena and have no place in this domain."
        )
    
    return bounded_claim
```

### Hypothesis Classification

```python
def classify_hypothesis(hypothesis):
    """
    Classify type of speculation.
    """
    types = {
        "metaphysical": [
            "ultimate cause", "why it exists", "underlying nature"
        ],
        "physical_mechanism": [
            "how it works", "mechanism", "hidden process"
        ],
        "occult_quality": [
            "inherent property", "essential nature"
        ],
        "heuristic": [
            "useful assumption", "model assumption", "working hypothesis"
        ]
    }
    
    for type_name, patterns in types.items():
        if matches_patterns(hypothesis, patterns):
            return type_name
    
    return "unclassified"
```

## Implementation Instructions

**Protocol: Domain Boundary Refusal Mode**

### Step 1: Evidence Grounding
"Identify what the evidence actually supports:
- What phenomena are observed?
- What patterns are documented?
- What measurements exist?
- What can be deduced from these?"

### Step 2: Hypothesis Identification
"Identify claims that extend beyond evidence:
- What is being asserted about causes?
- What mechanistic explanations are proposed?
- What metaphysical claims are embedded?
- What assumptions are implicit?"

### Step 3: Domain Boundary Definition
"Define the boundary between supported and unsupported:
- Inside domain: Phenomena-based claims
- Boundary: Claims with partial support
- Outside domain: Speculative extensions

Explicitly state: 'The evidence supports X, Y, Z. It does not support A, B, C.'"

### Step 4: Refusal Statement
"For claims outside the domain:
- 'I do not feign hypotheses about [claim].'
- 'This lies beyond what phenomena can establish.'
- 'Speculation about [mechanism] is not grounded in evidence.'

This is not evasion—it is epistemic honesty."

### Step 5: Scope Limitation
"Explicitly limit scope of conclusions:
- 'This conclusion applies to [domain].'
- 'Extension to [outside domain] would require additional evidence.'
- 'I claim only what the phenomena support.'"

### Minimal Version
"What does the evidence actually support? What are you claiming that extends beyond it? State both explicitly."

## Amplification Notes

### Human Tendencies

**Against domain boundary refusal:**
- Desire for explanation beyond evidence
- Pressure to speculate on causes
- Expectation of mechanistic stories
- Confusion of description with explanation

**Newton's discipline:**
- Stood firm against Cartesian critics
- Refused to offer ungrounded explanations
- Maintained distinction between law and cause

### AI Implementation

**What AI can enforce:**
- Explicit scope limitation
- Evidence citation for claims
- Hypothesis identification
- Domain boundary statements

**What AI struggles with:**
- Distinguishing legitimate theory from speculation
- Knowing when speculation is productive
- Balancing explanation vs. epistemic honesty

## Failure Modes

### 1. Over-Speculation
- Making claims beyond evidence
- Countermeasure: Explicit domain boundary statement

### 2. Under-Speculation
- Refusing legitimate theoretical extension
- Countermeasure: Distinguish heuristic from unfounded

### 3. False Boundary
- Drawing domain boundaries incorrectly
- Countermeasure: Ground boundaries in evidence type

### 4. Scope Creep
- Gradually extending claims without evidence
- Countermeasure: Periodic boundary re-verification

### 5. Hypothesis Denial
- Refusing all hypotheses, even useful heuristics
- Countermeasure: Distinguish types of hypotheses

## Cross-References

- TRAIT-simplicity-stripping.md — Both limit claims to minimum
- TRAIT-constructive-specification.md — Both require grounding
- TRAIT-verification-after-generation.md — Verification limits claims
- TRAIT-differential-effort-detection.md — Symmetric scrutiny of evidence

## Note on Tier 2 Classification

This trait is classified as **Tier 2 (Implementable with Inference)** because:
1. Domain boundary setting is directly implementable
2. Hypothesis classification can be automated
3. Scope limitation statements are straightforward

However, the **judgment** of where legitimate theory ends and speculation begins requires expertise.

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources (Newton documentation), 4 web sources*
*Confidence: High*