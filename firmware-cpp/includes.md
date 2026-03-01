# Includes

This guide is normative for include ordering and header hygiene.

Formatting of includes (indentation, spacing) is owned by `clang-format`, but ordering is not
(`SortIncludes: false`).

## Principles

- Includes should be minimal, explicit, and stable over time.
- Each header should compile on its own (when included first in an empty translation unit).
- Do not rely on transitive includes.

## `"..."` vs `<...>`

- Use `<...>` for toolchain/standard library and third-party headers.
- Use `"..."` for project headers.
- Vendor headers can use either form; pick one convention per vendor and keep it consistent.

## Include Order (MUST)

Use this order with a blank line between groups:

1) Associated header (in `.c`/`.cpp`): the matching public header for the translation unit
2) C system headers
3) C++ standard library headers
4) Third-party library headers (including RTOS, HALs treated as third-party)
5) Vendor / device headers (CMSIS, MCU device headers, register definitions)
6) Project headers (everything else)

Within a group:

- Keep a sensible, stable order (usually alphabetical by full path).
- Do not re-order unrelated includes without reason.

Example:

```cpp
#include "drivers/gpio.h"

#include <stdint.h>

#include <array>

#include <etl/vector.h>

#include <cmsis_device_header.h>

#include "platform/irq.h"
#include "util/bitops.h"
```

## Header Self-Sufficiency

- A header MUST include what it needs for its own declarations.
- A header MUST NOT require a specific include order to compile.
- Prefer including the narrowest header that provides a symbol.

## Forward Declarations

- Prefer forward declarations in headers when they reduce coupling and do not obscure ownership or
  lifetime.
- Do not forward declare standard library types.
- Do not forward declare types that must be complete in the header (by value members, `sizeof`,
  templates requiring definitions).

## C Headers In C++

- If a C library header is not C++-safe, wrap its include with `extern "C"` in a dedicated shim
  header and include that shim from C++.
- Do not sprinkle ad-hoc `extern "C"` blocks across unrelated files.

## Special Cases

- Register/device headers often depend on preprocessor configuration and include order; isolate them
  behind a small platform header when possible.
- Avoid including heavy vendor headers from widely used public headers; prefer including them in
  `.c`/`.cpp` files or narrow internal headers.
