# Related Existing Playbooks

## Primary Reference: VST Plugin Playbook v7

**Location:** `/home/myuser/agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json`

**Relevance:** Direct predecessor. This is the existing JUCE VST development playbook that the new playbook will supersede or extend.

**What it covers:**
- 13-phase VST development workflow
- DSP implementation guidance
- Audio thread safety patterns
- JUCE-specific patterns
- Sound Design and UI Knowledge Base integration

**What to preserve:**
- Phase structure (0-12 workflow)
- Sound Design KB architecture
- UI KB architecture
- Audio thread safety patterns
- SmoothedValue audit patterns
- DAW testing methodology

**What to improve:**
- Add commercial/licensing phases
- Add marketing strategy phase
- Expand KB to include Commercial layer
- Better session boundary definitions
- More explicit complexity handling

## Secondary Reference: JUCE Agent System

**Location:** `/home/myuser/agents/juce-agent/`

**Relevance:** Skill definitions and validation infrastructure that the playbook should integrate.

**Components:**
- Skills: `juce-plugin-spec`, `juce-dsp-implementation`, `juce-daw-testing`, `juce-sound-design-bridge`, `juce-ui-bridge`
- Validation logs: `validation-logs/GROUND_TRUTH_PRESETS.md`
- Test scripts: `tests/` directory

**Integration approach:**
- Reference skills in phase task descriptions
- Use validation infrastructure for quality gates
- Incorporate ground truth presets as KB content

## Knowledge Base References

| KB | Location | Use |
|----|----------|-----|
| Sound Design KB | `playbookdata/sound-design-kb/` | Translation bridges |
| UI KB | `playbookdata/ui-kb/` | Interface patterns |
| Failure Modes KB | `playbookdata/failure-modes-kb/` | Audio thread safety |
| DSP KB | `playbookdata/dsp-kb/` | Algorithm references |

## Upstream Dependencies

- JUCE framework documentation
- VST3 SDK documentation
- C++17 reference (audio-relevant sections only)

## Downstream Consumers

- Individual VST plugin projects following this playbook
- Claude Code skills derived from playbook phases