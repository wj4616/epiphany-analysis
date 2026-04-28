# Audio Thread Safety

---
id: TECH-001
category: cpp-patterns
tags: [audio, thread-safety, real-time]
---

## Summary

Rules for safe real-time audio processing. Violating these causes clicks, pops, crashes, or unbounded latency.

## The Seven Deadly Sins (Ross Bencina)

1. **No memory allocation** - `new`, `delete`, `malloc`, `free`, `std::vector::push_back`
2. **No locks** - `std::mutex`, `std::lock_guard`, `std::unique_lock`
3. **No I/O** - File read/write, network, `printf`
4. **No blocking OS calls** - System calls with unbounded execution
5. **No exceptions** - `throw`, `try`/`catch`
6. **No unbounded loops** - Must have worst-case bound
7. **No virtual dispatch overhead** - Use `final` for hot paths

## Correct Patterns

### Parameter Updates (GUI → Audio)

```cpp
// BAD: Lock in audio thread
void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    std::lock_guard<std::mutex> lock(paramMutex);  // WRONG!
    // ...
}

// GOOD: Lock-free atomic
std::atomic<float> gainParam;

void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    float gain = gainParam.load(std::memory_order_acquire);
    // ...
}
```

### Pre-Allocation

```cpp
// In prepareToPlay (not processBlock)
void prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Pre-allocate all buffers
    tempBuffer.setSize(2, samplesPerBlock);
    delayBuffer.setSize(2, maxDelaySamples);
}
```

### Lock-Free Queue

```cpp
// GUI → Audio communication
juce::AbstractFifo commandFifo{256};
std::vector<Command> commandBuffer;

void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    // Read commands without blocking
    int start1, size1, start2, size2;
    commandFifo.prepareToRead(1, start1, size1, start2, size2);
    // Process commands...
}
```

## Memory Ordering

```cpp
// For parameter updates
std::atomic<float> param;

// GUI thread (writer)
param.store(newValue, std::memory_order_release);

// Audio thread (reader)
float value = param.load(std::memory_order_acquire);
```

## Detection Methods

| Method | What It Catches |
|--------|-----------------|
| Audio thread audit (FM-01 to FM-07) | All violations |
| Address Sanitizer | Memory issues |
| Thread Sanitizer | Race conditions |
| Manual code review | Logic errors |

## Failure Modes

| FM | Failure | Symptom |
|----|---------|---------|
| FM-01 | Allocation | Random clicks, crashes |
| FM-02 | Lock | Clicks, pops, audio dropouts |
| FM-03 | I/O | Long delays, stuttering |
| FM-07 | Exception | Crash on edge case |

## Sources

- [Ross Bencina - Real-time Audio Programming 101](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing)
- [Bruce Dawson - Memory Ordering for Audio](https://www.bruce.audio/post/2025/02/24/memory_ordering/)
- JUCE Official Course