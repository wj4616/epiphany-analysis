# KB Access System for Cognitive Skill Design — Implementation Plan

Generated: 2026-04-10
Mode: --plan
Source: 10-04-kb-cognitive-access-system-spec.md

---

```
---
<plan>
<goal>
  Create a working kb-cognitive skill that enables cognitive KB access during skill design. The skill follows the kb-route pattern (procedure skill referenced inline), calculates confidence scores using the NFR-3 algorithm, returns up to 5 results ordered by relevance (NFR-4), caches in-memory with timestamp validation, and handles all query types (natural language, concept, genius, trait, explore).
</goal>

<prerequisites>
  Before starting:
  1. Cognitive KB files exist at ~/.claude/skills/epiphany-cognitive/knowledgebase/
  2. kb-route skill exists at ~/.claude/skills/kb-route/SKILL.md (reference pattern)
  3. File write access to ~/.claude/ directory
</prerequisites>

<phases>
  <phase id="1" name="KB Registry Setup">
    Step 1.1: Read existing ~/.claude/kb-registry.json
    Verify: File read succeeds, current registries array visible
    
    Step 1.2: Add cognitive KB entry to kb-registry.json
    Verify: Registry contains entry with name="epiphany-cognitive", path="/home/myuser/.claude/skills/epiphany-cognitive/knowledgebase", layers=["research", "genius-minds", "traits"], bridge_eligible_layers=["research"]
    
    Checkpoint 1: Cognitive KB is registered with correct path and layer structure
  </phase>

  <phase id="2" name="Master Index Generation">
    Step 2.1: Scan ~/.claude/skills/epiphany-cognitive/knowledgebase/ to discover all .md files
    Verify: File list includes research-*.md (root), genius-minds/*.md, traits/T1/*.md, traits/T2/*.md, traits/T3-BATCH-RESEARCH.md
    
    Step 2.2: Parse each .md file to extract metadata (title from first # heading, word count, section structure)
    Verify: Each file parsed with title, word_count, has_headings, has_references, has_frontmatter extracted
    
    Step 2.3: Calculate confidence score for each entry using NFR-3 algorithm
    Verify: Score = 0.70 + 0.15(if ## headings) + 0.10(if references) + 0.05(if frontmatter) - 0.20(if TODO/FIXME) - 0.10(if <500 chars), clamped to 0.40-1.00
    
    Step 2.4: Write master-index.json to ~/.claude/skills/epiphany-cognitive/knowledgebase/
    Verify: master-index.json exists with kb_layers array containing research, genius-minds, traits layers with entries and authority_score values (0.85, 0.90, 0.80)
    
    Checkpoint 2: master-index.json exists with complete, valid structure matching actual files
  </phase>

  <phase id="3" name="SKILL.md Creation">
    Step 3.1: Create ~/.claude/skills/kb-cognitive/ directory
    Verify: Directory exists at ~/.claude/skills/kb-cognitive/
    
    Step 3.2: Write SKILL.md with frontmatter (name, description) and Resolution Procedure header
    Verify: File exists at ~/.claude/skills/kb-cognitive/SKILL.md with valid YAML frontmatter (name: kb-cognitive, description: Resolution Procedure for querying cognitive knowledge base)
    
    Step 3.3: Write Input Parameters section documenting concept, genius, trait, explore, kb parameters
    Verify: All 5 parameters documented with purpose, example, and behavior
    
    Step 3.4: Write Resolution Procedure Step 1 (Setup) - registry read, KB selection, master-index loading
    Verify: Procedure reads ~/.claude/kb-registry.json, finds epiphany-cognitive KB, loads master-index.json
    
    Step 3.5: Write Resolution Procedure Step 2 (Query Processing) - natural language detection, structured parameter parsing
    Verify: Detects "param=value" syntax as structured, free-form text as natural language; both paths handled
    
    Step 3.6: Write Resolution Procedure Step 3 (Search Execution) - layer-specific search logic
    Verify: concept searches all layers; genius searches genius-minds; trait searches traits; explore lists layer contents
    
    Step 3.7: Write Resolution Procedure Step 4 (Confidence Scoring) - NFR-3 algorithm inline
    Verify: Algorithm documented: base 0.70, +0.15 headings, +0.10 references, +0.05 frontmatter, -0.20 TODO/FIXME, -0.10 short
    
    Step 3.8: Write Resolution Procedure Step 5 (Relevance Ordering) - NFR-4 algorithm inline
    Verify: Algorithm documented: title match +3, content match +1 each (max +5), × confidence, × layer priority
    
    Step 3.9: Write Resolution Procedure Step 6 (Result Formatting) - output structure
    Verify: Output format includes source_file, confidence, relevance_score, content_summary, applicability; max 5 results
    
    Step 3.10: Write Resolution Procedure Step 7 (Caching) - in-memory cache with timestamp validation
    Verify: Cache stores parsed content + timestamps; invalidates on file change; no disk persistence
    
    Step 3.11: Write Resolution Procedure Step 8 (Error Handling) - missing entries, malformed files, empty results
    Verify: Returns informative messages; provides suggestions on empty results; skips malformed files with warning
    
    Step 3.12: Write Usage Instructions section for consumption skills
    Verify: Shows inline reference pattern: "Read and follow the Resolution Procedure in ~/.claude/skills/kb-cognitive/SKILL.md"
    
    Checkpoint 3: kb-cognitive SKILL.md is complete with all 8 Resolution Procedure steps
  </phase>

  <phase id="4" name="Query Resolution Implementation">
    Step 4.1: Implement Setup procedure (Step 1 from SKILL.md)
    Verify: Code reads kb-registry.json, selects KB, loads master-index.json
    
    Step 4.2: Implement query format detection (structured vs natural language)
    Verify: Detects "param=value" pattern; extracts parameter and value; falls back to natural language search
    
    Step 4.3: Implement concept search across all layers
    Verify: Grep searches research/*.md, genius-minds/*.md, traits/*/*.md; returns up to 5 results
    
    Step 4.4: Implement layer-specific searches (genius, trait, explore)
    Verify: genius=feynman searches genius-minds only; trait=T1 searches traits/T1 only; explore lists contents
    
    Step 4.5: Implement natural language query parsing
    Verify: Extracts keywords from free-form text; searches title and content; returns ranked results
    
    Step 4.6: Implement result ranking by relevance score (NFR-4)
    Verify: Results ordered by: title match (+3), content occurrences (+1 each, max +5), × confidence, × layer priority
    
    Checkpoint 4: All query types return correctly ordered results with proper formatting
  </phase>

  <phase id="5" name="Caching and Error Handling">
    Step 5.1: Implement in-memory cache structure (session-scoped)
    Verify: Cache object stores: parsed_entries[], file_timestamps{}, confidence_scores{}, loaded flag
    
    Step 5.2: Implement cache loading on first query
    Verify: First query reads all .md files; populates cache; subsequent queries use cache
    
    Step 5.3: Implement timestamp-based cache invalidation
    Verify: Compare cached timestamps to current file modification times; rebuild cache if changed
    
    Step 5.4: Implement graceful error handling for missing KB entries
    Verify: Missing entry returns "No results found for [query]. Available layers: [list]. Try: [suggestions]"
    
    Step 5.5: Implement error handling for malformed .md files
    Verify: Files without valid frontmatter/structure are skipped with warning; confidence set to 0.40
    
    Step 5.6: Implement empty result suggestions (FR-13)
    Verify: Empty result set returns available layers and example queries
    
    Checkpoint 5: Cache works correctly, errors handled gracefully, suggestions provided on empty results
  </phase>

  <phase id="6" name="Verification">
    Step 6.1: Verify FR-1 - Registry entry exists and is valid
    Verify: Read ~/.claude/kb-registry.json; confirm epiphany-cognitive entry with correct path and layers
    
    Step 6.2: Verify FR-2 - master-index.json exists and is valid
    Verify: Read master-index.json; confirm kb_layers array with research, genius-minds, traits
    
    Step 6.3: Verify FR-3 - SKILL.md exists with frontmatter and procedure
    Verify: Read ~/.claude/skills/kb-cognitive/SKILL.md; confirm frontmatter and Resolution Procedure
    
    Step 6.4: Verify FR-4 - Skill follows kb-route pattern
    Verify: Description includes "Not invoked directly by users"; inline reference documented
    
    Step 6.5: Verify FR-5 through FR-7 - Query types work correctly
    Verify: Test natural language query, concept param, genius param, trait param, explore param
    
    Step 6.6: Verify FR-8 and NFR-3 - Confidence scores calculated correctly
    Verify: Results include confidence 0.40-1.00; scores match algorithm specification
    
    Step 6.7: Verify FR-9 - Warnings for confidence < 0.60
    Verify: Low-confidence entries show "⚠ Medium confidence (X.XX) — verify before relying"
    
    Step 6.8: Verify FR-10 and NFR-6 - Caching works
    Verify: First query loads cache; second query uses cache; file modification triggers rebuild
    
    Step 6.9: Verify FR-11 - Conflicting results returned with attribution
    Verify: When sources disagree, both returned with source files and confidence
    
    Step 6.10: Verify FR-12 and FR-13 - Error handling and suggestions
    Verify: Missing entries show suggestions; malformed files handled gracefully
    
    Step 6.11: Verify IR-1 through IR-7 - All interface requirements
    Verify: Natural language query works; all structured params work; output format correct
    
    Step 6.12: Verify NFR-1 - Response within 5 seconds
    Verify: Query response time under 5 seconds for typical search
    
    Step 6.13: Verify NFR-4 - Relevance ordering correct
    Verify: Results in relevance order; highest relevance first
    
    Checkpoint 6: All functional and non-functional requirements verified
  </phase>
</phases>

<dependency_notes>
  - Step 1.2 must precede Step 2.4 (master-index needs registry entry)
  - Step 2.4 must precede Step 3.4 (Resolution Procedure references master-index)
  - Phase 3 must complete before Phase 4 (implementation needs procedure defined)
  - Phase 4 must complete before Phase 5 (caching wraps query logic)
  - Phase 5 must complete before Phase 6 (verification requires full implementation)
</dependency_notes>

<completion_criteria>
  The plan is complete when ALL of the following are true:
  - ~/.claude/kb-registry.json contains epiphany-cognitive entry with path=/home/myuser/.claude/skills/epiphany-cognitive/knowledgebase and layers=["research", "genius-minds", "traits"]
  - ~/.claude/skills/epiphany-cognitive/knowledgebase/master-index.json exists with valid kb_layers structure
  - ~/.claude/skills/kb-cognitive/SKILL.md exists with 8 Resolution Procedure steps documented
  - Natural language query "how does feynman approach first principles" returns Feynman-related files
  - Structured queries work: genius=feynman, trait=T1, explore=genius-minds
  - Confidence scores appear in all results (0.40-1.00 range)
  - Results with confidence < 0.60 show warning message
  - Maximum 5 results returned per query in relevance order
  - Cache loads on first query, uses cache on subsequent, rebuilds on file change
  - Missing entries return suggestions; malformed files handled gracefully
</completion_criteria>
</plan>
---
Plan: 31 steps across 6 phases | Verification tests: 31 | Safeguards: 0 | Open questions: 0
```