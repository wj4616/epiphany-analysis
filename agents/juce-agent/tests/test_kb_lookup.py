"""
Unit tests for KB lookup module.
"""

import pytest
import json
from pathlib import Path
from playbookdata.kb_lookup import (
    get_kb, reset_kb, KBQuery, KBResult, KBFile, KBError, KBNotFoundError
)


class TestKBQuery:
    """Unit tests for KB query functionality."""

    def setup_method(self):
        """Reset KB singleton before each test."""
        reset_kb()

    def test_get_kb_returns_instance(self):
        """get_kb returns a KBQuery instance."""
        kb = get_kb()
        assert kb is not None
        assert isinstance(kb, KBQuery)

    def test_get_kb_singleton(self):
        """get_kb returns the same instance."""
        kb1 = get_kb()
        kb2 = get_kb()
        assert kb1 is kb2

    def test_list_kbs_returns_dict(self):
        """list_kbs returns a dictionary."""
        kb = get_kb()
        kbs = kb.list_kbs()
        assert isinstance(kbs, dict)

    def test_query_topic_unknown_returns_empty(self):
        """Query for unknown topic should return empty list."""
        kb = get_kb()
        results = kb.query_topic("nonexistent_topic_xyz")
        assert results == []

    def test_query_topic_invalid_name_returns_empty(self):
        """Query with invalid topic name should return empty."""
        kb = get_kb()
        results = kb.query_topic("invalid-topic!")
        assert results == []
        results = kb.query_topic("a" * 100)
        assert results == []

    def test_clear_cache_resets(self):
        """clear_cache resets internal caches."""
        kb = get_kb()
        kb.query_topic("dsp")
        kb.clear_cache()
        assert kb.query_topic.cache_info().currsize == 0

    def test_health_check_returns_structure(self):
        """health_check returns proper structure."""
        kb = get_kb()
        health = kb.health_check()
        assert "overall_valid" in health
        assert "kbs" in health
        assert "timestamp" in health

    def test_validate_kb_missing_raises(self):
        """validate_kb raises for missing KB."""
        kb = get_kb()
        with pytest.raises(KBNotFoundError):
            kb.validate_kb("nonexistent-kb")


class TestKBResult:
    """Tests for KBResult dataclass."""

    def test_kbresult_comparison(self):
        """KBResult can be compared by relevance."""
        r1 = KBResult(
            kb_name="test",
            file_path="test.json",
            relevance_score=5.0,
            audio_relevance=7
        )
        r2 = KBResult(
            kb_name="test",
            file_path="test2.json",
            relevance_score=10.0,
            audio_relevance=9
        )
        assert r1 < r2
        assert r2 > r1


class TestKBFile:
    """Tests for KBFile dataclass."""

    def test_kbfile_creation(self):
        """KBFile can be created with all fields."""
        kb_file = KBFile(
            kb_name="test-kb",
            file_path="test.json",
            content={"markdown": "# Test"},
            metadata={"source": "test"}
        )
        assert kb_file.kb_name == "test-kb"
        assert kb_file.content["markdown"] == "# Test"


class TestConvenienceFunctions:
    """Tests for convenience functions."""

    def setup_method(self):
        """Reset KB singleton before each test."""
        reset_kb()

    def test_query_topic_function(self):
        """query_topic convenience function works."""
        from playbookdata.kb_lookup import query_topic
        results = query_topic("dsp")
        assert isinstance(results, list)

    def test_get_content_function(self):
        """get_content convenience function works."""
        from playbookdata.kb_lookup import get_content
        content = get_content("nonexistent", fallback="# Default")
        assert content == "# Default"