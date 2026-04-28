# Task Granularity Standard

## Definition

A **task** is the smallest unit of work that:
1. Produces ONE deliverable OR makes ONE decision
2. Can be completed by the assigned role without ambiguity
3. Has a clear completion criterion (not subjective)

## Size Rules

| Condition | Action |
|-----------|--------|
| Task takes > 4 hours | Split into smaller tasks |
| Task requires multiple skill sets | Split by skill |
| Task can't fail independently | Merge with dependent task |
| Task has multiple deliverables | Split by deliverable |
| Task is "and" (do X and Y) | Split by conjunction |

## Title vs Description

| Type | When to Use | Example |
|------|-------------|---------|
| **Title Only** | Simple, obvious action | "Create CMakeLists.txt" |
| **Short Description** | Specific requirements needed | "Create SPEC.md with: plugin type, signal flow, parameters (ranges, defaults, skew), gain staging philosophy" |
| **Detailed Description** | Methodology, syntax, multi-step procedures | "Implement two-stage review: (1) spec compliance review checks algorithm, formula, ranges, signal flow; (2) code quality review checks buffer overflows, off-by-one, thread safety, edge cases. If issues found: fix + re-review BOTH stages." |

## Task Format

```json
{
  "title": "[Role] — Action verb + object",
  "owner": "[Role]",
  "description": "What to do, how to verify, any constraints",
  "output": "file.md or deliverable name",
  "validation": "verifiable condition"
}
```

## Role Assignment Rules

| Role | Typical Tasks |
|------|--------------|
| **[Coordinator]** | Gates, tracking, decisions-ledger updates, manifest updates |
| **[Researcher]** | Domain research, best practices, competitive analysis, SME interviews |
| **[Architect]** | Phase structure, task granularity, dependencies, gates, document tree |
| **[Builder]** | Implementation, file creation, JSON assembly, fixes |
| **[Auditor]** | Quality review, gap analysis, stress testing, failure mode cataloging |
| **[Stakeholder]** | Purpose, scope, constraints, success criteria, approval |

## Granularity Examples

### Too Large (Split)
❌ "Implement the DSP engine" — Multiple modules, ambiguous
❌ "Write all the tests" — Multiple test types, ambiguous
❌ "Create the GUI" — Multiple components, ambiguous

### Just Right
✓ "Implement filter module with two-stage review"
✓ "Write processBlock smoke test (silence + noise)"
✓ "Create LookAndFeel class with custom knob drawing"

### Too Small (Merge)
❌ "Open the file" — Not a deliverable
❌ "Add a comment" — Not a deliverable
❌ "Check if it compiles" — Not independent (part of implementation task)

## Verification Checklist

Each task must pass:

- [ ] Has clear deliverable or decision
- [ ] Can be completed in < 4 hours
- [ ] Assignable to ONE role
- [ ] Has verifiable completion condition
- [ ] Not dependent on incomplete prior tasks
- [ ] Clear title (role + action + object)

## Task Dependencies Within Phases

Within a phase, tasks may have dependencies:

```
Task A (no dependencies)
Task B (depends on A)
Task C (depends on A)
Task D (depends on B and C)
```

**Rule**: List dependencies explicitly in task description. Gate verifies all tasks complete.

## Output Field Rules

| Task Type | Output Field |
|----------|-------------|
| Creates file | "path/to/file.md" |
| Creates multiple files | "file1.md, file2.md" |
| Updates file | "file.md (updated)" |
| Decision | "Decision recorded in decisions-ledger.md" |
| Gate | "Gate passed: conditions verified" |

## Session Boundaries

Tasks should be grouped to respect session boundaries:

| Session Type | Task Groups |
|--------------|-------------|
| Scoping + Research | Phase 0 + Phase 1 (can share session) |
| Architecture | Phase 2 + Phase 3 (fresh session) |
| Implementation | Phase 5 (multiple sessions) |
| Integration | Phase 6 + Phase 7 (can share session) |
| Quality | Phase 9 + Phase 10 (separate sessions) |
| Commercial | Phase 11 + Phase 13 + Phase 14 (can share session) |