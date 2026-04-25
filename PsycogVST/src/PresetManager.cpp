/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Factory presets implementation

  20 presets tuned for psybient / psychedelic ambient pad processing:
  - Drones (5, 16): extreme stretch, minimal fold, immersive 95-100% mix
  - Evolving textures (1, 2, 7, 10): multi-target LFO, deeper modulation
  - Freeze explorers (3, 6, 12, 17): auto-freeze with position scanning
  - Harmonic shapers (4, 9, 13): fold 0.25-0.35, stereo offset movement
  - Glitch/pulse (8, 11, 18, 19): square/S&H LFO, bold state changes
  - Spatial (14, 15): manual/auto freeze, all-target evolution
*/

#include "PresetManager.h"
#include "Parameters.h"
#include "Version.h"

namespace Presets
{
    struct PresetParams
    {
        float stretch;
        float position;
        int freezeMode;
        float threshold;
        float foldAmount;
        float foldOffset;
        float lfoRate;
        int lfoWaveform;
        float lfoDepth;
        bool lfoTargetStretch;
        bool lfoTargetPosition;
        bool lfoTargetFoldAmount;
        bool lfoTargetFoldOffset;
        float mix;
    };

    // clang-format off
    // Presets for psybient / psychedelic ambient pad processing.
    // Fold 0.15-0.35 is the sweet spot: audible harmonic character without harshness.
    // Higher mix, deeper LFO, more multi-target for evolving organic textures.
    constexpr PresetParams factoryPresets[numPresets] = {
        // =====================================================================
        // INDEX 0: Init
        // Clean starting point — no effects, just passthrough
        // =====================================================================
        {
            ParamDefaults::stretch,    // 0.5 normalized = 1.0x
            ParamDefaults::position,   // 0.5
            ParamDefaults::freezeMode, // Off
            ParamDefaults::threshold,  // 0.3
            ParamDefaults::foldAmount, // 0.0 — no folding
            ParamDefaults::foldOffset, // 0.0
            ParamDefaults::lfoRate,    // 0.5 normalized
            ParamDefaults::lfoWaveform,// Sine
            ParamDefaults::lfoDepth,   // 0.0 — no modulation
            ParamDefaults::lfoTargetStretch,
            ParamDefaults::lfoTargetPosition,
            ParamDefaults::lfoTargetFoldAmount,
            ParamDefaults::lfoTargetFoldOffset,
            ParamDefaults::mix         // 1.0
        },

        // =====================================================================
        // INDEX 1: Turning Through Time
        // Slow granular drift — pad grains wander through position and
        // gently fold, creating shimmering evolving texture
        // =====================================================================
        {
            0.65f,   // stretch: ~2x — noticeable expansion
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.18f,   // foldAmount: warm harmonic saturation
            0.2f,    // foldOffset: stereo width from fold
            0.2f,    // lfoRate: ~0.04 Hz — very slow drift
            0,       // lfoWaveform: Sine
            0.45f,   // lfoDepth: deep modulation
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — grains drift through time
            true,    // lfoTargetFoldAmount — harmonics breathe
            false,   // lfoTargetFoldOffset
            0.85f    // mix: 85% — effect-forward
        },

        // =====================================================================
        // INDEX 2: Golden Memories
        // Stretched pad cloud with pitch drift — shimmering, nostalgic,
        // like a memory dissolving into golden particles
        // =====================================================================
        {
            0.72f,   // stretch: ~2.8x — deep expansion
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.15f,   // foldAmount: gentle harmonic glow
            0.25f,   // foldOffset: wide stereo separation
            0.15f,   // lfoRate: ~0.02 Hz — glacial evolution
            1,       // lfoWaveform: Triangle — smooth ramps
            0.4f,    // lfoDepth: stretch drifts through octave range
            true,    // lfoTargetStretch — pitch rises and falls like tides
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo field breathes
            0.9f     // mix: 90%
        },

        // =====================================================================
        // INDEX 3: Omnipotent Observers
        // Auto-freeze captures pad fragments, S&H position explores them
        // randomly — alien intelligence scanning your sound
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x — expanded fragments
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.35f,   // threshold: triggers readily for more freeze activity
            0.2f,    // foldAmount: alien harmonic texture
            0.15f,   // foldOffset: stereo depth
            0.35f,   // lfoRate: ~0.13 Hz — deliberate random rhythm
            3,       // lfoWaveform: S&H — random steps through frozen content
            0.5f,    // lfoDepth: bold position jumps
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — scanning through captured moments
            true,    // lfoTargetFoldAmount — harmonic character shifts per step
            false,   // lfoTargetFoldOffset
            0.8f     // mix: 80%
        },

        // =====================================================================
        // INDEX 4: Infinite Cogs
        // Mechanical psychedelic movement — triangle LFO weaves stereo
        // offset while fold adds grinding harmonic edge
        // =====================================================================
        {
            0.58f,   // stretch: ~1.5x — slightly expanded
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.25f,   // foldAmount: crunchy mechanical texture
            0.0f,    // foldOffset: centered — LFO sweeps it
            0.28f,   // lfoRate: ~0.06 Hz — slow mechanical pulse
            1,       // lfoWaveform: Triangle — smooth sweeps
            0.5f,    // lfoDepth: deep stereo weaving
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount — fold intensity sweeps
            true,    // lfoTargetFoldOffset — stereo field rotates
            0.8f     // mix: 80%
        },

        // =====================================================================
        // INDEX 5: Primordial Gear
        // Deep granular drone — extreme stretch turns pad into vast
        // slowly breathing drone landscape
        // =====================================================================
        {
            0.8f,    // stretch: ~4x — deep time dilation
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.1f,    // foldAmount: subtle warmth on the drone
            0.0f,    // foldOffset: none
            0.12f,   // lfoRate: ~0.015 Hz — one breath per minute
            0,       // lfoWaveform: Sine
            0.35f,   // lfoDepth: stretch breathes 2.5x-6x
            true,    // lfoTargetStretch — drone breathes slowly
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.95f    // mix: 95% — fully immersive drone
        },

        // =====================================================================
        // INDEX 6: Observer's Gaze
        // Auto-freeze + S&H random jumps through position AND stretch
        // — unpredictable alien texture collage
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x base
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.4f,    // threshold: triggers on moderate levels
            0.15f,   // foldAmount: each fragment gets harmonic color
            0.2f,    // foldOffset: stereo fragments
            0.42f,   // lfoRate: ~0.24 Hz — faster random rhythm
            3,       // lfoWaveform: S&H — random jumps
            0.45f,   // lfoDepth: bold jumps in position + pitch
            true,    // lfoTargetStretch — random pitch shifts per step
            true,    // lfoTargetPosition — random position per step
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.75f    // mix: 75%
        },

        // =====================================================================
        // INDEX 7: Golden Helix
        // Spiraling pitch with harmonic shimmer — pad corkscrews
        // through octaves while fold amount ebbs and flows
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x — base slightly expanded
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.2f,    // foldAmount: warm harmonic character
            0.15f,   // foldOffset: stereo width
            0.18f,   // lfoRate: ~0.03 Hz — slow spiral
            0,       // lfoWaveform: Sine — smooth
            0.5f,    // lfoDepth: wide pitch range + fold breathing
            true,    // lfoTargetStretch — pitch spiral up and down
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount — harmonics intensify at extremes
            false,   // lfoTargetFoldOffset
            0.85f    // mix: 85%
        },

        // =====================================================================
        // INDEX 8: Infinite Recursion
        // Square LFO hard-switches between two realities — pad jumps
        // between pitch states with fold offset flipping stereo image
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x — center point
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.2f,    // foldAmount: noticeable fold on both states
            0.0f,    // foldOffset: centered — square LFO flips it
            0.303f,  // lfoRate: ~0.1 Hz — deliberate slow alternation
            2,       // lfoWaveform: Square — hard-cut between states
            0.4f,    // lfoDepth: dramatic state changes
            true,    // lfoTargetStretch — pitch jumps between two states
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo image flips with state
            0.85f    // mix: 85%
        },

        // =====================================================================
        // INDEX 9: Weaver's Dance
        // Deep stereo weaving — fold offset LFO creates immersive
        // rotating stereo field, stretched for width
        // =====================================================================
        {
            0.65f,   // stretch: ~2x — expanded for space
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.3f,    // foldAmount: strong fold to make offset very audible
            0.0f,    // foldOffset: centered — LFO sweeps it fully
            0.22f,   // lfoRate: ~0.04 Hz — slow immersive weave
            1,       // lfoWaveform: Triangle — smooth sweeps
            0.55f,   // lfoDepth: full stereo rotation
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo field rotates
            0.9f     // mix: 90% — immersive
        },

        // =====================================================================
        // INDEX 10: Cog Within Cog
        // Multi-dimensional drift — position + stretch + fold all modulated
        // by slow sine, creating endlessly evolving organic texture
        // =====================================================================
        {
            0.65f,   // stretch: ~2x
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.22f,   // foldAmount: present harmonic character
            0.1f,    // foldOffset: base stereo offset
            0.15f,   // lfoRate: ~0.02 Hz — glacial evolution
            0,       // lfoWaveform: Sine
            0.45f,   // lfoDepth: everything moves deeply
            true,    // lfoTargetStretch — pitch drifts
            true,    // lfoTargetPosition — time drifts
            true,    // lfoTargetFoldAmount — harmonics drift
            true,    // lfoTargetFoldOffset — stereo drifts
            0.85f    // mix: 85%
        },

        // =====================================================================
        // INDEX 11: Quantum Collapse
        // S&H on stretch + fold — random pitch AND harmonic states,
        // auto-freeze captures then mangles fragments
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x base
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.4f,    // threshold: active triggering
            0.2f,    // foldAmount: each random state has fold character
            0.15f,   // foldOffset: stereo per fragment
            0.4f,    // lfoRate: ~0.2 Hz — moderate random rate
            3,       // lfoWaveform: S&H — random states
            0.4f,    // lfoDepth: bold random jumps
            true,    // lfoTargetStretch — random pitch per step
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount — random harmonic intensity per step
            false,   // lfoTargetFoldOffset
            0.8f     // mix: 80%
        },

        // =====================================================================
        // INDEX 12: Eternal Return
        // Auto-freeze + deep triangle sweep through frozen content —
        // pad moments captured and slowly scanned, ever-returning
        // =====================================================================
        {
            0.68f,   // stretch: ~2.2x — expanded frozen grains
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.4f,    // threshold: active capture
            0.18f,   // foldAmount: warm frozen texture
            0.2f,    // foldOffset: stereo depth in frozen content
            0.12f,   // lfoRate: ~0.015 Hz — glacial sweep
            1,       // lfoWaveform: Triangle — smooth position scanning
            0.6f,    // lfoDepth: wide sweep through entire frozen buffer
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — scans through frozen moment
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo shifts during scan
            0.85f    // mix: 85%
        },

        // =====================================================================
        // INDEX 13: Mirror of Mirrors
        // Harmonic hall of mirrors — fold amount pulsing with sine LFO
        // creates breathing distortion layers over stretched pad
        // =====================================================================
        {
            0.68f,   // stretch: ~2.2x
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.35f,   // foldAmount: bold wavefold — the main attraction
            0.25f,   // foldOffset: wide stereo from fold
            0.2f,    // lfoRate: ~0.04 Hz — slow breathing
            0,       // lfoWaveform: Sine
            0.45f,   // lfoDepth: fold breathes from subtle to intense
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount — harmonic intensity breathes
            true,    // lfoTargetFoldOffset — stereo field pulses
            0.8f     // mix: 80%
        },

        // =====================================================================
        // INDEX 14: Omniscient Dawn
        // Everything drifts — all four targets modulated, auto-freeze
        // captures moments, maximum organic evolution
        // =====================================================================
        {
            0.65f,   // stretch: ~2x
            0.5f,    // position: center
            2,       // freezeMode: Auto — captures moments
            0.35f,   // threshold: active triggering
            0.2f,    // foldAmount: present harmonic bed
            0.15f,   // foldOffset: stereo base
            0.1f,    // lfoRate: ~0.012 Hz — ultra-slow, one cycle ~80 seconds
            0,       // lfoWaveform: Sine
            0.5f,    // lfoDepth: deep, everything moves
            true,    // lfoTargetStretch — pitch evolves
            true,    // lfoTargetPosition — time evolves
            true,    // lfoTargetFoldAmount — harmonics evolve
            true,    // lfoTargetFoldOffset — stereo evolves
            0.9f     // mix: 90% — deep immersion
        },

        // =====================================================================
        // INDEX 15: Frozen Cathedral
        // MANUAL freeze — freeze any moment, it becomes a vast reverberant
        // space. Position LFO slowly explores the frozen architecture.
        // =====================================================================
        {
            0.7f,    // stretch: ~2.5x — frozen content pitched slightly
            0.5f,    // position: center — LFO explores from here
            1,       // freezeMode: Manual — user freezes the moment
            0.3f,    // threshold: unused in Manual
            0.12f,   // foldAmount: subtle warmth on frozen texture
            0.18f,   // foldOffset: stereo cathedral width
            0.1f,    // lfoRate: ~0.012 Hz — ultra-slow cathedral drift
            0,       // lfoWaveform: Sine
            0.55f,   // lfoDepth: wide exploration of frozen space
            true,    // lfoTargetStretch — pitch slowly rises/falls
            true,    // lfoTargetPosition — drifts through frozen moment
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.95f    // mix: 95% — fully inside the cathedral
        },

        // =====================================================================
        // INDEX 16: Event Horizon
        // Extreme stretch drone — pad becomes glacial, endless,
        // barely recognizable. Pure granular enormity.
        // =====================================================================
        {
            0.88f,   // stretch: ~7.5x — extreme time dilation
            0.5f,    // position: center
            0,       // freezeMode: Off — pure stretch
            0.3f,    // threshold: unused
            0.08f,   // foldAmount: hint of warmth on the drone
            0.1f,    // foldOffset: subtle stereo
            0.08f,   // lfoRate: ~0.01 Hz — one cycle per ~100 seconds
            0,       // lfoWaveform: Sine
            0.3f,    // lfoDepth: stretch breathes between ~5x and ~10x
            true,    // lfoTargetStretch — drone slowly breathes
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            1.0f     // mix: 100% — fully immersive, no dry signal
        },

        // =====================================================================
        // INDEX 17: Ghost Layer
        // Parallel haunting — auto-freeze ghost sits behind dry pad,
        // folded differently, drifting through captured fragments
        // =====================================================================
        {
            0.65f,   // stretch: ~2x — ghost is expanded
            0.5f,    // position: center
            2,       // freezeMode: Auto — ghost captures fragments
            0.4f,    // threshold: ghost captures readily
            0.22f,   // foldAmount: ghost has its own harmonic character
            0.2f,    // foldOffset: ghost has wide stereo
            0.18f,   // lfoRate: ~0.03 Hz — slow haunting drift
            1,       // lfoWaveform: Triangle — smooth ghost movement
            0.4f,    // lfoDepth: ghost wanders noticeably
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — ghost drifts through captured moment
            true,    // lfoTargetFoldAmount — ghost's harmonics shift
            false,   // lfoTargetFoldOffset
            0.5f     // mix: 50% — ghost present but pad leads
        },

        // =====================================================================
        // INDEX 18: Temporal Rift
        // Torn time — stretched pad with S&H randomly ripping through
        // position, fold creates jagged edges on the tears
        // =====================================================================
        {
            0.75f,   // stretch: ~3x — expanded tears
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.35f,   // threshold: tears happen often
            0.28f,   // foldAmount: edges of tears have harmonic bite
            0.2f,    // foldOffset: torn stereo field
            0.38f,   // lfoRate: ~0.17 Hz — deliberate tear rhythm
            3,       // lfoWaveform: S&H — random position tears
            0.55f,   // lfoDepth: bold tears across the buffer
            true,    // lfoTargetStretch — pitch shifts with tears
            true,    // lfoTargetPosition — position jumps with tears
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.85f    // mix: 85%
        },

        // =====================================================================
        // INDEX 19: Lucid Pulse
        // MANUAL freeze + square LFO — frozen pad hard-switches between
        // two pitch states and stereo positions, hypnotic psybient pulse
        // =====================================================================
        {
            0.58f,   // stretch: ~1.5x base
            0.5f,    // position: center
            1,       // freezeMode: Manual — user controls what gets frozen
            0.3f,    // threshold: unused in Manual
            0.22f,   // foldAmount: harmonic character on both states
            0.15f,   // foldOffset: stereo width
            0.35f,   // lfoRate: ~0.13 Hz — slow hypnotic pulse
            2,       // lfoWaveform: Square — hard alternation between states
            0.45f,   // lfoDepth: dramatic state switches
            true,    // lfoTargetStretch — pitch hard-switches
            true,    // lfoTargetPosition — reads different parts of frozen buffer
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo flips with each state
            0.9f     // mix: 90% — pulsing is the whole point
        }
    };
    // clang-format on

    const char* presetNames[numPresets] = {
        "Init",
        "Turning Through Time",
        "Golden Memories",
        "Omnipotent Observers",
        "Infinite Cogs",
        "Primordial Gear",
        "Observer's Gaze",
        "Golden Helix",
        "Infinite Recursion",
        "Weaver's Dance",
        "Cog Within Cog",
        "Quantum Collapse",
        "Eternal Return",
        "Mirror of Mirrors",
        "Omniscient Dawn",
        "Frozen Cathedral",
        "Event Horizon",
        "Ghost Layer",
        "Temporal Rift",
        "Lucid Pulse"
    };

    juce::String getPresetName(int index)
    {
        if (index < 0 || index >= numPresets)
            return {};
        return presetNames[index];
    }

    std::unique_ptr<juce::XmlElement> createPresetState(int index)
    {
        if (index < 0 || index >= numPresets)
            return nullptr;

        const auto& params = factoryPresets[index];

        auto xml = std::make_unique<juce::XmlElement>("PsycogVST");
        xml->setAttribute("version", PsycogVersion::current);

        xml->setAttribute(ParamIDs::stretch, params.stretch);
        xml->setAttribute(ParamIDs::position, params.position);
        xml->setAttribute(ParamIDs::freezeMode, params.freezeMode);
        xml->setAttribute(ParamIDs::threshold, params.threshold);
        xml->setAttribute(ParamIDs::foldAmount, params.foldAmount);
        xml->setAttribute(ParamIDs::foldOffset, params.foldOffset);
        xml->setAttribute(ParamIDs::lfoRate, params.lfoRate);
        xml->setAttribute(ParamIDs::lfoWaveform, params.lfoWaveform);
        xml->setAttribute(ParamIDs::lfoDepth, params.lfoDepth);
        xml->setAttribute(ParamIDs::lfoTargetStretch, params.lfoTargetStretch ? 1 : 0);
        xml->setAttribute(ParamIDs::lfoTargetPosition, params.lfoTargetPosition ? 1 : 0);
        xml->setAttribute(ParamIDs::lfoTargetFoldAmount, params.lfoTargetFoldAmount ? 1 : 0);
        xml->setAttribute(ParamIDs::lfoTargetFoldOffset, params.lfoTargetFoldOffset ? 1 : 0);
        xml->setAttribute(ParamIDs::mix, params.mix);

        return xml;
    }

    bool applyPreset(juce::AudioProcessorValueTreeState& apvts, int index)
    {
        if (index < 0 || index >= numPresets)
            return false;

        const auto& p = factoryPresets[index];

        // Set parameters through APVTS using convertTo0to1 for correct normalization
        // This properly updates the APVTS tree and notifies the host for automation
        auto setParam = [&](const char* id, float rawValue) {
            if (auto* param = apvts.getParameter(id))
                param->setValueNotifyingHost(param->convertTo0to1(rawValue));
        };

        setParam(ParamIDs::stretch,    p.stretch);
        setParam(ParamIDs::position,   p.position);
        setParam(ParamIDs::threshold,  p.threshold);
        setParam(ParamIDs::foldAmount, p.foldAmount);
        setParam(ParamIDs::foldOffset, p.foldOffset);
        setParam(ParamIDs::lfoRate,    p.lfoRate);
        setParam(ParamIDs::lfoDepth,   p.lfoDepth);
        setParam(ParamIDs::mix,        p.mix);

        // Choice parameters
        setParam(ParamIDs::freezeMode,  static_cast<float>(p.freezeMode));
        setParam(ParamIDs::lfoWaveform, static_cast<float>(p.lfoWaveform));

        // Bool parameters
        setParam(ParamIDs::lfoTargetStretch,    p.lfoTargetStretch ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetPosition,   p.lfoTargetPosition ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetFoldAmount, p.lfoTargetFoldAmount ? 1.0f : 0.0f);
        setParam(ParamIDs::lfoTargetFoldOffset, p.lfoTargetFoldOffset ? 1.0f : 0.0f);

        return true;
    }
}