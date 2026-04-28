# Gap Analysis: JUCE VST Plugin Development Playbook

Extracted from `conversation_full.md` — a single 24,292-line session (2026-03-27 to 2026-03-28) building Epiphany Machine VST v3.

---

## Phases with Thin Coverage

### 1. Phase 0: Plugin Concept & Spec
**Coverage: STRONG** — Session 1 has detailed brainstorming with 12 multi-choice design questions, all creative decisions documented.

**Gaps:**
- No evidence of user sketching GUI layout during concept phase — GUI was deferred to Phase 7
- No evidence of defining CPU budget quantitatively ("should run on a laptop" level)
- No evidence of target DAW selection during concept phase (user tested in a DAW later but no explicit "I'll test in Ableton/Reaper/etc.")

**Questions to fill holes:**
- What DAW(s) do you test in? This should be locked in Phase 0 so validation is repeatable.
- Do you have a CPU budget target (% of single core at 48kHz, 256 buffer)?

### 2. Phase 1: Performance Audit
**Coverage: STRONG** — Task 0 was thorough with 4 specific fixes, two-stage review caught a subtle cache bug.

**Gaps:**
- No profiling data (no Instruments/VTune/perf measurements cited)
- No before/after CPU measurement — fixes were based on code review, not profiling
- No memory leak check (valgrind or ASAN)

**Questions to fill holes:**
- Did you run any profiler on the v2 plugin before the audit?
- Is there a baseline CPU measurement for v2 vs v3?

### 3. Phase 3: Build Config
**Coverage: THIN** — Only CMakeLists.txt version bump and source file removal. No test framework setup.

**Missing validation steps:**
- No automated test framework mentioned anywhere in the session
- No CI/CD pipeline
- No static analysis (clang-tidy, cppcheck)

**Questions to fill holes:**
- Do you want automated tests? (Even just "does it compile" CI would catch regressions)
- Do you use any static analysis tools?

### 4. Phase 5: Preset Data
**Coverage: MODERATE** — Presets were defined with values but no evidence of listening validation for each preset.

**Gaps:**
- No session evidence of user auditioning each preset and approving the sound
- Preset values appear to be AI-generated based on spec descriptions, not tuned by ear
- No A/B comparison between v2 and v3 presets

**Questions to fill holes:**
- Did you listen to each of the 18 presets and approve them?
- Were any presets adjusted after initial AI generation?
- Do you have a preset validation workflow (load each one, play audio, evaluate)?

### 5. Phase 8: Build Verification
**Coverage: MODERATE** — Build succeeded, binary installed, integration review done. But:

**Missing validation steps:**
- No multi-format testing (only VST3, no AU/AAX/Standalone)
- No cross-platform testing (only Linux)
- No buffer size stress testing (64, 128, 256, 512, 1024, 2048)
- No sample rate stress testing (44.1k, 48k, 96k, 192k) despite buffer overflow fix being critical
- No multiple-instance test
- No state save/restore test (getStateInformation/setStateInformation are empty stubs!)

**Questions to fill holes:**
- Have you tested at 96kHz or 192kHz to verify the buffer overflow fix?
- Have you tested with different buffer sizes?
- Is state save/restore implemented? The session shows getStateInformation/setStateInformation as empty stubs.

### 6. Phase 9: DAW Testing
**Coverage: MODERATE** — User found 3 real bugs. But testing was informal.

**Missing validation steps:**
- No structured test checklist was followed (the plan had one but it wasn't explicitly executed)
- No automation test (record automation, play back, verify)
- No bypass test (DAW bypass vs plugin bypass)
- No stress test (process silence, process noise, extreme parameter values)
- No multiple instance test

**Questions to fill holes:**
- Did you complete the full smoke test checklist from Task 11 Step 3?
- Have you tested automation recording/playback?
- Have you tested with multiple instances simultaneously?

### 7. Phase 10: Release
**Coverage: THIN** — Binary copied to root, pushed to GitHub. But:

**Missing entirely:**
- No installer packaging (NSIS, Inno Setup, .pkg, DMG)
- No factory preset export as separate files
- No user documentation update for v3 changes
- No release notes
- No clean-system install test

**Questions to fill holes:**
- Do you distribute via GitHub release + .so download, or do you build installers?
- Is the v2 manual (epiphany-machine-VST-v2-manual-full.md) updated for v3?
- Do you want release notes generated from git log?

---

## Missing Phases (no session data)

### Optimization & Hardening (Playbook Phase 5 skeleton)
**Status: no session data — populate after next build**

No evidence of:
- CPU profiling with actual tools (Instruments, VTune, perf)
- Memory leak detection (valgrind, ASAN)
- Heap allocation audit of processBlock with runtime verification
- SIMD optimization consideration
- Before/after benchmarks

### Retrospective & Reusable Components (Playbook Phase 7 skeleton)
**Status: no session data — populate after next build**

No evidence of:
- Process retrospective documenting what worked / what didn't
- Extraction of reusable components as templates
- v2 scope definition

---

## Missing Validation Steps (across all phases)

| Validation | Phase | Status |
|---|---|---|
| Automated test framework | Phase 3 | Not implemented |
| State save/restore (getStateInformation) | Phase 6 | Empty stubs — NOT IMPLEMENTED |
| Multi-format build (AU, AAX, Standalone) | Phase 8 | Not tested (VST3 only) |
| Cross-platform build (macOS, Windows) | Phase 8 | Not tested (Linux only) |
| Sample rate stress test (96k, 192k) | Phase 8 | Not explicitly tested despite critical buffer overflow fix |
| Buffer size stress test (64-2048) | Phase 8 | Not tested |
| Multiple instance test | Phase 9 | Not tested |
| Automation record/playback test | Phase 9 | Not tested |
| DAW bypass test | Phase 9 | Not tested |
| Process silence for 60s (denormal check) | Phase 9 | Not tested |
| Process full-scale noise (NaN/inf check) | Phase 9 | Not tested |
| Preset audition/approval | Phase 5 | No evidence |
| Clean-system install test | Phase 10 | Not done |
| User documentation update | Phase 10 | Not done |

---

## Specific Questions to Ask User

### Creative / Sound
1. Did you listen to and approve each of the 18 presets? If not, which ones need tuning?
2. Are you satisfied with the equal-power mix curve, or do you want a different taper?
3. Is the 2.0s default decay responsive enough, or should it be shorter?
4. With DRIVE at max and FEEDBACK at max, does it sound right or does it get out of control?

### Technical
5. Have you tested at 96kHz? The buffer overflow fix was critical — did you verify it works?
6. Is state save/restore important to you? It's currently empty stubs (no preset persistence between DAW sessions).
7. Do you target only Linux, or do you also need macOS/Windows builds?
8. Do you want automated tests? Even a simple "does it compile" CI would prevent regressions.

### Process
9. What DAW(s) do you test in? (Never explicitly stated in session)
10. How do you distribute the plugin? (GitHub release page? .so direct download? Installer?)
11. Did you keep the export_claude_session.py script for future session analysis?
12. Do you plan to run the Mattermost playbook format, or is the JSON playbook sufficient?

---

## Session Citations

| Session | Lines | Time | Content |
|---|---|---|---|
| Session 1 | 1-2406 | 2026-03-27 07:00-07:35 | Brainstorming, design decisions, spec writing started |
| Session 2 | 2407-7371 | 2026-03-27 07:38-11:33 | Plan writing (interrupted by API 529 + context limits) |
| Session 3 | 7372-13071 | 2026-03-27 11:33-21:20 | Plan completion, Task 0 (perf audit), skill security review |
| Session 4 | 13072-17458 | 2026-03-27 21:21-05:13+1 | Tasks 1-9 implementation (DSP, presets, processor) |
| Session 5 | 17459-20534 | 2026-03-28 05:13-05:36 | Tasks 9 fixes, 10 (GUI), 11 (build), tag v3.0.0 |
| Session 6 | 20535-24292 | 2026-03-28 05:45-08:04 | DAW testing, bug fixes, GitHub push, playbook started |
