cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_ML_NATIVE_INTERNAL)

add_library(ant_ml_native SHARED ant_ml_native_internal.c)