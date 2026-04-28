# Knowledge Base Expansion Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Create 63 new knowledge base topic files across 5 KBs with proper folder structure, indexes, and cross-references. Preserve existing data while merging new content.

**Architecture:** Each topic is a JSON file with standard KB format. Two new KBs (testing-kb, midi-kb) require full infrastructure. Three existing KBs (dsp-kb, sound-design-kb, juce-kb) require careful merging of new topics with existing index entries.

**Tech Stack:** JSON files, Python 3.12+, shell commands.

**Working Directory:** All commands assume `/home/myuser/agents/juce-agent/playbookdata`

---

## Pre-Flight: Set Working Directory

All subsequent commands must be run from playbookdata directory.

```bash
cd /home/myuser/agents/juce-agent/playbookdata
```

---

## Phase 1: Infrastructure Setup

### Task 1: Create Directory Structure

**Files:**
- Create: `testing-kb/` subdirectories
- Create: `midi-kb/` subdirectories
- Create: `dsp-kb/` subdirectories
- Create: `sound-design-kb/` subdirectories
- Create: `juce-kb/` subdirectories
- Create: `scripts/` directory

- [ ] **Step 1: Create all KB directory structures**

```bash
cd /home/myuser/agents/juce-agent/playbookdata
mkdir -p testing-kb/{unit-testing,daw-testing,validation,regression,performance-testing}
mkdir -p midi-kb/{protocol,realtime,mpe,controllers,advanced}
mkdir -p dsp-kb/{reverb,dynamics,modulation,delay,distortion,pitch,spatial}
mkdir -p sound-design-kb/{preset-design,modulation-routing,layering,effects-chain}
mkdir -p juce-kb/{realtime,formats,performance,state-advanced}
mkdir -p scripts
```

- [ ] **Step 2: Verify directories created**

Run: `find . -type d -name "reverb" -o -name "testing-kb" -o -name "midi-kb" -o -name "scripts" 2>/dev/null | sort`
Expected:
```
./dsp-kb/reverb
./midi-kb
./scripts
./testing-kb
```

- [ ] **Step 3: Commit infrastructure changes**

```bash
git add -A
git commit -m "feat(kb): create directory structure for expansion"
```

---

### Task 2: Create Template Generator Script

**Files:**
- Create: `scripts/create-kb-template.py`

- [ ] **Step 1: Write template generator script**

```python
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
```

- [ ] **Step 2: Make script executable**

Run: `chmod +x scripts/create-kb-template.py`

- [ ] **Step 3: Test script with dry run**

```bash
echo '{"search_terms": ["test"], "sources": ["test-source"]}' | python3 scripts/create-kb-template.py testing-kb unit-testing test-placeholder.json "Test Title" "Test description" 5
cat testing-kb/unit-testing/test-placeholder.json
rm testing-kb/unit-testing/test-placeholder.json
```

Expected: JSON file created with correct structure

- [ ] **Step 4: Commit template script**

```bash
git add scripts/create-kb-template.py
git commit -m "feat(kb): add KB template generator script"
```

---

### Task 3: Create testing-kb Index and Manifest

**Files:**
- Create: `testing-kb/index.json`
- Create: `testing-kb/manifest.json`

- [ ] **Step 1: Write testing-kb/index.json**

```bash
cat > testing-kb/index.json << 'EOF'
{
  "kb_name": "testing-kb",
  "version": "1.0.0",
  "topics": {
    "unit-testing": [
      "unit-testing/catch2-juce.json",
      "unit-testing/mocking-audio.json",
      "unit-testing/test-driven-audio.json"
    ],
    "daw-testing": [
      "daw-testing/reaper-testing.json",
      "daw-testing/ableton-testing.json",
      "daw-testing/logic-testing.json"
    ],
    "validation": [
      "validation/pluginval.json",
      "validation/automated-validation.json"
    ],
    "regression": [
      "regression/audio-comparison.json",
      "regression/snapshot-testing.json"
    ],
    "performance-testing": [
      "performance-testing/cpu-profiling.json",
      "performance-testing/latency-measurement.json"
    ]
  },
  "last_updated": "2026-03-30T00:00:00Z",
  "metadata": {
    "total_files": 10,
    "source_type": "curated",
    "categories": ["unit-testing", "daw-testing", "validation", "regression", "performance-testing"]
  }
}
EOF
```

- [ ] **Step 2: Write testing-kb/manifest.json**

```bash
cat > testing-kb/manifest.json << 'EOF'
{
  "kb_name": "testing-kb",
  "status": "ready",
  "created": "2026-03-30T00:00:00Z",
  "description": "Audio plugin testing and validation knowledge",
  "phases": [
    {
      "name": "unit-testing",
      "status": "pending",
      "topics": ["catch2-juce", "mocking-audio", "test-driven-audio"]
    },
    {
      "name": "daw-testing",
      "status": "pending",
      "topics": ["reaper-testing", "ableton-testing", "logic-testing"]
    },
    {
      "name": "validation",
      "status": "pending",
      "topics": ["pluginval", "automated-validation"]
    },
    {
      "name": "regression",
      "status": "pending",
      "topics": ["audio-comparison", "snapshot-testing"]
    },
    {
      "name": "performance-testing",
      "status": "pending",
      "topics": ["cpu-profiling", "latency-measurement"]
    }
  ]
}
EOF
```

- [ ] **Step 3: Validate testing-kb JSON**

Run: `python3 -c "import json; json.load(open('testing-kb/index.json')); json.load(open('testing-kb/manifest.json')); print('Valid JSON')"`
Expected: "Valid JSON"

---

### Task 4: Create midi-kb Index and Manifest

**Files:**
- Create: `midi-kb/index.json`
- Create: `midi-kb/manifest.json`

- [ ] **Step 1: Write midi-kb/index.json**

```bash
cat > midi-kb/index.json << 'EOF'
{
  "kb_name": "midi-kb",
  "version": "1.0.0",
  "topics": {
    "protocol": [
      "protocol/midi-basics.json",
      "protocol/midi-messages.json",
      "protocol/channel-messages.json",
      "protocol/system-messages.json"
    ],
    "realtime": [
      "realtime/midi-timing.json",
      "realtime/midi-buffering.json",
      "realtime/jitter-compensation.json",
      "realtime/midi-latency.json"
    ],
    "mpe": [
      "mpe/mpe-basics.json",
      "mpe/mpe-implementation.json",
      "mpe/mpe-mapping.json"
    ],
    "controllers": [
      "controllers/controller-mapping.json",
      "controllers/midi-learn.json",
      "controllers/parameter-automation.json",
      "controllers/hardware-integration.json"
    ],
    "advanced": [
      "advanced/sysex.json",
      "advanced/midi-clock.json",
      "advanced/midi-2.json"
    ]
  },
  "last_updated": "2026-03-30T00:00:00Z",
  "metadata": {
    "total_files": 16,
    "source_type": "curated",
    "categories": ["protocol", "realtime", "mpe", "controllers", "advanced"]
  }
}
EOF
```

- [ ] **Step 2: Write midi-kb/manifest.json**

```bash
cat > midi-kb/manifest.json << 'EOF'
{
  "kb_name": "midi-kb",
  "status": "ready",
  "created": "2026-03-30T00:00:00Z",
  "description": "MIDI protocol and implementation for audio plugins",
  "phases": [
    {
      "name": "protocol",
      "status": "pending",
      "topics": ["midi-basics", "midi-messages", "channel-messages", "system-messages"]
    },
    {
      "name": "realtime",
      "status": "pending",
      "topics": ["midi-timing", "midi-buffering", "jitter-compensation", "midi-latency"]
    },
    {
      "name": "mpe",
      "status": "pending",
      "topics": ["mpe-basics", "mpe-implementation", "mpe-mapping"]
    },
    {
      "name": "controllers",
      "status": "pending",
      "topics": ["controller-mapping", "midi-learn", "parameter-automation", "hardware-integration"]
    },
    {
      "name": "advanced",
      "status": "pending",
      "topics": ["sysex", "midi-clock", "midi-2"]
    }
  ]
}
EOF
```

- [ ] **Step 3: Validate midi-kb JSON**

Run: `python3 -c "import json; json.load(open('midi-kb/index.json')); json.load(open('midi-kb/manifest.json')); print('Valid JSON')"`
Expected: "Valid JSON"

---

### Task 5: Create testing-kb Placeholder Files (10 files)

**Files:**
- Create: 10 JSON files in testing-kb/

- [ ] **Step 1: Create unit-testing files (3 files)**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py testing-kb unit-testing catch2-juce.json "Catch2 Testing for JUCE" "Unit testing JUCE audio plugins with Catch2 framework" 9 << 'EOF'
{"search_terms": ["Catch2 JUCE", "unit test audio plugin", "JUCE testing", "Catch2 C++"], "sources": ["Catch2 docs", "JUCE forum", "melatonin.dev"]}
EOF

python3 scripts/create-kb-template.py testing-kb unit-testing mocking-audio.json "Mocking Audio Components" "Mocking AudioProcessor and audio buffers for testing" 8 << 'EOF'
{"search_terms": ["mocking audio", "mock AudioProcessor", "fake audio buffer", "test double audio"], "sources": ["JUCE forum", "testing blogs"]}
EOF

python3 scripts/create-kb-template.py testing-kb unit-testing test-driven-audio.json "Test-Driven Audio Development" "TDD methodology for audio plugin development" 8 << 'EOF'
{"search_terms": ["TDD audio plugin", "test driven development DSP", "testing audio code"], "sources": ["Software testing blogs", "JUCE forum"]}
EOF
```

- [ ] **Step 2: Create daw-testing files (3 files)**

```bash
python3 scripts/create-kb-template.py testing-kb daw-testing reaper-testing.json "REAPER Plugin Testing" "Testing plugins in REAPER DAW environment" 7 << 'EOF'
{"search_terms": ["REAPER plugin testing", "REAPER development", "ReaScript plugin test"], "sources": ["REAPER forums", "Cockos docs"]}
EOF

python3 scripts/create-kb-template.py testing-kb daw-testing ableton-testing.json "Ableton Live Plugin Testing" "Testing plugins in Ableton Live DAW" 7 << 'EOF'
{"search_terms": ["Ableton plugin testing", "Live plugin validation", "Ableton development"], "sources": ["Ableton docs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py testing-kb daw-testing logic-testing.json "Logic Pro Plugin Testing" "Testing AU plugins in Logic Pro" 7 << 'EOF'
{"search_terms": ["Logic Pro plugin testing", "AU testing Logic", "Logic plugin validation"], "sources": ["Apple docs", "JUCE forum"]}
EOF
```

- [ ] **Step 3: Create validation files (2 files)**

```bash
python3 scripts/create-kb-template.py testing-kb validation pluginval.json "Pluginval Validation" "Automated plugin validation with pluginval" 9 << 'EOF'
{"search_terms": ["pluginval JUCE", "JUCE validation", "plugin validation tool", "Tracktion pluginval"], "sources": ["pluginval docs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py testing-kb validation automated-validation.json "Automated Plugin Validation" "CI/CD pipeline validation for audio plugins" 8 << 'EOF'
{"search_terms": ["CI plugin testing", "GitHub Actions audio plugin", "automated plugin validation"], "sources": ["JUCE forum", "CI docs"]}
EOF
```

- [ ] **Step 4: Create regression files (2 files)**

```bash
python3 scripts/create-kb-template.py testing-kb regression audio-comparison.json "Audio Output Comparison Testing" "Regression testing by comparing audio output" 8 << 'EOF'
{"search_terms": ["audio output comparison", "regression testing audio", "audio snapshot", "WAV comparison"], "sources": ["Testing blogs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py testing-kb regression snapshot-testing.json "Snapshot Testing for Plugins" "Parameter and state snapshot testing" 7 << 'EOF'
{"search_terms": ["snapshot testing plugin", "parameter state snapshot", "UI snapshot testing"], "sources": ["Testing blogs"]}
EOF
```

- [ ] **Step 5: Create performance-testing files (2 files)**

```bash
python3 scripts/create-kb-template.py testing-kb performance-testing cpu-profiling.json "CPU Profiling for Audio Plugins" "Profiling CPU usage in real-time audio" 9 << 'EOF'
{"search_terms": ["CPU profiling plugin", "audio CPU usage", "plugin performance testing"], "sources": ["Plugin Doctor", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py testing-kb performance-testing latency-measurement.json "Plugin Latency Measurement" "Measuring and testing plugin latency" 8 << 'EOF'
{"search_terms": ["plugin latency measurement", "round trip latency", "PDC testing"], "sources": ["JUCE forum", "DAW docs"]}
EOF
```

- [ ] **Step 6: Verify testing-kb files created**

Run: `find testing-kb -name "*.json" | wc -l`
Expected: 12 (10 content files + index.json + manifest.json)

- [ ] **Step 7: Commit testing-kb**

```bash
git add testing-kb/
git commit -m "feat(kb): create testing-kb with 10 placeholder files"
```

---

### Task 6: Create midi-kb Placeholder Files (16 files)

**Files:**
- Create: 16 JSON files in midi-kb/

- [ ] **Step 1: Create protocol files (4 files)**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py midi-kb protocol midi-basics.json "MIDI Protocol Basics" "Fundamental MIDI protocol concepts and data format" 10 << 'EOF'
{"search_terms": ["MIDI protocol basics", "MIDI specification", "MIDI data format", "MIDI message structure"], "sources": ["MIDI Association", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py midi-kb protocol midi-messages.json "MIDI Message Types" "MIDI status bytes and message formats" 10 << 'EOF'
{"search_terms": ["MIDI message types", "MIDI status byte", "MIDI data bytes", "MIDI message format"], "sources": ["MIDI Association", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py midi-kb protocol channel-messages.json "MIDI Channel Messages" "Note on/off, control change, pitch bend, program change" 10 << 'EOF'
{"search_terms": ["MIDI note on off", "MIDI control change", "MIDI pitch bend", "MIDI program change"], "sources": ["MIDI Association", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py midi-kb protocol system-messages.json "MIDI System Messages" "System exclusive, system common, and real-time messages" 9 << 'EOF'
{"search_terms": ["MIDI system exclusive", "MIDI sysEx", "MIDI system common", "MIDI real-time messages"], "sources": ["MIDI Association", "JUCE forum"]}
EOF
```

- [ ] **Step 2: Create realtime files (4 files)**

```bash
python3 scripts/create-kb-template.py midi-kb realtime midi-timing.json "MIDI Timing and Timestamps" "Sample-accurate MIDI timing and timestamps" 9 << 'EOF'
{"search_terms": ["MIDI timing", "MIDI clock", "MIDI timestamp", "sample accurate MIDI"], "sources": ["JUCE forum", "CCRMA"]}
EOF

python3 scripts/create-kb-template.py midi-kb realtime midi-buffering.json "MIDI Buffering" "MIDI buffer management in JUCE" 9 << 'EOF'
{"search_terms": ["MIDI buffer", "MIDI queue", "MidiBuffer JUCE", "MIDI buffering audio"], "sources": ["JUCE forum", "melatonin.dev"]}
EOF

python3 scripts/create-kb-template.py midi-kb realtime jitter-compensation.json "MIDI Jitter Compensation" "Handling timing jitter in MIDI" 8 << 'EOF'
{"search_terms": ["MIDI jitter", "MIDI timing jitter", "MIDI delay compensation", "MIDI timestamp"], "sources": ["DSP forums", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py midi-kb realtime midi-latency.json "MIDI Latency" "MIDI input and output latency handling" 8 << 'EOF'
{"search_terms": ["MIDI latency", "MIDI round trip", "MIDI input latency", "MIDI output latency"], "sources": ["JUCE forum", "DAW docs"]}
EOF
```

- [ ] **Step 3: Create mpe files (3 files)**

```bash
python3 scripts/create-kb-template.py midi-kb mpe mpe-basics.json "MPE Fundamentals" "MIDI Polyphonic Expression specification basics" 10 << 'EOF'
{"search_terms": ["MIDI Polyphonic Expression", "MPE specification", "MPE zones", "multidimensional polyphonic"], "sources": ["MPE specification", "MIDI Association"]}
EOF

python3 scripts/create-kb-template.py midi-kb mpe mpe-implementation.json "MPE Implementation" "Implementing MPE in JUCE plugins" 9 << 'EOF'
{"search_terms": ["MPE implementation", "MPE JUCE", "MPE zone setup", "MPE channel assignment"], "sources": ["JUCE forum", "MPE docs"]}
EOF

python3 scripts/create-kb-template.py midi-kb mpe mpe-mapping.json "MPE Parameter Mapping" "Mapping MPE dimensions to plugin parameters" 9 << 'EOF'
{"search_terms": ["MPE mapping", "MPE to parameters", "MPE dimension mapping", "MPE timbre"], "sources": ["JUCE forum", "synth docs"]}
EOF
```

- [ ] **Step 4: Create controllers files (4 files)**

```bash
python3 scripts/create-kb-template.py midi-kb controllers controller-mapping.json "MIDI Controller Mapping" "Mapping MIDI CC to plugin parameters" 9 << 'EOF'
{"search_terms": ["MIDI controller mapping", "CC to parameter", "MIDI CC assignment", "controller assignment"], "sources": ["JUCE forum", "synth tutorials"]}
EOF

python3 scripts/create-kb-template.py midi-kb controllers midi-learn.json "MIDI Learn Implementation" "Implementing MIDI learn for plugins" 10 << 'EOF'
{"search_terms": ["MIDI learn", "MIDI teach", "plugin MIDI learn", "automatic MIDI mapping"], "sources": ["JUCE forum", "plugin docs"]}
EOF

python3 scripts/create-kb-template.py midi-kb controllers parameter-automation.json "MIDI Parameter Automation" "MIDI-based parameter automation" 8 << 'EOF'
{"search_terms": ["MIDI automation", "MIDI CC automation", "parameter mapping MIDI", "MIDI recording"], "sources": ["JUCE forum", "DAW docs"]}
EOF

python3 scripts/create-kb-template.py midi-kb controllers hardware-integration.json "MIDI Hardware Integration" "Integrating MIDI hardware with plugins" 8 << 'EOF'
{"search_terms": ["MIDI hardware integration", "MIDI controller plugin", "USB MIDI", "MIDI interface"], "sources": ["JUCE forum", "MIDI docs"]}
EOF
```

- [ ] **Step 5: Create advanced files (3 files)**

```bash
python3 scripts/create-kb-template.py midi-kb advanced sysex.json "MIDI System Exclusive" "SysEx message handling and protocols" 8 << 'EOF'
{"search_terms": ["MIDI system exclusive", "SysEx protocol", "SysEx JUCE", "manufacturer SysEx"], "sources": ["MIDI Association", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py midi-kb advanced midi-clock.json "MIDI Clock and Sync" "MIDI beat clock and synchronization" 8 << 'EOF'
{"search_terms": ["MIDI clock", "MIDI sync", "MIDI tempo", "MIDI beat clock"], "sources": ["JUCE forum", "MIDI docs"]}
EOF

python3 scripts/create-kb-template.py midi-kb advanced midi-2.json "MIDI 2.0" "MIDI 2.0 specification and JUCE support" 7 << 'EOF'
{"search_terms": ["MIDI 2.0", "MIDI 2 specification", "MIDI CI", "MIDI 2.0 JUCE"], "sources": ["MIDI Association", "MIDI 2.0 spec"]}
EOF
```

- [ ] **Step 6: Verify midi-kb files created**

Run: `find midi-kb -name "*.json" | wc -l`
Expected: 18 (16 content files + index.json + manifest.json)

- [ ] **Step 7: Commit midi-kb**

```bash
git add midi-kb/
git commit -m "feat(kb): create midi-kb with 16 placeholder files"
```

---

### Task 7: Create dsp-kb Placeholder Files - reverb/ (4 files)

**Files:**
- Create: 4 JSON files in dsp-kb/reverb/

**Note:** These will be MERGED with existing `reverb` topic in dsp-kb/index.json (Task 10).

- [ ] **Step 1: Create reverb files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb reverb algorithmic-reverb.json "Algorithmic Reverb Design" "Freeverb, Schroeder, FDN reverb algorithms" 10 << 'EOF'
{"search_terms": ["algorithmic reverb design", "Freeverb", "Schroeder reverb", "feedback delay network", "FDN reverb"], "sources": ["CCRMA DSP papers", "EarLevel blog", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb reverb convolution-reverb.json "Convolution Reverb" "FFT-based convolution reverb implementation" 9 << 'EOF'
{"search_terms": ["convolution reverb", "FFT convolution", "partitioned convolution", "zero latency convolution"], "sources": ["CCRMA", "Signal Processing Stack Exchange"]}
EOF

python3 scripts/create-kb-template.py dsp-kb reverb impulse-response.json "Impulse Response Measurement" "Creating and measuring impulse responses" 8 << 'EOF'
{"search_terms": ["impulse response measurement", "IRS creation", "sine sweep", "maximum length sequence"], "sources": ["CCRMA papers", "OpenAir IR library docs"]}
EOF

python3 scripts/create-kb-template.py dsp-kb reverb reverb-design.json "Reverb Parameter Design" "Designing reverb parameters and controls" 9 << 'EOF'
{"search_terms": ["reverb parameter design", "reverb EQ", "damping", "pre-delay", "room size"], "sources": ["Sound on Sound", "Journal of AES"]}
EOF
```

- [ ] **Step 2: Verify reverb files**

Run: `ls -1 dsp-kb/reverb/`
Expected: 4 JSON files

---

### Task 8: Create dsp-kb Placeholder Files - dynamics/ (5 files)

**Files:**
- Create: 5 JSON files in dsp-kb/dynamics/

**Note:** These will be MERGED with existing `dynamics` topic in dsp-kb/index.json.

- [ ] **Step 1: Create dynamics files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb dynamics compressor.json "Audio Compressor Design" "Feedforward and feedback compression algorithms" 10 << 'EOF'
{"search_terms": ["audio compressor design", "feedforward compression", "feedback compression", "RMS compression", "peak compression"], "sources": ["EarLevel blog", "CCRMA DSP papers"]}
EOF

python3 scripts/create-kb-template.py dsp-kb dynamics limiter-design.json "Limiter Design" "Brickwall and lookahead limiter implementations" 10 << 'EOF'
{"search_terms": ["brickwall limiter", "lookahead limiter", "true peak limiter", "limiter algorithms"], "sources": ["CCRMA", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb dynamics noise-gate.json "Noise Gate Implementation" "Noise gate algorithms with hysteresis" 8 << 'EOF'
{"search_terms": ["noise gate algorithm", "hysteresis", "attack release gate", "sidechain gate"], "sources": ["musicdsp.org", "DSP Stack Exchange"]}
EOF

python3 scripts/create-kb-template.py dsp-kb dynamics expander.json "Audio Expander" "Downward and upward expansion algorithms" 7 << 'EOF'
{"search_terms": ["audio expander", "downward expansion", "upward expansion", "dynamic range"], "sources": ["AES papers", "DSP textbooks"]}
EOF

python3 scripts/create-kb-template.py dsp-kb dynamics multiband-compression.json "Multiband Compression" "Crossover-based multiband compression" 9 << 'EOF'
{"search_terms": ["multiband compressor", "crossover filter", "band splitting", "compression bands"], "sources": ["Sound on Sound", "Plugin Doctor"]}
EOF
```

- [ ] **Step 2: Verify dynamics files**

Run: `ls -1 dsp-kb/dynamics/`
Expected: 5 JSON files

---

### Task 9: Create dsp-kb Placeholder Files - modulation/ (5 files)

**Files:**
- Create: 5 JSON files in dsp-kb/modulation/

- [ ] **Step 1: Create modulation files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb modulation chorus.json "Chorus Effect Algorithm" "Delay-based chorus with multiple voices" 8 << 'EOF'
{"search_terms": ["chorus effect algorithm", "delay modulation", "multiple voices chorus", "stereo chorus"], "sources": ["musicdsp.org", "CCRMA"]}
EOF

python3 scripts/create-kb-template.py dsp-kb modulation flanger.json "Flanger Algorithm" "Comb filter flanging with feedback" 8 << 'EOF'
{"search_terms": ["flanger algorithm", "comb filter flanger", "through-zero flanger", "feedback flanger"], "sources": ["DSP Stack Exchange", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb modulation phaser.json "Phaser Algorithm" "Allpass filter phaser with LFO modulation" 8 << 'EOF'
{"search_terms": ["phaser algorithm", "allpass filter phaser", "LFO phaser", "stages phaser"], "sources": ["CCRMA papers", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb modulation tremolo.json "Tremolo Effect" "Amplitude modulation with LFO" 7 << 'EOF'
{"search_terms": ["tremolo effect", "amplitude modulation", "LFO tremolo", "stereo tremolo"], "sources": ["musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb modulation vibrato.json "Vibrato Effect" "Pitch modulation via delay modulation" 7 << 'EOF'
{"search_terms": ["vibrato effect", "pitch modulation", "delay vibrato", "depth vibrato"], "sources": ["musicdsp.org"]}
EOF
```

- [ ] **Step 2: Verify modulation files**

Run: `ls -1 dsp-kb/modulation/`
Expected: 5 JSON files

---

### Task 10: Create dsp-kb Placeholder Files - delay/ (3 files)

**Files:**
- Create: 3 JSON files in dsp-kb/delay/

**Note:** These will be MERGED with existing `delay` topic in dsp-kb/index.json.

- [ ] **Step 1: Create delay files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb delay delay-lines.json "Digital Delay Lines" "Fractional delay and interpolation techniques" 9 << 'EOF'
{"search_terms": ["digital delay line", "fractional delay", "interpolation delay", "circular buffer delay"], "sources": ["CCRMA DSP papers", "DSP textbooks"]}
EOF

python3 scripts/create-kb-template.py dsp-kb delay echo.json "Echo Effect" "Feedback delay and multi-tap echo" 7 << 'EOF'
{"search_terms": ["echo effect", "feedback delay", "multi-tap delay", "tape echo emulation"], "sources": ["musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb delay ping-pong-delay.json "Ping-Pong Delay" "Stereo delay with cross-feedback" 7 << 'EOF'
{"search_terms": ["ping pong delay", "stereo delay", "cross feedback", "delay panning"], "sources": ["Plugin development forums"]}
EOF
```

- [ ] **Step 2: Verify delay files**

Run: `ls -1 dsp-kb/delay/`
Expected: 3 JSON files

---

### Task 11: Create dsp-kb Placeholder Files - distortion/ (3 files)

**Files:**
- Create: 3 JSON files in dsp-kb/distortion/

- [ ] **Step 1: Create distortion files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb distortion waveshaping.json "Waveshaping Design" "Transfer function waveshapers and polynomial shaping" 9 << 'EOF'
{"search_terms": ["waveshaper design", "waveshaping function", "transfer function", "polynomial waveshaper"], "sources": ["CCRMA", "DSP textbooks"]}
EOF

python3 scripts/create-kb-template.py dsp-kb distortion saturation.json "Analog Saturation" "Tape and transformer saturation modeling" 8 << 'EOF'
{"search_terms": ["analog saturation", "tape saturation", "transformer saturation", "soft saturation"], "sources": ["Sound on Sound", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py dsp-kb distortion soft-clipping.json "Soft Clipping" "Anti-aliased soft clipping algorithms" 8 << 'EOF'
{"search_terms": ["soft clipper", "tanh clipping", "cubic soft clipper", "anti-aliased clipping"], "sources": ["DSP Stack Exchange"]}
EOF
```

- [ ] **Step 2: Verify distortion files**

Run: `ls -1 dsp-kb/distortion/`
Expected: 3 JSON files

---

### Task 12: Create dsp-kb Placeholder Files - pitch/ (3 files)

**Files:**
- Create: 3 JSON files in dsp-kb/pitch/

- [ ] **Step 1: Create pitch files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb pitch pitch-shifting.json "Pitch Shifting Algorithm" "Phase vocoder and granular pitch shifting" 10 << 'EOF'
{"search_terms": ["pitch shifting algorithm", "phase vocoder pitch", "granular pitch shift", "formant preservation"], "sources": ["CCRMA papers", "DSP Stack Exchange"]}
EOF

python3 scripts/create-kb-template.py dsp-kb pitch harmonizer.json "Harmonizer Effect" "Intelligent pitch-based harmonization" 8 << 'EOF'
{"search_terms": ["harmonizer effect", "intelligent harmonizer", "chord harmonizer", "pitch corrected harmonizer"], "sources": ["Plugin development blogs"]}
EOF

python3 scripts/create-kb-template.py dsp-kb pitch formant-shifting.json "Formant Shifting" "Formant preservation and shifting" 8 << 'EOF'
{"search_terms": ["formant shifting", "formant preservation", "vocoder formant", "spectral envelope"], "sources": ["CCRMA papers"]}
EOF
```

- [ ] **Step 2: Verify pitch files**

Run: `ls -1 dsp-kb/pitch/`
Expected: 3 JSON files

---

### Task 13: Create dsp-kb Placeholder Files - spatial/ (3 files)

**Files:**
- Create: 3 JSON files in dsp-kb/spatial/

- [ ] **Step 1: Create spatial files**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py dsp-kb spatial stereo-width.json "Stereo Width Processing" "Mid-side encoding and stereo widening" 8 << 'EOF'
{"search_terms": ["stereo width processing", "mid-side encoding", "stereo widening", "mono compatibility"], "sources": ["Sound on Sound"]}
EOF

python3 scripts/create-kb-template.py dsp-kb spatial mid-side-processing.json "Mid-Side Processing" "M/S encoding, decoding, and processing" 9 << 'EOF'
{"search_terms": ["M/S processing", "mid side encoding", "mid side EQ", "side chain processing"], "sources": ["AES papers", "Sound on Sound"]}
EOF

python3 scripts/create-kb-template.py dsp-kb spatial binaural-audio.json "Binaural Audio" "HRTF-based 3D audio for headphones" 9 << 'EOF'
{"search_terms": ["binaural rendering", "HRTF", "head related transfer function", "3D audio headphones"], "sources": ["CCRMA", "AES Spatial Audio papers"]}
EOF
```

- [ ] **Step 2: Verify spatial files**

Run: `ls -1 dsp-kb/spatial/`
Expected: 3 JSON files

---

### Task 14: Update dsp-kb/index.json (Merge New Topics)

**Files:**
- Modify: `dsp-kb/index.json`

**Critical:** This must MERGE with existing topics, not overwrite.

- [ ] **Step 1: Create Python script to merge dsp-kb index**

```python
#!/usr/bin/env python3
"""Merge new DSP topics into existing index.json"""

import json

# Read existing index
with open('dsp-kb/index.json', 'r') as f:
    index = json.load(f)

# New topics to ADD (not replace)
new_topics = {
    "reverb": [
        "reverb/algorithmic-reverb.json",
        "reverb/convolution-reverb.json",
        "reverb/impulse-response.json",
        "reverb/reverb-design.json"
    ],
    "dynamics": [
        "dynamics/compressor.json",
        "dynamics/limiter-design.json",
        "dynamics/noise-gate.json",
        "dynamics/expander.json",
        "dynamics/multiband-compression.json"
    ],
    "modulation": [
        "modulation/chorus.json",
        "modulation/flanger.json",
        "modulation/phaser.json",
        "modulation/tremolo.json",
        "modulation/vibrato.json"
    ],
    "delay": [
        "delay/delay-lines.json",
        "delay/echo.json",
        "delay/ping-pong-delay.json"
    ],
    "distortion": [
        "distortion/waveshaping.json",
        "distortion/saturation.json",
        "distortion/soft-clipping.json"
    ],
    "pitch": [
        "pitch/pitch-shifting.json",
        "pitch/harmonizer.json",
        "pitch/formant-shifting.json"
    ],
    "spatial": [
        "spatial/stereo-width.json",
        "spatial/mid-side-processing.json",
        "spatial/binaural-audio.json"
    ]
}

# For existing topics (reverb, delay, dynamics), APPEND new files
existing_topics = ["reverb", "delay", "dynamics"]
for topic in existing_topics:
    if topic in index["topics"]:
        # Append new files to existing list
        for new_file in new_topics[topic]:
            if new_file not in index["topics"][topic]:
                index["topics"][topic].append(new_file)
    else:
        # Create new topic
        index["topics"][topic] = new_topics[topic]

# For new topics, add them
new_only = ["modulation", "distortion", "pitch", "spatial"]
for topic in new_only:
    index["topics"][topic] = new_topics[topic]

# Add to categories
new_categories = ["modulation", "distortion", "pitch", "spatial"]
for cat in new_categories:
    if cat not in index["metadata"]["categories"]:
        index["metadata"]["categories"].append(cat)

# Update file count (existing 27 + new 26 = 53)
index["metadata"]["total_files"] = 53
index["last_updated"] = "2026-03-30T00:00:00Z"

# Write back
with open('dsp-kb/index.json', 'w') as f:
    json.dump(index, f, indent=2)

print("dsp-kb/index.json updated successfully")
print(f"Total topics: {len(index['topics'])}")
print(f"Total files: {index['metadata']['total_files']}")
```

- [ ] **Step 2: Run the merge script**

```bash
cd /home/myuser/agents/juce-agent/playbookdata
python3 << 'SCRIPT'
import json

# Read existing index
with open('dsp-kb/index.json', 'r') as f:
    index = json.load(f)

# New topics to ADD
new_topics = {
    "reverb": [
        "reverb/algorithmic-reverb.json",
        "reverb/convolution-reverb.json",
        "reverb/impulse-response.json",
        "reverb/reverb-design.json"
    ],
    "dynamics": [
        "dynamics/compressor.json",
        "dynamics/limiter-design.json",
        "dynamics/noise-gate.json",
        "dynamics/expander.json",
        "dynamics/multiband-compression.json"
    ],
    "modulation": [
        "modulation/chorus.json",
        "modulation/flanger.json",
        "modulation/phaser.json",
        "modulation/tremolo.json",
        "modulation/vibrato.json"
    ],
    "delay": [
        "delay/delay-lines.json",
        "delay/echo.json",
        "delay/ping-pong-delay.json"
    ],
    "distortion": [
        "distortion/waveshaping.json",
        "distortion/saturation.json",
        "distortion/soft-clipping.json"
    ],
    "pitch": [
        "pitch/pitch-shifting.json",
        "pitch/harmonizer.json",
        "pitch/formant-shifting.json"
    ],
    "spatial": [
        "spatial/stereo-width.json",
        "spatial/mid-side-processing.json",
        "spatial/binaural-audio.json"
    ]
}

# For existing topics, APPEND new files
existing_topics = ["reverb", "delay", "dynamics"]
for topic in existing_topics:
    if topic in index["topics"]:
        for new_file in new_topics[topic]:
            if new_file not in index["topics"][topic]:
                index["topics"][topic].append(new_file)
    else:
        index["topics"][topic] = new_topics[topic]

# For new-only topics, add them
new_only = ["modulation", "distortion", "pitch", "spatial"]
for topic in new_only:
    index["topics"][topic] = new_topics[topic]

# Add to categories
new_categories = ["modulation", "distortion", "pitch", "spatial"]
for cat in new_categories:
    if cat not in index["metadata"]["categories"]:
        index["metadata"]["categories"].append(cat)

# Update file count
index["metadata"]["total_files"] = 53
index["last_updated"] = "2026-03-30T00:00:00Z"

with open('dsp-kb/index.json', 'w') as f:
    json.dump(index, f, indent=2)

print(f"Updated dsp-kb/index.json: {len(index['topics'])} topics, {index['metadata']['total_files']} files")
SCRIPT
```

- [ ] **Step 3: Verify merge**

Run: `python3 -c "import json; d=json.load(open('dsp-kb/index.json')); print('reverb:', d['topics']['reverb']); print('delay:', d['topics']['delay']); print('dynamics:', d['topics']['dynamics'])"`
Expected: Each topic should show both old and new files

- [ ] **Step 4: Commit dsp-kb updates**

```bash
git add dsp-kb/
git commit -m "feat(kb): create dsp-kb placeholder files with 26 new topics"
```

---

### Task 15: Create sound-design-kb Placeholder Files (10 files)

**Files:**
- Create: 10 JSON files in sound-design-kb/

- [ ] **Step 1: Create preset-design files (4 files)**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py sound-design-kb preset-design preset-methodology.json "Preset Design Methodology" "Sound design workflow and preset organization" 9 << 'EOF'
{"search_terms": ["preset design methodology", "sound design workflow", "preset organization", "library design"], "sources": ["Sound on Sound", "Attack Magazine"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb preset-design macro-controls.json "Macro Control Design" "Parameter morphing and macro assignments" 8 << 'EOF'
{"search_terms": ["macro control design", "parameter morphing", "macro assignment", "performance controls"], "sources": ["Plugin development forums"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb preset-design preset-morphing.json "Preset Morphing" "Interpolation and continuous preset control" 8 << 'EOF'
{"search_terms": ["preset morphing", "interpolation presets", "XY pad presets", "continuous preset control"], "sources": ["Music production blogs"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb preset-design sound-design-workflow.json "Sound Design Workflow" "Systematic patch design process" 8 << 'EOF'
{"search_terms": ["sound design process", "synthesis workflow", "patch design steps"], "sources": ["Sound on Sound", "YouTube tutorials"]}
EOF
```

- [ ] **Step 2: Create modulation-routing files (4 files)**

```bash
python3 scripts/create-kb-template.py sound-design-kb modulation-routing modulation-routing.json "Modulation Routing" "Modulation sources, destinations, and routing" 9 << 'EOF'
{"search_terms": ["modulation routing", "mod sources destinations", "modulation matrix design", "multiple modulations"], "sources": ["Synth tutorials", "Sound on Sound"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb modulation-routing lfo-shapes.json "LFO Shapes" "Custom LFO waveforms and rate sync" 7 << 'EOF'
{"search_terms": ["LFO shapes", "custom LFO waveform", "LFO rate sync", "LFO phase"], "sources": ["Synth documentation", "musicdsp.org"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb modulation-routing envelope-curves.json "Envelope Curves" "ADSR shapes and curve design" 8 << 'EOF'
{"search_terms": ["envelope curves", "ADSR shapes", "exponential envelope", "linear envelope"], "sources": ["Synth design papers"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb modulation-routing mod-matrix.json "Modulation Matrix" "Source-destination matrix design" 9 << 'EOF'
{"search_terms": ["modulation matrix", "mod routing", "source destination matrix", "polyphonic modulation"], "sources": ["Plugin development forums"]}
EOF
```

- [ ] **Step 3: Create layering files (2 files)**

```bash
python3 scripts/create-kb-template.py sound-design-kb layering parallel-layering.json "Parallel Layering" "Stacking and blending parallel sounds" 8 << 'EOF'
{"search_terms": ["parallel synthesis", "layer blending", "stack sounds", "parallel processing"], "sources": ["Sound on Sound", "Attack Magazine"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb layering serial-layering.json "Serial Layering" "Effect chaining and cascade processing" 7 << 'EOF'
{"search_terms": ["serial processing", "effect chaining", "cascade effects", "feedback layering"], "sources": ["Sound design tutorials"]}
EOF
```

- [ ] **Step 4: Create effects-chain files (2 files)**

```bash
python3 scripts/create-kb-template.py sound-design-kb effects-chain effects-ordering.json "Effects Chain Ordering" "Optimal order for effects in signal chain" 8 << 'EOF'
{"search_terms": ["effects chain order", "EQ before compression", "reverb delay order", "signal flow effects"], "sources": ["Sound on Sound", "mixing tutorials"]}
EOF

python3 scripts/create-kb-template.py sound-design-kb effects-chain parallel-effects.json "Parallel Effects" "Parallel compression and effects routing" 8 << 'EOF'
{"search_terms": ["parallel compression", "parallel reverb", "dry wet mixing", "parallel processing"], "sources": ["Sound on Sound", "mixing blogs"]}
EOF
```

- [ ] **Step 5: Verify sound-design-kb files**

Run: `find sound-design-kb -name "*.json" | wc -l`
Expected: 15 (10 new content files + 2 existing from presets/ + 2 index files)

---

### Task 16: Update sound-design-kb/index.json

**Files:**
- Modify: `sound-design-kb/index.json`

- [ ] **Step 1: Run index update script**

```bash
cd /home/myuser/agents/juce-agent/playbookdata
python3 << 'SCRIPT'
import json

with open('sound-design-kb/index.json', 'r') as f:
    index = json.load(f)

# Add new topics
new_topics = {
    "preset-design": [
        "preset-design/preset-methodology.json",
        "preset-design/macro-controls.json",
        "preset-design/preset-morphing.json",
        "preset-design/sound-design-workflow.json"
    ],
    "modulation-routing": [
        "modulation-routing/modulation-routing.json",
        "modulation-routing/lfo-shapes.json",
        "modulation-routing/envelope-curves.json",
        "modulation-routing/mod-matrix.json"
    ],
    "layering": [
        "layering/parallel-layering.json",
        "layering/serial-layering.json"
    ],
    "effects-chain": [
        "effects-chain/effects-ordering.json",
        "effects-chain/parallel-effects.json"
    ]
}

for topic, files in new_topics.items():
    index["topics"][topic] = files

# Add to categories
new_categories = ["preset-design", "modulation-routing", "layering", "effects-chain"]
for cat in new_categories:
    if cat not in index["metadata"]["categories"]:
        index["metadata"]["categories"].append(cat)

# Update file count (13 existing + 10 new = 23)
index["metadata"]["total_files"] = 23
index["last_updated"] = "2026-03-30T00:00:00Z"

with open('sound-design-kb/index.json', 'w') as f:
    json.dump(index, f, indent=2)

print(f"Updated sound-design-kb/index.json: {len(index['topics'])} topics, {index['metadata']['total_files']} files")
SCRIPT
```

- [ ] **Step 2: Commit sound-design-kb updates**

```bash
git add sound-design-kb/
git commit -m "feat(kb): create sound-design-kb placeholder files with 10 new topics"
```

---

### Task 17: Create juce-kb Placeholder Files (12 files)

**Files:**
- Create: 12 JSON files in juce-kb/

- [ ] **Step 1: Create realtime files (4 files)**

```bash
cd /home/myuser/agents/juce-agent/playbookdata

python3 scripts/create-kb-template.py juce-kb realtime lock-free-queues.json "Lock-Free Queues for Audio" "SPSC queues and real-time safe message passing" 10 << 'EOF'
{"search_terms": ["lock free queue C++", "SPSC queue audio", "moodycamel queue", "real time safe queue"], "sources": ["JUCE forum", "CppCon talks", "CCRMA"]}
EOF

python3 scripts/create-kb-template.py juce-kb realtime allocation-free-patterns.json "Allocation-Free Patterns" "Preallocated memory patterns for audio thread" 10 << 'EOF'
{"search_terms": ["allocation free audio", "preallocated memory", "audio thread allocation", "real time safe"], "sources": ["JUCE forum", "melatonin.dev blog"]}
EOF

python3 scripts/create-kb-template.py juce-kb realtime audio-thread-safety.json "Audio Thread Safety" "Thread safety patterns for real-time audio" 10 << 'EOF'
{"search_terms": ["audio thread safety", "JUCE thread safety", "MessageManagerLock audio", "real time thread"], "sources": ["JUCE forum", "melatonin.dev blog"]}
EOF

python3 scripts/create-kb-template.py juce-kb realtime realtime-profiling.json "Real-Time Profiling" "Profiling CPU usage in audio plugins" 9 << 'EOF'
{"search_terms": ["audio profiler", "real time profiler", "CPU profiler audio", "plugin profiler"], "sources": ["JUCE forum", "Plugin Doctor"]}
EOF
```

- [ ] **Step 2: Create formats files (4 files)**

```bash
python3 scripts/create-kb-template.py juce-kb formats vst3-specifics.json "VST3 Plugin Development" "VST3-specific requirements and patterns" 10 << 'EOF'
{"search_terms": ["VST3 plugin development", "VST3 parameters", "VST3 preset", "Steinberg VST3 SDK"], "sources": ["Steinberg docs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py juce-kb formats au-specifics.json "Audio Unit Development" "AU-specific requirements for macOS" 9 << 'EOF'
{"search_terms": ["Audio Unit development", "AU plugin", "AU parameters", "macOS AU", "AudioUnit framework"], "sources": ["Apple docs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py juce-kb formats aax-considerations.json "AAX Plugin Considerations" "Pro Tools AAX development notes" 8 << 'EOF'
{"search_terms": ["AAX plugin", "Avid AAX SDK", "Pro Tools plugin", "AAX development"], "sources": ["Avid docs", "JUCE forum"]}
EOF

python3 scripts/create-kb-template.py juce-kb formats plugin-sandboxing.json "Plugin Sandboxing" "macOS sandboxing and code signing for plugins" 8 << 'EOF'
{"search_terms": ["macOS sandboxing", "plugin notarization", "code signing audio plugin", "entitlements plugin"], "sources": ["JUCE forum", "Apple docs"]}
EOF
```

- [ ] **Step 3: Create performance files (3 files)**

```bash
python3 scripts/create-kb-template.py juce-kb performance simd-optimization.json "SIMD Optimization for Audio" "Vectorized DSP with SSE/AVX" 10 << 'EOF'
{"search_terms": ["SIMD audio processing", "JUCE dsp SIMD", "vectorized DSP", "SSE AVX audio"], "sources": ["JUCE forum", "DSP blog posts"]}
EOF

python3 scripts/create-kb-template.py juce-kb performance opengl-rendering.json "OpenGL Rendering in JUCE" "GPU-accelerated UI rendering" 8 << 'EOF'
{"search_terms": ["JUCE OpenGL", "OpenGL audio plugin", "GPU rendering UI", "JUCE OpenGL context"], "sources": ["JUCE forum", "melatonin.dev"]}
EOF

python3 scripts/create-kb-template.py juce-kb performance memory-pooling.json "Memory Pooling for Audio" "Object pools and preallocated buffers" 9 << 'EOF'
{"search_terms": ["memory pool audio", "object pool C++", "audio memory management", "preallocated buffers"], "sources": ["JUCE forum", "CCRMA"]}
EOF
```

- [ ] **Step 4: Create state-advanced files (3 files)**

```bash
python3 scripts/create-kb-template.py juce-kb state-advanced apvts-patterns.json "APVTS Patterns" "AudioProcessorValueTreeState best practices" 10 << 'EOF'
{"search_terms": ["JUCE APVTS", "AudioProcessorValueTreeState", "parameter attachment", "state management JUCE"], "sources": ["JUCE forum", "melatonin.dev"]}
EOF

python3 scripts/create-kb-template.py juce-kb state-advanced automation-optimization.json "Automation Optimization" "Efficient DAW automation handling" 9 << 'EOF'
{"search_terms": ["plugin automation", "parameter automation", "DAW automation", "smooth parameters"], "sources": ["JUCE forum"]}
EOF

python3 scripts/create-kb-template.py juce-kb state-advanced preset-performance.json "Preset Loading Performance" "Optimizing preset load and save" 8 << 'EOF'
{"search_terms": ["preset loading performance", "preset browser optimization", "async preset loading"], "sources": ["JUCE forum"]}
EOF
```

- [ ] **Step 5: Verify juce-kb files**

Run: `find juce-kb/realtime juce-kb/formats juce-kb/performance juce-kb/state-advanced -name "*.json" 2>/dev/null | wc -l`
Expected: 12

---

### Task 18: Update juce-kb/index.json

**Files:**
- Modify: `juce-kb/index.json`

- [ ] **Step 1: Run index update script**

```bash
cd /home/myuser/agents/juce-agent/playbookdata
python3 << 'SCRIPT'
import json

with open('juce-kb/index.json', 'r') as f:
    index = json.load(f)

# Add new topics
new_topics = {
    "realtime": [
        "realtime/lock-free-queues.json",
        "realtime/allocation-free-patterns.json",
        "realtime/audio-thread-safety.json",
        "realtime/realtime-profiling.json"
    ],
    "formats": [
        "formats/vst3-specifics.json",
        "formats/au-specifics.json",
        "formats/aax-considerations.json",
        "formats/plugin-sandboxing.json"
    ],
    "performance": [
        "performance/simd-optimization.json",
        "performance/opengl-rendering.json",
        "performance/memory-pooling.json"
    ],
    "state-advanced": [
        "state-advanced/apvts-patterns.json",
        "state-advanced/automation-optimization.json",
        "state-advanced/preset-performance.json"
    ]
}

for topic, files in new_topics.items():
    index["topics"][topic] = files

# Add to categories
new_categories = ["realtime", "formats", "performance", "state-advanced"]
for cat in new_categories:
    if cat not in index["metadata"]["categories"]:
        index["metadata"]["categories"].append(cat)

# Update file count (169 existing + 12 new = 181)
index["metadata"]["total_files"] = 181
index["last_updated"] = "2026-03-30T00:00:00Z"

with open('juce-kb/index.json', 'w') as f:
    json.dump(index, f, indent=2)

print(f"Updated juce-kb/index.json: {len(index['topics'])} topics, {index['metadata']['total_files']} files")
SCRIPT
```

- [ ] **Step 2: Commit juce-kb updates**

```bash
git add juce-kb/
git commit -m "feat(kb): create juce-kb placeholder files with 12 new topics"
```

---

### Task 19: Update master-index.json

**Files:**
- Modify: `master-index.json`

- [ ] **Step 1: Run master-index update script**

```bash
cd /home/myuser/agents/juce-agent/playbookdata
python3 << 'SCRIPT'
import json

with open('master-index.json', 'r') as f:
    index = json.load(f)

# Add testing-kb
index["knowledge_bases"]["testing-kb"] = {
    "path": "testing-kb",
    "description": "Audio plugin testing and validation",
    "source": "curated",
    "status": "ready",
    "file_count": 10,
    "topics": ["unit-testing", "daw-testing", "validation", "regression", "performance-testing"]
}

# Add midi-kb
index["knowledge_bases"]["midi-kb"] = {
    "path": "midi-kb",
    "description": "MIDI protocol and implementation for audio plugins",
    "source": "curated",
    "status": "ready",
    "file_count": 16,
    "topics": ["protocol", "realtime", "mpe", "controllers", "advanced"]
}

# Add new cross-references
new_refs = {
    "reverb": {"description": "Reverb algorithms and design", "kbs": ["dsp-kb"]},
    "compression": {"description": "Dynamic range compression", "kbs": ["dsp-kb"]},
    "chorus": {"description": "Chorus and modulation effects", "kbs": ["dsp-kb"]},
    "pitch_shift": {"description": "Pitch shifting algorithms", "kbs": ["dsp-kb"]},
    "preset_design": {"description": "Preset design methodology", "kbs": ["sound-design-kb"]},
    "modulation_routing": {"description": "Modulation routing and matrices", "kbs": ["sound-design-kb"]},
    "layering": {"description": "Sound layering techniques", "kbs": ["sound-design-kb"]},
    "realtime": {"description": "Real-time audio programming", "kbs": ["juce-kb", "cpp-kb"]},
    "testing": {"description": "Plugin testing", "kbs": ["testing-kb", "juce-kb"]},
    "validation": {"description": "Plugin validation", "kbs": ["testing-kb"]},
    "daw_testing": {"description": "DAW-specific testing", "kbs": ["testing-kb"]},
    "midi": {"description": "MIDI protocol and implementation", "kbs": ["midi-kb", "juce-kb"]},
    "mpe": {"description": "MIDI Polyphonic Expression", "kbs": ["midi-kb"]},
    "midi_learn": {"description": "MIDI learn and mapping", "kbs": ["midi-kb", "juce-kb"]},
    "midi_timing": {"description": "MIDI timing and jitter", "kbs": ["midi-kb"]},
    "controllers": {"description": "MIDI controller integration", "kbs": ["midi-kb"]}
}

for key, val in new_refs.items():
    if key not in index["cross_references"]:
        index["cross_references"][key] = val

# Update timestamp
index["last_updated"] = "2026-03-30T00:00:00Z"

with open('master-index.json', 'w') as f:
    json.dump(index, f, indent=2)

print(f"Updated master-index.json: {len(index['knowledge_bases'])} KBs, {len(index['cross_references'])} cross-refs")
SCRIPT
```

- [ ] **Step 2: Verify master-index**

Run: `python3 -c "import json; d=json.load(open('master-index.json')); print('KBs:', list(d['knowledge_bases'].keys())); print('testing-kb:', 'testing-kb' in d['knowledge_bases']); print('midi-kb:', 'midi-kb' in d['knowledge_bases'])"`
Expected: Both testing-kb and midi-kb present

- [ ] **Step 3: Commit master-index**

```bash
git add master-index.json
git commit -m "feat(kb): add testing-kb and midi-kb to master index"
```

---

### Task 20: Create Validation Script

**Files:**
- Create: `scripts/validate-kb-structure.py`

- [ ] **Step 1: Write validation script**

```python
#!/usr/bin/env python3
"""Validate KB structure: check for orphan files, missing files, index consistency."""

import json
import os
import sys
from pathlib import Path

def validate_kb(kb_path):
    """Validate a KB directory."""
    errors = []

    # Load index
    index_path = os.path.join(kb_path, "index.json")
    if not os.path.exists(index_path):
        return [f"Missing index.json in {kb_path}"]

    try:
        with open(index_path) as f:
            index = json.load(f)
    except json.JSONDecodeError as e:
        return [f"Invalid JSON in {kb_path}/index.json: {e}"]

    # Get all indexed files
    indexed_files = set()
    for topic, files in index.get("topics", {}).items():
        for file in files:
            indexed_files.add(file)

    # Get all actual files
    actual_files = set()
    for root, dirs, files in os.walk(kb_path):
        for f in files:
            if f.endswith(".json") and f not in ["index.json", "manifest.json", "validation.json"]:
                rel = os.path.relpath(os.path.join(root, f), kb_path)
                actual_files.add(rel)

    # Check for orphans
    orphans = actual_files - indexed_files
    if orphans:
        errors.append(f"Orphan files in {kb_path}: {orphans}")

    # Check for missing
    missing = indexed_files - actual_files
    if missing:
        errors.append(f"Missing files in {kb_path}: {missing}")

    # Check file count
    metadata_count = index.get("metadata", {}).get("total_files", 0)
    if metadata_count != len(actual_files):
        errors.append(f"File count mismatch in {kb_path}: index says {metadata_count}, actual {len(actual_files)}")

    return errors

def main():
    kb_dirs = ["juce-kb", "dsp-kb", "sound-design-kb", "ui-kb", "cpp-kb", "cmake-kb", "testing-kb", "midi-kb"]

    all_errors = []
    for kb in kb_dirs:
        if os.path.exists(kb):
            errors = validate_kb(kb)
            all_errors.extend(errors)

    if all_errors:
        print("VALIDATION ERRORS:")
        for e in all_errors:
            print(f"  - {e}")
        sys.exit(1)
    else:
        print("All KBs valid!")
        sys.exit(0)

if __name__ == "__main__":
    main()
```

- [ ] **Step 2: Make script executable**

Run: `chmod +x scripts/validate-kb-structure.py`

- [ ] **Step 3: Run validation**

Run: `python3 scripts/validate-kb-structure.py`
Expected: "All KBs valid!"

- [ ] **Step 4: Commit validation script**

```bash
git add scripts/validate-kb-structure.py
git commit -m "feat(kb): add KB structure validation script"
```

---

### Task 21: Final Verification

- [ ] **Step 1: Count all new files**

Run:
```bash
echo "testing-kb: $(find testing-kb -name '*.json' | grep -v index.json | grep -v manifest.json | wc -l) files"
echo "midi-kb: $(find midi-kb -name '*.json' | grep -v index.json | grep -v manifest.json | wc -l) files"
echo "dsp-kb new: $(ls dsp-kb/{reverb,dynamics,modulation,delay,distortion,pitch,spatial}/*.json 2>/dev/null | wc -l) files"
echo "sound-design-kb new: $(ls sound-design-kb/{preset-design,modulation-routing,layering,effects-chain}/*.json 2>/dev/null | wc -l) files"
echo "juce-kb new: $(ls juce-kb/{realtime,formats,performance,state-advanced}/*.json 2>/dev/null | wc -l) files"
```

Expected:
- testing-kb: 10 files
- midi-kb: 16 files
- dsp-kb new: 26 files
- sound-design-kb new: 10 files
- juce-kb new: 12 files
- **Total: 74 files** (63 new content + 10 existing merged + 1 correction)

- [ ] **Step 2: Verify all indexes valid**

Run: `for kb in juce-kb dsp-kb sound-design-kb testing-kb midi-kb; do echo "$kb:"; python3 -c "import json; json.load(open('$kb/index.json')); print('  Valid JSON')"; done`
Expected: All valid JSON

- [ ] **Step 3: Final commit**

```bash
git status
git add -A
git commit -m "feat(kb): complete KB expansion with 63 new topics across 5 KBs"
```

---

## Success Criteria Checklist

- [ ] testing-kb created with 10 files + index + manifest
- [ ] midi-kb created with 16 files + index + manifest
- [ ] dsp-kb expanded with 26 new files (merged with existing topics)
- [ ] sound-design-kb expanded with 10 new files
- [ ] juce-kb expanded with 12 new files
- [ ] master-index.json updated with testing-kb and midi-kb
- [ ] master-index.json has new cross-references
- [ ] All KB index.json files updated correctly
- [ ] Validation script passes (no orphan or missing files)
- [ ] File counts match index metadata
- [ ] No existing data lost (dsp-kb merge verified)

---

## File Count Summary

| KB | Existing | New | Total |
|----|----------|-----|-------|
| dsp-kb | 27 | 26 | 53 |
| sound-design-kb | 13 | 10 | 23 |
| juce-kb | 169 | 12 | 181 |
| testing-kb | 0 | 10 | 10 |
| midi-kb | 0 | 16 | 16 |
| **Total** | **209** | **74** | **283** |

Note: dsp-kb shows 27 existing + 26 new = 53, but some new files merge into existing topics, so the actual count reflects proper merging.

---

## Appendix: Verification Commands

```bash
# Verify directory structure
find . -type d -name "testing-kb" -o -name "midi-kb" -o -name "reverb" -o -name "modulation"

# Verify file counts
for kb in testing-kb midi-kb dsp-kb sound-design-kb juce-kb; do
    echo "$kb: $(find $kb -name '*.json' | grep -v index.json | grep -v manifest.json | wc -l)"
done

# Run validation
python3 scripts/validate-kb-structure.py

# Check git status
git status
```