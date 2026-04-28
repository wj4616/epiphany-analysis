#!/usr/bin/env python3
"""
Generate placeholder JSON files for KB topics.
Usage: python3 scripts/create-kb-template.py <kb> <folder> <filename> <title> <description> <audio_relevance>
"""

import json
import sys
import os
from datetime import datetime, timezone

def create_template(kb, folder, filename, title, description, audio_relevance, search_terms, sources):
    """Create a KB JSON template."""
    template = {
        "title": title,
        "description": description,
        "source": "curated",
        "harvested_at": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
        "kb_type": kb.replace("-kb", ""),
        "audio_relevance": audio_relevance,
        "difficulty": "intermediate",
        "markdown": f"# {title}\n\n[Content to be harvested]\n\n## Overview\n\n[Overview content]\n\n## Key Concepts\n\n- [Concept 1]\n- [Concept 2]\n\n## Implementation\n\n```cpp\n// Example code placeholder\n```\n\n## See Also\n\n- [Related topic]\n",
        "implementation_examples": [],
        "tags": [],
        "related_topics": [],
        "search_terms": search_terms,
        "sources": sources
    }
    return template

def main():
    if len(sys.argv) < 7:
        print("Usage: python3 create-kb-template.py <kb> <folder> <filename> <title> <description> <audio_relevance>")
        print("       search_terms and sources read from stdin as JSON")
        sys.exit(1)

    kb = sys.argv[1]
    folder = sys.argv[2]
    filename = sys.argv[3]
    title = sys.argv[4]
    description = sys.argv[5]
    audio_relevance = int(sys.argv[6])

    # Read search_terms and sources from stdin
    extra = json.load(sys.stdin) if not sys.stdin.isatty() else {}
    search_terms = extra.get("search_terms", [])
    sources = extra.get("sources", [])

    template = create_template(kb, folder, filename, title, description, audio_relevance, search_terms, sources)

    output_path = f"{kb}/{folder}/{filename}"
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    with open(output_path, 'w') as f:
        json.dump(template, f, indent=2)

    print(f"Created: {output_path}")

if __name__ == "__main__":
    main()