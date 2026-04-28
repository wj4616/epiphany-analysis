# Domain Analysis: VST Plugin Development & Commercial Release

## 1. Standard Development Process

### 1.1 Development Lifecycle Phases

| Phase | Activities | Key Artifacts |
|-------|------------|---------------|
| **Concept** | Plugin type, signal flow, sound identity | SPEC.md, parameter list |
| **Architecture** | Module structure, dependencies | Architecture doc, CMake setup |
| **DSP Implementation** | Audio processing algorithms | Processor class, DSP modules |
| **Parameter Management** | APVTS, smoothing, state | Parameter layout, presets |
| **GUI Design** | Editor, controls, LookAndFeel | PluginEditor, UI assets |
| **Testing** | Unit tests, DAW testing, profiling | Test suite, test results |
| **Build** | Release builds, code signing | VST3 binary, installer |
| **Commercial** | Licensing, marketplace, marketing | Licensed plugin, listings |

### 1.2 Critical Technical Standards

**Audio Thread Safety (Ross Bencina Rules):**
- No memory allocation/deallocation in audio callback
- No mutexes/locks (priority inversion)
- No I/O (disk, network)
- No blocking OS calls
- No unpredictable timing

**Parameter Smoothing:**
- Use `SmoothedValue` for all audio parameters
- Linear vs Multiplicative smoothing based on parameter type
- `reset(sampleRate, rampLength)` in `prepareToPlay()`
- `getNextValue()` per-sample in `processBlock()`

**State Management:**
- `AudioProcessorValueTreeState` for parameter management
- `getStateInformation()`/`setStateInformation()` for preset save/load
- XML serialization with `copyXmlToBinary()`/`getXmlFromBinary()`

## 2. DSP Algorithm Categories

### 2.1 Synthesis Types

| Type | Description | Applications |
|------|-------------|--------------|
| **Subtractive** | Oscillator → Filter → Amp | Classic synths, bass, leads |
| **FM** | Carrier + Modulator oscillators | Bells, metallic, electric piano |
| **Wavetable** | Scanning through waveforms | Evolving pads, modern synths |
| **Granular** | Processing sound grains | Textures, atmospheres, psybient |
| **Additive** | Sum of sine harmonics | Organs, spectral synthesis |
| **Hybrid** | Multiple methods combined | Complex, unique timbres |

### 2.2 Effect Categories

| Category | Algorithms | Parameters |
|----------|------------|------------|
| **Dynamics** | Compressor, Limiter, Gate, Expander | Threshold, Ratio, Attack, Release |
| **EQ/Filter** | Parametric, Shelving, Graphic, Resonant | Frequency, Q, Gain |
| **Time-based** | Delay, Reverb, Chorus, Flanger, Phaser | Time, Feedback, Mix |
| **Distortion** | Saturation, Waveshaping, Bitcrush | Drive, Tone, Mix |
| **Modulation** | Tremolo, Vibrato, Ring Mod | Rate, Depth |
| **Spatial** | Stereo Width, Panner, 3D | Width, Position |

### 2.3 Psychedelic/Ambient Sound Design Techniques

From research on psybient production:

1. **Granular Textures**: Long grain sizes (200-800ms), high density, freeze positions
2. **No-Input Feedback**: Self-oscillating feedback loops with tuned EQ boosts
3. **Recursive Reverb**: Reverbs that feed their own reflections back
4. **Asynchronous Modulation**: Multiple unsynced LFOs at unrelated speeds
5. **Harmonic Layering**: Saturation as harmonic generator
6. **Worldizing**: Recording sounds through speakers in real spaces
7. **Controlled Glitch**: Subtle (10%) textural glitch effects

**Key Artists**: Shpongle, Ott, Carbon Based Lifeforms, Solar Fields

## 3. JUCE Framework Specifics

### 3.1 Build System (CMake + FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.10
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(YourPlugin
    PLUGIN_MANUFACTURER_CODE Xxxx
    PLUGIN_CODE Yyyy
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Your Plugin"
    COMPANY_NAME "Your Company"
)
```

### 3.2 Required Linux Dependencies

```bash
libx11-xcb-dev libxcb-util-dev libxcb-cursor-dev libxcb-xkb-dev
libxkbcommon-dev libxkbcommon-x11-dev libfontconfig1-dev libcairo2-dev
libgtkmm-3.0-dev libsqlite3-dev libxcb-keysyms1-dev
```

### 3.3 AudioProcessor Implementation Pattern

```cpp
class PluginProcessor : public juce::AudioProcessor
{
public:
    // Constructor: initialize APVTS
    PluginProcessor() : apvts(*this, nullptr, "Parameters", createParameterLayout()) {}

    // prepareToPlay: initialize DSP, smoothing
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    // processBlock: audio processing (NO allocations, NO locks)
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    // State management
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    juce::AudioProcessorValueTreeState apvts;
    // SmoothedValue for each parameter
};
```

## 4. Failure Modes Catalog

### 4.1 Critical Audio Thread Violations (FM-01 to FM-07)

| ID | Failure | Prevention |
|----|---------|-------------|
| FM-01 | Memory allocation in processBlock | Pre-allocate all buffers |
| FM-02 | Lock acquisition in processBlock | Use lock-free queues |
| FM-03 | Blocking I/O in processBlock | Async message passing |
| FM-04 | Unbounded iteration | Worst-case bounds |
| FM-05 | Virtual function calls with unbounded dispatch | Final methods |
| FM-06 | Dynamic casting | Static type design |
| FM-07 | Exception throwing | No exceptions in audio code |

### 4.2 Parameter Failures (EM-xx)

| ID | Failure | Prevention |
|----|---------|-------------|
| EM-13 | Dead parameters (not connected to DSP) | Audit parameter → DSP connections |
| CM-05 | Once-per-block instead of per-sample smoothing | Use getNextValue() per sample |

### 4.3 Gain Staging Failures

| ID | Failure | Prevention |
|----|---------|-------------|
| EM-01 | Excessive conservative gain staging | Audit early, measure signal levels |
| EM-02 | Formula errors at boundaries | Test param=0, param=1, param=max |

## 5. Testing Standards

### 5.1 Unit Testing (Catch2)

```cpp
TEST_CASE("Filter frequency response", "[DSP]")
{
    Filter filter;
    filter.setCutoff(1000.0f);
    filter.prepare({44100.0, 512, 1});

    // Test impulse response
    juce::AudioBuffer<float> buffer(1, 512);
    buffer.setSample(0, 0, 1.0f);  // Impulse
    buffer.clear(1, 511);

    juce::MidiBuffer midi;
    filter.process(buffer, midi);

    // Verify frequency response
}
```

### 5.2 DAW Testing Checklist

- [ ] Plugin loads in REAPER
- [ ] Audio passes through correctly
- [ ] Parameters respond and smooth correctly
- [ ] Presets save/load correctly
- [ ] No clicks/pops when changing parameters
- [ ] No crashes with extreme settings
- [ ] CPU usage acceptable
- [ ] Works at 44.1kHz, 48kHz, 96kHz
- [ ] Works with various buffer sizes (64, 128, 256, 512, 1024)

## 6. Sources

- [JUCE Official Course](https://juce.com/learn/course/)
- [AudioProcessor Reference](https://docs.juce.com/master/classAudioProcessor.html)
- [SmoothedValue Reference](https://docs.juce.com/master/classSmoothedValue.html)
- [Ross Bencina - Real-time Audio Programming](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing)
- [Will Pirkle - Designing Audio Effect Plugins](https://www.routledge.com/Designing-Audio-Effect-Plugins-in-C-For-AAX-AU-and-VST3-with-DSP-Theory/Pirkle/p/book/9781138591936)
- [Ambient Sound Design Techniques](https://artistsindsp.com/ambient-sound-design-7-advanced-techniques-for-evolving-drones-and-textures/)
- [JanWilczek Audio Plugin Template](https://github.com/JanWilczek/audio-plugin-template)
- [KristofferKarlAxelEkstrand JUCE Template](https://github.com/KristofferKarlAxelEkstrand/juce-project-template)