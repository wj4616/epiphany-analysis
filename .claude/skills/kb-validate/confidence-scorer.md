# Confidence Scorer

Calculates multi-factor confidence scores for technical knowledge claims.

## Multi-Factor Score Formula

```
# Base formula (for claims without harvest_metadata)
confidence_score = (
    source_completeness * 0.30 +
    cross_reference_match * 0.25 +
    recency * 0.15 +
    contradiction_check * 0.20 +
    implementation_history * 0.10
)

# When harvest_metadata is present on the source KB entry,
# blend the harvest confidence into the score:
if entry.harvest_metadata:
    harvest_factor = calculate_harvest_factor(entry.harvest_metadata)
    confidence_score = confidence_score * 0.70 + harvest_factor * 0.30
```

## Factor Definitions

### Source Completeness (30%)

| Completeness | Score | Condition |
|--------------|-------|-----------|
| Full | 1.0 | Entire source file read |
| Partial | 0.5 | 50-99% of source read |
| Snippet | 0.2 | < 50% of source read |
| Unverified | 0.0 | Source not verified |

**How to determine:**
- Check Read tool calls for the source file
- Full file read = file was read without offset/limit
- Partial = file was read with offset/limit parameters
- Snippet = Grep result or snippet from another file

### Cross-Reference Match (25%)

| Cross-refs | Score | Condition |
|------------|-------|-----------|
| 3+ sources | 1.0 | Claim confirmed by 3+ independent sources |
| 2 sources | 0.7 | Claim confirmed by 2 sources |
| 1 source | 0.5 | Only one source confirms |
| No support | 0.0 | No supporting sources found |

**How to determine:**
- Search KB files for matching claims
- Check session history for prior mentions
- Look for corroborating documentation

### Recency (15%)

| Age | Score | Condition |
|-----|-------|-----------|
| Fresh | 1.0 | < 1 year old |
| Recent | 0.8 | 1-2 years old |
| Dated | 0.5 | 2-5 years old |
| Stale | 0.2 | > 5 years old |

**How to determine:**
- Check file modification date
- Check documentation version/date
- Consider framework version mentioned

### Contradiction Check (20%)

| Status | Score | Condition |
|--------|-------|-----------|
| Clear | 1.0 | No contradictions found |
| Ambiguous | 0.5 | Minor ambiguity in sources |
| Contradicted | 0.0 | Explicit contradiction found |

**How to determine:**
- Search for NOT/NEVER variants of the claim
- Check for deprecation notices
- Look for version-specific differences

### Implementation History (10%)

| History | Score | Condition |
|---------|-------|-----------|
| Proven | 1.0 | Successfully used in code this session |
| Similar | 0.5 | Similar pattern used successfully |
| New | 0.0 | Not yet implemented |

**How to determine:**
- Check session history for Write/Edit operations
- Look for similar implementations in codebase
- Consider test results if available

### Harvest Confidence (blended at 30% when present)

When the source KB entry includes `harvest_metadata`, this factor blends into the overall score. It does NOT replace the base 5-factor formula — it adjusts the result.

**Calculation:**

```
harvest_factor = min(overall_confidence * method_modifier, 1.0) * source_count_modifier

method_modifier:
  - If any code_blocks have field_provenance.method == "direct-extracted": 1.1
  - If all field_provenance methods are "ai-inferred": 0.8
  - Otherwise: 1.0

source_count_modifier:
  - If source_urls has 1 entry: 0.9 (single source penalty)
  - If source_urls has 2+ entries: 1.0
  - If source_urls is empty or absent: 1.0 (not applicable, e.g., research-doc entries)
```

| Scenario | harvest_factor | Effect |
|----------|---------------|--------|
| High confidence, direct code extraction, 3 sources | min(0.72 × 1.1, 1.0) × 1.0 = 0.79 | Strong positive signal |
| Medium confidence, all ai-inferred, 1 source | min(0.55 × 0.8, 1.0) × 0.9 = 0.40 | Drags score down |
| No harvest_metadata | N/A | Base formula only |

**How to determine:**
- Read the KB entry JSON file
- Check for `harvest_metadata` key
- If present: read `overall_confidence`, `field_provenance`, and `source_urls` from `harvest_metadata`
- Calculate `harvest_factor` per formula above
- Blend: `final_score = base_score * 0.70 + harvest_factor * 0.30`

## Score Calculation Example

**Claim:** "JUCE SmoothedValue uses getNextValue() per sample"

**Base Factors:**
- Source completeness: 1.0 (full file read)
- Cross-reference: 0.7 (2 sources confirm)
- Recency: 1.0 (< 1 year)
- Contradiction: 1.0 (none found)
- Implementation history: 0.5 (similar pattern used)

**Base Calculation:**
```
base_score = 1.0 * 0.30 + 0.7 * 0.25 + 1.0 * 0.15 + 1.0 * 0.20 + 0.5 * 0.10
base_score = 0.30 + 0.175 + 0.15 + 0.20 + 0.05
base_score = 0.875
```

**Harvest metadata present?** Yes — source entry has:
- `overall_confidence`: 0.78
- `field_provenance.code_blocks.method`: "direct-extracted"
- `source_urls`: 3 URLs

**Harvest Factor:**
```
method_modifier = 1.1 (direct-extracted code_blocks present)
source_count_modifier = 1.0 (3 URLs)
harvest_factor = min(0.78 * 1.1, 1.0) * 1.0 = min(0.858, 1.0) = 0.858
```

**Final Score:**
```
score = 0.875 * 0.70 + 0.858 * 0.30
score = 0.6125 + 0.2574
score = 0.870
```

**Result:** MEDIUM (0.87) — close to HIGH threshold

## Confidence Level Thresholds

| Level | Range | Color | Action |
|-------|-------|-------|--------|
| HIGH | >= 0.85 | ✅ Green | Proceed |
| MEDIUM | 0.60 - 0.84 | ⚠️ Yellow | Proceed with warning |
| LOW | 0.40 - 0.59 | 🔍 Orange | Auto-investigate |
| FAIL | < 0.40 | ❌ Red | Block, require decision |

## Output Format

```markdown
**Confidence Score:** [LEVEL] (0.XX)

**Factors:**
- Source Completeness: [score] ([details])
- Cross-Reference: [score] ([details])
- Recency: [score] ([details])
- Contradiction: [score] ([details])
- Implementation History: [score] ([details])
```