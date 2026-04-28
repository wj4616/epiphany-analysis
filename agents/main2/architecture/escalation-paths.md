# Escalation Paths

## Overview

When a task is blocked, the escalation path defines who resolves it and how. In a single-agent system, escalation typically goes to the human.

## Escalation Categories

| Category | Examples | Primary Resolver | Secondary |
|----------|----------|-------------------|-----------|
| **Technical** | Build fails, algorithm unclear, bug unclear | [AI-Builder] → [Human] | Research KB |
| **Creative** | Sound identity unclear, GUI design choice | [AI-Architect] → [Human] | None |
| **Quality** | Review disagreement, perceptual quality | [AI-Auditor] → [Human] | Two-stage re-review |
| **Scope** | Feature request, out-of-scope item | [AI-Coordinator] → [Human] | decisions-ledger |
| **Resource** | Time, budget, access | [AI-Coordinator] → [Human] | None |
| **Business** | Pricing, marketplace choice | [Human] → [Human] | None (human decision) |

## Escalation Format

```markdown
## [Issue Type] Escalation

**Trigger**: What condition triggers escalation
**From**: Role that escalates
**To**: Role that resolves
**Format**: What information to provide
**Resolution Time**: Expected response time
**Documentation**: Where to record resolution
```

---

## Technical Escalation

**Trigger**:
- Build fails after reasonable debugging attempts
- Algorithm implementation unclear from KB
- Bug reproduction requires domain knowledge
- Performance issue unresolvable

**From**: [AI-Builder]

**To**: [Human]

**Format**:
1. Error message or symptom
2. Steps to reproduce
3. What was tried
4. What KB was consulted
5. Specific question

**Example**:
```
Technical Escalation: Filter instability at high resonance

Symptom: Filter oscillates at resonance > 0.9
Tried: Reduced Q, checked coefficient calculation
KB consulted: dsp-algorithms/filters, failure-modes-kb
Question: Is this expected behavior or implementation error?
```

**Resolution Time**: Same session if possible, next session otherwise

**Documentation**: `decisions-ledger.md` with "Technical Resolution" entry

---

## Creative Escalation

**Trigger**:
- Sound identity word ambiguous
- GUI design choice between options
- Preset naming unclear
- Aesthetic decision needed

**From**: [AI-Architect] or [AI-Builder]

**To**: [Human]

**Format**:
1. The decision needed
2. Options available
3. Trade-offs of each
4. Recommendation (with rationale)

**Example**:
```
Creative Escalation: GUI color scheme

Decision needed: Primary color for UI
Options:
1. Blue (#4A90D9) - Professional, common in audio plugins
2. Purple (#7B68EE) - Creative, matches psychedelic theme
3. Orange (#FF6B35) - Energetic, stands out

Trade-offs:
- Blue: Safe, but generic
- Purple: Matches sound identity, but may clash with DAW themes
- Orange: Distinctive, but may tire users

Recommendation: Purple - matches "psychedelic" sound identity
```

**Resolution Time**: Same session

**Documentation**: `decisions-ledger.md` with creative decision

**Critical Rule**: AI must NEVER make creative decisions autonomously. Always escalate to human.

---

## Quality Escalation

**Trigger**:
- Auditor and Builder disagree on issue severity
- Perceptual quality question (AI cannot perceive sound)
- Test failure unclear cause

**From**: [AI-Auditor]

**To**: [Human]

**Format**:
1. Issue description
2. Severity classification
3. Proposed fix
4. Disagreement point (if any)

**Example**:
```
Quality Escalation: Mix knob range

Issue: Mix knob only changes sound in first 30% of range
Severity: Important
Proposed fix: Use equal-power (cosine) mixing
Disagreement: AI-Auditor classifies as Important, AI-Builder classifies as Minor

Please advise: Is this Important or Minor?
```

**Resolution Time**: Next session

**Documentation**: `decisions-ledger.md` with severity decision

---

## Scope Escalation

**Trigger**:
- Feature requested not in spec
- Out-of-scope item proposed
- Scope creep detected

**From**: [AI-Coordinator]

**To**: [Human]

**Format**:
1. Proposed item
2. Current scope reference
3. Impact assessment
4. Recommendation

**Example**:
```
Scope Escalation: AAX format support

Proposed: Add AAX (Pro Tools) format
Current scope: VST3 only (Linux)
Impact: +1-2 sessions, requires AAX SDK, Windows/Mac build
Recommendation: Defer to v2.0 - stay focused on VST3

Decision: [ ] Accept  [ ] Defer  [ ] Reject
```

**Resolution Time**: Same session

**Documentation**: `decisions-ledger.md` with scope decision

---

## Resource Escalation

**Trigger**:
- Budget exceeded
- Timeline conflict
- Access/tool unavailable

**From**: [AI-Coordinator]

**To**: [Human]

**Format**:
1. Resource constraint
2. Impact on timeline
3. Options available

**Example**:
```
Resource Escalation: PACE iLok licensing budget

Constraint: PACE Indie program requires $5000/year
Current budget: $200/month Claude subscription
Impact: Cannot proceed with PACE licensing
Options:
1. Use open-source licensing alternative
2. Increase budget
3. Skip licensing for v1.0
```

**Resolution Time**: Same session

**Documentation**: `decisions-ledger.md` with resource decision

---

## Business Escalation

**Trigger**:
- Pricing strategy choice
- Marketplace selection
- Marketing budget allocation

**From**: [Human] (self-escalation for documentation)

**To**: [Human]

**Format**:
1. Decision needed
2. Options
3. Business impact
4. Recommendation

**Example**:
```
Business Escalation: Pricing strategy

Decision: Price point for v1.0
Options: $29, $49, $99
Business impact:
- $29: High volume, lower margin
- $49: Balanced
- $99: Premium positioning

Recommendation: $49 - matches indie developer market expectations
```

**Resolution Time**: Human decides

**Documentation**: `decisions-ledger.md` with business decision

---

## Blocker Resolution Flow

```
┌─────────────────┐
│ Task Blocked?   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Identify Type   │
│ (Technical,     │
│  Creative,      │
│  Quality, etc.) │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Attempt KB      │
│ Resolution      │
└────────┬────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
Resolved!   Still Blocked
    │              │
    ▼              ▼
Continue    ┌─────────────┐
            │ Escalate to │
            │   Human     │
            └──────┬──────┘
                   │
                   ▼
            ┌─────────────┐
            │ Human       │
            │ Decides     │
            └──────┬──────┘
                   │
                   ▼
            ┌─────────────┐
            │ Document in │
            │ Decisions   │
            │ Ledger      │
            └──────┬──────┘
                   │
                   ▼
               Continue
```

---

## Escalation Response Times

| Type | Expected Response | Max Wait |
|------|-------------------|----------|
| Technical | Same session | 1 day |
| Creative | Same session | Same session |
| Quality | Next session | 2 days |
| Scope | Same session | 1 day |
| Resource | Same session | 1 day |
| Business | Human decides | Human decides |

---

## Decisions Ledger Entries

All escalations resolved by human must be recorded:

```markdown
| Phase | Decision | Rationale | Alternatives Considered |
|-------|----------|-----------|-------------------------|
| 5 | Use cosine mixing for dry/wet | Equal-power prevents volume dip | Linear mixing (rejected) |
```

---

## Prevention Strategies

| Issue Type | Prevention |
|------------|-------------|
| Technical | Comprehensive KB, two-stage review |
| Creative | Clear spec, human approval gates |
| Quality | Two-stage review, automated tests |
| Scope | Explicit out-of-scope list in Phase 0 |
| Resource | Budget tracking in metrics |
| Business | Business plan in Phase 14 |