---
name: epiphany-brainstorm
version: 1.0.0
last_modified: 2026-04-08
description: "Transforms any input (single-sentence idea → full specification) into enhanced brainstormed context via curated 5–6-stage methodology pipeline (SCAMPER, Morphological Analysis, Six Thinking Hats, TRIZ, Reverse Brainstorming, Pugh Matrix). ONLY on /epiphany-brainstorm or explicit name mention. Do NOT activate for generic 'brainstorm' requests. Supports --minimal / --standard / --deep flags. Outputs XML-structured block; offers save to ~/prompts/brainstorm/."
---

# Epiphany Brainstorm

Takes any input — from a single-sentence idea up to a full product specification with code blocks, tables, and formulas — and produces an enhanced, validated brainstormed analysis. The output is a semantic XML block optimized as context for downstream AI agent consumption (not primarily for human reading). The skill preserves every input detail in a mandatory `<input_inventory>` and runs a curated set of brainstorming methodologies through firewalled "lenses" that stay in their own modes while still being able to reference prior lens outputs for gap detection.

## What this skill is not

- Not a spec writer (use `writing-plans` after brainstorming)
- Not an implementation planner
- Not a code generator
- Not a prompt enhancer (that's `prompt-epiphany`)
- Not a runtime consumer of any other skill — fully standalone
- Not a web researcher — runtime operates offline on the input text only

## Scope boundary

A single `<brainstorm_output_v1>` XML block per invocation. No multi-turn dialogue. No implementation action. The skill enhances text with brainstormed context and emits the block; it does not execute anything the input describes.

## Trigger Conditions

| Trigger | Behavior |
|---|---|
| `/epiphany-brainstorm` | Activate immediately. If no input provided, ask for one. |
| User explicitly says "epiphany-brainstorm" or "epiphany brainstorm" | Activate. Ask for input if not provided. |
| User says "brainstorm this" / "think about" / "ideate" / "enhance" WITHOUT naming this skill | Do NOT activate. Never auto-brainstorm. |
| `/epiphany-brainstorm --minimal` | Activate, force MINIMAL. Flag at first or last token only. |
| `/epiphany-brainstorm --standard` | Activate, force STANDARD. Flag at first or last token only. |
| `/epiphany-brainstorm --deep` | Activate, force DEEP. Flag at first or last token only. |
| Two or more mode flags present | Ask user to pick one before proceeding. |
| Flag mid-sentence within input body | Treated as content, not mode selector. Not stripped. |
| All other cases | Do NOT activate. |

**Input channels:** inline text, file path, or follow-up message.