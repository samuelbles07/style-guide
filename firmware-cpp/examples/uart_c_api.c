#include "uart_c_api.h"

static UartRxCallback g_rx_cb;
static void *g_rx_context;

bool uart_init(Uart *uart, const UartConfig *config) {
  if ((uart == NULL) || (config == NULL)) {
    return false;
  }
  if (uart->base_addr == 0u) {
    return false;
  }
  if (config->baud_hz == 0u) {
    return false;
  }

#if UART_ENABLE_DMA
  // DMA setup would go here.
#endif

  return true;
}

size_t uart_write(Uart *uart, const uint8_t *data, size_t size) {
  if ((uart == NULL) || (data == NULL)) {
    return 0u;
  }
  if (size == 0u) {
    return 0u;
  }

  // Example placeholder only.
  // Real implementations would write to a FIFO/register and may block.
  return size;
}

void uart_set_rx_callback(Uart *uart, UartRxCallback cb, void *context) {
  (void)uart;
  g_rx_cb = cb;
  g_rx_context = context;
}
