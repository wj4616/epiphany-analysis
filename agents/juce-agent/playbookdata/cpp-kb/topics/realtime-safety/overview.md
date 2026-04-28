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

- **std::atomic\<T\>** - Lock-free atomic operations for thread-safe communication
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