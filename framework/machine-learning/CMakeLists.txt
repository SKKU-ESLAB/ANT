include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../message/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../communication/inc)
link_directories(${CMAKE_SOURCE_DIR}/out/libs)

add_definitions("`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`")

set(CMAKE_CXX_STANDARD 11)
set(MLFW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(MLFW_sources
  ${MLFW_SOURCE_DIR}/main.cpp
  ${MLFW_SOURCE_DIR}/MLDaemon.cpp
  ${MLFW_SOURCE_DIR}/ModelPackageLoader.cpp
  ${MLFW_SOURCE_DIR}/InputReaderSet.cpp
  ${MLFW_SOURCE_DIR}/CameraInputReader.cpp
  ${MLFW_SOURCE_DIR}/SensorInputReader.cpp
  ${MLFW_SOURCE_DIR}/InferenceUnit.cpp
  ${MLFW_SOURCE_DIR}/DNNInferenceRunner.cpp
  ${MLFW_SOURCE_DIR}/ANNInferenceRunner.cpp)

add_executable(ant-ml ${MLFW_sources})
add_dependencies(ant-ml ant-message)

target_link_libraries(ant-ml
  ant-message ant-cmfw pthread fann doublefann fixedfann floatfann)

set_target_properties(ant-ml
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
