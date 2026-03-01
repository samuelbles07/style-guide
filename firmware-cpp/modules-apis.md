# Modules And APIs

This guide is normative for module boundaries, public APIs, and header/source responsibilities.

Formatting details are owned by `clang-format`; this guide focuses on structure and contracts.

## Principles

- Keep module interfaces small and stable.
- Make costs explicit (allocation, blocking, latency).
- Make execution context explicit (task vs ISR).
- Prefer deterministic behavior.
- Avoid hidden dependencies and transitive include reliance.

## What Is A Module

A module is a unit of code with a clear responsibility and a defined public surface. Examples:

- a device driver
- a platform abstraction
- a protocol implementation
- a service used by multiple features

Modules should be composable and testable.

## Public vs Private Surface

- Public API: what other modules are allowed to call/include.
- Private API: internal helpers and details; may change freely.

Rules:

- Public headers MUST NOT expose unnecessary implementation details.
- Public headers SHOULD avoid heavy vendor/device headers.
- Private helpers belong in `.c`/`.cpp` or private headers not used outside the module.

## Headers vs Sources

Headers:

- Declare types, constants, and functions needed by callers.
- Must be self-sufficient: include what is required for the declarations.
- Keep inline code minimal; only inline when required for performance or templating.

Sources:

- Own heavy includes and vendor headers.
- Own implementation details and private helper functions.
- Own explicit instantiations if needed.

## API Shape (Pick What Fits)

This guide does not mandate a single API style. Common shapes in firmware:

- C style: explicit context/handle + free functions.
- C++ style: a class with explicit `init()` / `deinit()` and clear ownership.
- C ABI + C++ wrapper: common when integrating vendor code.

When designing a new module, prefer the simplest shape that:

- matches the project architecture
- keeps ABI stable where needed
- is testable without hardware

## Contract Checklist

Public APIs SHOULD document:

- Ownership: who owns pointers/buffers; whether the callee stores them.
- Lifetime: how long returned pointers/references remain valid.
- Concurrency: thread-safety expectations.
- ISR-safety: whether the API may be called from ISR context.
- Blocking: whether the call can block or wait on hardware.
- Allocation: whether the call allocates heap memory.
- Units: where ambiguity exists (e.g. `_ms`, `_hz`, `_bytes`).

If a module introduces a project-wide framework (new base classes, allocators, status/result types,
build-time codegen), an AI agent SHOULD ask first.

## Errors

- Do not rely on exceptions for error handling.
- Use explicit error reporting appropriate to the module.
- Avoid logging deep inside low-level modules unless explicitly designed for it.

See `firmware-cpp/errors.md`.

## Allocation And Latency

- Avoid hidden allocation.
- Avoid hidden unbounded work.
- If a function may allocate, block, or take an unbounded amount of time, make that visible in the
  API contract.

See `firmware-cpp/memory.md`.

## Concurrency And ISR Boundaries

- Mark ISR-safe APIs explicitly (by documentation and/or naming).
- Prefer non-blocking designs for APIs callable from both ISR and task contexts.

See `firmware-cpp/isr-concurrency.md`.

## Dependency Hygiene

- Prefer acyclic dependencies.
- Prefer injecting dependencies explicitly (constructor params, init structs, context pointers)
  instead of using global mutable singletons.
- Keep platform-specific `#if` usage contained; isolate platform differences behind a small
  interface.

See `firmware-cpp/includes.md` and `firmware-cpp/macros-config.md`.

## Test Seams

- Favor APIs that can be exercised in host tests.
- Separate pure logic from hardware I/O.
- Prefer narrow interfaces for hardware access so they can be faked.

## Minimal Examples

Example (API contract comment):

```cpp
// ISR-safe: no
// Thread-safe: no
// Blocking: yes (waits for peripheral)
// Allocates: no
// Errors: explicit status return
[[nodiscard]] int spi_transfer(const uint8_t* tx, uint8_t* rx, size_t size);
```

Example (explicit context in C):

```c
typedef struct {
  uintptr_t base_addr;
} UART;

void uart_init(UART* uart);
int uart_write(UART* uart, const uint8_t* data, size_t size);
```
