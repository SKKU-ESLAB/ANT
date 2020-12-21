cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(COAP_INTERNAL)

# To support build at 64-bit machines
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-conversion")

add_library(coap_internal SHARED coap_internal.c)

target_link_libraries(coap_internal coap-2-openssl)
