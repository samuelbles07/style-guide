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

## Generated / Vendor Code

- Do not reformat generated or vendor-managed files unless you own the generator/template.
- Prefer to keep generated sources in clearly named directories (for example: `generated/`,
  `vendor/`) so tooling can exclude them from formatting.
- If formatting must be disabled inside an otherwise hand-written file, use
  `// clang-format off` / `// clang-format on` around the generated block.
