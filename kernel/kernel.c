#include "printf.h"
#include "drivers/uart/uart.h"
#include "smp.h"
#include "utils.h"

volatile int semaphore = 0;

void kernel_secondary(unsigned int pid) {
    while(semaphore != pid) {
        delay(50);
    }
    printf("Hello, from core %d\r\n", pid);
    int el = get_el();
    printf("Exception level of core %d is: %d\r\n", pid, el);
    semaphore++;
    while(1);
}

void kernel_main(int secondary_wakeup_adr) {
    uart_init(9600);
    printf("Hello, from the main core!\r\n");
    int el = get_el();
    printf("Exception level: %d\r\n", el);
    semaphore = 1;
    // printf("Waking up core 1...\r\n");
    // wakeup_core(1, secondary_wakeup_adr);
    // printf("Waking up core 2...\r\n");
    // wakeup_core(2, secondary_wakeup_adr);
    // printf("Waking up core 3...\r\n");
    // wakeup_core(3, secondary_wakeup_adr);

    // printf("All cores running, main process exiting\r\n");
}