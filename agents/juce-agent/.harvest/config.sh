#!/bin/bash
# Harvest Configuration for Knowledge Base System
# This file contains shared configuration for all harvest scripts

# ============================================================================
# API Configuration
# ============================================================================

# Firecrawl API Key
export FIRECRAWL_API_KEY="fc-47b7637dde8645afb12b77cf11fd79ec"
export FIRECRAWL_API_URL="https://api.firecrawl.dev"
export FIRECRAWL_TOTAL_CREDITS=20500

# ============================================================================
# Directory Configuration (Portable - uses script location)
# ============================================================================

# Get the directory where this config.sh is located
HARVEST_CONFIG_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Project root is parent of .harvest directory
export HARVEST_ROOT="$(cd "$HARVEST_CONFIG_DIR/.." && pwd)"
export KB_ROOT="$HARVEST_ROOT/playbookdata"

# Individual KB paths
export JUCE_KB="$KB_ROOT/juce-kb"
export DSP_KB="$KB_ROOT/dsp-kb"
export SOUND_DESIGN_KB="$KB_ROOT/sound-design-kb"
export UI_KB="$KB_ROOT/ui-kb"
export CPP_KB="$KB_ROOT/cpp-kb"
export CMAKE_KB="$KB_ROOT/cmake-kb"

# Legacy compatibility
export TECHNICAL_KB="$JUCE_KB"

export SITE_MAPS="$HARVEST_ROOT/.harvest/site-maps"
export SCRIPTS="$HARVEST_ROOT/.harvest/scripts"

# ============================================================================
# Rate Limiting (Respect API limits)
# ============================================================================

export FIRECRAWL_DELAY="${FIRECRAWL_DELAY:-0.5}"
export FIRECRAWL_MAX_CONCURRENT="${FIRECRAWL_MAX_CONCURRENT:-2}"

# ============================================================================
# JUCE Version Target
# ============================================================================

export JUCE_VERSION="7.0.12"
export JUCE_DOCS_BASE="https://docs.juce.com/master"

# ============================================================================
# Source URLs
# ============================================================================

export JUCE_DOCS_INDEX="https://docs.juce.com/master/index.html"
export JUCE_CLASSES_INDEX="https://docs.juce.com/master/classes.html"
export JUCE_TUTORIALS_INDEX="https://juce.com/learn/tutorials/"
export MELATONIN_BLOG="https://melatonin.dev/blog/"
export STANFORD_CCRMA="https://ccrma.stanford.edu/"
export STANFORD_320C="https://ccrma.stanford.edu/courses/320c/"
export AWESOME_JUCE="https://github.com/sudara/awesome-juce"

# ============================================================================
# Output Functions
# ============================================================================

log_info() {
    echo "[$(date -Iseconds)] INFO: $1"
}

log_error() {
    echo "[$(date -Iseconds)] ERROR: $1" >&2
}

log_success() {
    echo "[$(date -Iseconds)] SUCCESS: $1"
}

# ============================================================================
# JSON Helper Functions (using Python instead of jq)
# ============================================================================

json_update() {
    # Update JSON file with Python
    # Usage: json_update <file> <python_dict_of_updates>
    local file="$1"
    local updates="$2"

    python3 -c "
import json
import sys

with open('$file', 'r') as f:
    data = json.load(f)

updates = $updates
for key, value in updates.items():
    if '.' in key:
        # Handle nested keys like 'phases[].name'
        parts = key.split('.')
        obj = data
        for part in parts[:-1]:
            if part.endswith('[]'):
                # Array index
                obj = obj[parts[-2]]
            else:
                obj = obj.get(part, {})
        obj[parts[-1]] = value
    else:
        data[key] = value

with open('$file', 'w') as f:
    json.dump(data, f, indent=2)
"
}

# ============================================================================
# Manifest Functions
# ============================================================================

get_manifest() {
    local kb_type="$1"
    case "$kb_type" in
        juce|technical) echo "$JUCE_KB/manifest.json" ;;
        dsp) echo "$DSP_KB/manifest.json" ;;
        sound-design) echo "$SOUND_DESIGN_KB/manifest.json" ;;
        ui) echo "$UI_KB/manifest.json" ;;
        cpp) echo "$CPP_KB/manifest.json" ;;
        cmake) echo "$CMAKE_KB/manifest.json" ;;
        *) log_error "Unknown KB type: $kb_type"; return 1 ;;
    esac
}

update_manifest_page() {
    local kb_type="$1"
    local phase_name="$2"
    local url="$3"
    local manifest=$(get_manifest "$kb_type")
    local timestamp=$(date -Iseconds)

    python3 << EOF
import json

with open('$manifest', 'r') as f:
    data = json.load(f)

for phase in data['phases']:
    if phase['name'] == '$phase_name':
        phase['pages_completed'] = phase.get('pages_completed', 0) + 1
        phase['last_url_processed'] = '$url'

with open('$manifest', 'w') as f:
    json.dump(data, f, indent=2)
EOF
}

add_failed_url() {
    local kb_type="$1"
    local phase_name="$2"
    local url="$3"
    local error="$4"
    local manifest=$(get_manifest "$kb_type")

    python3 << EOF
import json

with open('$manifest', 'r') as f:
    data = json.load(f)

for phase in data['phases']:
    if phase['name'] == '$phase_name':
        if 'failed_urls' not in phase:
            phase['failed_urls'] = []
        phase['failed_urls'].append({'url': '$url', 'error': '$error'})

with open('$manifest', 'w') as f:
    json.dump(data, f, indent=2)
EOF
}

# ============================================================================
# Index Functions
# ============================================================================

get_index() {
    local kb_type="$1"
    case "$kb_type" in
        juce|technical) echo "$JUCE_KB/index.json" ;;
        dsp) echo "$DSP_KB/index.json" ;;
        sound-design) echo "$SOUND_DESIGN_KB/index.json" ;;
        ui) echo "$UI_KB/index.json" ;;
        cpp) echo "$CPP_KB/index.json" ;;
        cmake) echo "$CMAKE_KB/index.json" ;;
        *) log_error "Unknown KB type: $kb_type"; return 1 ;;
    esac
}

add_cross_reference() {
    local kb_type="$1"
    local topic="$2"
    local file_path="$3"
    local index=$(get_index "$kb_type")

    python3 << EOF
import json

with open('$index', 'r') as f:
    data = json.load(f)

if 'cross_references' not in data:
    data['cross_references'] = {}

if '$topic' not in data['cross_references']:
    data['cross_references']['$topic'] = []

if '$file_path' not in data['cross_references']['$topic']:
    data['cross_references']['$topic'].append('$file_path')

with open('$index', 'w') as f:
    json.dump(data, f, indent=2)
EOF
}

# ============================================================================
# File Naming Functions
# ============================================================================

url_to_filename() {
    local url="$1"
    local base=$(echo "$url" | sed 's/.*:\/\/[^\/]*//' | sed 's/^\///' | sed 's/\///g' | sed 's/\..*//')
    echo "$base.json"
}

class_url_to_filename() {
    local url="$1"
    local class_name=$(echo "$url" | sed 's/.*class//' | sed 's/.html//')
    echo "class${class_name}.json"
}

# ============================================================================
# Rate Limiting
# ============================================================================

rate_limit_delay() {
    sleep "$FIRECRAWL_DELAY"
}

echo "Harvest configuration loaded"
echo "Technical KB: $TECHNICAL_KB"
echo "JUCE Version: $JUCE_VERSION"