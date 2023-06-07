cmake_minimum_required(VERSION 3.16)
project(fredspeedtest)

include(cmake/CPM.cmake)

# FRED dependency
CPMAddPackage(
    NAME libfred
    GIT_REPOSITORY https://github.com/fred-framework/fred-linux-client-lib.git
    GIT_TAG master # Do not care for version numbers for now
)

if (NOT libfred_ADDED STREQUAL "YES")
    message(FATAL_ERROR "Required dependency libfred not found!")
endif()


add_executable(fredspeedtest
    fredspeedtest.cpp
)

set_target_properties(fredspeedtest PROPERTIES
    LANGUAGE CXX
)

target_compile_features(fredspeedtest PUBLIC cxx_std_20)
target_compile_options(fredspeedtest PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wno-variadic-functions
    -Wno-long-long
    -Wno-variadic-macros
)

target_link_libraries(fredspeedtest FredFramework::libfredcpp_static)


include(GNUInstallDirs)

# Export targets to install
install(TARGETS fredspeedtest
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
