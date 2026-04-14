#!/usr/bin/env bash
# Structural assertions for epiphany-prompt skill files.
# Checks: SKILL.md exists with required sections; all 11 module files exist
# with valid frontmatter. Exits 0 on success, 1 on failure.

set -u
SKILL_DIR="$HOME/.claude/skills/epiphany-prompt"
FAIL=0

assert_file() {
    if [[ ! -f "$1" ]]; then
        echo "FAIL: missing file $1"
        FAIL=1
    fi
}

assert_grep() {
    # args: pattern, file, description
    if ! grep -qE "$1" "$2" 2>/dev/null; then
        echo "FAIL: $3 — pattern not found in $2"
        FAIL=1
    fi
}

# SKILL.md existence + required sections
assert_file "$SKILL_DIR/SKILL.md"
if [[ -f "$SKILL_DIR/SKILL.md" ]]; then
    assert_grep '^name: epiphany-prompt' "$SKILL_DIR/SKILL.md" "frontmatter name"
    assert_grep '^## Trigger Conditions' "$SKILL_DIR/SKILL.md" "trigger section"
    assert_grep '^## Hard Gates' "$SKILL_DIR/SKILL.md" "hard gates section"
    assert_grep '^## Orchestrator' "$SKILL_DIR/SKILL.md" "orchestrator section"
    assert_grep '^## FAST Inline Pipeline' "$SKILL_DIR/SKILL.md" "FAST inline section"
    assert_grep '^## Techniques' "$SKILL_DIR/SKILL.md" "techniques section"
    assert_grep '^## Preservation Methodology' "$SKILL_DIR/SKILL.md" "preservation section"
    assert_grep 'Mandatory Preservation Categories' "$SKILL_DIR/SKILL.md" "preservation categories"
    assert_grep 'Preservation Verification Protocol' "$SKILL_DIR/SKILL.md" "preservation verification"
    assert_grep 'Handling Overlapping Categories' "$SKILL_DIR/SKILL.md" "overlapping categories"
    assert_grep 'Handling Malformed Items' "$SKILL_DIR/SKILL.md" "malformed items"
    assert_grep 'STEP 0 — FLAG DETECTION' "$SKILL_DIR/SKILL.md" "STEP 0"
    assert_grep 'STEP 1 — INPUT ROUTING' "$SKILL_DIR/SKILL.md" "STEP 1"
    assert_grep 'STEP 2 — ANNOUNCE' "$SKILL_DIR/SKILL.md" "STEP 2"
    assert_grep 'STEP 3 — SUFFICIENCY CHECK' "$SKILL_DIR/SKILL.md" "STEP 3"
    assert_grep 'Type A — raw text' "$SKILL_DIR/SKILL.md" "type A detection"
    assert_grep 'Type B — prompt-epiphany XML' "$SKILL_DIR/SKILL.md" "type B detection"
    assert_grep 'Type C — prior epiphany-prompt output' "$SKILL_DIR/SKILL.md" "type C detection"
    assert_grep 'Mode routing signal' "$SKILL_DIR/SKILL.md" "mode routing signal"
    assert_grep 'STEP 4 — SESSION INIT' "$SKILL_DIR/SKILL.md" "STEP 4"
    assert_grep 'STEP 7 — OUTPUT' "$SKILL_DIR/SKILL.md" "STEP 7"
fi

# All 11 module files + frontmatter
MODULES=(
    m12-analysis-ideation.md
    m3-synthesis.md
    m4-verification.md
    m5-expansion.md
    m4m5-verify-output.md
    mspec12-domain-req.md
    mspec3-synthesis.md
    mspec4m5-verify-output.md
    mplan12-analysis-design.md
    mplan3-synthesis.md
    mplan4m5-verify-output.md
)
for m in "${MODULES[@]}"; do
    assert_file "$SKILL_DIR/modules/$m"
done

# Run frontmatter validator across all existing module files
EXISTING=()
for m in "${MODULES[@]}"; do
    [[ -f "$SKILL_DIR/modules/$m" ]] && EXISTING+=("$SKILL_DIR/modules/$m")
done
if (( ${#EXISTING[@]} > 0 )); then
    if ! "$SKILL_DIR/tests/validate-frontmatter.py" "${EXISTING[@]}"; then
        FAIL=1
    fi
fi

if (( FAIL == 0 )); then
    echo "PASS: all structural checks passed"
    exit 0
else
    echo "FAILURES detected (see above)"
    exit 1
fi
