#include "PresetBar.h"
#include "../PluginProcessor.h"
#include "../PresetManager.h"

PresetBar::PresetBar(PsycogAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(prevBtn);
    addAndMakeVisible(nextBtn);
    prevBtn.addListener(this);
    nextBtn.addListener(this);

    presetNameLabel.setJustificationType(juce::Justification::centred);
    presetNameLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    presetNameLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::gold());
    addAndMakeVisible(presetNameLabel);

    presetIndexLabel.setJustificationType(juce::Justification::centred);
    presetIndexLabel.setFont(juce::Font(9.0f));
    presetIndexLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::textSecondary());
    addAndMakeVisible(presetIndexLabel);

    // Update preset name at ~12fps
    startTimer(80);
}

PresetBar::~PresetBar()
{
    prevBtn.removeListener(this);
    nextBtn.removeListener(this);
}

void PresetBar::buttonClicked(juce::Button* btn)
{
    int idx = processor.getCurrentProgram();
    int total = processor.getNumPrograms();

    if (btn == &prevBtn)
        idx = (idx - 1 + total) % total;
    else
        idx = (idx + 1) % total;

    processor.setCurrentProgram(idx);
}

void PresetBar::timerCallback()
{
    int idx = processor.getCurrentProgram();
    int total = processor.getNumPrograms();

    juce::String name = Presets::getPresetName(idx);
    if (presetNameLabel.getText() != name)
        presetNameLabel.setText(name, juce::dontSendNotification);

    juce::String indexStr = juce::String(idx + 1) + " / " + juce::String(total);
    if (presetIndexLabel.getText() != indexStr)
        presetIndexLabel.setText(indexStr, juce::dontSendNotification);
}

void PresetBar::paint(juce::Graphics& g)
{
    g.setColour(CustomLookAndFeel::surface());
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
    g.setColour(CustomLookAndFeel::neonPrimary().withAlpha(0.2f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 4.0f, 1.0f);
}

void PresetBar::resized()
{
    auto b = getLocalBounds().reduced(4);
    prevBtn.setBounds(b.removeFromLeft(32));
    b.removeFromLeft(4);
    nextBtn.setBounds(b.removeFromRight(32));
    b.removeFromRight(4);

    // Index label on the right side of the name area
    presetIndexLabel.setBounds(b.removeFromRight(60));
    presetNameLabel.setBounds(b);
}
