cmake_minimum_required(VERSION 2.8)

if(CONFIG_CAMERA_JS_API STREQUAL "y")
  add_subdirectory(camera)
endif()

if(CONFIG_SENSOR_JS_API STREQUAL "y")
  add_subdirectory(sensor)
endif()