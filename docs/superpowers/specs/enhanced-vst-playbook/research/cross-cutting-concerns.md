# Cross-Cutting Concerns

> Concerns that touch every phase of VST plugin development, not isolated to a single phase.

---

## 1. Audio Thread Safety

**Affects**: All phases (especially 4, 6, 7)

### Rules

| Rule | Rationale | Enforcement |
|------|-----------|--------------|
| No allocations in processBlock | Real-time deadline violation | Code audit + pluginval |
| No locks/mutexes in processBlock | Priority inversion causes dropouts | Code audit + static analysis |
| No blocking I/O in processBlock | Violates real-time constraints | Code audit |
| Use atomic<bool> for cross-thread flags | Safe message passing | Pattern enforcement |
| Pre-allocate all buffers in prepareToPlay | No runtime allocations | Code audit |

### Enforcement Points

- **Phase 4**: DSP module review
- **Phase 6**: Processor integration audit
- **Phase 9**: pluginval testing
- **Phase 11**: CPU profiling

### Verification

```
[ ] No new/delete in processBlock
[ ] No locks in processBlock
[ ] All buffers pre-allocated
[ ] Atomic flags used for cross-thread
[ ] ScopedNoDenormals at processBlock start
```

---

## 2. State Persistence

**Affects**: Phases 5, 6, 8, 9

### Requirements

| Requirement | Enforcement |
|-------------|--------------|
| All parameters save/restore | APVTS state serialization |
| Presets save/restore | PresetManager implementation |
| State round-trips correctly | pluginval test |
| Automation preserves state | DAW testing |

### Enforcement Points

- **Phase 5**: PresetData design
- **Phase 6**: getStateInformation/setStateInformation
- **Phase 8**: State round-trip test
- **Phase 9**: Automation test

### Verification

```
[ ] APVTS state serialization implemented
[ ] PresetManager saves/loads correctly
[ ] pluginval state round-trip passes
[ ] Save DAW session, reopen, verify all parameters
[ ] Automation record/playback works
```

---

## 3. Parameter Smoothing

**Affects**: Phases 4, 6, 9

### Rules

| Rule | Enforcement |
|------|--------------|
| All continuous parameters smooth | SmoothedValue pattern |
| Call getNextValue() every sample | Code audit |
| Reset in prepareToPlay | Code audit |
| 20ms for continuous, 42ms for bypass | Pattern |

### Enforcement Points

- **Phase 4**: SmoothedValue for all parameters
- **Phase 6**: SmoothedValue audit
- **Phase 9**: Zipper noise testing

### Verification

```
[ ] All continuous parameters use SmoothedValue
[ ] getNextValue() called per sample (not getCurrentValue)
[ ] reset() called in prepareToPlay
[ ] No zipper noise on automation
[ ] Bypass crossfade is smooth
```

---

## 4. Mono/Stereo Handling

**Affects**: Phases 4, 6, 8

### Rules

| Rule | Enforcement |
|------|--------------|
| Handle mono input gracefully | Copy channel 0 to 1 |
| Stereo processing for stereo input | Process both channels |
| Don't assume channel count | Use buffer.getNumChannels() |

### Enforcement Points

- **Phase 4**: DSP module design
- **Phase 6**: processBlock implementation
- **Phase 8**: Mono/stereo testing

### Verification

```
[ ] Mono input doesn't produce silent right channel
[ ] Stereo input processed correctly
[ ] Plugin works with mono tracks
[ ] Plugin works with stereo tracks
```

---

## 5. Sample Rate Independence

**Affects**: Phases 4, 6, 8, 11

### Rules

| Rule | Enforcement |
|------|--------------|
| All coefficients recalculated in prepareToPlay | Code audit |
| Buffer sizes scale with sample rate | Code audit |
| Test at 44.1k, 48k, 96k, 192k | Validation |

### Enforcement Points

- **Phase 4**: DSP module prepare() methods
- **Phase 6**: prepareToPlay implementation
- **Phase 8**: Sample rate stress test
- **Phase 11**: Denormal stress test

### Verification

```
[ ] Filters sound correct at all sample rates
[ ] Delay times accurate at all rates
[ ] Buffer sizes don't overflow at 192kHz
[ ] processBlock tested at multiple rates
```

---

## 6. Buffer Size Independence

**Affects**: Phases 4, 6, 8

### Rules

| Rule | Enforcement |
|------|--------------|
| Never assume fixed block size | Use buffer.getNumSamples() |
| Don't cache samplesPerBlock | Use actual size each call |

### Enforcement Points

- **Phase 4**: DSP module design
- **Phase 6**: processBlock implementation
- **Phase 8**: Buffer size stress test

### Verification

```
[ ] Works at 64 samples
[ ] Works at 128 samples
[ ] Works at 256 samples
[ ] Works at 512 samples
[ ] Works at 1024 samples
[ ] Works at 2048 samples
```

---

## 7. Multi-Instance Isolation

**Affects**: Phases 4, 6, 8

### Rules

| Rule | Enforcement |
|------|--------------|
| No static mutable state | Code audit |
| No singletons | Code audit |
| All state as instance members | Code audit |

### Enforcement Points

- **Phase 4**: Module design
- **Phase 6**: Processor design
- **Phase 8**: Multi-instance test

### Verification

```
[ ] No static mutable variables
[ ] No singletons
[ ] 4+ instances can run without interference
[ ] Each instance has isolated state
```

---

## 8. Gain Staging

**Affects**: Phases 4, 6, 9

### Rules

| Rule | Enforcement |
|------|--------------|
| Output never exceeds 0dBFS | Limiter or tanh at output |
| Feedback clamped to <1.0 | Code audit |
| Soft clipping in feedback paths | Code audit |

### Enforcement Points

- **Phase 4**: DSP module design
- **Phase 6**: Output limiter
- **Phase 9**: Full-scale noise test

### Verification

```
[ ] Output limiter present
[ ] Feedback parameter clamped to 0.95
[ ] Soft clip in feedback paths
[ ] Process full-scale noise - no NaN/Inf
```

---

## 9. Equal-Power Mixing

**Affects**: Phases 4, 6

### Rules

| Rule | Enforcement |
|------|--------------|
| Dry/wet uses cosine/sine | Code audit |
| No linear crossfade for dry/wet | Pattern |

### Formula

```cpp
float dryGain = std::cos(mix * juce::MathConstants<float>::halfPi);
float wetGain = std::sin(mix * juce::MathConstants<float>::halfPi);
```

### Enforcement Points

- **Phase 4**: DSP module design
- **Phase 6**: processBlock implementation

### Verification

```
[ ] Dry/wet uses equal-power mixing
[ ] No volume dip at 50% mix
[ ] Mix knob is effective across full range
```

---

## 10. Parameter ID Stability

**Affects**: Phases 6, 8, 10

### Rules

| Rule | Enforcement |
|------|--------------|
| Never change parameter IDs after release | Spec lock |
| IDs are hashed strings in VST3 | Knowledge |
| New parameters get new IDs | Design |

### Enforcement Points

- **Phase 6**: Parameter layout
- **Phase 10**: Release notes

### Verification

```
[ ] Parameter IDs locked before v1.0
[ ] Automation preserved across versions
[ ] No ID changes after release
```

---

## 11. GUI Thread Safety

**Affects**: Phases 7, 9

### Rules

| Rule | Enforcement |
|------|--------------|
| No heavy work in parameter callback | Pattern |
| Set atomic flag, check in Timer | Pattern |
| repaint() never called from audio thread | Code audit |
| Every addListener has matching removeListener | Code audit |

### Enforcement Points

- **Phase 7**: GUI component design
- **Phase 9**: Editor open/close test

### Verification

```
[ ] Parameter callbacks set atomic flags only
[ ] Timer checks flags for UI updates
[ ] No repaint() from audio thread
[ ] Editor open/close 10 times without crash
```

---

## 12. Platform Compatibility

**Affects**: Phases 3, 8, 10

### Rules

| Rule | Enforcement |
|------|--------------|
| Test on Linux, Windows, macOS | CI or manual |
| VST3 works on all platforms | Validation |
| AU for macOS | Platform-specific |
| AAX for Pro Tools | Platform-specific |

### Enforcement Points

- **Phase 3**: CMake configuration
- **Phase 8**: Multi-platform build test
- **Phase 10**: Distribution packaging

### Verification

```
[ ] Builds on Linux
[ ] Builds on Windows
[ ] Builds on macOS
[ ] VST3 works on all platforms
[ ] AU works on macOS (if applicable)
[ ] AAX works in Pro Tools (if applicable)
```

---

## Cross-Cutting Concern Matrix

| Concern | Phase 0 | Phase 4 | Phase 6 | Phase 7 | Phase 8 | Phase 9 | Phase 11 |
|---------|---------|---------|---------|---------|---------|---------|----------|
| Audio thread safety | - | ✓ | ✓ | - | - | ✓ | ✓ |
| State persistence | ✓ | - | ✓ | - | ✓ | ✓ | - |
| Parameter smoothing | - | ✓ | ✓ | - | - | ✓ | - |
| Mono/stereo | ✓ | ✓ | ✓ | - | ✓ | - | - |
| Sample rate | ✓ | ✓ | ✓ | - | ✓ | - | ✓ |
| Buffer size | - | ✓ | ✓ | - | ✓ | - | - |
| Multi-instance | - | ✓ | ✓ | - | ✓ | - | - |
| Gain staging | - | ✓ | ✓ | - | - | ✓ | - |
| Equal-power mix | - | ✓ | ✓ | - | - | ✓ | - |
| Parameter IDs | ✓ | - | ✓ | - | - | ✓ | ✓ |
| GUI thread safety | - | - | - | ✓ | - | ✓ | - |
| Platform | ✓ | - | - | - | ✓ | ✓ | - |

---

## Sources

- `/home/myuser/agents/juce-agent/playbookdata/references/juce-failure-modes.md`
- `/home/myuser/agents/juce-agent/playbookdata/references/juce-patterns-reference.md`
- `/home/myuser/agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/`
- Epiphany Machine v3 session analysis