# fred-elinos

This is Elinos (by SysGo) realtime OS setup for the FRED framework. 

Please go through all four ElinOS/PikeOS projects for more details.

## Generating the image

Run `make all` to build all four projects and `make install` to write the Linux image into the sd-card.

## Booting the ZCU102 Board

Place the SD card and run the following commands in the uboot terminal:

```
env default -a
setenv kernel_img hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb
boot
```

## Testing FRED with Device Tree Overlay

Device tree overlay is currently not working, but this was supposed to be the flow to apply the overlay during Linux execution. 

```
$ modprobe fred-buffctl
$ modprobe zynqmp-fpga-fmod
$ mkdir -p /lib/firmware
$ cp /opt/fredsys/dart_fred/bits/static.bin /lib/firmware/
$ cp /opt/fredsys/static.dtbo /lib/firmware/
$ mount -t configfs none /sys/kernel/config
$ mkdir -p /sys/kernel/config/device-tree/overlays/fred-static
$ echo 0 > /sys/class/fpga_manager/fpga0/flags
$ echo -n "static.dtbo" > /sys/kernel/config/device-tree/overlays/fred-static/path
        crash at this point !!! 
```

Make sure the device tree was generated with symbols included. This is not the defaut behaviour of ELinos, but the rule `boot` in `zcu102_hwvirt_linux.app/Makefile` has this command to make it happen:

```
make -C $(ELINOS_PROJECT)/linux ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -j${ELINOS_MAKE_NUM_JOBS} $(DTB_FILE) DTC_FLAGS=-@
```

Finally, when testing the device tree feature, make sure that the current device tree does not already has `slot0` and `pr_decoupler0` defined.

## Testing FRED with Fixed Device Tree

Since device tree overlay is not working, this is the current workaround to add the FPGA configuration in Linux image building time.

```
modprobe fred-buffctl
modprobe zynqmp-fpga-fmod
mkdir -p /lib/firmware
cp /opt/fredsys/dart_fred/bits/static.bin /lib/firmware/
echo 0 > /sys/class/fpga_manager/fpga0/flags
cd /opt/fredsys
fred-server &
sum-vec
```

These commands lauch FRED device drivers, set the bitstream, launch the server and the sum-vec example application.

## Debugging FRED

The projects fred-lib.app and fred-server.app were made to enable step-by-step debug of both FRED client and server within CODEO.

**TBD how to setup CODEO for debug**

## Acknowledgement

Thanks for Enkhtuvshin Janchivnyambuu and Andrej Kruták from [SYSGO](https://www.sysgo.com) for the tecnical support during this migration of FRED framework from Petalinux to Elinos.

## Contributions

  Did you find a bug in this layer ? Do you have some extensions or updates to add ? Please send us a Pull Request.

## Authors

 - Alexandre Amory (Feb 2023), [Real-Time Systems Laboratory (ReTiS Lab)](https://retis.santannapisa.it/), [Scuola Superiore Sant'Anna (SSSA)](https://www.santannapisa.it/), Pisa, Italy.

## Funding
 
This software package has been developed in the context of the [AMPERE project](https://ampere-euproject.eu/). This project has received funding from the European Union’s Horizon 2020 research and innovation programme under grant agreement No 871669.
