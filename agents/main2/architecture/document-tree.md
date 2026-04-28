# Deliverable Document Tree

## Directory Structure

```
project-root/
├── README.md                      # Project overview (Phase 0)
├── scope.md                       # Scope boundaries (Phase 0)
├── constraints.md                 # Technical constraints (Phase 0)
├── success-criteria.md            # Measurable success (Phase 0)
├── decisions-ledger.md            # Decision rationale (Phase 0+)
├── artifact-manifest.md           # File index (Phase 0+)
├── metrics-tracker.md             # KPI tracking (Phase 0+)
│
├── research/                      # Phase 1 outputs
│   ├── domain-analysis.md
│   ├── best-practices.md
│   ├── competitive-templates.md
│   ├── cross-cutting-concerns.md
│   ├── platform-concerns.md
│   ├── requirements.md
│   └── sme-interviews.md
│
├── architecture/                  # Phase 2-3 outputs
│   ├── kb-architecture.md
│   ├── entry-schema.json
│   ├── bridge-schema.json
│   ├── population-strategy.md
│   ├── directory-structure.md
│   ├── phase-structure.md
│   ├── task-granularity.md
│   ├── dependency-map.md
│   ├── phase-gates.md
│   └── document-tree.md
│
├── kb/                            # Phase 3 outputs
│   ├── master-index.json
│   ├── technical-kb/
│   ├── sound-design-kb/
│   ├── ui-ux-kb/
│   ├── commercial-kb/
│   ├── reference-kb/
│   └── failure-modes-kb/
│
├── docs/                          # Phase 2 outputs
│   └── SPEC.md                    # Plugin specification
│
├── src/                           # Phase 4+ outputs
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   ├── PluginEditor.cpp
│   └── DSP/                       # DSP modules (Phase 5)
│       ├── ModuleA.h
│       ├── ModuleA.cpp
│       └── ...
│
├── tests/                         # Phase 4+ outputs
│   ├── CMakeLists.txt
│   ├── ModuleATest.cpp
│   └── ...
│
├── presets/                       # Phase 6 outputs
│   └── PresetData.h
│
├── marketing/                     # Phase 14 outputs
│   ├── strategy.md
│   ├── product-page.md
│   └── screenshots/
│
├── retrospective/                 # Phase 16 outputs
│   └── retrospective.md
│
├── CMakeLists.txt                 # Phase 4 output
├── build/                         # Build directory
└── .vst3/                         # Installed plugin (after build)
```

## Document Naming Conventions

| Type | Convention | Example |
|------|-----------|---------|
| Phase output | lowercase-with-dashes.md | `domain-analysis.md` |
| Source code | PascalCase.h/cpp | `PluginProcessor.cpp` |
| Test file | PascalCaseTest.cpp | `FilterModuleTest.cpp` |
| JSON | lowercase-with-dashes.json | `apvts-setup.json` |
| Bridge entry | bridge-{descriptor}.json | `bridge-warm.json` |
| Failure mode | fm-{id}-{name}.json | `fm-01-allocations.json` |

## Document Flow Between Phases

| From Phase | To Phase | Documents |
|------------|----------|-----------|
| 0 → 1 | Commission brief, scope, constraints |
| 1 → 2 | Research outputs, requirements, KB |
| 2 → 3 | SPEC.md, parameter list |
| 3 → 4 | Architecture, task list |
| 4 → 5 | Build system, project skeleton |
| 5 → 6 | DSP modules, tests |
| 6 → 7 | Presets, state management |
| 7 → 8 | Processor integration |
| 8 → 9 | GUI implementation |
| 9 → 10 | Build artifacts |
| 10 → 11 | Approved sound, bug fixes |
| 11 → 12 | Licensed binary |
| 12 → 13 | Optimized binary |
| 13 → 14 | Marketplace content |
| 14 → 15 | Marketing materials |
| 15 → 16 | Released binary |
| 16 → 0 | Playbook updates (for next project) |

## Persistent Files

These files are loaded at every phase and updated at every gate:

| File | Purpose |
|------|---------|
| decisions-ledger.md | Why decisions were made |
| artifact-manifest.md | Where files are located |
| metrics-tracker.md | KPI measurements |

## Task-to-Document Mapping

### Phase 0
| Task | Produces |
|------|----------|
| Create project folder | Directory structure |
| Define purpose | README.md |
| Define scope | scope.md |
| Define constraints | constraints.md |
| Define success criteria | success-criteria.md |
| Classify complexity | decisions-ledger.md entry |

### Phase 1
| Task | Produces |
|------|----------|
| Research domain | research/domain-analysis.md |
| Research best practices | research/best-practices.md |
| Audit competitive | research/competitive-templates.md |
| Identify concerns | research/cross-cutting-concerns.md |
| Synthesize requirements | research/requirements.md |

### Phase 2
| Task | Produces |
|------|----------|
| Define KB architecture | architecture/kb-architecture.md |
| Define entry schema | architecture/entry-schema.json |
| Define bridge schema | architecture/bridge-schema.json |
| Define population strategy | architecture/population-strategy.md |
| Define directory structure | architecture/directory-structure.md |

### Phase 3
| Task | Produces |
|------|----------|
| Create KB tree | kb/ directory |
| Seed placeholders | Placeholder entries |
| Harvest content | KB entries |
| Create bridges | Bridge entries |

### Phase 4
| Task | Produces |
|------|----------|
| Define phase structure | architecture/phase-structure.md |
| Define task granularity | architecture/task-granularity.md |
| Map dependencies | architecture/dependency-map.md |
| Design gates | architecture/phase-gates.md |
| Define document tree | architecture/document-tree.md |

## Archive Structure

Superseded files are moved to `_archive/` subdirectories:

```
kb/technical-kb/juce-patterns/_archive/
    apvts-setup_0.1.0.json   # Superseded version
```

## Deliverable Verification

Each gate verifies:
1. **Existence**: File exists at expected path
2. **Content**: File has required sections
3. **Schema**: JSON files validate against schema
4. **References**: Cross-references resolve correctly