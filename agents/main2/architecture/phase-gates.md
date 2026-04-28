# Phase Gates

## Gate Design Principles

1. **Verifiable**: Must be TRUE/FALSE, not subjective
2. **Complete**: All conditions must pass
3. **Blocker Examples**: Provide specific scenarios that prevent advancement
4. **Deliverables**: List what files/artifacts must exist

## Gate Format

```markdown
## Phase N: [Name] Gate

**Conditions**:
- [ ] Condition 1 (verifiable)
- [ ] Condition 2 (verifiable)
- [ ] Deliverables exist: file1.md, file2.cpp

**Blocker Examples**:
- Specific scenario that would block
- Another scenario

**Handoff**:
- Output artifacts: [list]
- Next phase context: [what to load]
- Excluded context: [what not to carry]
```

---

## Phase 0: Commission & Scoping Gate

**Conditions**:
- [ ] Purpose statement is one paragraph with no ambiguity
- [ ] Scope has explicit in/out/adjacent lists
- [ ] Success criteria are measurable, not subjective
- [ ] Strategic constraints documented
- [ ] Complexity profile documented in decisions-ledger.md

**Blocker Examples**:
- Purpose says "improve the process" without specifying which process
- Scope has no out-of-scope list
- Success criteria say "playbook is good" without metrics

**Deliverables**:
- README.md
- scope.md
- constraints.md
- success-criteria.md
- decisions-ledger.md
- artifact-manifest.md

---

## Phase 1: Domain Research Gate

**Conditions**:
- [ ] Domain analysis covers standard processes, failure modes, terminology
- [ ] Best practices documented with sources
- [ ] Cross-cutting concerns identified (minimum 5)
- [ ] Requirements document synthesizes all research
- [ ] Existing playbook audit complete (if replacing)

**Blocker Examples**:
- Domain analysis missing compliance requirements
- No sources cited for best practices
- Requirements has "TBD" sections

**Deliverables**:
- research/domain-analysis.md
- research/best-practices.md
- research/competitive-templates.md
- research/cross-cutting-concerns.md
- research/requirements.md

---

## Phase 2: Specification Gate

**Conditions**:
- [ ] Plugin type defined (synth/effect/analyzer/tool)
- [ ] Signal flow documented in user's words
- [ ] All parameters listed with: name, range, default, skew, type
- [ ] Sound identity documented (NOT paraphrased)
- [ ] Gain staging philosophy explicit
- [ ] Preset theme chosen with user-approved names
- [ ] SPEC.md self-review passed (no placeholders, no contradictions)
- [ ] Human approval on all creative decisions

**Blocker Examples**:
- Signal flow proposed by AI instead of user-defined
- Parameter missing range or default
- Sound identity paraphrased instead of user's exact words
- Gain staging not documented

**Deliverables**:
- docs/SPEC.md
- Parameter spreadsheet or table

---

## Phase 3: Architecture Gate

**Conditions**:
- [ ] Class diagram documented
- [ ] Signal flow diagram matches SPEC
- [ ] All module interfaces defined (prepare, reset, process)
- [ ] Dependency order documented
- [ ] Every spec requirement has corresponding task
- [ ] Task list complete with code snippets

**Blocker Examples**:
- Architecture missing module interface
- Signal flow doesn't match specification
- Tasks not ordered by dependency

**Deliverables**:
- architecture/ARCHITECTURE.md
- architecture/dependency-map.md
- architecture/task-list.md

---

## Phase 4: Project Setup Gate

**Conditions**:
- [ ] CMakeLists.txt configured for all target formats
- [ ] Build succeeds on primary platform (Linux)
- [ ] Test framework integrated (Catch2 or equivalent)
- [ ] Basic tests pass (instantiation, silence, noise)
- [ ] Plugin loads in REAPER

**Blocker Examples**:
- Build fails with unresolved symbols
- Tests fail with segfault
- Plugin doesn't appear in REAPER

**Deliverables**:
- CMakeLists.txt
- tests/ directory with basic tests
- Build output (.vst3 file)

---

## Phase 5: DSP Implementation Gate

**Conditions**:
- [ ] Each module passed two-stage review (spec + code quality)
- [ ] All critical and important issues resolved
- [ ] Fix subagents re-reviewed after changes
- [ ] Automated tests pass
- [ ] Build succeeds with zero errors

**Blocker Examples**:
- Module passed spec review but failed code quality review
- Buffer overflow found at boundary condition
- Formula wrong at param=0 or param=1

**Deliverables**:
- Source files for each module
- Unit tests for each module
- Review log documenting two-stage process

---

## Phase 6: State & Presets Gate

**Conditions**:
- [ ] State save/restore round-trip test passes
- [ ] Preset count consistent across PresetData, APVTS, PresetManager
- [ ] Morph interpolation uses log-space for frequencies
- [ ] Morph interpolation uses cosine for mix
- [ ] Automated state test passes

**Blocker Examples**:
- State not restored after DAW save/reload
- Preset count mismatch between files
- Linear interpolation for frequency parameters

**Deliverables**:
- PresetData struct
- PresetManager implementation
- State save/restore implementation

---

## Phase 7: Processor Integration Gate

**Conditions**:
- [ ] All parameters match spec ranges
- [ ] SmoothedValue audit: every SmoothedValue has getNextValue() per-sample
- [ ] Feedback loop has safety limiter
- [ ] getTailLengthSeconds() matches max decay
- [ ] Equal-power dry/wet mixing implemented
- [ ] Default decay <= 2.0s
- [ ] Build succeeds with zero errors
- [ ] Automated tests pass

**Blocker Examples**:
- SmoothedValue declared but getNextValue() never called
- Feedback gain can reach 1.0
- Linear dry/wet mixing

**Deliverables**:
- PluginProcessor.cpp with complete processBlock
- Parameter layout in APVTS

---

## Phase 8: GUI Implementation Gate

**Conditions**:
- [ ] All parameters have GUI controls with APVTS attachments
- [ ] Custom LookAndFeel applied
- [ ] All controls visible and within bounds
- [ ] Version label visible
- [ ] Human has approved visual design
- [ ] Build succeeds

**Blocker Examples**:
- Control overflows parent panel
- LookAndFeel not applied to all components
- Bounds arithmetic error

**Deliverables**:
- PluginEditor.h/cpp
- LookAndFeel class
- Component files

---

## Phase 9: Build Verification Gate

**Conditions**:
- [ ] Linux build succeeds with zero errors
- [ ] Cross-platform builds succeed (or CMakeLists verified)
- [ ] Sample rate tests pass (44.1k, 48k, 96k, 192k)
- [ ] Buffer size tests pass (64, 128, 256, 512, 1024, 2048)
- [ ] Extreme parameter test passes
- [ ] Multi-instance test passes (4+ instances)
- [ ] State save/restore works in DAW
- [ ] All automated tests pass
- [ ] Git tagged

**Blocker Examples**:
- Crash at 192kHz
- Static state shared across instances
- State not restored after DAW reload

**Deliverables**:
- Test results log
- Git tag
- Build artifacts for all platforms

---

## Phase 10: DAW Testing Gate

**Conditions**:
- [ ] Plugin loads in REAPER without crashing
- [ ] EVERY control produces audible/visible change when moved
- [ ] Mix knob effective across full range (not bunched at one end)
- [ ] Parameter changes audible within ~2 seconds
- [ ] ALL presets auditioned — usable ones approved
- [ ] Automation recording/playback works
- [ ] Bypass and freeze work correctly
- [ ] No audio glitches, clicks, or dropouts
- [ ] Human has approved the sound

**Blocker Examples**:
- DRIVE knob produces no change (SmoothedValue not advancing)
- Mix knob only changes in first 30% of range (linear instead of equal-power)
- Control delayed by 4+ seconds (default decay too long)

**Deliverables**:
- Bug reports (if any)
- Approved preset list
- Sound sign-off from human

---

## Phase 11: Licensing Integration Gate

**Conditions**:
- [ ] Licensing system compiles and links
- [ ] Trial version expires correctly
- [ ] Full version activates correctly
- [ ] License check doesn't block audio thread
- [ ] No license leaks in demo mode

**Blocker Examples**:
- License check allocates memory on audio thread
- Trial doesn't expire
- Full version doesn't activate after license entry

**Deliverables**:
- Licensed plugin build
- License integration code
- Test results

---

## Phase 12: Optimization Gate

**Conditions**:
- [ ] CPU profiled — top hotspots addressed
- [ ] No heap allocations in processBlock
- [ ] Denormal stress test passes (60s silence, CPU stable)
- [ ] Sound verified unchanged after optimization
- [ ] All automated tests still pass

**Blocker Examples**:
- New allocation in processBlock after optimization
- Sound changed after optimization
- Denormal CPU spike on silence

**Deliverables**:
- Profile report
- Optimization notes
- Build after optimization

---

## Phase 13: Marketplace Preparation Gate

**Conditions**:
- [ ] Product page content complete (description, features, requirements)
- [ ] Screenshots captured and edited
- [ ] Audio demos prepared
- [ ] Marketplace accounts created (KVR, Plugin Boutique)
- [ ] Pricing documented and competitive

**Blocker Examples**:
- Screenshots missing or poor quality
- Pricing not documented
- No marketplace account

**Deliverables**:
- Product page content
- Screenshots
- Audio demos
- Marketplace account credentials

---

## Phase 14: Marketing Strategy Gate

**Conditions**:
- [ ] Pre-launch strategy documented
- [ ] Launch day activities listed
- [ ] Post-launch marketing plan documented
- [ ] All activities achievable by solo developer
- [ ] Budget under $500 (excluding Claude)

**Blocker Examples**:
- Marketing plan requires team
- Activities exceed budget
- Strategy is vague ("do marketing")

**Deliverables**:
- marketing/strategy.md

---

## Phase 15: Release Gate

**Conditions**:
- [ ] Binary at project root (or distributed)
- [ ] Documentation updated (README, user guide)
- [ ] All commits and tag pushed
- [ ] Release announced (forum, social, etc.)

**Blocker Examples**:
- Git push rejected (remote diverged)
- Documentation outdated
- Binary missing

**Deliverables**:
- Release binary
- Updated documentation
- Release announcement

---

## Phase 16: Retrospective Gate

**Conditions**:
- [ ] Process retrospective documented
- [ ] Reusable components extracted
- [ ] Playbook updated with lessons learned
- [ ] Next version scope captured (if applicable)

**Blocker Examples**:
- No lessons documented
- Playbook not updated
- Reusable components left in project

**Deliverables**:
- retrospective.md
- Updated playbook
- Extracted components