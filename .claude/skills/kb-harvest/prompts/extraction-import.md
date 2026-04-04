You are importing content from an external knowledge base into our KB system.

SOURCE FORMAT: [format]
TARGET: "[kb-layer]" layer of "[kb-name]"

The source content below is from a file at [source-path].
[If field_map provided: Field mapping: [field_map_summary]]

Extract all fields per the entry schema. For fields that have a direct
mapping in the source, copy the value exactly (method: "direct-mapped").
For fields that must be derived from the content, synthesize them
(method: "ai-synthesized"). For fields that require estimation, infer
them (method: "ai-inferred").

Preserve ALL code blocks with exact original formatting.

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

Return as JSON with this structure (same as extraction-standard.md):
```json
{
  "fields": { "title": "...", "description": "...", ... },
  "field_provenance": {
    "title": { "confidence": 0.95, "method": "direct-mapped" },
    "description": { "confidence": 0.85, "method": "direct-mapped" },
    "concepts": { "confidence": 0.75, "method": "ai-synthesized" }
  },
  "bridge_detections": []
}
```

Method values: "direct-mapped" (1:1 from format adapter), "direct-extracted"
(verbatim from source), "ai-synthesized" (generated from content), "ai-inferred" (estimated).