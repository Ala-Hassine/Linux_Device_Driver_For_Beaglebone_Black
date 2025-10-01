# BeagleBone Black Boot Sequence

## Boot Process Overview
1. **ROM Bootloader** - Executes from SoC internal ROM
2. **SPL (Secondary Program Loader)** - Loads from MMC/FAT partition
3. **U-Boot** - Main bootloader
4. **Linux Kernel** - Loaded from MMC
5. **Root Filesystem** - Mounted from eMMC/SD card

## Boot Components
- **MLO**: X-Loader (SPL)
- **u-boot.img**: U-Boot bootloader  
- **zImage**: Compressed Linux kernel
- **dtb**: Device Tree Blob
- **rootfs**: Root filesystem

## Boot Configuration
- Boot order configurable via SYSBOOT pins
- Default: MMC0 (eMMC), then MMC1 (SD card)
- Serial console: 115200 baud, 8N1