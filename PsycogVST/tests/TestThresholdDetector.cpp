/*
  PsycogVST Tests - ThresholdDetector module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/ThresholdDetector.h"
#include <cmath>

static constexpr double testSampleRate = 48000.0;

TEST_CASE("ThresholdDetector above threshold triggers", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Feed sustained amplitude > threshold for 10ms+ to build EMA
    bool triggered = false;
    for (int i = 0; i < 1000; ++i)  // ~20ms
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            triggered = true;
    }
    REQUIRE(triggered);
}

TEST_CASE("ThresholdDetector below threshold no trigger", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    bool triggered = false;
    for (int i = 0; i < 48000; ++i)  // 1 second
    {
        if (td.processSample(0.1f, 0.1f, 0.5f))
            triggered = true;
    }
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector cooldown blocks re-trigger", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Build up to trigger
    bool firstTrigger = false;
    for (int i = 0; i < 1000; ++i)
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
        {
            firstTrigger = true;
            break;
        }
    }
    REQUIRE(firstTrigger);

    // Immediately try again — should be in cooldown (100ms = 4800 samples)
    bool secondTrigger = false;
    for (int i = 0; i < 2000; ++i)  // Less than 100ms
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            secondTrigger = true;
    }
    REQUIRE_FALSE(secondTrigger);

    // After cooldown (remaining ~2800 samples + some margin)
    bool afterCooldown = false;
    for (int i = 0; i < 5000; ++i)
    {
        if (td.processSample(0.8f, 0.8f, 0.3f))
            afterCooldown = true;
    }
    REQUIRE(afterCooldown);
}

TEST_CASE("ThresholdDetector transient spike ignored", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Single spike then silence
    td.processSample(1.0f, 1.0f, 0.3f);

    bool triggered = false;
    for (int i = 0; i < 100; ++i)
    {
        if (td.processSample(0.0f, 0.0f, 0.3f))
            triggered = true;
    }
    // Single sample spike should be smoothed out by 10ms EMA
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector threshold=0 chaos mode", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Any non-zero sustained input should trigger at threshold=0
    bool triggered = false;
    for (int i = 0; i < 1000; ++i)
    {
        if (td.processSample(0.01f, 0.01f, 0.0f))
            triggered = true;
    }
    // threshold=0 means rms >= 0 triggers — always true once EMA builds up
    REQUIRE(triggered);
}

TEST_CASE("ThresholdDetector threshold=1 effectively disabled", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Normal audio levels (-12dBFS ≈ 0.25 linear) should never trigger at threshold=1
    bool triggered = false;
    for (int i = 0; i < 48000; ++i)
    {
        if (td.processSample(0.25f, 0.25f, 1.0f))
            triggered = true;
    }
    REQUIRE_FALSE(triggered);
}

TEST_CASE("ThresholdDetector reset clears state", "[ThresholdDetector]")
{
    ThresholdDetector td;
    td.prepare(testSampleRate);

    // Build up EMA
    for (int i = 0; i < 1000; ++i)
        td.processSample(0.8f, 0.8f, 0.3f);

    // Reset
    td.reset();

    // After reset, no immediate trigger (EMA is 0)
    bool triggered = td.processSample(0.01f, 0.01f, 0.3f);
    REQUIRE_FALSE(triggered);
}
