#!/bin/bash

function build_fred_library() {
    cd fred-lib.app
    ./configure -n
    source ELINOS.sh
    make install
    cd ..
}

function build_fred_server() {
    cd fred-server.app
    ./configure -n
    source ELINOS.sh
    make install
    cd ..
}
function build_zcu102_hwvirt_linux() {
    cd zcu102_hwvirt_linux.app
    /opt/elinos-7.1/bin/elinos-share-project import
    source ELINOS.sh
    make boot
    cd ..
}
function build_zcu102_hwvirt_pikeos() {
    cd zcu102_hwvirt_pikeos.int
    /opt/pikeos-5.1/bin/pikeos-share-project import || true # Must accept that this task may fail for a simple warning
    make all
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

function main() {
    VERBOSE=0

    while [ "$#" -gt 0 ]; do
        case "$1" in
        -h|--help)
            usage
            return 0
            ;;
        -v|--verbose)
            VERBOSE=1
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
    else
        OUTFILE=/dev/null
    fi

    # Cleanup, just in case
    # rm -f fred-lib.app/ELINOS.sh
    # rm -f fred-server.app/ELINOS.sh
    # rm -f zcu102_hwvirt_linux.app/ELINOS.sh
    # rm -f zcu102_hwvirt_pikeos.int/ELINOS.sh

    # Restart the server, just in case
    echo " +-- Restarting the license server"
    echo "     You might be prompted for your password..."
    sudo systemctl stop lmxserv51
    sudo rm -f /usr/lmx-5.1/lmx-serv.pid
    sudo systemctl start lmxserv51

    run_step "Fred Library" build_fred_library
    run_step "Fred Server" build_fred_server
    run_step "ZCU102 HwVirt Linux" build_zcu102_hwvirt_linux
    run_step "ZCU102 HwVirt PikeOS" build_zcu102_hwvirt_pikeos

    echo ' +-- Success!'
}

(
    set -e
    main "$@"
)
