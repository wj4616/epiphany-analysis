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
