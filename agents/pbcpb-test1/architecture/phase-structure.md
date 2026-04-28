# Phase Structure Design

## Overview

PBCPB uses a **17-phase structure** organized into 5 phase groups. This design inherits phases 0-12 from the existing JUCE v7 playbook and extends with commercial phases 13-16.

## Phase Groups

```
┌─────────────────────────────────────────────────────────────────┐
│  GROUP 1: FOUNDATION (Phases 0-3)                               │
│  Commission, Research, Architecture, Setup                      │
├─────────────────────────────────────────────────────────────────┤
│  GROUP 2: DEVELOPMENT (Phases 4-8)                             │
│  DSP, Parameters, Integration, GUI, Build Verification          │
├─────────────────────────────────────────────────────────────────┤
│  GROUP 3: VALIDATION (Phases 9-12)                             │
│  DAW Testing, Audits, Profiling, Release Prep                   │
├─────────────────────────────────────────────────────────────────┤
│  GROUP 4: COMMERCIAL (Phases 13-16)                            │
│  Licensing, Marketplace, Marketing, Launch                     │
├─────────────────────────────────────────────────────────────────┤
│  GROUP 5: POST-RELEASE (Phase 17+)                              │
│  Updates, Analytics, Iteration                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Phase Detail

### GROUP 1: FOUNDATION

#### Phase 0: Plugin Concept & Specification
| Attribute | Value |
|-----------|-------|
| **Owner** | Human |
| **Role** | Creative Director |
| **Duration** | 1-2 sessions |
| **Key Output** | SPEC.md with all creative decisions locked |
| **Skill** | brainstorming (if needed) |

**Tasks:**
1. Define plugin type (synth/effect/module/tool)
2. Define signal flow (human draws, AI implements)
3. Define sound identity in user's exact words
4. Define technical constraints
5. Define gain staging philosophy
6. Define preset theme and naming
7. Write one-page spec
8. Self-review spec for completeness

**Gate Conditions:**
- [ ] Plugin type defined
- [ ] Signal flow documented (diagram)
- [ ] Sound identity in user's words
- [ ] Technical constraints locked
- [ ] Gain staging philosophy explicit
- [ ] One-page spec written

**Prevention Rules (from v7):**
- AI NEVER proposes signal flow unprompted
- AI NEVER paraphrases sound identity words
- ALWAYS audit gain staging early
- ALWAYS verify formulas at boundaries

---

#### Phase 1: Performance Audit (if upgrading)
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Analyst |
| **Duration** | 1 session |
| **Key Output** | Performance baseline |
| **Skill** | executing-plans |

**Tasks:**
1. Benchmark existing code (if applicable)
2. Identify performance hotspots
3. Document baseline metrics

**Gate Conditions:**
- [ ] Baseline documented (or N/A for new project)

---

#### Phase 2: Architecture & Task Breakdown
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Architect |
| **Duration** | 1-2 sessions |
| **Key Output** | Architecture doc, implementation plan |
| **Skill** | writing-plans |

**Tasks:**
1. Design module structure
2. Define DSP chain architecture
3. Define KB architecture (5-layer)
4. Create implementation plan with task breakdown
5. Identify dependencies and critical path

**Gate Conditions:**
- [ ] Module structure defined
- [ ] DSP chain documented
- [ ] KB architecture defined
- [ ] Implementation plan written

---

#### Phase 3: Project Setup & Test Framework
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Builder |
| **Duration** | 1 session |
| **Key Output** | CMake project, test framework, directory structure |
| **Skill** | executing-plans |

**Tasks:**
1. Create CMakeLists.txt with FetchContent for JUCE
2. Set up Catch2 test framework
3. Create directory structure matching architecture
4. Create initial plugin stub (Processor + Editor)
5. Seed KB directories with placeholders
6. Verify build compiles

**Gate Conditions:**
- [ ] CMake project builds successfully
- [ ] Test framework runs (empty tests pass)
- [ ] Plugin stub loads in REAPER
- [ ] KB directories created

---

### GROUP 2: DEVELOPMENT

#### Phase 4: Core DSP Implementation
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | DSP Engineer |
| **Duration** | 3-6 sessions |
| **Key Output** | DSP modules implemented |
| **Skill** | juce-dsp-implementation |

**Tasks:**
1. Implement oscillator modules (if synth)
2. Implement filter modules
3. Implement effect modules
4. Implement modulation sources (LFO, envelopes)
5. Add audio thread safety audit
6. Add SmoothedValue for all parameters
7. Unit tests for each module

**Gate Conditions:**
- [ ] All DSP modules implemented per spec
- [ ] Audio thread safety audit passed
- [ ] Unit tests pass
- [ ] No allocations in processBlock

**Prevention Rules:**
- Use juce-audio-thread-audit skill before commit
- Use juce-smoothedvalue-audit skill after parameter work

---

#### Phase 5: Preset Data & State Management
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Integration Engineer |
| **Duration** | 1-2 sessions |
| **Key Output** | APVTS setup, preset save/load |

**Tasks:**
1. Configure AudioProcessorValueTreeState
2. Define parameter layout with ranges
3. Implement getStateInformation / setStateInformation
4. Create preset file format
5. Add factory presets
6. Implement preset browser (if applicable)

**Gate Conditions:**
- [ ] All parameters connected to APVTS
- [ ] Preset save/load works
- [ ] Factory presets validated

---

#### Phase 6: Processor Integration
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Integration Engineer |
| **Duration** | 1-2 sessions |
| **Key Output** | Integrated audio processor |

**Tasks:**
1. Connect DSP modules to Processor
2. Connect parameters to DSP (via SmoothedValue)
3. Implement processBlock with proper signal flow
4. Add gain staging between modules
5. Integration tests

**Gate Conditions:**
- [ ] Signal flow matches spec
- [ ] All parameters functional
- [ ] Integration tests pass

---

#### Phase 7: GUI / Editor Implementation
| Attribute | Value |
|-----------|-------|
| **Owner** | Human |
| **Role** | UI Designer |
| **Duration** | 2-4 sessions |
| **Key Output** | Plugin Editor with custom LookAndFeel |
| **Skill** | juce-ui-bridge |

**Tasks:**
1. Design layout (human decides, AI implements)
2. Create custom LookAndFeel class
3. Implement control components (knobs, sliders, buttons)
4. Add parameter attachments
5. Add meters/visualizations (if applicable)
6. Test on different screen sizes

**Gate Conditions:**
- [ ] Layout approved by human
- [ ] All controls functional
- [ ] Responsive scaling works

**Prevention Rules:**
- Human approves all aesthetic decisions
- Use user's exact color/font preferences

---

#### Phase 8: Build Verification & Cross-Platform
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Build Engineer |
| **Duration** | 1 session |
| **Key Output** | Verified release build |

**Tasks:**
1. Create Release build configuration
2. Verify plugin loads in REAPER
3. Test on different sample rates/buffer sizes
4. Create installer (optional)

**Gate Conditions:**
- [ ] Release build succeeds
- [ ] Plugin loads in REAPER
- [ ] Works at 44.1k, 48k, 96k
- [ ] Works with various buffer sizes

---

### GROUP 3: VALIDATION

#### Phase 9: DAW Testing & Sound Approval
| Attribute | Value |
|-----------|-------|
| **Owner** | Human |
| **Role** | Sound Designer |
| **Duration** | 1-2 sessions |
| **Key Output** | Approved sound, issue log |
| **Skill** | juce-daw-testing |

**Tasks:**
1. Human loads plugin in REAPER
2. Human tests all presets
3. Human tests parameter ranges
4. Human listens for clicks/pops
5. Human approves sound identity match
6. Log any issues for fixing

**Gate Conditions:**
- [ ] Human approves sound
- [ ] No clicks/pops at any parameter setting
- [ ] Presets sound correct

**Prevention Rules:**
- This is HUMAN-ONLY phase
- AI assists with debugging if issues found

---

#### Phase 10: Quality Audits
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | QA Engineer |
| **Duration** | 1 session |
| **Key Output** | Audit reports |

**Tasks:**
1. Run audio thread safety audit
2. Run SmoothedValue audit
3. Run memory leak check
4. Run CPU profiling
5. Document findings

**Gate Conditions:**
- [ ] Audio thread safety: PASS
- [ ] SmoothedValue: PASS
- [ ] No memory leaks
- [ ] CPU usage acceptable

---

#### Phase 11: Profiling & Optimization
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Performance Engineer |
| **Duration** | 1 session |
| **Key Output** | Optimized DSP |

**Tasks:**
1. Profile with real-time audio load
2. Identify bottlenecks
3. Optimize critical paths
4. Re-verify after optimization

**Gate Conditions:**
- [ ] CPU usage within budget (<5% typical)
- [ ] No performance regressions

---

#### Phase 12: Release Preparation
| Attribute | Value |
|-----------|-------|
| **Owner** | AI |
| **Role** | Release Engineer |
| **Duration** | 1 session |
| **Key Output** | Release candidate |

**Tasks:**
1. Version bump
2. Update changelog
3. Create release notes
4. Final build verification
5. Package for distribution

**Gate Conditions:**
- [ ] Version set
- [ ] Changelog updated
- [ ] Release build tested

---

### GROUP 4: COMMERCIAL

#### Phase 13: Licensing Integration
| Attribute | Value |
|-----------|-------|
| **Owner** | AI + Human |
| **Role** | Commercial Engineer |
| **Duration** | 2-3 sessions |
| **Key Output** | Licensed plugin |

**Tasks:**
1. Research licensing options (from Commercial KB)
2. Human selects licensing approach
3. Implement license validation
4. Create license generation workflow
5. Test license enforcement
6. Create demo/limited version (if applicable)

**Gate Conditions:**
- [ ] Licensing system functional
- [ ] License generation working
- [ ] Demo restrictions working (if applicable)

---

#### Phase 14: Marketplace Preparation
| Attribute | Value |
|-----------|-------|
| **Owner** | AI + Human |
| **Role** | Marketplace Manager |
| **Duration** | 2 sessions |
| **Key Output** | Marketplace listings |

**Tasks:**
1. Create product descriptions
2. Create product screenshots
3. Create demo video/audio
4. Prepare KVR listing
5. Prepare Plugin Boutique submission
6. Set pricing with launch discount

**Gate Conditions:**
- [ ] Product page ready
- [ ] Screenshots/video ready
- [ ] KVR listing submitted
- [ ] Pricing set

---

#### Phase 15: Marketing Strategy
| Attribute | Value |
|-----------|-------|
| **Owner** | AI + Human |
| **Role** | Marketing Manager |
| **Duration** | 1-2 sessions |
| **Key Output** | Marketing plan |

**Tasks:**
1. Define pre-launch activities
2. Define launch week activities
3. Define post-launch activities
4. Identify YouTube reviewers
5. Plan social media schedule
6. Create marketing content templates

**Gate Conditions:**
- [ ] Marketing plan documented
- [ ] Content templates ready
- [ ] Reviewer list prepared

---

#### Phase 16: Launch Execution
| Attribute | Value |
|-----------|-------|
| **Owner** | Human |
| **Role** | Product Manager |
| **Duration** | Launch week |
| **Key Output** | Live product |

**Tasks:**
1. Execute launch announcement
2. Send review copies
3. Post on forums/social
4. Monitor feedback
5. Address immediate issues

**Gate Conditions:**
- [ ] Product live on marketplace(s)
- [ ] Launch announcement sent
- [ ] Initial feedback addressed

---

### GROUP 5: POST-RELEASE

#### Phase 17: Updates & Analytics
| Attribute | Value |
|-----------|-------|
| **Owner** | Human + AI |
| **Role** | Product Manager |
| **Duration** | Ongoing |
| **Key Output** | Updates, analytics |

**Tasks:**
1. Monitor sales analytics
2. Collect user feedback
3. Plan updates
4. Implement fixes
5. Release updates

**Gate Conditions:**
- [ ] Analytics tracked
- [ ] Update process defined

---

## Session Boundary Definitions

### Tokens Per Session

| Session Type | Target Tokens | Max Tokens |
|--------------|---------------|------------|
| Planning | 40,000 | 60,000 |
| Implementation | 30,000 | 50,000 |
| Review/Audit | 20,000 | 40,000 |
| Commercial | 25,000 | 45,000 |

### Session Boundaries by Phase

| Phase | Sessions | Boundary Point |
|-------|----------|----------------|
| 0 | 1-2 | After spec approved |
| 1 | 0-1 | Skip if new project |
| 2 | 1-2 | After architecture approved |
| 3 | 1 | After build verified |
| 4 | 3-6 | Per DSP module completion |
| 5 | 1-2 | After preset system working |
| 6 | 1-2 | After integration tests pass |
| 7 | 2-4 | After layout approved + controls implemented |
| 8 | 1 | After release build verified |
| 9 | 1-2 | After human sound approval |
| 10 | 1 | After all audits pass |
| 11 | 1 | After performance verified |
| 12 | 1 | After release package ready |
| 13 | 2-3 | After licensing working |
| 14 | 2 | After listings submitted |
| 15 | 1-2 | After marketing plan approved |
| 16 | Launch | During launch week |
| 17 | Ongoing | Per update cycle |

### Session Handoff Protocol

Between sessions, AI must:
1. Document current progress in decisions-ledger.md
2. Update artifact-manifest.md with new files
3. List next steps for continuation
4. Note any blockers or decisions needed

---

## Phase Dependencies

```
Phase 0 ──► Phase 1 ──► Phase 2 ──► Phase 3
                                    │
                                    ▼
Phase 7 ◄── Phase 6 ◄── Phase 5 ◄── Phase 4
   │           │
   ▼           ▼
Phase 8 ──► Phase 9 ──► Phase 10 ──► Phase 11 ──► Phase 12
                                                        │
                                                        ▼
                                    Phase 16 ◄── Phase 15 ◄── Phase 14 ◄── Phase 13
                                        │
                                        ▼
                                    Phase 17 (ongoing)
```

---

## Role Assignments Summary

| Role | Phases | Primary Owner |
|------|--------|---------------|
| Creative Director | 0, 7, 9, 16 | Human |
| Architect | 2 | AI |
| Builder | 3, 5, 6 | AI |
| DSP Engineer | 4 | AI |
| UI Designer | 7 | Human |
| QA Engineer | 10 | AI |
| Performance Engineer | 11 | AI |
| Release Engineer | 12 | AI |
| Commercial Engineer | 13 | AI + Human |
| Marketplace Manager | 14 | AI + Human |
| Marketing Manager | 15 | AI + Human |
| Product Manager | 16, 17 | Human |

---

## Cross-Cutting Concerns per Phase

| Concern | Phases Applied |
|---------|-----------------|
| Audio Thread Safety | 4, 6, 10 |
| Gain Staging | 0, 4, 6, 9 |
| Parameter Smoothing | 4, 5, 10 |
| Human Creative Decisions | 0, 7, 9, 16 |
| KB Population | 2, 3, 4, 7, 13-15 |
| Session Handoff | All phases |