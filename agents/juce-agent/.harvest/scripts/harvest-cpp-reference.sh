#!/bin/bash
# Harvest C++ Reference Content for cpp-kb
# Focuses on C++ patterns relevant to audio development

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Configuration
# ============================================================================

OUTPUT_DIR="$CPP_KB/cppreference"
MANIFEST="$CPP_KB/manifest.json"
INDEX="$CPP_KB/index.json"
PHASE_NAME="cppreference"

# C++ topics relevant to audio/plugin development
# Focus on: thread safety, memory management, smart pointers, containers
# URL paths follow cppreference structure: /w/cpp/<category>/<topic>
CPP_TOPICS=(
    # Threading - under thread/ namespace
    "thread"
    "thread/mutex"
    "thread/condition_variable"
    "thread/atomic"
    "thread/lock_guard"
    "thread/unique_lock"
    "thread/shared_mutex"

    # Memory - under memory/ namespace
    "memory/unique_ptr"
    "memory/shared_ptr"
    "memory/weak_ptr"
    "memory/make_unique"
    "memory/make_shared"
    "memory/allocator"

    # Containers - direct under cpp/
    "container/vector"
    "container/array"
    "container/span"
    "string"
    "string/string_view"

    # Algorithms
    "algorithm"
    "algorithm/sort"
    "algorithm/find"
    "algorithm/copy"
    "algorithm/transform"

    # Utilities
    "utility/optional"
    "utility/variant"
    "utility/any"
    "utility/function"
    "functional/bind"
)

# URLs for C++ reference
CPP_REFERENCE_BASE="https://en.cppreference.com/w/cpp"

# ============================================================================
# Functions
# ============================================================================

start_phase() {
    log_info "Starting cppreference harvest"

    mkdir -p "$OUTPUT_DIR"

    python3 << PYEOF
import json
from datetime import datetime

manifest = {
    "phases": [{
        "name": "$PHASE_NAME",
        "status": "in_progress",
        "started_at": datetime.now().isoformat(),
        "pages_total": ${#CPP_TOPICS[@]},
        "pages_completed": 0
    }],
    "status": "in_progress"
}

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)
PYEOF
}

scrape_cpp_page() {
    local topic="$1"
    local url="${CPP_REFERENCE_BASE}/${topic}"
    local output_file="$OUTPUT_DIR/${topic}.json"

    log_info "Scraping: $url"

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
data['source'] = 'cppreference'
data['topic'] = '$topic'
data['harvested_at'] = datetime.now().isoformat()
data['kb_type'] = 'cpp'

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
    log_info "Updating cpp-kb index..."

    python3 << PYEOF
import json
from pathlib import Path
from datetime import datetime

kb_path = Path('$CPP_KB')

# Scan for harvested files
topics = {
    'thread_safety': [],
    'memory_management': [],
    'containers': [],
    'algorithms': [],
    'utilities': []
}

for json_file in kb_path.rglob('*.json'):
    if json_file.name in ['index.json', 'manifest.json', 'validation.json']:
        continue

    rel_path = str(json_file.relative_to(kb_path))

    # Categorize by topic
    topic = json_file.stem.lower()

    if topic in ['thread', 'mutex', 'condition_variable', 'atomic', 'lock_guard', 'unique_lock', 'shared_mutex']:
        topics['thread_safety'].append(rel_path)
    elif topic in ['unique_ptr', 'shared_ptr', 'weak_ptr', 'make_unique', 'make_shared', 'allocator']:
        topics['memory_management'].append(rel_path)
    elif topic in ['vector', 'array', 'span', 'string', 'string_view']:
        topics['containers'].append(rel_path)
    elif topic in ['algorithm', 'sort', 'find', 'copy', 'transform']:
        topics['algorithms'].append(rel_path)
    elif topic in ['optional', 'variant', 'any', 'function', 'bind']:
        topics['utilities'].append(rel_path)

# Count total files
total_files = sum(len(files) for files in topics.values())

# Update index
index_path = kb_path / 'index.json'
if index_path.exists():
    with open(index_path) as f:
        index = json.load(f)
else:
    index = {'kb_name': 'cpp-kb', 'version': '1.0.0'}

index['topics'] = topics
index['last_updated'] = datetime.now().isoformat()
index['metadata'] = {
    'total_files': total_files,
    'source_type': 'harvested+curated'
}

with open(index_path, 'w') as f:
    json.dump(index, f, indent=2)

print(f"✓ Updated cpp-kb index: {total_files} files across {len(topics)} topics")
PYEOF
}

complete_phase() {
    log_info "Completing cppreference harvest"

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

master['knowledge_bases']['cpp-kb']['status'] = 'ready'
master['last_updated'] = datetime.now().isoformat()

with open(master_index, 'w') as f:
    json.dump(master, f, indent=2)

PYEOF

    log_success "cppreference harvest complete"
}

# ============================================================================
# Main
# ============================================================================

main() {
    log_info "=========================================="
    log_info "C++ Reference Harvest for cpp-kb"
    log_info "=========================================="

    start_phase

    local success=0
    local failed=0

    for topic in "${CPP_TOPICS[@]}"; do
        if scrape_cpp_page "$topic"; then
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