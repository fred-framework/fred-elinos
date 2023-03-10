
###############################################################################
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#
# Purpose:
# Project Makefile
#
# $Id$
# $Author$
# $Date$
# $Revision$
###############################################################################

# only allow ourselves to run when ELINOS.sh is sourced:
ifeq "$(ELINOS_PREFIX)" ""

.PHONY: boot
.DEFAULT: all ; @$(MAKE) all

all:
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
boot: all
clean: all

includes:
	@echo "Cannot determine include paths and defines as the "; \
	echo "project environment is not set up correctly."; \
	echo "You must probably run"; \
	echo "    sh# elinos-share-project import"
	
else

# Set to NO or unset to switch off debug information
DEBUG = YES

# define additional objects to be cleaned
CLEAN_OBJS = libfred.so libfred.a sum-vec app.rootfs/usr

# not really in use
FRED_PATH ?= /opt/fredsys/

SRCS = $(wildcard $(ELINOS_PROJECT)/src/**/*.c) $(wildcard $(ELINOS_PROJECT)/src/**/**/*.c)
#$(info $$SRCS is [${SRCS}])
C_OBJS = $(SRCS:.c=.o)
C_DEPS = $(C_OBJS:.o=.d)

ADDITIONAL_CFLAGS = -fPIC -I./include -I $(ELINOS_PROJECT)/src/fred-linux-client-lib/ -std=gnu99 -Wall -Werror -DFRED_PATH=${FRED_PATH}
ADDITIONAL_LDFLAGS = -shared 

-include $(ELINOS_PREFIX)/scripts/application.mk

# the main client application, sum-vec
all: sum-vec

clean: clean_common

distclean: distclean_common

# to compile sum-vec, we need to compile and install the fred-lib 1st, in install_libs
sum-vec: install_libs 
	$(CC) $(C_OBJS) -o $@ -L $(ELINOS_PROJECT)/app.rootfs/usr/lib -lfred -lpthread

libfred.so: $(C_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ELinOS C Linker'
	$(CC) $(LDFLAGS) -o "$@" $^ $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libfred.a: $(C_OBJS)
	@echo 'Invoking: ELinOS Archiver'
	$(AR) -r "$@" $^ $(LIBS)
	@echo 'Finished building: $@'
	@echo ' '

install_libs: libfred.so libfred.a
	@echo 'Installing libfred'
	mkdir -p $(ELINOS_PROJECT)/app.rootfs/usr/include $(ELINOS_PROJECT)/app.rootfs/usr/lib
	mkdir -p $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/include
	cp $(ELINOS_PROJECT)/src/fred-linux-client-lib/fred_lib.h $(ELINOS_PROJECT)/app.rootfs/usr/include
	cp $(ELINOS_PROJECT)/libfred.* $(ELINOS_PROJECT)/app.rootfs/usr/lib 
	mkdir -p $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/lib
	mkdir -p $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/include
	cp $(ELINOS_PROJECT)/src/fred-linux-client-lib/fred_lib.h $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/include
	cp $(ELINOS_PROJECT)/libfred.* $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/lib

install: sum-vec
	@echo 'Installing FRED example application'
	mkdir -p $(ELINOS_PROJECT)/app.rootfs/usr/bin
	cp $(ELINOS_PROJECT)/sum-vec $(ELINOS_PROJECT)/app.rootfs/usr/bin 
	mkdir -p $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/bin
	cp $(ELINOS_PROJECT)/sum-vec $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/bin

endif
