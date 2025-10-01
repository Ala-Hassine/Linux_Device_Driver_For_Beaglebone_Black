# Top-level Makefile for BBB kernel modules
obj-m += bbb_hello/
obj-m += bbb_uart/
obj-m += bbb_i2c/
obj-m += bbb_spi/

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
