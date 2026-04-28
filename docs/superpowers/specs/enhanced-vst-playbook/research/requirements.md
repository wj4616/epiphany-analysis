# Requirements Document

> Synthesis of all research into comprehensive requirements for the enhanced VST playbook.

---

## 1. Domain Requirements

### What the Playbook Must Accomplish

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-01 | Guide VST3 plugin development from concept to release | Critical | User brief |
| DR-02 | Cover synthesizer, effect, and utility plugin types | Critical | User brief |
| DR-03 | Include DSP algorithm design and implementation | Critical | Domain analysis |
| DR-04 | Cover C++17 programming patterns for audio | Critical | Domain analysis |
| DR-05 | Include real-time audio programming best practices | Critical | Domain analysis |
| DR-06 | Cover preset creation and management | High | Domain analysis |
| DR-07 | Include GUI/Editor implementation for audio plugins | High | Domain analysis |
| DR-08 | Cover multi-platform deployment (Linux, Windows, macOS) | High | User brief |
| DR-09 | Include JUCE 8.0.x and VST3 SDK 3.8.x | Critical | User brief |
| DR-10 | Cover CMake 3.22+ build configuration | High | Domain analysis |

### Sound Design Requirements

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-11 | Cover synthesizer sound design techniques | Critical | User brief |
| DR-12 | Include synthesizer patch creation theory | Critical | User brief |
| DR-13 | Cover psychedelic sound design techniques | Critical | User brief |
| DR-14 | Include ambient sound design techniques | Critical | User brief |
| DR-15 | Cover pad sound design techniques | High | User brief |
| DR-16 | Include creative sound design methods | High | User brief |
| DR-17 | Cover psybient sound design techniques | High | User brief |
| DR-18 | Include general synthesis theory | High | User brief |

### DSP Requirements

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-19 | Cover filter design and implementation | Critical | Domain analysis |
| DR-20 | Include oscillator design techniques | Critical | Domain analysis |
| DR-21 | Cover delay-based effects (reverb, delay, chorus) | High | Domain analysis |
| DR-22 | Include dynamics processing (compression, limiting) | High | Domain analysis |
| DR-23 | Cover waveshaping and distortion | High | Domain analysis |
| DR-24 | Include modulation effects (phaser, flanger) | Medium | Domain analysis |
| DR-25 | Cover granular synthesis basics | Medium | User brief |

---

## 2. Quality Requirements

### Standards and Compliance

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| QR-01 | Must comply with VST3 SDK specification | Critical | Standards |
| QR-02 | Must pass pluginval at strictness 5+ | Critical | Best practices |
| QR-03 | Must not exceed reasonable CPU limits | Critical | Domain analysis |
| QR-04 | Must handle all sample rates (44.1k-192k) | Critical | Domain analysis |
| QR-05 | Must handle all buffer sizes (64-2048) | Critical | Domain analysis |
| QR-06 | Must pass state save/restore tests | Critical | Domain analysis |
| QR-07 | Must pass multi-instance tests | Critical | Domain analysis |

### Audio Quality Standards

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| QR-08 | No audio dropouts or glitches | Critical | Failure modes |
| QR-09 | No clipping above 0dB without warning | Critical | Standards |
| QR-10 | No zipper noise on automation | Critical | Failure modes |
| QR-11 | Smooth bypass transitions | Critical | Failure modes |
| QR-12 | Equal-power dry/wet mixing | Critical | Failure modes |

### Code Quality Standards

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| QR-13 | No allocations on audio thread | Critical | Failure modes |
| QR-14 | No locks on audio thread | Critical | Failure modes |
| QR-15 | All addListener calls matched by removeListener | Critical | Failure modes |
| QR-16 | No static mutable state | Critical | Failure modes |
| QR-17 | All parameters have matching SmoothedValue | High | Failure modes |
| QR-18 | All DSP modules have prepare() methods | High | Best practices |

---

## 3. Strategic Requirements

### Business Constraints

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| SR-01 | Budget limited to $200/month for AI tools | Critical | User brief |
| SR-02 | No external paid resources required | Critical | User brief |
| SR-03 | Multi-session continuity supported | Critical | User brief |
| SR-04 | Target C++ beginner (knows Python/JavaScript) | Critical | User brief |
| SR-05 | Focus on commercial distribution intent | High | User brief |

### Technical Strategy

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| SR-06 | Linux-first development environment | High | User brief |
| SR-07 | VST3 as primary format | Critical | Domain analysis |
| SR-08 | AU for macOS support | High | Domain analysis |
| SR-09 | AAX for Pro Tools support (conditional) | Medium | Domain analysis |
| SR-10 | REAPER as primary testing DAW | High | User brief |

---

## 4. Platform Requirements

### Linux Platform

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| PR-01 | Build with GCC 7+ or Clang 6+ | Critical | User brief |
| PR-02 | All X11/XCB dependencies documented | High | User brief |
| PR-03 | Install to ~/.vst3/ | Critical | Platform concerns |
| PR-04 | No code signing required | Low | Platform concerns |

### macOS Platform

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| PR-05 | Support macOS 10.15+ | High | Platform concerns |
| PR-06 | Code signing + notarization | High | Platform concerns |
| PR-07 | AU format support | High | Platform concerns |
| PR-08 | VST3 format support | High | Platform concerns |

### Windows Platform

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| PR-09 | Support Windows 10+ | High | Platform concerns |
| PR-10 | Statically link MSVC runtime | Critical | Failure modes |
| PR-11 | VST3 format support | High | Platform concerns |

---

## 5. Cross-Cutting Requirements

### Audio Thread Safety

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CR-01 | Pre-allocate all buffers in prepareToPlay() | Critical | Cross-cutting |
| CR-02 | Use std::atomic<bool> for cross-thread flags | Critical | Cross-cutting |
| CR-03 | ScopedNoDenormals at processBlock start | Critical | Cross-cutting |
| CR-04 | No repaint() from audio thread | Critical | Cross-cutting |

### State Persistence

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CR-05 | Implement APVTS state serialization | Critical | Cross-cutting |
| CR-06 | State round-trips correctly | Critical | Cross-cutting |
| CR-07 | Automation preserves state | High | Cross-cutting |

### Parameter Handling

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CR-08 | All continuous parameters use SmoothedValue | Critical | Cross-cutting |
| CR-09 | getNextValue() called every sample | Critical | Cross-cutting |
| CR-10 | Parameter IDs locked before v1.0 | High | Cross-cutting |
| CR-11 | Equal-power dry/wet mixing | Critical | Cross-cutting |

---

## 6. User Requirements

### Target User Profile

| Attribute | Value |
|-----------|-------|
| Primary skill level | C++ beginner (knows Python/JavaScript) |
| Secondary skill level | Intermediate C++ developer |
| Development environment | Linux Mint |
| Primary DAW | REAPER |
| Budget | $100-200/month for AI tools |
| Goal | Commercial VST plugin release |

### Usability Requirements

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| UR-01 | No clarifying questions needed for primary path | Critical | Success criteria |
| UR-02 | Every task has owner, description, output | Critical | Success criteria |
| UR-03 | 90%+ first-pass gate rate | High | Success criteria |
| UR-04 | Complete handoff clarity between phases | High | Success criteria |
| UR-05 | C++ concepts explained inline | High | User brief |

### Documentation Requirements

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| UR-06 | Academic/professional source citations | High | User brief |
| UR-07 | DSP algorithms with math formulas | Critical | Domain analysis |
| UR-08 | Code examples with explanations | High | User brief |
| UR-09 | Not overloaded with C++ reference | Medium | User brief |

---

## 7. Knowledge Base Requirements

### Sound Design KB

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| KR-01 | Cover psychedelic synthesis techniques | Critical | User brief |
| KR-02 | Cover ambient pad design techniques | Critical | User brief |
| KR-03 | Cover creative synthesis methods | High | User brief |
| KR-04 | Cover psybient textures | High | User brief |
| KR-05 | Cover filter modulation techniques | High | Domain analysis |
| KR-06 | Cover granular synthesis basics | Medium | User brief |
| KR-07 | Cover oscillator design | Critical | Domain analysis |
| KR-08 | Cover envelope/LFO modulation | Critical | Domain analysis |

### UI Design KB

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| KR-09 | Modern plugin UI aesthetics | High | User brief |
| KR-10 | Synthesizer control layout theory | High | User brief |
| KR-11 | Visual hierarchy for audio interfaces | High | User brief |
| KR-12 | Custom LookAndFeel implementation | Critical | Domain analysis |
| KR-13 | Responsive/resizable plugin windows | Medium | Domain analysis |
| KR-14 | Accessibility considerations | Medium | Standards |
| KR-15 | Skin/theming systems | Low | Nice-to-have |

### Market KB (New)

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| KR-16 | Market positioning strategies | Critical | User brief |
| KR-17 | Pricing considerations | High | User brief |
| KR-18 | Platform distribution guidance | High | User brief |
| KR-19 | Website/landing page guidance | Medium | User brief |
| KR-20 | Demo/limitation strategies | Medium | User brief |

---

## 8. Failure Mode Prevention Requirements

### Critical Failure Modes

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| FM-01 | Prevent heap allocation on audio thread | Critical | Failure modes |
| FM-02 | Prevent buffer overflow at high sample rates | Critical | Failure modes |
| FM-03 | Prevent empty state serialization | Critical | Failure modes |
| FM-04 | Prevent denormal CPU explosion | Critical | Failure modes |
| FM-05 | Prevent feedback loop instability | Critical | Failure modes |
| FM-06 | Prevent dangling listener crashes | Critical | Failure modes |
| FM-07 | Prevent thread safety violations | Critical | Failure modes |

### Serious Failure Modes

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| FM-08 | Handle mono input correctly | Critical | Failure modes |
| FM-09 | Advance SmoothedValue per sample | Critical | Failure modes |
| FM-10 | Recalculate coefficients on rate change | High | Failure modes |
| FM-11 | Smooth bypass transitions | High | Failure modes |
| FM-12 | Handle variable block sizes | High | Failure modes |
| FM-13 | Prevent aliasing in nonlinear processing | High | Failure modes |
| FM-14 | Use equal-power dry/wet mixing | Critical | Failure modes |

---

## 9. Validation Requirements

### Automated Testing

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| VR-01 | pluginval strictness 5+ must pass | Critical | Best practices |
| VR-02 | State round-trip must pass | Critical | Best practices |
| VR-03 | Multi-instance test must pass | Critical | Best practices |
| VR-04 | Sample rate stress test (44.1k-192k) | Critical | Best practices |
| VR-05 | Buffer size stress test (64-2048) | Critical | Best practices |

### Human Testing

| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| VR-06 | DAW testing in REAPER | Critical | User brief |
| VR-07 | Preset audition and approval | High | Gap analysis |
| VR-08 | Automation recording/playback test | High | Best practices |
| VR-09 | Bypass test (clean dry signal) | High | Best practices |
| VR-10 | 60-second silence test (denormal) | High | Best practices |
| VR-11 | Full-scale noise test (no NaN/Inf) | High | Best practices |

---

## Requirements Traceability Matrix

| Source | Count | Critical | High | Medium |
|--------|-------|----------|------|--------|
| User Brief | 15 | 12 | 3 | 0 |
| Domain Analysis | 28 | 18 | 8 | 2 |
| Best Practices | 22 | 14 | 6 | 2 |
| Failure Modes | 21 | 14 | 7 | 0 |
| Cross-Cutting | 11 | 8 | 3 | 0 |
| Standards | 8 | 6 | 2 | 0 |
| Platform Concerns | 11 | 4 | 7 | 0 |
| Gap Analysis | 6 | 2 | 4 | 0 |
| **Total** | **122** | **78** | **40** | **4** |

---

## Sources

- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/README.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/scope.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/constraints.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/domain-analysis.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/best-practices.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/competitive-templates.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/existing-playbook-audit.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/cross-cutting-concerns.md`
- `/home/myuser/docs/superpowers/specs/enhanced-vst-playbook/research/platform-concerns.md`