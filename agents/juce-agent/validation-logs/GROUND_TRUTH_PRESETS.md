# Ground Truth Preset Templates

> Preset templates verified through human DAW testing. Each preset documents expected sonic result and verified parameters.

## Purpose

Ground truth presets serve as validation benchmarks for the Sound Design KB. When a preset is marked as `ground_truth`, it means:
1. Parameters have been tested in a real DAW
2. Sonic result matches the expected description
3. Translation accuracy has been verified

## Preset Categories

### Pads (Atmospheric Sustained Sounds)

#### 1. Bright Pad - Minimal
```json
{
  "name": "Bright Pad - Minimal",
  "category": "pad",
  "capability_requirements": ["filter_lowpass"],
  "verified": true,
  "confidence": "ground_truth",
  "parameters": {
    "filter_cutoff": {"value": 0.75, "unit": "normalized", "verified": true},
    "filter_resonance": {"value": 0.15, "unit": "normalized", "verified": true}
  },
  "sonic_description": "Bright, open pad with subtle resonance. Cutoff moderately high, resonance subtle.",
  "expected_frequency_balance": "Bright, presence in 2-8kHz range",
  "expected_dynamic_range": "Even sustain, no pumping",
  "testing_notes": "Test with sustained chord. Should be bright but not harsh.",
  "source": "preset_analysis",
  "verified_by": "human_tested",
  "verification_date": "2026-03-29"
}
```

#### 2. Warm Pad - Classic
```json
{
  "name": "Warm Pad - Classic",
  "category": "pad",
  "capability_requirements": ["filter_lowpass", "chorus"],
  "verified": true,
  "confidence": "ground_truth",
  "parameters": {
    "filter_cutoff": {"value": 0.30, "unit": "normalized", "verified": true},
    "filter_resonance": {"value": 0.15, "unit": "normalized", "verified": true},
    "chorus_depth": {"value": 0.40, "unit": "normalized", "verified": true},
    "chorus_rate": {"value": 0.20, "unit": "normalized", "verified": true}
  },
  "sonic_description": "Warm, wide pad with gentle movement. Cutoff low-mid, chorus adds width.",
  "expected_frequency_balance": "Warm, emphasis in 200-800Hz range",
  "expected_stereo_image": "Wide from chorus, centered low end",
  "testing_notes": "Test with chord progression. Should feel warm and wide without being muddy.",
  "source": "preset_analysis",
  "verified_by": "human_tested",
  "verification_date": "2026-03-29"
}
```

#### 3. Atmospheric Pad
```json
{
  "name": "Atmospheric Pad",
  "category": "pad",
  "capability_requirements": ["filter_lowpass", "reverb", "lfo"],
  "verified": false,
  "confidence": "high",
  "parameters": {
    "filter_cutoff": {"value": 0.35, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.10, "unit": "normalized", "verified": false},
    "reverb_size": {"value": 0.70, "unit": "normalized", "verified": false},
    "reverb_mix": {"value": 0.40, "unit": "normalized", "verified": false},
    "lfo_rate": {"value": 0.15, "unit": "Hz_equiv", "verified": false},
    "lfo_depth": {"value": 0.20, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Evolving, spacious pad with subtle movement. Long reverb tail, slow LFO modulation.",
  "expected_frequency_balance": "Focused midrange, softened highs",
  "expected_movement": "Slow, evolving modulation",
  "testing_notes": "Test with sustained notes. Should create an evolving atmosphere.",
  "source": "sound_design_principle",
  "verification_status": "needs_testing"
}
```

#### 4. Lush Strings
```json
{
  "name": "Lush Strings",
  "category": "pad",
  "capability_requirements": ["filter_lowpass", "chorus", "detune"],
  "verified": false,
  "confidence": "medium",
  "parameters": {
    "filter_cutoff": {"value": 0.45, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.10, "unit": "normalized", "verified": false},
    "chorus_depth": {"value": 0.50, "unit": "normalized", "verified": false},
    "chorus_rate": {"value": 0.25, "unit": "normalized", "verified": false},
    "detune": {"value": 0.10, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Rich, wide string pad. Chorus and detune create thickness.",
  "expected_frequency_balance": "Even across spectrum",
  "expected_stereo_image": "Very wide, centered low end",
  "testing_notes": "Test with chord progression. Should sound thick and rich.",
  "source": "attack_magazine",
  "verification_status": "needs_testing"
}
```

### Leads (Melodic Sounds)

#### 5. Sharp Lead
```json
{
  "name": "Sharp Lead",
  "category": "lead",
  "capability_requirements": ["filter_highpass", "distortion"],
  "verified": false,
  "confidence": "medium",
  "parameters": {
    "filter_type": {"value": "highpass", "verified": false},
    "filter_cutoff": {"value": 0.70, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.20, "unit": "normalized", "verified": false},
    "drive": {"value": 0.50, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Cutting, bright lead. Highpass filter removes low mud, drive adds edge.",
  "expected_frequency_balance": "Bright, presence in 2-5kHz",
  "expected_transient": "Punchy attack, quick decay",
  "testing_notes": "Test with melody. Should cut through mix without being harsh.",
  "source": "sound_design_principle",
  "verification_status": "needs_testing"
}
```

#### 6. Punchy Lead
```json
{
  "name": "Punchy Lead",
  "category": "lead",
  "capability_requirements": ["filter_lowpass", "filter_envelope", "compression"],
  "verified": false,
  "confidence": "medium",
  "parameters": {
    "filter_cutoff": {"value": 0.60, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.25, "unit": "normalized", "verified": false},
    "filter_envelope_amount": {"value": 0.50, "unit": "normalized", "verified": false},
    "attack": {"value": 0.01, "unit": "seconds", "verified": false},
    "compression_ratio": {"value": 4.0, "unit": "ratio", "verified": false}
  },
  "sonic_description": "Punchy, percussive lead. Filter envelope creates snap, compression adds sustain.",
  "expected_transient": "Quick attack, filter snap",
  "expected_dynamic_range": "Controlled by compression",
  "testing_notes": "Test with staccato notes. Should have punchy attack.",
  "source": "sound_design_principle",
  "verification_status": "needs_testing"
}
```

### Bass (Low Frequency Sounds)

#### 7. Aggressive Bass - Drive
```json
{
  "name": "Aggressive Bass - Drive",
  "category": "bass",
  "capability_requirements": ["distortion", "filter_lowpass"],
  "verified": true,
  "confidence": "ground_truth",
  "parameters": {
    "filter_cutoff": {"value": 0.35, "unit": "normalized", "verified": true},
    "filter_resonance": {"value": 0.25, "unit": "normalized", "verified": true},
    "drive": {"value": 0.70, "unit": "normalized", "verified": true}
  },
  "sonic_description": "Punchy, distorted bass. Filter allows mid-low content through, drive adds harmonics.",
  "expected_frequency_balance": "Strong low end, harmonic content in mids",
  "expected_transient": "Punchy attack from filter + drive",
  "testing_notes": "Test with bass line. Should be aggressive but controlled.",
  "source": "preset_analysis",
  "verified_by": "human_tested",
  "verification_date": "2026-03-29"
}
```

#### 8. Deep Bass - Sub
```json
{
  "name": "Deep Bass - Sub",
  "category": "bass",
  "capability_requirements": ["filter_lowpass"],
  "verified": false,
  "confidence": "high",
  "parameters": {
    "filter_cutoff": {"value": 0.20, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.10, "unit": "normalized", "verified": false},
    "attack": {"value": 0.005, "unit": "seconds", "verified": false},
    "sustain": {"value": 0.90, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Deep, sustained sub bass. Low cutoff, minimal resonance, long sustain.",
  "expected_frequency_balance": "Sub bass focused, 30-100Hz",
  "expected_transient": "Quick attack, long sustain",
  "testing_notes": "Test on proper monitoring (sub or headphones). Should be felt more than heard.",
  "source": "preset_analysis",
  "verification_status": "needs_testing"
}
```

#### 9. Punchy Bass
```json
{
  "name": "Punchy Bass",
  "category": "bass",
  "capability_requirements": ["filter_lowpass", "filter_envelope"],
  "verified": false,
  "confidence": "high",
  "parameters": {
    "filter_cutoff": {"value": 0.40, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.15, "unit": "normalized", "verified": false},
    "filter_envelope_amount": {"value": 0.50, "unit": "normalized", "verified": false},
    "attack": {"value": 0.005, "unit": "seconds", "verified": false}
  },
  "sonic_description": "Punchy bass with filter snap. Envelope creates transient punch.",
  "expected_transient": "Quick attack, filter snap on transient",
  "expected_frequency_balance": "Controlled low end with punch in mids",
  "testing_notes": "Test with bass line. Should have clear transient attack.",
  "source": "preset_analysis",
  "verification_status": "needs_testing"
}
```

### Experimental / Sound Design

#### 10. Evolving Drone
```json
{
  "name": "Evolving Drone",
  "category": "experimental",
  "capability_requirements": ["reverb", "delay", "lfo", "filter_lowpass"],
  "verified": false,
  "confidence": "medium",
  "parameters": {
    "reverb_size": {"value": 0.80, "unit": "normalized", "verified": false},
    "reverb_mix": {"value": 0.50, "unit": "normalized", "verified": false},
    "delay_time": {"value": 0.40, "unit": "seconds", "verified": false},
    "delay_feedback": {"value": 0.60, "unit": "normalized", "verified": false},
    "filter_cutoff": {"value": 0.35, "unit": "normalized", "verified": false},
    "lfo_rate": {"value": 0.10, "unit": "Hz_equiv", "verified": false},
    "lfo_depth": {"value": 0.30, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Long-evolving drone texture. Reverb and delay create space, LFO modulates filter.",
  "expected_movement": "Slow, continuous evolution",
  "expected_spatial": "Wide, spacious, atmospheric",
  "testing_notes": "Test with sustained note. Should evolve over time without repetition.",
  "source": "psybient_research",
  "verification_status": "needs_testing"
}
```

#### 11. Glitch Texture
```json
{
  "name": "Glitch Texture",
  "category": "experimental",
  "capability_requirements": ["granular", "lfo", "filter"],
  "verified": false,
  "confidence": "low",
  "parameters": {
    "grain_size": {"value": 0.05, "unit": "normalized", "verified": false},
    "grain_density": {"value": 0.40, "unit": "normalized", "verified": false},
    "position_jitter": {"value": 0.50, "unit": "normalized", "verified": false},
    "filter_cutoff": {"value": 0.50, "unit": "normalized", "verified": false},
    "lfo_rate": {"value": 2.0, "unit": "Hz", "verified": false}
  },
  "sonic_description": "Glitchy, rhythmic texture. Small grains with high jitter create rhythmic artifacts.",
  "expected_texture": "Granular, glitchy, rhythmic",
  "testing_notes": "Test with various source material. Should create glitch artifacts.",
  "source": "fractalsounds_psytrance",
  "verification_status": "needs_testing"
}
```

#### 12. Psybient Texture
```json
{
  "name": "Psybient Texture",
  "category": "experimental",
  "capability_requirements": ["filter_lowpass", "reverb", "delay", "feedback"],
  "verified": false,
  "confidence": "medium",
  "parameters": {
    "filter_cutoff": {"value": 0.30, "unit": "normalized", "verified": false},
    "filter_resonance": {"value": 0.20, "unit": "normalized", "verified": false},
    "reverb_size": {"value": 0.90, "unit": "normalized", "verified": false},
    "delay_time": {"value": 0.33, "unit": "seconds", "verified": false},
    "delay_feedback": {"value": 0.75, "unit": "normalized", "verified": false}
  },
  "sonic_description": "Atmospheric, evolving psybient texture. Long reverb and delay create space.",
  "expected_spatial": "Very wide, deep space",
  "expected_movement": "Slow evolution from feedback trails",
  "testing_notes": "Test with sustained notes. Should create deep, atmospheric space.",
  "source": "psybient_research",
  "verification_status": "needs_testing"
}
```

---

## Verification Process

### Testing Protocol

1. **Load preset in DAW** (REAPER recommended)
2. **Play test sequence** (sustained chord, melody, bass line as appropriate)
3. **Compare sonic result** with expected description
4. **Document any discrepancies**
5. **Mark as verified** if matches expectation
6. **Mark for revision** if adjustments needed

### Updating Presets

When a preset is verified or adjusted:

```bash
# Update the preset in the playbook
# Update confidence score based on results
# Log the validation in validation-logs/<project>/
# Update global-patterns.json statistics
```

### Verification Status

- `needs_testing` - Not yet tested
- `verified` - Tested and matches expectation
- `needs_revision` - Tested but needs parameter adjustments
- `deprecated` - No longer recommended

---

## Using Ground Truth Presets

When suggesting presets:

1. **Prefer verified presets** over unverified ones
2. **Check confidence score** - higher is better
3. **Read testing_notes** for important context
4. **Document any deviations** from expected result

---

## Adding New Presets

To add a new ground truth preset:

1. Define parameters based on Sound Design KB translation
2. Test in DAW with human verification
3. Document expected and actual sonic result
4. Set confidence based on verification outcome
5. Add to this file and update playbook preset_templates