# AudioProcessorValueTreeState (APVTS)

---
id: TECH-002
category: juce-framework
tags: [juce, parameters, state]
---

## Summary

JUCE's parameter management system. Handles parameter layout, thread-safe access, state save/load, and UI binding.

## Basic Setup

```cpp
class PluginProcessor : public AudioProcessor
{
public:
    PluginProcessor()
        : apvts(*this, nullptr, "Parameters", createParameterLayout())
    {}

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID{"gain", 1},  // ID with version hint
            "Gain",                   // Name
            0.0f,                     // Min
            1.0f,                     // Max
            0.5f                      // Default
        ));

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParameterID{"filterType", 1},
            "Filter Type",
            StringArray{"Lowpass", "Highpass", "Bandpass"},
            0
        ));

        return {params.begin(), params.end()};
    }

private:
    AudioProcessorValueTreeState apvts;
};
```

## Thread-Safe Parameter Access

```cpp
void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override
{
    // Get raw atomic pointer (thread-safe)
    float gain = *apvts.getRawParameterValue("gain");

    // Or use the parameter directly
    auto* gainParam = dynamic_cast<AudioParameterFloat*>(apvts.getParameter("gain"));
    float gainValue = gainParam->get();
}
```

## UI Attachments

```cpp
class PluginEditor : public AudioProcessorEditor
{
public:
    PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(p), processor(p)
    {
        // Slider attachment
        gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
            processor.apvts, "gain", gainSlider);
        addAndMakeVisible(gainSlider);
    }

private:
    Slider gainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
};
```

## State Save/Load

```cpp
void getStateInformation(MemoryBlock& destData) override
{
    auto state = apvts.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void setStateInformation(const void* data, int sizeInBytes) override
{
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(ValueTree::fromXml(*xml));
    }
}
```

## Parameter Types

| Type | Use Case |
|------|----------|
| `AudioParameterFloat` | Continuous values (gain, frequency) |
| `AudioParameterInt` | Integer values (octave, voice count) |
| `AudioParameterBool` | Toggle switches |
| `AudioParameterChoice` | Drop-down selections |

## Best Practices

1. Use `ParameterID` with version hint for future compatibility
2. Group related parameters with prefixes: `"filter_cutoff"`, `"filter_resonance"`
3. Always use `apvts.getRawParameterValue()` in audio thread (atomic)
4. Use attachments in GUI (thread-safe binding)

## Failure Modes

| FM | Failure | Prevention |
|----|---------|------------|
| FM-14 | Duplicate IDs | Use unique, descriptive IDs |
| FM-10 | Parameter not connected | Test each parameter affects audio |
| FM-39 | State not saved | Implement both get/setStateInformation |

## Sources

- [JUCE APVTS Documentation](https://docs.juce.com/master/classjuce_1_1AudioProcessorValueTreeState.html)
- [JUCE Tutorial: Saving and Loading State](http://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html)