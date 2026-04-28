# Sound Design Validation Logs

This directory contains project-specific validation logs for the Sound Design Knowledge Base.

## Structure

- `global-patterns.json` — Aggregated patterns across all projects
- `project-name/` — Project-specific validation logs
  - `YYYY-MM-DD-preset-name.md` — Individual preset validation

## Log Format

Each validation entry contains:

- Date and project
- Preset name
- Parameters used
- Sonic description (expected)
- Sonic result (actual)
- Discrepancies found
- Adjustments made
- Outcome (approved/needs_revision)

## Purpose

Validation logs provide ground truth data for improving the Sound Design KB. They track:

1. Which translations work correctly
2. Which translations need refinement
3. Patterns in user feedback
4. Success rates for preset suggestions

## Usage

The juce-sound-design-bridge skill appends to these logs after each preset iteration. Do not edit manually.

## Integration

During Phase 0 (spec): Sound design bridge logs creative language for later reference.
During Phase 4 (implementation): Sound design context is verified against capability schema.
During Phase 9 (DAW testing): Quality criteria checklist is used for evaluation.