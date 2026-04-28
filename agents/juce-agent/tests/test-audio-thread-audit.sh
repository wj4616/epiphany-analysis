#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Audio Thread Audit Skill
# Tests that the juce-audio-thread-audit skill integrates correctly

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILL="$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md"

echo "=== Audio Thread Audit Skill Integration Tests ==="
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
echo "Test 2: Skill references FM-01 through FM-07..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
    content = f.read()

required_fms = ["FM-01", "FM-02", "FM-03", "FM-04", "FM-05", "FM-06", "FM-07"]
missing = []

for fm in required_fms:
    if fm not in content:
        missing.append(fm)

if missing:
    print(f"  ✗ Missing failure modes: {missing}")
    exit(1)
else:
    print("  ✓ All FM-01 through FM-07 failure modes documented")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 3: Playbook contains failure modes
echo "Test 3: Failure modes in playbook..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

failure_modes = data.get("failure_modes", {}).get("generic_failure_modes", [])
if len(failure_modes) == 0:
    print("  ✗ No failure modes in playbook")
    exit(1)

# Check for audio thread related failure modes
audio_fms = [fm for fm in failure_modes if "alloc" in str(fm).lower() or
             "thread" in str(fm).lower() or "audio" in str(fm).lower()]

if len(audio_fms) < 5:
    print(f"  ⚠ Only {len(audio_fms)} audio-thread failure modes found")
else:
    print(f"  ✓ {len(audio_fms)} audio-thread related failure modes found")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Skill contains quick checklist
echo "Test 4: Quick audit checklist present..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
    content = f.read()

required_checks = [
    "processBlock",
    "allocation",
    "heap",
    "lock",
    "atomic"
]

missing = []
for check in required_checks:
    if check.lower() not in content.lower():
        missing.append(check)

if missing:
    print(f"  ✗ Missing checklist items: {missing}")
    exit(1)
else:
    print("  ✓ All required checklist items present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Skill mentions ScopedNoDenormals
echo "Test 5: Denormal handling documented..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
    content = f.read()

if "ScopedNoDenormals" in content or "denormal" in content.lower():
    print("  ✓ Denormal handling documented")
else:
    print("  ✗ Denormal handling not documented")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Skill contains buffer overflow checks
echo "Test 6: Buffer overflow checks..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
    content = f.read()

if "192kHz" in content or "buffer" in content.lower():
    print("  ✓ Buffer overflow checks documented")
else:
    print("  ✗ Buffer overflow checks missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 7: Skill contains feedback loop checks
echo "Test 7: Feedback loop stability..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
    content = f.read()

if "feedback" in content.lower() and ("0.95" in content or "0.99" in content or "clamp" in content.lower()):
    print("  ✓ Feedback loop stability documented")
else:
    print("  ✗ Feedback loop stability missing")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 8: Skill contains fallback handling
echo "Test 8: Fallback handling documented..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-audio-thread-audit/SKILL.md") as f:
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
echo "=== All Audio Thread Audit Tests Passed ==="