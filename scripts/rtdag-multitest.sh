#!/bin/bash

function main() {
    FRED_SERVER=fred-server
    RUNSCHED=runsched
    RTDAG=/rtdag-new.sh

    echo "+------- Starting fred server"

    # Ready to start the fred server on core 1
    "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 2 "${PREFIX}${FRED_SERVER}" >./fred.log 2>./fred.err &

    sleep 3s

    echo "+------- Fred server definitely started"

    for f in "$1"/*.yaml; do
        echo "+------------ Testing $f"
        "$RTDAG" "$@"

        sleep 5s
    done
    echo "+------------DONE!"
}

(
    set -e
    main "$@"
)
