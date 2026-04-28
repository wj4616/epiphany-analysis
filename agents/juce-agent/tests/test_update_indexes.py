# tests/test_update_indexes.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import json

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from update_indexes import (
    update_per_kb_index,
    update_master_index,
    get_kb_file_count
)


def test_update_per_kb_index_adds_file():
    """Per-KB index gets file added."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb" / "reverb"
        kb_dir.mkdir(parents=True)

        # Create KB file
        kb_file = kb_dir / "test.json"
        json.dump({"id": "test"}, open(kb_file, 'w'))

        # Create index
        index_file = Path(tmpdir) / "dsp-kb" / "index.json"

        update_per_kb_index("dsp-kb", "reverb", "test.json", index_file)

        assert index_file.exists()
        index = json.loads(index_file.read_text())
        assert "reverb" in index.get("topics", {})

def test_get_kb_file_count_counts_files():
    """File count excludes index and manifest."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb" / "reverb"
        kb_dir.mkdir(parents=True)

        # Create files
        (kb_dir / "file1.json").write_text("{}")
        (kb_dir / "file2.json").write_text("{}")
        (kb_dir / "index.json").write_text("{}")  # Should not count
        (kb_dir / "manifest.json").write_text("{}")  # Should not count

        count = get_kb_file_count(Path(tmpdir) / "dsp-kb")
        assert count == 2

def test_update_master_index_updates_file_count():
    """Master index gets updated file count."""
    with tempfile.TemporaryDirectory() as tmpdir:
        master_index = Path(tmpdir) / "master-index.json"
        master_index.write_text(json.dumps({
            "knowledge_bases": {
                "dsp-kb": {"file_count": 0}
            }
        }))

        update_master_index(master_index, "dsp-kb", file_count=53)

        index = json.loads(master_index.read_text())
        assert index["knowledge_bases"]["dsp-kb"]["file_count"] == 53

def test_update_kb_manifest_creates_entry():
    """KB manifest creates file entry."""
    from update_indexes import update_kb_manifest

    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb"
        kb_dir.mkdir()

        # Create manifest
        manifest_file = kb_dir / "manifest.json"
        manifest_file.write_text(json.dumps({"kb_name": "dsp-kb", "topics": []}))

        result = update_kb_manifest("dsp-kb", "reverb", "test.json", "synced", kb_root=Path(tmpdir))

        assert result == True
        manifest = json.loads(manifest_file.read_text())

        # Check topic was added
        topics = [t["name"] for t in manifest.get("topics", [])]
        assert "reverb" in topics

        # Check file entry
        for topic in manifest["topics"]:
            if topic["name"] == "reverb":
                assert "test.json" in topic["files"]
                assert topic["files"]["test.json"]["status"] == "synced"