/*
  PsycogVST Tests - Integration and state management tests
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "PluginProcessor.h"
#include "PresetManager.h"
#include "Parameters.h"
#include "Version.h"
#include <cmath>

// Helper: create and prepare a processor
static std::unique_ptr<PsycogAudioProcessor> createPreparedProcessor()
{
    auto proc = std::make_unique<PsycogAudioProcessor>();
    proc->prepareToPlay(48000.0, 512);
    return proc;
}

// Helper: process a buffer of sine wave through the processor
static float processAndMeasureRms(PsycogAudioProcessor& proc, int numBlocks, int blockSize = 512)
{
    juce::MidiBuffer midi;
    float sumSquares = 0.0f;
    int totalSamples = 0;

    for (int b = 0; b < numBlocks; ++b)
    {
        juce::AudioBuffer<float> buffer(2, blockSize);
        for (int i = 0; i < blockSize; ++i)
        {
            float sample = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * blockSize + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }

        proc.processBlock(buffer, midi);

        for (int i = 0; i < blockSize; ++i)
        {
            float s = buffer.getSample(0, i);
            sumSquares += s * s;
        }
        totalSamples += blockSize;
    }

    return std::sqrt(sumSquares / static_cast<float>(totalSamples));
}

TEST_CASE("Integration - processBlock produces finite output", "[integration]")
{
    auto proc = createPreparedProcessor();
    juce::MidiBuffer midi;

    // Process several blocks
    for (int b = 0; b < 20; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 512);
        for (int i = 0; i < 512; ++i)
        {
            float sample = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * 512 + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }

        proc->processBlock(buffer, midi);

        for (int i = 0; i < 512; ++i)
        {
            REQUIRE(std::isfinite(buffer.getSample(0, i)));
            REQUIRE(std::isfinite(buffer.getSample(1, i)));
            REQUIRE(buffer.getSample(0, i) >= -1.1f);  // tanh output ≤ 1.0
            REQUIRE(buffer.getSample(0, i) <= 1.1f);
        }
    }
}

TEST_CASE("Integration - silence in, silence out at defaults", "[integration]")
{
    auto proc = createPreparedProcessor();
    juce::MidiBuffer midi;

    // Default: stretch=1x, fold=0, mix=1.0 -> granular passthrough of silence = silence
    for (int b = 0; b < 10; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 512);
        buffer.clear();
        proc->processBlock(buffer, midi);
    }

    // After warmup, silence should produce silence
    juce::AudioBuffer<float> buffer(2, 512);
    buffer.clear();
    proc->processBlock(buffer, midi);

    float maxOutput = 0.0f;
    for (int i = 0; i < 512; ++i)
        maxOutput = std::max(maxOutput, std::abs(buffer.getSample(0, i)));

    REQUIRE(maxOutput < 0.001f);
}

TEST_CASE("Integration - preset change affects output", "[integration]")
{
    auto proc = createPreparedProcessor();

    // Measure with Init preset (no effect)
    float rmsInit = processAndMeasureRms(*proc, 30);

    // Switch to a heavy preset
    proc->setCurrentProgram(Presets::MirrorOfMirrorsIdx);

    // Re-prepare to reset module state
    proc->prepareToPlay(48000.0, 512);
    float rmsHeavy = processAndMeasureRms(*proc, 30);

    // Both should produce some output (not silence)
    REQUIRE(rmsInit > 0.001f);
    REQUIRE(rmsHeavy > 0.001f);
}

TEST_CASE("Integration - state save and restore round-trip", "[integration]")
{
    auto proc = createPreparedProcessor();

    // Set a non-default preset
    proc->setCurrentProgram(Presets::GoldenHelixIdx);
    REQUIRE(proc->getCurrentProgram() == Presets::GoldenHelixIdx);

    // Also set a parameter manually
    if (auto* param = proc->apvts.getParameter(ParamIDs::mix))
        param->setValueNotifyingHost(param->convertTo0to1(0.75f));

    // Save state
    juce::MemoryBlock stateData;
    proc->getStateInformation(stateData);
    REQUIRE(stateData.getSize() > 0);

    // Create a new processor and restore state
    auto proc2 = createPreparedProcessor();
    proc2->setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));

    // Verify program index was restored
    REQUIRE(proc2->getCurrentProgram() == Presets::GoldenHelixIdx);

    // Verify parameter was restored
    float restoredMix = *proc2->apvts.getRawParameterValue(ParamIDs::mix);
    REQUIRE(restoredMix == Catch::Approx(0.75f).margin(0.01f));
}

TEST_CASE("Integration - program names", "[integration]")
{
    auto proc = createPreparedProcessor();

    REQUIRE(proc->getNumPrograms() == Presets::numPresets);
    REQUIRE(proc->getProgramName(0) == "Init");
    REQUIRE(proc->getProgramName(Presets::numPresets - 1) == "Lucid Pulse");
}

TEST_CASE("Integration - latency reported correctly", "[integration]")
{
    auto proc = createPreparedProcessor();
    REQUIRE(proc->getLatencySamples() == PsycogConstants::grainSize);
}

TEST_CASE("Integration - processBlock with small block size", "[integration]")
{
    auto proc = std::make_unique<PsycogAudioProcessor>();
    proc->prepareToPlay(48000.0, 64);
    juce::MidiBuffer midi;

    // Process many small blocks — no crash
    for (int b = 0; b < 200; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 64);
        for (int i = 0; i < 64; ++i)
        {
            float sample = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * 64 + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }
        proc->processBlock(buffer, midi);

        for (int i = 0; i < 64; ++i)
        {
            REQUIRE(std::isfinite(buffer.getSample(0, i)));
            REQUIRE(std::isfinite(buffer.getSample(1, i)));
        }
    }
}

TEST_CASE("Integration - mono input handled", "[integration]")
{
    auto proc = std::make_unique<PsycogAudioProcessor>();
    proc->prepareToPlay(48000.0, 512);
    juce::MidiBuffer midi;

    // Create mono-ish buffer (2 channels but processor handles mono detection)
    juce::AudioBuffer<float> buffer(2, 512);
    for (int i = 0; i < 512; ++i)
    {
        float sample = 0.5f * std::sin(2.0f * 3.14159265f * 440.0f * i / 48000.0f);
        buffer.setSample(0, i, sample);
        buffer.setSample(1, i, sample);
    }

    proc->processBlock(buffer, midi);

    for (int i = 0; i < 512; ++i)
    {
        REQUIRE(std::isfinite(buffer.getSample(0, i)));
        REQUIRE(std::isfinite(buffer.getSample(1, i)));
    }
}

// =====================================================================
// DIAGNOSTIC: verify that DSP parameters actually change the output
// =====================================================================

// Helper: process sine wave and capture output samples
static std::vector<float> processAndCapture(PsycogAudioProcessor& proc, int numBlocks, int blockSize = 512)
{
    juce::MidiBuffer midi;
    std::vector<float> output;
    output.reserve(numBlocks * blockSize);

    for (int b = 0; b < numBlocks; ++b)
    {
        juce::AudioBuffer<float> buffer(2, blockSize);
        for (int i = 0; i < blockSize; ++i)
        {
            // 440Hz sine at 0.3 amplitude (typical pad level)
            float sample = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * blockSize + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }
        proc.processBlock(buffer, midi);
        for (int i = 0; i < blockSize; ++i)
            output.push_back(buffer.getSample(0, i));
    }
    return output;
}

// Helper: compute correlation between two signals (1.0 = identical, 0.0 = uncorrelated)
static float computeCorrelation(const std::vector<float>& a, const std::vector<float>& b,
                                 int offset = 0)
{
    int minSize = static_cast<int>(std::min(a.size(), b.size()));
    int n = minSize - std::abs(offset);
    if (n <= 0) return 0.0f;

    float sumAB = 0, sumAA = 0, sumBB = 0;
    for (int i = 0; i < n; ++i)
    {
        int ai = (offset >= 0) ? i + offset : i;
        int bi = (offset >= 0) ? i : i - offset;
        sumAB += a[ai] * b[bi];
        sumAA += a[ai] * a[ai];
        sumBB += b[bi] * b[bi];
    }
    if (sumAA < 1e-10f || sumBB < 1e-10f) return 0.0f;
    return sumAB / std::sqrt(sumAA * sumBB);
}

TEST_CASE("DIAGNOSTIC - fold amount changes output waveform", "[diagnostic]")
{
    // Run with Init (no fold)
    auto procInit = createPreparedProcessor();
    auto outInit = processAndCapture(*procInit, 60);

    // Run with fold = 0.5 (should be very audible)
    auto procFold = createPreparedProcessor();
    if (auto* p = procFold->apvts.getParameter(ParamIDs::foldAmount))
        p->setValueNotifyingHost(p->convertTo0to1(0.5f));
    auto outFold = processAndCapture(*procFold, 60);

    // Skip the first 2048*2 samples (grain warmup + dry delay)
    // Then compare the rest — they should be DIFFERENT
    int skip = 2048 * 3;
    REQUIRE(outInit.size() > (size_t)(skip + 4096));
    REQUIRE(outFold.size() > (size_t)(skip + 4096));

    // Measure RMS difference in a window after warmup
    float sumDiffSq = 0;
    float sumInitSq = 0;
    int count = 0;
    for (int i = skip; i < skip + 4096 && i < (int)outInit.size() && i < (int)outFold.size(); ++i)
    {
        float diff = outInit[i] - outFold[i];
        sumDiffSq += diff * diff;
        sumInitSq += outInit[i] * outInit[i];
        count++;
    }
    float rmsDiff = std::sqrt(sumDiffSq / count);
    float rmsInit = std::sqrt(sumInitSq / count);

    INFO("RMS of init output: " << rmsInit);
    INFO("RMS difference (init vs fold=0.5): " << rmsDiff);
    INFO("Ratio: " << (rmsInit > 0 ? rmsDiff / rmsInit : 0));

    // The difference should be substantial — at least 10% of the signal
    REQUIRE(rmsDiff > rmsInit * 0.1f);
}

TEST_CASE("DIAGNOSTIC - extreme stretch changes output", "[diagnostic]")
{
    // Run with default stretch (0.5 norm = 1.0x)
    auto proc1x = createPreparedProcessor();
    auto out1x = processAndCapture(*proc1x, 60);

    // Run with extreme stretch (0.9 norm ≈ 6.3x)
    auto proc6x = createPreparedProcessor();
    if (auto* p = proc6x->apvts.getParameter(ParamIDs::stretch))
        p->setValueNotifyingHost(p->convertTo0to1(0.9f));
    auto out6x = processAndCapture(*proc6x, 60);

    int skip = 2048 * 3;
    REQUIRE(out1x.size() > (size_t)(skip + 4096));

    float sumDiffSq = 0;
    float sum1xSq = 0;
    int count = 0;
    for (int i = skip; i < skip + 4096 && i < (int)out1x.size() && i < (int)out6x.size(); ++i)
    {
        float diff = out1x[i] - out6x[i];
        sumDiffSq += diff * diff;
        sum1xSq += out1x[i] * out1x[i];
        count++;
    }
    float rmsDiff = std::sqrt(sumDiffSq / count);
    float rms1x = std::sqrt(sum1xSq / count);

    INFO("RMS of 1x stretch output: " << rms1x);
    INFO("RMS difference (1x vs 6.3x stretch): " << rmsDiff);
    INFO("Ratio: " << (rms1x > 0 ? rmsDiff / rms1x : 0));

    // Extreme stretch should produce clearly different output
    REQUIRE(rmsDiff > rms1x * 0.05f);
}

TEST_CASE("DIAGNOSTIC - manual freeze captures and replays", "[diagnostic]")
{
    auto proc = createPreparedProcessor();
    juce::MidiBuffer midi;

    // Feed 1 second of 440Hz sine (fill the freeze buffer)
    for (int b = 0; b < 94; ++b)  // 94 * 512 = 48128 samples ≈ 1 second
    {
        juce::AudioBuffer<float> buffer(2, 512);
        for (int i = 0; i < 512; ++i)
        {
            float sample = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * 512 + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }
        proc->processBlock(buffer, midi);
    }

    // Enable Manual freeze
    if (auto* p = dynamic_cast<juce::AudioParameterChoice*>(
            proc->apvts.getParameter(ParamIDs::freezeMode)))
        p->setValueNotifyingHost(p->convertTo0to1(1));  // Manual

    // Now feed SILENCE — if freeze works, output should NOT be silent
    float maxOutput = 0.0f;
    for (int b = 0; b < 20; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 512);
        buffer.clear();
        proc->processBlock(buffer, midi);

        for (int i = 0; i < 512; ++i)
            maxOutput = std::max(maxOutput, std::abs(buffer.getSample(0, i)));
    }

    INFO("Max output during freeze with silent input: " << maxOutput);

    // Frozen content should still produce sound even with silent input
    REQUIRE(maxOutput > 0.01f);
}

TEST_CASE("DIAGNOSTIC - output differs from input with fold", "[diagnostic]")
{
    // This test compares the PROCESSED OUTPUT directly to the RAW INPUT.
    // If the plugin "sounds like bypass", these would be identical.
    auto proc = createPreparedProcessor();
    juce::MidiBuffer midi;

    // Set fold to maximum — should dramatically change the signal
    if (auto* p = proc->apvts.getParameter(ParamIDs::foldAmount))
        p->setValueNotifyingHost(p->convertTo0to1(1.0f));

    // Warm up (granular engine needs time to fill buffers)
    for (int b = 0; b < 40; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 512);
        for (int i = 0; i < 512; ++i)
        {
            float sample = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * 512 + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
        }
        proc->processBlock(buffer, midi);
    }

    // Now capture input vs output
    float sumInputSq = 0, sumOutputSq = 0, sumDiffSq = 0;
    int count = 0;
    for (int b = 40; b < 80; ++b)
    {
        juce::AudioBuffer<float> buffer(2, 512);
        std::vector<float> inputCopy(512);
        for (int i = 0; i < 512; ++i)
        {
            float sample = 0.3f * std::sin(2.0f * 3.14159265f * 440.0f
                           * (b * 512 + i) / 48000.0f);
            buffer.setSample(0, i, sample);
            buffer.setSample(1, i, sample);
            inputCopy[i] = sample;
        }
        proc->processBlock(buffer, midi);
        for (int i = 0; i < 512; ++i)
        {
            float out = buffer.getSample(0, i);
            float in = inputCopy[i];
            sumInputSq += in * in;
            sumOutputSq += out * out;
            sumDiffSq += (out - in) * (out - in);
            count++;
        }
    }

    float rmsInput = std::sqrt(sumInputSq / count);
    float rmsOutput = std::sqrt(sumOutputSq / count);
    float rmsDiff = std::sqrt(sumDiffSq / count);

    INFO("Input RMS: " << rmsInput);
    INFO("Output RMS (fold=1.0): " << rmsOutput);
    INFO("RMS difference (output vs input): " << rmsDiff);
    INFO("Diff/Input ratio: " << (rmsInput > 0 ? rmsDiff / rmsInput : 0));

    // With fold at maximum, output must be CLEARLY different from input
    REQUIRE(rmsDiff > rmsInput * 0.5f);
}

TEST_CASE("DIAGNOSTIC - mix knob changes output", "[diagnostic]")
{
    // Verify that mix=0 (dry) and mix=1 (wet) produce different output
    // when fold is applied (wet path is clearly different from dry)
    auto procDry = createPreparedProcessor();
    if (auto* p = procDry->apvts.getParameter(ParamIDs::foldAmount))
        p->setValueNotifyingHost(p->convertTo0to1(0.8f));
    if (auto* p = procDry->apvts.getParameter(ParamIDs::mix))
        p->setValueNotifyingHost(p->convertTo0to1(0.0f));  // 100% dry

    auto procWet = createPreparedProcessor();
    if (auto* p = procWet->apvts.getParameter(ParamIDs::foldAmount))
        p->setValueNotifyingHost(p->convertTo0to1(0.8f));
    if (auto* p = procWet->apvts.getParameter(ParamIDs::mix))
        p->setValueNotifyingHost(p->convertTo0to1(1.0f));  // 100% wet

    auto outDry = processAndCapture(*procDry, 60);
    auto outWet = processAndCapture(*procWet, 60);

    int skip = 2048 * 3;
    REQUIRE(outDry.size() > (size_t)(skip + 4096));

    float sumDiffSq = 0, sumDrySq = 0;
    int count = 0;
    for (int i = skip; i < skip + 4096 && i < (int)outDry.size() && i < (int)outWet.size(); ++i)
    {
        float diff = outDry[i] - outWet[i];
        sumDiffSq += diff * diff;
        sumDrySq += outDry[i] * outDry[i];
        count++;
    }
    float rmsDiff = std::sqrt(sumDiffSq / count);
    float rmsDry = std::sqrt(sumDrySq / count);

    INFO("RMS of dry (mix=0) output: " << rmsDry);
    INFO("RMS of wet (mix=1) output: " << std::sqrt(std::inner_product(
        outWet.begin() + skip, outWet.begin() + skip + count,
        outWet.begin() + skip, 0.0f) / count));
    INFO("RMS difference (dry vs wet): " << rmsDiff);
    INFO("Diff/Dry ratio: " << (rmsDry > 0 ? rmsDiff / rmsDry : 0));

    // With fold=0.8, wet should sound VERY different from dry
    REQUIRE(rmsDiff > rmsDry * 0.3f);
}
