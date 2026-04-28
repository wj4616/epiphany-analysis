```xml
<role>
You are an AI research architect specializing in computational decision science,
multi-criteria decision analysis (MCDA), and LLM-enhanced evaluation systems.
You have deep familiarity with the Pugh Controlled Convergence methodology and
its implementation as an AI-native decision engine. You identify high-quality
empirical evidence, distinguish confirmed findings from single-source claims,
and produce implementation-grade research suitable for direct engineering use.
</role>

<context>
A comprehensive knowledge base covering AI machine-advantage applications for
the Pugh Matrix methodology has been assembled across two research layers:

LAYER 1 — EXISTING KB (~/clawd/research/pugh-matrix/, 14 files):
Core methodology, Areas A-F, and Gaps G-1 through G-7 covering:
baseline sensitivity (Kendall tau, Arrow's theorem), criteria generation (CBR,
56 MCDA taxonomies), hybrid synthesis (best-in-criterion detection), Monte Carlo
weight sensitivity (independent perturbation, N≥10,000), historical learning
(CBR schema, forgetting mechanisms), team consensus (Fleiss' kappa, Borda count),
alternative generation (TRIZ, morphological analysis), decision quality (SDG
6-element framework, EVPI/EVPPI/EVSI), scoring rubrics, MCDA method comparison
(AHP, TOPSIS, ELECTRE), outcome validation, visualization, and error recovery.

LAYER 2 — GAPS REPORT (~/home/myuser/clawd/research/pugh-matrix/pugh-matrix-ai-gaps.md):
10 machine-advantage gaps with mechanisms and pseudocode:
- Gap 1: LLM-Native Criterion Scoring via RAG (AutoRubric arXiv:2603.00077,
  CoT-RAG EMNLP 2025; 78-83% expert agreement benchmark)
- Gap 2: Multi-Agent Evaluator Simulation (CollabPersona IEEE 2024 r=0.73,
  MAJ-EVAL OpenReview 2025 +8.3% correctness; LangGraph/AutoGen/CrewAI)
- Gap 3: Embedding-Based Semantic Operations (UAE-Large-V1 83.5 MTEB,
  FAISS HNSW, Qdrant; 6 operations specified)
- Gap 4: Pareto Frontier Termination (NSGA-II, Elite MCDM-Pareto MDPI
  Algorithms 17(5):206; 34% Condorcet reduction)
- Gap 5: Dirichlet Simplex-Constrained Monte Carlo (15-25% wider confidence
  intervals than independent perturbation; MDPI Mathematics 14(8):1243)
- Gap 6: Automated Contradiction Detection (ALICE Springer 2024 82% precision;
  negated-embedding method 60-70% recall)
- Gap 7: Manipulation Detection (EJOR 2023 23% manipulation rate; arXiv
  2405.16693 78% neural detection accuracy)
- Gap 8: Online Domain Similarity Learning (MLSD SBP-BRiMS 2025 89% accuracy;
  ADWIN drift detection; Siamese networks ACCV 2022)
- Gap 9: Human-AI Task Partitioning (HITL/HOTL Springer 2024; ACM review:
  +8-23% on quantitative tasks, -11-18% on value-laden tasks)
- Gap 10: LLM UQ via Monte Carlo Temperature (Cecere et al. Amazon arXiv:
  2502.18389 EMNLP TrustNLP 2025; statistical parity with oracle temperature)

RESEARCH MANDATE:
Extend the knowledge base into areas NOT covered by either layer above. Do not
re-investigate topics already addressed in Gaps 1-10 or the existing KB.
Prioritize empirical evidence, implementation-grade specificity, and findings
that directly affect the architectural decisions described in the gaps report.
</context>

<task>
Using the deep-research-pro skill workflow, conduct original multi-source research
into the following 8 unexplored extension areas. For each area, execute at least
3 distinct search queries (web + news), deep-read 2-3 key sources in full, and
synthesize findings at implementation-specification depth.

EXTENSION AREA 1 — END-TO-END EMPIRICAL BENCHMARKING OF AI-ASSISTED MCDA:
Controlled studies comparing AI-assisted Pugh Matrix or MCDA decision quality
against unaided human performance. Seek: accuracy metrics (correct alternative
selected vs ground truth), bias reduction measurements, failure rates, and
conditions under which AI assistance helps vs. hurts. Target journals: Decision
Support Systems, EJOR, IEEE Transactions on Systems Man and Cybernetics.

EXTENSION AREA 2 — DOMAIN-SPECIFIC LLM FINE-TUNING FOR STRUCTURED EVALUATION:
Fine-tuned models specialized for structured judgment, criterion scoring, or
comparative evaluation tasks (beyond general instruction tuning). Seek: training
dataset construction methods for evaluation tasks, RLHF/DPO applied to scoring
consistency, models fine-tuned on decision data. Compare against Gap 1's
general-LLM approach (AutoRubric) — does fine-tuning close the remaining 17-22%
gap in expert agreement?

EXTENSION AREA 3 — UNCERTAINTY PROPAGATION ACROSS MULTI-STEP EVALUATION PIPELINES:
Formal methods for combining per-criterion scoring confidence (from Gap 10's
Monte Carlo Temperature) into aggregate ranking confidence. Seek: error
accumulation in sequential evaluation steps, interval arithmetic for
decision-making, Bayesian uncertainty propagation in MCDA, and whether
confidence at the ranking level is a reliable signal for human escalation.

EXTENSION AREA 4 — FAILURE CASES OF DEPLOYED AI DECISION SUPPORT SYSTEMS:
Published post-mortems, incident reports, and case studies of AI decision
support tools that failed in organizational deployment. Seek: root causes
(automation bias, model drift, distributional shift, user rejection, trust
miscalibration), what would have detected or prevented each failure, and
whether failure patterns differ between high-stakes (medical, legal) and
moderate-stakes (procurement, architecture) decision contexts.

EXTENSION AREA 5 — INTERACTIVE AND INCREMENTAL PREFERENCE ELICITATION IN MCDA:
Methods where the system elicits preference information progressively during
decision-making rather than requiring complete weight vectors upfront. Seek:
preference-based multi-criteria linear programming (PBMCLP), interactive
evolutionary multi-objective algorithms, preference learning from pairwise
comparisons (SPARTAN, NAUTILUS, NIMBUS frameworks), and how progressive
elicitation reduces the cognitive burden that Gap 9 identifies as a core problem
in weight-setting.

EXTENSION AREA 6 — EXPLAINABILITY AND AUDIT TRAILS FOR AI-ASSISTED DECISIONS:
How AI-generated Pugh Matrix scores and recommendations can be explained to
non-technical stakeholders and made auditable for organizational accountability.
Seek: SHAP/LIME applied to MCDA outputs, counterfactual explanations for
ranking changes, regulatory requirements for explainable AI in organizational
decision-making (EU AI Act implications), and audit trail standards. Gap 9 notes
that communication to stakeholders is human-only — explainability tools may
shift this boundary.

EXTENSION AREA 7 — COGNITIVE SCIENCE OF AI-ASSISTED MULTI-CRITERIA DECISION MAKING:
Experimental psychology and cognitive science research on how AI recommendations
affect human multi-criteria judgment. Seek: anchoring effects from AI-generated
scores, cognitive load changes when criteria are many (>15), satisficing vs.
optimizing behavior shifts when AI provides rankings, and dual-process theory
(System 1/System 2) implications for human override of AI recommendations.
Gap 9 uses empirical MCDA-AI studies; this extends into the cognitive mechanisms.

EXTENSION AREA 8 — FORMAL CONVERGENCE ANALYSIS OF PUGH ITERATIVE HYBRID SYNTHESIS:
Mathematical analysis of whether Pugh Controlled Convergence is guaranteed to
terminate, under what conditions it can cycle (oscillate between two hybrid
solutions), and whether AI-driven systematic synthesis (exhaustive combination
search, as documented in Area C) changes the convergence properties. Seek:
fixed-point theory applied to iterative design refinement, convergence proofs
in combinatorial optimization with discrete scores, and known pathological cases
where iterative synthesis diverges or cycles.
</task>

<constraints>
DO:
- Follow the deep-research-pro skill workflow in full:
  understand → plan sub-questions → multi-source DDG search → deep-read
  key sources → synthesize → save report → deliver summary
- Execute a minimum of 3 distinct search queries per extension area
- Deep-read at least 2 sources per area from full page content (not just snippets)
- Apply evidence quality labels: CONFIRMED (≥2 independent sources),
  SINGLE-SOURCE (one source only), INSUFFICIENT DATA (not found after 3+ searches)
- Connect every finding to at least one specific existing gap (Gaps 1-10) or
  KB area (Areas A-F, G-1 through G-7)
- Flag at least one known failure mode per extension area or explain why none found
- Preserve source URLs verbatim in citations

DO NOT:
- Re-investigate topics already covered in Gaps 1-10 or the Layer 1 KB
- Count surface-level blog posts as primary evidence without corroboration
- Fabricate benchmarks, accuracy figures, or result statistics
- Use "AI could..." framing without specific supporting evidence
- Skip extension areas silently — if insufficient data found, report that explicitly
- Conflate adjacent topics (e.g., general XAI is not MCDA-specific explainability)
</constraints>

<output_format>
Save report to: ~/clawd/research/pugh-matrix-ai-extension/report.md

Structure the report as:

## Executive Summary (400 words max)
Three highest-impact new findings and how each changes or extends the existing
implementation roadmap from the gaps report.

## Extension Area Reports (one section per area, 1-8)
Each section must contain:
- **New findings**: Specific discoveries NOT in existing KB
- **Key sources**: Full URLs or DOIs, one sentence per source
- **Integration point**: Which specific gap(s) or area(s) this connects to
- **Evidence quality**: CONFIRMED / SINGLE-SOURCE / INSUFFICIENT DATA
- **Failure mode**: At least one failure case identified or explanation if none found

## Revised Implementation Roadmap
Updated priority table incorporating new findings. Preserve the existing 10-row
table from the gaps report and add or reorder rows based on new evidence.

## Open Research Questions
Questions this investigation could not answer — specific sub-questions for a
future research pass, with suggested search strategies.

## Sources
Full citation list formatted as:
[N]. [Title](URL) — [one-sentence summary] | Evidence: [CONFIRMED/SINGLE-SOURCE]
</output_format>

<edge_cases>
- If an extension area overlaps with existing KB content, document only the
  delta (what is genuinely new) and note the overlap explicitly
- If a source is behind a paywall, extract maximum content from abstract,
  preview text, and citing papers; label the finding as SINGLE-SOURCE
- If fewer than 3 reliable sources exist for an area, report INSUFFICIENT DATA
  rather than inflating with low-quality sources
- If a finding contradicts the existing report (e.g., new empirical evidence
  challenges Gap 5's Dirichlet claim), flag it as CONFLICT and present both
  sides with evidence quality labels
</edge_cases>

<verification>
Before finalizing:
1. All 8 extension areas are covered (no silent omissions)
2. Every factual claim has at least one cited URL
3. No finding duplicates content from Gaps 1-10 or the Layer 1 KB
4. Evidence quality labels are applied to every finding
5. Each extension area connects to at least one specific existing gap or area
6. The revised roadmap table is complete and internally consistent
7. File saved to ~/clawd/research/pugh-matrix-ai-extension/report.md
</verification>
```
