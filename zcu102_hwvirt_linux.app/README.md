# ElinOS Project for FRED

This ElinOS project contains all information needed to build a working
ElinOS kernel (together with the corresponding root file system) to use the
FRED Framework on a Xilinx Ultrascale+ ZCU102 Evaluation Board.

> **Please refrain from manually building this project by itself. Instead,
> use the `build.sh` and the `burn.sh` scripts in the top-level directory
> to do so. Refer to [../README.md](../README.md) for further info.**

## Missing Features

> If you use the project "as is", no manual intervention is required.

However, not everything in this project works as intended in a typical FRED
installation. Missing features include:

 - Dynamically changing the Linux device tree via overlays. Typically, FRED
   creates an overlay on top of the base Linux device tree where it places
   the FPGA slots and other devices' information. This is not possible as
   of now. See [this section](#updating-the-device-tree-manually) for how
   to statically include information about your FPGA slots into the ElinOS
   device tree.
   > NOTICE that FRED currently is tested with only one slot, so you should
   > be fine leaving everything as-is.

 - The shared memory regions, defined in the device tree, among
   fred-server, fred client app, and fpga-manager must be configured in
   PikeOS see [the PikeOS Integration
   Project](../zcu102_hwvirt_pikeos.int/README.md) for further info.

 - The following patches are pending to be applied in this project (as per
   AMPERE requirements):
    - [ ] https://github.com/fred-framework/meta-retis/blob/main/recipes-kernel/linux/files/0001-Disable-runtime-frequency-scaling-in-dl-tasks.patch
    - [ ] https://github.com/fred-framework/meta-retis/blob/main/recipes-kernel/linux/files/0002-Set-rt-class-priority-higher-than-dl.patch

___________________________________________________________________________

## Useful information for debugging purposes

### Project Targets

This project builds a Linux kernel image in `boot` and populates a
corresponding rootfs in `app.rootfs`.

### Moving from PetaLinux to ElinOS

FRED is tightly integrated with the PetaLinux kernel. As such, lots of its
component may rely on other components that are only present in a PetaLinux
kernel and not in others Linux versions, like ElinOS. In addition, ElinOS
disables by default several kernel features compared to other
distributions.

Thankfully, this migration from PetaLinux to ElinOS let us re-discover
(seemingly) all FRED kernel requirements, which are now documented
[here](https://github.com/fred-framework/meta-fred#kernel-requirements).

### Project Structure

In addition to porting FRED into ElinOS, this project contains other tools
that are useful from a user perspective when interacting with ElinOS (tree,
dtc, rsync, cpufreq, etc.), together with FRED kernel modules.

Included tools are located [here](app.rootfs/). Some are included as
binaries, while others (like `rsync`) are built from sources and then
copied into that directory for later use. Binaries are taken from the
PetaLinux project (version 2020.2).

> **NOTE**: beware that the `app.rootfs` directory will contain other files
> built by this project. As such, it is mostly ignored by `git`. Manually
> add (with `-f`) all the files you want to keep and track.

[fpga-mgr-fmod]: https://github.com/fred-framework/fred-linux-fpga-mgr-fmod
[buffctl]: https://github.com/fred-framework/fred-linux-buffctl-kmod

FRED kernel modules are located [here](src/kernel-modules). They include
the [fpga-mgr-fmod][fpga-mgr-fmod] and the [buffctl][buffctl] modules.

Included versions refer to the following commits:

| Module        | Commit SHA                               |
| :------------ | :--------------------------------------- |
| fpga-mgr-fmod | e7f9bb0cca1227eb34819acb8f9b29c6bac69ef7 |
| buffctl       | 9c0b9677ad830c1bb520853afe4318efcdc9907f |

### Project Dependencies

This project depends on the [../fred-lib.app](../fred-lib.app) and the
[../fred-server.app](../fred-server.app) projects. If you make any changes
to those projects either run again `build.sh` or run `make install` in
those project before continuing development here.

### Updating the Device Tree Manually

> If you come from PetaLinux, you might be used to let FRED dynamically
> change the hardware IPs available by injecting a device tree overlay at
> runtime. This feature **does not work**.

For any change that you want to make to the FPGA manager configuration
(list of IPs, etc.), you need to re-build the device tree in this project
and build a new image from scratch. You also need to replace the FPGA
Manager driver in ElinOS to reflect the new configuration.

To do so, add the following lines inside the device tree `axi` block
related to `slot0` and `pr_decoupler0`:
```
	axi {
		compatible = "simple-bus";
		u-boot,dm-pre-reloc;
		#address-cells = <0x2>;
		#size-cells = <0x2>;
		ranges;

		slot_p0_s0@a0000000 {
			interrupts = <0x00 0x59 0x04>;
			interrupt-parent = <0x01>;
			compatible = "generic-uio";
			reg = <0x00 0xa0000000 0x00 0x10000>;
		};

		pr_decoupler_p0_s0@a0010000 {
			compatible = "generic-uio";
			reg = <0x00 0xa0010000 0x00 0x10000>;
		};
```

The actual lines depend on the DTS that you want to place of course, but
you should get an idea from the example above.

As of now, the file [system.dts](system.dts) is already edited to include
this information. You can edit it to make changes yourself and rebuild the
project if you need.

There is also an already-compiled version of this DTS file in
[linux-modif.dtb](linux-modif.dtb). If you make some changes, delete it and
rebuild it yourself.

> Be aware that if ElinOS configuration is changed just that it also
> changes its device tree, then you need to make the manual device tree
> modification again. Checkout the comment section of the `Makefile`, under
> rule `boot`, to generate the newer device tree.

### Committing Changes to ElinOS Configuration

> **NOTE**: you must have a valid ElinOS license and a license server
> running on your host to run this step.

Before re-running the `build.sh` script, export the changes that you made
to the project in CODEO/Eclipse (or your editor of choice) by running:
```bash
PATH_TO_ELINOS_INSTALLATION/bin/elinos-share-project export
```

You can now re-run the `build.sh` script and everything will be updated
accordingly.

<!--
## Diff Elinos configuration

In the future, when migrating to newer Elinos version, it will be useful to
know all the configurations done manually in CODEO. For this reason, run
the command `make diffconfig` to highlight those differences compared to
the default Elinos config. Then, it's is just a matter of finding each
parameter in the newer CODEO/Elinos.

 A. Amory
-->
