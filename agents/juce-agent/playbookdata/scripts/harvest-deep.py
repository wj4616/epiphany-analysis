#!/usr/bin/env python3
"""
Deep Harvesting System for Knowledge Base
Professional-grade content harvester with:
- Uses search_terms as seed inspiration, generates new terms
- Deep crawling with proper status handling
- Saves full raw JSON with no size limits
- Automatic term discovery and extraction
- Progress checkpointing and resume capability
- Accurate credit tracking
- Rate limit handling with retries
"""

import json
import os
import sys
import time
import requests
import fcntl
from datetime import datetime, timezone
from pathlib import Path
from urllib.parse import urlparse
import re
import hashlib
from typing import Optional

# Configuration - read env vars at runtime to allow late binding
def get_api_key():
    """Get API key from environment (allows setting after module load)."""
    return os.environ.get("FIRECRAWL_API_KEY", "")

FIRECRAWL_API_URL = os.environ.get("FIRECRAWL_API_URL", "https://api.firecrawl.dev")
FIRECRAWL_API_KEY = os.environ.get("FIRECRAWL_API_KEY", "")  # Fallback, use get_api_key() for fresh read
FIRECRAWL_DELAY = float(os.environ.get("FIRECRAWL_DELAY", "1.0"))
FIRECRAWL_MAX_RETRIES = int(os.environ.get("FIRECRAWL_MAX_RETRIES", "3"))
FIRECRAWL_RETRY_DELAY = float(os.environ.get("FIRECRAWL_RETRY_DELAY", "5.0"))

# Output directories - use absolute path from script location
SCRIPT_DIR = Path(__file__).parent.resolve()
KB_ROOT = SCRIPT_DIR.parent
RAW_OUTPUT_DIR = KB_ROOT / "harvested" / "raw"
PROCESSED_DIR = KB_ROOT / "harvested" / "processed"
MANIFEST_DIR = KB_ROOT / "harvested" / "manifests"
TERMS_DIR = KB_ROOT / "harvested" / "discovered-terms"
CHECKPOINT_DIR = KB_ROOT / "harvested" / "checkpoints"
LOCK_DIR = KB_ROOT / "harvested" / "locks"

# Common words to filter from extracted terms
COMMON_WORDS = {
    'the', 'and', 'for', 'are', 'but', 'not', 'you', 'all', 'can', 'her',
    'was', 'one', 'our', 'out', 'has', 'had', 'his', 'him', 'its', 'let',
    'may', 'new', 'now', 'old', 'see', 'way', 'who', 'how', 'when', 'what',
    'where', 'which', 'while', 'this', 'that', 'these', 'those', 'then', 'than',
    'there', 'their', 'they', 'them', 'some', 'such', 'same', 'from', 'into',
    'over', 'just', 'only', 'also', 'more', 'most', 'made', 'make', 'many',
    'much', 'been', 'being', 'have', 'each', 'other', 'after', 'before',
    'should', 'would', 'could', 'might', 'must', 'will', 'with', 'within',
    'without', 'about', 'above', 'below', 'between', 'during', 'through',
    'using', 'used', 'uses', 'use', 'code', 'data', 'file', 'files', 'value',
    'values', 'function', 'functions', 'method', 'methods', 'class', 'classes',
    'example', 'examples', 'note', 'notes', 'returns', 'return', 'param',
    'params', 'type', 'types', 'void', 'int', 'float', 'double', 'string',
    'audio', 'sound', 'signal', 'signals', 'plugin', 'plugins'
}


def acquire_lock(resource_name: str) -> tuple:
    """
    Acquire an exclusive lock for a resource.

    Creates a lock file in LOCK_DIR and acquires an exclusive lock.
    Uses non-blocking mode to fail immediately if lock is held.

    Args:
        resource_name: Unique identifier for the resource (e.g., "dsp-kb_reverb_algorithmic-reverb.json")

    Returns:
        Tuple of (lock_path, fd) on success, (None, None) if lock failed
    """
    try:
        LOCK_DIR.mkdir(parents=True, exist_ok=True)
        lock_path = LOCK_DIR / f"{resource_name}.lock"

        fd = open(lock_path, 'w')
        fcntl.flock(fd.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
        fd.write(f"{os.getpid()}\n{datetime.now(timezone.utc).isoformat()}\n")
        fd.flush()

        return lock_path, fd
    except (IOError, OSError) as e:
        # Lock is held by another process
        if fd:
            try:
                fd.close()
            except:
                pass
        return None, None


def release_lock(lock_path: Path, fd) -> None:
    """
    Release a previously acquired lock.

    Args:
        lock_path: Path to the lock file
        fd: File descriptor of the lock file
    """
    if fd:
        try:
            fcntl.flock(fd.fileno(), fcntl.LOCK_UN)
            fd.close()
        except:
            pass

    if lock_path and lock_path.exists():
        try:
            lock_path.unlink()
        except:
            pass


class CreditTracker:
    """Track Firecrawl API credit usage."""

    def __init__(self):
        self.credits_used = 0
        self.operations = []

    def add_search(self, query: str, results_count: int):
        """Add search operation credits (5 + 1 per result)."""
        credits = 5 + results_count
        self.credits_used += credits
        self.operations.append({
            "type": "search",
            "query": query[:100],
            "results": results_count,
            "credits": credits
        })

    def add_crawl(self, url: str, pages_count: int):
        """Add crawl operation credits (1 per page)."""
        credits = pages_count
        self.credits_used += credits
        self.operations.append({
            "type": "crawl",
            "url": url[:100],
            "pages": pages_count,
            "credits": credits
        })

    def add_scrape(self, url: str):
        """Add scrape operation credits (1 per page)."""
        self.credits_used += 1
        self.operations.append({
            "type": "scrape",
            "url": url[:100],
            "credits": 1
        })

    def to_dict(self) -> dict:
        return {
            "total_credits": self.credits_used,
            "operations": self.operations
        }


class Checkpoint:
    """Save and restore harvest progress."""

    def __init__(self, kb_type: str, topic: str, filename: str):
        self.checkpoint_file = CHECKPOINT_DIR / f"{kb_type}_{topic}_{filename}.json"
        self.data = {
            "kb_type": kb_type,
            "topic": topic,
            "filename": filename,
            "phase": "initialized",
            "queries_completed": [],
            "urls_crawled": [],
            "discovered_terms": [],
            "credits_used": 0,
            "started_at": datetime.now(timezone.utc).isoformat(),
            "updated_at": datetime.now(timezone.utc).isoformat()
        }

    def save(self):
        """Save checkpoint to file."""
        self.data["updated_at"] = datetime.now(timezone.utc).isoformat()
        CHECKPOINT_DIR.mkdir(parents=True, exist_ok=True)
        with open(self.checkpoint_file, 'w') as f:
            json.dump(self.data, f, indent=2)

    def load(self) -> bool:
        """Load checkpoint from file if exists."""
        if self.checkpoint_file.exists():
            with open(self.checkpoint_file) as f:
                self.data = json.load(f)
            return True
        return False

    def mark_query_done(self, query: str):
        self.data["queries_completed"].append(query)
        self.data["phase"] = f"query_{len(self.data['queries_completed'])}"
        self.save()

    def mark_url_crawled(self, url: str):
        if url not in self.data["urls_crawled"]:
            self.data["urls_crawled"].append(url)
            self.save()

    def add_terms(self, terms: list):
        for term in terms:
            if term not in self.data["discovered_terms"]:
                self.data["discovered_terms"].append(term)
        self.save()

    def update_credits(self, credits: int):
        self.data["credits_used"] = credits
        self.save()

    def complete(self):
        self.data["phase"] = "completed"
        self.save()

    def get_remaining_queries(self, all_queries: list) -> list:
        """Get queries not yet completed."""
        done = set(self.data["queries_completed"])
        return [q for q in all_queries if q not in done]

    def get_remaining_urls(self, all_urls: list) -> list:
        """Get URLs not yet crawled."""
        done = set(self.data["urls_crawled"])
        return [u for u in all_urls if u not in done]


class FirecrawlClient:
    """Firecrawl API client with retry logic."""

    def __init__(self, api_key: str, api_url: str):
        self.api_key = api_key
        self.api_url = api_url
        self.session = requests.Session()
        self.session.headers.update({
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json"
        })

    def _request_with_retry(self, method: str, endpoint: str,
                            payload: dict = None, timeout: int = 120) -> dict:
        """Make API request with retry logic."""
        url = f"{self.api_url}{endpoint}"
        last_error = None

        for attempt in range(FIRECRAWL_MAX_RETRIES):
            try:
                if method == "POST":
                    response = self.session.post(url, json=payload, timeout=timeout)
                elif method == "GET":
                    response = self.session.get(url, timeout=timeout)
                else:
                    raise ValueError(f"Unsupported method: {method}")

                # Handle rate limiting
                if response.status_code == 429:
                    retry_after = int(response.headers.get("Retry-After", FIRECRAWL_RETRY_DELAY * 2))
                    print(f"    Rate limited, waiting {retry_after}s...")
                    time.sleep(retry_after)
                    continue

                response.raise_for_status()
                return response.json()

            except requests.exceptions.Timeout:
                last_error = f"Timeout after {timeout}s"
                print(f"    Timeout (attempt {attempt + 1}/{FIRECRAWL_MAX_RETRIES})")
                time.sleep(FIRECRAWL_RETRY_DELAY)

            except requests.exceptions.HTTPError as e:
                if response.status_code >= 500:
                    last_error = f"Server error: {response.status_code}"
                    print(f"    Server error (attempt {attempt + 1}/{FIRECRAWL_MAX_RETRIES})")
                    time.sleep(FIRECRAWL_RETRY_DELAY)
                else:
                    # Client error - don't retry
                    return {"error": str(e), "status_code": response.status_code}

            except requests.exceptions.RequestException as e:
                last_error = str(e)
                print(f"    Network error (attempt {attempt + 1}/{FIRECRAWL_MAX_RETRIES}): {e}")
                time.sleep(FIRECRAWL_RETRY_DELAY)

        return {"error": last_error, "retries_exhausted": True}

    def search(self, query: str, domains: list = None, limit: int = 10) -> dict:
        """Search Firecrawl index."""
        payload = {
            "query": query,
            "limit": limit,
            "scrapeOptions": {
                "formats": ["markdown", "html", "rawHtml"],
                "onlyMainContent": False
            }
        }
        # Note: Firecrawl v1 search API doesn't support domain filtering
        # Domain filtering is done post-search if needed

        return self._request_with_retry("POST", "/v1/search", payload)

    def start_crawl(self, url: str, max_depth: int = 2, limit: int = 50) -> dict:
        """Start a crawl job."""
        payload = {
            "url": url,
            "limit": limit,
            "maxDepth": max_depth,
            "scrapeOptions": {
                "formats": ["markdown", "html", "rawHtml"],
                "onlyMainContent": False
            }
        }
        return self._request_with_retry("POST", "/v1/crawl", payload, timeout=60)

    def get_crawl_status(self, crawl_id: str) -> dict:
        """Get crawl job status."""
        return self._request_with_retry("GET", f"/v1/crawl/{crawl_id}", timeout=30)

    def wait_for_crawl(self, crawl_id: str, max_wait: int = 300,
                        poll_interval: int = 5, status_callback=None) -> dict:
        """Wait for crawl to complete with proper status handling."""
        start_time = time.time()
        valid_statuses = {"pending", "processing", "scraping", "completed", "failed"}

        while time.time() - start_time < max_wait:
            result = self.get_crawl_status(crawl_id)

            if "error" in result:
                return result

            status = result.get("status", "unknown")

            # Validate status
            if status not in valid_statuses:
                print(f"    Warning: Unknown crawl status: {status}")

            # Call status callback if provided
            if status_callback:
                pages = len(result.get("data", []))
                status_callback(status, pages)

            if status == "completed":
                return result
            elif status == "failed":
                error_msg = result.get("error", "Unknown error")
                return {"error": f"Crawl failed: {error_msg}"}
            elif status in ("pending", "processing", "scraping"):
                time.sleep(poll_interval)
            else:
                # Unknown status - wait and retry
                time.sleep(poll_interval)

        return {"error": f"Crawl timeout after {max_wait}s", "crawl_id": crawl_id}

    def scrape(self, url: str) -> dict:
        """Scrape a single page."""
        payload = {
            "url": url,
            "formats": ["markdown", "html", "rawHtml", "links"],
            "onlyMainContent": False
        }
        return self._request_with_retry("POST", "/v1/scrape", payload)


class DeepHarvester:
    """Harvest content using Firecrawl with automatic term discovery."""

    def __init__(self, kb_type: str, topic: str, filename: str):
        self.kb_type = kb_type
        self.topic = topic
        self.filename = filename
        self.placeholder_path = KB_ROOT / f"{kb_type}/{topic}/{filename}"

        # Verify placeholder exists
        if not self.placeholder_path.exists():
            raise FileNotFoundError(f"Placeholder not found: {self.placeholder_path}")

        # Load placeholder
        with open(self.placeholder_path) as f:
            self.data = json.load(f)

        self.seed_terms = self.data.get("search_terms", [])
        self.source_hints = self.data.get("sources", [])
        self.title = self.data.get("title", "")
        self.description = self.data.get("description", "")

        # Initialize components
        self.client = FirecrawlClient(get_api_key(), FIRECRAWL_API_URL)
        self.credits = CreditTracker()
        self.checkpoint = Checkpoint(kb_type, topic, filename)
        self.discovered_terms = set()
        self.crawled_urls = set()

    def get_domain_priorities(self) -> dict:
        """Map source hints to domain priorities for crawling."""
        domain_map = {
            "CCRMA": ["ccrma.stanford.edu"],
            "Stanford CCRMA": ["ccrma.stanford.edu"],
            "EarLevel blog": ["earlevel.com"],
            "musicdsp.org": ["musicdsp.org", "www.musicdsp.org"],
            "JUCE forum": ["forum.juce.com", "juce.com"],
            "melatonin.dev": ["melatonin.dev"],
            "Sound on Sound": ["soundonsound.com"],
            "Attack Magazine": ["attackmagazine.com"],
            "MIDI Association": ["midi.org", "www.midi.org"],
            "MPE specification": ["midi.org", "www.midi.org"],
            "Apple docs": ["developer.apple.com"],
            "Steinberg docs": ["steinberg.net", "developer.steinberg.help", "steinberg.help"],
            "Avid docs": ["avid.com"],
            "pluginval docs": ["github.com", "tracktion.github.io"],
            "DSP Stack Exchange": ["dsp.stackexchange.com"],
            "Signal Processing Stack Exchange": ["dsp.stackexchange.com"],
            "GitHub": ["github.com"],
            "Plugin Doctor": ["plugin-doctor.com"],
            "AES papers": ["aes.org", "www.aes.org"],
            "REAPER forums": ["forum.cockos.com", "reaper.fm"],
            "Cockos docs": ["reaper.fm", "cockos.com"],
            "Ableton docs": ["ableton.com", "help.ableton.com"],
            "CI docs": ["docs.github.com", "github.com"],
            "YouTube tutorials": ["youtube.com", "youtu.be"],
            "Synth tutorials": [],
            "Plugin development forums": ["forum.juce.com", "kvraudio.com"],
            "Plugin development blogs": [],
            "Music production blogs": [],
            "Synth docs": [],
            "DSP textbooks": [],
            "Testing blogs": [],
        }

        domains = []
        priority = 5

        for hint in self.source_hints:
            if hint in domain_map:
                domains.extend(domain_map[hint])
                priority = max(priority, 6)  # Known sources get higher priority

        return {
            "domains": list(set(domains)),
            "priority": priority
        }

    def generate_search_queries(self) -> list:
        """Generate diverse search queries from seed terms."""
        queries = []

        if not self.seed_terms:
            queries.append(self.title)
            return queries

        # 1. Primary seed terms (top 3)
        queries.extend(self.seed_terms[:3])

        # 2. Title + primary term combinations
        primary = self.seed_terms[0]
        queries.append(f"{self.title} {primary}")
        queries.append(f"{primary} implementation")
        queries.append(f"{primary} algorithm")

        # 3. KB-specific context
        if self.kb_type == "juce":
            queries.append(f"{primary} JUCE")
            queries.append(f"{primary} JUCE C++")
        elif self.kb_type == "dsp":
            queries.append(f"{primary} DSP")
            queries.append(f"{primary} algorithm code")
        elif self.kb_type == "testing":
            queries.append(f"{primary} test")
            queries.append(f"{primary} unit test")
        elif self.kb_type == "midi":
            queries.append(f"{primary} MIDI")
            queries.append(f"{primary} implementation")

        # 4. Tutorial/article variations
        queries.append(f"{primary} tutorial")
        queries.append(f"{primary} guide")
        queries.append(f"how to {primary.lower()}")

        # Deduplicate while preserving order
        seen = set()
        unique = []
        for q in queries:
            q_lower = q.lower()
            if q_lower not in seen:
                seen.add(q_lower)
                unique.append(q)

        return unique

    def extract_terms(self, content: str) -> tuple:
        """Extract technical terms from content."""
        if not content:
            return [], []

        terms = set()
        urls = set()

        # Technical term patterns
        patterns = [
            # Hyphenated technical terms (free-verb, feed-back, etc.)
            (r'\b([a-z]+-[a-z]+(?:-[a-z]+)*)\b', lambda m: m.group(1).lower()),
            # JUCE class names (AudioProcessor, dsp::Processor, etc.)
            (r'\b(Juce|JUCE)::(\w+)', lambda m: f"JUCE::{m.group(2)}"),
            (r'\bjuce::(\w+)', lambda m: f"juce::{m.group(1)}"),
            # STL containers and algorithms
            (r'\bstd::(\w+)', lambda m: f"std::{m.group(1)}"),
            # Acronyms (FFT, IIR, etc.)
            (r'\b([A-Z]{2,})\b', lambda m: m.group(1)),
            # Numbers with units (1st order, 2nd order, etc.)
            (r'\b(\d+(?:st|nd|rd|th)\s+order)\b', lambda m: m.group(1).lower()),
            # Technical compound terms
            (r'\b([a-z]+\s+(?:filter|delay|reverb|compressor|envelope|oscillator|buffer|queue|thread|lock)\b)',
             lambda m: m.group(1).lower()),
        ]

        for pattern, extractor in patterns:
            try:
                for match in re.finditer(pattern, content):
                    term = extractor(match)
                    if term and len(term) > 2:
                        # Filter common words
                        word_part = term.split()[0] if ' ' in term else term
                        if word_part.lower() not in COMMON_WORDS:
                            terms.add(term)
            except Exception:
                pass

        # Extract URLs
        url_pattern = r'https?://[^\s<>"\'()\[\]]+'
        for match in re.finditer(url_pattern, content):
            url = match.group(0)
            # Clean URL
            url = url.rstrip('.,;:!?)')
            urls.add(url)

        return list(terms), list(urls)

    def filter_urls_by_priority(self, urls: list, domain_info: dict) -> list:
        """Filter and prioritize URLs by domain relevance."""
        high_priority = []
        medium_priority = []
        low_priority = []

        for url in urls:
            parsed = urlparse(url)
            domain = parsed.netloc.lower()

            # Check if domain matches our priority domains
            if any(d in domain for d in domain_info["domains"]):
                high_priority.append(url)
            elif any(d in domain for d in ["github.com", "stackoverflow.com", "stackexchange.com"]):
                medium_priority.append(url)
            else:
                # Check for relevant keywords in URL
                relevant_keywords = [
                    "audio", "dsp", "juce", "plugin", "filter", "reverb",
                    "compressor", "midi", "synth", "effect", "tutorial",
                    "implementation", "algorithm", "code"
                ]
                if any(kw in url.lower() for kw in relevant_keywords):
                    medium_priority.append(url)
                else:
                    low_priority.append(url)

        # Combine in priority order
        return high_priority + medium_priority + low_priority

    def save_raw_result(self, result: dict, query: str, source: str,
                        crawl_id: str = None) -> Path:
        """Save raw result with full metadata."""
        timestamp = datetime.now(timezone.utc).strftime("%Y%m%d-%H%M%S")
        safe_query = re.sub(r'[^\w-]', '_', query)[:40]
        crawl_suffix = f"_{crawl_id[:8]}" if crawl_id else ""

        raw_data = {
            "harvest_metadata": {
                "kb_type": self.kb_type,
                "topic": self.topic,
                "filename": self.filename,
                "query": query,
                "source": source,
                "timestamp": timestamp,
                "crawl_id": crawl_id
            },
            "credits": self.credits.to_dict(),
            "result": result
        }

        filename = f"{self.kb_type}_{self.topic}_{safe_query}{crawl_suffix}_{timestamp}.json"
        raw_file = RAW_OUTPUT_DIR / filename

        RAW_OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        with open(raw_file, 'w') as f:
            json.dump(raw_data, f, indent=2, default=str)

        print(f"    Saved: {filename}")
        return raw_file

    def save_discovered_terms(self):
        """Save discovered terms for future harvesting."""
        terms_file = TERMS_DIR / f"{self.kb_type}_{self.topic}_{self.filename.replace('.json', '')}_terms.json"

        data = {
            "source_file": str(self.placeholder_path),
            "seed_terms": self.seed_terms,
            "discovered_terms": sorted(list(self.discovered_terms)),
            "crawled_urls": sorted(list(self.crawled_urls)),
            "credits_used": self.credits.credits_used,
            "timestamp": datetime.now(timezone.utc).isoformat()
        }

        TERMS_DIR.mkdir(parents=True, exist_ok=True)
        with open(terms_file, 'w') as f:
            json.dump(data, f, indent=2)

        print(f"    Saved terms: {terms_file.name}")

    def process_search_results(self, result: dict) -> list:
        """Process search results and extract data."""
        items = []

        # Handle both possible response formats
        data = result.get("data", result.get("results", []))

        if not isinstance(data, list):
            return items

        for item in data:
            if not isinstance(item, dict):
                continue

            markdown = item.get("markdown", "")
            metadata = item.get("metadata", {})
            url = item.get("url", metadata.get("sourceURL", metadata.get("url", "")))

            if markdown and url:
                terms, urls = self.extract_terms(markdown)
                self.discovered_terms.update(terms)
                self.crawled_urls.add(url)

                items.append({
                    "url": url,
                    "title": metadata.get("title", ""),
                    "description": metadata.get("description", "")[:200],
                    "markdown_length": len(markdown),
                    "terms_found": len(terms)
                })

        return items

    def _harvest_internal(self, max_queries: int = 5, deep_crawl: bool = True,
                max_crawls: int = 3, resume: bool = True) -> dict:
        """
        Internal harvesting method with checkpoint support.

        Args:
            max_queries: Maximum search queries to run
            deep_crawl: Whether to crawl discovered URLs
            max_crawls: Maximum number of deep crawls
            resume: Whether to resume from checkpoint
        """
        print(f"\n{'=' * 60}")
        print(f"Harvesting: {self.kb_type}/{self.topic}/{self.filename}")
        print(f"Title: {self.title}")
        print(f"Seed terms: {self.seed_terms[:3]}{'...' if len(self.seed_terms) > 3 else ''}")
        print(f"{'=' * 60}")

        domain_info = self.get_domain_priorities()
        all_queries = self.generate_search_queries()

        # Load checkpoint if resuming
        if resume and self.checkpoint.load():
            print(f"Resuming from checkpoint (phase: {self.checkpoint.data['phase']})")
            self.discovered_terms = set(self.checkpoint.data.get("discovered_terms", []))
            self.crawled_urls = set(self.checkpoint.data.get("urls_crawled", []))
            self.credits.credits_used = self.checkpoint.data.get("credits_used", 0)

        # Get remaining queries
        remaining_queries = self.checkpoint.get_remaining_queries(all_queries)[:max_queries]
        all_results = []

        # Phase 1: Search with generated queries
        print(f"\n[Phase 1: Search] Running {len(remaining_queries)} queries...")

        for i, query in enumerate(remaining_queries):
            print(f"\n  Query {i + 1}/{len(remaining_queries)}: {query[:60]}{'...' if len(query) > 60 else ''}")

            result = self.client.search(query, domain_info["domains"])

            if "error" in result:
                print(f"    Error: {result['error']}")
                continue

            if result.get("success") or result.get("data"):
                # Count results
                data = result.get("data", result.get("results", []))
                results_count = len(data) if isinstance(data, list) else 0
                self.credits.add_search(query, results_count)

                # Save raw result
                self.save_raw_result(result, query, "search")

                # Process results
                items = self.process_search_results(result)
                all_results.extend(items)

                print(f"    Found {len(items)} results, {len(self.discovered_terms)} terms")

                # Mark checkpoint
                self.checkpoint.mark_query_done(query)
                self.checkpoint.add_terms(list(self.discovered_terms))
                self.checkpoint.update_credits(self.credits.credits_used)

            time.sleep(FIRECRAWL_DELAY)

        # Phase 2: Deep crawl discovered URLs
        if deep_crawl and self.crawled_urls:
            print(f"\n[Phase 2: Deep Crawl] {len(self.crawled_urls)} URLs discovered...")

            # Filter and prioritize URLs
            prioritized_urls = self.filter_urls_by_priority(
                list(self.crawled_urls), domain_info
            )[:5]  # Top 5 most relevant

            # Get remaining URLs to crawl
            remaining_urls = self.checkpoint.get_remaining_urls(prioritized_urls)[:max_crawls]

            for i, url in enumerate(remaining_urls):
                print(f"\n  Crawl {i + 1}/{len(remaining_urls)}: {url[:60]}{'...' if len(url) > 60 else ''}")

                # Start crawl
                crawl_start = self.client.start_crawl(url, max_depth=2)

                if "error" in crawl_start:
                    print(f"    Failed to start: {crawl_start['error']}")
                    continue

                crawl_id = crawl_start.get("id")
                if not crawl_id:
                    print(f"    No crawl ID returned")
                    continue

                print(f"    Crawl ID: {crawl_id[:8]}...")

                # Wait for completion
                def on_status(status, pages):
                    print(f"    Status: {status}, Pages: {pages}")

                crawl_result = self.client.wait_for_crawl(
                    crawl_id,
                    max_wait=300,
                    status_callback=on_status
                )

                if "error" in crawl_result:
                    print(f"    Crawl error: {crawl_result['error']}")
                    continue

                # Process crawled pages
                data = crawl_result.get("data", [])
                if isinstance(data, list):
                    self.credits.add_crawl(url, len(data))
                    self.save_raw_result(crawl_result, url, "crawl", crawl_id)

                    for page in data:
                        if isinstance(page, dict):
                            markdown = page.get("markdown", "")
                            page_url = page.get("url", "")

                            terms, _ = self.extract_terms(markdown)
                            self.discovered_terms.update(terms)

                            all_results.append({
                                "query": f"crawl:{url}",
                                "url": page_url,
                                "title": page.get("metadata", {}).get("title", ""),
                                "markdown_length": len(markdown),
                                "crawl_source": url
                            })

                    self.checkpoint.mark_url_crawled(url)
                    print(f"    Crawled {len(data)} pages, {len(self.discovered_terms)} total terms")

                time.sleep(FIRECRAWL_DELAY * 2)

        # Save discovered terms
        self.save_discovered_terms()

        # Mark complete
        self.checkpoint.complete()

        # Generate summary
        summary = {
            "kb_type": self.kb_type,
            "topic": self.topic,
            "filename": self.filename,
            "title": self.title,
            "seed_terms": self.seed_terms,
            "discovered_terms": sorted(list(self.discovered_terms))[:100],
            "total_results": len(all_results),
            "total_urls_crawled": len(self.crawled_urls),
            "credits_used": self.credits.credits_used,
            "checkpoint": str(self.checkpoint.checkpoint_file),
            "results": all_results
        }

        # Save manifest
        manifest_file = MANIFEST_DIR / f"{self.kb_type}_{self.topic}_{self.filename}"
        MANIFEST_DIR.mkdir(parents=True, exist_ok=True)
        with open(manifest_file.with_suffix(".json"), 'w') as f:
            json.dump(summary, f, indent=2)

        print(f"\n{'=' * 60}")
        print(f"Harvest Complete:")
        print(f"  Queries: {len(remaining_queries)}")
        print(f"  Results: {len(all_results)}")
        print(f"  URLs crawled: {len(self.crawled_urls)}")
        print(f"  Terms discovered: {len(self.discovered_terms)}")
        print(f"  Credits used: {self.credits.credits_used}")
        print(f"{'=' * 60}\n")

        return summary

    def harvest(self, max_queries: int = 5, deep_crawl: bool = True,
                max_crawls: int = 3, resume: bool = True) -> dict:
        """
        Main harvesting method with file locking.

        Acquires a lock before harvesting to prevent concurrent access.
        Releases lock in finally block even on error.

        Args:
            max_queries: Maximum search queries to run
            deep_crawl: Whether to crawl discovered URLs
            max_crawls: Maximum number of deep crawls
            resume: Whether to resume from checkpoint

        Returns:
            Harvest summary dict, or error dict if lock failed
        """
        resource_name = f"{self.kb_type}_{self.topic}_{self.filename}"
        lock_path, fd = acquire_lock(resource_name)

        if lock_path is None:
            return {
                "error": "Lock acquisition failed",
                "message": f"Resource {resource_name} is currently being harvested by another process",
                "kb_type": self.kb_type,
                "topic": self.topic,
                "filename": self.filename
            }

        try:
            return self._harvest_internal(
                max_queries=max_queries,
                deep_crawl=deep_crawl,
                max_crawls=max_crawls,
                resume=resume
            )
        finally:
            release_lock(lock_path, fd)


def harvest_topic(kb_type: str, topic: str, filename: str, **kwargs) -> dict:
    """Convenience function to harvest a single topic."""
    harvester = DeepHarvester(kb_type, topic, filename)
    return harvester.harvest(**kwargs)


def harvest_kb(kb_type: str, topics: list = None, max_per_topic: int = 3,
               max_queries: int = 5, deep_crawl: bool = True,
               max_crawls: int = 2) -> dict:
    """
    Harvest multiple topics from a KB.

    Args:
        kb_type: Knowledge base type (e.g., "dsp-kb")
        topics: List of topic folders (None = all)
        max_per_topic: Max files to harvest per topic
        max_queries: Max search queries per file
        deep_crawl: Whether to deep crawl URLs
        max_crawls: Max deep crawls per file
    """
    results = []

    kb_path = KB_ROOT / kb_type
    if not kb_path.exists():
        return {"error": f"KB not found: {kb_type}"}

    # Get topic folders
    if topics:
        topic_folders = [kb_path / t for t in topics]
    else:
        topic_folders = [d for d in kb_path.iterdir() if d.is_dir()]

    for topic_folder in topic_folders:
        if not topic_folder.exists():
            continue

        topic_name = topic_folder.name

        # Get placeholder files (skip index.json, manifest.json)
        placeholders = [
            f for f in topic_folder.glob("*.json")
            if f.name not in ["index.json", "manifest.json", "validation.json"]
        ]

        for placeholder in placeholders[:max_per_topic]:
            try:
                result = harvest_topic(
                    kb_type, topic_name, placeholder.name,
                    max_queries=max_queries,
                    deep_crawl=deep_crawl,
                    max_crawls=max_crawls
                )
                results.append(result)

                # Delay between files
                time.sleep(FIRECRAWL_DELAY * 3)

            except FileNotFoundError as e:
                print(f"Skipping: {e}")
                continue
            except Exception as e:
                print(f"Error harvesting {placeholder}: {e}")
                continue

    return {
        "kb_type": kb_type,
        "topics_harvested": len(topic_folders),
        "total_files": len(results),
        "total_credits_used": sum(r.get("credits_used", 0) for r in results),
        "results": results
    }


def main():
    """CLI interface."""
    import argparse

    parser = argparse.ArgumentParser(
        description="Deep harvest KB content with automatic term discovery",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Harvest single file
  python3 harvest-deep.py dsp-kb --topic reverb --file algorithmic-reverb.json

  # Harvest entire topic (max 3 files)
  python3 harvest-deep.py dsp-kb --topic reverb

  # Harvest with limits
  python3 harvest-deep.py juce-kb --topic realtime --max-queries 3 --max-crawls 1

  # Harvest without deep crawling
  python3 harvest-deep.py testing-kb --topic validation --no-crawl

  # Start fresh (ignore checkpoint)
  python3 harvest-deep.py dsp-kb --topic reverb --file algorithmic-reverb.json --no-resume
        """
    )

    parser.add_argument("kb", help="Knowledge base (e.g., dsp-kb, juce-kb, midi-kb)")
    parser.add_argument("--topic", "-t", help="Specific topic folder")
    parser.add_argument("--file", "-f", help="Specific file within topic")
    parser.add_argument("--max-queries", "-q", type=int, default=5,
                        help="Max search queries per file (default: 5)")
    parser.add_argument("--max-crawls", "-c", type=int, default=3,
                        help="Max deep crawls per file (default: 3)")
    parser.add_argument("--max-per-topic", "-m", type=int, default=3,
                        help="Max files per topic (default: 3)")
    parser.add_argument("--no-crawl", action="store_true",
                        help="Skip deep crawling")
    parser.add_argument("--no-resume", action="store_true",
                        help="Start fresh, ignore checkpoint")

    args = parser.parse_args()

    # Validate environment
    if not get_api_key():
        print("Error: FIRECRAWL_API_KEY not set")
        print("Set it with: export FIRECRAWL_API_KEY=your_key")
        sys.exit(1)

    # Create output directories
    for d in [RAW_OUTPUT_DIR, PROCESSED_DIR, MANIFEST_DIR, TERMS_DIR, CHECKPOINT_DIR]:
        d.mkdir(parents=True, exist_ok=True)

    # Change to KB root for relative paths
    os.chdir(KB_ROOT)

    try:
        if args.file:
            if not args.topic:
                print("Error: --topic required when --file specified")
                sys.exit(1)

            result = harvest_topic(
                args.kb, args.topic, args.file,
                max_queries=args.max_queries,
                deep_crawl=not args.no_crawl,
                max_crawls=args.max_crawls,
                resume=not args.no_resume
            )

        elif args.topic:
            result = harvest_kb(
                args.kb,
                topics=[args.topic],
                max_per_topic=args.max_per_topic,
                max_queries=args.max_queries,
                deep_crawl=not args.no_crawl,
                max_crawls=args.max_crawls
            )

        else:
            result = harvest_kb(
                args.kb,
                max_per_topic=args.max_per_topic,
                max_queries=args.max_queries,
                deep_crawl=not args.no_crawl,
                max_crawls=args.max_crawls
            )

        print("\n" + json.dumps(result, indent=2, default=str))

    except KeyboardInterrupt:
        print("\n\nInterrupted. Progress saved to checkpoint.")
        sys.exit(1)
    except Exception as e:
        print(f"\nError: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()