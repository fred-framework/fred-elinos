cmake_minimum_required(VERSION 3.16)
project(chrt)

add_executable(${PROJECT_NAME}
    ${PROJECT_NAME}.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LANGUAGE CXX
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wno-variadic-functions
    -Wno-long-long
    -Wno-variadic-macros
)

include(GNUInstallDirs)

# Export targets to install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
