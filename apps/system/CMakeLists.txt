cmake_minimum_required(VERSION 2.8)

if(CONFIG_CAMERA_VIEWER_SYSTEM_APP STREQUAL y)
add_custom_target(
  camera-viewer-system-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/CameraViewer ${CMAKE_SOURCE_DIR}/out/apps/system/
  
  COMMENT "Copy Camera Viewer system application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_SENSOR_VIEWER_SYSTEM_APP STREQUAL y)
add_custom_target(
  sensor-viewer-system-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/SensorViewer ${CMAKE_SOURCE_DIR}/out/apps/system/
  
  COMMENT "Copy Sensor Viewer system application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_MOTION_CLASSIFIER_SYSTEM_APP STREQUAL y)
add_custom_target(
  motion-classifier-system-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/MotionClassifier ${CMAKE_SOURCE_DIR}/out/apps/system/
  
  COMMENT "Copy Motion Classifier system application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_IMAGE_CLASSIFIER_SYSTEM_APP STREQUAL y)
add_custom_target(
  image-classifier-system-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/ImageClassifier ${CMAKE_SOURCE_DIR}/out/apps/system/
  
  COMMENT "Copy Image Classifier system application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()