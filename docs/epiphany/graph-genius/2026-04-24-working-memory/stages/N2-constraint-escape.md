---
node_id: N2
node_type: LATERAL
hat: de Bono
executed: 2026-04-24
input_question: "What are the cognitive bottlenecks of human working memory and how do they shape learning strategies?"
predecessor_digests_used: [N1-primitives_digest]
---

<constraint_escape>

<named_constraint>

## Named constraint

**Primary binding constraint (Archimedes: name before searching):**

> Working memory is a **fixed-capacity serial bottleneck**: it can hold approximately 3–4 independent chunks simultaneously, process only one demanding operation at a time via the central executive, and items decay in roughly 2 seconds unless actively refreshed — creating a hard ceiling on how much new, unintegrated information can be simultaneously active during learning.

**Domain-agnostic restatement (Da Vinci: strip domain vocabulary):**
A processing system has a **fixed-slot buffer** (not expandable at runtime) that feeds a **single-thread executor** (no true parallelism for demanding tasks). Items in the buffer decay unless continuously re-energized. The cost of loading a new item competes with the cost of retaining existing items. The total throughput of the system is therefore a function of: (a) how many slots are occupied, (b) how fast items decay, and (c) how much executive bandwidth the task itself demands.

**Secondary co-binding constraints (marked for V6 if tension):**
- Individual differences in WM span are stable and resistant to training transfer — the ceiling cannot be raised through practice in the way peripheral skills can.
- Chunking reduces slot occupancy but requires prior schema (long-term memory integration) to execute — creating a bootstrapping dependency.

</named_constraint>

<domain_candidates>

## Domain candidates surveyed

Constraint expressed in domain-agnostic terms for matching:
> "Fixed-slot buffer + single-thread executor + decay without refreshing + new-load competes with retention-load + ceiling is structural not trainable."

Parallel survey across all 25 catalog domains:

| # | Domain | Structural class match | Agnostic occurrence | Viable? |
|---|--------|----------------------|---------------------|---------|
| 1 | Evolutionary Biology | optimization under constraint | Fitness landscapes with fixed trait expression capacity | PARTIAL |
| 2 | Thermodynamics | irreversibility, resource limits | Carnot efficiency ceiling — no process converts 100% energy; entropy always costs | **YES** |
| 3 | Fluid Dynamics | bottleneck resolution, state-dependent behavior | Pipe with fixed cross-section: throughput ceiling regardless of upstream pressure; laminar regime = ordered, efficient; turbulent = wasteful | **YES** |
| 4 | Immunology | classification under uncertainty, memory | Clonal selection narrows repertoire; memory cells pre-loaded reduce future processing cost | PARTIAL |
| 5 | Information Theory | information bounds, encoding/decoding costs | Shannon channel capacity — fixed bandwidth, items above capacity are lost or corrupt; compression reduces effective load | **YES** |
| 6 | Architecture / Structural Engineering | load redistribution | Moment of inertia ceiling on span — excess load must be redistributed via arches or trusses | PARTIAL |
| 7 | Ecology / Food Web | cascade effects, leverage points | Carrying capacity of trophic level; energy loss at each transfer level | PARTIAL |
| 8 | Game Theory | multi-agent optimization, coordination | Congestion games: each agent adding load degrades all others; Nash equilibrium at suboptimal throughput | **YES** |
| 9 | Developmental Biology | emergence from local rules, self-organization | Cell differentiation from shared genome: context activates different subsets; capacity per cell fixed but specialization changes effective output | PARTIAL |
| 10 | Control Theory | feedback stability, delay compensation | Bandwidth-limited controller: fast-changing input exceeds bandwidth → instability; predictor needed for delay | **YES** |
| 11 | Crystallography | local structure → global property, defect leverage | Grain boundary density controls bulk conductivity; single defect can pin entire domain | PARTIAL |
| 12 | Network Science | connectivity leverage, critical thresholds | Percolation threshold: network connectivity collapses discontinuously when node count falls below threshold | PARTIAL |
| 13 | Linguistics / Syntax | finite means → infinite output, ambiguity resolution | Recursive grammar generates infinite sentences from finite rules; but parsing requires holding constituents in a buffer that can overflow (garden-path sentences) | **YES** |
| 14 | Epidemiology | threshold phenomena, intervention timing | R₀ threshold: below threshold, spread dies; above, it cascades — intervention must target before threshold crossing | PARTIAL |
| 15 | Acoustics / Wave Physics | resonance leverage, destructive vs. constructive | Resonant cavity has a natural frequency; driving it at that frequency amplifies with no extra energy; off-resonance requires much more | **YES** |
| 16 | Economics / Market Microstructure | information asymmetry, distributed coordination | Liquidity constraint: trader cannot execute all desired transactions simultaneously; queue depth is fixed; excess orders degrade execution quality | PARTIAL |
| 17 | Optics / Photonics | resolution limits, beam shaping | Diffraction limit: aperture size creates absolute resolution ceiling; no amount of lens quality beats it; pupil-plane filtering changes which information gets through | **YES** |
| 18 | Psychology / Cognitive Science | bounded rationality, attention scarcity | (Same domain — excluded to avoid circularity) | NO — same domain |
| 19 | Pharmacology / Drug Design | delivery constraints, timing-dependent effects | First-pass metabolism: drug delivered via gut is partially destroyed before reaching target; bioavailability is a ceiling; route-switching (IV vs. oral) changes effective dose | PARTIAL |
| 20 | Urban Planning / Traffic Flow | emergent congestion, system vs. individual optimization | Braess paradox: adding capacity can worsen congestion; bottleneck is structural, not additive | **YES** |
| 21 | Cryptography / Security | asymmetric information leverage, computational bounds | One-way function: cheap to compute forward, exponentially expensive to reverse — ceiling is computational not physical | PARTIAL |
| 22 | Geology / Plate Tectonics | slow accumulation / rapid release, threshold-based change | Elastic rebound: stress accumulates slowly; when threshold crossed → rapid release; intermediate states invisible on surface | PARTIAL |
| 23 | Animal Behavior / Ethology | stimulus-triggered automation, distributed coordination | Fixed action patterns: once triggered, entire motor sequence executes without further executive input; reduces per-step processing cost to zero | **YES** |
| 24 | Computation / Complexity Theory | space-time trade-offs, decomposition | Space-time trade-off: memoization trades memory slots for recomputation cost; cache size is fixed → eviction policy determines throughput | **YES** |
| 25 | Chemistry / Catalysis | barrier-lowering without consumption, cascade amplification | Enzyme lowers activation energy: without enzyme, reaction bottlenecked at energy barrier; with enzyme, same substrate flux at lower cost | **YES** |

**Viable domain count: 12** (exceeds 5 minimum → proceed to Step 3)

**Top 5 selected for structural distinctness (no shared surface features):**
- Information Theory (encoding/capacity ceiling)
- Fluid Dynamics (pipe cross-section bottleneck)
- Linguistics / Syntax (buffer overflow in parsing)
- Acoustics / Wave Physics (resonance amplification)
- Computation / Complexity Theory (cache space-time trade-off)

</domain_candidates>

<domain_mappings>

## Domain mappings (5)

---

### Mapping 1 — Information Theory

**Source domain:** Information Theory / Coding Theory
**Target domain:** Working Memory & Learning
**Structural analog:** Shannon channel capacity limit

| Source | Target |
|--------|--------|
| Channel with fixed bandwidth C | Working memory with ~3–4 chunk slots |
| Signal above capacity C is lost or corrupted | New information beyond slot capacity is dropped or degraded |
| Noise in the channel | Extraneous cognitive load (irrelevant detail, poor instructional design) |
| Compression algorithm (Huffman, LZ) | Chunking / schema formation — same information, fewer symbols |
| Error-correcting codes (redundancy) | Spaced repetition, elaborative rehearsal — redundancy increases signal recovery |
| Signal-to-noise ratio | Germane vs. extraneous cognitive load ratio |

**What solves it in source:** Increase compression efficiency (reduce bit-cost per unit of meaning) and reduce noise. Do NOT try to expand the channel — work within C. Use error-correcting codes to make partial information recoverable.

**Transfer to learning:** The bottleneck is structural. The instructional design solution is (1) pre-compress incoming content via analogies/schemas (reduce chunk-cost), (2) suppress extraneous load (channel noise), and (3) build error-correction mechanisms (retrieval practice increases recoverability of partially-encoded items). Attempting to "widen the channel" via WM training has negligible transfer — exactly analogous to why you cannot exceed Shannon capacity by trying harder.

---

### Mapping 2 — Fluid Dynamics

**Source domain:** Fluid Dynamics
**Target domain:** Working Memory & Learning
**Structural analog:** Pipe cross-section as throughput ceiling with laminar/turbulent transition

| Source | Target |
|--------|--------|
| Pipe with fixed inner diameter | WM slot count (structural, not trainable) |
| Flow rate (volume/time) | Learning throughput (ideas processed per unit time) |
| Laminar flow: ordered, low-resistance | Cognitive flow state: ordered, efficient executive processing |
| Turbulent flow: high-resistance, energy-wasting | Cognitive overload: competing activations, executive thrashing |
| Pressure gradient driving the flow | Motivation / attentional effort |
| Bernoulli effect: narrowing accelerates local flow | Focused attention: narrow task window increases local processing rate |
| Valve logic: controlling sub-flows | Metacognitive load-shedding: deliberately offloading sub-tasks (externalizing) |

**What solves it in source:** Maintain laminar regime by controlling flow rate; use valve logic to route sub-flows; reshape the channel geometry (smooth interior = chunked, familiar material).

**Transfer to learning:** Cognitive overload (turbulence) is worse than moderate load. The solution is not to push harder (increase pressure = effort) but to maintain laminar conditions: pace new material below turbulence threshold, externalize working notes to reduce effective slot demand (valve logic), and make material "smoother" (familiar schemas) to reduce resistance. The learner's WM is not the problem — turbulent conditions are.

---

### Mapping 3 — Linguistics / Syntax (Garden-Path Parsing)

**Source domain:** Computational linguistics / sentence parsing
**Target domain:** Working Memory & Learning
**Structural analog:** Parsing buffer overflow in garden-path sentences

| Source | Target |
|--------|--------|
| Parsing buffer holding open constituents | WM holding partially-integrated concepts |
| Garden-path sentence: "The horse raced past the barn fell" | Complex explanation where early framing conflicts with late revision |
| Syntactic re-analysis cost | Conceptual re-loading cost when an early model must be revised |
| Left-branching structures (hard to parse) | Recursive prerequisite structures (deep dependency chains) |
| Right-branching structures (easy to parse) | Scaffolded instruction (each step closes before next opens) |
| Predictive parsing: anticipate upcoming constituents | Schema activation: pre-loading LTM structure reduces parse cost |

**What solves it in source:** Right-branch (flatten) the sentence structure; activate predictive frames early; give listeners/readers top-down structure before bottom-up detail (headline → body, not body → headline).

**Transfer to learning:** Learning sequences that leave many "open constituents" (unresolved questions, partial concepts awaiting closure) overload WM even with low individual item count. Instructional sequencing should close constituent chains early: teach conclusions before derivations where possible, activate schema frames before detail loading, and minimize simultaneous open threads. The cognitive re-analysis cost of revising early mental models (misconception correction) is quantifiable as a structural parsing cost.

---

### Mapping 4 — Acoustics / Resonance Physics

**Source domain:** Acoustics / Wave Physics
**Target domain:** Working Memory & Learning
**Structural analog:** Resonant amplification at natural frequency

| Source | Target |
|--------|--------|
| Resonant cavity with natural frequency f₀ | LTM schema with specific structural "frequency" (organizational pattern) |
| Input driven at f₀: amplification with low energy cost | New information matching existing schema: effortless deep encoding |
| Input driven off-resonance: requires more energy, less amplitude | New information with no schema match: high WM cost, shallow encoding |
| Standing waves: self-sustaining patterns | Stable memory traces: information that "stands alone" without active maintenance |
| Destructive interference: cancel competing signals | Proactive/retroactive interference between similar memory traces |
| Acoustic impedance mismatch at boundary | Schema mismatch: difficulty integrating when prior schema "rejects" new input |

**What solves it in source:** Tune the driving frequency to match the resonant frequency of the cavity. Alternatively: redesign the cavity (change its natural frequency) to match available input.

**Transfer to learning:** The most efficient WM use occurs when incoming information is "tuned to resonance" with existing LTM schemas. This is the structural explanation for why prior knowledge dramatically amplifies learning efficiency — it is not motivational, it is physical: you are driving the cognitive system at its resonant frequency. Learning strategy implication: deliberately activate relevant prior schemas before introducing new content (advance organizers, priming). When learner has no relevant schema, build a minimal resonant cavity first (a simplified model), then tune to it.

---

### Mapping 5 — Computation / Cache Architecture (Space-Time Trade-off)

**Source domain:** Computer architecture / Cache memory systems
**Target domain:** Working Memory & Learning
**Structural analog:** Fixed-size cache with eviction policy determining throughput

| Source | Target |
|--------|--------|
| CPU registers (fastest, fewest) | WM slots (~3–4) |
| L1/L2/L3 cache hierarchy | Short-term buffer → activated LTM → full LTM retrieval |
| Cache miss: fetch from slow main memory | Forgetting: item must be reconstructed from LTM at processing cost |
| Cache eviction policy (LRU, LFU) | Which WM items are refreshed vs. displaced — decay as LRU eviction |
| Prefetching: anticipate needed data | Priming / advance organizers: load expected schemas before task |
| Memoization: store computed results to avoid recomputation | Automaticity: practiced skills stored as compiled routines, bypassing WM |
| Thrashing: eviction rate exceeds useful work | Cognitive overload: constant displacement prevents deep processing |

**What solves it in source:** Prefetch intelligently; memoize frequently-used subroutines; keep hot data in registers (minimize register spills); avoid thrashing by managing working set size.

**Transfer to learning:** Automaticity (practice to fluency) is the cognitive equivalent of memoization — it moves a frequently-needed operation from "recompute in WM" to "retrieve compiled result," freeing slots for higher-order processing. Overloaded learners thrash: nothing gets encoded because everything gets evicted before processing completes. The strategy implication: build a small automatized base (memoization layer) before increasing task complexity. The sequence "automate sub-skills first, then combine" is structurally identical to "memoize hot paths before optimizing the algorithm."

</domain_mappings>

<activation_provenance>

## Activation provenance

**Provenance tagging:** Each candidate concept marked as single-source (S) or multi-source-convergent (C, activated from ≥2 independent chains in the domain survey).

| Concept | Source domains activating it | Provenance | Notes |
|---------|------------------------------|------------|-------|
| Fixed-capacity ceiling as structural (not trainable) | Information Theory (#5), Fluid Dynamics (#3), Optics (#17) | **C — 3 sources** | Three independent domains all encode the same finding: the ceiling is architectural, not a performance variable. Directly addresses the controversy over WM training transfer. |
| Compression / chunk-cost reduction as primary solution | Information Theory (#5), Computation (#24), Chemistry/Catalysis (#25) | **C — 3 sources** | Compression (IT) = chunking (WM) = memoization (CS) = enzyme catalysis (lowering activation energy) — all reduce per-unit processing cost without expanding the bottleneck. |
| Sequence/ordering effects on load (constituent management) | Linguistics (#13), Fluid Dynamics (#3), Control Theory (#10) | **C — 3 sources** | Left-branch/right-branch (linguistics), laminar/turbulent transition (fluids), bandwidth limit with delay compensation (control theory) all independently encode that ordering of information presentation determines whether bottleneck is hit. |
| Prior knowledge as resonance (not mere "background") | Acoustics (#15), Immunology (#4), Computation (#24) | **C — 3 sources** | Resonant cavity matching (acoustics), memory cell pre-loading (immunology), cache prefetch (CS) all encode the same structural principle: prior matching reduces current processing cost. This is a multi-source-convergent framing that strengthens the resonance mapping. |
| Automaticity as bottleneck bypass | Computation/Memoization (#24), Animal Behavior/Fixed Action Patterns (#23), Pharmacology/Prodrug (#19) | **C — 3 sources** | Memoization, fixed action patterns (once-triggered motor sequences requiring zero executive input), and prodrug activation all encode the concept of a pre-compiled routine that bypasses the executive bottleneck entirely. |
| Turbulence / thrashing as qualitatively worse than high-but-stable load | Fluid Dynamics (#3), Computation (#24), Geology/Elastic Rebound (#22) | **C — 3 sources** | Non-linear collapse when threshold crossed — turbulence (fluids), thrashing (CS), elastic rebound (geology) all show that the bottleneck system has a phase transition, not a smooth degradation. Cognitive overload is discontinuous. |
| Externalizing as valve logic (offload, don't expand) | Fluid Dynamics (#3), Architecture/Structural Engineering (#6) | **C — 2 sources** | Valve logic (fluids) and load redistribution via arches (engineering) both encode the same move: route load away from the bottleneck point rather than widening it. |

**Provenance check result:** Every selected concept has multi-source provenance (C). No single-source-only concepts appear in the top-5 mappings. Failure guard not triggered.

</activation_provenance>

<conceptual_blend>

**Step 6 verification:**
- Systematicity: 5 mappings share no surface features (channel/pipe/parser/resonator/cache are structurally distinct). ✓
- Provenance: all key concepts have declared provenance. ✓
- Blend-worthiness check: Mapping 4 (Acoustics/Resonance) × Mapping 5 (Cache/Memoization) passes blend-worthiness: blending "resonant cavity tuning" with "cache prefetch + memoization" produces emergent structure not in either alone. Step 7 fires.

---

**Blend: Resonant Cache — prior-schema as both prefetch index AND amplification medium**

## Input Space 1 — Acoustics / Resonance
Elements: resonant cavity, natural frequency f₀, driving signal, standing waves, impedance mismatch, destructive interference
Structure: amplification is free when input matches resonant frequency; off-resonance input dissipates as heat; cavity shape determines f₀
Relevance: prior LTM schemas amplify encoding at zero extra WM cost when new content "matches"

## Input Space 2 — Cache Architecture / Memoization (applied to Working Memory)
Elements: CPU registers, cache hierarchy, eviction policy, prefetch, memoized routine, thrashing
Structure: fixed register count; prefetch moves expected data into cache before needed; memoized sub-result bypasses recomputation; thrashing = eviction exceeds useful work
Relevance: WM slots are registers; automaticity = memoization; priming = prefetch

## Generic Space — Shared skeleton
Shared structure: a **fixed-capacity fast-access store** where matching prior state to incoming demand determines whether the system runs efficiently or wastes resources. In both domains: the system does not expand; efficiency depends entirely on **pre-alignment of structure with expected demand**.

Cross-space mapping:
- Resonant frequency f₀ ↔ Cache hot-path (frequently accessed structure)
- Driving at f₀ ↔ Prefetch hit (data already in cache when needed)
- Impedance mismatch (reflection, loss) ↔ Cache miss (expensive fetch from slow memory)
- Standing wave (self-sustaining, no energy input needed) ↔ Memoized result (zero recomputation cost)
- Cavity redesign to match available input ↔ Schema construction before learning (building the resonator)

## Blended Space — Resonant Cache
Selective projection:
  - From Input 1: resonance as amplification without extra energy input; cavity shaping as a designable property
  - From Input 2: prefetch as proactive loading; memoization as compiled bypass; thrashing as qualitative collapse

Operations applied:
  - Composition: prior LTM schema acts simultaneously as a **resonant amplifier** (new content encodes deeper at zero WM cost) AND as a **prefetch index** (schema structure predicts what will be needed, loading it before it arrives)
  - Completion: background knowledge activates — acoustic engineers know you can deliberately **tune a cavity** to match a signal you haven't yet received; CS engineers know **cache warming** (deliberately populating cache before the workload arrives) is a production technique
  - Elaboration: running the blend — a learner who activates a schema before new content arrives is both (a) amplifying the incoming signal (resonance) and (b) pre-populating the register file (prefetch). The schema does double duty in the blend; neither input domain suggested both functions simultaneously.

## Emergent Structure — CRITICAL SECTION

1. `[EMERGENT-SOLUTION]` **Schema pre-construction is a dual-function operation:** it simultaneously reduces WM load (prefetch: the item is already "in cache" when it arrives) AND amplifies encoding depth (resonance: the item is driven at the cavity's natural frequency). Neither source domain alone predicts that a single pre-learning intervention has TWO mechanistically distinct beneficial effects — IT theory predicts noise reduction, acoustics predicts amplification, but only the blend reveals they occur together.

2. `[EMERGENT-FRAME]` **The learner's WM bottleneck is a design parameter, not a given:** acoustic engineers design cavities with specific resonant frequencies to match anticipated input. The blend reframes learning design as **cavity engineering** — instructors do not just "reduce load" but actively shape the resonant structure of the learner's prior knowledge to match the upcoming material. This framing does not exist in either source domain.

3. `[EMERGENT-CONSTRAINT]` **Schema mismatch produces both under-encoding AND active interference:** impedance mismatch causes reflection (energy bounces back) + cache miss causes stall (pipeline freezes). The blend predicts that wrong-schema activation is WORSE than no schema activation — it both fails to amplify and actively consumes WM slots for conflict resolution. This emergent constraint is not visible in either source alone.

</conceptual_blend>

<escape_path>

## Constraint-escape path

**Ranking criterion:** convergence score (number of independent domain chains activating the concept) × structural fit (how precisely the solution pattern addresses the named constraint).

| Mapping | Convergence score | Structural fit | Combined rank |
|---------|------------------|----------------|---------------|
| Information Theory (compression/noise) | 3 | High — directly maps slot-count limit to bandwidth | 1 |
| Cache/Memoization (space-time trade-off) | 3 | High — slot-count maps to register count; automaticity = memoization | 1 |
| Acoustics/Resonance (prior schema) | 3 | High — explains the prior-knowledge effect mechanistically | 2 |
| Fluid Dynamics (laminar/turbulent) | 3 | Medium-High — load ordering → phase transition | 3 |
| Linguistics/Parsing (constituent closure) | 3 | Medium — instructional sequencing insight | 4 |

**Tie at rank 1** (Information Theory + Cache/Memoization): both emitted per failure-guard rule.

---

### Primary escape path A — Information Theory / Compression

**Constraint escape:** The WM bottleneck is a fixed-bandwidth channel. The constraint is escaped not by widening the channel (impossible: WM training transfer is negligible) but by **reducing the bit-cost per unit of meaning** (chunking, schema activation) and **reducing channel noise** (suppressing extraneous cognitive load). The Shannon capacity limit is an absolute bound, but the *effective* information per slot is adjustable. Learning strategies that escape the constraint are those that increase the information density per chunk: analogies, schemas, worked examples with explicit structure, and progressive complexity scaffolding.

**Actionable path:** Instructional design should treat cognitive load as a communication engineering problem. Measure: intrinsic load (task complexity, irreducible) + extraneous load (noise, reducible by design) ≤ WM capacity C. All high-yield strategies (interleaving, retrieval practice, elaborative interrogation) are, in this framing, compression or error-correction algorithms.

---

### Primary escape path B — Cache Architecture / Memoization

**Constraint escape:** WM slots are CPU registers — the fewest, fastest tier. Automaticity (practice to fluency) is memoization: it moves a frequently-needed sub-operation from "compute in WM registers" to "retrieve compiled result," freeing registers for higher-order processing. The constraint is escaped not by gaining more registers but by **shrinking the working set** — the set of items that must occupy registers simultaneously.

**Actionable path:** Learning sequence design should explicitly build a memoization layer before combining skills. "Master sub-skills to automaticity before combining" is not pedagogically conservative; it is structurally necessary — it is the cache warming step. Attempting to combine before automaticity is thrashing: slots are consumed keeping sub-skills active, leaving none for the combination itself.

---

### Escape path with blend amplification

The conceptual blend (Resonant Cache) reveals that schema pre-construction before learning provides **both escape paths simultaneously** — it is the single intervention that functions as both compression algorithm and cache warm-up. This is the emergent solution: advance organizers and schema activation are not merely helpful priming — they are dual-function operations that simultaneously reduce per-chunk WM cost and prefetch expected content into the active buffer.

</escape_path>

</constraint_escape>
