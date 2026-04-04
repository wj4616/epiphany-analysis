# Phase 9: DAW Testing Checklist — PsycogVST

> Test in REAPER with real audio. Check each item, note any issues.
> Plugin: `~/.vst3/Psycog VST.vst3`

## Setup
- [ ] Open REAPER, create new project
- [ ] Add audio track with varied source material (vocals, drums, pads, guitar)
- [ ] Insert PsycogVST on track FX chain
- [ ] Plugin window opens without crash, shows cyberpunk dark purple theme

## 1. GUI Verification
- [ ] All 4 columns visible: TIME | WAVEFOLD | LFO | OUTPUT
- [ ] "PSYCOG" title top-left, "v1.0" top-right
- [ ] Signal flow strip: "INPUT >> STRETCH >> FOLD >> MIX >> OUTPUT"
- [ ] Preset bar shows "Init" with "1 / 20" index
- [ ] All 8 knobs respond to drag (gold arc tracks position)
- [ ] Value labels update in real-time (stretch shows "1.00x", mix shows "100%")
- [ ] Freeze buttons: OFF highlighted by default, clicking MANUAL/AUTO toggles cyan
- [ ] LFO waveform buttons: SIN highlighted by default
- [ ] LFO target buttons: all off by default, toggle gold when clicked

## 2. Bypass / Transparency
- [ ] Mix=0%: audio is identical to bypassed plugin (dry only)
- [ ] Mix=100%, foldAmount=0, stretch=1x: audio plays with ~46ms latency, no tonal change
- [ ] Plugin bypassed in REAPER: seamless, no click/pop

## 3. Time Stretch (Unfrozen)
- [ ] Stretch=1x: audio passes through with granular artifacts (mild)
- [ ] Stretch=2x: audio slows down, grains overlap, stretched texture
- [ ] Stretch=10x: extreme expansion, glacial drone-like output
- [ ] Stretch=0.1x: glitchy compression, audio fragments skip forward
- [ ] Moving stretch knob smoothly: no clicks, smooth transition

## 4. Freeze System
- [ ] **Manual mode:** Click MANUAL — audio freezes at current moment. Moving POSITION scrubs through 3-second frozen buffer. Click OFF to unfreeze — smooth crossfade back to live.
- [ ] **Auto mode:** Set threshold ~0.3, play loud audio. Freeze triggers on transients. ~100ms cooldown between triggers. Listen for cascading frozen layers.
- [ ] **Stretch during freeze:** While frozen, stretch controls pitch. 2x = octave up, 0.5x = octave down. 1x = normal pitch, looping.
- [ ] **Position during freeze:** Moving position knob scrubs through different parts of frozen buffer.
- [ ] **Mode transitions:** Switching between OFF/MANUAL/AUTO — no clicks, smooth 10ms crossfade.

## 5. Wavefolder
- [ ] foldAmount=0: passthrough (no harmonic change)
- [ ] foldAmount=50%: audible harmonic enrichment, "warm saturation"
- [ ] foldAmount=100%: heavy folding, bright harmonics, aggressive
- [ ] foldOffset=0: symmetric folding
- [ ] foldOffset>0: stereo width increases (L and R diverge) — check with stereo meter
- [ ] Mono input with offset>0: creates stereo from mono (verify with stereo scope)

## 6. LFO Modulation
- [ ] Enable LFO target "STR" + set depth=50%, rate=0.5Hz: stretch breathes audibly
- [ ] Enable LFO target "POS" while frozen: position drifts through buffer
- [ ] Enable LFO target "FLD": fold amount modulates, harmonics pulse
- [ ] Enable LFO target "OFS": stereo field modulates (widening/narrowing)
- [ ] **Waveform shapes:**
  - SIN: smooth modulation
  - TRI: linear ramps
  - SQ: hard steps between two states
  - S&H: random jumps at rate interval
- [ ] Multiple targets enabled: all modulate simultaneously
- [ ] Depth=0%: no modulation regardless of targets
- [ ] Rate at minimum (0.01Hz): one cycle per ~100 seconds — glacial drift
- [ ] Rate at maximum (20Hz): fast modulation, approaching audio rate

## 7. Presets
- [ ] Click through all 20 presets with ">" button
- [ ] Each preset produces distinctly different character
- [ ] Key presets to verify:
  - **Init (1/20):** Clean default, no effects
  - **Turning Through Time (2/20):** Granular shimmer with position drift
  - **Frozen Cathedral (16/20):** Manual freeze, switch to freeze and listen for reverberant pad
  - **Event Horizon (17/20):** Extreme 8x stretch drone
  - **Ghost Layer (18/20):** 50% mix, effect sits behind dry signal
  - **Omniscient Dawn (15/20):** Maximum chaos — S&H, all targets, auto-freeze
- [ ] Preset name and index update correctly in preset bar

## 8. State Save/Restore
- [ ] Set a non-default preset (e.g., "Golden Memories")
- [ ] Save REAPER project
- [ ] Close and reopen REAPER project
- [ ] PsycogVST loads with correct preset and all parameters restored

## 9. Edge Cases
- [ ] Empty track (silence): no output, no noise floor
- [ ] Very loud input (0dBFS+): output stays controlled (soft-clip protection)
- [ ] Change buffer size in REAPER (64, 256, 1024, 2048): no crash
- [ ] Change sample rate (44100, 48000, 96000): no crash, correct behavior
- [ ] Rapid preset switching: no crash or audio glitch
- [ ] Automate mix parameter: smooth transition, no zipper noise

## 10. CPU & Stability
- [ ] CPU usage shown in REAPER performance meter: < 15% at 48kHz/512 buffer
- [ ] Play for 5+ minutes: no memory leak, no gradual CPU increase
- [ ] Remove and re-add plugin: no crash

---

## Results

| Test | Pass/Fail | Notes |
|------|-----------|-------|
| GUI | | |
| Bypass | | |
| Time Stretch | | |
| Freeze | | |
| Wavefolder | | |
| LFO | | |
| Presets | | |
| State | | |
| Edge Cases | | |
| CPU/Stability | | |

**Overall:** ___  
**Date tested:** ___  
**REAPER version:** ___  
**Sample rate / buffer:** ___
