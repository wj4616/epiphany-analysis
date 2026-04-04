---
name: juce-ui-bridge
description: Use for UI/GUI questions when designing or implementing JUCE plugin interfaces. Translates visual concepts to JUCE LookAndFeel implementations and provides design guidance from the UI Knowledge Base. Integrates with Phase 0 (juce-plugin-spec) and Phase 8 (juce-daw-testing).
---

# JUCE UI Bridge

**Purpose:** Bridge between UI/UX concepts and JUCE GUI implementation. Translates visual design language to concrete JUCE code patterns while drawing from the UI Knowledge Base.

**Playbook Reference:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json` - `ui_design` section

## When to Use

Invoke this skill when:
- User asks about plugin GUI design ("modern clean interface", "vintage look")
- User asks about component implementation ("how do I make custom knobs?", "meter visualization")
- User asks about layout patterns ("best layout for synthesizer", "compact effect UI")
- User asks about colors, fonts, or visual hierarchy
- User asks about accessibility in JUCE plugins
- User asks about LookAndFeel customization
- User provides UI feedback during DAW testing ("knobs are too small", "can't read labels")
- Phase 0 (juce-plugin-spec) requires UI design decisions
- Phase 8 (juce-daw-testing) identifies UI issues

## Algorithm

```
1. Parse the query for UI concepts and visual descriptors
2. Load UI Knowledge Base from playbook (ui_design section)
3. Load Capability Schema if plugin context exists
4. Translate concepts to JUCE implementations:
   - UI concepts → LookAndFeel methods
   - Visual descriptors → color schemes, typography
   - Layout patterns → JUCE layout systems
5. Verify against JUCE 7 API capabilities
6. Generate response with progressive disclosure:
   - Quick answer for immediate implementation
   - Detailed explanation if requested
   - Educational context if time permits
```

## Progressive Disclosure

### Quick Answer (default)
Direct implementation guidance with specific JUCE classes and code patterns.

Example:
> "For a modern clean look, use `LookAndFeel_V4` with the Dark color scheme. Override `drawRotarySlider()` for custom knobs. Use 48-64px for primary controls."

### Detailed Answer (on request)
Full explanation with design rationale, alternatives, and code examples.

Example:
> "Modern clean interfaces typically use `LookAndFeel_V4` as a base. Here's why:
> - Built-in dark/light variants
> - Clean, minimal aesthetic
> - Easy to customize by overriding specific methods
>
> Custom knobs require overriding `drawRotarySlider()`. Here's a complete example:
> ```cpp
> void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
>                      float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
>                      Slider& slider) override {
>     // Draw knob body
>     auto bounds = Rectangle<int>(x, y, width, height).toFloat();
>     g.setColour(findColour(Slider::rotarySliderFillColourId));
>     g.fillEllipse(bounds);
>
>     // Draw value arc
>     auto radius = bounds.getWidth() / 2;
>     auto lineW = radius * 0.1f;
>     auto arcRadius = radius - lineW / 2;
>     Path p;
>     p.addArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius,
>              rotaryStartAngle, rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle), true);
>     g.strokePath(p, PathStrokeType(lineW));
> }
> ```
>
> Size recommendations: Primary controls 48-64px, secondary 32-40px, tertiary 24-28px."

### Educational Layer (on request)
Design theory and best practices context.

Example:
> "Control sizing follows Fitts's Law - larger targets are easier to hit. Primary controls like main volume or filter cutoff should be largest (48-64px) because they're used most frequently. This is why hardware mixers put the main fader large and prominent.
>
> Visual hierarchy guides the user's eye through the interface. The brain processes size, color, and position in milliseconds. Use these to lead users to important controls first:
> 1. Size: Largest controls are processed first
> 2. Position: Top-left in Western reading order
> 3. Color: Accent colors draw attention
> 4. Grouping: Related controls form visual units"

## Translation Tables

### Visual Concepts to JUCE

| Visual Concept | JUCE Implementation |
|----------------|---------------------|
| Modern clean | `LookAndFeel_V4` Dark or Grey |
| Vintage hardware | Custom `LookAndFeel` with textures, gradients |
| Minimal | Flat design, `LookAndFeel_V4` Grey |
| Glassmorphism | `Graphics::fillAll()` with alpha, blur overlay |
| Neumorphism | Multiple shadows with `DropShadow` |
| Skeuomorphic | Custom drawing with `Graphics::drawImage()` |
| Rotary knob | `Slider::RotaryHorizontalVerticalDrag` + custom `drawRotarySlider()` |
| Linear fader | `Slider::LinearVertical` + custom `drawLinearSlider()` |
| Meter | Custom `Component` with `Timer` callback |
| Tabbed interface | `TabbedComponent` or custom `TabbedButtonBar` |
| Resizable window | `ComponentBoundsConstrainer` + `setResizable()` |

### UI Feedback to Parameter Adjustments

| UI Feedback | Issue | Solution |
|-------------|-------|----------|
| "Can't read labels" | Low contrast | Increase text contrast, use larger font |
| "Controls feel tiny" | Small touch targets | Minimum 32px, primary 48-64px |
| "Interface is cramped" | Overcrowding | Use tabs, collapsible panels, larger window |
| "Don't know what this does" | Unclear labeling | Add tooltips, clearer labels |
| "Hard to see values" | No value display | Add numeric readout near control |
| "Colors are distracting" | Color overload | Reduce palette, use semantic colors |
| "Window is too small" | Fixed size | Make resizable with minimum constraint |

### Color Psychology for Plugins

| Color | Associations | Use For |
|-------|--------------|---------|
| Blue | Trust, stability, professional | Primary actions, selections |
| Green | Success, active, safe | Active states, enabled features |
| Yellow/Orange | Warning, attention, energy | Caution levels, attention points |
| Red | Danger, error, stop | Errors, clipping, destructive actions |
| Purple | Creativity, premium, unique | Brand identity, special features |

## Integration Points

### With juce-plugin-spec (Phase 0)

During plugin specification, this skill provides:

1. **UI style recommendations** based on plugin type
   ```
   Plugin type: Synthesizer with complex modulation
   Recommended: Keyboard style layout with tabbed interface
   Color: Dark theme with accent color for mod routing
   ```

2. **Dimension recommendations** based on control count
   ```
   Parameter count: 47 controls
   Recommended: Resizable window, minimum 800x500
   Consider: Tabbed interface for grouping
   ```

3. **LookAndFeel architecture** decisions
   ```
   Visual identity: Vintage analog synthesizer
   Implementation: Custom LookAndFeel inheriting LookAndFeel_V4
   Key overrides: drawRotarySlider, drawButtonBackground
   ```

### With juce-dsp-implementation (Phase 4)

For each DSP module, provide UI implementation guidance:

```cpp
// After filter module implementation, UI bridge suggests:
// Filter controls UI implementation:
// - Rotary knobs for cutoff (primary), resonance (secondary)
// - Radio buttons for filter type
// - Visual filter response display (optional)
// - Cutoff range: logarithmic skew for perceptual linearity
```

### With juce-daw-testing (Phase 8)

During DAW testing, provide UI quality criteria checklist:

```
UI Quality Criteria:
- [ ] All controls visible at minimum window size
- [ ] Text readable (WCAG AA contrast minimum)
- [ ] Control sizes appropriate (32px minimum, 48px+ primary)
- [ ] Visual hierarchy clear (primary controls prominent)
- [ ] Consistent spacing (8px grid)
- [ ] Tooltips present for all controls
- [ ] Keyboard navigation works
- [ ] Window resizable
- [ ] Scaling works at 1x, 1.5x, 2x
- [ ] Accessibility handler implemented
```

## Common Patterns

### Custom LookAndFeel Setup

```cpp
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
    CustomLookAndFeel() {
        // Set color scheme
        setColour(Slider::rotarySliderFillColourId, juce::Colour(0xff3b82f6));
        setColour(Slider::rotarySliderOutlineColourId, juce::Colour(0xff1a1a1a));
        setColour(Slider::thumbColourId, juce::Colour(0xffffffff));

        // Customize default font
        juce::Font font("Inter", 12.0f, juce::Font::plain);
        setDefaultSansSerifTypeface(font.getTypefacePtr());
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override {
        // Custom drawing implementation
        auto bounds = juce::Rectangle<float>(x, y, width, height);
        auto radius = bounds.getWidth() / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Knob body
        g.setColour(findColour(juce::Slider::rotarySliderOutlineColourId));
        g.fillEllipse(bounds);

        // Knob highlight
        g.setColour(findColour(juce::Slider::rotarySliderFillColourId));
        g.drawEllipse(bounds.reduced(2), 2.0f);

        // Value arc
        juce::Path p;
        p.addArc(bounds.getCentreX(), bounds.getCentreY(), radius - 4, radius - 4,
                 rotaryStartAngle, toAngle, true);
        g.strokePath(p, juce::PathStrokeType(3.0f));

        // Indicator line
        juce::Point<float> tip(bounds.getCentreX() + std::sin(toAngle) * (radius - 8),
                               bounds.getCentreY() - std::cos(toAngle) * (radius - 8));
        g.setColour(juce::Colours::white);
        g.drawLine(bounds.getCentreX(), bounds.getCentreY(), tip.x, tip.y, 2.0f);
    }
};
```

### Resizable Window with Constraints

```cpp
class PluginEditor : public juce::AudioProcessorEditor {
public:
    PluginEditor(AudioProcessor& p) : AudioProcessorEditor(p) {
        // Set up constrainer
        constrainer.setMinimumWidth(400);
        constrainer.setMinimumHeight(300);
        constrainer.setMaximumWidth(1600);
        constrainer.setMaximumHeight(1200);

        setConstrainer(&constrainer);
        setResizable(true, true);  // true = allow resizing, true = show corner resizer

        // Set initial size
        setSize(800, 600);
    }

    void resized() override {
        auto bounds = getLocalBounds();

        // Scale-aware layout
        auto scale = juce::Component::getApproximateScaleFactorForComponent(this);

        // Use FlexBox for responsive layout
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.items.add(juce::FlexItem(headerSection).withFlex(0.1));
        fb.items.add(juce::FlexItem(mainSection).withFlex(0.8));
        fb.items.add(juce::FlexItem(footerSection).withFlex(0.1));
        fb.performLayout(bounds);
    }

private:
    juce::ComponentBoundsConstrainer constrainer;
};
```

### Level Meter Component

```cpp
class LevelMeter : public juce::Component, private juce::Timer {
public:
    LevelMeter() {
        startTimerHz(30);  // 30fps refresh
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();

        // Background
        g.setColour(juce::Colour(0xff1a1a1a));
        g.fillRoundedRectangle(bounds, 4.0f);

        // Meter level
        auto meterHeight = bounds.getHeight() * level;
        auto meterBounds = bounds.removeFromBottom(meterHeight);

        // Gradient from green to yellow to red
        juce::ColourGradient gradient(
            juce::Colour(0xff00ff00),  // Green (bottom)
            bounds.getX(), bounds.getBottom(),
            juce::Colour(0xffff0000),  // Red (top)
            bounds.getX(), bounds.getY(),
            false
        );
        gradient.addColour(0.6, juce::Colour(0xffffff00));  // Yellow at 60%

        g.setGradientFill(gradient);
        g.fillRoundedRectangle(meterBounds.reduced(2), 2.0f);

        // Peak hold
        if (peakHold > 0.0f) {
            auto peakY = bounds.getY() + bounds.getHeight() * (1.0f - peakHold);
            g.setColour(juce::Colours::white);
            g.drawHorizontalLine(static_cast<int>(peakY), bounds.getX() + 2, bounds.getRight() - 2);
        }
    }

    void timerCallback() override {
        // Get level from audio processor
        // level = processor->getCurrentLevel();
        repaint();
    }

    void setLevel(float newLevel) {
        level = newLevel;
        if (newLevel > peakHold) {
            peakHold = newLevel;
            peakHoldTime = 0;
        } else {
            peakHoldTime++;
            if (peakHoldTime > 60) {  // 2 seconds at 30fps
                peakHold *= 0.95f;    // Decay
            }
        }
    }

private:
    float level = 0.0f;
    float peakHold = 0.0f;
    int peakHoldTime = 0;
};
```

## Accessibility Implementation

```cpp
class AccessibleKnob : public juce::Slider {
public:
    AccessibleKnob(const juce::String& name, const juce::String& description) {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setTextValueSuffix(" " + description);
        setTooltip(description);

        // Set up accessibility
        setTitle(name);
        setDescription(description);
        setAccessible(true);
    }

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<juce::AccessibilityHandler>(*this, juce::AccessibilityRole::slider,
            juce::AccessibilityActions()
                .addAction(juce::AccessibilityAction::showMenu, [this] { showContextMenu(); })
                .addAction(juce::AccessibilityAction::press, [this] { startDrag(); }),
            { std::make_unique<SliderValueInterface>(*this) });
    }

private:
    struct SliderValueInterface : public juce::AccessibilityValueInterface {
        explicit SliderValueInterface(AccessibleKnob& s) : slider(s) {}

        juce::String getCurrentValueAsString() const override {
            return slider.getTextFromValue(slider.getValue());
        }

        double getCurrentValue() const override { return slider.getValue(); }
        void setValue(double newValue) override { slider.setValue(newValue); }
        juce::String getValueAsString() const override { return slider.getTextFromValue(slider.getValue()); }
        AccessibleValueRange getRange() const override {
            return { slider.getMinimum(), slider.getMaximum(), slider.getInterval() };
        }

        AccessibleKnob& slider;
    };
};
```

## Key Principles

1. **Visual hierarchy drives usability** - Primary controls should be largest and most prominent
2. **Consistency reduces cognitive load** - Same control types should look and behave identically
3. **Spacing creates meaning** - Use consistent grid (8px) for alignment and grouping
4. **Accessibility is not optional** - All controls must be keyboard-accessible and screen-reader compatible
5. **Performance matters** - Animations and meters should not impact audio performance
6. **Test at scale** - Verify UI at 1x, 1.5x, 2x scale factors
7. **Dark theme is standard** - Most DAWs use dark themes; match the environment

## Anti-Patterns to Avoid

| Anti-Pattern | Issue | Fix |
|--------------|-------|-----|
| Tiny controls | Hard to click accurately | Minimum 32px, primary 48-64px |
| Inconsistent styling | Confuses users | Use single LookAndFeel |
| Poor contrast | Unreadable text | WCAG AA minimum (4.5:1) |
| Overcrowded layout | Overwhelming | Use tabs, collapsible panels |
| No visual feedback | Users don't know actions worked | Hover states, active indicators |
| Fixed window size | Can't adapt to screens | Make resizable |
| Jumpy parameters | Audible clicks | Use SmoothedValue |
| Ignoring accessibility | Unusable for some users | Implement AccessibilityHandler |

## Key Files

- **KB Registry:** `~/.claude/kb-registry.json` — resolves KB locations (optional — skill works without it)
- **Playbook:** `~/.agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json` — ui_design section
- **UI Validation Logs:** `~/.agents/juce-agent/ui-validation-logs/<project-name>/`
- **UI Global Patterns:** `~/.agents/juce-agent/ui-validation-logs/global-patterns.json`
- **Validation Template:** `~/.agents/juce-agent/ui-validation-logs/VALIDATION_TEMPLATE.md`

## Validation Logging

After UI evaluation during DAW testing:

1. **Log results** to `ui-validation-logs/<project-name>/<date>-ui-evaluation.md`
2. **Document measurements:**
   - Control sizes vs recommendations
   - Font sizes vs recommendations
   - Spacing consistency
   - Contrast ratios
3. **Update global patterns** in `ui-validation-logs/global-patterns.json`:
   - Track control usability metrics
   - Track visual hierarchy issues
   - Track accessibility problems
4. **Mark translations as verified** when user confirms UI is correct

## Related Skills

- **juce-plugin-spec**: Use during Phase 0 to define UI style and layout
- **juce-dsp-implementation**: Coordinate UI controls with DSP parameters
- **juce-daw-testing**: Verify UI quality during DAW testing phase

## KB Dependency

This skill primarily reads from the playbook's `ui_design` section. When a registered KB has a `ui` or `ui-ux` layer, the skill can also read UI knowledge entries for richer guidance.

### Registry Resolution

1. Read `~/.claude/kb-registry.json`
2. Find a registered KB with a `ui` or `ui-ux` layer (or `ui-kb` for prototype)
3. If found: read entries from that layer for design patterns, component examples, color schemes
4. If not found: use playbook `ui_design` section only (current behavior)

### Confidence Awareness

When reading KB entries with `harvest_metadata`:

| Confidence | Action |
|------------|--------|
| >= 0.60 | Use UI guidance normally |
| 0.40 - 0.59 | Use guidance but note: "Low confidence UI reference — verify recommendations" |
| < 0.40 | Skip KB entry, use playbook fallback |

### Placeholder Detection

If a UI KB entry has status "placeholder":
```
kb-harvest --kb <kb_name> --auto --entry <entry-id> --batch 1
```

## Fallback and Error Handling

### UI Knowledge Base Unavailable

**Symptom:** Cannot load playbook or UI section

**Fallback:**
```markdown
The UI Design Knowledge Base is temporarily unavailable.

**Basic guidance:**
- Primary controls: 48-64px minimum
- Secondary controls: 32-40px
- Minimum contrast: 4.5:1 (WCAG AA)
- Use LookAndFeel_V4 as base
- Make windows resizable with minimum constraints

Please restart the agent or check playbook availability.
```

### LookAndFeel Implementation Fails

**Symptom:** Custom LookAndFeel doesn't compile or renders incorrectly

**Fallback:**
```cpp
// Fallback to JUCE built-in LookAndFeel_V4
// This provides clean, modern appearance while debugging custom implementation

auto* lookAndFeel = &juce::LookAndFeel::getDefaultLookAndFeel();
// Or use LookAndFeel_V4 directly:
// setLookAndFeel(new juce::LookAndFeel_V4(juce::LookAndFeel_V4::getDarkColourScheme()));
```

**Resolution steps:**
1. Check base class inheritance (must inherit from juce::LookAndFeel_V4 or similar)
2. Verify method signatures match JUCE 7 API
3. Check color ID names (changed in JUCE 7)
4. Ensure all used methods are public/protected

### Control Size Issues

**Symptom:** Controls appear too small or too large

**Fallback:**
```cpp
// Emergency sizing reference
// Use these as baseline while debugging:

int getPrimaryControlSize() { return 56; }  // Primary controls
int getSecondaryControlSize() { return 36; } // Secondary controls
int getMinimumTouchTarget() { return 32; }  // Absolute minimum

// For rotary sliders:
// Knob body: 48-64px for primary
// Knob body: 32-40px for secondary
```

### Resizable Window Issues

**Symptom:** Window doesn't resize or content jumps

**Fallback:**
```cpp
// Use FlexBox for automatic layout
// Simple fallback layout:

void resized() override {
    auto bounds = getLocalBounds();

    // Header
    headerSection.setBounds(bounds.removeFromTop(50));

    // Main content
    mainContent.setBounds(bounds.reduced(10));

    // Footer
    footerSection.setBounds(bounds.removeFromBottom(30));
}

// For more complex layouts, use juce::FlexBox
```

### Accessibility Implementation Issues

**Symptom:** Screen reader doesn't announce controls

**Fallback:**
```cpp
// Basic accessibility fallback
// Ensure these are set for every control:

slider.setAccessible(true);
slider.setTitle("Control Name");
slider.setDescription("Control description for screen readers");

// For value display:
slider.setTextValueSuffix(" unit");
slider.onValueChange = [this] {
    // Update accessibility value
};
```

### Font Rendering Issues

**Symptom:** Custom fonts don't display or look wrong

**Fallback:**
```cpp
// Use JUCE's default font as fallback
// System fonts always available:

// Option 1: System default
setFont(12.0f); // Uses default sans-serif

// Option 2: Common system font
juce::Font font("Inter", 12.0f, juce::Font::plain);
// Or "Arial", "Helvetica" on macOS

// Check font availability:
auto typeface = juce::Typeface::createSystemTypefaceFor("Inter");
if (typeface == nullptr) {
    // Fall back to default
}
```

### Performance Issues During Paint

**Symptom:** UI lagging, high CPU on paint

**Fallback:**
```cpp
// Reduce paint complexity
// 1. Use cached images for static elements
// 2. Only repaint changed areas:

void paint(juce::Graphics& g) override {
    // Cache background
    if (backgroundNeedsRepaint) {
        cachedBackground = getBackgroundImage();
        backgroundNeedsRepaint = false;
    }
    g.drawImageAt(cachedBackground, 0, 0);

    // Only paint dynamic elements
    paintKnobs(g);
    paintMeters(g); // Consider throttling meter repaints
}

// For meters, use timer instead of every frame
void timerCallback() override {
    if (meterChanged) {
        repaint(meterBounds); // Only repaint meter area
    }
}
```

### UI Validation Log Write Failure

**Symptom:** Cannot write to UI validation logs

**Fallback:**
```markdown
**Warning:** Cannot log UI validation results.

The UI guidance will still be provided, but results won't be saved.
Please check directory permissions: `~/.agents/juce-agent/ui-validation-logs/`

Continue with UI design guidance. Results can be logged manually later.
```