/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 7: GUI Implementation
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/CustomLookAndFeel.h"
#include "GUI/PresetBar.h"

class PsycogAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   private juce::Timer
{
public:
    explicit PsycogAudioProcessorEditor(PsycogAudioProcessor& p);
    ~PsycogAudioProcessorEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateValueLabels();
    void updateFreezeModeButtons();
    void updateLfoWaveformButtons();

    PsycogAudioProcessor& audioProcessor;
    CustomLookAndFeel laf;
    PresetBar presetBar;

    //==========================================================================
    // Reusable knob group: rotary slider + label above + value label below.
    // The APVTS SliderAttachment binds the slider to a parameter automatically.
    //==========================================================================
    struct KnobGroup
    {
        juce::Slider slider;
        juce::Label  label;
        juce::Label  valueLabel;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach;

        // Create the slider, labels, and attachment. Call once in the constructor.
        void setup(const juce::String& paramId, const juce::String& labelText,
                   juce::AudioProcessorValueTreeState& apvts,
                   juce::LookAndFeel& lf, juce::Component* parent);

        // Position the knob group at (x, y) with the given knob diameter.
        void place(int x, int y, int knobSize);
    };

    // ---- Section 1: TIME ----
    KnobGroup stretch, threshold, position;
    juce::TextButton freezeModeBtns[3];   // Off, Manual, Auto
    juce::Label positionHintLabel;        // "Use in Manual/Auto freeze"

    // ---- Section 2: WAVEFOLDER ----
    KnobGroup foldAmount, foldOffset;
    juce::Label stereoOffsetLabel;        // "L+offset  R-offset"

    // ---- Section 3: LFO ----
    KnobGroup lfoRate, lfoDepth;
    juce::TextButton lfoWaveformBtns[4];  // Sine, Tri, Sq, S&H
    juce::TextButton lfoTargetBtns[4];    // Stretch, Position, FoldAmt, FoldOff
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoTargetAttach[4];

    // ---- Bypass ----
    juce::TextButton bypassBtn { "BYPASS" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttach;

    // ---- Section 4: OUTPUT ----
    KnobGroup mix;
    juce::Label dryWetLabel;              // "DRY  < >  WET"

    // Column bounds computed in resized(), cached for paint()
    juce::Rectangle<int> col[4];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PsycogAudioProcessorEditor)
};
