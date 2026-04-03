/*
  PsycogVST - Interdimensional sound transformation plugin
  ThresholdDetector — Per-sample sliding window RMS with cooldown

  Uses 10ms EMA for smooth amplitude tracking.
  Integrated 100ms cooldown prevents rapid re-triggering.
  Receives amplitude from Wavefolder output, BEFORE WetProcessor.
*/

#pragma once

#include <cmath>
#include "../utils/Constants.h"

class ThresholdDetector
{
public:
    ThresholdDetector();
    ~ThresholdDetector() = default;

    void prepare(double sampleRate);
    void reset();

    // Per-sample: returns true if freeze should trigger
    // ampL/ampR are wavefolder output samples (not absolute values — we take abs internally)
    bool processSample(float ampL, float ampR, float threshold);

private:
    double sampleRate = 48000.0;
    float rmsPower = 0.0f;        // EMA of squared amplitude
    float alpha = 0.0f;           // EMA coefficient (10ms time constant)
    int cooldownRemaining = 0;    // Remaining cooldown samples
    int cooldownLength = 0;       // 100ms in samples
};
