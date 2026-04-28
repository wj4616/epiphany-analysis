#!/usr/bin/env python3
"""
Extract and update cross-references from harvested content.

Cross-references link related topics across KBs.
"""

import re
import json
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import List, Dict
from urllib.parse import urlparse

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

# Configuration
GENERIC_TERMS = {'audio', 'sound', 'plugin', 'music', 'digital', 'signal', 'process',
                 'implementation', 'design', 'code', 'example', 'tutorial', 'guide'}
MAX_CROSS_REFS_PER_TOPIC = 50
MIN_CONFIDENCE = 0.6

# Priority domains with confidence scores
PRIORITY_DOMAINS = {
    "ccrma.stanford.edu": 0.9,
    "forum.juce.com": 0.85,
    "juce.com": 0.85,
    "earlevel.com": 0.8,
    "musicdsp.org": 0.75,
    "melatonin.dev": 0.75,
    "soundonsound.com": 0.7,
    "attackmagazine.com": 0.7,
    "midi.org": 0.8,
    "github.com": 0.6,
    "stackoverflow.com": 0.6,
    "dsp.stackexchange.com": 0.7
}


def extract_topics(title: str, content: str = None) -> List[str]:
    """
    Extract topic keywords from title and optional content.

    Args:
        title: Title text
        content: Optional content text

    Returns:
        List of extracted topics
    """
    topics = set()

    # Extract capitalized words (likely proper nouns)
    caps = re.findall(r'\b[A-Z][a-z]{2,}\b', title)
    topics.update(caps)

    # Extract hyphenated terms
    hyphenated = re.findall(r'\b([a-z]+-[a-z]+(?:-[a-z]+)*)\b', title.lower())
    topics.update(hyphenated)

    # Extract from content if provided
    if content:
        # Technical terms in content
        tech_terms = re.findall(r'\b[A-Z]{2,}\b', content[:1000])  # Acronyms
        topics.update(tech_terms)

    # Filter out generic terms
    filtered = [t for t in topics if t.lower() not in GENERIC_TERMS]

    # Limit to top 5
    return filtered[:5]


def extract_cross_references(references: List[Dict], existing_refs: Dict = None) -> List[Dict]:
    """
    Extract cross-references from harvested content.

    Args:
        references: List of reference dicts with title, url, domain
        existing_refs: Existing cross-references to check against

    Returns:
        List of new cross-reference dicts
    """
    if existing_refs is None:
        existing_refs = {}

    new_refs = []

    for ref in references:
        domain = ref.get("domain", "")
        url = ref.get("url", "")
        title = ref.get("title", "")

        # Check domain priority
        if domain not in PRIORITY_DOMAINS:
            continue

        confidence = PRIORITY_DOMAINS[domain]

        # Extract topics from title
        topics = extract_topics(title, ref.get("content"))

        for topic in topics:
            if topic.lower() in GENERIC_TERMS:
                continue

            new_refs.append({
                "topic": topic,
                "source": url,
                "confidence": min(confidence, 1.0),
                "manual": False,
                "domain": domain
            })

    return deduplicate_and_prune(new_refs, existing_refs)


def deduplicate_and_prune(new_refs: List[Dict], existing_refs: Dict) -> List[Dict]:
    """
    Deduplicate and prune cross-references.

    Args:
        new_refs: New cross-references
        existing_refs: Existing cross-references

    Returns:
        Deduplicated and pruned list
    """
    # Get existing topics
    existing_topics = set()
    for topic, data in existing_refs.items():
        if isinstance(data, dict):
            existing_topics.add(topic.lower())

    # Filter out existing
    unique = []
    seen = set()
    for ref in new_refs:
        topic_lower = ref["topic"].lower()
        if topic_lower not in existing_topics and topic_lower not in seen:
            unique.append(ref)
            seen.add(topic_lower)

    # Sort by confidence
    unique.sort(key=lambda x: x["confidence"], reverse=True)

    # Prune to max per topic
    topic_counts = {}
    result = []
    for ref in unique:
        topic = ref["topic"]
        count = topic_counts.get(topic, 0)
        if count < MAX_CROSS_REFS_PER_TOPIC:
            result.append(ref)
            topic_counts[topic] = count + 1

    return result


def update_master_cross_refs(master_path: Path, new_refs: List[Dict]) -> bool:
    """
    Update master index with new cross-references.

    Args:
        master_path: Path to master-index.json
        new_refs: New cross-references to add

    Returns:
        True if successful
    """
    if not master_path.exists():
        return False

    try:
        index = json.loads(master_path.read_text())
    except (json.JSONDecodeError, IOError):
        return False

    if "cross_references" not in index:
        index["cross_references"] = {}

    # Add new refs
    for ref in new_refs:
        topic = ref["topic"]
        if topic not in index["cross_references"]:
            index["cross_references"][topic] = {
                "description": f"{topic} related content",
                "kbs": [],
                "confidence": ref["confidence"],
                "manual": False
            }

    index["last_updated"] = datetime.now(timezone.utc).isoformat()

    # Write atomically
    temp_path = master_path.with_suffix(".json.tmp")
    temp_path.write_text(json.dumps(index, indent=2))
    temp_path.rename(master_path)

    return True


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Update cross-references")
    parser.add_argument("--input", help="Input JSON file")
    parser.add_argument("--master", help="Master index path")

    args = parser.parse_args()

    if args.input:
        data = json.loads(open(args.input).read())
        refs = data.get("references", [])
        result = extract_cross_references(refs)
        print(json.dumps(result, indent=2))