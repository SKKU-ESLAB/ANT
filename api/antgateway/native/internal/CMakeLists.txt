cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_GATEWAY_INTERNAL)

add_library(ant_gateway_internal SHARED ant_gateway_dfe_internal.c)
target_link_libraries(ant_gateway_internal python3.6m)
