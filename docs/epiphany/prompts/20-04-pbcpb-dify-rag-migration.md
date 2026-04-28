<?xml version="1.0" encoding="UTF-8"?>
<!-- Skipped contracts: none -->
<prompt>
  <meta source="prompt-cog"/>

  <role>
    You are a senior agent-system architect specializing in RAG pipeline integration and knowledge management system migration. You have deep expertise in Dify RAG architecture, MCP tool integration, and JSON playbook system design.
  </role>

  <context>
    <!-- Concept anchor glossary -->
    - **pbcpb**: Playbook-Centered Prompt Builder — the existing system located at /home/myuser/Documents/pbcpb.
    - **pbcpb-dify**: the new Dify-RAG-aware fork to be created at /home/myuser/Documents/pbcpb-dify.
    - **DIFY**: the RAG platform managing the knowledge base, replacing all legacy KB infrastructure.
    - **MCP**: Model Context Protocol tools enabling Claude Code to query DIFY at runtime.
    - **RAG**: Retrieval-Augmented Generation — querying the KB at runtime rather than storing it inline.

    <!-- Consumer calibration -->
    This prompt is executed by Claude Code, which has read/write filesystem access, shell command execution, and access to the Dify MCP tools currently listed in the active MCP configuration. The agent should enumerate available MCP tools at the start of execution to confirm Dify KB tools are present.

    The pbcpb system produces JSON playbooks that guide users through a process including bootstrapping a knowledge base, generating a master index, and performing crossreferencing. This KB infrastructure was designed for a legacy local KB approach and is now outdated. The replacement is a more advanced retrieval augmented generation system — RAG — set up in DIFY and connected to Claude Code via MCP. The agent executing this prompt will assess the existing system at /home/myuser/Documents/pbcpb, produce a complete fork at /home/myuser/Documents/pbcpb-dify, and migrate that fork to use DIFY's MCP-based RAG instead of the legacy KB stack.

    Knowledge will continue to be gathered, but in markdown format rather than JSON. This markdown is uploaded to DIFY manually and processed into the knowledge base there. pbcpb-dify playbooks should scaffold this markdown gathering process but must not implement JSON harvesting, master indexing, or automated crossreferencing.
  </context>

  <task>
    Execute the following phases in strict sequential order. Do NOT begin a later phase until the preceding phase is complete and its outputs are confirmed.

    ---

    **PRE-FLIGHT — Existence Check (before all phases)**

    Before any other action, check whether /home/myuser/Documents/pbcpb-dify already exists. If it does, halt immediately and prompt the user to confirm whether to overwrite before proceeding. Do not continue until the user responds.

    ---

    **PHASE 0 — MCP Discovery**

    Enumerate all available MCP tools in the current Claude Code configuration. Extract all Dify-related tool names. Output the discovered tool list. Store these exact tool names as the authoritative reference for pbcpb-dify playbook KB-retrieval annotations — never use generic placeholder names like "dify_tool" when actual names are available.

    ---

    **PHASE 1 — Safe Copy and Verify**

    1. Record a checksum baseline of /home/myuser/Documents/pbcpb before any operations begin:
       `find /home/myuser/Documents/pbcpb -type f | sort | xargs md5sum > /tmp/pbcpb_pre_baseline.md5`
       Store this as the verification baseline for the post-task integrity check.
    2. Copy /home/myuser/Documents/pbcpb to /home/myuser/Documents/pbcpb-dify using filesystem operations that do not modify the source.
    3. Verify /home/myuser/Documents/pbcpb-dify exists and contains the expected directory structure before proceeding.

    ---

    **PHASE 2 — System Analysis**

    Perform a full analysis of pbcpb-dify (never pbcpb). Produce a Component Analysis Table with the following columns:

    | Component Name | Current KB Behavior | Removal Required? | Dify Replacement | MCP Guidance Needed? |

    Pay particular attention to:
    - The JSON playbook generation pipeline and its KB-referencing phases.
    - All harvesting, master index generation, and crossreferencing logic.
    - Any file path references that embed /home/myuser/Documents/pbcpb (these must be updated to /home/myuser/Documents/pbcpb-dify in the fork).

    **Change Manifest (mandatory gate before Phase 3):**
    After completing analysis, produce a Change Manifest listing:
    - Every file in pbcpb-dify that will be modified, created, or deleted.
    - Every playbook phase that will be altered and the nature of each alteration.

    The agent MUST NOT proceed to Phase 3 until this manifest is complete and output to the user.

    If the full scope of pbcpb-dify modifications revealed in Phase 2 exceeds a single session, produce a phased migration plan document with clear phase boundaries and halt at each boundary for user confirmation before proceeding.

    ---

    **PHASE 3 — Architectural Redesign**

    Apply all changes identified in the Phase 2 Component Analysis Table to pbcpb-dify only.

    **Removals:**
    Remove all legacy JSON-based KB infrastructure from pbcpb-dify: harvesting pipeline, JSON storage, master index generation, automated crossreferencing, and KB bootstrapping phases.

    **Replacements:**
    - Replace removed KB phases with markdown-first knowledge gathering guidance: pbcpb-dify playbooks should instruct users to collect domain knowledge in markdown files for manual upload to DIFY.
    - Where pbcpb-dify playbooks require domain knowledge retrieval at runtime, annotate those phases to use the Dify MCP tools discovered in Phase 0, using their exact tool names.
    - Generate a bootstrap KB structure — a markdown scaffold of recommended knowledge base sections (KB-STRUCTURE.md) — that the user can populate and upload to DIFY manually.

    **Research directive:**
    Where architectural decisions require research (e.g., Dify KB schema best practices, MCP integration patterns), perform targeted research scoped to the specific decision point. Research must terminate once a well-grounded decision is reached.

    ---

    **PHASE 4 — Validation**

    Execute the self-verification checklist in `<verification>` before declaring completion. Output all checklist results explicitly.
  </task>

  <constraints>
    <!-- Preservation constraints -->
    - DO preserve all files in /home/myuser/Documents/pbcpb exactly as-is at all times.
    - DO NOT touch any files in /home/myuser/Documents/pbcpb — all removals and modifications apply to pbcpb-dify only.
    - DO use POSIX paths (/home/myuser/Documents/pbcpb and /home/myuser/Documents/pbcpb-dify) in all shell operations; the URIs file:///home/myuser/Documents/pbcpb and file:///home/myuser/Documents/pbcpb-dify are provided for reference only and must not be used as shell arguments.
    - DO check the current MCP tool list before referencing Dify tool names in any playbook output.

    <!-- Explicit DO/DO NOT list -->
    DO:
    - store knowledge in markdown format in pbcpb-dify outputs.
    - indicate MCP KB content retrieval where necessary using exact Dify MCP tool names from Phase 0 discovery.
    - scaffold manual upload to Dify as the designated knowledge ingestion path in playbook guidance.
    - perform targeted research where architectural decisions require it, within the scope defined in Phase 3.

    DO NOT:
    - add explicit MCP KB retrieval invocation steps in playbook phases where the agent already has a deterministic natural trigger for retrieval. When in doubt, annotate with a comment rather than adding a hard invocation step.

    <!-- MCP guidance resolution rule -->
    Add explicit MCP KB retrieval invocation guidance in pbcpb-dify playbook phases ONLY when: (a) a phase requires domain knowledge lookup AND (b) the agent has no other deterministic signal to trigger retrieval naturally. In all other cases, rely on natural MCP tool behavior. When in doubt, annotate with a comment rather than adding a hard invocation step. This rule resolves the tension between "indicate MCP KB content retrieval where necessary" and "do not get in the way if naturally working."
  </constraints>

  <output_format>
    Produce the following deliverables:

    1. **Phase 0 MCP tool list** — output the discovered Dify MCP tool names at the start of execution, before any filesystem operations.
    2. **Modified pbcpb-dify directory** at /home/myuser/Documents/pbcpb-dify — all playbook templates and system files updated per Phase 3, with legacy KB infrastructure removed and Dify RAG via MCP integrated.
    3. **MIGRATION-NOTES.md** — placed in /home/myuser/Documents/pbcpb-dify, documenting every removed component, its Dify replacement, and the rationale for the change. Include a section flagging all ambiguous MCP trigger points for user review.
    4. **KB-STRUCTURE.md** — a markdown scaffold of the recommended knowledge base structure, placed in /home/myuser/Documents/pbcpb-dify, intended for manual population and upload to DIFY. Section headers should reflect the domain knowledge structure that pbcpb-dify playbooks will reference via MCP at runtime.

    All four deliverables must be present and verified before the task is considered complete.
  </output_format>

  <verification>
    After completing all phases, verify each of the following and output the result (PASS/FAIL) for each item:

    1. Re-run the integrity check: `find /home/myuser/Documents/pbcpb -type f | sort | xargs md5sum > /tmp/pbcpb_post_baseline.md5 && diff /tmp/pbcpb_pre_baseline.md5 /tmp/pbcpb_post_baseline.md5` — zero differences confirms copy without hurting the old system.
    2. pbcpb-dify playbook templates reference Dify MCP retrieval using exact tool names discovered in Phase 0, not legacy KB paths or generic placeholders.
    3. No JSON harvesting code remains in pbcpb-dify — remove harvesting and json storage feature is confirmed.
    4. MIGRATION-NOTES.md exists in pbcpb-dify and covers all removed components with replacements and rationale, including flagged ambiguous MCP trigger points.
    5. KB-STRUCTURE.md exists in pbcpb-dify with populated section headers ready for manual Dify upload — store knowledge in markdown format is confirmed.
    6. The Change Manifest produced at the end of Phase 2 matches the actual set of files modified in Phase 3 (no unplanned changes).
  </verification>

  <edge_cases>
    - **pbcpb-dify already exists:** Check performed during Pre-Flight before Phase 0. If /home/myuser/Documents/pbcpb-dify already exists, halt immediately and prompt the user to confirm whether to overwrite before proceeding.
    - **Dify MCP tools not found:** If no Dify-related MCP tools are found during Phase 0 enumeration, halt immediately and output: `MCP_NOT_FOUND — verify Dify MCP setup before proceeding.` Do not continue to Phase 1.
    - **KB not yet populated in Dify:** If Dify MCP tools are present but the knowledge base has not yet been populated, continue execution but annotate all pbcpb-dify playbook phases that require KB content with `[KB-PENDING]` tags. Add a pre-execution note in the pbcpb-dify playbook README explaining that KB content must be uploaded to DIFY before running playbooks that reference those phases.
    - **Scope exceeds single session:** If the Component Analysis Table in Phase 2 reveals a migration scope that cannot be completed in a single session, produce a phased migration plan document with clear phase boundaries and halt at each boundary for explicit user confirmation before proceeding to the next boundary.
    - **Ambiguous MCP trigger points:** Where it is hard to tell whether the Claude Code agent will naturally trigger MCP retrieval, apply the resolution rule in `<constraints>`: annotate with a comment rather than adding a hard invocation step. Flag these annotation points explicitly in MIGRATION-NOTES.md so the user can review them after running an initial playbook.
  </edge_cases>
</prompt>
