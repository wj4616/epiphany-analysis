#!/usr/bin/env bash
# Structural assertions for epiphany-analysis skill files.
# Exits 0 on success, 1 on failure.

set -u
SKILL_DIR="$HOME/.claude/skills/epiphany-analysis"
FAIL=0

assert_file() {
    if [[ ! -f "$1" ]]; then echo "FAIL: missing file $1"; FAIL=1; fi
}
assert_grep() {
    if ! grep -qE "$1" "$2" 2>/dev/null; then
        echo "FAIL: $3 — pattern '$1' not found in $2"; FAIL=1
    fi
}

# SKILL.md required sections
assert_file "$SKILL_DIR/SKILL.md"
if [[ -f "$SKILL_DIR/SKILL.md" ]]; then
    assert_grep '^name: epiphany-analysis' "$SKILL_DIR/SKILL.md" "frontmatter name"
    assert_grep '^trigger: /epiphany-analysis' "$SKILL_DIR/SKILL.md" "trigger"
    assert_grep '^## Trigger Conditions' "$SKILL_DIR/SKILL.md" "trigger section"
    assert_grep '^## Hard Gates' "$SKILL_DIR/SKILL.md" "hard gates"
    assert_grep '^## Orchestrator' "$SKILL_DIR/SKILL.md" "orchestrator"
    for step in 'STEP 0' 'STEP 1' 'STEP 2' 'STEP 3' 'STEP 4' 'STEP 5' 'STEP 6' 'STEP 7'; do
        assert_grep "^### $step" "$SKILL_DIR/SKILL.md" "$step"
    done
    assert_grep '^## Section-Tailoring Map' "$SKILL_DIR/SKILL.md" "map"
    assert_grep '^## Genius Detection' "$SKILL_DIR/SKILL.md" "detection"
    assert_grep '^## Termination Caps' "$SKILL_DIR/SKILL.md" "caps"
    assert_grep '^## Anti-Patterns' "$SKILL_DIR/SKILL.md" "anti-patterns"
fi

# All 6 module files exist
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    assert_file "$SKILL_DIR/modules/$m.md"
done

# Template exists
assert_file "$SKILL_DIR/templates/session.json.template"
if [[ -f "$SKILL_DIR/templates/session.json.template" ]]; then
    if ! python3 -c "import json; json.load(open('$SKILL_DIR/templates/session.json.template'))" 2>/dev/null; then
        echo "FAIL: session.json.template is not valid JSON"; FAIL=1
    fi
fi

# Frontmatter validator
if ! "$SKILL_DIR/tests/validate-frontmatter.py" "$SKILL_DIR/modules"/*.md > /dev/null; then
    echo "FAIL: module frontmatter invalid"
    "$SKILL_DIR/tests/validate-frontmatter.py" "$SKILL_DIR/modules"/*.md
    FAIL=1
fi

if (( FAIL == 0 )); then echo "ALL STRUCTURAL ASSERTIONS PASS"; fi
exit $FAIL
