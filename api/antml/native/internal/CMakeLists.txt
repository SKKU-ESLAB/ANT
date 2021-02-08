cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_ML_INTERNAL)

add_library(ant_ml_internal SHARED ant_ml_internal.c)
