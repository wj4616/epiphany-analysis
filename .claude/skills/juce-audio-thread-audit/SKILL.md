---
name: juce-audio-thread-audit
description: Use BEFORE DSP implementation or AFTER code review to verify audio thread safety. Checks for allocations, locks, and other violations of real-time constraints. Based on FM-01 through FM-07 failure modes.
---

# JUCE Audio Thread Safety Audit

Critical audit for real-time audio thread constraints. Run this before DSP implementation (as a checklist) and after code review (as verification).

**Playbook Reference:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v6-unified.json` — failure_modes.audio_thread_safety

## When to Use

- **Before Phase 4:** As a pre-flight checklist for DSP implementation
- **After Phase 4:** As verification after code review
- **Before release:** As a pre-release validation step
- **When debugging:** When investigating audio glitches or CPU spikes

## Quick Audit Checklist

Run through this list for any `processBlock` implementation:

```
processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiBuffer) {
    // Line 1: ScopedNoDenormals MUST be first
    juce::ScopedNoDenormals noDenormals;

    // NO allocations in this function:
    // - No new, delete, malloc
    // - No std::vector::push_back
    // - No std::string construction
    // - No DBG() macro (it allocates)
    // - No std::unordered_map::find() with string keys

    // NO locks in this function:
    // - No std::mutex
    // - No juce::CriticalSection
    // - No juce::SpinLock

    // NO blocking operations:
    // - No file I/O
    // - No network calls
    // - No sleep or wait

    // Pre-allocated buffers used here:
    // - All buffers sized in prepareToPlay()
    // - All state variables are members (not local)

    // SmoothedValue pattern:
    // - setTargetValue() called once per block (NOT per sample)
    // - getNextValue() called per sample (NOT getCurrentValue)
}
```

## Detailed Checks

### FM-01: Heap Allocation on Audio Thread

**Symptom:** Audio dropouts, glitches, occasional crashes

**What to search for:**
```bash
# In processBlock, search for:
new|delete|malloc|std::vector::push_back|std::string|DBG\(|insert\(|emplace\(
```

**Correct pattern:**
```cpp
class MyProcessor {
    // Pre-allocated in prepareToPlay
    juce::AudioBuffer<float> workBuffer;
    std::vector<float, juce::AlignedAllocator<float>> coefficients;

    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        workBuffer.setSize(2, samplesPerBlock);
        coefficients.resize(1024);  // Pre-allocate
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
        // Use pre-allocated buffers - NO allocations here
        auto workBlock = juce::dsp::AudioBlock<float>(workBuffer);
    }
};
```

### FM-02: Buffer Overflow at High Sample Rates

**Symptom:** Crash or memory corruption at 96kHz or 192kHz

**What to check:**
- All buffer sizes computed from `sampleRate` parameter
- Delay lines: `maxDelaySamples = maxDelayTime * sampleRate` where `sampleRate` could be 192000

**Correct pattern:**
```cpp
void prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Scale for maximum expected rate
    const double maxRate = 192000.0;
    const double scaleFactor = maxRate / 48000.0;

    delayBuffer.setSize(2, static_cast<int>(baseBufferSize * scaleFactor));
}
```

### FM-03: Empty State Save/Restore

**Symptom:** All parameters reset to defaults when DAW session reopened

**What to check:**
```cpp
// WRONG - empty stubs
void getStateInformation(juce::MemoryBlock&) override {}
void setStateInformation(const void*, int) override {}

// CORRECT - implement properly
void getStateInformation(juce::MemoryBlock& dest) override {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}

void setStateInformation(const void* data, int size) override {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
```

### FM-04: Denormal CPU Explosion

**Symptom:** CPU spikes to 100% on silence or very quiet signals

**What to check:**
```cpp
// FIRST LINE of processBlock:
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
    juce::ScopedNoDenormals noDenormals;  // MUST be first
    // ... rest of processing
}

// In feedback paths, add small DC offset:
for (int i = 0; i < numSamples; ++i) {
    float sample = /* processing */;
    sample += 1e-18f;  // Prevent denormals in feedback
    feedbackBuffer[i] = sample;
}
```

### FM-05: Feedback Loop Instability

**Symptom:** Audio grows to infinity, NaN/Inf in output, loud noise

**What to check:**
```cpp
// FEEDBACK PARAMETER: Never allow 1.0
feedback = std::clamp(feedbackParam, 0.0f, 0.95f);  // Max 0.95

// OUTPUT: Always have safety limiter
output = std::tanh(output);  // Soft clip
// OR
output = output / (1.0f + std::abs(output));  // Soft clip alternative
```

### FM-06: Dangling Listener

**Symptom:** Crash when plugin editor closed and reopened

**What to check:**
```cpp
class MyEditor : public juce::AudioProcessorEditor {
public:
    MyEditor(MyProcessor& p) : processor(p) {
        processor.apvts.addParameterListener("gain", this);
    }

    ~MyEditor() override {
        // MUST remove in destructor
        processor.apvts.removeParameterListener("gain", this);
    }
};
```

### FM-07: Thread Safety Violation in Parameter Callback

**Symptom:** Intermittent crashes, data races, UI freezes

**What to check:**
```cpp
// WRONG - calling repaint() from parameter callback
void parameterChanged(const juce::String& id, float value) override {
    repaint();  // DANGER - can be called from audio thread!
}

// CORRECT - use atomic flag
std::atomic<bool> parameterChanged{false};

void parameterChanged(const juce::String& id, float value) override {
    parameterChanged.store(true);  // Atomic - safe
}

void timerCallback() override {
    if (parameterChanged.exchange(false)) {
        repaint();  // Now on message thread
    }
}
```

**Critical:** NEVER use `juce::AsyncUpdater` from audio thread — it is NOT lock-free. Use `std::atomic<bool>` + Timer or VBlankAttachment instead.

## Audit Output Format

After running the audit, output:

```
## Audio Thread Safety Audit Report

### FM-01: Heap Allocation
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-02: Buffer Overflow
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-03: State Save/Restore
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-04: Denormal Protection
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-05: Feedback Stability
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-06: Listener Cleanup
Status: [PASS/FAIL]
Details: [Any issues found]

### FM-07: Thread Safety
Status: [PASS/FAIL]
Details: [Any issues found]

### Summary
- Total issues: [count]
- Critical: [count]
- Recommended fixes: [list]
```

## Pre-Release Checklist

Before release, verify ALL items pass:

- [ ] No heap allocations in processBlock (pluginval tests this)
- [ ] Buffer sizes computed for 192kHz
- [ ] State save/restore implemented and tested
- [ ] ScopedNoDenormals at top of processBlock
- [ ] Feedback clamped to max 0.95
- [ ] Output limiter present
- [ ] All addListener calls have matching removeListener in destructor
- [ ] Parameter callbacks use atomic flags, not direct GUI updates
- [ ] pluginval at strictness 5+ passes

## Fallback and Error Handling

### Cannot Read Source Files

**Symptom:** Source files not accessible or don't exist

**Fallback:**
```markdown
**Audit cannot proceed without source files.**

Required: Access to all source files (header and implementation).

**Options:**
1. Verify file paths are correct
2. Check that files exist at expected locations
3. Provide correct paths to source files

Audio thread safety cannot be verified without code review.
```

### Incomplete Code Review

**Symptom:** Cannot complete all audit checks

**Fallback:**
```markdown
**Partial Audit Report**

The following checks could not be completed:
- [List incomplete checks]

**Reason:** [Explanation]

**Risk:** Audio thread issues may exist in unchecked areas.

**Recommendation:**
- Complete audit before release
- Use pluginval for automated testing of available checks
- Manual testing in DAW with CPU-intensive project
```

### Unknown Pattern

**Symptom:** Code pattern not covered by FM-01 to FM-07

**Fallback:**
```markdown
**Unknown Audio Thread Pattern**

Pattern found: [Description]

This pattern is not covered by standard failure modes FM-01 through FM-07.

**Analysis:**
- Consider if this pattern could block the audio thread
- Consider if this pattern could allocate memory
- Consider if this pattern could cause undefined behavior

**Recommendation:**
- Document the pattern for future reference
- Consider adding to failure modes list
- Test the pattern with CPU-intensive DAW project

If unsure, treat as potentially unsafe and investigate.
```

### pluginval Unavailable

**Symptom:** Cannot run pluginval validation

**Fallback:**
```markdown
**pluginval not available**

Manual checklist for audio thread safety:

1. **Heap allocation check:**
   - Search `processBlock` for: `new`, `std::vector`, `std::string`, `juce::String`
   - Search `processBlock` for: `push_back`, `emplace_back`, `resize`

2. **Lock check:**
   - Search for: `lock()`, `tryLock()`, `std::mutex`, `CriticalSection`
   - In audio thread context, use `std::atomic` instead

3. **Buffer size check:**
   - Search for fixed-size arrays: `float buffer[1024]`
   - Verify sizes handle 192kHz at max block size

4. **Denormal check:**
   - Verify `ScopedNoDenormals` at top of `processBlock`

5. **Feedback check:**
   - Search for: `feedback`, `* 0.99` or similar
   - Verify max feedback < 1.0

These manual checks cover the automated pluginval checks.
```

### Audit Passes But Issues Found Later

**Symptom:** Audit passed but DAW testing finds audio issues

**Fallback:**
```markdown
**Audit limitation detected**

Audio thread audit covers static code patterns, not runtime behavior.

**Issues not caught by audit:**
- Race conditions that only occur under specific timing
- Issues triggered by specific parameter combinations
- Issues triggered by specific MIDI/audio input patterns
- CPU spikes from denormals in specific scenarios

**Additional testing needed:**
- Run pluginval at strictness 5+
- Test with CPU-intensive DAW project
- Test with automation on all parameters
- Test at extreme sample rates (192kHz)
- Test with specific MIDI sequences

Report findings to improve audit coverage.
```

## Related

- **C++ Knowledge Base:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md`
- **Lock-Free Patterns:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/lock-free-patterns.md`
- **Atomic Audio:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/atomic-audio.md`