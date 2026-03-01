# Naming

This guide is normative for identifier, file, and macro naming.

## Reserved Identifiers

Avoid names reserved by the C/C++ standards and common toolchains.

- Never use identifiers beginning with `__`.
- Never use identifiers beginning with `_` followed by an uppercase letter.
- Leading `_` is permitted only for class/struct private members and private methods in this style.
- Do not use leading `_` for globals (including namespaces), macros, or file-scope symbols.

Examples:

```cpp
class SPIBus {
 public:
  void init();

 private:
  void _configure_clock();
  int _error_count;
};
```

## Files

- Use `lower_snake_case`.
- C headers: `.h`; C sources: `.c`.
- C++ headers: `.h` (unless a project already uses `.hpp`); C++ sources: `.cpp`.

Examples:

```text
gpio_driver.cpp
gpio_driver.h
spi_bus.cpp
spi_bus.h
```

## Types

- Classes: `PascalCase`.
- Structs: `PascalCase`.
- Enums: `PascalCase`.
- Type aliases: `PascalCase`.

Examples:

```cpp
struct PacketHeader {
  uint16_t length;
  uint16_t type;
};

class FlashDriver {
 public:
  void init();
};

enum class ResetReason {
  PowerOn,
  Watchdog,
  Software,
};
```

## Acronyms

- For domain-standard acronyms in type names, prefer all-caps: `SPIConfig`, `SPIDriver`, `UART`.
- For non-type identifiers, follow the general casing rules in this guide.

## Functions And Methods

- Use `snake_case` for free functions, member functions, and methods.
- Private methods of a class/struct MUST use a leading underscore: `_do_thing()`.
- Avoid abbreviations unless they are domain-standard in firmware (e.g. `spi`, `i2c`, `uart`, `irq`).

Examples:

```cpp
void enable_irq();

class UART {
 public:
  void write_bytes(const uint8_t* data, size_t size);

 private:
  void _kick_tx();
};
```

## Variables

- Use `snake_case`.
- Private data members MUST use a leading underscore: `_name`.
- Do not encode type in names (no pointer-specific suffix rules). Name by meaning/role.

Notes:

- Private pointer members follow the same rule: `_buffer`, `_next`, `_callback`.
- If you need to disambiguate ownership, prefer API design and types over naming.

Examples:

```cpp
class RingBuffer {
 public:
  void reset();

 private:
  uint8_t* _data;
  size_t _size_bytes;
  size_t _head;
  size_t _tail;
};
```

## Constants

### C++

- Prefer `static constexpr` for class-scope constants.
- Prefer `inline constexpr` for namespace-scope constants in headers (C++17).
- Prefer `constexpr` variables over preprocessor macros.
- Use `UPPER_CASE` names.

Examples:

```cpp
inline constexpr uint32_t CPU_CLOCK_HZ = 48'000'000u;

class ADC {
 public:
  static constexpr uint16_t MAX_COUNTS = 4095;
};
```

### C

- Use `#define` for constants.
- Use `UPPER_CASE` names.

Examples:

```c
#define CPU_CLOCK_HZ 48000000u
#define ADC_MAX_COUNTS 4095u
```

## Macros

- Use `UPPER_CASE`.
- Prefix macros with a short module tag to avoid collisions (recommended): `GPIO_`, `SPI_`,
  `PLATFORM_`.
- Do not use leading `_`.

## Namespaces

- Use `lower_snake_case`.
- Keep namespaces short and meaningful; avoid deep nesting in firmware code.

Example:

```cpp
namespace platform {
void init_clocks();
}
```

## Template Parameters

- Use `PascalCase` with a leading `T` for type parameters: `T`, `TValue`, `TClock`.
- Use `snake_case` for non-type template parameters: `size`, `capacity`.

Example:

```cpp
template <typename TValue, size_t capacity>
class FixedQueue {
 public:
  void push(const TValue& value);
};
```

## Units And Booleans

- Include units in names where ambiguity is likely: `_timeout_ms`, `_rate_hz`, `_size_bytes`.
- Use boolean prefixes: `is_`, `has_`, `enable_`, `should_`.

Examples:

```cpp
bool is_ready();
bool has_error();
void enable_crc(bool enable);
```
