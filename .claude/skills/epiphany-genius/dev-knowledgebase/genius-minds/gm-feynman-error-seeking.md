# Feynman: Error-Seeking and Calibration

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Figure(s):** Richard Feynman
**Category:** C — Metacognitive habits
**Sources fetched:** https://calteches.library.caltech.edu/51/2/CargoCult.htm · https://en.wikipedia.org/wiki/Richard_Feynman · https://www.themarginalian.org/2012/06/08/richard-feynman-caltech-cargo-cult-science/
**Content quality:** High — Full text of Cargo Cult Science speech retrieved; Millikan example, self-deception principle, and reporting standards all documented verbatim

## Documented Evidence

### The First Principle

From Feynman's 1974 Caltech Commencement Address ("Cargo Cult Science"):

> "The first principle is that you must not fool yourself — and you are the easiest person to fool."

This is not advice about fooling others. It is a calibration claim: the primary threat to accurate belief is not external deception — it is the systematic self-deception that operates invisibly during your own reasoning.

### The Millikan Oil Drop Calibration Failure

Feynman documented one of the clearest historical examples of cascading miscalibration:

> "When they got a number that was too high above Millikan's, they thought something must be wrong — and they would look for and find a reason why something might be wrong. When they got a number closer to Millikan's value they didn't look so hard."

The error did not come from dishonesty. It came from differential effort: scientists applied more scrutiny to results that deviated from expectation than to results that confirmed it. The asymmetry in error-seeking was the bias. Each researcher who accepted a slightly-wrong-but-not-too-wrong result anchored the next researcher's prior. The error accumulated in one direction and persisted across the scientific community for years.

### The Reporting Standard

Feynman's operationalized requirement for honest scientific work:

> "You must report everything that you think might make it invalid — not only what you think is right about it: other causes that could possibly explain your results; and things you thought of that you've eliminated by some other experiment, and how they worked — to make sure the other fellow can tell they have been eliminated."

And: "Details that could throw doubt on your interpretation must be given, if you know them."

This is not a general instruction to be thorough — it is a specific requirement to seek and report disconfirming evidence with at least as much effort as confirming evidence.

### Cargo Cult Science Definition

The negative example: practitioners "follow all the apparent precepts and forms of scientific investigation, but they're missing something essential" — the planes don't land. What they are missing is the active error-seeking behavior. The surface structure of science (hypotheses, measurements, reports) can be reproduced without the epistemic discipline that gives it value.

## Cognitive Mechanism

Feynman's error-seeking methodology has three structural components:

**1. Self-deception as the primary threat**: Not ignorance, not external error, not bad data — the first-order danger is the agent's own motivated reasoning. This reframes the problem: the most dangerous errors are not random but systematically directional, because they are filtered by what you want to be true.

**2. Differential effort detection**: The Millikan example reveals the mechanism: humans apply asymmetric scrutiny to confirming vs. disconfirming results. Calibration requires detecting and correcting this asymmetry. The operational question is not "is this result plausible?" but "am I applying the same scrutiny to this result as I would to a result pointing the other direction?"

**3. Active disconfirmation reporting**: Error-seeking is not complete when you fail to find errors — it is complete only when you have actively generated and investigated the strongest reasons your conclusion might be wrong, and reported what you found. Absence of effort is not the same as absence of error.

The failure mode Feynman named: generating reasons why a result might be wrong selectively (when the result is inconvenient), but not generating those same reasons when the result is convenient. Calibration means applying disconfirmation effort uniformly, not just when the result surprises you.

## AI/Machine Amplification

Human error-seeking failures are systematic and predictable:
- Confirmation bias: more scrutiny to disconfirming evidence
- Fluency heuristic: smooth, easy-to-construct explanations feel more correct
- Anchoring: prior estimates bias evaluation of new results
- Publication selection: results that confirm hypotheses are reported; null results are not

An LLM can be explicitly instructed to invert these biases:
- **Symmetric scrutiny enforcement**: After generating a conclusion, run an explicit pass with equal effort on "why this might be wrong" as was spent on "why this is right"
- **Steel-man disconfirmation**: Generate the three strongest arguments against the conclusion before accepting it
- **Millikan audit**: Ask "If I were unconsciously motivated to reach this conclusion, what evidence would I be systematically under-weighting or under-scrutinizing?"
- **Uniform reporting**: Require that every generated conclusion includes a list of conditions under which it would be false — not as a formality but as a substantive check that those conditions have been considered
- **Differential effort test**: Compare: "How hard did I look for evidence that this is wrong vs. how hard I looked for evidence it is right?" If asymmetric, rebalance.

The key machine advantage: an LLM does not have beliefs it wants to protect. It can apply disconfirmation effort mechanically, without the motivated suppression that makes human error-seeking fail. The instruction to seek errors can be executed without the unconscious dampening that prevents humans from looking too hard for problems in their preferred conclusions.

## Skill Mapping

**Protocol: Feynman Calibration Mode**

1. **Self-deception prompt**: "Before evaluating this conclusion: identify what outcome you are implicitly hoping for. State it explicitly. Now apply the Millikan test: if you were unconsciously filtering evidence toward this outcome, what would that look like? What evidence would you examine less carefully?"

2. **Symmetric scrutiny pass**: "Generate the three strongest arguments against this conclusion. Apply at least as much effort to each as you applied to your best argument for it. If you find a disconfirming argument stronger than any confirming argument, update the conclusion."

3. **Active disconfirmation inventory**: "List every condition under which this conclusion would be false. For each condition: (a) have you checked whether it holds? (b) if not, why not? Unchecked conditions are not absent conditions."

4. **Cargo cult audit**: "Identify the form of this conclusion vs. the substance. What would someone who was performing the appearance of reasoning — without the epistemic discipline — have produced? Does your conclusion differ from that? Where?"

5. **Reporting standard check**: "What would you need to report to give someone else a full picture of how you reached this conclusion — including the things that might make it wrong? Write that report. If there are items in it you are reluctant to include, those are the highest-priority items to check."

**Trigger condition**: Use Feynman calibration mode any time a conclusion has been reached by a process where the agent had a preference about the outcome, or when evaluating an answer that "feels right" without being able to articulate exactly why.

**Minimal version**: "List the top 3 reasons this conclusion might be wrong. For each: have you looked at it as hard as you looked for reasons it was right?"

## Related KB Entries

- `gm-feynman-first-principles.md` — First-principles decomposition reveals gaps; error-seeking actively probes those gaps before they cause failures. Both are Feynman's epistemology; they address different failure modes
- `research-metacognition-ai.md` — Calibration monitoring is a metacognitive control operation; Flavell's monitoring/control loop governs the detection of understanding failures
- `research-abductive-reasoning.md` — Peirce's falsifiability constraint ("hypothesis must specify conditions under which it would be wrong") is structurally identical to Feynman's reporting standard
