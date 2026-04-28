# 01-analysis.md — M12 Phase 1 Analysis
# Scale: STANDARD | Mode: normal | Session: 20260414-deep-research-subtractive-synthesizer-dsp

---

## INTENT

**What the prompt is trying to accomplish:**
The user wants a deep research survey of DSP algorithms and techniques used in subtractive synthesizer design that are:
1. Freely available (open-source, academic, or royalty-free)
2. Implementable in C++ within a JUCE VST3 plugin
3. Capable of producing professional-grade "analog warmth" rather than digital/clinical sound character

**Desired end state:**
A comprehensive, actionable reference covering the full DSP stack for a professional subtractive synthesizer — oscillators, filters, envelopes, modulation, anti-aliasing, saturation/warmth circuits — with guidance on which algorithms are most effective and why they create analog-sounding results.

**Success criteria:**
- Research covers the core signal chain: oscillators → filter → amplifier → modulation
- Each algorithm or technique is accompanied by implementation-relevant information (freely available, C++-friendly, JUCE-compatible)
- Emphasis on analog warmth characteristics: harmonic content, self-oscillation behavior, subtle non-linearity, pitch drift, soft saturation
- Sufficient depth to guide implementation decisions (not just naming techniques but explaining tradeoffs)

---

## STRUCTURE

**Current organization:**
- Single run-on sentence, no sections, no hierarchy
- No role or persona assigned to the research agent
- No explicit output format requested
- No domain context established (skill level, target complexity, scope boundaries)

**Missing structural elements:**
- Role/persona framing (expert DSP researcher, audio engineer with analog modeling background)
- Decomposed research areas (oscillator algorithms, filter algorithms, modulation, anti-aliasing, saturation/warmth)
- Output format specification (what should the research look like? structured sections? comparison tables? code references?)
- Scope constraints (what counts as "freely available"? academic papers? open-source repos? public domain implementations?)
- Priority ordering (which sub-topics are most impactful for "analog warmth"?)
- Consumer context (C++ beginner, JUCE plugin target — this affects depth of explanation needed)

---

## CONSTRAINTS

**Explicit constraints in input:**
- Algorithms must be "freely available" (no commercial/proprietary-only techniques)
- Implementation target: JUCE VST3 plugin in C++
- Quality target: "most professional" sound
- Sound character target: "analog warmth rather than digital sounding"

**Implicit constraints that should be made explicit:**
- Algorithms should be implementable in real-time audio thread context (no blocking, bounded CPU)
- Must be suitable for polyphonic voice architecture (voice-level DSP, not just global effects)
- Should not require external proprietary DSP libraries (JUCE-only or standard C++ math)
- Research should be practically oriented — not purely theoretical (the user is building, not just studying)
- "Freely available" likely means: open-source licenses (MIT, GPL, BSD), academic publications with clear descriptions, or public-domain implementations

**Conflicts:**
- None identified. "Most professional" and "freely available" are not inherently contradictory in the DSP domain (many industry-standard algorithms are well-documented in literature).

---

## TECHNIQUES

**T1 — XML semantic structuring:** NOT PRESENT. Single unstructured sentence. NEEDED — high impact. The prompt needs `<context>`, `<task>`, `<research_areas>`, `<constraints>`, `<output_format>` sections to guide a research agent.

**T2 — Prompt decomposition:** NOT PRESENT. Monolithic single sentence. NEEDED — high impact. Breaking into sub-research areas (oscillators, filters, modulation, warmth/saturation, anti-aliasing) would dramatically improve research quality and coverage.

**T3 — Explicit constraint specification:** PARTIALLY PRESENT (freely available, C++, JUCE, analog warmth implied). NEEDED — medium impact. Implicit constraints (real-time safe, polyphonic, no external proprietary libs) should be made explicit.

**T4 — Role/persona assignment:** NOT PRESENT. NEEDED — medium impact. Assigning an expert DSP research persona grounds the response in domain expertise.

**T5 — Output format templates:** NOT PRESENT. NEEDED — high impact. A structured output format (per-algorithm entries with name, category, description, analog warmth contribution, implementation notes, references) would make research directly actionable.

**T6 — Structured reasoning injection:** NOT PRESENT. NEEDED — medium impact. Adding CoT guidance for evaluating "analog warmth" quality of each technique would improve discriminative depth.

**T7 — Priority hierarchy:** NOT PRESENT. NEEDED — medium impact. "If techniques conflict in complexity vs. warmth tradeoff, prioritize warmth contribution." Helps the research agent make useful tradeoff recommendations.

**T8 — Boundary/edge case spec:** NOT PRESENT. USEFUL — low impact. Edge cases: what if an algorithm has no freely available implementation? What if analog warmth and computational cost conflict?

**T9 — Few-shot exemplar injection:** NOT PRESENT. OPTIONAL — low impact. Could demonstrate desired output format with one example algorithm entry, but may over-constrain creative research scope.

**T10 — Self-critique/validation:** NOT PRESENT. USEFUL — low impact. "Before finalizing, verify each listed algorithm has a confirmed free/open implementation reference."

**T11 — Context preservation anchoring:** NOT PRESENT. LOW NEED — input is short, not at risk of concept drift.

**T12 — Audience calibration:** NOT PRESENT. NEEDED — medium impact. Specifying that the consumer is a C++ beginner learning JUCE, needing explanations of C++ concepts inline, guides the depth and accessibility of the research output.

**T13 — Escape hatch provision:** NOT PRESENT. USEFUL — low impact. "If a sub-area has no freely available algorithms, state this explicitly rather than omitting the topic."

**Techniques to apply:** T1, T2, T3, T4, T5, T6, T7, T12 (high/medium impact). T8, T10, T13 as supporting additions.

---

## WEAKNESSES

**W1 — Monolithic unstructured request:**
Single sentence covers multiple distinct research domains (oscillators, filters, modulation, saturation, anti-aliasing) without decomposition. A research agent receiving this is likely to produce a shallow survey across all areas rather than depth in the most impactful ones.

**W2 — "Analog warmth" is undefined:**
The key quality target is stated as "analog warmth rather than digital sounding" but no criteria are given. What makes something sound "analog warm"? (Even harmonics, soft clipping, filter self-oscillation, pitch drift, slight detuning, noise floor character.) Without definition, the research agent cannot evaluate techniques against this criterion.

**W3 — "Freely available" is ambiguous:**
Does this mean open-source code repositories? Academic papers with pseudocode? Patent-expired algorithms? Public domain C++ implementations? The ambiguity could yield research citing techniques that are theoretically described but have no usable implementation reference.

**W4 — No output format specified:**
The research could be returned as a narrative essay, a bullet list, or a structured reference. Without format guidance, the output may be difficult to use as an implementation reference.

**W5 — Consumer context absent:**
The research consumer is a C++ beginner building a JUCE plugin. This affects what "implementable" means — complex DSP mathematics without explanation would not serve this user. This context is missing.

**W6 — No prioritization of research areas:**
All subtractive synthesis DSP is treated with equal weight. In practice, filter algorithms and anti-aliasing have the highest impact on "analog warmth" vs. oscillator waveform generation. Without prioritization, the research may under-serve the most impactful areas.

**W7 — "Most professional" is unanchored:**
Professional compared to what baseline? This phrase could guide the research toward academic completeness at the cost of practical implementability. Needs grounding in the target use case.
