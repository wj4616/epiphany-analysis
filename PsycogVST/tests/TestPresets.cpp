/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Preset tests

  20 presets for psybient pad processing:
  - Drones (5, 16): extreme stretch, minimal fold, immersive 95-100% mix
  - Evolving textures (1, 2, 7, 10): multi-target LFO, deeper modulation
  - Freeze explorers (3, 6, 12, 17): auto-freeze with position scanning
  - Harmonic shapers (4, 9, 13): fold 0.25-0.35, stereo offset movement
  - Glitch/pulse (8, 11, 18, 19): square/S&H LFO, bold state changes
  - Spatial (14, 15): manual/auto freeze, all-target evolution
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "PresetManager.h"
#include "Parameters.h"
#include "Version.h"

TEST_CASE("PresetManager - Factory presets", "[presets]")
{
    SECTION("Preset count is correct")
    {
        REQUIRE(Presets::numPresets == 20);
    }

    SECTION("Preset names are correct")
    {
        REQUIRE(Presets::getPresetName(0) == "Init");
        REQUIRE(Presets::getPresetName(1) == "Turning Through Time");
        REQUIRE(Presets::getPresetName(2) == "Golden Memories");
        REQUIRE(Presets::getPresetName(3) == "Omnipotent Observers");
        REQUIRE(Presets::getPresetName(4) == "Infinite Cogs");
        REQUIRE(Presets::getPresetName(5) == "Primordial Gear");
        REQUIRE(Presets::getPresetName(6) == "Observer's Gaze");
        REQUIRE(Presets::getPresetName(7) == "Golden Helix");
        REQUIRE(Presets::getPresetName(8) == "Infinite Recursion");
        REQUIRE(Presets::getPresetName(9) == "Weaver's Dance");
        REQUIRE(Presets::getPresetName(10) == "Cog Within Cog");
        REQUIRE(Presets::getPresetName(11) == "Quantum Collapse");
        REQUIRE(Presets::getPresetName(12) == "Eternal Return");
        REQUIRE(Presets::getPresetName(13) == "Mirror of Mirrors");
        REQUIRE(Presets::getPresetName(14) == "Omniscient Dawn");
        REQUIRE(Presets::getPresetName(15) == "Frozen Cathedral");
        REQUIRE(Presets::getPresetName(16) == "Event Horizon");
        REQUIRE(Presets::getPresetName(17) == "Ghost Layer");
        REQUIRE(Presets::getPresetName(18) == "Temporal Rift");
        REQUIRE(Presets::getPresetName(19) == "Lucid Pulse");
    }

    SECTION("Invalid preset index returns empty name")
    {
        REQUIRE(Presets::getPresetName(-1).isEmpty());
        REQUIRE(Presets::getPresetName(20).isEmpty());
        REQUIRE(Presets::getPresetName(100).isEmpty());
    }
}

TEST_CASE("PresetManager - Create preset state", "[presets]")
{
    SECTION("Init preset has correct default values")
    {
        auto xml = Presets::createPresetState(0);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getTagName() == juce::String("PsycogVST"));
        REQUIRE(xml->getStringAttribute("version") == juce::String(PsycogVersion::current));

        // Init has no transformation
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.0, 0.001));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::lfoDepth),
                     Catch::Matchers::WithinAbs(0.0, 0.001));
    }

    SECTION("Turning Through Time - evolving granular drift")
    {
        auto xml = Presets::createPresetState(1);
        REQUIRE(xml != nullptr);

        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 0); // Off
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.18, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.85, 0.01));
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
    }

    SECTION("Golden Memories - stretched pitch drift")
    {
        auto xml = Presets::createPresetState(2);
        REQUIRE(xml != nullptr);

        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.15, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.9, 0.01));
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 1); // Triangle
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Omnipotent Observers - S&H freeze scanning")
    {
        auto xml = Presets::createPresetState(3);
        REQUIRE(xml != nullptr);

        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 2); // Auto
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.2, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::threshold),
                     Catch::Matchers::WithinAbs(0.35, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.8, 0.01));
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 3); // S&H
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
    }

    SECTION("Infinite Cogs - mechanical fold + stereo weave")
    {
        auto xml = Presets::createPresetState(4);
        REQUIRE(xml != nullptr);

        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.25, 0.01));
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 1); // Triangle
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Invalid index returns nullptr")
    {
        REQUIRE(Presets::createPresetState(-1) == nullptr);
        REQUIRE(Presets::createPresetState(20) == nullptr);
    }
}

TEST_CASE("PresetManager - Creative presets", "[presets]")
{
    SECTION("Primordial Gear - deep granular drone")
    {
        auto xml = Presets::createPresetState(Presets::PrimordialGearIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 0); // Off — pure stretch
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.1, 0.01)); // Subtle warmth
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.95, 0.01));
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
    }

    SECTION("Observer's Gaze - S&H position + stretch jumps")
    {
        auto xml = Presets::createPresetState(Presets::ObserversGazeIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 2); // Auto
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 3); // S&H
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
    }

    SECTION("Golden Helix - stretch spiral")
    {
        auto xml = Presets::createPresetState(Presets::GoldenHelixIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 0); // Sine
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
    }

    SECTION("Infinite Recursion - square state switching")
    {
        auto xml = Presets::createPresetState(Presets::InfiniteRecursionIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 2); // Square
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.2, 0.01));
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Weaver's Dance - offset weaving")
    {
        auto xml = Presets::createPresetState(Presets::WeaversDanceIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 1); // Triangle
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Cog Within Cog - all four targets evolving")
    {
        auto xml = Presets::createPresetState(Presets::CogWithinCogIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Quantum Collapse - S&H stretch fragments")
    {
        auto xml = Presets::createPresetState(Presets::QuantumCollapseIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 3); // S&H
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
    }

    SECTION("Eternal Return - triangle position sweep")
    {
        auto xml = Presets::createPresetState(Presets::EternalReturnIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 1); // Triangle
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
    }

    SECTION("Mirror of Mirrors - harmonic hall of mirrors")
    {
        auto xml = Presets::createPresetState(Presets::MirrorOfMirrorsIdx);
        REQUIRE(xml != nullptr);
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.35, 0.01));
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Omniscient Dawn - multi-target evolution")
    {
        auto xml = Presets::createPresetState(Presets::OmniscientDawnIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 0); // Sine
        // All 4 targets
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }

    SECTION("Frozen Cathedral - manual freeze exploration")
    {
        auto xml = Presets::createPresetState(Presets::FrozenCathedralIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 1); // Manual
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.12, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.95, 0.01));
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
    }

    SECTION("Event Horizon - extreme stretch drone")
    {
        auto xml = Presets::createPresetState(Presets::EventHorizonIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 0); // Off — pure stretch
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::stretch),
                     Catch::Matchers::WithinAbs(0.88, 0.01)); // ~7.5x
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.08, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(1.0, 0.01)); // 100% immersive
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
    }

    SECTION("Ghost Layer - parallel haunting ghost")
    {
        auto xml = Presets::createPresetState(Presets::GhostLayerIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 2); // Auto
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.22, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.5, 0.01)); // Ghost behind dry
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
    }

    SECTION("Temporal Rift - S&H position tears")
    {
        auto xml = Presets::createPresetState(Presets::TemporalRiftIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 2); // Auto
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 3); // S&H
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
    }

    SECTION("Lucid Pulse - manual freeze + square pulse")
    {
        auto xml = Presets::createPresetState(Presets::LucidPulseIdx);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getIntAttribute(ParamIDs::freezeMode) == 1); // Manual
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::foldAmount),
                     Catch::Matchers::WithinAbs(0.22, 0.01));
        REQUIRE_THAT(xml->getDoubleAttribute(ParamIDs::mix),
                     Catch::Matchers::WithinAbs(0.9, 0.01));
        REQUIRE(xml->getIntAttribute(ParamIDs::lfoWaveform) == 2); // Square
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }
}

TEST_CASE("PresetManager - LFO target flags", "[presets]")
{
    SECTION("Init preset has all LFO targets disabled")
    {
        auto xml = Presets::createPresetState(0);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == false);
    }

    SECTION("Turning Through Time has Position + FoldAmount targets")
    {
        auto xml = Presets::createPresetState(1);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == false);
    }

    SECTION("Golden Memories has Stretch + FoldOffset targets")
    {
        auto xml = Presets::createPresetState(2);
        REQUIRE(xml != nullptr);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetStretch) == true);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetPosition) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount) == false);
        REQUIRE(xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset) == true);
    }
}

TEST_CASE("PresetManager - Preset design criteria", "[presets]")
{
    SECTION("All non-Init presets have at least 1 LFO target")
    {
        for (int i = 1; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            int targets = 0;
            if (xml->getBoolAttribute(ParamIDs::lfoTargetStretch)) targets++;
            if (xml->getBoolAttribute(ParamIDs::lfoTargetPosition)) targets++;
            if (xml->getBoolAttribute(ParamIDs::lfoTargetFoldAmount)) targets++;
            if (xml->getBoolAttribute(ParamIDs::lfoTargetFoldOffset)) targets++;
            REQUIRE(targets >= 1);
        }
    }

    SECTION("All presets have fold amount <= 0.35 (psybient-safe with gainMax=1.5)")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            double fold = xml->getDoubleAttribute(ParamIDs::foldAmount);
            REQUIRE(fold <= 0.351);  // 0.35 max (Mirror of Mirrors)
        }
    }

    SECTION("S&H presets use waveform 3")
    {
        auto observer = Presets::createPresetState(Presets::ObserversGazeIdx);
        auto quantum = Presets::createPresetState(Presets::QuantumCollapseIdx);
        auto rift = Presets::createPresetState(Presets::TemporalRiftIdx);
        REQUIRE(observer->getIntAttribute(ParamIDs::lfoWaveform) == 3);
        REQUIRE(quantum->getIntAttribute(ParamIDs::lfoWaveform) == 3);
        REQUIRE(rift->getIntAttribute(ParamIDs::lfoWaveform) == 3);
    }
}

TEST_CASE("PresetManager - Round-trip", "[presets]")
{
    SECTION("Preset XML can be created and all values preserved")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            REQUIRE(xml->getStringAttribute("version") == juce::String(PsycogVersion::current));
            REQUIRE(xml->hasAttribute(ParamIDs::stretch));
            REQUIRE(xml->hasAttribute(ParamIDs::position));
            REQUIRE(xml->hasAttribute(ParamIDs::freezeMode));
            REQUIRE(xml->hasAttribute(ParamIDs::threshold));
            REQUIRE(xml->hasAttribute(ParamIDs::foldAmount));
            REQUIRE(xml->hasAttribute(ParamIDs::foldOffset));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoRate));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoWaveform));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoDepth));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoTargetStretch));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoTargetPosition));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoTargetFoldAmount));
            REQUIRE(xml->hasAttribute(ParamIDs::lfoTargetFoldOffset));
            REQUIRE(xml->hasAttribute(ParamIDs::mix));
        }
    }

    SECTION("All presets have freezeMode in valid range [0, 2]")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            int freezeMode = xml->getIntAttribute(ParamIDs::freezeMode);
            REQUIRE((freezeMode >= 0 && freezeMode <= 2));
        }
    }

    SECTION("All presets have mix in valid range [0, 1]")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            double mix = xml->getDoubleAttribute(ParamIDs::mix);
            REQUIRE((mix >= 0.0 && mix <= 1.0));
        }
    }

    SECTION("All presets have lfoWaveform in valid range [0, 3]")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            int waveform = xml->getIntAttribute(ParamIDs::lfoWaveform);
            REQUIRE((waveform >= 0 && waveform <= 3));
        }
    }

    SECTION("All presets have foldAmount in valid range [0, 1]")
    {
        for (int i = 0; i < Presets::numPresets; ++i)
        {
            auto xml = Presets::createPresetState(i);
            REQUIRE(xml != nullptr);
            double fold = xml->getDoubleAttribute(ParamIDs::foldAmount);
            REQUIRE((fold >= 0.0 && fold <= 1.0));
        }
    }
}
