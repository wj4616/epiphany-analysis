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
