# C And C++ Interop

This guide is normative for boundaries between C11 and C++17 in firmware code.

## Goals

- Keep ABI boundaries simple and stable.
- Make ownership, lifetime, and threading assumptions explicit.
- Allow vendor/HAL code in C while keeping application and drivers in modern C++.

## When To Use C

Common reasons to keep code in C:

- vendor-provided sources/headers
- tight hardware bring-up code
- linker/section glue
- very small, freestanding modules

Prefer C++ for new modules unless a C-only boundary is required.

## `extern "C"` (Name Mangling)

Expose C-callable functions from C++ and include C headers in C++ safely using an `extern "C"`
guard.

In a C header that may be included by C++:

```c
#ifndef MY_MODULE_H
#define MY_MODULE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t baud_rate_hz;
} UARTConfig;

bool uart_init(const UARTConfig* config);

#ifdef __cplusplus
}
#endif

#endif  // MY_MODULE_H
```

Notes:

- `extern "C"` affects linkage (name mangling) of functions and variables, not type layout.
- Do not use overloaded functions, templates, or references across a C ABI boundary.

## ABI-Safe API Design

Across a C/C++ boundary, prefer plain data and explicit sizes.

- Use fixed-width integer types (`uint32_t`, `int32_t`, etc.).
- Prefer `size_t` for sizes and counts.
- Prefer pointer + length pairs for buffers.
- Avoid passing `bool` across the boundary unless the C side includes `<stdbool.h>`.

Avoid:

- C++ standard library types (`std::string`, `std::vector`, etc.)
- exceptions as an error mechanism
- virtual interfaces, RTTI, and dynamic_cast
- passing ownership implicitly

## Struct Layout

Do not assume the compiler will pack or align structs the same way across compilers/options.

- Avoid bitfields in ABI structs.
- Prefer explicit padding fields when layout matters.
- If a packed layout is required, isolate it and wrap compiler attributes in macros.

## Ownership And Lifetime

Make ownership explicit in the API.

- If the caller retains ownership: take `const T*` or `T*` and do not store it unless documented.
- If the callee stores a pointer: document required lifetime.
- Prefer callbacks that carry an explicit context pointer.

Example callback shape:

```c
typedef void (*uart_rx_cb_t)(void* context, const uint8_t* data, size_t size);

void uart_set_rx_callback(uart_rx_cb_t cb, void* context);
```

## Error Handling Across The Boundary

- Prefer explicit status codes.
- Avoid logging inside low-level C glue; return an error and let higher layers decide.

If a project introduces a shared status enum for C and C++, keep it small and stable.

## Globals

- Avoid exposing global mutable state across the boundary.
- Prefer explicit init/deinit functions.
- If a global is required (MMIO registers, linker symbols), keep it in a narrow header and document
  its usage.

## Sections, Alignment, And Linker Symbols

Firmware often requires placing symbols into specific sections or using linker-defined symbols.

- Wrap compiler-specific attributes in macros.
- Keep section/linker concerns in a small platform header/module.

Example (linker symbol):

```c
extern uint8_t _flash_start;
extern uint8_t _flash_end;
```

## Header Hygiene

- C headers should be valid C11 and also compile as C++ when wrapped with `extern "C"` guards.
- Do not include C++ headers from C sources.
- Avoid C headers that require include-order tricks; include what you use.

## Build Flags Consistency

Keep flags consistent across translation units that share headers and ABI:

- packing/alignment options
- floating point ABI
- endianness
- `-fshort-enums`, `-fpack-struct`, or other ABI-affecting flags (avoid if possible)

If an ABI-affecting flag is necessary, document it at the module boundary.
