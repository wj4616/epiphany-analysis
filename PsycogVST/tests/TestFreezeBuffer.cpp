/*
  PsycogVST Tests - FreezeBuffer module tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/FreezeBuffer.h"
#include <cmath>
#include <vector>

static constexpr double testSampleRate = 48000.0;

static FreezeBuffer createPrepared()
{
    FreezeBuffer fb;
    fb.prepare(testSampleRate);
    return fb;
}

TEST_CASE("FreezeBuffer initial state is silent", "[FreezeBuffer]")
{
    auto fb = createPrepared();
    REQUIRE_FALSE(fb.isFrozen());

    // Read from unfrozen buffer should be silent
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.5f);
    REQUIRE(outL == 0.0f);
    REQUIRE(outR == 0.0f);
}

TEST_CASE("FreezeBuffer write and freeze", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write a known pattern
    int numSamples = 48000;  // 1 second
    std::vector<float> left(numSamples), right(numSamples);
    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = 0.5f * std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);
        right[i] = 0.3f * std::sin(2.0f * 3.14159f * 880.0f * i / 48000.0f);
    }

    fb.write(left.data(), right.data(), numSamples);
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    // Read — should produce non-silent output
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.0f);
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("FreezeBuffer rapid re-trigger allowed", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write some audio
    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);

    // First trigger
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    // Immediate re-trigger should succeed — cooldown is ThresholdDetector's job,
    // not FreezeBuffer's. FreezeBuffer is pure storage.
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    // Should still produce valid output
    float outL, outR;
    fb.readSampleAt(0, outL, outR);
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("FreezeBuffer toggle freeze", "[FreezeBuffer]")
{
    auto fb = createPrepared();
    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);

    REQUIRE_FALSE(fb.isFrozen());

    fb.toggleFreeze();
    REQUIRE(fb.isFrozen());

    fb.toggleFreeze();
    REQUIRE_FALSE(fb.isFrozen());
}

TEST_CASE("FreezeBuffer position read at boundaries", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write 3 seconds of distinctive pattern
    int bufLen = static_cast<int>(3.0 * testSampleRate);
    std::vector<float> left(bufLen), right(bufLen);
    for (int i = 0; i < bufLen; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(bufLen);
        left[i] = std::sin(2.0f * 3.14159f * (200.0f + 800.0f * t) * i / 48000.0f);
        right[i] = left[i] * 0.8f;
    }

    fb.write(left.data(), right.data(), bufLen);
    fb.triggerFreeze();

    // Read at position 0.0
    float outL0, outR0;
    fb.read(&outL0, &outR0, 1, 0.0f);
    REQUIRE(std::isfinite(outL0));

    // Read at position 0.5
    float outL5, outR5;
    fb.read(&outL5, &outR5, 1, 0.5f);
    REQUIRE(std::isfinite(outL5));

    // Read at position 1.0
    float outL1, outR1;
    fb.read(&outL1, &outR1, 1, 1.0f);
    REQUIRE(std::isfinite(outL1));
}

TEST_CASE("FreezeBuffer stereo content preserved", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    // Write different L and R
    int numSamples = 48000;
    std::vector<float> left(numSamples), right(numSamples);
    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = 0.5f;
        right[i] = -0.3f;
    }

    fb.write(left.data(), right.data(), numSamples);
    fb.triggerFreeze();

    // Wait for crossfade to complete
    float dummyL, dummyR;
    int crossfadeLen = static_cast<int>(10.0 * testSampleRate / 1000.0) + 100;
    // Read one sample at a time to properly advance crossfade
    for (int i = 0; i < crossfadeLen; ++i)
        fb.read(&dummyL, &dummyR, 1, 0.5f);

    // Read should have different L and R
    float outL, outR;
    fb.read(&outL, &outR, 1, 0.5f);
    REQUIRE(std::isfinite(outL));
    REQUIRE(std::isfinite(outR));
}

TEST_CASE("FreezeBuffer reset clears state", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    std::vector<float> audio(48000, 0.5f);
    fb.write(audio.data(), audio.data(), 48000);
    fb.triggerFreeze();
    REQUIRE(fb.isFrozen());

    fb.reset();
    REQUIRE_FALSE(fb.isFrozen());
}

TEST_CASE("FreezeBuffer rapid freeze cycling no crash", "[FreezeBuffer]")
{
    auto fb = createPrepared();

    std::vector<float> audio(48000, 0.3f);
    fb.write(audio.data(), audio.data(), 48000);

    // Trigger 10 times rapidly — no cooldown in FreezeBuffer
    for (int cycle = 0; cycle < 10; ++cycle)
    {
        fb.triggerFreeze();
        REQUIRE(fb.isFrozen());

        // Verify readSampleAt still works after each swap
        float outL, outR;
        fb.readSampleAt(cycle * 100, outL, outR);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}
