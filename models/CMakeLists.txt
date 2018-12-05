cmake_minimum_required(VERSION 2.8)

if(CONFIG_ML_FW STREQUAL "y")
add_custom_target(
  models ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/models
  COMMENT "Make a directory for machine learning models"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_SQUEEZENET_DNN_MODEL STREQUAL "y")
add_custom_target(
  squeezenet-dnn-model ALL
  COMMAND cp ./squeezenet_v1.1.caffemodel ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./squeezenet_v1.1_deploy.prototxt ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./ilsvrc12_synset_words.txt ${CMAKE_SOURCE_DIR}/out/models
  COMMAND cp ./ilsvrc12_imagenet_mean.binaryproto ${CMAKE_SOURCE_DIR}/out/models
  COMMENT "Copy built-in SqueezeNet model based on Caffe"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

add_dependencies(squeezenet-dnn-model models)

if(CONFIG_MOTION_CLASSIFIER_ANN_MODEL STREQUAL "y")
add_custom_target(
  motion-classifier-ann-model ALL
  COMMAND cp ./accelerometer.net ${CMAKE_SOURCE_DIR}/out/models
  COMMENT "Copy built-in Motion Classifier model based on ANN"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

add_dependencies(motion-classifier-ann-model models)