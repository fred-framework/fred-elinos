#!/bin/bash

function main() {
    SCRIPT_PATH="$(realpath "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

    FRED_SERVER=fred-server
    RUNSCHED=runsched

    echo "+------- Starting fred server"

    # Ready to start the fred server on core 1
    "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 2 "${PREFIX}${FRED_SERVER}" >./fred.log 2>./fred.err &

    sleep 3s

    echo "+------- Fred server definitely started"

    for f in "$1"/*.yaml; do
        echo "+------------ Testing $f"
        "$SCRIPT_PATH/rtdag.sh" "$f"

        sleep 5s
    done
    echo "+------------DONE!"
}

(
    set -e
    main "$@"
)
