# CMakeList.txt: sqlite 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
SET(SQLITE3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
MESSAGE(STATUS "cmakeAdd:  ${SQLITE3_INCLUDE_DIR}")

SET(SQLITE3_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR} )
LINK_DIRECTORIES(${SQLITE3_LIBRARY_DIR})
# 将源代码添加到此项目的可执行文件。
# 递归遍历头文件
file(GLOB_RECURSE SrcHeaderFiles "${SQLITE3_INCLUDE_DIR}/*.hpp" "${SQLITE3_INCLUDE_DIR}/*.h")
# 递归遍历源文件
file(GLOB_RECURSE SrcSrcFiles "${SQLITE3_INCLUDE_DIR}/*.cpp" "${SQLITE3_INCLUDE_DIR}/*.c")

add_executable (sqlite "sqlite.cpp" ${SrcHeaderFiles} ${SrcSrcFiles} )

# TODO: 如有需要，请添加测试并安装目标。
target_link_libraries(sqlite pthread dl sqlite3)
INSTALL(TARGETS sqlite DESTINATION  ${CMAKE_SOURCE_DIR}/out/bin)
MESSAGE(STATUS "测试CMakeLists.txt完成")