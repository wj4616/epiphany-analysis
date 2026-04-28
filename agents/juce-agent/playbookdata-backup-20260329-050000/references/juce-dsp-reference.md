# JUCE DSP Reference: Built-in Classes & Algorithm Catalog
## Compiled from: JUCE docs, chowdsp_utils, awesome-juce, Stanford 320c

This reference covers what JUCE provides out-of-the-box, what the community provides, and common DSP algorithms with implementation notes.

---

## 1. Built-in juce::dsp Classes

### Filters
| Class | Description | When to use |
|---|---|---|
| `dsp::IIR::Filter` | Standard IIR (biquad) filter | Simple EQ bands, basic filtering |
| `dsp::IIR::Coefficients` | Biquad coefficient computation | LP, HP, BP, notch, shelf, peak EQ |
| `dsp::FIR::Filter` | Finite impulse response filter | Linear phase filtering, crossovers |
| `dsp::StateVariableTPTFilter` | Topology-preserving SVF | Modulated filters, resonant sweeps — stable at all frequencies |
| `dsp::FirstOrderTPTFilter` | First-order TPT filter | Simple LP/HP, DC blocking, one-pole smoothing |
| `dsp::LinkwitzRileyFilter` | Crossover filter (flat summing) | Multi-band processing, crossovers |
| `dsp::LadderFilter` | Moog-style 4-pole ladder | Analog-style resonant filtering |
| `dsp::FilterDesign` | Design utilities | Generating coefficients for various filter types |

### Delay & Time-based
| Class | Description | When to use |
|---|---|---|
| `dsp::DelayLine` | Variable-length delay with interpolation | Delays, reverbs, chorus, flanger |
| `dsp::DelayLineInterpolationTypes::Linear` | Linear interpolation | General purpose, good quality |
| `dsp::DelayLineInterpolationTypes::Lagrange3rd` | 3rd-order Lagrange | Higher quality pitch shifting |
| `dsp::DelayLineInterpolationTypes::Thiran` | Allpass interpolation | Fractional delay, physical modeling |
| `dsp::DelayLineInterpolationTypes::None` | No interpolation | Integer-sample delays only |

### Effects
| Class | Description | When to use |
|---|---|---|
| `dsp::Chorus` | Modulated delay chorus | Chorus/ensemble effect |
| `dsp::Phaser` | Allpass chain phaser | Phaser effect |
| `dsp::Reverb` | Simple room reverb | Basic reverb (limited quality — custom FDN preferred for serious use) |
| `dsp::Convolution` | FFT-based convolution | IR reverbs, cabinet simulation |
| `dsp::WaveShaper` | Transfer function distortion | Saturation, clipping, waveshaping |

### Dynamics
| Class | Description | When to use |
|---|---|---|
| `dsp::Compressor` | Basic compressor | Simple dynamic range compression |
| `dsp::Limiter` | Brick-wall limiter | Output protection |
| `dsp::NoiseGate` | Noise gate | Signal suppression below threshold |
| `dsp::BallisticsFilter` | Envelope follower (attack/release) | Level detection, metering, sidechain |

### Mixing & Routing
| Class | Description | When to use |
|---|---|---|
| `dsp::DryWetMixer` | Dry/wet mixing | Effect mix control (handles latency compensation) |
| `dsp::Gain` | Level control | Volume, gain staging |
| `dsp::Panner` | Stereo panning | Pan control |
| `dsp::ProcessorChain` | Sequential DSP chain | Chaining multiple processors |
| `dsp::ProcessorDuplicator` | Mono→stereo duplication | Running mono processor on stereo signal |

### Synthesis
| Class | Description | When to use |
|---|---|---|
| `dsp::Oscillator` | Wavetable oscillator | Tone generation, LFOs |
| `ADSR` (juce_audio_basics) | Envelope generator | Amplitude/filter envelopes |
| `Synthesiser` (juce_audio_basics) | Voice allocator | Polyphonic instrument plugins |
| `SynthesiserVoice` | Single voice | Per-voice processing |
| `SynthesiserSound` | Sound mapping | MIDI note→voice routing |

### Analysis & Math
| Class | Description | When to use |
|---|---|---|
| `dsp::FFT` | Fast Fourier Transform | Spectrum analysis, spectral processing |
| `dsp::WindowingFunction` | Hann, Hamming, Blackman, etc. | FFT windowing, grain envelopes |
| `dsp::LookupTable` | Precomputed function table | Fast sin/cos/tanh approximation |
| `dsp::FastMathApproximations` | Approximate sin, cos, tan, exp, log | CPU savings in inner loops |
| `dsp::Matrix` | Matrix operations | FDN mixing matrices, spatial audio |
| `dsp::Oversampling` | Up/downsampling | Anti-aliasing for nonlinear processing |
| `dsp::SIMDRegister` | SIMD vector operations | Vectorized per-sample processing |
| `dsp::AudioBlock` | Block-level audio view | Efficient buffer manipulation |

---

## 2. Community DSP Libraries

### chowdsp_utils (largest community library)
| Module | Provides | License |
|---|---|---|
| `chowdsp_filters` | Butterworth, Chebyshev, Elliptic, SVF, FIR, Werner filters, ARP 1047 emulation | GPLv3/BSD |
| `chowdsp_dsp_utils` | Advanced delay lines, pitch shifters, convolution, resampling | GPLv3 |
| `chowdsp_waveshapers` | ADAA waveshaping (hard/soft clip, tanh, rectifiers), wavefolding | GPLv3 |
| `chowdsp_reverb` | FDN reverb templates | GPLv3 |
| `chowdsp_compressor` | Level detectors, gain computers | GPLv3 |
| `chowdsp_eq` | EQ bands, multi-band, linear phase EQ | GPLv3 |
| `chowdsp_sources` | Anti-aliased oscillators (saw, square, triangle), noise generators | GPLv3 |
| `chowdsp_modal_dsp` | Modal filters for physical modeling | GPLv3 |
| `chowdsp_simd` | XSIMD wrapper, SIMD math | GPLv3 |
| `chowdsp_buffers` | SIMD-optimized audio buffers | GPLv3 |
| `chowdsp_plugin_state` | APVTS replacement with better performance | BSD |
| `chowdsp_presets` | Preset management system | BSD |

### Other notable DSP resources
| Library | What it provides | URL |
|---|---|---|
| Gin (FigBug) | Large utility collection, StackBlur, websockets | github.com/FigBug/Gin |
| sst-filters | Surge synthesizer filters as standalone library | github.com/surge-synthesizer/sst-filters |
| sst-waveshapers | Surge waveshapers as standalone library | github.com/surge-synthesizer/sst-waveshapers |
| signalsmith-stretch | Polyphonic pitch/time stretching | github.com/Signalsmith-Audio/signalsmith-stretch |
| signalsmith-dsp | General DSP utilities | github.com/Signalsmith-Audio/dsp |
| Audio-Effects | Book examples: delay, chorus, flanger, phaser, tremolo, ring mod, distortion, compressor | github.com/juandagilc/Audio-Effects |
| ob_HIIR | Laurent DeSoras' upsampling/downsampling library | github.com/AndrewJJ/ob_HIIR |
| AudioTK | Workflow/pipeline DSP toolkit, circuit modeling | github.com/AudioTK/AudioTK |

---

## 3. DSP Algorithm Catalog

### Filters

#### Biquad (IIR 2nd order)
- **Use:** EQ bands, simple LP/HP/BP/notch
- **JUCE:** `dsp::IIR::Filter` with `dsp::IIR::Coefficients::makeLowPass()` etc.
- **CPU:** Very low (2 multiply-adds per sample)
- **Pitfall:** Unstable at very low frequencies relative to sample rate; coefficient recomputation needed on sample rate change
- **Formula:** `y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]`

#### Cytomic TPT State Variable Filter
- **Use:** Modulated filters, resonant sweeps — THE preferred filter for plugins
- **JUCE:** `dsp::StateVariableTPTFilter` (implements Andy Simper's Cytomic TPT design)
- **CPU:** Low (few multiply-adds per sample)
- **Key advantage:** Unconditionally stable at all frequencies, click-free modulation
- **Formula:** Uses trapezoidal integration: `g = tan(pi * fc / fs)`
- **Outputs:** LP, HP, BP simultaneously from same computation
- **When to use over biquad:** Always, unless you need specific biquad topology for emulation
- **WARNING:** Do NOT confuse with the **Chamberlin SVF** (`f = 2*sin(pi*fc/fs)`). The Chamberlin SVF is unstable at high frequencies and clicks during modulation. JUCE's `StateVariableTPTFilter` is the correct (TPT) implementation. If you see `2*sin(...)` in SVF code, it's the wrong algorithm.
- **Reference:** Vadim Zavalishin — "The Art of VA Filter Design" (free PDF); Andy Simper (Cytomic) papers at cytomic.com/files/dsp/

#### Moog Ladder Filter
- **Use:** Analog-style 4-pole (24dB/oct) resonant filter
- **JUCE:** `dsp::LadderFilter`
- **CPU:** Moderate (4 cascaded stages)
- **Note:** Self-oscillates at high resonance; drive parameter adds saturation

#### Linkwitz-Riley Crossover
- **Use:** Multi-band processing (splitting signal into frequency bands)
- **JUCE:** `dsp::LinkwitzRileyFilter`
- **Key property:** Flat magnitude response when LP + HP outputs are summed
- **Common orders:** 2nd (12dB/oct), 4th (24dB/oct)

#### DC Blocker
- **Use:** Remove DC offset from signal
- **Implementation:** First-order highpass at ~5Hz
- **Formula:** `y[n] = x[n] - x[n-1] + 0.995 * y[n-1]`

### Reverb

#### Feedback Delay Network (FDN)
- **Use:** Algorithmic reverb (dense, colorless diffusion)
- **Custom implementation required** (juce::dsp::Reverb is basic)
- **Components:** N delay lines + N×N mixing matrix + per-line feedback
- **Typical sizes:** 4, 8, or 16 lines
- **Delay lengths:** Use mutually prime numbers to avoid coloration
- **Mixing matrices:** Hadamard (orthogonal, normalized by 1/sqrt(N)) or Householder
- **Damping:** One-pole lowpass per line in feedback path
- **Decay formula:** `gain = pow(10, -3 * delayLength / (decaySeconds * sampleRate))`
- **CPU:** Moderate to high (scales with N)

#### Schroeder Reverb
- **Use:** Classic digital reverb (historical reference, less used now)
- **Components:** Parallel comb filters → series allpass filters
- **CPU:** Low
- **Quality:** Metallic coloration; FDN preferred for modern use

#### Convolution Reverb
- **Use:** Realistic room simulation using impulse responses
- **JUCE:** `dsp::Convolution`
- **CPU:** High (FFT-based partitioned convolution)
- **Latency:** Introduces latency equal to first partition size
- **Note:** Use `suspendProcessing(true)` when loading new IRs

#### Dattorro Plate Reverb
- **Use:** Plate-style reverb with rich diffusion
- **Reference implementation:** mverb (github.com/FigBug/mverb)
- **Components:** Input diffusion → tank (modulated allpass loops)

### Delay Effects

#### Simple Delay
- **Use:** Echo, slapback
- **JUCE:** `dsp::DelayLine`
- **Parameters:** time (ms), feedback (0–0.95), mix
- **Interpolation:** Linear for basic, Lagrange3rd for pitch-modulated

#### Chorus
- **Use:** Thickening, ensemble effect
- **JUCE:** `dsp::Chorus` (or custom with `dsp::DelayLine` + LFO)
- **Parameters:** rate (0.1–5Hz), depth (1–20ms), mix
- **Implementation:** Short delay (5–30ms) modulated by LFO

#### Flanger
- **Use:** Metallic sweep, jet-plane effect
- **Implementation:** Very short delay (0.1–10ms) modulated by LFO with feedback
- **Difference from chorus:** Shorter delay, higher feedback, more pronounced comb filtering

#### Comb Filter
- **Use:** Metallic resonance, Karplus-Strong synthesis
- **Implementation:** `y[n] = x[n] + g * y[n - D]` (feedback comb)
- **Or:** `y[n] = x[n - D] + g * x[n]` (feedforward comb)
- **Resonant frequency:** `fs / D` Hz

### Distortion & Saturation

#### Waveshaping (transfer function)
- **Use:** Saturation, distortion, harmonic enhancement
- **JUCE:** `dsp::WaveShaper`
- **Common curves:** `tanh(x)`, `x / (1 + |x|)` (soft clip), `atan(x)`, hard clip
- **Note:** Use oversampling (`dsp::Oversampling`) to reduce aliasing

#### Wavefolder
- **Use:** Complex harmonic generation, west-coast synthesis
- **Full formula:** `output = input + drive * (sin(input * foldAmt * PI) / foldAmt - input)`
  - At `drive=0`: output = input (true bypass, no processing)
  - At `drive=1`: output = sin(input * foldAmt * PI) / foldAmt (full fold)
  - The `/ foldAmt` normalizes amplitude as fold amount increases
- **Boundary check:** Verify at drive=0 and drive=1 — formula must reduce to identity at drive=0
- **Aliasing:** Use `dsp::Oversampling` (2x–4x) or ADAA (chowdsp_waveshapers) — wavefolder generates dense harmonics
- **Note:** Each fold adds harmonics; multiple passes compound

#### Tube/Valve Saturation
- **Use:** Warm analog-style distortion
- **Approach:** Asymmetric waveshaping (different curves for positive/negative)
- **Community:** SmartGuitarAmp (neural network), Schrammel_OJD, BYOD

#### Bitcrusher
- **Use:** Lo-fi, retro digital distortion
- **Implementation:** Sample-rate reduction (sample & hold) + bit-depth reduction (quantization)
- **Parameters:** bits (1–16), sample rate reduction factor

#### ADAA Waveshaping (Anti-Derivative Anti-Aliasing)
- **Use:** Alias-free nonlinear processing without oversampling
- **Community:** chowdsp_waveshapers provides ADAA implementations
- **Advantage:** Lower CPU than oversampling for equivalent alias reduction

### Dynamics

#### Compressor
- **Use:** Dynamic range control
- **JUCE:** `dsp::Compressor` (basic) or custom
- **Community:** CTAGDRC, SimpleCompressor (with look-ahead), valentine
- **Components:** Level detector (RMS or peak) → gain computer (threshold, ratio, knee) → gain stage
- **Formula:** `gainReduction = (input_dB - threshold) * (1 - 1/ratio)` (above threshold)
- **Key parameters:** threshold, ratio, attack, release, makeup gain, knee
- **Architecture pattern (SimpleCompressor):** Separate gain calculation from audio processing:
  1. **Gain reduction computer:** Takes sidechain level → computes gain reduction values (reusable for compressor/limiter/expander)
  2. **Look-ahead processor:** Buffers gain reduction samples, delays them to anticipate peaks before they arrive at output
  3. **Apply stage:** Multiply audio by smoothed gain reduction
- **Sidechain:** Feed external signal to level detector instead of input; gain reduction still applied to main signal
- **Reference:** Reiss — "Tutorial on dynamic range compression" (theory); SimpleCompressor repo (implementation)

#### Limiter
- **Use:** Output protection, loudness maximization
- **JUCE:** `dsp::Limiter`
- **Key:** Look-ahead buffer allows the limiter to react before peaks arrive
- **Output safety:** Always have a limiter or tanh() at the final output

#### Envelope Follower
- **Use:** Amplitude detection, sidechain, auto-wah, ducking
- **JUCE:** `dsp::BallisticsFilter` (attack/release follower)
- **Formula (one-pole):** `env = (|x| > env) ? attackCoeff * (env - |x|) + |x| : releaseCoeff * (env - |x|) + |x|`
- **Coefficients:** `coeff = exp(-1 / (time * sampleRate))`

### Synthesis

#### Wavetable Oscillator
- **Use:** Tone generation with arbitrary waveforms
- **JUCE:** `dsp::Oscillator` (basic) or custom
- **Implementation:** Pre-compute one cycle, read with phase accumulator + interpolation
- **Anti-aliasing:** Band-limit wavetables per octave (mipmap approach) or use PolyBLEP

#### PolyBLEP (Polynomal Band-Limited Step)
- **Use:** Anti-aliased classic waveforms (saw, square, triangle)
- **Community:** chowdsp_sources provides anti-aliased oscillators
- **Advantage:** CPU-efficient alias reduction for hard-edged waveforms

#### FM Synthesis
- **Use:** Metallic, bell-like, evolving timbres
- **Implementation:** Carrier oscillator frequency modulated by modulator oscillator
- **Parameters:** carrier freq, modulator ratio, modulation index
- **Reference:** Dexed (DX7 emulation)

#### Karplus-Strong (Plucked String)
- **Use:** Physical modeling of plucked strings
- **Implementation:** Short noise burst → delay line with LP filter in feedback
- **Parameters:** frequency (from delay length), damping (LP filter cutoff), excitation

#### ADSR Envelope
- **Use:** Amplitude or filter modulation per note
- **JUCE:** `juce::ADSR`
- **Parameters:** attack, decay, sustain (level), release (all in seconds except sustain)

#### Voice Allocation
- **Use:** Polyphonic instrument plugins
- **JUCE:** `juce::Synthesiser` + `juce::SynthesiserVoice`
- **Features:** Voice stealing, note-on/off handling, per-voice rendering
- **Pattern:** Subclass `SynthesiserVoice`, override `renderNextBlock()`

### Spatial / Stereo

#### Mid-Side Processing
- **Use:** Stereo width control
- **Encode:** `mid = (L + R) / 2; side = (L - R) / 2`
- **Decode:** `L = mid + side; R = mid - side`
- **Width:** Scale side signal; 0 = mono, 1 = natural, 2 = extra wide

#### Per-Line FDN Panning
- **Use:** Stereo reverb with motion (used in Epiphany Machine v3)
- **Implementation:** Each FDN delay line gets its own pan position + slow sine LFO
- **Advantage:** Natural stereo motion without mid-side artifacts

#### Equal-Power Panning
- **Formula:** `left = cos(pan * PI/2); right = sin(pan * PI/2)` where pan 0=left, 1=right
- **For mix:** `dryGain = cos(mix * PI/2); wetGain = sin(mix * PI/2)`

### Modulation

#### LFO (Low Frequency Oscillator)
- **Use:** Periodic modulation of any parameter
- **Implementation:** Phase accumulator + waveform lookup
- **Waveforms:** Sine, triangle, saw, square, sample-and-hold (random)
- **Sync:** Optional tempo sync via host BPM

#### Envelope Follower (as modulation source)
- **Use:** Input-reactive modulation
- **Implementation:** Dual one-pole filters (fast attack, slow release)
- **TextureBodyDetector pattern:** `texture = fast_env - slow_env; body = slow_env`

### Analysis

#### FFT Spectrum Analyzer (full pattern from JUCE tutorial)
- **JUCE:** `dsp::FFT` + `dsp::WindowingFunction`
- **Constants:** `fftOrder = 10` or `11`, `fftSize = 1 << fftOrder` (1024 or 2048 points)
- **Members:** `dsp::FFT forwardFFT{fftOrder}`, `std::array<float, fftSize> fifo`, `std::array<float, fftSize * 2> fftData`, `int fifoIndex = 0`, `bool nextFFTBlockReady = false`
- **Pipeline:**
  1. **processBlock:** Push samples into FIFO. When `fifoIndex == fftSize`, copy fifo→fftData, reset index, set ready flag
  2. **Timer/VBlank (UI thread):** Check ready flag → `forwardFFT.performFrequencyOnlyForwardTransform(fftData.data())` → normalize magnitudes → `repaint()`
  3. **paint():** Map frequency bins to x-axis (log scale), magnitude to y-axis (dB scale). Log frequency: `skewedX = 1.0f - std::exp(std::log(proportion) * 0.2f)`
- **Thread safety:** Only set `bool` flag from audio thread; only read fftData from UI thread after flag check
- **Tutorial:** JUCE "The fast Fourier transform"

#### Level Meter
- **Implementation:** Peak or RMS detection with ballistics (attack/release)
- **Thread safety:** Use `std::atomic<float>` to pass levels from audio to UI thread
- **Community:** `ff_meters` (LED-style meters)

---

## 4. CPU Cost Guide

| Operation | Relative CPU | Notes |
|---|---|---|
| Gain scaling | Very low | 1 multiply per sample |
| Biquad filter | Very low | 5 multiply-adds per sample |
| TPT SVF filter | Low | ~8 operations per sample |
| SmoothedValue | Very low | 1 multiply-add per sample |
| Delay line read | Low | 1 read + interpolation |
| Envelope follower | Very low | 2 comparisons + multiply per sample |
| 8-line FDN reverb | Moderate | 8 delay reads + 8x8 matrix multiply per sample |
| Convolution (1024pt) | Moderate-High | FFT-based, partitioned |
| Oversampling 4x | High | 4x processing + up/down filter |
| FFT (2048pt) | Moderate | Per-block, not per-sample |
| Granular processing | Moderate | Depends on grain count and windowing |

---

## Sources
- docs.juce.com/master/group__juce__dsp.html
- github.com/Chowdhury-DSP/chowdsp_utils
- github.com/sudara/awesome-juce
- ccrma.stanford.edu/courses/320c/
- melatonin.dev/blog/
