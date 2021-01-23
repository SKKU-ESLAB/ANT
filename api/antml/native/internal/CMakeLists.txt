cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

project(ANT_ML_INTERNAL)

add_library(ant_ml_internal SHARED ant_ml_internal.c ant_ml_dfe_internal.c)
target_link_libraries(ant_ml_internal python3.6m)
