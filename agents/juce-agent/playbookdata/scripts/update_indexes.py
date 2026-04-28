#!/usr/bin/env python3
"""
Update KB indexes after sync.

Updates:
- Per-KB index.json with topic-to-file mappings
- Master index with file counts and metadata
"""

import json
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import Optional

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

# Default KB root
KB_ROOT = Path(__file__).parent.parent


def get_kb_file_count(kb_path: Path) -> int:
    """
    Count KB files, excluding index.json and manifest.json.

    Args:
        kb_path: Path to KB directory

    Returns:
        Number of KB content files
    """
    count = 0

    for json_file in kb_path.rglob("*.json"):
        name = json_file.name
        if name not in ("index.json", "manifest.json", "validation.json"):
            count += 1

    return count


def update_per_kb_index(kb: str, topic: str, filename: str,
                        index_path: Path = None) -> bool:
    """
    Update per-KB index with file entry.

    Args:
        kb: KB name (e.g., "dsp-kb")
        topic: Topic folder (e.g., "reverb")
        filename: JSON filename
        index_path: Path to index.json (default: kb/index.json)

    Returns:
        True if successful
    """
    if index_path is None:
        index_path = Path(kb) / "index.json"

    # Load or create index
    if index_path.exists():
        try:
            index = json.loads(index_path.read_text())
        except (json.JSONDecodeError, IOError):
            index = {}
    else:
        index = {}

    # Ensure structure
    if "kb_name" not in index:
        index["kb_name"] = kb
    if "topics" not in index:
        index["topics"] = {}

    # Add topic if not exists
    if topic not in index["topics"]:
        index["topics"][topic] = {
            "files": [],
            "last_updated": None
        }

    # Add file if not exists
    if filename not in index["topics"][topic]["files"]:
        index["topics"][topic]["files"].append(filename)

    index["topics"][topic]["last_updated"] = datetime.now(timezone.utc).isoformat()
    index["last_updated"] = datetime.now(timezone.utc).isoformat()

    # Ensure parent directory exists
    index_path.parent.mkdir(parents=True, exist_ok=True)

    # Write atomically
    temp_path = index_path.with_suffix(".json.tmp")
    temp_path.write_text(json.dumps(index, indent=2))
    temp_path.rename(index_path)

    return True


def update_master_index(master_path: Path, kb: str,
                        file_count: Optional[int] = None,
                        topics: Optional[list] = None) -> bool:
    """
    Update master index with KB metadata.

    Args:
        master_path: Path to master-index.json
        kb: KB name
        file_count: Optional file count to update
        topics: Optional topics list to update

    Returns:
        True if successful
    """
    if not master_path.exists():
        return False

    try:
        index = json.loads(master_path.read_text())
    except (json.JSONDecodeError, IOError):
        return False

    # Update KB entry
    if "knowledge_bases" in index and kb in index["knowledge_bases"]:
        if file_count is not None:
            index["knowledge_bases"][kb]["file_count"] = file_count
        if topics is not None:
            index["knowledge_bases"][kb]["topics"] = topics

    index["last_updated"] = datetime.now(timezone.utc).isoformat()

    # Write atomically
    temp_path = master_path.with_suffix(".json.tmp")
    temp_path.write_text(json.dumps(index, indent=2))
    temp_path.rename(master_path)

    return True


def update_kb_manifest(kb: str, topic: str, filename: str,
                       status: str, credits_used: int = 0,
                       sources: list = None, kb_root: Path = None) -> bool:
    """
    Update KB manifest with sync status.

    Args:
        kb: KB name (e.g., "dsp-kb")
        topic: Topic folder (e.g., "reverb")
        filename: JSON filename (e.g., "algorithmic-reverb.json")
        status: New status
        credits_used: Credits used for harvest
        sources: Source domains
        kb_root: KB root directory (default: playbookdata/)

    Returns:
        True if successful
    """
    if kb_root is None:
        kb_root = KB_ROOT

    manifest_path = kb_root / kb / "manifest.json"

    # Load or create manifest
    if manifest_path.exists():
        try:
            manifest = json.loads(manifest_path.read_text())
        except (json.JSONDecodeError, IOError):
            manifest = {"kb_name": kb, "topics": []}
    else:
        manifest = {"kb_name": kb, "topics": []}

    # Find or create topic entry
    topic_entry = None
    for t in manifest.get("topics", []):
        if t.get("name") == topic:
            topic_entry = t
            break

    if not topic_entry:
        topic_entry = {"name": topic, "files": {}}
        manifest.setdefault("topics", []).append(topic_entry)

    # Update file entry
    if "files" not in topic_entry:
        topic_entry["files"] = {}

    topic_entry["files"][filename] = {
        "status": status,
        "synced_at": datetime.now(timezone.utc).isoformat(),
        "synced_timestamp": int(datetime.now(timezone.utc).timestamp()),
        "credits_used": credits_used,
        "sources": sources or [],
        "retry_count": 0,
        "last_error": None
    }

    manifest["last_sync"] = datetime.now(timezone.utc).isoformat()

    # Write atomically
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    temp_path = manifest_path.with_suffix(".json.tmp")
    temp_path.write_text(json.dumps(manifest, indent=2))
    temp_path.rename(manifest_path)

    return True


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Update KB indexes")
    parser.add_argument("kb", help="KB name")
    parser.add_argument("--topic", help="Topic folder")
    parser.add_argument("--file", help="Filename")
    parser.add_argument("--count", type=int, help="File count")

    args = parser.parse_args()

    if args.topic and args.file:
        update_per_kb_index(args.kb, args.topic, args.file)
        print(f"Updated {args.kb}/index.json")

    if args.count:
        master_path = Path("master-index.json")
        update_master_index(master_path, args.kb, file_count=args.count)
        print(f"Updated master-index.json")