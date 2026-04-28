#!/usr/bin/env python3
"""
One-time migration for existing KB content.

Migrates markdown-based KB files to semantic structure.
"""

import json
import shutil
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import Dict

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

from extract_structure import extract_semantic_fields
from validate_sync import validate_semantic_content

KB_ROOT = Path(__file__).parent.parent


class MigrationError(Exception):
    """Migration operation error."""
    pass


def migrate_kb_file(kb: str, topic: str, filename: str,
                    kb_root: Path = None, dry_run: bool = False) -> Dict:
    """
    Migrate existing markdown-based KB file to semantic structure.

    Args:
        kb: KB name (e.g., "dsp-kb")
        topic: Topic folder (e.g., "reverb")
        filename: JSON filename (e.g., "algorithmic-reverb.json")
        kb_root: KB root directory (default: playbookdata/)
        dry_run: Preview without changes

    Returns:
        Result dict with status
    """
    if kb_root is None:
        kb_root = KB_ROOT

    kb_path = kb_root / kb / topic / filename

    if not kb_path.exists():
        return {"status": "error", "error": f"File not found: {kb_path}"}

    try:
        kb_data = json.loads(kb_path.read_text())
    except json.JSONDecodeError as e:
        return {"status": "error", "error": f"Invalid JSON: {e}"}

    # Check if already migrated
    if "markdown" not in kb_data:
        return {"status": "skipped", "reason": "No markdown field"}

    if "summary" in kb_data and "concepts" in kb_data:
        return {"status": "skipped", "reason": "Already migrated"}

    markdown = kb_data.get("markdown", "")
    if not markdown:
        return {"status": "error", "error": "Empty markdown field"}

    # Extract semantic fields
    semantic = extract_semantic_fields(markdown)

    # Validate
    is_valid, error = validate_semantic_content(semantic)
    if not is_valid:
        return {"status": "error", "error": f"Semantic validation failed: {error}"}

    if dry_run:
        return {
            "status": "dry_run",
            "kb": kb,
            "topic": topic,
            "filename": filename,
            "preview": {
                "summary": semantic["summary"][:100] + "...",
                "concepts": len(semantic["concepts"]),
                "code_blocks": len(semantic["code_blocks"])
            }
        }

    # Create backup
    backup_path = kb_path.with_suffix(".json.backup")
    shutil.copy(kb_path, backup_path)

    try:
        # Build new content
        new_content = {
            "id": kb_data.get("id", f"{kb}_{topic}_{filename.replace('.json', '')}"),
            "kb": kb,
            "topic": topic,
            "status": "synced",
            "version": "2.0.0",
            "summary": semantic["summary"],
            "concepts": semantic["concepts"],
            "code_blocks": semantic["code_blocks"],
            "references": semantic["references"],
            "original_markdown": markdown,  # Keep for rollback
            "metadata": {
                "migrated_from": "markdown",
                "migrated_at": datetime.now(timezone.utc).isoformat()
            }
        }

        # Preserve any additional fields from original
        for key, value in kb_data.items():
            if key not in new_content and key not in ("markdown", "summary", "concepts", "code_blocks", "references"):
                new_content[key] = value

        # Write atomically
        temp_path = kb_path.with_suffix(".json.tmp")
        temp_path.write_text(json.dumps(new_content, indent=2))
        temp_path.rename(kb_path)

        return {
            "status": "success",
            "kb": kb,
            "topic": topic,
            "filename": filename,
            "backup": str(backup_path)
        }

    except Exception as e:
        # Restore backup
        if backup_path.exists():
            shutil.copy(backup_path, kb_path)

        return {"status": "error", "error": str(e)}


def migrate_kb(kb: str, kb_root: Path = None, dry_run: bool = False) -> Dict:
    """
    Migrate all files in a KB.

    Args:
        kb: KB name (e.g., "dsp-kb")
        kb_root: KB root directory (default: playbookdata/)
        dry_run: Preview without changes

    Returns:
        Summary dict with succeeded/skipped/failed counts
    """
    if kb_root is None:
        kb_root = KB_ROOT

    kb_path = kb_root / kb
    if not kb_path.exists():
        return {"status": "error", "error": f"KB not found: {kb}"}

    results = []
    succeeded = 0
    skipped = 0
    failed = 0

    for topic_dir in kb_path.iterdir():
        if not topic_dir.is_dir():
            continue

        topic = topic_dir.name

        for json_file in topic_dir.glob("*.json"):
            if json_file.name in ("index.json", "manifest.json", "validation.json"):
                continue

            result = migrate_kb_file(kb, topic, json_file.name, kb_root, dry_run)
            results.append(result)

            if result["status"] == "success":
                succeeded += 1
            elif result["status"] == "skipped":
                skipped += 1
            else:
                failed += 1

    return {
        "kb": kb,
        "total": len(results),
        "succeeded": succeeded,
        "skipped": skipped,
        "failed": failed,
        "results": results
    }


def main():
    import argparse

    parser = argparse.ArgumentParser(description="Migrate KB content")
    parser.add_argument("kb", help="KB to migrate")
    parser.add_argument("--topic", help="Specific topic")
    parser.add_argument("--file", help="Specific file")
    parser.add_argument("--dry-run", action="store_true", help="Preview")
    parser.add_argument("--all", action="store_true", help="Migrate all KBs")

    args = parser.parse_args()

    if args.all:
        for kb in ["dsp-kb", "juce-kb", "testing-kb", "midi-kb", "sound-design-kb", "ui-kb", "cpp-kb", "cmake-kb"]:
            print(f"\nMigrating {kb}...")
            result = migrate_kb(kb, dry_run=args.dry_run)
            print(f"  Succeeded: {result['succeeded']}, Skipped: {result['skipped']}, Failed: {result['failed']}")
    elif args.topic and args.file:
        result = migrate_kb_file(args.kb, args.topic, args.file, dry_run=args.dry_run)
        print(json.dumps(result, indent=2))
    else:
        result = migrate_kb(args.kb, dry_run=args.dry_run)
        print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()