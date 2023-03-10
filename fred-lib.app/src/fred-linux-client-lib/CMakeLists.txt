cmake_minimum_required(VERSION 3.0)
project(fred_lib LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# default installation path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/opt/fredsys/"
           CACHE PATH "default install path" FORCE)
endif()

#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Werror -fpic -DFRED_PATH=${CMAKE_INSTALL_PREFIX}")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(fred SHARED
    fred_lib.c
    user_buff.c
)

add_library(fred_s STATIC
    fred_lib.c
    user_buff.c
)

set_target_properties(fred PROPERTIES PUBLIC_HEADER "fred_lib.h")

install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/libfred.so 
    ${CMAKE_CURRENT_BINARY_DIR}/libfred_s.a
         DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/fred_lib.h
         DESTINATION include
)

########################################################################
# Install cmake search helper for this library
########################################################################
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# install(FILES cmake/fredConfig.cmake
#     DESTINATION ${CMAKE_MODULES_DIR}/fred
# )