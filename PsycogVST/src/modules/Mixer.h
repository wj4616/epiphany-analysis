/*
  PsycogVST - Interdimensional sound transformation plugin
  Mixer — Wet/dry blend with equal-power (cosine) crossfade

  Mix = 0%: Dry only (transparent)
  Mix = 100%: Wet only
  Module owns smoothing (20ms ramp).
  EM-14 fix: Uses cos/sin equal-power crossfade.
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class Mixer
{
public:
    Mixer();
    ~Mixer() = default;

    void prepare(double sampleRate, int maxSamples);
    void reset();

    // mix is raw value from APVTS; smoothed internally per-sample
    void process(float* leftOut, float* rightOut,
                 const float* wetLeft, const float* wetRight,
                 const float* dryLeft, const float* dryRight,
                 int numSamples, float mix);

private:
    double sampleRate = 48000.0;
    juce::SmoothedValue<float> mixSmoother;
};
