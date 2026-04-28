# Platform/Environment Concerns

## JUCE Framework

### Version Requirements
| Component | Version | Notes |
|-----------|---------|-------|
| JUCE | 8.0.x | Latest stable |
| C++ Standard | C++17 | C++20 optional |
| CMake | 3.22+ | FetchContent native |

### Build Configuration
```cmake
# Standard JUCE CMake setup (FetchContent)
cmake_minimum_required(VERSION 3.22)
project(MyPlugin VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG juce-8.0.x
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(MyPlugin
    PLUGIN_NAME "My Plugin"
    PLUGIN_MANUFACTURER "MyCompany"
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_SYNTH FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_link_libraries(MyPlugin PRIVATE juce::juce_audio_processors)
```

### Plugin Formats
| Format | Platform | Install Path | Notes |
|--------|----------|--------------|-------|
| VST3 | All | `~/.vst3/` (Linux), `/Library/Audio/Plug-Ins/VST3/` (macOS), `C:\Program Files\Common Files\VST3\` (Windows) | Primary format |
| AU | macOS only | `~/Library/Audio/Plug-Ins/Components/` | Code signing required |
| AAX | Pro Tools | `/Library/Application Support/Avid/Audio/Plug-Ins/` | Requires Avid SDK |
| Standalone | All | N/A | For testing |

### Anti-Patterns
| Anti-Pattern | Why Bad | Alternative |
|--------------|---------|-------------|
| Projucer-generated CMake | Less portable, less CI-friendly | Pure CMake with FetchContent |
| Static JUCE modules | Dependency drift | FetchContent for version control |
| Global JUCE install | Version conflicts | Per-project FetchContent |

---

## Linux (Primary Platform)

### Required Dependencies
```bash
sudo apt install libx11-xcb-dev libxcb-util-dev libxcb-cursor-dev \
    libxcb-xkb-dev libxkbcommon-dev libxkbcommon-x11-dev \
    libfontconfig1-dev libcairo2-dev libgtkmm-3.0-dev \
    libsqlite3-dev libxcb-keysyms1-dev
```

### Build Commands
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
# Plugin copied to ~/.vst3/ automatically (COPY_PLUGIN_AFTER_BUILD)
```

### DAW Testing (REAPER)
```bash
# Install REAPER
sudo apt install reaper
# Or download from reaper.fm

# Plugin scan path: ~/.vst3/
# Restart REAPER or rescan to pick up changes
reaper
```

### Linux-Specific Issues
| Issue | Cause | Fix |
|-------|-------|-----|
| X11 display errors | Missing X11 libraries | Install libx11-xcb-dev |
| GTK theme issues | Missing GTK libraries | Install libgtkmm-3.0-dev |
| Audio permission denied | User not in audio group | `sudo usermod -aG audio $USER` |

---

## macOS (Secondary Platform)

### Requirements
| Requirement | Details |
|-------------|---------|
| Xcode | 14.0+ |
| Code signing | Apple Developer account for AU distribution |
| Notarization | Required for distribution outside App Store |

### Build Commands
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Plugin Paths
| Format | Path |
|--------|------|
| VST3 | `/Library/Audio/Plug-Ins/VST3/` |
| AU | `/Library/Audio/Plug-Ins/Components/` |

### macOS-Specific Issues
| Issue | Cause | Fix |
|-------|-------|-----|
| AU not appearing | Cache not refreshed | `killall -9 AudioComponentRegistrar` |
| Code signing error | Missing certificate | Use Xcode signing or manual signing |
| Universal binary | ARM + x86 | Set `CMAKE_OSX_ARCHITECTURES` |

---

## Windows (Secondary Platform)

### Requirements
| Requirement | Details |
|-------------|---------|
| Visual Studio | 2019+ with C++ workload |
| CMake | 3.22+ |
| Ninja | Recommended for speed |

### Build Commands
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### Plugin Paths
| Format | Path |
|--------|------|
| VST3 | `C:\Program Files\Common Files\VST3\` |

### Windows-Specific Issues
| Issue | Cause | Fix |
|-------|-------|-----|
| Missing MSVC runtime | Dynamic linking | Statically link: `set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)` |
| Plugin not loading | Architecture mismatch | Ensure x64 build for 64-bit DAW |

---

## REAPER DAW (Testing Environment)

### Configuration
| Setting | Value | Notes |
|---------|-------|-------|
| Plugin scan path | `~/.vst3/` (Linux) | Automatic with COPY_PLUGIN_AFTER_BUILD |
| Sample rate | 44.1k, 48k, 96k, 192k | Test all |
| Buffer size | 64, 128, 256, 512, 1024, 2048 | Test all |

### Testing Workflow
1. Build plugin
2. Restart REAPER or rescan plugins
3. Insert plugin on track
4. Test parameters, presets, automation
5. Save/close/reopen project (state test)

### REAPER-Specific Features
| Feature | Use |
|---------|-----|
| Plugin browser | Quick plugin loading |
| FX chain | Parameter automation |
| Project save/load | State persistence testing |
| Render | Offline processing test |

---

## CI/CD Pipeline

### GitHub Actions Example
```yaml
name: Build
on: [push, pull_request]

jobs:
  linux:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v4
      - run: sudo apt install libx11-xcb-dev libgtkmm-3.0-dev
      - run: cmake -B build -DCMAKE_BUILD_TYPE=Release
      - run: cmake --build build
      - run: cd build && ctest --output-on-failure
```

### Recommended CI Platforms
| Platform | OS Support | Cost |
|----------|------------|------|
| GitHub Actions | Linux, macOS, Windows | Free (public), 2000 min/month (private) |
| GitLab CI | Linux, macOS, Windows | Free (public) |
| CircleCI | Linux, macOS | Free tier available |