
.PHONY: all install distclean
.DEFAULT: help

# prepare the linux image w FRED
all:
	@echo "Run ./build.sh to build the ELinOS image for FRED"
	exit 0

# this will write in the SD card
install:
	@echo "Run ./burn.sh to write the image into the SD card. You might need to change the partition names"
	make -C zcu102_hwvirt_pikeos.int install

distclean:
	@echo "Run ./clean.sh to clean all ELinOS projects"
	exit 0
