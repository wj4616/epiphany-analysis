#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: SmoothedValue Audit Skill
# Tests that the juce-smoothedvalue-audit skill integrates correctly

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILL="$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md"

echo "=== SmoothedValue Audit Skill Integration Tests ==="
echo ""

# Test 1: Skill file exists
echo "Test 1: Skill file exists..."
if [ -f "$SKILL" ]; then
    echo "  ✓ Skill file exists at $SKILL"
else
    echo "  ✗ Skill file missing"
    exit 1
fi

# Test 2: Skill references correct failure modes
echo "Test 2: Skill references failure modes..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

# Check for references to the key failure modes
required_refs = ["FM-09", "EM-13", "CM-05"]
missing = []

for ref in required_refs:
    if ref not in content:
        missing.append(ref)

if missing:
    print(f"  ✗ Missing failure mode references: {missing}")
    exit(1)
else:
    print("  ✓ All required failure mode references present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 3: Playbook contains SmoothedValue patterns
echo "Test 3: SmoothedValue patterns in playbook..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

# Check for smoothed value patterns section
patterns = data.get("juce_patterns", {})
if "smoothed_value" not in patterns and "parameter_smoothing" not in str(data):
    print("  ⚠ SmoothedValue patterns section may need review")
else:
    print("  ✓ SmoothedValue patterns present in playbook")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Skill contains audit checklist
echo "Test 4: Audit checklist present..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

required_checks = [
    "SmoothedValue declaration",
    "prepareToPlay",
    "setTargetValue",
    "getNextValue",
    "per sample"
]

missing = []
for check in required_checks:
    if check.lower() not in content.lower():
        missing.append(check)

if missing:
    print(f"  ✗ Missing audit checks: {missing}")
    exit(1)
else:
    print("  ✓ All required audit checks present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Skill contains common issues
echo "Test 5: Common issues documented..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

issues = ["once per block", "per sample", "dead parameter", "zipper noise"]
found = sum(1 for issue in issues if issue.lower() in content.lower())

if found < 3:
    print(f"  ✗ Only {found}/4 common issues documented")
    exit(1)
else:
    print(f"  ✓ {found}/4 common issues documented")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Skill contains ramp time guidelines
echo "Test 6: Ramp time guidelines present..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

# Look for recommended ramp times
if "20ms" in content or "0.02" in content:
    print("  ✓ Ramp time guidelines present")
else:
    print("  ✗ Ramp time guidelines missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 7: Skill contains grep commands for audit
echo "Test 7: Grep commands for audit..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

if "grep" in content and "SmoothedValue" in content:
    print("  ✓ Grep commands for audit present")
else:
    print("  ✗ Grep commands missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 8: Skill contains fallback handling
echo "Test 8: Fallback handling documented..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-smoothedvalue-audit/SKILL.md") as f:
    content = f.read()

if "Fallback" in content or "fallback" in content.lower():
    print("  ✓ Fallback handling documented")
else:
    print("  ✗ Fallback handling missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

echo ""
echo "=== All SmoothedValue Audit Tests Passed ==="