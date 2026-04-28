# Existing Playbook Audit: VST Plugin Playbook v7

**Source:** `/home/myuser/agents/juce-agent/playbooks/vst-plugin-playbook-v7-unified.json`

## KEEP (Strengths to Preserve)

### Phase Structure
- 13-phase workflow (0-12) is battle-tested and comprehensive
- Clear phase gates with explicit conditions
- Skill mapping per phase (brainstorming, writing-plans, executing-plans)
- Handoff blocks with next_phase_context

### Quality Mechanisms
- **Two-stage review process** — Spec compliance + Code quality catches 70%+ bugs
- **Prevention rules** — Real bugs from EM/CM sessions documented
- **Failure mode catalog** — FM-01 through FM-23, EM-01 through EM-16, CM-01 through CM-17
- **SmoothedValue audit checklist** — Critical for catching silent bugs

### Knowledge Base Integration
- Sound Design KB with bridge entries (subjective → parameter)
- UI KB for audio plugin interfaces
- DSP KB with algorithm references
- Failure Modes KB for audio thread safety

### Human-AI Division
- Clear separation: human makes creative decisions, AI implements
- "AI NEVER proposes signal flow unprompted" rule
- Sound identity must use user's exact words

### Testing Methodology
- DAW testing phase (Phase 9) — human perceptual testing
- Automated tests (Catch2, pluginval)
- Sample rate/buffer stress tests

## FIX (Weaknesses to Address)

### Session Boundaries
- **Issue:** No explicit token budget management per phase
- **Fix:** Add compilation blocks with context_load priorities

### Platform Coverage
- **Issue:** Linux-only (REAPER) for testing
- **Fix:** Document cross-platform considerations (macOS AU, Windows)

### State Persistence
- **Issue:** State save/restore sometimes treated as afterthought
- **Fix:** Elevate to mandatory checkpoint in Phase 5

### Parameter Management
- **Issue:** APVTS patterns scattered
- **Fix:** Consolidate into single reference section

## ADD (Gaps to Fill)

### Commercial & Licensing Phase
- **Gap:** No coverage of commercial licensing, anti-piracy
- **Add:** New phases for:
  - Licensing system selection (PACE iLok, eLicenser, custom)
  - License integration implementation
  - Trial/full version differentiation

### Marketplace Phase
- **Gap:** No coverage of marketplace listing, distribution
- **Add:** New phases for:
  - KVR Audio listing
  - Plugin Boutique submission
  - Direct sales setup

### Marketing Phase
- **Gap:** No coverage of marketing strategy
- **Add:** New phase for:
  - Pre-launch strategy
  - Launch activities
  - Post-launch marketing (solo developer scale)

### Business Plan Phase
- **Gap:** No business planning content
- **Add:** Phase for:
  - Pricing strategy
  - Competitive positioning
  - Revenue projections

### Reference KB Layer
- **Gap:** Academic/textbook references not indexed
- **Add:** Reference KB layer with:
  - Pirkle, Zölzer, Smith citations
  - Academic paper index
  - Tutorial/video references

## REMOVE (Outdated/Contradictory)

### Version-Specific References
- Some JUCE 7.x patterns need update to 8.0.x
- CM-01 (SmoothedValue.setRampLength) API changed

### Platform Dependencies
- Some macOS-specific notes may need refresh
- Windows build notes may be outdated

## Summary Matrix

| Category | Count | Examples |
|----------|-------|----------|
| KEEP | 8 | Phase structure, two-stage review, failure modes, Sound Design KB |
| FIX | 4 | Session boundaries, platform coverage, state persistence, APVTS consolidation |
| ADD | 5 | Commercial/licensing, marketplace, marketing, business plan, reference KB |
| REMOVE | 2 | JUCE 7.x patterns, outdated platform notes |

## Recommendations

1. **Extend to 17 phases** to include commercial/licensing, marketplace, and marketing phases
2. **Add compilation blocks** from playbook-creator-playbook for session management
3. **Expand KB architecture** to 5 layers (Technical, Sound Design, UI/UX, Commercial, Reference)
4. **Bridge layer enhancement** for Sound Design → Technical translation
5. **Business model integration** for solo developer economics