#include "mini_uart.h"
#include "smp.h"

volatile int busy;

void kernel_secondary(unsigned int pid) {
    while(busy) {
        delay(10);
    }
    busy = 1;
    uart_send_string("Hello, from core: ");
    uart_send(pid + '0');
    uart_send_string("\r\n");
    busy = 0;
    while(1);
}

void kernel_main(int secondary_wakeup_adr) {
    uart_init(9600);
    uart_send_string("Hello world, from the main core!\r\n");

    uart_send_string("Waking up core 1...\r\n");
    wakeup_core(1, secondary_wakeup_adr);
    uart_send_string("Waking up core 2...\r\n");
    wakeup_core(2, secondary_wakeup_adr);
    uart_send_string("Waking up core 3...\r\n");
    wakeup_core(3, secondary_wakeup_adr);

    uart_send_string("All cores running, main process exiting\r\n");
}