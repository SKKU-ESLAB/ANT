find_library(WIRINGPI_LIB wiringPi)
find_library(WIRINGPI_DEV_LIB wiringPiDev)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../message/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../communication/inc)
link_directories(${CMAKE_SOURCE_DIR}/out/libs)

add_definitions("`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`")
add_definitions(-Wl,-Wall -O2 -fPIC -rdynamic -lwiringPi)

set(CMAKE_CXX_STANDARD 11)

set(SM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SM_SOURCES
	${SM_SOURCE_DIR}/main.cpp
	${SM_SOURCE_DIR}/SensorManager.cpp)

ADD_EXECUTABLE(ant-sensor ${SM_SOURCES})

TARGET_LINK_LIBRARIES(ant-sensor
	pthread ant-message ant-cmfw ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB})

SET_TARGET_PROPERTIES(ant-sensor
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)