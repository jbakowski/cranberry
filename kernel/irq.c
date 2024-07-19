#include "peripherals/irq.h"
#include "drivers/uart/uart.h"
#include "entry.h"
#include "printf.h"
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
  printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr,
         address);
}

void handle_irq(void) {
  unsigned int irq = get32(IRQ_PENDING_1);
  // wrap the handler in a loop to handle
  // multiple interrupts during a single
  // handler execution
  while (irq != 0) {
    switch (irq) {
    case (SYSTEM_TIMER_IRQ_1):
      handle_timer_irq();
      break;
    case (LOCAL_TIMER_IRQ):
      handle_local_timer_irq();
      break;
    default:
      printf("Unknown pending irq: %x\r\n", irq);
      break;
    }
    irq = get32(IRQ_PENDING_1);
  }
}
