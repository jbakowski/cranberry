#include "printf.h"
#include "drivers/uart/uart.h"
#include "timer.h"
#include "irq.h"
#include "smp.h"
#include "utils.h"

volatile int semaphore = 0;

void kernel_secondary(unsigned int pid) {
    // while(semaphore != pid) {
    //     delay(50);
    // }
    // printf("Hello, from core %d\r\n", pid);
    // int el = get_el();
    // printf("Exception level of core %d is: %d\r\n", pid, el);
    // semaphore++;
    while(1);
}

void kernel_main(int secondary_wakeup_adr) {
    uart_init(9600);
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();

    while(1) {
        uart_send(uart_recv());
    }
}