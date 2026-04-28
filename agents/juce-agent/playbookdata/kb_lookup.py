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
        """Load JSON file with error handling."""
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
        """Save JSON file with formatting."""
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, 'w') as f:
            json.dump(data, f, indent=2)

    # ========================================================================
    # KB Index Access
    # ========================================================================

    def _get_kb_index(self, kb_name: str) -> Optional[Dict[str, Any]]:
        """Load and cache KB index."""
        if kb_name not in self._kb_cache:
            try:
                index_path = KB_ROOT / kb_name / "index.json"
                self._kb_cache[kb_name] = self._load_json(index_path)
            except (KBNotFoundError, KBIntegrityError) as e:
                logger.warning(f"Index not found for KB: {kb_name}: {e}")
                self._kb_cache[kb_name] = None
        return self._kb_cache[kb_name]

    def list_kbs(self) -> Dict[str, Dict]:
        """List all available knowledge bases with their status."""
        return self.master_index.get("knowledge_bases", {})

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
        """Query all KBs for a topic, return ranked results."""
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
        """Batch query multiple topics efficiently."""
        kb_tuple = tuple(kb_names) if kb_names else None
        return {
            topic: self.query_topic(topic, kb_tuple)
            for topic in topics
        }

    def get_file_content(self, file_path: str, use_cache: bool = True) -> KBFile:
        """Load full content from a KB file."""
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
        """Get implementation examples with high audio relevance."""
        results = self.query_topic(topic)
        filtered = [r for r in results if r.audio_relevance >= min_audio_relevance]
        return filtered[:max_results]

    def search_content(
        self,
        query: str,
        kb_names: Optional[List[str]] = None,
        case_sensitive: bool = False
    ) -> List[KBResult]:
        """Full-text search across KB content."""
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
        """Find topics related to the given topic via cross-references."""
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
        """Suggest which skills to use for a given topic."""
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
        """Get files that this file references or depends on."""
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

    # ========================================================================
    # KB Management and Validation
    # ========================================================================

    def validate_kb(self, kb_name: str) -> Dict[str, Any]:
        """Validate KB integrity."""
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
        """Run health check on all KBs."""
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
        """Pre-warm caches with common queries."""
        topics = topics or ["dsp", "thread_safety", "parameters", "gui", "audio_processing"]

        logger.info(f"Warming cache for {len(topics)} topics...")
        for topic in topics:
            self.query_topic(topic)

        logger.info("Cache warm complete")

    # ========================================================================
    # Private Helper Methods
    # ========================================================================

    def _get_audio_relevance(self, kb_name: str, file_path: str) -> int:
        """Get audio relevance score from validation metadata."""
        try:
            validation = self._load_json(KB_ROOT / kb_name / "validation.json")
            file_rel = validation.get("files", {}).get(file_path, {})
            return file_rel.get("audio_relevance",
                              validation.get("defaults", {}).get("audio_relevance", 5))
        except (KBNotFoundError, KBIntegrityError):
            return KB_BASELINE_RELEVANCE.get(kb_name, 5)

    def _calculate_relevance(self, file_path: str, topic: str, kb_name: str) -> float:
        """Calculate relevance score based on file metadata and topic match."""
        score = 10.0

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
        """Score a search hit based on frequency and position."""
        count = text.count(query)
        position = text.find(query)

        score = min(count * 0.5, 5.0)
        if position >= 0:
            score += max(5 - position / 100, 0)

        return min(score, 10.0)

    def _extract_title(self, kb_name: str, file_path: str) -> str:
        """Extract title from file content."""
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
        """Get source attribution for a file."""
        try:
            content = self._get_file_content_raw(kb_name, file_path)
            if content:
                return content.get("source", "unknown")
        except:
            pass
        return "unknown"

    def _get_file_content_raw(self, kb_name: str, file_path: str) -> Optional[Dict]:
        """Get raw file content without KBFile wrapper."""
        cache_key = f"{kb_name}:{file_path}"
        if cache_key in self._file_cache:
            return self._file_cache[cache_key].content

        try:
            return self._load_json(KB_ROOT / kb_name / file_path)
        except:
            return None

    def _get_all_kb_files(self, kb_name: str) -> List[str]:
        """Get all file paths in a KB from its index."""
        index = self._get_kb_index(kb_name)
        if not index:
            return []

        files = []
        for topic_files in index.get("topics", {}).values():
            files.extend(topic_files)
        return list(set(files))

    def _extract_kb_name(self, path: Path) -> str:
        """Extract KB name from file path."""
        try:
            relative = path.relative_to(KB_ROOT)
            return relative.parts[0]
        except ValueError:
            return "unknown"

    def _extract_metadata(self, content: Dict) -> Dict:
        """Extract relevant metadata from content."""
        return {
            "source": content.get("source", "unknown"),
            "harvested_at": content.get("harvested_at"),
            "source_url": content.get("source_url"),
            "juce_version": content.get("juce_version")
        }

    def _find_file_for_url(self, kb_name: str, url: str) -> Optional[str]:
        """Find KB file that matches a URL."""
        for file_path in self._get_all_kb_files(kb_name):
            try:
                content = self._get_file_content_raw(kb_name, file_path)
                if content and content.get("source_url") == url:
                    return file_path
            except:
                continue
        return None


# ============================================================================
# Singleton Instance
# ============================================================================

_kb_lookup: Optional[KBQuery] = None
_init_lock = threading.Lock()


def get_kb(initialize: bool = True) -> Optional[KBQuery]:
    """Get the singleton KB lookup instance."""
    global _kb_lookup
    if _kb_lookup is None and initialize:
        with _init_lock:
            if _kb_lookup is None:
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
    """Convenience function to query a topic."""
    kb = get_kb()
    if kb is None:
        return []
    kb_tuple = tuple(kb_names) if kb_names else None
    return kb.query_topic(topic, kb_tuple)


def get_content(topic: str, fallback: str = "") -> str:
    """Get markdown content for a topic with fallback."""
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