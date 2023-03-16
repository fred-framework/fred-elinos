#
# Example for a simple Makefile for a one-file kernel module
# The kernels make rules and configuration will be used.
#

# if special CFLAGS shall be used for this module, use EXTRA_CLFAGS
EXTRA_CFLAGS	+= -fno-stack-protector -Wno-error

# select the file to be a kernel module
obj-m		+= zynqmp-fpga-fmod.o

