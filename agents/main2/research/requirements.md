# Requirements Document

## Domain Requirements

### Core Development
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-01 | Guide developer through full VST3 plugin lifecycle | Critical | Commission brief |
| DR-02 | Cover JUCE 8.0.x + C++17 + CMake FetchContent | Critical | Constraints |
| DR-03 | Include DSP implementation with two-stage review | Critical | VST Playbook v7 |
| DR-04 | Include audio thread safety patterns | Critical | Failure modes |
| DR-05 | Include parameter management (APVTS) | Critical | VST Playbook v7 |
| DR-06 | Include state save/restore | Critical | FM-03 |
| DR-07 | Include preset system design | High | VST Playbook v7 |
| DR-08 | Include GUI implementation guidance | High | VST Playbook v7 |
| DR-09 | Include DAW testing methodology | High | VST Playbook v7 |
| DR-10 | Include optimization/profiling guidance | Medium | VST Playbook v7 |

### Sound Design Integration
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-11 | Include synthesis theory coverage | High | Commission brief |
| DR-12 | Cover psychedelic/ambient/creative sound design | High | Commission brief |
| DR-13 | Provide bridge entries (subjective → parameter) | Critical | Commission brief |
| DR-14 | Reference authoritative sources (Pirkle, Zölzer, Smith) | Medium | Best practices |

### Commercial & Business
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| DR-15 | Cover licensing system integration | Critical | Commission brief |
| DR-16 | Cover marketplace listing (KVR, Plugin Boutique) | Critical | Commission brief |
| DR-17 | Include pricing strategy guidance | High | Commission brief |
| DR-18 | Include marketing strategy for solo developer | High | Commission brief |
| DR-19 | Include business plan template | Medium | Commission brief |

---

## Quality Requirements

### Playbook Quality Standards
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| QR-01 | Every task has owner [Role] | Critical | Playbook standard |
| QR-02 | Every non-obvious task has description | Critical | Playbook standard |
| QR-03 | Every phase has gate with explicit conditions | Critical | Playbook standard |
| QR-04 | No placeholder content (TBD, TODO) | Critical | Playbook standard |
| QR-05 | Compilation blocks on every phase | High | Playbook-creator-playbook |
| QR-06 | Handoff blocks on every gate | High | Playbook-creator-playbook |
| QR-07 | Role consistency throughout | High | CCC-02 |

### Output Quality
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| QR-08 | Produced playbook executable without ambiguity | Critical | Success criteria |
| QR-09 | Another developer can follow and produce result | High | Success criteria |
| QR-10 | Pilot test 90%+ tasks completed as-written | High | Success criteria |

---

## Strategic Requirements

### Budget Constraints
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| SR-01 | Claude Code budget ≤ $200/month | High | Constraints |
| SR-02 | External tooling prefer free | Medium | Constraints |
| SR-03 | Total capital < $500 (excluding Claude) | High | Constraints |

### Technical Constraints
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| SR-04 | C++17 minimum | Critical | Constraints |
| SR-05 | JUCE 8.0.x | Critical | Constraints |
| SR-06 | CMake 3.22+ with FetchContent | Critical | Constraints |
| SR-07 | VST3 format primary | Critical | Constraints |
| SR-08 | Linux + REAPER testing primary | High | Constraints |

### Process Constraints
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| SR-09 | Human makes all creative decisions | Critical | Constraints |
| SR-10 | Single developer + AI assistant workflow | Critical | Constraints |
| SR-11 | Multiple sessions per feature expected | High | Constraints |
| SR-12 | Session boundaries respect token budget | High | Constraints |

---

## Platform Requirements

### Development Environment
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| PR-01 | Build on Linux Mint | Critical | Constraints |
| PR-02 | Test in REAPER DAW | Critical | Constraints |
| PR-03 | VST3 SDK 3.8.x | High | Constraints |
| PR-04 | GCC 7+ or Clang 6+ | High | Constraints |

### Cross-Platform Considerations
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| PR-05 | Document macOS build considerations | Medium | Scope |
| PR-06 | Document Windows build considerations | Medium | Scope |
| PR-07 | AU format for macOS (secondary) | Low | Scope |
| PR-08 | AAX format (future consideration) | Low | Scope |

---

## Cross-Cutting Requirements

### Audio Thread Safety
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CCR-01 | No allocations on audio thread | Critical | CCC-01 |
| CCR-02 | ScopedNoDenormals in processBlock | Critical | CCC-01 |
| CCR-03 | All buffers pre-allocated | Critical | CCC-01 |
| CCR-04 | Thread-safe cross-thread communication | Critical | CCC-01 |

### State & Parameters
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CCR-05 | State round-trip test passes | Critical | CCC-03 |
| CCR-06 | Parameter IDs locked before release | High | CCC-04 |
| CCR-07 | SmoothedValue audit at each gate | High | CCC-04 |

### Human Authority
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| CCR-08 | Signal flow user-defined | Critical | CCC-06 |
| CCR-09 | Sound identity words exact | Critical | CCC-06 |
| CCR-10 | Preset names user-approved | High | CCC-06 |

---

## User Requirements

### Target User Profile
| Requirement | Detail |
|-------------|--------|
| Strong: Python/JavaScript, VST usage | Leverage existing knowledge |
| Basic: C++ theory, sound design | Provide C++ explanations inline |
| None: JUCE, DSP implementation, commercial distribution | Comprehensive coverage needed |
| AI tooling: Claude Code | Session-based workflow |

### Documentation Needs
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| UR-01 | C++ concepts explained inline | High | Target user |
| UR-02 | JUCE patterns documented | Critical | Target user |
| UR-03 | DSP algorithms with math | High | Target user |
| UR-04 | Code examples for each task | Critical | Target user |

---

## Knowledge Base Requirements

### Five-Layer Architecture
| Layer | Purpose | Priority |
|-------|---------|----------|
| Technical | C++/JUCE/DSP patterns | Critical |
| Sound Design | Synthesis + bridges | Critical |
| UI/UX | Audio plugin standards | High |
| Commercial | Marketplaces/licensing | Critical (NEW) |
| Reference | Academic/textbook index | High |

### Bridge Requirements
| ID | Requirement | Priority | Source |
|----|-------------|----------|--------|
| KBR-01 | Sound Design → Technical bridges | Critical | Commission brief |
| KBR-02 | Subjective → parameter mapping | Critical | Commission brief |
| KBR-03 | "Warm" → filter cutoff + saturation | High | Example |
| KBR-04 | Multiple bridge entries per descriptor | Medium | Coverage |

---

## Summary

| Category | Total | Critical | High | Medium | Low |
|----------|-------|----------|------|--------|-----|
| Domain | 19 | 10 | 7 | 2 | 0 |
| Quality | 10 | 5 | 5 | 0 | 0 |
| Strategic | 12 | 6 | 4 | 2 | 0 |
| Platform | 8 | 3 | 3 | 2 | 0 |
| Cross-Cutting | 10 | 6 | 4 | 0 | 0 |
| User | 4 | 2 | 2 | 0 | 0 |
| Knowledge Base | 4 | 3 | 1 | 0 | 0 |
| **Total** | **67** | **35** | **26** | **6** | **0** |