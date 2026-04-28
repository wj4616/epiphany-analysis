# Session State Management

## Purpose

Session state persistence allows projects to be resumed across different agent sessions. This addresses the problem of losing context when starting fresh.

## Location

Session state files are stored in:
```
/home/myuser/agents/juce-agent/sessions/<project-name>-session.json
```

## File Format

```json
{
  "session_state": {
    "project_name": "my-synth-plugin",
    "created": "2026-03-29T10:00:00Z",
    "last_updated": "2026-03-29T14:30:00Z",
    "current_phase": 4,
    "phase_status": {
      "phase_0": {
        "name": "Plugin Concept & Specification",
        "status": "completed",
        "completed_tasks": ["Define plugin type", "Define sound identity", "Write spec"],
        "pending_tasks": [],
        "started": "2026-03-29T10:00:00Z",
        "completed": "2026-03-29T11:30:00Z"
      },
      "phase_4": {
        "name": "DSP Implementation",
        "status": "in_progress",
        "completed_tasks": ["Oscillator module", "Filter module"],
        "pending_tasks": ["Envelope module", "LFO module"],
        "started": "2026-03-29T13:00:00Z",
        "completed": null
      }
    },
    "artifacts": {
      "spec_file": "docs/superpowers/specs/2026-03-29-my-synth-plugin-design.md",
      "plan_file": "docs/superpowers/plans/2026-03-29-my-synth-plugin-implementation.md",
      "source_directory": "src/my-synth-plugin/",
      "build_directory": "build/"
    },
    "key_decisions": [
      "Using TPT filters for better frequency response",
      "Stereo detune for width without chorus"
    ],
    "pending_issues": [
      "Filter resonance causing click at high values"
    ],
    "creative_language": {
      "sound_identity": "Warm analog pad with subtle movement",
      "character_words": ["warm", "evolving", "analog", "wide"],
      "reference_plugins": ["Juno-106", "OB-8"],
      "genre_context": "Ambient, electronic"
    },
    "plugin_info": {
      "plugin_type": "synthesizer",
      "plugin_name": "WarmPadSynth",
      "formats": ["VST3", "AU"],
      "target_os": ["Linux", "macOS", "Windows"],
      "target_daw": "REAPER"
    },
    "session_history": [
      {
        "session_id": "sess-001",
        "date": "2026-03-29",
        "phase_worked": 0,
        "tasks_completed": ["Define plugin type", "Write spec"],
        "notes": "Completed Phase 0, moving to architecture"
      }
    ]
  }
}
```

## Usage

### Creating a New Session

1. Copy `SESSION_TEMPLATE.json` to `sessions/<project-name>-session.json`
2. Fill in `project_name`, `created`, `plugin_info`
3. Update `current_phase` as you progress

### Updating Session State

After each task completion:
1. Update `completed_tasks` in the current phase
2. Move tasks from `pending_tasks` to `completed_tasks`
3. Update `last_updated` timestamp
4. Add any `key_decisions` made
5. Log any `pending_issues` encountered

### Session History

Each session should add an entry to `session_history`:
```json
{
  "session_id": "sess-003",
  "date": "2026-03-30",
  "phase_worked": 4,
  "tasks_completed": ["Filter module", "Envelope module"],
  "notes": "Finished filter, started envelope"
}
```

### Resuming a Session

When starting fresh:
1. Read the session state file
2. Review `current_phase` and `phase_status`
3. Check `pending_tasks` for next steps
4. Review `key_decisions` for context
5. Check `pending_issues` for blockers
6. Review `creative_language` for original intent

## Integration with Skills

### Phase Skills

Each phase skill should:
1. Read session state at start
2. Update `completed_tasks` during work
3. Write session state at completion
4. Add session history entry

### Example Integration (juce-plugin-spec)

```markdown
## Session State Integration

Before starting Phase 0:
1. Check for existing session state file
2. If exists, offer to resume or start fresh
3. If starting fresh, create new session state

During Phase 0:
- Update session state after each major task
- Record key decisions
- Store creative language exactly as user says it

After Phase 0:
- Update phase status to "completed"
- Set next phase as "ready"
- Add session history entry
```

## Benefits

1. **Resume capability**: Continue from where you left off
2. **Context preservation**: Key decisions and creative language preserved
3. **Progress tracking**: See which tasks completed, which pending
4. **Issue tracking**: Blockers are documented
5. **Team collaboration**: State can be shared across sessions

## File Locking (Future)

For multi-user environments, consider:
- File-based locking before writes
- Merge conflict resolution
- Session ID tracking for concurrent sessions