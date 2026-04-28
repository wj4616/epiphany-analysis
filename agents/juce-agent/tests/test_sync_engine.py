# tests/test_sync_engine.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import json

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from sync_engine import SyncEngine, find_pending_syncs


def test_find_pending_syncs_finds_harvested():
    """Finds entries with status='harvested'."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb"
        kb_dir.mkdir()

        # Create manifest
        manifest = {
            "kb_name": "dsp-kb",
            "topics": [{
                "name": "reverb",
                "files": {
                    "test.json": {
                        "status": "harvested",
                        "harvested_at": "2026-03-30T12:00:00Z"
                    }
                }
            }]
        }
        (kb_dir / "manifest.json").write_text(json.dumps(manifest))

        pending = find_pending_syncs(Path(tmpdir))
        assert len(pending) == 1
        assert pending[0]["kb"] == "dsp-kb"
        assert pending[0]["topic"] == "reverb"
        assert pending[0]["filename"] == "test.json"

def test_find_pending_syncs_ignores_synced():
    """Ignores entries already synced."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb"
        kb_dir.mkdir()

        manifest = {
            "kb_name": "dsp-kb",
            "topics": [{
                "name": "reverb",
                "files": {
                    "test.json": {
                        "status": "synced",
                        "synced_timestamp": 1743338700
                    }
                }
            }]
        }
        (kb_dir / "manifest.json").write_text(json.dumps(manifest))

        pending = find_pending_syncs(Path(tmpdir))
        assert len(pending) == 0

def test_sync_engine_initializes():
    """Sync engine initializes with required components."""
    engine = SyncEngine()
    assert engine.state is not None

def test_find_pending_syncs_finds_retries():
    """Finds failed entries that need retry."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb"
        kb_dir.mkdir()

        manifest = {
            "kb_name": "dsp-kb",
            "topics": [{
                "name": "reverb",
                "files": {
                    "failed.json": {
                        "status": "failed",
                        "retry_count": 1
                    }
                }
            }]
        }
        (kb_dir / "manifest.json").write_text(json.dumps(manifest))

        pending = find_pending_syncs(Path(tmpdir))
        assert len(pending) == 1
        assert pending[0].get("retry") == True