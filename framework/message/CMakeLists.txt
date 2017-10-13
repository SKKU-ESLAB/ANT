include_directories(./inc)
include_directories(../communication/inc)
link_directories(../../out/libs)

add_definitions("`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`")

set(CMAKE_CXX_STANDARD 11)
set(MESSAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(MESSAGE_sources
  ${MESSAGE_SOURCE_DIR}/BaseMessage.cpp
  ${MESSAGE_SOURCE_DIR}/MLMessage.cpp
  ${MESSAGE_SOURCE_DIR}/MLDataUnit.cpp
  ${MESSAGE_SOURCE_DIR}/MLTensor.cpp
  ${MESSAGE_SOURCE_DIR}/MessageFactory.cpp
  ${MESSAGE_SOURCE_DIR}/Channel.cpp
  ${MESSAGE_SOURCE_DIR}/CommChannel.cpp
  ${MESSAGE_SOURCE_DIR}/DbusChannel.cpp
  ${MESSAGE_SOURCE_DIR}/LocalChannel.cpp
  ${MESSAGE_SOURCE_DIR}/MessageRouter.cpp
  ${MESSAGE_SOURCE_DIR}/cJSON.c)

add_library(ant-message SHARED ${MESSAGE_sources})

target_link_libraries(ant-message
  ant-cmfw
  dbus-glib-1 dbus-1 glib-2.0 pthread
  bluetooth)

set_target_properties(ant-message
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "../../out/libs"
  LIBRARY_OUTPUT_DIRECTORY "../../out/libs"
  RUNTIME_OUTPUT_DIRECTORY "../../out/bin"
  COMPILE_FLAGS "-std=c++11")
