#!/bin/bash
# Firecrawl Shortcuts for JUCE Development
#
# Quick one-liners for common operations
# Source this file: source .firecrawl/shortcuts.sh

# ============================================================================
# One-Liner Shortcuts
# ============================================================================

# Harvest JUCE tutorial
alias fc-juce-tutorial='firecrawl scrape "https://docs.juce.com/master/tutorials.html" --only-main-content -o $AGENT_ROOT/playbookdata/firecrawl/juce-tutorials-index.md'

# Harvest JUCE AudioProcessor docs
alias fc-juce-processor='firecrawl scrape "https://docs.juce.com/master/classAudioProcessor.html" --only-main-content -o $AGENT_ROOT/playbookdata/firecrawl/juce-audioprocessor.md'

# Harvest JUCE DSP module
alias fc-juce-dsp='firecrawl scrape "https://docs.juce.com/master/group__juce__dsp.html" --only-main-content -o $AGENT_ROOT/playbookdata/firecrawl/juce-dsp-module.md'

# Search JUCE forums for a topic
fc-search-juce() {
    firecrawl search "$1" --include-hosts "forum.juce.com" --limit 5 --scrape -o $AGENT_ROOT/playbookdata/firecrawl/juce-forum-$1.md
}

# Search DSP content
fc-search-dsp() {
    firecrawl search "$1" --include-hosts "earlevel.com,ccrma.stanford.edu" --limit 5 --scrape -o $AGENT_ROOT/playbookdata/firecrawl/dsp-$1.md
}

# Harvest Sound on Sound article
fc-soundonsound() {
    firecrawl scrape "$1" --only-main-content -o $AGENT_ROOT/playbookdata/firecrawl/sos-$(date +%Y%m%d).md
}

# Harvest Attack Magazine article
fc-attack() {
    firecrawl scrape "$1" --only-main-content -o $AGENT_ROOT/playbookdata/firecrawl/attack-$(date +%Y%m%d).md
}

# ============================================================================
# Batch Operations
# ============================================================================

# Harvest all JUCE tutorial categories
fc-harvest-all-tutorials() {
    echo "Harvesting JUCE tutorials..."
    firecrawl scrape "https://docs.juce.com/master/tutorial_create_basic_audio_plugin.html" -o $AGENT_ROOT/playbookdata/firecrawl/juce-tutorial-basic.md &
    firecrawl scrape "https://docs.juce.com/master/tutorial_dsp_introduction.html" -o $AGENT_ROOT/playbookdata/firecrawl/juce-tutorial-dsp-intro.md &
    firecrawl scrape "https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html" -o $AGENT_ROOT/playbookdata/firecrawl/juce-tutorial-apvts.md &
    wait
    echo "Done harvesting tutorials"
}

# Quick search for common topics
fc-quick-searches() {
    echo "Running quick searches..."
    firecrawl search "JUCE filter implementation" --limit 3 -o $AGENT_ROOT/playbookdata/firecrawl/search-filter.json &
    firecrawl search "JUCE SmoothedValue" --limit 3 -o $AGENT_ROOT/playbookdata/firecrawl/search-smoothedvalue.json &
    firecrawl search "JUCE audio thread safety" --limit 3 -o $AGENT_ROOT/playbookdata/firecrawl/search-threadsafety.json &
    wait
    echo "Done with quick searches"
}

# ============================================================================
# Status Checks
# ============================================================================

# Check credits
alias fc-credits='firecrawl credit-usage'

# Check status
alias fc-check='firecrawl --status'

# List recent files
alias fc-recent='ls -lt $AGENT_ROOT/playbookdata/firecrawl/*.md | head -10'

# Count harvested files
alias fc-count='ls -1 $AGENT_ROOT/playbookdata/firecrawl/*.json 2>/dev/null | wc -l'

echo "Firecrawl shortcuts loaded. Use fc-<tab> to see available commands."