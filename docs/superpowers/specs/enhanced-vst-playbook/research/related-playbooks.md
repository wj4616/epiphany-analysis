# Related Existing Playbooks

> Documents playbooks that overlap, are upstream/downstream dependencies, or serve as reference/inspiration sources for the enhanced VST playbook.

---

## Primary Source Playbook

### [PLAYBOOK-001] VST Plugin Playbook v7 Unified
- **Path**: `/home/myuser/agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json`
- **Status**: Active, 3500+ lines, 13 phases
- **Relationship**: **Primary Source** - This is the playbook being enhanced
- **What It Covers**:
  - Complete 13-phase VST plugin development workflow
  - DSP implementation phases
  - GUI design phases
  - DAW testing phases
  - Audio thread safety audits
  - SmoothedValue audits
- **What It Lacks**:
  - Deep sound design knowledge (psychedelic, ambient, creative)
  - Modern UI design theory
  - Commercial market strategies
  - Academic/professional source citations
- **Action**: Audit, enhance, extend (Phase 1 will perform detailed audit)

---

## Knowledge Base Dependencies

### [KB-001] Sound Design Knowledge Base
- **Path**: `/home/myuser/agents/juce-agent/playbookdata/sound-design-kb/`
- **Status**: Active, partially populated
- **Relationship**: **Upstream KB** - Will be enhanced with psychedelic/ambient techniques
- **Current Coverage**:
  - `ambient-textures/psychedelic-ambient-sound-design.md` - Initial psychedelic content
- **Action**: Enhance with comprehensive sound design techniques

### [KB-002] C++ Knowledge Base
- **Path**: `/home/myuser/agents/juce-agent/playbookdata/cpp-kb/`
- **Status**: Active
- **Relationship**: **Upstream KB** - C++ concepts for audio programming
- **Current Coverage**:
  - `realtime-safety/overview.md`
  - `realtime-safety/atomic-audio.md`
  - `realtime-safety/lock-free-patterns.md`
  - `realtime-safety/pitfalls.md`
- **Action**: Preserve, reference in new playbook

### [KB-003] JUCE Reference KB
- **Path**: `/home/myuser/agents/juce-agent/playbookdata/references/`
- **Status**: Active
- **Relationship**: **Upstream KB** - Technical reference material
- **Current Coverage**:
  - `juce-api-reference.md`
  - `juce-failure-modes.md`
  - `juce-patterns-reference.md`
  - `juce-tooling-reference.md`
  - `juce-dsp-reference.md`
- **Action**: Preserve, potentially enhance with modern JUCE 8.x updates

---

## Reference Playbooks (Not Being Enhanced)

### [PLAYBOOK-002] JUCE VST Dev Playbook v3
- **Path**: `/home/myuser/Documents/playbookdata/juce-vst-dev-playbook-v3.json`
- **Status**: Historical reference
- **Relationship**: **Reference Only** - Earlier version, may have useful patterns
- **Action**: Mine for patterns not in v7

### [PLAYBOOK-003] Playbook Creator Playbook
- **Path**: `/home/myuser/Documents/playbookdev/playbook-creator-playbook.json`
- **Status**: Active (this playbook is being used to create the enhanced playbook)
- **Relationship**: **Meta-Playbook** - This is the playbook creating the playbook
- **Action**: Follow its phases exactly

### [PLAYBOOK-004] Web Dev Playbook
- **Path**: `/home/myuser/Documents/web-dev-playbook/final/playbook.json`
- **Status**: Reference
- **Relationship**: **Unrelated** - Web development, different domain
- **Action**: None - different domain

---

## Inspiration Sources

### [INS-001] JUCE Official Documentation
- **Source**: https://juce.com/learn/
- **Relationship**: **Upstream Authority** - Official framework documentation
- **Action**: Cite for all JUCE-specific patterns

### [INS-002] Steinberg VST3 SDK Documentation
- **Source**: https://steinbergmedia.github.io/vst3_doc/
- **Relationship**: **Upstream Authority** - Official plugin format specification
- **Action**: Cite for all VST3-specific patterns

### [INS-003] The Audio Programming Book
- **Source**: Academic DSP reference
- **Relationship**: **Authority** - Cited for DSP algorithms
- **Action**: Harvest techniques for KB

### [INS-004] Designing Sound
- **Source**: Andy Farnell
- **Relationship**: **Reference** - Sound design principles
- **Action**: Harvest for sound design KB

### [INS-005] KVR Developer Forum
- **Source**: https://www.kvraudio.com/forum/
- **Relationship**: **Community Reference** - Real-world developer experiences
- **Action**: Mine for common issues and solutions

---

## Gap Analysis Summary

| Area | Existing Coverage | Gap | Action |
|------|-------------------|-----|--------|
| Basic VST workflow | ✅ Complete | None | Preserve |
| DSP fundamentals | ✅ Good coverage | Missing academic citations | Add citations |
| Sound design basics | ⚠️ Partial | Missing psychedelic/ambient depth | Enhance |
| Sound design advanced | ❌ Minimal | Major gap | Create new KB entries |
| UI design theory | ⚠️ Partial | Missing modern aesthetics | Enhance |
| Market strategies | ❌ Missing | Major gap | Create new sections |
| Academic sources | ❌ Missing | Major gap | Add source citations |
| Professional sources | ❌ Missing | Major gap | Add source citations |

---

## Dependency Graph

```
playbook-creator-playbook.json
           │
           ▼
enhanced-vst-playbook/
           │
           ├── builds on ──► vst-plugin-playbook-v7-unified.json
           │
           ├── enhances ──► sound-design-kb/
           │
           ├── references ──► cpp-kb/
           │
           ├── references ──► juce-references/
           │
           └── creates new ──► market-strategies-kb/
```

---

## Next Steps

1. **Phase 1**: Detailed audit of vst-plugin-playbook-v7-unified.json
2. **Phase 1**: Mine PLAYBOOK-002 for useful patterns
3. **Phase 2**: Design KB architecture to integrate existing KBs
4. **Phase 3**: Harvest content from academic and professional sources