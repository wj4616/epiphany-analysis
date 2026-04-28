# Strategic Constraints

## Budget Constraints

| Category | Limit | Notes |
|----------|-------|-------|
| Claude AI | $200/month max | Currently $100, may upgrade |
| External Tooling | Prefer free | Paid tools only if essential, documented with justification |
| Total Capital | < $500 | Excludes Claude subscription; solo enthusiast budget |

## Technical Stack

| Component | Version | Notes |
|-----------|---------|-------|
| C++ | C++17 | Modern features, wide compatibility |
| CMake | 3.22+ | FetchContent for dependencies |
| Compiler | GCC 7+ or Clang 6+ | Linux Mint standard |
| JUCE | 8.0.x | Latest stable |
| VST3 SDK | 3.8.x | Current VST3 standard |
| AAX SDK | Latest compatible | For future Pro Tools support |
| Platform | Linux Mint | Primary development |
| DAW | REAPER | Testing environment |
| Test Framework | Catch2 | Header-only, FetchContent compatible |

## Required Linux Dependencies

```bash
sudo apt install libx11-xcb-dev libxcb-util-dev libxcb-cursor-dev \
    libxcb-xkb-dev libxkbcommon-dev libxkbcommon-x11-dev \
    libfontconfig1-dev libcairo2-dev libgtkmm-3.0-dev \
    libsqlite3-dev libxcb-keysyms1-dev
```

## Process Constraints

### Workflow Model
- **Single developer + AI assistant** (role-based-single-agent)
- Multiple Claude Code sessions expected per major feature
- Playbook must define session boundaries respecting token budget

### Human-AI Division
| Human Decides | AI Handles |
|---------------|------------|
| Sound identity | DSP implementation |
| Aesthetic direction | Code generation |
| Signal flow design | Research synthesis |
| Target genre | KB curation |
| Creative choices | Technical execution |

### Knowledge Base Constraints
- C++ KB: Audio-relevant only, not exhaustive
- Sound Design KB: Focus on creative/psychedelic/ambient techniques
- Reference KB: Curated sources, not comprehensive library

## Timeline Constraints

| Phase | Target Duration | Notes |
|-------|-----------------|-------|
| Phases 0-1 | 1 session | Scoping + research coupled |
| Phase 2 | Focused session | KB schema design |
| Phase 3 | Fresh session | Architecture only |
| Phases 4-5 | Shared session | Role + task engineering |
| Phases 6-8 | Shared session | Output config + metrics + assembly |
| Phases 9-10 | Separate sessions | Quality audits |
| Phase 11 | As needed | Stakeholder iteration |
| Phase 12 | Separate session | Pilot test |
| Phase 13 | Wrap session | Documentation |