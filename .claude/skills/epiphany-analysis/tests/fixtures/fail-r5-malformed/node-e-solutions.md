## E01 — Add error-handling section
**from_d_id:** D01
**target:** {"kind": "heading_path", "path": ["## Task"]}
**action:** append
**text:** |
  ## Error Handling

  Failures must be reported to stderr without aborting.
**rationale:** From D01; preserves both input-ordering and no-debug-output constraints.
