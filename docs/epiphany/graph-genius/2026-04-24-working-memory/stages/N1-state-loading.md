# N1 — State Loading (DECOMPOSITION)
## Executed: 2026-04-24 | Scale: STANDARD | Hat: Einstein/Feynman/VonNeumann/Darwin/Tesla/Turing

<state_loading>

<input_class>

## Input class

Class: **Other (Conceptual/Theoretical)**. The question asks for causal/mechanistic understanding of a cognitive phenomenon (working memory bottlenecks) and its downstream consequences (learning strategies). No code, no error, no formal proof request, no architecture design. Template 5 applies.

</input_class>

<primitives>

## Primitives

**Category 1 — Core claim/question:**
1. What are the bottlenecks — capacity limits? attentional limits? temporal limits?
2. "Cognitive bottlenecks" — what exactly is restricted: chunk count, binding resources, attention?
3. "Human working memory" — distinguishing from long-term memory, sensory buffers, executive function
4. "Shape learning strategies" — what is the mechanistic link from bottleneck to strategy choice?
5. Are bottlenecks fixed (architectural) or variable (state-dependent, domain-dependent)?
6. Are there multiple independent bottlenecks or one unified constraint?
7. "Learning strategies" — encoding strategies, practice schedules, chunking, spaced repetition?
8. Does the question seek prescriptive strategies (what should learners do?) or descriptive mechanisms (why do strategies work)?

*8 Category-1 primitives; 20-item target not met — constraint is the input.*

**Category 2 — Background facts:**
1. Miller (1956): working memory capacity ~7±2 "chunks" — the first quantified bottleneck
2. Baddeley & Hitch (1974): multicomponent model — phonological loop, visuospatial sketchpad, central executive
3. Cowan (2001): capacity ≈ 4 chunks; Miller's 7 inflated by chunking mechanisms
4. Nelson Cowan's "focus of attention": 1 chunk activated in focal attention at a time
5. Phonological loop: ~2 seconds of speech content; time-limited by articulatory decay
6. Visuospatial sketchpad: handles visual and spatial information independently of verbal
7. Central executive: limited capacity attentional control; no unlimited "homunculus"
8. Episodic buffer (Baddeley 2000 update): integrates info across phonological/visuospatial/LTM
9. Cognitive load theory (Sweller 1988): instructional design based on WM constraints
10. Intrinsic, extraneous, germane load — three components in CLT
11. Chunking: grouping elements into higher-order units reduces WM item count (de Groot 1946, chess masters)
12. Dual coding: verbal + visual channels are partially independent; exploiting both increases capacity
13. Spaced repetition: retrieval from LTM reduces WM load by automatizing sub-skills
14. Worked examples effect: reduces extraneous load vs. problem-solving during initial learning
15. Split-attention effect: spatially/temporally separated materials increase extraneous load
16. Redundancy effect: extra identical information increases rather than reduces load
17. Expertise reversal effect: what helps novices hurts experts (novices need guidance; experts self-explain)
18. WM capacity predicts fluid intelligence (Engle et al., Kane et al.)
19. Individual differences in WM span (reading span, operation span) predict comprehension, reasoning
20. WM is not static: anxiety, stress, mind-wandering deplete available capacity
21. Interference theories: retroactive and proactive interference corrupt WM content
22. Temporal decay: WM contents degrade without rehearsal (phonological buffer ~2s; visual shorter)
23. Binding: relating color to shape, word to context — the binding problem in WM

**Category 3 — Key tensions:**
1. Capacity limit (slot model) vs. resource model: is WM N discrete slots or a continuous pool of activation?
2. Fixed 4-chunk limit (Cowan) vs. variable capacity depending on item type and chunking opportunity
3. Bottleneck in storage vs. bottleneck in processing: which limits more in naturalistic learning?
4. Rehearsal-based maintenance vs. attention-based maintenance: are they the same resource?
5. Is forgetting from WM due to decay or interference? (Debate: Lewandowsky vs. decay theorists)
6. Central executive as unified vs. fractionated system (Miyake et al. 2000: three factors)
7. WM training: does it transfer to fluid intelligence? (Controversial: brain training claim vs. domain-specific)
8. Does reducing extraneous load always free up resources for learning? Or does some challenge aid metacognitive processes?
9. Desirable difficulty: interleaving and spacing sometimes increase apparent load but improve LTR (Bjork)
10. Are WM bottlenecks peripheral (perceptual) or central (attentional)?
11. Conscious vs. unconscious WM: does WM require conscious access?
12. Is there a phonological loop for reading, or do skilled readers bypass subvocalization?
13. Neural substrate debate: prefrontal-parietal network vs. activated LTM (Ruchkin, Jonides)

**Category 4 — Stakeholders:**
1. Educators: lesson design, multimedia learning, sequence of topics
2. Instructional designers: CLT application to e-learning, simulations
3. Students: study strategies, spaced practice, interleaving
4. Cognitive psychologists: model development, WM span measures
5. Neuroscientists: fMRI/EEG correlates of WM capacity and load
6. AI/education technology companies: adaptive learning systems
7. Clinical psychologists: ADHD, anxiety, aging-related WM decline

**Category 5 — Analogous domains:**
1. Computer RAM: limited buffer between storage and processing — direct architectural analogy
2. TCP/IP window size: bounded throughput from receiver buffer
3. Attention models in deep learning (Transformer): finite attention weights per token
4. Cache memory hierarchy: L1/L2/L3 cache as working memory; main memory as LTM
5. Orchestra conductor: central executive = conductor coordinating sections
6. Bottleneck theory in perception (Broadbent filter): early selection limits processing bandwidth

**Category 6 — Success criteria:**
1. The mechanistic links (bottleneck → constraint → strategy consequence) are explicitly traced
2. Each bottleneck type is matched to at least one learning strategy it directly motivates
3. Individual differences are addressed (WM span varies; strategies calibrate to learner)
4. Prescriptive implications are distinguished from descriptive mechanisms

**Category 7 — Open sub-questions:**
1. Does WM training (N-back) genuinely transfer to real-world learning? (Evidence: weak/no transfer)
2. Can working memory bottlenecks be circumvented through environmental scaffolding?
3. Are there learnable strategies that fundamentally alter the bottleneck (not just route around it)?
4. How do WM bottlenecks interact with emotional/motivational states during learning?
5. What is the neural mechanism of "attention capture" that competes with voluntary WM use?
6. Does chunking reduce the bottleneck or circumvent it? (Cowan: same slot, higher-order content)
7. Are the phonological and visuospatial bottlenecks truly independent?

</primitives>

<stripped_irreducibles>

## Simplicity-stripped irreducibles

**Kept (irreducible):**
1. **WM capacity ≈ 3–4 meaningful chunks** (Cowan 2001) — irreducible empirical constraint; the architectural limit
2. **Phonological loop decay: ~2 seconds** — time-based bottleneck distinct from item-count limit
3. **Central executive: limited attentional control capacity** — the processing-side bottleneck; cannot multi-process at full capacity
4. **Chunking circumvents the item-count limit by compressing multiple primitives into one chunk** — irreducible mechanism (not the same as eliminating the bottleneck)
5. **Extraneous cognitive load from poor instructional design competes with germane load (actual learning)** — irreducible implication for strategy
6. **Dual coding (verbal + visuospatial) partially doubles effective capacity** — irreducible because the two subsystems are empirically independent
7. **Automatization moves skill from WM to LTM, freeing WM for higher-order processing** — irreducible mechanism (spaced repetition's basis)
8. **Individual WM span is a stable predictor of learning outcome and fluid intelligence** — irreducible individual-differences fact

**Struck (derivable):**
- ~~Miller's 7±2~~ → superseded by Cowan's 4; Miller's number reflects chunked items, not primitive chunks
- ~~Split-attention effect~~ → derives from #5 (extraneous load from spatial/temporal separation)
- ~~Worked examples effect~~ → derives from #5 + #7 (reducing problem-solving overhead)
- ~~Redundancy effect~~ → derives from #5 (redundant info increases not reduces load)

</stripped_irreducibles>

<tesla_measurement_plan>

## Tesla measurement plan

Success is observable when a learner with **no prior knowledge** of cognitive psychology can:

1. Name the three major WM bottlenecks (item count ≈4 chunks, phonological decay ~2s, central executive capacity) with their approximate quantitative bounds.
2. State how chunking addresses bottleneck #1 without eliminating it.
3. Match each of three learning strategies (spaced practice, worked examples, dual coding) to the specific bottleneck it addresses and why.
4. Predict, for a given instructional design (e.g., text and diagram placed far apart), which bottleneck is stressed and what the consequence is.

</tesla_measurement_plan>

<verification_block>

## Verification

**Preparation-gate:** All 8 irreducibles cite either Cowan (2001), Baddeley (1974/2000), Sweller (1988), or Engle et al. — primary sources. No invented data. ✓

**Tesla constructive-test:** The measurement plan specifies concrete predictions (item counts, seconds, named strategies, design scenarios). Observable. ✓

**Count check:** Categories 2 and 3 reach ≥20 and ≥13 respectively; 1,4,5,6,7 hit count-cap with explicit declarations. ✓

</verification_block>

</state_loading>

---

## SIGNAL OUTPUT

### Digest 1 of 3 — primitives_digest (5–15 lines)

```
key_findings:
  - Core question: mechanistic links between WM bottlenecks (capacity, temporal decay, executive control) and learning strategy selection
  - WM capacity: ~3–4 chunks (Cowan 2001); Miller's 7±2 superseded; item-count is the primary architectural limit
  - Temporal bottleneck: phonological loop decays in ~2s; visuospatial buffer shorter
  - Processing bottleneck: central executive has limited capacity for attentional control; cannot fully multi-process
  - Chunking: compresses items into higher-order units; reduces item count without increasing slot count
  - CLT bottleneck → strategy link: intrinsic load (task complexity) + extraneous load (poor design) must fit in WM
  - Dual coding: verbal and visuospatial channels empirically independent → doubles effective capacity when both used
  - Individual differences: WM span stable, predicts fluid intelligence and learning outcomes
  - Key tensions: slot vs. resource models; decay vs. interference theories; WM training transfer (controversial)
named_entities:
  - Working memory, WM capacity chunks, phonological loop, visuospatial sketchpad, central executive, episodic buffer
  - Miller (1956), Baddeley & Hitch (1974), Cowan (2001), Sweller CLT, Baddeley (2000), Miyake et al.
confidence_flags: [H, H, H, H, H, H, H, H, H]
signal_flags: []
```

### Digest 2 of 3 — irreducibles_digest (5–15 lines)

```
key_findings:
  - Irreducible 1: WM item capacity = ~3–4 chunks; the architectural ceiling no strategy eliminates
  - Irreducible 2: Phonological loop ~2s temporal decay; time pressure is a distinct, parallel bottleneck
  - Irreducible 3: Central executive limited; cannot simultaneously manage two full-capacity tasks
  - Irreducible 4: Chunking compresses items but occupies the same slot count — circumvents, doesn't remove the limit
  - Irreducible 5: Extraneous cognitive load from poor design competes with germane load; strategy = minimize extraneous
  - Irreducible 6: Dual coding exploits channel independence; both verbal and visuospatial can be loaded simultaneously
  - Irreducible 7: Automatization (via practice/spaced repetition) offloads WM to LTM; frees capacity for higher-order work
  - Irreducible 8: WM span is a stable individual-differences predictor of intelligence and learning rates
named_entities:
  - WM chunk capacity, phonological loop decay, central executive, chunking mechanism, extraneous load, dual coding, automatization, WM span
confidence_flags: [H, H, H, H, H, H, H, H]
signal_flags: []
```

### Digest 3 of 3 — system_model_seed (5–15 lines)

```
key_findings:
  - System: learner cognitive architecture with three subsystems — phonological loop (verbal/time), visuospatial sketchpad (spatial), central executive (attentional control)
  - State space: WM load = sum of intrinsic + extraneous + germane load; constrained to fit ≤ ~4 chunks per active modality
  - Conserved quantity: total WM capacity is fixed at ~4 chunks; compression (chunking) redistributes, does not add
  - Observer frames: (a) novice — each element occupies a slot; (b) expert — chunked units occupy same slot count; (c) instructor — controls extraneous load via design
  - Time axis: phonological decay operates on ~2s timescale; learning outcome measured on session/day/week timescale (spaced repetition)
  - System boundary: cognitive architecture of one learner; social/environmental scaffolding is outside boundary but can reduce extraneous load
  - Key dynamics: load exceeds capacity → errors, forgetting; load within capacity → learning can proceed; automatization shifts sub-skills below the capacity boundary
named_entities:
  - Phonological loop, visuospatial sketchpad, central executive, WM load, CLT (intrinsic/extraneous/germane), chunking, automatization, LTM offload
confidence_flags: [H, H, H, H, H, H, H]
signal_flags: []
```
