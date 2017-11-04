cmake_minimum_required(VERSION 2.8)

option(VIRTUAL_SENSOR "Use virtual sensor when there's no sensor" OFF)

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
add_subdirectory(new-api)
add_subdirectory(apps)
add_subdirectory(models)

if(TARGET_BOARD STREQUAL raspberry-pi)
  add_subdirectory(target/raspberry-pi2_3/sensor-drivers)
elseif(TARGET_BOARD STREQUAL tegraTX)
  add_subdirectory(target/tegraTX1/sensor-drivers)
else()
  add_subdirectory(target/virtual-sensor-drivers)
  
endif()
