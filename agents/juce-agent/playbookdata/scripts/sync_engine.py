#!/usr/bin/env python3
"""
Main sync engine for KB synchronization.

Orchestrates the full sync workflow:
1. Create lock directory
2. Acquire locks
3. Scan for changes
4. Validate input
5. Extract semantic content
6. Validate semantic
7. Create backup
8. Write temp files
9. Atomic rename
10. Update state
11. Release locks
"""

import json
import shutil
import sys
from pathlib import Path
from datetime import datetime, timezone
from typing import List, Dict, Optional, Tuple
import traceback

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

# Local imports
from sync_lock import SyncLock, MultiLock, acquire_kb_lock
from sync_state import SyncState
from validate_sync import validate_harvested_input, validate_semantic_content
from extract_structure import extract_semantic_fields
from update_indexes import update_per_kb_index, update_master_index, update_kb_manifest, KB_ROOT as INDEXES_KB_ROOT
from update_cross_refs import extract_cross_references, update_master_cross_refs

# Constants
KB_ROOT = Path(__file__).parent.parent
MASTER_INDEX = KB_ROOT / "master-index.json"
HARVESTED_RAW = KB_ROOT / "harvested" / "raw"
MAX_RETRIES = 3


def ensure_harvested_raw():
    """Ensure harvested/raw directory exists."""
    HARVESTED_RAW.mkdir(parents=True, exist_ok=True)


class SyncError(Exception):
    """Sync operation error."""
    pass


def find_pending_syncs(kb_root: Path = None) -> List[Dict]:
    """
    Find KB entries pending sync.

    Looks for manifest entries with:
    - status = "harvested"
    - No synced_timestamp

    Args:
        kb_root: KB root directory

    Returns:
        List of pending sync dicts with kb, topic, filename
    """
    if kb_root is None:
        kb_root = KB_ROOT

    pending = []

    for manifest_file in kb_root.glob("*/manifest.json"):
        try:
            manifest = json.loads(manifest_file.read_text())
        except (json.JSONDecodeError, IOError):
            continue

        kb = manifest.get("kb_name", manifest_file.parent.name)

        for topic_entry in manifest.get("topics", []):
            topic = topic_entry.get("name")
            if not topic:
                continue

            for filename, file_entry in topic_entry.get("files", {}).items():
                status = file_entry.get("status")
                synced_timestamp = file_entry.get("synced_timestamp")
                retry_count = file_entry.get("retry_count", 0)

                # Check if needs sync
                if status == "harvested" and not synced_timestamp:
                    pending.append({
                        "kb": kb,
                        "topic": topic,
                        "filename": filename,
                        "retry_count": retry_count
                    })
                # Check if needs retry
                elif status == "failed" and retry_count < MAX_RETRIES:
                    pending.append({
                        "kb": kb,
                        "topic": topic,
                        "filename": filename,
                        "retry_count": retry_count,
                        "retry": True
                    })

    return pending


class SyncEngine:
    """
    Main sync orchestration with transaction support.
    """

    def __init__(self, kb_root: Path = None):
        self.kb_root = kb_root or KB_ROOT
        self.state = SyncState(self.kb_root / "harvested")
        self.dry_run = False

    def sync_file(self, kb: str, topic: str, filename: str) -> Dict:
        """
        Sync single KB file with backup and recovery.

        Args:
            kb: KB name
            topic: Topic folder
            filename: JSON filename

        Returns:
            Result dict with status and details
        """
        kb_path = self.kb_root / kb / topic / filename
        index_path = self.kb_root / kb / "index.json"
        manifest_path = self.kb_root / kb / "manifest.json"

        if not kb_path.exists():
            return {"status": "error", "error": f"KB file not found: {kb_path}"}

        # Ensure harvested/raw directory exists
        ensure_harvested_raw()

        # Find harvested content
        harvested_files = list(HARVESTED_RAW.glob(f"{kb}_{topic}_{filename.replace('.json', '')}*.json"))

        # If no match, search all harvested files for matching metadata
        if not harvested_files:
            harvested_files = []
            for raw_file in HARVESTED_RAW.glob(f"{kb}_{topic}_*.json"):
                try:
                    data = json.loads(raw_file.read_text())
                    metadata = data.get("harvest_metadata", {})
                    if (metadata.get("kb_type") == kb and
                        metadata.get("topic") == topic and
                        metadata.get("filename") == filename):
                        harvested_files.append(raw_file)
                except:
                    continue

        if not harvested_files:
            return {"status": "error", "error": f"No harvested content found in {HARVESTED_RAW}"}

        # Read best harvested content
        harvested_data = self._read_best_harvested(harvested_files)

        # Validate input
        is_valid, error = validate_harvested_input(harvested_data)
        if not is_valid:
            return {"status": "error", "error": f"Input validation failed: {error}"}

        # Extract semantic content
        markdown = harvested_data.get("_combined_markdown", "")
        if not markdown:
            markdown = harvested_data.get("result", {}).get("markdown", "")
        if not markdown:
            markdown = harvested_data.get("markdown", "")

        semantic = extract_semantic_fields(markdown)

        # Validate semantic
        is_valid, error = validate_semantic_content(semantic)
        if not is_valid:
            return {"status": "error", "error": f"Semantic validation failed: {error}"}

        if self.dry_run:
            return {
                "status": "dry_run",
                "kb": kb,
                "topic": topic,
                "filename": filename,
                "semantic_preview": {
                    "summary": semantic["summary"][:100] + "...",
                    "concepts_count": len(semantic["concepts"]),
                    "code_blocks_count": len(semantic["code_blocks"])
                }
            }

        # Create backups
        backup_kb = kb_path.with_suffix(".json.backup")
        shutil.copy(kb_path, backup_kb)

        temp_files = []
        try:
            # Build new KB content
            kb_data = json.loads(kb_path.read_text())
            kb_data.update({
                "status": "synced",
                "synced_at": datetime.now(timezone.utc).isoformat(),
                "summary": semantic["summary"],
                "concepts": semantic["concepts"],
                "code_blocks": semantic["code_blocks"],
                "references": semantic["references"]
            })

            # Write to temp files
            temp_kb = kb_path.with_suffix(".json.tmp")
            temp_kb.write_text(json.dumps(kb_data, indent=2))
            temp_files.append(temp_kb)

            # Atomic rename
            temp_kb.rename(kb_path)

            # Update indexes
            update_per_kb_index(kb, topic, filename, index_path)
            update_kb_manifest(kb, topic, filename, "synced", kb_root=self.kb_root)

            # Update cross-references
            refs = harvested_data.get("result", {}).get("references", [])
            if refs:
                cross_refs = extract_cross_references(refs)
                if cross_refs:
                    update_master_cross_refs(MASTER_INDEX, cross_refs)

            # Clean backup
            backup_kb.unlink()

            return {
                "status": "success",
                "kb": kb,
                "topic": topic,
                "filename": filename
            }

        except Exception as e:
            # Restore from backup
            if backup_kb.exists():
                shutil.copy(backup_kb, kb_path)
                backup_kb.unlink()

            # Clean temp files
            for temp in temp_files:
                if temp.exists():
                    temp.unlink()

            return {"status": "error", "error": str(e)}

    def _read_best_harvested(self, harvested_files: List[Path]) -> Dict:
        """Read the harvested file with most content."""
        best = None
        best_length = 0

        for hf in harvested_files:
            try:
                data = json.loads(hf.read_text())
                result = data.get("result", {})

                # Check for direct markdown
                markdown = result.get("markdown", "")

                # If no direct markdown, check for data list
                if not markdown and isinstance(result.get("data"), list):
                    # Combine markdown from all results
                    combined = []
                    for item in result["data"]:
                        if isinstance(item, dict) and item.get("markdown"):
                            combined.append(f"# {item.get('title', 'Untitled')}\n\n{item['markdown']}")
                    markdown = "\n\n---\n\n".join(combined)

                if len(markdown) > best_length:
                    best = data
                    best_length = len(markdown)
                    # Store combined markdown for later
                    data["_combined_markdown"] = markdown
            except:
                continue

        return best or {}

    def sync_all(self, kb_filter: str = None) -> Dict:
        """
        Sync all pending entries.

        Args:
            kb_filter: Optional KB name to filter

        Returns:
            Summary dict
        """
        pending = find_pending_syncs(self.kb_root)

        if kb_filter:
            pending = [p for p in pending if p["kb"] == kb_filter]

        results = []
        succeeded = 0
        failed = 0

        for entry in pending:
            with acquire_kb_lock(entry["kb"]):
                result = self.sync_file(
                    entry["kb"],
                    entry["topic"],
                    entry["filename"]
                )
                results.append(result)

                if result["status"] == "success":
                    succeeded += 1
                else:
                    failed += 1
                    self.state.record_error(
                        f"{entry['kb']}/{entry['topic']}/{entry['filename']}",
                        result.get("error", "Unknown error")
                    )

        self.state.record_run(len(results), succeeded, failed)

        return {
            "total": len(results),
            "succeeded": succeeded,
            "failed": failed,
            "results": results
        }


def main():
    import argparse

    parser = argparse.ArgumentParser(description="KB Sync Engine")
    parser.add_argument("--run", action="store_true", help="Run sync")
    parser.add_argument("--kb", help="Sync specific KB only")
    parser.add_argument("--dry-run", action="store_true", help="Preview without changes")
    parser.add_argument("--status", action="store_true", help="Show status")
    parser.add_argument("--clean-backups", action="store_true", help="Remove backup files older than 30 days")

    args = parser.parse_args()

    engine = SyncEngine()
    engine.dry_run = args.dry_run

    if args.status:
        pending = find_pending_syncs()
        print(f"Pending syncs: {len(pending)}")
        for p in pending[:10]:
            print(f"  {p['kb']}/{p['topic']}/{p['filename']}")

    elif args.clean_backups:
        # Remove backup files older than 30 days
        import time
        cutoff = time.time() - (30 * 24 * 60 * 60)
        removed = 0
        for backup in KB_ROOT.rglob("*.json.backup"):
            if backup.stat().st_mtime < cutoff:
                backup.unlink()
                removed += 1
        print(f"Removed {removed} old backup files")

    elif args.dry_run and not args.run:
        # Show what would sync without executing
        pending = find_pending_syncs()
        print(f"Pending syncs: {len(pending)}")
        if pending:
            print("Would sync:")
            for p in pending[:10]:
                print(f"  {p['kb']}/{p['topic']}/{p['filename']}")
            if len(pending) > 10:
                print(f"  ... and {len(pending) - 10} more")

    elif args.run:
        result = engine.sync_all(kb_filter=args.kb)
        print(json.dumps(result, indent=2))

    else:
        print("Use --run, --status, --dry-run, or --clean-backups")


if __name__ == "__main__":
    main()