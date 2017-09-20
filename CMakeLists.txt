cmake_minimum_required(VERSION 2.8)

add_custom_target(
  out-dir ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/bin
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/bin/api
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/config
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/libs
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/sensor-drivers
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/system-apps
  COMMENT "Make out directory"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_subdirectory(framework)
add_subdirectory(api)
add_subdirectory(apps/system)
