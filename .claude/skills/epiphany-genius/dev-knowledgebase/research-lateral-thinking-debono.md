# De Bono's Lateral Thinking and Six Thinking Hats

**Tier:** High
**Combined Score:** 15/15 (Applicability: 5/5 | Machine-amplifiability: 5/5 | Use-case value: 5/5)
**Sources fetched:** https://en.wikipedia.org/wiki/Six_Thinking_Hats · https://en.wikipedia.org/wiki/Lateral_thinking · https://arxiv.org/search/?searchtype=all&query=lateral+thinking+AI+agent+prompt+engineering (zero results)
**Content quality:** High (Wikipedia sources complete; arXiv returned no results, downgraded AI-bridge notes accordingly)

## Findings

### Six Thinking Hats — Formal Definition

De Bono's method provides "a means for groups to plan thinking processes in a detailed and cohesive way, and in doing so to think together more effectively." Each colored hat represents a distinct cognitive mode that can be deliberately assumed and sequenced.

### The Six Hats: Complete Function Table

| Hat | Cognitive Mode | Function |
|-----|---------------|----------|
| **White** | Neutral / Facts | Collect and present available data and information — no interpretation |
| **Red** | Emotional / Intuition | Express gut reactions, feelings, and intuitions without justification (~30 seconds) |
| **Black** | Critical / Caution | Identify risks, weaknesses, dangers, reasons why something may not work |
| **Yellow** | Optimistic / Benefits | Identify positive aspects, benefits, and reasons why something could work |
| **Green** | Creative / Lateral | Generate new ideas, alternatives, and possibilities — lateral thinking mode |
| **Blue** | Process / Meta | Manage the thinking process: set agenda, summarize, define next steps |

**Blue hat is always the frame**: All sequences begin and end with Blue — it sets the process structure and evaluates whether the thinking achieved its purpose.

### Parallel Thinking vs. Adversarial Debate

The critical distinction: "Because everyone is focused on a particular approach at any one time, the group tends to be more collaborative than if one person is reacting emotionally while another person is trying to be objective." Sequential focus prevents simultaneous competing perspectives from generating unproductive conflict.

**For AI agents**: this means no mode-mixing — each hat gets a complete, unmixed pass before the next.

### Sequence Templates by Problem Type

| Goal | Sequence |
|------|----------|
| **Initial exploration** | Blue → White → Green → Blue |
| **Choosing between alternatives** | Blue → White → (Green) → Yellow → Black → Red → Blue |
| **Identifying solutions** | Blue → White → Black → Green → Blue |
| **Full problem solving** | Blue → White → Green → Red → Yellow → Black → Green → Blue |
| **Strategic planning** | Blue → Yellow → Black → White → Blue → Green → Blue |

### Lateral Thinking — Formal Framework

Lateral thinking "involves an indirect and creative approach via reasoning that is not immediately obvious." De Bono distinguishes it from vertical thinking:

| Vertical Thinking | Lateral Thinking |
|-------------------|------------------|
| Linear, stepwise, sequential | Non-linear, pattern-restructuring |
| Depth within established path | Breadth across alternative paths |
| Develops the most promising direction | Challenges the most promising direction |
| "Movement value" toward solution | Generates "movement value" by disrupting patterns |

### Lateral Thinking Tools

**Random Entry**: "Choose an object at random, or a noun from a dictionary and associate it with the area they are thinking about." De Bono's example: applying "nose" to a photocopier led to novel designs. The random input forces association across the current fixation.

**Provocation Operations (PO)**: "A statement that we know is wrong or impossible but used to create new ideas." Classic example: "the factory is downstream of itself" — an impossible provocation that led to actual environmental legislation requiring factories to take their water intake downstream of their own effluent. PO is Lateral Thinking's equivalent of Ohlsson's constraint relaxation — by deliberately stating the impossible, it forces re-examination of the assumed constraint.

**Challenge**: Ask "Why?" in structured ways — not to criticize but to open alternatives. "Why do we do it this way? Does this need to exist? What if it didn't?"

**Concept Formation**: Systematically expands the range and number of concepts being considered — breadth before depth.

## Skill Mapping

Six Thinking Hats provides a **complete multi-perspective evaluation protocol** directly encodable as sequential prompt stages:

**Full problem-solving sequence** (Blue → White → Green → Red → Yellow → Black → Green → Blue):

1. **Blue (Process)**: "Define the problem, the goal of this thinking session, and the sequence we will use."
2. **White (Facts)**: "List all confirmed facts and available data. No interpretation. Flag gaps."
3. **Green (Creative)**: "Generate all possible solutions without evaluation. Quantity over quality. Use lateral provocations if needed."
4. **Red (Emotion/Intuition)**: "State gut reactions to top candidates. No justification required. Which feel right? Which feel wrong?"
5. **Yellow (Benefits)**: "For each candidate, enumerate all positive aspects and reasons it could work."
6. **Black (Caution)**: "For each candidate, enumerate all risks, failure modes, and reasons it could fail."
7. **Green (Creative — second pass)**: "Given what Black revealed, generate solutions that address the identified risks."
8. **Blue (Evaluate)**: "Summarize: which solution best survived Black and benefited from the second Green pass?"

**Lateral Thinking triggers** (insert before Green passes when stuck):
- Random Entry: "Associate a randomly selected concept — [RANDOM NOUN] — with this problem. What connections appear?"
- PO provocation: "State an impossible version of the solution. What does this reveal about the current constraints?"
- Challenge: "Why does the current approach have the structure it does? What if that structure didn't exist?"

**Machine amplification**: Humans skip uncomfortable hats (Black in optimistic teams, Red in analytical teams). An LLM applies all six with equal completeness. The Red hat is particularly amplified — LLMs can express approximate intuitive assessments without the social cost that suppresses human Red hat expression.
