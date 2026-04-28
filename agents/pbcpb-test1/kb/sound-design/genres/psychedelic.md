# Psychedelic Sound Design

---
id: SD-GENRE-001
category: genres
tags: [sound-design, psychedelic, psybient, ambient]
---

## Summary

Sound design techniques for psychedelic, psybient, and ambient music. Focus on evolving textures, spatial depth, and unusual timbres.

## Core Techniques

### 1. Granular Textures

- Use long grain sizes (200-800ms) for smooth textures
- High grain density for "smeared" sounds
- Freeze playhead on interesting positions
- Add 10% jitter/spray for organic feel

**DSP Parameters:**
- `grain_size_ms`: 200-800
- `grain_density`: high (overlapping)
- `jitter`: 0.05-0.15
- `freeze`: true for static textures

### 2. Asynchronous Modulation

Multiple LFOs at unrelated speeds:

```
LFO 1: 0.07 Hz (14-second cycle)
LFO 2: 0.13 Hz (7.7-second cycle)
LFO 3: 0.03 Hz (33-second cycle)
```

Creates non-repeating, evolving movement.

### 3. Harmonic Layering

Use saturation as harmonic generator:

| Type | Harmonics | Character |
|------|-----------|-----------|
| Soft clip | Even | Warm, tube-like |
| Hard clip | Odd | Bright, aggressive |
| Tape | Both | Complex, saturated |

### 4. Recursive Reverb

Reverbs that feed their own reflections:
- Creates infinite sustain
- Reveals hidden harmonics
- Excellent for drones

### 5. Worldizing

Recording sounds through real spaces:
1. Play sound through speakers
2. Record in bathroom, stairwell, forest
3. Mix with original for organic ambience

**Advanced:** Speed up 4x, record acoustic, slow down 4x for impossibly vast reverb.

### 6. No-Input Feedback (Experimental)

Self-oscillating feedback loops:
1. Route aux track back to itself
2. Use EQ to tune feedback frequencies
3. Always use limiter for safety

Creates chaotic, evolving drones.

## Genre Characteristics

### Psybient

| Element | Typical Value |
|---------|---------------|
| Tempo | 60-110 BPM |
| Rhythm | Downtempo, tribal |
| Texture | Deep, spatial |
| Melody | Ethereal, modal |
| FX | Long reverb, delay |

### Dark Ambient

| Element | Typical Value |
|---------|---------------|
| Texture | Drone-based |
| Timbre | Dark, unsettling |
| Movement | Very slow |
| FX | Distortion, reverb |

### Psychedelic Rock

| Element | Typical Value |
|---------|---------------|
| Guitar | Fuzz, wah, phaser |
| Keyboards | Organ, mellotron |
| FX | Tape delay, reverb |
| Rhythm | Unconventional |

## Sound Designer Workflow

1. **Start with character** - Define the vibe in words first
2. **Layer multiple sources** - Oscillators + samples + feedback
3. **Add asynchronous modulation** - Multiple slow LFOs
4. **Process heavily** - Reverb, delay, distortion
5. **Automate continuously** - Evolving parameters

## Key Artists

- **Shpongle** - Complex, layered, psychedelic
- **Ott** - Dub-influenced, lush
- **Carbon Based Lifeforms** - Deep, spacey
- **Solar Fields** - Cinematic, evolving
- **Entheogenic** - Organic, world-influenced

## Sources

- [Artists in DSP - Ambient Techniques](https://artistsindsp.com/ambient-sound-design-7-advanced-techniques-for-evolving-drones-and-textures/)
- Psybient.org interviews
- Genre analysis from research