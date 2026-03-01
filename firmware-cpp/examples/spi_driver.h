#ifndef EXAMPLES_SPI_DRIVER_H
#define EXAMPLES_SPI_DRIVER_H

#include <cstddef>
#include <cstdint>

inline constexpr uint32_t SPI_DEFAULT_BAUD_HZ = 1'000'000u;

struct SPIConfig {
  uint32_t baud_hz = SPI_DEFAULT_BAUD_HZ;
  bool cpol = false;
  bool cpha = false;
};

class SPIDriver {
public:
  SPIDriver() = default;

  // ISR-safe: no
  // Blocking: may wait on peripheral
  // Allocates: no
  [[nodiscard]] bool init(uintptr_t base_addr, const SPIConfig &config);

  // ISR-safe: no
  // Blocking: yes
  // Allocates: no
  [[nodiscard]] bool transfer(const uint8_t *tx, uint8_t *rx, size_t size);

private:
  bool _configure(const SPIConfig &config);

  uintptr_t _base_addr = 0u;
  SPIConfig _config;
};

#endif // EXAMPLES_SPI_DRIVER_H
