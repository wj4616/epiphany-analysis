/*
  PsycogVST Tests - Parameter conversion tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Parameters.h"

TEST_CASE("Stretch log conversion round-trip", "[Parameters]")
{
    SECTION("normalized=0.0 -> stretch=0.1x")
    {
        float stretch = ParamConversions::stretchFromNormalized(0.0f);
        REQUIRE(stretch == Catch::Approx(0.1f).margin(0.001f));
    }

    SECTION("normalized=0.5 -> stretch=1.0x")
    {
        float stretch = ParamConversions::stretchFromNormalized(0.5f);
        REQUIRE(stretch == Catch::Approx(1.0f).margin(0.01f));
    }

    SECTION("normalized=1.0 -> stretch=10.0x")
    {
        float stretch = ParamConversions::stretchFromNormalized(1.0f);
        REQUIRE(stretch == Catch::Approx(10.0f).margin(0.01f));
    }

    SECTION("Round-trip at multiple points")
    {
        for (float norm : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
        {
            float stretch = ParamConversions::stretchFromNormalized(norm);
            float backToNorm = ParamConversions::stretchToNormalized(stretch);
            REQUIRE(backToNorm == Catch::Approx(norm).margin(1e-5f));
        }
    }
}

TEST_CASE("LFO rate log conversion round-trip", "[Parameters]")
{
    SECTION("normalized=0.0 -> rate=0.01 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(0.0f);
        REQUIRE(rate == Catch::Approx(0.01f).margin(0.001f));
    }

    SECTION("normalized=0.5 -> rate ~0.45 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(0.5f);
        // sqrt(0.01 * 20) = sqrt(0.2) ≈ 0.447
        REQUIRE(rate == Catch::Approx(0.447f).margin(0.05f));
    }

    SECTION("normalized=1.0 -> rate=20.0 Hz")
    {
        float rate = ParamConversions::lfoRateFromNormalized(1.0f);
        REQUIRE(rate == Catch::Approx(20.0f).margin(0.01f));
    }

    SECTION("Round-trip at multiple points")
    {
        for (float norm : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
        {
            float rate = ParamConversions::lfoRateFromNormalized(norm);
            float backToNorm = ParamConversions::lfoRateToNormalized(rate);
            REQUIRE(backToNorm == Catch::Approx(norm).margin(1e-5f));
        }
    }
}
