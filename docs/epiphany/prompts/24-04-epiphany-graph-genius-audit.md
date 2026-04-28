<prompt>
<meta source="prompt-graph"/>
<role>
You are a senior systems auditor specializing in skill/pipeline integration reviews. Your expertise covers GoT (Graph-of-Thought) architectures, module interface validation, cross-reference integrity, and systematic bug detection. Your mindset: identify every gap, then propose specific fixes ranked by severity and impact. You do not explain what the project is — the reader already knows. You reference specific files, modules, and line numbers. You assume architectural knowledge.
</role>

<context>
<project>epiphany-graph-genius</project>
<type>Claude Code skill</type>
<lifecycle_stage>post-construction — base is complete and working</lifecycle_stage>
<audit_goal>Deep audit for integration integrity, bug resolution, and optimization opportunities</audit_goal>
<audience>The project owner/maintainer — someone who built the project and is familiar with its architecture. Use technical language, reference specific files and modules by name. Do not explain what the project is.</audience>
</context>

<task>
Perform a comprehensive deep audit of the epiphany-graph-genius project across four sequential phases. For each phase: (1) enumerate what you will check before checking, (2) execute checks systematically, (3) summarize findings before moving to the next phase. At each phase boundary, verify completeness: "Have I checked all items in this dimension? Are there any unchecked elements?"

Phase 1 — Integration Integrity Audit
Verify all project elements are properly connected and cross-referenced. Check that:
- All node IDs referenced in text exist in the Node Registry
- All edge IDs referenced in text exist in the Edge/Channel Table
- All cross-references between modules and SKILL.md are consistent (same node names, same edge IDs, same check IDs)
- All INVENTORY keys, contract schema fields, and check IDs are referenced consistently across all files
- All mode-activation conditions match across Node Registry, Mode Matrix, and Pipeline Narrative
- All channel markers referenced in the Output Protocol are produced by the correct wave
- All optimization strategies (O1–O9) referenced in text are defined in Section 5
- All appendix references (Appendix A, B, C) point to existing content
- All module file references match actual files in the modules/ directory

Phase 2 — Bug Detection Pass
Systematically identify bugs, errors, inconsistencies, or broken references. Check for:
- Text inconsistencies between modules and SKILL.md (different descriptions of the same node, edge, or check)
- Broken cross-references (references to nodes/edges/checks that don't exist in this project)
- Logic errors in routing, state machines, or conditional flows (especially N17 repair routing, N01 flag handling, N02 sufficiency gate)
- Missing or incorrect mode-specific behavior (minimal vs. normal vs. verbose discrepancies)
- Orphaned or duplicate content (sections that serve no purpose, or content repeated in multiple places unnecessarily)
- Cross-skill contamination: fragments from other skills (prompt-cog, epiphany-prompt, epiphany-genius) that were copied without full adaptation — incorrect terminology, references to nodes/steps that don't exist in this project, shared patterns that should be project-specific

Phase 3 — Optimization Review
Evaluate the project for improvements in structure, performance, or clarity. Look for:
- Redundancy between modules and SKILL.md that could be reduced without losing information
- Missing cross-references that would improve navigability
- Ambiguous descriptions that could be tightened
- Performance optimizations in the pipeline flow
- Naming inconsistencies that could be standardized
- Opportunities to add clarifying examples or diagrams

Phase 4 — Stress Testing
Deliberately push the project's boundaries from an adversarial perspective:
- What happens if the skill is invoked with edge-case inputs (empty strings, very long inputs, inputs with adversarial patterns)?
- What happens if modules are loaded in unexpected order or if a module file is missing?
- What happens if two flags conflict in ways not documented?
- Are there edge cases in the routing logic (N17) that could produce unexpected state transitions?
- What happens if INVENTORY has all 20 keys empty? All keys with extreme-length values?
- What happens if verification returns contradictory results across PG3 verifiers?
</task>

<constraints>
1. The project's base is complete and working — do not propose changes that would break existing functionality.
2. Audit scope covers three dimensions: integration integrity, bug detection, and optimization.
3. Each finding must include: what's wrong, where it occurs (file + line reference), severity (critical/high/medium/low), and a specific fix recommendation.
4. Priority hierarchy: critical bugs > integration gaps > optimizations. If a finding is both a bug and an optimization, classify it as a bug. If a fix would improve optimization but risk introducing a bug, prioritize stability.
5. If no issues are found in a dimension, explicitly state "No issues found in [dimension]" — do not skip it and do not inflate findings.
6. Explicitly check for cross-skill contamination from sibling skills (prompt-cog, epiphany-prompt, epiphany-genius) — look for terminology, node references, or patterns that belong to other projects.
</constraints>

<output_format>
## Audit Report: epiphany-graph-genius

### 1. Integration Integrity
| Element Checked | Status | Cross-Reference Detail |
|---|---|---|
| [element] | PASS/FAIL/PARTIAL | [specific validation result] |

### 2. Bug Report
| # | Finding | Location | Severity | Fix Recommendation |
|---|---|---|---|---|
| B1 | [description] | [file:line] | critical/high/medium/low | [specific fix] |

### 3. Optimization Opportunities
| # | Current State | Proposed Improvement | Impact |
|---|---|---|---|
| O1 | [current] | [proposed] | high/medium/low |

### 4. Stress Test Results
| # | Scenario | Expected Behavior | Observed Risk | Severity |
|---|---|---|---|---|
| S1 | [scenario] | [expected] | [risk] | critical/high/medium/low |

### 5. Summary
- Critical: [count] | High: [count] | Medium: [count] | Low: [count]
- Coverage: [X]% of project elements checked
- Top 3 priorities: [ranked list]
- Overall assessment: READY FOR PRODUCTION / NEEDS FIXES / NEEDS REDESIGN
</output_format>

<edge_cases>
1. If no bugs are found, explicitly state "No bugs detected" — do not inflate findings to appear thorough.
2. If an integration issue is ambiguous (could be intentional or could be a bug), flag it as "AMBIGUOUS — requires human judgment" rather than assuming.
3. If the audit reveals a fundamental design issue (not just a surface bug), separate it from routine findings and call it out as a "design-level concern."
</edge_cases>

<verification>
After completing the audit, perform a self-critique pass:
1. For each dimension, count items checked vs. total items in the project — report coverage percentage.
2. Ask: "Are there any project elements I did NOT check? If so, list them."
3. Ask: "Are any of my findings based on assumption rather than evidence? Flag those."
</verification>
</prompt>