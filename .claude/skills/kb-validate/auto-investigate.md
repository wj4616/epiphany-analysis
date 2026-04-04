# Auto-Investigate

Automatically attempts to improve confidence when score < 0.60.

## Trigger Condition

Auto-investigate runs when:
- Initial confidence score < 0.60
- AND not already investigated this session

## Investigation Process

### Step 1: Identify Missing Information

Analyze which factors are dragging down the score:

| Factor | Low Score Cause | Investigation Action |
|--------|-----------------|----------------------|
| Source Completeness | Partial read | Read full source file |
| Cross-Reference | No supporting sources | Search for additional sources |
| Recency | Old information | Search for recent sources |
| Contradiction | Conflict found | Investigate conflict |
| Implementation History | New/untested | Look for similar implementations |

### Step 2: Execute Investigation

**For partial reads:**
```markdown
1. Identify the source file
2. Use Read tool to read the full file (no offset/limit)
3. Update source_completeness to 1.0
4. Check for additional relevant claims
5. Re-score
```

**For cross-reference gaps:**
```markdown
1. Extract key terms from claim
2. Use Grep to search local KB files (check all registered KBs via ~/.claude/kb-registry.json)
3. Use WebSearch for public documentation
4. Record supporting sources found
5. Update cross_reference score
6. Re-score
```

**For recency issues:**
```markdown
1. Identify date/version from source
2. Use WebSearch for latest version docs
3. Compare information for changes
4. Update recency score
5. Re-score
```

**For contradictions:**
```markdown
1. Identify the contradicting source(s)
2. Read both sources fully
3. Determine if version difference explains conflict
4. Determine if context difference explains conflict
5. If resolved: update contradiction_check to 1.0
6. If unresolved: note conflict for user
7. Re-score
```

**For implementation history:**
```markdown
1. Search codebase for similar implementations
2. Check session history for Write/Edit operations
3. Look for test files with similar patterns
4. Update implementation_history score
5. Re-score
```

### Step 3: Re-Score with New Information

```markdown
After investigation:
1. Recalculate all five factors
2. Compute new confidence score
3. Update cache with new results
4. If score >= 0.60: Proceed
5. If score < 0.60: Warn user
```

### Step 4: Handle Unresolved Low Confidence

When auto-investigate cannot resolve:

| New Score | Action |
|-----------|--------|
| >= 0.85 | HIGH - Proceed without warning |
| 0.60 - 0.84 | MEDIUM - Proceed with warning |
| 0.40 - 0.59 | LOW - Warn user, proceed |
| < 0.40 | FAIL - Block, require user decision |

## Investigation Limits

To prevent runaway investigation:

| Limit | Value |
|-------|-------|
| Max full file reads | 3 |
| Max searches | 5 |
| Max time | 60 seconds |
| Max sources checked | 10 |

If limits reached without resolution:
- Report findings
- Note what was investigated
- Let user decide

## Investigation Report Format

```markdown
## 🔍 Auto-Investigation Results

**Original Confidence:** LOW (0.45)

**Investigation Steps:**
1. ✅ Read full source file: juce-kb/realtime/smoothedvalue.json
2. ✅ Searched for cross-references: 2 additional sources found
3. ✅ Checked for contradictions: None found
4. ⏱️ Investigation time: 12 seconds

**New Confidence:** MEDIUM (0.68)

**Improvements:**
- Source completeness: 0.2 → 1.0 (full file read)
- Cross-reference: 0.0 → 0.7 (2 sources found)

**Remaining Gaps:**
- Implementation history: No similar implementation found in session
```

## Integration with Other Skills

**Uses:**
- `WebSearch` - Search for additional sources and public documentation
- `Grep` - Search local KB files (across all registered KBs)
- `Read` - Read full source files

**Called by:**
- `confidence-scorer.md` - When score < 0.60