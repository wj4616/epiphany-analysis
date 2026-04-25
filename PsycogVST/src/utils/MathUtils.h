/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 3: Project Setup - Math utilities
*/

#pragma once

#include <cmath>

namespace MathUtils
{
    /**
     * Triangle fold function: reflects signal at ±1 boundaries.
     * Input can be any value; output is always in [-1, 1].
     * Produces rich harmonics that increase with drive.
     */
    inline float triangleFold(float x)
    {
        // Wrap to period [0, 4)
        x = std::fmod(x + 1.0f, 4.0f);
        if (x < 0.0f) x += 4.0f;

        // Triangle wave in [-1, 1]
        return (x < 2.0f) ? (x - 1.0f) : (3.0f - x);
    }

    /**
     * Apply wavefolding with drive and offset.
     *
     * @param input        Input sample [-1, 1]
     * @param foldAmount   Normalized fold amount [0, 1]
     * @param offset       DC offset for asymmetry [-1, 1]
     * @return             Folded sample in [-1, 1]
     */
    inline float applyWavefold(float input, float foldAmount, float offset)
    {
        // Drive mapping: foldAmount [0, 1] → drive [1.0, 20.0]
        // At foldAmount=0: drive=1.0, no folding (passthrough)
        // At foldAmount=0.2 with ±0.3 pad: drive=4.8, driven=±1.44 → audible fold
        // At foldAmount=1: drive=20.0, aggressive folding
        float drive = 1.0f + foldAmount * 19.0f;

        // Offset scales with foldAmount so foldAmount=0 is true passthrough
        // At foldAmount=0, offset has no effect regardless of its value
        float effectiveOffset = offset * foldAmount;

        // Apply drive and offset before folding
        float folded = triangleFold((input + effectiveOffset) * drive);

        return folded;
    }

    /**
     * Convert decibels to linear gain.
     */
    inline float dbToGain(float db)
    {
        return std::pow(10.0f, db / 20.0f);
    }

    /**
     * Convert linear gain to decibels.
     */
    inline float gainToDb(float gain)
    {
        return 20.0f * std::log10(gain);
    }
}