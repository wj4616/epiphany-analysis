#!/usr/bin/env python3
"""Tests for harvest-data skill."""

import importlib.util
import sys
from pathlib import Path

# Add scripts directory to path for import
SCRIPTS_DIR = Path(__file__).parent.parent / "scripts"
SCRIPT_FILE = SCRIPTS_DIR / "harvest_data.py"

# Use importlib to load module with underscore name
spec = importlib.util.spec_from_file_location("harvest_data", SCRIPT_FILE)
harvest_data = importlib.util.module_from_spec(spec)
spec.loader.exec_module(harvest_data)

# Access functions
determine_kb = harvest_data.determine_kb
assess_quality = harvest_data.assess_quality
calculate_relevance = harvest_data.calculate_relevance
generate_queries = harvest_data.generate_queries
deduplicate_results = harvest_data.deduplicate_results


class TestKBRouter:
    """Tests for KB categorization."""

    def test_determine_kb_dsp(self):
        """DSP topics with multiple keyword matches should route to dsp-kb."""
        # Multiple keywords match dsp-kb: "reverb", "filter", "delay", "dsp"
        assert determine_kb("reverb delay dsp") == "dsp-kb"
        assert determine_kb("filter oscillator envelope") == "dsp-kb"
        assert determine_kb("audio compression dynamics") == "dsp-kb"

    def test_determine_kb_single_keyword(self):
        """Single keyword matches create new KB name (need score >= 2)."""
        # "reverb" alone only scores 1, creates new KB
        result = determine_kb("reverb algorithms")
        assert result == "reverb-kb"

    def test_determine_kb_juce(self):
        """JUCE topics should route to juce-kb."""
        assert determine_kb("JUCE plugin development") == "juce-kb"
        assert determine_kb("AudioProcessor implementation") == "juce-kb"

    def test_determine_kb_midi(self):
        """MIDI topics should route to midi-kb."""
        assert determine_kb("MIDI protocol") == "midi-kb"
        assert determine_kb("MPE controllers") == "midi-kb"

    def test_determine_kb_unknown(self):
        """Unknown topics should generate new KB name."""
        result = determine_kb("completely unknown topic xyz")
        # Should be a new KB name
        assert "-kb" in result


class TestQualityFilter:
    """Tests for quality filtering."""

    def test_assess_quality_pass(self):
        """High-quality content should pass."""
        # Content must be >= 500 chars (QUALITY_THRESHOLDS["min_content_length"])
        result = {
            "markdown": """# Reverb Algorithms

Convolution reverb is a digital signal processing technique that uses impulse responses to simulate the acoustic characteristics of real spaces. This technique is widely used in audio production and plugin development.

## How Convolution Reverb Works

The process involves convolving an input signal with an impulse response (IR) that captures the acoustic properties of a physical space. The IR is typically recorded by playing a known signal (like a sine sweep) in the space and capturing the response.

## Implementation Example

```cpp
class ConvolutionReverb {
public:
    void process(AudioBuffer<float>& buffer) {
        // Convolve input with impulse response
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            float* channelData = buffer.getWritePointer(channel);
            convolver.process(channelData, channelData, buffer.getNumSamples());
        }
    }
private:
    Convolver convolver;
};
```

## Key Concepts

1. **Impulse Response**: Captures room acoustics
2. **Convolution**: Mathematical operation combining signals
3. **FFT Optimization**: Efficient convolution in frequency domain

This detailed explanation covers reverb algorithms with practical implementation guidance.""",
            "url": "https://example.edu/reverb",
            "title": "Reverb Algorithms"
        }

        passed, report = assess_quality(result, "reverb")
        assert passed is True
        assert report["rejection_reason"] is None

    def test_assess_quality_too_short(self):
        """Short content should be rejected."""
        result = {
            "markdown": "Short content",
            "url": "https://example.com/short",
            "title": "Short"
        }

        passed, report = assess_quality(result, "reverb")
        assert passed is False
        assert report["rejection_reason"] == "content_too_short"

    def test_calculate_relevance(self):
        """Relevance scoring should work correctly."""
        markdown = "Reverb is an audio effect. Convolution reverb uses impulse responses. DSP algorithms for reverb."
        score = calculate_relevance(markdown, "reverb")
        assert score > 0
        assert score <= 1.0


class TestQueryGenerator:
    """Tests for query generation."""

    def test_primary_queries(self):
        """Primary queries should include tutorial and implementation."""
        queries = generate_queries("reverb", "", expansion_level=0)
        assert len(queries) == 4
        assert any("tutorial" in q for q in queries)
        assert any("implementation" in q for q in queries)

    def test_expanded_queries(self):
        """Expanded queries should include academic sources."""
        queries = generate_queries("reverb", "", expansion_level=1)
        assert len(queries) == 4
        assert any("academic" in q or "research" in q for q in queries)


class TestDeduplication:
    """Tests for URL deduplication."""

    def test_deduplicate_removes_duplicates(self):
        """Duplicate URLs should be removed."""
        results = [
            {"url": "https://example.com/a", "title": "A"},
            {"url": "https://example.com/a", "title": "A duplicate"},
            {"url": "https://example.com/b", "title": "B"},
        ]

        unique = deduplicate_results(results)
        assert len(unique) == 2
        assert unique[0]["url"] == "https://example.com/a"
        assert unique[1]["url"] == "https://example.com/b"


if __name__ == "__main__":
    import pytest
    pytest.main([__file__, "-v"])