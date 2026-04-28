#!/bin/bash
# Firecrawl Configuration for JUCE Audio Plugin Development Agent
# Optimized for DSP research, JUCE documentation, and sound design knowledge harvesting
#
# Usage:
#   source ~/.agents/juce-agent/.firecrawl/config.sh
#   firecrawl-search-juce "filter design"
#   firecrawl-harvest-dsp "https://example.com/dsp-article"

# Portable paths - derive from script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AGENT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# ============================================================================
# API Configuration
# ============================================================================

# Load API key from .env file
if [ -f "${BASH_SOURCE%/*}/.env" ]; then
    source "${BASH_SOURCE%/*}/.env"
fi

# Use environment variables if set, otherwise use defaults
export FIRECRAWL_API_URL="${FIRECRAWL_API_URL:-https://api.firecrawl.dev}"

# Total credits available
export FIRECRAWL_TOTAL_CREDITS="${FIRECRAWL_TOTAL_CREDITS:-20500}"

# Output directory for harvested content (portable)
export FIRECRAWL_OUTPUT_DIR="$AGENT_ROOT/playbookdata/firecrawl"

# Index file for harvested content
export FIRECRAWL_INDEX_FILE="$FIRECRAWL_OUTPUT_DIR/ENHANCED_MASTER_INDEX.md"

# ============================================================================
# Rate Limiting (Respect API limits)
# ============================================================================

# Max concurrent requests (firecrawl free tier: 2)
export FIRECRAWL_MAX_CONCURRENT="${FIRECRAWL_MAX_CONCURRENT:-2}"

# Delay between requests in seconds (be nice to servers)
export FIRECRAWL_DELAY="${FIRECRAWL_DELAY:-0.5}"

# ============================================================================
# Content Categories for Audio Plugin Development
# ============================================================================

# JUCE-specific sources
export JUCE_SOURCES=(
    "https://docs.juce.com/"
    "https://forum.juce.com/"
    "https://github.com/juce-framework/JUCE"
)

# DSP Education sources
export DSP_SOURCES=(
    "https://www.earlevel.com/main/"
    "https://ccrma.stanford.edu/~jos/"
    "https://www.musicdsp.org/"
)

# Sound Design sources
export SOUND_DESIGN_SOURCES=(
    "https://www.soundonsound.com/"
    "https://www.attackmagazine.com/"
)

# Testing & CI sources
export TESTING_SOURCES=(
    "https://github.com/catchorg/Catch2"
    "https://github.com/sudara/melatonin"
    "https://github.com/tracktion/pluginval"
)

# ============================================================================
# Helper Functions
# ============================================================================

# Search JUCE documentation and forums
firecrawl-search-juce() {
    local query="$1"
    local output="${2:-$FIRECRAWL_OUTPUT_DIR/search-juce-$(echo "$query" | tr ' ' '-').json}"

    echo "Searching JUCE for: $query"
    firecrawl search "$query" \
        --include-hosts "juce.com,forum.juce.com,docs.juce.com,github.com/juce-framework" \
        --limit 5 \
        --scrape \
        -o "$output" \
        --json \
        --pretty

    echo "Results saved to: $output"
}

# Search DSP educational content
firecrawl-search-dsp() {
    local query="$1"
    local output="${2:-$FIRECRAWL_OUTPUT_DIR/search-dsp-$(echo "$query" | tr ' ' '-').json}"

    echo "Searching DSP for: $query"
    firecrawl search "$query" \
        --include-hosts "earlevel.com,ccrma.stanford.edu,musicdsp.org,dsprelated.com" \
        --limit 5 \
        --scrape \
        -o "$output" \
        --json \
        --pretty

    echo "Results saved to: $output"
}

# Harvest a single page with proper naming
firecrawl-harvest-page() {
    local url="$1"
    local name="${2:-$(echo "$url" | sed 's/.*:\/\/\([^/]*\).*/\1/' | tr '/-' '--')}"
    local output="$FIRECRAWL_OUTPUT_DIR/$name.json"

    echo "Harvesting: $url"
    echo "Output: $output"

    firecrawl scrape "$url" \
        --only-main-content \
        -o "$output" \
        --json \
        --pretty
}

# Harvest JUCE documentation page
firecrawl-harvest-juce-docs() {
    local path="$1"
    local url="https://docs.juce.com/$path"
    local name="juce-$(echo "$path" | tr '/' '-')"

    firecrawl-harvest-page "$url" "$name"
}

# Harvest DSP blog article
firecrawl-harvest-dsp-blog() {
    local url="$1"
    local name="${2:-dsp-$(date +%Y%m%d-%H%M)}"

    firecrawl scrape "$url" \
        --only-main-content \
        -o "$FIRECRAWL_OUTPUT_DIR/$name.md"
}

# Crawl a documentation section
firecrawl-crawl-docs() {
    local url="$1"
    local name="${2:-docs-$(date +%Y%m%d)}"

    echo "Crawling: $url"
    echo "Output directory: $FIRECRAWL_OUTPUT_DIR/$name/"

    firecrawl crawl "$url" \
        --include-pattern "**/docs/**" \
        --limit 50 \
        -o "$FIRECRAWL_OUTPUT_DIR/$name/"
}

# Map a site to find relevant URLs
firecrawl-map-site() {
    local url="$1"
    local search="${2:-}"
    local output="$FIRECRAWL_OUTPUT_DIR/map-$(echo "$url" | sed 's/.*:\/\/\([^/]*\).*/\1/').json"

    if [ -n "$search" ]; then
        firecrawl map "$url" --search "$search" -o "$output" --json --pretty
    else
        firecrawl map "$url" -o "$output" --json --pretty
    fi

    echo "Site map saved to: $output"
}

# ============================================================================
# Integration with Sound Design KB
# ============================================================================

# Harvest content and update the Sound Design KB index
firecrawl-harvest-to-kb() {
    local url="$1"
    local category="${2:-general}"  # general, dsp, juce, sound-design
    local name="${3:-$(echo "$url" | sed 's/.*:\/\/\([^/]*\).*/\1/')}"

    local output="$FIRECRAWL_OUTPUT_DIR/$category-$name.json"

    echo "Harvesting $category content: $url"

    firecrawl scrape "$url" \
        --only-main-content \
        -o "$output" \
        --json \
        --pretty

    # Update the master index
    local timestamp=$(date -Iseconds)
    echo "- [$name]($category-$name.json) - $timestamp - $category" >> "$FIRECRAWL_INDEX_FILE"

    echo "Added to index: $FIRECRAWL_INDEX_FILE"
}

# ============================================================================
# Status and Diagnostics
# ============================================================================

# Check firecrawl status
firecrawl-status() {
    echo "=== Firecrawl Status ==="
    echo "API Key: ${FIRECRAWL_API_KEY:0:10}..."
    echo "Total Credits: $FIRECRAWL_TOTAL_CREDITS"
    echo ""
    firecrawl --status 2>/dev/null || echo "Firecrawl CLI not available"

    echo ""
    echo "=== Output Directory ==="
    if [ -d "$FIRECRAWL_OUTPUT_DIR" ]; then
        local count=$(ls -1 "$FIRECRAWL_OUTPUT_DIR"/*.json 2>/dev/null | wc -l)
        echo "Directory: $FIRECRAWL_OUTPUT_DIR"
        echo "Files: $count JSON files"
    else
        echo "Output directory does not exist: $FIRECRAWL_OUTPUT_DIR"
    fi

    echo ""
    echo "=== Recent Files ==="
    ls -lt "$FIRECRAWL_OUTPUT_DIR"/*.json 2>/dev/null | head -5 || echo "No JSON files found"
}

# List harvested content by category
firecrawl-list-harvested() {
    local category="${1:-}"

    if [ -n "$category" ]; then
        echo "=== Harvested: $category ==="
        ls -la "$FIRECRAWL_OUTPUT_DIR/$category-"*.json 2>/dev/null || echo "No files in category: $category"
    else
        echo "=== All Harvested Content ==="
        ls -la "$FIRECRAWL_OUTPUT_DIR"/*.json 2>/dev/null | wc -l
        echo "files in $FIRECRAWL_OUTPUT_DIR"
    fi
}

# ============================================================================
# Quick Reference
# ============================================================================

firecrawl-help() {
    cat << 'EOF'
=== Firecrawl Quick Reference for JUCE Development ===

SEARCH:
  firecrawl-search-juce "filter design"     # Search JUCE docs/forums
  firecrawl-search-dsp "biquad filter"       # Search DSP educational content

HARVEST:
  firecrawl-harvest-page "https://..."      # Harvest a single page
  firecrawl-harvest-juce-docs "tutorials"   # Harvest JUCE documentation
  firecrawl-harvest-dsp-blog "https://..."  # Harvest DSP blog article

CRAWL:
  firecrawl-crawl-docs "https://docs..."    # Crawl entire documentation section
  firecrawl-map-site "https://juce.com"     # Map all URLs on a site

KB INTEGRATION:
  firecrawl-harvest-to-kb "url" "dsp" "name"  # Harvest and add to KB

STATUS:
  firecrawl-status                          # Check status and credits
  firecrawl-list-harvested                  # List harvested content
  firecrawl-list-harvested "juce"           # List JUCE content only

RAW COMMANDS:
  firecrawl scrape "url" -o output.md       # Basic scrape
  firecrawl search "query" --scrape         # Search and scrape
  firecrawl map "url" --search "term"       # Map site with search
  firecrawl crawl "url" --limit 50          # Crawl with limit

OUTPUT LOCATION:
  All output saved to: $FIRECRAWL_OUTPUT_DIR

EOF
}

# Print ready message
echo "Firecrawl configured for JUCE Audio Plugin Development"
echo "Run 'firecrawl-help' for quick reference"
echo "Output directory: $FIRECRAWL_OUTPUT_DIR"