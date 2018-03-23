cmake_minimum_required(VERSION 2.8)
PROJECT(SENSOR_DRIVER_TegraTX1)

include(FindPkgConfig)

#dependencies
FIND_PACKAGE(PkgConfig)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE RELEASE)

set(DRIVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sensor-drivers)
set(ACTUATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/actuator-drivers)


set(sensor_driver_sources 
  ${DRIVER_SOURCE_DIR}/jetsonGPIO.c 
  ${DRIVER_SOURCE_DIR}/sensor0_button.c
  )
set(actuator_driver_sources 
  ${ACTUATOR_SOURCE_DIR}/actuator1_virt_act1.c
  )

ADD_DEFINITIONS(-Wl,-Wall -O2 -fPIC -rdynamic)
ADD_LIBRARY(sensors SHARED ${sensor_driver_sources})
ADD_LIBRARY(actuators SHARED ${actuator_driver_sources})

INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}/sensor-drivers
		/usr/local/include
		/usr/include
)

LINK_DIRECTORIES(
		/usr/local/lib/pkgconfig
		/usr/local/lib
)

set_target_properties( sensors
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/out/sensor-drivers)

set_target_properties( actuators
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/out/sensor-drivers)

add_custom_target(
	make-sensor-driver-dir ALL
	COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/device-drivers	
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/sensor_config.json ${CMAKE_SOURCE_DIR}/out/device-drivers/sensor_config.json
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/actuator_config.json ${CMAKE_SOURCE_DIR}/out/device-drivers/actuator_config.json
	COMMENT "make sensor driver directory"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
