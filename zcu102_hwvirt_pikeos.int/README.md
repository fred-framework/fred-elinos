# FRED for PikeOS 5.1

This PikeOS Integration Project, where PikeOS is configure to work with the FRED Framework. The configuration used here is mostly based on Deliverable 5.3, from the AMPERE project.

## Restoring the PikeOS Project

Run 

```
$ cd zcu102_hwvirt_pikeos.int
$ /opt/pikeos-5.1/bin/pikeos-share-project import
$ ./configure -n
$ . ./ELINOS.sh
```

to restore the project once the repository is cloned.

## Generating the Linux Image and Updating the SD-card

First, run `make boot` in zcu102_hwvirt_linux.app to compile the kernel and generate the linus image. 

Run `make all` to compile PikeOS configuration with ElinOS image. Or `make install` to write the image into the SD-card. You might need to change the partition names according to the your sd-card.

The generated boot is located here:
```
boot/
└── hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb

```
## Missing Features

 - The shared memory regions, defined in the device tree, among fred-server, fred client app, and fpga-manager must be configured in PikeOS
 - It's possible that the interruptions issues when the IP finishes its tasks, defined in the device tree, should also be mapped in PikeOS.

## Updating the PikeOS configuration

Since there are still some work to be done to configure PikeOS for FRED, it will be necessary to update both Elinos System Project 
and PikeOS project. Run the following command to prepare the project to update the repository. 

```
$ cd zcu102_hwvirt_pikeos.int
$ /opt/pikeos-5.1/bin/pikeos-share-project export
```

Then repeat the procedure mentioned above to restore the project.

## Diff PikeOS configuration

In the future, when migrating to newer PikeOS version, it will be useful to know all the configurations done manually in CODEO.
For this reason, run the command `make diffconfig` to highlight those differences compared to the default PikeOS config. Then,
it's is just a matter of finding each parameter in the newer CODEO/PikeOS.

A. Amory
