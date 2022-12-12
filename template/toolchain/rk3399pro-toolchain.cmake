set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PROJECT_PLATFORM "__RK3399_ENV__")

set(SYSROOT_PATH  /opt/rk3399pro_sdk/host/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

SET(CMAKE_FIND_ROOT_PATH  ${SYSROOT_PATH})
#SET(ENV{PKG_CONFIG_LIBDIR} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig/)

set(TOOLCHAIN_PATH /usr)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/bin/aarch64-linux-gnu)

#set(PKG_CONFIG_EXECUTABLE "/opt/host/bin/pkg-config")
#set(PKG_CONFIG_PATH /opt/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/pkgconfig)

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(YOUR_TARGET_OS linux)
set(YOUR_TARGET_ARCH aarch64)

set(SERVER_3RDPARTY_PATH "/opt/rk3399pro_sdk/thirdparty")
set(FOLDER_NAME "rk3399")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

add_link_options("LINKER:-rpath-link,/opt/rk3399pro_sdk/host/aarch64-buildroot-linux-gnu/sysroot/lib:/opt/rk3399pro_sdk/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib:/opt/rk3399pro_sdk/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib/aarch64-linux-gnu:/mnt/e/VMEnv/maxvision/firefly_env/sysroot/opt/opencv4.4/lib")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

