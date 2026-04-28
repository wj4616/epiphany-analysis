# KB Population Strategy

## Overview

This document defines how the Knowledge Base gets populated: placeholder seeding, content harvesting, curation rules, and synchronization.

---

## Placeholder Seeding

### Purpose
Create minimal entry structure before content exists, enabling immediate reference during playbook execution.

### Rules

1. **One placeholder per topic per layer** — minimum viable structure
2. **Status: `placeholder`** — clearly marked as incomplete
3. **Version: `0.1.0`** — initial version
4. **Summary: describes what will be added** — not empty
5. **Description: empty** — content to be harvested

### Placeholder Template

```json
{
  "id": "{kb-name}_{topic}_{entry-name}",
  "kb": "{layer-name}",
  "topic": "{topic}",
  "status": "placeholder",
  "version": "0.1.0",
  "title": "{Title}",
  "summary": "PLACEHOLDER: This entry will contain {description of what it will cover}.",
  "description": "",
  "source": "PLACEHOLDER",
  "tags": ["placeholder"],
  "difficulty": "intermediate",
  "domain_relevance": 5
}
```

### Required Placeholders by Layer

| Layer | Topics | Min Placeholders |
|-------|--------|------------------|
| Technical KB | cpp-audio, juce-patterns, dsp-algorithms, audio-thread-safety, cmake-build, vst3-sdk, parameter-management, state-persistence | 8 |
| Sound Design KB | synthesis-theory, oscillator-design, filter-design, modulation, effects, psychedelic, ambient, pad, creative, psybient, presets | 11 |
| UI/UX KB | visual-standards, control-layout, knobs, sliders, meters, theming, lookandfeel, accessibility | 8 |
| Commercial KB | marketplaces, licensing, pricing, marketing, anti-piracy, distribution, business-planning | 7 |
| Reference KB | textbooks, academic-papers, official-docs, tutorials, videos, community | 6 |
| **Total** | | **40** |

---

## Harvesting

### Sources

| Source | Target Layers | Quality Criteria |
|--------|--------------|------------------|
| research/domain-analysis.md | Technical KB, Sound Design KB | All process descriptions, failure modes, terminology |
| research/best-practices.md | Technical KB, Sound Design KB, Reference KB | All patterns, standards, textbook references |
| research/competitive-templates.md | All layers | Gap analysis, template features |
| research/cross-cutting-concerns.md | Technical KB | All CCC entries |
| research/platform-concerns.md | Technical KB | JUCE, Linux, macOS, Windows specifics |
| VST Playbook v7 failure_modes | Technical KB | All FM-xx, EM-xx, CM-xx entries |
| VST Playbook v7 sound-design-kb | Sound Design KB, Bridge entries | Existing bridges, preset templates |
| Web search results | Commercial KB, Reference KB | Marketplace docs, licensing info |

### Harvesting Process

1. **Read source document**
2. **Identify knowledge items** — discrete facts, patterns, references
3. **Map to KB topic** — which layer/topic does this belong to?
4. **Create/update entry** — follow entry-schema.json
5. **Set status: `harvested`** — content captured, not yet reviewed
6. **Update manifest** — increment harvested count

### Quality Criteria for Harvested Content

| Criterion | Requirement |
|-----------|-------------|
| Accuracy | Matches source without interpretation |
| Completeness | All key points from source captured |
| Attribution | Source field populated |
| Schema compliance | Validates against entry-schema.json |
| No duplication | Check existing entries before adding |

---

## Curation Rules

### Transition: `harvested` → `curated`

| Rule | Description |
|------|-------------|
| **Human review** | Entry reviewed by domain-knowledgeable human |
| **Accuracy verified** | Content confirmed correct |
| **Completeness verified** | No significant gaps |
| **Cross-references added** | Related topics populated |
| **Tags refined** | Searchable tags appropriate |
| **Code tested** | Code blocks compile and work (Technical KB) |

### Curation Checklist

- [ ] Content accurate and complete
- [ ] Source properly attributed
- [ ] Related topics linked
- [ ] Tags appropriate
- [ ] Difficulty rating appropriate
- [ ] Domain relevance accurate
- [ ] Code blocks tested (if any)
- [ ] Failure modes linked (Technical KB)

---

## Sync Rules

### Transition: `curated` → `synced`

| Rule | Description |
|------|-------------|
| **Manifest updated** | Entry appears in correct manifest |
| **Master index updated** | Entry appears in master-index.json |
| **Cross-references resolved** | All related_topic links valid |
| **Version bumped** | Major content changes = minor version bump |

### Index Update Process

1. Entry status changes to `curated`
2. Per-layer manifest.json updated
3. Master master-index.json updated
4. Cross-reference graph rebuilt
5. Status changes to `synced`

---

## Versioning Protocol

### Rules

1. **Entries are append-only** — Never edit existing entries
2. **New versions supersede old** — Queries filter by latest version
3. **Old versions remain for reference** — Stored in `_archive/`
4. **Version format: semver** — `major.minor.patch`

### Version Bump Rules

| Change Type | Version Impact |
|-------------|----------------|
| New entry | 1.0.0 |
| Content correction | patch (1.0.1) |
| Content expansion | minor (1.1.0) |
| Schema change | major (2.0.0) |
| Placeholder → harvested | 0.1.0 → 0.2.0 |
| Harvested → curated | 0.2.0 → 1.0.0 |

### Archive Process

When superseding an entry:
1. Copy old entry to `{layer}/_archive/{id}_{old-version}.json`
2. Create new entry with incremented version
3. Update manifest to point to new version
4. Old entry remains accessible in archive

---

## Bridge Entry Population

### Initial Seeding

Bridge entries are pre-seeded with common mappings:

| Descriptor | Parameters | Source |
|------------|------------|--------|
| warm | filter_cutoff, filter_resonance, saturation | Sound Design KB |
| bright | filter_cutoff, resonance | Sound Design KB |
| lush | detune, chorus, reverb_mix | Sound Design KB |
| punchy | attack, filter_env_amount | Sound Design KB |
| psychedelic | delay_feedback, delay_mod, filter_cutoff | Sound Design KB |
| ambient | reverb_decay, delay_feedback, filter_cutoff | Sound Design KB |
| deep | filter_cutoff, resonance, sub_osc | Sound Design KB |

### Auto-Generation

When agent encounters unmapped descriptor:
1. Check if similar descriptor exists
2. If not, create placeholder bridge entry
3. Prompt user for mapping details
4. Update with `confidence: 0.5` (uncertain)
5. Mark for curation review

### Feedback Enrichment

When user evaluation refines entry:
1. Add feedback to `user_feedback` array
2. Adjust parameter ranges if validated
3. Increase confidence score
4. Add `why` rationale if missing

---

## Quality Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| Placeholder coverage | 100% topics | Count placeholders vs topics |
| Harvest completion | 80%+ entries | Count harvested vs placeholders |
| Curation completion | 50%+ entries | Count curated vs harvested |
| Bridge coverage | 7+ descriptors | Count unique descriptors |
| Schema compliance | 100% | Validate all entries against schema |