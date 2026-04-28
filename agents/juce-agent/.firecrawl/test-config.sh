#!/bin/bash
# Test script for firecrawl configuration
# Run this to verify your setup is working

# Source the configuration
source "$AGENT_ROOT/.firecrawl/config.sh"

echo "=== Testing Firecrawl Configuration ==="
echo ""

# Test 1: Check authentication
echo "Test 1: Authentication"
if firecrawl --status 2>&1 | grep -q "Authenticated"; then
    echo "✓ Authenticated"
else
    echo "✗ Not authenticated"
    echo "  Run: firecrawl login --browser"
fi
echo ""

# Test 2: Check output directory
echo "Test 2: Output Directory"
if [ -d "$FIRECRAWL_OUTPUT_DIR" ]; then
    count=$(ls -1 "$FIRECRAWL_OUTPUT_DIR"/*.json 2>/dev/null | wc -l)
    echo "✓ Output directory exists: $FIRECRAWL_OUTPUT_DIR"
    echo "  Files: $count JSON files"
else
    echo "✗ Output directory missing: $FIRECRAWL_OUTPUT_DIR"
    echo "  Creating..."
    mkdir -p "$FIRECRAWL_OUTPUT_DIR"
fi
echo ""

# Test 3: Test helper function
echo "Test 3: Helper Functions"
if type firecrawl-status &>/dev/null; then
    echo "✓ firecrawl-status function available"
else
    echo "✗ firecrawl-status function not found"
fi

if type firecrawl-help &>/dev/null; then
    echo "✓ firecrawl-help function available"
else
    echo "✗ firecrawl-help function not found"
fi
echo ""

# Test 4: Test search function (requires API)
echo "Test 4: Search Function (may consume credits)"
read -p "Run live test? (y/n) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Running test search..."
    firecrawl search "JUCE audio plugin" --limit 1 -o "$FIRECRAWL_OUTPUT_DIR/test-search.json" --json --pretty
    if [ -f "$FIRECRAWL_OUTPUT_DIR/test-search.json" ]; then
        echo "✓ Search test passed"
        rm "$FIRECRAWL_OUTPUT_DIR/test-search.json"
    else
        echo "✗ Search test failed"
    fi
else
    echo "Skipped"
fi
echo ""

echo "=== Configuration Test Complete ==="
echo ""
echo "To use: source "$AGENT_ROOT/.firecrawl/config.sh""
echo "For help: firecrawl-help"