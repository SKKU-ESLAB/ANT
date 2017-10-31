include_directories(./inc)

set(CMAKE_CXX_STANDARD 11)
set(CMFW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(BluetoothCommPort OBJECT ${CMFW_SOURCE_DIR}/BluetoothCommPort.cpp)
add_library(WifiDirectCommPort OBJECT ${CMFW_SOURCE_DIR}/WifiDirectCommPort.cpp)
add_library(CommRawPacket OBJECT ${CMFW_SOURCE_DIR}/CommRawPacket.cpp)
add_library(DeviceController OBJECT ${CMFW_SOURCE_DIR}/DeviceController.cpp)
add_library(TmpControl OBJECT ${CMFW_SOURCE_DIR}/TmpControl.cpp)
add_library(ant-cmfw SHARED ${CMFW_SOURCE_DIR}/CommPort.cpp
  $<TARGET_OBJECTS:BluetoothCommPort>
  $<TARGET_OBJECTS:WifiDirectCommPort>
  $<TARGET_OBJECTS:CommRawPacket>
  $<TARGET_OBJECTS:DeviceController>
  $<TARGET_OBJECTS:TmpControl>)

target_link_libraries(ant-cmfw bluetooth)

set_target_properties(ant-cmfw
		PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/libs
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/libs
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin
		COMPILE_FLAGS "-std=c++11")
