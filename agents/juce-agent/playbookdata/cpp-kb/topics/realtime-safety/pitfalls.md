# Real-Time Audio Pitfalls

## Why This Matters

Real-time audio programming violates many common C++ intuitions. Code that works perfectly in normal applications can cause glitches, crashes, or undefined behavior in audio plugins. Understanding these pitfalls prevents subtle bugs that only appear under specific DAW/host conditions.

## When to Use

Reference this guide during code review, when auditing `processBlock()`, or investigating audio glitches.

## Common Pitfalls

### 1. Hidden Allocations

**Problem:** Standard library containers allocate on the heap.

```cpp
// WRONG: Allocation in processBlock
void processBlock(juce::AudioBuffer<float>& buffer) override
{
    std::vector<float> temp(buffer.getNumSamples());  // ALLOCATES!
    // ...
}

// RIGHT: Pre-allocate as member
class Processor
{
    std::vector<float> tempBuffer;  // Allocated in prepareToPlay()

    void prepareToPlay(double sr, int maxBlock) override
    {
        tempBuffer.resize(maxBlock);  // Safe: not on audio thread
    }

    void processBlock(juce::AudioBuffer<float>& buffer) override
    {
        // Use pre-allocated buffer
        juce::FloatVectorOperations::copy(tempBuffer.data(),
            buffer.getReadPointer(0), buffer.getNumSamples());
    }
};
```

### 2. std::function Allocates

**Problem:** `std::function` may allocate memory when capturing.

```cpp
// WRONG: std::function may allocate
std::function<void(float)> callback = [this](float x) { process(x); };

// RIGHT: Use template for type erasure at compile time
template<typename Callback>
void setCallback(Callback&& cb) { callback = std::forward<Callback>(cb); }

// OR: Use raw function pointer + void* context
using AudioCallback = void(*)(void* context, float sample);
```

### 3. Lock Contention and Priority Inversion

**Problem:** Mutexes can block indefinitely; priority inversion causes audio thread starvation.

```cpp
// WRONG: Mutex in audio thread
void processBlock(juce::AudioBuffer<float>& buffer) override
{
    juce::ScopedLock lock(presetMutex);  // BLOCKS!
    // ...
}

// RIGHT: Use atomics or lock-free structures
std::atomic<PresetIndex> currentPreset{0};

// OR: Double-buffer with quick swap
// (See lock-free-patterns.md)
```

### 4. String Operations

**Problem:** `std::string` operations allocate.

```cpp
// WRONG: String in processBlock
void processBlock(...) override
{
    std::string name = getPluginName();  // ALLOCATES
    // ...
}

// RIGHT: Pre-store string, use string_view for viewing
std::string pluginName;  // Set in constructor

void processBlock(...) override
{
    std::string_view nameView = pluginName;  // No allocation
}
```

### 5. Dynamic Cast and RTTI

**Problem:** RTTI has unpredictable performance.

```cpp
// WRONG: dynamic_cast in processBlock
DerivedProcessor* derived = dynamic_cast<DerivedProcessor*>(base);

// RIGHT: Use enum tags or static polymorphism
enum class ProcessorType { Reverb, Delay, Filter };
ProcessorType type;
```

### 6. Virtual Functions (Acceptable but Know the Cost)

**Problem:** Virtual calls add indirection.

```cpp
// Virtual calls are acceptable in audio code
// But avoid virtual calls in tight inner loops

// ACCEPTABLE: Once per buffer
void processBlock(...) override { processor->process(buffer); }

// AVOID: In tight sample loop
for (int i = 0; i < numSamples; ++i) {
    processor->processSample(buffer[i]);  // Virtual call per sample!
}
```

### 7. Floating Point Denormals

**Problem:** Denormalized floats cause CPU stalls (10x-100x slower).

```cpp
// WRONG: Denormals can cause CPU spikes
float output = filter.process(input);  // May produce denormals

// RIGHT: Flush denormals to zero
#include <juce_dsp/juce_dsp.h>

void processBlock(...) override
{
    juce::ScopedNoDenormals noDenormals;  // RAII handler
    // All DSP here is safe
}

// OR: Add small DC offset to avoid zero-crossing denormals
constexpr float antiDenormal = 1e-20f;
float output = filter.process(input + antiDenormal);
```

### 8. Debug vs Release Performance

**Problem:** Debug builds can be 10-100x slower than release.

```cpp
// Always profile RELEASE builds
// Debug assertions and bounds checking add overhead

// WRONG: Optimizing for debug build speed
// RIGHT: Optimize for release; debug is for correctness

// Use juce::ScopedNoDenormals consistently
// Use juce::FloatVectorOperations for SIMD
```

## Quick Audit Checklist

Before committing `processBlock()` code, verify:

- [ ] No heap allocations (no `new`, `std::vector::push_back`, `std::string` operations)
- [ ] No mutex locks (use `std::atomic` or lock-free)
- [ ] No blocking I/O (no file, network, or system calls)
- [ ] Denormal handling (`juce::ScopedNoDenormals`)
- [ ] Bounds checking only where needed (avoid redundant checks)
- [ ] No virtual calls in inner loops
- [ ] All buffers pre-sized in `prepareToPlay()`

## Related

- cppreference: `thread_mutex.json`, `thread_atomic.json`
- JUCE: `juce::ScopedNoDenormals`, `juce::FloatVectorOperations`
- Topic: `overview.md`, `atomic-audio.md`