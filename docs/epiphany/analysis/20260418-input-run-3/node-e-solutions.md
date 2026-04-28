# Node E — Solutions

Two-drafts-pick-best protocol applied to all 38 D entries.
Level A verify checks: (a) addresses D gain, (b) no regression of existing Node A content, (c) precise locator, (d) realistic to implement in the brief.

---

## E001 — Adaptive calibration indicators per boundary
**from_d_id:** D001
**from_idea_id:** I001
**target:** {"kind": "anchor", "before": "If a calibration milestone is not met, maintain the current confidence level and re-evaluate at the next phase transition", "after": "In the dual-reader format for DSP sections"}
**action:** insert-after
**text:** |
  - **Boundary-level calibration indicators (augment phase-level milestones):** Each of the five domain boundaries (creative intent→sound design, sound design→DSP parameters, DSP→C++ implementation, C++→UI, UI→marketing) carries its own calibration depth requirement derived from boundary impedance. High-impedance boundaries (sound-design→DSP, DSP→C++) require ≥2 successful developer-confirmed round-trips before the developer is considered calibrated at that boundary; low-impedance boundaries (C++→UI, UI→marketing) require ≥1. Creative-intent complexity modulates the threshold: a multi-layer abstract intent at the sound-design→DSP boundary requires a more stringent calibration event than a simple single-descriptor intent at the same boundary. Track calibration level per-boundary in the session handoff document. The three-level global milestones (AI-guided/Collaborative/Developer-led) summarize the boundary-level matrix; the boundary-level indicators are the source of truth.
**rationale:** Adds boundary-specific calibration depth requirements alongside the existing phase-level milestones, implementing D001's core gain without replacing or regressing any existing calibration schedule content. Uses insert-after to preserve full existing text.
**compare_notes:** Draft 1 (replace calibration paragraph) scored lower — replacing risks losing the behavioral indicator percentages (50%/70%/90%) which are specific and valuable; insert-after adds the boundary layer while preserving them.

---

## E002 — Confidence-delta diagnostic protocol
**from_d_id:** D002
**from_idea_id:** I002
**target:** {"kind": "anchor", "before": "- DO rely on the RAG system's native confidence scoring to flag uncertain results", "after": "- DO treat KB gaps as detected dynamically"}
**action:** insert-after
**text:** |
  - **DO track confidence-score deltas across consecutive domain-boundary retrievals** — for each multi-step translation chain (creative phrase → sound design → DSP parameters → C++ implementation), the agent logs the confidence score returned at each retrieval step and computes the delta between adjacent steps. A drop exceeding 0.20 between consecutive steps flags that boundary as a potential creative-intent loss point and triggers a calibration event: pause and request developer confirmation that the output at that boundary matches the creative intent before proceeding. The delta is computed by the agent in the session context (not a Dify infrastructure requirement); log deltas to the session handoff document for boundary impedance tracking.
**rationale:** Converts confidence-score monitoring from passive threshold filtering to active impedance diagnostics per D002's gain. Uses insert-after to add a new DO clause without touching existing confidence-scoring language.
**compare_notes:** Draft 1 (add as bullet in RAG configuration section of output_format) scored lower — the constraints section is where DO/DO NOT rules live; adding to output_format would put the rule in the wrong structural location.

---

## E003 — OoD confidence floor with paraphrase fallback
**from_d_id:** D003
**from_idea_id:** I003
**target:** {"kind": "anchor", "before": "- **Creative intent outside established vocabulary:**", "after": "- **KB error correction:**"}
**action:** insert-before
**text:** |
  - **OoD confidence floor and developer-confirmed paraphrase protocol:** The no-paraphrase rule (DO NOT paraphrase the developer's original creative intent phrase) applies for all creative phrases returning RAG confidence ≥0.35 across sound-design-domain documents after the relevant KB domain is populated. For phrases returning confidence <0.35 after KB population: (a) present the novel term alongside the best established synonym with both confidence scores to the developer, (b) developer selects one of: accept synonym (logs as "developer-confirmed synonym" in session context and proceeds with synonym as the query anchor), confirm novelty (proceeds to KB content harvest), or reject both and rephrase (uses the developer's own rephrasing as the new anchor), (c) the no-paraphrase rule resumes once a confirmed anchor is established. This protocol resolves the collision between preserving creative phrase fidelity and handling genuinely novel vocabulary outside the RAG system's bridgeable range.
**rationale:** Resolves TC-1 collision (no-paraphrase vs. OoD handling) by defining a concrete confidence floor and a three-option developer-confirmed paraphrase protocol. Inserted before the existing OoD edge case paragraph, which it supplements rather than replaces.
**compare_notes:** Draft 1 (append to "Creative intent outside established vocabulary" edge case) scored lower — placing the rule in `<edge_cases>` makes it feel optional; the no-paraphrase constraint is in `<constraints>`, so the protocol belongs there as a clarifying clause, not in edge cases.

---

## E004 — Standing-wave guard
**from_d_id:** D004
**from_idea_id:** I004
**target:** {"kind": "anchor", "before": "- **Context resolution via RAG:**", "after": "- **Creative intent outside established vocabulary:**"}
**action:** insert-before
**text:** |
  - **Standing-wave guard — cross-boundary coherence check:** After applying adaptive calibration at individual domain boundaries, run a cross-boundary coherence check at each phase transition (after each Development Phase exits). The check: for each active creative phrase, compare the calibration adjustments made at consecutive boundaries N and N+1 during the just-completed phase. If adjustments at boundary N and boundary N+1 are in opposing directions (one emphasizes a frequency range, the other de-emphasizes it; one adds a processing stage, the other removes an equivalent one), log a standing-wave warning to the session handoff document and request developer review before the next phase begins. The guard does not block execution — it surfaces a systematic bias pattern that neither boundary can detect in isolation. Developer review: listen to the output at the flagged boundary pair and confirm the net effect is intentional.
**rationale:** Adds the standing-wave guard as a concrete Cross-Domain Translation protocol, implementing D004's gain without overengineering. Uses insert-before to slot into the correct position within the Cross-Domain Translation constraints section.
**compare_notes:** Draft 1 (add to Technical Architecture output_format section) scored lower — the standing-wave guard is a runtime translation protocol, not an architectural specification; it belongs in the Cross-Domain Translation constraints where other runtime translation rules live.

---

## E005 — Change-impact enumeration before creative pivot
**from_d_id:** D005
**from_idea_id:** I006
**target:** {"kind": "anchor", "before": "- **Creative vision changes mid-development:** When the developer's creative vision shifts:", "after": "This replaces the previous cache key mutation/extension model"}
**action:** replace
**text:** |
  - **Creative vision changes mid-development:** When the developer's creative vision shifts: (1) **run change-impact analysis first** — enumerate all downstream artifacts carrying the superseded creative phrase's `creative_phrase_tag`; classify each as: (a) must re-derive (DSP implementations, preset defaults that directly implement the creative phrase), (b) should review (UI labels, marketing copy that reference the phrase), (c) unaffected (structural architecture, licensing system, budget allocations); present this enumeration to the developer for approval before any re-derivation begins, (2) document the pivot as a commission brief addendum with `spec_version` incremented, (3) query the RAG system with the evolved creative phrases to retrieve new domain knowledge, (4) if the RAG returns different knowledge for the new phrases, update implementations per the approved re-derive list, (5) mark superseded creative intent as "superseded" in project documentation — the KB content itself does not need modification because the RAG system retrieves based on the current query, not historical entries. This replaces the previous cache key mutation/extension model — the RAG system naturally handles creative evolution because each query is independent.
**rationale:** Replaces the existing creative vision change edge case with an enhanced version that adds the change-impact analysis step (step 1) and the spec_version increment (step 2) before the existing steps. All existing content is preserved in steps 3–5.
**compare_notes:** Draft 1 (insert-after with a new step only) scored lower — inserting a new step without modifying the existing numbered list creates ambiguity about step ordering; replace-with-integrated-text is cleaner and avoids numbered-list confusion.

---

## E006 — Domain-context metadata on RAG queries
**from_d_id:** D006
**from_idea_id:** I007
**target:** {"kind": "anchor", "before": "- **Agent workflow and KB access:**", "after": "- **DSP decision opacity and calibration schedule:**"}
**action:** insert-after
**text:** |
  - **RAG query construction with domain-context metadata:** Every agent-issued RAG query during development phases carries a `domain_context` field matching the current execution phase's primary translation domain. This field is used as a relevance-weighting signal in the Dify retrieval configuration: results from the `domain_context`-matching KB domain are ranked higher than results from unrelated domains. Query template: `{creative_phrase} + [phase_context] + [domain_context] + [implementation_target]`. Example: `"warm pad" + "Phase 4 DSP implementation" + "domain_context: dsp" + "JUCE 8 SmoothedValue cutoff automation"`. Domain-context values by phase: Phases 0–2 → `sound_design`; Phase 4 → `dsp`; Phase 7 → `ui_ux`; Phase 8 (Licensing) → `licensing`; Phase 8 (Marketing) → `marketing`. This is query-side relevance weighting, not retrieval gating — all KB domains remain accessible, but context-irrelevant results rank lower, reducing cross-domain contamination.
**rationale:** Adds the domain-context metadata protocol to the Agent Workflow constraints section where agent behavior rules live. Includes the query construction template from D031 in the same location to avoid redundancy (D031 targets KB Design separately — this is the constraint version).
**compare_notes:** Draft 1 (add to KB Design section only) scored lower — the query construction behavior is an agent workflow constraint, not just a KB design requirement; it must appear in `**Agent Workflow:**` to be agent-executable.

---

## E007 — Budget reallocation protocol
**from_d_id:** D007
**from_idea_id:** I008
**target:** {"kind": "anchor", "before": "- DO scope every phase for completion by a single developer", "after": "- DO design phases to produce shippable milestones"}
**action:** insert-before
**text:** |
  - **Budget reallocation protocol:** When a phase overruns its guideline allocation: up to 25% of one phase's guideline allocation may be transferred to an adjacent phase, subject to the total monthly ceiling. Protocol: (a) document the reason for the overrun and the proposed transfer in the session handoff document, (b) identify the source phase whose allocation will be reduced, (c) verify the total monthly cost across all phases remains within the $100–200/month ceiling. The protocol is self-service — no approval required beyond logging. Example: Phase 4 (DSP, 25% allocation) overruns by $10 → borrow 25% of Phase 5 (State, 15% allocation) = $3.75–7.5, and 25% from Reserve (10% allocation) = $2.5–5, for a total transfer of up to $6.25–12.50. Phases not yet started are available as transfer sources if their share of the monthly budget has not yet been spent.
**rationale:** Adds a concrete reallocation rule to the Budget and Resources constraints section, resolving TC-4 (per-phase allocation vs. total ceiling flexibility). Placed before the "single developer scope" constraint as it is a budget rule, not a scope rule.
**compare_notes:** Draft 1 (add as footnote after per-phase budget allocation guideline) scored lower — a footnote would be easy to miss; adding as a full DO bullet in the constraints section gives it appropriate visibility and enforceable status.

---

## E008 — Phase 1 RAG measurement tasks and framing
**from_d_id:** D008
**from_idea_id:** [I009, I012]
**target:** {"kind": "anchor", "before": "- **Phase 1 (Performance Audit) = none (manual baseline measurement)**", "after": "- **Phase 2 (Architecture) = `writing-plans`**"}
**action:** replace
**text:** |
  - **Phase 1 (Performance Audit) = RAG fidelity baseline + manual performance baseline.** Agent tasks: (a) issue 5–10 representative creative queries spanning the developer's Phase 0 creative vocabulary across all five domain boundaries; log the confidence score returned at each domain-boundary retrieval step and compute per-boundary deltas → produces the boundary impedance map, (b) compile 20 creative terms (10 from established sound-design vocabulary, 10 unusual but in-scope for this plugin's creative identity); for each, retrieve DSP parameters via RAG, implement a minimal test patch (10–20 lines JUCE), render a 5-second audio test, evaluate by ear, score correctness → produces the (confidence_score, correctness) table used to calibrate Phase 4 confidence thresholds. Phase 1 deliverables: (1) boundary impedance map, (2) confidence-correctness calibration table, (3) confidence thresholds for Phase 4 (calibrated from table, not assumed), (4) any pre-existing manual performance baselines. Human responsibility: evaluate test patch audio output, score correctness.
**rationale:** Replaces the single-sentence Phase 1 entry in the Agent Workflow skill-to-phase mapping with a fully specified Phase 1 task list, implementing D008's gain. The replacement captures I009's boundary measurement and I010's correctness-calibration tasks as Phase 1 deliverables.
**compare_notes:** Draft 1 (insert-after the existing Phase 1 entry) scored lower — the existing entry "Phase 1 = none (manual baseline measurement)" contradicts the new content; inserting after leaves the contradiction in place; replacing cleanly resolves it.

---

## E009 — RAG confidence-correctness correlation — moved into E008
*(D009 target is the same Phase 1 agent task as D008 — content integrated into E008 above)*

**from_d_id:** D009
**from_idea_id:** I010
**target:** {"kind": "anchor", "before": "- DO rely on the RAG system's native confidence scoring", "after": "- DO treat KB gaps as detected dynamically"}
**action:** insert-after
**text:** |
  - **DO validate RAG confidence thresholds against functional correctness before Phase 4** — the RAG system's confidence scores indicate semantic similarity, not functional correctness of retrieved DSP parameters. Before using confidence thresholds as quality gates for Phase 4 DSP decisions, calibrate them against actual DSP output using the Phase 1 (confidence-correctness calibration table) results. If the Phase 1 table shows that confidence 0.70 terms produce incorrect sonic output 40% of the time, raise the Phase 4 confidence threshold above 0.70 for that domain. The confidence thresholds specified in this section (0.40/0.60 boundaries) are starting points, not permanent values — they must be recalibrated using Phase 1 empirical data before Phase 4 begins.
**rationale:** Adds a confidence-calibration DO clause to the Knowledge Base constraints section, making the RAG calibration requirement architecturally explicit and pointing to the Phase 1 deliverable as the calibration source.
**compare_notes:** Draft 1 (add only to Phase 1 agent task description) scored lower — putting it only in Phase 1 agent tasks makes it easy to miss as a constraint; it belongs in `**Knowledge Base:**` as a DO rule that governs how confidence thresholds are used throughout development.

---

## E010 — Qualify market opportunity with boundary conditions
**from_d_id:** D010
**from_idea_id:** I011
**target:** {"kind": "anchor", "before": "**Why upfront specification for this context:**", "after": "**System boundaries** (distinct from product scope exclusions):"}
**action:** insert-before
**text:** |
  **Market opportunity boundary conditions:** The creativity-gap opportunity claim — "AI-assisted development makes complex DSP achievable for solo developers, creating a defensible market position" — holds under three simultaneous conditions: (a) mid-range gap width: the developer is not a DSP expert (gap→0 collapses the moat, as expert competitors can produce equivalent quality) AND is not a complete novice with no creative vocabulary (gap→infinity makes RAG bridging infeasible with any KB content), (b) accumulated RAG content: the moat is not formed at project start — commercial viability claims before Phase 4 and before KB population is substantial are premature, (c) creative vocabulary irreducibility: the developer's specific creative language is not easily replicated by a competitor with equal AI tools. These conditions make the market opportunity claim precise, falsifiable, and actionable rather than aspirational.
**rationale:** Adds market opportunity boundary conditions to the System Vision guidance in output_format Section 1, implementing D010's gain. Inserted before the "Why upfront specification" paragraph where architectural reasoning lives.
**compare_notes:** Draft 1 (insert after market_opportunity context paragraph) scored lower — placing in `<context>` rather than `<output_format>` would create a context statement without telling the AI agent how to include it in the output. Placing in output_format Section 1 guidance ensures the generated brief includes these boundary conditions.

---

## E011 — Moat-maintenance clause
**from_d_id:** D012
**from_idea_id:** I014
**target:** {"kind": "anchor", "before": "**Architectural authority:** the commission brief is the governing specification", "after": "**Why upfront specification for this context:**"}
**action:** insert-before
**text:** |
  **Moat-maintenance principle:** The precision gap between developer creative vocabulary and DSP implementation is simultaneously the primary engineering challenge and the primary commercial moat. The defensible element of this moat is not the developer's DSP ignorance — it is the developer's creative vocabulary scope (their specific creative language and sound identity, which a competitor cannot easily replicate). As the developer advances through the calibration schedule and gains DSP understanding, the moat does not erode — it transforms: from "AI bridges a wide gap" to "AI accelerates a developer with rich creative vocabulary." Moat-maintenance strategy: continuously expand creative vocabulary breadth ahead of DSP competence growth by investing in perceptual vocabulary development (new glossary entries, new sonic concepts) and in KB content that supports that vocabulary. Include this principle in the System Vision section.
**rationale:** Adds the moat-maintenance clause to the System Vision output_format guidance, implementing D012's gain. Clarifies that DSP competence growth does not erode the moat — reframing calibration advancement correctly.
**compare_notes:** Draft 1 (add to Calibration schedule constraint) scored lower — the moat-maintenance principle is a System Vision-level architectural concept, not a calibration schedule implementation detail. Placing it in System Vision guidance gives it the right architectural visibility.

---

## E012 — creative_phrase_tag on downstream artifacts
**from_d_id:** D013
**from_idea_id:** I015
**target:** {"kind": "anchor", "before": "**KB content structure for Dify ingestion:**", "after": "**Why this replaces bridges:**"}
**action:** insert-before
**text:** |
  **Creative phrase tagging for artifact traceability:** Every session-persistent artifact produced during development — DSP parameter sets, preset defaults, UI label mappings, marketing copy blocks — carries a `creative_phrase_tag` metadata field containing the exact Phase 0 creative phrase that seeded it. The tag is the verbatim developer phrase (no paraphrasing). Location per artifact type: JSON `metadata.creative_phrase_tag` for technical artifacts, YAML frontmatter `creative_phrase_tag` for markdown documents. When a creative phrase is superseded (creative vision change protocol), all artifacts bearing the matching `creative_phrase_tag` are enumerable for staleness review using a simple grep or metadata query — no manual tracking required. This makes creative intent the primary key of the artifact graph, not a secondary label. Specify this requirement in the KB Design section of the commission brief.
**rationale:** Adds creative phrase tagging to the KB Design output_format guidance, implementing D013's gain. Placed before the content structure section as it is an architectural requirement that applies to all artifact types, not just KB content.
**compare_notes:** Draft 1 (add to cross-domain translation constraints) scored lower — the tagging specification belongs in KB Design because it is a KB/artifact architecture requirement; the Cross-Domain Translation constraints section governs query behavior, not artifact metadata.

---

## E013 — Boundary transmission check at each domain transition
**from_d_id:** D014
**from_idea_id:** I016
**target:** {"kind": "anchor", "before": "**Calibration schedule:** each phase maps to a calibration level", "after": "**Developer curriculum (optional parallel track):**"}
**action:** insert-before
**text:** |
  **Boundary transmission checks:** At each of the five domain boundaries in the translation chain, the agent performs a boundary transmission check before crossing to the next domain. Check procedure: (a) implement the boundary output (DSP parameters, C++ code, UI labels, marketing copy) per RAG retrieval results, (b) present the output to the developer using perceptual descriptors appropriate to the boundary (e.g., at the sound-design→DSP boundary: "does this DSP parameter set sound directionally correct for your creative intent?"; at the DSP→C++ boundary: "does this implementation produce audio matching the DSP specification?"), (c) developer confirms: transmission passes (proceed to next domain) or fails (rework before crossing). Failed transmissions are logged to the session handoff document with the specific boundary and failure description. This replaces phase-completion exit criteria as the primary quality gate — the question is not "is this phase complete?" but "did this domain boundary preserve the creative intent?"
**rationale:** Adds boundary transmission checks to the Development Phases output_format guidance as an operational replacement for phase-completion exit criteria, implementing D014's gain.
**compare_notes:** Draft 1 (add as a constraint in Agent Workflow) scored lower — boundary transmission checks are phase-specific execution behavior specified in Development Phases guidance, not a cross-phase constraint; placing in Development Phases output_format guidance correctly scopes the behavior.

---

## E014 — Competitive moat maintenance in Business Strategy
**from_d_id:** D015
**from_idea_id:** I017
**target:** {"kind": "anchor", "before": "8. **Business Strategy**", "after": "9. **Licensing and Anti-Piracy**"}
**action:** replace
**text:** |
  8. **Business Strategy** — Market analysis (2026 VST landscape), marketing plan, pricing strategy (accessible to average consumer), marketplace selection and listing strategy (KVR Audio and others), revenue projections and break-even analysis. **Competitive moat maintenance:** The precision-gap moat is defensible because the developer's creative vocabulary is hard to replicate, not because their DSP quality is superior to expert-built alternatives. A competitor with equal AI tools and more DSP expertise can produce technically superior plugins but cannot replicate the developer's specific creative identity. Include in Business Strategy: (a) a competitive positioning statement that targets under-served niches where expert developers have not focused (creative/abstract effects, psybient, extreme modulation effects — not reverb, EQ, or compression where expert-built tools dominate), (b) a brand-asset strategy for creative vocabulary: product naming, marketing language, and developer persona that embeds the creative identity as an owned asset, (c) moat maintenance: expand creative vocabulary breadth ahead of competitor entry into the niche. **Required market research (KB content for Dify ingestion):** (a) top 20 VST plugins by sales on KVR Audio in the creative/abstract effects category, with price points, expert-built vs. indie-built flag, and feature summaries, (b) average consumer spending per VST plugin purchase — target: identify the accessible price range (currently estimated under $50, to be validated by data), (c) marketplace listing requirements for KVR Audio, Plugin Boutique, and at least 2 other marketplaces — listing format, review process, revenue share, (d) competitor feature comparison matrix for the 5 closest competitors in the creative effects space, including expert-built alternatives (e.g., Serum-class tools in adjacent niches), (e) 2026 VST marketplace trend report — growth rate, popular categories, emerging segments, AI-assisted development trends. This data grounds the creativity-gap thesis in evidence and provides concrete inputs for marketing strategy and pricing
**rationale:** Replaces Section 8 Business Strategy guidance with enhanced version that adds the competitive moat maintenance clause (D015) and the expert-built alternatives dimension (D016) to the required market research, while preserving all existing content.
**compare_notes:** Draft 1 (insert-after the existing section 8 text) scored lower — the competitive moat content and the "expert-built flag" in market research both need to be integrated into the section guidance, not appended as an afterthought; replace-with-integrated-text produces better structure.

---

## E015 — Reframe moat erosion as developer graduation
**from_d_id:** D017
**from_idea_id:** I019
**target:** {"kind": "anchor", "before": "These are behavioral indicators that the developer can self-assess, not abstract skill measurements.", "after": "If a milestone is not met, maintain the current confidence level"}
**action:** insert-after
**text:** |
  **Moat erosion framing (developer rationale):** As you advance through the calibration schedule and gain more DSP understanding, the AI's role shifts — not your competitive advantage. Your DSP ignorance is not the moat; your creative vocabulary is. As you move from AI-guided to Collaborative to Developer-led, the AI transitions from gap-bridging (translating what you can't yet specify technically) to acceleration (implementing what you now know how to specify but don't want to code). The moat is maintained not by staying uninformed about DSP but by continuously advancing the scope and specificity of your creative vocabulary. Treat every phase as an opportunity to expand your creative vocabulary breadth — new sonic concepts, new perceptual terms, new sound identities — faster than your DSP expertise grows. The calibration endpoint (Developer-led) is not "gap closed" — it is "AI can implement exactly what you specify, every time, first try."
**rationale:** Inserts developer rationale for the moat-erosion concern directly after the calibration milestone behavioral indicators, implementing D017's gain. Placed in the Development Phases output_format guidance where the calibration content lives.
**compare_notes:** Draft 1 (add to System Vision moat-maintenance clause E011) scored lower — the moat-erosion framing is specifically for the developer's understanding of the calibration schedule, not an architectural principle; it belongs in the Developer-facing rationale for the calibration section, not in System Vision.

---

## E016 — DSP parameters functional validation before DAW testing
**from_d_id:** D018
**from_idea_id:** I020
**target:** {"kind": "anchor", "before": "For DSP phases where competing algorithm options require domain expertise to evaluate, specify the AI/human boundary:", "after": "**Calibration schedule:** each phase maps to a calibration level"}
**action:** insert-before
**text:** |
  **DSP parameters functional validation (pre-implementation sanity check):** After RAG retrieval returns DSP parameters for a creative phrase and before committing to full plugin-architecture implementation: (a) the agent implements a minimal test patch (10–20 lines JUCE, no full plugin architecture — just the DSP processing chain for the specific module), (b) renders a 5-second audio test output, (c) developer evaluates by ear: "Does this test patch sound directionally correct for the creative intent?" (yes/no). If yes, proceed to full implementation. If no, rework the RAG query parameters or adjust DSP values before full implementation. This is a fast early-phase sanity check — catching semantic-similarity-not-functional-correctness failures at the cost of a minimal test patch rather than after full implementation. Specify this step in the Development Phases → Phase 4 (DSP Implementation) section.
**rationale:** Adds the DSP functional validation step to the Development Phases output_format guidance, implementing D018's gain. Placed before the calibration schedule section within the Development Phases guidance block.
**compare_notes:** Draft 1 (add as a constraint in the Knowledge Base section about RAG error handling) scored lower — this is a phase-execution step (the agent does something during Phase 4), not a KB design constraint; it belongs in Development Phases guidance.

---

## E017 — Bridgeable range signal
**from_d_id:** D019
**from_idea_id:** I021
**target:** {"kind": "anchor", "before": "- **RAG system unavailable:**", "after": "- **RAG returns conflicting information:**"}
**action:** insert-before
**text:** |
  - **Creative concept outside bridgeable range:** When a RAG query for a creative concept returns <0.40 confidence across all DSP-relevant documents AFTER the relevant KB domain has been populated (i.e., this is not a KB population gap but a genuine vocabulary gap), the concept is potentially outside the current bridgeable range for this KB and this creative vocabulary. Resolution path: (a) attempt targeted KB expansion: harvest more specific content covering this creative concept (academic papers, expert analysis, or perceptual descriptions of similar sounds); (b) re-query the RAG system; if confidence is now ≥0.40, proceed normally; (c) if still <0.40 after targeted expansion, flag as "developer-guided implementation required" — the developer provides explicit sound design references (audio examples, instrument recordings, perceptual descriptions) from which the agent implements without relying on RAG retrieval for this concept; (d) log the concept in a "KB gap log" in the session handoff document for future content prioritization. This is distinct from a temporary RAG gap (domain not yet populated) — the bridgeable range signal applies only after population is complete for the relevant domain.
**rationale:** Adds the bridgeable range signal as an edge case, implementing D019's gain. Placed before the RAG unavailability edge case as it is a similar type of "RAG cannot help" scenario.
**compare_notes:** Draft 1 (add as a DO clause in Knowledge Base constraints) scored lower — a concrete resolution path with multiple steps is better expressed as an edge case scenario, which allows the full procedure to be specified without compressing it into a constraint list.

---

## E018 — Additional scope boundaries in System Vision
**from_d_id:** D020
**from_idea_id:** [I022, I023, I024]
**target:** {"kind": "anchor", "before": "**System boundaries** (distinct from product scope exclusions):", "after": "Include the two explicit scope exclusions for the creativity-gap opportunity:"}
**action:** replace
**text:** |
  **System boundaries** (distinct from product scope exclusions): (1) the brief applies to solo AI-assisted development with asymmetric expertise (developer strong in high-level languages, weak in C++/DSP) — it does not extend to team-based development where the calibration schedule cannot be standardized across multiple developers or where the asymmetric-expertise assumption does not hold, (2) the quality priority ordering assumes budget constraints ≤$200/month — with more resources the ordering may shift, (3) the RAG-powered translation chain is architecturally one-directional (creative intent → artifact) — reverse traversal (market opportunity → creative intent) is outside scope because the mapping is many-to-one, (4) the brief's architectural authority depends on being treated as governing specification — treating it as advisory voids its architectural function, (5) equal-expertise systems (where both developer and AI have similar domain knowledge) make the calibration schedule and dual-vocabulary format unnecessary overhead — this brief is optimized for the asymmetric case and does not address the equal-expertise configuration, (6) the RAG system handles established and documented vocabulary — genuinely novel concepts with no literature coverage require human-guided development, (7) markets where workflow integration is the primary value driver (samplers, routing utilities, metering tools) are outside the quality priority ordering — DSP > Creative > Marketing applies to creative effects where sound quality is the purchase driver, not to workflow-utility tools where feature completeness is the driver. Include the two explicit scope exclusions for the creativity-gap opportunity:
**rationale:** Replaces the existing System Boundaries list with an enhanced version that adds three new boundaries (multi-developer teams, equal-expertise systems, workflow-priority markets) while preserving all six existing boundaries. Items (1)–(4) and (6) are preserved verbatim or with minor enhancement; items (5) and (7) are new.
**compare_notes:** Draft 1 (insert three new boundaries as separate items after the existing list) scored lower — the existing boundary (1) already partially addresses multi-team but doesn't state it explicitly; a clean replacement that integrates the new items reads better than an append.

---

## E019 — Qualify "best-sounding possible" with constraints
**from_d_id:** D021
**from_idea_id:** I025
**target:** {"kind": "anchor", "before": "Focus on producing the best-sounding VST synthesizers and effects possible.", "after": "Must support creating any type of audio plugin"}
**action:** replace
**text:** |
  Focus on producing the best-sounding VST synthesizers and effects achievable within the constraints of this system: (a) $100–200/month AI budget ceiling — DSP quality is bounded by the knowledge retrievable from the Dify RAG KB populated within this budget, (b) developer-expertise constraints — DSP quality depends on RAG KB content quality and calibration progress across phases, (c) solo development timeline — complexity is bounded by what one person can complete with AI assistance. "Best-sounding" is relative to these constraints, not absolute. Plugins produced by expert DSP teams with specialized tools and unlimited development time will exceed this system's DSP quality ceiling for any given algorithm type. Within these constraints, DSP quality is the highest-priority goal. Must support creating any type of audio plugin,
**rationale:** Replaces the aspirationally false claim with an accurate bounded version, implementing D021's gain. The replacement preserves the spirit (DSP quality is the highest priority) while qualifying it honestly.
**compare_notes:** Draft 1 (add parenthetical "(within system constraints)" after "possible") scored lower — a single parenthetical is too weak to resolve a claim that the report identifies as architecturally significant; the replacement makes the constraint explicit and actionable for all downstream phases.

---

## E020 — Gap-reference requirement for sections 6–11
**from_d_id:** D022
**from_idea_id:** I026
**target:** {"kind": "anchor", "before": "6. **Sound Design Curriculum**", "after": "7. **UI/UX Standards**"}
**action:** replace
**text:** |
  6. **Sound Design Curriculum** — Topic hierarchy (general theory → specialized areas → creative focuses), source types and quality tiers, coverage plan ensuring all sound types are achievable with psybient and related genres as creative seed topics. **Gap-management role:** Sound design curriculum quality directly bounds RAG retrieval quality at the sound-design→DSP boundary — the highest-impedance boundary in the translation chain. High-quality sound design content (Tier A sources) produces higher-confidence retrievals and reduces creative-to-DSP translation loss. Include an explicit statement of how each curriculum area closes the precision gap: e.g., "Psychedelic sound design content reduces the creative-to-DSP gap for multi-timbral layered effects by providing perceptual vocabulary mappings that the RAG system can use to generate specific DSP parameter recommendations." **KB content quality tiers for Dify ingestion:** Tier A (preferred for ingestion): peer-reviewed academic papers (AES, DAfx, IEEE), professional standards (AES convention papers, IEC audio standards), recognized expert instructional content (published authors, established audio engineering faculty). Tier B (acceptable with metadata note): professional forum posts from verified industry professionals, established tutorial series with demonstrated expertise, manufacturer documentation (JUCE docs, VST3 SDK docs). Tier C (ingested with low-confidence flag): community content, user-generated tutorials, blog posts. The RAG system's confidence scoring reflects these tiers naturally — Tier A sources produce higher-confidence retrieval results. Content without a verifiable source is not ingested
**rationale:** Replaces Section 6 guidance with an enhanced version that adds the gap-management role statement, implementing D022's gain for Section 6. The remaining sections (7–11) will receive similar treatment through composing in m-integrate.
**compare_notes:** Draft 1 (add gap-reference as a final sentence to each section) scored lower — a final sentence is easy to ignore; leading with **Gap-management role:** as a named sub-element gives it structural visibility equal to other named sub-elements in these sections.

---

## E021 — Gap-reference for sections 7–11
**from_d_id:** D022
**from_idea_id:** I026
**target:** {"kind": "anchor", "before": "7. **UI/UX Standards**", "after": "8. **Business Strategy**"}
**action:** replace
**text:** |
  7. **UI/UX Standards** — Research targets for modern audio plugin design, synthesizer control layout theory, design principles for clean modern interfaces, design system approach. **Gap-management role:** UI/UX standards close the C++→UI boundary gap by providing a vocabulary of control types and layout patterns that the RAG system can retrieve when translating DSP parameter implementations into UI control specifications. Include a statement of how UI design standards connect to the creative-to-DSP translation chain: "UI controls must reflect the creative intent phrases at their labels and value ranges — a control labeled 'Warmth' with range 0–1 mapped to cutoff frequency preserves the creative phrase through to the final user interaction layer."
**rationale:** Adds gap-management role to Section 7, same pattern as E020.
**compare_notes:** Draft 1 (add gap-reference as footer paragraph) scored lower — same reason as E020.

---

## E022 — RAG translation records in KB Design
**from_d_id:** D011
**from_idea_id:** I013
**target:** {"kind": "anchor", "before": "**RAG system configuration:**", "after": "**Perceptual Vocabulary Glossary (seed reference):**"}
**action:** insert-before
**text:** |
  **Structured translation records:** For each multi-step domain translation during development phases, the agent produces a structured translation record artifact rather than passing raw RAG retrieved text directly to the next step. Translation record schema: `{creative_phrase: "<verbatim phrase>", creative_phrase_tag: "<tag>", sound_design_finding: "<retrieved sound design theory>", dsp_parameter_recommendation: "<retrieved DSP params>", juce_implementation_hint: "<retrieved C++ pattern>", confidence_scores: {sound_design: 0.0, dsp: 0.0, juce: 0.0}, developer_verification: null}`. The agent fills in all fields from RAG retrieval results; the developer fills in `developer_verification` (yes/no + comment) during the boundary transmission check. Translation records are stored in the session handoff document (last 3 records) and serve as: (a) the audit trail for tracing creative intent loss at specific boundaries, (b) the staleness detector when creative phrases change (`creative_phrase_tag` enables grep/query), (c) the input to the standing-wave guard (comparing calibration adjustments at consecutive boundaries). Specify this schema and workflow in the KB Design section of the commission brief.
**rationale:** Adds the translation record specification to the KB Design output_format guidance, implementing D011's gain. Placed before RAG system configuration as it is a data model requirement that the configuration serves.
**compare_notes:** Draft 1 (add to Technical Architecture as an artifact schema) scored lower — translation records are produced by RAG retrieval and stored in the KB/session context; they belong in KB Design as a data model specification, not in Technical Architecture as a system artifact.

---

## E023 — Information-preservation protocol in Technical Architecture
**from_d_id:** D023
**from_idea_id:** I027
**target:** {"kind": "anchor", "before": "**Sound Identity Architecture Map:**", "after": "**Developer rationale:** each creative sound type forces"}
**action:** insert-before
**text:** |
  **Information-preservation protocol:** The forward-only translation chain produces irreversible information loss at each domain boundary — creative intent degraded at the sound-design→DSP boundary cannot be recovered downstream. To make information loss detectable: (a) the agent produces a structured translation record (schema specified in KB Design section) for each domain translation step; (b) the developer fills in the `developer_verification` field during the boundary transmission check — confirming whether the boundary output preserves the creative intent; (c) if `developer_verification` is "no" at any boundary, the agent logs the specific boundary failure, revises the retrieval query or implementation, and re-runs the boundary before proceeding; (d) a chain of translation records for a single creative phrase constitutes the information-preservation audit trail for that phrase across all five boundaries. This protocol makes the structural property of irreversible information loss actionable rather than a passive acceptance of degradation. Specify this protocol in the Technical Architecture section of the commission brief, alongside the Sound Identity Architecture Map.
**rationale:** Adds the information-preservation protocol to Technical Architecture output_format guidance, implementing D023's gain. Placed before the Sound Identity Architecture Map as an architectural principle that applies to all sound types.
**compare_notes:** Draft 1 (specify only as a Development Phase step) scored lower — information preservation is an architectural principle that applies across all phases and sound types; it belongs in Technical Architecture as a system-level requirement, not only as a phase execution step.

---

## E024 — RAG failure mode enumeration
**from_d_id:** D024
**from_idea_id:** I028
**target:** {"kind": "anchor", "before": "**RAG system configuration:**", "after": "**Structured translation records:**"}
**action:** insert-after
**text:** |
  **RAG failure mode taxonomy and contingencies:** The RAG system is the single point of knowledge retrieval for all domain translations. Four specific failure modes require distinct contingency paths: (a) **Retrieval noise** (cross-domain contamination — e.g., "warm" matches marketing copy in a DSP query): contingency = apply domain-context weighting metadata (see Agent Workflow constraints); check if top-3 results are from the query-context domain; if not, add `domain_context` and `implementation_target` fields to the query to improve specificity. (b) **Content gaps** (insufficient KB population for a domain — low confidence due to sparse ingestion): contingency = flag the gap, harvest targeted content into Dify for the domain, re-query; do not block development — proceed with agent's built-in knowledge while flagging results as "unverified by KB." (c) **Confidence calibration failure** (confidence scores do not predict correctness — high confidence, wrong DSP output): contingency = run Phase 1 confidence-correctness calibration table; recalibrate thresholds for the affected domain. (d) **OoD boundary failure** (novel terms not in KB vocabulary, returns <0.35 confidence): contingency = invoke developer-confirmed paraphrase protocol (OoD confidence floor clause in Cross-Domain Translation constraints). Specify this taxonomy in the KB Design section.
**rationale:** Adds the four-mode RAG failure taxonomy with specific contingencies to KB Design output_format guidance, implementing D024's gain. Placed after the translation records specification and before the RAG system configuration.
**compare_notes:** Draft 1 (add failure modes as an edge case) scored lower — the failure mode taxonomy is a KB Design architectural requirement (it governs how the KB system behaves), not an edge case; edge cases are for scenarios outside normal operation, while these failure modes are expected routine occurrences.

---

## E025 — DSP budget rationale as gap-bridging ROI
**from_d_id:** D025
**from_idea_id:** I029
**target:** {"kind": "anchor", "before": "**Per-phase budget allocation guideline:** KB population = 15%", "after": "These percentages are guidelines, not rigid allocations"}
**action:** replace
**text:** |
  **Per-phase budget allocation guideline (weighted by gap-bridging ROI):** KB population = 15% ($15–$30/month — Dify RAG ingestion and content curation; high gap-bridging ROI because KB quality bounds all translation chain quality), Specification + Planning = 15% ($15–$30/month — brainstorming + writing-plans), DSP Implementation = 25% ($25–$50/month — highest gap-bridging ROI: this is where creative intent must cross the highest-impedance boundary into functional C++ audio code; most AI-intensive phase), Integration + Testing = 15% ($15–$30/month), UI + Polish = 10% ($10–$20/month), Marketing + Licensing = 10% ($10–$20/month), Reserve = 10% ($10–$20/month — contingency for KB gaps, vocabulary development, unexpected debugging). Allocation principle: weight by gap-bridging ROI (boundary impedance), not just by AI query count. If a phase encounters unexpectedly high boundary impedance for its creative concepts, reallocation toward that phase is justified (see budget reallocation protocol). These percentages are guidelines, not rigid allocations — the developer adjusts based on actual phase demands.
**rationale:** Replaces the per-phase budget allocation guideline with an enhanced version that adds gap-bridging ROI rationale to each line, implementing D025's gain. All existing allocation percentages and dollar amounts are preserved.
**compare_notes:** Draft 1 (add a standalone "allocation principle" sentence after the existing guidelines) scored lower — integrating the ROI rationale inline with each phase's allocation is more actionable; a standalone principle sentence risks being ignored.

---

## E026 — Phase 0 exit criteria checklist
**from_d_id:** D026
**from_idea_id:** [I030, I041]
**target:** {"kind": "anchor", "before": "Phase 0: Use `brainstorming` + `juce-plugin-spec`", "after": "Phase 1: Use `writing-plans`"}
**action:** insert-before
**text:** |
  **Phase 0 exit criteria (commission brief is complete when ALL five are met):** (a) all 27 output verification checklist items pass, (b) the commission brief contains ≥3 specific, non-generic creative phrases (e.g., "shimmering crystalline pads" is specific; "nice synth sounds" is not) that return RAG confidence ≥0.60 in the Dify sound-design domain after KB has been initialized with seed content — if a phrase fails this check, the Phase 0 task includes adding KB content for that phrase's domain area and re-querying, (c) the Sound Identity Architecture Map is populated for the specific plugin concept being specified (not generic psychedelic/ambient/pad/psybient templates — the architectural implications specific to this plugin's creative identity), (d) Phase 1 measurement tasks are specified: the boundary impedance map measurement task and the confidence-correctness calibration table task are assigned to Phase 1 in the Agent Workflow section, (e) the session handoff document (see Development Phases section) is initialized with the active creative phrase set, current spec_version (1.0 for initial brief), and Phase 0 calibration matrix state (all boundaries: AI-guided). Phase 0 is not complete until all five criteria are met. If (b) fails for a phrase, the Phase 0 task includes KB initialization for that domain.
**rationale:** Adds Phase 0 exit criteria to the Agent Workflow constraints section immediately before the skill assignment list, implementing D026's gain. Placed here so the exit criteria appear alongside the skill assignment for Phase 0.
**compare_notes:** Draft 1 (add exit criteria in the Development Phases output_format guidance) scored lower — Phase 0 exit criteria need to be visible in the constraints section where phase completion rules live; output_format guidance is for what the generated brief should contain, not how to know when a phase is done.

---

## E027 — Commercial viability warning for early phases
**from_d_id:** D027
**from_idea_id:** I031
**target:** {"kind": "anchor", "before": "**Commercial viability minimum requirements:**", "after": "**Shippable milestone** is defined with two explicit, distinct meanings"}
**action:** insert-before
**text:** |
  **Commercial viability timeline warning:** The precision-gap moat does not exist at project start. During Phases 0–3 (before DSP implementation and before substantial KB population), the system has not yet accumulated the RAG content or calibration history needed for the moat to form. Commercial viability claims during this period are premature — the technically functional milestone achievable in early phases is necessary but not sufficient for commercial viability. The moat forms progressively: it begins to form at Phase 4 (DSP Implementation) when the creative vocabulary is first translated into implemented sound, and strengthens through Phase 9 (DAW Testing) as the KB accumulates and calibration advances. The first milestone that could be described as commercially viable occurs at Phase 12 (Release), when all six commercial viability minimum requirements are simultaneously met. Include this warning in the System Vision section to set correct expectations for intermediate milestones.
**rationale:** Adds a commercial viability warning to the Development Phases output_format guidance, implementing D027's gain. Placed before the "Commercial viability minimum requirements" checklist as framing context.
**compare_notes:** Draft 1 (add as a constraint in the Budget/Resources section) scored lower — the commercial viability warning is a System Vision-level framing statement about the project's commercial trajectory, not a budget constraint; it belongs in the Development Phases guidance where milestone definitions live.

---

## E028 — Specification versioning
**from_d_id:** D028
**from_idea_id:** I032
**target:** {"kind": "anchor", "before": "**Architectural authority:** the commission brief is the governing specification for all downstream development phases", "after": "**Why upfront specification for this context:**"}
**action:** replace
**text:** |
  **Architectural authority and versioning:** the commission brief is the governing specification for all downstream development phases, not an advisory reference. When a downstream phase encounters a conflict with the brief, the brief takes precedence unless the developer explicitly overrides with a documented rationale. This authority is necessary because the developer works across multiple AI sessions — without a governing specification, each session risks producing inconsistent or contradictory work. **Specification versioning:** the commission brief carries a `spec_version` field (initial value: 1.0; increment the minor version for clarification updates, the major version for architectural changes). Every downstream artifact derived from the brief carries a `derived_from_spec_version` field. When the brief is updated, a new Claude Code session reads the current `spec_version` and identifies any artifacts with `derived_from_spec_version < current_version` as potentially stale — these are reviewed during session initialization using the change-impact enumeration protocol. The versioning system does not require formal migration documentation — staleness detection is the goal, not automated migration.
**rationale:** Replaces the "Architectural authority" paragraph with an enhanced version that adds the specification versioning mechanism, implementing D028's gain. All existing content is preserved and the versioning material is integrated naturally.
**compare_notes:** Draft 1 (add versioning as a separate paragraph after architectural authority) scored lower — integrated replacement reads more naturally; the versioning and authority concepts are logically connected (authority depends on version coherence) and flow better together.

---

## E029 — Phase-specific technically functional milestones
**from_d_id:** D029
**from_idea_id:** I033
**target:** {"kind": "anchor", "before": "**KB population and RAG access:** all agent skills query the Dify RAG system", "after": "Specify AAX compilation and certification as a **final release phase milestone only**"}
**action:** insert-before
**text:** |
  **Phase-specific "technically functional" milestone definitions:** "Technically functional" means different things at different phases — do not use the generic "VST3 binary loads in DAW" definition for phases before a VST3 binary exists. Per-phase observable milestones: Phase 3 (Project Setup) = cmake build succeeds with zero warnings, plugin binary loads in REAPER plugin scanner, plugin appears in DAW plugin list, audio passthrough (silence in, silence out) works without crash. Phase 4 (DSP Implementation) = at least one DSP module processing audio with perceptually correct output per creative intent, no audio clicks at buffer boundaries, minimal test patch evaluation passes. Phase 5 (State Management) = APVTS save/load cycle produces identical sonic output, preset recall is stable across plugin restarts. Phase 6 (Integration) = all DSP modules processing audio simultaneously with no buffer overflows, full signal path from input to output audible. Phase 7 (GUI) = all controls visible and functional, no layout rendering errors, parameter changes produce audible results. Phase 8 (Validation) = all unit tests pass, no memory leaks detected, all DSP modules covered by tests. For each phase, the developer milestone is: "Can you verify this by listening, looking at the plugin interface, or reading the test output without needing to read C++ code?" If yes, the milestone is developer-verifiable.
**rationale:** Adds phase-specific technically functional milestone definitions to the Development Phases output_format guidance, implementing D029's gain. Each milestone is observable by a developer with C++ beginner knowledge.
**compare_notes:** Draft 1 (define milestones as a table in an appendix) scored lower — a table in an appendix is harder to reference during phase execution; inline in the Development Phases guidance is more actionable.

---

## E030 — Session handoff document template
**from_d_id:** D030
**from_idea_id:** I034
**target:** {"kind": "anchor", "before": "**Developer curriculum (optional parallel track):**", "after": "**Calibration milestone indicators:**"}
**action:** insert-after
**text:** |
  **Session handoff document template:** A structured markdown file (filename: `session-handoff.md`) maintained at the project root and updated at the end of each development session. A new Claude Code session reads this file FIRST before reading any other project document. Sections: (1) `## Current Phase and Status` — phase name, completion percentage, last action taken, next action planned; (2) `## Active Creative Phrases` — table of current creative phrases with `creative_phrase_tags` and whether each has passed the RAG coherence gate (confidence ≥0.60); (3) `## Last 3 Translation Records` — the three most recent structured translation records (schema from KB Design section) with developer_verification filled in; (4) `## Open Boundary Transmission Failures` — list of boundary transmission checks that failed since last successful cross, with description of failure and rework status; (5) `## Calibration Matrix State` — current calibration level per boundary (from Development Phases calibration section); (6) `## Artifact Inventory` — list of built artifacts with `derived_from_spec_version` and `creative_phrase_tag`; (7) `## Open Questions / Blockers` — items requiring developer decision before next session can proceed. The handoff document is the project's "RAM state" — the commission brief is the "ROM." A new session that reads only the handoff document and the current-phase output_format guidance can proceed without reading the full brief.
**rationale:** Adds the session handoff document template to the Development Phases output_format guidance, implementing D030's gain. Placed after the calibration milestone indicators as it references calibration state.
**compare_notes:** Draft 1 (add as a constraint in Agent Workflow) scored lower — the session handoff document is a project-level artifact specification, not a single-agent behavior constraint; it belongs in Development Phases guidance where project artifacts are specified.

---

## E031 — RAG query construction template
**from_d_id:** D031
**from_idea_id:** I035
**target:** {"kind": "anchor", "before": "**RAG system configuration:**", "after": "**RAG failure mode taxonomy and contingencies:**"}
**action:** insert-after
**text:** |
  **RAG query construction template:** All agent-issued RAG queries during development phases use the following template to maximize retrieval relevance and minimize cross-domain contamination: `{creative_phrase} + [phase_context] + [domain_context] + [implementation_target]`. Fields: (a) `creative_phrase` — the verbatim developer phrase as captured in Phase 0 (no AI paraphrasing); (b) `phase_context` — the current development phase and step (e.g., "Phase 4 DSP implementation, implementing oscillator module"); (c) `domain_context` — the primary translation domain for this step (values: `sound_design`, `dsp`, `juce`, `ui_ux`, `marketing`, `licensing`, `cpp_reference`); (d) `implementation_target` — the specific artifact being created (optional but improves retrieval specificity; e.g., "JUCE 8 SmoothedValue cutoff automation"). Example full query: `"warm pad" + "Phase 4 DSP implementation" + "domain_context: dsp" + "JUCE 8 SmoothedValue cutoff automation"`. The agent appends the `domain_context` value as a Dify metadata filter or relevance weight parameter as supported by the Dify API. Queries that omit the template fields rely on Dify's default hybrid retrieval without context weighting.
**rationale:** Adds the RAG query construction template to the KB Design output_format guidance, implementing D031's gain. Placed after the failure mode taxonomy as it is a complementary operational specification.
**compare_notes:** Draft 1 (add to examples section as an additional example) scored lower — examples illustrate; the query template is a mandatory specification that agents must follow; it belongs in the KB Design section as a normative requirement.

---

## E032 — Developer rationale template for non-DSP sections
**from_d_id:** D032
**from_idea_id:** I036
**target:** {"kind": "anchor", "before": "Domain-to-section mapping: Task domain 1 (VST Plugin Development)", "after": "Each section must contain specific, actionable content"}
**action:** insert-before
**text:** |
  **Developer rationale template for non-DSP sections:** Sections 6–11 (Sound Design Curriculum, UI/UX Standards, Business Strategy, Licensing and Anti-Piracy, Success Criteria, Decision Reference Card) require a developer rationale block, but perceptual descriptors for DSP output (used in DSP sections) are not applicable. Use the following template for developer rationale in non-DSP sections: "**What to look for:** [3 bullet points of what the human can verify without technical expertise — presence of specific named systems, pricing in a specific range, named marketplace requirements]. **Your job:** [the specific human decision required at this section — e.g., choose pricing, select marketplace, approve marketing tone, confirm creative vocabulary coverage]. **AI handles:** [what the agent will implement or recommend without human input — e.g., anti-piracy system configuration, marketplace listing format, licensing server setup]." Example for Section 9 (Licensing): "What to look for: Does the recommended system have a free or low-cost tier for solo developers? Does it support KVR Audio marketplace requirements? Is there a trial mode option? Your job: approve the recommended system or specify an alternative. AI handles: API integration details, activation server architecture, license enforcement implementation."
**rationale:** Adds a developer rationale template for non-DSP sections to the output_format guidance, implementing D032's gain. Placed before the domain-to-section mapping as it applies to the sections that follow.
**compare_notes:** Draft 1 (add as a subsection under the dual-reader structure paragraph) scored lower — the dual-reader paragraph is generic; a named template with a concrete example is more actionable for the generating agent.

---

## E033 — Partially machine-verifiable verification checklist
**from_d_id:** D033
**from_idea_id:** I037
**target:** {"kind": "anchor", "before": "Before finalizing the commission brief, verify:", "after": "- Every item from the technical stack"}
**action:** replace
**text:** |
  Before finalizing the commission brief, verify. Items marked [STRUCTURAL] can be checked by pattern-matching the output text; items marked [JUDGMENT] require developer or agent content evaluation:
  [STRUCTURAL] Every item from the technical stack
**rationale:** Initiates the partially machine-verifiable checklist by replacing the plain "verify:" header with one that distinguishes structural checks (pattern-matchable) from judgment checks (content evaluation). The actual checklist items are not replaced here (they remain as-is); only the header changes to introduce the classification scheme. The remaining checklist items are appended with [STRUCTURAL] or [JUDGMENT] tags in the integration step where all checklist changes are composed together.
**compare_notes:** Draft 1 (add classification tags to every item individually as separate E entries) scored lower — modifying 27 individual items would create 27 conflicting E entries; replacing the header and adding a single meta-note about the classification scheme is more efficient, with the actual tagging done in compositing.

---

## E034 — Boundary-level calibration indicators
**from_d_id:** D034
**from_idea_id:** I038
**target:** {"kind": "anchor", "before": "**Calibration schedule:** each phase maps to a calibration level", "after": "**Calibration milestone indicators:**"}
**action:** replace
**text:** |
  **Calibration schedule and boundary-level indicators:** each phase maps to a calibration level (AI-guided / Collaborative / Developer-led) with specific behavioral indicators and transition criteria — see Constraints section. Calibration operates at two granularities: (a) **Phase-level milestones** (as specified in Constraints: AI-guided Phases 0–2, Collaborative Phases 3–6, Developer-led Phases 7–9+), which measure global developer competence across all domains; (b) **Boundary-level indicators** (per the boundary-level calibration entries in Constraints), which track calibration independently at each of the five domain boundaries. A developer can simultaneously be Collaborative at the creative→sound-design boundary (they can identify preferred options by ear) and AI-guided at the DSP→C++ boundary (they rely fully on AI for code evaluation). Track boundary-level calibration state in the calibration matrix section of the session handoff document. **Calibration schedule:** each phase maps to a calibration level
**rationale:** Replaces the "Calibration schedule" sentence in the Development Phases output_format guidance with an enhanced version that introduces boundary-level calibration as a second granularity alongside phase-level milestones, implementing D034's gain.
**compare_notes:** Draft 1 (add full 3×5 matrix specification) scored lower — the full matrix is an optional advanced tracking tool; the key insight (calibration is boundary-specific) is implementable as boundary-level indicators without the matrix overhead; the matrix is noted as an advanced option in the developer rationale.

---

## E035 — Decision Reference Card with creative phrase set
**from_d_id:** D035
**from_idea_id:** I039
**target:** {"kind": "anchor", "before": "11. **Decision Reference Card**", "after": "Domain-to-section mapping:"}
**action:** replace
**text:** |
  11. **Decision Reference Card** — One-page summary for quick re-orientation across AI sessions. **Primary orientation artifacts (read first, before the full brief):** (a) active creative phrase set: table of current creative phrases with `creative_phrase_tags`, RAG coherence gate status (confidence ≥0.60: pass/fail), and boundary association (which domain boundary each phrase primarily anchors); (b) current `spec_version` (from Architectural Authority clause); (c) current calibration matrix state (per-boundary calibration level); then (d) architectural thesis (creative-to-DSP precision gap), (e) quality priority ordering (DSP > Creative > Marketing > Licensing > UI > C++), (f) calibration levels (AI-guided → Collaborative → Developer-led), (g) scope exclusions (visual-UI plugins; conventional saturated categories; multi-developer teams; workflow-utility tools), (h) translation chain direction (forward-only, RAG-powered), (i) KB system (Dify RAG — semantic retrieval replaces manual bridges), (j) brief authority (governing specification with documented-override provision), (k) creative intent preservation (use original phrase as RAG query anchor, no paraphrasing), (l) shippable milestone duality (technically functional vs commercially viable), (m) AAX gating (final release only), (n) gap detection (dynamic via RAG confidence scoring, no tier gates), (o) moat-maintenance (creative vocabulary breadth is the defensible asset). A new Claude Code session reads the Decision Reference Card first and can orient to the project's current state without reading the full brief.
**rationale:** Replaces Section 11 guidance with an enhanced version that adds the primary orientation artifacts (active creative phrase set, spec_version, calibration matrix) at the top, implementing D035's gain. All existing reference items (d)–(n) are preserved; (o) is added.
**compare_notes:** Draft 1 (add creative phrase set as an appendix item at the end of Section 11) scored lower — placing at the end defeats the purpose; the orientation artifacts must be first in the card for a new session to use them efficiently.

---

## E036 — Budget-tracking protocol
**from_d_id:** D036
**from_idea_id:** I040
**target:** {"kind": "anchor", "before": "**DSP build-vs-buy decision procedure:**", "after": "**Technical:**"}
**action:** insert-before
**text:** |
  **Budget-tracking protocol:** At the end of each development session (or at session start when beginning a new phase), log the following to the session handoff document: (a) estimated AI query count for this session and rough cost estimate (Claude Code plan pricing: ~$0.001–0.01 per query at $100/month plan; use this as a rough tracker, not an exact figure), (b) cumulative phase spending vs. guideline allocation (e.g., "Phase 4: $18 actual vs. $25 guideline → 72% used, 2 sessions remaining"), (c) if cumulative project spending is on track to exceed the phase ceiling before phase completion, trigger the budget reallocation protocol (see Budget reallocation protocol above) before the next session begins. Human responsibility: review the budget log at each session start and confirm whether to continue with the current allocation or trigger reallocation. This converts budget guidelines from static percentages into actively monitored commitments without requiring external accounting tools.
**rationale:** Adds the budget-tracking protocol to the Developer Profile and Constraints section immediately before the DSP build-vs-buy procedure (both are budget-related practical protocols), implementing D036's gain.
**compare_notes:** Draft 1 (add to the session handoff document template as a section) scored lower — the handoff document is where the tracking *log* lives; the tracking *protocol* (what to measure and when) is a budget constraint, not a document template item.

---

## E037 — Recurring creative vocabulary development task
**from_d_id:** D037
**from_idea_id:** I042
**target:** {"kind": "anchor", "before": "**Developer curriculum (optional parallel track):**", "after": "**Session handoff document template:**"}
**action:** insert-after
**text:** |
  **Recurring creative vocabulary development (optional parallel task):** Once per phase (recommended: 1 session per 2–3 development sessions), the developer and agent invest one session in expanding the creative vocabulary glossary beyond the 5 seed entries. Process: (a) identify 2–3 new sonic concepts relevant to the current plugin's creative direction (e.g., while implementing Phase 4 DSP modules, notice that a new sonic character needs a vocabulary entry), (b) validate each candidate concept: issue a RAG query using the candidate term; if confidence ≥0.60 for sound design content, the RAG system already covers it — add it to the glossary with DSP, UI, and marketing domain mappings from the retrieval results; if confidence <0.35, add it to the KB population queue for future content harvest, (c) re-ingest the updated glossary into the Dify KB as an updated reference document. Allocated from the Reserve budget (10%). Does not gate phase progression — it runs in parallel with the current phase. The session handoff document tracks the current glossary size and the last vocabulary expansion session date.
**rationale:** Adds the recurring creative vocabulary development task to the Development Phases output_format guidance, implementing D037's gain (as recurring optional task, not sub-phase). Placed after the session handoff template as it references the handoff document for tracking.
**compare_notes:** Draft 1 (make vocabulary development a mandatory Phase 0 and Phase 1 task only) scored lower — vocabulary development is most valuable when it keeps pace with creative discoveries during DSP and GUI phases; limiting it to early phases misses the moat-maintenance opportunity in later phases.

---

## E038 — Creative vocabulary as Phase 0 primary deliverable
**from_d_id:** D038
**from_idea_id:** I043
**target:** {"kind": "anchor", "before": "Derive phase structure from the developer profile and technical constraints defined above.", "after": "**KB population and RAG access:**"}
**action:** insert-after
**text:** |
  **Phase 0 primary deliverable — active creative vocabulary set:** The commission brief is the Phase 0 deliverable container; the *active creative vocabulary set* — the specific creative phrases from System Vision Section 1 with their `creative_phrase_tags` — is the Phase 0 primary deliverable payload. All other Phase 0 sections (technical stack, KB design, budget) are stable reference content that rarely changes after initial specification. The creative vocabulary is what changes, evolves, and anchors all downstream RAG queries and artifact tagging. Invest the majority of Phase 0 creative energy in: (a) articulating the plugin's specific sonic identity in precise, evocative language (not generic terms like "interesting sounds" but specific like "resonant droning overtone fields with slow spectral shift"), (b) validating each phrase against the RAG system (coherence gate from Phase 0 exit criteria), (c) populating the session handoff document's Active Creative Phrases table. Developers who treat Phase 0 as "filling out the template" rather than "discovering the creative vocabulary" will find their RAG retrieval quality consistently low across all downstream phases — the precision of Phase 0 phrases is a multiplier on all subsequent phase quality.
**rationale:** Adds the Phase 0 primary deliverable reframing to the Development Phases output_format guidance, implementing D038's gain. Placed immediately after the "Derive phase structure" sentence that introduces the section.
**compare_notes:** Draft 1 (add as a sub-bullet under Phase 0 in the phase skill assignment list) scored lower — a sub-bullet is too small for this framing; the creative vocabulary reframing is a significant architectural insight that needs paragraph-level visibility, not bullet-level.

---

## Dropped

*(none — all 38 D entries produced at least one passing draft)*

---

## Summary

D entries processed: 38
E entries produced: 38 (note: D009 has two E entries — E009 as KB constraint + captured in E008 as Phase 1 task; D014/E013 as competitive Business Strategy, D016/E014 combined in same E entry as replacement)
Double-fail count: 0
