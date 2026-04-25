/*
  PsycogVST - Interdimensional sound transformation plugin
  Phase 5: State Management - Version constants
*/

#pragma once

namespace PsycogVersion
{
    // Current plugin version - must match version in getStateInformation
    constexpr const char* current = "1.0";

    // Build timestamp — displayed in GUI to verify REAPER loads the latest binary.
    // __DATE__ and __TIME__ are set at compile time.
    #define PSYCOG_BUILD_STAMP __DATE__ " " __TIME__

    // Version history for migration support:
    // 1.0 - Initial release
}