set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

#set(PROJECT_PLATFORM "__RV1126_ENV__")

set(SYSROOT_PATH  /opt/rk3588_sdk/host/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
#message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

#SET(CMAKE_FIND_ROOT_PATH  ${SYSROOT_PATH})
#SET(ENV{PKG_CONFIG_LIBDIR} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig/)

set(TOOLCHAIN_PATH  /opt/rk3588_sdk/host)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/bin/aarch64-linux)

#set(PKG_CONFIG_EXECUTABLE "/opt/host/bin/pkg-config")
#set(PKG_CONFIG_PATH /opt/rk3568_sdk/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib/pkgconfig)

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

set(SERVER_3RDPARTY_PATH "/opt/rk3588_sdk/thirdparty")

add_link_options("LINKER:-rpath-link,/opt/rk3588_sdk/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib:/opt/rk3588_sdk/host/aarch64-buildroot-linux-gnu/sysroot/lib")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

