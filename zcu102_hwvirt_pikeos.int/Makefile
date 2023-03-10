#
#----------------------------------------------------------------------
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# Project:     PikeOS
# Description: PikeOS Integration project Makefile
# This file:   Makefile
# Purpose:     Build a project
#----------------------------------------------------------------------

-include project.mk

# Only allow ourselves to run when PikeOS.sh is sourced or project.mk exist:
ifneq "$(shell pwd)" "$(PIKEOS_PROJECT)"
$(error PIKEOS_PROJECT is not correctly set, please configure your project)
else


include $(PIKEOS_POOL_DIR)/makeinc/help.mk
include $(PIKEOS_POOL_DIR)/makeinc/variables.mk
include $(PIKEOS_POOL_DIR)/makeinc/verbose.mk
include $(PIKEOS_POOL_DIR)/makeinc/buildid.mk
include $(PIKEOS_POOL_DIR)/$(PIKEOS_PROJECT_TYPE)/makeinc/personality.mk

.PHONY: boot run
.PRECIOUS: vmit.mod pikeos.boot

ifeq ($(PIKEOS_BOOT_IMAGE),)
# Replace all spaces by _ in the image name
empty :=
space := $(empty) $(empty)
PIKEOS_BOOT_IMAGE := $(subst $(space),_,$(PIKEOS_PROJECT_NAME)-$(PIKEOS_BOARD)-$(PIKEOS_BOOT_STRAT))
endif

# Allow to have a project specific makefile
-include makefile.defs

all: boot/$(PIKEOS_BOOT_IMAGE)

#
# Install the generated image
#
# You can change this to install your image for network loading.
# Due to Cygwin, please use forward slashes, even on Windows machines
# and for network target folders:
# install:
#     cp -f boot/$(PIKEOS_BOOT_IMAGE) /tftpboot/.
#
# or, if using sd-card, change the partition names according to your sd-card. In this example, ROOT is the boot, and ROOT1 is rootfs
install: all
	cp boot/hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb /media/ubuntu/ROOT
	sudo tar -zxvf ../zcu102_hwvirt_linux.app/rootfs.tgz -C /media/ubuntu/ROOT1/
	sync
	umount /media/ubuntu/ROOT
	umount /media/ubuntu/ROOT1

#
# For backward compatibility
#
boot: all

$(eval $(call declare_target,boot,Build All (backward compatibility)))

run_cmd ?= $(2)

#
# Generate boot image according to the boot strategy
#
boot/$(PIKEOS_BOOT_IMAGE): pikeos.boot
ifeq ($(PIKEOS_BOARD),)
	$(error No Board, Please rebuild your configuration or configure your project)
else ifeq ($(PIKEOS_BOOT_STRAT),)
	$(error No boot Strategy, Please rebuild your configuration or configure your project)
else
	@test -d "$(@D)" || mkdir -p "$(@D)"
	$(call run_cmd,MAKEBOOT $@,$(PIKEOS_PREFIX)/bin/pikeos-makeboot \
		--project=$(PIKEOS_PROJECT) \
		--input=$< \
		--output=$@)
endif

#
# Force regeneration of boot image (in case binaries have changed)
#
pikeos.boot: vmit.mod FORCE

FORCE:

#
# Validate the project4.rbx file (xml validation)
#
validate-xml: project4.rbx

ifeq ($(PIKEOS_PROJECT_TYPE), integration-preconf)

#
# Handle precompiled vmit for each WRDSZ and ENDIAN)
#
ifneq ($(wildcard vmit.$(PIKEOS_WRDSZ).$(PIKEOS_TARGET_ENDIAN).mod),)
vmit.mod: vmit.$(PIKEOS_WRDSZ).$(PIKEOS_TARGET_ENDIAN).mod
	cp -f $< $@
endif

#
# Call validate xml before creating boot file
#
pikeos.boot: | validate-xml

else

#
# Validate the vmit4.xml file (xml validation)
#
validate-xml: vmit4.xml

#
# Call validate xml before creating vmit.mod
#
vmit.mod: | validate-xml


endif

#
# Clean generated files
#
clean:
	rm -f pikeos.boot boot/$(PIKEOS_BOOT_IMAGE)

#
# Clean created directories
#
distclean: clean
	rm -rf boot

#
# Call makeboot for run
#
run: boot/$(PIKEOS_BOOT_IMAGE)
ifeq ($(PIKEOS_BOOT_STRAT),)
	$(error No boot Strategy set in your project)
else
	$(call run_cmd,RUN $<,$(PIKEOS_PREFIX)/bin/pikeos-makeboot \
		--project=$(PIKEOS_PROJECT) \
		--run=$<)
endif

$(eval $(call declare_target,run,Run the image (for emulators, when available)))

# Generic compilation rules
include $(PIKEOS_POOL_DIR)/makeinc/rules.mk

endif
