#ifndef _UART_H
#define _UART_H

void uart_init(unsigned int baudrate);
char uart_recv(void);
void uart_send(char c);
void uart_send_string(char *str);
void handle_uart_irq(void);
void putc(void *p, char c);

#endif /* _UART_H */
