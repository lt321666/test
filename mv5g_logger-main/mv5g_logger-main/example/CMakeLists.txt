set(EXM_NAME crash_test)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE testHeaderFiles "${CMAKE_SOURCE_DIR}/src/backtrace/*.h")
set(INCLUDE_FILES_LIST
    ${testHeaderFiles}
)

set(CMAKE_C_FLAGS "-std=gnu99 -g -rdynamic")

set(C_SOURCE_FILES_LIST
    CtrlCMain.c
)

link_directories(${CMAKE_SOURCE_DIR}/target/lib)

ADD_EXECUTABLE(${EXM_NAME} ${INCLUDE_FILES_LIST} ${C_SOURCE_FILES_LIST})

target_link_libraries(${EXM_NAME} btrace_api)

INSTALL(TARGETS ${EXM_NAME} 
        RUNTIME DESTINATION bin
)