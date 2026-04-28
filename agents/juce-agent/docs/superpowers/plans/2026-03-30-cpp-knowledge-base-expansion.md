# C++ Knowledge Base Expansion Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Expand cpp-kb with filled placeholders and audio-focused realtime-safety topic guides.

**Architecture:** Preserve existing cppreference JSON files, add new topics/realtime-safety/ subdirectory with markdown guides, update index/manifest files to register new content.

**Tech Stack:** Python for harvesting scripts, JSON for index/manifest, Markdown for topic guides.

**Source Spec:** `docs/superpowers/specs/2026-03-30-cpp-knowledge-base-design.md`

---

## File Structure

```
cpp-kb/
├── cppreference/                    # EXISTING - 28 files
│   ├── algorithm*.json              # 5 synced
│   ├── container_*.json              # 3 synced
│   ├── memory_*.json                 # 4 synced + 2 placeholder
│   ├── string*.json                 # 1 synced + 1 placeholder
│   ├── thread_*.json                 # 4 synced + 1 placeholder
│   ├── utility_*.json               # 2 synced + 1 placeholder
│   └── functional_bind.json         # 1 placeholder
│
├── topics/                          # NEW
│   └── realtime-safety/
│       ├── overview.md              # NEW
│       ├── atomic-audio.md          # NEW
│       ├── lock-free-patterns.md    # NEW
│       └── pitfalls.md              # NEW
│
├── index.json                       # MODIFY
└── manifest.json                    # MODIFY
```

**Placeholder files to fill (6):**
1. `thread_atomic.json`
2. `memory_make_shared.json`
3. `memory_make_unique.json`
4. `string_string_view.json`
5. `functional_bind.json`
6. `utility_function.json`

---

## Task 1: Harvest std::atomic Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/thread_atomic.json`

**Source URL:** `https://en.cppreference.com/w/cpp/atomic`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch to retrieve std::atomic documentation:

```
URL: https://en.cppreference.com/w/cpp/atomic
Extract: Function signatures, member functions, example code, C++17 notes
```

- [ ] **Step 2: Create JSON content**

Update `thread_atomic.json` with harvested content. Structure must match existing synced files:

```json
{
  "id": "cpp-kb_cppreference_thread_atomic",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::atomic",
      "url": "https://en.cppreference.com/w/cpp/atomic",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/atomic",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/thread_atomic.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/thread_atomic.json
git commit -m "feat(cpp-kb): fill thread_atomic with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 2: Harvest make_shared Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/memory_make_shared.json`

**Source URL:** `https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch to retrieve make_shared documentation:

```
URL: https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
Extract: Function signatures, template parameters, example code, C++17 notes
```

- [ ] **Step 2: Create JSON content**

Update `memory_make_shared.json`:

```json
{
  "id": "cpp-kb_cppreference_memory_make_shared",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::make_shared",
      "url": "https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/memory_make_shared.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/memory_make_shared.json
git commit -m "feat(cpp-kb): fill memory_make_shared with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 3: Harvest make_unique Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/memory_make_unique.json`

**Source URL:** `https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch:

```
URL: https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
Extract: Function signatures, overloads, example code, C++14/17 notes
```

- [ ] **Step 2: Create JSON content**

Update `memory_make_unique.json`:

```json
{
  "id": "cpp-kb_cppreference_memory_make_unique",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::make_unique",
      "url": "https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/memory_make_unique.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/memory_make_unique.json
git commit -m "feat(cpp-kb): fill memory_make_unique with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 4: Harvest string_view Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/string_string_view.json`

**Source URL:** `https://en.cppreference.com/w/cpp/string/basic_string_view`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch:

```
URL: https://en.cppreference.com/w/cpp/string/basic_string_view
Extract: Class template, member functions, C++17 features, example code
```

- [ ] **Step 2: Create JSON content**

Update `string_string_view.json`:

```json
{
  "id": "cpp-kb_cppreference_string_string_view",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::string_view",
      "url": "https://en.cppreference.com/w/cpp/string/basic_string_view",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/string/basic_string_view",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/string_string_view.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/string_string_view.json
git commit -m "feat(cpp-kb): fill string_string_view with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 5: Harvest std::bind Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/functional_bind.json`

**Source URL:** `https://en.cppreference.com/w/cpp/utility/functional/bind`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch:

```
URL: https://en.cppreference.com/w/cpp/utility/functional/bind
Extract: Function template, placeholders, example code, C++17 notes
```

- [ ] **Step 2: Create JSON content**

Update `functional_bind.json`:

```json
{
  "id": "cpp-kb_cppreference_functional_bind",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::bind",
      "url": "https://en.cppreference.com/w/cpp/utility/functional/bind",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/utility/functional/bind",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/functional_bind.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/functional_bind.json
git commit -m "feat(cpp-kb): fill functional_bind with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 6: Harvest std::function Content

**Files:**
- Modify: `playbookdata/cpp-kb/cppreference/utility_function.json`

**Source URL:** `https://en.cppreference.com/w/cpp/utility/functional/function`

- [ ] **Step 1: Fetch cppreference content**

Use WebFetch:

```
URL: https://en.cppreference.com/w/cpp/utility/functional/function
Extract: Class template, member functions, call semantics, example code
```

- [ ] **Step 2: Create JSON content**

Update `utility_function.json`:

```json
{
  "id": "cpp-kb_cppreference_utility_function",
  "kb": "cpp-kb",
  "topic": "cppreference",
  "status": "synced",
  "version": "2.0.0",
  "summary": "<extracted summary>",
  "concepts": [],
  "code_blocks": [
    {
      "language": "cpp",
      "code": "<example code from cppreference>",
      "preserved": true
    }
  ],
  "references": [
    {
      "title": "std::function",
      "url": "https://en.cppreference.com/w/cpp/utility/functional/function",
      "domain": "en.cppreference.com"
    }
  ],
  "source_url": "https://en.cppreference.com/w/cpp/utility/functional/function",
  "harvested_at": "<ISO timestamp>",
  "synced_at": "<ISO timestamp>"
}
```

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/cppreference/utility_function.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/cppreference/utility_function.json
git commit -m "feat(cpp-kb): fill utility_function with cppreference content

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 7: Create topics/realtime-safety Directory

**Files:**
- Create: `playbookdata/cpp-kb/topics/realtime-safety/` (directory)

- [ ] **Step 1: Create directory structure**

Run:
```bash
mkdir -p playbookdata/cpp-kb/topics/realtime-safety
```

- [ ] **Step 2: Verify directory exists**

Run: `ls -la playbookdata/cpp-kb/topics/`
Expected: `realtime-safety` directory visible

---

## Task 8: Create overview.md Topic File

**Files:**
- Create: `playbookdata/cpp-kb/topics/realtime-safety/overview.md`

- [ ] **Step 1: Write overview content**

Create file with real-time audio constraints introduction:

```markdown
# Real-Time Safety Overview

## Why This Matters

Real-time audio processing has strict timing requirements. The audio thread must complete processing within the buffer period (typically 1-10ms). Missing this deadline causes audible glitches, dropouts, or crashes. Unlike typical software development, real-time audio code cannot safely use many common C++ patterns.

## When to Use

Every line of code running on the audio thread in a JUCE plugin:
- `processBlock()` method
- Audio thread callbacks
- Real-time parameter updates
- DSP processing chains

## C++17 Features

### Safe for Real-Time

- **std::atomic<T>** - Lock-free atomic operations for thread-safe communication
- **std::array** - Fixed-size containers with no allocation
- **std::string_view** - Non-owning string references (C++17)
- **constexpr** - Compile-time computation
- **Pre-allocated buffers** - Memory allocated during initialization

### Unsafe for Real-Time

- **new/delete** - Heap allocation has unbounded latency
- **std::vector::push_back()** - May trigger reallocation
- **std::mutex** - Can block indefinitely
- **std::function** - Type erasure may allocate
- **Virtual function calls** - Indirection overhead (acceptable in moderation)

## Core Principle

**No allocation, no blocking, no undefined latency.**

Audio thread code must:
1. Never allocate heap memory
2. Never block on locks or I/O
3. Complete processing within the buffer deadline

## Code Pattern

```cpp
// REAL-TIME SAFE: Atomic parameter updates
class SafeProcessor
{
public:
    void setGain(float newGain)
    {
        // Called from UI thread - can allocate
        gain.store(newGain, std::memory_order_release);
    }

    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        // Called from audio thread - NO allocations
        const float currentGain = gain.load(std::memory_order_acquire);

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] *= currentGain;  // No allocation
            }
        }
    }

private:
    std::atomic<float> gain{1.0f};  // Atomic for thread safety
};
```

## Pitfalls

1. **Hidden allocations** - std::function, std::string, std::vector inside processBlock
2. **Lock contention** - std::mutex can deadlock or cause priority inversion
3. **Debug builds** - Debug code may be slower; always profile release builds
4. **Assumed fast operations** - Even std::shared_ptr::operator= allocates sometimes

## Related

- cppreference: `thread_atomic.json`
- JUCE: `AudioProcessor::processBlock()`
- Topic: `lock-free-patterns.md`
```

- [ ] **Step 2: Verify file exists**

Run: `cat playbookdata/cpp-kb/topics/realtime-safety/overview.md | head -20`
Expected: First 20 lines of the content

- [ ] **Step 3: Commit**

```bash
git add playbookdata/cpp-kb/topics/realtime-safety/overview.md
git commit -m "feat(cpp-kb): add realtime-safety overview topic

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 9: Create atomic-audio.md Topic File

**Files:**
- Create: `playbookdata/cpp-kb/topics/realtime-safety/atomic-audio.md`

- [ ] **Step 1: Write atomic-audio content**

```markdown
# std::atomic for Audio Thread Communication

## Why This Matters

The audio thread and UI thread run concurrently. Parameters changed on the UI thread must safely propagate to the audio thread without blocking or allocation. std::atomic provides lock-free thread-safe communication ideal for this use case.

## When to Use

- Parameter updates from UI to audio thread
- Transport state (playing/stopped)
- Bypass state changes
- Simple state flags that need thread visibility

## C++17 Features

### std::atomic<T>

Lock-free atomic operations for primitive types:
- `std::atomic<bool>` - Flags, bypass states
- `std::atomic<float>` - Gain, mix levels
- `std::atomic<int>` - Voice counts, buffer sizes
- `std::atomic<std::shared_ptr<T>>` (C++20) - NOT available in C++17

### Memory Ordering

- `memory_order_relaxed` - No ordering guarantees, just atomicity
- `memory_order_acquire` - No reads below can be reordered before this
- `memory_order_release` - No writes above can be reordered after this
- `memory_order_seq_cst` (default) - Full sequential consistency

## Code Pattern

```cpp
// Thread-safe parameter communication
class AudioPluginProcessor : public juce::AudioProcessor
{
public:
    // Called from UI thread (can allocate, can block)
    void setReverbMix(float mix)
    {
        reverbMix.store(mix, std::memory_order_release);
    }

    void setBypass(bool shouldBypass)
    {
        bypass.store(shouldBypass, std::memory_order_release);
    }

    // Called from audio thread (must be real-time safe)
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        // Acquire ensures we see the latest value from UI thread
        const float currentMix = reverbMix.load(std::memory_order_acquire);
        const bool isBypassed = bypass.load(std::memory_order_acquire);

        if (isBypassed)
        {
            buffer.clear();  // Fast, no allocation
            return;
        }

        // Use currentMix for DSP processing
        reverb.process(buffer, currentMix);
    }

private:
    std::atomic<float> reverbMix{0.5f};
    std::atomic<bool> bypass{false};
    ReverbProcessor reverb;  // Pre-allocated DSP
};
```

## Pitfalls

1. **Large atomics are not lock-free** - std::atomic for types > sizeof(void*) may use locks internally. Check with `is_always_lock_free`.

2. **ABA problem** - If you compare_exchange multiple times, another thread's change can be hidden. Use versioned values for complex state.

3. **Memory ordering errors** - Using relaxed ordering incorrectly can cause subtle bugs. Default to acquire/release for safety.

4. **Atomic pointers** - std::atomic<T*> works, but you're still responsible for the pointed-to object's lifetime.

## Safe Pattern: Lock-Free Queue for Arrays

```cpp
// For arrays/objects, use a lock-free queue or ring buffer
// JUCE provides AbstractFifo for this purpose
juce::AbstractFifo fifo{1024};

// Producer (UI thread) writes to fifo
// Consumer (audio thread) reads from fifo
// Both sides are allocation-free and wait-free
```

## Related

- cppreference: `thread_atomic.json`
- JUCE: `juce::AbstractFifo`
- Topic: `lock-free-patterns.md`
```

- [ ] **Step 2: Verify file exists**

Run: `cat playbookdata/cpp-kb/topics/realtime-safety/atomic-audio.md | head -20`
Expected: First 20 lines of the content

- [ ] **Step 3: Commit**

```bash
git add playbookdata/cpp-kb/topics/realtime-safety/atomic-audio.md
git commit -m "feat(cpp-kb): add atomic-audio topic for std::atomic patterns

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 10: Create lock-free-patterns.md Topic File

**Files:**
- Create: `playbookdata/cpp-kb/topics/realtime-safety/lock-free-patterns.md`

- [ ] **Step 1: Write lock-free-patterns content**

```markdown
# Lock-Free Communication Patterns

## Why This Matters

When atomics aren't enough (e.g., passing arrays, strings, or complex objects between threads), you need lock-free data structures. These avoid the unbounded latency of mutexes while safely passing data between threads.

## When to Use

- Parameter changes that involve more than one value
- Sending preset data to the audio thread
- Communication between UI thread and audio thread
- Multi-threaded audio processing chains

## C++17 Features

### JUCE AbstractFifo

JUCE's `AbstractFifo` provides a lock-free ring buffer implementation:
- Single-producer, single-consumer (SPSC) queue
- Wait-free operations (no blocking)
- Pre-allocated memory
- Suitable for real-time audio

### Common Patterns

1. **Ring buffer for parameter arrays**
2. **Double buffering for large state**
3. **Message queue for commands**

## Code Pattern: Lock-Free Parameter Queue

```cpp
#include <juce_core/juce_core.h>

// Thread-safe lock-free queue for parameter changes
class ParameterQueue
{
public:
    ParameterQueue() : fifo(256) {}  // Pre-allocated size

    // Called from UI thread - copies parameter into queue
    bool pushParameter(int paramId, float value)
    {
        int start1, size1, start2, size2;
        fifo.prepareToWrite(1, start1, size1, start2, size2);

        if (size1 == 0) return false;  // Queue full

        buffer[start1] = {paramId, value};

        fifo.finishedWrite(1);
        return true;
    }

    // Called from audio thread - pops parameter from queue
    bool popParameter(int& paramId, float& value)
    {
        int start1, size1, start2, size2;
        fifo.prepareToRead(1, start1, size1, start2, size2);

        if (size1 == 0) return false;  // Queue empty

        paramId = buffer[start1].paramId;
        value = buffer[start1].value;

        fifo.finishedRead(1);
        return true;
    }

private:
    struct ParamChange { int paramId; float value; };

    juce::AbstractFifo fifo;
    std::array<ParamChange, 256> buffer;  // Pre-allocated, fixed size
};
```

## Code Pattern: Double Buffering for Large State

```cpp
// Double buffering for preset changes
class PresetManager
{
public:
    // UI thread: writes to back buffer
    void updatePreset(const Preset& newPreset)
    {
        {
            juce::ScopedLock lock(bufferSwap);
            backBuffer = newPreset;  // Copy into back buffer
            swapNeeded.store(true, std::memory_order_release);
        }
    }

    // Audio thread: swaps to front buffer if needed
    const Preset& getCurrentPreset()
    {
        if (swapNeeded.load(std::memory_order_acquire))
        {
            juce::ScopedLock lock(bufferSwap);
            std::swap(frontBuffer, backBuffer);
            swapNeeded.store(false, std::memory_order_release);
        }
        return frontBuffer;
    }

private:
    Preset frontBuffer;  // Active in audio thread
    Preset backBuffer;   // Being written by UI thread
    std::atomic<bool> swapNeeded{false};
    juce::CriticalSection bufferSwap;  // Only for swap operation
};
```

## Pitfalls

1. **Queue overflow** - If the queue fills faster than it drains, data is lost. Size queues appropriately and profile.

2. **Unbounded queues** - Never use `std::queue` or `std::deque` on audio thread; they allocate.

3. **Priority inversion** - Even lock-free code can suffer if the UI thread starves the audio thread.

4. **False sharing** - If producers/consumers write to adjacent memory locations, cache thrashing degrades performance.

## Related

- cppreference: `thread_atomic.json`
- JUCE: `juce::AbstractFifo`
- Topic: `pitfalls.md`
```

- [ ] **Step 2: Verify file exists**

Run: `cat playbookdata/cpp-kb/topics/realtime-safety/lock-free-patterns.md | head -20`
Expected: First 20 lines of the content

- [ ] **Step 3: Commit**

```bash
git add playbookdata/cpp-kb/topics/realtime-safety/lock-free-patterns.md
git commit -m "feat(cpp-kb): add lock-free-patterns topic

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 11: Create pitfalls.md Topic File

**Files:**
- Create: `playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md`

- [ ] **Step 1: Write pitfalls content**

```markdown
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
```

- [ ] **Step 2: Verify file exists**

Run: `cat playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md | head -20`
Expected: First 20 lines of the content

- [ ] **Step 3: Commit**

```bash
git add playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md
git commit -m "feat(cpp-kb): add realtime-safety pitfalls topic

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 12: Update index.json

**Files:**
- Modify: `playbookdata/cpp-kb/index.json`

- [ ] **Step 1: Add topics section to index.json**

Add the `realtime-safety` topic to the topics object. The current index only has `cppreference`:

```json
{
  "kb_name": "cpp-kb",
  "version": "1.0.0",
  "last_updated": "<current ISO timestamp>",
  "topics": {
    "cppreference": {
      "files": [
        "algorithm_sort.json",
        "algorithm_transform.json",
        "thread_shared_mutex.json",
        "container_vector.json",
        "utility_function.json",
        "container_array.json",
        "thread_condition_variable.json",
        "thread_mutex.json",
        "thread_unique_lock.json",
        "memory_make_shared.json",
        "memory_allocator.json",
        "algorithm_find.json",
        "thread_lock_guard.json",
        "string_string_view.json",
        "memory_make_unique.json",
        "algorithm.json",
        "functional_bind.json",
        "memory_unique_ptr.json",
        "string.json",
        "algorithm_copy.json",
        "utility_any.json",
        "utility_optional.json",
        "memory_weak_ptr.json",
        "thread_atomic.json",
        "memory_shared_ptr.json",
        "utility_variant.json",
        "thread.json",
        "container_span.json"
      ],
      "file_count": 28,
      "last_updated": "<existing timestamp>"
    },
    "realtime-safety": {
      "files": [
        "overview.md",
        "atomic-audio.md",
        "lock-free-patterns.md",
        "pitfalls.md"
      ],
      "file_count": 4,
      "last_updated": "<current ISO timestamp>"
    }
  }
}
```

- [ ] **Step 2: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/index.json'))"`
Expected: No error

- [ ] **Step 3: Commit**

```bash
git add playbookdata/cpp-kb/index.json
git commit -m "feat(cpp-kb): add realtime-safety topic to index

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 13: Update manifest.json

**Files:**
- Modify: `playbookdata/cpp-kb/manifest.json`

- [ ] **Step 1: Add realtime-safety topic to manifest**

Add new topic entry with all 4 files:

```json
{
  "kb_name": "cpp-kb",
  "version": "1.0.0",
  "created": "<existing timestamp>",
  "last_sync": "<current ISO timestamp>",
  "status": "ready",
  "topics": [
    {
      "name": "cppreference",
      "files": {
        // ... existing 28 files (with 6 now having status: "synced")
      }
    },
    {
      "name": "realtime-safety",
      "files": {
        "overview.md": {
          "status": "synced",
          "harvested_at": "<current ISO timestamp>",
          "synced_at": "<current ISO timestamp>",
          "synced_timestamp": <unix timestamp>,
          "has_semantic": false,
          "source": "curated",
          "markdown_length": <actual length>
        },
        "atomic-audio.md": {
          "status": "synced",
          "harvested_at": "<current ISO timestamp>",
          "synced_at": "<current ISO timestamp>",
          "synced_timestamp": <unix timestamp>,
          "has_semantic": false,
          "source": "curated",
          "markdown_length": <actual length>
        },
        "lock-free-patterns.md": {
          "status": "synced",
          "harvested_at": "<current ISO timestamp>",
          "synced_at": "<current ISO timestamp>",
          "synced_timestamp": <unix timestamp>,
          "has_semantic": false,
          "source": "curated",
          "markdown_length": <actual length>
        },
        "pitfalls.md": {
          "status": "synced",
          "harvested_at": "<current ISO timestamp>",
          "synced_at": "<current ISO timestamp>",
          "synced_timestamp": <unix timestamp>,
          "has_semantic": false,
          "source": "curated",
          "markdown_length": <actual length>
        }
      }
    }
  ]
}
```

- [ ] **Step 2: Update placeholder file statuses**

For the 6 cppreference placeholder files, change `"status": "placeholder"` to `"status": "synced"` and add the appropriate timestamps.

- [ ] **Step 3: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/cpp-kb/manifest.json'))"`
Expected: No error

- [ ] **Step 4: Commit**

```bash
git add playbookdata/cpp-kb/manifest.json
git commit -m "feat(cpp-kb): update manifest with realtime-safety and synced placeholders

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 14: Update master-index.json

**Files:**
- Modify: `playbookdata/master-index.json`

- [ ] **Step 1: Add cpp-kb cross-references**

Add new topic entries for realtime_safety and audio_thread:

```json
{
  "realtime_safety": {
    "description": "Real-time audio thread safety patterns",
    "kbs": ["cpp-kb"],
    "topics": ["overview", "atomic-audio", "lock-free-patterns", "pitfalls"]
  },
  "audio_thread": {
    "description": "Audio thread programming rules and constraints",
    "kbs": ["cpp-kb"],
    "topics": ["realtime-safety"]
  }
}
```

- [ ] **Step 2: Verify JSON is valid**

Run: `python3 -c "import json; json.load(open('playbookdata/master-index.json'))"`
Expected: No error

- [ ] **Step 3: Commit**

```bash
git add playbookdata/master-index.json
git commit -m "feat(master-index): add realtime_safety and audio_thread cross-references

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 15: Update Skill References

**Files:**
- Modify: `~/.claude/skills/juce-audio-thread-audit/SKILL.md`
- Modify: `~/.claude/skills/juce-dsp-implementation/SKILL.md`

- [ ] **Step 1: Add reference to juce-audio-thread-audit skill**

Add to the Related section of `juce-audio-thread-audit/SKILL.md`:

```markdown
## Related

- **C++ Knowledge Base:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/pitfalls.md`
- **Lock-Free Patterns:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/lock-free-patterns.md`
```

- [ ] **Step 2: Add reference to juce-dsp-implementation skill**

Add to the Related section of `juce-dsp-implementation/SKILL.md`:

```markdown
## Related

- **Real-Time Safety Overview:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/overview.md`
- **Atomic Audio Patterns:** `~/.agents/juce-agent/playbookdata/cpp-kb/topics/realtime-safety/atomic-audio.md`
```

- [ ] **Step 3: Verify skill files are valid**

Run: `head -50 ~/.claude/skills/juce-audio-thread-audit/SKILL.md`
Run: `head -50 ~/.claude/skills/juce-dsp-implementation/SKILL.md`
Expected: Both show the Related section with the new links

- [ ] **Step 4: Commit**

```bash
git add ~/.claude/skills/juce-audio-thread-audit/SKILL.md
git add ~/.claude/skills/juce-dsp-implementation/SKILL.md
git commit -m "feat(skills): add cpp-kb realtime-safety references to skills

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 16: Validate KB Structure

**Files:**
- Test: KB structure validation

- [ ] **Step 1: Run validation script**

Run:
```bash
cd ~/agents/juce-agent/playbookdata
python3 scripts/validate-kb-structure.py
```

Expected: All checks pass

- [ ] **Step 2: Verify no remaining placeholders**

Run:
```bash
cat cpp-kb/manifest.json | python3 -c "import json, sys; data = json.load(sys.stdin); placeholders = [f for t in data['topics'] for f, v in t['files'].items() if v.get('status') == 'placeholder']; print('Placeholders:', placeholders if placeholders else 'None')"
```

Expected: `Placeholders: None`

- [ ] **Step 3: Verify topic files exist**

Run:
```bash
ls -la cpp-kb/topics/realtime-safety/
```

Expected: 4 files listed (overview.md, atomic-audio.md, lock-free-patterns.md, pitfalls.md)

---

## Success Criteria

- [ ] All 6 placeholder files have status "synced" with real content
- [ ] 4 topic files created in `topics/realtime-safety/`
- [ ] `index.json` includes `realtime-safety` topic
- [ ] `manifest.json` includes `realtime-safety` topic with 4 files
- [ ] `master-index.json` includes `realtime_safety` and `audio_thread` cross-references
- [ ] Skill files have references to new topics
- [ ] No existing cppreference content lost or modified
- [ ] Validation script passes

## Files Modified Summary

| File | Action |
|------|--------|
| `cppreference/thread_atomic.json` | Fill placeholder |
| `cppreference/memory_make_shared.json` | Fill placeholder |
| `cppreference/memory_make_unique.json` | Fill placeholder |
| `cppreference/string_string_view.json` | Fill placeholder |
| `cppreference/functional_bind.json` | Fill placeholder |
| `cppreference/utility_function.json` | Fill placeholder |
| `topics/realtime-safety/overview.md` | Create |
| `topics/realtime-safety/atomic-audio.md` | Create |
| `topics/realtime-safety/lock-free-patterns.md` | Create |
| `topics/realtime-safety/pitfalls.md` | Create |
| `index.json` | Add realtime-safety topic |
| `manifest.json` | Add realtime-safety topic, update statuses |
| `master-index.json` | Add cross-references |
| `juce-audio-thread-audit/SKILL.md` | Add Related links |
| `juce-dsp-implementation/SKILL.md` | Add Related links |

**Total:** 6 placeholder files filled + 4 topic files created + 3 index files updated + 2 skill files updated = 15 files