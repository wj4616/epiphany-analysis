#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "CustomLookAndFeel.h"

class PsycogAudioProcessor;

// PresetBar shows the current preset name with prev/next arrows and index.
// PsycogVST uses setCurrentProgram() for preset changes (not an APVTS parameter),
// so this component reads/writes via the processor directly.
class PresetBar : public juce::Component,
                  private juce::Button::Listener,
                  private juce::Timer
{
public:
    explicit PresetBar(PsycogAudioProcessor& processor);
    ~PresetBar() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void buttonClicked(juce::Button*) override;
    void timerCallback() override;

    PsycogAudioProcessor& processor;

    juce::TextButton prevBtn { "<" };
    juce::TextButton nextBtn { ">" };
    juce::Label      presetNameLabel;
    juce::Label      presetIndexLabel;   // "3 / 20"

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBar)
};
