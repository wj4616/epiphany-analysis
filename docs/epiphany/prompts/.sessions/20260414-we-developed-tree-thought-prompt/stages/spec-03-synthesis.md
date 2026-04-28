# spec-03-synthesis.md

Output of MSPEC3 — Specification Synthesis (S5).
Scale: STANDARD. Mode: specification. Input type: A (concept + embedded workflow outline).

The draft `<specification>` XML below is the formal specification of the proposed
skill described in `00-input.md`. It integrates the domain analysis (`spec-01`) and
the extracted requirements (`spec-02`) into a single cohesive specification. No
`<meta source="epiphany-prompt"/>` or `<original_input>` blocks are included here —
MSPEC4M5 adds those during final assembly. No `<note>` block for failures is
included — that is also MSPEC4M5's responsibility.

---

```xml
<specification>

  <domain>
    <field>software — AI agent skill / prompt-engineering pipeline for Claude Code</field>
    <stakeholders>
      <stakeholder role="skill-author">Designs and maintains the epiphany-* family of skills.</stakeholder>
      <stakeholder role="claude-code-orchestrator">The AI agent that executes the skill's modules.</stakeholder>
      <stakeholder role="module-executor">Individual modules invoked programmatically per-stage (subagents).</stakeholder>
      <stakeholder role="downstream-consumer">Any human or skill that receives the improved input as output.</stakeholder>
      <stakeholder role="analysis-producer">epiphany-genius (primary) OR any other analysis method (secondary).</stakeholder>
      <stakeholder role="original-input-author">The human or system that produced node A (the input being improved).</stakeholder>
      <stakeholder role="future-integrator">Engineer who will later plug this skill into the epiphany-genius modular system.</stakeholder>
    </stakeholders>
    <existing_constraints>
      <constraint>MUST run programmatically (non-interactive execution by the Claude Code harness).</constraint>
      <constraint>MUST be modular so each stage maps onto an epiphany-genius module slot.</constraint>
      <constraint>MUST NOT overwrite the original input; MUST save a fresh final copy.</constraint>
      <constraint>MUST use ideation + synthesis + verification cycles (pattern borrowed from prompt-epiphany).</constraint>
      <constraint>MUST perform a full self-audit / verification for bug-free integration.</constraint>
      <constraint>MUST tolerate analyses NOT produced by epiphany-genius (graceful fallback path).</constraint>
      <constraint>SHOULD finish in bounded time ("we can't let this process take too long").</constraint>
      <constraint>Per-section analyses MUST be independent and optimized per section (10 sections total).</constraint>
    </existing_constraints>
  </domain>

  <scope>
    <item>Define a NEW skill (working name: "epiphany-analysis") that accepts a pair (input, analysis-of-input) and produces an improved version of the input.</item>
    <item>Specify a tree-of-thought / graph-of-thought pipeline over nodes A, B, A1, B1, C, D, E.</item>
    <item>Specify an optional extended pipeline over nodes x2 and x3 for per-section analysis branches.</item>
    <item>Specify how the skill detects whether node B was produced by any version of epiphany-genius, tolerating cross-version schema drift.</item>
    <item>Specify section-specific analysis paths for the 10 main sections of an epiphany-genius report.</item>
    <item>Specify the ideation → synthesis → verification cycle borrowed from prompt-epiphany, applied both per-idea and on the final integrated artifact.</item>
    <item>Specify the fresh-copy output rule (never overwrite the original input file).</item>
    <item>Specify modular design boundaries that enable future integration into epiphany-genius's modular system.</item>
    <item>Specify data shape for every node (A, B, A1, B1, C, D, E, x2, x3) and for the final output artifact.</item>
    <item>Specify coverage, audit-trail, and no-silent-drop requirements for ideas and solutions.</item>
  </scope>

  <out_of_scope>
    <item>Implementing the skill (this document is specification only — no code, no module files, no SKILL.md front-matter is produced here).</item>
    <item>Defining epiphany-genius itself or its report schema; epiphany-genius is treated as an external contract.</item>
    <item>Fetching, dereferencing, or re-analyzing the referenced example report at file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md (preserved verbatim as an example pointer only).</item>
    <item>UX / prompt wording for the end-user-facing interaction surface.</item>
    <item>Performance tuning targets beyond "not too long" (concrete latency and token-budget ceilings are out of scope for this spec).</item>
    <item>Migration or deprecation of prompt-epiphany (that skill is referenced as a pattern source, not modified).</item>
    <item>Any runtime call into prompt-epiphany; the borrow is a design pattern, not a runtime dependency.</item>
  </out_of_scope>

  <concept_decomposition>

    <part id="skill-identity">
      <description>A new AI agent skill (working name "epiphany-analysis") with its own activation triggers and module filenames; runs inside Claude Code.</description>
      <relationships>
        <relationship to="claude-code-orchestrator">Invoked by the orchestrator as a self-contained skill.</relationship>
        <relationship to="future-integrator">Exposes modular stage boundaries suitable for re-slotting into epiphany-genius.</relationship>
      </relationships>
    </part>

    <part id="node-A">
      <description>Storage for the ORIGINAL user-provided input. Verbatim, never mutated.</description>
      <relationships>
        <relationship to="input-interface">Ingested via inline prompt text, a file path, or another user-provided format (see open_questions).</relationship>
        <relationship to="node-A1">Node A1 is produced by fully analyzing node A.</relationship>
        <relationship to="final-artifact">Final artifact is a NEW file derived from, but never overwriting, node A's source.</relationship>
      </relationships>
    </part>

    <part id="node-B">
      <description>Storage for the analysis document that describes node A. Verbatim, with provenance metadata attached.</description>
      <relationships>
        <relationship to="input-interface">Ingested via file path or inline content.</relationship>
        <relationship to="detection-probe">Probe inspects B to set the from-epiphany-genius flag.</relationship>
        <relationship to="node-B1">Node B1 is produced by fully analyzing node B.</relationship>
      </relationships>
    </part>

    <part id="node-A1">
      <description>Exhaustive structured analysis of node A. Preserves every detail (technical, conceptual, fine, coarse). Never generalizes.</description>
      <relationships>
        <relationship to="node-A">Source of truth for A1.</relationship>
        <relationship to="idea-generator">Feeds idea generation together with B1.</relationship>
      </relationships>
    </part>

    <part id="node-B1">
      <description>Exhaustive structured analysis of node B. Same exhaustivity rule as A1. When B is an epiphany-genius report, B1 also includes a decomposition into the 10 main sections.</description>
      <relationships>
        <relationship to="node-B">Source of truth for B1.</relationship>
        <relationship to="ten-section-analyzer">B1's per-section decomposition feeds the ten-section branch.</relationship>
      </relationships>
    </part>

    <part id="node-C">
      <description>AI-optimized database of improvement ideas. Each entry is atomic (one idea). Schema is permitted to expand and create relationships as needed.</description>
      <relationships>
        <relationship to="idea-generator">Produced by the idea-generation loop over A1 and B1.</relationship>
        <relationship to="node-D">Each C entry is evaluated and produces either an accepted D entry or a recorded rejection.</relationship>
      </relationships>
    </part>

    <part id="node-D">
      <description>Accepted-ideas database. Stores ideas that passed the accept/discard filter with documented rationale, pros/cons, utility score, and complexity score. Rejected C entries are retained with rationale (no silent drops).</description>
      <relationships>
        <relationship to="node-C">Each D entry links back to a C entry via from_C_id.</relationship>
        <relationship to="node-E">Each accepted D entry seeds solution engineering in E.</relationship>
      </relationships>
    </part>

    <part id="node-E">
      <description>Solution store. Per accepted idea, two candidate solutions are generated; the pair is compared on pros, cons, utility, and over-engineering risk; only the best candidate is kept as the chosen solution.</description>
      <relationships>
        <relationship to="node-D">Each E entry links back to a D entry via from_D_id.</relationship>
        <relationship to="final-artifact">All E entries are integrated into the final artifact.</relationship>
      </relationships>
    </part>

    <part id="ten-section-analyzer">
      <description>When node B is identified as an epiphany-genius report, runs 10 independent section-optimized analysis branches, one per main section.</description>
      <relationships>
        <relationship to="detection-probe">Activated only when the probe returns true.</relationship>
        <relationship to="node-x2">Per-section accepted ideas are written into node x2.</relationship>
        <relationship to="headline-bias">The headline insight / primary conclusion section is processed first.</relationship>
      </relationships>
    </part>

    <part id="node-x2">
      <description>Per-section extension of node D. Each record carries a section_id in 1..10 and mirrors the D schema.</description>
      <relationships>
        <relationship to="ten-section-analyzer">Populated by per-section idea evaluation.</relationship>
        <relationship to="node-x3">Each x2 entry feeds per-section solution engineering into x3.</relationship>
      </relationships>
    </part>

    <part id="node-x3">
      <description>Per-section extension of node E. Each record carries a section_id in 1..10 and mirrors the E schema (two candidates, comparison, chosen solution).</description>
      <relationships>
        <relationship to="node-x2">Each x3 entry links back to an x2 entry.</relationship>
        <relationship to="final-artifact">All x3 entries are integrated into the final artifact alongside E entries.</relationship>
      </relationships>
    </part>

    <part id="detection-probe">
      <description>Heuristic that determines whether node B is a product of ANY version of epiphany-genius, tolerating cross-version schema drift. Sets the provenance flag (and, when available, a genius-version string).</description>
      <relationships>
        <relationship to="node-B">Operates on B's content and metadata.</relationship>
        <relationship to="generic-fallback">On negative detection, routes to the generic analysis branch.</relationship>
      </relationships>
    </part>

    <part id="generic-fallback">
      <description>Pipeline path taken when node B is not recognized as epiphany-genius output. Still produces a final enhanced artifact using the generic (non-per-section) pipeline over A1 and B1.</description>
      <relationships>
        <relationship to="detection-probe">Activated on negative detection.</relationship>
        <relationship to="final-artifact">Produces the same artifact shape as the genius branch (output-shape stability).</relationship>
      </relationships>
    </part>

    <part id="headline-bias">
      <description>Priority rule that treats the headline insight / primary conclusion as the first area of enhancement focus, before other sections are processed.</description>
      <relationships>
        <relationship to="ten-section-analyzer">Determines section processing order.</relationship>
      </relationships>
    </part>

    <part id="theory-collision-handler">
      <description>For each theory collision in the analysis, compares claim A vs. claim B under the discriminating condition, resolves the collision, and synthesizes an enhanced solution from the resolution.</description>
      <relationships>
        <relationship to="node-B1">Collisions are surfaced during B1 analysis.</relationship>
        <relationship to="node-E">Resolved collisions feed solution records in E (or x3 when per-section).</relationship>
      </relationships>
    </part>

    <part id="alternative-hypothesis-filter">
      <description>Selection filter that recognizes most alternative hypotheses are wrong and picks exactly one correct hypothesis before generating enhancements from it; rejected hypotheses are recorded.</description>
      <relationships>
        <relationship to="node-B1">Hypotheses surface during B1 analysis.</relationship>
        <relationship to="node-E">Chosen hypothesis seeds an enhancement solution.</relationship>
      </relationships>
    </part>

    <part id="isv-cycle">
      <description>The ideation → synthesis → verification cycle borrowed from prompt-epiphany. Applied per-idea (during solution engineering) AND on the final integrated artifact.</description>
      <relationships>
        <relationship to="prompt-epiphany">Borrowed as a design pattern, not a runtime dependency.</relationship>
        <relationship to="node-E">Each solution record documents ideation, synthesis, verification sub-steps.</relationship>
        <relationship to="final-artifact">Final integration passes through the same three-phase cycle.</relationship>
      </relationships>
    </part>

    <part id="self-audit-gate">
      <description>Terminal self-audit that verifies bug-free integration, absence of errors/issues/potential problems, no regressions, and no over-engineering. Blocks or warns based on failure class (policy pending — see open_questions).</description>
      <relationships>
        <relationship to="final-artifact">Gates emission of the final artifact.</relationship>
        <relationship to="isv-cycle">Implements the verification phase at the integration level.</relationship>
      </relationships>
    </part>

    <part id="final-artifact">
      <description>A fresh, separate file containing the full enhanced version of node A, integrating every chosen solution from E and (when applicable) x3, accompanied by an audit / verification report. Never overwrites node A.</description>
      <relationships>
        <relationship to="node-A">Derived from node A but written to a NEW path.</relationship>
        <relationship to="node-E">Integrates every E entry.</relationship>
        <relationship to="node-x3">Integrates every x3 entry when the per-section branch ran.</relationship>
      </relationships>
    </part>

    <part id="modular-integration-hook">
      <description>Contract that each stage (detection, A1, B1, C, D, E, x2, x3, integration, audit) is an independently addressable module with a stable input/output data contract, enabling future re-slotting into epiphany-genius's modular system.</description>
      <relationships>
        <relationship to="future-integrator">Consumed when integrating with epiphany-genius's module registry.</relationship>
      </relationships>
    </part>

  </concept_decomposition>

  <requirements>

    <!-- Functional Requirements -->

    <requirement id="FR-1" class="MUST">
      <text>The system shall accept a user-provided original input and store it verbatim as node A, without mutation, regardless of whether the input is delivered inline in the user prompt, as a file path, or in another user-provided format.</text>
      <rationale>Node A is the source of truth for the improved artifact; verbatim preservation is required by the no-data-loss rule and by the input's ingestion directive.</rationale>
      <source>00-input.md lines 11–14.</source>
    </requirement>

    <requirement id="FR-2" class="MUST">
      <text>The system shall accept an analysis document describing node A and store it verbatim as node B, preserving provenance metadata including a boolean from-epiphany-genius flag.</text>
      <rationale>B drives the improvement process; provenance must be captured so the correct branch (genius per-section vs. generic) is selected later.</rationale>
      <source>00-input.md lines 15, 66–67.</source>
    </requirement>

    <requirement id="FR-3" class="MUST">
      <text>The system shall produce node A1, a full analysis of node A in which every detail — concept, idea, facet (large, small, fine), and technical detail — is reviewed and recorded; the system shall not generalize or gloss over any detail.</text>
      <rationale>Downstream idea generation requires exhaustive detail or it will miss improvement vectors.</rationale>
      <source>00-input.md lines 16–18.</source>
    </requirement>

    <requirement id="FR-4" class="MUST">
      <text>The system shall produce node B1 applying the same exhaustivity rule as FR-3 to node B.</text>
      <rationale>Identical rationale to FR-3, applied to the analysis input.</rationale>
      <source>00-input.md lines 16–18.</source>
    </requirement>

    <requirement id="FR-5" class="MUST">
      <text>The system shall generate improvement ideas by reasoning over how node B1's meaning yields insight, bug fixes, or enhancements for node A (as represented in A1), and shall store each idea as an individual atomic element in node C.</text>
      <rationale>Atomic storage enables per-idea evaluation, solution engineering, and audit-trail traceability.</rationale>
      <source>00-input.md lines 20–24.</source>
    </requirement>

    <requirement id="FR-6" class="MUST">
      <text>Node C shall be represented in an AI-optimized database form that is permitted to expand its schema and create relationships between entries as needed; the same rule shall apply to nodes D, E, x2, and x3.</text>
      <rationale>The input's explicit "expand and create relationships as necessary for ideal design" directive. Enables cross-node traceability.</rationale>
      <source>00-input.md lines 24–27.</source>
    </requirement>

    <requirement id="FR-7" class="MUST">
      <text>The system shall iterate idea generation with varied roles and goals to surface additional creative perspectives, appending every new idea to node C.</text>
      <rationale>Prevents single-perspective blind spots and maximizes idea coverage before termination.</rationale>
      <source>00-input.md lines 29–31.</source>
    </requirement>

    <requirement id="FR-8" class="MUST">
      <text>The system shall continue idea generation until no new improvement or problem-resolution ideas can be produced from the available context, then terminate the idea-generation loop.</text>
      <rationale>The input's "run until out of ideas completely" termination rule.</rationale>
      <source>00-input.md lines 33–34.</source>
    </requirement>

    <requirement id="FR-9" class="MUST">
      <text>The system shall iterate over every entry in node C exactly once during evaluation and shall not skip any entry.</text>
      <rationale>Coverage guarantee — no silent drop of ideas before they are even evaluated.</rationale>
      <source>00-input.md lines 37–38, 47.</source>
    </requirement>

    <requirement id="FR-10" class="MUST">
      <text>For each idea in node C, the system shall decide whether to use it based on overall utility, relevancy, realism, avoidance of excessive over-engineering or complexity, and capacity to resolve issues; accepted ideas shall be stored in node D and rejected ideas shall be recorded with a rationale.</text>
      <rationale>The input's accept/discard rule, combined with auditability — rejected ideas must remain on record.</rationale>
      <source>00-input.md lines 41–45.</source>
    </requirement>

    <requirement id="FR-11" class="MUST">
      <text>The system shall apply the variation-of-roles / perspectives rule during idea evaluation as well as during generation.</text>
      <rationale>The input explicitly repeats the "new perspectives with different roles and goals" directive within the evaluation step.</rationale>
      <source>00-input.md line 42.</source>
    </requirement>

    <requirement id="FR-12" class="MUST">
      <text>For each accepted idea in node D, the system shall engineer a solution that achieves the intended improvement or resolution against node A and shall store each solution as an individual entry in node E.</text>
      <rationale>Per-idea solution engineering is the bridge between accepted ideas and the final integrated artifact.</rationale>
      <source>00-input.md lines 48–51.</source>
    </requirement>

    <requirement id="FR-13" class="MUST">
      <text>For each accepted idea, the system shall generate two candidate solutions separately.</text>
      <rationale>The input's explicit two-candidate rule; generating two candidates forces comparative thinking and reduces premature commitment.</rationale>
      <source>00-input.md line 52.</source>
    </requirement>

    <requirement id="FR-14" class="MUST">
      <text>The system shall compare the two candidate solutions on pros, cons, utility, and over-engineering risk, and shall pass only the best candidate into node E's chosen field.</text>
      <rationale>Enforces quality selection and prevents both candidates from propagating and bloating the integration step.</rationale>
      <source>00-input.md lines 53–54.</source>
    </requirement>

    <requirement id="FR-15" class="MUST">
      <text>The system shall apply a three-phase ideation → synthesis → verification cycle to each individual idea, modeled on prompt-epiphany's pattern.</text>
      <rationale>Per-idea ISV applies proven quality controls from prompt-epiphany to each solution before it reaches integration.</rationale>
      <source>00-input.md lines 54–55.</source>
    </requirement>

    <requirement id="FR-16" class="MUST">
      <text>The system shall continue the solution-engineering loop until every accepted idea in node D has a corresponding solution recorded in node E.</text>
      <rationale>Coverage guarantee — every accepted idea must produce a solution, otherwise the final artifact is incomplete.</rationale>
      <source>00-input.md line 57.</source>
    </requirement>

    <requirement id="FR-17" class="MUST">
      <text>The system shall produce a final version of the original input that flushly integrates all solutions held in node E, enhancing the original based on the analysis and verified for accuracy and completeness.</text>
      <rationale>This is the skill's primary deliverable — a single cohesive enhanced version of node A.</rationale>
      <source>00-input.md lines 58–60.</source>
    </requirement>

    <requirement id="FR-18" class="MUST">
      <text>The system shall apply the ideation → synthesis → verification cycle to the final integrated artifact, not only to individual ideas.</text>
      <rationale>Integration is where conflicts and regressions emerge; applying ISV at the top level catches them.</rationale>
      <source>00-input.md line 60.</source>
    </requirement>

    <requirement id="FR-19" class="MUST">
      <text>The system shall run a self-audit on the integrated artifact focused on bug-free integration and absence of errors, issues, and potential problems.</text>
      <rationale>The input's "verify all changes integrated flush with self-audit" directive.</rationale>
      <source>00-input.md line 62.</source>
    </requirement>

    <requirement id="FR-20" class="MUST">
      <text>The system shall save the final enhanced output to a NEW file and shall not overwrite the existing specification (node A's source file).</text>
      <rationale>Preserves the original while delivering the improvement; enables side-by-side comparison and rollback.</rationale>
      <source>00-input.md line 64.</source>
    </requirement>

    <requirement id="FR-21" class="MUST">
      <text>The system shall check whether node B is the product of any version of epiphany-genius (including versions that differ slightly due to updates) and shall set the provenance flag accordingly.</text>
      <rationale>Branch selection for the per-section analysis depends on this detection; cross-version tolerance is explicitly required.</rationale>
      <source>00-input.md line 66.</source>
    </requirement>

    <requirement id="FR-22" class="MUST">
      <text>When node B is not identified as epiphany-genius output, the system shall still perform the improvement pipeline on node A using node B as a generic analysis source and shall produce the final enhanced artifact.</text>
      <rationale>Robustness requirement — the skill must degrade gracefully to a generic analysis path.</rationale>
      <source>00-input.md line 67.</source>
    </requirement>

    <requirement id="FR-23" class="MUST">
      <text>When node B is an epiphany-genius report, the system shall treat it as having 10 main sections and shall run an independent, section-optimized analysis for each section.</text>
      <rationale>The input asserts an explicit 10-section structure and demands section-tailored handling.</rationale>
      <source>00-input.md lines 68–69.</source>
    </requirement>

    <requirement id="FR-24" class="SHOULD">
      <text>The system should tailor each section's analysis to remain within a reasonable total runtime; the overall process should not take excessively long.</text>
      <rationale>The input explicitly warns that the process cannot take too long, but does not supply a numeric bound (see OQ-10).</rationale>
      <source>00-input.md line 69.</source>
    </requirement>

    <requirement id="FR-25" class="SHOULD">
      <text>The system should treat the headline insight / primary conclusion of the analysis as the initial focus area for enhancement before addressing other portions of the analysis.</text>
      <rationale>The input's priority-rule language is hedged ("probably a main indicator"), so the requirement is recommended rather than mandatory.</rationale>
      <source>00-input.md line 70.</source>
    </requirement>

    <requirement id="FR-26" class="MUST">
      <text>For each theory collision in the analysis, the system shall compare claim A and claim B, take the discriminating condition into account, decide what the collision implies, and synthesize an enhanced solution based on the resolution.</text>
      <rationale>The input demands explicit per-collision reasoning to produce enhancement value.</rationale>
      <source>00-input.md line 71.</source>
    </requirement>

    <requirement id="FR-27" class="MUST">
      <text>For the alternative-hypothesis section of the analysis, the system shall recognize that most listed hypotheses are not correct and shall select the single correct version before generating an enhancement from it.</text>
      <rationale>Without filtering, spurious hypotheses would bleed into enhancements and degrade quality.</rationale>
      <source>00-input.md line 72.</source>
    </requirement>

    <requirement id="FR-28" class="MUST">
      <text>The system shall examine each of the 10 sections and decide how to make the analysis cater to that particular section's designed focus, producing a per-section recipe.</text>
      <rationale>Section-tailoring is explicitly called out; the recipe catalog is the artifact of that tailoring.</rationale>
      <source>00-input.md line 73.</source>
    </requirement>

    <requirement id="FR-29" class="MUST">
      <text>Within the per-section branch, each idea generated per section shall be stored in its database entry, analyzed for pros, cons, utility, and other evaluation criteria; ideas judged usable shall be stored in node x2.</text>
      <rationale>Per-section branches need their own C/D equivalent; x2 is the accepted-idea store for that branch.</rationale>
      <source>00-input.md lines 74–76.</source>
    </requirement>

    <requirement id="FR-30" class="MUST">
      <text>The system shall iterate the items in node x2, develop the solution to implement each improvement/enhancement or resolve each issue, and shall store each solution in node x3.</text>
      <rationale>Per-section E equivalent — x3 holds the engineered solution for each x2 item.</rationale>
      <source>00-input.md line 77.</source>
    </requirement>

    <requirement id="FR-31" class="MUST">
      <text>The system shall iterate over all solutions in node x3, integrate all of them to resolve all items, and merge them into the final enhanced version produced by FR-17.</text>
      <rationale>Without this integration step, per-section solutions would not reach the user-facing artifact.</rationale>
      <source>00-input.md lines 78–79.</source>
    </requirement>

    <requirement id="FR-32" class="MUST">
      <text>The final step before output shall be a full verification and validation pass modeled on prompt-epiphany's final checks.</text>
      <rationale>A named final gate aligned with prompt-epiphany ensures consistency across the epiphany-* family.</rationale>
      <source>00-input.md line 80.</source>
    </requirement>

    <requirement id="FR-33" class="MUST">
      <text>The system shall implement its pipeline as an optimized tree-of-thought or graph-of-thought modular flow designed for the Claude Code AI agent system.</text>
      <rationale>A non-linear reasoning graph is the architectural requirement explicitly stated by the input.</rationale>
      <source>00-input.md line 81.</source>
    </requirement>

    <requirement id="FR-34" class="MAY">
      <text>The system may conduct research or ask clarifying questions as needed during execution.</text>
      <rationale>The input grants permission ("do research or ask questions as necessary") without forcing the action.</rationale>
      <source>00-input.md line 82.</source>
    </requirement>

    <requirement id="FR-35" class="SHOULD">
      <text>The system should treat the referenced file at file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md as an example of the analysis form epiphany-analysis targets; the system shall not dereference or fetch the file during the specification phase.</text>
      <rationale>The file is cited as an example only; preserving it verbatim without fetching respects Hard Gate 3 and treats input as data.</rationale>
      <source>00-input.md line 66.</source>
    </requirement>

    <requirement id="FR-36" class="MUST">
      <text>The system shall follow the ideation → synthesis → verification pattern as used in prompt-epiphany, applied both per-idea (FR-15) and at final integration (FR-18).</text>
      <rationale>Explicitly named in multiple places; the borrowed pattern is load-bearing across the pipeline.</rationale>
      <source>00-input.md lines 5, 55, 60, 80.</source>
    </requirement>

    <!-- Non-Functional Requirements -->

    <requirement id="NFR-1" class="MUST">
      <text>The system shall be runnable programmatically, without requiring interactive user prompts for its normal flow.</text>
      <rationale>Declared as a top-level design constraint by the input.</rationale>
      <source>00-input.md line 6.</source>
    </requirement>

    <requirement id="NFR-2" class="MUST">
      <text>The system shall be modular such that its stages map cleanly onto epiphany-genius's modular system, enabling future re-engineering.</text>
      <rationale>Future integration is an explicit first-class goal of the skill design.</rationale>
      <source>00-input.md line 6.</source>
    </requirement>

    <requirement id="NFR-3" class="MUST">
      <text>Analyses A1 and B1 shall preserve every detail, including technical minutiae (numeric values, file paths, code, API names, quoted strings), and shall not generalize.</text>
      <rationale>Detail loss here propagates and compounds into every downstream node and the final artifact.</rationale>
      <source>00-input.md lines 16–18.</source>
    </requirement>

    <requirement id="NFR-4" class="MUST">
      <text>Every accepted change in the final artifact shall be traceable back to a specific idea in C, a decision in D, a solution in E, and (when applicable) section-aware ideas/solutions in x2/x3.</text>
      <rationale>Auditability is implied by the "be sure we don't miss anything" rule and by the per-idea provenance requirement.</rationale>
      <source>Implied by 00-input.md lines 11–79.</source>
    </requirement>

    <requirement id="NFR-5" class="MUST">
      <text>The system shall not mutate or overwrite node A or its source file.</text>
      <rationale>Hard-line data-loss prevention; the input states this verbatim.</rationale>
      <source>00-input.md line 64.</source>
    </requirement>

    <requirement id="NFR-6" class="MUST">
      <text>Rejected ideas shall be retained for audit with a rationale and shall not be silently discarded.</text>
      <rationale>Audit trail requires every C entry to have a recorded verdict.</rationale>
      <source>00-input.md lines 45, 47.</source>
    </requirement>

    <requirement id="NFR-7" class="MUST">
      <text>Accepted ideas and chosen solutions shall be realistic to implement and shall not introduce complexity disproportionate to the improvement delivered; regressions shall not be introduced.</text>
      <rationale>Explicit over-engineering and regression prohibitions from the input.</rationale>
      <source>00-input.md lines 41–43.</source>
    </requirement>

    <requirement id="NFR-8" class="SHOULD">
      <text>The overall process should complete within a bounded time window; per-section analyses should be tailored so total runtime remains reasonable.</text>
      <rationale>Time-bound directive is hedged ("we can't let this process take too long") — binding intent without a numeric target.</rationale>
      <source>00-input.md line 69.</source>
    </requirement>

    <requirement id="NFR-9" class="MUST">
      <text>The system shall complete successfully for analyses not produced by epiphany-genius, taking the generic-analysis fallback path without crashing.</text>
      <rationale>Robustness across analysis sources is explicitly required.</rationale>
      <source>00-input.md line 67.</source>
    </requirement>

    <requirement id="NFR-10" class="SHOULD">
      <text>The final artifact's shape (fresh file, full enhanced content, verification report) should be stable regardless of whether the epiphany-genius branch or the generic branch was taken.</text>
      <rationale>Output-shape stability simplifies downstream consumption and testing.</rationale>
      <source>Implied by 00-input.md lines 64 and 67.</source>
    </requirement>

    <requirement id="NFR-11" class="SHOULD" origin="domain-inferred">
      <text>The system should produce an execution log that records stage entries, stage exits, iteration counts, termination conditions, detection results, audit outcomes, and any research/clarification events.</text>
      <rationale>Standard auditable-pipeline practice; required to substantiate NFR-4's traceability in operation.</rationale>
      <source>Domain-inferred — consistent with NFR-4 and FR-19.</source>
    </requirement>

    <requirement id="NFR-12" class="SHOULD" origin="domain-inferred">
      <text>When the 10 per-section analyses are executed concurrently to meet the time bound, the system should prevent race conditions over shared databases (C, D, E, x2, x3).</text>
      <rationale>Direct consequence of FR-23 / FR-24 / FR-28 combined with NFR-8; whether sections actually run in parallel is a design choice (see OQ-10).</rationale>
      <source>Domain-inferred.</source>
    </requirement>

    <!-- Data Requirements -->

    <requirement id="DR-1" class="MUST">
      <text>Node A shall store the verbatim original input content with its original format preserved.</text>
      <rationale>Verbatim preservation is the foundation of auditability and no-data-loss guarantees.</rationale>
      <source>00-input.md lines 11–14.</source>
    </requirement>

    <requirement id="DR-2" class="MUST">
      <text>Node B shall store the verbatim analysis content plus a provenance flag `from-epiphany-genius: bool` and, when detectable, `genius-version: string`.</text>
      <rationale>Provenance is required by branch selection and by cross-version tolerance.</rationale>
      <source>00-input.md lines 15, 66.</source>
    </requirement>

    <requirement id="DR-3" class="MUST">
      <text>A1 and B1 shall each include a structured decomposition of their source node covering technical details, concepts, ideas, facets, and (for B1 when applicable) a per-section breakdown into the 10 main sections.</text>
      <rationale>Required structure for downstream idea generation and per-section branching.</rationale>
      <source>00-input.md lines 16–18, 68.</source>
    </requirement>

    <requirement id="DR-4" class="MUST">
      <text>Each node-C record shall carry `{id, idea_text, origin_pointer, generating_role}`.</text>
      <rationale>Minimum fields required to preserve provenance and enable audit.</rationale>
      <source>00-input.md lines 20–31.</source>
    </requirement>

    <requirement id="DR-5" class="MUST">
      <text>Each node-D record shall carry `{id, from_C_id, verdict (accept|reject), rationale, pro_list, con_list, utility_score, complexity_score}`; rejected entries shall be retained with `verdict = reject`.</text>
      <rationale>Enables the accept/reject audit trail and scoring-based acceptance.</rationale>
      <source>00-input.md lines 41–45.</source>
    </requirement>

    <requirement id="DR-6" class="MUST">
      <text>Each node-E record shall carry `{id, from_D_id, candidate_1, candidate_2, comparison, chosen, justification}`.</text>
      <rationale>Captures the two-candidate generation, comparison, and selection in a single record.</rationale>
      <source>00-input.md lines 48–54.</source>
    </requirement>

    <requirement id="DR-7" class="MUST">
      <text>Each node-x2 record shall carry everything in DR-5 plus a `section_id` in `1..10`.</text>
      <rationale>Per-section parallel of D; section_id enables section-aware traceability.</rationale>
      <source>00-input.md lines 68, 74–76.</source>
    </requirement>

    <requirement id="DR-8" class="MUST">
      <text>Each node-x3 record shall carry everything in DR-6 plus a `section_id` in `1..10`.</text>
      <rationale>Per-section parallel of E; section_id preserves section context through integration.</rationale>
      <source>00-input.md line 77.</source>
    </requirement>

    <requirement id="DR-9" class="MUST">
      <text>The final artifact shall contain the full enhanced version of node A with every E and (when applicable) x3 solution integrated, plus an audit/verification report (see FR-19 and FR-32).</text>
      <rationale>Specifies the deliverable's content and mandatory companion report.</rationale>
      <source>00-input.md lines 58–64, 78–80.</source>
    </requirement>

    <!-- Constraints -->

    <requirement id="CON-1" class="MUST_NOT">
      <text>The system shall not write to, truncate, or otherwise modify the source file of node A.</text>
      <rationale>Reinforces NFR-5 at the file-system-operation level.</rationale>
      <source>00-input.md line 64.</source>
    </requirement>

    <requirement id="CON-2" class="MUST_NOT">
      <text>The system shall not discard any generated idea without recording the discard with a rationale.</text>
      <rationale>Complements NFR-6; prevents silent drops at any point in the pipeline.</rationale>
      <source>00-input.md lines 45, 47.</source>
    </requirement>

    <requirement id="CON-3" class="MUST_NOT">
      <text>The system shall not accept ideas or solutions that introduce excessive complexity, over-engineering, or regression.</text>
      <rationale>Reinforces NFR-7 as a hard rejection criterion.</rationale>
      <source>00-input.md lines 41–43.</source>
    </requirement>

    <requirement id="CON-4" class="MUST_NOT">
      <text>A1 and B1 shall not generalize or gloss over details.</text>
      <rationale>Reinforces NFR-3 at the analysis-step level.</rationale>
      <source>00-input.md lines 16–18.</source>
    </requirement>

    <requirement id="CON-5" class="MUST_NOT">
      <text>During the specification phase, the system shall not dereference, fetch, or analyze the referenced file at file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md; the path is a data reference only.</text>
      <rationale>The reference is an example pointer; fetching during spec would change the nature of this document from specification to analysis.</rationale>
      <source>Hard Gate 3 in the module protocol; input treated as data.</source>
    </requirement>

    <requirement id="CON-6" class="MUST">
      <text>Of each two-candidate comparison, only one solution shall enter node E's chosen field; the other shall be recorded as rejected with rationale.</text>
      <rationale>Prevents ambiguity at integration time and ensures one canonical solution per accepted idea.</rationale>
      <source>00-input.md line 53.</source>
    </requirement>

  </requirements>

  <interfaces>

    <interface id="IR-1" name="node-A-input" direction="inbound">
      <description>Accepts node A content.</description>
      <accepted_forms>
        <form>Inline prompt content.</form>
        <form>User-provided file path.</form>
        <form>Other user-provided format (enumeration pending — see OQ-14).</form>
      </accepted_forms>
      <contract>On ingestion, the system writes the content verbatim into the node-A record; no transformation or normalization is applied before downstream stages.</contract>
      <source>00-input.md lines 13–14.</source>
    </interface>

    <interface id="IR-2" name="node-B-input" direction="inbound">
      <description>Accepts the analysis document describing node A.</description>
      <accepted_forms>
        <form>File path.</form>
        <form>Inline content.</form>
      </accepted_forms>
      <contract>On ingestion, the system records the content verbatim into node B and runs the detection probe to set `from-epiphany-genius` (and, when available, `genius-version`).</contract>
      <source>00-input.md lines 15, 66.</source>
    </interface>

    <interface id="IR-3" name="enhanced-output" direction="outbound">
      <description>Writes the final enhanced version of node A to a NEW file at a path derived from (but distinct from) the source of node A.</description>
      <contract>The output path differs from node A's source path; the output file contains the full enhanced content plus the companion audit/verification report; node A's source file is untouched (pre- vs. post-run hash equality).</contract>
      <source>00-input.md line 64.</source>
      <notes>Exact path-derivation convention is pending — see OQ-2.</notes>
    </interface>

    <interface id="IR-4" name="external-reference" direction="reference-only">
      <description>Preserves the reference URL file:///home/myuser/docs/epiphany/genius/20260414-prompt-cog-skill-design-spec/report.md verbatim in the technical-details inventory without fetching or dereferencing it during the specification phase.</description>
      <contract>The URL appears byte-for-byte in this spec and in subsequent specification artifacts; no network or filesystem fetch is performed against the URL.</contract>
      <source>00-input.md line 66.</source>
    </interface>

    <interface id="IR-5" name="internal-node-interface" direction="internal">
      <description>Each of nodes A, B, A1, B1, C, D, E, x2, x3 has a defined producer stage and consumer stage(s), with a stable data contract that downstream modules can consume without re-deriving structure.</description>
      <contract>Module-contract documentation enumerates the producer stage, consumer stage(s), and schema reference (DR-1..DR-8) for each node; modules are independently addressable to enable future integration with epiphany-genius.</contract>
      <source>00-input.md lines 11–79 (pipeline description) combined with the modularity rule (line 6).</source>
    </interface>

    <interface id="IR-6" name="sibling-pattern-reference" direction="pattern-borrow">
      <description>Documents the per-idea (FR-15) and final-integration (FR-18) cycles as instances of the prompt-epiphany ideation → synthesis → verification pattern. This is a pattern reference, not a runtime dependency.</description>
      <contract>Specification and documentation name the borrowed pattern and identify its application points; no runtime invocation of prompt-epiphany is performed.</contract>
      <source>00-input.md lines 5, 55, 60, 80.</source>
    </interface>

  </interfaces>

  <success_criteria>
    <criterion id="SC-1">A final, fresh, enhanced version of node A is produced that integrates every chosen solution from node E (and, when applicable, from node x3), with every change traceable back to a specific analysis finding via the C → D → E (or C → x2 → x3) audit chain.</criterion>
    <criterion id="SC-2">The produced artifact passes the self-audit for bug-free integration, absence of errors/issues/potential problems, no regressions introduced, no dropped requirements from node A, and no over-engineering.</criterion>
    <criterion id="SC-3">The stage contract for nodes A, B, A1, B1, C, D, E, and optional x2/x3 is honored — each node has defined contents, a producer stage, and a consumer stage, matching the data contracts in DR-1 through DR-8.</criterion>
    <criterion id="SC-4">Every idea in node C has a recorded verdict (accept or reject) with a non-empty rationale; no idea is silently dropped.</criterion>
    <criterion id="SC-5">Every accepted idea in node D has exactly one chosen solution recorded in node E (and a matching x3 entry per section when the per-section branch ran).</criterion>
    <criterion id="SC-6">The detection probe returns true for any epiphany-genius analysis (including cross-version samples with minor schema drift) and false otherwise, and the appropriate branch (per-section vs. generic) executes.</criterion>
    <criterion id="SC-7">The generic-analysis fallback path completes successfully on non-epiphany-genius analyses without crashing and produces the same artifact shape as the per-section branch.</criterion>
    <criterion id="SC-8">Node A's source file is unchanged after the run (pre- vs. post-run hash equality) AND a new file exists at the enhanced-output path containing the full enhanced content.</criterion>
    <criterion id="SC-9">The stage graph is a tree-of-thought or graph-of-thought structure — not a purely linear pipeline — with documented branches and joins.</criterion>
    <criterion id="SC-10">Each of the 10 main sections (when the per-section branch ran) has an independent section-optimized analysis record and recipe tag, with the headline/primary-conclusion section processed first.</criterion>
  </success_criteria>

  <assumptions>
    <assumption id="A-1">Node B is provided in a text-processable form (plain text, markdown, JSON, or similar) that supports structural inspection by the detection probe.</assumption>
    <assumption id="A-2">When node B is an epiphany-genius report, it actually contains the 10 main sections; mild schema drift across versions is tolerated, but the 10-section structure is assumed to remain identifiable.</assumption>
    <assumption id="A-3">Node A is small enough to analyze exhaustively within a reasonable time window; the spec does not yet define behavior for extremely large A (flagged as OQ-10 via runtime bound).</assumption>
    <assumption id="A-4">The prompt-epiphany skill exists and its ideation → synthesis → verification pattern is documented sufficiently to be borrowed; no runtime call into prompt-epiphany is required.</assumption>
    <assumption id="A-5">The Claude Code AI agent system is the runtime target; modular stages are invoked by the orchestrator per the existing skill-execution conventions.</assumption>
    <assumption id="A-6">The reference file URL in IR-4 is not required to exist or be readable for this specification to be correct; only its verbatim preservation is required.</assumption>
    <assumption id="A-7">The "AI-optimized database" is realizable in the deployment environment (concrete backing store is deferred to implementation — see OQ-3).</assumption>
    <assumption id="A-8">Rejected ideas and candidate solutions can be stored alongside accepted ones without violating token or storage budgets; if budgets become binding, a summarized-rejection format would still satisfy NFR-6.</assumption>
  </assumptions>

  <constraints>
    <constraint id="C-1" refs="CON-1 NFR-5">Never overwrite node A or its source file.</constraint>
    <constraint id="C-2" refs="CON-2 NFR-6">Never silently drop ideas; every C entry has a recorded verdict with rationale.</constraint>
    <constraint id="C-3" refs="CON-3 NFR-7">No over-engineering and no regressions in accepted ideas or chosen solutions.</constraint>
    <constraint id="C-4" refs="CON-4 NFR-3">Never generalize or gloss over details in A1 and B1.</constraint>
    <constraint id="C-5" refs="CON-5 IR-4">During the specification phase, never dereference or fetch the referenced example report URL.</constraint>
    <constraint id="C-6" refs="CON-6 FR-14">Of each two-candidate comparison, only one solution enters node E's chosen field.</constraint>
    <constraint id="C-7" refs="NFR-1">Execution is programmatic — no interactive user prompts in the normal flow.</constraint>
    <constraint id="C-8" refs="NFR-2 FR-33">Stages are modular and implemented as an independently addressable tree-of-thought / graph-of-thought graph, designed to be re-slotted into epiphany-genius.</constraint>
    <constraint id="C-9" refs="NFR-8 FR-24">Total runtime is bounded; per-section work is tailored to keep within a reasonable total duration (numeric target pending — OQ-10).</constraint>
    <constraint id="C-10" refs="NFR-9 FR-22">Robustness across analysis sources — the generic fallback path completes without crashing on non-genius analyses.</constraint>
    <constraint id="C-11" refs="NFR-10">Output shape is stable across branches (genius per-section vs. generic).</constraint>
  </constraints>

  <open_questions>
    <question id="OQ-1">What is the canonical skill name — "epiphany-analysis" or a different name?</question>
    <question id="OQ-2">What is the precise output file path convention for the enhanced version (prefix, suffix, directory, collision handling)?</question>
    <question id="OQ-3">What concrete representation is the "AI-optimized database" (JSON, JSONL, embedded store such as SQLite, in-memory only, a vector store, or a mix)? Does it persist across runs?</question>
    <question id="OQ-4">What roster of roles/perspectives is rotated through during ideation and evaluation, and how many rotations are performed per phase?</question>
    <question id="OQ-5">What are the concrete utility and complexity scoring rubrics for node D, including the thresholds that define "excessive over-engineering"?</question>
    <question id="OQ-6">What is the exact detection heuristic for "is this an epiphany-genius report?" given the cross-version tolerance requirement (signature fields, version flag, structural fingerprint, section-header regex)?</question>
    <question id="OQ-7">What are the 10 main sections of the target epiphany-genius report, and what is the section-optimized analysis recipe for each?</question>
    <question id="OQ-8">What is the discriminating-condition field format for theory-collision records (free text, structured predicate, typed object)?</question>
    <question id="OQ-9">What is the selection rule for choosing a single "correct" alternative hypothesis from the list?</question>
    <question id="OQ-10">What is the time budget / iteration cap for the "iterate until out of ideas" loop and for the overall process? Are the 10 per-section analyses run in parallel or sequentially?</question>
    <question id="OQ-11">What is the tie-break rule when two candidate solutions in node E are judged equivalent on pros/cons/utility/over-engineering?</question>
    <question id="OQ-12">What is the conflict-resolution rule when two accepted ideas contradict each other during integration?</question>
    <question id="OQ-13">Which self-audit failure modes block emission of the final artifact versus being surfaced as warnings?</question>
    <question id="OQ-14">What input formats beyond inline prompt content and file paths must node A accept?</question>
    <question id="OQ-15">What is the exact mapping between epiphany-analysis stages and epiphany-genius module slots for future integration?</question>
    <question id="OQ-16">Is there a minimum-improvement threshold below which no enhanced file is produced, and if so, how is the threshold measured?</question>
  </open_questions>

</specification>
```
