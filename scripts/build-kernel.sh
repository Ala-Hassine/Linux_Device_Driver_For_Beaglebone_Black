#!/bin/bash

# Build Linux kernel for BeagleBone Black
KERNEL_VERSION="5.10"
KERNEL_SOURCE="linux-$KERNEL_VERSION"
CONFIG_FILE="configs/bbb-kernel-config"

echo "Building Linux kernel $KERNEL_VERSION for BeagleBone Black"

# Source environment
source ../env-setup.sh

# Download kernel if not present
if [ ! -d "$KERNEL_SOURCE" ]; then
    echo "Downloading kernel source..."
    wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-$KERNEL_VERSION.tar.xz
    tar -xf linux-$KERNEL_VERSION.tar.xz
fi

cd $KERNEL_SOURCE

# Apply configuration
cp ../$CONFIG_FILE .config

# Build kernel
make -j4 zImage
make -j4 modules
make -j4 dtbs

echo "Kernel build complete!"
echo "Output files:"
echo " - arch/arm/boot/zImage"
echo " - arch/arm/boot/dts/am335x-boneblack.dtb"
