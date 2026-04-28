# Strategic Constraints

> These constraints become universal rules that every phase must respect. They are non-negotiable boundaries that shape all decisions.

## Technology Constraints

### Required Stack
| Component | Constraint | Rationale |
|-----------|------------|-----------|
| JUCE | Version 8.0.x | Latest stable, required features |
| C++ | C++17 minimum | Modern features without compatibility issues |
| CMake | 3.22+ | Cross-platform build, FetchContent support |
| VST3 SDK | 3.8.x | Current plugin standard |
| Compiler | GCC 7+ or Clang 6+ | Minimum C++17 support |

### Platform Requirements
| Platform | Status | Priority |
|----------|--------|----------|
| Linux | Primary | User development platform |
| Windows | Required | 70%+ of VST market |
| macOS | Required | Professional audio market |
| VST3 | Required | Cross-platform standard |
| AU | Conditional | macOS only, secondary |
| AAX | Conditional | Pro Tools market |

### Architecture Constraints
- **FetchContent for dependencies**: Self-contained builds, no global JUCE installation
- **No proprietary dependencies**: All dependencies must be freely available
- **Linux-first development**: Primary development on Linux Mint, test on Windows/macOS
- **REAPER for testing**: Primary DAW for plugin validation

---

## Methodology Constraints

### Development Approach
| Constraint | Enforcement |
|------------|-------------|
| Phase-based workflow | Phases must complete in order, gates must pass |
| Spec-first implementation | No DSP code without SPEC.md approval |
| Audio thread safety audit | Required before any phase gate passes |
| Human-only creative decisions | AI proposes, human decides on sound character |
| Incremental validation | Test after each phase, not just at end |

### Knowledge Integration Rules
| Constraint | Enforcement |
|------------|-------------|
| Source quality hierarchy | Academic > Professional > Tutorial > Forum |
| Cite sources | All DSP algorithms must cite original source |
| No commercial secrets | Don't document patented algorithms without attribution |
| Verify before harvest | Knowledge base entries must be verified |

### Code Quality Rules
| Constraint | Enforcement |
|------------|-------------|
| Explain C++ inline | Comments explaining C++ concepts for beginners |
| Readability over terseness | Favor clear code over clever code |
| Self-contained projects | No global installation requirements |
| Real hardware testing | Must test on actual DAW, not just unit tests |

---

## Budget/Resource Constraints

### Hard Limits
| Resource | Limit | Consequence |
|----------|-------|--------------|
| Claude AI subscription | $100-200/month maximum | Enforces efficient session usage |
| External purchases | $0 | No paid courses, libraries, or tools |
| Development timeline | Multiple sessions OK | Must support session continuity |
| Hardware | Existing Linux Mint workstation | No new hardware purchases |

### Soft Limits
| Resource | Guideline | Notes |
|----------|-----------|-------|
| Session length | Efficient token usage | Avoid redundant research |
| Knowledge harvest depth | Conservative by default | Harvest only what's needed |
| Code generation | Quality over quantity | Less code, better documented |

### Budget Allocation Strategy
| Category | Allocation | Notes |
|----------|------------|-------|
| Planning/Research | 20% of sessions | Phase 0-3 |
| Implementation | 50% of sessions | Phase 4-7 |
| Testing/Validation | 20% of sessions | Phase 8-9 |
| Polish/Release | 10% of sessions | Phase 10-12 |

---

## Timeline Constraints

### Project Structure
| Constraint | Details |
|------------|---------|
| Multi-session project | Single session continuity not required |
| Session handoff | Decisions-ledger.md must capture all decisions |
| Artifact persistence | All outputs tracked in artifact-manifest.md |
| Phase gates | Each phase gate creates checkpoint for resumption |

### Milestone Approach
| Milestone | Approach |
|-----------|----------|
| Phase gates | Natural stopping points |
| Knowledge base | Survives sessions via file persistence |
| Validation logs | Track progress across sessions |

---

## Compliance/Legal Constraints

### Intellectual Property
| Constraint | Enforcement |
|------------|-------------|
| Open source compatibility | All code must be compatible with chosen license |
| Attribution required | Cite all sources, even open source |
| No code copying | Understand and implement, don't copy-paste |
| License clarity | Every file must have license header |

### Audio Standards Compliance
| Constraint | Enforcement |
|------------|-------------|
| VST3 specification | Must comply with Steinberg guidelines |
| Audio safety | No clipping above 0dB without warning |
| CPU efficiency | Must not exceed reasonable CPU limits |
| Plugin validation | Must pass VST3 validator |

### Distribution Legalities
| Constraint | Enforcement |
|------------|-------------|
| Trademark clearance | Verify plugin name availability |
| License compatibility | Check JUCE GPL vs commercial |
| Patent awareness | Avoid patented algorithms or license properly |

---

## Constraint Precedence

When constraints conflict, this order applies:

1. **Audio thread safety** (cannot be violated)
2. **Budget limits** (hard cap)
3. **Platform requirements** (VST3 required)
4. **Code quality** (can be adjusted if necessary)
5. **Timeline** (flexible within budget)

---

## Constraint Review Triggers

Constraints should be reviewed if:
- Budget increases significantly
- New platform requirements emerge
- Legal/compliance requirements change
- Technology stack becomes obsolete
- User skill level changes dramatically

Any constraint changes require Phase 0 revision and stakeholder approval.