# Sound Design Knowledge Base Research Plan

> **Goal:** Populate Sound Design KB with professional sound design theory from verified sources.

**Tiered Source Strategy:**
- Tier 1 (Open access): Harvest directly via firecrawl
- Tier 2 (Copyrighted): Summarize principles, cite sources
- Tier 3 (Reference only): Cite without reproduction
- Tier 4 (Create ourselves): Preset analysis and validation

---

## Phase 1: Tier 1 Open Access Sources

Harvest directly using firecrawl:

### music-dsp.org
- **URL:** http://www.musicdsp.org/
- **Content:** Community-sourced DSP algorithms and discussions
- **License:** Open
- **Harvest targets:**
  - Filter algorithms
  - Envelope implementations
  - LFO shapes and rates
  - Effect recipes

### JUCE Forum DSP Discussions
- **URL:** https://forum.juce.com/
- **Content:** Community discussions on JUCE DSP implementation
- **License:** Open
- **Harvest targets:**
  - Filter implementation discussions
  - Parameter smoothing techniques
  - Common DSP patterns

### OpenAudio Blog Posts
- **Targets:** Public domain DSP tutorials
- **Harvest targets:**
  - Synthesis techniques
  - Filter design fundamentals
  - Modulation theory

---

## Phase 2: Tier 2 Copyrighted Sources (Principles Only)

Summarize principles without reproducing content:

### Sound on Sound Magazine
- **URL:** https://www.soundonsound.com/
- **Content:** Professional audio magazine
- **License:** Copyrighted
- **Extract principles:**
  - Filter design fundamentals (what cutoff values create warmth)
  - Reverb parameters for different spaces
  - Compression attack/release relationships
  - Synthesis types and characteristics

**Citation format:** "Sound on Sound: Filter Design Fundamentals (2024)"

### Universal Audio Blog
- **URL:** https://www.uaudio.com/blog/
- **Content:** Professional audio tutorials
- **License:** Copyrighted
- **Extract principles:**
  - Analog modeling techniques
  - Tape saturation characteristics
  - Vintage synth characteristics

**Citation format:** "Universal Audio: Analog Synth Basics (2024)"

### Ableton Learning Music
- **URL:** https://learningmusic.ableton.com/
- **Content:** Interactive music theory
- **License:** Educational use
- **Extract principles:**
  - Synthesis fundamentals
  - Beat creation
  - Melody design

---

## Phase 3: Tier 3 Reference Only

Cite without reproduction:

### Books
- **Designing Sound** by Andy Farnell
- **The Audio Programming Book** by Boulanger & Lazzarini
- **DAFX: Digital Audio Effects** by Udo Zölzer

**Usage:** Reference principles, no reproduction

### Academic Papers
- **AES Journal** articles on DSP
- **ICMC proceedings** on sound synthesis

**Usage:** Cite for educational layer

---

## Phase 4: Create Our Own Content

### Preset Analysis
- Analyze factory presets from major synths (Serum, Massive, Vital)
- Document parameter patterns
- Create ground truth presets with human testing

### Validation Protocol
1. Create preset from Sound Design KB suggestion
2. Human tests in DAW
3. Compare expected vs actual sonic result
4. Log discrepancies
5. Adjust KB entries

### Ground Truth Creation
- Start with 10-15 core presets (pad, lead, bass, etc.)
- Verify with human listening
- Mark as `ground_truth` confidence
- Use for future translations

---

## Implementation Tasks

### Task R1: Harvest Tier 1 Sources

Use firecrawl skill to harvest open access content. Target: music-dsp.org filter algorithms. Store in validation-logs/harvested/ directory.

### Task R2: Extract Tier 2 Principles

Manual extraction from:
- Sound on Sound filter articles
- Synthesis tutorials
- Mixing guidelines

Document principles in Sound Design KB format.

### Task R3: Create Ground Truth Presets

Design and test:
1. Bright Pad
2. Warm Pad
3. Aggressive Lead
4. Deep Bass
5. Atmospheric Pad
6. Punchy Lead
7. Lush Pad
8. Sharp Lead
9. Sub Bass
10. Drone

Each preset:
- Define parameters
- Human test in DAW
- Document sonic result
- Mark confidence level

### Task R4: Document Validation Process

Create validation log format:
- Date
- Preset name
- Parameters used
- Expected sonic result
- Actual sonic result
- Discrepancies
- Adjustments made
- Outcome (approved/needs_revision)

---

## Success Criteria

- [ ] Tier 1 sources harvested (music-dsp, JUCE forum)
- [ ] Tier 2 principles extracted (Sound on Sound, UA blog)
- [ ] Tier 3 sources cited in educational layer
- [ ] Ground truth presets created (10-15 minimum)
- [ ] Validation process documented
- [ ] All entries cite sources
- [ ] Confidence levels assigned