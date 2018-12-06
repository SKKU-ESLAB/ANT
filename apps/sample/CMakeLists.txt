cmake_minimum_required(VERSION 2.8)

if(CONFIG_MACHINE_LEARNING_SAMPLE_APP STREQUAL y)
add_custom_target(
  machine-learning-sample-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/MachineLearning ${CMAKE_SOURCE_DIR}/out/apps/sample/
  
  COMMENT "Copy Machine Learning sample application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_QUICK_CAM_SAMPLE_APP STREQUAL y)
add_custom_target(
  quick-cam-sample-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/QuickCam ${CMAKE_SOURCE_DIR}/out/apps/sample/
  
  COMMENT "Copy Quick Camera sample application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

if(CONFIG_REMOTE_UI_SAMPLE_APP STREQUAL y)
add_custom_target(
  remote-ui-sample-app ALL
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/RemoteUI ${CMAKE_SOURCE_DIR}/out/apps/sample/
  
  COMMENT "Copy Remote UI sample application"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()