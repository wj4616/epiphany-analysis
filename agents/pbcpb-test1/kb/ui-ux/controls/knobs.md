# Knob Design

---
id: UI-001
category: controls
tags: [ui, knobs, controls, design]
---

## Summary

Rotary knobs are the primary control in audio plugin UIs. This guide covers sizing, interaction, and JUCE implementation.

## Standard Sizes

| Size | Pixel Range | Use Case |
|------|-------------|----------|
| Large | 80-100px | Primary controls (cutoff, resonance, volume) |
| Medium | 50-70px | Secondary controls (envelope times, LFO depth) |
| Small | 30-45px | Fine controls, modulation amounts |

## Interaction Patterns

| Action | Behavior |
|--------|----------|
| Click + drag vertical | Change value (up = increase) |
| Double-click | Reset to default |
| Shift + drag | Fine control (slower) |
| Right-click | Context menu (if applicable) |
| Mouse wheel | Increment/decrement |

## Visual Design Elements

1. **Track arc** - Shows parameter range (typically 270°)
2. **Indicator** - Current position marker
3. **Label** - Parameter name
4. **Value display** - Numeric readout (optional)

## Color Recommendations

| Element | Dark Theme | Light Theme |
|---------|------------|-------------|
| Background | #1e1e1e | #f0f0f0 |
| Track | #3a3a3a | #c0c0c0 |
| Active | #4a90d9 | #2070c0 |
| Indicator | #ffffff | #1a1a1a |
| Label | #a0a0a0 | #404040 |

## JUCE Implementation

### Basic Slider as Knob

```cpp
knob.setSliderStyle(Slider::RotaryVerticalDrag);
knob.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
knob.setLookAndFeel(&customLookAndFeel);
```

### Custom LookAndFeel

```cpp
void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                      float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                      Slider& slider) override
{
    auto bounds = Rectangle<float>(x, y, width, height);
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centre = bounds.getCentre();
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Track
    Path track;
    track.addArc(centre.getX() - radius, centre.getY() - radius,
                 radius * 2.0f, radius * 2.0f,
                 rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(Colours::grey);
    g.strokePath(track, PathStrokeType(3.0f));

    // Active arc
    Path active;
    active.addArc(centre.getX() - radius, centre.getY() - radius,
                  radius * 2.0f, radius * 2.0f,
                  rotaryStartAngle, angle, true);
    g.setColour(findColour(Slider::rotarySliderFillColourId));
    g.strokePath(active, PathStrokeType(3.0f));

    // Indicator
    auto indicatorLength = radius * 0.8f;
    Path indicator;
    indicator.startNewSubPath(centre);
    indicator.lineTo(centre.getX() + sin(angle) * indicatorLength,
                     centre.getY() - cos(angle) * indicatorLength);
    g.setColour(Colours::white);
    g.strokePath(indicator, PathStrokeType(2.0f));
}
```

## Accessibility

```cpp
knob.setComponentID("filter-cutoff");
knob.setTitle("Filter Cutoff");
knob.setDescription("Controls the filter cutoff frequency");
knob.setWantsKeyboardFocus(true);
```

## Common Mistakes

| Mistake | Fix |
|---------|-----|
| Too small | Minimum 40px for touch |
| No value display | Add textBox or tooltip |
| Inconsistent styling | Use single LookAndFeel |
| Wrong sensitivity | Adjust setSliderSnapsToMousePosition |

## Sources

- [Audio UI Toolkit](https://www.audio-ui.com/)
- JUCE LookAndFeel documentation
- Voger Design blog