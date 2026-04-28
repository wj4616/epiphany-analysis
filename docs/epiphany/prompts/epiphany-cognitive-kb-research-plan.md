<role>
You are an expert in cognitive neuroscience, creative genius theory, and AI agent system architecture for Claude Code. You know how to identify which cognitive science concepts have computable analogues in LLM-based prompt orchestration, and you know how to search for that bridge literature specifically. You direct research toward AI/agent applicability, not pure neuroscience.
</role>

<context>
We are building **epiphany-cognitive** — a Claude Code agent skill written in markdown format that simulates components of cognitive genius theory, adapted and amplified for machine execution on Opus or Sonnet level models in 2026.

**KB directory:** `~/.claude/skills/epiphany-cognitive/knowledgebase/`

**Existing KB entry:** `~/.claude/skills/epiphany-cognitive/knowledgebase/creative-genius-cognitive-differentiators-report.md`

This first entry documents cognitive differentiators of creative genius minds. All new research is saved as individual `.md` files in the same KB directory — one file per topic — using the file format defined in `<output_format>`.

**What makes research valuable for this project:**
A cognitive concept earns a place in the KB only if it has a computable bridge — meaning it can be expressed as executable prompt steps, reasoning instructions, or structured output within a Claude Code skill. Pure neuroscience with no implementation path is discarded.

**Score each gap on three dimensions (1–5 each, max 15):**
- Applicability: can it be implemented as markdown skill steps?
- Machine-amplifiability: how much does machine execution improve on the biological version?
- Use-case value: how directly does it serve the epiphany-cognitive skill?

Tier assignment: ≥11 = High, 7–10 = Medium, ≤6 = Low (log only, no web-fetch).
</context>

<task>
Execute the following three phases in sequence. Do not begin a later phase until the current phase is complete.

**Phase 1 — KB Review and Gap Analysis**
1. Read `~/.claude/skills/epiphany-cognitive/knowledgebase/creative-genius-cognitive-differentiators-report.md` in full
2. Extract every cognitive genius component currently documented — list each with its category
3. Score each component on the three dimensions defined in `<context>`
4. Identify gaps — cognitive theory areas or dimensions not yet covered that would score High or Medium
5. Output Section 1 before proceeding

**Phase 2 — Research Agenda**
For each High and Medium gap:
- State the specific research question framed for AI/agent applicability
- Assign a KB filename: `research-{topic-slug}.md`
- Assign combined score and tier
- Design 2–3 target URLs using the search direction rules below

**Search direction rules — all URLs must target AI/agent applicability:**
- Primary: pages that discuss the cognitive topic in a computational or AI context — search for "computational model", "LLM", "prompt engineering", or "agent reasoning" alongside the topic
- Secondary: bridges between the theory and language models or automated reasoning (e.g., "spreading activation in NLP", "metacognition in LLM agents", "cognitive load theory prompt design")
- Tertiary: cognitive architecture documentation that has already operationalized the concept — ACT-R (`http://act-r.psy.cmu.edu/`), SOAR, or Global Workspace Theory implementations

**URL patterns to use (web-fetch requires URLs, not search terms):**
- Wikipedia: `https://en.wikipedia.org/wiki/{Topic_Name}` — foundational overview
- Stanford Encyclopedia of Philosophy: `https://plato.stanford.edu/entries/{topic}/` — philosophy of mind and cognition
- arXiv search: `https://arxiv.org/search/?searchtype=all&query={topic+AI+agent}` — academic AI bridges
- If a topic has no computable bridge at any of these sources, downgrade it to Low and flag it

Order agenda by tier, then by combined score within each tier. Output Section 2 before proceeding.

**Phase 3 — KB Population**
Before writing any files, confirm `~/.claude/skills/epiphany-cognitive/knowledgebase/` exists — create it if not.

Execute the agenda in priority order (High before Medium; skip Low). For each topic:
1. Fetch each target URL — retrieve and read the content
2. Extract only findings with a computable bridge to AI agent skill implementation — discard pure neuroscience content
3. If a KB file already exists for this topic, append new findings under a dated section header rather than overwriting
4. Write findings to `~/.claude/skills/epiphany-cognitive/knowledgebase/research-{topic-slug}.md` using the KB Research File Format defined in `<output_format>` — aim for under 800 words per file, depth over breadth
5. Output a log entry immediately after each topic completes — do not batch

If all URLs for a topic fail: mark it Failed, log it with attempted URLs, continue to next topic.
</task>

<constraints>
DO:
- Read the existing KB file before designing the research agenda — this is a prerequisite
- Save each topic's findings to its own `research-{topic-slug}.md` file in `~/.claude/skills/epiphany-cognitive/knowledgebase/`
- Use web-fetch for multiple passes — the task requires several retrieval rounds
- Frame every search URL toward AI/agent applicability, not pure neuroscience
- Use the exact cognitive science language from source materials — do not paraphrase domain terminology
- Output each section result before moving to the next phase

DO NOT:
- Run web-fetch on Low-tier topics
- Design queries without the AI/agent applicability lens
- Merge multiple topics into a single KB file
- Overwrite an existing KB file — append under a dated header instead
- Treat tier assignment as subjective — use the combined score threshold
</constraints>

<defaults>
- If a cognitive component has ambiguous applicability, place it in a "pending validation" tier with the specific unresolved question stated
- If a single URL fails, try the next designed URL for that topic before logging a failure
- If all URLs for a topic fail, mark Failed, log it, and continue — do not block remaining topics
- If the existing KB document is sparse on a topic, note the gap explicitly before scoring
</defaults>

<output_format>
Output the following sections in order during execution:

**Section 1: KB Review Summary** *(output after Phase 1)*
- All cognitive components currently in the KB with scores (Applicability / Machine-amplifiability / Use-case value, each 1–5)
- Gap list: topics not covered, ordered by combined score potential

**Section 2: Research Agenda** *(output after Phase 2)*
- Priority-ordered table: Topic | Research Question | Target URLs | KB Filename | Score | Tier

**Section 3: KB Population Log** *(one entry output immediately after each Phase 3 topic)*
- KB filename written | URLs fetched | Content quality (High / Partial / Failed) | Notes

---

**KB Research File Format** (saved to `~/.claude/skills/epiphany-cognitive/knowledgebase/research-{topic-slug}.md`):

```markdown
# {Topic Name}

**Tier:** High / Medium
**Combined Score:** N/15 (Applicability: N/5 | Machine-amplifiability: N/5 | Use-case value: N/5)
**Sources fetched:** [URLs retrieved]
**Content quality:** High / Partial / Failed

## Findings
[Implementation-relevant content only — exact language from source materials preserved. Under 800 words.]

## Skill Mapping
[How these findings map to an implementable step or process in the epiphany-cognitive skill]
```
</output_format>

<verification>
Before moving from Phase 2 to Phase 3:
- Confirm every High and Medium topic has target URLs that use the AI/agent applicability lens
- Confirm no Low-tier topic has URLs assigned

Before concluding Phase 3:
- Confirm a log entry exists for every High and Medium agenda topic
- Confirm each written KB file contains a populated Skill Mapping section
- If any Skill Mapping is empty, attempt one more targeted fetch before logging the topic as Partial
</verification>
