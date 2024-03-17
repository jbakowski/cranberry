#ifndef	_MINI_UART_H
#define	_MINI_UART_H

// mini-UARTs is clocked by a system clock
// frequency is 250 MHz (and unchangeable, for now)

void uart_init (int baudrate);
char uart_recv (void);
void uart_send (char c);
void uart_send_string (char* str);

#endif  /*_MINI_UART_H */