# tests/test_validate_sync.py
import pytest
import os
import sys
from pathlib import Path

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from validate_sync import (
    validate_harvested_input,
    validate_semantic_content,
    validate_manifest_entry
)


def test_validate_harvested_input_accepts_valid():
    """Valid harvested input passes validation."""
    data = {
        "content": "Test content",
        "kb": "dsp-kb",
        "topic": "reverb",
        "file": "test.json"
    }
    is_valid, error = validate_harvested_input(data)
    assert is_valid
    assert error == ""

def test_validate_harvested_input_rejects_empty():
    """Empty input fails validation."""
    is_valid, error = validate_harvested_input({})
    assert not is_valid
    assert "Empty" in error

def test_validate_harvested_input_rejects_no_content():
    """Input without content fails validation."""
    data = {"kb": "dsp-kb"}
    is_valid, error = validate_harvested_input(data)
    assert not is_valid
    assert "content" in error.lower()

def test_validate_semantic_content_accepts_valid():
    """Valid semantic content passes validation."""
    semantic = {
        "summary": "This is a valid summary.",
        "concepts": [{"name": "Test", "description": "A test concept"}],
        "code_blocks": []
    }
    is_valid, error = validate_semantic_content(semantic)
    assert is_valid
    assert error == ""

def test_validate_semantic_content_rejects_empty_summary():
    """Empty summary fails validation."""
    semantic = {
        "summary": "",
        "concepts": [{"name": "Test"}]
    }
    is_valid, error = validate_semantic_content(semantic)
    assert not is_valid
    assert "summary" in error.lower()

def test_validate_semantic_content_accepts_no_concepts():
    """No concepts is allowed (many valid files don't have concept-formatted content)."""
    semantic = {
        "summary": "Valid summary",
        "concepts": []
    }
    is_valid, error = validate_semantic_content(semantic)
    assert is_valid  # Concepts are now optional

def test_validate_semantic_content_checks_code_blocks():
    """Code blocks are validated."""
    semantic = {
        "summary": "Valid",
        "concepts": [{"name": "Test"}],
        "code_blocks": [
            {"code": "", "language": "cpp"}  # Empty code
        ]
    }
    is_valid, error = validate_semantic_content(semantic)
    assert not is_valid
    assert "code block" in error.lower()

def test_validate_manifest_entry_accepts_harvested():
    """Harvested status with harvested_at passes validation."""
    entry = {
        "status": "harvested",
        "harvested_at": "2026-03-30T12:00:00Z"
    }
    is_valid, error = validate_manifest_entry(entry)
    assert is_valid
    assert error == ""

def test_validate_manifest_entry_rejects_missing_harvested_at():
    """Harvested status without harvested_at fails."""
    entry = {
        "status": "harvested"
    }
    is_valid, error = validate_manifest_entry(entry)
    assert not is_valid
    assert "harvested_at" in error.lower()