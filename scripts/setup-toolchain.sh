#!/bin/bash

# BeagleBone Black Cross Compilation Toolchain Setup
TOOLCHAIN_URL="https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz"
TOOLCHAIN_DIR="toolchain"

echo "Setting up cross-compilation toolchain for BeagleBone Black ..."

# Create toolchain directory
mkdir -p $TOOLCHAIN_DIR
cd $TOOLCHAIN_DIR

# Download toolchain
echo "Downloading toolchain ..."
wget $TOOLCHAIN_URL

# Extract toolchain
echo "Extracting toolchain..."
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz

# Set permissions
chmod -R 755 gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf

echo "Toolchain setup complete!"
echo "Add to your PATH: export PATH=\$PATH:$(pwd)/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin"
