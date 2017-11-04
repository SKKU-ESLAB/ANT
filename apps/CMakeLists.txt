cmake_minimum_required(VERSION 2.8)

add_custom_target(
  system-apps ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/system-apps
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/system/* ${CMAKE_SOURCE_DIR}/out/system-apps/
  COMMENT "Copy system apps"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
