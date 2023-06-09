#!/bin/bash

function main() {
    # Initialize fred stuff
    # modprobe zynqmp-fpga-fmod ; sleep 1
    # modprobe fred-buffctl ; sleep 1
    # load_hw ; sleep 1

    PREFIX=./usr/bin/

    FRED_SERVER=fred-server
    FST=fredspeedtest
    RUNSCHED=runsched

    echo "+------- Starting fred server"

    # Ready to start the fred server on core 1
    "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 2 ${PREFIX}${FRED_SERVER} >./fred.log 2>./fred.err &

    sleep 10s

    echo "+------- Fred server definitely started"

    # Create output directory, cleanup pre-existing output if any
    rm -rf ./fst.out.d
    mkdir -p ./fst.out.d

    # The speedtest application will run on core 2, grep on core 3

    echo "+------- Starting experiments"

    for ip in sum_vec xor_vec mul64 mul128 ; do
        other_ip=sum_vec
        if [ "$ip" = "$other_ip" ]; then
            other_ip=xor_vec
        fi

        # Run with re-configuration 100 times
        for i in $(seq 1 100); do
            echo "+------- Running other        $i" >&2
            "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 4 "${PREFIX}${FST}" $other_ip >/dev/null

            sleep 1

            echo "+------- Running reconf test  $i" >&2
            "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 4 "${PREFIX}${FST}" $ip

            sleep 1
        done | taskset 8 grep took >./fst.out.d/$ip.reconf.txt

        # Run without re-configuration 100 times
        for i in $(seq 1 100); do
            echo "+------- Running regular test $i" >&2
            "${PREFIX}${RUNSCHED}" SCHED_FIFO 99 taskset 4 "${PREFIX}${FST}" $ip

            sleep 1
        done | taskset 8 grep took >./fst.out.d/$ip.regular.txt

    done

    echo "DONE!" >&2
}

(
    set -e
    main "$@"
)