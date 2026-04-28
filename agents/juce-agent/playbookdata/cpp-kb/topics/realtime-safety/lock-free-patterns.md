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