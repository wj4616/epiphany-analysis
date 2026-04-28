# Best Practices & Standards

## Published Standards

### JUCE Framework Standards
| Aspect | Standard | Notes |
|--------|----------|-------|
| Version | JUCE 8.0.x | Latest stable, CMake-native |
| C++ Standard | C++17 minimum | C++20 recommended for new projects |
| Build System | CMake 3.22+ | FetchContent for self-contained builds |
| Plugin Formats | VST3 (all), AU (macOS), Standalone | AAX optional, requires Avid SDK |

### Audio Engineering Standards
| Standard | Application |
|----------|-------------|
| IEEE 754 | Floating-point audio processing |
| AES17 | Digital audio measurement |
| VST3 SDK | Plugin interface specification |
| CLAP | Open plugin standard |

## Expert Recommendations

### DSP Implementation (Pirkle, Zölzer, Smith)
1. **Filter Design**
   - Use SVF for musical filters (stable, efficient)
   - Pre-warp analog prototypes for digital conversion
   - Test at multiple sample rates (44.1k, 48k, 96k, 192k)

2. **Oscillator Design**
   - BLIT/MinBLEP for classic waveforms
   - Wavetable for complex waveforms
   - Phase distortion for character

3. **Dynamics**
   - Attack/release time constant interpretation
   - Lookahead for transparent limiting
   - RMS vs peak detection trade-offs

4. **Reverb/Space**
   - FDN for efficient reverb
   - Allpass for diffusion
   - Delay line interpolation for smooth results

### Code Quality Patterns
| Pattern | Purpose | Implementation |
|---------|---------|----------------|
| Two-stage review | Catch implementation + quality bugs | Spec compliance + code quality passes |
| Pre-allocation | Avoid audio thread allocations | All buffers in prepareToPlay() |
| Atomic flags | Cross-thread communication | std::atomic<bool> for GUI ↔ audio |
| ScopedNoDenormals | CPU stability | Always at top of processBlock |

### JUCE-Specific Best Practices
```cpp
// Parameter layout (APVTS)
// Use parameter IDs that won't change after release
const juce::String paramGainID = "gain";

// Audio thread safety
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
    juce::ScopedNoDenormals noDenormals;  // Always first
    
    // Pre-allocated member buffer for processing
    // No allocations, no system calls
}

// State save/restore
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

## Common Pitfalls & Prevention

| Pitfall | Prevention |
|---------|------------|
| Assuming buffer size | Always use buffer.getNumSamples() |
| Forgetting mono handling | Check buffer.getNumChannels(), copy ch0→ch1 |
| Static state | Never use static for mutable members |
| Missing listener cleanup | Every addListener needs matching remove |
| Unbounded feedback | Clamp to 0.95, add soft-clip |
| Linear dry/wet mix | Use equal-power cosine mixing |
| SmoothedValue misuse | Call getNextValue() per-sample in loop |

## Testing Standards

### Unit Test Coverage (Catch2/GoogleTest)
| Test Type | Required |
|-----------|----------|
| Parameter ranges | All parameters |
| State round-trip | Save → restore → compare |
| Silence processing | 60s, CPU stable |
| Extreme values | All params min, all max |
| Multi-instance | 4+ simultaneous |
| Sample rates | 44.1k, 48k, 96k, 192k |

### Validation Tools
| Tool | Purpose |
|------|---------|
| pluginval | JUCE validation, strictness 5+ |
| ASIO4ALL | Windows audio testing |
| REAPER | Primary DAW test environment |
| valgrind | Memory leak detection (Linux) |
| perf | CPU profiling (Linux) |

## Sources
- Designing Audio Effect Plugins in C++ (Will Pirkle)
- DAFX: Digital Audio Effects (Udo Zölzer)
- Physical Audio Signal Processing (Julius O. Smith)
- JUCE Documentation & Forum
- Real-Time Safe Multi-Threaded DAW Audio (Ed Loeffler)