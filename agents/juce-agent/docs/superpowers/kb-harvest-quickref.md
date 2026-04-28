# KB Harvesting Quick Reference

## Commands

### Check Status

```bash
# Show KB fill status
/kb-harvest --status

# Show credit usage
/kb-harvest --credits
```

### Harvest Content

```bash
# Harvest phase 1 (DSP core)
/kb-harvest --phase 1

# Harvest specific file
/kb-harvest --kb dsp-kb --topic reverb --file algorithmic-reverb.json

# Harvest entire topic
/kb-harvest --kb midi-kb --topic protocol
```

### Merge Content

```bash
# Merge single file
/kb-harvest --merge --kb dsp-kb --topic reverb --file algorithmic-reverb.json

# Merge all harvested content
/kb-harvest --merge --all
```

### Clean Up

```bash
# Clean checkpoints
/kb-harvest --clean --kb dsp-kb

# Start fresh
/kb-harvest --phase 1 --no-resume
```

## Phase Priority

| Phase | KB | Topics | Impact |
|-------|-----|--------|--------|
| 1 | dsp-kb | reverb, dynamics | DSP core |
| 2 | juce-kb | realtime | Real-time safety |
| 3 | testing-kb | validation | Testing foundation |
| 4 | midi-kb | protocol, mpe | MIDI fundamentals |

## Credit Budget

- Available: 20,500 credits
- Per file: ~95 credits (range: 50-150)
- Per phase: ~200-400 credits
- Recommended reserve: ~3,500 credits

## Auto-Harvest Configuration

File: `~/.claude/kb-harvest-config.json`

```json
{
  "session_budget": 100,
  "budget_exceeded_action": "prompt",
  "auto_harvest_enabled": true
}
```

## Status Values

| Status | Meaning |
|--------|---------|
| `placeholder` | File created, not harvested |
| `harvested` | Content harvested, not reviewed |
| `curated` | Content reviewed and approved |
| `failed` | Harvest failed, needs retry |

## Troubleshooting

### API Key Not Set

```bash
export FIRECRAWL_API_KEY="your_key"
```

### Rate Limited

```bash
# Increase delay
FIRECRAWL_DELAY=5.0 ./scripts/harvest-batch.sh --phase 1
```

### Resume Interrupted Harvest

```bash
# Automatically resumes from checkpoint
./scripts/harvest-batch.sh --phase 1
```