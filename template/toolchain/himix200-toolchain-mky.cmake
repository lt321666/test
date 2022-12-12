MESSAGE(STATUS "Configure Cross Compiler")

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER        /opt/hisi-linux/x86-arm/arm-himix200-linux/host_bin/arm-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER      /opt/hisi-linux/x86-arm/arm-himix200-linux/host_bin/arm-linux-gnueabi-g++)

# set searching rules for cross-compiler
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(YOUR_TARGET_OS linux)
SET(YOUR_TARGET_ARCH arm_himix200)

SET(CMAKE_SKIP_BUILD_RPATH TRUE)
SET(CMAKE_SKIP_RPATH TRUE)

SET(SERVER_3RDPARTY_PATH "/opt/hisi-linux/thirdparty")

# set ${CMAKE_C_FLAGS} and ${CMAKE_CXX_FLAGS}flag for cross-compiled process
#SET(CROSS_COMPILATION_ARM himix200)
#SET(CROSS_COMPILATION_ARCHITECTURE armv7-a)

# set g++ param
# -fopenmp link libgomp
#SET(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4 \
#    -ffunction-sections \
#    -fdata-sections -O2 -fstack-protector-strong -lm -ldl -lstdc++\
#    ${CMAKE_CXX_FLAGS}")
