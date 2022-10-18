set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PROJECT_PLATFORM "__RV1126_ENV__")

set(SYSROOT_PATH  /opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

SET(CMAKE_FIND_ROOT_PATH  /opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot)
SET(ENV{PKG_CONFIG_LIBDIR} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig/)

set(TOOLCHAIN_PATH /opt/rv1126_sdk/cross-compiler/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf)

set(PKG_CONFIG_EXECUTABLE "/opt/host/bin/pkg-config")
#set(PKG_CONFIG_PATH /opt/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/pkgconfig)

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

include_directories(${PROJECT_NAME} /opt/rv1126_sdk/thirdparty/ffmpeg-libs/x264/include)

set(SERVER_3RDPARTY_PATH "/opt/rv1126_sdk/thirdparty")

set(DRM_INCLUDE_DIRS "/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/drm")
include_directories(${DRM_INCLUDE_DIRS})

set(RGA_INCLUDE_DIRS "/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rga")
include_directories(${RGA_INCLUDE_DIRS})

set(YOUR_TARGET_OS linux)
set(YOUR_TARGET_ARCH arm32)

add_link_options("LINKER:-rpath-link,/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/lib:/opt/rv1126_sdk/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

