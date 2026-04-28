# Cross-Cutting Concerns

These concerns touch every phase of VST plugin development and must be woven throughout the playbook, not isolated in one phase.

## CCC-01: Audio Thread Safety

**Description:** All code on the audio thread (processBlock, parameter callbacks) must be real-time safe.

**Phases Applied:** 2, 4, 5, 6, 8, 9, 11

**Enforcement:**
- Code review checklist item in every DSP phase
- Explicit audit step before each gate
- FM-01 through FM-07 patterns referenced

**Concrete Checks:**
- [ ] No heap allocations (new, delete, vector::push_back)
- [ ] No system calls (file I/O, network)
- [ ] No mutexes or blocking operations
- [ ] No unbounded loops
- [ ] ScopedNoDenormals at processBlock start
- [ ] All buffers pre-allocated in prepareToPlay()

---

## CCC-02: Gain Staging

**Description:** Signal levels must be managed conservatively throughout the signal chain.

**Phases Applied:** 0, 2, 4, 6, 9

**Enforcement:**
- Audited in Phase 0 (before any code)
- Verified in Phase 4 (DSP implementation)
- Re-checked in Phase 9 (DAW testing)

**Concrete Checks:**
- [ ] Hot signals only where intentional (distortion)
- [ ] -6dB to -12dB headroom typical
- [ ] Output limiter on any feedback path
- [ ] No unintentional gain accumulation

---

## CCC-03: State Persistence

**Description:** Plugin state must survive DAW save/load cycles.

**Phases Applied:** 5, 6, 8, 9

**Enforcement:**
- State round-trip test required
- APVTS pattern enforced
- Gate condition: state test passes

**Concrete Checks:**
- [ ] getStateInformation implemented (not stub)
- [ ] setStateInformation implemented
- [ ] Save/close/reopen test passes
- [ ] All parameters restore correctly
- [ ] Presets round-trip

---

## CCC-04: Parameter Consistency

**Description:** Parameters must have consistent IDs, ranges, and behavior across plugin lifetime.

**Phases Applied:** 0, 5, 6, 8, 16

**Enforcement:**
- Parameter IDs locked before v1.0 release
- Ranges documented in spec
- SmoothedValue audit at Phase 6

**Concrete Checks:**
- [ ] Parameter IDs never change after release
- [ ] Ranges match spec
- [ ] Skew factors appropriate (0.3 for freq, 0.5 for decay)
- [ ] Every SmoothedValue has getNextValue() per-sample
- [ ] Parameter changes audible within 2 seconds

---

## CCC-05: Cross-Platform Compatibility

**Description:** Plugin must work across target platforms (Linux primary, macOS/Windows secondary).

**Phases Applied:** 2, 3, 7, 8

**Enforcement:**
- CMake configuration verified
- CI builds on multiple platforms
- Format-specific paths documented

**Concrete Checks:**
- [ ] VST3 builds on all target platforms
- [ ] AU builds on macOS (if supported)
- [ ] Plugin paths correct per platform
- [ ] No platform-specific code without guards

---

## CCC-06: Human Creative Authority

**Description:** All creative decisions (sound identity, aesthetics, signal flow) are human-only.

**Phases Applied:** 0, 4, 7, 9

**Enforcement:**
- AI must ask, never assume defaults
- User's exact words recorded
- Spec approval gate

**Concrete Checks:**
- [ ] Signal flow user-defined
- [ ] Sound identity words exact (not paraphrased)
- [ ] UI visual direction user-specified
- [ ] Preset names user-approved

---

## CCC-07: Commercial Viability

**Description:** Product must be commercially distributable and licensable.

**Phases Applied:** 13, 14, 15, 16 (NEW)

**Enforcement:**
- Licensing system integrated
- Marketplace requirements met
- Pricing strategy documented

**Concrete Checks:**
- [ ] Licensing system functional
- [ ] Trial/full differentiation works
- [ ] Marketplace listing prepared
- [ ] Product page complete
- [ ] Pricing documented

---

## CCC-08: Knowledge Base Sufficiency

**Description:** KB must have enough content for informed implementation at every phase.

**Phases Applied:** All

**Enforcement:**
- KB gaps flagged during execution
- Bridge entries for subjective→parameter translation
- Reference layer for deep dives

**Concrete Checks:**
- [ ] Sound Design KB has bridge entries
- [ ] Technical KB covers JUCE patterns
- [ ] UI KB covers audio plugin standards
- [ ] Commercial KB covers marketplaces/licensing
- [ ] Reference KB indexes authoritative sources