/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Factory presets
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <array>
#include <string_view>

namespace Presets
{
    // Factory preset names (matching spec + creative extensions)
    constexpr std::string_view DefaultName = "Init";

    // Number of factory presets (including default/init)
    constexpr int numPresets = 20;

    // Preset index constants
    enum PresetIndex
    {
        Init = 0,
        TurningThroughTimeIdx = 1,
        GoldenMemoriesIdx = 2,
        OmnipotentObserversIdx = 3,
        InfiniteCogsIdx = 4,
        PrimordialGearIdx = 5,
        ObserversGazeIdx = 6,
        GoldenHelixIdx = 7,
        InfiniteRecursionIdx = 8,
        WeaversDanceIdx = 9,
        CogWithinCogIdx = 10,
        QuantumCollapseIdx = 11,
        EternalReturnIdx = 12,
        MirrorOfMirrorsIdx = 13,
        OmniscientDawnIdx = 14,
        FrozenCathedralIdx = 15,
        EventHorizonIdx = 16,
        GhostLayerIdx = 17,
        TemporalRiftIdx = 18,
        LucidPulseIdx = 19
    };

    /**
     * Get the name of a factory preset by index.
     * @param index Preset index (0 to numPresets-1)
     * @return Preset name, or empty string if index invalid
     */
    juce::String getPresetName(int index);

    /**
     * Apply a factory preset to an APVTS.
     * @param apvts The AudioProcessorValueTreeState to modify
     * @param index Preset index (0 to numPresets-1)
     * @return true if preset was applied, false if index invalid
     */
    bool applyPreset(juce::AudioProcessorValueTreeState& apvts, int index);

    /**
     * Create XML state for a preset by index.
     * @param index Preset index (0 to numPresets-1)
     * @return XML element representing the preset state, or nullptr if index invalid
     */
    std::unique_ptr<juce::XmlElement> createPresetState(int index);
}