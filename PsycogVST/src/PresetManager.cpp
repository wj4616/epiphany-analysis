/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Factory presets implementation

  Presets designed for PSYBIENT MUSIC with HEAVY PSYCHEDELIC TRANSFORMATION

  Design Principles:
  - foldAmount ≥ 0.5 for rich harmonic content
  - threshold ≤ 0.3 for frequent freeze cascade layers
  - lfoDepth ≥ 0.5 for dramatic modulation
  - mix ≥ 0.85 for complete transformation
  - Multi-target LFO for complex interweaving movement
  - S&H and Square waveforms for glitchy psychedelic artifacts
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
    constexpr PresetParams factoryPresets[numPresets] = {
        // =====================================================================
        // INDEX 0: Init (Default)
        // Neutral starting point - NO TRANSFORMATION
        // =====================================================================
        {
            ParamDefaults::stretch,
            ParamDefaults::position,
            ParamDefaults::freezeMode,
            ParamDefaults::threshold,
            ParamDefaults::foldAmount,
            ParamDefaults::foldOffset,
            ParamDefaults::lfoRate,
            ParamDefaults::lfoWaveform,
            ParamDefaults::lfoDepth,
            ParamDefaults::lfoTargetStretch,
            ParamDefaults::lfoTargetPosition,
            ParamDefaults::lfoTargetFoldAmount,
            ParamDefaults::lfoTargetFoldOffset,
            ParamDefaults::mix
        },

        // =====================================================================
        // INDEX 1: Turning Through Time (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Evolving morphing psychedelic pad with cascading freeze
        // Heavy: fold=0.65, thresh=0.22, depth=0.55, mix=0.92
        // =====================================================================
        {
            0.349f,  // stretch: 0.5x - compressed granular
            0.5f,    // position: center
            2,       // freezeMode: Auto - cascades
            0.22f,   // threshold: LOW for frequent freeze layers
            0.65f,   // foldAmount: HIGH - rich harmonics
            0.25f,   // foldOffset: asymmetry for stereo width
            0.303f,  // lfoRate: ~0.1 Hz - slow drift
            0,       // lfoWaveform: Sine - smooth
            0.55f,   // lfoDepth: STRONG modulation
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition - time movement
            true,    // lfoTargetFoldAmount - harmonic evolution
            false,   // lfoTargetFoldOffset
            0.92f    // mix: 92% - heavy transformation
        },

        // =====================================================================
        // INDEX 2: Golden Memories (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Warm keys TRANSFORM to aggressive psychedelic
        // Heavy: fold=0.6, thresh=0.2, depth=0.6, mix=0.92
        // =====================================================================
        {
            0.452f,  // stretch: 0.8x - warmth
            0.5f,    // position: static
            2,       // freezeMode: Auto - catches sustained notes
            0.2f,    // threshold: LOW - frequent cascade
            0.6f,    // foldAmount: HIGH - strong harmonics
            0.15f,   // foldOffset: subtle asymmetry
            0.212f,  // lfoRate: ~0.05 Hz - slow evolution
            1,       // lfoWaveform: Triangle - smooth
            0.6f,    // lfoDepth: STRONG
            true,    // lfoTargetStretch - pitch warping
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount - harmonic evolution
            false,   // lfoTargetFoldOffset
            0.92f    // mix: 92% - transformed
        },

        // =====================================================================
        // INDEX 3: Omnipotent Observers (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Thick fat psy pad with overlapping zones - HEAVY
        // Heavy: fold=0.7, thresh=0.22, depth=0.55, mix=0.93
        // =====================================================================
        {
            0.239f,  // stretch: 0.3x - strong compression
            0.5f,    // position: center with LFO
            2,       // freezeMode: Auto - frequent captures
            0.22f,   // threshold: LOW for cascade
            0.7f,    // foldAmount: HIGH harmonic content
            0.45f,   // foldOffset: strong asymmetry for width
            0.356f,  // lfoRate: ~0.15 Hz
            0,       // lfoWaveform: Sine
            0.55f,   // lfoDepth: STRONG modulation
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition - time-scrubbing
            true,    // lfoTargetFoldAmount - harmonic complexity
            false,   // lfoTargetFoldOffset
            0.93f    // mix: 93% - heavy transformation
        },

        // =====================================================================
        // INDEX 4: Infinite Cogs (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Interlocking evolution - mechanical to organic HEAVY
        // Heavy: fold=0.65, thresh=0.22, depth=0.5, mix=0.9
        // =====================================================================
        {
            0.389f,  // stretch: 0.6x - moderate compression
            0.5f,    // position: center with LFO
            2,       // freezeMode: Auto - catches transformation
            0.22f,   // threshold: LOW for cascade
            0.65f,   // foldAmount: HIGH harmonics
            0.35f,   // foldOffset: moderate asymmetry
            0.394f,  // lfoRate: ~0.2 Hz - mechanical rhythm
            1,       // lfoWaveform: Triangle - smooth steps
            0.5f,    // lfoDepth: STRONG modulation
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition - cog rotation
            true,    // lfoTargetFoldAmount - harmonic gears
            false,   // lfoTargetFoldOffset
            0.9f     // mix: 90% - transformed
        },

        // =====================================================================
        // INDEX 5: Primordial Gear (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Deep drone awakening - HEAVY geological time
        // Heavy: fold=0.55, depth=0.5, mix=0.95
        // =====================================================================
        {
            0.75f,   // stretch: ~3x - extended drone
            0.5f,    // position: center
            2,       // freezeMode: Auto - captures sustained input
            0.2f,    // threshold: LOW for long note cascade
            0.55f,   // foldAmount: HIGH harmonics (was 0.15)
            0.08f,   // foldOffset: subtle asymmetry
            0.15f,   // lfoRate: ~0.02 Hz - geological
            0,       // lfoWaveform: Sine - smooth
            0.5f,    // lfoDepth: STRONG (was 0.45)
            true,    // lfoTargetStretch - time dilation
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount - harmonic depth
            false,   // lfoTargetFoldOffset
            0.95f    // mix: 95% - deep immersion
        },

        // =====================================================================
        // INDEX 6: Observer's Gaze (ALREADY EXCELLENT - MINOR TWEAK)
        // Character: Chaos mode - quantum jumps, heavy transformation
        // Heavy: fold=0.65, thresh=0.1, depth=0.55, mix=0.92, S&H
        // =====================================================================
        {
            0.3f,    // stretch: ~0.35x - compressed glitchy
            0.5f,    // position: center
            2,       // freezeMode: Auto - constant triggering
            0.1f,    // threshold: VERY LOW - chaos mode
            0.65f,   // foldAmount: HIGH harmonics
            0.28f,   // foldOffset: strong asymmetry
            0.5f,    // lfoRate: ~0.5 Hz
            3,       // lfoWaveform: S&H - random jumps
            0.55f,   // lfoDepth: STRONG
            true,    // lfoTargetStretch - pitch jumps
            true,    // lfoTargetPosition - position jumps
            true,    // lfoTargetFoldAmount - harmonic jumps
            false,   // lfoTargetFoldOffset
            0.92f    // mix: 92% - heavy transformation
        },

        // =====================================================================
        // INDEX 7: Golden Helix (IMPROVED FOR HEAVY PSYBIENT)
        // Character: DNA spiral through frozen audio - HEAVY interweaving
        // Heavy: fold=0.6, thresh=0.22, depth=0.58, mix=0.9
        // =====================================================================
        {
            0.5f,    // stretch: 1.0x - neutral time
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.22f,   // threshold: LOW for cascade
            0.6f,    // foldAmount: HIGH harmonics
            0.38f,   // foldOffset: strong asymmetry for width
            0.27f,   // lfoRate: ~0.04 Hz - slow spiral
            1,       // lfoWaveform: Triangle - smooth helix
            0.58f,   // lfoDepth: STRONG
            true,    // lfoTargetStretch - vertical spiral
            true,    // lfoTargetPosition - horizontal spiral
            true,    // lfoTargetFoldAmount - harmonic spiral
            false,   // lfoTargetFoldOffset
            0.9f     // mix: 90% - transformed
        },

        // =====================================================================
        // INDEX 8: Infinite Recursion (ALREADY EXCELLENT - MINOR TWEAK)
        // Character: Self-referential loops - HEAVY stepped recursion
        // Heavy: fold=0.75, thresh=0.18, depth=0.65, mix=0.95, Square
        // =====================================================================
        {
            0.2f,    // stretch: ~0.25x - strong compression
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.18f,   // threshold: LOW - frequent triggers
            0.75f,   // foldAmount: VERY HIGH harmonics
            0.48f,   // foldOffset: strong asymmetry
            0.4f,    // lfoRate: ~0.16 Hz
            2,       // lfoWaveform: Square - hard steps
            0.65f,   // lfoDepth: STRONG
            true,    // lfoTargetStretch - stepped pitch
            true,    // lfoTargetPosition - stepped position
            true,    // lfoTargetFoldAmount - stepped harmonics
            false,   // lfoTargetFoldOffset
            0.95f    // mix: 95% - complete recursion
        },

        // =====================================================================
        // INDEX 9: Weaver's Dance (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Threads intertwining - HEAVY harmonic weaving
        // Heavy: fold=0.65, thresh=0.22, depth=0.58, mix=0.92
        // =====================================================================
        {
            0.45f,   // stretch: ~0.75x - slight compression
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.22f,   // threshold: LOW for cascade
            0.65f,   // foldAmount: HIGH harmonics
            0.05f,   // foldOffset: LFO provides movement
            0.33f,   // lfoRate: ~0.06 Hz - slow weave
            1,       // lfoWaveform: Triangle - smooth
            0.58f,   // lfoDepth: STRONG
            false,   // lfoTargetStretch
            false,   // lfoTargetPosition
            true,    // lfoTargetFoldAmount - harmonic weaving
            true,    // lfoTargetFoldOffset - offset weaving
            0.92f    // mix: 92% - transformed
        },

        // =====================================================================
        // INDEX 10: Cog Within Cog (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Fractal machinery - HEAVY nested infinities
        // Heavy: fold=0.7, thresh=0.2, depth=0.55, mix=0.94, 4 targets
        // =====================================================================
        {
            0.35f,   // stretch: ~0.5x - compressed detail
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.2f,    // threshold: LOW - frequent captures
            0.7f,    // foldAmount: HIGH harmonics
            0.35f,   // foldOffset: moderate asymmetry
            0.45f,   // lfoRate: ~0.21 Hz
            0,       // lfoWaveform: Sine - smooth gear rotation
            0.55f,   // lfoDepth: STRONG
            true,    // lfoTargetStretch - outer gear
            true,    // lfoTargetPosition - inner gear
            true,    // lfoTargetFoldAmount - harmonic gear
            true,    // lfoTargetFoldOffset - offset gear (4TH TARGET)
            0.94f    // mix: 94% - complete transformation
        },

        // =====================================================================
        // INDEX 11: Quantum Collapse (ALREADY EXCELLENT - MINOR TWEAK)
        // Character: Probability waves crystallizing - HEAVY chaos
        // Heavy: fold=0.7, thresh=0.12, depth=0.7, mix=0.92, S&H, 4 targets
        // =====================================================================
        {
            0.25f,   // stretch: ~0.28x - compressed quantum
            0.5f,    // position: center
            2,       // freezeMode: Auto - constantly collapsing
            0.12f,   // threshold: VERY LOW - many observations
            0.7f,    // foldAmount: HIGH harmonics
            0.52f,   // foldOffset: strong asymmetry
            0.6f,    // lfoRate: ~0.8 Hz - rapid jumps
            3,       // lfoWaveform: S&H - random collapse
            0.7f,    // lfoDepth: VERY STRONG
            true,    // lfoTargetStretch
            true,    // lfoTargetPosition - observation jumps
            true,    // lfoTargetFoldAmount - harmonic collapse
            true,    // lfoTargetFoldOffset - offset collapse
            0.92f    // mix: 92% - transformed
        },

        // =====================================================================
        // INDEX 12: Eternal Return (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Time folding back - HEAVY eternal cycling
        // Heavy: fold=0.68, thresh=0.22, depth=0.6, mix=0.93, Square
        // =====================================================================
        {
            0.55f,   // stretch: ~1.3x - slightly extended
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.22f,   // threshold: LOW for cascade
            0.68f,   // foldAmount: HIGH harmonics
            0.18f,   // foldOffset: slight asymmetry
            0.35f,   // lfoRate: ~0.14 Hz
            2,       // lfoWaveform: Square - hard return points
            0.6f,    // lfoDepth: STRONG
            true,    // lfoTargetStretch - time cycling
            true,    // lfoTargetPosition - position cycling
            true,    // lfoTargetFoldAmount - harmonic cycling
            false,   // lfoTargetFoldOffset
            0.93f    // mix: 93% - complete return
        },

        // =====================================================================
        // INDEX 13: Mirror of Mirrors (IMPROVED FOR HEAVY PSYBIENT)
        // Character: Infinite reflections - HEAVY hall of mirrors
        // Heavy: fold=0.95, thresh=0.2, depth=0.65, mix=0.98, 3 targets
        // =====================================================================
        {
            0.3f,    // stretch: ~0.35x - compressed reflections
            0.5f,    // position: center
            2,       // freezeMode: Auto
            0.2f,    // threshold: LOW for cascade
            0.95f,   // foldAmount: NEAR MAXIMUM - intense harmonics
            0.62f,   // foldOffset: strong asymmetry - reflection angle
            0.4f,    // lfoRate: ~0.16 Hz
            0,       // lfoWaveform: Sine - smooth reflection
            0.65f,   // lfoDepth: STRONG
            false,   // lfoTargetStretch
            true,    // lfoTargetPosition - depth movement
            true,    // lfoTargetFoldAmount - reflection harmonics
            true,    // lfoTargetFoldOffset - angle shifting
            0.98f    // mix: 98% - almost pure reflection
        },

        // =====================================================================
        // INDEX 14: Omniscient Dawn (ALREADY EXCELLENT)
        // Character: All observers seeing all - MAXIMUM CHAOS
        // Heavy: fold=0.85, thresh=0.05, depth=0.8, mix=1.0, S&H, ALL targets
        // =====================================================================
        {
            0.4f,    // stretch: ~0.6x - moderate compression
            0.5f,    // position: center
            2,       // freezeMode: Auto - constant awakening
            0.05f,   // threshold: MINIMUM - chaos, many triggers
            0.85f,   // foldAmount: VERY HIGH - intense
            0.45f,   // foldOffset: moderate asymmetry
            0.55f,   // lfoRate: ~0.6 Hz - awakening pulse
            3,       // lfoWaveform: S&H - omniscient randomness
            0.8f,    // lfoDepth: VERY STRONG - full omniscience
            true,    // lfoTargetStretch - time awakening
            true,    // lfoTargetPosition - space awakening
            true,    // lfoTargetFoldAmount - harmonic awakening
            true,    // lfoTargetFoldOffset - offset awakening
            1.0f     // mix: 100% - complete omniscience
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
        "Omniscient Dawn"
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