# Firmware C/C++ Style Guide

This folder is a modular style guide intended for embedded/firmware C and modern C++.

## Core Principle

`clang-format` is the single source of truth for formatting.

If any written guidance conflicts with `.clang-format`, `.clang-format` wins.

## Baseline Assumptions

- C++: C++17, `-fno-exceptions`, `-fno-rtti` (error-handling is explicit; no thrown exceptions)
- C: C11 (mainly for interop, low-level, or vendor code)
- Memory: heap allocation is allowed when it is the least-bad option; prefer static/stack/arena patterns

## Files

- Formatting (normative): `firmware-cpp/.clang-format`
- Entry point (this file): `firmware-cpp/README.md`

Planned modular docs (one topic per file):

- `firmware-cpp/formatting.md` (workflow; formatter disable regions; generated code policy)
- `firmware-cpp/includes.md` (include grouping/order; header self-sufficiency; since includes are not sorted)
- `firmware-cpp/naming.md` (identifiers; files; namespaces; macros)
- `firmware-cpp/errors.md` (no-exception patterns; status/expected; asserts/logging)
- `firmware-cpp/memory.md` (ownership; heap constraints; arenas; lifetimes)
- `firmware-cpp/isr-concurrency.md` (ISR-safe rules; atomics/volatile; critical sections)
- `firmware-cpp/c-interop.md` (`extern "C"`; ABI; headers; build flags)
- `firmware-cpp/macros-config.md` (feature flags; config headers; register access patterns)
- `firmware-cpp/modules-apis.md` (layering; dependencies; public/private headers)
- `firmware-cpp/testing.md` (unit tests; fakes; host vs target builds)

Examples (non-normative; kept tiny for retrieval):

- `firmware-cpp/examples/README.md`

## How To Apply Formatting

- Use the repository-local style file: `.clang-format`.
- Prefer formatting at save in your editor (Neovim should be configured to use `-style=file`).

Example CLI usage:

```sh
clang-format -style=file -i path/to/file.cpp
```

## Retrieval Notes (for agents)

- Use this file (`firmware-cpp/README.md`) to find the right sub-guide.
- Treat `.clang-format` as normative for all whitespace/braces/wrapping decisions.
