cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(OCF_INTERNAL)

add_library(ocf SHARED ocf_adapter_internal.c ocf_resource_internal.c)

include_directories("${CMAKE_SOURCE_DIR}/../iotivity" "${CMAKE_SOURCE_DIR}/../iotivity/include" "${CMAKE_SOURCE_DIR}/../iotivity/port/linux")
target_link_libraries(ocf ${CMAKE_SOURCE_DIR}/../iotivity/port/linux/libiotivity-lite-client-server.so pthread)