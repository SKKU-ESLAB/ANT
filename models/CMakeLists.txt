cmake_minimum_required(VERSION 2.8)

add_custom_target(
  models ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./accelerometer.net ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./squeezenet_v1.1.caffemodel ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./squeezenet_v1.1_deploy.prototxt ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./ilsvrc12_synset_words.txt ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./ilsvrc12_imagenet_mean.binaryproto ${CMAKE_SOURCE_DIR}/out/models
  COMMENT "Copy built-in machine learning models"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
