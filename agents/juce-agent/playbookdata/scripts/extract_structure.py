#!/usr/bin/env python3
"""
Extract semantic structure from markdown content.

Parses harvested markdown into structured semantic fields:
- summary: First paragraph or abstract
- concepts: Key technical concepts with descriptions
- code_blocks: Code examples with language and preserved whitespace
- references: URLs and citations
"""

import re
import sys
from pathlib import Path
from typing import List, Dict
from urllib.parse import urlparse

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))


def extract_code_blocks(markdown: str) -> List[Dict]:
    """
    Extract code blocks from markdown, preserving exact whitespace.

    Args:
        markdown: Markdown content

    Returns:
        List of code block dicts with language and code
    """
    blocks = []

    # Match fenced code blocks: ```language\ncode\n```
    pattern = r'```(\w*)\n(.*?)```'

    for match in re.finditer(pattern, markdown, re.DOTALL):
        language = match.group(1) or "text"
        code = match.group(2)

        # Preserve exact whitespace - do not strip
        blocks.append({
            "language": language,
            "code": code,
            "preserved": True
        })

    return blocks


def extract_concepts(markdown: str) -> List[Dict]:
    """
    Extract concepts from markdown.

    Looks for:
    - **Term**: Description patterns
    - Definition lists
    - Header-based sections

    Args:
        markdown: Markdown content

    Returns:
        List of concept dicts with name and description
    """
    concepts = []

    # Pattern: **Term**: Description
    pattern = r'\*\*([^*]+)\*\*[：:]\s*([^\n]+)'
    for match in re.finditer(pattern, markdown):
        name = match.group(1).strip()
        description = match.group(2).strip()

        concepts.append({
            "name": name,
            "description": description,
            "related": []
        })

    # Pattern: - **Term** (list items)
    list_pattern = r'-\s*\*\*([^*]+)\*\*'
    for match in re.finditer(list_pattern, markdown):
        name = match.group(1).strip()

        # Avoid duplicates
        if not any(c["name"] == name for c in concepts):
            # Get rest of line as description
            start = match.end()
            rest = markdown[start:markdown.find('\n', start)]

            concepts.append({
                "name": name,
                "description": rest.strip('：: ').strip() or f"A {name.lower()} concept",
                "related": []
            })

    return concepts


def extract_references(markdown: str) -> List[Dict]:
    """
    Extract references/URLs from markdown.

    Args:
        markdown: Markdown content

    Returns:
        List of reference dicts with url, title, domain
    """
    references = []
    seen_urls = set()

    # Match [Title](URL) format
    pattern = r'\[([^\]]+)\]\(([^)]+)\)'

    for match in re.finditer(pattern, markdown):
        title = match.group(1).strip()
        url = match.group(2).strip()

        if url in seen_urls:
            continue
        seen_urls.add(url)

        # Skip internal anchors
        if url.startswith('#'):
            continue

        # Extract domain
        try:
            domain = urlparse(url).netloc
        except:
            domain = ""

        references.append({
            "title": title,
            "url": url,
            "domain": domain
        })

    return references


def extract_summary(markdown: str) -> str:
    """
    Extract summary from markdown.

    Uses first paragraph after any headers.

    Args:
        markdown: Markdown content

    Returns:
        Summary string
    """
    lines = markdown.strip().split('\n')

    # Skip headers (lines starting with #)
    content_lines = []
    in_code_block = False

    for line in lines:
        # Track code blocks
        if line.startswith('```'):
            in_code_block = not in_code_block
            continue

        if in_code_block:
            continue

        # Skip headers
        if line.startswith('#'):
            continue

        # Skip empty lines at start
        if not content_lines and not line.strip():
            continue

        # Stop at next header
        if line.startswith('#') and content_lines:
            break

        content_lines.append(line)

        # Stop after first paragraph
        if not line.strip() and content_lines:
            break

    summary = '\n'.join(content_lines).strip()

    # Limit to reasonable length
    if len(summary) > 500:
        summary = summary[:500].rsplit(' ', 1)[0] + '...'

    return summary


def extract_semantic_fields(markdown: str) -> Dict:
    """
    Extract all semantic fields from markdown.

    Args:
        markdown: Markdown content (or dict with markdown field)

    Returns:
        Dict with summary, concepts, code_blocks, references
    """
    if isinstance(markdown, dict):
        markdown = markdown.get("markdown", "")

    if not markdown:
        return {
            "summary": "",
            "concepts": [],
            "code_blocks": [],
            "references": []
        }

    return {
        "summary": extract_summary(markdown),
        "concepts": extract_concepts(markdown),
        "code_blocks": extract_code_blocks(markdown),
        "references": extract_references(markdown)
    }


if __name__ == "__main__":
    import json
    import sys

    if len(sys.argv) < 2:
        print("Usage: extract_structure.py <markdown_file>")
        sys.exit(1)

    file_path = sys.argv[1]
    content = open(file_path).read()

    result = extract_semantic_fields(content)
    print(json.dumps(result, indent=2))