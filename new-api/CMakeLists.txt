cmake_minimum_required(VERSION 2.8)

add_custom_target(
  new-api ALL
  COMMAND node-gyp configure
  COMMAND node-gyp build
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/build/Release/antnative.node ${CMAKE_CURRENT_SOURCE_DIR}/../out/bin/api
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/js/ant ${CMAKE_CURRENT_SOURCE_DIR}/../out/bin/api/
  COMMENT "Building New ANT API"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
add_dependencies(new-api ant-resource)