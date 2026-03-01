# Memory And Ownership

This guide is normative for memory allocation, ownership, and lifetime in firmware code.

## Priorities

- Prefer deterministic memory behavior.
- Prefer clear ownership and lifetimes.
- Prefer stack and static storage by default.
- Heap allocation is allowed when it is the least-bad option, but it must be intentional.

## Allocation Policy

- Do not allocate in ISRs.
- Avoid allocating in hard real-time paths.
- Prefer allocating during init/startup and reusing memory (pools, arenas, pre-sized buffers).

If a module uses heap allocation, document:

- when allocation happens (init vs steady state)
- failure behavior (what happens if allocation fails)
- upper bounds (maximum size/count)

## C++ Ownership

- Use RAII for anything that needs cleanup.
- Prefer automatic storage duration (stack) where possible.
- Prefer value types (`struct`/`class` members) when the size is reasonable.

### Owning Pointers

- Prefer `std::unique_ptr<T>` for single-owner heap objects.
- Avoid owning raw pointers.
- Avoid `std::shared_ptr` in firmware unless there is a clear, reviewed ownership graph need.

Prefer factory helpers that make allocation explicit. Be aware that in no-exception builds,
allocation failure does not throw.

If allocation failure must be handled, prefer:

- `new (std::nothrow) T(...)` with explicit null checks, or
- a project allocator API that returns a status.

Example:

```cpp
std::unique_ptr<Foo> make_foo() {
  Foo* raw = new (std::nothrow) Foo();
  return std::unique_ptr<Foo>(raw);
}
```

### Non-Owning Views

- Use raw pointers and references for non-owning relationships.
- Use `T*` for optional non-owning references.
- Use `T&` for required non-owning references.

Prefer passing buffer pairs as `(uint8_t* data, size_t size)` or `(const uint8_t* data, size_t size)`.

## C Ownership (C11)

- Make ownership explicit in API naming and documentation.
- Prefer caller-allocated buffers.
- If dynamic allocation is required, centralize it behind a small allocator module.

## Containers

- Prefer fixed-capacity containers in firmware.
- If using dynamic containers (e.g. `std::vector`), make allocation behavior explicit:
  - reserve upfront where possible
  - avoid steady-state growth

## Lifetime Rules

- Do not return pointers/references to stack objects.
- Avoid storing pointers to temporaries.
- Be explicit about ownership transfer.

If an API returns a pointer:

- Specify whether the caller owns it.
- Specify whether it is valid until the next call, until deinit, or indefinitely.

## Alignment And Placement

- Use `alignas(N)` for buffers with hardware alignment requirements.
- If using placement-new into a buffer/pool, ensure:
  - alignment is correct
  - destructor is called (or the type is trivially destructible)

## Fragmentation And Long Uptime

- Avoid unbounded allocate/free patterns in long-running firmware.
- Prefer pools/arenas for repeated allocations.
- Prefer bounded-size allocations.

## Memory-Mapped I/O

- Treat MMIO regions as non-owning.
- Keep register pointers/refs scoped and type-safe.
- Do not allocate as part of MMIO access.
