include_directories(./inc)
include_directories(../message/inc)
include_directories(../communication/inc)
link_directories(../../out/libs)

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

target_link_libraries(ant-ml
  ant-message ant-cmfw pthread fann doublefann fixedfann floatfann)

set_target_properties(ant-ml
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "../../out/bin")
