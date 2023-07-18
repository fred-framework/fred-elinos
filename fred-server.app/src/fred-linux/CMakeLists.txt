cmake_minimum_required(VERSION 3.16)

# ------------------------- PROJECT INFORMATION ------------------------- #

project(fred-server VERSION 0.1)

# ---------------------------- SOURCE FILES ----------------------------- #

set(fred-server-sources
    ./main.c
    ./hw_support/decoup_drv_xil.c
    ./hw_support/devcfg_drv_fpga_mgr.c
    ./hw_support/gpio_probes.c
    ./hw_support/slot_drv_master.c
    ./hw_support/slot_drv_null.c
    ./hw_support/uio_drv.c
    ./shared_user/user_buff.c
    ./srv_core/accel_req.c
    ./srv_core/devcfg.c
    ./srv_core/fd_timer.c
    ./srv_core/fred_sys.c
    ./srv_core/hw_task.c
    ./srv_core/partition.c
    ./srv_core/reactor_epoll.c
    ./srv_core/reactor_poll.c
    ./srv_core/scheduler_fred.c
    ./srv_core/signals_recv.c
    ./srv_core/slot.c
    ./srv_core/slot_timer.c
    ./srv_core/sw_task_client.c
    ./srv_core/sw_tasks_listener.c
    ./srv_core/sys_layout.c
    ./srv_core_mocks/cyclic_client.c
    ./srv_core_mocks/scheduler_fred_rand.c
    ./srv_support/buffctl.c
    ./srv_support/parser.c
    ./utils/fd_utils.c
    ./utils/logger.c
)

# --------------------------- PROJECT TARGETS --------------------------- #

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}-sources})

# ------------------------ CONFIGURATION OPTIONS ------------------------ #

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/util.cmake)

# FRED supports 64-bit addresses, but DART does not. Therefore we stick to
# 32-bit addresses by default for C_M_AXI_MEM_BUS_ADDR_WIDTH, even for
# ZynqM devices.
#
# To enable 64-bit addresses, set the following option to ON:
add_option_bool(FREDS_64BIT_ADDRESSES OFF "Use 64-bit addresses for AXI memory bus width")

# Logger level
add_option_numbered_choice(FREDS_LOG_LEVEL "mute" "mute;simple;full;pedantic" "Log level for the server")

# Fred configuration path
add_option_string(FREDS_PATH "/opt/fredsys" "The path where fred-server looks for hardware binaries and configuration")

# ------------------------- CONFIGURATION TYPES ------------------------- #

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type not specified: defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# ------------------------- COMPILATION OPTIONS ------------------------- #

# TODO: add option for LOG_GLOBAL_LEVEL and HW_TASKS_A64

set_target_properties(${PROJECT_NAME} PROPERTIES
    LANGUAGE C
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-but-set-variable
    # -Wno-variadic-functions
    # -Wno-long-long
    # -Wno-variadic-macros
)

# Select log level
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        LOG_GLOBAL_LEVEL=${FREDS_LOG_LEVEL_VALUE}
        FRED_PATH="${FREDS_PATH}"
)

# Add compile definition for 64-bit address support
if (FREDS_64BIT_ADDRESSES)
target_compile_definitions(${PROJECT_NAME} PRIVATE HW_TASKS_A64)
endif()

# 64-bit addresses is supported by FRED, but currently not by DART.
# Therefore, we stick with 32 bits C_M_AXI_MEM_BUS_ADDR_WIDTH even for ZynqM devices
# The second implication is that FRED server MUST BE compiled without the define `HW_TASKS_A64`, to make it compatible with 32 bits

# -------------------------- TARGET PROPERTIES -------------------------- #

# TODO: change FRED_PATH to be an option

message(STATUS "install path: ${CMAKE_INSTALL_PREFIX}")

# Select default installation path (NOTE: works only if fred is built in
# isolation, not as a dependency of another project)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/opt/fredsys/"
           CACHE PATH "default install path" FORCE)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/hw_support
        ${CMAKE_CURRENT_SOURCE_DIR}/shared_kernel
        ${CMAKE_CURRENT_SOURCE_DIR}/shared_user
        ${CMAKE_CURRENT_SOURCE_DIR}/srv_core
        ${CMAKE_CURRENT_SOURCE_DIR}/srv_core_mocks
        ${CMAKE_CURRENT_SOURCE_DIR}/srv_support
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# ------------------------- INSTALL PROPERTIES -------------------------- #

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/libfred)

# Export targets to install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
