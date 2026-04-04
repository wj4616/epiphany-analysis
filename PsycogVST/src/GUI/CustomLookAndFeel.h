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
