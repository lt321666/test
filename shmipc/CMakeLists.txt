cmake_minimum_required (VERSION 3.8)

project ("shmipcTest")

SET(SHM_LIBS ShmIPCSubApi ShmIPCPubApi pthread)

include_directories(${PROJECT_SOURCE_DIR}/shmipc_include)
MESSAGE(STATUS "${cmake_source_dir}/shmipc_include")

link_directories(${PROJECT_SOURCE_DIR}/shmipc_lib)
MESSAGE(STATUS "${cmake_source_dir}/shmipc_lib")

add_executable(shmtest main.cpp)
target_link_libraries(shmtest ${SHM_LIBS})



