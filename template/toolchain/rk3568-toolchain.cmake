set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(SYSROOT_PATH  /opt/rk3568_sdk/host/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

set(TOOLCHAIN_PATH /opt/rk3568_sdk/host/bin)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/aarch64-linux)

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

set(YOUR_TARGET_OS linux)
set(YOUR_TARGET_ARCH aarch64)

set(SERVER_3RDPARTY_PATH "/opt/rk3568_sdk/thirdparty")

add_link_options("LINKER:-rpath-link,/opt/rk3568_sdk/host/aarch64-buildroot-linux-gnu/sysroot/lib:/opt/rk3568_sdk/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib:/opt/rk3568_sdk/thirdparty/opencv/lib")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

