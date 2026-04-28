---
stage_id: S2
stage_name: Constraint Escape
session_id: 20260414-prompt-cog-skill-design-spec
module_version: 1.1.0
status: complete
---

<constraint_escape>

## Named constraint

The **binding constraint** is:

> Agent isolation (spawning a dedicated agent per step) is the mechanism that prevents context contamination between analysis, ideation, and synthesis — and therefore is what produces quality output. But isolation is also the primary source of latency cost in epiphany-prompt. prompt-cog must achieve the *quality benefit* of isolation (clean synthesis context, no analysis-to-synthesis bleed) without paying the *per-step spawn cost* that isolation normally requires.

This is not a simple speed/quality trade-off; it is a structural dependency: the quality mechanism and the overhead mechanism are the same mechanism. Removing one removes the other. The escape must break that coupling.

*Failure guard note: two secondary constraints were identified but are not equally binding:*
- *"Not lower quality than prompt-epiphany baseline" — this is a floor constraint, not the primary coupling point.*
- *"Deterministic, reliable step execution" — this is a behavior requirement, not a structural dependency.*

*Both subordinate to the primary constraint above.*

---

## Domain candidates surveyed

**Domain-agnostic restatement of the constraint (Da Vinci):**

"A production system requires a clean, uncontaminated operational state to produce high-quality output. The standard mechanism guaranteeing that clean state is physical or logical separation (isolation) — but that isolation is invoked per-operation, making it the dominant cost center. The challenge: obtain isolation's output-quality benefit through structural design that substitutes for separation, without invoking separation itself per cycle."

**Candidate survey (25 domains, parallel evaluation):**

| # | Domain | Domain-Agnostic Occurrence | Structural Class Match | Viable? |
|---|--------|---------------------------|------------------------|---------|
| 1 | Evolutionary Biology | Sympatric speciation: niche differentiation achieves lineage separation without geographic isolation | optimization under constraint, niche specialization | Moderate |
| 2 | Thermodynamics | Maxwell demon: sorting without full entropy cost via information; phase transitions as discrete state shifts | irreversibility, hidden coupling | Partial |
| 3 | Fluid Dynamics | Stratified flow / free shear layers: laminar separation maintained by density gradient rather than physical wall | state-dependent behavior, path-dependence | **Viable** |
| 4 | Immunology | Thymic selection: self/non-self discrimination via learned tolerance, not physical compartmentation of self-proteins | classification under uncertainty, adaptive specificity | **Viable** |
| 5 | Information Theory | Multiplexing: independent logical channels over a shared physical medium; channel coding separates signals in frequency/time/code space | information bounds, encoding/decoding costs | **Viable** — direct |
| 6 | Architecture / Structural Engineering | Prestressed concrete: embedded tension compensates compression forces without separate structural layer | load redistribution | Partial |
| 7 | Ecology / Food Web | Niche partitioning: species coexist in shared habitat via resource differentiation, avoiding competitive exclusion | cascade effects, network resilience | Moderate |
| 8 | Game Theory | Mechanism design: incentive structures achieve coordination outcomes (as if agents were separated) without enforcing separation | multi-agent optimization, incentive alignment | **Viable** |
| 9 | Developmental Biology | Cell differentiation: identical-genome cells achieve functionally distinct identities via chemical gradients and gene expression — no new DNA, no physical separation | emergence from local rules, self-organization | **Viable** — strong |
| 10 | Control Theory | Observer design / Kalman filtering: separates signal from noise without physically isolating signal source; state estimation reconstructs clean state from contaminated observations | feedback stability, robustness vs. performance | **Viable** |
| 11 | Crystallography | Grain boundary engineering: material phases separated at micro-scale without macroscopic physical division | local structure → global property | Partial |
| 12 | Network Science | Community detection: logical partitions identified in shared graph; separation is virtual | connectivity leverage, critical thresholds | Moderate |
| 13 | Linguistics | Pragmatic disambiguation: same phoneme/syntax achieves different interpretations via context — functional separation without separate grammars | finite means, infinite output | Partial |
| 14 | Epidemiology | Behavioral quarantine: infection control via behavioral separation (masking, distancing) rather than physical room isolation | threshold phenomena, intervention timing | Partial |
| 15 | Acoustics / Wave Physics | Active noise cancellation: destructive interference achieves the *effect* of acoustic absence without physical separation from source | resonance leverage, destructive vs. constructive combination | **Viable** |
| 16 | Economics | Coase theorem: externality internalization via negotiation rather than regulatory separation of parties; Pigou taxes internalize without structural separation | information asymmetry resolution, externality internalization | **Viable** |
| 17 | Optics | Spatial filtering / band-pass filtering: selects desired frequency components from mixed-frequency beam without blocking undesired light at its source | resolution limits, beam shaping, interference exploitation | **Viable** — direct |
| 18 | Psychology / Cognitive Science | Choice architecture / attentional cueing: task-relevant cognitive focus achieved without removing competing stimuli; role-switching resets processing frame | bounded rationality, attention scarcity | **Viable** |
| 19 | Pharmacology | Receptor-specific binding: targeting selectivity achieved by molecular complementarity, not by delivering drug only to target tissue | specificity vs. selectivity, delivery constraints | **Viable** |
| 20 | Urban Planning | Traffic signal time-slicing: flow separation (green/red cycles) on shared road achieves isolation benefit without adding physical lane separation | emergent congestion, routing under shared resource | **Viable** |
| 21 | Cryptography | Zero-knowledge proofs: prover demonstrates knowledge without revealing it — the benefit of disclosure obtained without its cost; commitment schemes | asymmetric information leverage, trust bootstrapping | **Viable** — strong |
| 22 | Geology | Elastic rebound: stress accumulated across a shared crust, released cleanly at a threshold; isolation emerges at event boundary | slow accumulation / rapid release | Partial |
| 23 | Animal Behavior | Ritualized conflict: territory/mate allocation outcome achieved without full combat; sign stimuli trigger fixed-action patterns that substitute for costly full engagement | stimulus-triggered automation, distributed coordination | Partial |
| 24 | Computation / Complexity Theory | Memoization / dynamic programming: subproblem independence achieved without re-execution; separation of concerns in algorithm design via careful problem decomposition | exponential vs. polynomial limits, decomposition | **Viable** — direct |
| 25 | Chemistry / Catalysis | Transition state stabilization: reaction selectivity (routing to desired product) achieved without physically channeling reactants; catalyst creates specificity without isolation | barrier-lowering without consumption, cascade amplification, selectivity at transition states | **Viable** — strong |

**Viable candidates (≥5 confirmed, Step 8 fallback not triggered):** Domains 3, 4, 5, 8, 9, 10, 15, 16, 17, 18, 19, 20, 21, 24, 25 — **15 viable candidates identified.**

---

## Domain mappings (5)

Five structurally distinct mappings, selected for maximal structural divergence (no shared surface features verified).

---

### Mapping 1 — Information Theory: Multiplexing

**Source domain:** Information Theory / Channel Coding
**Target domain:** prompt-cog skill architecture
**Structural analog:** Logical channel separation over a shared physical medium (FDMA/CDMA/TDMA)

**What solves it in source domain:**
Multiple independent signals transmitted over a single wire/spectrum by assigning each a distinct frequency band, time slot, or orthogonal code. The receiver separates channels via a *demultiplexer* that operates on structural markers, not physical barriers. The wire is shared; the channels are logically clean.

**Transfer to target:**
The orchestrator context (shared medium) can carry multiple logically clean "channels" — analyst role, ideation role, synthesis preparation — if each is **marked structurally** (explicit role declaration headers, section delimiters, hard section boundaries). The synthesis spawn prompt is the demultiplexer: it selects only synthesis-relevant content from the shared medium by structural addressing (INVENTORY YAML, contracts, normalized input — each labeled). Context contamination is attenuated because the synthesis agent's receive filter is tightly specified.

**Key insight:** The contamination problem is a *receiver* problem, not a *source* problem. If the receiver (synthesis spawn prompt) specifies what to attend to with structural precision, the shared medium can carry contaminated content without quality loss at the output.

**Structural distinctness check:** Addresses the *communication channel* structure — separation-by-coding, not separation-by-process, space, frame, or chemistry.

---

### Mapping 2 — Developmental Biology: Differentiation via Local Context

**Source domain:** Developmental Biology / Morphogenesis
**Target domain:** prompt-cog role-switched step execution
**Structural analog:** Cell fate determination — identical genome, different phenotype via local chemical gradient

**What solves it in source domain:**
Every cell carries the full genome (full knowledge), but differentiation is achieved by suppressing the non-relevant genes via local signaling molecules (morphogens, transcription factors). The cell does not need a separate genome to become a neuron vs. a muscle cell — it uses the same starting material with a context-specific activation pattern. The "isolation" of function is achieved by *selective expression*, not by physically separating DNA.

**Transfer to target:**
The orchestrator model carries full capability (full knowledge). Role-switching (explicit role framing: "You are a structured prompt analyst…") acts as a morphogen — it activates the relevant behavioral/reasoning subset and suppresses others, without instantiating a new agent. Each step's role declaration is a chemical gradient that differentiates the same underlying model into a specialized functional state.

**Key insight:** Quality isolation is achievable as *selective expression from a shared substrate*, not as *physical duplication of the substrate per function*. The engineering question becomes: how precisely can the "morphogen" (role declaration) specify the activation pattern?

**Structural distinctness check:** Addresses the *identity differentiation* structure — same-substrate specialization via expression control. Does not share surface features with channel coding, optical filtering, catalysis, or frame-shifting.

---

### Mapping 3 — Optics: Spatial Filtering

**Source domain:** Optics / Photonics — band-pass spatial filtering
**Target domain:** synthesis spawn prompt construction
**Structural analog:** Spatial filter in a 4f optical system — places an aperture at the Fourier plane to transmit only desired spatial frequencies

**What solves it in source domain:**
A lens system collimates mixed-frequency light. At the Fourier plane, all frequencies are spatially separated (by wave physics, not by a physical source-split). A narrow aperture at that plane passes only the desired band. The original beam is never split; the filtering happens at a *conjugate domain* where the undesired components are naturally separated from the desired ones, making blocking cheap.

**Transfer to target:**
The orchestrator context window is the mixed beam — it contains all stages' outputs (analysis, ideation, contaminating dialogue). The synthesis spawn prompt is the aperture at the Fourier plane: it is constructed in a conjugate representation (structured YAML inventory, numbered contracts, normalized input with hard labels) where the desired components (synthesis-relevant content) are structurally distinct from the undesired components (role-framing residue, exploratory analysis notes). By writing the spawn prompt in this *conjugate form*, the synthesis agent receives a pre-filtered beam — not because source contamination was prevented, but because the spawn prompt construction maps the context into a domain where the components are already separated.

**Key insight:** Pre-filter at a conjugate representation, not at the source. The spawn prompt is a Fourier transform of the context into a domain where separation is cheap.

**Structural distinctness check:** Addresses the *representation space* structure — filtering via conjugate-domain construction. Distinct from channel coding (medium sharing), cell differentiation (expression suppression), catalysis (energy barrier), and clock gating (temporal separation).

---

### Mapping 4 — Chemistry: Catalysis and Transition State Specificity

**Source domain:** Chemistry / Catalysis — enzyme active site selectivity
**Target domain:** prompt-cog constraint conflict (isolation benefit without spawn cost)
**Structural analog:** Enzyme active site: achieves substrate specificity without physically channeling the substrate through a dedicated pathway

**What solves it in source domain:**
An enzyme operates in a solution where thousands of molecular species are present (shared medium, high "contamination"). Yet it achieves near-perfect selectivity by complementary geometry at the active site — it binds and transforms *only* the target substrate, ignoring all others. Specificity is not achieved by separating the target substrate into a different reaction vessel before catalysis; it is achieved by designing the active site to be complementary to the transition state of the desired reaction only.

**Transfer to target:**
The synthesis agent's spawn prompt is the active site. It can be designed to be "complementary" to the structural form of synthesis-relevant content (INVENTORY items have verbatim labels, contracts have `technique | target_section | action | rationale | priority` format, input is wrapped in an explicit delimiter). The synthesis agent's attention/action mechanism "binds" to these structurally-specific forms with high selectivity, ignoring the analytically-contaminating context that is present but structurally foreign to the active-site specification.

The catalytic analogy also imports: the active site design is reusable across many invocations (the enzyme is not consumed). The synthesis spawn prompt template is a reusable "active site" specification that enforces the same selectivity across all invocations.

**Key insight:** Selectivity (quality-under-contamination) is a function of the *specificity of the receptor design*, not the purity of the incoming stream. Design the active site for the target; contamination becomes irrelevant.

**Structural distinctness check:** Addresses the *binding specificity* structure — selectivity via complementary geometry. Distinct from frequency separation, expression suppression, Fourier-plane filtering, and clock gating.

---

### Mapping 5 — Urban Planning: Time-Sliced Flow Separation (Traffic Signal Logic)

**Source domain:** Urban Planning / Traffic Flow — traffic signal coordination
**Target domain:** orchestrator step sequencing in prompt-cog
**Structural analog:** Traffic signals achieve conflict-free intersection flow by *temporal separation* on shared space, not by building separate roads

**What solves it in source domain:**
At an intersection, flows from multiple directions conflict (they would contaminate each other if simultaneous). The standard solution is traffic signals: exclusive time slots for each direction, enforced by a controller with global timing knowledge. No additional road is built; the existing space is shared. Conflict is prevented by *sequencing with hard boundaries*, not by physical lane separation. The controller does not need to be in each car — it manages the intersection centrally with a small state machine.

**Transfer to target:**
The orchestrator model runs each step in a temporally exclusive "green light" slot — analyst role active during Step 3 (all other roles red), ideation role active during Step 4, pre-spawn checkpoint during Step 5. No parallel execution; strict temporal separation of role-states in the same context space. The contamination risk (prior-step output bleeding into current step) is mitigated by **hard section headers** (the stop/go boundary) and **forward-only flow** (no re-examination of prior steps except via structured references).

The prompt-cog architecture's "three-layer routing rule" (orchestrator reads synthesis return message only) maps directly to traffic signal logic: one lane has right-of-way at a time; the orchestrator does not re-enter the synthesis agent's context window.

**Key insight:** Temporal sequencing with hard state boundaries (stop/go) is a low-cost substitute for spatial separation. The key engineering variable is the *sharpness of the boundary signal*, not the depth of the separation.

**Structural distinctness check:** Addresses the *temporal scheduling* structure — isolation via time-slicing on shared resource. Fully distinct from frequency coding, expression suppression, Fourier-plane filtering, and molecular binding geometry.

---

## Activation provenance

Provenance assessment for candidate concepts that emerged during the domain survey:

| Concept | Activated from (chains) | Provenance class |
|---------|------------------------|------------------|
| "Logical separation over shared medium" | Chain A: Information Theory (multiplexing) → Chain B: Urban Planning (time-sliced lanes) → Chain C: Acoustics (constructive/destructive interference patterns share medium) | **Multi-source-convergent** (3 independent chains) |
| "Selective expression from shared substrate" | Chain A: Developmental Biology (gene expression) → Chain B: Pharmacology (receptor binding specificity) → Chain C: Immunology (thymic tolerance education) | **Multi-source-convergent** (3 independent chains) |
| "Pre-filter at conjugate representation" | Chain A: Optics (Fourier-plane spatial filter) → Chain B: Control Theory (Kalman state estimation in observation space) | **Multi-source-convergent** (2 independent chains) |
| "Selectivity via complementary geometry" | Chain A: Catalysis (active site) → Chain B: Pharmacology (drug-receptor binding) | **Multi-source-convergent** (2 independent chains) |
| "Hard boundary as low-cost isolation signal" | Chain A: Urban Planning (traffic signals) → Chain B: Geology (elastic rebound — threshold boundary) → Chain C: Thermodynamics (phase transition — discontinuous change at threshold) | **Multi-source-convergent** (3 independent chains) |
| "Role as activation pattern rather than identity" | Chain A: Developmental Biology (morphogen) → Chain B: Psychology/Cognitive Science (role framing, attentional cueing) | **Multi-source-convergent** (2 independent chains) |
| "Receiver-side filtering responsibility" | Chain A: Information Theory (demultiplexer) → Chain B: Optics (aperture at Fourier plane) | **Multi-source-convergent** (2 independent chains) |
| "Conjugate domain construction" | Chain A: Optics (4f system) only | **Single-source** — notable but not convergent |
| "Reusable active site specification" | Chain A: Catalysis only | **Single-source** — notable (catalytic reuse analogy) |

**Summary:** 7 of 9 candidate concepts are multi-source-convergent. The two single-source concepts (conjugate domain construction, reusable active site) appear as sub-features within Mapping 3 and Mapping 4 respectively and are structurally subordinate to multi-source-convergent parent concepts.

*Failure guard result: convergence confirmed; no Step 2 widening required.*

---

## Verification (Step 6)

**Systematicity check:**
- Mapping 1: channel coding / shared medium / demultiplexer
- Mapping 2: expression suppression / shared substrate / morphogen
- Mapping 3: Fourier-plane filtering / conjugate representation / aperture
- Mapping 4: active site complementarity / binding geometry / transition state
- Mapping 5: time-sliced flow / temporal sequencing / stop-go boundary

No two mappings share surface features (medium, biology, optics, chemistry, and traffic are all distinct surface domains). Systematicity: **PASS**.

**Provenance check:** All 9 candidate concepts have provenance declared (7 multi-source, 2 single-source). **PASS**.

**Blend-worthiness check:**
Mapping 2 (Developmental Biology) × Mapping 1 (Information Theory): blending "shared substrate with selective expression" with "shared medium with structural channel marking" produces an emergent question: *what if the role declarations are not just activation patterns but also channel-coding markers — so that the synthesis agent can reconstruct which step produced which content by structural address?* This emergent structure (role-as-channel-code-AND-expression-activator simultaneously) exists in neither source domain alone. **Blend-worthy: YES** — Step 7 fires.

---

<conceptual_blend>

## Input Space 1 — Information Theory (Multiplexing)

**Elements:** transmitter, receiver, shared medium, channel markers (frequency/time/code), demultiplexer, signal, noise/interference
**Structure:** Multiple logically distinct signals coexist on one physical medium. Each signal is encoded with a unique marker that allows the receiver to separate it from the others. Quality depends on the encoding clarity and receiver specificity, not on medium cleanliness. The transmitter and receiver are coupled by structural convention (the multiplexing scheme), not by physical separation.
**Relevance:** Maps to the problem of multiple processing steps sharing one context window, where each step's output must be recoverable and usable by later steps without corruption from other steps' outputs.

## Input Space 2 — Developmental Biology (Differentiation)

**Elements:** genome (shared), cell, morphogen (chemical gradient), transcription factor, gene expression, phenotype, differentiation state, tissue identity
**Structure:** One shared substrate (genome) produces radically different functional outputs (neuron, muscle, immune cell) via context-specific activation patterns (morphogens). No new DNA is created per cell type. Identity is a *state of expression*, not a *separate object*. The differentiation signal (morphogen) both specifies *what* to express and *suppresses* non-relevant expression simultaneously — it is a bidirectional gate.
**Relevance:** Maps to role-switching in prompt-cog, where the same model produces different functional outputs (analyst, ideation specialist, synthesis specialist) via role declarations that activate relevant capabilities and suppress irrelevant ones.

## Generic Space — Shared Skeleton

**Shared structure:**
- One substrate/medium carries multiple distinguishable functional states simultaneously
- A *marking/signaling mechanism* differentiates states without creating separate substrates
- A *receiver/interpreter* uses structural markers to reconstruct the intended state with high fidelity
- Quality depends on marker specificity, not on substrate purity

**Cross-space mapping:**
| Input 1 (Information Theory) | Input 2 (Developmental Biology) |
|---|---|
| Shared physical medium | Shared genome / shared context window |
| Channel marker (frequency/code) | Morphogen / role declaration |
| Demultiplexer (receiver filter) | Active site in synthesis prompt / downstream consumer |
| Signal (desired content) | Expressed genes / relevant step output |
| Noise/interference | Co-present signals from other channels / prior-step residue |
| Channel encoding scheme | Role declaration protocol |

## Blended Space — Role-Channel Architecture

**Name:** *Role-as-Channel-Code*

**Selective projection:**
- From Input 1 (Information Theory): the demultiplexer construct (receiver-side structural filter that reconstructs clean signal from mixed medium); the idea that encoding specificity determines quality, not medium cleanliness
- From Input 2 (Developmental Biology): the bidirectional gate function (morphogen both activates and suppresses simultaneously); the idea that identity is a state, not a separate object; context-locality of activation

**Operations applied:**
- *Composition:* Role declarations are not just activation signals (morphogen interpretation) — they are also structural channel codes (multiplexing interpretation). A role declaration header (e.g., `[ANALYST ROLE: ...]`) marks the content that follows as belonging to the "analyst channel" and simultaneously activates analyst-mode behavior. The synthesis spawn prompt's structural extraction (INVENTORY, contracts) is a demultiplexer that reads only the "synthesis-relevant channels."
- *Completion:* From Information Theory: the engineering implication is that the demultiplexer design is the critical quality determinant — so the synthesis spawn prompt construction is the highest-leverage design point in the architecture. From Developmental Biology: the implication is that role fidelity degrades when the "morphogen gradient" is diffuse — meaning role declarations must be proximal to the content they are supposed to govern, not distant.
- *Elaboration:* Running the blend: if each step in the orchestrator uses a structured header that marks its output as a named channel (`## ANALYST OUTPUT`, `## IDEATION OUTPUT`), and the synthesis spawn prompt is constructed by extracting from those channels by name, then: (a) context contamination is not a problem because all content is channel-coded and the synthesis agent's demultiplexer extracts only synthesis-relevant channels; (b) the same shared context window becomes a multiplexed wire rather than a contaminated pool; (c) the role declaration does double duty — activating the right behavior AND creating the addressable structure for later extraction.

## Emergent Structure — CRITICAL SECTION

**Emergent Feature 1:** `[EMERGENT-SOLUTION]`
Role declarations, if written as *structural output markers* (section headers labeling their output by role-channel name), simultaneously solve two problems with one mechanism: they activate the right behavioral mode (morphogen function) AND they create addressable structure for the synthesis spawn prompt constructor (multiplexing function). Neither input space alone suggests this dual function. Information Theory has demultiplexers but no behavioral activation. Developmental Biology has morphogen activation but no addressable structural labeling for downstream consumption.

**Emergent Feature 2:** `[EMERGENT-FRAME]`
The contamination problem reframes from a *purity problem* (how do we keep the context clean?) to an *addressability problem* (how do we make the desired content recoverable from a mixed signal?). This reframing is not available in either source domain alone: Information Theory says "encode to recover" but does not model contamination as a behavioral state issue; Developmental Biology models contamination as undifferentiation risk but does not suggest structural labeling as the solution.

**Emergent Feature 3:** `[EMERGENT-CONSTRAINT]`
A new constraint becomes visible in the blend: if role declarations function as channel codes, they must appear *before* the content they govern (not after, not interleaved), and the output sections they produce must be structurally terminated (so the demultiplexer knows where the channel ends). This "channel boundary discipline" is a constraint not derivable from either source alone — morphogens do not require termination markers; channel codes in FDMA are continuous signals, not segment headers.

</conceptual_blend>

---

## Constraint-escape path

**Ranking of mappings by convergence × structural fit:**

| Mapping | Convergence score (supporting independent chains) | Structural fit to binding constraint | Rank |
|---------|--------------------------------------------------|-------------------------------------|------|
| M1 — Multiplexing | 3 chains (Information Theory, Urban Planning, Acoustics) | Direct: addresses shared medium → logical separation | 2 |
| M2 — Differentiation | 3 chains (Developmental Biology, Pharmacology, Immunology) | Very strong: addresses same-substrate specialization | 1 (tie) |
| M3 — Spatial Filtering | 2 chains (Optics, Control Theory) | Very strong: addresses pre-filtering at conjugate representation | 1 (tie) |
| M4 — Catalysis | 2 chains (Catalysis, Pharmacology) | Strong: addresses selectivity via receiver geometry | 3 |
| M5 — Traffic Signals | 3 chains (Urban Planning, Geology, Thermodynamics) | Strong: addresses temporal separation as isolation substitute | 3 (tie) |

**Top path (tie between M1 and M2 resolved by blend analysis):**

The conceptual blend between M1 and M2 produced three emergent features, elevating the combined path above either individually. The **selected constraint-escape path is:**

> **Role-as-Channel-Code Architecture** — Implement role declarations as structural output markers that simultaneously activate role-specific behavior (morphogen function) and create addressable channel-labeled sections in the context window (multiplexing function). The synthesis spawn prompt constructor operates as a demultiplexer: it extracts from named channels by structural address, not by reading the full context. The constraint is escaped because context contamination becomes an addressability problem, not a purity problem — and addressability is solved by structural encoding, not by spawning a separate agent per step.

**Operational implications for prompt-cog SKILL.md:**

1. Each orchestrator step should write its output under a structurally-named header (e.g., `## ANALYST OUTPUT`, `## IDEATION OUTPUT`) — not mixed into dialogue.
2. The synthesis spawn prompt constructor should reference these headers explicitly when extracting content for the spawn prompt body.
3. Role declarations should appear as the *first line* of a step's processing block, before any output is written, and the output block should be explicitly closed (e.g., with a `---` delimiter) so the demultiplexer knows channel boundaries.
4. The Hard Gate 3 reminder and T4 binding rule serve as synthesis-channel-specific constraints — they belong in the demultiplexer specification (spawn prompt), not in the shared context.
5. The structural discipline of the INVENTORY YAML (verbatim labels, fixed schema) is already a channel-code implementation — this should be recognized as a first-class architectural feature, not just a preservation mechanism.

**Tie note (per failure guard):** M3 (Spatial Filtering) also ranks in the top tier. Its specific contribution (pre-filter at conjugate representation) is fully incorporated into the blend's emergent Feature 2 (reframing as addressability problem) and implication 2 (spawn prompt constructor as Fourier-plane extractor). M3's distinct contribution survives in the escape path via the "conjugate domain" framing of spawn prompt construction.

</constraint_escape>
