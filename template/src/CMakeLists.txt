CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/public/buffer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/calculate)

set(EXE_NAME demo_template)

file(GLOB_RECURSE SrcHeaderFiles  "${CMAKE_CURRENT_SOURCE_DIR}/public/buffer/*.hpp" 
                                  "${CMAKE_CURRENT_SOURCE_DIR}/public/buffer/*.h"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/calculate/buffer/*.hpp")

file(GLOB_RECURSE SrcSrcFiles  "${CMAKE_CURRENT_SOURCE_DIR}/public/*.cpp"
                               "${CMAKE_CURRENT_SOURCE_DIR}/main/*.cpp")


# add_compile_options(-Wno-stringop-overflow -Wno-deprecated-declarations)

add_executable(${EXE_NAME}
	${SrcHeaderFiles}          #功能文件夹的头文件
	${SrcSrcFiles}             #功能软件的所有源文件
	)

LINK_DIRECTORIES(
    ${SPDLOG_LIBRARY_DIR}
    ${COMMON_LIBRARY_DIR}
)

# 链接库文件
target_link_libraries(${EXE_NAME}	
    ${GTEST_LIBS}               #链接gtest等库gmock gtest pthread
	
)

#安装可执行文件到bin目录
INSTALL(TARGETS ${EXE_NAME}
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin)


MESSAGE(STATUS "测试CMakeLists.txt完成")
