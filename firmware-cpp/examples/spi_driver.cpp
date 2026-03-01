#include "spi_driver.h"

#include <stdint.h>

#include <array>

bool SPIDriver::init(uintptr_t base_addr, const SPIConfig &config) {
  _base_addr = base_addr;
  _config = config;
  return _configure(_config);
}

bool SPIDriver::transfer(const uint8_t *tx, uint8_t *rx, size_t size) {
  if (_base_addr == 0u) {
    return false;
  }
  if ((tx == nullptr) || (rx == nullptr)) {
    return false;
  }
  if (size == 0u) {
    return true;
  }

  // Example placeholder only.
  // Real implementations would write/read registers at _base_addr.
  for (size_t i = 0; i < size; ++i) {
    rx[i] = tx[i];
  }
  return true;
}

bool SPIDriver::_configure(const SPIConfig &config) {
  (void)config;

  // Example placeholder only.
  // A real driver would program CPOL/CPHA/baud and validate capabilities.
  constexpr std::array<uint32_t, 2> SUPPORTED_BAUDS_HZ = {1'000'000u, 2'000'000u};
  (void)SUPPORTED_BAUDS_HZ;
  return true;
}
