#!/bin/bash

function average() {
    awk '{ sum += $1 } END { if (NR > 0) print sum / NR }'
}

function main() {
    for d in petal pikeos; do
        for t in cpu fpga; do
            for f in $d/$t/*.out.0; do
                period=$(head -1 "$f")
                avg=$(tail -n +2 "$f" | average)

                echo "$(dirname "$f")" "$(basename "$f" .out.0)" "$period" "$avg"
            done
        done
    done
}

(
    set -e
    main "$@"
)
