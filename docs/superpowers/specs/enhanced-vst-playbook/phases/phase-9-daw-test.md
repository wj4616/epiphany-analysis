# Phase 9: DAW Testing

> Human perceptual testing — this phase is **HUMAN-ONLY**.

## Overview

| Attribute | Value |
|-----------|-------|
| **Phase** | 9 |
| **Name** | DAW Testing |
| **Role Mindset** | Tester |
| **Duration** | 1-2 sessions |
| **Criticality** | **CRITICAL** — Perceptual bugs cannot be caught by code review |

## Purpose

Human listening tests in actual DAW environment. This phase catches perceptual and UX issues that no automated test can detect.

## Critical Insight

> EM-13, EM-14, and EM-15 were caught by the **USER in DAW testing**, not by automated review.
> 
> Two-stage code review catches implementation bugs but **CANNOT** catch perceptual/UX issues.
> 
> **Human DAW testing is irreplaceable.**

## Pre-Check

Before starting this phase, verify:
- [ ] Phase 8 gate passed
- [ ] Builds available for target platforms
- [ ] pluginval strictness 5+ passes
- [ ] State round-trip test passes

## Test Environment

| Requirement | Value |
|-------------|-------|
| Primary DAW | REAPER (Linux/Windows/macOS) |
| Secondary DAW | Ableton Live, Logic Pro |
| Sample Rates | 44.1kHz, 48kHz, 96kHz |
| Buffer Sizes | 64, 128, 256, 512, 1024, 2048 |
| Instance Count | 1, 2, 4+ |

## Tasks

### Task 9.1: Load in DAW

**Owner**: Tester (Human)

**Description**: Load plugin in target DAW(s) and verify basic functionality.

**Checklist**:
- [ ] Plugin appears in DAW plugin list
- [ ] Plugin loads without crash
- [ ] UI opens without crash
- [ ] All parameters visible and editable
- [ ] No console errors

---

### Task 9.2: Control Responsiveness

**Owner**: Tester (Human)

**Description**: Verify all UI controls respond correctly.

**Checklist**:
- [ ] All knobs/sliders respond to mouse input
- [ ] All knobs/sliders respond to automation
- [ ] All buttons toggle correctly
- [ ] All dropdowns work
- [ ] All text editors work
- [ ] No parameter changes on open/close

---

### Task 9.3: Mix Range Test

**Owner**: Tester (Human)

**Description**: Verify dry/wet mix knob works across full range.

**Checklist**:
- [ ] Mix at 0% = dry signal only
- [ ] Mix at 50% = balanced mix (no volume dip)
- [ ] Mix at 100% = wet signal only
- [ ] No click/pop when moving mix knob
- [ ] Mix automation works correctly

**Note**: Use **equal-power mixing** (cosine/sine) to avoid volume dip at 50%.

---

### Task 9.4: Preset Audition

**Owner**: Tester (Human)

**Description**: Load and audition each preset. **Listen carefully.**

**Checklist**:
- [ ] Each preset loads correctly
- [ ] Each preset sounds correct for its name
- [ ] No unexpected noise/artifacts
- [ ] Preset transitions are smooth
- [ ] Factory presets match sound identity

**Sound Identity Verification**:
- Does "Warm Pad" sound warm?
- Does "Bright Lead" sound bright?
- Does "Psychedelic Ambient" sound psychedelic?
- (Reference sound-design-kb/bridges for expected character)

---

### Task 9.5: Automation Test

**Owner**: Tester (Human)

**Description**: Record and play back automation for all parameters.

**Checklist**:
- [ ] Record automation for each parameter
- [ ] Play back automation — parameters move correctly
- [ ] No zipper noise on automation
- [ ] Automation curves are smooth
- [ ] Parameter IDs match between sessions

---

### Task 9.6: Bypass Test

**Owner**: Tester (Human)

**Description**: Test DAW bypass functionality.

**Checklist**:
- [ ] DAW bypass produces clean dry signal
- [ ] No click/pop on bypass toggle
- [ ] No click/pop on bypass release
- [ ] Bypass state preserved between sessions

**Note**: Use ~42ms crossfade for bypass to avoid clicks.

---

### Task 9.7: Silence Test (Denormal Check)

**Owner**: Tester (Human)

**Description**: Process 60 seconds of silence and monitor CPU.

**Checklist**:
- [ ] Play 60 seconds of silence through plugin
- [ ] Monitor CPU usage throughout
- [ ] CPU should remain low (no denormal explosion)
- [ ] No noise accumulated in feedback loops

**Note**: Denormal explosion shows as CPU spike on silence.

---

### Task 9.8: Full-Scale Noise Test

**Owner**: Tester (Human)

**Description**: Process full-scale noise and check for NaN/Inf.

**Checklist**:
- [ ] Process full-scale white noise
- [ ] No NaN/Inf in output
- [ ] No clipping without warning
- [ ] Output never exceeds 0dBFS (limiter present)

---

### Task 9.9: Phase Gate

**Owner**: Coordinator

**Description**: Verify all gate conditions before proceeding.

**Gate Conditions**:
- [ ] Plugin loads in target DAW(s)
- [ ] All controls respond correctly
- [ ] Mix knob effective across full range
- [ ] All presets sound correct (verified by listening)
- [ ] Automation works
- [ ] Bypass produces clean dry signal
- [ ] 60s silence: CPU stays low
- [ ] Full-scale noise: no NaN/Inf

**Blocker Examples**:
- "Preset sounds harsh when it should be warm" — verify sound identity
- "Mix knob causes volume dip at 50%" — fix equal-power mixing
- "CPU spikes on silence" — fix denormal handling

---

## KB Lookups

| KB | Entry | Purpose |
|----|-------|---------|
| testing-kb/manual | DAW testing checklist | Test procedures |
| sound-design-kb | Preset validation | Expected sound character |
| failure-modes-kb | Common issues | Known failure modes |

## Skills

- `juce-daw-testing`: Skill for DAW testing (human testing assistance)

## Test Checklist Summary

```
DAW Testing Checklist:
[ ] 1. Plugin loads in DAW
[ ] 2. All controls respond
[ ] 3. Mix range works (0% dry, 100% wet, 50% balanced)
[ ] 4. All presets auditioned and approved
[ ] 5. Automation records and plays back
[ ] 6. Bypass produces clean dry signal
[ ] 7. 60s silence: CPU stays low
[ ] 8. Full-scale noise: no NaN/Inf
[ ] 9. Multiple instances don't interfere
[ ] 10. Open/close cycles don't crash
```

## Output Artifacts

| Artifact | Purpose |
|----------|---------|
| Test report | All test results |
| Bug list | Issues found during testing |
| Preset validation | Approved presets |

## Common Failure Modes

| FM | Risk | Detection |
|----|------|-----------|
| EM-13 | SmoothedValue not advanced | Silent parameter, zipper noise |
| EM-14 | Linear mix instead of equal-power | Volume dip at 50% |
| EM-15 | Default decay too long | Parameter changes masked |

## Next Phase

Phase 10: Market Preparation — Prepare for commercial release