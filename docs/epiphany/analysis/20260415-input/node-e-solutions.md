# Node E — Solutions Store

---

## E01 — Established vocabulary as Tier 1 deliverable
**from_d_id:** D01
**from_idea_id:** I01
**target:** {"kind": "heading_path", "path": ["<constraints>", "**Knowledge Base:**"]}
**action:** insert-after
**text:**
- DO define the "established synthesis vocabulary" as a Tier 1 KB deliverable — a curated, versioned list of creative intent phrases with accepted bridge-entry translations. Phrases not on this list are classified as Tier 2 gaps until promoted. The list is the authoritative reference for Tier 1/Tier 2 classification decisions
**rationale:** Without an explicit vocabulary list, the Tier 1/Tier 2 boundary is undecidable. Making the list a Tier 1 deliverable ensures it is populated before development begins, resolving Theory Collision #1.
**compare_notes:** Draft 2 proposed embedding the list in the bridge system schema rather than as a standalone KB deliverable — rejected because the list must be accessible independently for classification decisions, not only through bridge traversal.

---

## E02 — Tier 2 → Tier 1 promotion protocol
**from_d_id:** D02
**from_idea_id:** I02
**target:** {"kind": "heading_path", "path": ["<edge_cases>", "- **KB gaps discovered during development:**"]}
**action:** replace
**text:**
- **KB gaps discovered during development:** KB harvesting operates in two tiers (see Constraints). When a gap is found mid-build, classify it as: Tier 1 omission (blocking — must fill before the current phase proceeds) or Tier 2 addition (non-blocking — fill and continue). Gap-filling must not require restructuring existing layers or bridge systems. **Tier 2 → Tier 1 Promotion:** when a Tier 2 vocabulary entry has been validated through at least 2 successful bridge traversals and 1 DAW listening test confirmation, it qualifies for Tier 1 promotion. Promotion triggers a downstream re-validation check of bridge entries that reference the promoted term. A promoted entry does not invalidate existing Tier 2 entries that depend on it — they remain valid until independently promoted
**rationale:** The original edge case specifies gap classification but not promotion. Adding the promotion protocol closes the Tier 2 feedback loop and enables KB growth without restructuring.
**compare_notes:** Draft 2 proposed automatic promotion after 3 traversals without requiring DAW confirmation — rejected because automated promotion without perceptual validation could elevate entries with wrong DSP translations.

---

## E03 — DSP build-vs-buy decision procedure
**from_d_id:** D03
**from_idea_id:** I03
**target:** {"kind": "heading_path", "path": ["<constraints>", "**Budget and Resources:**"]}
**action:** insert-after
**text:**
- **DSP build-vs-buy decision procedure:** When a paid library provides superior DSP audio quality and a free alternative provides adequate but lower quality: (a) cost ceiling per library = 10% of annual AI budget ($12–$24 per library at $100–$200/month), (b) quality-delta threshold = the paid library must produce perceptually distinguishable superior output in a blind A/B test by the developer, (c) approval = developer decision after A/B test, (d) fallback = build in-house using free library as starting point. This resolves the tension between DSP quality priority (C3) and free-library constraint (C2) by making the decision procedure concrete and developer-controlled
**rationale:** The build-vs-buy tension between C2 and C3 is acknowledged but unresolved. This procedure makes it actionable with concrete thresholds.
**compare_notes:** Draft 2 proposed a "DSP quality committee" review process — rejected because a solo developer does not need committee approval; the A/B test + cost ceiling is sufficient.

---

## E04 — Domain-level Tier 1 gating
**from_d_id:** D04
**from_idea_id:** I04
**target:** {"kind": "anchor", "before": "- **Agent workflow sequencing:** Gate `brainstorming` and `writing-plans` after Tier 1 KB completion.", "after": "correct them before development phases begin"}
**action:** replace
**text:**
- **Agent workflow sequencing:** Gate `brainstorming` and `writing-plans` after Tier 1 KB completion, applied per-domain: each domain's Tier 1 completion gates only the agent skills that depend on that domain (e.g., VST development Tier 1 gates `brainstorming` and `writing-plans` for DSP phases; marketing Tier 1 gates marketing-strategy phases). A domain-level dependency map (specifying which skills require which domains to be Tier 1 complete) replaces the global binary gate. If planning must precede domain-specific Tier 1 completion, include a mandatory "Tier 1 assumption audit" revision pass for that domain after KB population — audit every plan for embedded DSP or synthesis assumptions from the incomplete domain and correct them before dependent development phases begin
**rationale:** The global Tier 1 gate blocks all skills regardless of domain relevance. Domain-level gating preserves milestone momentum while respecting the gate's intent — you don't need marketing Tier 1 to start DSP development.
**compare_notes:** Draft 2 proposed removing the gate entirely and replacing it with "advisory warnings" — rejected because the gate's purpose (preventing embedded assumptions) is architecturally important; domain-level gating preserves this with finer granularity.

---

## E05 — Calibration schedule with qualitative confidence levels
**from_d_id:** D05
**from_idea_id:** I05
**target:** {"kind": "anchor", "before": "- **DSP decision opacity:** When a phase requires choosing", "after": "the developer does not understand"}
**action:** replace
**text:**
- **DSP decision opacity and calibration schedule:** When a phase requires choosing between competing DSP algorithms and the trade-offs require domain expertise to evaluate, the developer cannot make a substantive technical choice. Resolve this boundary: AI presents options using **perceptual descriptors** ("Option A: dense harmonic texture with slower transients; Option B: sharper attack, more metallic character"), and the developer selects by DAW listening test — never by evaluating technical parameters the developer does not understand. **Calibration schedule:** The developer's ability to evaluate DSP output improves across development phases, tracked by three qualitative confidence levels: (1) **AI-guided** (Phases 0–2): developer relies on AI recommendations, validates by ear when prompted; cannot distinguish DSP options without AI perceptual descriptors; (2) **Collaborative** (Phases 3–6): developer and AI co-evaluate options; developer can identify preferred option in A/B test 70%+ of the time; (3) **Developer-led** (Phases 7–9+): developer drives creative decisions, AI translates to implementation; developer specifies creative targets and confirms implementation matches intent 90%+ of the time. If a calibration milestone is not met, maintain the current confidence level and re-evaluate at the next phase transition
**rationale:** Replaces the abstract "developer selects by DAW listening test" with a concrete, phased calibration schedule. Addresses S6 Millikan's weakest-link finding by using behavioral indicators instead of hypothesized numerical ratios.
**compare_notes:** Draft 2 proposed fixed percentages (80/20, 50/50, 20/80) from the genius analysis — rejected because these are hypothesized without empirical validation (flagged by S7 Cargo-Cult check). Qualitative levels with behavioral indicators are verifiable.

---

## E06 — Scope exclusions apply to product categories
**from_d_id:** D06
**from_idea_id:** I06
**target:** {"kind": "anchor", "before": "- DO NOT apply the creativity-gap market opportunity framing to plugin types where UI quality is inseparable from core product value", "after": "include this as an explicit scope boundary"}
**action:** replace
**text:**
- DO NOT apply the creativity-gap market opportunity framing to plugin types where UI quality is inseparable from core product value (spectrum analyzers, oscilloscopes, metering and visual tools) — the DSP > UI quality priority ordering fails for this class; exclude it explicitly in the System Vision section. DO NOT apply the creativity-gap market opportunity framing to conventional saturated product categories (standalone reverb, compressor, equalizer plugins) — the uniqueness thesis does not hold where the market is already well-served; include this as an explicit scope boundary for product-category positioning only. **Clarification:** scope exclusions govern product-category positioning, not individual DSP module selection — a creative effect plugin (e.g., "psychedelic atmosphere generator") may legitimately include reverb, EQ, or compression as sub-components when they serve the creative effect. The exclusion means the product is not positioned as "a reverb plugin"; it is positioned as a creative effect that may use reverb as one element
**rationale:** Resolves Theory Collision #5 (scope exclusion: product categories vs DSP sub-components). A creative plugin may include a reverb module; the exclusion forbids positioning as a reverb product.
**compare_notes:** Draft 2 proposed listing specific allowed DSP sub-components (reverb, EQ, compression with caveats) — rejected because a whitelist of allowed modules is over-constraining and would need constant updating. The category-vs-module distinction is sufficient.

---

## E07 — Intermediate bridge validation checkpoints
**from_d_id:** D07
**from_idea_id:** I07
**target:** {"kind": "heading_path", "path": ["<edge_cases>", "- **Bridge entry produces incorrect DSP output:**"]}
**action:** insert-after
**text:**
- **Intermediate bridge validation:** After a creative intent phrase traverses a bridge node, the output is validated against the bridge entry's coverage criterion (component f) before proceeding to the next bridge node. Validation at each node uses: (a) structural completeness test (does the output conform to the required form?), applied automatically by the AI agent; (b) semantic validation test (does the output perceptually match the source phrase?), applied during developer review at designated review points (not at every node — review points are at phase transitions and DAW testing milestones). This catches errors at the point of introduction rather than waiting for end-of-chain DAW testing
**rationale:** Bridge errors can propagate invisibly through correct-but-misapplied translations. Adding structural validation at each node and semantic validation at review points catches errors early without adding excessive overhead.
**compare_notes:** Draft 2 proposed full DAW testing at each bridge node — rejected as excessive overhead. Structural validation at each node + semantic validation at review points balances error detection with development velocity.

---

## E08 — Context-resolution for ambiguous cache keys
**from_d_id:** D08
**from_idea_id:** I08
**target:** {"kind": "anchor", "before": "- DO require creative intent phrases to be preserved **verbatim**", "after": "forces re-derivation from scratch"}
**action:** replace
**text:**
- DO require creative intent phrases to be preserved **verbatim** throughout the KB bridge chain — the original phrase is the cache key; AI agents must not paraphrase it at any bridge node, as paraphrasing corrupts the key and forces re-derivation from scratch. **Context resolution:** the cache key is the full creative phrase, not individual terms — "warm pad" and "warm lead" are different cache keys. When a term is genuinely ambiguous in context (e.g., "bright" with no instrument qualifier), the bridge entry includes a context-resolution field listing disambiguation rules based on surrounding terms (e.g., "bright + pad → high-frequency emphasis; bright + lead → filter resonance emphasis"). This preserves verbatim integrity while handling real-world polysemy
**rationale:** The original constraint requires verbatim keys but doesn't address the synonymy/polysemy problem that real creative language creates. Context resolution based on the full phrase handles ambiguity without violating the verbatim principle.
**compare_notes:** Draft 2 proposed decomposing creative phrases into term-level cache keys with dependency graphs — rejected because term-level decomposition violates the verbatim principle (the phrase as a whole, not its parts, is the identity anchor).

---

## E09 — Elevate creative-to-DSP boundary to architectural thesis
**from_d_id:** D09
**from_idea_id:** I09
**target:** {"kind": "heading_path", "path": ["<output_format>", "1. **System Vision** —"]}
**action:** replace
**text:**
1. **System Vision** — Problem statement, market opportunity, value proposition, why AI-assisted solo development is viable for this market. **Architectural thesis:** the creative-to-DSP translation boundary is the system's defining constraint — the precision gap between creative intent and DSP implementation is the architectural organizing principle for the entire brief. Every subsystem (KB, bridge chain, calibration schedule, dual-reader format) exists to manage this precision gap. Each section should reference how it manages the gap, not just list its domain content independently. Establish that Phase 0 is the **base-case computation** for the entire system: the semantic quality of creative intent captured here is a multiplier on all downstream phase quality — a structural checklist (16/16 items checked) is a floor, not a quality signal; poor creative specificity at Phase 0 causes cache misses at every downstream phase. Include the two explicit scope exclusions for the creativity-gap opportunity: (a) visual-UI-as-product plugins are outside scope, (b) conventional saturated segments (reverb, EQ, compression) are outside the creativity-gap framing — and clarify that scope exclusions govern product-category positioning, not individual DSP module selection (see Constraints)
**rationale:** The precision gap is the system's binding constraint (6-stage convergence). Elevating it to the architectural thesis makes every section coherent — each section explains how it manages the gap, not just what it contains.
**compare_notes:** Draft 2 proposed adding a separate "Architectural Thesis" section before System Vision — rejected because the thesis should be embedded within System Vision, not separate. A separate section would duplicate content that belongs in the existing structure.

---

## E10 — Sound Identity Architecture Map
**from_d_id:** D10
**from_idea_id:** I10
**target:** {"kind": "heading_path", "path": ["<output_format>", "3. **Technical Architecture** —"]}
**action:** insert-after
**text:**

**Sound Identity Architecture Map** — The plugin's creative focus determines architectural requirements. For each creative focus area, specify the architectural constraint it imposes: (a) **Psychedelic:** independent parallel signal chains — psychedelic layering requires multiple simultaneous processing paths that can be independently modulated; architecture must support parallel DSP graphs with cross-feedback paths. (b) **Ambient:** slew limiting and noise floor management — ambient textures require sustained sounds with minimal artifacts; architecture must support slow parameter transitions (slew limiting on all modulation), noise gating at low amplitudes, and long release times without clicks. (c) **Pad:** polyphonic voice management with long release — pad sounds require many simultaneous voices with overlapping releases; architecture must support high voice counts, voice-stealing with crossfading, and memory-bounded reverb tails. (d) **Psybient:** drone stability with bounded memory — psybient requires sustained low-frequency content that can play indefinitely; architecture must prevent memory accumulation in delay lines, support infinite sustain without CPU creep, and maintain phase coherence in detuned oscillators. This map appears in both System Vision (as justification for the architectural thesis) and Technical Architecture (as concrete requirements). **Developer rationale:** each creative sound type forces a different architectural decision — psychedelic needs parallel paths, ambient needs smooth transitions, pad needs many voices, psybient needs infinite stability. These are not optional features; they are architectural requirements derived from the sound identity
**rationale:** The genius analysis identifies sound identity → architecture as a structural invariant (3-stage convergence). Node A lists creative focus areas but never derives architectural implications. The map makes "form follows sonic function" explicit.
**compare_notes:** Draft 2 proposed putting the map only in the Sound Design Curriculum section — rejected because the architectural implications belong in Technical Architecture, not just the sound design section.

---

## E11 — Justify forward-only bridge with many-to-one mapping
**from_d_id:** D11
**from_idea_id:** I11
**target:** {"kind": "anchor", "before": "- DO specify the bridge chain as **forward-direction only**:", "after": "acknowledge this boundary explicitly"}
**action:** replace
**text:**
- DO specify the bridge chain as **forward-direction only**: creative intent → sound design → DSP parameter → C++ implementation → UI control → marketing descriptor. Reverse traversal (market opportunity → creative intent) is outside the system's scope; acknowledge this boundary explicitly. **Justification:** reverse traversal is architecturally unsound because the creative-intent-to-DSP mapping is many-to-one — multiple creative intents can produce the same DSP output (e.g., "warm pad" and "smooth lead" may both use a low-pass filter at similar cutoff). Inferring creative intent from DSP parameters would produce ambiguous results. The forward-only constraint is a necessary consequence of the asymmetric mapping, not an arbitrary design choice
**rationale:** The forward-only constraint is stated but not justified. Providing the many-to-one mapping argument makes the architectural decision defensible and prevents future challenges.
**compare_notes:** Draft 2 proposed adding "bidirectional for verification only" — rejected as contradicting the forward-only principle (see D27 rejection). The justification approach preserves the principle while explaining it.

---

## E12 — Extend perceptual vocabulary across all bridge interfaces
**from_d_id:** D12
**from_idea_id:** I12
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Perceptual vocabulary as bridge lingua franca:** the perceptual vocabulary ("warm," "bright," "lush," "deep," "sharp," "smooth") serves as the common interface language at every bridge node in the six-node chain: creative intent (perceptual) → sound design (perceptual) → DSP parameter (structural, with perceptual label) → C++ implementation (structural, with perceptual comment) → UI control (perceptual label on knob/slider) → marketing descriptor (perceptual). Each bridge entry preserves the perceptual label alongside the structural translation. **Caveat (S6 Millikan assessment):** perceptual vocabulary may not fully cover legal/commercial/architectural domains. The Tier 1 KB construction phase must include a vocabulary coverage evaluation (see below) — test whether a single enriched perceptual vocabulary can serve all seven domains before adding a structural vocabulary stratum
**rationale:** Perceptual vocabulary is the natural language of the bridge chain. Currently used only for DSP decisions, extending it across all nodes creates a unified interface. The Millikan caveat prevents over-commitment.
**compare_notes:** Draft 2 proposed defining a complete perceptual vocabulary taxonomy upfront — rejected as premature before the Tier 1 evaluation. The specification should require the evaluation, not predetermine its outcome.

---

## E13 — Declare brief's architectural authority
**from_d_id:** D13
**from_idea_id:** I13
**target:** {"kind": "heading_path", "path": ["<output_format>", "1. **System Vision** —"]}
**action:** insert-after
**text:**
**Architectural authority:** the commission brief is the governing specification for all downstream development phases, not an advisory reference. When a downstream phase encounters a conflict with the brief, the brief takes precedence unless the developer explicitly overrides with a documented rationale. This authority is necessary because the developer works across multiple AI sessions — without a governing specification, each session risks producing inconsistent or contradictory work. The brief's architectural authority depends on it being treated as the definitive source, not one input among many
**rationale:** The genius analysis concludes the brief must have architectural authority. Without explicit declaration, downstream AI sessions may treat the brief as advisory, leading to divergence across sessions.
**compare_notes:** Draft 2 proposed a "version control" system for the brief with explicit override tracking — rejected as over-engineering for a solo developer. A simple authority declaration with documented-override provision is sufficient.

---

## E14 — Creative pivot protocol
**from_d_id:** D14
**from_idea_id:** I14
**target:** {"kind": "heading_path", "path": ["<edge_cases>"]}
**action:** append
**text:**
- **Creative vision changes mid-development:** When the developer's creative vision shifts significantly: (1) document the pivot as a commission brief addendum, (2) identify affected bridge entries (those whose cache keys no longer match the evolved creative intent), (3) create new bridge entries for the evolved concepts using cache key extension (see bridge/vocabulary constraints), (4) mark affected old entries as "superseded" (not deleted — they remain valid for their original context), (5) re-validate downstream implementations against the new entries. This allows creative evolution without corrupting the bridge system or violating the forward-only constraint
**rationale:** The strongest objection to the forward-only constraint is that creative vision changes. The pivot protocol resolves this by treating creative evolution as extension (new keys) rather than mutation (changed keys).
**compare_notes:** Draft 2 proposed invalidating all bridge entries and restarting the chain — rejected as excessive. Marking old entries as superseded preserves the forward-only chain while accommodating evolution.

---

## E15 — System boundaries section
**from_d_id:** D15
**from_idea_id:** I15
**target:** {"kind": "heading_path", "path": ["<output_format>", "1. **System Vision** —"]}
**action:** insert-after
**text:**
**System boundaries** (distinct from product scope exclusions): (1) the brief applies to solo AI-assisted development with asymmetric expertise — it does not extend to team-based development where the calibration schedule cannot be standardized, (2) the quality priority ordering assumes budget constraints ≤$200/month — with more resources the ordering may shift, (3) the forward-only bridge is architecturally required (many-to-one mapping makes reverse traversal unsound) — this is a system boundary, not a design limitation, (4) the brief's architectural authority depends on being treated as governing specification — treating it as advisory voids its architectural function, (5) equal-expertise systems (where both developer and AI have similar domain knowledge) make the calibration schedule and dual-vocabulary unnecessary overhead
**rationale:** The genius analysis identifies 5 specific boundaries where the architecture breaks. These are distinct from product scope exclusions (which govern product categories) — they govern the specification's own applicability.
**compare_notes:** Draft 2 proposed integrating system boundaries into each affected section — rejected because scattered boundaries are harder to reference. A consolidated list in System Vision is more accessible.

---

## E16 — Cache key mutation vs extension distinction
**from_d_id:** D16
**from_idea_id:** I16
**target:** {"kind": "anchor", "before": "- DO require creative intent phrases to be preserved **verbatim**", "after": "forces re-derivation from scratch"}
**action:** insert-after
**text:**
- **Cache key mutation vs. extension:** (a) **Cache key mutation** (forbidden) = changing the DSP translation of an existing creative phrase while keeping the same key. This corrupts all downstream implementations without warning and is prohibited. (b) **Cache key extension** (permitted) = adding a new creative phrase that builds on an existing one (e.g., "warm ambient pad with granular texture" extends "warm ambient pad"). The new phrase creates a new bridge entry; the original key's translation remains unchanged. Extended keys reference the original key, inherit its base translation, and add delta translations. This distinction allows creative vocabulary to evolve without violating the verbatim preservation constraint
**rationale:** The strongest objection to forward-only is creative intent evolution. The mutation/extension distinction resolves this by allowing vocabulary growth without corrupting existing translations.
**compare_notes:** Draft 2 proposed versioning cache keys (key_v1, key_v2) — rejected because versioning implies mutation is permitted under version control, which undermines the verbatim principle. Extension (new keys) is cleaner.

---

## E17 — Evaluate single vs dual vocabulary during Tier 1
**from_d_id:** D17
**from_idea_id:** I17
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Vocabulary coverage evaluation (Tier 1 task):** Before committing to a dual vocabulary (perceptual + structural strata), the Tier 1 KB construction phase must evaluate whether a single enriched perceptual vocabulary can serve all seven domains. Evaluation criteria: for each domain, can at least 80% of required terms be expressed in perceptual language? Domains likely to fail: licensing (e.g., "license key validation" has no perceptual equivalent), C++ reference (e.g., "RAII" is purely structural). If a domain fails, add a structural stratum only for that domain — not globally. This prevents over-engineering while acknowledging the S6 Millikan finding that perceptual vocabulary has asymmetric support
**rationale:** The dual-vocabulary hypothesis has asymmetric support. Committing without evaluation adds complexity that may be unnecessary. The evaluation grounds the decision in evidence.
**compare_notes:** Draft 2 proposed committing to dual vocabulary upfront with a "simplification review" after Tier 1 — rejected because it's harder to remove an established dual-vocabulary system than to add one where evaluation shows it's needed.

---

## E18 — Connect dual-reader format to precision-gap architecture
**from_d_id:** D18
**from_idea_id:** I18
**target:** {"kind": "anchor", "before": "Dual-reader structure: Within each section", "after": "These may be co-located within prose rather than split into formal sub-sections."}
**action:** replace
**text:**
Dual-reader structure: Within each section, distinguish between (a) **AI-executable specification** — precise, structured content an AI agent can act on directly (parameters, named systems, criteria, decision rules) — this speaks the **structural vocabulary** (machine-actionable, precise) and (b) **developer rationale** — plain-language explanation of what the section means and what the human must review or decide — this speaks the **perceptual vocabulary** (accessible, evaluable by ear). The two formats are not just a presentation convenience — they are the human-facing interface for managing the creative-to-DSP precision gap. The spec part tells the AI what to do; the rationale part tells the developer how to evaluate whether the AI did it correctly. For DSP sections specifically, the developer rationale must use perceptual descriptors interpretable by a Python/JS developer without reading the AI-executable spec (e.g., "creates warmth by reducing high-frequency energy" rather than "applies second-order low-pass filter at cutoff frequency 400Hz"). These may be co-located within prose rather than split into formal sub-sections
**rationale:** The dual-reader format is a precision-gap management mechanism, not just a formatting choice. Making this connection explicit ensures future brief authors understand WHY the format exists.
**compare_notes:** Draft 2 proposed renaming the two parts to "structural stratum" and "perceptual stratum" — rejected because "AI-executable specification" and "developer rationale" are the established terms from the task specification. Renaming adds terminology without adding clarity.

---

## E19 — Acknowledge agile counter-argument
**from_d_id:** D19
**from_idea_id:** I19
**target:** {"kind": "heading_path", "path": ["<output_format>", "1. **System Vision** —"]}
**action:** insert-after
**text:**
**Why upfront specification for this context:** acknowledge that successful projects (including many VST plugins) demonstrate architecture emerging from iterative code. In this specific context, upfront specification is warranted because: (a) **AI session continuity** — the developer works across multiple Claude Code sessions; without an architectural anchor, each session risks producing inconsistent work, (b) **forward-only bridge dependency** — the bridge system requires upfront vocabulary definition to function; building bridges retroactively after creative intent has been lost is impossible, (c) **developer skill gaps** — a C++ beginner needs more upfront guidance than an expert; architecture emerging from iteration works when the architect can evaluate each iteration's quality, which the developer cannot yet do for DSP code. These three justifications apply specifically to the AI-assisted solo developer context — they do not argue against agile in general
**rationale:** The agile counter-argument is a legitimate objection. Providing specific, contextual justifications strengthens the brief's design rationale.
**compare_notes:** Draft 2 proposed a "hybrid approach" combining upfront specification with iterative refinement — rejected because it muddies the architectural authority of the brief. The brief should be the governing spec; iteration happens within its boundaries.

---

## E20 — Tool resilience for AI ecosystem changes
**from_d_id:** D20
**from_idea_id:** I20
**target:** {"kind": "heading_path", "path": ["<output_format>", "3. **Technical Architecture** —"]}
**action:** insert-after
**text:**
**Tool resilience:** The KB system and bridge architecture are tool-agnostic — they are markdown files and structured data consumable by any AI coding assistant. Claude Code is the primary tool, but the commission brief and KB structure are designed to survive tool changes because: (a) all architectural content (bridge entries, vocabulary, phase specifications) is stored in format-agnostic markdown, (b) the brief's output format is consumable by any AI tool, not just Claude Code, (c) if AI pricing changes, the developer can adjust session frequency within the budget rather than redesigning the system, (d) the calibration schedule and bridge system are architectural patterns that work regardless of the specific AI implementation tool
**rationale:** AI ecosystem changes are a real risk. Documenting tool resilience reduces anxiety about vendor lock-in and provides practical fallback options.
**compare_notes:** Draft 2 proposed abstracting the AI tool behind an interface layer — rejected as over-engineering. The brief is already tool-agnostic by virtue of being markdown. Acknowledging this is sufficient.

---

## E21 — Bridge entry lifecycle with developer review workflow
**from_d_id:** D21
**from_idea_id:** [I21, I33]
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Bridge entry lifecycle and developer review workflow:** (a) **Creation:** new entries follow the 6-component schema (direction, source vocabulary, target vocabulary, transformation rule, example I/O pair, coverage criterion) and are validated against the coverage criterion before activation. (b) **Developer review:** AI presents the bridge entry with full schema; developer reads the example I/O pair and asks "Does the output perceptually match the input?" — if yes, approve; if no, request revision with specific feedback (e.g., "this sounds too bright, I expected warmer"). For structural vocabulary entries (licensing, C++), developer reviews logical consistency rather than perceptual match. Approval is recorded with timestamp. (c) **Activation:** reviewed entries are added to the active vocabulary (Tier 1 or Tier 2 per classification). (d) **Deprecation:** entries that produce consistently wrong perceptual results are marked deprecated with a reason, not deleted. (e) **Consistency check:** during Tier 1 completion, all bridge entries are cross-checked for contradictions (same source term mapping to different targets without context resolution). (f) **Supersession:** entries superseded by creative pivots are marked "superseded by [new key]" and retained for historical reference
**rationale:** Node A specifies bridge entry creation (6 components) and error correction (C18) but not ongoing maintenance or the developer's review workflow. The lifecycle specification closes this gap.
**compare_notes:** Draft 2 proposed an automated consistency checker that runs after each entry creation — rejected as over-engineering. The manual cross-check during Tier 1 completion is sufficient for a solo developer's KB size.

---

## E22 — Commercially viable minimum requirements
**from_d_id:** D22
**from_idea_id:** I22
**target:** {"kind": "anchor", "before": "DO design phases to produce shippable milestones", "after": "These two definitions must be explicitly distinguished"}
**action:** replace
**text:**
- DO design phases to produce shippable milestones — define "shippable" as **technically functional** (VST3 binary loads in DAW, produces correct audio output) for all intermediate phases; a **commercially viable** milestone (marketplace-distributable, with full bridge chain coverage, licensing system in place) is only achievable at final release. These two definitions must be explicitly distinguished: a technically functional plugin that lacks a marketing bridge or licensing system is not commercially shippable. **Commercial viability minimum requirements:** (a) functional VST3 binary that loads in major DAWs, (b) at least one marketplace listing (KVR Audio or equivalent) with product description, (c) licensing system in place (even minimal — e.g., serial number validation), (d) basic marketing assets (product description, 3+ audio demos, one visual asset), (e) primary bridge chain complete (creative intent → DSP → C++ → UI → marketing descriptor), (f) pricing defined and competitive within the accessible price range
**rationale:** "Commercially viable" was undefined beyond "full bridge chain + licensing system." The enumerated minimum requirements make it a concrete, verifiable checklist.
**compare_notes:** Draft 2 proposed a tiered commercial viability model (bronze/silver/gold) — rejected as over-complex for a solo developer's first release. A single minimum checklist is sufficient.

---

## E23 — Developer perceptual improvement indicators
**from_d_id:** D23
**from_idea_id:** I23
**target:** {"kind": "heading_path", "path": ["<output_format>", "5. **Development Phases** —"]}
**action:** insert-after
**text:**
**Calibration milestone indicators:** (a) **AI-guided level** (Phases 0–2): developer can identify which of two DSP options sounds "better" when presented with AI perceptual descriptors — success criterion: 50%+ consistent preference, (b) **Collaborative level** (Phases 3–6): developer can identify preferred DSP option in A/B test without descriptors — success criterion: 70%+ consistent preference, (c) **Developer-led level** (Phases 7–9+): developer can specify creative targets using perceptual vocabulary and confirm implementation matches intent — success criterion: 90%+ first-attempt approval rate. These are behavioral indicators that the developer can self-assess, not abstract skill measurements. If a milestone is not met, maintain the current confidence level and re-evaluate at the next phase transition
**rationale:** The calibration schedule assumes developer improvement but provides no measurable definition. Behavioral indicators are self-assessable and verifiable without external expertise.
**compare_notes:** Draft 2 proposed formal A/B testing protocols with statistical significance — rejected as over-rigorous for a solo developer's self-assessment. Simple consistency thresholds are sufficient.

---

## E24 — Minimum required bridge set
**from_d_id:** D24
**from_idea_id:** I24
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Minimum bridge set (9 bridges):** Primary chain (5, required for the end-to-end translation): (1) Creative Intent → Sound Design, (2) Sound Design → DSP Parameters, (3) DSP Parameters → C++ Implementation, (4) C++ Implementation → UI Control, (5) UI Control → Marketing Descriptor. Cross-domain bridges (4, required for non-adjacent domain pairs): (6) Sound Design → UI/UX (perceptual vocabulary to UI design language — "warm" maps to both DSP cutoff and UI knob color/label), (7) Marketing Strategy → Pricing (positioning to price point — creative/unique positioning justifies premium within accessible range), (8) DSP Quality → Marketing Descriptor (sonic capability to product claims — ensures marketing claims are supported by actual DSP output), (9) Sound Design → Marketing (creative vocabulary to market positioning — "psybient" as both a sound design category and a market niche). Additional bridges may be discovered during Tier 1 construction. Each bridge follows the 6-component schema (direction, source vocabulary, target vocabulary, transformation rule, example I/O pair, coverage criterion with structural + semantic validation)
**rationale:** "All relevant domain pairs" is vague. The 9-bridge minimum makes Tier 1 completion concrete and provides a verification checklist.
**compare_notes:** Draft 2 proposed 14 bridges (including C++ → Licensing and UI → Pricing) — rejected as over-specifying. The 9-bridge minimum covers the end-to-end chain and the most important cross-domain pairs; additional bridges can be added during Tier 1.

---

## E28 — Bridge coverage metric for Tier 1 completion
**from_d_id:** D28
**from_idea_id:** I28
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Tier 1 completion criteria:** Tier 1 is complete when: (a) all 9 minimum bridges have at least one entry per required domain pair, (b) the established vocabulary covers at least 80% of common creative intent phrases (defined by a reference list curated during Tier 1), (c) every bridge entry passes both structural completeness test and semantic/perceptual validation test, (d) cross-check reveals no contradictions between bridge entries, (e) each domain has baseline content: sound design (general synthesis + 5 creative focuses), JUCE/VST (framework basics + CMake setup), UI/UX (design system + control layout), marketing (market data + positioning), licensing (system recommendation + implementation approach). These criteria make Tier 1 completion a measurable, auditable state
**rationale:** Without measurable completion criteria, "Tier 1 complete" is a qualitative judgment. The criteria make it an auditable checklist.
**compare_notes:** Draft 2 proposed a scoring system (0-100) for Tier 1 completion — rejected as over-complex. Binary pass/fail per criterion is sufficient and matches the existing gate model.

---

## E29 — Developer onboarding curriculum
**from_d_id:** D29
**from_idea_id:** I29
**target:** {"kind": "heading_path", "path": ["<output_format>", "5. **Development Phases** —"]}
**action:** insert-after
**text:**
**Developer curriculum (optional parallel track):** A structured learning path that runs alongside development phases, designed to build the developer's ability to review AI-generated code: Phase 0–2 = C++ fundamentals (variables, functions, classes, RAII, smart pointers) via guided AI explanations of generated code; Phase 3–4 = JUCE framework concepts (AudioProcessor, APVTS, DSP module structure) via code review of AI-generated implementations; Phase 5–6 = DSP fundamentals (filters, oscillators, envelopes, their perceptual characteristics) via perceptual-to-structural vocabulary building; Phase 7–8 = Audio thread safety (lock-free patterns, sample accuracy, real-time constraints) via audit review; Phase 9+ = Independent creative direction. This is an optional parallel track — the developer may learn differently. The calibration schedule milestones (see §5) define when the developer's competence has reached each level, regardless of how they got there
**rationale:** The calibration schedule assumes developer improvement but provides no mechanism for it. The curriculum makes the improvement pathway explicit and optional.
**compare_notes:** Draft 2 proposed a mandatory curriculum with graded assessments — rejected because mandating learning pace is unrealistic for a solo developer. Optional with milestone-based assessment is more practical.

---

## E30 — Bridge validation cascade specification
**from_d_id:** D30
**from_idea_id:** I30
**target:** {"kind": "anchor", "before": "- **Bridge entry produces incorrect DSP output:**", "after": "no full-phase restart required."}
**action:** replace
**text:**
- **Bridge entry produces incorrect DSP output:** When a KB bridge entry causes DSP output that does not perceptually match the original creative phrase (detectable during DAW testing), it is a bridge entry defect, not a code defect. Correction path (bridge validation cascade): (1) detect error at any validation point (DAW testing, intermediate checkpoint, or developer review), (2) identify the bridge node where the semantic error was introduced (which bridge step produced the wrong translation), (3) update only that bridge entry with correct parameters, (4) re-run bridge traversal from that node forward for the affected cache key only, (5) re-validate downstream implementations that depended on the corrected translation, (6) if re-validation fails, escalate to developer for DAW listening test. The bridge system must be structured to support point corrections without cascading restructuring. Steps 4–6 are scoped to the affected cache key — not the entire bridge system and not a full-phase restart
**rationale:** The original edge case specifies "re-run downstream from that node only" but not what "re-run" means operationally. The cascade makes it concrete.
**compare_notes:** Draft 2 proposed automated re-generation of all downstream implementations — rejected because re-generation may introduce new errors. Re-validation with escalation to developer is safer.

---

## E31 — Quick reference card
**from_d_id:** D31
**from_idea_id:** I31
**target:** {"kind": "heading_path", "path": ["<output_format>"]}
**action:** append
**text:**
11. **Decision Reference Card** — One-page summary for quick re-orientation across AI sessions: (a) architectural thesis (creative-to-DSP precision gap), (b) quality priority ordering (DSP > Creative > Marketing > Licensing > UI > C++), (c) calibration levels (AI-guided → Collaborative → Developer-led), (d) scope exclusions (visual-UI plugins; conventional saturated categories), (e) bridge direction (forward-only, many-to-one justification), (f) Tier 1 gate (domain-level), (g) brief authority (governing specification with documented-override provision), (h) cache key rule (verbatim, mutation forbidden, extension permitted), (i) shippable milestone duality (technically functional vs commercially viable), (j) AAX gating (final release only)
**rationale:** Both the developer and AI agents benefit from a quick reference. The card enables orientation without re-reading the full brief.
**compare_notes:** Draft 2 proposed integrating the card into the System Vision section — rejected because the card is a reference tool, not part of the system vision. A standalone appendix is more accessible.

---

## E32 — Perceptual-to-technical glossary
**from_d_id:** D32
**from_idea_id:** I32
**target:** {"kind": "heading_path", "path": ["<output_format>", "4. **Knowledge Base Design** —"]}
**action:** insert-after
**text:**
**Perceptual Vocabulary Glossary (seed entries):** A consolidated reference mapping perceptual terms to technical equivalents across domains. Seed entries for Tier 1: "warm" → DSP: low-pass filter, cutoff 200–800Hz / C++: state variable filter with resonance / UI: knob labeled "Warmth" / Marketing: "Warm analog character"; "bright" → DSP: high-frequency emphasis, cutoff >2kHz / UI: knob labeled "Brightness" / Marketing: "Sparkling high-end clarity"; "deep" → DSP: sub-harmonic generation, emphasis below 200Hz / UI: knob labeled "Depth" / Marketing: "Deep sub-bass presence"; "lush" → DSP: detune spread 0.05–0.15 semitones, chorus rate 0.3–0.5Hz / UI: knob labeled "Lush" / Marketing: "Rich, enveloping texture"; "sharp" → DSP: fast transient, high resonance, filter emphasis 2–5kHz / UI: knob labeled "Sharpness" / Marketing: "Precise, cutting definition." The glossary serves as both a developer reference and a seed for bridge entries. Additional entries are populated during Tier 1 KB construction
**rationale:** The developer needs a consolidated reference for translating between perceptual and technical vocabularies. The glossary also seeds bridge entries with concrete examples.
**compare_notes:** Draft 2 proposed a full 50+ term glossary — rejected as premature. Seed entries establish the format; full population happens during Tier 1.

---

## E35 — Bridge traversal failure diagnostic guide
**from_d_id:** D35
**from_idea_id:** I35
**target:** {"kind": "heading_path", "path": ["<edge_cases>"]}
**action:** append
**text:**
- **Bridge traversal failure modes:** (1) "No matching entry" — developer sees: "Creative phrase 'X' has no bridge entry. Options: (a) check if a similar phrase exists in the established vocabulary, (b) create a new Tier 2 bridge entry, (c) refine the creative phrase to use established vocabulary." (2) "Ambiguous result" — developer sees: "Phrase 'X' matches multiple entries. Options: (a) add context to the phrase to disambiguate, (b) select the intended entry by reviewing example I/O pairs." (3) "Wrong perceptual output" — developer sees: "Bridge entry for 'X' produced output that does not match the creative intent. Options: (a) report bridge entry defect (triggers bridge validation cascade), (b) request re-derivation with corrected perceptual feedback." These failure modes complement the bridge error correction path and make failure recovery accessible to the developer without requiring AI expertise
**rationale:** The error correction path (C18/cascade) handles corrections but not the developer-facing diagnostic experience. The guide makes failure recovery actionable.
**compare_notes:** Draft 2 proposed automated failure classification using the bridge system's metadata — rejected because the developer needs human-readable guidance, not a metadata-driven classifier. Simple option lists are sufficient.

---

## E36 — Per-phase budget decomposition
**from_d_id:** D36
**from_idea_id:** I36
**target:** {"kind": "anchor", "before": "- DO NOT require significant capital investment beyond the AI subscription", "after": "is the only planned cost)"}
**action:** replace
**text:**
- DO NOT require significant capital investment beyond the AI subscription ($100–$200/month for Claude Code is the only planned cost). **Per-phase budget allocation guideline:** Tier 1 KB construction = 20% ($20–$40/month — heavy AI usage for research and harvesting), Specification + Planning = 15% ($15–$30/month — brainstorming + writing-plans), DSP Implementation = 25% ($25–$50/month — most AI-intensive phase for code generation), Integration + Testing = 15% ($15–$30/month), UI + Polish = 10% ($10–$20/month), Marketing + Licensing = 10% ($10–$20/month), Reserve = 5% ($5–$10/month — contingency for Tier 2 gaps and unexpected debugging). These percentages are guidelines, not rigid allocations — the developer adjusts based on actual phase demands. Phases not yet started consume no budget
**rationale:** The global budget has no allocation guidance. Per-phase percentages give the developer a practical spending framework.
**compare_notes:** Draft 2 proposed a detailed spreadsheet-style tracking system — rejected as over-engineering. Simple percentage guidelines are sufficient for a solo developer.

---

## E37 — KB content quality tiers
**from_d_id:** D37
**from_idea_id:** I37
**target:** {"kind": "heading_path", "path": ["<output_format>", "6. **Sound Design Curriculum** —"]}
**action:** insert-after
**text:**
**KB content quality tiers and acceptance criteria:** Tier A (required for Tier 1): peer-reviewed academic papers (AES, DAfx, IEEE), professional standards (AES convention papers, IEC audio standards), recognized expert instructional content (published authors, established audio engineering faculty). Tier B (acceptable for Tier 1 with cross-reference): professional forum posts from verified industry professionals, established tutorial series with demonstrated expertise, manufacturer documentation (JUCE docs, VST3 SDK docs). Tier C (Tier 2 only, requires validation before promotion): community content, user-generated tutorials, blog posts. Acceptance criteria: Tier 1 content must have at least one independent source confirming the claim, or be a direct JUCE/framework reference. Harvested content records its source tier and validation status. Content without a verifiable source is rejected
**rationale:** "Best possible quality sources" is stated but undefined. Quality tiers make the criterion concrete and provide acceptance/rejection criteria for harvested content.
**compare_notes:** Draft 2 proposed a 5-tier system (A through E) with formal review boards — rejected as over-complex. Three tiers with simple criteria are sufficient for a solo developer's KB.

---

## E38 — Map agent skills to all 13 development phases
**from_d_id:** D38
**from_idea_id:** I38
**target:** {"kind": "anchor", "before": "- DO map agent skills to phases: `brainstorming` for specifications", "after": "for execution"}
**action:** replace
**text:**
- DO map agent skills to phases: Phase 0 (Specification) = `brainstorming` + `juce-plugin-spec`; Phase 1 (Performance Audit) = none (manual baseline measurement); Phase 2 (Architecture) = `writing-plans`; Phase 3 (Project Setup) = `subagent-driven-development`; Phase 4 (DSP Implementation) = `juce-dsp-implementation`; Phase 5 (State Management) = `subagent-driven-development`; Phase 6 (Integration) = `subagent-driven-development`; Phase 7 (GUI) = `juce-ui-bridge`; Phase 8 (Validation) = `subagent-driven-development`; Phase 9 (DAW Testing) = `juce-daw-testing`; Phase 10 (Profiling) = `subagent-driven-development`; Phase 11 (Release Prep) = `subagent-driven-development`; Phase 12 (Release) = `subagent-driven-development`. Where no specific skill exists, default to `subagent-driven-development`. Pre-commit audits: `juce-audio-thread-audit` and `juce-smoothedvalue-audit`
**rationale:** Only 3 skills were mapped to phases. Mapping all 13 phases gives AI agents complete execution guidance.
**compare_notes:** Draft 2 proposed creating new custom skills for Phases 1, 5, 6, 8, 10–12 — rejected as over-engineering. The existing skill set (with defaults) covers all phases. New skills can be created later if specific needs emerge.

---

## E39 — Market research requirements
**from_d_id:** D39
**from_idea_id:** I39
**target:** {"kind": "heading_path", "path": ["<output_format>", "8. **Business Strategy** —"]}
**action:** insert-after
**text:**
**Required market research (Tier 1 KB content):** (a) top 20 VST plugins by sales on KVR Audio in the creative/abstract effects category, with price points and feature summaries, (b) average consumer spending per VST plugin purchase — target: identify the accessible price range (currently estimated under $50, to be validated by data), (c) marketplace listing requirements for KVR Audio, Plugin Boutique, and at least 2 other marketplaces — listing format, review process, revenue share, (d) competitor feature comparison matrix for the 5 closest competitors in the creative effects space, (e) 2026 VST marketplace trend report — growth rate, popular categories, emerging segments, AI-assisted development trends. This data grounds the creativity-gap thesis in evidence and provides concrete inputs for marketing strategy and pricing
**rationale:** The creativity-gap thesis is asserted without supporting data. Market research requirements make it an evidence-based claim.
**compare_notes:** Draft 2 proposed including the actual research data in the commission brief — rejected because the brief defines WHAT to research, not the research results themselves. Tier 1 KB construction performs the research.

---

## Dropped

(none — all D entries produced at least one passing draft)