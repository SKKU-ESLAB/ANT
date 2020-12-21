cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_RUNTIME_NATIVE_INTERNAL)

add_library(ant_runtime_native SHARED ant_runtime_native_internal.c)
target_link_libraries(ant_runtime_native curl)
target_link_libraries(ant_runtime_native tar)
