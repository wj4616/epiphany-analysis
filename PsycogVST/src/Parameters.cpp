/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 3: Project Setup - Parameter layout implementation
*/

#include "Parameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Time Stage
    // Stretch: log scale 0.1x to 10x, center at 1.0x (normalized 0.5)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::stretch, "Stretch", 0.0f, 1.0f, ParamDefaults::stretch));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::position, "Position", 0.0f, 1.0f, ParamDefaults::position));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        ParamIDs::freezeMode, "Freeze Mode",
        juce::StringArray{"Off", "Manual", "Auto"}, ParamDefaults::freezeMode));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::threshold, "Threshold", 0.0f, 1.0f, ParamDefaults::threshold));

    // Wavefolding
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::foldAmount, "Fold Amount", 0.0f, 1.0f, ParamDefaults::foldAmount));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::foldOffset, "Fold Offset", -1.0f, 1.0f, ParamDefaults::foldOffset));

    // LFO
    // LFO Rate: log scale 0.01 Hz to 20 Hz
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::lfoRate, "LFO Rate", 0.0f, 1.0f, ParamDefaults::lfoRate));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        ParamIDs::lfoWaveform, "LFO Waveform",
        juce::StringArray{"Sine", "Triangle", "Square", "S&H"}, ParamDefaults::lfoWaveform));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::lfoDepth, "LFO Depth", 0.0f, 1.0f, ParamDefaults::lfoDepth));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::lfoTargetStretch, "LFO Target Stretch", ParamDefaults::lfoTargetStretch));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::lfoTargetPosition, "LFO Target Position", ParamDefaults::lfoTargetPosition));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::lfoTargetFoldAmount, "LFO Target Fold Amount", ParamDefaults::lfoTargetFoldAmount));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::lfoTargetFoldOffset, "LFO Target Fold Offset", ParamDefaults::lfoTargetFoldOffset));

    // Output
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::mix, "Mix", 0.0f, 1.0f, ParamDefaults::mix));

    // Bypass (integrates with host bypass via getBypassParameter)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::bypass, "Bypass", false));

    return { params.begin(), params.end() };
}