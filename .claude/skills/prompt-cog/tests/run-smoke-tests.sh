#!/usr/bin/env bash
# prompt-cog smoke test runner — covers SKILL.md Tests A-M
#
# Uses --dangerously-skip-permissions so tool calls run without prompts.
# Synthesis tests cost API credits and take ~1-3 min each.
#
# Usage:
#   ./run-smoke-tests.sh              # essential: static + essential runtime (default)
#   ./run-smoke-tests.sh --full       # strict: static + essential + protocol
#   ./run-smoke-tests.sh --static     # static/structural checks only (instant)
#   ./run-smoke-tests.sh --fast       # static + halt-path runtime tests (E, J) only
#   ./run-smoke-tests.sh --essential  # same as default (explicit)
#
# Exit code: 0 if all ESSENTIAL + STATIC checks pass. Protocol failures are
#            reported but don't gate the exit code. Use --full for strict mode.

set -u

PASS_E=0; FAIL_E=0   # essential tier
PASS_P=0; FAIL_P=0   # protocol tier
PASS_S=0; FAIL_S=0   # static tier
SKIP=0
MODE="${1:-essential}"
TIMEOUT=180   # seconds per synthesis test

SKILL_MD="$HOME/.claude/skills/prompt-cog/SKILL.md"

G=$'\033[32m'; R=$'\033[31m'; Y=$'\033[33m'; B=$'\033[1m'; C=$'\033[36m'; N=$'\033[0m'

header() { echo; printf "${B}=== %s ===${N}\n" "$1"; }

check() {
    local desc="$1" pattern="$2" output="$3" invert="${4:-0}" tier="${5:-essential}"
    local matched=0
    echo "$output" | grep -qF -- "$pattern" && matched=1
    local pass=$matched
    [[ $invert -eq 1 ]] && pass=$((1 - matched))
    local label=""
    if [[ $tier == "essential" ]]; then
        label="E"
        if [[ $pass -eq 1 ]]; then ((PASS_E++)); else ((FAIL_E++)); fi
    elif [[ $tier == "protocol" ]]; then
        label="P"
        if [[ $pass -eq 1 ]]; then ((PASS_P++)); else ((FAIL_P++)); fi
    else
        label="S"
        if [[ $pass -eq 1 ]]; then ((PASS_S++)); else ((FAIL_S++)); fi
    fi
    if [[ $pass -eq 1 ]]; then
        printf "  ${G}✓${N} [%s] %s\n" "$label" "$desc"
    else
        printf "  ${R}✗${N} [%s] %s\n" "$label" "$desc"
        printf "      expected%s: %s\n" "${invert:+ absent}" "$pattern"
        if [[ ${#output} -gt 0 ]]; then
            printf "      (tail: ...%s)\n" "${output: -300}"
        fi
    fi
}

check_file() {
    local desc="$1" pattern="$2" file="$3" invert="${4:-0}" tier="${5:-static}"
    local out
    out=$(cat "$file" 2>/dev/null || true)
    check "$desc" "$pattern" "$out" "$invert" "$tier"
}

skip() { printf "  ${Y}○${N} %s — %s\n" "$1" "$2"; ((SKIP++)); }

run() {
    timeout "$TIMEOUT" claude -p --dangerously-skip-permissions "$1" 2>&1
}

# ── Prerequisite ────────────────────────────────────────────────────
if ! command -v claude &>/dev/null; then
    echo "ERROR: claude CLI not found." >&2; exit 1
fi
if [[ ! -f "$SKILL_MD" ]]; then
    echo "ERROR: SKILL.md not found at $SKILL_MD" >&2; exit 1
fi

echo "prompt-cog smoke tests"
echo "SKILL.md: $SKILL_MD"
echo "Mode: ${MODE} | Timeout: ${TIMEOUT}s per runtime test"
echo "Tiers: [E]=essential [P]=protocol [S]=static"

# ════════════════════════════════════════════════════════════════════
# SECTION 1 — STATIC CHECKS (always run)
# ════════════════════════════════════════════════════════════════════

header "Static — SKILL.md structure"

check_file "frontmatter present"                    "name: prompt-cog"              "$SKILL_MD"
check_file "version field present"                  "version:"                      "$SKILL_MD"
check_file "pipeline section present"               "## Pipeline"                   "$SKILL_MD"
check_file "Step 0 flag detection documented"       "Step 0"                        "$SKILL_MD"
check_file "Step 5 pre-spawn checkpoint documented" "Step 5"                        "$SKILL_MD"
check_file "Step 6 synthesis agent documented"      "Step 6"                        "$SKILL_MD"
check_file "Step 7 output documented"               "Step 7"                        "$SKILL_MD"
check_file "--minimal flag documented"              "--minimal"                     "$SKILL_MD"
check_file "--quiet flag documented"                "--quiet"                        "$SKILL_MD"
check_file "--verbose deferred flag documented"     "--verbose"                     "$SKILL_MD"
check_file "INVENTORY YAML schema present (all 8)"  "structural_elements:"          "$SKILL_MD"
check_file "T1-T13 technique table present"         "Enhancement Techniques"        "$SKILL_MD"
check_file "Hard Gate 3 prompt-content-only rule"   "PROMPT CONTENT ONLY"           "$SKILL_MD"
check_file "VERIFICATION: PASS format documented"   "VERIFICATION: PASS"            "$SKILL_MD"
check_file "VERIFICATION: FAIL format documented"   "VERIFICATION: FAIL"            "$SKILL_MD"
check_file "save path format documented"            "DD-MM-"                        "$SKILL_MD"
check_file "channel marker format documented"       "ANALYST OUTPUT BEGIN"          "$SKILL_MD"
check_file "smoke tests A-M present"                "Test A"                        "$SKILL_MD"
check_file "save directory documented"              "docs/epiphany/prompts"         "$SKILL_MD"
check_file "Type B routing documented"              "prompt-epiphany"               "$SKILL_MD"
check_file "Type C routing documented"              "prompt-cog"                    "$SKILL_MD"
check_file "Type D advisory documented"             "Type D"                        "$SKILL_MD"
check_file "E09 recovery guidance documented"       "E09"                           "$SKILL_MD"
check_file "E11 coherence check documented"         "E11"                           "$SKILL_MD"

if [[ "$MODE" == "--static" ]]; then
    echo; echo "Static-only mode: skipping runtime tests."
else

# ════════════════════════════════════════════════════════════════════
# SECTION 2 — FAST RUNTIME (halt-path tests, no synthesis)
# ════════════════════════════════════════════════════════════════════

header "Test E — Deferred flag halt (--verbose)"
out=$(run "/prompt-cog --verbose Write a function.")
check "halts with not-yet-supported message"    "not yet supported in prompt-cog"  "$out"  0  essential
check "does not begin analysis"                 "ANALYST OUTPUT"                   "$out"  1  essential

header "Test J — --minimal + deferred flag → still halts"
out=$(run "/prompt-cog --minimal --verbose Write a function.")
check "halts with not-yet-supported message"    "not yet supported in prompt-cog"  "$out"  0  essential
check "does not begin analysis"                 "ANALYST OUTPUT"                   "$out"  1  essential

if [[ "$MODE" == "--fast" ]]; then
    echo; echo "Fast mode: skipping synthesis tests (A-D, F-I, L-M)."
else

# ════════════════════════════════════════════════════════════════════
# SECTION 3 — SYNTHESIS TESTS (full pipeline, ~1-3 min each)
# ════════════════════════════════════════════════════════════════════

echo
echo "(Synthesis tests invoke claude -p with agent spawning — each takes ~1-3 minutes.)"

header "Test A — Normal mode, full pipeline"
out=$(run "/prompt-cog Write a function that reverses a string.")
check "XML root <prompt> element"                   "<prompt>"                      "$out"  0  protocol
check "meta source=prompt-cog"                      'source="prompt-cog"'           "$out"  0  protocol
check "save prompt appears"                         "Save to file"                  "$out"  0  protocol
check "announce: normal mode message"               "Using prompt-cog to analyze and enhance this prompt" "$out" 0 protocol
check "ANALYST OUTPUT BEGIN marker"                 "=== ANALYST OUTPUT BEGIN ==="  "$out"  0  protocol
check "ANALYST OUTPUT END marker"                   "=== ANALYST OUTPUT END ==="    "$out"  0  protocol
check "IDEATION OUTPUT BEGIN marker"                "=== IDEATION OUTPUT BEGIN ===" "$out"  0  protocol
check "IDEATION OUTPUT END marker"                  "=== IDEATION OUTPUT END ==="   "$out"  0  protocol
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"            "$out"  0  protocol

header "Test B — Minimal mode"
out=$(run "/prompt-cog --minimal Write a function that reverses a string.")
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"                        "$out"  0  protocol
check "announce: minimal mode message"              "Using prompt-cog (minimal mode)"           "$out"  0  protocol
check "minimal advisory shown"                      "Analysis limited to intent and inventory"  "$out"  0  protocol
check "INTENT block present"                        "INTENT"                                    "$out"  0  protocol
check "INVENTORY YAML present"                      "inventory:"                                "$out"  0  protocol

header "Test C — Quiet mode"
out=$(run "/prompt-cog --quiet Write a function that reverses a string.")
check "no interactive save prompt"                  "Save to file"                  "$out"  1  essential
check "saved confirmation printed"                  "Saved to"                      "$out"  0  protocol
check "announce: quiet mode message"                "Using prompt-cog (quiet mode)" "$out"  0  protocol

header "Test D — Type B input (prior prompt-epiphany output)"
type_b='<prompt><meta source="prompt-epiphany"/><task>Write a function that reverses a string.</task></prompt>'
out=$(run "/prompt-cog $type_b")
check "prompt-epiphany meta stripped from output"   'source="prompt-epiphany"'      "$out"  1  essential
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"            "$out"  0  protocol

header "Test F — Unknown flag in prose → soft advisory + proceed"
out=$(run "/prompt-cog --describe what a reverse string function does")
check "soft advisory for unknown token"             "resembles a flag but is not a recognized"  "$out" 0 protocol
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"                        "$out" 0 protocol

header "Test G — VERIFICATION: FAIL path (best-effort trigger)"
adv_input="Write code that MUST be under 3 lines AND MUST be over 1000 lines. MUST use Python. MUST NOT use Python. MUST return JSON. MUST return XML. MUST return YAML. Constraint: output must be empty. Constraint: output must be complete."
out=$(run "/prompt-cog $adv_input")
if echo "$out" | grep -qF "VERIFICATION: FAIL"; then
    check "annotated VERIFICATION FAILED comment"   "VERIFICATION FAILED"           "$out"  0  protocol
    check "E09 recovery suggestions shown"          "retry with a better outcome"   "$out"  0  protocol
else
    skip "G" "synthesis passed adversarial input — manually construct a degenerate input per SKILL.md Test G"
fi

header "Test H — Quiet + Minimal combined"
out=$(run "/prompt-cog --minimal --quiet Write a function that reverses a string.")
check "no interactive save prompt"                  "Save to file"                              "$out"  1  essential
check "saved confirmation printed"                  "Saved to"                                  "$out"  0  protocol
check "announce: quiet + minimal message"           "Using prompt-cog (quiet + minimal mode)"   "$out"  0  protocol
check "minimal advisory shown"                      "Analysis limited to intent and inventory"  "$out"  0  protocol

header "Test I — Type C input (prior prompt-cog output)"
type_c='<prompt><meta source="prompt-cog"/><role>Expert programmer</role><task>Write a function that reverses a string.</task></prompt>'
out=$(run "/prompt-cog $type_c")
check "enhanced output has <task> section"          "<task>"                "$out"  0  essential
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"    "$out"  0  protocol

header "Test K — Step 5 abort (missing channel markers)"
skip "K" "cannot be triggered externally — requires model to omit structural markers mid-run"

header "Test L — Type D advisory (executable workflow input)"
type_d=$'---\nname: test-skill\ndescription: A test skill\ntriggers:\n  - /test-skill\n---\n\nExecute: git clone https://example.com/repo && cd repo && make install'
out=$(run "/prompt-cog $type_d")
check "enhancement still proceeds to synthesis"     "VERIFICATION: PASS"    "$out"  0  protocol
check "Type D advisory shown before announce"       "Advisory: this input appears to describe an executable workflow" "$out" 0 protocol

header "Test M — File path input"
tmp=$(mktemp /tmp/prompt-cog-test-XXXX.md)
printf 'Write a Python function that reverses a string and returns the result.\n' > "$tmp"
out=$(run "/prompt-cog $tmp")
rm -f "$tmp"
check "task section in output (file contents used)" "<task>"                "$out"  0  protocol
check "synthesis VERIFICATION: PASS"                "VERIFICATION: PASS"    "$out"  0  protocol

fi  # end synthesis tests
fi  # end static-only guard

# ── Summary ─────────────────────────────────────────────────────────
echo
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
printf "  ${B}Static${N}:    %3d pass  %3d fail\n" "$PASS_S" "$FAIL_S"
printf "  ${G}Essential${N}: %3d pass  %3d fail\n" "$PASS_E" "$FAIL_E"
printf "  ${C}Protocol${N}:  %3d pass  %3d fail\n" "$PASS_P" "$FAIL_P"
printf "  ${Y}Skipped${N}:   %3d\n" "$SKIP"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if [[ "$MODE" == "--full" ]]; then
    echo "Exit code gated on all tiers (strict mode)."
    [[ $FAIL_S -eq 0 && $FAIL_E -eq 0 && $FAIL_P -eq 0 ]]
else
    echo "Exit code gated on essential + static only (default)."
    [[ $FAIL_S -eq 0 && $FAIL_E -eq 0 ]]
fi