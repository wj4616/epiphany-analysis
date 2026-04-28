# std::atomic for Audio Thread Communication

## Why This Matters

The audio thread and UI thread run concurrently. Parameters changed on the UI thread must safely propagate to the audio thread without blocking or allocation. std::atomic provides lock-free thread-safe communication ideal for this use case.

## When to Use

- Parameter updates from UI to audio thread
- Transport state (playing/stopped)
- Bypass state changes
- Simple state flags that need thread visibility

## C++17 Features

### std::atomic\<T\>

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

4. **Atomic pointers** - std::atomic\<T*\> works, but you're still responsible for the pointed-to object's lifetime.

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