#!/bin/bash

# Enable USB Ethernet on BeagleBone Black
echo "Setting up USB Ethernet gadget ..."

# Load USB gadget modules
modprobe libcomposite
modprobe usb_f_rndis
modprobe usb_f_ecm

# Create gadget configuration
cd /sys/kernel/config/usb_gadget/
mkdir -p g1
cd g1

echo "0x1234" > idVendor
echo "0x5678" > idProduct
echo "0x0100" > bcdDevice
echo "0x0200" > bcdUSB

mkdir -p strings/0x409
echo "0000000000000000" > strings/0x409/serialnumber
echo "BeagleBone" > strings/0x409/manufacturer
echo "BeagleBone Black" > strings/0x409/product

mkdir -p configs/c.1/strings/0x409
echo "RNDIS" > configs/c.1/strings/0x409/configuration
echo "250" > configs/c.1/MaxPower

# Add RNDIS function
mkdir -p functions/rndis.usb0
ln -s functions/rndis.usb0 configs/c.1/

# Enable the gadget
echo "musb-hdrc.1.auto" > UDC

echo "USB Ethernet gadget configured"
echo "Host IP: 192.168.7.1"
echo "BeagleBone IP: 192.168.7.2"
