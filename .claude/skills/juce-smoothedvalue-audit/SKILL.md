---
name: juce-smoothedvalue-audit
description: Use AFTER implementing SmoothedValue parameters to verify correct usage. Checks for EM-13 and CM-05 bugs (dead parameters, once-per-block instead of per-sample).
---

# JUCE SmoothedValue Audit

Specific audit for SmoothedValue usage — one of the most common bug patterns in JUCE plugins. Based on EM-13 (dead DRIVE parameter) and CM-05 (SmoothedValue called once per block).

**Playbook Reference:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v6-unified.json` — failure_modes (FM-09, EM-13, CM-05)

## The Bug Pattern

**EM-13:** The DRIVE knob was dead because `getNextValue()` was never called — silent bug, no crash, no error.

**CM-05:** SmoothedValue was called once per block instead of per sample.

Both bugs result in parameters that don't work, but the code compiles fine.

## Quick Check

For each SmoothedValue in your code:

```cpp
// Step 1: Find all SmoothedValue declarations
// In header file:
juce::SmoothedValue<float> gainSmoothed;
juce::SmoothedValue<float> freqSmoothed;
juce::SmoothedValue<float> mixSmoothed;

// Step 2: Check prepareToPlay - MUST initialize
void prepareToPlay(double sampleRate, int /*samplesPerBlock*/) {
    gainSmoothed.reset(sampleRate, 0.02);  // 20ms ramp
    gainSmoothed.setCurrentAndTargetValue(initialGain);

    freqSmoothed.reset(sampleRate, 0.02);
    freqSmoothed.setCurrentAndTargetValue(initialFreq);
}

// Step 3: Check processBlock - setTargetValue ONCE per block
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    // Set targets once per block (NOT per sample!)
    gainSmoothed.setTargetValue(*apvts.getRawParameterValue("gain"));
    freqSmoothed.setTargetValue(*apvts.getRawParameterValue("freq"));

    // Step 4: Check that getNextValue() is called PER SAMPLE
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            // CORRECT: Get next value in sample loop
            const float gain = gainSmoothed.getNextValue();
            const float freq = freqSmoothed.getNextValue();

            channelData[sample] *= gain;  // Use the smoothed value!
        }
    }
}
```

## Common Mistakes

### Mistake 1: Using getCurrentValue() instead of getNextValue()

```cpp
// WRONG - doesn't advance the smoother!
for (int sample = 0; sample < numSamples; ++sample) {
    const float gain = gainSmoothed.getCurrentValue();  // BUG!
    output[sample] *= gain;
}

// CORRECT - advances the smoother each call
for (int sample = 0; sample < numSamples; ++sample) {
    const float gain = gainSmoothed.getNextValue();  // Correct!
    output[sample] *= gain;
}
```

### Mistake 2: Not calling getNextValue() at all

```cpp
// WRONG - smoother value never used!
gainSmoothed.setTargetValue(targetGain);  // Set target
for (int sample = 0; sample < numSamples; ++sample) {
    output[sample] *= targetGain;  // BUG: using raw target, not smoothed
}

// CORRECT - use the smoothed value
gainSmoothed.setTargetValue(targetGain);
for (int sample = 0; sample < numSamples; ++sample) {
    output[sample] *= gainSmoothed.getNextValue();  // Use smoothed
}
```

### Mistake 3: Calling getNextValue() outside sample loop

```cpp
// WRONG - called once per block, not per sample
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    gainSmoothed.setTargetValue(targetGain);
    const float gain = gainSmoothed.getNextValue();  // BUG: only once!

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        output[sample] *= gain;  // Same value for all samples - no smoothing!
    }
}

// CORRECT - called per sample in the loop
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    gainSmoothed.setTargetValue(targetGain);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        const float gain = gainSmoothed.getNextValue();  // Correct!
        output[sample] *= gain;
    }
}
```

### Mistake 4: Forgetting to use the smoothed value

```cpp
// WRONG - getNextValue() called but not used
for (int sample = 0; sample < numSamples; ++sample) {
    gainSmoothed.getNextValue();  // Advances smoother but...
    output[sample] *= someOtherGain;  // BUG: not using smoothed value!
}

// CORRECT - use the returned value
for (int sample = 0; sample < numSamples; ++sample) {
    const float gain = gainSmoothed.getNextValue();
    output[sample] *= gain;  // Use it!
}
```

## SmoothedValue Audit Checklist

For **each** SmoothedValue in your plugin:

```markdown
### [SmoothedValue Name]

- [ ] Declared in header as member variable
- [ ] Initialized in prepareToPlay with reset(sampleRate, rampTime)
- [ ] setCurrentAndTargetValue(initialValue) called in prepareToPlay
- [ ] setTargetValue() called ONCE per block (not per sample)
- [ ] getNextValue() called PER SAMPLE in processing loop
- [ ] Returned value from getNextValue() is actually USED in processing
- [ ] NEVER using getCurrentValue() in processBlock
- [ ] Ramp time appropriate:
    - ~20ms (0.02s) for continuous parameters (gain, frequency, mix)
    - ~42ms (0.042s) for bypass/mode transitions (prevents clicks)
```

## Audit Output Format

```markdown
## SmoothedValue Audit Report

### [SmoothedValue 1: gainSmoothed]
- Declared: ✅
- Initialized in prepareToPlay: ✅
- setTargetValue per block: ✅
- getNextValue per sample: ✅
- Value used: ✅
- Ramp time: 20ms ✅
**Status: PASS**

### [SmoothedValue 2: freqSmoothed]
- Declared: ✅
- Initialized in prepareToPlay: ❌ MISSING
- setTargetValue per block: ✅
- getNextValue per sample: ✅
- Value used: ✅
- Ramp time: N/A
**Status: FAIL - Add: freqSmoothed.reset(sampleRate, 0.02); freqSmoothed.setCurrentAndTargetValue(initialFreq);**

### Summary
- Total SmoothedValues: 5
- Passing: 4
- Failing: 1
- Issues: [List with fix suggestions]
```

## Ramp Time Guidelines

| Parameter Type | Recommended Ramp Time | Why |
|----------------|----------------------|-----|
| Gain/Volume | 20ms (0.02s) | Prevents zipper noise |
| Frequency | 20ms (0.02s) | Smooth pitch changes |
| Mix/Dry-Wet | 20ms (0.02s) | Smooth crossfade |
| Bypass Toggle | 42ms (0.042s) | Prevents click on toggle |
| Mode Switch | 42ms (0.042s) | Prevents click on mode change |

**Note:** Snap parameter changes (immediate) should use `setCurrentAndTargetValue()` only in `prepareToPlay()` or when loading state — not during normal operation.

## grep Commands for Audit

```bash
# Find all SmoothedValue declarations
grep -r "SmoothedValue" --include="*.h" src/

# Find setTargetValue calls
grep -r "setTargetValue" --include="*.cpp" src/

# Find getNextValue calls
grep -r "getNextValue" --include="*.cpp" src/

# Find getCurrentValue (should be rare in processBlock)
grep -r "getCurrentValue" --include="*.cpp" src/

# Count getNextValue calls vs setTargetValue
# There should be MANY more getNextValue calls (per sample vs per block)
grep -c "getNextValue" src/PluginProcessor.cpp
grep -c "setTargetValue" src/PluginProcessor.cpp
```

## Related Failure Modes

| ID | Description | Prevention |
|---|---|---|
| FM-09 | Parameters don't smooth (zipper noise) | Call getNextValue() per sample |
| EM-13 | SmoothedValue never called — dead parameter | Audit every SmoothedValue |
| CM-05 | SmoothedValue called once per block | Call inside sample loop |

## Phase Gate

Before proceeding from Phase 6 (Processor Integration), verify:

- [ ] Every SmoothedValue declaration found in header
- [ ] Every SmoothedValue initialized in prepareToPlay
- [ ] Every SmoothedValue has setTargetValue per block
- [ ] Every SmoothedValue has getNextValue per sample
- [ ] Every SmoothedValue returned value is used
- [ ] No getCurrentValue in processBlock
- [ ] Ramp times documented and appropriate

## Fallback and Error Handling

### Cannot Find SmoothedValue Declarations

**Symptom:** No SmoothedValue found in headers

**Fallback:**
```markdown
**No SmoothedValue declarations found**

This could mean:
1. Plugin uses direct parameter access (may cause zipper noise)
2. Plugin doesn't smooth parameter changes
3. SmoothedValue declared differently (e.g., using alias)

**Check:**
```bash
# Alternative search patterns
grep -r "LinearSmoothedValue\|SmoothedValue\|smooth" --include="*.h" src/
grep -r "skew\|ramp" --include="*.h" src/
```

**Recommendation:**
If parameters change during playback without smoothing:
- Add `SmoothedValue<float>` for each parameter
- Initialize in prepareToPlay()
- Call setTargetValue() per block, getNextValue() per sample
```

### Audit Finds Missing Smoothing

**Symptom:** Parameters changed without SmoothedValue

**Fallback:**
```markdown
**Parameter smoothing missing**

Parameter found without smoothing: `[parameter_name]`

**Impact:**
- May cause zipper noise on parameter change
- May cause clicks when toggling effects
- May cause audible artifacts during automation

**Fix:**
```cpp
// Add to header:
SmoothedValue<float> cutoffSmooth;

// In prepareToPlay:
cutoffSmooth.reset(sampleRate, 0.02); // 20ms smoothing
cutoffSmooth.setCurrentAndTargetValue(initialCutoff);

// In parameter callback:
cutoffSmooth.setTargetValue(newCutoff);

// In processBlock (per sample):
float cutoff = cutoffSmooth.getNextValue();
```

**If intentionally unsmoothed:**
Document why (e.g., instant mute, intentional glitch effect).
```

### Audit Finds Once-Per-Block Smoothing

**Symptom:** getNextValue called only once per block

**Fallback:**
```cpp
// PROBLEM: Once per block smoothing
// This causes zipper noise!

void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    auto cutoff = cutoffSmooth.getNextValue(); // WRONG: once per block

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        // cutoff is constant for entire block
        filter.setCutoff(cutoff); // Zipper noise!
    }
}

// FIX: Per-sample smoothing
void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        float cutoff = cutoffSmooth.getNextValue(); // RIGHT: per sample

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            // cutoff updated every sample
            filter.processSample(buffer.getWritePointer(channel)[sample], cutoff);
        }
    }
}
```

### Dead Parameter (Value Never Used)

**Symptom:** SmoothedValue declared and set but value never used

**Fallback:**
```markdown
**Dead parameter detected**

SmoothedValue: `[variable_name]` is set but never used.

**Possible causes:**
1. Parameter was removed but cleanup incomplete
2. Parameter connected to wrong DSP module
3. Copy-paste error created duplicate

**Check:**
1. Is the smoothed value used in processBlock?
2. Is it passed to the DSP module that should use it?
3. Is there a duplicate declaration?

**Fix:**
Either connect the parameter to its DSP module, or remove it entirely.
```

### Ramp Time Issues

**Symptom:** Ramp time too short (clicks) or too long (unresponsive)

**Fallback:**
```markdown
**Ramp time may be inappropriate**

Current: `[ramp_time]` for `[parameter_type]`

**Guidelines:**
| Parameter Type | Recommended Ramp |
|----------------|------------------|
| Gain/Volume | 20ms (0.02s) |
| Frequency | 20ms |
| Mix/Dry-Wet | 20ms |
| Bypass Toggle | 42ms |
| Mode Switch | 42ms |

**If ramp is:**
- `< 5ms`: May cause clicks
- `> 100ms`: May feel unresponsive
- `> 500ms`: Definitely too slow for real-time use

**Adjust:**
```cpp
smooth.reset(sampleRate, 0.02); // 20ms - standard
smooth.reset(sampleRate, 0.042); // 42ms - for toggles
```
```

### Incomplete Audit (Multiple Files)

**Symptom:** Cannot audit all source files

**Fallback:**
```markdown
**Partial SmoothedValue Audit**

Files audited: [count]
Files not audited: [list]

**Risk:** Unaudited files may contain smoothing issues.

**Recommendation:**
1. Complete audit before release
2. Priority files to audit:
   - PluginProcessor.cpp (main audio processing)
   - Any DSP module files used in processBlock

**Manual checks for unaudited files:**
1. Search for SmoothedValue declarations
2. Verify each has reset() in prepareToPlay
3. Verify each has setTargetValue() per block
4. Verify each has getNextValue() per sample
```