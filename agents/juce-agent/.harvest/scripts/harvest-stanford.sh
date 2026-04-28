#!/bin/bash
# Harvest Stanford CCRMA Audio DSP Content

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Configuration
# ============================================================================

OUTPUT_DIR_COURSES="$TECHNICAL_KB/stanford-ccrma/courses"
OUTPUT_DIR_PAPERS="$TECHNICAL_KB/stanford-ccrma/papers"
MANIFEST="$TECHNICAL_KB/manifest.json"
INDEX="$TECHNICAL_KB/index.json"
PHASE_NAME="stanford-ccrma"

# ============================================================================
# Course URLs (Audio DSP at Stanford CCRMA)
# ============================================================================

get_course_urls() {
    cat << 'EOF'
# Music 320C - Audio DSP Projects
https://ccrma.stanford.edu/courses/320c/
https://ccrma.stanford.edu/~jos/pasp/
https://ccrma.stanford.edu/~jos/pasp/Introduction.html
https://ccrma.stanford.edu/~jos/pasp/Delay_Lines.html
https://ccrma.stanford.edu/~jos/pasp/Filters.html
https://ccrma.stanford.edu/~jos/pasp/Reverb.html
https://ccrma.stanford.edu/~jos/pasp/Physical_Modeling.html
https://ccrma.stanford.edu/~jos/pasp/Wave_Digital_Filters.html
https://ccrma.stanford.edu/~jos/pasp/Commuted_Synthesis.html
# Introduction to Digital Filters
https://ccrma.stanford.edu/~jos/filters/
https://ccrma.stanford.edu/~jos/filters/Introduction.html
https://ccrma.stanford.edu/~jos/filters/Fundamentals.html
https://ccrma.stanford.edu/~jos/filters/Filter_Design.html
https://ccrma.stanford.edu/~jos/filters/IIR_Filters.html
https://ccrma.stanford.edu/~jos/filters/FIR_Filters.html
https://ccrma.stanford.edu/~jos/filters/State_Space_Filters.html
# Spectral Audio Signal Processing
https://ccrma.stanford.edu/~jos/sasp/
https://ccrma.stanford.edu/~jos/sasp/Introduction.html
https://ccrma.stanford.edu/~jos/sasp/FFT.html
https://ccrma.stanford.edu/~jos/sasp/STFT.html
https://ccrma.stanford.edu/~jos/sasp/Phase_Vocoder.html
https://ccrma.stanford.edu/~jos/sasp/Convolution.html
https://ccrma.stanford.edu/~jos/sasp/Window_Functions.html
# Audio Signal Processing for Music Applications
https://ccrma.stanford.edu/~jos/mpft/
https://ccrma.stanford.edu/~jos/mpft/Introduction.html
https://ccrma.stanford.edu/~jos/mpft/Discrete_Time_Signals.html
https://ccrma.stanford.edu/~jos/mpft/Discrete_Fourier_Transform.html
https://ccrma.stanford.edu/~jos/mpft/Fast_Fourier_Transform.html
https://ccrma.stanford.edu/~jos/mpft/Filter_Transfer_Functions.html
# Additional courses
https://ccrma.stanford.edu/courses/424/
https://ccrma.stanford.edu/courses/420/
https://ccrma.stanford.edu/courses/256a/
https://ccrma.stanford.edu/courses/250a/
# DSP Resources
https://ccrma.stanford.edu/~jos/dsp/
https://ccrma.stanford.edu/~jos/resample/
https://ccrma.stanford.edu/~jos/piano/
https://ccrma.stanford.edu/~jos/guitar/
EOF
}

# ============================================================================
# Functions
# ============================================================================

start_phase() {
    log_info "Starting Stanford CCRMA harvest"

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

url_to_filename() {
    local url="$1"
    local base=$(echo "$url" | sed 's/.*ccrma.stanford.edu\///' | sed 's/\//_/g' | sed 's/\.html$//' | sed 's/\.pdf$//' | sed 's/\/$//')
    echo "${base}.json"
}

determine_output_dir() {
    local url="$1"
    if echo "$url" | grep -q "courses"; then
        echo "$OUTPUT_DIR_COURSES"
    else
        echo "$OUTPUT_DIR_PAPERS"
    fi
}

scrape_stanford_page() {
    local url="$1"
    local output_dir=$(determine_output_dir "$url")
    local filename=$(url_to_filename "$url")
    local output_file="$output_dir/$filename"

    log_info "Scraping: $url"

    mkdir -p "$output_dir"

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
data['source'] = 'stanford-ccrma'
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
    log_info "Completing Stanford CCRMA harvest"

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

index['sources']['stanford-ccrma'] = {
    'status': 'completed',
    'pages_harvested': pages,
    'harvest_completed': timestamp
}
index['last_updated'] = timestamp

with open('$INDEX', 'w') as f:
    json.dump(index, f, indent=2)
PYEOF

    log_success "Stanford CCRMA harvest complete"
}

# ============================================================================
# Main
# ============================================================================

main() {
    log_info "=========================================="
    log_info "Stanford CCRMA Harvest"
    log_info "=========================================="

    mkdir -p "$OUTPUT_DIR_COURSES"
    mkdir -p "$OUTPUT_DIR_PAPERS"

    start_phase

    local total=0
    local success=0

    while IFS= read -r line; do
        # Skip comments and empty lines
        if [[ "$line" =~ ^#.*$ ]] || [[ -z "$line" ]]; then
            continue
        fi

        if [ -n "$line" ]; then
            ((total++))
            if scrape_stanford_page "$line"; then
                ((success++))
            fi
            rate_limit_delay
        fi
    done < <(get_course_urls)

    complete_phase

    log_info "=========================================="
    log_info "Harvest Summary"
    log_info "Total URLs: $total"
    log_info "Successful: $success"
    log_info "=========================================="
}

main "$@"