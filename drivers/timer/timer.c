#include "timer.h"
#include "peripherals/timer.h"
#include "printf.h"
#include "scheduler.h"
#include "utils.h"

const unsigned int interval = 200000;
unsigned int curVal = 0;

void timer_init(void) {
  curVal = get32(TIMER_CLO);
  curVal += interval;
  put32(TIMER_C1, curVal);
}

void handle_timer_irq(void) {
  curVal += interval;
  put32(TIMER_C1, curVal);
  put32(TIMER_CS, TIMER_CS_M1);
  timer_tick();
}

void local_timer_init(void) {
  put32(LTIMER_CTRL, (interval | LTIMER_CTRL_VAL));
}

void handle_local_timer_irq(void) { put32(LTIMER_CLR, LTIMER_CLR_ACK); }
