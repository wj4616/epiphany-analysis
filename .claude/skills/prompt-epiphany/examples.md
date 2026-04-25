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

<verification>
Verify: every theme cites specific numbers or quotes from the report,
every concern includes a supporting data point, no editorializing beyond data.
</verification>
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

<examples>
**Example finding (critical):**
- **Buffer overrun in processBlock** — `PluginProcessor.cpp:142` — Reads past buffer end when block size < 64 samples. Causes memory corruption and potential crash. Fix: clamp loop to `buffer.getNumSamples()`.

**Example finding (low priority):**
- **Unused include** — `Utils.h:3` — `#include <algorithm>` not referenced. Harmless but adds compile time.
</examples>

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
> - Added edge case for grouping findings by root cause

**What changed:** Normal pipeline ran first (producing output similar to Example 3). Expansion pass identified four thin spots: sparse context (no domain background), bare constraints (no "why" explanations), no examples (audit task benefits from demonstrating expected finding format), and missing edge cases (root-cause grouping). Expanded context with audio plugin domain knowledge, added rationale to each constraint, added two exemplar findings, and added a fourth edge case. Inferred expansions flagged for user review.

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

## Example 7: Preservation — URLs and File Paths

**BEFORE:**
```
analyze the documentation at https://docs.example.com/api/v2/reference and create a summary.
Also check ~/projects/my-app/docs/README.md for local context.
The app uses React 18.2.0 and Node.js v20.10.0.
```

**AFTER:**
```
---
<task>
Analyze the documentation and create a summary.
</task>

<context>
Documentation sources:
- Web: https://docs.example.com/api/v2/reference
- Local: ~/projects/my-app/docs/README.md

Technology stack:
- Frontend: React 18.2.0
- Runtime: Node.js v20.10.0
</context>

<constraints>
- DO use both sources for comprehensive context
- DO preserve the exact URLs and paths for reference
- DO note version-specific API features where relevant
</constraints>

<output_format>
## Summary
[Key points from documentation]

## Source References
- https://docs.example.com/api/v2/reference — [relevant sections]
- ~/projects/my-app/docs/README.md — [relevant sections]

## Version-Specific Notes
- React 18.2.0: [version-specific features mentioned]
- Node.js v20.10.0: [compatibility notes]
</output_format>
---
```

**What was preserved:**
- URL `https://docs.example.com/api/v2/reference` — appears verbatim in context AND output format
- File path `~/projects/my-app/docs/README.md` — appears verbatim in context AND output format
- `React 18.2.0` — technology + version together, not just "React"
- `Node.js v20.10.0` — technology + version together, not just "Node.js"
- Embedded directive "analyze the documentation" preserved with its targets intact

## Example 8: Preservation — Embedded Directives

**BEFORE:**
```
Crawl https://example.com/blog for recent posts about AI agents,
then fetch https://arxiv.org/abs/2403.13002v2 for the full paper.
Compare findings with the notes in ./research/ai-agents/notes.md.
Using Python 3.11, write a summary.
```

**AFTER:```
---
<task>
Crawl web sources, fetch academic paper, compare with local notes,
and write a summary comparing findings.
</task>

<context>
Sources to analyze:
1. Crawl: https://example.com/blog — recent posts about AI agents
2. Fetch: https://arxiv.org/abs/2403.13002v2 — full academic paper
3. Compare with: ./research/ai-agents/notes.md — local research notes

Technology requirement: Python 3.11
</context>

<constraints>
- DO crawl https://example.com/blog for AI agent content
- DO fetch https://arxiv.org/abs/2403.13002v2 for the complete paper
- DO read ./research/ai-agents/notes.md for local context
- Use Python 3.11 (not earlier versions) for compatibility
</constraints>

<output_format>
## AI Agents: Comparative Summary

### From https://example.com/blog
[Key findings from blog crawl]

### From https://arxiv.org/abs/2403.13002v2
[Key findings from paper fetch]

### Local Notes Alignment (./research/ai-agents/notes.md)
[Comparison with local research]

### Python 3.11 Implementation Notes
[Version-specific considerations]
</output_format>
---
```

**What was preserved:**
- URL `https://example.com/blog` — appears verbatim in context, constraints, AND output format
- URL `https://arxiv.org/abs/2403.13002v2` — appears verbatim in context, constraints, AND output format
- File path `./research/ai-agents/notes.md` — appears verbatim in context, constraints, AND output format
- `Python 3.11` — technology + version together
- Directive "Crawl" preserved with its target URL intact
- Directive "fetch" preserved with its target URL intact
- Directive "compare" preserved with its target path intact

## Example 9: Preservation — Technical Specifications

**BEFORE:**
```
Build a REST API using FastAPI 0.109.0 with PostgreSQL 16.
Must handle 10,000 requests/second with p99 latency under 50ms.
Follow patterns in ~/dev/api-patterns/README.md.
```

**AFTER:**
```
---
<task>
Build a REST API that meets performance requirements.
</task>

<context>
Technology specifications:
- Framework: FastAPI 0.109.0
- Database: PostgreSQL 16
- Patterns reference: ~/dev/api-patterns/README.md

Performance requirements:
- Throughput: 10,000 requests/second
- p99 latency: under 50ms
</context>

<constraints>
- DO use FastAPI 0.109.0 (specific version required)
- DO use PostgreSQL 16 (specific version required)
- DO follow patterns documented in ~/dev/api-patterns/README.md
- DO achieve 10,000 requests/second throughput
- DO maintain p99 latency under 50ms
</constraints>

<verification>
Verify: implementation uses FastAPI 0.109.0 features correctly,
PostgreSQL 16 specific features used where appropriate,
patterns from ~/dev/api-patterns/README.md are followed,
performance meets stated requirements.
</verification>
---
```

**What was preserved:**
- `FastAPI 0.109.0` — framework + version together, NOT just "FastAPI"
- `PostgreSQL 16` — database + version together, NOT just "PostgreSQL"
- `~/dev/api-patterns/README.md` — file path verbatim
- `10,000 requests/second` — numeric specification with unit
- `p99 latency under 50ms` — metric specification preserved exactly

## Example 10: Preservation — Minimal Mode with URLs

**BEFORE (with `--minimal` flag):**
```
check the docs at https://example.com/docs/api and https://example.com/docs/sdk
then analyze the code in ~/src/main.py
```

**AFTER:**
```
---
<task>
Check documentation and analyze source code.
</task>

<constraints>
- Check https://example.com/docs/api
- Check https://example.com/docs/sdk
- Analyze ~/src/main.py
</constraints>
---
```

**What was preserved (minimal still preserves all URLs/paths):**
- `https://example.com/docs/api` — verbatim
- `https://example.com/docs/sdk` — verbatim
- `~/src/main.py` — verbatim

**Note:** Even in minimal mode, ALL preservation items must appear in output. Minimal mode compresses structure and enhancement, NOT content.

## Example 11: Specification Mode

**BEFORE (with `--specification` flag):**
```
I want a user notification system for a web app — things like email confirmations, alerts when something goes wrong, and maybe push notifications someday
```

**Mode routing note:** No flag needed here since `--specification` was provided. Without the flag, the skill would suggest it based on the specification signal ("I want a ... system").

**AFTER:**
```
---
<specification>
<scope>
  Included: User notification system for a web application — delivery, templating, preferences, tracking.
  Excluded: Notification content authorship (business logic that triggers notifications), analytics dashboards beyond delivery status, third-party marketing platforms.
</scope>

<context>
  Domain: Web application — backend notification infrastructure
  Stakeholders:
    - End users: receive notifications, manage preferences, expect timely delivery
    - Application: triggers notifications in response to events
    - Developers: integrate notification system into app logic
    - Operations: monitor delivery, troubleshoot failures
  Background: System handles transactional and operational notifications. Push notifications are out of scope initially but the architecture must not preclude them.
</context>

<functional_requirements>
  FR-1: The system SHALL send email notifications to registered users upon triggering events.
         Verification: Trigger a test event; confirm email arrives at user's registered address within 60 seconds.

  FR-2: The system SHALL send in-app alert notifications when error-class events occur.
         Verification: Trigger an error event; confirm alert appears in user's session within 5 seconds.

  FR-3: The system SHALL deliver email confirmation notifications for account-related actions (registration, password change, email change).
         Verification: Perform each account action; confirm confirmation email is delivered with correct content.

  FR-4: The system SHALL allow users to manage their notification preferences (enable/disable by notification type).
         Verification: User disables email alerts; trigger alert event; confirm no email is sent.

  FR-5: The system SHALL log delivery status for each notification (sent, failed, bounced).
         Verification: Send a batch of notifications; confirm each has a corresponding status log entry.

  FR-6: The system SHALL retry failed notifications at least once before marking as permanently failed.
         Verification: Simulate delivery failure; confirm retry occurs; confirm status transitions to "failed" after retry exhaustion.
</functional_requirements>

<non_functional_requirements>
  NFR-1: The system SHALL deliver email notifications within 60 seconds of the triggering event under normal load. Metric: 95th percentile delivery latency ≤ 60s.
  NFR-2: The system SHALL deliver in-app alerts within 5 seconds of the triggering event. Metric: 95th percentile alert latency ≤ 5s.
  NFR-3: The system SHALL support at least 1,000 simultaneous notification sends without degradation. Metric: No increase in p95 latency at 1,000 concurrent sends.
</non_functional_requirements>

<interface_requirements>
  IR-1: The system SHALL expose an internal API (not public-facing) for the application to trigger notifications by event type and user ID.
  IR-2: The system SHALL integrate with at least one transactional email provider (e.g., SendGrid, SES) for email delivery.
  IR-3: The system SHALL store user notification preferences accessible via the application's existing user settings interface.
</interface_requirements>

<data_requirements>
  DR-1: The system SHALL store notification templates (subject, body, type) separately from delivery logic.
  DR-2: The system SHALL persist a delivery log containing: notification ID, user ID, type, timestamp, status, retry count.
  DR-3: The system SHALL store user preferences as: user ID + notification type + channel (email/in-app) + enabled boolean.
</data_requirements>

<constraints>
  C-1: The system SHALL NOT send notifications to users who have disabled that notification type.
  C-2: The system SHALL NOT expose the notification trigger API externally — internal use only.
  C-3: The system SHALL NOT store notification body content in the delivery log (privacy constraint).
</constraints>

<open_questions>
  OQ-1: What constitutes an "error-class event"? The triggering application must define which events map to in-app alerts vs. email alerts.
  OQ-2: Push notifications are mentioned as a future requirement — should the architecture reserve extensibility for push channels now, or is this strictly out of scope?
  OQ-3: What is the preferred transactional email provider? (FR-2 defers to IR-2 but a concrete choice is needed before implementation.)
  OQ-4: Is there a required retention period for the delivery log (DR-2)? This affects storage planning.
</open_questions>
</specification>
---
Coverage: 6 functional requirements, 3 non-functional, 3 interface, 3 data, 3 constraints | Open questions: 4
```

**What happened:** Domain Analysis identified stakeholders (users, app, developers, ops). Concept Decomposition surfaced 6 functional areas (email, in-app alerts, confirmations, preferences, delivery logging, retry logic), 3 NFR areas, 3 interface touchpoints, and 3 data entities. Requirement Extraction wrote formal SHALL statements with verification criteria. Completeness Audit found all Decomposition items traced to requirements. S7 verification passed — no contradictions, no TBDs without OQs. Four Open Questions surfaced where the input was ambiguous.

---

## Example 12: Plan Mode

**BEFORE (with `--plan` flag):**
```
Set up a PostgreSQL database for a new Node.js API project — need tables for users and posts, proper indexing, and a migration system so future schema changes are tracked
```

**AFTER:**
```
---
<plan>
<goal>
  A PostgreSQL database is fully configured for a Node.js API project, with:
  - users and posts tables created with correct schema and indexes
  - a migration system in place so all future schema changes are tracked and reproducible
  Verification: Running `npm run migrate` on a fresh machine creates the database, tables, and indexes with no errors.
</goal>

<prerequisites>
  Before starting:
  1. PostgreSQL is installed and running (verify: `psql --version` succeeds; `pg_isready` returns "accepting connections")
  2. Node.js and npm are installed (verify: `node --version` and `npm --version` succeed)
  3. A project directory exists with a `package.json` (verify: `ls package.json` succeeds)
  4. Database superuser credentials are available for initial setup
</prerequisites>

<phases>
  <phase id="1" name="Database Setup">
    Step 1.1: Create a dedicated PostgreSQL database for the project.
    Run: `createdb myproject_dev` (replace "myproject_dev" with your project name)
    Verify: `psql -d myproject_dev -c "\l"` lists the new database without error.

    Step 1.2: Create a dedicated database user with limited privileges.
    Run: `psql -c "CREATE USER myproject_user WITH PASSWORD 'your_password';"`
    Run: `psql -c "GRANT ALL PRIVILEGES ON DATABASE myproject_dev TO myproject_user;"`
    Verify: `psql -U myproject_user -d myproject_dev -c "\du"` connects without error.
    Recovery: If connection fails, check pg_hba.conf allows local password auth for new user.

    Checkpoint 1: psql connects as the project user to the project database without error.
  </phase>

  <phase id="2" name="Migration System Installation">
    Step 2.1: Install a Node.js migration library.
    Run: `npm install --save-dev db-migrate db-migrate-pg`
    Verify: `ls node_modules/db-migrate` exists; `npx db-migrate --version` outputs a version number.

    Step 2.2: Create the db-migrate configuration file at project root.
    Create `database.json` with content:
    ```json
    {
      "dev": {
        "driver": "pg",
        "user": "myproject_user",
        "password": "your_password",
        "database": "myproject_dev",
        "host": "127.0.0.1",
        "port": 5432
      }
    }
    ```
    Verify: File exists at `./database.json`; content is valid JSON (`cat database.json | node -e "JSON.parse(require('fs').readFileSync('/dev/stdin','utf8'))" && echo valid`).
    Recovery: If db-migrate cannot connect, verify host/port match `pg_isready` output.

    Step 2.3: Add migration run script to `package.json`.
    In `package.json` scripts section, add: `"migrate": "db-migrate up"`
    Verify: `npm run migrate --dry-run` executes without error (no migrations yet — this confirms configuration is valid).

    Checkpoint 2: `npm run migrate` runs without configuration errors (zero migrations applied is acceptable at this stage).
  </phase>

  <phase id="3" name="Schema Creation via Migrations">
    Step 3.1: Create the users table migration.
    Run: `npx db-migrate create create-users-table --sql-file`
    Verify: A new timestamped directory appears in `./migrations/` containing `up.sql` and `down.sql`.

    Step 3.2: Write the users table UP migration in the generated `up.sql`.
    Content:
    ```sql
    CREATE TABLE users (
      id SERIAL PRIMARY KEY,
      email VARCHAR(255) NOT NULL UNIQUE,
      password_hash VARCHAR(255) NOT NULL,
      created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
      updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
    );
    CREATE INDEX idx_users_email ON users(email);
    ```
    Verify: File saved; `cat migrations/.../up.sql` shows the SQL.

    Step 3.3: Write the users table DOWN migration in the generated `down.sql`.
    Content: `DROP TABLE IF EXISTS users;`
    Verify: File saved.

    Step 3.4: Create the posts table migration.
    Run: `npx db-migrate create create-posts-table --sql-file`
    Verify: A new timestamped directory appears in `./migrations/` after the users migration directory.

    Step 3.5: Write the posts table UP migration.
    Content:
    ```sql
    CREATE TABLE posts (
      id SERIAL PRIMARY KEY,
      user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      title VARCHAR(500) NOT NULL,
      body TEXT NOT NULL,
      created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
      updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW()
    );
    CREATE INDEX idx_posts_user_id ON posts(user_id);
    CREATE INDEX idx_posts_created_at ON posts(created_at DESC);
    ```
    Verify: File saved.

    Step 3.6: Write the posts table DOWN migration.
    Content: `DROP TABLE IF EXISTS posts;`
    Verify: File saved.

    Step 3.7: Run all migrations.
    Run: `npm run migrate`
    Verify:
    - Command exits with code 0
    - `psql -U myproject_user -d myproject_dev -c "\dt"` lists both `users` and `posts` tables
    - `psql -U myproject_user -d myproject_dev -c "\di"` lists `idx_users_email`, `idx_posts_user_id`, `idx_posts_created_at`
    Recovery: If migration fails, run `npx db-migrate down` to roll back, check SQL syntax in the failing migration file, fix, then re-run `npm run migrate`.

    Checkpoint 3: Both tables exist with correct columns and indexes. `npm run migrate` is idempotent (running it again produces no changes).
  </phase>
</phases>

<dependency_notes>
  - Phase 2 must complete before Phase 3: db-migrate must be configured before migrations can run.
  - Step 3.4 must follow Step 3.1: posts table references users(id) — users migration must be ordered first.
  - Step 3.7 must follow Steps 3.1-3.6: all SQL files must be written before running migrations.
</dependency_notes>

<completion_criteria>
  The plan is complete when ALL of the following are true:
  1. `psql -U myproject_user -d myproject_dev -c "\dt"` lists `users` and `posts` with no errors
  2. `psql -U myproject_user -d myproject_dev -c "\di"` lists all three indexes
  3. `npm run migrate` on this machine exits with code 0 and no changes (idempotent)
  4. `./migrations/` directory contains exactly two migration directories, in correct order
  5. Running the full sequence on a fresh PostgreSQL instance (drop and recreate database) produces identical results
</completion_criteria>
</plan>
---
Plan: 10 steps across 3 phases | Verification tests: 10 | Safeguards: 2 | Open questions: 0
```

**What happened:** Goal Analysis identified the precise end state (reproducible schema on any machine). Action Decomposition broke the work into 3 phases: DB setup, migration system, schema. Dependency Mapping found the critical ordering constraint (posts references users — migration order must be enforced). Safeguard Design added Recovery notes to the two steps with destructive potential (new user creation, migration execution). Execution Simulation verified each step's output feeds the next step's input. Gap Audit confirmed all actions from decomposition appear in the plan.

## Example 13: Preservation — Malformed URLs

**BEFORE:**
```
Fetch content from htp://example.com/data (note the typo) and also check the docs at ~/missing-dir/readme.md.
The app uses React 18. (incomplete version).
```

**AFTER:**
```
---
<task>
Fetch content from documentation and check local readme.
</task>

<context>
Sources:
- htp://example.com/data — **Note: URL appears malformed, preserved as-is**
- ~/missing-dir/readme.md — **Note: Path may not exist, preserved as-is**

Technology: React 18. (incomplete version specification)
</context>

<constraints>
- Fetch from htp://example.com/data (preserved exactly as provided)
- Check ~/missing-dir/readme.md (preserved exactly as provided)
- Note: React version is incomplete ("18." without patch)
</constraints>
---
```

**Flagged issues (shown before the prompt):**
> **Note:** URL 'htp://example.com/data' appears malformed (expected 'https://'). Preserved as-is.
> **Note:** Path '~/missing-dir/readme.md' may not exist. Preserved as-is.
> **Note:** React version incomplete: "18." without patch number. Preserved as-is.

**What was preserved:**
- `htp://example.com/data` — malformed URL preserved verbatim
- `~/missing-dir/readme.md` — potentially non-existent path preserved verbatim
- `React 18.` — incomplete version preserved verbatim

**Key behavior:** Malformed and potentially incorrect content is preserved exactly as provided. The skill flags potential issues but never corrects user input.

---

## Example 14: Full Pipeline Chain (Normal → --specification → --plan)

This example shows the complete three-mode chain: raw human text → enhanced prompt → specification → executable plan.

**Original human text (Stage 1 input):**
```
I want to add a simple rate limiter to my Node.js API so users can't spam the
endpoints. Something that limits to 100 requests per 15 minutes per IP address,
using Redis to store the counters. Need it to work with Express and return
proper 429 errors with a Retry-After header.
```

---

**Stage 1: Normal mode (no flag)**

*Announcement:* I'm using the prompt-epiphany skill to analyze and enhance this prompt.

*Sufficiency check:* Sufficient — identifiable implementation task with tech stack (Node.js, Express, Redis), constraints (100 req/15 min per IP), and output requirement (429 + Retry-After).

*This looks like a spec — add `--plan` to turn it into a step-by-step plan, or `--specification` for a formal requirements document, or continue for prompt enhancement.*

**Stage 1 output (enhanced prompt):**
```
---
<role>
You are a senior Node.js backend engineer with expertise in API rate limiting,
Redis-based distributed counters, and Express middleware design.
</role>

<context>
Implementing a rate limiter middleware for an Express-based Node.js API. Redis
is available for counter storage. The rate limit is 100 requests per 15 minutes
per client IP address.
</context>

<task>
Implement a rate limiting middleware for Express that:
1. Tracks request counts per IP address using Redis counters
2. Enforces a limit of 100 requests per 15-minute sliding window
3. Returns HTTP 429 Too Many Requests with a Retry-After header when limit exceeded
4. Integrates transparently with existing Express routes
</task>

<constraints>
- DO use Redis for counter storage (not in-memory — must survive restarts)
- DO track per IP address, not per user or per route
- DO limit to 100 requests per 15-minute window
- DO return HTTP 429 with a Retry-After header when limit is hit
- DO NOT block legitimate requests that are within the limit
- If Redis is unavailable, fail open (allow requests) rather than blocking all traffic
</constraints>

<edge_cases>
- IP behind a proxy: read X-Forwarded-For header if available, fall back to req.ip
- Retry-After value: set to seconds remaining until window resets
- Redis key expiry: set TTL to 15 minutes (900 seconds) to auto-clean counters
</edge_cases>

<output_format>
Provide:
1. The Express middleware function (rate-limiter.js)
2. Usage example showing how to mount it in app.js
3. Redis key schema used (e.g., ratelimit:{ip}:{window})
</output_format>

<verification>
Verify: middleware correctly increments Redis counter per IP, returns 429 with
Retry-After on limit exceeded, fails open if Redis is down, handles X-Forwarded-For.
</verification>
---
```
Preserved: 3 named entities, 2 numeric specifications, 1 tech+version pair

---

**Stage 2: Specification mode (feed Stage 1 output to `--specification`)**

*Announcement:* I'm using the prompt-epiphany skill (specification mode) to develop a complete specification.

*Sufficiency check:* Sufficient — clear system concept with technology constraints, performance requirements, and interface requirements.

**Stage 2 output (specification):**
```
---
<specification>
<scope>
  Included: Rate limiting middleware for an Express-based Node.js API using Redis.
            Covers counter storage, limit enforcement, HTTP error responses, and
            proxy-awareness.
  Excluded: Authentication or user-level rate limiting, route-specific limits,
            distributed multi-region Redis setups, admin dashboards for limit management.
</scope>

<context>
  Domain: Node.js backend API middleware
  Stakeholders:
    - API consumers: must receive clear 429 responses with Retry-After guidance
    - Application developers: must be able to mount middleware with minimal configuration
    - Operations: Redis must be the backing store; middleware must degrade gracefully
  Background: Rate limiter uses Redis counters with TTL-based window expiry.
              Per-IP tracking uses X-Forwarded-For with fallback to req.ip.
</context>

<functional_requirements>
  FR-1: The middleware SHALL increment a Redis counter for each request, keyed by
        client IP address and the current 15-minute window.
        Verification: Send 5 requests from IP 192.0.2.1; confirm Redis key
        ratelimit:192.0.2.1:{window} equals 5 after 5 requests.

  FR-2: The middleware SHALL allow requests when the counter is below 100 for
        the current 15-minute window.
        Verification: Send 99 requests; confirm all receive 2xx responses.

  FR-3: The middleware SHALL return HTTP 429 Too Many Requests when the counter
        reaches or exceeds 100 for the current 15-minute window.
        Verification: Send 101st request; confirm HTTP 429 response.

  FR-4: The middleware SHALL include a Retry-After header in every 429 response,
        set to the integer seconds remaining until the current window resets.
        Verification: On 429 response, confirm Retry-After header is present,
        numeric, and ≤ 900.

  FR-5: The middleware SHALL read the client IP from X-Forwarded-For header when
        present, falling back to req.ip when header is absent.
        Verification: Send request with X-Forwarded-For: 10.0.0.1; confirm Redis
        key uses 10.0.0.1, not the proxy IP.

  FR-6: The middleware SHALL set Redis key TTL to 900 seconds (15 minutes) on
        first write, to auto-expire counters.
        Verification: Confirm TTL on new Redis key is 900 seconds.
</functional_requirements>

<non_functional_requirements>
  NFR-1: The middleware SHALL add no more than 5ms latency to requests under
         normal Redis operation. Metric: p99 middleware overhead ≤ 5ms.
  NFR-2: The middleware SHALL fail open (allow all requests) when Redis is
         unavailable. Metric: 0 requests blocked due to Redis downtime.
</non_functional_requirements>

<interface_requirements>
  IR-1: The middleware SHALL integrate as a standard Express middleware function
        mountable via app.use() or router.use().
  IR-2: The middleware SHALL connect to Redis via a configurable connection
        (host, port, or connection string) passed at initialization.
</interface_requirements>

<data_requirements>
  DR-1: Redis key schema: ratelimit:{ip}:{window_bucket} where window_bucket is
        floor(Date.now() / 900000).
  DR-2: Redis value: integer counter. TTL: 900 seconds from first write.
</data_requirements>

<constraints>
  C-1: The middleware SHALL NOT use in-memory storage — all counters must be in Redis.
  C-2: The middleware SHALL NOT block requests when Redis is unavailable (fail open).
  C-3: The middleware SHALL NOT modify the request or response for allowed requests
       beyond incrementing the counter.
</constraints>

<open_questions>
  OQ-1: Should the first request in a window set the TTL, or should the window
        be aligned to clock boundaries (e.g., :00, :15, :30, :45)? Clock-aligned
        windows require different key schema. BLOCKS PLAN PHASE: Redis key design.
  OQ-2: Should the Retry-After value be the seconds to the window reset (sliding)
        or a fixed 900? Sliding window requires storing the window start timestamp.
</open_questions>
</specification>
---
Coverage: 6 functional requirements, 2 non-functional, 2 interface, 2 data, 3 constraints | Open questions: 2
```

---

**Stage 3: Plan mode (feed Stage 2 output to `--plan`)**

*Note: OQ-1 and OQ-2 are open. For this example, assume: fixed window aligned to floor(Date.now() / 900000) bucket, Retry-After = seconds to next bucket boundary.*

*Announcement:* I'm using the prompt-epiphany skill (plan mode) to develop a step-by-step plan.

*Sufficiency check:* Sufficient — formal specification with 6 FRs, 2 NFRs, concrete Redis schema, and resolved OQs.

**Stage 3 output (plan):**
```
---
<plan>
<goal>
  A rate-limiting Express middleware is implemented, tested, and mounted in the
  application. It enforces 100 requests per 15-minute window per IP using Redis
  (key schema: ratelimit:{ip}:{floor(Date.now()/900000)}), returns HTTP 429 with
  Retry-After on limit exceeded, and fails open when Redis is unavailable.
  Verification: Running the integration test suite passes all 6 FR checks.
</goal>

<prerequisites>
  Before starting:
  1. Node.js and npm are installed (verify: node --version, npm --version succeed)
  2. An Express application exists with package.json (verify: ls package.json succeeds)
  3. Redis is installed and running locally (verify: redis-cli ping returns PONG)
  4. ioredis or redis npm package is available or can be installed
</prerequisites>

<phases>
  <phase id="1" name="Install Redis Client Dependency">
    Step 1.1: Install the ioredis package.
    Run: npm install ioredis
    Verify: ls node_modules/ioredis succeeds; package.json dependencies includes "ioredis".
    Recovery: If npm install fails, check network access and npm registry; try npm install --legacy-peer-deps.

    Checkpoint 1: ioredis is listed in package.json dependencies and node_modules/ioredis exists.
  </phase>

  <phase id="2" name="Implement Rate Limiter Middleware">
    Step 2.1: Create file src/middleware/rate-limiter.js.
    Verify: File exists at src/middleware/rate-limiter.js.

    Step 2.2: Write the rate limiter middleware with this exact implementation:
    - Accept a Redis client instance as a constructor parameter
    - Compute window bucket: Math.floor(Date.now() / 900000)
    - Compute Redis key: `ratelimit:${clientIp}:${bucket}`
    - Use Redis INCR to increment counter; use EXPIRE 900 if counter === 1 (first request in window)
    - Read clientIp from req.headers['x-forwarded-for']?.split(',')[0].trim() || req.ip
    - If counter > 100: compute Retry-After = Math.ceil(((bucket + 1) * 900000 - Date.now()) / 1000); return res.status(429).set('Retry-After', retryAfter).json({ error: 'Too Many Requests' })
    - If Redis throws: log error, call next() (fail open)
    Verify: File contains INCR, EXPIRE 900, 429 status, Retry-After header, and try/catch with next() in catch.

    Checkpoint 2: src/middleware/rate-limiter.js exists and contains all required logic elements.
  </phase>

  <phase id="3" name="Write Integration Tests">
    Step 3.1: Create file tests/rate-limiter.test.js.
    Verify: File exists at tests/rate-limiter.test.js.

    Step 3.2: Write tests covering:
    - FR-2: 99 requests all receive 2xx
    - FR-3: 101st request receives 429
    - FR-4: 429 response contains Retry-After header with numeric value ≤ 900
    - FR-5: X-Forwarded-For IP is used when present
    - FR-6: Redis key TTL is 900 after first request
    - NFR-2: Middleware calls next() when Redis client throws (fail open)
    Verify: Test file contains at minimum 6 describe/it blocks matching above.

    Step 3.3: Run the test suite.
    Run: npx jest tests/rate-limiter.test.js --verbose
    Verify: All tests pass (0 failures); output shows 6+ passing tests.
    Recovery: If a test fails, read the failure message, identify which FR is violated, return to Step 2.2 and fix the specific logic, then re-run Step 3.3.

    Checkpoint 3: All tests pass with 0 failures.
  </phase>

  <phase id="4" name="Mount Middleware in Application">
    Step 4.1: In app.js (or the main Express entry point), import the middleware and a Redis client.
    Add: const Redis = require('ioredis'); const createRateLimiter = require('./src/middleware/rate-limiter');
    Verify: Import lines present in app.js.

    Step 4.2: Instantiate Redis client and mount middleware before route handlers.
    Add: const redis = new Redis(); app.use(createRateLimiter(redis));
    Verify: app.use(createRateLimiter(redis)) appears before any app.get/app.post/router definitions.

    Step 4.3: Start the application and send a test request.
    Run: node app.js (or npm start) in background; curl -i http://localhost:3000/ (adjust port as needed)
    Verify: Response is 2xx and does not contain Retry-After header (under the limit).
    Recovery: If application fails to start, check Redis connection settings match running Redis instance.

    Checkpoint 4: Application starts without error; first request returns 2xx with no Retry-After.
  </phase>
</phases>

<dependency_notes>
  - Phase 1 must complete before Phase 2: ioredis must be installed before require('ioredis') works.
  - Phase 2 must complete before Phase 3: middleware must exist before tests can import it.
  - Phase 3 must complete before Phase 4: tests must pass before mounting in production app.
</dependency_notes>

<completion_criteria>
  The plan is complete when ALL of the following are true:
  1. src/middleware/rate-limiter.js exists and implements Redis INCR, EXPIRE 900, 429 + Retry-After, and fail-open behavior
  2. npx jest tests/rate-limiter.test.js runs with 0 failures
  3. app.js mounts the middleware before route handlers
  4. A live curl request to the running application returns 2xx for request #1 and 429 for request #101
  5. Redis key ratelimit:{ip}:{bucket} exists with TTL ≤ 900 after first request
</completion_criteria>
</plan>
---
Plan: 9 steps across 4 phases | Verification tests: 9 | Safeguards: 2 | Open questions: 0
```

**What the pipeline demonstrated:**
- Stage 1 (Normal): Raw description → structured prompt preserving all technical details (Express, Redis, 100 req/15 min, 429, Retry-After, X-Forwarded-For)
- Stage 2 (--specification): Enhanced prompt → 6 FRs, 2 NFRs with metrics, Redis key schema, 2 open questions surfaced
- Stage 3 (--plan): Specification → 9 atomic steps across 4 phases, every step with Verify, Recovery on destructive steps, completion criteria observable end-to-end
- Information loss across all three stages: none — every technical detail from the original human text appears in the final plan