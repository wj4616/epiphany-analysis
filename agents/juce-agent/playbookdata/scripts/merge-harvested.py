#!/usr/bin/env python3
"""
Merge harvested content into KB placeholder files.

Reads raw Firecrawl responses from harvested/raw/ and updates
the corresponding KB file with markdown content.
"""

import json
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import Optional
import argparse

# Constants
PREVIEW_LENGTH = 500

# Directory structure
SCRIPT_DIR = Path(__file__).parent.resolve()
KB_ROOT = SCRIPT_DIR.parent
RAW_DIR = KB_ROOT / "harvested" / "raw"
MANIFEST_DIR = KB_ROOT / "harvested" / "manifests"


def validate_path_component(component: str, name: str) -> None:
    """
    Validate a path component to prevent path traversal attacks.

    Args:
        component: The path component to validate
        name: Name of the component for error messages

    Raises:
        ValueError: If component contains invalid characters
    """
    if not component:
        raise ValueError(f"{name} cannot be empty")

    # Reject path traversal and path separators
    dangerous_chars = ['..', '/', '\\']
    for char in dangerous_chars:
        if char in component:
            raise ValueError(
                f"Invalid {name}: '{component}' contains forbidden sequence '{char}'"
            )

    # Only allow alphanumeric, dash, underscore
    import re
    if not re.match(r'^[a-zA-Z0-9_-]+$', component):
        raise ValueError(
            f"Invalid {name}: '{component}' must contain only alphanumeric, dash, or underscore"
        )


def find_best_result(raw_file: Path) -> Optional[dict]:
    """
    Find the result with longest markdown content.

    Args:
        raw_file: Path to raw JSON file

    Returns:
        Best result dict or None
    """
    try:
        with open(raw_file, encoding='utf-8') as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"Warning: Raw file not found: {raw_file}", file=sys.stderr)
        return None
    except PermissionError:
        print(f"Warning: Permission denied reading: {raw_file}", file=sys.stderr)
        return None
    except json.JSONDecodeError as e:
        print(f"Warning: Invalid JSON in {raw_file}: {e}", file=sys.stderr)
        return None

    result = data.get("result", {})
    items = result.get("data", result.get("results", []))

    if not items:
        return None

    # Find item with longest markdown
    best = None
    best_length = 0

    for item in items:
        if not isinstance(item, dict):
            continue

        markdown = item.get("markdown", "")
        if len(markdown) > best_length:
            best = item
            best_length = len(markdown)

    return best


def merge_harvested_content(kb: str, topic: str, filename: str,
                           dry_run: bool = False) -> dict:
    """
    Merge harvested content into KB placeholder file.

    Args:
        kb: KB type (e.g., "dsp-kb")
        topic: Topic folder (e.g., "reverb")
        filename: JSON filename (e.g., "algorithmic-reverb.json")
        dry_run: If True, don't write changes

    Returns:
        Dict with merge result
    """
    # Validate path components to prevent path traversal
    try:
        validate_path_component(kb, "kb")
        validate_path_component(topic, "topic")
        # For filename, allow .json extension
        base_name = filename.replace('.json', '')
        validate_path_component(base_name, "filename")
    except ValueError as e:
        return {
            "status": "error",
            "error": f"Path validation failed: {e}"
        }

    # Construct paths
    kb_file = KB_ROOT / kb / topic / filename

    # Find harvested files for this KB entry
    # First try filename pattern match
    raw_pattern = f"{kb}_{topic}_{filename.replace('.json', '')}"
    matching_files = list(RAW_DIR.glob(f"{raw_pattern}*.json"))

    # If no match, search all harvested files for matching metadata
    if not matching_files:
        matching_files = []
        for raw_file in RAW_DIR.glob(f"{kb}_{topic}_*.json"):
            try:
                with open(raw_file, encoding='utf-8') as f:
                    data = json.load(f)
                metadata = data.get("harvest_metadata", {})
                if (metadata.get("kb_type") == kb and
                    metadata.get("topic") == topic and
                    metadata.get("filename") == filename):
                    matching_files.append(raw_file)
            except:
                continue

    if not matching_files:
        return {
            "status": "error",
            "error": f"No harvested content found for {kb}/{topic}/{filename}",
            "searched": str(RAW_DIR / f"{raw_pattern}*.json")
        }

    # Find the best result (longest markdown)
    best_content = None
    best_length = 0
    best_file = None
    total_results = 0

    for raw_file in matching_files:
        result = find_best_result(raw_file)
        if result:
            total_results += 1
            markdown = result.get("markdown", "")
            if len(markdown) > best_length:
                best_content = markdown
                best_length = len(markdown)
                best_file = raw_file

    if not best_content:
        return {
            "status": "error",
            "error": "No markdown content found in harvested files",
            "files_checked": len(matching_files)
        }

    # Read KB file
    if not kb_file.exists():
        return {
            "status": "error",
            "error": f"KB file not found: {kb_file}"
        }

    try:
        with open(kb_file, encoding='utf-8') as f:
            kb_data = json.load(f)
    except FileNotFoundError:
        return {
            "status": "error",
            "error": f"KB file not found: {kb_file}"
        }
    except PermissionError:
        return {
            "status": "error",
            "error": f"Permission denied reading: {kb_file}"
        }
    except json.JSONDecodeError as e:
        return {
            "status": "error",
            "error": f"Invalid JSON in KB file {kb_file}: {e}"
        }

    # Validate KB file structure
    if not isinstance(kb_data, dict):
        return {
            "status": "error",
            "error": f"KB file must contain a JSON object, got {type(kb_data).__name__}"
        }

    # Check current status
    current_status = kb_data.get("status", kb_data.get("source", "placeholder"))
    if current_status == "curated":
        return {
            "status": "warning",
            "warning": f"KB file already curated, skipping merge",
            "file": str(kb_file)
        }

    # Prepare merge result
    merge_result = {
        "status": "success",
        "kb": kb,
        "topic": topic,
        "filename": filename,
        "source_file": str(best_file),
        "content_length": best_length,
        "total_sources": total_results,
        "previous_status": current_status,
        "new_status": "harvested",
        "timestamp": datetime.now(timezone.utc).isoformat()
    }

    if dry_run:
        merge_result["dry_run"] = True
        merge_result["preview"] = best_content[:PREVIEW_LENGTH] + "..." if len(best_content) > PREVIEW_LENGTH else best_content
        return merge_result

    # Update KB file
    kb_data["markdown"] = best_content
    kb_data["status"] = "harvested"
    kb_data["harvested_at"] = datetime.now(timezone.utc).isoformat()
    kb_data["harvested_source"] = str(best_file.name)

    # Write back
    try:
        with open(kb_file, 'w', encoding='utf-8') as f:
            json.dump(kb_data, f, indent=2)
    except PermissionError:
        return {
            "status": "error",
            "error": f"Permission denied writing to: {kb_file}"
        }
    except IOError as e:
        return {
            "status": "error",
            "error": f"Failed to write KB file: {e}"
        }

    # Update manifest
    update_manifest(kb, topic, filename, merge_result)

    return merge_result


def update_manifest(kb: str, topic: str, filename: str, merge_result: dict) -> bool:
    """
    Update manifest with harvested status.

    Args:
        kb: KB type
        topic: Topic folder
        filename: JSON filename
        merge_result: Dict with merge result

    Returns:
        True if successful, False otherwise
    """
    manifest_path = KB_ROOT / kb / "manifest.json"

    # Load or create manifest
    manifest = None
    if manifest_path.exists():
        try:
            with open(manifest_path, encoding='utf-8') as f:
                manifest = json.load(f)
        except FileNotFoundError:
            manifest = None
        except PermissionError:
            print(f"Warning: Permission denied reading manifest: {manifest_path}", file=sys.stderr)
            return False
        except json.JSONDecodeError as e:
            print(f"Warning: Invalid JSON in manifest {manifest_path}: {e}", file=sys.stderr)
            return False

    # Validate manifest structure if loaded
    if manifest is not None:
        if not isinstance(manifest, dict):
            print(f"Warning: Manifest must be a JSON object, got {type(manifest).__name__}", file=sys.stderr)
            manifest = None

    # Create default manifest if needed
    if manifest is None:
        manifest = {
            "kb_name": kb,
            "phases": []
        }

    # Find or create phase entry
    phase_entry = None
    for phase in manifest.get("phases", []):
        if phase.get("name") == topic:
            phase_entry = phase
            break

    if not phase_entry:
        phase_entry = {"name": topic, "files": {}}
        manifest.setdefault("phases", []).append(phase_entry)

    # Update file status
    if "files" not in phase_entry:
        phase_entry["files"] = {}

    phase_entry["files"][filename] = {
        "status": merge_result["new_status"],
        "harvested_at": merge_result["timestamp"],
        "credits_used": merge_result.get("credits_used", 0),
        "sources": merge_result.get("total_sources", 1)
    }

    # Create parent directories if needed
    try:
        manifest_path.parent.mkdir(parents=True, exist_ok=True)
    except PermissionError:
        print(f"Warning: Permission denied creating directory: {manifest_path.parent}", file=sys.stderr)
        return False
    except IOError as e:
        print(f"Warning: Failed to create directory: {e}", file=sys.stderr)
        return False

    # Write back
    try:
        with open(manifest_path, 'w', encoding='utf-8') as f:
            json.dump(manifest, f, indent=2)
    except PermissionError:
        print(f"Warning: Permission denied writing manifest: {manifest_path}", file=sys.stderr)
        return False
    except IOError as e:
        print(f"Warning: Failed to write manifest: {e}", file=sys.stderr)
        return False

    return True


def main():
    parser = argparse.ArgumentParser(
        description="Merge harvested content into KB placeholder files"
    )
    parser.add_argument("kb", help="KB type (e.g., dsp-kb)")
    parser.add_argument("--topic", "-t", help="Topic folder")
    parser.add_argument("--file", "-f", help="JSON filename")
    parser.add_argument("--dry-run", action="store_true",
                       help="Preview merge without writing")
    parser.add_argument("--all", action="store_true",
                       help="Merge all harvested content for this KB")

    args = parser.parse_args()

    # Validate arguments
    if args.all:
        if args.topic or args.file:
            print("Warning: --topic and --file are ignored when --all is used")
    elif not args.topic or not args.file:
        parser.error("--topic and --file are required unless --all is specified")

    if args.all:
        # Find all harvested manifests for this KB
        manifest_pattern = f"{args.kb}_*.json"
        manifest_files = list(MANIFEST_DIR.glob(manifest_pattern))

        if not manifest_files:
            print(f"No harvested manifests found for {args.kb}")
            sys.exit(1)

        results = []
        for manifest_file in manifest_files:
            # Parse topic and filename from manifest
            parts = manifest_file.stem.split('_')
            if len(parts) >= 3:
                topic = parts[1]
                filename = parts[2] + ".json"
                result = merge_harvested_content(args.kb, topic, filename, args.dry_run)
                results.append(result)
                print(f"  {topic}/{filename}: {result['status']}")

        print(f"\nMerged {len(results)} files")
        return

    result = merge_harvested_content(args.kb, args.topic, args.file, args.dry_run)

    if result["status"] == "error":
        print(f"Error: {result['error']}")
        sys.exit(1)
    elif result["status"] == "warning":
        print(f"Warning: {result['warning']}")
    else:
        print(f"Merged {result['content_length']} chars from {result['total_sources']} sources")
        print(f"Status: {result['previous_status']} -> {result['new_status']}")

    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()