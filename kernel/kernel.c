#include "drivers/uart/uart.h"
#include "fork.h"
#include "irq.h"
#include "printf.h"
#include "scheduler.h"
#include "timer.h"
#include "utils.h"

void process(char *array) {
  while (1) {
    for (int i = 0; i < 5; i++) {
      uart_send(array[i]);
      delay(100000);
    }
  }
}

void kernel_secondary(unsigned int pid) {
  while (1)
    ;
}

void kernel_main(int secondary_wakeup_adr) {
  int res = 0;

  uart_init(9600);
  init_printf(0, putc);
  printf("Hello, world!\r\n");
  irq_vector_init();
  timer_init();
  enable_interrupt_controller();
  enable_irq();

  res = copy_process((unsigned long)&process, (unsigned long)"12345");
  if (res != 0) {
    printf("Error while starting process 1");
    return;
  }
  res = copy_process((unsigned long)&process, (unsigned long)"abcde");
  if (res != 0) {
    printf("Error while starting process 2");
    return;
  }

  while (1) {
    schedule();
  }
}
