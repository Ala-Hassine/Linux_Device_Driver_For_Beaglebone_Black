#!/bin/bash

# Compile Device Tree Overlays
echo "Compiling Device Tree Overlays ..."

# Source environment
source ../env-setup.sh

# Compile each overlay
for dts in *.dts; do
    dtb=${dts%.dts}.dtbo
    echo "Compiling $dts to $dtb"
    dtc -O dtb -o $dtb -b 0 -@ $dts
done

echo "Device Tree Compilation Complete"
