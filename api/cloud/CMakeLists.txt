cmake_minimum_required(VERSION 2.8)

add_custom_target(
  cloud ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/out/bin/api/cloud-api
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/index.js ${CMAKE_SOURCE_DIR}/out/bin/api/cloud-api
  COMMENT "Building Cloud API"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
