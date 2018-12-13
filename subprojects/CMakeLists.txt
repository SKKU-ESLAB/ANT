cmake_minimum_required(VERSION 2.8)

if(CONFIG_SENSOR_FW STREQUAL "y")
  add_subdirectory(new-sensor)
endif()