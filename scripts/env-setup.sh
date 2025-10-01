#!/bin/bash

# Environment setup for BeagleBone Black development
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=$PATH:$(pwd)/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin

echo "Environment setup for ARM cross-compilation"