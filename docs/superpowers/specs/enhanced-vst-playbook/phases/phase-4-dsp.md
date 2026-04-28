# Phase 4: DSP Implementation

> Build the audio engine with two-stage code review.

## Overview

| Attribute | Value |
|-----------|-------|
| **Phase** | 4 |
| **Name** | DSP Implementation |
| **Role Mindset** | Implementer |
| **Duration** | Multiple sessions |
| **Criticality** | Core functionality |

## Purpose

Implement all DSP modules with two-stage code review for each module. This phase builds the actual audio engine.

## Pre-Check

Before starting this phase, verify:
- [ ] Phase 3 gate passed
- [ ] Project builds
- [ ] Architecture documented
- [ ] Signal flow diagram complete
- [ ] All DSP algorithms specified

## Two-Stage Review Process

**Critical**: Every DSP module must pass two-stage review:

| Stage | Purpose | What It Checks |
|-------|---------|----------------|
| **Stage 1** | Spec Compliance | Correct algorithm, formula constants, parameter ranges, signal flow |
| **Stage 2** | Code Quality | Buffer overflows, off-by-one errors, thread safety, edge cases, performance |

**Key Insight**: Two-stage review catches bugs that either stage alone misses. Code quality reviewer caught EM-03 through EM-10 that spec review alone missed.

## Tasks

### Task 4.1-N: Implement DSP Module N

**Owner**: Implementer

**Description**: Implement each DSP module with the following steps:

1. Create module class
2. Implement process() method
3. Implement prepare() method
4. Implement parameter setters
5. Write unit tests

**Output**: Module code + tests

**For each module, complete Stage 1 review before Stage 2.**

---

### Task 4.N+1: Stage 1 Review (Spec Compliance)

**Owner**: Reviewer

**Description**: Verify implementation matches specification.

**Checklist**:
- [ ] Algorithm matches spec description
- [ ] Formula constants match spec
- [ ] Parameter ranges match spec
- [ ] Signal flow matches diagram
- [ ] Edge cases handled

**Pass Criteria**: All checklist items pass

---

### Task 4.N+2: Stage 2 Review (Code Quality)

**Owner**: Reviewer

**Description**: Verify code quality independent of spec.

**Checklist**:
- [ ] No buffer overflows
- [ ] No off-by-one errors
- [ ] No allocations in process()
- [ ] Thread safety verified
- [ ] Denormals handled
- [ ] SmoothedValue correct usage
- [ ] Edge cases handled

**Pass Criteria**: All checklist items pass

---

### Task 4.Final: Phase Gate

**Owner**: Coordinator

**Description**: Verify all gate conditions before proceeding.

**Gate Conditions**:
- [ ] All DSP modules implemented
- [ ] All modules pass two-stage review
- [ ] Audio thread safety verified (no allocations/locks in processBlock)
- [ ] SmoothedValue audit passed (getNextValue() called every sample)
- [ ] Output limiter present (no clipping above 0dBFS)
- [ ] Feedback paths clamped to < 1.0

**Blocker Examples**:
- "SmoothedValue getNextValue() never called" — EM-13, must fix
- "Linear mix instead of equal-power" — EM-14, must fix
- "Buffer overflow at high sample rates" — EM-04, must fix

---

## KB Lookups

| KB | Entry | Purpose |
|----|-------|---------|
| dsp-kb/filters | lowpass, highpass, bandpass | Filter implementations |
| dsp-kb/oscillators | sawtooth, square, wavetable | Oscillator implementations |
| dsp-kb/effects | delay, reverb, distortion | Effect implementations |
| cpp-kb/realtime-safety | atomic-operations, lock-free | Thread safety patterns |
| failure-modes-kb/audio-thread | allocation, locking, denormals | Common pitfalls |

## Skills

- `juce-dsp-implementation`: Skill for DSP implementation
- `juce-audio-thread-audit`: Skill for audio thread safety verification
- `juce-smoothedvalue-audit`: Skill for SmoothedValue verification

## Audio Thread Safety Rules

**NEVER in processBlock**:
- `new`, `delete`, `malloc`, `free`
- `std::vector::push_back`, `std::string` operations
- `DBG()`, `std::cout`, console I/O
- Locks, mutexes, condition variables
- File I/O, network I/O
- Any blocking operation

**ALWAYS in processBlock**:
- Pre-allocated buffers (use `prepareToPlay()`)
- `std::atomic<bool>` for cross-thread flags
- `ScopedNoDenormals` at start
- `getNextValue()` per sample for SmoothedValue

## SmoothedValue Pattern

```cpp
// In prepareToPlay()
smoothedValue.reset(sampleRate, 0.02);
smoothedValue.setCurrentAndTargetValue(initialValue);

// In processBlock (once per block)
smoothedValue.setTargetValue(newValue);

// In processBlock (per sample)
for (int i = 0; i < numSamples; ++i) {
    float value = smoothedValue.getNextValue();  // MUST call every sample
    // use value...
}
```

## Equal-Power Mixing Pattern

```cpp
// For dry/wet mixing - ALWAYS use equal-power
float dryGain = std::cos(mix * juce::MathConstants<float>::halfPi);
float wetGain = std::sin(mix * juce::MathConstants<float>::halfPi);
```

## Output Artifacts

| Artifact | Purpose |
|----------|---------|
| DSP module files | Implemented audio engine |
| Unit tests | Module verification |
| Review reports | Two-stage review documentation |

## Common Failure Modes

| FM | Risk | Prevention |
|----|------|------------|
| FM-01 | Heap allocation in processBlock | Code review checklist |
| FM-04 | Denormal CPU explosion | ScopedNoDenormals |
| FM-05 | Feedback loop instability | Clamp feedback to 0.95 |
| FM-09 | SmoothedValue not advanced | Call getNextValue() every sample |
| FM-13 | Aliasing in distortion | Use oversampling |
| EM-04 | Buffer overflow at high rates | Scale buffers by sampleRate/48000 |
| EM-13 | SmoothedValue never advanced | Code review checklist |

## Next Phase

Phase 5: State & Presets — Implement state management and presets