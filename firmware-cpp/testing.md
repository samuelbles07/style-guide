# Testing

This guide is intentionally small and project-agnostic.

## Principles

- Prefer tests that run on the host (fast feedback) and a smaller set that runs on target.
- Separate pure logic from hardware I/O so logic can be tested without a board.
- Keep tests deterministic (no timing races, no hidden dependencies on global state).
- Do not introduce a new test framework or test folder structure if the codebase does not already
  have testing in place. If adding tests requires new tooling, ask first.

## Where Tests Live

Follow the existing codebase convention.

Common patterns:

- `tests/`
- `test/`
- `unit_tests/`
- `firmware/tests/`

If no convention exists, choose a single top-level folder (e.g. `tests/`) and keep it consistent.

## Test Types

- Unit tests: pure functions, state machines, parsers, protocol logic.
- Integration tests: module boundaries with fakes/stubs for hardware.
- Hardware-in-the-loop (HIL): a small set to validate real peripherals/timing.

## Fakes And Seams

- Prefer dependency injection (pass interfaces/handles) rather than hard-coding globals.
- For C modules, use function pointers or a small vtable struct to inject behavior.
- For C++ modules, inject dependencies by reference/pointer; avoid needing a mocking framework.

## What Not To Test

- Do not unit-test vendor/HAL code unless you own it.
- Avoid brittle tests that assert exact register sequences unless the module contract requires it.
