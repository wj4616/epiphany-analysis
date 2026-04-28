#!/usr/bin/env python3
"""Validate KB structure: check for orphan files, missing files, index consistency.

Usage:
    python3 scripts/validate-kb-structure.py

Exit codes:
    0 - All KBs valid
    1 - Validation errors found
"""

import json
import os
import sys
from pathlib import Path


def validate_kb(kb_path):
    """Validate a KB directory.

    Checks:
    1. index.json exists and is valid JSON
    2. All files referenced in index exist
    3. All .json files (except index/manifest) are referenced in index
    4. File count matches metadata

    Args:
        kb_path: Path to KB directory

    Returns:
        List of error messages (empty if valid)
    """
    errors = []

    # Load index
    index_path = os.path.join(kb_path, "index.json")
    if not os.path.exists(index_path):
        return [f"Missing index.json in {kb_path}"]

    try:
        with open(index_path) as f:
            index = json.load(f)
    except json.JSONDecodeError as e:
        return [f"Invalid JSON in {kb_path}/index.json: {e}"]

    # Get all indexed files
    indexed_files = set()
    for topic, files in index.get("topics", {}).items():
        for file in files:
            indexed_files.add(file)

    # Get all actual files
    actual_files = set()
    for root, dirs, files in os.walk(kb_path):
        for f in files:
            if f.endswith(".json") and f not in ["index.json", "manifest.json", "validation.json"]:
                rel = os.path.relpath(os.path.join(root, f), kb_path)
                actual_files.add(rel)

    # Check for orphans (files in directory but not in index)
    orphans = actual_files - indexed_files
    if orphans:
        errors.append(f"Orphan files in {kb_path}: {sorted(orphans)}")

    # Check for missing (files in index but not in directory)
    missing = indexed_files - actual_files
    if missing:
        errors.append(f"Missing files in {kb_path}: {sorted(missing)}")

    # Check file count
    metadata_count = index.get("metadata", {}).get("total_files", 0)
    if metadata_count != len(actual_files):
        errors.append(
            f"File count mismatch in {kb_path}: "
            f"index says {metadata_count}, actual {len(actual_files)}"
        )

    return errors


def validate_master_index():
    """Validate master-index.json references to KBs.

    Returns:
        List of error messages (empty if valid)
    """
    errors = []

    master_path = "master-index.json"
    if not os.path.exists(master_path):
        return [f"Missing {master_path}"]

    try:
        with open(master_path) as f:
            master = json.load(f)
    except json.JSONDecodeError as e:
        return [f"Invalid JSON in {master_path}: {e}"]

    # Check each KB exists
    for kb_name, kb_info in master.get("knowledge_bases", {}).items():
        kb_path = kb_info.get("path", "")
        if not os.path.exists(kb_path):
            errors.append(f"KB '{kb_name}' path '{kb_path}' does not exist")

    # Check cross-references reference valid KBs
    valid_kbs = set(master.get("knowledge_bases", {}).keys())
    for term, ref in master.get("cross_references", {}).items():
        for kb in ref.get("kbs", []):
            if kb not in valid_kbs:
                errors.append(
                    f"Cross-reference '{term}' references unknown KB '{kb}'"
                )

    return errors


def main():
    """Run validation on all KBs."""
    # Get playbookdata directory from script location
    script_dir = Path(__file__).parent.resolve()
    playbookdata_dir = script_dir.parent

    # Change to playbookdata directory
    os.chdir(playbookdata_dir)

    kb_dirs = [
        "juce-kb",
        "dsp-kb",
        "sound-design-kb",
        "ui-kb",
        "cpp-kb",
        "cmake-kb",
        "testing-kb",
        "midi-kb",
    ]

    all_errors = []

    # Validate each KB
    for kb in kb_dirs:
        if os.path.exists(kb):
            errors = validate_kb(kb)
            all_errors.extend(errors)
        else:
            all_errors.append(f"KB directory '{kb}' does not exist")

    # Validate master index
    master_errors = validate_master_index()
    all_errors.extend(master_errors)

    if all_errors:
        print("VALIDATION ERRORS:")
        for e in all_errors:
            print(f"  - {e}")
        sys.exit(1)
    else:
        print("All KBs valid!")
        sys.exit(0)


if __name__ == "__main__":
    main()