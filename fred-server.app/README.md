# FRED Server

This ELinOS App project builds and installs the `fred-server` application
for the _ElinOS Project for FRED_ (see
[here](../zcu102_hwvirt_linux.app/)).

Fred Server source code is included as a git submodule.

## Building

> **Please refrain from manually building this project by itself. Instead,
> use the `build.sh` and the `burn.sh` scripts in the top-level directory
> to do so. Refer to [../README.md](../README.md) for further info.**

## Compilation options

Export the following variables before running the `build.sh` script to
configure the way the Fred Server is built:

| Variable        | Accepted Values                      | Default Value  |
| :-------------- | :----------------------------------- | :------------- |
| BUILD_TYPE      | `Release`, `Debug`                   | `Release`      |
| FREDS_LOG_LEVEL | `mute`, `simple`, `full`, `pedantic` | `mute`         |
| FREDS_PATH      | Any valid Unix path                  | `/opt/fredsys` |

## Authors

- Alexandre Amory (Feb 2023)
- Gabriele Ara (May 2023-)

[Real-Time Systems Laboratory (ReTiS Lab)](https://retis.santannapisa.it/),
[Scuola Superiore Sant'Anna (SSSA)](https://www.santannapisa.it/), Pisa,
Italy.
