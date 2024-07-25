#include "arch/arm64/sysregs.h"
#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    mrs x0, mpidr_el1                   // check core id
    and x0, x0,#0xFF                    // main core will initialize memory
    mov x19, x0
    cbz x0, el1_main_entry
    b el1_secondary_entry

proc_hang:
    b proc_hang

set_el1:
    ldr x0, =SCTLR_VALUE_MMU_DISABLED   // disable the MMU
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE                  // disable the hypervisor
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE                  // set EL2 execution state to AArch64
    msr scr_el3, x0                     // lower exception levels are non-secure

    ldr x0, =SPSR_VALUE                 // store the saved program status register
    msr spsr_el3, x0

    ldr x0, =CPACR_EL1_FPEN_ENABLE     // disable trapping of floating-point instructions
    msr cpacr_el1, x0
    ret

el1_main_entry:
    bl set_el1
    adr x0, main_init
    msr elr_el3, x0
    eret

el1_secondary_entry:
    bl set_el1
    adr x0, secondary_init
    msr elr_el3, x0
    eret

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
