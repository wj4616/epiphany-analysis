/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 7: GUI Implementation
*/

#include "PluginEditor.h"
#include "Parameters.h"
#include "PresetManager.h"
#include "Version.h"

//==============================================================================
// KnobGroup helper — sets up a rotary slider with label + value display
//==============================================================================
void PsycogAudioProcessorEditor::KnobGroup::setup(
    const juce::String& paramId, const juce::String& labelText,
    juce::AudioProcessorValueTreeState& apvts,
    juce::LookAndFeel& lf, juce::Component* parent)
{
    // Configure the rotary slider
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&lf);
    parent->addAndMakeVisible(slider);

    // Parameter name label (above the knob)
    label.setText(labelText, juce::dontSendNotification);
    label.setFont(juce::Font(10.0f, juce::Font::bold));
    label.setColour(juce::Label::textColourId, CustomLookAndFeel::textPrimary());
    label.setJustificationType(juce::Justification::centred);
    label.setMinimumHorizontalScale(1.0f);
    parent->addAndMakeVisible(label);

    // Value readout label (below the knob, gold text)
    valueLabel.setFont(juce::Font(9.0f, juce::Font::bold));
    valueLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::gold());
    valueLabel.setJustificationType(juce::Justification::centred);
    parent->addAndMakeVisible(valueLabel);

    // Bind the slider to the APVTS parameter
    attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, paramId, slider);
}

void PsycogAudioProcessorEditor::KnobGroup::place(int x, int y, int sz)
{
    // Label is wider than the knob so long names don't get truncated
    const int labelW = sz + 40;
    const int labelX = x + sz / 2 - labelW / 2;
    label.setBounds(labelX, y, labelW, 13);
    slider.setBounds(x, y + 13, sz, sz);
    valueLabel.setBounds(labelX, y + 13 + sz, labelW, 12);
}

//==============================================================================
// Constructor
//==============================================================================
PsycogAudioProcessorEditor::PsycogAudioProcessorEditor(PsycogAudioProcessor& p)
    : AudioProcessorEditor(p),
      audioProcessor(p),
      presetBar(p)
{
    setSize(780, 480);
    setLookAndFeel(&laf);

    // ---- Bypass button (top-right, next to version tag) ----
    bypassBtn.setClickingTogglesState(true);
    bypassBtn.setLookAndFeel(&laf);
    bypassBtn.setColour(juce::TextButton::buttonOnColourId,
                        juce::Colour(0xffff4040));  // Red when bypassed
    addAndMakeVisible(bypassBtn);
    bypassAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        p.apvts, ParamIDs::bypass, bypassBtn);

    // ---- Preset bar ----
    presetBar.setLookAndFeel(&laf);
    addAndMakeVisible(presetBar);

    // ---- Section 1: TIME ----
    stretch.setup(ParamIDs::stretch,     "STRETCH",   p.apvts, laf, this);
    threshold.setup(ParamIDs::threshold, "THRESHOLD", p.apvts, laf, this);
    position.setup(ParamIDs::position,   "POSITION",  p.apvts, laf, this);

    // Hint label for position knob
    positionHintLabel.setText("active in freeze mode", juce::dontSendNotification);
    positionHintLabel.setFont(juce::Font(8.0f));
    positionHintLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::freezeAccent().withAlpha(0.6f));
    positionHintLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(positionHintLabel);

    // Freeze mode buttons (Off / Manual / Auto) — choice parameter, not toggle
    juce::StringArray fmLabels{ "OFF", "MANUAL", "AUTO" };
    for (int i = 0; i < 3; ++i)
    {
        freezeModeBtns[i].setButtonText(fmLabels[i]);
        freezeModeBtns[i].setClickingTogglesState(false);
        freezeModeBtns[i].setLookAndFeel(&laf);
        // Cyan accent to match the freeze sub-panel
        freezeModeBtns[i].setColour(juce::TextButton::buttonOnColourId,
                                    CustomLookAndFeel::freezeAccent());
        freezeModeBtns[i].onClick = [this, i]()
        {
            if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(
                    audioProcessor.apvts.getParameter(ParamIDs::freezeMode)))
                param->setValueNotifyingHost(param->convertTo0to1(i));
            updateFreezeModeButtons();
        };
        addAndMakeVisible(freezeModeBtns[i]);
    }

    // ---- Section 2: WAVEFOLDER ----
    foldAmount.setup(ParamIDs::foldAmount,  "FOLD",   p.apvts, laf, this);
    foldOffset.setup(ParamIDs::foldOffset,  "OFFSET", p.apvts, laf, this);

    stereoOffsetLabel.setText("L +offset    R -offset", juce::dontSendNotification);
    stereoOffsetLabel.setFont(juce::Font(9.0f));
    stereoOffsetLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::textSecondary());
    stereoOffsetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(stereoOffsetLabel);

    // ---- Section 3: LFO ----
    lfoRate.setup(ParamIDs::lfoRate,   "RATE",  p.apvts, laf, this);
    lfoDepth.setup(ParamIDs::lfoDepth, "DEPTH", p.apvts, laf, this);

    // LFO waveform buttons (Sine / Tri / Sq / S&H) — choice parameter
    juce::StringArray wfLabels{ "SIN", "TRI", "SQ", "S&H" };
    for (int i = 0; i < 4; ++i)
    {
        lfoWaveformBtns[i].setButtonText(wfLabels[i]);
        lfoWaveformBtns[i].setClickingTogglesState(false);
        lfoWaveformBtns[i].setLookAndFeel(&laf);
        lfoWaveformBtns[i].onClick = [this, i]()
        {
            if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(
                    audioProcessor.apvts.getParameter(ParamIDs::lfoWaveform)))
                param->setValueNotifyingHost(param->convertTo0to1(i));
            updateLfoWaveformButtons();
        };
        addAndMakeVisible(lfoWaveformBtns[i]);
    }

    // LFO target toggles (Stretch / Position / Fold / Offset) — bool parameters
    const char* targetParamIds[4] = {
        ParamIDs::lfoTargetStretch, ParamIDs::lfoTargetPosition,
        ParamIDs::lfoTargetFoldAmount, ParamIDs::lfoTargetFoldOffset
    };
    juce::StringArray targetLabels{ "STR", "POS", "FLD", "OFS" };
    for (int i = 0; i < 4; ++i)
    {
        lfoTargetBtns[i].setButtonText(targetLabels[i]);
        lfoTargetBtns[i].setClickingTogglesState(true);
        lfoTargetBtns[i].setLookAndFeel(&laf);
        // Gold accent for active targets (set once, ButtonAttachment handles toggle state)
        lfoTargetBtns[i].setColour(juce::TextButton::buttonOnColourId,
                                   CustomLookAndFeel::gold());
        addAndMakeVisible(lfoTargetBtns[i]);
        lfoTargetAttach[i] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            p.apvts, targetParamIds[i], lfoTargetBtns[i]);
    }

    // ---- Section 4: OUTPUT ----
    mix.setup(ParamIDs::mix, "MIX", p.apvts, laf, this);

    dryWetLabel.setText("DRY  < >  WET", juce::dontSendNotification);
    dryWetLabel.setFont(juce::Font(9.0f));
    dryWetLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::textSecondary());
    dryWetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dryWetLabel);

    // Sync button states immediately so they're correct on first paint
    updateFreezeModeButtons();
    updateLfoWaveformButtons();

    // Start timer for value label updates (~15fps)
    startTimer(66);
}

PsycogAudioProcessorEditor::~PsycogAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
// Timer callback — update value labels and button states
//==============================================================================
void PsycogAudioProcessorEditor::timerCallback()
{
    updateValueLabels();
    updateFreezeModeButtons();
    updateLfoWaveformButtons();

}

void PsycogAudioProcessorEditor::updateFreezeModeButtons()
{
    int val = (int)audioProcessor.apvts.getRawParameterValue(ParamIDs::freezeMode)->load();
    for (int i = 0; i < 3; ++i)
        freezeModeBtns[i].setToggleState(i == val, juce::dontSendNotification);
}

void PsycogAudioProcessorEditor::updateLfoWaveformButtons()
{
    int val = (int)audioProcessor.apvts.getRawParameterValue(ParamIDs::lfoWaveform)->load();
    for (int i = 0; i < 4; ++i)
        lfoWaveformBtns[i].setToggleState(i == val, juce::dontSendNotification);
}

void PsycogAudioProcessorEditor::updateValueLabels()
{
    // Helper lambda to read raw parameter values
    auto getV = [&](const char* id) {
        return audioProcessor.apvts.getRawParameterValue(id)->load();
    };

    // Stretch: convert normalized 0-1 to actual multiplier for display
    float stretchNorm = getV(ParamIDs::stretch);
    float stretchActual = ParamConversions::stretchFromNormalized(stretchNorm);
    stretch.valueLabel.setText(juce::String(stretchActual, 2) + "x",
                               juce::dontSendNotification);

    // Position: display as percentage
    position.valueLabel.setText(juce::String((int)(getV(ParamIDs::position) * 100)) + "%",
                                juce::dontSendNotification);

    // Threshold: display as 0.00-1.00
    threshold.valueLabel.setText(juce::String(getV(ParamIDs::threshold), 2),
                                 juce::dontSendNotification);

    // Fold amount: display as percentage
    foldAmount.valueLabel.setText(juce::String((int)(getV(ParamIDs::foldAmount) * 100)) + "%",
                                  juce::dontSendNotification);

    // Fold offset: display as -1.00 to +1.00
    float offVal = getV(ParamIDs::foldOffset);
    juce::String offStr = (offVal >= 0 ? "+" : "") + juce::String(offVal, 2);
    foldOffset.valueLabel.setText(offStr, juce::dontSendNotification);

    // LFO rate: convert normalized to Hz
    float rateNorm = getV(ParamIDs::lfoRate);
    float rateHz = ParamConversions::lfoRateFromNormalized(rateNorm);
    juce::String rateStr = rateHz < 1.0f ? juce::String(rateHz, 2) + "Hz"
                                         : juce::String(rateHz, 1) + "Hz";
    lfoRate.valueLabel.setText(rateStr, juce::dontSendNotification);

    // LFO depth: display as percentage
    lfoDepth.valueLabel.setText(juce::String((int)(getV(ParamIDs::lfoDepth) * 100)) + "%",
                                juce::dontSendNotification);

    // Mix: display as percentage
    mix.valueLabel.setText(juce::String((int)(getV(ParamIDs::mix) * 100)) + "%",
                           juce::dontSendNotification);
}

//==============================================================================
// Paint — background, gradient, column panels, headers, signal flow
//==============================================================================
void PsycogAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Dark background fill
    g.fillAll(CustomLookAndFeel::bg());

    // Subtle purple gradient overlay from top
    juce::ColourGradient grad(CustomLookAndFeel::neonSecondary().withAlpha(0.08f),
                              0, 0,
                              CustomLookAndFeel::bg(),
                              0, (float)getHeight(), false);
    g.setGradientFill(grad);
    g.fillAll();

    // Plugin title
    g.setColour(CustomLookAndFeel::textPrimary());
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("PSYCOG", 12, 8, 200, 28, juce::Justification::centredLeft);

    // Version tag + build timestamp (verify REAPER loads the latest binary)
    g.setFont(juce::Font(9.0f));
    g.setColour(CustomLookAndFeel::gold().withAlpha(0.5f));
    g.drawText(juce::String("v") + PsycogVersion::current + " | " + PSYCOG_BUILD_STAMP,
               getWidth() - 220, 8, 212, 14, juce::Justification::centredRight);

    // Signal flow strip
    const int sfY = 82;
    g.setColour(CustomLookAndFeel::textSecondary());
    g.setFont(juce::Font(9.0f));
    g.drawText("INPUT >> STRETCH >> FOLD >> MIX >> OUTPUT",
               12, sfY, getWidth() - 24, 12, juce::Justification::centredLeft);
    g.setColour(CustomLookAndFeel::neonPrimary().withAlpha(0.2f));
    g.drawLine(12, (float)(sfY + 13), (float)getWidth() - 12, (float)(sfY + 13), 1.0f);

    // Column panel backgrounds
    const char* headers[] = { "1  TIME", "2  WAVEFOLD", "3  LFO", "4  OUTPUT" };
    for (int c = 0; c < 4; ++c)
    {
        // Panel background
        g.setColour(CustomLookAndFeel::surface().withAlpha(0.8f));
        g.fillRoundedRectangle(col[c].toFloat(), 6.0f);

        // Panel border
        g.setColour(CustomLookAndFeel::neonPrimary().withAlpha(0.12f));
        g.drawRoundedRectangle(col[c].toFloat(), 6.0f, 1.0f);

        // Column header text
        g.setColour(CustomLookAndFeel::neonPrimary().withAlpha(0.8f));
        g.setFont(juce::Font(9.0f, juce::Font::bold));
        g.drawText(headers[c], col[c].getX() + 6, col[c].getY() + 6,
                   col[c].getWidth() - 8, 12, juce::Justification::centredLeft);
    }

    // Freeze sub-panel in TIME column (buttons + position knob)
    {
        const auto& c = col[0];
        juce::Rectangle<float> freezePanel(
            (float)(c.getX() + 4), (float)(c.getY() + 115),
            (float)(c.getWidth() - 8), 180.0f);
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.08f));
        g.fillRoundedRectangle(freezePanel, 4.0f);
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.3f));
        g.drawRoundedRectangle(freezePanel, 4.0f, 1.0f);
        g.setFont(juce::Font(8.0f, juce::Font::bold));
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.7f));
        g.drawText("FREEZE", (int)freezePanel.getX() + 4,
                   (int)freezePanel.getY() + 3,
                   (int)freezePanel.getWidth() - 4, 10,
                   juce::Justification::centredLeft);
    }

    // LFO targets sub-panel in LFO column
    {
        const auto& c = col[2];
        juce::Rectangle<float> targetPanel(
            (float)(c.getX() + 4), (float)(c.getY() + 210),
            (float)(c.getWidth() - 8), 50.0f);
        g.setColour(CustomLookAndFeel::gold().withAlpha(0.06f));
        g.fillRoundedRectangle(targetPanel, 4.0f);
        g.setColour(CustomLookAndFeel::gold().withAlpha(0.25f));
        g.drawRoundedRectangle(targetPanel, 4.0f, 1.0f);
        g.setFont(juce::Font(8.0f, juce::Font::bold));
        g.setColour(CustomLookAndFeel::gold().withAlpha(0.7f));
        g.drawText("TARGETS", (int)targetPanel.getX() + 4,
                   (int)targetPanel.getY() + 3,
                   (int)targetPanel.getWidth() - 4, 10,
                   juce::Justification::centredLeft);
    }

}

//==============================================================================
// Layout — 4-column grid with header
//==============================================================================
void PsycogAudioProcessorEditor::resized()
{
    // Bypass button in header (right side, left of version tag)
    bypassBtn.setBounds(getWidth() - 110, 8, 60, 20);

    // Preset bar at top
    presetBar.setBounds(9, 40, getWidth() - 18, 36);

    // Column geometry — symmetric 9px margins
    const int colY = 98;
    const int colH = getHeight() - colY - 12;
    const int gap = 6;
    const int margin = 9;

    // Column widths: TIME/LFO wider (side-by-side knobs), OUTPUT narrower
    // 218 + 168 + 218 + 140 = 744, + 3 gaps(18) = 762, + 2 margins(18) = 780
    int widths[4] = { 218, 168, 218, 140 };
    int x = margin;
    for (int c = 0; c < 4; ++c)
    {
        col[c] = { x, colY, widths[c], colH };
        x += widths[c] + gap;
    }

    // ---- Section 1: TIME ----
    {
        const auto& c = col[0];
        const int ks = 56;  // knob size
        const int top = c.getY() + 22;

        // Row 1: Stretch and Threshold side by side
        stretch.place(c.getX() + 28, top, ks);
        threshold.place(c.getX() + c.getWidth() - 28 - ks, top, ks);

        // Freeze mode buttons (below knobs, inside sub-panel)
        const int fmY = c.getY() + 130;
        const int btnW = (c.getWidth() - 16) / 3;
        for (int i = 0; i < 3; ++i)
            freezeModeBtns[i].setBounds(c.getX() + 8 + i * btnW, fmY, btnW - 2, 22);

        // Position knob below freeze buttons (centred)
        const int posKs = 50;  // slightly smaller
        position.place(c.getX() + c.getWidth() / 2 - posKs / 2, fmY + 32, posKs);

        // Hint label below position value label (label=13 + knob=posKs + value=12 + gap=2)
        positionHintLabel.setBounds(c.getX() + 4, fmY + 32 + 13 + posKs + 14,
                                    c.getWidth() - 8, 10);
    }

    // ---- Section 2: WAVEFOLDER ----
    {
        const auto& c = col[1];
        const int ks = 56;
        const int cx = c.getX() + c.getWidth() / 2;

        foldAmount.place(cx - ks / 2, c.getY() + 30, ks);
        foldOffset.place(cx - ks / 2, c.getY() + 30 + ks + 32, ks);

        // Stereo offset indicator below the offset knob
        stereoOffsetLabel.setBounds(c.getX() + 4,
                                    c.getY() + 30 + (ks + 25) * 2 + 40,
                                    c.getWidth() - 8, 12);
    }

    // ---- Section 3: LFO ----
    {
        const auto& c = col[2];
        const int ks = 52;
        const int top = c.getY() + 22;

        // LFO waveform buttons (top of column)
        const int wfBtnW = (c.getWidth() - 16) / 4;
        for (int i = 0; i < 4; ++i)
            lfoWaveformBtns[i].setBounds(c.getX() + 8 + i * wfBtnW, top, wfBtnW - 2, 20);

        // Rate and Depth knobs side by side (28px inner padding)
        const int knobY = top + 28;
        lfoRate.place(c.getX() + 28, knobY, ks);
        lfoDepth.place(c.getX() + c.getWidth() - 28 - ks, knobY, ks);

        // LFO target toggles (inside sub-panel)
        const int tgtY = c.getY() + 224;
        const int tgtBtnW = (c.getWidth() - 16) / 4;
        for (int i = 0; i < 4; ++i)
            lfoTargetBtns[i].setBounds(c.getX() + 8 + i * tgtBtnW, tgtY, tgtBtnW - 2, 22);
    }

    // ---- Section 4: OUTPUT ----
    {
        const auto& c = col[3];
        const int ks = 70;  // larger mix knob
        const int cx = c.getX() + c.getWidth() / 2;
        mix.place(cx - ks / 2, c.getY() + 40, ks);

        // "DRY < > WET" label below the mix knob
        dryWetLabel.setBounds(c.getX() + 4, c.getY() + 40 + ks + 30,
                              c.getWidth() - 8, 12);
    }
}
