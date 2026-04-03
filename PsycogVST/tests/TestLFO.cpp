/*
  PsycogVST Tests - LFO module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/LFO.h"
#include "Parameters.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("LFO Sine shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    // 1 Hz -> full cycle in 48000 samples
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    // Advance past depth smoother ramp (~960 samples at 48kHz, 20ms)
    for (int i = 0; i < 2000; ++i)
        lfo.advance();

    SECTION("Quarter-cycle rises to +1")
    {
        // Reset phase by re-preparing
        lfo.prepare(testSampleRate);
        lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
        lfo.setDepth(1.0f);
        lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
        // Burn through smoother
        for (int i = 0; i < 2000; ++i) lfo.advance();

        // At 1Hz, peak at 12000 samples (quarter cycle)
        float peak = 0.0f;
        for (int i = 0; i < 12000; ++i)
        {
            float v = lfo.advance();
            if (v > peak) peak = v;
        }
        REQUIRE(peak == Catch::Approx(1.0f).margin(0.02f));
    }
}

TEST_CASE("LFO Triangle shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Triangle);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    // Burn through smoother
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Peaks at +depth and -depth")
    {
        float maxVal = -2.0f;
        float minVal = 2.0f;
        for (int i = 0; i < 48000; ++i)
        {
            float v = lfo.advance();
            if (v > maxVal) maxVal = v;
            if (v < minVal) minVal = v;
        }
        REQUIRE(maxVal == Catch::Approx(1.0f).margin(0.02f));
        REQUIRE(minVal == Catch::Approx(-1.0f).margin(0.02f));
    }
}

TEST_CASE("LFO Square shape", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Square);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Values are exactly +depth or -depth")
    {
        bool seenPositive = false, seenNegative = false;
        for (int i = 0; i < 48000; ++i)
        {
            float v = lfo.advance();
            REQUIRE((std::abs(v - 1.0f) < 0.02f || std::abs(v + 1.0f) < 0.02f));
            if (v > 0.0f) seenPositive = true;
            if (v < 0.0f) seenNegative = true;
        }
        REQUIRE(seenPositive);
        REQUIRE(seenNegative);
    }
}

TEST_CASE("LFO S&H behavior", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::SampleAndHold);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    SECTION("Constant between wraps")
    {
        // At 1Hz/48kHz, phase wraps every 48000 samples
        // Within a half cycle, value should be constant
        float firstVal = lfo.advance();
        bool allSame = true;
        for (int i = 1; i < 24000; ++i)
        {
            float v = lfo.advance();
            if (std::abs(v - firstVal) > 0.001f)
            {
                allSame = false;
                break;
            }
        }
        REQUIRE(allSame);
    }

    SECTION("Non-zero initialization")
    {
        LFO lfo2;
        lfo2.prepare(testSampleRate);
        lfo2.setWaveform(PsycogConstants::LfoWaveform::SampleAndHold);
        lfo2.setDepth(1.0f);
        lfo2.setRate(ParamConversions::lfoRateToNormalized(0.01f));
        for (int i = 0; i < 2000; ++i) lfo2.advance();
        // heldValue should be initialized non-zero (from LCG in prepare)
        float v = lfo2.advance();
        // Just verify it doesn't crash and produces a valid float
        REQUIRE(std::isfinite(v));
    }
}

TEST_CASE("LFO depth=0 always zero", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(0.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));

    for (int i = 0; i < 48000; ++i)
    {
        float v = lfo.advance();
        REQUIRE(std::abs(v) < 0.001f);
    }
}

TEST_CASE("LFO depth scaling", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(0.5f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(1.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    float maxVal = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        float v = std::abs(lfo.advance());
        if (v > maxVal) maxVal = v;
    }
    REQUIRE(maxVal == Catch::Approx(0.5f).margin(0.02f));
}

TEST_CASE("LFO rate accuracy", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(10.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Count zero-crossings over 1 second
    int zeroCrossings = 0;
    float prev = lfo.advance();
    for (int i = 1; i < 48000; ++i)
    {
        float curr = lfo.advance();
        if ((prev >= 0.0f && curr < 0.0f) || (prev < 0.0f && curr >= 0.0f))
            zeroCrossings++;
        prev = curr;
    }
    // 10Hz sine = 20 zero-crossings per second (±1 for edge cases)
    REQUIRE(zeroCrossings >= 19);
    REQUIRE(zeroCrossings <= 21);
}

TEST_CASE("LFO phase continuity at block boundary", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(ParamConversions::lfoRateToNormalized(5.0f));
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Process 512 samples, record last value
    float lastVal = 0.0f;
    for (int i = 0; i < 512; ++i)
        lastVal = lfo.advance();

    // Next call should be continuous (no jump)
    float nextVal = lfo.advance();
    float delta = std::abs(nextVal - lastVal);
    // At 5Hz/48kHz, max per-sample delta for sine ≈ 2*pi*5/48000 ≈ 0.00065
    REQUIRE(delta < 0.01f);
}

TEST_CASE("LFO minimum rate", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(0.0f);  // 0.01Hz
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Should produce valid values with no numerical issues
    for (int i = 0; i < 48000; ++i)
    {
        float v = lfo.advance();
        REQUIRE(std::isfinite(v));
    }
}

TEST_CASE("LFO maximum rate", "[LFO]")
{
    LFO lfo;
    lfo.prepare(testSampleRate);
    lfo.setWaveform(PsycogConstants::LfoWaveform::Sine);
    lfo.setDepth(1.0f);
    lfo.setRate(1.0f);  // 20Hz
    for (int i = 0; i < 2000; ++i) lfo.advance();

    // Should oscillate correctly
    float maxVal = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        float v = std::abs(lfo.advance());
        if (v > maxVal) maxVal = v;
    }
    REQUIRE(maxVal == Catch::Approx(1.0f).margin(0.02f));
}
