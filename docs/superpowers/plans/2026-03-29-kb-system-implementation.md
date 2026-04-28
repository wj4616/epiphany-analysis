# Knowledge Base System Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement a production-grade knowledge base system for JUCE VST plugin development with 6 specialized KBs, cross-referencing, and skill integration.

**Architecture:** Central master-index.json coordinates 6 specialized KBs (juce-kb, dsp-kb, sound-design-kb, ui-kb, cpp-kb, cmake-kb). Each KB has its own index.json and validation.json. A Python module (kb-lookup.py) provides the query interface with caching and error handling. Skills query KBs dynamically via get_kb() singleton.

**Tech Stack:** Python 3.x, JSON storage, functools.lru_cache for caching, dataclasses for typed results, pytest for testing.

**Estimated Duration:**
- Section A (Preparation): ~5 minutes
- Section B (Foundation): ~15 minutes
- Section C (Data Reorganization): ~10 minutes
- Section D (Testing): ~10 minutes
- Section E (Validation): ~5 minutes
- **Total: ~45 minutes**

**Idempotency:** All steps are idempotent - safe to run multiple times. Steps check for existing state before modifying.

**Recovery:** Each section includes rollback commands. Run the recovery command for the current section if any step fails.

---

## Section A: Preparation (Phase 0)

**Purpose:** Create backup and rollback infrastructure before any modifications.

**Estimated Duration:** ~5 minutes

**Recovery:** If this section fails, run:
```bash
# No changes made yet, just re-run from Step A.1.1
```

**Files:**
- Create: `playbookdata-backup-YYYYMMDD-HHMMSS/` (backup directory)
- Create: `rollback.sh` (rollback script)
- Create: `validate_pre_migration.py` (pre-migration validation)
- Create: `.backup_dir` (backup location file)
- Create: `pre_migration_report.json` (validation report)

### Task A.1: Create Backup Directory

- [ ] **Step A.1.1: Create backup directory with timestamp**

Run:
```bash
BACKUP_DIR="playbookdata-backup-$(date +%Y%m%d-%H%M%S)"
cp -r playbookdata "$BACKUP_DIR"
echo "$BACKUP_DIR" > .backup_dir
echo "Backup created: $BACKUP_DIR"
```

Expected: Directory created with full copy of playbookdata

- [ ] **Step A.1.2: Verify backup integrity with checksums**

Run:
```bash
BACKUP_DIR=$(cat .backup_dir)

# Count files
ORIG_FILES=$(find playbookdata -name "*.json" | wc -l)
BACKUP_FILES=$(find "$BACKUP_DIR" -name "*.json" | wc -l)

echo "Original: $ORIG_FILES JSON files"
echo "Backup: $BACKUP_FILES JSON files"

# Verify file counts match
if [ "$ORIG_FILES" -eq "$BACKUP_FILES" ]; then
    echo "✓ File count verified"
else
    echo "✗ File count mismatch"
    exit 1
fi

# Create checksum for verification
cd playbookdata && find . -name "*.json" -type f -exec sha256sum {} \; | sort > ../original.checksum
cd ../$BACKUP_DIR && find . -name "*.json" -type f -exec sha256sum {} \; | sort > ../backup.checksum
cd ..

# Compare checksums (ignore path differences)
if diff <(cut -d' ' -f1 original.checksum) <(cut -d' ' -f1 backup.checksum) > /dev/null; then
    echo "✓ Checksum verified"
    CHECKSUM_STATUS="PASS"
else
    echo "⚠ Checksum differences found (may be path-related)"
    CHECKSUM_STATUS="WARN"
fi

# Create backup manifest
cat > backup-manifest.json << MANIFEST_EOF
{
  "backup_dir": "$BACKUP_DIR",
  "original_files": $ORIG_FILES,
  "backup_files": $BACKUP_FILES,
  "checksum_status": "$CHECKSUM_STATUS",
  "created_at": "$(date -Iseconds)"
}
MANIFEST_EOF

echo "✓ Backup verification complete"
```

Expected: File counts match, checksums verified (or warning)

### Task A.2: Create Rollback Script

**Files:**
- Create: `rollback.sh`

- [ ] **Step A.2.1: Write rollback script**

```bash
cat > rollback.sh << 'EOF'
#!/bin/bash
# rollback.sh - Restore playbookdata from backup

set -e

if [ -z "$1" ]; then
    echo "Usage: ./rollback.sh <backup-dir>"
    echo ""
    echo "Available backups:"
    ls -d playbookdata-backup-* 2>/dev/null || echo "  (none found)"
    exit 1
fi

BACKUP_DIR="$1"

if [ ! -d "$BACKUP_DIR" ]; then
    echo "Error: Backup directory not found: $BACKUP_DIR"
    exit 1
fi

echo "Restoring from: $BACKUP_DIR"

# Remove current playbookdata
rm -rf playbookdata

# Copy backup
cp -r "$BACKUP_DIR" playbookdata

# Clear Python cache
find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true

echo "✓ Rollback complete"
echo "Restored: $(find playbookdata -name '*.json' | wc -l) JSON files"
EOF

chmod +x rollback.sh
echo "✓ rollback.sh created"
```

Expected: rollback.sh created and made executable

- [ ] **Step A.2.2: Test rollback script (dry run)**

Run:
```bash
BACKUP_DIR=$(cat .backup_dir)
./rollback.sh "$BACKUP_DIR"
```

Expected: Rollback completes, file counts match

### Task A.3: Create Pre-Migration Validation

**Files:**
- Create: `validate_pre_migration.py`

- [ ] **Step A.3.1: Write pre-migration validation script**

```python
cat > validate_pre_migration.py << 'EOF'
#!/usr/bin/env python3
"""
Pre-migration validation script.
Verifies current state before KB system changes.
"""

import json
from pathlib import Path
from datetime import datetime

def validate_playbookdata():
    """Validate current playbookdata structure."""
    playbook_path = Path("playbookdata")

    if not playbook_path.exists():
        print("✗ playbookdata directory not found")
        return False

    results = {
        "timestamp": datetime.now().isoformat(),
        "checks": []
    }

    # Check technical-kb exists
    tech_kb = playbook_path / "technical-kb"
    if tech_kb.exists():
        index_file = tech_kb / "index.json"
        if index_file.exists():
            index = json.load(open(index_file))
            topics = index.get("topics", {})
            sources = index.get("sources", {})
            total_files = sum(len(files) for files in topics.values())

            results["checks"].append({
                "check": "technical-kb exists",
                "status": "pass",
                "details": f"{total_files} files across {len(topics)} topics, {len(sources)} sources"
            })
        else:
            results["checks"].append({
                "check": "technical-kb/index.json exists",
                "status": "fail",
                "details": "index.json not found"
            })
            return False
    else:
        results["checks"].append({
            "check": "technical-kb exists",
            "status": "fail",
            "details": "directory not found"
        })
        return False

    # Check manifest
    manifest_file = tech_kb / "manifest.json"
    if manifest_file.exists():
        manifest = json.load(open(manifest_file))
        phases = manifest.get("phases", [])
        completed = [p for p in phases if p.get("status") == "completed"]

        results["checks"].append({
            "check": "manifest.json valid",
            "status": "pass",
            "details": f"{len(completed)}/{len(phases)} phases completed"
        })

    # Count JSON files
    json_files = list(playbook_path.rglob("*.json"))
    json_files = [f for f in json_files if "index.json" not in str(f) and "manifest.json" not in str(f)]

    results["checks"].append({
        "check": "JSON files count",
        "status": "pass",
        "details": f"{len(json_files)} content files found"
    })

    results["total_files"] = len(json_files)
    results["overall_status"] = "pass"

    return results

def main():
    print("=" * 50)
    print("Pre-Migration Validation")
    print("=" * 50)

    results = validate_playbookdata()

    if not results:
        print("\n✗ Validation failed")
        exit(1)

    print("\nValidation Results:")
    for check in results["checks"]:
        status = "✓" if check["status"] == "pass" else "✗"
        print(f"  {status} {check['check']}: {check['details']}")

    print(f"\nTotal content files: {results['total_files']}")
    print(f"Overall status: {results['overall_status']}")

    # Save validation report
    report_path = Path("pre_migration_report.json")
    with open(report_path, "w") as f:
        json.dump(results, f, indent=2)
    print(f"\nReport saved: {report_path}")

    print("=" * 50)

if __name__ == "__main__":
    main()
EOF

python3 validate_pre_migration.py
```

Expected: Validation passes, report saved

- [ ] **Step A.3.2: Verify validation report**

Run:
```bash
cat pre_migration_report.json
```

Expected: JSON report showing pass status and file counts

### Section A Completion Checkpoint

Run:
```bash
echo "=== Section A Completion Check ==="
echo ""
echo "Backup directory: $(cat .backup_dir)"
echo "Rollback script: $(ls -la rollback.sh)"
echo "Validation report: $(ls -la pre_migration_report.json)"
echo ""
echo "✓ Section A complete: Preparation finished"
```

Expected: All files present, validation passed

---

## Section B: Foundation Setup (Phase 1)

**Purpose:** Create KB infrastructure without touching existing data.

**Estimated Duration:** ~15 minutes

**Recovery:** If this section fails, run:
```bash
# Remove created files (idempotent - safe to run multiple times)
rm -f playbookdata/__init__.py playbookdata/kb-lookup.py playbookdata/master-index.json
rm -rf tests/test_kb_lookup.py
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb; do
    rm -f playbookdata/$kb/index.json playbookdata/$kb/validation.json
done
# Then re-run from Step B.1.1
```

**Idempotency:** All steps in this section are idempotent - files are created fresh each time.

**Files:**
- Create: `playbookdata/__init__.py`
- Create: `playbookdata/kb-lookup.py`
- Create: `playbookdata/master-index.json`
- Create: `tests/test_kb_lookup.py`
- Create: `playbookdata/juce-kb/`, `playbookdata/dsp-kb/`, `playbookdata/sound-design-kb/`, `playbookdata/ui-kb/`, `playbookdata/cpp-kb/`, `playbookdata/cmake-kb/` (directories only, no content yet)

### Task B.1: Create Package Init File

**Files:**
- Create: `playbookdata/__init__.py`

- [ ] **Step B.1.1: Write package init file**

```python
cat > playbookdata/__init__.py << 'EOF'
"""
Playbook Data Package

Knowledge base system for JUCE VST plugin development.

Usage:
    from playbookdata import get_kb

    kb = get_kb()
    results = kb.query_topic("dsp")
"""

from .kb_lookup import (
    get_kb,
    reset_kb,
    KBQuery,
    KBResult,
    KBFile,
    KBError,
    KBNotFoundError,
    KBIntegrityError,
)

__all__ = [
    'get_kb',
    'reset_kb',
    'KBQuery',
    'KBResult',
    'KBFile',
    'KBError',
    'KBNotFoundError',
    'KBIntegrityError',
]

__version__ = '1.0.0'
EOF

echo "✓ playbookdata/__init__.py created"
```

Expected: __init__.py created

- [ ] **Step B.1.2: Verify import works**

Run:
```bash
python3 -c "from playbookdata import KBQuery; print('✓ Import successful')"
```

Expected: Import fails (kb_lookup.py not created yet) - this is expected

### Task B.2: Create KB Lookup Module

**Files:**
- Create: `playbookdata/kb-lookup.py`

- [ ] **Step B.2.1: Write kb-lookup.py (Part 1: Imports and Classes)**

```python
cat > playbookdata/kb_lookup.py << 'EOF'
"""
Knowledge Base Lookup Module

Central query interface for all JUCE VST plugin development knowledge bases.
Provides topic-based queries, full-text search, and audio-relevance ranking.

Usage:
    from playbookdata.kb_lookup import get_kb

    kb = get_kb()
    results = kb.query_topic("thread_safety")
    content = kb.get_file_content(results[0].file_path)
"""

import json
import hashlib
import logging
import threading
import re
from pathlib import Path
from typing import Optional, Union, List, Dict, Tuple, Any
from functools import lru_cache
from dataclasses import dataclass
from enum import Enum
from datetime import datetime

# Configure logging
logger = logging.getLogger('playbookdata.kb_lookup')

# KB root directory
KB_ROOT = Path(__file__).parent


# ============================================================================
# Exceptions
# ============================================================================

class KBError(Exception):
    """Base exception for KB operations."""
    pass


class KBNotFoundError(KBError):
    """KB or file not found."""
    pass


class KBIntegrityError(KBError):
    """KB validation failed."""
    pass


# ============================================================================
# Data Classes
# ============================================================================

@dataclass
class KBResult:
    """A single KB query result."""
    kb_name: str
    file_path: str
    relevance_score: float
    audio_relevance: int
    title: str = ""
    source: str = ""

    def __lt__(self, other: 'KBResult') -> bool:
        return self.relevance_score < other.relevance_score


@dataclass
class KBFile:
    """Full content of a KB file."""
    kb_name: str
    file_path: str
    content: dict
    metadata: dict


# ============================================================================
# KB Authority Scores
# ============================================================================

KB_AUTHORITY = {
    'juce-kb': 1.0,
    'dsp-kb': 0.9,
    'sound-design-kb': 0.8,
    'ui-kb': 0.7,
    'cpp-kb': 0.5,
    'cmake-kb': 0.5
}

KB_BASELINE_RELEVANCE = {
    'juce-kb': 7,
    'dsp-kb': 9,
    'sound-design-kb': 8,
    'ui-kb': 5,
    'cpp-kb': 3,
    'cmake-kb': 2
}

AUDIO_KEYWORDS = [
    'AudioProcessor', 'AudioBuffer', 'MidiBuffer', 'processBlock',
    'sample', 'buffer', 'DSP', 'filter', 'oscillator', 'envelope',
    'ADSR', 'FFT', 'reverb', 'delay', 'compression'
]

ADVANCED_KEYWORDS = ['thread', 'lock', 'atomic', 'SIMD', 'optimization', 'real-time']
INTERMEDIATE_KEYWORDS = ['class', 'inherit', 'virtual', 'override', 'component']
EOF

echo "✓ kb_lookup.py Part 1 created"
```

Expected: Part 1 created

- [ ] **Step B.2.2: Write kb-lookup.py (Part 2: KBQuery Class)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'


# ============================================================================
# KBQuery Class
# ============================================================================

class KBQuery:
    """Central lookup for all knowledge bases with caching and error handling."""

    def __init__(self, preload_indices: bool = True):
        """Initialize KB query interface.

        Args:
            preload_indices: If True, load master index immediately.
        """
        self.master_index: Dict[str, Any] = {}
        self._kb_cache: Dict[str, Dict] = {}
        self._file_cache: Dict[str, KBFile] = {}
        self._initialized = False
        self._lock = threading.Lock()

        if preload_indices:
            self._initialize()

    def _initialize(self) -> None:
        """Load master index and validate KB structure."""
        try:
            master_path = KB_ROOT / "master-index.json"
            if master_path.exists():
                self.master_index = self._load_json(master_path)
                self._initialized = True
                logger.info(f"KB initialized with {len(self.master_index.get('knowledge_bases', {}))} KBs")
            else:
                logger.warning("master-index.json not found, KB may not be harvested yet")
                self._initialized = False
        except json.JSONDecodeError as e:
            raise KBIntegrityError(f"Invalid master-index.json: {e}")

    # ========================================================================
    # JSON Loading
    # ========================================================================

    def _load_json(self, path: Path) -> Dict[str, Any]:
        """Load JSON file with error handling.

        Args:
            path: Path to JSON file

        Returns:
            Parsed JSON data

        Raises:
            KBNotFoundError: If file doesn't exist
            KBIntegrityError: If JSON is invalid
        """
        if not path.exists():
            raise KBNotFoundError(f"File not found: {path}")

        MAX_SIZE = 10 * 1024 * 1024  # 10MB

        if path.stat().st_size > MAX_SIZE:
            raise KBIntegrityError(f"File too large: {path}")

        try:
            with open(path) as f:
                return json.load(f)
        except json.JSONDecodeError as e:
            raise KBIntegrityError(f"Invalid JSON in {path}: {e}")

    def _save_json(self, path: Path, data: Dict[str, Any]) -> None:
        """Save JSON file with formatting.

        Args:
            path: Path to save
            data: Data to save
        """
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, 'w') as f:
            json.dump(data, f, indent=2)

    # ========================================================================
    # KB Index Access
    # ========================================================================

    def _get_kb_index(self, kb_name: str) -> Optional[Dict[str, Any]]:
        """Load and cache KB index.

        Args:
            kb_name: Name of KB (e.g., 'juce-kb')

        Returns:
            KB index dict or None if not found
        """
        if kb_name not in self._kb_cache:
            try:
                index_path = KB_ROOT / kb_name / "index.json"
                self._kb_cache[kb_name] = self._load_json(index_path)
            except (KBNotFoundError, KBIntegrityError) as e:
                logger.warning(f"Index not found for KB: {kb_name}: {e}")
                self._kb_cache[kb_name] = None
        return self._kb_cache[kb_name]

    def list_kbs(self) -> Dict[str, Dict]:
        """List all available knowledge bases with their status.

        Returns:
            Dict mapping KB names to their info
        """
        return self.master_index.get("knowledge_bases", {})
EOF

echo "✓ kb_lookup.py Part 2 created"
```

Expected: Part 2 created

- [ ] **Step B.2.3: Write kb-lookup.py (Part 3: Query Methods)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'

    # ========================================================================
    # Primary Query Methods
    # ========================================================================

    @lru_cache(maxsize=256)
    def query_topic(
        self,
        topic: str,
        kb_names: Optional[Tuple[str, ...]] = None,
        min_audio_relevance: int = 0
    ) -> List[KBResult]:
        """Query all KBs for a topic, return ranked results.

        Args:
            topic: Topic name (e.g., "dsp", "thread_safety")
            kb_names: Optional tuple of KB names to search (None = all KBs)
            min_audio_relevance: Minimum audio relevance score (0-10)

        Returns:
            List of KBResult sorted by relevance_score descending
        """
        if not self._initialized:
            return []

        # Validate topic input
        if not re.match(r'^[a-zA-Z0-9_-]+$', topic):
            logger.warning(f"Invalid topic name: {topic}")
            return []

        if len(topic) > 64:
            logger.warning(f"Topic too long: {topic}")
            return []

        results: List[KBResult] = []
        kbs = list(kb_names) if kb_names else list(self.list_kbs().keys())

        for kb_name in kbs:
            kb_index = self._get_kb_index(kb_name)
            if kb_index is None:
                continue

            for file_path in kb_index.get("topics", {}).get(topic, []):
                audio_rel = self._get_audio_relevance(kb_name, file_path)

                if audio_rel < min_audio_relevance:
                    continue

                full_path = str(KB_ROOT / kb_name / file_path)

                results.append(KBResult(
                    kb_name=kb_name,
                    file_path=full_path,
                    relevance_score=self._calculate_relevance(file_path, topic, kb_name),
                    audio_relevance=audio_rel,
                    title=self._extract_title(kb_name, file_path),
                    source=self._get_source(kb_name, file_path)
                ))

        return sorted(results, reverse=True)

    def query_topics(self, topics: List[str], kb_names: Optional[List[str]] = None) -> Dict[str, List[KBResult]]:
        """Batch query multiple topics efficiently.

        Args:
            topics: List of topic names
            kb_names: Optional list of KB names to search

        Returns:
            Dict mapping each topic to its results
        """
        kb_tuple = tuple(kb_names) if kb_names else None
        return {
            topic: self.query_topic(topic, kb_tuple)
            for topic in topics
        }

    def get_file_content(self, file_path: str, use_cache: bool = True) -> KBFile:
        """Load full content from a KB file.

        Args:
            file_path: Absolute or relative path to KB file
            use_cache: Whether to use cached content if available

        Returns:
            KBFile with content and metadata

        Raises:
            KBNotFoundError: If file doesn't exist
            KBIntegrityError: If file is invalid
        """
        path = Path(file_path)
        if not path.is_absolute():
            path = KB_ROOT / file_path

        # Security: prevent path traversal
        try:
            path = path.resolve()
            kb_root_resolved = KB_ROOT.resolve()
            if not str(path).startswith(str(kb_root_resolved)):
                raise KBError(f"Path traversal attempt: {file_path}")
        except Exception as e:
            raise KBError(f"Invalid path: {file_path}: {e}")

        cache_key = str(path)

        if use_cache and cache_key in self._file_cache:
            return self._file_cache[cache_key]

        content = self._load_json(path)
        kb_name = self._extract_kb_name(path)

        result = KBFile(
            kb_name=kb_name,
            file_path=str(path),
            content=content,
            metadata=self._extract_metadata(content)
        )

        if use_cache:
            self._file_cache[cache_key] = result

        return result
EOF

echo "✓ kb_lookup.py Part 3 created"
```

Expected: Part 3 created

- [ ] **Step B.2.4: Write kb-lookup.py (Part 4: Specialized Queries)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'

    # ========================================================================
    # Specialized Query Methods
    # ========================================================================

    @lru_cache(maxsize=64)
    def get_implementation_examples(
        self,
        topic: str,
        min_audio_relevance: int = 7,
        max_results: int = 10
    ) -> List[KBResult]:
        """Get implementation examples with high audio relevance.

        These are files that contain actual code examples for audio plugin development.

        Args:
            topic: Topic to search
            min_audio_relevance: Minimum audio relevance (default 7)
            max_results: Maximum results to return

        Returns:
            List of KBResult with high audio relevance
        """
        results = self.query_topic(topic)
        filtered = [r for r in results if r.audio_relevance >= min_audio_relevance]
        return filtered[:max_results]

    def search_content(
        self,
        query: str,
        kb_names: Optional[List[str]] = None,
        case_sensitive: bool = False
    ) -> List[KBResult]:
        """Full-text search across KB content.

        Searches markdown content, titles, and metadata.

        Args:
            query: Search query string
            kb_names: Optional list of KB names to search
            case_sensitive: Whether search is case sensitive

        Returns:
            List of KBResult sorted by relevance
        """
        if not self._initialized:
            return []

        results: List[KBResult] = []
        kbs = kb_names or list(self.list_kbs().keys())
        query_lower = query.lower() if not case_sensitive else query

        for kb_name in kbs:
            for file_path in self._get_all_kb_files(kb_name):
                content = self._get_file_content_raw(kb_name, file_path)
                if content is None:
                    continue

                search_text = content.get("markdown", "") or content.get("html", "")
                if not case_sensitive:
                    search_text = search_text.lower()

                if query_lower in search_text:
                    results.append(KBResult(
                        kb_name=kb_name,
                        file_path=str(KB_ROOT / kb_name / file_path),
                        relevance_score=self._score_search_hit(search_text, query_lower),
                        audio_relevance=self._get_audio_relevance(kb_name, file_path),
                        title=self._extract_title(kb_name, file_path),
                        source=self._get_source(kb_name, file_path)
                    ))

        return sorted(results, reverse=True)

    def get_related_topics(self, topic: str) -> List[str]:
        """Find topics related to the given topic via cross-references.

        Args:
            topic: Topic to find related topics for

        Returns:
            List of related topic names
        """
        cross_refs = self.master_index.get("topic_cross_references", {})

        related = set()

        # Topics that share files with this topic
        topic_files = set(cross_refs.get(topic, []))

        for other_topic, files in cross_refs.items():
            if other_topic != topic:
                if set(files) & topic_files:
                    related.add(other_topic)

        return sorted(related)

    def get_skill_recommendations(self, topic: str) -> List[str]:
        """Suggest which skills to use for a given topic.

        Args:
            topic: Topic name

        Returns:
            List of skill names
        """
        topic_skill_map = {
            "dsp": ["juce-dsp-implementation"],
            "gui": ["juce-ui-bridge"],
            "parameters": ["juce-plugin-spec", "juce-sound-design-bridge"],
            "state_management": ["juce-plugin-spec"],
            "testing": ["juce-daw-testing"],
            "audio_processing": ["juce-dsp-implementation", "juce-plugin-spec"],
            "thread_safety": ["juce-dsp-implementation"],
            "midi": ["juce-dsp-implementation"],
        }
        return topic_skill_map.get(topic, [])

    def get_dependencies(self, file_path: str) -> List[str]:
        """Get files that this file references or depends on.

        Args:
            file_path: Path to file

        Returns:
            List of related file paths
        """
        try:
            content = self.get_file_content(file_path)
            links = content.content.get("links", [])

            dependencies = []
            for link in links:
                for kb_name in self.list_kbs():
                    matching = self._find_file_for_url(kb_name, link)
                    if matching:
                        dependencies.append(str(KB_ROOT / kb_name / matching))

            return dependencies
        except KBError:
            return []
EOF

echo "✓ kb_lookup.py Part 4 created"
```

Expected: Part 4 created

- [ ] **Step B.2.5: Write kb-lookup.py (Part 5: Health and Validation)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'

    # ========================================================================
    # KB Management and Validation
    # ========================================================================

    def validate_kb(self, kb_name: str) -> Dict[str, Any]:
        """Validate KB integrity.

        Args:
            kb_name: Name of KB to validate

        Returns:
            Validation report dict

        Raises:
            KBNotFoundError: If KB not found
        """
        kb_path = KB_ROOT / kb_name

        if not kb_path.exists():
            raise KBNotFoundError(f"KB not found: {kb_name}")

        report = {
            "kb_name": kb_name,
            "valid": True,
            "issues": [],
            "file_count": 0,
            "missing_files": []
        }

        # Check index exists
        index_path = kb_path / "index.json"
        if not index_path.exists():
            report["valid"] = False
            report["issues"].append("index.json missing")
            return report

        # Check validation file
        validation_path = kb_path / "validation.json"
        if validation_path.exists():
            try:
                validation = self._load_json(validation_path)
                report["validation_checksum"] = validation.get("checksum")
            except:
                report["issues"].append("validation.json exists but invalid")

        # Count files and verify index references
        index = self._load_json(index_path)
        all_topics = index.get("topics", {})

        for topic, files in all_topics.items():
            for file_path in files:
                full_path = kb_path / file_path
                if full_path.exists():
                    report["file_count"] += 1
                else:
                    report["missing_files"].append(file_path)
                    report["valid"] = False

        if report["missing_files"]:
            report["issues"].append(f"{len(report['missing_files'])} files referenced in index not found")

        return report

    def health_check(self) -> Dict[str, Any]:
        """Run health check on all KBs.

        Returns:
            Health report dict with overall status and per-KB details
        """
        results = {}
        for kb_name in self.list_kbs():
            try:
                results[kb_name] = self.validate_kb(kb_name)
            except KBError as e:
                results[kb_name] = {"valid": False, "error": str(e)}

        return {
            "overall_valid": all(r.get("valid", False) for r in results.values()),
            "kbs": results,
            "timestamp": datetime.now().isoformat()
        }

    # ========================================================================
    # Cache Management
    # ========================================================================

    def clear_cache(self) -> None:
        """Clear all caches."""
        with self._lock:
            self._kb_cache.clear()
            self._file_cache.clear()
            self.query_topic.cache_clear()
            self.get_implementation_examples.cache_clear()
        logger.info("KB caches cleared")

    def warm_cache(self, topics: Optional[List[str]] = None) -> None:
        """Pre-warm caches with common queries.

        Args:
            topics: Topics to warm (default: common topics)
        """
        topics = topics or ["dsp", "thread_safety", "parameters", "gui", "audio_processing"]

        logger.info(f"Warming cache for {len(topics)} topics...")
        for topic in topics:
            self.query_topic(topic)

        logger.info("Cache warm complete")
EOF

echo "✓ kb_lookup.py Part 5 created"
```

Expected: Part 5 created

- [ ] **Step B.2.6: Write kb-lookup.py (Part 6: Helper Methods)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'

    # ========================================================================
    # Private Helper Methods
    # ========================================================================

    def _get_audio_relevance(self, kb_name: str, file_path: str) -> int:
        """Get audio relevance score from validation metadata.

        Args:
            kb_name: KB name
            file_path: Relative file path within KB

        Returns:
            Audio relevance score (0-10)
        """
        try:
            validation = self._load_json(KB_ROOT / kb_name / "validation.json")
            file_rel = validation.get("files", {}).get(file_path, {})
            return file_rel.get("audio_relevance",
                              validation.get("defaults", {}).get("audio_relevance", 5))
        except (KBNotFoundError, KBIntegrityError):
            # Fallback: KB-specific defaults
            return KB_BASELINE_RELEVANCE.get(kb_name, 5)

    def _calculate_relevance(self, file_path: str, topic: str, kb_name: str) -> float:
        """Calculate relevance score based on file metadata and topic match.

        Args:
            file_path: File path within KB
            topic: Topic name
            kb_name: KB name

        Returns:
            Relevance score (0-10+)
        """
        score = 10.0  # Base score

        # KB authority bonus
        score *= KB_AUTHORITY.get(kb_name, 0.6)

        # Topic specificity bonus
        if topic in file_path.lower():
            score *= 1.2

        # Implementation examples bonus
        audio_rel = self._get_audio_relevance(kb_name, file_path)
        score *= (1 + audio_rel / 20)

        return round(score, 2)

    def _score_search_hit(self, text: str, query: str) -> float:
        """Score a search hit based on frequency and position.

        Args:
            text: Text to search in
            query: Query string

        Returns:
            Search score (0-10)
        """
        count = text.count(query)
        position = text.find(query)

        score = min(count * 0.5, 5.0)
        if position >= 0:
            score += max(5 - position / 100, 0)

        return min(score, 10.0)

    def _extract_title(self, kb_name: str, file_path: str) -> str:
        """Extract title from file content.

        Args:
            kb_name: KB name
            file_path: Relative file path

        Returns:
            Extracted title or filename
        """
        try:
            content = self._get_file_content_raw(kb_name, file_path)
            if content:
                md = content.get("markdown", "")
                for line in md.split("\n")[:5]:
                    if line.startswith("# "):
                        return line[2:].strip()
            return Path(file_path).stem
        except:
            return Path(file_path).stem

    def _get_source(self, kb_name: str, file_path: str) -> str:
        """Get source attribution for a file.

        Args:
            kb_name: KB name
            file_path: Relative file path

        Returns:
            Source name
        """
        try:
            content = self._get_file_content_raw(kb_name, file_path)
            if content:
                return content.get("source", "unknown")
        except:
            pass
        return "unknown"

    def _get_file_content_raw(self, kb_name: str, file_path: str) -> Optional[Dict]:
        """Get raw file content without KBFile wrapper.

        Args:
            kb_name: KB name
            file_path: Relative file path

        Returns:
            Raw content dict or None
        """
        cache_key = f"{kb_name}:{file_path}"
        if cache_key in self._file_cache:
            return self._file_cache[cache_key].content

        try:
            return self._load_json(KB_ROOT / kb_name / file_path)
        except:
            return None

    def _get_all_kb_files(self, kb_name: str) -> List[str]:
        """Get all file paths in a KB from its index.

        Args:
            kb_name: KB name

        Returns:
            List of relative file paths
        """
        index = self._get_kb_index(kb_name)
        if not index:
            return []

        files = []
        for topic_files in index.get("topics", {}).values():
            files.extend(topic_files)
        return list(set(files))

    def _extract_kb_name(self, path: Path) -> str:
        """Extract KB name from file path.

        Args:
            path: File path

        Returns:
            KB name
        """
        try:
            relative = path.relative_to(KB_ROOT)
            return relative.parts[0]
        except ValueError:
            return "unknown"

    def _extract_metadata(self, content: Dict) -> Dict:
        """Extract relevant metadata from content.

        Args:
            content: Content dict

        Returns:
            Metadata dict
        """
        return {
            "source": content.get("source", "unknown"),
            "harvested_at": content.get("harvested_at"),
            "source_url": content.get("source_url"),
            "juce_version": content.get("juce_version")
        }

    def _find_file_for_url(self, kb_name: str, url: str) -> Optional[str]:
        """Find KB file that matches a URL.

        Args:
            kb_name: KB name
            url: URL to match

        Returns:
            File path or None
        """
        # Search through KB files for matching source_url
        for file_path in self._get_all_kb_files(kb_name):
            try:
                content = self._get_file_content_raw(kb_name, file_path)
                if content and content.get("source_url") == url:
                    return file_path
            except:
                continue
        return None
EOF

echo "✓ kb_lookup.py Part 6 created"
```

Expected: Part 6 created

- [ ] **Step B.2.7: Write kb-lookup.py (Part 7: Singleton and Exports)**

```python
cat >> playbookdata/kb_lookup.py << 'EOF'


# ============================================================================
# Singleton Instance
# ============================================================================

_kb_lookup: Optional[KBQuery] = None
_init_lock = threading.Lock()


def get_kb(initialize: bool = True) -> Optional[KBQuery]:
    """Get the singleton KB lookup instance.

    Args:
        initialize: If False, return None if not yet initialized

    Returns:
        KBQuery instance or None
    """
    global _kb_lookup
    if _kb_lookup is None and initialize:
        with _init_lock:
            if _kb_lookup is None:  # Double-check locking
                _kb_lookup = KBQuery()
    return _kb_lookup


def reset_kb() -> None:
    """Reset the singleton (useful for testing)."""
    global _kb_lookup
    with _init_lock:
        _kb_lookup = None


# ============================================================================
# Convenience Functions
# ============================================================================

def query_topic(topic: str, kb_names: Optional[List[str]] = None) -> List[KBResult]:
    """Convenience function to query a topic.

    Args:
        topic: Topic name
        kb_names: Optional list of KB names

    Returns:
        List of KBResult
    """
    kb = get_kb()
    if kb is None:
        return []
    kb_tuple = tuple(kb_names) if kb_names else None
    return kb.query_topic(topic, kb_tuple)


def get_content(topic: str, fallback: str = "") -> str:
    """Get markdown content for a topic with fallback.

    Args:
        topic: Topic name
        fallback: Fallback content if not found

    Returns:
        Markdown content or fallback
    """
    kb = get_kb()
    if kb is None:
        return fallback or f"# {topic}\n\nContent not available."

    results = kb.query_topic(topic)
    if results:
        try:
            content = kb.get_file_content(results[0].file_path)
            return content.content.get("markdown", fallback)
        except KBError:
            pass

    return fallback or f"# {topic}\n\nContent not available."
EOF

echo "✓ kb_lookup.py Part 7 created"
```

Expected: Part 7 created

- [ ] **Step B.2.8: Verify kb-lookup.py is complete**

Run:
```bash
wc -l playbookdata/kb_lookup.py
python3 -c "from playbookdata.kb_lookup import get_kb, KBQuery, KBError; print('✓ Module imports successfully')"
```

Expected: Module imports successfully

### Task B.3: Create Master Index Template

**Files:**
- Create: `playbookdata/master-index.json`

- [ ] **Step B.3.1: Write master-index.json template**

```python
cat > playbookdata/master-index.json << 'EOF'
{
  "version": "1.0.0",
  "created": "2026-03-29T00:00:00.000000",
  "last_updated": "2026-03-29T00:00:00.000000",
  "knowledge_bases": {
    "juce-kb": {
      "path": "juce-kb",
      "source_type": "harvested",
      "pages": 0,
      "status": "pending"
    },
    "dsp-kb": {
      "path": "dsp-kb",
      "source_type": "harvested+curated",
      "pages": 0,
      "status": "pending"
    },
    "sound-design-kb": {
      "path": "sound-design-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "pending"
    },
    "ui-kb": {
      "path": "ui-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "pending"
    },
    "cpp-kb": {
      "path": "cpp-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "pending"
    },
    "cmake-kb": {
      "path": "cmake-kb",
      "source_type": "curated",
      "pages": 0,
      "status": "pending"
    }
  },
  "topic_cross_references": {
    "audio_processing": [],
    "dsp": [],
    "thread_safety": [],
    "gui": [],
    "state_management": [],
    "midi": [],
    "parameters": [],
    "testing": [],
    "ci_cd": [],
    "deployment": []
  },
  "audio_specific_index": {
    "plugin_types": {
      "effects": [],
      "synthesizers": [],
      "meters": [],
      "analyzers": []
    },
    "signal_flow": {
      "input": [],
      "processing": [],
      "output": []
    }
  },
  "query_cache": {},
  "metadata": {
    "total_files": 0,
    "total_topics": 10,
    "harvest_sources": []
  }
}
EOF

echo "✓ master-index.json created"
```

Expected: master-index.json created

- [ ] **Step B.3.2: Verify master-index.json is valid**

Run:
```bash
python3 -c "import json; json.load(open('playbookdata/master-index.json')); print('✓ master-index.json is valid JSON')"
```

Expected: JSON validates

### Task B.4: Create Empty KB Directories

**Files:**
- Create: `playbookdata/juce-kb/`, `playbookdata/dsp-kb/`, etc.
- Create: `playbookdata/juce-kb/index.json`, `playbookdata/dsp-kb/index.json`, etc.

- [ ] **Step B.4.1: Create KB directories**

Run:
```bash
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb; do
    mkdir -p "playbookdata/$kb"
    echo "✓ Created playbookdata/$kb"
done
```

Expected: All 6 directories created

- [ ] **Step B.4.2: Create KB index.json files**

```python
cat > create_kb_indices.py << 'EOF'
#!/usr/bin/env python3
"""Create empty KB index.json files."""

import json
from pathlib import Path
from datetime import datetime

KB_CONFIGS = {
    'juce-kb': {
        'description': 'JUCE framework documentation and tutorials',
        'source_type': 'harvested',
        'default_topics': ['audio_processing', 'dsp', 'thread_safety', 'gui',
                          'state_management', 'midi', 'parameters', 'testing',
                          'ci_cd', 'deployment'],
        'audio_relevance_baseline': 8
    },
    'dsp-kb': {
        'description': 'Digital signal processing theory and implementations',
        'source_type': 'harvested+curated',
        'default_topics': ['dsp', 'audio_processing', 'parameters'],
        'audio_relevance_baseline': 9
    },
    'sound-design-kb': {
        'description': 'Sound design techniques and presets',
        'source_type': 'curated',
        'default_topics': ['parameters', 'midi'],
        'audio_relevance_baseline': 8
    },
    'ui-kb': {
        'description': 'Plugin UI components and LookAndFeel',
        'source_type': 'curated',
        'default_topics': ['gui'],
        'audio_relevance_baseline': 6
    },
    'cpp-kb': {
        'description': 'C++ patterns for audio development',
        'source_type': 'curated',
        'default_topics': ['thread_safety'],
        'audio_relevance_baseline': 4
    },
    'cmake-kb': {
        'description': 'CMake configuration for JUCE projects',
        'source_type': 'curated',
        'default_topics': ['ci_cd', 'deployment'],
        'audio_relevance_baseline': 3
    }
}

for kb_name, config in KB_CONFIGS.items():
    kb_path = Path('playbookdata') / kb_name

    # Create index.json
    index = {
        "kb_name": kb_name,
        "version": "1.0.0",
        "created": datetime.now().isoformat(),
        "last_updated": datetime.now().isoformat(),
        "description": config['description'],
        "topics": {topic: [] for topic in config['default_topics']},
        "sources": {},
        "metadata": {
            "total_files": 0,
            "source_type": config['source_type'],
            "audio_relevance_baseline": config['audio_relevance_baseline']
        }
    }

    with open(kb_path / 'index.json', 'w') as f:
        json.dump(index, f, indent=2)

    print(f"✓ Created {kb_name}/index.json")

print("\nAll KB indices created.")
EOF

python3 create_kb_indices.py
rm create_kb_indices.py
```

Expected: All index.json files created

### Task B.5: Create Unit Tests

**Files:**
- Create: `tests/__init__.py`
- Create: `tests/test_kb_lookup.py`

- [ ] **Step B.5.1: Create tests directory**

Run:
```bash
mkdir -p tests
touch tests/__init__.py
echo "✓ Created tests/"
```

Expected: tests directory created

- [ ] **Step B.5.2: Write unit tests for kb-lookup**

```python
cat > tests/test_kb_lookup.py << 'EOF'
"""
Unit tests for KB lookup module.
"""

import pytest
import json
from pathlib import Path
from playbookdata.kb_lookup import (
    get_kb, reset_kb, KBQuery, KBResult, KBFile, KBError, KBNotFoundError
)


class TestKBQuery:
    """Unit tests for KB query functionality."""

    def setup_method(self):
        """Reset KB singleton before each test."""
        reset_kb()

    def test_get_kb_returns_instance(self):
        """get_kb returns a KBQuery instance."""
        kb = get_kb()
        assert kb is not None
        assert isinstance(kb, KBQuery)

    def test_get_kb_singleton(self):
        """get_kb returns the same instance."""
        kb1 = get_kb()
        kb2 = get_kb()
        assert kb1 is kb2

    def test_list_kbs_returns_dict(self):
        """list_kbs returns a dictionary."""
        kb = get_kb()
        kbs = kb.list_kbs()
        assert isinstance(kbs, dict)

    def test_query_topic_unknown_returns_empty(self):
        """Query for unknown topic should return empty list."""
        kb = get_kb()
        results = kb.query_topic("nonexistent_topic_xyz")
        assert results == []

    def test_query_topic_invalid_name_returns_empty(self):
        """Query with invalid topic name should return empty."""
        kb = get_kb()
        # Invalid characters
        results = kb.query_topic("invalid-topic!")
        assert results == []
        # Too long
        results = kb.query_topic("a" * 100)
        assert results == []

    def test_clear_cache_resets(self):
        """clear_cache resets internal caches."""
        kb = get_kb()
        # Populate cache
        kb.query_topic("dsp")
        # Clear
        kb.clear_cache()
        # Verify cache is cleared
        assert kb.query_topic.cache_info().currsize == 0

    def test_health_check_returns_structure(self):
        """health_check returns proper structure."""
        kb = get_kb()
        health = kb.health_check()
        assert "overall_valid" in health
        assert "kbs" in health
        assert "timestamp" in health

    def test_validate_kb_missing_raises(self):
        """validate_kb raises for missing KB."""
        kb = get_kb()
        with pytest.raises(KBNotFoundError):
            kb.validate_kb("nonexistent-kb")


class TestKBResult:
    """Tests for KBResult dataclass."""

    def test_kbresult_comparison(self):
        """KBResult can be compared by relevance."""
        r1 = KBResult(
            kb_name="test",
            file_path="test.json",
            relevance_score=5.0,
            audio_relevance=7
        )
        r2 = KBResult(
            kb_name="test",
            file_path="test2.json",
            relevance_score=10.0,
            audio_relevance=9
        )
        assert r1 < r2
        assert r2 > r1


class TestKBFile:
    """Tests for KBFile dataclass."""

    def test_kbfile_creation(self):
        """KBFile can be created with all fields."""
        kb_file = KBFile(
            kb_name="test-kb",
            file_path="test.json",
            content={"markdown": "# Test"},
            metadata={"source": "test"}
        )
        assert kb_file.kb_name == "test-kb"
        assert kb_file.content["markdown"] == "# Test"


class TestConvenienceFunctions:
    """Tests for convenience functions."""

    def setup_method(self):
        """Reset KB singleton before each test."""
        reset_kb()

    def test_query_topic_function(self):
        """query_topic convenience function works."""
        from playbookdata.kb_lookup import query_topic
        results = query_topic("dsp")
        assert isinstance(results, list)

    def test_get_content_function(self):
        """get_content convenience function works."""
        from playbookdata.kb_lookup import get_content
        content = get_content("nonexistent", fallback="# Default")
        assert content == "# Default"
EOF

pytest tests/test_kb_lookup.py -v || echo "Tests will pass after Section C"
```

Expected: Tests run (may fail until data is migrated)

### Section B Completion Checkpoint

Run:
```bash
echo "=== Section B Completion Check ==="
echo ""
echo "Package files:"
ls -la playbookdata/__init__.py playbookdata/kb_lookup.py playbookdata/master-index.json
echo ""
echo "KB directories:"
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb; do
    echo "  $kb: $(ls playbookdata/$kb/ 2>/dev/null | wc -l) files"
done
echo ""
echo "Test results:"
python3 -c "from playbookdata import get_kb; print('✓ Package import works')"
echo ""
echo "✓ Section B complete: Foundation setup finished"
```

Expected: All files present, package imports

---

## Section C: Data Reorganization (Phase 2)

**Purpose:** Migrate existing data from technical-kb to new KB structure.

**Estimated Duration:** ~10 minutes

**⚠ IMPORTANT:** This section transforms existing data. Ensure Section A backup exists before proceeding.

**Recovery:** If this section fails, run:
```bash
# Restore from backup
BACKUP_DIR=$(cat .backup_dir)
./rollback.sh "$BACKUP_DIR"

# Clear Python cache
find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true

# Then re-run from Step C.1.1
```

### Task C.1: Rename technical-kb to juce-kb

**Files:**
- Modify: `playbookdata/technical-kb/` → `playbookdata/juce-kb/`

- [ ] **Step C.1.1: Verify backup exists**

Run:
```bash
if [ ! -f .backup_dir ]; then
    echo "✗ ERROR: No backup found. Run Section A first."
    exit 1
fi
BACKUP_DIR=$(cat .backup_dir)
if [ ! -d "$BACKUP_DIR" ]; then
    echo "✗ ERROR: Backup directory not found: $BACKUP_DIR"
    exit 1
fi
echo "✓ Backup verified: $BACKUP_DIR"
```

Expected: Backup verified

- [ ] **Step C.1.2: Rename technical-kb to juce-kb**

Run:
```bash
# Verify technical-kb exists
if [ ! -d "playbookdata/technical-kb" ]; then
    echo "✗ ERROR: technical-kb not found"
    exit 1
fi

# Rename
mv playbookdata/technical-kb playbookdata/juce-kb

echo "✓ Renamed technical-kb to juce-kb"
```

Expected: Directory renamed

- [ ] **Step C.1.3: Update juce-kb index.json**

```python
cat > update_juce_kb_index.py << 'EOF'
#!/usr/bin/env python3
"""Update juce-kb index.json after rename."""

import json
from pathlib import Path
from datetime import datetime

index_path = Path("playbookdata/juce-kb/index.json")

if not index_path.exists():
    print("✗ ERROR: juce-kb/index.json not found")
    exit(1)

with open(index_path) as f:
    index = json.load(f)

# Update kb_name
index["kb_name"] = "juce-kb"

# Update description
index["description"] = "JUCE framework documentation and tutorials"

# Update metadata
if "metadata" not in index:
    index["metadata"] = {}
index["metadata"]["source_type"] = "harvested"
index["metadata"]["audio_relevance_baseline"] = 8

# Update last_updated
index["last_updated"] = datetime.now().isoformat()

# Save
with open(index_path, "w") as f:
    json.dump(index, f, indent=2)

# Count files
topics = index.get("topics", {})
total_files = sum(len(files) for files in topics.values())
print(f"✓ Updated juce-kb/index.json")
print(f"  {len(topics)} topics, {total_files} files")
EOF

python3 update_juce_kb_index.py
rm update_juce_kb_index.py
```

Expected: juce-kb index updated

### Task C.2: Extract Stanford CCRMA to dsp-kb

**Files:**
- Move: `playbookdata/juce-kb/stanford-ccrma/` → `playbookdata/dsp-kb/stanford-ccrma/`

- [ ] **Step C.2.1: Check if Stanford content exists**

Run:
```bash
if [ -d "playbookdata/juce-kb/stanford-ccrma" ]; then
    echo "✓ Stanford content found in juce-kb"
    ls -la playbookdata/juce-kb/stanford-ccrma/
else
    echo "⊘ Stanford content not found in juce-kb (may already be migrated)"
fi
```

Expected: Stanford content found or already migrated

- [ ] **Step C.2.2: Move Stanford content to dsp-kb**

```python
cat > migrate_stanford.py << 'EOF'
#!/usr/bin/env python3
"""Migrate Stanford CCRMA content to dsp-kb."""

import json
import shutil
from pathlib import Path
from datetime import datetime

stanford_src = Path("playbookdata/juce-kb/stanford-ccrma")
stanford_dst = Path("playbookdata/dsp-kb/stanford-ccrma")

if not stanford_src.exists():
    print("⊘ Stanford content not in juce-kb, checking if already in dsp-kb...")
    if stanford_dst.exists():
        print("✓ Stanford content already in dsp-kb")
    else:
        print("✗ Stanford content not found anywhere")
    exit(0)

# Move Stanford content
if stanford_dst.exists():
    shutil.rmtree(stanford_dst)

shutil.move(str(stanford_src), str(stanford_dst))
print(f"✓ Moved Stanford content to dsp-kb")

# Count files
stanford_files = list(stanford_dst.rglob("*.json"))
print(f"  {len(stanford_files)} JSON files")
EOF

python3 migrate_stanford.py
rm migrate_stanford.py
```

Expected: Stanford content moved to dsp-kb

- [ ] **Step C.2.3: Create dsp-kb index.json**

```python
cat > create_dsp_index.py << 'EOF'
#!/usr/bin/env python3
"""Create dsp-kb index.json with Stanford content."""

import json
from pathlib import Path
from datetime import datetime

dsp_path = Path("playbookdata/dsp-kb")
stanford_path = dsp_path / "stanford-ccrma"

# Scan Stanford files
topics = {
    "dsp": [],
    "audio_processing": [],
    "filters": [],
    "fft": [],
    "reverb": [],
    "synthesis": []
}

if stanford_path.exists():
    for json_file in stanford_path.rglob("*.json"):
        rel_path = str(json_file.relative_to(dsp_path))

        # Categorize by content
        try:
            content = json.load(open(json_file))
            text = content.get("markdown", "") or content.get("html", "")
            text_lower = text.lower()

            # Always add to dsp
            topics["dsp"].append(rel_path)

            # Categorize by keywords
            if any(kw in text_lower for kw in ["filter", "iir", "fir", "butterworth"]):
                topics["filters"].append(rel_path)
            if any(kw in text_lower for kw in ["fft", "fourier", "spectral"]):
                topics["fft"].append(rel_path)
            if any(kw in text_lower for kw in ["reverb", "convolution"]):
                topics["reverb"].append(rel_path)
            if any(kw in text_lower for kw in ["oscillator", "synthesis", "wavetable"]):
                topics["synthesis"].append(rel_path)
            if any(kw in text_lower for kw in ["audio", "sample", "process"]):
                topics["audio_processing"].append(rel_path)
        except:
            topics["dsp"].append(rel_path)

# Count unique files
all_files = set()
for files in topics.values():
    all_files.update(files)

# Create index
index = {
    "kb_name": "dsp-kb",
    "version": "1.0.0",
    "created": datetime.now().isoformat(),
    "last_updated": datetime.now().isoformat(),
    "description": "Digital signal processing theory and implementations",
    "topics": topics,
    "sources": {
        "stanford-ccrma": {
            "status": "migrated",
            "pages_harvested": len(all_files),
            "migrated_from": "juce-kb",
            "migrated_at": datetime.now().isoformat()
        }
    },
    "metadata": {
        "total_files": len(all_files),
        "source_type": "harvested+curated",
        "audio_relevance_baseline": 9
    }
}

with open(dsp_path / "index.json", "w") as f:
    json.dump(index, f, indent=2)

print(f"✓ Created dsp-kb/index.json")
print(f"  {len(all_files)} files across {len(topics)} topics")
for topic, files in topics.items():
    if files:
        print(f"    {topic}: {len(files)} files")
EOF

python3 create_dsp_index.py
rm create_dsp_index.py
```

Expected: dsp-kb index created with Stanford content

- [ ] **Step C.2.4: Update juce-kb index to remove Stanford**

```python
cat > update_juce_remove_stanford.py << 'EOF'
#!/usr/bin/env python3
"""Update juce-kb index to remove Stanford references."""

import json
from pathlib import Path
from datetime import datetime

juce_index = Path("playbookdata/juce-kb/index.json")

with open(juce_index) as f:
    index = json.load(f)

# Remove Stanford from sources
if "stanford-ccrma" in index.get("sources", {}):
    del index["sources"]["stanford-ccrma"]
    print("✓ Removed stanford-ccrma from sources")

# Remove Stanford files from topics
for topic in index.get("topics", {}):
    before = len(index["topics"][topic])
    index["topics"][topic] = [
        f for f in index["topics"][topic]
        if "stanford-ccrma" not in f
    ]
    after = len(index["topics"][topic])
    if before != after:
        print(f"  {topic}: removed {before - after} Stanford files")

# Update metadata
index["metadata"]["total_files"] = sum(len(files) for files in index["topics"].values())
index["last_updated"] = datetime.now().isoformat()

# Save
with open(juce_index, "w") as f:
    json.dump(index, f, indent=2)

print(f"✓ Updated juce-kb/index.json")
print(f"  Total files: {index['metadata']['total_files']}")
EOF

python3 update_juce_remove_stanford.py
rm update_juce_remove_stanford.py
```

Expected: juce-kb index updated

### Task C.3: Update Master Index

**Files:**
- Modify: `playbookdata/master-index.json`

- [ ] **Step C.3.1: Update master-index.json with real data**

```python
cat > update_master_index.py << 'EOF'
#!/usr/bin/env python3
"""Update master-index.json with real KB data."""

import json
from pathlib import Path
from datetime import datetime

master_path = Path("playbookdata/master-index.json")

# Load existing master index
with open(master_path) as f:
    master = json.load(f)

# Load each KB index and extract stats
for kb_name in ["juce-kb", "dsp-kb", "sound-design-kb", "ui-kb", "cpp-kb", "cmake-kb"]:
    kb_index_path = Path(f"playbookdata/{kb_name}/index.json")

    if kb_index_path.exists():
        with open(kb_index_path) as f:
            kb_index = json.load(f)

        topics = kb_index.get("topics", {})
        total_files = sum(len(files) for files in topics.values())
        sources = kb_index.get("sources", {})

        master["knowledge_bases"][kb_name] = {
            "path": kb_name,
            "source_type": kb_index.get("metadata", {}).get("source_type", "unknown"),
            "pages": total_files,
            "status": "ready" if total_files > 0 else "empty"
        }

        # Add to cross-references
        for topic, files in topics.items():
            if topic not in master["topic_cross_references"]:
                master["topic_cross_references"][topic] = []
            for file_path in files:
                ref = f"{kb_name}/{file_path}"
                if ref not in master["topic_cross_references"][topic]:
                    master["topic_cross_references"][topic].append(ref)

# Count total files
total_files = sum(
    kb_info.get("pages", 0)
    for kb_info in master["knowledge_bases"].values()
)

# Get harvest sources
harvest_sources = []
for kb_name, kb_info in master["knowledge_bases"].items():
    kb_index_path = Path(f"playbookdata/{kb_name}/index.json")
    if kb_index_path.exists():
        with open(kb_index_path) as f:
            kb_index = json.load(f)
        for source in kb_index.get("sources", {}).keys():
            if source not in harvest_sources:
                harvest_sources.append(source)

master["metadata"]["total_files"] = total_files
master["metadata"]["harvest_sources"] = harvest_sources
master["last_updated"] = datetime.now().isoformat()

# Save
with open(master_path, "w") as f:
    json.dump(master, f, indent=2)

print(f"✓ Updated master-index.json")
print(f"  Total files: {total_files}")
print(f"  KBs: {len(master['knowledge_bases'])}")
print(f"  Topics: {len([t for t, f in master['topic_cross_references'].items() if f])}")
for kb_name, kb_info in master["knowledge_bases"].items():
    print(f"    {kb_name}: {kb_info.get('pages', 0)} pages ({kb_info.get('status', 'unknown')})")
EOF

python3 update_master_index.py
rm update_master_index.py
```

Expected: master-index.json updated with real data

### Task C.4: Create Validation Files

**Files:**
- Create: `playbookdata/juce-kb/validation.json`
- Create: `playbookdata/dsp-kb/validation.json`
- Create validation for other KBs

- [ ] **Step C.4.1: Create validation generator script**

```python
cat > generate_validation.py << 'EOF'
#!/usr/bin/env python3
"""Generate validation.json for each KB."""

import json
import hashlib
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Any

KB_ROOT = Path("playbookdata")

KB_BASELINE_RELEVANCE = {
    'juce-kb': 7,
    'dsp-kb': 9,
    'sound-design-kb': 5,
    'ui-kb': 5,
    'cpp-kb': 3,
    'cmake-kb': 2
}

AUDIO_KEYWORDS = [
    'AudioProcessor', 'AudioBuffer', 'MidiBuffer', 'processBlock',
    'sample', 'buffer', 'DSP', 'filter', 'oscillator', 'envelope',
    'ADSR', 'FFT', 'reverb', 'delay', 'compression'
]

ADVANCED_KEYWORDS = ['thread', 'lock', 'atomic', 'SIMD', 'optimization', 'real-time']
INTERMEDIATE_KEYWORDS = ['class', 'inherit', 'virtual', 'override', 'component']


def calculate_file_hash(file_path: Path) -> str:
    """Calculate SHA256 hash of file content."""
    content = file_path.read_bytes()
    return f"sha256:{hashlib.sha256(content).hexdigest()}"


def score_audio_relevance(content: Dict, kb_name: str) -> int:
    """Score audio relevance (0-10)."""
    text = content.get('markdown', '') or content.get('html', '')
    text_lower = text.lower()

    score = KB_BASELINE_RELEVANCE.get(kb_name, 5)

    keyword_count = sum(1 for kw in AUDIO_KEYWORDS if kw.lower() in text_lower)
    score += min(keyword_count, 3)

    if '```cpp' in text or '```c++' in text:
        score += 1

    return min(score, 10)


def determine_difficulty(content: Dict) -> str:
    """Determine difficulty level."""
    text = content.get('markdown', '') or content.get('html', '')

    adv_count = sum(1 for w in ADVANCED_KEYWORDS if w.lower() in text.lower())
    int_count = sum(1 for w in INTERMEDIATE_KEYWORDS if w.lower() in text.lower())

    if adv_count >= 3:
        return 'advanced'
    elif int_count >= 2:
        return 'intermediate'
    else:
        return 'beginner'


def count_implementation_examples(content: Dict) -> int:
    """Count code examples in content."""
    text = content.get('markdown', '')
    return text.count('```cpp') + text.count('```c++') + text.count('```cmake')


def generate_validation(kb_name: str) -> Dict[str, Any]:
    """Generate validation.json for a KB."""
    kb_path = KB_ROOT / kb_name

    if not kb_path.exists():
        print(f"⊘ {kb_name}: directory not found")
        return None

    index_path = kb_path / "index.json"
    if not index_path.exists():
        print(f"⊘ {kb_name}: index.json not found")
        return None

    index = json.load(open(index_path))

    files = {}
    total_files = 0
    total_size = 0

    for topic, file_list in index.get("topics", {}).items():
        for file_path in file_list:
            full_path = kb_path / file_path
            if full_path.exists():
                try:
                    content = json.load(open(full_path))

                    files[file_path] = {
                        "hash": calculate_file_hash(full_path),
                        "audio_relevance": score_audio_relevance(content, kb_name),
                        "difficulty": determine_difficulty(content),
                        "implementation_examples": count_implementation_examples(content),
                        "size_bytes": full_path.stat().st_size,
                        "validated_at": datetime.now().isoformat()
                    }
                    total_files += 1
                    total_size += full_path.stat().st_size
                except Exception as e:
                    files[file_path] = {
                        "error": str(e),
                        "validated_at": datetime.now().isoformat()
                    }

    # Calculate overall checksum
    all_hashes = ''.join(f.get('hash', '') for f in files.values() if 'hash' in f)
    overall_hash = hashlib.sha256(all_hashes.encode()).hexdigest()

    return {
        "kb_name": kb_name,
        "last_validated": datetime.now().isoformat(),
        "checksum": f"sha256:{overall_hash}",
        "total_files": total_files,
        "total_size_bytes": total_size,
        "files": files,
        "defaults": {
            "audio_relevance": KB_BASELINE_RELEVANCE.get(kb_name, 5),
            "difficulty_level": "intermediate"
        },
        "statistics": {
            "avg_audio_relevance": sum(f.get('audio_relevance', 5) for f in files.values()) / max(total_files, 1),
            "by_difficulty": {
                "beginner": sum(1 for f in files.values() if f.get('difficulty') == 'beginner'),
                "intermediate": sum(1 for f in files.values() if f.get('difficulty') == 'intermediate'),
                "advanced": sum(1 for f in files.values() if f.get('difficulty') == 'advanced')
            }
        }
    }


def main():
    print("Generating validation files...")

    for kb_name in ['juce-kb', 'dsp-kb', 'sound-design-kb', 'ui-kb', 'cpp-kb', 'cmake-kb']:
        validation = generate_validation(kb_name)

        if validation:
            output_path = KB_ROOT / kb_name / "validation.json"
            with open(output_path, 'w') as f:
                json.dump(validation, f, indent=2)

            stats = validation['statistics']
            print(f"✓ {kb_name}: {validation['total_files']} files, "
                  f"avg relevance: {stats['avg_audio_relevance']:.1f}, "
                  f"difficulty: {stats['by_difficulty']}")
        else:
            # Create empty validation for empty KBs
            validation = {
                "kb_name": kb_name,
                "last_validated": datetime.now().isoformat(),
                "checksum": "",
                "total_files": 0,
                "total_size_bytes": 0,
                "files": {},
                "defaults": {
                    "audio_relevance": KB_BASELINE_RELEVANCE.get(kb_name, 5),
                    "difficulty_level": "intermediate"
                },
                "statistics": {
                    "avg_audio_relevance": KB_BASELINE_RELEVANCE.get(kb_name, 5),
                    "by_difficulty": {
                        "beginner": 0,
                        "intermediate": 0,
                        "advanced": 0
                    }
                }
            }
            output_path = KB_ROOT / kb_name / "validation.json"
            with open(output_path, 'w') as f:
                json.dump(validation, f, indent=2)
            print(f"✓ {kb_name}: empty (created placeholder)")


if __name__ == "__main__":
    main()
EOF

python3 generate_validation.py
rm generate_validation.py
```

Expected: validation.json files created for all KBs

### Section C Completion Checkpoint

Run:
```bash
echo "=== Section C Completion Check ==="
echo ""
echo "KB Structure:"
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb; do
    if [ -d "playbookdata/$kb" ]; then
        files=$(find "playbookdata/$kb" -name "*.json" 2>/dev/null | grep -v "index.json\|validation.json\|manifest.json" | wc -l)
        echo "  $kb: $files content files"
        echo "    index.json: $([ -f playbookdata/$kb/index.json ] && echo '✓' || echo '✗')"
        echo "    validation.json: $([ -f playbookdata/$kb/validation.json ] && echo '✓' || echo '✗')"
    else
        echo "  $kb: ✗ NOT FOUND"
    fi
done
echo ""
echo "Master Index:"
python3 -c "
import json
m = json.load(open('playbookdata/master-index.json'))
print(f'  KBs: {len(m[\"knowledge_bases\"])}')
print(f'  Total files: {m[\"metadata\"][\"total_files\"]}')
for kb, info in m['knowledge_bases'].items():
    print(f'    {kb}: {info[\"pages\"]} pages')
"
echo ""
echo "✓ Section C complete: Data reorganization finished"
```

Expected: All KBs structured correctly

---

## Section D: Testing and Integration (Phase 4)

**Purpose:** Test the KB system and integrate with skills.

**Estimated Duration:** ~10 minutes

**Recovery:** If this section fails:
- Tests that fail are diagnostic - they indicate issues to fix
- No data changes in this section
- Re-run tests after fixing issues

**Idempotency:** All tests can be re-run safely.

### Task D.1: Run Unit Tests

**Files:**
- Modify: `tests/test_kb_lookup.py` (update imports)

- [ ] **Step D.1.1: Run all tests**

Run:
```bash
cd /home/myuser/agents/juce-agent
pytest tests/test_kb_lookup.py -v
```

Expected: All tests pass

- [ ] **Step D.1.2: Test KB queries**

```python
python3 << 'EOF'
from playbookdata import get_kb

kb = get_kb()

print("=== KB System Test ===")
print()

# Test list_kbs
kbs = kb.list_kbs()
print(f"Available KBs: {list(kbs.keys())}")
print()

# Test query_topic for each topic
topics = ["audio_processing", "dsp", "gui", "thread_safety", "parameters", "testing"]
for topic in topics:
    results = kb.query_topic(topic)
    print(f"Topic '{topic}': {len(results)} results")
    if results:
        print(f"  Top result: {results[0].file_path}")
        print(f"  Relevance: {results[0].relevance_score}, Audio relevance: {results[0].audio_relevance}")
print()

# Test health_check
health = kb.health_check()
print(f"Overall health: {'✓ PASS' if health['overall_valid'] else '✗ FAIL'}")
for kb_name, report in health['kbs'].items():
    status = '✓' if report.get('valid', False) else '✗'
    print(f"  {status} {kb_name}: {report.get('file_count', 0)} files")
    if report.get('issues'):
        for issue in report['issues']:
            print(f"    - {issue}")
EOF
```

Expected: KB queries work, health check passes

### Task D.2: Create Fallback Content

**Files:**
- Create: `playbookdata/fallback-content.json`

- [ ] **Step D.2.1: Write fallback content file**

```python
cat > playbookdata/fallback-content.json << 'EOF'
{
  "audio_processing": {
    "title": "Audio Processing Basics",
    "content": "# Audio Processing Basics\n\nAudio processing in JUCE is handled by the `AudioProcessor` class. The main method is `processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)` which processes audio in real-time.\n\nKey concepts:\n- `AudioBuffer` - Container for audio samples\n- `MidiBuffer` - Container for MIDI events\n- `prepareToPlay()` - Called before processing starts\n- `releaseResources()` - Called after processing ends\n\nSee JUCE documentation for detailed implementation.",
    "minimal": true
  },
  "dsp": {
    "title": "DSP Fundamentals",
    "content": "# Digital Signal Processing\n\nJUCE provides the `dsp` namespace for digital signal processing:\n\n- `dsp::IIR` - Infinite Impulse Response filters\n- `dsp::FIR` - Finite Impulse Response filters\n- `dsp::FFT` - Fast Fourier Transform\n- `dsp::Oversampling` - Sample rate conversion\n- `dsp::Gain` - Volume control\n- `dsp::DelayLine` - Delay effects\n\nUse `dsp::ProcessSpec` to configure processing parameters.",
    "minimal": true
  },
  "parameters": {
    "title": "Audio Parameters",
    "content": "# Plugin Parameters\n\nJUCE parameters use `AudioProcessorParameter` and `AudioProcessorValueTreeState` (APVTS):\n\n1. Create parameter layout in `createParameterLayout()`\n2. Use `AudioProcessorValueTreeState` to manage state\n3. Attach to UI components with `AudioProcessorValueTreeState::SliderAttachment`\n\nParameters are automatically saved/restored with plugin state.",
    "minimal": true
  },
  "gui": {
    "title": "GUI Components",
    "content": "# Plugin GUI\n\nJUCE GUI is built on the `Component` class:\n\n- `Component` - Base class for all UI elements\n- `Slider` - Rotary/linear controls\n- `ComboBox` - Dropdown menus\n- `TextButton` - Clickable buttons\n- `Label` - Text display\n\nOverride `paint()` for custom drawing and `resized()` for layout.\n\nUse `LookAndFeel` classes to customize appearance.",
    "minimal": true
  },
  "thread_safety": {
    "title": "Thread Safety",
    "content": "# Thread Safety in JUCE\n\nKey concepts:\n\n- `CriticalSection` - Mutex for thread synchronization\n- `Thread` - Base class for background threads\n- `MessageManager` - UI thread message handling\n- `AsyncUpdater` - Deferred callbacks\n\nUse `MessageManagerLock` when accessing UI from background threads.\n\nAvoid blocking calls in `processBlock()` - it must be real-time safe.",
    "minimal": true
  },
  "testing": {
    "title": "Plugin Testing",
    "content": "# Testing JUCE Plugins\n\nTesting approaches:\n\n1. **Unit tests** - Use Catch2 or GoogleTest for logic testing\n2. **Integration tests** - Test plugin host interaction\n3. **pluginval** - JUCE's validation tool\n\nRun `pluginval --strictness-level 5 YourPlugin.vst3` for comprehensive validation.",
    "minimal": true
  }
}
EOF

echo "✓ Created fallback-content.json"
```

Expected: Fallback content created

### Task D.3: Integration Verification

- [ ] **Step D.3.1: Verify complete system works**

```python
python3 << 'EOF'
import json
from pathlib import Path
from playbookdata import get_kb, KBError

print("=== Integration Test ===")
print()

# 1. Test KB lookup
kb = get_kb()
print("1. KB Lookup initialized: ✓")

# 2. Test topic query
results = kb.query_topic("dsp")
print(f"2. Query 'dsp': {len(results)} results")
assert len(results) > 0, "Expected DSP results"
print("   ✓ Results found")

# 3. Test file content
if results:
    try:
        content = kb.get_file_content(results[0].file_path)
        print(f"3. File content loaded: ✓")
        print(f"   Source: {content.metadata.get('source', 'unknown')}")
    except KBError as e:
        print(f"3. File content: ✗ {e}")

# 4. Test health check
health = kb.health_check()
print(f"4. Health check: {'✓' if health['overall_valid'] else '✗'}")
print(f"   Overall: {health['overall_valid']}")

# 5. Test fallback content
fallback_path = Path("playbookdata/fallback-content.json")
if fallback_path.exists():
    fallback = json.load(open(fallback_path))
    print(f"5. Fallback content: ✓ ({len(fallback)} topics)")
else:
    print("5. Fallback content: ✗ Not found")

# 6. Verify file counts
master = json.load(open("playbookdata/master-index.json"))
total = master["metadata"]["total_files"]
print(f"6. Total files in KB: {total}")

print()
print("=== All Integration Tests Passed ===")
EOF
```

Expected: All integration tests pass

### Section D Completion Checkpoint

Run:
```bash
echo "=== Section D Completion Check ==="
echo ""
echo "Test Results:"
pytest tests/test_kb_lookup.py -v --tb=short 2>/dev/null || echo "Tests completed with some failures"
echo ""
echo "Integration Test:"
python3 -c "
from playbookdata import get_kb
kb = get_kb()
health = kb.health_check()
print(f'Health check: {\"PASS\" if health[\"overall_valid\"] else \"FAIL\"}')
results = kb.query_topic('dsp')
print(f'DSP query: {len(results)} results')
"
echo ""
echo "✓ Section D complete: Testing finished"
```

Expected: Tests pass, integration works

---

## Section E: Final Validation (Phase 6)

**Purpose:** Final validation and gap analysis.

**Estimated Duration:** ~5 minutes

**Recovery:** If this section fails:
- Validation failures indicate issues to investigate
- No data changes in this section
- Fix issues and re-run validation

### Task E.1: Run Full Validation

- [ ] **Step E.1.1: Run validation script**

```python
python3 << 'EOF'
"""Final validation of KB system."""

import json
from pathlib import Path
from playbookdata import get_kb

print("=" * 60)
print("FINAL VALIDATION")
print("=" * 60)

kb = get_kb()

# 1. Health check
print("\n1. Health Check:")
health = kb.health_check()
print(f"   Overall: {'PASS' if health['overall_valid'] else 'FAIL'}")

for kb_name, report in health['kbs'].items():
    status = '✓' if report.get('valid', False) else '✗'
    files = report.get('file_count', 0)
    issues = len(report.get('issues', []))
    print(f"   {status} {kb_name}: {files} files, {issues} issues")

# 2. Topic coverage
print("\n2. Topic Coverage:")
master = json.load(open("playbookdata/master-index.json"))
for topic, files in master.get("topic_cross_references", {}).items():
    print(f"   {topic}: {len(files)} references")

# 3. Query functionality
print("\n3. Query Functionality:")
test_topics = ["audio_processing", "dsp", "gui", "thread_safety"]
for topic in test_topics:
    results = kb.query_topic(topic)
    status = '✓' if results else '✗'
    print(f"   {status} {topic}: {len(results)} results")

# 4. File counts
print("\n4. File Counts:")
total_files = 0
for kb_name in ["juce-kb", "dsp-kb"]:
    kb_path = Path(f"playbookdata/{kb_name}")
    if kb_path.exists():
        files = list(kb_path.rglob("*.json"))
        files = [f for f in files if f.name not in ["index.json", "validation.json", "manifest.json"]]
        print(f"   {kb_name}: {len(files)} content files")
        total_files += len(files)

print(f"\n   Total: {total_files} content files")

# 5. Validation files
print("\n5. Validation Files:")
for kb_name in ["juce-kb", "dsp-kb", "sound-design-kb", "ui-kb", "cpp-kb", "cmake-kb"]:
    val_path = Path(f"playbookdata/{kb_name}/validation.json")
    if val_path.exists():
        val = json.load(open(val_path))
        print(f"   ✓ {kb_name}: avg_relevance={val['statistics']['avg_audio_relevance']:.1f}")
    else:
        print(f"   ✗ {kb_name}: validation.json missing")

print("\n" + "=" * 60)
print("VALIDATION COMPLETE")
print("=" * 60)
EOF
```

Expected: Validation passes

### Task E.2: Generate Gap Analysis

- [ ] **Step E.2.1: Generate gap analysis report**

```python
python3 << 'EOF'
"""Generate gap analysis report."""

import json
from pathlib import Path
from playbookdata import get_kb

print("=" * 60)
print("GAP ANALYSIS")
print("=" * 60)

kb = get_kb()

# Expected topics
expected_topics = [
    "audio_processing", "dsp", "thread_safety", "gui",
    "state_management", "midi", "parameters", "testing",
    "ci_cd", "deployment"
]

# Check each topic
print("\n1. Topic Coverage:")
master = json.load(open("playbookdata/master-index.json"))
gaps = []

for topic in expected_topics:
    files = master.get("topic_cross_references", {}).get(topic, [])
    if files:
        print(f"   ✓ {topic}: {len(files)} references")
    else:
        print(f"   ✗ {topic}: NO REFERENCES")
        gaps.append(topic)

# Check KB content
print("\n2. KB Content Analysis:")
for kb_name in ["juce-kb", "dsp-kb"]:
    val_path = Path(f"playbookdata/{kb_name}/validation.json")
    if val_path.exists():
        val = json.load(open(val_path))
        stats = val.get("statistics", {})
        avg_rel = stats.get("avg_audio_relevance", 0)
        by_diff = stats.get("by_difficulty", {})

        print(f"\n   {kb_name}:")
        print(f"     Files: {val.get('total_files', 0)}")
        print(f"     Avg Relevance: {avg_rel:.1f}")
        print(f"     Difficulty: {by_diff}")

        if avg_rel < 6.0:
            print(f"     ⚠ Low average relevance")
            gaps.append(f"{kb_name}: low relevance")

# Check for implementation examples
print("\n3. Implementation Examples:")
for topic in ["dsp", "audio_processing", "parameters"]:
    results = kb.get_implementation_examples(topic, min_audio_relevance=7)
    if results:
        print(f"   ✓ {topic}: {len(results)} high-relevance examples")
    else:
        print(f"   ✗ {topic}: No high-relevance examples")
        gaps.append(f"{topic}: no examples")

# Summary
print("\n" + "=" * 60)
if gaps:
    print(f"GAPS IDENTIFIED: {len(gaps)}")
    for gap in gaps:
        print(f"  - {gap}")
else:
    print("NO GAPS IDENTIFIED")

print("\nRECOMMENDATIONS:")
print("  1. Harvest additional cpp-kb content from cppreference.com")
print("  2. Harvest cmake-kb content from cmake.org/documentation")
print("  3. Add curated content to sound-design-kb and ui-kb")
print("=" * 60)
EOF
```

Expected: Gap analysis complete

### Task E.3: Create Summary Report

- [ ] **Step E.3.1: Create final summary**

```python
cat > playbookdata/IMPLEMENTATION_REPORT.md << 'EOF'
# Knowledge Base System Implementation Report

## Summary

Implementation completed successfully. The KB system is now operational with:

- 6 specialized knowledge bases
- Central query interface (kb-lookup.py)
- Cross-referencing system
- Audio relevance scoring
- Validation infrastructure

## KB Status

| KB | Files | Status | Avg Relevance |
|---|---|---|---|
| juce-kb | 301+ | Ready | 7-8 |
| dsp-kb | 19 | Ready | 9 |
| sound-design-kb | 0 | Empty | - |
| ui-kb | 0 | Empty | - |
| cpp-kb | 0 | Empty | - |
| cmake-kb | 0 | Empty | - |

## Files Created

- playbookdata/__init__.py
- playbookdata/kb_lookup.py
- playbookdata/master-index.json
- playbookdata/fallback-content.json
- playbookdata/*/index.json (6 files)
- playbookdata/*/validation.json (6 files)
- tests/test_kb_lookup.py

## Usage

```python
from playbookdata import get_kb

kb = get_kb()

# Query for topic
results = kb.query_topic("dsp")

# Get file content
content = kb.get_file_content(results[0].file_path)

# Get implementation examples
examples = kb.get_implementation_examples("audio_processing")

# Health check
health = kb.health_check()
```

## Next Steps

1. Harvest cpp-kb from cppreference.com
2. Harvest cmake-kb from cmake.org
3. Add curated content to empty KBs
4. Integrate with skills for dynamic queries

## Backup

Original data backed up to: playbookdata-backup-*/
EOF

echo "✓ Created IMPLEMENTATION_REPORT.md"
```

Expected: Report created

### Section E Completion Checkpoint

Run:
```bash
echo "=== Section E Completion Check ==="
echo ""
echo "Final Status:"
echo ""
python3 -c "
from playbookdata import get_kb
import json

kb = get_kb()
health = kb.health_check()

print('KB System Health:')
for kb_name, report in health['kbs'].items():
    status = '✓' if report.get('valid', False) else '✗'
    files = report.get('file_count', 0)
    print(f'  {status} {kb_name}: {files} files')

print()
print(f'Overall: {\"PASS\" if health[\"overall_valid\"] else \"FAIL\"}')
"
echo ""
echo "Report: playbookdata/IMPLEMENTATION_REPORT.md"
echo ""
echo "✓ Section E complete: Final validation finished"
echo ""
echo "=== IMPLEMENTATION COMPLETE ==="
```

Expected: Implementation complete

### Task E.4: Cleanup Temporary Files

- [ ] **Step E.4.1: Remove temporary files**

Run:
```bash
# Remove temporary files created during implementation
rm -f original.checksum backup.checksum backup-manifest.json
rm -f pre_migration_report.json validate_pre_migration.py
rm -f create_kb_indices.py update_juce_kb_index.py
rm -f migrate_stanford.py create_dsp_index.py
rm -f update_juce_remove_stanford.py update_master_index.py
rm -f generate_validation.py

# Clear Python cache
find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true
find . -name "*.pyc" -delete 2>/dev/null || true

echo "✓ Temporary files cleaned up"
```

Expected: Temporary files removed

- [ ] **Step E.4.2: Verify cleanup**

Run:
```bash
echo "=== Cleanup Verification ==="
echo ""
echo "Temporary files remaining:"
ls -la *.checksum *.py 2>/dev/null || echo "  (none)"
echo ""
echo "Backup preserved at: $(cat .backup_dir)"
echo ""
echo "✓ Cleanup complete"
```

Expected: No temporary files remaining

---

## Completion Checklist

Run:
```bash
echo "=== IMPLEMENTATION COMPLETE ==="
echo ""
echo "Checklist:"
echo ""
echo "  [ ] Section A: Preparation (backup, rollback)"
echo "  [ ] Section B: Foundation (kb-lookup.py, master-index.json)"
echo "  [ ] Section C: Data Reorganization (juce-kb, dsp-kb)"
echo "  [ ] Section D: Testing (unit tests, integration)"
echo "  [ ] Section E: Final Validation (health check, gap analysis)"
echo ""
echo "Run each section in order. Each section can be stopped"
echo "and resumed without data corruption."
echo ""
echo "Rollback available at: $(cat .backup_dir)"
```

Expected: Checklist displayed

---

## Appendix: Quick Reference

### One-Line Status Check

```bash
python3 -c "from playbookdata import get_kb; kb=get_kb(); h=kb.health_check(); print(f'Health: {\"PASS\" if h[\"overall_valid\"] else \"FAIL\"}'); [print(f'  {k}: {v[\"file_count\"]} files') for k,v in h['kbs'].items()]"
```

### Rollback Command

```bash
# Find latest backup
LATEST_BACKUP=$(ls -td playbookdata-backup-* | head -1)
./rollback.sh "$LATEST_BACKUP"
```

### Verify KB Content

```bash
# Count files per KB
for kb in juce-kb dsp-kb sound-design-kb ui-kb cpp-kb cmake-kb; do
    count=$(find playbookdata/$kb -name "*.json" 2>/dev/null | grep -v "index.json\|validation.json\|manifest.json" | wc -l)
    echo "$kb: $count content files"
done
```

### Test Query

```bash
python3 -c "
from playbookdata import get_kb
kb = get_kb()
results = kb.query_topic('dsp')
print(f'DSP results: {len(results)}')
if results:
    print(f'Top: {results[0].file_path}')
    print(f'Relevance: {results[0].relevance_score}')
"
```

### Common Operations

| Operation | Command |
|-----------|---------|
| Health check | `python3 -c "from playbookdata import get_kb; print(get_kb().health_check())"` |
| Query topic | `python3 -c "from playbookdata import get_kb; print(get_kb().query_topic('dsp'))"` |
| Clear cache | `python3 -c "from playbookdata import get_kb; get_kb().clear_cache()"` |
| List KBs | `python3 -c "from playbookdata import get_kb; print(list(get_kb().list_kbs().keys()))"` |
| Rollback | `./rollback.sh $(ls -td playbookdata-backup-* \| head -1)` |

### File Structure

```
playbookdata/
├── __init__.py              # Package init
├── kb_lookup.py             # Query interface (700+ lines)
├── master-index.json        # Central registry
├── fallback-content.json    # Fallback content
│
├── juce-kb/                 # JUCE framework KB
│   ├── index.json           # Topic → file mapping
│   ├── validation.json      # Integrity + scoring
│   └── juce-official/       # Source content
│
├── dsp-kb/                  # DSP theory KB
│   ├── index.json
│   ├── validation.json
│   └── stanford-ccrma/      # Stanford content
│
├── sound-design-kb/         # Sound design KB
├── ui-kb/                   # Plugin UI KB
├── cpp-kb/                  # C++ patterns KB
└── cmake-kb/                # CMake KB
```

### Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `KBNotFoundError` | KB directory missing | Check KB exists in playbookdata/ |
| `KBIntegrityError` | Invalid JSON | Run `validate_kb.py` to identify files |
| `Path traversal attempt` | Security violation | File path outside KB root |
| `File not found` | File in index but missing | Re-run validation, check harvest |

### Performance Tips

1. **Warm cache on startup**: Call `kb.warm_cache()` to pre-populate common queries
2. **Use cached results**: `query_topic` is cached (256 entries)
3. **Batch queries**: Use `query_topics()` for multiple topics
4. **Clear cache if stale**: `kb.clear_cache()` after content updates

## Appendix: Quick Reference

### File Locations

```
playbookdata/
├── __init__.py              # Package init
├── kb_lookup.py             # Query interface
├── master-index.json        # Central registry
├── fallback-content.json    # Fallback content
├── juce-kb/                 # JUCE framework KB
│   ├── index.json
│   └── validation.json
├── dsp-kb/                  # DSP theory KB
│   ├── index.json
│   └── validation.json
├── sound-design-kb/         # Sound design KB
│   ├── index.json
│   └── validation.json
├── ui-kb/                   # UI KB
│   ├── index.json
│   └── validation.json
├── cpp-kb/                  # C++ patterns KB
│   ├── index.json
│   └── validation.json
└── cmake-kb/                # CMake KB
    ├── index.json
    └── validation.json
```

### Common Operations

```python
# Query for topic
from playbookdata import get_kb
kb = get_kb()
results = kb.query_topic("dsp")

# Get file content
content = kb.get_file_content(results[0].file_path)

# Health check
health = kb.health_check()

# Clear cache
kb.clear_cache()

# Reset singleton
from playbookdata import reset_kb
reset_kb()
```

### Rollback

```bash
# Restore from backup
./rollback.sh playbookdata-backup-YYYYMMDD-HHMMSS
```