# FRED-Library and FRED Client Applications

This ElinOS App Project contains some FRED Client Applications useful to
test and debug FRED when running on ElinOS.

## Building

> **Please refrain from manually building this project by itself. Instead,
> use the `build.sh` and the `burn.sh` scripts in the top-level directory
> to do so. Refer to [../README.md](../README.md) for further info.**

## Compilation options

Export the following variables before running the `build.sh` script to
configure the way the Fred Server is built:

| Variable               | Accepted Values                             | Default Value |
| :--------------------- | :------------------------------------------ | :------------ |
| BUILD_TYPE             | `Release`, `Debug`                          | `Release`     |
| RTDAG_LOG_LEVEL        | `none`, `error`, `warning`, `info`, `debug` | `none`        |
| RTDAG_TASK_IMPL        | `thread`, `process` (untested)              | `thread`      |
| RTDAG_INPUT_TYPE       | `yaml`, `header` (untested)                 | `yaml`        |
| RTDAG_COMPILER_BARRIER | `ON`, `OFF`                                 | `ON`          |
| RTDAG_MEM_ACCESS       | `ON`, `OFF`                                 | `OFF`         |
| RTDAG_COUNT_TICK       | `ON`, `OFF`                                 | `ON`          |

## Project sources

This project builds the FRED Client Library and multiple client
applications. Target binaries are placed in [app.rootfs](app.rootfs) and
copied automatically to the correct location in
[../zcu102_hwvirt_linux.app/](../zcu102_hwvirt_linux.app/).

The following projects code is included as submodules:

- [Fred Client Library][fred-cli-lib]
- [Sum Vec Application][sum-vec]

[fred-cli-lib]: https://github.com/fred-framework/fred-linux-client-lib
[sum-vec]: https://github.com/fred-framework/meta-fred/tree/main/recipes-example/sum-vec/files

## Authors

- Alexandre Amory (Feb 2023)
- Gabriele Ara (May 2023-)

[Real-Time Systems Laboratory (ReTiS Lab)](https://retis.santannapisa.it/),
[Scuola Superiore Sant'Anna (SSSA)](https://www.santannapisa.it/), Pisa,
Italy.

___________________________________________________________________________

## Changing FRED/DART IPs

FRED/DART IPs are not contained in this project and must be built
separately. The result of the build process is a `tar` archive that should
be placed in [this location](../zcu102_hwvirt_linux.app/app.rootfs/opt/fredsys/fred.tar.gz).
Notice that the file is NOT part of this project, but part of the [ElinOS Project for FRED](../zcu102_hwvirt_linux.app/).

You will find the archive to substitute at the following path from the root
of the project:

```txt
zcu102_hwvirt_linux.app/app.rootfs/opt/fredsys/fred.tar.gz
```

> **TODO**: Further information on how to re-build and change IPs should be
> included here.
