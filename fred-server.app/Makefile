
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

# Must be defined for the standard Makefile system
PROG = fred-server

# Set to NO or unset to switch off debug information
DEBUG = YES

# define additional objects to be cleaned
CLEAN_OBJS = app.rootfs/bin

#C_OBJS = $(patsubst %.c,%.o,$(wildcard src/**/*.c))
#C_DEPS = $(patsubst %.c,%.d,$(wildcard src/**/*.c))
SRCS = $(wildcard src/fred-linux/*.c) $(wildcard src/fred-linux/**/*.c)
#$(info $$SRCS is [${SRCS}])
C_OBJS = $(SRCS:.c=.o)
C_DEPS = $(C_OBJS:.o=.d)

ADDITIONAL_CFLAGS = -I./include -std=gnu99  -Wno-unused-but-set-variable -D LOG_GLOBAL_LEVEL=LOG_LEV_FULL
#ADDITIONAL_INC_FLAGS =  -I./src/fred-server/hw_support -I./src/fred-server/shared_kernel -I./src/fred-server/shared_user -I./src/fred-server/srv_core -I./src/fred-server/srv_core_mocks -I./src/fred-server/srv_support -I./src/fred-server/utils

-include $(ELINOS_PREFIX)/scripts/application.mk


all: all_common

clean: clean_common
	rm -f $(ELINOS_PROJECT)/$(PROG)
	rm -rf $(ELINOS_PROJECT)/app.rootfs/*

distclean: distclean_common

filesystem: filesystem_common

install: $(PROG)
	@echo 'Installing fred-server'
	mkdir -p $(ELINOS_PROJECT)/app.rootfs/usr/bin
	cp $(ELINOS_PROJECT)/$(PROG) $(ELINOS_PROJECT)/app.rootfs/usr/bin
	mkdir -p $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/bin
	cp $(ELINOS_PROJECT)/$(PROG) $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app/app.rootfs/usr/bin

endif
