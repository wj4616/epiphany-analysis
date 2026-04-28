# CLAUDE.md

Guidance for Claude Code when working in `/home/myuser`. The user's primary work is **agentic engineering** — building, optimizing, and orchestrating skills, subagents, KB retrieval, and prompt pipelines. Software projects (audio plugins, websites, general apps) and research are first-class domains, not the default frame. The agent should be equally capable across all of them.

## User Background

- Strong in Python and JavaScript; C++ beginner.
- Linux Mint primary OS; REAPER used when audio plugin work is in scope.
- Prefers self-contained projects (e.g. CMake `FetchContent` for C++).
- Values: token efficiency, readable code, production-grade quality over feature sprawl.
- Explain non-obvious C++ concepts inline when they appear. Otherwise: no narration.

## Primary Work — Agentic Engineering

Building and refining an agent system: skills, subagent orchestration, KB retrieval, prompt enhancement pipelines, and the workflows that tie them together.

### Skills

Skills live at `~/.claude/skills/`. Active families:

- **Prompt / cognitive pipelines** — `epiphany-*` (cognitive, brainstorm, omnipotent, prompt, genius, graph-genius, cogenius, context, analysis), `prompt-cog`, `prompt-graph`, `socratic-ai-prompt-skill`
- **KB access & maintenance** — `kb-route`, `kb-cognitive`, `kb-validate`, `kb-harvest`, `kb-sync`
- **Web & research** — `firecrawl-*`, `ddg-search-cli-*`, `deep-research-pro-*`
- **Domain skills** — `juce-*` (audio plugins), `comfyui-node-*` (ComfyUI nodes), `claude-api`
- **Workflow** — `writing-plans`, `executing-plans`, `subagent-driven-development`, `dispatching-parallel-agents`, `requesting-code-review`, `receiving-code-review`, `simplify`, `loop`, `schedule`
- **Discovery** — `find-skills` for surfacing what's available

### Subagents

Specialized agents available via the Agent tool: `brainstorm-architect`, `prompt-architect`, `implementation-planner`, `juce-agent`, `claude-code-guide`, plus general `Explore`, `Plan`, `general-purpose`. Use the user's stated rule: don't spawn unless asked or when the task clearly matches a named agent.

### Knowledge Bases — Dify MCP (current)

The system has migrated from the older pattern (skills reading local JSON KBs) to **Dify-hosted MCP servers** for retrieval. Prefer the MCP KBs when the task touches their domain:

- `mcp__dify-cognitive-kb__cognitive-research-kb-dify` — cognitive science / genius-minds research
- `mcp__dify-thought-kb__ToT-GoT-Cot-KB-retrieval` — Chain / Tree / Graph of Thought research

`kb-route` describes the resolution procedure across KB layers; `kb-validate` checks claims before they drive implementation. Local JSON KB lookup still exists in legacy skills but is no longer the canonical path.

## Critical Rules (general)

1. **Don't fabricate** — verify file paths, function names, and KB claims before recommending action.
2. **Confirm before destructive ops** — follow the safety guidance in the system prompt.
3. **Match the scope of the request** — don't refactor or "improve" code that wasn't asked about.
4. **Token efficiency** — concise updates, no narrating internal deliberation, no trailing summaries unless asked.
5. **Memory hygiene** — save user/feedback/project/reference memories per the auto-memory rules; don't save anything derivable from the codebase or git history.

## Build / Test Conventions

Always check the project's actual config before assuming. Common patterns:

```bash
# C++/CMake (audio plugins, native projects)
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
cd build && ctest --output-on-failure

# Python
pytest -q

# Node
npm test
```

## When Working on JUCE / VST Audio Plugins

Audio plugin work is one domain among many — not the assumed default.

> **Note on the legacy system:** `~/agents/juce-agent/` (playbook, validation logs, test scripts) is a *prior* generation of the agent system. Useful for reference, but new work should use the `juce-*` skills directly and the current Dify-based KB layer rather than the playbook's JSON-driven flow.

### Active plugin projects

- `~/CogVST/` — Interdimensional sound design module (active)
- `~/EpiphanyMachine/` — Earlier learning project
- `~/JUCE/` — JUCE framework clone (most projects use `FetchContent` instead)

VST3 builds install to `~/.vst3/`; restart REAPER (or rescan) to pick up changes.

### Phase workflow (reference)

| Phase | Skill | Output |
|-------|-------|--------|
| 0 Spec | `juce-plugin-spec` | SPEC.md |
| 2 Architecture | `writing-plans` | Implementation plan |
| 4 DSP | `juce-dsp-implementation` | DSP modules |
| 7 GUI | `juce-ui-bridge` | Editor, LookAndFeel |
| 9 DAW Test | `juce-daw-testing` | Human perceptual testing |
| Pre-commit | `juce-audio-thread-audit`, `juce-smoothedvalue-audit` | Safety checks |

### Plugin-specific rules

1. Never propose signal flow unprompted — user describes or draws it.
2. Use the user's exact creative language; never paraphrase sound-identity words.
3. Audit gain staging early — multiple conservative stages mask effects.
4. Verify formulas at parameter boundaries (param=0, param=1).
5. Creative decisions are human-only — ask, don't assume.

For sonic-to-DSP translation, use `juce-sound-design-bridge` (e.g. "warm" → low filter cutoff, mild resonance; "bright" → high cutoff; "punchy" → fast attack + filter envelope; "lush" → mild detune + chorus).

## When Working on Other Domains

- **Web / general software** — no enforced playbook. Use `writing-plans` for multi-step tasks, `executing-plans` for cross-session execution, and language-appropriate tooling.
- **Research** — `deep-research-pro-*`, `firecrawl-*` (search/scrape/crawl/map/agent), `ddg-search-cli-*`. Cite sources; validate via KB when applicable.
- **ComfyUI nodes** — `comfyui-node-*` family covers basics, lifecycle, packaging, frontend, advanced patterns, datatypes, inputs/outputs, migration.
- **Claude API / SDK work** — use the `claude-api` skill; default to current Claude 4.x models; include prompt caching.

## Key Locations

| Path | Purpose |
|------|---------|
| `~/.claude/skills/` | All skill definitions |
| `~/.claude/agents/` | Subagent definitions |
| `~/.claude/projects/-home-myuser/memory/` | Auto-memory store (persists across sessions) |
| `~/agents/juce-agent/` | Legacy plugin agent system (reference only) |
| `~/docs/superpowers/` | System analyses, specs, design notes |
| `~/docs/epiphany/` | Outputs from epiphany-* skills |
| `~/CogVST/`, `~/EpiphanyMachine/` | Active and prior plugin projects |
