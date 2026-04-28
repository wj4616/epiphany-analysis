# User Warning Template

Shown when auto-investigate cannot resolve low confidence.

## Template

```markdown
## ⚠️ Validation Warning - User Input Required

### Claim
"[The technical claim being validated]"

### Confidence Score: [LEVEL] (0.XX)

### Source
- Primary: `path/to/source.json` (read: [full/partial X%])
- Cross-refs: [None found / X sources]
- Contradictions: [None found / Description]

### Auto-Investigation Results
- Searched for additional sources: [X results found / None]
- Full source read: [Still shows ambiguity / Clarified X]
- Remaining issue: [Specific ambiguity or gap]

### Options
1. **Proceed anyway** - Accept risk and continue implementation
2. **Investigate manually** - Pause for you to research
3. **Provide clarification** - Give me more context about this claim
4. **Skip this claim** - Exclude from validation for this session

What would you like to do?
```

## Example Output

```markdown
## ⚠️ Validation Warning - User Input Required

### Claim
"Audio buffer size should always be a power of 2 for optimal performance"

### Confidence Score: LOW (0.48)

### Source
- Primary: `dsp-kb/buffers/buffer-basics.json` (read: partial 35%)
- Cross-refs: 1 source (dsp-kb)
- Contradictions: None found

### Auto-Investigation Results
- Searched for additional sources: 3 results found
- Full source read: Still shows ambiguity
- Remaining issue: Source mentions modern systems may not require power-of-2

### Options
1. **Proceed anyway** - Accept risk and continue implementation
2. **Investigate manually** - Pause for you to research
3. **Provide clarification** - Give me more context about this claim
4. **Skip this claim** - Exclude from validation for this session

What would you like to do?
```

## FAIL Level Template

When confidence < 0.40:

```markdown
## ❌ Validation Failed - Implementation Blocked

### Claim
"[The technical claim that failed validation]"

### Confidence Score: FAIL (0.XX)

### Why It Failed
[Specific reason - contradiction found, no source support, etc.]

### Source Analysis
- Primary: [Source or "No primary source found"]
- Cross-refs: [Number of supporting sources]
- Contradictions: [Description of contradicting sources]

### Investigation Summary
[What was checked and what was found]

### Options
1. **Provide alternative** - Give me a different claim to validate
2. **Investigate manually** - You research and provide clarification
3. **Override with caution** - Accept risk and proceed (not recommended)

What would you like to do?
```

## FAIL Level Example

```markdown
## ❌ Validation Failed - Implementation Blocked

### Claim
"Always use std::mutex to protect shared data in processBlock"

### Confidence Score: FAIL (0.28)

### Why It Failed
Explicit contradiction found in authoritative source.

### Source Analysis
- Primary: juce-kb/realtime/audio-thread.md
- Cross-refs: None supporting
- Contradictions: juce-kb states "NEVER use std::mutex or any blocking synchronization in processBlock"

### Investigation Summary
- Checked juce-kb/realtime/audio-thread-safety.json
- Checked dsp-kb/threading/audio-thread-guidelines.json
- Both sources explicitly contradict the claim
- JUCE documentation confirms mutex-free approach required

### Options
1. **Provide alternative** - Give me a different claim to validate
2. **Investigate manually** - You research and provide clarification
3. **Override with caution** - Accept risk and proceed (not recommended)

What would you like to do?
```