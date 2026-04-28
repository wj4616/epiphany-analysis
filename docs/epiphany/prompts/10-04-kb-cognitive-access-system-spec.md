# KB Access System for Cognitive Skill Design — Specification

Generated: 2026-04-10
Mode: --specification
Audit: Fixed all identified issues

---

```
---
<specification>
<scope>
  Included:
    - Registration of cognitive KB in ~/.claude/kb-registry.json
    - New skill (kb-cognitive) for accessing cognitive knowledge base
    - Inline reference integration (thinking skills reference this skill's procedure)
    - Knowledge verification/validation with defined confidence scoring
    - Design-time access (during skill creation, not runtime)
    - Both natural language and structured query support
    - In-memory session caching with file timestamp validation
    - Master-index.json generation for cognitive KB
  
  Excluded:
    - Modification of existing thinking skills (brainstorming, epiphany-genius, etc.)
    - Runtime KB access (skills operate independently after creation)
    - KB content creation/editing (files already exist)
    - Automatic KB harvesting from web sources
</scope>

<context>
  Domain: Software / Knowledge Management / Skill-Based Agent Architecture
  
  Stakeholders:
    - Skill developers: Design cognitive/creative skills using KB knowledge
    - Thinking skills: epiphany-genius, brainstorming, prompt-epiphany, writing-plans, executing-plans
    - Cognitive KB: Markdown files containing cognitive research, genius analysis, thinking traits
    - New skill (kb-cognitive): Provides verified KB access during design
  
  Background:
    The user has created a knowledge base at ~/.claude/skills/epiphany-cognitive/knowledgebase/ 
    containing:
    - Root level: research-*.md files (TRIZ, conceptual blending, lateral thinking, etc.)
    - genius-minds/ subdirectory: genius analysis files (Einstein, Feynman, Turing, etc.)
    - traits/ subdirectory: T1/, T2/, T3-BATCH-RESEARCH.md (cognitive trait files)
    
    This KB is not yet formally registered. The user wants to create a new skill that 
    accesses this cognitive knowledge during skill design. The skill must verify claims 
    before using them. Existing thinking skills should NOT be modified.
    
    Integration pattern: kb-cognitive is a procedure skill (like kb-route). Thinking 
    skills reference it inline with:
    ```
    Read and follow the Resolution Procedure in ~/.claude/skills/kb-cognitive/SKILL.md
    ```
    
  Primary Intent:
    Enable cognitive skill designers to access verified cognitive KB knowledge during design.
    Skills created using this knowledge operate independently — KB is design-time only.
</context>

<functional_requirements>
  FR-1: The system SHALL register the cognitive KB in ~/.claude/kb-registry.json.
        Verification: Registry contains entry with name="epiphany-cognitive", path pointing to knowledgebase directory, and layers array matching actual structure.
        
  FR-2: The system SHALL create master-index.json in the cognitive KB root directory.
        Verification: master-index.json exists with kb_layers array listing all entries with metadata.
        
  FR-3: The system SHALL create a new skill named kb-cognitive at ~/.claude/skills/kb-cognitive/SKILL.md.
        Verification: Skill file exists with frontmatter (name, description) and Resolution Procedure.
        
  FR-4: The kb-cognitive skill SHALL follow the kb-route pattern: procedure skill referenced inline by consumption skills.
        Verification: Skill description states "Not invoked directly by users" and documents inline reference pattern.
        
  FR-5: The kb-cognitive skill SHALL accept both natural language queries and structured parameters (concept, genius, trait, explore).
        Verification: Query "how does feynman approach first principles" returns Feynman analysis; parameter genius='feynman' returns same.
        
  FR-6: The kb-cognitive skill SHALL auto-detect query format by checking for structured parameter syntax (param=value).
        Verification: Input with "param=value" syntax is parsed as structured; free-form text is natural language query.
        
  FR-7: The kb-cognitive skill SHALL query the cognitive KB and return up to 5 most relevant results ordered by relevance score.
        Verification: Queries return at most 5 results, sorted by relevance (see NFR-4 for scoring).
        
  FR-8: The kb-cognitive skill SHALL calculate confidence score for each entry based on defined algorithm (see NFR-3).
        Verification: Every result includes confidence score between 0.00-1.00.
        
  FR-9: The kb-cognitive skill SHALL warn when entry confidence is below 0.60.
        Verification: Results with confidence < 0.60 include "⚠ Medium confidence (X.XX) — verify before relying" warning.
        
  FR-10: The kb-cognitive skill SHALL cache parsed KB content in memory for session duration with file modification timestamp tracking.
        Verification: First query loads and caches KB; subsequent queries use cache unless files modified (timestamp check).
        
  FR-11: The kb-cognitive skill SHALL return all conflicting results with attribution when sources disagree.
        Verification: When entries have contradictory claims, both are returned with source files and confidence scores.
        
  FR-12: The kb-cognitive skill SHALL handle missing KB entries, malformed files, and empty directories gracefully.
        Verification: Missing entries return "No results found for [query]. Try: [suggestions]"; malformed files are skipped with warning.
        
  FR-13: The kb-cognitive skill SHALL provide query suggestions when no results are found.
        Verification: Empty result set returns list of available layers and example queries.
</functional_requirements>

<non_functional_requirements>
  NFR-1: The kb-cognitive skill SHALL respond within 5 seconds for typical searches (under 100 entries scanned).
         Metric: Query response time under 5 seconds.
         
  NFR-2: The system SHALL maintain KB integrity — read-only access to KB files.
         Metric: No modifications to source .md files during any operation.
         
  NFR-3: Confidence Scoring Algorithm:
         Each KB entry receives a confidence score (0.00-1.00) calculated as:
         - Base score: 0.70 for all entries
         - +0.15 if entry has clear section structure (## headings)
         - +0.10 if entry cites sources or references
         - +0.05 if entry has frontmatter with status/metadata
         - -0.20 if entry has TODO, FIXME, or placeholder markers
         - -0.10 if entry is under 500 characters (likely incomplete)
         - Final score clamped to 0.40-1.00 range
         Metric: Confidence score appears in all results with breakdown visible on request.
         
  NFR-4: Relevance Ordering Algorithm:
         Results are ordered by relevance score calculated as:
         - Query term match in title/heading: +3 points
         - Query term match in content: +1 point per occurrence (max +5)
         - Confidence score multiplier: points × confidence
         - Layer priority: genius-minds (×1.2), traits (×1.1), research (×1.0)
         Metric: Results returned in relevance order; relevance score visible on request.
         
  NFR-5: The system SHALL be extensible to additional cognitive content without code changes.
         Metric: Adding new .md files to knowledgebase directories makes them discoverable after cache refresh.
         
  NFR-6: The cache SHALL store: parsed content, file timestamps, confidence scores, and computed relevance scores.
         Metric: Cache structure includes all fields needed to answer queries without re-reading files.
</non_functional_requirements>

<interface_requirements>
  IR-1: The kb-cognitive skill SHALL accept natural language queries.
        Verification: "how does feynman approach first principles" returns Feynman analysis with relevance ranking.
        
  IR-2: The kb-cognitive skill SHALL accept structured parameter 'concept=<term>' for semantic search across all layers.
        Verification: "concept=constraint-naming" returns relevant trait and research files.
        
  IR-3: The kb-cognitive skill SHALL accept structured parameter 'genius=<name>' for genius-minds layer lookup.
        Verification: "genius=feynman" returns gm-feynman-first-principles.md and related Feynman files.
        
  IR-4: The kb-cognitive skill SHALL accept structured parameter 'trait=<tier>' for traits layer lookup.
        Verification: "trait=T1" returns all T1-tier trait files with confidence scores.
        
  IR-5: The kb-cognitive skill SHALL accept structured parameter 'explore=<layer>' for layer browsing.
        Verification: "explore=genius-minds" lists all genius files with titles and confidence scores.
        
  IR-6: The kb-cognitive skill SHALL accept structured parameter 'kb=<name>' to target specific registered KB.
        Verification: "kb=epiphany-cognitive concept=test" searches only that KB.
        
  IR-7: The kb-cognitive skill SHALL output structured knowledge with: source_file, confidence, relevance_score, content_summary, applicability.
        Verification: Output includes all required fields in consistent format.
</interface_requirements>

<data_requirements>
  DR-1: The system SHALL read KB files from ~/.claude/skills/epiphany-cognitive/knowledgebase/.
        Verification: File reads succeed for research-*.md (root), genius-minds/*.md, traits/*/*.md.
        
  DR-2: The system SHALL create/update ~/.claude/kb-registry.json with cognitive KB entry.
        Verification: Registry contains epiphany-cognitive with correct path and layer configuration.
        
  DR-3: The system SHALL create master-index.json at knowledgebase root with structure:
        {
          "kb_layers": [
            {"name": "research", "entries": [...], "authority_score": 0.85},
            {"name": "genius-minds", "entries": [...], "authority_score": 0.90},
            {"name": "traits", "entries": [...], "authority_score": 0.80}
          ],
          "generated_at": "ISO-timestamp",
          "total_entries": N
        }
        Verification: master-index.json exists with valid structure matching actual files.
        
  DR-4: The cache SHALL be stored in memory (session-scoped, not persisted to disk).
        Verification: No cache files written to disk; cache exists only in session context.
        
  DR-5: Each master-index entry SHALL include: id, title, file_path, confidence, tags[], word_count.
        Verification: Entry structure matches specification; all fields populated.
</data_requirements>

<constraints>
  C-1: The system SHALL NOT modify existing thinking skills (epiphany-genius, brainstorming, prompt-epiphany, etc.).
  C-2: The system SHALL NOT require KB access at skill runtime (design-time only).
  C-3: The system SHALL NOT modify source KB files (read-only access).
  C-4: The kb-cognitive skill SHALL verify claims with confidence before presenting them (no blind assertions).
  C-5: The kb-cognitive skill SHALL return maximum 5 results per query.
  C-6: The kb-cognitive skill SHALL NOT invoke kb-harvest or other external skills (standalone operation).
</constraints>

<kb_structure>
  The cognitive KB has the following structure:
  
  kb-root: ~/.claude/skills/epiphany-cognitive/knowledgebase/
  ├── research-*.md              # Root-level research files (TRIZ, conceptual blending, etc.)
  ├── creative-genius-*.md       # Root-level cognitive differentiators report
  ├── genius-minds/              # Genius analysis layer
  │   ├── gm-einstein-*.md
  │   ├── gm-feynman-*.md
  │   └── ...
  ├── traits/                    # Cognitive traits layer
  │   ├── T1/                   # Tier 1 traits
  │   ├── T2/                   # Tier 2 traits  
  │   └── T3-BATCH-RESEARCH.md  # Tier 3 batch research
  └── master-index.json          # Generated index (not source file)
  
  Layers for registry:
  - research (root-level .md files)
  - genius-minds (genius-minds/ subdirectory)
  - traits (traits/ subdirectory including T1, T2, T3)
</kb_structure>

<resolved_questions>
  RQ-1: Query format → Both natural language and structured parameters, auto-detect by syntax.
  RQ-2: Confidence threshold → 0.60 (below this triggers warning).
  RQ-3: Caching → In-memory session cache with file timestamp validation.
  RQ-4: Result limits → Maximum 5 results per query, ordered by relevance.
  RQ-5: Conflict handling → Return all conflicting results with attribution.
  RQ-6: Integration mechanism → Procedure skill referenced inline (like kb-route pattern).
  RQ-7: Confidence calculation → Algorithm defined in NFR-3 (base 0.70 + modifiers).
  RQ-8: Relevance ordering → Algorithm defined in NFR-4 (match points × confidence × layer).
  RQ-9: Indexing → master-index.json generated at KB root with entry metadata.
  RQ-10: Cache storage → In-memory only, not persisted to disk.
</resolved_questions>
</specification>
---
Coverage: 13 functional requirements, 6 non-functional, 7 interface, 5 data, 6 constraints | Open questions: 0
```