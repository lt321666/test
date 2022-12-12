CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)


project(template)

# RK3568 need standard 17
set(CMAKE_CXX_STANDARD 17)
# SET TRUE IF NEED TO BUILD EXAMPLE OR FALSE
set(EXAMPLE_REQUIRED FALSE)

set(LIB_VERSION 0.0.2)
set(LIB_SOVERSION 0)


OPTION(BUILD_TESTS "Build with tests" ON)

IF(BUILD_TESTS)
    INCLUDE(cmake/load_gtest.cmake)
    LOAD_LIBGTEST(YOUR_TARGET_OS YOUR_TARGET_ARCH)
    MESSAGE(INFO "YOUR_TARGET_OS ${YOUR_TARGET_OS}")   
    include_directories(${GTEST_INCLUDE_DIR})
    link_directories(${GTEST_LIBRARY_DIR})
    MESSAGE(INFO "----------BUILD_TESTS 开启----------------------")

ENDIF()

INCLUDE(cmake/load_jsoncpp.cmake)

LOAD_LIBJSON_CPP(YOUR_TARGET_OS YOUR_TARGET_ARCH)

include_directories(${JSON_CPP_INCLUDE_DIR})
link_directories(${JSON_CPP_LIBRARY_DIR})

add_subdirectory(src)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/target)


IF(BUILD_TESTS)
    MESSAGE(INFO "----------编译测试文件----------------------")
    ADD_SUBDIRECTORY(tests)
ENDIF()


