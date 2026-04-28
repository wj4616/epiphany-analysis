# C++ Knowledge Base Expansion Design

**Created:** 2026-03-30
**Status:** Approved
**Scope:** Phase 1 - Fill placeholders + add realtime-safety topic

## Overview

Expand the existing `cpp-kb` knowledge base to better support JUCE plugin development with audio-specific C++17 patterns. The expansion preserves all existing cppreference content while adding topic-based guides focused on real-time audio constraints.

## Goals

1. **Fill placeholder content** - Complete 5 missing cppreference files
2. **Add audio-focused topics** - Create realtime-safety topic with 4 guide files
3. **Integrate with agent skills** - Update index/manifest for skill references
4. **Maintain C++17 focus** - Use C++17 features, avoid C++20 dependencies

## Non-Goals

- C++20/23 features (JUCE targets C++17)
- Comprehensive STL coverage (only audio-relevant patterns)
- Third-party library docs (cppreference.com only)
- Phase 2 topics (containers, memory, threading guides)

## Architecture

### Directory Structure

```
cpp-kb/
├── cppreference/                    # PRESERVED - existing + filled placeholders
│   ├── algorithm*.json               # 5 files (existing)
│   ├── container_*.json              # 3 files (existing)
│   ├── memory_*.json                 # 6 files (4 existing + 2 filled)
│   ├── string*.json                 # 2 files (1 existing + 1 filled)
│   ├── thread_*.json                 # 5 files (4 existing + 1 filled)
│   ├── utility_*.json               # 3 files (2 existing + 1 filled)
│   └── functional_bind.json         # 1 file (filled)
│
├── topics/                          # NEW - audio-focused guides
│   └── realtime-safety/
│       ├── overview.md
│       ├── atomic-audio.md
│       ├── lock-free-patterns.md
│       └── pitfalls.md
│
├── index.json
└── manifest.json
```

### Content Organization

| Directory | Purpose | Source |
|-----------|---------|--------|
| `cppreference/` | C++17 API documentation | cppreference.com |
| `topics/realtime-safety/` | Audio-specific patterns and pitfalls | Curated from C++17 + audio context |

## Files to Create/Modify

### New Files (10)

| File | Content |
|------|---------|
| `cppreference/thread_atomic.json` | C++17 atomic API documentation |
| `cppreference/memory_make_shared.json` | make_shared API documentation |
| `cppreference/memory_make_unique.json` | make_unique API documentation |
| `cppreference/string_string_view.json` | string_view API documentation |
| `cppreference/functional_bind.json` | bind API documentation |
| `cppreference/utility_function.json` | function API documentation |
| `topics/realtime-safety/overview.md` | Real-time constraints introduction |
| `topics/realtime-safety/atomic-audio.md` | std::atomic for audio thread |
| `topics/realtime-safety/lock-free-patterns.md` | Lock-free communication |
| `topics/realtime-safety/pitfalls.md` | Common mistakes |

### Modified Files (2)

| File | Changes |
|------|---------|
| `index.json` | Add topics structure, new cross-references |
| `manifest.json` | Register new files with synced status |

### Placeholder URLs

| Placeholder | Source URL |
|-------------|-----------|
| `thread_atomic` | `https://en.cppreference.com/w/cpp/atomic` |
| `memory_make_shared` | `https://en.cppreference.com/w/cpp/memory/make_shared` |
| `memory_make_unique` | `https://en.cppreference.com/w/cpp/memory/make_unique` |
| `string_string_view` | `https://en.cppreference.com/w/cpp/string/basic_string_view` |
| `functional_bind` | `https://en.cppreference.com/w/cpp/utility/functional/bind` |
| `utility_function` | `https://en.cppreference.com/w/cpp/utility/functional/function` |

## Integration Points

### master-index.json Updates

Add new topic entries:

```json
"realtime_safety": {
  "description": "Real-time audio thread safety patterns",
  "kbs": ["cpp-kb", "juce-kb"],
  "topics": ["atomic", "lock_free", "audio_thread"]
},
"audio_thread": {
  "description": "Audio thread programming rules",
  "kbs": ["cpp-kb"]
}
```

### Skill References

Update skill files to reference new content:

| Skill | New Reference |
|-------|---------------|
| `juce-audio-thread-audit/SKILL.md` | Link to `realtime-safety/pitfalls.md` |
| `juce-dsp-implementation/SKILL.md` | Link to `realtime-safety/overview.md` |

### kb_lookup.py Updates

Check if `kb_lookup.py` supports subdirectory searches:
```python
# In kb_lookup.py, ensure search paths include topics/
# Current structure searches: cpp-kb/*.json
# Add: cpp-kb/topics/*/*.md
```

If modification needed, add topic directory to search paths in `resolve_topic()` function.

## Content Guidelines

### cppreference JSON Files

Preserve exact API documentation format:
- Function signatures with parameter types
- Return value documentation
- Code examples from cppreference
- C++17 version notes where applicable

### topics/*.md Files

Each topic file follows this structure:

```markdown
# [Title]

## Why This Matters
[2-3 sentences on real-time audio context]

## When to Use
[Specific JUCE plugin scenarios]

## C++17 Features
[Feature list with brief explanations]

## Code Pattern
```cpp
// Real-time safe example with beginner comments
```

## Pitfalls
1. **[Pitfall name]** - [Explanation]
2. **[Pitfall name]** - [Explanation]

## Related
- cppreference: [linked file]
- JUCE: [relevant JUCE class/docs]
```

## Implementation Steps

### Step 1: Harvest Placeholder Content

Fill 6 placeholder files using one of these methods:

**Method A: Web Search (Preferred)**
```
Use WebSearch to fetch cppreference.com content for each URL in the Placeholder URLs table.
Extract API documentation and save as JSON files.
```

**Method B: Manual Content Creation**
If automated harvesting is unavailable:
```
1. Visit each cppreference URL in the table
2. Extract function signatures, parameters, return values, examples
3. Create JSON file with structure matching existing cppreference files
```

Files to fill:
- `thread_atomic.json`
- `memory_make_shared.json`
- `memory_make_unique.json`
- `string_string_view.json`
- `functional_bind.json`
- `utility_function.json`

### Step 2: Create topics Directory Structure

```bash
mkdir -p cpp-kb/topics/realtime-safety
```

### Step 3: Write Topic Files

Create 4 markdown files in `topics/realtime-safety/`:
1. `overview.md` - Introduction to real-time constraints
2. `atomic-audio.md` - std::atomic patterns for audio
3. `lock-free-patterns.md` - Lock-free communication
4. `pitfalls.md` - Common mistakes

### Step 4: Update Index Files

- Update `index.json` with topics structure
- Update `manifest.json` with new file entries
- Update `master-index.json` with cross-references

### Step 5: Update Skills

Add references in skill files using portable paths:
```
**Related:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md`
```

Update these skills:
- `juce-audio-thread-audit/SKILL.md` - add link to `pitfalls.md`
- `juce-dsp-implementation/SKILL.md` - add link to `overview.md`

### Step 6: Validate

```bash
# From playbookdata directory
cd ~/agents/juce-agent/playbookdata
python3 scripts/validate-kb-structure.py

# Check placeholder files are filled
cat cpp-kb/manifest.json | jq '.topics[].files | to_entries[] | select(.value.status == "placeholder")'

# Verify topic files exist
ls -la cpp-kb/topics/realtime-safety/
```

## Testing

### Validation Checks

| Check | Command |
|-------|---------|
| KB structure valid | `cd $AGENT_ROOT/playbookdata && python3 scripts/validate-kb-structure.py` |
| Index updated | `cat cpp-kb/index.json \| jq '.topics'` |
| Manifest synced | `cat cpp-kb/manifest.json \| jq '.topics[].files \| keys'` |
| Placeholder filled | Check 6 files have `status: "synced"` |

### Manual Review

- [ ] Topic files have correct structure
- [ ] Cross-references resolve correctly
- [ ] Skill links point to valid files
- [ ] Content is audio-focused with C++17 scope

## Phase 2 (Future)

After Phase 1 validation:
- Add `topics/containers/` - vector, span, array for DSP
- Add `topics/memory/` - smart pointers and RAII for audio
- Add `topics/threading/` - message thread patterns

## Success Criteria

1. All 6 placeholder files have content (status: synced)
2. 4 topic files created with audio-specific guidance
3. Index and manifest updated correctly
4. Skills can reference new content
5. No existing cppreference content lost or modified

## Risks

| Risk | Mitigation |
|------|------------|
| Content drift between topics and cppreference | Topics summarize, cppreference is source of truth |
| Placeholder harvesting fails | Use manual cppreference content if automated fails |
| Integration breaks existing skills | Test skill references before/after changes |

## References

- cppreference.com - Primary C++17 documentation source
- JUCE documentation - For JUCE-specific context
- Existing cpp-kb structure - Preserve current organization
- master-index.json format - Follow existing cross-reference pattern