# 02-ideation.md — M12 Phase 2 Enhancement Contracts
# Scale: STANDARD | Session: 20260414-deep-research-subtractive-synthesizer-dsp
# Contracts for M3 Synthesis to execute. Primary pass (STANDARD scale — no anti-conformity pass).

---

- technique: T2
  target_section: global
  action: "Decompose the single research request into 6 explicit sub-research areas: (1) Oscillator algorithms, (2) Filter algorithms, (3) Anti-aliasing and bandlimited synthesis, (4) Saturation and harmonic enrichment for analog warmth, (5) Modulation sources (LFO, envelope), (6) Polyphonic voice architecture and gain staging."
  rationale: "The monolithic request spans multiple distinct DSP domains. Decomposition ensures depth across each area rather than a shallow survey, and allows the research agent to allocate effort proportionally to impact on analog warmth."
  priority: high

- technique: T1
  target_section: global
  action: "Wrap the prompt in XML semantic sections: <context>, <task>, <research_areas>, <constraints>, <output_format>, <quality_criteria>."
  rationale: "XML structure makes the prompt machine-parseable for a research agent. Clear section boundaries prevent conflation of context, task, and constraints, improving focus and completeness of the response."
  priority: high

- technique: T5
  target_section: "<output_format>"
  action: "Specify a per-algorithm structured output format for each research area entry: Algorithm Name | Category | Plain-language description | How it contributes to analog warmth | Key implementation considerations for C++ / JUCE | Freely available reference (paper, repo, or implementation). Also request a summary comparison table at the end ranking top recommendations by warmth impact."
  rationale: "Without format specification the research may be returned as narrative prose, which is hard to use as an implementation reference. A structured per-algorithm format makes the output directly actionable for a developer building the plugin."
  priority: high

- technique: T4
  target_section: "<context>"
  action: "Assign expert research persona: 'You are an expert audio DSP engineer and researcher specializing in analog hardware modeling and virtual analog synthesis. You have deep knowledge of classic synthesizer circuit design and their digital equivalents.'"
  rationale: "A grounded expert persona aligns the research agent's vocabulary, depth, and tradeoff framing with the domain. Without it, the agent may produce generic survey content lacking the nuance needed to distinguish genuinely warm-sounding algorithms from merely functional ones."
  priority: medium

- technique: T3
  target_section: "<constraints>"
  action: "Convert all implicit constraints to explicit DO/DO NOT rules: (1) DO only include algorithms with a confirmed freely available reference — open-source repository, peer-reviewed publication with full pseudocode, or public domain implementation. (2) DO NOT include algorithms requiring proprietary DSP libraries. (3) DO prioritize algorithms suitable for real-time audio thread execution (bounded, predictable CPU cost, no dynamic memory allocation). (4) DO include algorithms appropriate for polyphonic voice architecture. (5) DO NOT present techniques as 'freely available' without citing a specific reference."
  rationale: "The input's key constraint 'freely available' is ambiguous. Making constraints explicit prevents the research agent from citing techniques that are described in literature but have no usable implementation, or that require commercial licenses."
  priority: high

- technique: T3
  target_section: "<quality_criteria>"
  action: "Define 'analog warmth' explicitly with measurable/observable criteria: even-order harmonic saturation, smooth frequency response rolloff (no digital brickwalling), filter self-oscillation behavior, subtle pitch and amplitude drift characteristics, soft-knee saturation rather than hard clipping, slight non-linearity in filter resonance response. Instruct the research agent to evaluate each algorithm against these criteria."
  rationale: "W2: 'analog warmth' is the central quality target but is undefined. Without criteria, the agent cannot meaningfully evaluate which algorithms produce warmer results. Defining it grounds the research in observable DSP characteristics."
  priority: high

- technique: T12
  target_section: "<context>"
  action: "Specify the output consumer context: 'The reader is a C++ beginner learning JUCE who knows Python and JavaScript well. Explain DSP mathematics conceptually with C++-relevant implementation notes. Prefer clarity over academic formalism. When citing algorithms, note any JUCE-specific helper classes or modules that simplify implementation.'"
  rationale: "W5: The consumer context is absent. A C++ beginner needs different depth and framing than an experienced DSP engineer. This context guides the research agent to produce accessible, actionable explanations rather than mathematics-heavy academic summaries."
  priority: medium

- technique: T7
  target_section: "<constraints>"
  action: "Add priority hierarchy rule: 'If a technique scores high on analog warmth but has significant computational cost, present it with a note on the tradeoff rather than excluding it. If two algorithms serve the same role, prefer the one with a more accessible open-source implementation for a C++ beginner. If analog warmth contribution and implementation simplicity conflict, prioritize warmth contribution but document the complexity cost.'"
  rationale: "W6/W7: Without priority rules, the research agent makes implicit tradeoff decisions that may not serve the user's goal. Explicit hierarchy ensures warmth is the primary optimization axis while acknowledging complexity."
  priority: medium

- technique: T6
  target_section: "<task>"
  action: "Inject structured reasoning guidance: 'For each sub-area, reason through: (a) What analog hardware behavior is being modeled? (b) What are the most commonly cited algorithms in DSP literature and open-source synthesizers? (c) Which algorithms produce the most perceptually warm result and why? (d) What are the C++/JUCE implementation considerations? Then synthesize your findings into recommendations.'"
  rationale: "Chain-of-thought framing guides the research agent to move from hardware analog behavior through literature review to practical recommendations — producing richer, more grounded output than a direct 'list algorithms' request."
  priority: medium

- technique: T8
  target_section: "<edge_cases>"
  action: "Add boundary case specifications: 'If a research sub-area has no freely available algorithms (e.g., a technique is only described in proprietary patents), state this explicitly and suggest the nearest open alternative. If a technique is theoretically superior but has no known C++ implementation, note it as a research reference only, not a build recommendation.'"
  rationale: "W3: Without edge case handling, the research may silently omit topics where free implementations are sparse, or include techniques the user cannot actually implement."
  priority: low

- technique: T10
  target_section: "<verification>"
  action: "Add self-validation step: 'Before finalizing the research output, verify: (1) Every listed algorithm cites a specific freely available reference (not just a general description). (2) All six sub-research areas are covered. (3) The analog warmth criteria defined in <quality_criteria> are addressed for each major algorithm. (4) At least one recommendation is given per sub-area. If any check fails, add the missing information before responding.'"
  rationale: "Self-critique ensures completeness and prevents the research agent from returning output with gaps in coverage or uncited claims."
  priority: low

- technique: T13
  target_section: "<edge_cases>"
  action: "Add escape hatch: 'If you cannot identify a freely available implementation for a specific sub-area of subtractive synthesis DSP, state clearly what is missing, why, and what partial information is available rather than omitting the topic.'"
  rationale: "Prevents silent omission of hard-to-source topics, ensuring the user knows when coverage is incomplete rather than assuming the absence of a topic means it is unimportant."
  priority: low
