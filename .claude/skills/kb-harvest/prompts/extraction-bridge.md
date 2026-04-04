ADDITIONALLY — Bridge Detection:
From this content, extract any mappings between sound descriptions or
subjective terms and specific DSP parameter settings. For each found:
- descriptor: the subjective quality (warm, bright, punchy, etc.)
- parameter: which DSP parameter (filter_cutoff, resonance, etc.)
- value_range: [min, max] recommended range with units
- typical_default: common starting value
- rationale: why this mapping works
- anti_patterns: what NOT to do with this mapping

Only extract EXPLICIT mappings with specific parameter values mentioned
in the source text. Do NOT invent mappings from general descriptions.

Return bridge mappings as a separate "bridge_detections" array in the JSON.