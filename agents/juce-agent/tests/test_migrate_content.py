# tests/test_migrate_content.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import json

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from migrate_content import migrate_kb_file


def test_migrate_preserves_code_whitespace():
    """Migration preserves code block whitespace."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb" / "reverb"
        kb_dir.mkdir(parents=True)

        # Create file with markdown containing summary, concepts, and code
        kb_file = kb_dir / "test.json"
        kb_file.write_text(json.dumps({
            "id": "test",
            "markdown": """# Test Module

This is a test module for testing.

## Concepts

- **TestClass**: A test class for demonstration

## Code

```cpp
class Test {
    int x;
};
```"""
        }))

        result = migrate_kb_file("dsp-kb", "reverb", "test.json", kb_root=Path(tmpdir))

        assert result["status"] == "success"

        # Check file was updated
        data = json.loads(kb_file.read_text())
        assert "summary" in data
        assert "code_blocks" in data
        # Check whitespace is preserved in code block
        assert "    int x;" in data["code_blocks"][0]["code"]

def test_migrate_skips_already_migrated():
    """Already migrated files are skipped."""
    with tempfile.TemporaryDirectory() as tmpdir:
        kb_dir = Path(tmpdir) / "dsp-kb" / "reverb"
        kb_dir.mkdir(parents=True)

        kb_file = kb_dir / "test.json"
        kb_file.write_text(json.dumps({
            "id": "test",
            "summary": "Already migrated"
        }))

        result = migrate_kb_file("dsp-kb", "reverb", "test.json", kb_root=Path(tmpdir))

        assert result["status"] == "skipped"

def test_migrate_handles_missing_file():
    """Missing files return error."""
    with tempfile.TemporaryDirectory() as tmpdir:
        result = migrate_kb_file("dsp-kb", "reverb", "missing.json", kb_root=Path(tmpdir))
        assert result["status"] == "error"
        assert "not found" in result["error"]