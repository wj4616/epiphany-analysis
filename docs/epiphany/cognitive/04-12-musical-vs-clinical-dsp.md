> v1.2.0 · Scale: STANDARD · Pipeline: complete at 6 of 6 planned stages · For raw state, re-invoke with --xml

## Headline Insight

DSP algorithms sound "musical" when their processing shares three structural properties with natural acoustic phenomena: **soft non-linearity** (which generates harmonics the auditory system — itself non-linear — is optimized to process), **program-dependent temporal behavior** (envelopes that vary with the signal rather than following fixed mathematical curves), and **minimum-phase character** (transient timing that matches physical acoustic systems). "Clinical" algorithms maximize mathematical precision at the cost of these properties. The underlying mechanism: the auditory system evolved for natural complex sounds, and processing that mirrors the statistical signature of those sounds "feels right."

**Confidence:** MODERATE — strong theoretical grounding and practitioner consensus, but cultural entrainment and blind-test weaknesses are genuine confounds.

---

## Theory Collisions

*(Stage S4 skipped at STANDARD scale — run DEEP for full dynamic simulation output.)*

**From S5/S6 precision and falsification work, two collisions emerged:**

1. "Non-linearity causes musicality" collides with "harsh digital clipping is non-linear and sounds terrible."
   - Discriminating condition: *degree and type* of non-linearity matters, not its mere presence. Soft saturation (tanh-like) vs. hard clipping. Simple "more non-linearity = more musical" is falsified.

2. "The musical preference is biological (auditory evolution)" collides with "the preference is culturally learned (analog-era entrainment)."
   - Discriminating condition: listeners raised exclusively on digital productions with no analog reference. If they still prefer soft saturation, biological > cultural. If not, cultural entrainment explains most of the effect.

---

## Discovery vs. Proof

| Discovered | Proved |
|---|---|
| Temporal naturalness as a key variable: program-dependent envelopes have statistical properties resembling natural sound trajectories | Non-linear auditory physiology: cochlear mechanics and hair cell transduction are compressive and non-linear (established biology) |
| The biological-resonance pattern — non-linear evaluator prefers non-linear processor — generalizes beyond audio (film grain, physical modeling, instrument acoustics) | Minimum-phase filters preserve transient attack character; linear-phase filters produce pre-ringing that has no analog in natural acoustics |
| The gastronomy structural analogy: Maillard/caramelization (non-linear chemistry) → preferred by taste receptors (non-linear biology); maps to DSP (non-linear processing) → preferred by auditory system (non-linear biology) | Soft non-linearity adds harmonics at integer multiples; hard non-linearity creates spectral noise and intermodulation products above a threshold |

---

## Independence-Verified Bridges

**Food science → audio DSP**
- Source: gastronomy (S2 domain switch)
- Structural invariant: *non-linear evaluator ↔ non-linear processor resonance*. A biological system optimized to interpret complex, chemically transformed inputs will prefer complex, non-linearly transformed outputs over clean linear transformations.
- Disanalogy limit: taste evaluation has no temporal or phase dimension — the bridge captures harmonic enrichment but doesn't illuminate the program-dependent envelope component.

**Visual domain → audio DSP**
- Source: film grain vs. CGI motion (S3 peripheral exploration, independent of food domain)
- Structural invariant: biological evaluators detect "natural-variance signatures" and mark their absence as wrong. CGI motion before motion blur was added looked inhuman despite being mathematically smooth. Linear-phase EQ pre-ringing is the audio equivalent — mathematically perfect, perceptually foreign.
- Independence verified: visual and auditory processing are genuinely independent systems. Both pointing toward the same pattern is a real convergence signal.

---

## Alternative Hypotheses

Three live hypotheses — none eliminated:

1. **Cultural entrainment hypothesis**: "Musical" processing is what engineers learned to associate with quality from analog recordings. The preference is primarily learned, not biological. *Discriminating observation:* engineers raised on digital-only productions assessing the same material. Predicted outcome if true: no reliable preference for non-linear processing.

2. **Gain-staging confound hypothesis**: "Musical" processors provide better level consistency, dynamic compression side-effects, and mix compatibility — these functional benefits are what practitioners actually prefer, not the harmonic or phase properties. *Discriminating observation:* equalize output level carefully in ABX tests across multiple musical contexts; if preference persists with level-matched material, this hypothesis weakens.

3. **Statistical complexity-matching hypothesis**: The auditory system expects outputs with specific amplitude distribution properties (1/f-like spectra, log-normal dynamics). "Musical" algorithms happen to produce this profile; "clinical" algorithms produce flatter or statistically different distributions. *Discriminating observation:* measure amplitude distribution statistics of confirmed "musical" vs. "clinical" algorithm outputs; musical ones should cluster toward natural-sound statistics.

---

## Density-Checked Falsification

**Vectors tried (6):**
1. Harsh digital clipping — non-linear, not musical → falsifies "non-linearity = sufficient condition" ✓ landed
2. Convolution reverb of real halls — linear, yet can sound musical → falsifies "linearity = clinical" ✓ landed
3. Cultural change over decades (early CDs called "clinical," now preferred by some genres) → alive, weakens biological-only explanation
4. Individual variation among engineers → alive, weakens universal claims
5. Blind test weakness (effect sizes smaller than practitioner reports suggest) → alive, moderate concern
6. Gain staging confound (Alternative Hypothesis 2 above) → alive, not ruled out

**Survived:** Temporal naturalness claim, minimum-phase character claim, soft-non-linearity specificity claim.

**Failed:** Simple "more non-linearity = more musical" — definitively killed by harsh clipping case.

`2 falsification vectors applied to temporal naturalness claim specifically — density low — residual uncertainty moderate`

---

## Scope Limits (hypotheses non fingo)

**Applies to:** Perceptual assessment of audio DSP algorithms by experienced listeners in Western music production contexts, specifically the practitioner-level distinction between "musical" and "clinical" processing character.

**Does not extend to:** Scientific claims about auditory neuroscience mechanisms (which remain partially speculative), cross-cultural universality of the musical/clinical distinction (evidence is predominantly Western-professional), objective measurement protocols that reliably predict the musical/clinical percept without human listeners.

**Claims refused:** That non-linearity is a sufficient condition for musicality (falsified by harsh clipping), that temporal naturalness is fully operationalized (it is named but not precisely measured), that the distinction is purely biological vs. purely cultural (both are real; the proportion is unknown).

---

## Coherence Signals

The non-linear-evaluator ↔ non-linear-process resonance pattern emerged independently from four locations:
- S2 domain shift (gastronomy / Maillard reaction)
- S3 peripheral exploration (visual domain / film grain)
- S5 self-reference (reasoning pipelines that include non-linear association feel more "insightful" than purely deductive ones)
- Generalization check (acoustic instruments, physical modeling synthesis)

**Reported as supporting evidence, not proof.** The pattern is genuinely structural — preserved at the level of systems, not surface vocabulary — but structural coherence can still be wrong.

---

## Generalization Checks

**General form:** A biological evaluator optimized for natural-process outputs prefers outputs from natural-process-like algorithms over mathematically optimal algorithms.

**Holds for:** Film grain vs. digital sensor noise · Physical modeling synthesis vs. sample playback · Acoustic instruments vs. MIDI mockups · Tube/tape processing vs. purely digital processing

**Breaks at a specific edge case:** When the "natural" signal has unwanted properties (noise floor, dynamic range problems, room resonances) that optimal processing corrects — and when the listener's reference expectation has been calibrated to *produced recordings* as the new "natural." In this case, a cleanly processed digital signal sounds "more natural" than the raw acoustic capture. The generalization breaks when the baseline expectation shifts.

**Implication for DSP design:** The question isn't "add non-linearity" — it's "match the statistical signature of the listener's calibrated reference." For genres calibrated to digital productions, "musical" may mean something different than for genres calibrated to analog-era recordings.

---

## Open Questions & Next Probes

Ranked by discriminating power per unit effort:

1. **(Highest)** Controlled blind ABX test with listeners raised on digital-only music (born post-2000, minimal analog exposure), assessing matched-level non-linear vs. linear processing. Would cleanly separate cultural entrainment from biological preference.

2. **(High)** Statistical analysis: measure amplitude distribution and autocorrelation structure of confirmed "musical" algorithm outputs (LA-2A emulation, tape saturation) vs. confirmed "clinical" outputs (linear-phase brickwall limiter, surgical FFT EQ). Do they cluster toward different statistical profiles? Would operationalize "temporal naturalness."

3. **(Medium)** Build a linear algorithm that mimics the temporal envelope statistics of a known non-linear one (same release curve shape, same gain trajectory), then run listening tests. If it sounds musical, temporal structure alone is sufficient and non-linearity is instrumental. If it sounds clinical, non-linearity is load-bearing beyond its envelope effects.

4. **(Medium)** Cross-cultural study: non-Western listeners without significant analog music exposure. Would test whether the "musical" category is universal or culturally constructed.

5. **(Lower, but mechanistic)** Formalize the "soft non-linearity threshold": at what THD percentage and harmonic distribution does saturation transition from "warmth" to "harshness"? This threshold is discussed in practitioner knowledge but not rigorously measured.
