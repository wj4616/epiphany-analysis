# TRAIT: Golden Rule for Disconfirming Evidence

---
trait_name: Golden Rule for Disconfirming Evidence
tier: Tier 1
source_individuals:
  - Darwin
ai_implementation_category: Evidence Management / Bias Mitigation
replication_feasibility: 5
utility_score: 5
primary_keywords:
  - disconfirming evidence capture
  - confirmation bias mitigation
  - asymmetric memory intervention
  - negative evidence handling
  - objection documentation
---

## Background and Definition

**Golden Rule for Disconfirming Evidence** is the cognitive practice of immediately documenting observations that contradict one's developing conclusions. It is an asymmetric memory intervention designed to counteract the systematic bias toward forgetting disconfirming evidence.

Darwin's explicit statement:
> "Whenever a published fact, a new observation or thought came across me, which was opposed to my general results, to make a memorandum of it without fail and at once; for I had found by experience that such facts and thoughts were far more apt to escape from memory than favorable ones."

SOURCE: gm-darwin-evidence-accumulation.md; direct Darwin quote

The golden rule is not general advice to keep good notes—it is a targeted intervention against a specific cognitive failure mode: **confirmation bias operates through differential memory decay**.

## Cognitive/Psychological Basis

### Asymmetric Belief Revision

Research reveals a fundamental asymmetry in how people update beliefs:
- **Easier to believe than to disbelieve**: People are better at believing something they previously thought was false (negation-to-affirmation) than at unbelieving something they previously believed to be true (affirmation-to-negation)
- **Memory mechanisms**: When correcting false beliefs, negations leave "holes" in mental representations without providing alternative explanations, making the correction less memorable

SOURCE: Yang, Stone & Marsh (2022); [Asymmetric Belief Revision](https://www.marshmemorylab.com/s/Applied-Cognitive-Psychology-2022-Yang-Asymmetry-in-belief-revision-1.pdf)

INFERENCE: This asymmetry is precisely what Darwin's golden rule counteracts—by capturing disconfirming evidence immediately, it prevents the "hole" problem.

### Confirmation Bias Mechanisms

Three manifestations of confirmation bias:
1. **Selective exposure** — Seeking confirming over disconfirming information
2. **Biased assimilation** — Interpreting ambiguous evidence as confirming
3. **Differential memory decay** — Forgetting disconfirming evidence faster

SOURCE: [Confirmation Bias Mitigation Techniques](https://www.tandfonline.com/doi/full/10.1080/23743603.2023.2196755)

### The Memory Intervention

Darwin's insight: confirming evidence is rehearsed and reinforced; disconfirming evidence is uncomfortable and fades. The golden rule corrects a specific hardware limitation, not a reasoning error.

The rule is asymmetric: favorable observations do not require the same urgency because they are less likely to be forgotten.

## Documented Examples

### Darwin's Practice

Darwin documented his practice of actively seeking objections:
> "Owing to this habit, very few objections were raised against my views which I had not at least noticed and attempted to answer."

He admired Charles Lyell's methodology — "advance all possible objections to my suggestions, and even after these were exhausted would long remain doubtful" — and emulated it.

SOURCE: gm-darwin-evidence-accumulation.md; direct Darwin quote

### The Timeline of Patience

Darwin formed the core of natural selection theory by 1838 — twenty-one years before publishing in 1859. The timeline shows systematic objection-hunting:

| Date | Event |
|------|-------|
| 1831–1836 | HMS Beagle voyage — observation collection |
| July 1837 | Opens "B" transmutation notebook |
| September 1838 | Reads Malthus; natural selection concept formed |
| 1842 | First pencil sketch of theory |
| 1844 | Expands to 230-page essay |
| 1846–1854 | Eight-year barnacle study — evidence accumulation at micro-level |
| 1858 | Wallace sends parallel theory; joint publication forced |
| 1859 | Origin published |

The eight-year barnacle study was deliberate evidence accumulation—variation within species before arguing about variation between species.

SOURCE: gm-darwin-evidence-accumulation.md

### Evidence Source Diversification

Darwin's evidence came from multiple independent sources: "people with practical experience in selective breeding such as farmers and pigeon fanciers, family members, a butler, neighbors, colonists, and former shipmates."

The deliberate inclusion of non-academic sources was methodological—observations from people with no theoretical stake provided confirmation that could not be attributed to shared theoretical bias.

## AI Equivalent

The AI equivalent is **explicit disconfirming evidence tracking**—maintaining a running list of observations that contradict the developing conclusion.

### Why This Matters for AI

LLMs do not experience motivated forgetting—but they experience **anchoring on recent framing**. When a conclusion is developing, disconfirming observations in the context window may be implicitly down-weighted.

### Implementation Pattern

```python
# Golden Rule Protocol
conclusion_draft = "..."
disconfirming_observations = []

def capture_disconfirming(observation, evidence_base, conclusion):
    """If observation opposes conclusion, capture immediately."""
    if contradicts(observation, conclusion):
        disconfirming_observations.append({
            'observation': observation,
            'timestamp': current_time(),
            'attempted_answer': None
        })
        # Do NOT explain it away yet — just record it

def golden_rule_pass(conclusion, disconfirming_observations):
    """Before proceeding: scan for disconfirming observations."""
    for obs in disconfirming_observations:
        if obs['attempted_answer'] is None:
            print(f"Unaddressed disconfirming observation: {obs['observation']}")
            # Flag as highest priority to address
```

### The Key Machine Advantage

An LLM does not have beliefs it wants to protect—but it can apply disconfirmation effort mechanically, without the unconscious dampening that prevents humans from looking too hard for problems in their preferred conclusions.

SOURCE: Research on AI bias mitigation; INFERENCE from cognitive literature on confirmation bias

## Implementation Instructions

**Protocol: Golden Rule Mode**

### Step 1: Asymmetric Capture
"Before proceeding: scan the current evidence base for observations that are opposed to the developing conclusion. Record each disconfirming observation explicitly. Do not explain it away yet—just record it."

Prompt template:
```
## Golden Rule Pass
Current conclusion: [stated conclusion]

Instructions:
- List every observation in the evidence base that seems to contradict this conclusion
- Write each one down now
- Do NOT attempt to explain or dismiss
- These observations will be most apt to escape if not captured

Disconfirming observations:
1. [observation 1]
2. [observation 2]
...
```

### Step 2: Asymmetric Urgency
"Disconfirming observations require immediate capture; confirming observations do not. This is not equal treatment—this is asymmetric intervention against memory bias."

### Step 3: Objection Sweep
"For each disconfirming observation: (a) state the observation precisely, (b) attempt a full answer, (c) if you cannot answer it fully, identify what additional evidence would be required to answer it."

### Step 4: Evidence Source Audit
"What types of evidence support this conclusion? List them by category. Identify which evidence types are absent. If the conclusion rests entirely on one category, what would convergent evidence from a different category look like?"

### Step 5: Density Check
"Is the evidence base dense enough to be definitive, or merely sufficient to be suggestive? State the minimum evidence density that would make the conclusion defensible against a comprehensive objection sweep. Have you reached it?"

### Minimal Version
"Before finishing: list every observation in the evidence base that seems to contradict the conclusion. Write each one down now. Then attempt to answer each. If any is unanswerable, the conclusion needs more evidence."

## Amplification Notes

### Human Limitations
- Confirmation bias operates through differential memory decay
- Disconfirming evidence is uncomfortable and fades
- Motivated forgetting is systematic, not random
- Expertise can increase confidence while not increasing accuracy

### AI Amplification Potential
- **Perfect memory fidelity**: Disconfirming observations do not decay
- **Systematic capture**: Mechanical application of asymmetric urgency
- **No motivated suppression**: No beliefs to protect
- **Convergent evidence tracking**: Can track multiple evidence types simultaneously

The key insight: LLMs do not experience the biological memory decay that makes Darwin's golden rule necessary—but they experience the anchoring equivalent. Explicit capture protocols counteract this.

## Failure Modes

### 1. Explaining Away Immediately
- Disconfirming observation is captured but immediately dismissed
- The golden rule requires capture WITHOUT immediate explanation
- Countermeasure: Separate capture phase from explanation phase

### 2. Single-Source Validation
- Evidence from one domain or one community
- All sharing the same theoretical assumptions
- Countermeasure: Diversify evidence sources explicitly

### 3. Premature Density Declaration
- Declaring evidence density sufficient before comprehensive objection sweep
- Countermeasure: Density check must follow objection sweep, not precede it

### 4. Asymmetric Effort
- More effort on confirming evidence than disconfirming
- The Millikan example from Feynman
- Countermeasure: Symmetric scrutiny enforcement

## Cross-References

- TRAIT-differential-effort-detection.md — Feynman's Millikan example is the same bias
- TRAIT-verification-after-generation.md — Verification phase must include disconfirming evidence
- TRAIT-evidence-source-diversification.md — Darwin's multi-source evidence collection
- TRAIT-preparation-before-solution.md — Loading disconfirming evidence into working memory

---

*Trait report generated: 2026-04-09*
*Sources: 3 primary sources, 3 web sources*
*Confidence: High*