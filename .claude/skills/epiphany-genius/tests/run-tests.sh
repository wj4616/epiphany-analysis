#!/usr/bin/env bash
# epiphany-genius test runner — three tiers
#
# Tier 1: SKILL.md structure checks    (instant, always runs)
# Tier 2: pipeline validation           (runs validate-pipeline.sh — instant)
# Tier 3: gold-reference session replay (runs test-runner.sh on saved session — free)
#
# Usage:
#   ./run-tests.sh                                           # Tier 1 + 2
#   ./run-tests.sh --replay <session_dir> [SCALE] [CONDS]   # + Tier 3
#     SCALE: MINIMAL | STANDARD | DEEP  (default: STANDARD)
#     CONDS: comma-separated conditional stage IDs, or "none"  (default: none)
#
# After a full run, find the session dir at ~/docs/epiphany/genius/<session_id>/
# Example: ./run-tests.sh --replay ~/docs/epiphany/genius/abc123 DEEP S3.1

set -u

PASS=0; FAIL=0; SKIP=0
REPLAY_MODE=0
SESSION_DIR=""
SCALE="STANDARD"
CONDS="none"

# Parse args
if [[ "${1:-}" == "--replay" ]]; then
    REPLAY_MODE=1
    SESSION_DIR="${2:?--replay requires a session directory path}"
    SCALE="${3:-STANDARD}"
    CONDS="${4:-none}"
fi

SKILL_DIR="$HOME/.claude/skills/epiphany-genius"
SKILL_MD="$SKILL_DIR/SKILL.md"
INDEX="$SKILL_DIR/index.json"
SCRIPTS="$SKILL_DIR/scripts"
MODULES="$SKILL_DIR/modules"
KB="$SKILL_DIR/kb"

G=$'\033[32m'; R=$'\033[31m'; Y=$'\033[33m'; B=$'\033[1m'; N=$'\033[0m'

header() { echo; printf "${B}=== %s ===${N}\n" "$1"; }

pass_check() { printf "  ${G}✓${N} %s\n" "$1"; ((PASS++)); }
fail_check() { printf "  ${R}✗${N} %s\n" "$1"; ((FAIL++)); }
skip_check() { printf "  ${Y}○${N} %s — %s\n" "$1" "$2"; ((SKIP++)); }

check_contains() {
    local desc="$1" pattern="$2" file="$3" invert="${4:-0}"
    local matched=0
    grep -qF -- "$pattern" "$file" 2>/dev/null && matched=1
    local pass=$matched
    [[ $invert -eq 1 ]] && pass=$((1 - matched))
    if [[ $pass -eq 1 ]]; then pass_check "$desc"; else fail_check "$desc (missing: $pattern)"; fi
}

check_exists() {
    local desc="$1" path="$2"
    if [[ -e "$path" ]]; then pass_check "$desc"; else fail_check "$desc (missing: $path)"; fi
}

check_exec() {
    local desc="$1" path="$2"
    if [[ -x "$path" ]]; then pass_check "$desc"; else fail_check "$desc not executable: $path"; fi
}

# ════════════════════════════════════════════════════════════════════
# TIER 1 — STATIC: SKILL.md structure
# ════════════════════════════════════════════════════════════════════

header "Tier 1a — SKILL.md presence & frontmatter"
check_exists "SKILL.md exists"                                  "$SKILL_MD"
check_contains "name: epiphany-genius"                          "name: epiphany-genius"    "$SKILL_MD"
check_contains "version field present"                          "version:"                 "$SKILL_MD"
check_contains "trigger: /epiphany-genius"                      "/epiphany-genius"         "$SKILL_MD"
check_contains "skill_path documented"                          "skill_path:"              "$SKILL_MD"
check_contains "kb_base documented"                             "kb_base:"                 "$SKILL_MD"
check_contains "session_output_base documented"                 "session_output_base:"     "$SKILL_MD"

header "Tier 1b — SKILL.md orchestrator steps (STEP 0–9)"
for step in 0 1 2 3 4 5 6 7 8 9; do
    check_contains "STEP $step documented" "STEP $step" "$SKILL_MD"
done

header "Tier 1c — SKILL.md flags & modes"
for flag in --minimal --standard --deep --xml --quiet --verbose --conjecture --no-save --resume; do
    check_contains "$flag documented" "$flag" "$SKILL_MD"
done
check_contains "MINIMAL scale documented"           "MINIMAL"           "$SKILL_MD"
check_contains "STANDARD scale documented"          "STANDARD"          "$SKILL_MD"
check_contains "DEEP scale documented"              "DEEP"              "$SKILL_MD"
check_contains "CONJECTURE mode documented"         "CONJECTURE"        "$SKILL_MD"

header "Tier 1d — SKILL.md architecture rules"
check_contains "three-layer rule documented"        "Three-layer rule"  "$SKILL_MD"
check_contains "OSP documented"                     "OSP"               "$SKILL_MD"
check_contains "V1-V7 verification documented"      "V1–V7"             "$SKILL_MD"
check_contains "V4 retry documented"                "V4"                "$SKILL_MD"
check_contains "S3_thin_or_empty signal documented" "S3_thin_or_empty"  "$SKILL_MD"
check_contains "S6_no_alternatives signal documented" "S6_no_alternatives" "$SKILL_MD"
check_contains "wave-based execution documented"    "wave"              "$SKILL_MD"
check_contains "session dir structure documented"   "session"           "$SKILL_MD"
check_contains "test-runner.sh invocation documented" "test-runner.sh"  "$SKILL_MD"
check_contains "OSP spawn passes flag_verbose"      "flag_verbose"      "$SKILL_MD"
check_contains "OSP spawn passes scale"             "scale: {scale}"    "$SKILL_MD"
check_contains "optional_dependencies in spawn prompt" "optional_dependencies" "$SKILL_MD"
check_contains "context_budget enforcement in spawn" "context_budget_lines" "$SKILL_MD"
check_contains "session directory layout section"   "Session directory layout" "$SKILL_MD"

# ════════════════════════════════════════════════════════════════════
# TIER 1 — STATIC: file structure
# ════════════════════════════════════════════════════════════════════

header "Tier 1e — Module files"
for mod in \
    S1-state-loading.md \
    S2-constraint-escape.md \
    S3-peripheral-exploration.md \
    S3-1-defixation.md \
    S4-dynamic-simulation.md \
    S5-precision-forcing.md \
    S6-falsification.md \
    S6-1-conjecture.md \
    S7-integration-verification.md \
    output-synthesis-pass.md; do
    check_exists "modules/$mod" "$MODULES/$mod"
done

header "Tier 1f — Module required sections (from index.json)"
# Extract and check required_output_sections for each module
if command -v python3 &>/dev/null && [[ -f "$INDEX" ]]; then
    python3 - "$INDEX" "$MODULES" << 'PYEOF'
import json, sys
from pathlib import Path

index_path, modules_dir = sys.argv[1], sys.argv[2]
with open(index_path) as f:
    idx = json.load(f)

entries = idx.get("stages", []) + idx.get("conditional_modules", [])
fail = 0

for entry in entries:
    sid = entry["stage_id"]
    module_file = Path(modules_dir) / Path(entry["module_file"]).name
    sections = entry.get("required_output_sections", [])
    if not module_file.exists():
        print(f"  \033[31m✗\033[0m {sid}: module file missing — {module_file}")
        fail += 1
        continue
    content = module_file.read_text()
    for section in sections:
        if section.lower() in content.lower():
            print(f"  \033[32m✓\033[0m {sid}: section '{section}'")
        else:
            print(f"  \033[31m✗\033[0m {sid}: section '{section}' not found in {module_file.name}")
            fail += 1

sys.exit(fail)
PYEOF
    if [[ $? -eq 0 ]]; then ((PASS++)); else ((FAIL++)); fi
else
    skip_check "Module required sections" "python3 or index.json not available"
fi

header "Tier 1g — Scripts present & executable"
for script in session-init.sh test-runner.sh validate-pipeline.sh validate-stage.sh xml-assemble.sh; do
    check_exists   "scripts/$script exists"       "$SCRIPTS/$script"
    check_exec     "scripts/$script executable"   "$SCRIPTS/$script"
done

header "Tier 1h — index.json validity"
check_exists "index.json exists" "$INDEX"
if [[ -f "$INDEX" ]]; then
    if python3 -c "import json,sys; json.load(open(sys.argv[1]))" "$INDEX" 2>/dev/null; then
        pass_check "index.json is valid JSON"
    else
        fail_check "index.json is not valid JSON"
    fi

    # Version matches SKILL.md
    idx_version=$(python3 -c "import json,sys; print(json.load(open(sys.argv[1]))['version'])" "$INDEX" 2>/dev/null)
    if grep -qF -- "$idx_version" "$SKILL_MD"; then
        pass_check "index.json version ($idx_version) referenced in SKILL.md"
    else
        fail_check "index.json version ($idx_version) not found in SKILL.md"
    fi

    # Required top-level keys
    python3 - "$INDEX" << 'PYEOF'
import json, sys
with open(sys.argv[1]) as f: idx = json.load(f)
required = {"version","kb_base","session_output_base","mode_shift_after",
            "stages","conditional_modules","output_synthesis_pass","signals","scale_auto_detection"}
missing = required - set(idx.keys())
if missing:
    print(f"  \033[31m✗\033[0m index.json missing keys: {', '.join(sorted(missing))}")
    sys.exit(1)
else:
    print(f"  \033[32m✓\033[0m index.json has all required top-level keys")
PYEOF
    if [[ $? -eq 0 ]]; then ((PASS++)); else ((FAIL++)); fi
fi

# ════════════════════════════════════════════════════════════════════
# TIER 2 — PIPELINE VALIDATION (validate-pipeline.sh)
# ════════════════════════════════════════════════════════════════════

header "Tier 2 — Pipeline validation (DAG, KB files, activation grammar)"
if [[ -x "$SCRIPTS/validate-pipeline.sh" ]]; then
    pipeline_out=$("$SCRIPTS/validate-pipeline.sh" 2>&1)
    pipeline_exit=$?
    echo "$pipeline_out" | sed 's/^/  /'
    if [[ $pipeline_exit -eq 0 ]]; then
        pass_check "validate-pipeline.sh: all checks passed"
    else
        fail_check "validate-pipeline.sh: one or more checks failed"
    fi
else
    fail_check "validate-pipeline.sh not executable — cannot run pipeline validation"
fi

# ════════════════════════════════════════════════════════════════════
# TIER 3 — GOLD-REFERENCE REPLAY (test-runner.sh on saved session)
# ════════════════════════════════════════════════════════════════════

if [[ $REPLAY_MODE -eq 1 ]]; then
    header "Tier 3 — Gold-reference replay: $SESSION_DIR (scale=$SCALE, conds=$CONDS)"

    if [[ ! -d "$SESSION_DIR" ]]; then
        fail_check "Session directory not found: $SESSION_DIR"
    elif [[ ! -x "$SCRIPTS/test-runner.sh" ]]; then
        fail_check "test-runner.sh not executable — cannot replay"
    else
        replay_out=$("$SCRIPTS/test-runner.sh" "$SESSION_DIR" "$SCALE" "$CONDS" "distilled" 2>&1)
        replay_exit=$?
        echo "$replay_out" | sed 's/^/  /'
        if [[ $replay_exit -eq 0 ]]; then
            pass_check "test-runner.sh T1-T5: all checks passed"
        else
            fail_check "test-runner.sh T1-T5: one or more checks failed"
        fi

        # Also run XML mode replay if output.xml exists
        if [[ -f "$SESSION_DIR/stages/output.xml" ]]; then
            replay_xml=$("$SCRIPTS/test-runner.sh" "$SESSION_DIR" "$SCALE" "$CONDS" "xml" 2>&1)
            xml_exit=$?
            echo "$replay_xml" | sed 's/^/  /'
            if [[ $xml_exit -eq 0 ]]; then
                pass_check "test-runner.sh T5 (xml mode): all checks passed"
            else
                fail_check "test-runner.sh T5 (xml mode): one or more checks failed"
            fi
        else
            skip_check "T5 xml mode" "output.xml not found in session (run with --xml to generate)"
        fi
    fi
else
    echo
    printf "  ${Y}○${N} Tier 3 skipped — no --replay session provided\n"
    printf "    After a full run, replay with:\n"
    printf "    %s --replay ~/docs/epiphany/genius/<session_id> [SCALE] [CONDS]\n" "$0"
    ((SKIP++))
fi

# ── Summary ─────────────────────────────────────────────────────────
echo
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
printf "  ${G}PASS${N}: %-3d  ${R}FAIL${N}: %-3d  ${Y}SKIP${N}: %-3d\n" "$PASS" "$FAIL" "$SKIP"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

[[ $FAIL -eq 0 ]]
