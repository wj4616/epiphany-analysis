# KB Validate Integration Guide

## Installation

The skill is installed at: `~/.claude/skills/kb-validate/`

No additional installation required - skills are automatically recognized.

## Hook Configuration

To enable automatic validation, add hooks to `~/.claude/settings.json`:

### Post-Knowledge Hook (Automatic)

Add after existing hooks configuration:

```json
{
  "hooks": {
    "after": [
      {
        "matcher": "kb-harvest|kb-sync|firecrawl-scrape|firecrawl-search",
        "hooks": [
          {
            "command": "kb-validate --auto --source ${last_skill_output}",
            "timeout": 30000
          }
        ]
      }
    ]
  }
}
```

### Pre-Implementation Gate (Automatic)

Add to existing hooks configuration:

```json
{
  "hooks": {
    "before": [
      {
        "matcher": "Write|Edit",
        "condition": "file_pattern:*.cpp|*.h|*.js|*.py|*.ts",
        "hooks": [
          {
            "command": "kb-validate --gate --check-session",
            "timeout": 60000
          }
        ]
      }
    ]
  }
}
```

## Playbook Integration

Add validation task to [Auditor] role phases.

### Example: Phase 0 Specification

Add to Auditor tasks:

```json
{
  "title": "[Auditor] — Validate technical claims from specification",
  "description": "Before implementation, validate all technical claims from KB, playbooks, web sources. Use kb-validate to check confidence levels. Resolve any FAIL or unresolved LOW claims.",
  "output": "validation-report.md",
  "verification": [
    "No FAIL claims unresolved",
    "All LOW claims have user decision recorded",
    "Validation report generated"
  ]
}
```

### Example: Phase 4 DSP Implementation

Add to Auditor tasks:

```json
{
  "title": "[Auditor] — Validate DSP implementation claims",
  "description": "Validate all DSP knowledge claims before code generation. Check confidence levels for filter coefficients, algorithm implementations, and real-time safety.",
  "output": "validation-report.md (appended)",
  "verification": [
    "All filter design claims validated",
    "All real-time safety claims validated",
    "No FAIL claims blocking implementation"
  ]
}
```

## Manual Invocation

From any session:

```
/kb-validate --claim "SmoothedValue uses getNextValue() per sample"
```

From playbooks:

```markdown
[Auditor] validates all technical claims using kb-validate before implementation proceeds.
```

## Session Cache Behavior

- Cache is created fresh each session
- Validated claims are stored in memory
- No disk persistence
- Cache invalidated on source file changes
- Use `/kb-validate --refresh` to clear cache

## Confidence Levels and Actions

| Level | Score | Auto-Action | User Action |
|-------|-------|--------------|-------------|
| HIGH | >= 0.85 | Proceed | None required |
| MEDIUM | 0.60 - 0.84 | Proceed + warn | Review if desired |
| LOW | 0.40 - 0.59 | Auto-investigate | Review result |
| FAIL | < 0.40 | Block | Decision required |

## Troubleshooting

### Hook Not Triggering

Check:
1. Hook is in `~/.claude/settings.json`
2. Skill name matches exactly
3. Timeout is sufficient

### Validation Taking Too Long

The auto-investigate has limits:
- Max 3 full file reads
- Max 5 searches
- Max 60 seconds

If still slow, use `/kb-validate --claim "..."` for targeted validation.

### False Positive Blocks

If a claim is incorrectly blocked:
1. Check the investigation log
2. Use "Skip this claim" option
3. Provide clarification to improve scoring