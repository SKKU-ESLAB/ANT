cmake_minimum_required(VERSION 2.8)

add_subdirectory(message)

add_subdirectory(appcore)

if(CONFIG_CAMERA_FW STREQUAL "y")
  add_subdirectory(camera)
endif()

if(CONFIG_COMM_FW STREQUAL "y")
  add_subdirectory(communication)
endif()

if(CONFIG_SENSOR_FW STREQUAL "y")
  add_subdirectory(sensor)
  add_subdirectory(new-sensor)
endif()

if(CONFIG_ML_FW STREQUAL "y")
add_subdirectory(machine-learning)
endif()
