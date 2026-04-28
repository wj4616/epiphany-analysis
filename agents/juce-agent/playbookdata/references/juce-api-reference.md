# JUCE 7.0.12 Complete API Reference
## Source: Local JUCE headers at build/_deps/juce-src/modules/

This reference was extracted directly from the JUCE 7.0.12 source headers. It covers every module and key class relevant to plugin development.

---

## Module Overview (21 modules)

| Module | Description | License |
|---|---|---|
| **juce_core** | Text, containers, memory, threading, I/O | ISC |
| **juce_events** | Message/event handling, timers | ISC |
| **juce_data_structures** | Undo/redo, ValueTree | GPL/Commercial |
| **juce_graphics** | 2D vector graphics, images, fonts | GPL/Commercial |
| **juce_gui_basics** | UI components | GPL/Commercial |
| **juce_gui_extra** | Specialized GUI (WebBrowser, etc.) | GPL/Commercial |
| **juce_audio_basics** | Audio buffers, MIDI, synthesis | ISC |
| **juce_audio_devices** | Audio/MIDI I/O (ALSA, JACK, CoreAudio, WASAPI) | ISC |
| **juce_audio_formats** | Audio file reading/writing | GPL/Commercial |
| **juce_audio_processors** | Plugin framework (VST/AU/AAX) | GPL/Commercial |
| **juce_audio_plugin_client** | Plugin client wrapper | GPL/Commercial |
| **juce_audio_utils** | Audio GUI widgets | GPL/Commercial |
| **juce_dsp** | DSP: filters, effects, math | GPL/Commercial |
| **juce_analytics** | Analytics tracking | GPL/Commercial |
| **juce_box2d** | Box2D physics | GPL/Commercial |
| **juce_cryptography** | Hash/encryption | GPL/Commercial |
| **juce_midi_ci** | MIDI Capability Inquiry | GPL/Commercial |
| **juce_opengl** | OpenGL rendering | GPL/Commercial |
| **juce_osc** | Open Sound Control | GPL/Commercial |
| **juce_product_unlocking** | In-app purchase/activation | GPL/Commercial |
| **juce_video** | Video playback | GPL/Commercial |

**Typical plugin uses:** juce_audio_processors, juce_audio_basics, juce_dsp, juce_gui_basics, juce_audio_utils

---

## juce_audio_processors — Plugin Framework

### AudioProcessor (base class for ALL plugins)

**Must implement:**
- `getName()` — plugin name
- `prepareToPlay(double sampleRate, int maxBlockSize)` — setup DSP
- `releaseResources()` — cleanup
- `processBlock(AudioBuffer<float>&, MidiBuffer&)` — THE audio callback (real-time thread)
- `createEditor()` — return GUI component
- `hasEditor()` — return true if GUI exists
- `getStateInformation(MemoryBlock&)` / `setStateInformation(const void*, int)` — save/recall
- `acceptsMidi()` / `producesMidi()` — MIDI capability

**Important methods:**
- `getTotalNumInputChannels()` / `getTotalNumOutputChannels()`
- `getSampleRate()`, `getBlockSize()`
- `BusesProperties` — declare I/O bus layouts
- `isBusesLayoutSupported(BusesLayout)` — validate channel configs
- `suspendProcessing(true)` — pause audio for heavy operations (preset loading, IR changes)

### AudioProcessorEditor (GUI base)
- `processor` member — reference to AudioProcessor
- `setResizable(bool, bool)`, `setResizeLimits(min/max)`
- `setScaleFactor(float)` — DPI scaling

### AudioProcessorValueTreeState (APVTS)
- Constructor: `APVTS(processor, undoManager, "STATE", ParameterLayout)`
- `getRawParameterValue(StringRef paramID)` — returns `atomic<float>*` for real-time read
- `getParameter(StringRef paramID)` — returns `RangedAudioParameter*`
- `copyState()` / `replaceState(ValueTree)` — thread-safe state get/set
- `state` — the underlying ValueTree
- `addParameterListener()` / `removeParameterListener()`

### Parameter Types
| Type | Use |
|---|---|
| `AudioParameterFloat` | Continuous float with NormalisableRange |
| `AudioParameterInt` | Integer with min/max |
| `AudioParameterBool` | On/off toggle |
| `AudioParameterChoice` | String choice list |

### Parameter Attachments (connect APVTS to GUI)
| Attachment | Binds to |
|---|---|
| `SliderParameterAttachment` | Slider |
| `ComboBoxParameterAttachment` | ComboBox |
| `ButtonParameterAttachment` | ToggleButton |
| `ParameterAttachment` | Custom controls |

### Other Classes
- `AudioProcessorGraph` — node-based audio routing
- `PluginHostType` — detect which DAW is hosting your plugin
- `GenericAudioProcessorEditor` — auto-generated editor

---

## juce_audio_basics — Core Audio Types

### AudioBuffer<float>
- `AudioBuffer(numChannels, numSamples)` — allocate
- `getNumChannels()`, `getNumSamples()`
- `getReadPointer(channel)`, `getWritePointer(channel)` — raw float*
- `clear()`, `clear(channel, start, count)`
- `addFrom(destCh, destStart, source, srcCh, srcStart, count, gain)`
- `copyFrom(destCh, destStart, source, srcCh, srcStart, count)`
- `applyGain(channel, start, count, gain)`
- `applyGainRamp(channel, start, count, startGain, endGain)`
- `getMagnitude(channel, start, count)`, `getRMSLevel(...)`

### MidiBuffer
- Range-for: `for (const auto metadata : midiBuffer)`
- `addEvent(MidiMessage, samplePosition)`
- `clear()`, `isEmpty()`
- Metadata: `.getMessage()`, `.samplePosition`, `.numBytes`

### MidiMessage
- Static: `noteOn()`, `noteOff()`, `controllerEvent()`, `pitchWheel()`
- Query: `isNoteOn()`, `getNoteNumber()`, `getVelocity()`, `getControllerNumber()`, `getControllerValue()`

### SmoothedValue<float>
- `reset(sampleRate, rampLengthSeconds)`
- `setCurrentAndTargetValue(value)` — snap immediately
- `setTargetValue(value)` — smooth ramp
- `getNextValue()` — call per sample (MUST advance every sample)
- `isSmoothing()`, `skip(numSamples)`

### ADSR
- `setSampleRate(double)`
- `setParameters(Parameters{attack, decay, sustain, release})` — seconds (sustain = 0–1 level)
- `noteOn()`, `noteOff()`
- `getNextSample()` — returns 0–1
- `applyEnvelopeToBuffer(AudioBuffer&, start, count)`

### Synthesiser / SynthesiserVoice / SynthesiserSound
- Polyphonic voice management with stealing
- Subclass `SynthesiserVoice`: override `renderNextBlock()`
- `Synthesiser::addVoice()`, `addSound()`, `renderNextBlock()`

### Other
- `Decibels::decibelsToGain()` / `gainToDecibels()`
- `FloatVectorOperations` — SIMD-optimized bulk operations
- `AudioPlayHead` — get transport position from DAW (BPM, time sig, bar position)
- `AudioChannelSet` — mono, stereo, 5.1, etc.
- `LagrangeInterpolator`, `WindowedSincInterpolator`, `LinearInterpolator`

---

## juce_dsp — Complete DSP Toolkit

### Infrastructure
| Class | Purpose |
|---|---|
| `ProcessSpec` | Setup info: sampleRate, maximumBlockSize, numChannels |
| `AudioBlock<float>` | Lightweight non-owning view into channel data |
| `ProcessContextReplacing<float>` | In-place processing context |
| `ProcessContextNonReplacing<float>` | Separate I/O context |
| `ProcessorChain<A, B, C...>` | Chain processors in series |
| `ProcessorDuplicator<Mono, State>` | Mono→multi-channel duplication |

### Filters
| Class | Description |
|---|---|
| `IIR::Filter<float>` | Biquad (Direct Form II Transposed) |
| `IIR::Coefficients<float>` | Static factories: `makeLowPass`, `makeHighPass`, `makeBandPass`, `makeNotch`, `makeAllPass`, `makeLowShelf`, `makeHighShelf`, `makePeakFilter` |
| `FIR::Filter<float>` | FIR filter (efficient for <128 taps) |
| `StateVariableTPTFilter<float>` | 12dB/oct SVF — safe for fast modulation, LP/HP/BP outputs |
| `FirstOrderTPTFilter<float>` | 6dB/oct first-order TPT — LP/HP/allpass |
| `LinkwitzRileyFilter<float>` | 24dB/oct crossover — flat summing LP+HP |
| `LadderFilter<float>` | Moog ladder — LPF12/HPF12/BPF12/LPF24/HPF24/BPF24, drive, resonance |
| `BallisticsFilter<float>` | Attack/release envelope follower |
| `FilterDesign<float>` | High-order FIR/IIR design (Kaiser, Chebyshev, elliptic) |

### Effects
| Class | Description |
|---|---|
| `Compressor<float>` | threshold (dB), ratio, attack (ms), release (ms) |
| `Limiter<float>` | Dual-stage compressor + hard clip at 0dB |
| `NoiseGate<float>` | Gate with threshold, ratio, attack, release |
| `Chorus<float>` | rate, depth, centreDelay (ms), feedback, mix |
| `Phaser<float>` | 6-stage: rate, depth, centreFrequency, feedback, mix |
| `Reverb` | Wrapper around Freeverb: roomSize, damping, wet/dry/width/freeze |
| `Convolution` | FFT-based IR convolution, thread-safe IR loading |
| `WaveShaper<float>` | Arbitrary waveshaping function |
| `Oscillator<float>` | Function-based with optional lookup table |
| `Gain<float>` | Smoothed gain: setGainLinear/setGainDecibels |
| `Bias<float>` | DC offset |
| `Panner<float>` | Multiple pan laws: linear, balanced, sin3dB, sin4p5dB, sin6dB, sqrt3dB, sqrt4p5dB |
| `DryWetMixer<float>` | Dry/wet with latency compensation, multiple mixing rules |
| `DelayLine<float>` | Fractional delay: None/Linear/Lagrange3rd/Thiran interpolation |
| `Oversampling<float>` | 2x/4x/8x/16x with FIR or IIR anti-aliasing |

### Math / Utility
| Class | Description |
|---|---|
| `FFT` | Forward/inverse FFT, frequency-only mapping |
| `WindowingFunction<float>` | rectangular, triangular, hann, hamming, blackman, blackmanHarris, flatTop, kaiser |
| `SIMDRegister<float>` | SIMD wrapper (SSE/NEON) |
| `Matrix<float>` | General matrix operations |
| `Polynomial<float>` | Polynomial evaluation |
| `Phase<float>` | Phase accumulator for oscillators |
| `LookupTable<float>` | Fast function approximation |
| `LogRampedValue<float>` | Logarithmic smoothing |
| `FastMathApproximations` | Fast sin, cos, tan, sinh, cosh, tanh, exp, log, pow, sqrt |
| `SpecialFunctions` | Erfc, Bessel functions |

### What's built-in vs needs custom implementation

**Built-in (use directly):**
All standard filter types, Moog ladder, Linkwitz-Riley crossover, compressor, limiter, noise gate, chorus, phaser, basic reverb, convolution, waveshaper, oscillator, delay line, oversampling, FFT, windowing, gain, panning, dry/wet mixing, envelope follower

**Needs custom implementation:**
- Advanced reverb (FDN, plate, shimmer) — juce::Reverb is basic Freeverb
- Granular processing
- Pitch shifting / time stretching
- Anti-aliased oscillators (PolyBLEP) — juce::Oscillator is naive
- Multi-band EQ (compose from IIR filters)
- Specific delay effects (ping-pong, multi-tap, reverse)
- Vocoder
- Sidechain processing logic
- Stereo widening beyond basic panning
- Wavefolder (beyond basic waveshaper)
- Multiband compression (compose from LinkwitzRiley + Compressor)

---

## juce_audio_formats — File I/O

| Class | Purpose |
|---|---|
| `AudioFormatManager` | Register/manage audio codecs; `registerBasicFormats()` |
| `AudioFormatReader` / `AudioFormatWriter` | Read/write audio files |
| `WavAudioFormat` | WAV codec |
| `AiffAudioFormat` | AIFF codec |
| `FlacAudioFormat` | FLAC codec |
| `OggVorbisAudioFormat` | Ogg Vorbis codec |
| `MP3AudioFormat` | MP3 decoder (opt-in) |
| `CoreAudioFormat` | macOS/iOS AAC, ALAC, etc. |

---

## juce_audio_utils — Audio GUI Widgets

| Class | Purpose |
|---|---|
| `AudioProcessorPlayer` | Connect AudioProcessor to AudioDeviceManager |
| `AudioDeviceSelectorComponent` | Device selection GUI |
| `AudioThumbnail` / `AudioThumbnailCache` | Waveform display |
| `AudioVisualiserComponent` | Real-time level visualizer |
| `MidiKeyboardComponent` | On-screen MIDI keyboard |

---

## juce_gui_basics — UI Components

### Core
- `Component` — base for all UI; `paint()`, `resized()`, `addAndMakeVisible()`, `setBounds()`
- `LookAndFeel_V4` — theme system
- `Graphics` — 2D drawing context

### Widgets
- `Slider` — rotary/linear knobs
- `Label` — text display
- `ComboBox` — dropdown
- `TextButton` / `ToggleButton` — buttons
- `TextEditor` — text input
- `ListBox` / `TableListBox` — lists
- `TreeView` — hierarchical tree
- `ProgressBar` — progress

### Layout
- `FlexBox` / `FlexItem` — CSS flexbox
- `Grid` / `GridItem` — CSS grid
- `GroupComponent` — labeled group
- `TabbedComponent` — tabs
- `ComponentAnimator` — animate bounds
- `ComponentBoundsConstrainer` — resize constraints

### Windows
- `DocumentWindow` / `DialogWindow` — window types
- `AlertWindow` / `NativeMessageBox` — dialogs
- `CallOutBox` — popup callout
- `TooltipWindow` — tooltips
- `FileChooser` — native file dialog

---

## juce_audio_devices — Hardware I/O

| Class | Purpose |
|---|---|
| `AudioDeviceManager` | Audio device selection/config |
| `MidiInput` / `MidiOutput` | MIDI hardware I/O |
| `MidiMessageCollector` | Device→MidiBuffer bridge |
| `AudioTransportSource` | Playback with start/stop/position |

**Backends:** ALSA, JACK (Linux), CoreAudio/CoreMIDI (macOS/iOS), WASAPI/DirectSound/ASIO (Windows), Oboe (Android)

---

## Dependency Graph

```
juce_core
  └─ juce_events
       ├─ juce_data_structures
       └─ juce_graphics
            └─ juce_gui_basics
                 └─ juce_gui_extra
  └─ juce_audio_basics
       ├─ juce_audio_formats
       │    └─ juce_dsp
       ├─ juce_audio_devices
       └─ juce_audio_processors (+ juce_gui_extra)
            ├─ juce_audio_plugin_client
            └─ juce_audio_utils
```
