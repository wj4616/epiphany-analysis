#!/bin/bash
# Harvest Melatonin Blog (sudara's expert JUCE blog)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Configuration
# ============================================================================

OUTPUT_DIR="$TECHNICAL_KB/melatonin-dev/blog"
MANIFEST="$TECHNICAL_KB/manifest.json"
INDEX="$TECHNICAL_KB/index.json"
PHASE_NAME="melatonin-dev"

BLOG_INDEX="https://melatonin.dev/blog/"

# ============================================================================
# Known Blog Posts (will be expanded via site map)
# ============================================================================

get_blog_urls() {
    # Known high-value Melatonin blog posts about JUCE
    cat << 'EOF'
https://melatonin.dev/blog/how-to-make-a-juce-plugin-from-scratch/
https://melatonin.dev/blog/what-is-juce-audio-application-vs-audio-plugin/
https://melatonin.dev/blog/juce-plugin-template-pamplejuce/
https://melatonin.dev/blog/juce-audio-plugin-example-tutorial/
https://melatonin.dev/blog/cmake-juce-audio-plugin-example/
https://melatonin.dev/blog/juce-audio-parameter-value-tree-state-apvts/
https://melatonin.dev/blog/juce-smoothedvalue-audio-parameter-attachments/
https://melatonin.dev/blog/juce-thread-safety-audio-plugin-real-time/
https://melatonin.dev/blog/juce-audio-buffer-processblock-audio-thread/
https://melatonin.dev/blog/juce-preset-management-save-load-state/
https://melatonin.dev/blog/juce-look-and-feel-custom-components/
https://melatonin.dev/blog/juce-custom-skin-theme-lookAndFeel/
https://melatonin.dev/blog/how-to-write-tests-for-a-juce-plugin/
https://melatonin.dev/blog/catch2-juce-plugin-testing-tutorial/
https://melatonin.dev/blog/continuous-integration-github-actions-juce-plugin/
https://melatonin.dev/blog/pluginval-juce-plugin-validation/
https://melatonin.dev/blog/melatonin-components-juce-module/
https://melatonin.dev/blog/why-i-made-melatonin-components/
https://melatonin.dev/blog/melatonin-inspector-debug-tool-juce/
https://melatonin.dev/blog/modern-cmake-best-practices-juce-audio-plugins/
https://melatonin.dev/blog/how-to-use-vcpkg-with-juce/
https://melatonin.dev/blog/conan-package-manager-juce-dependencies/
https://melatonin.dev/blog/clang-tidy-static-analysis-juce-plugins/
https://melatonin.dev/blog/address-sanitizer-asan-juce-audio-plugins/
EOF
}

# ============================================================================
# Functions
# ============================================================================

start_phase() {
    log_info "Starting Melatonin blog harvest"

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

scrape_blog_post() {
    local url="$1"

    # Extract post name from URL
    local post_name=$(echo "$url" | sed 's/.*blog\///' | sed 's/\/$//' | tr '/' '-')
    local output_file="$OUTPUT_DIR/${post_name}.json"

    log_info "Scraping: $url"

    if firecrawl scrape "$url" \
        --format markdown,html,links \
        -o "$output_file" \
        --json \
        --pretty 2>/dev/null; then

        # Add metadata
        python3 << PYEOF
import json
import datetime

with open('$output_file', 'r') as f:
    data = json.load(f)

data['source_url'] = '$url'
data['source'] = 'melatonin-dev'
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
    log_info "Completing Melatonin blog harvest"

    python3 << PYEOF
import json
import datetime

with open('$MANIFEST', 'r') as f:
    manifest = json.load(f)

timestamp = datetime.datetime.now().isoformat()
for phase in manifest['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['status'] = 'completed'
        phase['completed_at'] = timestamp
        pages = phase.get('pages_completed', 0)

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)

with open('$INDEX', 'r') as f:
    index = json.load(f)

index['sources']['melatonin-dev'] = {
    'status': 'completed',
    'pages_harvested': pages,
    'harvest_completed': timestamp
}
index['last_updated'] = timestamp

with open('$INDEX', 'w') as f:
    json.dump(index, f, indent=2)
PYEOF

    log_success "Melatonin blog harvest complete"
}

# ============================================================================
# Main
# ============================================================================

main() {
    log_info "=========================================="
    log_info "Melatonin Blog Harvest"
    log_info "=========================================="

    mkdir -p "$OUTPUT_DIR"

    start_phase

    # Get URLs and harvest
    local total=0
    local success=0

    while IFS= read -r url; do
        if [ -n "$url" ]; then
            ((total++))
            if scrape_blog_post "$url"; then
                ((success++))
            fi
            rate_limit_delay
        fi
    done < <(get_blog_urls)

    complete_phase

    log_info "=========================================="
    log_info "Harvest Summary"
    log_info "Total URLs: $total"
    log_info "Successful: $success"
    log_info "=========================================="
}

main "$@"