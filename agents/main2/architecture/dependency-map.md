# Phase Dependency Map

## Dependency Types

| Type | Symbol | Meaning |
|------|--------|---------|
| **Hard** | → | Must complete before next phase starts |
| **Soft** | ⇢ | Should complete, can proceed without |
| **Parallel** | ‖ | Can run simultaneously |

## Phase Dependencies

```
[0] Commission
      ↓ (hard)
[1] Research ────────┐
      ↓ (hard)        │
[2] Specification    │ (soft: KB provides support)
      ↓ (hard)        │
[3] Architecture ←──┘
      ↓ (hard)
[4] Project Setup
      ↓ (hard)
[5] DSP Implementation
      ↓ (hard)
[6] State & Presets
      ↓ (hard)
[7] Processor Integration
      ↓ (hard)
[8] GUI Implementation ───┐
      ↓ (hard)             │ (parallel possible)
[9] Build Verification ←───┘
      ↓ (hard)
[10] DAW Testing ───────────┐
      ↓ (hard)              │ (may return to 5, 7, 8)
[11] Licensing Integration ←┘
      ↓ (hard)
[12] Optimization
      ↓ (hard)
[13] Marketplace Preparation ──┐
      ↓ (hard)                  │ (parallel possible)
[14] Marketing Strategy ───────┘
      ↓ (hard)
[15] Release
      ↓ (hard)
[16] Retrospective
```

## Parallel Opportunities

| Phases | Parallel? | Rationale |
|--------|-----------|-----------|
| 0 + 1 | No | Research needs scope |
| 2 + 3 | No | Architecture needs spec |
| 3 + 4 | No | Setup needs architecture |
| 8 + 9 | Partial | Build can start while GUI finishing |
| 11 + 13 | Yes | Licensing and marketplace prep are independent |
| 13 + 14 | Yes | Marketplace and marketing can run together |

## Hard Dependencies

| From | To | Reason |
|------|-----|-----|
| 0 → 1 | Commission defines research scope |
| 1 → 2 | Research informs specification |
| 2 → 3 | Spec defines architecture |
| 3 → 4 | Architecture defines project setup |
| 4 → 5 | Setup required before implementation |
| 5 → 6 | DSP modules needed for state integration |
| 6 → 7 | Presets needed for processor wiring |
| 7 → 8 | Parameters needed for GUI |
| 9 → 10 | Build must pass before DAW testing |
| 10 → 11 | Sound must be approved before licensing |
| 11 → 12 | Licensing must work before optimization |
| 12 → 13 | Final binary before marketplace |
| 15 → 16 | Release before retrospective |

## Soft Dependencies

| From | To | Reason |
|------|-----|-----|
| 1 → 5 | KB supports implementation, but spec is sufficient |
| 1 → 11 | KB supports licensing, but licensing system docs suffice |
| 3 → 5 | Architecture guides, but DSP can start with spec |

## Return Paths (Re-entrancy)

| From | Back To | Trigger |
|------|---------|---------|
| 10 → 5 | DAW testing finds DSP bug | Silent parameter, wrong formula |
| 10 → 7 | DAW testing finds integration bug | SmoothedValue not advancing |
| 10 → 8 | DAW testing finds GUI bug | Control not visible, wrong bounds |
| 9 → 4 | Build fails on configuration | CMake issue |
| 12 → 5 | Optimization changes DSP | Sound changes after optimization |

## Task Dependencies Within Phases

### Phase 5: DSP Implementation

```
5.1 Create DSP module interface
5.2 Implement module A (depends on 5.1)
5.3 Implement module B (depends on 5.1)
5.4 Two-stage review module A (depends on 5.2)
5.5 Two-stage review module B (depends on 5.3)
5.6 Integration test (depends on 5.4, 5.5)
```

### Phase 6: State & Presets

```
6.1 Implement state save/restore (no dependencies)
6.2 Implement PresetData struct (no dependencies)
6.3 Implement PresetManager (depends on 6.2)
6.4 Test state round-trip (depends on 6.1, 6.3)
```

### Phase 11: Licensing Integration

```
11.1 Choose licensing system (no dependencies)
11.2 Integrate licensing SDK (depends on 11.1)
11.3 Implement trial logic (depends on 11.2)
11.4 Test licensing flow (depends on 11.3)
```

## Dependency Graph Visualization

```
                    ┌──────────────────────────────────────────────────┐
                    │                   KB Layer                        │
                    │  (Technical, Sound Design, UI/UX, Commercial)   │
                    └────────────┬─────────────────────────────────────┘
                                 │
                                 ▼
    ┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐
    │ Phase 0 │ ──▶ │ Phase 1 │ ──▶ │ Phase 2 │ ──▶ │ Phase 3 │
    │Commission│     │Research │     │  Spec   │     │  Arch   │
    └─────────┘     └─────────┘     └─────────┘     └─────────┘
                                                       │
                                                       ▼
    ┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐
    │ Phase 4 │ ──▶ │ Phase 5 │ ──▶ │ Phase 6 │ ──▶ │ Phase 7 │
    │  Setup  │     │   DSP   │     │  State  │     │ Proc Int│
    └─────────┘     └─────────┘     └─────────┘     └─────────┘
                                                       │
                                                       ▼
    ┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐
    │ Phase 8 │ ──▶ │ Phase 9 │ ──▶ │Phase 10 │ ──▶ │Phase 11 │
    │   GUI   │     │  Build  │     │  DAW    │     │License  │
    └─────────┘     └─────────┘     └─────────┘     └─────────┘
        │               │               │ (return)      │
        │               │               └──────┐        │
        │               │                      ▼        │
        │           ┌───┴────────────────────────────┐  │
        │           │      Return to 5, 7, or 8     │  │
        │           │      if DAW testing fails     │  │
        │           └───────────────────────────────┘  │
        │                                              │
        ▼                                              ▼
    ┌─────────┐     ┌─────────┐     ┌─────────┐     ┌─────────┐
    │Phase 12 │ ──▶ │Phase 13 │ ──▶│Phase 14 │ ──▶ │Phase 15 │
    │  Opt    │     │ Market  │     │ Market  │     │ Release │
    └─────────┘     └─────────┘     └─────────┘     └─────────┘
                                                        │
                                                        ▼
                                                   ┌─────────┐
                                                   │Phase 16 │
                                                   │ Retro   │
                                                   └─────────┘
```