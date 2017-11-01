cmake_minimum_required(VERSION 2.8)

add_custom_target(
  models ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./accelerometer.net ${CMAKE_SOURCE_DIR}/out/models
  COMMENT "Copy built-in machine learning models"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
