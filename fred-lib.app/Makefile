#----------------------------------------------------------------------
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# Project:     Fred Client Applications for ElinOS
# Author:      Gabriele Ara (Scuola Superiore Sant'Anna, Pisa)
#----------------------------------------------------------------------

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


###############################################################################
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#
# Purpose:
#    Simple ELinOS Makefile, which forwards the targets "all, install,
#    clean, distclean" to any Makefile.elinos found one level below of
#    where we are now.
#
#    Customize to your liking, but make sure *this* Makefile.elinos is
#    present and responds to all, install, clean, distclean"
#
# $Id$
# $Author$
# $Date$
# $Revision$
###############################################################################

.PHONY: all install install_common distclean_common clean

M_ELINOS_DIRECTORY  := $(ELINOS_PROJECT)/../zcu102_hwvirt_linux.app
M_ELINOS_ROOTDIR    := $(M_ELINOS_DIRECTORY)/app.rootfs

# forward make target to Makefile.elinos in src dir
all:
	$(MAKE) -C src -f Makefile.elinos all
install_common:
	$(MAKE) -C src -f Makefile.elinos install
clean:
	$(MAKE) -C src -f Makefile.elinos clean
distclean_common:
	$(MAKE) -C src -f Makefile.elinos distclean

INSTALLED_FILES         := \
	usr/lib64/libfred.so \
	usr/lib64/libfred.a \
	usr/include/fred_lib.h \
	usr/include/fredcpp.h \
	usr/lib64/cmake/libfred/libfredTargets.cmake \
	usr/lib64/cmake/libfred/libfredTargets-release.cmake \
	usr/lib64/cmake/libfred/libfredConfig.cmake \
	usr/lib64/cmake/libfred/libfredConfigVersion.cmake

INSTALLED_DIRS := $(dir $(INSTALLED_FILES))

install: install_common
	@echo "Installing into ElinOS Project directory... ($(M_ELINOS_DIRECTORY))"
	@for d in $(INSTALLED_DIRS); do mkdir -p $(M_ELINOS_ROOTDIR)/$$d ; done
	@for f in $(INSTALLED_FILES); do echo "Copying into $(M_ELINOS_ROOTDIR)/$$f"; cp app.rootfs/$$f $(M_ELINOS_ROOTDIR)/$$f ; done

distclean: distclean_common
	@for f in $(INSTALLED_FILES); do rm -f $(M_ELINOS_ROOTDIR)/$$f ; done

# endif (ELINOS_PROJECT)
endif
