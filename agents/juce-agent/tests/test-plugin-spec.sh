#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Plugin Specification Skill
# Tests that the juce-plugin-spec skill integrates correctly with playbook

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILL="$HOME/.claude/skills/juce-plugin-spec/SKILL.md"

echo "=== Plugin Specification Skill Integration Tests ==="
echo ""

# Test 1: Skill file exists
echo "Test 1: Skill file exists..."
if [ -f "$SKILL" ]; then
    echo "  ✓ Skill file exists at $SKILL"
else
    echo "  ✗ Skill file missing"
    exit 1
fi

# Test 2: Skill references correct playbook
echo "Test 2: Skill references playbook..."
if grep -q "vst-plugin-playbook-v7-unified.json" "$SKILL"; then
    echo "  ✓ Skill references correct playbook"
else
    echo "  ✗ Skill does not reference playbook"
    exit 1
fi

# Test 3: Playbook has Phase 0 specification
echo "Test 3: Phase 0 specification exists..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
phase_0 = None
for phase in phases:
    if phase.get("phase_id") == 0:
        phase_0 = phase
        break

if phase_0 is None:
    print("  ✗ Phase 0 not found in playbook")
    exit(1)

if "checklist" not in phase_0:
    print("  ✗ Phase 0 missing checklist")
    exit(1)

if "phase_gate" not in phase_0:
    print("  ✗ Phase 0 missing phase_gate")
    exit(1)

print(f"  ✓ Phase 0 found with {len(phase_0['checklist'])} checklist items")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Phase 0 has required checklist items
echo "Test 4: Required Phase 0 items..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
phase_0 = None
for phase in phases:
    if phase.get("phase_id") == 0:
        phase_0 = phase
        break

required_items = [
    "plugin type",
    "sound identity",
    "technical constraints",
    "preset theme"
]

checklist_tasks = [item.get("task", "").lower() for item in phase_0.get("checklist", [])]
missing = []

for required in required_items:
    found = any(required in task for task in checklist_tasks)
    if not found:
        missing.append(required)

if missing:
    print(f"  ✗ Missing required items: {missing}")
    exit(1)
else:
    print("  ✓ All required Phase 0 items present")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Plugin type requirements exist
echo "Test 5: Plugin type requirements in capability schema..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

capability_schema = data.get("capability_schema", {})
if "plugin_type_requirements" not in capability_schema:
    print("  ✗ Plugin type requirements missing from capability schema")
    exit(1)

types = capability_schema["plugin_type_requirements"].get("types", {})
expected_types = ["synthesizer", "effect"]
missing_types = [t for t in expected_types if t not in types]

if missing_types:
    print(f"  ✗ Missing plugin types: {missing_types}")
    exit(1)
else:
    print(f"  ✓ Plugin type requirements present with {len(types)} types")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Capability requirements for sound types
echo "Test 6: Capability requirements for sound types..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
capability_reqs = sound_design.get("capability_requirements", {}).get("requirements", {})

if len(capability_reqs) == 0:
    print("  ✗ No capability requirements defined")
    exit(1)

# Check that each sound type has required fields
for sound_type, reqs in capability_reqs.items():
    if "required" not in reqs:
        print(f"  ✗ Sound type '{sound_type}' missing 'required' field")
        exit(1)

print(f"  ✓ {len(capability_reqs)} sound types have capability requirements")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 7: Skill mentions integration points
echo "Test 7: Skill integration points..."
python3 << 'EOF'
with open("$HOME/.claude/skills/juce-plugin-spec/SKILL.md") as f:
    content = f.read()

# Check for key integration concepts (Phase 0 is required, others are recommended)
required_sections = ["Phase 0", "spec"]
recommended_sections = ["brainstorming", "writing-plans"]

missing = []
for section in required_sections:
    if section.lower() not in content.lower():
        missing.append(section)

missing_recommended = []
for section in recommended_sections:
    if section.lower() not in content.lower():
        missing_recommended.append(section)

if missing:
    print(f"  ✗ Skill missing required sections: {missing}")
    exit(1)
elif missing_recommended:
    print(f"  ⚠ Skill missing recommended sections: {missing_recommended}")
    print("  ✓ Required sections present")
else:
    print("  ✓ Skill contains all sections")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 8: Prevention rules exist for Phase 0
echo "Test 8: Prevention rules for Phase 0..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

phases = data.get("phases", [])
phase_0 = None
for phase in phases:
    if phase.get("phase_id") == 0:
        phase_0 = phase
        break

prevention_rules = phase_0.get("prevention_rules", [])
if len(prevention_rules) == 0:
    print("  ✗ No prevention rules in Phase 0")
    exit(1)
else:
    print(f"  ✓ Phase 0 has {len(prevention_rules)} prevention rules")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

echo ""
echo "=== All Plugin Specification Tests Passed ==="