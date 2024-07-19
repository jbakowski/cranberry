#include "peripherals/pl011.h"
#include "drivers/uart/uart.h"
#include "peripherals/gpio.h"
#include "utils.h"

#define SYS_CLK 48000000

void uart_set_baudrate(unsigned int baudrate, unsigned short *IBRD_reg,
                       unsigned short *FBRD_reg) {
  // BAUDDIV = UART_CLK / 16*baudrate
  // C truncates integer division to zero
  // since we are dealing with positive numbers only, this is equivalent to
  // floor()
  float baudrate_divisor = SYS_CLK / (16 * baudrate);
  *IBRD_reg = (unsigned short)baudrate_divisor;
  float fractional_value = *IBRD_reg - baudrate_divisor;
  *FBRD_reg = (unsigned short)((fractional_value * 64) + 0.5);
}

void handle_uart_irq(void) {
  char c = uart_recv();
  uart_send_string("mini uart irq received\n\r");
  uart_send(c);
  uart_send_string("\n\r");
}

void uart_init(unsigned int baudrate) {
  unsigned int selector;
  unsigned short IBRD_reg, FBRD_reg;

  selector = get32(GPFSEL1);
  selector &= ~(7 << 12); // clear GPIO14
  selector |= 4 << 12;    // set ALT0 for GPIO14 (TXD0)
  selector &= ~(7 << 15); // clear GPIO15
  selector |= 4 << 15;    // set ALT0 for GPIO15 (RXD0)
  put32(GPFSEL1, selector);

  put32(GPPUD, 0);
  delay(150);
  put32(GPPUDCLK0, (1 << 14) | (1 << 15));
  delay(150);
  put32(GPPUDCLK0, 0);

  put32(PL011_CR, 0);   // disable UART
  put32(PL011_IMSC, 0); // disable UART interrupts

  uart_set_baudrate(baudrate, &IBRD_reg, &FBRD_reg);
  put32(PL011_IBRD, IBRD_reg);
  put32(PL011_FBRD, FBRD_reg);

  put32(PL011_LCRH, (3 << 5));              // set 8-bit mode
  put32(PL011_CR, 1 | (1 << 8) | (1 << 9)); // set UARTEN/TX/RX
}

void uart_send(char c) {
  while (1) {
    if (get32(PL011_FR) & (1 << 7)) {
      break;
    }
  }
  put32(PL011_DR, c);
}

char uart_recv(void) {
  while (1) {
    if (get32(PL011_FR) & (1 << 6)) {
      break;
    }
  }
  return (get32(PL011_DR) & 0xFF);
}

void uart_send_string(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    uart_send((char)str[i]);
  }
}
