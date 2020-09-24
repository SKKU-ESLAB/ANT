cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_RUNTIME_NATIVE_INTERNAL)

add_library(ant_runtime_native SHARED ant_runtime_native_internal.c)