# Prompt Epiphany Examples

## Example 1: Code Generation Prompt

**BEFORE:**
```
Write a Python function that takes a CSV file and finds duplicate rows.
It should be efficient for large files and return the line numbers of duplicates.
```

**AFTER:**
```
---
<role>
You are a senior Python engineer with expertise in data processing
and memory-efficient file handling.
</role>

<task>
Write a Python function that detects duplicate rows in a CSV file.
Requirements:
1. Accept a file path as input
2. Identify duplicate rows (identical content across all columns)
3. Return 1-based line numbers of every duplicate occurrence
4. Optimize for large files — minimize memory footprint
</task>

<constraints>
- DO optimize for large files
- DO return line numbers of all duplicates
- If efficiency and readability conflict, prioritize efficiency
</constraints>

<defaults>
Unless otherwise specified:
- Use standard library (csv, hashlib)
- Treat first row as header
- Handle degenerate inputs gracefully
</defaults>

<edge_cases>
- Empty file → return empty list
- File not found → raise FileNotFoundError
- Whitespace-only row → treat as valid row
</edge_cases>

<verification>
Verify: handles 1GB CSV without memory errors, correct 1-based line numbers.
</verification>
---
```

## Example 2: Analysis Prompt

**BEFORE:**
```
Summarize the key themes in this quarterly earnings report and flag anything investors should worry about.
```

**AFTER:**
```
---
<role>
You are a senior financial analyst with expertise in earnings report interpretation.
</role>

<task>
Analyze the quarterly earnings report. Produce:
1. Summary of 3-5 key themes (revenue trends, margin shifts, guidance changes)
2. Flagged investor concerns — risk items, negative trajectory, deviation from guidance
</task>

<constraints>
- If a theme is ambiguous, state what makes it ambiguous rather than picking a side
</constraints>

<defaults>
Unless otherwise specified:
- Ground every claim in specific numbers or quotes from the report
- Do not editorialize beyond what the data supports
</defaults>

<output_format>
## Key Themes
1. **[Theme]** — [2-3 sentence summary with supporting data]

## Investor Concerns
- **[Concern]** — [Why this matters, with data point]
  Risk level: [low / moderate / elevated]
</output_format>
---
```

## Example 3: Audit Prompt (Short Input)

**BEFORE:**
```
audit for errors, bugs, issues, or areas for useful improvement in the current project, but without overengineering unless useful
```

**AFTER:**
```
---
<role>
You are a senior code auditor with expertise in C++, JUCE audio plugins, and systematic
quality review. You balance thoroughness with pragmatism.
</role>

<task>
Audit the current project for errors, bugs, issues, and areas for useful improvement.
Produce actionable findings prioritized by impact.
</task>

<constraints>
- DO focus on real problems, not hypothetical ones
- DO prioritize findings by severity: critical bugs > correctness issues > code quality > nice-to-haves
- DO provide specific file:line references for every finding
- DO explain WHY something is a problem, not just THAT it is
- DO NOT suggest changes that add complexity without clear benefit
- DO NOT propose refactoring for its own sake
- DO NOT flag style preferences as issues
</constraints>

<defaults>
Unless otherwise specified:
- Focus on correctness, memory safety, and real-world failure modes
- Prefer simple fixes over architectural changes
- Consider the user's skill level (C++ beginner, knows Python/JS)
- Treat comments in the codebase as truth unless clearly outdated
</defaults>

<edge_cases>
- If a pattern appears intentional but unusual, note it without flagging as error
- If a finding requires domain expertise you lack, state the limitation
- If you cannot determine severity confidently, mark as "needs investigation"
</edge_cases>

<output_format>
## Findings

### Critical (must fix)
- **[Issue]** — `file.cpp:line` — [description] — [why it matters] — [suggested fix]

### Recommended (should fix)
- **[Issue]** — `file.cpp:line` — [description]

### Low Priority (nice to have)
- **[Issue]** — `file.cpp:line` — [description]

## Summary
- Total findings: X critical, Y recommended, Z low priority
- Most impactful area: [area]
</output_format>

<verification>
Verify: every finding has a file:line reference, every severity is justified,
no findings are purely stylistic preferences.
</verification>
---
```

## Example 4: Minimal Mode

**BEFORE (with `--minimal` flag):**
```
audit for errors, bugs, issues, or areas for useful improvement in the current project, but without overengineering unless useful
```

**AFTER:**
```
---
<task>
Audit the current project for errors, bugs, issues, and areas for useful improvement.
Prioritize findings by impact.
</task>

<constraints>
- Focus on real problems, not hypothetical ones
- Severity order: critical bugs > correctness issues > code quality > nice-to-haves
- Provide specific file:line references for every finding
- Explain WHY something is a problem
- Do not suggest changes that add complexity without clear benefit
- Do not flag style preferences as issues
</constraints>
---
```

**What changed:** Quick Analysis identified task + constraints as the core content. Direct Synthesis applied T1 (XML structure with only `<task>` and `<constraints>`), T2 (decomposition into two sections), T3 (structured existing constraints into bullets). Skipped T4 (persona), T8 (edge cases), T9 (examples) — these are creative/depth techniques that minimal foregoes. T5 (output format) and T7 (priority) were eligible but not needed for this prompt — the structured constraint bullets already imply the output structure. All original content preserved in compact form.

## Example 5: Verbose Mode

**BEFORE (with `--verbose` flag):**
```
audit for errors, bugs, issues, or areas for useful improvement in the current project, but without overengineering unless useful
```

**AFTER:**
```
---
<role>
You are a senior code auditor with expertise in C++, JUCE audio plugins, and systematic
quality review. You balance thoroughness with pragmatism.
</role>

<context>
This is a JUCE-based VST3 audio plugin project built on Linux with CMake/FetchContent.
The developer is a C++ beginner (experienced in Python/JS) building their first plugin.
Audio plugins have strict real-time constraints — allocations, locks, and blocking calls
on the audio thread cause glitches audible to users. Code quality issues that would be
minor in a web app can cause hard-to-debug audio artifacts here.
</context>

<task>
Audit the current project for errors, bugs, issues, and areas for useful improvement.
Produce actionable findings prioritized by impact.
</task>

<constraints>
- DO focus on real problems, not hypothetical ones
  *Why: speculative findings waste review time and erode trust in the audit*
- DO prioritize findings by severity: critical bugs > correctness issues > code quality > nice-to-haves
  *Why: developer time is limited — highest-impact fixes should come first*
- DO provide specific file:line references for every finding
  *Why: vague findings ("somewhere in the codebase") are not actionable*
- DO explain WHY something is a problem, not just THAT it is
  *Why: a beginner C++ developer needs context to learn from findings*
- DO NOT suggest changes that add complexity without clear benefit
  *Why: user explicitly requested no overengineering*
- DO NOT propose refactoring for its own sake
  *Why: refactoring without clear benefit adds complexity the user explicitly wants to avoid*
- DO NOT flag style preferences as issues
  *Why: style debates are subjective and distract from real correctness problems*
</constraints>

<defaults>
Unless otherwise specified:
- Focus on correctness, memory safety, and real-world failure modes
- Prefer simple fixes over architectural changes
- Consider the user's skill level (C++ beginner, knows Python/JS)
- Treat comments in the codebase as truth unless clearly outdated
</defaults>

<edge_cases>
- If a pattern appears intentional but unusual, note it without flagging as error
- If a finding requires domain expertise you lack, state the limitation
- If you cannot determine severity confidently, mark as "needs investigation"
- If multiple findings share a root cause, group them under one entry
</edge_cases>

<examples>
**Example finding (critical):**
- **Buffer overrun in processBlock** — `PluginProcessor.cpp:142` — Reads past buffer end when block size < 64 samples. Causes memory corruption and potential crash. Fix: clamp loop to `buffer.getNumSamples()`.

**Example finding (low priority):**
- **Unused include** — `Utils.h:3` — `#include <algorithm>` not referenced. Harmless but adds compile time.
</examples>

<output_format>
## Findings

### Critical (must fix)
- **[Issue]** — `file.cpp:line` — [description] — [why it matters] — [suggested fix]

### Recommended (should fix)
- **[Issue]** — `file.cpp:line` — [description]

### Low Priority (nice to have)
- **[Issue]** — `file.cpp:line` — [description]

## Summary
- Total findings: X critical, Y recommended, Z low priority
- Most impactful area: [area]
</output_format>

<verification>
Verify: every finding has a file:line reference, every severity is justified,
no findings are purely stylistic preferences.
</verification>
---
```

**Flagged issues (shown before the prompt):**
> **Verbose expansion inferred the following — verify these are correct:**
> - Added JUCE/VST3 audio plugin context and real-time constraints explanation
> - Added "Why" explanations for each constraint based on inferred reasoning
> - Added two example findings to demonstrate expected format

**What changed:** Normal pipeline ran first (producing output similar to Example 3). Expansion pass identified three thin spots: sparse context (no domain background), bare constraints (no "why" explanations), and no examples (audit task benefits from demonstrating expected finding format). Expanded context with audio plugin domain knowledge, added rationale to each constraint, and added two exemplar findings. Inferred expansions flagged for user review.

## Example 6: Verbose — No Gaps Found

**BEFORE (with `--verbose` flag):**
```
You are an expert PostgreSQL DBA. Analyze this migration plan for a 500M-row table.

Context: Production database, zero-downtime requirement, PostgreSQL 15, read-heavy workload (90% reads).

Task: Review the migration and produce a risk assessment.

Constraints:
- DO NOT suggest approaches requiring downtime
- DO flag any operation that locks the table for >100ms
- If a step has unknown lock duration, estimate conservatively and flag
- Prioritize data integrity over migration speed

Edge cases:
- If migration affects indexes used by critical queries, flag separately
- If rollback is not possible for a step, it must be called out explicitly

Output: Risk matrix with severity, likelihood, mitigation for each risk.

Verify: every risk has a mitigation, no step assumes downtime is acceptable.
```

**AFTER:** Normal enhancement returned with note:

> "Normal enhancement is already comprehensive. Returning standard version."

**What changed:** Normal pipeline ran and made minor structural improvements (XML tags, tightened language). Gap Scan found no thin spots — context is domain-specific, constraints have implicit rationale, edge cases are covered, task is clear. Verbose expansion added no additional content.