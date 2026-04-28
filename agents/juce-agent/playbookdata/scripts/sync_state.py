#!/usr/bin/env python3
"""
Persistent sync state tracking.

Tracks sync history, errors, and statistics.
"""

import json
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import Optional
import threading

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

# Default state file location
DEFAULT_STATE_FILE = Path(__file__).parent.parent / "harvested" / "sync-state.json"


class SyncState:
    """
    Persistent sync state tracking.

    Usage:
        state = SyncState()
        state.record_run(files_processed=10, files_succeeded=10, files_failed=0)
        state.record_error("dsp-kb/reverb/test.json", "Empty summary")
    """

    def __init__(self, state_dir: Path = None):
        """
        Initialize sync state.

        Args:
            state_dir: Directory for state file (defaults to harvested/)
        """
        if state_dir:
            self.state_file = state_dir / "sync-state.json"
        else:
            self.state_file = DEFAULT_STATE_FILE

        self._lock = threading.Lock()
        self._data = self._load()

    def _load(self) -> dict:
        """Load state from file."""
        if self.state_file.exists():
            try:
                return json.loads(self.state_file.read_text())
            except (json.JSONDecodeError, IOError):
                pass

        return self._default_state()

    def _default_state(self) -> dict:
        """Return default state structure."""
        return {
            "last_run": None,
            "files_processed": 0,
            "files_succeeded": 0,
            "files_failed": 0,
            "errors": [],
            "total_syncs": 0,
            "total_credits_saved": 0,
            "created_at": datetime.now(timezone.utc).isoformat()
        }

    def _save(self):
        """Save state to file."""
        self.state_file.parent.mkdir(parents=True, exist_ok=True)
        self.state_file.write_text(json.dumps(self._data, indent=2))

    @property
    def last_run(self) -> Optional[datetime]:
        """Get last run timestamp."""
        last = self._data.get("last_run")
        if last:
            return datetime.fromisoformat(last.replace("Z", "+00:00"))
        return None

    @property
    def files_processed(self) -> int:
        """Get total files processed."""
        return self._data.get("files_processed", 0)

    @property
    def files_succeeded(self) -> int:
        """Get total files succeeded."""
        return self._data.get("files_succeeded", 0)

    @property
    def files_failed(self) -> int:
        """Get total files failed."""
        return self._data.get("files_failed", 0)

    def record_run(self, files_processed: int, files_succeeded: int, files_failed: int):
        """
        Record a sync run.

        Args:
            files_processed: Number of files processed
            files_succeeded: Number of files succeeded
            files_failed: Number of files failed
        """
        with self._lock:
            self._data["last_run"] = datetime.now(timezone.utc).isoformat()
            self._data["files_processed"] = files_processed
            self._data["files_succeeded"] = files_succeeded
            self._data["files_failed"] = files_failed
            self._data["total_syncs"] = self._data.get("total_syncs", 0) + 1
            self._save()

    def record_error(self, file: str, error: str):
        """
        Record a sync error.

        Args:
            file: File path that had error
            error: Error message
        """
        with self._lock:
            self._data["errors"].append({
                "file": file,
                "error": error,
                "timestamp": datetime.now(timezone.utc).isoformat()
            })
            # Keep only last 100 errors
            if len(self._data["errors"]) > 100:
                self._data["errors"] = self._data["errors"][-100:]
            self._save()

    def get_errors(self) -> list:
        """Get all recorded errors."""
        return self._data.get("errors", [])

    def clear_errors(self):
        """Clear all errors."""
        with self._lock:
            self._data["errors"] = []
            self._save()

    def get_status_report(self) -> dict:
        """Get status report for display."""
        return {
            "last_run": self._data.get("last_run"),
            "files_processed": self.files_processed,
            "files_succeeded": self.files_succeeded,
            "files_failed": self.files_failed,
            "total_syncs": self._data.get("total_syncs", 0),
            "error_count": len(self._data.get("errors", []))
        }


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Manage sync state")
    parser.add_argument("--status", action="store_true", help="Show status")
    parser.add_argument("--clear-errors", action="store_true", help="Clear errors")

    args = parser.parse_args()

    state = SyncState()

    if args.status:
        report = state.get_status_report()
        print(json.dumps(report, indent=2))
    elif args.clear_errors:
        state.clear_errors()
        print("Errors cleared")
    else:
        print("Use --status or --clear-errors")