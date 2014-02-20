INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_E4406A e4406a)

FIND_PATH(
    E4406A_INCLUDE_DIRS
    NAMES e4406a/api.h
    HINTS $ENV{E4406A_DIR}/include
        ${PC_E4406A_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREEFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    E4406A_LIBRARIES
    NAMES gnuradio-e4406a
    HINTS $ENV{E4406A_DIR}/lib
        ${PC_E4406A_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(E4406A DEFAULT_MSG E4406A_LIBRARIES E4406A_INCLUDE_DIRS)
MARK_AS_ADVANCED(E4406A_LIBRARIES E4406A_INCLUDE_DIRS)

