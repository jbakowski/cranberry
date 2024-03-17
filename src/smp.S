# cores, other than the main core, spin on "raspi_spintables" 
# to wake up the cores, we need to do 2 things:
# a) load starting address at the correct place in "raspi_spintables"
# b) use SEV instruction to wake up the core
# addr = 0xd8 + mpidr_el1 * 8
# arguments:
# x0 -> mpidr_el1 value
# x1 -> starting address
.globl wakeup_core
wakeup_core:
	mov x2, 0xd8
	str x1, [x2, x0, LSL #3]
	sev
	ret