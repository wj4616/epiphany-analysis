```xml
<role>
Skill designer extending prompt-epiphany with two new first-class modes: --specification
and --plan. You have full knowledge of the existing prompt-epiphany pipeline (Gather →
Sufficiency Check → Analysis → Ideation → Synthesis → Verification → Output), the
--minimal and --verbose extensions, and the 6a–6l verification system.
</role>

<context>
prompt-epiphany currently operates in four modes: normal, --minimal, --verbose, and
--quiet. Each mode follows a defined pipeline and integrates cleanly with the base skill.

This extension introduces two additional modes with distinct output types:
- --specification: produces a complete, detailed specification document describing a
  solution, product, or concept — every element of the input must be represented,
  verified, and nothing missed
- --plan: produces a maximally granular, action-by-action execution plan written from a
  specification, with safeguards ensuring no action is omitted and that following the
  plan exactly will produce the target outcome

Both modes must integrate cleanly with the existing pipeline, announcement system, file
naming convention, and the existing 6a–6l verification system (which is reimplemented
and extended per mode as described below).
</context>

<task>
Design the complete behavioral specification for --specification and --plan modes.
Deliver a self-contained implementation spec covering all sections below, in sufficient
detail that the modes can be added to SKILL.md without modifying any existing pipeline
steps or breaking backward compatibility.

Deliver in order:
1. Mode Detection — flag-based and context-inference-based activation
2. Trigger Conditions additions — new rows for the existing Trigger Conditions table
3. Pipeline step definitions — concrete numbered steps for each mode
4. Output format — XML templates for each mode's output
5. Verification system — 6a–6l reapplied per mode, plus mode-specific additional checks
6. Integration rules — combinations with --quiet, --minimal, --verbose, and each other
7. SKILL.md sync checklist — every location in the existing skill that requires updating
8. Edge cases — new rows for the existing Edge Cases table
</task>

<task_decomposition>

## 1. Mode Detection

### Flag Detection (primary — same rule as --minimal/--verbose)
Detect `--specification` and `--plan` at the first or last standalone token of the input.
Strip detected flags before processing. Flags mid-sentence within the prompt body are
content, not mode selectors.

### Context Inference (secondary — activates without a flag)
Before requiring a flag, analyze whether the input clearly signals one of these patterns:

| Signal in Input | Infer Mode |
|-----------------|------------|
| Describes a problem needing a solution built out in full detail | --specification |
| Describes a product or system where all components need to be enumerated | --specification |
| Describes a concept that needs breaking down into its particular elements | --specification |
| Has a completed specification and asks "how do I build/execute this" | --plan |
| Asks for step-by-step execution methodology for a defined goal | --plan |
| Describes a task requiring granular action sequencing | --plan |

**Confidence threshold for auto-activation:** The signal must be unambiguous — if the
input could reasonably be a standard prompt enhancement, default to normal mode.
When auto-activating, announce the inferred mode and give the user a one-line
opportunity to correct: "Detected --specification mode from context. Proceeding —
reply 'stop' to use standard enhancement instead."

### Flag Combination Rules
- `--specification` + `--plan` together: run specification pipeline first, then feed
  output directly into plan pipeline (sequential — do not re-prompt between stages)
- `--specification` or `--plan` + `--quiet`: quiet behavior applies (save-only,
  skip terminal display)
- `--specification` or `--plan` + `--minimal` or `--verbose`: these modes are
  incompatible — ask user which mode to use before proceeding
- `--specification` + `--plan` + `--quiet`: full sequential pipeline, save both
  outputs as separate files, no terminal display

---

## 2. Trigger Conditions Table Additions

Add these rows to the existing Trigger Conditions table:

| Trigger | Behavior |
|---------|----------|
| `--specification` | Activate specification mode. Run specification pipeline. |
| `--plan` | Activate plan mode. Run plan pipeline. |
| `--specification --plan` | Activate sequential mode: specification first, plan second. |
| Context clearly signals specification need | Auto-activate specification mode with confirmation prompt. |
| Context clearly signals plan need | Auto-activate plan mode with confirmation prompt. |
| `--specification` or `--plan` + `--minimal` or `--verbose` | Ask user to pick one mode. |

---

## 3. Pipeline Step Definitions

### --specification Pipeline

**Route:** Gather → Sufficiency Check → Step S1 → Step S2 → Step S3 → Step S4 → Output

#### Step S1: Specification Analysis (internal — not shown to user)

Extends the normal Step 3 analysis with a specification-focused lens. Run all six
standard dimensions, then add:

**S1-g. Element Inventory (specification-specific)**
Exhaustively enumerate every concept, component, feature, behavior, requirement, and
constraint present in the input — explicitly stated or strongly inferable.

```
ELEMENT INVENTORY:
  Concepts:
    - [every distinct concept mentioned]
  Components / Features:
    - [every component, feature, or capability described]
  Behaviors:
    - [every behavior, action, or outcome described]
  Requirements (functional):
    - [what the solution must do]
  Requirements (non-functional):
    - [performance, quality, constraint requirements]
  Acceptance Criteria (explicit):
    - [stated conditions for success]
  Acceptance Criteria (inferable):
    - [conditions strongly implied but not stated]
  Open Questions:
    - [elements present in input that are ambiguous or underspecified]
```

This inventory is the authoritative completeness checklist for Step S4 verification.

#### Step S2: Specification Ideation (internal — not shown to user)

Completeness-focused ideation pass:

1. For every item in the Element Inventory: determine where in the output specification
   it belongs and how it should be expressed
2. For every open question: determine whether it can be resolved from context, or must
   be surfaced to the user in `<open_questions>`
3. For every inferable acceptance criterion: assess whether it is strongly enough
   supported to include without flagging, or should be flagged as inferred
4. Check: are there element types implied by what IS specified that the input does not
   mention? (e.g., if a UI is specified, are data persistence requirements implied?)
   Flag any strongly-implied missing elements for `<open_questions>`

**Every item added must trace to either the Element Inventory or a flagged inference.**

#### Step S3: Specification Synthesis (internal — not shown to user)

Produce the specification document using the following XML structure:

```xml
<specification>
  <overview>
    [One paragraph: what is being specified, for whom, and to what end]
  </overview>
  <elements>
    [Exhaustive enumeration of all components, features, and behaviors — one item per
    line, grouped by type. Nothing from the Element Inventory may be absent.]
  </elements>
  <requirements>
    [Functional requirements: what the solution must do]
    [Non-functional requirements: quality, performance, constraints]
    [Each requirement is atomic and independently verifiable]
  </requirements>
  <constraints>
    [Hard constraints: what the solution must not do, or limits it must respect]
    [Derived from input — no invented constraints]
  </constraints>
  <acceptance_criteria>
    [Testable conditions for completion — each criterion must be verifiable by
    observation or test, not by subjective judgment]
    [Inferred criteria are marked: [inferred]]
  </acceptance_criteria>
  <open_questions>
    [Elements from the input that are ambiguous or underspecified and require user
    input before a plan can proceed]
    [Strongly-implied missing elements that the user should confirm or deny]
    [If none: omit this section]
  </open_questions>
</specification>
```

**Completeness rule:** Every item in the Step S1 Element Inventory must appear in
the output. No element may be dropped. No element may be summarized to the point
where the original detail is lost.

#### Step S4: Specification Verification

Run all twelve standard checks (6a–6l) applied to the specification output, then
run these specification-specific additional checks:

**6m-spec. Element Completeness Sweep** — Cross-reference every item in the Step S1
Element Inventory against the specification output. Every item must appear.
Missing → FAIL. Recovery: add missing item to the appropriate section.

**6n-spec. Acceptance Criteria Testability** — Every item in `<acceptance_criteria>`
must be verifiable by observation or test without subjective judgment. Vague criterion
(e.g., "works well") → FAIL. Recovery: rewrite as a specific testable condition or
move to `<open_questions>`.

**6o-spec. Constraint Sourcing** — Every item in `<constraints>` must trace to
the input or a strongly-inferable implication. Invented constraints → FAIL.
Recovery: remove or move to `<open_questions>` with note.

**6p-spec. Open Questions Completeness** — Every element identified as ambiguous or
unresolvable in Step S2 ideation must appear in `<open_questions>`. Missing → FAIL.
Recovery: add the unresolved item.

Loop: all pass → output. Any fail → fix in Step S3, re-verify. Same check fails
twice → output with note: "Verification check [name] could not be fully resolved —
review flagged area."

---

### --plan Pipeline

**Route:** Gather → Sufficiency Check → Step P1 → Step P2 → Step P3 → Step P4
          (per phase) → Step P5 → Step P6 → Output

**Input requirement:** Plan mode requires specification-quality input — either the
output of --specification mode, or a user-provided specification that enumerates
elements, requirements, constraints, and acceptance criteria. If the input does not
meet this threshold, warn the user and offer to run --specification first before
proceeding.

#### Step P1: Plan Context Analysis (internal — not shown to user)

Extract from the input (or preceding specification output):

```
PLAN CONTEXT:
  Goal:
    - [The final state that executing this plan must produce]
  Specification elements to address:
    - [Every element from the specification that requires at least one action]
  Hard constraints (cannot be violated):
    - [Constraints from specification that limit what actions are permissible]
  Dependencies (external):
    - [Things that must exist or be true before the plan can begin]
  Acceptance criteria:
    - [The testable conditions the completed plan must satisfy]
```

This context is the completeness checklist for Steps P4 and P6.

#### Step P2: Phase Decomposition (internal — not shown to user)

Break the goal into major phases — logical groupings of actions that share a common
objective or produce a checkpoint outcome. For each phase:

- Name the phase
- State its prerequisite (what must be complete before this phase starts)
- State its deliverable (the verifiable output that marks this phase complete)
- Map which specification elements this phase addresses

Phases must be ordered such that each phase's prerequisites are satisfied by prior
phases or by the plan's starting conditions.

#### Step P3: Action-Level Decomposition (internal — not shown to user)

For each phase: enumerate every atomic action required to complete that phase.

**Granularity standard:** An action is atomic if it can be executed by a single agent
in a single step without further decomposition. If an action requires making a
sub-decision, it must be split. If an action requires information not yet available
at the time of execution, that information must be produced by a prior action.

For each action, specify:
- The exact action to take (verb + object + precision — not "set up the environment"
  but "install dependency X at version Y in directory Z")
- Precondition: what must be true immediately before this action
- Expected outcome: the verifiable state after this action completes
- Dependency: which prior actions must be complete before this action

**No action may be implied.** Every action that must occur must be explicitly stated.

#### Step P4: Phase Completeness Review (internal — after each phase)

After writing each phase, before proceeding to the next:

1. Check: does every specification element assigned to this phase appear in at least
   one action in this phase?
2. Check: does the phase deliverable follow verifiably from completing all actions in
   the phase?
3. Check: are there any transitions between actions where an undefined state exists?

If any check fails: add missing actions or correct the phase before proceeding.

#### Step P5: Plan Synthesis

Assemble the complete plan using this structure:

```
## Plan: [Goal name]
Prerequisites: [What must be true before Step 1]

---

### Phase 1: [Phase name]
Prerequisites: [What must be complete before this phase]
Deliverable: [Verifiable output marking phase complete]
Addresses: [Specification elements covered by this phase]

Step 1.1: [Action name]
- Action: [Exact action]
- Precondition: [What must be true]
- Expected outcome: [Verifiable result]
- Depends on: [Prior steps, if any]

Step 1.2: ...

Phase 1 completeness check: [Confirmation that all assigned spec elements are addressed]

---

### Phase 2: [Phase name]
...

---

## Final Audit
[See Step P6]
```

**Completeness rule:** Every item in the Step P1 Plan Context must be addressed by
at least one action. No element may remain unaddressed.

#### Step P6: Plan Audit

Final audit — execute a dry run of the plan in reasoning:

**6a-plan. Goal Coverage** — Trace from the final action back to the goal. Does
executing all steps in order deterministically produce the goal state?
If the gap exists anywhere → FAIL. Recovery: add missing steps.

**6b-plan. Specification Element Coverage** — Every element from the Step P1
context appears in at least one action. Missing element → FAIL.
Recovery: add actions for the missing element.

**6c-plan. Action Atomicity** — No action requires a sub-decision at execution
time. Any action that does → FAIL. Recovery: split into atomic sub-actions.

**6d-plan. Information Availability** — No action requires information that is not
available at the time it executes (i.e., produced by a prior step or provided as a
prerequisite). Dependency violation → FAIL. Recovery: add a prior step that produces
the required information, or add to prerequisites.

**6e-plan. Phase Ordering** — Each phase's prerequisites are fully satisfied by
prior phases or by the plan's starting conditions. Circular dependency or unsatisfied
prerequisite → FAIL. Recovery: reorder phases or add a prerequisite action.

**6f-plan. Outcome Verifiability** — Every step's expected outcome is verifiable
by observation or test. Vague outcome → FAIL. Recovery: rewrite as specific verifiable
state.

**6g-plan. No Gaps Between Actions** — Between each pair of adjacent actions,
the system state is fully defined — no undefined transitions. Gap → FAIL.
Recovery: add a bridging action or extend the expected outcome of the prior step.

**6h-plan. Acceptance Criteria Satisfaction** — Every acceptance criterion from the
specification is satisfied by the final state produced by the plan. Unsatisfied
criterion → FAIL. Recovery: add actions that produce the required state.

Loop: all pass → output. Any fail → fix in Step P5, re-verify. Same check fails
twice → output with note.

---

## 4. Output Formats (summary)

**--specification:** Wrapped in `---` delimiters, XML `<specification>` structure
with sections: `<overview>`, `<elements>`, `<requirements>`, `<constraints>`,
`<acceptance_criteria>`, `<open_questions>` (if needed).

**--plan:** Wrapped in `---` delimiters, markdown structure with phases, numbered
steps (N.M format), per-step precondition/action/expected-outcome/dependency, per-phase
completeness check, and a final audit section.

**--specification + --plan (sequential):** Deliver specification first (wrapped in
`---`), then immediately proceed to plan mode using the specification output as input,
deliver plan second (wrapped in `---`).

---

## 5. Announcements (additions to Output Flow)

Add to the announcement table:

| Mode | Announcement |
|------|-------------|
| `--specification` | "I'm using the prompt-epiphany skill (specification mode) to produce a complete specification from this input." |
| `--plan` | "I'm using the prompt-epiphany skill (plan mode) to produce a granular execution plan from this specification." |
| `--specification --plan` | "I'm using the prompt-epiphany skill (specification → plan mode) to produce a specification, then derive a complete execution plan from it." |
| Auto-inferred specification | "Detected --specification mode from context. Proceeding — reply 'stop' to use standard enhancement instead." |
| Auto-inferred plan | "Detected plan mode from context. Proceeding — reply 'stop' to use standard enhancement instead." |

---

## 6. File Naming Additions

Add to the File Save Naming section:

| Mode | Naming |
|------|--------|
| `--specification` | `DD-MM-name-spec.md` |
| `--plan` | `DD-MM-name-plan.md` |
| `--specification --plan` | Two files: `DD-MM-name-spec.md` and `DD-MM-name-plan.md` |
| `--quiet --specification` | `DD-MM-name-spec.md` (saved immediately, no display) |
| `--quiet --plan` | `DD-MM-name-plan.md` (saved immediately, no display) |

---

## 7. SKILL.md Sync Checklist

Every location in the existing SKILL.md that must be updated when adding these modes:

- [ ] **Pipeline summary line** at top of Pipeline section: add `--specification` and
      `--plan` routes
- [ ] **Step 1: Gather + Mode Detection**: add `--specification` and `--plan` to flag
      detection list; add context inference rules; add combination rules
- [ ] **Trigger Conditions table**: add new rows (see Section 2 above)
- [ ] **Announcements section**: add new announcement strings (see Section 5 above)
- [ ] **File Save Naming section**: add new rows (see Section 6 above)
- [ ] **Mode-Specific Output Differences table**: add rows for spec and plan modes
- [ ] **Edge Cases table**: add new rows (see Section 8 below)
- [ ] **After Step 10v (Verbose)**: insert new sections — Specification Mode (Steps
      S1–S4) and Plan Mode (Steps P1–P6)
- [ ] **Enhancement Techniques Reference**: note which techniques apply to spec/plan
      modes (T1, T2, T3, T10 apply; others generally do not)

---

## 8. Edge Cases (additions to existing table)

| Scenario | Behavior |
|----------|----------|
| `--plan` provided without specification-quality input | Warn: plan mode requires spec-quality input. Offer to run --specification first. Block plan until user confirms or provides a spec. |
| `--specification` on already-structured specification input | Run completeness analysis only — find what's missing, not what exists. Add missing elements and populate `<open_questions>` for unresolvable gaps. |
| `--specification + --plan` sequential: spec has open questions | Complete specification, then warn: "Plan cannot proceed until open questions are resolved." List open questions and block plan until user responds. |
| Very vague input with `--specification` | Produce maximum specification from available content; populate `<open_questions>` with everything that cannot be resolved. Deliver partial spec + full open questions list. |
| Auto-inferred mode is wrong | User replies "stop" to the one-line confirmation. Fall back to normal enhancement mode immediately. |
| `--specification` or `--plan` + `--minimal` | Ask: "Specification/plan mode and minimal mode are incompatible. Which mode should I use?" |
| `--specification` or `--plan` + `--verbose` | Ask: "Specification/plan mode and verbose mode are incompatible. Which mode should I use?" |
| Input already has a complete specification and `--plan` is inferred | Skip specification mode, proceed directly to plan pipeline. |
</task_decomposition>

<constraints>
BACKWARD COMPATIBILITY:
- All existing pipeline behavior (normal, --minimal, --verbose, --quiet) is unchanged
- New modes are additive only — no existing step is modified
- The 6a–6l verification system is reused in all modes; spec and plan modes add checks
  beyond 6l but do not remove or modify 6a–6l

COMPLETENESS STANDARD FOR --SPECIFICATION:
- Every concept, component, feature, behavior, requirement, and constraint from the
  input must appear in the specification output
- Nothing may be missing; nothing may be implied
- `<open_questions>` is the designated location for unresolvable gaps — using it does
  not count as a failure

GRANULARITY STANDARD FOR --PLAN:
- An action is insufficiently granular if it requires a sub-decision at execution time
- An action is insufficiently granular if its expected outcome is not independently
  verifiable
- The bar is: a new executor with no context beyond the plan can follow it exactly and
  produce the target outcome

INTEGRATION:
- --specification and --plan do not interact with --minimal or --verbose
- --quiet applies to both new modes (save-only behavior)
- Sequential --specification + --plan is a first-class combination, not an edge case
</constraints>

<verification>
This design is correct if and only if:
1. A developer can implement both modes by inserting new sections into SKILL.md without
   modifying any existing section
2. The sync checklist identifies every location in SKILL.md that requires updating
3. Trigger conditions for the new flags are as unambiguous as the existing
   --minimal/--verbose detection rules
4. Each pipeline step is specific enough to execute without interpretation
5. Output format templates are complete enough to be directly copied into SKILL.md
6. All flag combination cases are covered with unambiguous resolution rules
7. No existing mode behavior is altered
</verification>
```
