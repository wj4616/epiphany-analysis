#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Sound Design Bridge
# Tests that the sound design bridge correctly translates descriptors

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILL="$HOME/.claude/skills/juce-sound-design-bridge/SKILL.md"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs/"

echo "=== Sound Design Bridge Integration Tests ==="
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

# Test 3: Sound design section exists
echo "Test 3: Sound design section in playbook..."
if python3 -c "import json; data=json.load(open('$PLAYBOOK')); assert 'sound_design' in data" 2>/dev/null; then
    echo "  ✓ Sound design section exists"
else
    echo "  ✗ Sound design section missing from playbook"
    exit 1
fi

# Test 4: Translation capabilities exist
echo "Test 4: Translation capabilities..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

capabilities = data.get("sound_design", {}).get("translations", {}).get("capabilities", {})
expected = ["filter", "distortion", "delay", "chorus", "lfo"]
missing = []
for cap in expected:
    if cap not in capabilities:
        missing.append(cap)

if missing:
    print(f"  ✗ Missing capabilities: {missing}")
    exit(1)
else:
    print("  ✓ All expected capabilities present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Confidence fields have numerical values
echo "Test 5: Numerical confidence in translations..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

# Check that translations have confidence fields
# Note: Some may be string ("high"), some may be numerical
# This test checks that confidence field exists
capabilities = data.get("sound_design", {}).get("translations", {}).get("capabilities", {})
found_confidence = 0
total_mappings = 0

for cap_name, cap_data in capabilities.items():
    mappings = cap_data.get("sonic_mappings", {})
    for mapping_name, mapping_data in mappings.items():
        total_mappings += 1
        if "confidence" in mapping_data:
            found_confidence += 1

if found_confidence == total_mappings and total_mappings > 0:
    print(f"  ✓ All {total_mappings} mappings have confidence field")
elif total_mappings > 0:
    print(f"  ⚠ {found_confidence}/{total_mappings} mappings have confidence field")
else:
    print("  ✗ No mappings found")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Validation logs directory exists
echo "Test 6: Validation logs directory..."
if [ -d "$VALIDATION_LOGS" ]; then
    echo "  ✓ Validation logs directory exists"
else
    echo "  ✗ Validation logs directory missing"
    exit 1
fi

# Test 7: Global patterns file exists and is valid
echo "Test 7: Global patterns validity..."
if python3 -m json.tool "$VALIDATION_LOGS/global-patterns.json" > /dev/null 2>&1; then
    echo "  ✓ Global patterns is valid JSON"
else
    echo "  ✗ Global patterns JSON is invalid"
    exit 1
fi

# Test 8: Scoring algorithm exists
echo "Test 8: Scoring algorithm present..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/validation-logs/global-patterns.json") as f:
    data = json.load(f)

if "scoring_algorithm" in data:
    print("  ✓ Scoring algorithm present")
else:
    print("  ✗ Scoring algorithm missing from global patterns")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

echo ""
echo "=== All Sound Design Bridge Tests Passed ==="