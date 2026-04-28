# Success Criteria

> These criteria measure the quality of the **playbook itself**, not the plugins it produces. They must be measurable and objective.

## Primary Success Criteria

### 1. Executability
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| No clarifying questions needed | Count of user questions after phase start | 0 questions for primary path |
| Task completeness | Every task has owner, description, output | 100% tasks complete |
| Gate pass rate | Phases pass gates without revision | 90%+ first-pass rate |
| Handoff clarity | Next phase context is complete | No missing files |

**Test**: An unfamiliar user with C++ beginner knowledge can execute Phase 1-12 without asking "what do I do next?"

### 2. Completeness
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Knowledge base coverage | KB entries for every referenced concept | 100% referenced concepts have entries |
| Phase coverage | All 13 phases have complete checklists | All items have outputs defined |
| Failure mode prevention | FM referenced in each phase where relevant | 100% relevant FMs linked |
| Tool availability | All tools listed in compilation object | 100% tools documented |

**Test**: Every concept mentioned in the playbook can be looked up in the knowledge base.

### 3. Quality Bar
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Every task has owner | Role assigned to every item | 100% owner assignment |
| Every task has description | Clear instructions for every item | 100% description completeness |
| Every task has exit criteria | Known completion state for every item | 100% exit criteria defined |
| Every phase has gate | Gate conditions for all phases | 100% gate coverage |

**Test**: Checklist review shows no "TBD" or placeholder sections.

### 4. Pilot Test Criteria
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Time to first plugin | Hours from Phase 0 to working VST3 | < 40 hours total |
| Time to first sound | Hours from Phase 4 to audible output | < 10 hours |
| Bug rate in testing | Bugs found in Phase 9 DAW testing | < 5 critical bugs |
| User confidence | Self-reported confidence after completion | ≥ 7/10 confidence |

**Test**: Complete pilot run produces working plugin within time bounds.

---

## Secondary Success Criteria

### 5. Knowledge Integration
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Sound design coverage | KB covers all core techniques | Psychedelic, ambient, pad, creative all present |
| DSP algorithm sources | Every algorithm cites source | 100% algorithms attributed |
| UI design guidance | Modern UI principles documented | Layout theory, aesthetics, accessibility |
| Market strategies | Commercial guidance complete | Distribution, pricing, positioning all covered |

**Test**: Sound designer can find psybient technique without external research.

### 6. Cross-Platform Reliability
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Linux builds | CMake succeeds on Linux | First try |
| Windows builds | Cross-compile or native build succeeds | Documented process |
| macOS builds | Xcode build succeeds | Documented process |
| DAW compatibility | Plugin loads in REAPER, other DAWs | 3+ DAWs tested |

**Test**: Plugin builds and loads on all three platforms.

### 7. Commercial Readiness
| Criterion | Measurement | Pass Threshold |
|-----------|-------------|----------------|
| Release readiness | Plugin passes all validation steps | Phase 12 complete |
| Distribution path | Clear process for market release | Documented |
| Licensing clarity | License header on all files | 100% compliance |
| Versioning strategy | Clear update process | Documented |

**Test**: Plugin can be packaged for distribution.

---

## Failure Criteria (Blocking Issues)

The playbook **fails** if any of these occur:
- Cannot produce working VST3 plugin after pilot run
- Phase gate cannot pass without external resources beyond budget
- Knowledge base contains incorrect information that breaks implementation
- Audio thread safety violations in validated code
- Cannot resume after session break (decisions lost)

---

## Measurement Methods

### How Success is Measured

| Criterion Type | Measurement Method |
|----------------|-------------------|
| Count-based | Direct count of items (tasks, files, entries) |
| Time-based | Stopwatch from phase start to gate pass |
| Binary pass/fail | Gate conditions checked, pass or fail |
| Survey-based | User self-report after completion |
| Test-based | Automated or manual test execution |

### Who Measures

| Measurement | Responsible Party |
|-------------|-------------------|
| Phase gates | Coordinator role (automated where possible) |
| Time tracking | Metrics-tracker.md |
| Quality bar | Peer review or stakeholder |
| Pilot test | First-time user |
| Final validation | Stakeholder |

---

## Success Criteria Review

These criteria will be reviewed:
- After Phase 3 (KB bootstrapping) - knowledge coverage check
- After Phase 6 (Integration) - technical completeness check
- After Phase 9 (DAV testing) - quality check
- After Phase 12 (Release) - final validation

Criteria changes require stakeholder approval and Phase 0 revision.