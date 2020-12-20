cmake_minimum_required(VERSION 2.8)
set(NAME antruntime)

set(IOTJS_INCLUDE_DIR "$IOTJS_PATH$/include")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_library(${NAME} SHARED
    ./ant_runtime_native.c)

add_subdirectory(${MODULE_DIR}/internal/ ${CMAKE_BINARY_DIR}/out/antruntime/)

target_include_directories(${NAME}
    PRIVATE ${IOTJS_INCLUDE_DIR} ${JERRY_INCLUDE_DIR})
set_target_properties(${NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PREFIX ""
    SUFFIX ".node")
