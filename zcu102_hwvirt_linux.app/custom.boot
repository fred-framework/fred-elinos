#!/bin/bash

#----------------------------------------------------------------------
# (C) Copyright SYSGO GmbH.
# Klein-Winternheim, Germany
# All rights reserved.
#
# Boot strategy extension scriptlet. You can modify this file to
# address any of the following issues:
#
# - Create an entire personal boot strategy "custom". For instance,
#   this can be required if you need to specially prepare a boot
#   image for your boot-loader
# - Modify the "final_steps()" which are common to all boot strategies. 
#   This is handy if for example you want to automatically copy boot files 
#   to a /tftpboot download area.
#----------------------------------------------------------------------

# warn when using uninitialised variables
set -u

# name and label of the boot strategy
# Each line is one boot strategy defined in this scriptlet.
# "list_custom" reflects the name "bs.custom".
list_custom()
{
	echo "custom_boot" "--" "Custom Boot Strategy"
}

kernel_custom()
{
	# Add your desired kernel target (zImage, bzImage, ...)
	# (see applicable bs scriptlets for examples)
	echo "vmlinux"
}

# Special controls for each boot strategy defined here.
#
# Add one "echo" line per environment variable which you need to be defined
# for the 'make boot' process.
# Leave the function away, if not needed.
# Variable name, prompt, default value and flag must be separated by colons.
# Flags may be "ask" or nothing. "ask" will always prompt for that value,
# regardles if the corresponding variable is already set or not.
#
# "listcontrol_custom_boot" reflects the boot strategy name "custom_boot"
listcontrols_custom_boot() 
{
	# Example for a custom boot parameter:
	#
	# Variable name:  CUSTOM_BOOT_PARAM
	# Prompt:         "My custom boot parameter"
	# Default Value:  42
	#
	# echo "CUSTOM_BOOT_PARAM:My custom boot parameter:42:ask"

	# placeholder to let this function at least do *something*:
	true
}

# main scriptlet. Add one for each boot strategy.
# "prepare_custom_boot" reflects the boot strategy name "custom_boot"
prepare_custom_boot()
{
	# NOTE: if you defined special controls like shown above
	# in listcontrols_custom_boot() with $CUSTOM_BOOT_PARAM, 
	# you may use them here.

	echo
	echo "Preparing for custom Bootstrap:"
	echo
	
	cp $kerneldir/vmlinux ${ELINOS_PROJECT}/boot/ || exit 1
}

# final steps to be performed for your target.
# Here you may add e.g. the copy commands as needed.
final_steps()
{
cat <<EOT

                             - NOTE -

   You may want to customise this script to meet any special
   requirements of your target hardware. For instance, you could
   add the commands neccessary to copy the boot files to a disk
   or to the download directory of your tftp-server.

   The script is located here:
      $ELINOS_PROJECT/custom.boot
   Edit the "final_steps" part, where you currently find this note.

EOT
}
