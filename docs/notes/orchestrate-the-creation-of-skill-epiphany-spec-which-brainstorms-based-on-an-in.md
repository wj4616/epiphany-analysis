# orchestrate the creation of skill epiphany-spec which brainstorms based on an input using graph of thought, finding the true intentions of the human, finding possible flaws, gaps, areas for improvement, asking the human for input on each and helping them resolve it with suggestions and helpful insight. until ideal context created using graph of thought process from which to write the best possible specification document which does not lose details or have harmful hallucination. asking human for review of each section of the solution, allowing for further audit and refinement (integrating with graph and nodes to back track and aggregate all ideas). then writes a specification document using graph of thought and nodes theory to create enhanced cognitive process leading to tangibly super specification document solutions from which an actionable atomic plan for implementation can be written or generated without losing details or hallucinating, only generating enhanced ideal product. solution document should be readable and usable for humans but also optimized for machine use in all cases. The problem will be the run time if we include every feature we want

### Create the ideal optimal enhanced and creative unique academic professional quality industrial grade production ready solution for a solution generation system (brainstorm and specification writing portion)

## VOCABULARY — use these terms exactly, no synonyms

Node: A pipeline processing unit. Has a stage ID, hat, module file, input ports, output
ports, and scale gates.

Module file: The .md file under modules/ that defines a node's PROTOCOL. One module
file per node.

Edge: A directed data connection from one node's output file to another node's input
port. Declared in the PIPELINE GRAPH.

Port: A named attachment point on a node. INPUT ports consume files; OUTPUT ports
produce files.

Wave: A set of nodes that execute in parallel before the next sequential step.

Hat: A cognitive role assigned to a node. Determines which model tier the node runs on.

Tier: A model slot (model-large / model-medium / model-small). Maps to a concrete model
via hats.json.

Conditional edge: An edge that only activates when a declared signal or flag condition
is true.

Join semantics: How a node with multiple incoming edges decides when to fire (AND =
wait for all; XOR = exactly one fires).

this is not a final specification. this all must be optimized and enhanced further into an ideal integrated solution generation system. this is the brainstorming and specifcation writing skill portion. additional portions will include a plan writing skill and implementation creation skill (not creating yet)

We must implement a creative optimal graph of thought or graph of nodes ai agent system, a claude code skill system should by a system which takes input (the input will often be created by tool such as prompt-graph (already built) which enhances a human written input, but could start with an unenhanced input as well. basing everything on graph of thought with nodes, edges between them, all performing actions and passing inputs and outputs back and forth in and out in a circuit of thought like a real cognitive brain, using cognitive genius theory and graph of thought however exploited with AI and computerized machine systems in modern age 2026 ai agents first helps brainstorm using epiphany-spec skill we develop now to fill gaps, clarify and improve things repeatedly with the epiphany-spec skill brainstorming phases, then generates a specification in detail, based on any input but especially enhanced input such as from prompt-graph (graph of thought prompt enhancer) or prompt-epiphany (simple version). once epiphany-spec succesfully runs fully and develops a specificaion, the user should audit and improve the spec, then develop into a plan - however right now we are only developing the brainstorming element. we are developing a solution generation pipeline consisting of 4 skills epiphany-brain, epiphany-spec, epiphany-plan, epiphany-implement . we will develop each system seperately. we are starting with epiphany-brain, but it is designed to output into epiphany-spec, which is using the brain skill we create now, to analyze and then inquire further asking human user repeated questions targetted at clarifying things, resolving any potential issues, and improving everything as a whole repeatedly until everything is perfect, only then is the specifcation written using epiphany-spec. we are only developing one of 4 skills right now, but we must keep in mind the long range goal and full integration of all elements, so that everything works optimally synced.

designing it to take advantages of all possible optimizations and enhancements provided if we think about it as a graph/nodes based skill design with nodes, edges, contained in a modular programatticly run design which is not taking too long to run, can take some time but not so long human user gets frustrated

we are focused on creating a system as described defining nodes, modules, edges, described in tables, creating optimizations and fixes

Aggregation is the defining unlock. It lets multiple independent thought branches merge into a single
richer node — something no human-cognition model can do simultaneously. This is the machine
advantage you need to design around.

Your existing epiphany-graph-genius uses GoT topology (11 nodes, 21 edges) but treats the graph as a
fixed pipeline. The next level is a dynamic, self-modifying graph where intermediate scores reshape
the active topology in real time.

---

SECTION 1 — GoT Architecture Mapped to Your System

The four GoT components (from the KB) and what they become in your Claude Code agent system:

GoT Component → Your System Equivalent
─────────────────────────────────────────────────────────
Controller → Orchestrator agent (SKILL.md / main agent)
Owns: Graph of Operations (static graph.json)
Owns: Graph Reasoning State (live session artifacts)
Decides: which nodes fire, in what order, with what inputs

Prompter → The module file (N\*.md) for each node
Encodes the graph context into the subagent prompt
Includes: predecessor digests, KB files, session dir

Scorer → A dedicated SCORE node + per-node threshold gates
Scores thought quality: coverage, falsifiability, depth
Routes below-threshold nodes to refinement loops (back-edges)

Parser → Structured output contracts per node
Each node writes to a defined schema
Orchestrator reads schema fields — not free text
Updates GRS (session.md + stage artifacts) after each node

The Graph Reasoning State (GRS) maps exactly to your session artifacts directory. Right now it is
write-once per node. The upgrade: make it a live, annotatable document that every node can read AND
update — so later nodes see the full history of all prior thought, not just their direct
predecessors.

Dynamic Graph of Operations (Self-Modifying Topology)

## BRAINSTORM: DYNAMIC GOO — MACHINE ADVANTAGE

Current system: graph.json is fixed before run. Static topology.
Next level: the Controller MODIFIES the active graph mid-run based
on intermediate node outputs.

Rules for dynamic edge insertion:
Rule D1 — Coverage gap detected:
If AGGREGATION node identifies a domain class not covered by any
active branch → insert a new DOMAIN-TARGETED node with that domain
as its constraint survey focus. Wire it into the aggregation node
as an additional input.

    Rule D2 — Thin spread detected:
      If SPREADING node activation map has <3 convergent nodes →
      insert DEFIXATION back-edge (already in epiphany-graph-genius)
      AND insert a RANDOM-ENTRY node (De Bono technique 2) as a
      parallel branch feeding the next aggregation pass.

    Rule D3 — Score stagnation detected:
      If the same idea scores within ±0.5 across 2 refinement passes →
      this is a fixation signal. Insert a REFRAME node that re-encodes
      the problem in a different representational frame before the next
      refinement attempt.

These rules are enforced by the orchestrator after each node
completes — not predetermined. The graph.json provides the base
topology; the Controller extends it with dynamic edges at runtime.

## BRAINSTORM: PARALLEL IDEATION TOPOLOGY (problem: time and token constraint. optimize.)

Instead of sequential ideation nodes, fire N independent ideation
agents simultaneously from the same input — each with a different
cognitive lens. Then run one AGGREGATION node that merges their
outputs and finds cross-branch convergent nodes.

Recommended parallel branches (fire simultaneously):
Branch A — LATERAL (De Bono): constraint escape, cross-domain analogs
Branch B — SPREADING: activation map, convergent node detection
Branch C — SIMULATION: forward-chain top 3 assumptions, find collisions
Branch D — ADVERSARIAL SEED: generate explicit contradictions of the
problem statement before any solution is proposed

The AGGREGATION node receives all four outputs.
Its job is NOT to concatenate. Its job is:

1. Find concepts activated by ≥2 independent branches (cross-branch
   convergent nodes — the highest-signal creative candidates)
2. Find contradictions between branches (Janusian input)
3. Identify domains covered by Branch A but not activated by Branch B
   (coverage gap = seeding target for refinement loop)

Output: a single merged thought with provenance tags showing which
branch each element came from and how many branches independently
reached it.
The following capabilities exceed human cognitive limits.
They are not defaults — they must be explicitly exercised.

**FULL ACTIVATION MAP EXECUTION**
Human spreading activation is bounded by working memory.
This system's activation radius is unlimited.
The SPREADING node must enumerate the complete activation map
as an artifact — not just return top-N results. The full map
is stored in GRS and available to all downstream nodes.

**DETERMINISTIC ANTI-EINSTELLUNG**
LLMs comply with defixation instructions at 100% rate vs 50%
for humans. Every DEFIXATION node must begin with:
"Set aside all previous solution attempts. They are invalid
for this pass. Begin from scratch."
This instruction achieves full override. Use it every pass.

**ZERO-FATIGUE SCORING**
The SCORE node applies identical rigor to thought #1 and
thought #50. Never reduce scoring stringency due to context
length or iteration count. A thought that does not meet
threshold does not advance — regardless of how many iterations
have already occurred.

**PARALLEL BRANCH SIMULTANEOUS FIRE**
All nodes with no dependency edge fire simultaneously.
A human brainstorm runs sequentially in time.
This system runs all independent branches in one wall-clock
window. The parallel fan-out is not a shortcut — it is the
primary quality mechanism. Never serialize nodes that can fire
in parallel.

**GRS AS UNLIMITED WORKING MEMORY**
Human GoT is bounded by 4±1 items in conscious working memory.
This system stores the complete Graph Reasoning State to disk.
Every node reads the full GRS. No information is dropped due
to working memory pressure. Design nodes to READ the GRS
actively — not just their immediate predecessor's output.

**CROSS-RUN COMPOUNDING**
Every session's GRS is archived. The system improves over
multiple runs on the same problem domain by pre-seeding
activation maps from prior session convergent nodes.
Schedule periodic cross-run synthesis sessions to extract
durable patterns from GRS archives into the KB.

Cross-Run Seed Node (Long-Term GoT Memory)

## BRAINSTORM: CROSS-RUN SEED — MACHINE ADVANTAGE

Human brainstormers start from scratch each session.
This system archives every session's GRS to disk.
A CROSS-RUN SEED node reads prior session GRS archives
and pre-populates the current session's activation map.

Protocol:

1. At session start, scan the session archive directory for prior
   runs on related topics (match by topic slug similarity)
2. Load the top-3 prior sessions' convergent node lists
3. Inject these as pre-seeded activation nodes in the current
   SPREADING node (a = 0.6 for prior convergent nodes, vs a = 1.0
   for current primitives)
4. Tag all pre-seeded nodes as [prior-session] in provenance

Effect: each run builds on the strongest insights from prior runs.
The system gets smarter about this problem domain over time.
This is a capability with no human analog.

3.3 — Dependency Graph as GoT Artifact

## SPEC: DEPENDENCY MAPPER NODE

For each accepted spec item, run a FORWARD-CHAIN node that
traces consequences through the current GRS.

Protocol:

1. Read the spec item and all currently accepted items from the GRS
2. Ask: if this item is true, what other items does it constrain?
3. Ask: if this item is true, what new items does it imply that are
   not yet in the spec?
4. Ask: does this item conflict with any existing item?

Output: a dependency edge list for this item: - CONSTRAINS: [item_ids this item restricts] - IMPLIES: [new required items not yet written] - CONFLICTS: [item_ids that contradict this item]

After all items are processed, the orchestrator builds the
dependency graph from all edge lists. This graph: - Drives plan step ordering (topological sort = implementation order) - Surfaces hidden requirements (implied but unwritten items) - Detects circular dependencies (spec design error) - Identifies the critical path (longest dependency chain)

The dependency graph IS the handoff artifact to the plan phase.
The plan step ordering is derived from it, not invented separately.

- you're right to flag this. The dependency graph is the internal reasoning structure — it should never
  be the only artifact a human touches.

The actual design needs two separate things:

1. The human-facing artifact: a flat, structured spec document
   The N-GRS-EXPORT node renders the GRS into a clean readable markdown document — numbered items,
   clearly tagged type (requirement / constraint / assumption), failure clause, confidence score, and
   provenance. This is what gets reviewed, edited, and approved. A human can read it, mark items, add
   notes, delete items, rephrase wording.

2. The machine-facing artifact: the dependency graph
   This is derived FROM the approved spec document, not the other way around. After the human reviews
   and edits the spec document, a parse step reads it and (re)builds the dependency graph. The graph is
   an output of the approved spec, not something the human touches.

The review loop looks like this:

GoT pipeline runs
→ N-GRS-EXPORT writes spec.md (human-readable, flat, numbered)

[HUMAN REVIEW GATE — system pauses here]
Human reads spec.md
Human edits directly: changes wording, deletes items, adds items,
resolves open questions, changes confidence flags

[RESUME — human signals approval or "re-run this item"]

Options at the gate: - Approve whole spec → dependency graph is built → hand off to plan - Approve with edits → parse edited spec.md → update GRS → rebuild graph - Reject specific items → route those items back into the GoT loop
(re-enter at N-FALSIFY or N-REFINE-QUERY for those items only) - Add new items manually → inject into GRS → run through N-FALSIFY
and N-DEPENDENCY-MAP for the new items only

The key design principle: the graph is a derived artifact, not an editable one. Humans edit the
document. The system parses changes and updates the graph automatically.

This also solves iterative improvement — you can run review cycles multiple times. Each cycle: human
edits the spec doc → system re-parses → graph is updated → only changed items re-run through the GoT
nodes → new spec doc version is written. You get versioned spec documents (spec-v1.md, spec-v2.md)
and a complete audit trail.

The dependency graph only needs to be human-visible as a summary in the spec doc — something like a
section at the bottom showing which items depend on which, rendered as a simple list not a graph
diagram.

We can implement process of ideation, synthesis, and verification as seen in other skills such as prompt-graph (graph of thought based) and prompt-epiphany(original basic version of same concept). these skills take an input and generate a more detailed enhanced output, original new ideas surface as part of the ideation and synthesis process, the validation and verifation for accuracy, utility, etc. ensures everything is not a hallucination, these skill based systems are proven to work, but we need a slightly different system in this use case, and we can't let things take so long to run as the other skills can take 20 minutes just to optimize a prompt, we need to generate an atomic specifcation in full

considering the input to this may be enhanced data based on solution like prompt-graph, there will be fields such as the "role" which should be used for example. the skill should properly use these fields which are typically designated in semantic XML notation when created by our existing prompt enhancement skills

return semantically useful detailed markdown specifcation document

we want to develop our own unique skill exploiting graph of thought theory fully in unique creative ways, keeping in mind this process is going to take longer than desired to run most likely, so we want to optimize.

the biggest challenge is ensuring our documents contain all details of the original document, create an enhanced solution synthesis which is validated and verified fully while not hallucinationg and coming up with false data, making a solution accurate to the original intention of the user

and orchestration logic implementing a GoT controller mechanism orchestrator which can provide enhancements and fixes, but we want to make the system flow well, not bog it down with excessive engineering that does not add value and only makes the system slower, value utility to human

the agents work better if we take our input of requirements and details, first analyze everything, ensure we help brainstorm and find exactly what the user wants by asking clarifying questions repeatedly and then presenting the design in sections for the human to improve audit and offer further input and clarification. by using graph of thought we can create an ideal process which develops the best solutions, and create a detailed specification using the agent skill. We are developing the brainstorming portion, which must be designed to pass the brainstorming findings plus all original data to the specification writing skill

am trying to develop an solution generation system usable by ai agents

- Idea structuring (this is the unique part):

Every idea gets captured as a structured card, not prose. Fields: core mechanism, what it optimizes for, what it sacrifices, dependencies, failure modes, novelty score, rough complexity
Cards are comparable — same fields across all ideas means the spec writer can do real tradeoff analysis instead of vibes
Tag ideas with their "shape": is this a swap (replace component X), a wrap (add layer around X), a split (decompose X), a merge (combine X+Y), an invert (flip control flow), a defer (push decision later)? Shape vocabulary makes patterns visible

idea for a specification writing system for our ai agent skill systems in
claude code which fully explots ai systems and the graph of thought or graph and node system in ai agents.

- Convergent / pruning phase:

Pareto filter: drop any idea strictly dominated by another (worse on every axis). What survives is the actual decision space
Tradeoff matrix: surviving ideas plotted against the constraints/goals from the input phase. Forces explicit comparison rather than narrative preference
Hybrid pass: can the best parts of idea A combine with idea B? Often the winner is a chimera, not a single original idea
Kill criteria: explicit reasons each rejected idea was rejected. The spec writer needs the why-nots as much as the whys

- Adversarial stage (between brainstorm and spec):

Pre-mortem per surviving idea: imagine it failed in production — what went wrong? Failure modes become spec requirements
"What would change our mind?": for the leading idea, what evidence or constraint would flip the decision? This becomes the spec's risk section
Devil's advocate pass: explicitly argue against the leading idea. If the defense is weak, surface that
Boring-baseline check: does the leading creative idea actually beat the boring one by enough to justify its added complexity? If not, demote it

- Decision capture:

Explicit recommendation with confidence: the brainstorm doesn't just list options — it picks one and states confidence. Spec writer can override but starts from a position
Decision log with alternatives preserved: rejected ideas don't disappear. They're stored with kill reasons so the spec writer (or future agent) can revisit if context changes
Open questions queue: anything the brainstorm couldn't resolve becomes a structured question for the user or the spec phase. Nothing gets quietly dropped

Idea provenance tags: which lens or technique produced each idea. Helps debug the brainstorm — if all winners came from one lens, you under-explored
Novelty vs. risk axis: every idea plotted on (novelty, risk). Lets the user/spec phase pick their tolerance band

designing it to take advantages of all possible optimizations and enhancements provided if we think about it as a graph/nodes based skill design with nodes, edges, contained in a modular programatticly run design which is not taking too long to run, can take some time but not so long human user gets frustrated

we are focused on creating a system as described defining nodes, modules, edges, described in tables, creating optimizations and fixes

and orchestration logic implementing a GoT controller mechanism orchestrator which can provide enhancements and fixes, but we want to make the system flow well, not bog it down with excessive engineering that does not add value and only makes the system slower, value utility to human

this tool should be able to be later integrated to other skill system such as prompt-graph, which is a prompt-enhancement tool. we want to develop the spec and plan writing systems seperately first, then integrate to the other tool later. they are both graph of thought or graph nodes system

the biggest challenge is ensuring our documents contain all details of the original document, create an enhanced solution synthesis which is validated and verified fully while not hallucinationg and coming up with false data, making a solution accurate to the original intention of the user

we have already created spec modes in our prompt enhancement skill prompt-epiphany, but they are not fully functioning. we need a completely new system.

focus on harnessing and exploiting machine advantage and nature of computer and artificial intelligence in a graph of thought or graph nodes interface design,

modular design running things programmaticaly within reason, may take longer than desired to run tool so must optimize for speed while ensuring accuracy and validity of spec document.

return semantically useful detailed markdown specifcation document , possibly using light semantic XML if appropriate as we have done in other skills. containing specification document, depending on which context was given by the user.

### concept beta partial details:

- can gather context if user references it, such as mentionaing a specific file, project, directory, then look at the mentioned area. we do not want to gather context if not relevant or not wanted by human user
- This will require a series of steps to ensure the user has every possible opportunity to give input before the writing begins

Quantity gate before quality gate: require N raw ideas before any evaluation begins. Premature filtering kills the search

Create the ideal optimal enhanced and creative unique academic professional quality industrial grade production ready solution

        Input absorption phase:

Restate-before-explore: agent paraphrases the request back in its own structured form before generating any ideas. Catches misreads early and creates a stable reference the rest of the brainstorm anchors to

Intent layering: separate what the user asked for from what they're trying to achieve from what they'd actually accept as success. Ideas should target the deepest layer
Context inventory: list what's known, what's assumed, what's missing. Missing items become brainstorm prompts ("if X were true vs. false, the solution differs — explore both")
Constraint extraction first, ideas second — most brainstorms fail by generating ideas that violate constraints nobody wrote down. Lock the box before filling it

- Context Decomposition Phase (not limited to this, only starting point)
- Split the original input into atomic problem statements, each stored with a unique ID
- Tag each problem: functional requirement / constraint / assumption / open question / dependency
- Identify which problems are prerequisites for others (dependency graph)
- Flag ambiguous or underspecified problems before moving forward, asking user repeatedly for more context until each problem is individually resolved, giving plenty of opportunity for audit at each step of the skill, and to provide additional imputs to creatively shape the solution while ensuring the human does not make things worse. if the human is making things worse with a design decision, we need to warn them explicitly.
- We need a methodology which will create a robust professional grade solution creation or generation system implemented as an ai agent skill in claude code in 2026 modern tech and models

Specification Audit (not limited to this, only starting point)

- Auto-generate an audit checklist derived from the spec items themselves
- Risk/complexity rating per spec item (feeds into plan step ordering)
- Scope boundary definition — explicit list of what is out of scope
- "Strawman test" — describe what a broken implementation would look like, to sharpen the spec
- Quality Gates (not limited to this, only starting point)
- Spec-to-plan coverage matrix — every spec item must map to at least one plan step
- "Atomic test" for each step — if a step has two verbs it needs to be split
- Dead-end detection — steps that produce no output consumed by later steps
- Missing precondition check — steps that assume state not established by prior steps
- (not limited to this, only starting point)  
   Context Preservation
- Store original user intent verbatim at every phase — never let paraphrasing drift from it
- Delta tracking — when spec evolves during audit, record what changed and why
- "Intent alignment check" at plan completion — does the final plan still match the original stated goal?

- (these are not all the phases. these are just sample phases. improve and create our own solution which is new and much better, more efficent and makes better solution implementation based on initial brainstorming and spec writing)

- Phase — Intake & Decomposition  
   Receive raw input. Extract and store atomic problem statements with IDs. Preserve original wording verbatim. Tag  
   each item: requirement / constraint / assumption / open question / dependency.
- Phase — Dependency Mapping  
   Build a lightweight graph: which problems depend on which. Identify blockers (can't spec X until Y is resolved).  
   Surface circular dependencies as immediate errors.
- Phase — Ambiguity & Conflict Pass  
   Scan all items for vague language and contradictions. Generate a targeted clarifying question for each unresolved  
   item. Pause and surface questions before proceeding — do not assume answers.
- Phase — Specification Construction  
   Write the formal spec in at leastt four layers: What (functional), How (technical approach), Why (motivation), Constraints  
   (scope/limits). Every spec item references its source problem ID. be sure all claims are validated and verified, all details from original context accounted for
- The point of a specification document is to accurately and fully describe the requirements, elements, details etc of a given entity or problemset in a manner that an implementation plan can be created, and also reviewed by humans as well. the output of these tools should be readable by humans and also ideal for use by ai machine systems

## should be readable and usable by humans for manual review

Phase — Spec Audit  
Auto-generate audit checklist from spec items. Score completeness. Define explicit out-of-scope boundary. Run  
"strawman test" — describe what a broken implementation looks like to sharpen each item. Assign risk/complexity  
rating per item.

Phase — Spec Sign-off Gate  
Check completeness score against threshold. All open questions must be resolved. Conflicts must be resolved or  
explicitly accepted. Output: a frozen, versioned spec document.

- Structural ideas worth stealing:

The brainstorm has phases, not freeform — diverge, structure, prune, adversarial, decide. Each phase has clear entry/exit conditions so the agent can't loop or skip

---

---

Every phase produces a written artifact that the next phase consumes. This is what lets it pass full detail to the spec writer in the same skill — the spec doesn't read the user's input, it reads the brainstorm's output
Confidence checkpoints: between phases, the agent declares confidence. Low confidence triggers another divergent pass rather than premature convergence
Time-box per phase: prevents the brainstorm from becoming the whole solution. Brainstorms that don't end on time produce worse specs than brainstorms that end early

Branch budget: the brainstorm can hold N parallel ideas at most. Forces pruning rather than infinite expansion

Vocabulary alignment: the brainstorm extracts and locks domain terms early so ideas don't drift in meaning. Same term across all idea cards means the same thing

Output handoff to spec writer (beta test idea - improve all ideas):
The brainstorm artifact should hand off:

The locked vocabulary
The constraint set
The chosen idea with its full structured card
The rejected alternatives with kill reasons
The open questions queue
The pre-mortem failure modes (these become spec invariants)
The confidence level on the choice

If the spec writer has all seven, it can produce a defensible spec without re-doing the analysis.

- Split raw input into Atomic Problem Units (APUs), each with an ID, type tag (functional/constraint/edge-case/architectural/non-goal), and a confidence score on how well it was understood  
  Run an "implicit requirements" pass — what the user didn't say but obviously expects (error handling, logging, auth, etc.)

- Tag each APU with provenance: was it stated explicitly, inferred, or assumed? Assumed items get flagged for confirmation  
  Detect contradictions between APUs early and surface them as a dedicated "conflict ledger" rather than letting them propagate

- Build a dependency graph between APUs — which problems block which, which are parallelizable  
  Classify each APU on two axes: certainty (do we know what to build?) and complexity (do we know how?). The four quadrants get different treatment downstream  
  "Unknown unknowns" probe — actively generate questions the user probably hasn't considered (failure modes, scale assumptions, security boundaries)  
  Constraint extraction pass — separate hard constraints (must) from soft preferences (should) from aesthetics (nice). Plans treat these very differently

- Build the spec as a layered document: invariants at the top (things that must always be true), then interfaces/contracts, then behavior, then implementation hints. This ordering matters because it forces thinking about what can't change before what will

Every spec section links back to the APU IDs that justify it — full traceability, no orphan requirements

"Falsifiability check" — for each requirement, define how you'd know it's been met. If you can't write the test, the requirement is too vague

Include an explicit non-goals section. What this spec deliberately does not address is often more valuable than what it does

&nbsp;

**Core pipeline metaphor options to consider:**

- **Distill → Crystallize → Fracture** — raw input distilled to problems, crystallized into a spec, then fractured into atomic steps
- **Excavate → Survey → Blueprint → Deploy** — archaeological framing where you dig out hidden requirements before designing

we have a dify kb about cognitive science. dont integrate to the skill we need to use the KB to design the skill and think hard about the solution
generation system, and how we can integrate the traits we studied and put into this kb, to develop a
more effective system of brainstorming and specification writing using ai agent claude code system.

the system is capable of human like reasoning and completes tasks in a fraction of the time, with
very powrful memory, and all other advantages of machine computerized systems and ai agents like
claude code in 2026.

## BRAINSTORM PHASE — Cognitive Trait Application

Before generating any ideas, activate three parallel processes:

**1. Constraint Inventory (Ohlsson)**
List every assumption embedded in the input. Label each as:

- Hard constraint (non-negotiable physical/logical requirement)
- Soft constraint (assumed but relaxable)
- Ghost constraint (implied but never stated)
  For every soft and ghost constraint: ask what solutions open if this constraint is removed.

**2. Spreading Activation Pass**
Extract all problem primitives from the input. Propagate outward
conceptually 3–5 steps from each primitive. Flag any concept activated
by 2 or more independent primitive chains — these convergent nodes are
the hidden bridges and highest-signal creative candidates.
Enumerate the activation map explicitly; do not discard non-top results.

**3. Janusian Contradiction Generator**
For each major problem element:

- State the goal clearly
- State its explicit opposite
- Hold both simultaneously — do not resolve prematurely
- Seek a synthesis that transcends both (not a compromise)

3. Chunk Decomposition for Ambiguity Detection\*\*
   For each element you are treating as a single unit: break it into
   constituent parts. Ask: which sub-elements could be specified or
   implemented independently? Anything that cannot be subdivided is truly
   atomic — flag these as atomic spec items we need a specifcation in enough detail a full atomic plan can be written, the spec is not yet at atomic plan level, but it is in atomic detail relative to original input.

Assumptions are not requirements — list them separately and mark which
are load-bearing

**Original Intent Preservation**
The user's original wording is stored verbatim at intake and is
never paraphrased. At every phase transition, read the original
wording and ask: does the current output still serve that intent?
If the answer is unclear, surface it — do not assume alignment.

**Darwin's Disconfirmation Rule**
Any observation or result that contradicts the current model must be
written down immediately and explicitly preserved. Never allow
disconfirming evidence to be discarded by omission. The model must
update or the evidence must be explicitly rejected with documented
reason.
These are capabilities that exceed human cognitive limits and should
be explicitly exercised — not treated as defaults:

**Full Activation Map**: Human spreading activation is bounded by
working memory; only top-activation nodes reach consciousness.
This system's activation radius is orders of magnitude larger.
Execute the full activation map — do not return only top results.

**Anti-Einstellung Compliance**: Humans comply with de-fixation
instructions ~50% of the time. This system complies 100%. The
explicit instruction "set aside all prior solution attempts —
this pass starts from scratch" achieves full override. Use it.

**Fatigue-Free Metacognitive Loop**: Human metacognitive monitoring
degrades under load. This system can apply Planning-Monitoring-
Evaluating at every step without degradation. Never skip the
monitoring pass due to "it looks fine."

**Exhaustive Constraint Enumeration**: Humans miss implicit
constraints because they never enter working memory. This system
can enumerate all embedded assumptions in a problem representation
exhaustively. The constraint inventory must be complete, not
approximate.

- ensure when you write the file for the specifcation document in ~/docs/solution , that we writing it in chunks so that it doesn't time out without finishing, and we are careful we don't lose any data. give the file a name starting with date so that it is sorted, with some descriptive words as well. this is the primary output of the skill. this output can then be later refernced, after spec is audited.

# Create the ideal optimal enhanced and creative unique academic professional quality industrial grade production ready solution for ai agent skill epiphany-spec which takes all details and requirements into account, ensuring it will run smoothly and quickly as possible while retaining quality, producing output ideal for later integration with a plan writing skill (yet to be developed)
