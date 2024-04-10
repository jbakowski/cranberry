#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    mrs    x0, mpidr_el1
    and    x0, x0,#0xFF          // check processor id
    mov    x19, x0
    cbz    x0, main_init         // main core inits memory
    b      secondary_init        // start up rest of the cores

proc_hang:
    b proc_hang

secondary_init:
    mrs x19, mpidr_el1
    and x19, x19,#0xFF
    mov x0, SECONDARY_STACK_START
    mul x2, x0, x19             // calculate SP for each core
    mov sp, x2
    mov x1, #30000
    mul x0, x1, x19
    bl delay
    mov x0, x19
    bl kernel_secondary

main_init:
    adr x0, bss_begin           // init memory
    adr x1, bss_end
    sub x1, x1, x0
    bl memzero

    mov sp, #LOW_MEMORY
    adr x0, secondary_init
    bl kernel_main
    b proc_hang
