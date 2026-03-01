#ifndef EXAMPLES_UART_C_API_H
#define EXAMPLES_UART_C_API_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART_ENABLE_DMA 0

typedef struct {
  uintptr_t base_addr;
} UART;

typedef struct {
  uint32_t baud_hz;
} UARTConfig;

typedef void (*UARTRxCallback)(void *context, const uint8_t *data, size_t size);

// ISR-safe: no
// Blocking: no
// Allocates: no
bool uart_init(UART *uart, const UARTConfig *config);

// ISR-safe: no
// Blocking: yes (may wait for space)
// Allocates: no
size_t uart_write(UART *uart, const uint8_t *data, size_t size);

// Callback may be invoked from ISR context (implementation-defined).
void uart_set_rx_callback(UART *uart, UARTRxCallback cb, void *context);

#ifdef __cplusplus
}
#endif

#endif // EXAMPLES_UART_C_API_H
