#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Phase Transitions
# Tests that phases can transition correctly through the workflow

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"

echo "=== Phase Transition Integration Tests ==="
echo ""

# Test 1: All phase skills exist
echo "Test 1: Phase skills exist..."
PHASE_SKILLS=("juce-plugin-spec" "juce-dsp-implementation" "juce-daw-testing")
for skill in "${PHASE_SKILLS[@]}"; do
    if [ -f "$SKILLS_DIR/$skill/SKILL.md" ]; then
        echo "  ✓ $skill exists"
    else
        echo "  ✗ $skill missing"
        exit 1
    fi
done

# Test 2: Bridge skills exist
echo "Test 2: Bridge skills exist..."
BRIDGE_SKILLS=("juce-sound-design-bridge" "juce-ui-bridge")
for skill in "${BRIDGE_SKILLS[@]}"; do
    if [ -f "$SKILLS_DIR/$skill/SKILL.md" ]; then
        echo "  ✓ $skill exists"
    else
        echo "  ✗ $skill missing"
        exit 1
    fi
done

# Test 3: Phase 0 outputs are defined
echo "Test 3: Phase 0 outputs..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
phase_0 = None
for p in phases:
    if p.get("phase_id") == 0:
        phase_0 = p
        break

if not phase_0:
    print("  ✗ Phase 0 not found")
    exit(1)

outputs = phase_0.get("output_artifacts", [])
if len(outputs) >= 2:
    print(f"  ✓ Phase 0 has {len(outputs)} output artifacts defined")
else:
    print(f"  ✗ Phase 0 only has {len(outputs)} output artifacts")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Phase gates are defined
echo "Test 4: Phase gates..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
gates_found = 0
for p in phases:
    if "phase_gate" in p:
        gates_found += 1

if gates_found >= 5:
    print(f"  ✓ {gates_found} phase gates defined")
else:
    print(f"  ⚠ Only {gates_found} phase gates defined (expected at least 5)")
EOF

# Test 5: Sound design integration in Phase 0
echo "Test 5: Sound design integration in spec phase..."
if grep -q "sound_design_bridge\|Sound Design" "$SKILLS_DIR/juce-plugin-spec/SKILL.md" 2>/dev/null; then
    echo "  ✓ Sound design referenced in plugin-spec skill"
else
    echo "  ✗ Sound design not integrated in plugin-spec"
    exit 1
fi

# Test 6: UI integration in Phase 0
echo "Test 6: UI integration in spec phase..."
if grep -q "ui_bridge\|UI Design" "$SKILLS_DIR/juce-plugin-spec/SKILL.md" 2>/dev/null; then
    echo "  ✓ UI referenced in plugin-spec skill"
else
    echo "  ✗ UI not integrated in plugin-spec"
    exit 1
fi

# Test 7: Sound design integration in DAW testing
echo "Test 7: Sound design in DAW testing..."
if grep -q "Sound Design\|validation" "$SKILLS_DIR/juce-daw-testing/SKILL.md" 2>/dev/null; then
    echo "  ✓ Sound design referenced in DAW testing skill"
else
    echo "  ✗ Sound design not integrated in DAW testing"
    exit 1
fi

# Test 8: UI quality criteria in DAW testing
echo "Test 8: UI quality in DAW testing..."
if grep -q "UI Quality\|Visual" "$SKILLS_DIR/juce-daw-testing/SKILL.md" 2>/dev/null; then
    echo "  ✓ UI quality criteria in DAW testing skill"
else
    echo "  ✗ UI quality not integrated in DAW testing"
    exit 1
fi

# Test 9: Validation logging in DAW testing
echo "Test 9: Validation logging..."
if grep -q "Validation Logging\|validation-logs" "$SKILLS_DIR/juce-daw-testing/SKILL.md" 2>/dev/null; then
    echo "  ✓ Validation logging documented in DAW testing"
else
    echo "  ✗ Validation logging not documented"
    exit 1
fi

# Test 10: DSP implementation has sound design context
echo "Test 10: Sound design context in DSP implementation..."
if grep -q "Sound Design\|sound_design_bridge" "$SKILLS_DIR/juce-dsp-implementation/SKILL.md" 2>/dev/null; then
    echo "  ✓ Sound design context in DSP implementation"
else
    echo "  ✗ Sound design not integrated in DSP implementation"
    exit 1
fi

echo ""
echo "=== All Phase Transition Tests Passed ==="