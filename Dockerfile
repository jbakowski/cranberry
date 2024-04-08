FROM ubuntu:22.04
RUN apt-get update && apt-get install -y gcc-aarch64-linux-gnu build-essential
RUN apt-get install -y qemu-system-aarch64
RUN apt-get install gdb-multiarch