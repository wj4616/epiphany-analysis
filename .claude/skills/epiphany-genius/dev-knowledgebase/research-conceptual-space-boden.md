# Boden's Conceptual Space Taxonomy

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/Computational_creativity · https://plato.stanford.edu/entries/creativity/ · https://arxiv.org/search/?searchtype=all&query=conceptual+space+exploration+AI+creativity
**Content quality:** High — SEP entry provided complete three-type taxonomy with examples; Computational Creativity Wikipedia supplemented; arXiv not attempted (search returned zero results for similar queries in this session)

## Findings

### Boden's Three Types of Creativity (from SEP Philosophy of Creativity)

Boden's framework provides the most comprehensive theoretical taxonomy for classifying creative operations — and each type maps to a distinct computational strategy.

#### Type 1 — Combinatorial Creativity

**Definition**: "Occurs when preexisting ideas are merged in novel ways."

**Examples**:
- Pegasus (horse + wings) — merging two known biological entities
- Bohr's atomic model comparing atoms to solar systems — analogical merge of two scientific domains

**Mechanism**: No constraint is violated. The novel result comes from placing existing elements in new proximity or relation. The conceptual space itself is unchanged.

**Key property**: This is the *most accessible* creativity type — it requires only access to diverse elements and the permission to combine them without prejudice.

#### Type 2 — Exploratory Creativity

**Definition**: "Creativity that arises from an exploration within an established conceptual space."

**Examples**:
- Dickens using multiple adjectives before a noun — "grammatically permitted but striking"
- A chess player discovering an unusual combination that was theoretically always possible given the rules

**Mechanism**: The conceptual space (its rules and constraints) is preserved. The creative act is finding a previously unvisited region of the space — not a new possibility but an *unvisited* one. The space defines "a certain range of possibilities."

**Key property**: Requires knowing the space's rules and systematically traversing less-visited regions. Thoroughness within constraints is the amplifiable variable.

#### Type 3 — Transformational Creativity

**Definition**: "Creativity that arises when creators alter the foundational rules of a conceptual space itself."

**Examples**:
- Schoenberg negating the home-key constraint in Western music → atonal composition
- Lobachevsky negating Euclid's fifth axiom (parallel postulate) → non-Euclidean geometry
- Kekulé negating the constraint that molecular chains must be open → ring-shaped organic molecules (benzene ring)

**Mechanism**: "The creator must transform the space itself — downright impossible within the previous framework." A constitutive constraint is identified, negated, and the implications elaborated.

**Key property**: This is Boden's highest creativity type — it creates genuinely new possibilities that could not have existed before. It requires both knowing which constraints define the current space AND being willing to negate one.

### The Conceptual Space Definition

A conceptual space represents "the generative system that underlies that domain and defines a certain range of possibilities: chess moves, or molecular structures, or jazz melodies." It comprises:
- **Basic elements**: The atomic units of the domain
- **Operational rules or constraints**: What combinations and transformations are permitted
- **Excluded possibilities**: What is "impossible" within the current space

Boden argues that conceptual space elements and rules can be captured computationally, "suggesting the mind operates similarly to algorithmic systems."

### Wiggins' Formalization

The SEP entry notes that "Boden's insights are also the subject of formalization, most notably in the work by Geraint Wiggins," who provided a formal grammar-based model of conceptual spaces. This confirms the computational tractability of the framework.

## Skill Mapping

Boden's three types map to three distinct prompt modes for epiphany-cognitive — each appropriate at a different stage of creative problem-solving:

**Mode 1 — Combinatorial (default generative mode)**:
- Trigger: Early in ideation; standard generation pass
- Instruction: "Identify the key elements from Domain A and Domain B relevant to this problem. Generate all plausible novel combinations. Do not evaluate yet."
- This mode is equivalent to the Combinatorial Play component already in the KB, now given taxonomic grounding.

**Mode 2 — Exploratory (systematic coverage mode)**:
- Trigger: When standard generation produces familiar solutions; when coverage of the solution space seems incomplete
- Instruction: "Map the boundaries of the conceptual space: what are the rules and constraints that define it? Now systematically identify regions that have not been explored. What solutions exist at the edges of the permissible space?"
- Key question: "What is theoretically possible within the current constraint set that has not yet been tried?"

**Mode 3 — Transformational (breakthrough mode)**:
- Trigger: When Exploratory mode exhausts the space without finding adequate solutions; when the best solution within the current space is still insufficient
- Instruction: "List the foundational constraints that define this conceptual space. These are the rules that make certain solutions 'impossible.' Select one constraint. State what becomes possible if this constraint is negated. Elaborate the implications."
- This is the most powerful mode — and the most computationally demanding, because it requires identifying *constitutive* constraints (which define the space) rather than merely *incidental* constraints (which happen to be present).

**Sequencing logic**: Combinatorial → Exploratory → Transformational represents an escalating ladder of creative radicalism. Move to the next mode when the current one has been exhausted without satisfactory output.

**Machine amplification**: Transformational creativity is the hardest for humans — there is enormous social and cognitive resistance to negating foundational assumptions. An LLM can be explicitly instructed to negate any named constraint without the resistance bias. The machine executes constraint negation as readily as any other operation.
