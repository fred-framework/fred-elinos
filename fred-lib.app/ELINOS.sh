:
# Start ELinOS session
#
# Automatically created on Fri Feb 24 09:58:56 CET 2023

# --- Configuration Settings ------------------------------------------
ELINOS_PRJTYPE="library"
export ELINOS_PRJTYPE
ELINOS_BOARD="custom"
export ELINOS_BOARD
ELINOS_CPU="arm"
export ELINOS_CPU
ELINOS_ARCH="v8hf"
export ELINOS_ARCH
ELINOS_LIBC="glibc"
export ELINOS_LIBC
ELINOS_BOARD_LINUX_ARCH=""
export ELINOS_BOARD_LINUX_ARCH
ELINOS_BOOT_STRAT=""
export ELINOS_BOOT_STRAT
ELINOS_KERNELPATH=""
export ELINOS_KERNELPATH
ELINOS_DEFCONFIG_PATH=""
export ELINOS_DEFCONFIG_PATH
ELINOS_HIDE_CONFIG=""
export ELINOS_HIDE_CONFIG
ELINOS_PROJECT_PRJTYPE="library"
export ELINOS_PROJECT_PRJTYPE
ELINOS_CUSTOM_POOL_DIR=""
export ELINOS_CUSTOM_POOL_DIR
ELINOS_CUSTOM_POOL_SUBDIR=""
export ELINOS_CUSTOM_POOL_SUBDIR
ELINOS_CUSTOM_POOL_BOOT_IMAGENAME=""
export ELINOS_CUSTOM_POOL_BOOT_IMAGENAME
ELINOS_MAKE_VERBOSE=""
export ELINOS_MAKE_VERBOSE
ELINOS_FORCE_LINK=""
export ELINOS_FORCE_LINK
ELINOS_MAKE_NUM_JOBS="9"
export ELINOS_MAKE_NUM_JOBS
ELINOS_MKEFS_AUTO_STRIP="y"
export ELINOS_MKEFS_AUTO_STRIP
ELINOS_MKEFS_AUTO_INCLUDE="y"
export ELINOS_MKEFS_AUTO_INCLUDE
ELINOS_MKEFS_ADD_SEARCH_PATH=""
export ELINOS_MKEFS_ADD_SEARCH_PATH
ELINOS_MKEFS_REPORT_INODES=""
export ELINOS_MKEFS_REPORT_INODES
ELINOS_MKEFS_VERBOSITY_NONE="y"
export ELINOS_MKEFS_VERBOSITY_NONE
ELINOS_MKEFS_VERBOSITY_NORMAL=""
export ELINOS_MKEFS_VERBOSITY_NORMAL
ELINOS_MKEFS_VERBOSITY_MORE=""
export ELINOS_MKEFS_VERBOSITY_MORE
ELINOS_MKEFS_VERBOSITY_MOST=""
export ELINOS_MKEFS_VERBOSITY_MOST
ELINOS_MKEFS_SIGN=""
export ELINOS_MKEFS_SIGN
ELINOS_MKEFS_ALLOW_DUPLICATE_FILES=""
export ELINOS_MKEFS_ALLOW_DUPLICATE_FILES
ELINOS_MKEFS_ADD_PARAMETERS=""
export ELINOS_MKEFS_ADD_PARAMETERS
ELINOS_MKIMAGE_FIT_IMAGE=""
export ELINOS_MKIMAGE_FIT_IMAGE
ELINOS_MKIMAGE_UIMAGE=""
export ELINOS_MKIMAGE_UIMAGE
ELINOS_MKIMAGE_UIMAGE_APPENDED_DTB=""
export ELINOS_MKIMAGE_UIMAGE_APPENDED_DTB
ELINOS_MKIMAGE_CUIMAGE=""
export ELINOS_MKIMAGE_CUIMAGE
ELINOS_MKIMAGE_COMP_NONE=""
export ELINOS_MKIMAGE_COMP_NONE
ELINOS_MKIMAGE_COMP_GZIP=""
export ELINOS_MKIMAGE_COMP_GZIP
ELINOS_MKIMAGE_COMP_BZIP2=""
export ELINOS_MKIMAGE_COMP_BZIP2
ELINOS_MKIMAGE_COMP_LZMA=""
export ELINOS_MKIMAGE_COMP_LZMA
ELINOS_VALIDATION_ALWAYS=""
export ELINOS_VALIDATION_ALWAYS
ELINOS_VALIDATION_FILE_SYSTEM=""
export ELINOS_VALIDATION_FILE_SYSTEM
ELINOS_VALIDATION_KERNEL=""
export ELINOS_VALIDATION_KERNEL
ELINOS_VALIDATION_ELINOS=""
export ELINOS_VALIDATION_ELINOS
ELINOS_VALIDATION_CROSS=""
export ELINOS_VALIDATION_CROSS
ELINOS_PREFIX="/opt/elinos-7.1"
export ELINOS_PREFIX
ELINOS_PROJECT="/home/ubuntu/repos/pikeos/workspace7.3-2/fred-lib.app"
export ELINOS_PROJECT
ELINOS_CDK="/opt/elinos-7.1/cdk/arm/v8hf/glibc"
export ELINOS_CDK
ELINOS_BIN_PREFIX="aarch64-unknown-linux-gnu"
export ELINOS_BIN_PREFIX
ELINOS_TARGET="aarch64-unknown-linux-gnu"
export ELINOS_TARGET
ELINOS_TARGET_FILES="/opt/elinos-7.1/target/arm/v8hf"
export ELINOS_TARGET_FILES
ELINOS_VERSION="7.1"
export ELINOS_VERSION
LINUX_ARCH=""
export LINUX_ARCH
ARCH=""
export ARCH
INSTALL_PATH=""
export INSTALL_PATH
INSTALL_MOD_PATH=""
export INSTALL_MOD_PATH
LIST_BOOTSTRATS=""
export LIST_BOOTSTRATS
AS="aarch64-unknown-linux-gnu-as"
export AS
AR="aarch64-unknown-linux-gnu-ar"
export AR
NM="aarch64-unknown-linux-gnu-nm"
export NM
LD="aarch64-unknown-linux-gnu-ld"
export LD
RANLIB="aarch64-unknown-linux-gnu-ranlib"
export RANLIB
STRIPTOOL="aarch64-unknown-linux-gnu-strip"
export STRIPTOOL
CC="aarch64-unknown-linux-gnu-gcc"
export CC
CXX="aarch64-unknown-linux-gnu-g++"
export CXX
GDB="aarch64-unknown-linux-gnu-gdb"
export GDB
CROSS_COMPILE="/opt/elinos-7.1/cdk/arm/v8hf/glibc/bin/aarch64-unknown-linux-gnu-"
export CROSS_COMPILE
unset ELINOS_FEATURE_BASE
# --- Configuration Settings End --------------------------------------

echo "STARTING ELINOS SESSION"
echo "======================="
echo 

# addpath {env-variable} {new-path-element}
addpath()
{
    eval "_X=\"\${$1:-}\""
    if [ -n "$_X" ]; then
        if [ -z "`echo $_X | grep $2`" ]; then
            eval "$1=\"$2:$_X\""
        fi
    else
        eval "$1=\"$2\""
    fi
}

# removepath {env-variable} {old-path-element}
removepath()
{
    eval "_X=\"\${$1:-}\""
    eval "$1=\"`echo \"$_X\" | sed \"s,:$2,,\"`\""
}


# remove ${ELINOS_PREFIX}/bin from PATH if it is not the first entry.
removepath "PATH" "/opt/elinos-7.1/bin"
addpath "PATH" "${ELINOS_CDK}/bin"
addpath "PATH" "${ELINOS_PREFIX}/bin"
addpath "PATH" "${ELINOS_PREFIX}/share/qemu/bin"

if [ -z "${INFOPATH:-}" ]; then
    INFOPATH=""
    export INFOPATH
fi
addpath "INFOPATH" "${ELINOS_CDK}/info"
addpath "INFOPATH" "${ELINOS_PREFIX}/info"
export INFOPATH

# if possible use manpath to preset MANPATH first
manpath=`which manpath 2>/dev/null || true`
if [ -n "$manpath" -a -x "$manpath" ]; then
    if [ -z "${MANPATH:-}" ]; then
        MANPATH="`$manpath 2>/dev/null`"
    fi
fi
# export MANPATH in any case.
export MANPATH
addpath "MANPATH" "${ELINOS_PREFIX}/${ELINOS_ARCH}-target/${ELINOS_LIBC}/default/man"
addpath "MANPATH" "${ELINOS_CDK}/${ELINOS_BIN_PREFIX}/man"
addpath "MANPATH" "${ELINOS_CDK}/share/man"
export MANPATH
unset manpath
unset addpath

echo "Setting up CDK ${ELINOS_CPU}_${ELINOS_ARCH} for $ELINOS_LIBC"
echo ""
echo "\$ELINOS_PREFIX     = $ELINOS_PREFIX"
echo "\$ELINOS_BOARD      = $ELINOS_BOARD"
echo "\$ELINOS_BIN_PREFIX = $ELINOS_BIN_PREFIX"
echo "\$ELINOS_BOOT_STRAT = $ELINOS_BOOT_STRAT"
echo "\$ELINOS_KERNELPATH = $ELINOS_KERNELPATH"
echo "\$ELINOS_PROJECT    = $ELINOS_PROJECT"
echo "\$CC                = $CC"
echo "\$CXX               = $CXX"
echo "\$AS                = $AS"
echo "\$GDB               = $GDB"

alias ddd='ddd --debugger $GDB'

echo
