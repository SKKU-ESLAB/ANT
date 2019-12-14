cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_COMPANION_NATIVE_INTERNAL)

add_library(ant_companion_native SHARED ant_companion_native_internal.c)