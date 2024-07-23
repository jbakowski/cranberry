#include "fork.h"
#include "mm.h"
#include "printf.h"
#include "scheduler.h"
#include "utils.h"

void sys_write(char *buf) { printf(buf); }

int sys_clone(unsigned long stack, unsigned long priority) {
  return copy_process(0, 0, 0, stack, priority);
}

unsigned long sys_malloc(void) {
  unsigned long addr = get_free_page();
  if (!addr) {
    return -1;
  }
  return addr;
}

void sys_exit(void) { exit_process(); }

void *const sys_call_table[] = {sys_write, sys_clone, sys_malloc, sys_exit};
