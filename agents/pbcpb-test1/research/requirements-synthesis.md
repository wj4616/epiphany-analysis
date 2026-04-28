# Requirements Synthesis: PBCPB Playbook

## 1. Executive Summary

This document synthesizes all domain research into actionable requirements for the PBCPB (Playbook for Building Creative VST Plugins). The playbook must guide a single developer with Claude Code assistance through:

1. **Development**: VST3 plugin creation using JUCE 8.0.x + C++17
2. **Sound Design**: Research-informed DSP implementation
3. **UI/UX**: Modern, clean interface design
4. **Commercial**: Licensing, marketplace, marketing
5. **Release**: Complete product launch

## 2. Playbook Structure Requirements

### 2.1 Phase Count Estimate

Based on complexity analysis:
- **Existing v7 JUCE playbook**: Phases 0-12 (development only)
- **PBCPB additions**: 5+ phases for commercial aspects
- **Total estimated**: 17-20 phases

### 2.2 Phase Categories

| Category | Phases | Description |
|----------|--------|-------------|
| **Foundation** | 0-3 | Concept, research, architecture, setup |
| **Development** | 4-8 | DSP, parameters, integration, GUI, testing |
| **Validation** | 9-12 | DAW testing, audits, profiling, release prep |
| **Commercial** | 13-16 | Licensing, marketplace, marketing, launch |
| **Post-release** | 17+ | Updates, analytics, iteration |

### 2.3 Required Outputs per Phase

| Output | Phases | Mandatory |
|--------|--------|-----------|
| Task checklist | All | Yes |
| Gate conditions | All | Yes |
| Artifact deliverables | Most | Yes |
| Failure modes | All | Yes |
| Prevention rules | Key phases | Yes |
| Skill invocation | Select phases | Where applicable |

## 3. Knowledge Base Architecture Requirements

### 3.1 Five-Layer Structure

```
Technical KB (Layer 1)
├── C++17 audio-relevant patterns
├── JUCE framework specifics
├── CMake build configuration
├── DSP algorithms catalog
├── Audio thread safety patterns
└── Testing frameworks

Sound Design KB (Layer 2)
├── Synthesis theory
├── Effect theory
├── Patch methodology
├── Genre-specific techniques (psychedelic, ambient, psybient)
└── Creative processes

UI/UX KB (Layer 3)
├── Control design patterns
├── Layout theory
├── Color/typography
├── JUCE LookAndFeel
└── Accessibility

Commercial KB (Layer 4)
├── Licensing systems
├── Marketplace strategies
├── Pricing frameworks
├── Marketing playbooks
└── Business planning

Reference KB (Layer 5)
├── Academic sources index
├── Textbook references
├── Expert tutorials
└── Tool documentation
```

### 3.2 Bridge Layer (Sound Design → Technical)

Required translation mappings for subjective descriptors:

| Descriptor | DSP Translation |
|------------|-----------------|
| "warm" | Filter cutoff: [0.2, 0.4], saturation: [0.1, 0.3], even harmonics |
| "bright" | Filter cutoff: [0.6, 1.0], presence boost: 2-5kHz |
| "lush" | Detune: [0.05, 0.15], chorus: [0.3, 0.5], reverb: [0.3, 0.6] |
| "punchy" | Fast attack: <10ms, filter envelope, transient emphasis |
| "ethereal" | Long reverb: >2s, high-pass filter, modulation |
| "aggressive" | Distortion: [0.4, 0.8], odd harmonics, fast release |
| "deep" | Low emphasis: <200Hz, slow modulation, sub content |

### 3.3 KB Population Strategy

| Phase | KB Activity |
|-------|-------------|
| Phase 2 | Architecture defines KB structure |
| Phase 3 | Placeholder seeding in directories |
| Phase 4-8 | Population during development |
| Phase 9+ | Validation and curation |

## 4. Cross-Cutting Requirements

### 4.1 Session Boundaries

Based on commission brief:
- Multiple Claude Code sessions per major feature expected
- Token budget: $200/month maximum
- Playbook must define session boundaries

**Session Boundary Design:**
| Phase Group | Sessions | Boundary Points |
|--------------|----------|-----------------|
| 0-3 (Foundation) | 2-4 | After spec, after architecture |
| 4-8 (Development) | 6-10 | Per DSP module, after integration |
| 9-12 (Validation) | 2-4 | After testing, after profiling |
| 13-16 (Commercial) | 4-6 | Per commercial phase |
| 17+ (Post-release) | Ongoing | Per update cycle |

### 4.2 Role Assignment

| Role | Responsibilities |
|------|-------------------|
| **Human** | Creative decisions, DAW testing, final approval |
| **AI** | Research, implementation, testing, documentation |
| **Gatekeeper** | Phase gate verification (AI with human sign-off) |

### 4.3 Quality Standards

Every task must have:
- Owner (human/AI)
- Description
- Exit criteria (yes/no verification)
- Output artifact (file path)
- Prevention rules (from failure modes)

## 5. Gap Analysis: Existing vs Required

### 5.1 Existing v7 Playbook Covers

- ✅ Phases 0-12: Development lifecycle
- ✅ Audio thread safety patterns
- ✅ DSP implementation guidance
- ✅ Parameter management (APVTS, SmoothedValue)
- ✅ GUI implementation basics
- ✅ Testing frameworks
- ✅ Failure modes catalog (FM-01 to FM-23, EM-01 to EM-13)

### 5.2 PBCPB Must Add

- ❌ Commercial licensing integration
- ❌ Marketplace listing process
- ❌ Business plan development
- ❌ Marketing strategy (solo developer scale)
- ❌ Extended Sound Design KB (psychedelic/ambient specific)
- ❌ Bridge layer (sound design → DSP translation)
- ❌ Session boundary definitions
- ❌ Knowledge base architecture for 5 layers

### 5.3 Integration Strategy

1. **Inherit** v7 phases 0-12 as development core
2. **Extend** with commercial phases 13-16
3. **Enhance** with KB architecture and bridge layer
4. **Add** session boundary guidance

## 6. Technical Requirements Summary

| Requirement | Specification |
|-------------|---------------|
| JUCE version | 8.0.x |
| C++ standard | C++17 |
| Build system | CMake + FetchContent |
| Formats | VST3 (primary), AU, Standalone |
| Platform | Linux (v1.0), expandable |
| DAW testing | REAPER |
| Testing framework | Catch2 |
| Parameter system | AudioProcessorValueTreeState |
| Smoothing | SmoothedValue |
| Licensing | JUCE OnlineUnlock or MuseHub |

## 7. Success Criteria Validation

| Criterion | Playbook Coverage |
|-----------|-------------------|
| Working VST3 plugin | Phases 0-12 |
| Quality audits passed | Phase 9-12 |
| Licensing integrated | Phase 13-14 |
| Marketplace ready | Phase 14-15 |
| Business plan | Phase 16 |
| KB populated | Phases 2-16 |
| Reproducible | All phases have explicit gates |

## 8. Next Steps

Proceed to **Phase 2: KB Architecture & Phase Structure Design**

1. Define 5-layer KB schema
2. Map phases to KB population activities
3. Design bridge layer schema
4. Create phase structure for 17-20 phases
5. Define session boundaries per phase group