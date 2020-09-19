cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(OCF_NATIVE_INTERNAL)

add_library(ocf_native SHARED ocf_native_internal.c)
message("cmake_source_dir: ${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/../iotivity/include")
link_directories("${CMAKE_SOURCE_DIR}/../iotivity/port/linux")
target_link_libraries(ocf_native iotivity-lite-client-server)