/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Factory presets implementation

  20 presets with progressive intensity:
  - Subtle (1-2): fold 0.30-0.35, mix 0.85
  - Moderate (3-5): fold 0.30-0.35, mix 0.85-0.90
  - Heavy (6-12): fold 0.35-0.40, mix 0.85-0.90
  - Extreme (13-14): fold 0.50-0.55, mix 0.95-1.0
  - Creative (15-19): fold 0.25-0.40, mix 0.50-0.95 (varied for specific character)
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
    // Presets calibrated for synthesizer pads with long release.
    // Fold amounts kept low (0.0-0.20) to avoid harshness on harmonically rich input.
    // Mix blends dry pad underneath for body. Slow LFO rates for evolving textures.
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
        // Gentle granular shimmer — pad drifts slowly through grain position
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x — gentle expansion
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.08f,   // foldAmount: just a touch of warmth
            0.1f,    // foldOffset: subtle stereo widening
            0.25f,   // lfoRate: ~0.035 Hz — very slow drift
            0,       // lfoWaveform: Sine
            0.3f,    // lfoDepth: gentle movement
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — slow position drift
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.7f     // mix: 70% — pad body preserved
        },

        // =====================================================================
        // INDEX 2: Golden Memories
        // Warm stretched texture — pad expanded into soft granular cloud
        // =====================================================================
        {
            0.65f,   // stretch: ~2x — noticeable expansion
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.1f,    // foldAmount: gentle harmonic enrichment
            0.12f,   // foldOffset: stereo warmth
            0.18f,   // lfoRate: ~0.03 Hz — glacial evolution
            1,       // lfoWaveform: Triangle — smooth ramps
            0.25f,   // lfoDepth: subtle
            true,    // lfoTargetStretch — slow pitch drift
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 3: Omnipotent Observers
        // Auto-freeze layers — pad fragments captured and layered
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x — slightly expanded
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.5f,    // threshold: only louder passages trigger
            0.06f,   // foldAmount: barely there — just grain texture
            0.08f,   // foldOffset: subtle stereo
            0.22f,   // lfoRate: ~0.04 Hz — slow sweep
            0,       // lfoWaveform: Sine
            0.3f,    // lfoDepth: gentle
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — drift through frozen layers
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.65f    // mix: 65% — dry pad anchors the layers
        },

        // =====================================================================
        // INDEX 4: Infinite Cogs
        // Slow mechanical movement — triangle LFO on position
        // =====================================================================
        {
            0.5f,    // stretch: 1.0x — natural time
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.12f,   // foldAmount: light saturation
            0.15f,   // foldOffset: stereo movement
            0.303f,  // lfoRate: ~0.1 Hz — slow pulse
            1,       // lfoWaveform: Triangle
            0.35f,   // lfoDepth: noticeable movement
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo weaving
            0.65f    // mix: 65%
        },

        // =====================================================================
        // INDEX 5: Primordial Gear
        // Deep granular drone — extreme stretch, no fold, immersive
        // =====================================================================
        {
            0.75f,   // stretch: ~3x — deep expansion
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.0f,    // foldAmount: none — pure granular texture
            0.0f,    // foldOffset: none
            0.15f,   // lfoRate: ~0.02 Hz — glacial
            0,       // lfoWaveform: Sine
            0.2f,    // lfoDepth: slow breathing
            true,    // lfoTargetStretch — stretch breathes 2x-4x
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.8f     // mix: 80% — deep but grounded
        },

        // =====================================================================
        // INDEX 6: Observer's Gaze
        // S&H position jumps on frozen pad — gentle glitch texture
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.45f,   // threshold: triggers on louder moments
            0.05f,   // foldAmount: minimal — let the grains speak
            0.1f,    // foldOffset: subtle stereo
            0.394f,  // lfoRate: ~0.2 Hz — gentle random rhythm
            3,       // lfoWaveform: S&H — random steps
            0.25f,   // lfoDepth: moderate jumps
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — random position jumps in frozen buffer
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.6f     // mix: 60%
        },

        // =====================================================================
        // INDEX 7: Golden Helix
        // Spiraling stretch drift — pad slowly rises and falls in pitch
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x — base slightly expanded
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.1f,    // foldAmount: light warmth
            0.1f,    // foldOffset: subtle stereo
            0.2f,    // lfoRate: ~0.04 Hz — slow spiral
            0,       // lfoWaveform: Sine — smooth
            0.3f,    // lfoDepth: stretch drifts between ~0.8x and ~2x
            true,    // lfoTargetStretch — pitch spiral
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.75f    // mix: 75%
        },

        // =====================================================================
        // INDEX 8: Infinite Recursion
        // Square LFO steps between two stretch states — pad alternates
        // =====================================================================
        {
            0.5f,    // stretch: 1.0x — center point
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.08f,   // foldAmount: touch of grit on transitions
            0.0f,    // foldOffset: centered
            0.356f,  // lfoRate: ~0.15 Hz — slow alternation
            2,       // lfoWaveform: Square — hard steps
            0.2f,    // lfoDepth: steps between ~0.7x and ~1.4x
            true,    // lfoTargetStretch — stepped pitch
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 9: Weaver's Dance
        // Stereo offset weaving — fold offset LFO creates moving stereo field
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.15f,   // foldAmount: enough to make offset audible
            0.0f,    // foldOffset: centered — LFO moves it
            0.25f,   // lfoRate: ~0.035 Hz — slow weave
            1,       // lfoWaveform: Triangle — smooth sweeps
            0.3f,    // lfoDepth: offset sweeps gently
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo field moves
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 10: Cog Within Cog
        // Dual modulation — position + offset move together, slow evolve
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.12f,   // foldAmount: light saturation
            0.05f,   // foldOffset: subtle base offset
            0.2f,    // lfoRate: ~0.04 Hz — slow
            0,       // lfoWaveform: Sine
            0.25f,   // lfoDepth: gentle
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — time movement
            false,   // lfoTargetFoldAmount
            true,    // lfoTargetFoldOffset — stereo movement
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 11: Quantum Collapse
        // S&H on stretch — random pitch fragments from frozen pad
        // =====================================================================
        {
            0.5f,    // stretch: 1.0x base
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.45f,   // threshold: triggers on louder passages
            0.06f,   // foldAmount: minimal
            0.08f,   // foldOffset: subtle
            0.45f,   // lfoRate: ~0.21 Hz
            3,       // lfoWaveform: S&H — random pitch fragments
            0.2f,    // lfoDepth: moderate jumps
            true,    // lfoTargetStretch — random pitch shifts
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.6f     // mix: 60% — keep dry pad as anchor
        },

        // =====================================================================
        // INDEX 12: Eternal Return
        // Auto-freeze with slow position sweep — looping through moments
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.5f,    // threshold: only loud notes trigger
            0.08f,   // foldAmount: light warmth
            0.1f,    // foldOffset: subtle stereo
            0.18f,   // lfoRate: ~0.03 Hz — glacial sweep
            1,       // lfoWaveform: Triangle — smooth
            0.35f,   // lfoDepth: wide position sweep through frozen content
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — sweeps through frozen moment
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 13: Mirror of Mirrors
        // Higher fold — the "drive" preset, still controlled for pads
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x
            0.5f,    // position: center
            0,       // freezeMode: Off
            0.3f,    // threshold: unused
            0.2f,    // foldAmount: noticeable saturation (but not harsh)
            0.15f,   // foldOffset: stereo spread
            0.22f,   // lfoRate: ~0.04 Hz
            0,       // lfoWaveform: Sine
            0.2f,    // lfoDepth: fold amount breathes
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount — harmonic breathing
            false,   // lfoTargetFoldOffset
            0.65f    // mix: 65%
        },

        // =====================================================================
        // INDEX 14: Omniscient Dawn
        // Multi-target slow evolution — everything drifts, gentle chaos
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.45f,   // threshold: moderate
            0.1f,    // foldAmount: light
            0.1f,    // foldOffset: subtle stereo
            0.15f,   // lfoRate: ~0.02 Hz — glacial
            0,       // lfoWaveform: Sine
            0.2f,    // lfoDepth: gentle
            true,    // lfoTargetStretch — slow pitch drift
            true,    // lfoTargetPosition — slow position drift
            true,    // lfoTargetFoldAmount — subtle harmonic shift
            true,    // lfoTargetFoldOffset — stereo shifts
            0.7f     // mix: 70%
        },

        // =====================================================================
        // INDEX 15: Frozen Cathedral
        // MANUAL freeze — freeze a pad, it becomes vast ambient space
        // Position LFO drifts through the frozen moment
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x — slight pitch shift on frozen content
            0.5f,    // position: center — LFO drifts through
            1,       // freezeMode: Manual — user controls freeze
            0.3f,    // threshold: unused in Manual mode
            0.05f,   // foldAmount: barely there
            0.1f,    // foldOffset: subtle stereo width
            0.15f,   // lfoRate: ~0.02 Hz — slow cathedral drift
            0,       // lfoWaveform: Sine
            0.4f,    // lfoDepth: wide position sweep
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — drift through frozen moment
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.8f     // mix: 80%
        },

        // =====================================================================
        // INDEX 16: Event Horizon
        // Extreme stretch — pad becomes glacial drone, no fold needed
        // =====================================================================
        {
            0.85f,   // stretch: ~6.3x — extreme time dilation
            0.5f,    // position: center
            0,       // freezeMode: Off — pure stretch
            0.3f,    // threshold: unused
            0.0f,    // foldAmount: none — pure granular
            0.0f,    // foldOffset: none
            0.12f,   // lfoRate: ~0.015 Hz — one cycle per ~67 seconds
            0,       // lfoWaveform: Sine
            0.15f,   // lfoDepth: stretch gently breathes
            true,    // lfoTargetStretch — breathing between ~5x and ~8x
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.85f    // mix: 85% — immersive
        },

        // =====================================================================
        // INDEX 17: Ghost Layer
        // Low mix — granular ghost sits behind the dry pad
        // =====================================================================
        {
            0.6f,    // stretch: ~1.6x — ghost is slightly expanded
            0.5f,    // position: center
            2,       // freezeMode: Auto — ghost captures fragments
            0.5f,    // threshold: only louder notes trigger
            0.05f,   // foldAmount: minimal
            0.08f,   // foldOffset: ghost has subtle stereo
            0.2f,    // lfoRate: ~0.04 Hz — slow haunting
            0,       // lfoWaveform: Sine
            0.25f,   // lfoDepth: gentle drift
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — ghost drifts through captured moment
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.4f     // mix: 40% — ghost barely there, behind dry pad
        },

        // =====================================================================
        // INDEX 18: Temporal Rift
        // Stretched + S&H position — pad tears into random fragments
        // =====================================================================
        {
            0.7f,    // stretch: ~2.5x — expanded
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.45f,   // threshold: moderate
            0.08f,   // foldAmount: touch of texture
            0.1f,    // foldOffset: subtle stereo
            0.356f,  // lfoRate: ~0.15 Hz
            3,       // lfoWaveform: S&H — random position tears
            0.3f,    // lfoDepth: noticeable jumps
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition — random position in frozen content
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.65f    // mix: 65%
        },

        // =====================================================================
        // INDEX 19: Lucid Pulse
        // MANUAL freeze + Square LFO — frozen pad pulses between states
        // =====================================================================
        {
            0.5f,    // stretch: 1.0x — natural pitch
            0.5f,    // position: center
            1,       // freezeMode: Manual — user controls freeze
            0.3f,    // threshold: unused
            0.1f,    // foldAmount: light warmth
            0.0f,    // foldOffset: centered
            0.394f,  // lfoRate: ~0.2 Hz — slow pulse
            2,       // lfoWaveform: Square — hard alternation
            0.2f,    // lfoDepth: moderate pitch steps
            true,    // lfoTargetStretch — pitch alternates between two states
            false,   // lfoTargetPosition
            false,   // lfoTargetFoldAmount
            false,   // lfoTargetFoldOffset
            0.7f     // mix: 70%
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