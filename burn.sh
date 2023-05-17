#!/bin/bash

function usage() {
    cat <<EOF

USAGE: ${BASH_SOURCE[0]} [options] DESTINATION_UBOOT DESTINATION_ROOTFS

OPTIONS:
    -h, --help          Display this help message
EOF
}

function main() {
    if [ "$(id -u)" != 0 ]; then
        echo "Please run this script as root or with sudo" >&2
        return 1
    fi

    POSITIONAL_ARGS=0
    NEXT_POSITIONA_ARG=(DESTINATION_UBOOT DESTINATION_ROOTFS)

    DESTINATION_UBOOT=
    DESTINATION_ROOTFS=

    while [ "$#" -gt 0 ]; do
        case "$1" in
        -h|--help)
            usage
            return 0
            ;;
        -*)
            echo "ERROR: Unrecognized option '$1'" >&2
            usage
            return 1
            ;;
        *)
            if [ "$POSITIONAL_ARGS" -ge 2 ] ; then
                echo "ERROR: too many positional arguments!" >&2
                usage
                return 1
            fi

            declare "${NEXT_POSITIONA_ARG[$POSITIONAL_ARGS]}"="$1"
            POSITIONAL_ARGS=$((POSITIONAL_ARGS+1))
        esac
        shift
    done

    if [ -z "$DESTINATION_UBOOT" ] || [ -z "$DESTINATION_ROOTFS" ]; then
        echo "ERROR: required argument missing" >&2
        usage
        return 1
    fi

    if ! [ -d "$DESTINATION_UBOOT" ] || [ "$DESTINATION_UBOOT" = / ]; then
        echo "Invalid argument DESTINATION_UBOOT: '$DESTINATION_UBOOT'" >&2
        usage
        return 1
    fi

    if ! [ -d "$DESTINATION_ROOTFS" ] || [ "$DESTINATION_ROOTFS" = / ]; then
        echo "Invalid argument DESTINATION_ROOTFS: '$DESTINATION_ROOTFS'" >&2
        usage
        return 1
    fi

    DESTINATION_UBOOT="$(realpath "$DESTINATION_UBOOT")"
    DESTINATION_ROOTFS="$(realpath "$DESTINATION_ROOTFS")"

    echo ""
    echo "About to write to the following directories,"
    echo "please type 'y' to confirm."
    echo "Anything else will exit."
    echo ""
    echo "boot:     '$DESTINATION_UBOOT'"
    echo "rootfs:   '$DESTINATION_ROOTFS'"
    echo ""
    read -r -p "Are you sure? [y/N] " response
    case "$response" in
        [yY][eE][sS]|[yY])
            # Continue
            ;;
        *)
            echo "I'll take that as a no. Exiting."
            return 1
            ;;
    esac

    SCRIPT_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
    cd "$SCRIPT_DIR"

    MAKEFILE="zcu102_hwvirt_pikeos.int/Makefile"
    if ! [ -f "$MAKEFILE.bkp" ]; then
        echo "" >&2
        echo "WARN: Backing up file '$MAKEFILE'." >&2
        echo "If this is the first time you run this script this is expected." >&2
        echo "" >&2
        cp "$MAKEFILE" "$MAKEFILE.bkp"
    fi

    echo " + Setting target directories as instructed..."

    # Substitute destination directories
    tmpfile=$(mktemp)
    cp "$MAKEFILE.bkp" "$tmpfile"
    sed -i 's#/media/ubuntu/ROOT1#'"\"$DESTINATION_ROOTFS\""'#g' "$tmpfile"
    sed -i 's#/media/ubuntu/ROOT#'"\"$DESTINATION_UBOOT\""'#g' "$tmpfile"
    sed -i 's/umount/#umount/g/' "$tmpfile"
    # TODO: rename to image.ub
    cp "$tmpfile" "$MAKEFILE"
    rm -f "$tmpfile"

    echo " + Cleaning target directories..."
    rm -rf "${DESTINATION_UBOOT:?}"/*
    rm -rf "${DESTINATION_ROOTFS:?}"/*

    echo " + Burning on target directories..."
    cd zcu102_hwvirt_pikeos.int
    # source ELINOS.sh
    source PikeOS.sh
    make install
    cd ..

    mv "$DESTINATION_UBOOT"/hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb "$DESTINATION_UBOOT/image.ub"
    umount "$DESTINATION_UBOOT"
    umount "$DESTINATION_ROOTFS"
    echo " + Success"
}

(
    set -e
    main "$@"
)
