#!/usr/bin/env bash
# session-init.sh — epiphany-genius v1.1.0
# Usage: session-init.sh <session_dir>
# <session_dir> is the full absolute path to the session folder (trailing slash optional).
# If the directory already exists, appends -1, -2, ... suffix to avoid collision.
# Creates session directory and writes stages/session.md.
# Prints the resolved directory path (stdout line 1) for orchestrator to read back.
# Called by orchestrator STEP 2.

set -euo pipefail

RAW_SESSION_DIR="${1:?Usage: session-init.sh <session_dir>}"
# Strip trailing slash if present
SESSION_DIR="${RAW_SESSION_DIR%/}"

# Collision safety: if dir exists, append -N suffix
if [ -d "$SESSION_DIR" ]; then
  N=1
  while [ -d "${SESSION_DIR}-${N}" ]; do
    N=$((N+1))
  done
  SESSION_DIR="${SESSION_DIR}-${N}"
fi

SESSION_ID="$(basename "$SESSION_DIR")"
STAGES_DIR="${SESSION_DIR}/stages"

mkdir -p "${STAGES_DIR}"

TIMESTAMP=$(date '+%Y-%m-%dT%H:%M:%S')
START_EPOCH=$(date +%s)

cat > "${STAGES_DIR}/session.md" << EOF
# epiphany-genius Session Manifest

session_id: ${SESSION_ID}
timestamp: ${TIMESTAMP}
start_epoch: ${START_EPOCH}
skill_version: 1.1.0

## State (updated by orchestrator)
scale: TBD
input_type: TBD
depth_flag: TBD
flag_xml: false
flag_quiet: false
flag_verbose: false
flag_conjecture: false
flag_no_save: false
flag_resume: false
wave_plan: TBD
active_conditionals: []
stage_list: []
signals: []

## Cost observability (populated at end of run)
spawns_total: 0
wall_seconds: 0
retry_path_taken: none

## Output paths
session_dir: ${SESSION_DIR}/
stages_dir: ${STAGES_DIR}/
EOF

# Line 1 = resolved dir so the orchestrator can capture the final path
echo "${SESSION_DIR}"
echo "Session initialized: ${SESSION_DIR}"
echo "Stages directory: ${STAGES_DIR}"
