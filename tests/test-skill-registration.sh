#!/usr/bin/env bash
# Verify epiphany-analysis is discoverable by Claude Code.
# v2.0: Updated for inline pipeline (module sections in SKILL.md, not separate files)
set -u
SKILL_DIR="$HOME/.claude/skills/epiphany-analysis"
FAIL=0

[[ -d "$SKILL_DIR" ]] || { echo "FAIL: $SKILL_DIR does not exist"; exit 1; }
echo "OK: $SKILL_DIR exists"

first_line=$(head -1 "$SKILL_DIR/SKILL.md")
if [[ "$first_line" == "---" ]]; then
    echo "OK: SKILL.md starts with '---'"
else
    echo "FAIL: SKILL.md first line is '$first_line'"; FAIL=1
fi

for key in "name: epiphany-analysis" "trigger: /epiphany-analysis" "version:" "skill_path:"; do
    if grep -q "^$key" "$SKILL_DIR/SKILL.md"; then
        echo "OK: frontmatter contains '$key'"
    else
        echo "FAIL: frontmatter missing '$key'"; FAIL=1
    fi
done

# v2.0: Check for 6 inline module sections in SKILL.md instead of separate module files
count=$(grep -c '^### M-' "$SKILL_DIR/SKILL.md")
if (( count == 6 )); then
    echo "OK: 6 inline module sections in SKILL.md"
else
    echo "FAIL: expected 6 inline module sections, found $count"; FAIL=1
fi

# Reference module files still exist (archive)
count=$(find "$SKILL_DIR/modules" -maxdepth 1 -name '*.md' ! -name 'README.md' | wc -l)
if (( count == 6 )); then
    echo "OK: 6 reference module files in archive"
else
    echo "FAIL: expected 6 reference modules, found $count"; FAIL=1
fi

exit $FAIL