#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Test kb-autofill skill functionality
set -e

echo "=== KB Autofill Skill Tests ==="

# Test 1: Skill file exists
echo -n "Test 1: kb-autofill skill exists... "
if [ -f "$HOME/.claude/skills/kb-autofill/SKILL.md" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 2: Config file exists
echo -n "Test 2: kb-harvest-config.json exists... "
if [ -f "$HOME/.claude/kb-harvest-config.json" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 3: Config is valid JSON
echo -n "Test 3: Config is valid JSON... "
if python3 -c "import json; json.load(open('$HOME/.claude/kb-harvest-config.json'))" 2>/dev/null; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

# Test 4: Config has required fields
echo -n "Test 4: Config has required fields... "
python3 << 'EOF'
import json
with open('$HOME/.claude/kb-harvest-config.json') as f:
    config = json.load(f)

required = ['session_budget', 'budget_exceeded_action', 'auto_harvest_enabled']
for field in required:
    if field not in config:
        print(f"FAIL: missing {field}")
        exit(1)
print("PASS")
EOF

# Test 5: Manifest update script exists
echo -n "Test 5: update-manifests.py exists... "
if [ -f "$AGENT_ROOT/playbookdata/scripts/update-manifests.py" ]; then
    echo "PASS"
else
    echo "FAIL"
    exit 1
fi

echo ""
echo "=== All tests passed ==="