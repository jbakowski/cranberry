#include "peripherals/irq.h"
#include "drivers/uart/uart.h"
#include "entry.h"
#include "printf.h"
#include "scheduler.h"
#include "timer.h"
#include "utils.h"

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",    "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",  "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",  "ERROR_INVALID_EL0_32"};

void enable_interrupt_controller(void) {
  put32(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1);
}

void show_invalid_entry_message(int type, unsigned long esr,
                                unsigned long address) {
  printf("Type: %d\r\n");
  printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr,
         address);
}

void show_data_abort_message(unsigned long esr, unsigned long address) {
  printf("Data abort exception, ESR:%x, fault address: %x\r\n", esr, address);
  printf("terminating faulting process...\r\n");
  exit_process();
}

void handle_irq(void) {
  unsigned int irq = get32(IRQ_PENDING_1);

  while (irq) {
    if (irq & SYSTEM_TIMER_IRQ_1) {
      handle_timer_irq();
      irq &= ~SYSTEM_TIMER_IRQ_1;
    } else if (irq & AUX_IRQ) {
      handle_uart_irq();
      irq &= ~AUX_IRQ;
    } else {
      printf("Unknown pending irq: %x\r\n", irq);
      irq = 0;
    }
  }
}
