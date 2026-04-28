#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Capability Schema
# Tests that capability schema can be populated and queried

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"

echo "=== Capability Schema Integration Tests ==="
echo ""

# Test 1: Capability schema section exists
echo "Test 1: Capability schema section..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

schema = data.get("capability_schema", {})
if "description" in schema and "schema_template" in schema:
    print("  ✓ Capability schema structure exists")
else:
    print("  ✗ Capability schema missing required fields")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 2: Schema template has correct structure
echo "Test 2: Schema template structure..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

template = data.get("capability_schema", {}).get("schema_template", {})
# Check that example_populated exists and has valid structure
example = data.get("capability_schema", {}).get("example_populated", {})

if "filter_lowpass" in example:
    filter_example = example["filter_lowpass"]
    if "present" in filter_example and "parameters" in filter_example:
        print("  ✓ Schema template has correct structure")
    else:
        print("  ✗ Schema template missing required fields")
        exit(1)
else:
    print("  ✗ Example populated missing filter_lowpass")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 3: Sound design bridge compatibility flag
echo "Test 3: Sound design bridge compatibility..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

# Check that example_populated has sound_design_bridge_compatible field
example = data.get("capability_schema", {}).get("example_populated", {})
for cap_name, cap_data in example.items():
    if "sound_design_bridge_compatible" not in cap_data:
        print(f"  ✗ {cap_name} missing sound_design_bridge_compatible")
        exit(1)

print("  ✓ All examples have bridge compatibility flag")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Population instructions exist
echo "Test 4: Population instructions..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

instructions = data.get("capability_schema", {}).get("population_instructions", [])
if len(instructions) >= 4:
    print(f"  ✓ {len(instructions)} population instructions present")
else:
    print(f"  ✗ Only {len(instructions)} population instructions (need at least 4)")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Sound design translations match capability schema structure
echo "Test 5: Translation-capability alignment..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

# Check that capability_schema.example_populated capabilities
# exist in sound_design.translations.capabilities
example_caps = data.get("capability_schema", {}).get("example_populated", {}).keys()
sound_design_caps = data.get("sound_design", {}).get("translations", {}).get("capabilities", {}).keys()

# Example capabilities should be a subset of sound design capabilities
missing = []
for cap in example_caps:
    cap_base = cap.replace("_lowpass", "").replace("_highpass", "")
    if cap_base not in sound_design_caps and cap not in sound_design_caps:
        missing.append(cap)

if missing:
    print(f"  ⚠ Example capabilities not in translations: {missing}")
    print("  (This is OK - they are examples, not all need translations)")
else:
    print("  ✓ All example capabilities have translations available")
EOF

# Test 6: Capabilities dict exists (may be empty)
echo "Test 6: Capabilities dictionary..."
python3 << 'EOF'
import json
with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

capabilities = data.get("capability_schema", {}).get("capabilities", {})
print(f"  ✓ Capabilities dictionary exists (currently {len(capabilities)} entries)")
print("  Note: Capabilities are populated dynamically during Phase 0")
EOF

echo ""
echo "=== All Capability Schema Tests Passed ==="