/*
  PsycogVST Tests - Wavefolder module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/Wavefolder.h"
#include "utils/MathUtils.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

// Helper: prepare wavefolder and burn through smoother ramp
static Wavefolder createPreparedWavefolder()
{
    Wavefolder wf;
    wf.prepare(testSampleRate, 512);
    return wf;
}

TEST_CASE("Wavefolder passthrough at foldAmount=0", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float inL = 0.5f, inR = -0.3f;
    float outL, outR;

    // Process enough samples for smoother to settle
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, inL, inR, 0.0f, 0.0f, false);

    REQUIRE(outL == Catch::Approx(inL).margin(1e-4f));
    REQUIRE(outR == Catch::Approx(inR).margin(1e-4f));
}

TEST_CASE("Wavefolder output always bounded [-1, 1]", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Test extreme inputs at max fold
    float inputs[] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
    for (float in : inputs)
    {
        for (int i = 0; i < 2000; ++i)
            wf.processSample(outL, outR, in, in, 1.0f, 1.0f, false);

        REQUIRE(outL >= -1.0f);
        REQUIRE(outL <= 1.0f);
        REQUIRE(outR >= -1.0f);
        REQUIRE(outR <= 1.0f);
    }
}

TEST_CASE("Wavefolder stereo opposite offset", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Stereo input with offset=0.5 should produce different L and R
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.3f, 0.3f, 0.5f, 0.5f, false);

    // L gets +0.5 offset, R gets -0.5 offset -> different outputs
    REQUIRE(outL != Catch::Approx(outR).margin(0.01f));
}

TEST_CASE("Wavefolder mono stereo widening", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Mono input with offset > 0 should produce L != R
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.4f, 0.4f, 0.5f, 0.5f, true);

    REQUIRE(outL != Catch::Approx(outR).margin(0.01f));
}

TEST_CASE("Wavefolder DC offset scaling", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // foldAmount=0, offset=1.0 -> effective_offset=0 -> passthrough
    float inL = 0.7f, inR = -0.4f;
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, inL, inR, 0.0f, 1.0f, false);

    REQUIRE(outL == Catch::Approx(inL).margin(1e-4f));
    REQUIRE(outR == Catch::Approx(inR).margin(1e-4f));
}

TEST_CASE("Wavefolder zero input zero output", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // Zero input with zero offset at any fold amount -> output should be 0 (or near 0)
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.0f, 0.0f, 0.8f, 0.0f, false);

    REQUIRE(std::abs(outL) < 1e-4f);
    REQUIRE(std::abs(outR) < 1e-4f);
}

TEST_CASE("Wavefolder high drive output range", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();
    float outL, outR;

    // foldAmount=1.0 (drive=10x) should still be bounded
    for (int i = 0; i < 2000; ++i)
        wf.processSample(outL, outR, 0.9f, -0.9f, 1.0f, 0.0f, false);

    REQUIRE(outL >= -1.0f);
    REQUIRE(outL <= 1.0f);
    REQUIRE(outR >= -1.0f);
    REQUIRE(outR <= 1.0f);
}

TEST_CASE("Wavefolder negative offset swaps channels", "[Wavefolder]")
{
    auto wfPos = createPreparedWavefolder();
    auto wfNeg = createPreparedWavefolder();
    float posL, posR, negL, negR;

    // Positive offset: L=+offset, R=-offset
    for (int i = 0; i < 2000; ++i)
    {
        wfPos.processSample(posL, posR, 0.5f, 0.5f, 0.6f, 0.5f, false);
        wfNeg.processSample(negL, negR, 0.5f, 0.5f, 0.6f, -0.5f, false);
    }

    // Negative offset should swap L and R relative to positive offset
    REQUIRE(posL == Catch::Approx(negR).margin(0.01f));
    REQUIRE(posR == Catch::Approx(negL).margin(0.01f));
}

TEST_CASE("MathUtils::triangleFold values", "[Wavefolder]")
{
    // Verify the triangle fold function at key points
    REQUIRE(MathUtils::triangleFold(0.0f) == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(1.0f) == Catch::Approx(1.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(2.0f) == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(3.0f) == Catch::Approx(-1.0f).margin(1e-6f));
    REQUIRE(MathUtils::triangleFold(-1.0f) == Catch::Approx(-1.0f).margin(1e-6f));
}

TEST_CASE("Wavefolder block process produces valid output", "[Wavefolder]")
{
    auto wf = createPreparedWavefolder();

    // Process a block of non-zero input
    float inputs[512];
    float outputsL[512], outputsR[512];
    for (int i = 0; i < 512; ++i)
        inputs[i] = 0.5f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);

    wf.process(outputsL, outputsR, inputs, inputs, 512, 0.5f, 0.0f, false);

    // All outputs should be finite and bounded
    for (int i = 0; i < 512; ++i)
    {
        REQUIRE(std::isfinite(outputsL[i]));
        REQUIRE(std::isfinite(outputsR[i]));
        REQUIRE(outputsL[i] >= -1.0f);
        REQUIRE(outputsL[i] <= 1.0f);
    }
}
