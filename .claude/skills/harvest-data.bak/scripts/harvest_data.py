#!/usr/bin/env python3
"""Harvest web data into KB with quality filtering."""

import argparse
import importlib.util
import json
import os
import sys
from datetime import datetime, timezone
from pathlib import Path
import hashlib

# Find playbookdata directory
# Script is at: ~/.claude/skills/harvest-data/scripts/harvest_data.py
# Playbookdata is at: ~/.agents/juce-agent/playbookdata/ (configurable)
SCRIPT_DIR = Path(__file__).parent.resolve()

# Try multiple possible locations for playbookdata
POSSIBLE_PATHS = [
    Path(__file__).parent.parent.parent / "playbookdata",  # skills/../playbookdata
    Path.home() / "agents" / "juce-agent" / "playbookdata",  # ~/agents/juce-agent/playbookdata
    Path.home() / "agents" / "juce-agent" / "playbooks" / ".." / "playbookdata",  # Alternative structure
]

PLAYBOOKDATA = None
for path in POSSIBLE_PATHS:
    if path.exists():
        PLAYBOOKDATA = path
        break

if PLAYBOOKDATA is None:
    print("ERROR: Could not find playbookdata directory")
    print("Searched:")
    for path in POSSIBLE_PATHS:
        print(f"  {path}")
    sys.exit(1)

HARVESTED_RAW = PLAYBOOKDATA / "harvested" / "raw"
HARVESTED_MANIFESTS = PLAYBOOKDATA / "harvested" / "manifests"
KB_ROOT = PLAYBOOKDATA  # For KB directory access

# Import from existing harvest-deep.py (hyphenated filename requires importlib)
HARVEST_DEEP = PLAYBOOKDATA / "scripts" / "harvest-deep.py"
if not HARVEST_DEEP.exists():
    print(f"ERROR: harvest-deep.py not found at {HARVEST_DEEP}")
    sys.exit(1)

spec = importlib.util.spec_from_file_location("harvest_deep", HARVEST_DEEP)
harvest_deep = importlib.util.module_from_spec(spec)
spec.loader.exec_module(harvest_deep)

# Access exports
FirecrawlClient = harvest_deep.FirecrawlClient
get_api_key = harvest_deep.get_api_key

# Quality thresholds
QUALITY_THRESHOLDS = {
    "min_content_length": 500,
    "min_relevance_score": 0.3,
    "max_noise_ratio": 0.5,
}

# Source domain weighting for relevance
SOURCE_WEIGHTS = {
    ".edu": 1.3,
    "scholar.google.com": 1.5,
    "arxiv.org": 1.4,
    "acm.org": 1.4,
    "ieee.org": 1.4,
    "github.com": 1.2,
    "stackoverflow.com": 1.0,
    "juce.com": 1.3,
    "forum.juce.com": 1.2,
    "medium.com": 0.9,
    "blogspot.com": 0.8,
}

# KB keyword mapping
KB_KEYWORDS = {
    "dsp-kb": ["reverb", "filter", "delay", "fft", "dsp", "audio",
               "convolution", "oscillator", "envelope", "dynamics",
               "compressor", "limiter", "eq", "equalizer", "mixing",
               "sampling", "interpolation", "wavetable", "synthesis"],
    "juce-kb": ["juce", "audioprocessor", "editor", "component",
                "realtime", "thread", "midiinput", "audiobuffer",
                "plugin", "vst", "au", "standalone"],
    "midi-kb": ["midi", "note", "controller", "mpe", "cc", "sysex",
                "pitchbend", "aftertouch", "clock", "sync"],
    "testing-kb": ["test", "mock", "assert", "tdd", "unit test",
                   "integration test", "catch2", "googletest"],
    "sound-design-kb": ["synthesis", "preset", "sound design", "timbre",
                        "waveform", "modulation", "lfo", "adsr"],
    "ui-kb": ["ui", "gui", "widget", "lookandfeel", "component",
              "slider", "button", "combobox", "editor"],
    "cmake-kb": ["cmake", "build", "makefile", "target", "fetchcontent",
                 "findpackage", "toolchain"],
    "cpp-kb": ["c++", "template", "lambda", "smart pointer", "move",
               "constexpr", "raii", "stl", "memory"],
}

# Domain terms for relevance expansion
DOMAIN_TERMS = {
    "reverb": ["audio", "signal", "processing", "impulse", "response",
               "convolution", "reflection", "decay", "room"],
    "filter": ["cutoff", "resonance", "frequency", "iir", "fir",
               "biquad", "lowpass", "highpass", "bandpass"],
    "delay": ["echo", "feedback", "modulation", "flanger", "chorus"],
    "compressor": ["threshold", "ratio", "attack", "release", "gain",
                   "dynamics", "limiter"],
    "oscillator": ["waveform", "frequency", "phase", "sine", "sawtooth",
                   "square", "noise"],
    "envelope": ["adsr", "attack", "decay", "sustain", "release"],
    "juce": ["plugin", "processor", "editor", "audio", "audiobuffer",
             "audioprocessor", "component", "realtime", "thread"],
    "plugin": ["vst", "au", "audiounit", "vst3", "standalone", "wrapper"],
    "midi": ["note", "controller", "cc", "channel", "sysex",
             "pitchbend", "aftertouch", "clock"],
    "mpe": ["multidimensional", "polyphonic", "expression", "slide",
            "press", "glide"],
    "test": ["mock", "assert", "unit", "integration", "fixture",
             "coverage", "catch2"],
    "synthesis": ["waveform", "modulation", "lfo", "envelope",
                  "oscillator", "filter", "amplifier"],
    "preset": ["patch", "sound", "program", "bank", "factory"],
    "ui": ["component", "widget", "slider", "button", "combobox",
           "lookandfeel", "editor"],
    "gui": ["widget", "window", "dialog", "menu", "layout"],
    "cmake": ["target", "build", "makefile", "fetchcontent",
              "findpackage", "toolchain", "install"],
    "c++": ["template", "lambda", "smart pointer", "move",
            "constexpr", "raii", "stl", "memory"],
}

# Noise patterns for quality estimation
NOISE_PATTERNS = [
    "skip to content", "navigation menu", "main menu", "breadcrumb",
    "subscribe to newsletter", "sign up for", "subscribe now",
    "cookie policy", "accept cookies", "privacy policy",
    "log in", "sign in", "create account", "register",
    "advertisement", "sponsored", "ad",
    "all rights reserved", "copyright ©",
]


class InsufficientCreditsError(Exception):
    """Raised when credit budget is exhausted."""
    pass


def determine_kb(topic: str, context: str = "") -> str:
    """Find best-matching KB or suggest new one."""
    topic_lower = topic.lower()
    context_lower = context.lower()

    best_match = None
    best_score = 0

    for kb, keywords in KB_KEYWORDS.items():
        score = sum(1 for kw in keywords if kw in topic_lower or kw in context_lower)
        if score > best_score:
            best_score = score
            best_match = kb

    if best_score >= 2:
        return best_match

    # No good match - suggest new KB name
    return suggest_new_kb_name(topic)


def suggest_new_kb_name(topic: str) -> str:
    """Generate a KB name from topic."""
    stop_words = {"how", "to", "do", "the", "a", "an", "in", "for", "with", "using"}
    words = [w for w in topic.lower().split() if w not in stop_words]

    if words:
        return f"{words[0]}-kb"

    # Fallback to hash-based name
    hash_val = hashlib.md5(topic.encode()).hexdigest()[:6]
    return f"topic-{hash_val}-kb"


def create_placeholder(kb: str, topic: str, filename: str) -> dict:
    """Create a placeholder KB file ready for harvested content."""
    return {
        "kb_type": kb,
        "topic": topic,
        "filename": filename,
        "status": "placeholder",
        "created_at": datetime.now(timezone.utc).isoformat(),
        "title": filename.replace("-", " ").replace(".json", "").title(),
        "summary": "",
        "concepts": [],
        "code_blocks": [],
        "references": []
    }


def create_kb_directory(kb_name: str, topic: str, filename: str) -> Path:
    """Create KB directory structure and placeholder file if needed.

    Returns: Path to the KB file
    """
    kb_dir = KB_ROOT / kb_name
    topic_dir = kb_dir / topic
    kb_file = topic_dir / filename

    # Create directories
    topic_dir.mkdir(parents=True, exist_ok=True)

    # Create placeholder if file doesn't exist
    if not kb_file.exists():
        placeholder = create_placeholder(kb_name, topic, filename)
        kb_file.write_text(json.dumps(placeholder, indent=2))
        print(f"Created placeholder: {kb_file}")

    # Create or update manifest
    manifest_path = kb_dir / "manifest.json"
    if not manifest_path.exists():
        manifest = {
            "kb_name": kb_name,
            "topics": []
        }
        manifest_path.write_text(json.dumps(manifest, indent=2))

    return kb_file


def apply_source_weight(relevance_score: float, url: str) -> float:
    """Apply source domain weighting to relevance score."""
    for domain, weight in SOURCE_WEIGHTS.items():
        if domain in url:
            return relevance_score * weight
    return relevance_score


def calculate_relevance(markdown: str, topic: str) -> float:
    """Score how relevant content is to topic."""
    topic_terms = set(topic.lower().split())

    # Expand terms using domain knowledge (iterate over copy to avoid RuntimeError)
    expanded_terms = set(topic_terms)
    for term in topic_terms:
        if term in DOMAIN_TERMS:
            expanded_terms.update(DOMAIN_TERMS[term])
    topic_terms = expanded_terms

    # Count occurrences
    markdown_lower = markdown.lower()
    matches = sum(1 for term in topic_terms if term in markdown_lower)

    # Normalize by content length (per 1000 chars)
    density = matches / max(len(markdown) / 1000, 0.1)
    return min(1.0, density / 5)  # Scale to 0-1


def estimate_noise(markdown: str) -> float:
    """Estimate ratio of navigation/boilerplate content."""
    lines = markdown.lower().split("\n")
    noise_lines = 0

    for line in lines:
        line = line.strip()
        if not line:
            continue
        for pattern in NOISE_PATTERNS:
            if pattern in line:
                noise_lines += 1
                break

    return noise_lines / max(len(lines), 1)


def assess_quality(result: dict, topic: str) -> tuple:
    """Assess if result meets quality standards.

    Returns: (passed, quality_report)
    """
    markdown = result.get("markdown", "")
    url = result.get("url", "")
    title = result.get("title", "")

    # Apply source weight to relevance
    base_relevance = calculate_relevance(markdown, topic)
    weighted_relevance = apply_source_weight(base_relevance, url)

    report = {
        "url": url,
        "title": title,
        "content_length": len(markdown),
        "has_code": "```" in markdown,
        "code_blocks": markdown.count("```"),
        "relevance_score": weighted_relevance,
        "noise_ratio": estimate_noise(markdown),
        "passed": False,
        "rejection_reason": None
    }

    # Check minimum content
    if report["content_length"] < QUALITY_THRESHOLDS["min_content_length"]:
        report["rejection_reason"] = "content_too_short"
        return False, report

    # Check relevance
    if report["relevance_score"] < QUALITY_THRESHOLDS["min_relevance_score"]:
        report["rejection_reason"] = "not_relevant"
        return False, report

    # Check noise ratio (too much navigation/ads)
    if report["noise_ratio"] > QUALITY_THRESHOLDS["max_noise_ratio"]:
        report["rejection_reason"] = "too_much_noise"
        return False, report

    report["passed"] = True
    return True, report


def generate_queries(topic: str, context: str, expansion_level: int = 0) -> list:
    """Generate search queries based on topic and expansion level."""
    base = f"{topic} {context}".strip()

    if expansion_level == 0:
        return [
            base,
            f"{topic} tutorial",
            f"{topic} implementation",
            f"{topic} guide",
        ]
    elif expansion_level == 1:
        return [
            f"{topic} academic site:edu",
            f"{topic} research paper",
            f"{topic} algorithm explanation",
            f"{topic} documentation",
        ]
    else:
        return [
            f"{topic} mathematics theory",
            f"{topic} practical implementation",
            f"how does {topic} work",
            f"{topic} open source code",
            f"{topic} JUCE implementation",
        ]


def deduplicate_results(results: list) -> list:
    """Remove duplicate URLs across different query results."""
    seen_urls = set()
    unique_results = []

    for result in results:
        url = result.get("url", "")
        if url and url not in seen_urls:
            seen_urls.add(url)
            unique_results.append(result)

    return unique_results


def execute_searches(queries: list, max_results: int = 15) -> tuple:
    """Execute multiple Firecrawl searches and combine results.

    Returns: (results, credits_used)
    """
    key = get_api_key()
    if not key:
        raise ValueError("FIRECRAWL_API_KEY not set")

    api_url = os.environ.get("FIRECRAWL_API_URL", "https://api.firecrawl.dev")
    client = FirecrawlClient(key, api_url)
    all_results = []
    seen_urls = set()
    total_credits = 0

    for query in queries:
        try:
            response = client.search(query, limit=max_results)
            # Track credits: 5 per search + 1 per result
            results_count = len(response.get("data", []))
            total_credits += 5 + results_count

            for item in response.get("data", []):
                url = item.get("url", "")
                if url not in seen_urls:
                    seen_urls.add(url)
                    all_results.append(item)
        except Exception as e:
            print(f"Query '{query}' failed: {e}")
            continue

    return all_results, total_credits


def expand_context(topic: str, current_context: str) -> tuple:
    """Generate expanded context for better search terms.

    Returns: (new_context, prompt_message)
    """
    topic_lower = topic.lower()

    if topic_lower in DOMAIN_TERMS:
        related = DOMAIN_TERMS[topic_lower]
        new_context = f"{current_context} {' '.join(related[:5])}".strip()
        return new_context, ""

    prompt = (
        f"No quality results found for '{topic}'.\n"
        f"Current context: {current_context or 'none'}\n"
        f"Provide more specific context (or 'skip' to give up): "
    )
    return current_context, prompt


def store_results(results: list, topic: str, kb: str, output_dir: Path) -> dict:
    """Store quality-filtered results to harvested/raw/."""
    stored = 0
    rejected = 0
    stored_results = []
    rejection_reasons = {}

    for result in results:
        passed, report = assess_quality(result, topic)

        if passed:
            stored += 1
            stored_results.append({
                "url": result.get("url", ""),
                "title": result.get("title", ""),
                "markdown": result.get("markdown", ""),
                "quality_report": report
            })
        else:
            rejected += 1
            reason = report.get("rejection_reason", "unknown")
            rejection_reasons[reason] = rejection_reasons.get(reason, 0) + 1

    # Generate filename
    timestamp = datetime.now(timezone.utc).strftime("%Y%m%d-%H%M%S")
    safe_topic = topic.replace(" ", "_").replace("/", "_")
    filename = f"{kb}_{safe_topic}_{timestamp}.json"

    # Ensure output directory exists
    output_dir.mkdir(parents=True, exist_ok=True)

    # Write to file
    output_path = output_dir / filename
    output_path.write_text(json.dumps({
        "harvest_metadata": {
            "kb_type": kb,
            "topic": topic,
            "harvested_at": datetime.now(timezone.utc).isoformat(),
            "quality_filter": {
                "passed": stored,
                "rejected": rejected,
                "rejection_reasons": rejection_reasons
            },
            "thresholds": QUALITY_THRESHOLDS
        },
        "result": {
            "data": stored_results
        }
    }, indent=2))

    return {
        "stored": stored,
        "rejected": rejected,
        "rejection_reasons": rejection_reasons,
        "output_file": str(output_path)
    }


def create_manifest(kb: str, topic: str, filename: str, stored_count: int,
                    rejected_count: int, queries: list, credits: int) -> dict:
    """Create manifest file for harvest."""
    manifest_dir = HARVESTED_MANIFESTS
    manifest_dir.mkdir(parents=True, exist_ok=True)

    # Determine target filename in KB
    base_name = topic.replace(" ", "-").lower()
    target_filename = f"{base_name}.json"

    manifest = {
        "kb_name": kb,
        "topic": topic,
        "filename": target_filename,
        "status": "harvested",
        "harvested_at": datetime.now(timezone.utc).isoformat(),
        "sources": stored_count,
        "rejected": rejected_count,
        "credits_used": credits,
        "search_queries": queries,
        "quality_thresholds": QUALITY_THRESHOLDS,
        "notes": "Auto-harvested via harvest-data skill"
    }

    manifest_path = manifest_dir / f"{kb}_{topic.replace(' ', '_')}_{target_filename}"
    manifest_path.write_text(json.dumps(manifest, indent=2))

    return manifest


def track_credits(used: int, budget: int = 2000) -> dict:
    """Track and warn about credit usage."""
    remaining = budget - used

    if remaining < 1000:
        print(f"Warning: Low credits remaining ({remaining})")

    if remaining < 100:
        raise InsufficientCreditsError(f"Only {remaining} credits remaining")

    return {
        "used": used,
        "remaining": remaining,
        "percentage_used": round(used / budget * 100, 1)
    }


def parse_args():
    parser = argparse.ArgumentParser(
        description="Harvest web data into KB with quality filtering",
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument("--topic", help="Topic to harvest (required for harvest)")
    parser.add_argument("--context", default="", help="Additional context for search")
    parser.add_argument("--kb", help="Force specific KB (auto-detect if not specified)")
    parser.add_argument("--new-kb", help="Create new KB with this name")
    parser.add_argument("--dry-run", action="store_true", help="Show queries without executing")
    parser.add_argument("--max-results", type=int, default=15, help="Max results per query")
    parser.add_argument("--min-length", type=int, default=500, help="Minimum content length")
    parser.add_argument("--min-relevance", type=float, default=0.3, help="Minimum relevance score")
    parser.add_argument("--max-noise", type=float, default=0.5, help="Maximum noise ratio")
    parser.add_argument("--credits-budget", type=int, default=2000, help="Credit limit")
    parser.add_argument("--phase", type=int, help="Harvest phase (for batch mode)")

    args = parser.parse_args()

    # Show help if no topic and not dry-run
    if not args.topic and not args.dry_run:
        parser.print_help()
        sys.exit(0)

    return args


def harvest(topic: str, context: str = "", kb: str = None,
            new_kb: str = None, dry_run: bool = False,
            max_results: int = 15, credits_budget: int = 2000) -> dict:
    """Execute harvest for a topic."""
    # Determine KB
    if kb:
        target_kb = kb
    elif new_kb:
        target_kb = new_kb
    else:
        target_kb = determine_kb(topic, context)

    print(f"Target KB: {target_kb}")

    # Generate queries
    all_queries = []
    for level in range(3):
        queries = generate_queries(topic, context, expansion_level=level)
        all_queries.extend(queries)

    print(f"Generated {len(all_queries)} search queries")

    if dry_run:
        print("\nQueries (dry-run):")
        for i, q in enumerate(all_queries[:12], 1):
            print(f"  {i}. {q}")
        return {"status": "dry_run", "queries": all_queries[:12], "kb": target_kb}

    # Track credits
    total_credits = 0

    # Execute searches
    print("\nExecuting searches...")
    results, credits_used = execute_searches(all_queries[:4], max_results)
    total_credits += credits_used

    if not results:
        # Try expanded queries
        print("No results from primary queries, trying expanded...")
        results, credits_used = execute_searches(all_queries[4:8], max_results)
        total_credits += credits_used

    # Track credits and warn if needed
    credit_info = track_credits(total_credits, credits_budget)
    print(f"Credits used: {credit_info['used']} ({credit_info['percentage_used']}% of budget)")

    # Deduplicate
    results = deduplicate_results(results)
    print(f"Found {len(results)} unique results")

    # Store with quality filtering
    stored = store_results(results, topic, target_kb, HARVESTED_RAW)

    print(f"\nHarvest complete:")
    print(f"  Stored: {stored['stored']} results")
    print(f"  Rejected: {stored['rejected']} results")
    if stored['rejection_reasons']:
        print("  Rejection reasons:")
        for reason, count in stored['rejection_reasons'].items():
            print(f"    - {reason}: {count}")

    # Create manifest
    if stored['stored'] > 0:
        create_manifest(target_kb, topic, f"{topic.replace(' ', '-')}.json",
                       stored['stored'], stored['rejected'],
                       all_queries[:4], total_credits)

    return {
        **stored,
        "kb": target_kb,
        "credits_used": total_credits
    }


def main():
    args = parse_args()

    # Update thresholds from args
    if args.min_length:
        QUALITY_THRESHOLDS["min_content_length"] = args.min_length
    if args.min_relevance:
        QUALITY_THRESHOLDS["min_relevance_score"] = args.min_relevance
    if args.max_noise:
        QUALITY_THRESHOLDS["max_noise_ratio"] = args.max_noise

    result = harvest(
        topic=args.topic,
        context=args.context,
        kb=args.kb,
        new_kb=args.new_kb,
        dry_run=args.dry_run,
        max_results=args.max_results,
        credits_budget=args.credits_budget
    )

    print(f"\nOutput: {result.get('output_file', 'N/A')}")


if __name__ == "__main__":
    main()