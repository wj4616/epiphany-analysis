#!/bin/bash
# Harvest JUCE Official Documentation
# Exhaustive crawl of docs.juce.com and juce.com/learn for JUCE 7.0.12

set -e

# ============================================================================
# Source Configuration (Portable - uses script location)
# ============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../config.sh"

# ============================================================================
# Output Directories and Manifest Files
# ============================================================================

OUTPUT_DIR="$TECHNICAL_KB/juce-official/v$JUCE_VERSION"
SITE_MAP_FILE="$SITE_MAPS/juce-docs-sitemap.json"
MANIFEST="$TECHNICAL_KB/manifest.json"
INDEX="$TECHNICAL_KB/index.json"

PHASE_NAME="juce-official"

# ============================================================================
# Phase Functions
# ============================================================================

start_phase() {
    log_info "Starting JUCE official documentation harvest"

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

if data.get('started_at') is None:
    data['started_at'] = timestamp

with open('$MANIFEST', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF
}

# ============================================================================
# Site Mapping
# ============================================================================

map_juce_site() {
    log_info "Mapping JUCE documentation site..."

    mkdir -p "$(dirname "$SITE_MAP_FILE")"

    # Map the main docs site using firecrawl map
    if firecrawl map "$JUCE_DOCS_BASE" \
        --search "class module tutorial example" \
        -o "$SITE_MAP_FILE" \
        --json \
        --pretty 2>/dev/null; then

        log_success "Site map saved to $SITE_MAP_FILE"

        # Count URLs using Python
        local url_count=$(python3 -c "import json; print(len(json.load(open('$SITE_MAP_FILE')).get('links', [])))" 2>/dev/null || echo "0")
        log_info "Found $url_count URLs in site map"

        # Update manifest with total pages
        python3 << PYEOF
import json

with open('$MANIFEST', 'r') as f:
    data = json.load(f)

for phase in data['phases']:
    if phase['name'] == '$PHASE_NAME':
        phase['pages_total'] = $url_count

with open('$MANIFEST', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF
    else
        log_error "Failed to map site, using fallback URL list"
    fi
}

# ============================================================================
# URL Extraction - Class URLs
# ============================================================================

extract_class_urls() {
    # Key JUCE classes to harvest - Core Audio
    local classes=(
        "AudioProcessor"
        "AudioProcessorEditor"
        "AudioProcessorListener"
        "AudioProcessorParameter"
        "AudioProcessorValueTreeState"
        "AudioBuffer"
        "AudioSampleBuffer"
        "MidiBuffer"
        "MidiMessage"
        "MidiFile"
        "MidiKeyboardState"

        # GUI Components
        "Component"
        "Slider"
        "ComboBox"
        "TextButton"
        "ToggleButton"
        "Label"
        "TextEditor"
        "Button"
        "Drawable"
        "DrawableButton"
        "ImageButton"
        "HyperlinkButton"

        # State Management
        "Value"
        "ValueTree"
        "CachedValue"
        "AudioProcessorValueTreeState"

        # File and Memory
        "File"
        "FileInputStream"
        "FileOutputStream"
        "MemoryBlock"
        "MemoryInputStream"
        "MemoryOutputStream"

        # Strings
        "String"
        "StringArray"
        "StringPairArray"

        # Collections
        "Array"
        "OwnedArray"
        "ReferenceCountedArray"
        "HashMap"
        "SparseSet"

        # Smart Pointers
        "ScopedPointer"
        "OptionalScopedPointer"

        # Threading
        "Thread"
        "ThreadListener"
        "CriticalSection"
        "ReadWriteLock"
        "Atomic"
        "AbstractFifo"
        "WaitableEvent"
        "InterprocessLock"

        # Timing
        "Time"
        "RelativeTime"
        "Timer"
        "HighResolutionTimer"

        # Messages
        "Message"
        "MessageListener"
        "MessageManager"
        "MessageManagerLock"
        "CallbackMessage"
        "AsyncUpdater"

        # DSP Module
        "dsp::Processor"
        "dsp::ProcessorChain"
        "dsp::IIR"
        "dsp::IIR::Filter"
        "dsp::FIR"
        "dsp::FFT"
        "dsp::Oversampling"
        "dsp::Gain"
        "dsp::DelayLine"
        "dsp::Chorus"
        "dsp::Reverb"
        "dsp::LadderFilter"
        "dsp::Panner"
        "dsp::Oscillator"
        "dsp::Noise"
        "dsp::WaveTable"
        "dsp::ProcessSpec"
        "dsp::AudioBlock"

        # Audio Utilities
        "AudioDeviceManager"
        "AudioDeviceSelectorComponent"
        "AudioFormat"
        "AudioFormatReader"
        "AudioFormatWriter"
        "AudioSource"
        "AudioSourcePlayer"
        "AudioTransportSource"
        "SoundPlayer"
    )

    for class_name in "${classes[@]}"; do
        echo "$JUCE_DOCS_BASE/class${class_name}.html"
    done
}

# ============================================================================
# URL Extraction - Module URLs
# ============================================================================

extract_module_urls() {
    # JUCE module overview pages
    local modules=(
        "juce_audio_basics"
        "juce_audio_devices"
        "juce_audio_formats"
        "juce_audio_processors"
        "juce_audio_utils"
        "juce_core"
        "juce_cryptography"
        "juce_data_structures"
        "juce_dsp"
        "juce_events"
        "juce_graphics"
        "juce_gui_basics"
        "juce_gui_extra"
        "juce_opengl"
        "juce_osc"
        "juce_product_unlocking"
        "juce_video"
    )

    for module in "${modules[@]}"; do
        echo "$JUCE_DOCS_BASE/group__${module}.html"
    done
}

# ============================================================================
# URL Extraction - Tutorial URLs
# ============================================================================

extract_tutorial_urls() {
    # JUCE tutorials - common ones
    local tutorials=(
        "tutorial_create_basic_audio_plugin.html"
        "tutorial_audio_processor_value_tree_state.html"
        "tutorial_dsp_introduction.html"
        "tutorial_dsp_filter.html"
        "tutorial_dsp_oscillator.html"
        "tutorial_dsp_noise.html"
        "tutorial_dsp_delay.html"
        "tutorial_dsp_chorus.html"
        "tutorial_dsp_reverb.html"
        "tutorial_dsp_over_sampling.html"
        "tutorial_dsp_ladder_filter.html"
        "tutorial_gui_basics.html"
        "tutorial_component_listener.html"
        "tutorial_animating_components.html"
        "tutorial_parent_child_components.html"
        "tutorial_buttons.html"
        "tutorial_sliders.html"
        "tutorial_comboboxes.html"
        "tutorial_text_editor.html"
        "tutorial_look_and_feel.html"
        "tutorial_thread_manager.html"
        "tutorial_threads.html"
        "tutorial_value_tree.html"
        "tutorial_state_saved_properties.html"
        "tutorial_state_main_window.html"
        "tutorial_audio_source.html"
        "tutorial_playing_sound_files.html"
        "tutorial_midi.html"
        "tutorial_midi_messages.html"
        "tutorial_midi_output.html"
        "tutorial_plugin_examples.html"
    )

    for tutorial in "${tutorials[@]}"; do
        echo "$JUCE_DOCS_BASE/${tutorial}"
    done
}

# ============================================================================
# Scrape Functions
# ============================================================================

scrape_page() {
    local url="$1"
    local output_file="$2"

    log_info "Scraping: $url"

    # Create output directory if needed
    mkdir -p "$(dirname "$output_file")"

    # Scrape with full content preservation
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
data['source'] = 'juce-official'
data['juce_version'] = '$JUCE_VERSION'
data['harvested_at'] = datetime.datetime.now().isoformat()
data['kb_type'] = 'technical'

with open('$output_file', 'w') as f:
    json.dump(data, f, indent=2)
PYEOF

        # Update manifest
        update_manifest_page "technical" "$PHASE_NAME" "$url"

        log_success "Saved: $output_file"
        return 0
    else
        log_error "Failed to scrape: $url"
        return 1
    fi
}

# ============================================================================
# Main Harvest Loop
# ============================================================================

harvest_classes() {
    log_info "Harvesting JUCE class documentation..."

    for url in $(extract_class_urls); do
        local filename=$(class_url_to_filename "$url")
        local output_file="$OUTPUT_DIR/$filename"

        scrape_page "$url" "$output_file" || true
        rate_limit_delay
    done
}

harvest_modules() {
    log_info "Harvesting JUCE module documentation..."

    for url in $(extract_module_urls); do
        local module_name=$(echo "$url" | sed 's/.*group__//' | sed 's/.html//')
        local output_file="$OUTPUT_DIR/module-${module_name}.json"

        scrape_page "$url" "$output_file" || true
        rate_limit_delay
    done
}

harvest_tutorials() {
    log_info "Harvesting JUCE tutorials..."

    mkdir -p "$OUTPUT_DIR/tutorials"

    for url in $(extract_tutorial_urls); do
        local tutorial_name=$(echo "$url" | sed 's/.*tutorial_//' | sed 's/.html//')
        local output_file="$OUTPUT_DIR/tutorials/${tutorial_name}.json"

        scrape_page "$url" "$output_file" || true
        rate_limit_delay
    done
}

# ============================================================================
# Completion
# ============================================================================

complete_phase() {
    log_info "Completing JUCE official documentation harvest"

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

manifest['completed_at'] = timestamp

with open('$MANIFEST', 'w') as f:
    json.dump(manifest, f, indent=2)

with open('$INDEX', 'r') as f:
    index = json.load(f)

index['sources']['juce-official'] = {
    'status': 'completed',
    'pages_harvested': pages,
    'harvest_completed': timestamp
}
index['last_updated'] = timestamp

with open('$INDEX', 'w') as f:
    json.dump(index, f, indent=2)
PYEOF

    log_success "JUCE official documentation harvest complete"
}

# ============================================================================
# Entry Point
# ============================================================================

main() {
    log_info "=========================================="
    log_info "JUCE Official Documentation Harvest"
    log_info "Target Version: $JUCE_VERSION"
    log_info "=========================================="

    start_phase

    # Ensure output directory exists
    mkdir -p "$OUTPUT_DIR"
    mkdir -p "$OUTPUT_DIR/tutorials"

    # Map site first (optional, will use fallback if fails)
    map_juce_site || true

    # Harvest in order
    harvest_classes
    harvest_modules
    harvest_tutorials

    # Mark complete
    complete_phase

    # Get summary
    pages_completed=$(python3 -c "import json; m=json.load(open('$MANIFEST')); print([p['pages_completed'] for p in m['phases'] if p['name']=='$PHASE_NAME'][0])" 2>/dev/null || echo "unknown")

    log_info "=========================================="
    log_info "Harvest Summary"
    log_info "Pages completed: $pages_completed"
    log_info "=========================================="
}

main "$@"