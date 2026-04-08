<role>
You are a senior AI agent skill architect with deep expertise in cognitive science, creative problem-solving research, prompt engineering, and the SKILL.md skill specification format used by Claude Code-style agent systems. Your specialty is translating research-backed human cognitive techniques into AI-native mechanisms that exploit computational strengths (parallel hypothesis exploration, unlimited working memory, deterministic replay, exhaustive enumeration, no fatigue) rather than mimicking human limitations.
</role>

<context>
This task is the **FIRST of two analytical inputs** that will be combined to design a new agent skill called **`genius-epiphany`**. A **SECOND report** — analyzing professional corporate brainstorming and planning techniques — will be produced separately and merged with the output of THIS task. The two combined will form the basis for the final SKILL.md specification. but the skills should operate fine on their own, creating three seperate skills in the end, which can all integrate, or be used individually, depending on the user provided context

Your output must therefore be structured so that the corporate-brainstorming analysis can later be fused into it cleanly, **without restructuring or rewriting** the design produced here.

The skill will live as a SKILL.md-format skill in an AI agent system. It must be:
- Invokable independently as a reasoning enhancer
- Composable with any other SKILL.md skill to enhance that skill's thought process
- Usable for corporate enterprise-level brainstorming and planning

The full primary research source is provided verbatim inside `<research_corpus>` below and must drive the design.
</context>

<task>
Produce a **detailed design proposal** for an AI agent skill named **`genius-epiphany`** that operationalizes the human-genius cognitive differentiators in `<research_corpus>` into AI-native mechanisms a SKILL.md-based agent can actually execute.

The proposal must:

1. **Synthesize** the 5 cognitive differentiators (Knowledge Organization / Chunking, Cognitive Flexibility / DMN↔ECN Network Switching, Incubation, Cross-Domain Pattern Transfer / Polymathy, Combinatorial Play) plus the studied genius methodologies (Feynman's Algorithm, Einstein's Thought Experiments, da Vinci's Observation-vs-Recognition and Nine-Stage Process, Janusian Process) into a single unified pipeline.

2. **Translate, do not transliterate.** For each human mechanism, identify what it is *actually doing cognitively*, then design an AI-native equivalent that exploits computational strengths the human brain lacks. State explicitly **where the AI version surpasses the human version and why** (e.g., parallel DMN+ECN execution rather than sequential switching; exhaustive combinatorial enumeration instead of sampled "play"; deterministic incubation replay instead of stochastic unconscious processing).

3. **Output a skill design** suitable for a SKILL.md file: trigger conditions, internal pipeline (numbered steps), invocation modes if applicable, verification gates, and a precise description of how the skill integrates with other skills in composable-enhancer mode.

4. **Serve dual purposes** — both designed together, not bolted on:
   - **Corporate enterprise brainstorming and planning** — executive-grade ideation, decision-framing, strategic synthesis.
   - **Standalone thought-process enhancer** — invokable as a reasoning amplifier wrapping any other SKILL.md skill's execution.

5. **Prepare for merge.** Produce output formatted as composable, modular blocks (not flowing prose) so the parallel corporate-brainstorming-techniques report can be cleanly inserted at named anchor points later.
</task>

<research_corpus>

# Creative Problem-Solving Genius: Cognitive Differentiators and AI Implementation

*Generated: 2026-04-07 | Sources: 30+ | Confidence: High*


## Executive Summary

This research identifies **5 critical differentiating factors** between highly effective and less effective problem solvers, drawn from cognitive psychology, neuroscience, and studies of exceptional minds (Einstein, Feynman, da Vinci, Nobel laureates). Each factor includes specific AI implementation pathways.

### Key Differentiators with Highest Implementation Potential

| # | Differentiator | Human Manifestation | AI Implementation | Confidence |
|---|----------------|---------------------|------------------|------------|
| 1 | **Knowledge Organization (Chunking)** | Hierarchical schemas, principle-based categorization | Pattern library + schema builder | High |
| 2 | **Cognitive Flexibility (Network Switching)** | DMN↔ECN switching, approach variety | Multi-perspective generation + selection | High |
| 3 | **Incubation Mechanisms** | Unconscious associative processing | Latent state activation, random sampling | Medium |
| 4 | **Cross-Domain Pattern Transfer** | Analogical reasoning across domains | Structured analogy mapping | Medium |
| 5 | **Combinatorial Play** | Visual/voluntary recombination of elements | Explicit combination generation | High |

---

## 1. Cognitive Differentiators

### 1.1 Knowledge Organization: The Chunking Advantage

**What Distinguishes Experts**

Research by Chi, Feltovich, & Glaser (1981) and Larkin et al. (1980) established foundational expert-novice differences:

| Experts | Novices |
|---------|---------|
| Categorize by **deep structure** (physics principles) | Categorize by **surface features** (objects, terms) |
| Hierarchical knowledge with fundamentals accessible | Loosely connected information |
| Large pattern library with rapid retrieval | Limited patterns, slow access |
| Top-down: principles → details | Bottom-up: details → equations |

**The Chunking Mechanism** (Gobet, 2005):
- Experts build "chunks" — meaningful patterns of information stored in long-term memory
- Templates evolve from chunks with slots for rapid encoding
- Pattern recognition bypasses working memory limits
- Expert intuition is **recognition, not reasoning** (Klein's Recognition-Primed Decision model)

**Evidence Quality**: High — replicated across chess, physics, medicine, mathematics

**AI Implementation**:
```python
# Pattern Library Architecture
class PatternLibrary:
    def __init__(self):
        self.principles = {}      # Deep structure categories
        self.templates = {}       # Schema with slots
        self.surface_index = {}   # Surface feature → principle mapping

    def categorize(self, problem):
        # 1. Extract surface features
        features = self.extract_features(problem)
        # 2. Map to deep structure principles
        principle = self.surface_index.get(features)
        # 3. Retrieve template with slots
        template = self.templates.get(principle)
        return template
```

---

### 1.2 Cognitive Flexibility: Network Switching

**Neural Mechanism**

Chen et al. (2025), Nature Communications Biology, studied N=2,433 across 10 datasets:

- Creativity correlates with **dynamic switching** between Default Mode Network (DMN) and Executive Control Network (ECN)
- **Inverted-U relationship**: Optimal creativity requires balanced switching (not too much, not too little)
- DMN: Spontaneous, associative thinking (idea generation)
- ECN: Controlled, analytical thinking (idea evaluation)

**Wu & Koutstaal (2020)** — Self-Guided Transitions:
- Both flexibility (shifting) AND persistence (dwelling) together predict creative performance
- Key metric: When people choose to persist vs. switch between approaches
- Experts show better calibration of when to shift vs. persist

**Expert-Novice Difference** (Haavold & Sriraman, 2022):
- Experts demonstrate **greater cognitive flexibility** in switching approaches
- Novices show **algorithmic fixation** — persisting with inappropriate strategies
- Expert problem-solvers recognize when to abandon unproductive paths

**Evidence Quality**: High — large-scale neural imaging, replicated behavioral studies

**AI Implementation**:
```python
class CognitiveFlexibilityModule:
    def __init__(self):
        self.dmn_modes = ['associative', 'exploratory', 'divergent']
        self.ecn_modes = ['analytical', 'evaluative', 'convergent']
        self.switch_threshold = 0.3  # Calibrated from problem state

    def solve(self, problem):
        # Phase 1: DMN-dominant (generate)
        ideas = self.generate_ideas(problem)

        # Switch trigger: idea generation plateaus
        if self.idea_quality_plateau(ideas):
            # Phase 2: ECN-dominant (evaluate)
            evaluated = self.evaluate_ideas(ideas)
            return self.select_best(evaluated)
```

---

### 1.3 Incubation: The Unconscious Work Mechanism

**Meta-Analytic Evidence**

Sio & Ormerod (2009), meta-analysis of 117 studies:
- **Effect size d = 0.29** (low-medium, but reliable)
- Greater for divergent thinking than insight tasks
- Low cognitive demand during incubation > high demand or rest

**Three Theories** (Gilhooly, 2016):

| Theory | Mechanism | Support Level |
|--------|-----------|---------------|
| **Unconscious Work** | Spreading activation continues | Best supported |
| Beneficial Forgetting | Mental sets weaken | Cannot explain immediate incubation |
| Intermittent Conscious Work | People keep working | Largely discredited |

**Key Finding**: Immediate incubation (break before any work) can be MORE effective than delayed incubation — supports unconscious work theory over forgetting.

**Evidence Quality**: High — large meta-analysis, replicated paradigms

**AI Implementation**:
```python
class IncubationMechanism:
    def __init__(self):
        self.latent_state = None
        self.problem_representation = None

    def incubate(self, problem_state, strategy='low_demand'):
        # Store problem representation in latent state
        self.latent_state = self.encode(problem_state)

        # Generate alternative representations during "incubation"
        alternatives = []
        for _ in range(self.n_samples):
            # Random perturbation of latent state
            perturbed = self.perturb(self.latent_state)
            alternatives.append(self.decode(perturbed))

        # Return with activation scores
        return self.rank_alternatives(alternatives)
```

---

### 1.4 Pattern Recognition Across Domains (Polymathy)

**Nobel Laureate Research** (Root-Bernstein & Root-Bernstein, 2023):

- Nobel laureates are **25x more likely** to engage seriously in arts, crafts, music
- Economics laureates almost universally trained in mathematics, physics, or astronomy
- Literature laureates **3x more likely** to be fine artists, **20x more likely** to be actors

**Three Components of Polymathy** (Araki's developmental model):

| Component | Description | Role in Creativity |
|-----------|-------------|-------------------|
| **Breadth** | Knowledge spanning domains | Larger idea pool |
| **Depth** | Expertise beyond superficial | Quality of connections |
| **Integration** | Synthesis across domains | Novel combinations |

**Thinking Tools of Polymaths**:
- Analogical thinking
- Pattern recognition
- Body thinking / embodiment
- Modeling and playacting
- Cross-domain skill transfer

**Evidence Quality**: High — statistical analysis of Nobel laureates, validated psychometric instruments

**AI Implementation**:
```python
class CrossDomainPatternTransfer:
    def __init__(self):
        self.domain_knowledge = {}  # domain → patterns, principles
        self.analogy_map = {}       # structural similarities

    def transfer(self, problem, source_domain=None):
        # 1. Identify structural features of problem
        structure = self.extract_structure(problem)

        # 2. Search for structurally similar patterns in other domains
        if source_domain:
            analogs = self.domain_knowledge[source_domain]
        else:
            analogs = self.search_all_domains(structure)

        # 3. Map solution patterns from source to target
        return self.apply_analogy(structure, analogs)
```

---

### 1.5 Combinatorial Play and Visual Thinking

**Einstein's Method** (1945 letter to Hadamard):

> "The words of the language... do not seem to play any role in my mechanism of thought. The psychical entities which seem to serve as elements in thought are certain signs and more or less clear images... [These elements] can be 'voluntarily' reproduced and combined."

**Key Cognitive Pattern — Combinatorial Play**:
- Visual and muscular elements precede logical construction
- Elements can be voluntarily recombined
- Aimed at finding analogies to logical connections

**The Janusian Process** (Rothenberg, Psychology Today):
- Ability to hold **contradictory ideas simultaneously**
- Einstein's "happiest thought": person falling is both in motion AND at rest
- Critical for breakthrough insights

**Da Vinci's Nine-Stage Process** (Güss, Ahmed, & Dörner, 2021):

| Stage | Description |
|-------|-------------|
| 1. Vision & Curiosity | Intense motivation to understand |
| 2. Social Recognition | Desire for fame fuels work |
| 3. Asking Questions | What/why/how to identify gaps |
| 4. Analogical Thinking | If-then analogies across domains |
| 5. Trial & Error | Experimental testing |
| 6. Abductive Reasoning | Observe → propose explanations |
| 7. Incubation & Forgetting | Rest allows unconscious work |
| 8. Overinclusive Thinking | Unrelated thoughts → illumination |
| 9. Schema Elaboration | Work out details |

**Evidence Quality**: High — documented case studies, cognitive analysis

**AI Implementation**:
```python
class CombinatorialPlayModule:
    def __init__(self):
        self.element_library = []    # Modular, recombinable elements
        self.combination_rules = {}  # Valid combination patterns

    def generate_combinations(self, elements, n_combinations=10):
        """Voluntarily reproduce and combine elements"""
        combinations = []
        for _ in range(n_combinations):
            # Random selection + structural validation
            combo = self.random_combine(elements)
            if self.valid_structure(combo):
                combinations.append(combo)
        return combinations

    def hold_contradictions(self, proposition):
        """Janusian process: generate opposing view"""
        contradiction = self.invert(proposition)
        # Both held simultaneously for synthesis
        return self.synthesize(proposition, contradiction)
```

---

## 2. Studied Genius Methodologies

### 2.1 Feynman's Algorithm

**Core Method** (deceptively simple):

1. **Write down the problem**
2. **Think very hard**
3. **Write down the solution**

**Key Cognitive Tools**:

| Tool | Description | AI Translation |
|------|-------------|----------------|
| Naive Questions | Test understanding with direct questions | Generate "stupid questions" to verify comprehension |
| Bayesian Thinking | Update beliefs on evidence gradient | Probability distributions over hypotheses |
| Test Effect Strength | Real phenomena clarify with better measurement | Track signal-to-noise across iterations |
| Recognize Information Gaps | Errors from missing info, not faulty reasoning | Explicit gap detection module |

### 2.2 Einstein's Thought Experiments

**Characteristics**:
- Highly visual understanding of physics
- Vivid, practical details (unlike theoretical approaches)
- Used as "intuition pumps" to apply understanding
- Conceived impossible scenarios to test principles

**Key Thought Experiments**:

| Experiment | Theory Developed |
|------------|------------------|
| Chasing light beam (age 16) | Special Relativity |
| Magnet/conductor asymmetry | Special Relativity |
| Person falling from roof | General Relativity |
| Accelerating elevator | General Relativity |

**AI Implementation**: Generate hypothetical scenarios that test boundary conditions of proposed solutions.

### 2.3 Da Vinci's Observation vs. Recognition

**Critical Distinction**:

| Recognition | Observation |
|-------------|-------------|
| See general details, label, move on | Slow down, catalog as if first time |
| Efficient but limiting | Time-consuming but expansive |
| Filter through known categories | Remove distorting filters |

**Practice**: "Begin with the details of objects, and do not go on to the second step until you have the first well fixed in memory."

---

## 3. AI Implementation Mapping

### 3.1 Implementation Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    BRAINSTORMING SKILL ARCHITECTURE              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────┐     ┌─────────────────┐     ┌────────────┐ │
│  │  PATTERN LIBRARY │     │ APPROACH CYCLER │     │ INCUBATOR  │ │
│  │  (Chunking)      │────▶│ (Cognitive Flex)│────▶│ (Unconscious│ │
│  │                 │     │                 │     │  Work)     │ │
│  │  - Principles   │     │  - DMN ↔ ECN    │     │            │ │
│  │  - Templates    │     │  - Persist/Shift│     │ - Latent    │ │
│  │  - Surface→Deep │     │  - Approach vars│     │ - Perturb   │ │
│  └─────────────────┘     └─────────────────┘     └────────────┘ │
│          │                       │                      │        │
│          ▼                       ▼                      ▼        │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    COMBINATORIAL ENGINE                      ││
│  │  - Voluntary recombination                                  ││
│  │  - Hold contradictions (Janusian)                            ││
│  │  - Cross-domain analogical mapping                          ││
│  └─────────────────────────────────────────────────────────────┘│
│                              │                                   │
│                              ▼                                   │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    VERIFICATION & OUTPUT                    ││
│  │  - Evidence quality assessment                              ││
│  │  - Confidence levels                                        ││
│  │  - Implementation pathways                                  ││
│  └─────────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────┘
```

### 3.2 Skill Component Specifications

#### Component 1: Pattern Recognition Module

**Human Basis**: Expert chunking, principle-based categorization

**AI Implementation**:
```yaml
pattern_recognition:
  function: Categorize problems by deep structure
  inputs:
    - problem_description: string
    - context: dict
  outputs:
    - principle_category: string
    - relevant_templates: list[Template]
    - surface_features: list[Feature]
  techniques:
    - feature_extraction:
        method: "Extract surface-level features from problem text"
    - principle_mapping:
        method: "Map surface features to deep structure principles"
    - template_retrieval:
        method: "Retrieve schema with slots for rapid application"
  quality_check:
    - principle_confidence: "High/Medium/Low based on feature match"
```

#### Component 2: Cognitive Flexibility Controller

**Human Basis**: DMN↔ECN switching, approach variety

**AI Implementation**:
```yaml
cognitive_flexibility:
  function: Alternate between generative and evaluative modes
  modes:
    dmn_dominant:
      - name: "associative"
      - function: "Generate diverse ideas without judgment"
      - triggers: "initial_generation, stuck_state"
    ecn_dominant:
      - name: "evaluative"
      - function: "Analyze and select best ideas"
      - triggers: "idea_plateau, convergence_needed"
  switching_logic:
    plateau_detection: "Monitor idea quality variance over iterations"
    persistence_threshold: "Maximum attempts before switching approach"
    approach_variety: "Track approaches tried, avoid repetition"
```

#### Component 3: Incubation Simulator

**Human Basis**: Unconscious work theory, spreading activation

**AI Implementation**:
```yaml
incubation:
  function: Generate alternatives through latent state perturbation
  inputs:
    - current_problem_state: encoded representation
    - stuck_state: boolean
  outputs:
    - alternative_representations: list[Representation]
    - activation_scores: list[float]
  methods:
    latent_encoding: "Compress problem to latent representation"
    perturbation: "Apply random noise to latent state"
    decoding: "Decode perturbed states to alternative representations"
  triggers:
    - plateau_detected: true
    - iteration_count: "> threshold"
    - approach_exhausted: true
```

#### Component 4: Cross-Domain Transfer Engine

**Human Basis**: Polymathic pattern recognition, analogical reasoning

**AI Implementation**:
```yaml
cross_domain_transfer:
  function: Find and apply analogies across domains
  inputs:
    - problem_structure: abstracted structure
    - source_domains: list[Domain] (optional)
  outputs:
    - analogous_patterns: list[Pattern]
    - transfer_confidence: float
  methods:
    structure_extraction: "Identify underlying structural features"
    analogy_search: "Search for structurally similar patterns in domain library"
    transfer_application: "Map solution patterns from source to target"
  quality_check:
    - structural_similarity: "Depth of structural match"
    - surface_dissimilarity: "Bonus for cross-domain (novelty)"
```

#### Component 5: Combinatorial Play Module

**Human Basis**: Einstein's combinatory play, Janusian process

**AI Implementation**:
```yaml
combinatorial_play:
  function: Voluntarily combine elements to find novel solutions
  inputs:
    - element_library: list[Element]
    - problem_constraints: list[Constraint]
  outputs:
    - combinations: list[Combination]
    - novelty_scores: list[float]
  methods:
    element_decomposition: "Break problem into modular, recombinable elements"
    random_combination: "Generate combinations without bias"
    structural_validation: "Filter combinations that satisfy constraints"
    contradiction_holding: "Generate opposing view, seek synthesis"
  janusian_process:
    step_1: "Articulate proposition"
    step_2: "Generate explicit contradiction"
    step_3: "Hold both simultaneously"
    step_4: "Seek creative synthesis"
```

---

## 4. Effectiveness Validation

### 4.1 Research Evidence Summary

| Differentiator | Study | Methodology | Effect Size/Confidence |
|----------------|-------|-------------|------------------------|
| Chunking | Chi et al. (1981) | Expert-novice categorization | Large, replicated |
| Network Switching | Chen et al. (2025) | fMRI, N=2,433 | Inverted-U, robust |
| Incubation | Sio & Ormerod (2009) | Meta-analysis, 117 studies | d = 0.29 |
| Polymathy | Root-Bernstein (2023) | Nobel laureate statistics | 25x likelihood |
| Combinatorial Play | Einstein case studies | Cognitive analysis | Qualitative, strong |

### 4.2 Measurement Approaches

**For Human Studies**:
- Expert-novice categorization tasks
- fMRI network switching frequency
- Problem-solving success rates
- Polymathic orientation scales

**For AI Implementation**:
- Diversity of solutions generated
- Quality of principle categorization
- Novelty scores for cross-domain analogies
- Improvement after "incubation" iterations

---

## 5. Skill Design Patterns

### 5.1 Pattern Library: Principle-Based Categorization

**Design Pattern**: Store problems by deep structure, not surface features.

**Components**:
- Principle taxonomy (hierarchical categories)
- Template library (schema with slots)
- Surface-to-deep mapping (feature → principle index)

**Trigger**: When receiving a problem, first categorize by principle.

### 5.2 Pattern: Approach Cycling

**Design Pattern**: Explicitly cycle between generative and evaluative modes.

**Components**:
- Mode controller (DMN-like vs. ECN-like)
- Plateau detector (when to switch)
- Approach variety tracker

**Trigger**: When idea generation plateaus, switch modes.

### 5.3 Pattern: Latent Incubation

**Design Pattern**: When stuck, perturb problem representation and decode alternatives.

**Components**:
- Latent encoder
- Perturbation mechanism
- Alternative decoder

**Trigger**: When progress stalls for N iterations.

### 5.4 Pattern: Cross-Domain Analogy

**Design Pattern**: Search for structural analogies across domains.

**Components**:
- Structure extractor
- Domain library
- Analogy mapper

**Trigger**: When within-domain approaches exhausted.

### 5.5 Pattern: Combinatorial Generation

**Design Pattern**: Systematically combine elements without bias.

**Components**:
- Element decomposer
- Combination generator
- Constraint validator

**Trigger**: After initial generation, before evaluation.

---

## 6. Evidence Quality Assessment

### Confidence Levels by Finding

| Finding | Confidence | Evidence Base |
|---------|------------|---------------|
| Expert chunking | **High** | Multiple replications, computational models |
| DMN↔ECN switching | **High** | Large fMRI studies, behavioral consistency |
| Incubation effect | **Medium-High** | Meta-analysis, theoretical debate continues |
| Polymathy advantage | **High** | Statistical analysis of Nobel laureates |
| Combinatorial play | **Medium** | Case studies, theoretical models |
| Janusian process | **Medium** | Qualitative analysis, limited quantitative |

### Gaps in Research

1. **Quantitative measurement of combinatory play** — How many combinations before breakthrough?
2. **Optimal switching frequency** — What's the right DMN↔ECN balance?
3. **Individual differences** — Who benefits most from which technique?
4. **AI-specific validation** — How well do these patterns transfer to AI?

---

## 7. Implementation Priorities

### Ranked by Impact vs. Implementation Difficulty

| Priority | Component | Impact | Difficulty | Recommendation |
|----------|-----------|--------|------------|----------------|
| 1 | Pattern Library (Chunking) | High | Medium | **Implement first** — foundational |
| 2 | Combinatorial Engine | High | Medium | **Implement early** — generative core |
| 3 | Cognitive Flexibility Controller | High | Low | **Implement early** — mode switching |
| 4 | Cross-Domain Transfer | Medium | High | **Implement later** — requires domain library |
| 5 | Incubation Simulator | Medium | Medium | **Implement later** — optimization only |

---

## Key Takeaways for AI Brainstorming Skill Design

### 1. **Recognize Patterns, Don't Just Reason**
Expert intuition is recognition, not reasoning. Build pattern libraries indexed by deep structure principles.

### 2. **Switch Modes Deliberately**
Design explicit switches between generative (DMN-like) and evaluative (ECN-like) modes. Optimal creativity is balanced, not extreme.

### 3. **Embrace "Incubation" Through Latent Perturbation**
When stuck, encode the problem, perturb the representation, decode alternatives. This simulates unconscious work.

### 4. **Enable Cross-Domain Transfer**
Creativity comes from connecting distant domains. Build analogy-mapping across structural, not surface, similarity.

### 5. **Systematically Combine**
Make combinatory play explicit. Generate combinations without bias, then validate against constraints.

### 6. **Hold Contradictions**
The Janusian process — holding opposing ideas simultaneously — is critical for breakthrough insights. Build contradiction generation into the skill.

### 7. **Start with Questions, Not Solutions**
Feynman and da Vinci both emphasized naive questioning. Generate "stupid questions" to test understanding before proposing solutions.

---

## Sources

### Cognitive Psychology & Neuroscience
1. [Chi, Feltovich, & Glaser (1981) - Categorization by Experts and Novices](https://onlinelibrary.wiley.com/doi/abs/10.1207/s15516709cog0502_2)
2. [Larkin, McDermott, Simon, & Simon (1980) - Expert and Novice Problem Solving](https://www.researchgate.net/publication/281345564_Expert_and_Novice_Performance_in_Solving_Physics_Problems)
3. [Chen et al. (2025) - Dynamic Switching Between Brain Networks](https://www.nature.com/articles/s42003-025-07470-9)
4. [Wu & Koutstaal (2020) - Cognitive Flexibility and Creativity](https://journals.plos.org/plosone/article/file?id=10.1371%2Fjournal.pone.0234473)
5. [Scientific American - The Brain Science of Aha Moments](https://www.scientificamerican.com/article/the-elusive-brain-science-of-aha-moments/)
6. [Duke University - Brain Scans Reveal What Happens When Insight Strikes](https://today.duke.edu/2025/05/brain-scans-reveal-what-happens-mind-when-insight-strikes)

### Incubation & Insight
7. [Sio & Ormerod (2009) - Incubation Meta-Analysis](https://eric.ed.gov/?id=EJ827110)
8. [Gilhooly (2016) - Incubation and Intuition in Creative Problem Solving](https://www.frontiersin.org/articles/10.3389/fpsyg.2016.01076/full)

### Genius Case Studies
9. [The Marginalian - How Einstein Thought: Combinatory Play](https://www.themarginalian.org/2013/08/14/how-einstein-thought-combinatorial-creativity/)
10. [Psychology Today - Einstein's Creative Thinking](https://www.psychologytoday.com/us/blog/creative-explorations/201601/einsteins-creative-thinking-and-relativity)
11. [Farnam Street - Feynman's Mental Tools](https://fs.blog/mental-tools-richard-feynman/)
12. [Güss, Ahmed, & Dörner (2021) - da Vinci's Creative Process](https://journals.sagepub.com/doi/10.1177/1745691620966790)
13. [APS - The Creative Genius Within: Learning From Leonardo](https://www.psychologicalscience.org/news/minds-business/creative-genius-davinci.html)

### Polymathy & Cross-Domain Expertise
14. [Root-Bernstein & Root-Bernstein (2023) - Polymathy Among Nobel Laureates](https://eric.ed.gov/?id=EJ1381601)
15. [PMC - Creative Polymathy and the COVID-19 Crisis](https://pmc.ncbi.nlm.nih.gov/articles/PMC7769766/)
16. [The Conversation - Nobel Prizes Go to Researchers Who Defy Specialization](https://www.theconversation.com/nobel-prizes-most-often-go-to-researchers-who-defy-specialization-winners-are-creative-thinkers-who-synthesize-innovations-from-varied-fields-and-even-hobbies-186193)

### Mental Models & Expert Thinking
17. [Sources of Insight - Charlie Munger Mental Models](https://sourcesofinsight.com/charlie-munger-mental-models/)
18. [James Clear - A Lesson on Elementary Worldly Wisdom](https://jamesclear.com/great-speeches/a-lesson-on-elementary-worldly-wisdom-by-charlie-munger)

### AI Creative Reasoning
19. [ArXiv - Flow of Reasoning](https://arxiv.org/html/2406.05673v4)
20. [ArXiv - Distributional Creative Reasoning](https://arxiv.org/pdf/2601.00747)

### Chunking & Expertise
21. [Gobet (2005) - Chunking Models of Expertise](https://onlinelibrary.wiley.com/doi/10.1002/acp.1110)
22. [Gobet (1997) - Pattern Recognition Theory of Search](http://bura.brunel.ac.uk/handle/2438/843)

---

## Methodology

**Queries**: 18 web searches across cognitive psychology, neuroscience, genius case studies, AI reasoning, and mental models
**Sources Analyzed**: 30+ academic papers, articles, and case studies
**Sub-questions Investigated**:
1. Cognitive processes distinguishing expert from novice problem-solvers
2. Documented techniques from Feynman, Einstein, da Vinci
3. Neuroscience of insight and aha moments
4. AI/ML implementations of creative reasoning
5. Polymathy and cross-domain expertise
6. Incubation effects and mechanisms
7. Mental models frameworks for expert thinking

**Confidence Assessment**: High for chunking, network switching, polymathy; Medium for incubation mechanisms and combinatory play; Medium-High overall for AI implementation recommendations.

</research_corpus>

<constraints>

**DO:**

- **Preserve every concept, table, code block, YAML spec, citation, and effect size** from `<research_corpus>` as the evidence base for design decisions. When citing, quote exactly — do not paraphrase.
- For each of the **5 cognitive differentiators**, produce a corresponding skill component block specifying: (a) human basis with citation, (b) underlying cognitive operation, (c) AI-native mechanism, (d) **explicit point where the AI version surpasses the human version**, (e) trigger conditions, (f) integration points with other components, (g) confidence tier from the research's own evidence-quality assessment.
- Decide where the studied genius methodologies (Feynman's Algorithm including Naive Questions / Bayesian Thinking / Test Effect Strength / Recognize Information Gaps; Einstein's Thought Experiments; da Vinci's Observation-vs-Recognition and Nine-Stage Process; Janusian Process) live in the architecture. They may map onto existing components, become first-class components themselves, or form a separate cross-cutting meta-cognition layer. **State your design choice and avoid duplicating the same technique in two places.** Note that the research embeds Janusian directly inside the `combinatorial_play` YAML in §3.2 — you may keep it there or promote it, but do not list it in both.
- Structure output as **composable, modular blocks** (not flowing prose) so the second report can be merged in later without rewriting.
- Treat "applicability across every relevant situation" as a property of the **trigger and composition layer**, NOT as a reason to add per-situation components. The base pipeline stays small; situation-specific behavior emerges from how triggers fire and how the skill composes with other skills.
- **Do additional high-quality research as necessary, crawling pages and following subsequent pages from the source list in `<research_corpus>`** to get the full idea before producing the design. Cite sources inline. Do not invent evidence.
- Produce a concrete **SKILL.md skeleton** (frontmatter + section headings + numbered pipeline + verification gates) the merge step can later fill in.
- Use the research's own confidence tiers (High / Medium-High / Medium) to mark each component. Mark Medium-tier components as exploratory with explicit fallback behavior, **but include all five differentiators in the design** — the user wants the research integrated *in full*, not narrowed to a minimum viable subset.

**DO NOT:**

- Add components, modes, plug-in slots, or features that do not trace to a specific finding in `<research_corpus>` or a stated requirement here.
- Create one component per situation type, "future work" sections, or speculative branches.
- Mimic human limitations (working memory caps, sequential-only processing, fatigue, attention narrowing) when the AI implementation does not have them. Where the brain switches DMN↔ECN sequentially, the AI may run both in parallel — say so explicitly.
- Paraphrase or compress the research block when reproducing it for the next stage. Quote exactly when citing.
- Strip technical content (code blocks, YAML specs, tables, citations, effect sizes) for brevity.
- Output a finished SKILL.md. Output a **DESIGN PROPOSAL** that the merge step will turn into the final SKILL.md once combined with the corporate-brainstorming report.
- Conflate "the corporate use case" with "extra components". The dual-purpose design must use the SAME core pipeline; the difference lives in the trigger/wrapper layer.

</constraints>

<priority>

When constraints conflict, resolve in this order:

1. **No fabrication** — never invent evidence to fill a gap. Use Section 11 (Open Questions) instead.
2. **No over-engineering** > comprehensive coverage. A small composable pipeline that triggers correctly across situations beats a large pipeline with situation-specific branches.
3. **Confidence-tier honesty** — Mark each component with its research confidence tier (High / Medium-High / Medium per `<research_corpus>` §6). Treat Medium-tier components as exploratory and require explicit fallback behavior when their preconditions cannot be detected. **Include all five differentiators** — the user explicitly wants the research integrated in full. Confidence tiers govern *labeling and fallback*, not *inclusion*.
4. **AI-native > faithful translation** — when a faithful translation of a human technique would saddle the AI with an artificial limitation, design from the cognitive operation upward instead.
5. **Mergeability** > self-containment — leaving clean anchor points for the second report is more important than producing a complete-feeling document on its own.

</priority>

<reasoning_guidance>

Work through the design in this order so the analysis compounds correctly:

1. **Distill** — Read `<research_corpus>` end-to-end and extract the *underlying cognitive operation* each technique performs (not the technique label itself). Example: "Janusian process" is the label; the operation is "represent two contradictory hypotheses simultaneously and search for a synthesis state that satisfies both."

2. **Compare** — Identify cognitive operations that appear in 2+ techniques. Common operations are stronger design signals than single-source techniques. Example: "hold contradictory state" appears in Janusian, Einstein's thought experiments, and combinatorial play; that convergence justifies a first-class component.

3. **AI-native re-derivation** — For each common operation, ask: how would a system *without working-memory limits, with parallel sampling, with cheap deterministic replay, and without fatigue* implement this? Write the AI version FROM SCRATCH against the cognitive operation, not by translating the human procedure step by step. State the surpassing point.

4. **Compose** — Assemble the AI-native operations into a minimal pipeline with explicit trigger conditions (when does this fire?) and exit conditions (when does it stop?). No "always on" components.

5. **Dual-mode design** — Show how the SAME core pipeline serves both corporate brainstorming and standalone reasoning enhancement, with the difference living entirely in the trigger/wrapper layer, never in the core.

6. **Self-critique** — For each design choice, name a specific way it could fail or be over-engineered. Cut anything that fails this test. Apply the Feynman naive-question test from `<research_corpus>` §2.1: ask "what is this component actually doing, in one sentence, and would removing it break anything?"

</reasoning_guidance>

<output_format>

Produce the design proposal using the structure below as a **recommendation**. The Section 10 merge anchors **must be preserved exactly** (the second report depends on them), but you may adapt other section organization if the synthesis surfaces a better arrangement. Keep section headings clear and discoverable so the merge step can locate them:

```
## genius-epiphany — Design Proposal (Genius-Mind Foundations)

### 1. Cognitive Operations Distilled
[Table: Technique from research → underlying cognitive operation → confidence tier]

### 2. Common Operations Across Techniques
[List of operations that appear in 2+ techniques, with source citations from <research_corpus>]

### 3. AI-Native Re-Derivation
[For each common operation: human version → AI-native version → where AI surpasses human → why]

### 4. Proposed Skill Pipeline
[Numbered steps with explicit trigger conditions, explicit exit conditions, and an explicit list of what the skill does NOT do. Keep the pipeline as small as possible while still integrating all five differentiators — situation-coverage comes from triggers and composition, not from adding pipeline steps.]

### 5. Component Specifications
[For each of: Chunking / Pattern Library, Cognitive Flexibility / Network Switching, Cross-Domain Transfer / Polymathy, Combinatorial Play, Incubation — one modular block per the template below]

### 6. Treatment of the Studied Genius Methodologies (architecture-decision section)
[Document where Feynman's tools, Einstein's thought experiments, da Vinci's observation-vs-recognition, and the Janusian process live in your design. Three valid choices: (a) absorbed into existing Section 5 components, (b) promoted to first-class components in Section 5, (c) collected into a separate cross-cutting meta-cognition layer. State which choice you made and why. If choice (a), this section can be a one-paragraph pointer to where each methodology lives in Section 5.]

### 7. Composition Model — Standalone Reasoning Enhancer
[How genius-epiphany wraps another SKILL.md skill's execution. Explicit trigger conditions. Explicit hands-off conditions. How it avoids interfering with skills that should not be enhanced.]

### 8. Composition Model — Corporate Enterprise Brainstorming and Planning
[How the same core pipeline serves enterprise ideation, decision-framing, and strategic synthesis. Explicit hooks where the SECOND report (corporate techniques) will plug in.]

### 9. SKILL.md Skeleton
[Frontmatter + section headings + numbered pipeline placeholders + verification gate placeholders, ready for the merge step to populate]

### 10. Merge Hooks for Corporate-Brainstorming Report
[Named anchors / section IDs the second report should target when fused. Be explicit and mechanical so the merge does not require a redesign pass.]

### 11. Open Questions and Research Gaps
[Honest list of what could not be grounded in <research_corpus> and would require additional evidence. Use the escape hatch — do not fabricate.]
```

**Section 5 component-block template** — every component in Section 5 must include the **required** fields. Optional fields are recommended for richer components but may be omitted if they would be padding:

```
**Component: [name]**

Required:
- Human basis: [from <research_corpus>, with inline citation]
- Cognitive operation: [the underlying thing it does in one sentence]
- AI-native mechanism: [how a SKILL.md agent executes it]
- Where AI surpasses human: [specific computational advantage with one-line justification]
- Confidence tier: [High / Medium-High / Medium per <research_corpus> §6]

Optional but recommended:
- Trigger: [when this component activates]
- Exit condition: [when this component stops]
- Integration points: [which other components feed it / which it feeds]
- Failure mode: [one specific way this could go wrong, from the self-critique step]
```

</output_format>

<edge_cases>

- **Untranslatable findings** — If a finding in `<research_corpus>` cannot be cleanly translated into an AI-native mechanism, list it in Section 11 (Open Questions) rather than forcing a translation.
- **Operation collapse** — If two cognitive operations from different techniques collapse into the same AI-native mechanism, MERGE them into a single component and call this out explicitly with citations to both source techniques.
- **Medium-confidence components** — If the medium-confidence findings (Incubation mechanism specifics, Janusian process, Combinatorial Play quantification) cannot be operationalized without speculation, mark them as "exploratory" components and specify a fallback behavior of `no-op` when their preconditions cannot be detected.
- **Corporate gaps** — If a corporate-brainstorming use case appears to require a feature not derivable from genius-mind research, list it in Section 10 (Merge Hooks) as a dependency on the second report rather than inventing it here.
- **External research** — If you must perform additional web research, cite sources inline; do not present synthesized claims as if they came from `<research_corpus>`. Mark any external claim with `[external: source]`.
- **Composition conflicts** — If wrapping certain skill types could degrade their output (e.g., trivial single-step skills, time-critical skills), specify the hands-off conditions in Section 7 explicitly.
- **Always-on temptation** — If you find yourself describing a component that has no exit condition, that is a design smell. Either define an exit condition or move it to Section 11 as an open question.

</edge_cases>

<verification>

Before finalizing, run through these checks. Any failure → revise before output.

1. **Component traceability** — Every component in Section 5 traces to a specific finding in `<research_corpus>` AND specifies a computational advantage over the human version.
2. **Trigger/exit completeness** — The pipeline in Section 4 has explicit trigger AND explicit exit conditions for every step. No "always on" components anywhere.
3. **No situation branches** — No section adds situation-specific branches. Situation-coverage emerges from triggers and composition only.
4. **Research preservation** — The output preserves every table, code block, YAML spec, and citation from `<research_corpus>` (or references them by exact section name when not reproduced inline).
5. **Mergeability** — Section 10 (Merge Hooks) is mechanical. A second author should be able to insert content from the corporate-brainstorming report without redesigning Sections 1-9.
6. **Honesty gate** — Section 11 (Open Questions) is non-empty if any design decision required reasoning beyond the research. Honesty over completeness.
7. **Anti-inflation gate** — For every component, ask: "what is this actually doing, in one sentence, and would removing it break anything?" (Feynman naive-question test from `<research_corpus>` §2.1). Cut any component that fails.
8. **AI-surpassing gate** — Every component in Section 5 includes an explicit "Where AI surpasses human" line. If you cannot fill it in, the component is a transliteration, not a translation — redesign it.
9. **Dual-purpose unity** — Sections 7 and 8 use the SAME pipeline from Section 4. If the corporate-brainstorming model requires modifying the core pipeline, that is a design failure — fix Section 4 instead.
10. **Escape hatch active** — If any required information is missing from `<research_corpus>` and external research did not resolve it, state what is missing in Section 11 rather than guessing.

</verification>
