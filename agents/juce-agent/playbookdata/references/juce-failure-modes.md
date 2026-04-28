# JUCE Plugin Failure Modes & Prevention
## Compiled from: melatonin.dev, pluginval, awesome-juce, Epiphany Machine v3 session

Comprehensive database of known failure modes for JUCE VST plugin development. Organized by severity and phase.

---

## Critical (Crashes, Data Loss, Audio Corruption)

### FM-01: Heap allocation on audio thread
- **Symptom:** Audio dropouts, glitches, occasional crashes
- **Cause:** `new`, `delete`, `std::vector::push_back`, `juce::String`, `DBG()` in processBlock
- **Fix:** Pre-allocate all buffers in `prepareToPlay()`. Use `juce::SmoothedValue` instead of creating objects per sample.
- **Prevention:** Code review checklist item. Run pluginval (tests for this). Consider using `juce::ScopedNoDenormals` as a reminder anchor.
- **Detection:** pluginval strictness level 5+; Perfetto profiling shows allocation stalls

### FM-02: Buffer overflow at high sample rates
- **Symptom:** Crash or memory corruption at 96kHz or 192kHz
- **Cause:** Fixed-size buffers calculated for 48kHz; delay lines too short for higher rates
- **Fix:** Scale all buffer sizes by `sampleRate / 48000.0` or use maximum expected rate
- **Prevention:** Test at 44.1k, 48k, 96k, 192k. Calculate buffer sizes in `prepareToPlay()` from actual sample rate.
- **Epiphany Machine v3:** FDN delay line buffer was fixed at 16384 samples — safe up to 96kHz but could overflow at 192kHz with long delays

### FM-03: Empty getStateInformation / setStateInformation
- **Symptom:** All parameters reset to defaults when DAW session is reopened
- **Cause:** Forgetting to implement state save/restore (left as stubs)
- **Fix:** Implement APVTS state serialization (see patterns reference)
- **Prevention:** Plugin brief template requires state persistence decision. pluginval tests round-trip.

### FM-04: Denormal CPU explosion
- **Symptom:** CPU spikes to 100% when processing silence or very quiet signals
- **Cause:** Subnormal floating-point values in feedback loops or filters
- **Fix:** `juce::ScopedNoDenormals` at top of processBlock. DC offset in FDN feedback path.
- **Prevention:** Always include `ScopedNoDenormals`. Test by processing 60 seconds of silence and monitoring CPU.

### FM-05: Feedback loop instability
- **Symptom:** Audio grows to infinity, NaN/Inf in output, loud noise
- **Cause:** Feedback gain >= 1.0, or accumulation of gain through loop components
- **Fix:** Clamp feedback to 0.95 max. Add soft clip (`x / (1 + |x|)`) in feedback path. Apply `tanh()` at output.
- **Prevention:** Never allow feedback parameter to reach 1.0. Always have output limiter.

### FM-06: Dangling listener on editor re-open
- **Symptom:** Crash when plugin editor is closed and re-opened in DAW
- **Cause:** `addListener()` called without corresponding `removeListener()` in destructor
- **Fix:** Every `addListener` / `addChangeListener` must have a matching `remove*` in the destructor
- **Prevention:** pluginval tests editor open/close cycles. Search for addListener without matching remove.
- **Detection:** pluginval catches this reliably

### FM-07: Thread safety violation in parameter callback
- **Symptom:** Intermittent crashes, data races, UI freezes
- **Cause:** Calling `repaint()`, heavy computation, or non-atomic operations from `parameterChanged()` (which can run on audio thread)
- **Fix:** Set `std::atomic<bool>` flag in callback, check in `Timer` or `VBlankAttachment`
- **Prevention:** Never do anything non-trivial in parameter callbacks

---

## Serious (Incorrect Audio, Plugin Rejection)

### FM-08: Missing mono-to-stereo handling
- **Symptom:** Silent right channel when plugin receives mono input
- **Cause:** Not copying channel 0 to channel 1 for mono inputs
- **Fix:** Check `buffer.getNumChannels()` and copy if mono: `buffer.copyFrom(1, 0, buffer, 0, 0, numSamples)`
- **Prevention:** Always handle mono input in processBlock. Test with mono tracks.

### FM-09: SmoothedValue not advanced per sample
- **Symptom:** Parameters don't smooth (zipper noise on automation)
- **Cause:** Calling `getCurrentValue()` instead of `getNextValue()` per sample, or not calling it every sample
- **Fix:** Call `getNextValue()` exactly once per sample in the processing loop
- **Prevention:** Code review. Never use `getCurrentValue()` in processBlock.

### FM-10: Coefficients not recalculated on sample rate change
- **Symptom:** Filters sound wrong after sample rate changes, wrong decay times at different rates
- **Cause:** Calculating filter coefficients once and not updating in `prepareToPlay()`
- **Fix:** Recalculate all sample-rate-dependent values in `prepareToPlay()`, which is called on rate changes
- **Prevention:** All DSP module `prepare()` methods should accept sample rate and recalculate

### FM-11: Click/pop on bypass toggle
- **Symptom:** Audible click when toggling bypass or switching processing modes
- **Cause:** Discontinuity in output level at the switch point
- **Fix:** Smooth bypass transitions using `SmoothedValue` over ~42ms (0.042s / ~512 samples at 48kHz). Use ~20ms for continuous parameter smoothing; the longer 42ms is specifically for mode/bypass transitions where the discontinuity is larger.
- **Prevention:** Never hard-switch between dry and wet. Always crossfade.

### FM-12: Wrong block size assumption
- **Symptom:** Buffer overrun or underrun, intermittent crashes
- **Cause:** Assuming processBlock always receives exactly `samplesPerBlock` samples
- **Fix:** Always use `buffer.getNumSamples()` for the actual count, not the prepared block size
- **Prevention:** `samplesPerBlock` in `prepareToPlay()` is a hint, not a guarantee

### FM-13: Aliasing in nonlinear processing
- **Symptom:** Harsh, metallic artifacts from distortion or waveshaping
- **Cause:** Waveshaping without oversampling creates harmonic foldback
- **Fix:** Use `dsp::Oversampling` (2x or 4x) around waveshaping stages, or use ADAA techniques
- **Prevention:** Always oversample when applying nonlinear transfer functions, especially with high-frequency content

### FM-14: Equal-power mix not used
- **Symptom:** Volume dip at 50% wet/dry mix
- **Cause:** Linear crossfade: `dry*(1-mix) + wet*mix` loses 3dB at center
- **Fix:** Equal-power: `dryGain = cos(mix * PI/2); wetGain = sin(mix * PI/2)`
- **Prevention:** Always use equal-power for dry/wet mixing

---

## Moderate (DAW Compatibility, User Experience)

### FM-15: AU plugin not discovered on macOS
- **Symptom:** Plugin doesn't appear in DAW's plugin list (macOS AU format)
- **Cause:** Plugin not in correct location; AU cache not refreshed
- **Fix:** Copy to `~/Library/Audio/Plug-Ins/Components/`. Run `killall -9 AudioComponentRegistrar`.
- **Prevention:** Set `COPY_PLUGIN_AFTER_BUILD TRUE` in CMake. Test in fresh DAW session.

### FM-16: VST3 parameter ID mismatch after refactoring
- **Symptom:** Automation data breaks when loading old sessions after parameter changes
- **Cause:** VST3 uses hashed parameter IDs; changing ID strings breaks backward compatibility
- **Fix:** Never change parameter ID strings after release. Add new parameters, deprecate old ones.
- **Prevention:** Lock parameter IDs in the plugin brief before v1.0 release

### FM-17: LookAndFeel destruction order crash
- **Symptom:** Crash on plugin close, especially on Windows
- **Cause:** LookAndFeel destroyed before components that reference it
- **Fix:** Declare LookAndFeel members BEFORE component members in class (destructors run in reverse order)
- **Prevention:** Member ordering convention: LookAndFeel first, then components, then attachments

### FM-18: Static/singleton state shared across instances
- **Symptom:** Multiple plugin instances interfere with each other
- **Cause:** Using `static` variables or singletons (DAWs may run multiple instances in one process)
- **Fix:** Store all state as instance members, pass through component hierarchy
- **Prevention:** Never use `static` for mutable state. Never use singletons in plugins.

### FM-19: UI jank from excessive repainting
- **Symptom:** Jerky, flickering UI, high CPU from GUI thread
- **Cause:** Calling `repaint()` too frequently or on too-large regions; transparent components triggering parent repaints
- **Fix:** Use `setOpaque(true)` where possible. Use `setBufferedToImage(true)` for expensive static visuals. Repaint only the dirty region.
- **Prevention:** Enable `JUCE_ENABLE_REPAINT_DEBUGGING=1` during development. Use Perfetto for UI profiling.

### FM-20: MSVC runtime linking issues on Windows
- **Symptom:** Plugin fails to load on some Windows systems
- **Cause:** Dynamically linked MSVC runtime not present on target machine
- **Fix:** Statically link runtime: `set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")`
- **Prevention:** Always statically link on Windows. Verify with `dumpbin /dependents`.

---

## Build & CI Specific

### FM-21: Build cache corruption after toolchain update
- **Symptom:** Mysterious build failures, missing headers (e.g., TargetConditionals.h on macOS)
- **Cause:** Stale CMake cache after Xcode/compiler update
- **Fix:** `rm -rf build/` and reconfigure
- **Prevention:** CI should always build from clean state

### FM-22: Plugin reports wrong version in DAW
- **Symptom:** User can't verify they have the latest version
- **Cause:** Version not propagated from CMake to plugin binary
- **Fix:** Use `project(PluginName VERSION x.y.z)` in CMake, reference `PROJECT_VERSION` in code
- **Prevention:** Embed build timestamp/type in UI: `__DATE__`, `__TIME__`, `CMAKE_BUILD_TYPE`

### FM-23: pluginval failures in CI
- **Symptom:** CI pipeline fails on pluginval step
- **Common causes:** Listener leaks, assertion failures, state round-trip failures, excessive CPU in processBlock
- **Fix:** Run pluginval locally with debugger attached. Use random seed from CI output to reproduce.
- **Prevention:** Run pluginval locally before pushing. Include in CI pipeline from day one.

---

## Prevention Checklist (Pre-Release)

Run before every release:

- [ ] pluginval at strictness 5+ passes on all target formats
- [ ] State save/restore round-trips correctly (save, quit, reopen, verify)
- [ ] Tested at 44.1k, 48k, 96k sample rates
- [ ] Tested at buffer sizes: 64, 128, 256, 512, 1024, 2048
- [ ] Process 60 seconds of silence — CPU stays low (denormal check)
- [ ] Process full-scale noise — no NaN/Inf in output
- [ ] Multiple instances simultaneously don't crash or interfere
- [ ] Editor open/close 10 times rapidly without crash
- [ ] Automation record/playback works for all parameters
- [ ] DAW bypass produces clean dry signal
- [ ] No `DBG()` calls in Release build
- [ ] No `static` mutable state
- [ ] All `addListener` calls have matching `removeListener` in destructors
- [ ] Output never exceeds 0dBFS (limiter/tanh present)

---

## Numbering Systems

This file uses **FM-xx** (FM-01 through FM-23) for **generic JUCE failure modes** — common across all plugin projects.

The playbook (juce-vst-dev-playbook-v2.json) uses a separate **EM-xx** namespace for **session-specific bugs** found during the Epiphany Machine v3 build. These are real bugs from a real session, and many map to generic failure modes:

| Session Bug | Description | Maps to Generic |
|---|---|---|
| EM-01 | Cache keyed on names not IDs — still O(N) | FM-01 (audio thread perf) |
| EM-02 | Implementer modified out-of-scope files | (process issue) |
| EM-03 | FHT at output only, not in feedback path | (algorithm correctness) |
| EM-04 | Buffer overflow at >48kHz | FM-02 |
| EM-05 | Reverse playback off-by-one | (algorithm correctness) |
| EM-06 | writeHead unbounded overflow | (algorithm correctness) |
| EM-07 | Drift on reverse grains — out-of-window reads | (algorithm correctness) |
| EM-08 | Q constant wrong — 24.5 gives Q=25 not Q=50 | (formula verification) |
| EM-09 | Wavefolder multiplier 3x instead of 4x | (formula verification) |
| EM-10 | Missing feedback soft-clip | FM-05 |
| EM-11 | getTailLengthSeconds() too short | (DAW compatibility) |
| EM-12 | GUI component overflows parent panel | (GUI bounds) |
| EM-13 | SmoothedValue getNextValue() never called | FM-09 |
| EM-14 | Linear mix blend — only first 30% effective | FM-14 |
| EM-15 | Default decay too long — masks parameter changes | (UX) |
| EM-16 | Git push rejected — remote diverged | (release process) |

**Key insight:** EM-13, EM-14, EM-15 were caught by the **user in DAW testing**, not by automated review. Two-stage code review catches implementation bugs but cannot catch perceptual/UX issues.

---

## Sources
- melatonin.dev/blog/pluginval-is-a-plugin-devs-best-friend/
- melatonin.dev/blog/big-list-of-juce-tips-and-tricks/
- melatonin.dev/blog/how-juce-components-work/
- Epiphany Machine v3 build session (16 bugs found and fixed)
- github.com/Tracktion/pluginval
