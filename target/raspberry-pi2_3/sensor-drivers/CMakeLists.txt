cmake_minimum_required(VERSION 2.8)
PROJECT(SENSOR_DRIVER_RPI)

include(FindPkgConfig)

#dependencies
FIND_PACKAGE(PkgConfig)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE RELEASE)

set(DRIVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/drivers)
set(driver_sources ${DRIVER_SOURCE_DIR}/driver_common.c ${DRIVER_SOURCE_DIR}/sensor1_acc.c ${DRIVER_SOURCE_DIR}/sensor2_temp.c ${DRIVER_SOURCE_DIR}/sensor3_sound.c ${DRIVER_SOURCE_DIR}/sensor4_light.c ${DRIVER_SOURCE_DIR}/sensor5_motion.c ${DRIVER_SOURCE_DIR}/sensor6_vib.c ${DRIVER_SOURCE_DIR}/sensor7_touch.c)

ADD_DEFINITIONS(-Wl,-Wall -O2 -fPIC -rdynamic)
ADD_LIBRARY(sensors SHARED ${driver_sources})

INCLUDE_DIRECTORIES( 
		${CMAKE_CURRENT_SOURCE_DIR}/drivers
		/usr/local/include
		/usr/include
)

LINK_DIRECTORIES(
		/usr/local/lib/pkgconfig
		/usr/local/lib
)

set_target_properties( sensors
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/sensor-drivers"
)

add_custom_target(
  make-sensor-driver-dir ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/sensor-drivers
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/SENSOR_LIST ${CMAKE_SOURCE_DIR}/out/sensor-drivers/SENSOR_LIST
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/sensor_config.json ${CMAKE_SOURCE_DIR}/out/sensor-drivers/sensor_config.json
  COMMENT "make sensor driver directory"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
