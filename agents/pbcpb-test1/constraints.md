# Constraints

## Budget

| Category | Limit | Notes |
|----------|-------|-------|
| Claude AI | $200/month max | Currently $100, may upgrade |
| External tooling | Prefer free | Document cost if paid tool essential |
| Capital investment | Minimal | Solo enthusiast developer assumptions |

## Technical Stack

| Component | Version | Notes |
|-----------|---------|-------|
| C++ | C++17 | Modern features, wide compatibility |
| CMake | 3.22+ | FetchContent support |
| Compiler | GCC 7+ or Clang 6+ | Linux toolchain |
| JUCE | 8.0.x | Latest stable |
| VST3 SDK | 3.8.x | Steinberg SDK |
| AAX SDK | Latest compatible | Pro Tools support |
| Platform | Linux Mint | Target OS |
| DAW | REAPER | Testing environment |
| Build method | FetchContent | Self-contained, no global installs |

## Required Linux Dependencies

```bash
libx11-xcb-dev libxcb-util-dev libxcb-cursor-dev libxcb-xkb-dev
libxkbcommon-dev libxkbcommon-x11-dev libfontconfig1-dev libcairo2-dev
libgtkmm-3.0-dev libsqlite3-dev libxcb-keysyms1-dev
```

## Process Constraints

| Constraint | Description |
|------------|-------------|
| Workflow | Single developer + AI assistant (role-based-single-agent) |
| Session budget | Multiple Claude Code sessions per major feature expected |
| Token budget | Multiple limit-hits per build cycle expected — playbook must define session boundaries |
| Creative decisions | Human makes all: sound identity, aesthetic direction, signal flow, target genre |
| AI role | DSP implementation, code generation, research synthesis — human reviews and directs |
| C++ KB | Curated for audio relevance, not exhaustive general C++ |
