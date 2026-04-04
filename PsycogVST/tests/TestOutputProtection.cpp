/*
  PsycogVST Tests - OutputProtection module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/OutputProtection.h"
#include <cmath>
#include <vector>

TEST_CASE("OutputProtection bounds output to [-1, 1]", "[OutputProtection]")
{
    OutputProtection op;

    float inputs[] = {-10.0f, -1.0f, 0.0f, 1.0f, 10.0f};
    for (float in : inputs)
    {
        float outL, outR;
        op.process(&outL, &outR, &in, &in, 1);
        REQUIRE(outL >= -1.0f);
        REQUIRE(outL <= 1.0f);
        REQUIRE(outR >= -1.0f);
        REQUIRE(outR <= 1.0f);
    }
}

TEST_CASE("OutputProtection transparent below 0.5", "[OutputProtection]")
{
    OutputProtection op;

    // tanh(0.3) ≈ 0.2913, deviation from 0.3 is ~0.009
    float in = 0.3f;
    float outL, outR;
    op.process(&outL, &outR, &in, &in, 1);

    REQUIRE(outL == Catch::Approx(std::tanh(0.3f)).margin(1e-6f));
    REQUIRE(std::abs(outL - in) < 0.01f);
}

TEST_CASE("OutputProtection zero in zero out", "[OutputProtection]")
{
    OutputProtection op;

    float in = 0.0f;
    float outL, outR;
    op.process(&outL, &outR, &in, &in, 1);

    REQUIRE(outL == 0.0f);
    REQUIRE(outR == 0.0f);
}

TEST_CASE("OutputProtection block processing", "[OutputProtection]")
{
    OutputProtection op;

    int n = 512;
    std::vector<float> input(n), outL(n), outR(n);
    for (int i = 0; i < n; ++i)
        input[i] = 5.0f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);

    op.process(outL.data(), outR.data(), input.data(), input.data(), n);

    for (int i = 0; i < n; ++i)
    {
        REQUIRE(std::isfinite(outL[i]));
        REQUIRE(outL[i] >= -1.0f);
        REQUIRE(outL[i] <= 1.0f);
        REQUIRE(outL[i] == Catch::Approx(std::tanh(input[i])).margin(1e-6f));
    }
}

TEST_CASE("OutputProtection in-place processing", "[OutputProtection]")
{
    OutputProtection op;

    int n = 256;
    std::vector<float> left(n), right(n);
    for (int i = 0; i < n; ++i)
    {
        left[i] = 3.0f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);
        right[i] = -2.0f * std::cos(2.0f * 3.14159f * 440.0f * i / 48000.0f);
    }

    // Save originals for comparison
    std::vector<float> origL(left), origR(right);

    op.processInPlace(left.data(), right.data(), n);

    for (int i = 0; i < n; ++i)
    {
        REQUIRE(left[i] == Catch::Approx(std::tanh(origL[i])).margin(1e-6f));
        REQUIRE(right[i] == Catch::Approx(std::tanh(origR[i])).margin(1e-6f));
        REQUIRE(left[i] >= -1.0f);
        REQUIRE(left[i] <= 1.0f);
    }
}

TEST_CASE("OutputProtection stereo independence", "[OutputProtection]")
{
    OutputProtection op;

    float inL = 2.0f, inR = -3.0f;
    float outL, outR;
    op.process(&outL, &outR, &inL, &inR, 1);

    REQUIRE(outL == Catch::Approx(std::tanh(2.0f)).margin(1e-6f));
    REQUIRE(outR == Catch::Approx(std::tanh(-3.0f)).margin(1e-6f));
    REQUIRE(outL != Catch::Approx(outR));
}
