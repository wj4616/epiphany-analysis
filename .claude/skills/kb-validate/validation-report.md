# Validation Report Template

Output format for validation reports.

## Inline Validation Output

Single claim validation:

```markdown
✅ Validated: "SmoothedValue uses getNextValue() per sample" (HIGH: 0.92)
   Source: juce-kb/realtime/smoothedvalue.json
   Cross-refs: juce-kb, dsp-kb
```

Medium confidence:

```markdown
⚠️ Validated: "Reverb tail should be 2-4 seconds" (MEDIUM: 0.71)
   Source: dsp-kb/reverb/reverb-basics.json (partial: 55%)
   Note: Cross-reference check incomplete
```

After auto-investigation:

```markdown
🔍 Investigating: "Buffer size should be power of 2" (LOW: 0.45)
   Searching additional sources... → MEDIUM: 0.68
```

Failed validation:

```markdown
❌ Failed: "Always use std::mutex in processBlock" (FAIL: 0.28)
   Contradiction found: juce-kb states "NEVER use mutex in audio thread"
   BLOCKED - User decision required
```

## Full Report Format

Complete validation report:

```markdown
# Validation Report

**Session:** 2026-03-30T21:00:00Z - 2026-03-30T21:30:00Z
**Total Claims:** 12

---

## Summary

| Level | Count | Percentage |
|-------|-------|------------|
| HIGH | 8 | 67% |
| MEDIUM | 3 | 25% |
| LOW (resolved) | 1 | 8% |
| FAIL | 0 | 0% |

**Overall Status:** ✅ All claims validated

---

## Validated Claims

### Claim 1: SmoothedValue uses getNextValue() per sample
- **Level:** HIGH (0.92)
- **Source:** juce-kb/realtime/smoothedvalue.json
- **Cross-refs:** juce-kb, dsp-kb, session-history
- **Validated:** 2026-03-30T21:05:00Z

### Claim 2: Reverb tail should be 2-4 seconds
- **Level:** MEDIUM (0.71)
- **Source:** dsp-kb/reverb/reverb-basics.json
- **Note:** Partial read (55%), cross-reference incomplete
- **Validated:** 2026-03-30T21:10:00Z

### Claim 3: Buffer size should be power of 2
- **Level:** MEDIUM (0.68)
- **Source:** dsp-kb/buffers/buffer-basics.json
- **Investigation:** Auto-investigated (LOW: 0.45 → MEDIUM: 0.68)
- **Validated:** 2026-03-30T21:15:00Z

---

## Investigation Log

### Investigation 1: Buffer size claim
- **Original:** LOW (0.45)
- **Actions:**
  - Read full source file
  - Searched for cross-references (2 found)
- **Result:** MEDIUM (0.68)
- **Time:** 12 seconds

---

## Blocked Claims

*None*

---

## Recommendations

1. Consider reading dsp-kb/reverb/reverb-basics.json fully to improve confidence for Claim 2
2. Session had 1 auto-investigation (resolved successfully)
```

## Gate Check Format

Pre-implementation gate check:

```markdown
## 🚦 Implementation Gate Check

**Claims Validated:** 12
**Blocking Issues:** 0

### Status by Level
- ✅ HIGH: 8 claims
- ⚠️ MEDIUM: 3 claims (proceeding with warning)
- 🔍 LOW: 0 claims
- ❌ FAIL: 0 claims

**Result:** ✅ PROCEED with implementation
```

### With Failures:

```markdown
## 🚦 Implementation Gate Check

**Claims Validated:** 12
**Blocking Issues:** 1

### Status by Level
- ✅ HIGH: 8 claims
- ⚠️ MEDIUM: 3 claims
- 🔍 LOW: 0 claims
- ❌ FAIL: 1 claim

### Blocked Claims

**Claim 4:** "Always use std::mutex in processBlock"
- **Level:** FAIL (0.28)
- **Issue:** Contradiction found
- **Action Required:** User decision needed

**Result:** ❌ BLOCKED - Resolve FAIL claims before implementation
```

## Compact Format

For inline use in other outputs:

```markdown
[✅ HIGH: 0.92] "SmoothedValue uses getNextValue() per sample"
[⚠️ MEDIUM: 0.71] "Reverb tail should be 2-4 seconds"
[🔍 LOW → MEDIUM: 0.68] "Buffer size should be power of 2" (investigated)
[❌ FAIL: 0.28] "Always use std::mutex in processBlock" (BLOCKED)
```