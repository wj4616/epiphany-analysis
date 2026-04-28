# Scope Boundaries

## IN SCOPE

### Core Plugin Development
- VST3 audio plugin development using JUCE framework
- Audio effect plugins (filters, delays, reverb, distortion, dynamics)
- Synthesizer plugins (subtractive, FM, wavetable, granular)
- Utility plugins (meters, analyzers, MIDI tools)
- Hybrid plugin types

### Technical Implementation
- DSP algorithm design and implementation
- C++17 programming patterns for audio
- CMake build configuration for JUCE projects
- Real-time audio programming best practices
- Audio thread safety (no allocations, no locks on audio thread)
- Parameter smoothing and automation
- State management (preset save/load)

### Sound Design Knowledge
- **Synthesizer Sound Design**
  - Oscillator design and waveshapes
  - Filter design (lowpass, highpass, bandpass, notch)
  - Envelope generators (ADSR, ADSR+)
  - LFO modulation design
  - Voice architecture and polyphony

- **Psychedelic Sound Design** (core focus)
  - Frequency modulation techniques
  - Phase modulation and distortion
  - Granular synthesis for ambient textures
  - Spatial processing for psybient
  - Filter modulation and resonance abuse

- **Ambient Sound Design**
  - Pad synthesis techniques
  - Evolving textures
  - Spatial reverb design
  - Drift and animation techniques

- **Creative Sound Design**
  - Unusual signal routing
  - Feedback networks
  - Algorithmic/ generative elements
  - Hybrid acoustic-electronic techniques

### User Interface Design
- Modern plugin UI aesthetics
- Synthesizer control layout theory
- Visual hierarchy for audio interfaces
- Custom LookAndFeel implementation in JUCE
- Responsive/resizable plugin windows
- Accessibility considerations
- Skin/theming systems

### Commercial Integration
- Market positioning strategies
- Pricing considerations
- Platform distribution (Windows, macOS, Linux)
- Format support (VST3, AU, AAX, CLAP)
- Website/landing page guidance
- Demo/limitation strategies
- Update and versioning strategies

### Quality Sources
- Academic DSP papers and textbooks
- Professional audio engineering references
- Expert synthesizer design documentation
- Industry UI/UX standards for audio software
- Market research on VST plugin industry

---

## OUT OF SCOPE

### Not Covered
- **Mobile app development** (iOS/Android audio apps)
- **Hardware synthesizer development** (embedded systems, Eurorack)
- **Game audio middleware** (Wwise, FMOD integration)
- **Web-based audio** (Web Audio API, web plugins)
- **Streaming/broadcast solutions**
- **Music production workflow** (arrangement, mixing, mastering)
- **Sound library/sample pack creation** (preset packs without plugin)
- **iOS/macOS-only deployment** (must support cross-platform)
- **Enterprise/team workflows** (CI/CD pipelines, team management)
- **Free/open-source-only releases** (commercial focus required)
- **Audiobus/IAA integration** (iOS-specific)
- **Physical modeling synthesis** (beyond basic implementation)

### Platform Exclusions
- iOS deployment (iOS-specific APIs, App Store)
- Android deployment
- Web browser plugins
- Hardware DSP platforms (SHARC, etc.)

---

## ADJACENT

### Connects To But Doesn't Own

| Adjacent Area | Connection Point | Ownership |
|---------------|------------------|-----------|
| DAW-specific features | Plugin communicates with host via standard APIs | DAW developers |
| Audio interface configuration | Plugin receives/outputs audio | System/Driver |
| Sample rate conversion | Handled by JUCE/host | JUCE framework |
| Plugin hosting | Plugin is hosted, not hosting | DAW developers |
| Operating system audio | CoreAudio, ASIO, PulseAudio | OS developers |
| Preset sharing platforms | Plugin exports presets | Third-party services |
| Community forums | User support, feedback | Community management |
| Legal/compliance | License terms, open source compliance | Legal counsel |

### Dependencies
- JUCE framework development (upstream)
- VST3 SDK development (upstream)
- Steinberg plugin standards (upstream)
- DAW plugin support (platform)

### Outputs That Feed Others
- Plugin binary → Distribution platforms
- Preset files → User libraries
- Documentation → User support

---

## Primary vs Variant Paths

This playbook defines the **primary path** for:

**Primary Path**: Desktop VST3 plugin development for Linux/Windows/macOS, targeting DAW integration, with commercial distribution intent.

**Variant Paths** (documented as conditionals, not main flow):
- AU format for macOS (conditional: macOS-specific tasks)
- AAX format for Pro Tools (conditional: AAX-specific tasks)
- CLAP format (conditional: emerging format)
- Linux-only releases (conditional: reduced platform scope)

---

## Scope Creep Prevention

The following will NOT be added to scope without explicit Phase 0 revision:
- Mobile platforms
- Hardware development
- Web deployment
- Team/enterprise workflows
- Free-only release strategies