#!/usr/bin/env python3
"""
Validation for KB sync operations.

Validates harvested input and semantic content before sync.
"""

import sys
from pathlib import Path
from typing import Tuple

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))


def validate_harvested_input(data: dict) -> Tuple[bool, str]:
    """
    Validate harvested JSON before sync.

    Args:
        data: Harvested JSON data

    Returns:
        Tuple of (is_valid, error_message)
    """
    if not data:
        return False, "Empty harvested content"

    if not isinstance(data, dict):
        return False, f"Harvested content must be dict, got {type(data).__name__}"

    # Check for content field (markdown or content)
    has_content = (
        data.get("content") or
        data.get("markdown") or
        data.get("result", {}).get("data") or
        data.get("result", {}).get("markdown")
    )

    if not has_content:
        return False, "No content field in harvested JSON"

    # Check for incomplete marker
    if data.get("_incomplete"):
        return False, "Harvested JSON marked as incomplete"

    return True, ""


def validate_semantic_content(semantic: dict) -> Tuple[bool, str]:
    """
    Validate extracted semantic content.

    Args:
        semantic: Extracted semantic dict

    Returns:
        Tuple of (is_valid, error_message)
    """
    if not semantic:
        return False, "Empty semantic content"

    if not isinstance(semantic, dict):
        return False, f"Semantic must be dict, got {type(semantic).__name__}"

    # Required: non-empty summary
    summary = semantic.get("summary", "")
    if not summary or not summary.strip():
        return False, "Empty summary in semantic extraction"

    # Concepts are optional - many valid files don't have concept-formatted content
    concepts = semantic.get("concepts", [])
    if concepts and not isinstance(concepts, list):
        return False, f"Concepts must be list, got {type(concepts).__name__}"

    # Validate code blocks if present
    code_blocks = semantic.get("code_blocks", [])
    for i, block in enumerate(code_blocks):
        if not isinstance(block, dict):
            return False, f"Code block {i} must be dict"

        if not block.get("code"):
            return False, f"Empty code block {i}"

        if not block.get("language"):
            return False, f"Code block {i} missing language"

    return True, ""


def validate_manifest_entry(entry: dict) -> Tuple[bool, str]:
    """
    Validate manifest entry before sync.

    Args:
        entry: Manifest entry dict

    Returns:
        Tuple of (is_valid, error_message)
    """
    if not entry:
        return False, "Empty manifest entry"

    status = entry.get("status")
    if status not in ("placeholder", "harvested", "synced", "curated", "failed"):
        return False, f"Invalid status: {status}"

    # For harvested status, require harvested_at
    if status == "harvested":
        if not entry.get("harvested_at"):
            return False, "Harvested entry missing harvested_at"

    return True, ""


if __name__ == "__main__":
    import json
    import sys

    if len(sys.argv) < 2:
        print("Usage: validate_sync.py <json_file>")
        sys.exit(1)

    file_path = sys.argv[1]
    data = json.loads(open(file_path).read())

    is_valid, error = validate_harvested_input(data)
    if not is_valid:
        print(f"Input validation failed: {error}")
        sys.exit(1)

    print("Validation passed")