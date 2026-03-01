# Errors (No Exceptions)

Firmware C++ is assumed to be built with `-fno-exceptions` and `-fno-rtti`.

Error handling is explicit and cheap enough for firmware.

## Principles

- Prefer explicit results over hidden control flow.
- Make failure modes obvious at the call site.
- Do not use heap allocation as part of reporting an error.
- Separate:
  - programmer bugs (invariants violated) from
  - recoverable runtime failures (timeouts, I/O errors, busy) from
  - expected absence of a value (not an error)

## Choosing An Error Representation

Pick the simplest representation that preserves the needed information.

- If the operation cannot fail: return `void`.
- If the operation returns true/false only: return `bool`.
- If the operation can fail in multiple ways: return an explicit status code.
- If the operation produces a value and can fail:
  - Use an out-parameter + status code, or
  - Use a value-or-error type (only when it materially improves clarity).

This guide does not mandate a specific `Status` enum or `Result<T>` template.

If introducing a project-wide status/result framework (new types, macros, third-party libraries),
an AI agent SHOULD ask first.

## Must-Check Errors

- Do not ignore status returns that indicate a real failure.
- Use `[[nodiscard]]` selectively in C++:
  - Use it for APIs where ignoring failure is almost always a bug.
  - Avoid it for "try"/polling APIs where a non-success result is common and explicitly handled by
    control flow.
- In C, consider naming conventions or review rules for must-check APIs.

## Propagation

- Propagate errors upward quickly.
- Add context at module boundaries, not deep inside drivers.
- Keep error values stable; do not overload them with logging strings.

Prefer RAII to guarantee cleanup on early returns (works well without exceptions).

## Assertions And Panics

Use assertions for programmer errors and violated invariants.

- Assertions are for "this should never happen".
- Do not convert assertions into recoverable error paths unless the system design requires it.
- A panic/fault handler should be deterministic and safe to call from any context.

## Logging

- Logging is optional; do not make logging a requirement for error handling.
- Do not log in tight loops, ISRs, or allocation-failure paths.
- Prefer logging at higher layers where context exists and rate limiting is possible.

## ISR / Concurrency Constraints

- ISR-safe functions MUST NOT block.
- ISR-safe functions MUST NOT allocate.
- ISR-safe functions MUST NOT do heavy formatting or logging.
- Prefer signaling errors via:
  - a simple status return,
  - an atomic flag/counter,
  - or an event queued to a task-level handler.

## C Patterns (C11)

Common patterns for recoverable failures:

- Return a status code.
- Return `bool` and use out parameters.

Example (status return):

```c
typedef enum {
  STATUS_OK = 0,
  STATUS_TIMEOUT,
  STATUS_BUSY,
  STATUS_IO_ERROR,
  STATUS_INVALID_ARGUMENT,
} status_t;

status_t spi_write(const uint8_t* data, size_t size);
```

Example (out parameter + status):

```c
status_t adc_read(uint16_t* out_counts);
```

## C++ Patterns (C++17, No Exceptions)

Preferred patterns:

- Return a status code.
- Return `std::optional<T>` for "no value" (not an error).
- Use RAII types to manage resources and cleanup on early returns.

Value + error options:

- `Status` + out parameter.
- A value-or-error type ("expected"-like) when it materially improves clarity in a given module.

Example (selective `[[nodiscard]]`):

```cpp
enum class InitStatus : uint8_t {
  Ok,
  NotSupported,
  IOError,
};

[[nodiscard]] InitStatus init_flash();
```

Example (`std::optional`):

```cpp
std::optional<uint32_t> try_read_device_id();
```

## Heap And Error Paths

- Prefer not to allocate as part of an error path.
- Prefer RAII ownership wrappers instead of raw `new`/`delete`:
  - Prefer `std::unique_ptr` over owning raw pointers.
  - Avoid `std::shared_ptr` in firmware unless there is a clear ownership graph need.

Heap use is allowed when it is the least-bad option, but APIs should make allocation behavior clear.
