#!/usr/bin/env python3
"""
Integrate existing KB content into the new sync system.

This script:
1. Scans existing KB files
2. Creates/updates manifests with proper structure
3. Marks existing files as 'synced' (they already have content)
4. Runs migration to add semantic fields where missing
"""

import json
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import Dict, List

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

KB_ROOT = Path(__file__).parent.parent


def scan_kb_files(kb_name: str) -> Dict:
    """
    Scan KB directory and return topic/file structure.

    Args:
        kb_name: KB directory name (e.g., "dsp-kb")

    Returns:
        Dict with topics and their files
    """
    kb_path = KB_ROOT / kb_name
    if not kb_path.exists():
        return {}

    topics = {}

    for item in kb_path.iterdir():
        if item.is_dir() and not item.name.startswith('.'):
            topic_name = item.name
            files = {}

            for json_file in item.glob("*.json"):
                if json_file.name in ("index.json", "manifest.json", "validation.json"):
                    continue

                # Read file to check if it has content
                try:
                    data = json.loads(json_file.read_text())
                    markdown = data.get("markdown", "")
                    has_content = len(markdown) > 100

                    files[json_file.name] = {
                        "status": "synced" if has_content else "placeholder",
                        "harvested_at": datetime.now(timezone.utc).isoformat() if has_content else None,
                        "synced_at": datetime.now(timezone.utc).isoformat() if has_content else None,
                        "synced_timestamp": int(datetime.now(timezone.utc).timestamp()) if has_content else None,
                        "has_semantic": all(k in data for k in ["summary", "concepts", "code_blocks", "references"]),
                        "source": data.get("source", "existing"),
                        "markdown_length": len(markdown)
                    }
                except:
                    files[json_file.name] = {
                        "status": "error",
                        "error": "Failed to parse JSON"
                    }

            # Also check for JSON files directly in topic dir (not in subdirs)
            if files:
                topics[topic_name] = {
                    "name": topic_name,
                    "files": files
                }

    return topics


def create_manifest(kb_name: str, topics: Dict) -> Dict:
    """
    Create manifest structure for KB.

    Args:
        kb_name: KB name
        topics: Topics dict from scan_kb_files

    Returns:
        Manifest dict
    """
    return {
        "kb_name": kb_name,
        "version": "1.0.0",
        "created": datetime.now(timezone.utc).isoformat(),
        "last_sync": datetime.now(timezone.utc).isoformat(),
        "status": "ready",
        "topics": list(topics.values())
    }


def update_index(kb_name: str, topics: Dict) -> Dict:
    """
    Create/update index structure for KB.

    Args:
        kb_name: KB name
        topics: Topics dict from scan_kb_files

    Returns:
        Index dict
    """
    index = {
        "kb_name": kb_name,
        "version": "1.0.0",
        "last_updated": datetime.now(timezone.utc).isoformat(),
        "topics": {}
    }

    for topic_name, topic_data in topics.items():
        files = topic_data.get("files", {})
        index["topics"][topic_name] = {
            "files": list(files.keys()),
            "file_count": len(files),
            "last_updated": datetime.now(timezone.utc).isoformat()
        }

    return index


def integrate_kb(kb_name: str, dry_run: bool = False) -> Dict:
    """
    Integrate existing KB into sync system.

    Args:
        kb_name: KB name (e.g., "dsp-kb")
        dry_run: Preview only

    Returns:
        Result dict with stats
    """
    kb_path = KB_ROOT / kb_name
    if not kb_path.exists():
        return {"error": f"KB not found: {kb_name}"}

    # Scan files
    topics = scan_kb_files(kb_name)

    if not topics:
        return {"error": f"No topics found in {kb_name}"}

    # Count stats
    total_files = 0
    synced_files = 0
    needs_migration = 0

    for topic_name, topic_data in topics.items():
        for fname, finfo in topic_data.get("files", {}).items():
            total_files += 1
            if finfo.get("status") == "synced":
                synced_files += 1
            if not finfo.get("has_semantic"):
                needs_migration += 1

    if dry_run:
        return {
            "kb": kb_name,
            "total_files": total_files,
            "synced_files": synced_files,
            "needs_migration": needs_migration,
            "topics": list(topics.keys()),
            "dry_run": True
        }

    # Create manifest
    manifest = create_manifest(kb_name, topics)
    manifest_path = kb_path / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2))

    # Create/update index
    index = update_index(kb_name, topics)
    index_path = kb_path / "index.json"

    # Merge with existing index if present
    if index_path.exists():
        try:
            existing = json.loads(index_path.read_text())
            if "topics" in existing:
                # Preserve existing topic metadata
                for topic_name in index["topics"]:
                    if topic_name in existing["topics"]:
                        existing_meta = existing["topics"][topic_name]
                        if isinstance(existing_meta, dict):
                            index["topics"][topic_name].update({
                                k: v for k, v in existing_meta.items()
                                if k not in ["files", "file_count", "last_updated"]
                            })
        except:
            pass

    index_path.write_text(json.dumps(index, indent=2))

    return {
        "kb": kb_name,
        "total_files": total_files,
        "synced_files": synced_files,
        "needs_migration": needs_migration,
        "topics": list(topics.keys()),
        "manifest": str(manifest_path),
        "index": str(index_path)
    }


def main():
    import argparse

    parser = argparse.ArgumentParser(description="Integrate existing KB content into sync system")
    parser.add_argument("kb", nargs="?", help="KB name (default: all)")
    parser.add_argument("--dry-run", action="store_true", help="Preview only")

    args = parser.parse_args()

    # Known KBs
    kb_list = ["dsp-kb", "juce-kb", "sound-design-kb", "ui-kb", "cpp-kb", "cmake-kb", "midi-kb", "testing-kb"]

    if args.kb:
        if args.kb not in kb_list:
            # Check if it's a valid directory
            if (KB_ROOT / args.kb).exists():
                kb_list = [args.kb]
            else:
                print(f"Unknown KB: {args.kb}")
                sys.exit(1)
        else:
            kb_list = [args.kb]

    results = []
    for kb_name in kb_list:
        print(f"\n{'[DRY RUN] ' if args.dry_run else ''}Integrating {kb_name}...")
        result = integrate_kb(kb_name, dry_run=args.dry_run)
        results.append(result)

        if "error" in result:
            print(f"  Error: {result['error']}")
        else:
            print(f"  Files: {result.get('total_files', 0)} total, {result.get('synced_files', 0)} synced")
            print(f"  Needs migration: {result.get('needs_migration', 0)}")
            if result.get('topics'):
                print(f"  Topics: {', '.join(result['topics'][:5])}{'...' if len(result['topics']) > 5 else ''}")

    # Summary
    total_files = sum(r.get('total_files', 0) for r in results if 'total_files' in r)
    total_synced = sum(r.get('synced_files', 0) for r in results if 'synced_files' in r)
    total_migration = sum(r.get('needs_migration', 0) for r in results if 'needs_migration' in r)

    print(f"\n{'[DRY RUN] ' if args.dry_run else ''}Summary:")
    print(f"  Total files: {total_files}")
    print(f"  Already synced: {total_synced}")
    print(f"  Need semantic migration: {total_migration}")

    if not args.dry_run and total_migration > 0:
        print(f"\nRun migration to add semantic fields:")
        print(f"  python3 scripts/migrate_content.py <kb> --dry-run  # Preview")
        print(f"  python3 scripts/migrate_content.py <kb>             # Execute")


if __name__ == "__main__":
    main()