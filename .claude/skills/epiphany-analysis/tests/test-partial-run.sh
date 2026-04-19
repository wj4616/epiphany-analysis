#!/usr/bin/env bash
# Contract test: SKILL.md documents --session-dir / --start-at / --stop-at correctly.
# v2.0: Updated for inline pipeline (Pre-Pipeline section replaces STEP 0)
set -u
SKILL_MD="$HOME/.claude/skills/epiphany-analysis/SKILL.md"
FAIL=0

for flag in "--session-dir" "--start-at" "--stop-at"; do
    if grep -q -- "$flag" "$SKILL_MD"; then
        echo "OK: SKILL.md documents $flag"
    else
        echo "FAIL: SKILL.md missing $flag documentation"; FAIL=1
    fi
done

# Each module name must appear in the Pipeline section
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    if grep -q "$m" "$SKILL_MD"; then
        echo "OK: pipeline references $m"
    else
        echo "FAIL: pipeline missing $m reference"; FAIL=1
    fi
done

# Flag Parsing section must document the requirement that partial-run flags need --session-dir
if grep -A5 "Flag Parsing" "$SKILL_MD" | grep -q "session-dir"; then
    echo "OK: Flag Parsing documents session-dir requirement"
else
    echo "FAIL: Flag Parsing does not document that --start-at/--stop-at require --session-dir"; FAIL=1
fi

exit $FAIL