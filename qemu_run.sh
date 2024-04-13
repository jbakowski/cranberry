BIN=kernel8.img
MONITOR_PORT=1235

qemu-system-aarch64 -s -S \
	-M raspi3b \
	-m 1024 \
    -cpu cortex-a53 \
	-device loader,file=./$BIN \
    -serial vc \
    -serial vc