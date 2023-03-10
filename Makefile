
.PHONY: all install distclean
.DEFAULT: help

# prepare the linux image w FRED
all:
	make -C fred-lib.app install
	make -C fred-server.app install
	make -C zcu102_hwvirt_linux.app boot
	make -C zcu102_hwvirt_pikeos.int all

# this will write in the SD card
install:
	make -C zcu102_hwvirt_pikeos.int install

distclean:
	make -C fred-lib.app distclean
	make -C fred-server.app distclean
	make -C zcu102_hwvirt_linux.app distclean
	make -C zcu102_hwvirt_pikeos.int distclean
