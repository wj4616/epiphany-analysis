# Knowledge Base Architecture

## KB Complexity Decision

**Decision:** Structured multi-layer KB with bridge layer

**Rationale:**
1. Users describe goals in creative language ("warm", "psychedelic", "lush") that differs from implementation language (filter cutoff, resonance, delay feedback)
2. Domain has 5 distinct knowledge types warranting separate layers
3. Bridge layer required for Sound Design → Technical translation
4. Matches Phase 0 complexity profile (Knowledge: Bridged)

---

## Knowledge Layers

### Layer 1: Technical KB

| Attribute | Value |
|-----------|-------|
| **Name** | `technical-kb` |
| **Domain Boundary** | Implementation knowledge: C++17, JUCE framework, CMake, DSP algorithms, VST3 SDK, audio thread safety |
| **Authority Score** | 0.95 (highly authoritative — from official documentation and proven patterns) |
| **Separation Rule** | Technical content is implementation-focused; does not address creative decisions or business concerns |
| **Topics** | `cpp-audio`, `juce-patterns`, `dsp-algorithms`, `audio-thread-safety`, `cmake-build`, `vst3-sdk`, `parameter-management`, `state-persistence` |

**What Belongs Here:**
- JUCE API patterns and code examples
- C++ patterns specific to audio (real-time safety, RAII, move semantics)
- DSP algorithm implementations with math
- Audio thread safety checklists
- CMake configuration patterns
- Parameter management (APVTS) patterns

**What Doesn't Belong Here:**
- Sound design theory (→ Sound Design KB)
- UI visual design (→ UI/UX KB)
- Marketplace strategies (→ Commercial KB)
- Academic references (→ Reference KB)

---

### Layer 2: Sound Design KB

| Attribute | Value |
|-----------|-------|
| **Name** | `sound-design-kb` |
| **Domain Boundary** | Creative sound design knowledge: synthesis theory, patch methodology, sonic descriptors, genre techniques |
| **Authority Score** | 0.85 (authoritative — from textbooks and expert sources) |
| **Separation Rule** | Sound design content is creative/subjective; does not contain code implementations |
| **Topics** | `synthesis-theory`, `oscillator-design`, `filter-design`, `modulation`, `effects`, `psychedelic`, `ambient`, `pad`, `creative`, `psybient`, `presets` |

**What Belongs Here:**
- Synthesis theory (subtractive, FM, wavetable, granular, additive)
- Sound design methodology
- Sonic descriptors and their meaning
- Genre-specific techniques (psychedelic, ambient, psybient)
- Patch creation guides

**What Doesn't Belong Here:**
- DSP algorithm code (→ Technical KB)
- Knob/slider visual design (→ UI/UX KB)
- Plugin pricing (→ Commercial KB)

---

### Layer 3: UI/UX KB

| Attribute | Value |
|-----------|-------|
| **Name** | `ui-ux-kb` |
| **Domain Boundary** | Audio plugin interface design: visual standards, control layout, component design, LookAndFeel patterns |
| **Authority Score** | 0.80 (authoritative — from industry standards and successful plugins) |
| **Separation Rule** | UI content is visual/interaction-focused; does not contain DSP or sound design theory |
| **Topics** | `visual-standards`, `control-layout`, `knobs`, `sliders`, `meters`, `theming`, `lookandfeel`, `accessibility` |

**What Belongs Here:**
- Audio plugin visual standards
- Control layout theory (skeuomorphic vs flat)
- Knob/slider/meter design patterns
- LookAndFeel implementation patterns
- Theming and color systems
- Accessibility considerations

**What Doesn't Belong Here:**
- JUCE component API (→ Technical KB)
- Sound design presets (→ Sound Design KB)
- Plugin screenshots for marketing (→ Commercial KB)

---

### Layer 4: Commercial KB

| Attribute | Value |
|-----------|-------|
| **Name** | `commercial-kb` |
| **Domain Boundary** | Business and distribution knowledge: marketplaces, licensing, pricing, marketing, anti-piracy |
| **Authority Score** | 0.75 (moderately authoritative — from marketplace docs and industry practice) |
| **Separation Rule** | Commercial content is business-focused; does not contain technical implementation details |
| **Topics** | `marketplaces`, `licensing`, `pricing`, `marketing`, `anti-piracy`, `distribution`, `business-planning` |

**What Belongs Here:**
- Marketplace requirements (KVR, Plugin Boutique)
- Licensing systems (iLok, eLicenser, custom)
- Pricing strategies for indie developers
- Marketing tactics for solo developers
- Anti-piracy measures
- Business plan templates

**What Doesn't Belong Here:**
- Licensing integration code (→ Technical KB)
- Sound identity decisions (→ Sound Design KB)
- UI design (→ UI/UX KB)

---

### Layer 5: Reference KB

| Attribute | Value |
|-----------|-------|
| **Name** | `reference-kb` |
| **Domain Boundary** | Curated references to external authoritative sources: textbooks, academic papers, tutorials, official documentation |
| **Authority Score** | 1.0 (highest — pointers to authoritative sources) |
| **Separation Rule** | Reference content is citation/index only; does not contain original content |
| **Topics** | `textbooks`, `academic-papers`, `official-docs`, `tutorials`, `videos`, `community` |

**What Belongs Here:**
- Textbook citations (Pirkle, Zölzer, Smith)
- Academic paper index
- JUCE official documentation links
- Tutorial/video references
- Community resource links

**What Doesn't Belong Here:**
- Synthesized content from these sources (→ appropriate layer)
- Original commentary (→ appropriate layer)

---

## Layer Separation Matrix

| Layer | Technical | Sound Design | UI/UX | Commercial | Reference |
|-------|-----------|--------------|-------|------------|-----------|
| Technical | — | Bridge | — | — | Index |
| Sound Design | Bridge | — | — | — | Index |
| UI/UX | — | — | — | — | Index |
| Commercial | — | — | — | — | Index |
| Reference | Source | Source | Source | Source | — |

---

## Bridge Layer: Sound Design ↔ Technical

**Purpose:** Translate subjective/creative descriptors to DSP parameters

**Direction:** Sound Design KB → Technical KB

**Mechanism:** Bridge entries map sonic descriptors to parameter ranges

**Example:**
```
Descriptor: "warm"
Maps to:
  - filter_cutoff: [0.2, 0.4]
  - filter_resonance: [0.1, 0.2]
  - saturation: [0.1, 0.3]
Why: "Warmth comes from reduced high frequencies (lower cutoff), subtle resonance, and soft saturation"
```

---

## Cross-Reference Rules

1. **Queries rank by authority score** — Higher authority entries appear first
2. **Bridge entries lower confidence for combinations** — Multiple descriptors composed = lower confidence
3. **Reference KB entries link, don't duplicate** — Use as source pointers only
4. **Commercial KB independent** — Business decisions don't affect authority of other layers