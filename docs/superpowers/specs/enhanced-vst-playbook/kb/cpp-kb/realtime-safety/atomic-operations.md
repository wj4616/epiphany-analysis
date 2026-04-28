# Real-Time Safety: Atomic Operations

> Audio thread safety patterns for real-time programming

**Category**: realtime-safety
**Confidence**: 1.0
**Source**: JUCE best practices, melatonin.dev, C++ standards

## The Problem

The audio thread must complete within a deadline (typically 1-10ms). Any blocking operation causes dropouts.

## Atomic Operations

### Safe Patterns

```cpp
// Flag pattern for cross-thread communication
std::atomic<bool> parameterChanged{false};
float targetValue{0.0f};

// Audio thread (processBlock)
float currentValue = targetValue.load();  // safe
if (parameterChanged.load()) {
    // update local state
    parameterChanged.store(false);
}

// GUI thread (parameterChanged callback)
void parameterChanged(float newValue) {
    targetValue.store(newValue);  // safe
    parameterChanged.store(true); // safe
}
```

### Unsafe Patterns

```cpp
// NEVER do this on audio thread:
std::lock_guard<std::mutex> lock(mutex);  // BLOCKS!
std::vector::push_back(x);                  // ALLOCATES!
new Object();                               // ALLOCATES!
std::string str = "hello";                  // ALLOCATES!
```

## Why Atomics Are Safe

| Operation | Thread-Safe? | Real-Time Safe? |
|-----------|--------------|-----------------|
| `std::atomic<T>.load()` | Yes | Yes |
| `std::atomic<T>.store()` | Yes | Yes |
| `std::atomic<T>.exchange()` | Yes | Yes (usually) |
| `std::mutex.lock()` | Yes | **NO** |
| `std::vector.push_back()` | No | **NO** |
| `new Object()` | No | **NO** |

## Lock-Free Alternatives

```cpp
// Instead of mutex:
std::atomic<bool> flag{false};  // safe

// Instead of vector::push_back:
// Pre-allocate in prepareToPlay()
buffer.resize(maxSize);  // called once

// Instead of std::string:
// Use fixed-size char array or juce::String (pre-allocated)
```

## Checklist

- [ ] All atomic operations in processBlock
- [ ] No mutex/lock_guard in processBlock
- [ ] No vector::push_back in processBlock
- [ ] No new/delete in processBlock
- [ ] No std::string operations in processBlock

## Related Entries

- [Allocation on Audio Thread](./allocation.md)
- [Lock-Free Patterns](./lock-free-patterns.md)

## Sources

- JUCE Forum best practices
- melatonin.dev/audio-thread-safety
- C++11 std::atomic specification