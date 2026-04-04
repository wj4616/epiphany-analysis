/*
  PsycogVST Tests - Mixer module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/Mixer.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

static Mixer createPrepared()
{
    Mixer m;
    m.prepare(testSampleRate, 512);
    return m;
}

TEST_CASE("Mixer 100% wet outputs wet only", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;  // Enough for smoother to settle
    std::vector<float> wet(n, 0.7f);
    std::vector<float> dry(n, 0.3f);
    std::vector<float> outL(n), outR(n);

    m.process(outL.data(), outR.data(), wet.data(), wet.data(),
              dry.data(), dry.data(), n, 1.0f);

    // At mix=1.0: wetGain = sin(pi/2) = 1.0, dryGain = cos(pi/2) = 0.0
    REQUIRE(outL[n - 1] == Catch::Approx(0.7f).margin(0.01f));
}

TEST_CASE("Mixer 0% wet outputs dry only", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;
    std::vector<float> wet(n, 0.7f);
    std::vector<float> dry(n, 0.3f);
    std::vector<float> outL(n), outR(n);

    m.process(outL.data(), outR.data(), wet.data(), wet.data(),
              dry.data(), dry.data(), n, 0.0f);

    // At mix=0.0: wetGain = sin(0) = 0.0, dryGain = cos(0) = 1.0
    REQUIRE(outL[n - 1] == Catch::Approx(0.3f).margin(0.01f));
}

TEST_CASE("Mixer 50% equal-power crossfade", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;
    std::vector<float> wet(n, 1.0f);
    std::vector<float> dry(n, 1.0f);
    std::vector<float> outL(n), outR(n);

    m.process(outL.data(), outR.data(), wet.data(), wet.data(),
              dry.data(), dry.data(), n, 0.5f);

    // At mix=0.5: wetGain = sin(pi/4) = 0.707, dryGain = cos(pi/4) = 0.707
    // Sum = 0.707 + 0.707 = 1.414 (not 1.0 — equal power, not equal amplitude)
    float expected = std::sin(0.5f * juce::MathConstants<float>::halfPi)
                   + std::cos(0.5f * juce::MathConstants<float>::halfPi);
    REQUIRE(outL[n - 1] == Catch::Approx(expected).margin(0.02f));
}

TEST_CASE("Mixer output bounded", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;
    std::vector<float> wet(n, 1.0f);
    std::vector<float> dry(n, 1.0f);
    std::vector<float> outL(n), outR(n);

    // At any mix value, equal-power crossfade of unit signals should not exceed ~1.414
    for (float mix : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
    {
        m.reset();
        m.process(outL.data(), outR.data(), wet.data(), wet.data(),
                  dry.data(), dry.data(), n, mix);

        REQUIRE(std::abs(outL[n - 1]) <= 1.5f);
    }
}

TEST_CASE("Mixer stereo preserves channels", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;
    std::vector<float> wetL(n, 0.8f);
    std::vector<float> wetR(n, -0.4f);
    std::vector<float> dryL(n, 0.2f);
    std::vector<float> dryR(n, -0.1f);
    std::vector<float> outL(n), outR(n);

    m.process(outL.data(), outR.data(), wetL.data(), wetR.data(),
              dryL.data(), dryR.data(), n, 1.0f);

    // 100% wet: output should match wet channels
    REQUIRE(outL[n - 1] == Catch::Approx(0.8f).margin(0.01f));
    REQUIRE(outR[n - 1] == Catch::Approx(-0.4f).margin(0.01f));
}

TEST_CASE("Mixer smooths transitions", "[Mixer]")
{
    auto m = createPrepared();

    int n = 2000;
    std::vector<float> wet(n, 1.0f);
    std::vector<float> dry(n, 0.0f);
    std::vector<float> outL(n), outR(n);

    // Process at mix=0 to settle
    m.process(outL.data(), outR.data(), wet.data(), wet.data(),
              dry.data(), dry.data(), n, 0.0f);

    // Jump to mix=1.0 — first sample should NOT be at full wet (smoother ramps)
    float singleWet = 1.0f, singleDry = 0.0f;
    float oL, oR;
    m.process(&oL, &oR, &singleWet, &singleWet,
              &singleDry, &singleDry, 1, 1.0f);

    // Should be less than full wet due to smoothing ramp
    REQUIRE(oL < 0.9f);
}
