cmake_minimum_required(VERSION 2.8)
PROJECT(SENSOR_DRIVER_TegraTX1)

include(FindPkgConfig)

#dependencies
FIND_PACKAGE(PkgConfig)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE RELEASE)

set(DRIVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/drivers)
set(driver_sources ${DRIVER_SOURCE_DIR}/jetsonGPIO.c ${DRIVER_SOURCE_DIR}/sensor0_button.c)

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
	LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/out/sensor-drivers")

add_custom_target(
	make-sensor-driver-dir ALL
	COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/sensor-drivers	
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/SENSOR_LIST ${CMAKE_SOURCE_DIR}/out/sensor-drivers/SENSOR_LIST
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/sensor_config.json ${CMAKE_SOURCE_DIR}/out/sensor-drivers/sensor_config.json
	COMMENT "make sensor driver directory"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
