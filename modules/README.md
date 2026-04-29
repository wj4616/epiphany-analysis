# Reference Archive

These files are **NOT** invoked at runtime.

The epiphany-analysis v2.0 pipeline runs all modules inline from `SKILL.md`.
These files are preserved for audit, migration verification, and frontmatter validation.

## Status

| File | Purpose |
|------|---------|
| `m-intake.md` | Reference copy of the M-INTAKE protocol (now inlined in SKILL.md) |
| `m-analyze.md` | Reference copy of the M-ANALYZE protocol |
| `m-ideate.md` | Reference copy of the M-IDEATE protocol |
| `m-filter.md` | Reference copy of the M-FILTER protocol |
| `m-engineer.md` | Reference copy of the M-ENGINEER protocol |
| `m-integrate.md` | Reference copy of the M-INTEGRATE protocol |

## Validation

`tests/validate-frontmatter.py` still validates these files against the frontmatter schema.
This ensures the reference copies remain structurally sound for audit purposes.