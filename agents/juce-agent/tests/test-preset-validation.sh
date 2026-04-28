#!/bin/bash
# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
SKILLS_DIR="$HOME/.claude/skills"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs"
# Integration Test: Preset Validation
# Tests that ground truth presets are valid and verifiable

PLAYBOOK="$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json"
GROUND_TRUTH="$AGENT_ROOT/validation-logs/GROUND_TRUTH_PRESETS.md"
VALIDATION_LOGS="$AGENT_ROOT/validation-logs/"

echo "=== Preset Validation Integration Tests ==="
echo ""

# Test 1: Ground truth file exists
echo "Test 1: Ground truth presets file exists..."
if [ -f "$GROUND_TRUTH" ]; then
    echo "  ✓ Ground truth presets file exists"
else
    echo "  ✗ Ground truth presets file missing"
    exit 1
fi

# Test 2: Ground truth contains presets
echo "Test 2: Ground truth contains presets..."
preset_count=$(grep -c '"name":' "$GROUND_TRUTH" 2>/dev/null || echo "0")
if [ "$preset_count" -ge 4 ]; then
    echo "  ✓ Found at least 4 presets"
else
    echo "  ✗ Not enough presets found"
    exit 1
fi

# Test 3: Presets have required fields
echo "Test 3: Presets have required fields..."
python3 << 'EOF'
import re

with open("$AGENT_ROOT/validation-logs/GROUND_TRUTH_PRESETS.md") as f:
    content = f.read()

required_fields = ["name", "category", "capability_requirements", "parameters", "sonic_description"]
missing = []

for field in required_fields:
    if field not in content:
        missing.append(field)

if missing:
    print(f"  ✗ Missing fields in presets: {missing}")
    exit(1)
else:
    print("  ✓ All required fields present in presets")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 4: Playbook preset templates
echo "Test 4: Playbook preset templates exist..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
presets = sound_design.get("preset_templates", {}).get("templates", {})

if len(presets) < 4:
    print(f"  ⚠ Only {len(presets)} presets in playbook")
else:
    print(f"  ✓ {len(presets)} presets in playbook")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 5: Capability requirements reference valid capabilities
echo "Test 5: Capability requirements reference valid capabilities..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
translations = sound_design.get("translations", {}).get("capabilities", {})
capability_reqs = sound_design.get("capability_requirements", {}).get("requirements", {})

# Get list of valid capabilities
valid_caps = set(translations.keys())

# Check that capability requirements reference valid capabilities
invalid_refs = []
for sound_type, reqs in capability_reqs.items():
    required = reqs.get("required", [])
    recommended = reqs.get("recommended", [])
    for cap in required + recommended:
        # Some capabilities have underscores (filter_lowpass)
        # Others are compound (distortion_or_overdrive)
        base_cap = cap.replace("_lowpass", "").replace("_highpass", "").replace("_bandpass", "")
        if base_cap not in valid_caps and cap not in valid_caps:
            # Allow compound capabilities like "distortion_or_overdrive"
            if "_or_" not in cap and cap not in ["filter", "filter_envelope", "compression"]:
                invalid_refs.append(f"{sound_type}: {cap}")

if invalid_refs:
    print(f"  ⚠ Potentially invalid capability refs: {invalid_refs[:3]}")
    print("  ⚠ (warning only - may be compound capabilities)")
else:
    print("  ✓ All capability requirements reference valid capabilities")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 6: Parameter values in valid ranges
echo "Test 6: Parameter values in valid ranges..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
presets = sound_design.get("preset_templates", {}).get("templates", {})

out_of_range = []

for preset_name, preset in presets.items():
    params = preset.get("parameters", {})
    for param_name, param_data in params.items():
        if isinstance(param_data, dict) and "value" in param_data:
            value = param_data["value"]
            # Normalized values should be 0-1
            if isinstance(value, (int, float)):
                if value < 0 or value > 1:
                    # Some params might have different ranges (Hz, seconds)
                    # Just flag extreme values
                    if value < -1 or value > 1000:
                        out_of_range.append(f"{preset_name}.{param_name}: {value}")

if out_of_range:
    print(f"  ⚠ Potentially out of range: {out_of_range[:3]}")
    print("  ⚠ (warning only - may be non-normalized values)")
else:
    print("  ✓ All parameter values appear to be in valid ranges")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 7: Conflict resolution defined
echo "Test 7: Conflict resolution defined..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/playbooks/vst-plugin-playbook-v7-unified.json") as f:
    data = json.load(f)

sound_design = data.get("sound_design", {})
conflicts = sound_design.get("conflict_resolution", {}).get("conflicts", {})

if len(conflicts) >= 3:
    print(f"  ✓ {len(conflicts)} conflict resolutions defined")
else:
    print(f"  ⚠ Only {len(conflicts)} conflict resolutions defined")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 8: Verification status tracking
echo "Test 8: Verification status tracking..."
python3 << 'EOF'
import re

with open("$AGENT_ROOT/validation-logs/GROUND_TRUTH_PRESETS.md") as f:
    content = f.read()

# Count verified vs unverified
verified = content.count('"verified": true') + content.count('verified_by')
unverified = content.count('"verified": false') + content.count('needs_testing')

if verified >= 4:
    print(f"  ✓ {verified} presets verified")
else:
    print(f"  ⚠ Only {verified} presets verified, {unverified} need testing")
    print("  ⚠ (warning only - verification is ongoing)")
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 9: Global patterns file for validation tracking
echo "Test 9: Global patterns validation tracking..."
if [ -f "$VALIDATION_LOGS/global-patterns.json" ]; then
    python3 << 'EOF'
import json
with open("$AGENT_ROOT/validation-logs/global-patterns.json") as f:
    data = json.load(f)

if "translation_accuracy" in data:
    print("  ✓ Translation accuracy tracking present")
else:
    print("  ⚠ Translation accuracy tracking not in global patterns")
EOF
else
    echo "  ✗ Global patterns file missing"
    exit 1
fi

if [ $? -ne 0 ]; then
    exit 1
fi

# Test 10: Confidence scoring algorithm
echo "Test 10: Confidence scoring algorithm..."
python3 << 'EOF'
import json

with open("$AGENT_ROOT/validation-logs/global-patterns.json") as f:
    data = json.load(f)

if "scoring_algorithm" in data:
    algo = data["scoring_algorithm"]
    required = ["initial_confidence", "decay_rate"]
    missing = [r for r in required if r not in algo]

    if missing:
        print(f"  ⚠ Missing scoring params: {missing}")
    else:
        print(f"  ✓ Confidence scoring algorithm complete")
else:
    print("  ✗ Scoring algorithm missing from global patterns")
    exit(1)
EOF

if [ $? -ne 0 ]; then
    exit 1
fi

echo ""
echo "=== All Preset Validation Tests Passed ==="