#!/bin/bash

function main() {
    export TICKS_PER_US=3.82
    echo -1 | tee /proc/sys/kernel/sched_rt_runtime_us

    local fname
    local curdir

    curdir="$PWD"
    fname="$(realpath "$1")"

    rm -rf /tmp/rtdag.log.d
    mkdir -p /tmp/rtdag.log.d
    cd /tmp/rtdag.log.d

    # ---------------------------------
    rtdag "$fname"
    # ---------------------------------

    dag_id=0
    for f in */*; do
        cp "$f" "$curdir/$(basename "$1" .yaml).out.$dag_id"
        dag_id=$((dag_id+1))
    done
}

(
    set -e
    main "$@"
)
