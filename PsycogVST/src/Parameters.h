/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 3: Project Setup - Parameter layout
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace ParamIDs
{
    // Time Stage
    constexpr const char* stretch = "stretch";
    constexpr const char* position = "position";
    constexpr const char* freezeMode = "freezeMode";
    constexpr const char* threshold = "threshold";

    // Wavefolding
    constexpr const char* foldAmount = "foldAmount";
    constexpr const char* foldOffset = "foldOffset";

    // LFO
    constexpr const char* lfoRate = "lfoRate";
    constexpr const char* lfoWaveform = "lfoWaveform";
    constexpr const char* lfoDepth = "lfoDepth";
    constexpr const char* lfoTargetStretch = "lfoTargetStretch";
    constexpr const char* lfoTargetPosition = "lfoTargetPosition";
    constexpr const char* lfoTargetFoldAmount = "lfoTargetFoldAmount";
    constexpr const char* lfoTargetFoldOffset = "lfoTargetFoldOffset";

    // Output
    constexpr const char* mix = "mix";
    constexpr const char* bypass = "bypass";
}

namespace ParamDefaults
{
    // Time Stage
    constexpr float stretch = 0.5f;      // Normalized (1.0x on log scale)
    constexpr float position = 0.5f;
    constexpr int freezeMode = 0;       // Off
    constexpr float threshold = 0.3f;

    // Wavefolding
    constexpr float foldAmount = 0.0f;
    constexpr float foldOffset = 0.0f;

    // LFO
    constexpr float lfoRate = 0.5f;      // Normalized (~0.5 Hz on log scale)
    constexpr int lfoWaveform = 0;       // Sine
    constexpr float lfoDepth = 0.0f;
    constexpr bool lfoTargetStretch = false;
    constexpr bool lfoTargetPosition = false;
    constexpr bool lfoTargetFoldAmount = false;
    constexpr bool lfoTargetFoldOffset = false;

    // Output
    constexpr float mix = 1.0f;           // 100% wet
}

// Logarithmic parameter conversions
namespace ParamConversions
{
    constexpr float minStretch = 0.1f;
    constexpr float maxStretch = 10.0f;
    constexpr float minLfoRate = 0.01f;
    constexpr float maxLfoRate = 20.0f;

    inline float stretchFromNormalized(float normalized)
    {
        return minStretch * std::pow(maxStretch / minStretch, normalized);
    }

    inline float stretchToNormalized(float stretch)
    {
        return std::log(stretch / minStretch) / std::log(maxStretch / minStretch);
    }

    inline float lfoRateFromNormalized(float normalized)
    {
        return minLfoRate * std::pow(maxLfoRate / minLfoRate, normalized);
    }

    inline float lfoRateToNormalized(float rate)
    {
        return std::log(rate / minLfoRate) / std::log(maxLfoRate / minLfoRate);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();