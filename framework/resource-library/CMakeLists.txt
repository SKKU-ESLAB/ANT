include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cmfw-library/inc)
link_directories(${CMAKE_SOURCE_DIR}/out/libs)

add_definitions("`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`")

set(CMAKE_CXX_STANDARD 11)
set(MESSAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(MESSAGE_sources
  ${MESSAGE_SOURCE_DIR}/BaseMessage.cpp
  ${MESSAGE_SOURCE_DIR}/LocalResourceManager.cpp
  ${MESSAGE_SOURCE_DIR}/NativeResourceAPI.cpp
  ${MESSAGE_SOURCE_DIR}/ResourceRequest.cpp
  ${MESSAGE_SOURCE_DIR}/ResourceResponse.cpp
  ${MESSAGE_SOURCE_DIR}/AppMessage.cpp
  ${MESSAGE_SOURCE_DIR}/AppCoreMessage.cpp
  ${MESSAGE_SOURCE_DIR}/CompanionMessage.cpp
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

add_library(ant-resource SHARED ${MESSAGE_sources})
add_dependencies(ant-resource ant-cmfw)

target_link_libraries(ant-resource
  ant-cmfw
  dbus-glib-1 dbus-1 glib-2.0 pthread
  bluetooth)

set_target_properties(ant-resource
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/libs
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/libs
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin
  COMPILE_FLAGS "-std=c++11")
