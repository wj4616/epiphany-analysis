# Platform-Specific Concerns

> JUCE/VST3-specific concerns: data storage, safe modification, anti-patterns, platform tools, deployment.

---

## 1. JUCE Framework Concerns

### Data Storage

| Storage Type | Path | Notes |
|--------------|------|-------|
| Plugin binary | `~/.vst3/` (Linux) | No code signing required |
| Plugin binary | `/Library/Audio/Plug-Ins/VST3/` (macOS) | Code signing + notarization required |
| Plugin binary | `C:\Program Files\Common Files\VST3\` (Windows) | Statically link MSVC runtime |
| User presets | `~/Documents/[PluginName]/Presets/` | Application data directory |
| Application data | `juce::File::getSpecialLocation()` | Platform-appropriate directories |
| Plugin state | APVTS state serialization | XML stream |

### Safe Modification Methods

| Method | When to Use | Notes |
|--------|-------------|-------|
| `prepareToPlay()` | Sample rate/buffer change | Recalculate all coefficients |
| `releaseResources()` | Plugin disable | Cleanup (rarely needed) |
| `getStateInformation()` | DAW save | Serialize all state |
| `setStateInformation()` | DAW load | Restore all state |
| `parameterChanged()` | Parameter change | Set atomic flag, don't call repaint |
| `Timer::timerCallback()` | UI update | Check atomic flags, call repaint |

### Anti-Patterns

| Anti-Pattern | Why It's Wrong | Correct Approach |
|--------------|----------------|------------------|
| `new` in processBlock | Real-time violation | Pre-allocate in prepareToPlay |
| `std::vector::push_back` in processBlock | Real-time violation | Pre-size vectors |
| `DBG()` in processBlock | Console I/O blocks | Remove in Release |
| `std::cout` in processBlock | Console I/O blocks | Never use |
| Lock in processBlock | Priority inversion | Use atomic flags |
| `repaint()` from parameter callback | Thread unsafe | Set flag, check in Timer |
| Static mutable state | Instance interference | Instance members only |
| `getCurrentValue()` for SmoothedValue | No smoothing | `getNextValue()` every sample |
| Linear dry/wet mix | Volume dip at center | Equal-power cosine/sine |
| Hard bypass switch | Click at transition | Smooth crossfade |

---

## 2. VST3 SDK Concerns

### Parameter Normalization

| Concern | Implementation |
|---------|----------------|
| All parameters normalized 0.0 to 1.0 | VST3 requirement |
| Display conversion for UI | 0.0-1.0 → displayed value |
| Automation records normalized values | Handled by VST3 SDK |

### Parameter ID Stability

| Concern | Implementation |
|---------|----------------|
| VST3 uses hashed parameter IDs | Never change ID strings |
| Automation breaks if IDs change | Lock IDs before v1.0 release |
| New parameters get new IDs | Add, don't modify |

### State Persistence

| Method | Purpose |
|--------|---------|
| `getStateInformation()` | Serialize for DAW save |
| `setStateInformation()` | Restore from DAW load |
| IBStream format | Binary stream |
| APVTS state | XML or binary |

---

## 3. Platform-Specific Concerns

### Linux

| Concern | Solution |
|---------|----------|
| Plugin path | `~/.vst3/` |
| Code signing | None required |
| Dependencies | X11, XCB, fontconfig, Cairo, GTK |
| Build system | CMake + GCC or Clang |
| Testing DAW | REAPER, Carla, Bitwig |

**Linux Dependencies:**
```
libx11-xcb-dev, libxcb-util-dev, libxcb-cursor-dev,
libxcb-xkb-dev, libxkbcommon-dev, libxkbcommon-x11-dev,
libfontconfig1-dev, libcairo2-dev, libgtkmm-3.0-dev,
libsqlite3-dev, libxcb-keysyms1-dev
```

### macOS

| Concern | Solution |
|---------|----------|
| Plugin path (AU) | `~/Library/Audio/Plug-Ins/Components/` |
| Plugin path (VST3) | `/Library/Audio/Plug-Ins/VST3/` |
| Code signing | `codesign` + `notarytool` |
| Notarization | Required for macOS 10.15+ |
| AU cache reset | `killall -9 AudioComponentRegistrar` |
| Build system | CMake + Xcode toolchain |

**AU Note:** macOS DAWs may prefer AU over VST3. Build both formats.

### Windows

| Concern | Solution |
|---------|----------|
| Plugin path | `C:\Program Files\Common Files\VST3\` |
| Runtime linking | Statically link MSVC runtime |
| Build system | CMake + Visual Studio |
| Dependencies | Windows SDK |

**MSVC Runtime:**
```cmake
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
```

---

## 4. Build System Concerns

### CMake Configuration

| Concern | Implementation |
|---------|----------------|
| JUCE integration | FetchContent for self-contained builds |
| Version management | `project(PluginName VERSION x.y.z)` |
| Build types | Release, Debug, ReleaseWithDebugInfo |
| pluginval integration | Add test target |
| Static runtime | Required for Windows |

**CMake Template:**
```cmake
cmake_minimum_required(VERSION 3.22)
project(MyPlugin VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE
    GIT_TAG juce-8.0.x
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(MyPlugin
    PLUGIN_NAME "My Plugin"
    PLUGIN_CODE "MyPl"
    ...
)
```

### CI/CD Concerns

| Stage | Purpose |
|-------|---------|
| Build | Compile on all platforms |
| Test | Run unit tests |
| pluginval | Validate plugin |
| Package | Create distributable |
| Release | GitHub release or distribution |

---

## 5. Deployment Concerns

### Distribution Platforms

| Platform | Format | Notes |
|----------|--------|-------|
| GitHub Releases | .vst3, .component, .dll | Free distribution |
| App Store | macOS only | Requires developer account |
| Plugin stores | VST3, AU, AAX | Various platforms |
| Direct website | All formats | Need website + payment |

### Versioning Strategy

| Version Type | When to Bump |
|--------------|--------------|
| Major (x.0.0) | Breaking changes |
| Minor (0.x.0) | New features |
| Patch (0.0.x) | Bug fixes |

**Version Embedding:**
```cpp
// In CMakeLists.txt
project(MyPlugin VERSION 1.2.3)

// In code
#define PLUGIN_VERSION "1.2.3"
```

### Code Signing

| Platform | Tool | Notes |
|----------|------|-------|
| macOS | `codesign`, `notarytool` | Required for distribution |
| Windows | Azure Trusted Signing | Replaces expensive EV certs |
| Linux | None | No infrastructure |

---

## 6. DAW Compatibility

### Testing Matrix

| DAW | Platform | Format | Priority |
|-----|----------|--------|----------|
| REAPER | All | VST3, AU | Primary |
| Ableton Live | All | VST3, AU | Secondary |
| Logic Pro | macOS | AU | Secondary |
| Pro Tools | All | AAX | Tertiary |
| Bitwig Studio | All | VST3 | Secondary |

### Common DAW Issues

| Issue | DAW | Solution |
|-------|-----|----------|
| AU not discovered | Logic | Reset AU cache |
| Parameter changes not saved | Various | Implement state serialization |
| Plugin crashes on reopen | Various | Match addListener/removeListener |
| Automation breaks | Various | Never change parameter IDs |

---

## 7. Testing Tools

### pluginval

| Strictness | What It Tests |
|-----------|---------------|
| 1-4 | Basic call coverage |
| 5 | Host compatibility, state round-trip |
| 6-9 | Parameter fuzzing |
| 10 | Real-time safety |

**Usage:**
```bash
pluginval --strictness-level 5 --verbose MyPlugin.vst3
```

### Perfetto

| Purpose | Notes |
|---------|-------|
| CPU profiling | Chrome tracing format |
| Thread analysis | Visualize audio thread |
| Allocation tracking | Find real-time violations |

### Static Analysis

| Tool | Purpose |
|------|---------|
| clang-tidy | Code quality |
| cppcheck | Static analysis |
| ASAN | Memory errors |
| TSAN | Thread errors |

---

## 8. Anti-Pattern Summary

### Never Do

- `new`, `delete`, `malloc`, `free` in processBlock
- `std::vector::push_back` in processBlock
- `std::string` operations in processBlock
- `std::cout`, `DBG()` in processBlock
- Locks, mutexes in processBlock
- File I/O in processBlock
- Network I/O in processBlock
- `repaint()` from audio thread
- Static mutable state
- Singleton pattern in plugins

### Always Do

- Pre-allocate in prepareToPlay
- Use atomic flags for cross-thread
- Call getNextValue() every sample
- Use equal-power for dry/wet
- Clamp feedback < 1.0
- Add limiter at output
- Match addListener/removeListener
- Test at multiple sample rates
- Test at multiple buffer sizes
- Run pluginval before release

---

## Sources

- JUCE Documentation (juce.com/learn/)
- Steinberg VST3 SDK Documentation
- `/home/myuser/agents/juce-agent/playbookdata/references/juce-patterns-reference.md`
- `/home/myuser/agents/juce-agent/playbookdata/references/juce-tooling-reference.md`
- `/home/myuser/agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/`
- melatonin.dev blog