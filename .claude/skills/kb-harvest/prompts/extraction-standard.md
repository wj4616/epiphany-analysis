You are extracting structured knowledge from web content for a Knowledge Base.

TARGET: KB entry for "[topic]" in the "[kb-layer]" layer of "[kb-name]"

SOURCE METADATA (auto-populated, do not extract):
- source.reference: will be set to page title or first <h1>
- source.url: will be set to the fetched URL
- source.backend: will be set to the backend used
- source.retrieved_date: will be set to today's date

ENTRY SCHEMA FIELDS TO EXTRACT:
1. title: Clear, descriptive title for this knowledge entry
2. summary: One line, max 100 characters
3. description: Substantive content, minimum 100 characters. Include specific
   parameter ranges, implementation details, and practical guidance.
4. concepts: Key concepts as [{name, description, related[]}]. Minimum 2.
5. code_blocks: ALL code examples from the source.
   CRITICAL: Preserve code blocks EXACTLY as they appear — do not reformat,
   fix, simplify, or modify any code. Copy verbatim including whitespace,
   indentation, and comments. Include language tag and description for each.
6. tags: Searchable keywords. Minimum 3.
7. difficulty: beginner | intermediate | advanced
8. domain_relevance: 1-10 for [domain description]
9. related_topics: Other topics this content connects to
10. cross_references: If content explicitly references concepts from other
    domains (e.g., DSP concepts in a sound design article), note them.

For each field, also provide confidence and method metadata.

Return as JSON with this structure:
```json
{
  "fields": {
    "title": "...",
    "summary": "...",
    "description": "...",
    "concepts": [...],
    "code_blocks": [...],
    "tags": [...],
    "difficulty": "...",
    "domain_relevance": 8,
    "related_topics": [...],
    "cross_references": [...]
  },
  "field_provenance": {
    "title": { "confidence": 0.9, "method": "direct-extracted" },
    "description": { "confidence": 0.8, "method": "ai-synthesized" },
    "code_blocks": { "confidence": 0.95, "method": "direct-extracted" },
    "difficulty": { "confidence": 0.6, "method": "ai-inferred" }
  },
  "bridge_detections": []
}
```

Method values: "direct-extracted" (verbatim from source), "ai-synthesized"
(generated from source content), or "ai-inferred" (estimated with limited basis).
`fields` contains entry schema values. `field_provenance` maps to harvest_metadata.
`bridge_detections` is empty unless bridge detection prompt is appended.