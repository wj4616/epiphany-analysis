---
name: juce-preset-methodology
description: Apply preset design methodology from KB
---

# JUCE Preset Methodology

Apply preset design methodology from the Knowledge Base to create parameter starting points for sound design goals.

## When to Use

Invoke this skill when:
- User requests preset creation guidance
- User asks "how do I create a X sound?"
- During Phase 9 (DAW testing) for preset refinement

## Process

### Step 1: KB-Route Concept Lookup

Read and follow the Resolution Procedure in `~/.claude/skills/kb-route/SKILL.md`
with parameters: `concept="preset methodology"`, `kb="vst-product-lifecycle"`

### Step 2: Handle Results

If results found with confidence >= 0.60:
- Extract methodology steps from entry
- Present to user: "Found preset methodology with confidence X.XX"

If confidence 0.40-0.59:
- Present with warning: "Medium confidence - verify before applying"

If no results:
- Fall back to GROUND_TRUTH_PRESETS.md patterns at:
  `$HOME/agents/juce-agent/validation-logs/GROUND_TRUTH_PRESETS.md`

### Step 3: Apply Methodology

- User provides sound goal
- Generate parameter starting points based on methodology
- Cross-reference with bridge entries for descriptor alignment

### Step 4: Present Results

Output parameter suggestions in a structured format:

```markdown
**Preset Starting Point:** <sound_goal>
**Confidence:** X.XX
**Source:** KB/fallback

**Parameters:**
| Parameter | Value | Range | Why |
|-----------|-------|-------|-----|
| ... | ... | ... | ... |

**Next Steps:**
- Refine during DAW testing
- Log results for validation
```

## Fallback

If KB unavailable, use built-in principles:
- Start with filter cutoff around 0.5 for most sounds
- Use moderate attack/release for pads
- Use fast attack for percussive sounds