#!/bin/bash

function main() {
    INDIR="$1"
    if ! [ -d "$INDIR" ]; then
        echo "usage: ${BASH_SOURCE[0]} INDIR" >&2
        return 1
    fi

    SCRIPT_PATH="$(realpath "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

    for dest in cpu fpga; do
        for i in $(seq 1 5); do
            fin="$INDIR/input/test_fpga_${i}_input.yaml"
            sol="$INDIR/$dest/test_$i/test_fpga_$i.yaml"
            fout="$INDIR/$dest/rtdag_${i}.yaml"

            if ! [ -f "$sol" ]; then
                # No solution, skip
                continue
            fi

            # At least 30 seconds per DAG
            echo "+-- $fout"
            "${SCRIPT_PATH}/dagc2rtdag.py" "$fin" "$sol" -S 30 >"$fout"
        done
    done

}

(
    set -e
    main "$@"
)
