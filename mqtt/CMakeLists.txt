cmake_minimum_required (VERSION 3.8)

project ("MqttTest")

SET(MQTT_LIBS wolfmqtt pthread)

include_directories(${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "${cmake_source_dir}/wolfmqtt")

link_directories(${PROJECT_SOURCE_DIR}/ib)
MESSAGE(STATUS "${cmake_source_dir}/lib")

add_executable(MQTTtest main.cpp)
target_link_libraries(MQTTtest ${MQTT_LIBS})



