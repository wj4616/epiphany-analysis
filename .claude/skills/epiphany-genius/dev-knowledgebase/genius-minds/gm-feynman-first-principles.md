# Feynman: First-Principles Decomposition

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Figure(s):** Richard Feynman
**Category:** A — Reasoning/problem-solving
**Sources fetched:** https://en.wikipedia.org/wiki/Richard_Feynman · https://en.wikipedia.org/wiki/The_Feynman_Lectures_on_Physics · https://fs.blog/feynman-learning-technique/ · https://fs.blog/richard-feynman-knowing-something/
**Content quality:** High — Wikipedia confirmed first-principles problem orientation from childhood; fs.blog provided complete formal 4-step technique with cognitive mechanism documentation and direct Feynman quotes

## Documented Evidence

### Feynman's Core Epistemological Distinction

Feynman stated explicitly:

> "I learned very early the difference between knowing the name of something and knowing something."

His bird metaphor makes the distinction precise:

> "See that bird? It's a brown-throated thrush, but in Germany it's called a Halzenfugel, and in Chinese they call it a Chung Ling and even if you know all those names for it, you still know nothing about the bird. You only know something about people."

Naming is a social fact. Understanding is a structural fact. Feynman insisted that jargon and technical vocabulary are precisely what people use to mask the absence of real understanding: "We often talk in fact-deficient, obfuscating generalities to cover up our lack of understanding."

His operational test: "If you have to test your knowledge about the concept of something, say energy, then without using the word 'energy', try to rephrase what you know about it in your own language." If you cannot name-drop your way through the explanation, you must actually understand it.

### First-Principles Behavior from Childhood

Wikipedia documents that when repairing radios as a child, Feynman "would analyze the issues theoretically and arrive at the solutions" — not by applying memorized repair patterns, but by reasoning from the physics of how radios work. This first-principles orientation was not a methodology he adopted; it was his default mode.

He also created custom mathematical notation before college — personalized symbols for trigonometric functions — because the standard notation obscured the relationships he wanted to think with. He re-derived the conceptual tools from scratch when the standard ones interfered with understanding.

### The Feynman Lectures: Derive, Don't Assert

In the preface to The Feynman Lectures on Physics, he stated his pedagogical principle:

> "When new ideas came in, I would try either to deduce them if they were deducible or to explain that it was a new idea... and which was not supposed to be provable."

He distinguished rigorously between what follows from prior principles and what must be accepted as a new primitive. This distinction — derivable vs. axiomatic — is the operational definition of first-principles reasoning: know which conclusions you are deducing and which starting assumptions you are accepting.

### The Self-Test Mechanism

Feynman identified the precise failure mode that first-principles discipline prevents: "One of the ways we mask our lack of understanding is by using complicated vocabulary and jargon." The presence of jargon in an explanation is a signal, not a proof — it may indicate understanding, or it may indicate fluency at reciting absorbed formulations.

The simplicity test breaks this ambiguity: explain it to a sixth-grader, without technical vocabulary. The child's failure to understand is not the child's limitation — it is information about the explainer's understanding.

## Cognitive Mechanism

Feynman's first-principles method has three interlocking operations:

**1. Name/concept separation**: Actively distinguish between knowing a label (a social convention) and knowing the underlying structure (a factual claim about the world). Names can be recalled; concepts must be reconstructed. The separation prevents fluent recall from being mistaken for understanding.

**2. Jargon removal as a diagnostic**: Technical vocabulary carries two loads: conceptual content AND the ability to hide the absence of conceptual content. Stripping jargon is a diagnostic test — what remains is either genuine understanding or exposed ignorance. There is no third option.

**3. Gap-identification via teaching attempt**: When you attempt to explain something at a level another person can understand, gaps in your own understanding become structurally visible. You cannot explain what you do not understand; you can only repeat formulations. The teaching attempt surfaces exactly the boundaries of actual vs. apparent knowledge.

The full four-step technique as formalized:

1. **Write as if teaching a sixth-grader**: Use only simple vocabulary. Complex vocabulary is disallowed.
2. **Find where the explanation breaks down**: These are the exact locations of understanding gaps. They are not approximate — they are structurally precise.
3. **Return to source material specifically for the gaps**: Not general re-reading, but targeted acquisition of the missing structural understanding.
4. **Rebuild the explanation until it flows without jargon**: The coherence test is narrative flow — can you tell a connected story that makes the concept make sense to a non-expert?

## AI/Machine Amplification

Human first-principles application is limited by:
- Fluency illusion: familiarity with a domain makes jargon feel like understanding
- Anchoring on inherited explanations: people use the explanation they learned, not the one that follows from the structure
- Inability to identify exactly which step in a multi-step explanation fails — it "feels right" until tested

An LLM can amplify each of these:
- **Forced jargon prohibition**: Explicitly banning domain vocabulary in an explanation-generation step forces the model to reconstruct from structure, not from pattern-matched formulations
- **Gap scanning**: After generating an explanation, run a second pass that asks: "At which step does this explanation assume rather than derive? Mark each assumption." This makes the derivation boundary explicit
- **Sixth-grader test at scale**: Generate 5 explanations at progressively lower vocabulary levels and check whether the structural relationships survive or dissolve at each step — the level where they dissolve is where the explanation was relying on vocabulary rather than structure
- **Name/concept audit**: For any explanation, enumerate every technical term used and demand a definition of each term without using the term itself — this surfaces borrowed language precisely

## Skill Mapping

**Protocol: Feynman First-Principles Mode**

1. **Name-stripping pass**: "Identify every technical term in this problem or explanation. For each term, provide a definition that does not use the term itself or any synonym of it. If you cannot provide such a definition, mark the term as a 'borrowed label.'"

2. **Sixth-grader explanation**: "Explain this concept to a sixth-grader using no technical vocabulary. Write the explanation as a narrative, not a list. If you find yourself writing a phrase you cannot explain further without using technical terms, stop and flag it as a gap."

3. **Gap inventory**: "List every gap you identified in Step 2. For each gap: (a) state exactly what would need to be true for the explanation to hold at that point, and (b) identify whether this is a deducible consequence of prior principles or a new primitive that must be accepted."

4. **Derivation vs. axiom classification**: "Classify each component of this explanation as either: DERIVED (follows from prior principles — state which ones) or AXIOM (accepted as a starting point — state why). Any component classified as derived must have its derivation path made explicit."

5. **Reconstruction test**: "Reconstruct the full explanation using only the AXIOM-level primitives and explicit derivations. Does the original conclusion still follow? If not, identify which assumed derivation was actually a hidden axiom."

**Trigger condition**: Use Feynman mode when an explanation or problem solution relies heavily on domain vocabulary, when a concept "seems understood" but resists re-explanation, or when two experts disagree on a principle (both may be repeating inherited formulations rather than deriving from shared primitives).

**Minimal version**: "Explain this without using any technical vocabulary. Find the first sentence where you cannot continue without borrowing a term. That is the boundary of your actual understanding."

## Related KB Entries

- `research-metacognition-ai.md` — The gap-identification step is a metacognitive monitoring operation; Flavell's monitoring component tracks the boundary between known and unknown
- `research-representational-change.md` — Chunk decomposition (Ohlsson) is the formal mechanism behind the jargon-removal step: breaking compiled knowledge chunks back into their constituents
- `gm-feynman-error-seeking.md` — Companion file: where first-principles decomposition reveals gaps, error-seeking actively probes for those gaps before they cause failures
