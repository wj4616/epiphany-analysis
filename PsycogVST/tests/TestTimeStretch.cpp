/*
  PsycogVST Tests - TimeStretch granular engine tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "modules/TimeStretch.h"
#include "Parameters.h"
#include <cmath>
#include <vector>
#include <numeric>

static constexpr double testSampleRate = 48000.0;

// Helper: create and prepare a TimeStretch
static TimeStretch createPrepared()
{
    TimeStretch ts;
    ts.prepare(testSampleRate, 512);
    return ts;
}

// Helper: generate sine wave
static std::vector<float> generateSine(int numSamples, float freq = 440.0f)
{
    std::vector<float> buf(numSamples);
    for (int i = 0; i < numSamples; ++i)
        buf[i] = 0.5f * std::sin(2.0f * 3.14159265f * freq * i / static_cast<float>(testSampleRate));
    return buf;
}

TEST_CASE("TimeStretch produces output at stretch=1.0", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(8192);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    float sumSquares = 0.0f;
    for (int i = 0; i < 8192; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        sumSquares += outL * outL;
    }
    float rms = std::sqrt(sumSquares / 8192.0f);
    // Should produce some output (not silence)
    REQUIRE(rms > 0.01f);
}

TEST_CASE("TimeStretch extreme stretch=0.1 no crash", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);
    float normStretch = ParamConversions::stretchToNormalized(0.1f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch extreme stretch=10.0 no crash", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);
    float normStretch = ParamConversions::stretchToNormalized(10.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch silence in silence out", "[TimeStretch]")
{
    auto ts = createPrepared();
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    float maxOutput = 0.0f;
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        maxOutput = std::max(maxOutput, std::max(std::abs(outL), std::abs(outR)));
    }
    REQUIRE(maxOutput < 1e-6f);
}

TEST_CASE("TimeStretch freeze capture and playback", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(static_cast<int>(3.0 * testSampleRate));  // 3 seconds

    // Write 3 seconds of audio (fills freeze buffer)
    ts.writeFreezeBuffer(sine.data(), sine.data(), static_cast<int>(sine.size()));

    // Feed audio through (builds input buffer)
    float normStretch = ParamConversions::stretchToNormalized(1.0f);
    for (int i = 0; i < static_cast<int>(sine.size()); ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    // Trigger freeze
    ts.triggerFreeze();

    // Read in frozen mode — should produce non-silent output
    float sumSquares = 0.0f;
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Auto);
        sumSquares += outL * outL;
    }
    float rms = std::sqrt(sumSquares / 4096.0f);
    REQUIRE(rms > 0.001f);
}

TEST_CASE("TimeStretch mode transition no NaN", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(8192);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    // Process in Off mode
    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    ts.writeFreezeBuffer(sine.data(), sine.data(), 8192);
    ts.triggerFreeze();

    // Switch to Auto mode — crossfade should produce valid output
    for (int i = 4096; i < 8192; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Auto);
        REQUIRE(std::isfinite(outL));
        REQUIRE(std::isfinite(outR));
    }
}

TEST_CASE("TimeStretch position scrub changes output", "[TimeStretch]")
{
    auto ts = createPrepared();
    // Write distinctive audio to freeze buffer
    int bufLen = static_cast<int>(3.0 * testSampleRate);
    std::vector<float> audio(bufLen);
    for (int i = 0; i < bufLen; ++i)
        audio[i] = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f * i / static_cast<float>(testSampleRate));

    ts.writeFreezeBuffer(audio.data(), audio.data(), bufLen);

    // Feed enough to build input buffer
    float normStretch = ParamConversions::stretchToNormalized(1.0f);
    for (int i = 0; i < bufLen; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, audio[i], audio[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }

    ts.triggerFreeze();

    // Read at position=0.2
    float sumA = 0.0f;
    for (int i = 0; i < 2048; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.2f,
                         PsycogConstants::FreezeMode::Auto);
        sumA += outL;
    }

    // Read at position=0.8
    ts.reset();
    ts.prepare(testSampleRate, 512);
    ts.writeFreezeBuffer(audio.data(), audio.data(), bufLen);
    for (int i = 0; i < bufLen; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, audio[i], audio[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
    }
    ts.triggerFreeze();

    float sumB = 0.0f;
    for (int i = 0; i < 2048; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, 0.0f, 0.0f, normStretch, 0.8f,
                         PsycogConstants::FreezeMode::Auto);
        sumB += outL;
    }

    // Different positions should produce different sums
    REQUIRE(std::abs(sumA - sumB) > 0.001f);
}

TEST_CASE("TimeStretch sample rate 44100", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(44100.0, 512);
    auto sine = generateSine(4096, 440.0f);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
    }
}

TEST_CASE("TimeStretch sample rate 96000", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(96000.0, 512);
    auto sine = generateSine(4096, 440.0f);
    float normStretch = ParamConversions::stretchToNormalized(1.0f);

    for (int i = 0; i < 4096; ++i)
    {
        float outL, outR;
        ts.processSample(outL, outR, sine[i], sine[i], normStretch, 0.5f,
                         PsycogConstants::FreezeMode::Off);
        REQUIRE(std::isfinite(outL));
    }
}

TEST_CASE("TimeStretch buffer size stress 64", "[TimeStretch]")
{
    TimeStretch ts;
    ts.prepare(testSampleRate, 64);
    float normStretch = ParamConversions::stretchToNormalized(2.0f);

    // Process many 64-sample blocks
    for (int block = 0; block < 100; ++block)
    {
        for (int i = 0; i < 64; ++i)
        {
            float outL, outR;
            float in = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f * (block * 64 + i) / 48000.0f);
            ts.processSample(outL, outR, in, in, normStretch, 0.5f,
                             PsycogConstants::FreezeMode::Off);
            REQUIRE(std::isfinite(outL));
        }
    }
}

TEST_CASE("TimeStretch writeFreezeBuffer always works", "[TimeStretch]")
{
    auto ts = createPrepared();
    auto sine = generateSine(4096);

    // Writing in Off mode should not crash
    ts.writeFreezeBuffer(sine.data(), sine.data(), 4096);

    // Verify by switching to freeze — should have valid content
    ts.triggerFreeze();
    float outL, outR;
    ts.processSample(outL, outR, 0.0f, 0.0f,
                     ParamConversions::stretchToNormalized(1.0f), 0.5f,
                     PsycogConstants::FreezeMode::Auto);
    REQUIRE(std::isfinite(outL));
}
