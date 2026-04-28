# tests/test_extract_structure.py
import pytest
import os
import sys
from pathlib import Path

# Add playbookdata/scripts to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from extract_structure import extract_semantic_fields, extract_code_blocks, extract_concepts


def test_extract_semantic_fields_returns_dict():
    """Extract returns semantic dict with required fields."""
    markdown = """# Algorithmic Reverb

This is a summary of algorithmic reverb.

## Concepts

- **Freeverb**: Open-source reverb algorithm
- **Schroeder**: Classic reverb design

## Code Example

```cpp
class CombFilter {
    float process(float input) {
        return buffer[readPos];
    }
};
```

## References

- [Freeverb](https://ccrma.stanford.edu)
"""

    result = extract_semantic_fields(markdown)

    assert "summary" in result
    assert "concepts" in result
    assert "code_blocks" in result
    assert "references" in result

def test_extract_code_blocks_preserves_whitespace():
    """Code blocks preserve exact whitespace."""
    markdown = """```cpp
class Test {
    int x;
    // 4 spaces indent
};
```"""

    blocks = extract_code_blocks(markdown)

    assert len(blocks) == 1
    assert "    int x;" in blocks[0]["code"]
    assert blocks[0]["language"] == "cpp"

def test_extract_concepts_finds_bold_terms():
    """Concepts extraction finds bold terms."""
    markdown = """
- **Freeverb**: An open-source algorithm
- **Schroeder Reverb**: Classic design
"""

    concepts = extract_concepts(markdown)

    assert len(concepts) >= 2
    names = [c["name"] for c in concepts]
    assert "Freeverb" in names
    assert "Schroeder Reverb" in names

def test_extract_semantic_handles_empty():
    """Empty markdown returns empty semantic."""
    result = extract_semantic_fields("")

    assert result["summary"] == ""
    assert result["concepts"] == []

def test_extract_references_from_markdown():
    """References are extracted from markdown links."""
    markdown = """See [Freeverb](https://ccrma.stanford.edu/freeverb) for details.
Also check [JUCE Forum](https://forum.juce.com) for discussions.
"""

    from extract_structure import extract_references
    refs = extract_references(markdown)

    assert len(refs) == 2
    assert refs[0]["title"] == "Freeverb"
    assert refs[0]["domain"] == "ccrma.stanford.edu"