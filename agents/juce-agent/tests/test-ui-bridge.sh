#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: UI Bridge
# Tests that the UI bridge correctly translates UI concepts

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILL="$HOME/.claude/skills/juce-ui-bridge/SKILL.md"
UI_LOGS="$AGENT_ROOT/ui-validation-logs/"

echo "=== UI Bridge Integration Tests ==="
echo ""

# Test 1: Playbook exists and is valid JSON
echo "Test 1: Playbook JSON validity..."
if python3 -m json.tool "$PLAYBOOK" > /dev/null 2>&1; then
    echo "  ✓ Playbook is valid JSON"
else
    echo "  ✗ Playbook JSON is invalid"
    exit 1
fi

# Test 2: Skill file exists
echo "Test 2: Skill file exists..."
if [ -f "$SKILL" ]; then
    echo "  ✓ Skill file exists at $SKILL"
else
    echo "  ✗ Skill file missing"
    exit 1
fi

# Test 3: UI design section exists
echo "Test 3: UI design section in playbook..."
if python3 -c "import json; data=json.load(open('$PLAYBOOK')); assert 'ui_design' in data" 2>/dev/null; then
    echo "  ✓ UI design section exists"
else
    echo "  ✗ UI design section missing from playbook"
    exit 1
fi

# Test 4: UI design sections exist
echo "Test 4: UI design structure..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

ui_design = data.get("ui_design", {})
expected_sections = ["design_principles", "layout_patterns", "component_styles", "color_theory", "typography"]
missing = []
for section in expected_sections:
    if section not in ui_design:
        missing.append(section)

if missing:
    print(f"  ✗ Missing sections: {missing}")
    exit(1)
else:
    print("  ✓ All expected UI sections present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: UI translations exist
echo "Test 5: UI translations..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

translations = data.get("ui_design", {}).get("translations", {})
if "ui_concepts" in translations:
    print("  ✓ UI concept translations present")
else:
    print("  ✗ UI concept translations missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: UI validation logs directory exists
echo "Test 6: UI validation logs directory..."
if [ -d "$UI_LOGS" ]; then
    echo "  ✓ UI validation logs directory exists"
else
    echo "  ✗ UI validation logs directory missing"
    exit 1
fi

# Test 7: UI global patterns file exists
echo "Test 7: UI global patterns validity..."
if [ -f "$UI_LOGS/global-patterns.json" ]; then
    if python3 -m json.tool "$UI_LOGS/global-patterns.json" > /dev/null 2>&1; then
        echo "  ✓ UI global patterns is valid JSON"
    else
        echo "  ✗ UI global patterns JSON is invalid"
        exit 1
    fi
else
    echo "  ✗ UI global patterns file missing"
    exit 1
fi

# Test 8: Anti-patterns exist
echo "Test 8: UI anti-patterns..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

anti_patterns = data.get("ui_design", {}).get("anti_patterns", {})
if "entries" in anti_patterns or len(anti_patterns) > 0:
    print("  ✓ UI anti-patterns present")
else:
    print("  ✗ UI anti-patterns missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

echo ""
echo "=== All UI Bridge Tests Passed ==="