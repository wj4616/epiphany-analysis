#!/bin/bash
# Harvest awesome-juce GitHub Repository Links

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Configuration
# ============================================================================

OUTPUT_DIR="$TECHNICAL_KB/github-awesome-juce/linked-resources"
MANIFEST="$TECHNICAL_KB/manifest.json"
INDEX="$TECHNICAL_KB/index.json"
PHASE_NAME="github-awesome-juce"

REPO_URL="https://github.com/sudara/awesome-juce"

# ============================================================================
# Priority Categories for Linked Resources
# ============================================================================

# High priority - harvest first
declare -a HIGH_PRIORITY=(
    # Official JUCE resources (already harvested, but good to reference)
    "juce.com"
    "docs.juce.com"

    # Expert blogs with JUCE content
    "melatonin.dev"
    "chowdsp.com"
    "tracktion.com"

    # Testing frameworks
    "github.com/catchorg/Catch2"
    "github.com/sudara/melatonin"
    "github.com/tracktion/pluginval"

    # DSP libraries
    "github.com/Chowdhury-DSP"
    "github.com/sdatkinson"

    # CI/CD templates
    "github.com/sudara/pamplejuce"
)

# Medium priority - harvest second
declare -a MEDIUM_PRIORITY=(
    # Community resources
    "forum.juce.com"
    "github.com/juce-framework"

    # Tutorials and examples
    "github.com" # General GitHub search for JUCE repos
    "youtube.com" # Video tutorials
)

# ============================================================================
# Functions
# ============================================================================

start_phase() {
    log_info "Starting awesome-juce linked resources harvest"

    python3 << PYEOF
import json
import datetime

with open('$MANIFEST', 'r') as f:
    data = json.load(f)

timestamp = datetime.datetime.now().isoformat()
for phase in data['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['status'] = 'in_progress'
        phase['started_at'] = timestamp

with open('$MANIFEST', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF
}

harvest_readme() {
    log_info "Harvesting awesome-juce README..."

    local output_file="$OUTPUT_DIR/awesome-juce-readme.json"

    if firecrawl scrape "$REPO_URL" \
        --format markdown,html,links \
        -o "$output_file" \
        --json \
        --pretty 2>/dev/null; then

        # Add metadata using Python
        python3 << PYEOF
import json
import datetime

with open('$output_file', 'r') as f:
    data = json.load(f)

data['source_url'] = '$REPO_URL'
data['source'] = 'github-awesome-juce'
data['harvested_at'] = datetime.datetime.now().isoformat()
data['kb_type'] = 'technical'

with open('$output_file', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

        update_manifest_page "technical" "$PHASE_NAME" "$REPO_URL"
        log_success "Saved: $output_file"

        # Return the file path
        echo "$output_file"
        return 0
    else
        log_error "Failed to scrape awesome-juce README"
        return 1
    fi
}

extract_links_from_readme() {
    local readme_file="$1"

    log_info "Extracting links from README..."

    # Extract all URLs from the README using Python
    python3 << PYEOF
import json

with open('$readme_file', 'r') as f:
    data = json.load(f)

links = data.get('links', [])
with open('$SITE_MAPS/awesome-juce-links.txt', 'w') as f:
    for link in sorted(set(links)):
        f.write(link + '\n')

print(len(links))
PYEOF

    local link_count=$(wc -l < "$SITE_MAPS/awesome-juce-links.txt")
    log_info "Found $link_count unique links"

    # Update manifest with total using Python
    python3 << PYEOF
import json

with open('$MANIFEST', 'r') as f:
    data = json.load(f)

for phase in data['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['pages_total'] = $link_count

with open('$MANIFEST', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF
}

is_high_priority() {
    local url="$1"
    for pattern in "${HIGH_PRIORITY[@]}"; do
        if [[ "$url" == *"$pattern"* ]]; then
            return 0
        fi
    done
    return 1
}

is_medium_priority() {
    local url="$1"
    for pattern in "${MEDIUM_PRIORITY[@]}"; do
        if [[ "$url" == *"$pattern"* ]]; then
            return 0
        fi
    done
    return 1
}

scrape_linked_resource() {
    local url="$1"
    local priority="$2"

    # Skip already harvested URLs
    if echo "$url" | grep -qE "docs.juce.com|juce.com/learn|melatonin.dev"; then
        log_info "Skipping already harvested: $url"
        return 0
    fi

    local domain=$(echo "$url" | sed 's/.*:\/\/\([^\/]*\).*/\1/' | tr '/' '_')
    local path=$(echo "$url" | sed 's/.*:\/\/[^\/]*//' | tr '/' '_')
    local output_file="$OUTPUT_DIR/${domain}${path}.json"

    log_info "[$priority] Scraping: $url"

    if firecrawl scrape "$url" \
        --format markdown,html,links \
        -o "$output_file" \
        --json \
        --pretty 2>/dev/null; then

        # Add metadata using Python
        python3 << PYEOF
import json
import datetime

with open('$output_file', 'r') as f:
    data = json.load(f)

data['source_url'] = '$url'
data['source'] = 'github-awesome-juce'
data['priority'] = '$priority'
data['harvested_at'] = datetime.datetime.now().isoformat()
data['kb_type'] = 'technical'

with open('$output_file', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

        update_manifest_page "technical" "$PHASE_NAME" "$url"
        log_success "Saved: $output_file"
        return 0
    else
        log_error "Failed to scrape: $url"
        add_failed_url "technical" "$PHASE_NAME" "$url" "scrape_failed"
        return 1
    fi
}

complete_phase() {
    log_info "Completing awesome-juce harvest"

    python3 << PYEOF
import json
import datetime

with open('$MANIFEST', 'r') as f:
    manifest = json.load(f)

timestamp = datetime.datetime.now().isoformat()
pages = 0
for phase in manifest['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['status'] = 'completed'
        phase['completed_at'] = timestamp
        pages = phase.get('pages_completed', 0)

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)

with open('$INDEX', 'r') as f:
    index = json.load(f)

index['sources']['github-awesome-juce'] = {
    'status': 'completed',
    'pages_harvested': pages,
    'harvest_completed': timestamp
}
index['last_updated'] = timestamp

with open('$INDEX', 'w') as f:
    json.dump(index, f, indent=2)
PYEOF

    log_success "awesome-juce harvest complete"
}

# ============================================================================
# Main
# ============================================================================

main() {
    log_info "=========================================="
    log_info "awesome-juce Linked Resources Harvest"
    log_info "=========================================="

    mkdir -p "$OUTPUT_DIR"
    mkdir -p "$SITE_MAPS"

    start_phase

    # Harvest README first
    local readme_file=$(harvest_readme)

    # Extract links
    extract_links_from_readme "$readme_file"

    # Harvest high priority first
    log_info "Harvesting high-priority resources..."
    while IFS= read -r url; do
        if is_high_priority "$url"; then
            scrape_linked_resource "$url" "high"
            rate_limit_delay
        fi
    done < "$SITE_MAPS/awesome-juce-links.txt"

    # Then medium priority
    log_info "Harvesting medium-priority resources..."
    while IFS= read -r url; do
        if is_medium_priority "$url" && ! is_high_priority "$url"; then
            scrape_linked_resource "$url" "medium"
            rate_limit_delay
        fi
    done < "$SITE_MAPS/awesome-juce-links.txt"

    complete_phase

    log_info "=========================================="
    log_info "Harvest Summary"
    log_info "Total URLs processed: $(wc -l < "$SITE_MAPS/awesome-juce-links.txt")"
    log_info "=========================================="
}

main "$@"