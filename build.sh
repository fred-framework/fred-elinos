#!/bin/bash

function build_fred_library() {
    if [ -z "${ACTIONS[0]}" ]; then
        return
    fi

    cd fred-lib.app
    ./configure -n
    source ELINOS.sh
    make "${ACTIONS[0]}"
    cd ..
}

function build_fred_server() {
    if [ -z "${ACTIONS[1]}" ]; then
        return
    fi

    cd fred-server.app
    ./configure -n
    source ELINOS.sh
    make "${ACTIONS[1]}"
    cd ..
}
function build_zcu102_hwvirt_linux() {
    if [ -z "${ACTIONS[2]}" ]; then
        return
    fi

    cd zcu102_hwvirt_linux.app
    /opt/elinos-7.1/bin/elinos-share-project import
    source ELINOS.sh
    make "${ACTIONS[2]}"
    cd ..
}
function build_zcu102_hwvirt_pikeos() {
    if [ -z "${ACTIONS[3]}" ]; then
        return
    fi

    cd zcu102_hwvirt_pikeos.int
    /opt/pikeos-5.1/bin/pikeos-share-project import || true # Must accept that this task may fail for a simple warning
    make "${ACTIONS[3]}"
    cd ..
}

function run_step() {
    echo " +-- Running Step '$1' ..."
    if ! "$2" > "$OUTFILE" ; then
        echo " +-- Error while running Step '$1', exiting..."
        return 1
    fi
}

function usage() {
    cat <<EOF

USAGE: ${BASH_SOURCE[0]} [options]

OPTIONS:
    -h, --help          Display this help message
    -v, --verbose       Print output of all commands
                        NOTE: by default this script hides
                              stdout, but not stderr.

EOF
}

function run_all_steps() {
    run_step "Fred Library" build_fred_library
    run_step "Fred Server" build_fred_server
    run_step "ZCU102 HwVirt Linux" build_zcu102_hwvirt_linux
    run_step "ZCU102 HwVirt PikeOS" build_zcu102_hwvirt_pikeos
}

function main() {
    VERBOSE=0
    CLEAN=0

    while [ "$#" -gt 0 ]; do
        case "$1" in
        -h|--help)
            usage
            return 0
            ;;
        -v|--verbose)
            VERBOSE=1
            ;;
        -c|--clean)
            CLEAN=1
            ;;
        *)
            echo "ERROR: Unrecognized option '$1'" >&2
            usage
            return 1
        esac
        shift
    done

    SCRIPT_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
    cd "$SCRIPT_DIR"

    if [ "$VERBOSE" = 1 ]; then
        OUTFILE=/dev/stdout
        export VERBOSE
    else
        OUTFILE=/dev/null
        export VERBOSE=0
    fi


    # Restart the server, just in case
    echo " +-- Restarting the license server"
    echo "     You might be prompted for your password..."
    sudo systemctl stop lmxserv51
    sudo rm -f /usr/lmx-5.1/lmx-serv.pid
    sudo systemctl start lmxserv51

    if [ "$CLEAN" = 1 ]; then
        ACTIONS=(distclean distclean) # Deliberately only the first two

        echo "============================CLEANING=============================="
        run_all_steps
        echo "=========================DONE CLEANING============================"
    fi

    # Build
    ACTIONS=(install install boot all)
    run_all_steps

    echo ' +-- Success!'
}

(
    set -e
    main "$@"
)
