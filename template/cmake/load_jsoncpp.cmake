# Once done, this will define
#
#  JSON_CPP_INCLUDE_DIR - the JSON_CPP include directory
#  JSON_CPP_LIBRARY_DIR - the JSON_CPP library directory
#  JSON_CPP_LIBS - link these to use JSON_CPP
#
#  ......

MACRO(LOAD_LIBJSON_CPP os arch)
    #MESSAGE(INFO "--------------------------------")
    #MESSAGE(STATUS "3RDPARTY_DIR: ${3RDPARTY_DIR}")
    #FIND_FILE(SPDLOG_INCLUDE_DIR include ${3RDPARTY_DIR} NO_DEFAULT_PATH)
    #FIND_FILE(SPDLOG_LIBRARY_DIR lib ${3RDPARTY_DIR} NO_DEFAULT_PATH)
    IF(DEFINED SERVER_3RDPARTY_PATH)
        SET(JSON_CPP_INCLUDE_DIR ${SERVER_3RDPARTY_PATH}/jsoncpp/include)
        SET(JSON_CPP_LIBRARY_DIR ${SERVER_3RDPARTY_PATH}/jsoncpp/lib)
    ELSE()
        SET(3RDPARTY_DIR ${PROJECT_SOURCE_DIR}/3rdparty/target)
        SET(JSON_CPP_INCLUDE_DIR ${3RDPARTY_DIR}/${${os}}_${${arch}}/include/json-cpp)
        SET(JSON_CPP_LIBRARY_DIR ${3RDPARTY_DIR}/${${os}}_${${arch}}/lib/json-cpp)
        MESSAGE(STATUS "3RDPARTY_DIR: ${3RDPARTY_DIR}")
    ENDIF()

    IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
        SET(JSON_CPP_LIBS jsoncpp)
    ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
        SET(JSON_CPP_LIBS jsoncpp)
    ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
ENDMACRO()
