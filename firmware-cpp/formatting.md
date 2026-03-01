# Formatting

`.clang-format` is normative for all formatting decisions.

If this document conflicts with `.clang-format`, `.clang-format` wins.

## Apply clang-format

- Format on save in your editor using `clang-format` with `-style=file`.
- Format files you touch as part of a change.

CLI examples:

```sh
clang-format -style=file -i path/to/file.c
clang-format -style=file -i path/to/file.cpp
clang-format -style=file -i path/to/file.h
```

Check-only examples (CI-friendly; fails if reformatting would change the file):

```sh
clang-format -style=file --dry-run -Werror path/to/file.cpp
```

## What Not To Hand-Format

- Do not align code with manual spacing; express alignment via structure.
- Do not introduce tabs; `.clang-format` enforces spaces.
- Do not fight wrapping/bracing decisions; accept the formatter output.

## Disabling Formatting

Use these markers sparingly and keep the disabled region as small as practical.

```cpp
// clang-format off
// (formatter will not change this region)
// clang-format on
```

Prefer to disable formatting only for:

- Tables meant for human scanning (bitfield maps, protocol byte layouts)
- Code that must match an external spec or datasheet verbatim
- Workarounds for known `clang-format` limitations

## Comments

This style uses a `CommentPragmas` setting that prevents `clang-format` from reflowing/splitting
comments.

- Keep comment lines readable and wrap manually when needed.
- Avoid long, single-line comments that exceed the column limit.

Comment content guidance:

- Use comments to explain constraints, invariants, and "why" (not what the code obviously does).
- Use all-caps acronyms in comments when they are domain-standard: `SPI`, `UART`, `IRQ`, `DMA`.
- For a small but non-obvious function, a short comment at the function can explain intent.
- For a large/long function, prefer short comments per chunk/section of code inside the function.
- If the codebase already uses a doc-comment system (Doxygen, etc.), follow that convention. Do not
  introduce a new one just for this guide.

## Generated / Vendor Code

- Do not reformat generated or vendor-managed files unless you own the generator/template.
- Prefer to keep generated sources in clearly named directories (for example: `generated/`,
  `vendor/`) so tooling can exclude them from formatting.
- If formatting must be disabled inside an otherwise hand-written file, use
  `// clang-format off` / `// clang-format on` around the generated block.
