# FRED-Library and FRED Client Applications

This ElinOS App Project contains some FRED Client Applications useful to
test and debug FRED when running on ElinOS.

> **Please refrain from manually building this project by itself. Instead,
> use the `build.sh` and the `burn.sh` scripts in the top-level directory
> to do so. Refer to [../README.md](../README.md) for further info.**

___________________________________________________________________________

## Useful information for debugging purposes

### Building in Debug/Release Mode

By default, the project is built in debug mode. This can be changed by
editing the [Makefile], so that:
```Makefile
DEBUG=NO
```

### Changing FRED/DART IPs

FRED/DART IPs are not contained in this project and must be built
separately. The result of the build process is a `tar` archive that should
be placed in [this
location](../zcu102_hwvirt_linux.app/app.rootfs/opt/fredsys/fred.tar.gz).
Notice that the file is NOT part of this project, but part of the [ElinOS
Project for FRED](../zcu102_hwvirt_linux.app/).

> **TODO**: Further information on how to re-build and change IPs should be
> included here.

### Project Targets

This project builds the FRED Client Library and multiple client
applications. Target binaries are located in [app.rootfs](app.rootfs) and
copied automatically to the correct location in
[../zcu102_hwvirt_linux.app/].

The FRED Client Libraries and the client applications included in this
project are copied from other projects (with some minor changes). The
following table summarizes the sources of each subproject.

| Subproject          | URL                 | Commit SHA                               |
| :------------------ | :------------------ | :--------------------------------------- |
| Fred Client Library | [URL][fred-cli-lib] | bffd5141f86cd3ed1f2626bac84f1ceadfaf8d0c |
| Sum Vec Application | [URL][sum-vec]      | bd210b3c1966a9e5509a4cd051be84aa96601aaf |


[fred-cli-lib]: https://github.com/fred-framework/fred-linux-client-lib
[sum-vec]: https://github.com/fred-framework/meta-fred/tree/main/recipes-example/sum-vec/files
