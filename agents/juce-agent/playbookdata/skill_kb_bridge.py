"""
Skill-to-KB Bridge Module

Provides skill-specific queries that bridge JUCE plugin skills to the knowledge base.
Each skill can query relevant KB content dynamically.

Usage:
    from playbookdata.skill_kb_bridge import SkillKBBridge

    bridge = SkillKBBridge()
    content = bridge.get_dsp_content("filter")
    ui_content = bridge.get_ui_content("knob")
"""

import json
from pathlib import Path
from typing import Optional, List, Dict, Any
from dataclasses import dataclass

# Import from sibling module
try:
    from .kb_lookup import get_kb, KBQuery, KBResult, KBError
except ImportError:
    from kb_lookup import get_kb, KBQuery, KBResult, KBError

# Skill-to-KB topic mappings
SKILL_KB_MAPPINGS = {
    "juce-plugin-spec": {
        "primary_kb": "juce-kb",
        "topics": ["AudioProcessor", "AudioProcessorEditor", "parameters", "state_management"],
        "fallback_content": {
            "AudioProcessor": """# AudioProcessor

## Overview
The AudioProcessor is the core class for audio plugins. It handles:
- Audio processing via processBlock()
- Parameter management
- State persistence
- MIDI handling

## Essential Methods
```cpp
void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
void prepareToPlay(double sampleRate, int samplesPerBlock) override;
void releaseResources() override;
```
"""
        }
    },
    "juce-dsp-implementation": {
        "primary_kb": "dsp-kb",
        "secondary_kb": "juce-kb",
        "topics": ["filter", "oscillator", "envelope", "fft", "delay", "reverb"],
        "fallback_content": {
            "filter": """# Filter Implementation

## Overview
Digital filters for audio processing.

## Filter Types
- Low-pass: Attenuates frequencies above cutoff
- High-pass: Attenuates frequencies below cutoff
- Band-pass: Passes frequencies within a band

## JUCE Implementation
```cpp
dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> filter;
*filter.state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoffFreq);
```
"""
        }
    },
    "juce-sound-design-bridge": {
        "primary_kb": "sound-design-kb",
        "secondary_kb": "dsp-kb",
        "topics": ["bass", "lead", "pad", "drum", "fm", "subtractive"],
        "fallback_content": {
            "bass": """# Synth Bass Sound Design

## Core Elements
- Strong fundamental using saw or square waves
- Tight envelope with short decay
- Mono compatibility check
"""
        }
    },
    "juce-ui-bridge": {
        "primary_kb": "ui-kb",
        "secondary_kb": "juce-kb",
        "topics": ["knob", "slider", "meter", "preset", "component", "lookandfeel"],
        "fallback_content": {
            "knob": """# Custom Knob Component

## Basic Implementation
```cpp
class CustomKnob : public Slider
{
public:
    CustomKnob()
    {
        setSliderStyle(RotaryVerticalDrag);
        setTextBoxStyle(NoTextBox, false, 0, 0);
    }
};
```
"""
        }
    },
    "juce-daw-testing": {
        "primary_kb": "juce-kb",
        "topics": ["testing", "automation", "validation"],
        "fallback_content": {
            "testing": """# DAW Testing

## Validation Steps
1. Build plugin in debug and release modes
2. Load in multiple DAWs
3. Test automation recording
4. Verify parameter recall
"""
        }
    }
}


@dataclass
class SkillContent:
    """Content returned for a skill query."""
    skill_name: str
    topic: str
    markdown: str
    source: str
    kb_name: str
    relevance_score: float


class SkillKBBridge:
    """Bridge between JUCE skills and knowledge bases."""

    def __init__(self):
        self.kb = get_kb()
        self._cache: Dict[str, SkillContent] = {}

    def get_content_for_skill(
        self,
        skill_name: str,
        topic: str,
        use_fallback: bool = True
    ) -> Optional[SkillContent]:
        """Get KB content for a specific skill and topic.

        Args:
            skill_name: Name of the skill (e.g., "juce-dsp-implementation")
            topic: Topic to query (e.g., "filter")
            use_fallback: If True, return fallback content when KB unavailable

        Returns:
            SkillContent if found, None otherwise
        """
        cache_key = f"{skill_name}:{topic}"
        if cache_key in self._cache:
            return self._cache[cache_key]

        # Get skill mapping
        mapping = SKILL_KB_MAPPINGS.get(skill_name)
        if not mapping:
            return None

        # Try primary KB
        primary_kb = mapping.get("primary_kb")
        if primary_kb and self.kb:
            results = self.kb.query_topic(topic, (primary_kb,))
            if results:
                try:
                    content = self.kb.get_file_content(results[0].file_path)
                    result = SkillContent(
                        skill_name=skill_name,
                        topic=topic,
                        markdown=content.content.get("markdown", ""),
                        source=content.metadata.get("source", "unknown"),
                        kb_name=primary_kb,
                        relevance_score=results[0].relevance_score
                    )
                    self._cache[cache_key] = result
                    return result
                except KBError:
                    pass

        # Try secondary KB
        secondary_kb = mapping.get("secondary_kb")
        if secondary_kb and self.kb:
            results = self.kb.query_topic(topic, (secondary_kb,))
            if results:
                try:
                    content = self.kb.get_file_content(results[0].file_path)
                    result = SkillContent(
                        skill_name=skill_name,
                        topic=topic,
                        markdown=content.content.get("markdown", ""),
                        source=content.metadata.get("source", "unknown"),
                        kb_name=secondary_kb,
                        relevance_score=results[0].relevance_score
                    )
                    self._cache[cache_key] = result
                    return result
                except KBError:
                    pass

        # Use fallback content
        if use_fallback:
            fallback = mapping.get("fallback_content", {}).get(topic)
            if fallback:
                result = SkillContent(
                    skill_name=skill_name,
                    topic=topic,
                    markdown=fallback,
                    source="fallback",
                    kb_name="fallback",
                    relevance_score=0.5
                )
                self._cache[cache_key] = result
                return result

        return None

    def get_all_topics_for_skill(self, skill_name: str) -> List[str]:
        """Get all available topics for a skill."""
        mapping = SKILL_KB_MAPPINGS.get(skill_name)
        if mapping:
            return mapping.get("topics", [])
        return []

    def get_dsp_content(self, topic: str) -> Optional[SkillContent]:
        """Convenience method for DSP content."""
        return self.get_content_for_skill("juce-dsp-implementation", topic)

    def get_ui_content(self, topic: str) -> Optional[SkillContent]:
        """Convenience method for UI content."""
        return self.get_content_for_skill("juce-ui-bridge", topic)

    def get_sound_design_content(self, topic: str) -> Optional[SkillContent]:
        """Convenience method for sound design content."""
        return self.get_content_for_skill("juce-sound-design-bridge", topic)

    def get_plugin_spec_content(self, topic: str) -> Optional[SkillContent]:
        """Convenience method for plugin spec content."""
        return self.get_content_for_skill("juce-plugin-spec", topic)

    def get_testing_content(self, topic: str) -> Optional[SkillContent]:
        """Convenience method for testing content."""
        return self.get_content_for_skill("juce-daw-testing", topic)

    def search_across_kbs(
        self,
        query: str,
        skill_name: Optional[str] = None
    ) -> List[SkillContent]:
        """Search across all KBs for a query.

        Args:
            query: Search query string
            skill_name: Optional skill to prioritize

        Returns:
            List of matching SkillContent objects
        """
        results = []

        if self.kb:
            kb_results = self.kb.search_content(query)

            for r in kb_results[:10]:  # Limit to top 10
                try:
                    content = self.kb.get_file_content(r.file_path)
                    results.append(SkillContent(
                        skill_name=skill_name or "search",
                        topic=query,
                        markdown=content.content.get("markdown", ""),
                        source=content.metadata.get("source", "unknown"),
                        kb_name=r.kb_name,
                        relevance_score=r.relevance_score
                    ))
                except KBError:
                    continue

        return results

    def get_implementation_examples(
        self,
        skill_name: str,
        topic: str,
        max_results: int = 5
    ) -> List[SkillContent]:
        """Get implementation examples for a topic with code samples."""
        mapping = SKILL_KB_MAPPINGS.get(skill_name)
        if not mapping:
            return []

        primary_kb = mapping.get("primary_kb")
        results = []

        if primary_kb and self.kb:
            kb_results = self.kb.get_implementation_examples(topic, max_results=max_results)

            for r in kb_results:
                try:
                    content = self.kb.get_file_content(r.file_path)
                    # Filter for content with implementation examples
                    if content.content.get("implementation_examples", 0) > 0:
                        results.append(SkillContent(
                            skill_name=skill_name,
                            topic=topic,
                            markdown=content.content.get("markdown", ""),
                            source=content.metadata.get("source", "unknown"),
                            kb_name=r.kb_name,
                            relevance_score=r.relevance_score
                        ))
                except KBError:
                    continue

        return results

    def clear_cache(self) -> None:
        """Clear the content cache."""
        self._cache.clear()


# Singleton instance
_skill_bridge: Optional[SkillKBBridge] = None


def get_skill_bridge() -> SkillKBBridge:
    """Get the singleton skill bridge instance."""
    global _skill_bridge
    if _skill_bridge is None:
        _skill_bridge = SkillKBBridge()
    return _skill_bridge