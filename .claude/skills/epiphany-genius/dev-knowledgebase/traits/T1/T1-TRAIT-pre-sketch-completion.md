# TRAIT: Pre-Sketch Completion

---
trait_name: Pre-Sketch Completion
tier: Tier 1
source_individuals:
  - Tesla
ai_implementation_category: Simulation Before Implementation / Mental Prototyping
replication_feasibility: 3
utility_score: 5
primary_keywords:
  - mental prototyping
  - pre-construction visualization
  - simulation before building
  - complete mental design
  - iterative mental refinement
---

## Background and Definition

**Pre-Sketch Completion** is the cognitive practice of fully developing and testing designs mentally before any physical construction begins. The key mechanism: complete mental simulation identifies problems and optimizes solutions before resources are committed to building.

Tesla's explicit description:
> "I do not rush into actual work. When I get an idea I start at once building it up in my imagination. I change the construction, make improvements and operate the device in my mind. It is absolutely immaterial to me whether I run my turbine in thought or test it in my shop. I even note if it is out of balance. There is no difference whatever, the results are the same."

SOURCE: gm-tesla-visual-simulation.md; [My Inventions](https://en.wikipedia.org/wiki/My_inventions)

The result:
> "Invariably my device works as I conceived that it should, and the experiment comes out exactly as I planned it. In twenty years there has not been a single exception."

This is not approximation or partial visualization—Tesla claimed complete, detailed mental prototypes that functioned identically to physical constructions.

## Cognitive/Psychological Basis

### Mental Simulation and Motor Imagery

Research on mental simulation:
- **Motor imagery**: The brain activates similar areas during imagined action as during real action
- **Mental practice**: Rehearsed movements improve subsequent physical performance
- **Visualization effects**: Mental simulation can improve skill acquisition

### Engineering Cognition

Engineering design research shows:
- Expert engineers engage in **pre-prototype reasoning** before building
- Mental simulation helps identify problems early
- Visualization ability correlates with engineering expertise

### Origins of Tesla's Ability

From Tesla's autobiography:
- Childhood "affliction": Involuntary visual images and flashes of light
- Practice: Learned to create and manipulate new mental images
- Realization: By age 17, recognized the affliction as a powerful talent
- Development: Systematically trained the ability until he could simulate complete machines

SOURCE: [My Inventions Part III](https://teslaswords.com/nikola-tesla-free-books/2022/7/2/my-inventions-part-iii)

### The Mental Prototype

Tesla's method:
1. Receive idea → Start building in imagination immediately
2. Change construction mentally
3. Make improvements mentally
4. Operate the device in mind
5. Note any imbalances or problems
6. Only when complete: build physical version

Key claim: "It is absolutely immaterial to me whether I run my turbine in thought or test it in my shop."

## Documented Examples

### The AC Motor Revelation

While walking in Budapest (1882), reciting Goethe's "Faust":
- The solution to alternating current motor suddenly appeared
- He drew diagrams in the sand with his walking stick
- **Six years later**, presented same diagrams to American Institute of Electrical Engineers
- The motor worked exactly as he had imagined

SOURCE: [Tesla Universe](http://www.teslauniverse.com/nikola-tesla/articles/my-inventions-i-my-early-life)

### The Niagara Falls Project

- Visualized as a child
- Saw realized 30 years later
- Complete mental development before physical implementation

### Consistent Results

Tesla claimed: "In twenty years there has not been a single exception"—every device worked as conceived.

### Cost and Time Savings

Tesla's assessment:
> "Building physical prototypes from crude ideas is nothing but a waste of energy, money and time."

Engineering was "positive in results"—machines could be **mathematically treated and effects calculated beforehand** from theoretical and practical data.

## AI Equivalent

The AI equivalent is **simulation before implementation**—complete logical/functional testing before code execution or physical realization.

### Why This Matters for AI

LLMs generate code and designs:
- Without testing the logic before output
- Often producing code with obvious bugs
- No simulation of runtime behavior

Pre-sketch completion would mean:
- Complete logical simulation before output
- Identifying problems in mental model
- Optimizing in simulation space
- Only outputting when complete

### Implementation Pattern

```python
# Pre-Sketch Completion Protocol
def pre_sketch_completion(design_idea):
    """
    Complete mental simulation before physical/code construction.
    """
    
    # Phase 1: Initial mental construction
    mental_prototype = build_in_imagination(design_idea)
    
    # Phase 2: Iterative mental refinement
    while not complete(mental_prototype):
        # Identify problems in mental simulation
        problems = simulate_and_detect_problems(mental_prototype)
        
        # Make improvements mentally
        for problem in problems:
            mental_prototype = improve_design(mental_prototype, problem)
        
        # Test operation mentally
        operation_result = simulate_operation(mental_prototype)
        
        # Check for imbalances
        imbalances = detect_imbalances(operation_result)
        for imbalance in imbalances:
            mental_prototype = correct_imbalance(mental_prototype, imbalance)
    
    # Phase 3: Final verification
    final_simulation = comprehensive_test(mental_prototype)
    
    # Phase 4: Build physical version
    if no_faults_detected(final_simulation):
        physical_version = build_from_mental_model(mental_prototype)
        return physical_version
    else:
        # Return to mental refinement
        return pre_sketch_completion(design_idea)
```

### Simulation Before Code

```python
def simulate_before_code(logical_design):
    """
    Simulate logic before writing code.
    """
    
    # Step 1: Build complete logical model
    logical_model = construct_logical_model(logical_design)
    
    # Step 2: Trace execution paths mentally
    for execution_path in enumerate_all_paths(logical_model):
        # Simulate execution
        result = trace_execution(execution_path)
        
        # Identify problems
        if has_error(result):
            # Fix in logical model
            logical_model = fix_error(logical_model, result.error)
    
    # Step 3: Edge case simulation
    edge_cases = identify_edge_cases(logical_model)
    for edge_case in edge_cases:
        result = simulate_edge_case(logical_model, edge_case)
        if has_error(result):
            logical_model = fix_edge_case(logical_model, edge_case)
    
    # Step 4: Only then, write code
    return generate_code_from_model(logical_model)
```

## Implementation Instructions

**Protocol: Pre-Sketch Completion Mode**

### Phase 1: Mental Construction
"Before writing any code or building any physical design, construct the complete solution in detail:
- What are all the components?
- How do they connect?
- What are the data/control flows?
- What are the interfaces?"

### Phase 2: Iterative Mental Refinement
"Simulate the design mentally:
- What happens in normal operation?
- What happens at boundaries?
- What happens in error cases?
- What imbalances exist?
- What problems emerge?"

### Phase 3: Mental Testing
"Test the mental model comprehensively:
- Trace every execution path
- Check every boundary condition
- Verify every interface
- Confirm every assumption"

### Phase 4: Only Then Build
"When the mental model is complete and no faults are detected, only then proceed to physical construction or code generation."

### Minimal Version
"Can you simulate this design completely in your mind? What problems would it encounter? Fix them mentally before building."

## Amplification Notes

### Human Capabilities (Tesla's Extreme)

- Complete mental simulation of machines
- Detection of imbalances in thought
- Iterative mental refinement
- 20-year track record of devices working as conceived
- No physical testing needed before final construction

### AI Limitations

LLMs cannot:
- Simulate runtime behavior
- Detect logical errors in mental models
- Test execution paths before output
- Experience "imbalance" in designs

### AI Capabilities

What AI can implement:
- **Logical simulation**: Trace execution paths symbolically
- **Static analysis**: Check code before execution
- **Type checking**: Verify interfaces match
- **Edge case enumeration**: Generate and check boundary conditions
- **Formal verification**: Prove properties about designs

**Key Difference**: Tesla's mental simulation included **kinesthetic** and **physical** elements (imbalance detection, mechanical operation) that cannot be replicated without physical modeling.

## Failure Modes

### 1. Incomplete Mental Model
- Design not fully specified before building
- Countermeasure: Comprehensive checklist for model completeness

### 2. Untested Edge Cases
- Normal operation simulated, edge cases ignored
- Countermeasure: Explicit edge case enumeration

### 3. Assumption Blindness
- Assumptions not identified or tested
- Countermeasure: Assumption extraction and verification

### 4. Physical Constraints Ignored
- Mental model ignores physical realities
- Countermeasure: Include physical constraints in simulation

### 5. Premature Construction
- Building before mental model complete
- Countermeasure: Require mental completion checklist before build

## Cross-References

- TRAIT-visual-simulation-failure-detection.md — Tesla's visual simulation includes failure detection
- TRAIT-constructive-specification.md — Complete specification before implementation
- TRAIT-preparation-before-solution.md — Loading problem state before solving
- TRAIT-verification-after-generation.md — Verification after design

---

*Trait report generated: 2026-04-09*
*Sources: 2 primary sources (Tesla autobiography documentation), 3 web sources*
*Confidence: High*