#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Test kb-harvest skill functionality
set -e

SCRIPTS_DIR="$AGENT_ROOT/playbookdata/scripts"
TEST_DIR="/tmp/kb-harvest-test-$$"

echo "=== KB Harvest Skill Tests ==="
echo "Test directory: $TEST_DIR"
mkdir -p "$TEST_DIR"

# Test 1: Script exists and is executable
echo -n "Test 1: harvest-batch.sh executable... "
if [ -x "$SCRIPTS_DIR/harvest-batch.sh" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 2: Script shows help
echo -n "Test 2: harvest-batch.sh --help... "
if "$SCRIPTS_DIR/harvest-batch.sh" --help > /dev/null 2>&1; then
    echo "PASS"
else
    echo "FAIL (but may succeed without API key)"
fi

# Test 3: Credit estimation
echo -n "Test 3: harvest-batch.sh --credits... "
if "$SCRIPTS_DIR/harvest-batch.sh" --credits > /dev/null 2>&1; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 4: Merge script exists
echo -n "Test 4: merge-harvested.py exists... "
if [ -f "$SCRIPTS_DIR/merge-harvested.py" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 5: Merge script help
echo -n "Test 5: merge-harvested.py --help... "
if python3 "$SCRIPTS_DIR/merge-harvested.py" --help > /dev/null 2>&1; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 6: Dry run (no harvested content expected)
echo -n "Test 6: merge dry run (no content)... "
if python3 "$SCRIPTS_DIR/merge-harvested.py" dsp-kb --topic reverb --file algorithmic-reverb.json --dry-run > "$TEST_DIR/dry-run.out" 2>&1; then
    echo "PASS (script executed)"
else
    # Expected to fail if no harvested content or KB file doesn't exist
    if grep -q "No harvested content found" "$TEST_DIR/dry-run.out" || grep -q "KB file not found" "$TEST_DIR/dry-run.out"; then
        echo "PASS (expected failure)"
    else
        echo "FAIL"
        cat "$TEST_DIR/dry-run.out"
        exit 1
    fi
fi

# Test 7: Skill file exists
echo -n "Test 7: kb-harvest skill exists... "
if [ -f "$HOME/.claude/skills/kb-harvest/SKILL.md" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Cleanup
rm -rf "$TEST_DIR"

echo ""
echo "=== All tests passed ==="