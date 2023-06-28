#!/bin/bash

function main() {
    modprobe zynqmp-fpga-fmod
    sleep 1
    modprobe fred-buffctl
    sleep 1
    load_hw
    sleep 1
}

(
    set -e
    main "$@"
)
