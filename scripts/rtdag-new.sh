#!/bin/bash

function run_rtdag() {
    local mdir="$PWD"
    local tmpdir
    tmpdir=$(mktemp -d)

    flist=()
    for f in "$@" ;do
        flist+=("$(realpath "$f")")
    done

    cd "$tmpdir"
    for f in "${flist[@]}"; do
        "$RTDAG_CMD" "$f" &
    done

    wait

    for f in $(find -type f); do
        cp "$f" "$mdir"
    done

    cd - >/dev/null
    rm -rf "$tmpdir"
}

function main() {
    echo -1 | tee /proc/sys/kernel/sched_rt_runtime_us

    export TICKS_PER_US=1.218
    RTDAG_CMD="${PREFIX}rtdag"

    if [ $# -lt 1 ]; then
        echo "Expecting at least one argument file!!" >&2
        return 1
    fi

    FRED_SERVER=fred-server
    RUNSCHED=runsched

    echo "Starting fred server..."
    echo "PREFIX = ${PREFIX}"

    "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 4 "${PREFIX}${FRED_SERVER}" >./fred.log 2>./fred.err &
    fred_pid="$!"
    disown "$fred_pid"

    echo "JOBS: $(jobs -p)"

    sleep 5

    (
        run_rtdag "$@"
    )

    sleep 1

    kill "$fred_pid"

    echo "DONE!"
}

(
    set -e
    main "$@"
)
