#!/usr/bin/env bash
# xml-assemble.sh — epiphany-genius v1.1.0
# Usage: xml-assemble.sh <session_dir>
# Assembles stages/output.xml from stage output files, driven by index.json.
# Schema model: envelope per stage wrapping stage markdown (CDATA-escaped).
# Stages not run → empty self-closing elements.
# Called by orchestrator STEP 7 when --xml flag is set.

set -euo pipefail

SESSION_DIR="${1:?Usage: xml-assemble.sh <session_dir>}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SKILL_DIR="$(dirname "$SCRIPT_DIR")"
INDEX_FILE="${SKILL_DIR}/index.json"
STAGES_DIR="${SESSION_DIR}/stages"
OUTPUT_XML="${STAGES_DIR}/output.xml"

if ! command -v python3 &>/dev/null; then
  echo "FAIL: python3 required for xml-assemble.sh"
  exit 1
fi

if [ ! -f "$INDEX_FILE" ]; then
  echo "FAIL: index.json not found at ${INDEX_FILE}"
  exit 1
fi

python3 - "$STAGES_DIR" "$OUTPUT_XML" "$INDEX_FILE" << 'PYEOF'
import json
import os
import re
import sys
from datetime import datetime
from pathlib import Path

stages_dir = Path(sys.argv[1])
output_xml = sys.argv[2]
index_path = sys.argv[3]

with open(index_path) as f:
    idx = json.load(f)


def read_file_safe(path):
    try:
        with open(path) as f:
            return f.read()
    except FileNotFoundError:
        return ""


def xml_escape_attr(text):
    return (text
            .replace("&", "&amp;")
            .replace("<", "&lt;")
            .replace(">", "&gt;")
            .replace('"', "&quot;"))


def cdata_wrap(text):
    """Wrap content in CDATA, escaping any embedded CDATA closers."""
    if not text:
        return ""
    safe = text.replace("]]>", "]]]]><![CDATA[>")
    return f"<![CDATA[{safe}]]>"


def extract_session_field(session_md, field, default="unknown"):
    m = re.search(rf"^{field}:\s*(.+)$", session_md, re.MULTILINE)
    return m.group(1).strip() if m else default


# Read session metadata
session_md = read_file_safe(stages_dir / "session.md")
session_id = extract_session_field(session_md, "session_id",
                                    datetime.now().strftime("%Y%m%d-%H%M%S"))
scale = extract_session_field(session_md, "scale", "STANDARD")
input_type = extract_session_field(session_md, "input_type", "raw")
# Flag values live on separate lines in the manifest
flag_xml = extract_session_field(session_md, "flag_xml", "false")
flag_quiet = extract_session_field(session_md, "flag_quiet", "false")
flag_verbose = extract_session_field(session_md, "flag_verbose", "false")
flag_conjecture = extract_session_field(session_md, "flag_conjecture", "false")
signals = extract_session_field(session_md, "signals", "[]")

# Build list of all stages (core + conditional) in declared order.
# Each entry needs: xml_element, output_file
all_stages = idx.get("stages", []) + idx.get("conditional_modules", [])

# Active conditionals derived from file presence
active_conditionals = []
for entry in idx.get("conditional_modules", []):
    of = entry.get("output_file", "")
    if of and (stages_dir / Path(of).name).exists():
        active_conditionals.append(entry.get("stage_id", ""))

active_cond_str = ", ".join(c for c in active_conditionals if c) or "none"

# Read processed input + V6 scope
processed_input = read_file_safe(stages_dir / "00-processed-input.md")
v6_scope = read_file_safe(stages_dir / "S7-v6-scope.txt")

# --- Build XML ---
lines = []
lines.append('<?xml version="1.0" encoding="UTF-8"?>')
lines.append('<cognitive_output_v1>')

# Schema note — documents the envelope model so downstream consumers
# know what to expect (not structured sub-fields, but stage-wrapped markdown).
lines.append('  <schema_note>')
lines.append('    Envelope model: each stage element wraps the stage\'s full markdown output')
lines.append('    in CDATA. Parse markdown content per stage. Only &lt;meta&gt;, &lt;v6_scope&gt;,')
lines.append('    &lt;active_conditionals&gt;, and &lt;signals&gt; are attribute-style structured.')
lines.append('  </schema_note>')

# meta
lines.append('  <meta>')
lines.append(f'    <skill_version>1.1.0</skill_version>')
lines.append(f'    <session_id>{xml_escape_attr(session_id)}</session_id>')
lines.append(f'    <scale>{xml_escape_attr(scale)}</scale>')
lines.append(f'    <input_type>{xml_escape_attr(input_type)}</input_type>')
lines.append(f'    <flag_xml>{xml_escape_attr(flag_xml)}</flag_xml>')
lines.append(f'    <flag_verbose>{xml_escape_attr(flag_verbose)}</flag_verbose>')
lines.append(f'    <flag_quiet>{xml_escape_attr(flag_quiet)}</flag_quiet>')
lines.append(f'    <flag_conjecture>{xml_escape_attr(flag_conjecture)}</flag_conjecture>')
lines.append(f'    <active_conditionals>{xml_escape_attr(active_cond_str)}</active_conditionals>')
lines.append(f'    <signals>{xml_escape_attr(signals)}</signals>')
lines.append('  </meta>')

# input_inventory
if processed_input.strip():
    lines.append(f'  <input_inventory>{cdata_wrap(processed_input.strip())}</input_inventory>')
else:
    lines.append('  <input_inventory/>')

# Emit one element per stage, in declared order
stage_presence = {}
for entry in all_stages:
    xml_element = entry.get("xml_element")
    stage_id = entry.get("stage_id", "?")
    output_file = entry.get("output_file", "")
    if not xml_element:
        continue  # skip stages without an XML element (OSP has xml_element: null)

    # stage file name is the basename of output_file
    filename = Path(output_file).name if output_file else ""
    content = read_file_safe(stages_dir / filename) if filename else ""
    stage_presence[stage_id] = bool(content.strip())

    if content.strip():
        lines.append(f'  <{xml_element}>{cdata_wrap(content.strip())}</{xml_element}>')
    else:
        lines.append(f'  <{xml_element}/>')

# verification_report — V6 is the only truly structured piece
if stage_presence.get("S7"):
    lines.append('  <verification_report>')
    if v6_scope.strip():
        lines.append(f'    <v6_scope>{cdata_wrap(v6_scope.strip())}</v6_scope>')
    else:
        lines.append('    <v6_scope/>')
    lines.append('    <note>V1-V5 and V7 results are inline in &lt;integration&gt; content.</note>')
    lines.append('  </verification_report>')
else:
    lines.append('  <verification_report/>')

# downstream_handoff — kept for API stability; empty unless populated by future stage
lines.append('  <downstream_handoff/>')

lines.append('</cognitive_output_v1>')

output_content = "\n".join(lines) + "\n"
with open(output_xml, "w") as f:
    f.write(output_content)

# Summary for orchestrator
print(f"XML assembled: {output_xml}")
present = [sid for sid, p in stage_presence.items() if p]
absent = [sid for sid, p in stage_presence.items() if not p]
print(f"  Stages present: {', '.join(present) if present else 'none'}")
print(f"  Stages absent:  {', '.join(absent) if absent else 'none'}")
print(f"  Active conditionals: {active_cond_str}")
PYEOF
