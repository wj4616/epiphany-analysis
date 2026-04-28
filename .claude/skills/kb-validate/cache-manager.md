# Cache Manager

Manages session-scoped cache for validated claims. In-memory only, no disk persistence.

## Cache Structure

```json
{
  "validated_claims": {
    "claim_hash_1": {
      "claim": "JUCE SmoothedValue uses getNextValue() per sample",
      "source": "juce-kb/realtime/smoothedvalue.json",
      "source_completeness": "full",
      "confidence": 0.92,
      "confidence_level": "HIGH",
      "cross_references": ["juce-kb", "session-history-line-45"],
      "validated_at": "2026-03-30T21:00:00Z",
      "validation_type": "auto"
    }
  },
  "partial_reads": {
    "dsp-kb/reverb/convolution-reverb.json": {
      "bytes_read": 5000,
      "total_bytes": 15000,
      "completeness": 0.33
    }
  },
  "source_cache": {
    "juce-kb/realtime/smoothedvalue.json": {
      "last_read": "2026-03-30T21:00:00Z",
      "read_type": "full",
      "content_hash": "abc123"
    }
  }
}
```

## Cache Operations

### Initialize Cache

Create new session cache:

```json
{
  "validated_claims": {},
  "partial_reads": {},
  "source_cache": {},
  "session_start": "2026-03-30T21:00:00Z"
}
```

### Store Validated Claim

```json
{
  "operation": "store",
  "claim_hash": "sha256:abc123...",
  "claim_data": {
    "claim": "string",
    "source": "string",
    "confidence": 0.92,
    "confidence_level": "HIGH",
    "cross_references": ["array"],
    "validated_at": "ISO timestamp",
    "validation_type": "auto|manual"
  }
}
```

### Check Cache for Claim

```json
{
  "operation": "lookup",
  "claim_text": "SmoothedValue uses getNextValue() per sample",
  "result": {
    "found": true,
    "cached_result": { ... }
  }
}
```

### Record Partial Read

```json
{
  "operation": "partial_read",
  "source_path": "dsp-kb/reverb/convolution-reverb.json",
  "bytes_read": 5000,
  "total_bytes": 15000
}
```

### Invalidate Entry

```json
{
  "operation": "invalidate",
  "claim_hash": "sha256:abc123...",
  "reason": "source_file_changed"
}
```

### Refresh All

```json
{
  "operation": "refresh",
  "result": {
    "cleared_claims": 12,
    "cleared_sources": 5
  }
}
```

## Claim Hashing

Claims are hashed for cache lookup:

```
claim_hash = sha256(normalized_claim_text)
```

**Normalization:**
1. Lowercase
2. Remove extra whitespace
3. Remove punctuation variations
4. Standardize quotes to double quotes

**Example:**
```
"SmoothedValue uses getNextValue() per sample"
→ "smoothedvalue uses getnextvalue per sample"
→ sha256 hash
```

## Session Scope

- Cache lives in memory for the session only
- No disk persistence
- New session starts fresh
- Cache is not shared between sessions

## Invalidation Triggers

| Trigger | Action |
|---------|--------|
| Source file changed | Invalidate all claims from that source |
| Explicit contradiction found | Invalidate original claim |
| Manual `/kb-validate --refresh` | Clear all cache |
| Session end | Cache discarded |

## Cache Statistics

```json
{
  "stats": {
    "total_claims": 12,
    "by_level": {
      "HIGH": 8,
      "MEDIUM": 3,
      "LOW": 1
    },
    "hit_rate": 0.75,
    "partial_reads_tracked": 3
  }
}
```