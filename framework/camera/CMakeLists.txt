cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

set(CAMFW_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_definitions(`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1 gstreamer-1.0`)

add_executable(ant-camera ${CAMFW_SRC_DIR}/CameraManager.cpp ${CAMFW_SRC_DIR}/CameraController.cpp ${CAMFW_SRC_DIR}/CameraCommunicator.cpp ${CAMFW_SRC_DIR}/CameraConfigParser.cpp ${CAMFW_SRC_DIR}/CameraDevice.cpp ${CAMFW_SRC_DIR}/CameraRequest.cpp ${CAMFW_SRC_DIR}/cJSON.c ${CAMFW_SRC_DIR}/ANTRawRequest.cpp)

target_link_libraries(ant-camera dbus-1 glib-2.0 dbus-glib-1 gstreamer-1.0 gstapp-1.0 gobject-2.0 gmodule-2.0 pthread)

set_target_properties(ant-camera
		PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
