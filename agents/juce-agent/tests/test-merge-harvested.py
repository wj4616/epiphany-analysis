#!/usr/bin/env python3
"""Test merge-harvested.py functionality."""
import json
import sys
import tempfile
from pathlib import Path

# Derive paths from script location
SCRIPT_DIR = Path(__file__).parent.resolve()
AGENT_ROOT = SCRIPT_DIR.parent

# Add scripts path using importlib to handle hyphen in filename
import importlib.util

def test_find_best_result():
    """Test finding best result from harvested content."""
    # Load merge_harvested module using importlib (filename has hyphen)
    merge_script = AGENT_ROOT / "playbookdata" / "scripts" / "merge-harvested.py"
    spec = importlib.util.spec_from_file_location(
        "merge_harvested",
        str(merge_script)
    )
    merge_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(merge_module)

    find_best_result = merge_module.find_best_result

    # Create temp file with test data
    with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
        json.dump({
            "result": {
                "data": [
                    {"markdown": "Short content", "url": "http://example.com/1"},
                    {"markdown": "Much longer content with more information", "url": "http://example.com/2"},
                    {"markdown": "Medium", "url": "http://example.com/3"}
                ]
            }
        }, f)
        temp_path = f.name

    try:
        result = find_best_result(Path(temp_path))
        assert result is not None, "Should find a result"
        assert "longer content" in result.get("markdown", ""), "Should pick longest"
        print("PASS: find_best_result selects longest content")
    finally:
        Path(temp_path).unlink()

def test_merge_dry_run():
    """Test merge dry run without writing."""
    merge_script = AGENT_ROOT / "playbookdata" / "scripts" / "merge-harvested.py"
    spec = importlib.util.spec_from_file_location(
        "merge_harvested",
        str(merge_script)
    )
    merge_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(merge_module)

    merge_harvested_content = merge_module.merge_harvested_content

    # This should fail gracefully if no harvested content exists
    result = merge_harvested_content("dsp-kb", "reverb", "test.json", dry_run=True)

    # Either success or error for missing content is acceptable
    assert "status" in result, "Should return status"
    print(f"PASS: merge returns status: {result['status']}")

def test_merge_missing_kb():
    """Test merge with non-existent KB."""
    merge_script = AGENT_ROOT / "playbookdata" / "scripts" / "merge-harvested.py"
    spec = importlib.util.spec_from_file_location(
        "merge_harvested",
        str(merge_script)
    )
    merge_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(merge_module)

    merge_harvested_content = merge_module.merge_harvested_content

    result = merge_harvested_content("nonexistent-kb", "topic", "file.json", dry_run=True)
    assert result["status"] == "error", "Should error for missing KB"
    print("PASS: merge errors on missing KB")

if __name__ == "__main__":
    print("=== Merge Harvested Tests ===")

    try:
        test_find_best_result()
        test_merge_dry_run()
        test_merge_missing_kb()
        print("\n=== All tests passed ===")
    except ImportError as e:
        print(f"SKIP: Cannot import merge_harvested module: {e}")
        print("This is expected if running before Task 1 is complete")
        sys.exit(0)
    except AssertionError as e:
        print(f"FAIL: {e}")
        sys.exit(1)