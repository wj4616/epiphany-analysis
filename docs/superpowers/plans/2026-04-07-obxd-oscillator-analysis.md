# OB-Xd Oscillator Analysis Methodology — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the runnable methodology specified in `docs/superpowers/specs/2026-04-07-obxd-oscillator-analysis-design.md` so a Claude Code orchestrator session can execute the 10-stage OB-Xd oscillator analysis pipeline.

**Architecture:** A Python-validated, markdown-driven methodology package. Two small Python validators (pytest-tested) enforce pre-flight checks and Stage 1 reuse logic. YAML configs hold the OB-Xd target. 13 markdown schemas describe artifact shapes. 14 markdown prompt templates drive subagents. A markdown runbook tells the orchestrator (the main Claude session) what to do step by step. Setup and commit helpers are bash. The analysis output goes to a separate git repo at `~/synth-research/OB-Xd-oscillator-analysis/`, populated stage-by-stage at runtime.

**Tech Stack:** Python 3 + pytest + PyYAML for validators; Bash for setup/commit helpers; Markdown for schemas, prompts, and runbook; Git for per-stage checkpointing.

---

## File Structure

The methodology package lives at `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/`. The analysis runtime output lives at `~/synth-research/OB-Xd-oscillator-analysis/` (separate git repo, created by the setup script).

```
~/agents/juce-agent/methodologies/obxd-oscillator-analysis/
├── README.md                           # how to run the methodology
├── config/
│   ├── config.yaml                     # OB-Xd target config
│   └── source-map.yaml                 # OB-Xd file mapping
├── orchestrator/
│   ├── runbook.md                      # step-by-step the Claude session follows
│   ├── preflight.py                    # config + filesystem validators
│   ├── repo_validate.py                # Stage 1 reuse check
│   ├── setup_workdir.sh                # one-time working-dir + git init
│   └── stage_commit.sh                 # per-stage git commit helper
├── schemas/
│   ├── 01-repo-map.schema.md
│   ├── 02-entry-trace.schema.md
│   ├── 03-waveform.schema.md           # template for 3a/3b/3c
│   ├── 04-composite.schema.md          # 3d
│   ├── 05-voice-alloc.schema.md        # 4a
│   ├── 06-voice-routing.schema.md      # 4b
│   ├── 07-voice-wrapper.schema.md      # 4c
│   ├── 08-antialias.schema.md          # 5
│   ├── 09-analog-character.schema.md   # 6
│   ├── 10-parameters.schema.md         # 7
│   ├── 11-adjacent.schema.md           # 8
│   ├── 12-review.schema.md             # 9
│   └── 13-final-spec.schema.md         # 10 (the deliverable's shape)
├── prompts/
│   ├── _common.md                      # shared contract block
│   ├── stage-01-repo-map.md
│   ├── stage-02-entry-trace.md
│   ├── stage-03-waveform.md            # parameterized for saw/pulse/triangle
│   ├── stage-03d-composite.md
│   ├── stage-04a-voice-alloc.md
│   ├── stage-04b-voice-routing.md
│   ├── stage-04c-voice-wrapper.md
│   ├── stage-05-antialias.md
│   ├── stage-06-analog-character.md
│   ├── stage-07-parameters.md
│   ├── stage-08-adjacent.md
│   ├── stage-09-reviewer.md
│   └── stage-10-synthesis.md
└── tests/
    ├── conftest.py
    ├── test_preflight.py
    └── test_repo_validate.py
```

Working dir (created at runtime, populated stage-by-stage):
```
~/synth-research/OB-Xd-oscillator-analysis/   # its own git repo
├── 01-repo-map.md
├── 02-entry-trace.md
├── 03a-saw.md, 03b-pulse.md, 03c-triangle.md, 03d-osc-composite.md
├── 04a-voice-alloc.md, 04b-voice-routing.md, 04c-voice-wrapper.md
├── 05-antialias.md, 06-analog-character.md, 07-parameters.md, 08-adjacent.md
├── 09-review/<one file per extraction artifact>
├── 10-final-spec.md
└── BLOCKERS.md
```

---

## Phase 1 — Scaffolding

### Task 1: Create project skeleton

**Files:**
- Create: `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/` (and subdirs)
- Create: `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/README.md` (placeholder)

- [ ] **Step 1: Create directory tree**

```bash
ROOT=~/agents/juce-agent/methodologies/obxd-oscillator-analysis
mkdir -p "$ROOT"/{config,orchestrator,schemas,prompts,tests}
```

- [ ] **Step 2: Add placeholder README**

Create `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/README.md`:

```markdown
# OB-Xd Oscillator Analysis Methodology

Runnable methodology that extracts OB-Xd 2.19's oscillator design into a portable spec.
See `orchestrator/runbook.md` for usage. Design doc: `docs/superpowers/specs/2026-04-07-obxd-oscillator-analysis-design.md`.

Dependencies: `pip install pyyaml pytest`
```

- [ ] **Step 3: Verify structure**

Run: `find ~/agents/juce-agent/methodologies/obxd-oscillator-analysis -type d`
Expected: 6 directories listed (root + 5 subdirs).

- [ ] **Step 4: Commit**

```bash
cd ~/agents/juce-agent
git add methodologies/obxd-oscillator-analysis/
git commit -m "scaffold(obxd-analysis): create methodology package skeleton"
```

---

## Phase 2 — Configuration

### Task 2: Write config.yaml (OB-Xd target)

**Files:**
- Create: `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/config/config.yaml`

- [ ] **Step 1: Write config.yaml**

```yaml
target_repo_path:        ~/synth/OB-Xd-2.19
target_synth_name:       OB-Xd
target_synth_version:    "2.19"
target_synth_license:    GPLv2
working_dir:             ~/synth-research/OB-Xd-oscillator-analysis/
git_track_artifacts:     true
default_voice_count:     8
default_oscs_per_voice:  2
worked_example_target:   "4 voices, 2 osc/voice"
source_map:              ./source-map.yaml

enabled_stages:
  - 1_repo_map
  - 2_entry_trace
  - 3a_saw
  - 3b_pulse
  - 3c_triangle
  - 3d_composite
  - 4a_voice_alloc
  - 4b_voice_routing
  - 4c_voice_wrapper
  - 5_antialias
  - 6_analog_character
  - 7_parameters
  - 8_adjacent
  - 9_review
  - 10_synthesis
```

- [ ] **Step 2: Verify YAML parses**

Run: `python -c "import yaml; print(yaml.safe_load(open('config/config.yaml')))" `
Expected: prints a dict with all keys, no errors.

- [ ] **Step 3: Commit**

```bash
git add methodologies/obxd-oscillator-analysis/config/config.yaml
git commit -m "config(obxd-analysis): add OB-Xd target config"
```

---

### Task 3: Write source-map.yaml (OB-Xd target)

**Files:**
- Create: `~/agents/juce-agent/methodologies/obxd-oscillator-analysis/config/source-map.yaml`

- [ ] **Step 1: Write source-map.yaml**

```yaml
# Maps each pipeline stage to source files relative to config.yaml's target_repo_path.
# Stage YAML keys are lowercase snake_case versions of the design doc's stage labels.

read_declaration_only:
  - Source/Engine/BlepData.h

overrides: {}    # no overrides for OB-Xd; defaults from spec Section 2.2 apply

stages:
  1_repo_map:
    inputs: [Source/Engine/]
  2_entry_trace:
    inputs:
      - Source/Engine/ObxdOscillatorB.h
      - Source/Engine/ObxdVoice.h
  3a_saw:
    inputs: [Source/Engine/SawOsc.h]
  3b_pulse:
    inputs: [Source/Engine/PulseOsc.h]
  3c_triangle:
    inputs: [Source/Engine/TriangleOsc.h]
  3d_composite:
    inputs: [Source/Engine/ObxdOscillatorB.h]
  4a_voice_alloc:
    inputs:
      - Source/Engine/Motherboard.h
      - Source/Engine/VoiceQueue.h
  4b_voice_routing:
    inputs: [Source/Engine/SynthEngine.h]
  4c_voice_wrapper:
    inputs: [Source/Engine/ObxdVoice.h]
  5_antialias:
    inputs:
      - Source/Engine/SawOsc.h
      - Source/Engine/PulseOsc.h
      - Source/Engine/TriangleOsc.h
      - Source/Engine/Decimator.h
      - Source/Engine/BlepData.h
  7_parameters:
    inputs:
      - Source/Engine/Params.h
      - Source/Engine/ParamsEnum.h
      - Source/Engine/Tuning.h
      - Source/Engine/midiMap.h
  8_adjacent:
    inputs:
      - Source/Engine/Filter.h
      - Source/Engine/AdsrEnvelope.h
      - Source/Engine/Lfo.h
      - Source/Engine/AudioUtils.h
```

- [ ] **Step 2: Verify YAML parses**

Run: `python -c "import yaml; d = yaml.safe_load(open('config/source-map.yaml')); print(list(d['stages'].keys()))"`
Expected: prints a list of 13 stage keys (stages 6, 9, 10 are not in source-map by design).

- [ ] **Step 3: Commit**

```bash
git add methodologies/obxd-oscillator-analysis/config/source-map.yaml
git commit -m "config(obxd-analysis): add OB-Xd source-map.yaml"
```

---

## Phase 3 — Pre-flight Validator (TDD)

### Task 4: Test and implement config loading

**Files:**
- Create: `tests/conftest.py`
- Create: `tests/test_preflight.py`
- Create: `orchestrator/preflight.py`

- [ ] **Step 1: Write conftest.py with shared fixtures**

```python
import pathlib
import pytest

REPO_ROOT = pathlib.Path(__file__).parent.parent
CONFIG_DIR = REPO_ROOT / "config"

@pytest.fixture
def config_path():
    return CONFIG_DIR / "config.yaml"

@pytest.fixture
def source_map_path():
    return CONFIG_DIR / "source-map.yaml"
```

- [ ] **Step 2: Write the failing test for config loading**

Add to `tests/test_preflight.py`:

```python
from orchestrator import preflight

def test_load_config_returns_dict(config_path):
    cfg = preflight.load_config(config_path)
    assert cfg["target_synth_name"] == "OB-Xd"
    assert cfg["default_voice_count"] == 8
    assert "1_repo_map" in cfg["enabled_stages"]

def test_load_source_map_returns_dict(source_map_path):
    sm = preflight.load_source_map(source_map_path)
    assert "Source/Engine/BlepData.h" in sm["read_declaration_only"]
    assert "3a_saw" in sm["stages"]
    assert sm["stages"]["3a_saw"]["inputs"] == ["Source/Engine/SawOsc.h"]
```

- [ ] **Step 3: Run test, verify it fails**

Run: `cd ~/agents/juce-agent/methodologies/obxd-oscillator-analysis && python -m pytest tests/test_preflight.py -v`
Expected: FAIL with `ModuleNotFoundError: No module named 'orchestrator'` or `AttributeError: load_config`.

- [ ] **Step 4: Implement preflight.py loaders**

Create `orchestrator/preflight.py`:

```python
"""Pre-flight validator for the OB-Xd oscillator analysis methodology.

Loads config and source-map, validates required stages and filesystem paths.
Run from CLI: python -m orchestrator.preflight config/config.yaml
"""
from __future__ import annotations
import pathlib
import sys
import yaml


def load_config(path: pathlib.Path) -> dict:
    """Load config.yaml and return as dict."""
    with open(path) as f:
        return yaml.safe_load(f)


def load_source_map(path: pathlib.Path) -> dict:
    """Load source-map.yaml and return as dict."""
    with open(path) as f:
        return yaml.safe_load(f)
```

Also add an empty `__init__.py`:

```bash
touch orchestrator/__init__.py
touch tests/__init__.py
```

- [ ] **Step 5: Run test, verify it passes**

Run: `python -m pytest tests/test_preflight.py -v`
Expected: 2 passed.

- [ ] **Step 6: Commit**

```bash
git add orchestrator/__init__.py orchestrator/preflight.py tests/__init__.py tests/conftest.py tests/test_preflight.py
git commit -m "feat(obxd-analysis): add config + source-map loaders with tests"
```

---

### Task 5: Test and implement required-stage validation

**Files:**
- Modify: `orchestrator/preflight.py`
- Modify: `tests/test_preflight.py`

- [ ] **Step 1: Write the failing tests**

Append to `tests/test_preflight.py`:

```python
# Required-stage table from design doc Section 4.3
REQUIRED_ALWAYS = {"1_repo_map", "2_entry_trace", "3d_composite", "7_parameters", "10_synthesis"}
WAVEFORM_GROUP = {"3a_saw", "3b_pulse", "3c_triangle"}
VOICE_GROUP = {"4a_voice_alloc", "4b_voice_routing", "4c_voice_wrapper"}


def test_validate_required_stages_passes_on_obxd(config_path, source_map_path):
    cfg = preflight.load_config(config_path)
    sm = preflight.load_source_map(source_map_path)
    errors = preflight.validate_required_stages(cfg, sm)
    assert errors == []


def test_validate_required_stages_fails_on_missing_required():
    cfg = {"enabled_stages": ["1_repo_map", "2_entry_trace"]}  # missing 3d, 7, 10, etc.
    sm = {"stages": {}}
    errors = preflight.validate_required_stages(cfg, sm)
    assert any("3d_composite" in e for e in errors)
    assert any("7_parameters" in e for e in errors)
    assert any("10_synthesis" in e for e in errors)


def test_validate_required_stages_fails_on_no_waveform():
    cfg = {"enabled_stages": list(REQUIRED_ALWAYS) + list(VOICE_GROUP) + ["9_review"]}
    sm = {"stages": {k: {"inputs": []} for k in cfg["enabled_stages"] if k not in {"9_review", "10_synthesis"}}}
    errors = preflight.validate_required_stages(cfg, sm)
    assert any("waveform" in e.lower() for e in errors)


def test_validate_required_stages_fails_on_no_voice_management():
    cfg = {"enabled_stages": list(REQUIRED_ALWAYS) + list(WAVEFORM_GROUP) + ["9_review"]}
    sm = {"stages": {k: {"inputs": []} for k in cfg["enabled_stages"] if k not in {"9_review", "10_synthesis"}}}
    errors = preflight.validate_required_stages(cfg, sm)
    assert any("voice management" in e.lower() for e in errors)
```

- [ ] **Step 2: Run tests, verify they fail**

Run: `python -m pytest tests/test_preflight.py::test_validate_required_stages_passes_on_obxd -v`
Expected: FAIL with `AttributeError: validate_required_stages`.

- [ ] **Step 3: Implement validate_required_stages**

Append to `orchestrator/preflight.py`:

```python
# Required-stage table — design doc Section 4.3 is canonical.
REQUIRED_ALWAYS = {
    "1_repo_map",
    "2_entry_trace",
    "3d_composite",
    "7_parameters",
    "10_synthesis",
}
WAVEFORM_GROUP = {"3a_saw", "3b_pulse", "3c_triangle"}
VOICE_GROUP = {"4a_voice_alloc", "4b_voice_routing", "4c_voice_wrapper"}


def validate_required_stages(cfg: dict, sm: dict) -> list[str]:
    """Return a list of error strings; empty list means valid."""
    errors: list[str] = []
    enabled = set(cfg.get("enabled_stages", []))

    # Always-required stages must be in enabled_stages
    for stage in REQUIRED_ALWAYS:
        if stage not in enabled:
            errors.append(f"required stage missing from enabled_stages: {stage}")

    # At least one waveform stage must be enabled
    if not (WAVEFORM_GROUP & enabled):
        errors.append(
            "at least one waveform stage required (3a_saw, 3b_pulse, 3c_triangle)"
        )

    # At least one voice-management stage must be enabled
    if not (VOICE_GROUP & enabled):
        errors.append(
            "at least one voice management stage required (4a_voice_alloc, 4b_voice_routing, 4c_voice_wrapper)"
        )

    return errors
```

- [ ] **Step 4: Run tests, verify they pass**

Run: `python -m pytest tests/test_preflight.py -v`
Expected: 6 passed (the 2 from Task 4 + 4 new ones).

- [ ] **Step 5: Commit**

```bash
git add orchestrator/preflight.py tests/test_preflight.py
git commit -m "feat(obxd-analysis): add required-stage validation"
```

---

### Task 6: Test and implement filesystem path validation

**Files:**
- Modify: `orchestrator/preflight.py`
- Modify: `tests/test_preflight.py`

- [ ] **Step 1: Write the failing tests**

Append to `tests/test_preflight.py`:

```python
def test_validate_filesystem_paths_passes_on_obxd(config_path, source_map_path, tmp_path, monkeypatch):
    # Skip if OB-Xd source isn't checked out
    obxd_root = pathlib.Path("~/synth/OB-Xd-2.19").expanduser()
    if not obxd_root.exists():
        pytest.skip("OB-Xd source not present")
    cfg = preflight.load_config(config_path)
    sm = preflight.load_source_map(source_map_path)
    errors = preflight.validate_filesystem_paths(cfg, sm)
    assert errors == []


def test_validate_filesystem_paths_fails_on_missing_file(tmp_path):
    cfg = {"target_repo_path": str(tmp_path)}
    sm = {"stages": {"3a_saw": {"inputs": ["does_not_exist.h"]}}}
    errors = preflight.validate_filesystem_paths(cfg, sm)
    assert len(errors) == 1
    assert "does_not_exist.h" in errors[0]


def test_validate_filesystem_paths_passes_on_existing_file(tmp_path):
    (tmp_path / "fake.h").write_text("// stub\n")
    cfg = {"target_repo_path": str(tmp_path)}
    sm = {"stages": {"3a_saw": {"inputs": ["fake.h"]}}}
    errors = preflight.validate_filesystem_paths(cfg, sm)
    assert errors == []
```

Note: import `pytest` at the top of `test_preflight.py` if not already.

- [ ] **Step 2: Run tests, verify they fail**

Run: `python -m pytest tests/test_preflight.py -v -k filesystem`
Expected: FAIL with `AttributeError: validate_filesystem_paths`.

- [ ] **Step 3: Implement validate_filesystem_paths**

Append to `orchestrator/preflight.py`:

```python
def validate_filesystem_paths(cfg: dict, sm: dict) -> list[str]:
    """Verify every source path declared in source-map exists on disk.

    Paths are resolved relative to cfg['target_repo_path'].
    Returns a list of error strings; empty list means all paths exist.
    """
    errors: list[str] = []
    repo_root = pathlib.Path(cfg["target_repo_path"]).expanduser()

    for stage_key, stage_def in sm.get("stages", {}).items():
        for rel_path in stage_def.get("inputs", []):
            # A directory entry (Stage 1) is OK; just check it exists
            full = repo_root / rel_path
            if not full.exists():
                errors.append(
                    f"stage {stage_key}: source path does not exist: {full}"
                )
    return errors
```

- [ ] **Step 4: Run tests, verify they pass**

Run: `python -m pytest tests/test_preflight.py -v`
Expected: 9 passed (or 8 passed + 1 skipped if OB-Xd isn't checked out).

- [ ] **Step 5: Commit**

```bash
git add orchestrator/preflight.py tests/test_preflight.py
git commit -m "feat(obxd-analysis): add filesystem path validation"
```

---

### Task 7: Pre-flight CLI entry point

**Files:**
- Modify: `orchestrator/preflight.py`

- [ ] **Step 1: Write the failing test**

Append to `tests/test_preflight.py`:

```python
def test_main_returns_zero_on_valid_config(config_path, source_map_path, capsys):
    obxd_root = pathlib.Path("~/synth/OB-Xd-2.19").expanduser()
    if not obxd_root.exists():
        pytest.skip("OB-Xd source not present")
    rc = preflight.main([str(config_path)])
    assert rc == 0


def test_main_returns_nonzero_on_missing_required_stage(tmp_path, capsys):
    cfg_path = tmp_path / "config.yaml"
    sm_path = tmp_path / "source-map.yaml"
    cfg_path.write_text(
        "target_repo_path: " + str(tmp_path) + "\n"
        "source_map: ./source-map.yaml\n"
        "enabled_stages: [1_repo_map]\n"
    )
    sm_path.write_text("stages: {}\n")
    rc = preflight.main([str(cfg_path)])
    assert rc != 0
    captured = capsys.readouterr()
    assert "required stage missing" in captured.err.lower() or "required stage missing" in captured.out.lower()
```

- [ ] **Step 2: Run test, verify it fails**

Run: `python -m pytest tests/test_preflight.py::test_main_returns_nonzero_on_missing_required_stage -v`
Expected: FAIL with `AttributeError: main`.

- [ ] **Step 3: Implement main()**

Append to `orchestrator/preflight.py`:

```python
def main(argv: list[str] | None = None) -> int:
    """CLI entry point. Returns 0 on success, 1 on validation errors.

    Usage: python -m orchestrator.preflight path/to/config.yaml
    """
    argv = argv if argv is not None else sys.argv[1:]
    if len(argv) != 1:
        print("usage: python -m orchestrator.preflight <config.yaml>", file=sys.stderr)
        return 2

    config_path = pathlib.Path(argv[0])
    cfg = load_config(config_path)
    sm_path = (config_path.parent / cfg["source_map"]).resolve()
    sm = load_source_map(sm_path)

    all_errors: list[str] = []
    all_errors.extend(validate_required_stages(cfg, sm))
    all_errors.extend(validate_filesystem_paths(cfg, sm))

    if all_errors:
        print("PRE-FLIGHT FAILED:", file=sys.stderr)
        for e in all_errors:
            print(f"  - {e}", file=sys.stderr)
        return 1

    print("pre-flight OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

- [ ] **Step 4: Run all tests, verify they pass**

Run: `python -m pytest tests/test_preflight.py -v`
Expected: 11 passed (or 9 passed + 2 skipped).

- [ ] **Step 5: Smoke-run from CLI**

Run: `python -m orchestrator.preflight config/config.yaml`
Expected: prints `pre-flight OK` and exits 0 (assuming OB-Xd source is present), or prints clear errors and exits 1 otherwise.

- [ ] **Step 6: Commit**

```bash
git add orchestrator/preflight.py tests/test_preflight.py
git commit -m "feat(obxd-analysis): add pre-flight CLI entry point"
```

---

## Phase 4 — Stage 1 Reuse Validator

### Task 8: Test and implement repo_validate.py

**Files:**
- Create: `orchestrator/repo_validate.py`
- Create: `tests/test_repo_validate.py`

- [ ] **Step 1: Write the failing tests**

Create `tests/test_repo_validate.py`:

```python
import pathlib
import pytest
from orchestrator import repo_validate


def test_enumerate_repo_files_returns_path_line_tuples(tmp_path):
    (tmp_path / "a.h").write_text("line1\nline2\nline3\n")
    (tmp_path / "b.h").write_text("only one line\n")
    files = repo_validate.enumerate_files(tmp_path)
    assert ("a.h", 3) in files
    assert ("b.h", 1) in files
    assert len(files) == 2


def test_parse_repo_map_returns_path_line_tuples(tmp_path):
    repo_map = tmp_path / "01-repo-map.md"
    repo_map.write_text(
        "# Repo Map\n\n"
        "## Files\n\n"
        "| File | Lines | Role |\n"
        "|---|---|---|\n"
        "| Source/Engine/SawOsc.h | 128 | saw waveform generator |\n"
        "| Source/Engine/PulseOsc.h | 194 | pulse waveform generator |\n"
    )
    parsed = repo_validate.parse_repo_map(repo_map)
    assert ("Source/Engine/SawOsc.h", 128) in parsed
    assert ("Source/Engine/PulseOsc.h", 194) in parsed


def test_validate_reuse_passes_when_unchanged(tmp_path):
    src = tmp_path / "src"
    src.mkdir()
    (src / "a.h").write_text("a\nb\nc\n")
    repo_map = tmp_path / "01-repo-map.md"
    repo_map.write_text(
        "## Files\n\n"
        "| File | Lines | Role |\n"
        "|---|---|---|\n"
        "| a.h | 3 | example |\n"
    )
    result = repo_validate.validate_reuse(repo_map, src)
    assert result.reusable is True
    assert result.reasons == []


def test_validate_reuse_fails_on_added_file(tmp_path):
    src = tmp_path / "src"
    src.mkdir()
    (src / "a.h").write_text("a\n")
    (src / "b.h").write_text("b\n")  # not in repo map
    repo_map = tmp_path / "01-repo-map.md"
    repo_map.write_text(
        "## Files\n\n"
        "| File | Lines | Role |\n"
        "|---|---|---|\n"
        "| a.h | 1 | example |\n"
    )
    result = repo_validate.validate_reuse(repo_map, src)
    assert result.reusable is False
    assert any("b.h" in r for r in result.reasons)


def test_validate_reuse_fails_on_changed_line_count(tmp_path):
    src = tmp_path / "src"
    src.mkdir()
    (src / "a.h").write_text("a\nb\n")  # 2 lines
    repo_map = tmp_path / "01-repo-map.md"
    repo_map.write_text(
        "## Files\n\n"
        "| File | Lines | Role |\n"
        "|---|---|---|\n"
        "| a.h | 1 | example |\n"  # claims 1 line
    )
    result = repo_validate.validate_reuse(repo_map, src)
    assert result.reusable is False
    assert any("line count" in r.lower() for r in result.reasons)
```

- [ ] **Step 2: Run tests, verify they fail**

Run: `python -m pytest tests/test_repo_validate.py -v`
Expected: FAIL with `ModuleNotFoundError: orchestrator.repo_validate`.

- [ ] **Step 3: Implement repo_validate.py**

Create `orchestrator/repo_validate.py`:

```python
"""Stage 1 reuse validator.

Decides whether an existing 01-repo-map.md artifact in the working dir can be
reused for a new run, by comparing its file list against the current target
repo state. Per design doc Section 4.4 step 7.
"""
from __future__ import annotations
import dataclasses
import pathlib
import re


@dataclasses.dataclass
class ReuseResult:
    reusable: bool
    reasons: list[str]


def enumerate_files(repo_root: pathlib.Path) -> list[tuple[str, int]]:
    """Walk repo_root recursively, return list of (relative_path, line_count) tuples.

    Only counts regular files (skips directories and symlinks).
    """
    out: list[tuple[str, int]] = []
    for p in sorted(repo_root.rglob("*")):
        if not p.is_file() or p.is_symlink():
            continue
        rel = str(p.relative_to(repo_root))
        try:
            n_lines = sum(1 for _ in p.open("rb"))
        except OSError:
            continue
        out.append((rel, n_lines))
    return out


# Match a markdown table row like "| Source/Engine/SawOsc.h | 128 | role text |"
_ROW_RE = re.compile(r"^\|\s*([^|]+?)\s*\|\s*(\d+)\s*\|.*\|\s*$")


def parse_repo_map(repo_map_path: pathlib.Path) -> list[tuple[str, int]]:
    """Parse the markdown table from a 01-repo-map.md artifact.

    Returns list of (file_path, line_count) tuples. Only rows where the second
    column is an integer are accepted; the header row is naturally skipped.
    """
    out: list[tuple[str, int]] = []
    for line in repo_map_path.read_text().splitlines():
        m = _ROW_RE.match(line)
        if not m:
            continue
        path, lines = m.group(1), int(m.group(2))
        out.append((path, lines))
    return out


def validate_reuse(repo_map_path: pathlib.Path, repo_root: pathlib.Path) -> ReuseResult:
    """Compare current repo state to recorded repo map. See design 4.4 step 7."""
    recorded = dict(parse_repo_map(repo_map_path))
    current = dict(enumerate_files(repo_root))

    reasons: list[str] = []

    # Files present now but not recorded
    for path in sorted(set(current) - set(recorded)):
        reasons.append(f"file added since repo map was written: {path}")

    # Files recorded but missing now
    for path in sorted(set(recorded) - set(current)):
        reasons.append(f"file removed since repo map was written: {path}")

    # Line count mismatches on common files
    for path in sorted(set(recorded) & set(current)):
        if recorded[path] != current[path]:
            reasons.append(
                f"line count changed for {path}: recorded {recorded[path]}, now {current[path]}"
            )

    return ReuseResult(reusable=(not reasons), reasons=reasons)
```

- [ ] **Step 4: Run tests, verify they pass**

Run: `python -m pytest tests/test_repo_validate.py -v`
Expected: 5 passed.

- [ ] **Step 5: Commit**

```bash
git add orchestrator/repo_validate.py tests/test_repo_validate.py
git commit -m "feat(obxd-analysis): add Stage 1 reuse validator"
```

---

## Phase 5 — Artifact Schemas

Each schema is a markdown template that the corresponding extraction subagent fills out. Schemas live in `schemas/` and are referenced by prompts in Phase 6.

### Task 9: Schemas 01 (repo map) and 02 (entry trace)

**Files:**
- Create: `schemas/01-repo-map.schema.md`
- Create: `schemas/02-entry-trace.schema.md`

- [ ] **Step 1: Write schema 01**

Create `schemas/01-repo-map.schema.md`:

```markdown
# Repo Map — {TARGET_SYNTH_NAME} {TARGET_VERSION}

## Source root
- Path: <abs path to scanned directory>
- Scanned at: <commit hash if git, otherwise mtime>

## Files

| File | Lines | Role |
|---|---|---|
| <relative path> | <n> | <one-line role description> |

## Files flagged READ-DECLARATION-ONLY (>1500 lines)

- <path>: <n> lines — <reason this file should be read by declaration only>

## Inferred role labels

- Per-voice oscillator entry: <file>
- Voice wrapper: <file>
- Voice allocator: <file>
- Synth engine / voice driver: <file>
- Saw waveform generator: <file or "not present">
- Pulse waveform generator: <file or "not present">
- Triangle waveform generator: <file or "not present">
- Anti-alias support files: <list or "not present">
- Filter: <file or "not present">
- Envelope: <file or "not present">
- LFO: <file or "not present">
- Parameters / param enum: <files>
- Tuning system: <file or "not present">
- MIDI map: <file or "not present">

## Open Questions
- <anything ambiguous about role mapping>
```

- [ ] **Step 2: Write schema 02**

Create `schemas/02-entry-trace.schema.md`:

```markdown
# Entry Trace — {TARGET_SYNTH_NAME}

## Per-voice oscillator block
- Class name: <name>
- File: <path>:<line>
- Public method called per sample: <method signature>
- What it returns: <type, units, range>

## Voice wrapper
- Class name: <name>
- File: <path>:<line>
- Owns the oscillator block at: <member name>
- Per-voice render loop entry: <method signature, file:line>

## Where the per-voice oscillator block is instantiated
- File: <path>:<line>
- Construction context (one sentence)

## Where the per-voice oscillator block's output feeds
- Next stage class: <name>
- File: <path>:<line>
- Coupling: (e.g., "summed into filter input buffer per sample")

## Open Questions
- <anything unclear about the entry path>
```

- [ ] **Step 3: Commit**

```bash
git add schemas/01-repo-map.schema.md schemas/02-entry-trace.schema.md
git commit -m "schema(obxd-analysis): add repo map and entry trace schemas"
```

---

### Task 10: Schemas 03 (waveform) and 04 (composite)

**Files:**
- Create: `schemas/03-waveform.schema.md`
- Create: `schemas/04-composite.schema.md`

- [ ] **Step 1: Write schema 03 (waveform)**

This is the schema that 3a/3b/3c all reuse. Create `schemas/03-waveform.schema.md`:

~~~markdown
# {WAVEFORM_NAME} Oscillator ({SOURCE_FILE_BASENAME})

## Source
- File: <abs path>
- Lines: <total>
- Class: <name>

## Algorithm
- One-paragraph plain-English summary of the generation method.

## Phase accumulator
- How phase advances (cite line)
- Range and wrap behavior (cite line)
- Initial phase / randomization at construction (cite line, or "none")

## Anti-aliasing
- Method (BLEP / polyBLEP / oversampling / none)
- Table referenced (cite line, or "n/a")
- Correction insertion point (cite line)

## Hard sync support
- Master path: function name, behavior, cite lines
- Slave path: function name, behavior, cite lines
- Sync fraction handling (cite line)

## Pseudocode
```
<language-neutral pseudocode reproducing the algorithm>
```

## Formulas
| Step | Formula | File:line |
|---|---|---|
| <name> | `out = f(in)` | <path>:<n> |

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- What in this implementation is OB-Xd-specific
- What a portable version would need to parameterize
~~~

- [ ] **Step 2: Write schema 04 (composite)**

Create `schemas/04-composite.schema.md`:

~~~markdown
# Per-Voice Oscillator Composite ({SOURCE_FILE_BASENAME})

## Source
- File: <abs path>
- Lines: <total>
- Class: <name>

## Mix structure
- Inputs being summed: <list — osc1, osc2, sub, noise, etc.>
- Per-input gain symbol: <e.g., o1mx, o2mx, nmx>
- Mix line citation: <file:line>
- Output gain stage (e.g., `*3`): <file:line, value>

## Cross-modulation routing
- Source signal: <which oscillator output feeds the modulation>
- Target parameter: <e.g., osc2 pitch>
- Delay-line compensation: <yes/no, length, file:line>
- Modulation depth symbol: <e.g., xmod>

## Hard sync routing
- Master oscillator: <which one>
- Slave oscillator: <which one>
- Hard sync gate signal name: <e.g., hsr>
- Sync fraction signal name: <e.g., hsfrac>
- Delay-line compensation for sync: <yes/no, file:line>

## Per-voice random state
- Random factors initialized in constructor: <list with file:line>
- What they multiply (drift sources): <list>
- Phase initialization (random / fixed): <file:line>

## Dirt / noise injection
- Dirt scalar: <variable, default value, file:line>
- Where injected per sample: <file:line>
- Perpetual noise floor (additive constant): <value, file:line, or "none">

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Pseudocode (one sample step)
```
<language-neutral pseudocode of the per-sample composite update>
```

## Open Questions
- <anything unclear>

## Generalization notes
- Which structural choices are OB-Xd-specific
- What a portable version would need to parameterize
~~~

- [ ] **Step 3: Commit**

```bash
git add schemas/03-waveform.schema.md schemas/04-composite.schema.md
git commit -m "schema(obxd-analysis): add waveform and composite schemas"
```

---

### Task 11: Schemas 05 (voice alloc), 06 (voice routing), 07 (voice wrapper)

**Files:**
- Create: `schemas/05-voice-alloc.schema.md`
- Create: `schemas/06-voice-routing.schema.md`
- Create: `schemas/07-voice-wrapper.schema.md`

- [ ] **Step 1: Write schema 05 (voice alloc)**

Create `schemas/05-voice-alloc.schema.md`:

```markdown
# Voice Allocation ({SOURCE_FILE_BASENAME})

## Source
- File: <abs path>
- Class: <name>
- Helper file: <path or "none">

## Voice pool
- Native voice count constant: <name, value, file:line>
- How voices are stored (array, vector, etc.): <file:line>
- Per-voice state ownership: <one sentence>

## Allocation policy
- Note-on path: <function, file:line, behavior summary>
- Voice selection algorithm: <e.g., round-robin, oldest, lowest priority — file:line>
- Voice stealing trigger: <when, file:line>
- Voice stealing victim selection: <file:line>

## Note-off path
- Function: <name, file:line>
- Release behavior: <one sentence>

## Polyphony characteristics
- Max simultaneous voices: <value>
- Mono mode support: <yes/no, file:line>
- Legato/portamento support: <yes/no, file:line>

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- What scales with voice count, what doesn't
- What a portable version would need to parameterize
```

- [ ] **Step 2: Write schema 06 (voice routing)**

Create `schemas/06-voice-routing.schema.md`:

```markdown
# Voice Routing / Synth Engine ({SOURCE_FILE_BASENAME})

## Source
- File: <abs path>
- Class: <name>

## Per-block render entry
- Function: <signature, file:line>
- Block size handling: <fixed/variable, file:line>

## Per-voice render loop
- Iteration shape: <e.g., for each voice, render N samples — file:line>
- Per-voice output buffer or accumulator: <file:line>
- Voice-to-bus summation: <file:line>

## Shared state inventory
- State that ALL voices read but ONE writes (e.g., LFO, global tuning): <list with file:line>
- State that each voice owns independently: <list>

## Audio output path
- Stereo split: <where, file:line, panning behavior>
- Master gain: <symbol, default, file:line>
- DC blocker / output filter: <yes/no, file:line>

## Sample-rate handling
- Setter: <function, file:line>
- Propagation to voices: <file:line>

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- What in this routing is OB-Xd-specific vs reusable
- What a portable version would need to parameterize
```

- [ ] **Step 3: Write schema 07 (voice wrapper)**

Create `schemas/07-voice-wrapper.schema.md`:

```markdown
# Per-Voice Wrapper ({SOURCE_FILE_BASENAME})

## Source
- File: <abs path>
- Class: <name>

## Per-voice state inventory
For every member variable, classify as:
- **Independent per voice** (different value per voice instance)
- **Shared across voices** (same value, e.g., a sample rate)
- **Random / drift** (per-voice, randomized at construction or note-on)

| Member | Type | Class | File:line |
|---|---|---|---|

## Voice lifecycle
- Note-on entry point: <method, file:line>
- Note-off entry point: <method, file:line>
- Retrigger behavior: <one sentence, file:line>
- Voice-active flag: <member, file:line>

## Oscillator slot ownership
- How many oscillators per voice (native): <count>
- Member name(s) for the oscillator block: <list>
- File:line of the oscillator process call inside the per-voice render: <path>:<n>

## Coupling to filter / envelopes
- Filter member: <name, file:line>
- Envelope members: <list, file:line>
- Modulation routing inside the voice (osc → filter cutoff, env → amp, etc.): <list>

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- What a portable version would need to parameterize
- Which couplings are essential to character vs. swappable
```

- [ ] **Step 4: Commit**

```bash
git add schemas/05-voice-alloc.schema.md schemas/06-voice-routing.schema.md schemas/07-voice-wrapper.schema.md
git commit -m "schema(obxd-analysis): add voice management schemas (alloc, routing, wrapper)"
```

---

### Task 12: Schemas 08 (antialias) and 09 (analog character)

**Files:**
- Create: `schemas/08-antialias.schema.md`
- Create: `schemas/09-analog-character.schema.md`

- [ ] **Step 1: Write schema 08**

Create `schemas/08-antialias.schema.md`:

```markdown
# Anti-Alias / BLEP Strategy

## BLEP table(s)
For each BLEP table found:
- Source file: <path>
- Variable name: <e.g., blep, blepd2>
- Type: <e.g., const float[]>
- Size: <count of entries from declaration line, file:line>
- Oversampling factor (if encoded in name or comment): <value>
- **Body NOT read** — declaration only per design doc Section 5

## Correction insertion points
For each waveform that uses BLEP:
- Waveform: <saw/pulse/triangle>
- Function that adds the correction: <name, file:line>
- Correction stored in: <buffer name>
- Buffer wrap-around mechanism: <one sentence, file:line>

## Oversampling toggle
- Setter on the oscillator block: <function name, file:line>
- Setter on per-waveform classes: <function names, file:lines>
- What changes when toggled (which BLEP table is selected): <file:line>

## Decimator
- Source file: <path>
- Class: <name>
- Where it's invoked in the signal path: <file:line>
- Filter coefficients (count, source line): <file:line>

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- What of this strategy is OB-Xd-specific (table contents)
- What is portable (the BLEP technique itself, the toggle pattern)
```

- [ ] **Step 2: Write schema 09**

Create `schemas/09-analog-character.schema.md`:

```markdown
# Analog Character Inventory

## Techniques observed in the source

For each technique, fill out one block:

### <Technique name>
- What it does (one sentence)
- Mechanism (one paragraph)
- Where: <class, function, file:line>
- Constants involved: <list of (name, value, file:line)>
- Audible effect: <one sentence>

(Repeat for each technique. Examples to look for: phase randomization at construction,
per-voice random detune factors, dirt/jitter on per-sample pitch, perpetual noise floor,
slow LFO drift, temperature drift modeling, component-tolerance simulation, BLEP imperfection,
DC offset injection.)

## Looked for but NOT found

Explicit negative results — techniques that the analyst checked for and confirmed are absent:

- <Technique name> — not observed (confirmed by reading <files>)

## Aggregated constants table

| Constant | Value | File:line | Technique it serves |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- Which constants are essential to character (must be preserved in any reuse)
- Which are conventional (any small value would work)
- How a portable version would parameterize the character "amount"
```

- [ ] **Step 3: Commit**

```bash
git add schemas/08-antialias.schema.md schemas/09-analog-character.schema.md
git commit -m "schema(obxd-analysis): add antialias and analog character schemas"
```

---

### Task 13: Schemas 10 (parameters) and 11 (adjacent systems)

**Files:**
- Create: `schemas/10-parameters.schema.md`
- Create: `schemas/11-adjacent.schema.md`

- [ ] **Step 1: Write schema 10**

Create `schemas/10-parameters.schema.md`:

```markdown
# Parameters & Modulation

## Sources
- Params file: <path>
- ParamsEnum file: <path>
- Tuning file: <path>
- MIDI map file: <path>

## Oscillator-affecting parameters

| Parameter ID | Display name | Range | Curve | Default | Affects | File:line |
|---|---|---|---|---|---|---|

(Range as `[min, max]` or named enum. Curve as `linear / log / exp / step`.
Affects: which oscillator subsystem field this parameter writes to.)

## Modulation routing

| Source | Destination | Depth control | File:line |
|---|---|---|---|

(Source = LFO / envelope / mod wheel / velocity / etc. Destination = oscillator pitch / pulse width / cross-mod / etc.)

## Tuning system
- Scale type: <12-TET / scala / MTS / other>
- Reference frequency / note: <value, file:line>
- Detune handling (cents per parameter unit): <file:line>
- Per-voice detune source: <file:line>

## MIDI map
- Note-on path summary: <one sentence, file:line>
- Velocity sensitivity routing: <file:line>
- Pitch bend range: <file:line>
- CC mappings of interest (mod wheel, sustain): <file:line>

## Constants
| Name | Value | File:line | Purpose |
|---|---|---|---|

## Open Questions
- <anything unclear>

## Generalization notes
- Which parameters are essential to the character
- Which can be replaced by a generic JUCE parameter API
```

- [ ] **Step 2: Write schema 11**

Create `schemas/11-adjacent.schema.md`:

```markdown
# Critical Adjacent Systems Survey

For each adjacent system that materially shapes the final sound, fill out one block.
This stage is **identification only** — no deep algorithm documentation.

### <System name> (e.g., Filter)
- Source file: <path>
- Class: <name>
- Role in the sound (1–3 sentences)
- Coupling to oscillator stage:
  - Receives: <what the oscillator stage gives it>
  - Gives back: <what it sends downstream, or back to the oscillator stage>
  - Modulates oscillator parameters? <yes/no, where>
- **Verdict:**
  - `MUST-REPLICATE-FOR-CHARACTER` — a generic JUCE-module equivalent will not preserve the sound; the specific algorithm must be ported
  - `JUCE-MODULE-EQUIVALENT-OK` — a standard JUCE/open-source equivalent will give a comparable result
- Reasoning for the verdict (one sentence)

(Repeat for: Filter, Amplitude envelope, Filter envelope, LFO, Saturation/waveshaper,
Voice-level gain and pan, Per-voice mixing, Output / master stage, Feedback paths,
Cross-modulation routing — only those that are present in the target.)

## Open Questions
- <anything unclear about coupling or boundaries>
```

- [ ] **Step 3: Commit**

```bash
git add schemas/10-parameters.schema.md schemas/11-adjacent.schema.md
git commit -m "schema(obxd-analysis): add parameters and adjacent systems schemas"
```

---

### Task 14: Schemas 12 (review) and 13 (final synthesis)

**Files:**
- Create: `schemas/12-review.schema.md`
- Create: `schemas/13-final-spec.schema.md`

- [ ] **Step 1: Write schema 12 (reviewer output)**

Create `schemas/12-review.schema.md`:

```markdown
# Review of <ARTIFACT-FILENAME>

## Verdict
- One of: `PASS` / `PASS-WITH-NOTES` / `FAIL`

## Check results

### 1a. Citation validity
- Total claims with citations: <n>
- Claims with valid citations: <n>
- Claims with citations that did not resolve or did not support the claim: <n>
- Failed citations: <list of file:line, with one-line note per>

### 1b. No forbidden references
- Citations to READ-DECLARATION-ONLY files: <n>
- If > 0: <list>

### 2. Schema completeness
- All required schema sections present: <yes/no>
- Missing sections: <list, or "none">

### 3. Formula correctness
- Formulas checked: <n>
- Formulas matching cited code: <n>
- Mismatches: <list with explanation>

### 4. Pseudocode fidelity
- Reproduces observable behavior: <yes/no>
- Issues: <list, or "none">

### 5. Generalization notes
- Section is non-empty and specific: <yes/no>
- Notes: <list, or "none">

### 6. Open Questions triage
- RESOLVED: <list>
- DEFER-TO-SYNTHESIS: <list>
- BLOCKER: <list>

## Issues found

| Severity | Description | Suggested fix |
|---|---|---|
| <critical/major/minor> | <one line> | <one line — do NOT rewrite the extraction> |

## Notes for synthesis
- <Anything Stage 10 should know that isn't an outright FAIL>
```

- [ ] **Step 2: Write schema 13 (final spec — the deliverable)**

This is the entire final-spec schema from design doc Section 3.3, plus the BLOCKERS.md schema from 3.4. Create `schemas/13-final-spec.schema.md`:

```markdown
# Final Spec Schema (10-final-spec.md and BLOCKERS.md)

This schema is the contract for the Stage 10 synthesis subagent's output.
The synthesis subagent fills in this template; the orchestrator verifies
all sections are present and dual citations exist where required.

## 10-final-spec.md template

# {TARGET_SYNTH_NAME} Oscillator Subsystem — Portable Specification

## 1. Overview
- One-paragraph sonic-character description
- One-paragraph design-goals statement
- Source attribution: {TARGET_SYNTH_NAME} {TARGET_VERSION}, {TARGET_LICENSE}

## 2. Architectural Diagram
- ASCII block diagram of the per-voice oscillator signal path
- Labeled interfaces to downstream systems
- Parameterization markers: {VOICE_COUNT}, {OSCS_PER_VOICE}, {WAVEFORM_SET}, {UNISON_DEPTH}

## 3. Waveform Generators
For each waveform: algorithm, pseudocode, formulas, constants table, anti-alias method,
hard-sync behavior. Citation: artifact path + source file:line (dual).

## 4. Per-Voice Oscillator Composite
Mix structure, cross-mod routing with delay compensation, hard sync routing, output gain.
Citation: artifact path + source file:line.

## 5. Polyphony & Voice Architecture
- Native voice count (config = expected, 04a canonical for observed; 04b/04c cross-checks; mismatch → BLOCKERS)
- Native oscs-per-voice (04c canonical)
- Voice allocation policy
- Per-voice vs shared state inventory
- Detune-across-voices model
### Parameterization (cite-only at native size)
### TODO — human step: scaling derivation
- Goal, inputs the human needs

## 6. Analog Character Inventory
- Each technique with mechanism and constants
- Looked-for-and-not-found list
- Citation: artifact path + source file:line
- If 06 DEFERRED: one-line statement, no fill-in

## 7. Parameters & Modulation
- Every osc-affecting parameter with range, curve, default, source artifact
- Modulation routing matrix
- Tuning system summary

## 8. Critical Adjacent Systems
For each: name, role (1–3 sentences), coupling, MUST-REPLICATE / JUCE-EQUIVALENT verdict,
citation: artifact path + source file:line.

## 9. Portability Guide
- OB-Xd-specific vs reusable inventory
- Required JUCE-host interfaces
### Replacement categories (cite-only)
- Categories only — no specific JUCE module names
- Required parametric inputs ({ALL_CAPS} markers from section 2)
### TODO — human step: module selection
- Goal, inputs the human needs

## 10. Scaling Guidance
- Cross-reference to section 5's TODO
- Cite-only material from section 11

## 11. Constants & Tuning Values
- Single table; every row has dual citations (artifact path + source file:line)
- Columns: name, value, artifact path, source file:line, role in the sound

## 12. Open Questions & Unknowns
- DEFER-TO-SYNTHESIS items synthesis could resolve
- Genuine unknowns (mutually exclusive with BLOCKERS.md per design rule 9)

## 13. Prototype Plan
### Cite-only test surface
### TODO — human step: audible criteria

## Appendix A. Artifact Provenance
- Section → artifact mapping
- Stage status table (PASS / PASS-WITH-NOTES / FAIL / DEFERRED)


## BLOCKERS.md template

# Synthesis Blockers

## Summary
- N disagreements
- M unresolved DEFER-TO-SYNTHESIS items
- K spec sections marked INCOMPLETE
- L spec sections marked DEFERRED

## Disagreements
For each:
- Topic
- Artifact A says (quote + path)
- Artifact B says (quote + path)
- Why I cannot adjudicate
- Recommended human action

## Unresolved Open Questions

## Incomplete Spec Sections

## Deferred Spec Sections


If empty: file still starts with "# Synthesis Blockers" and contains "## No blockers"
as its sole sub-header.
```

- [ ] **Step 3: Commit**

```bash
git add schemas/12-review.schema.md schemas/13-final-spec.schema.md
git commit -m "schema(obxd-analysis): add review and final synthesis schemas"
```

---

## Phase 6 — Subagent Prompt Templates

### Task 15: Common contract template

**Files:**
- Create: `prompts/_common.md`

- [ ] **Step 1: Write the shared contract block**

Create `prompts/_common.md`:

```markdown
# Common Contract Block — Extraction Subagents

This block is included verbatim by every extraction subagent prompt template
in this directory. Stage-specific prompts add ROLE, INPUTS, ARTIFACT SCHEMA,
and TOKEN BUDGET on top.

## FORBIDDEN READS
- Any file marked READ-DECLARATION-ONLY in 01-repo-map.md
  (BlepData.h is the canonical example — read its declaration line only,
  never the lookup-table body)
- Any file not on your INPUTS list above

Note: the FORBIDDEN list is a deterrent. The real protection is that your
INPUTS list is bounded — if a file isn't there, you have no reason to open it.

## OUTPUT
Write exactly one file: the artifact path declared in INPUTS.
Use the schema referenced in ARTIFACT SCHEMA below. Do not write anywhere else.
Do not create extra files.

If you cannot complete the artifact, write a top-level
`## FAILED — <one-line reason>` section into the same artifact path.
This is your only failure channel. Do not create a separate error file.

## CITATION RULE
Every claim about behavior must cite file:line. No paraphrase without citation.
If you cannot cite it, write `UNCITED — needs reviewer attention` and continue.

## UNKNOWN RULE
If something is unclear, write it under `## Open Questions` at the bottom.
Do not guess. Do not invent. Reviewer will resolve.

## STOP CONDITIONS
Stop when the schema is filled. Do not explore beyond INPUTS.
If you discover you need a file outside INPUTS, do not open it — instead,
write a `## FAILED — missing input <path>` section and stop.

## TOKEN BUDGET
See your stage's TOKEN BUDGET section. There is no hard meter; the real bound
is your INPUTS list. Stay focused on filling the schema, not exploring.

## FORBIDDEN ACTIONS (universal)
- Do not invoke other subagents (only the orchestrator does that)
- Do not run code, build anything, or modify source files
- Do not write outside your declared artifact path
- Do not reformat or "improve" the artifact schema — fill it as given
```

- [ ] **Step 2: Commit**

```bash
git add prompts/_common.md
git commit -m "prompt(obxd-analysis): add common contract template"
```

---

### Task 16: Stage 1 and Stage 2 prompts

**Files:**
- Create: `prompts/stage-01-repo-map.md`
- Create: `prompts/stage-02-entry-trace.md`

- [ ] **Step 1: Write Stage 1 prompt**

Create `prompts/stage-01-repo-map.md`:

```markdown
# Stage 1 — Repo Map

## ROLE
You are a code-base mapper. You produce one Markdown artifact that catalogs
every source file in {TARGET_REPO_PATH}/Source/Engine/, with line counts and
inferred role labels. You do NOT read file bodies in depth — only enough to
infer roles. Files over 1500 lines must be flagged as candidates for
READ-DECLARATION-ONLY.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Directory to scan: {TARGET_REPO_PATH}/Source/Engine/
- (No prior artifacts.)

## ARTIFACT
Write exactly one file: {WORKING_DIR}/01-repo-map.md
Use schema: schemas/01-repo-map.schema.md

## METHOD
1. List the directory contents (file names only).
2. For each file, get its line count.
3. Open each file just long enough to read the first ~30 lines (header,
   includes, class declaration). Infer a one-line role label from the class
   name and includes — do NOT read further.
4. Fill the schema's "Files" table with (path, lines, role).
5. Any file > 1500 lines: add to "Files flagged READ-DECLARATION-ONLY".
6. Fill "Inferred role labels" by mapping the roles to the canonical list
   in the schema. Use "not present" if you cannot find one.
7. Anything ambiguous → Open Questions.

## TOKEN BUDGET
Soft guidance: 5k input, 3k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 2: Write Stage 2 prompt**

Create `prompts/stage-02-entry-trace.md`:

```markdown
# Stage 2 — Entry Trace

## ROLE
You are a control-flow tracer. You identify the per-voice oscillator entry
point class, where it lives, where it is instantiated, and where its output
feeds. You do NOT document algorithms — only the entry path.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source files (read in full):
  - {TARGET_REPO_PATH}/Source/Engine/ObxdOscillatorB.h
  - {TARGET_REPO_PATH}/Source/Engine/ObxdVoice.h
- Prior artifact (read for context):
  - {WORKING_DIR}/01-repo-map.md

## ARTIFACT
Write exactly one file: {WORKING_DIR}/02-entry-trace.md
Use schema: schemas/02-entry-trace.schema.md

## METHOD
1. In ObxdOscillatorB.h, find the public class and the per-sample method
   (the method called once per output sample). Note its signature.
2. In ObxdVoice.h, find which member variable owns the oscillator block,
   and the per-voice render method that calls it.
3. Trace where the per-voice render method is called from (look for
   includes / forward declarations that point upward — but do NOT open
   files outside INPUTS; if you can't determine this from the two files,
   note it as Open Question).
4. Fill the schema. Cite file:line for everything.

## TOKEN BUDGET
Soft guidance: 15k input, 4k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 3: Commit**

```bash
git add prompts/stage-01-repo-map.md prompts/stage-02-entry-trace.md
git commit -m "prompt(obxd-analysis): add Stage 1 and Stage 2 prompts"
```

---

### Task 17: Stage 3 prompts (waveform template + composite)

**Files:**
- Create: `prompts/stage-03-waveform.md`
- Create: `prompts/stage-03d-composite.md`

- [ ] **Step 1: Write Stage 3 waveform template**

Create `prompts/stage-03-waveform.md` (parameterized for 3a/3b/3c):

```markdown
# Stage {3a|3b|3c} — {Saw|Pulse|Triangle} Extraction

## ROLE
You are a DSP code reader extracting the {WAVEFORM_NAME} oscillator algorithm
from {TARGET_SYNTH_NAME}. You produce one Markdown artifact filled per the
waveform schema. Every claim must be cited.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source file (read in full): {TARGET_REPO_PATH}/Source/Engine/{SOURCE_FILE}
  - 3a uses SawOsc.h
  - 3b uses PulseOsc.h
  - 3c uses TriangleOsc.h

## ARTIFACT
Write exactly one file:
- 3a → {WORKING_DIR}/03a-saw.md
- 3b → {WORKING_DIR}/03b-pulse.md
- 3c → {WORKING_DIR}/03c-triangle.md
Use schema: schemas/03-waveform.schema.md

## METHOD
1. Read the entire source file once.
2. Identify the public class.
3. For each schema section (Algorithm, Phase accumulator, Anti-aliasing,
   Hard sync, Pseudocode, Formulas, Constants, Generalization notes),
   find the relevant code and fill the section with file:line citations.
4. The Pseudocode section should reproduce the per-sample update in
   language-neutral form (Python-like is fine). Do not paraphrase comments;
   reproduce the algorithm.
5. Anything unclear → Open Questions.

## TOKEN BUDGET
Soft guidance: 8–10k input, 5k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 2: Write Stage 3d composite prompt**

Create `prompts/stage-03d-composite.md`:

```markdown
# Stage 3d — Per-Voice Oscillator Composite

## ROLE
You are a DSP code reader extracting the per-voice oscillator composite from
{TARGET_SYNTH_NAME}. This is the class that owns osc1+osc2 (or however many
slots), the cross-modulation path, hard-sync routing, dirt injection, and
the final mix and output gain.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source file (read in full): {TARGET_REPO_PATH}/Source/Engine/ObxdOscillatorB.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/03d-osc-composite.md
Use schema: schemas/04-composite.schema.md

## METHOD
1. Read the entire source file once.
2. Identify the per-sample method (likely `ProcessSample` or similar).
3. For each schema section, locate the corresponding code and cite file:line.
   Pay special attention to:
   - The mix line that sums osc1+osc2+noise (often near the end of the per-sample method)
   - Output gain stage (any `* <constant>` on the return value)
   - Per-voice random factors set in the constructor (drift sources)
   - Dirt scalar and where it's added per sample
   - Any constant added that is NOT zero (perpetual noise floor)
   - Cross-mod path: which oscillator output feeds the modulation, and the delay-line compensation
   - Hard sync gate signal name and the sync fraction signal
4. Pseudocode reproduces the per-sample composite update.
5. Anything unclear → Open Questions.

## TOKEN BUDGET
Soft guidance: 15k input, 8k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 3: Commit**

```bash
git add prompts/stage-03-waveform.md prompts/stage-03d-composite.md
git commit -m "prompt(obxd-analysis): add Stage 3 waveform and composite prompts"
```

---

### Task 18: Stage 4 prompts (voice mgmt trio)

**Files:**
- Create: `prompts/stage-04a-voice-alloc.md`
- Create: `prompts/stage-04b-voice-routing.md`
- Create: `prompts/stage-04c-voice-wrapper.md`

- [ ] **Step 1: Write Stage 4a prompt**

Create `prompts/stage-04a-voice-alloc.md`:

```markdown
# Stage 4a — Voice Allocation (Motherboard)

## ROLE
You are a polyphony specialist extracting voice allocation from
{TARGET_SYNTH_NAME}. Focus: how voices are stored, how note-on selects a
voice, when stealing happens, and what the polyphony characteristics are.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source files (read in full):
  - {TARGET_REPO_PATH}/Source/Engine/Motherboard.h
  - {TARGET_REPO_PATH}/Source/Engine/VoiceQueue.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/04a-voice-alloc.md
Use schema: schemas/05-voice-alloc.schema.md

## METHOD
1. Read both files in full.
2. Find the voice pool storage (array, vector, or other container).
3. Find the note-on entry point and trace voice selection.
4. Find the voice-stealing trigger and victim selection.
5. Identify the native voice count (look for a hardcoded constant or
   template parameter).
6. Mono / legato support: search for relevant flags or modes.
7. Fill the schema with citations.

## TOKEN BUDGET
Soft guidance: 20k input, 6k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 2: Write Stage 4b prompt**

Create `prompts/stage-04b-voice-routing.md`:

```markdown
# Stage 4b — Voice Routing (SynthEngine)

## ROLE
You extract how the synth engine drives per-voice rendering, what state is
shared across voices vs. per-voice, and how voice outputs are summed to the
master bus.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source file (read in full): {TARGET_REPO_PATH}/Source/Engine/SynthEngine.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/04b-voice-routing.md
Use schema: schemas/06-voice-routing.schema.md

## METHOD
1. Read the file in full (~600 lines).
2. Find the per-block render entry point. Note block size handling.
3. Trace how it iterates over voices and calls each voice's render.
4. Identify shared state (LFOs, global tuning, master gain, output filters)
   vs per-voice state.
5. Find the audio output path: stereo split, master gain, DC blocker if any.
6. Find the sample-rate setter and how it propagates to voices.
7. Fill the schema with citations.

## TOKEN BUDGET
Soft guidance: 30k input, 8k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 3: Write Stage 4c prompt**

Create `prompts/stage-04c-voice-wrapper.md`:

```markdown
# Stage 4c — Per-Voice Wrapper

## ROLE
You catalog the per-voice wrapper class: every member variable classified
as independent / shared / random, the voice lifecycle, and how the
oscillator block couples to filters and envelopes inside the voice.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source file (read in full): {TARGET_REPO_PATH}/Source/Engine/ObxdVoice.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/04c-voice-wrapper.md
Use schema: schemas/07-voice-wrapper.schema.md

## METHOD
1. Read the file in full.
2. List EVERY member variable. Classify each as Independent / Shared / Random.
3. Find the note-on, note-off, retrigger entry points. Cite each.
4. Find how many oscillator slots the voice owns. Find the per-voice render
   loop and cite where the oscillator block is called.
5. Identify filter and envelope members and how they couple to the oscillator
   (modulation routing inside the voice).
6. Fill the schema with citations.

## TOKEN BUDGET
Soft guidance: 18k input, 6k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 4: Commit**

```bash
git add prompts/stage-04a-voice-alloc.md prompts/stage-04b-voice-routing.md prompts/stage-04c-voice-wrapper.md
git commit -m "prompt(obxd-analysis): add Stage 4 voice management prompts"
```

---

### Task 19: Stages 5 (antialias) and 6 (analog character) prompts

**Files:**
- Create: `prompts/stage-05-antialias.md`
- Create: `prompts/stage-06-analog-character.md`

- [ ] **Step 1: Write Stage 5 prompt**

Create `prompts/stage-05-antialias.md`:

```markdown
# Stage 5 — Anti-Alias / BLEP Strategy

## ROLE
You document the anti-aliasing strategy used by {TARGET_SYNTH_NAME}: BLEP
table sizes (from declaration only, NEVER body), correction insertion
points, the oversampling toggle, and the decimator. The BLEP lookup table
data is forbidden — declaration line only.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source files (read in full):
  - {TARGET_REPO_PATH}/Source/Engine/SawOsc.h
  - {TARGET_REPO_PATH}/Source/Engine/PulseOsc.h
  - {TARGET_REPO_PATH}/Source/Engine/TriangleOsc.h
  - {TARGET_REPO_PATH}/Source/Engine/Decimator.h
- Source file (DECLARATION ONLY — NEVER read body):
  - {TARGET_REPO_PATH}/Source/Engine/BlepData.h
  Read at most the first 30 lines to capture the table declaration
  (`const float blep[]`, `const float blepd2[]`, etc.) and infer size from
  any surrounding constants. Do NOT read the lookup-table values.

## ARTIFACT
Write exactly one file: {WORKING_DIR}/05-antialias.md
Use schema: schemas/08-antialias.schema.md

## METHOD
1. Open BlepData.h, read at most the first 30 lines, note the table
   declarations and any size constants. STOP reading.
2. In SawOsc.h, PulseOsc.h, TriangleOsc.h: find where the BLEP correction
   is inserted. Find any setDecimation/removeDecimation methods that swap
   the active table.
3. In Decimator.h: identify the class and where it's invoked.
4. Fill the schema with citations. Mark BlepData.h citations as
   `(declaration only)`.

## TOKEN BUDGET
Soft guidance: 25k input, 6k output.

## CRITICAL FORBIDDEN READ
BlepData.h body is the canonical READ-DECLARATION-ONLY file. Reading past
line ~30 of that file will cause this stage to FAIL. If you find yourself
needing the table values, write `## FAILED — attempted forbidden read of
BlepData.h body` and stop.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 2: Write Stage 6 prompt**

Create `prompts/stage-06-analog-character.md`:

```markdown
# Stage 6 — Analog Character Inventory

## ROLE
You synthesize an inventory of analog-character techniques from prior
extraction artifacts. You read NO source files — only artifacts. Your job
is to catalog everything that moves the sound away from sterile digital,
and to explicitly list techniques you looked for and DID NOT find.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Prior artifacts (read in full):
  - {WORKING_DIR}/03d-osc-composite.md
  - {WORKING_DIR}/04a-voice-alloc.md
  - {WORKING_DIR}/04b-voice-routing.md
  - {WORKING_DIR}/04c-voice-wrapper.md

## ARTIFACT
Write exactly one file: {WORKING_DIR}/06-analog-character.md
Use schema: schemas/09-analog-character.schema.md

## METHOD
1. Read all four input artifacts.
2. For each technique observed in the source (per the extraction artifacts),
   fill out one block in "Techniques observed". Cite the artifact and the
   underlying source file:line that the artifact cited.
3. Look for and explicitly note absence of: phase randomization, per-voice
   random detune, dirt/jitter on pitch, perpetual noise floor, slow LFO
   drift, temperature drift, component-tolerance modeling, BLEP imperfection,
   DC offset injection. For each one NOT found, add it to "Looked for but
   NOT found" with a citation explaining how you confirmed absence.
4. Aggregate every constant into the constants table.
5. Generalization notes: which constants are essential vs conventional.

## TOKEN BUDGET
Soft guidance: 15k input, 6k output.

## NO SOURCE FILES
Your INPUTS contain ONLY artifacts. You do not open source files.
Every claim cites an artifact (and dual-cites the source line the artifact cited).

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 3: Commit**

```bash
git add prompts/stage-05-antialias.md prompts/stage-06-analog-character.md
git commit -m "prompt(obxd-analysis): add Stage 5 antialias and Stage 6 analog character prompts"
```

---

### Task 20: Stages 7 (parameters) and 8 (adjacent) prompts

**Files:**
- Create: `prompts/stage-07-parameters.md`
- Create: `prompts/stage-08-adjacent.md`

- [ ] **Step 1: Write Stage 7 prompt**

Create `prompts/stage-07-parameters.md`:

```markdown
# Stage 7 — Parameters & Tuning

## ROLE
You catalog every oscillator-affecting parameter, every modulation route,
the tuning system, and the MIDI map.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source files (read in full):
  - {TARGET_REPO_PATH}/Source/Engine/Params.h
  - {TARGET_REPO_PATH}/Source/Engine/ParamsEnum.h
  - {TARGET_REPO_PATH}/Source/Engine/Tuning.h
  - {TARGET_REPO_PATH}/Source/Engine/midiMap.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/07-parameters.md
Use schema: schemas/10-parameters.schema.md

## METHOD
1. Read all four files.
2. For each parameter in Params.h / ParamsEnum.h, decide whether it affects
   the oscillator subsystem (pitch, detune, waveform, pulse width, sub level,
   xmod, sync, dirt, etc.). Skip non-osc parameters.
3. For each affecting parameter, fill the table: ID, display name, range,
   curve, default, what it affects (which oscillator field), file:line.
4. Find modulation routing in any of the four files (LFO depth, env depth,
   mod wheel routing). Fill the modulation routing table.
5. Tuning.h: extract scale type, reference, detune handling.
6. midiMap.h: extract note-on path, velocity, pitch bend, CC mappings.

## TOKEN BUDGET
Soft guidance: 30k input, 8k output.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 2: Write Stage 8 prompt**

Create `prompts/stage-08-adjacent.md`:

```markdown
# Stage 8 — Adjacent Systems Survey

## ROLE
You IDENTIFY the critical adjacent systems that shape the final sound. This
is identification only — name the system, summarize its role in 1–3
sentences, describe its coupling to the oscillator stage, and decide
MUST-REPLICATE-FOR-CHARACTER vs JUCE-MODULE-EQUIVALENT-OK. Do NOT document
internal algorithms in depth.

## INPUTS
- config.yaml: {ABS_PATH_TO_CONFIG}
- Source files (read in full, but only enough to identify roles and
  coupling — do not transcribe algorithms):
  - {TARGET_REPO_PATH}/Source/Engine/Filter.h
  - {TARGET_REPO_PATH}/Source/Engine/AdsrEnvelope.h
  - {TARGET_REPO_PATH}/Source/Engine/Lfo.h
  - {TARGET_REPO_PATH}/Source/Engine/AudioUtils.h

## ARTIFACT
Write exactly one file: {WORKING_DIR}/08-adjacent.md
Use schema: schemas/11-adjacent.schema.md

## METHOD
1. Read each file enough to understand its role.
2. For each system, fill one block with name, role, coupling, verdict,
   and reasoning.
3. Verdict criteria:
   - MUST-REPLICATE-FOR-CHARACTER: the system has unusual / signature
     properties that a generic JUCE-module equivalent would not reproduce
     (e.g., a non-standard filter topology, a particular envelope curve,
     a deliberately imperfect LFO).
   - JUCE-MODULE-EQUIVALENT-OK: the system is conventional and a standard
     JUCE/open-source equivalent will give a comparable result.
4. Reasoning is a single sentence per verdict.
5. AudioUtils.h: catalog any helper functions used elsewhere (saturation,
   gain shaping, etc.).

## TOKEN BUDGET
Soft guidance: 25k input, 6k output.

## STAY OUT OF DEPTH
Do NOT transcribe filter coefficients, envelope curves, or LFO shapes.
That is out of scope. If you find yourself writing more than 3 paragraphs
about one system, you have over-extracted.

(Then include the contents of prompts/_common.md verbatim.)
```

- [ ] **Step 3: Commit**

```bash
git add prompts/stage-07-parameters.md prompts/stage-08-adjacent.md
git commit -m "prompt(obxd-analysis): add Stage 7 parameters and Stage 8 adjacent prompts"
```

---

### Task 21: Stage 9 reviewer prompt

**Files:**
- Create: `prompts/stage-09-reviewer.md`

- [ ] **Step 1: Write the reviewer prompt**

Create `prompts/stage-09-reviewer.md`:

```markdown
# Stage 9 — Per-Artifact Reviewer

## ROLE
You are a verification reviewer. You read ONE extraction artifact and the
source files the orchestrator pre-approved for you. You produce ONE review
file. You do NOT re-extract, do NOT propose new content, do NOT rewrite
the extraction.

## INPUTS (read-only, allowlisted)
- Extraction artifact: {ARTIFACT_PATH}
- Pre-approved cited files: {ALLOWLISTED_PATHS}
  (The orchestrator computed this list from the artifact's citations,
  filtered against the FORBIDDEN list. You cannot dynamically open files.)

## FORBIDDEN READS
- Any file not on the allowlist above
- Any file marked READ-DECLARATION-ONLY (the orchestrator already filtered
  these out, but the rule still applies if one slipped through)

## ARTIFACT
Write exactly one file: {WORKING_DIR}/09-review/{ARTIFACT_BASENAME}
Use schema: schemas/12-review.schema.md

## CHECKS (in order)
1a. Citation validity:    every file:line resolves and supports the claim
1b. No forbidden refs:    no citation points to a READ-DECLARATION-ONLY file
2.  Schema completeness:  every required schema section is present
3.  Formula correctness:  each formula matches the cited code
4.  Pseudocode fidelity:  pseudocode reproduces observable behavior
5.  Generalization notes: section is non-empty and specific
6.  Open Questions:       triaged into RESOLVED / DEFER-TO-SYNTHESIS / BLOCKER

## VERDICT
- PASS: all checks pass
- PASS-WITH-NOTES: minor issues, synthesis can handle them
- FAIL: critical issues that synthesis must not paper over

## STOP CONDITIONS
Stop when the review schema is filled. Do not re-extract. Do not propose
new content.

## TOKEN BUDGET
Soft guidance: 20k input, 4k output.

## FORBIDDEN ACTIONS
- Do not invoke other subagents (only the orchestrator does that)
- Do not run code, build anything, or modify source files
- Do not write outside your declared review file path
- Do not rewrite the extraction artifact — your job is verification only
```

- [ ] **Step 2: Commit**

```bash
git add prompts/stage-09-reviewer.md
git commit -m "prompt(obxd-analysis): add Stage 9 reviewer prompt"
```

---

### Task 22: Stage 10 synthesis prompt

**Files:**
- Create: `prompts/stage-10-synthesis.md`

- [ ] **Step 1: Write the synthesis prompt**

Create `prompts/stage-10-synthesis.md`:

```markdown
# Stage 10 — Final Synthesis

## ROLE
You are the final synthesis subagent. You read every extraction artifact and
every review artifact and produce ONE deliverable: the portable oscillator
specification. You also produce BLOCKERS.md if you find unresolvable
conflicts. You do NOT open source files — synthesize from artifacts only.

## NOTE ON SPLIT EXECUTION
When the orchestrator splits Stage 10 per design doc Section 3.6, you receive
this contract with INPUTS, OUTPUT, and STOP CONDITIONS adjusted for either
10a (sections 1–8 + 11) or 10b (sections 9, 10, 12, 13 + Appendix A). The
conflict-resolution rules, citation policy, and forbidden actions stay
identical across the split.

## INPUTS (read-only, all explicit absolute paths)
The orchestrator computes this list at dispatch time, including any
FAILED-<stage>.md markers it finds in the working dir, and passes the
resolved list explicitly. You receive concrete paths, not globs. Typical:
- config.yaml
- 01-repo-map.md, 02-entry-trace.md
- 03a-saw.md, 03b-pulse.md, 03c-triangle.md, 03d-osc-composite.md
- 04a-voice-alloc.md, 04b-voice-routing.md, 04c-voice-wrapper.md
- 05-antialias.md, 06-analog-character.md (may be DEFERRED), 07-parameters.md, 08-adjacent.md
- 09-review/<all 11 review files>
- Any FAILED-<stage>.md files

## FORBIDDEN READS
- All original source files. You synthesize from artifacts only.
- READ-DECLARATION-ONLY rules still apply.

## ARTIFACTS
Write two files:
1. {WORKING_DIR}/10-final-spec.md — the deliverable
   Use schema: schemas/13-final-spec.schema.md (the "10-final-spec.md template" section)
2. {WORKING_DIR}/BLOCKERS.md — conflicts you could not resolve
   Use schema: schemas/13-final-spec.schema.md (the "BLOCKERS.md template" section)
   Always starts with "# Synthesis Blockers". Empty case has "## No blockers"
   as its sole sub-header.

## CITATION POLICY (dual citations mandatory)
Every claim cites both:
(a) the artifact path that synthesized it, and
(b) the original source file:line that the artifact itself cited.

This preserves chain-of-custody (a) while letting any reader verify against
the actual code (b). Section 11 (Constants) MUST have dual citations on
every row — it is the verification surface.

## NO-FABRICATION RULE (sections 5, 9, 13)
Sections 5 (scaling), 9 (portability), and 13 (prototype plan) contain parts
that are creative engineering — not extractable from artifacts. For those
parts, write only what you can cite. Anything that would require invention
goes under a "## TODO — human step" sub-section with a bullet list of inputs
the human will need.

Specifically:
- Section 5 cite-only: native voice count, structural state inventory
- Section 5 TODO: scaling derivation, 4v/2osc worked example
- Section 9 cite-only: replacement categories (filter, env, LFO, ...)
- Section 9 TODO: specific JUCE module names (you may NOT name them)
- Section 13 cite-only: test cases derivable from artifacts
- Section 13 TODO: audible criteria for analog detune authenticity

## CONFLICT RESOLUTION RULES (apply in order; first to produce a verdict wins)
1. Reviewer FAIL on a claim → drop the claim
2. PASS-WITH-NOTES → keep claim, attach [reviewer-note] tag
3. Two extractions disagree, prefer more deeply-cited; ties → topically-closer stage
4. Disagreement, depth and proximity equal → both into BLOCKERS, drop from spec
5. FAILED stage's content needed → mark spec section "## INCOMPLETE — depends on FAILED <stage>"
6. Stage 6 DEFERRED → spec section 6 is a one-line reference; do NOT partial-fill
7. DEFER-TO-SYNTHESIS Open Question → resolve only if unambiguous in another artifact, else BLOCKERS
8. Generalization conflict → prefer OB-Xd-specific only when parameterization changes observable behavior; prefer reusable when disagreement is about whether the value is conventional
9. Mutual exclusivity: each item appears in exactly one place — BLOCKERS or spec section 12, never both

## STOP CONDITIONS
Stop when 10-final-spec.md is filled per the schema and BLOCKERS.md exists.
Do not re-extract. Do not open source files.

## TOKEN BUDGET
150k input. If even that is tight, the orchestrator will split you into
10a and 10b per Section 3.6 of the design doc.

## FORBIDDEN ACTIONS
- Open any source file (your INPUTS does not include them)
- Invent claims not in any artifact
- Resolve a disagreement by averaging or compromising — pick a rule from above or BLOCKERS
- Fill the Analog Character section if Stage 6 is DEFERRED
- Name specific JUCE modules in section 9 (categories only)
- Fabricate audible criteria in section 13 (cite-only test surface; rest is TODO)
- Fabricate scaling rules in section 5 (cite-only at native size; rest is TODO)
- Skip writing BLOCKERS.md (must always exist, even if empty)
- Reformat or "improve" the final spec schema — fill it as given
- Duplicate items between BLOCKERS.md and spec section 12
- Override a reviewer FAIL (rule 1 is authoritative)
```

- [ ] **Step 2: Commit**

```bash
git add prompts/stage-10-synthesis.md
git commit -m "prompt(obxd-analysis): add Stage 10 synthesis prompt"
```

---

## Phase 7 — Setup, Commit Helper, Runbook, README

### Task 23: Working dir setup script

**Files:**
- Create: `orchestrator/setup_workdir.sh`

- [ ] **Step 1: Write the setup script**

Create `orchestrator/setup_workdir.sh`:

```bash
#!/usr/bin/env bash
# Create the analysis working directory and initialize it as its own git repo.
# Idempotent: safe to re-run; will not clobber existing artifacts.
#
# Usage: ./orchestrator/setup_workdir.sh path/to/config.yaml

set -euo pipefail

CONFIG_PATH="${1:-config/config.yaml}"
if [[ ! -f "$CONFIG_PATH" ]]; then
    echo "error: config not found at $CONFIG_PATH" >&2
    exit 1
fi

# Extract working_dir from the YAML using a simple grep + sed
# (avoids requiring yq; PyYAML is the only Python dependency we declare).
WORK_DIR=$(python -c "import yaml,sys; print(yaml.safe_load(open('$CONFIG_PATH'))['working_dir'])")
WORK_DIR="${WORK_DIR/#\~/$HOME}"

if [[ -d "$WORK_DIR/.git" ]]; then
    echo "working dir already initialized at $WORK_DIR"
    exit 0
fi

mkdir -p "$WORK_DIR"
mkdir -p "$WORK_DIR/09-review"

cd "$WORK_DIR"
git init -q
git commit --allow-empty -q -m "init: empty working dir for OB-Xd oscillator analysis"

echo "working dir initialized at $WORK_DIR"
```

- [ ] **Step 2: Make executable and smoke-test**

```bash
chmod +x orchestrator/setup_workdir.sh
./orchestrator/setup_workdir.sh config/config.yaml
```

Expected: prints `working dir initialized at /home/myuser/synth-research/OB-Xd-oscillator-analysis` (or similar). Re-run; should print `working dir already initialized at ...`.

- [ ] **Step 3: Verify the working dir is a git repo**

Run: `cd ~/synth-research/OB-Xd-oscillator-analysis && git log --oneline`
Expected: shows the empty init commit.

- [ ] **Step 4: Commit**

```bash
cd ~/agents/juce-agent
git add methodologies/obxd-oscillator-analysis/orchestrator/setup_workdir.sh
git commit -m "feat(obxd-analysis): add working dir setup script"
```

---

### Task 24: Stage commit helper script

**Files:**
- Create: `orchestrator/stage_commit.sh`

- [ ] **Step 1: Write the helper**

Create `orchestrator/stage_commit.sh`:

```bash
#!/usr/bin/env bash
# Commit the working dir after a stage completes.
# Used by the orchestrator after each subagent's successful return,
# OR after a FAILED-marker artifact is written.
#
# Usage: ./orchestrator/stage_commit.sh <stage-tag> <message>
# Examples:
#   ./orchestrator/stage_commit.sh "stage 3a"     "saw extraction"
#   ./orchestrator/stage_commit.sh "stage 4b"     "voice routing — FAILED"
#   ./orchestrator/stage_commit.sh "stage 9 (3a)" "review of saw extraction"

set -euo pipefail

if [[ $# -lt 2 ]]; then
    echo "usage: $0 <stage-tag> <message>" >&2
    exit 2
fi

STAGE_TAG="$1"
MESSAGE="$2"

WORK_DIR=$(python -c "import yaml,sys; print(yaml.safe_load(open('config/config.yaml'))['working_dir'])")
WORK_DIR="${WORK_DIR/#\~/$HOME}"

cd "$WORK_DIR"

# Stage everything new under the working dir
git add -A

# If nothing changed, exit cleanly without an empty commit
if git diff --cached --quiet; then
    echo "no changes to commit for $STAGE_TAG"
    exit 0
fi

git commit -q -m "${STAGE_TAG}: ${MESSAGE}"
echo "committed: ${STAGE_TAG}: ${MESSAGE}"
```

- [ ] **Step 2: Make executable and smoke-test**

```bash
chmod +x orchestrator/stage_commit.sh
# Create a fake artifact and commit it
echo "# stub" > ~/synth-research/OB-Xd-oscillator-analysis/00-stub.md
./orchestrator/stage_commit.sh "stage 0" "smoke test"
# Re-run; should be a no-op
./orchestrator/stage_commit.sh "stage 0" "smoke test"
# Clean up the stub
rm ~/synth-research/OB-Xd-oscillator-analysis/00-stub.md
./orchestrator/stage_commit.sh "stage 0" "remove stub"
```

Expected: first call commits, second prints `no changes to commit for stage 0`, third commits the deletion.

- [ ] **Step 3: Commit**

```bash
cd ~/agents/juce-agent
git add methodologies/obxd-oscillator-analysis/orchestrator/stage_commit.sh
git commit -m "feat(obxd-analysis): add per-stage commit helper"
```

---

### Task 25: Orchestrator runbook

**Files:**
- Create: `orchestrator/runbook.md`

- [ ] **Step 1: Write the runbook**

Create `orchestrator/runbook.md`:

````markdown
# Orchestrator Runbook

This runbook is what the main Claude Code session (the "orchestrator")
follows to execute the OB-Xd oscillator analysis methodology. The
orchestrator is a Claude session running in this directory, dispatching
subagents via the Agent tool. The orchestrator does NOT delegate
verification, dispatch, commits, or pre-flight to subagents — those happen
in the parent context.

## Step 0 — Pre-flight

```bash
cd ~/agents/juce-agent/methodologies/obxd-oscillator-analysis
python -m orchestrator.preflight config/config.yaml
```

If pre-flight fails: surface the errors to the user and STOP. Do not
launch any subagents.

## Step 1 — Setup working dir

```bash
./orchestrator/setup_workdir.sh config/config.yaml
```

Idempotent. Working dir lives at `~/synth-research/OB-Xd-oscillator-analysis/`.

## Step 2 — Stage 1 (Repo Map), with reuse check

If `~/synth-research/OB-Xd-oscillator-analysis/01-repo-map.md` exists:

```bash
python -c "
from orchestrator.repo_validate import validate_reuse
import pathlib
result = validate_reuse(
    pathlib.Path('~/synth-research/OB-Xd-oscillator-analysis/01-repo-map.md').expanduser(),
    pathlib.Path('~/synth/OB-Xd-2.19/Source/Engine').expanduser(),
)
print('REUSE' if result.reusable else 'RERUN', result.reasons)
"
```

If REUSE: skip Stage 1 dispatch.
If RERUN: dispatch Stage 1 subagent now.

If 01-repo-map.md does NOT exist: dispatch Stage 1 subagent now.

**Dispatching Stage 1:** read `prompts/stage-01-repo-map.md`. Substitute
`{TARGET_REPO_PATH}`, `{ABS_PATH_TO_CONFIG}`, `{WORKING_DIR}`. Append the
contents of `prompts/_common.md`. Pass the rendered prompt to the Agent
tool with `subagent_type: general-purpose`.

After the subagent returns:
1. Check that `01-repo-map.md` exists in the working dir.
2. If yes and no `## FAILED` section: success. Run `./orchestrator/stage_commit.sh "stage 1" "repo map"`.
3. If yes and `## FAILED` section present: see Failure Handling below.
4. If no: artifact missing. Treat as failure.

## Step 3 — Parallel fan-out (Stages 2 through 8)

After Stage 1 succeeds, dispatch Stage 2 (Entry Trace). After Stage 2
succeeds, dispatch the parallel batch:

- 3a Saw, 3b Pulse, 3c Triangle, 3d Composite
- 4a Voice Alloc, 4b Voice Routing, 4c Voice Wrapper
- 5 Anti-alias
- 7 Parameters
- 8 Adjacent

Send these as parallel Agent tool calls in a single message (the Claude
Code Agent tool supports parallel calls). Stage 6 is NOT in this batch —
it depends on 3d + 4a + 4b + 4c.

For each subagent:
1. Read its prompt template from `prompts/`.
2. Substitute placeholders.
3. Append `_common.md`.
4. Dispatch via Agent tool.

After each subagent returns: verify artifact, commit per Step 2's pattern,
or trigger failure handling.

## Step 4 — Stage 6 (Analog Character)

ONLY run after 3d, 4a, 4b, 4c have all succeeded. If any of those is in
FAILED state, mark Stage 6 as **DEFERRED** (do not run with gaps; analog
character is the easiest section to fabricate). Write a marker:

```bash
cat > ~/synth-research/OB-Xd-oscillator-analysis/06-analog-character.md <<EOF
## DEFERRED — depends on FAILED <stage(s)>
EOF
./orchestrator/stage_commit.sh "stage 6" "DEFERRED"
```

Otherwise, dispatch Stage 6 subagent. It reads only artifacts (no source files).

## Step 5 — Stage 9 (Reviewers, parallel)

After every extraction artifact (3a, 3b, 3c, 3d, 4a, 4b, 4c, 5, 6, 7, 8)
is in PASS or FAILED state, dispatch one reviewer subagent per artifact in
parallel. Skipped artifacts (Stage 6 DEFERRED) get no reviewer.

For each reviewer:
1. Parse the extraction artifact's citations to extract a list of cited
   file:line references.
2. Filter the file paths against `read_declaration_only` from
   `source-map.yaml`. Drop any matches.
3. Build the reviewer's allowlist as the filtered set of unique file paths.
4. Read `prompts/stage-09-reviewer.md`, substitute `{ARTIFACT_PATH}`,
   `{ALLOWLISTED_PATHS}`, `{WORKING_DIR}`, `{ARTIFACT_BASENAME}`.
5. Dispatch.

After each reviewer returns: commit `09-review/<basename>` per the same
pattern.

## Step 6 — Stage 10 (Synthesis), with split fallback

Before dispatching Stage 10: estimate the input size. Sum the byte sizes
of all extraction artifacts + all review artifacts + config.yaml + any
FAILED markers. If the rough token estimate (bytes / 4) exceeds 150k,
SPLIT.

**Single (no split):** dispatch one synthesis subagent reading the full
artifact set. Output: `10-final-spec.md` and `BLOCKERS.md`.

**Split (10a + 10b):**
1. Dispatch 10a with INPUTS = all extraction + review artifacts + config.
   OUTPUT = a partial `10-final-spec.md` filling sections 1–8 and 11,
   with placeholder headers `## 9. Portability Guide [PENDING 10b]`,
   `## 10. Scaling Guidance [PENDING 10b]`, `## 12. Open Questions & Unknowns [PENDING 10b]`,
   `## 13. Prototype Plan [PENDING 10b]`, `## Appendix A. Artifact Provenance [PENDING 10b]`.
   Plus partial `BLOCKERS.md`.
2. Wait for 10a to return. Verify partial spec exists and has the right
   placeholder headers.
3. Dispatch 10b with INPUTS = partial spec + 06, 07, 08 artifacts + all
   reviews. INSTRUCTION: read partial spec, fill placeholder sections in
   place, write back to same path. Append to BLOCKERS.md under
   `## From Stage 10b` (do not overwrite).
4. Wait for 10b to return. Verify the merged spec: every original 10a
   section is structurally unchanged (every header present, every content
   line present), every placeholder is resolved, no stray placeholders.

## Step 7 — Post-Stage-10 verification (orchestrator runs this in parent context)

1. Read `10-final-spec.md`. Verify:
   - All 13 numbered sections are present (1 through 13)
   - Appendix A is present and non-empty
   - Sections 5, 9, 13 have their `## TODO — human step` sub-sections (or
     a one-line note that no human step is needed)
   - Section 11 (Constants) has dual citations on every row
   - Every `{ALL_CAPS}` parameterization marker in section 2 is resolved
     or referenced in section 9
2. Read `BLOCKERS.md`. If non-empty (anything other than `## No blockers`),
   capture the summary.
3. Final commit:
   ```bash
   ./orchestrator/stage_commit.sh "stage 10" "final synthesis"
   ```
4. Report to the user: artifact path, blocker count, deferred section
   count, TODO-human-step count, and recommended next action.

## Failure Handling

When a subagent returns and its artifact contains `## FAILED — <reason>`
or is missing:

1. Attempt 1 was full scope. **Retry once** with the same prompt minus
   the "Generalization notes" instruction (drop that schema section from
   the prompt). If still failing, drop "Open Questions" too.
2. If retry succeeds: commit and continue.
3. If retry fails (second attempt): write `FAILED-<stage>.md` in the
   working dir with the reason, commit it, and continue with downstream
   stages that don't depend on this one. Do NOT run dependent stages with
   gaps (see Stage 6 DEFERRED rule).

## What the orchestrator does NOT do

- Read source files itself (subagents do that)
- Skip pre-flight or commits
- Dispatch reviewers for non-existent artifacts
- Run Stage 10 before all upstream stages have settled (PASS / PASS-WITH-NOTES / FAIL / DEFERRED)
- Modify prompts at runtime — only substitute the placeholders
- Override a reviewer FAIL — that gets passed to synthesis as-is
````

- [ ] **Step 2: Commit**

```bash
git add methodologies/obxd-oscillator-analysis/orchestrator/runbook.md
git commit -m "doc(obxd-analysis): add orchestrator runbook"
```

---

### Task 26: Final README

**Files:**
- Modify: `README.md`

- [ ] **Step 1: Replace README placeholder with full content**

Replace the contents of `README.md`:

```markdown
# OB-Xd Oscillator Analysis Methodology

A runnable, reliable, portable methodology that extracts the OB-Xd 2.19
oscillator design into a portable specification. Designed to be re-targetable
to any open-source JUCE synth via a config swap.

**Design doc (single source of truth):**
`docs/superpowers/specs/2026-04-07-obxd-oscillator-analysis-design.md`

## What this is

A 10-stage subagent pipeline driven by the main Claude Code session
("the orchestrator"). Each stage is a bounded subagent task with its own
INPUTS list, schema, and contract. Output is a portable oscillator spec at
`~/synth-research/OB-Xd-oscillator-analysis/10-final-spec.md`.

## Dependencies

```bash
pip install pyyaml pytest
```

OB-Xd 2.19 source must be checked out at `~/synth/OB-Xd-2.19/`.

## How to run

1. **Pre-flight:** `python -m orchestrator.preflight config/config.yaml`
2. **Setup working dir:** `./orchestrator/setup_workdir.sh config/config.yaml`
3. **Run the methodology:** open Claude Code in this directory and ask it
   to "execute the OB-Xd oscillator analysis methodology following
   `orchestrator/runbook.md`."

The Claude session reads the runbook and dispatches the 10 stages, runs
per-stage commits, handles failures, and produces the final spec.

## How to re-target to a different synth

Per design doc Section 4:
1. Edit `config/config.yaml` (target_repo_path, synth name/version/license).
2. Edit `config/source-map.yaml` (map each stage to the new repo's files).
3. Re-run pre-flight; fix any errors.
4. Run the methodology.

The 10 stages, schemas, prompts, and conflict rules don't change.

## Tests

```bash
python -m pytest tests/ -v
```

Tests cover the Python validators (preflight + repo_validate). Markdown
schemas, prompts, and the runbook are content — they're reviewed by hand,
not unit-tested.

## File map

| Path | Purpose |
|---|---|
| `config/config.yaml` | OB-Xd target config |
| `config/source-map.yaml` | OB-Xd source file → stage mapping |
| `orchestrator/preflight.py` | Pre-flight validator (Python, tested) |
| `orchestrator/repo_validate.py` | Stage 1 reuse check (Python, tested) |
| `orchestrator/setup_workdir.sh` | One-time working dir + git init |
| `orchestrator/stage_commit.sh` | Per-stage commit helper |
| `orchestrator/runbook.md` | Step-by-step orchestrator instructions |
| `schemas/*.schema.md` | 13 artifact schemas |
| `prompts/*.md` | 14 subagent prompt templates + `_common.md` |
| `tests/` | pytest suite for the Python validators |
```

- [ ] **Step 2: Run the full test suite once more**

Run: `python -m pytest tests/ -v`
Expected: all tests pass (or skip if OB-Xd source missing).

- [ ] **Step 3: Commit**

```bash
git add methodologies/obxd-oscillator-analysis/README.md
git commit -m "doc(obxd-analysis): add full README"
```

---

## Self-Review

Performed against the design doc at `docs/superpowers/specs/2026-04-07-obxd-oscillator-analysis-design.md`.

**1. Spec coverage:**
- Section 1 (Pipeline & Artifacts): 10 stages → schemas in Phase 5, prompts in Phase 6, runbook orchestration in Task 25 ✓
- Section 2 (Subagent Contracts): contract template → `_common.md` (Task 15), token budgets → encoded in each stage prompt (Tasks 16–22), failure handling → runbook (Task 25) ✓
- Section 3 (Verification & Synthesis): synthesis contract → Task 22, conflict rules → in synthesis prompt, BLOCKERS.md → schema 13 ✓
- Section 4 (Generalization Layer): config.yaml → Task 2, source-map.yaml → Task 3, pre-flight validator → Phase 3, Stage 1 reuse → Phase 4, re-targeting checklist → README ✓
- Per-stage commits → `stage_commit.sh` (Task 24) + runbook
- Working dir as separate git repo → `setup_workdir.sh` (Task 23)
- Split-synthesis fallback → runbook Step 6
- READ-DECLARATION-ONLY enforcement → in `_common.md` and Stage 5 prompt

All 8 user requirements covered.

**2. Placeholder scan:** No `TBD`, `TODO` (in plan-instruction sense), or "implement later" tokens in the plan itself. The `## TODO — human step` markers in the synthesis prompt and final spec schema are intentional content (instructions for the synthesis subagent) — they're flagged for the engineer in Task 22 as required, not as placeholders.

**3. Type / signature consistency:**
- `preflight.load_config(path) -> dict` — used in Task 4, 5, 6, 7 consistently
- `preflight.validate_required_stages(cfg, sm) -> list[str]` — defined Task 5, used Task 7
- `preflight.validate_filesystem_paths(cfg, sm) -> list[str]` — defined Task 6, used Task 7
- `preflight.main(argv) -> int` — defined Task 7
- `repo_validate.enumerate_files(path) -> list[tuple[str, int]]` — defined Task 8
- `repo_validate.parse_repo_map(path) -> list[tuple[str, int]]` — defined Task 8
- `repo_validate.validate_reuse(...) -> ReuseResult(reusable, reasons)` — defined Task 8
- Schema filenames `01-...schema.md` through `13-...schema.md` consistent across Phase 5 and Phase 6 prompt references
- Prompt template filenames `stage-01-...md` through `stage-10-...md` consistent across Phase 6 and the runbook in Task 25

No drift detected.

---

## Execution Handoff

Plan complete and saved to `docs/superpowers/plans/2026-04-07-obxd-oscillator-analysis.md`. Two execution options:

**1. Subagent-Driven (recommended)** — I dispatch a fresh subagent per task, review between tasks, fast iteration

**2. Inline Execution** — Execute tasks in this session using executing-plans, batch execution with checkpoints

**Which approach?**
