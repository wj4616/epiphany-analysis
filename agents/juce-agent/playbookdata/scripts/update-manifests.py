#!/usr/bin/env python3
"""
Update KB manifests with status tracking.

Scans each KB directory and creates/updates manifest.json files
with file-level status tracking.

Status values:
  - "placeholder": Empty markdown or contains "[Content to be harvested]"
  - "harvested": status field is "harvested"
  - "curated": status field is "curated"
  - "unknown": Other status or missing status field
"""

import json
import os
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Any

# KB directories
KB_ROOT = Path(__file__).parent.parent.resolve()
KB_DIRS = [
    "dsp-kb",
    "juce-kb",
    "testing-kb",
    "midi-kb",
    "sound-design-kb",
    "cpp-kb",
    "ui-kb",
    "cmake-kb"
]

# Files to skip
SKIP_FILES = {"index.json", "manifest.json", "validation.json"}


def get_file_status(file_path: Path) -> str:
    """
    Determine the status of a KB file.

    Returns:
        "placeholder" if markdown is empty/contains placeholder marker
        "harvested" if status field is "harvested"
        "curated" if status field is "curated"
        "unknown" otherwise
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)

        # Check if status field exists
        status = data.get("status", "")

        if status == "curated":
            return "curated"
        elif status == "harvested":
            return "harvested"
        elif status == "placeholder":
            return "placeholder"

        # Check markdown content for placeholder markers
        markdown = data.get("markdown", "")

        if not markdown or not markdown.strip():
            return "placeholder"

        if "[Content to be harvested]" in markdown:
            return "placeholder"

        # If there's substantial content but no explicit status
        if len(markdown.strip()) > 100:
            if status:
                return status
            return "unknown"

        return "placeholder"

    except json.JSONDecodeError:
        return "error"
    except Exception as e:
        print(f"  Error reading {file_path}: {e}")
        return "error"


def create_manifest(kb_name: str, kb_path: Path) -> Dict[str, Any]:
    """
    Create manifest for a KB directory.

    Args:
        kb_name: Name of the KB (e.g., "dsp-kb")
        kb_path: Path to the KB directory

    Returns:
        Manifest dictionary
    """
    manifest = {
        "kb_name": kb_name,
        "last_updated": datetime.now(timezone.utc).isoformat(),
        "phases": []
    }

    if not kb_path.exists():
        print(f"  Warning: KB directory not found: {kb_path}")
        return manifest

    # Get topic folders
    topic_folders = sorted([d for d in kb_path.iterdir() if d.is_dir()])

    for topic_folder in topic_folders:
        topic_name = topic_folder.name
        phase_entry = {
            "name": topic_name,
            "files": {}
        }

        # Get JSON files in topic folder
        json_files = sorted([
            f for f in topic_folder.glob("*.json")
            if f.name not in SKIP_FILES
        ])

        for json_file in json_files:
            file_status = get_file_status(json_file)
            phase_entry["files"][json_file.name] = {
                "status": file_status,
                "last_checked": datetime.now(timezone.utc).isoformat()
            }

        if phase_entry["files"]:
            manifest["phases"].append(phase_entry)

    return manifest


def update_manifests(dry_run: bool = False) -> Dict[str, Any]:
    """
    Update all KB manifests.

    Args:
        dry_run: If True, don't write files, just show what would change

    Returns:
        Summary dictionary
    """
    summary = {
        "kb_directories_processed": 0,
        "total_topics": 0,
        "total_files": 0,
        "status_counts": {
            "placeholder": 0,
            "harvested": 0,
            "curated": 0,
            "unknown": 0,
            "error": 0
        }
    }

    for kb_name in KB_DIRS:
        kb_path = KB_ROOT / kb_name
        manifest_path = kb_path / "manifest.json"

        print(f"\nProcessing {kb_name}...")

        if not kb_path.exists():
            print(f"  Skipping: Directory not found")
            continue

        # Create manifest
        manifest = create_manifest(kb_name, kb_path)

        # Count stats
        summary["kb_directories_processed"] += 1
        summary["total_topics"] += len(manifest["phases"])

        for phase in manifest["phases"]:
            for filename, file_info in phase["files"].items():
                summary["total_files"] += 1
                status = file_info["status"]
                if status in summary["status_counts"]:
                    summary["status_counts"][status] += 1

        # Write or show manifest
        if dry_run:
            print(f"  Would write: {manifest_path}")
            print(f"    Topics: {len(manifest['phases'])}")
            for phase in manifest["phases"]:
                print(f"    - {phase['name']}: {len(phase['files'])} files")
        else:
            with open(manifest_path, 'w', encoding='utf-8') as f:
                json.dump(manifest, f, indent=2)
            print(f"  Written: {manifest_path}")
            print(f"    Topics: {len(manifest['phases'])}")

    return summary


def main():
    """CLI interface."""
    import argparse

    parser = argparse.ArgumentParser(
        description="Update KB manifests with status tracking"
    )
    parser.add_argument(
        "--dry-run",
        "-n",
        action="store_true",
        help="Show what would be done without making changes"
    )
    parser.add_argument(
        "--kb",
        "-k",
        help="Process only specific KB (e.g., 'dsp-kb')"
    )
    parser.add_argument(
        "--status",
        "-s",
        help="Filter files by status (placeholder, harvested, curated)"
    )

    args = parser.parse_args()

    # Filter KB directories if specified
    global KB_DIRS
    if args.kb:
        if args.kb in KB_DIRS:
            KB_DIRS = [args.kb]
        else:
            print(f"Error: Unknown KB '{args.kb}'")
            print(f"Available KBs: {', '.join(KB_DIRS)}")
            sys.exit(1)

    print("=" * 60)
    print("KB Manifest Updater")
    print("=" * 60)

    if args.dry_run:
        print("DRY RUN - No changes will be made")

    summary = update_manifests(dry_run=args.dry_run)

    print("\n" + "=" * 60)
    print("Summary")
    print("=" * 60)
    print(f"KB directories processed: {summary['kb_directories_processed']}")
    print(f"Total topics: {summary['total_topics']}")
    print(f"Total files: {summary['total_files']}")
    print(f"\nStatus counts:")
    for status, count in summary["status_counts"].items():
        print(f"  {status}: {count}")

    if args.dry_run:
        print("\n(Dry run - no files were modified)")


if __name__ == "__main__":
    main()