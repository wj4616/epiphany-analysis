/*
  PsycogVST Tests - DryDelay module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/DryDelay.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("DryDelay produces silence for first 2048 samples", "[DryDelay]")
{
    DryDelay dd;
    dd.prepare(testSampleRate, 512);

    float outL, outR;
    float inL = 1.0f, inR = 0.5f;

    // First 2048 samples should be silence (delay buffer is empty)
    for (int i = 0; i < 2048; ++i)
    {
        dd.process(&outL, &outR, &inL, &inR, 1);
        REQUIRE(outL == 0.0f);
        REQUIRE(outR == 0.0f);
    }
}

TEST_CASE("DryDelay outputs input after 2048 samples", "[DryDelay]")
{
    DryDelay dd;
    dd.prepare(testSampleRate, 512);

    // Feed 2048 samples of constant value, then check output
    std::vector<float> inputL(4096, 0.7f);
    std::vector<float> inputR(4096, -0.3f);
    std::vector<float> outputL(4096, 0.0f);
    std::vector<float> outputR(4096, 0.0f);

    dd.process(outputL.data(), outputR.data(), inputL.data(), inputR.data(), 4096);

    // Sample at index 2048 should be the input from index 0
    REQUIRE(outputL[2048] == Catch::Approx(0.7f));
    REQUIRE(outputR[2048] == Catch::Approx(-0.3f));
}

TEST_CASE("DryDelay preserves signal content", "[DryDelay]")
{
    DryDelay dd;
    dd.prepare(testSampleRate, 512);

    // Generate a sine wave
    int total = 8192;
    std::vector<float> inputL(total), outputL(total), outputR(total);
    for (int i = 0; i < total; ++i)
        inputL[i] = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f * i / 48000.0f);

    dd.process(outputL.data(), outputR.data(), inputL.data(), inputL.data(), total);

    // After the 2048-sample delay, output should match input shifted by 2048
    for (int i = 2048; i < total; ++i)
    {
        REQUIRE(outputL[i] == Catch::Approx(inputL[i - 2048]).margin(1e-6f));
    }
}

TEST_CASE("DryDelay reset clears buffer", "[DryDelay]")
{
    DryDelay dd;
    dd.prepare(testSampleRate, 512);

    // Fill the delay buffer
    std::vector<float> input(4096, 0.5f);
    std::vector<float> outL(4096), outR(4096);
    dd.process(outL.data(), outR.data(), input.data(), input.data(), 4096);

    // Reset
    dd.reset();

    // Next output should be silence again
    float inL = 1.0f, inR = 1.0f;
    float oL, oR;
    dd.process(&oL, &oR, &inL, &inR, 1);
    REQUIRE(oL == 0.0f);
    REQUIRE(oR == 0.0f);
}

TEST_CASE("DryDelay stereo independence", "[DryDelay]")
{
    DryDelay dd;
    dd.prepare(testSampleRate, 512);

    int total = 4096;
    std::vector<float> inputL(total), inputR(total);
    std::vector<float> outputL(total), outputR(total);

    for (int i = 0; i < total; ++i)
    {
        inputL[i] = 0.8f;
        inputR[i] = -0.4f;
    }

    dd.process(outputL.data(), outputR.data(), inputL.data(), inputR.data(), total);

    // After delay, L and R should have their own distinct values
    REQUIRE(outputL[2048] == Catch::Approx(0.8f));
    REQUIRE(outputR[2048] == Catch::Approx(-0.4f));
}
