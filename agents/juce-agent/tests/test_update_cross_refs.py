# tests/test_update_cross_refs.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import json

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from update_cross_refs import (
    extract_topics,
    extract_cross_references,
    deduplicate_and_prune,
    GENERIC_TERMS,
    MAX_CROSS_REFS_PER_TOPIC
)


def test_extract_topics_finds_capitalized():
    """Extract topics finds capitalized words."""
    topics = extract_topics("Freeverb Algorithm Design")
    assert "Freeverb" in topics
    assert "Algorithm" in topics

def test_extract_topics_filters_generic():
    """Generic terms are filtered."""
    topics = extract_topics("Audio Sound Plugin Music")
    # Should filter out generic terms
    assert len(topics) == 0 or not any(t.lower() in GENERIC_TERMS for t in topics)

def test_extract_cross_references_prioritizes_domains():
    """Priority domains get higher confidence."""
    references = [
        {"title": "Freeverb", "url": "https://ccrma.stanford.edu/freeverb", "domain": "ccrma.stanford.edu"}
    ]

    result = extract_cross_references(references)
    assert len(result) >= 1
    # Find Freeverb entry and check confidence
    freeverb_ref = next((r for r in result if r["topic"] == "Freeverb"), None)
    assert freeverb_ref is not None
    assert freeverb_ref["confidence"] >= 0.9

def test_deduplicate_removes_duplicates():
    """Duplicate topics are removed."""
    refs = [
        {"topic": "reverb", "source": "url1", "confidence": 0.8, "manual": False},
        {"topic": "reverb", "source": "url2", "confidence": 0.9, "manual": False}
    ]

    result = deduplicate_and_prune(refs, {})
    # Should keep only one
    assert len(result) == 1

def test_update_master_cross_refs_updates_file():
    """update_master_cross_refs writes to master index."""
    from update_cross_refs import update_master_cross_refs

    with tempfile.TemporaryDirectory() as tmpdir:
        master_path = Path(tmpdir) / "master-index.json"
        master_path.write_text(json.dumps({
            "knowledge_bases": {}
        }))

        refs = [{"topic": "Freeverb", "source": "url1", "confidence": 0.9, "manual": False}]
        result = update_master_cross_refs(master_path, refs)

        assert result == True
        index = json.loads(master_path.read_text())
        assert "cross_references" in index
        assert "Freeverb" in index["cross_references"]