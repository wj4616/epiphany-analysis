---
name: juce-testing-methodology
description: Query testing best practices from KB
---

# JUCE Testing Methodology

Query testing best practices from the Knowledge Base for DAW testing phase.

## When to Use

Invoke this skill when:
- Entering Phase 9 (DAW testing)
- User asks about testing procedures
- Debugging audio issues

## Process

### Step 1: KB-Route Concept Lookup

Read and follow the Resolution Procedure in `~/.claude/skills/kb-route/SKILL.md`
with parameters: `concept="testing methodology"`, `kb="vst-product-lifecycle"`

### Step 2: Handle Results

If results found:
- Extract testing procedures
- Present checklist for DAW testing phase

If no results:
- Fall back to built-in testing principles:
  - Audio thread safety verification
  - Parameter smoothing validation
  - Preset load/save roundtrip
  - Real-time performance check

### Step 3: Generate Testing Checklist

Present as:

```markdown
**Testing Checklist:**

- [ ] Audio thread safety: No allocations in processBlock
- [ ] Parameter smoothing: Verify ramp times
- [ ] Preset roundtrip: Save and load correctly
- [ ] Real-time performance: CPU under 5%
- [ ] Edge cases: Parameter at 0, 0.5, 1.0
- [ ] Sample rates: 44.1k, 48k, 96k
```

## Fallback Testing Principles

1. Audio thread safety verification
2. Parameter smoothing validation
3. Preset load/save roundtrip
4. Real-time performance check