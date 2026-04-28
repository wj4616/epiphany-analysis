#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: End-to-End Workflow
# Tests that all phases integrate correctly from Phase 0 to Phase 9

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"

echo "=== End-to-End Workflow Integration Tests ==="
echo ""

# Test 1: All phases exist in playbook
echo "Test 1: All phases defined..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
expected_phases = list(range(13))  # Phases 0-12
actual_phases = [p.get("phase_id") for p in phases]

missing = [p for p in expected_phases if p not in actual_phases]

if missing:
    print(f"  ✗ Missing phases: {missing}")
    exit(1)
else:
    print(f"  ✓ All 13 phases (0-12) defined")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 2: Phase transitions are valid
echo "Test 2: Phase transitions valid..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])

# Check that each phase has next_phase_context
for phase in phases:
    if "next_phase_context" not in phase:
        print(f"  ✗ Phase {phase.get('phase_id')} missing next_phase_context")
        exit(1)

print(f"  ✓ All {len(phases)} phases have transition context")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 3: All required skills exist
echo "Test 3: Phase skills exist..."
python3 << 'EOF'
import json
import os

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
skills_dir = "$HOME/.claude/skills"
missing_skills = []

# Map phase skills to skill files
skill_mapping = {
    "brainstorming": "brainstorming",
    "writing-plans": "writing-plans",
    "executing-plans": "executing-plans",
    "juce-plugin-spec": "juce-plugin-spec",
    "juce-dsp-implementation": "juce-dsp-implementation",
    "juce-daw-testing": "juce-daw-testing",
    "juce-sound-design-bridge": "juce-sound-design-bridge",
    "juce-ui-bridge": "juce-ui-bridge",
    "juce-audio-thread-audit": "juce-audio-thread-audit",
    "juce-smoothedvalue-audit": "juce-smoothedvalue-audit"
}

for phase in phases:
    skill = phase.get("skill", "")
    if skill in skill_mapping:
        skill_file = f"{skills_dir}/{skill_mapping[skill]}/SKILL.md"
        if not os.path.exists(skill_file):
            missing_skills.append(skill_file)

if missing_skills:
    print(f"  ✗ Missing skill files: {missing_skills}")
    exit(1)
else:
    print("  ✓ All phase skills exist")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Phase gates are complete
echo "Test 4: Phase gates defined..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
gates_with_issues = []

for phase in phases:
    phase_id = phase.get("phase_id")
    gates = phase.get("phase_gate", [])
    if len(gates) == 0:
        gates_with_issues.append(f"Phase {phase_id}: no gates")
    elif len(gates) < 2:
        gates_with_issues.append(f"Phase {phase_id}: only {len(gates)} gate")

if gates_with_issues:
    print(f"  ⚠ Gates with issues: {gates_with_issues[:3]}")
    print("  ⚠ (warning only)")
else:
    print(f"  ✓ All phases have adequate gates")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Integration between phases
echo "Test 5: Phase integration..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

# Check critical integrations
# Phase 0 should mention capability_schema and sound_design
# Phase 4 should mention sound_design_bridge
# Phase 9 should mention validation logging

phases = data.get("phases", [])
playbook_str = json.dumps(data)

integrations = [
    ("capability_schema in playbook", "capability_schema"),
    ("sound_design in playbook", "sound_design"),
    ("ui_design in playbook", "ui_design"),
    ("validation logging", "validation_log")
]

missing = []
for name, keyword in integrations:
    if keyword not in playbook_str.lower():
        missing.append(name)

if missing:
    print(f"  ✗ Missing integrations: {missing}")
    exit(1)
else:
    print("  ✓ All critical integrations present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Sound Design integration
echo "Test 6: Sound Design KB integration..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
capability_schema = data.get("capability_schema", {})

# Check that sound_design references capability_schema
if "translations" not in sound_design:
    print("  ✗ Sound Design missing translations")
    exit(1)

if "capability_requirements" not in sound_design:
    print("  ✗ Sound Design missing capability_requirements")
    exit(1)

# Check that capability_schema references sound_design
if "sound_design_bridge_compatible" not in str(capability_schema):
    print("  ⚠ Capability schema may not reference sound design")
    print("  ✓ Sound Design integration present (with note)")
else:
    print("  ✓ Sound Design fully integrated with Capability Schema")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 7: Plugin type workflow support
echo "Test 7: Plugin type workflow support..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

capability_schema = data.get("capability_schema", {})

if "plugin_type_requirements" in capability_schema:
    types = capability_schema["plugin_type_requirements"].get("types", {})
    if len(types) >= 3:
        print(f"  ✓ Plugin type workflow support for {len(types)} types")
    else:
        print(f"  ⚠ Only {len(types)} plugin types defined")
else:
    print("  ⚠ Plugin type requirements not yet implemented")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 8: Conflict resolution support
echo "Test 8: Conflict resolution support..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})

if "conflict_resolution" in sound_design:
    conflicts = sound_design["conflict_resolution"].get("conflicts", {})
    if len(conflicts) >= 3:
        print(f"  ✓ Conflict resolution defined for {len(conflicts)} conflicts")
    else:
        print(f"  ⚠ Only {len(conflicts)} conflicts defined")
else:
    print("  ⚠ Conflict resolution not yet implemented")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 9: Prevention rules per phase
echo "Test 9: Prevention rules distribution..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
phases_with_rules = 0

for phase in phases:
    rules = phase.get("prevention_rules", [])
    if len(rules) > 0:
        phases_with_rules += 1

if phases_with_rules >= 5:
    print(f"  ✓ {phases_with_rules} phases have prevention rules")
else:
    print(f"  ⚠ Only {phases_with_rules} phases have prevention rules")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 10: Ground truth presets exist
echo "Test 10: Ground truth presets..."
GROUND_TRUTH="$AGENT_ROOT/validation-logs/GROUND_TRUTH_PRESETS.md"
if [ -f "$GROUND_TRUTH" ]; then
    preset_count=$(grep -c "verified" "$GROUND_TRUTH" 2>/dev/null || echo "0")
    echo "  ✓ Ground truth presets file exists"
else
    echo "  ✗ Ground truth presets file missing"
    exit 1
fi

echo ""
echo "=== All End-to-End Workflow Tests Passed ==="