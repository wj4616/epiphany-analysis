# Playbook Creator Playbook — Design Spec

**Date:** 2026-03-30
**Status:** Draft — pending user review
**Approach:** 15-phase playbook creator with KB construction, compilation blocks, and failure mode tracking
**Output:** Standalone JSON playbook, no platform dependencies

---

## Purpose

A meta-playbook that produces domain-specific playbooks. The creator defines a repeatable process for turning any domain (software development, content creation, product launches, operations, research) into a structured, gated, executable playbook JSON file.

The creator is domain-agnostic. Domain-specific knowledge enters through the research and KB construction phases, not through hardcoded assumptions in the structure.

---

## Functional Roles

Roles signal what mode of thinking a task requires. A single agent fills all roles sequentially. Every task has exactly one `[Role]` owner.

| Role | Responsibility |
|------|---------------|
| `[Coordinator]` | Phase gates, tracking, status updates, blocker escalation, decisions ledger and artifact manifest maintenance |
| `[Researcher]` | Domain research, best practices, SME knowledge, competitive analysis |
| `[Architect]` | Phase structure, task granularity, role design, dependency mapping, template design |
| `[Builder]` | Task titles/descriptions, JSON assembly, validation, implementation of fixes |
| `[Auditor]` | Quality review, scenario walkthroughs, gap analysis, stress testing, failure mode cataloging, contamination testing, final verification before handoff |
| `[Stakeholder]` | Purpose, scope, constraints, success criteria, business decisions, final approval |

Rules:
- Every task has exactly one `[Role]` owner
- Roles map to capabilities, not people — one agent fills all roles
- Domain-specific roles (legal, security, marketing) enter through the KB Construction phase as domain concerns, not as standing roles in the creator
- The output playbook defines its own roles appropriate to its domain

---

## Phase Structure (15 Phases, 0–14)

| # | Phase | Purpose | Key Change from Original |
|---|-------|---------|--------------------------|
| 0 | Commission & Scoping | Define purpose, users, scope, constraints, success criteria | `[Role]` syntax, no platform deps |
| 1 | Domain Research & Process Discovery | Research domain, best practices, competitors, SME knowledge, cross-cutting concerns | Output feeds KB Construction, not directly Architecture |
| 2 | Knowledge Base Construction | Convert flat research into structured, layered, queryable knowledge specification | **NEW** |
| 3 | Process Architecture | Define phase structure, task granularity standard, dependencies, gates, document tree | Consumes structured KB spec |
| 4 | Role Engineering | Define functional roles, handoffs, escalation paths for the output playbook | Functional `[Role]` model, simplified (no RACI matrix) |
| 5 | Task Engineering | Write all task titles, descriptions, gates with handoff blocks, cross-cutting enforcement | `[Role]` syntax, handoff blocks required on every gate |
| 6 | Output Configuration | Define workflow model, roles, usage instructions, session strategy, KB mapping, skill activation | **Reshaped** — replaces Mattermost templates |
| 7 | Metrics & KPI Definition | Define process, quality, and domain outcome metrics | Three metric categories required |
| 8 | JSON Assembly & Validation | Build JSON, validate syntax, structure, `[Role]` consistency, KB schema | Platform-agnostic, added KB validation |
| 9 | Quality Audit — Gap Analysis | Requirements, cross-cutting, phase gaps, contradictions, completability, detail level | Added KB coverage audit, mechanical checklist approach |
| 10 | Quality Audit — Stress Testing | Scenario walkthroughs: happy path, domain novice, blockers, variants, edge cases, Auditor verification of fixes | Added domain novice scenario, Auditor verification |
| 11 | Stakeholder Review & Iteration | Business alignment, roles review, feedback, final approval | Unchanged in purpose |
| 12 | Pilot Test | Run playbook on real low-stakes project, human observes and documents friction | Generic execution, no Mattermost |
| 13 | Documentation & Version Control | Changelog, archive research/audit docs, store JSON, quick-start guide | Unchanged in purpose |
| 14 | Continuous Improvement | Post-run review (manual, human-initiated), playbook updates | Reframed from time-based to event-based |

---

## Phase Transition Protocol

Every phase has two bookends: a **compilation block** (entry side) that assembles the agent before the phase runs, and a **handoff block** (exit side) that packages results for the next phase. Together they make session isolation work without losing continuity.

### Compilation Block

Every phase opens with a compilation block. This is the boot sequence — it tells the agent (or a fresh session) exactly how to assemble itself before any task runs.

```json
{
  "compilation": {
    "context_load": ["files to load before this phase starts"],
    "role_mindset": "string — dominant thinking mode for this phase",
    "objective": "string — one-line statement of what this phase must accomplish",
    "pre_check": ["verifiable conditions that must be true before starting — typically previous gate conditions"],
    "failure_modes_relevant": ["FM-IDs from failure_modes[] that apply to this phase"]
  }
}
```

Rules:
- `context_load` is derived from the previous phase's `next_phase_context` plus the two persistent files (decisions ledger and artifact manifest — see below)
- `role_mindset` signals how the agent should think, not which [Role] tags to use — a phase with Builder tasks might still need an Architect mindset if the work is structural
- `pre_check` conditions should be mechanically verifiable before any task starts — if they fail, do not proceed
- `failure_modes_relevant` lists which cataloged failure modes the agent should be aware of during this phase — these become active guardrails

### Handoff Block

Every phase gate includes a handoff block with four fields.

```json
{
  "title": "[Coordinator] — Phase gate: <conditions>",
  "gate_conditions": ["verifiable condition 1", "verifiable condition 2"],
  "blocker_examples": ["specific scenario that would block advancement"],
  "handoff": {
    "output_artifacts": ["files this phase produced"],
    "next_phase_context": ["minimum context the next phase needs to start"],
    "excluded_context": ["what was deliberately left out and why"],
    "skill": "prompting skill for next phase, or none"
  }
}
```

Rules:
- `next_phase_context` is the load list — it feeds the next phase's `context_load`. `decisions-ledger.md` and `artifact-manifest.md` are implicitly included in every `next_phase_context` — they are not listed per-handoff to avoid repetition.
- `excluded_context` documents WHY certain things are not in `next_phase_context` (helps future editors understand handoff design, not a runtime instruction to delete)
- `output_artifacts` lists every file the phase produced (used by audits to verify completeness)
- `skill` tells the agent what mode to enter for the next phase
- Every gate task implicitly includes: append key decisions to `decisions-ledger.md` and update `artifact-manifest.md` with all output files. These are not listed per-gate to avoid repetition — they are mandatory at every gate.
- Top-level `skill_activation` and `phase_kb_mapping` in the output JSON are summary views derived from gate handoffs during JSON Assembly — gates are authoritative, summaries are convenience

### Context Preservation

Two persistent files solve the problem of information loss across session boundaries. They are always loaded in every compilation block's `context_load`, never excluded.

1. **Decisions ledger** (`decisions-ledger.md`)
   - Append-only. Every phase gate appends: phase number, key decisions made, constraints discovered, rejected alternatives with brief rationale.
   - Maximum one paragraph per phase. Concise, not exhaustive.
   - Purpose: ensures no decision rationale is silently lost when underlying docs are excluded from handoff. A phase that needs to understand WHY a previous phase chose X can find it here without loading the full source documents.

2. **Artifact manifest** (`artifact-manifest.md`)
   - Running index of every file produced across all phases: file path, producing phase, status (active | superseded | archived), one-line summary.
   - Updated at every phase gate (part of the Coordinator's gate task).
   - Purpose: any phase can scan this to locate and pull archived material on demand. If a phase discovers it needs something not in its `context_load`, the manifest tells it exactly where to find it — no guessing, no re-deriving.

Rules:
- Both files are initialized in Phase 0 and updated at every gate
- Both files are always included in `context_load` — they are exempt from token optimization cuts
- The decisions ledger captures the WHY; the artifact manifest captures the WHERE
- Together they replace the need to carry full documents forward "just in case"

---

## Phase Details

### Phase 0: Commission & Scoping

**Purpose:** Define exactly what playbook is being built, for whom, and under what constraints. Nothing else starts until this is locked.

**Compilation:**
- context_load: [commission brief from user — the initial request describing what playbook to build]
- role_mindset: Stakeholder — this phase is about decisions, not research or building
- objective: Lock purpose, scope, constraints, and success criteria so every subsequent phase has a fixed target
- pre_check: [User has provided a commission brief or verbal description of the playbook they want]
- failure_modes_relevant: []

**Tasks:**

1. `[Coordinator] — Create project knowledge base folder`
   - Folder structure: README.md, research/, architecture/, drafts/, audits/, testing/, final/
   - Initialize `decisions-ledger.md` — empty, with header: "# Decisions Ledger — append at every phase gate"
   - Initialize `artifact-manifest.md` — empty, with header: "# Artifact Manifest — update at every phase gate" and columns: File | Phase | Status | Summary

2. `[Stakeholder] — Define the playbook's purpose and problem statement`
   - What process does this playbook automate/guide?
   - What problem does it solve? What goes wrong without it?
   - Is this new, an optimization of existing, or a variant?
   - One paragraph, no ambiguity.
   - Output: README.md (purpose statement and project overview)

3. `[Stakeholder] — Define target users and team composition`
   - Who will RUN this playbook?
   - Team size, roles, skill levels
   - What tools/access do they have?
   - The playbook's detail level is calibrated to the least experienced person who will use it.

4. `[Stakeholder] — Define scope boundaries`
   - IN SCOPE: what this playbook covers
   - OUT OF SCOPE: what it does NOT cover
   - ADJACENT: what it connects to but doesn't own
   - Scope creep is the #1 killer of playbook quality.
   - Output: scope.md

5. `[Stakeholder] — Define success criteria for the playbook itself`
   - Measurable criteria (e.g., "an unfamiliar user can execute it without clarifying questions")
   - Quality bar (e.g., "every task has an owner, description, and exit criteria")
   - Pilot test pass/fail criteria
   - Output: success-criteria.md

6. `[Stakeholder] — Define strategic constraints`
   - Technology constraints
   - Methodology constraints
   - Budget/resource constraints
   - Timeline constraints
   - Compliance/legal constraints
   - These become universal rules that every phase must respect.
   - Output: constraints.md

7. `[Stakeholder] — Identify related existing playbooks`
   - Playbooks covering overlapping territory
   - Upstream/downstream dependencies
   - Reference/inspiration sources
   - If replacing an existing playbook, that playbook becomes input for Phase 1 audit.

8. `[Coordinator] — Phase gate: Purpose defined, scope locked, constraints documented, success criteria measurable, knowledge base initialized`
   - Gate conditions:
     - Purpose statement is one paragraph with no ambiguity
     - Scope has explicit in/out/adjacent lists
     - Success criteria are measurable, not subjective
     - Strategic constraints documented
     - Project knowledge base folder created
     - decisions-ledger.md and artifact-manifest.md initialized
   - Blocker examples:
     - Purpose statement says "improve the process" without specifying which process or what improvement looks like — too vague, rewrite
     - Scope has no out-of-scope list — everything is implicitly in scope, which means scope creep is guaranteed
     - Success criteria say "playbook is good" — not measurable, define what "good" means quantitatively
   - Append to decisions-ledger.md: purpose decision, scope boundaries chosen, key constraints
   - Update artifact-manifest.md with all output files
   - Handoff:
     - output_artifacts: [README.md, scope.md, constraints.md, success-criteria.md, decisions-ledger.md, artifact-manifest.md]
     - next_phase_context: [README.md (full), scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
     - excluded_context: [Conversation about why scope decisions were made — resolved, captured in scope.md and decisions-ledger.md]
     - skill: none

### Phase 1: Domain Research & Process Discovery

**Purpose:** Understand the domain deeply enough to build a playbook for it. Research produces flat documents that Phase 2 will structure.

**Compilation:**
- context_load: [README.md, scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Researcher — gather, synthesize, document. Breadth over depth at this stage.
- objective: Produce comprehensive flat research documents covering domain processes, best practices, competitors, cross-cutting concerns, and platform specifics
- pre_check: [Purpose statement is unambiguous, scope has in/out/adjacent lists, constraints documented]
- failure_modes_relevant: []

**Tasks:**

1. `[Researcher] — Research the domain this playbook serves`
   - Standard processes in this industry/discipline
   - Common failure modes
   - Regulatory/compliance requirements
   - Standard tools and platforms
   - Terminology glossary
   - KPIs professionals track
   - Output: research/domain-analysis.md

2. `[Researcher] — Research industry best practices and standards`
   - Published standards (ISO, industry frameworks)
   - Expert recommendations (books, courses, authoritative sources)
   - Common process frameworks
   - Typical phase structures
   - Common pitfalls and how experts avoid them
   - Output: research/best-practices.md

3. `[Researcher] — Competitive playbook/template audit`
   - Existing playbooks, templates, SOPs, checklists covering similar territory
   - For each: what it covers, what it does well, what it misses, structure, detail level
   - Output: research/competitive-templates.md

4. `[Researcher] — Audit existing playbook(s) being replaced (if applicable)`
   - What the existing playbook does well (KEEP)
   - What it does poorly (FIX)
   - What it's missing (ADD)
   - What contradicts current strategy (REMOVE)
   - Output: audits/existing-playbook-audit.md

5. `[Stakeholder] — Gather process knowledge from subject matter experts`
   - The actual process (not documented — the real one)
   - Steps that aren't written down
   - Common blockers and resolutions
   - Decisions requiring judgment vs always-the-same
   - What they wish they'd known before their first time
   - Output: research/sme-interviews.md

6. `[Researcher] — Identify cross-cutting concerns`
   - Concerns that touch every phase, not just one
   - Technology/platform concerns affecting all phases
   - Compliance concerns applying to every task
   - Quality standards verified at every phase
   - Strategic constraints shaping every decision
   - Cross-cutting concerns must be woven throughout the playbook, not isolated in one phase.
   - Output: research/cross-cutting-concerns.md

7. `[Researcher] — Identify platform/environment-specific concerns`
   - If the playbook involves a specific platform or environment:
     - How does data/content get stored?
     - What are the safe modification methods?
     - What should NEVER be done? (anti-patterns)
     - What platform-specific tools exist?
     - What deployment/migration procedures apply?
   - Output: research/platform-concerns.md

8. `[Coordinator] — Synthesize research into requirements document`
   - Domain requirements (what the process must accomplish)
   - Quality requirements (standards and compliance)
   - Strategic requirements (business constraints from Phase 0)
   - Platform requirements (technical constraints)
   - Cross-cutting requirements
   - User requirements (what the target team needs to succeed)
   - Output: research/requirements.md

9. `[Coordinator] — Phase gate: Domain researched, requirements synthesized`
   - Gate conditions:
     - Domain analysis covers standard processes, failure modes, and terminology
     - Best practices documented with sources
     - Competitive templates audited with strengths/weaknesses
     - Cross-cutting concerns identified with justification
     - Requirements document synthesizes all research
   - Append to decisions-ledger.md: research scope decisions, key domain findings, cross-cutting concerns identified
   - Update artifact-manifest.md
   - Handoff:
     - output_artifacts: [research/domain-analysis.md, research/best-practices.md, research/competitive-templates.md, research/cross-cutting-concerns.md, research/platform-concerns.md, research/sme-interviews.md, research/requirements.md, audits/existing-playbook-audit.md (if applicable)]
     - next_phase_context: [research/requirements.md (full), research/cross-cutting-concerns.md (full), research/domain-analysis.md (full — needed for KB layer classification: reveals distinct knowledge types that synthesis collapses), scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
     - excluded_context: [SME interview transcripts — synthesized into requirements. Rejected competitive templates — audit captured what matters. best-practices.md and platform-concerns.md — key points captured in requirements.md.]
     - skill: none

### Phase 2: Knowledge Base Construction

**Purpose:** Convert flat research into a structured, queryable knowledge specification. This phase produces a KB ARCHITECTURE (blueprint), not the KB itself. The actual KB gets built when the output playbook is executed.

**Compilation:**
- context_load: [research/requirements.md, research/cross-cutting-concerns.md, research/domain-analysis.md, scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Architect — structuring knowledge, not gathering it. Classify, separate, define schemas.
- objective: Produce a KB architecture blueprint (layers, schemas, bridge, population strategy) that an agent can build from without ambiguity
- pre_check: [Domain analysis exists with distinct knowledge types identified, requirements synthesized, cross-cutting concerns documented]
- failure_modes_relevant: []

**Tasks:**

1. `[Architect] — Determine KB complexity level`
   - Decision question: "Do users of this playbook describe goals in language that differs from implementation language?"
     - YES → structured KB with bridge layer needed (e.g., audio: "warm" → filter cutoff 0.2-0.4)
     - NO → assess if domain has enough distinct knowledge types to warrant layers
   - Decision question: "Does this domain have enough distinct knowledge types to justify separate layers?"
     - YES → multi-layer KB (define layers)
     - NO → flat reference (single folder of markdown files with table of contents)
   - The minimal path is genuinely minimal: single reference layer, no bridge, no cross-references, flat file list. A folder of markdown with a TOC. That is the floor.
   - Output: KB complexity decision with rationale

2. `[Architect] — Define knowledge layers (if multi-layer)`
   - For each layer:
     - Name and domain boundary (what belongs here and what doesn't)
     - Authority score (0.0–1.0) — used for ranking when multiple layers cover related topics
     - Separation rule (why this can't merge with another layer)
     - Topics within the layer
   - Separation principle: knowledge that changes independently lives in separate layers
   - Output: kb-architecture.md (layer definition table)

3. `[Architect] — Define KB entry schema`
   - Exact JSON structure every entry must follow
   - Required fields:
     - id: {kb-name}_{topic}_{entry-name}
     - kb: which KB layer
     - topic: category within the KB
     - status: placeholder | harvested | curated | synced
     - version: semver
     - title: human-readable name
     - summary: one-line description
     - description: extended description
     - source: where the knowledge came from
     - concepts: array of {name, description, related[]}
     - code_blocks: array of {language, code, preserved} (if domain involves code)
     - tags: searchable keywords
     - related_topics: cross-KB navigation links
     - difficulty: beginner | intermediate | advanced
     - domain_relevance: 1-10 scale for the specific domain
   - Additional domain-specific fields defined here (e.g., audio_relevance, compliance_impact, risk_level)
   - Output: entry-schema.json

4. `[Architect] — Define bridge entry schema (if bridge needed)`
   - Only if the KB complexity decision identified a need for creative-to-technical translation
   - Bridge entry required fields:
     - category: which capability this maps to
     - parameters: array of parameter names this category controls
     - intent_mappings (domain-appropriate name): keyed by subjective descriptor
       - parameter: which parameter this affects
       - value_range: [min, max] normalized or with units
       - typical_default: starting point value
       - confidence: 0.0-1.0 (0.85+ verified, 0.65+ derived, 0.45+ speculative, below 0.45 flag for human review)
       - source: citation or derivation method
       - why: human-readable explanation of mapping rationale
       - anti_patterns: what NOT to do with this mapping
   - Bridge is hybrid: pre-seeded with common mappings from research, auto-generated when agent encounters unmapped concepts, feedback-enriched when user evaluation refines entries.
   - Combination layer: bridge entries can be composed. When two or more concepts are combined, the bridge cross-references their individual entries — identifies compatible parameter ranges, flags conflicts between anti_patterns, and proposes a combined recipe with its own confidence score (lower than the inputs, since the combination is untested). This is where domain-specific AI value emerges: systematic exploration of the combinatorial space of techniques that would be impractical manually.
   - Output: bridge-schema.json

5. `[Architect] — Define population strategy`
   - Placeholder seeding: what entries get created as placeholders during playbook setup (minimum one per topic per layer, with fallback content)
   - Harvesting: where content comes from (web sources, documentation, codebases, domain expert input), quality criteria for harvested content
   - Curation rules: what must happen before harvested content becomes curated (human review, automated validation, or both)
   - Sync rules: how curated entries get indexed (master index update, cross-reference generation, manifest update)
   - Versioning protocol: entries are append-only. Existing entries are never edited — they are superseded by new entries tagged with the relevant version (e.g., framework version, date). Old entries remain for reference or rollback. Queries filter by version tag so the correct generation of knowledge is loaded. This prevents KB corruption while allowing the knowledge to evolve.
   - Output: population-strategy.md

6. `[Architect] — Define index and directory structure`
   - Master index format (registry of all KBs, cross-topic mappings)
   - Per-KB index format (file listing by topic)
   - Per-KB manifest format (file status and sync history)
   - Cross-reference rules: when a topic appears in multiple KBs, results ranked by authority score
   - File naming conventions
   - Exact directory tree (an agent reading this can mkdir the structure and start populating)
   - Output: directory-structure.md

7. `[Coordinator] — Phase gate: KB architecture specified`
   - Gate conditions:
     - KB complexity decision documented with rationale
     - If multi-layer: every knowledge type from Phase 1 research assigned to exactly one layer
     - If multi-layer: no two layers cover the same knowledge type (separation rule verified)
     - Entry schema defined with all required fields, no placeholders
     - Bridge schema defined if domain requires translation layer
     - If bridge not needed: explicit justification documented
     - Population pipeline defined with source, quality criteria, and curation rules
     - Index and cross-reference structure defined with exact file formats
     - Directory tree specified down to file naming convention
   - Blocker examples:
     - Chose "structured multi-layer" for a simple ops checklist that has one knowledge type — over-engineered, switch to flat
     - Two layers both claim "implementation patterns" — overlapping boundaries, merge or redefine separation rules
     - Entry schema lists fields but no example entry — an agent can't verify the schema works without a concrete example, add one
   - Handoff:
     - output_artifacts: [kb-architecture.md, entry-schema.json, bridge-schema.json (if applicable), population-strategy.md, directory-structure.md]
     - next_phase_context: [kb-architecture.md (summary — layer names and boundaries only), research/requirements.md, research/cross-cutting-concerns.md, scope.md, constraints.md]
     - excluded_context: [Full entry schema detail — Architecture phase only needs layer names for phase-to-KB mapping. Population strategy — relevant during execution, not during process design. domain-analysis.md — knowledge types now captured in KB layer definitions.]
     - skill: none

### Phase 3: Process Architecture

**Purpose:** Design the output playbook's phase structure, task granularity, dependencies, and gates.

**Compilation:**
- context_load: [kb-architecture.md (summary — layer names and boundaries only), research/requirements.md, research/cross-cutting-concerns.md, scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Architect — structure, design, map. Fresh session benefits from reviewing with new eyes.
- objective: Design the output playbook's complete phase structure with task granularity, dependencies, gates, and document tree
- pre_check: [KB complexity decided, layers defined (if multi-layer), entry schema defined, population strategy defined]
- failure_modes_relevant: []

**Tasks:**

1. `[Architect] — Define phase structure for the output playbook`
   - Phase design principles:
     1. Each phase is a coherent block of work with a clear objective
     2. A phase's output is the next phase's input
     3. Every phase ends with a gate — explicit approval criteria before proceeding
     4. Early phases: understanding (audit, research). Middle phases: doing (build, create). Late phases: verifying and operating (QA, launch, monitor)
     5. Cross-cutting concerns woven into relevant phases, not isolated
   - Output: architecture/phase-structure.md

2. `[Architect] — Define task granularity standard`
   - A task represents ONE deliverable or ONE decision
   - If a task takes more than 4 hours or requires multiple skill sets, split it
   - If a task can't fail independently of the next task, merge them
   - Every task must be completable by the assigned role without ambiguity
   - Tasks requiring judgment must describe the decision criteria
   - Tasks producing a document must name the document path
   - Title-only vs description rules:
     - TITLE ONLY: simple, obvious actions
     - SHORT DESCRIPTION: tasks with specific requirements
     - DETAILED DESCRIPTION: tasks requiring methodology, syntax, decision criteria, or multi-step procedures
   - Output: architecture/task-granularity.md

3. `[Architect] — Map phase dependencies`
   - For each phase: hard dependencies (must complete first), soft dependencies (should complete), parallel opportunities (can run simultaneously)
   - Within phases: which tasks block others
   - Output: architecture/dependency-map.md

4. `[Architect] — Design phase gates`
   - For each phase:
     - What must be TRUE before the next phase starts
     - What deliverables must exist
     - What quality checks must pass
   - Gate conditions must be VERIFIABLE, not subjective:
     - Good: "All 15 city pages have 300+ words of unique localized content"
     - Bad: "City pages are done"
   - Each gate includes blocker_examples — specific scenarios that would prevent advancement
   - Output: architecture/phase-gates.md

5. `[Architect] — Identify early assessment/detection phases needed`
   - Does this domain have variable starting conditions that affect the workflow?
   - Does detection change the workflow for subsequent phases?
   - Does the playbook need an environment setup phase?
   - Does the playbook need a deconstruction/audit phase before implementation?
   - If yes, design these as dedicated phases positioned BEFORE any implementation work.
   - Output: integrated into architecture/phase-structure.md

6. `[Architect] — Define the deliverable document tree`
   - Which tasks produce documents
   - File path/naming conventions
   - Which documents feed into later tasks
   - Every task that says "Document in X" must have X defined here
   - Output: architecture/document-tree.md

7. `[Stakeholder] — Review and approve architecture`
   - Phase structure covers the full process
   - No gaps between phases
   - Phase gates are meaningful, not rubber stamps
   - Task granularity matches target user capability
   - Architecture satisfies all requirements from research/requirements.md

8. `[Coordinator] — Phase gate: Architecture approved`
   - Gate conditions:
     - Phase structure covers full process from start to finish
     - Task granularity standard documented and approved
     - Dependencies mapped with no circular dependencies
     - Every phase has a gate with verifiable conditions and blocker examples
     - Early assessment/detection phases included if domain requires them
     - Document tree complete — every referenced document has a defined path
     - Architecture satisfies requirements.md
   - Handoff:
     - output_artifacts: [architecture/phase-structure.md, architecture/task-granularity.md, architecture/dependency-map.md, architecture/phase-gates.md, architecture/document-tree.md]
     - next_phase_context: [architecture/phase-structure.md, architecture/task-granularity.md, kb-architecture.md (layer names), research/cross-cutting-concerns.md, scope.md, constraints.md]
     - excluded_context: [Dependency map detail — used during architecture, referenced if questions arise during task engineering. Full requirements.md — architecture already satisfies it, tasks reference architecture not requirements directly. Document tree — referenced during task engineering if needed.]
     - skill: none

### Phase 4: Role Engineering

**Purpose:** Define the functional roles for the output playbook. Who does what, who approves what, how do roles hand off work.

**Compilation:**
- context_load: [architecture/phase-structure.md, architecture/task-granularity.md, kb-architecture.md (layer names), research/cross-cutting-concerns.md, scope.md, constraints.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Architect — designing roles as capabilities, not org chart positions
- objective: Define all functional roles, handoff points, and escalation paths for the output playbook
- pre_check: [Phase structure covers full process, task granularity standard documented, dependencies mapped, every phase has a gate]
- failure_modes_relevant: []

**Tasks:**

1. `[Architect] — Define all roles for the output playbook`
   - For each role:
     - Role name (bracket syntax: `[RoleName]`)
     - One-line responsibility description
     - Skills/knowledge required
     - Tools/access needed
     - Decision authority (what they decide alone vs what requires escalation)
   - Role design principles:
     1. Every task must have exactly one owner
     2. No role should appear fewer than 3 times — if it does, expand scope or merge into another role
     3. Roles map to capabilities, not people
     4. Include coordination roles (who tracks progress)
     5. Include QA roles (who verifies work)
     6. Include decision roles (who approves)

2. `[Architect] — Define handoff points between roles`
   - Where does one role's output become another role's input?
   - For each handoff: what is transferred, in what format, what constitutes a complete handoff

3. `[Architect] — Define escalation paths`
   - When a task is blocked, who resolves it?
   - Technical blockers, business/strategy questions, quality disputes, resource conflicts, scope questions
   - For single-agent systems: escalation = ask the human. For multi-role systems: define the chain.

4. `[Coordinator] — Phase gate: Roles defined, handoffs documented`
   - Gate conditions:
     - All roles defined with responsibilities, required skills, and decision authority
     - Role count is appropriate for playbook complexity (minimum 2, justified if more than 6)
     - Handoff points documented for every role transition
     - Escalation paths defined
   - Note: orphaned/undefined role verification happens in Phase 8 (JSON Assembly) after tasks are written
   - Handoff:
     - output_artifacts: [architecture/role-definitions.md, architecture/handoff-points.md, architecture/escalation-paths.md]
     - next_phase_context: [architecture/role-definitions.md, architecture/phase-structure.md, architecture/task-granularity.md, architecture/phase-gates.md, kb-architecture.md (layer names — needed for writing output playbook handoff blocks), research/cross-cutting-concerns.md (needed for Task 6: weaving concerns into tasks)]
     - excluded_context: [Role engineering discussion and alternatives considered — final definitions captured in role-definitions.md. Escalation paths and handoff points — referenced during task writing if needed, not primary context.]
     - skill: none

### Phase 5: Task Engineering

**Purpose:** Write every task in the output playbook. This is where most of the output quality is created.

**Compilation:**
- context_load: [architecture/role-definitions.md, architecture/phase-structure.md, architecture/task-granularity.md, architecture/phase-gates.md, kb-architecture.md (layer names), research/cross-cutting-concerns.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Builder — writing concrete tasks, not designing structure. Every task must be executable by reading title + description alone.
- objective: Write all task titles, descriptions, gates with handoff blocks, and weave cross-cutting concerns throughout
- pre_check: [All roles defined with responsibilities, handoff points documented, escalation paths defined]
- failure_modes_relevant: []

**Tasks:**

1. `[Builder] — Write all task titles`
   - Title format: `[Role] — Action verb + specific deliverable`
   - Rules:
     1. Start with [Role] bracket
     2. Use action verbs: Build, Create, Configure, Audit, Validate, Document, Review, Deploy, Test, Fix
     3. Be specific: "Build homepage" not "Work on homepage"
     4. Include quantity or scope where relevant
     5. Every title must be understandable without reading the description
   - Write all tasks in drafts/task-list-v0.1.md first. JSON comes later.

2. `[Builder] — Write task descriptions for all non-obvious tasks`
   - WHEN to include a description:
     - Task has specific requirements (dimensions, counts, limits)
     - Task requires a specific methodology or procedure
     - Task has acceptance criteria beyond "done"
     - Task involves a decision with criteria
     - Task has anti-patterns (things NOT to do)
     - Task produces a specific document
     - Task has dependencies or prerequisites
   - DESCRIPTION STRUCTURE:
     - WHAT: what exactly is being produced
     - HOW: methodology, tools, commands, syntax rules
     - SPECS: quantitative requirements
     - DON'T: anti-patterns and common mistakes
     - OUTPUT: document path or deliverable name
     - VERIFY: how to confirm the task is done correctly
   - QUALITY CHECK: Could someone unfamiliar with this project execute this task using only the description?

3. `[Builder] — Include implementation method on all technical tasks`
   - For every implementation task, specify:
     - Which tools/interfaces to use
     - Where content/data lives
     - What scope the edit has
     - What syntax rules or constraints apply
     - What NOT to do (anti-patterns that cause problems)
     - What to test after the edit
   - A task that says "build X" without specifying how is incomplete.

4. `[Builder] — Include risk levels on tasks that affect shared resources`
   - Any task that edits a global or shared element must include:
     - SCOPE: isolated / template-level / global
     - BLAST RADIUS: what else is affected
     - RISK LEVEL: low / medium / high
     - ROLLBACK: how to undo
     - TEST AFTER: specific things to verify post-edit

5. `[Builder] — Write phase compilation blocks and gate tasks with handoff blocks`
   - Every phase opens with a compilation block: context_load, role_mindset, objective, pre_check, failure_modes_relevant
   - context_load is derived from the previous phase's next_phase_context plus decisions-ledger.md and artifact-manifest.md
   - pre_check conditions must be verifiable — typically the previous phase's gate conditions
   - failure_modes_relevant is [] for v1.0 playbooks
   - Every phase ends with a gate task assigned to [Coordinator]
   - Gate format: `[Coordinator] — Phase gate: [comma-separated verifiable conditions]`
   - Conditions must be verifiable, not subjective
   - Every gate includes blocker_examples
   - Every gate includes handoff block: output_artifacts, next_phase_context, excluded_context, skill

6. `[Builder] — Weave cross-cutting concerns into relevant tasks`
   - From cross-cutting concerns identified in Phase 1
   - Each concern must appear in at least 3 phases
   - Cross-cutting concerns that only appear once are audit failures

7. `[Auditor] — Review all tasks for completeness and clarity`
   - Binary verification checklist:
     - Does each task have a [Role] owner? Y/N
     - Is each title specific and actionable? Y/N
     - Does each non-obvious task have a description? Y/N
     - Does each description answer what, how, and how to verify? Y/N
     - Do implementation tasks include method specification? Y/N
     - Do shared-resource tasks include risk levels? Y/N
     - Is there any placeholder text (TBD, TODO)? Y/N
     - Is terminology consistent across all tasks? Y/N
     - Do file paths in descriptions match the document tree? Y/N
     - Does the [Role] in each title match the owner field? Y/N

8. `[Coordinator] — Phase gate: All tasks engineered`
   - Gate conditions:
     - All tasks written with titles and descriptions per granularity standard
     - Implementation methods specified on all technical tasks
     - Risk levels assigned on shared-resource tasks
     - Every phase has a compilation block with context_load, role_mindset, objective, pre_check, failure_modes_relevant
     - Every phase has a gate task with handoff block
     - Cross-cutting concerns appear in 3+ phases each
     - [Auditor] review passed with no unresolved issues
     - No placeholder text anywhere
   - Blocker examples:
     - A task says "Build the dashboard" with no description — violates granularity standard, needs WHAT/HOW/SPECS/VERIFY
     - Cross-cutting concern "mobile responsiveness" only appears in the design phase — must also appear in build, QA, and monitoring phases
     - Gate handoff lists next_phase_context but no excluded_context — handoff is incomplete, document what was left behind and why
   - Handoff:
     - output_artifacts: [drafts/task-list-v0.1.md, all gate definitions with handoffs]
     - next_phase_context: [drafts/task-list-v0.1.md, architecture/phase-structure.md, kb-architecture.md (layer names), architecture/role-definitions.md]
     - excluded_context: [Task engineering discussion and iterations — final tasks captured in task-list. Auditor review notes — issues resolved, fixes applied.]
     - skill: none

### Phase 6: Output Configuration

**Purpose:** Define how the output playbook is used: workflow model, session strategy, KB mapping, skill activation, and post-run review guidance. This phase produces the "operating manual" section of the output JSON.

**Compilation:**
- context_load: [drafts/task-list-v0.1.md, architecture/phase-structure.md, kb-architecture.md (layer names), architecture/role-definitions.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Architect — designing how the playbook operates, not what it contains
- objective: Define workflow model, usage instructions, session strategy, KB mapping, skill activation, router, and post-run review
- pre_check: [All tasks engineered with titles and descriptions, gates have handoff blocks, cross-cutting concerns woven into 3+ phases each, Auditor review passed]
- failure_modes_relevant: []

**Tasks:**

1. `[Architect] — Define workflow model and role descriptions`
   - Workflow model: human-in-the-loop, fully autonomous, human-directed, or custom
   - Role descriptions for the output playbook (from Phase 4 role definitions, condensed for the JSON)

2. `[Architect] — Define usage instructions`
   - How to start: what context to provide for the first phase
   - Session strategy: which phases can share a session, where to break, what context to carry
   - Cost optimization: how to minimize token usage while maintaining quality
   - General operating rules that apply to all phases

3. `[Architect] — Define phase-to-KB-layer mapping`
   - For each phase in the output playbook: which KB layers are relevant
   - Derived from: what knowledge each phase needs to do its work
   - No phase loads the entire KB — only the layers it needs
   - Must be consistent with KB layer definitions from Phase 2

4. `[Architect] — Define skill activation mapping`
   - For each phase: which prompting skill/mode to activate (or "none")
   - Must be consistent with the skill field in each gate's handoff block
   - Inconsistencies between gate handoffs and this mapping are errors (caught in JSON Assembly)
   - For new domains without existing skills, all values are "none". Skills are identified after execution runs and added in subsequent playbook versions. This is expected for first-run playbooks.

5. `[Architect] — Define router/dispatcher and context preservation specification`
   - Router: a short prompt or decision tree that sits at the entry point of the output playbook
   - Given a user request, the router determines: which KB layers to load, which playbook phase applies, and which skill to activate
   - Prevents users from having to manually decide "which KB do I need for this?" every time
   - For single-layer/flat KB playbooks: router is trivial (load the one reference, pick the phase)
   - For multi-layer KB playbooks: router maps request type to the minimum KB slice needed
   - Must be consistent with phase_kb_mapping and skill_activation
   - Context preservation: define the two persistent files for the output playbook (decisions ledger and artifact manifest) — file paths, format, update rules, what never to exclude
   - File paths must match what compilation blocks reference in context_load
   - Output: integrated into output-config.md

6. `[Architect] — Define post-run review structure`
   - What to assess after the output playbook completes a run:
     - What went well in the process
     - What didn't go well
     - What should change in the playbook
     - Metrics: target vs actual for every defined metric
     - Quality assessment: were gates effective, were tasks clear, were roles right
   - This is guidance, not a template — it tells the user what to evaluate

7. `[Coordinator] — Phase gate: Output configuration complete`
   - Gate conditions:
     - Workflow model defined
     - Roles defined with responsibilities and decision authority
     - Usage instructions cover how to start, where to break sessions, what context to carry
     - Phase-to-KB-layer mapping is consistent with KB layer definitions from Phase 2
     - Skill activation is defined for every phase (even if "none")
     - Router/dispatcher defined (or explicitly trivial for flat-KB playbooks)
     - Post-run review structure defined
   - Handoff:
     - output_artifacts: [output-config.md covering workflow, roles, usage, KB mapping, skill activation, post-run review]
     - next_phase_context: [output-config.md (summary), architecture/phase-structure.md, drafts/task-list-v0.1.md]
     - excluded_context: [Session strategy rationale — captured in output-config.md. Alternative workflow models considered — final choice documented.]
     - skill: none

### Phase 7: Metrics & KPI Definition

**Purpose:** Define measurable success criteria for the output playbook.

**Compilation:**
- context_load: [output-config.md (summary), architecture/phase-structure.md, drafts/task-list-v0.1.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Researcher — defining what success looks like, not building it. Domain outcome metrics require understanding what the playbook is supposed to achieve.
- objective: Define process, output quality, and domain outcome metrics with targets and measurement methods
- pre_check: [Workflow model defined, usage instructions defined, KB mapping and skill activation defined, router defined]
- failure_modes_relevant: []

**Tasks:**

1. `[Researcher] — Define process metrics`
   - Metrics measuring the execution of the playbook itself:
     - Duration metrics: time to complete, time per phase
     - Volume metrics: tasks completed, deliverables produced
     - Quality metrics: issues found at QA, rework count, gate failures
   - For each: title, description, type (metric_integer | metric_currency | metric_duration), category ("process"), target, measurement method

2. `[Researcher] — Define output quality metrics`
   - Metrics measuring the QUALITY of the work produced:
     - Gate pass rate, audit findings count, description completeness
     - Domain-specific quality measures
   - For each: title, description, type, category ("output_quality"), target, measurement method

3. `[Researcher] — Define domain outcome metrics`
   - Metrics measuring whether the playbook achieved its PURPOSE:
     - These are entirely domain-specific (downloads, revenue, coverage, compliance score, etc.)
     - For optimization playbooks: include BEFORE and AFTER variants
   - For each: title, description, type, category ("domain_outcome"), target, measurement method
   - At least one metric in this category is mandatory — a playbook without outcome metrics can't prove it worked.

4. `[Stakeholder] — Review and approve metrics`
   - Metrics align with business goals from Phase 0
   - Targets are ambitious but achievable
   - Every metric has a clear measurement method
   - At least one metric in each of the three categories

5. `[Coordinator] — Phase gate: Metrics defined and approved`
   - Gate conditions:
     - At least one process metric defined
     - At least one output quality metric defined
     - At least one domain outcome metric defined
     - All metrics have title, description, type, and measurement method
     - Targets set where applicable
     - [Stakeholder] approved
   - Handoff:
     - output_artifacts: [metrics-definition.md]
     - next_phase_context: [metrics-definition.md, drafts/task-list-v0.1.md, output-config.md, scope.md, constraints.md, research/cross-cutting-concerns.md, kb-architecture.md (full), entry-schema.json, bridge-schema.json (if applicable), population-strategy.md, directory-structure.md, architecture/role-definitions.md, architecture/phase-structure.md]
     - excluded_context: [Metric alternatives discussed and rejected — final metrics captured. Task granularity and dependency docs — their content is already reflected in the task list. Audit reports from previous phases — not needed for assembly.]
     - skill: none

### Phase 8: JSON Assembly & Validation

**Purpose:** Build the complete output playbook JSON and verify it is valid, complete, and consistent.

**Compilation:**
- context_load: [metrics-definition.md, drafts/task-list-v0.1.md, output-config.md, scope.md, constraints.md, research/cross-cutting-concerns.md, kb-architecture.md (full), entry-schema.json, bridge-schema.json (if applicable), population-strategy.md, directory-structure.md, architecture/role-definitions.md, architecture/phase-structure.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Builder — assembling, validating, fixing. Mechanical precision required.
- objective: Assemble the complete output playbook JSON with all fields, validate syntax/structure/consistency, include compilation blocks on every phase
- pre_check: [At least one metric per category defined, all metrics approved by Stakeholder]
- failure_modes_relevant: []

**Tasks:**

1. `[Builder] — Assemble the playbook JSON structure`
   - Required top-level fields:
     - title: playbook name
     - version: 1
     - description: 1-3 sentence overview
     - workflow_model: string
     - roles: object mapping role names to responsibility descriptions
     - scope: {in_scope: [], out_of_scope: [], adjacent: []}
     - cross_cutting_concerns: []
     - knowledge_base: {layers: [], entry_schema: {}, bridge_schema: {}, population_strategy: {}, directory_structure: {}}
     - checklists: array of phase objects with title, purpose, compilation{}, items[]
     - metrics: array of metric objects (NEVER empty)
     - usage_instructions: {how_to_run: [], session_strategy: [], cost_optimization: [], post_run_review: {}}
     - failure_modes: array of failure mode objects (empty [] for v1.0)
     - phase_kb_mapping: object mapping phase IDs to KB layer arrays
     - skill_activation: object mapping phase IDs to skill names
     - router: {description, decision_tree[], default}
     - context_preservation: {decisions_ledger, artifact_manifest, rules[]}
   - Checklist item fields:
     - title: required (string, starts with [Role])
     - owner: required (string, must match [Role] in title)
     - description: optional (string)
     - conditional: optional (string — condition for when this task applies, null if always required)
     - output: optional (file path)
   - Gate item additional fields:
     - gate_conditions: required (array of strings)
     - blocker_examples: required (array of strings)
     - handoff: required (object with output_artifacts, next_phase_context, excluded_context, skill)

2. `[Builder] — Validate JSON syntax`
   - Parse without errors
   - No trailing commas, unescaped characters, mismatched brackets
   - All string values properly escaped
   - Verification command: python3 -c "import json; json.load(open('playbook.json'))"

3. `[Builder] — Validate structural completeness`
   - Every checklist has title, purpose, and non-empty items array
   - Every item has a title starting with [Role]
   - Every item has an owner field matching the [Role] in title
   - Phase numbering is sequential
   - metrics[] is not empty
   - All required top-level fields present and non-empty
   - All boolean fields are actual booleans

4. `[Builder] — Validate [Role] consistency`
   - Extract all [Role] references from task titles
   - Compare against roles defined in the roles{} section
   - No typos, no orphaned roles (appear < 3 times), no undefined roles
   - Every owner field matches the [Role] in its task title

5. `[Builder] — Validate KB schema section`
   - If knowledge_base.layers is non-empty:
     - Every layer has name, domain boundary, authority score, separation rule
     - Entry schema has all required fields defined
     - Bridge schema (if present) has all required fields
     - Population strategy is complete
     - Directory structure is buildable (can mkdir from it)
   - If knowledge_base is minimal/flat:
     - At minimum: description of the reference format and location

6. `[Builder] — Validate compilation blocks on every phase`
   - Every checklist entry has a compilation block with all required fields
   - context_load always includes decisions-ledger.md and artifact-manifest.md (except Phase 0)
   - pre_check conditions are verifiable (not subjective)
   - role_mindset is defined and appropriate for the phase
   - objective is a single clear line
   - failure_modes_relevant references valid FM-IDs (or is empty for v1.0)
   - Each phase's context_load is consistent with the previous phase's next_phase_context

7. `[Builder] — Validate summary views match gate sources`
   - phase_kb_mapping entries must be consistent with next_phase_context in gate handoffs
   - skill_activation entries must match skill field in gate handoffs
   - Flag any inconsistencies as errors

8. `[Builder] — Validate failure_modes section`
   - failure_modes array exists (may be empty for v1.0)
   - If populated: every entry has all required fields (id, symptom, root_cause, fix, prevention, phase, severity, source)
   - severity values are valid: crash | error | degraded | cosmetic
   - phase values reference actual phases in the playbook
   - FM-IDs in compilation blocks' failure_modes_relevant reference existing entries
   - No duplicate FM-IDs

9. `[Auditor] — Manual review of assembled JSON`
   - Read through entire playbook as a user would:
     - Do tasks flow logically within each phase?
     - Are descriptions consistent in tone and detail level?
     - Would you know how to complete every task from title + description alone?
     - Are metrics measurable with the tools/access defined?

10. `[Coordinator] — Phase gate: JSON valid and complete`
   - Gate conditions:
     - JSON parses without errors
     - All structural validation checks pass
     - All [Role] references are consistent and defined
     - KB schema section is valid and complete (or explicitly minimal)
     - Every phase has a compilation block with all required fields
     - Compilation context_load is consistent with previous phase's next_phase_context
     - failure_modes section valid (empty or well-formed)
     - Summary views (phase_kb_mapping, skill_activation) match gate handoffs
     - Manual review passed
   - Blocker examples:
     - JSON has trailing comma after last array element — parser rejects it, fix syntax
     - A [Role] appears in 2 task titles but is not in the roles{} section — undefined role, either define it or fix the titles
     - phase_kb_mapping says Phase 3 uses "dsp-kb" but no gate handoff references that layer — summary doesn't match gates, reconcile
   - Handoff:
     - output_artifacts: [drafts/playbook-v0.1.json]
     - next_phase_context: [drafts/playbook-v0.1.json (full), research/requirements.md, research/cross-cutting-concerns.md]
     - excluded_context: [All intermediate drafts (task-list, architecture docs) — consolidated into JSON. Validation logs — issues fixed.]
     - skill: none

### Phase 9: Quality Audit — Gap Analysis

**Purpose:** Systematic audit of the assembled playbook against requirements, cross-cutting concerns, phase flow, contradictions, completability, and detail level.

All audit tasks use binary yes/no checklists for maximum effectiveness in single-agent review.

**Compilation:**
- context_load: [drafts/playbook-v0.1.json, research/requirements.md, research/cross-cutting-concerns.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Auditor — reviewing with fresh eyes, not defending previous work. Binary yes/no judgments, no hedging.
- objective: Systematically audit the playbook for requirements gaps, cross-cutting coverage, phase flow, contradictions, completability, detail level, and KB integrity
- pre_check: [JSON parses without errors, all structural validations pass, all Role references consistent]
- failure_modes_relevant: []

**Tasks:**

1. `[Auditor] — Audit: Does the playbook satisfy all requirements?`
   - For each requirement in research/requirements.md:
     - Is it addressed by at least one task? Y/N
     - Is the task specific enough to satisfy the requirement? Y/N
   - Output: audits/requirements-gap-analysis.md

2. `[Auditor] — Audit: Are all cross-cutting concerns woven throughout?`
   - For each concern in research/cross-cutting-concerns.md:
     - How many phases reference it?
     - If < 3 phases: flagged as insufficiently woven
   - Output: audits/cross-cutting-audit.md

3. `[Auditor] — Audit: Are there gaps between phases?`
   - Walk through sequentially:
     - Does Phase N's output fully satisfy Phase N+1's input needs? Y/N per transition
     - Is there work falling between phases? Y/N
     - Are there implicit tasks not written down? Y/N
     - Could a new user follow this playbook without asking questions? Y/N
   - Output: audits/phase-gap-analysis.md

4. `[Auditor] — Audit: Are there contradictions?`
   - Check for:
     - Tasks contradicting strategic constraints Y/N
     - Tasks contradicting the implementation method guidance Y/N
     - Metrics targets contradicting each other Y/N
     - Description details conflicting across tasks Y/N
   - Output: audits/contradiction-audit.md

5. `[Auditor] — Audit: Is every task assignable and completable?`
   - For each task:
     - Is the assigned role capable of this work? Y/N
     - Can the task be completed independently (or are dependencies explicit)? Y/N
     - Does the task have clear "done" criteria? Y/N
     - Does the task require information that no previous task produces? Y/N
   - Output: audits/task-completability-audit.md

6. `[Auditor] — Audit: Is the detail level appropriate?`
   - Check calibration:
     - Any simple tasks over-described? Y/N (list them)
     - Any complex tasks under-described? Y/N (list them)
     - Any implementation tasks missing method specification? Y/N
     - Any shared-resource tasks missing risk levels? Y/N
     - Would the least experienced target user understand every task? Y/N
   - Output: audits/detail-level-audit.md

7. `[Auditor] — Audit: Is the KB specification buildable and integrity-safe?`
   - Walk through the knowledge_base section:
     - Can an agent create the directory structure from the spec alone? Y/N
     - Can an agent create a valid KB entry from the schema alone? Y/N
     - Can an agent populate a bridge entry from the bridge schema alone? Y/N
     - Is the population strategy actionable (sources identified, quality criteria defined)? Y/N
   - Layer contamination checks (if multi-layer):
     - Do technical layer entries contain subjective/creative language? Y/N (contamination if yes)
     - Do creative/design layer entries contain code or implementation class names? Y/N (contamination if yes)
     - Does every bridge entry reference valid entries in both source KBs? Y/N
     - Do any bridge entries contradict each other? Y/N
   - Output: audits/kb-buildability-audit.md

8. `[Auditor] — Compile audit findings into prioritized fix list`
   - CRITICAL: contradictions, missing phases, broken dependencies
   - HIGH: cross-cutting gaps, incomplete descriptions, orphaned roles, unbuildable KB spec
   - MEDIUM: detail level issues, consistency problems
   - LOW: formatting, naming conventions, minor wording
   - Output: audits/fix-list.md

9. `[Coordinator] — Phase gate: All audits complete, fix list compiled`
   - Gate conditions:
     - All 7 audits completed with documented findings
     - Fix list compiled and prioritized
     - No unexamined areas
   - Handoff:
     - output_artifacts: [audits/requirements-gap-analysis.md, audits/cross-cutting-audit.md, audits/phase-gap-analysis.md, audits/contradiction-audit.md, audits/task-completability-audit.md, audits/detail-level-audit.md, audits/kb-buildability-audit.md, audits/fix-list.md]
     - next_phase_context: [audits/fix-list.md, drafts/playbook-v0.1.json]
     - excluded_context: [Individual audit reports — findings consolidated into fix-list.md. Only reference individual reports if a fix needs more context.]
     - skill: none

### Phase 10: Quality Audit — Stress Testing

**Purpose:** Walk through the playbook under different scenarios to find issues that gap analysis misses.

**Compilation:**
- context_load: [audits/fix-list.md, drafts/playbook-v0.1.json, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Auditor — adversarial thinking. Try to break the playbook by simulating real usage scenarios.
- objective: Walk through the playbook under happy path, domain novice, KB construction, blocker, variant, and edge case scenarios
- pre_check: [All 7 gap analysis audits completed, fix list compiled and prioritized]
- failure_modes_relevant: []

**Tasks:**

1. `[Auditor] — Scenario walkthrough: Happy path`
   - Execute the playbook mentally with no problems:
     - Can you start Phase 0 with only information available at project start? Y/N
     - Does each phase's output give the next phase everything it needs? Y/N
     - At each gate, are conditions verifiable? Y/N
     - At the end, has the playbook produced everything the process requires? Y/N
   - Output: testing/scenario-happy-path.md

2. `[Auditor] — Scenario walkthrough: Domain novice`
   - Walk through as someone who has NEVER done this type of work:
     - Can they understand and complete their assigned tasks? Y/N
     - Do descriptions give enough context? Y/N
     - Are tools and methods explained? Y/N
     - Would they know what to do when stuck? Y/N
   - Output: testing/scenario-domain-novice.md

3. `[Auditor] — Scenario walkthrough: KB construction by unfamiliar agent`
   - Walk through the KB specification as an agent encountering this domain for the first time:
     - Can you build the directory structure from the spec? Y/N
     - Can you create a valid entry from the schema? Y/N
     - Can you populate a bridge entry from the bridge schema? Y/N
     - Can you run a cross-reference lookup from the index structure? Y/N
   - Attempt a contamination test: write a sample entry for each layer and verify it naturally stays within its domain boundary. If writing a technical entry tempts you to use subjective language, or a creative entry tempts you to include implementation details, the layer boundaries need tightening.
   - Output: testing/scenario-kb-construction.md

4. `[Auditor] — Scenario walkthrough: Blocker scenarios`
   - Simulate common blockers:
     - What if access/tools are unavailable?
     - What if the domain has unusual or undocumented aspects?
     - What if QA fails at a phase gate? (Is the rework path clear?)
     - What if requirements change mid-execution?
   - Does the playbook handle these? Are escalation paths clear?
   - Output: testing/scenario-blockers.md

5. `[Auditor] — Scenario walkthrough: Domain variants`
   - If the playbook covers variants of the domain:
     - Walk through with Variant A
     - Walk through with Variant B
     - Do all tasks make sense for each variant?
   - Output: testing/scenario-variants.md

6. `[Auditor] — Edge case identification`
   - Minimal case (smallest possible project) — does it still work?
   - Maximal case (largest, most complex) — does it scale?
   - Legacy case (old technology/process) — does it handle it?
   - Migration case (moving from old to new) — is there a path?
   - Output: testing/edge-cases.md

7. `[Builder] — Fix all CRITICAL and HIGH issues from audit and stress testing`

8. `[Builder] — Fix MEDIUM issues from audit and stress testing`

9. `[Builder] — Re-validate JSON after all fixes`

10. `[Auditor] — Verify fixes don't introduce new issues`
    - Spot-check that CRITICAL/HIGH fixes are clean — no new contradictions, no regressions
    - Verify fix-list.md entries are resolved, not just closed
    - If issues found: return to [Builder], then re-verify
    - Output: audits/stress-test-verification.md

11. `[Coordinator] — Phase gate: Stress testing complete, fixes applied, Auditor verified`
    - Gate conditions:
      - All scenario walkthroughs completed
      - Edge cases documented
      - All CRITICAL and HIGH issues fixed
      - MEDIUM issues fixed
      - JSON re-validated after fixes
      - [Auditor] verification passed — no regressions from fixes
    - Handoff:
      - output_artifacts: [testing/scenario-*.md, testing/edge-cases.md, audits/stress-test-verification.md, drafts/playbook-v0.2.json]
      - next_phase_context: [drafts/playbook-v0.2.json, success-criteria.md, audits/fix-list.md (resolved status)]
      - excluded_context: [Scenario walkthrough details — issues extracted and fixed. Pre-fix JSON draft — superseded by v0.2.]
      - skill: none

### Phase 11: Stakeholder Review & Iteration

**Purpose:** Final human review before pilot testing.

**Compilation:**
- context_load: [drafts/playbook-v0.2.json, success-criteria.md, audits/fix-list.md (resolved status), decisions-ledger.md, artifact-manifest.md]
- role_mindset: Stakeholder — business alignment, strategic review. Does this playbook achieve what was commissioned?
- objective: Human review of playbook against business strategy, roles, and success criteria. Specific feedback, then final approval.
- pre_check: [All scenario walkthroughs completed, all CRITICAL/HIGH/MEDIUM issues fixed, JSON re-validated]
- failure_modes_relevant: []

**Tasks:**

1. `[Stakeholder] — Review playbook against business strategy`
   - Does the playbook produce the right outcome?
   - Are strategic constraints respected throughout?
   - Are metrics aligned with business KPIs?
   - Is the playbook parameterized properly (per-run variables, not hardcoded values)?
   - Would you trust this playbook to run without supervising every step?

2. `[Stakeholder] — Review roles and ownership`
   - Roles match available capabilities
   - No role is overloaded
   - Decision authority is correctly placed
   - Coordination role has enough visibility

3. `[Stakeholder] — Provide feedback and change requests`
   - Specific, actionable feedback:
     - Tasks to add, remove, or modify
     - Description changes needed
     - Phase structure changes
     - Metric or target adjustments
   - "Make it better" is not feedback. Specific changes only.

4. `[Builder] — Implement stakeholder feedback`

5. `[Builder] — Re-validate JSON after changes`

6. `[Auditor] — Verify stakeholder changes don't introduce regressions`
   - Spot-check that feedback implementation is clean — no new contradictions, no broken handoffs
   - Verify compilation blocks still consistent after any phase structure changes
   - If issues found: return to [Builder], then re-verify

7. `[Stakeholder] — Final approval of playbook`
   - Satisfies success criteria from Phase 0
   - All feedback addressed
   - Business strategy respected throughout
   - Would you run this playbook tomorrow with confidence?

8. `[Coordinator] — Phase gate: Stakeholder approved, Auditor verified`
   - Gate conditions:
     - All stakeholder feedback addressed
     - [Auditor] verification passed — no regressions from changes
     - Final approval given
     - JSON valid after changes
     - Success criteria from Phase 0 are satisfied
   - Blocker examples:
     - Stakeholder says "looks fine" without reviewing specific phases — not approval, need documented review of each concern area
     - Feedback says "add more detail to Phase 3" without specifying which tasks — not actionable, request specific task references
   - Handoff:
     - output_artifacts: [drafts/playbook-v0.3.json, stakeholder-feedback.md]
     - next_phase_context: [drafts/playbook-v0.3.json]
     - excluded_context: [All feedback discussion — changes applied to JSON. Previous draft versions — superseded.]
     - skill: none

### Phase 12: Pilot Test

**Purpose:** Run the playbook on a real project to find issues that design review can't catch. The pilot is human-observed — the agent executes, the human watches for confusion, gaps, and friction.

**Compilation:**
- context_load: [drafts/playbook-v0.3.json, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Coordinator — executing and observing. Document friction in real-time, don't rationalize it away.
- objective: Run the playbook on a real low-stakes project, document friction, incorporate feedback, get Auditor final sign-off
- pre_check: [All stakeholder feedback addressed, final approval given, JSON valid, success criteria from Phase 0 satisfied]
- failure_modes_relevant: []

**Tasks:**

1. `[Coordinator] — Select pilot project`
   - Choose a real but low-stakes project that exercises most phases
   - Define what "success" looks like for the pilot

2. `[Coordinator] — Execute pilot run`
   - Run the playbook on the selected project
   - Execute at normal pace — don't rush
   - Human observes and documents:
     - Tasks that are unclear or need more detail
     - Tasks that take longer than expected
     - Tasks in the wrong order
     - Tasks that don't apply (conditional tasks needed?)
     - Phase gates that are too strict or too loose
     - KB specification sections that were hard to follow during construction
   - Track time per task and per phase

3. `[Auditor] — Document pilot friction and catalog failure modes`
   - Questions that arose that the playbook should answer
   - Tasks over-described (can be simplified)
   - Tasks under-described (need more detail)
   - Dependency issues (wrong order)
   - Missing tasks (discovered during execution)
   - Gate adjustments needed
   - For every bug, crash, or unexpected behavior encountered during the pilot:
     - Catalog as a failure mode entry: symptom, root cause, fix, prevention, phase, severity, source
     - Prevention field becomes a candidate checklist item or gate condition in the playbook
     - Phase field assigns the failure mode to the output playbook phase where it should be caught
   - Output: testing/pilot-friction.md, testing/failure-modes-pilot.md

4. `[Builder] — Incorporate pilot feedback into playbook`
   - Common updates: clarifying descriptions, reordering tasks, adjusting scope, fixing gate conditions, updating KB spec based on build experience

5. `[Builder] — Re-validate and produce final JSON`

6. `[Auditor] — Final verification before handoff`
   - The Auditor is always the last role to touch the playbook before it leaves the creator process. This is the principle: no playbook advances to documentation/handoff without Auditor sign-off after the last round of changes.
   - Verify:
     - All pilot fixes are clean — no new contradictions introduced
     - JSON still validates (syntax, structure, [Role] consistency)
     - KB specification still buildable after any pilot-driven updates
     - No regression from earlier audit fixes (spot-check fix-list.md resolved items)
     - Gate conditions throughout the playbook are still accurate after task modifications
   - If issues found: return to [Builder] for fixes, then re-verify. Do not advance.
   - Output: audits/final-verification.md

7. `[Coordinator] — Phase gate: Pilot complete, feedback incorporated, Auditor verified`
   - Gate conditions:
     - Pilot run completed on a real project
     - Friction documented
     - Feedback incorporated into playbook
     - Updated JSON validated
     - [Auditor] final verification passed — no regressions, no new contradictions
   - Handoff:
     - output_artifacts: [final/playbook-v1.0.json, testing/pilot-friction.md, testing/failure-modes-pilot.md, audits/final-verification.md]
     - next_phase_context: [final/playbook-v1.0.json]
     - excluded_context: [All pilot execution history — findings captured in pilot-friction.md and failure-modes-pilot.md, applied to JSON. Previous draft versions.]
     - skill: none

### Phase 13: Documentation & Version Control

**Purpose:** Package the final playbook with supporting documentation.

**Compilation:**
- context_load: [final/playbook-v1.0.json, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Coordinator — packaging and archiving. Completeness and clarity for future users.
- objective: Create changelog, archive research/audit docs, store production JSON, write quick-start guide
- pre_check: [Pilot completed, friction documented, feedback incorporated, Auditor final verification passed]
- failure_modes_relevant: []

**Tasks:**

1. `[Coordinator] — Create playbook changelog`
   - Version history with dates and change summaries:
     - v0.1: initial draft
     - v0.2: post-audit fixes
     - v0.3: post-stakeholder feedback
     - v1.0: post-pilot, production ready
   - Output: final/CHANGELOG.md

2. `[Coordinator] — Archive all research and audit documents`
   - Preserve: research/, architecture/, audits/, testing/
   - These are the "why" behind design decisions

3. `[Builder] — Store production playbook JSON`
   - Final JSON in designated location
   - Naming: {playbook-name}-playbook.json
   - Never overwrite without backup of previous version

4. `[Coordinator] — Create quick-start guide`
   - One-page guide:
     - What this playbook is for
     - Prerequisites before starting
     - How to start a run
     - Key roles
     - Phase overview (one line per phase)
     - Where to find documentation
   - Output: final/QUICKSTART.md

5. `[Coordinator] — Phase gate: Documentation complete`
   - Gate conditions:
     - Changelog written
     - Research and audit docs archived
     - JSON in version control
     - Quick-start guide complete
   - Handoff:
     - output_artifacts: [final/CHANGELOG.md, final/QUICKSTART.md, final/playbook-v1.0.json, archived research/ and audits/]
     - next_phase_context: [final/playbook-v1.0.json, final/QUICKSTART.md]
     - excluded_context: [All creation process artifacts — archived for reference, not needed for ongoing use.]
     - skill: none

### Phase 14: Continuous Improvement

**Purpose:** After each completed run of the output playbook, review and optionally improve it. Manual and human-initiated — no automatic feedback loops.

**Note:** This phase has no gate — it is a recurring process with no completion state. Every other phase (0–13) has a gate.

**Compilation:**
- context_load: [final/playbook-v1.0.json (or latest version), final/QUICKSTART.md, decisions-ledger.md, artifact-manifest.md]
- role_mindset: Coordinator/Stakeholder — reviewing execution data, deciding what to change
- objective: After each playbook run, collect review data, catalog failure modes, decide whether to update the playbook
- pre_check: [At least one run of the output playbook has been completed]
- failure_modes_relevant: [all — this is where failure modes are reviewed and new ones cataloged]

**Tasks:**

1. `[Coordinator] — After each playbook run: collect review data`
   - Review the post-run assessment (using the post_run_review structure from Output Configuration)
   - Compare actual metrics to targets
   - Note tasks consistently skipped, reordered, or added ad-hoc
   - Note where the KB spec was insufficient during construction

2. `[Coordinator] — Catalog failure modes from this run`
   - For every bug, crash, unexpected behavior, or degraded result encountered during the run:
     - Create a failure mode entry: id (FM-NNN, sequential), symptom, root_cause, fix, prevention, phase, severity, source
     - severity: crash (process cannot continue), error (wrong output produced), degraded (output works but quality reduced), cosmetic (visual/formatting issue)
     - prevention: write as a concrete checklist item or gate condition that would catch this before it happens
     - phase: assign to the output playbook phase where this should be prevented or detected
   - Add entries to the playbook's `failure_modes[]` array
   - Update relevant phase compilation blocks: add new FM-IDs to `failure_modes_relevant`
   - This is how the playbook learns from its own execution

3. `[Stakeholder] — Decide whether to update the playbook`
   - Review collected data and new failure modes
   - Decide: update now, update later, or no changes needed
   - If updating: specify what changes to make and why
   - This is a human decision — the agent does not auto-modify the playbook

4. `[Builder] — Implement playbook updates (if approved)`
   - Follow same quality standards: validate JSON, check structural completeness, verify no introduced contradictions
   - Integrate failure mode prevention items into relevant phase tasks or gate conditions
   - Update changelog

---

## Output JSON Schema (Complete)

The playbook creator produces a JSON file with this structure:

```json
{
  "title": "string — playbook name",
  "version": 1,
  "description": "string — 1-3 sentence overview",

  "workflow_model": "string — human-in-the-loop | fully-autonomous | human-directed",
  "roles": {
    "[RoleName]": "string — responsibility description"
  },

  "scope": {
    "in_scope": ["what this playbook covers"],
    "out_of_scope": ["what it does NOT cover"],
    "adjacent": ["what it connects to but does not own"]
  },

  "cross_cutting_concerns": [
    "concern that must be addressed across multiple phases"
  ],

  "knowledge_base": {
    "complexity": "flat | structured",
    "layers": [
      {
        "name": "string",
        "domain_boundary": "string — what belongs here",
        "authority_score": 0.0,
        "separation_rule": "string — why this is a separate layer",
        "topics": ["string"]
      }
    ],
    "entry_schema": {
      "_note": "Domain-specific. When populated, must include required fields: id, kb, topic, status, version, title, summary, description, source, concepts, tags, related_topics, difficulty, domain_relevance. Additional domain-specific fields as needed."
    },
    "bridge_schema": {
      "_note": "Domain-specific. When populated, must include: intent_mapping fields, confidence scoring, anti_patterns, and combination_rules (how to compose multiple bridge entries with conflict detection and combined confidence scoring)"
    },
    "population_strategy": {
      "placeholder_seeding": "string",
      "harvesting_sources": ["string"],
      "curation_rules": "string",
      "sync_rules": "string",
      "versioning_protocol": "string — append-only rules, version tagging, supersede-not-edit policy"
    },
    "directory_structure": "string — exact directory tree specification"
  },

  "failure_modes": [
    {
      "id": "FM-001",
      "symptom": "string — what you observe (the bug, crash, or unexpected behavior)",
      "root_cause": "string — why it happens",
      "fix": "string — how to resolve it",
      "prevention": "string — becomes a checklist item or gate condition",
      "phase": "string — which phase this failure mode belongs to",
      "severity": "crash | error | degraded | cosmetic",
      "source": "string — which run/session this was discovered in"
    }
  ],

  "checklists": [
    {
      "title": "Phase N: Name",
      "purpose": "string — what this phase accomplishes",
      "compilation": {
        "context_load": ["string — files to load before this phase starts"],
        "role_mindset": "string — dominant thinking mode",
        "objective": "string — one-line phase goal",
        "pre_check": ["string — verifiable condition before starting"],
        "failure_modes_relevant": ["FM-IDs relevant to this phase"]
      },
      "items": [
        {
          "title": "[Role] — Action verb + deliverable",
          "owner": "[Role]",
          "description": "string (optional for obvious tasks)",
          "conditional": "string — condition under which this task applies, or null if always required",
          "output": "string — file path (optional)"
        },
        {
          "title": "[Coordinator] — Phase gate: verifiable conditions",
          "owner": "[Coordinator]",
          "gate_conditions": ["string — verifiable condition"],
          "blocker_examples": ["string — specific blocking scenario"],
          "handoff": {
            "output_artifacts": ["string — files produced"],
            "next_phase_context": ["string — what loads into next session"],
            "excluded_context": ["string — what was left behind and why"],
            "skill": "string — skill for next phase, or none"
          }
        }
      ]
    }
  ],

  "metrics": [
    {
      "title": "string",
      "description": "string",
      "type": "metric_integer | metric_currency | metric_duration",
      "category": "process | output_quality | domain_outcome",
      "target": null,
      "measurement_method": "string"
    }
  ],

  "usage_instructions": {
    "how_to_run": ["string — step-by-step instructions"],
    "session_strategy": ["string — when to break sessions, what to carry"],
    "cost_optimization": ["string — how to minimize waste"],
    "post_run_review": {
      "assess": ["string — what to evaluate after a completed run"]
    }
  },

  "phase_kb_mapping": {
    "phase_0": [],
    "phase_1": ["kb-layer-name"]
  },

  "skill_activation": {
    "phase_0": "skill name or none",
    "phase_1": "skill name or none"
  },

  "router": {
    "description": "string — what the router does for this playbook",
    "decision_tree": ["string — ordered rules: given request type X, load KB layers Y, enter phase Z, activate skill W"],
    "default": "string — what to do when no rule matches"
  },

  "context_preservation": {
    "decisions_ledger": "string — file path and format description",
    "artifact_manifest": "string — file path and format description",
    "rules": ["string — when to update, what to include, what never to exclude"]
  }
}
```

### Schema Rules

- `phase_kb_mapping` and `skill_activation` are summary views derived from gate handoffs during JSON Assembly — gates are authoritative, summaries are convenience
- Every `owner` field must match the `[Role]` prefix in its task's `title`
- `metrics` array must never be empty — minimum one metric per category (process, output_quality, domain_outcome)
- `knowledge_base` must be present. If `complexity` is `"flat"`, only `complexity` and `directory_structure` are required; `layers`, `entry_schema`, `bridge_schema`, and `population_strategy` may be empty objects/arrays
- `failure_modes` starts as an empty array `[]` in v1.0 — entries are added after execution runs. A playbook with no failure modes has never been run, not that it has no bugs
- Every checklist entry must have a `compilation` block. `failure_modes_relevant` may be `[]` for a new playbook but must reference valid FM-IDs once failure modes exist
- `compilation.context_load` must always include `decisions-ledger.md` and `artifact-manifest.md` (except Phase 0, which initializes them)
- `router` must be present. For flat-KB playbooks, `decision_tree` may be a single rule and `default` may be "load everything"
- `context_preservation` must be present. File paths must match what `compilation.context_load` references
- No trailing commas, no comments in the JSON
- All strings properly escaped
- No placeholder text (TBD, TODO, fill later)
- All gates require `blocker_examples` in the final JSON. This spec provides them for high-stakes gates (Phases 0, 2, 5, 8, 11); remaining gates should have examples added during JSON Assembly (Phase 8)

---

## Creator's Own Metrics

These metrics measure the creator's execution and the quality of the playbook it produces:

| Title | Description | Type | Category | Target | Measurement |
|-------|-------------|------|----------|--------|-------------|
| Total Phases | Phases in the produced playbook | metric_integer | output_quality | — | Count checklists[] array length |
| Total Tasks | Tasks across all phases in the produced playbook | metric_integer | output_quality | — | Count all items[] across checklists |
| Tasks With Descriptions | % of tasks that have descriptions | metric_integer | output_quality | 70 | (tasks with description / total tasks) x 100 |
| Metrics Defined | KPI metrics defined in the produced playbook (never zero) | metric_integer | output_quality | 6 | Count metrics[] array length |
| Days to Complete | Calendar days from commission (Phase 0) to documentation (Phase 13) | metric_integer | process | 10 | Calendar days between Phase 0 start and Phase 13 gate |
| Audit Issues Found | Total issues found during quality audit phases (lower = better initial drafting) | metric_integer | process | — | Count entries in audits/fix-list.md |
| Critical Issues at Pilot | Issues discovered during pilot that should have been caught earlier | metric_integer | process | 0 | Count issues in testing/pilot-friction.md and testing/failure-modes-pilot.md rated critical/high |
| Pilot Completion Rate | % of tasks in the pilot run completed as written (no ad-hoc modifications) | metric_integer | domain_outcome | 90 | (tasks completed as-written / total tasks attempted) x 100 |

---

## Creator's Own Usage Instructions

This creator playbook itself should be run as follows:

1. Start with Phase 0. Provide: the domain you want to create a playbook for, any existing playbooks to analyze, and your constraints.
2. Phases 0-1 can share a session (scoping and research are tightly coupled).
3. Phase 2 (KB Construction) benefits from a focused session — schema design requires sustained architectural thinking.
4. Phase 3 (Process Architecture) benefits from a fresh session with only the requirements, cross-cutting concerns, and KB layer names as context.
5. Phases 4-5 (Role + Task Engineering) can share a session — roles inform task writing directly.
6. Phases 6-8 (Output Config, Metrics, JSON Assembly) can share a session.
7. Phases 9-10 (Audits) benefit from a fresh session — reviewing with fresh eyes catches more.
8. Phase 11 (Stakeholder Review) is a human review gate — break session here.
9. Phase 12 (Pilot) is a full run — this will be multiple sessions.
10. Phases 13-14 (Docs, Improvement) complete the cycle.

---

## All Refinements Applied

**Design refinements (passes 1-3):**
1. Skill activation derived from gates (no duplication conflict)
2. phase_kb_mapping derived from gates (no duplication conflict)
3. KB Construction has early complexity decision (flat vs structured)
4. `drop` renamed to `excluded_context` with clear framing as documentation
5. Three metric categories required (process, quality, outcome)
6. `owner` field validates against `[Role]` in title
7. Pilot is human-observed, explicit
8. Continuous Improvement reframed as post-run, not time-based
9. Output Configuration has gate conditions
10. Task granularity standard preserved as explicit named task
11. Description writing rules (WHAT/HOW/SPECS/DON'T/OUTPUT/VERIFY) preserved
12. Platform/environment detection generalized
13. Implementation method specification preserved
14. Post-run review structure included in output configuration
15. `scope` and `cross_cutting_concerns` added to output schema
16. Audit tasks use binary yes/no checklists for single-agent effectiveness
17. Bridge applicability test defined (goal language differs from implementation language)
18. Minimal KB path is genuinely minimal (flat files with TOC)
19. KB buildability audit added to gap analysis
20. KB construction walkthrough added to stress testing
21. JSON Assembly validates KB schema section

**Spec fixes (passes 4-5):**
22. Phase 4 premature "verify orphaned roles" task removed — verification moved to Phase 8 where tasks exist
23. Phase 4 gate condition updated to verify role definitions, not usage counts
24. Creator's own metrics section added (8 metrics matching original creator)
25. `category` field added to Phase 7 metric task descriptions
26. Handoff fix: `domain-analysis.md` added to Phase 1→2 (needed for KB layer classification)
27. Handoff fix: `cross-cutting-concerns.md` carried through Phase 2→3→4→5 chain (needed for task engineering)
28. Handoff fix: `kb-architecture.md` added to Phase 4→5 (needed for output handoff block writing)
29. Handoff fix: Phase 7→8 explicit file list replaces vague "all architecture docs"
30. Blocker examples added to 5 high-stakes gates (Phases 0, 2, 5, 8, 11)
31. Phase 14 gate exemption documented (recurring process, no completion state)
32. First-run skill activation note added (all "none" is expected for new domains)
33. Flat KB minimal schema rule defined (only complexity + directory_structure required)

**Enhancements (pass 6):**
34. Bridge combination layer added — cross-reference multiple bridge entries to propose novel compositions with conflict detection and lower confidence scores
35. Append-only versioning protocol added to population strategy — entries superseded not edited, version-tagged for filtered queries
36. Router/dispatcher task added to Phase 6 (Output Configuration) — entry point that maps user requests to KB layers, phase, and skill
37. KB integrity/contamination checks added to Phase 9 gap analysis and Phase 10 stress testing — layer boundary enforcement, cross-reference validity, bridge contradiction detection
38. Auditor final verification added as last task in Phase 12 (Pilot) — no playbook advances to documentation/handoff without Auditor sign-off after the last round of changes

**Structural additions (pass 7):**
39. Compilation blocks added to all 15 phases — entry-side counterpart to handoff blocks. Each phase defines context_load, role_mindset, objective, pre_check, and failure_modes_relevant before any task runs.
40. Context preservation via two persistent files: decisions-ledger.md (append-only key decisions at every gate) and artifact-manifest.md (running index of all artifacts with phase/status/summary). Both always loaded, never excluded.
41. failure_modes[] added to output JSON schema — structured catalog of symptom, root_cause, fix, prevention, phase, severity, source. Starts empty, populated after execution runs.
42. Failure mode cataloging tasks added to Phase 12 (Pilot) and Phase 14 (Continuous Improvement). Prevention fields become checklist items or gate conditions. Phase fields assign failure modes to where they should be caught.
43. Phase 8 (JSON Assembly) expanded with compilation block validation and failure_modes section validation tasks.
44. Phase Transition Protocol section replaces Handoff Protocol — now covers both compilation (entry) and handoff (exit) with context preservation rules.
