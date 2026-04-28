# tests/test_sync_state.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import json
from datetime import datetime

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from sync_state import SyncState


def test_sync_state_creates_file():
    """Sync state creates state file on first save."""
    with tempfile.TemporaryDirectory() as tmpdir:
        state = SyncState(Path(tmpdir))

        # Initially no file
        assert state.last_run is None
        assert not state.state_file.exists()

        # After recording a run, file should exist
        state.record_run(files_processed=1, files_succeeded=1, files_failed=0)
        assert state.state_file.exists()

def test_sync_state_tracks_last_run():
    """Sync state tracks last run timestamp."""
    with tempfile.TemporaryDirectory() as tmpdir:
        state = SyncState(Path(tmpdir))

        state.record_run(files_processed=10, files_succeeded=9, files_failed=1)

        assert state.last_run is not None
        assert state.files_processed == 10
        assert state.files_succeeded == 9
        assert state.files_failed == 1

def test_sync_state_records_errors():
    """Sync state records errors."""
    with tempfile.TemporaryDirectory() as tmpdir:
        state = SyncState(Path(tmpdir))

        state.record_error("dsp-kb/reverb/test.json", "Empty summary")

        errors = state.get_errors()
        assert len(errors) == 1
        assert errors[0]["file"] == "dsp-kb/reverb/test.json"
        assert errors[0]["error"] == "Empty summary"

def test_sync_state_persists_across_reloads():
    """Sync state persists across reloads."""
    with tempfile.TemporaryDirectory() as tmpdir:
        state_path = Path(tmpdir)

        state1 = SyncState(state_path)
        state1.record_run(files_processed=15, files_succeeded=14, files_failed=1)

        # Create new instance
        state2 = SyncState(state_path)
        assert state2.files_processed == 15
        assert state2.files_succeeded == 14