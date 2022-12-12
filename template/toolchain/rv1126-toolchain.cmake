set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PROJECT_PLATFORM "__RV1126_ENV__")

set(SYSROOT_PATH  /opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

SET(CMAKE_FIND_ROOT_PATH  /opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot)
SET(ENV{PKG_CONFIG_LIBDIR} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig/)

set(TOOLCHAIN_PATH /opt/rv1126_sdk/host)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf)

SET(ENV{PKG_CONFIG_PATH} ${SYSROOT_PATH}/usr/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
SET(PKG_CONFIG_EXECUTABLE "/opt/rv1126_sdk/host/bin/pkg-config")

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

set(SERVER_3RDPARTY_PATH "/opt/rv1126_sdk/thirdparty")

set(YOUR_TARGET_OS linux)
set(YOUR_TARGET_ARCH arm32)

add_link_options("LINKER:-rpath-link,/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/lib:/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

