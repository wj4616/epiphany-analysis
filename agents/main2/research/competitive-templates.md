# Competitive Playbook/Template Audit

## Existing VST Development Resources

### 1. VST Plugin Playbook v7 (Primary Reference)

**Location:** `/home/myuser/agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json`

| Aspect | Evaluation |
|--------|------------|
| **Coverage** | Development phases 0-12, comprehensive failure modes |
| **Strengths** | Battle-tested, real bug patterns, two-stage review, sound design integration |
| **Weaknesses** | No commercial/licensing phases, no marketing strategy, Linux-only testing |
| **Detail Level** | High — specific code patterns, prevention rules |
| **Structure** | 13 phases, JSON format, skills integration |

**What to Keep:**
- Phase structure (0-12 workflow)
- Two-stage review process (spec compliance + code quality)
- Failure mode catalog (FM-xx, EM-xx, CM-xx)
- Sound Design KB architecture
- SmoothedValue audit checklist
- Audio thread safety patterns
- DAW testing methodology

**What to Add:**
- Commercial licensing phase(s)
- Marketing strategy phase
- Marketplace listing phase
- Business plan for solo developer
- Knowledge base expansion (Commercial, Reference layers)

---

### 2. JUCE Project Template (KristofferKarlAxelEkstrand)

**Location:** [GitHub](https://github.com/KristofferKarlAxelEkstrand/juce-project-template)

| Aspect | Evaluation |
|--------|------------|
| **Coverage** | Project scaffolding, CI/CD |
| **Strengths** | Modern C++20, CMake, Ninja, CI ready |
| **Weaknesses** | No DSP guidance, no process |
| **Detail Level** | Code template only |
| **Structure** | Single project template |

**What to Incorporate:**
- CMake FetchContent pattern
- Ninja build presets
- GitHub Actions CI/CD
- Dev Container support

---

### 3. ACE-Step VST3 Development (Recent 2026)

**Location:** [GitHub Issues](https://github.com/ace-step/ACE-Step-1.5/issues)

| Aspect | Evaluation |
|--------|------------|
| **Coverage** | WebView UI integration, state persistence |
| **Strengths** | Modern hybrid architecture, documented decisions |
| **Weaknesses** | Not a general-purpose guide |
| **Detail Level** | Project-specific |
| **Structure** | Issue-based documentation |

**What to Incorporate:**
- JUCE + WebView hybrid architecture option
- State persistence versioning
- Cross-platform build pipeline pattern

---

### 4. Plugin Architecture Patterns

**Location:** [LobeHub](https://lobehub.com/it/skills/yebot-rad-cc-plugins-plugin-architecture-patterns)

| Aspect | Evaluation |
|--------|------------|
| **Coverage** | Architecture patterns, testing |
| **Strengths** | Clean architecture separation, preset systems |
| **Weaknesses** | Less JUCE-specific |
| **Detail Level** | Pattern catalog |
| **Structure** | Skill definition |

**What to Incorporate:**
- DSP/Parameters/UI/State separation
- Modulation routing architectures
- Voice management for polyphonic synths
- Multi-format support patterns

---

## Gap Analysis

| Gap | Existing Coverage | New Playbook Need |
|-----|-------------------|-------------------|
| Commercial licensing | None | Full phase needed |
| Marketplace strategy | None | Full phase needed |
| Business plan | None | Full phase needed |
| Marketing (solo dev) | None | Full phase needed |
| Sound design KB | Good | Expand + add bridges |
| UI/UX KB | Partial | Expand with audio-specific |
| Technical KB | Good | Maintain + update |
| Reference KB | Partial | Curate academic/textbook refs |

---

## Template Selection

For the new playbook:

| Component | Source | Rationale |
|-----------|--------|-----------|
| Phase structure | VST Playbook v7 | Battle-tested 13-phase flow |
| Project setup | JUCE Project Template | Modern CMake, CI-ready |
| Failure modes | VST Playbook v7 | Comprehensive catalog |
| Architecture patterns | Plugin Architecture Patterns | Clean separation |
| Licensing/commercial | NEW | No existing coverage |
| Marketing | NEW | No existing coverage |

## Sources
- [JUCE Project Template](https://github.com/KristofferKarlAxelEkstrand/juce-project-template)
- [ACE-Step VST3 Development](https://github.com/ace-step/ACE-Step-1.5/issues)
- [Plugin Architecture Patterns](https://lobehub.com/it/skills/yebot-rad-cc-plugins-plugin-architecture-patterns)
- VST Plugin Playbook v7 (local)