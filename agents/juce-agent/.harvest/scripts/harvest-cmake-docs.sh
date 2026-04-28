#!/bin/bash
# Harvest CMake Documentation for cmake-kb
# Focuses on CMake patterns relevant to JUCE plugin development

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Configuration
# ============================================================================

OUTPUT_DIR="$CMAKE_KB/cmake-docs"
MANIFEST="$CMAKE_KB/manifest.json"
INDEX="$CMAKE_KB/index.json"
PHASE_NAME="cmake-docs"

# CMake topics relevant to JUCE/plugin development
CMAKE_TOPICS=(
    # Core commands
    "command/add_executable"
    "command/add_library"
    "command/target_link_libraries"
    "command/target_include_directories"
    "command/target_compile_definitions"
    "command/set_target_properties"

    # Variables
    "variable/CMAKE_BUILD_TYPE"
    "variable/CMAKE_INSTALL_PREFIX"
    "variable/CMAKE_SOURCE_DIR"
    "variable/CMAKE_BINARY_DIR"

    # Build configuration
    "command/project"
    "command/cmake_minimum_required"
    "command/option"
    "command/set"

    # Installation
    "command/install"
    "command/install_targets"

    # Testing
    "command/enable_testing"
    "command/add_test"

    # Generator expressions
    "manual/cmake-generator-expressions.7"

    # Find modules
    "command/find_package"
    "command/find_library"
    "command/find_path"
)

CMAKE_BASE="https://cmake.org/cmake/help/latest"

# ============================================================================
# Functions
# ============================================================================

start_phase() {
    log_info "Starting CMake documentation harvest"

    mkdir -p "$OUTPUT_DIR"

    python3 << PYEOF
import json
from datetime import datetime

manifest = {
    "phases": [{
        "name": "$PHASE_NAME",
        "status": "in_progress",
        "started_at": datetime.now().isoformat(),
        "pages_total": ${#CMAKE_TOPICS[@]},
        "pages_completed": 0
    }],
    "status": "in_progress"
}

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)
PYEOF
}

scrape_cmake_page() {
    local topic="$1"
    local url="${CMAKE_BASE}/${topic}.html"
    local output_file="$OUTPUT_DIR/${topic//\//_}.json"

    log_info "Scraping: $url"

    # Create directory if needed
    mkdir -p "$(dirname "$output_file")"

    if firecrawl scrape "$url" \
        --format markdown,html,links \
        -o "$output_file" \
        --json \
        --pretty 2>/dev/null; then

        # Add metadata
        python3 << PYEOF
import json
from datetime import datetime

with open('$output_file', 'r') as f:
    data = json.load(f)

data['source_url'] = '$url'
data['source'] = 'cmake-docs'
data['topic'] = '$topic'
data['harvested_at'] = datetime.now().isoformat()
data['kb_type'] = 'cmake'

with open('$output_file', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

        # Update manifest
        python3 << PYEOF
import json

with open('$MANIFEST', 'r') as f:
    data = json.load(f)

for phase in data['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['pages_completed'] = phase.get('pages_completed', 0) + 1

with open('$MANIFEST', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

        log_success "Saved: $output_file"
        return 0
    else
        log_error "Failed to scrape: $url"
        return 1
    fi
}

update_index() {
    log_info "Updating cmake-kb index..."

    python3 << PYEOF
import json
from pathlib import Path
from datetime import datetime

kb_path = Path('$CMAKE_KB')

# Scan for harvested files
topics = {
    'build_configuration': [],
    'targets': [],
    'ci_cd': [],
    'deployment': []
}

for json_file in kb_path.rglob('*.json'):
    if json_file.name in ['index.json', 'manifest.json', 'validation.json']:
        continue

    rel_path = str(json_file.relative_to(kb_path))

    # Categorize by topic
    topic = json_file.stem.lower()

    if topic in ['project', 'cmake_minimum_required', 'option', 'set', 'cmake_build_type', 'cmake_install_prefix']:
        topics['build_configuration'].append(rel_path)
    elif topic in ['add_executable', 'add_library', 'target_link_libraries', 'target_include_directories', 'target_compile_definitions']:
        topics['targets'].append(rel_path)
    elif topic in ['enable_testing', 'add_test', 'find_package']:
        topics['ci_cd'].append(rel_path)
    elif topic in ['install', 'install_targets']:
        topics['deployment'].append(rel_path)

# Count total files
total_files = sum(len(files) for files in topics.values())

# Update index
index_path = kb_path / 'index.json'
if index_path.exists():
    with open(index_path) as f:
        index = json.load(f)
else:
    index = {'kb_name': 'cmake-kb', 'version': '1.0.0'}

index['topics'] = topics
index['last_updated'] = datetime.now().isoformat()
index['metadata'] = {
    'total_files': total_files,
    'source_type': 'harvested+curated'
}

with open(index_path, 'w') as f:
    json.dump(index, f, indent=2)

print(f"✓ Updated cmake-kb index: {total_files} files across {len(topics)} topics")
PYEOF
}

complete_phase() {
    log_info "Completing CMake documentation harvest"

    python3 << PYEOF
import json
from datetime import datetime

with open('$MANIFEST', 'r') as f:
    manifest = json.load(f)

for phase in manifest['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['status'] = 'completed'
        phase['completed_at'] = datetime.now().isoformat()

manifest['status'] = 'completed'

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)

# Update master index
master_index = '$KB_ROOT/master-index.json'
with open(master_index) as f:
    master = json.load(f)

master['knowledge_bases']['cmake-kb']['status'] = 'ready'
master['last_updated'] = datetime.now().isoformat()

with open(master_index, 'w') as f:
    json.dump(master, f, indent=2)

PYEOF

    log_success "CMake documentation harvest complete"
}

# ============================================================================
# Main
# ============================================================================

main() {
    log_info "=========================================="
    log_info "CMake Documentation Harvest for cmake-kb"
    log_info "=========================================="

    start_phase

    local success=0
    local failed=0

    for topic in "${CMAKE_TOPICS[@]}"; do
        if scrape_cmake_page "$topic"; then
            ((success++))
        else
            ((failed++))
        fi
        rate_limit_delay
    done

    update_index
    complete_phase

    log_info "=========================================="
    log_info "Harvest Summary"
    log_info "Successful: $success"
    log_info "Failed: $failed"
    log_info "=========================================="
}

main "$@"