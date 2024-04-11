#BIN=build/kernel8.elf
BIN=kernel8.img
MONITOR_PORT=1235

#qemu-system-aarch64 \
qemu-system-aarch64 -s -S \
	-M raspi3b \
	-m 1024 \
    -cpu cortex-a53 \
	-kernel ${BIN} \
    -serial vc \
    -serial vc \
    -serial vc