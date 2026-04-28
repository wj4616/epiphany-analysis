#!/bin/bash
# update-index.sh - Scan harvested JSON files and update topic cross-references
#
# This script scans all harvested JSON files in the knowledge bases and
# updates the index with topic cross-references based on keyword matching.
#
# Usage: ./update-index.sh [kb_type]
#   kb_type: technical, sound-design, or ui (default: all)

set -euo pipefail

# ============================================================================
# Configuration
# ============================================================================

# Source shared configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Topic Keywords
# ============================================================================

# Keywords for each topic (used by Python script)
TOPIC_AUDIO_PROCESSING="AudioProcessor processBlock AudioBuffer prepareToPlay releaseResources"
TOPIC_DSP="dsp:: IIR FIR FFT Filter Gain Delay Reverb Chorus Compression Limiter"
TOPIC_THREAD_SAFETY="Thread CriticalSection Atomic Lock Mutex thread-safe MessageThread MessageManager"
TOPIC_GUI="Component Slider Button ComboBox Editor Graphics paint resized mouseDown mouseUp"
TOPIC_STATE_MANAGEMENT="ValueTree State APVTS Parameter save restore getState setState"
TOPIC_MIDI="MidiBuffer MidiMessage MIDI Note noteOn noteOff controller channel"
TOPIC_PARAMETERS="Parameter AudioProcessorParameter RangedParameter FloatParameter ChoiceParameter"
TOPIC_TESTING="test Test Catch2 GoogleTest pluginval unit integration benchmark"
TOPIC_CI_CD="GitHub Actions CI CD workflow build pipeline automation"
TOPIC_DEPLOYMENT="deploy install package sign notarize distribute release"

# ============================================================================
# Functions
# ============================================================================

# Scan file content for topic keywords using Python
# Arguments: file_path
# Returns: Comma-separated list of matched topics
scan_file_for_topics() {
    local file_path="$1"

    python3 << PYEOF
import json
import re

# Topic keywords
topics = {
    'audio_processing': '$TOPIC_AUDIO_PROCESSING'.split(),
    'dsp': '$TOPIC_DSP'.split(),
    'thread_safety': '$TOPIC_THREAD_SAFETY'.split(),
    'gui': '$TOPIC_GUI'.split(),
    'state_management': '$TOPIC_STATE_MANAGEMENT'.split(),
    'midi': '$TOPIC_MIDI'.split(),
    'parameters': '$TOPIC_PARAMETERS'.split(),
    'testing': '$TOPIC_TESTING'.split(),
    'ci_cd': '$TOPIC_CI_CD'.split(),
    'deployment': '$TOPIC_DEPLOYMENT'.split()
}

try:
    with open('$file_path', 'r') as f:
        content = f.read().lower()
except:
    print('')
    exit(0)

matched = []
for topic, keywords in topics.items():
    for keyword in keywords:
        if keyword.lower() in content:
            if topic not in matched:
                matched.append(topic)
            break

print(','.join(matched))
PYEOF
}

# Add file to topic index with cross-references using Python
# Arguments: kb_type file_path topics
add_file_to_index() {
    local kb_type="$1"
    local file_path="$2"
    local topics="$3"

    local index=$(get_index "$kb_type")

    # Get relative path from KB root
    local kb_root
    case "$kb_type" in
        technical) kb_root="$TECHNICAL_KB" ;;
        sound-design) kb_root="$SOUND_DESIGN_KB" ;;
        ui) kb_root="$UI_KB" ;;
        *) log_error "Unknown KB type: $kb_type"; return 1 ;;
    esac

    local relative_path="${file_path#$kb_root/}"

    # Add cross-references using Python
    python3 << PYEOF
import json

with open('$index', 'r') as f:
    data = json.load(f)

if 'cross_references' not in data:
    data['cross_references'] = {}
if 'topics' not in data:
    data['topics'] = {}

topics = '$topics'.split(',') if '$topics' else []

for topic in topics:
    if topic:
        if topic not in data['cross_references']:
            data['cross_references'][topic] = []
        if '$relative_path' not in data['cross_references'][topic]:
            data['cross_references'][topic].append('$relative_path')

        if topic not in data['topics']:
            data['topics'][topic] = []
        if '$relative_path' not in data['topics'][topic]:
            data['topics'][topic].append('$relative_path')

with open('$index', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

    return 0
}

# Scan all JSON files in a knowledge base directory
# Arguments: kb_type
# Returns: Number of files processed
scan_directory() {
    local kb_type="$1"
    local kb_root
    local file_count=0
    local topic_count=0

    # Get KB root directory
    case "$kb_type" in
        technical) kb_root="$TECHNICAL_KB" ;;
        sound-design) kb_root="$SOUND_DESIGN_KB" ;;
        ui) kb_root="$UI_KB" ;;
        *) log_error "Unknown KB type: $kb_type"; return 1 ;;
    esac

    log_info "Scanning $kb_type knowledge base: $kb_root"

    # Clear existing cross-references using Python
    local index=$(get_index "$kb_type")

    python3 << PYEOF
import json

with open('$index', 'r') as f:
    data = json.load(f)

data['cross_references'] = {}
data['topics'] = {}

for topic in ['audio_processing', 'dsp', 'thread_safety', 'gui', 'state_management', 'midi', 'parameters', 'testing', 'ci_cd', 'deployment']:
    data['cross_references'][topic] = []
    data['topics'][topic] = []

with open('$index', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

    # Find all JSON files (excluding index and manifest)
    while IFS= read -r -d '' file; do
        # Skip index and manifest files
        local basename=$(basename "$file")
        if [[ "$basename" == "index.json" ]] || [[ "$basename" == "manifest.json" ]]; then
            continue
        fi

        # Scan file for topics
        local topics=$(scan_file_for_topics "$file")

        if [[ -n "$topics" ]]; then
            # Add to index
            add_file_to_index "$kb_type" "$file" "$topics"

            # Count topics
            local topic_count_for_file=$(echo "$topics" | tr ',' '\n' | wc -l)
            ((topic_count += topic_count_for_file))
        fi

        ((file_count++))
    done < <(find "$kb_root" -name "*.json" -type f -print0 2>/dev/null)

    # Update last_updated timestamp using Python
    python3 << PYEOF
import json
import datetime

with open('$index', 'r') as f:
    data = json.load(f)

data['last_updated'] = datetime.datetime.now().isoformat()

with open('$index', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

    # Show summary
    log_success "Indexed $file_count files"
    echo ""

    # Show topic distribution
    echo "Topic Distribution:"
    python3 << PYEOF
import json

with open('$index', 'r') as f:
    data = json.load(f)

for topic, files in sorted(data.get('topics', {}).items()):
    if files:
        print(f"  {topic + ':':<18} {len(files)} files")
PYEOF

    return $file_count
}

# Update all knowledge base indexes
update_all_indexes() {
    local total_files=0

    log_info "Updating all knowledge base indexes..."

    # Update technical KB
    if [[ -d "$TECHNICAL_KB" ]]; then
        scan_directory "technical"
        ((total_files += $?))
    else
        log_error "Technical KB directory not found: $TECHNICAL_KB"
    fi

    echo ""

    # Update sound-design KB
    if [[ -d "$SOUND_DESIGN_KB" ]]; then
        scan_directory "sound-design"
        ((total_files += $?))
    else
        log_error "Sound Design KB directory not found: $SOUND_DESIGN_KB"
    fi

    echo ""

    # Update UI KB
    if [[ -d "$UI_KB" ]]; then
        scan_directory "ui"
        ((total_files += $?))
    else
        log_error "UI KB directory not found: $UI_KB"
    fi

    log_success "Total files indexed: $total_files"
}

# ============================================================================
# Main
# ============================================================================

main() {
    local kb_type="${1:-all}"

    log_info "Starting index update..."

    case "$kb_type" in
        technical)
            if [[ -d "$TECHNICAL_KB" ]]; then
                scan_directory "technical"
            else
                log_error "Technical KB directory not found: $TECHNICAL_KB"
                exit 1
            fi
            ;;
        sound-design)
            if [[ -d "$SOUND_DESIGN_KB" ]]; then
                scan_directory "sound-design"
            else
                log_error "Sound Design KB directory not found: $SOUND_DESIGN_KB"
                exit 1
            fi
            ;;
        ui)
            if [[ -d "$UI_KB" ]]; then
                scan_directory "ui"
            else
                log_error "UI KB directory not found: $UI_KB"
                exit 1
            fi
            ;;
        all|"")
            update_all_indexes
            ;;
        *)
            log_error "Unknown KB type: $kb_type"
            echo "Usage: $0 [technical|sound-design|ui|all]"
            exit 1
            ;;
    esac
}

# Run main function
main "$@"