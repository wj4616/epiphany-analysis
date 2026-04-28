<prompt>
  <meta source="prompt-cog"/>

  <role>
You are a principal Claude Code skill architect with deep knowledge of subagent pipeline design, multi-model routing, and the epiphany-genius skill architecture. Your audience is a Claude Code agent with filesystem access; it must not ask clarifying questions — resolve all ambiguities using the constraints provided in this prompt. You write with precision and economy: no filler, no hedging, no over-explanation. Your deliverables are complete and production-ready on first output.
  </role>

  <context>
### epiphany-genius Architecture

epiphany-genius is a subagent orchestrator skill. It executes a wave-based pipeline in which each stage is defined in index.json (stage ID, module file path, KB sources, output section requirements). Stage modules are markdown files that define a genius perspectives system — named cognitive viewpoints that each stage applies to produce its output. KB sources are imported per stage from the skill's knowledge base directory. The orchestrator reads index.json, dispatches each stage as a subagent, collects outputs into a session directory, and passes accumulated context forward. The genius perspectives sections in module files are the reasoning layer; they must not be touched by the hat system.

### Hat System Design Principle

The hat system is a routing layer orthogonal to the reasoning layer. Hats define which model tier executes a stage's work. The genius perspectives system defines what cognitive mechanism is applied within that stage. These two systems must coexist without interference: genius perspectives and KB source assignments remain untouched. A hat is a named cognitive task type carrying a default model assignment, an optional escalation model, and a confidence threshold. The hat registry (hats.json) maps hat names to model assignments and is the sole location where model names appear. Stage module files never contain model names.

The `confidence_threshold` field defines the minimum fraction of a stage's `required_output_sections` that must be present and non-empty in the stage output for the stage to pass without escalation. A value of `0.80` means 80% of required sections must be present. The orchestrator evaluates this after stage completion; if the fraction falls below the threshold, the stage is re-run using the hat's `escalation_tier` model (if `escalation_tier` is not null).

### Model Tier Definitions

- `--model-large`: Full-strength model. Defaults to `claude-opus-4-7`. For `ollama`, this is the largest capable model the user has pulled (e.g., `GLM-5.1:cloud`, `qwen3.5:27b`).
- `--model-medium`: Mid-tier model. Defaults to `claude-sonnet-4-6`. For `ollama`, an intermediate variant (e.g., `qwen3.5:latest`).
- `--model-small`: Lightweight model. Defaults to `claude-haiku-4-5-20251001`. For `ollama`, a fast small model.

When no flags are set, the orchestrator uses `claude` defaults silently: large → `claude-opus-4-7`, medium → `claude-sonnet-4-6`, small → `claude-haiku-4-5-20251001`.

### Six Hat Names (anchored before task)

The hat taxonomy is purpose-built for the pipeline's actual cognitive task types:

1. **Enumerator** — Structured state loading and enumeration (S1). Assigned to `--model-small` tier by default. **Critical-path note:** S1 output feeds all downstream stages; despite small-tier assignment, set a higher `confidence_threshold` (recommended: 0.90) to catch incomplete state loading before it propagates.
2. **Explorer** — Divergent association and domain bridging (S2, S3, and conditional stages). Assigned to `--model-small` tier by default.
3. **Simulator** — Exhaustive boundary and scenario analysis (S4). Assigned to `--model-medium` tier by default.
4. **Precision** — Deep vocabulary stripping and specification work (S5). Assigned to `--model-medium` tier by default.
5. **Adversary** — Falsification and contradiction detection (S6). Assigned to `--model-large` tier by default. This is where confirmation bias is caught or missed; no lighter-tier assignment is permitted without a downstream rescue signal. **Max-tier note:** `escalation_tier` is `null` for this hat — no higher tier exists. On confidence gate failure, emit advisory and accept output without re-running.
6. **Synthesizer** — Integration and distillation (S7 and OSP). Assigned to `--model-small` tier by default; novelty is not required since the stage operates on top of preceding stage outputs.

### WebUI Readiness Architecture

The hat registry (hats.json) and the session directory filesystem are the natural WebUI API surface. Any future WebUI reads hats.json to render the model configuration UI and reads `session_dir/stages/*.md` files to display pipeline progress in real time. The skill's file-based architecture is not incidental — it is the integration contract. Design hats.json and the session manifest with this consumer in mind: clean key names, consistent field types, no embedded markdown in JSON values.

The skill runs standalone in `claude` / `claude code` and is kept separate from any future webui version. An AI agent will later analyze the standalone skill to migrate changes to the webui version; the standalone skill's file-based design must support this migration path without refactoring.

### Ollama / Claude Provider Detection

At runtime, inspect the value supplied to each model flag:
- If the value starts with `claude-`, route through Claude. This takes precedence even if the value also contains `:`.
- If the value contains `:` or is a bare name without the `claude-` prefix, route through `ollama` (e.g., `GLM-5.1:cloud`, `qwen3.5:27b`, `qwen3.5:latest`).
- If the value is a bare single word with no colon and no `claude-` prefix (e.g., `mistral`), the routing is ambiguous — emit disambiguation guidance and halt.

The current `claude` / `claude code` session is assumed to be started with `claude` (not launching `ollama` directly). Mixed routing — some hats using Claude models, others using `ollama` — is supported by design.

### Orchestrator Dispatch Interface

The orchestrator calls `scripts/resolve-hat.sh` before spawning each stage to determine the resolved model:

```
resolved_model=$(scripts/resolve-hat.sh <hat_name> [--model-large <val>] [--model-medium <val>] [--model-small <val>])
```

Exit code 0: `resolved_model` contains the single model string. Non-zero: HALT with stderr message already emitted by the script.

- **Claude models** (`resolved_model` starts with `claude-`): spawn the stage as a subagent using the Agent tool with `model` set to `resolved_model`.
- **Ollama models**: pass the stage module content plus accumulated context as stdin to `ollama run <resolved_model>`, capturing stdout to the stage output file via Bash tool.

### Module and KB File Origins

Module files for epiphany-cogenius are copied from `~/.claude/skills/epiphany-genius/modules/` without modification. The hat system does not alter module file content; genius perspectives sections remain byte-for-byte identical to the originals. KB sources are referenced from `~/.claude/skills/epiphany-genius/kb/`; set `kb_base` in the epiphany-cogenius index.json to `~/.claude/skills/epiphany-genius/kb/`.
  </context>

  <task>
Design and specify the complete epiphany-cogenius skill: a production-grade extension of epiphany-genius that introduces a hat-based model routing layer. Work through the following six sub-sections in order.

**(1) Background and Constraints**
Summarize the problem: epiphany-genius currently allocates the same model uniformly across all pipeline stages, wasting cost and latency on stages that do not require deep reasoning. epiphany-cogenius eliminates this by assigning each stage a hat that carries a model tier assignment. Confirm the non-regression constraint: no file in `~/.claude/skills/epiphany-genius/` is modified.

**(2) Hat Registry Specification**
Define the complete hats.json schema. For each of the six hats (Enumerator, Explorer, Simulator, Precision, Adversary, Synthesizer), specify: `hat_name`, `default_tier`, `escalation_tier`, `confidence_threshold`, `description`. Provide one complete example entry as valid JSON before specifying all six entries. Set `escalation_tier` to `null` for hats whose `default_tier` is already `model-large`; `resolve-hat.sh` must detect a null `escalation_tier` and emit the large-tier advisory without attempting a model lookup.

**(3) Stage-to-Hat Mapping**
Apply the CoT scaffold to every stage in index.json:
1. Identify the primary cognitive operation.
2. Match to the hat taxonomy.
3. Verify model tier is appropriate for that operation's reasoning depth.
4. Note confidence gate threshold if the stage is on the critical path.

Produce the epiphany-cogenius index.json as a new file based on the epiphany-genius index.json structure. Add a `hat` field to each entry in both `stages` and `conditional_modules`. Do not remove or alter any existing field. Set `kb_base` to `~/.claude/skills/epiphany-genius/kb/`.

**(4) Model Tier Flag Interface**
Specify the complete flag resolution logic in `scripts/resolve-hat.sh`: inputs, priority hierarchy (explicit flag > hat default > global default), escalation order (small → medium → large), flag precedence (last flag wins), and output (single model string to stdout).

The call interface is: `scripts/resolve-hat.sh <hat_name> [--model-large <val>] [--model-medium <val>] [--model-small <val>]`

**(5) Ollama/Claude Integration Logic**
Specify provider detection as implemented in `scripts/resolve-hat.sh`. Distinguish two ollama failure modes explicitly:
- **Service unavailable**: `ollama list` command fails entirely → HALT: "ollama service is not running. Start it with: ollama serve"
- **Model not found**: `ollama list` succeeds but the model is absent → HALT: "Model [name] not found in ollama. Run: ollama pull [name]"

For Claude routing: emit model string directly. Mixed routing must work without configuration changes.

**(6) WebUI Readiness Contract**
Specify the session manifest format written to `session_dir/manifest.json`. Required fields: `skill_version` (read from the `version` key in SKILL.md frontmatter at session start), `session_id`, `flags` (object with underscore keys: `model_large`, `model_medium`, `model_small` — corresponding to the `--model-large`, `--model-medium`, `--model-small` CLI flags), `stages` (array: `stage_id`, `hat`, `resolved_model`, `status`, `output_path`). All values scalar or arrays of scalars. Confirm hats.json is the sole external configuration surface.
  </task>

  <constraints>
DO:
- Default to `claude-opus-4-7`, `claude-sonnet-4-6`, `claude-haiku-4-5-20251001` when no flags are set.
- Support `ollama` models (`GLM-5.1:cloud`, `qwen3.5:27b`, `qwen3.5:latest`) by explicit flag assignment.
- Write all stage outputs to `session_dir/stages/` using consistent filenames.
- Expose hats.json as the sole external configuration surface for model assignment.
- Include a session manifest at `session_dir/manifest.json`.
- Support mixed Claude/`ollama` routing across hats within a single session.
- Create all deliverable files under `~/.claude/skills/epiphany-cogenius/` as the install root.

DO NOT:
- DO NOT modify any file in `~/.claude/skills/epiphany-genius/`.
- DO NOT hardcode model names in stage module files — resolve all model assignments at runtime from hats.json.
- DO NOT alter genius perspectives sections in module files — the hat system must coexist with existing genius perspectives orthogonally.
- DO NOT trigger escalation except on confidence gate failure.
- DO NOT build any webui component — the skill must run standalone in claude code.
- DO NOT introduce more than six hats unless a seventh is strictly necessary and justified.
- DO NOT embed markdown formatting inside JSON field values.
- DO NOT loop on escalation exhaustion — after large-tier failure, accept with advisory.
- DO NOT assume models exist in ollama — verify via `ollama list` before routing.
- DO NOT issue `ollama pull` commands — the user manages their own model library.
- DO NOT launch ollama from within the skill — assume the current claude code session was started with `claude`.
- DO NOT couple the skill to any webui component — keep the file-based architecture clean to support future AI-agent migration.
  </constraints>

  <output_format>
Produce exactly four deliverables, each preceded by its filename as a level-2 heading. All files are created under `~/.claude/skills/epiphany-cogenius/` as the install root.

**SKILL.md** — Orchestrator file: frontmatter, role description, invocation syntax including `--model-large`, `--model-medium`, `--model-small`, pipeline overview referencing the hat system, session directory layout, reference to hats.json.

**hats.json** — Valid JSON. Six entries. Fields per entry: `hat_name`, `default_tier`, `escalation_tier`, `confidence_threshold`, `description`. No embedded markdown in any value. `escalation_tier` is `null` for hats at the large-tier ceiling.

**index.json** — Valid JSON. New file for epiphany-cogenius based on the epiphany-genius index.json structure. Each entry in `stages` and `conditional_modules` gains a `hat` field. No other field is altered or removed. `kb_base` points to `~/.claude/skills/epiphany-genius/kb/`.

**scripts/resolve-hat.sh** — Shell script. Reads hats.json. Accepts hat name and optional flag values. Applies priority hierarchy. Emits single model string to stdout. Performs provider detection and `ollama` availability check. Emits HALT to stderr and exits non-zero if model unavailable or service not running. Emits advisory to stderr and exits 0 when escalation_tier is null.
  </output_format>

  <verification>
Before finalizing, self-check:
1. No lighter-tier hat assigned to Adversary or Precision stage without a downstream rescue signal.
2. hats.json is valid JSON; all six hat names appear; all `hat` field values in index.json match exactly one hats.json entry.
3. No file in `~/.claude/skills/epiphany-genius/` was modified or proposed for modification.
4. `resolve-hat.sh` handles: all three flags set, only some flags set, no flags set, conflicting flags (last wins).
5. Session manifest schema contains no embedded markdown and no nested objects beyond one level.
6. Tech version strings `GLM-5.1:cloud`, `qwen3.5:27b`, `qwen3.5:latest` appear verbatim.
7. `resolve-hat.sh` produces the correct model string for all five cases: (a) all three flags set; (b) only `--model-large` set; (c) no flags set (uses defaults `claude-opus-4-7` / `claude-sonnet-4-6` / `claude-haiku-4-5-20251001`); (d) mixed Claude and ollama flags in the same session; (e) hat with `escalation_tier: null` emits advisory and accepts output rather than attempting a model lookup.
  </verification>

  <tests>
Implement three smoke tests alongside the skill deliverables. Place them in `scripts/test-resolve-hat.sh`.

1. **Default resolution** — Invoke `scripts/resolve-hat.sh Adversary` with no flags. Assert stdout equals `claude-opus-4-7`. Assert exit code 0.
2. **Flag override** — Invoke `scripts/resolve-hat.sh Adversary --model-large qwen3.5:27b`. Assert stdout equals `qwen3.5:27b`. Assert exit code 0. (Requires ollama with qwen3.5:27b pulled; document as optional / mock-able in the test script.)
3. **Ollama model absent** — Invoke `scripts/resolve-hat.sh Explorer --model-small nonexistent:model`. Assert stderr contains "not found in ollama". Assert exit code non-zero.
  </tests>

  <edge_cases>
1. **`ollama` model not found** — HALT: "Model [name] not found in ollama. Run: ollama pull [name]". Exit non-zero.
2. **`ollama` service not running** — HALT: "ollama service is not running. Start it with: ollama serve". Exit non-zero.
3. **Escalation exhausted after large-tier failure** — Accept output; emit advisory: "Stage [id] completed below confidence threshold after large-tier escalation. Output accepted with advisory." Do not re-run.
4. **`--model-large` and `--model-medium` set to same value** — Allowed, not an error.
5. **No flags set** — Use `claude` defaults silently: `claude-opus-4-7` / `claude-sonnet-4-6` / `claude-haiku-4-5-20251001`.
6. **Ambiguous model flag value** — A bare single word with no colon and no `claude-` prefix (e.g., `--model-small mistral`) is ambiguous. Emit: "Flag value [value] is ambiguous. Prefix with 'claude-' for Claude routing or use 'name:tag' format for `ollama` routing." Halt.
7. **Sub-agents within a stage wearing independent hats** — Sub-agent hat assignments are independent of parent stage hat; both use the same hats.json registry and priority hierarchy.
8. **Value starts with `claude-` and contains `:`** — Claude routing takes precedence; `claude-` prefix is the authoritative signal.
9. **Hat with null escalation_tier on confidence gate failure** — Do not attempt a model lookup. Emit: "Stage [id] at max model tier. Confidence gate not met; output accepted with advisory." Accept output and continue.
  </edge_cases>
</prompt>
