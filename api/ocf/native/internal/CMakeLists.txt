cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(OCF_NATIVE_INTERNAL)

add_library(ocf_native SHARED ocf_native_internal.c)

include_directories("${CMAKE_SOURCE_DIR}/../iotivity/include")
target_link_libraries(ocf_native ${CMAKE_SOURCE_DIR}/../iotivity/port/linux/libiotivity-lite-client-server.so pthread)