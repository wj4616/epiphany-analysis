#!/usr/bin/env bash
# Structural assertions for epiphany-analysis skill files.
# v2.0: Updated for inline pipeline (all modules in SKILL.md, modules/ as reference archive)
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
    assert_grep '^## Pipeline' "$SKILL_DIR/SKILL.md" "pipeline section"
    # Pre-Pipeline sections
    assert_grep '^### Flag Parsing' "$SKILL_DIR/SKILL.md" "flag parsing"
    assert_grep '^### Session Init' "$SKILL_DIR/SKILL.md" "session init"
    assert_grep '^### Replay Contract' "$SKILL_DIR/SKILL.md" "replay contract"
    # Inline pipeline modules (M-INTAKE through M-INTEGRATE)
    for m in M-INTAKE M-ANALYZE M-IDEATE M-FILTER M-ENGINEER M-INTEGRATE; do
        assert_grep "^### $m" "$SKILL_DIR/SKILL.md" "$m section"
    done
    assert_grep '^## Section-Tailoring Map' "$SKILL_DIR/SKILL.md" "map"
    assert_grep '^## Genius Detection' "$SKILL_DIR/SKILL.md" "detection"
    assert_grep '^## Termination Caps' "$SKILL_DIR/SKILL.md" "caps"
    assert_grep '^## Anti-Patterns' "$SKILL_DIR/SKILL.md" "anti-patterns"
fi

# Reference module files still exist (archive)
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    assert_file "$SKILL_DIR/modules/$m.md"
done

# Reference archive README
assert_file "$SKILL_DIR/modules/README.md"

# Template exists
assert_file "$SKILL_DIR/templates/session.json.template"
if [[ -f "$SKILL_DIR/templates/session.json.template" ]]; then
    if ! python3 -c "import json; json.load(open('$SKILL_DIR/templates/session.json.template'))" 2>/dev/null; then
        echo "FAIL: session.json.template is not valid JSON"; FAIL=1
    fi
fi

# Frontmatter validator (validates reference module files, excludes README.md)
MODULE_FILES=$(find "$SKILL_DIR/modules" -maxdepth 1 -name 'm-*.md' | sort)
if ! "$SKILL_DIR/tests/validate-frontmatter.py" $MODULE_FILES > /dev/null; then
    echo "FAIL: module frontmatter invalid"
    "$SKILL_DIR/tests/validate-frontmatter.py" $MODULE_FILES
    FAIL=1
fi

if (( FAIL == 0 )); then echo "ALL STRUCTURAL ASSERTIONS PASS"; fi
exit $FAIL