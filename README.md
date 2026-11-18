# cranberry

A minimal bare-metal AArch64 kernel for the Raspberry Pi 3B+, runnable in QEMU. It boots, brings up the mini UART and timer, and runs a small round-robin scheduler with kernel threads and a user-mode process. The build uses a trimmed-down Linux-style Kbuild/Kconfig.

## Requirements

- `aarch64-linux-gnu-` cross toolchain
- `make`, host `gcc`
- `qemu-system-aarch64` (to run it in an emulator)
- `gdb-multiarch` (optional, for debugging)
- `libncurses-dev` (optional, only for `make menuconfig`)

On Ubuntu/Debian:

```sh
sudo apt install gcc-aarch64-linux-gnu build-essential qemu-system-arm gdb-multiarch
```

Or use the provided Docker image, which has all of the above:

```sh
docker build -t cranberry-build .
docker run --rm -v "$PWD":/workspace -w /workspace cranberry-build make all
```

## Configure

A working `.config` is already committed (timer + mini UART enabled). To change it:

```sh
make menuconfig
```

## Build

```sh
make all
```

This produces `cranberry.elf` and the raw image `kernel8.img`.

> **Known quirk:** on a clean tree the first `make all` can fail with
> `objcopy: 'cranberry.elf': No such file`, because the link and objcopy steps race.
> Just run `make all` a second time.

Use `make V=1` for verbose output, `make clean` to remove build output, and `make help` for all targets.

## Run in QEMU

Headless, with the kernel's console in your terminal:

```sh
qemu-system-aarch64 -M raspi3b -m 1024 -cpu cortex-a53 \
    -device loader,file=kernel8.img \
    -serial null -serial stdio -display none
```

The mini UART is QEMU's *second* serial port, which is why the first is set to `null`. If you switch to the PL011 UART in `menuconfig`, swap the two (`-serial stdio -serial null`). You should see `Hello, world!` followed by output from the scheduled processes. Quit with `Ctrl+A`, then `X`.

### Debugging with GDB

`qemu_run.sh` starts QEMU paused with a GDB server on port 1234 (`-s -S`) and the serial ports in the QEMU window:

```sh
./qemu_run.sh
```

Then, in another terminal:

```sh
gdb-multiarch cranberry.elf -ex "target remote :1234"
```

and `continue` to boot.

## Run on a Raspberry Pi 3B+

1. Take an SD card with a FAT32 boot partition containing the standard Raspberry Pi firmware files (`bootcode.bin`, `start.elf`, `fixup.dat`).
2. Copy `kernel8.img` and `src/config.txt` onto it.
3. Connect a USB-to-serial adapter to the mini UART on GPIO 14 (TX) / GPIO 15 (RX) and GND, and open it at **9600 baud**.
4. Power on the Pi.

## CI

`.github/workflows/buildImage.yml` builds the image in the Docker container on every push and pull request to `main`, and uploads `kernel8.img` as a build artifact.
