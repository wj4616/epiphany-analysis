cmake_minimum_required(VERSION 3.22)
project(PsycogVST VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

# Catch2 for testing
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

juce_add_plugin(PsycogVST
    COMPANY_NAME              "CogAudio"
    PLUGIN_MANUFACTURER_CODE  CgAu
    PLUGIN_CODE               PsVt
    FORMATS                   VST3
    PRODUCT_NAME              "Psycog VST"
    IS_SYNTH                  FALSE
    NEEDS_MIDI_INPUT          FALSE
    NEEDS_MIDI_OUTPUT         FALSE
    IS_MIDI_EFFECT            FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD   TRUE
    VST3_COPY_DIR             "$ENV{HOME}/.vst3"
)

juce_generate_juce_header(PsycogVST)

# Module sources (to be implemented in Phase 4)
set(MODULE_SOURCES
    src/modules/DryDelay.cpp
    src/modules/TimeStretch.cpp
    src/modules/FreezeBuffer.cpp
    src/modules/Wavefolder.cpp
    src/modules/ThresholdDetector.cpp
    src/modules/WetProcessor.cpp
    src/modules/Mixer.cpp
    src/modules/LFO.cpp
    src/modules/OutputProtection.cpp
)

target_sources(PsycogVST PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/PresetManager.cpp
    ${MODULE_SOURCES}
)

target_include_directories(PsycogVST PRIVATE src)

target_compile_definitions(PsycogVST
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(PsycogVST
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ==============================================================================
# Tests
# ==============================================================================

# Module sources needed for tests (same as plugin)
set(TEST_MODULE_SOURCES
    src/modules/DryDelay.cpp
    src/modules/TimeStretch.cpp
    src/modules/FreezeBuffer.cpp
    src/modules/Wavefolder.cpp
    src/modules/ThresholdDetector.cpp
    src/modules/WetProcessor.cpp
    src/modules/Mixer.cpp
    src/modules/LFO.cpp
    src/modules/OutputProtection.cpp
)

add_executable(PsycogVSTTests
    tests/TestTimeStretch.cpp
    tests/TestWavefolder.cpp
    tests/TestLFO.cpp
    tests/TestFreezeBuffer.cpp
    tests/TestParameters.cpp
    tests/TestPresets.cpp
    tests/TestThresholdDetector.cpp
    tests/TestWetProcessor.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/PresetManager.cpp
    ${TEST_MODULE_SOURCES}
)

target_link_libraries(PsycogVSTTests
    PRIVATE
        Catch2::Catch2WithMain
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_dsp
)

# Don't need GUI for tests
target_compile_definitions(PsycogVSTTests
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_include_directories(PsycogVSTTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Add tests to CTest
include(Catch)
catch_discover_tests(PsycogVSTTests)