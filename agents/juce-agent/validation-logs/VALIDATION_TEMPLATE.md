# Sound Design Validation Log Template

> Use this template for each preset validation during DAW testing (Phase 9)

## Project Information
- **Project Name:** [Plugin name]
- **Date:** [YYYY-MM-DD]
- **Tester:** [Human/AI assistant name]
- **DAW:** [REAPER/other]

## Preset Information
- **Preset Name:** [Name of preset being tested]
- **Preset Category:** [pad/lead/bass/drone/etc.]
- **Expected Sonic Result:** [What the preset should sound like based on Sound Design KB]

## Parameters Used
```
[Parameter Name] | [Value] | [Normalized 0-1]
----------------|---------|------------------
cutoff          | 0.3     | 0.3
resonance       | 0.15    | 0.15
drive           | 0.7     | 0.7
...
```

## Sound Design KB Reference
- **Source Translation:** [Which KB entry was used, e.g., "warm_pad_classic"]
- **Confidence:** [high/medium/low]
- **Source:** [Where the translation came from, e.g., "preset_analysis"]

## Testing Results

### Sound Quality Checklist
- [ ] Frequency balance appropriate
- [ ] Dynamic range appropriate
- [ ] Stereo image correct for sound type
- [ ] Harmonic content matches expectation
- [ ] Movement (if applicable) correct

### Actual Sonic Result
[Describe what was actually heard]

### Discrepancies Found
| Expected | Actual | Adjustment Needed |
|----------|--------|-------------------|
| [e.g., "Warm pad"] | [e.g., "Too bright"] | [e.g., "cutoff -0.1"] |

### Adjustments Made
```
[Parameter] | [Original] | [Adjusted] | [Reason]
------------|------------|------------|----------
cutoff      | 0.3        | 0.2        | Too bright
...
```

## Outcome
- [ ] Approved - preset matches expected result
- [ ] Needs Revision - preset adjusted and approved
- [ ] Major Revision Needed - significant changes required
- [ ] KB Update Needed - translation was incorrect

## KB Feedback (if applicable)
If the translation from Sound Design KB was incorrect, document here:

### Original Translation
```json
{
  "cutoff": {"range": [0.2, 0.4], "typical_default": 0.3},
  "resonance": {"range": [0.1, 0.2], "typical_default": 0.15}
}
```

### Corrected Translation
```json
{
  "cutoff": {"range": [0.15, 0.35], "typical_default": 0.25},
  "resonance": {"range": [0.1, 0.2], "typical_default": 0.15}
}
```

### Reason for Correction
[Explain why the original was wrong]

## UI/UX Notes (optional)
- [ ] Controls responsive
- [ ] Mix knob effective across range
- [ ] No zipper noise on automation
- [ ] UI reflects sound character

## Additional Notes
[Any other observations]

---
*Log entry created by juce-daw-testing skill. Do not edit manually after creation.*