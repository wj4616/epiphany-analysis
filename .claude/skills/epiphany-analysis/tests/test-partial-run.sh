#!/usr/bin/env bash
# Contract test: SKILL.md documents --session-dir / --start-at / --stop-at correctly.
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

# Each module name must appear under STEP 2-7
for m in m-intake m-analyze m-ideate m-filter m-engineer m-integrate; do
    if grep -q "$m" "$SKILL_MD"; then
        echo "OK: orchestrator references $m"
    else
        echo "FAIL: orchestrator missing $m reference"; FAIL=1
    fi
done

# STEP 0 must document the requirement that partial-run flags need --session-dir
if grep -A5 "STEP 0" "$SKILL_MD" | grep -q "session-dir"; then
    echo "OK: STEP 0 documents session-dir requirement"
else
    echo "FAIL: STEP 0 does not document that --start-at/--stop-at require --session-dir"; FAIL=1
fi

exit $FAIL
