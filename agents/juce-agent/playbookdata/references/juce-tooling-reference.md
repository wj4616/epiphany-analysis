# JUCE Plugin Tooling Reference
## Testing, CI/CD, Profiling, Validation

---

## 1. Testing

### pluginval (ESSENTIAL — non-negotiable pre-release tool)
- **URL:** github.com/Tracktion/pluginval
- **What:** Cross-platform plugin validator. Loads your plugin as a DAW would and runs comprehensive tests.
- **Tests include:** Editor open/close cycles, state save/restore round-trip, parameter randomization, audio processing stress, listener cleanup, CPU measurement
- **Strictness levels:** 1 (basic) to 10 (comprehensive). Level 5+ recommended.
- **macOS:** Also runs Apple's `auval` at strictness 5+
- **Online option:** validatemyplugin.com (no local install needed)
- **CI integration:** Download binary, run against built plugin. Pamplejuce template includes this.
- **Debugging failures:** Run pluginval from IDE with debugger. Note the random seed from CI output to reproduce intermittent issues.
- **Common failures:** Listener leaks, state round-trip, assertion failures, CPU spikes

### Catch2 (recommended C++ testing framework)
- **URL:** github.com/catchorg/Catch2
- **Version:** v3.x recommended
- **Setup:** Pamplejuce template includes Catch2 v3.7.1 with dedicated test target
- **IDE support:** CLion has built-in Catch2 test runner UI
- **What to test:**
  - Parameter conversion functions (dB to gain, frequency to coefficient)
  - DSP modules in isolation (feed known input, verify output)
  - State serialization round-trip
  - Buffer boundary conditions
- **What NOT to test:** Exact DSP output values for ear-tuned algorithms. Instead test bounds (no NaN/Inf, output within range).
- **Pattern: Null testing** — sum output with inverted expected result, verify near zero

### melatonin_test_helpers
- **URL:** github.com/sudara/melatonin_test_helpers
- **What:** Catch2 matchers for `juce::AudioBlock` (assert buffer contents in tests)

### melatonin_audio_sparklines
- **URL:** github.com/sudara/melatonin_audio_sparklines
- **What:** ASCII waveform display of audio buffers in test output and console
- **Use:** Visual debugging of DSP test results

### juce-end-to-end
- **URL:** github.com/FocusriteGroup/juce-end-to-end
- **What:** End-to-end functional testing via JavaScript
- **Use:** Automated UI interaction testing

### straw
- **URL:** github.com/kunitoki/straw
- **What:** JUCE automation framework for integration testing

---

## 2. CI/CD

### Pamplejuce (gold standard template)
- **URL:** github.com/sudara/pamplejuce
- **Provides:**
  - GitHub Actions workflow for Linux, macOS, Windows
  - Automatic pluginval validation
  - macOS code signing + notarization
  - Windows Azure Trusted Signing
  - Catch2 test execution
  - Benchmark target
  - CPM dependency management
  - `.clang-format` for code consistency
  - VERSION file propagation
- **How to use:** GitHub "Use this template" button, not fork

### plugin-ci-example
- **URL:** github.com/jatinchowdhury18/plugin-ci-example
- **What:** CI examples for GitLab, GitHub Actions, and Azure Pipelines
- **Reference:** Good for understanding CI patterns across platforms

### Key CI Steps (from Pamplejuce workflow)
1. **Configure:** `cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER_LAUNCHER=sccache -DCMAKE_CXX_COMPILER_LAUNCHER=sccache`
2. **Build:** `cmake --build build --config Release`
3. **Test:** `ctest --verbose --output-on-failure`
4. **Validate:** `pluginval --strictness-level 10 --verbose --validate [VST3_PATH]`
5. **Sign:** macOS: codesign + notarytool; Windows: Azure Trusted Signing; Linux: none needed
6. **Package:** macOS: pkgbuild + productbuild → .pkg; Windows: Inno Setup → .exe; Linux: .zip
7. **Release:** On version tags, publish all artifacts to GitHub Releases

### Pamplejuce matrix strategy
- **Linux:** ubuntu-22.04, Clang, Ninja, Xvfb display
- **macOS:** macos-14, Ninja, universal binary (`arm64;x86_64`)
- **Windows:** windows-latest, MSVC, Ninja
- `fail-fast: false` to see all platform errors
- sccache for build caching across runs
- Optional Intel IPP on Linux (OneAPI) and Windows (NuGet)

### Code Signing
- **macOS:** Notarization required for macOS 10.15+. Blog: melatonin.dev/blog/how-to-code-sign-and-notarize-macos-audio-plugins-in-ci/
- **Windows:** Azure Trusted Signing (replaces expensive EV certs). Blog: melatonin.dev/blog/code-signing-on-windows-with-azure-trusted-signing/
- **Linux:** No code signing required

---

## 3. Profiling & Performance

### Perfetto (recommended for JUCE)
- **URL:** github.com/sudara/melatonin_perfetto
- **What:** Cross-platform performance tracer with timeline visualization
- **Advantage over traditional profilers:** Shows every callback in sequence over time (not aggregated). Critical for audio thread where worst-case matters.
- **Setup:** Add melatonin_perfetto module. Annotate with `TRACE_DSP`, `TRACE_COMPONENT` macros.
- **Output:** `.pftrace` file → upload to ui.perfetto.dev
- **What to measure:**
  - Audio thread: processBlock execution time, per-callback timing variation, worst-case latency
  - UI thread: paint call frequency, per-component paint duration, unexpected repaints
- **Key insight:** Audio work must complete within `bufferSize / sampleRate` seconds. At 32 samples/44.1kHz = 0.725ms budget.
- **Always profile Release builds** for accurate results

### JUCE_ENABLE_REPAINT_DEBUGGING
- **What:** Flashes repainted areas with random colors
- **Setup:** `target_compile_definitions(Plugin PUBLIC JUCE_ENABLE_REPAINT_DEBUGGING=1)`
- **Use:** First diagnostic for UI jank. Shows which components repaint and how often.
- **macOS alternative:** Quartz Debug tool (Xcode Additional Tools)

### Intel IPP (Performance Primitives)
- **URL:** melatonin.dev/blog/using-intel-performance-primitives-ipp-with-juce-and-cmake/
- **What:** Intel's optimized signal processing functions (FFT, convolution, etc.)
- **When:** For CPU-critical DSP when JUCE built-ins aren't fast enough
- **Integration:** Optional in Pamplejuce template

### DSP-Testbench
- **URL:** github.com/AndrewJJ/DSP-Testbench
- **What:** Plugin for testing DSP with signal sources, routing, analysis, monitoring
- **Use:** Feed known test signals through your plugin and analyze output

---

## 4. Debugging

### melatonin_inspector
- **URL:** github.com/sudara/melatonin_inspector
- **What:** Web-inspector-style tool for JUCE component debugging
- **Features:** Inspect component hierarchy, bounds, visibility, repaint regions
- **Included in:** Pamplejuce template

### juce-toys (LLDB helpers)
- **URL:** github.com/jcredland/juce-toys
- **What:** LLDB type summaries for JUCE types, component debugger, buffer debugger, ValueTree debugger
- **Use:** Better debugging experience in Xcode/CLion

### Console debugging
- **`DBG()` macro:** Prints to console. WARNING: Allocates memory — never use in processBlock.
- **Custom debug macro pattern:** Variadic helper that prints variable names + values. No-op in Release.
- **melatonin_audio_sparklines:** Visualize AudioBlock content in LLDB

### Leak Detector
- JUCE's built-in `JUCE_LEAK_DETECTOR` fires on leaked objects
- When hit: press "Continue" in debugger — multiple types may leak, first isn't always the root
- First reported objects are often members of the actual leaked parent

---

## 5. Build Tools

### CMake (recommended)
- Modern CMake 3.22+ with JUCE's `juce_add_plugin()` helper
- Replaces Projucer for all new projects
- `CONFIGURE_DEPENDS` glob for automatic source file detection
- Ninja generator for fastest builds
- See juce-patterns-reference.md Section 6 for full CMake guide

### clang-format
- Automatic code formatting
- `.clang-format` file in project root
- CLion has built-in support
- Pamplejuce includes a configuration file

### Plugalyzer
- **URL:** github.com/CrushedPixel/Plugalyzer
- **What:** Command-line plugin host for debugging
- **Use:** Load and test plugin from terminal without opening a DAW

### formula
- **URL:** github.com/soundspear/formula
- **What:** VST/AU plugin for live-coding DSP in C
- **Use:** Rapid DSP prototyping inside your DAW

---

## 6. AI-Assisted Development Methodology

### Two-stage code review (from Epiphany Machine v3 session)
- **Stage 1 — Spec compliance review:** Does the implementation match the spec? Correct parameter ranges, signal flow, algorithm choice.
- **Stage 2 — Code quality review:** Thread safety, buffer overflow, off-by-one, formula correctness at boundary values, resource cleanup.
- **Key finding:** Stage 1 and Stage 2 catch different bug classes. Skipping either leaves gaps. In the EM v3 session, the code quality reviewer caught 6 bugs that the spec reviewer missed (EM-03 through EM-08).
- **Limitation:** Neither stage catches perceptual/UX issues (EM-13, EM-14, EM-15). Human DAW testing is irreplaceable.

### Boundary condition verification (DSP formulas)
- Before committing any DSP formula, verify output at boundary parameter values:
  - `param = 0` (minimum)
  - `param = 1` or `param = max` (maximum)
  - `param = default`
- Common catches: wavefolder that amplifies at drive=0 (should be identity), Q mapping that gives wrong range, gain formula that's non-unity at mix=0
- **From EM session:** EM-08 (Q constant wrong), EM-09 (wavefolder multiplier wrong) — both caught by boundary verification

### Pre-flight checks per phase
- Before starting each development phase, verify a checklist of known failure modes relevant to that phase
- Example for DSP implementation: verify mixing matrix is in feedback path (not just output), compute max buffer size at 192kHz, verify all circular buffer indices use `& mask` wrapping
- Pre-flight checks are prevention — cheaper than debugging after the fact

---

## 7. Community Resources Index

### Most important for agent knowledge
| Resource | URL | Why |
|---|---|---|
| Pamplejuce template | github.com/sudara/pamplejuce | Gold standard project template with CI |
| chowdsp_utils | github.com/Chowdhury-DSP/chowdsp_utils | 30+ modules of production-quality DSP and plugin utilities |
| pluginval | github.com/Tracktion/pluginval | Non-negotiable validation tool |
| melatonin_inspector | github.com/sudara/melatonin_inspector | UI debugging |
| melatonin_perfetto | github.com/sudara/melatonin_perfetto | Performance profiling |
| JUCE tutorials | juce.com/learn/tutorials/ | 62 official tutorials covering all domains |
| melatonin blog | melatonin.dev/blog/ | 20 expert posts on JUCE patterns and pitfalls |
| awesome-juce | github.com/sudara/awesome-juce | 293 curated JUCE resources |
| Audio-Effects examples | github.com/juandagilc/Audio-Effects | Reference implementations of common effects |
| SimpleCompressor | github.com/DanielRudrich/SimpleCompressor | Look-ahead compressor with theory |

### Open-source plugins to study
| Plugin | Type | What to learn from it |
|---|---|---|
| Surge | Synth | Large-scale synth architecture, modulation routing, filter design |
| Vital | Wavetable synth | Wavetable synthesis, spectral warping, modern GUI |
| Dexed | FM synth | FM synthesis, DX7 emulation |
| AnalogTapeModel | Effect | Physical modeling, neural network DSP |
| BYOD | Distortion | Modular effect chain, circuit modeling |
| ZLEqualizer | EQ | Dynamic EQ, multi-band processing |
| RoomReverb | Reverb | Algorithmic reverb implementation |
| valentine | Compressor | Musical compression, gain reduction display |

---

## Sources
- melatonin.dev/blog/ (multiple posts)
- github.com/sudara/awesome-juce
- github.com/sudara/pamplejuce
- github.com/Tracktion/pluginval
- juce.com/learn/tutorials/
