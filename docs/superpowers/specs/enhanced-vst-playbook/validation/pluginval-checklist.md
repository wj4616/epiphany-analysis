# Pluginval Validation Checklist

> Pre-release validation using pluginval at strictness level 5+.

---

## Setup

```bash
# Download pluginval
# Linux
wget https://github.com/Tracktion/pluginval/releases/download/latest/pluginval_Linux.zip
unzip pluginval_Linux.zip

# macOS
wget https://github.com/Tracktion/pluginval/releases/download/latest/pluginval_macOS.zip
unzip pluginval_macOS.zip

# Windows
wget https://github.com/Tracktion/pluginval/releases/download/latest/pluginval_Windows.zip
unzip pluginval_Windows.zip

# Run validation
./pluginval --strictness-level 5 --verbose /path/to/Plugin.vst3
```

---

## Validation Levels

| Level | What It Tests | Release Ready? |
|-------|---------------|----------------|
| 1-4 | Basic call coverage, crash testing | No |
| **5** | Host compatibility, state round-trip | **Minimum for release** |
| 6-9 | Extended parameter fuzzing, multiple state restoration | Recommended |
| 10 | Real-time safety checks, full stress testing | Maximum |

---

## Strictness Level 5 Tests

### State Round-Trip
- [ ] `getStateInformation()` produces valid data
- [ ] `setStateInformation()` restores all parameters
- [ ] State round-trip preserves all values
- [ ] State round-trip works after parameter changes
- [ ] State round-trip works after multiple saves

### Parameter Tests
- [ ] All parameters have valid IDs
- [ ] All parameters have valid names
- [ ] All parameters are automatable (or marked non-automatable)
- [ ] Parameter IDs don't change between versions

### Editor Tests
- [ ] Editor opens without crash
- [ ] Editor closes without crash
- [ ] Editor reopens without crash (10 cycles)
- [ ] Editor resize works correctly

### Audio Processing Tests
- [ ] processBlock works with silence
- [ ] processBlock works with noise
- [ ] processBlock works with various buffer sizes
- [ ] processBlock works at various sample rates

---

## Strictness Level 6-9 Tests

### Extended State Tests
- [ ] Multiple state restorations work
- [ ] State restoration after parameter changes
- [ ] State restoration after preset changes

### Parameter Fuzzing
- [ ] Random parameter values don't crash
- [ ] Extreme parameter values don't crash
- [ ] Parameter automation works correctly

---

## Strictness Level 10 Tests

### Real-Time Safety
- [ ] No allocations in processBlock
- [ ] No locks in processBlock
- [ ] No blocking I/O in processBlock
- [ ] All buffers pre-allocated in prepareToPlay

### Stress Testing
- [ ] Extended processing (60 seconds)
- [ ] Multiple instances simultaneously
- [ ] Rapid open/close cycles
- [ ] Rapid parameter changes

---

## Common Failure Modes

| Failure | Cause | Fix |
|---------|-------|-----|
| State round-trip fails | Empty getStateInformation | Implement APVTS serialization |
| Editor crash on reopen | Dangling listener | Match addListener with removeListener |
| Parameter ID mismatch | Changed parameter IDs | Never change IDs after release |
| Allocation in processBlock | new/delete in audio thread | Pre-allocate in prepareToPlay |
| Lock in processBlock | Mutex in audio thread | Use atomic flags |

---

## Pre-Release Checklist

Run this before every release:

```
[ ] pluginval strictness 5+ passes on VST3
[ ] pluginval strictness 5+ passes on AU (if applicable)
[ ] State save/restore round-trips correctly
[ ] Tested at 44.1kHz, 48kHz, 96kHz sample rates
[ ] Tested at 64, 128, 256, 512, 1024, 2048 buffer sizes
[ ] Process 60s silence — CPU stays low (denormal check)
[ ] Process full-scale noise — no NaN/Inf in output
[ ] Multiple instances simultaneously don't crash
[ ] Editor open/close 10 times without crash
[ ] Automation record/playback works for all parameters
[ ] DAW bypass produces clean dry signal
[ ] No DBG() calls in Release build
[ ] No static mutable state
[ ] All addListener calls have matching removeListener in destructor
[ ] Output never exceeds 0dBFS (limiter/tanh present)
```

---

## Platform-Specific Notes

### Linux
- Path: `~/.vst3/`
- No code signing required
- Test in REAPER

### macOS
- Path: `/Library/Audio/Plug-Ins/VST3/` or `~/Library/Audio/Plug-Ins/VST3/`
- AU Path: `/Library/Audio/Plug-Ins/Components/` or `~/Library/Audio/Plug-Ins/Components/`
- Code signing required
- Notarization required for 10.15+
- Reset AU cache: `killall -9 AudioComponentRegistrar`

### Windows
- Path: `C:\Program Files\Common Files\VST3\`
- Statically link MSVC runtime
- No code signing required (but recommended)

---

## Results Template

```
Pluginval Validation Report
===========================
Date: [DATE]
Plugin: [NAME]
Version: [VERSION]
Strictness: 5

Results:
[ ] State round-trip: PASS
[ ] Parameter tests: PASS
[ ] Editor tests: PASS
[ ] Audio processing: PASS

Issues Found:
- [Issue 1]
- [Issue 2]

Resolution:
- [Fix 1]
- [Fix 2]

Final Status: [PASS/FAIL]
```