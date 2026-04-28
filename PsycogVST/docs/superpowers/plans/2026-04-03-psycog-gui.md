# PsycogVST Phase 7: GUI Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a cyberpunk dark-themed GUI for PsycogVST with 14 parameter controls and 20-preset navigation, adapted from EpiphanyMachine's proven UI architecture.

**Architecture:** 4-section column layout (TIME | FOLD | LFO | OUTPUT) with a preset bar header. Reuses EpiphanyMachine's KnobGroup pattern, CustomLookAndFeel, and PresetBar — adapted for PsycogVST's parameter set. Timer-based value label updates at ~15fps.

**Tech Stack:** JUCE 7.0.12 (LookAndFeel_V4, AudioProcessorValueTreeState, SliderAttachment, ButtonAttachment, Timer)

---

## File Structure

| Action | File | Responsibility |
|--------|------|---------------|
| Create | `src/GUI/CustomLookAndFeel.h` | Color palette constants, draw method declarations |
| Create | `src/GUI/CustomLookAndFeel.cpp` | Rotary slider rendering (arc + glow + dot), button/label styling |
| Create | `src/GUI/PresetBar.h` | Preset navigation component (prev/next + name + index) |
| Create | `src/GUI/PresetBar.cpp` | Button handlers, timer animation, layout |
| Rewrite | `src/PluginEditor.h` | KnobGroup struct, all control members, Timer inheritance |
| Rewrite | `src/PluginEditor.cpp` | Constructor setup, layout, paint, value label updates |
| Modify | `CMakeLists.txt` | Add 4 new GUI source files to both plugin and test targets |

---

## Layout Math

```
Window: 780 x 480

Columns:  TIME(218) + gap(6) + FOLD(168) + gap(6) + LFO(218) + gap(6) + OUTPUT(140) = 762
Margins:  left(9) + columns(762) + right(9) = 780   [symmetric]

Vertical: header(98) + columns(370) + bottom(12) = 480

KnobGroup height: label(13) + knob(sz) + value(12) = sz + 25
Side-by-side knobs: inner padding 28px ensures label (knob+40 wide) stays inside column
```

---

### Task 1: CustomLookAndFeel

**Files:**
- Create: `src/GUI/CustomLookAndFeel.h`
- Create: `src/GUI/CustomLookAndFeel.cpp`

- [ ] **Step 0: Create the GUI directory**

```bash
mkdir -p /home/myuser/PsycogVST/src/GUI
```

- [ ] **Step 1: Create the header file**

```cpp
// src/GUI/CustomLookAndFeel.h
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// Cyberpunk dark purple theme with gold accents.
// Subclasses LookAndFeel_V4 and overrides draw methods for rotary sliders,
// buttons, and labels. Static colour methods provide the palette to all GUI code.
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h,
                          float sliderPos, float startAngle, float endAngle,
                          juce::Slider&) override;

    void drawButtonBackground(juce::Graphics&, juce::Button&,
                              const juce::Colour& bg,
                              bool highlighted, bool down) override;

    void drawButtonText(juce::Graphics&, juce::TextButton&,
                        bool highlighted, bool down) override;

    void drawLabel(juce::Graphics&, juce::Label&) override;

    // ---- Colour palette ----
    static juce::Colour bg()            { return juce::Colour(0xff0d0a1a); }
    static juce::Colour surface()       { return juce::Colour(0xff1a1430); }
    static juce::Colour surfaceRaised() { return juce::Colour(0xff241e3d); }
    static juce::Colour neonPrimary()   { return juce::Colour(0xffb44aff); }
    static juce::Colour neonSecondary() { return juce::Colour(0xff7b2ff2); }
    static juce::Colour neonGlow()      { return juce::Colour(0xffd88aff); }
    static juce::Colour gold()          { return juce::Colour(0xfff0c040); }
    static juce::Colour textPrimary()   { return juce::Colour(0xffe8e0f0); }
    static juce::Colour textSecondary() { return juce::Colour(0xff8878a0); }
    static juce::Colour freezeAccent()  { return juce::Colour(0xff40c8f0); }
};
```

- [ ] **Step 2: Create the implementation file**

```cpp
// src/GUI/CustomLookAndFeel.cpp
#include "CustomLookAndFeel.h"
#include <cmath>

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default JUCE colour IDs so any component using this L&F gets our palette
    setColour(juce::Slider::backgroundColourId,          bg());
    setColour(juce::Slider::thumbColourId,               gold());
    setColour(juce::Slider::trackColourId,               neonPrimary());
    setColour(juce::Label::textColourId,                 textPrimary());
    setColour(juce::Label::backgroundColourId,           juce::Colours::transparentBlack);
    setColour(juce::TextButton::buttonColourId,          surfaceRaised());
    setColour(juce::TextButton::buttonOnColourId,        neonPrimary());
    setColour(juce::TextButton::textColourOffId,         textSecondary());
    setColour(juce::TextButton::textColourOnId,          textPrimary());
    setColour(juce::ResizableWindow::backgroundColourId, bg());
}

//==============================================================================
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float startAngle, float endAngle,
    juce::Slider& /*slider*/)
{
    // Centre point and radius of the knob area
    const float cx = (float)x + (float)width  * 0.5f;
    const float cy = (float)y + (float)height * 0.5f;
    const float r  = (float)juce::jmin(width, height) * 0.5f - 4.0f;

    // 1. Background disc (dark raised surface)
    g.setColour(surfaceRaised());
    g.fillEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f);

    // 2. Dim track arc (shows full range in faint neon purple)
    juce::Path track;
    track.addArc(cx - r + 4, cy - r + 4, (r - 4) * 2, (r - 4) * 2,
                 startAngle, endAngle, true);
    g.setColour(neonPrimary().withAlpha(0.25f));
    g.strokePath(track, juce::PathStrokeType(2.0f));

    // 3. Value arc (gold, from start angle to current position)
    const float angle = startAngle + sliderPos * (endAngle - startAngle);
    if (sliderPos > 0.001f)
    {
        juce::Path arc;
        arc.addArc(cx - r + 4, cy - r + 4, (r - 4) * 2, (r - 4) * 2,
                   startAngle, angle, true);
        g.setColour(gold());
        g.strokePath(arc, juce::PathStrokeType(2.5f));
    }

    // 4. Outer glow ring (neon purple, subtle)
    g.setColour(neonPrimary().withAlpha(0.4f));
    g.drawEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f, 1.5f);

    // 5. Indicator dot (gold) at current angle
    const float dotR = 3.0f;
    const float dotX = cx + (r - 9.0f) * std::sin(angle);
    const float dotY = cy - (r - 9.0f) * std::cos(angle);
    g.setColour(gold());
    g.fillEllipse(dotX - dotR, dotY - dotR, dotR * 2, dotR * 2);
}

//==============================================================================
void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& btn,
    const juce::Colour& /*bg*/, bool highlighted, bool /*down*/)
{
    auto b = btn.getLocalBounds().toFloat().reduced(0.5f);

    // Use the button's own buttonOnColourId if toggled (allows per-button accent overrides)
    juce::Colour fill = btn.getToggleState()
        ? btn.findColour(juce::TextButton::buttonOnColourId)
        : surfaceRaised();
    if (highlighted) fill = fill.brighter(0.15f);
    g.setColour(fill);
    g.fillRoundedRectangle(b, 3.0f);

    // Border: bright glow when toggled on, dim when off
    g.setColour(btn.getToggleState() ? neonGlow().withAlpha(0.6f)
                                     : neonPrimary().withAlpha(0.3f));
    g.drawRoundedRectangle(b, 3.0f, 1.0f);
}

void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& btn,
    bool /*highlighted*/, bool /*down*/)
{
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.setColour(btn.getToggleState() ? textPrimary() : textSecondary());
    g.drawText(btn.getButtonText(), btn.getLocalBounds(),
               juce::Justification::centred, false);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& lbl)
{
    g.setFont(lbl.getFont());
    g.setColour(lbl.findColour(juce::Label::textColourId));
    g.drawText(lbl.getText(), lbl.getLocalBounds().reduced(2, 0),
               lbl.getJustificationType(), true);
}
```

- [ ] **Step 3: Verify files exist**

Run: `ls /home/myuser/PsycogVST/src/GUI/CustomLookAndFeel.h /home/myuser/PsycogVST/src/GUI/CustomLookAndFeel.cpp`
Expected: Both files listed.

- [ ] **Step 4: Commit**

```bash
cd /home/myuser/PsycogVST
git add src/GUI/CustomLookAndFeel.h src/GUI/CustomLookAndFeel.cpp
git commit -m "feat(gui): add CustomLookAndFeel with cyberpunk dark theme"
```

---

### Task 2: PresetBar

**Files:**
- Create: `src/GUI/PresetBar.h`
- Create: `src/GUI/PresetBar.cpp`

- [ ] **Step 1: Create the header file**

```cpp
// src/GUI/PresetBar.h
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
```

- [ ] **Step 2: Create the implementation file**

```cpp
// src/GUI/PresetBar.cpp
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
```

- [ ] **Step 3: Commit**

```bash
cd /home/myuser/PsycogVST
git add src/GUI/PresetBar.h src/GUI/PresetBar.cpp
git commit -m "feat(gui): add PresetBar with prev/next navigation and index display"
```

---

### Task 3: Rewrite PluginEditor Header

**Files:**
- Rewrite: `src/PluginEditor.h`

- [ ] **Step 1: Rewrite the header with all GUI members**

```cpp
// src/PluginEditor.h
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
    KnobGroup stretch, position, threshold;
    juce::TextButton freezeModeBtns[3];   // Off, Manual, Auto

    // ---- Section 2: WAVEFOLDER ----
    KnobGroup foldAmount, foldOffset;
    juce::Label stereoOffsetLabel;        // "L+offset  R-offset"

    // ---- Section 3: LFO ----
    KnobGroup lfoRate, lfoDepth;
    juce::TextButton lfoWaveformBtns[4];  // Sine, Tri, Sq, S&H
    juce::TextButton lfoTargetBtns[4];    // Stretch, Position, FoldAmt, FoldOff
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoTargetAttach[4];

    // ---- Section 4: OUTPUT ----
    KnobGroup mix;
    juce::Label dryWetLabel;              // "DRY  < >  WET"

    // Column bounds computed in resized(), cached for paint()
    juce::Rectangle<int> col[4];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PsycogAudioProcessorEditor)
};
```

- [ ] **Step 2: Commit**

```bash
cd /home/myuser/PsycogVST
git add src/PluginEditor.h
git commit -m "feat(gui): rewrite PluginEditor header with KnobGroup pattern and 4-section layout"
```

---

### Task 4: Rewrite PluginEditor Implementation

**Files:**
- Rewrite: `src/PluginEditor.cpp`

- [ ] **Step 1: Write the full editor implementation**

```cpp
// src/PluginEditor.cpp
/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 7: GUI Implementation
*/

#include "PluginEditor.h"
#include "Parameters.h"
#include "PresetManager.h"

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

    // ---- Preset bar ----
    presetBar.setLookAndFeel(&laf);
    addAndMakeVisible(presetBar);

    // ---- Section 1: TIME ----
    stretch.setup(ParamIDs::stretch,     "STRETCH",   p.apvts, laf, this);
    position.setup(ParamIDs::position,   "POSITION",  p.apvts, laf, this);
    threshold.setup(ParamIDs::threshold, "THRESHOLD", p.apvts, laf, this);

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

    // Version tag
    g.setFont(juce::Font(9.0f));
    g.setColour(CustomLookAndFeel::gold().withAlpha(0.5f));
    g.drawText("v1.0", getWidth() - 40, 8, 32, 14, juce::Justification::centredRight);

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

    // Freeze mode sub-panel in TIME column
    {
        const auto& c = col[0];
        juce::Rectangle<float> freezePanel(
            (float)(c.getX() + 4), (float)(c.getY() + 240),
            (float)(c.getWidth() - 8), 60.0f);
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.08f));
        g.fillRoundedRectangle(freezePanel, 4.0f);
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.3f));
        g.drawRoundedRectangle(freezePanel, 4.0f, 1.0f);
        g.setFont(juce::Font(8.0f, juce::Font::bold));
        g.setColour(CustomLookAndFeel::freezeAccent().withAlpha(0.7f));
        g.drawText("FREEZE MODE", (int)freezePanel.getX() + 4,
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

        // Row 1: Stretch and Position side by side (28px inner padding)
        stretch.place(c.getX() + 28, top, ks);
        position.place(c.getX() + c.getWidth() - 28 - ks, top, ks);

        // Row 2: Threshold (centred)
        threshold.place(c.getX() + c.getWidth() / 2 - ks / 2, top + ks + 30, ks);

        // Freeze mode buttons (below threshold, inside sub-panel)
        const int fmY = c.getY() + 255;
        const int btnW = (c.getWidth() - 16) / 3;
        for (int i = 0; i < 3; ++i)
            freezeModeBtns[i].setBounds(c.getX() + 8 + i * btnW, fmY, btnW - 2, 22);
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
```

- [ ] **Step 2: Commit**

```bash
cd /home/myuser/PsycogVST
git add src/PluginEditor.cpp
git commit -m "feat(gui): rewrite PluginEditor with 4-section layout, knobs, and button groups"
```

---

### Task 5: Update CMakeLists.txt

**Files:**
- Modify: `CMakeLists.txt`

- [ ] **Step 1: Add GUI source files to the plugin target**

In `CMakeLists.txt`, the `target_sources(PsycogVST PRIVATE` block becomes:

```cmake
target_sources(PsycogVST PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/PresetManager.cpp
    src/GUI/CustomLookAndFeel.cpp
    src/GUI/PresetBar.cpp
    ${MODULE_SOURCES}
)
```

And the test executable also needs the GUI files (it compiles PluginEditor.cpp which includes them):

```cmake
add_executable(PsycogVSTTests
    tests/TestTimeStretch.cpp
    tests/TestWavefolder.cpp
    tests/TestLFO.cpp
    tests/TestFreezeBuffer.cpp
    tests/TestParameters.cpp
    tests/TestPresets.cpp
    tests/TestThresholdDetector.cpp
    tests/TestWetProcessor.cpp
    tests/TestDryDelay.cpp
    tests/TestMixer.cpp
    tests/TestOutputProtection.cpp
    tests/TestIntegration.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/PresetManager.cpp
    src/GUI/CustomLookAndFeel.cpp
    src/GUI/PresetBar.cpp
    ${TEST_MODULE_SOURCES}
)
```

- [ ] **Step 2: Commit**

```bash
cd /home/myuser/PsycogVST
git add CMakeLists.txt
git commit -m "build: add GUI source files to plugin and test targets"
```

---

### Task 6: Build and Fix Compilation Errors

**Files:**
- May touch any of the files from Tasks 1-5

- [ ] **Step 1: Run a full build**

```bash
cd /home/myuser/PsycogVST
cmake -B build -DCMAKE_BUILD_TYPE=Release 2>&1 | tail -20
cmake --build build 2>&1 | tail -60
```

Expected: Clean build with 0 errors. If there are errors, fix them.

- [ ] **Step 2: Run all tests**

```bash
cd /home/myuser/PsycogVST/build && ctest --output-on-failure 2>&1 | tail -30
```

Expected: All 87 existing tests pass. The GUI code doesn't add new tests (it's visual), but must not break existing tests.

- [ ] **Step 3: Verify VST3 was installed**

```bash
ls -la ~/.vst3/PsycogVST.vst3/ 2>/dev/null || ls -la ~/.vst3/ | grep -i psycog
```

Expected: Plugin bundle exists at `~/.vst3/PsycogVST.vst3/`

- [ ] **Step 4: Commit any fixes**

```bash
cd /home/myuser/PsycogVST
git add -A src/GUI/ src/PluginEditor.h src/PluginEditor.cpp CMakeLists.txt
git commit -m "fix(gui): resolve compilation issues from Phase 7 build"
```

(Skip this step if no fixes were needed.)

---

### Task 7: Visual Verification and Polish

**Files:**
- May adjust: `src/PluginEditor.cpp` (layout tweaks)
- May adjust: `src/GUI/CustomLookAndFeel.cpp` (rendering tweaks)

- [ ] **Step 1: Launch REAPER and load the plugin**

This is a manual step. The engineer should:
1. Open REAPER
2. Add a track, insert PsycogVST on it
3. Verify the GUI opens with 4 columns, all knobs visible
4. Verify preset navigation works (prev/next buttons cycle through 20 presets, index updates)
5. Verify all knobs respond to mouse drag and show updating value labels
6. Verify freeze mode buttons toggle correctly with cyan highlight
7. Verify LFO waveform buttons toggle correctly with purple highlight
8. Verify LFO target buttons toggle on/off independently with gold highlight
9. Verify no label text is clipped or overflows column panels
10. Verify "DRY < > WET" label visible below mix knob
11. Verify "L +offset R -offset" label visible below fold offset knob

- [ ] **Step 2: Adjust any layout/rendering issues found**

If knobs overlap, labels are cut off, or buttons don't align, fix the constants in `resized()` or `paint()`. Typical adjustments:
- Column widths in the `widths[4]` array
- Inner padding (28px) for side-by-side knobs
- Button strip Y positions
- Sub-panel Y/height values in paint()

- [ ] **Step 3: Final commit**

```bash
cd /home/myuser/PsycogVST
git add src/PluginEditor.cpp src/GUI/CustomLookAndFeel.cpp
git commit -m "polish(gui): adjust layout after visual verification in REAPER"
```

(Skip if no changes needed.)

---

## Reference: Parameter → Control Mapping

| Parameter ID | Control Type | Section | Display Format |
|-------------|-------------|---------|---------------|
| `stretch` | KnobGroup | TIME | `1.00x` (log scale) |
| `position` | KnobGroup | TIME | `50%` |
| `freezeMode` | 3 TextButtons (cyan) | TIME | Toggle highlight |
| `threshold` | KnobGroup | TIME | `0.30` |
| `foldAmount` | KnobGroup | WAVEFOLD | `50%` |
| `foldOffset` | KnobGroup | WAVEFOLD | `+0.25` |
| `lfoRate` | KnobGroup | LFO | `0.50Hz` (log scale) |
| `lfoWaveform` | 4 TextButtons (purple) | LFO | Toggle highlight |
| `lfoDepth` | KnobGroup | LFO | `75%` |
| `lfoTargetStretch` | Toggle button (gold) | LFO | On/off highlight |
| `lfoTargetPosition` | Toggle button (gold) | LFO | On/off highlight |
| `lfoTargetFoldAmount` | Toggle button (gold) | LFO | On/off highlight |
| `lfoTargetFoldOffset` | Toggle button (gold) | LFO | On/off highlight |
| `mix` | KnobGroup | OUTPUT | `100%` |

## Layout Math Verification

```
Window: 780 x 480
Margins: 9px left + 9px right = 18px
Columns: 218 + 168 + 218 + 140 = 744px
Gaps: 3 x 6 = 18px
Total: 18 + 744 + 18 = 780  [exact fit, symmetric]

Side-by-side knob labels (worst case, TIME column, ks=56):
  labelW = 56 + 40 = 96
  Left knob at x = col.x + 28:  labelX = col.x + 28 + 28 - 48 = col.x + 8   [inside]
  Right knob at x = col.x + 218 - 28 - 56 = col.x + 134:
    labelX = col.x + 134 + 28 - 48 = col.x + 114, right = col.x + 210  [inside 218]

LFO column side-by-side (ks=52):
  labelW = 52 + 40 = 92
  Left knob at x = col.x + 28:  labelX = col.x + 28 + 26 - 46 = col.x + 8   [inside]
  Right knob at x = col.x + 218 - 28 - 52 = col.x + 138:
    labelX = col.x + 138 + 26 - 46 = col.x + 118, right = col.x + 210  [inside 218]
```
