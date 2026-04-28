# Node D — Accepted Ideas

Evaluated: 43 | Accepted: 38 | Rejected: 0 | Merged during filter: 3 pairs

---

## D001 — Adaptive calibration thickness by boundary
**from_idea_id:** I001
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Replaces uniform phase-level calibration indicators with boundary-specific impedance indicators, matching calibration depth to actual information-loss risk at each domain boundary.
**target_section:** Development Phases / Calibration schedule

## D002 — Confidence-delta diagnostic protocol
**from_idea_id:** I002
**verdict:** ACCEPT-CONDITIONAL
**utility:** MEDIUM-HIGH
**gain:** Converts confidence-score monitoring from passive threshold filtering to active impedance diagnostics, enabling targeted calibration resource allocation.
**target_section:** Knowledge Base Design / RAG configuration
**condition:** Implement as agent-side logging per query chain (log confidence per boundary, compute delta in session context). Do not require Dify infrastructure changes — the delta computation is a simple arithmetic step the agent performs manually.

## D003 — OoD confidence floor with paraphrase fallback
**from_idea_id:** I003
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Resolves the TC-1 collision between no-paraphrase constraint and OoD handling by defining a concrete confidence floor threshold and a developer-confirmed paraphrase protocol.
**target_section:** Constraints / Cross-Domain Translation

## D004 — Standing-wave guard across consecutive boundaries
**from_idea_id:** I004
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Adds a cross-boundary coherence check that detects opposing calibration adjustments at consecutive boundaries — a failure mode invisible to single-boundary monitoring.
**target_section:** Technical Architecture / Translation pipeline

## D005 — Change-impact enumeration before creative pivot
**from_idea_id:** I006
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Prevents untracked artifact staleness when creative vision changes by enumerating all `creative_phrase_tag`-bearing artifacts before re-derivation, classified as must-re-derive / should-review / unaffected.
**target_section:** Edge Cases / creative vision changes

## D006 — Domain-context metadata on RAG queries
**from_idea_id:** I007
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Reduces cross-domain retrieval noise by tagging every agent-issued RAG query with a `domain_context` field, enabling Dify to weight results from the query-origin domain higher without gating retrieval.
**target_section:** Knowledge Base Design / RAG configuration

## D007 — Budget reallocation protocol
**from_idea_id:** I008
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Provides a concrete self-service reallocation rule (25% transfer cap between adjacent phases) when a phase overruns, preventing both rigid allocation and undefined flexibility.
**target_section:** Developer Profile and Constraints / Budget

## D008 — Phase 1 RAG measurement tasks and framing
**from_idea_id:** [I009, I012]
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Extends Phase 1 (Performance Audit) to include two concrete RAG measurement tasks: (a) boundary confidence-score baseline (5–10 queries, log per-boundary delta → impedance map), (b) RAG fidelity baseline named as an explicit Phase 1 deliverable with exit criteria. Provides the pre-calibration baseline for all adaptive calibration decisions.
**target_section:** Development Phases / Phase 1

## D009 — RAG confidence-correctness correlation step
**from_idea_id:** I010
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Empirically calibrates RAG confidence thresholds to actual DSP functional correctness, replacing the uncalibrated assumption (confidence ≥0.60 = usable result) with a project-specific threshold grounded in test-patch listening results.
**target_section:** Development Phases / Phase 1 (also: Knowledge Base Design / RAG configuration)

## D010 — Qualify market opportunity with boundary conditions
**from_idea_id:** I011
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Makes the market opportunity claim precise and falsifiable by documenting the three conditions under which gap-as-moat holds (mid-range gap width, accumulated RAG content, creative vocabulary irreducibility).
**target_section:** System Vision

## D011 — RAG produces structured translation records
**from_idea_id:** I013
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Replaces raw RAG chunk delivery with structured translation record artifacts per domain translation step, making creative-intent loss detectable at each boundary and enabling change-impact enumeration.
**target_section:** Knowledge Base Design

## D012 — Moat-maintenance clause in System Vision
**from_idea_id:** I014
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Establishes creative vocabulary breadth (not DSP ignorance) as the defensible moat element, framing moat-maintenance as a positive investment strategy rather than a dependency on developer incompetence.
**target_section:** System Vision

## D013 — creative_phrase_tag on all downstream artifacts
**from_idea_id:** I015
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Makes all downstream artifacts addressable by the creative phrase that seeded them, enabling staleness detection when phrases change and change-impact enumeration on creative pivots.
**target_section:** Knowledge Base Design / cross-session state

## D014 — Boundary transmission check at each domain transition
**from_idea_id:** I016
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Reframes quality gates from phase-completion checklists to boundary-enforcement mechanisms, requiring developer verification that creative intent transmitted correctly before crossing each domain boundary.
**target_section:** Development Phases / Quality gates

## D015 — Competitive moat maintenance in Business Strategy
**from_idea_id:** I017
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Adds a competitive moat maintenance clause to Business Strategy — identifies creative vocabulary irreducibility as the defensible element and creative-vocabulary brand positioning as the maintenance strategy.
**target_section:** Business Strategy

## D016 — Expert-built alternatives as competitive positioning
**from_idea_id:** I018
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Grounds the creativity-gap opportunity in a specific market positioning that avoids direct competition with expert-built tools (Serum-class) by targeting under-served niches where experts have not focused.
**target_section:** Business Strategy

## D017 — Reframe moat erosion as developer graduation
**from_idea_id:** I019
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Removes the implicit tension in the calibration schedule (advancing developer = eroding moat) by reframing DSP competence growth as shift from gap-bridging to AI-acceleration rather than moat erosion.
**target_section:** Development Phases / Calibration schedule

## D018 — DSP parameters functional validation before DAW testing
**from_idea_id:** I020
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Catches semantic-similarity-not-functional-correctness failures early (minimal test patch + ear check) before the cost of full DSP implementation is committed.
**target_section:** Development Phases / DSP validation

## D019 — Bridgeable range signal
**from_idea_id:** I021
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Provides a concrete signal (confidence <0.40 after KB population = potentially outside bridgeable range) for when a creative concept needs developer-guided development rather than RAG translation.
**target_section:** Knowledge Base Design / System Vision

## D020 — Additional scope boundaries in System Vision
**from_idea_id:** [I022, I023, I024]
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** Adds three explicit scope boundaries to System Vision — multi-developer teams (calibration cannot be standardized), workflow-priority markets (quality ordering fails), and equal-expertise systems (dual-reader overhead unnecessary). Prevents misapplication of the playbook.
**target_section:** System Vision

## D021 — Qualify "best-sounding possible" with constraints
**from_idea_id:** I025
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Replaces a false absolute claim with an accurate bounded claim, strengthening the commission brief's architectural authority (it can now be held accountable to its actual capability ceiling).
**target_section:** System Vision / task

## D022 — Gap-reference requirement for sections 6–11
**from_idea_id:** I026
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Makes the organizing principle (creative-to-DSP precision gap) visible throughout the full commission brief, not just Section 1, by requiring each section's output_format guidance to include a gap-management statement.
**target_section:** output_format (Sections 6–11)

## D023 — Information-preservation protocol at domain boundaries
**from_idea_id:** I027
**verdict:** ACCEPT-CONDITIONAL
**utility:** HIGH
**gain:** Makes creative-intent information loss detectable at each domain boundary through transmission record artifacts with developer-fillable verification fields.
**target_section:** Technical Architecture
**condition:** The transmission record schema is specified in D011 (I013). D023 adds the Technical Architecture requirement and the developer's responsibility to fill in the `developer_verification` field. Accept D023 as the architectural requirement clause only; schema content comes from D011.

## D024 — RAG failure mode enumeration
**from_idea_id:** I028
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Replaces the generic "RAG may be unavailable" edge case with a four-mode failure taxonomy (retrieval noise, content gaps, confidence calibration failure, OoD boundary failure), each with a concrete contingency path.
**target_section:** Knowledge Base Design

## D025 — DSP budget rationale as gap-bridging ROI
**from_idea_id:** I029
**verdict:** ACCEPT
**utility:** LOW-MEDIUM
**gain:** Corrects the DSP phase budget rationale from "most AI-intensive" to "highest gap-bridging ROI / highest boundary impedance," establishing a principled basis for future reallocation decisions.
**target_section:** Developer Profile and Constraints / Budget

## D026 — Phase 0 exit criteria checklist
**from_idea_id:** [I030, I041]
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Replaces the implied "commission brief is written = Phase 0 done" with five concrete exit criteria: structural completeness (27-item checklist), creative coherence (≥3 phrases at ≥0.60 RAG confidence), Sound Identity Architecture Map populated, Phase 1 tasks specified, session handoff document initialized.
**target_section:** Development Phases / Phase 0

## D027 — Commercial viability warning for early phases
**from_idea_id:** I031
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Adds an explicit note that the precision-gap moat does not exist in early phases (time→0 boundary condition), preventing premature commercial viability claims.
**target_section:** System Vision / Development Phases

## D028 — Specification versioning
**from_idea_id:** I032
**verdict:** ACCEPT
**utility:** MEDIUM-HIGH
**gain:** Adds a lightweight `spec_version` field to the commission brief and `derived_from_spec_version` to artifacts, enabling staleness detection when the brief is enhanced (e.g., by this epiphany-analysis pipeline).
**target_section:** System Vision

## D029 — Phase-specific "technically functional" milestone definitions
**from_idea_id:** I033
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Provides concrete, observable milestone definitions per phase that a C++ beginner can verify without deep technical expertise, replacing the abstract "VST3 binary loads in DAW" definition that doesn't apply to pre-DSP phases.
**target_section:** Development Phases (all phases)

## D030 — Session handoff document template
**from_idea_id:** I034
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Provides a structured session-state artifact (current phase, active creative phrases, last 3 translation records, open failures, calibration state, artifact inventory) that a new Claude Code session reads first to orient in ≤5 minutes.
**target_section:** Development Phases

## D031 — RAG query construction template
**from_idea_id:** I035
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Gives agents a concrete 4-field query construction template (`creative_phrase` + `phase_context` + `domain_context` + `implementation_target`), reducing retrieval noise and cross-domain contamination across all development phases.
**target_section:** Knowledge Base Design

## D032 — Developer rationale template for non-DSP sections
**from_idea_id:** I036
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Extends the dual-reader format to non-DSP sections (marketing, licensing, business strategy) with a concrete "What to look for / Your job / AI handles" template, making the developer's review role explicit for every section.
**target_section:** output_format (Sections 6–11)

## D033 — Partially machine-verifiable verification checklist
**from_idea_id:** I037
**verdict:** ACCEPT-CONDITIONAL
**utility:** MEDIUM
**gain:** Adds objective checkable criteria to structural verification items, enabling agent-assisted checklist verification for pattern-matchable items while marking judgment-required items explicitly.
**target_section:** verification
**condition:** Convert only the structurally checkable items to yes/no patterns (approximately 15 of 27). Items requiring content judgment remain natural language, marked "developer judgment required." Do not attempt to convert all 27 — that would overengineer the checklist.

## D034 — Boundary-level calibration indicators
**from_idea_id:** I038
**verdict:** ACCEPT-CONDITIONAL
**utility:** HIGH
**gain:** Adds per-boundary calibration indicators to the calibration schedule, revealing that the developer can be at different calibration levels at different boundaries simultaneously.
**target_section:** Development Phases / Calibration schedule
**condition:** Accept as boundary-level calibration indicators that augment the existing phase-level milestones. The full 3×5 matrix is an optional advanced tracking tool noted in the developer rationale block. Primary implementation is the per-boundary indicator — not the matrix. This prevents overengineering while preserving the key insight.

## D035 — Decision Reference Card with creative phrase set
**from_idea_id:** I039
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Elevates the Decision Reference Card to include the active creative phrase set and spec_version as primary orientation artifacts, enabling agent cold-start in ≤5 minutes without reading the full brief.
**target_section:** output_format / Section 11

## D036 — Budget-tracking protocol across sessions
**from_idea_id:** I040
**verdict:** ACCEPT
**utility:** HIGH
**gain:** Adds a concrete budget-tracking mechanism (running total logged in session handoff document per session, compared to phase guideline, with reallocation trigger) that converts budget guidelines from static percentages to actively monitored commitments.
**target_section:** Developer Profile and Constraints / Budget

## D037 — Recurring creative vocabulary development task
**from_idea_id:** I042
**verdict:** ACCEPT-CONDITIONAL
**utility:** MEDIUM
**gain:** Makes moat-maintenance concrete by adding a recurring vocabulary development activity (one session per phase, expand glossary by 2–3 entries, validate via RAG, re-ingest).
**target_section:** Development Phases
**condition:** Frame as a recurring optional task per phase (allocated from Reserve budget), not a separate sub-phase or mandatory gate. The developer may skip a given phase's vocabulary session without blocking progression. Do not gate phase advancement on vocabulary expansion.

## D038 — Creative vocabulary as Phase 0 primary deliverable
**from_idea_id:** I043
**verdict:** ACCEPT
**utility:** MEDIUM
**gain:** Reframes Phase 0's primary deliverable from "commission brief document" to "active creative vocabulary set" (the specific creative phrases), helping the developer invest creative energy in the highest-value artifact.
**target_section:** Development Phases / Phase 0

---

## Rejected

*(none — all 43 evaluated ideas meet realistic-to-implement and utility criteria; 3 accepted conditionally with scoping conditions)*

---

## Merge consolidation sub-pass

**Merge 1:** D008 = [I009, I012] — both target Phase 1 RAG measurement; I012 provides the framing ("RAG Fidelity Baseline" as named deliverable); I009 provides the specific measurement task. Verdict = ACCEPT (stronger of ACCEPT + ACCEPT = ACCEPT). Gain = concatenation.

**Merge 2:** D020 = [I022, I023, I024] — all target System Vision scope boundaries; each is a distinct boundary (multi-team, workflow-priority, equal-expertise); one combined D entry is cleaner than three nearly identical low-utility entries. Verdict = ACCEPT (all three were ACCEPT). Gain = concatenation of three boundary statements.

**Merge 3:** D026 = [I030, I041] — both target Phase 0 exit criteria; I041 subsumes I030 (I041's criterion (b) covers I030's RAG coherence gate). Verdict = ACCEPT. Gain = I041's fuller 5-criteria checklist.

**Final count:** 38 D entries accepted.
