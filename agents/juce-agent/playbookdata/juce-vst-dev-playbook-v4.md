# JUCE VST Plugin Development Playbook v4

**Version:** 4.0
**Date:** 2026-03-28
**Status:** Battle-tested (Epiphany Machine v3 complete)
**Scope:** Full plugin lifecycle — concept through release and retrospective

---

## What This Playbook Is

An end-to-end development playbook for building creative VST audio plugins with JUCE and AI assistance. Designed for a single developer retaining full creative control, with AI handling code generation, review, and validation. Each phase is gated — nothing advances until acceptance criteria are met.

**Extracted from:** 24,292-line development session building Epiphany Machine VST v3 (2026-03-27 to 2026-03-28), plus deep crawl of JUCE docs, awesome-juce, melatonin.dev, Pamplejuce, and community resources.

**Changelog from v3:**
- Converted from JSON to readable markdown for analysis
- Integrated gap analysis findings as concrete lessons learned with resolution status
- Updated project status: v3 is COMPLETE, tagged, released
- Added v3 retrospective data (what worked, what didn't)
- Consolidated reference material from FINAL_PLAYBOOK.md web crawl
- Added concrete timing data from actual session

---

## Roles

| Role | Responsibility |
|------|---------------|
| **You** (human) | Creative director, sound designer, final decision-maker on ALL audio/UX choices. You define WHAT the plugin does and HOW it should sound. Every creative decision is `human_only` — AI asks, never assumes. |
| **AI** | Code architect, implementation engine, reviewer, test generator. AI handles HOW to build what you've defined. Never makes creative decisions autonomously. Must ask for creative direction, never propose defaults for sound/naming/aesthetics. |

---

## References

### Project
| Resource | Location |
|----------|----------|
| GitHub repo | https://github.com/wj4616/EpiphanyMachineVST |
| Website | https://epiphanymachineart.com |
| v2 spec template | `playbookdata/data-2/claude-code-prompt-epiphany-machine-v2-FINAL.md` |
| v3 design spec | `docs/superpowers/specs/2026-03-27-epiphany-machine-v3-design.md` |
| v3 implementation plan | `docs/superpowers/plans/2026-03-27-epiphany-machine-v3.md` |
| v3 user manual | `epiphany-machine-VST-v3-manual.md` |

### JUCE Official
| Resource | URL | When to Use |
|----------|-----|-------------|
| API modules | https://docs.juce.com/master/index.html | Phase 2 (architecture), Phase 4 (DSP), Phase 7 (GUI) |
| API classes | https://docs.juce.com/master/classes.html | Any implementation phase |
| Tutorials | https://juce.com/learn/tutorials/ | Phase 0 (learning), Phase 2 (patterns) |

### Community
| Resource | URL | When to Use |
|----------|-----|-------------|
| awesome-juce | https://github.com/sudara/awesome-juce | Phase 2 (reference implementations), Phase 3 (CI templates), Phase 11 (optimization) |
| Melatonin blog | https://melatonin.dev/blog/ | Phase 3 (test framework), Phase 8 (CI), Phase 11 (optimization) |

### DSP Education
| Resource | Description |
|----------|-------------|
| Julius O. Smith III | Physical Audio Signal Processing (FDN reverbs, delay networks) — https://ccrma.stanford.edu/~jos/ |
| Vadim Zavalishin | The Art of VA Filter Design (Cytomic TPT SVF, all filter topologies) — free PDF |
| Andy Simper (Cytomic) | Original TPT SVF papers — https://cytomic.com/files/dsp/ |
| Will Pirkle | Designing Audio Effect Plugins in C++ (practical JUCE-adjacent DSP) |
| Stanford 320c | https://ccrma.stanford.edu/courses/320c/ — FDN reverbs, filters, granular synthesis |

### Community Libraries
| Library | Description | URL |
|---------|-------------|-----|
| chowdsp_utils | 30+ modules: filters (Butterworth, Chebyshev, SVF), waveshapers (ADAA), reverb (FDN templates), oscillators, plugin state, presets | https://github.com/Chowdhury-DSP/chowdsp_utils |
| Gin | Large utility collection, StackBlur, websockets | https://github.com/FigBug/Gin |
| sst-filters | Surge synthesizer filters as standalone library | https://github.com/surge-synthesizer/sst-filters |
| sst-waveshapers | Surge waveshapers as standalone | https://github.com/surge-synthesizer/sst-waveshapers |
| signalsmith-stretch | Polyphonic pitch/time stretching | https://github.com/Signalsmith-Audio/signalsmith-stretch |
| ff_meters | LED-style level meters | https://github.com/ffAudio/ff_meters |

### Tooling
| Tool | Purpose | Details |
|------|---------|---------|
| pluginval | Cross-platform plugin validator | Strictness 1-10. Level 5+ for release. Tests: editor open/close, state round-trip, param randomization, audio stress, CPU measurement. Debug: run from IDE, note random seed. https://github.com/Tracktion/pluginval |
| melatonin_perfetto | Performance tracer | Shows every callback in sequence (not aggregated). Critical for audio thread worst-case. Annotate with TRACE_DSP/TRACE_COMPONENT macros. Output .pftrace to ui.perfetto.dev. Always profile Release builds. https://github.com/sudara/melatonin_perfetto |
| melatonin_inspector | Component debugger | Web-inspector-style tool for JUCE hierarchy, bounds, visibility, repaint regions. https://github.com/sudara/melatonin_inspector |

---

## Superpowers Integration

| Superpowers Skill | Phase | How to Use |
|-------------------|-------|------------|
| `brainstorming` | Phase 0 | Develop plugin concept and spec. AI asks multi-choice questions, user picks. |
| `writing-plans` | Phase 2 | Produce architecture doc and task breakdown from spec. |
| `subagent-driven-development` | Phases 4-7 | Fresh subagent per task with two-stage review. Caught critical bugs implementers missed. |
| `security-review` | Any | Review new skills or external code before integrating. |

**Spec template:** Use the v2 spec (`claude-code-prompt-epiphany-machine-v2-FINAL.md`) as structural template. Covers: architecture overview with signal flow diagram, DSP implementation details, GUI design spec, parameter list, preset system, build instructions, test checklist.

**Session management:** After each completed phase, extract the working conversation into a phase summary. This becomes the context seed for the next phase — prevents token waste from carrying full history.

---

## Phase 0: Plugin Concept & Spec

**Purpose:** Define exactly what you're building before writing any code. AI should be ASKING YOU questions, not proposing solutions.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 0.1 | Define plugin type and signal flow | `human_only` | Interviewer — ask multi-choice questions | Plugin type (synth/effect/analyzer/hybrid), signal flow, DSP chain order, parameter count. AI NEVER proposes a signal flow unprompted. |
| 0.2 | Define the sound identity | `human_only` | None — purely creative | Reference plugins/hardware, genre context, character words, uniqueness. AI records exact words as creative north star. Never paraphrases. |
| 0.3 | Define technical constraints | human | Advisor — flag JUCE issues | Lock: JUCE version, formats (VST3/AU/AAX/Standalone), target OS (ALL), target DAW (Reaper), sample rates (44.1k-192kHz), buffer size range, CPU budget. |
| 0.4 | Gain staging design | human | Advisor — audit existing issues | Define when hot signals are OK vs not. AI audits existing gain staging before proposing changes. |
| 0.5 | Preset theme and naming | `human_only` | Drafter — user has FINAL say | AI will suggest generic names. REJECT THEM. User defines creative direction and approves every name individually. |
| 0.6 | Write the one-page spec | ai | Writer — user approves | Structured doc: architecture, DSP with code, GUI with dimensions/colors, parameters, presets, build instructions, "what not to do" section. Output: `docs/superpowers/specs/{date}-{plugin-name}-design.md` |
| 0.7 | Spec self-review | ai | Reviewer | Check: placeholder values, contradictions, ambiguity, scope gaps, formula correctness. Verify formulas at boundary conditions (param=0, param=1, param=max). |

### Prevention Rules
- **EM-session1:** AI proposed generic preset names — user rejected. ALWAYS use user's exact creative language.
- **EM-session1:** 6 layers of conservative gain staging identified as root cause of "effect not noticeable." Always audit existing gain staging early.
- **EM-session2:** Wavefolder formula `sin(x*PI)/1.0` amplifies by PI at drive=0. Always verify formulas at boundary conditions.

### Phase Gate
- [ ] Plugin type and signal flow defined
- [ ] Sound identity documented in YOUR words (not paraphrased by AI)
- [ ] Gain staging philosophy explicit
- [ ] Technical constraints locked (JUCE version, formats, ALL target OS, Reaper DAW)
- [ ] One-page spec written and self-reviewed
- [ ] Spec approved by you
- [ ] Out-of-scope list exists
- [ ] Preset theme chosen with creative names you approve

---

## Phase 1: Performance Audit (if upgrading existing code)

**Purpose:** Before ANY new features, audit the existing codebase for memory/CPU/thread-safety issues. Skip for brand-new plugins.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 1.1 | Thread safety audit | ai | Auditor | Scan for non-atomic shared state between audio/GUI threads. Use `std::atomic<bool>` for all cross-thread flags. |
| 1.2 | Instance isolation audit | ai | Auditor | Check for static variables shared across instances. All state must be per-instance members. |
| 1.3 | Audio thread allocation audit | ai | Auditor | Search for heap allocations, large memsets, string construction on audio thread. |
| 1.4 | Parameter lookup optimization | ai | Auditor | Check for O(N) parameter lookups on audio thread. Cache parameter indices at init time. |

### Prevention Rules
- **EM-01:** First cache attempt keyed on names (still O(N) to compute key). Verify ENTIRE call chain is O(1).
- **EM-session3:** `static int lastPreset` shared across all instances — data race.
- **EM-session3:** GlitchEngine cleared 1MB buffer on audio thread.
- **EM-session3:** `std::unordered_map::find()` can allocate via `std::string` construction — use `std::vector<std::pair<>>` for audio-thread lookups.

### Phase Gate
- [ ] No non-atomic shared state between audio/GUI threads
- [ ] No static variables shared across instances
- [ ] No heap allocations in processBlock path
- [ ] Parameter lookups are O(1) on audio thread
- [ ] Build succeeds with all fixes
- [ ] Sound unchanged

---

## Phase 2: Architecture & Task Breakdown

**Purpose:** Turn the spec into a buildable plan with complete code.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 2.1 | Read all existing source files | ai | Architect | AI must read EVERY source file before planning. No planning from memory. |
| 2.2 | Design class architecture | ai | Architect | Class diagram: processor, editor, DSP modules, parameter layout, state management. APVTS for all parameters, processBlock thread-safe, Editor talks to Processor ONLY through APVTS, DSP modules self-contained with prepare()/reset()/process(). |
| 2.3 | Specify DSP algorithms with math | ai | Engineer | For each block: algorithm choice, math, JUCE implementation path, CPU cost, gotchas. VERIFY all formulas at boundary conditions. |
| 2.4 | Break into ordered task list with code | ai | Planner | Each task: ONE thing, files to modify, JUCE classes used, ACTUAL CODE, build command, commit message. Ordered by dependency. |
| 2.5 | Plan self-review | ai | Reviewer | Check: scope completeness, dependency order, type consistency, formula correctness at boundaries, buffer sizes at max sample rate (192kHz). |

### Prevention Rules
- **EM-08:** Plan specified Q constant 24.5f when correct was 49.5f. Always compute formula output at param=0 and param=1.
- **EM-session2:** Plan was 1577 lines — too large for single response. Write in chunks using Write + Edit. Use unique anchor strings to avoid multi-match errors.
- **EM-03/04/08:** Plan self-review MISSED FHT placement, buffer overflow, and Q constant errors. Self-review is necessary but NOT sufficient — two-stage review during implementation catches what self-review misses.

### Phase Gate
- [ ] Every spec requirement has a corresponding task
- [ ] Tasks ordered by dependency
- [ ] Each task has complete code and build step
- [ ] Plan self-review completed
- [ ] All DSP formulas verified at boundary conditions
- [ ] Buffer sizes computed for max supported sample rate (192kHz)

---

## Phase 3: Project Setup & Test Framework

**Purpose:** Set up build system, test framework, and CI before writing DSP code.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 3.1 | Update CMakeLists.txt | ai | Implementer | Version, source files, JUCE modules, target formats (VST3 + Standalone min), C++17, COPY_PLUGIN_AFTER_BUILD. |
| 3.2 | Set up automated test framework | ai | Implementer | Catch2 or GoogleTest. Minimum tests: plugin instantiation, parameter range validation, processBlock smoke (silence + noise), extreme params, state round-trip, multi-instance. |
| 3.3 | Set up CI (recommended) | ai | Implementer | GitHub Actions: Linux (required), macOS (recommended), Windows (recommended). Run tests after build. |

### CI Matrix (from Pamplejuce)
| Platform | Config |
|----------|--------|
| Linux | ubuntu-22.04, Clang, Ninja, Xvfb |
| macOS | macos-14, Ninja, universal binary (arm64;x86_64) |
| Windows | windows-latest, MSVC, Ninja |

### CI Packaging
| Platform | Method |
|----------|--------|
| macOS | pkgbuild + productbuild -> .pkg, codesign + notarytool |
| Windows | Inno Setup -> .exe, Azure Trusted Signing |
| Linux | .zip archive |

### Build Commands
```bash
# Local
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -- -j$(nproc)

# CI (with sccache)
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER_LAUNCHER=sccache -DCMAKE_CXX_COMPILER_LAUNCHER=sccache
cmake --build build --config Release
ctest --verbose --output-on-failure
pluginval --strictness-level 10 --verbose --validate [VST3_PATH]
```

### VST3 Install Paths
| OS | Path |
|----|------|
| Linux | `~/.vst3/` |
| macOS | `/Library/Audio/Plug-Ins/VST3/` |
| Windows | `C:\Program Files\Common Files\VST3\` |

### Phase Gate
- [ ] CMakeLists.txt configured for all target formats
- [ ] Build succeeds on primary platform (Linux)
- [ ] Test framework integrated and basic tests pass
- [ ] CI pipeline runs (if set up)

---

## Phase 4: Core DSP Implementation

**Purpose:** Build the audio engine module by module with two-stage review after each.

### Pre-Flight Checks (run before EVERY DSP module)
- [ ] **EM-03:** In any FDN implementation, verify mixing matrix is in FEEDBACK path, not just output
- [ ] **EM-04:** Compute max delay length at 192kHz: `max_base * (192000/48000)`. Buffer must be >= this.
- [ ] **EM-05:** Reverse playback uses `(start+length-1)`, not `(start+length)`
- [ ] **EM-06:** All circular buffer indices use `& mask` wrapping
- [ ] **EM-08:** Verify filter Q mapping at reso=0 and reso=1

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 4.1 | Implement DSP modules with two-stage review | ai | Implementer + Reviewer | For EACH module: (1) implement, (2) spec compliance review, (3) code quality review, (4) if issues: fix + re-review BOTH stages. |
| 4.2 | Run automated tests after each module | ai | Tester | Compilation succeeds, processBlock smoke test passes. |

### Two-Stage Review Process (Critical)
This is the single most important quality gate in the playbook. From the v3 session:

**Stage 1 — Spec Compliance Review:** Does the code match the spec? Correct algorithm, correct formula constants, correct parameter ranges, correct signal flow.

**Stage 2 — Code Quality Review:** Independent of spec. Buffer overflows, off-by-one errors, thread safety, edge cases, performance. This catches what spec review misses.

**Evidence:** Code quality reviewer caught EM-03 (FHT not in feedback — critical), EM-04 (buffer overflow), EM-05/06/07 (3 GlitchEngine bugs), EM-08 (Q constant). Spec reviewer alone would have missed ALL of these.

### Phase Gate
- [ ] Each module passed two-stage review (spec + code quality)
- [ ] All critical and important issues resolved
- [ ] Fix subagents re-reviewed
- [ ] Automated tests pass
- [ ] Build succeeds

---

## Phase 5: Preset Data & State Management

**Purpose:** Implement presets AND state save/restore. State persistence is NOT optional.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 5.1 | Implement state save/restore | ai | Implementer | **CRITICAL:** `getStateInformation` and `setStateInformation` must be implemented, not empty stubs. Test: save project in Reaper, close, reopen — all parameters restored. |
| 5.2 | Update PresetData with preset values | ai | Implementer | Add new fields to struct, populate from spec. Values are AI-generated but must be auditioned in Phase 9. |
| 5.3 | Update PresetManager | ai | Implementer | Add new params to capture/apply. Log-space interpolation for frequency params, cosine for mix. Choice params snap at 50%. Bools not morphed. |

### State Save/Restore Pattern
```cpp
void getStateInformation(juce::MemoryBlock& dest) override {
    auto state = apvts.copyState();
    auto xml = state.createXml();
    copyXmlToBinary(*xml, dest);
}
void setStateInformation(const void* data, int size) override {
    auto xml = getXmlFromBinary(data, size);
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
```

### Prevention Rules
- **Gap analysis:** Epiphany Machine v3 shipped with empty stubs. Broken state save = lost presets = angry users.
- **EM-14:** Use equal-power (cosine) mixing for dry/wet, not linear. Linear concentrates change in low end.

### Phase Gate
- [ ] State save/restore round-trip test passes
- [ ] Preset count consistent across PresetData, APVTS, PresetManager
- [ ] Morph interpolation uses log-space for frequencies, cosine for mix
- [ ] Automated state save/restore test passes

---

## Phase 6: Processor Integration

**Purpose:** Wire DSP modules together in processBlock.

### Pre-Flight Checks
- [ ] **EM-13:** EVERY SmoothedValue must have `getNextValue()` called per-sample. Audit ALL declarations.
- [ ] **EM-09:** Copy formula constants from spec exactly — no rounding
- [ ] **EM-10:** Every feedback loop must have a safety limiter (soft-clip or hard-clip)
- [ ] **EM-11:** `getTailLengthSeconds()` must return max decay parameter value
- [ ] **EM-14:** Use equal-power (cosine) dry/wet mixing
- [ ] **EM-15:** Default decay should be <=2.0s so parameter changes are audible quickly

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 6.1 | Update parameter layout | ai | Implementer | All parameters with correct ranges, defaults, skew values. Frequency: skew 0.3. Decay/feedback: skew 0.5. |
| 6.2 | Implement processBlock | ai | Implementer | Full signal flow per spec. After writing, grep for `getNextValue()` in per-sample loops. Any missing = dead parameter. |
| 6.3 | SmoothedValue audit | ai | Reviewer | List every SmoothedValue in header. For each: declared? initialized in prepareToPlay? setTargetValue per block? getNextValue per sample? Value used downstream? |

### SmoothedValue Audit Checklist (for each SmoothedValue)
- [ ] Declared in header
- [ ] Initialized in `prepareToPlay` with `setCurrentAndTargetValue()`
- [ ] `setTargetValue()` called once per block
- [ ] `getNextValue()` called per-sample in a loop
- [ ] Value used downstream (not just `.getCurrentValue()`)
- [ ] Ramp time: 20ms for continuous params, 42ms for bypass/mode transitions

### Phase Gate
- [ ] All parameters match spec ranges
- [ ] SmoothedValue audit: every SmoothedValue has getNextValue() per-sample
- [ ] Feedback loop has safety limiter
- [ ] getTailLengthSeconds() matches max decay
- [ ] Equal-power dry/wet mixing
- [ ] Default decay <= 2.0s
- [ ] Build succeeds with zero errors
- [ ] Automated tests pass

---

## Phase 7: GUI / Editor Implementation

**Purpose:** Build the visual interface. You direct look and feel, AI implements.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 7.1 | Define GUI layout and visual identity | `human_only` | Interviewer | YOU define: dimensions, layout, colors, typography, metering. AI implements. |
| 7.2 | Implement custom LookAndFeel | ai | Implementer | Subclass LookAndFeel_V4. Apply to top-level editor. Use `juce::Colour` for theming. |
| 7.3 | Build component hierarchy | ai | Implementer | Every parameter control uses APVTS Attachment. Timer-based updates for value labels. Verified bounds arithmetic. |
| 7.4 | Visual polish iteration | `human_only` | Implementer | Load plugin, look at it, describe changes. Timebox: 2 sessions max. Ship what you have. |

### Prevention Rules
- **EM-12:** Verify ALL component bounds fit within parent panels. Walk the arithmetic: panel starts at panelY, component bottom = componentY + componentHeight, verify componentBottom - panelY <= panelHeight.

### GUI Performance Patterns
- `setOpaque(true)` when component fills its bounds completely — prevents parent repaint cascade
- `setBufferedToImage(true)` for expensive static visuals (leaf components only)
- `JUCE_ENABLE_REPAINT_DEBUGGING=1` during development
- JUCE 7.0.6+: Use `juce::VBlankAttachment` instead of Timer for frame-synced UI updates
- `JUCE_LIVE_CONSTANT(value)` for runtime tuning without recompiling
- Treat `paint()` like audio thread — minimize allocations, pre-allocate Path/Image/ColourGradient as members

### Phase Gate
- [ ] All parameters have GUI controls with APVTS attachments
- [ ] Custom LookAndFeel applied
- [ ] All controls visible and within bounds
- [ ] Version label visible
- [ ] YOU have approved the visual design
- [ ] Build succeeds

---

## Phase 8: Build Verification & Cross-Platform

**Purpose:** Verify the plugin builds and works across all target platforms and configurations.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 8.1 | Linux build and test | ai | Implementer | Clean build, run tests, verify .vst3 binary exists. |
| 8.2 | Cross-platform builds (macOS, Windows) | ai | Implementer | Build via CI or cross-compilation. At minimum verify CMakeLists.txt is correct. |
| 8.3 | Sample rate stress test | ai | Tester | 44.1kHz, 48kHz, 96kHz, 192kHz. No buffer overflows, no NaN, no corruption. |
| 8.4 | Buffer size stress test | ai | Tester | 64, 128, 256, 512, 1024, 2048. No crashes, no glitches. |
| 8.5 | Extreme parameter test | ai | Tester | All params at min, all at max. No crashes, no NaN, no inf. |
| 8.6 | Multi-instance test | ai | Tester | 4+ instances simultaneously. No cross-talk, no shared state corruption. |
| 8.7 | State save/restore in DAW | ai | Tester | Save Reaper project, close, reopen. All parameters match. |
| 8.8 | Git tag release | ai | Implementer | `git tag vX.Y.Z` |

### Prevention Rules
- **EM-04:** DiffusionNetwork kBufSize=4096 overflowed at 96kHz+. Always test at max supported sample rate.
- **EM-session3:** `static int lastPreset` shared across instances.

### Phase Gate
- [ ] Linux build succeeds with zero errors
- [ ] Cross-platform builds succeed (or CMakeLists verified)
- [ ] Sample rate tests pass (44.1k through 192k)
- [ ] Buffer size tests pass (64 through 2048)
- [ ] Extreme parameter test passes
- [ ] Multi-instance test passes
- [ ] State save/restore works in Reaper
- [ ] All automated tests pass
- [ ] Git tagged

---

## Phase 9: DAW Testing & Sound Approval

**Purpose:** Load in Reaper, LISTEN, and fix what you find. This is where real bugs surface and YOU approve the sound.

**Critical insight from v3:** EM-13, EM-14, EM-15 were caught by USER in DAW testing, not by any automated review. Two-stage review catches implementation bugs but CANNOT catch perceptual/UX issues. DAW testing by the human is irreplaceable.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 9.1 | Load in Reaper and play audio | `human_only` | None | Load plugin, play audio, describe what you hear. AI cannot evaluate sound. |
| 9.2 | Control responsiveness check | `human_only` | Debugger if issues | Move EVERY knob while audio plays. Does each produce audible change? Report dead or delayed controls. |
| 9.3 | Mix range check | `human_only` | Debugger if issues | Sweep mix 0->1. Is change evenly distributed or bunched at one end? |
| 9.4 | Preset audition | `human_only` | Implementer for adjustments | Load EVERY preset. Does it sound good? Showcase range? Usable? Mark any needing tuning. |
| 9.5 | Automation test | human | Debugger if issues | Record automation on 2-3 parameters, play back. No clicks, no zippers, smooth transitions. |
| 9.6 | Bypass and freeze test | human | Debugger if issues | Toggle bypass — clean? Toggle freeze — infinite sustain without explosion? Freeze + high drift + high feedback — safety limiter works? |
| 9.7 | Diagnose and fix reported issues | ai | Debugger | Read processBlock, trace signal flow, identify root cause, implement fix, rebuild, user re-tests. |

### Prevention Rules
- **EM-13:** DRIVE knob was dead because SmoothedValue never advanced. Silent bug — no crash, no error.
- **EM-14:** Linear mix blend concentrated change in first 30% of range. Equal-power cosine mixing fixes this.
- **EM-15:** Controls seemed delayed because 4.0s default decay masked changes. Default decay <= 2.0s.

### Phase Gate
- [ ] Plugin loads in Reaper without crashing
- [ ] EVERY control produces audible/visible change when moved
- [ ] Mix knob effective across full range
- [ ] Parameter changes audible within ~2 seconds
- [ ] ALL presets auditioned — usable ones approved, broken ones fixed or removed
- [ ] Automation recording/playback works
- [ ] Bypass and freeze work correctly
- [ ] No audio glitches, clicks, or dropouts
- [ ] YOU have approved the sound

---

## Phase 10: Release & Distribution

**Purpose:** Package, copy binary, push to repo.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 10.1 | Copy binary to project root | ai | Utility | cp compiled .so/.vst3 to project root for easy GitHub download. Commit it. |
| 10.2 | Update documentation | ai | Writer (user edits) | What the plugin does, system requirements, install instructions per OS, parameter reference, quick start. If manual exists, update it. |
| 10.3 | Push to GitHub | human | Helper | Push all commits + tag. Pull first if remote diverged. |

### Prevention Rules
- **EM-16:** Always git pull before pushing a large batch. For binary merge conflicts, use `git checkout --ours` or `--theirs` explicitly.

### Phase Gate
- [ ] Binary at project root (Linux .so at minimum)
- [ ] Documentation updated
- [ ] All commits and tag pushed
- [ ] README reflects current version

---

## Phase 11: Optimization & Hardening

**Purpose:** Make it fast and stable. Profile before optimizing.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 11.1 | CPU profiling | ai | Optimizer | Profile processBlock with perf (Linux), Instruments (macOS), VTune (Windows). Identify top 3 hotspots. Do NOT optimize without profiling first. |
| 11.2 | Memory audit | ai | Reviewer | No heap allocations in processBlock, no memory leaks, all buffers pre-allocated in prepareToPlay. |
| 11.3 | Denormal stress test | ai | Tester | Process 60s of silence. CPU should remain near-zero. Use BOTH ScopedNoDenormals AND 1e-18 offset in feedback paths. |
| 11.4 | Sound verification after optimization | `human_only` | None | LISTEN after every optimization. Confirm sound hasn't changed. Never sacrifice sound quality for CPU. |

### Optimization Targets
- Per-sample transcendental functions (pow, sin, tan, exp) — pre-compute per block where possible
- Per-sample divisions — pre-compute reciprocals
- Cache-unfriendly access patterns in delay line arrays
- Unnecessary computations when parameters haven't changed

### Phase Gate
- [ ] CPU profiled — top hotspots addressed
- [ ] No heap allocations in processBlock
- [ ] Denormal stress test passes (60s silence, CPU stable)
- [ ] Sound verified unchanged after optimization
- [ ] All automated tests still pass

---

## Phase 12: Retrospective & Reusable Components

**Purpose:** What worked, what didn't. Extract templates for next plugin.

### Checklist

| # | Task | Owner | AI Role | Notes |
|---|------|-------|---------|-------|
| 12.1 | Process retrospective | human | Facilitator | Which phases took longest? Where did AI produce best code first try? Where did most iteration burn? What to add to playbook? |
| 12.2 | Extract reusable components | ai | Implementer | Custom DSP blocks, LookAndFeel, APVTS boilerplate, build config, test framework. Save as template or personal JUCE module. |
| 12.3 | Update this playbook | ai | Writer | Add lessons learned, new failure modes, updated prevention rules. Each build should improve the playbook. |
| 12.4 | Define next version scope (optional) | `human_only` | Note-taker | Features cut, user feedback, new ideas, deferred improvements. Becomes Phase 0 input for next run. |

### Phase Gate
- [ ] Process retrospective documented
- [ ] Reusable components extracted
- [ ] Playbook updated with lessons learned
- [ ] Next version scope captured (if applicable)

---

## JUCE Patterns Reference

### Parameter Management — APVTS Setup
- Use `AudioProcessorValueTreeState` with lambda-based parameter creation
- `NormalisableRange` skew: 0.5 for decay/feedback, 0.3 for cutoff frequencies
- `AudioParameterChoice` stores 0-based index integers, not strings
- Parameter ranges must be consistent between APVTS, PresetData, and DSP code

### SmoothedValue (Critical Pattern)
```
prepareToPlay: sm.reset(sr, 0.02); sm.setCurrentAndTargetValue(init);
processBlock:  sm.setTargetValue(val);           // once per block
loop:          const float v = sm.getNextValue(); // per sample
```
- **20ms** for continuous params (gain, freq, mix)
- **42ms** for bypass/mode transitions
- Snap with `setCurrentAndTargetValue()` only in prepareToPlay or state load
- **EM-13:** `getCurrentValue()` alone does NOT advance the smoother

### DSP Module Lifecycle
```cpp
void prepare(double sampleRate, int samplesPerBlock);
void reset();    // zero all state
void process(juce::AudioBuffer<float>& buffer);  // in-place stereo, no allocations
```

### Audio Thread Safety Rules
1. No heap allocations (new, malloc, vector resize, string construction)
2. No locks (mutex, critical section)
3. No I/O, no exceptions
4. `juce::ScopedNoDenormals` at top of processBlock
5. `std::atomic<bool>` for cross-thread flags
6. Pre-allocate all buffers in prepareToPlay
7. SmoothedValue for all parameter reads
8. `std::unordered_map::find()` can allocate via `std::string` key construction — use flat arrays
9. NEVER use `juce::AsyncUpdater` from audio thread — it is NOT lock-free. Use `std::atomic<bool>` + Timer or VBlankAttachment instead

### Equal-Power Mixing
```cpp
float dryGain = std::cos(mix * juce::MathConstants<float>::halfPi);
float wetGain = std::sin(mix * juce::MathConstants<float>::halfPi);
```
**EM-14:** ALWAYS use equal-power, not linear.

### Cross-Platform Notes
| Platform | Key Details |
|----------|-------------|
| macOS | AU cache reset: `killall -9 AudioComponentRegistrar`. Notarization required 10.15+. Universal: `CMAKE_OSX_ARCHITECTURES "arm64;x86_64"` |
| Windows | Static link MSVC runtime: `set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")` |
| Linux | VST3 path: `~/.vst3/`. Deps: libasound2-dev libjack-jackd2-dev libfreetype-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev |

---

## DSP Algorithm Catalog

### DiffusionNetwork (FDN Reverb)
- **Algorithm:** 8-line FDN with Fast Hadamard Transform in feedback path, RT60 decay, per-line stereo panning with LFOs
- **Key Formula:** `RT60 gain = pow(10, -3 * delayLength / (decaySeconds * sampleRate))`
- **Rules:** EM-03 (FHT must be in feedback path), EM-04 (buffer size for 192kHz)
- **Reference:** Julius O. Smith — Physical Audio Signal Processing

### Cytomic TPT State Variable Filter
- **Algorithm:** Trapezoidal SVF (Andy Simper). `g = tan(pi * fc / sr)`, stable at all frequencies, no modulation clicks.
- **Critical:** DO NOT use Chamberlin SVF (`f = 2*sin(...)`). Unstable at high frequencies and clicks during modulation.
- **Reference:** Cytomic papers: https://cytomic.com/files/dsp/

### Granular Engine
- **Algorithm:** Circular-buffer grain playback with adaptive envelopes (Hann >50ms, Tukey 10-50ms, Trapezoid <10ms)
- **Rules:** EM-05 (reverse uses start+length-1), EM-06 (& mask for all indices), EM-07 (no drift on reverse grains)

### Wavefolder
- **Algorithm:** `output = input + drive * (sin(input * foldAmt * PI) / foldAmt - input)`. True bypass at drive=0.
- **Rules:** EM-09 (copy constants exactly), EM-10 (feedback loop needs safety limiter after wavefolder)

---

## Failure Modes Database

16 bugs from Epiphany Machine v3 development. `EM-xx` namespace for session-specific bugs.

| ID | Phase | Bug | Caught By | Key Lesson |
|----|-------|-----|-----------|------------|
| EM-01 | 1 | Cache keyed on names not IDs — still O(N) | Spec reviewer | Verify ENTIRE call chain is O(1) |
| EM-02 | 3 | Implementer modified out-of-scope files | Spec reviewer | One task per subagent, strict scope |
| EM-03 | 4 | FHT at output only, not in feedback — not a real FDN | Code quality reviewer | FHT must be in feedback path |
| EM-04 | 4 | Buffer overflow at >48kHz — kBufSize too small | Code quality reviewer | Compute max delay at 192kHz |
| EM-05 | 4 | Reverse playback off-by-one | Code quality reviewer | `(start+length-1)` not `(start+length)` |
| EM-06 | 4 | writeHead unbounded overflow | Code quality reviewer | All circular indices use `& mask` |
| EM-07 | 4 | Drift on reverse grains — out-of-window reads | Code quality reviewer | No drift on reverse grains |
| EM-08 | 4 | Q constant wrong — 24.5 gives Q=25 not Q=50 | Code quality reviewer | Verify at param=0 and param=1 |
| EM-09 | 6 | Wavefolder multiplier 3x instead of 4x | Code quality reviewer | Copy constants exactly from spec |
| EM-10 | 6 | Missing feedback soft-clip | Code quality reviewer | Every feedback loop needs limiter |
| EM-11 | 6 | getTailLengthSeconds() too short | Code quality reviewer | Return max decay param value |
| EM-12 | 7 | GUI component overflows parent panel | Code quality reviewer | Walk the bounds arithmetic |
| EM-13 | 9 | SmoothedValue getNextValue() never called — dead param | **USER (DAW)** | Audit EVERY SmoothedValue |
| EM-14 | 9 | Linear mix — only first 30% effective | **USER (DAW)** | Equal-power cosine mixing |
| EM-15 | 9 | Default decay too long — masks param changes | **USER (DAW)** | Default decay <= 2.0s |
| EM-16 | 10 | Git push rejected — remote diverged | git | Always pull before push |

**Key insight:** EM-13, EM-14, EM-15 were caught by the human in DAW testing, not by any code review. Two-stage review catches implementation bugs but cannot catch perceptual/UX issues. Human DAW testing is irreplaceable.

---

## Session Management

### When to Start a Fresh Session
- After completing a phase gate
- After 3-4 implementation tasks (subagent transcripts fill context)
- When API errors (529) indicate context too large
- When switching from implementation to DAW testing
- After 2+ hours

### What to Carry Forward
- Spec file path + key decisions
- Plan file path + task progress
- Commit hashes for completed work
- Specific pending issues with symptoms and fixes

### What to Drop
- Full source file contents (AI re-reads them)
- Subagent transcripts (decisions are in commits)
- API error retry conversations
- Rejected design options

---

## Cost Optimization

### Model Selection
| Model | Use For |
|-------|---------|
| **Opus** | Brainstorming (Phase 0), complex architectural decisions, playbook creation |
| **Sonnet** | Plan writing, implementation execution — reliable and sufficient for all implementation |
| **Haiku** | AVOID for scoped implementation — prone to scope creep (modified out-of-scope files in session) |

### Token Saving
- One task per subagent — clean context
- Spec + current task section as context — not the whole plan
- Let AI re-read files instead of pasting
- `/compact` when context fills
- Switch to Sonnet for implementation

---

## How to Run This Playbook

1. **New session (Opus).** Paste Phase 0 + brainstorming skill. AI asks questions, you answer. Output: spec doc.
2. **New session (Sonnet).** Paste spec + Phase 2 + writing-plans skill. Output: task-level plan.
3. **If upgrading:** run Phase 1 (perf audit). If new plugin: skip to Phase 3.
4. **Phase 3:** Set up build, tests, CI.
5. **Phases 4-7:** one session per 3-4 tasks. Use subagent-driven-development.
6. **Phase 8:** Build verification and stress tests.
7. **Phase 9:** Load in Reaper and LISTEN. Fix what you find.
8. **Phase 10:** Release to GitHub.
9. **Phases 11-12:** Optimize, retrospect, extract templates.

---

## Epiphany Machine v3 — Completed Status

**All 13 phases executed. v3.0.0 tagged and released.**

### What Was Built
- 8-line FDN reverb with Hadamard matrix (replaced 4-line)
- Wavefolder in feedback loop (new)
- DRIVE parameter (new)
- Expanded parameter ranges (Q 0.5-50, drift 0-24st, LFO depth +/-3 oct)
- 18 quantum physics presets (replaced 34)
- Per-line FDN panning with LFOs (replaced StereoWidener)
- tanh() output limiter (replaced 4+ internal soft clips)
- Cyberpunk GUI at 950x580

### Known Remaining Gaps (from Gap Analysis)
| Gap | Status | Priority |
|-----|--------|----------|
| State save/restore (empty stubs) | **NOT IMPLEMENTED** | HIGH — broken state = lost presets |
| Automated test framework | Not set up | MEDIUM |
| CI/CD pipeline | Not set up | MEDIUM |
| Multi-format (AU/AAX/Standalone) | Not tested | LOW (Linux-primary) |
| Cross-platform (macOS/Windows) | Not tested | MEDIUM |
| Sample rate stress (96k/192k) | Not explicitly tested | HIGH (EM-04 was critical) |
| Buffer size stress | Not tested | MEDIUM |
| Multiple instance test | Not tested | MEDIUM |
| Automation record/playback | Not tested | LOW |
| pluginval validation | Not run | HIGH |
| Preset listening validation | Not confirmed | MEDIUM |

### Session Timing (actual)
| Session | Duration | Content |
|---------|----------|---------|
| Session 1 | ~35 min | Brainstorming, design decisions, spec |
| Session 2 | ~4 hours | Plan writing (interrupted by API 529) |
| Session 3 | ~10 hours | Plan completion, Task 0 (perf audit) |
| Session 4 | ~8 hours | Tasks 1-9 implementation |
| Session 5 | ~23 min | Task 9 fixes, Task 10 (GUI), Task 11 (build) |
| Session 6 | ~2.5 hours | DAW testing, bug fixes, GitHub push, playbook |

---

## Playbook File Inventory

| File | Purpose | Status |
|------|---------|--------|
| `juce-vst-dev-playbook-v4.md` | **THIS FILE — definitive playbook** | Current |
| `juce-vst-dev-playbook-v3.json` | Structured JSON (v3) | Superseded by v4 |
| `juce-vst-dev-playbook-v2.json` | Structured JSON (v2) | Superseded |
| `juce-vst-dev-playbook.json` | Initial JSON (v1) | Superseded |
| `juce-vst-dev-playbook-gap-analysis.md` | Gap analysis | Integrated into v4 |
| `firecrawl/FINAL_PLAYBOOK.md` | Generic JUCE playbook from web crawl | Reference material, integrated into v4 |
| `conversation_full.md` | 24,292-line session transcript | Source data |
| `data-2/claude-code-prompt-epiphany-machine-v2-FINAL.md` | v2 spec template | Template reference |
