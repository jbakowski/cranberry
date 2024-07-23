#include "drivers/uart/uart.h"
#include "fork.h"
#include "irq.h"
#include "printf.h"
#include "scheduler.h"
#include "sys.h"
#include "timer.h"
#include "utils.h"

void user_process1(char *array) {
  char buf[2] = {0};
  while (1) {
    for (int i = 0; i < 5; i++) {
      buf[0] = array[i];
      call_sys_write(buf);
      delay(100000);
    }
  }
}

void user_process(void) {
  char buf[30] = {0};
  tfp_sprintf(buf, "User process started \n\r");
  call_sys_write(buf);
  unsigned long stack = call_sys_malloc();
  if (stack < 0) {
    printf("Error while allocating stack for process 1\n\r");
    return;
  }
  int err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"1234",
                           stack, 5);
  if (err < 0) {
    printf("Error while cloning process 1\n\r");
    return;
  }
  stack = call_sys_malloc();
  if (stack < 0) {
    printf("Error while allocating stack for process 1\n\r");
    return;
  }
  err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"abcd",
                       stack, 3);
  if (err < 0) {
    printf("Error while cloning process 2\n\r");
    return;
  }
  call_sys_exit();
}

void kernel_process(void) {
  printf("Kernel process started. EL %d\r\n", get_el());
  int err = move_to_user_mode((unsigned long)&user_process);
  if (err < 0) {
    printf("Error while moving process to user mode\n\r");
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

  res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0, 0, 3);
  if (res < 0) {
    printf("Error while starting kernel process");
    return;
  }

  while (1) {
    schedule();
  }
}
