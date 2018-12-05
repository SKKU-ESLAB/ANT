cmake_minimum_required(VERSION 2.8)

include(${CMAKE_SOURCE_DIR}/kconfig.cmake)
import_kconfig(${CMAKE_SOURCE_DIR}/.config)

option(VIRTUAL_SENSOR "Use virtual sensor when there's no sensor" OFF)

add_custom_target(
  out-dir ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/bin
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/bin/api
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/config
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/libs
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/device-drivers
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/apps/system
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/apps/user
  COMMENT "Make output directory"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_subdirectory(framework)
add_subdirectory(api)
add_subdirectory(new-api)
add_subdirectory(apps)
add_subdirectory(models)

if(CONFIG_TARGET_RASPBERRY_PI_2_3 STREQUAL y)
  add_subdirectory(target/raspberry-pi2_3/device-drivers)
elseif(CONFIG_TARGET_TEGRA_TX STREQUAL y)
  add_subdirectory(target/tegraTX1/device-drivers)
elseif(CONFIG_TARGET_OTHER STREQUAL y)
  add_subdirectory(target/other/device-drivers)
endif()