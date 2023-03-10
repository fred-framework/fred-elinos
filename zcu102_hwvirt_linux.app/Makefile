#----------------------------------------------------------------------
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# Project:     Elinos -- Embedded Linux OS
# Description: Project Makefile Template
# This file:   Makefile
# Purpose:     Build a project
#----------------------------------------------------------------------

# only allow ourselves to run when ELINOS.sh is sourced:
ifneq "$(shell pwd)" "$(ELINOS_PROJECT)"
.PHONY: *
*:
	@echo "*** The project environment is not correctly set ***"; \
	echo "Maybe you just moved project to another directory,"; \
	echo "some ELinOS part has been reinstalled or"; \
	echo "ELINOS.sh has not been sourced"; \
	echo; \
	if [ ! -e ./configure ]; then \
		echo "You need to regenerate project configuration using: "; \
		echo "    sh# /opt/elinos*/bin/elinos-configure -n"; \
	else \
		echo "Please regenerate ELINOS.sh first:"; \
		echo "    sh# ./configure -n"; \
	fi; \
	echo "After regenerating ELINOS.sh, you need to source it:";\
	echo "    sh# . ELINOS.sh"
	exit 1
else

include $(ELINOS_PREFIX)/scripts/system.mk

.PHONY: boot clean distclean filesystem help install burn diffconfig
.DEFAULT: help

### common make targets ###
boot: boot_common
	#echo "DTB: Generating device-tree $(DTB_FILE) with symbols, required to apply overlays"
	#make -C $(ELINOS_PROJECT)/linux ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -j${ELINOS_MAKE_NUM_JOBS} $(DTB_FILE) DTC_FLAGS=-@
	#cp $(ELINOS_PROJECT)/linux/arch/arm64/boot/dts/$(DTB_FILE) $(ELINOS_PROJECT)/boot/linux.dtb
	#echo "FPGA-MGR: replacing the fpga-manager by the one for FRED"
	#dtc -O dts -o $(ELINOS_PROJECT)/boot/linux.dts -b 0 -@ $(ELINOS_PROJECT)/boot/linux.dtb
	#sed -i 's/zynqmp-pcap-fpga/zynqmp-pcap-fpga-fmod/g' $(ELINOS_PROJECT)/boot/linux.dts
	#dtc -O dtb -o $(ELINOS_PROJECT)/boot/linux.dtb -b 0 -@ $(ELINOS_PROJECT)/boot/linux.dts
	cp $(ELINOS_PROJECT)/linux-modif.dtb $(ELINOS_PROJECT)/boot/linux.dtb

clean: clean_common

distclean: distclean_common

filesystem: filesystem_common

help: help_common

install: install_common
# change the partition names according to your sd-card. In this example, ROOT is the boot, and ROOT1 is rootfs
burn:
	cp ../pool/images/hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb /media/ubuntu/ROOT
	sudo tar -zxvf ./rootfs.tgz -C /media/ubuntu/ROOT1/
	sync
	umount /media/ubuntu/ROOT
	umount /media/ubuntu/ROOT1

diffconfig:
	echo "TBD !!!"

# endif (ELINOS_PROJECT)
endif
