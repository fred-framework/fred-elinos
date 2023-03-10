INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FRED fred)

FIND_PATH(
    FRED_INCLUDE_DIRS
    NAMES fred.h
    HINTS $ENV{FRED_DIR}/include
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FRED_LIBRARIES
    NAMES fred
    HINTS $ENV{FRED_DIR}/lib
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

include("${CMAKE_CURRENT_LIST_DIR}/fredTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FRED DEFAULT_MSG FRED_LIBRARIES FRED_INCLUDE_DIRS)
MARK_AS_ADVANCED(FRED_LIBRARIES FRED_INCLUDE_DIRS)

