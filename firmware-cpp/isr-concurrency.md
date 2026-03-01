# ISR And Concurrency

This guide is normative for interrupt-safety and concurrency in firmware.

## Goals

- Keep interrupt handlers fast, bounded, and deterministic.
- Make shared-state access correct under preemption.
- Avoid concurrency primitives that are unavailable or too costly on the target.

## Definitions

- ISR: interrupt service routine (including high-priority callbacks).
- Task/thread context: normal execution context (bare-metal main loop or RTOS task).
- ISR-safe API: an API that may be called from an ISR.

## ISR Rules (MUST)

- Do the minimum work necessary.
- MUST NOT block (no mutex waits, no sleeps, no waiting on peripherals).
- MUST NOT allocate (no heap, no dynamic containers).
- MUST NOT do heavy logging or string formatting.
- MUST NOT call non-ISR-safe drivers/APIs.
- Prefer deferring work to task context using a queue, ring buffer, event flag, or a state machine.

If an ISR must interact with shared state, use one of:

- a critical section (brief interrupt mask)
- an atomic variable
- a lock-free single-producer/single-consumer structure (typical ISR->task direction)

## Volatile vs Atomic

- `volatile` is for memory-mapped I/O and variables changed outside the current control flow (e.g.
  modified by hardware).
- `volatile` is not a synchronization primitive and does not make code thread-safe.

Use atomics for cross-context synchronization:

- In C++: `std::atomic<T>`.
- In C: use compiler intrinsics or a tiny platform abstraction for atomic ops.

## Critical Sections

Use critical sections to protect short sequences that must be atomic with respect to interrupts.

- Keep critical sections as short as possible.
- Do not call slow functions from a critical section.
- Prefer masking only the specific interrupt(s) where the platform supports it.

Example (platform abstraction):

```cpp
class IrqLock {
 public:
  IrqLock();
  ~IrqLock();
  IrqLock(const IrqLock&) = delete;
  IrqLock& operator=(const IrqLock&) = delete;
};

void enqueue_byte(uint8_t b) {
  IrqLock lock;
  // update shared ring buffer indices
}
```

## Locks And Blocking Primitives

- Do not use blocking primitives in ISRs.
- If an API can be called from both ISR and task context, design it as non-blocking.
- Avoid nested locks where possible; when unavoidable, define and follow a lock order.

## Memory Ordering

- Default to simple patterns that do not require custom memory orders.
- If you must use relaxed/acquire/release orderings, document the invariant and test it.
- Use platform barriers where required for DMA/cache-coherency or MMIO ordering.

## Shared State Patterns

Preferred patterns (simple and common in firmware):

- ISR writes data into a ring buffer; task drains it.
- ISR sets an atomic flag/counter; task polls or waits on an event.
- ISR captures a timestamp/counter; task does the heavy processing.

Avoid:

- multi-writer shared mutable structures without strict ownership
- unbounded queues
- allocating on the fly under load

## API Marking

Make ISR constraints visible.

- Prefer naming or documentation that clearly marks ISR-safe functions.
- If a function is not ISR-safe, say so in its header/API comment.

Example naming options (pick one per project):

```text
gpio_set_level_isr(...)
gpio_set_level_from_isr(...)
gpio_set_level_irq(...)
```

## Register Access

- MMIO access is typically `volatile`.
- Avoid read-modify-write races on shared registers; use atomic bit set/clear registers if
  available, or protect with a critical section.
- Keep register writes localized (small functions) to reduce accidental ordering bugs.

## Interrupt Priorities (When Applicable)

- Be explicit about which interrupts may preempt others.
- If a shared resource is touched by multiple priority levels, choose a single ownership point or
  protect it with a mechanism that is correct for that priority scheme.

## Testing

- Prefer designs that can be tested without real interrupts (state machines, explicit queues).
- For host tests, model ISR->task interactions with a producer/consumer thread or an injected
  callback.
