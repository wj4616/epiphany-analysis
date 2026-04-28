#!/bin/bash
# Batch harvesting runner - Professional grade
# Harvests KB topics with automatic term discovery and checkpoint support

set -e

# Configuration
FIRECRAWL_API_KEY="${FIRECRAWL_API_KEY:-fc-47b7637dde8645afb12b77cf11fd79ec}"
export FIRECRAWL_API_KEY

SCRIPTS_DIR="$(dirname "$0")"
HARVEST_SCRIPT="$SCRIPTS_DIR/harvest-deep.py"
KB_ROOT="$(dirname "$SCRIPTS_DIR")"

# Rate limiting
FIRECRAWL_DELAY="${FIRECRAWL_DELAY:-2.0}"
export FIRECRAWL_DELAY

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
log_error() { echo -e "${RED}[ERROR]${NC} $1"; }
log_phase() { echo -e "${BLUE}[PHASE]${NC} $1"; }

# Clean stale locks (locks older than 1 hour)
clean_stale_locks() {
    local lock_dir="$KB_ROOT/harvested/locks"
    if [ -d "$lock_dir" ]; then
        find "$lock_dir" -name "*.lock" -mmin +60 -delete 2>/dev/null || true
    fi
}

# Call at script start
clean_stale_locks

# Harvest priority order (most impactful first)
# Format: "kb:topic:file" or "kb:topic" for all files in topic
PHASE_1=(
    "dsp-kb:reverb:algorithmic-reverb.json"
    "dsp-kb:dynamics:compressor.json"
    "dsp-kb:dynamics:limiter-design.json"
    "dsp-kb:reverb:convolution-reverb.json"
)

PHASE_2=(
    "juce-kb:realtime:lock-free-queues.json"
    "juce-kb:realtime:audio-thread-safety.json"
    "juce-kb:realtime:allocation-free-patterns.json"
)

PHASE_3=(
    "testing-kb:validation:pluginval.json"
    "testing-kb:unit-testing:catch2-juce.json"
)

PHASE_4=(
    "midi-kb:protocol:midi-basics.json"
    "midi-kb:mpe:mpe-basics.json"
    "midi-kb:controllers:midi-learn.json"
)

usage() {
    cat << 'EOF'
Usage: harvest-batch.sh [OPTIONS]

Deep harvesting with automatic term discovery and checkpoint support.

Options:
  --phase N           Harvest phase N (1-4)
  --kb TYPE           Harvest specific KB (dsp-kb, juce-kb, etc.)
  --topic NAME         Harvest specific topic within KB
  --file FILE          Harvest specific file
  --queries N         Max queries per file (default: 5)
  --crawls N          Max deep crawls per file (default: 3)
  --no-crawl          Skip deep crawling (search only)
  --no-resume         Start fresh, ignore checkpoints
  --dry-run           Show what would be harvested
  --credits           Show credit usage estimates
  --clean             Clean checkpoints for specified scope
  -h, --help          Show this help

Examples:
  # Harvest phase 1 (DSP core)
  ./harvest-batch.sh --phase 1

  # Harvest single file with limits
  ./harvest-batch.sh --kb dsp-kb --topic reverb --file algorithmic-reverb.json --queries 3 --crawls 1

  # Harvest entire topic
  ./harvest-batch.sh --kb midi-kb --topic protocol

  # Resume from checkpoint (default behavior)
  ./harvest-batch.sh --phase 2

  # Start fresh
  ./harvest-batch.sh --phase 2 --no-resume

  # Search only, no crawling
  ./harvest-batch.sh --kb testing-kb --topic validation --no-crawl

Priority Order:
  Phase 1: DSP core (reverb, dynamics) - highest impact
  Phase 2: JUCE real-time safety
  Phase 3: Testing foundation
  Phase 4: MIDI fundamentals

Credit Estimates:
  Per file: ~50-100 credits (5 queries + 1-2 crawls)
  Per phase: ~200-400 credits
  Total: ~800-1600 credits
  Available: 20,500 credits
EOF
}

show_credits() {
    cat << 'EOF'
Credit Usage Estimates
======================

Firecrawl Pricing:
  - Search with scrape: ~5 credits per query + 1 per result
  - Deep crawl: ~1 credit per page crawled
  - Single page scrape: ~1 credit

Per-File Estimates:
  - 5 search queries × 5 credits = 25 credits
  - Average 10 results per query = 50 credits
  - 2 deep crawls × 10 pages = 20 credits
  - Total per file: ~95 credits (range: 50-150)

Phase Estimates:
  Phase 1 (4 files): ~200-400 credits
  Phase 2 (3 files): ~150-300 credits
  Phase 3 (2 files): ~100-200 credits
  Phase 4 (3 files): ~150-300 credits
  ----------------------------------------
  Total: ~600-1200 credits

Available: 20,500 credits
Recommended reserve: ~3,500 credits

After all phases: ~16,000-17,000 credits remaining
EOF
}

clean_checkpoints() {
    local kb="$1"
    local topic="$2"
    local file="$3"

    local checkpoint_dir="$KB_ROOT/harvested/checkpoints"

    if [ -n "$kb" ] && [ -n "$topic" ] && [ -n "$file" ]; then
        rm -f "$checkpoint_dir/${kb}_${topic}_${file}"*.json 2>/dev/null || true
        log_info "Cleaned checkpoint for $kb/$topic/$file"
    elif [ -n "$kb" ] && [ -n "$topic" ]; then
        rm -f "$checkpoint_dir/${kb}_${topic}"*.json 2>/dev/null || true
        log_info "Cleaned checkpoints for $kb/$topic"
    elif [ -n "$kb" ]; then
        rm -f "$checkpoint_dir/${kb}"*.json 2>/dev/null || true
        log_info "Cleaned all checkpoints for $kb"
    else
        rm -rf "$checkpoint_dir"/*.json 2>/dev/null || true
        log_info "Cleaned all checkpoints"
    fi
}

get_phase_files() {
    local phase="$1"
    case "$phase" in
        1) printf '%s\n' "${PHASE_1[@]}" ;;
        2) printf '%s\n' "${PHASE_2[@]}" ;;
        3) printf '%s\n' "${PHASE_3[@]}" ;;
        4) printf '%s\n' "${PHASE_4[@]}" ;;
        *) log_error "Unknown phase: $phase"; exit 1 ;;
    esac
}

harvest_file() {
    local kb="$1"
    local topic="$2"
    local file="$3"
    local max_queries="${4:-5}"
    local max_crawls="${5:-3}"
    local deep_crawl="${6:-true}"
    local resume="${7:-true}"

    local args=""
    args+="--max-queries $max_queries "
    args+="--max-crawls $max_crawls "
    [ "$deep_crawl" != "true" ] && args+="--no-crawl "
    [ "$resume" != "true" ] && args+="--no-resume "

    log_info "Harvesting: $kb/$topic/$file"

    python3 "$HARVEST_SCRIPT" "$kb" --topic "$topic" --file "$file" $args
}

harvest_topic() {
    local kb="$1"
    local topic="$2"
    local max_queries="${3:-5}"
    local max_files="${4:-3}"
    local deep_crawl="${5:-true}"

    local args=""
    args+="--max-queries $max_queries "
    args+="--max-per-topic $max_files "
    [ "$deep_crawl" != "true" ] && args+="--no-crawl "

    log_info "Harvesting topic: $kb/$topic"

    python3 "$HARVEST_SCRIPT" "$kb" --topic "$topic" $args
}

harvest_phase() {
    local phase="$1"
    local max_queries="${2:-5}"
    local max_crawls="${3:-3}"

    log_phase "Starting Phase $phase harvest"

    local files
    files=$(get_phase_files "$phase")

    while IFS= read -r entry; do
        IFS=':' read -r kb topic file <<< "$entry"
        harvest_file "$kb" "$topic" "$file" "$max_queries" "$max_crawls" "true" "true"
        sleep "$FIRECRAWL_DELAY"
    done <<< "$files"

    log_phase "Phase $phase complete"
}

# Parse arguments
PHASE=""
KB=""
TOPIC=""
FILE=""
MAX_QUERIES=5
MAX_CRAWLS=3
NO_CRAWL=false
NO_RESUME=false
DRY_RUN=false
ACTION=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --phase)
            PHASE="$2"
            shift 2
            ;;
        --kb)
            KB="$2"
            shift 2
            ;;
        --topic)
            TOPIC="$2"
            shift 2
            ;;
        --file)
            FILE="$2"
            shift 2
            ;;
        --queries)
            MAX_QUERIES="$2"
            shift 2
            ;;
        --crawls)
            MAX_CRAWLS="$2"
            shift 2
            ;;
        --no-crawl)
            NO_CRAWL=true
            shift
            ;;
        --no-resume)
            NO_RESUME=true
            shift
            ;;
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --credits)
            show_credits
            exit 0
            ;;
        --clean)
            ACTION="clean"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# Validate environment
if [ -z "$FIRECRAWL_API_KEY" ]; then
    log_error "FIRECRAWL_API_KEY not set"
    echo "Set it with: export FIRECRAWL_API_KEY=your_key"
    exit 1
fi

if ! command -v python3 &> /dev/null; then
    log_error "python3 not found"
    exit 1
fi

if [ ! -f "$HARVEST_SCRIPT" ]; then
    log_error "Harvest script not found: $HARVEST_SCRIPT"
    exit 1
fi

# Change to KB root
cd "$KB_ROOT"

# Handle clean action
if [ "$ACTION" = "clean" ]; then
    clean_checkpoints "$KB" "$TOPIC" "$FILE"
    exit 0
fi

# Dry run
if [ "$DRY_RUN" = "true" ]; then
    log_info "Dry run - showing what would be harvested:"

    if [ -n "$PHASE" ]; then
        echo "  Phase $PHASE:"
        get_phase_files "$PHASE" | while read -r entry; do
            IFS=':' read -r k t f <<< "$entry"
            echo "    $k/$t/$f"
        done
    elif [ -n "$KB" ]; then
        if [ -n "$TOPIC" ]; then
            if [ -n "$FILE" ]; then
                echo "  $KB/$TOPIC/$FILE"
            else
                echo "  $KB/$TOPIC/*"
            fi
        else
            echo "  $KB/*/*"
        fi
    fi
    exit 0
fi

# Execute harvest
DEEP_CRAWL_FLAG="true"
[ "$NO_CRAWL" = "true" ] && DEEP_CRAWL_FLAG="false"

RESUME_FLAG="true"
[ "$NO_RESUME" = "true" ] && RESUME_FLAG="false"

if [ -n "$PHASE" ]; then
    harvest_phase "$PHASE" "$MAX_QUERIES" "$MAX_CRAWLS"
elif [ -n "$KB" ]; then
    if [ -n "$TOPIC" ]; then
        if [ -n "$FILE" ]; then
            harvest_file "$KB" "$TOPIC" "$FILE" "$MAX_QUERIES" "$MAX_CRAWLS" "$DEEP_CRAWL_FLAG" "$RESUME_FLAG"
        else
            harvest_topic "$KB" "$TOPIC" "$MAX_QUERIES" "3" "$DEEP_CRAWL_FLAG"
        fi
    else
        log_info "Harvesting entire KB: $KB"
        python3 "$HARVEST_SCRIPT" "$KB" \
            --max-queries "$MAX_QUERIES" \
            --max-crawls "$MAX_CRAWLS" \
            $([ "$NO_CRAWL" = "true" ] && echo "--no-crawl") \
            $([ "$NO_RESUME" = "true" ] && echo "--no-resume")
    fi
else
    usage
    exit 1
fi

log_info "Harvest complete!"
log_info "Output directory: $KB_ROOT/harvested/"