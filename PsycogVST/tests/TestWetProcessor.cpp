/*
  PsycogVST Tests - WetProcessor module tests
  WetProcessor is now a passthrough — OutputProtection handles safety clipping.
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/WetProcessor.h"
#include "utils/MathUtils.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("WetProcessor passthrough preserves signal", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    wp.processSample(outL, outR, 0.3f, -0.5f);

    REQUIRE(outL == Catch::Approx(0.3f));
    REQUIRE(outR == Catch::Approx(-0.5f));
}

TEST_CASE("WetProcessor passthrough with hot signal", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    wp.processSample(outL, outR, 5.0f, -5.0f);

    // Passthrough — no clipping, OutputProtection handles that
    REQUIRE(outL == Catch::Approx(5.0f));
    REQUIRE(outR == Catch::Approx(-5.0f));
}

TEST_CASE("WetProcessor reset is safe", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    float outL, outR;
    for (int i = 0; i < 1000; ++i)
        wp.processSample(outL, outR, 0.8f, 0.8f);

    wp.reset();

    wp.processSample(outL, outR, 0.5f, 0.5f);
    REQUIRE(outL == Catch::Approx(0.5f));
    REQUIRE(outR == Catch::Approx(0.5f));
}

TEST_CASE("WetProcessor block processing matches per-sample", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    const int N = 64;
    float inL[N], inR[N], outL[N], outR[N];
    for (int i = 0; i < N; ++i)
    {
        inL[i] = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f * i / 48000.0f);
        inR[i] = inL[i];
    }

    wp.process(outL, outR, inL, inR, N);

    for (int i = 0; i < N; ++i)
    {
        REQUIRE(outL[i] == Catch::Approx(inL[i]));
        REQUIRE(outR[i] == Catch::Approx(inR[i]));
    }
}

TEST_CASE("WetProcessor freeze transition notification is safe", "[WetProcessor]")
{
    WetProcessor wp;
    wp.prepare(testSampleRate, 512);

    // Should not crash
    wp.notifyFreezeTransition();

    float outL, outR;
    for (int i = 0; i < 9600; ++i)
        wp.processSample(outL, outR, 0.5f, 0.5f);

    REQUIRE(outL == Catch::Approx(0.5f));
}
