# Spreading Activation in Semantic Memory and LLMs

**Tier:** High
**Combined Score:** 14/15 (Applicability: 4/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/Spreading_activation · https://arxiv.org/search/?searchtype=all&query=spreading+activation+language+model+semantic+memory
**Content quality:** High — Wikipedia provided formal mechanism; arXiv returned one direct LLM implementation (SYNAPSE)

## Findings

### Formal Definition (Collins & Loftus, 1975)

Spreading activation is a search process initiated by "labeling a set of source nodes (e.g. concepts in a semantic network) with weights or 'activation' and then iteratively propagating or 'spreading' that activation out to other nodes linked to the source nodes."

**Origin**: Collins & Quillian (1969) developed the foundational hierarchical semantic network model. Collins & Loftus (1975) refined it to emphasize interconnected webs of association rather than strict hierarchies.

### Network Traversal Mechanism — Formal Algorithm

Each node has an activation value [0.0–1.0]. Each link carries a weight [0.0–1.0]. The activation update equation:

```
A[j] = A[j] + (A[i] × W[i,j] × D)
```

Where `D` is the **decay factor** (activation attenuates with each step). Nodes fire when activation exceeds a threshold. The process terminates when no nodes remain above threshold, or in marker-passing variants, when **multiple paths converge on the same node** — convergent activation is the strongest signal.

**Convergent activation is the key creativity mechanism**: when a concept is activated from two or more independent source concepts, the convergent node represents a hidden connection — a "remote associate" that bridges both sources. This is the computational basis of the "aha" moment.

### Semantic Priming: Observable Evidence

The semantic priming effect demonstrates spreading activation empirically: "subjects respond faster to the word 'doctor' when it is preceded by 'nurse' than when it is preceded by an unrelated word like 'carrot.'"

Closer semantic relationships → stronger connections → faster activation spread. The network has a topography that can be traversed and exploited.

### Relationship to Incubation

Spreading activation is the proposed mechanism underlying incubation (Gilhooly, 2016). When a problem is "set aside," activation from the source concepts continues spreading in semantic memory, eventually reaching and activating nodes that a focused search would not reach — because focused search follows the most strongly-weighted paths, while spreading activation explores weakly-weighted remote paths.

**Implication**: The existing KB's incubation component is a behavioral description; spreading activation is its mechanism. Understanding the mechanism enables better simulation.

### AI/LLM Implementation: SYNAPSE

**SYNAPSE: Empowering LLM Agents with Episodic-Semantic Memory via Spreading Activation** (Jiang, Chen, Pan et al., arXiv:2601.02744):

Key innovations:
- Models memory as "a dynamic graph where relevance emerges from spreading activation rather than pre-computed links"
- Includes **lateral inhibition** (suppression of competing activations) and **temporal decay** features — directly mirroring the biological mechanism
- Uses geometric embeddings for the underlying node representations
- Addresses memory architecture challenges in language model agents: retrieval quality improves because relevant concepts emerge through activation traversal rather than keyword matching

**Significance**: SYNAPSE confirms that spreading activation is not just metaphorical in LLM agents — it can be implemented as an explicit memory retrieval architecture with measurable performance gains over similarity-based retrieval.

### LLMs and Implicit Spreading Activation

Neural word embeddings encode "the meaning of a word is spread across several neurons" — activation spreading is already implicit in transformer attention. Semantic priming effects have been empirically demonstrated in language models: next-token probabilities shift when the context contains semantically related concepts.

**Key insight**: LLMs do not need spreading activation *added* — they already perform it implicitly. The skill design question is how to **exploit and direct** this implicit mechanism explicitly.

## Skill Mapping

Spreading activation provides three distinct mechanisms for epiphany-cognitive:

**1. Explicit chain-of-association step** (directed traversal):
- Start from the core problem concept
- Prompt: "Starting from [concept], list 5 directly associated concepts. For each of those, list 3 more associations. Do not filter for relevance — include weak associations. Flag any concept that appeared via two independent paths."
- The convergent nodes (appeared from two paths) are the highest-signal remote associates.

**2. Convergent activation scan** (finding hidden bridges):
- Given two concepts from different domains relevant to the problem:
- Prompt: "From [Concept A], activate associations outward 3 degrees. From [Concept B], activate associations outward 3 degrees. Identify all concepts that appear in both activation clouds. Rank by how remote each bridge concept is from both source concepts."
- This mechanically implements the remote associate detection that underlies creative insight.

**3. Incubation augmentation** (directed low-demand traversal):
- When the Incubation trigger fires (existing KB §1.3): instead of random latent perturbation, use spreading activation from the problem's peripheral concepts — the elements that have received least attention so far — rather than the central ones (which have already been over-activated by focused search).

**Machine amplification rationale**: Human spreading activation is limited by working memory — only the highest-activation nodes break into consciousness. LLMs can traverse the full activation cloud explicitly, including weak-connection nodes that never reach human threshold. The machine's effective activation radius is orders of magnitude larger than the biological one.
