cmake_minimum_required(VERSION 3.16)

# ------------------------- PROJECT INFORMATION ------------------------- #

project(libfred VERSION 1.0)


# ---------------------------- SOURCE FILES ----------------------------- #

set(libfred_sources
    fred_lib.c
    user_buff.c
)

set(libfred_headers
    fred_lib.h
)

set(libfredcpp_headers
    fredcpp.h
)

# --------------------------- PROJECT TARGETS --------------------------- #

# This is the object library, builds the sources only once
add_library(libfred_object OBJECT)

target_sources(libfred_object
    PRIVATE ${libfred_sources}
)

# Shared and static libraries built from the same object
# library and include its interface
add_library(libfred_shared SHARED)
add_library(libfred_static STATIC)
target_link_libraries(libfred_shared PUBLIC libfred_object)
target_link_libraries(libfred_static PUBLIC libfred_object)

# C++ library, it is a header-only library that links to the C library
add_library(libfredcpp INTERFACE)
add_library(libfredcpp_shared INTERFACE)
add_library(libfredcpp_static INTERFACE)
target_link_libraries(libfredcpp_shared INTERFACE libfred_shared libfredcpp)
target_link_libraries(libfredcpp_static INTERFACE libfred_static libfredcpp)

# Aliases, used to hide original targets when importing the library
add_library(FredFramework::libfred_object ALIAS libfred_object)
add_library(FredFramework::libfred_shared ALIAS libfred_shared)
add_library(FredFramework::libfred_static ALIAS libfred_static)
add_library(FredFramework::libfredcpp ALIAS libfredcpp)
add_library(FredFramework::libfredcpp_shared ALIAS libfredcpp_shared)
add_library(FredFramework::libfredcpp_static ALIAS libfredcpp_static)

# ------------------------- CONFIGURATION TYPES ------------------------- #

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type not specified: defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# ------------------------- COMPILATION OPTIONS ------------------------- #

target_compile_features(libfred_object PUBLIC c_std_99)
target_compile_options(libfred_object PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    # -Wno-variadic-functions
    # -Wno-long-long
    # -Wno-variadic-macros
)

target_compile_features(libfredcpp INTERFACE cxx_std_17)
target_compile_features(libfredcpp_shared INTERFACE cxx_std_17)
target_compile_features(libfredcpp_static INTERFACE cxx_std_17)

# -------------------------- TARGET PROPERTIES -------------------------- #

# Since we use it to build a shared library, we must force PIC
set_target_properties(libfred_object PROPERTIES
    LANGUAGE C
    POSITION_INDEPENDENT_CODE 1
)

# Set headers to install
set_target_properties(libfred_object PROPERTIES PUBLIC_HEADER ${libfred_headers})
set_target_properties(libfredcpp PROPERTIES PUBLIC_HEADER ${libfredcpp_headers})

target_include_directories(libfred_object
    PUBLIC
        $<INSTALL_INTERFACE:>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(libfredcpp
    INTERFACE
        $<INSTALL_INTERFACE:>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Give the libraries the same output name "libfred.so/.a"
set_target_properties(libfred_shared PROPERTIES LIBRARY_OUTPUT_NAME "fred")
set_target_properties(libfred_static PROPERTIES ARCHIVE_OUTPUT_NAME "fred")

# ------------------------- INSTALL PROPERTIES -------------------------- #

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/libfred)

# Export all libraries targets
install(TARGETS
    libfred_shared libfred_static libfred_object
    libfredcpp_shared libfredcpp_static libfredcpp
  EXPORT libfred-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install file containing all project targets
install(EXPORT libfred-targets
    FILE libfredTargets.cmake
    NAMESPACE FredFramework::
    DESTINATION ${INSTALL_CONFIGDIR}
)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libfredConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Substitute minimum version provided by the config file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/libfredConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libfredConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libfredConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libfredConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

# ---------------------------- TEST TARGETS ----------------------------- #
# add_executable(sum_vec sum_vec.cpp)
# set_target_properties(sum_vec PROPERTIES EXCLUDE_FROM_ALL TRUE)
# target_link_libraries(sum_vec PRIVATE libfredcpp_static)
