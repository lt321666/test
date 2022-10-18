include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIB_NAME btrace_api)

file(GLOB_RECURSE testHeaderFiles "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" "${CMAKE_SOURCE_DIR}/include/*.h")
set(INCLUDE_FILES_LIST
    ${testHeaderFiles}
)

set(OUTER_INCLUDE_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/backtrace/backtrace_helper.h)

set(BT_SOURCE_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/backtrace/backtrace_helper.c
)

ADD_LIBRARY(${LIB_NAME} SHARED ${INCLUDE_FILES_LIST} ${BT_SOURCE_FILES_LIST})
SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_SOVERSION})

target_link_libraries(${LIB_NAME} rt pthread)

INSTALL(TARGETS ${LIB_NAME} 
        LIBRARY DESTINATION lib
)

install (FILES ${OUTER_INCLUDE_FILES_LIST}
        DESTINATION include)
