cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(OCF_INTERNAL)

# To support build at 64-bit machines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-conversion")

add_library(ocf SHARED ocf_adapter_internal.c ocf_resource_internal.c ant_async.c hashmap.c ll.c)

include_directories("${CMAKE_SOURCE_DIR}/../iotivity" "${CMAKE_SOURCE_DIR}/../iotivity/include" "${CMAKE_SOURCE_DIR}/../iotivity/port/linux" "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/deps/jerry/jerry-core/include" "${CMAKE_SOURCE_DIR}/deps/libtuv/include")
target_link_libraries(ocf ${CMAKE_SOURCE_DIR}/../iotivity/port/linux/libiotivity-lite-client-server.so pthread)               
