# tests/test_kb_sync_integration.py
"""Integration tests for kb-sync skill."""

import pytest
import subprocess
from pathlib import Path
import tempfile
import json
import shutil

PLAYBOOKDATA = Path(__file__).parent.parent / "playbookdata"


def run_sync(args: list) -> subprocess.CompletedProcess:
    """Run sync_engine.py with args."""
    cmd = ["python3", str(PLAYBOOKDATA / "scripts" / "sync_engine.py")] + args
    return subprocess.run(cmd, capture_output=True, text=True, cwd=PLAYBOOKDATA)


class TestSyncEngineIntegration:
    """Integration tests for sync engine."""

    def test_status_command_works(self):
        """Status command returns without error."""
        result = run_sync(["--status"])
        assert result.returncode == 0

    def test_dry_run_works(self):
        """Dry run returns without error."""
        result = run_sync(["--dry-run"])
        assert result.returncode == 0

    def test_sync_engine_imports(self):
        """All modules can be imported."""
        import sys
        sys.path.insert(0, str(PLAYBOOKDATA / "scripts"))

        from sync_lock import SyncLock
        from sync_state import SyncState
        from validate_sync import validate_harvested_input
        from extract_structure import extract_semantic_fields
        from update_indexes import update_per_kb_index
        from update_cross_refs import extract_cross_references
        from sync_engine import SyncEngine, find_pending_syncs
        from migrate_content import migrate_kb_file

        # Basic instantiation
        engine = SyncEngine()
        assert engine is not None

    def test_migrate_content_imports(self):
        """Migrate content module can be imported."""
        import sys
        sys.path.insert(0, str(PLAYBOOKDATA / "scripts"))

        from migrate_content import migrate_kb_file, migrate_kb

        # Functions are callable
        assert callable(migrate_kb_file)
        assert callable(migrate_kb)

    def test_full_workflow_simulation(self):
        """Simulate full workflow with temporary KB."""
        import sys
        sys.path.insert(0, str(PLAYBOOKDATA / "scripts"))

        from sync_engine import SyncEngine, find_pending_syncs
        from update_indexes import update_per_kb_index, update_kb_manifest
        from extract_structure import extract_semantic_fields

        with tempfile.TemporaryDirectory() as tmpdir:
            kb_dir = Path(tmpdir) / "test-kb" / "topic"
            kb_dir.mkdir(parents=True)

            # Create a manifest
            manifest = {
                "kb_name": "test-kb",
                "topics": [{
                    "name": "topic",
                    "files": {
                        "test.json": {
                            "status": "harvested",
                            "harvested_at": "2026-03-30T12:00:00Z"
                        }
                    }
                }]
            }
            manifest_path = Path(tmpdir) / "test-kb" / "manifest.json"
            manifest_path.parent.mkdir(parents=True, exist_ok=True)
            manifest_path.write_text(json.dumps(manifest))

            # Test find_pending_syncs
            pending = find_pending_syncs(Path(tmpdir))
            assert len(pending) == 1
            assert pending[0]["kb"] == "test-kb"

            # Test semantic extraction
            markdown = """# Test Topic

This is a test topic description.

## Concepts

- **TestConcept**: A test concept

## Code

```cpp
int main() { return 0; }
```
"""
            semantic = extract_semantic_fields(markdown)
            assert "summary" in semantic
            assert len(semantic["concepts"]) > 0
            assert len(semantic["code_blocks"]) > 0