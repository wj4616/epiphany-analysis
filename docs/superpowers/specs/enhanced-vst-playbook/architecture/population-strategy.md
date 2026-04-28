# Population Strategy

> Defines how the knowledge base will be populated: placeholder seeding, harvesting sources, curation rules, and sync rules.

---

## 1. Population Overview

### Population Phases

| Phase | When | What | Method |
|-------|------|------|--------|
| **Phase 3** | KB Bootstrapping | Initial structure + placeholders | Template seeding |
| **Phase 3** | KB Bootstrapping | Existing content migration | Direct copy |
| **Phase 3** | KB Bootstrapping | Research harvest | Knowledge extraction |
| **Ongoing** | During development | New topics | Harvest on demand |
| **Phase 12** | Retrospective | Final validation | Confidence review |

### Population Sources

| Source Type | Confidence | When Used |
|-------------|------------|-----------|
| Academic paper | 1.0 | DSP algorithms, theory |
| Professional reference | 0.95 | Implementation patterns |
| Expert interview | 0.85 | Sound design translations |
| Community best practice | 0.75 | Common patterns |
| Tutorial/guide | 0.65 | How-to content |
| Forum discussion | 0.50 | Troubleshooting (verify first) |

---

## 2. Placeholder Seeding

### When to Seed Placeholders

Placeholders are seeded when:
1. A category is defined but content is not yet harvested
2. A topic is identified as needed but not yet researched
3. A bridge term is encountered but translation not yet created

### Placeholder Template

```json
{
  "id": "placeholder-{category}-{topic}",
  "title": "PLACEHOLDER: {Topic Name}",
  "category": "{category}",
  "status": "placeholder",
  "needed_by": ["Phase 4", "Phase 5"],
  "confidence": 0.0,
  "source": {
    "type": "pending_harvest",
    "priority": "high|medium|low"
  }
}
```

### Placeholder Status Tracking

| Status | Meaning | Action |
|--------|---------|--------|
| `placeholder` | Not yet populated | Harvest needed |
| `partial` | Some content | Additional harvest needed |
| `complete` | Fully populated | Ready for use |
| `validated` | Reviewed and verified | Safe to use |

---

## 3. Harvesting Sources

### Academic Sources

| Source | Topics | URL/Reference |
|--------|--------|---------------|
| Julius O. Smith III | FDN reverb, filters, DSP theory | ccrma.stanford.edu/~jos/ |
| Will Pirkle Books | VST/AU/AAX development, DSP | willpirkle.com |
| Cytomic Papers | TPT SVF, filters | cytomic.com |
| AES Convention Papers | Audio engineering | aes.org |
| DAFx Conference | DSP research | dafx.de |

### Professional Sources

| Source | Topics | URL/Reference |
|--------|--------|---------------|
| JUCE Documentation | JUCE framework | juce.com/learn/ |
| JUCE Tutorials | Implementation patterns | juce.com/learn/tutorials |
| melatonin.dev Blog | JUCE tips, pluginval | melatonin.dev |
| KVR Developer Forum | Real-world issues | kvraudio.com/forum |
| DSPRelated.com | DSP theory | dsprelated.com |

### Expert Sources (For Sound Design)

| Source | Topics | Method |
|--------|--------|--------|
| Sound Design Interviews | Psychedelic, ambient, creative | Direct interview |
| Preset Analysis | Parameter translations | Reverse engineering |
| Synthesizer Manuals | Sound design techniques | Documentation |
| YouTube Tutorials | Creative techniques | Transcript + verify |
| Community Knowledge | Common patterns | KVR, Gearspace |

### Market Research Sources (New)

| Source | Topics | URL/Reference |
|--------|--------|---------------|
| KVR Marketplace | Pricing strategies | kvraudio.com/marketplace |
| Plugin Alliance | Distribution models | plugin-alliance.com |
| Splice | Market trends | splice.com |
| Gearnews | Industry news | gearnews.com |
| Bedroom Producers Blog | Pricing analysis | bedroomproducersblog.com |

---

## 4. Harvesting Process

### Step 1: Identify Need

```
Trigger: Phase needs KB entry
  → Check if entry exists
  → If not, create placeholder
  → Mark as needed_by: [Phase N]
```

### Step 2: Find Source

```
Search priority:
  1. Academic papers (if algorithm)
  2. Professional references (if implementation)
  3. Expert interviews (if sound design)
  4. Community best practices (if common pattern)
  5. Tutorials (if how-to)
```

### Step 3: Extract Knowledge

```
For each source:
  1. Read/analyze source
  2. Extract key concepts
  3. Map to entry schema
  4. Add citations
  5. Assign confidence score
```

### Step 4: Create Entry

```
Entry creation:
  1. Generate unique ID
  2. Fill required fields
  3. Add optional fields as available
  4. Link to related entries
  5. Mark phase_usage
  6. Update manifest
```

### Step 5: Validate Entry

```
Validation:
  1. Schema validation
  2. Confidence check (> 0.5)
  3. Source citation check
  4. Related entries linked
  5. Update master-index
```

---

## 5. Harvesting by KB Type

### DSP KB Harvesting

| Entry Type | Primary Source | Confidence Target |
|------------|----------------|-------------------|
| Filter algorithms | Julius Smith, Cytomic | 1.0 |
| Oscillator algorithms | Will Pirkle | 0.95 |
| Effects algorithms | Will Pirkle, JUCE | 0.95 |
| Dynamics | Academic papers | 1.0 |

**Example Harvest (TPT SVF):**
```json
{
  "id": "dsp-tpt-svf",
  "title": "Topology-Preserving State Variable Filter",
  "category": "filters",
  "algorithm": {
    "description": "State variable filter with stable modulation",
    "math": "y_lp = y_lp + g * (input - y_hp); y_hp = input - y_lp - r * y_bp; y_bp = y_bp + g * y_hp",
    "coefficients": {
      "g": "tan(π * cutoff / sampleRate)",
      "r": "1 / (2 * Q)"
    }
  },
  "source": {
    "type": "professional_reference",
    "author": "Andy Simper",
    "title": "The Art of VA Filter Design",
    "url": "https://cytomic.com/"
  },
  "confidence": 1.0
}
```

### Sound Design KB Harvesting

| Entry Type | Primary Source | Confidence Target |
|------------|----------------|-------------------|
| Synthesis techniques | Expert interviews, manuals | 0.85 |
| Sonic translations | Expert interviews | 0.85 |
| Preset templates | Preset analysis | 0.75 |
| Effect chains | Sound design guides | 0.75 |

**Example Harvest (Psychedelic Bridge):**
```json
{
  "id": "bridge-psychedelic-ambient",
  "sonic_description": "swirling, evolving, psychedelic ambient texture",
  "sonic_keywords": ["psychedelic", "swirling", "evolving", "ambient", "texture"],
  "translation": {
    "filter": {
      "cutoff_range": [0.2, 0.5],
      "resonance_range": [0.1, 0.3],
      "modulation": [{
        "source": "lfo",
        "depth": [0.1, 0.3],
        "rate_range": [0.05, 0.2]
      }]
    },
    "reverb": {
      "mix_range": [0.4, 0.8],
      "decay_range": [2.0, 8.0]
    }
  },
  "why": "Slow filter modulation in mid-range creates evolving textures; long reverb adds space",
  "source": {
    "type": "expert_synthesist",
    "reference": "Interview with ambient music producer"
  },
  "confidence": 0.85
}
```

### Market KB Harvesting

| Entry Type | Primary Source | Confidence Target |
|------------|----------------|-------------------|
| Pricing strategies | Market analysis | 0.75 |
| Distribution models | Platform research | 0.80 |
| Marketing tactics | Industry analysis | 0.70 |

---

## 6. Curation Rules

### Entry Quality Standards

| Field | Requirement |
|-------|-------------|
| `id` | Unique, kebab-case, prefixed by category |
| `title` | Human-readable, 5-200 characters |
| `confidence` | Minimum 0.5 for use |
| `source` | Required, with citation |
| `description` | Clear, actionable description |

### Confidence Thresholds

| Confidence | Use Case |
|------------|----------|
| 1.0 | Academic source, verified |
| 0.85-0.99 | Expert source, verified |
| 0.75-0.84 | Community best practice |
| 0.65-0.74 | Tutorial, needs verification |
| 0.50-0.64 | Forum discussion, needs verification |
| < 0.50 | Not for use, needs research |

### Curation Workflow

```
1. Entry created with confidence
2. If confidence < 0.5 → Mark as needs_research
3. If confidence >= 0.5 and < 0.75 → Mark as needs_verification
4. If confidence >= 0.75 → Mark as usable
5. Periodic review → Update confidence based on usage
```

---

## 7. Bridge Entry Creation

### When to Create Bridge Entries

Bridge entries are created when:
1. Sonic description needs translation to parameters
2. User uses descriptive terms (warm, bright, lush, etc.)
3. Preset creation from description
4. Sound design guidance from qualitative terms

### Bridge Creation Process

```
1. Identify sonic term (e.g., "warm")
2. Research parameter ranges
   a. Expert interview or
   b. Preset analysis or
   c. Community consensus
3. Define translation object
4. Add "why" explanation
5. Set confidence score
6. Link to related bridges
7. Update master-index
```

### Sonic Term Mapping

| Sonic Term | Typical Parameters |
|------------|-------------------|
| **warm** | Filter cutoff [0.2, 0.4], resonance [0.1, 0.2], soft attack |
| **bright** | Filter cutoff [0.6, 1.0], fast attack |
| **lush** | Detune [0.05, 0.15], chorus [0.3, 0.5], reverb [0.3, 0.6] |
| **punchy** | Fast attack, filter envelope, transient emphasis |
| **evolving** | LFO modulation, slow rates [0.05, 0.2] |
| **psychedelic** | Filter modulation, feedback, reverb, modulation |
| **ambient** | Long release, reverb [0.4, 0.8], slow attack |
| **harsh** | Distortion, bright filter [0.7, 1.0], fast attack |

---

## 8. Sync Rules

### KB Sync Events

| Event | Action |
|-------|--------|
| New entry created | Update master-index.json |
| Entry updated | Update manifest.json |
| Entry deprecated | Mark deprecated, add deprecated_by |
| Confidence changed | Re-validate usage |
| Phase completed | Audit phase_usage |

### Master Index Structure

```json
{
  "version": "1.0.0",
  "updated": "2026-04-02",
  "categories": {
    "dsp-kb": {
      "count": 45,
      "last_updated": "2026-04-02"
    },
    "sound-design-kb": {
      "count": 78,
      "bridges": 15,
      "last_updated": "2026-04-02"
    }
  },
  "total_entries": 250,
  "confidence_average": 0.82
}
```

### Manifest Structure (Per KB)

```json
{
  "kb_name": "dsp-kb",
  "version": "1.0.0",
  "entries": [
    {
      "id": "dsp-tpt-svf",
      "title": "Topology-Preserving State Variable Filter",
      "category": "filters",
      "confidence": 1.0,
      "status": "validated"
    }
  ]
}
```

---

## 9. Harvesting Schedule

### Phase 3 (KB Bootstrapping)

| KB | Priority | Estimated Entries | Source |
|----|----------|-------------------|--------|
| Failure Modes KB | High | 50 | Existing playbook |
| DSP KB | High | 30 | Existing playbook + harvest |
| C++ KB | High | 20 | Existing playbook |
| Platform KB | Medium | 15 | Research docs |
| Sound Design KB | High | 40 | Harvest + bridges |
| UI KB | Medium | 20 | Existing + enhance |
| Market KB | High | 15 | Research + create |
| Testing KB | Medium | 10 | Existing playbook |

### Ongoing Harvesting

| Trigger | Action |
|---------|--------|
| Phase 4 needs algorithm | Harvest from sources |
| Phase 5 needs preset | Create from bridges |
| Phase 7 needs UI pattern | Harvest from existing |
| Phase 9 needs validation | Verify bridges |
| User requests new topic | Harvest on demand |

---

## 10. Validation Checklist

### Before Use (Per Entry)

- [ ] Entry has valid schema
- [ ] Confidence >= 0.5
- [ ] Source is cited
- [ ] Related entries are linked
- [ ] Phase usage is documented

### Before Phase Gate (Per KB)

- [ ] All phase-required entries exist
- [ ] Placeholder entries are populated
- [ ] Bridge entries have translations
- [ ] Confidence average >= 0.75

### Before Release (Overall)

- [ ] All entries validated
- [ ] No placeholder entries remaining
- [ ] Confidence average >= 0.80
- [ ] All sources cited
- [ ] Master index up to date