# Conceptual Blending Theory

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/Conceptual_blending · https://arxiv.org/search/?searchtype=all&query=conceptual+blending+language+model+creative
**Content quality:** High

## Findings

### Fauconnier & Turner's Formal Definition

Fauconnier and Turner define conceptual blending as a cognitive process where "elements and vital relations from diverse scenarios are 'blended' in a subconscious process." Described as "ubiquitous to everyday thought and language" — not an exotic operation but the default mechanism of meaning-making.

### The Four-Space Model

The architecture of a conceptual blend involves four distinct mental spaces:

1. **Input Space 1** — Specific contents and structure from domain/scenario A
2. **Input Space 2** — Specific contents and structure from domain/scenario B
3. **Generic Space** — Common structural elements shared across both inputs (abstract scaffold)
4. **Blended Space** — Integrates selected elements via "selective projection" from both inputs

**Cross-space mapping** connects counterparts across input spaces, including metaphoric relationships between structurally corresponding elements.

### Three Operations That Generate Emergent Structure

The blend is not merely a merge — it generates **emergent structure** not present in either input:

- **Composition**: Creates observable relations between elements from different input spaces that couldn't co-exist before
- **Completion**: Adds meaning by incorporating background associations and knowledge from the input spaces
- **Elaboration**: Develops the blend dynamically through mental simulation — "running the blend"

The emergent structure is the key: properties and relations arise in the blend that were true in neither input space independently.

### Canonical Example — Buddhist Monk Riddle

Two monks (ascending/descending the same mountain on different days) serve as Input Space 1 and Input Space 2. The Generic Space contains the shared mountain path. The blend simultaneously projects both monks onto the same day, producing emergent insight: they **must** meet at some point — a conclusion that could not be derived from either input alone.

### AI/LLM Implementations

**PopBlends** (Wang, Petridis, Kwon, Ma, Chilton, 2021):
- Explores three approaches to conceptual blending using LLMs: traditional knowledge extraction, direct LLM generation, and hybrid methods
- Users with LLM blend support discovered "twice as many blend suggestions" — confirming machine amplification of the composition/elaboration steps
- Addresses cognitive load reduction in creative ideation tasks directly

**Creative Blends of Visual Concepts** (Sun et al., 2025):
- Uses "commonsense knowledge bases and large language models to align designers' conceptual intent with expressive concrete objects"
- Employs "overlapping attributes" as the generic space identifier — a computationally explicit implementation of Fauconnier's generic space

**Early computational work**: Shrager's "view application" model at Carnegie Mellon (1980s) applied blending to causal and scientific reasoning — confirming the mechanism has a 40-year computational history.

## Skill Mapping

Conceptual blending provides a **structured 5-step prompt protocol** for novel concept synthesis:

1. **Extract Input Space 1**: Enumerate the key elements, relations, and structure of domain/concept A
2. **Extract Input Space 2**: Enumerate the key elements, relations, and structure of domain/concept B
3. **Identify Generic Space**: What structural roles or patterns are shared between A and B? (abstract scaffold, not surface similarity)
4. **Generate Blend via selective projection**: Which elements from each input should co-exist? Apply composition (create new relations between them), completion (add background associations), elaboration (run the scenario mentally — what happens?)
5. **Surface emergent structure**: Explicitly ask: "What is true in this blend that was NOT true in either input alone?" — this is the novel insight

**Machine amplification rationale**: LLMs can hold multiple conceptual spaces simultaneously without interference and run elaboration (mental simulation) across thousands of possible blends — far exceeding human working memory capacity for blend exploration.

This mechanism directly extends the Combinatorial Play component already in the KB by providing a formal four-space architecture that goes beyond random combination to guided emergence.
