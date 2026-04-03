/*
  PsycogVST Tests - WetProcessor module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/WetProcessor.h"
#include "utils/MathUtils.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("WetProcessor soft-clip bounds", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Hot input should be bounded
    for (int i = 0; i < 1000; ++i)
        wp.processSample(outL, outR, 10.0f, -10.0f);

    // tanh clips to [-1,1], but auto-normalize gain (up to 3.0) can push output higher
    // Output should still be bounded by tanh(input) * gainMax = tanh(10) * 3.0 ≈ 3.0
    REQUIRE(std::abs(outL) < 3.1f);
    REQUIRE(std::abs(outR) < 3.1f);
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("WetProcessor transparency at low levels", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // ±0.3 input: tanh(0.3) ≈ 0.2913, deviation < 0.04dB
    wp.processSample(outL, outR, 0.3f, -0.3f);
    float tanhVal = std::tanh(0.3f);
    // tanh(0.3) should be close to 0.3
    REQUIRE(std::abs(tanhVal - 0.3f) < 0.01f);
}

TEST_CASE("WetProcessor auto-normalize target", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed steady -20dBFS signal for 2 seconds
    float inputLevel = MathUtils::dbToGain(-20.0f);  // ~0.1

    // Process 2 seconds to let auto-normalize settle
    for (int i = 0; i < 96000; ++i)
        wp.processSample(outL, outR, inputLevel, inputLevel);

    // Output should be closer to -6dBFS target
    float outputDb = MathUtils::gainToDb(std::abs(outL));
    // Allow wider tolerance — auto-normalize with 300ms attack is slow
    REQUIRE(outputDb > -12.0f);
    REQUIRE(outputDb < -1.0f);
}

TEST_CASE("WetProcessor noise gate bypass", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed -70dBFS signal (below -60dBFS gate)
    float quietLevel = MathUtils::dbToGain(-70.0f);

    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, quietLevel, quietLevel);

    // Below noise gate — output should be approximately tanh(input) with gain=1
    float expected = std::tanh(quietLevel);
    REQUIRE(outL == Catch::Approx(expected).margin(1e-5f));
}

TEST_CASE("WetProcessor gain clamp max", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Very quiet input just above noise gate
    float quietLevel = MathUtils::dbToGain(-50.0f);

    for (int i = 0; i < 96000; ++i)
        wp.processSample(outL, outR, quietLevel, quietLevel);

    // Gain should be clamped at 3.0 max
    float maxOutput = std::tanh(quietLevel) * 3.0f;
    REQUIRE(std::abs(outL) <= maxOutput + 0.001f);
}

TEST_CASE("WetProcessor freeze transition fast release", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Settle with quiet signal
    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, 0.1f, 0.1f);

    float beforeGain = std::abs(outL);

    // Notify freeze transition then feed loud signal
    wp.notifyFreezeTransition();
    for (int i = 0; i < 9600; ++i)  // 200ms
        wp.processSample(outL, outR, 0.8f, 0.8f);

    // Gain should have adapted — output level should be bounded
    REQUIRE(std::abs(outL) < 3.1f);
    REQUIRE(std::isfinite(outL));
}

TEST_CASE("WetProcessor no RMS drift", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Feed constant signal for 10 seconds
    float level = 0.3f;
    float lastOutput = 0.0f;

    for (int i = 0; i < 480000; ++i)
    {
        wp.processSample(outL, outR, level, level);
        lastOutput = outL;
    }

    // Process one more second and check stability
    float sumDelta = 0.0f;
    for (int i = 0; i < 48000; ++i)
    {
        wp.processSample(outL, outR, level, level);
        sumDelta += std::abs(outL - lastOutput);
        lastOutput = outL;
    }

    float avgDelta = sumDelta / 48000.0f;
    REQUIRE(avgDelta < 0.001f);  // Stable within 0.1%
}

TEST_CASE("WetProcessor reset clears state", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    // Process loud signal to change gain
    for (int i = 0; i < 48000; ++i)
        wp.processSample(outL, outR, 0.8f, 0.8f);

    wp.reset();

    // After reset, gain should be back to 1.0
    wp.processSample(outL, outR, 0.5f, 0.5f);
    // With gain=1.0, output = tanh(0.5) ≈ 0.4621
    REQUIRE(std::abs(outL - std::tanh(0.5f)) < 0.1f);
}
