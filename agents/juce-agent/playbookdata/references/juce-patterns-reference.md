# JUCE Patterns & Best Practices Reference
## Compiled from: melatonin.dev, JUCE tutorials, awesome-juce, JUCE docs

This reference captures patterns the agent should follow when writing JUCE plugin code. Organized by domain.

---

## 1. Audio Thread Safety (CRITICAL)

These rules are non-negotiable. Violating them causes audio dropouts, crashes, or undefined behavior.

### No heap allocation in processBlock
- No `new`, `delete`, `malloc`, `std::vector::push_back`, `std::string` construction
- No `juce::String` construction or manipulation
- No `DBG()` macro (it allocates)
- **Hidden allocation:** `std::unordered_map::find()` can allocate internally via `std::string` key construction — use `std::vector<std::pair<>>` or flat arrays for audio-thread lookups
- Pre-allocate all buffers in `prepareToPlay()`

### No locks on audio thread
- No `std::mutex`, `juce::CriticalSection`, `juce::SpinLock` in processBlock
- Use `std::atomic` for flag communication between threads
- Use `juce::AbstractFifo` or lock-free queues for data transfer

### No blocking operations
- No file I/O, network calls, or system calls
- No `sleep`, `wait`, or condition variable waits

### Denormal protection
- Always use `juce::ScopedNoDenormals` at the top of processBlock
- Alternative: `juce::dsp::util::snapToZero()` on individual values
- Denormals cause 10-100x CPU spikes on x86 processors

### Parameter reading pattern
```
// In processBlock — read once per block, not per sample
auto decay = apvts.getRawParameterValue("decay")->load();
```

### SmoothedValue pattern
- Use `juce::SmoothedValue` for all automatable parameters
- Reset in `prepareToPlay()`: `smoothed.reset(sampleRate, rampTimeSeconds)`
- Advance per sample: `smoothed.getNextValue()` — MUST call every sample
- Set target from parameter: `smoothed.setTargetValue(newValue)`
- **Ramp times:**
  - ~20ms (0.02s) for continuous parameters (gain, frequency, mix) — prevents zipper noise
  - ~42ms (0.042s / ~512 samples at 48kHz) for bypass/mode transitions — prevents click/pop
  - Snap immediately with `setCurrentAndTargetValue()` only in `prepareToPlay()` or when loading state

---

## 2. Parameter Management (APVTS)

### AudioProcessorValueTreeState setup
- Define all parameters in `createParameterLayout()` static method
- Use helper lambdas for repetitive parameter creation
- Always provide `NormalisableRange` with step size and optional skew
- Skew < 1.0 gives more resolution at low end (good for frequencies)
- Skew > 1.0 gives more resolution at high end (rare)

### Parameter callbacks
- `parameterChanged()` and `parameterValueChanged()` can execute on ANY thread including audio
- NEVER call `repaint()` from parameter callbacks
- NEVER use `AsyncUpdater` (not lock-free)
- Pattern: set an `std::atomic<bool>` flag, check it in a `juce::Timer` callback on the message thread
- JUCE 7.0.6+: use `juce::VBlankAttachment` instead of Timer for frame-synced repaints

### Parameter ranges for common types
| Type | Typical range | Skew | Notes |
|---|---|---|---|
| Frequency (Hz) | 20–20000 | 0.2–0.3 | Log-space perception |
| Gain (dB) | -60 to +24 | 1.0 | Linear in dB is already log-perception |
| Time (seconds) | 0.001–20 | 0.3–0.5 | Log-space for musical feel |
| Resonance (Q) | 0.1–40 | 0.3 | Log-space; >30 self-oscillates |
| Mix (dry/wet) | 0.0–1.0 | 1.0 | Use equal-power cosine |
| Feedback | 0.0–0.95 | 0.5 | NEVER allow 1.0; log-space |
| Semitones | -24 to +24 | 1.0 | Linear |
| LFO rate | 0.01–20 | 0.3 | Log-space |
| Attack/Release | 0.001–2.0 | 0.3 | Log-space; seconds |

---

## 3. State Save/Restore (MANDATORY)

Every plugin MUST implement `getStateInformation` and `setStateInformation`. Empty stubs mean no DAW session persistence.

### Standard APVTS pattern
```cpp
void getStateInformation(juce::MemoryBlock& destData) override
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void setStateInformation(const void* data, int sizeInBytes) override
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
```

### Validation
- Round-trip test: save state, load state, verify all parameters match
- Test with DAW save/reopen cycle
- pluginval tests this automatically

### Preset morphing / interpolation
- **Continuous float params:** Linear interpolation between preset A and B values
- **Frequency params:** Log-space interpolation (`exp(lerp(log(a), log(b), t))`) — perceptually even
- **Mix/dry-wet params:** Cosine (equal-power) interpolation
- **Choice params (AudioParameterChoice):** Snap at 50% crossover point — no intermediate states
- **Bool params:** Not morphed — hold source value

---

## 4. Plugin Lifecycle

### prepareToPlay(sampleRate, samplesPerBlock)
- Allocate all working buffers here
- Initialize SmoothedValues with `reset(sampleRate, rampTime)`
- Recalculate all sample-rate-dependent coefficients
- Call `prepare()` on all DSP modules
- May be called multiple times (sample rate change, buffer size change)

### processBlock(buffer, midiMessages)
- `juce::ScopedNoDenormals` first line
- Handle mono input: copy channel 0 to channel 1
- Read parameters once per block (or use SmoothedValue per sample)
- Process audio
- Never assume buffer size equals samplesPerBlock — it can vary

### releaseResources()
- Called when audio stops
- Free any resources not needed when idle
- Rarely needs implementation beyond clearing buffers

### getTailLengthSeconds()
- Must return the maximum tail length (e.g., max decay parameter value for reverbs)
- DAWs use this to know how long audio persists after input stops
- If too short: reverb tail cut off when track is muted or bounced

### suspendProcessing(bool)
- Call `suspendProcessing(true)` before loading heavy resources (IRs, wavetables, large preset data)
- Call `suspendProcessing(false)` when done
- Prevents audio thread from calling processBlock during the load
- The DAW will output silence during suspension

### Custom DSP module lifecycle pattern
- Define all DSP modules with consistent interface: `prepare(sampleRate, blockSize)`, `reset()`, `process(buffer)`
- `prepare()`: allocate buffers, compute sample-rate-dependent coefficients
- `reset()`: zero all delay/filter state (called on transport reset)
- `process()`: in-place stereo processing, no allocations, no I/O

### Bus layouts
```cpp
bool isBusesLayoutSupported(const BusesLayout& layouts) const override
{
    auto output = layouts.getMainOutputChannelSet();
    if (output != juce::AudioChannelSet::stereo()) return false;
    auto input = layouts.getMainInputChannelSet();
    return input == juce::AudioChannelSet::stereo()
        || input == juce::AudioChannelSet::mono();
}
```

---

## 5. GUI Patterns

### Component lifecycle
- Call `setSize(w, h)` LAST in editor constructor, after all children are added
- Use `addAndMakeVisible()` for all child components
- Override `resized()` to position children (called when size changes)
- Override `paint(Graphics& g)` for custom drawing

### Member ordering matters
- Destructors run in reverse declaration order
- LookAndFeel objects must be declared BEFORE components that use them
- Slider attachments must be declared AFTER the slider

### Performance-conscious painting
- Treat `paint()` like audio thread: minimize allocations
- Pre-allocate `juce::Path`, `juce::Image`, `juce::ColourGradient` as member variables
- Use `setBufferedToImage(true)` for expensive static visuals (leaf components only)
- Use `setOpaque(true)` when component fills its bounds completely (prevents parent repaint)
- Use `JUCE_ENABLE_REPAINT_DEBUGGING=1` to diagnose unnecessary repaints

### Repaint system
- `repaint()` doesn't paint immediately — it flags bounds as dirty
- OS coalesces dirty regions and triggers actual painting later
- Calling `repaint()` in a tight loop doesn't cause multiple paints
- `isShowing()` checks actual visibility; `isVisible()` is just a flag

### Custom LookAndFeel
- Subclass `juce::LookAndFeel_V4`
- Override specific draw methods (drawRotarySlider, drawButtonBackground, etc.)
- For fully custom widgets: paint directly, don't create LookAndFeel classes
- Don't reinvent `TextEditor` — it's complex and handles text layout

### Slider configuration
- `setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag)` for knobs
- `setTextBoxStyle(juce::Slider::NoTextBox, ...)` to hide text box
- Use `juce::SliderParameterAttachment` (JUCE 7+) or `juce::AudioProcessorValueTreeState::SliderAttachment`

### JUCE_LIVE_CONSTANT
- Use `JUCE_LIVE_CONSTANT(value)` for runtime tuning during development
- Provides a slider UI to adjust values without recompiling
- Good for: shadow blur, spacing, opacity, color values

---

## 6. CMake Configuration

### Recommended CMakeLists.txt structure
```cmake
cmake_minimum_required(VERSION 3.22)
project(PluginName VERSION 1.0.0)

# Fetch JUCE
include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12)
FetchContent_MakeAvailable(JUCE)

# Plugin target
juce_add_plugin(PluginName
    COMPANY_NAME "YourCompany"
    PLUGIN_MANUFACTURER_CODE Xxxx
    PLUGIN_CODE Yyyy
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Plugin Name"
    COPY_PLUGIN_AFTER_BUILD TRUE)

# Source files (glob with CONFIGURE_DEPENDS)
file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
target_sources(PluginName PRIVATE ${SourceFiles})

# Link JUCE modules
target_link_libraries(PluginName
    PRIVATE juce::juce_audio_utils juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)

# Compile definitions
target_compile_definitions(PluginName PUBLIC
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_WEB_BROWSER=0)
```

### Key settings
- `COPY_PLUGIN_AFTER_BUILD TRUE` — auto-copies to system plugin directory
- `juce::juce_recommended_lto_flags` — enables link-time optimization in Release
- Use Ninja generator for fastest builds: `cmake -B build -G Ninja`
- Windows: statically link MSVC runtime to avoid DLL dependency issues

### File globbing
- Use `CONFIGURE_DEPENDS` — re-scans when files change
- Globe `*.cpp` and `*.h` together
- Acceptable in modern CMake despite historical advice against it

---

## 7. Cross-Platform Considerations

### macOS
- AU plugins must be in `/Library/Audio/Plug-Ins/Components/` or `~/Library/...`
- Reset AU cache: `killall -9 AudioComponentRegistrar`
- Code signing required for distribution (notarization for macOS 10.15+)
- Apple Silicon: universal binary via `CMAKE_OSX_ARCHITECTURES "arm64;x86_64"`

### Windows
- Statically link MSVC runtime to avoid redistribution issues
- Code signing required for SmartScreen bypass
- VST3 path: `C:\Program Files\Common Files\VST3\`

### Linux
- VST3 path: `~/.vst3/`
- May need to install ALSA, JACK, or X11 dev packages
- Build deps: `sudo apt install libasound2-dev libjack-jackd2-dev libfreetype-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev`

---

## 8. Useful JUCE Utility Classes

| Class | Module | Purpose |
|---|---|---|
| `AudioBuffer<float>` | audio_basics | Main audio buffer type |
| `MidiBuffer` | audio_basics | MIDI event container |
| `SmoothedValue<float>` | audio_basics | Click-free parameter ramping |
| `ADSR` | audio_basics | Attack-Decay-Sustain-Release envelope |
| `AudioProcessorValueTreeState` | audio_processors | Parameter management system |
| `ValueTree` | data_structures | Hierarchical data storage |
| `UndoManager` | data_structures | Undo/redo for ValueTree |
| `File` | core | Cross-platform file access |
| `Timer` | events | Periodic callback on message thread |
| `AsyncUpdater` | events | Trigger message-thread callback (NOT lock-free) |
| `Component` | gui_basics | Base UI element |
| `Slider` | gui_basics | Rotary/linear knob |
| `TextButton` | gui_basics | Clickable button |
| `Label` | gui_basics | Text display |
| `ComboBox` | gui_basics | Dropdown selector |
| `LookAndFeel_V4` | gui_basics | UI theme system |
| `FlexBox` / `Grid` | gui_basics | Responsive layout |
| `Colour` | graphics | Color representation |
| `Path` | graphics | Vector graphics path |
| `Graphics` | graphics | Drawing context |

---

## Sources
- melatonin.dev/blog/big-list-of-juce-tips-and-tricks/
- melatonin.dev/blog/how-juce-components-work/
- melatonin.dev/blog/dealing-with-jank-in-juce/
- melatonin.dev/blog/how-to-use-cmake-with-juce/
- juce.com/learn/tutorials/
- docs.juce.com/master/
