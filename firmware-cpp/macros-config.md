# Macros And Configuration

This guide is normative for preprocessor usage, compile-time configuration, and register-access
macros in firmware.

## Principles

- Prefer C++ language features over macros when possible.
- Keep macros simple, local, and side-effect free.
- Keep build-time configuration explicit and discoverable.
- Make hardware register access correct and race-free.

## Prefer Language Features

- Prefer `constexpr`, `enum class`, `inline` functions, and templates over macros in C++.
- Prefer `static inline` functions over function-like macros in C.

Use macros for:

- include guards
- conditional compilation
- compiler attributes and pragmas (wrapped behind macros)
- register definitions and bit masks

## Macro Naming

- Macros MUST be `UPPER_CASE`.
- Macros MUST NOT start with `_`.
- Prefer a module prefix to avoid collisions (recommended): `UART_`, `SPI_`, `PLATFORM_`.

## Include Guards

Use classic include guards in C/C headers.

```c
#ifndef DRIVERS_UART_H
#define DRIVERS_UART_H

// declarations

#endif  // DRIVERS_UART_H
```

## Configuration Placement

Prefer one of these patterns:

- A single project config header (e.g. `project_config.h`) included by most modules.
- A per-module config header (e.g. `uart_config.h`) included by that module.

Avoid:

- scattering `-D` flags across build files without a corresponding header location
- defining feature macros in random `.c`/`.cpp` files

## Feature Flags

Prefer boolean feature macros with explicit values `0`/`1`.

```c
#define UART_ENABLE_DMA 1
```

Preferred checks:

- Use `#if UART_ENABLE_DMA` for boolean macros.
- Use `#if defined(UART_ENABLE_DMA)` when the macro is a presence-only switch.

Provide defaults in a single place:

```c
#ifndef UART_ENABLE_DMA
#define UART_ENABLE_DMA 0
#endif
```

## Macro Safety

- Parenthesize macro parameters and expansions.
- Do not pass expressions with side effects into macros.
- Avoid function-like macros that evaluate parameters multiple times.

Example:

```c
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
```

Prefer an inline function when possible.

## Compiler Attributes And Pragmas

Wrap compiler-specific attributes behind macros in a small platform header.

Examples:

```c
#if defined(__GNUC__)
#define ATTR_PACKED __attribute__((packed))
#define ATTR_ALIGNED(x) __attribute__((aligned(x)))
#else
#define ATTR_PACKED
#define ATTR_ALIGNED(x)
#endif
```

## Register Access

Register access is performance- and correctness-critical.

- MMIO pointers/references are typically `volatile`.
- Prefer typed register structs provided by the vendor when available.
- Avoid read-modify-write races on shared registers; prefer atomic set/clear registers if provided,
  otherwise protect with a critical section.

If you use address macros, keep them narrow and explicit.

Example (C, illustrative):

```c
#define REG32(addr) (*(volatile uint32_t*)(addr))
#define UART0_BASE 0x40001000u

#define UART_CR_OFFSET 0x00u
#define UART_CR_RX_ENABLE (1u << 0)

static inline void uart0_enable_rx(void) {
  REG32(UART0_BASE + UART_CR_OFFSET) |= UART_CR_RX_ENABLE;
}
```

Notes:

- Prefer `uintptr_t` for address arithmetic when mixing pointers and integers.
- Keep register macros and offsets together to reduce mistakes.

## Conditional Compilation Hygiene

- Keep `#if` trees shallow.
- Prefer to isolate platform-specific code in a platform layer rather than sprinkling `#ifdef`.
- Do not use `#ifdef` as a substitute for module boundaries.

## Generated And Vendor Headers

- Do not edit generated config headers by hand.
- Keep vendor macro sets contained; wrap them behind a small project header when practical.
