/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 3: Project Setup - Constants
*/

#pragma once

namespace PsycogConstants
{
    // === Buffer Sizes ===
    constexpr int grainSize = 2048;                    // Hann window size
    constexpr int dryDelaySamples = 2048;              // Dry path delay (matches grain latency)
    constexpr double freezeBufferSeconds = 3.0;        // Each freeze buffer holds 3 seconds

    // === Freeze System ===
    constexpr double freezeCrossfadeMs = 10.0;         // Crossfade for freeze/unfreeze transitions
    constexpr double autoTriggerCooldownMs = 100.0;    // Minimum time between auto-triggers

    // === Auto-Normalize ===
    constexpr double autoNormalizeTargetDb = -6.0;     // Target RMS level
    constexpr double autoNormalizeAttackMs = 300.0;   // Attack time
    constexpr double autoNormalizeReleaseMs = 300.0;   // Release time (was 1000ms — too slow, caused gain spikes)
    constexpr double noiseGateThresholdDb = -60.0;     // Bypass below this
    constexpr double gainMin = 0.1;                    // ~ -20dB max reduction
    constexpr double gainMax = 1.5;                    // ~ +3.5dB max boost (was 3.0 — too aggressive)

    // === Parameter Smoothing ===
    constexpr double smoothingTimeSeconds = 0.02;      // 20ms — JUCE SmoothedValue::reset() expects seconds

    // === CPU Budget ===
    constexpr double cpuBudgetPercent = 15.0;         // Target max CPU

    // === Freeze Mode Enum ===
    enum class FreezeMode
    {
        Off = 0,
        Manual = 1,
        Auto = 2
    };

    // === LFO Waveform Enum ===
    enum class LfoWaveform
    {
        Sine = 0,
        Triangle = 1,
        Square = 2,
        SampleAndHold = 3
    };
}